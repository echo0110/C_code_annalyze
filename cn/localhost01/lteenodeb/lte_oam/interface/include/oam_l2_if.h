/*****************************************************************************
 * File Name   : oam_l2_if.h
 *
 * Description : This file contains OAM L2 Function Prototypes and typedefs.
 *
 * Revision History:
 *
 * Date		   Author		  Reference	      Comments
 * -----	   ------		  ---------	      --------
 * May,2012	   Harsh Parmar	         OAM Design Document  Initial Version
 * Sept,2013       Raveen Sharma         CSR 51606 Fix        Code Changes for 
 *                                                            CSR Fix
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ****************************************************************************/
#ifndef L2_HEADER_H
#define L2_HEADER_H

/***************************************************************************
 *    INCLUDES
 ***************************************************************************/

#include <oam_types.h>
#include <oam_defines.h>
#include <oam_proto.h>
#include <lteMacOAMInterface.h>


/***************************************************************************
 *    MACROS
 ***************************************************************************/
#define  OAM_MOD_GBR_BKT_DEPTH_FACTOR                       1
#define  OAM_MOD_AMBR_BKT_DEPTH_FACTOR                      2 
#define  OAM_MOD_TOKEN_PERIODICITY                          4 
#define  OAM_MOD_QOS_STRATEGY_OVERALLOC_FACTOR              8  
#define  OAM_MOD_DL_QOS_STRATEGY_WEIGHT                     16
#define  OAM_MOD_UL_QOS_STRATEGY_WEIGHT                     32   
#define  OAM_MOD_ENABLE_ALLOC_AFTER_ALLOWED_BITRATE         64 

/*CSR_00057386*/
#define MAC_STATUS_ARR_SIZE             255
#define MAC_2D_ARR_SIZE                 2000 

#define L2_INTF_API_HEADER_SIZE   12
#define MAC_MIN_LOG_CAT            0
/*CSR:00055317_FIX_START*/
#define MAC_MAX_LOG_CAT           32 /* SPR 55317 */ 
/*CSR:00055317_FIX_END*/
#define LTE_SRB_LC_ID_LOW_BOUND    1
#define LTE_SRB_LC_ID_HIGH_BOUND   2
#define LTE_MAX_LC_ID              10
#define LTE_DRB_LC_ID_LOW_BOUND    3
#define LTE_DRB_LC_ID_HIGH_BOUND   10
/*CSR:00055317_FIX_START*/
#define PDCP_MIN_LOG_CAT           43 /* SPR 55317 */ 
#define PDCP_MAX_LOG_CAT           46 
/*CSR:00055317_FIX_END*/
#define MAC_MESSAGE_API_START      1
#define RRC_MIN_EXT_MODULE_ID      1
#define RRC_OAM_MODULE_ID          (RRC_MIN_EXT_MODULE_ID + 0)




#define ACTIVE_USER_SAMPLING_TIMER_TTI 100

/*CSR:00055317_FIX_START*/
#define RLC_MIN_LOG_CAT  33 /* SPR 55317 */ 
#define RLC_MAX_LOG_CAT  42

#define PR_EGTPU_MIN_LOG_CAT 46 /* SPR 55317 */
#define PR_EGTPU_MAX_LOG_CAT 50
/*CSR:00055317_FIX_END*/
#define EGTPU_SUCCESS              0

UInt8 time_buffer[TIME_SIZE];
UInt16 kpiToGetBitMap;
/***************************************************************************
 *    Extern Global Variable 
 ***************************************************************************/
extern UInt16 kpiToReportBitMap;
extern UInt8  kpiPdcpGetBitMap;

/***************************************************************************
 *    FUNCTION PROTOTYPES
 ***************************************************************************/
/*Prototypes of MAC & PDCP */
oam_return_et
oam_send_mac_init_layer_req
(
   oam_mac_init_req_params_t *p_mac_init_req,
   UInt16 src_module_id,
   UInt16 transaction_id,
   UInt8 cell_index,
   oam_error_code_et *p_error_code
);

