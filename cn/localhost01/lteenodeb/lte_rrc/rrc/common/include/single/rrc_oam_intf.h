/*********************************************************************
 *
 *  FILE NAME   : rrc_oam_intf.h
 *
 *  DESCRIPTION : File contains the OAM interface API structures.
 *
 *  REVISION HISTORY :
 *
 *  DATE                Name            Reference        Comments
 *  may 11, 2009        Pankaj A        ---------        --------
 *
 *
 *  Copyright (c) 2009, Aricent Inc.
 *
 *******************************************************************/
#ifndef __RRC_OAM_INTF__
#define __RRC_OAM_INTF__

#include "rrc_defines.h"
#include "rrc_rrm_intf.h"
#include "lteTypes.h"

#define MAC_IP_ADDRESS_LENGTH                   4
#define MAX_EXT_MODULES                         9 
#define RRC_OAM_MAX_EXT_MODULES                 8 
#define MAX_LLIM_TIMER_INFO                     14
#define MAX_UECC_TIMER_INFO                     21
#define MAX_CSC_TIMER_INFO                      7
#define RRC_NUM_ESTABLISHMENT_CAUSE             8
#define RRC_NUM_RELEASE_CAUSE                   4
#define RRC_MAX_NUM_UE_STATUS                   100
/* CSR_00040799 Fix Start */
/* #define RRC_MAX_NUM_UE                          600 */
/* CSR_00040799 Fix End */
#define CIPHERING_ALGO_EEA0_BIT                 0
#define CIPHERING_ALGO_EEA1_BIT                 1
#define CIPHERING_ALGO_EEA2_BIT                 2

typedef enum
{
    OAM_TRANSPORT_MODE_TCP,
    OAM_TRANSPORT_MODE_UDP
} rrc_oam_transport_mode_et;

typedef enum
{
    RRC_OAM_NOT_CONNECTED,
    RRC_OAM_CONNECTED
} rrc_oam_connection_status_et;

typedef enum
{
    OAM_LOG_LEVEL_FATAL = LTE_LOG_CRITICAL,
    OAM_LOG_LEVEL_ERROR = LTE_LOG_ERROR,
    OAM_LOG_LEVEL_WARNING = LTE_LOG_WARNING,
    OAM_LOG_LEVEL_INFO = LTE_LOG_INFO,
    OAM_LOG_LEVEL_BRIEF = LTE_LOG_BRIEF,
    OAM_LOG_LEVEL_DETAILED = LTE_LOG_DETAILED,
    OAM_LOG_LEVEL_DETAILEDALL = LTE_LOG_DETAILEDALL
} rrc_oam_log_level_et;

typedef enum
{
    RRC_OAM_LOGGER_OPTION_CONSOLE,
    RRC_OAM_LOGGER_OPTION_FILE,
    RRC_OAM_LOGGER_OPTION_NETWORK,
    RRC_OAM_LOGGER_OPTION_SHARED_MEM
}rrc_oam_logger_option_et;

typedef enum
{
    OAM_LOG_OFF,
    OAM_LOG_ON
} rrc_oam_log_on_off_et;


typedef enum
{
    CSC_CELL_SETUP_TIMER,               /* 0 */
    LLIM_CELL_SETUP_TIMER,              /* 1 */
    UECC_RRC_CONN_SETUP_TIMER,          /* 2 */
    LLIM_CREATE_UE_ENTITY_TIMER,        /* 3 */
    UECC_RRC_CONN_RECONFIG_TIMER,       /* 4 */
    LLIM_RECONFIG_UE_ENTITY_TIMER,      /* 5 */
    UECC_RRC_CONN_RELEASE_TIMER,        /* 6 */
    LLIM_DELETE_UE_ENTITY_TIMER,        /* 7 */
    LLIM_CONFIG_UE_SECURITY_TIMER,      /* 8 */
    UECC_RRC_MME_GUARD_TIMER,           /* 9 */
    UECC_RRM_UE_CAPABILITY_ENQ_TIMER,   /* 10 */
    CSC_CELL_RECONFIG_TIMER,            /* 11 */
    LLIM_CELL_RECONFIG_TIMER,           /* 12 */
    CSC_CELL_DELETE_TIMER,              /* 13 */
    UECC_UE_CONTXT_MODIFY_TIMER,       /* 14 */
    UECC_ERAB_SETUP_TIMER,        /* 15 */
    UECC_ERAB_MODIFY_TIMER,       /* 16 */
    UECC_ERAB_RELEASE_CMD_TIMER,  /* 17 */
    UECC_ERAB_RELEASE_IND_TIMER,  /* 18 */
    UECC_RLF_WAIT_FOR_REEST_TIMER,      /* 19 */
    UECC_RLF_UE_SUSPEND_TIMER,          /* 20 */

    
/*REL 1.2:HANDOVER PROCEDURE START*/
    UECC_RRC_HO_PREP_TIMER,             /* 21 */
    UECC_RRC_HO_OVERALL_TIMER,          /* 22 */
    UECC_LL_GUARD_TIMER,                /* 23 */
    UECC_RRC_OFFSET_TIMER,              /* 24 */
    UECC_MEAS_CONFIG_GUARD_TIMER,       /* 25 */
    UECC_RRC_TRG_HO_GUARD_TIMER,        /* 26 */
/*REL 1.2:HANDOVER PROCEDURE END*/
    LLIM_SUSPEND_UE_ENTITY_TIMER, /* 27 */
    LLIM_REESTABLISH_UE_ENTITY_TIMER, /* 28 */
    LLIM_RECONFIG_SECURITY_UE_ENTITY_TIMER, /* 29 */
    LLIM_RESUME_UE_ENTITY_TIMER, /* 30 */
    UECC_CRE_GUARD_TIMER,  /* 31 */
    LLIM_CHANGE_CRNTI_TIMER, /* 32*/
    CSC_PWS_TIMER, /* 33*/
    CSC_KILL_TIMER, /* 34*/
    
    CSC_CELL_START_TIMER,               /* 35 */
    LLIM_CELL_START_TIMER,              /* 36 */
    
    CSC_CELL_STOP_TIMER,               /* 37 */
    LLIM_CELL_STOP_TIMER,              /* 38 */
    LLIM_CELL_DELETE_TIMER,              /* 39 */
    /*x2_ho_timer*/
    UECC_RRC_X2_HO_PREP_TIMER,             /* 40 */
    UECC_RRC_X2_HO_OVERALL_TIMER,          /* 41 */
    UECC_RRC_INTRA_CELL_HO_GUARD_TIMER,    /* 42 */
    UECC_RRM_UE_RECONFIG_TIMER,            /* 43 */
    /* SPR 4118 Start */
    UECC_RRC_INTEGRITY_FAILURE_TIMER,      /* 44 */
    /* SPR 4118 Stop */
    UECC_RRC_CCO_GUARD_TIMER,              /* 45 */
    UECC_RRC_COUNTER_CHECK_TIMER,          /* 46 */
    UECC_RRM_UE_INFORMATION_REQ_TIMER,     /* 47 */
    UECC_RRC_STORE_UE_CONTEXT_TIMER,       /* 48 */
    UECC_OAM_HEALTH_MONITOR_TIMER,         /* 49 */
    RRC_OAM_TIMER_ID_LAST
} rrc_oam_timer_id_et;

#pragma pack(push, 1)

/******************************************************************************
*   RRC_OAM_COMMUNICATION_INFO_REQ
******************************************************************************/
#define RRC_OAM_IP_ADDR_PRESENT         0x01

typedef struct _rrc_oam_ext_comm_info_t
{
    U16             bitmask;                        /*^ BITMASK ^*/

    U8              ext_module_id;
/*^ M, 0, B, 1, 8 ^*/ 

    U8              transport_mode;
/*^ M, 0, H, 0, 1 ^*/  /* rrc_oam_transport_mode_et */

    U8              ip_addr[MAC_IP_ADDRESS_LENGTH];
/*^ O, RRC_OAM_IP_ADDR_PRESENT, OCTET_STRING, FIXED ^*/

    U16             port;                           /*^ M, 0, N, 0, 0 ^*/
} rrc_oam_ext_comm_info_t;

typedef struct _rrc_oam_communication_info_req_t
{
    rrc_oam_ext_comm_info_t ext_comm_info[RRC_OAM_MAX_EXT_MODULES];
/*^ M, 0, OCTET_STRING, FIXED ^*/

} rrc_oam_communication_info_req_t; /*^ API, RRC_OAM_COMMUNICATION_INFO_REQ ^*/

/******************************************************************************
*   RRC_OAM_COMMUNICATION_INFO_RESP
******************************************************************************/
typedef struct _rrc_oam_ext_connection_status_t
{
    U8              ext_module_id;
/*^ M, 0, B, 1, 8 ^*/ 

    U8              connection_status;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_oam_connection_status_et */

} rrc_oam_ext_connection_status_t;

typedef struct _rrc_oam_communication_info_resp_t
{
    rrc_oam_ext_connection_status_t ext_connection_status[RRC_OAM_MAX_EXT_MODULES];
/*^ M, 0, OCTET_STRING, FIXED ^*/

} rrc_oam_communication_info_resp_t;
/*^ API, RRC_OAM_COMMUNICATION_INFO_RESP ^*/


/******************************************************************************
*   RRC_OAM_PROVISION_REQ
******************************************************************************/
typedef struct _rrc_oam_integrity_algorithms_t
{
/* SPR-18445 start */
    U8                                  num_algorithms; /*^ M, 0, B, 1, RRC_MAX_INTEGRITY_ALGORITHMS ^*/
/* SPR-18445 stop */
    U8  algorithms[RRC_MAX_INTEGRITY_ALGORITHMS];
/*^ M, 0, OCTET_STRING, VARIABLE, H, 0, 2 ^*/ /* rrc_int_algorithm_et */

} rrc_oam_integrity_algorithms_t;

typedef struct _rrc_oam_ciphering_algorithms_t
{
/* SPR-18445 start */
    U8                                  num_algorithms; /*^ M, 0, B, 1, RRC_MAX_CIPHERING_ALGORITHMS ^*/
/* SPR-18445 stop */
    U8  algorithms[RRC_MAX_CIPHERING_ALGORITHMS];
/*^ M, 0, OCTET_STRING, VARIABLE, H, 0, 2 ^*/ /* rrc_ciph_algorithm_et */

} rrc_oam_ciphering_algorithms_t;

typedef struct _rrc_oam_supported_security_algorithms_t
{
    rrc_oam_integrity_algorithms_t      integrity_algorithms;
/*^ M, 0, N, 0, 0 ^*/

    rrc_oam_ciphering_algorithms_t      ciphering_algorithms;
/*^ M, 0, N, 0, 0 ^*/

} rrc_oam_supported_security_algorithms_t;

typedef struct _rrc_oam_timer_info_t
{
    U8              timer_id;
/*^ M, 0, H, 0, 41 ^*/ /* rrc_oam_timer_id_et */

    U16             timer_val;          /*^ M, 0, L, 50, 65535 ^*/
} rrc_oam_timer_info_t;

typedef struct _rrc_oam_llim_timer_info_t
{

	/* SPR_17664_start*/
    U8                      num_of_timers;
/*^ M, 0, B, 14, MAX_LLIM_TIMER_INFO ^*/

	/* SPR_17664_end*/
    rrc_oam_timer_info_t    timer_info[MAX_LLIM_TIMER_INFO];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} rrc_oam_llim_timer_info_t;

