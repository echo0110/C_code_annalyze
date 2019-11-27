/******************************************************************************
*
*   FILE NAME:
*       rrc_oamh_intf.h
*
*   DESCRIPTION:
*       This file contains types used for representation of OAMH_IF API
*       inside RRC
*       Based on LTE_RRC_DD_v0.2.doc.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   11 May 2009     Pradeep S       ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_OAMH_INTF_H_
#define _RRC_OAMH_INTF_H_

#include "rrc_defines.h"
#include "rrc_oam_intf.h"

#pragma pack(push, 1)
/******************************************************************************
*   Common types
******************************************************************************/
typedef struct
{
    U8      result;
} rrc_oamh_prov_resp_t;


/******************************************************************************
*   UECC_OAMH_PROV_REQ
******************************************************************************/

typedef struct _rrc_oam_supported_security_algo_t
{
    rrc_cell_index_t                    cell_index;

    rrc_oam_integrity_algorithms_t      integrity_algorithms;

    rrc_oam_ciphering_algorithms_t      ciphering_algorithms;

} rrc_oam_supported_security_algo_t;

typedef struct _rrc_oam_supported_security_algo_per_cell_t
{
    U8                                       num_cells;

    rrc_oam_supported_security_algo_t  supported_security_algorithms[MAX_NUM_CELLS];

} rrc_oam_supported_security_algo_per_cell_t;

#define RRC_UECC_INTRA_CELL_HO_GUARD_TIMER_PRESENT       0x01
#define RRC_UECC_UE_RECONFIG_TIMER_PRESENT               0x02
#define RRC_UECC_COUNTER_CHECK_TIMER_PRESENT             0x04
#define RRC_UECC_UE_INFORMATION_TIMER_PRESENT            0x08
#define RRC_UECC_STORE_UE_CONTEXT_TIMER_PRESENT          0x10
#define RRC_UECC_CCO_GUARD_TIMER_PRESENT                 0x20
#define RRC_UECC_HEALTH_MONITORING_TIMER_PRESENT         0x40

/*NR_DC Code Change Start*/
#define RRC_UECC_DC_PREP_TIMER_PRESENT                   0x80
#define RRC_UECC_DC_BEARER_CHANGE_GUARD_PRESENT          0x100
/*NR_DC Code Change Stop*/
typedef struct _rrc_uecc_timers_info_t
{
    U16             bitmask;

    U16             uecc_intra_cell_ho_guard_timer;

    U16             uecc_ue_reconfig_timer;

    U16             uecc_counter_check_timer;

    U16             uecc_ue_information_timer;

    U16             uecc_store_ue_context_timer;

    U16             uecc_cco_guard_timer;

    U16             uecc_health_monitor_timer_value;

/*NR_DC Code Change Start*/
    U16             uecc_dc_prep_timer;
    U16             uecc_dc_bearer_change_guard_timer;
/*NR_DC Code Change Stop*/
} rrc_uecc_timers_info_t;

#define RRC_UECC_LOG_FILE_NAME_PRESENT       0x01
#define RRC_UECC_TIMERS_INFO                 0x02
#define RRC_UECC_NETWORK_INDEX_PRESENT       0x04
#define RRC_UECC_INTEGRITY_THRESHOLD_COUNTER_PRESENT    0x08
#define RRC_UECC_INTEGRITY_TIMER_VALUE_PRESENT          0x10
#define RRC_UECC_UE_CONTEXT_LIST_SIZE_PRESENT           0x20
#define RRC_UECC_SHARED_MEM_INDEX_PRESENT               0x40
typedef struct _rrc_uecc_oamh_prov_req_t
{
    U8                                               bitmask;
    rrc_oam_supported_security_algo_per_cell_t       supported_security_algo_per_cell;
    rrc_oam_uecc_timer_info_t                        timer_info;
    U8                                               len_vendor_name;
    U8                                               vendor_name[RRC_VENDOR_NAME_LEN];
    U16                                              total_ue_supported;
    S8                                               log_file_name[MAX_FILE_NAME_SIZE];
    rrc_uecc_timers_info_t                           uecc_timers_info;
    U16                                              network_index;
    U16                                              shared_mem_index;
    /* SPR 4118 Start */
    U16        integrity_threshold_counter;
    /*^ O, RRC_UECC_INTEGRITY_THRESHOLD_COUNTER_PRESENT, N, 0, 0 ^*/
    U16        uecc_integrity_timer_value;
    /*^ O, RRC_UECC_INTEGRITY_TIMER_VALUE_PRESENT, N, 0, 0 ^*/
    /* SPR 4118 Stop */
    U8        ue_context_list_size;
    /*^ O, RRC_UECC_UE_CONTEXT_LIST_SIZE_PRESENT, N, 0, 0 ^*/

} rrc_uecc_oamh_prov_req_t;

