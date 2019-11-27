/****************************************************************************
 *
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 * File Details
 * ------------
 *  $Id: son_mif_msg_handler.c 
 *
 ****************************************************************************
 *
 * File Description:  This file is the main entry point handler for 
 *                    SON MgmtIfH Module. It handles all the APIs from 
 *                    OAMAgent, processes the message and sends the responses
 *                    to the OAMAgent
 ****************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR       REFERENCE       REASON
 *   Mar, 2012       Ritika       Initial         Initial
 *   Dec, 2012       Ravi         MRO Support     Added Code for MRO
 *   May, 2014       Shilpi                       Coverity Fixes  
 *   July, 2014      Shilpi                       SPR 10331 Fix
 *   Aug, 2014       Shilpi                       SPR 13251 Fix
 ******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include <son_mif_fsm.h>
#include <son_utils.h>
#include <son_defines.h>
#include <son_oam_parser.h>
#include <son_oam_composer.h>
#include <son_oam_intf.h>

#include <son_apps_intf.h>
#include <son_es_intf.h>
#include <son_anr_intf.h>
#include <son_x2_intf.h>
#include <son_nmmctl_fsm.h>
#ifdef SON_MRO_FEATURE
#include <son_mro_intf.h>
#endif
#include <son_mlb_intf.h>
/* RACH OPT CHANGES : STARTS */
#include <son_rach_opt_intf.h>
/* RACH OPT CHANGES : ENDS */

/**
 * Global variables
 */

extern son_global_config_params_t g_config_params;

/* Variable storing the SMIF name for logging */
const son_8 *p_son_mif_facility_name = "SON_MIF";

/* Global variable storing the status (Enabled/Disabled) of SMIF logging */
son_oam_log_on_off_et g_mif_log_on_off = SON_OAM_LOG_OFF;

/* Array of pointers to storing the names of log levels */
extern const son_s8 *p_son_log_level_names[];

extern const QSHELL son_shell;

extern son_void_t *p_listener;

/**
 * Static functions
 */

static son_void_t
son_mif_sonmlb_dispatch_message
(
    son_void_t *,
    STACKAPIHDR,
    son_mif_gb_context_t *
);
static son_void_t
son_mif_dispatch_message
(
    son_void_t *,
    son_mif_gb_context_t *
);
static son_void_t
son_mif_oam_dispatch_message
(
    son_void_t *,
    STACKAPIHDR,
    son_mif_gb_context_t *
);
static son_void_t
son_mif_sonnmm_dispatch_message
(
    son_void_t *,
    STACKAPIHDR,
    son_mif_gb_context_t *
);
static son_void_t
son_mif_sonapps_dispatch_message
(
    son_void_t *,
    STACKAPIHDR,
    son_mif_gb_context_t *
);
static son_void_t
son_mif_sonanr_dispatch_message
(
    son_void_t *,
    STACKAPIHDR,
    son_mif_gb_context_t *
);
static son_void_t
son_mif_sones_dispatch_message
(
    son_void_t *,
    STACKAPIHDR,
    son_mif_gb_context_t *
);
static son_void_t 
son_mif_oam_anr_neighbor_default_config_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
);

#ifdef SON_MRO_FEATURE
static son_void_t
son_mif_sonmro_dispatch_message
(
    son_void_t *,
    STACKAPIHDR,
    son_mif_gb_context_t *
);

#endif
static son_void_t
son_mif_sonx2_dispatch_message
(
    son_void_t *,
    STACKAPIHDR,
    son_mif_gb_context_t *
);

/* RACH OPT CHANGES : STARTS */
static son_void_t
son_mif_sonrach_opt_dispatch_message
(
    son_void_t *,
    STACKAPIHDR,
    son_mif_gb_context_t *
);
/* RACH OPT CHANGES : ENDS */

static son_return_et
son_mif_convert_eutran_scan_req_to_nmm_scan_req
(
    const son_start_intra_rat_scan_req_t *p_eutran_scan,
    start_scan_req_t *p_nmm_scan_req
);

static son_return_et
son_mif_convert_utran_scan_req_to_nmm_scan_req
(
const son_start_utra_scan_req_t *p_utran_scan,
start_scan_req_t *p_nmm_scan_req
);

static const unsigned long son_mif_dst_list[] =
{
    SON_NMM_MODULE_ID,
    SON_APPS_MODULE_ID,
    SON_ANR_MODULE_ID,
    SON_ES_MODULE_ID,
    SON_OAM_MODULE_ID,
    SON_MRO_MODULE_ID,
    SON_MLB_MODULE_ID,
/* RACH OPT CHANGES : STARTS */
    SON_RACH_OPT_MODULE_ID
/* RACH OPT CHANGES : ENDS */
};

const QMANIFEST son_manifest_mif =
{
    "SON_MIF_MODULE_ID",
     SON_MIF_MODULE_ID,
     { 0, son_mif_init },
     { son_mif_process_msg, son_mif_process_timer_msg },
     { 0, 0, 0 },
     son_mif_dst_list
};

/* LTE_SON_KLOCWORK_WARN_9JULY_2012_START */
/* PR 7715 Fix Start */
const son_s8 *SON_MIF_OAM_EVENT_NAMES[] =
/* PR 7715 Fix End */
{
    (const son_s8 *)"SON_OAM_INIT_IND = 12000",
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
/* RACH OPT CHANGES : STARTS */
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
    /* RACH OPT CHANGES : ENDS */
    /* Rel 3.0 Cell delete support changes Start */
    (const son_s8 *)"SON_OAM_CELL_DELETE_REQ",
    /* SPR-13251 Fix Starts */
    (const son_s8 *)"SON_OAM_CELL_DELETE_RESP",
    /* Rel 3.0 Cell delete support changes Stop */
    (const son_s8 *)"SON_OAM_GET_LOG_LEVEL_REQ",
    (const son_s8 *)"SON_OAM_GET_LOG_LEVEL_RESP",
    /* SPR_19619 start */
    (const son_s8 *)"SON_OAM_PEER_CELL_ACTIVATION_REQ",
    /* SPR 19260:coverity_109481 start */
    (const son_s8 *)"SON_OAM_PEER_CELL_ACTIVATION_RESP",
    /* SPR 19260: coverity_109481 stop */
    /* SPR_19619 stop */
    /* SPR-13251 Fix Ends */
    /*spr 22474 start*/       
    /*+CID 109481*/ 
    (const son_s8 *)"SON_OAM_GET_DEBUG_INFO_REQ", 
    (const son_s8 *)"SON_OAM_GET_DEBUG_INFO_RESP",
    /*-CID 109481*/ 
    /*spr 22474 end*/
    /* SPR 20653 Fix Start */
    (const son_s8 *)"SON_OAM_ANR_MODIFY_ATTRIBUTE_REQ",
    (const son_s8 *)"SON_OAM_ANR_MODIFY_ATTRIBUTE_RESP",
    /* SPR 20653 Fix End */
};

/* LTE_SON_KLOCWORK_WARN_9JULY_2012_END */

/* LTE_SON_KLOCWORK_WARN_10JULY_2012_START */
static const son_s8 *SON_ANR_RES_EVENT_NAMES[] =
{
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
    (const son_s8 *)"SONANR_NR_ENB_ADD_RES",
    (const son_s8 *)"SONANR_NR_ENB_UPDATE_RES",
    (const son_s8 *)"SONANR_NR_ENB_DELETE_RES",
    (const son_s8 *)"SONANR_NR_ENB_RETRIEVE_RES",
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
};

static const son_s8 *SON_SMIF_EVENT_NAMES[] =
{
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
    /* SPR_19619 start */
    (const son_s8 *)"SMIF_GET_LOG_LEVEL_RESP",
    /* SPR-13251 Fix Ends */
    (const son_s8 *)"SMIF_PEER_CELL_ACTIVATION_RESP"
    /* SPR_19619 stop */
};

static const son_s8 *SON_ES_RES_EVENT_NAMES[] =
{
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
    (const son_s8 *)"SONES_FEATURE_STATE_CHANGE_IND"
};

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_START */
static const son_s8 *SON_X2_RES_EVENT_NAMES[] =
{
    (const son_s8 *)"SONX2_ADD_ENB_RES",
    (const son_s8 *)"SONX2_LINK_UP_IND",
    (const son_s8 *)"SONX2_LINK_DOWN_IND",
    (const son_s8 *)"SONX2_DELETE_ENB_RES",
    (const son_s8 *)"SONX2_ENB_CONFIG_UPDATE_IND",
    (const son_s8 *)"SONX2_ENB_RESET_RES",
    (const son_s8 *)"SONX2_HO_REPORT_IND",
    (const son_s8 *)"SONX2AP_HO_REPORT_IND",
    (const son_s8 *)"SONX2_GU_GROUP_ID_CONFIG_RES"
};

static const son_s8 *SON_APPS_RES_EVENT_NAMES[] =
{
     (const son_s8 *)"SONAPPS_NW_SCAN_RES",
     (const son_s8 *)"SONAPPS_CARRIER_FREQ_SELECTION_RES",
     (const son_s8 *)"SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_RES",
     (const son_s8 *)"SONAPPS_PCI_SELECTION_RES",
     (const son_s8 *)"SONAPPS_PCI_RESELECTION_RES",
     (const son_s8 *)"SONAPPS_PCI_CONFLICT_CONFUSION_IND",
     (const son_s8 *)"SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_RESP"  
};

#ifdef SON_MRO_FEATURE
static const son_s8 *SON_MRO_RES_EVENT_NAMES[] =
{
    (const son_s8 *)"SONMRO_ENABLE_RES",
    (const son_s8 *)"SONMRO_DISABLE_RES",
    (const son_s8 *)"SONMRO_MODIFY_CONFIG_PARAM_RES",
    (const son_s8 *)"SONMRO_TTT_UPDATE_IND",
    (const son_s8 *)"SONMRO_FEATURE_STATE_CHANGE_IND",
    (const son_s8 *)"SONMRO_HO_COUNTER_REPORT",
    (const son_s8 *)"SONMRO_OPTIMIZATION_NOTIFICATION"
};
#endif /* ifdef SON_MRO_FEATURE */
static const son_s8 *SON_MLB_RES_EVENT_NAMES[] =
{
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
    /* Start Coverity Fix: 30729, 30730 */ 
    /* Rel 3.0 Multi Sector code changes Start */
    (const son_s8 *)"SONMLB_INTERNAL_MOBILITY_CHANGE_ACK",
    (const son_s8 *)"SONMLB_INTERNAL_MOBILITY_CHANGE_FAILURE"
    /* Rel 3.0 Multi Sector code changes Stop */
    /* End Coverity Fix: 30729, 30730 */ 
};

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_STOP */

/* LTE_SON_KLOCWORK_WARN_10JULY_2012_END */

/* RACH OPT CHANGES : STARTS */
static const son_s8 *SON_RACH_OPT_RES_EVENT_NAMES[] =
{
    (const son_s8 *)"SONRACH_OPT_ENABLE_RES",
    (const son_s8 *)"SONRACH_OPT_DISABLE_RES",
    (const son_s8 *)"SONRACH_OPT_MODIFY_CONFIG_PARAMS_RES",
    (const son_s8 *)"SONRACH_OPT_MODIFY_RACH_OPTMODE_RES",
    (const son_s8 *)"SONRACH_OPT_PARAMETER_UPDATE_REQ",
    (const son_s8 *)"SONRACH_OPT_FAILURE_NOTIFICATION",
    (const son_s8 *)"SONRACH_OPT_UE_INFO_REPORT",
    (const son_s8 *)"SONRACH_OPT_L2_PERF_REPORT",
    (const son_s8 *)"SONRACH_OPT_FEATURE_STATE_CHANGE_IND",
};
/* RACH OPT CHANGES : ENDS */

/*****************************************************************************
 *                      Functions implementations
 *****************************************************************************/

/******For future use*********/
void *health_monitoring (void *p)
{
    lgr_thread_id_t thread_id = *((lgr_thread_id_t*)p);
    son_oam_set_thread_status(thread_id); 
    return SON_PNULL;
}

/****************************************************************
 * SON MIF Message Handlers
  ***************************************************************/

/* Rel 3.0 Cell delete support changes Start */
/*****************************************************************************
  * Function Name  : son_mif_send_nack_cell_delete_resp
  * Inputs         : p_smif_gb_context - SMIF global context pointer
  * Outputs        : None
  * Returns        : None
  * Description    : This function constructs and sends SON_OAM_CELL_DELETE_RESP
  ****************************************************************************/
static void son_mif_send_nack_cell_delete_resp
(
 son_oam_cell_delete_req_t* p_req,
 son_u16                    txn_id
 )
{
    son_oam_cell_delete_resp_t   cell_del_res;
    son_u8  idx = 0;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&cell_del_res, 0, sizeof(son_oam_cell_delete_resp_t));

    /* Populate the resp structure */
    cell_del_res.cell_delete_status_list_size = p_req->cell_delete_list_size;

    for (; idx < p_req->cell_delete_list_size; idx ++)
    {
        SON_MEMCPY(&cell_del_res.cell_delete_status_list[idx].cgi,
                &p_req->cell_delete_list[idx],
                sizeof(son_intra_rat_global_cell_id_t));
        cell_del_res.cell_delete_status_list[idx].result = SON_FAILURE;
    }

    son_oam_send_son_oam_cell_delete_resp(
            &cell_del_res,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            txn_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
}
/* Rel 3.0 Cell delete support changes Stop */

/*****************************************************************************
  * Function Name  : son_mif_log_enable_disable_cmd
  * Inputs         : Log Enable/Disable
  *                  log_level_present - Is log level present
  *                  Log Level to be set
  *                  Pointer to SON MgmtIfH global context 
  * Outputs        : None
  * Returns        : None
  * Description    : This function processes SON_OAM_LOG_ENABLE_DISABLE_REQ msg
  *                  to set the logging parameters for SON MgmtIfH
  ****************************************************************************/
static son_void_t
son_mif_log_enable_disable_cmd
(
    son_oam_log_on_off_et log_on_off,
    son_bool_et log_level_present,
    /* SPR-13251 Fix Starts */
    son_oam_log_level_et log_level,
    son_mif_gb_context_t   *p_smif_gb_context
    /* SPR-13251 Fix Ends */
)
{
    SON_UT_TRACE_ENTER();

    g_mif_log_on_off = log_on_off;
    SON_LOG(SON_OAM_LOG_ON, p_son_mif_facility_name, SON_BRIEF,
            "son_mif_log_enable_disable_cmd: Log enable/disable = %d",
            g_mif_log_on_off);

    if (g_mif_log_on_off == SON_OAM_LOG_ON)
    {
        if (SON_TRUE == log_level_present)
        {
            /* SPR-13251 Fix Starts */
            p_smif_gb_context->smif_log_level = log_level;
            /* SPR-13251 Fix Ends */
            SET_MODULE_LOG_LEVEL(log_level);
        }
    }
    else
    {
        /* SPR-13251 Fix Starts */
        p_smif_gb_context->smif_log_level = SON_LOG_LEVEL_NONE;
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(SON_LOG_LEVEL_NONE);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_log_enable_disable_cmd */

/*****************************************************************************
  * Function Name  : son_mif_set_log_level_cmd
  * Inputs         : Log Level to be set
  *                : p_smif_gb_context - pointer to the SMIF global context
  * Outputs        : None
  * Returns        : None
  * Description    : This function processes SON_OAM_SET_LOG_LEVEL_REQ msg
  *                  to set the log level for SON MgmtIfH
  ****************************************************************************/
static son_void_t
son_mif_set_log_level_cmd
(
    /* SPR-13251 Fix Starts */
    son_oam_log_level_et log_level,
    son_mif_gb_context_t *p_smif_gb_context
    /* SPR-13251 Fix Ends */
)
{
    SON_UT_TRACE_ENTER();

    if (g_mif_log_on_off == SON_OAM_LOG_ON)
    {                
        /* SPR-13251 Fix Starts */
        p_smif_gb_context->smif_log_level = log_level;
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(log_level);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, p_son_mif_facility_name, SON_WARNING,
                "son_mif_set_log_level_cmd: SMIF Logging is currently disabled."
                "Log level NOT set!");
    }

    SON_UT_TRACE_EXIT();
}

/* start mlb */

/*****************************************************************************
 * Function Name  : son_mif_oam_mlb_enable_req
 * Inputs         : p_api - pointer to the api
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_MLB_ENABLE_REQ msg
 ****************************************************************************/
son_void_t
son_mif_oam_mlb_enable_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_mlb_enable_req_t in_message;
    son_mlb_enable_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_mlb_enable_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);

    SON_MEMCPY(&(out_message.enable_req), &in_message,
            sizeof(son_oam_mlb_enable_req_t));

    son_mif_send_internal_msg(SON_MLB_MODULE_ID,
            SONMLB_ENABLE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_mlb_enable_req */

/*****************************************************************************
 * Function Name  : son_mif_mlb_enable_res
 * Inputs         : p_msg - pointer to the msg
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SONMLB_ENABLE_RES msg
 ****************************************************************************/
son_void_t
son_mif_mlb_enable_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_mlb_enable_res_t *p_in_message = SON_PNULL;
    son_oam_mlb_enable_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_mlb_enable_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_mlb_enable_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);
    SON_MEMCPY(&out_message, &(p_in_message->enable_res),
            sizeof(p_in_message->enable_res));

    son_oam_send_son_oam_mlb_enable_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_mlb_enable_res */

/*****************************************************************************
 * Function Name  : son_mif_oam_mlb_disable_req
 * Inputs         : p_api - pointer to the api
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_MLB_DISABLE_REQ msg
 ****************************************************************************/
