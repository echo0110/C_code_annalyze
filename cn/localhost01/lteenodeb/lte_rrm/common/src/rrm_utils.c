/* vim: ts=4:et:ai:ci:sw=4:
*/
/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_common_utils.c $
 *
 *******************************************************************************
 *
 * File Description:
 *       This is the main file of the bridge module. This module provides
 *       the bridge between RRM layer and external libraries (CSPL, logger etc).
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   3 Apr,2012      ---------   Initial
 *
 ******************************************************************************/

#include <cspl.h>
#include <stacklayer.h>
#include "rrm_utils.h"
#include "rrm_mem_utils.h"
#include "rrm_event_defines.h"
#include "rrm_l2_intf.h"
#include "rrc_ext_api.h"
#include "x2ap_api.h"
#include "s1ap_api.h"
#include <pthread.h>
#include <sys/time.h>
#if  (LOGGER_TYPE == RRM_INTERNAL_LOGGER)
#include <stdarg.h>
#include "assert.h"
#endif

extern const   QMANIFEST CellRrmEntity;
extern const   QMANIFEST CellMifEntity;
extern const   QMANIFEST UeRrmEntity;

/*CA STAGE 2*/
extern rrm_ext_communication_data comm_data;
extern rrm_global_context_t *p_g_rrm_cell_ctx;
/* Used in logger for identifying RRM UTILS */
const S8* p_rrm_utils_facility_name = (const S8*)"RRM_UTILS";
/* SPR 18145 Fix Start */
const S8* p_rrm_cellm_facility_name = (const S8*)"RRM_CELLM";
const S8* p_rrm_mif_facility_name = (const S8*)"RRM_MIF";
const S8* p_rrm_uem_facility_name = (const S8*)"RRM_UEM";
const S8* p_rrm_event_facility_name = (const S8*)"RRM_EVENT";
const S8* p_rrm_g_cellm_facility_name = (const S8*)"RRM_G_CELLM";
/* SPR 18145 Fix End */
#define GET_API_NAME(apiId, srcModuleId, dstModuleId) \
    rrm_get_api_name(apiId, srcModuleId, dstModuleId)

/*SPR 22554 Fix Start*/
#define RRM_THREAD_NAME_LEN 20
/*SPR 22554 Fix End*/
const S8 *p_rrm_log_level_names[] =
{
    /* Typecasting strings to const S8 pointers to remove klocwork
       warnings. */
    (const S8 *)"RRM_NONE/RRM_CRITICAL",
    (const S8 *)"RRM_ERROR",
    (const S8 *)"RRM_WARNING",
    (const S8 *)"RRM_INFO",
    (const S8 *)"RRM_BRIEF",
    (const S8 *)"RRM_DETAILED",
    (const S8 *)"RRM_DETAILEDALL"
};

const S8 * rrm_log_facility_name = (const S8*)"eNodeB RRM";
static struct timeval g_thread_update_status[RRM_MAX_THREADS];

/* Bit Mask Map 0 -> 1, 2 -> 2, 3 -> 4, 4 -> 8 etc. */
U32 bit_masks_map[32] =
{
    RRM_BIT0_MASK,
    RRM_BIT1_MASK,
    RRM_BIT2_MASK,
    RRM_BIT3_MASK,
    RRM_BIT4_MASK,
    RRM_BIT5_MASK,
    RRM_BIT6_MASK,
    RRM_BIT7_MASK,
    RRM_BIT8_MASK,
    RRM_BIT9_MASK,
    RRM_BIT10_MASK,
    RRM_BIT11_MASK,
    RRM_BIT12_MASK,
    RRM_BIT13_MASK,
    RRM_BIT14_MASK,
    RRM_BIT15_MASK,
    RRM_BIT16_MASK,
    RRM_BIT17_MASK,
    RRM_BIT18_MASK,
    RRM_BIT19_MASK,
    RRM_BIT20_MASK,
    RRM_BIT21_MASK,
    RRM_BIT22_MASK,
    RRM_BIT23_MASK,
    RRM_BIT24_MASK,
    RRM_BIT25_MASK,
    RRM_BIT26_MASK,
    RRM_BIT27_MASK,
    RRM_BIT28_MASK,
    RRM_BIT29_MASK,
    RRM_BIT30_MASK,
    RRM_BIT31_MASK
};

#if (LOGGER_TYPE == RRM_INTERNAL_LOGGER)
/*
 *   Internal logger
 */
#define FMT_MAX_LEN 255
#define RRM_LOG_MAX_LOG_LEN 2000
#endif

/* Debugging Definitions */
#define UNKNOWN_API_NAME "UNKNOWN_API_ID"
#define INT_API_NAME ""

#define UNKNOWN_MODULE "UNKNOWN_MODULE_ID"

/* External Modules Identifiers Names */
const char* extModuleName[] =
{
    "RRM_OAM_MODULE_ID",
    "RRM_MODULE_ID",
    "RRC_MODULE_ID",
    "PDCP_MODULE_ID",
    "MAC_MODULE_ID",
    "RRM_SON_MODULE_ID"
};

/* Internal Modules Identifiers Names */
const char* intModuleName[] =
{
    "RRM_MIF_MODULE_ID",
    "RRM_CM_MODULE_ID",
    "RRM_UEM_MODULE_ID",
    "RRM_LOGGER_MODULE_ID"
};

const char* rrm_mac_req_api_name[] =
{
    "RRM_MAC_CELL_CONFIG_REQ",
    "RRM_MAC_SCHEDULER_RECONFIG_REQ",
    "RRM_MAC_UE_RECONFIG_REQ",
    "RRM_MAC_CONFIGURE_KPI_STATS_REQ",
    "RRM_MAC_GET_KPI_STATS_REQ",
    "RRM_MAC_UE_SINR_TA_REQ",
    "RRM_MAC_E_CID_MEAS_CONFIG_REQ",
    "RRM_MAC_CONFIGURE_PERF_STATS_REQ"
};

const char* rrm_mac_cnf_api_name[] =
{
    "RRM_MAC_CELL_CONFIG_CNF",
    "RRM_MAC_SCHEDULER_RECONFIG_CNF",
    "RRM_MAC_UE_RECONFIG_CNF",
    "RRM_MAC_CONFIGURE_KPI_STATS_CNF",
    "RRM_MAC_KPI_STATS_IND",
    "RRM_MAC_GET_KPI_STATS_CNF",
    "RRM_MAC_KPI_THP_STATS_IND",
    "RRM_MAC_GET_KPI_THP_STATS_CNF",
    "RRM_MAC_UE_SINR_TA_RESP",
    "MAC_RRM_E_CID_MEAS_REPORT_RESP",
    "RRM_MAC_CONFIGURE_PERF_STATS_CNF"
};

const char* rrm_mac_ind_api_name[] =
{
    "RRM_MAC_PERIODIC_REPORT_IND",
    "RRM_MAC_L1_REPORT_IND",
    "RRM_MAC_ICIC_REPORT_IND",
    "RRM_MAC_CELL_PERF_STATS_IND",
    "RRM_MAC_EICIC_REPORT_IND"
};

const char* rrm_rrc_api_name[] =
{
    "RRC_RRM_UE_RELEASE_REQ",
    "RRC_RRM_UE_RELEASE_RESP",
    "RRC_RRM_CELL_SETUP_REQ",
    "RRC_RRM_CELL_SETUP_RESP",
    "RRC_RRM_CELL_DELETE_REQ",
    "RRC_RRM_CELL_DELETE_RESP",
    "RRC_RRM_UE_ADMISSION_REQ",
    "RRC_RRM_UE_ADMISSION_RESP",
    "RRC_RRM_UE_ADMISSION_CNF",
    "RRC_RRM_ERB_SETUP_REQ",
    "RRC_RRM_ERB_SETUP_RESP",
    "RRC_RRM_ERB_SETUP_CNF",
    "RRC_RRM_ERB_RELEASE_REQ",
    "RRC_RRM_ERB_RELEASE_RESP",
    "RRC_RRM_UE_CONNECTION_RELEASE_IND",
    "RRC_RRM_MEASURMENT_RESULTS_IND",
    "RRC_RRM_CELL_RECONFIGURE_REQ",
    "RRC_RRM_CELL_RECONFIG_RESP",
    "RRC_RRM_UE_CAPABILITY_ENQUIRY_REQ",
    "RRC_RRM_UE_CAPABILITY_ENQUIRY_RESP",
    "RRC_RRM_UE_CAPABILITY_IND",
    "RRC_RRM_ERB_MODIFY_REQ",
    "RRC_RRM_ERB_MODIFY_RESP",
    "RRC_RRM_ERB_MODIFY_CNF",
    "RRC_RRM_ERB_RELEASE_CNF",
    "RRC_RRM_UE_CONTEXT_MOD_REQ",
    "RRC_RRM_UE_CONTEXT_MOD_RESP",
    "RRC_RRM_ERB_RELEASE_IND",
    "RRC_RRM_UE_CONTEXT_MOD_CNF",
    "RRC_RRM_HO_REQUIRED",
    "RRC_RRM_UE_HO_ADM_REQ",
    "RRC_RRM_UE_HO_ADM_RESP",
    "RRC_RRM_UE_HO_ADM_CNF",
    "RRC_RRM_HO_RESTRICTION_LIST_IND",
    "RRC_RRM_UE_HO_CMD_REQ",
    "RRC_RRM_UE_HO_CMD_RESP",
    "RRC_RRM_MEAS_CONFIG_REQ",
    "RRC_RRM_MEAS_CONFIG_RESP",
    "RRC_RRM_HO_FAILURE",
    "RRC_RRM_HO_CANCEL_REQ",
    "RRC_RRM_HO_CANCEL_RESP",
    "RRC_RRM_UPDATED_PWS_SI_LIST_REQ",
    "RRC_RRM_UPDATED_PWS_SI_LIST_RESP",
    "RRC_RRM_UPDATED_PWS_SI_LIST_CNF",
    "RRC_RRM_INACTIVE_UES_IND",
    "RRC_RRM_CELL_START_REQ",
    "RRC_RRM_CELL_START_RESP",
    "RRC_RRM_CELL_STOP_REQ",
    "RRC_RRM_CELL_STOP_RESP",
    "RRC_RRM_PROXIMITY_IND",
    "RRC_RRM_INTRA_ENB_HO_IND",
    "RRC_RRM_UE_RECONFIG_REQ",
    "RRC_RRM_UE_RECONFIG_RESP",
    "RRC_RRM_CSFB_PARAMETERS_REQ_CDMA2000",
    "RRC_RRM_CSFB_PARAMETERS_RESP_CDMA2000",
    "RRC_RRM_RAN_RIM_INFORMATION_REQ",
    "RRC_RRM_RAN_RIM_INFORMATION",
    "RRC_RAN_INFO_REQ_RIM",
    "RRC_RAN_INFO_RIM",
    "RRC_RAN_ACK_RIM",
    "RRC_RAN_INFO_ERROR_RIM",
    "RRC_RAN_INFO_APP_ERROR_RIM",
    "RRC_RRM_COUNTER_CHECK_REQ",
    "RRC_RRM_COUNTER_CHECK_RESP",
    "RRC_RRM_UE_INFORMATION_REQ",
    "RRC_RRM_UE_INFORMATION_RESP",
    "RRC_RRM_DL_NAS_TRANSPORT_IND",
    "RRC_RRM_INTER_FREQ_MEASUREMENT_IND",
    "RRC_RRM_UE_SYNC_STATUS",
    "RRC_RRM_LOCAL_ERROR_IND",
    "RRC_RRM_UE_REESTAB_CONFIG_REQ",
    "RRC_RRM_UE_REESTAB_CONFIG_RESP",
    "RRC_RRM_UE_REESTAB_CONFIG_CNF"
};

const char* rrm_x2ap_api_name[] =
{
    "X2AP_RRM_LINK_DOWN_IND",
    "X2AP_RRM_LINK_UP_IND",
    "X2AP_RRM_CELL_UPDATED_INDICATION",
    "X2AP_RRM_RSU_RRM_START_REQ",
    "X2AP_RRM_RSU_ENB_START_RES",
    "X2AP_RRM_RSU_RRM_STOP_REQ",
    "X2AP_RRM_RSU_ENB_STOP_RES",
    "X2AP_RRM_RSU_ENB_START_REQ",
    "X2AP_RRM_RSU_RRM_START_RES",
    "X2AP_RRM_RSU_ENB_STOP_REQ",
    "X2AP_RRM_RSU_RRM_STOP_RES",
    "X2AP_RRM_RSU_RRM_UPDATE",
    "X2AP_RRM_RSU_ENB_UPDATE_IND",
    "X2AP_RRM_LI_RRM_LOAD_INFORMATION_REQ",
    "X2AP_RRM_LI_ENB_LOAD_INFORMATION_RES",
    "X2AP_RRM_LI_ENB_LOAD_INFORMATION_IND",
    "X2AP_ENB_CONFIG_UPDATE_REQ",
    "X2AP_ENB_CONFIG_UPDATE_RES",
    "X2AP_ENB_CONFIG_UPDATE_IND",
    "X2AP_RESET_REQ",
    "X2AP_RESET_RES",
    "X2AP_RESET_IND",
    "X2AP_RLF_IND",
    "X2AP_ENB_LINK_UP_IND",
    "X2AP_ENB_LINK_DOWN_IND	",
    "X2AP_ENB_CONFIG_UPDATE_WAIT_IND",
    "X2AP_HO_REPORT_IND",
    "X2AP_PEER_CELL_ACTIVATION_REQ",
    "X2AP_PEER_CELL_ACTIVATION_RESP",
    "X2AP_CELL_ACTIVATION_REQ",
    "X2AP_CELL_ACTIVATION_RESP",
    "X2AP_MOBILITY_CHANGE_REQ",
    "X2AP_MOBILITY_CHANGE_RESP",
    "X2AP_MOBILITY_SETTING_CHANGE_REQ",
    "X2AP_MOBILITY_SETTING_CHANGE_RESP",
    "X2AP_RRM_UPDATED_MBMS_SF_INFO_IND"
#ifdef ENDC_ENABLED
    ,"X2AP_ENDC_CONFIG_UPDATE_REQ",
    "X2AP_ENDC_CONFIG_UPDATE_RES",
    "X2AP_ENDC_CONFIG_UPDATE_IND",
    "X2AP_ENDC_CONFIG_UPDATE_WAIT_IND"
#endif
};

const char* rrm_s1ap_api_name[] =
{
    "S1AP_RRM_ENB_DIRECT_INFO_TRANSFER",
    "S1AP_RRM_MME_DIRECT_INFO_TRANSFER",
    "S1AP_RRM_ENB_CONFIG_TRANSFER",
    "S1AP_RRM_MME_CONFIG_TRANSFER",
    "S1AP_RRM_PWS_REQ",
    "S1AP_RRM_PWS_RESP",
    "S1AP_RRM_PWS_CNF",
    "S1AP_RRM_KILL_REQ",
    "S1AP_RRM_KILL_RESP",
    "S1AP_RRM_KILL_CNF",
    "S1AP_RRM_RIM_INFO_REQ",
    "S1AP_RRM_RIM_INFO",
    "S1AP_RRM_RIM_INFO_ACK",
    "S1AP_RRM_RIM_INFO_ERROR",
    "S1AP_RRM_RIM_INFO_APP_ERROR",
    "RRC_RRM_RAN_INFO_REQ",
    "RRC_RRM_RAN_INFO",
    "RRC_RRM_RAN_INFO_ACK",
    "RRC_RRM_RAN_INFO_ERROR",
    "RRC_RRM_RAN_INFO_APP_ERROR	",
    "S1AP_RRM_MME_INFO",
    "S1AP_RRM_LPPA_ECID_MEAS_INIT_REQ",
    "S1AP_RRM_LPPA_ECID_MEAS_INIT_RESP",
    "S1AP_RRM_LPPA_ECID_MEAS_INIT_FAIL",
    "S1AP_RRM_LPPA_ECID_MEAS_FAIL_IND",
    "S1AP_RRM_LPPA_ECID_MEAS_REPORT",
    "S1AP_RRM_LPPA_ECID_MEAS_TERMINATION_COMMAND",
    "S1AP_RRM_LPPA_OTDOA_INFO_REQ",
    "S1AP_RRM_LPPA_OTDOA_INFO_RESP",
    "S1AP_RRM_LPPA_OTDOA_INFO_FAIL",
    "S1AP_RRM_LPPA_ERROR_IND",
    "S1AP_RRM_LPPA_ERROR_IND_REQ"
};

const char* rrm_son_api_name[] =
{
    "RRM_SON_REGISTER_REQ",
    "RRM_SON_DEREGISTER_REQ",
    "RRM_SON_SET_ATTRIBUTE_REQ",
    "RRM_SON_CELL_SWITCH_ON_REQ",
    "RRM_SON_CELL_SWITCH_OFF_REQ",
    "RRM_SON_NMM_PREPARE_REQ",
    "RRM_SON_NMM_COMPLETE_REQ",
    "RRM_SON_MEAS_CONFIG_REQ",
    "RRM_SON_TNL_DISCOVERY_REQ",
    "RRM_SON_MEAS_CONFIG_OBJ_REMOVE_REQ",
    "RRM_SON_RACH_CONFIG_REQ",
    "RRM_SON_ENB_CONFIG_UPDATE_IND"
};

const char* son_rrm_api_name[] =
{
    "RRM_SON_REGISTER_RESP",
    "RRM_SON_DEREGISTER_RESP",
    "RRM_SON_SET_ATTRIBUTE_RESP",
    "RRM_SON_CELL_SWITCH_ON_RESP",
    "RRM_SON_CELL_SWITCH_OFF_RESP",
    "RRM_SON_CELL_STATE_CHANGE_IND",
    "RRM_SON_NUM_ACTIVE_UE_REPORT",
    "RRM_UE_COUNT_THRESHOLD_REACHED_IND",
    "RRM_SON_NMM_PREPARE_RESP",
    "RRM_SON_NMM_COMPLETE_RESP",
    "RRM_SON_MEAS_CONFIG_RESP",
    "RRM_SON_MEAS_RESULTS_IND",
    "RRM_SON_HO_REPORT",
    "RRM_SON_TNL_DISCOVERY_RESP",
    "RRM_SON_HO_ATTEMPT_IND",
    "RRM_SON_HO_FAILURE_REPORT",
    "RRM_SON_LOAD_REPORT_IND",
    "RRM_SON_HO_EXPIRE_IND",
    "RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP",
    "RRM_SON_RACH_CONFIG_RESP",
    "RRM_SON_RACH_UE_INFO_IND",
    "RRM_SON_L2_RACH_PERF_REPORT",
    "RRM_SON_EICIC_ENABLE_DISABLE_IND"
};