/**************************************************************ma****************
*   UECC_OAMH_PROV_RESP
******************************************************************************/

/******************************************************************************
*   UECCMD_OAMH_PROV_REQ
******************************************************************************/
typedef struct _rrc_ueccmd_oamh_prov_req_t
{
    U8                          no_of_uecc_instances;

    U8                          rest_rrc_core_no;

    U8                          core_no[MAX_UECC_INSTANCES];

    rrc_uecc_oamh_prov_req_t    uecc_oamh_prov_req;
} rrc_ueccmd_oamh_prov_req_t;

/******************************************************************************
*   UECCMD_OAMH_PROV_RESP
******************************************************************************/

typedef rrc_oamh_prov_resp_t rrc_uecc_oamh_prov_resp_t;

/**************************************************************ma****************
*   UECCMD_OAMH_PROV_RESP
******************************************************************************/
typedef rrc_oamh_prov_resp_t rrc_ueccmd_oamh_prov_resp_t;

/******************************************************************************
*   UECC_OAMH_GET_CELL_STATUS_REQ
******************************************************************************/
typedef struct _rrc_uecc_oamh_get_cell_status_req_t
{
    rrc_transaction_id_t    transaction_id;

    rrc_cell_index_t        cell_index;     /* MAX_NUM_CELLS - 1 */
} rrc_uecc_oamh_get_cell_status_req_t;

/******************************************************************************
*   UECC_OAMH_GET_CELL_STATUS_RESP
******************************************************************************/
#define UECC_OAMH_CELL_STATUS_PRESENT           0x01

typedef struct _rrc_uecc_oamh_get_cell_status_resp_t
{
    rrc_transaction_id_t    transaction_id;

    U16                     bitmask;    /*^ BITMASK ^*/

    rrc_cell_index_t        cell_index; /* MAX_NUM_CELLS - 1 */
    U8                      response;   /* rrc_return_et */
    rrc_oam_cell_status_t   cell_status;/*^ O, UECC_OAMH_CELL_STATUS_PRESENT ^*/
} rrc_uecc_oamh_get_cell_status_resp_t;

/******************************************************************************
*   UECC_OAMH_GET_UE_STATUS_REQ
******************************************************************************/
#define UECC_OAMH_UE_INDEX_PRESENT          0x01

typedef struct _rrc_uecc_oamh_get_ue_status_req_t
{
    rrc_transaction_id_t    transaction_id;

    U16                     bitmask;    /*^ BITMASK ^*/

    rrc_cell_index_t        cell_index; /* MAX_NUM_CELLS - 1 */
    rrc_ue_index_t          ue_index;   /*^ O, UECC_OAMH_UE_INDEX_PRESENT ^*/
} rrc_uecc_oamh_get_ue_status_req_t;

/******************************************************************************
*   UECC_OAMH_GET_UE_STATUS_RESP
******************************************************************************/
#define UECC_OAMH_UE_STATUS_LIST_PRESENT    0x01

typedef struct _rrc_uecc_oamh_get_ue_status_resp_t
{
    rrc_transaction_id_t        transaction_id;

    U16                         bitmask;        /*^ BITMASK ^*/

    rrc_cell_index_t            cell_index;     /* MAX_NUM_CELLS - 1 */
    U8                          response;       /* rrc_return_et */
    rrc_oam_ue_status_list_t    ue_status_list;
/*^ O, UECC_OAMH_UE_STATUS_LIST_PRESENT ^*/

} rrc_uecc_oamh_get_ue_status_resp_t;