son_void_t
son_mif_oam_mlb_disable_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_mlb_disable_req_t in_message;
    son_mlb_disable_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_mlb_disable_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);

    SON_MEMCPY(&(out_message.disable_req), &in_message,
            sizeof(son_oam_mlb_disable_req_t));

    son_mif_send_internal_msg(SON_MLB_MODULE_ID,
            SONMLB_DISABLE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_mlb_disable_req */

/*****************************************************************************
 * Function Name  : son_mif_mlb_disable_res
 * Inputs         : p_msg - pointer to the api
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SONMLB_DISABLE_RES msg
 ****************************************************************************/
son_void_t
son_mif_mlb_disable_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_mlb_disable_res_t *p_in_message = SON_PNULL;
    son_oam_mlb_disable_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_mlb_disable_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_mlb_disable_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMCPY(&out_message, &(p_in_message->disable_res),
            sizeof(p_in_message->disable_res));

    son_oam_send_son_oam_mlb_disable_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_mlb_disable_res */

/*****************************************************************************
 * Function Name  : son_mif_mlb_modify_attributes_res
 * Inputs         : p_msg - pointer to the api
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SONMLB_MODIFY_ATTRIBUTE_RES msg
 ****************************************************************************/
son_void_t
son_mif_mlb_modify_attributes_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_mlb_modify_res_t *p_in_message = SON_PNULL;
    son_oam_mlb_modify_attributes_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_mlb_modify_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_mlb_modify_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMCPY(&out_message, &(p_in_message->modify_attributes_res),
            sizeof(p_in_message->modify_attributes_res));

    son_oam_send_son_oam_mlb_modify_attributes_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_mlb_modify_attributes_res */

/*****************************************************************************
 * Function Name  : son_mif_oam_mlb_modify_atributes_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles
 *                  SON_OAM_MLB_MODIFY_ATTRIBUTES_REQ msg
 ******************************************************************************/
son_void_t
son_mif_oam_mlb_modify_attributes_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_mlb_modify_attributes_req_t in_message;
    son_mlb_modify_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

     SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_mlb_modify_attributes_req(
		&in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
		cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
		&length_read))
    {
	SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
		"Bad message - ignore.");
	SON_UT_TRACE_EXIT();
	return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.modify_attributes_req), &in_message,
            sizeof(son_oam_mlb_modify_attributes_req_t));

    son_mif_send_internal_msg(SON_MLB_MODULE_ID,
            SONMLB_MODIFY_ATTRIBUTES_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_mlb_modify_attributes_req */

/*SONMLB-- Centralized support start*/

/*****************************************************************************
 * Function Name  : son_mif_oam_mlb_offset_change_req
 * Inputs         : pointer to the message received
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_MLB_OFFSET_CHANGE_REQ msg
 ****************************************************************************/
son_void_t
son_mif_oam_mlb_offset_change_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_mlb_offset_change_req_t in_message;
    son_mlb_offset_change_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
        son_parse_son_oam_mlb_offset_change_req(
            &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
            cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
            &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);

    SON_MEMCPY(&(out_message.offset_change_req), &in_message,
        sizeof(son_oam_mlb_offset_change_req_t));

    son_mif_send_internal_msg(SON_MLB_MODULE_ID,
        SONMLB_OFFSET_CHANGE_REQ,
        sizeof(out_message),
        &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_mlb_offset_change_req */

/*****************************************************************************
 * Function Name  : son_mif_mlb_offset_change_resp
 * Inputs         : pointer to the message received
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_MLB_OFFSET_CHANGE_RESP msg
 ****************************************************************************/
son_void_t
son_mif_mlb_offset_change_resp
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_mlb_offset_change_resp_t *p_in_message = SON_PNULL;
    son_oam_mlb_offset_change_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
        sizeof(son_mlb_offset_change_resp_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_mlb_offset_change_resp_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMCPY(&out_message, &(p_in_message->offset_change_resp),
        sizeof(p_in_message->offset_change_resp));

    son_oam_send_son_oam_mlb_offset_change_resp(
        &out_message,
        SON_MIF_MODULE_ID,
        SON_OAM_MODULE_ID,
        p_in_message->transaction_id,
        SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_mlb_offset_change_resp */

/*****************************************************************************
 * Function Name  : son_mif_mlb_ho_load_params_info
 * Inputs         : pointer to the message received
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_MLB_PRARAMS_INFO msg
 ****************************************************************************/
son_void_t
son_mif_mlb_ho_load_params_info
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_mlb_params_info_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
        sizeof(son_mlb_params_info_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_mlb_params_info_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_mlb_params_info(
        p_in_message,
        SON_MIF_MODULE_ID,
        SON_OAM_MODULE_ID,
        SON_NULL,
        SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_mlb_ho_load_params_info */

/*****************************************************************************
 * Function Name  : son_mif_mlb_peer_offset_change_req
 * Inputs         : pointer to the message received
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_MLB_PEER_OFFSET_CHANGE_REQ msg
 ****************************************************************************/
son_void_t
son_mif_mlb_peer_offset_change_req
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_mlb_peer_offset_change_req_t *p_in_message = SON_PNULL;
    son_oam_mlb_peer_offset_change_req_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
        sizeof(son_mlb_peer_offset_change_req_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_mlb_peer_offset_change_req_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMCPY(&out_message, &(p_in_message->peer_offset_change_req),
        sizeof(p_in_message->peer_offset_change_req));

    son_oam_send_son_oam_mlb_peer_offset_change_req(
        &out_message,
        SON_MIF_MODULE_ID,
        SON_OAM_MODULE_ID,
        p_in_message->transaction_id,
        SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_mlb_peer_offset_change_req */

/*****************************************************************************
 * Function Name  : son_mif_oam_mlb_peer_offset_change_resp
 * Inputs         : pointer to the message received
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_MLB_PEER_OFFSET_CHANGE_RESP msg
 ****************************************************************************/
son_void_t
son_mif_oam_mlb_peer_offset_change_resp
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_mlb_peer_offset_change_resp_t in_message;
    son_mlb_peer_offset_change_resp_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
        son_parse_son_oam_mlb_peer_offset_change_resp(
            &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
            cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
            &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);

    SON_MEMCPY(&(out_message.peer_offset_change_resp), &in_message,
        sizeof(son_oam_mlb_peer_offset_change_resp_t));

    son_mif_send_internal_msg(SON_MLB_MODULE_ID,
        SONMLB_PEER_OFFSET_CHANGE_RESP,
        sizeof(out_message),
        &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_mlb_peer_offset_change_resp */

/*SONMLB-- Centralized support end
 * Rel 1.4 Start */

/*****************************************************************************
  * Function Name  : son_mif_oam_carrier_freq_and_dl_tx_power_req
 * Inputs         : p_api - pointer to the api
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_CARRIER_FREQ_AND_DL_TX_POWER_REQ
  ****************************************************************************/
static son_void_t
son_mif_oam_carrier_freq_and_dl_tx_power_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_carrier_freq_and_dl_tx_power_req_t      in_message;
    sonapps_carrier_freq_and_power_selection_req_t  out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();
    SON_ASSERT(SON_PNULL != p_api);

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (SON_SUCCESS !=
            son_parse_son_oam_carrier_freq_and_dl_tx_power_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
        "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.carrier_freq_dl_pwr_selection_req), &in_message,
        sizeof(son_oam_carrier_freq_and_dl_tx_power_req_t));

    son_mif_send_internal_msg(SON_APPS_MODULE_ID,
            SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_carrier_freq_and_dl_tx_power_req */

/* Rel 1.4 Stop */

/*****************************************************************************
 * Function Name  : son_mif_oam_mlb_modify_atributes_res
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles
 *                  SON_OAM_MLB_MODIFY_ATTRIBUTES_RES msg
 ******************************************************************************/
son_void_t
son_mif_oam_mlb_modify_atributes_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_mlb_modify_res_t *p_in_message = SON_PNULL;
    son_oam_mlb_modify_attributes_resp_t out_message;


    SON_UT_TRACE_ENTER();
    SON_ASSERT(SON_PNULL != p_msg);

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
	    sizeof(son_mlb_modify_res_t))
    {
	SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
		"Wrong message size - ignore.");
	SON_UT_TRACE_EXIT();
	return;
    }

    p_in_message = 
        (son_mlb_modify_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);
    
    SON_MEMCPY(&out_message, &p_in_message->modify_attributes_res,
            sizeof(son_oam_mlb_modify_attributes_resp_t));

    son_oam_send_son_oam_mlb_modify_attributes_resp( 
    		&out_message,
		SON_MIF_MODULE_ID,
        SON_OAM_MODULE_ID,
        SON_NULL,
        SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_mlb_modify_atributes_res */

/*end mlb*/

/*****************************************************************************
  * Function Name  : son_mif_init_config_res
  * Inputs         : pointer to the message received
 *                  mif global context
 *                  event id
  * Outputs        : None
  * Returns        : Success/Failure
  * Description    : This function handles SMIF_INIT_CONFIG_RES msg
  *                  sends Failure response to OAM if init config req fails for
  *                  one or more SON modules otherwise, sends Success response
  *                  to OAM after receiving Success init config responses from 
  *                  all SON internal modules
  ****************************************************************************/
son_return_et
son_mif_init_config_res
(
    son_void_t *p_msg,
    son_mif_gb_context_t *p_smif_gb_context,
    son_u16 event_id
)
{
    smif_init_config_res_t      *p_in_message = SON_PNULL;
    son_oam_init_config_resp_t   out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    p_in_message = (smif_init_config_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    if (p_in_message->result == SON_FAILURE)
    {
        out_message.result = SON_FAILURE;
        out_message.error_code = SON_INIT_CONFIG_FAIL;

        /*If init configuration has failed for the module then send a Failure 
         * response to OAM */
        son_oam_send_son_oam_init_config_resp(
                &out_message,
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                p_in_message->transaction_id,
                SON_NULL);

        SON_LOG( g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, "Failure received: ");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }

    else
    {
        SON_LOG( g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF, 
            "Success received: ");

        /* Update the respective flag for the module for the receipt of init 
         * config resp msg */
        p_smif_gb_context->init_config_resp_status |= event_id;
        
        SON_LOG( g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF, 
            "Event_id:%d, config_resp_status :%d", event_id, 
            p_smif_gb_context->init_config_resp_status );
            
        /* Check if smif_init_config_resp messages received from all SON 
         * modules */
        if ((p_smif_gb_context->init_config_resp_status & 
                SMIF_SONNMM_INIT_CONFIG_RESP_RECD)
                && (p_smif_gb_context->init_config_resp_status & 
                    SMIF_SONAPPS_INIT_CONFIG_RESP_RECD)
                && (p_smif_gb_context->init_config_resp_status & 
                    SMIF_SONANR_INIT_CONFIG_RESP_RECD)
                && (p_smif_gb_context->init_config_resp_status & 
                    SMIF_SONES_INIT_CONFIG_RESP_RECD)
                && (p_smif_gb_context->init_config_resp_status & 
                    SMIF_SONX2_INIT_CONFIG_RESP_RECD)
#ifdef SON_MRO_FEATURE                    
                && (p_smif_gb_context->init_config_resp_status &
                    SMIF_SONMRO_INIT_CONFIG_RESP_RECD)
#endif                    
                && (p_smif_gb_context->init_config_resp_status &
                    SMIF_SONMLB_INIT_CONFIG_RESP_RECD
                    )
/* RACH OPT CHANGES : STARTS */
                && (p_smif_gb_context->init_config_resp_status &
                    SMIF_SONRACH_OPT_INIT_CONFIG_RESP_RECD)
/* RACH OPT CHANGES : ENDS */
           )
        {
            out_message.result = SON_SUCCESS;
            out_message.error_code = SON_NO_ERROR;
            
            /*If Success responses from all the modules have been received, 
             * send the Success response to OAM */
            son_oam_send_son_oam_init_config_resp(
                    &out_message,
                    SON_MIF_MODULE_ID,
                    SON_OAM_MODULE_ID,
                    p_in_message->transaction_id,
                    SON_NULL);
        }

        SON_UT_TRACE_EXIT();
        return SON_SUCCESS;
    }
} /* son_mif_init_config_res */

/* SPR_19619 start */

/*****************************************************************************
  * Function Name  : son_mif_peer_cell_activation_res
  * Inputs         : pointer to the message received
 *                  mif global context
 *                  event id
  * Outputs        : None
  * Returns        : Success/Failure
  * Description    : This function handles SMIF_PEER_CELL_ACTIVATION_RESP msg
  ****************************************************************************/
son_return_et
son_mif_peer_cell_activation_res
(
    son_void_t *p_msg,
    son_mif_gb_context_t *p_smif_gb_context,
    son_u16 event_id
)
{
    smif_peer_cell_activation_res_t      *p_in_message = SON_PNULL;
    son_oam_peer_cell_activation_resp_t   out_message;
    (void)event_id;
    (void)p_smif_gb_context;
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    p_in_message = (smif_peer_cell_activation_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    if (p_in_message->result == SON_FAILURE)
    {
        out_message.result = SON_FAILURE;
        out_message.error_code = SON_ERR_CELL_ACTIVATION_FAILED;

        son_oam_send_son_oam_peer_cell_activation_resp(
                &out_message,
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                p_in_message->transaction_id,
                SON_NULL);

        SON_LOG( g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, "Failure received: ");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }

    else
    {
        SON_LOG( g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF, 
                "Success response received: ");

        out_message.result = SON_SUCCESS;
        out_message.error_code = SON_NO_ERROR;

        /*If Success responses from SON module, 
         * send the Success response to OAM */
        son_oam_send_son_oam_peer_cell_activation_resp(
                &out_message,
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                p_in_message->transaction_id,
                SON_NULL);

    }
    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;

}
/* SPR_19619 stop */

/* Rel 3.0 Cell delete support changes Start */
/*****************************************************************************
  * Function Name  : son_mif_oam_cell_delete_req
  * Inputs         : pointer to the message received
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_CELL_DELETE_REQ msg
  ****************************************************************************/
static son_void_t son_mif_oam_cell_delete_req
(
 son_void_t             *p_api,
 STACKAPIHDR            cspl_hdr,
 son_mif_gb_context_t*  p_smif_gb_context
 )
{
    son_oam_cell_delete_req_t   in_message;
    son_u16                     transaction_id;

    S32 length_read = SON_NULL;

    SON_UT_TRACE_ENTER();

    if(SON_SUCCESS !=
            son_parse_son_oam_cell_delete_req(
                &in_message,
                (U8*)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Get the transaction id from the incoming message */
    transaction_id = son_get_transaction_id(p_api);

    /* Check if cell delete request is already in process */
    if(SON_NULL != p_smif_gb_context->cell_delete_req_in_process)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                "Unexpected message - Cell delete request is already "
                "in process.");

        son_mif_send_nack_cell_delete_resp(&in_message, transaction_id);

        SON_UT_TRACE_EXIT();
        return;
    }

    /* Reset the cell delete resp status flag and structure from SMIF global 
       context */
    p_smif_gb_context->cell_delete_req_in_process = SON_ONE;
    p_smif_gb_context->cell_delete_resp_status = SON_NULL;

    SON_MEMSET(&p_smif_gb_context->cell_delete_res, 0,
            sizeof(son_cell_delete_resp_t));

    p_smif_gb_context->cell_delete_res.transaction_id = transaction_id;
    p_smif_gb_context->cell_delete_res.cell_delete_resp.
        cell_delete_status_list_size = in_message.cell_delete_list_size;

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SMIF_CELL_DELETE_REQ,
            sizeof(in_message),
            &in_message);

    son_mif_send_internal_msg(SON_ES_MODULE_ID,
            SMIF_CELL_DELETE_REQ,
            sizeof(in_message),
            &in_message);

#ifdef SON_MRO_FEATURE
    son_mif_send_internal_msg(SON_MRO_MODULE_ID,
            SMIF_CELL_DELETE_REQ,
            sizeof(in_message),
            &in_message);
#endif

    son_mif_send_internal_msg(SON_MLB_MODULE_ID,
            SMIF_CELL_DELETE_REQ,
            sizeof(in_message),
            &in_message);

    son_mif_send_internal_msg(SON_RACH_OPT_MODULE_ID,
            SMIF_CELL_DELETE_REQ,
            sizeof(in_message),
            &in_message);
    
    SON_UT_TRACE_EXIT();
}
/* Rel 3.0 Cell delete support changes Stop */

/* SPR-10331 Fix Starts */
/*****************************************************************************
 * Function Name  : son_mif_oam_anr_gu_group_id_config_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_ANR_GU_GROUP_ID_CONFIG_REQ msg
 ****************************************************************************/
static son_void_t
son_mif_oam_anr_gu_group_id_config_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_anr_gu_group_id_config_req_t in_message;
    son_x2_gu_group_id_config_req_t out_message;

    S32 length_read = SON_NULL;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_anr_gu_group_id_config_req(
                &in_message,
                (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);

    SON_MEMCPY(&(out_message.gu_group_id_config_req), &in_message,
            sizeof(son_oam_anr_gu_group_id_config_req_t));
       
    son_mif_send_internal_msg(SON_X2_MODULE_ID,
            SONX2_GU_GROUP_ID_CONFIG_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_anr_gu_group_id_config_req */
/* SPR-10331 Fix Ends */

/****************************************************************
 * SON ANR Message Handlers
 ***************************************************************/

/*****************************************************************************
  * Function Name  : son_mif_nr_scan_res
 * Inputs         : p_msg - pointer to the msg
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_NR_SCAN_RES msg
  ****************************************************************************/
static son_void_t
son_mif_nr_scan_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_scan_res_t *p_in_message = SON_PNULL;
    son_oam_nr_scan_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_scan_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_anr_scan_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);
    SON_MEMCPY((&out_message), &(p_in_message->scan_res),
        sizeof(son_oam_nr_scan_resp_t));

    son_oam_send_son_oam_nr_scan_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);
    SON_UT_TRACE_EXIT();
} /* son_mif_nr_scan_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_updated_nrt_info
 * Inputs         : p_msg - pointer to the msg
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_UPDATED_NRT_INFO msg
  ****************************************************************************/
static son_void_t
son_mif_anr_updated_nrt_info
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_updated_nrt_info_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_updated_nrt_info_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message =
        (son_anr_updated_nrt_info_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);
        
    son_oam_send_son_oam_anr_updated_nrt_info(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_updated_nrt_info */

/*****************************************************************************
  * Function Name  : son_mif_anr_modify_ue_count_res
  * Inputs         : p_msg      Pointer to the message received
  *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_MODIFY_UE_COUNT_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_modify_ue_count_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_modify_ue_count_res_t *p_in_message = SON_PNULL;
    son_oam_anr_modify_ue_count_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_modify_ue_count_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message =
        (son_anr_modify_ue_count_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);
        
    SON_MEMCPY(&out_message, &p_in_message->generic_res, 
        sizeof(son_oam_anr_modify_ue_count_resp_t));

    son_oam_send_son_oam_anr_modify_ue_count_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_modify_ue_count_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_modify_meas_reporting_interval_res
  * Inputs         : p_msg      Pointer to the message received
  *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles 
  *                  SONANR_MODIFY_MEAS_REPORTING_INTERVAL_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_modify_meas_reporting_interval_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_modify_meas_reporting_interval_res_t *p_in_message = SON_PNULL;
    son_oam_anr_modify_meas_reporting_interval_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_modify_meas_reporting_interval_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message =
        (son_anr_modify_meas_reporting_interval_res_t *)((U8 *)p_msg +
            SON_API_HEADER_SIZE);
        
    SON_MEMCPY(&out_message, &p_in_message->generic_res, 
        sizeof(son_oam_anr_modify_meas_reporting_interval_resp_t));

    son_oam_send_son_oam_anr_modify_meas_reporting_interval_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_modify_meas_reporting_interval_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_modify_removal_attributes_res
  * Inputs         : p_msg      Pointer to the message received
  *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles 
  *                  SONANR_MODIFY_REMOVAL_ATTRIBUTES_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_modify_removal_attributes_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_modify_removal_attributes_res_t *p_in_message = SON_PNULL;
    son_oam_anr_modify_removal_attributes_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_modify_removal_attributes_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message =
        (son_anr_modify_removal_attributes_res_t *)((U8 *)p_msg +
            SON_API_HEADER_SIZE);
        
    SON_MEMCPY(&out_message, &p_in_message->modify_removal_attributes_res, 
        sizeof(son_oam_anr_modify_removal_attributes_resp_t));

    son_oam_send_son_oam_anr_modify_removal_attributes_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_modify_removal_attributes_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_modify_pci_confusion_cfg_res
  * Inputs         : p_msg      Pointer to the message received
  *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles 
  *                  SONANR_MODIFY_PCI_CONFUSION_CFG_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_modify_pci_confusion_cfg_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_modify_pci_confusion_cfg_res_t *p_in_message = SON_PNULL;
    son_oam_anr_modify_pci_confusion_cfg_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_modify_pci_confusion_cfg_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message =
        (son_anr_modify_pci_confusion_cfg_res_t *)((U8 *)p_msg +
            SON_API_HEADER_SIZE);
        
    SON_MEMCPY(&out_message, &p_in_message->modify_pci_confusion_cfg_res, 
        sizeof(son_oam_anr_modify_pci_confusion_cfg_resp_t));

    son_oam_send_son_oam_anr_modify_pci_confusion_cfg_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_modify_pci_confusion_cfg_res */

/* SPR 20653 Fix Start */
/*****************************************************************************
  * Function Name  : son_mif_anr_modify_attribute_res
  * Inputs         : p_msg      Pointer to the message received
  *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles 
  *                  SONANR_MODIFY_ATTRIBUTE_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_modify_attribute_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_modify_attribute_res_t *p_in_message = SON_PNULL;
    son_oam_anr_modify_attribute_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_modify_attribute_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message =
        (son_anr_modify_attribute_res_t *)((U8 *)p_msg +
            SON_API_HEADER_SIZE);
        
    SON_MEMCPY(&out_message, &p_in_message->modify_attribute_res, 
        sizeof(son_oam_anr_modify_attribute_resp_t));

    son_oam_send_son_oam_anr_modify_attribute_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_modify_attribute_res */
/* SPR 20653 Fix End */

/*****************************************************************************
  * Function Name  : son_mif_anr_enable_disable_limited_mode_res
  * Inputs         : p_msg      Pointer to the message received
  *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles 
  *                  SONANR_ENABLE_DISABLE_LIMITED_MODE_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_enable_disable_limited_mode_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_enable_disable_limited_mode_res_t *p_in_message = SON_PNULL;
    son_oam_anr_enable_disable_limited_mode_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_enable_disable_limited_mode_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message =
        (son_anr_enable_disable_limited_mode_res_t *)((U8 *)p_msg +
            SON_API_HEADER_SIZE);
        
    SON_MEMCPY(&out_message, &p_in_message->enable_disable_limited_mode_res, 
        sizeof(son_oam_anr_enable_disable_limited_mode_resp_t));

    son_oam_send_son_oam_anr_enable_disable_limited_mode_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_enable_disable_limited_mode_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_enable_disable_x2_res
  * Inputs         : p_msg      Pointer to the message received
  *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles 
  *                  SONANR_ENABLE_DISABLE_X2_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_enable_disable_x2_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_enable_disable_x2_res_t *p_in_message = SON_PNULL;
    son_oam_enable_disable_x2_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_enable_disable_x2_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message =
        (son_anr_enable_disable_x2_res_t *)((U8 *)p_msg +
            SON_API_HEADER_SIZE);
        
    SON_MEMCPY(&out_message, &p_in_message->enable_disable_x2_res, 
        sizeof(son_oam_enable_disable_x2_resp_t));

    son_oam_send_son_oam_enable_disable_x2_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_enable_disable_x2_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_enable_res 
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_ENABLE_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_enable_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_enable_res_t *p_in_message = SON_PNULL;
    son_oam_anr_enable_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_enable_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_anr_enable_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);
    SON_MEMCPY(&out_message, &(p_in_message->enable_res),
        sizeof(p_in_message->enable_res));

    son_oam_send_son_oam_anr_enable_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_enable_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_disable_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_DISABLE_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_disable_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_disable_res_t *p_in_message = SON_PNULL;
    son_oam_anr_disable_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_disable_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_anr_disable_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMCPY(&out_message, &(p_in_message->disable_res),
            sizeof(p_in_message->disable_res));

    son_oam_send_son_oam_anr_disable_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_disable_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_nr_update_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_NR_UPDATE_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_nr_update_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_nr_update_res_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_nr_update_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_anr_nr_update_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_nr_update_resp(
            &p_in_message->upd_res,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);
    
    SON_UT_TRACE_EXIT();
} /* son_mif_anr_nr_update_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_nr_add_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_NR_ADD_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_nr_add_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_nr_add_res_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_nr_add_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_anr_nr_add_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_nr_add_resp(
            &p_in_message->add_res,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_nr_add_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_nr_delete_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_NR_DELETE_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_nr_delete_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_nr_delete_res_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_nr_delete_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_anr_nr_delete_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_nr_delete_resp(
            &p_in_message->del_res,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_nr_delete_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_nr_delete_from_remove_list_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles 
  *                  SONANR_NR_DELETE_FROM_REMOVE_LIST_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_nr_delete_from_remove_list_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_nr_delete_from_remove_list_res_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_nr_delete_from_remove_list_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_anr_nr_delete_from_remove_list_res_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_nr_delete_from_remove_list_resp(
            &p_in_message->delete_from_remove_list_res,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_nr_delete_from_remove_list_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_nr_retrieve_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_NR_RETREIVE_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_nr_retrieve_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_nr_retrieve_res_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_nr_retrieve_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message =
        (son_anr_nr_retrieve_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_nr_retrieve_resp(
            &p_in_message->rtrv_res,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_nr_retrieve_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_meas_config_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_MEAS_CONFIG_RES  msg
  ****************************************************************************/
static son_void_t
son_mif_anr_meas_config_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_meas_config_res_t *p_in_message = SON_PNULL;
    son_oam_anr_meas_config_res_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_meas_config_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message =
        (son_anr_meas_config_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMCPY(&out_message, &(p_in_message->config_res),
            sizeof(p_in_message->config_res));

   son_oam_send_son_oam_anr_meas_config_res(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_meas_config_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_nr_enb_add_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_NR_ENB_ADD_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_nr_enb_add_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_enb_add_resp_t *p_in_message = SON_PNULL;
    son_oam_nr_enb_add_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_enb_add_resp_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_anr_enb_add_resp_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMCPY(&out_message, &(p_in_message->enb_add_res),
                        sizeof(p_in_message->enb_add_res));

    son_oam_send_son_oam_nr_enb_add_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_nr_enb_add_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_nr_enb_update_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_NR_ENB_UPDATE_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_nr_enb_update_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_enb_update_resp_t *p_in_message = SON_PNULL;
    son_oam_nr_enb_update_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_enb_update_resp_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_anr_enb_update_resp_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMCPY(&out_message, &(p_in_message->enb_update_res),
                        sizeof(p_in_message->enb_update_res));

    son_oam_send_son_oam_nr_enb_update_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_nr_enb_update_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_nr_enb_delete_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_NR_ENB_DELETE_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_nr_enb_delete_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_enb_delete_resp_t *p_in_message = SON_PNULL;
    son_oam_nr_enb_delete_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_enb_delete_resp_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_anr_enb_delete_resp_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMCPY(&out_message, &(p_in_message->enb_del_res),
                        sizeof(p_in_message->enb_del_res));

    son_oam_send_son_oam_nr_enb_delete_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_nr_enb_delete_res */

/*****************************************************************************
  * Function Name  : son_mif_anr_nr_enb_retrieve_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_NR_ENB_RETRIEVE_RES msg
  ****************************************************************************/
static son_void_t
son_mif_anr_nr_enb_retrieve_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_enb_retrieve_resp_t *p_in_message = SON_PNULL;
    son_oam_nr_enb_retrieve_resp_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_enb_retrieve_resp_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_anr_enb_retrieve_resp_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMCPY(&out_message, &(p_in_message->enb_retrieve_res),
            sizeof(p_in_message->enb_retrieve_res));

    son_oam_send_son_oam_nr_enb_retrieve_resp(
          &out_message,
          SON_MIF_MODULE_ID,
          SON_OAM_MODULE_ID,
          p_in_message->transaction_id,
          SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_nr_enb_retrieve_res */

/*****************************************************************************
 * Function Name  : son_mif_anr_limited_mode_pci_info
* Inputs         : p_msg- pointer to the message
*                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SONANR_LIMITED_MODE_PCI_INFO msg
 *****************************************************************************/
static son_void_t
son_mif_anr_limited_mode_pci_info
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_limited_mode_pci_info_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_limited_mode_pci_info_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message =
        (son_anr_limited_mode_pci_info_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_anr_limited_mode_pci_info(
            p_in_message->p_anr_limited_mode_pci_info_t,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);
    
    son_mem_free(p_in_message->p_anr_limited_mode_pci_info_t);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_limited_mode_pci_info */


/*****************************************************************************
  * Function Name  : son_mif_anr_meas_config_obj_remove_res
  * Inputs         : pointer to the message received
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONANR_MEAS_CONFIG_OBJ_REMOVE_RES  msg
  ****************************************************************************/
static son_void_t son_mif_anr_meas_config_obj_remove_res(son_void_t *p_msg,
            STACKAPIHDR cspl_hdr)
{
    son_anr_meas_config_obj_remove_res_t       *p_in_message = SON_PNULL;
    son_oam_anr_meas_config_obj_remove_res_t   out_message;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_meas_config_obj_remove_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }
    p_in_message =
        (son_anr_meas_config_obj_remove_res_t *)((U8*)p_msg + SON_API_HEADER_SIZE);

    SON_MEMCPY(&out_message , &(p_in_message->remove_res),
            sizeof(p_in_message->remove_res));


   son_oam_send_son_oam_anr_meas_config_obj_remove_res(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
}

/** eICIC changes start */
static son_void_t 
son_mif_anr_neighbor_default_config_resp
(
    son_void_t      *p_msg,
    STACKAPIHDR     cspl_hdr
)
{
    smif_neighbor_default_config_res_t *p_in_message = SON_PNULL;
    son_oam_anr_neighbor_default_config_resp_t out_message;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(son_oam_anr_neighbor_default_config_resp_t));
    if (cspl_hdr.paramlen != 
            sizeof(smif_neighbor_default_config_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size --ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }
    p_in_message = (smif_neighbor_default_config_res_t *) ((U8 *)p_msg + SON_API_HEADER_SIZE);
    SON_MEMCPY(&out_message,
               &(p_in_message->default_resp),
               sizeof(son_oam_anr_neighbor_default_config_resp_t));
    son_oam_send_son_oam_anr_neighbor_default_config_resp (
                &out_message,
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                p_in_message->transaction_id,
                SON_NULL);
    SON_UT_TRACE_EXIT();
}

/** eICIC changes end */

/*****************************************************************************
  * Function Name  : son_mif_x2_link_up_ind
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONX2_LINK_UP_IND msg
  ****************************************************************************/
static son_void_t
son_mif_x2_link_up_ind
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_x2_oam_link_up_ind_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_x2_oam_link_up_ind_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_x2_oam_link_up_ind_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_x2_link_up_ind(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_x2_link_up_ind */

/*****************************************************************************
  * Function Name  : son_mif_x2_link_down_ind
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONX2_LINK_DOWN_IND msg
  ****************************************************************************/
static son_void_t
son_mif_x2_link_down_ind
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_x2_oam_link_down_ind_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_x2_oam_link_down_ind_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_x2_oam_link_down_ind_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);
    son_oam_send_son_oam_x2_link_down_ind(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_x2_link_down_ind */

/* SPR-10331 Fix Starts */
/*****************************************************************************
 * Function Name  : son_mif_x2_gu_group_id_config_res
 * Inputs         : p_msg- Pointer to the message
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SONX2_GU_GROUP_ID_CONFIG_RES msg
  ****************************************************************************/
static son_void_t
son_mif_x2_gu_group_id_config_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_x2_gu_group_id_config_resp_t *p_in_message = SON_PNULL;
    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_x2_gu_group_id_config_resp_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_x2_gu_group_id_config_resp_t *)((U8 *)p_msg + 
            SON_API_HEADER_SIZE);
    son_oam_send_son_oam_anr_gu_group_id_config_resp(
            &p_in_message->gu_group_id_config_resp,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_x2_gu_group_id_config_res */
/* SPR-10331 Fix Ends */

/********************************************************************************
 * Function Name  : son_mif_anr_feature_state_change_ind
* Inputs         : p_msg- pointer to the message
*                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function handles SONANR_FEATURE_STATE_CHANGE_IND msg
 ********************************************************************************/
static son_void_t
son_mif_anr_feature_state_change_ind
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_anr_feature_state_change_ind_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_anr_feature_state_change_ind_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_anr_feature_state_change_ind_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_feature_state_change_ind(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_anr_feature_state_change_ind */

/****************************************************************
 * SONAPPS(ES) Message Handlers
 ***************************************************************/

/*****************************************************************************
  * Function Name  : son_mif_es_enable_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONES_ENABLE_RES msg
  ****************************************************************************/
static son_void_t
son_mif_es_enable_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_es_enable_res_t *p_in_message = SON_PNULL;
    son_oam_es_enable_resp_t out_message;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_es_enable_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    p_in_message = (son_es_enable_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMCPY(&out_message, &(p_in_message->enable_res),
            sizeof(p_in_message->enable_res));

    son_oam_send_son_oam_es_enable_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_es_enable_res */

/*****************************************************************************
  * Function Name  : son_mif_es_disable_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONES_DISABLE_RES msg
  ****************************************************************************/
static son_void_t
son_mif_es_disable_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_es_disable_res_t *p_in_message = SON_PNULL;
    son_oam_es_disable_resp_t out_message;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_es_disable_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_es_disable_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    SON_MEMCPY(&out_message, &(p_in_message->enable_res),
        sizeof(p_in_message->enable_res));

    son_oam_send_son_oam_es_disable_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_es_disable_res */

/*****************************************************************************
  * Function Name  : son_mif_es_modify_ue_count_interval_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles 
  *                  SONES_MODIFY_UE_COUNT_INTERVAL_RES msg
  ****************************************************************************/
static son_void_t
son_mif_es_modify_ue_count_interval_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_es_modify_ue_count_interval_res_t *p_in_message = SON_PNULL;
    son_oam_es_modify_ue_count_interval_resp_t out_message;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_es_modify_ue_count_interval_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_es_modify_ue_count_interval_res_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    SON_MEMCPY(&(out_message.srv_cgi), &(p_in_message->generic_res.cell_id),
                    sizeof(p_in_message->generic_res.cell_id));
    out_message.result = p_in_message->generic_res.result;
    out_message.error_code = p_in_message->generic_res.error_code;

    son_oam_send_son_oam_es_modify_ue_count_interval_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_es_modify_ue_count_interval_res */

/*****************************************************************************
  * Function Name  : son_mif_es_active_ue_count_report
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONES_ACTIVE_UE_COUNT_REPORT msg
  ****************************************************************************/
static son_void_t
son_mif_es_active_ue_count_report
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_es_active_ue_cnt_report_t *p_in_message = SON_PNULL;
    son_oam_es_active_ue_count_report_t out_message;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_es_active_ue_cnt_report_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message =
        (son_es_active_ue_cnt_report_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));
          
    SON_MEMCPY(&(out_message.srv_cgi), &(p_in_message->cell_id),
        sizeof(p_in_message->cell_id));
    out_message.ue_cnt = p_in_message->ue_cnt;

    son_oam_send_son_oam_es_active_ue_count_report(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_es_active_ue_count_report */

/*****************************************************************************
  * Function Name  : son_mif_es_modify_ue_count_threshold_resp
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONES_MODIFY_UE_COUNT_THRESHOLD_RES msg
  ****************************************************************************/
static son_void_t
son_mif_es_modify_ue_count_threshold_resp
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_es_modify_ue_count_interval_res_t         *p_in_message = SON_PNULL;
    son_oam_es_modify_ue_count_threshold_resp_t    out_message;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_es_modify_ue_count_interval_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_es_modify_ue_count_interval_res_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    SON_MEMCPY(&(out_message.srv_cgi), &(p_in_message->generic_res.cell_id),
            sizeof(p_in_message->generic_res.cell_id));
    out_message.result = p_in_message->generic_res.result;
    out_message.error_code = p_in_message->generic_res.error_code;

    son_oam_send_son_oam_es_modify_ue_count_threshold_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_es_modify_ue_count_threshold_resp */

/*****************************************************************************
  * Function Name  : son_mif_es_ue_count_threshold_hit_ind
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONES_UE_COUNT_THRESHOLD_HIT_IND msg
  ****************************************************************************/
static son_void_t
son_mif_es_ue_count_threshold_hit_ind
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_es_threshold_hit_ind_t  	            *p_in_message = SON_PNULL;
    son_oam_es_ue_count_threshold_hit_ind_t      out_message;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_es_threshold_hit_ind_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_es_threshold_hit_ind_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));
          
    SON_MEMCPY(&(out_message.srv_cgi), &(p_in_message->cell_id),
        sizeof(p_in_message->cell_id));

    if (RRM_ABOVE_TO_BELOW_THRESHOLD == p_in_message->cause)
    {
        out_message.cause = REACHED_THRESHOLD;
    }
    else if (RRM_BELOW_TO_ABOVE_THRESHOLD == p_in_message->cause)
    {
        out_message.cause = ABOVE_THRESHOLD;
    }

    son_oam_send_son_oam_es_ue_count_threshold_hit_ind(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_es_ue_count_threshold_hit_ind */

/*****************************************************************************
  * Function Name  : son_mif_es_request_to_switch_on_cell
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONES_REQUEST_TO_SWITCH_ON_CELL msg
  ****************************************************************************/
static son_void_t
son_mif_es_request_to_switch_on_cell
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_es_request_to_switch_on_cell_t         *p_in_message = SON_PNULL;
    son_oam_es_request_to_switch_on_cell_t      out_message;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_es_request_to_switch_on_cell_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_es_request_to_switch_on_cell_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));
          
    SON_MEMCPY(&out_message, (U8 *)p_in_message,
        sizeof(son_oam_es_request_to_switch_on_cell_t));

    son_oam_send_son_oam_es_request_to_switch_on_cell(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_es_request_to_switch_on_cell */

/*****************************************************************************
  * Function Name  : son_mif_es_switched_off_cell_info_resp
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONES_SWITCHED_OFF_CELL_INFO_RES msg
  ****************************************************************************/
static son_void_t
son_mif_es_switched_off_cell_info_resp
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_es_switched_off_cell_info_res_t        *p_in_message = SON_PNULL;
    son_oam_es_switched_off_cell_info_resp_t    out_message;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_es_switched_off_cell_info_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_es_switched_off_cell_info_res_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));
          
    SON_MEMCPY(&out_message, &(p_in_message->switched_off_cell_info),
        sizeof(p_in_message->switched_off_cell_info));

    son_oam_send_son_oam_es_switched_off_cell_info_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_es_switched_off_cell_info_resp */

/*****************************************************************************
  * Function Name  : son_mif_es_cell_switch_off_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONES_CELL_SWITCH_OFF_RES msg
  ****************************************************************************/
static son_void_t
son_mif_es_cell_switch_off_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_es_cell_switch_off_res_t *p_in_message = SON_PNULL;
    son_oam_es_cell_switch_off_resp_t out_message;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_es_cell_switch_off_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_es_cell_switch_off_res_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    SON_MEMCPY(&(out_message.srv_cgi), &(p_in_message->generic_res.cell_id),
            sizeof(p_in_message->generic_res.cell_id));
    out_message.result = p_in_message->generic_res.result;
    out_message.error_code = p_in_message->generic_res.error_code;

    son_oam_send_son_oam_es_cell_switch_off_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_es_cell_switch_off_res */

/*****************************************************************************
  * Function Name  : son_mif_es_cell_switch_on_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SONES_CELL_SWITCH_ON_RES msg
  ****************************************************************************/
static son_void_t
son_mif_es_cell_switch_on_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_es_cell_switch_on_res_t *p_in_message = SON_PNULL;
    son_oam_es_cell_switch_on_resp_t out_message;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_es_cell_switch_on_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_es_cell_switch_on_res_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    SON_MEMCPY(&(out_message.srv_cgi), &(p_in_message->generic_res.cell_id),
            sizeof(p_in_message->generic_res.cell_id));
    out_message.result = p_in_message->generic_res.result;
    out_message.error_code = p_in_message->generic_res.error_code;

    son_oam_send_son_oam_es_cell_switch_on_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_es_cell_switch_on_res */

/*****************************************************************************
 * Function Name  : son_mif_es_cell_switch_on_off_ind
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
 * Description    : This function handles SONES_CELL_SWITCH_ON_OFF_IND msg
  ****************************************************************************/
static son_void_t
son_mif_es_cell_switch_on_off_ind
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_es_cell_switchon_ind_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_es_cell_switchon_ind_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_es_cell_switchon_ind_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_es_cell_switch_on_off_ind(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_es_cell_switch_on_off_ind */

/********************************************************************************
 * Function Name  : son_mif_es_feature_state_change_ind
* Inputs         : p_msg- pointer to the message
*                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function handles SONES_FEATURE_STATE_CHANGE_IND msg
 ********************************************************************************/
static son_void_t
son_mif_es_feature_state_change_ind
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_es_feature_state_change_ind_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_es_feature_state_change_ind_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_es_feature_state_change_ind_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_feature_state_change_ind(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_es_feature_state_change_ind */

/* AUTONOMOUS SWITCH OFF START */

/*****************************************************************************
 * Function Name  : son_mif_es_autonomous_switch_off_config_resp
 * Inputs         : pointer to the message received
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_RESP msg
 ****************************************************************************/
static son_void_t
son_mif_es_autonomous_switch_off_config_resp
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_es_autonomous_switch_off_config_resp_t *p_in_message = SON_PNULL;
    son_oam_es_autonomous_switch_off_config_resp_t out_message;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
        sizeof(son_es_autonomous_switch_off_config_resp_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_es_autonomous_switch_off_config_resp_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    SON_MEMCPY(&(out_message), &(p_in_message->autonomous_switch_off_config_resp),
        sizeof(out_message));

    son_oam_send_son_oam_es_autonomous_switch_off_config_resp(
        &out_message,
        SON_MIF_MODULE_ID,
        SON_OAM_MODULE_ID,
        p_in_message->transaction_id,
        SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_es_autonomous_switch_off_config_resp */

/* AUTONOMOUS SWITCH OFF END */

#ifdef SON_MRO_FEATURE

/****************************************************************
 * SONMRO Message Handlers
 ***************************************************************/

/*****************************************************************************
 * Function Name  : son_mif_mro_enable_res
 * Inputs         : Pointer to the message received
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SONMRO_ENABLE_RES msg
 ****************************************************************************/
static son_void_t
son_mif_mro_enable_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_mro_enable_res_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_mro_enable_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_mro_enable_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    if (SON_PNULL != p_in_message)
    {
        son_oam_send_son_oam_mro_enable_resp(
                &(p_in_message->enable_res),
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                p_in_message->transaction_id,
                SON_NULL);
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Pointer to SONMRO_ENABLE_RES data is NULL - ignore");
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_mro_enable_res */

/*****************************************************************************
 * Function Name  : son_mif_mro_disable_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SONMRO_DISABLE_RES msg
 ****************************************************************************/
static son_void_t
son_mif_mro_disable_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_mro_disable_res_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_mro_disable_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_mro_disable_res_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    if (SON_PNULL != p_in_message)
    {
        son_oam_send_son_oam_mro_disable_resp(
                &(p_in_message->disable_res),
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                p_in_message->transaction_id,
                SON_NULL);
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Pointer to SONMRO_DISABLE_RES data is NULL - ignore");
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_mro_disable_res */

/*****************************************************************************
 * Function Name  : son_mif_mro_modify_config_param_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SONMRO_MODIFY_CONFIG_PARAM_RES msg
 ****************************************************************************/
static son_void_t
son_mif_mro_modify_config_param_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_mro_modify_config_params_res_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_mro_modify_config_params_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_mro_modify_config_params_res_t *)((U8 *)p_msg +
            SON_API_HEADER_SIZE);

    if (SON_PNULL != p_in_message)
    {
        son_oam_send_son_oam_mro_modify_config_params_resp(
                &(p_in_message->config_param_res),
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                p_in_message->transaction_id,
                SON_NULL);
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Pointer to SONMRO_MODIFY_CONFIG_PARAM_RES data is NULL - "
                "ignore");
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_mro_modify_config_param_res */

/*****************************************************************************
 * Function Name  : son_mif_mro_ttt_update_ind
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles son_mro_ttt_update_ind msg
 ****************************************************************************/
static son_void_t
son_mif_mro_ttt_update_ind
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_mro_eutran_ttt_update_ind_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_oam_mro_eutran_ttt_update_ind_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_oam_mro_eutran_ttt_update_ind_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_mro_eutran_ttt_update_ind(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);
    SON_UT_TRACE_EXIT();
} /* son_mif_mro_ttt_update_ind */

/*****************************************************************************
 * Function Name  : son_mif_mro_ho_counter_report_handler 
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles son_mro_ho_counter_report msg
 ****************************************************************************/
static son_void_t
son_mif_mro_ho_counter_report_handler
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_mro_ho_counter_report_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_oam_mro_ho_counter_report_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_oam_mro_ho_counter_report_t *)((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_mro_ho_counter_report(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);
    SON_UT_TRACE_EXIT();
} /* son_mif_mro_ho_counter_report_handler */

#endif /* ifdef SON_MRO_FEATURE */

/*****************************************************************************
 * Function Name  : son_mif_mlb_feature_state_change_ind
* Inputs         : p_msg- pointer to the message
*                  cspl_hdr - CSPL Header
 *                  cspl_hdr CSPL header
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function handles SONMLB_FEATURE_STATE_CHANGE_IND msg
 *****************************************************************************/
static son_void_t
son_mif_mlb_feature_state_change_ind
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_mlb_feature_state_change_ind_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_mlb_feature_state_change_ind_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_mlb_feature_state_change_ind_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_feature_state_change_ind(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_mlb_feature_state_change_ind */

#ifdef SON_MRO_FEATURE

/*****************************************************************************
* Function Name  : son_mif_mro_optimization_notification
* Inputs         : pointer to the message received
*                  cspl_hdr CSPL header
* Outputs        : None
* Returns        : son_void_t
* Description    : This function handles SONMRO_OPTIMIZATION_NOTIFICATION msg
*****************************************************************************/
static son_void_t
son_mif_mro_optimization_notification
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_mro_optimization_notification_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
        sizeof(son_mro_optimization_notification_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_mro_optimization_notification_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_mro_optimization_notification(
        p_in_message,
        SON_MIF_MODULE_ID,
        SON_OAM_MODULE_ID,
        SON_NULL,
        SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_mro_optimization_notification */

/*****************************************************************************
 * Function Name  : son_mif_mro_feature_state_change_ind
 * Inputs         : pointer to the message received
 *                  cspl_hdr CSPL header
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function handles SONMRO_FEATURE_STATE_CHANGE_IND msg
 *****************************************************************************/
static son_void_t
son_mif_mro_feature_state_change_ind
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_mro_feature_state_change_ind_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_mro_feature_state_change_ind_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_mro_feature_state_change_ind_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_feature_state_change_ind(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_mro_feature_state_change_ind */

#endif /* ifdef SON_MRO_FEATURE */

/****************************************************************
 * SONAPPS Message Handlers
 ***************************************************************/

/*****************************************************************************
 * Function Name  : mif_compare_nw_scan_plmn_info_list_node
 * Inputs         : p_val_1  Pointer to the key field(trans_id)as void pointer
 *                : p_val_2  Pointer to the key field(trans_id)as void pointer
 * Outputs        : None
 * Returns        : Zero -  If matches
 *                  Non Zero - If not matches
 * Description    : This function compares the key field(trans_id) 
 *****************************************************************************/
son_s32
mif_compare_nw_scan_plmn_info_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
    son_s32 ret_value = -1;
    son_u16 *p_trans_id_1 = SON_PNULL;
    son_u16 *p_trans_id_2 = SON_PNULL;

    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name,
                SON_ERROR,
                "mif_compare_nw_scan_plmn_info_list_node: Pointer to the trans Id is NULL");
        return ret_value;
    }
    else
    {
        p_trans_id_1 = (son_u16 *)p_val_1;
        p_trans_id_2 = (son_u16 *)p_val_2;
    }

    if ((*p_trans_id_1) == (*p_trans_id_2))
    {
        ret_value = 0;
    }

    SON_UT_TRACE_EXIT();
    return ret_value;
} /* mif_compare_nw_scan_plmn_info_list_node */

/*****************************************************************************
 * Function Name  : mif_key_of_nw_scan_plmn_info_list
 * Inputs         : p_node - anchor field of the requested PLMNs info
 * Outputs        : None
 * Returns        : pointer to key field(trans_id) as void pointer
 * Description    : This function determines the key field(trans_id) 
 ******************************************************************************/
const son_void_t *
mif_key_of_nw_scan_plmn_info_list
(
 const SON_LIST_NODE *p_node
 )
{
    son_nw_scan_plmn_node_t *p_data = SON_PNULL;


    SON_UT_TRACE_ENTER();
    SON_ASSERT(SON_PNULL != p_node);

    p_data = YMEMBEROF(son_nw_scan_plmn_node_t,
            anchor, p_node);

    return (son_void_t *)(&p_data->nw_scan_plmn.transaction_id);
}

/*****************************************************************************
 * Function Name  : son_mif_find_record_in_nw_scan_plmn_info_list
 * Inputs         : p_list  : Pointer to PLMNs info List
 *                  Transaction Id
 * Outputs        : None
 * Returns        : son_nw_scan_plmn_node_t*
 * Description    : This function finds the requested PLMNs node for the
*  given transaction id.
 *****************************************************************************/
son_nw_scan_plmn_node_t *
son_mif_find_record_in_nw_scan_plmn_info_list
(
 SON_LIST                     *p_list,
 U16                          transaction_id 
 )
{
    SON_LIST_NODE                *p_node             = SON_PNULL;
    son_nw_scan_plmn_node_t  *p_list_node        = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_node = list_find_node(p_list, (son_void_t *)&transaction_id,
            mif_key_of_nw_scan_plmn_info_list,
            mif_compare_nw_scan_plmn_info_list_node);

    if (SON_PNULL != p_node)
    {
        p_list_node = YMEMBEROF(son_nw_scan_plmn_node_t,
                anchor, p_node);
    }

    SON_UT_TRACE_EXIT();
    return p_list_node;
} /* son_mif_find_record_in_nw_scan_plmn_info_list */

/*****************************************************************************
 * Function Name  : son_mif_apps_nw_scan_res_cleanup
 * Inputs         : pointer to the structure of message received from SONapps
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function frees up the memory of the lists present in
 *                  SONAPPS_NW_SCAN_RES msg recieved from SONapps
 *****************************************************************************/
static son_void_t
son_mif_apps_nw_scan_res_cleanup
(
    sonapps_nw_scan_res_t *p_in_message
)
{
    /*Pointer to the EARFCN scan res list */
    earfcn_scan_res_list_t *p_earfcn_scan_res_list = SON_PNULL;

    /*Pointer to the EARFCN scan res list node*/
    earfcn_scan_res_data_t *p_earfcn_scan_res_data = SON_PNULL;

    /*Pointer to the  cell info list */
    cell_info_list_t *p_cell_info_list = SON_PNULL;

    /*Pointer to the cell info list node*/
    cell_info_data_t *p_cell_info_data = SON_PNULL; 
    
    U16 cell_index = SON_NULL;

    U32 count = SON_NULL;
    U32 no_of_nodes = SON_NULL;
    U32 earfcn_index = SON_NULL;


    SON_UT_TRACE_ENTER();

    /* Get the pointer to the earfcn list received from APPS */
    p_earfcn_scan_res_list = p_in_message->p_earfcn_list;
    if (SON_PNULL == p_earfcn_scan_res_list)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name,
                SON_ERROR, "son_mif_apps_nw_scan_res_cleanup: Failed to read "
                "data from APPS earfcn scan res list");
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Get the pointer to the earfcn list received from APPS */
    p_earfcn_scan_res_list = p_in_message->p_earfcn_list;

    no_of_nodes = p_earfcn_scan_res_list->count;

    /* Iterate through the response list */
    for (earfcn_index = 0; earfcn_index < no_of_nodes; earfcn_index++)
    {
        /* Get data from the earfcn scan res list */
        EARFCN_SCAN_LIST_DEQUEUE(p_earfcn_scan_res_list,
                 p_earfcn_scan_res_data);
        if (SON_PNULL != p_earfcn_scan_res_data)
        {
            p_cell_info_list = p_earfcn_scan_res_data->earfcn_data.
                                p_cell_info_list;
            if (SON_PNULL == p_cell_info_list)
            {
                SON_LOG(g_mif_log_on_off, p_son_mif_facility_name,
                        SON_ERROR, "son_mif_apps_nw_scan_res_cleanup: "
                        "Failed to read "
                        "data from APPS cell info list");
                SON_UT_TRACE_EXIT();
                return;
            }

            cell_index = p_cell_info_list->count;
            /* Iterate through the response list */
            for (count = 0; count < cell_index; count++)
            {
                /* Get data from the cell info list */
                CELL_INFO_LIST_DEQUEUE(p_cell_info_list, p_cell_info_data);
                if (SON_PNULL != p_cell_info_data)
                {
                    /* Free the memory of the cell info list pointer */
                    son_mem_free(p_cell_info_data);
                    p_cell_info_data = SON_PNULL;
                }
            }

            /* Free the memory of the cell info list pointer */
            son_mem_free(p_cell_info_list);
            p_cell_info_list = SON_PNULL;

            /* Free the memory of the node from earfcn scan res list */
            son_mem_free(p_earfcn_scan_res_data);
            p_earfcn_scan_res_data = SON_PNULL;
        }
    }

    /* Free the memory of the earfcn scan res list pointer */
    son_mem_free(p_earfcn_scan_res_list);
} /* son_mif_apps_nw_scan_res_cleanup */

/* REL1.1 CHANGES : STARTS */

/*****************************************************************************
  * Function Name  : son_mif_apps_pci_conflict_enable_disable_res 
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : son_void_t
  * Description    : This function handles SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_RES msg
  ****************************************************************************/
static son_void_t
son_mif_apps_pci_conflict_enable_disable_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    sonapps_pci_conflict_enable_disable_res_t     *p_in_message = SON_PNULL;
    son_oam_pci_conflict_enable_disable_resp_t     out_message;	


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(sonapps_pci_conflict_enable_disable_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (sonapps_pci_conflict_enable_disable_res_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));
          
    SON_MEMCPY(&out_message, &(p_in_message->pci_enable_disable_res),
        sizeof(p_in_message->pci_enable_disable_res));

    son_oam_send_son_oam_pci_conflict_enable_disable_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_apps_pci_conflict_enable_disable_res */

/*****************************************************************************
  * Function Name  : son_mif_apps_pci_selection_res 
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : son_void_t
  * Description    : This function handles SONAPPS_PCI_SELECTION_RES msg
  ****************************************************************************/
static son_void_t
son_mif_apps_pci_selection_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    sonapps_pci_selection_res_t     *p_in_message = SON_PNULL;
    son_oam_pci_selection_resp_t     out_message;	


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(sonapps_pci_selection_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (sonapps_pci_selection_res_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));
          
    SON_MEMCPY(&out_message, &(p_in_message->pci_selection_res),
        sizeof(p_in_message->pci_selection_res));

    son_oam_send_son_oam_pci_selection_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_apps_pci_selection_res */

/*****************************************************************************
  * Function Name  : son_mif_apps_pci_reselection_res 
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : son_void_t
  * Description    : This function handles SONAPPS_PCI_RESELECTION_RES msg
  ****************************************************************************/
static son_void_t
son_mif_apps_pci_reselection_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    sonapps_pci_reselection_res_t     *p_in_message = SON_PNULL;
    son_oam_pci_reselection_resp_t     out_message;	


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(sonapps_pci_reselection_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (sonapps_pci_reselection_res_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));
          
    SON_MEMCPY(&out_message, &(p_in_message->pci_reselection_res),
        sizeof(p_in_message->pci_reselection_res));

    son_oam_send_son_oam_pci_reselection_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_apps_pci_reselection_res */

/********************************************************************************
  * Function Name  : son_mif_apps_pci_conflict_confusion_ind 
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : son_void_t
  * Description    : This function handles SONAPPS_PCI_CONFLICT_CONFUSION_IND msg
  *******************************************************************************/
static son_void_t
son_mif_apps_pci_conflict_confusion_ind
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    sonapps_pci_conflict_confusion_ind_t     *p_in_message = SON_PNULL;
    son_oam_pci_conflict_confusion_ind_t      out_message;	


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(sonapps_pci_conflict_confusion_ind_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (sonapps_pci_conflict_confusion_ind_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));
          
    SON_MEMCPY(&out_message, (U8 *)p_in_message,
        sizeof(son_oam_pci_conflict_confusion_ind_t));

    son_oam_send_son_oam_pci_conflict_confusion_ind(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_apps_pci_conflict_confusion_ind */

/********************************************************************************
  * Function Name  : son_mif_apps_carrier_freq_selection_res 
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : son_void_t
  * Description    : This function handles SONAPPS_CARRIER_FREQ_SELECTION_RES msg
  *******************************************************************************/
static son_void_t
son_mif_apps_carrier_freq_selection_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    sonapps_carrier_freq_selection_res_t      *p_in_message = SON_PNULL;
    son_oam_carrier_freq_selection_resp_t      out_message;	


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(sonapps_carrier_freq_selection_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (sonapps_carrier_freq_selection_res_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));
          
    SON_MEMCPY(&out_message, &(p_in_message->carrier_freq_selection_res),
        sizeof(p_in_message->carrier_freq_selection_res));

    son_oam_send_son_oam_carrier_freq_selection_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_apps_carrier_freq_selection_res */

/* REL1.1 CHANGES : ENDS */

/* Rel 1.4 Start */

/********************************************************************************
  * Function Name  : son_mif_apps_carrier_freq_and_dl_tx_power_res
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : son_void_t
  * Description    : This function handles SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_RESP msg
  *******************************************************************************/
static son_void_t
son_mif_apps_carrier_freq_and_dl_tx_power_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    sonapps_carrier_freq_and_power_selection_res_t  *p_in_message = SON_PNULL;
    son_oam_carrier_freq_and_dl_tx_power_resp_t     out_message;


    SON_UT_TRACE_ENTER();

    SON_ASSERT(SON_PNULL != p_msg);

    if (cspl_hdr.paramlen !=
            sizeof(sonapps_carrier_freq_and_power_selection_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (sonapps_carrier_freq_and_power_selection_res_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    SON_MEMCPY(&out_message, &(p_in_message->carrier_freq_dl_pwr_selection_resp),
        sizeof(p_in_message->carrier_freq_dl_pwr_selection_resp));

    son_oam_send_son_oam_carrier_freq_and_dl_tx_power_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            p_in_message->transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_apps_carrier_freq_and_dl_tx_power_res */

/* Rel 1.4 Start */

/*****************************************************************************
  * Function Name  : son_mif_apps_nw_scan_res
  * Inputs         : pointer to the message received
  *                  CSPL header
  *                  Pointer ti mif global context
  * Outputs        : None
  * Returns        : son_void_t
  * Description    : This function handles SONAPPS_NW_SCAN_RES msg
  ****************************************************************************/
static son_void_t
son_mif_apps_nw_scan_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr,
    son_mif_gb_context_t   *p_smif_gb_context
)
{
    sonapps_nw_scan_res_t *p_in_message = SON_PNULL;
    U16 cell_index = 0;

    /* NW Scan Response to be sent to OAM */
    son_oam_bulk_nw_scan_resp_t *p_oam_scan_res = SON_PNULL;	
    son_size_t oam_scan_res_len = sizeof(son_oam_bulk_nw_scan_resp_t);
    /*Pointer to the EARFCN scan res list */
    earfcn_scan_res_list_t *p_earfcn_scan_res_list = SON_PNULL; 
    
    /*Pointer to the EARFCN scan res list node*/
    earfcn_scan_res_data_t *p_earfcn_scan_res_data = SON_PNULL; 

    /*Pointer to the  cell info list */
    cell_info_list_t *p_cell_info_list = SON_PNULL; 
    
    /*Pointer to the cell info list node*/
    cell_info_data_t *p_cell_info_data = SON_PNULL; 

    U32 count = SON_NULL;
    U32 no_of_nodes = SON_NULL;
    U32 earfcn_index = SON_NULL;
    U32 cell_plmn_index = SON_NULL;
    U32 plmn_list_index = SON_NULL;
    U8 plmn_index = SON_NULL;
    S32 result = SON_NULL;


    SON_UT_TRACE_ENTER();

    son_nw_scan_plmn_node_t *p_list_node     = SON_PNULL;

    p_oam_scan_res = (son_oam_bulk_nw_scan_resp_t *)
        son_mem_get(oam_scan_res_len);
    if (SON_PNULL == p_oam_scan_res)
    {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
                    "Memory allocation failed. "
                    "Can't send Network Scan response message to OAM.");
    }
    else
    {
        SON_MEMSET(p_oam_scan_res, 0, oam_scan_res_len);

        p_in_message = (sonapps_nw_scan_res_t *)((U8 *)p_msg +
            SON_API_HEADER_SIZE);
        p_oam_scan_res->result = p_in_message->result;
        p_oam_scan_res->error_code = p_in_message->error_code;
        result = p_in_message->result;
        p_list_node = son_mif_find_record_in_nw_scan_plmn_info_list(
            &p_smif_gb_context->nw_scan_plmn_list, 
            p_in_message->transaction_id);

        if (cspl_hdr.paramlen !=
                sizeof(sonapps_nw_scan_res_t))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                    "Wrong message size - ignore.");

            /* If NW Scanning is Successful or has Partial Successful results */
            if (SON_SUCCESS == result || SON_PARTIAL_SUCCESS == result)
            {
                son_mif_apps_nw_scan_res_cleanup(p_in_message);
            }

            p_oam_scan_res->result = SON_FAILURE;
            p_oam_scan_res->error_code = SON_ERR_INVALID_PARAMS;
        }
        else
        {
            /* If NW Scanning is Successful or has Partial Successful results */
            if (SON_SUCCESS == result || SON_PARTIAL_SUCCESS == result)
            {
                /* Get the pointer to the earfcn list received from APPS */
                p_earfcn_scan_res_list = p_in_message->p_earfcn_list;
                if (SON_PNULL == p_earfcn_scan_res_list)
                {
                    SON_LOG(g_mif_log_on_off, p_son_mif_facility_name,
                            SON_ERROR, "Failed to "
                            "read data from APPS earfcn scan res list");
                    p_oam_scan_res->result = SON_FAILURE;
                    p_oam_scan_res->error_code = SON_ERR_INVALID_PARAMS;
                }
                else
                {
                    p_oam_scan_res->nw_scan_resp.bitmask = 
                        SON_INTRA_RAT_BULK_NW_SCAN_RESP_PRESENT;
                    p_oam_scan_res->nw_scan_resp.intra_rat_scan_resp.
                        rf_freq_channel_list_size = 
                        p_earfcn_scan_res_list->count;

                    no_of_nodes = p_earfcn_scan_res_list->count;

                    /* Iterate through the response list */
                    for (earfcn_index = 0; earfcn_index < no_of_nodes;
                            earfcn_index++)
                    {
                        /* Get data from the earfcn scan res list */
                        EARFCN_SCAN_LIST_DEQUEUE(p_earfcn_scan_res_list,
                                p_earfcn_scan_res_data);
                        if (SON_PNULL != p_earfcn_scan_res_data)
                        {
                            p_oam_scan_res->nw_scan_resp.intra_rat_scan_resp.
                                rf_freq_channel_num[earfcn_index].earfcn =
                                p_earfcn_scan_res_data->earfcn_data.earfcn;

                            p_oam_scan_res->nw_scan_resp.intra_rat_scan_resp.
                                rf_freq_channel_num[earfcn_index].rssi =
                                p_earfcn_scan_res_data->earfcn_data.rssi;

                            p_cell_info_list = p_earfcn_scan_res_data->
                                earfcn_data.p_cell_info_list;
                            SON_LOG(g_mif_log_on_off, 
                                    p_son_mif_facility_name,
                                    SON_DETAILED,
                                    "Success to read data from APPS cell "
                                    "info list %p", p_cell_info_list);

                            if (SON_PNULL == p_cell_info_list)
                            {
                                SON_LOG(g_mif_log_on_off, 
                                        p_son_mif_facility_name,
                                        SON_ERROR,
                                        "Failed to read data from APPS cell "
                                        "info list");
                                /* SPR 15935 Fix Start */
                                son_mem_free(p_oam_scan_res);
                                p_oam_scan_res = SON_PNULL;
                                /* SPR 15935 Fix Stop */
                                SON_UT_TRACE_EXIT();
                                return;
                            } 

                            p_oam_scan_res->nw_scan_resp.intra_rat_scan_resp.
                                rf_freq_channel_num[earfcn_index].
                                cell_list_size =
                                p_cell_info_list->count;
                            cell_index = p_cell_info_list->count;

                            /* Iterate through the response list */
                            for (count = 0; count < cell_index; 
                                    count++)
                            {
                                /* Get data from the cell info list */
                                CELL_INFO_LIST_DEQUEUE(p_cell_info_list, 
                                        p_cell_info_data);
                                if (SON_PNULL != p_cell_info_data)
                                {
                                    /* Copy the results for sending to OAM */
                                    p_oam_scan_res->nw_scan_resp.
                                        intra_rat_scan_resp.
                                        rf_freq_channel_num[earfcn_index].
                                        cell_list[count].bcch_list.bitmask =
                                        p_cell_info_data->cell_info.bitmask;

                                    p_oam_scan_res->nw_scan_resp.
                                        intra_rat_scan_resp.
                                        rf_freq_channel_num[earfcn_index].
                                        cell_list[count].rf_list.pci_id =
                                        p_cell_info_data->cell_info.pci_id;

                                    p_oam_scan_res->nw_scan_resp.
                                        intra_rat_scan_resp.
                                        rf_freq_channel_num[earfcn_index].
                                        cell_list[count].rf_list.rsrq =
                                        p_cell_info_data->cell_info.rsrq;

                                    p_oam_scan_res->nw_scan_resp.
                                        intra_rat_scan_resp.
                                        rf_freq_channel_num[earfcn_index].
                                        cell_list[count].rf_list.rsrp =
                                        p_cell_info_data->cell_info.rsrp;

                                    SON_MEMCPY(p_oam_scan_res->nw_scan_resp.
                                            intra_rat_scan_resp.
                                            rf_freq_channel_num
                                            [earfcn_index].cell_list[count].
                                            bcch_list.cell_Id,
                                            p_cell_info_data->cell_info.cell_Id, 
                                            SON_CELL_ID_OCTET_SIZE * sizeof(U8));

                                    p_oam_scan_res->nw_scan_resp.
                                        intra_rat_scan_resp.
                                        rf_freq_channel_num[earfcn_index].
                                        cell_list[count].bcch_list.cell_barred =
                                        p_cell_info_data->cell_info.cell_barred;

                                    p_oam_scan_res->nw_scan_resp.
                                        intra_rat_scan_resp.
                                        rf_freq_channel_num[earfcn_index].
                                        cell_list[count].bcch_list.csg_indication = 
                                        p_cell_info_data->cell_info.csg_indication;

                                    if (p_oam_scan_res->nw_scan_resp.
                                            intra_rat_scan_resp.
                                            rf_freq_channel_num[earfcn_index].
                                            cell_list[count].bcch_list.bitmask 
                                            & SON_CSG_ID_PRESENT)
                                    {
                                        SON_MEMCPY(p_oam_scan_res->nw_scan_resp.
                                                intra_rat_scan_resp.
                                                rf_freq_channel_num[
                                                earfcn_index].
                                                cell_list[count].bcch_list.
                                                csg_identity,
                                                p_cell_info_data->cell_info.
                                                csg_identity, 
                                                SON_CSG_ID_OCTET_SIZE * 
                                                sizeof(U8));
                                    }

                                    if (p_cell_info_data->cell_info.num_plmn_id
                                        != 0)
                                    {
                                        if (SON_PNULL != p_list_node)
                                        {
                                            for (plmn_list_index = 0;
                                                  p_list_node->nw_scan_plmn.num_plmn_id > plmn_list_index;
                                                  plmn_list_index++)
                                            {
                                                for (cell_plmn_index = 0;
                                                        p_cell_info_data->cell_info.num_plmn_id > cell_plmn_index;
                                                        cell_plmn_index++)
                                                {    
                                                    if (0 ==
                                                        SON_MEMCMP(&p_list_node->nw_scan_plmn.plmn_id[plmn_list_index],
                                                            &p_cell_info_data->cell_info.plmn_identity_info[
                                                                cell_plmn_index].plmn_id,
                                                                sizeof(son_cell_plmn_info_t)))
                                                    {
                                                        p_oam_scan_res->nw_scan_resp.
                                                        intra_rat_scan_resp.
                                                        rf_freq_channel_num[earfcn_index].
                                                        cell_list[count].bcch_list.bitmask 
                                                        |= SON_BCCH_PLMN_ID_PRESENT;
                                                        SON_MEMCPY(&p_oam_scan_res->nw_scan_resp.
                                                                intra_rat_scan_resp.
                                                                rf_freq_channel_num[
                                                                earfcn_index].cell_list[count].
                                                                bcch_list.plmn_identity_info[plmn_index],
                                                            &p_cell_info_data->cell_info.plmn_identity_info[
                                                                cell_plmn_index],
                                                                sizeof(son_plmn_identity_info_t));
                                                        plmn_index++;
                                                    }
                                                }
                                            }

                                            p_oam_scan_res->nw_scan_resp.
                                                intra_rat_scan_resp.
                                                rf_freq_channel_num[earfcn_index].
                                                cell_list[count].bcch_list.
                                                num_plmn_id = plmn_index;
                                        }
					else
					{    
                                        p_oam_scan_res->nw_scan_resp.
                                            intra_rat_scan_resp.
                                            rf_freq_channel_num[earfcn_index].
                                            cell_list[count].bcch_list.bitmask 
                                            |= SON_BCCH_PLMN_ID_PRESENT;
                                        p_oam_scan_res->nw_scan_resp.
                                            intra_rat_scan_resp.
                                            rf_freq_channel_num[earfcn_index].
                                            cell_list[count].bcch_list.
                                            num_plmn_id =
                                            p_cell_info_data->cell_info.
                                            num_plmn_id;

                                        SON_MEMCPY(p_oam_scan_res->nw_scan_resp.
                                                intra_rat_scan_resp.
                                                rf_freq_channel_num[
                                                earfcn_index].cell_list[count].
								bcch_list.plmn_identity_info,
                                                p_cell_info_data->cell_info.
								plmn_identity_info, 
                                                p_cell_info_data->cell_info.
                                                num_plmn_id * 
								sizeof(son_plmn_identity_info_t));
					}
                                    }

                                    SON_MEMCPY(p_oam_scan_res->nw_scan_resp.
                                            intra_rat_scan_resp.
                                            rf_freq_channel_num[earfcn_index].
                                            cell_list[count].bcch_list.tac,
                                            p_cell_info_data->cell_info.tac, 
                                            SON_TAC_OCTET_SIZE * sizeof(U8));

                                    p_oam_scan_res->nw_scan_resp.
                                        intra_rat_scan_resp.
                                        rf_freq_channel_num[earfcn_index].
                                        cell_list[count].bitmask = 
                                        SON_BCCH_LIST_PRESENT;

                                    p_oam_scan_res->nw_scan_resp.
                                        intra_rat_scan_resp.
                                        rf_freq_channel_num[earfcn_index].
                                        cell_list[count].bcch_list.rsTxPower =
                                        p_cell_info_data->cell_info.rsTxPower;

                                    p_oam_scan_res->nw_scan_resp.
                                        intra_rat_scan_resp.
                                        rf_freq_channel_num[earfcn_index].
                                        cell_list[count].bcch_list.dl_sys_bw =
                                        p_cell_info_data->cell_info.dl_sys_bw;

                                    p_oam_scan_res->nw_scan_resp.
                                        intra_rat_scan_resp.
                                        rf_freq_channel_num[earfcn_index].
                                        cell_list[count].bcch_list.ul_sys_bw =
                                        p_cell_info_data->cell_info.ul_sys_bw;

                                    /* Free the memory of the node from 
                                     * p_cell_info_list */
                                    son_mem_free(p_cell_info_data);
                                    p_cell_info_data = SON_PNULL;
                                }
                            }

                            /* Free the memory of the cell info list pointer */
                            son_mem_free(p_cell_info_list);
                            p_cell_info_list = SON_PNULL;

                            /* Free the memory of the node from earfcn scan res 
                             * list */
                            son_mem_free(p_earfcn_scan_res_data);
                            p_earfcn_scan_res_data = SON_PNULL;
                        }
                    }

                    /* Free the memory of the earfcn scan res list pointer */
                    son_mem_free(p_earfcn_scan_res_list);
                }
            }
        }

        son_oam_send_son_oam_bulk_nw_scan_resp(
                p_oam_scan_res,
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                p_in_message->transaction_id,
                SON_NULL);
        son_mem_free(p_oam_scan_res);
    }

    if (SON_PNULL != p_list_node)
    {
        list_delete_node(&p_smif_gb_context->nw_scan_plmn_list, &p_list_node->anchor);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_apps_nw_scan_res */

/*******************************************************************
 * OAM Agent Message Handlers
  ******************************************************************/

/*****************************************************************************
  * Function Name  : son_mif_send_set_log_level_to_dest_module
  * Inputs         : Destination module ID
  *                  Log Level to be set
  *                : p_smif_gb_context - pointer to the SMIF global context
  * Outputs        : None
  * Returns        : None
  * Description    : This function sends SMIF_SET_LOG_LEVEL_CMD to 
  *		     SON internal modules
  ****************************************************************************/
static son_void_t
son_mif_send_set_log_level_to_dest_module
(
    son_module_id_et dest_module_id,
    /* SPR-13251 Fix Starts */
    son_oam_log_level_et log_level,
    son_mif_gb_context_t *p_smif_gb_context
    /* SPR-13251 Fix Ends */
)
{
    smif_set_log_level_cmd_t out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    /* If the destination module is SON MgmtIfH itself, then set the self log 
     * level otherwise send SMIF_SET_LOG_LEVEL_CMD to the specified internal
     * module of SON */
    if (SON_MIF_MODULE_ID == dest_module_id)
    {
        /* SPR-13251 Fix Starts */
        son_mif_set_log_level_cmd(log_level, p_smif_gb_context);
        /* SPR-13251 Fix Ends */
    }
    else
    {
        out_message.log_level = log_level;

        son_mif_send_internal_msg(dest_module_id,
                SMIF_SET_LOG_LEVEL_CMD,
                sizeof(out_message),
                &out_message);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_send_set_log_level_to_dest_module */

/* SPR-13251 Fix Starts */
/*****************************************************************************
  * Function Name  : son_mif_send_get_log_level_req_to_dest_module
  * Inputs         : Transaction ID for the request
  *                : dest_module_id - Destination module ID
  * Outputs        : None
  * Returns        : None
  * Description    : This function sends SMIF_GET_LOG_LEVEL_REQ to SON 
  *                  internal modules
  ****************************************************************************/
static son_void_t
son_mif_send_get_log_level_req_to_dest_module
(
    son_transaction_id_t    transaction_id,
    son_module_id_et        dest_module_id
)
{
    smif_get_log_level_req_t out_message;    
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    out_message.transaction_id = transaction_id;

    son_mif_send_internal_msg(dest_module_id,
            SMIF_GET_LOG_LEVEL_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_send_get_log_level_req_to_dest_module */
/* SPR-13251 Fix Ends */

/*****************************************************************************
  * Function Name  : son_mif_oam_set_log_level_resp
  * Inputs         : Transaction ID for the response
  *                : Result (SUCCESS/FAILURE) 
  * Outputs        : None
  * Returns        : None
  * Description    : This function sends SON_OAM_SET_LOG_LEVEL_RESP msg to OAM
  *                  after sending the SMIF_SET_LOG_LEVEL_CMD to all the 
  *                  SON internal modules
  ****************************************************************************/
static son_void_t
son_mif_oam_set_log_level_resp
(
    son_transaction_id_t transaction_id,
    son_return_et result,
    son_error_et error_code
)
{
    son_oam_set_log_level_resp_t    out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    out_message.result = result;
    out_message.error_code = error_code;

    son_oam_send_son_oam_set_log_level_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_set_log_level_resp */

/* SPR-13251 Fix Starts */
/*****************************************************************************
  * Function Name  : son_mif_oam_send_get_log_level_resp
  * Inputs         : transaction_id Transaction ID for the response
  *                : result     Result (SUCCESS/FAILURE) 
  *                : error_code Error Code in case of failure
  *                : module_id  Module Id
  *                : log_level  Log Level of the module
  * Outputs        : None
  * Returns        : None
  * Description    : This function sends SON_OAM_GET_LOG_LEVEL_RESP msg to OAM
  ****************************************************************************/
son_void_t
son_mif_oam_send_get_log_level_resp
(
    son_transaction_id_t    transaction_id,
    son_return_et           result,
    son_error_et            error_code,
    son_module_id_et        module_id,
    son_u8                  log_level
)
{
    son_oam_get_log_level_resp_t out_message;
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    out_message.result = result;
    out_message.error_code = error_code;
    if (SON_FAILURE == result)
    {
        out_message.num_of_modules = 0;
    }
    else
    {
        out_message.num_of_modules = 1;
        out_message.module_log_level[0].module_id = module_id;
        if (SON_LOG_LEVEL_NONE != log_level)
        {
            out_message.module_log_level[0].bitmask |= 
                SON_MODULE_LOG_LEVEL_PRESENT;
            out_message.module_log_level[0].log_level = 
                (son_oam_log_level_et)log_level;
            out_message.module_log_level[0].error_code = SON_NO_ERROR;
        }
        else
        {
            out_message.module_log_level[0].error_code = 
                SON_ERR_LOGGING_DISABLED;
        }
    }

    son_oam_send_son_oam_get_log_level_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_send_get_log_level_resp */
/* SPR-13251 Fix Ends */

/*****************************************************************************
  * Function Name  : son_mif_oam_log_enable_disable_resp
  * Inputs         : Transaction ID for the response
  *                : Result (SUCCESS/FAILURE) 
  * Outputs        : None
  * Returns        : None
  * Description    : This function sends SON_OAM_LOG_ENABLE_DISABLE_RESP msg 
  *                  to OAM after sending the SMIF_LOG_ENABLE_DISABLE_CMD to 
  *                  all the SON internal modules
  ****************************************************************************/
static son_void_t
son_mif_oam_log_enable_disable_resp
(
    son_transaction_id_t transaction_id,
    son_return_et result,
    son_error_et error_code
)
{
    son_oam_log_enable_disable_resp_t	out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    out_message.result = result;
    out_message.error_code = error_code;

    son_oam_send_son_oam_log_enable_disable_resp(
            &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            transaction_id,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_log_enable_disable_resp */

/* REL1.1 CHANGES : STARTS */

/*****************************************************************************
  * Function Name  : son_mif_send_enable_disable_log_to_dest_module
  * Inputs         : Destination module ID
  *		     Log Enable/Disable
  *		     Log Level to be set
  *                : p_smif_gb_context - pointer to the SMIF global context
  * Outputs        : None
  * Returns        : None
  * Description    : This function sends SMIF_LOG_ENABLE_DISABLE_CMD msg to 
  *		     SON internal modules
  ****************************************************************************/
static son_void_t
son_mif_send_enable_disable_log_to_dest_module
(
                                son_module_id_et dest_module_id,
                                son_feature_id_et feature_id,
                                son_oam_log_on_off_et log_on_off,
                                son_bool_et log_level_present,
                                /* SPR-13251 Fix Starts */
                                son_oam_log_level_et log_level,
                                son_mif_gb_context_t   *p_smif_gb_context 
                                /* SPR-13251 Fix Ends */
)
{
    smif_log_enable_disable_cmd_t   out_message;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));

    /* If the destination is SON MgmtIfH itself, then set the logging 
     *  parameters for self otherwise send SMIF_LOG_ENABLE_DISABLE_CMD to the
     *  specified internal mosules of SON */
    if (SON_MIF_MODULE_ID == dest_module_id)
    {
        /* SPR-13251 Fix Starts */
        son_mif_log_enable_disable_cmd(log_on_off, log_level_present, 
                log_level, p_smif_gb_context);
        /* SPR-13251 Fix Ends */
    }
    else
    {
        out_message.log_enable = log_on_off;
        out_message.log_level  = log_level;
        out_message.feature_id = feature_id;
        if (SON_TRUE == log_level_present)
        {
            out_message.bitmask = SMIF_MODULE_LOG_LEVEL_PRESENT;
        }

        son_mif_send_internal_msg(dest_module_id,
                SMIF_LOG_ENABLE_DISABLE_CMD,
                sizeof(out_message),
                &out_message);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_send_enable_disable_log_to_dest_module */

/*****************************************************************************
  * Function Name  : son_mif_oam_log_enable_disable_req 
  * Inputs         : p_api- Pointer to the api received
  *                : p_smif_gb_context - pointer to the SMIF global context
  *                : cspl_hdr - CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_LOG_ENABLE_DISABLE_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_log_enable_disable_req
(
 son_void_t             *p_api,
 son_mif_gb_context_t   *p_smif_gb_context, 
 STACKAPIHDR            cspl_hdr
 )
{
    son_oam_log_enable_disable_req_t    in_message;
    S32                                 length_read    = SON_NULL;
    son_transaction_id_t                transaction_id = SON_NULL;
    son_return_et                       result         = SON_SUCCESS;
    son_error_et                        error_code     = SON_NO_ERROR;

    son_module_id_et                    dest_module_id;
    son_feature_id_et                   feature_id;
    son_oam_log_on_off_et               log_on_off;
    son_oam_log_level_et                log_level;
    son_bool_et                         log_level_present = SON_FALSE;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&in_message, 0, sizeof(in_message));

    if (SON_SUCCESS !=
            son_parse_son_oam_log_enable_disable_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    transaction_id = son_get_transaction_id(p_api);
    log_on_off = in_message.log_configuration.log_on_off;
    log_level = in_message.log_configuration.log_level;
    feature_id = in_message.feature_id;
    if (!(in_message.log_configuration.bitmask & SON_OAM_MODULE_LOG_LEVEL_PRESENT))
    {
        log_level = SON_OAM_LOG_LEVEL_BRIEF;
    }

    log_level_present = SON_TRUE;

    /* If global request, set logging parameters for all the SON internal 
     * modules */
    if (in_message.module_id == SON_ALL_INT_MODULES)
    {
        for (dest_module_id = SON_MIN_INT_MODULE_ID;
            dest_module_id <= SON_MAX_INT_MODULE_ID; dest_module_id++)
        {
            /* MRO FIX START */
#ifdef SON_MRO_FEATURE
            /* SPR-13251 Fix Starts */
            son_mif_send_enable_disable_log_to_dest_module(dest_module_id,
                feature_id,
                log_on_off,
                log_level_present,
                log_level,
                p_smif_gb_context);
            /* SPR-13251 Fix Ends */
#else
            if (SON_MRO_MODULE_ID != dest_module_id)
            {
                /* SPR-13251 Fix Starts */
                son_mif_send_enable_disable_log_to_dest_module(dest_module_id,
                    feature_id,
                    log_on_off,
                    log_level_present,
                    log_level,
                    p_smif_gb_context);
                /* SPR-13251 Fix Ends */
            }

#endif
        }
    }

    /* Request for a particular module */
#ifdef SON_MRO_FEATURE
    else if ((in_message.module_id >= SON_MIN_INT_MODULE_ID) &&
             (in_message.module_id <= SON_MAX_INT_MODULE_ID))
#else
    else if ((in_message.module_id >= SON_MIN_INT_MODULE_ID) &&
             (in_message.module_id <= SON_MAX_INT_MODULE_ID) &&
             (SON_MRO_MODULE_ID != in_message.module_id))
#endif
    /* MRO FIX END */
    {
        dest_module_id = in_message.module_id;
        /* SPR-13251 Fix Starts */
        son_mif_send_enable_disable_log_to_dest_module(dest_module_id,
            feature_id,
            log_on_off,
            log_level_present,
            log_level,
            p_smif_gb_context);
        /* SPR-13251 Fix Ends */
    }
    /* Request came for an incorrect module-id */
    else
    {
        SON_LOG( g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Parameter [Module_id] should be in range [%d <= %d <= %d] !",
            SON_MIN_INT_MODULE_ID, in_message.module_id, SON_MAX_INT_MODULE_ID);
        result = SON_FAILURE;
        error_code = SON_ERR_INVALID_PARAMS;
    }

    /* send the response to OAM after sending logging parameters to the
     * specified SON internal module(s) */
    son_mif_oam_log_enable_disable_resp( transaction_id, result, error_code);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_log_enable_disable_req */

/* REL1.1 CHANGES : ENDS */

/*****************************************************************************
  * Function Name  : son_mif_oam_set_log_level_req 
  * Inputs         : p_api- Pointer to the api received
  *                : p_smif_gb_context - pointer to the SMIF global context
  *                : cspl_hdr - CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_SET_LOG_LEVEL_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_set_log_level_req
(
    son_void_t *p_api,
    son_mif_gb_context_t *p_smif_gb_context,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_set_log_level_req_t    in_message;
    smif_set_log_level_cmd_t       out_message;
    S32                            length_read = SON_NULL;
    son_transaction_id_t           transaction_id = SON_NULL;
    son_return_et                  result = SON_SUCCESS;
    son_error_et                   error_code = SON_NO_ERROR;

    son_module_id_et dest_module_id;
    son_oam_log_level_et log_level;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_set_log_level_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    transaction_id = son_get_transaction_id(p_api);
    log_level = in_message.log_level;

    /* If global request, send SMIF_SET_LOG_LEVEL_CMD to all the SON internal 
     *  modules */
    if (in_message.module_id == SON_ALL_INT_MODULES)
    {
        for (dest_module_id = SON_MIN_INT_MODULE_ID;
            dest_module_id <= SON_MAX_INT_MODULE_ID; dest_module_id++)
        {
            /* MRO FIX START */
#ifdef SON_MRO_FEATURE
            /* SPR-13251 Fix Starts */
            son_mif_send_set_log_level_to_dest_module(dest_module_id, log_level,
                    p_smif_gb_context);
            /* SPR-13251 Fix Ends */
#else
            if (SON_MRO_MODULE_ID != dest_module_id)
            {
                /* SPR-13251 Fix Starts */
                son_mif_send_set_log_level_to_dest_module(dest_module_id, 
                        log_level, p_smif_gb_context);
                /* SPR-13251 Fix Ends */
        }

#endif
        }

        /* SPR-13251 Fix Starts */
        son_mif_send_set_log_level_to_dest_module(SON_X2_MODULE_ID, log_level,
                p_smif_gb_context);
        /* SPR-13251 Fix Ends */
    }

    /* Request for a particular module */
#ifdef SON_MRO_FEATURE
    else if ((in_message.module_id >= SON_MIN_INT_MODULE_ID) && 
                (in_message.module_id <= SON_MAX_INT_MODULE_ID))
#else
    else if ((in_message.module_id >= SON_MIN_INT_MODULE_ID) && 
                (in_message.module_id <= SON_MAX_INT_MODULE_ID) &&
                (SON_MRO_MODULE_ID != in_message.module_id))
#endif
    {
        dest_module_id = in_message.module_id;
        
        /* SPR-13251 Fix Starts */
        son_mif_send_set_log_level_to_dest_module(dest_module_id, log_level,
                p_smif_gb_context);
        /* SPR-13251 Fix Ends */

        if (SON_ANR_MODULE_ID == dest_module_id)
        {
            /* SPR-13251 Fix Starts */
            son_mif_send_set_log_level_to_dest_module(SON_X2_MODULE_ID, 
                    log_level, p_smif_gb_context);
            /* SPR-13251 Fix Ends */
        }

        /* MRO FIX END */
    }
    /* Request came for an incorrect module-id */
    else
    {
        SON_LOG( g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Parameter [Module_id] should be in range [%d <= %d <= %d] !", 
            SON_MIN_INT_MODULE_ID, in_message.module_id, SON_MAX_INT_MODULE_ID);
        result = SON_FAILURE;
        error_code = SON_ERR_INVALID_PARAMS;
    }

    /* send the response to OAM after sending logging parameters to the 
     * specified SON internal module(s) */
    son_mif_oam_set_log_level_resp(transaction_id, result, error_code);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_set_log_level_req */

/* SPR-13251 Fix Starts */
/*****************************************************************************
  * Function Name  : son_mif_oam_get_log_level_req 
  * Inputs         : p_api- Pointer to the api received
  *                : p_smif_gb_context - pointer to the SMIF global context
  *                : cspl_hdr - CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_GET_LOG_LEVEL_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_get_log_level_req
(
    son_void_t *p_api,
    son_mif_gb_context_t *p_smif_gb_context,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_get_log_level_req_t    in_message;
    S32                            length_read = SON_NULL;
    son_transaction_id_t           transaction_id = SON_NULL;
    smif_state_et current_state = SMIF_NULL;
    son_module_id_et dest_module_id;
    SON_UT_TRACE_ENTER();

    if (SON_SUCCESS !=
            son_parse_son_oam_get_log_level_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    transaction_id = son_get_transaction_id(p_api);
    current_state = SMIF_GET_FSM_STATE(p_smif_gb_context);

    /* Check if get log level request is received in invalid state */
    if (SMIF_STATE_ACTIVE != current_state)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                "Unexpected message - Get log level request is received "
                "in invalid state %s",
                SMIF_FSM_STATES_NAMES[current_state]);

        son_mif_oam_send_get_log_level_resp(transaction_id, SON_FAILURE, 
                SON_ERR_UNEXPECTED_MSG, SON_ALL_INT_MODULES,
                SON_OAM_LOG_LEVEL_ERROR);

        SON_UT_TRACE_EXIT();
        return;
    }

    /* Check if get log level request is already in process */
    if(SON_NULL != p_smif_gb_context->get_log_level_req_in_process)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                "Unexpected message - Get log level request is already "
                "in process.");

        son_mif_oam_send_get_log_level_resp(transaction_id, SON_FAILURE, 
                SON_ERR_REQ_ALREADY_IN_PROGRESS, SON_ALL_INT_MODULES,
                SON_OAM_LOG_LEVEL_ERROR);

        SON_UT_TRACE_EXIT();
        return;
    }

    /* If global request, send SMIF_GET_LOG_LEVEL_REQ to all the SON internal 
     *  modules */
    if (SON_ALL_INT_MODULES == in_message.module_id)
    {
        for (dest_module_id = SON_APPS_MODULE_ID;
                dest_module_id <= SON_MAX_INT_MODULE_ID; dest_module_id++)
        {
#ifdef SON_MRO_FEATURE
            son_mif_send_get_log_level_req_to_dest_module(
                    transaction_id, dest_module_id);
            /* Set the expected number of get log level response messages */
            p_smif_gb_context->expected_get_log_level_resp++;
#else
            if (SON_MRO_MODULE_ID != dest_module_id && )
            {
                son_mif_send_get_log_level_req_to_dest_module(
                        transaction_id, dest_module_id);
                /* Set the expected number of get log level response messages */
                p_smif_gb_context->expected_get_log_level_resp++;
            }
#endif
        }
        /* Set the get_log_level_req_in_process flag to 1 to indicate that 
           1 get log level request is in progress */
        p_smif_gb_context->get_log_level_req_in_process = 1;
        if (SON_OAM_LOG_OFF == g_mif_log_on_off)
        {
            p_smif_gb_context->get_log_level_resp.module_log_level[0].
                bitmask &= (~SON_MODULE_LOG_LEVEL_PRESENT); 
            p_smif_gb_context->get_log_level_resp.module_log_level[0].
                error_code = SON_ERR_LOGGING_DISABLED;
        }
        else
        {
            p_smif_gb_context->get_log_level_resp.module_log_level[0].
                bitmask |= SON_MODULE_LOG_LEVEL_PRESENT;
            p_smif_gb_context->get_log_level_resp.module_log_level[0].
                error_code = SON_NO_ERROR;
            p_smif_gb_context->get_log_level_resp.module_log_level[0].
                log_level = (son_oam_log_level_et)
                p_smif_gb_context->smif_log_level;
        }
        p_smif_gb_context->get_log_level_resp.module_log_level[0].
            module_id = SON_MIF_MODULE_ID;
        p_smif_gb_context->get_log_level_resp.num_of_modules = 1;
        p_smif_gb_context->expected_get_log_level_resp++;
    }

    /* Request for a particular module */
#ifdef SON_MRO_FEATURE
    else if ((in_message.module_id >= SON_MIN_INT_MODULE_ID) && 
                (in_message.module_id <= SON_MAX_INT_MODULE_ID))
#else
    else if ((in_message.module_id >= SON_MIN_INT_MODULE_ID) && 
                (in_message.module_id <= SON_MAX_INT_MODULE_ID) &&
                (SON_MRO_MODULE_ID != in_message.module_id))
#endif
    {
        if (SON_MIF_MODULE_ID == in_message.module_id) 
        {
            son_mif_oam_send_get_log_level_resp(transaction_id, 
                    SON_SUCCESS, SON_NO_ERROR, SON_MIF_MODULE_ID,
                    p_smif_gb_context->smif_log_level);
        }
        else
        {
            son_mif_send_get_log_level_req_to_dest_module(transaction_id,
                    in_message.module_id);
            /* Set the expected number of get log level response messages */
            p_smif_gb_context->expected_get_log_level_resp = 1;
            /* Set the get_log_level_req_in_process flag to 1 to indicate that 
               1 get log level request is in progress */
            p_smif_gb_context->get_log_level_req_in_process = 1;
        }
    }
    /* Request came for an incorrect module-id */
    else
    {
        SON_LOG( g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Parameter [Module_id] should be in range [%d <= %d <= %d] !", 
            SON_MIN_INT_MODULE_ID, in_message.module_id, SON_MAX_INT_MODULE_ID);
        son_mif_oam_send_get_log_level_resp(transaction_id, SON_FAILURE, 
                SON_ERR_INVALID_PARAMS, SON_ALL_INT_MODULES,
                SON_OAM_LOG_LEVEL_ERROR);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_get_log_level_req */
/* SPR-13251 Fix Ends */

/* eICIC feature related code starts */
/*****************************************************************************
  * Function Name  : son_mif_oam_anr_neighbor_default_config_req
  * Inputs         : p_api- Pointer to the api received
  *                : cspl_hdr - CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles 
  *                  SON_OAM_ANR_NEIGHBOR_DEFAULT_CONFIG_REQ.
  ****************************************************************************/
static son_void_t 
son_mif_oam_anr_neighbor_default_config_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_anr_neighbor_default_config_req_t in_message = {0};
    son_anr_neighbor_default_config_req_t out_message = {0};
    S32 length_read = SON_NULL;
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&in_message, 0,sizeof(son_oam_anr_neighbor_default_config_req_t));

    SON_MEMSET(&out_message, 0, sizeof(son_anr_neighbor_default_config_req_t));

    if (SON_SUCCESS != son_parse_son_oam_anr_neighbor_default_config_req(
                            &in_message,
                            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                            cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                            &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }
    out_message.transaction_id = son_get_transaction_id(p_api);
    
    SON_MEMCPY(&(out_message.son_anr_default_req),
               &in_message,
               sizeof(son_oam_anr_neighbor_default_config_req_t));
    
    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
                              SONANR_NEIGHBOR_DEFAULT_CONFIG_REQ,
                              sizeof(out_message),
                              &out_message);
    
    SON_UT_TRACE_EXIT();
}

/* eICIC feature related code ends */
/*SPR_19279_START*/
#define SON_API_HEADER_CELL_INDEX_OFFSET 10
/*SPR_19279_END*/
/*****************************************************************************
  * Function Name  : son_mif_oam_cell_info_ind
  * Inputs         : p_api- Pointer to the api received
  *                : cspl_hdr - CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_CELL_INFO_IND msg
  ****************************************************************************/
static son_void_t
son_mif_oam_cell_info_ind
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_cell_info_ind_t in_message;

    S32 length_read = SON_NULL;
    /*SPR_19279_START*/
    U8 cell_index = *((U8 *)(p_api + CV_HDRSIZE +SON_API_HEADER_CELL_INDEX_OFFSET));
    /*SPR_19279_END*/


    SON_UT_TRACE_ENTER();

    if (SON_SUCCESS !=
            son_parse_son_oam_cell_info_ind(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    /*SPR_19279_START*/
    son_mif_send_internal_cell_info_ind_msg(SON_ANR_MODULE_ID,
            SMIF_CELL_INFO_IND,
            sizeof(in_message),
            &in_message,
            cell_index);

    son_mif_send_internal_cell_info_ind_msg(SON_ES_MODULE_ID,
            SMIF_CELL_INFO_IND,
            sizeof(in_message),
            &in_message,
            cell_index);
#ifdef SON_MRO_FEATURE
    son_mif_send_internal_cell_info_ind_msg(SON_MRO_MODULE_ID,
            SMIF_CELL_INFO_IND,
            sizeof(in_message),
            &in_message,
            cell_index);
#endif
 
    son_mif_send_internal_cell_info_ind_msg(SON_MLB_MODULE_ID,
            SMIF_CELL_INFO_IND,
            sizeof(in_message),
            &in_message,
            cell_index);

/* RACH OPT CHANGES : STARTS */
    son_mif_send_internal_cell_info_ind_msg(SON_RACH_OPT_MODULE_ID,
            SMIF_CELL_INFO_IND,
            sizeof(in_message),
            &in_message,
            cell_index);
    /*SPR_19279_END*/
/* RACH OPT CHANGES : ENDS */

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_cell_info_ind */

/*****************************************************************************
 * Function Name  : son_mif_oam_periodic_nw_scan_req
 * Inputs         : p_api- Pointer to the api received
 *                : cspl_hdr - CSPL header 
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_PERIODIC_NW_SCAN_REQ msg
 ****************************************************************************/
static son_void_t
son_mif_oam_periodic_nw_scan_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_periodic_nw_scan_req_t *p_in_message = SON_PNULL;
    sonapps_periodic_nw_scan_req_t out_message;

    S32 length_read = SON_NULL;
    son_bool_et send = SON_FALSE;


    SON_UT_TRACE_ENTER();

    p_in_message   = (son_oam_periodic_nw_scan_req_t *)
        son_mem_get(sizeof(son_oam_periodic_nw_scan_req_t));

    if(SON_PNULL == p_in_message)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Memory for the IN msg not available");
        SON_UT_TRACE_EXIT();
        return;
    }
    SON_MEMSET(p_in_message,0,sizeof(son_oam_periodic_nw_scan_req_t));

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_periodic_nw_scan_req(
                p_in_message,
                (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        son_mem_free(p_in_message);
        SON_UT_TRACE_EXIT();
        return;
    }

    if ((p_in_message->bitmask & SON_INTRA_RAT_PERIODIC_NW_SCAN_REQ_PRESENT) ||
            (p_in_message->bitmask & SON_INTER_RAT_PERIODIC_NW_SCAN_REQ_PRESENT))
    {
        out_message.transaction_id       = son_get_transaction_id(p_api);
        out_message.originator_module_id = SON_NULL;
        out_message.scan_on_off          = p_in_message->scan_on_off;
        out_message.scan_time_interval   = p_in_message->scan_time_interval;

        out_message.p_start_scan_req = SON_PNULL;
        out_message.p_start_scan_req = son_mem_get(sizeof(start_scan_req_t));

        if (SON_PNULL == out_message.p_start_scan_req)
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                    "son_mif_oam_periodic_nw_scan_req: Failed to allocate memory for "
                    "p_start_scan_req");
            son_mem_free(p_in_message);
            SON_UT_TRACE_EXIT();
            return;
        }

        SON_MEMSET(out_message.p_start_scan_req, 0, sizeof(start_scan_req_t));

        if (p_in_message->bitmask & SON_INTRA_RAT_PERIODIC_NW_SCAN_REQ_PRESENT)
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF,
                    "son_mif_oam_periodic_nw_scan_req: Scan Type Requested: INTRA-RAT Periodic Scan");
            if (SON_FAILURE == son_mif_convert_eutran_scan_req_to_nmm_scan_req
                    (&p_in_message->start_intra_rat_periodic_scan_req,
                     out_message.p_start_scan_req))
            {
                son_mem_free(out_message.p_start_scan_req);
                son_mem_free(p_in_message);
                return;
            }
        }
        else
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF,
                    "son_mif_oam_periodic_nw_scan_req: Scan Type Requested: INTER-RAT Periodic Scan");
            if (SON_FAILURE == son_mif_convert_utran_scan_req_to_nmm_scan_req
                    (&p_in_message->start_inter_rat_periodic_scan_req.start_utra_scan_req,
                     out_message.p_start_scan_req))
            {
                son_mem_free(out_message.p_start_scan_req);
                son_mem_free(p_in_message);
                return;
            }
        }

        send = SON_TRUE;
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "son_mif_oam_periodic_nw_scan_req: Bitmask NOT set for any Scan type"
                "(Inra-Rat Scan/Inter-Rat Scan) ");
    }

    if (SON_TRUE == send)
    {
        son_mif_send_internal_msg(SON_APPS_MODULE_ID,
                SONAPPS_PERIODIC_NW_SCAN_REQ,
                sizeof(out_message),
                &out_message);
    }

    if(SON_PNULL != p_in_message)
    {
        son_mem_free(p_in_message);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_periodic_nw_scan_req */

/*****************************************************************************
 * Function Name  : son_mif_oam_set_nw_scan_config_cmd 
 * Inputs         : p_api- Pointer to the api received
 *                : cspl_hdr - CSPL header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_SET_SCAN_CONFIG_CMD msg
 ****************************************************************************/
static son_void_t
son_mif_oam_set_nw_scan_config_cmd
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_set_nw_scan_config_cmd_t    in_message;
    smif_set_nw_scan_config_cmd_t       out_message;
    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_set_nw_scan_config_cmd(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id    = son_get_transaction_id(p_api);
    out_message.force_scan_on_off = in_message.force_scan_on_off;
    if (out_message.force_scan_on_off == SON_FALSE)
    {
        out_message.scan_time_interval     = in_message.scan_time_interval;
    }

    son_mif_send_internal_msg(SON_NMM_MODULE_ID,
            SMIF_SET_SCAN_CONFIG_CMD,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_set_nw_scan_config_cmd */

/*****************************************************************************
 * Function Name  : son_mif_insert_nw_scan_plmn_info
 * Inputs         : p_eutran_scan : Pointer to the eutran scan request
 *                  p_smif_gb_context : Pointer to mif global context
 *                  transaction_id 
 * Outputs        : None
 * Returns        : son_nw_scan_plmn_node_t*
 * Description    : This function inserts the requested plmns and transaction 
*                   id in the List.
 *****************************************************************************/
son_nw_scan_plmn_node_t *
son_mif_insert_nw_scan_plmn_info
(
    son_start_intra_rat_scan_req_t *p_eutran_scan,
    son_mif_gb_context_t *p_smif_gb_context,
    U16 transaction_id
)
{
    son_nw_scan_plmn_node_t *p_list_node  = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_list_node = (son_nw_scan_plmn_node_t *)son_mem_get
        (sizeof(son_nw_scan_plmn_node_t));

    if (SON_PNULL != p_list_node)
    {
        SON_MEMSET(p_list_node, 0, sizeof(son_nw_scan_plmn_node_t));
        p_list_node->nw_scan_plmn.transaction_id = transaction_id;
        p_list_node->nw_scan_plmn.num_plmn_id = p_eutran_scan->num_plmn_id;
        SON_MEMCPY(p_list_node->nw_scan_plmn.plmn_id, p_eutran_scan->plmn_id,
                    p_eutran_scan->num_plmn_id * sizeof(son_cell_plmn_info_t));

        list_insert_node(&p_smif_gb_context->nw_scan_plmn_list, 
                             &p_list_node->anchor);
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                " Failed to get memory for storing the NW SCAN PLMNs");
    }

    SON_UT_TRACE_EXIT();
    return p_list_node;
} /* son_mif_insert_nw_scan_plmn_info */

/*****************************************************************************
 * Function Name  : son_mif_oam_nw_scan_req
 * Inputs         : p_api- Pointer to the api received
 *                : p_smif_gb_context - pointer to the SMIF global context
 *                : cspl_hdr - CSPL header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_NW_SCAN_REQ msg
 ****************************************************************************/
static son_void_t
son_mif_oam_nw_scan_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr,
    son_mif_gb_context_t   *p_smif_gb_context
)
{
    son_oam_nw_scan_req_t *p_in_message = SON_PNULL;
    sonapps_nw_scan_req_t out_message;

    S32 length_read = SON_NULL;
    son_bool_et send = SON_FALSE;


    SON_UT_TRACE_ENTER();

    p_in_message = (son_oam_nw_scan_req_t *)son_mem_get(
                  sizeof(son_oam_nw_scan_req_t));
    if(SON_PNULL == p_in_message)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                "Failed to allocate memory");
        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMSET(p_in_message,0,sizeof(son_oam_nw_scan_req_t));
    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_nw_scan_req(
                p_in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        son_mem_free(p_in_message);
        SON_UT_TRACE_EXIT();
        return;
    }

    if (p_in_message->bitmask & SON_INTRA_RAT_NR_SCAN_REQ_PRESENT)
    {
        out_message.eutran_scan_req.transaction_id = son_get_transaction_id(p_api);
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
                "Scan Type Requested: INTRA-RAT Scan");

        out_message.eutran_scan_req.p_start_scan_req = SON_PNULL;
        out_message.eutran_scan_req.p_start_scan_req =
            son_mem_get(sizeof(start_scan_req_t));

        if (SON_PNULL == out_message.eutran_scan_req.p_start_scan_req)
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                    "Failed to allocate memory for p_start_scan_req");
            son_mem_free(p_in_message);
            SON_UT_TRACE_EXIT();
            return;
        }

        if (SON_FAILURE ==
                son_mif_convert_eutran_scan_req_to_nmm_scan_req(&p_in_message->start_intra_rat_scan_req,
                    out_message.eutran_scan_req.p_start_scan_req))
        {
            son_mem_free(out_message.eutran_scan_req.p_start_scan_req);
            son_mem_free(p_in_message);
            return;
        }

        if (p_in_message->start_intra_rat_scan_req.bitmask & SON_NW_SCAN_PLMN_ID_PRESENT)
        {
            if (SON_PNULL == son_mif_insert_nw_scan_plmn_info(&p_in_message->start_intra_rat_scan_req,
                                     p_smif_gb_context,
                                     out_message.eutran_scan_req.transaction_id))
            {
                son_mem_free(p_in_message);
                /* Coverity_87486 Fix Start */
                son_mem_free(out_message.eutran_scan_req.p_start_scan_req);
                /* Coverity_87486 Fix End */
                return;
            }
        }

        send = SON_TRUE;
    }
    else if (p_in_message->bitmask & SON_INTER_RAT_NR_SCAN_REQ_PRESENT)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
                "Scan Type Requested: INTER-RAT Scan");
        if (SON_UTRA_SCAN_REQ_PRESENT & p_in_message->start_inter_rat_scan_req.bitmask)
        {
            out_message.utran_scan_req.transaction_id = son_get_transaction_id(p_api);
            out_message.utran_scan_req.p_start_scan_req = SON_PNULL;
            out_message.utran_scan_req.p_start_scan_req =
                son_mem_get(sizeof(start_scan_req_t));

            if (SON_PNULL == out_message.utran_scan_req.p_start_scan_req)
            {
                SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                        "Failed to allocate memory for p_start_scan_req");
                /* SPR 19838 : Coverity 11089 fix start */
                son_mem_free(out_message.eutran_scan_req.p_start_scan_req);
                /* SPR 19838 : Coverity 11089 fix end */
                son_mem_free(p_in_message);
                SON_UT_TRACE_EXIT();
                return;
            }

            if (SON_FAILURE ==
               son_mif_convert_utran_scan_req_to_nmm_scan_req
                (&p_in_message->start_inter_rat_scan_req.start_utra_scan_req,
                 out_message.utran_scan_req.p_start_scan_req))
            {
                son_mem_free(out_message.utran_scan_req.p_start_scan_req);
                son_mem_free(p_in_message);
                return;
            }

            send = SON_TRUE;
        }
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bitmask NOT set for any Scan type (Inra-Rat Scan/Inter-Rat Scan) ");
    }

    if (SON_TRUE == send)
    {
        son_mif_send_internal_msg(SON_APPS_MODULE_ID,
                SONAPPS_NW_SCAN_REQ,
                sizeof(out_message),
                &out_message);
    }
    
    if(SON_PNULL != p_in_message)
    {
        son_mem_free(p_in_message);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_nw_scan_req */

/*****************************************************************************
  * Function Name  : son_mif_convert_eutran_scan_req_to_nmm_scan_req
  * Inputs         : p_eutran_scan - pointer to son_start_intra_rat_scan_req_t
  * Outputs        : p_nmm_scan_req - pointer to start_scan_req_t
  * Returns        : None
  * Description    : This function converts son_start_intra_rat_scan_req_t to 
  *                : start_scan_req_t
  ****************************************************************************/
static son_return_et
son_mif_convert_eutran_scan_req_to_nmm_scan_req
(
const son_start_intra_rat_scan_req_t *p_eutran_scan,
start_scan_req_t *p_nmm_scan_req
)
{
    U16 index,pci_index;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(p_nmm_scan_req, 0, sizeof(start_scan_req_t));
    p_nmm_scan_req->rat_type = SON_EUTRAN_CELL;

    p_nmm_scan_req->meas_period =
        p_eutran_scan->measurement_period;
    p_nmm_scan_req->retry_count =
        p_eutran_scan->retry_count;
    p_nmm_scan_req->freq_band_id =
        p_eutran_scan->freq_band_id;

    if ((p_eutran_scan->bitmask &
                SON_MEAS_BANDWINDTH_PRESENT) &&
            (p_eutran_scan->bitmask &
             SON_MEAS_BANDWINDTH_PER_EARFCN_PRESENT))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Both bitmasks set for Bandwidth Presence!"
                "Only one bitmask out of SON_MEAS_BANDWINDTH_PRESENT and "
                "SON_MEAS_BANDWINDTH_PER_EARFCN_PRESENT must be set");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }

    /* If single measurement bandwidth value is present for all EARFCN.
     * Number of earfcn value is set to 0 to interpret the same */
    else if (p_eutran_scan->bitmask &
            SON_MEAS_BANDWINDTH_PRESENT)
    {
        p_nmm_scan_req->is_meas_bandwidth_valid = SON_TRUE;
        p_nmm_scan_req->meas_bandwidth =
            p_eutran_scan->meas_bandwidth;
        p_nmm_scan_req->num_earfcn = 0;
    }
    /* If measurement bandwidth value is present for each EARFCN */
    else if (p_eutran_scan->bitmask &
            SON_MEAS_BANDWINDTH_PER_EARFCN_PRESENT)
    {
        p_nmm_scan_req->is_meas_bandwidth_valid = SON_FALSE;
        p_nmm_scan_req->num_earfcn = p_eutran_scan->meas_bandwidth_per_earfcn.num_earfcn;
        /* SPR 10540 starts */
        /* Copy the value of each earfcn and corresponding bandwidth to
         * out_message to be sent to ANR */
        for (index = 0; index < p_nmm_scan_req->num_earfcn;
                index++)
        {
            p_nmm_scan_req->earfcn_pci_info[index].earfcn =
                p_eutran_scan->
                meas_bandwidth_per_earfcn.meas_bandwidth_per_earfcn[index].
                earfcn;
            p_nmm_scan_req->earfcn_pci_info[index].meas_bandwidth_per_earfcn =
                p_eutran_scan->
                meas_bandwidth_per_earfcn.meas_bandwidth_per_earfcn[index].
                meas_bandwidth_for_earfcn;
            /* Start: CSR 00053250 */
            p_nmm_scan_req->earfcn_pci_info[index].num_pci =
                p_eutran_scan->meas_bandwidth_per_earfcn.meas_bandwidth_per_earfcn[index].
                num_pci;

            for (pci_index = 0; pci_index < p_nmm_scan_req->earfcn_pci_info[index].num_pci; pci_index++)
            {
                p_nmm_scan_req->earfcn_pci_info[index].pci[pci_index] =
                    p_eutran_scan->meas_bandwidth_per_earfcn.
                    meas_bandwidth_per_earfcn[index].pci[pci_index];
            }
            /*End: CSR 00053250 end */
            /* SPR 10540 ends */
        }
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bitmask NOT set for Bandwidth Presence! "
                "One bitmask out of SON_MEAS_BANDWINDTH_PRESENT and "
                "SON_MEAS_BANDWINDTH_PER_EARFCN_PRESENT must be set");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }


    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* son_mif_convert_eutran_scan_req_to_nmm_scan_req */

/*****************************************************************************
  * Function Name  : son_mif_convert_utran_scan_req_to_nmm_scan_req
  * Inputs         : p_utran_scan - pointer to son_start_utra_scan_req_t
  * Outputs        : p_nmm_scan_req -pointer to start_scan_req_t
  * Returns        : None
  * Description    : This function converts son_start_utra_scan_req_t to
  *                : start_scan_req_t
  ****************************************************************************/
static son_return_et
son_mif_convert_utran_scan_req_to_nmm_scan_req
(
const son_start_utra_scan_req_t *p_utran_scan,
start_scan_req_t *p_nmm_scan_req
)
{
    U16 index;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(p_nmm_scan_req, 0, sizeof(start_scan_req_t));
    p_nmm_scan_req->rat_type = SON_UTRAN_CELL;

    p_nmm_scan_req->meas_period =
        p_utran_scan->measurement_period;
    p_nmm_scan_req->retry_count =
        p_utran_scan->retry_count;
    p_nmm_scan_req->freq_band_id =
        p_utran_scan->freq_band_id;

    if ((p_utran_scan->bitmask &
                SON_MEAS_BANDWINDTH_PRESENT) &&
            (p_utran_scan->bitmask &
             SON_MEAS_BANDWINDTH_PER_EARFCN_PRESENT))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Both bitmasks set for Bandwidth Presence!"
                "Only one bitmask out of SON_MEAS_BANDWINDTH_PRESENT and "
                "SON_MEAS_BANDWINDTH_PER_EARFCN_PRESENT must be set");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }

    /* If single measurement bandwidth value is present for all EARFCN.
     * Number of earfcn value is set to 0 to interpret the same */
    else if (p_utran_scan->bitmask &
            SON_MEAS_BANDWINDTH_PRESENT)
    {
        p_nmm_scan_req->is_meas_bandwidth_valid = SON_TRUE;
        p_nmm_scan_req->meas_bandwidth =
            p_utran_scan->meas_bandwidth;
        p_nmm_scan_req->num_earfcn = 0;
    }
    /* If measurement bandwidth value is present for each EARFCN */
    else if (p_utran_scan->bitmask &
            SON_MEAS_BANDWINDTH_PER_EARFCN_PRESENT)
    {
        p_nmm_scan_req->is_meas_bandwidth_valid = SON_FALSE;
        p_nmm_scan_req->num_earfcn = p_utran_scan->meas_bandwidth_per_arfcn.num_earfcn;

        /*SPR 10540 Start */

        /* Copy the value of each earfcn and corresponding bandwidth to
         * out_message to be sent to ANR */
        for (index = 0; index < p_nmm_scan_req->num_earfcn;
                index++)
        {
            p_nmm_scan_req->earfcn_pci_info[index].earfcn =
                p_utran_scan->
                meas_bandwidth_per_arfcn.meas_bandwidth_per_earfcn[index].
                earfcn;
            p_nmm_scan_req->earfcn_pci_info[index].meas_bandwidth_per_earfcn =
                p_utran_scan->
                meas_bandwidth_per_arfcn.meas_bandwidth_per_earfcn[index].
                meas_bandwidth_for_earfcn;
        }
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bitmask NOT set for Bandwidth Presence! "
                "One bitmask out of SON_MEAS_BANDWINDTH_PRESENT and "
                "SON_MEAS_BANDWINDTH_PER_EARFCN_PRESENT must be set");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }
    /* Start: CSR 00053250 */
    p_nmm_scan_req->earfcn_pci_info[0].num_pci =
        p_utran_scan->num_pci;

    for (index = 0; index < p_nmm_scan_req->earfcn_pci_info[0].num_pci; index++)
    {
        p_nmm_scan_req->earfcn_pci_info[0].pci[index] =
            p_utran_scan->pci[index];
    }
    /*End: CSR 00053250 end */
    /*SPR 10540 ends */

    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* son_mif_convert_utran_scan_req_to_nmm_scan_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_nr_scan_req
  * Inputs         : p_api- Pointer to the api received
  *                  cspl_hdr- CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_NR_SCAN_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_nr_scan_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_nr_scan_req_t *in_message = SON_PNULL;
    son_anr_scan_req_t out_message;

    S32 length_read = SON_NULL;
    son_bool_et send = SON_FALSE;


    SON_UT_TRACE_ENTER();
    in_message = (son_oam_nr_scan_req_t *)
        son_mem_get(sizeof(son_oam_nr_scan_req_t));
    if(SON_PNULL ==  in_message)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
                "Failed to allocate memory");
        SON_UT_TRACE_EXIT();
        return;
    }
    SON_MEMSET(in_message, 0,sizeof(son_oam_nr_scan_req_t));

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_nr_scan_req(
                in_message,
                (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                "Bad message - ignore.");
        son_mem_free(in_message);
        SON_UT_TRACE_EXIT();
        return;
    }

    if (in_message->bitmask & SON_INTRA_RAT_NR_SCAN_REQ_PRESENT)
    {
        out_message.eutran_scan_req.transaction_id = son_get_transaction_id(p_api);
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF, 
                "Scan Type Requested: INTRA-RAT Scan");

        out_message.eutran_scan_req.p_start_scan_req = SON_PNULL;
        out_message.eutran_scan_req.p_start_scan_req = 
            son_mem_get(sizeof(start_scan_req_t));

        if (SON_PNULL == out_message.eutran_scan_req.p_start_scan_req)
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
                    "Failed to allocate memory for p_start_scan_req");
            son_mem_free(in_message);
            SON_UT_TRACE_EXIT();
            return;
        } 

        if (SON_FAILURE ==
                son_mif_convert_eutran_scan_req_to_nmm_scan_req(&in_message->start_intra_rat_scan_req,
                    out_message.eutran_scan_req.p_start_scan_req))
        {
            son_mem_free(out_message.eutran_scan_req.p_start_scan_req);
            son_mem_free(in_message);
            return;
        }

        send = SON_TRUE;
    }
    else if (in_message->bitmask & SON_INTER_RAT_NR_SCAN_REQ_PRESENT)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO, 
                "Scan Type Requested: INTER-RAT Scan");
        if (SON_UTRA_SCAN_REQ_PRESENT & in_message->start_inter_rat_scan_req.bitmask)
        {
            out_message.utran_scan_req.transaction_id = son_get_transaction_id(p_api);
            out_message.utran_scan_req.p_start_scan_req = SON_PNULL;
            out_message.utran_scan_req.p_start_scan_req =
                son_mem_get(sizeof(start_scan_req_t));

            if (SON_PNULL == out_message.utran_scan_req.p_start_scan_req)
            {
                SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                        "Failed to allocate memory for p_start_scan_req");
                son_mem_free(in_message);
                SON_UT_TRACE_EXIT();
                return;
            }

            if (SON_FAILURE ==
                    son_mif_convert_utran_scan_req_to_nmm_scan_req(&in_message->start_inter_rat_scan_req.start_utra_scan_req,
                        out_message.utran_scan_req.p_start_scan_req))
            {
                son_mem_free(out_message.utran_scan_req.p_start_scan_req);
                son_mem_free(in_message);
                return;
            }

            send = SON_TRUE;
        }
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                "Bitmask NOT set for any Scan type"
                "(Inra-Rat Scan/Inter-Rat Scan) ");
    }

    if (SON_TRUE == send)
    {
        son_mif_send_internal_msg(SON_ANR_MODULE_ID,
                SONANR_NR_SCAN_REQ,
                sizeof(out_message),
                &out_message);
    }
    if(SON_PNULL != in_message)
    {
        son_mem_free(in_message);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_nr_scan_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_init_config_req 
  * Inputs         : p_api- Pointer to the api received
  *                : p_smif_gb_context - pointer to the SMIF global context
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_INIT_CONFIG_REQ msg
  ****************************************************************************/
son_void_t
son_mif_oam_init_config_req
(
 son_void_t *p_api, 
 son_mif_gb_context_t   *p_smif_gb_context
 )
{
    son_oam_init_config_req_t in_message;
    smif_init_config_req_t out_message;
    son_oam_init_config_resp_t   son_oam_init_config_resp;
    SON_MEMSET(&son_oam_init_config_resp, 0, sizeof(son_oam_init_config_resp_t));

    son_module_id_et dest_module_id;

    son_oam_log_on_off_et   default_log_on_off              = SON_OAM_LOG_ON;
    son_oam_log_level_et    default_log_level               = SON_OAM_LOG_LEVEL_BRIEF;
    static son_module_id_et default_dest_module_id          = SON_MIN_INT_MODULE_ID;
    son_u16                 default_health_upd_timer_period = 5;

    U8 index;

    S32 length_read = SON_NULL;
    STACKAPIHDR cspl_hdr = {0};


    SON_UT_TRACE_ENTER();

    /* Get the API Id and source module ID */
    son_parse_cspl_header(p_api, &cspl_hdr);

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_init_config_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Save the health monitoring timer in MIF global context */
    if(in_message.bitmask & SON_HEALTH_UPD_TMR_PERIOD_PRESENT)
    {
        p_smif_gb_context->son_health_monitoring_timer_prd = 
            in_message.health_upd_tmr_period; 
    }
    else
    {
        p_smif_gb_context->son_health_monitoring_timer_prd = 
            default_health_upd_timer_period;
    }

    /*SPR-12944-Start*/
    if((in_message.bitmask & SON_NMM_SUPPORT_PRESENT) && (SON_FALSE == in_message.nmm_supported))
    {
        out_message.is_nmm_supported = SON_FALSE;
    }
    else
    {
        out_message.is_nmm_supported = SON_TRUE;
    }
    /*SPR-12944-End*/

    /* Send the init config request to all SON internal modules with the 
     * configuration received from OAM or default configuration if not received
     * from OAM */

    for (index = 0; index < SON_MAX_NO_INT_MODULES; index++)
    {
        out_message.transaction_id = son_get_transaction_id(p_api);

        /* If init configuration is received from OAM send that configuration to
         * internal modules */
        if (in_message.bitmask & SON_INIT_CONFIG_PRESENT)
        {
            out_message.log_enable = in_message.int_module_config[index].
                log_configuration.log_on_off;
            if (in_message.int_module_config[index].log_configuration.bitmask & SON_OAM_MODULE_LOG_LEVEL_PRESENT)
            {
                out_message.log_level = in_message.int_module_config[index].
                                log_configuration.log_level;
            }
            else
            {
                out_message.log_level = default_log_level;
            }

            dest_module_id = in_message.int_module_config[index].module_id;
        }

        /* If init configuration is not received from OAM send the default 
         * configuration to internal modules */
        else
        {
            out_message.log_enable = default_log_on_off;
            out_message.log_level = default_log_level;

            /* MRO FIX START */
#ifndef SON_MRO_FEATURE
            if (SON_MRO_MODULE_ID == default_dest_module_id)
            {
                default_dest_module_id++;
            }

#endif
        /* MRO FIX END */
            dest_module_id = default_dest_module_id;
            default_dest_module_id++;
        }

        if ((dest_module_id < SON_MIN_INT_MODULE_ID) ||
            (dest_module_id > SON_MAX_INT_MODULE_ID))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
            "Invalid module ID: %d", dest_module_id);
            continue;
        }

        /* If destination is SON MgmtIfH itself, set the configuration for 
         * self */
        if (SON_MIF_MODULE_ID == dest_module_id)
	{
        /* SPR-13251 Fix Starts */
		son_mif_log_enable_disable_cmd(out_message.log_enable,
                SON_TRUE,
				out_message.log_level,
                p_smif_gb_context);
        /* SPR-13251 Fix Ends */
		continue;
	}
        else
        {
            if (SON_ANR_MODULE_ID == dest_module_id)
            {
                son_mif_send_internal_msg(dest_module_id,
                        SMIF_INIT_CONFIG_REQ,
                        sizeof(out_message),
                        &out_message);
                son_mif_send_internal_msg(SON_X2_MODULE_ID,
                        SMIF_INIT_CONFIG_REQ,
                        sizeof(out_message),
                        &out_message);
            }
            else
            {
                son_mif_send_internal_msg(dest_module_id,
                        SMIF_INIT_CONFIG_REQ,
                        sizeof(out_message),
                        &out_message);
            }
        }
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_init_config_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_anr_enable_req
  * Inputs         : p_api- Pointer to the api received
  *                : cspl_hdr -CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_ANR_ENABLE_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_anr_enable_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_anr_enable_req_t in_message;
    son_anr_enable_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_anr_enable_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);

    SON_MEMCPY(&(out_message.enable_req), &in_message,
            sizeof(son_oam_anr_enable_req_t));
    
    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_ENABLE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_anr_enable_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_anr_disable_req
  * Inputs         : p_api- Pointer to the api received
  *                : cspl_hdr -CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_ANR_DISABLE_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_anr_disable_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_anr_disable_req_t in_message;
    son_anr_disable_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_anr_disable_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    
    SON_MEMCPY(&(out_message.disable_req), &in_message,
            sizeof(son_oam_anr_disable_req_t));
        
    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_DISABLE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_anr_disable_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_nr_update_req
  * Inputs         : p_api- Pointer to the api received
  *                : cspl_hdr - CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_NR_UPDATE_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_nr_update_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_nr_update_req_t *p_in_message = SON_PNULL;
    son_anr_nr_update_req_t *p_out_message = SON_PNULL;
    son_size_t in_msg_len = sizeof(son_oam_nr_update_req_t);
    son_size_t out_msg_len = sizeof(son_anr_nr_update_req_t);
    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    p_out_message = (son_anr_nr_update_req_t *)son_mem_get(out_msg_len);
    p_in_message = (son_oam_nr_update_req_t *)son_mem_get(in_msg_len);
    if (SON_PNULL == p_out_message || SON_PNULL == p_in_message)
    {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
                    "Memory allocation failed. "
                    "Can't send NR Update request message to ANR.");
    }
    else
    {
        SON_MEMSET(p_out_message, 0, out_msg_len);
        if (SON_SUCCESS !=
                son_parse_son_oam_nr_update_req(
                    p_in_message,
                (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                    cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                    &length_read))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                    "Bad message - ignore.");
            /* SPR 15935 Fix Start */
            if (p_in_message)
            {
                son_mem_free(p_in_message);
                p_in_message = SON_PNULL;
            }
            if (p_out_message)
            {
                son_mem_free(p_out_message);
                p_out_message = SON_PNULL;
            }
            /* SPR 15935 Fix Stop */
            SON_UT_TRACE_EXIT();
	    return;
	}

        p_out_message->transaction_id = son_get_transaction_id(p_api);

        SON_MEMCPY(&(p_out_message->upd_req), p_in_message,
                in_msg_len);

        son_mif_send_internal_msg(SON_ANR_MODULE_ID,
                SONANR_NR_UPDATE_REQ,
                out_msg_len,
                p_out_message);
    }

    if (p_in_message)
    {
        son_mem_free(p_in_message);
    }

    if (p_out_message)
    {
        son_mem_free(p_out_message);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_nr_update_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_nr_add_req
  * Inputs         : p_api- Pointer to the api received
  *                : cspl_hdr - cspl header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_NR_ADD_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_nr_add_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_nr_add_req_t *p_in_message = SON_PNULL;
    son_anr_nr_add_req_t *p_out_message = SON_PNULL;
    son_size_t in_msg_len = sizeof(son_oam_nr_add_req_t);
    son_size_t out_msg_len = sizeof(son_anr_nr_add_req_t);
    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    p_in_message = (son_oam_nr_add_req_t *)son_mem_get(in_msg_len);
    p_out_message = (son_anr_nr_add_req_t *)son_mem_get(out_msg_len);
    if (SON_PNULL == p_out_message || SON_PNULL == p_in_message)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
                "Memory allocation failed. "
                "Can't send NR add request message to ANR.");
    }
    else
    {
        SON_MEMSET(p_out_message, 0, out_msg_len);
        if (SON_SUCCESS !=
                son_parse_son_oam_nr_add_req(
                    p_in_message,
                (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                    cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                    &length_read))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                    "Bad message - ignore.");
            /* SPR 15935 Fix Start */
            if(p_in_message)
            {
                son_mem_free(p_in_message);
                p_in_message = SON_PNULL;
            }
            if(p_out_message)
            {
                son_mem_free(p_out_message);
                p_out_message = SON_PNULL;
            }
            /* SPR 15935 Fix Stop */
            SON_UT_TRACE_EXIT();
            return;
        }

        p_out_message->transaction_id = son_get_transaction_id(p_api);

        SON_MEMCPY(&(p_out_message->add_req), p_in_message,
                in_msg_len);

        son_mif_send_internal_msg(SON_ANR_MODULE_ID,
                SONANR_NR_ADD_REQ,
                out_msg_len,
                p_out_message);
    }

    if (p_in_message)
    {
        son_mem_free(p_in_message);
    }

    if (p_out_message)
    {
        son_mem_free(p_out_message);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_nr_add_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_nr_delete_req
  * Inputs         : p_api- Pointer to the api received
  *                : cspl header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_NR_DELETE_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_nr_delete_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_nr_delete_req_t *p_in_message = SON_PNULL;
    son_anr_nr_delete_req_t *p_out_message = SON_PNULL;
    son_size_t in_msg_len = sizeof(son_oam_nr_delete_req_t);
    son_size_t out_msg_len = sizeof(son_anr_nr_delete_req_t);
    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    p_out_message = (son_anr_nr_delete_req_t *)son_mem_get(out_msg_len);
    p_in_message = (son_oam_nr_delete_req_t *)son_mem_get(in_msg_len);
    if (SON_PNULL == p_out_message || SON_PNULL == p_in_message)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
                "Memory allocation failed. "
                "Can't send NR Delete request message to ANR.");
    }
    else
    {
        SON_MEMSET(p_out_message, 0, out_msg_len);
        if (SON_SUCCESS !=
                son_parse_son_oam_nr_delete_req(
                    p_in_message,
                (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                    cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                    &length_read))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                    "Bad message - ignore.");
            /* SPR 15935 Fix Start */
            if (p_out_message)
            {
                son_mem_free(p_out_message);
                p_out_message = SON_PNULL;
            }
            if (p_in_message)
            {
                son_mem_free(p_in_message);
                p_in_message = SON_PNULL;
            }
            /* SPR 15935 Fix Stop */
            SON_UT_TRACE_EXIT();
            return;
        }

        p_out_message->transaction_id = son_get_transaction_id(p_api);

        SON_MEMCPY(&(p_out_message->del_req), p_in_message,
                sizeof(son_oam_nr_delete_req_t));

        son_mif_send_internal_msg(SON_ANR_MODULE_ID,
                SONANR_NR_DELETE_REQ,
                out_msg_len,
                p_out_message);
    }

    if (p_in_message)
    {
        son_mem_free(p_in_message);
    }

    if (p_out_message)
    {
        son_mem_free(p_out_message);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_nr_delete_req */

/*****************************************************************************
 *  Function Name  : son_mif_oam_nr_delete_from_remove_list_req 
 * Inputs         : p_api- Pointer to the api received
 *                : cspl_hdr - CSPL header
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This function handles 
 *                   SON_OAM_NR_DELETE_FROM_REMOVE_LIST_REQ msg
 *****************************************************************************/
static son_void_t
son_mif_oam_nr_delete_from_remove_list_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_nr_delete_from_remove_list_req_t *p_in_message = SON_PNULL;
    son_anr_nr_delete_from_remove_list_req_t *p_out_message = SON_PNULL;
    son_size_t in_msg_len = sizeof(son_oam_nr_delete_from_remove_list_req_t);
    son_size_t out_msg_len = sizeof(son_anr_nr_delete_from_remove_list_req_t);
    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    p_in_message = (son_oam_nr_delete_from_remove_list_req_t *)
        son_mem_get(in_msg_len);
    p_out_message = (son_anr_nr_delete_from_remove_list_req_t *)
        son_mem_get(out_msg_len);
    if (SON_PNULL == p_out_message || SON_PNULL == p_in_message)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
                "Memory allocation failed. Can't send NR Delete from remove "
                "list request message to ANR.");
    }
    else
    {
        SON_MEMSET(p_out_message, 0, out_msg_len);
        if (SON_SUCCESS !=
                son_parse_son_oam_nr_delete_from_remove_list_req(
                    p_in_message,
                (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                    cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                    &length_read))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                    "Bad message - ignore.");
            /* SPR 15935 Fix Start */
            if (p_in_message)
            {
                son_mem_free(p_in_message);
                p_in_message = SON_PNULL;
            }
            if (p_out_message)
            {
                son_mem_free(p_out_message);
                p_out_message = SON_PNULL;
            }
            /* SPR 15935 Fix Stop */
            SON_UT_TRACE_EXIT();
            return;
        }

        p_out_message->transaction_id = son_get_transaction_id(p_api);

        SON_MEMCPY(&(p_out_message->delete_from_remove_list_req),
            p_in_message, in_msg_len);

        son_mif_send_internal_msg(SON_ANR_MODULE_ID,
                SONANR_NR_DELETE_FROM_REMOVE_LIST_REQ,
                out_msg_len,
                p_out_message);
    }

    if (p_in_message)
    {
        son_mem_free(p_in_message);
    }

    if (p_out_message)
    {
        son_mem_free(p_out_message);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_nr_delete_from_remove_list_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_nr_retrieve_req
  * Inputs         : p_api- Pointer to the api received
  *                : cspl_hdr - cspl header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_NR_RETRIEVE_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_nr_retrieve_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_nr_retrieve_req_t in_message;
    son_anr_nr_retrieve_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_nr_retrieve_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.rtrv_req), &in_message,
        sizeof(son_oam_nr_retrieve_req_t));

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_NR_RETRIEVE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_nr_retrieve_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_anr_meas_config_req
  * Inputs         : p_api- Pointer to the api received
  *                : cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_ANR_MEAS_CONFIG_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_anr_meas_config_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_anr_meas_config_req_t *p_in_message = SON_PNULL;
    son_anr_meas_config_req_t     *p_out_message = SON_PNULL;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    p_out_message = (son_anr_meas_config_req_t *)son_mem_get(sizeof(son_anr_meas_config_req_t));
    p_in_message = (son_oam_anr_meas_config_req_t *)son_mem_get(sizeof(son_oam_anr_meas_config_req_t));

    if (SON_PNULL == p_out_message || SON_PNULL == p_in_message)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
            "Memory allocation failed. "
            "Can't send Meas Config request message to ANR.");
    }
    else
    {
        SON_MEMSET(p_out_message, 0, sizeof(son_anr_meas_config_req_t));
        if (SON_SUCCESS != son_parse_son_oam_anr_meas_config_req(
                p_in_message,
                (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
            /* SPR 15935 Fix Start */
            if (p_in_message)
            {
                son_mem_free(p_in_message);
                p_in_message = SON_PNULL;
            }
            if (p_out_message)
            {
                son_mem_free(p_out_message);
                p_out_message = SON_PNULL;
            }
            /* SPR 15935 Fix Stop */
            SON_UT_TRACE_EXIT();
            return;
        }

        p_out_message->transaction_id = son_get_transaction_id(p_api);
        SON_MEMCPY(&(p_out_message->config_req), p_in_message,
        sizeof(son_oam_anr_meas_config_req_t));

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_MEAS_CONFIG_REQ,
            sizeof(son_anr_meas_config_req_t),
            p_out_message);
    }

    if (p_in_message)
    {
        son_mem_free(p_in_message);
    }

    if (p_out_message)
    {
        son_mem_free(p_out_message);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_anr_meas_config_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_anr_modify_ue_count_req
  * Inputs         : p_api- Pointer to the api received
  *                : cspl_hdr - CSPL Header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_ANR_MODIFY_UE_COUNT_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_anr_modify_ue_count_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_anr_modify_ue_count_req_t in_message;
    son_anr_modify_ue_count_req_t     out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_anr_modify_ue_count_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.modify_ue_cnt_req), &in_message,
        sizeof(son_oam_anr_modify_ue_count_req_t));

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_MODIFY_UE_COUNT_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_anr_modify_ue_count_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_anr_modify_meas_reporting_interval_req
  * Inputs         : p_api      Pointer to the message received
  *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles 
  *                  SON_OAM_ANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_anr_modify_meas_reporting_interval_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_anr_modify_meas_reporting_interval_req_t in_message;
    son_anr_modify_meas_reporting_interval_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_anr_modify_meas_reporting_interval_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.modify_interval_req), &in_message,
        sizeof(son_oam_anr_modify_meas_reporting_interval_req_t));

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_anr_modify_meas_reporting_interval_req */