const char* rrm_pdcp_api_name[] =
{
    "RRM_PDCP_UE_DRB_ECN_CE_REQ",
    "RRM_PDCP_CONFIG_PARAMS_REQ",
    "RRM_PDCP_CONFIGURE_KPI_STATS_REQ",
    "RRM_PDCP_GET_KPI_STATS_REQ",
    "RRM_PDCP_UE_DRB_ECN_CE_CNF",
    "RRM_PDCP_CONFIG_PARAMS_CNF",
    "RRM_PDCP_UE_DRB_ECN_CE_IND",
    "RRM_PDCP_CONFIGURE_KPI_STATS_CNF",
    "RRM_PDCP_KPI_STATS_IND",
    "RRM_PDCP_GET_KPI_STATS_CNF",
    "RRM_PDCP_KPI_THP_STATS_IND",
    "RRM_PDCP_GET_KPI_THP_STATS_CNF"
};

const char* rrm_oam_api_name[] =
{
    "RRM_OAM_INIT_IND",
    "RRM_OAM_INIT_CNF",
    "RRM_OAM_SHUTDOWN_REQ",
    "RRM_OAM_LOG_ENABLE_DISABLE_REQ",
    "RRM_OAM_LOG_ENABLE_DISABLE_RESP",
    "RRM_OAM_SET_LOG_LEVEL_REQ",
    "RRM_OAM_SET_LOG_LEVEL_RESP",
    "RRM_OAM_CELL_CONFIG_REQ",
    "RRM_OAM_CELL_CONFIG_RESP",
    "RRM_OAM_CELL_RECONFIG_REQ",
    "RRM_OAM_CELL_RECONFIG_RESP",
    "RRM_OAM_CELL_DELETE_REQ",
    "RRM_OAM_CELL_DELETE_RESP",
    "RRM_OAM_GENERIC_RESP",
    "RRM_OAM_INIT_CONFIG_REQ",
    "RRM_OAM_INIT_CONFIG_RESP",
    "RRM_OAM_CELL_START_REQ",
    "RRM_OAM_CELL_STOP_REQ",
    "RRM_OAM_RESUME_SERVICE_REQ",
    "RRM_OAM_RESUME_SERVICE_RESP",
    "RRM_OAM_CELL_START_RESP",
    "RRM_OAM_CELL_STOP_RESP",
    "RRM_OAM_CELL_DELETE_RESP",
    "RRM_OAM_RAC_ENABLE_DISABLE_REQ",
    "RRM_OAM_RAC_ENABLE_DISABLE_RESP",
    "RRM_OAM_SHUTDOWN_RESP",
    "RRM_OAM_CELL_CONTEXT_PRINT_REQ",
    "RRM_OAM_CARRIER_FREQ_AND_DL_TX_POWER_REQ",
    "RRM_OAM_CARRIER_FREQ_AND_DL_TX_POWER_RESP",
    "RRM_OAM_UE_RELEASE_REQ",
    "RRM_OAM_CELL_BLOCK_REQ",
    "RRM_OAM_CELL_BLOCK_RESP",
    "RRM_OAM_CELL_UNBLOCK_CMD",
    "RRM_OAM_READY_FOR_CELL_BLOCK_IND",
    "RRM_OAM_CELL_ECN_CAPACITY_ENHANCE_REQ",
    "RRM_OAM_CELL_ECN_CAPACITY_ENHANCE_RESP",
    "RRM_OAM_PROC_SUPERVISION_RESP",
    "RRM_OAM_CELL_UPDATE_REQ",
    "RRM_OAM_CELL_UPDATE_RESP",
    "RRM_OAM_GET_VER_ID_REQ",
    "RRM_OAM_GET_VER_ID_RESP",
    "RRM_OAM_EVENT_NOTIFICATION",
    "RRM_OAM_LOAD_CONFIG_REQ",
    "RRM_OAM_LOAD_CONFIG_RESP",
    "RRM_OAM_LOAD_REPORT_IND",
    "RRM_OAM_EVENT_CONFIG_REQ",
    "RRM_OAM_EVENT_CONFIG_RESP",
    "RRM_PLATFORM_LOAD_IND",
    "RRM_OAM_CONFIG_KPI_REQ",
    "RRM_OAM_CONFIG_KPI_RESP",
    "RRM_OAM_GET_KPI_REQ",
    "RRM_OAM_GET_KPI_RESP",
    "RRM_OAM_KPI_IND",
    "RRM_OAM_CHK_HEALTH_REQ",
    "RRM_OAM_CHK_HEALTH_RESP",
    "RRM_OAM_ENB_CONFIG_REQ",
    "RRM_OAM_ENB_CONFIG_RESP",
    "RRM_OAM_GET_LOG_LEVEL_REQ",
    /*+CID 147570 */
    "RRM_OAM_GET_LOG_LEVEL_RESP",
    /*-CID 147570 */
    /*spr 22474 start*/
    "RRM_OAM_GET_DEBUG_INFO_REQ",
    "RRM_OAM_GET_DEBUG_INFO_RESP"
    /*spr 22474 end*/
};

/****************************************************************************
 ** Function Name  : rrm_abort
 ** Inputs         : void
 ** Outputs        : None
 ** Returns        : void
 ** Description    : Encaps function for abort
 *****************************************************************************/
void rrm_abort(void)
{
        abort();
}


/******************************************************************************
 *   FUNCTION NAME: rrm_get_word_from_header
 *
 *   DESCRIPTION:
 *       Function retreives a word Id from RRM header
 *
 *   RETURNS:
 *       specific word
 *
 ******************************************************************************/
/******************************************************************************
  *   FUNCTION NAME: rrm_get_word_from_header
  *
  *   DESCRIPTION:
  *       Function retreives a word Id from RRM header
  *
  *   RETURNS:
  *       specific word
  *
  ******************************************************************************/
U16
rrm_get_word_from_header
(
 U8 *p_header
 )
{
    U16 value = 0xFFFF;
    value = (U16)(p_header[0] << 8 | p_header[1]);
    return value;
}

/*SPR 15436 Start*/
/******************************************************************************
 *   FUNCTION NAME: rrm_quick_sort
 *
 *   DESCRIPTION:
 *       Sort the list provided as a base pointer on the basis of the comp_func
 *
 *   RETURNS:
 *       void
 *
 ******************************************************************************/
void
rrm_quick_sort
(
 void *p_base,
 size_t num,
 size_t size,
 int (*p_comp_func)(const void *,const void *)
 )
{
    qsort(p_base, num,size, p_comp_func);

}
/*SPR 15436 End*/

/******************************************************************************
 *   FUNCTION NAME: rrm_get_cell_id_from_header
 *
 *   DESCRIPTION:
 *       Function cell index from interface header
 *
 *   RETURNS:
 *       Cell index from interface header 
 *
 ******************************************************************************/

rrm_cell_index_t
rrm_get_cell_id_from_header
(
 U8 *p_header
 )
{
    return p_header[EXT_CELL_INDEX_OFFSET];
}
/******************************************************************************
 *   FUNCTION NAME: rrm_get_src_module_id
 *
 *   DESCRIPTION:
 *       Function retreives source module Id from RRM header
 *
 *   RETURNS:
 *       Source module Id
 *
 ******************************************************************************/
rrm_module_id_t
rrm_get_src_module_id
(
 void *p_header /* RRM header */
 )
{
    RRM_ASSERT(p_header != NULL);

    return((rrm_module_id_t)(rrm_get_word_from_header(&(((U8 *)p_header)[1]))));
}

/*****************************************************************************
 * Function Name  : rrm_get_api_id
 * Inputs         : Pointer to input API buffer
 * Outputs        : None
 * Returns        : API id retrieved from API buffer
 * Description    : It retrieves API id from the input API buffer
 ****************************************************************************/
/*rrm_procedure_code_et*/
U16
rrm_get_api_id
(
 rrm_void_t *p_header
 )
{
    return(rrm_get_word_from_header(&(((U8 *)p_header)[5])));
}

/******************************************************************************
 *   FUNCTION NAME: rrm_get_api_buf_size
 *
 *   DESCRIPTION:
 *       Function retreives API buffer size from RRM header
 *
 *   RETURNS:
 *       API buffer size
 *
 ******************************************************************************/
U16
rrm_get_api_buf_size
(
 U8 *p_header /* RRM header */
 )
{
    return(rrm_get_word_from_header(&(((U8 *)p_header)[7])));
}

//start

#if (LOGGER_TYPE == RRM_ADV_LOGGER)

AdvL_ContextIndex        g_advl_ctxt[RRM_MODULE_ID_ARR];
/*****************************************************************************
 * Function Name  : rrm_init_log
 * Inputs         : p_log_file_name - Pointer to the log file name
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to initialize advance logger 
 ******************************************************************************/
rrm_void_t
rrm_init_log
(
    const S8 * p_log_file_name
)
{
    U32  module_id = RRM_ZERO;
    module_id = qvGetServiceName(qvSelf());
    if (0 != module_id )
    {
        if (RRM_PNULL == p_log_file_name)
        {
			/*CID 86967 Start*/
			if(LogSuccess != init_log(RRM_LOG_CTXT_FILE, DEFAULT_LOG_FILE,&g_advl_ctxt[module_id - RRM_MIN_MODULE_ID_FOR_ADV_LOGGER]))
			{
				fprintf(stderr, "Log Init Unsuccessful" );
			}
			/*CID 86967 End*/
        }
        else
        {
			/*CID 86967 Start*/
			if(LogSuccess != init_log(RRM_LOG_CTXT_FILE, ((char*)p_log_file_name),&g_advl_ctxt[module_id - RRM_MIN_MODULE_ID_FOR_ADV_LOGGER]))
			{
				fprintf(stderr, "Log Init Unsuccessful" );
			}
			/*CID 86967 End*/
        }
    }
}

/*****************************************************************************
 * Function Name  : rrm_set_module_loglevel_limit
 * Inputs         : log_level - Log level to be used(rrm_ERROR, rrm_BRIEF etc.)
 * Outputs        : None
 * Returns        : None
 * Description    : This function sets the module log level to advance logger
 ****************************************************************************/
rrm_void_t
rrm_set_module_loglevel_limit
(
    rrm_oam_log_level_et  log_level
)
{
    U32 module_id = RRM_ZERO;
    U8 rrm_log_level =0xFF;
    module_id = qvGetServiceName(qvSelf());
    RRM_LOG(RRM_TRUE, p_rrm_utils_facility_name, RRM_DETAILED,
            "Module Id:%d",module_id);

    if (0 == module_id)
    {
        return;
    }
    switch(log_level)
    {
	    case RRM_OAM_CRITICAL:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
		rrm_log_level = RRM_LOG_LEVEL_NONE;
		break;

	    case RRM_OAM_ERROR:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = RRM_ERROR;
            break;
        case RRM_OAM_WARNING:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = RRM_WARNING;
            break;
	    case RRM_OAM_INFO:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = RRM_INFO;
            break;
        case RRM_OAM_BRIEF:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = RRM_BRIEF;
            break;
        case RRM_OAM_DETAILED:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = RRM_DETAILED ;
            break;
        case RRM_OAM_DETAILEDALL:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = RRM_DETAILEDALL ;
            break;
        default:
            RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_WARNING, 
                "set_module_log_level: Invalid log level = %u received." 
                 "Using BRIEF as default",
                (U32)rrm_log_level);
            rrm_log_level = RRM_BRIEF;
            break;
    }
    if (0xFF != rrm_log_level)
    {
        /* coverity : CID 16316*/
        set_loglevel_limit(g_advl_ctxt[module_id - RRM_MIN_MODULE_ID_FOR_ADV_LOGGER], 
            (AdvL_LogLevel) rrm_log_level);
        RRM_LOG(RRM_TRUE, p_rrm_utils_facility_name, RRM_DETAILED,
            "set_module_log_level: Log level is set to %d for module with"
            " Id = %lu", log_level, qvName());
    }
}

/* SPR 21275 changes start */
#elif (LOGGER_TYPE == RRM_INTERNAL_LOGGER)
/* SPR 21275 changes end */
/*
 *   Internal logger
 */
/*****************************************************************************
 * Function Name  : set_module_log_level
 * Inputs         : log_level - Log level to be used(RRM_ERROR, RRM_BRIEF etc.)
 * Outputs        : None
 * Returns        : None
 * Description    : This function sets the module log level to CSPL logging
 *                  framework.
 ****************************************************************************/
void
set_module_log_level
(
    rrm_oam_log_level_et  log_level
)
{
    U32 rrm_log_level = 0xFF;

    RRM_UT_TRACE_ENTER();


    switch(log_level)
    {
    	case RRM_OAM_CRITICAL:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
		rrm_log_level = RRM_LOG_LEVEL_NONE +1;
		break;

    	case RRM_OAM_ERROR:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = RRM_ERROR +1;
            break;
        case RRM_OAM_WARNING:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = RRM_WARNING +1;
            break;
    	case RRM_OAM_INFO:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = RRM_INFO +1;
            break;
        case RRM_OAM_BRIEF:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = RRM_BRIEF +1;
            break;
        case RRM_OAM_DETAILED:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = RRM_DETAILED +1;
            break;
        case RRM_OAM_DETAILEDALL:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = RRM_DETAILEDALL +1;
            break;
        default:
            RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_WARNING, 
                "set_module_log_level: Invalid log level = %u received." 
                 "Using BRIEF as default",
                (U32)rrm_log_level);
            rrm_log_level = RRM_BRIEF + 1;
            break;
    }
    if (0xFF != rrm_log_level)
    {
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_DETAILED, 
                "set_module_log_level: Setting CSPL Log Level," 
                "loglevel(%d)", rrm_log_level);
        /* Register module log level with CSPL */

        qvSetModuleLogLevel(qvSelf(), rrm_log_level);
        rrm_log_level = qvGetModuleLogLevel(qvSelf());

        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_DETAILED, 
            "set_module_log_level: Getting CSPL Log Level, loglevel(%d)", 
            rrm_log_level);
    }

    RRM_UT_TRACE_EXIT();
}
#elif (LOGGER_TYPE == RRM_IN_MEM_LOGGER)
/* SPR 21275 changes start */
/*
 *   Internal logger
 */
/*****************************************************************************
 * Function Name  : set_module_log_level
 * Inputs         : log_level - Log level to be used(RRM_ERROR, RRM_BRIEF etc.)
 * Outputs        : None
 * Returns        : None
 * Description    : This function sets the module log level to CSPL logging
 *                  framework.
 ****************************************************************************/
void
set_module_log_level
(
    rrm_oam_log_level_et  log_level
)
{
    U32 rrm_log_level = 0xFF;

    RRM_UT_TRACE_ENTER();


    switch(log_level)
    {
    	case RRM_OAM_CRITICAL:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
		rrm_log_level = LGR_CRITICAL;
		break;

    	case RRM_OAM_ERROR:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = LGR_ERROR;
            break;
        case RRM_OAM_WARNING:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = LGR_WARNING;
            break;
    	case RRM_OAM_INFO:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = LGR_INFO;
            break;
        case RRM_OAM_BRIEF:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = LGR_BRIEF;
            break;
        case RRM_OAM_DETAILED:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = LGR_DETAILED;
            break;
        case RRM_OAM_DETAILEDALL:
            /* Add 1 to the log level since CSPL prints all the logs
                whose level is less than the level set */
            rrm_log_level = LGR_DEBUG;
            break;
        default:
            RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_WARNING, 
                "set_module_log_level: Invalid log level = %u received." 
                 "Using BRIEF as default",
                (U32)rrm_log_level);
            rrm_log_level = LGR_BRIEF;
            break;
    }
    if (0xFF != rrm_log_level)
    {
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_DETAILED, 
                "set_module_log_level: Setting CSPL Log Level," 
                "loglevel(%d)", rrm_log_level);
        /* Register module log level with CSPL */

        if (RRM_PNULL != qvSelf())
        {
            qvSetModuleLogLevel(qvSelf(), rrm_log_level);
            rrm_log_level = qvGetModuleLogLevel(qvSelf());
        }

        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_DETAILED, 
                "set_module_log_level: Getting CSPL Log Level, loglevel(%d)", 
                rrm_log_level);
    }

    RRM_UT_TRACE_EXIT();
}
/* SPR 21275 changes end */
#endif

#ifdef FILE_LOGGING
/******************************************************************************
 *   FUNCTION NAME: rrm_trace_message
 *
 *   DESCRIPTION:
 *       Implements the trace functionality.
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void rrm_trace_message(
        S32 log_level,              /* log level of this message */
        const S8 *facility_name,  /* identify who log message */
        const S8* format,         /* format string like in printf */
        ...                         /* optional parameters */
        )
{
    S8    tmp_fmt[FMT_MAX_LEN+1];
    S8    log_str[RRM_LOG_MAX_LOG_LEN];
    S8    temp_str[RRM_LOG_MAX_LOG_LEN/2];

    S8 temp_facility[8]="RRM";

    va_list arglist;

    RRM_MEMSET(&arglist,0,sizeof(va_list));
    RRM_MEMSET(log_str,  '\0', RRM_LOG_MAX_LOG_LEN);
    RRM_MEMSET(temp_str, '\0', (RRM_LOG_MAX_LOG_LEN/2));


    RRM_VA_START(arglist, format);
    if (NULL == facility_name)
    {
        RRM_SNPRINTF(log_str, (RRM_LOG_MAX_LOG_LEN / 2), "\n<%s>: ",
                (const char*)temp_facility);
    }
    else
    {
        /* Should add trailing 0 in case when string in snprintf is truncated */
        RRM_SNPRINTF(log_str, (RRM_LOG_MAX_LOG_LEN / 2), "\n<%s>: ",
                (const char*)facility_name);
    }
    switch(log_level)
    {
        case RRM_CRITICAL:
            RRM_STRNCAT(log_str, "[CRITICAL]: ", (RRM_LOG_MAX_LOG_LEN / 2));
            RRM_VSNPRINTF(temp_str, (RRM_LOG_MAX_LOG_LEN/2), 
                    (char *)format, arglist);
            break;
        case RRM_ERROR:
            RRM_STRNCAT(log_str, "[ERROR]: ", (RRM_LOG_MAX_LOG_LEN / 2));
            RRM_VSNPRINTF(temp_str, (RRM_LOG_MAX_LOG_LEN/2), 
                    (char *)format, arglist);
            break;
        case RRM_WARNING:
            RRM_STRNCAT(log_str, "[WARNING]: ", (RRM_LOG_MAX_LOG_LEN / 2));
            RRM_VSNPRINTF(temp_str, (RRM_LOG_MAX_LOG_LEN/2), 
                    (char *)format, arglist);
            break;
        case RRM_INFO:
            RRM_STRNCAT(log_str, "[INFO]: ", (RRM_LOG_MAX_LOG_LEN / 2));
            RRM_VSNPRINTF(temp_str, (RRM_LOG_MAX_LOG_LEN/2), 
                    (char *)format, arglist);
            break;
        case RRM_BRIEF:
            RRM_STRNCAT(log_str, "[BRIEF]: ", (RRM_LOG_MAX_LOG_LEN / 2));
            RRM_VSNPRINTF(temp_str, (RRM_LOG_MAX_LOG_LEN/2), 
                    (char *)format, arglist);
            break;
        case RRM_DETAILED:
            RRM_STRNCAT(log_str, "[DETAILED]: ", (RRM_LOG_MAX_LOG_LEN / 2));
            RRM_VSNPRINTF(temp_str, (RRM_LOG_MAX_LOG_LEN/2), 
                    (char *)format, arglist);
            break;
        case RRM_DETAILEDALL:
            RRM_STRNCAT(log_str, "[DETAILEDALL]: ", (RRM_LOG_MAX_LOG_LEN / 2));
            RRM_VSNPRINTF(temp_str, (RRM_LOG_MAX_LOG_LEN/2), 
                    (char *)format, arglist);
            break;
        default:
            RRM_STRNCAT(log_str, "[NONE]: ", (RRM_LOG_MAX_LOG_LEN / 2));
            RRM_VSNPRINTF(temp_str, (RRM_LOG_MAX_LOG_LEN/2), 
                    (char *)format, arglist);
            break;
    }

    RRM_VA_END(arglist);
    RRM_STRNCAT(log_str, temp_str, (RRM_LOG_MAX_LOG_LEN/2));
}
#endif
/******************************************************************************
 *   FUNCTION NAME: rrm_set_loglevel
 *
 *   DESCRIPTION:
 *       Update global variable rrm_log_level.
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
#if (LOGGER_TYPE == RRM_INTERNAL_LOGGER)
void rrm_set_loglevel(
        U8 new_log_level   /* New RRM log level */
        )
{
    qvLogLevel(new_log_level);
}
#endif 

