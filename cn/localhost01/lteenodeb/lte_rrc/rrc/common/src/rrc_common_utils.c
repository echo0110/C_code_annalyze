/******************************************************************************
*
*   FILE NAME:
*       rrc_common_utils.c
*
*   DESCRIPTION:
*       This is the main file of the bridge module. This module provides
*       the bridge between RRC layer and external libraries (CSPL, logger etc).
*
*   DATE            AUTHOR      REFERENCE       REASON
*   31 Mar 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include <cspl.h>
#include <stacklayer.h>
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"
#include "rrc_ext_api.h"
#include "s1ap_api.h"
#include "x2ap_api.h"
#include "rrc_logging.h"
/* SPR 16017 Fix Start */ 
#include "uecc_logger.h"
/* SPR 16017 Fix Stop */ 


#ifdef RRC_UNIT_TEST_FRAMEWORK
#include "rrc_ut_stub_db.h"
#endif


/*SPR_19937_START*/
#include "rrc_oamh_intf.h"
#include "rrc_rrm_intf.h"
#include "rrc_uecc_llim_intf.h"
#include "rrc_x2ap_uecc_intf.h"
/*SPR_19937_END*/
U32 length_read_g = 0;
/* SPR 1643 Start */
#define MAX_VAL_DIGIT          0x09

#define IS_VALID_PLMN(x) \
   ((((x) & 0x0F) <= MAX_VAL_DIGIT) && (((x) >> 0x04) <= MAX_VAL_DIGIT))


#define IS_VALID_PLMN_FILLER(x) \
   ((((x) >> 0x04) <= MAX_VAL_DIGIT) || (((x) >> 0x04) == 0xF))

/* SPR 1643 Stop */

/*
*   Global variables
*/

/* Used in logger for identifying RRC in general
 *  (just for avoid string duplication) */
const S8* rrc_log_facility_name = (const S8*)"eNodeB RRC";
/*Lockless Implementation start*/
#ifdef LOCKLESS_IMPL
QLOCK  rrc_lock  = PNULL;
QLOCK  s1ap_lock = PNULL;
QLOCK  x2ap_lock = PNULL;
#else
extern QLOCK  rrc_lock;
extern QLOCK  s1ap_lock;
extern QLOCK  x2ap_lock;
#endif
/*Lockless Implementation stop*/

/*Bug 9305 start*/
U32 rrc_health_monitor_val[RRC_MAX_THREADS] = {0};
U32 s1ap_health_monitor_val[S1AP_SCTP_MAX_THREADS] = {0};
U32 x2ap_health_monitor_val = 0;
/*Bug 9305 end*/

#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    AdvL_ContextIndex g_advl_ctxt_rrc = RRC_LOG_CTXT_CONSOLE;
    AdvL_ContextIndex g_advl_ctxt_s1ap = S1AP_LOG_CTXT_CONSOLE;
    AdvL_ContextIndex g_advl_ctxt_x2ap = X2AP_LOG_CTXT_CONSOLE;

    AdvL_ContextIndex g_advl_ctxt_m2ap = X2AP_LOG_CTXT_CONSOLE;

#endif /* (LOGGER_TYPE == RRC_ADV_LOGGER) */

#if (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
/*
*   Internal logger
*/
#define FMT_MAX_LEN 255
#endif

/* Debugging Definitions */
#define UNKNOWN_MODULE_NAME "UNKNOWN_MODULE_ID"
#define UNKNOWN_API_NAME "UNKNOWN_API_ID"
#define INT_API_NAME ""

/* External Modules Identifiers Names */
const char* extModuleName[] =
{
    "RRC_OAM_MODULE_ID",
    "RRC_RRM_MODULE_ID",
    "RRC_MODULE_ID",
    "RRC_S1U_MODULE_ID",
    "RRC_PDCP_MODULE_ID",
    "RRC_RLC_MODULE_ID",
    "RRC_MAC_MODULE_ID",
    "RRC_PHY_MODULE_ID",
    "RRC_MME_MODULE_ID",
    "RRC_X2AP_PEER_eNODEB_MODULE_ID",
    "RRC_SON_MODULE_ID"
#ifdef LTE_EMBMS_SUPPORTED
    ,"RRC_MCE_MODULE_ID" 
#endif    
};

/* Internal Module Indentifies Names */
const char* intModuleName[] =
{
   "RRC_OAMH_MODULE_ID",
   "RRC_UECC_MODULE_ID",
   "RRC_CSC_MODULE_ID",
   "RRC_LLIM_MODULE_ID",
   "RRC_S1AP_MODULE_ID",
   "RRC_X2AP_MODULE_ID",
   "RRC_UECCMD_MODULE_ID",
   "RRC_UECC_1_MODULE_ID",
   "RRC_UECC_2_MODULE_ID",
   "RRC_UECC_3_MODULE_ID",
   "RRC_UECC_4_MODULE_ID",
   "RRC_UECC_5_MODULE_ID",
   "RRC_UECC_6_MODULE_ID",
   "RRC_UECC_7_MODULE_ID",
   "RRC_UECC_8_MODULE_ID",
   "RRC_S1U_1_MODULE_ID",
   "RRC_S1U_2_MODULE_ID",
   "RRC_S1U_3_MODULE_ID",
   "RRC_S1U_4_MODULE_ID",
   "RRC_S1U_5_MODULE_ID",
   "RRC_S1U_6_MODULE_ID",
   "RRC_S1U_7_MODULE_ID",
   "RRC_S1U_8_MODULE_ID",
   "RRC_PDCP_1_MODULE_ID",
   "RRC_PDCP_2_MODULE_ID",
   "RRC_PDCP_3_MODULE_ID",
   "RRC_PDCP_4_MODULE_ID",
   "RRC_PDCP_5_MODULE_ID",
   "RRC_PDCP_6_MODULE_ID",
   "RRC_PDCP_7_MODULE_ID",
   "RRC_PDCP_8_MODULE_ID",
   "RRC_RLC_1_MODULE_ID",
   "RRC_RLC_2_MODULE_ID",
   "RRC_RLC_3_MODULE_ID",
   "RRC_RLC_4_MODULE_ID",
   "RRC_RLC_5_MODULE_ID",
   "RRC_RLC_6_MODULE_ID",
   "RRC_RLC_7_MODULE_ID",
   "RRC_RLC_8_MODULE_ID",
   "RRC_MAC_1_MODULE_ID",
   "RRC_MAC_2_MODULE_ID",
   "RRC_MAC_3_MODULE_ID",
   "RRC_MAC_4_MODULE_ID",
   "RRC_MAC_5_MODULE_ID",
   "RRC_MAC_6_MODULE_ID",
   "RRC_MAC_7_MODULE_ID",
   "RRC_MAC_8_MODULE_ID",
   "RRC_PHY_1_MODULE_ID",
   "RRC_PHY_2_MODULE_ID",
   "RRC_PHY_3_MODULE_ID",
   "RRC_PHY_4_MODULE_ID",
   "RRC_PHY_5_MODULE_ID",
   "RRC_PHY_6_MODULE_ID",
   "RRC_PHY_7_MODULE_ID",
   "RRC_PHY_8_MODULE_ID",
   "RRC_LOGGER_MODULE_ID",
   "RRC_XWAP_MODULE_ID",
   "RRC_M2AP_MODULE_ID"
};

#define RRC_MAC_API_RANGE_OFFEST_1   (RRC_MAC_API_OFFSET_1_START - \
                                      RRC_MAC_API_BASE)

#define RRC_MAC_API_RANGE_OFFEST_2   (RRC_MAC_API_OFFSET_2_START - \
                                      RRC_MAC_API_OFFSET_1_END)

#define RRC_MAC_API_RANGE_OFFEST_3   (RRC_MAC_API_OFFSET_3_START - \
                                      RRC_MAC_API_OFFSET_2_END)


const char* rrc_mac_api_name[] = 
{
    /*RRC_MAC_API_OFFSET_1_START*/
    "RRC_MAC_CONFIG_CELL_REQ",
    "RRC_MAC_SFN_REQ",
    "RRC_MAC_RECONFIG_CELL_REQ",
    "RRC_MAC_CREATE_UE_ENTITY_REQ",
    "RRC_MAC_DELETE_UE_ENTITY_REQ",
    "RRC_MAC_RECONFIGURE_UE_ENTITY_REQ",
    "RRC_MAC_UE_DRX_CMD_REQ",
    "RRC_MAC_BCCH_CONFIG_REQ",
    "RRC_MAC_PCCH_MSG_REQ",
    "RRC_MAC_CCCH_MSG_REQ",
    "RRC_MAC_UE_CON_REJ_REQ",
    "RRC_MAC_DELETE_CELL_REQ",
    "RRC_MAC_HO_RACH_RESOURCE_REQ",
    "RRC_MAC_UE_INACTIVE_TIME_REQ_DELETED_API",
    "RRC_MAC_RESET_UE_ENTITY_REQ",
    "RRC_MAC_CHANGE_CRNTI_REQ",
    /*RRC_MAC_API_OFFSET_1_END*/

    /*RRC_MAC_API_OFFSET_2_START*/ 
    "RRC_MAC_CONFIG_CELL_CNF",
    "RRC_MAC_SFN_CNF",
    "RRC_MAC_RECONFIG_CELL_CNF",
    "RRC_MAC_CREATE_UE_ENTITY_CNF",
    "RRC_MAC_DELETE_UE_ENTITY_CNF",
    "RRC_MAC_RECONFIGURE_UE_ENTITY_CNF",
    "RRC_MAC_DELETE_CELL_CNF",
    "RRC_MAC_HO_RACH_RESOURCE_RESP",
    "RRC_MAC_UE_INACTIVE_TIME_RESP_DELETED_API",
    "RRC_MAC_RESET_UE_ENTITY_CNF",
    "RRC_MAC_CHANGE_CRNTI_CNF",
    /*RRC_MAC_API_OFFSET_2_END*/
    
    /*RRC_MAC_API_OFFSET_3_START*/ 
    "RRC_MAC_UE_ENTITY_POWER_HEADROOM_IND",
    "RRC_MAC_SFN_IND",
    "RRC_MAC_CCCH_MSG_IND",
    "RRC_MAC_HO_REL_RACH_RESOURCE_IND",
    "RRC_MAC_RADIO_LINK_FAILURE_IND",
    "RRC_MAC_INACTIVE_UES_IND_DELETED_API",
    "RRC_MAC_CELL_START_REQ",
    "RRC_MAC_CELL_START_CNF",
    "RRC_MAC_CELL_STOP_REQ",
    "RRC_MAC_CELL_STOP_CNF",
    "RRC_MAC_UE_SYNC_STATUS_IND",
    "RRC_MAC_RECONFIG_COMPLETE_IND",
    
    /*RRC_MAC_API_OFFSET_3_END*/    

    "RRC_M2AP_MAC_AREA_CONFIG_REQ",
    "RRC_M2AP_MAC_AREA_CONFIG_CNF",
    "RRC_M2AP_MAC_COUNTING_MSG",
    "RRC_M2AP_MAC_COUNTING_MSG_CNF",
    "RRC_M2AP_MAC_RESET_SESSION_REQ",
    "RRC_M2AP_MAC_RESET_SESSION_CNF",
    /* eMTC changes start */
    "RRC_MAC_EMTC_CCCH_MSG_IND"
    /* eMTC changes stop */
/* SPR 21660 changes start  */
    /*RRC_MAC_API_OFFSET_NB_START*/ 
    ,"RRC_MAC_CONFIG_NB_CELL_REQ",
    "RRC_MAC_CONFIG_NB_CELL_CNF",
    "RRC_MAC_CREATE_UE_ENTITY_NB_REQ",
    "RRC_MAC_CREATE_UE_ENTITY_NB_CNF",
    "RRC_MAC_CCCH_MSG_NB_IND",
    "RRC_MAC_DELETE_NB_UE_ENTITY_REQ",
    "RRC_MAC_DELETE_NB_UE_ENTITY_CNF"
    /*RRC_MAC_API_OFFSET_NB_END*/    
/* SPR 21660 changes end */
};

const char* rrc_rlc_api_name[] =
{
    "RRC_RLC_CREATE_UE_ENTITY_REQ",         
    "RRC_RLC_CREATE_UE_ENTITY_CNF",         
    "RRC_RLC_RECONFIG_UE_ENTITY_REQ",       
    "RRC_RLC_RECONFIG_UE_ENTITY_CNF",       
    "RRC_RLC_DELETE_UE_ENTITY_REQ",         
    "RRC_RLC_DELETE_UE_ENTITY_CNF",         
    "RRC_RLC_RE_ESTABLISH_UE_ENTITY_REQ",   
    "RRC_RLC_RE_ESTABLISH_UE_ENTITY_CNF",   
    "RRC_RLC_COMMON_CHANNEL_DATA_REQ",      
    "RRC_RLC_COMMON_CHANNEL_DATA_IND",     
    "RRC_RLC_TO_BE_ADDED ",  
    "RRC_RLC_UE_ENTITY_ERROR_IND",
    "RRC_RLC_CHANGE_CRNTI_REQ",
    "RRC_RLC_CHANGE_CRNTI_CNF",
    "RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND",
    "RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF",
    "RRC_RLC_CELL_CONFIG_REQ",
    "RRC_RLC_CELL_CONFIG_CNF",
    "RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_REQ",
    "RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_CNF",
    "RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_REQ",
    "RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_CNF",
    /*COV_START*/
    "RRC_M2AP_RLC_AREA_CONFIG_REQ",
    "RRC_M2AP_RLC_AREA_CONFIG_RESP",
    "RRC_M2AP_RLC_MCCH_IND"
    /*COV_STOP*/
};

const char* rrc_pdcp_api_name[] =
{
    "RRC_PDCP_CREATE_UE_ENTITY_CNF",         
    "RRC_PDCP_RECONFIG_UE_ENTITY_CNF",       
    "RRC_PDCP_DELETE_UE_ENTITY_CNF",         
    "RRC_PDCP_SRB_DATA_IND",                 
    "RRC_PDCP_SRB_DATA_STATUS_IND",          
    "RRC_PDCP_SN_HFN_STATUS_REQ",            
    "RRC_PDCP_SN_HFN_STATUS_RESP",           
    "RRC_PDCP_SN_HFN_STATUS_IND",            
    "RRC_PDCP_DATA_BUFFER_STOP_IND",         
    "RRC_PDCP_MAC_I_REQ",                    
    "RRC_PDCP_MAC_I_RESP",                   
    "RRC_PDCP_SUSPEND_UE_ENTITY_REQ",        
    "RRC_PDCP_SUSPEND_UE_ENTITY_CNF",
    "RRC_PDCP_RE_ESTABLISH_UE_ENTITY_REQ",
    "RRC_PDCP_RE_ESTABLISH_UE_ENTITY_CNF",
    "RRC_PDCP_RESUME_UE_ENTITY_REQ",
    "RRC_PDCP_RESUME_UE_ENTITY_CNF",
    "RRC_PDCP_CHANGE_CRNTI_REQ",
    "RRC_PDCP_CHANGE_CRNTI_CNF",
    "RRC_PDCP_COUNT_WRAPAROUND_IND",
    "RRC_PDCP_NOTIFY_INTEGRITY_FAILURE",   
    "RRC_PDCP_DRB_COUNT_MSB_REQ",
    "RRC_PDCP_DRB_COUNT_MSB_RESP",
    "RRC_PDCP_CREATE_UE_ENTITY_REQ",         
    "RRC_PDCP_RECONFIG_UE_ENTITY_REQ",       
    "RRC_PDCP_DELETE_UE_ENTITY_REQ",         
    "RRC_PDCP_SRB_DATA_REQ",     
    "RRC_PDCP_UPDATE_LWA_TRAFFIC_IND",
    "RRC_PDCP_CELL_CONFIG_REQ",
    "RRC_PDCP_CELL_CONFIG_CNF",
    /*SPR_19066_START*/
    "RRC_PDCP_HO_PREP_INFO_REQ",
    "RRC_PDCP_HO_PREP_INFO_RESP",
    "RRC_PDCP_INACTIVE_UES_IND"
    /*SPR_19066_END*/
};