/*****************************************************************************
 * Function Name  : son_mif_oam_anr_modify_removal_atributes_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles
 *                  SON_OAM_ANR_MODIFY_REMOVAL_ATTRIBUTES_REQ msg
 ******************************************************************************/
static son_void_t
son_mif_oam_anr_modify_removal_atributes_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_anr_modify_removal_attributes_req_t in_message;
    son_anr_modify_removal_attributes_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_anr_modify_removal_attributes_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.modify_removal_attributes_req), &in_message,
            sizeof(son_oam_anr_modify_removal_attributes_req_t));

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_MODIFY_REMOVAL_ATTRIBUTES_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_anr_modify_removal_atributes_req */

/*****************************************************************************
 * Function Name  : son_mif_oam_anr_modify_pci_confusion_cfg_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles
 *                  SON_OAM_ANR_MODIFY_PCI_CONFUSION_CFG_REQ msg
 ******************************************************************************/
static son_void_t
son_mif_oam_anr_modify_pci_confusion_cfg_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_anr_modify_pci_confusion_cfg_req_t in_message;
    son_anr_modify_pci_confusion_cfg_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_anr_modify_pci_confusion_cfg_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    /*Klockwork fix*/
    SON_MEMCPY(&(out_message.modify_pci_confusion_cfg_req), &in_message,
            sizeof(son_oam_anr_modify_pci_confusion_cfg_req_t));

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_MODIFY_PCI_CONFUSION_CFG_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_anr_modify_pci_confusion_cfg_req */