/*SPR 20170 Fix Start*/
/*Code Deleted*/
/*SPR 20172 Fix Start*/
/*
 *   ASSERT
 */

#ifdef RRM_DEBUG
void rrm_assert(
        void* file,
        U32 line,
        void* expression)
{
    qvPanic("[%s:%u] Assertion Failed <%s>", (U8*)file, line, (U8*)expression);
    rrm_abort();
}
#endif /* RRM_DEBUG */

/******************************************************************************
 *   FUNCTION NAME: rrm_mem_get
 *
 *   DESCRIPTION:
 *       This function returns memory buffer from memory pool.
 *       Function is used for general memory management purposes.
 *
 *   RETURNS:
 *       Pointer to memory buffer or NULL in case of failure
 *
 ******************************************************************************/
void*
rrm_mem_get
(
 rrm_size_t size /* Size of buffer which will be allocated */
 )
{
    void *p_buf = RRM_PNULL;
#ifdef MEM_CHECK_DOUBLE_FREE
    p_buf = malloc(size);
#else
    int   error = RRM_ZERO;

    /*
      This function is replaced by the qvAllocEx to get 
      error-cause in case of mem allocation fail.
      p_buf = qvAlloc(size, RRM_PNULL); 
    */

    p_buf = qvAllocEx(size, RRM_PNULL, &error);
#endif
    if (p_buf == RRM_PNULL)
    {
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_ERROR, 
                "rrm_mem_get: memory allocation failed");

        /* alarm framework integration */
        ALARM_MSG_RRM(RRM_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,CRITICAL_ALARM);

        /*raising error event*/
        rrm_raise_error_event_notification(RRM_EE_MEM_ALLOC_FAIL);
    }
    return p_buf;
}

/******************************************************************************
 *   FUNCTION NAME: rrm_mem_free
 *
 *   DESCRIPTION:
 *       This function frees memory buffer allocated in pool.
 *       Function is used for general memory management purposes.
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void
rrm_mem_free
(
 void *p_buffer /* Pointer to buffer which will be freed */
 )
{
#ifdef MEM_CHECK_DOUBLE_FREE
    free(p_buffer);
#else
    if (p_buffer != RRM_PNULL)
    {
        qvFree(p_buffer);
        /* Re-initialize the buffer to RRM_PNULL */
        p_buffer = RRM_PNULL;
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_WARNING, "rrm_mem_free: null pointer passed");

        /*raising error event*/
        /*SPR 21060 +-*/
        rrm_raise_error_event_notification(RRM_EE_MEM_FREE_FAIL);
    }
#endif
}

/******************************************************************************
 *   FUNCTION NAME: rrm_msg_mem_get
 *
 *   DESCRIPTION:
 *       This function returns memory buffer from memory pool.
 *       Function is used for messages management purposes.
 *
 *   RETURNS:
 *       Pointer to memory buffer or NULL in case of failure
 *
 ******************************************************************************/
void*
rrm_msg_mem_get
(
 rrm_size_t size /* Size of buffer which will be allocated */
 )
{
    void *p_buf = RRM_PNULL;
#ifdef MEM_CHECK_DOUBLE_FREE
    p_buf = malloc(size);
#else
    int  error = RRM_ZERO;

    /* 
      This function is replaced by the qvMsgAllocEx to get
      error cause in case of mem allocation fails.
      p_buf = qvMsgAlloc(RRM_PNULL, 0, 0, size);
    */

    p_buf = qvMsgAllocEx(RRM_PNULL, 0, 0, size, &error);
#endif
    if(p_buf == RRM_PNULL)
    {
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_ERROR, "rrm_msg_mem_get: memory allocation failed");

        /* alarm framework integration */
        ALARM_MSG_RRM(RRM_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,CRITICAL_ALARM);

        /*raising error event*/
        rrm_raise_error_event_notification(RRM_EE_MEM_ALLOC_FAIL);
    }

    return(p_buf);
}

/******************************************************************************
 *   FUNCTION NAME: rrm_msg_mem_free
 *
 *   DESCRIPTION:
 *       This function frees memory buffer allocated in pool.
 *       Function is used for messages management purposes.
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void
rrm_msg_mem_free
(
 void *p_buffer /* Pointer to buffer which will be freed */
 )
{
#ifdef MEM_CHECK_DOUBLE_FREE
    free(p_buffer);
#else
    if (p_buffer != RRM_PNULL)
    {
        qvMsgFree(p_buffer);
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_WARNING, "rrm_msg_mem_free: null pointer passed");
        /*raising error event*/
        rrm_raise_error_event_notification(RRM_EE_MEM_FREE_FAIL);
    }
#endif
}

/*****************************************************************************
 * Function Name  : rrm_construct_cspl_header
 * Inputs         : p_buf - Pointer to the buffer where CSPL header needs to be
 *                  added
 *                  version - version of API
 *                  from - Source module Id
 *                  to - Destination module Id
 *                  api_id - API Identifier
 *                  priority - Priority of the message
 *                  payloadlen - lenght of the payload
 * Outputs        : None
 * Returns        : None
 * Description    : This functions appends the CSPL Header on the top of
 *                  the passed buffer.
 ****************************************************************************/
rrm_void_t
rrm_construct_cspl_header

(
 rrm_void_t  *p_buf,
 U8          version,
 U16         from,
 U16         to,
 U16         api_id,
 U8          priority,
 U16         paramlen
 )
{
    STACKAPIHDR cspl_hdr;
    cspl_hdr.priority = priority;
    cspl_hdr.version = version;
    cspl_hdr.from = from;
    cspl_hdr.to = to;
    cspl_hdr.paramlen = paramlen;
    cspl_hdr.payloadlen = RRM_ZERO;
    cspl_hdr.api = api_id;

    /* Append CSPL Header on top of p_buf */
    cvApiBuildHeader((U8 *)p_buf, &cspl_hdr);
}


/*****************************************************************************
 * Function Name  : rrm_parse_cspl_header
 * Inputs         : p_buf - Pointer to the buffer where CSPL header is present
 * Outputs        : p_cspl_hdr - Pointer to CSPL header
 * Returns        : None
 * Description    : This functions parse the CSPL Header and populates the
 *                  STACKAPIHDR structure
 ****************************************************************************/
rrm_void_t
rrm_parse_cspl_header
(
 rrm_void_t  *p_buf,
 STACKAPIHDR *p_cspl_hdr
 )
{
/*Coverity 86966 Fix Start */
    U32 msg_size = RRM_ZERO;
    msg_size = cvApiParseHeader(p_buf, CV_HDRSIZE, p_cspl_hdr);
    if(msg_size == RRM_ZERO)
    {
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_WARNING, "Failure in Parsing the Buffer");
    }
/*Coverity 86966 Fix End */
}


/******************************************************************************
 *   FUNCTION NAME: rrm_mem_get
 *
 *   DESCRIPTION:
 *       This function returns memory buffer using direct OS call allocation
 *       routine.
 *       Function is used for managing static buffers like rrm_gb_ctx.
 *
 *   RETURNS:
 *       Pointer to memory buffer or NULL in case of failure
 *
 ******************************************************************************/
void*
rrm_sys_mem_get
(
 rrm_size_t size /* Size of buffer which will be allocated */
 )
{
    void *p_buf = RRM_PNULL;

#ifdef MEM_CHECK_DOUBLE_FREE
    p_buf = malloc(size);
#else
    p_buf = (void *)qvSysMalloc(size);
#endif
    if(p_buf == RRM_PNULL)
    {
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_ERROR, "rrm_sys_mem_get: memory allocation failed");

        /* alarm framework integration */
        ALARM_MSG_RRM(RRM_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,CRITICAL_ALARM);
    }

    return(p_buf);
}


/*SPR 17777 +-*/

/******************************************************************************
 *   File management functions
 ******************************************************************************/

void
rrm_free
(
 void *p_var
 )
{
    free(p_var);
}

float
rrm_ceil
(
    float var
)
{
    float n = 0.0;
    n = ceil(var);
    return n;
}


S32 
rrm_fclose
(
 FILE *fp
 )
{
    return fclose(fp);
}
/* SPR 19337 : CID 11193 fix start */
FILE *
rrm_fopen
(
    const char* p_filename,
    const char* p_mode
)
{   
    return(fopen(p_filename,p_mode));
}
/* SPR 19337 : CID 11193 fix stop */
/*extern const   QMANIFEST CellMifEntity;*/

const QMANIFEST *p_manifest[] = {
    &CellRrmEntity,
    &UeRrmEntity,
    &CellMifEntity
};




/****************************************************************************
* Function Name  : rrm_logger_init
* Inputs         : p_rrm_logger_init_data, initial data
* Outputs        : None
* Returns        : None
* Description    : Start the monitoring of the memory
****************************************************************************/
rrm_void_t
*rrm_logger_init(	
		rrm_void_t *p_rrm_logger_init_data)
{
    rrm_logger_ctxt_t        *p_lg_ctxt = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    rrm_oam_set_status_for_health(RRM_ONE);
    p_lg_ctxt = (rrm_logger_ctxt_t*)rrm_mem_get(sizeof(rrm_logger_ctxt_t));

    /* Coverity_ID : 41522 */
	SET_MODULE_LOG_LEVEL(RRM_OAM_ERROR);

    lgr_process_log_segments();

    /* coverity : CID 41506 */
    if( RRM_PNULL != p_lg_ctxt )
    {
        p_lg_ctxt->wait_interval = RRM_ZERO;
    }
    else
    {
         /*SPR 17777 +-*/
         RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_ERROR,
        "Memory could not be alloacted for p_lg_ctxt;Unused variable p_rrm_logger_init_data=%p",p_rrm_logger_init_data );
         /*SPR 17777 +-*/
    }
    RRM_UT_TRACE_EXIT();
    return p_lg_ctxt;
}

/****************************************************************************
* Function Name  : rrm_mif_msg_handler
* Inputs         : p_api
* Outputs        : None
* Returns        : RRM_ZERO
* Description    : Handle the API received from MIF
****************************************************************************/
rrm_return_et
rrm_mif_msg_handler
(
 rrm_void_t  *p_api/*pointer to input API buffer*/
)
{
    void *p_data          = RRM_PNULL;
    U16	api_id	          = RRM_ZERO;
    cpu_set_t   cpuSetSch;
    U32         core_num = RRM_ZERO;
    U32         bitmap = RRM_ZERO;
    rrm_logger_init_cfg_t *p_lgr_cfg;
    rrm_logger_ctxt_t        *p_lg_ctxt = RRM_PNULL;
    QTIME                   time;
    /* SPR 15822 Fix Start */
    QTIMER	                timer;
    /* SPR 15822 Fix End */

    RRM_UT_TRACE_ENTER();

    api_id = rrm_get_api_id(p_api);
    p_data   = ((U8*)p_api) + RRM_API_HEADER_SIZE;  

    /*extracting message size*/

    switch(api_id)
    {
        case RMIF_LOGGER_INIT_IND:
            {
                p_lgr_cfg = (rrm_logger_init_cfg_t*)p_data;
                RRM_CPU_ZERO(&cpuSetSch);
                bitmap = p_lgr_cfg->cpu_bitmap;

                if (RRM_ZERO != bitmap)
                {
                    while (RRM_ZERO != bitmap)
                    {
                        if (bitmap & RRM_ONE)
                        {
                            RRM_CPU_SET(core_num ,&cpuSetSch);
                        }
                        bitmap = bitmap >> RRM_ONE;
                        core_num++;
                    }

#ifdef RRM_IN_MEM_LOGGER
                    if (RRM_ZERO != pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuSetSch))
                    {
                        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_ERROR,
                                "Core binding for the logger failed on CPU [%u], errno[%u]", core_num, errno);
                    }
#endif
                }
                p_lg_ctxt = qvThisContext();
                /* Coverity: CID 42113 */
                if(RRM_PNULL != p_lg_ctxt)
                {
                    p_lg_ctxt->wait_interval = p_lgr_cfg->wait_interval;
                    time.s = p_lg_ctxt->wait_interval;
                    time.us = RRM_ZERO;
                    /* SPR 15822 Fix Start */
                    timer = qvTimerStart(&time, RRM_NULL, RRM_ZERO);
                    RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_BRIEF, 
                            "Timer Started: Timer [%p]", timer);
                    /* SPR 15822 Fix End */
                }
                else
                {
                    RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_ERROR, "p_lg_ctxt is NULL");
                }
            }
            break;

        default:
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_ERROR,
                "Invalid API [%u] received", api_id);
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }

    RRM_UT_TRACE_EXIT();
    return RRM_SUCCESS;
}

/****************************************************************************
* Function Name  : rrm_logger_process_msg
* Inputs         : p_api_buf, p_gl_ctx
* Outputs        : None
* Returns        : RRM_ZERO
* Description    : Logger message handler function. All message for logger
*                  thread will be processed by this function
****************************************************************************/
S32
rrm_logger_process_msg
(
 rrm_void_t *p_api_buf/*pointer to api buffer*/,
 rrm_void_t *p_gl_ctx/*pointer to global logger context*/)
{
	U16         src_id  =  RRM_ZERO;
    RRM_UT_TRACE_ENTER();

	src_id = rrm_get_src_module_id(p_api_buf);

	switch (src_id)
    {
		case RRM_MIF_MODULE_ID:
            {
                rrm_mif_msg_handler(p_api_buf);
            }
            break;

        default:
         /*SPR 17777 +-*/
			RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR, 
					"Unknown interface message from Module Id (%d);Unused variable p_gl_ctx=%p", src_id,p_gl_ctx);
         /*SPR 17777 +-*/
            break;
    }

    RRM_UT_TRACE_EXIT();
    return RRM_ONE;
}

/****************************************************************************
 * Function Name  : rrm_logger_process_timer_msg
 * Inputs         : timer_id, p_buf, p_context
 * Outputs        : None
 * Returns        : None
 * Description    : Function for handling the timer expiry of the logger thread
 ****************************************************************************/

rrm_void_t
rrm_logger_process_timer_msg
(
 QTIMER timer_id,
 rrm_void_t *p_buf,
 rrm_void_t *p_context)
{
    QTIME                   time;
    QTIMER                  tmr;
    rrm_logger_ctxt_t        *p_lg_ctxt = RRM_PNULL;

    lgr_process_log_segments();

    rrm_oam_set_status_for_health(RRM_ONE);

    p_lg_ctxt = qvThisContext();

    /* Coverity: CID 42112 */
    if(RRM_PNULL != p_lg_ctxt)
    {
        time.s = p_lg_ctxt->wait_interval;
        time.us = RRM_ZERO;
        tmr = qvTimerStart(&time, RRM_NULL, RRM_ZERO);
/* SPR 20636 Changes Start*/
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_DETAILED, "Started Timer[0x%x]", (U32_U32Long_Bit)tmr);
/* SPR 20636 Changes End*/
    }
    else
    {
         /*SPR 17777 +-*/
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_ERROR, "p_lg_ctxt is NULL"
                "Unused variable timer_id=%p p_buf=%p p_context%p",timer_id,p_buf,p_context);
         /*SPR 17777 +-*/
    }

    return;
}

static const unsigned long loggerDestinationList[] =
{
	0
};

const   QMANIFEST LoggerEntity =
{
	"RRM_LOGGER_MODULE_ID",
	RRM_LOGGER_MODULE_ID,

	{ RRM_PNULL, rrm_logger_init },
	{ rrm_logger_process_msg, rrm_logger_process_timer_msg },
	{ RRM_PNULL, RRM_PNULL, RRM_PNULL },
	loggerDestinationList /* Does not send message to any module */
};

/*****************************************************************************
 * Function Name  : rrm_register_modules
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : It registers the modules to the CSPL. The module is
 *                  described by its respective manifest data structure
 ****************************************************************************/
rrm_void_t
rrm_register_modules
(
    thrd_arg_t *p_thrd_arg
 )
{
    /*SPR 17777 +-*/
    (void)p_thrd_arg;
    U32             index   = RRM_ZERO;
#ifdef RRM_IN_MEM_LOGGER
    pthread_t       tid     = RRM_ZERO;
/*SPR 22554 Fix Start*/
    U8  threadName[RRM_THREAD_NAME_LEN] = "rrm_logger";
/*SPR 22554 Fix End*/
#endif

    RRM_UT_TRACE_ENTER();


    for (index = RRM_ZERO; index < sizeof(p_manifest)/sizeof(QMANIFEST*); index++)
    {
        qvRegister(0, 0, 0, p_manifest[index], 0);
    }

#ifdef RRM_IN_MEM_LOGGER
    qvRegister(p_thrd_arg->driver_id, 0, 0, &LoggerEntity, 0);

    pthread_create(&tid, 0, rrm_thread_start, (void *)p_thrd_arg);
/*SPR 22554 Fix Start*/

    /* Set name of the RRM module thread */
    if (0 != rrm_pthread_setname_np(&tid, threadName))
    {
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_WARNING,
                      "Failed to set name of the RRM module thread");
    }