/******************************************************************************
 * *   UECC_OAMH_GET_PS_STATS_REQ
 * ******************************************************************************/

typedef struct _rrc_uecc_oamh_get_ps_stats_req_t
{
    rrc_transaction_id_t    transaction_id;
    
    rrc_cell_index_t        cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/     /* MAX_NUM_CELLS - 1 */

} rrc_uecc_oamh_get_ps_stats_req_t; /*^ API, RRC_OAM_GET_PS_STATS_REQ ^*/

/******************************************************************************
 * *   UECC_OAMH_GET_PS_STATS_RESP
 * ******************************************************************************/

typedef struct _rrc_uecc_oamh_get_ps_stats_resp_t
{
    rrc_transaction_id_t        transaction_id;
    
    rrc_cell_index_t        cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/     /* MAX_NUM_CELLS - 1 */
    U8                          response;       /* rrc_return_et */
    
    oam_rrc_ps_stats_resp_t  ps_stats_resp;  
/*^ M, 0, OCTET_STRING, VARIABLE ^*/ 
} rrc_uecc_oamh_get_ps_stats_resp_t; /*^ API, RRC_OAM_GET_PS_STATS_REQ ^*/

/******************************************************************************
*   UECC_OAMH_CELL_TRAFFIC_TRACE_START
******************************************************************************/
typedef struct _rrc_uecc_oamh_cell_traffic_trace_start_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index; /* MAX_NUM_CELLS - 1 */
    U8                      interface_to_trace; 
    U8                      trace_depth;
    rrc_oam_trace_collection_entity_t trace_collection_entity;
    U8                      traceRef[TRACE_REF_LENGTH];
} rrc_uecc_oamh_cell_traffic_trace_start_t;

/******************************************************************************
*   UECC_OAMH_CELL_TRAFFIC_TRACE_STOP
******************************************************************************/
typedef struct _rrc_uecc_oamh_cell_traffic_trace_stop_t
{
    rrc_cell_index_t        cell_index; /* MAX_NUM_CELLS - 1 */
} rrc_uecc_oamh_cell_traffic_trace_stop_t;

/******************************************************************************
*   UECC_OAMH_CELL_TRAFFIC_TRACE_START_ERROR_IND
******************************************************************************/
typedef struct _rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index; /* MAX_NUM_CELLS - 1 */
    U8                      cause; 
} rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t;

/******************************************************************************
*   UECC_OAMH_TRACE_START_ERROR_IND
******************************************************************************/
typedef struct _rrc_uecc_oamh_trace_start_error_ind_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index; /* MAX_NUM_CELLS - 1 */
    rrc_trace_failure_list_t  trace_failure_list;
} rrc_uecc_oamh_trace_start_error_ind_t;

/******************************************************************************
*   UECC_OAMH_TRACE_FILE_TRANSFER_START_IND
******************************************************************************/
typedef struct _rrc_uecc_oamh_trace_file_transfer_start_ind_t
{
    rrc_cell_index_t        cell_index; /* MAX_NUM_CELLS - 1 */
    rrc_ue_index_t          ue_index; 
    rrc_oam_e_utran_trace_id_t e_utran_trace_id;
    rrc_oam_trace_file_t trace_file;
    rrc_oam_trace_collection_entity_t trace_collection_entity;
} rrc_uecc_oamh_trace_file_transfer_start_ind_t;

/******************************************************************************
*   UECC_OAMH_TRACE_FILE_TRANSFER_COMPLETE_IND
******************************************************************************/
typedef struct _rrc_uecc_oamh_trace_file_transfer_complete_ind_t
{
    rrc_cell_index_t        cell_index; /* MAX_NUM_CELLS - 1 */
    rrc_oam_trace_file_t trace_file;
} rrc_uecc_oamh_trace_file_transfer_complete_ind_t;

/******************************************************************************
*   CSC_OAMH_PROV_REQ
******************************************************************************/