/* SPR 20653 Fix Start */
/*****************************************************************************
  * Function Name  : son_mif_oam_anr_modify_attribute_req
  * Inputs         : p_api      Pointer to the message received
  *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles 
  *                  SON_OAM_ANR_MODIFY_ATTRIBUTE_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_anr_modify_attribute_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_anr_modify_attribute_req_t in_message;
    son_anr_modify_attribute_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_anr_modify_attribute_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.modify_attribute_req), &in_message,
        sizeof(son_oam_anr_modify_attribute_req_t));

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_MODIFY_ATTRIBUTE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_anr_modify_attribute_req */
/* SPR 20653 Fix End */

/*****************************************************************************
 * Function Name  : son_mif_oam_enable_disable_x2_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles
 *                  SON_OAM_ENABLE_DISABLE_X2_REQ msg
 *****************************************************************************/
static son_void_t
son_mif_oam_enable_disable_x2_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_enable_disable_x2_req_t in_message;
    son_anr_enable_disable_x2_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_enable_disable_x2_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.enable_disable_x2_req), &in_message,
            sizeof(son_oam_enable_disable_x2_req_t));

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_ENABLE_DISABLE_X2_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_enable_disable_x2_req */

/*****************************************************************************
 * Function Name  : son_mif_oam_anr_enable_disable_limited_mode_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles
 *                  SON_OAM_ANR_ENABLE_DISABLE_LIMITED_MODE_REQ msg
 *****************************************************************************/