/*SPR 22554 Fix End*/
    /* creating Driver loop for Logger Stack */
#endif

    RRM_UT_TRACE_EXIT();
    return;
}

/*SPR 17777 +-*/



void
/*SPR 20172 Fix START*/
rrm_send_message_ex
/*SPR 20172 Fix END*/
(
 void            *p_msg,     /* Message that will be passed */
 rrm_module_id_t dst_module  /* Module id for which message will be passed */
 )
{
    int error_code = RRM_NULL;
    QMODULE service = qvGetServiceEx(dst_module,(int *)&error_code);

    RRM_ASSERT(RRM_PNULL != p_msg);

    if(QVERROR_NONE != error_code)
    {
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_ERROR, "Cannot get service name: %s",
                error_code == QVERROR_INVALID_ARGUMENT ?
                "invalid argument passed" :
                "module allocation failed");

        /* alarm framework integration */
        ALARM_MSG_RRM(RRM_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);

        qvPanic("Cannot get service name: %s",
                error_code == QVERROR_INVALID_ARGUMENT ?
                "invalid argument passed" :
                "module allocation failed");
    }
    qvSend(service, 0, p_msg);
}



void
rrm_construct_interface_api_header
(
 U8                  *p_header,      /* RRM interface header */
 U16                 transaction_id, /* Interface transaction identifier */
 rrm_module_id_t     src_module_id,  /* Source module identifier */
 rrm_module_id_t     dst_module_id,  /* Destination module identifier */
 U16                 api_id,         /* API identifier */
 U16                 api_buf_size,   /* API buffer size */
 rrm_cell_index_t    cell_index      /* Cell Index */
 )
{
    /* transaction identifier (HI) */
    p_header[0] = (U8)((transaction_id & 0xFF00) >> 8);

    /* transaction identifier (LOW) */
    p_header[1] = (U8)(transaction_id & 0x00FF);

    /* from (HI) */
    p_header[2] = (U8)((src_module_id & 0xFF00) >> 8);

    /* from (LOW) */
    p_header[3] = (U8)(src_module_id & 0x00FF);

    /* to (HI) */
    p_header[4] = (U8)((dst_module_id & 0xFF00) >> 8);

    /* to (LOW) */
    p_header[5] = (U8)(dst_module_id & 0x00FF);

    /* api id (HI) */
    p_header[6] = (U8)((api_id & 0xFF00) >> 8);
    /* api id (LOW) */
    p_header[7] = (U8)(api_id & 0x00FF);

    /*size includes length of header*/
    api_buf_size = (U16)(api_buf_size + RRM_INTERFACE_API_HEADER_SIZE);

    /* api size (HI) */
    p_header[8] = (U8)((api_buf_size & 0xFF00) >> 8);

    /* api size (LOW) */
    p_header[9] = (U8)(api_buf_size & 0x00FF);

    p_header[10]=cell_index;

}

void
rrm_construct_api_header
(
 U8                  *p_header,      /* RRM header */
 U8                  version_id,     /* API version Id */
 rrm_module_id_t     src_module_id,  /* Source module Id */
 rrm_module_id_t     dst_module_id,  /* Destination module Id */
 U16                 api_id,         /* API Id */
 U16                 api_buf_size    /* API buffer size */
 )
{
    /* version */
    p_header[0] = version_id;

    /* from (HI) */
    p_header[1] = (U8)((src_module_id & 0xFF00) >> 8);

    /* from (LOW) */
    p_header[2] = (U8)(src_module_id & 0x00FF);

    /* to (HI) */
    p_header[3] = (U8)((dst_module_id & 0xFF00) >> 8);

    /* to (LOW) */
    p_header[4] = (U8)(dst_module_id & 0x00FF);

    /* api id (HI) */
    p_header[5] = (U8)((api_id & 0xFF00) >> 8);

    /* api id (LOW) */
    p_header[6] = (U8)(api_id & 0x00FF);

    /* api size (HI) */
    p_header[7] = (U8)((api_buf_size & 0xFF00) >> 8);

    /* api size (LOW) */
    p_header[8] = (U8)(api_buf_size & 0x00FF);

    /* spare bytes */
    p_header[9]      = 0x00;
    p_header[10]    = 0x00;
    p_header[11]    = 0x00;
    p_header[12]    = 0x00;
    p_header[13]    = 0x00;
    p_header[14]    = 0x00;
    p_header[15]    = 0x00;
}

/* CSR-58333-fix start*/
/* rrm_stop_timer function has been deleted */
/* rrm_start_timer function has been deleted */
/* CSR-58333-fix end*/

/*****************************************************************************
 * Function Name  : rrm_generate_txn_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : Transcation Id
 * Description    : This function generates the transaction identity value
 ******************************************************************************/
/* + SPR 17439 */
U16
rrm_generate_txn_id(void)
/* - SPR 17439 */
{
    static U16 tx_id = RRM_MIN_TRANSACTION_ID;
    if (tx_id < RRM_MAX_TRANSACTION_ID)
    {
         tx_id++;
    }
    else
    {
        tx_id = RRM_MIN_TRANSACTION_ID;
    }
    RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_DETAILED, 
        "rrm_generate_txn_id: Transaction Id generated = %u", tx_id);
    return tx_id;
}

/*CODE OPT START*/
/****************************************************************************
 * Function Name  : rrm_memcpy_rrm_son_tnl_discovery_res
 * Inputs         : p_dst: Pointer to destination tnl discovery location
 *                  p_src: Pointer to source tnl discovery location
 * Outputs        : None
 * Returns        : None
 *Description    : Memory copy function for rrm_son_tnl_discovery_res
 ****************************************************************************/
void
rrm_memcpy_rrm_son_tnl_discovery_res(
    rrm_son_tnl_discovery_res_t *p_dst,
    rrm_son_tnl_discovery_res_t *p_src)
{
    U8    count = RRM_ZERO;
    U8 count1 = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    p_dst->result = p_src->result;
    p_dst->error_code = p_src->error_code;
   
    p_dst->target_enb_id.enb_type = p_src->target_enb_id.enb_type;    
   
    rrm_memcpy_rrm_oam_cell_plmn_info(&p_dst->target_enb_id.plmn_id,
                                         &p_src->target_enb_id.plmn_id); 

    for(count=RRM_ZERO; count < HOME_ENB_ID_OCTET_SIZE; ++count)
    {
        p_dst->target_enb_id.eNb_id[count] =
                              p_src->target_enb_id.eNb_id[count];
    }

    p_dst->enb_tnl_address_list_size = p_src->enb_tnl_address_list_size;

    for(count=RRM_ZERO; count < p_dst->enb_tnl_address_list_size; ++count)
    {
        p_dst->enb_tnl_address_list[count].bitmask = 
                          p_src->enb_tnl_address_list[count].bitmask;
   
        if ((p_dst->enb_tnl_address_list[count].bitmask) &
                                           RRM_IPV4_ADD_PRESENT)
        {
            for(count1 = RRM_ZERO; count1 < RRM_IPV4_SIZE; ++count1)
            {
                p_dst->enb_tnl_address_list[count].ipv4_add[count1] =
                       p_src->enb_tnl_address_list[count].ipv4_add[count1];
            }
        }
            
        if ((p_dst->enb_tnl_address_list[count].bitmask) &
                                            RRM_IPV6_ADD_PRESENT)
        {
            for(count1 = RRM_ZERO; count1 < RRM_IPV6_SIZE; ++count1)
            {
                p_dst->enb_tnl_address_list[count].ipv6_add[count1] =
                      p_src->enb_tnl_address_list[count].ipv6_add[count1];
            } 
        }                     
    }
}
/****************************************************************************
 * Function Name  : rrm_memcpy_rrm_oam_cell_plmn_info
 * Inputs         : p_dst: Pointer to destination plmn info location
 *                  p_src: Pointer to source plmn info location
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for rrm_memcpy_rrm_oam_cell_plmn_info 
 ****************************************************************************/
void
rrm_memcpy_rrm_oam_cell_plmn_info(
    rrm_oam_cell_plmn_info_t *p_dst,
    rrm_oam_cell_plmn_info_t *p_src)
{
    U8    count = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    for(count = RRM_ZERO; count < MAX_MCC_DIGITS; ++count)
    {
        p_dst->mcc[count] =  p_src->mcc[count];
    }

    p_dst->num_mnc_digit = p_src->num_mnc_digit;

    for(count = RRM_ZERO; count < p_dst->num_mnc_digit; ++count)
    {
        p_dst->mnc[count] =  p_src->mnc[count];
    }
    /* Klocwork changes start 22 may */
    return;
    /* Klocwork changes end 22 may */
}
/****************************************************************************
 * Function Name  : rrm_memcpy_rrm_son_tnl_discovery_req
 * Inputs         : p_dst: Pointer to destination tnl discovery req
 *                  p_src: Pointer to source tnl discovery req
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for rrm_son_tnl_discovery_req
 ****************************************************************************/
void
rrm_memcpy_rrm_son_tnl_discovery_req(
    rrm_son_tnl_discovery_req_t *p_dst,
    rrm_son_tnl_discovery_req_t *p_src)
{
    U8    count = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    p_dst->target_enb_id.enb_type = p_src->target_enb_id.enb_type;

    rrm_memcpy_rrm_oam_cell_plmn_info(&p_dst->target_enb_id.plmn_id,
                                        &p_src->target_enb_id.plmn_id);

    for(count=RRM_ZERO; count<HOME_ENB_ID_OCTET_SIZE; count++)
    {
        p_dst->target_enb_id.eNb_id[count] = 
                           p_src->target_enb_id.eNb_id[count];
    }

    for(count=RRM_ZERO; count<MAX_TAC_SIZE; count++)
    {
        p_dst->tac[count] = p_src->tac[count];
    }

}
/****************************************************************************
 * Function Name  : rrm_memcpy_rrmcm_rmif_generic_resp
 * Inputs         : p_dst: Pointer to destination cm_rmif response location
 *                  p_src: Pointer to source cm_rmif response location
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for rrm_memcpy_rrmcm_rmif_generic_resp
 ****************************************************************************/
 void
rrm_memcpy_rrmcm_rmif_generic_resp(
    rrmcm_rmif_generic_resp_t *p_dst,
    rrmcm_rmif_generic_resp_t *p_src)
{
    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    p_dst->cellindex = p_src->cellindex;

    p_dst->transaction_id = p_src->transaction_id; 

    p_dst->response = p_src->response;

    p_dst->fail_cause = p_src->fail_cause;
}

/****************************************************************************
 * Function Name  : rrm_memcpy_rrmuem_rmif_emergency_call_active_res
 * Inputs         : p_dst: Pointer to destination active emergency call resp
 *                  p_src: Pointer to source active emergency call resp
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for 
 *                  rrm_memcpy_rrmuem_rmif_emergency_call_active_res
 ****************************************************************************/
 void
rrm_memcpy_rrmuem_rmif_emergency_call_active_res(
    rrmuem_rmif_emergency_call_active_res_t *p_dst,
    rrmuem_rmif_emergency_call_active_res_t *p_src)
{
    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    p_dst->cell_index = p_src->cell_index;

    p_dst->transaction_id = p_src->transaction_id;

    p_dst->emergency_call_ongoing = p_src->emergency_call_ongoing;
}
/****************************************************************************
 * Function Name  : rrm_memcpy_rrmuem_rmif_non_emrgncy_active_calls_ho_resp
 * Inputs         : p_dst: Pointer to destination active non-emergency call resp
 *                  p_src: Pointer to source active non-emergency call resp
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for
 *                  rrm_memcpy_rrmuem_rmif_non_emrgncy_active_calls_ho_resp
 ****************************************************************************/
 void
rrm_memcpy_rrmuem_rmif_non_emrgncy_active_calls_ho_resp(
    rrmuem_rmif_non_emrgncy_active_calls_ho_resp_t *p_dst,
    rrmuem_rmif_non_emrgncy_active_calls_ho_resp_t *p_src)
{
    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    p_dst->cell_index = p_src->cell_index;

    p_dst->transaction_id = p_src->transaction_id;

    p_dst->response = p_src->response;
}
/****************************************************************************
 * Function Name  : rrm_memcpy_rrmuem_rmif_no_active_ue_ind
 * Inputs         : p_dst: Pointer to destination uem_rmif no active ue indication
 *                  p_src: Pointer to source uem_rmif no active ue indication
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for
 *                  rrm_memcpy_rrmuem_rmif_no_active_ue_ind
 ****************************************************************************/
 void
rrm_memcpy_rrmuem_rmif_no_active_ue_ind(
    rrmuem_rmif_no_active_ue_ind_t *p_dst,
    rrmuem_rmif_no_active_ue_ind_t *p_src)
{
    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    p_dst->cell_index = p_src->cell_index;
}

/****************************************************************************
 * Function Name  : rrm_memcpy_rrmcm_rmif_cell_config_resp
 * Inputs         : p_dst: Pointer to destination cm_rmif cell config response 
 *                  p_src: Pointer to source cm_rmif cell config response
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for rrm_memcpy_rrmcm_rmif_cell_config_resp
 ****************************************************************************/
void
rrm_memcpy_rrmcm_rmif_cell_config_resp(
    rrmcm_rmif_cell_config_resp *p_dst,
    rrmcm_rmif_cell_config_resp *p_src)
{
    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    rrm_memcpy_rrm_global_cell_id(&p_dst->global_cell_id,
                                   &p_src->global_cell_id);
 
    p_dst->cellindex = p_src->cellindex;

    p_dst->transaction_id = p_src->transaction_id;

    p_dst->response = p_src->response;

    p_dst->fail_cause = p_src->fail_cause;
}

/****************************************************************************
 * Function Name  : rrm_memcpy_rrm_global_cell_id
 * Inputs         : p_dst: Pointer to destination global cell id
 *                  p_src: Pointer to source global cell id
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for rrm_memcpy_rrm_global_cell_id
 ****************************************************************************/
void
rrm_memcpy_rrm_global_cell_id(
    rrm_global_cell_id_t *p_dst,
    rrm_global_cell_id_t *p_src)
{
    U8    count = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    rrm_memcpy_rrm_oam_cell_plmn_info(&p_dst->primary_plmn_id,
                                       &p_src->primary_plmn_id);

    for(count=RRM_ZERO; count<MAX_CELL_IDENTITY_OCTETS; count++)
    {
        p_dst->cell_identity[count] = p_src->cell_identity[count];
    }
}
/****************************************************************************
 * Function Name  : rrm_memcpy_rrmcm_rmif_rac_enable_disable_res
 * Inputs         : p_dst: Pointer to destination rac enable_disable response
 *                  p_src: Pointer to source rac enable_disable response
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for 
 *                  rrm_memcpy_rrmcm_rmif_rac_enable_disable_res
 ****************************************************************************/
void
rrm_memcpy_rrmcm_rmif_rac_enable_disable_res(
    rrmcm_rmif_rac_enable_disable_res_t *p_dst,
    rrmcm_rmif_rac_enable_disable_res_t *p_src)
{
    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    p_dst->cell_index = p_src->cell_index;

    p_dst->transaction_id = p_src->transaction_id;

    p_dst->response = p_src->response;

    p_dst->fail_cause = p_src->fail_cause;

    p_dst->bitmask = p_src->bitmask;

    p_dst->is_cell_index_present = p_src->is_cell_index_present;
}
/****************************************************************************
 * Function Name  : rrm_memcpy_rrm_ue_count_threshold_reached_ind
 * Inputs         : p_dst: Pointer to destination ue count threshold
 *                  p_src: Pointer to source ue count threshold
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for 
 *                  rrm_memcpy_rrm_ue_count_threshold_reached_ind
 ****************************************************************************/
 void
rrm_memcpy_rrm_ue_count_threshold_reached_ind(
    rrm_ue_count_threshold_reached_ind_t *p_dst,
    rrm_ue_count_threshold_reached_ind_t *p_src)
{
    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    p_dst->cellindex = p_src->cellindex;

    p_dst->cause = p_src->cause;
}
/****************************************************************************
 * Function Name  : rrm_memcpy_rrmcm_rmif_cell_active_ue_report_ind
 * Inputs         : p_dst: Pointer to destination cm_rmif cell active ue report
 *                  p_src: Pointer to source cm_rmif cell active ue report
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for
 *                  rrm_memcpy_rrmcm_rmif_cell_active_ue_report_ind
 ****************************************************************************/
 void
rrm_memcpy_rrmcm_rmif_cell_active_ue_report_ind(
    rrmcm_rmif_cell_active_ue_report_ind_t *p_dst,
    rrmcm_rmif_cell_active_ue_report_ind_t *p_src)
{
    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    p_dst->cellindex = p_src->cellindex;

    p_dst->ue_cnt = p_src->ue_cnt;
}
/****************************************************************************
 * Function Name  : rrm_memcpy rrmcm_rmif_cell_state_change_ind_
 * Inputs         : p_dst: Pointer to destination cm_rmif cell state change ind
 *                  p_src: Pointer to source cm_rmif cell state change ind
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for
 *                  rrm_memcpy_rrmcm_rmif_cell_state_change_ind
 ****************************************************************************/
 void
rrm_memcpy_rrmcm_rmif_cell_state_change_ind(
    rrmcm_rmif_cell_state_change_ind_t *p_dst,
    rrmcm_rmif_cell_state_change_ind_t *p_src)
{
    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    p_dst->cellindex = p_src->cellindex;

    p_dst->cell_state = p_src->cell_state;
    
    p_dst->cell_state_change_cause = p_src->cell_state_change_cause;
}

/****************************************************************************
 * Function Name  : rrm_memcpy_rrm_oam_eutran_global_cell_id
 * Inputs         : p_dst: Pointer to destination eutran global cell id
 *                  p_src: Pointer to source eutran global cell id
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for 
 *                  rrm_memcpy_rrm_oam_eutran_global_cell_id
 ****************************************************************************/
void
rrm_memcpy_rrm_oam_eutran_global_cell_id(
    rrm_oam_eutran_global_cell_id_t *p_dst,
    rrm_oam_eutran_global_cell_id_t *p_src)
{
    U8    count = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    rrm_memcpy_rrm_oam_cell_plmn_info(&p_dst->primary_plmn_id,
                                        &p_src->primary_plmn_id);

    for(count=RRM_ZERO; count<MAX_CELL_IDENTITY_OCTETS; count++)
    {
        p_dst->cell_identity[count] =  p_src->cell_identity[count];
    }
}
/****************************************************************************
 * Function Name  : rrm_memcpy_rrm_son_utran_physical_cell_id
 * Inputs         : p_dst: Pointer to destination PCI location
 *                  p_src: Pointer to source PCI location
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for 
 *                  rrm_memcpy_rrm_son_utran_physical_cell_id
 ****************************************************************************/