oam_return_et
oam_send_mac_init_cell_req
(
    oam_mac_init_cell_req_params_t *p_mac_init_cell_req,
    oam_counter_t src_module_id,
    oam_counter_t transaction_id,
    oam_error_code_et *p_error_code,
    UInt8 cell_index
);

oam_return_et
oam_send_mac_ue_status_req
(
    oam_mac_ue_status_req_t *p_mac_ue_status_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_mac_get_stats_req
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_mac_reset_stats_req
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_mac_send_set_log_level_req
(
    oam_mac_log_level_req_t *p_mac_set_log_level_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_mac_send_get_log_level_req
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_mac_send_set_log_category_req
(
    oam_mac_enable_log_category_req_t *p_mac_set_log_category_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_mac_send_get_log_category_req
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_mac_send_disable_log_category_req
(
    oam_mac_disable_log_category_req_t *p_mac_disable_log_category_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_mac_modify_layer_req
(
    oam_mac_modify_req_params_t *p_mac_modify_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_mac_get_kpi_req
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_mac_config_kpi_stats_req
(
    oam_mac_config_KPI_stats_req_t *p_mac_config_kpi_stats_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code,
    /*SPR 21941 Start*/
    UInt8 l2_instance
    /*SPR 21941 End*/
);

oam_return_et
oam_mac_get_kpi_stats_req
(
    oam_mac_get_KPI_stats_req_t *p_mac_get_kpi_stats_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_mac_send_mac_ue_sinr_ta_req
(
    UInt16 ueIndex,
    UInt16    src_module_id,
    UInt16    transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_mac_reconfig_scheduler_params
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_pdcp_init_layer_req
(
    oam_pdcp_init_layer_req_t *p_pdcp_init_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_pdcp_ue_status_req
(
    oam_pdcp_status_req_t *p_pdcp_ue_status_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_get_pdcp_stats_req
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_pdcp_reset_stats_req
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_pdcp_send_set_log_level_req
(
    oam_pdcp_log_level_req_t *p_pdcp_set_log_level_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_pdcp_send_get_log_level_req
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_pdcp_send_disable_log_category_req
(
    oam_pdcp_disable_log_category_req_t *p_pdcp_disable_log_category_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_pdcp_send_get_log_category_req
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et  *p_error_code
);

oam_return_et
oam_pdcp_send_set_log_category_req
(
    oam_pdcp_enable_log_category_req_t *p_pdcp_set_log_category_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et  *p_error_code
);

oam_return_et
oam_pdcp_config_kpi_stats_req
(
    oam_pdcp_config_KPI_stats_req_t *p_pdcp_config_kpi_stats_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code,
    /*SPR 21941 Start*/
    UInt8 l2_instance
    /*SPR 21941 End*/
);

oam_return_et
oam_pdcp_get_kpi_stats_req
(
    oam_pdcp_get_KPI_stats_req_t *p_pdcp_get_kpi_stats_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);


oam_return_et
oam_handle_mac_init_layer_cnf
(
    oam_mac_init_resp_params_t *p_mac_init_resp,
    void* p_api_buf
);

oam_return_et
oam_handle_mac_init_cell_cnf
(
    oam_mac_init_resp_params_t  *p_mac_init_cell_resp,
    void* p_api_buf
);

oam_return_et
oam_handle_mac_modify_layer_cnf
(
    void* p_api_buf

);

oam_return_et
oam_handle_mac_get_kpi_cnf
(
    void* p_api_buf
);

oam_return_et
oam_handle_mac_get_stats_cnf
(
    void* p_api_buf,
   /*SPR 17777 fix*/
    UInt16 transaction_id,
    UInt16 dest_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_handle_mac_reset_stats_cnf
(
    void* p_api_buf,
    UInt16 transaction_id,
    UInt16 dest_id
);

oam_return_et
oam_handle_mac_get_status_cnf
(
    void* p_api_buf
);

oam_return_et
oam_mac_get_loglevel_resp
(
    void* p_api_buf
   /*SPR 17777 fix*/

);

oam_return_et
oam_mac_get_logcategory_resp
(
    void* p_api_buf,
    oam_error_code_et *p_error_code
);
oam_return_et
oam_mac_kpi_stat_ind
(
    void* p_api_buf
);

oam_return_et
oam_mac_kpi_thp_stats_ind
(
    void* p_api_buf
);

oam_return_et
oam_mac_get_kpi_thp_stats_cnf
(
    void* p_api_buf
);

oam_return_et
oam_mac_get_kpi_stat_conf
(
    oam_mac_KPI_stats_ind_t *p_oam_mac_get_kpi_stats_conf,
    void* p_api_buf,
    oam_error_code_et  *p_error_code
);

oam_return_et
oam_mac_config_kpi_stat_conf
(
    oam_mac_config_KPI_stats_conf_t *p_oam_mac_config_kpi_stats_conf,
    void* p_api_buf,
    oam_error_code_et  *p_error_code
);

oam_return_et
oam_mac_get_mac_ue_sinr_ta_resp
(
    void* p_api_buf,
    oam_error_code_et *p_error_code
);


oam_return_et
oam_handle_pdcp_init_layer_cnf
(
    void *p_api_buf

);

oam_return_et
oam_handle_pdcp_reset_stats_cnf
(
    void *p_api_buf
);

oam_return_et
oam_handle_pdcp_get_stats_cnf
(
    void* p_api_buf,
    /*SPR 17777 fix*/
    UInt16 transaction_id,
    UInt16 dest_id,
    oam_error_code_et *p_error_code

);

oam_return_et
oam_handle_pdcp_get_status_cnf
(
    void* p_api_buf
);

oam_return_et
oam_pdcp_get_loglevel_resp
(
    void* p_api_buf
    /*SPR 17777 fix*/
);
/*SPR 19771 FIXED START*/
oam_return_et
oam_pdcp_get_logcategory_resp
(
    void* p_api_buf,
    oam_error_code_et *p_error_code
    /*SPR 17777 fix*/

);
/*SPR 19771 FIXED END*/

oam_return_et
oam_pdcp_config_kpi_stat_conf
(
    oam_pdcp_config_KPI_stats_conf_t *p_oam_pdcp_config_kpi_stats_conf,
    void* p_api_buf,
    oam_error_code_et  *p_error_code
);

oam_return_et
oam_pdcp_kpi_stat_ind
(
    void* p_api_buf
);

oam_return_et
oam_pdcp_kpi_thp_stats_ind
(
    void* p_api_buf
);

oam_return_et
oam_pdcp_get_kpi_stat_conf
(
    oam_pdcp_KPI_stats_ind_t  *p_oam_pdcp_get_kpi_stats_conf,
    void* p_api_buf,
    oam_error_code_et  *p_error_code
);

oam_return_et
oam_pdcp_get_kpi_thp_stats_cnf
(
    void* p_api_buf
);

oam_return_et
oam_send_rlc_init_layer_req
(
    oam_rlc_init_layer_req_t *p_oam_rlc_init_req,
    UInt16                    src_module_id,
    UInt16                    transaction_id,
    UInt8                     cell_index,
    oam_error_code_et        *p_error_code
);
oam_return_et
oam_handle_rlc_init_layer_cnf
(
    void*              p_api_buf
);

oam_return_et
oam_send_rlc_ue_status_req
(
    oam_rlc_ue_status_req_t *p_oam_rlc_ue_status_req,
    UInt16                   src_module_id,
    UInt16                   transaction_id,
    oam_error_code_et       *p_error_code
);

oam_return_et
oam_rlc_get_loglevel_resp
(
                   void* p_api_buf
    /*SPR 17777 fix*/
);
oam_return_et
oam_rlc_get_logcategory_resp
(
    void* p_api_buf,
    UInt16 msg_len,
    UInt16 transaction_id,
    UInt16 dest_id,
    oam_error_code_et *p_error_code
);
oam_return_et
oam_rlc_send_get_log_category_req
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);
oam_return_et
oam_pr_egtpu_send_get_log_category_req
(
 oam_counter_t src_module_id,  /* Source module identifier */
 oam_counter_t transaction_id,/* Interface transaction identifier */
oam_error_code_et *p_error_code
);
oam_return_et
oam_rlc_send_get_log_level_req
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);
oam_return_et
oam_rlc_send_set_log_level_req
(
    Oam_LogLevel_Req_t *p_Oam_rlc_set_log_level_req,
    UInt16             src_module_id,
    UInt16             transaction_id,
    oam_error_code_et  *p_error_code
);
oam_return_et
oam_rlc_send_set_log_category_req
(
    Oam_LogCategory_Req_t *p_rlc_set_log_category_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);
oam_return_et
oam_rlc_send_disable_log_category_req
(
    Oam_LogCategory_Req_t *p_rlc_disable_log_category_req,
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);


oam_return_et
oam_egtpu_get_loglevel_resp
(
    void* p_api_buf
    /*SPR 17777 fix*/
);
oam_return_et
oam_send_egtpu_initdb_req
(
    oam_egtpu_initdb_req_t *p_egtpu_initdb_req_t,
    oam_counter_t src_module_id,
    oam_counter_t transaction_id,
    oam_error_code_et *p_error_code
);
oam_return_et
oam_handle_egtpu_init_layer_cnf
(
    void* p_api_buf
);
oam_return_et
oam_pr_egtpu_get_logcategory_resp
(
    void* p_api_buf,
    UInt16 transaction_id,
    UInt16 dest_id,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_handle_egtpu_set_trace_level_cnf(
void                  *p_api_buf,
    /*SPR 17777 fix*/
oam_error_code_et *p_error_code
);
oam_return_et
oam_handle_egtpu_get_trace_level_cnf(
void                  *p_api_buf,
    /*SPR 17777 fix*/
oam_error_code_et *p_error_code
);
oam_return_et
oam_egtpu_get_trace_level_req(
oam_relay_Get_Trace_Lvl_Req_t *p_egtpu_get_trace_level_req,
oam_counter_t                 src_module_id,
oam_counter_t                 transaction_id,
oam_error_code_et *p_error_code
);
oam_return_et
oam_set_trace_level_req(
    oam_relay_Set_Trace_Lvl_Req_t *p_egtpu_set_trace_level_req,
    oam_counter_t                  src_module_id,
    oam_counter_t                  transaction_id,
    oam_error_code_et    *p_error_code
);
oam_return_et
oam_egtpu_get_log_level_req(
        oam_relay_Gtpu_Get_Log_Lvl_Req_t  *p_egtpu_get_log_level_req,
        oam_counter_t                 src_module_id,
        oam_counter_t                 transaction_id,
        oam_error_code_et *p_error_code
        );
oam_return_et
oam_egtpu_set_log_level_req(
        oam_relay_Gtpu_Set_Log_Lvl_Req_t *p_egtpu_set_log_level_req,
        oam_counter_t                  src_module_id,
        oam_counter_t                  transaction_id,
        oam_error_code_et    *p_error_code
        );
oam_return_et
oam_egtpu_set_log_cat_enable_req
(
    void* p_api_buf,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);
oam_return_et
oam_egtpu_set_log_cat_disable_req
(
    void* p_api_buf,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);
oam_return_et
oam_handle_rlc_get_stats_cnf
(            
    void* p_api_buf,
    /*SPR 17777 fix*/
    UInt16 transaction_id,
    UInt16 dest_id,
    oam_error_code_et *p_error_code
            
);

oam_return_et
oam_send_mac_cleanup_req
(
    oam_counter_t src_module_id,
    oam_counter_t transaction_id,
    /*SPR 21369 Start*/
    UInt8 cell_indx,
    /*SPR 21369 End */
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_pdcp_cleanup_req
(
    oam_counter_t src_module_id,
    oam_counter_t transaction_id,
    /*SPR 21369 Start*/
    UInt8 cell_indx,
    /*SPR 21369 End */
    oam_error_code_et *p_error_code
);

oam_return_et
oam_rlc_send_cleanup_req
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);


oam_return_et
oam_handle_mac_get_ue_status_cnf
(
 void* p_api_buf,
 UInt16 msg_len,
 UInt16 transaction_id,
 oam_error_code_et *p_error_code
 );

oam_return_et
oam_handle_pdcp_get_ue_status_cnf
(
 void* p_api_buf,
 UInt16 msg_len,
 UInt16 transaction_id,
 oam_error_code_et *p_error_code
 );

oam_return_et
oam_send_mac_start_phy_cnf
(
    UInt16 src_module_id,
    UInt16 transaction_id,
    oam_error_code_et *p_error_code
);
/*CSR_00057386*/
oam_return_et
oam_mac_get_ue_status_resp
(
    void* p_api_buf,
    oam_error_code_et *p_error_code

);


oam_return_et
oam_mac_get_ue_status_req
(
    oam_mac_ue_status_req_t *p_mac_ue_status_req,
    oam_counter_t src_module_id,
    oam_counter_t transaction_id,
    oam_error_code_et *p_error_code
);


oam_return_et
oam_handle_rlc_get_ue_status_cnf
(
 void* p_api_buf,
 UInt16 msg_len,
 UInt16 transaction_id,
 oam_error_code_et *p_error_code
 );
oam_return_et
oam_send_pdcp_get_perf_stats_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 );
/*CSR 00053551 FIX START*/ 
#define EGTPU_STATS_PROTO_MSG_SIZE 12
#define EGTPU_STATS_API_SIZE 38
#define EGTPU_STATS_ERRORS_SIZE 10
#define EGTPU_STATS_INTERNAL_SIZE 1
#define EGTPU_STATS_ERR_PROTO_MSG_SIZE 3
#define MAX_EGTPU_STATS_STR_SIZE 256
oam_return_et
oam_send_egtpu_stats_req
(
    relayGtpuGetStatsReq *p_egtpu_stats_req,
    oam_counter_t src_module_id,
    oam_counter_t transaction_id,
    oam_error_code_et *p_error_code
);

/*CSR 00053551 FIX END*/ 


typedef enum __oam_mac_return_et {
	OAM_MAC_FAILURE,
	OAM_MAC_SUCCESS,
	OAM_MAC_PARTIAL_SUCCESS
} oam_mac_return_et;


typedef enum __oam_rlc_return_et {
	OAM_RLC_FAILURE,
	OAM_RLC_SUCCESS,
	OAM_RLC_PARTIAL_SUCCESS
} oam_rlc_return_et;
/*spr 22474 start*/
oam_return_et
oam_send_mac_get_debug_info_req
(
 UInt16 debugType,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id
 );

oam_return_et
oam_send_rlc_get_debug_info_req
(
 UInt16 debugType,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id
 );

oam_return_et
oam_send_pdcp_get_debug_info_req
(
 UInt16 debugType,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id
 );

oam_return_et
oam_send_gtpu_get_debug_info_req
(
 UInt16 debugType,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id
 );

oam_return_et
oam_handle_mac_get_debug_info_cnf
(
 void* p_api_buf,
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
);

UInt16  
oam_get_mac_debug_info_prints
(           
 MacGetDebugInfoRespParams* p_mac_resp,
 UInt8*** str_arr
);

oam_return_et
oam_handle_rlc_get_debug_info_cnf
(
 void* p_api_buf,
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
);

UInt16  
oam_get_rlc_debug_info_prints
(           
 RlcGetDebugInfoRespParams* p_resp,
 UInt8*** str_arr
);

oam_return_et
oam_handle_pdcp_get_debug_info_cnf
(
 void* p_api_buf,
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
);

UInt16
oam_get_pdcp_debug_info_prints
(           
 PdcpGetDebugInfoRespParams* p_resp,
 UInt8*** str_arr
);

oam_return_et
oam_handle_gtpu_get_debug_info_cnf
(
 void* p_api_buf,
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
);

UInt16  
oam_get_gtpu_debug_info_prints
(           
 GtpuGetDebugInfoRespParams* p_resp,
 UInt8*** str_arr
);
oam_return_et
oam_send_egtpu_get_debug_info_req
(
 UInt16 debugType,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id
 );

/*spr 22474 end*/

#endif /* L2_HEADER_H */