static son_void_t
son_mif_oam_anr_enable_disable_limited_mode_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_anr_enable_disable_limited_mode_req_t in_message;
    son_anr_enable_disable_limited_mode_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_anr_enable_disable_limited_mode_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.enable_disable_limited_mode_req), &in_message,
            sizeof(son_oam_anr_enable_disable_limited_mode_req_t));

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_ENABLE_DISABLE_LIMITED_MODE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_anr_enable_disable_limited_mode_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_nr_enb_add_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_NR_ENB_ADD_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_nr_enb_add_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_nr_enb_add_req_t in_message;
    son_anr_enb_add_req_t out_message;

    S32 length_read = SON_NULL;
    son_u8 iterator = 0;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_nr_enb_add_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);

    SON_MEMCPY(&(out_message.enb_add_req), &in_message,
            sizeof(son_oam_nr_enb_add_req_t));
       
    for (iterator = 0; 
        iterator < out_message.enb_add_req.nbr_enb_list_size; 
        iterator++)   
    {
        if (SON_HOME_ENB & 
                out_message.enb_add_req.nbr_enb_list[iterator].enb_id.enb_type)
        {
            out_message.enb_add_req.nbr_enb_list[iterator].enb_id.
                enb_id[SON_ENB_ID_OCTET_SIZE - 1] &= 0xF0;
        }
        else if (SON_MACRO_ENB & 
            out_message.enb_add_req.nbr_enb_list[iterator].enb_id.enb_type)
        {
            out_message.enb_add_req.nbr_enb_list[iterator].enb_id.
                enb_id[SON_ENB_ID_OCTET_SIZE - 2] &= 0xF0;
            out_message.enb_add_req.nbr_enb_list[iterator].enb_id.
                enb_id[SON_ENB_ID_OCTET_SIZE - 1] = 0; 
        }
    }

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_NR_ENB_ADD_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_nr_enb_add_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_nr_enb_update_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_NR_ENB_UPDATE_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_nr_enb_update_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_nr_enb_update_req_t in_message;
    son_anr_enb_update_req_t out_message;

    S32 length_read = SON_NULL;
    son_u8 iterator = 0;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_nr_enb_update_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);

    SON_MEMCPY(&(out_message.enb_update_req), &in_message,
            sizeof(son_oam_nr_enb_update_req_t));

    for (iterator = 0; 
        iterator < out_message.enb_update_req.nbr_enb_list_size; 
        iterator++)
    {
        if (SON_HOME_ENB & out_message.enb_update_req.nbr_enb_list[iterator].
                enb_id.enb_type)
        {
            out_message.enb_update_req.nbr_enb_list[iterator].enb_id.
                enb_id[SON_ENB_ID_OCTET_SIZE - 1] &= 0xF0;
        }
        else if (SON_MACRO_ENB & out_message.enb_update_req.
            nbr_enb_list[iterator].enb_id.enb_type)
        {
            out_message.enb_update_req.nbr_enb_list[iterator].enb_id.
                enb_id[SON_ENB_ID_OCTET_SIZE - 2] &= 0xF0;
            out_message.enb_update_req.nbr_enb_list[iterator].enb_id.
                enb_id[SON_ENB_ID_OCTET_SIZE - 1] = 0;
        }
    }                                                                            

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_NR_ENB_UPDATE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_nr_enb_update_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_nr_enb_delete_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_NR_ENB_DELETE_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_nr_enb_delete_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_nr_enb_delete_req_t in_message;
    son_anr_enb_delete_req_t out_message;

    S32 length_read = SON_NULL;
    son_u8 iterator = 0;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_nr_enb_delete_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);

    SON_MEMCPY(&(out_message.enb_del_req), &in_message,
            sizeof(son_oam_nr_enb_delete_req_t));
    
    for (iterator = 0; 
        iterator < out_message.enb_del_req.nbr_enb_id_list_size; 
        iterator++)
    {
        if (SON_HOME_ENB & out_message.enb_del_req.nbr_enb_id_list[iterator].
            enb_type)
        {
            out_message.enb_del_req.nbr_enb_id_list[iterator].
                enb_id[SON_ENB_ID_OCTET_SIZE - 1] &= 0xF0;
        }
        else if (SON_MACRO_ENB & out_message.enb_del_req.nbr_enb_id_list[iterator].
                enb_type)
        {
            out_message.enb_del_req.nbr_enb_id_list[iterator].
                enb_id[SON_ENB_ID_OCTET_SIZE - 2] &= 0xF0;
            out_message.enb_del_req.nbr_enb_id_list[iterator].
                enb_id[SON_ENB_ID_OCTET_SIZE - 1] = 0;
        }
    }

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_NR_ENB_DELETE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_nr_enb_delete_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_nr_enb_retrieve_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_NR_ENB_RETRIEVE_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_nr_enb_retrieve_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_nr_enb_retrieve_req_t in_message;
    son_anr_enb_retrieve_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS != son_parse_son_oam_nr_enb_retrieve_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);

    SON_MEMCPY(&(out_message.enb_retrieve_req), &in_message,
            sizeof(son_oam_nr_enb_retrieve_req_t));

    if (SON_HOME_ENB & out_message.enb_retrieve_req.nbr_enb_id.enb_type)
    {
        out_message.enb_retrieve_req.nbr_enb_id.
            enb_id[SON_ENB_ID_OCTET_SIZE - 1] &= 0xF0;
    }
    else if (SON_MACRO_ENB & out_message.enb_retrieve_req.nbr_enb_id.enb_type)
    {
        out_message.enb_retrieve_req.nbr_enb_id.
            enb_id[SON_ENB_ID_OCTET_SIZE - 2] &= 0xF0;
        out_message.enb_retrieve_req.nbr_enb_id.
            enb_id[SON_ENB_ID_OCTET_SIZE - 1] = 0;
    }

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_NR_ENB_RETRIEVE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_nr_enb_retrieve_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_anr_meas_config_obj_remove_req
  * Inputs         : pointer to the message received
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_ANR_MEAS_CONFIG_OBJ_REMOVE_REQ msg
  ****************************************************************************/