const char* rrc_phy_api_name[] =
{
    "RRC_PHY_CONFIG_CELL_REQ",               
    "RRC_PHY_CONFIG_CELL_CNF",               
    "RRC_PHY_DELETE_CELL_REQ",               
    "RRC_PHY_DELETE_CELL_CNF",               
    "RRC_PHY_CREATE_UE_ENTITY_REQ",          
    "RRC_PHY_CREATE_UE_ENTITY_CNF",          
    "RRC_PHY_DELETE_UE_ENTITY_REQ",          
    "RRC_PHY_DELETE_UE_ENTITY_CNF",          
    "RRC_PHY_RECONFIG_UE_ENTITY_REQ",        
    "RRC_PHY_RECONFIG_UE_ENTITY_CNF",        
    "RRC_PHY_RECONFIG_CELL_REQ",             
    "RRC_PHY_RECONFIG_CELL_CNF",
    "RRC_PHY_CHANGE_CRNTI_REQ",
    "RRC_PHY_CHANGE_CRNTI_CNF",
    "RRC_PHY_CELL_START_REQ",
    "RRC_PHY_CELL_START_CNF",
    "RRC_PHY_CELL_STOP_REQ",
    "RRC_PHY_CELL_STOP_CNF"
};

const char* rrc_s1u_api_name[] = 
{
    "RRC_S1U_CREATE_UE_ENTITY_REQ",        
    "RRC_S1U_CREATE_UE_ENTITY_CNF",        
    "RRC_S1U_DELETE_UE_ENTITY_REQ",        
    "RRC_S1U_DELETE_UE_ENTITY_CNF",        
    "RRC_S1U_RECONFIGURE_UE_ENTITY_REQ",   
    "RRC_S1U_RECONFIGURE_UE_ENTITY_CNF",   
    "RRC_S1U_ERROR_IND",                   
    "RRC_S1U_PATH_FAILURE_IND",            
    "RRC_S1U_END_MARKER_IND",
    "RRC_S1U_PATH_SUCCESS_IND", 
    /*BUG 604 changes start */
    "RRC_S1U_INTRA_ENB_DATA_FWD_REQ",
    "RRC_S1U_INTRA_ENB_DATA_FWD_CNF",
    "RRC_S1U_LOCAL_PATH_SWITCH_REQ",
    "RRC_S1U_LOCAL_PATH_SWITCH_CNF", 
    /*BUG 604 changes stop */
    "RRC_S1U_CELL_CONFIG_REQ",
    "RRC_S1U_CELL_CONFIG_CNF"
};

const char* rrc_rrm_api_name[] = 
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
    "RRC_RRM_UE_CAPABILITY_ENQUIRY_REQ" ,    
    "RRC_RRM_UE_CAPABILITY_ENQUIRY_RESP",    
    "RRC_RRM_UE_CAPABILITY_IND",             
    /* Start: E-RAB Modify API */
    "RRC_RRM_ERB_MODIFY_REQ"   ,             
    "RRC_RRM_ERB_MODIFY_RESP"  ,             
    "RRC_RRM_ERB_MODIFY_CNF"  ,              
    /* End: E-RAB Modify API */
    /* ERB RELEASE COMMAND START */
    "RRC_RRM_ERB_RELEASE_CNF" ,              
    "RRC_RRM_UE_CONTEXT_MOD_REQ",              
    "RRC_RRM_UE_CONTEXT_MOD_RESP",              
    "RRC_RRM_ERB_RELEASE_IND",                  
    "RRC_RRM_UE_CONTEXT_MOD_CNF",             
    /*Handover Start*/
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
    /*RIM Start*/
    "RRC_RRM_RAN_RIM_INFORMATION_REQ",
    "RRC_RRM_RAN_RIM_INFORMATION",
    "RRC_RAN_INFO_REQ_RIM",
    "RRC_RAN_INFO_RIM",
    "RRC_RAN_ACK_RIM",
    "RRC_RAN_INFO_ERROR_RIM",
    "RRC_RAN_INFO_APP_ERROR_RIM",
    /*RIM end*/
    "RRC_RRM_COUNTER_CHECK_REQ",        
    "RRC_RRM_COUNTER_CHECK_RESP",        
    "RRC_RRM_UE_INFORMATION_REQ",
    "RRC_RRM_UE_INFORMATION_RESP",
    "RRC_RRM_DL_NAS_TRANSPORT_IND",   
    "RRC_RRM_INTER_FREQ_MEASUREMENT_IND",
    "RRC_RRM_UE_SYNC_STATUS",
    "RRC_RRM_LOCAL_ERROR_IND", /* lipa start */
                              /* lipa end */
    "RRC_RRM_UE_REESTAB_CONFIG_REQ",
    /* coverity fix 25345*/
    "RRC_RRM_UE_REESTAB_CONFIG_RESP",
    "RRC_RRM_UE_REESTAB_CONFIG_CNF",
    /* coverity fix 25345*/
    /* SPR 16053 Fix Start */
    "RRC_RRM_UE_REESTAB_COMPLETE_IND", 
    /* SPR 16053 Fix Stop */
    /* coverity_fix_25345_start */
    "RRC_RRM_UE_CONTEXT_MOD_IND"
    /* coverity_fix_25345_stop */
/* OPTION3X Change Start */
    ,"RRC_RRM_ERB_RECONFIG_REQ"
    ,"RRC_RRM_ERB_RECONFIG_RESP"
#ifdef ENDC_ENABLED
    ,"RRC_RRM_DC_BEARER_CHANGE_REQ"
#endif
/* OPTION3X Change End */
};


const char* rrc_oam_api_name[] =
{
    "RRC_OAM_INIT_IND",                      
    "RRC_OAM_INIT_CNF",                      
    "RRC_OAM_COMMUNICATION_INFO_REQ",        
    "RRC_OAM_COMMUNICATION_INFO_RESP",       
    "RRC_OAM_PROVISION_REQ",                 
    "RRC_OAM_PROVISION_RESP",                
    "RRC_OAM_S1AP_INFO_REQ",                 
    "RRC_OAM_S1AP_INFO_RESP",                
    "RRC_OAM_SET_LOG_LEVEL_REQ",             
    "RRC_OAM_SET_LOG_LEVEL_RESP",            
    "RRC_OAM_LOG_ENABLE_REQ",                
    "RRC_OAM_LOG_ENABLE_RESP",               
    "RRC_OAM_GET_CELL_STATS_REQ",            
    "RRC_OAM_GET_CELL_STATS_RESP",           
    "RRC_OAM_GET_CELL_STATUS_REQ",           
    "RRC_OAM_GET_CELL_STATUS_RESP",          
    "RRC_OAM_GET_UE_STATUS_REQ",             
    "RRC_OAM_GET_UE_STATUS_RESP",            
    "RRC_OAM_RESET_CELL_STATS_REQ",          
    "RRC_OAM_RESET_CELL_STATS_RESP",         
    "RRC_OAM_CLEANUP_REQ",                   
    "RRC_OAM_CLEANUP_RESP",
    "RRC_OAM_CELL_TRAFFIC_TRACE_START",
    "RRC_OAM_CELL_TRAFFIC_TRACE_STOP",
    "RRC_OAM_CELL_TRAFFIC_TRACE_START_ERROR_IND",
    "RRC_OAM_TRACE_FILE_TRANSFER_START_IND",
    "RRC_OAM_TRACE_FILE_TRANSFER_COMPLETE_IND",
    "RRC_OAM_TRACE_START_ERROR_IND",
    "RRC_OAM_CONFIG_STATS_REQ",
    "RRC_OAM_CONFIG_STATS_RESP",
    "RRC_OAM_RRC_CONNECTION_STATS_IND",
    "RRC_OAM_ERAB_STATS_IND",
    "RRC_OAM_HANDOVER_STATS_IND",
    "RRC_OAM_PAGING_STATS_IND",
    "RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND",
    "RRC_OAM_IRAT_MOBILITY_STATS_IND",
    "RRC_OAM_PROC_SUPP_REQ",
    "RRC_OAM_PROC_SUPP_RESP",
    "RRC_OAM_ADD_LGW_REQ", /* lipa start */
    "RRC_OAM_ADD_LGW_RESP",
    "RRC_OAM_DELETE_LGW_REQ",
    "RRC_OAM_DELETE_LGW_RESP", /* lipa end */
    "RRC_OAM_GET_LOG_LEVEL_REQ", /* SPR 13115 Start */
    "RRC_OAM_GET_LOG_LEVEL_RESP" /*SPR 13115 Stop */
    
};

const char* s1ap_oam_api_name[] = 
{
    "S1AP_OAM_INIT_IND",                      
    "S1AP_OAM_INIT_CNF",                      
    "S1AP_OAM_PROVISION_REQ",                 
    "S1AP_OAM_PROVISION_RESP",                
    "S1AP_OAM_RESET_REQ",                     
    "S1AP_OAM_RESET_RESP",                    
    "S1AP_OAM_CLEANUP_REQ",                   
    "S1AP_OAM_CLEANUP_RESP",                  
/* CSR 00058587 Fix Start */
    "S1AP_OAM_STATUS_REQ",                     
    "S1AP_OAM_STATUS_RESP",
/* CSR 00058587 Fix Stop */
    "S1AP_OAM_ENB_CONFIG_UPDATE",
    "S1AP_OAM_ENB_CONFIG_UPDATE_RESPONSE",
    "S1AP_OAM_CLOSE_MME_CONN",
    "S1AP_OAM_REESTAB_MME_CONN",
    "S1AP_OAM_S1AP_LINK_STATUS_IND",
    "S1AP_ADD_MME_REQ",
    "S1AP_ADD_MME_RES",
    "S1AP_OAM_MME_CONN_INFO_RESPONSE",
    "S1AP_OAM_MME_INFO",
    "S1AP_OAM_GET_SCTP_STATUS_REQ",
    "S1AP_OAM_GET_SCTP_STATUS_RESP",
    "S1AP_OAM_PROC_SUPP_REQ",
    "S1AP_OAM_PROC_SUPP_RESP",
    "S1AP_OAM_GET_LOG_LEVEL_REQ", 
    "S1AP_OAM_GET_LOG_LEVEL_RESP" 
};

/*x2ap_oam_api_name*/
const char* x2ap_oam_api_name[] =
{
    "X2AP_OAM_INIT_IND", 
    "X2AP_OAM_INIT_CNF",             
    "X2AP_OAM_PROVISION_REQ",
    "X2AP_OAM_PROVISION_RESP",         
    "X2AP_OAM_CLEANUP_REQ",    
    "X2AP_OAM_CLEANUP_RESP",           
    "X2AP_OAM_LINK_DOWN_IND",          
    "X2AP_OAM_LINK_UP_IND",      
    "X2AP_OAM_LINK_SETUP_REQ",
    "X2AP_OAM_LINK_SETUP_RESP",        
    "X2AP_ADD_ENB_REQ",
    "X2AP_ADD_ENB_RES",
    "X2AP_DEL_ENB_REQ",
    "X2AP_DEL_ENB_RES",
    "X2AP_OAM_GET_SCTP_STATUS_REQ",
    "X2AP_OAM_GET_SCTP_STATUS_RESP",
    "X2AP_OAM_PROC_SUPP_REQ",
    "X2AP_OAM_PROC_SUPP_RESP",
    /* SPR-16334 START */
    "X2AP_ENB_CONFIG_UPDATE_REQ",
    "X2AP_ENB_CONFIG_UPDATE_RES",
    /* SPR-16334 END */
    "X2AP_OAM_GET_LOG_LEVEL_REQ", 
    "X2AP_OAM_GET_LOG_LEVEL_RESP" 
};

/*xwap_oam_api_name*/
const char* xwap_oam_api_name[] =
{
    "XWAP_OAM_INIT_IND", 
	"XWAP_OAM_INIT_CNF",
	"XWAP_OAM_PROVISION_REQ",
	"XWAP_OAM_PROVISION_RESP",
	"XWAP_OAM_ADD_WT_REQ",
	"XWAP_OAM_ADD_WT_RES",
	"XWAP_OAM_XWAP_LINK_STATUS_IND",
	"XWAP_OAM_SET_LOG_LEVEL_REQ",
	"XWAP_OAM_SET_LOG_LEVEL_RESP",
	"XWAP_OAM_GET_LOG_LEVEL_REQ",
	"XWAP_OAM_GET_LOG_LEVEL_RESP",
	"XWAP_OAM_LOG_ENABLE_REQ",
	"XWAP_OAM_LOG_ENABLE_RESP",
	"XWAP_OAM_LOG_DISABLE_REQ",
	"XWAP_OAM_STATUS_REQ",
	"XWAP_OAM_STATUS_RESP",
	"XWAP_OAM_GET_SCTP_STATUS_REQ",
	"XWAP_OAM_GET_SCTP_STATUS_RESP",
	"XWAP_OAM_RESET_RESP",
	"XWAP_OAM_WT_INFO"

};

/*xwap_rrm_api_name*/
const char* xwap_rrm_api_name[] =
{
    "XWAP_RRM_WT_INFO"           
};


/*x2ap_rrm_api_name*/
const char* x2ap_rrm_api_name[] =
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
    "X2AP_ENB_LINK_DOWN_IND",
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

/* Internal APIs */

const char* csc_llim_api_name[] = 
{
    "CSC_LLIM_CONFIG_CELL_REQ",                      
    "CSC_LLIM_CONFIG_CELL_RESP",                      
    "CSC_LLIM_DELETE_CELL_REQ",                 
    "CSC_LLIM_DELETE_CELL_RESP",                
    "CSC_LLIM_SFN_IND",                     
    "CSC_LLIM_BCCH_CONFIG_REQ",                    
    "CSC_LLIM_PCCH_DATA_REQ",                   
    "CSC_LLIM_SFN_REQ",                  
    "CSC_LLIM_SFN_RESP",                     
    "CSC_LLIM_RECONFIG_CELL_REQ",
    "CSC_LLIM_RECONFIG_CELL_RESP",
    "CSC_LLIM_PWS_REQ",
    "CSC_LLIM_PWS_RESP",
    "CSC_LLIM_UPDATED_PWS_SI_REQ",
    "CSC_LLIM_UPDATED_PWS_SI_RESP",
    "CSC_LLIM_CELL_START_REQ",
    "CSC_LLIM_CELL_START_RESP",
    "CSC_LLIM_CELL_STOP_REQ",
    "CSC_LLIM_CELL_STOP_RESP"

};

const char* uecc_llim_api_name[] = 
{
    "UECC_LLIM_CREATE_UE_ENTITY_REQ",                      
    "UECC_LLIM_CREATE_UE_ENTITY_RESP",                      
    "UECC_LLIM_DELETE_UE_ENTITY_REQ",                 
    "UECC_LLIM_DELETE_UE_ENTITY_RESP",                
    "UECC_LLIM_ADD_LC_REQ",                     
    "UECC_LLIM_ADD_LC_RESP",                    
    "UECC_LLIM_SRB_DATA_REQ",                     
    "UECC_LLIM_SRB_DATA_IND",
    "UECC_LLIM_SRB_DATA_STATUS_IND",
    "UECC_LLIM_CCCH_DATA_REQ",
    "UECC_LLIM_CCCH_DATA_IND",
    "UECC_LLIM_UE_CON_REJ_REQ",
    "UECC_LLIM_CONFIGURE_SECURITY_REQ",
    "UECC_LLIM_CONFIGURE_SECURITY_RESP",
    "UECC_LLIM_S1U_ERROR_IND",
    "UECC_LLIM_S1U_PATH_FAILURE_IND",
    "UECC_LLIM_MODIFY_LC_REQ",
    "UECC_LLIM_MODIFY_LC_RESP",
    "UECC_LLIM_ADD_ROLLBACK_REQ",
    "UECC_LLIM_ADD_ROLLBACK_RESP",
    "UECC_LLIM_MODIFY_ROLLBACK_REQ",
    "UECC_LLIM_MODIFY_ROLLBACK_RESP",
    /* SPR 15644 fix start */
    "UECC_LLIM_RADIO_LINK_FAILURE_IND",
    /* SPR 15644 fix end */
    "UECC_LLIM_DELETE_LC_REQ",
    "UECC_LLIM_DELETE_LC_RESP",
    "UECC_LLIM_SUSPEND_UE_ENTITY_REQ",
    "UECC_LLIM_SUSPEND_UE_ENTITY_RESP",
    "UECC_LLIM_SN_HFN_STATUS_REQ",
    "UECC_LLIM_SN_HFN_STATUS_RESP",
    "UECC_LLIM_DATA_BUFFER_STOP_IND",
    "UECC_LLIM_SN_HFN_STATUS_IND",
    "UECC_LLIM_MAC_I_REQ",
    "UECC_LLIM_MAC_I_RESP",
    "UECC_LLIM_S1U_RECONFIGURE_REQ",
    "UECC_LLIM_S1U_RECONFIGURE_CNF",
    "UECC_LLIM_HO_RACH_RESOURCE_REQ",
    "UECC_LLIM_HO_RACH_RESOURCE_RESP",
    "UECC_LLIM_HO_REL_RACH_RESOURCE_IND",
    "UECC_LLIM_UE_INACTIVE_TIME_REQ",
    "UECC_LLIM_UE_INACTIVE_TIME_RESP",
    "UECC_LLIM_RE_ESTABLISH_UE_ENTITY_REQ",
    "UECC_LLIM_S1U_END_MARKER_IND",
    "UECC_LLIM_RE_ESTABLISH_UE_ENTITY_RESP",
    "UECC_LLIM_RESUME_UE_ENTITY_REQ",
    "UECC_LLIM_RESUME_UE_ENTITY_RESP",
    "UECC_LLIM_RECONFIG_SECURITY_REQ",
    "UECC_LLIM_RECONFIG_SECURITY_RESP",
    "UECC_LLIM_UE_CON_RE_ESTABLISH_REJ_REQ",
    "UECC_LLIM_REESTABLISH_UE_ENTITY_REQ",
    "UECC_LLIM_CHANGE_CRNTI_REQ",
    "UECC_LLIM_CHANGE_CRNTI_RESP",
    "UECC_LLIM_MEAS_CONFIG_REQ",
    "UECC_LLIM_MEAS_CONFIG_RESP",
    "UECC_LLIM_INACTIVE_UES_IND",
    "UECC_LLIM_S1U_PATH_SUCCESS_IND",
    "UECC_LLIM_COUNT_WRAPAROUND_IND",
    "UECC_LLIM_RESET_UE_ENTITY_REQ",
    "UECC_LLIM_RESET_UE_ENTITY_CNF",
    "UECC_LLIM_NOTIFY_INTEGRITY_FAILURE",   
    "UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND",  
    "UECC_LLIM_DRB_COUNT_MSB_REQ",
    "UECC_LLIM_DRB_COUNT_MSB_RESP",
    "UECC_LLIM_MAC_RECONFIG_COMPLETE_IND",
    "UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF",
    /*BUG 604 changes start*/
    "UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_REQ",
    "UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_CNF",
    "UECC_LLIM_S1U_LOCAL_PATH_SWITCH_REQ",
    "UECC_LLIM_S1U_LOCAL_PATH_SWITCH_CNF",				/* klocwork fix */
    /*BUG 604 changes stop*/
    "UECC_LLIM_PDCP_RE_ESTABLISH_UE_ENTITY_REQ",
    "UECC_LLIM_PDCP_RE_ESTABLISH_UE_ENTITY_RESP"
};