#define RRC_CSC_HEALTH_MONITORING_TIMER_PRESENT         0x01
typedef struct _rrc_csc_oamh_prov_req_t
{
    rrc_oam_csc_timer_info_t    oam_csc_timer_info;
    rrc_sfn_t                   sfn_offset;
    U8                          max_num_supported_cells;
    U8                          csc_cell_index_status;
    U16                         max_num_supported_ue_per_cell[MAX_NUM_CELLS];
    U8                          max_cell_reconfig_retry_count ;
    rrc_bool_t                  null_integrity_support[MAX_NUM_CELLS];
    U8                          no_of_uecc_instances;
    U16                         bitmask;
    U16                         csc_health_monitor_timer_value;
}rrc_csc_oamh_prov_req_t;

/******************************************************************************
*   CSC_OAMH_PROV_RESP
******************************************************************************/
typedef rrc_oamh_prov_resp_t        rrc_csc_oamh_prov_resp_t;

/******************************************************************************
*   LLIM_OAMH_PROV_REQ
******************************************************************************/
#define RRC_LLIM_HEALTH_MONITORING_TIMER_PRESENT         0x01
typedef struct _rrc_llim_oamh_prov_req_t
{
    U8                          no_of_uecc_instances;

    U16                         max_num_supported_ue_per_cell[MAX_NUM_CELLS];

    rrc_oam_llim_timer_info_t   timer_info;

    U16                         bitmask;

    U16                         llim_health_monitor_timer_value;

} rrc_llim_oamh_prov_req_t;

/******************************************************************************
*   LLIM_OAMH_PROV_RESP
******************************************************************************/
typedef rrc_oamh_prov_resp_t        rrc_llim_oamh_prov_resp_t;

/******************************************************************************
*   CSC_OAMH_CELL_START_IND
******************************************************************************/
typedef struct _rrc_csc_oamh_cell_start_ind_t
{
    rrc_cell_index_t        cell_index;     /* MAX_NUM_CELLS - 1 */
}rrc_csc_oamh_cell_start_ind_t; 

/******************************************************************************
*   CSC_OAMH_CELL_STOP_IND
******************************************************************************/
typedef struct _rrc_csc_oamh_cell_stop_ind_t
{ 
    rrc_cell_index_t        cell_index;     /* MAX_NUM_CELLS - 1 */
} rrc_csc_oamh_cell_stop_ind_t;

/******************************************************************************
*   RRC_UECC_PROC_STATUS_IND
******************************************************************************/
typedef struct _rrc_uecc_proc_status_ind_t
{
    rrc_cell_index_t        cell_index;     /* MAX_NUM_CELLS - 1 */
    U8                      result;
} rrc_uecc_proc_status_ind_t;

/* lipa start */
/* This api will be used for notifying ueccmd about the addition of LGW node 
 * and its IP address and deletion of configured LGW node 
 */
/******************************************************************************
*  UECCMD_OAMH_ADD_DEL_LGW_IND
******************************************************************************/
#define RRC_OAM_ADD_LGW_ADDR_PRESENT   0x01
typedef struct _ueccmd_oamh_add_del_lgw_ind_t
{
   U8 bitmask ;
   U8 data_length; 
   /*^ O,RRC_OAM_ADD_LGW_ADDR_PRESENT,OCTET_STRING,FIXED ^*/
   U8 lgw_addr[RRC_MAX_IP_ADDR_LEN_IPV6]; 
   /*^ O,RRC_OAM_ADD_LGW_ADDR_PRESENT,OCTET_STRING,FIXED ^*/
}ueccmd_oamh_add_del_lgw_ind_t;

typedef ueccmd_oamh_add_del_lgw_ind_t uecc_oamh_add_del_lgw_ind_t;
/* lipa end */
/* SPR 20908 Start */
/******************************************************************************
*   CSC_OAMH_OAM_COMMUNICATION_INFO_IND
******************************************************************************/
typedef struct _rrc_oamh_communication_info_ind_t
{
    rrc_cell_index_t         cell_index;
    rrc_l2_instance_t        l2_instance_id;
    rrc_oam_ext_comm_info_t  ext_comm_info_lower_layer[RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES];
} rrc_oamh_communication_info_ind_t;

typedef struct _rrc_oamh_communication_info_resp_t
{
    rrc_cell_index_t         cell_index;
    rrc_bool_et              result;
} rrc_oamh_communication_info_resp_t;
/* SPR 20908 End */

#pragma pack(pop)

#endif /* _RRC_OAMH_INTF_H_ */