typedef struct _rrc_oam_csc_timer_info_t
{
	
	/* SPR_17664_start*/
    U8                      num_of_timers;
/*^ M, 0, B, 7, MAX_CSC_TIMER_INFO ^*/

	/* SPR_17664_end*/
    rrc_oam_timer_info_t    timer_info[MAX_CSC_TIMER_INFO];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} rrc_oam_csc_timer_info_t;

typedef struct _rrc_oam_uecc_timer_info_t
{

	/* SPR_17664_start*/
    U8                      num_of_timers;
/*^ M, 0, B, 21, MAX_UECC_TIMER_INFO  ^*/

	/* SPR_17664_end*/
    rrc_oam_timer_info_t    timer_info[MAX_UECC_TIMER_INFO];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} rrc_oam_uecc_timer_info_t;

#define RRC_OAM_UECC_INTRA_CELL_HO_GUARD_TIMER_PRESENT       0x01
#define RRC_OAM_UECC_UE_RECONFIG_TIMER_PRESENT               0x02
#define RRC_OAM_UECC_INTEGRITY_THRESHOLD_TIMER_PRESENT       0x04
#define RRC_OAM_UECC_CCO_GUARD_TIMER_PRESENT                 0x08
#define RRC_OAM_UECC_STORE_UE_CONTEXT_TIMER_PRESENT          0x10
#define RRC_OAM_UECC_UE_COUNTER_CHECK_TIMER_PRESENT          0x20
#define RRC_OAM_UECC_UE_INFORMATION_TIMER_PRESENT            0x40
#define RRC_OAM_UECC_HEALTH_MONITORING_TIMER_PRESENT         0x80
/*NR_DC Code Change Start*/
#define RRC_OAM_UECC_DC_PREP_TIMER_PRESENT                   0X100
#define RRC_OAM_UECC_DC_BEARER_CHANGE_GUARD_TIMER            0x200
/*NR_DC Code Change Stop*/

typedef struct _rrc_oam_module_timers_info_t
{
    U16             bitmask;            /*^ BITMASK ^*/

    U16             uecc_intra_cell_ho_guard_timer;
/*^ O, RRC_OAM_UECC_INTRA_CELL_HO_GUARD_TIMER_PRESENT, L, 50, 65535 ^*/

    U16             uecc_ue_reconfig_timer;
/*^ O, RRC_OAM_UECC_UE_RECONFIG_TIMER_PRESENT, L, 50, 65535 ^*/

    U16                       uecc_integrity_threshold_timer_value;
/*^ O, RRC_OAM_UECC_INTEGRITY_THRESHOLD_TIMER_PRESENT, N, 0, 0  ^*/

    U16             uecc_cco_guard_timer_value;
/*^ O, RRC_OAM_UECC_CCO_GUARD_TIMER_PRESENT, L, 50, 65535 ^*/

    U16             uecc_store_ue_context_timer;
/*^ O, RRC_OAM_UECC_STORE_UE_CONTEXT_TIMER_PRESENT, L, 50, 65535 ^*/

    U16             uecc_ue_counter_check_timer;
/*^ O, RRC_OAM_UECC_UE_COUNTER_CHECK_TIMER_PRESENT, L, 50, 65535 ^*/

    U16             uecc_ue_information_timer;
/*^ O, RRC_OAM_UECC_UE_INFORMATION_TIMER_PRESENT, L, 50, 65535 ^*/

    U16             rrc_health_mon_time_int;
/*^ O, RRC_OAM_UECC_HEALTH_MONITORING_TIMER_PRESENT, N, 0, 0  ^*/

    /*NR_DC Code Change Start*/
    U16             uecc_dc_prep_timer;
/*^ O, RRC_OAM_UECC_DC_PREP_TIMER_PRESENT, B, 50, 65535 ^*/ 

    U16             uecc_dc_bearer_change_guard_timer;
/*^ O, RRC_OAM_UECC_DC_BEARER_CHANGE_GUARD_TIMER, B, 50, 65535 ^*/ 
    /*NR_DC Code Change Stop*/
} rrc_oam_module_timers_info_t;

#define RRC_OAM_SHARED_MEM_LOG_FILE_PATH_PRESENT       0x01
#define RRC_OAM_SHARED_MEM_CORE_NUMBER_PRESENT         0x02
#define RRC_OAM_SHARED_MEM_LOG_FILE_COUNT_PRESENT      0x04
typedef struct _rrc_shared_mem_config_t
{
    U16        bitmask;       /*^ BITMASK ^*/
    /*SPR_17020_START*/
    U8         SharedMemSize; /*^ M, 0, L, 3, 255 ^*/

    U8         MaxLogFileSize; /*^ M, 0, L, 3, 255 ^*/
    /*SPR_17020_END*/

    S8         LogFilePath[MAX_FILE_NAME_SIZE];
    /*^ O, RRC_OAM_SHARED_MEM_LOG_FILE_PATH_PRESENT, OCTET_STRING, FIXED ^*/

    U8         core_no;  
    /*^ O, RRC_OAM_SHARED_MEM_CORE_NUMBER_PRESENT, N, 0, 0 ^*/

    U8         MaxLogFileCount;
    /*^ O, RRC_OAM_SHARED_MEM_LOG_FILE_COUNT_PRESENT, L, 1, 0 ^*/
}rrc_shared_mem_config_t;

#define RRC_OAM_LOG_FILE_NAME_PRESENT       0x01
#define RRC_OAM_INTEGRITY_THRESHOLD_COUNTER_PRESENT 0x02
#define RRC_OAM_MODULE_TIMERS_INFO          0x04
#define RRC_OAM_CONFIG_OPTION_PRESENT       0x08
#define RRC_OAM_NETWORK_IP_PORT_PRESENT     0x10
#define RRC_OAM_UE_CONTEXT_LIST_SIZE_PRESENT  0x20
#define RRC_OAM_SHARED_MEM_NAME_PRESENT       0x40 

typedef struct _rrc_oam_provision_req_t
{
    U16                         bitmask;                    /*^ BITMASK ^*/

    U8                          log_level;
/*^ M, 0, H, 0, 6 ^*/  /* rrc_oam_log_level_et */

    S8                          log_file_name[MAX_FILE_NAME_SIZE];
/*^ O, RRC_OAM_LOG_FILE_NAME_PRESENT, OCTET_STRING, FIXED ^*/

    U8                          phy_sync_mode;
/*^ M, 0, N, 0, 0 ^*/
/* CSR_00040799 Fix Start */
    U16                         max_num_supported_ue;
/* SPR 21553 Start */
/*^ M, 0, B, 1, RRC_MAX_NUM_SUPPORTED_UE ^*/ /* RRC_MAX_NUM_SUPPORTED_UE */
/* SPR 21553 End */
/* CSR_00040799 Fix End */
    rrc_oam_supported_security_algorithms_t supported_security_algorithms;
/*^ M, 0, N, 0, 0 ^*/

    rrc_oam_llim_timer_info_t   llim_timer_info;
/*^ M, 0, N, 0, 0 ^*/

    rrc_oam_csc_timer_info_t    csc_timer_info;
/*^ M, 0, N, 0, 0 ^*/

    rrc_oam_uecc_timer_info_t   uecc_timer_info;
/*^ M, 0, N, 0, 0 ^*/

    rrc_sfn_t                   non_broadcast_sfn_offset;
/*^ M, 0, H, 0,1023 ^*/

 /* This is the maxium number of retries to be done for cell reconfiguration */
     U8                          max_cell_reconfig_retry_count ;
 /*^ M,0,N,0,0 ^*/


	/* SPR_17664_start*/
    U8                           len_vendor_name;
/*^ M, 0, B, 1,RRC_VENDOR_NAME_LEN ^*/
	/* SPR_17664_end*/
    U8                           vendor_name[RRC_VENDOR_NAME_LEN]; 
/*^ M, 0, OCTET_STRING, VARIABLE ^*/ 

    U16                          pdcp_integrity_threshold_counter;
/*^ O, RRC_OAM_INTEGRITY_THRESHOLD_COUNTER_PRESENT, N, 0, 0 ^*/

    rrc_oam_module_timers_info_t module_timers_info;
/*^ O, RRC_OAM_MODULE_TIMERS_INFO, N, 0, 0 ^*/

    U8                          advance_logging_config_option;
/*^ O, RRC_OAM_CONFIG_OPTION_PRESENT, H, 0, 3 ^*/ /* rrc_oam_logger_option_et */
    
    S8                          network_ip_port[MAX_STRING_SIZE];
/*^ O, RRC_OAM_NETWORK_IP_PORT_PRESENT, OCTET_STRING, FIXED ^*/
    
    U8                         ue_context_list_size;
/*^ O, RRC_OAM_UE_CONTEXT_LIST_SIZE_PRESENT, N, 0, 0 ^*/

    rrc_shared_mem_config_t     shared_mem_logging_config;
/*^ O, RRC_OAM_SHARED_MEM_NAME_PRESENT, N, 0, 0 ^*/

} rrc_oam_provision_req_t; /*^ API, RRC_OAM_PROVISION_REQ ^*/

/******************************************************************************
*   RRC_OAM_PROVISION_RESP
******************************************************************************/
typedef struct _rrc_oam_provision_resp_t
{
    U8                  response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} rrc_oam_provision_resp_t; /*^ API, RRC_OAM_PROVISION_RESP ^*/

typedef struct _ueRrcInfoStatus_t
{
   U16   cRNTI;    /*^ M, 0, N, 0, 0 ^*/
   U16   ueIndex;  /*^ M, 0, N, 0, 0 ^*/
   U32   enbS1APId;  /*^ M, 0, H, 0, 16777215 ^*/
   U32   mmeS1APId;  /*^ M, 0, N, 0, 0 ^*/
   U8    status; /*^ M, 0, N, 0, 0 ^*/ 
}ueRrcInfoStatus_t;

/*CID 113590 fix +-*/
typedef struct _oam_rrc_ps_stats_resp_t
{
    U16           noOfUe;
/*^ M, 0, N, 0, 0 ^*/ 
    ueRrcInfoStatus_t  ueInfoStatus[MAX_NUM_UE];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}oam_rrc_ps_stats_resp_t;

/*SPR 13115 Start*/

/******************************************************************************
*   RRC_OAM_GET_LOG_LEVEL_RESP
******************************************************************************/
typedef struct _rrc_oam_get_log_level_resp_t
{
#define RRC_OAM_LOG_LEVEL_PRESENT 0x01
    U16                     bitmask;    /*^ BITMASK ^*/
    U8                  log_on_off;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_oam_log_on_off_et */
    U8                  log_level;
/*^ O, RRC_OAM_LOG_LEVEL_PRESENT, H, 0, 6 ^*/ /* rrc_oam_log_level_et */

} rrc_oam_get_log_level_resp_t; /*^ API, RRC_OAM_GET_LOG_LEVEL_RESP ^*/
/*SPR 13115 Stop*/

/******************************************************************************
*   RRC_OAM_SET_LOG_LEVEL_REQ
******************************************************************************/
typedef struct _rrc_oam_set_log_level_req_t
{
    U8                  log_level;
/*^ M, 0, H, 0, 6 ^*/ /* rrc_oam_log_level_et */

} rrc_oam_set_log_level_req_t; /*^ API, RRC_OAM_SET_LOG_LEVEL_REQ ^*/

/******************************************************************************
*   RRC_OAM_SET_LOG_LEVEL_RESP
******************************************************************************/
typedef struct _rrc_oam_set_log_level_resp_t
{
    U8                  response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} rrc_oam_set_log_level_resp_t; /*^ API, RRC_OAM_SET_LOG_LEVEL_RESP ^*/