static son_void_t son_mif_oam_anr_meas_config_obj_remove_req (son_void_t *p_api,
            STACKAPIHDR cspl_hdr)
{
    son_oam_anr_meas_config_obj_remove_req_t in_message;
    son_anr_meas_config_obj_remove_req_t     out_message;

    S32 length_read = SON_NULL;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if(SON_SUCCESS !=son_parse_son_oam_anr_meas_config_obj_remove_req (
                &in_message,
                (U8*)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }
    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.remove_req), &in_message,
        sizeof(son_oam_anr_meas_config_obj_remove_req_t));

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
            SONANR_MEAS_CONFIG_OBJ_REMOVE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
}


/*****************************************************************************
  * Function Name  : son_mif_oam_es_enable_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_ES_ENABLE_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_es_enable_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_es_enable_req_t in_message;
    son_es_enable_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_es_enable_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.enable_req), &in_message,
        sizeof(son_oam_es_enable_req_t));

    son_mif_send_internal_msg(SON_ES_MODULE_ID,
            SONES_ENABLE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_es_enable_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_es_disable_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_ES_DISABLE_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_es_disable_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_es_disable_req_t in_message;
    son_es_disable_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_es_disable_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.disable_req), &in_message,
        sizeof(son_oam_es_disable_req_t));

    son_mif_send_internal_msg(SON_ES_MODULE_ID,
            SONES_DISABLE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_es_disable_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_es_modify_ue_count_interval_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles 
  *                  SON_OAM_ES_MODIFY_UE_COUNT_INTERVAL_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_es_modify_ue_count_interval_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_es_modify_ue_count_interval_req_t in_message;
    son_es_modify_ue_count_interval_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_es_modify_ue_count_interval_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.modify_interval_req), &in_message,
        sizeof(son_oam_es_modify_ue_count_interval_req_t));

    son_mif_send_internal_msg(SON_ES_MODULE_ID,
            SONES_MODIFY_UE_COUNT_INTERVAL_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_es_modify_ue_count_interval_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_es_cell_switch_off_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_ES_CELL_SWITCH_OFF_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_es_cell_switch_off_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_es_cell_switch_off_req_t in_message;
    son_es_cell_switch_off_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_es_cell_switch_off_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.switch_off_req), &in_message,
        sizeof(son_oam_es_cell_switch_off_req_t));

    son_mif_send_internal_msg(SON_ES_MODULE_ID,
            SONES_CELL_SWITCH_OFF_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_es_cell_switch_off_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_es_cell_switch_on_req 
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_ES_CELL_SWITCH_ON_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_es_cell_switch_on_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_es_cell_switch_on_req_t in_message;
    son_es_cell_switch_on_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_es_cell_switch_on_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.switch_on_req), &in_message,    
        sizeof(son_oam_es_cell_switch_on_req_t));

    son_mif_send_internal_msg(SON_ES_MODULE_ID,
            SONES_CELL_SWITCH_ON_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_es_cell_switch_on_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_es_switched_off_cell_info_req 
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_ES_SWITCHED_OFF_CELL_INFO_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_es_switched_off_cell_info_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_es_switched_off_cell_info_req_t out_message;

    /*SPR 17777 +-*/
    SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"unused variable cspl_hdr=%p",&cspl_hdr);
    /*SPR 17777 +-*/

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    
    out_message.transaction_id = son_get_transaction_id(p_api);

    son_mif_send_internal_msg(SON_ES_MODULE_ID,
            SONES_SWITCHED_OFF_CELL_INFO_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
  * Function Name  : son_mif_oam_es_modify_ue_count_threshold_req 
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_ES_MODIFY_UE_COUNT_THRESHOLD_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_es_modify_ue_count_threshold_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_es_modify_ue_count_threshold_req_t  in_message;
    son_es_modify_ue_count_threshold_req_t      out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_es_modify_ue_count_threshold_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);

    SON_MEMCPY(&(out_message.modify_threshold_req), &in_message,
        sizeof(son_oam_es_modify_ue_count_threshold_req_t));

    son_mif_send_internal_msg(SON_ES_MODULE_ID,
            SONES_MODIFY_UE_COUNT_THRESHOLD_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_es_modify_ue_count_threshold_req */

/* AUTONOMOUS SWITCH OFF START */

/*****************************************************************************
 * Function Name  : son_mif_oam_es_autonomous_switch_off_config_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_ES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ msg
 ****************************************************************************/
static son_void_t
son_mif_oam_es_autonomous_switch_off_config_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_es_autonomous_switch_off_config_req_t in_message;
    son_es_autonomous_switch_off_config_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
        son_parse_son_oam_es_autonomous_switch_off_config_req(
            &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
            cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
            &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.autonomous_switch_off_config_req), &in_message,
        sizeof(son_oam_es_autonomous_switch_off_config_req_t));

    son_mif_send_internal_msg(SON_ES_MODULE_ID,
        SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ,
        sizeof(out_message),
        &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_es_autonomous_switch_off_config_req */

/* AUTONOMOUS SWITCH OFF END */

/******************************************************************************
*   FUNCTION NAME: son_oam_send_son_oam_init_ind
*
*   INPUTS :src_module_id - source module id
*            transaction_id - transcation module id
*   DESCRIPTION:
*       This function constructs and sends SON_OAM_INIT_IND message to OAM
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_oam_send_son_oam_init_ind
(
    son_module_id_et    src_module_id,  	/* Source module identifier */
    U16                 transaction_id 	/* Interface transaction identifier */
)
{
    U16 msg_api_length = SON_NULL;
    U8 *p_msg, *p_oam_msg;


    msg_api_length = SON_API_HEADER_SIZE + EXT_MSG_API_HDR_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_oam_msg = p_msg;

    /* Fill CSPL header */
    son_construct_cspl_header(p_oam_msg, SON_VERSION_ID, src_module_id,
        SON_OAM_MODULE_ID, SON_OAM_INIT_IND, SON_NULL, msg_api_length);

    /* Fill interface header */
    p_oam_msg = p_oam_msg + SON_API_HEADER_SIZE;
    son_construct_interface_api_header(p_oam_msg, transaction_id, src_module_id,
        SON_OAM_MODULE_ID, SON_OAM_INIT_IND, SON_NULL, SON_NULL);

    SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO, 
            "Src(%u)->Dst(%u):SON_OAM_INIT_IND", src_module_id, SON_OAM_MODULE_ID);
    /* Send message to */
    son_send_msg(p_msg, SON_OAM_MODULE_ID);

    return SON_SUCCESS;
} /* son_oam_send_son_oam_init_ind */

/* REL1.1 CHANGES : STARTS */

/*****************************************************************************
  * Function Name  : son_mif_oam_pci_conflict_enable_disable_req
  * Inputs         : pointer to the message received
 *                  cspl header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_PCI_CONFLICT_ENABLE_DISABLE_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_pci_conflict_enable_disable_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_pci_conflict_enable_disable_req_t       in_message;
    sonapps_pci_conflict_enable_disable_req_t       out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_pci_conflict_enable_disable_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.pci_enable_disable_req), &in_message,
        sizeof(son_oam_pci_conflict_enable_disable_req_t));

    son_mif_send_internal_msg(SON_APPS_MODULE_ID,
            SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_pci_conflict_enable_disable_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_pci_selection_req
  * Inputs         : pointer to the message received
 *                  cspl header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_PCI_SELECTION_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_pci_selection_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_pci_selection_req_t         in_message;
    sonapps_pci_selection_req_t         out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_pci_selection_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
			"Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.pci_selection_req), &in_message,
        sizeof(son_oam_pci_selection_req_t));

    son_mif_send_internal_msg(SON_APPS_MODULE_ID,
            SONAPPS_PCI_SELECTION_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_pci_selection_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_pci_reselection_req
  * Inputs         : pointer to the message received
 *                  cspl header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_PCI_RESELECTION_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_pci_reselection_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_pci_reselection_req_t         in_message;
    sonapps_pci_reselection_req_t         out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_pci_reselection_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
        "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.pci_reselection_req), &in_message,
        sizeof(son_oam_pci_reselection_req_t));

    son_mif_send_internal_msg(SON_APPS_MODULE_ID,
            SONAPPS_PCI_RESELECTION_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_pci_reselection_req */

/*****************************************************************************
  * Function Name  : son_mif_oam_carrier_freq_selection_req
  * Inputs         : pointer to the message received
 *                  cspl header
  * Outputs        : None
  * Returns        : None
  * Description    : This function handles SON_OAM_PCI_SELECTION_REQ msg
  ****************************************************************************/
static son_void_t
son_mif_oam_carrier_freq_selection_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_carrier_freq_selection_req_t        in_message;
    sonapps_carrier_freq_selection_req_t        out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_carrier_freq_selection_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
        "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.carrier_freq_selection_req), &in_message,
        sizeof(son_oam_carrier_freq_selection_req_t));

    son_mif_send_internal_msg(SON_APPS_MODULE_ID,
            SONAPPS_CARRIER_FREQ_SELECTION_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_carrier_freq_selection_req */

/* REL1.1 CHANGES : ENDS */

/*****************************************************************************
 * Function Name  : son_mif_oam_proc_sup_req
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_PROC_SUP_REQ msg
 ****************************************************************************/
static son_void_t
son_mif_oam_proc_sup_req
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    son_oam_proc_sup_resp_t out_message;


    SON_UT_TRACE_ENTER();
    
    out_message.alive_status = SON_OK;
    
    son_oam_send_son_oam_proc_sup_resp( &out_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
}
    
#ifdef SON_MRO_FEATURE 
/* MRO CHANGES : STARTS */

/*****************************************************************************
 * Function Name  : son_mif_oam_mro_enable_req
 * Inputs         : pointer to the message received
 *                  cspl header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_MRO_ENABLE_REQ msg
 ****************************************************************************/
static son_void_t
son_mif_oam_mro_enable_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_mro_enable_req_t in_message;
    son_mro_enable_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_mro_enable_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.enable_req), &in_message,
            sizeof(son_oam_mro_enable_req_t));

    son_mif_send_internal_msg(SON_MRO_MODULE_ID,
            SONMRO_ENABLE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_mro_enable_req */

/*****************************************************************************
 * Function Name  : son_mif_oam_mro_disable_req
 * Inputs         : pointer to the message received
 *                  cspl header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_MRO_DISABLE_REQ msg
 ****************************************************************************/
static son_void_t
son_mif_oam_mro_disable_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_mro_disable_req_t in_message;
    son_mro_disable_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_mro_disable_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.disable_req), &in_message,
            sizeof(son_oam_mro_disable_req_t));

    son_mif_send_internal_msg(SON_MRO_MODULE_ID,
            SONMRO_DISABLE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_mro_disable_req */

/*****************************************************************************
 * Function Name  : son_mif_oam_mro_modify_config_param_req
 * Inputs         : pointer to the message received
 *                  cspl header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_MRO_MODIFY_CONFIG_PARAM_REQ 
 *                  msg
 ****************************************************************************/
static son_void_t
son_mif_oam_mro_modify_config_param_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_mro_modify_config_params_req_t in_message;
    son_mro_modify_config_params_req_t out_message;
    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_mro_modify_config_params_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.config_param_req), &in_message,
            sizeof(son_oam_mro_modify_config_params_req_t));

    son_mif_send_internal_msg(SON_MRO_MODULE_ID,
            SONMRO_MODIFY_CONFIG_PARAM_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_mro_modify_config_param_req */

#endif /* ifdef SON_MRO_FEATURE */

/*****************************************************************************
 * Function Name  : son_mif_oam_chk_health_req
 * Inputs         : pointer to the message received
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_CHK_HEALTH_REQ
 *                  msg from OAM
 ****************************************************************************/
static son_void_t 
son_mif_oam_chk_health_req
(
 son_void_t             *p_api,
 STACKAPIHDR            cspl_hdr,
 son_mif_gb_context_t   *p_smif_gb_context
 )
{
    son_oam_chk_health_req_t            chk_health_req;
    son_oam_chk_health_resp_t           chk_health_resp;
    S32                                 length_read    = SON_NULL;
    son_transaction_id_t                transaction_id = SON_NULL;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&chk_health_req, 0, sizeof(son_oam_chk_health_req_t));
    SON_MEMSET(&chk_health_resp, 0, sizeof(son_oam_chk_health_resp_t));

    if(SON_SUCCESS !=
            son_parse_son_oam_chk_health_req(
                &chk_health_req,
                (U8*)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    transaction_id = son_get_transaction_id(p_api);

    /* Check the status of all the threads */

    if (SON_SUCCESS == son_oam_get_thread_status
            (p_smif_gb_context->son_health_monitoring_timer_prd))
    {
         son_oam_send_son_oam_chk_health_resp(
                &chk_health_resp,
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                transaction_id,
                SON_NULL);
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Cannot send health check response to OAM "
                "as one or more threads are non-responsive");
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_nmm_last_scan_timestamp_ind
 * Inputs         : pointer to the message received
 *                  cspl header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SONNMM_LAST_SCAN_TIMESTAMP_IND msg
 ****************************************************************************/
static son_void_t
son_mif_nmm_last_scan_timestamp_ind
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_nmm_last_scan_timestamp_ind_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_nmm_last_scan_timestamp_ind_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_nmm_last_scan_timestamp_ind_t *)((U8 *)p_msg +
            SON_API_HEADER_SIZE);

    if (SON_PNULL != p_in_message)
    {
        son_oam_send_son_oam_last_scan_timestamp_ind(
                p_in_message,
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                SON_NULL,
                SON_NULL);
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Pointer to SONNMM_LAST_SCAN_TIMESTAMP_IND data is NULL - "
                "ignore");
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_nmm_last_scan_timestamp_ind */

/*****************************************************************************
  * Function Name  : son_mif_init 
 * Inputs         : pointer to init parameters
  * Outputs        : None
  * Returns        : Pointer to the SON MgmtIfH global context
  * Description    : This function initialises the SON MgmtIfH global context
  ****************************************************************************/
son_void_t *
son_mif_init
(
    son_void_t *p_init_param
)
{
#ifdef SON_IN_MEM_LOGGER
    lgr_shm_logging_config_t 	shm_cfg[SON_MAX_NUMBER_OF_THREADS];
    son_u32			*p_cbk_arg 		= SON_PNULL;
    son_u16			index 			= 0;
#endif
    son_mif_gb_context_t *p_smif_gb_context = SON_PNULL;
    SON_UT_TRACE_ENTER();
    
    /* Init advance logger with default log file */
    SON_INIT_LOG(SON_PNULL);
        
    p_smif_gb_context = son_mem_get(sizeof(son_mif_gb_context_t));

    if (SON_PNULL == p_smif_gb_context)
    {
        /*SPR 17777 +-*/
        SON_LOG(SON_OAM_LOG_ON, p_son_mif_facility_name, SON_ERROR,
        "Unable to Allocate Memory for SMIF global context!");

        LTE_GCC_UNUSED_PARAM(p_init_param);
        /*SPR 17777 +-*/
    }
    else
    {
        SON_MEMSET(p_smif_gb_context, 0, sizeof(son_mif_gb_context_t));

        /* Initialise logging parameters to default values */
        /* SPR-13251 Fix Starts */
        son_mif_log_enable_disable_cmd(SON_OAM_LOG_ON, SON_TRUE, 
                SON_OAM_LOG_LEVEL_BRIEF, p_smif_gb_context);
        /* SPR-13251 Fix Ends */
        
        son_mif_reset_context(p_smif_gb_context);
    }

#ifdef SON_IN_MEM_LOGGER
    /* Copying thread name to the structure */
    SON_STRNCPY(shm_cfg[0].thread_name, "son_main", LGR_MAX_THREAD_NAME_LEN);	
    shm_cfg[0].thread_name[LGR_MAX_THREAD_NAME_LEN - 1] = '\0';    
    SON_STRNCPY(shm_cfg[1].thread_name, "son_logger", LGR_MAX_THREAD_NAME_LEN);	
    shm_cfg[1].thread_name[LGR_MAX_THREAD_NAME_LEN - 1] = '\0';    

    /* Copying memory name to the structure */
    SON_STRNCPY(shm_cfg[0].lgr_region_name, "SON_MAIN", LGR_MAX_REGION_NAME_LEN);	
    shm_cfg[0].lgr_region_name[LGR_MAX_REGION_NAME_LEN - 1] = '\0';    
    SON_STRNCPY(shm_cfg[1].lgr_region_name, "SON_LOGGER", LGR_MAX_REGION_NAME_LEN);	
    shm_cfg[1].lgr_region_name[LGR_MAX_REGION_NAME_LEN - 1] = '\0';

    p_cbk_arg = (son_u32*)son_mem_get(sizeof(son_u32));

    if(SON_PNULL != p_cbk_arg)
    {
        *p_cbk_arg = SON_ONE;
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                "son_mif_init: Memory allocation failed. ");
    }

    for (index = 0; index < SON_MAX_NUMBER_OF_THREADS; index++)
    {
        shm_cfg[index].p_call_back = (call_back_pointer)health_monitoring;
        shm_cfg[index].p_cbk_arg = p_cbk_arg;
        shm_cfg[index].shm_size = g_config_params.son_logger_cfg.log_shm_size;
        shm_cfg[index].log_file_sz = g_config_params.son_logger_cfg.log_file_sz;
        shm_cfg[index].log_chunk_cnt = g_config_params.son_logger_cfg.log_chunk_cnt;
        shm_cfg[index].file_cnt = g_config_params.son_logger_cfg.log_file_cnt;
    }
 
    if (LGR_SUCCESS != lgr_in_mem_logging_init(SON_MAX_NUMBER_OF_THREADS,
			    shm_cfg,
			    (char*)g_config_params.son_logger_cfg.log_path))
    {
        SON_LOG(SON_OAM_LOG_ON, p_son_mif_facility_name, SON_ERROR,
                "Failed to Initialize IN-MEMORY Logging configuration");
        son_abort ();
    }
#endif
    SON_UT_TRACE_EXIT();

    return p_smif_gb_context;
} /* son_mif_init */

/*****************************************************************************
  * Function Name  : son_mif_process_msg 
  * Inputs         : pointer to the message received
  *                  pointer to the SMIF global context
  * Outputs        : None
  * Returns        : RELEASE_SON_BUFFER = 1
  * Description    : This is entry point function of the SON MgmtIfH module. 
  *                  This function receives messages from OAM agent and other
  *                  SON modules.
  ****************************************************************************/
S32
son_mif_process_msg
(
    son_void_t  *p_api,
    son_void_t  *p_gl_ctx
)
{
    son_mif_gb_context_t *p_smif_gb_context = SON_PNULL;

    
    SON_UT_TRACE_ENTER();
            
    if ((SON_PNULL == p_api) || (SON_PNULL == p_gl_ctx))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
            "Somebody call SMIF in wrong context");
        SON_UT_TRACE_EXIT();
        /* Encapsulating shell should release the buffer */
        return RELEASE_SON_BUFFER;
    }

    /* Setting thread status for main thread */
    son_oam_set_thread_status(SON_NULL);
#ifndef SON_IN_MEM_LOGGER
    son_oam_set_thread_status(SON_ONE);
#endif

    p_smif_gb_context = (son_mif_gb_context_t *)p_gl_ctx;

    /* Find appropriate handler for incoming message */
    son_mif_dispatch_message(p_api, p_smif_gb_context);

    SON_UT_TRACE_EXIT();

    return RELEASE_SON_BUFFER;
} /* son_mif_process_msg */

/*****************************************************************************
  * Function Name  : son_mif_process_timer_msg 
  * Inputs         : Timer ID of the timer
  *                  pointer to the timer data
  *                  pointer to the SMIF global context
  * Outputs        : None
  * Returns        : None
  * Description    : This function is entry point function for timer expiry
  *                  of init cnf msg from OAM
  ****************************************************************************/
son_void_t
son_mif_process_timer_msg
(
 son_timer_t timer_id,
 son_void_t        *p_timer_buf,
 son_void_t        *p_gl_ctx
 )
{
    son_timer_buf_t         *p_son_timer_buf = SON_PNULL;

    /* Timers currently supported */
    son_oam_shutdown_resp_t    shutdown_resp;


    SON_UT_TRACE_ENTER();

    /* Precondition check */
    if ((SON_PNULL == p_timer_buf) || (SON_PNULL == p_gl_ctx))
    {
        /*SPR 17777 +-*/
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
            "Called in wrong context p_timer_buf is NULL or p_gl_ctx is NULL");
        LTE_GCC_UNUSED_PARAM(timer_id);
        /*SPR 17777 +-*/
        SON_UT_TRACE_EXIT();
        return;
    }

    p_son_timer_buf = (son_timer_buf_t *)p_timer_buf;
    switch (p_son_timer_buf->timer_type)
    {
        case SON_SHUTDOWN_TIMER:
            {
                SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF,
                        " Timer 0x%x SHUTDOWN_TIMER Expired", timer_id);

                /* Send Failure message to OAM */
                shutdown_resp.result     = SON_FAILURE;
                shutdown_resp.error_code = SON_ERR_TIMER_EXPIRED;

                son_oam_send_son_oam_shutdown_resp( &shutdown_resp,
                        SON_MIF_MODULE_ID,
                        SON_OAM_MODULE_ID,
                        SON_NULL,
                        SON_NULL);

                /* Close all the sockets and free the listener structure memory
             * before terminating */
                qvClose(&son_shell, p_listener);

                SON_UT_TRACE_EXIT();
                son_exit(0);
            break;
        }

        default:
            {
                SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF,
                        " Timer 0x%x type %u expired", timer_id, p_son_timer_buf->timer_type);
                son_oam_send_son_oam_init_ind( 
                        SON_MIF_MODULE_ID,
                        SON_NULL);

            break;
        }
    } /* switch */

    SON_UT_TRACE_EXIT();
} /* son_mif_process_timer_msg */