const char* oamh_api_name[] = 
{
    "UECC_OAMH_INIT_IND",
    "UECC_OAMH_INIT_CNF",
    "UECC_OAMH_PROV_REQ",
    "UECC_OAMH_PROV_RESP",
    "CSC_OAMH_INIT_IND",
    "CSC_OAMH_INIT_CNF",
    "CSC_OAMH_PROV_REQ",
    "CSC_OAMH_PROV_RESP",
    "LLIM_OAMH_INIT_IND",
    "LLIM_OAMH_INIT_CNF",
    "LLIM_OAMH_PROV_REQ",
    "LLIM_OAMH_PROV_RESP",
    "UECC_OAMH_UNUSED_1",
    "UECC_OAMH_UNUSED_2",
    "UECC_OAMH_GET_CELL_STATS_REQ",
    "UECC_OAMH_GET_CELL_STATS_RESP",
    "UECC_OAMH_GET_CELL_STATUS_REQ",
    "UECC_OAMH_GET_CELL_STATUS_RESP",
    "UECC_OAMH_GET_UE_STATUS_REQ",
    "UECC_OAMH_GET_UE_STATUS_RESP",
    "UECC_OAMH_RESET_CELL_STATS_REQ",
    "UECC_OAMH_RESET_CELL_STATS_RESP",
    "UECC_OAMH_CLEANUP_REQ",
    "UECC_OAMH_CLEANUP_RESP",
    "CSC_OAMH_CLEANUP_REQ",
    "CSC_OAMH_CLEANUP_RESP",
    "LLIM_OAMH_CLEANUP_REQ",
    "LLIM_OAMH_CLEANUP_RESP",
    "UECC_OAMH_CELL_TRAFFIC_TRACE_START",
    "UECC_OAMH_CELL_TRAFFIC_TRACE_STOP",
    "UECC_OAMH_CELL_TRAFFIC_TRACE_START_ERROR_IND",
    "UECC_OAMH_TRACE_FILE_TRANSFER_START_IND",
    "UECC_OAMH_TRACE_FILE_TRANSFER_COMPLETE_IND",
    "UECC_OAMH_TRACE_START_ERROR_IND",
    "CSC_OAMH_GET_CELL_STATS_REQ",
    "CSC_OAMH_GET_CELL_STATS_RESP",
    "CSC_OAMH_RESET_CELL_STATS_REQ",
    "CSC_OAMH_RESET_CELL_STATS_RESP",
    "CSC_OAMH_CELL_START_IND",
    "CSC_OAMH_CELL_STOP_IND",
    "UECCMD_OAMH_INIT_IND",
    "UECCMD_OAMH_INIT_CNF",
    "UECCMD_OAMH_PROV_REQ",
    "UECCMD_OAMH_PROV_RESP",
    "UECCMD_OAMH_CLEANUP_REQ",
    "UECCMD_OAMH_CLEANUP_RESP",
    "UECCMD_OAMH_ADD_DEL_LGW_IND", /* lipa start */  /* lipa end */
    /* SPR 20908 Start */
    /*CID 113178 Start*/
    "CSC_OAMH_OAM_COMMUNICATION_INFO_IND",
    /*CID 113178 End*/
    "CSC_OAMH_OAM_COMMUNICATION_INFO_RESP"
    /* SPR 20908 End */
};


const char* csc_uecc_api_name[] = 
{
    "CSC_UECC_INIT_SETUP_IND",
    "CSC_UECC_DELETE_ALL_UE_REQ",
    "UECC_CSC_DELETE_ALL_UE_RESP",
    "CSC_UECC_CELL_RECONFIG_IND",
    "CSC_UECC_GET_CELL_STATS_REQ",
    "CSC_UECC_GET_CELL_STATS_RESP",
    "CSC_UECC_RESET_CELL_STATS_REQ",
    "CSC_UECC_RESET_CELL_STATS_RESP",
    "CSC_UECC_CELL_START_IND",
    "CSC_UECC_CELL_STOP_IND"
};

const char* s1ap_uecc_api_name[] = 
{
    "S1AP_STATUS_MSG_IND",
    "S1AP_UE_ASSOCIATED_SIG_MSG_REQ",
    "S1AP_UE_ASSOCIATED_SIG_MSG_IND",
    "S1AP_RESET_MSG_IND",
    "S1AP_UE_RELEASE_MSG_REQ",
    "S1AP_FAILURE_MSG_REQ",
    "S1AP_DEL_CONTEXT_MSG_IND",
    "S1AP_OVERLOAD_MSG_IND",
    "S1AP_UE_ASSOCIATED_MME_UPDATE_STATUS_IND",
    "S1AP_UE_RELEASE_IND",
    "S1AP_UE_REL_REQ_INT",
    "S1AP_HO_ONGOING_UE_RELEASE_IND",
    "S1AP_UECCMD_UECC_INFO",
    "S1AP_ALLOCATE_MME_REQ",
    "S1AP_ALLOCATE_MME_RESP",
/*BUG 604 changes start*/
    "S1AP_INTRA_CELL_HO_SUCCESS_IND",
    "S1AP_ALLOCATE_S1AP_UEID_REQ",
    "S1AP_ALLOCATE_S1AP_UEID_RESP"
/*BUG 604 changes stop*/
};

/* xwap_uecc_api_name */
const char* xwap_uecc_api_name[] = 
{
	"XWAP_STATUS_MSG_IND",
	"XWAP_ALLOCATE_WT_REQ",
	"XWAP_UE_ASSOCIATED_SIG_MSG_REQ",
	"XWAP_UE_RELEASE_IND",
	"XWAP_UE_ASSOCIATED_SIG_MSG_IND",
	"XWAP_ALLOCATE_WT_RESP"
	
};

/* x2ap_uecc_api_name */
const char* x2ap_uecc_api_name[] = 
{
    "X2AP_STATUS_MSG_IND",           
    "X2AP_UE_ASSOCIATED_SIG_MSG_REQ",
    "X2AP_UE_ASSOCIATED_SIG_MSG_IND",
    "X2AP_UE_RELEASE_IND",
    "X2AP_RESET_MSG_IND",
    "X2AP_UE_REESTABLISHMENT_CONN_SETUP_RECONFIGURE",
    "X2AP_UE_RLF_MSG_IND",
    /* CSR_00040799 Fix Start */
    "X2AP_UE_RLF_RESP",
    /* Bug 4691 Changes Start */
    "X2AP_UECCMD_UE_INFO",
    "X2AP_UPDATE_NBOUR_INFO"
#ifdef ENDC_ENABLED
    ,"X2AP_UPDATE_NR_NBOUR_INFO"
#endif
    /* Bug 4691 Changes End */
    /* CSR_00040799 Fix End */
};

const char* s1ap_csc_api_name[] = 
{
    "S1AP_PAGING_IND",
    "S1AP_CELL_REG_REQ",
    "S1AP_CELL_DEREG_REQ",
    "S1AP_PWS_REQ",
    "S1AP_PWS_RESP",
    "S1AP_CELL_AREA_INFO_UPDATE_IND",
    "S1AP_PWS_FAILURE_REQ",
    "S1AP_PWS_FAILURE_RESP",
    "S1AP_KILL_REQ",
    "S1AP_KILL_RESP",
    "S1AP_KILL_FAILURE_IND"
};

const char* s1ap_rrm_api_name[] = 
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
    "RRC_RRM_RAN_INFO_APP_ERROR",
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

/* x2ap_csc_api_name */
const char* x2ap_csc_api_name[] =
{
    "X2AP_CSC_CELL_ADD_IND",
    "X2AP_CSC_CELL_DEL_IND",
    "X2AP_CSC_CELL_UPD_IND",
    "X2AP_CSC_CELL_STATUS_REQ",
    "X2AP_CSC_CELL_STATUS_RESP"
};

/* x2ap_sctp_api_name */
const char* x2ap_sctp_api_name[] =
{
    "X2AP_ENB_MESSAGE",
    "X2AP_SCTP_CONN_FAILURE_IND",
    "X2AP_SCTP_CONN_RESTART_IND",
    "X2AP_SCTP_CLOSED_IND",
    "X2AP_SCTP_COMM_UP_IND",
    "X2AP_SCTP_PEND_CONN_IND",
    "X2AP_SCTP_SHUT_DOWN_INIT_IND"
#ifdef LINUX_PC_TEST
    ,"X2AP_ENB_MESSAGE_PS_TO_TEST"
#endif
};

/* ueccmd_uecc_api_name */
const char* ueccmd_uecc_api_name[] =
{
    "UECCMD_UE_CONTEXT_RELEASE_ONGOING_IND",
    "UECCMD_UECC_PROC_STATUS_IND", 
    /*SPR_17727_START*/
    /* lipa start */
    "UECC_OAMH_ADD_DEL_LGW_IND",
    /* lipa end*/
    "UECCMD_UE_CON_RE_ESTABLISH_ONGOING"
    /*SPR_17727_END*/
};

/* x2ap_s1ap_api_name */
const char* x2ap_s1ap_api_name[] =
{
    "X2AP_S1AP_COMM_INFO"
};

/* Internal APIs */

/* Private Functions */
const char* rrc_get_module_name(rrc_module_id_t); 
static const char* rrc_get_api_name(U16,  rrc_module_id_t srcModuleId, rrc_module_id_t dstModuleId); 
/*SPR_19937_START*/
static U16 rrc_get_ue_index_from_msg(U16 api_id, U8 *p_api);

#define GET_MODULE_NAME(moduleId) rrc_get_module_name(moduleId)
#define GET_API_NAME(apiId, srcModuleId, dstModuleId) rrc_get_api_name(apiId, srcModuleId, dstModuleId)
#define GET_UE_INDEX_FROM_MSG(apiId, p_msg) rrc_get_ue_index_from_msg(apiId, p_msg)
/*SPR_19937_END*/

/******************************************************************************
*   FUNCTION NAME: rrc_get_module_name 
*   INPUT        : rrc_module_id_t  moduleId
*   OUTPUT       : None
*   RETURNS      : pointer to char 
*   DESCRIPTION  : This Function returns module name
******************************************************************************/
const char* rrc_get_module_name(rrc_module_id_t  moduleId) 
{
    if((moduleId >= RRC_MIN_EXT_MODULE_ID) && 
            (moduleId<= RRC_MAX_EXT_MODULE_ID)) 
        return ((char*)(extModuleName[moduleId - RRC_MIN_EXT_MODULE_ID]));  
    else if((moduleId >= RRC_MIN_INT_MODULE_ID) && 
            (moduleId <= RRC_MAX_INT_MODULE_ID)) 
        return ((char*)(intModuleName[moduleId - RRC_MIN_INT_MODULE_ID]));
    else if((RRC_SON_MODULE_ID == moduleId))
    {
#if   LTE_EMBMS_SUPPORTED
		/* Coverity ID 92897 starts */
		return ((char*)(extModuleName[RRC_MAX_EXT_MODULE_ID - RRC_MIN_EXT_MODULE_ID])); 
		/* Coverity ID 92897 ends */
#else
        return ((char*)(extModuleName[RRC_MAX_EXT_MODULE_ID])); 
#endif
     }
    else 
        return UNKNOWN_MODULE_NAME;
}

#ifdef LINUX_PC_TEST
const char* LogLevelName[] =
{
    "Emergency",
    "Alert",
    "Fatal",
    "Error",
    "Warning",
    "Notice",
    "Info",
    "Brief",
    "Detailed",
    "Debug",
    "Trace" 
};

const char* rrc_get_loglevel_name(U8 loglevel)
{
	if (loglevel > 10)
		return "unkown loglevel";
	else
		return LogLevelName[loglevel];
};