/******************************************************************************
*   RRC_OAM_LOG_ENABLE_REQ
******************************************************************************/
typedef struct _rrc_oam_log_enable_req_t
{
    U8                  log_on_off;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_oam_log_on_off_et */

} rrc_oam_log_enable_req_t; /*^ API, RRC_OAM_LOG_ENABLE_REQ ^*/

/******************************************************************************
*   RRC_OAM_LOG_ENABLE_RESP
******************************************************************************/
typedef struct _rrc_oam_log_enable_resp_t
{
    U8                  response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} rrc_oam_log_enable_resp_t; /*^ API, RRC_OAM_LOG_ENABLE_RESP ^*/


/******************************************************************************
*   RRC_OAM_GET_CELL_STATS_RESP
******************************************************************************/
#define RRC_ESTABLISHMENT_CAUSE_INDEX         8       /* EstablishmentCause     */
#define RRC_REESTABLISHMENT_CAUSE_INDEX       4       /* ReestablishmentCause   */
/* CR 101 changes start*/
#define RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX    39      /* s1ap_CauseRadioNetwork */ 
/* CR 101 changes stop*/
#define RRC_S1AP_CAUSE_TRANSPORT_INDEX        2       /* s1ap_CauseTransport    */
/* CR 101 changes start*/
#define RRC_S1AP_CAUSE_NAS_INDEX              5       /* s1ap_CauseNas          */
/* CR 101 changes stop*/
#define RRC_S1AP_CAUSE_PROTOCOL_INDEX         7       /* s1ap_CauseProtocol     */
#define RRC_S1AP_CAUSE_MISC_INDEX             6       /* s1ap_CauseMisc         */
#define RRC_MAX_QCI_INDEX                     10      /*0:Invalid QCI; 1-9: Valid QCI */            
#define DUMMY_RANGE_CHECK_INDEX               1       /* Dummy Index            */
#define RRC_MMEGI_RIGHT_SHIFT_BITS            7
#define RRC_MMEGI_BITMASK                     1
/* BUG 12662 Fix Start */
#define RRC_MAX_CONNECTION                    32 