/*****************************************************************************
  * Function Name  : son_mif_dispatch_message
  * Inputs         : pointer to the message received
  *                  pointer to the SMIF global context
  * Outputs        : None
  * Returns        : None
  * Description    : This function parses the API header and call appropriate 
  *                  action handler 
  ****************************************************************************/
static son_void_t
son_mif_dispatch_message
(
 son_void_t                  *p_api,
 son_mif_gb_context_t *p_smif_gb_context
 )
{
    STACKAPIHDR cspl_hdr = {0};


    SON_UT_TRACE_ENTER();
    
    /* Get the API Id and source module ID */
    son_parse_cspl_header(p_api, &cspl_hdr);
        
    /* Call appropriate message parser according to the source of the API
     * received and get a event id */
    switch (cspl_hdr.from)
    {
        case SON_OAM_MODULE_ID:
        {
            son_mif_oam_dispatch_message(
                    p_api,
                    cspl_hdr,
                    p_smif_gb_context);
            break;
        }

        case SON_NMM_MODULE_ID:
        {
            son_mif_sonnmm_dispatch_message(
                    p_api,
                    cspl_hdr,
                    p_smif_gb_context);
            break;
        }

        case SON_APPS_MODULE_ID:
        {
            son_mif_sonapps_dispatch_message(
                    p_api,
                    cspl_hdr,
                    p_smif_gb_context);
            break;
        }

        case SON_ANR_MODULE_ID:
        {
            son_mif_sonanr_dispatch_message(
                    p_api,
                    cspl_hdr,
                    p_smif_gb_context);
            break;
        }

        case SON_ES_MODULE_ID:
        {
            son_mif_sones_dispatch_message(
                    p_api,
                    cspl_hdr,
                    p_smif_gb_context);
            break;
        }

        case SON_X2_MODULE_ID:
        {
            son_mif_sonx2_dispatch_message(
                    p_api,
                    cspl_hdr,
                    p_smif_gb_context);
            break;
        }

#ifdef SON_MRO_FEATURE
        case SON_MRO_MODULE_ID:
        {
            son_mif_sonmro_dispatch_message(
                    p_api,
                    cspl_hdr,
                    p_smif_gb_context);
            break;
        }

#endif            
        case SON_MLB_MODULE_ID:
        {
            son_mif_sonmlb_dispatch_message(
                    p_api,
                    cspl_hdr,
                    p_smif_gb_context);
            break;
        }

/* RACH OPT CHANGES : STARTS */
        case SON_RACH_OPT_MODULE_ID:
        {
            son_mif_sonrach_opt_dispatch_message(
                    p_api,
                    cspl_hdr,
                    p_smif_gb_context);
            break;
        }
/* RACH OPT CHANGES : ENDS */

        default:
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                "API from unexpected module %u received", cspl_hdr.from);
            break;
        }
    } /* switch */

    SON_UT_TRACE_EXIT();
} /* son_mif_dispatch_message */
/*spr 22474 start*/
/****************************************************************************
 **  * Function Name  : rrm_prepare_memPool_stats
 **  * Inputs         : pointer to buffer
 **  * Outputs        : prepares the mempool stats for OAM
 **  * Returns        : None
 **  * Description    : This function updates the stats of mempool
 **  ****************************************************************************/

son_void_t son_prepare_memPool_stats
(
 son_MemPoolStats *csplMemPoolStats_p
)
{
    UInt32 allocBufMemPool = 0, nbufMemPool = 0, peakBufMemPool = 0;
    UInt32 poolIndex = 0, bufSize = 0, validPool = 0;

    /* Get the mempool stats */
    for( poolIndex = 0,bufSize = MINBUF; poolIndex < NVARMEMPOOL; poolIndex++, bufSize *= 2)
    {
        allocBufMemPool = nbufMemPool = peakBufMemPool = 0;
        nbufMemPool = getMemPoolStatsInShm(poolIndex, &allocBufMemPool, &peakBufMemPool );
        /* Send only pools which has non zero total buffers */
        if (nbufMemPool)
        {
            csplMemPoolStats_p->stats[validPool].bufSize = bufSize;
            csplMemPoolStats_p->stats[validPool].numOfBuf = nbufMemPool;
            csplMemPoolStats_p->stats[validPool].numOfAllocBuf = allocBufMemPool;
            csplMemPoolStats_p->stats[validPool].numOfPeakBuf = peakBufMemPool;
            validPool++;
        }
    }
    /* Set the end marker as bufSize to 0 so that OAM is aware where the status ends in array */
    if (validPool < NVARMEMPOOL)
    {
        csplMemPoolStats_p->stats[validPool].bufSize = 0;
    }
    return;
}


 /* son_mif_oam_get_debug_info_req */
/*****************************************************************************
 * Function Name  : son_mif_oam_get_debug_info_req 
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_GET_DEBUG_INFO_REQ msg received from OAM
 ****************************************************************************/
son_void_t son_mif_oam_get_debug_info_req(
        son_void_t *p_api, 
        STACKAPIHDR cspl_hdr)
{
    son_oam_get_debug_info_req_t *p_son_oam_get_debug_info_req = SON_PNULL;
    son_oam_get_debug_info_resp_t *p_son_oam_get_debug_info_resp = SON_PNULL;
    son_MemPoolStats son_csplMemPoolStats;
    son_transaction_id_t           transaction_id = SON_NULL;
    S32 length_read = SON_NULL;
    SON_MEMSET(&son_csplMemPoolStats, SON_NULL, sizeof(son_MemPoolStats));
    p_son_oam_get_debug_info_req = (son_oam_get_debug_info_req_t *)
        son_mem_get(sizeof(son_oam_get_debug_info_req_t));
    if (p_son_oam_get_debug_info_req == SON_PNULL)
    {

        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
                "Memory allocation to p_son_oam_get_debug_info_req failed");
        son_abort();
    }
    SON_MEMSET(p_son_oam_get_debug_info_req, SON_NULL, sizeof(son_oam_get_debug_info_req_t));
    p_son_oam_get_debug_info_resp = (son_oam_get_debug_info_resp_t *)
        son_mem_get(sizeof(son_oam_get_debug_info_resp_t));
    if (p_son_oam_get_debug_info_resp == SON_PNULL)
    {

        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
                "Memory allocation to p_son_oam_get_debug_info_resp failed");
        son_abort();
    }
    SON_MEMSET(p_son_oam_get_debug_info_resp, SON_NULL, sizeof(son_oam_get_debug_info_resp_t));
    if(SON_SUCCESS !=  son_parse_son_oam_get_debug_info_req(
                p_son_oam_get_debug_info_req,
                (U8*)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
                "SON_OAM_GET_DEBUG_INFO_REQ parsing failed");
        SON_UT_TRACE_EXIT();
        return;
    }
    else
    {
        transaction_id = son_get_transaction_id(p_api);
        if(p_son_oam_get_debug_info_req->debug_type == SON_GET_MEM_POOL_STATS)
        {
            p_son_oam_get_debug_info_resp->response = SON_SUCCESS;
            p_son_oam_get_debug_info_resp->bitmask |= SON_OAM_DEBUG_GET_MEM_POOL_PRESENT;
            son_prepare_memPool_stats(&son_csplMemPoolStats);
            SON_MEMCPY(&p_son_oam_get_debug_info_resp->csplMemPoolStats,&son_csplMemPoolStats,sizeof(son_MemPoolStats));
        }
        else
        {
            p_son_oam_get_debug_info_resp->response = SON_FAILURE;
            p_son_oam_get_debug_info_resp->bitmask |= SON_OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT;
            p_son_oam_get_debug_info_resp->fail_cause = SON_WRONG_DEBUG_TYPE;
        }
        son_oam_send_son_oam_get_debug_info_resp(
                p_son_oam_get_debug_info_resp,
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                transaction_id,
                SON_NULL);
    }  
}
/*spr 22474 end*/
/* SPR_19619 start */

 /* son_mif_oam_peer_cell_activation_req */
/*****************************************************************************
 * Function Name  : son_mif_oam_peer_cell_activation_req
 * Inputs         : p_api      Pointer to the message received
 *                  cspl_hdr   CSPL header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_PEER_CELL_ACTIVATION_REQ msg received from OAM
 ****************************************************************************/
son_void_t
son_mif_oam_peer_cell_activation_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_es_peer_cell_activation_req_t 		out_message;
    son_oam_peer_cell_activation_req_t	  	in_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_peer_cell_activation_req(
                &in_message,
                (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
                "SON_OAM_PEER_CELL_ACTIVATION_REQ parsing failed");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.peer_cell_activation_req), &in_message,
            sizeof(son_oam_peer_cell_activation_req_t));

    son_mif_send_internal_msg(SON_ES_MODULE_ID,
            SONES_PEER_CELL_ACTIVATION_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_peer_cell_activation_req */

/* SPR_19619 stop */



/* RACH OPT CHANGES : STARTS */

/****************************************************************
 * SONRACH OPT Message Handlers
 ***************************************************************/
/*****************************************************************************
 * Function Name  : son_mif_oam_rach_opt_enable_req
 * Inputs         : pointer to the message received
 *                  cspl header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_RACH_OPT_ENABLE_REQ msg
 ****************************************************************************/
static son_void_t
son_mif_oam_rach_opt_enable_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_rach_opt_enable_req_t in_message;
    son_rach_opt_enable_req_t out_message;

    S32 length_read = SON_NULL;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_rach_opt_enable_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.enable_req), &in_message,
            sizeof(son_oam_rach_opt_enable_req_t));

    son_mif_send_internal_msg(SON_RACH_OPT_MODULE_ID,
            SONRACH_OPT_ENABLE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_rach_opt_enable_req */

/*****************************************************************************
 * Function Name  : son_mif_oam_rach_opt_disable_req
 * Inputs         : pointer to the message received
 *                  cspl header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_RACH_OPT_DISABLE_REQ msg
 ****************************************************************************/
static son_void_t
son_mif_oam_rach_opt_disable_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_rach_opt_disable_req_t in_message;
    son_rach_opt_disable_req_t out_message;

    S32 length_read = SON_NULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_rach_opt_disable_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.disable_req), &in_message,
            sizeof(son_oam_rach_opt_disable_req_t));

    son_mif_send_internal_msg(SON_RACH_OPT_MODULE_ID,
            SONRACH_OPT_DISABLE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_rach_opt_disable_req */

/*****************************************************************************
 * Function Name  : son_mif_oam_rach_opt_modify_rach_optmode_req
 * Inputs         : pointer to the message received
 *                  cspl header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles 
 *                  SON_OAM_RACH_OPT_MODIFY_RACH_OPTMODE_REQ message
 ****************************************************************************/
static son_void_t
son_mif_oam_rach_opt_modify_rach_optmode_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_rach_opt_modify_rach_optmode_req_t in_message;
    son_rach_opt_modify_rach_optmode_req_t out_message;
    S32 length_read = SON_NULL;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_rach_opt_modify_rach_optmode_req(
                &in_message,
                (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.modify_mode_req), &in_message,
            sizeof(son_oam_rach_opt_modify_rach_optmode_req_t));

    son_mif_send_internal_msg(SON_RACH_OPT_MODULE_ID,
            SONRACH_OPT_MODIFY_RACH_OPTMODE_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_rach_opt_modify_rach_optmode_req */

/*****************************************************************************
 * Function Name  : son_mif_oam_rach_opt_modify_config_params_req
 * Inputs         : pointer to the message received
 *                  cspl header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles 
 *                  SON_OAM_RACH_OPT_MODIFY_CONFIG_PARAMS_REQ message
 ****************************************************************************/
static son_void_t
son_mif_oam_rach_opt_modify_config_params_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_rach_opt_modify_config_params_req_t in_message;
    son_rach_opt_modify_config_params_req_t out_message;
    S32 length_read = SON_NULL;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&out_message, 0, sizeof(out_message));
    if (SON_SUCCESS !=
            son_parse_son_oam_rach_opt_modify_config_params_req(
                &in_message,
            (U8 *)p_api + SON_FULL_INTERFACE_HEADERS_SIZE,
                cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    out_message.transaction_id = son_get_transaction_id(p_api);
    SON_MEMCPY(&(out_message.modify_req), &in_message,
            sizeof(son_oam_rach_opt_modify_config_params_req_t));

    son_mif_send_internal_msg(SON_RACH_OPT_MODULE_ID,
            SONRACH_OPT_MODIFY_CONFIG_PARAMS_REQ,
            sizeof(out_message),
            &out_message);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_rach_opt_modify_config_params_req */

/* RACH OPT CHANGES : ENDS */

/* LTE_SON_KLOCWORK_WARN_9JULY_2012_START */

/*****************************************************************************
  * Function Name  : son_mif_oam_dispatch_message
  * Inputs         : pointer to the message received,
  *                  API Id,
  *                  pointer to the SMIF global context
  * Outputs        : None
  * Returns        : None
  * Description    : This function parses the API header and call appropriate 
  *                  OAM msg handler
  *****************************************************************************/
static son_void_t
son_mif_oam_dispatch_message
(
 son_void_t                  *p_api,
 STACKAPIHDR                  cspl_hdr, 
 son_mif_gb_context_t  *p_smif_gb_context
 )
{
    SON_UT_TRACE_ENTER();
    /* Bug 8036 Fix start */
    if ((SON_OAM_INIT_IND > cspl_hdr.api) || 
            (SON_OAM_MAX_API_ID <= cspl_hdr.api))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR, 
                "OAM->SON_MIF:Invalid message %u", cspl_hdr.api);
        SON_UT_TRACE_EXIT();
        return;
    }
    /* Bug 8036 Fix end */
    SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO, 
            "OAM->SON_MIF:%s", 
            SON_MIF_OAM_EVENT_NAMES[cspl_hdr.api - SON_OAM_INIT_IND]);

    switch (cspl_hdr.api)
    {
        case SON_OAM_INIT_CONFIG_REQ:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_OAM_INIT_CNF,
                    p_api);
            break;
        }

        case SON_OAM_LOG_ENABLE_DISABLE_REQ:
        {
            son_mif_oam_log_enable_disable_req(p_api, p_smif_gb_context, 
                cspl_hdr);
            break;
        }

        case SON_OAM_SET_LOG_LEVEL_REQ:
        {
            son_mif_oam_set_log_level_req(p_api, p_smif_gb_context, cspl_hdr);
            break;
        }

        case SON_OAM_CELL_INFO_IND:
        {
            son_mif_oam_cell_info_ind(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_PERIODIC_NW_SCAN_REQ:
        {
            son_mif_oam_periodic_nw_scan_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_SET_NW_SCAN_CONFIG_CMD:
        {
            son_mif_oam_set_nw_scan_config_cmd(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_NW_SCAN_REQ:
        {
            son_mif_oam_nw_scan_req(p_api, cspl_hdr, p_smif_gb_context);
            break;
        }

        case SON_OAM_NR_SCAN_REQ:
        {
            son_mif_oam_nr_scan_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ANR_ENABLE_REQ:
        {
            son_mif_oam_anr_enable_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ANR_DISABLE_REQ:
        {
            son_mif_oam_anr_disable_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_NR_UPDATE_REQ:
        {
            son_mif_oam_nr_update_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_NR_ADD_REQ:
        {
            son_mif_oam_nr_add_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_NR_DELETE_REQ:
        {
            son_mif_oam_nr_delete_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_NR_RETRIEVE_REQ:
        {
            son_mif_oam_nr_retrieve_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_NR_ENB_ADD_REQ:
        {
            son_mif_oam_nr_enb_add_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_NR_ENB_UPDATE_REQ:
        {
            son_mif_oam_nr_enb_update_req(p_api, cspl_hdr);
            break;
        }
            
        case SON_OAM_NR_ENB_DELETE_REQ:
        {
            son_mif_oam_nr_enb_delete_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_NR_ENB_RETRIEVE_REQ:
        {
            son_mif_oam_nr_enb_retrieve_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ANR_MEAS_CONFIG_REQ:
        {
            son_mif_oam_anr_meas_config_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ANR_MODIFY_UE_COUNT_REQ:
        {
            son_mif_oam_anr_modify_ue_count_req(p_api, cspl_hdr);
            break;
        }
            
        case SON_OAM_ANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ:
        {
            son_mif_oam_anr_modify_meas_reporting_interval_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_NR_DELETE_FROM_REMOVE_LIST_REQ:
        {
            son_mif_oam_nr_delete_from_remove_list_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ANR_MODIFY_REMOVAL_ATTRIBUTES_REQ:
        {
            son_mif_oam_anr_modify_removal_atributes_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ANR_MODIFY_PCI_CONFUSION_CFG_REQ:
        {
            son_mif_oam_anr_modify_pci_confusion_cfg_req(p_api, cspl_hdr);
            break;
        }
        
        /* SPR 20653 Fix Start */    
        case SON_OAM_ANR_MODIFY_ATTRIBUTE_REQ:
        {
            son_mif_oam_anr_modify_attribute_req(p_api, cspl_hdr);
            break;
        }
        /* SPR 20653 Fix End */    

        case SON_OAM_ENABLE_DISABLE_X2_REQ:
        {
            son_mif_oam_enable_disable_x2_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ANR_ENABLE_DISABLE_LIMITED_MODE_REQ:
        {
            son_mif_oam_anr_enable_disable_limited_mode_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ES_ENABLE_REQ:
        {
            son_mif_oam_es_enable_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ES_DISABLE_REQ:
        {
            son_mif_oam_es_disable_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ES_MODIFY_UE_COUNT_INTERVAL_REQ:
        {
            son_mif_oam_es_modify_ue_count_interval_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ES_CELL_SWITCH_OFF_REQ:
        {
            son_mif_oam_es_cell_switch_off_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ES_CELL_SWITCH_ON_REQ:
        {
            son_mif_oam_es_cell_switch_on_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ES_SWITCHED_OFF_CELL_INFO_REQ:
        {
            son_mif_oam_es_switched_off_cell_info_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_ES_MODIFY_UE_COUNT_THRESHOLD_REQ:
        {
            son_mif_oam_es_modify_ue_count_threshold_req(p_api, cspl_hdr);
            break;
        }

        /* AUTONOMOUS SWITCH OFF START */
        case SON_OAM_ES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ:
        {
            son_mif_oam_es_autonomous_switch_off_config_req(p_api, cspl_hdr);
            break;
        }
        /* AUTONOMOUS SWITCH OFF END */

        case SON_OAM_SHUTDOWN_REQ:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SHUTDOWN_REQ,
                    p_api);
            break;
        }
            
/* REL1.1 CHANGES : STARTS */
        case SON_OAM_PCI_CONFLICT_ENABLE_DISABLE_REQ: 
        {
            son_mif_oam_pci_conflict_enable_disable_req(p_api, cspl_hdr);
            break;
        }
            
        case SON_OAM_PCI_SELECTION_REQ: 
        {
            son_mif_oam_pci_selection_req(p_api, cspl_hdr);
            break;
        }
            
        case SON_OAM_PCI_RESELECTION_REQ: 
        {
            son_mif_oam_pci_reselection_req(p_api, cspl_hdr);
            break;
        }
            
        case SON_OAM_CARRIER_FREQ_SELECTION_REQ: 
        {
            son_mif_oam_carrier_freq_selection_req(p_api, cspl_hdr);
            break;
        }
        
        case SON_OAM_CHK_HEALTH_REQ:
        {
            son_mif_oam_chk_health_req(p_api, cspl_hdr,p_smif_gb_context);
            break;
        }
/* REL1.1 CHANGES : ENDS */
            
        case SON_OAM_PROC_SUP_REQ:
        {
            son_mif_oam_proc_sup_req();
            break;
        }

#ifdef SON_MRO_FEATURE            
/* MRO CHANGES : STARTS */
        case SON_OAM_MRO_ENABLE_REQ:
        {
            son_mif_oam_mro_enable_req(p_api, cspl_hdr);
            break;
        }
        
        case SON_OAM_MRO_DISABLE_REQ:
        {
            son_mif_oam_mro_disable_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_MRO_MODIFY_CONFIG_PARAM_REQ:
        {
            son_mif_oam_mro_modify_config_param_req(p_api, cspl_hdr);
            break;
        }
/* MRO CHANGES : ENDS */            
#endif 
/* ifdef SON_MRO_FEATURE
        * MLB CHANGES : STARTS */

        case SON_OAM_MLB_ENABLE_REQ:
        {
            son_mif_oam_mlb_enable_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_MLB_DISABLE_REQ:
        {
            son_mif_oam_mlb_disable_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_MLB_MODIFY_ATTRIBUTES_REQ:
        {
            son_mif_oam_mlb_modify_attributes_req(p_api, cspl_hdr);
            break;
        }

        /* MLB CHANGES : END
         * Rel 1.4 Start */
        case SON_OAM_CARRIER_FREQ_AND_DL_UL_POWER_SELECTION_REQ:
        {
            son_mif_oam_carrier_freq_and_dl_tx_power_req(p_api, cspl_hdr);
            break;
        }

        /* Rel 1.4 Stop
         * MLB CHANGES : END
         *SONMLB-- Centralized support start*/
        case SON_OAM_MLB_OFFSET_CHANGE_REQ:
        {
            son_mif_oam_mlb_offset_change_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_MLB_PEER_OFFSET_CHANGE_RESP:
        {
            son_mif_oam_mlb_peer_offset_change_resp(p_api, cspl_hdr);
            break;
        }
        /*SONMLB-- Centralized support end*/

        case SON_OAM_ANR_MEAS_CONFIG_OBJ_REMOVE_REQ:
        {
            son_mif_oam_anr_meas_config_obj_remove_req(p_api, cspl_hdr);
            break;
        }

/* RACH OPT CHANGES : STARTS */
        case SON_OAM_RACH_OPT_ENABLE_REQ:
        {
            son_mif_oam_rach_opt_enable_req(p_api, cspl_hdr);
            break;
        }
        
        case SON_OAM_RACH_OPT_DISABLE_REQ:
        {
            son_mif_oam_rach_opt_disable_req(p_api, cspl_hdr);
            break;
        }

        case SON_OAM_RACH_OPT_MODIFY_RACH_OPTMODE_REQ:
        {
            son_mif_oam_rach_opt_modify_rach_optmode_req(p_api, cspl_hdr);
            break;
        }
        case SON_OAM_RACH_OPT_MODIFY_CONFIG_PARAMS_REQ:
        {   
            son_mif_oam_rach_opt_modify_config_params_req(p_api, cspl_hdr);
            break;
        }
/* RACH OPT CHANGES : ENDS */           
        /* Rel 3.0 Cell delete support changes Start */
        case SON_OAM_CELL_DELETE_REQ:
        {
            son_mif_oam_cell_delete_req(p_api, cspl_hdr, p_smif_gb_context);
            break;
        }
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-10331 Fix Starts */
        case SON_OAM_ANR_GU_GROUP_ID_CONFIG_REQ:
        {
            son_mif_oam_anr_gu_group_id_config_req(p_api, cspl_hdr);
            break;
        }
        /* SPR-10331 Fix Ends */
        /* SPR-13251 Fix Starts */
        case SON_OAM_GET_LOG_LEVEL_REQ:
        {
            son_mif_oam_get_log_level_req(p_api, p_smif_gb_context, cspl_hdr);
            break;
        }
        /* SPR-13251 Fix Ends */
        /* eICIC feature related code starts */
        case SON_OAM_ANR_NEIGHBOR_DEFAULT_CONFIG_REQ:
        {
            son_mif_oam_anr_neighbor_default_config_req(p_api, cspl_hdr);
            break;
        }
        /* eICIC feature related code ends */
        /* SPR_19619 start */
        case SON_OAM_PEER_CELL_ACTIVATION_REQ:
        {
            son_mif_oam_peer_cell_activation_req(p_api, cspl_hdr);
            break;
        }
        /* SPR_19619 stop */
        /*spr 22474 start*/
        case SON_OAM_GET_DEBUG_INFO_REQ:
        {
            son_mif_oam_get_debug_info_req(p_api, cspl_hdr);
            break;
        }
        /*spr 22474 end*/
        default:
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                "unexpected API %u from OAM module "
                "received", cspl_hdr.api);
            break;
        }
    } /* switch */

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_dispatch_message */

/* LTE_SON_KLOCWORK_WARN_9JULY_2012_END */

/*****************************************************************************
  * Function Name  : son_mif_sonnmm_dispatch_message
  * Inputs         : pointer to the message received,
  *                  API Id,
  *                  pointer to the SMIF global context
  * Outputs        : None
  * Returns        : None
  * Description    : This function parses the API header and call appropriate 
  *                  SONnmm msg handler
  ****************************************************************************/
static son_void_t
son_mif_sonnmm_dispatch_message
(
 son_void_t                  *p_api,
  STACKAPIHDR                 cspl_hdr,
 son_mif_gb_context_t *p_smif_gb_context
 )
{
    smif_event_et event_id = SMIF_INVALID_EVENT;


    SON_UT_TRACE_ENTER();

    SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF, 
        "NMM->SON:Invalid message %d", cspl_hdr.api);
    
    switch (cspl_hdr.api)
    {
        case SMIF_INIT_IND:
        {
               SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO, 
                       "NMM->SON:SMIF_INIT_IND");
               event_id = SMIF_EVENT_SONNMM_INIT_IND;
               break;
        }

        case SMIF_INIT_CONFIG_RES:
        {
               SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF, 
                       "NMM->SON:SMIF_INIT_CONFIG_RES");
               event_id = SMIF_EVENT_SONNMM_INIT_CONFIG_RESP; 
               break;
        }

        case SMIF_SHUTDOWN_RES:
        {
               SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF, 
                       "NMM->SON:SMIF_SHUTDOWN_RES");
               event_id = SMIF_EVENT_SONNMM_SHUTDOWN_RES;
               break;
        }

        case SONNMM_LAST_SCAN_TIMESTAMP_IND:
        {
               SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF,
                       "NMM->SON:SONNMM_LAST_SCAN_TIMESTAMP_IND");
               son_mif_nmm_last_scan_timestamp_ind(p_api, cspl_hdr);
               break;
        }
        /* SPR-13251 Fix Starts */
        case SMIF_GET_LOG_LEVEL_RESP:
        {
            event_id = SMIF_EVENT_SONNMM_GET_LOG_LEVEL_RES;
            break;
        }
        /* SPR-13251 Fix Ends */
        default:
        {
               SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                    "Unexpected API %u from SONNMM "
                    "module received", cspl_hdr.api);
               break;
    }
    } /* switch */

    if (SMIF_INVALID_EVENT != event_id)
    {
        son_mif_fsm_process_event(p_smif_gb_context,
                event_id,
                p_api);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_sonnmm_dispatch_message */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_START */

/*****************************************************************************
  * Function Name  : son_mif_sonapps_dispatch_message
  * Inputs         : pointer to the message received,
  *                  API Id,
  *                  pointer to the SMIF global context
  * Outputs        : None
  * Returns        : None
  * Description    : This function parses the API header and call appropriate
  *                  SONapps msg handler
  ****************************************************************************/
static son_void_t
son_mif_sonapps_dispatch_message
(
 son_void_t                  *p_api,
 STACKAPIHDR                  cspl_hdr,
 son_mif_gb_context_t *p_smif_gb_context
 /* Coverity CID-55062 Fix Starts */
)
{
    smif_event_et event_id = SMIF_INVALID_EVENT;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.api >= SONAPPS_RES_BASE_API_ID  &&
            cspl_hdr.api < SONAPPS_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
                "SONAPPS->MIF:%s",
                SON_APPS_RES_EVENT_NAMES[cspl_hdr.api - SONAPPS_RES_BASE_API_ID]); 
    }
    else if (cspl_hdr.api >= SMIF_RES_BASE_API_ID &&
            cspl_hdr.api < SMIF_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
                "SONAPPS->MIF:%s",
                SON_SMIF_EVENT_NAMES[cspl_hdr.api - SMIF_RES_BASE_API_ID]);
    }

    switch (cspl_hdr.api)
    {
        case SONAPPS_NW_SCAN_RES:
            {
                son_mif_apps_nw_scan_res(p_api, cspl_hdr, p_smif_gb_context);    
                break;
            }

            /* REL1.1 CHANGES : STARTS */
        case SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_RES:
            {
                son_mif_apps_pci_conflict_enable_disable_res(p_api, cspl_hdr);    
                break;
            }

        case SONAPPS_PCI_SELECTION_RES:
            {
                son_mif_apps_pci_selection_res(p_api, cspl_hdr);    
                break;
            }

        case SONAPPS_PCI_RESELECTION_RES:
            {
                son_mif_apps_pci_reselection_res(p_api, cspl_hdr);    
                break;
            }

        case SONAPPS_PCI_CONFLICT_CONFUSION_IND:
            {
                son_mif_apps_pci_conflict_confusion_ind(p_api, cspl_hdr);    
                break;
            }

        case SONAPPS_CARRIER_FREQ_SELECTION_RES:
            {
                son_mif_apps_carrier_freq_selection_res(p_api, cspl_hdr);    
                break;
            }

            /* REL1.1 CHANGES : ENDS
             * Rel 1.4 Start */
        case SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_RESP:
            {
                son_mif_apps_carrier_freq_and_dl_tx_power_res(p_api, cspl_hdr);
                break;
            }

            /* Rel 1.4 Stop */
        case SMIF_INIT_IND:
            {
                event_id = SMIF_EVENT_SONAPPS_INIT_IND;
                break;
            }

        case SMIF_INIT_CONFIG_RES:
            {
                event_id = SMIF_EVENT_SONAPPS_INIT_CONFIG_RESP; 
                break;
            }

        case SMIF_SHUTDOWN_RES:
            {
                event_id = SMIF_EVENT_SONAPPS_SHUTDOWN_RES;
                break;
            }
            /* SPR-13251 Fix Starts */
        case SMIF_GET_LOG_LEVEL_RESP:
            {
                event_id = SMIF_EVENT_SONAPPS_GET_LOG_LEVEL_RES;
                break;
            }
            /* SPR-13251 Fix Ends */
        default:
            {
                SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                        "Unexpected API %u from SONAPPS module received", cspl_hdr.api);
                break;
            }
    } /* switch */

    if (SMIF_INVALID_EVENT != event_id)
    {
        son_mif_fsm_process_event(p_smif_gb_context,
                event_id,
                p_api);
    }

    SON_UT_TRACE_EXIT();
    /* Coverity CID-55062 Fix Ends */
} /* son_mif_sonapps_dispatch_message */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_STOP
 * LTE_SON_KLOCWORK_WARN_10JULY_2012_START */

/*****************************************************************************
  * Function Name  : son_mif_sonanr_dispatch_message
  * Inputs         : pointer to the message received,
  *                  API Id,
  *                  pointer to the SMIF global context
  * Outputs        : None
  * Returns        : None
  * Description    : This function parses the API header and call appropriate 
  *                  SONanr msg handler
  ****************************************************************************/
static son_void_t
son_mif_sonanr_dispatch_message
(
 son_void_t                  *p_api,
 STACKAPIHDR                  cspl_hdr,
 son_mif_gb_context_t *p_smif_gb_context
 )
{
    SON_UT_TRACE_ENTER();

    if (cspl_hdr.api >= SONANR_RES_BASE_API_ID &&
        cspl_hdr.api <= SONANR_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO, 
            "ANR->:MIF%s", 
            SON_ANR_RES_EVENT_NAMES[cspl_hdr.api - SONANR_RES_BASE_API_ID]);
    }
    else if (cspl_hdr.api >= SMIF_RES_BASE_API_ID &&
        cspl_hdr.api < SMIF_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
            "ANR->MIF: %s",
           SON_SMIF_EVENT_NAMES[cspl_hdr.api - SMIF_RES_BASE_API_ID]);
    }
    
    switch (cspl_hdr.api)
    {
        case SONANR_NR_SCAN_RES:
        {
            son_mif_nr_scan_res(p_api, cspl_hdr);
            break;
        }

        case SONANR_ENABLE_RES:
        {
            son_mif_anr_enable_res(p_api, cspl_hdr);
            break;
        }
            
        case SONANR_DISABLE_RES:
        {
            son_mif_anr_disable_res(p_api, cspl_hdr);
            break;
        }
            
        case SONANR_NR_UPDATE_RES:
        {
            son_mif_anr_nr_update_res(p_api, cspl_hdr);    
            break;
        }
            
        case SONANR_NR_ADD_RES:
        {
            son_mif_anr_nr_add_res(p_api, cspl_hdr);    
            break;
        }
            
        case SONANR_NR_DELETE_RES:
        {
            son_mif_anr_nr_delete_res(p_api, cspl_hdr);    
            break;
        }
        
        case SONANR_NR_RETRIEVE_RES:
        {
            son_mif_anr_nr_retrieve_res(p_api, cspl_hdr);    
            break;
        }

        case SONANR_NR_ENB_ADD_RESP:
        {
            son_mif_anr_nr_enb_add_res(p_api, cspl_hdr);    
            break;
        }
            
        case SONANR_NR_ENB_UPDATE_RESP:
        {
            son_mif_anr_nr_enb_update_res(p_api, cspl_hdr);    
            break;
        }
            
        case SONANR_NR_ENB_DELETE_RESP:
        {
            son_mif_anr_nr_enb_delete_res(p_api, cspl_hdr);    
            break;
        }
        
        case SONANR_NR_ENB_RETRIEVE_RESP:
        {
            son_mif_anr_nr_enb_retrieve_res(p_api, cspl_hdr);    
            break;
        }

        case SONANR_MEAS_CONFIG_RES:
        {
            son_mif_anr_meas_config_res(p_api, cspl_hdr);
            break;
        }
            
        case SONANR_UPDATED_NRT_INFO:
        {
            son_mif_anr_updated_nrt_info(p_api, cspl_hdr);    
            break;
        }

        case SMIF_INIT_IND:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONANR_INIT_IND,
                    p_api);
            break;
        }

        case SMIF_INIT_CONFIG_RES:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONANR_INIT_CONFIG_RESP,
                    p_api);
            break;
        }
           
        case SMIF_SHUTDOWN_RES:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONANR_SHUTDOWN_RES,
                    p_api);
            break;
        }
            
        case SONANR_MODIFY_UE_COUNT_RES:
        {
            son_mif_anr_modify_ue_count_res(p_api, cspl_hdr); 
            break;
        }
            
        case SONANR_MODIFY_MEAS_REPORTING_INTERVAL_RES:
        {
            son_mif_anr_modify_meas_reporting_interval_res(p_api, cspl_hdr);
            break; 
        }

        case SONANR_NR_DELETE_FROM_REMOVE_LIST_RES:
        {
            son_mif_anr_nr_delete_from_remove_list_res(p_api, cspl_hdr);    
            break;
        }

        case SONANR_MODIFY_REMOVAL_ATTRIBUTES_RES:
        {
            son_mif_anr_modify_removal_attributes_res(p_api, cspl_hdr);
            break; 
        }

        case SONANR_MODIFY_PCI_CONFUSION_CFG_RES:
        {
            son_mif_anr_modify_pci_confusion_cfg_res(p_api, cspl_hdr);
            break;
        }

        /* SPR 20653 Fix Start */
        case SONANR_MODIFY_ATTRIBUTE_RES:
        {
            son_mif_anr_modify_attribute_res(p_api, cspl_hdr);
            break; 
        }
        /* SPR 20653 Fix End */

        case SONANR_ENABLE_DISABLE_LIMITED_MODE_RES:
        {
            son_mif_anr_enable_disable_limited_mode_res(p_api, cspl_hdr);
            break;
        }

        case SONANR_ENABLE_DISABLE_X2_RES:
        {
            son_mif_anr_enable_disable_x2_res(p_api, cspl_hdr);
            break;
        }

        case SONANR_LIMITED_MODE_PCI_INFO:
        {
            son_mif_anr_limited_mode_pci_info(p_api, cspl_hdr);    
            break;
        }

        case SONANR_FEATURE_STATE_CHANGE_IND:
        {
            son_mif_anr_feature_state_change_ind(p_api, cspl_hdr);
            break;
        }

        case SONANR_MEAS_CONFIG_OBJ_REMOVE_RES:
        {
            son_mif_anr_meas_config_obj_remove_res(p_api,cspl_hdr);
            break;
        }
        
        /* Rel 3.0 Cell delete support changes Start */
        case SMIF_CELL_DELETE_RESP:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONANR_CELL_DELETE_RES,
                    p_api);
            break;
        }
        /* Rel 3.0 Cell delete support changes Stop */
            /* SPR-13251 Fix Starts */
        case SMIF_GET_LOG_LEVEL_RESP:
            {
                son_mif_fsm_process_event(p_smif_gb_context,
                        SMIF_EVENT_SONANR_GET_LOG_LEVEL_RES,
                        p_api);
                break;
            }
            /* SPR-13251 Fix Ends */
        /** eICIC changes start */
        case SONANR_NEIGHBOR_DEFAULT_CONFIG_RESP:
        {
            son_mif_anr_neighbor_default_config_resp(p_api,cspl_hdr);
            break;
        }
        /** eICIC changes end */
        default:
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                "Unexpected API %u from SONANR "
                "module received", cspl_hdr.api);
            break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
} /* son_mif_sonanr_dispatch_message */
        
/*****************************************************************************
  * Function Name  : son_mif_sonx2_dispatch_message
  * Inputs         : pointer to the message received,
  *                  API Id,
  *                  pointer to the SMIF global context
  * Outputs        : None
  * Returns        : None
  * Description    : This function parses the API header and call appropriate
  *                  SONX2 msg handler
  ****************************************************************************/
static son_void_t
son_mif_sonx2_dispatch_message
(
 son_void_t                  *p_api,
 STACKAPIHDR                  cspl_hdr,
 son_mif_gb_context_t *p_smif_gb_context
 )
{
    SON_UT_TRACE_ENTER();

    if (cspl_hdr.api >= SONX2_RES_BASE_API_ID &&
        cspl_hdr.api <= SONX2_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
            "SONX2->MIF:%s",
            SON_X2_RES_EVENT_NAMES[cspl_hdr.api - SONX2_RES_BASE_API_ID]);
    }
    else if (cspl_hdr.api >= SMIF_RES_BASE_API_ID &&
        cspl_hdr.api < SMIF_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
            "SONX2->MIF:%s",
           SON_SMIF_EVENT_NAMES[cspl_hdr.api - SMIF_RES_BASE_API_ID]);
    }

    switch (cspl_hdr.api)
    {
        case SONX2_LINK_UP_IND:
        {
            son_mif_x2_link_up_ind(p_api, cspl_hdr);
            break;
        }

        case SONX2_LINK_DOWN_IND:
        {
            son_mif_x2_link_down_ind(p_api, cspl_hdr);
            break;
        }

        case SMIF_INIT_IND:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONX2_INIT_IND,
                    p_api);
            break;
        }

        case SMIF_INIT_CONFIG_RES:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONX2_INIT_CONFIG_RESP,
                    p_api);
            break;
        }

        case SMIF_SHUTDOWN_RES:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONX2_SHUTDOWN_RES,
                    p_api);
            break;
        }

        /* SPR-10331 Fix Starts */
        case SONX2_GU_GROUP_ID_CONFIG_RES:
        {
            son_mif_x2_gu_group_id_config_res(p_api, cspl_hdr);
            break;
        }
        /* SPR-10331 Fix Ends */

        default:
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                    "Unexpected API %u received "
                    "from SONX2 module", cspl_hdr.api);
            break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
} /* son_mif_sonx2_dispatch_message */