void
rrm_memcpy_rrm_son_utran_physical_cell_id(
    rrm_son_utran_physical_cell_id_t *p_dst,
    rrm_son_utran_physical_cell_id_t *p_src)
{
    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    p_dst->bitmask = p_src->bitmask;

    if (p_dst->bitmask & RRM_PCI_FDD_PRESENT)
    {
        p_dst->pci_fdd = p_src->pci_fdd;
    }

    if (p_dst->bitmask & RRM_PCI_TDD_PRESENT)
    {
        p_dst->pci_tdd = p_src->pci_tdd;
    }
}
/****************************************************************************
 * Function Name  : rrm_memcpy_rrm_son_utran_geran_global_cell_id
 * Inputs         : p_dst: Pointer to destination CGI location
 *                  p_src: Pointer to source CGI location
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for
 *                  rrm_memcpy_rrm_son_utran_geran_global_cell_id
 ****************************************************************************/
void
rrm_memcpy_rrm_son_utran_geran_global_cell_id(
    rrm_son_utran_geran_global_cell_id_t *p_dst,
    rrm_son_utran_geran_global_cell_id_t *p_src)
{
    U8    count = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    p_dst->rat_type = p_src->rat_type;

    rrm_memcpy_rrm_oam_cell_plmn_info(&p_dst->plmn_id,
                                        &p_src->plmn_id);   

    for(count=RRM_ZERO; count<MAX_CELL_IDENTITY_OCTETS; count++)
    {
        p_dst->cell_identity[count] =  p_src->cell_identity[count];
    }
}
/****************************************************************************
 * Function Name  : rrm_memcpy_lac
 * Inputs         : p_dst: Pointer to destination LAC
 *                  p_src: Pointer to source LAC
                    size: total number 
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for
 *                  rrm_memcpy_lac
 ****************************************************************************/
void
rrm_memcpy_lac(
    U8 *p_dst,
    U8 *p_src,
    U16 size)
{
    U8    count = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    for(count=RRM_ZERO; count<size; count++)
    {
        p_dst[count] =  p_src[count];
    }
}
/****************************************************************************
 * Function Name  : rrm_memcpy_tac
 * Inputs         : p_dst: Pointer to destination TAC
 *                  p_src: Pointer to source TAC
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for
 *                  rrm_memcpy_tac
 ****************************************************************************/
void
rrm_memcpy_tac(
    U8 *p_dst,
    U8 *p_src)
{
    U8    count = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    for(count=RRM_ZERO; count<MAX_TAC_SIZE; count++)
    {
        p_dst[count] =  p_src[count];
    }
}
/****************************************************************************
 * Function Name  : rrm_memcpy_mcc
 * Inputs         : p_dst: Pointer to destination MCC
 *                  p_src: Pointer to source MCC
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for
 *                  rrm_memcpy_mcc
 ****************************************************************************/
void
rrm_memcpy_mcc(
    U8 *p_dst,
    U8 *p_src)
{
    U8    count = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    for(count=RRM_ZERO; count<MAX_MCC_DIGITS; count++)
    {
        p_dst[count] =  p_src[count];
    }
}

/****************************************************************************
 * Function Name  : rrm_memcpy_mnc
 * Inputs         : p_dst: Pointer to destination MNC
 *                  p_src: Pointer to source MNC
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for
 *                  rrm_memcpy_mnc
 ****************************************************************************/
void
rrm_memcpy_mnc(
    U8 *p_dst,
    U8 *p_src,
    U8 size)
{
    U8    count = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    for(count=RRM_ZERO; (count < size) && (count < MAX_MNC_DIGITS); 
                                               count++)
    {
        p_dst[count] =  p_src[count];
    }
}
/****************************************************************************
 * Function Name  : rrm_memcpy_u8
 * Inputs         : p_dst: Pointer to destination U8
 *                  p_src: Pointer to source U8
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for
 *                  rrm_memcpy_u8
 ****************************************************************************/
void
rrm_memcpy_u8(
    U8 *p_dst,
    U8 *p_src,
    U8 size)
{
    U8    count = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    for(count=RRM_ZERO; count<size; count++)
    {
        p_dst[count] =  p_src[count];
    }
}
/****************************************************************************
 * Function Name  : rrm_memcpy_u16
 * Inputs         : p_dst: Pointer to destination U16
 *                  p_src: Pointer to source U16
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for
 *                  rrm_memcpy_u16
 ****************************************************************************/
void
rrm_memcpy_u16(
    U16 *p_dst,
    U16 *p_src,
    U16 size)
{
    U8    count = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    for(count=RRM_ZERO; count<size; count++)
    {
        p_dst[count] =  p_src[count];
    }
}
/****************************************************************************
 * Function Name  : rrm_memcpy_plmn_id
 * Inputs         : p_dst: Pointer to destination PLMN
 *                  p_src: Pointer to source PLMN
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for
 *                  rrm_memcpy_plmn_id
 ****************************************************************************/
void
rrm_memcpy_plmn_id(
    rrm_oam_cell_plmn_info_t *p_dst,
    rrm_oam_cell_plmn_info_t *p_src)
{
    U8    count = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_dst);
    RRM_ASSERT(RRM_PNULL != p_src);

    for(count=RRM_ZERO; count<MAX_PLMN_ID_LIST2; count++)
    {
        rrm_memcpy_rrm_oam_cell_plmn_info(p_dst+count ,
                                          p_src+count);
    }
}
/****************************************************************************
* Function Name  : rrm_memcpy_rrc_uplink_um_rlc_erb_service_profile
* Inputs         : p_dst: Pointer to destination rlc erb service profile 
*                  p_src: Pointer to source rlc erb service profile
* Outputs        : None
* Returns        : None
* Description    : Memory copy function for
*                  rrm_memcpy_rrc_uplink_um_rlc_erb_service_profile
****************************************************************************/
 void
rrm_memcpy_rrc_uplink_um_rlc_erb_service_profile(
    rrc_uplink_um_rlc_erb_service_profile_t *p_dst,
    rrc_uplink_um_rlc_erb_service_profile_t *p_src)
{
    p_dst->sn_field_length = p_src->sn_field_length;
}
/****************************************************************************
* Function Name  : rrm_memcpy_rrm_son_geran_physical_cell_id
* Inputs         : p_dst: Pointer to destination priority cfg
*                  p_src: Pointer to source priority cfg
* Outputs        : None
* Returns        : None
* Description    : Memory copy function for
*                  rrm_memcpy_rrm_son_geran_physical_cell_id
****************************************************************************/
 void
rrm_memcpy_rrm_son_geran_physical_cell_id(
    rrm_son_geran_physical_cell_id_t *p_dst,
    rrm_son_geran_physical_cell_id_t *p_src)
{
    p_dst->ncc = p_src->ncc;
    p_dst->bscc = p_src->bscc;
}

/****************************************************************************
 * Function Name  : rrm_memcpy_rrm_son_registration_based_res
 * Inputs         : p_dst: Pointer to SON registration resp
 *                  p_src: Pointer to SON registration resp
 * Outputs        : None
 * Returns        : None
 * Description    : Memory copy function for
 *                  rrm_memcpy_rrm_son_registration_based_res
 ****************************************************************************/
void
rrm_memcpy_rrm_son_registration_based_res(
    rrm_son_registration_based_res_t *p_dst,
    rrm_son_registration_based_res_t *p_src)
{
    U8    count = RRM_ZERO;

    rrm_memcpy_rrm_oam_cell_plmn_info(
                                 &(p_dst->id.object_id.cell_id.primary_plmn_id),
                                 &(p_src->id.object_id.cell_id.primary_plmn_id));

    for(count=RRM_ZERO; count<MAX_CELL_IDENTITY_OCTETS;
                                                  count++)
    {
        p_dst->id.object_id.cell_id.cell_identity[count] =
                            p_src->id.object_id.cell_id.cell_identity[count];
    }

    p_dst->result = p_src->result;

    p_dst->error_code = p_src->error_code;
}


/****************************************************************************
* Function Name  : rrm_memcpy_rrc_eutran_type
* Inputs         : p_dst: Pointer to destination priority cfg
*                  p_src: Pointer to source priority cfg
* Outputs        : None
* Returns        : None
* Description    : Memory copy function for
*                  rrm_memcpy_rrc_eutran_type
****************************************************************************/
 void
rrm_memcpy_rrc_eutran_type(
    rrc_eutran_type_t *p_dst,
    rrc_eutran_type_t *p_src)
{
    p_dst->cell_size = p_src->cell_size;
}

/****************************************************************************
* Function Name  : rrm_memcpy_rrm_l2_get_snr_report
* Inputs         : p_dst: Pointer to destination priority cfg
*                  p_src: Pointer to source priority cfg
* Outputs        : None
* Returns        : None
* Description    : Memory copy function for
*                  rrm_memcpy_rrm_l2_get_snr_report
****************************************************************************/
 void
rrm_memcpy_rrm_l2_get_snr_report(
    rrm_l2_get_snr_report_t *p_dst,
    rrm_l2_get_snr_report_t *p_src)
{
    p_dst->ue_index = p_src->ue_index;
}

/****************************************************************************
* Function Name  : rrm_memcpy_rrm_l2_snr_report
* Inputs         : p_dst: Pointer to destination priority cfg
*                  p_src: Pointer to source priority cfg
* Outputs        : None
* Returns        : None
* Description    : Memory copy function for
*                  rrm_memcpy_rrm_l2_snr_report
****************************************************************************/
 void
rrm_memcpy_rrm_l2_snr_report(
    rrm_l2_snr_report_t *p_dst,
    rrm_l2_snr_report_t *p_src)
{
    p_dst->ue_index = p_src->ue_index;
    p_dst->averageULSINR = p_src->averageULSINR;
    p_dst->averageDLSNR = p_src->averageDLSNR;
    p_dst->timingAdvanceValue = p_src->timingAdvanceValue;
}

/****************************************************************************
* Function Name  : rrm_memcpy_rrm_scheduling_info_list
* Inputs         : p_dst: Pointer to destination priority cfg
*                  p_src: Pointer to source priority cfg
* Outputs        : None
* Returns        : None
* Description    : Memory copy function for
*                  rrm_memcpy_rrm_scheduling_info_list
****************************************************************************/
void
rrm_memcpy_rrm_scheduling_info_list(
    rrm_scheduling_info_list_t *p_dst,
    rrm_scheduling_info_list_t *p_src)
{
    U8 index;
    p_dst->count = p_src->count;

    for(index = RRM_ZERO; index <p_dst->count; index++)
    {
        rrm_memcpy_rrm_scheduling_info(&(p_dst->scheduling_info[index]),
                                         &(p_src->scheduling_info[index]));
    }
}

/****************************************************************************
* Function Name  : rrm_memcpy_rrm_scheduling_info
* Inputs         : p_dst: Pointer to destination priority cfg
*                  p_src: Pointer to source priority cfg
* Outputs        : None
* Returns        : None
* Description    : Memory copy function for
*                  rrm_memcpy_rrm_scheduling_info
****************************************************************************/
void
rrm_memcpy_rrm_scheduling_info(
    rrm_scheduling_info_t *p_dst,
    rrm_scheduling_info_t *p_src)
{
   rrm_memcpy_rrm_sib_mapping_info(&(p_dst->sib_mapping_info),
                                    &(p_src->sib_mapping_info));
   p_dst->si_periodicity = p_src->si_periodicity;
}

/****************************************************************************
* Function Name  : rrm_memcpy_rrm_sib_mapping_info
* Inputs         : p_dst: Pointer to destination priority cfg
*                  p_src: Pointer to source priority cfg
* Outputs        : None
* Returns        : None
* Description    : Memory copy function for
*                  rrm_memcpy_rrm_sib_mapping_info
****************************************************************************/
void
rrm_memcpy_rrm_sib_mapping_info(
    rrm_sib_mapping_info_t *p_dst,
    rrm_sib_mapping_info_t *p_src)
{
    p_dst->count = p_src->count;
    rrm_memcpy_u8(p_dst->sib_type,
                  p_src->sib_type,
                  p_dst->count);
}

/*************************************************
        MEMSET
*************************************************/



/****************************************************************************
* Function Name  : rrm_memset_rrm_oam_cell_plmn_info
* Inputs         : rrm_oam_cell_plmn_info_t variable
*                   new value
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_oam_cell_plmn_info
****************************************************************************/
void
rrm_memset_rrm_oam_cell_plmn_info(
        rrm_oam_cell_plmn_info_t *p_dst,
        U8 val)
{
    U8 counter = RRM_ZERO ;
    for(counter = RRM_ZERO; counter < MAX_MCC_DIGITS; counter++)
    {
        p_dst->mcc[counter] = val;
    }
    
    p_dst->num_mnc_digit = val;
    
    for(counter = RRM_ZERO; counter < MAX_MNC_DIGITS; counter++)
    {
        p_dst->mnc[counter] = val;
    }
}