typedef struct _rrc_uecc_oam_stat_connection_establishment_t
{
    U32 rrc_connEstabAtt_cause[RRC_ESTABLISHMENT_CAUSE_INDEX]; 
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 rrc_connEstabAtt_sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 rrc_connEstabSucc_cause[RRC_ESTABLISHMENT_CAUSE_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 rrc_connEstabSucc_sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 rrc_connEstabFail_cause[RRC_ESTABLISHMENT_CAUSE_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 rrc_connEstabFail_sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 rrc_connEstabFaileNBCause_Unspecified;
    /*^ M, 0, N, 0, 0 ^*/
    U32 rrc_ConnEstabFaileNBCause_Congestion;
    /*^ M, 0, N, 0, 0 ^*/
    /* CR 026 changes start*/
    U32 rrc_ConnEstabFaileNBCause_EnergySaving;
    /*^ M, 0, N, 0, 0 ^*/
    /* CR 026 changes stop*/
} rrc_uecc_oam_stat_connection_establishment_t;

typedef struct _rrc_uecc_oam_stat_connection_reestablishment_t
{
    U32 rrc_connReEstabAtt_cause[RRC_REESTABLISHMENT_CAUSE_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 rrc_connReEstabAtt_sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 rrc_connReEstabSucc_cause[RRC_REESTABLISHMENT_CAUSE_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 rrc_connReEstabSucc_sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 rrc_connReEstabFail_cause[RRC_REESTABLISHMENT_CAUSE_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 rrc_connReEstabFail_sum;
    /*^ M, 0, N, 0, 0 ^*/
} rrc_uecc_oam_stat_connection_reestablishment_t;

typedef struct _rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_t
{
    U32 ueCtxt_relReq_cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 ueCtxt_relReq_cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 ueCtxt_relReq_cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 ueCtxt_relReq_cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 ueCtxt_relReq_cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
}rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_t;

typedef struct _rrc_uecc_oam_stat_ue_context_release_t
{
    rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_t ueCtxt_relReq_cause;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ueCtxt_relReq_sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ueContext_RelSuccNbr;
    /*^ M, 0, N, 0, 0 ^*/
} rrc_uecc_oam_stat_ue_context_release_t;

typedef struct _rrc_uecc_oam_erab_estab_init_fail_t
{
    U32 erab_estabInitFailNbr_cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_estabInitFailNbr_cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_estabInitFailNbr_cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_estabInitFailNbr_cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_estabInitFailNbr_cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
}rrc_uecc_oam_erab_estab_init_fail_t;


typedef struct _rrc_uecc_oam_erab_estab_add_fail_t
{
    U32 erab_estabAddFailNbr_cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_estabAddFailNbr_cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_estabAddFailNbr_cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_estabAddFailNbr_cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_estabAddFailNbr_cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
}rrc_uecc_oam_erab_estab_add_fail_t;

typedef struct _rrc_erab_estabTime_t
{
    U32  count;
    /*^ M, 0, N, 0, 0 ^*/
    U32  erab_estabTime_Sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32  erab_estabTime_Max;
    /*^ M, 0, N, 0, 0 ^*/
}rrc_erab_estabTime_t;

typedef struct _rrc_uecc_oam_stat_erab_setup_t
{
    U32 erab_estabInitAttNbr_qci[RRC_MAX_QCI_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_estabInitAttNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 erab_estabInitSuccNbr_qci[RRC_MAX_QCI_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_estabInitSuccNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
    rrc_uecc_oam_erab_estab_init_fail_t erab_estab_init_fail_stats;
    /*^ M, 0, N, 0, 0 ^*/
    U32 erab_estabInitFailNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 erab_estabAddAttNbr_qci[RRC_MAX_QCI_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_estabAddAttNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 erab_estabAddSuccNbr_qci[RRC_MAX_QCI_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_estabAddSuccNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
    rrc_uecc_oam_erab_estab_add_fail_t erab_estab_add_fail_stats;
    /*^ M, 0, N, 0, 0 ^*/
    U32 erab_estabAddFailNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
    rrc_erab_estabTime_t    erab_estabTime_qci[RRC_MAX_QCI_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
} rrc_uecc_oam_stat_erab_setup_t;

typedef struct _rrc_uecc_oam_rel_fail_cause_t
{
    U32 erab_relFailNbr_cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_relFailNbr_cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_relFailNbr_cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_relFailNbr_cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_relFailNbr_cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
}rrc_uecc_oam_rel_fail_cause_t;    

typedef struct _rrc_uecc_oam_erab_rel_enb_cause_t
{
    U32 erab_relEnbNbr_cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_relEnbNbr_cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_relEnbNbr_cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_relEnbNbr_cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_relEnbNbr_cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
}rrc_uecc_oam_erab_rel_enb_cause_t;

typedef struct _rrc_uecc_oam_stat_erab_release_t
{
    U32 erab_relEnbNbr_qci[RRC_MAX_QCI_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_relEnbNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
    rrc_uecc_oam_erab_rel_enb_cause_t erab_rel_enb_cause_stats;
    /*^ M, 0, N, 0, 0 ^*/
    U32 erab_relAttNbr_qci[RRC_MAX_QCI_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_relAttNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 erab_relSuccNbr_qci[RRC_MAX_QCI_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_relSuccNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
    rrc_uecc_oam_rel_fail_cause_t rel_fail_cause_stats;
    /*^ M, 0, N, 0, 0 ^*/
    U32 erab_relFailNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 erab_relActNbr_qci[RRC_MAX_QCI_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 erab_relActNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
}rrc_uecc_oam_stat_erab_release_t;

typedef struct _rrc_uecc_oam_stat_erab_mod_fail_cause_t
{
    U32 erab_modQosFailNbr_cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_modQosFailNbr_cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_modQosFailNbr_cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_modQosFailNbr_cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_modQosFailNbr_cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
}rrc_uecc_oam_stat_erab_mod_fail_cause_t;

typedef struct _rrc_uecc_oam_stat_erab_modification_t
{
    U32 erab_modQosAttNbr_qci[RRC_MAX_QCI_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_modQosAttNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 erab_modQosSuccNbr_qci[RRC_MAX_QCI_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
    U32 erab_modQosSuccNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
    rrc_uecc_oam_stat_erab_mod_fail_cause_t erab_mod_fail_cause_stats;
    /*^ M, 0, N, 0, 0 ^*/
    U32 erab_modQosFailNbr_sum;
    /*^ M, 0, N, 0, 0 ^*/
} rrc_uecc_oam_stat_erab_modification_t;

typedef struct _rrc_uecc_oam_stat_ue_assoc_s1_connection_t
{
    U32 s1sig_connEstabAtt;
    /*^ M, 0, N, 0, 0 ^*/
    U32 s1sig_connEstabSucc;
    /*^ M, 0, N, 0, 0 ^*/
} rrc_uecc_oam_stat_ue_assoc_s1_connection_t;

typedef struct _rrc_connEstabTime_t
{
    U32  count;
    /*^ M, 0, N, 0, 0 ^*/
    U32 rrc_connEstabTime_Sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 rrc_connEstabTime_Max;
    /*^ M, 0, N, 0, 0 ^*/
} rrc_connEstabTime_t;

#define RRC_OAM_STATISTICS_CONNECTION_ESTB_STATS_PRESENT        0x01
#define RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_STATS_PRESENT     0x02
#define RRC_OAM_STATISTICS_CONNECTION_NUMBER_STATS_PRESENT      0x04
#define RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_STATS_PRESENT  0x08
#define RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_STATS_PRESENT     0x10
typedef struct _rrc_oam_connection_related_stats_t
{
    U16                     bitmask;    /*^ BITMASK ^*/

    rrc_uecc_oam_stat_connection_establishment_t rrc_connection_establishment; 
/*^ O, RRC_OAM_STATISTICS_CONNECTION_ESTB_STATS_PRESENT, N, 0, 0 ^*/

    rrc_uecc_oam_stat_connection_reestablishment_t rrc_connection_reestablishment; 
/*^ O, RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_STATS_PRESENT, N, 0, 0 ^*/

    U32  rrcConnectionNumber;   
/*^ O, RRC_OAM_STATISTICS_CONNECTION_NUMBER_STATS_PRESENT, N, 0, 0 ^*/

    rrc_connEstabTime_t  rrc_connEstabTime_cause[RRC_ESTABLISHMENT_CAUSE_INDEX];
/*^ O, RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_STATS_PRESENT, OCTET_STRING, FIXED ^*/

    rrc_uecc_oam_stat_ue_context_release_t ue_contxt_rel_stats;
/*^ O, RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_STATS_PRESENT, N, 0, 0 ^*/

}rrc_oam_connection_related_stats_t;

typedef struct _rrc_uecc_oam_stat_erab_number_t
{
    U32 erabSimNumber_qci[RRC_MAX_QCI_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 erabSimNumber_sum;
    /*^ M, 0, N, 0, 0 ^*/    
} rrc_uecc_oam_stat_erab_number_t;

#define RRC_OAM_STATISTICS_ERAB_SETUP_STATS_PRESENT    0x01
#define RRC_OAM_STATISTICS_ERAB_RELEASE_STATS_PRESENT  0x02
#define RRC_OAM_STATISTICS_ERAB_MODIFY_STATS_PRESENT   0x04
/* Future Use: #define RRC_OAM_STATISTICS_ERAB_ACTIIVITY_STATS_PRESENT   0x08*/
#define RRC_OAM_STATISTICS_ERAB_NUMBER_STATS_PRESENT   0x10
typedef struct _rrc_oam_erab_related_stats_t
{
    U16                     bitmask;    /*^ BITMASK ^*/
    
    rrc_uecc_oam_stat_erab_setup_t          erab_setup_stats;
/*^ O, RRC_OAM_STATISTICS_ERAB_SETUP_STATS_PRESENT, N, 0, 0 ^*/
    
    rrc_uecc_oam_stat_erab_release_t        erab_release_stats;
/*^ O, RRC_OAM_STATISTICS_ERAB_RELEASE_STATS_PRESENT, N, 0, 0 ^*/
    
    rrc_uecc_oam_stat_erab_modification_t   erab_mod_stats;
/*^ O, RRC_OAM_STATISTICS_ERAB_MODIFY_STATS_PRESENT, N, 0, 0 ^*/

    rrc_uecc_oam_stat_erab_number_t         erab_sim_num_stats;   
/*^ O, RRC_OAM_STATISTICS_ERAB_NUMBER_STATS_PRESENT, N, 0, 0 ^*/

}rrc_oam_erab_related_stats_t;

/* For Future Use*/

typedef struct _rrc_oam_ho_IntraEnbOutAtt_cause_t
{
    U32 ho_IntraEnbOutAtt_Cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IntraEnbOutAtt_Cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IntraEnbOutAtt_Cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IntraEnbOutAtt_Cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IntraEnbOutAtt_Cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
} rrc_oam_ho_IntraEnbOutAtt_cause_t;

typedef struct _rrc_oam_ho_IntraEnbOutSucc_cause_t
{
    U32 ho_IntraEnbOutSucc_Cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IntraEnbOutSucc_Cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IntraEnbOutSucc_Cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IntraEnbOutSucc_Cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IntraEnbOutSucc_Cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
} rrc_oam_ho_IntraEnbOutSucc_cause_t;

typedef struct _rrc_oam_intra_freq_ho_stats_t
{
    U32 ho_IntraFreqOutAtt;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_IntraFreqOutSucc;
    /*^ M, 0, N, 0, 0 ^*/
} rrc_oam_intra_freq_ho_stats_t;


typedef struct _rrc_oam_intra_enb_ho_stats_t
{
    rrc_oam_ho_IntraEnbOutAtt_cause_t ho_IntraEnbOutAtt_cause;
    /*^ M, 0, N, 0, 0 ^*/ 
    U32 ho_IntraEnbOutAtt_sum;
    /*^ M, 0, N, 0, 0 ^*/
    rrc_oam_ho_IntraEnbOutSucc_cause_t ho_IntraEnbOutSucc_cause;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_IntraEnbOutSucc_sum;
    /*^ M, 0, N, 0, 0 ^*/
}rrc_oam_intra_enb_ho_stats_t;


typedef struct _rrc_oam_ho_InterEnbOutAtt_cause_t
{
    U32 ho_InterEnbOutAtt_Cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_InterEnbOutAtt_Cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_InterEnbOutAtt_Cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_InterEnbOutAtt_Cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_InterEnbOutAtt_Cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrc_oam_ho_InterEnbOutAtt_cause_t;

typedef struct _rrc_oam_ho_InterEnbOutSucc_cause_t
{
    U32 ho_InterEnbOutSucc_Cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_InterEnbOutSucc_Cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_InterEnbOutSucc_Cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_InterEnbOutSucc_Cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_InterEnbOutSucc_Cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrc_oam_ho_InterEnbOutSucc_cause_t;


typedef struct _rrc_oam_inter_freq_ho_stats_t
{
    U32 ho_InterFreqMeasGapOutAtt;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_InterFreqMeasGapOutSucc; 
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_InterFreqNoMeasGapOutAtt; 
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_InterFreqNoMeasGapOutSucc;
    /*^ M, 0, N, 0, 0 ^*/
} rrc_oam_inter_freq_ho_stats_t;


typedef struct _rrc_oam_inter_enb_ho_stats_t
{
    rrc_oam_ho_InterEnbOutAtt_cause_t ho_InterEnbOutAtt_cause;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_InterEnbOutAtt_sum;
    /*^ M, 0, N, 0, 0 ^*/
    rrc_oam_ho_InterEnbOutSucc_cause_t ho_InterEnbOutSucc_cause;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_InterEnbOutSucc_sum;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_InterEnbOutPrepAtt;
    /*^ M, 0, N, 0, 0 ^*/

} rrc_oam_inter_enb_ho_stats_t;

typedef struct _rrc_oam_ho_IratOutAtt_cause_t
{
    U32 ho_IratOutAtt_Cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IratOutAtt_Cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IratOutAtt_Cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IratOutAtt_Cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IratOutAtt_Cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrc_oam_ho_IratOutAtt_cause_t;

typedef struct _rrc_oam_ho_IratOutSucc_cause_t
{
    U32 ho_IratOutSucc_Cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IratOutSucc_Cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IratOutSucc_Cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IratOutSucc_Cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_IratOutSucc_Cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/

}rrc_oam_ho_IratOutSucc_cause_t;

/*SPR_19166_START*/
typedef enum
{
    RRC_RAT_TYPE_UTRAN,
    RRC_RAT_TYPE_GERAN,
}rrc_inter_rat_type_et;


typedef struct 
{
    rrc_inter_rat_type_et     rat_type; /*^ M, 0, N, 0, 0 ^*/
    plmn_identity_t     plmn_identity; /*^ M, 0, N, 0, 0 ^*/
    U8                  cell_identity[MAX_CELL_IDENTITY_NUMBYTES]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrc_utran_geran_cgi_t;


typedef struct
{
        rrc_bitmask_t                               bitmask; /*^ BITMASK ^*/   
#define RRC_TARGET_ID_UTRAN_GERAN_CGI_PRESENT   0X01
#define RRC_TARGET_ID_CDMA2000_1XRTT_CELL_ID_PRESENT         0X02
#define RRC_TARGET_ID_CDMA2000_HRPD_SECTOR_ID_PRESENT         0X04
        rrc_utran_geran_cgi_t                       cgi_info; 
/*^ O, RRC_TARGET_ID_UTRAN_GERAN_CGI_PRESENT, N, 0, 0 ^*/

        cdma2000_1xrtt_cell_identifier_t            cell_id_1xrtt; 
/*^ O,RRC_TARGET_ID_CDMA2000_1XRTT_CELL_ID_PRESENT, N, 0, 0 ^*/

        cdma2000_hrpd_sector_identifier_t           sector_id_hrpd; 
/*^ O,RRC_TARGET_ID_CDMA2000_HRPD_SECTOR_ID_PRESENT, N, 0, 0 ^*/

}rrc_inter_rat_global_cgi_t;
/*SPR_19166_END*/



/* BUG 12662 Fix Start */
typedef struct _rrc_oam_inter_rat_ho_stats_t
{
    /*SPR_19166_START*/
    rrc_inter_rat_global_cgi_t  target_cgi;
    /*SPR_19166_END*/
    /*^ M, 0, N, 0, 0 ^*/
    rrc_oam_ho_IratOutAtt_cause_t ho_IratOutAtt_cause;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_IratOutAtt_sum;
    /*^ M, 0, N, 0, 0 ^*/
    rrc_oam_ho_IratOutSucc_cause_t ho_IratOutSucc_cause;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_IratOutSucc_sum;
    /*^ M, 0, N, 0, 0 ^*/
}rrc_oam_inter_rat_ho_stats_t;

typedef struct _rrc_oam_inter_rat_ho_stats_list_t
{
    U8  count;

	/* SPR_17664_start*/
    /*^ M, 0, H, 0, RRC_MAX_CONNECTION ^*/
    
	/* SPR_17664_end*/
rrc_oam_inter_rat_ho_stats_t inter_rat_ho_stats[RRC_MAX_CONNECTION];	
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_oam_inter_rat_ho_stats_list_t;
/* BUG 12662 Fix Stop */

typedef struct _rrc_oam_drx_non_drx_ho_stats_t
{
    U32 ho_DrxOutAtt;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_DrxOutSucc;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_NoDrxOutAtt;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_NoDrxOutSucc;
    /*^ M, 0, N, 0, 0 ^*/
}rrc_oam_drx_non_drx_ho_stats_t;

typedef struct _rrc_oam_ho_OutAttTarget_cause_t
{
    U32 ho_OutAttTarget_Cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_OutAttTarget_Cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_OutAttTarget_Cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_OutAttTarget_Cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_OutAttTarget_Cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrc_oam_ho_OutAttTarget_cause_t;

typedef struct _rrc_oam_ho_OutSuccTarget_cause_t
{
    U32 ho_OutSuccTarget_Cause_radioNetwork[RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_OutSuccTarget_Cause_transport[RRC_S1AP_CAUSE_TRANSPORT_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_OutSuccTarget_Cause_nas[RRC_S1AP_CAUSE_NAS_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_OutSuccTarget_Cause_protocol[RRC_S1AP_CAUSE_PROTOCOL_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U32 ho_OutSuccTarget_Cause_misc[RRC_S1AP_CAUSE_MISC_INDEX];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrc_oam_ho_OutSuccTarget_cause_t;

typedef struct _rrc_oam_meas_on_neigh_cell_ho_stats_t
{
    /*SPR_19166_START*/
    cell_global_id_eutra_t                         ecgi;
    /*SPR_19166_END*/
    /*^ M, 0, N, 0, 0 ^*/
    rrc_oam_ho_OutAttTarget_cause_t     ho_OutAttTarget_cause;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_OutAttTarget_sum;
    /*^ M, 0, N, 0, 0 ^*/
    rrc_oam_ho_OutSuccTarget_cause_t    ho_OutSuccTarget_cause;
    /*^ M, 0, N, 0, 0 ^*/
    U32 ho_OutSuccTarget_sum;
    /*^ M, 0, N, 0, 0 ^*/
}rrc_oam_meas_on_neigh_cell_ho_stats_t;

typedef struct _rrc_oam_meas_on_neigh_cell_ho_stats_list_t
{
    U8  count;

	/* SPR_17664_start*/
    /*^ M, 0, H, 0, RRC_MAX_CONNECTION ^*/

	/* SPR_17664_end*/
 /* BUG 12662 Fix Stop */
    rrc_oam_meas_on_neigh_cell_ho_stats_t  
        meas_on_neigh_cell_ho_stats[RRC_MAX_CONNECTION];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_oam_meas_on_neigh_cell_ho_stats_list_t;

#define RRC_OAM_INTRA_ENB_HO_STATS_PRESENT                0x01
#define RRC_OAM_INTER_ENB_HO_STATS_PRESENT                0x02
#define RRC_OAM_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT       0x04
#define RRC_OAM_INTRA_FREQ_HO_STATS_PRESENT               0x08
#define RRC_OAM_INTER_FREQ_HO_STATS_PRESENT               0x10
#define RRC_OAM_DRX_NON_DRX_HO_STATS_PRESENT              0x20
#define RRC_OAM_INTER_RAT_HO_STATS_PRESENT                0x40
#define RRC_OAM_HO_FAILURE_NUM_STATS_PRESENT              0x80
typedef struct _rrc_oam_handover_related_stats_t 
{
    U16                                      bitmask;    /*^ BITMASK ^*/

    rrc_oam_intra_enb_ho_stats_t             intra_enb_ho_stats;
/*^ O, RRC_OAM_INTRA_ENB_HO_STATS_PRESENT, N, 0, 0 ^*/

    rrc_oam_inter_enb_ho_stats_t             inter_enb_ho_stats;
/*^ O, RRC_OAM_INTER_ENB_HO_STATS_PRESENT, N, 0, 0 ^*/
    
    rrc_oam_meas_on_neigh_cell_ho_stats_list_t    meas_on_neigh_cell_ho_stats_list;
/*^ O, RRC_OAM_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT, N, 0, 0 ^*/
    
    rrc_oam_intra_freq_ho_stats_t            intra_freq_ho_stats;
/*^ O, RRC_OAM_INTRA_FREQ_HO_STATS_PRESENT, N, 0, 0 ^*/
    
    rrc_oam_inter_freq_ho_stats_t            inter_freq_ho_stats;
/*^ O, RRC_OAM_INTER_FREQ_HO_STATS_PRESENT, N, 0, 0 ^*/
    
    rrc_oam_drx_non_drx_ho_stats_t           drx_non_drx_ho_stats;
/*^ O, RRC_OAM_DRX_NON_DRX_HO_STATS_PRESENT, N, 0, 0 ^*/
   
    /* BUG 12662 Fix Start */ 
    rrc_oam_inter_rat_ho_stats_list_t        inter_rat_ho_stats_list;
/*^ O, RRC_OAM_INTER_RAT_HO_STATS_PRESENT, N, 0, 0 ^*/
    /* BUG 12662 Fix Stop */

    U32                                      ho_failure_num;
/*^ O, RRC_OAM_HO_FAILURE_NUM_STATS_PRESENT, N, 0, 0 ^*/
} rrc_oam_handover_related_stats_t;

typedef enum
{
    RRC_OAM_GET_CELL_STATS_FAILURE,
    RRC_OAM_GET_CELL_STATS_SUCCESS,
    RRC_OAM_GET_CELL_STATS_ALREADY_ONGOING
} rrc_return_cell_stats_et;

#define RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_PRESENT                 0x01
#define RRC_OAM_STATISTICS_ERAB_RELATED_STATS_PRESENT                       0x02
#define RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_PRESENT                   0x04 
#define RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT           0x08
#define RRC_OAM_STATISTICS_PAGING_RELATED_STATS_PRESENT                     0x10
#define RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT        0x20
typedef struct _rrc_oam_get_cell_stats_resp_t
{
    U16                     bitmask;    /*^ BITMASK ^*/

    rrc_cell_index_t        cell_index; 
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/ /* MAX_NUM_CELLS - 1 */ 

    U8                      response;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_return_cell_stats_et */

    rrc_oam_connection_related_stats_t           rrc_connection_related_stats; 
/*^ O, RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_PRESENT, N, 0, 0 ^*/
    
    rrc_oam_erab_related_stats_t                 rrc_erab_related_stats; 
/*^ O, RRC_OAM_STATISTICS_ERAB_RELATED_STATS_PRESENT, N, 0, 0 ^*/

    rrc_oam_handover_related_stats_t             rrc_handover_related_stats;
/*^ O, RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_PRESENT, N, 0, 0 ^*/    
    
    rrc_uecc_oam_stat_ue_assoc_s1_connection_t   rrc_ue_assoc_s1_conn_related_stats; 
/*^ O, RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT, N, 0, 0 ^*/
    
    rrc_csc_oam_stat_paging_t                    rrc_paging_related_stats; 
/*^ O, RRC_OAM_STATISTICS_PAGING_RELATED_STATS_PRESENT, N, 0, 0 ^*/

    U32                                          rrc_IratIncMobility_LAI; 
/*^ O, RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT, N, 0, 0 ^*/ 

} rrc_oam_get_cell_stats_resp_t; /*^ API, RRC_OAM_GET_CELL_STATS_RESP ^*/

/******************************************************************************
*   RRC_OAM_GET_CELL_STATS_REQ
******************************************************************************/

#define RRC_OAM_STATISTICS_REQ_CONNECTION_ESTB_STATS_PRESENT        0x01
#define RRC_OAM_STATISTICS_REQ_CONNECTION_RE_ESTB_STATS_PRESENT     0x02
#define RRC_OAM_STATISTICS_REQ_CONNECTION_NUMBER_STATS_PRESENT      0x04
#define RRC_OAM_STATISTICS_REQ_CONNECTION_SETUP_TIME_STATS_PRESENT  0x08    
#define RRC_OAM_STATISTICS_REQ_CONNECTION_UE_CTX_REL_STATS_PRESENT  0x10
#define RRC_OAM_STATISTICS_REQ_CONNECTION_ALL_STATS_PRESENT         0x20
typedef struct _rrc_oam_connection_stats_t
{
    U16 bitmask;    /*^ BITMASK ^*/
    U8  rrc_connection_establishment; 
/*^ O, RRC_OAM_STATISTICS_REQ_CONNECTION_ESTB_STATS_PRESENT, N, 0, 0 ^*/

    U8  rrc_connection_reestablishment; 
/*^ O, RRC_OAM_STATISTICS_REQ_CONNECTION_RE_ESTB_STATS_PRESENT, N, 0, 0 ^*/
    
    U8  rrcConnectionNumber;
/*^ O, RRC_OAM_STATISTICS_REQ_CONNECTION_NUMBER_STATS_PRESENT, N, 0, 0 ^*/   

    U8  rrcConnectionSetupTime;
/*^ O, RRC_OAM_STATISTICS_REQ_CONNECTION_SETUP_TIME_STATS_PRESENT, N, 0, 0 ^*/    

    U8  ue_contxt_rel_stats;
/*^ O, RRC_OAM_STATISTICS_REQ_CONNECTION_UE_CTX_REL_STATS_PRESENT, N, 0, 0 ^*/
}rrc_oam_connection_stats_t;

#define RRC_OAM_STATISTICS_REQ_ERAB_SETUP_STATS_PRESENT    0x01
#define RRC_OAM_STATISTICS_REQ_ERAB_RELEASE_STATS_PRESENT  0x02
#define RRC_OAM_STATISTICS_REQ_ERAB_MODIFY_STATS_PRESENT   0x04
/* Future Use: #define RRC_OAM_STATISTICS_REQ_ERAB_ACTIIVITY_STATS_PRESENT   0x08*/
#define RRC_OAM_STATISTICS_REQ_ERAB_NUMBER_STATS_PRESENT   0x10
#define RRC_OAM_STATISTICS_REQ_ERAB_ALL_STATS_PRESENT      0x20
typedef struct _rrc_oam_erab_stats_t
{
    U16         bitmask;    /*^ BITMASK ^*/
    U8          erab_setup_stats;
/*^ O, RRC_OAM_STATISTICS_REQ_ERAB_SETUP_STATS_PRESENT, N, 0, 0 ^*/
    
    U8          erab_release_stats;
/*^ O, RRC_OAM_STATISTICS_REQ_ERAB_RELEASE_STATS_PRESENT, N, 0, 0 ^*/
    
    U8          erab_mod_stats;
/*^ O, RRC_OAM_STATISTICS_REQ_ERAB_MODIFY_STATS_PRESENT, N, 0, 0 ^*/

    U8          erab_num_stats;
/*^ O, RRC_OAM_STATISTICS_REQ_ERAB_NUMBER_STATS_PRESENT, N, 0, 0 ^*/
}rrc_oam_erab_stats_t;

#define RRC_OAM_STATISTICS_REQ_INTRA_ENB_HO_RELATED_STATS_PRESENT   0x01
#define RRC_OAM_STATISTICS_REQ_INTER_ENB_HO_RELATED_STATS_PRESENT   0x02
#define RRC_OAM_STATISTICS_REQ_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT  0x04
#define RRC_OAM_STATISTICS_REQ_INTRA_FREQ_HO_RELATED_STATS_PRESENT  0x08
#define RRC_OAM_STATISTICS_REQ_INTER_FREQ_HO_RELATED_STATS_PRESENT  0x10
#define RRC_OAM_STATISTICS_REQ_DRX_NON_DRX_HO_RELATED_STATS_PRESENT 0x20
#define RRC_OAM_STATISTICS_REQ_INTER_RAT_HO_STATS_PRESENT           0x40
#define RRC_OAM_STATISTICS_REQ_HANDOVER_ALL_STATS_PRESENT           0x80
#define RRC_OAM_STATISTICS_REQ_HANDOVER_FAILURE_NUM_STATS_PRESENT   0x100
typedef struct _rrc_oam_handover_stats_t
{
    U16                 bitmask;    /*^ BITMASK ^*/

    U8                  intra_enb_ho_related_stats;
/*^ O, RRC_OAM_STATISTICS_REQ_INTRA_ENB_HO_RELATED_STATS_PRESENT, N, 0, 0 ^*/

    U8                  inter_enb_ho_related_stats;
/*^ O, RRC_OAM_STATISTICS_REQ_INTER_ENB_HO_RELATED_STATS_PRESENT, N, 0, 0 ^*/

    U8                  meas_on_neigh_cell_ho_related_stats;
/*^ O, RRC_OAM_STATISTICS_REQ_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT, N, 0, 0 ^*/

    U8                  intra_freq_ho_related_stats;
/*^ O, RRC_OAM_STATISTICS_REQ_INTRA_FREQ_HO_RELATED_STATS_PRESENT, N, 0, 0 ^*/

    U8                  inter_freq_ho_related_stats;
/*^ O, RRC_OAM_STATISTICS_REQ_INTER_FREQ_HO_RELATED_STATS_PRESENT, N, 0, 0 ^*/

    U8                  drx_non_drx_ho_related_stats;
/*^ O, RRC_OAM_STATISTICS_REQ_DRX_NON_DRX_HO_RELATED_STATS_PRESENT, N, 0, 0 ^*/

    U8                  inter_rat_ho_related_stats;
/*^ O, RRC_OAM_STATISTICS_REQ_INTER_RAT_HO_STATS_PRESENT, N, 0, 0 ^*/

    U8                  ho_failure_num;
 /*^ O, RRC_OAM_STATISTICS_REQ_HANDOVER_FAILURE_NUM_STATS_PRESENT, N, 0, 0 ^*/
} rrc_oam_handover_stats_t;


#define RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_REQUESTED_PRESENT                   0x01
#define RRC_OAM_STATISTICS_ERAB_RELATED_STATS_REQUESTED_PRESENT                         0x02
#define RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_REQUESTED_PRESENT                     0x04 
#define RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_REQUESTED_PRESENT             0x08
#define RRC_OAM_STATISTICS_PAGING_RELATED_STATS_REQUESTED_PRESENT                       0x10
#define RRC_OAM_RESET_CELL_STATISTICS_PRESENT                                           0x20
#define RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_REQUESTED_PRESENT          0x40
#define RRC_OAM_STATISTICS_ALL_STATS_PRESENT                                            0x80
typedef struct _rrc_oam_get_cell_stats_req_t
{
    U16                 bitmask;    /*^ BITMASK ^*/

    rrc_cell_index_t    cell_index; 
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/ /* MAX_NUM_CELLS - 1 */ 
    
    rrc_oam_connection_stats_t      rrc_connection_stats; 
/*^ O, RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_REQUESTED_PRESENT, N, 0, 0 ^*/
    
    rrc_oam_erab_stats_t            rrc_erab_stats; 
/*^ O, RRC_OAM_STATISTICS_ERAB_RELATED_STATS_REQUESTED_PRESENT, N, 0, 0 ^*/
    
    rrc_oam_handover_stats_t        rrc_handover_stats;
/*^ O, RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_REQUESTED_PRESENT, N, 0, 0 ^*/

    U8                              rrc_ue_assoc_s1_conn_stats; 
/*^ O, RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_REQUESTED_PRESENT, N, 0, 0 ^*/
    
    U8                              rrc_paging_stats; 
/*^ O, RRC_OAM_STATISTICS_PAGING_RELATED_STATS_REQUESTED_PRESENT, N, 0, 0 ^*/
    
    U8                              reset_stats;
/*^ O, RRC_OAM_RESET_CELL_STATISTICS_PRESENT, N, 0, 0 ^*/ 

    U8                             rrc_IratIncMobility_LAI;
/*^ O, RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_REQUESTED_PRESENT, N, 0, 0 ^*/

} rrc_oam_get_cell_stats_req_t; /*^ API, RRC_OAM_GET_CELL_STATS_REQ ^*/

/******************************************************************************
*   RRC_OAM_CONFIG_STATS_REQ
******************************************************************************/
#define RRC_OAM_STATISTICS_REQ_CONNECTION_ESTB_CONFIG_STATS_PRESENT        0x01
#define RRC_OAM_STATISTICS_REQ_CONNECTION_RE_ESTB_CONFIG_STATS_PRESENT     0x02
#define RRC_OAM_STATISTICS_REQ_CONNECTION_NUMBER_CONFIG_STATS_PRESENT      0x04
#define RRC_OAM_STATISTICS_REQ_CONNECTION_SETUP_TIME_CONFIG_STATS_PRESENT  0x08
#define RRC_OAM_STATISTICS_REQ_CONNECTION_UE_CTX_REL_CONFIG_STATS_PRESENT  0x10
typedef struct _rrc_oam_rrc_connection_stats_info_t
{
    U16          bitmask;    /*^ BITMASK ^*/
    U16          sampling_period;
    /*^ M, 0, L, 1, 0 ^*/
} rrc_oam_rrc_connection_stats_info_t;

#define RRC_OAM_STATISTICS_REQ_ERAB_SETUP_CONFIG_STATS_PRESENT    0x01
#define RRC_OAM_STATISTICS_REQ_ERAB_RELEASE_CONFIG_STATS_PRESENT  0x02
#define RRC_OAM_STATISTICS_REQ_ERAB_MODIFY_CONFIG_STATS_PRESENT   0x04
#define RRC_OAM_STATISTICS_REQ_ERAB_NUMBER_CONFIG_STATS_PRESENT   0x08
typedef struct _rrc_oam_erab_stats_info_t
{
    U16          bitmask;    /*^ BITMASK ^*/
    U16          sampling_period;
    /*^ M, 0, L, 1, 0 ^*/
} rrc_oam_erab_stats_info_t;


#define RRC_OAM_STATISTICS_REQ_INTRA_ENB_HO_RELATED_CONFIG_STATS_PRESENT   0x01
#define RRC_OAM_STATISTICS_REQ_INTER_ENB_HO_RELATED_CONFIG_STATS_PRESENT   0x02
#define RRC_OAM_STATISTICS_REQ_MEAS_ON_NEIGH_CELL_HO_CONFIG_STATS_PRESENT  0x04
#define RRC_OAM_STATISTICS_REQ_INTRA_FREQ_HO_RELATED_CONFIG_STATS_PRESENT  0x08
#define RRC_OAM_STATISTICS_REQ_INTER_FREQ_HO_RELATED_CONFIG_STATS_PRESENT  0x10
#define RRC_OAM_STATISTICS_REQ_DRX_NON_DRX_HO_RELATED_CONFIG_STATS_PRESENT 0x20
#define RRC_OAM_STATISTICS_REQ_INTER_RAT_HO_CONFIG_STATS_PRESENT           0x40
#define RRC_OAM_STATISTICS_REQ_HO_FAILURE_NUM_CONFIG_STATS_PRESENT         0x80
typedef struct _rrc_oam_handover_stats_info_t
{
    U16          bitmask;    /*^ BITMASK ^*/
    U16          sampling_period;
    /*^ M, 0, L, 1, 0 ^*/
} rrc_oam_handover_stats_info_t;

typedef struct _rrc_sampling_period_info_t
{
    U16      sampling_period;
    /*^ M, 0, L, 1, 0 ^*/
} rrc_sampling_period_info_t;


#define RRC_OAM_RRC_CONNECTION_RELATED_STATS_PRESENT          0x01
#define RRC_OAM_ERAB_RELATED_STATS_PRESENT                    0x02
#define RRC_OAM_HANDOVER_RELATED_STATS_PRESENT                0x04
#define RRC_OAM_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT        0x08
#define RRC_OAM_PAGING_RELATED_STATS_PRESENT                  0x10
#define RRC_OAM_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT     0x20 
typedef struct _rrc_oam_config_stats_req_t
{
    U16                                    bitmask;    /*^ BITMASK ^*/
   
    U8                                     rrc_periodic_reporting;
    /*^ M, 0, H, 0, 1 ^*/  /* rrc_oam_stats_periodicity_et */

    rrc_oam_rrc_connection_stats_info_t    rrc_connection_stats;
/*^ O, RRC_OAM_RRC_CONNECTION_RELATED_STATS_PRESENT, N, 0, 0 ^*/

    rrc_oam_erab_stats_info_t              rrc_erab_stats;
/*^ O, RRC_OAM_ERAB_RELATED_STATS_PRESENT, N, 0, 0 ^*/

    rrc_oam_handover_stats_info_t          rrc_handover_stats;
/*^ O, RRC_OAM_HANDOVER_RELATED_STATS_PRESENT, N, 0, 0 ^*/

    rrc_sampling_period_info_t             rrc_ue_assoc_s1_conn_stats;
/*^ O, RRC_OAM_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT, N, 0, 0 ^*/

    rrc_sampling_period_info_t             rrc_paging_stats;
/*^ O, RRC_OAM_PAGING_RELATED_STATS_PRESENT, N, 0, 0 ^*/

    rrc_sampling_period_info_t             rrc_IratIncMobility_LAI_stats;
/*^ O, RRC_OAM_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT, N, 0, 0 ^*/

} rrc_oam_config_stats_req_t;   /*^ API, RRC_OAM_CONFIG_STATS_REQ ^*/

/******************************************************************************
*   RRC_OAM_CONFIG_STATS_RESP
******************************************************************************/
typedef struct _rrc_oam_config_stats_resp_t
{
    U8                      response; 
    /*^ M, 0, H, 0, 1 ^*/    /* rrc_return_et */
} rrc_oam_config_stats_resp_t;  /*^ API, RRC_OAM_CONFIG_STATS_RESP ^*/

/******************************************************************************
*   RRC_OAM_RRC_CONNECTION_STATS_IND
******************************************************************************/
#define RRC_OAM_STATISTICS_CONNECTION_ESTB_IND_STATS_PRESENT        0x01
#define RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_IND_STATS_PRESENT     0x02
#define RRC_OAM_STATISTICS_CONNECTION_NUMBER_IND_STATS_PRESENT      0x04
#define RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_IND_STATS_PRESENT  0x08
#define RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_IND_STATS_PRESENT     0x10
typedef struct _rrc_connection_stats_t
{   
    U16                 bitmask;    /*^ BITMASK ^*/

    rrc_cell_index_t    cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/ /* MAX_NUM_CELLS - 1 */
    
    U16             sampling_period;
    /*^ M, 0, L, 1, 0 ^*/

    rrc_uecc_oam_stat_connection_establishment_t    rrc_connection_establishment;
/*^ O, RRC_OAM_STATISTICS_CONNECTION_ESTB_IND_STATS_PRESENT, N, 0, 0 ^*/

    rrc_uecc_oam_stat_connection_reestablishment_t  rrc_connection_reestablishment;
/*^ O, RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_IND_STATS_PRESENT, N, 0, 0 ^*/

    U32  rrcConnectionNumber;
/*^ O, RRC_OAM_STATISTICS_CONNECTION_NUMBER_IND_STATS_PRESENT, N, 0, 0 ^*/

    rrc_connEstabTime_t  rrc_connEstabTime_cause[RRC_ESTABLISHMENT_CAUSE_INDEX]; 
/*^ O, RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_IND_STATS_PRESENT, OCTET_STRING, FIXED ^*/    

    rrc_uecc_oam_stat_ue_context_release_t          ue_contxt_rel_stats;
/*^ O, RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_IND_STATS_PRESENT, N, 0, 0 ^*/

} rrc_connection_stats_t;

typedef struct  _rrc_oam_rrc_connection_stats_ind_t
{
    U16             count;
    /*^ M, 0, B, 1, MAX_NUM_CELLS ^*/  /* MAX_NUM_CELLS */
    rrc_connection_stats_t  rrc_connection[MAX_NUM_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_oam_rrc_connection_stats_ind_t;   /*^ API, RRC_OAM_RRC_CONNECTION_STATS_IND ^*/

/******************************************************************************
*   RRC_OAM_HANDOVER_STATS_IND
******************************************************************************/
#define RRC_OAM_INTRA_ENB_HO_IND_STATS_PRESENT              0x01
#define RRC_OAM_INTER_ENB_HO_IND_STATS_PRESENT              0x02
#define RRC_OAM_MEAS_ON_NEIGH_CELL_HO_IND_STATS_PRESENT     0x04
#define RRC_OAM_INTRA_FREQ_HO_IND_STATS_PRESENT             0x08
#define RRC_OAM_INTER_FREQ_HO_IND_STATS_PRESENT             0x10
#define RRC_OAM_DRX_NON_DRX_HO_IND_STATS_PRESENT            0x20
#define RRC_OAM_INTER_RAT_HO_IND_STATS_PRESENT              0x40
#define RRC_OAM_HO_FAILURE_NUM_IND_STATS_PRESENT            0x80
typedef struct  _rrc_handover_stats_t
{
    U16                                      bitmask;    /*^ BITMASK ^*/

    rrc_cell_index_t                         cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/ /* MAX_NUM_CELLS - 1 */
    
    U16                                      sampling_period;
    /*^ M, 0, L, 1, 0 ^*/
    
    rrc_oam_intra_enb_ho_stats_t             intra_enb_ho_stats;
/*^ O, RRC_OAM_INTRA_ENB_HO_IND_STATS_PRESENT, N, 0, 0 ^*/

    rrc_oam_inter_enb_ho_stats_t             inter_enb_ho_stats;
/*^ O, RRC_OAM_INTER_ENB_HO_IND_STATS_PRESENT, N, 0, 0 ^*/

    rrc_oam_meas_on_neigh_cell_ho_stats_list_t    meas_on_neigh_cell_ho_stats_list;
/*^ O, RRC_OAM_MEAS_ON_NEIGH_CELL_HO_IND_STATS_PRESENT, N, 0, 0 ^*/

    rrc_oam_intra_freq_ho_stats_t            intra_freq_ho_stats;
/*^ O, RRC_OAM_INTRA_FREQ_HO_IND_STATS_PRESENT, N, 0, 0 ^*/

    rrc_oam_inter_freq_ho_stats_t            inter_freq_ho_stats;
/*^ O, RRC_OAM_INTER_FREQ_HO_IND_STATS_PRESENT, N, 0, 0 ^*/
    
    rrc_oam_drx_non_drx_ho_stats_t           drx_non_drx_ho_stats;
/*^ O, RRC_OAM_DRX_NON_DRX_HO_IND_STATS_PRESENT, N, 0, 0 ^*/
    
    /* BUG 12662 Fix Start */
    rrc_oam_inter_rat_ho_stats_list_t        inter_rat_ho_stats_list;
/*^ O, RRC_OAM_INTER_RAT_HO_IND_STATS_PRESENT, N, 0, 0 ^*/
    /* BUG 12662 Fix Stop */    

    U32                                      ho_failure_num;
/*^ O, RRC_OAM_HO_FAILURE_NUM_IND_STATS_PRESENT, N, 0, 0 ^*/    
} rrc_handover_stats_t; 

typedef struct  _rrc_oam_handover_stats_ind_t
{
    U16                     count;
    /*^ M, 0, B, 1, MAX_NUM_CELLS ^*/  /* MAX_NUM_CELLS */
    rrc_handover_stats_t    handover_stats[MAX_NUM_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_oam_handover_stats_ind_t; /*^ API, RRC_OAM_HANDOVER_STATS_IND ^*/

/******************************************************************************
*   RRC_OAM_ERAB_STATS_IND
******************************************************************************/
#define RRC_OAM_STATISTICS_ERAB_SETUP_IND_STATS_PRESENT    0x01
#define RRC_OAM_STATISTICS_ERAB_RELEASE_IND_STATS_PRESENT  0x02
#define RRC_OAM_STATISTICS_ERAB_MODIFY_IND_STATS_PRESENT   0x04
#define RRC_OAM_STATISTICS_ERAB_NUMBER_IND_STATS_PRESENT   0x08
typedef struct _rrc_erab_stats_t
{
    U16                                     bitmask;    /*^ BITMASK ^*/
    
    rrc_cell_index_t                        cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/ /* MAX_NUM_CELLS - 1 */

    U16                                     sampling_period;
    /*^ M, 0, L, 1, 0 ^*/
    
    rrc_uecc_oam_stat_erab_setup_t          erab_setup_stats;
/*^ O, RRC_OAM_STATISTICS_ERAB_SETUP_IND_STATS_PRESENT, N, 0, 0 ^*/

    rrc_uecc_oam_stat_erab_release_t        erab_release_stats;
/*^ O, RRC_OAM_STATISTICS_ERAB_RELEASE_IND_STATS_PRESENT, N, 0, 0 ^*/

    rrc_uecc_oam_stat_erab_modification_t   erab_mod_stats;
/*^ O, RRC_OAM_STATISTICS_ERAB_MODIFY_IND_STATS_PRESENT, N, 0, 0 ^*/

    rrc_uecc_oam_stat_erab_number_t         erab_sim_num_stats;
/*^ O, RRC_OAM_STATISTICS_ERAB_NUMBER_IND_STATS_PRESENT, N, 0, 0 ^*/

} rrc_erab_stats_t;

typedef struct _rrc_oam_erab_stats_ind_t
{
    U16                     count;
    /*^ M, 0, B, 1, MAX_NUM_CELLS ^*/  /* MAX_NUM_CELLS */
    rrc_erab_stats_t        erab_stats[MAX_NUM_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_oam_erab_stats_ind_t;     /*^ API, RRC_OAM_ERAB_STATS_IND ^*/

/******************************************************************************
*   RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND
******************************************************************************/
typedef struct _rrc_ue_assoc_s1_conn_stats_t
{
    rrc_cell_index_t    cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/ /* MAX_NUM_CELLS - 1 */
    U16         sampling_period;
    /*^ M, 0, L, 1, 0 ^*/
    U32 s1sig_connEstabAtt;
    /*^ M, 0, N, 0, 0 ^*/
    U32 s1sig_connEstabSucc;
    /*^ M, 0, N, 0, 0 ^*/
}rrc_ue_assoc_s1_conn_stats_t;

typedef struct _rrc_oam_ue_assoc_s1_conn_stats_ind_t
{
    U16         count;
    /*^ M, 0, B, 1, MAX_NUM_CELLS ^*/  /* MAX_NUM_CELLS */
    rrc_ue_assoc_s1_conn_stats_t   
        ue_assoc_s1_conn_stats[MAX_NUM_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_oam_ue_assoc_s1_conn_stats_ind_t;     /*^ API, RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND ^*/

/******************************************************************************
*   RRC_OAM_PAGING_STATS_IND
******************************************************************************/
typedef struct _rrc_paging_stats_t
{
    rrc_cell_index_t    cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/ /* MAX_NUM_CELLS - 1 */
    U16         sampling_period;
    /*^ M, 0, L, 1, 0 ^*/
    U32 pag_discardedNbr;
    /*^ M, 0, N, 0, 0 ^*/
    U32 pag_succNbr;
    /*^ M, 0, N, 0, 0 ^*/
    
    /*CR 025 changes start */
    U32 pag_receivedNbr;
    /*^ M, 0, N, 0, 0 ^*/
    
    /*CR 025 changes stop */
}rrc_paging_stats_t;

typedef struct _rrc_oam_paging_stats_ind_t
{
    U16         count;
    /*^ M, 0, B, 1, MAX_NUM_CELLS ^*/  /* MAX_NUM_CELLS */
    rrc_paging_stats_t   rrc_paging_stats[MAX_NUM_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_oam_paging_stats_ind_t;   /*^ API, RRC_OAM_PAGING_STATS_IND ^*/

/******************************************************************************
*  RRC_OAM_IRAT_MOBILITY_STATS_IND 
******************************************************************************/
typedef struct _rrc_irat_mobility_stats_t
{
    rrc_cell_index_t    cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/ /* MAX_NUM_CELLS - 1 */
    U16         sampling_period;
    /*^ M, 0, L, 1, 0 ^*/
    U32 rrc_IratIncMobility_LAI;
    /*^ M, 0, N, 0, 0 ^*/
}rrc_irat_mobility_stats_t;

typedef struct _rrc_oam_irat_mobility_stats_ind_t
{
    U16         count;
    /*^ M, 0, B, 1, MAX_NUM_CELLS ^*/  /* MAX_NUM_CELLS */
    rrc_irat_mobility_stats_t  rrc_irat_mobility_stats[MAX_NUM_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_oam_irat_mobility_stats_ind_t;   /*^ API, RRC_OAM_IRAT_MOBILITY_STATS_IND ^*/

/******************************************************************************
*   RRC_OAM_GET_CELL_STATUS_REQ
******************************************************************************/
typedef struct _rrc_oam_get_cell_status_req_t
{
    rrc_cell_index_t    cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/  /* MAX_NUM_CELLS - 1 */ 

} rrc_oam_get_cell_status_req_t; /*^ API, RRC_OAM_GET_CELL_STATUS_REQ ^*/

/******************************************************************************
*   RRC_OAM_GET_CELL_STATUS_RESP
******************************************************************************/
typedef struct _rrc_oam_cell_status_t
{
    U32                 num_ue;         /*^ M, 0, N, 0, 0 ^*/
    U32                 num_srb1;       /*^ M, 0, N, 0, 0 ^*/
    U32                 num_srb2;       /*^ M, 0, N, 0, 0 ^*/
    U32                 num_drb;        /*^ M, 0, N, 0, 0 ^*/
} rrc_oam_cell_status_t;

#define RRC_OAM_CELL_STATUS_PRESENT 0x01

typedef struct _rrc_oam_get_cell_status_resp_t
{
    U16                     bitmask;    /*^ BITMASK ^*/

    rrc_cell_index_t        cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/     /* MAX_NUM_CELLS - 1 */

    U8                      response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

    rrc_oam_cell_status_t   cell_status;
/*^ O, RRC_OAM_CELL_STATUS_PRESENT, N, 0, 0 ^*/

} rrc_oam_get_cell_status_resp_t; /*^ API, RRC_OAM_GET_CELL_STATUS_RESP ^*/

/******************************************************************************
*   RRC_OAM_GET_UE_STATUS_REQ
******************************************************************************/
#define RRC_OAM_UE_INDEX_PRESENT            0x01

typedef struct _rrc_oam_get_ue_status_req_t
{
    U16                     bitmask;    /*^ BITMASK ^*/

    rrc_cell_index_t        cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/     /* MAX_NUM_CELLS - 1 */

    rrc_ue_index_t          ue_index;
/*^ O, RRC_OAM_UE_INDEX_PRESENT, N, 0, 0 ^*/

} rrc_oam_get_ue_status_req_t; /*^ API, RRC_OAM_GET_UE_STATUS_REQ ^*/

/******************************************************************************
*   RRC_OAM_GET_UE_STATUS_RESP
******************************************************************************/
#define RRC_OAM_UE_STATUS_MME_UE_S1AP_ID_PRESENT   0x01

typedef struct _rrc_oam_ue_status_t
{
    U16                     bitmask;    /*^ BITMASK ^*/
    rrc_ue_index_t          ue_index;   /*^ M, 0, N, 0, 0 ^*/
    U16                     crnti;      /*^ M, 0, N, 0, 0 ^*/
    U8                      ue_state;   /*^ M, 0, N, 0, 0 ^*/
    U8                      ue_category;/*^ M, 0, N, 0, 0 ^*/
    U8                      num_srb;
/*^ M, 0, B, 1, 3 ^*/ /* RRC_MAX_NUM_SRB */

    U8                      num_drb;
/*^ M, 0, H, 0, 11 ^*/ /* RRC_MAX_NUM_DRB */

    U32                     enb_ue_s1ap_id; /*^ M, 0, H, 0, 16777215 ^*/
    U32                     mme_ue_s1ap_id;
/*^ O, RRC_OAM_UE_STATUS_MME_UE_S1AP_ID_PRESENT, N, 0, 0 ^*/

} rrc_oam_ue_status_t;

typedef struct _rrc_oam_ue_status_list_t
{
    U16                     num_ue_status;

	/* SPR_17664_start*/
/*^ M, 0, H, 0, RRC_MAX_NUM_UE_STATUS ^*/ /* RRC_MAX_NUM_UE_STATUS */

	/* SPR_17664_end*/
    rrc_oam_ue_status_t     ue_status[RRC_MAX_NUM_UE_STATUS];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} rrc_oam_ue_status_list_t;

#define RRC_OAM_UE_STATUS_PRESENT           0x01

typedef struct _rrc_oam_get_ue_status_resp_t
{
    U16                         bitmask;        /*^ BITMASK ^*/

    rrc_cell_index_t            cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/         /* MAX_NUM_CELLS - 1 */

    U8                          response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

    rrc_oam_ue_status_list_t    ue_status_list;
/*^ O, RRC_OAM_UE_STATUS_PRESENT, N, 0, 0 ^*/

} rrc_oam_get_ue_status_resp_t; /*^ API, RRC_OAM_GET_UE_STATUS_RESP ^*/

/******************************************************************************
*   RRC_OAM_GET_PS_STATS_REQ
******************************************************************************/

typedef struct _rrc_oam_get_ps_stats_req_t
{
    rrc_cell_index_t        cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/     /* MAX_NUM_CELLS - 1 */

} rrc_oam_get_ps_stats_req_t; /*^ API, RRC_OAM_GET_PS_STATS_REQ ^*/

/******************************************************************************
*   RRC_OAM_GET_PS_STATS_RESP
******************************************************************************/

typedef struct _rrc_oam_get_ps_stats_resp_t
{
    rrc_cell_index_t            cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/         /* MAX_NUM_CELLS - 1 */

    U8                          response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

    oam_rrc_ps_stats_resp_t    ps_stats_resp;

/*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_oam_get_ps_stats_resp_t; /*^ API, RRC_OAM_GET_PS_STATS_RESP ^*/
/******************************************************************************
*   RRC_OAM_RESET_CELL_STATS_REQ
******************************************************************************/
typedef struct _rrc_oam_reset_cell_stats_req_t
{
    rrc_cell_index_t    cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/ /* MAX_NUM_CELLS - 1 */

} rrc_oam_reset_cell_stats_req_t; /*^ API, RRC_OAM_RESET_CELL_STATS_REQ ^*/

/******************************************************************************
*   RRC_OAM_RESET_CELL_STATS_RESP
******************************************************************************/
typedef struct _rrc_oam_reset_cell_stats_resp_t
{
    rrc_cell_index_t    cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/  /* MAX_NUM_CELLS - 1 */

    U8                  response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} rrc_oam_reset_cell_stats_resp_t; /*^ API, RRC_OAM_RESET_CELL_STATS_RESP ^*/

/******************************************************************************
*   RRC_OAM_CLEANUP_RESP
******************************************************************************/
typedef struct _rrc_oam_cleanup_resp_t
{
    U8                  response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} rrc_oam_cleanup_resp_t; /*^ API, RRC_OAM_CLEANUP_RESP ^*/

/******************************************************************************
*   RRC_OAM_INIT_IND
******************************************************************************/
typedef struct _rrc_oam_init_ind_t
{
/* SPR_18211 start */
    U8 dummy; /*^ M, 0, NOT_PRESENT_IN_MESSAGE ^*/
/* SPR_18211 stop */
} rrc_oam_init_ind_t; /*^ API, EMPTY, RRC_OAM_INIT_IND ^*/

/******************************************************************************
*   RRC_OAM_CELL_TRAFFIC_TRACE_START
******************************************************************************/
typedef struct _rrc_oam_trace_collection_entity_t
{
    U8     length; /*^ M, 0, H, 0, 20  ^*/

    U8     data[MAX_TRANSPORT_LAYER_ADDR_SIZE];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_oam_trace_collection_entity_t;

typedef struct _rrc_oam_cell_traffic_trace_start_t
{
    rrc_cell_index_t    cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/  /* MAX_NUM_CELLS - 1 */
    U8                  interface_to_trace;
/*^ M, 0, H, 0, 7 ^*/  /* first bit S1-MME, second bit X2, third bit Uu */  
    U8                  trace_depth;
/*^ M, 0, H, 0, 5 ^*/  
    rrc_oam_trace_collection_entity_t trace_collection_entity;
    
    U8                  traceRef[TRACE_REF_LENGTH] ;
/*^ M, 0, OCTET_STRING, FIXED ^*/

} rrc_oam_cell_traffic_trace_start_t; /*^ API, RRC_OAM_CELL_TRAFFIC_TRACE_START ^*/

/******************************************************************************
*   RRC_OAM_CELL_TRAFFIC_TRACE_STOP
******************************************************************************/
typedef struct _rrc_oam_cell_traffic_trace_stop_t
{
    rrc_cell_index_t    cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/  /* MAX_NUM_CELLS - 1 */
} rrc_oam_cell_traffic_trace_stop_t; /*^ API, RRC_OAM_CELL_TRAFFIC_TRACE_STOP ^*/

/******************************************************************************
*   RRC_OAM_CELL_TRAFFIC_TRACE_START_ERROR_IND
******************************************************************************/
typedef struct _rrc_oam_cell_traffic_trace_start_error_ind_t
{
    rrc_cell_index_t    cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/  /* MAX_NUM_CELLS - 1 */
   /* SPR 22584 Changes - Starts */
    U8                  cause;
/*^ M, 0, N, 0, 0 ^*/
   /* SPR 22584 Changes - Ends */

} rrc_oam_cell_traffic_trace_start_error_ind_t; /*^ API, RRC_OAM_CELL_TRAFFIC_TRACE_START_ERROR_IND ^*/

/******************************************************************************
*   RRC_OAM_TRACE_FILE_TRANSFER_START_IND
******************************************************************************/

typedef struct _rrc_oam_e_utran_trace_id_t
{
    U8     trace_reference[RRC_TRACE_REFERENCE_LENGTH];
/*^ M, 0, OCTET_STRING, FIXED ^*/
    U16    trace_recording_session_reference;
/*^ M, 0, N, 0, 0 ^*/
} rrc_oam_e_utran_trace_id_t;

typedef struct _rrc_oam_trace_file_t
{
/* SPR-18445 start */
    U16                 len_file_name;
/*^ M, 0, B, 1, MAX_FILE_NAME_SIZE ^*/ 
/* SPR-18445 stop */
    U8                  file_name[MAX_FILE_NAME_SIZE];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_oam_trace_file_t;

typedef struct _rrc_oam_trace_file_transfer_start_ind_t
{
    rrc_cell_index_t    cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/  /* MAX_NUM_CELLS - 1 */
    rrc_ue_index_t      ue_index;   /*^ M, 0, N, 0, 0 ^*/
    rrc_oam_e_utran_trace_id_t e_utran_trace_id;
    rrc_oam_trace_file_t trace_file;
/*^ M, 0, N, 0 , 0 ^*/

    rrc_oam_trace_collection_entity_t trace_collection_entity;
/*^ M, 0, N, 0, 0 ^*/

} rrc_oam_trace_file_transfer_start_ind_t; /*^ API, RRC_OAM_TRACE_FILE_TRANSFER_START_IND ^*/

/******************************************************************************
*   RRC_OAM_TRACE_FILE_TRANSFER_COMPLETE_IND
******************************************************************************/
typedef struct _rrc_oam_trace_file_transfer_complete_ind_t
{
    rrc_cell_index_t    cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/  /* MAX_NUM_CELLS - 1 */

    rrc_oam_trace_file_t trace_file;
/*^ M, 0, N, 0 , 0 ^*/
} rrc_oam_trace_file_transfer_complete_ind_t; /*^ API, RRC_OAM_TRACE_FILE_TRANSFER_COMPLETE_IND ^*/

/******************************************************************************
*   RRC_OAM_TRACE_START_ERROR_IND
******************************************************************************/
typedef struct _rrc_trace_failure_list_item_t
{
    rrc_ue_index_t       ue_index;
    U8                   cause; 
} rrc_trace_failure_list_item_t;

/* CSR_00040799 Fix Start */
typedef struct _rrc_trace_failure_list_t
{
	/* SPR_17664_start*/
    rrc_counter_t   counter;  /*^ M, 0, B, 1, RRC_MAX_NUM_SUPPORTED_UE ^*/ /* RRC_MAX_NUM_SUPPORTED_UE */
	/* SPR_17664_end*/
    rrc_trace_failure_list_item_t  item[RRC_MAX_NUM_SUPPORTED_UE]; /*^ M,0,OCTET_STRING,VARIABLE ^*/ 
} rrc_trace_failure_list_t;
/* CSR_00040799 Fix End */

typedef struct _rrc_oam_trace_start_error_ind_t
{
    rrc_cell_index_t    cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/  /* MAX_NUM_CELLS - 1 */
    rrc_trace_failure_list_t  trace_failure_list;
/*^ M, 0, N, 0, 0 ^*/
} rrc_oam_trace_start_error_ind_t; /*^ API, RRC_OAM_TRACE_START_ERROR_IND ^*/

/* LIPA start */
/******************************************************************************
*   RRC_OAM_ADD_LGW_REQ
******************************************************************************/
#define RRC_OAM_ADD_LGW_IPV4_ADDR_PRESENT   0x01
#define RRC_OAM_ADD_LGW_IPV6_ADDR_PRESENT   0x02
typedef struct _rrc_oam_add_lgw_req_t
{
    U8           bitmask;     /*^ BITMASK ^*/

    /* IPV4 addresses of LGW */
    U8           ip_addr[RRC_MAX_IP_ADDR_LEN]; 
    /*^ O,RRC_OAM_ADD_LGW_IPV4_ADDR_PRESENT,OCTET_STRING,FIXED ^*/

    /* IPV6 addresses of LGW */
    U8           ipv6_addr[RRC_MAX_IP_ADDR_LEN_IPV6];
    /*^ O,RRC_OAM_ADD_LGW_IPV6_ADDR_PRESENT,OCTET_STRING,FIXED ^*/
} rrc_oam_add_lgw_req_t; /*^ API, RRC_OAM_ADD_LGW_REQ ^*/

/******************************************************************************
*   RRC_OAM_ADD_LGW_RESP
******************************************************************************/
#define RRC_OAM_ADD_LGW_RESP_FAILURE_CAUSE_PRESENT   0x01
typedef struct _rrc_oam_add_lgw_resp_t
{
    U8           bitmask;     /*^ BITMASK ^*/

    U8           response;
    /*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

    U8           cause; 
    /*^ O, RRC_OAM_ADD_LGW_RESP_FAILURE_CAUSE_PRESENT, B, 1, 3 ^*/ /* rrc_oam_add_lgw_fail_cause_et */
} rrc_oam_add_lgw_resp_t; /*^ API, RRC_OAM_ADD_LGW_RESP ^*/

/******************************************************************************
*   RRC_OAM_DELETE_LGW_REQ
******************************************************************************/
typedef struct _rrc_oam_delete_lgw_req_t
{
    U8           bitmask;     /*^ BITMASK ^*/

} rrc_oam_delete_lgw_req_t; /*^ API, RRC_OAM_DELETE_LGW_REQ ^*/

/******************************************************************************
*   RRC_OAM_DELETE_LGW_RESP
******************************************************************************/
#define RRC_OAM_DELETE_LGW_RESP_FAILURE_CAUSE_PRESENT   0x01
typedef struct _rrc_oam_delete_lgw_resp_t
{
    U8           bitmask;     /*^ BITMASK ^*/

    U8           response;
    /*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

    U8           cause; 
    /*^ O, RRC_OAM_DELETE_LGW_RESP_FAILURE_CAUSE_PRESENT, B, 1, 2 ^*/ /* rrc_oam_delete_lgw_fail_cause_et */
} rrc_oam_delete_lgw_resp_t; /*^ API, RRC_OAM_DELETE_LGW_RESP ^*/

/* LIPA end */

/*spr 22474 start*/
/* This structure provides the POOL stats */
typedef struct rrc_PoolStatsT {
    /* the size of the buffer allocated in this pool */
    U32 bufSize; /*^ M, 0, N, 0, 0 ^*/
    /* the Total number of Buffers in this pool */
    U32 numOfBuf; /*^ M, 0, N, 0, 0 ^*/
    /* the Total number of Buffers allocated from this pool */
    U32 numOfAllocBuf; /*^ M, 0, N, 0, 0 ^*/
    /* the Peak of Total number of Buffers allocated from this pool during the system is running */
    U32 numOfPeakBuf; /*^ M, 0, N, 0, 0 ^*/
}rrc_PoolStats;

/* This structure provides the MEM POOL stats */
typedef struct rrc_MemPoolStatsT {
    /* This array is terminated when bufSize is 0 for an entry, other entries are not valid */
    rrc_PoolStats stats[NVARPOOL]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrc_MemPoolStats;

/* RRC_OAM_GET_DEBUG_INFO_REQ */
/*! \typedef rrc_oam_get_debug_info_req_t 
 *  \brief   This API is sent from OAM to RRC
 *           to request for debug info
 */
typedef struct _rrc_oam_get_debug_info_req_t
{
   U16                   debug_type;/*^ M, 0, N, 0, 0 ^*/
   /*rrc_oam_debug_info_req_et*/
}rrc_oam_get_debug_info_req_t; /*^ API, RRC_OAM_GET_DEBUG_INFO_REQ ^*/

/* RRC_OAM_GET_DEBUG_INFO_RESP */
/*! \typedef rrc_oam_get_debug_info_resp_t
 *  \brief   This API is sent from RRC to OAM
 *           in the response of rrc_oam_get_debug_info_req_t 
 */
#define RRC_OAM_DEBUG_GET_MEM_POOL_PRESENT 0x01
#define RRC_OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT 0x02
typedef struct _rrc_oam_get_debug_info_resp_t
{
  U16             bitmask;                        /*^ BITMASK ^*/
  S32  response;/*^ M, 0, N, 0, 0 ^*/
 S32  fail_cause;/*^ O, RRC_OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT, N, 0, 0 ^*/ 
  /* rrc_oam_debug_info_fail_cause_et */
  rrc_MemPoolStats csplMemPoolStats;/*^ O, RRC_OAM_DEBUG_GET_MEM_POOL_PRESENT, N, 0, 0 ^*/
}rrc_oam_get_debug_info_resp_t;/*^ API, RRC_OAM_GET_DEBUG_INFO_RESP ^*/
/*spr 22474 end*/

#pragma pack(pop)

#endif /* __RRC_OAM_INTF__ */