/*****************************************************************************
  * Function Name  : son_mif_sones_dispatch_message
  * Inputs         : pointer to the message received,
  *                  API Id,
  *                  pointer to the SMIF global context
  * Outputs        : None
  * Returns        : None
  * Description    : This function parses the API header and call appropriate
  *                  SONes msg handler
  ****************************************************************************/
static son_void_t
son_mif_sones_dispatch_message
(
 son_void_t                  *p_api,
 STACKAPIHDR                  cspl_hdr,
 son_mif_gb_context_t *p_smif_gb_context
 )
{
    SON_UT_TRACE_ENTER();

    if (cspl_hdr.api >= SONES_RES_BASE_API_ID &&
        cspl_hdr.api <= SONES_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
            "SONES->MIF:%s",
            SON_ES_RES_EVENT_NAMES[cspl_hdr.api - SONES_RES_BASE_API_ID]);
    }
    else if (cspl_hdr.api >= SMIF_RES_BASE_API_ID &&
        cspl_hdr.api < SMIF_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
            "SONES->MIF:%s",
           SON_SMIF_EVENT_NAMES[cspl_hdr.api - SMIF_RES_BASE_API_ID]);
    }

    switch (cspl_hdr.api)
    {
        case SONES_ENABLE_RES:
        {
            son_mif_es_enable_res(p_api, cspl_hdr);
            break;
        }
            
        case SONES_DISABLE_RES:
        {
            son_mif_es_disable_res(p_api, cspl_hdr);
            break;
        }
            
        case SONES_MODIFY_UE_COUNT_INTERVAL_RES:
        {
            son_mif_es_modify_ue_count_interval_res(p_api, cspl_hdr);
            break;
        }
            
        case SONES_ACTIVE_UE_COUNT_REPORT:
        {
            son_mif_es_active_ue_count_report(p_api, cspl_hdr);
            break;
        }
            
        case SONES_MODIFY_UE_COUNT_THRESHOLD_RES:
        {
            son_mif_es_modify_ue_count_threshold_resp(p_api, cspl_hdr);
            break;
        }
            
        case SONES_THRESHOLD_HIT_IND:
        {
            son_mif_es_ue_count_threshold_hit_ind(p_api, cspl_hdr);
            break;
        }
            
        case SONES_REQUEST_TO_SWITCH_ON_CELL:
        {
            son_mif_es_request_to_switch_on_cell(p_api, cspl_hdr);
            break;
        }
            
        case SONES_SWITCHED_OFF_CELL_INFO_RES:
        {
            son_mif_es_switched_off_cell_info_resp(p_api, cspl_hdr);
            break;
        }
        
        case SONES_CELL_SWITCH_OFF_RES:
        {
            son_mif_es_cell_switch_off_res(p_api, cspl_hdr);
            break;
        }
            
        case SONES_CELL_SWITCH_ON_RES:
        {
            son_mif_es_cell_switch_on_res(p_api, cspl_hdr);
            break;
        }
            
        case SMIF_INIT_IND:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONES_INIT_IND,
                    p_api);
            break;
        }

        case SMIF_INIT_CONFIG_RES:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONES_INIT_CONFIG_RESP,
                    p_api);
            break;
        }
                        
        case SMIF_SHUTDOWN_RES:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONES_SHUTDOWN_RES,
                    p_api);
            break;
        }

        case SONES_CELL_SWITCH_ON_OFF_IND:
        {
            son_mif_es_cell_switch_on_off_ind(p_api, cspl_hdr);
            break;
        }

        case SONES_FEATURE_STATE_CHANGE_IND:
        {
            son_mif_es_feature_state_change_ind(p_api, cspl_hdr);
            break;
        }

        /* AUTONOMOUS SWITCH OFF START */
        case SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_RESP:
        {
            son_mif_es_autonomous_switch_off_config_resp(p_api, cspl_hdr);
            break;
        }
        /* AUTONOMOUS SWITCH OFF END */

        /* Rel 3.0 Cell delete support changes Start */
        case SMIF_CELL_DELETE_RESP:
        {       
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONES_CELL_DELETE_RES,
                    p_api);
            break;
        }
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        case SMIF_GET_LOG_LEVEL_RESP:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONES_GET_LOG_LEVEL_RES,
                    p_api);
            break;
        }
        /* SPR-13251 Fix Ends */
   /* SPR_19619 start */
   
      case SMIF_PEER_CELL_ACTIVATION_RESP:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONES_PEER_CELL_ACTIVATION_RES,
                    p_api);
            break;
        }

   /* SPR_19619 stop */
        default:
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING, 
                "Unexpected API %u from SONES "
                "module received", cspl_hdr.api);
            break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
} /* son_mif_sones_dispatch_message */

/* LTE_SON_KLOCWORK_WARN_10JULY_2012_END */
#ifdef SON_MRO_FEATURE

/*****************************************************************************
 * Function Name  : son_mif_sonmro_dispatch_message
 * Inputs         : Pointer to the message received,
 *                  CSPL HDR,
 *                  pointer to the SMIF global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function parses the API header and call appropriate
 *                  SONMRO msg handler
 ****************************************************************************/
static son_void_t
son_mif_sonmro_dispatch_message
(
 son_void_t                  *p_api,
 STACKAPIHDR                  cspl_hdr,
 son_mif_gb_context_t *p_smif_gb_context
 )
{
    SON_UT_TRACE_ENTER();

    if (cspl_hdr.api >= SONMRO_RES_BASE_API_ID &&
            cspl_hdr.api < SONMRO_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
                "SONMRO->MIF:%s",
                SON_MRO_RES_EVENT_NAMES[cspl_hdr.api - SONMRO_RES_BASE_API_ID]);
    }
    else if (cspl_hdr.api >= SMIF_RES_BASE_API_ID &&
            cspl_hdr.api < SMIF_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
                "SONMRO->MIF:%s",
                SON_SMIF_EVENT_NAMES[cspl_hdr.api - SMIF_RES_BASE_API_ID]);
    }

    switch (cspl_hdr.api)
    {
        case SONMRO_ENABLE_RES:
        {
            son_mif_mro_enable_res(p_api, cspl_hdr);
            break;
        }

        case SONMRO_DISABLE_RES:
        {
            son_mif_mro_disable_res(p_api, cspl_hdr);
            break;
        }

        case SONMRO_MODIFY_CONFIG_PARAM_RES:
        {
            son_mif_mro_modify_config_param_res(p_api, cspl_hdr);
            break;
        }

        case SONMRO_EUTRAN_TTT_UPDATE_IND:
        {
            son_mif_mro_ttt_update_ind(p_api, cspl_hdr);
            break;
        }

        case SONMRO_HO_COUNTER_REPORT:
        {
            son_mif_mro_ho_counter_report_handler(p_api, cspl_hdr);
            break;
        }

        case SMIF_INIT_IND:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONMRO_INIT_IND,
                    p_api);
            break;
        }

        case SMIF_INIT_CONFIG_RES:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONMRO_INIT_CONFIG_RESP,
                    p_api);
            break;
        }

        case SMIF_SHUTDOWN_RES:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONMRO_SHUTDOWN_RES,
                    p_api);
            break;
        }

        case SONMRO_FEATURE_STATE_CHANGE_IND:
        {
            son_mif_mro_feature_state_change_ind(p_api, cspl_hdr);
            break;
        }

        case SONMRO_OPTIMIZATION_NOTIFICATION:
        {
            son_mif_mro_optimization_notification(p_api, cspl_hdr);
            break;
        }

        /* Rel 3.0 Cell delete support changes Start */
        case SMIF_CELL_DELETE_RESP:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONMRO_CELL_DELETE_RES,
                    p_api);
            break;
        }
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        case SMIF_GET_LOG_LEVEL_RESP:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONMRO_GET_LOG_LEVEL_RES,
                    p_api);
            break;
        }
        /* SPR-13251 Fix Ends */
        default:
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                    "Unexpected API %u received "
                    "from SONMRO module", cspl_hdr.api);
            break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
} /* son_mif_sonmro_dispatch_message */

#endif /* ifdef SON_MRO_FEATURE */

/*****************************************************************************
 * Function Name  : son_mif_sonmlb_dispatch_message
 * Inputs         : Pointer to the message received,
 *                  CSPL HDR,
 *                  pointer to the SMIF global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function parses the API header and call appropriate
 *                  SONMLB msg handler
 ****************************************************************************/
static son_void_t
son_mif_sonmlb_dispatch_message
(
 son_void_t                  *p_api,
 STACKAPIHDR                  cspl_hdr,
 son_mif_gb_context_t *p_smif_gb_context
 )
{
    SON_UT_TRACE_ENTER();

    if (cspl_hdr.api >= SONMLB_RES_BASE_API_ID &&
            cspl_hdr.api < SONMLB_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
                "SONMLB->MIF:%s",
                SON_MLB_RES_EVENT_NAMES[cspl_hdr.api - SONMLB_RES_BASE_API_ID]);
    }
    else if (cspl_hdr.api >= SMIF_RES_BASE_API_ID &&
            cspl_hdr.api < SMIF_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
                "SONMLB->MIF:%s",
                SON_SMIF_EVENT_NAMES[cspl_hdr.api - SMIF_RES_BASE_API_ID]);
    }

    switch (cspl_hdr.api)
    {
        case SONMLB_ENABLE_RES:
        {
            son_mif_mlb_enable_res(p_api, cspl_hdr);
            break;
        }

        case SONMLB_DISABLE_RES:
        {
            son_mif_mlb_disable_res(p_api, cspl_hdr);
            break;
        }

        case SONMLB_MODIFY_ATTRIBUTES_RES:
        {
            son_mif_mlb_modify_attributes_res(p_api, cspl_hdr);
            break;
        }

        case SMIF_INIT_IND:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONMLB_INIT_IND,
                    p_api);
            break;
        }

        case SMIF_INIT_CONFIG_RES:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONMLB_INIT_CONFIG_RESP,
                    p_api);
            break;
        }

        case SMIF_SHUTDOWN_RES:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONMLB_SHUTDOWN_RES,
                    p_api);
            break;
        }

        case SONMLB_FEATURE_STATE_CHANGE_IND:
        {
            son_mif_mlb_feature_state_change_ind(p_api, cspl_hdr);
            break;
        }

        /*SONMLB-- Centralized support start*/
        case SONMLB_HO_LOAD_PRARAMS_INFO:
        {
            son_mif_mlb_ho_load_params_info(p_api, cspl_hdr);
            break;
        }

        case SONMLB_OFFSET_CHANGE_RESP:
        {
            son_mif_mlb_offset_change_resp(p_api, cspl_hdr);
            break;
        }

        case SONMLB_PEER_OFFSET_CHANGE_REQ:
        {
            son_mif_mlb_peer_offset_change_req(p_api, cspl_hdr);
            break;
        }
        /*SONMLB-- Centralized support end*/

        /* Rel 3.0 Cell delete support changes Start */
        case SMIF_CELL_DELETE_RESP:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONMLB_CELL_DELETE_RES,
                    p_api);
            break;
        }
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        case SMIF_GET_LOG_LEVEL_RESP:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONMLB_GET_LOG_LEVEL_RES,
                    p_api);
            break;
        }
        /* SPR-13251 Fix Ends */
        default:
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                    "Unexpected API %u received "
                    "from SONMLB module", cspl_hdr.api);
            break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
} /* son_mif_sonmlb_dispatch_message */

/* RACH OPT CHANGES : STARTS */
/*****************************************************************************
 * Function Name  : son_mif_rach_opt_enable_res_handler
 * Inputs         : Pointer to the message received
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_RACH_OPT_ENABLE_RESP msg
 ****************************************************************************/
static son_void_t
son_mif_rach_opt_enable_res_handler
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_rach_opt_enable_res_t *p_in_message = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_rach_opt_enable_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_rach_opt_enable_res_t *)((U8 *)p_msg + 
            SON_API_HEADER_SIZE);

    if (SON_PNULL != p_in_message)
    {
        son_oam_send_son_oam_rach_opt_enable_resp(
                &(p_in_message->enable_res),
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                p_in_message->transaction_id,
                SON_NULL);
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Pointer to SON_OAM_RACH_OPT_ENABLE_RESP data is NULL - "
                "ignore");
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_rach_opt_enable_res_handler */

/*****************************************************************************
 * Function Name  : son_mif_rach_opt_disable_res_handler
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_RACH_OPT_DISABLE_RESP msg
 ****************************************************************************/
static son_void_t
son_mif_rach_opt_disable_res_handler
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_rach_opt_disable_res_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_rach_opt_disable_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_rach_opt_disable_res_t *)((U8 *)p_msg + 
            SON_API_HEADER_SIZE);

    if (SON_PNULL != p_in_message)
    {
        son_oam_send_son_oam_rach_opt_disable_resp(
                &(p_in_message->disable_res),
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                p_in_message->transaction_id,
                SON_NULL);
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Pointer to SON_OAM_RACH_OPT_DISABLE_RESP data is NULL - "
                "ignore");
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_rach_opt_disable_res_handler */

/*****************************************************************************
 * Function Name  : son_mif_rach_opt_modify_config_params_resp_handler
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles 
 *                  SON_OAM_RACH_OPT_MODIFY_CONFIG_PARAMS_RESP message
 ****************************************************************************/
static son_void_t
son_mif_rach_opt_modify_config_params_resp_handler
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_rach_opt_modify_config_params_res_t *p_in_message = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_rach_opt_modify_config_params_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_rach_opt_modify_config_params_res_t *)((U8 *)p_msg +
            SON_API_HEADER_SIZE);

    if (SON_PNULL != p_in_message)
    {
        son_oam_send_son_oam_rach_opt_modify_config_params_resp(
                &(p_in_message->modify_res),
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                p_in_message->transaction_id,
                SON_NULL);
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Pointer to SON_OAM_RACH_OPT_MODIFY_CONFIG_PARAMS_RESP data "
                "is NULL - ignore");
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_rach_opt_modify_config_params_resp_handler */

/*****************************************************************************
 * Function Name  : son_mif_rach_opt_modify_rach_optmode_res_handler
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles 
 *                  SON_OAM_RACH_OPT_MODIFY_RACH_OPTMODE_RESP message
 ****************************************************************************/
static son_void_t
son_mif_rach_opt_modify_rach_optmode_res_handler
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_rach_opt_modify_rach_optmode_res_t *p_in_message = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_rach_opt_modify_rach_optmode_res_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_rach_opt_modify_rach_optmode_res_t *)((U8 *)p_msg +
            SON_API_HEADER_SIZE);

    if (SON_PNULL != p_in_message)
    {
        son_oam_send_son_oam_rach_opt_modify_rach_optmode_resp(
                &(p_in_message->modify_mode_res),
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                p_in_message->transaction_id,
                SON_NULL);
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Pointer to SON_OAM_RACH_OPT_MODIFY_RACH_OPTMODE_RESP data "
                "is NULL - ignore");
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_rach_opt_modify_rach_optmode_res_handler */

/*****************************************************************************
 * Function Name  : son_mif_rach_opt_parameter_update_req_handler
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles 
 *                  SON_OAM_RACH_OPT_PARAMETER_UPDATE_REQ message.
 ****************************************************************************/
static son_void_t
son_mif_rach_opt_parameter_update_req_handler
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_rach_opt_parameter_update_req_t *p_in_message = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_oam_rach_opt_parameter_update_req_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_oam_rach_opt_parameter_update_req_t *)((U8 *)p_msg +
            SON_API_HEADER_SIZE);

    son_oam_send_son_oam_rach_opt_parameter_update_req(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_rach_opt_parameter_update_req_handler */

/*****************************************************************************
 * Function Name  : son_mif_rach_opt_failure_notification_handler 
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_RACH_OPT_FAILURE_NOTIFICATION
 *                  message.
 ****************************************************************************/
static son_void_t
son_mif_rach_opt_failure_notification_handler
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_rach_opt_failure_notification_t *p_in_message = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_oam_rach_opt_failure_notification_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_oam_rach_opt_failure_notification_t *)((U8 *)p_msg + 
            SON_API_HEADER_SIZE);

    son_oam_send_son_oam_rach_opt_failure_notification(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_rach_opt_failure_notification_handler */

/*****************************************************************************
 * Function Name  : son_mif_rach_opt_ue_info_report_handler 
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_RACH_OPT_UE_INFO_REPORT *
 *                  message
 ****************************************************************************/
static son_void_t
son_mif_rach_opt_ue_info_report_handler
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_rach_opt_ue_info_report_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_oam_rach_opt_ue_info_report_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_oam_rach_opt_ue_info_report_t *)((U8 *)p_msg + 
            SON_API_HEADER_SIZE);

    son_oam_send_son_oam_rach_opt_ue_info_report(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);
    SON_UT_TRACE_EXIT();
} /* son_mif_rach_opt_ue_info_report_handler */

/*****************************************************************************
 * Function Name  : son_mif_rach_opt_l2_perf_report_handler 
 * Inputs         : p_msg- pointer to the message
 *                  cspl_hdr - CSPL Header
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles SON_OAM_RACH_OPT_L2_PERF_REPORT
 *                  message
 ****************************************************************************/
static son_void_t
son_mif_rach_opt_l2_perf_report_handler
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_oam_rach_opt_l2_perf_report_t *p_in_message = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_oam_rach_opt_l2_perf_report_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_oam_rach_opt_l2_perf_report_t *)((U8 *)p_msg + 
            SON_API_HEADER_SIZE);

    son_oam_send_son_oam_rach_opt_l2_perf_report(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);
            
    SON_UT_TRACE_EXIT();
} /* son_mif_rach_opt_l2_perf_report_handler */

/*****************************************************************************
 * Function Name  : son_mif_rach_opt_feature_state_change_ind_handler
 * Inputs         : pointer to the message received
 *                  cspl_hdr CSPL header
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function handles SONRACH_OPT_FEATURE_STATE_CHANGE_IND
 *                  message.
 *****************************************************************************/
static son_void_t
son_mif_rach_opt_feature_state_change_ind_handler
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
)
{
    son_rach_opt_feature_state_change_ind_t *p_in_message = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (cspl_hdr.paramlen !=
            sizeof(son_rach_opt_feature_state_change_ind_t))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "Wrong message size - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_in_message = (son_rach_opt_feature_state_change_ind_t *)
                   ((U8 *)p_msg + SON_API_HEADER_SIZE);

    son_oam_send_son_oam_feature_state_change_ind(
            p_in_message,
            SON_MIF_MODULE_ID,
            SON_OAM_MODULE_ID,
            SON_NULL,
            SON_NULL);

    SON_UT_TRACE_EXIT();
} /* son_mif_rach_opt_feature_state_change_ind_handler */

/*****************************************************************************
 * Function Name  : son_mif_sonrach_opt_dispatch_message
 * Inputs         : Pointer to the message received,
 *                  CSPL HDR,
 *                  pointer to the SMIF global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function parses the API header and call appropriate
 *                  SONRACH OPT msg handler
 ****************************************************************************/
static son_void_t
son_mif_sonrach_opt_dispatch_message
(
 son_void_t                  *p_api,
 STACKAPIHDR                  cspl_hdr,
 son_mif_gb_context_t *p_smif_gb_context
 )
{
    SON_UT_TRACE_ENTER();

    if (cspl_hdr.api >= SONRACH_OPT_RES_BASE_API_ID &&
            cspl_hdr.api < SONRACH_OPT_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
                "SON_RACH_OPT->MIF:%s",
                SON_RACH_OPT_RES_EVENT_NAMES[cspl_hdr.api - 
                SONRACH_OPT_RES_BASE_API_ID]);
    }
    else if (cspl_hdr.api >= SMIF_RES_BASE_API_ID &&
            cspl_hdr.api < SMIF_RES_MAX_API)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
                "SON_RACH_OPT->MIF:%s",
                SON_SMIF_EVENT_NAMES[cspl_hdr.api - SMIF_RES_BASE_API_ID]);
    }

    switch (cspl_hdr.api)
    {
        case SONRACH_OPT_ENABLE_RES:
        {
            son_mif_rach_opt_enable_res_handler(p_api, cspl_hdr);
            break;
        }

        case SONRACH_OPT_DISABLE_RES:
        {
            son_mif_rach_opt_disable_res_handler(p_api, cspl_hdr);
            break;
        }

        case SONRACH_OPT_MODIFY_CONFIG_PARAMS_RES:
        {
            son_mif_rach_opt_modify_config_params_resp_handler(p_api, cspl_hdr);
            break;
        }

        case SONRACH_OPT_MODIFY_RACH_OPTMODE_RES:
        {
            son_mif_rach_opt_modify_rach_optmode_res_handler(p_api, cspl_hdr);
            break;
        }

        case SONRACH_OPT_PARAMETER_UPDATE_REQ:
        {
            son_mif_rach_opt_parameter_update_req_handler(p_api, cspl_hdr);
            break;
        }

        case SMIF_INIT_IND:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONRACH_OPT_INIT_IND,
                    p_api);
            break;
        }

        case SMIF_INIT_CONFIG_RES:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONRACH_OPT_INIT_CONFIG_RESP,
                    p_api);
            break;
        }

        case SMIF_SHUTDOWN_RES:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONRACH_OPT_SHUTDOWN_RES,
                    p_api);
            break;
        }

        case SONRACH_OPT_FEATURE_STATE_CHANGE_IND:
        {
            son_mif_rach_opt_feature_state_change_ind_handler(p_api, cspl_hdr);
            break;
        }

        case SONRACH_OPT_FAILURE_NOTIFICATION:
        {
            son_mif_rach_opt_failure_notification_handler(p_api, cspl_hdr);
            break;
        }

        case SONRACH_OPT_UE_INFO_REPORT:
        {
            son_mif_rach_opt_ue_info_report_handler(p_api, cspl_hdr);
            break;
        }

        case SONRACH_OPT_L2_PERF_REPORT:
        {
            son_mif_rach_opt_l2_perf_report_handler(p_api, cspl_hdr);
            break;
        }
        
        /* Rel 3.0 Cell delete support changes Start */
        case SMIF_CELL_DELETE_RESP:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONRACH_OPT_CELL_DELETE_RES,
                    p_api);
            break;
        }
        /* Rel 3.0 Cell delete support changes Stop */
        
        /* SPR-13251 Fix Starts */
        case SMIF_GET_LOG_LEVEL_RESP:
        {
            son_mif_fsm_process_event(p_smif_gb_context,
                    SMIF_EVENT_SONRACH_OPT_GET_LOG_LEVEL_RES,
                    p_api);
            break;
        }
        /* SPR-13251 Fix Ends */

        default:
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                    "Unexpected API: "
                    "%u received from SON_RACH_OPT module", cspl_hdr.api);
            break;
        }
    } /* switch */

    SON_UT_TRACE_EXIT();
} /* son_mif_sonrach_opt_dispatch_message */

/* RACH OPT CHANGES : ENDS */