void stub_rrc_log_print(int loglevel,char fmt,...)
{

	va_list ap;
	char bufContent[4096];
	va_start(ap, fmt);
	vsprintf(bufContent, fmt, ap);
	va_end(ap);
	
	 test_module_log(rrc_get_module_name(RRC_MODULE_ID),rrc_get_loglevel_name((U8)loglevel), GET_CELL_INDEX(), GET_UE_INDEX(),bufContent );
	 
}
#endif
/*SPR_19937_START*/
/******************************************************************************
*   FUNCTION NAME: rrc_get_ue_index_from_msg
*   INPUT        : U16 apiId
*                  U8 *p_api
*   OUTPUT       : None
*   RETURNS      : U16
*   DESCRIPTION  : This Function returns UE Index
******************************************************************************/
static U16 rrc_get_ue_index_from_msg(U16 api_id, U8 *p_api)
{
    U16 ue_index = 0xFFFF;

    switch(api_id)
    {
        case RRC_RRM_UE_RELEASE_REQ:
        ue_index = ((rrc_rrm_ue_release_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_RELEASE_RESP:
        ue_index = ((rrc_rrm_ue_release_resp_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_ADMISSION_REQ:
        ue_index = ((rrc_rrm_ue_admission_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_ADMISSION_RESP:
        ue_index = ((rrc_rrm_ue_admission_resp_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_ADMISSION_CNF:
        ue_index = ((rrc_rrm_ue_admission_cnf_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_ERB_SETUP_REQ:
        ue_index = ((rrc_rrm_erb_setup_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_ERB_SETUP_RESP:
        ue_index = ((rrc_rrm_erb_setup_resp_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_ERB_SETUP_CNF:
        ue_index = ((rrc_rrm_erb_setup_cnf_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_ERB_RELEASE_REQ:
        ue_index = ((rrc_rrm_erb_release_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_ERB_RELEASE_RESP:
        ue_index = ((rrc_rrm_erb_release_resp_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_CONNECTION_RELEASE_IND:
        ue_index = ((rrc_rrm_ue_connection_release_ind_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_MEASURMENT_RESULTS_IND:
        ue_index = ((rrc_rrm_measurment_results_ind_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_CAPABILITY_ENQUIRY_REQ:
        ue_index = ((rrc_rrm_ue_capability_enquiry_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_Index;
        break;
        case RRC_RRM_UE_CAPABILITY_ENQUIRY_RESP:
        ue_index = ((rrc_rrm_ue_capability_enquiry_resp_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_Index;
        break;
        case RRC_RRM_UE_CAPABILITY_IND:
        ue_index = ((rrc_rrm_ue_capability_ind_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_Index;
        break;
        case RRC_RRM_ERB_MODIFY_REQ:
        ue_index = ((rrc_rrm_erab_modify_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_ERB_MODIFY_RESP:
        ue_index = ((rrc_rrm_erab_modify_resp_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_ERB_MODIFY_CNF:
        ue_index = ((rrc_rrm_erb_modify_cnf_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_ERB_RELEASE_CNF:
        ue_index = ((rrc_rrm_erb_release_cnf_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_CONTEXT_MOD_REQ:
        ue_index = ((rrc_rrm_ue_contxt_mod_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_CONTEXT_MOD_RESP:
        ue_index = ((rrc_rrm_ue_contxt_mod_resp_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_ERB_RELEASE_IND:
        ue_index = ((rrc_rrm_erb_release_ind_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_CONTEXT_MOD_CNF:
        ue_index = ((rrc_rrm_ue_contxt_mod_cnf_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_HO_REQUIRED:
        ue_index = ((rrc_rrm_ho_required_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_HO_ADM_REQ:
        ue_index = ((rrc_rrm_ue_ho_adm_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_HO_ADM_RESP:
        ue_index = ((rrc_rrm_ue_ho_adm_resp_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_HO_ADM_CNF:
        ue_index = ((rrc_rrm_ue_ho_adm_cnf_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_HO_RESTRICTION_LIST_IND:
        ue_index = ((rrc_rrm_ho_restriction_list_ind_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_HO_CMD_REQ:
        ue_index = ((rrc_rrm_ue_ho_command_request_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_HO_CMD_RESP:
        ue_index = ((rrc_rrm_ue_ho_command_response_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_MEAS_CONFIG_REQ:
        ue_index = ((rrc_rrm_meas_config_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_MEAS_CONFIG_RESP:
        ue_index = ((rrc_rrm_meas_config_response_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_HO_FAILURE:
        ue_index = ((rrc_rrm_ho_failure_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_HO_CANCEL_REQ:
        ue_index = ((rrc_rrm_ho_cancel_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_HO_CANCEL_RESP:
        ue_index = ((rrc_rrm_ho_cancel_resp_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_PROXIMITY_IND:
        ue_index = ((rrc_rrm_proximity_ind_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_INTRA_ENB_HO_IND:
        ue_index = ((rrc_rrm_intra_enb_ho_ind_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_RECONFIG_REQ:
        ue_index = ((rrc_rrm_ue_reconfig_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_RECONFIG_RESP:
        ue_index = ((rrc_rrm_ue_reconfig_resp_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_CSFB_PARAMETERS_REQ_CDMA2000:
        ue_index = ((rrc_rrm_csfb_parameters_req_cdma2000_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_CSFB_PARAMETERS_RESP_CDMA2000:
        ue_index = ((rrc_rrm_csfb_parameters_resp_cdma2000_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_COUNTER_CHECK_REQ:
        ue_index = ((rrc_rrm_counter_check_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_COUNTER_CHECK_RESP:
        ue_index = ((rrc_rrm_counter_check_resp_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_INFORMATION_REQ:
        ue_index = ((rrc_rrm_ue_information_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_Index;
        break;
        case RRC_RRM_UE_INFORMATION_RESP:
        ue_index = ((rrc_rrm_ue_information_resp_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_Index;
        break;
        case RRC_RRM_DL_NAS_TRANSPORT_IND:
        ue_index = ((rrc_rrm_dl_nas_transport_ind_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_INTER_FREQ_MEASUREMENT_IND:
        ue_index = ((rrc_rrm_inter_freq_rstd_measurement_ind_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_SYNC_STATUS:
        ue_index = ((rrc_rrm_ue_sync_status_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case RRC_RRM_UE_REESTAB_CONFIG_REQ:
        ue_index = ((rrc_rrm_ue_reestab_config_req_t *)(p_api+RRC_INTERFACE_API_HEADER_SIZE))->ue_index;
        break;
        case UECC_LLIM_CREATE_UE_ENTITY_REQ:
        ue_index = ((rrc_uecc_llim_create_ue_entity_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_CREATE_UE_ENTITY_RESP:
        ue_index = ((rrc_uecc_llim_create_ue_entity_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_DELETE_UE_ENTITY_REQ:
        ue_index = ((rrc_uecc_llim_delete_ue_entity_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_DELETE_UE_ENTITY_RESP:
        ue_index = ((rrc_uecc_llim_delete_ue_entity_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_ADD_LC_REQ:
        ue_index = ((rrc_uecc_llim_add_lc_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_ADD_LC_RESP:
        ue_index = ((rrc_uecc_llim_add_lc_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_SRB_DATA_REQ:
        ue_index = ((rrc_uecc_llim_srb_data_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_SRB_DATA_IND:
        ue_index = ((rrc_uecc_llim_srb_data_ind_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_SRB_DATA_STATUS_IND:
        ue_index = ((rrc_uecc_llim_srb_data_status_ind_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_CONFIGURE_SECURITY_REQ:
        ue_index = ((rrc_uecc_llim_configure_security_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_CONFIGURE_SECURITY_RESP:
        ue_index = ((rrc_uecc_llim_configure_security_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_S1U_ERROR_IND:
        ue_index = ((rrc_uecc_llim_s1u_error_ind_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_MODIFY_LC_REQ:
        ue_index = ((rrc_uecc_llim_modify_lc_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_MODIFY_LC_RESP:
        ue_index = ((rrc_uecc_llim_modify_lc_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_ADD_ROLLBACK_REQ:
        ue_index = ((rrc_uecc_llim_add_rollback_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_ADD_ROLLBACK_RESP:
        ue_index = ((rrc_uecc_llim_add_rollback_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_MODIFY_ROLLBACK_REQ:
        ue_index = ((rrc_uecc_llim_modify_rollback_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_MODIFY_ROLLBACK_RESP:
        ue_index = ((rrc_uecc_llim_modify_rollback_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_RADIO_LINK_FAILURE_IND:
        ue_index = ((rrc_uecc_llim_radio_link_failure_ind_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_DELETE_LC_REQ:
        ue_index = ((rrc_uecc_llim_delete_lc_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_DELETE_LC_RESP:
        ue_index = ((rrc_uecc_llim_delete_lc_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_SUSPEND_UE_ENTITY_REQ:
        ue_index = ((rrc_uecc_llim_suspend_ue_entity_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_SUSPEND_UE_ENTITY_RESP:
        ue_index = ((rrc_uecc_llim_suspend_ue_entity_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_SN_HFN_STATUS_REQ:
        ue_index = ((rrc_uecc_llim_sn_hfn_status_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_SN_HFN_STATUS_RESP:
        ue_index = ((rrc_uecc_llim_sn_hfn_status_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_DATA_BUFFER_STOP_IND:
        ue_index = ((rrc_uecc_llim_data_buffer_stop_ind_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_SN_HFN_STATUS_IND:
        ue_index = ((rrc_uecc_llim_sn_hfn_status_ind_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_MAC_I_REQ:
        ue_index = ((rrc_uecc_llim_mac_i_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_MAC_I_RESP:
        ue_index = ((rrc_uecc_llim_mac_i_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_S1U_RECONFIGURE_REQ:
        ue_index = ((rrc_uecc_llim_s1u_reconfigure_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_S1U_RECONFIGURE_CNF:
        ue_index = ((rrc_uecc_llim_s1u_reconfigure_cnf_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_HO_RACH_RESOURCE_REQ:
        ue_index = ((rrc_uecc_llim_ho_rach_resource_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_HO_RACH_RESOURCE_RESP:
        ue_index = ((rrc_uecc_llim_ho_rach_resource_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_HO_REL_RACH_RESOURCE_IND:
        ue_index = ((rrc_uecc_llim_ho_rach_rel_resource_ind_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_UE_INACTIVE_TIME_REQ:
        ue_index = ((rrc_uecc_llim_ue_inactive_time_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_UE_INACTIVE_TIME_RESP:
        ue_index = ((rrc_uecc_llim_ue_inactive_time_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_RE_ESTABLISH_UE_ENTITY_REQ:
        ue_index = ((rrc_uecc_llim_re_establish_ue_entity_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_S1U_END_MARKER_IND:
        ue_index = ((rrc_uecc_llim_s1u_end_marker_ind_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_RE_ESTABLISH_UE_ENTITY_RESP:
        ue_index = ((rrc_uecc_llim_re_establish_ue_entity_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_RESUME_UE_ENTITY_REQ:
        ue_index = ((rrc_uecc_llim_resume_ue_entity_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_RESUME_UE_ENTITY_RESP:
        ue_index = ((rrc_uecc_llim_resume_ue_entity_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_RECONFIG_SECURITY_REQ:
        ue_index = ((rrc_uecc_llim_reconfig_security_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_RECONFIG_SECURITY_RESP:
        ue_index = ((rrc_uecc_llim_reconfig_security_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_REESTABLISH_UE_ENTITY_REQ:
        ue_index = ((rrc_uecc_llim_re_establish_ue_entity_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_CHANGE_CRNTI_REQ:
        ue_index = ((rrc_uecc_llim_change_crnti_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_CHANGE_CRNTI_RESP:
        ue_index = ((rrc_uecc_llim_change_crnti_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_MEAS_CONFIG_REQ:
        ue_index = ((rrc_uecc_llim_meas_config_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_MEAS_CONFIG_RESP:
        ue_index = ((rrc_uecc_llim_meas_config_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_COUNT_WRAPAROUND_IND:
        ue_index = ((rrc_uecc_llim_count_wraparound_ind_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_RESET_UE_ENTITY_REQ:
        ue_index = ((rrc_uecc_llim_reset_ue_entity_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_RESET_UE_ENTITY_CNF:
        ue_index = ((rrc_uecc_llim_reset_ue_entity_cnf_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_NOTIFY_INTEGRITY_FAILURE:
        ue_index = ((rrc_uecc_llim_notify_integrity_failure_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND:
        ue_index = ((rrc_uecc_llim_re_establish_ue_entity_complete_ind_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_DRB_COUNT_MSB_REQ:
        ue_index = ((rrc_uecc_llim_drb_count_msb_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_DRB_COUNT_MSB_RESP:
        ue_index = ((rrc_uecc_llim_drb_count_msb_resp_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_MAC_RECONFIG_COMPLETE_IND:
        ue_index = ((rrc_uecc_llim_mac_reconfig_complete_ind_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF:
        ue_index = ((rrc_uecc_llim_re_establish_ue_entity_complete_cnf_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_REQ:
        ue_index = ((rrc_uecc_llim_s1u_intra_enb_data_fwd_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_CNF:
        ue_index = ((rrc_uecc_llim_s1u_intra_enb_data_fwd_cnf_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_S1U_LOCAL_PATH_SWITCH_REQ:
        ue_index = ((rrc_uecc_llim_s1u_local_path_switch_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_S1U_LOCAL_PATH_SWITCH_CNF:
        ue_index = ((rrc_uecc_llim_s1u_local_path_switch_cnf_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_PDCP_RE_ESTABLISH_UE_ENTITY_REQ:
        ue_index = ((rrc_uecc_llim_re_establish_ue_entity_req_t *)p_api)->ue_index;
        break;
        case UECC_LLIM_PDCP_RE_ESTABLISH_UE_ENTITY_RESP:
        ue_index = ((rrc_uecc_llim_re_establish_ue_entity_resp_t *)p_api)->ue_index;
        break;
        case UECC_OAMH_GET_UE_STATUS_REQ:
        ue_index = ((rrc_uecc_oamh_get_ue_status_req_t *)p_api)->ue_index;
        break;
        case S1AP_UE_ASSOCIATED_SIG_MSG_REQ:
        ue_index = ((s1ap_ue_associated_sig_msg_req_t *)p_api)->enb_ue_s1ap_id;
        break;
        case S1AP_UE_ASSOCIATED_SIG_MSG_IND:
        ue_index = ((s1ap_ue_associated_sig_msg_ind_t *)p_api)->enb_ue_s1ap_id;
        break;
        case S1AP_UE_RELEASE_MSG_REQ:
        ue_index = ((s1ap_ue_release_msg_req_t *)p_api)->enb_ue_s1ap_id;
        break;
        case S1AP_DEL_CONTEXT_MSG_IND:
        ue_index = ((s1ap_del_ctx_msg_ind_t *)p_api)->ue_index;
        break;
        case S1AP_UE_ASSOCIATED_MME_UPDATE_STATUS_IND:
        ue_index = ((s1ap_ue_associated_mme_update_status_ind_t *)p_api)->enb_ue_s1ap_id;
        break;
        case S1AP_UE_RELEASE_IND:
        ue_index = ((s1ap_ue_release_ind_t *)p_api)->enb_ue_s1ap_id;
        break;
        case S1AP_UE_REL_REQ_INT:
        ue_index = ((s1ap_ue_rel_req_int_t *)p_api)->ue_index;
        break;
        case S1AP_ALLOCATE_MME_REQ:
        ue_index = ((s1ap_allocate_mme_req_t *)p_api)->ue_index;
        break;
        case S1AP_ALLOCATE_MME_RESP:
        ue_index = ((s1ap_allocate_mme_resp_t *)p_api)->ue_index;
        break;
        case S1AP_INTRA_CELL_HO_SUCCESS_IND:
        ue_index = ((s1ap_intra_cell_ho_success_ind_t *)p_api)->ue_index;
        break;
        case S1AP_ALLOCATE_S1AP_UEID_REQ:
        ue_index = ((s1ap_allocate_s1ap_ueid_req_t *)p_api)->ue_index;
        break;
        case S1AP_ALLOCATE_S1AP_UEID_RESP:
        ue_index = ((s1ap_allocate_s1ap_ueid_resp_t *)p_api)->ue_index;
        break;
        case X2AP_UE_ASSOCIATED_SIG_MSG_REQ:
        ue_index = ((x2ap_ue_associated_sig_msg_req_t *)p_api)->ue_id;
        break;
        case X2AP_UE_ASSOCIATED_SIG_MSG_IND:
        ue_index = ((x2ap_ue_associated_sig_msg_ind_t *)p_api)->enb_ue_s1ap_id;
        break;
        case X2AP_UE_RELEASE_IND:
        ue_index = ((x2ap_ue_release_ind_t *)p_api)->ue_id;
        break;
        case X2AP_UE_RLF_RESP:
        ue_index = ((x2ap_ue_rlf_resp_t *)p_api)->ue_index;
        break;
        default: ue_index = 0xFFFF;
        break;
    }
    return ue_index;
}
/*SPR_19937_END*/

/******************************************************************************
*   FUNCTION NAME: rrc_get_api_name 
*   INPUT        : U16 apiId
*                  rrc_module_id_t srcModuleId
*                  rrc_module_id_t dstModuleId
*   OUTPUT       : None
*   RETURNS      : pointer to char 
*   DESCRIPTION  : This Function returns api name
******************************************************************************/
static const char* rrc_get_api_name(U16 apiId, rrc_module_id_t srcModuleId, rrc_module_id_t dstModuleId) 
{
    /* MAC INTF */
   /*SPR 15543 Fix Start*/
    if((((dstModuleId == RRC_MAC_MODULE_ID) || (dstModuleId >= RRC_MAC_1_MODULE_ID  && dstModuleId <= RRC_MAC_8_MODULE_ID)) ||((srcModuleId == RRC_MAC_MODULE_ID) || (srcModuleId >=     RRC_MAC_1_MODULE_ID && srcModuleId <= RRC_MAC_8_MODULE_ID)))&& 
       ((apiId > RRC_MAC_API_BASE) && (apiId <= RRC_MAC_MAX_API)))
   /*SPR 15543 Fix Stop*/
    {
        if((apiId >= RRC_MAC_API_OFFSET_1_START ) && 
           (apiId <= RRC_MAC_API_OFFSET_1_END))
            return ((char*)(rrc_mac_api_name[apiId - RRC_MAC_API_RANGE_OFFEST_1])); 
    
        if((apiId >= RRC_MAC_API_OFFSET_2_START ) && 
           (apiId <= RRC_MAC_API_OFFSET_2_END))
            return ((char*)(rrc_mac_api_name[apiId - (RRC_MAC_API_RANGE_OFFEST_2
                + RRC_MAC_API_RANGE_OFFEST_1 - 1)])); 

        if((apiId >=RRC_MAC_API_OFFSET_3_START ) && 
           (apiId <= RRC_MAC_API_OFFSET_3_END))
            return ((char*)(rrc_mac_api_name[apiId - (RRC_MAC_API_RANGE_OFFEST_3
                + RRC_MAC_API_RANGE_OFFEST_2 + RRC_MAC_API_RANGE_OFFEST_1 - 2)]));
    }

        
    /* RLC INTF */
   /*SPR 15543 Fix Start*/
    if((((dstModuleId == RRC_RLC_MODULE_ID) || (dstModuleId >=RRC_RLC_1_MODULE_ID  && dstModuleId <= RRC_RLC_8_MODULE_ID ))||((srcModuleId == RRC_RLC_MODULE_ID) || (srcModuleId >= RRC_RLC_1_MODULE_ID  && srcModuleId <= RRC_RLC_8_MODULE_ID)))&& 
       ((apiId > RRC_RLC_API_BASE) && (apiId <= RRC_RLC_MAX_API)))
   /*SPR 15543 Fix Stop*/
        return ((char*)(rrc_rlc_api_name[apiId -RRC_RLC_API_BASE -1])); 
   
    /* PDCP INTF */
    /* Since API IDs for PDCP interface start from 0 , so not decremented */ 
   /*SPR 15543 Fix Start*/
    if((((dstModuleId == RRC_PDCP_MODULE_ID) || (dstModuleId >=RRC_PDCP_1_MODULE_ID  && dstModuleId <= RRC_PDCP_8_MODULE_ID))||((srcModuleId == RRC_PDCP_MODULE_ID) || (srcModuleId >= RRC_PDCP_1_MODULE_ID  && srcModuleId <= RRC_PDCP_8_MODULE_ID )))&&
        (apiId <= RRC_PDCP_MAX_API))
   /*SPR 15543 Fix Stop*/
    {
	/* Coverity 21836 Fix Start */ 
            return rrc_pdcp_api_name[apiId - RRC_PDCP_API_BASE];
	/* Coverity 21836 Fix Stop */ 
    }
    
    /* PHY INTF */
   /*SPR 15543 Fix Start*/
    if((((dstModuleId == RRC_PHY_MODULE_ID) || (dstModuleId >= RRC_PHY_1_MODULE_ID  && dstModuleId <= RRC_PHY_8_MODULE_ID))||((srcModuleId == RRC_PHY_MODULE_ID) || (srcModuleId >= RRC_PHY_1_MODULE_ID && srcModuleId <= RRC_PHY_8_MODULE_ID)) )&&
       ((apiId > RRC_PHY_API_BASE) && (apiId <= RRC_PHY_MAX_API)))
   /*SPR 15543 Fix Stop*/
            return ((char*)(rrc_phy_api_name[apiId - RRC_PHY_API_BASE - 1])); 
    /* S1U INTF */
   /*SPR 15543 Fix Start*/
    if(((dstModuleId == RRC_S1U_MODULE_ID || (dstModuleId >= RRC_S1U_1_MODULE_ID && dstModuleId <= RRC_S1U_8_MODULE_ID))||((srcModuleId == RRC_S1U_MODULE_ID )|| (srcModuleId >= RRC_S1U_1_MODULE_ID  && srcModuleId <= RRC_S1U_8_MODULE_ID)) )&&
       ((apiId > RRC_S1U_API_BASE) && (apiId <= RRC_S1U_MAX_API)))
   /*SPR 15543 Fix Stop*/
        return ((char*)(rrc_s1u_api_name[apiId -RRC_S1U_API_BASE -1])); 

    /* RRM INTF */
    if(((dstModuleId == RRC_RRM_MODULE_ID)||(srcModuleId == RRC_RRM_MODULE_ID))&&
       ((apiId > RRC_RRM_API_BASE) && (apiId <= RRC_RRM_MAX_API)))
        return ((char*)(rrc_rrm_api_name[apiId -RRC_RRM_API_BASE -1])); 
    /* RRC-OAM INTF */
    if(((dstModuleId == RRC_OAM_MODULE_ID)||(srcModuleId == RRC_OAM_MODULE_ID))&&
       ((apiId > RRC_OAM_API_BASE) && (apiId <= RRC_OAM_MAX_API)))
        return ((char*)(rrc_oam_api_name[apiId -RRC_OAM_API_BASE -1])); 

    /* S1AP-OAM INTF */
    if(((dstModuleId == RRC_OAM_MODULE_ID)||(srcModuleId == RRC_OAM_MODULE_ID)) &&
       ((apiId > S1AP_OAM_API_BASE) && (apiId <= S1AP_OAM_PROC_SUPP_RESP)))
        return ((char*)(s1ap_oam_api_name[apiId -S1AP_OAM_API_BASE -1])); 

    /* x2ap_oam */
    if((((dstModuleId == RRC_OAM_MODULE_ID) ||(srcModuleId == RRC_OAM_MODULE_ID)) ||
      ((dstModuleId == RRC_SON_MODULE_ID) ||(srcModuleId == RRC_SON_MODULE_ID))) &&
       ((apiId > X2AP_OAM_API_BASE) && (apiId <= X2AP_OAM_PROC_SUPP_RESP)))
        return ((char*)(x2ap_oam_api_name[apiId -X2AP_OAM_API_BASE -1]));
    /* xwap_oam */
    /* x2ap_rrm */
    if((((dstModuleId == RRC_RRM_MODULE_ID) ||(srcModuleId == RRC_RRM_MODULE_ID))||
       ((dstModuleId == RRC_SON_MODULE_ID) ||(srcModuleId == RRC_SON_MODULE_ID))) &&
       ((apiId > X2AP_RRM_IF_API_BASE) && (apiId <= X2AP_ENDC_CONFIG_UPDATE_WAIT_IND)))
        return ((char*)(x2ap_rrm_api_name[apiId -X2AP_RRM_IF_API_BASE -1]));

    /* Internal APIs */

    /* CSC_LLIM_API */
    if((((srcModuleId == RRC_CSC_MODULE_ID) && 
                    (dstModuleId == RRC_LLIM_MODULE_ID)) || 
       ((srcModuleId == RRC_LLIM_MODULE_ID) && 
                    (dstModuleId == RRC_CSC_MODULE_ID))) &&     
       ((apiId > CSC_LLIM_API_BASE) && (apiId <= CSC_LLIM_MAX_API)))
        return ((char*)(csc_llim_api_name[apiId - CSC_LLIM_API_BASE - 1])); 

    /* UECC_LLIM_API */
   /*SPR 15543 Fix Start*/
    if(((((srcModuleId == RRC_UECC_MODULE_ID) || ((srcModuleId >= RRC_UECC_1_MODULE_ID) && (srcModuleId <= RRC_UECC_8_MODULE_ID)) || (srcModuleId == RRC_UECCMD_MODULE_ID)) &&
             (dstModuleId == RRC_LLIM_MODULE_ID)) || 
       ((srcModuleId == RRC_LLIM_MODULE_ID) && 
             ((dstModuleId == RRC_UECC_MODULE_ID) || ((dstModuleId >= RRC_UECC_1_MODULE_ID) && (dstModuleId <= RRC_UECC_8_MODULE_ID)) || (dstModuleId == RRC_UECCMD_MODULE_ID)))) &&     
       ((apiId >= UECC_LLIM_API_BASE) && (apiId <= UECC_LLIM_MAX_API)))
   /*SPR 15543 Fix Stop*/
        return ((char*)(uecc_llim_api_name[apiId - UECC_LLIM_API_BASE])); 

    /* OAMH_API */
    if(((((srcModuleId >= RRC_MIN_INT_MODULE_ID) && 
                    (srcModuleId <=  RRC_MAX_INT_MODULE_ID)) &&
                    (dstModuleId == RRC_OAMH_MODULE_ID)) ||
       ((srcModuleId == RRC_OAMH_MODULE_ID) && 
                    ((dstModuleId >= RRC_MIN_INT_MODULE_ID) && 
                    (dstModuleId <=  RRC_MAX_INT_MODULE_ID)))) &&
       ((apiId >= OAMH_API_BASE) && (apiId <= OAMH_MAX_API)))
       return ((char*)(oamh_api_name[apiId - OAMH_API_BASE])); 

    /* CSC_UECC_API */
   /*SPR 15543 Fix Start*/
    if((((srcModuleId == RRC_CSC_MODULE_ID) && 
             ((dstModuleId == RRC_UECC_MODULE_ID) || ((dstModuleId >= RRC_UECC_1_MODULE_ID) && (dstModuleId <= RRC_UECC_8_MODULE_ID)) || (dstModuleId == RRC_UECCMD_MODULE_ID))) ||     
       (((srcModuleId == RRC_UECC_MODULE_ID) || ((srcModuleId >= RRC_UECC_1_MODULE_ID) && (srcModuleId <= RRC_UECC_1_MODULE_ID)) || (srcModuleId == RRC_UECCMD_MODULE_ID)) &&
                    (dstModuleId == RRC_CSC_MODULE_ID))) &&     
       ((apiId >= CSC_UECC_API_BASE) && (apiId <= CSC_UECC_MAX_API)))
   /*SPR 15543 Fix Stop*/
        return ((char*)(csc_uecc_api_name[apiId - CSC_UECC_API_BASE])); 

    /* S1AP_UECC_API */
   /*SPR 15543 Fix Start*/
    if((((srcModuleId == RRC_S1AP_MODULE_ID) && 
             ((dstModuleId == RRC_UECC_MODULE_ID) || ((dstModuleId >= RRC_UECC_1_MODULE_ID) && (dstModuleId <= RRC_UECC_8_MODULE_ID)) || (dstModuleId == RRC_UECCMD_MODULE_ID))) ||           (((srcModuleId == RRC_UECC_MODULE_ID) || ((srcModuleId >= RRC_UECC_1_MODULE_ID) && (srcModuleId <= RRC_UECC_1_MODULE_ID)) || (srcModuleId == RRC_UECCMD_MODULE_ID) ) &&
                    (dstModuleId == RRC_S1AP_MODULE_ID))) &&     
       ((apiId >= S1AP_UECC_API_BASE) && (apiId <= S1AP_UECC_MAX_API)))
   /*SPR 15543 Fix Stop*/
        return ((char*)(s1ap_uecc_api_name[apiId - S1AP_UECC_API_BASE])); 

    /* X2AP_UECC_API */
    if((((srcModuleId == RRC_X2AP_MODULE_ID) && 
             ((dstModuleId == RRC_UECC_MODULE_ID) || ((dstModuleId >= RRC_UECC_1_MODULE_ID) && (dstModuleId <= RRC_UECC_8_MODULE_ID)))) ||     
       (((srcModuleId == RRC_UECC_MODULE_ID) || ((srcModuleId >= RRC_UECC_1_MODULE_ID) && (srcModuleId <= RRC_UECC_1_MODULE_ID))) &&
                    (dstModuleId == RRC_X2AP_MODULE_ID))) &&     
       ((apiId >= X2AP_UECC_API_BASE) && (apiId <= X2AP_UECC_MAX_API)))
        return ((char*)(x2ap_uecc_api_name[apiId - X2AP_UECC_API_BASE]));
    /* XWAP_UECC_API */
/* CSR_00040799 Fix Start */
    /* X2AP_UECCMD_API */
    if((((srcModuleId == RRC_X2AP_MODULE_ID) && (dstModuleId == RRC_UECCMD_MODULE_ID)) ||
                ((srcModuleId == RRC_UECCMD_MODULE_ID)  && (dstModuleId == RRC_X2AP_MODULE_ID))) &&
            ((apiId >= X2AP_UECC_API_BASE) && (apiId <= X2AP_UECC_MAX_API)))
        return ((char*)(x2ap_uecc_api_name[apiId - X2AP_UECC_API_BASE]));
/* CSR_00040799 Fix End */

    /* S1AP_CSC_API */
    if((((srcModuleId == RRC_S1AP_MODULE_ID) && 
                    (dstModuleId == RRC_CSC_MODULE_ID)) || 
       ((srcModuleId == RRC_CSC_MODULE_ID) && 
                    (dstModuleId == RRC_S1AP_MODULE_ID))) &&     
       ((apiId >= S1AP_CSC_API_BASE) && (apiId <= S1AP_CSC_MAX_API)))
        return ((char*)(s1ap_csc_api_name[apiId - S1AP_CSC_API_BASE])); 
    
    /* X2AP_CSC_API */
    if((((srcModuleId == RRC_X2AP_MODULE_ID) && 
                    (dstModuleId == RRC_CSC_MODULE_ID)) || 
       ((srcModuleId == RRC_CSC_MODULE_ID) && 
                    (dstModuleId == RRC_X2AP_MODULE_ID))) &&     
       ((apiId >= X2AP_CSC_IF_API_BASE) && (apiId <= X2AP_CSC_MAX_API)))
        return ((char*)(x2ap_csc_api_name[apiId - X2AP_CSC_IF_API_BASE])); 
    /* Internal APIs End */
    
   /* X2AP_SCTP_API */
    if((((srcModuleId == RRC_X2AP_MODULE_ID) &&
                    (dstModuleId == RRC_X2AP_PEER_eNODEB_MODULE_ID)) ||
                ((srcModuleId == RRC_X2AP_PEER_eNODEB_MODULE_ID) &&
                 (dstModuleId == RRC_X2AP_MODULE_ID))) &&
            ((apiId > X2AP_EXTERNAL_API_BASE) && 
#ifdef LINUX_PC_TEST
                (apiId <= X2AP_ENB_MESSAGE_PS_TO_TEST)
#else
                (apiId <= X2AP_SCTP_SHUT_DOWN_INIT_IND)
#endif
                ))
        return ((char*)(x2ap_sctp_api_name[apiId - X2AP_EXTERNAL_API_BASE -1]));

    /* S1AP_RRM_API */
    if((((srcModuleId == RRC_S1AP_MODULE_ID) && 
                    (dstModuleId == RRC_RRM_MODULE_ID)) || 
       ((srcModuleId == RRC_RRM_MODULE_ID) && 
                    (dstModuleId == RRC_S1AP_MODULE_ID))) &&     
       ((apiId > S1AP_RRM_API_BASE) && (apiId <= S1AP_RRM_MAX_API)))
        return ((char*)(s1ap_rrm_api_name[apiId - S1AP_RRM_API_BASE - 1])); 
  
    /* UECCMD_UECC_API */

    if((((srcModuleId == RRC_UECCMD_MODULE_ID) && 
             ((dstModuleId == RRC_UECC_MODULE_ID) || ((dstModuleId >= RRC_UECC_1_MODULE_ID) && (dstModuleId <= RRC_UECC_8_MODULE_ID)))) ||     
       (((srcModuleId == RRC_UECC_MODULE_ID) || ((srcModuleId >= RRC_UECC_1_MODULE_ID) && (srcModuleId <= RRC_UECC_1_MODULE_ID))) &&
                    (dstModuleId == RRC_UECCMD_MODULE_ID)))) 
    {
        /*SPR 15543 Fix Start*/
        /*SPR_17727_START*/
        if(((apiId > UECCMD_UECC_API_BASE) && (apiId <= UECCMD_UECC_MAX_API)))
        {
            return ((char*)(ueccmd_uecc_api_name[apiId - UECCMD_UECC_API_BASE ])); 
        }
        else if ((apiId >= OAMH_API_BASE) && (apiId <= OAMH_MAX_API))
        {
            return ((char*)(oamh_api_name[apiId - OAMH_API_BASE ])); 
        }
        else if ((apiId >= UECC_LLIM_API_BASE) && (apiId <= UECC_LLIM_MAX_API))
        {
            return ((char*)(uecc_llim_api_name[apiId - UECC_LLIM_API_BASE])); 
        }
        else if ((apiId >= CSC_UECC_API_BASE) && (apiId <= CSC_UECC_MAX_API))
        {
            return ((char*)(csc_uecc_api_name[apiId - CSC_UECC_API_BASE])); 
        }
        else if ((apiId >= S1AP_UECC_API_BASE) && (apiId <= S1AP_UECC_MAX_API))
        {
            return ((char*)(s1ap_uecc_api_name[apiId - S1AP_UECC_API_BASE])); 
        }
        else if ((apiId >= X2AP_UECC_API_BASE) && (apiId <= X2AP_UECC_MAX_API))
        {
            return ((char*)(x2ap_uecc_api_name[apiId - X2AP_UECC_API_BASE])); 
        }
        /*SPR_17727_END*/
    }
   /*SPR 15543 Fix Stop*/
   
    /* S1AP - MME INTF */
    if((srcModuleId == RRC_MME_MODULE_ID) || 
            (dstModuleId == RRC_MME_MODULE_ID))  
        return INT_API_NAME; 
   
    /* X2AP_S1AP_API */
    if(((srcModuleId == RRC_X2AP_MODULE_ID) && 
             ((dstModuleId == RRC_S1AP_MODULE_ID))) &&     
       ((apiId >= X2AP_S1AP_API_BASE) && (apiId <= X2AP_S1AP_MAX_API)))
        return ((char*)(x2ap_s1ap_api_name[apiId - X2AP_S1AP_API_BASE])); 

    else 
        return UNKNOWN_API_NAME;
}


#if (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
/*
*   Internal logger
*/

/******************************************************************************
*   FUNCTION NAME: rrc_trace_message
*   INPUT        : S32 log_level
*                  const S8 *facility_name
*                  const S8* format
*   OUTPUT       : None  
*   RETURNS      : None
*   DESCRIPTION  : This Function Implements the trace functionality.
******************************************************************************/
void rrc_trace_message(
    S32 log_level,              /* log level of this message */
    const S8 *facility_name,  /* identify who log message */
    const S8* format,         /* format string like in printf */
    ...                         /* optional parameters */
)
{
    S8 tmp_fmt[FMT_MAX_LEN+1];

    va_list arglist;
    memset_wrapper(&arglist, 0, sizeof(va_list));
    VA_START(arglist, (S32)format);

    /* Should add trailing 0 in case when string in snprintf is truncated */
    tmp_fmt[FMT_MAX_LEN] = 0;
    snprintf_wrapper(tmp_fmt,FMT_MAX_LEN,(const Char8*)"<FACILITY NAME>: %s",(char *)facility_name);

    switch(log_level)
    {
    case RRC_FATAL:
        snprintf_wrapper(tmp_fmt,FMT_MAX_LEN,(const Char8*)"<FATAL>: %s",(char *)format);
        break;
    case RRC_ERROR:
        snprintf_wrapper(tmp_fmt,FMT_MAX_LEN,(const Char8*)"<ERROR>: %s",(char *)format);
        break;
    case RRC_WARNING:
        snprintf_wrapper(tmp_fmt,FMT_MAX_LEN,(const Char8*)"<WARNING>: %s",(char *)format);
        break;
    case RRC_INFO:
        snprintf_wrapper(tmp_fmt,FMT_MAX_LEN,(const Char8*)"<INFO>: %s",(char *)format);
        break;
    case RRC_BRIEF:
        snprintf_wrapper(tmp_fmt,FMT_MAX_LEN,(const Char8*)"<BRIEF>: %s",(char *)format);
        break;
    case RRC_DETAILED:
        snprintf_wrapper(tmp_fmt,FMT_MAX_LEN,(const Char8*)"<DETAILED>: %s",(char *)format);
        break;
    case RRC_DETAILEDALL:
        snprintf_wrapper(tmp_fmt,FMT_MAX_LEN,(const Char8*)"<DETAILEDALL>: %s",(char *)format);
        break;
    default:
        break;
    }
    RRC_QVLOGVA(log_level,format,arglist);

    va_end_wrapper(arglist);
}

/******************************************************************************
*   FUNCTION NAME: rrc_set_loglevel
*   INPUT        : U8 new_log_level
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : Update global variable rrc_log_level.
******************************************************************************/
void rrc_set_loglevel(
    U8 new_log_level   /* New RRC log level */
)
{
    qvLogLevel(new_log_level);
}

#endif /* (LOGGER_TYPE == RRC_INTERNAL_LOGGER) */


/******************************************************************************
*   FUNCTION NAME: rrc_get_loglevel
*   INPUT        : void
*   OUTPUT       : None
*   RETURNS      : Value of global variable rrc_log_level.
*   DESCRIPTION  : Retrives the RRC log level.
*
******************************************************************************/
U8 rrc_get_loglevel( void )
{
    return qvGetLogLevel();
}

/*
*   ASSERT
*/
#ifdef RRC_DEBUG
/*****************************************************************************
 * Function Name  : rrc_assert 
 * Inputs         : void*      file
 *                  U32        line
 *                  void*      expression
 * Outputs        : None
 * Returns        : None
 * Description    : prints the failed assertions
 ********************************************************************************/
void rrc_assert(
    void* file,
    U32 line,
    void* expression)
{
    qvPanic("[%s:%u] Assertion Failed <%s>", (U8*)file, line, (U8*)expression);
}
#endif /* RRC_DEBUG */

/******************************************************************************
*   FUNCTION NAME: rrc_mem_get
*   INPUT        : rrc_size_t size
*   OUTPUT       : None
*   RETURNS      : Pointer to memory buffer or NULL in case of failure
*   DESCRIPTION  : This function returns memory buffer from memory pool.
*                  Function is used for general memory management purposes.
*
******************************************************************************/
/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE
void*
rrc_mem_Get
(
    rrc_size_t size, /* Size of buffer which will be allocated */
    char *func, 
    unsigned int line
)
#else
/*SPR_18125_END*/
void*
rrc_mem_get
(
    rrc_size_t size /* Size of buffer which will be allocated */
)
#endif
{
    void *p_buf = PNULL;

    int  err = 0;
#ifdef MEM_CHECK_DOUBLE_FREE
    p_buf = malloc_wrapper(size);
    /*SPR_18125_START*/
#elif CSPL_LEAK_DEBUG_LITE
    p_buf = qvAllocEX(size, PNULL,&err,func,line);
        //RRC_TRACE(RRC_FATAL, " -- p_buf:%p Func %s Line %d",p_buf,func,line);
/*SPR_18125_END*/

#else
    p_buf = qvAllocEx(size, PNULL,&err);
#endif
    if(QVERROR_MEM_ALLOC_ILLEGAL_SIZE == err)
    {
        RRC_TRACE(RRC_FATAL, "rrc_mem_get: memory allocation failed invalid buff size %u error %d",size,err);
        
        /* alarm framework integration */
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_ALLOC_INVALID_PARAM_ERR_CAUSE_ID);
    }

    if((QVERROR_SYS_ALLOC_FAIL == err) || (p_buf == PNULL))
    {
        RRC_TRACE(RRC_FATAL, "rrc_mem_get: memory allocation failed no buff available error %d",err);
        
        /* alarm framework integration */
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_ALLOC_NO_BUFF_AVAILABLE_ERR_CAUSE_ID);
/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE  
        qvPrintMemPoolAllocBuffers();
        displayMemPoolStatsCspl(); 
        qvPanic("Mem Alloc failed in rrc_mem_get");
#endif
/*SPR_18125_END*/
    }
    /* Coverity Fix 64007 start */
    if (PNULL != p_buf)
    {
	    memset_wrapper(p_buf,0,size);
    }
    /* Coverity Fix 64007 stop */
    return p_buf;
}
/*SPR_18125_START*/
/******************************************************************************
*   FUNCTION NAME: rrc_asn_mem_get
*   INPUT        : rrc_size_t size
*   OUTPUT       : None
*   RETURNS      : Pointer to memory buffer or NULL in case of failure
*   DESCRIPTION  : This function returns memory buffer from memory pool.
*                  Function is used for asn memory management purposes.
*
******************************************************************************/
void*
rrc_asn_mem_get
(
    rrc_size_t size /* Size of buffer which will be allocated */
)
{
    void *p_buf = PNULL;
    p_buf = rrc_mem_get(size);
#ifdef CSPL_LEAK_DEBUG_LITE  
    RRC_TRACE(RRC_DETAILED, "rrc_asn_mem_get: allocated p_buf:%p size= %u",p_buf,size);
#endif
    return p_buf;
}
/******************************************************************************
*   FUNCTION NAME: rrc_asn_mem_free
*   INPUT        : void *p_buffer
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function frees memory buffer allocated in pool.
*                  Function is used for asn memory management purposes.
******************************************************************************/
void
rrc_asn_mem_free
(
    void *p_buffer /* Pointer to buffer which will be freed */
)
{
#ifdef CSPL_LEAK_DEBUG_LITE  
    RRC_TRACE(RRC_DETAILED, "rrc_asn_mem_free: deallocating p_buf:%p",p_buffer);
#endif
    return rrc_mem_free(p_buffer);
}
/*SPR_18125_END*/

/******************************************************************************
*   FUNCTION NAME: rrc_mem_free
*   INPUT        : void *p_buffer
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function frees memory buffer allocated in pool.
*                  Function is used for general memory management purposes.
******************************************************************************/
void
rrc_mem_free
(
    void *p_buffer /* Pointer to buffer which will be freed */
)
{
#ifdef MEM_CHECK_DOUBLE_FREE
    free_wrapper(p_buffer);
#else
    int err = 0;
    if (p_buffer != PNULL)
    {
        qvFreeEx(p_buffer,&err);
        if(QVERROR_MEM_UNALLOCATED_BUFFER == err)
        {
            RRC_TRACE(RRC_FATAL, "rrc_mem_free: Memory already freed error %d",err);
            ALARM_MSG_L3(RRC_MODULE_ID, MEM_FREE_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_FREE_ALREADY_FREE_ERR_CAUSE_ID);
        }
        /* Re-initialize the buffer to PNULL */
        /* Coverity ID 81630 Fix Start*/
      /*  p_buffer = PNULL; */
        /* Coverity ID 81630 Fix End*/
    }
    else
    {
        RRC_TRACE(RRC_FATAL, "rrc_mem_free: null pointer passed");
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_FREE_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_FREE_NULL_POINTER_PASSED_ERR_CAUSE_ID);
    }
#endif
}

/******************************************************************************
*   FUNCTION NAME: rrc_msg_mem_get
*   INPUT        : rrc_size_t size
*   OUTPUT       : None
*   RETURNS      : Pointer to memory buffer or NULL in case of failure
*   DESCRIPTION  : This function returns memory buffer from memory pool.
*                  Function is used for messages management purposes.
******************************************************************************/
void*
rrc_msg_mem_get
(
    rrc_size_t size /* Size of buffer which will be allocated */
)
{
    void *p_buf = PNULL;
    int err = 0;

#ifdef MEM_CHECK_DOUBLE_FREE
    p_buf = malloc_wrapper(size);
#else
    p_buf = qvMsgAllocEx(PNULL, 0, 0, size,&err);
#endif
    if(QVERROR_MEM_ALLOC_ILLEGAL_SIZE == err)
    {
        RRC_TRACE(RRC_FATAL, "rrc_msg_mem_get: memory allocation failed invalid buff size %u error %d",size,err);

        /* alarm framework integration */
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_ALLOC_INVALID_PARAM_ERR_CAUSE_ID);
    }

    if((QVERROR_SYS_ALLOC_FAIL == err) || (p_buf == PNULL))
    {
        RRC_TRACE(RRC_FATAL, "rrc_msg_mem_get: memory allocation failed no buff available error %d",err);
        
        /* alarm framework integration */
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_ALLOC_NO_BUFF_AVAILABLE_ERR_CAUSE_ID);
/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE  
        qvPrintMemPoolAllocBuffers();
        displayMemPoolStatsCspl(); 
        qvPanic("Mem Alloc failed in rrc_msg_mem_get");
#endif
/*SPR_18125_END*/
    }

    return(p_buf);
}

/******************************************************************************
*   FUNCTION NAME: rrc_msg_mem_free
*   INPUT        : void *p_buffer
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function frees memory buffer allocated in pool.
*                  Function is used for messages management purposes.
******************************************************************************/
void
rrc_msg_mem_free
(
    void *p_buffer /* Pointer to buffer which will be freed */
)
{
#ifdef MEM_CHECK_DOUBLE_FREE
    free_wrapper(p_buffer);
#else    
    int err = 0;
    if (p_buffer != PNULL)
    {
        qvMsgFreeEx(p_buffer,&err);
        if(QVERROR_MEM_UNALLOCATED_BUFFER == err)
        {
            RRC_TRACE(RRC_FATAL, "rrc_msg_mem_free: Memory already freed error %d",err);
            ALARM_MSG_L3(RRC_MODULE_ID, MEM_FREE_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_FREE_ALREADY_FREE_ERR_CAUSE_ID);
        }
    }
    else
    {
        RRC_TRACE(RRC_FATAL, "rrc_msg_mem_free: null pointer passed");
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_FREE_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_FREE_NULL_POINTER_PASSED_ERR_CAUSE_ID);
    }
#endif
}

/******************************************************************************
*   FUNCTION NAME: rrc_sys_mem_get
*   INPUT        : rrc_size_t size
*   OUTPUT       : None
*   RETURNS      : Pointer to memory buffer or NULL in case of failure
*   DESCRIPTION  : This function returns memory buffer using direct OS call allocation
*                  routine.
******************************************************************************/
void*
rrc_sys_mem_get
(
    rrc_size_t size /* Size of buffer which will be allocated */
)
{
    void *p_buf = PNULL;

#ifdef MEM_CHECK_DOUBLE_FREE
    p_buf = malloc_wrapper(size);
#else
    p_buf = (void *)qvSysMalloc(size);
#endif
    if(p_buf == PNULL)
    {
        RRC_TRACE(RRC_FATAL, "rrc_sys_mem_get: memory allocation failed");

        /* alarm framework integration */
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_ALLOC_NO_BUFF_AVAILABLE_ERR_CAUSE_ID);
/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE  
        qvPrintMemPoolAllocBuffers();
        displayMemPoolStatsCspl(); 
        qvPanic("Mem Alloc failed in rrc_sys_mem_get");
#endif
/*SPR_18125_END*/
    }

    return(p_buf);
}

/******************************************************************************
*   FUNCTION NAME: rrc_sys_mem_free
*   INPUT        : void *p_buffer
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function frees memory buffer using direct OS call free
*                  routine.
*
******************************************************************************/
void
rrc_sys_mem_free
(
    void *p_buffer /* Pointer to buffer which will be freed */
)
{
#ifdef MEM_CHECK_DOUBLE_FREE
    free_wrapper(p_buffer);
#else
    if (p_buffer != PNULL)
    {
        qvSysFree(p_buffer);
    }
    else
    {
        RRC_TRACE(RRC_FATAL, "rrc_msg_mem_free: null pointer passed");
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_FREE_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_FREE_NULL_POINTER_PASSED_ERR_CAUSE_ID);
    }
#endif
}

/******************************************************************************
*   FUNCTION NAME: rrc_start_timer
*   INPUT        : U32         timer_duration
*                  void        *p_timer_data
*                  U16         timer_data_size
*                  rrc_bool_t  is_repeated
*   OUTPUT       : None
*   RETURNS      : Timer Id as allocated by the CSPL
*   DESCRIPTION  : This function is used for starting timer by RRC.
*                  Timer allocates buffer which has size timer_data_size and
*                  this data will be freed in rrc_stop_timer
******************************************************************************/
rrc_timer_t
rrc_start_timer
(
    /* Duration (in milliseconds) of the timer to be started */
    U32         timer_duration,
    /* Timer data that will be attached to timer */
    void        *p_timer_data,
    U16         timer_data_size,    /* Size of timer data */
    rrc_bool_t  is_repeated         /* Indicates whether timer is repeated */
)
{
    QTIME qtime;
    void *p_timer_buf = PNULL;
    rrc_timer_t timer = PNULL;
    int err = 0;

    RRC_ASSERT(0 != timer_duration);

    p_timer_buf = rrc_mem_get(timer_data_size);

    if(p_timer_buf == PNULL)
    {
        return PNULL;
    }

    if((0 != timer_data_size) && (PNULL != p_timer_data))
    {
        l3_memcpy_wrapper(p_timer_buf, p_timer_data, timer_data_size);
    }

    qtime.s = timer_duration / 1000;
    qtime.us = (timer_duration % 1000) * 1000;

    timer = qvTimerStartEx(&qtime, p_timer_buf, is_repeated,&err);
    RRC_TRACE(RRC_BRIEF,
        "rrc_start_timer: Timer Id [%p]: Duration: [%d]msec",
        timer,
        timer_duration);

    if((QVERROR_TIMER_ALLOC_FAIL == err) || (NULL == timer))
    {
        RRC_TRACE(RRC_FATAL, "rrc_start_timer: Timer Start failed No buffer available in the POOL");

        /* Release the memory allocated for storing the timer data */
        rrc_mem_free(p_timer_buf);

       p_timer_buf = PNULL;

        /* alarm framework integration */
        ALARM_MSG_L3(RRC_MODULE_ID, TIMER_START_FAILED_ALARM_ID,CRITICAL_ALARM,TIMER_CREATE_NO_BUFF_IN_TIMER_POOL_ERR_CAUSE_ID);
    }

    return timer;
}

/******************************************************************************
*   FUNCTION NAME: rrc_timer_now
*   INPUT        : rrc_timer_t timer
*   OUTPUT       : None
*   RETURNS      : Timer duration in milliseconds
*   DESCRIPTION  : This function is called for fetching remaining
*                  time to expire in any running timer 
*  ******************************************************************************/
U16
rrc_timer_now
(
    rrc_timer_t timer
)
{
    QTIME qtime;
    U32 result = RRC_NULL;
    U16 timer_duration = RRC_NULL;
    
    result = (U32)qvTimerRemaining(timer, &qtime);
    if (0 == result)
    {
        RRC_TRACE(RRC_DETAILED, "No Remaining Time.");
    }
    timer_duration = (U16)((qtime.s) + (qtime.us/1000000));
    
    RRC_TRACE(RRC_DETAILED,
            "rrc_timer_now : Timer ID[%p] : Duration:[%d]sec",
            timer, timer_duration);

    return (timer_duration);
}

/******************************************************************************
*   FUNCTION NAME: rrc_stop_timer
*   INPUT        : rrc_timer_t timer
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function is used for stopping timer by RRC.
******************************************************************************/
void
rrc_stop_timer
(
    rrc_timer_t timer /* Timer Id as allocated by the CSPL */
)
{
    RRC_ASSERT(PNULL != timer);

    if (PNULL == timer)
    {
        RRC_TRACE(RRC_ERROR, "rrc_stop_timer: NULL timer passed!");

        /* alarm framework integration */
        ALARM_MSG_L3(RRC_MODULE_ID, TIMER_STOP_FAILED_ALARM_ID,MAJOR_ALARM,TIMER_STOP_INVALID_TIMER_ID_ERR_CAUSE_ID);
    }
    else
    {
        /* SPR 23608/SES-43 Fix Start */
        if(qvTimerRunning(timer) || qvTimerGetRepeatFlag(timer))//modify by wood for fixing bug at 2019.5.20 (bugid:114106)
        {
            void *p_timer_data = PNULL;

            p_timer_data = qvTimerData(timer);

            RRC_TRACE(RRC_DETAILED, "rrc_stop_timer: Timer Id [%p]", timer);

            if(p_timer_data != PNULL)
            {
                rrc_mem_free(p_timer_data);
                p_timer_data = PNULL;
            }
            else
            {
                RRC_TRACE(RRC_WARNING, "rrc_stop_timer: Timer Id [%p] contains "
                        "null data", timer);
            }

            qvTimerStop(timer, PNULL);
        }
        /* SPR 23608/SES-43 Fix End */
    }
}


/******************************************************************************
*   FUNCTION NAME: rrc_send_message
*   INPUT        : void            *p_msg
*                  rrc_module_id_t dst_module
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function is called by stack to Send a message to some external
*                  module.
*
******************************************************************************/
void
/*SPR 20172 Fix Start*/
rrc_send_message_ex
/*SPR 20172 Fix End*/
(
    void            *p_msg,     /* Message that will be passed */
    rrc_module_id_t dst_module  /* Module id for which message will be passed */
)
{
    S32 error_code = RRC_NULL;
    QMODULE service = qvGetServiceEx(dst_module,(int *)&error_code);
    /*SPR_19937_START*/
    U16             ue_index = 0xFFFF;
    rrc_module_id_t src_module = 0;
    /*SPR_19937_END*/
    U16 api_id = 0;

    RRC_ASSERT(PNULL != p_msg);

    if(QVERROR_NONE != error_code)
    {
        RRC_TRACE(RRC_FATAL, "Cannot get service name: %s",
            error_code == QVERROR_INVALID_ARGUMENT ?
                "invalid argument passed" :
                "module allocation failed");
        
        /* alarm framework integration */
        ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);

        qvPanic("Cannot get service name: %s",
                error_code == QVERROR_INVALID_ARGUMENT ?
                "invalid argument passed" :
                "module allocation failed");
        return;
    }
    /*SPR_19937_START*/
    src_module = rrc_get_src_module_id(p_msg);
    /*SPR_19937_END*/
#ifdef MEM_PROFILE_ON
    qvPoolStats();
#endif
    /*SPR_19937_START*/
    /*Lines Deleted*/
    /*SPR_19937_END*/
    U16             msg_size   = rrc_get_api_buf_size(p_msg);
    api_id     = rrc_get_api_id(p_msg);
#ifndef LINUX_PC_TEST
    if (( ((dst_module >= RRC_MIN_EXT_MODULE_ID) && (dst_module <= RRC_MAX_EXT_MODULE_ID) ) ||
                (dst_module == RRC_SON_MODULE_ID)) &&
        (!((dst_module == RRC_RRM_MODULE_ID) && (src_module == RRC_UECCMD_MODULE_ID))) )
#endif        
    {
        /* Binary dump of the message */
        /* coverity_93933 start */
        U32     sixteens_align  = ((msg_size / (U16)16) * (U16)16);
        U32     remains         = ((U32)msg_size - sixteens_align);
        /* coverity_93933 stop */

	/*SPR_19937_START*/
	if((RRC_UECC_MODULE_ID == src_module) ||
			(RRC_UECC_1_MODULE_ID <= src_module &&
			 RRC_UECC_8_MODULE_ID >= src_module))
	{
		ue_index = GET_UE_INDEX_FROM_MSG(api_id, (p_msg + CV_HDRSIZE)); 
		/* report about the message */
		RRC_TRACE(RRC_INFO, "[RRCEVT:SEND] [MODULE:%s(%u)] [UE_ID:%u] [API:%s(%u)]",
				GET_MODULE_NAME(dst_module), dst_module, ue_index, 
				GET_API_NAME(rrc_get_api_id(p_msg),rrc_get_src_module_id(p_msg),dst_module),
				rrc_get_api_id(p_msg)); 
	}
	else
	{
#ifdef RRC_UNIT_TEST_FRAMEWORK
        if(65535 == rrc_get_api_id(p_msg))
        {
            stub_roll_logs();
            return ;
        }
#endif
		RRC_TRACE(RRC_INFO, "[RRCEVT:SEND] [MODULE:%s(%u)] [API:%s(%u)]",
				GET_MODULE_NAME(dst_module), dst_module,  
				GET_API_NAME(rrc_get_api_id(p_msg),rrc_get_src_module_id(p_msg),dst_module),
				rrc_get_api_id(p_msg)); 
	}
	/*SPR_19937_END*/    
        if (remains > 0)
        {

            /*SPR 21933 Changes Start */
            /* Code Removed */
            /*SPR 21933 Changes Start */
        }
    }
#ifdef LATENCY_TRACE_ON

    struct timespec  tp;
    struct tm      *tm;
 
    clock_gettime_real_time(&tp); 
    tm = localtime_wrapper(&tp.tv_sec);

    if (start_logging == 1)
    {
        g_latency_data[g_index].send_rcv_flag = RRC_SEND;
        g_latency_data[g_index].src_mod_id = rrc_get_src_module_id(p_msg);
        g_latency_data[g_index].dest_mod_id = dst_module;
        g_latency_data[g_index].api_id = rrc_get_api_id(p_msg);
        l3_memcpy_wrapper(&g_latency_data[g_index].val_time, tm, 
                sizeof(g_latency_data[g_index].val_time)); 
        g_latency_data[g_index].time_u_secs = tp.tv_nsec; 
        g_index++;
    }

#endif
    qvSend(service, 0, p_msg);
}

/******************************************************************************
*   FUNCTION NAME: rrc_check_cspl_header
*   INPUT        : U8 *p_api
*   OUTPUT       : None
*   RETURNS      : rrc_return_et
*   DESCRIPTION  : This function check general CSPL message header integrity.
*
******************************************************************************/
rrc_return_et
rrc_check_cspl_header
(
    U8 *p_api   /* Pointer to input API buffer */
)
{
    STACKAPIHDR     api_header;
    rrc_return_et   result = RRC_FAILURE;
    U32             msg_size = RRC_NULL;
    /*SPR_19937_START*/
    U16             ue_index = 0xFFFF;
    /*SPR_19937_END*/

    RRC_ASSERT(PNULL != p_api);

    msg_size = cvApiParseHeader(p_api, CV_HDRSIZE, &api_header);

    if (0 != msg_size)
    {
        if ( (api_header.from >= RRC_MIN_INT_MODULE_ID) && (api_header.from <=
            RRC_MAX_INT_MODULE_ID) )
        {
            result = RRC_SUCCESS;
        }
        else if (( (api_header.from >= RRC_MIN_EXT_MODULE_ID) && (api_header.from
            <= RRC_MAX_EXT_MODULE_ID) ) || (api_header.from == RRC_SON_MODULE_ID))
        {
            /* Check whether external header present */
            if ((RRC_MME_MODULE_ID == api_header.from)
                  || (RRC_X2AP_PEER_eNODEB_MODULE_ID == api_header.from))
            {
                if (msg_size >= RRC_API_HEADER_SIZE)
                {
                    result = RRC_SUCCESS;
                }
            }
            else if (msg_size >= RRC_FULL_INTERFACE_HEADERS_SIZE)
            {
                result = RRC_SUCCESS;
            }
        }
    }

#ifdef MEM_PROFILE_ON
    qvPoolStats();
#endif

    if (RRC_SUCCESS == result)
    {
        /* report about the message */
#ifndef LINUX_PC_TEST
        /* BUG 9135 Fix Start */
        if ((( (api_header.from >= RRC_MIN_EXT_MODULE_ID) && (api_header.from
            <= RRC_MAX_EXT_MODULE_ID) ) || (api_header.from == RRC_SON_MODULE_ID)) &&
            (!((api_header.from == RRC_RRM_MODULE_ID) && (api_header.to == RRC_UECCMD_MODULE_ID))) )
        /* BUG 9135 Fix Stop */
#endif		
        {
	   /*SPR_19937_START*/
	   if((RRC_UECC_MODULE_ID == api_header.to) ||
			   (RRC_UECC_1_MODULE_ID <= api_header.to &&
			    RRC_UECC_8_MODULE_ID >= api_header.to))
	   {
		   ue_index = GET_UE_INDEX_FROM_MSG(api_header.api, (p_api + CV_HDRSIZE)); 
		   /* report about the message */
		   RRC_TRACE(RRC_INFO, "[RRCEVT:RECV] [MODULE:%s(%u)] [UE_ID:%u] [API:%s(%u)] [SIZE:%u]"
				   " [VER:%u]",
				   GET_MODULE_NAME(api_header.from),api_header.from, ue_index,
				   GET_API_NAME(api_header.api, api_header.from, api_header.to), api_header.api,
				   msg_size, api_header.version);
	   }
	   else
	   {
		   /* report about the message */
		   RRC_TRACE(RRC_BRIEF, "[RRCEVT:RECV] [MODULE:%s(%u)] [API:%s(%u)] [SIZE:%u]"
				   " [VER:%u]",
				   GET_MODULE_NAME(api_header.from),api_header.from,
				   GET_API_NAME(api_header.api, api_header.from, api_header.to), api_header.api,
				   msg_size, api_header.version);
	   }
	   /*SPR_19937_END*/

        }
    }

    return result;
}

/* SPR 1643 Start */
/*****************************************************************************
 * Function Name  : is_valid_plmn_identity
 * Inputs         : p_data - Pointer to data carrying PLMN ID Octets
 * Output         : None
 * Returns        : RRC_SUCCESS - PLMN ID Validation was successful.
 *                  RRC_FAILURE - PLMN ID Validation was not successful.
 * Description    : This function validates PLMN ID for invalid values.
 *****************************************************************************/
rrc_return_et 
is_valid_plmn_identity
(
    U8 *p_data 
)
{
 
    /* PLMN Identity Validation */
    if( IS_VALID_PLMN(p_data[0]) &&
      (((p_data[1] & 0x0F) <= MAX_VAL_DIGIT)) &&
      IS_VALID_PLMN_FILLER(p_data[1]) &&
      IS_VALID_PLMN(p_data[2]))
    {
        RRC_TRACE(RRC_DETAILED,
                  "PLMN Identity Range Pass.");
        return RRC_SUCCESS;
    }
    else
    {
        RRC_TRACE(RRC_WARNING,
                  "PLMN Identity Range Fail.");
        return RRC_FAILURE;
    }
}

/****************************************************************************
 ** Function Name  : is_valid_mcc_mnc
 ** Inputs         : PLMN_Identity    * - pointer to MCC/MNC Octets
 ** Outputs        : None
 ** Returns        : RRC_SUCCESS or RRC_FAILURE
 ** Description    : This function validates MCC/MNC for invalid values.
 *****************************************************************************/

rrc_return_et is_valid_mcc_mnc(
        PLMN_Identity    *p_plmn_Identity )
{
    OSUINT32      digit = 0;
    /* MCC Check */
    if (p_plmn_Identity->m.mccPresent)
    {
        for (digit = 0;
                digit < p_plmn_Identity->mcc.n;
                digit++)
        {
            if (!IS_VALID_MCC_MNC_DIGIT(p_plmn_Identity->mcc.elem[digit]))
            {
                RRC_TRACE(RRC_WARNING,
                        "MCC Range Check Fail");
                return RRC_FAILURE;
            }
        }
    }
    /* MNC Check */
    for (digit = 0;
            digit < p_plmn_Identity->mnc.n;
            digit++)
    {
        if (!IS_VALID_MCC_MNC_DIGIT(p_plmn_Identity->mnc.elem[digit]))
        {
            RRC_TRACE(RRC_WARNING,
                    "MNC Range Check Fail");
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}




/* SPR 1643 Stop */

/******************************************************************************
*   FUNCTION NAME: rrc_generate_protocol_events
*   INPUT        : U8 log_level
*                  void *p_api
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : Generates the Local Events.
******************************************************************************/
void rrc_generate_protocol_events(U8 log_level, void *p_api)
{
    if (PNULL != p_api)
    {
        RRC_EVENT_TRACE(log_level, "Protocol Event with event_id = %d has been "
            "generated, Total Message Length = %d",
            (*(U16*)((U8 *)p_api + sizeof(U32) + sizeof(U64) + sizeof(U16))),
            (*(U32 *)((U8 *)p_api)));

        rrc_mem_free (p_api);
    }
}

/******************************************************************************
*   File management functions
******************************************************************************/

/*****************************************************************************
 * Function Name  : rrc_free 
 * Inputs         :  void *p_var
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function frees memory
 ********************************************************************************/
void
rrc_free
(
    void *p_var
)
{
    if (p_var != PNULL)
    {
        free_wrapper(p_var);
    }
    else
    {
        RRC_TRACE(RRC_FATAL, "rrc_mem_free: null pointer passed");
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_FREE_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_FREE_NULL_POINTER_PASSED_ERR_CAUSE_ID);
    }
}

/*****************************************************************************
 * Function Name  : get_rand_val_from_seed 
 * Inputs         : U32  seed
 * Outputs        : None
 * Returns        : random_value
 * Description    : This function gets random value from the value passed 
 ********************************************************************************/
U32 get_rand_val_from_seed(U32 seed)
{
    U32 random_value = 0;
    timespec_t multiplier = {0};
    clock_gettime_real_time(&multiplier); 
    random_value = ( seed * multiplier.tv_sec ) + ( multiplier.tv_nsec );
    return random_value;
}


void rrc_update_health_monitor_variable
(
    U32 health_monitor_event
)
{
    switch(health_monitor_event)
    {
    /*Lockless Implementation start*/
        case OAMH_HEALTH_MONITOR_EXPIRY_EVENT:
            sync_fetch_and_or_wrapper(&rrc_health_monitor_val[0],1,rrc_lock);
            break;
        case UECC1_HEALTH_MONITOR_EXPIRY_EVENT:
            sync_fetch_and_or_wrapper(&rrc_health_monitor_val[1],1,rrc_lock);
            break;
        case UECC2_HEALTH_MONITOR_EXPIRY_EVENT:
            sync_fetch_and_or_wrapper(&rrc_health_monitor_val[2],1,rrc_lock);
            break;
        case UECC3_HEALTH_MONITOR_EXPIRY_EVENT:
            sync_fetch_and_or_wrapper(&rrc_health_monitor_val[3],1,rrc_lock);
            break;
        case UECC4_HEALTH_MONITOR_EXPIRY_EVENT:
            sync_fetch_and_or_wrapper(&rrc_health_monitor_val[4],1,rrc_lock);
            break;
        case UECC5_HEALTH_MONITOR_EXPIRY_EVENT:
            sync_fetch_and_or_wrapper(&rrc_health_monitor_val[5],1,rrc_lock);
            break;
        case UECC6_HEALTH_MONITOR_EXPIRY_EVENT:
            sync_fetch_and_or_wrapper(&rrc_health_monitor_val[6],1,rrc_lock);
            break;
        case UECC7_HEALTH_MONITOR_EXPIRY_EVENT:
            sync_fetch_and_or_wrapper(&rrc_health_monitor_val[7],1,rrc_lock);
            break;
        case UECC8_HEALTH_MONITOR_EXPIRY_EVENT:
            sync_fetch_and_or_wrapper(&rrc_health_monitor_val[8],1,rrc_lock);
            break;
        case CSC_HEALTH_MONITOR_EXPIRY_EVENT:
            sync_fetch_and_or_wrapper(&rrc_health_monitor_val[9],1,rrc_lock);
            break;
        case LLIM_HEALTH_MONITOR_EXPIRY_EVENT:
            sync_fetch_and_or_wrapper(&rrc_health_monitor_val[10],1,rrc_lock);
            break;
        case ADV_LOGGER_HEALTH_MONITOR_EXPIRY_EVENT:
            sync_fetch_and_or_wrapper(&rrc_health_monitor_val[11],1,rrc_lock);
            break;
        case S1AP_HEALTH_MONITOR_EXPIRY_EVENT:
            /*Bug 9305 start*/
            sync_fetch_and_or_wrapper(&s1ap_health_monitor_val[0],1,s1ap_lock);
            /*Bug 9305 end*/
            break;
#if 0		
#ifdef ARICENT_SCTP_IPR
        case SCTP_HEALTH_MONITOR_IPC_MASTER_EXPIRY_EVENT:
            /*Bug 9305 start*/
            sync_fetch_and_or_wrapper(&s1ap_health_monitor_val[1],1,s1ap_lock);
            /*Bug 9305 end*/
            break;
        case SCTP_HEALTH_MONITOR_RECV_TIMER_EXPIRY_EVENT:
            /*Bug 9305 start*/
            sync_fetch_and_or_wrapper(&s1ap_health_monitor_val[2],1,s1ap_lock);
            /*Bug 9305 end*/
            break;
#endif
#endif

        case X2AP_HEALTH_MONITOR_EXPIRY_EVENT:
            sync_fetch_and_or_wrapper(&x2ap_health_monitor_val,1,x2ap_lock);
            break;
    /*Lockless Implementation stop*/
        default:
            RRC_TRACE( RRC_WARNING,"Wrong Module Id");
            break;
    }

    return;
}

oamh_health_monitor_desriptors_t oamh_health_monitor_desriptors[RRC_MAX_THREADS] =
{
    {(S8*)"OAMH_THREAD_NOT_ALIVE"},
    {(S8*)"UECC1_THREAD_NOT_ALIVE"},
    {(S8*)"UECC2_THREAD_NOT_ALIVE"},
    {(S8*)"UECC3_THREAD_NOT_ALIVE"},
    {(S8*)"UECC4_THREAD_NOT_ALIVE"},
    {(S8*)"UECC5_THREAD_NOT_ALIVE"},
    {(S8*)"UECC6_THREAD_NOT_ALIVE"},
    {(S8*)"UECC7_THREAD_NOT_ALIVE"},
    {(S8*)"UECC8_THREAD_NOT_ALIVE"},
    {(S8*)"CSC_THREAD_NOT_SUPPORTED"},
    {(S8*)"LLIM_THREAD_NOT_SUPPORTED"},
    {(S8*)"ADV_LOGGER_THREAD_NOT_ALIVE"}
};

s1ap_health_monitor_desriptors_t s1ap_health_monitor_desriptors[S1AP_SCTP_MAX_THREADS] =
{
    {(S8*)"S1AP_THREAD_NOT_ALIVE"},
#if 0    
#ifdef ARICENT_SCTP_IPR
    {(S8*)"S1AP_SCTP_IPC_MASTER_THREAD_NOT_ALIVE"},
    {(S8*)"S1AP_SCTP_RECV_THREAD_NOT_ALIVE"}
#endif
#endif
};

/* SPR 16017 Fix Start */ 
/*****************************************************************************
 * Function Name  : rrc_check_logger_type 
 * Inputs         : U32  log_level
 * Outputs        : None
 * Returns        : RRC_SUCCESS?RRC_FAILURE
 * Description    : This function checks logger type and log level 
 ********************************************************************************/
rrc_return_et rrc_check_logger_type(U32 log_level)
{
    rrc_return_et       result= RRC_FAILURE;    
#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    U8 is_log_level_set = 0;
    RRC_UECC_CHECK_LOG_LEVEL (is_log_level_set, (AdvL_LogLevel)log_level);/*cov 88494+-*/
    if (is_log_level_set)
    {
        result = RRC_SUCCESS;
    }
#else
    if (rrc_get_loglevel() >= log_level)
    {
        result = RRC_SUCCESS;
    }
#endif
   return result; 
}

/* SPR 21275 changes start */
/*****************************************************************************
 * Function Name  : rrc_map_log_level 
 * Inputs         : AdvL_LogLevel  log_level
 * Outputs        : None
 * Returns        : lgr_log_level_et
 * Description    : This function does the conversion of log leve from type AdvL_LogLevel
 *                  to lgr_log_level_et
 ********************************************************************************/
lgr_log_level_et rrc_map_log_level(AdvL_LogLevel log_level)
{
    switch (log_level)
    {
        case LogFatal:
        return LGR_CRITICAL;

        case LogError:
        return LGR_ERROR;

        case LogWarning:
        return LGR_WARNING;

        case LogInfo:
        return LGR_INFO;

        case LogBrief:
        return LGR_BRIEF;

        case LogDetailed:
        return LGR_DETAILED;

        case LogDebug:
        return LGR_DEBUG;

        default:
        return LGR_TRACE;

    }
}
/* SPR 21275 changes end */
/* SPR 16017 Fix Stop */ 
/* SPR 22304 Fix Start */
    void rrc_trace_dbg(
            const S8* facility_name,U8 log_level,
            const char* format, ...)
{
    /* Empty Definition */
}
/* SPR 22304 Fix End */
#ifdef LINUX_PC_TEST

extern rrc_return_et
	rrc_compose_test_sctp_message
(
	U8** pp_buffer,
	test_sctp_message_t* p_test_sctp_message,
	const void * pmsg,
	U32 msglen
);

S32 pctest_forward_mme_message_to_pc
(
    U16     src_module_id,  /* Source module Id */
    U16     dst_module_id,  /* Destination module Id */
    U16     api_id,         /* API Id /msg id*/
	U32 socketId,
	U16 streamId,
	const void * pmsg,
	U32 msglen
)
{
	test_sctp_message_t p_test_sctp_message;
	U8 *p_msg,*p_sctp_msg;
	U16 msg_length=0,msg_api_length = 0;	
	
	p_test_sctp_message.ulSocketID = socketId;
	p_test_sctp_message.usStreamID = streamId; 

	msg_length += sizeof(p_test_sctp_message); 
	msg_length += msglen;
	msg_api_length = msg_length + RRC_API_HEADER_SIZE;

	/* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return -1; //failure
    }

    memset_wrapper(p_msg, 0, msg_api_length);

    /* Fill CSPL header */
    rrc_construct_api_header(p_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, api_id, msg_api_length);	

	RRC_TRACE(RRC_INFO, "src_module_id = %d,dst_module_id = %d,api_id = %d,socketId = %d, streamId = %d,msglen = %d",
			src_module_id,dst_module_id,api_id,socketId,streamId,msglen);

	RRC_TRACE(RRC_INFO,"s1ap msg: %s",change_data_to_str(msglen, (U8 * )pmsg));

	p_sctp_msg = p_msg + RRC_API_HEADER_SIZE;//fill msg
	rrc_compose_test_sctp_message(&p_sctp_msg, &p_test_sctp_message,pmsg,msglen);

	/* Send message to */
    rrc_send_message(p_msg, dst_module_id);	
	
	return 0;//succ
}

void pctest_send_sctp_close_to_pc(U32 sd)
{
	test_sctp_message_t p_test_sctp_message;
	U16 msg_length=0,msg_api_length = 0;
	U8 *p_msg,*p_sctp_msg;

	p_test_sctp_message.ulSocketID = sd;
	p_test_sctp_message.usStreamID = 0; //not used

	msg_length += sizeof(p_test_sctp_message); 
	msg_api_length = msg_length + RRC_API_HEADER_SIZE;

	/* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

	if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return ; //failure
    }

	memset_wrapper(p_msg, 0, msg_api_length);

	rrc_construct_api_header(p_msg, RRC_VERSION_ID, RRC_S1AP_MODULE_ID,
        RRC_MME_MODULE_ID, SCTP_CLOSED_PS_TO_TEST, msg_api_length);	

	RRC_TRACE(RRC_INFO, "SCTP_CLOSED_PS_TO_TEST:socketId = %d",	sd);

	p_sctp_msg = p_msg + RRC_API_HEADER_SIZE;//fill msg
	rrc_compose_test_sctp_message(&p_sctp_msg, &p_test_sctp_message,p_msg,0);

	/* Send message to */
    rrc_send_message(p_msg, RRC_MME_MODULE_ID);	
	
	return ;//succ		
}


S32 pctest_forward_x2ap_lksctp_message_to_pc
(
    U16     src_module_id,  /* Source module Id */
    U16     dst_module_id,  /* Destination module Id */
    U16     api_id,         /* API Id /msg id*/
    U32 socketId,
    U16 streamId,
    struct sockaddr *pstPeerAddr,   
    const void * pmsg,
    U32 msglen
)
{
    U8 *p_msg,*p_sctp_msg;
    U16 msg_length=0;	
    struct sockaddr_in *pstPeerAddrIpv4 = (struct sockaddr_in *)pstPeerAddr;

    msg_length += sizeof(socketId) + sizeof(streamId) + sizeof(struct sockaddr_storage); 
    msg_length += msglen;
    msg_length += RRC_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return 0; //failure
    }

    memset_wrapper(p_msg, 0, msg_length);

    /* Fill CSPL header */
    rrc_construct_api_header(p_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, api_id, msg_length);	

    RRC_TRACE(RRC_INFO, "src_module_id=%d, dst_module_id=%d, api_id=%d, socketId=%d, streamId=%d, msglen=%d, sin_port=%d, sin_addr=%s",
    		src_module_id,dst_module_id,api_id,socketId,streamId,msglen,pstPeerAddrIpv4->sin_port, inet_ntoa(pstPeerAddrIpv4->sin_addr));

    p_sctp_msg = p_msg + RRC_API_HEADER_SIZE;//fill msg
    //rrc_compose_test_sctp_message(&p_sctp_msg, &p_test_sctp_message,pmsg,msglen);

    rrc_cp_pack_U32(p_sctp_msg, &socketId, "socketId");
    p_sctp_msg += sizeof(socketId);

    rrc_cp_pack_U16(p_sctp_msg, &streamId, "streamId");
    p_sctp_msg += sizeof(streamId);

    /*already converted to net byteorder by caller*/
    memcpy(p_sctp_msg, (U8*)pstPeerAddr, sizeof(struct sockaddr_storage));
    p_sctp_msg += sizeof(struct sockaddr_storage);
        
    memcpy(p_sctp_msg, pmsg, msglen);

    rrc_send_message(p_msg, dst_module_id);	
	
    return 1;
}

void pc_test_send_msg(
U16 	src_module_id,	/* Source module Id */
U16 	dst_module_id,	/* Destination module Id */
U16 	api_id, 		/* API Id /msg id*/
const void * pmsg,
U32 msglen)
{
    U8 *p_msg,*p_tmp;
    U16 msg_length=0;	

    msg_length += msglen;
    msg_length += RRC_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return ; //failure
    }

    memset_wrapper(p_msg, 0, msg_length);

    /* Fill CSPL header */
    rrc_construct_api_header(p_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, api_id, msg_length);	

    RRC_TRACE(RRC_INFO, "pc_test_send_msg:srcModId=%d,dstModId=%d,msgid=0x%x,msglen=%d",
    		src_module_id,dst_module_id,api_id,msglen);

    p_tmp = p_msg + RRC_API_HEADER_SIZE;//fill msg    


    /*already converted to net byteorder by caller*/
    memcpy(p_tmp, (U8*)pmsg, msglen);
	SEND_TO_TEST(api_id,(U8*)p_msg, msg_length);
	return;
}

#include "s1ap_timers.h"
#include "x2ap_timer.h"
#include "oamh_timers.h"
#include "ueccmd_timers.h"
#include "rrc_llim_timers.h"
#include "csc_timers.h"
#include "rrc_llim_timers.h"


void pc_test_forward_timer_msg(U32 moduldId,const void* pdata,U8 timeType)
{
	s1ap_timer_buffer_t *pS1apTimerData = NULL;
	x2ap_timer_buffer_t *pX2apTimerData = NULL;
	uecc_timer_buffer_t *pUeccTimerData = NULL;
	oamh_timer_buf_t *pOamhTimerData = NULL;
	csc_timer_buf_t *pCscTimerData = NULL;
	rrc_llim_timer_buf_t *pLlimTimerData = NULL;
	timer_info_t msg;
	switch(moduldId)
	{
		case RRC_S1AP_MODULE_ID:
			pS1apTimerData = (s1ap_timer_buffer_t *)pdata;
			if(pS1apTimerData->timer_type == 0) //init timer; don't forward init timer to pc
				return;
			msg.timerId = pS1apTimerData->timer_type;
			msg.msgType = timeType;			
			RRC_TRACE(RRC_INFO, "pc_test_forward_timer_msg:moduldId=%d,timeType=%d,timerId=%d",
    		moduldId,timeType,msg.timerId);
			pc_test_send_msg(RRC_S1AP_MODULE_ID,RRC_S1AP_MODULE_ID,TIMER_MESSAGE_TO_PC_S1AP,
			&msg,sizeof(timer_info_t));
			break;

		case RRC_X2AP_MODULE_ID:
			pX2apTimerData = (x2ap_timer_buffer_t *)pdata;
			if(pX2apTimerData->timer_type == 0) //init timer; don't forward init timer to pc
				return;
			msg.timerId = pX2apTimerData->timer_type;
			msg.msgType = timeType;			
			RRC_TRACE(RRC_INFO, "pc_test_forward_timer_msg:moduldId=%d,timeType=%d,timerId=%d",
    		moduldId,timeType,msg.timerId);
			pc_test_send_msg(RRC_X2AP_MODULE_ID,RRC_X2AP_MODULE_ID,TIMER_MESSAGE_TO_PC_X2AP,
			&msg,sizeof(timer_info_t));
			break;	

		case RRC_OAMH_MODULE_ID:
			pOamhTimerData = (oamh_timer_buf_t *)pdata;
			if(pOamhTimerData->timer_type == OAMH_INIT_TIMER) //init timer; don't forward init timer to pc
				return;
			msg.timerId = pOamhTimerData->timer_type;
			msg.msgType = timeType;			
			RRC_TRACE(RRC_INFO, "pc_test_forward_timer_msg:moduldId=%d,timeType=%d,timerId=%d",
    		moduldId,timeType,msg.timerId);
			pc_test_send_msg(RRC_OAMH_MODULE_ID,RRC_OAMH_MODULE_ID,TIMER_MESSAGE_TO_PC_OAMH,
			&msg,sizeof(timer_info_t));			
			break;

		case RRC_UECC_MODULE_ID:
		case RRC_UECC_1_MODULE_ID:
		case RRC_UECC_2_MODULE_ID:
		case RRC_UECC_3_MODULE_ID:
		case RRC_UECC_4_MODULE_ID:
		case RRC_UECC_5_MODULE_ID:
		case RRC_UECC_6_MODULE_ID:
		case RRC_UECC_7_MODULE_ID:
		case RRC_UECC_8_MODULE_ID:
			pUeccTimerData = (uecc_timer_buffer_t *)pdata;
			msg.timerId = pUeccTimerData->id;
			msg.msgType = timeType;			
			RRC_TRACE(RRC_INFO, "pc_test_forward_timer_msg:moduldId=%d,timeType=%d,timerId=%d",
    		moduldId,timeType,msg.timerId);
			pc_test_send_msg(RRC_UECC_MODULE_ID,RRC_UECC_MODULE_ID,TIMER_MESSAGE_TO_PC_UECC,
			&msg,sizeof(timer_info_t));			
			break;

		case RRC_CSC_MODULE_ID:
			pCscTimerData = (csc_timer_buf_t *)pdata;
			msg.timerId = pCscTimerData->target_submodule;
			msg.msgType = timeType;			
			RRC_TRACE(RRC_INFO, "pc_test_forward_timer_msg:moduldId=%d,timeType=%d,timerId=%d",
    		moduldId,timeType,msg.timerId);
			pc_test_send_msg(RRC_OAMH_MODULE_ID,RRC_OAMH_MODULE_ID,TIMER_MESSAGE_TO_PC_CSC,
			&msg,sizeof(timer_info_t));					
			break;

		case RRC_LLIM_MODULE_ID:
			pLlimTimerData = (rrc_llim_timer_buf_t *)pdata;
			msg.timerId = pLlimTimerData->target_submodule;
			msg.msgType = timeType;			
			RRC_TRACE(RRC_INFO, "pc_test_forward_timer_msg:moduldId=%d,timeType=%d,timerId=%d",
    		moduldId,timeType,msg.timerId);
			pc_test_send_msg(RRC_OAMH_MODULE_ID,RRC_OAMH_MODULE_ID,TIMER_MESSAGE_TO_PC_LLIM,
			&msg,sizeof(timer_info_t));	
			break;
			
		default:
			break;
	}
	return;
}

#endif