/****************************************************************************
* Function Name  : rrm_memset_rrm_oam_eutran_global_cell_id
* Inputs         : rrm_oam_eutran_global_cell_id_t variable
*                   new value
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_oam_eutran_global_cell_id
****************************************************************************/
void
rrm_memset_rrm_oam_eutran_global_cell_id(
        rrm_oam_eutran_global_cell_id_t *p_dst,
        U8 val)
{
    U8 counter = RRM_ZERO;
    rrm_memset_rrm_oam_cell_plmn_info( &(p_dst->primary_plmn_id), val);
    for(counter = RRM_ZERO; counter<MAX_CELL_IDENTITY_OCTETS; counter++)
    {
        p_dst->cell_identity[counter] = val;
    }   
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_memset_rrm_son_eutran_neighbor_cell
* Inputs         : rrm_son_eutran_neighbor_cell_t variable
*                   new value
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_eutran_neighbor_cell
****************************************************************************/
void 
rrm_memset_rrm_son_eutran_neighbor_cell(
        rrm_son_eutran_neighbor_cell_t *p_dst,
        U8 val)
{
    U8 counter = RRM_ZERO;
    
    p_dst->bitmask = val;
    p_dst->pci = val;
    
    rrm_memset_rrm_oam_eutran_global_cell_id( &(p_dst->cgi), val);
    
    p_dst->num_plmn_id = val;
    
    for(counter = RRM_ZERO ;counter < MAX_PLMN_ID_LIST2; counter++)
    {
        rrm_memset_rrm_oam_cell_plmn_info( &(p_dst->plmn_id[counter]), val);
    }
    
    for(counter = RRM_ZERO ;counter < MAX_TAC_SIZE; counter++)
    {
        p_dst->tac[counter] = val;
    }
    
    for(counter = RRM_ZERO ;counter < MAX_CELL_IDENTITY_OCTETS; counter++)
    {
        p_dst->csg_identity[counter] = val;
    }
    
    p_dst->rsrp = val;
    p_dst->rsrq = val;
    /* Coverity_ID : 30977 */
    p_dst->csg_membershipStatus = (rrm_csg_membership_status_et)val;
}

/****************************************************************************
* Function Name  : rrm_memset_rrm_son_eutran_meas_results
* Inputs         : rrm_son_eutran_meas_results_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_eutran_meas_results
****************************************************************************/
void 
rrm_memset_rrm_son_eutran_meas_results(
        rrm_son_eutran_meas_results_t *p_dst,
        U8 val)
{
    U8 counter = RRM_ZERO;
    p_dst->earfcn = val;
    p_dst->neighbor_cell_list_size = val;
    for(counter = RRM_ZERO; counter< RRM_MAX_CELLS_REPORTED; counter++)
    {
        rrm_memset_rrm_son_eutran_neighbor_cell( &(p_dst->neighbor_cell_list[counter]), val);
    }
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_son_utran_neighbor_cell
* Inputs         : rrm_son_utran_neighbor_cell_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_utran_neighbor_cell
****************************************************************************/
void
rrm_memset_rrm_son_utran_neighbor_cell(
        rrm_son_utran_neighbor_cell_t *p_dst,
        U8 val)
{
    U8 counter = RRM_ZERO;
    p_dst->bitmask = val;
    
    (p_dst->pci).bitmask = val;
    (p_dst->pci).pci_fdd = val;
    (p_dst->pci).pci_tdd = val;
    /*coverity : CID 30786 */ 
    rrm_memset_rrm_oam_cell_plmn_info( &(p_dst->cgi.plmn_id), val);
    for(counter= RRM_ZERO ;counter < MAX_CELL_IDENTITY_OCTETS; counter++)
    {
        p_dst->cgi.cell_identity[counter] = val;
    }
 
    for( counter = RRM_ZERO ;counter < MAX_LAC_SIZE; counter++)
    {
        p_dst->lac[counter] = val;
    }
    
    p_dst->rac = val;
    p_dst->num_plmn_id = val;
    
    for(counter= RRM_ZERO ;counter < MAX_PLMN_ID_LIST2; counter++)
    {
        rrm_memset_rrm_oam_cell_plmn_info( &(p_dst->plmn_id[counter]), val);
    }
    
    for(counter= RRM_ZERO ;counter < MAX_CELL_IDENTITY_OCTETS; counter++)
    {
        p_dst->csg_identity[counter] = val;
    }
    
    p_dst->rscp = (S8)val;  
    p_dst->ecno = val;
    p_dst->cgi.rat_type = (rrm_rat_type_et) val;
    /* coverity : CID 30977,30753 */
    p_dst->csg_membershipStatus = (rrm_csg_membership_status_et)val;
}



/****************************************************************************
* Function Name  : rrm_memset_rrm_son_utran_meas_results
* Inputs         : rrm_son_utran_meas_results_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_utran_meas_results
****************************************************************************/
void
rrm_memset_rrm_son_utran_meas_results(
        rrm_son_utran_meas_results_t *p_dst,
        U8 val)
{
    U8 counter = RRM_ZERO;
    p_dst->uarfcn = val;
    p_dst->neighbor_cell_list_size = val;
    for(counter = RRM_ZERO; counter< RRM_MAX_CELLS_REPORTED; counter++)
    {
        rrm_memset_rrm_son_utran_neighbor_cell( &(p_dst->neighbor_cell_list[counter]), val);
    }
}

/****************************************************************************
* Function Name  : rrm_memset_rrm_son_cdma_meas_results 
* Inputs         : rrm_son_cdma_meas_results_t *variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_cdma_meas_results
****************************************************************************/
void
rrm_memset_rrm_son_cdma_meas_results (rrm_son_cdma_meas_results_t *p_dst, U8 val)
{
	U8 count = RRM_ZERO;
	U8 count1 = RRM_ZERO;

	p_dst->bitmask = val;
	p_dst->meas_obj_id = val;
	p_dst->neighbor_cell_list_size = val;
	for (count = RRM_ZERO; count < RRM_MAX_CELLS_REPORTED; ++count)
	{
		p_dst->neighbor_cell_list[count].bitmask = val;
		p_dst->neighbor_cell_list[count].arfcn = val;
                /* coverity : CID 30934 */
		p_dst->neighbor_cell_list[count].band_class = (rrm_son_cdma_band_class_et)val;
		p_dst->neighbor_cell_list[count].pci = val;
		p_dst->neighbor_cell_list[count].cgi.bitmask = val;
		for (count1 = RRM_ZERO; count1 < RRM_SON_CELL_GLOBAL_ID_1X_RTT_SIZE; ++count1)
		{
			p_dst->neighbor_cell_list[count].cgi.cell_global_id_1X_RTT[count1] = val;
		}
		for (count1 = RRM_ZERO; count1 < RRM_SON_CELL_GLOBAL_ID_HRPD_SIZE; ++count1)
		{
			p_dst->neighbor_cell_list[count].cgi.cell_global_id_HRPD[count1] = val;
		}
	}
}
/****************************************************************************
* Function Name  : rrm_memset_rrm_son_geran_neighbor_cell
* Inputs         : rrm_son_geran_neighbor_cell_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_geran_neighbor_cell
****************************************************************************/
void
rrm_memset_rrm_son_geran_neighbor_cell( 
        rrm_son_geran_neighbor_cell_t *p_dst,
        U8 val)
{
    U8    counter = RRM_ZERO;

    p_dst->bitmask = val;
    p_dst->arfcn = val;
   
    /* coverity  : CID 30975 */ 
    p_dst->band_ind = (rrm_son_geran_band_indicator_et) val;
    
    (p_dst->pci).ncc = val;
    (p_dst->pci).bscc = val;
   
    /* coverity : CID 30964,30749 */ 
    (p_dst->cgi).rat_type = (rrm_rat_type_et)val;
    for(counter = RRM_ZERO; counter < MAX_MCC_DIGITS; counter++)
    {
        (p_dst->cgi).plmn_id.mcc[counter] = val;
    }
    (p_dst->cgi).plmn_id.num_mnc_digit = val;
    for(counter = RRM_ZERO; counter < MAX_MNC_DIGITS; counter++)
    {
        (p_dst->cgi).plmn_id.mnc[counter] = val;
    }
    
    for(counter =RRM_ZERO; counter < MAX_CELL_IDENTITY_OCTETS; counter++)
    {
        (p_dst->cgi).cell_identity[counter] = val;
    }
    
    p_dst->rac = val;
    p_dst->rssi = val;
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_son_geran_meas_results
* Inputs         : rrm_son_geran_meas_results_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_geran_meas_results
****************************************************************************/
void 
rrm_memset_rrm_son_geran_meas_results(
        rrm_son_geran_meas_results_t *p_dst,
        U8 val)
{
    U8 counter=RRM_ZERO;
    
    p_dst->bitmask = val;
    p_dst->meas_obj_id = val;
    p_dst->neighbor_cell_list_size = val;
    
    for(counter=RRM_ZERO; counter < RRM_MAX_CELLS_REPORTED ; counter++)
    {
        rrm_memset_rrm_son_geran_neighbor_cell( &(p_dst->neighbor_cell_list[counter]), val);
    }
    
    
}



/****************************************************************************
* Function Name  : rrm_memset_rrm_oam_carrier_freq_dl_tx_params_resp
* Inputs         : rrm_oam_carrier_freq_dl_tx_params_resp_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_oam_carrier_freq_dl_tx_params_resp
****************************************************************************/
/*
 void 
rrm_memset_rrm_oam_carrier_freq_dl_tx_params_resp(
    rrm_oam_carrier_freq_dl_tx_params_resp_t *p_dst ,
    U8 val)
{
    p_dst->dl_earfcn = val;
    p_dst->reference_signal_power = val;
    p_dst->result = val;
    p_dst->error_code = val;
    p_dst->p_max = val;
}*/
/****************************************************************************
* Function Name  : rrm_memset_rrm_oam_ue_release_req
* Inputs         : rrm_oam_ue_release_req_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_oam_ue_release_req
****************************************************************************/
 void
rrm_memset_rrm_oam_ue_release_req(
    rrm_oam_ue_release_req_t *p_dst,
    U8 val)
{
    p_dst->ue_index = val;
}

/****************************************************************************
* Function Name  : rrm_memset_rrm_son_meas_config_res
* Inputs         : rrm_son_meas_config_res_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_meas_config_res
****************************************************************************/
void
rrm_memset_rrm_son_meas_config_res(
    rrm_son_meas_config_res_t *p_dst,
    U8 val)
{
    U8 counter = RRM_ZERO;
    rrm_memset_rrm_oam_cell_plmn_info(&((p_dst->meas_config_res).cell_id.primary_plmn_id), val);
    for(counter = RRM_ZERO; counter < MAX_CELL_IDENTITY_OCTETS; counter++)
    {
        (p_dst->meas_config_res).cell_id.cell_identity[counter] = val;
    }
     /* coverity : CID : 30970,30969,30750 */ 
    (p_dst->meas_config_res).result = (rrm_return_et)val;
    (p_dst->meas_config_res).error_code = (rrm_error_et) val;
        
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_son_meas_results_ind
* Inputs         : rrm_son_meas_results_ind_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_meas_results_ind
****************************************************************************/
void
rrm_memset_rrm_son_meas_results_ind(
    rrm_son_meas_results_ind_t *p_dst,
    U8 val)
{
    U8 counter = RRM_ZERO;
    
    rrm_memset_rrm_oam_cell_plmn_info( &((p_dst->cell_id).primary_plmn_id), val );
    for(counter = RRM_ZERO; counter < MAX_CELL_IDENTITY_OCTETS; counter++)
    {
        (p_dst->cell_id).cell_identity[counter] = val;
    }
    
    p_dst->ue_index = val;
    p_dst->eutran_meas_list_size = val;
    p_dst->utran_meas_list_size = val;
    p_dst->geran_meas_list_size = val;
	p_dst->cdma_meas_list_size = val;
    
    for(counter = RRM_ZERO; counter < RRM_MAX_CELLS_REPORTED; counter++)
    {
        rrm_memset_rrm_son_eutran_meas_results (&(p_dst->eutran_meas_list[counter]), val);
        rrm_memset_rrm_son_geran_meas_results ((rrm_son_geran_meas_results_t *)&(p_dst->geran_meas[counter]), val);
        rrm_memset_rrm_son_utran_meas_results ((rrm_son_utran_meas_results_t*)&(p_dst->utran_meas[counter]), val);
		rrm_memset_rrm_son_cdma_meas_results (&p_dst->cdma_meas[counter], val);
    }
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_oam_cell_config_resp
* Inputs         : rrm_oam_cell_config_resp_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_oam_cell_config_resp
****************************************************************************/
void
rrm_memset_rrm_oam_cell_config_resp(
    rrm_oam_cell_config_resp_t *p_dst,
    U8 val)
{
    U8 counter = RRM_ZERO;
    rrm_memset_rrm_oam_cell_plmn_info(&((p_dst->global_cell_id).primary_plmn_id), val);
    for(counter = RRM_ZERO; counter < MAX_CELL_IDENTITY_OCTETS; counter++)
    {
        (p_dst->global_cell_id).cell_identity[counter] = val;
    }
    /* Coverity: CID 30984,30980,30742 */ 
    p_dst->response = (rrm_return_et)val;
    p_dst->fail_cause = (rrm_error_et)val;
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_oam_cell_reconfig_resp
* Inputs         : rrm_memset_rrm_oam_cell_reconfig_resp_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_memset_rrm_oam_cell_reconfig_resp
****************************************************************************/
void
rrm_memset_rrm_oam_cell_reconfig_resp(
    rrm_oam_cell_reconfig_resp_t *p_dst,
    U8 val)
{
    U8 counter = RRM_ZERO;
    rrm_memset_rrm_oam_cell_plmn_info(&((p_dst->global_cell_id).primary_plmn_id), val);
    for(counter = RRM_ZERO; counter < MAX_CELL_IDENTITY_OCTETS; counter++)
    {
        (p_dst->global_cell_id).cell_identity[counter] = val;
    }
    /* Coverity: CID 30984,30980,30743 */ 
    p_dst->response = (rrm_return_et)val;
    p_dst->fail_cause = (rrm_error_et)val;
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_oam_delete_resp
* Inputs         : rrm_oam_delete_resp_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_oam_delete_resp
****************************************************************************/
void
rrm_memset_rrm_oam_delete_resp(
    rrm_oam_delete_resp_t *p_dst,
    U8 val)
{
    U8 counter = RRM_ZERO;
    rrm_memset_rrm_oam_cell_plmn_info(&((p_dst->global_cell_id).primary_plmn_id), val);
    for(counter = RRM_ZERO; counter < MAX_CELL_IDENTITY_OCTETS; counter++)
    {
        (p_dst->global_cell_id).cell_identity[counter] = val;
    }
    /* Coverity: CID 30984,30980,30746 */ 
    p_dst->response = (rrm_return_et)val;
    p_dst->fail_cause = (rrm_error_et)val;
}



/****************************************************************************
* Function Name  : rrm_memset_rrm_oam_cell_start_resp
* Inputs         : rrm_oam_cell_start_resp_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_oam_cell_start_resp
****************************************************************************/
void
rrm_memset_rrm_oam_cell_start_resp(
    rrm_oam_cell_start_resp_t *p_dst,
    U8 val)
{
    U8 counter = RRM_ZERO;
    rrm_memset_rrm_oam_cell_plmn_info(&((p_dst->global_cell_id).primary_plmn_id), val);
    for(counter = RRM_ZERO; counter < MAX_CELL_IDENTITY_OCTETS; counter++)
    {
        (p_dst->global_cell_id).cell_identity[counter] = val;
    }
    /* Coverity: CID 30984,30980,30744 */ 
    p_dst->response = (rrm_return_et)val;
    p_dst->fail_cause = (rrm_error_et)val;
}



/****************************************************************************
* Function Name  : rrm_memset_rrm_oam_cell_stop_resp
* Inputs         : rrm_oam_cell_stop_resp_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_oam_cell_stop_resp
****************************************************************************/
void
rrm_memset_rrm_oam_cell_stop_resp(
    rrm_oam_cell_stop_resp_t *p_dst,
    U8 val)
{
    U8 counter = RRM_ZERO;
    rrm_memset_rrm_oam_cell_plmn_info(&((p_dst->global_cell_id).primary_plmn_id), val);
    for(counter = RRM_ZERO; counter < MAX_CELL_IDENTITY_OCTETS; counter++)
    {
        (p_dst->global_cell_id).cell_identity[counter] = val;
    }
    /* Coverity: CID 30984,30980,30745 */ 
    p_dst->response = (rrm_return_et)val;
    p_dst->fail_cause = (rrm_error_et)val;
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_oam_cell_block_resp
* Inputs         : rrm_oam_cell_block_resp_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_oam_cell_block_resp
****************************************************************************/
void
rrm_memset_rrm_oam_cell_block_resp(
    rrm_oam_cell_block_resp_t *p_dst,
    U8 val)
{
    U8 counter = RRM_ZERO;
    rrm_memset_rrm_oam_cell_plmn_info(&((p_dst->global_cell_id).primary_plmn_id), val);
    for(counter = RRM_ZERO; counter < MAX_CELL_IDENTITY_OCTETS; counter++)
    {
        (p_dst->global_cell_id).cell_identity[counter] = val;
    }
   
    /* coverity : CID 30984,30980,30741 */ 
    p_dst->response = (rrm_return_et) val;
    p_dst->fail_cause = (rrm_error_et) val;
}


/****************************************************************************
* Function Name  : rrm_memset_son__and_dl_power_params_req
* Inputs         : rrmcm_rmif_cell_son_register_deregister_req_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrmcm_rmif_cell_son_register_deregister_req
****************************************************************************/
void
rrm_memset_son__and_dl_power_params_req(
    rrmcm_rmif_cell_son_register_deregister_req_t *p_dst,
    U8 val)
{
    U8 counter = RRM_ZERO;
    U8 counter2 = RRM_ZERO;
    p_dst->cellindex = val;
    p_dst->transaction_id = val;
    p_dst->attr_list_size = val;
    for(counter = RRM_ZERO ; counter < (RRM_ATTR_UNDEF-1); counter++)
    {
         /* coverity : CID 30951 ,30936,30755 */
        (p_dst->attribute_list[counter]).id = (rrm_cellm_attribute_id_et)val;
        (p_dst->attribute_list[counter]).trigger_list = val;
        (p_dst->attribute_list[counter]).value = val;
        for(counter2 = RRM_ZERO; counter < RRM_TRGR_UNDEF-1; counter++)
        {
            ((p_dst->attribute_list[counter]).trigger_type[counter2]).trigger_id = (rrm_cellm_trigger_type_et)val;
            ((p_dst->attribute_list[counter]).trigger_type[counter2]).trigger_type.periodic = val;
        }
    }
}

/* RACH_OPTIMIZATION_CHANGES_START */
/****************************************************************************
* Function Name  : rrm_memset_rrm_son_rach_info_res
* Inputs         : rrm_son_rach_config_resp_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_rach_info_res
****************************************************************************/
void
rrm_memset_rrm_son_rach_info_res(
    rrm_son_rach_config_resp_t *p_dst,
    U8 val)
{
    rrm_memset_rrm_oam_eutran_global_cell_id( &(p_dst->cell_id) , val);
    p_dst->result = (rrm_return_et)val;
    p_dst->error_code = (rrm_error_et)val; /* Coverity: CID 41386,30979,30985 */
}
/* RACH_OPTIMIZATION_CHANGES_END */
/****************************************************************************
* Function Name  : rrm_memset_rrm_son_registration_based_res
* Inputs         : rrm_son_registration_based_res_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_registration_based_res
****************************************************************************/
void
rrm_memset_rrm_son_registration_based_res(
    rrm_son_registration_based_res_t *p_dst,
    U8 val)
{
    rrm_memset_rrm_oam_eutran_global_cell_id( &((p_dst->id).object_id.cell_id) , val);
    p_dst->result = (rrm_return_et)val;
    p_dst->error_code = (rrm_error_et)val; /* Coverity: CID 30985,30979,30751 */
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_son_set_attr_res
* Inputs         : rrm_son_set_attr_res_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_set_attr_res
****************************************************************************/
void
rrm_memset_rrm_son_set_attr_res(
    rrm_son_set_attr_res_t *p_dst,
    U8 val)
{
    rrm_memset_rrm_son_registration_based_res( &(p_dst->set_attr_resp), val);
    /* coverity : CID 30981 */
    p_dst->id = (rrm_attr_id_et)val;
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_son_registration_res
* Inputs         : rrm_son_registration_res_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_registration_res
****************************************************************************/
void
rrm_memset_rrm_son_registration_res(
    rrm_son_registration_res_t *p_dst,
    U8 val)
{
    rrm_memset_rrm_son_registration_based_res( &(p_dst->registration_resp), val);
}


/****************************************************************************
*Function Name  : rrm_memset_rrm_son_deregistration_res
* Inputs         : rrm_son_deregistration_res_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_deregistration_res
****************************************************************************/
void
rrm_memset_rrm_son_deregistration_res(
    rrm_son_deregistration_res_t *p_dst,
    U8 val)
{
    rrm_memset_rrm_son_registration_based_res( &(p_dst->deregistration_resp), val);
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_son_cell_switch_off_res
* Inputs         : rrm_son_cell_switch_off_res_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_cell_switch_off_res
****************************************************************************/
void
rrm_memset_rrm_son_cell_switch_off_res(
    rrm_son_cell_switch_off_res_t *p_dst,
    U8 val)
{
    rrm_memset_rrm_oam_eutran_global_cell_id(&((p_dst->cell_switchoff_res).cell_id),val);
     /* coverity : CID 30961,30960,30747*/
    (p_dst->cell_switchoff_res).result = (rrm_return_et)val;
    (p_dst->cell_switchoff_res).error_code =(rrm_error_et)val;
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_son_cell_switch_on_res
* Inputs         : rrm_son_cell_switch_on_res_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_cell_switch_on_res
****************************************************************************/
void
rrm_memset_rrm_son_cell_switch_on_res(
    rrm_son_cell_switch_on_res_t *p_dst,
    U8 val)
{
    rrm_memset_rrm_oam_eutran_global_cell_id(&((p_dst->cell_switchon_res).cell_id),val);
     /* coverity  :CID 30963,30962,30748  */
    (p_dst->cell_switchon_res).result = (rrm_return_et)val;
    (p_dst->cell_switchon_res).error_code = (rrm_error_et)val;
}

/****************************************************************************
* Function Name  : rrm_memset_rrm_son_cell_switch_on_req
* Inputs         : rrm_son_cell_switch_on_req_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_cell_switch_on_req
****************************************************************************/
void
rrm_memset_rrm_son_cell_switch_on_req(
    rrm_son_cell_switch_on_req_t *p_dst,
    U8 val)
{
    rrm_memset_rrm_oam_eutran_global_cell_id( &(p_dst->cell_id), val);
}

/****************************************************************************
* Function Name  : rrm_memset_rrm_oam_ready_for_cell_block_ind
* Inputs         : rrm_oam_ready_for_cell_block_ind_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_oam_ready_for_cell_block_ind
****************************************************************************/
void
rrm_memset_rrm_oam_ready_for_cell_block_ind(
    rrm_oam_ready_for_cell_block_ind_t *p_dst,
    U8 val)
{
    rrm_memset_rrm_oam_eutran_global_cell_id( &(p_dst->global_cell_id), val);
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_enb_tnl_address
* Inputs         : rrm_enb_tnl_address_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_enb_tnl_address
****************************************************************************/
void
rrm_memset_rrm_enb_tnl_address(
    rrm_enb_tnl_address_t *p_dst,
    U8 val)
{
    U8 counter = RRM_ZERO;
    p_dst->bitmask = val;
    for(counter = RRM_ZERO; counter < RRM_IPV4_SIZE; counter++)
    {
        p_dst->ipv4_add[counter] = val;
    }
    
    for(counter = RRM_ZERO; counter < RRM_IPV6_SIZE; counter++)
    {
        p_dst->ipv6_add[counter] = val;
    }
}


/****************************************************************************
* Function Name  : rrm_memset_rrm_son_tnl_discovery_res
* Inputs         : rrm_son_tnl_discovery_res_t variable
*                   new value
                    message length
* Outputs        : None
* Returns        : None
* Description    : Memory set function for rrm_son_tnl_discovery_res
****************************************************************************/
void 
rrm_memset_rrm_son_tnl_discovery_res(
    rrm_son_tnl_discovery_res_t *p_dst,
    U8 val)
{
    U8 counter = RRM_ZERO;
    /* coverity : CID 30985,30979,30752 */
    p_dst->result = (rrm_return_et) val;
    p_dst->error_code = (rrm_error_et)val;
    p_dst->enb_tnl_address_list_size = val;
    //(p_dst->target_enb_id ).enb_type = val;
    //rrm_memset_rrm_oam_cell_plmn_info( &((p_dst->target_enb_id).plmn_id), val);
    
    for( counter = RRM_ZERO; counter < HOME_ENB_ID_OCTET_SIZE; counter++)
    {
        (p_dst->target_enb_id).eNb_id[counter] = val;
    }
    for(counter = RRM_ZERO; counter < MAX_NO_ENB_X2_TLAS ; counter++)
    {
        rrm_memset_rrm_enb_tnl_address( &(p_dst->enb_tnl_address_list[counter]), val);
    }
}


/******************************************************************************
                        MEMCMP
    
******************************************************************************/
/*CODE OPT END*/
/* DYNAMIC ICIC CHANGES START   */
/*  alphaBasedPathLossToTargetSINRMap   */
void rrm_memcpy_alpha_pathloss_target_sinr_map(
               AlphaBasedPathLossToTargetSINRMap     *d_ptr ,
               rrm_alpha_based_pathloss_target_sinr_map_t   *s_ptr)
{
    U8 count = RRM_ZERO;
    U8 count2 = RRM_ZERO;
    d_ptr->defaultPathLoss = s_ptr->default_path_loss;

    for(count =  RRM_ZERO; count < RRMCM_RMIF_MAX_NUM_ALPHA_COUNT; count++)
    {
        d_ptr->pathLossToTargetSINRMap[count].count =
            s_ptr->path_loss_target_sinr_map[count].count;

        for( count2 = RRM_ZERO; (count2 < d_ptr->pathLossToTargetSINRMap[count].count) &&
                count2 < RRMCM_RMIF_MAX_NUM_PATHLOSS_TO_SINR_COUNT; count2++)
        {
            d_ptr->pathLossToTargetSINRMap[count].pathLossToTargetSINRMapInfo[count2].startPL =
                s_ptr->path_loss_target_sinr_map[count].path_loss_to_target_sinr_map_info[count2].start_PL ;

            d_ptr->pathLossToTargetSINRMap[count].pathLossToTargetSINRMapInfo[count2].endPL =
                s_ptr->path_loss_target_sinr_map[count].path_loss_to_target_sinr_map_info[count2].end_PL ;

            d_ptr->pathLossToTargetSINRMap[count].pathLossToTargetSINRMapInfo[count2].targetSINR =
                s_ptr->path_loss_target_sinr_map[count].path_loss_to_target_sinr_map_info[count2].target_SINR ;
        }
    }

}


    /* deltaSINRToTPCMapForPUSCH    */ 
void rrm_memcpy_rrm_delta_sinr_to_tpc_map_pusch(
                        DeltaSINRToTPCMapForPUSCH    *d_ptr,
                        rrm_delta_sinr_to_tpc_map_pusch     *s_ptr)
{
    U8 count = RRM_ZERO;
    d_ptr->count = s_ptr->count;

    for( count = RRM_ZERO; (count < s_ptr->count) && (count < RRMCM_RMIF_MAX_DELTA_SINR_TO_TCP_MAP_PUSCH); count++)
    {
        d_ptr->deltaSINRToTPCMapForPUSCHInfo[count].startDeltaSINR =
            s_ptr->delta_sinr_to_tpc_map_pusch_info[count].start_delta_sinr ; 

        d_ptr->deltaSINRToTPCMapForPUSCHInfo[count].endDeltaSINR =
            s_ptr->delta_sinr_to_tpc_map_pusch_info[count].end_delta_sinr ; 

        d_ptr->deltaSINRToTPCMapForPUSCHInfo[count].puschTPC =
            s_ptr->delta_sinr_to_tpc_map_pusch_info[count].pusch_tpc ; 

    }
}


/* deltaBlerToTPCMapForPUCCH */
void rrm_memcpy_bler_to_tpc_map_for_pucch(
                            DeltaBLERToTPCMapForPUCCH    *d_ptr,
                            rrm_bler_to_tpc_map_for_pucch_t     *s_ptr)
{
    U8 counter = RRM_ZERO;
    d_ptr->pucchBLERTarget = s_ptr->pucch_bler_target;
    
    d_ptr->count = s_ptr->count;

    for(counter = RRM_ZERO; (counter < s_ptr->count) && (counter < RRMCM_RMIF_MAX_NUM_TPC_FOR_PUCCH); counter++)
    {
        d_ptr->deltaBlerToTPCMapForPUCCHInfo[counter].startBLER =
            s_ptr->delta_bler_tpc_map_pucch_info[counter].start_bler ;
        
        d_ptr->deltaBlerToTPCMapForPUCCHInfo[counter].endBLER =
            s_ptr->delta_bler_tpc_map_pucch_info[counter].end_bler ;
        
        d_ptr->deltaBlerToTPCMapForPUCCHInfo[counter].pucchTPC =
            s_ptr->delta_bler_tpc_map_pucch_info[counter].pucch_tpc ;
    }
}

    /*  DeltaSINRToTPCMapForPUCCH   */
void rrm_memcpy_delta_sinr_tpc_map_pucch_info(
                                    DeltaSINRToTPCMapForPUCCH        *d_ptr,
                                    rrm_delta_sinr_to_tpc_map_pucch_info_t  *s_ptr)
{
    U8 counter = RRM_ZERO;

    d_ptr->pucchSINRTarget = s_ptr->pucch_sint_target;
    d_ptr->count = s_ptr->count;
    
    for(counter = RRM_ZERO; (counter < s_ptr->count) && (counter < RRMCM_RMIF_MAX_NUM_TPC_FOR_PUCCH ); counter++)
    {
        d_ptr->deltaSinrToTPCMapForPUCCHInfo[counter].startSINR = 
            s_ptr->delta_sinr_tpc_map_for_pucch_Info[counter].start_sinr ;
        
        d_ptr->deltaSinrToTPCMapForPUCCHInfo[counter].endSINR = 
            s_ptr->delta_sinr_tpc_map_for_pucch_Info[counter].end_sinr ;
        
        d_ptr->deltaSinrToTPCMapForPUCCHInfo[counter].pucchTPC = 
            s_ptr->delta_sinr_tpc_map_for_pucch_Info[counter].pucch_tpc ;
    }
}
/*  rrm_l2_CqiToPHICHPower  */
void rrm_memcpy_rrm_l2_cqi_to_phich_power(
                            CqiToPHICHPower      *d_ptr,
                            rrm_cqi_to_phich_power_t    *s_ptr)
{
    U8 count = RRM_ZERO;

    for(count = RRM_ZERO; count < RRMCM_RMIF_MAX_CQI_VAL; count++)
    {
        d_ptr->cqiToPHICHPowerInfo[count] = s_ptr->cqi_to_phich_power_info[count];
    }
}
/*  rrm_sps_dl_scheduling_Info_per_tti_t    */
void rrm_memcpy_rrm_sps_dl_scheduling_Info_per_tti(
                            SpsDlSchedulingInfoPerTti        *d_ptr,
                            rrm_sps_dl_scheduling_Info_per_tti_t    *s_ptr)
{
    U8 count = RRM_ZERO;

    d_ptr->maxDlSpsDciPerTti =  s_ptr->max_dl_sps_dci_per_tti;
    d_ptr->maxDlSpsOccasionPerTti = s_ptr->max_dl_sps_Occasion_Per_tti ;
    d_ptr->maxDlSpsRbsPerTti= s_ptr->max_dl_sps_rbs_per_tti ;
    
    for(count = RRM_ZERO; count < RRMCM_RMIF_MAX_SPS_INTERVALS; count++)
    {
        d_ptr->maxDlSpsRbsPerTtiPerInterval[count] = s_ptr->max_dl_sps_rbs_per_tti_per_interval[count] ;
    }    
}

/*  rrm_sps_ul_scheduling_Info_per_tti_t    */
void rrm_memcpy_rrm_sps_ul_scheduling_Info_per_tti(
                            SpsUlSchedulingInfoPerTti        *d_ptr,
                            rrm_sps_ul_scheduling_Info_per_tti_t    *s_ptr)
{
   U8   count = RRM_ZERO; 

   d_ptr->maxUlSpsDciPerTti = s_ptr->max_ul_sps_dci_per_tti ;
   d_ptr->maxUlSpsOccasionPerTti = s_ptr->max_ul_sps_occasion_per_tti ;
   d_ptr->maxUlSpsRbsPerTti = s_ptr->max_ul_sps_rbs_per_tti ;

   for(count = RRM_ZERO; count < RRMCM_RMIF_MAX_SPS_INTERVALS; count++)
   {
        d_ptr->maxUlSpsRbsPerTtiPerInterval[count] = s_ptr->max_ul_sps_rbs_tti_per_interval[count] ;
   }     
}

/*  rrm_dynamic_cfi_extension_params_t  */
void rrm_memcpy_rrm_dynamic_cfi_extension_params(
                        DynamicCFIExtensionParams    *d_ptr,
                        rrm_dynamic_cfi_extension_params_t  *s_ptr)
{
    U8 count = RRM_ZERO;

    for(count = RRM_ZERO; count < RRMCM_RMIF_MAX_POSSIBLE_AGGREGAITION_LEVELS; count++)
    {
        d_ptr->cceCorrectionStepUpFactor[count] = s_ptr->cce_correction_step_up_factor[count] ;
        d_ptr->cceCorrectionStepDownFactor[count] = s_ptr->cce_correction_step_down_factor[count] ;
        d_ptr->cceAdjustFactor[count] = s_ptr->cce_adjust_factor[count] ;
    }
}
/*  rrm_aggregation_power_offset_t  */
void rrm_memcpy_rrm_aggregation_power_offset(
                                    AggregationPowerOffset   *d_ptr,
                                    rrm_aggregation_power_offset_t  *s_ptr)
{
    U8 count = RRM_ZERO;
    
    d_ptr->count = s_ptr->count;

    for(count = RRM_ZERO; (count < d_ptr->count) &&
                          (count < RRMCM_RMIF_MAX_AGGREGATION_LEVEL_POWER_OFFSET_COUNT); count++)
    {
       d_ptr->aggregationPowerOffsetTuples[count].aggregationLevel = 
                                        s_ptr->aggr_pwr_offset_tuples[count].aggregation_level; 
       d_ptr->aggregationPowerOffsetTuples[count].powerOffset = 
                                        s_ptr->aggr_pwr_offset_tuples[count].power_offset; 
    }
}

void rrm_memcpy_oam_alpha_pathloss_target_sinr_map(
                        rrm_alpha_based_pathloss_target_sinr_map_t      *d_ptr,
                        rrm_oam_alpha_based_pathloss_target_sinr_map_t  *s_ptr)
{
    U8 count = RRM_ZERO;
    U8 count2 = RRM_ZERO;
    d_ptr->default_path_loss = s_ptr->default_path_loss;
    for(count =  RRM_ZERO; count < RRM_OAM_MAX_NUM_ALPHA_COUNT; count++)
    {
        d_ptr->path_loss_target_sinr_map[count].count = 
            s_ptr->path_loss_target_sinr_map[count].count;

        for( count2 = RRM_ZERO; (count2 < d_ptr->path_loss_target_sinr_map[count].count) &&
                                (count2 < RRM_OAM_MAX_NUM_PATHLOSS_TO_SINR_COUNT); count2++)
        {
            d_ptr->path_loss_target_sinr_map[count].path_loss_to_target_sinr_map_info[count2].start_PL =
                s_ptr->path_loss_target_sinr_map[count].path_loss_to_target_sinr_map_info[count2].start_PL ;

            d_ptr->path_loss_target_sinr_map[count].path_loss_to_target_sinr_map_info[count2].end_PL = 
                s_ptr->path_loss_target_sinr_map[count].path_loss_to_target_sinr_map_info[count2].end_PL ;

            d_ptr->path_loss_target_sinr_map[count].path_loss_to_target_sinr_map_info[count2].target_SINR =
                s_ptr->path_loss_target_sinr_map[count].path_loss_to_target_sinr_map_info[count2].target_SINR ;

        }
    }
}
/* SPR_10352_change_start */
  /* Function definition deleted: rrm_memcpy_mif_alpha_pathloss_target_sinr_map */
/* SPR_10352_change_end */

void rrm_memcpy_mif_resource_partition(
                        resource_partition_t            *d_ptr,
                        rrm_oam_resource_partition_t    *s_ptr)
{
    d_ptr->start_rb = s_ptr->start_rb;
    d_ptr->num_of_rb = s_ptr->num_of_rb;
    
}


/*health monitoring changes start */
/************************************************************************
 * Function Name  : rrm_oam_set_status_for_health
 * Inputs         : Thread ID
 *
 * Returns        : Void 
 * Description    : This function is called by thread to update its status
 *                  whenever it is active
 *************************************************************************/
rrm_void_t rrm_oam_set_status_for_health(rrm_thread_id_t thread_id)
{
    RRM_UT_TRACE_ENTER();
    struct timeval timestamp = {RRM_ZERO};
    RRM_GETTIMEOFDAY(&timestamp,RRM_PNULL);
    RRM_TRACE (g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
               "Marking status of thread[%u]",thread_id);
    g_thread_update_status[thread_id].tv_sec = timestamp.tv_sec ;
    
    RRM_UT_TRACE_EXIT();

}

/************************************************************************
 * Function Name  : rrm_oam_check_thread_status
 * Inputs         : no input
 *
 * Returns        : SUCCESS/FAILURE
 * Description    : This function checks the status of the g_thread_update_status
 *                  and returns SUCCESS if all enteries in array are one else
 *                  returns FAILURE
 *************************************************************************/
rrm_return_et rrm_oam_check_thread_status(U16 periodicity)
{
    RRM_UT_TRACE_ENTER();
    U8 index = RRM_ZERO;
    /* coverity  : CID 41538 */
    rrm_bool_et dead_thrd = RRM_FALSE;
    rrm_return_et ret_val = RRM_SUCCESS;
    struct timeval timestamp = {RRM_ZERO};
    RRM_GETTIMEOFDAY(&timestamp,RRM_PNULL);
    for(index = RRM_ZERO; index < RRM_MAX_THREADS ;index++)
    {  /* if entry the value of periodicity is greater than the timestamp value 
        then the thread is active*/
        if(periodicity < (timestamp.tv_sec - g_thread_update_status[index].tv_sec))
        {
            dead_thrd = RRM_TRUE;
            RRM_TRACE (g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                   "Thread[%u] is inactive", index);
        }
        g_thread_update_status[index].tv_sec = RRM_ZERO;
    }
    if(RRM_TRUE != dead_thrd)
    {
         RRM_TRACE (g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "All the threads are in active state ");
    }
    else
    {
        RRM_TRACE (g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                   "All threads are not in active state");
        ret_val = RRM_FAILURE;
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
                                                                                                                                                      
/*health monitoring changes end */

/****************************************************************************
* Function Name  : rrm_thread_start
* Inputs         : Thread argument need for the initialization of the thread
* Outputs        : None
* Returns        : Null
* Description    : Entry function of the thread
****************************************************************************/
rrm_void_t
*rrm_thread_start(void * thread_arg)
{
    SET_CELL_AND_UE_INDEX(0xff, 0xffff);

    /*Get the logical thread id pthread_self*/
    thrd_arg_t *p_thread_arg = (thrd_arg_t *)thread_arg;
    lgr_logger_register_thread(p_thread_arg->logical_thid);
   
    qvDriverLoop(p_thread_arg->driver_id, p_thread_arg->p_qwait);

    return RRM_PNULL;
}
/* SPR-9815-fix start */
/****************************************************************************
* Function Name  : rrm_encode_rncid_to_cell_identity
* Inputs         : cid id.
* Outputs        : Encode the cid id into the cell_identity.
* Returns        : Null
* Description    : This function prepares cell identity from rnc id.
****************************************************************************/
void rrm_encode_rncid_to_cell_identity(U8 *p_cell_identity,
        U16 rncid)
{
    U32 offset = RRM_ZERO;
    U32 msb_byte_mask = RRM_EXTENDED_RNCID_BITMASK;
    U32 msb_byte_offset = RRM_THREE;

    RRM_UT_TRACE_ENTER();

    if(!p_cell_identity) {
        RRM_TRACE(g_mif_log_on_off,p_rrm_mif_facility_name,RRM_ERROR,  "NULL POINTER ERROR!!!");
    }
    else
    {
        if(rncid > RRM_12BIT_MAX_VALUE) {
            p_cell_identity[offset] = (rncid & 0xF000) >> RRM_BYTE_SIZE_IN_BITS;
            p_cell_identity[offset] |= (rncid & 0xF00) >> RRM_BYTE_SIZE_IN_BITS;

            ++offset;
            p_cell_identity[offset] = rncid & 0xF0;
            p_cell_identity[offset] |= rncid & 0xF;

            msb_byte_mask >>= msb_byte_offset * RRM_BYTE_SIZE_IN_BITS;
            msb_byte_mask >>= RRM_NIBBLE_SIZE_IN_BITS;
            p_cell_identity[msb_byte_offset] |= msb_byte_mask;
        }
        else {
            p_cell_identity[offset] = (rncid & 0xF00) >> RRM_NIBBLE_SIZE_IN_BITS;
            p_cell_identity[offset] |= (rncid & 0xF0) >> RRM_NIBBLE_SIZE_IN_BITS;

            ++offset;
            p_cell_identity[offset] |= (rncid & 0xF) << RRM_NIBBLE_SIZE_IN_BITS;
        }
    }

    RRM_UT_TRACE_EXIT();
    return;
}
/*SPR 20908 Start*/
/****************************************************************************
* Function Name  : map_cell_id_to_instance_id
* Inputs         : Cell Index
* Outputs        : None
* Returns        : instance_id
* Description    : Returns the instance Id according to the cell index
****************************************************************************/
void
/*SPR 21096 Start*/
rrm_map_cell_id_to_ip_port(U8 cell_index,rrm_oam_l2_comm_info_t *p_l2_comm_info,U8 src_module_id)
{
/*SPR 21759 Changes start*/
	if(RRM_PNULL != p_g_rrm_cell_ctx->p_rrm_cell_ctx[cell_index])
	{
/*SPR 21759 Changes end*/
		switch(src_module_id)
		{

			case(RRM_L2_MODULE_ID):
				RRM_MEMCPY(p_l2_comm_info,&p_g_rrm_cell_ctx->p_rrm_cell_ctx[cell_index]->l2_comm_info_params.mac_comm_info,sizeof(rrm_oam_l2_comm_info_t));
				break;
			case(PDCP_MODULE_ID):
				RRM_MEMCPY(p_l2_comm_info,&p_g_rrm_cell_ctx->p_rrm_cell_ctx[cell_index]->l2_comm_info_params.pdcp_comm_info,sizeof(rrm_oam_l2_comm_info_t));
				break;
			default:
				RRM_TRACE(g_mif_log_on_off,p_rrm_mif_facility_name,RRM_INFO, "Invalid Instance ID = %d ",src_module_id);

		}
	}
}
/*SPR 21096 End*/
/*SPR 20908 End*/
/*CA STAGE 2*/

/****************************************************************************
* Function Name  : rrm_encode_cid_to_cell_identity
* Inputs         : cid id.
* Outputs        : Encode the cid id into the cell_identity.
* Returns        : Null
* Description    : This function prepares cell identity from cid id.
****************************************************************************/

void rrm_encode_cid_to_cell_identity(U8 *p_cell_identity,
        U16 cid)
{
    U32 offset = RRM_ONE;
    U32 msb_byte_mask = RRM_EXTENDED_RNCID_BITMASK;
    U32 msb_byte_offset = RRM_THREE;

    RRM_UT_TRACE_ENTER();

    if(!p_cell_identity) {
        RRM_TRACE(g_mif_log_on_off,p_rrm_mif_facility_name,RRM_ERROR, "NULL POINTER ERROR!!!");
    }
    else
    {

        msb_byte_mask >>= msb_byte_offset * RRM_BYTE_SIZE_IN_BITS;
        msb_byte_mask >>= RRM_NIBBLE_SIZE_IN_BITS;

        if(!(p_cell_identity[msb_byte_offset] & msb_byte_mask)) {
            p_cell_identity[offset] |= (cid & 0xF000) >> RRM_UMTS_CID_MIN_NUM_BITS;
            RRM_TRACE(g_mif_log_on_off,p_rrm_mif_facility_name,RRM_DETAILED, "Cell Identity[%d]: %d", offset,
                    p_cell_identity[offset]);
        }
        else {
            RRM_TRACE(g_mif_log_on_off,p_rrm_mif_facility_name,RRM_ERROR,  "Extended-RNCID bit is found set");
        }

        ++offset;
        p_cell_identity[offset] = (cid & 0xF00) >> RRM_NIBBLE_SIZE_IN_BITS;
        p_cell_identity[offset] |= (cid & 0xF0) >> RRM_NIBBLE_SIZE_IN_BITS;

        ++offset;
        p_cell_identity[offset] |= (cid & 0xF) << RRM_NIBBLE_SIZE_IN_BITS;
    }
    RRM_UT_TRACE_EXIT();
    return;
}
/* SPR-9815-fix end */
/* SPR-9346-fix start */
/****************************************************************************
 * Function Name  : is_big_endian
 * Inputs         : void
 * Outputs        : TRUE/FALSE
 * Returns        : TRUE/FALSE
 * Variables      : None
 * Description    : This function returns true if the m/c is a big endian m/c,
                    returns false if little endian
 ****************************************************************************/
/* + SPR 17439 */
rrm_return_et
is_big_endian(void)
/* - SPR 17439 */
{
    U16 word = 0x0001;
    U8 *byte = (U8 *) &word;
    if (byte[RRM_ZERO])
    {
        return RRM_FAILURE;
    }
    else
    {
        return RRM_SUCCESS;
    }
}
/* SPR-9346-fix end */
/* Fix for CSR 58972 start*/
/****************************************************************************
 * Function Name  : rrm_get_enb_type_from_access_mode
 * Inputs         : access_mode.
 * Outputs        : NULL
 * Returns        : eNodeB type.
 * Description    : This function is used for fetch the eNodeB type from access mode.
 ****************************************************************************/

U8 rrm_get_enb_type_from_access_mode (
        S32  cell_access_mode
        )
{

    rrm_oam_access_mode_et required_cell_access_mode = (rrm_oam_access_mode_et) cell_access_mode;
    U8 cell_type = MACRO_ENB_ID;

    RRM_UT_TRACE_ENTER();

    if (RRM_OAM_ACCESS_MODE_OPEN !=  required_cell_access_mode)
    {
        cell_type = HOME_ENB_ID;
    }

    RRM_UT_TRACE_EXIT();

    return cell_type;
}
/* Fix for CSR 58972 end*/
/****************************************************************************
 * Function Name  : rrm_plmnid_from_mcc_mnc 
 * Inputs         : p_out_plmnid
 *                : p_in_plmnid
 * Outputs        : rrm_return_et
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function is used for covert the PLMN id in string
 *                  format
 ****************************************************************************/
rrm_return_et
rrm_plmnid_from_mcc_mnc
(
    U8                          *p_out_plmn, 
    rrm_oam_cell_plmn_info_t    *p_input_plmn
)
{
    /* SPR_17367_Start */
    RRM_UT_TRACE_ENTER();
    rrm_oam_cell_plmn_info_t *input_plmn = p_input_plmn;


    p_out_plmn[RRM_ZERO] = (U8)((U8)(p_input_plmn->mcc[RRM_ONE] << RRM_FOUR)| p_input_plmn->mcc[RRM_ZERO]);
    p_out_plmn[RRM_ONE] = (U8)(p_input_plmn->mcc[RRM_TWO]);
    if(RRM_TWO == input_plmn->num_mnc_digit)
    {
        p_out_plmn[RRM_ONE] = (U8)((p_out_plmn[RRM_ONE])|RRM_HIGH_NIBBLE);
        p_out_plmn[RRM_TWO] = (U8)(((U8)(p_input_plmn->mnc[RRM_ONE] << RRM_FOUR))| p_input_plmn->mnc[RRM_ZERO]);
    }
    else
    {
        p_out_plmn[RRM_ONE] = ((U8)(p_input_plmn->mnc[RRM_ZERO] << RRM_FOUR)| p_out_plmn[RRM_ONE]);
        p_out_plmn[RRM_TWO] = (U8)(((U8)(p_input_plmn->mnc[RRM_TWO] << RRM_FOUR))| p_input_plmn->mnc[RRM_ONE]);
    }

    RRM_UT_TRACE_EXIT(); 
    return RRM_SUCCESS;
    /* SPR_17367_End */
}
/* SPR 16477 Fix Stop */

/* SPR_17763_FIX_START */
/****************************************************************************
 * Function Name  : rrm_is_newer_node_getting_inserted 
 * Inputs         : p_node_time  timestamp of node to be inserted
 *                : p_temp_node_time timestamp of node at head 
 * Outputs        :  
 * Returns        : RRM_ZERO/RRM_ONE
 * Description    : This function is used to check if new node getting added
 *                   or some previously created node getting enqueued again.
 ****************************************************************************/
U8 rrm_is_newer_node_getting_inserted(struct timeval *p_node_time, struct timeval *p_temp_node_time)
{
  U8 retVal = RRM_ZERO;
  RRM_UT_TRACE_ENTER();
 
  if (RRM_ZERO < (p_node_time->tv_sec - p_temp_node_time->tv_sec))
  {
      retVal = RRM_ONE;
  }
  else if (RRM_ZERO == (p_node_time->tv_sec - p_temp_node_time->tv_sec) &&
             (RRM_ZERO < (p_node_time->tv_usec - p_temp_node_time->tv_usec)))
  {          
      retVal = RRM_ONE;
  }
  RRM_UT_TRACE_EXIT(); 
  return retVal;
}
/* SPR_17763_FIX_END */

/*SPR 21419 Fix Start*/
/****************************************************************************
 * Function Name  : rrm_module_timer_start 
 * Inputs         : rrm_ue_context_t *p_g_rrm_ue_ctx RRM UE Context.
 *                : rrm_module_id_t  destination module id where timer is started
 * Outputs        : None
 * Returns        : QTIMER timer-id
 * Description    : To start timer from any task of destination module.
 ****************************************************************************/
/*coverity_115250 Start*/
QTIMER
rrm_module_timer_start(rrm_module_id_t module_1, QTIME *p_duration, void *p_timer_data, U16 timer_data_size, rrm_bool_et is_repeated, S32 *p_error)
/*coverity_115250 Stop*/
{
    void                            *p_timer_buf = RRM_PNULL;
    void                            *p_timerId = RRM_PNULL;
    /*COV_2NOV_START*/
    QMODULE                         service = RRM_PNULL;
    /*COV_2NOV_STOP*/

    RRM_UT_TRACE_ENTER();
    RRM_ASSERT (RRM_PNULL != p_timer_data);

    p_timer_buf  = rrm_mem_get(timer_data_size);
    if (RRM_PNULL == p_timer_buf)
    {     
        /*raising error event - TIMER_START_FAIL*/
        rrm_raise_error_event_notification(RRM_EE_TIMER_START_FAIL);
    }     
    else  
    {    
        RRM_MEMCPY(p_timer_buf, p_timer_data, timer_data_size);

        service = qvGetServiceEx(module_1,(int *)p_error);
        if((p_error != RRM_PNULL) && (QVERROR_NONE == *p_error) 
         /*COV_2NOV_START*/
         && (RRM_PNULL != service))
         /*COV_2NOV_STOP*/
        {
            p_timerId = qvModuleTimerStart(service, p_duration,p_timer_buf,is_repeated,(int *)p_error);
            if (RRM_PNULL == p_timerId)
            {
                /*raising error event - TIMER_START_FAIL*/
                rrm_raise_error_event_notification(RRM_EE_TIMER_START_FAIL);
                RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_ERROR,
                        "Starting Timer failed, Error received is %d:",*p_error);
                if(PNULL != p_timer_buf)
                    RRM_MEM_FREE(p_timer_buf);
            }
            else
            {
                RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_DETAILED,
                        "Started Timer: [%p]: duration sec [%u]",p_timerId);
            }
        }
        else
        {
            RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_ERROR,
                    "Cannot get service name: invalid argument passed or module allocation failed");
            if(PNULL != p_timer_buf)
                RRM_MEM_FREE(p_timer_buf);

        }
    }     
    RRM_UT_TRACE_EXIT();
    return p_timerId;
}

/****************************************************************************
 * Function Name  : rrm_module_timer_stop 
 * Inputs         : rrm_ue_context_t *p_g_rrm_ue_ctx RRM UE Context.
 *                : rrm_module_id_t  destination module id where timer is started
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : To stop timer from any task of destination module.
 ****************************************************************************/
rrm_return_et
rrm_module_timer_stop(QTIMER p_timerId, QTIME *time, S32 *Err)
{
    void               *p_timerData = RRM_PNULL;
    rrm_return_et      ret_val= RRM_SUCCESS;
    RRM_UT_TRACE_ENTER();

    if (RRM_PNULL != p_timerId)
    {
        if(qvTimerRunning(p_timerId))
        {     
            p_timerData = (void*)cell_get_timer_data(p_timerId);

            if(p_timerData != RRM_PNULL)
            {     
                RRM_MEM_FREE(p_timerData);
            }   
            ret_val = (rrm_return_et)qvModuleTimerStop(p_timerId,time,(int *)Err);
        }     
        else  
        {
            RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_ERROR,"timer already expired or not running!");
        }     
    }
    else  
    {
        RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_ERROR,"NULL timer passed!");
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : cell_get_timer_data
 * Inputs         : 
 *      1.p_timerId : Pointer to Timer Identifier.
 * Outputs        : 
 * Returns        : 
 * Variables      : 
 * Description    : This function stops the running timer.p_timerId is expected 
 *                  to be NULL as well as NON NULL, so no NULL check is done on it.
 ****************************************************************************/
void* cell_get_timer_data(QTIMER p_timerId)
{
	void    *p_timerData = RRM_PNULL;

	RRM_UT_TRACE_ENTER();

	p_timerData = qvTimerData(p_timerId);
       
	RRM_UT_TRACE_EXIT();

	return(p_timerData);
}

/*SPR 21419 Fix Stop*/
/* SPR 21446 start */
/****************************************************************************
 * Function Name  : rrm_get_ho_type_string 
 * Inputs         : handover type
 * Outputs        : None
 * Returns        : String value of the handover type received
 * Description    : This function convert the handover type into string for printing
 ****************************************************************************/
const char *rrm_get_ho_type_string(rrc_handover_type_et ho_type)
{
    switch (ho_type)
    {
        case HANDOVER_TYPE_INTRA_LTE_S1:
            return "INTRA_LTE_S1";

        case HANDOVER_TYPE_LTE_TO_UTRAN:
            return "LTE_TO_UTRAN";

        case HANDOVER_TYPE_LTE_TO_GERAN:
            return "LTE_TO_GERAN";

        case HANDOVER_TYPE_UTRAN_TO_LTE:
            return "UTRAN_TO_LTE";

        case HANDOVER_TYPE_GERAN_TO_LTE:
            return "GERAN_TO_LTE";

        case HANDOVER_TYPE_INTRA_LTE_X2:
            return "INTRA_LTE_X2";

        case HANDOVER_TYPE_INTRA_CELL:
            return "INTRA_CELL";

        case HANDOVER_TYPE_LTE_TO_CDMA2000_1XRTT:
            return "LTE_TO_CDMA2000_1XRTT";

        case HANDOVER_TYPE_LTE_TO_CDMA2000_HRPD:
            return "LTE_TO_CDMA2000_HRPD";

        case HANDOVER_TYPE_LTE_TO_CDMA2000_CONC_1XRTT_HRPD:
            return "LTE_TO_CDMA2000_CONC_1XRTT_HRPD";

        case HANDOVER_TYPE_CCO:
            return "CCO";

        case HANDOVER_TYPE_INTER_CELL:
            return "INTER_CELL";

        default:
            return "Invalid handover type";

    }
}

/****************************************************************************
 * Function Name  : rrm_get_ue_fsm_procedure_state_string 
 * Inputs         : Procedure state
 * Outputs        : None
 * Returns        : String value of the state received
 * Description    : This function convert the procedure state into string for printing
 ****************************************************************************/
const char *rrm_get_ue_fsm_procedure_state_string(rrm_ue_erb_states_et state)
{
	switch(state)
	{
		case RRM_UE_ERB_NULL:
			return "RRM_UE_ERB_NULL";

		case RRM_UE_ERB_SETUP_ONGOING:
			return "RRM_UE_ERB_SETUP_ONGOING";

		case RRM_UE_ERB_MODIFY_ONGOING:
			return "RRM_UE_ERB_MODIFY_ONGOING";

		case RRM_UE_HO_ADM_ONGOING:
			return "RRM_UE_HO_ADM_ONGOING";

		case RRM_UE_CONTEXT_MOD_ONGOING:
			return "RRM_UE_CONTEXT_MOD_ONGOING";

		case RRM_UE_RECONFIG_ONGOING:
			return "RRM_UE_RECONFIG_ONGOING";

		case RRM_UE_ICIC_UPDATE_INFO_ONGOING:
			return "RRM_UE_ICIC_UPDATE_INFO_ONGOING";


		case RRM_UE_LIPA_SIPTO_HO_ONGOING:
			return "RRM_UE_LIPA_SIPTO_HO_ONGOING";

		case RRM_UE_MEAS_CONFIG_REQ_ONGOING:
			return "RRM_UE_MEAS_CONFIG_REQ_ONGOING";

		case RRM_UE_MEAS_RESULTS_AWAITED:
			return "RRM_UE_MEAS_RESULTS_AWAITED";

		case RRM_UE_SCELL_PROCEDURE_ONGOING:
			return "RRM_UE_SCELL_PROCEDURE_ONGOING";
		case RRM_UE_RECONFIG_GOING_FOR_EICIC:
			return "RRM_UE_RECONFIG_GOING_FOR_EICIC";

		case RRM_UE_REESTABLISH_PROCEDURE_ONGOING:
			return "RRM_UE_REESTABLISH_PROCEDURE_ONGOING";

		case RRM_UE_SRC_HO_ONGOING:
			return "RRM_UE_SRC_HO_ONGOING";
/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
		case RRM_UE_DC_BEARER_CHANGE_PROCEDURE_ONGOING:
			return "RRM_UE_DC_BEARER_CHANGE_PROCEDURE_ONGOING";
#endif
/* OPTION3X Changes End */

		default:
			return "Invalid procedure";
	}

}

/****************************************************************************
 * Function Name  : rrm_map_meas_event 
 * Inputs         : Meas event
 * Outputs        : None
 * Returns        : String value of the measurement event
 * Description    : This function convert the measurement event into string 
 ****************************************************************************/
const char *rrm_map_meas_event(rrmc_meas_report_event_et meas_event)
{
	switch(meas_event)
	{
		case MEAS_REP_EVENT_A1:
			return "EVENT_A1";
		case MEAS_REP_EVENT_A2:
			return "EVENT_A2";
		case MEAS_REP_EVENT_A3:
			return "EVENT_A3";
		case MEAS_REP_EVENT_A4:
			return "EVENT_A4";
		case MEAS_REP_EVENT_A5:
			return "EVENT_A5";
		case MEAS_REP_EVENT_B1:
			return "EVENT_B1";
		case MEAS_REP_EVENT_B2:
			return "EVENT_B2";
		case MEAS_REP_EVENT_STRONGEST_CELL_FOR_SON:
			return "EVENT_STRONGEST_CELL_FOR_SON";
		case MEAS_REP_EVENT_FOR_CGI:
			return "EVENT_FOR_CGI";
		case MEAS_REP_EVENT_FORCEFUL_A2:
			return "EVENT_FORCEFUL_A2";
		case MEAS_REP_EVENT_PERIODICAL:
			return "EVENT_PERIODICAL";
#ifdef ENDC_ENABLED
		case MEAS_REP_EVENT_B1_NR:
			return "MEAS_REP_EVENT_B1_NR";
		case MEAS_REP_EVENT_B2_NR:
			return "MEAS_REP_EVENT_B2_NR";
#endif
		default:
			return "INVALID EVENT";
	}
}

/****************************************************************************
 * Function Name  : rrm_map_trigger_quantity 
 * Inputs         : Trigger quantity
 * Outputs        : None
 * Returns        : String value of the trigger quantity
 * Description    : This function convert the trigger quantity event into string 
 ****************************************************************************/
const char *rrm_map_trigger_quantity(rrm_trigger_quantity_et trig_quantity)
{
	switch (trig_quantity)
	{
		case RRM_RRC_RSRP:
			return "RSRP";

		case RRM_RRC_RSRQ:
			return "RSRQ";

		default:
			return "Invalid trigger quantity";
	}
}
/* SPR 21446 end */

void rrm_trace_dbg(
        U32 is_log_enable, const S8* facility_name, U8 log_level,
        const char* format, ...) 
{
    /* Empty Definition */
}

#ifdef LINUX_PC_TEST
void pc_test_forward_timer_msg(U32 moduldId,const void* pdata,U8 timeType)
{
    /* Empty Definition */
}
#endif

