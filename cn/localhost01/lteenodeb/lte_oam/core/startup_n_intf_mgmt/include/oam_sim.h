/* vim:ts=4:et:ai:ci:sw=4
*/
/********************************************************************************
* File Name        : oam_sim.h 
* Description      : This file contains the enums, struct, constants definitions
*                    for SIM module
* Revision History :
* Date              Author                Reference            Comments
* May,2012          Brijesh madolia       OAM Design Document  Initial Version
* November,2012     Vivek Kumar Gupta     OAM 1.0              Changes for the new RRH module
* January,2013      Vinod Vyas            OAM Design Doc       Changes for SDM
*                                         Release 2.0
* September,2013    Raveen Sharma         CSR 51606 Fix        Code Changes for CSR Fix
* Nov, 2013        Vinod Vyas              CSR 00051709 Fix
* Jan, 2014        Vinod Vyas             CSR 00058746 Fix      
* Jan, 2014        Ravi Gupta             CSR 00058940 Fix
* April, 2014      Ankita Jain            CSR 00058157 Fix 
* May, 2014        Ankita Jain            SPR 8158 Fix
* June, 2014       Ankita Jain            SPR 11590 Fix
* June, 2014       Swetank Srivastava     SPR 11314 Fix
* June, 2014       Vivek K Gupta          Release 3.0         Changes for xml 
*                                                             add/delete frwk code
* June, 2014        Aditi Singh           SPR 11519 Fix
* July 2014        Vinod Vyas          SPR 12200 Fix
* July 2014         Shalu Sagar         SPR 12378 Fix
* July,2014     Aditi Singh           SPR 9620 Fix                  Guard timer Added
* July,2014        Ankita Jain           SPR 9485 Fix                  GU_GROUP_ID related enhancement
*AUG 2014       Shalu Sagar             SPR 13020  FIX
* Oct,2014        Ankita Jain           SPR 13586 Fix                 
* Sep 2014       Vinod Vyas        Changes Related to eICIC 
* Nov 2014      Karuna Bhardwaj    eICIC Enhacement
* Copyright (c) 2014, Aricent Inc. All Rights Reserved
*********************************************************************************/
#ifndef _OAM_SIM_H_ 
#define _OAM_SIM_H_

/****************************************************************************
 **  INCLUDES
 ****************************************************************************/
#include <oam_types.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <rrm_oam_defines.h>
#include <libxml/tree.h>
#include <oam_mgmt_intf_api.h>
/* SPR 15238 START */
#include <oam_xml_header.h>
/* SPR 15238 END */
/****************************************************************************
 **  MACROS
 ****************************************************************************/
 /* SPR 16567 START */
#define MAX_RETRY_COUNT                                                 4
 /* SPR 16567 END */
#define MAX_INTF_LIST                                                 500 
#define MAX_BUF_IPADDR                                                 64
#define MAX_BUF_COMMAND                                                64
#define MAX_NO_IP                                                       6
#define MAX_INTF_NAME                                                   5

#define OAM_ENB_CELL_ACTIVE                                           0
#define OAM_ENB_CELL_INACTIVE                                         1

/*SPR 21889 +- */

/* SPR21708 Changes Start */
#define PROCESS_TBL_INDEX_GTPU                                        2
/* SPR21708 Changes End */

/* SPR21717 Changes Start */
#define GTPU_NUM_FILE_INDEX                                           2
#define GTPU_LOG_FILE_SIZE_INDEX                                      3
#define GTPU_LOG_LEVEL_INDEX                                          4
/* SPR21717 Changes End */

#if !defined(OAM_EVM_TARGET) || defined(AUTOMATED_STARTUP)
    #define PROCESS_TBL_INDEX_RRM                                     3
#else
    #define PROCESS_TBL_INDEX_RRM                                     2
#endif

#define PROCESS_CORE_NUM_INDEX                                        1


#define MAX_PARENT_NAME_LENGTH                                        256
#define MAX_PARAMETER_TABLE_SIZE                                      512
#define MAX_PRODUCT_INFO_LENGTH                                       128
/* SPR 13024 FIX START */
#define OAM_PHY_CELL_ID_MASK                               0x0000000000000001
/* SPR 13024 FIX END */
#define RRM_OAM_CELL_START_REQ_MASK                        0x0000000000000002
#define RRM_OAM_CELL_STOP_REQ_MASK                         0x0000000000000004
#define OAM_SON_RACH_OPT_CONFIG_PARAMS_MASK		           0x0000000000000008
/* Bitmask from 8 to 100 are used in RADIO HEAD SUPPORT */

#define OAM_FTP_PARAMS_MASK                                0x0000000000000010
#define OAM_RRM_CELL_UNBLOCK_REQ_MASK                      0x0000000000000020
#define OAM_RRM_CELL_BLOCK_REQ_MASK                        0x0000000000000040
#define OAM_FM_SUPP_ALARM_REP_MECH_SET_REQ_MASK            0x0000000000000080

#define OAM_SON_NW_PERIODIC_SCAN_EUTRAN_MASK               0x0000000000000100
#define OAM_ADMIN_STATE_CHANGED_MASK                       0x0000000000000200
#define RRM_OAM_CELL_SETUP_REQ_MASK                        0x0000000000000400
#define RRM_OAM_RAC_DISABLE_REQ_MASK                       0x0000000000000800

#define RRM_OAM_RAC_ENABLE_REQ_MASK                        0x0000000000001000
#define RRM_OAM_CELL_DELETE_REQ_MASK                       0x0000000000002000
/**** DROP 4 *****/
#define OAM_SON_ANR_MEAS_CONFIG_MASK                       0x0000000000004000
#define SON_OAM_NR_ADD_UPDATE_DELETE_REQ_MASK              0x0000000000008000

#define SON_OAM_NR_ADD_UPDATE_DELETE_REQ_MASK_INTER        0x0000000000010000
#define OAM_SON_NR_ENB_REQ_MASK                            0x0000000000020000
#define OAM_LOCAL_PARAMS_MASK                              0x0000000000040000
#define OAM_SON_ANR_MODIFY_MEAS_REPORTING_INTERVAL_MASK    0x0000000000080000

#define X2AP_OAM_GET_SCTP_STATUS_RESP_MASK                 0x0000000000100000
#define X2AP_OAM_PROVISION_REQ_MASK                        0x0000000000200000
#define OAM_SON_NW_SCAN_EUTRAN_MASK                        0x0000000000400000
#define OAM_SON_NR_SCAN_EUTRAN_MASK                        0x0000000000800000

#define OAM_SON_ANR_MODIFY_REMOVAL_ATTRIBUTES_MASK         0x0000000001000000
#define OAM_SON_ANR_DISABLE_MASK                           0x0000000002000000
#define S1AP_OAM_PROVISION_REQ_MASK                        0x0000000004000000
#define OAM_SON_ANR_MODIFY_PCI_CONFUSION_CFG_MASK          0x0000000008000000

#define RRC_OAM_PROVISION_REQ_MASK		                   0x0000000010000000
#define SON_OAM_NR_DELETE_FROM_REM_LIST_MASK               0x0000000020000000
#define OAM_SON_X2_ENABLE_MASK                             0x0000000040000000
#define OAM_SON_X2_DISABLE_MASK                            0x0000000080000000

#define SON_OAM_ES_MODIFY_UE_COUNT_INTERVAL_REQ_MASK       0x0000000100000000
#define SON_OAM_ES_MODIFY_UE_COUNT_THRESHOLD_REQ_MASK      0x0000000200000000
#define OAM_SON_ANR_MODIFY_UE_COUNT_MASK                   0x0000000400000000
#define OAM_TR069_PARAMS_MASK                              0x0000000800000000

/****** REL 2.1.0 SON ********/

#define OAM_MAC_MODIFY_REQ_MASK 		                   0x0000001000000000
/* SPR 13024 FIX START */
#define RRM_OAM_CELL_CONFIG_REQ_MASK                       0x0000002000000000
/* SPR 13024 FIX END */
#define RRM_OAM_GET_VER_ID_REQ_MASK                        0x0000004000000000
#define RRM_OAM_LOAD_CONFIG_REQ_MASK                       0x0000008000000000

#define OAM_SON_ANR_ENABLE_MASK                            0x0000010000000000
#define OAM_REF_SIG_POWER_LIST_MASK                        0x0000020000000000
#define OAM_EARFCN_DL_LIST_MASK                            0x0000040000000000
#define OAM_P_MAX_LIST_MASK                                0x0000080000000000

#define OAM_FM_CLEAN_HISTORY_REQ_MASK                      0x0000100000000000
#define OAM_FM_CLEAN_ALARM_LIST_REQ_MASK                   0x0000200000000000
 /* CSR 00051709 Change Start*/
#define OAM_S1AP_ADD_MME_REQ_MASK                          0x0000400000000000 
 /* CSR 00051709 Change End*/
#define SON_OAM_ES_AUTONOMOUS_CELL_SWITCH_OFF_REQ_MASK     0x0000800000000000 
#define SON_OAM_PCI_CONFLICT_ENABLE_DISABLE_MASK           0x0001000000000000
#define OAM_SON_MRO_MODIFY_CONFIG_PARAM_MASK          	   0x0002000000000000
#define OAM_SON_MLB_MODIFY_ATTRIBUTE_REQ_MASK		   0x0004000000000000
/* CSR 58157 START */ 
#define OAM_EARFCN_UL_LIST_MASK                            0x0008000000000000
/* CSR 58157 END */
/* SPR-8158 Start */
#define SON_OAM_ANR_MEAS_OBJECT_REMOVE_REQ_MASK            0x0010000000000000
/* SPR-13586 START */
#define SON_OAM_NR_ADD_UPDATE_DELETE_REQ_MASK_INTER_GERAN  0x0020000000000000
/* SPR-13586 END */
/* SPR 17880 START */
#define OAM_TR059_MANAGEMENT_SERVER_PARAM_MASK             0x0080000000000000
/* SPR 17880 END */
/* SPR-8158 End */
/* SPR-11590 Start */
//#define SON_OAM_PCI_CONFLICT_ENABLE_DISABLE_MASK           0x0004000000000000
/* SPR-16334 START */
#define X2AP_OAM_ENB_ADD_UPDATE_DELETE_REQ_MASK            0x0004000000000000
/* SPR-16334 END */
/* SPR_19279_START */
#define	RRM_OAM_PLMNLIST_CONFIG_REQ_MASK                   0x0100000000000000
/* SPR_19279_END */

/* SPR 20653 Fix Start */
#define OAM_SON_ANR_MODIFY_ATTRIBUTE_MASK                  0x0400000000000000
/* SPR 20653 Fix End */
/* SPR-11590 End */


/*eICIC_PHASE_1_2_CHANGES_START*/
#define OAM_SON_ANR_NEIGHBOR_DEFAULT_CONFIG_REQ_MASK         0x0010000000000000
/*SPR 14554 start*/
#define OAM_SON_INFO_IND_MASK                                0x0040000000000000
/*SPR 14554 end*/
/* SPR_19619 start */
#define OAM_SON_CELL_ACTIVATION_INFO_MASK		     0x0200000000000000
/* SPR_19619 stop */
/*eICIC_PHASE_1_2_CHANGES_END*/

#define OAM_MAX_LOOP_INDEX                                            65535  
#define FORK_ERROR                                                       -1
#define FORK_CHILD                                                        0
#define OAM_PHY_MAC_MESSAGE_ERROR_ONE_ID                                    161
#define OAM_PHY_MAC_MESSAGE_ERROR_TWO_ID                                    162
#define OAM_PHY_MAC_MESSAGE_ERROR_THREE_ID                                    163
#define OAM_PHY_MAC_MESSAGE_ERROR_FOUR_ID                                    164


#define OAM_MAX_COMM_REQ_INFO_BLOCK 	                                  4 
#define OAM_RESTART_VAL 		                                          1
#define OAM_FACTORY_RESET_VAL		                                      2
#define OAM_SDM_MODULE_ID		                                         26
/* SPR 21889 +- */ 
#define OAM_SDM_COMMAND_REQ		                                        407

#ifdef LTE_EMBMS_SUPPORTED
#define OAM_MAX_API_TO_LOWER_LAYER                                       64
#else
/* SPR-16334 START */  /* SPR 17880 START/END */
/* SPR_19619 start */
/* SPR 20653 Fix Start */
#define OAM_MAX_API_TO_LOWER_LAYER 59
/* SPR 20653 Fix End */
/* SPR_19619 stop */
/* SPR-16334 END */

#endif

/*eICIC_PHASE_1_2_CHANGES_START*/
/*bug_13878_start */
/*SPR 14554 start*/
//#define OAM_MAX_API_TO_LOWER_LAYER 56  
/*SPR 14554 end*/
/*bug_13878_end */
/*eICIC_PHASE_1_2_CHANGES_END*/
#define OAM_PARENT_NODE_GATEWAY 	"InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.Gateway."
#define OAM_PARENT_NODE_CRYPTO 		"InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1]."
#define OAM_PARENT_NODE_DEVICE_INFO	"InternetGatewayDevice.DeviceInfo."
#define OAM_PARENT_NODE_MGNT_SRVR	"InternetGatewayDevice.ManagementServer."
#define OAM_PARENT_NODE_REM	        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE."
#define OAM_MAX_NO_OF_COMMANDS      	                                 10
#define OAM_MAX_CMD_LENGTH_NAME     	                                 30
#define OAM_RRH_SET_RESP_PAYLOAD_LEN                                      8

#define OAM_NO_ATTRIBUTE                                         0x00000000

#define OAM_NMM_PRESENCE_FLAG                                             0 
/* Bitmask value of parameter_mode field below */
#define OAM_READ_ONLY                                            0x00000001
#define OAM_IS_CELL_SPEC_PARAM                                   0x00000004
#define MULTI_INSTANCE_PARAM                                     0x00000008
#define OAM_WRITE_ONLY                                           0x00000010
#define OAM_IGNORE_VALIDATION                                    0x00000020
/* SPR 13850 FIX START */
#define OAM_MANDATORY_PARAM                                      0x00000040
/* SPR 13850 FIX END */

#define OAM_TIME_SIZE_TR                                                 30
#define OAM_SON_BIT_1                                            0x00000001
#define OAM_SON_BIT_2                                            0x00000002
#define OAM_SON_BIT_3                                            0x00000004
#define OAM_SON_BIT_4                                            0x00000008
#define OAM_SON_BIT_5                                            0x00000010
#define OAM_SON_BIT_6                                            0x00000020
#define OAM_SON_BIT_7                                            0x00000040
#define OAM_SON_BIT_8                                            0x00000080
#define OAM_SON_BIT_9                                            0x00000100
#define OAM_SON_BIT_10                                           0x00000200
#define OAM_SON_BIT_11                                           0x00000400

#define OAM_SON_BIT_12                                           0x00000800
#define OAM_SON_BIT_13                                           0x00001000
#define OAM_SON_BIT_14                                           0x00002000

#define MAX_NW_SCAN_STATUS_STR_SIZE                                      32
/* CSR 00058746 Fix Start */
#define S1AP_LINK_UP_RCVD                              0x02
/* SPR 11243 Fix Start*/
#define CELL_START_RESP_RCVD                           0x04
/* SPR 11243 Fix End*/
/*SPR_15770 Fix start*/
#define NR_SCAN_RESP_RCVD                              0x08
/*SPR_15770 Fix end*/
/* CSR 00058746 Fix End*/
/* SPR 19938 Fix Start*/
#define INIT_TIME_NR_SCAN_REQ_SEND                     0x10
/* SPR 19938 Fix Stop*/


#define OAM_TCB_TEMP_DATA_LEN   15

/* SPR 16603 Fix Start */
/* Spr 15612  Fix Start*/
#define OAM_INTER_RAT_UTRAN_NEIGH_PARAMS_COUNT                          22
/* Spr 15612  Fix End*/ 
#define OAM_INTER_RAT_GSM_NEIGH_PARAMS_COUNT                            11
/* SPR 16603 Fix End */

/* Start: CSR 00058940 */
#define LOWER_LAYERS_PROVISIONING_MASK                   0x01
#define INIT_TIME_PCI_SELECTION_MASK                     0x02
#define INIT_TIME_CARRIER_FREQ_AND_POWER_SELECTION_MASK  0x04
/*End: CSR 00058940 */


/* Start: CSR 00058940 */
#define LOWER_LAYERS_PROVISIONING_MASK                   0x01
#define INIT_TIME_PCI_SELECTION_MASK                     0x02
#define INIT_TIME_CARRIER_FREQ_AND_POWER_SELECTION_MASK  0x04
/*End: CSR 00058940 */

/* Carrier Aggregation changes start */
#define OAM_RRC_MIN_NUM_SUPP_UE                                          1
/* SPR 20526 Fix Start */
#define OAM_RRC_MAX_NUM_SUPP_UE                                        MAX_UE_SUPPORTED
/* SPR 20526 Fix End */

/* Start: CSR 00058940 */
#define LOWER_LAYERS_PROVISIONING_MASK                   0x01
#define INIT_TIME_PCI_SELECTION_MASK                     0x02
#define INIT_TIME_CARRIER_FREQ_AND_POWER_SELECTION_MASK  0x04
/*End: CSR 00058940 */

#define RRC_UECC_INTRA_CELL_HO_GUARD_TIMER_MIN_VAL                      50
#define RRC_UECC_INTRA_CELL_HO_GUARD_TIMER_MAX_VAL                   65535

#define RRC_UECC_UE_RECONFIG_TIMER_MIN_VAL                              50
#define RRC_UECC_UE_RECONFIG_TIMER_MAX_VAL                           65535

#define RRC_UECC_CCO_GUARD_TIMER_MIN_VAL                                50
#define RRC_UECC_CCO_GUARD_TIMER_MAX_VAL                             65535

#define RRC_UECC_STORE_UE_CONTEXT_TIMER_MIN_VAL                         50
#define RRC_UECC_STORE_UE_CONTEXT_TIMER_MAX_VAL                      65535

#define RRC_UECC_UE_COUNTER_CHECK_TIMER_MIN_VAL                         50
#define RRC_UECC_UE_COUNTER_CHECK_TIMER_MAX_VAL                      65535

#define RRC_UECC_UE_INFORMATION_TIMER_MIN_VAL                           50
#define RRC_UECC_UE_INFORMATION_TIMER_MAX_VAL                        65535
/* Carrier Aggregation changes end */

extern UInt8 no_of_cells ; 

/****************************************************************************
 **  ENUMERATIONS / STRUCTURES
 ****************************************************************************/
/*SPR 21889 Start*/
typedef struct _oam_process_info_t
{
    char name[MAX_BIN_NAME_LEN];
    UInt64    cksum;  /* Checksum of this binary */
    pid_t pid;
    Char8 **pp_args;
    UInt8 is_remote; /* If set to true it indicates this process needs to be executed at remote location */
    UInt8 instance_id; /* Instance on which this binary needs to be executed. This field is applicable only when is_remote field is set to false */
    UInt8 remote_spawn_status; /* Applicable only when process need to be executed on remote location. oam_remote_spawn_status_et */
}oam_process_info_t;
/*SPR 21889 End */

typedef struct _oam_sim_table_t
{
    char    user_name[OAM_MAX_USERNAME];
    char    password[OAM_MAX_PASSWORD];
    char    name[MAX_BIN_NAME_LEN];
    oam_process_info_t 	*p_info;
    UInt8   max_no_process;
}oam_sim_table_t;

typedef struct _oam_bin_map_name_tbl
{
    Char8   *p_bin_nick_name;
    Char8   *p_bin_real_name;
}oam_bin_map_name_tbl_t;


typedef struct 
{
    Char8 parameter_name[MAX_PARENT_NAME_LENGTH + MAX_PARAMETER_NAME_LENGTH];
    void          *cell_spec_base_addr;
    void          *multi_inst_param_base_addr;
    void          *param_base_addr;
    UInt64        lower_layer_bitmask;
    oam_return_et (*validate)(U8 * , int, U8 *);
    void          (*parameter_bitmask)(int, void *);
    xmlNode      **xml_parameter_parent;
    Char8         *tag_name;      
    UInt8         parameter_type;	
    UInt8         parameter_mode;	
}parameter_map_element_t;


/* Function pointers for handling of xml object creation */
typedef oam_return_et (*obj_add_del_handler_t)(UInt8 add_del_op,
                                               void* p_msg,
                                               UInt16 instance_id,
                                               void* p_add_data,
                                               oam_error_code_et *p_err);

typedef oam_return_et (*obj_inst_id_gen_t)(UInt16 *p_instance_id,
                                           UInt16  max_entries,
                                           oam_error_code_et *p_err);
/* SPR 18930  FIX START */
typedef oam_return_et (*obj_data_acc_handler_t)(UInt16 **curr_entries,
/* SPR 18930  FIX END */        
                                                UInt16 *max_entries,
                                                xmlNode **obj_parent,
                                                xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                                xmlNode *** obj_struct_parent,
                                                UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                                oam_error_code_et *p_err);

typedef struct {
    Char8                    object_name[MAX_PARENT_NAME_LENGTH];
	/* SPR 16603 Fix start */
	UInt8                     no_of_parameter;
	/* SPR 16603 Fix end */
    parameter_map_element_t  *parameter_table_address;
    obj_data_acc_handler_t   obj_data_accessor;
    obj_add_del_handler_t    lower_layer_handler;
    obj_inst_id_gen_t        instance_id_gen;
    Char8                    *tag_name;
    Char8                    *current_entry;
    /* +- SPR 17439 */

} object_map_element_t;

typedef struct oam_cmd_map_element_t 
{
	UInt8 		    cmd_name[OAM_MAX_CMD_LENGTH_NAME];
	oam_return_et 	(*callback)(void*,UInt16,UInt8*);
	UInt32 		    lower_layer_bitmask;

}cmd_map_element_t;

/* SPR 15238 START */
typedef enum
{
    OAM_CLOSED,
    OAM_HYBRID,
    OAM_OPEN_MACRO,
    OAM_OPEN_FEMTO
} oam_access_mode_et;

typedef enum __oam_eutran_nbr_cell_type_et {
    OAM_INTRA_FREQ_NBR_CELL,
    OAM_INTER_FREQ_NBR_CELL
} oam_eutran_nbr_cell_type_et;

typedef struct _per_freq_pci_ranges_t
{
    U16 open_pci_range;
    U16 open_pci_start;
    U16 closed_pci_range;
    U16 closed_pci_start;
    U16 hybrid_pci_range;
    U16 hybrid_pci_start;
}per_freq_pci_ranges_t;
/* SPR 15238 END */

/* SPR 17596 fixed start */
struct _ns1__GetRPCMethodsResponse
{
    struct MethodList *MethodList;  /* required element of type ArrayOfstring */
};


struct MethodList
{
    char **__ptrstring;
    int __size;
};

/*SPR 17596 fixed end */
/****************************************************************************
 **  FUNCTION PROTOTYPES
 ****************************************************************************/
oam_bool_et oam_verify_load_cfg_req_from_rrm
(
 rrm_oam_serving_enb_cell_info_t *p_srv_cell_info
);

oam_bool_et oam_check_load_perctg
(
 UInt8 load_percntg_over,
 UInt8 load_percntg_high,
 UInt8 load_percntg_mid
);


/*TR069*/
oam_return_et
oam_send_control_cmd_resp
(
    UInt16  trans_id,
    UInt16  dest_id,
    UInt8   status,
    UInt16  error_code,
    UInt8  *err_string
);

oam_return_et
oam_construct_n_send_sdm_command_req
(
 	UInt8 	admin_cmd_restart_flag
);

oam_return_et
oam_handle_shutdown_cmd
(
    void  *p_api_buf,
    UInt16 param_len,	
    UInt8 *p_param_list
);


oam_return_et
oam_handle_restart_cmd
(
    void  *p_api_buf,
    UInt16 param_len,	
    UInt8 *p_param_list
);

oam_return_et
oam_handle_factory_reset_cmd
(
    void *p_api_buf,
    UInt16 param_len,	
    UInt8 *p_param_list
);


oam_return_et
oam_handle_init_config_cmd
(
    void *p_api_buf,
    UInt16 param_len,	
    UInt8 *p_param_list
);


oam_return_et
oam_handle_control_command_req
(
    void *p_api_buf
);

oam_return_et
oam_send_tr_069_init_config_req
(
    oam_init_tr069_params_t *p_oam_tr069_init_req,
    UInt16                   src_module_id,
    UInt16                   transaction_id,
    oam_error_code_et       *p_error_code,
	oam_device_info_t       *p_oam_device_info
);

oam_return_et                                                                 
oam_handle_tr069_init_resp                                             
(                                                                            
    void  *p_api_buf                                                         
);


oam_return_et
oam_handle_file_download_ind                                             
(                                                                           
    UInt8  *p_msg                                                          
);  

oam_return_et
oam_proceed_with_init_flow(SInt32 g_no_of_arguments,
                           Char8 **g_list_of_init_arguments);

void 
oam_construct_n_send_get_resp_to_usr
(
    UInt8              resp_status,
    UInt32             resp_error_code,
    UInt8              oam_version, 
    UInt16             src_id,
    UInt16             dst_id, 
    UInt16             req_trans_id,
    UInt8              api_version,
    UInt8              api_hdr_resvd_byte,
    char              *error_str,
    oam_error_code_et *p_err_code
);

void 
oam_construct_n_send_set_resp_to_usr
(
    UInt8              resp_result,
    UInt32             resp_error_code,
	UInt8             *err_string, 
    UInt8              err_string_len,
    UInt8              oam_version, 
    UInt16             src_id,
    UInt16             dst_id,  
    UInt16             req_trans_id,
    UInt8              api_version,
    UInt8              api_hdr_resvd_byte,
    oam_error_code_et *p_err_code
);

/*SPR 17596 fixed start */
struct MethodList *GetRPCMethods(void);


void
oam_tr069_compose_get_rpc_message(UInt8 *p_buffer);

void *my_calloc(unsigned int nmemb, unsigned int size);


oam_return_et
oam_get_rpc_api_handler
(
    void               *p_api_buf,
    UInt16              msg_len,
    oam_error_code_et  *p_error_code
);
void
oam_construct_n_send_get_rpc_resp_to_usr
(
    UInt8   resp_status,
    UInt32  resp_code,
    UInt8   oam_version,
    UInt16  src_id,
    UInt16  dst_id,
    UInt16  req_trans_id,
    char    *str,
    oam_error_code_et *p_err_code
);

/* SPR 17880 START */
oam_return_et oam_construct_n_send_set_parameter_ind
(
    oam_error_code_et       *p_error_code,
    UInt16                  transaction_id
);

oam_length_t
oam_get_spv_ind_msg_length(oam_error_code_et *p_error_code,
                           UInt8* no_of_params);

void
oam_tr069_compose_spv_ind_message(UInt8 *p_buffer, UInt8 no_of_params);

/* SPR 17880 END */
oam_return_et
oam_get_parameter_api_handler
(
    void               *p_api_buf,
    UInt16              msg_len,
    UInt16              src_module_id,
    oam_error_code_et  *p_error_code
);

/*   VALIDATE FUNCTIONS*/
oam_return_et
validate_dl_bandwidth
(
    U8  *dl_bandwidth,
    int loop,
    U8  *get_value_parameter	
);

oam_return_et
validate_phich_duration
(
    U8  *p_phich_duration,
    int loop,
    U8  *get_value_parameter	
);

oam_return_et
validate_phich_resource
(
    U8  *p_phich_resource,
    int loop,
    U8  *get_value_parameter	
);

oam_return_et
validate_mcc
(
    U8    *p_mcc,
    int    loop,
	UInt8 *val
);

oam_return_et
validate_mnc
(
    U8    *p_mnc,
    int    loop,
	UInt8 *val
);

oam_return_et
validate_reserve_operator_use
(
    U8*,
    int,
	UInt8 *val
);
oam_return_et
validate_tac
(
    U8*,
    int,
	UInt8 *val 
);

oam_return_et
validate_cell_identity
(
    U8    *p_cell_identity,
    int    loop,
	UInt8 *val
);

oam_return_et
validate_cell_barred
(
    U8     *p_cell_barred,
    int     loop,
	UInt8  *val
);

oam_return_et
validate_intra_freq_reselection
(
    U8     *p_intra_freq_reselection,
    int     loop,
	UInt8  *val
);
oam_return_et
validate_csg_id
(
    U8     *p_csg_id,
    int     loop,
	UInt8  *cval
);

void
set_bitmask_SIntraSearch
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_idle_mode_common_params
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_idle_mode_intra_freq_params
(
    int loop,
    void * p_cell_reconfig_req
);

void
oam_init_msg_ipcs
(
   void
);

void
set_bitmask_inter_rat_geran_prms
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_irat_geran_cell_res_pri
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_irat_geran_cell_res_pri
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_inter_rat_eutran_to_geran_prms
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_inter_rat_geran_speed_scale_prms
(
    int loop,
    void * p_cell_reconfig_req
);


/* SPR 11001 FIX START */
void
set_bitmask_load_config_overload_action
(
    int loop,
    void * p_load_config
);

void
set_bitmask_load_config_overload_lvl_prcnt
(
    int loop,
    void * p_load_config
);

/*bug_13969_start */
void 
set_bitmask_load_config_overload_eligible_ue
(    int loop,
    void * p_load_config

);

void
set_bitmask_load_config_overload_abs_pattern
(    int loop,
    void * p_load_config

);

void
set_bitmask_load_config_midload_eligible_ue
(    int loop,
    void * p_load_config

);

void
set_bitmask_load_config_midload_abs_pattern
(    int loop,
    void * p_load_config

);

void 
set_bitmask_load_config_highload_abs_pattern
(    int loop,
    void * p_load_config

);
void 
set_bitmask_load_config_highload_eligible_ue
(    int loop,
    void * p_load_config

);
/*bug_13969_end */
void
set_bitmask_load_config_highload_lvl_prcnt
(
    int loop,
    void * p_load_config
);

void
set_bitmask_load_config_highload_action
(
    int loop,
    void * p_load_config
);

void
set_bitmask_load_config_midload_action
(
    int loop,
    void * p_load_config
);

void
set_bitmask_load_config_midload_lvl_prcnt
(
    int loop,
    void * p_load_config
);
/* SPR 11001 FIX END */

void
set_bitmask_access_mgmt_prms 
(
    int                           loop,
    void   *p_cell_reconfig_req
);

void
set_bitmask_access_mgmt_param_csg_id
(
   int                           loop,
   void   *p_cell_reconfig_req
);

void
set_bitmask_rstx_power
(
    int                            loop,
    void   *p_cell_reconfig_req
);

void
set_bitmask_n1cs_params 
(
    int                           loop,
    void  *p_cell_reconfig_req
);

void
set_bimask_csg_id
(
    int                           loop,
    void  *p_cell_reconfig_req
);

oam_return_et
validate_q_rx_lev_min
(
    U8     *p_q_rx_lev_min,
    int     loop,
	UInt8  *cval
);

void
set_bitmask_inter_rat_utra_prms
(
    int                          loop,
    void *p_cell_reconfig_req   
);


oam_return_et
validate_q_rx_lev_min_offset
(
    U8     *p_q_rx_lev_min_offset,
    int     loop,
	UInt8  *cval
);

void
set_bitmask_q_rx_lev_min_offset
(
    int                          loop,
    void *p_cell_reconfig_req
);

oam_return_et
validate_p_max
(
    U8     *p_p_max,
    int     loop,
	UInt8  *cval
);

void
set_bitmask_p_max
(
    int                           loop,
    void  *p_cell_reconfig_req
);


oam_return_et
validate_q_rx_lev_min_offset
(
    U8     *p_q_rx_lev_min_offset,
    int     loop,
	UInt8  *cval
);

void
set_bitmask_q_rx_lev_min_offset
(
    int                           loop,
    void  *p_cell_reconfig_req
);

oam_return_et
validate_frequency_band_indicator
(
    U8     *p_frequency_band_indicator,
    int     loop,
	UInt8  *cval
);

oam_return_et
validate_number_of_ra_preambles
(
    U8    *p_number_of_ra_preambles,
    int    loop,
	UInt8  *cval
);
oam_return_et
validate_size_of_ra_group_a
(
    U8    *p_size_of_ra_group_a,
    int    loop,
	UInt8  *cval
);

oam_return_et
validate_message_size_group_a
(
    U8     *p_message_size_group_a,
    int     loop,
	UInt8  *cval
);

void
set_bitmask_preamble_info
(
    int                          loop,
    void *p_cell_reconfig_req
);

oam_return_et
validate_message_power_offset_group_b
(
    U8    *p_message_power_offset_group_b,
    int    loop,
	UInt8 *cval
);


oam_return_et
validate_power_ramping_step
(
    U8   *p_power_ramping_step,
    int   loop,
	UInt8 *cval
);


void
set_bitmask_message_power_offset_group_b
(
    int                          loop,
    void *p_cell_reconfig_req
);

oam_return_et
validate_preamble_initial_received_target_power
(
    U8     *p_preamble_initial_received_target_power,
    int     loop,
	UInt8  *cval
);

oam_return_et
validate_preamble_trans_max
(
    U8 *p_preamble_trans_max,
     int loop,
	UInt8* cval
);
oam_return_et
validate_response_window_size
(
    U8     *p_response_window_size,
    int     loop,
	UInt8  *cval
);

oam_return_et
validate_contention_res_timer
(
    U8    *p_contention_resolution_timer,
    int    loop,
	UInt8  *cval
);

oam_return_et
validate_max_harq_msg_3tx
(
    U8     *p_max_harq_msg_3tx,
    int     loop,
	UInt8  *cval
);


oam_return_et
validate_mod_period_coeff
(
    U8    *p_modification_period_coeff,
    int    loop,
	UInt8 *cval
);

void
set_bitmask_mod_period_coeff
(
    int                          loop,
    void *p_cell_reconfig_req
);

/*SPR 13970 FIX Start*/

void
set_bitmask_ac_barring_info
(
    int                          loop,
    void *p_cell_reconfig_req
 );

void
set_bitmask_ac_barring_for_mo_signalling
(
    int                          loop,
    void *p_cell_reconfig_req
 );

void
set_bitmask_ac_barring_for_mo_data 
(
    int                          loop,
    void *p_cell_reconfig_req
 );

void
set_bitmask_ssac_barring_for_mmtel_voice_r9 
(
    int                          loop,
    void *p_cell_reconfig_req
 );

void
set_bitmask_ssac_barring_for_mmtel_video_r9 
(
    int                          loop,
    void *p_cell_reconfig_req
 );

void
set_bitmask_ac_barring_for_csfb_r10 
(
    int                          loop,
    void *p_cell_reconfig_req
 );

/*SPR 13970 FIX End*/
oam_return_et
validate_default_paging_cycle
(
    U8     *p_default_paging_cycle,
    int     loop,
	UInt8  *cval
);

void
set_bitmask_default_paging_cycle
(
    int                           loop,
    void  *p_cell_reconfig_req
);

oam_return_et
validate_nB
(
    U8     *p_nB,
    int     loop,
	UInt8  *cval
);


void
set_bitmask_nB
(
    int                           loop,
    void  *p_cell_reconfig_req
);


oam_return_et
validate_root_sequence_index
(
    U8     *p_root_sequence_index,
    int     loop,
	UInt8  *cval
);

oam_return_et
validate_configuration_index
(
    U8     *p_configuration_index,
    int     loop,
	UInt8  *cval
);

oam_return_et
validate_high_speed_flag
(
    U8 *p_high_speed_flag,
     int loop,
	UInt8* cval
);

oam_return_et
validate_zero_cor_zone_config
(
    U8 *p_zero_cor_zone_config,
     int loop,
	UInt8* cval
);

oam_return_et
validate_frequency_offset
(
    U8 *p_frequency_offset,
     int loop,
	UInt8* cval
);
oam_return_et
validate_ref_signal_power
(
    U8 *p_reference_signal_power,
     int loop,
	UInt8* cval
);

oam_return_et
validate_p_b
(
    U8 *p_p_b,
     int loop,
	UInt8* cval
);

oam_return_et
validate_n_sb
(
    U8 *p_n_sb,
     int loop,
	UInt8* cval
);

oam_return_et
validate_pusch_hopping_mode
(
    U8 *p_pusch_hopping_mode,
     int loop,
	UInt8* cval
);
oam_return_et
validate_hopping_offset
(
    U8 *p_hopping_offset,
     int loop,
	UInt8* cval
);

oam_return_et
validate_group_hopping_enabled
(
    U8 *p_group_hopping_enabled,
     int loop,
	UInt8* cval
);
oam_return_et
validate_seq_hopping_enabled
(
    U8 *p_sequence_hopping_enabled,
     int loop,
	UInt8* cval
);

oam_return_et
validate_delta_pucch_shift
(
    U8 *p_delta_pucch_shift,
     int loop,
	UInt8* cval
);
oam_return_et
validate_n_rb_cqi
(
    U8 *p_n_rb_cqi,
     int loop,
	UInt8* cval
);
oam_return_et
validate_n1_pucch_an
(
    U8 *p_n1_pucch_an,
     int loop,
	UInt8* cval
);

oam_return_et
validate_srs_bandwidth_config
(
    U8 *p_srs_bandwidth_config,
     int loop,
	UInt8* cval
);

oam_return_et
validate_srs_subframe_config
(
    U8 *p_srs_subframe_config,
     int loop,
	UInt8* cval
);

oam_return_et
validate_ack_nack_srs_sim_trans
(
    U8 *p_ack_nack_srs_sim_trans,
     int loop,
	UInt8* cval
);

oam_return_et
validate_srs_max_up_pts
(
    U8 *p_srs_max_up_pts,
     int loop,
	UInt8* cval
);
oam_return_et
validate_p_0_nominal_pucch
(
    U8 *p_p_0_nominal_pucch,
     int loop,
	UInt8* cval
);
oam_return_et
validate_ul_cyclic_pre_length
(
    U8 *p_ul_cyclic_prefix_length,
     int loop,
	UInt8* cval
);
void
set_bitmask_ul_cy_pre_len
(
    int loop,
    void * p_cell_reconfig_req
);
oam_return_et
validate_t300
(
    U8 *p_t300,
     int loop,
	UInt8* cval
);

oam_return_et
validate_t301
(
    U8 *p_t301,
     int loop,
	UInt8* cval
);

oam_return_et
validate_t310
(
    U8 *p_t310,
     int loop,
	UInt8* cval
);
oam_return_et
validate_n310
(
    U8 *p_n310,
     int loop,
	UInt8* cval
);
oam_return_et
validate_t311
(
    U8 *p_t311,
     int loop,
	UInt8* cval
);
oam_return_et
validate_n311
(
    U8 *p_n311,
     int loop,
	UInt8* cval
);
oam_return_et
validate_ul_bandwidth
(
    U8 *p_ul_bandwidth,
     int loop,
	UInt8* cval
);
oam_return_et
validate_add_spec_emission
(
    U8 *p_add_spec_emission,
     int loop,
	UInt8* cval
);

void
set_bitmask_srs_max_up_pts
(
    int loop,
    void * p_cell_reconfig_req
);

oam_return_et
validate_time_al_timer_common
(
    U8 *p_time_al_timer_common,
     int loop,
	UInt8* cval
);
oam_return_et
validate_p_0_nominal_pusch
(
    U8 *p_p_0_nominal_pusch,
     int loop,
	UInt8* cval
);
oam_return_et
validate_alpha
(
    U8 *p_alpha,
     int loop,
	UInt8* cval
);
 
oam_return_et
validate_p_a
(
    U8 *p_p_a,
     int loop,
	UInt8* cval
);
oam_return_et
validate_srsEnabled
(
    U8 *p_srsEnabled,
     int loop,
	UInt8* cval
);

oam_return_et
validate_cqi_pucch_res_index
(
    U8 *p_cqi_pucch_resource_index,
     int loop,
    U8 * value 	
);
oam_return_et
validate_drx_enabled
(
    U8 *p_drx_enabled,
     int loop,
    U8* get_value
);


oam_return_et
validate_num_valid_srb_info
(
    U8 *p_num_valid_srb_info,
     int loop,
    U8* get_value
);

oam_return_et
validate_q_hyst
(
    U8 *p_q_hyst,
     int loop,
    U8* get_value
);
oam_return_et
validate_q_hyst_sf_medium
(
    U8 *p_q_hyst_sf_medium,
     int loop,
    U8* get_value
);
void
set_bitmask_common_params
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_load_config_neighbor_cell
(
    int loop,
    void * p_cell_reconfig_req
);


void
set_bitmask_load_config_serving_cell
(
    int loop,
    void * p_cell_reconfig_req
);


oam_return_et
validate_q_hyst_sf_high
(
    U8 *p_q_hyst_sf_high,
     int loop,
    U8* get_value
);
/* SPR 11001 FIX START */
/* LINES DELETED */
/* SPR 11001 FIX END */

oam_return_et
validate_s1siglink
(
     U8 *p_val,
     int loop,
     U8* get_value
);
oam_return_et
validate_s1connmode
(
     U8 *p_val,
     int loop,
     U8* get_value
);

oam_return_et
validate_start_stop_cell_identity
(
     U8 *p_cell_identity,
     int loop,
     U8* get_value
);
void
set_bitmask_irat_cell_res_pri
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_drx_short_cy_timer
(
    int loop,
    void * p_cell_reconfig_req
);

/* SPR_19279_START */
oam_return_et
validate_plmnlist_enable
(
 UInt8 *parmeter_value,
 int loop,
 UInt8* index
 );
oam_return_et
validate_plmnlist_isprimary
(
 UInt8 *parmeter_value,
 int loop,
 UInt8* index
 );
oam_return_et
validate_plmnlist_plmnid
(
 /*SPR 19977 Fix Start */
 rrm_oam_cell_plmn_info_t *parmeter_value,
 /*SPR 19977 Fix End */
 int loop,
 UInt8* index
 );
void
oam_populate_three_byte_plmn_from_mcc_mnc
(
 UInt8 *dest_plmn,
 rrm_oam_cell_plmn_info_t src_plmn
);
void
oam_populate_PLMNList(void);
void 
oam_find_primary_plmnid(Char8 **temp,
        XML_struct_cell_config *p_cell_spec_params);

oam_return_et 
oam_validate_plmnList(UInt8 num_plmn_entries,
        XML_struct_cell_config *p_cell_spec_params);
/* SPR_19279_END */

oam_return_et
oam_validate_one_element(UInt16 loop,
                         UInt8 *p_value,
                         xmlNode **p_p_orig_parent,
                         xmlNode **p_p_req_parent,
                         xmlNode ***p_p_p_xml_struct_addr,
                         /* SPR 18930  FIX START */
                         UInt8 index,
                         UInt8 cid);
                         /* SPR 18930  FIX END */        
void
set_bitmask_ncl_params
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_cell_ind_offset
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_inter_cell_res_pri
(
    int loop,
    void * p_cell_reconfig_req
);


void
set_bitmask_srb_params
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_phy_layer_params
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_PMax
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_SIntraSearch
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_SNonIntraSearch
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_ShortDRXCycle
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_speed_state_param
(
   int loop,
   void * p_cell_reconfig_req
);
void
set_bitmask_mac_layer_params_rach
(
   int loop,
   void * p_cell_reconfig_req
);
void
set_bitmask_rlc_layer_param_srb_2
(
   int loop,
   void * p_cell_reconfig_req
);

void
set_bitmask_rlc_layer_param_srb
(
   int loop,
   void * p_cell_reconfig_req
);
void
set_bitmask_srb_params_2
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_idle_mode_mobility_params
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_common_param_speed_state_params
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_intra_f_spd_st_factor
(
   int loop,
   void * p_cell_reconfig_req
);


void set_bitmask_earfcnul
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_phy_layer_param_prs
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_phy_layer_param_prs
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_eaid
(
    int loop,
    void * p_cell_reconfig_req
);
/*RRM 1.2 code prototypes*/
void
set_bitmask_connected_mode_mob
(
    int loop,
    void * p_cell_reconfig_req
);



void
set_bitmask_connected_mode_irat
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_irat_b1_th_utra_rscp
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_b1_th_utra_ecn0
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_b1_th_geran
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_b2_th_2u_rscp
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_b2_th_2u_ecn0
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_b2_th_2geran
(
    int loop,
    void * p_cell_reconfig_req
);
/*RRM 1.2 code prototypes*/

/*0.9 chnage done for code review comment implementation*/
oam_return_et
oam_rrc_handle_trace_callstart_error_ind_resp
(
     void* p_api_buf,
     UInt16 transaction_id,
     UInt16 msg_len
);

oam_return_et
oam_layer_send_req(void *p_api_buf);

#ifdef OAM_SON_ENABLE
void 
set_bitmask_son_anr_modify_pci_confusion_cfg_suspect_pci_threshold
(
    int loop,
    void * p_cell_reconfig_req
);
void 
set_bitmask_son_anr_modify_pci_confusion_cfg_clear_suspect_pci_threshold
(
    int loop,
    void * p_cell_reconfig_req
);
/* SPR 20653 Fix Start */
void 
set_bitmask_son_anr_modify_attribute_ue_throughput_dl_anr_threshold
(
    int loop,
    void * p_cell_reconfig_req
);
void 
set_bitmask_son_anr_modify_attribute_ue_throughput_ul_anr_threshold
(
    int loop,
    void * p_cell_reconfig_req
);
/* SPR 20653 Fix End */
void
set_bitmask_son_anr_modify_removal_attributes_handover_failure_threshold
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_anr_modify_removal_attributes_timeout_no_nr_neighbors
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_anr_modify_removal_attributes_no_activity_interval
(
    int loop,
    void * p_cell_reconfig_req
);

oam_return_et
oam_send_rach_opt_disable_req
(
        UInt8 serving_list_size,
        UInt16 trans_id
);

oam_return_et
oam_send_rach_opt_enable_req
(
        UInt8 serving_list_size,
        UInt16 trans_id
);
oam_return_et
oam_send_rach_opt_modify_req
(
        UInt8 rach_opt_mode,
        UInt16 trans_id
);
/* SPR 15604 START */
oam_bool_et
oam_check_if_update_req_at_son_nrt(SInt16 neigh_idx,
                         /* SPR 18930  FIX START */
                                   SInt8 cell_arr_idx,
                                   SInt8 nliu_cell_idx
                                   );
                         /* SPR 18930  FIX END */        


typedef enum {
		PLMNID = OAM_ZERO,
		PhyCellID,
		CID,
		QOffset,
		CIO,
		EUTRACarrierARFCN,
		X_VENDOR_TAC,
		X_VENDOR_CSGID,
		Blacklisted,
		X_VENDOR_RSRP,
		X_VENDOR_RSRQ,
		X_VENDOR_NR_STATUS,
		X_VENDOR_BROADCAST_STATUS,
		X_VENDOR_PRACH_ROOT_SEQUENCE_INDEX,
		X_VENDOR_PRACH_ZERO_CORRELATION_ZONE_CONFIG,
		X_VENDOR_PRACH_HIGH_SPEED_FLAG,
		X_VENDOR_PRACH_FREQUENCY_OFFSET,
		X_VENDOR_PRACH_CONFIGURATION_INDEX,
		X_VENDOR_ACCESS_MODE,
		PCPICHScramblingCode,
		RNCID,
		LAC,
		RAC,
		X_VENDOR_RSCP,
		X_VENDOR_CELL_SPECIFIC_OFFSET,
		X_VENDOR_NUMPLMNID,
		X_VENDOR_PLMN_ID,
		X_VENDOR_ECNO,
		X_VENDOR_PS_HO_SUPPORTED,
		X_VENDOR_VOIP_CAPABLE,
		UARFCNDL,
		X_VENDOR_HO_STATUS,
        UNDEFINED = 255
}nliu_param_et;

/*SPR 21889 Start*/
typedef struct _oam_agnt_fsm_data
{
    UInt8   instance_id;
    void    *p_api_data;
}oam_agnt_fsm_data_t;

typedef enum {
    OAM_AGENT_STATE_INIT,
    OAM_AGENT_STATE_WAIT_ACTIVE, /* Process spawning request is sent */
    OAM_AGENT_STATE_ACTIVE,     /* Process are running on the agent */
    OAM_AGENT_STATE_WAIT_SHUTDOWN, /* All process including agent needs to be shutdown */ 
    OAM_AGENT_MAX_STATE
}oam_agent_fsm_main_state_et;

typedef enum {
    OAM_AGENT_PROCDRE_STATE_INIT,
    OAM_AGENT_SPAWN_RESP_PENDING,
    OAM_AGENT_KILL_RESP_PENDING,
    OAM_AGENT_GET_LOG_RESP_PENDING,
    OAM_AGENT_MAX_PROCDR_STATE
}oam_agent_fsm_procdre_state_et;

typedef enum {
    OAM_AGENT_EVENT_IND,
    OAM_AGENT_EVENT_SPAWN_RESP,
    OAM_AGENT_EVENT_KILL_REQ,
    OAM_AGENT_EVENT_KILL_RESP,
    OAM_AGENT_EVENT_GET_LOGS_REQ,
    OAM_AGENT_EVENT_GET_LOGS_RESP,
    OAM_AGENT_EVENT_PROCEDURE_TMR_EXPIRY,
    OAM_AGENT_EVENT_RESTART,
    OAM_AGENT_EVENT_IM_OK_IND,
    OAM_AGENT_EVENT_MAX
}oam_agent_event_et;
/* SPR 21889 end */

nliu_param_et oam_validate_nliu_parameter(
        UInt16 instance_id_value,       
        char *tag_name,                                 
        UInt8 *param_name_buff,                         
        UInt8 *value,                                   
        void  *p_req,
        /* SPR 18930  FIX START */
        son_rat_type_et rat_type,
        UInt8 cell_config_idx);
       /* SPR 18930  FIX END */        

UInt16 oam_handle_nliu_object_updation(
		void *p_to ,
		void *p_from, 
		SInt16 instance_id, 
        /* SPR 18930  FIX START */
		son_rat_type_et rat_type,
        UInt8 cell_config_idx);
       /* SPR 18930  FIX END */        

oam_return_et oam_handle_nliu_object_deletion
( 
    char *obj_name,
    /* SPR 18930  FIX START */
    UInt32 instance_id,
    UInt8 cell_config_idx
    /* SPR 18930  FIX END */        
);

oam_return_et 
oam_handle_nliu_object_addition( 
		char *obj_name ,
		void *p_req,
    /* SPR 18930  FIX START */
		son_rat_type_et rat_type,
        UInt8 cell_config_idx
    /* SPR 18930  FIX END */        
		);

/* SPR 17509/17456 changes start */
 oam_return_et 
oam_update_set_parameter(
        char *parmeter_name,
        char *parmeter_value,
        rrm_oam_cell_reconfig_req_t *p_local_req,
        xmlNode **p_p_orig_parent,
        xmlNode **p_p_req_parent,
        xmlNode ***p_p_p_xml_struct_addr,
        /* SPR 17777 fix code removed */
        /* oam_rrm_cell_reconfig_req_t *p_oam_cell_reconfig_req, */
    /* SPR 18930  FIX START */
        oam_error_code_et *p_error_code,
        UInt8 cell_config_idx);
    /* SPR 18930  FIX END */        

/* SPR 17509/17456 changes end */

void
oam_reset_nliu(UInt8);

oam_return_et 
oam_search_integer_to_interger_for_get
(
 char * parmeter_value,
 int   * par_table_address,
 oam_counter_t count
);
/* SPR 15604 END */





/* Spr 15438 Fix Start*/
void
set_bitmask_for_lte_cell_must_include
(
 int loop, 
 void *p_son_nr_add_req
 );

void
set_bitmask_for_lte_cell_enable
(
 int loop,
 void *p_son_nr_add_req
 );
    /* SPR 18930  FIX START */
    /* Code Removed */
    /* SPR 18930  FIX END */        
oam_return_et
oam_validate_send_son_nr_delete_req
(
 UInt8     *p_api_buf,
 oam_son_nr_delete_req_t *p_req,
 UInt16                  src_module_id,
 UInt16                  transaction_id,
 UInt8                   cid,
 oam_error_code_et       *p_error_code
 );

/* Spr 15438 Fix End*/

/* SPR 15238 START */
oam_return_et
oam_dynamic_nbr_cell_access_mode_pci_validation(oam_eutran_nbr_cell_type_et nbr_cell_type,
                                                UInt32 eutra_carrier_arfcn,
/* Spr 15438 Fix Start*/
    /* SPR 18930  FIX START */
                                                son_intra_rat_neighbor_info_t *p_req,
                                                UInt8 cell_arr_idx);
    /* SPR 18930  FIX END */        
/* Spr 15438 Fix End*/
/* SPR 15238 END */



/* Spr 15612  Fix Start*/
void
set_bitmask_for_umts_cell_must_include
(
 int loop, 
 void *p_son_nr_add_req
 );

void
set_bitmask_for_umts_cell_enable
(
 int loop,
 void *p_son_nr_add_req
 );
/* Spr 15612  Fix End*/ 

#endif

oam_return_et
oam_layer_intf_process_msg
(
    void *p_api_buf
);

oam_return_et
oam_qcli_intf_msg_handler
(void *p_api_buf, UInt16 api_id, UInt16 src_module_id, UInt16 msg_len);

/* + SPR 17439 */
oam_return_et
oam_qcli_send_inactive_message(void);
/* - SPR 17439 */

void change_address_format(U8* addr_str,U8* addr);

#ifdef OAM_SON_ENABLE
oam_return_et
oam_son_intf_msg_handler(void *p_api_buf, UInt16 api_id, UInt16 src_module_id, UInt16 msg_len);
#endif

oam_return_et oam_s1ap_intf_msg_handler
(void *p_api_buf, UInt16 api_id);
oam_return_et oam_rrc_intf_msg_handler
(void *p_api_buf, UInt16 transid,UInt16 api_id);
oam_return_et oam_egtpu_intf_msg_handler
(void *p_api_buf, UInt16 api_id);
oam_return_et oam_pdcp_intf_msg_handler
(void *p_api_buf, UInt16 api_id);
oam_return_et oam_rlc_intf_msg_handler
(void *p_api_buf,UInt16 msg_len, UInt16 api_id);
oam_return_et oam_mac_intf_msg_handler
    /*SPR 17777 fix*/
(void *p_api_buf, UInt16 api_id, UInt16 msg_len);
oam_return_et oam_rrm_intf_msg_handler
(void *p_api_buf, UInt16 api_id, UInt16 msg_len);

UInt16
oam_find_global_table_size
(
    parameter_map_element_t *g_parameter_map_element
);
oam_return_et
oam_set_parameter_values_api_handler
(
    void *p_api_buf,
    UInt16 msg_len,
    UInt16 src_module_id,
    oam_error_code_et *p_error_code
);

void set_bitmask_ran_info
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_group_assignment_prms
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_cyclic_shift_prms
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_delta_pucch_format_1_prms
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_delta_pucch_format_1b_prms
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_delta_pucch_format_2_prms
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_delta_pucch_format_2a_prms
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_delta_pucch_format_2b_prms
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_delta_preamble_msg_3_prms
(
    int loop,
    void * p_cell_reconfig_req
);
/* SPR 13020 START */
void
set_bitmask_b2_threshold_1rsrp
(
  int loop,
   void * p_cell_reconfig_req
    );
void
set_bitmask_b2_threshold_1rsrq
(
  int loop,
   void * p_cell_reconfig_req
    );
/* SPR 13020 END */


void
set_bitmask_operator
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_epc_info
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_epc_info_general
(
 int loop,
 void * p_cell_reconfig_req
 );

void
set_bitmask_adl_sib_2
(
 int loop,
 void * p_cell_reconfig_req
 );
oam_return_et
oam_send_get_rlc_stats_req
(  
     UInt16            src_module_id,
     UInt16            transaction_id,
     oam_error_code_et *p_error_code
);
oam_return_et
oam_handle_cell_block_response(oam_rrm_cell_block_resp_t *response, 
                               oam_error_code_et   *p_error_code);

oam_return_et oam_handle_admin_state_unlocked(UInt8   *p_api_buf);

/* + SPR 17439 */
void oam_update_tr069_params(void);
/* - SPR 17439 */

void
oam_send_sdm_shutdown_ind(void *p_data);

void*
oam_construct_sdm_start_upgrade_req
(   
 UInt32  file_size,
 UInt8   file_type,
 UInt8   file_des_len,
 UInt16  file_name_len,
 UInt8*  file_name,
 UInt8*  file_des,
 UInt16*  msg_size
 );

oam_return_et
oam_check_file_integerity(UInt8 *file_name);

oam_return_et
oam_send_request_restart_ind
(
    UInt8 *p_api_buf
);

oam_return_et
oam_handle_sdm_upgrade_start_response
(
    void *p_msg
);

oam_return_et
oam_send_sdm_start_upgrade_req
(
    UInt8 *p_data
);

oam_return_et
oam_handle_sdm_polling_req
(
    char *p_msg
);

void
oam_update_enodeb_product_info
(
    void
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

#ifdef OAM_SON_ENABLE
oam_return_et       
oam_send_son_shutdown_req
(             
    oam_son_shutdown_req_t  *p_oam_son_shutdown_req,
    UInt16                     src_module_id,
    UInt16                     transaction_id, 
    oam_error_code_et       *p_error_code
);
void
set_bitmask_son_anr_meas_bandwidth
(
    int loop,
    void * p_cell_reconfig_req
);


void
set_bitmask_son_anr_presence_antenna_port
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_son_anr_neighbor_cell_config
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_son_anr_offset_frequency
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_son_anr_q_rx_lev_min
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_son_anr_p_max
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_anr_t_reselection
(
    int loop,
    void * p_cell_reconfig_req
);


void
set_bitmask_son_anr_t_reselection_sf
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_son_anr_threshX_high
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_son_anr_threshX_low
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_son_anr_cell_reselection_priority
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_son_anr_q_qualmin_r9
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_son_anr_threshx_highq_r9
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_son_anr_threshx_lowq_r9
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_fdd_offset_frequency
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_tdd_offset_frequency
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_fdd_cell_reselection_priority
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_tdd_cell_reselection_priority
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_fdd_threshX_high
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_tdd_threshX_high
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_fdd_threshX_low
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_tdd_threshX_low
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_fdd_q_rx_lev_min
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_tdd_q_rx_lev_min
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_fdd_p_max
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_tdd_p_max
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_add_req_inter_rat_pci_tdd
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_fdd_q_qual_min
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_fdd_threshx_highq_r9
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_fdd_threshx_lowq_r9
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_t_reselection
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_utra_t_reselection_sf
(
    int loop,
    void * p_cell_reconfig_req
);

void 
set_bitmask_intra_pci 
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_intra_cell_specific_offset 
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_intra_rat_tac 
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_intra_rat_plmn_id 
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_intra_rat_csg_id 
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_intra_rat_earfcn 
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_intra_ho_status 
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_intra_rat_rsrp 
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_intra_rat_rsrq 
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_intra_rat_nr_status 
(
    int loop, 
    void *p_cell_reconfig_req
); 

void 
set_bitmask_add_req_intra_rat_suspected_pci 
(
    int loop, 
    void *p_cell_reconfig_req
);

/*eICIC_PHASE_1_2_CHANGES_START*/

/*SPR 14554 start*/
void 
set_bitmask_cell_info_min_abs_usage_threshold
(
    int loop, 
    void *p_serving_cell
);

void 
set_bitmask_cell_info_max_abs_usage_threshold
(
    int loop, 
    void *p_serving_cell
);
/*SPR 14554 end*/
void 
set_bitmask_add_req_intra_rat_interference_scheme
(
    int loop, 
    void *p_cell_reconfig_req

);
 
void
set_bitmask_add_req_intra_rat_min_abs_usage_threshold
(
 int loop,
 void * p_son_nr_add_req
);

void
set_bitmask_add_req_intra_rat_max_abs_usage_threshold
(
 int loop,
 void * p_son_nr_add_req
 );


void
set_bitmask_neighbour_default_config_nr_status
(   int loop, 
    void *p_neighbor_default_config_req
);

void 
set_bitmask_neighbour_default_config_ho_status
(   int loop, 
    void *p_neighbor_default_config_req
);
void 
set_bitmask_neighbour_default_config_x2_status
(   int loop, 
    void *p_neighbor_default_config_req
);
void 
set_bitmask_neighbour_default_config_csg_cell_intf_scheme
(   int loop, 
    void *p_neighbor_default_config_req
);
void 
set_bitmask_neighbour_default_config_pico_cell_intf_scheme
(   int loop, 
    void *p_neighbor_default_config_req
);
void 
set_bitmask_neighbour_default_config_macro_cell_intf_scheme
(   int loop, 
    void *p_neighbor_default_config_req
);


/*eICIC_PHASE_1_2_CHANGES_END*/

void 
set_bitmask_add_req_inter_rat_pci
(
    int loop, 
    void *p_cell_reconfig_req
); 

void 
set_bitmask_add_req_inter_rat_cso
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_inter_rat_lac
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_inter_rat_rac
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_inter_rat_plmn_id
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_inter_rat_csg_id
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_inter_uarfcn
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_inter_rat_rscp
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_inter_rat_ecno
(
    int loop, 
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_inter_rat_nr_status
(
    int loop,
    void *p_cell_reconfig_req
);

void 
set_bitmask_add_req_inter_rat_ho_status
(
    int loop, 
    void *p_cell_reconfig_req
);
void 
set_bitmask_add_req_inter_rat_ps_ho_supported
(
    int loop, 
    void *p_cell_reconfig_req
);
void 
set_bitmask_add_req_inter_rat_voip_capable
(
    int loop, 
    void *p_cell_reconfig_req
);
void 
set_bitmask_nr_enb_tnl_address_ipv4
(
    int loop, 
    void * p_cell_reconfig_req
);

void 
set_bitmask_nr_enb_tac
(
    int loop, 
    void * p_cell_reconfig_req
);

void 
set_bitmask_nr_enb_x2_status
(
    int loop, 
    void * p_cell_reconfig_req
);

void 
set_bitmask_nr_enb_x2_connection_status
(
    int loop, 
    void * p_cell_reconfig_req
);

void
set_bitmask_son_anr_enable_ue_count
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_anr_enable_reporting_interval
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_anr_enable_limited_mode
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_anr_enable_no_act_int
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_anr_enable_ho_failure_threshold
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_anr_enable_timeout_no_nr_neighbors
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_anr_enable_suspect_pci_threshold
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_anr_enable_clear_suspect_pci_threshold
(
    int loop,
    void * p_cell_reconfig_req
);
/* SPR 20653 Fix Start */
void
set_bitmask_son_anr_enable_ue_throughput_dl_anr_threshold
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_anr_enable_ue_throughput_ul_anr_threshold
(
    int loop,
    void * p_cell_reconfig_req
);
/* SPR 20653 Fix End */
void
set_bitmask_son_nw_scan_meas_bandwidth_per_earfcn
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_nw_scan_plmn_list
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_nw_scan_meas_bandwidth
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_son_nr_scan_meas_bandwidth_per_earfcn
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_nr_scan_meas_bandwidth
(
    int loop,
    void * p_cell_reconfig_req
);


void
set_bitmask_son_periodic_nw_scan_meas_bandwidth_per_earfcn
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_periodic_nw_scan_meas_bandwidth
(
    int loop,
    void * p_cell_reconfig_req
);
void 
set_bitmask_son_nr_del_from_rem_list_intra
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_nr_del_from_rem_list_utran
(
    int loop,
    void * p_cell_reconfig_req
);
void
set_bitmask_son_nr_del_from_rem_list_geran
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_gbrbktdepthfactor
(
        int loop,
	void *p_cell_reconfig_req
);

void set_bitmask_ambrBktDepthFactor
(
        int loop,
	void *p_cell_reconfig_req
);

void set_bitmask_tokenPeriodicity
(
        int loop,
	void *p_cell_reconfig_req
);

void set_bitmask_qosStrategyOverallocFactor
(
        int loop,
	void *p_cell_reconfig_req
);

void set_bitmask_dlQosStrategyWeights
(
        int loop,
	void *p_cell_reconfig_req
);

void set_bitmask_ulQosStrategyWeights
(
        int loop,
	void *p_cell_reconfig_req
);

void set_bitmask_enableAllocAfterAllowedBitrate
(
    int loop,
	void *p_cell_reconfig_req
);

void
set_bitmask_autonomous_switch_off_load
(
 int loop,
 void *p_cell_reconfig_req
 );

void
set_bitmask_autonomous_switch_off_both
(
 int loop,
 void *p_cell_reconfig_req
 );

void
set_bitmask_autonomous_switch_off_active
(
 int loop,
 void *p_cell_reconfig_req
 );

/* SPR-13586 START */
void
set_bitmask_son_nr_geran_band_ind(int loop,
                                  void *p_cell_reconfig_req);

void
set_bitmask_son_nr_geran_arfcn(int loop,
                               void *p_cell_reconfig_req);

void
set_bitmask_son_nr_geran_cell_spec_offset(int loop,
                                          void *p_cell_reconfig_req);

void
set_bitmask_son_nr_geran_cell_spec_offset(int loop,
                                          void *p_cell_reconfig_req);

void
set_bitmask_son_nr_geran_rac(int loop,
                             void *p_cell_reconfig_req);

void
set_bitmask_son_nr_geran_rssi(int loop,
                              void *p_cell_reconfig_req);

void
set_bitmask_son_nr_geran_nr_status(int loop,
                                   void *p_cell_reconfig_req);

void
set_bitmask_son_nr_geran_ho_status(int loop,
                                   void *p_cell_reconfig_req);
void
set_bitmask_son_nr_geran_bsic(int loop,
                              void *p_cell_reconfig_req);
/* SPR-13586 END */

/* SPR 14651 START */
/* Code Removed */
/* SPR 14651 END */
#endif



oam_return_et
oam_construct_n_send_gpv_response
(
    UInt8             *p_parent_node_name,    
    UInt8             *p_child_node_name_value_pair[255],
    UInt8              no_of_parameter,
    UInt16             transaction_id,
    oam_error_code_et *p_error_code,
    UInt16             des_id , 
    UInt8              start_index ,
    UInt8              end_index
);


/* SCTP CODE */
void
oam_populate_correct_x2ap_sctp_response
(
oam_x2ap_get_sctp_status_resp_t *p_oam_x2ap_get_sctp_status_resp   
);
void
oam_populate_correct_s1ap_sctp_response
(
oam_s1ap_get_sctp_status_resp_t *p_oam_s1ap_get_sctp_status_resp    
);

/* LWA changes start*/
/* LWA changes end*/

/*OAM_REVIEW_CHANGES*/
void
set_bitmask_x2ap_prov
(
    int loop,
    void *p_cell_reconfig_req
);

void
set_bitmask_s1ap_sctp_conf_param
(
    int loop,
    void * p_cell_reconfig_req
); 
/*OAM_REVIEW_CHANGES*/

void
set_bitmask_cell_restriction_params
(
    int loop,
    void * p_cell_reconfig_req	
);

  void
set_bitmask_mro_ho_fail_rate_for_optimization
  (
      int loop,
    void * p_mro_modify_config_params_req
  );
  void
  set_bitmask_son_mro_modify_config_param_req_mro_mode
  (
      int loop,
    void * p_mro_modify_config_params_req
  );

  void
  set_bitmask_son_mro_modify_config_param_req_mro_param
  (
      int loop,
    void * p_mro_modify_config_params_req
  );

void set_bitmask_mro_ho_failure_accumulation_duration
(
    int loop,
    void * p_mro_modify_config_params_req
  );

  void
  set_bitmask_son_mro_modify_config_param_req_delta_cio
  (
      int loop,
    void * p_mro_modify_config_params_req
  );

  void
set_bitmask_mro_trigger_optimization_ho_attempts
  (
      int loop,
    void * p_mro_modify_config_params_req
  );
  void
set_bitmask_mro_trigger_analysis_ho_failure_rate
  (
      int loop,
    void * p_mro_modify_config_params_req
  );

  void
set_bitmask_mro_ho_fail_rate
  (
      int loop,
    void * p_mro_modify_config_params_req
  );

  void
set_bitmask_mro_default_ttt
  (
      int loop,
  void * p_mro_modify_config_params_req
  );

  void
  set_bitmask_son_mro_modify_config_param_req_delta_ttt
  (
      int loop,
    void * p_mro_modify_config_params_req
  );
/* SCTP CODE END */
oam_return_et
oam_populate_plmnid(Char8 *temp, void *plmnid);

oam_return_et
oam_generate_plmnid(Char8 *out_plmn, void *input_plmn);
/* SPR 15917 START */
void 
oam_find_primary_plmnid(Char8 **temp,
        XML_struct_cell_config *p_cell_spec_params);

oam_return_et 
oam_validate_plmnList(UInt8 num_plmn_entries,
        XML_struct_cell_config *p_cell_spec_params);
/* SPR 15917 END */

#ifdef OAM_SON_ENABLE
oam_return_et
oam_parse_son_nr_enb_retrieve_resp_oam
(               
    UInt8  *p_src,  
    oam_son_nr_enb_retrieve_resp_t *p_oam_son_nr_enb_retrieve_resp,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_parse_son_nr_retrieve_resp_oam
(
    UInt8  *p_src,
    oam_son_nr_retrieve_resp_t *p_oam_son_nr_retrieve_resp,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_process_add_delete_utran_fdd_freq_list(UInt8 add_del_flag,
                                           void *p_msg,
                                           UInt16 instance_id,
                                           void* p_add_data,
                                           oam_error_code_et *p_err);

oam_return_et
oam_process_add_delete_geran_freq_group_list(UInt8 add_del_flag,
                                             void *p_msg,
                                             UInt16 instance_id,
                                             void* p_add_data,
                                             oam_error_code_et *p_err);


oam_return_et
oam_process_add_delete_inter_freq_carrier_list(UInt8 add_del_flag,
                                               void *p_msg,
                                               UInt16 instance_id,
                                               void* p_add_data,
                                               oam_error_code_et *p_err);

oam_return_et
oam_process_add_delete_NeighborList_LTECell(UInt8 add_del_flag,
                                            void *p_msg,
                                            UInt16 instance_id,
                                            void* p_add_data,
                                            oam_error_code_et *p_err);

oam_return_et
oam_process_add_delete_NeighborList_InterRATCell_UMTS(UInt8 add_del_flag,
                                                   void *ptr,
                                                   UInt16 instance_id,
                                                   void* p_add_data,
                                                   oam_error_code_et *p_err);
/* SPR-13586 START */
oam_return_et
oam_process_add_delete_NeighborList_InterRATCell_GSM(UInt8 add_del_flag,
                                                   void *ptr,
                                                   UInt16 instance_id,
                                                   void* p_add_data,
                                                   oam_error_code_et *p_err);
/* SPR-13586 END */
#endif

oam_return_et
oam_populate_rrm_plmnid(
      Char8                      *input_plmn,
      rrm_oam_cell_plmn_info_t      *out_plmn);



/* SPR 15527 START */
oam_return_et oam_process_add_delete_qos_list(UInt8 add_del_flag,
                                               void *p_msg,
                                               UInt16 instance_id,
                                               void* p_add_data,
                                               oam_error_code_et *p_err);

/* SPR 15527 END */
oam_return_et oam_process_add_delete_plmn_list(UInt8 add_del_flag,
                                               void *p_msg,
                                               UInt16 instance_id,
                                               void* p_add_data,
                                               oam_error_code_et *p_err);


oam_return_et  oam_add_object_api_handler(void *p_msg,
                                          oam_error_code_et *p_err,
                                          UInt16 *p_inst_num);

/* SPR 15527 START */
oam_return_et 
oam_generate_instance_id_for_qos_list(UInt16 *p_instance_id,
                                                     UInt16  max_entries,
                                                     oam_error_code_et *p_err);

/* SPR 15527 END */
/* SPR 16603 Fix Start */
oam_return_et 
oam_add_object( 
        char *obj_name, 
        void *p_msg,
        UInt16 *p_inst_num,
        xmlNode *p_obj_parent, 
        xmlNode *p_obj_sibling, 
    /* SPR 18930  FIX START */
        xmlNode **p_p_obj_struct_parent,
        UInt16 **curr_num_entries 
    /* SPR 18930  FIX END */        
        /* SPR 17777 fix code removed */
        );

/* SPR 16603 Fix End */

xmlNode*
oam_add_parent_object(Char8 *full_param_name,
                      UInt16 inst_num);

oam_return_et oam_delete_object_api_handler(void *p_msg,
                                            oam_error_code_et *p_err);

oam_return_et
oam_delete_object(Char8 *obj_name, void *p_msg, oam_error_code_et *p_err);

oam_return_et
oam_generate_instance_id_for_utran_fdd_freq_list(UInt16 *p_instance_id,
                                                     UInt16  max_entries,
                                                     oam_error_code_et *p_err);

oam_return_et
oam_generate_instance_id_for_inter_freq_carrier_list(UInt16 *p_instance_id,
                                                     UInt16  max_entries,
                                                     oam_error_code_et *p_err);

oam_return_et oam_generate_instance_id_for_plmn_list(UInt16 *p_instance_id,
                                                     UInt16  max_entries,
                                                     oam_error_code_et *p_err);

oam_return_et
oam_generate_instance_id_for_NeighborList_LTECell(UInt16 *p_instance_id,
                                                  UInt16  max_entries,
                                                  oam_error_code_et *p_err);

oam_return_et
oam_generate_instance_id_for_NeighborList_InterRATCell_UMTS
(
    UInt16 *p_instance_id,
    UInt16 max_entries
);

/*SPR 15604 START*/
oam_return_et
oam_process_add_delete_NeighborListInUse_LTECell(UInt8 add_del_flag,
        void *p_msg,
        UInt16 instance_id,
        void* p_add_data,
        oam_error_code_et *p_err);

oam_return_et
oam_process_add_delete_NeighborListInUse_InterRATCell_UMTS(UInt8 add_del_flag,
        void *ptr,
        UInt16 instance_id,
        void* p_add_data,
        oam_error_code_et *p_err);


oam_return_et
    /* SPR 18930  FIX START */
oam_neighborlistInUse_interratcell_umts_data_accessor(UInt16 **p_curr_entries,
    /* SPR 18930  FIX END */        
                                                 UInt16 *p_max_entries,
                                                 xmlNode **obj_parent,
                                                 xmlNode **obj_first_sibling,
                                                 xmlNode *** obj_struct_parent,
                                                 UInt8 *cell_idx, 
                                                 oam_error_code_et *p_err);

oam_return_et oam_generate_instance_id_for_NeighborListInUse_LTECell(UInt16 *p_instance_id,
                                                                     UInt16 max_entries,
    /* SPR 18930  FIX START */
                                                                     oam_error_code_et *p_err,UInt8 cell_arr_idx);
    /* SPR 18930  FIX END */        

oam_return_et oam_generate_instance_id_for_NeighborListInUse_InterRATCell_UMTS(UInt16 *p_instance_id,
                                                                               UInt16 max_entries,
                                                                               oam_error_code_et *p_err, UInt8 cell_arr_idx);
/*SPR 15604 END*/



oam_return_et
oam_generate_instance_id_for_geran_freq_group_list(UInt16 *p_instance_id,
                                                   UInt16  max_entries,
                                                   oam_error_code_et *p_err);

    void oam_construct_n_send_addobject_resp_to_usr(UInt8 resp_result,
            UInt32 resp_error_code,
            UInt8 *err_string,
            UInt8 err_string_size,
            UInt8 oam_version, UInt16 src_id,
            UInt16 dst_id, UInt16 req_trans_id,
            UInt8 api_version,
            UInt8 api_hdr_resvd_byte,
                                          oam_error_code_et *p_err_code,
            UInt16 instance_number);

void oam_construct_n_send_deleteobject_resp_to_usr(UInt8 resp_result,
        UInt16 resp_error_code,
        UInt8 *err_string,
        UInt8 err_string_size,
        UInt8 oam_version, UInt16 src_id,
        UInt16 dst_id, UInt16 req_trans_id,
        UInt8 api_version,
        UInt8 api_hdr_resvd_byte,
                                          oam_error_code_et *p_err_code);
                                                                        
oam_return_et oam_tr069_intf_msg_handler
(
 void *p_api_buf, 
 UInt16 api_id, 
 UInt16 src_module_id, 
 UInt16 msg_len
 );



oam_return_et
oam_handle_tr069_command_resp
(
    UInt8 *p_msg
    );

oam_return_et oam_tr69_controll_cmd_req
( 
    char** p_params_list,
    UInt8 num_params ,
    UInt16  sub_module_id,
    /* SPR 17777 fix code removed */
    UInt16 src_id,
    UInt16 dst_id,
    UInt8 oam_version,
    UInt16 req_trans_id,
    UInt8 api_version,
    UInt8 api_hdr_resvd_byte
);
oam_return_et oam_stop_timer_and_delete_tcb_entry(UInt8* p_api_buf);
/* SPR_9620_CHANGES_START */
oam_return_et
oam_handle_rrm_cell_block_for_shutdown(UInt16 transaction_id , void *p_api_buf);
/* SPR_9620_CHANGES_END */
oam_return_et
oam_copy_config_to_reconfig(
oam_rrm_cell_config_req_t *p_cell_config_req, 
 oam_rrm_cell_reconfig_req_t *p_cell_reconfig_req);
oam_return_et
oam_send_sdm_ack_upgrade_start(void);
#ifdef OAM_SON_ENABLE
/* + SPR 17439 */
    oam_return_et oam_save_nw_scan_results_in_dom(void);
/* - SPR 17439 */

oam_return_et
oam_startup_init_rrm_son
(
    oam_sim_error_info_et *p_error_code
);

void oam_son_nr_add_access_mode_handling(son_neighbor_info_t* p_nbr_info);

extern UInt8 g_oam_nmm_scan_first_time;

#endif 
extern 
oam_return_et
oam_fill_ecn_cells
(
  rrm_oam_cell_ecn_capacity_enhance_req_t *p_oam_rrm_cell_ecn_capacity_enhance_req,
  oam_ul_dl_qci_cli_input_t oam_ul_dl_qci_cli_input,
  SInt8 ue_num_count);

oam_return_et
oam_rrm_oam_cell_ecn_capacity_enhance_resp
(
   UInt8 *p_api_buf,
   UInt16  trans_id,
   UInt16  dest_id
);

#ifdef OAM_SON_ENABLE
UInt16 oam_son_fill_earfcn_array
(
/* Start: CSR 00053250 */
    son_meas_bandwidth_pci_per_earfcn_t *p_earfcn_array,
/*End: CSR 00053250 end */
    UInt16 start_value,
    UInt16 num_earfcn
);
#endif

oam_return_et
oam_radio_transmission_carrier_enable_disable
(
                oam_bool_et  carrier_state
);

/* + SPR 17439 */
void oam_ecn_init(void);
/* - SPR 17439 */

void
set_bitmask_phy_layer_param_prs_muting_configuration_two
(
 int loop,
 void * p_cell_reconfig_req
 );
void
set_bitmask_phy_layer_param_prs_muting_configuration_four
(
 int loop,
 void * p_cell_reconfig_req
 );
void
set_bitmask_phy_layer_param_prs_muting_configuration_eight
(
 int loop,
 void * p_cell_reconfig_req
 );
void
set_bitmask_phy_layer_param_prs_muting_configuration_sixteen
(
 int loop,
 void * p_cell_reconfig_req
 );

void
set_bitmask_eutran_access_point_pos
(
 int loop,
 void * p_cell_reconfig_req
 );
/* SPR 9620 CHANGE START */
oam_return_et
        oam_send_rrc_add_lgw_req
(
 oam_rrc_add_lgw_req_t *p_oam_rrc_add_lgw_req,
 UInt16                src_module_id,
 UInt16                trans_id,
 void                  *p_api_buf,
 UInt16                msg_len

 );

oam_return_et
oam_send_lgw_add_req
(
        oam_rrc_add_lgw_req_t *p_oam_rrc_add_lgw_req,
        char *value,
        UInt16 trans_id,
        void *p_api_buf,
        UInt16 msg_len

);
oam_return_et   
        oam_send_rrc_delete_lgw_req
(                                                   
 oam_rrc_delete_lgw_req_t *p_oam_rrc_delete_lgw_req,
 UInt16                src_module_id,               
 UInt16                trans_id,
 void                  *p_api_buf,
 UInt16                 msg_len

 );
/* SPR 9620 CHANGE END */
/* + SPR 17439 */
oam_return_et oam_handle_internal_shutdown(void);
/* - SPR 17439 */


/* SPR 15527 START */
oam_return_et
oam_partial_path_plmnlist_fill_params(UInt8 *num_params , UInt8 *max_multi_obj_cnt );
/* SPR 15527 END */
/* SPR 15527 START */
oam_return_et
oam_partial_path_qos_fill_params(UInt8 *num_params , UInt8 *max_multi_obj_cnt );
/* SPR 15527 END */

oam_return_et
oam_partial_path_neighborlist_inuse(char *temp_payload ,UInt8 *p_temp_buf, UInt16 curr_pos , UInt16 *resp_len , UInt16  *num_parameters );


oam_return_et
oam_partial_path_rem_lte(char *temp_payload ,UInt8 *p_temp_buf, UInt16 curr_pos , UInt16 *resp_len , UInt16  *num_parameters );

/* SPR 13501 Fix Start */
void
set_bitmask_dl_resorce_partition_info
(
    int                          loop,
    void *p_cell_reconfig_req
);

void
set_bitmask_operator_info
(
    int                          loop,
    void *p_cell_reconfig_req
);

void
set_bitmask_dynamic_icic_info_params
(
    int                          loop,
    void *p_cell_reconfig_req
);


void
set_bitmask_cell_edge_region_dl
(
    int                          loop,
    void *p_cell_reconfig_req
);

void
set_bitmask_cell_edge_region_ul
(
    int                          loop,
    void *p_cell_reconfig_req
);

void
set_bitmask_ul_resorce_partition_info
(
    int                          loop,
    void *p_cell_reconfig_req
);

oam_return_et
oam_validate_dyn_icic_num_ce_region(UInt8 *num_ce_regioin,
                                       SInt32 loop,
                                       UInt8* get_value);

oam_return_et
oam_validate_dyn_icic_num_cc_region(UInt8 *num_cc_regioin,
                                       SInt32 loop,
                                       UInt8* get_value);

oam_return_et
oam_validate_dyn_icic_start_rb(UInt8 *start_rb,
                                     SInt32 loop,
                                     UInt8* get_value);

oam_return_et
oam_validate_dyn_icic_end_rb(UInt8 *start_rb,
                                   SInt32 loop,
                                   UInt8* get_value);
/* SPR 13501 Fix End */

void oam_init_msg_ipcs(void);
void oam_update_device_info(char* string, char* value);

/* CSR 00051709 Change start*/
oam_return_et
oam_handle_close_mme_req(oam_s1SigLinkServerList_t * oam_s1ap_close_mme_req);

oam_return_et
oam_handle_reestablish_mme_conn_req(oam_s1SigLinkServerList_t * oam_s1ap_reestablish_mme_req);
/* SPR 9620 CHANGE START */
oam_return_et
oam_handle_add_mme_req(oam_s1SigLinkServerList_t * oam_add_mme_req, UInt16 trans_id,UInt16 msg_len,UInt8 *p_api_buf);
/* SPR 9620 CHANGE END */
oam_return_et
oam_mme_ip_analysis(
                    oam_s1SigLinkServerList_t * oam_add_mme_req,
                    oam_s1SigLinkServerList_t * oam_close_mme_req,
                    oam_s1SigLinkServerList_t * oam_reestablish_mme_req); 
/* CSR 00051709 Change End*/

/* SPR-10651 FIX Start */
oam_return_et
validate_earfcndl_with_freq_band_ind_value(UInt32 earfcn);
oam_return_et
validate_earfcnul_with_freq_band_ind_value(UInt32 earfcn);
/* SPR-10651 FIX End */

/* + SPR 17439 */
oam_return_et oam_populate_rrm_enb_config_params(void);
/* - SPR 17439 */
oam_return_et
oam_encode_plmnid(Char8* input, UInt8 *out_plmnid);
/* SPR-9485 START */
oam_return_et
oam_decode_plmnid(UInt8 *in_plmnid, Char8 *output);
oam_return_et
oam_plmnid_from_mcc_mnc(Char8 *out_plmn, void *input_plmn);
/* SPR-9485 END */

oam_return_et
oam_sim_main(SInt32 argc, Char8*arg[]);
/* SPR 11519 CHANGE START */
void
set_bitmask_mlb_modify_mlb_mode
(
    int loop,
    void * p_mlb_modify_config_params_req
 );
/* SPR 11519 CHANGE END */
void
set_bitmask_mlb_eutra_q_offset_config_q_offset_min
(
    int loop,
    void * p_mlb_modify_attr_req
);

void
set_bitmask_mlb_eutra_q_offset_config_q_offset_max
(
    int loop,
    void * p_mlb_modify_attr_req
);
/*SPR 12001 FIX START*/
void
set_bitmask_son_nw_scan_pci_list
(
 int loop,
 void * p_cell_reconfig_req
);

/*SPR 12001 FIX END*/
/* SPR 9678 Fix Start*/
#ifdef OAM_SON_ENABLE
oam_return_et oam_update_son_cell_info_ind_af_reconfig(UInt16 idx, UInt8 cell_config_idx);
#endif
/* SPR 9678 Fix End*/
/* + SPR 17439 */
void oam_killall(void);
/* - SPR 17439 */
/*SPR 17008 Fixed Start*/
oam_return_et
validate_download_file_name(UInt8 *file_name);
/*SPR 17008 Fixed End*/
Char8 *
oam_get_cell_state_string(UInt32 cell_state);/* Cov Fix 63711/12/13 */

/* SPR 11001 FIX START */
void
oam_commit_local_load_config_data(oam_rrm_load_config_req_t *p_local_req);
/* SPR 11001 FIX END */

oam_return_et
    /* SPR 18930  FIX START */
oam_cell_plmnlist_data_accessor(UInt16 **curr_entries,
    /* SPR 18930  FIX END */        
                                UInt16 *max_entries,
                                xmlNode **obj_parent,
                                xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                xmlNode *** obj_struct_parent,
                                UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                oam_error_code_et *p_err);

/* SPR 15527 START */
oam_return_et
oam_qos_data_accessor(UInt16 **curr_entries,
                      UInt16 *max_entries,
                      xmlNode **obj_parent,
                      xmlNode **obj_first_sibling,
                      xmlNode *** obj_struct_parent,
                      UInt8 *cell_idx, 
                      oam_error_code_et *p_err);
/* SPR 15527 END */

oam_return_et
    /* SPR 18930  FIX START */
oam_utran_fdd_freq_data_accessor(UInt16 **curr_entries,
    /* SPR 18930  FIX END */        
                                 UInt16 *max_entries,
                                 xmlNode **obj_parent,
                                 xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                 xmlNode *** obj_struct_parent,
                                 UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                 oam_error_code_et *p_err);

oam_return_et
    /* SPR 18930  FIX START */
oam_geran_freq_group_data_accessor(UInt16 **curr_entries,
    /* SPR 18930  FIX END */        
                                 UInt16 *max_entries,
                                 xmlNode **obj_parent,
                                 xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                 xmlNode *** obj_struct_parent,
                                 UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                 oam_error_code_et *p_err);

oam_return_et
    /* SPR 18930  FIX START */
oam_interfreq_carrier_data_accessor(UInt16 **curr_entries,
    /* SPR 18930  FIX END */        
                                    UInt16 *max_entries,
                                    xmlNode **obj_parent,
                                    xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                    xmlNode *** obj_struct_parent,
                                    UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                    oam_error_code_et *p_err);

/* SPR 15604 Fix start */
oam_return_et
    /* SPR 18930  FIX START */
oam_neighborlistInUse_ltecell_data_accessor(UInt16 **curr_entries,
    /* SPR 18930  FIX END */        
                                       UInt16 *max_entries,
                                       xmlNode **obj_parent,
                                       xmlNode **obj_first_sibling,
                                       xmlNode *** obj_struct_parent,
                                       UInt8 *cell_idx, 
                                       oam_error_code_et *p_err);



/*SPR 15604 Fix End */



oam_return_et
    /* SPR 18930  FIX START */
oam_neighborlist_ltecell_data_accessor(UInt16 **curr_entries,
    /* SPR 18930  FIX END */        
                                       UInt16 *max_entries,
                                       xmlNode **obj_parent,
                                       xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                       xmlNode *** obj_struct_parent,
                                       UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                       oam_error_code_et *p_err);

oam_return_et
    /* SPR 18930  FIX START */
oam_neighborlist_interratcell_umts_data_accessor(UInt16 **curr_entries,
    /* SPR 18930  FIX END */        
                                                 UInt16 *max_entries,
                                                 xmlNode **obj_parent,
                                                 xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                                 xmlNode *** obj_struct_parent,
                                                 UInt8 *cell_idx,
/* SPR 16603 Fix End */
                                                 oam_error_code_et *p_err);

oam_return_et
oam_generate_instance_id_for_NeighborList_UMTS(UInt16 *p_instance_id,
                                               UInt16  max_entries,
                                               oam_error_code_et *p_err);
/* SPR-13586 START */
oam_return_et
    /* SPR 18930  FIX START */
oam_neighborlist_interratcell_gsm_data_accessor(UInt16 **curr_entries,
    /* SPR 18930  FIX END */        
                                                 UInt16 *max_entries,
                                                 xmlNode **obj_parent,
                                                 xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                                 xmlNode *** obj_struct_parent,
                                                 UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                                 oam_error_code_et *p_err);

oam_return_et
oam_generate_instance_id_for_NeighborList_GSM(UInt16 *p_instance_id,
                                               UInt16  max_entries,
                                               oam_error_code_et *p_err);

/* SPR-13586 END */
void oam_update_first_object_list_node(UInt8 loop, xmlNode *obj);
/*SPR 11488 FIX START*/
oam_return_et oam_rrc_send_cleanup_req
(
    UInt16     src_module_id,
    UInt16     transaction_id,
    oam_error_code_et *p_error_code
);
oam_return_et oam_s1ap_send_cleanup_req
(
    UInt16     src_module_id,
    UInt16     transaction_id,
    oam_error_code_et *p_error_code
);
oam_return_et oam_x2ap_send_cleanup_req
(
    UInt16     src_module_id,
    UInt16     transaction_id,
    oam_error_code_et *p_error_code
); 
/*SPR 11488 FIX END*/
/* Start: SPR 10567 */
oam_return_et
oam_handle_son_nr_retrieve_eutran
(
 UInt16             dst_id,
 UInt16             trans_id,
 oam_error_code_et  *p_err
 );

oam_return_et
oam_handle_son_nr_retrieve_utran
(
 UInt16             dst_id,
 UInt16             trans_id,
 oam_error_code_et  *p_err
 );
/* SPR-13586 START */
oam_return_et
oam_handle_son_nr_retrieve_geran
(
 UInt16             dst_id,
 UInt16             trans_id,
 oam_error_code_et  *p_err
 );
/* SPR-13586 END */

oam_return_et
oam_handle_son_nliu_retrieve_eutran
(
 UInt16             dst_id,
 UInt16             trans_id,
 oam_error_code_et  *p_err
 );

oam_return_et
oam_handle_son_nliu_retrieve_utran
(
 UInt16             dst_id,
 UInt16             trans_id,
 oam_error_code_et  *p_err
 );
/* SPR-13586 START */
oam_return_et
oam_handle_son_nliu_retrieve_geran
(
 UInt16             dst_id,
 UInt16             trans_id,
 oam_error_code_et  *p_err
 );
/* SPR-13586 END */

void
oam_update_usr_added_neigh_to_nliu(UInt16 nliu_arr_idx, UInt16 nl_arr_idx);


void oam_update_usr_updateded_neigh_to_nliu(UInt16 nliu_arr_idx, UInt16 nl_arr_idx);

#ifdef OAM_SON_ENABLE
void oam_update_intra_rat_nr(son_intra_rat_neighbor_info_t *p_to,
                             son_intra_rat_neighbor_info_t *p_from);


/* SPR-13586 START */
void oam_update_inter_rat_nr(son_utran_neighbor_info_t *p_to,
                             son_utran_neighbor_info_t *p_from);
void oam_update_inter_rat_gsm_nr(son_geran_neighbor_info_t *p_to,
                             son_geran_neighbor_info_t *p_from);
/* SPR-13586 END*/
#endif
/*End: SPR 10567 */
/*SPR 12200 Fix Start */
void oam_copy_operator_info_parameter_to_reconfig(
        rrm_oam_operator_info_t *p_cell_reconf_operator_info, 
        rrm_oam_operator_info_t *p_cell_conf_operator_info);

void oam_copy_CA_parameter_to_reconfig(
        rrm_oam_operator_info_t *p_cell_reconf_operator_info, 
        rrm_oam_operator_info_t *p_cell_conf_operator_info);

void oam_copy_power_control_parameter_to_reconfig(
        rrm_oam_operator_info_t *p_cell_reconf_operator_info, 
        rrm_oam_operator_info_t *p_cell_conf_operator_info);

void oam_copy_radio_admission_control_info_parameter_to_reconfig(
        rrm_oam_operator_info_t *p_cell_reconf_operator_info, 
        rrm_oam_operator_info_t *p_cell_conf_operator_info);

void oam_copy_sib3_info_parameter_to_reconfig(
        rrm_oam_operator_info_t *p_cell_reconf_operator_info, 
        rrm_oam_operator_info_t *p_cell_conf_operator_info);

void oam_copy_sib2_info_parameter_to_reconfig(
        rrm_oam_operator_info_t *p_cell_reconf_operator_info, 
        rrm_oam_operator_info_t *p_cell_conf_operator_info);

/*SPR 12200 Fix End*/

oam_return_et
oam_qcli_save_current_config(UInt8 *p_buf, oam_error_code_et *p_error_code,
                             UInt16 trans_id, UInt8 dest_id);

oam_return_et
oam_qcli_maintain_parameter_persistance(Char8 *cfg_filename,
                                        oam_error_code_et *p_err);

/* + SPR 17439 */
void oam_maintain_parameter_persistence(void);
/* - SPR 17439 */


oam_return_et
oam_validate_cell_specific_max_non_csg_members(UInt8 *p_val,
                                           SInt32 loop,
                                           UInt8* opt);

oam_return_et
oam_validate_cell_specific_max_csg_members(UInt8 *p_val,
                                           SInt32 loop,
                                           UInt8* opt);


oam_return_et
oam_validate_cell_specific_csgid(UInt8 *p_val, SInt32 loop, UInt8* opt);


oam_return_et
oam_validate_cell_specific_max_ues_served(UInt8 *p_val,
                                          SInt32 loop,
                                          UInt8* opt);


oam_return_et
oam_validate_cell_specific_access_mode(UInt8 *p_val,
                                       SInt32 loop,
                                       UInt8* opt);

/* SPR-9485 START */
#ifdef OAM_SON_ENABLE
/* SPR 13937 Fixed Start */
/* + SPR 17439 */
void oam_killall_init_spawn(void);
/* - SPR 17439 */
/* SPR 13937 Fixed End */

/* Spr 15612  Fix Start*/
oam_return_et 
oam_validate_and_send_utran_update_req
          (void *p_api_buf, son_oam_nr_update_req_t *p_req, 
    /* SPR 18930  FIX START */
           UInt8 cid,
           oam_bool_et * p_is_dom_update_reqd);
    /* SPR 18930  FIX END */        
/* Spr 15612  Fix End*/ 
#endif

/* SPR 16334 START */
/* coverity 66603 Fix Start */

oam_return_et 
addUpdateMMEInfo
(
 s1ap_oam_mme_info_t *p_oam_s1ap_mme_info,
 SInt8 mme_index
);
/* coverity 66603 Fix End */

void
set_bitmask_IPV6Addr
(
 int loop,
 void * p_cell_reconfig_req
);   

#ifdef OAM_SON_ENABLE
void createCurrGrpIdList
  (local_oam_anr_gu_group_info_t * p_local_gu_group_info,
   /*Coverity 66604 Fix Start*/
   s1ap_served_gummei_info_t * p_served_gummei_info
   /*Coverity 66604 Fix End*/
  );
#else
void createCurrGrpIdList
(
        local_oam_gu_group_id_list_t *p_local_gu_group_info,
        /*Coverity 66604 Fix Start*/
        s1ap_served_gummei_info_t *p_served_gummei_info
        /*Coverity 66604 Fix End*/
);
#endif

oam_bool_et getDelta
(
#ifdef OAM_SON_ENABLE
 local_oam_anr_gu_group_info_t *p_local_gu_group_info,
 oam_son_anr_gu_group_id_config_req_t *p_gu_group_info,
#else
 local_oam_gu_group_id_list_t *p_local_gu_group_info,
 oam_x2ap_enb_config_update_req_t *p_gu_group_info,
#endif
/*Coverity 66605 Fix Start*/
s1ap_oam_mme_info_t *oam_s1ap_mme_info,
/*Coverity 66605 Fix Start*/
 SInt8 *mme_index
);


#ifndef OAM_SON_ENABLE

oam_void_t
oam_update_connected_gu_grp_id_list
(
 oam_x2ap_enb_config_update_req_t *ecu_req,
 UInt8 enb_update_all
);
/* SPR Fix 18741 Start */
oam_bool_et
oam_check_ecu_send_required
(void
);
/* SPR Fix 18741 End */

oam_void_t
oam_set_x2_ecu_required
(
Char8 * parmeter_name
);

oam_void_t
oam_update_list_per_enb
(
    oam_peer_enb_data_t peer_enb_data,
    oam_x2ap_enb_config_update_req_t *ecu_req
);

oam_return_et
oam_update_list_all_enb
(
 oam_x2ap_enb_config_update_req_t *ecu_req
);


oam_return_et
oam_send_x2ap_enb_config_update_req_at_link_up
(
    oam_peer_enb_data_t peer_enb_data
);

oam_return_et
oam_send_x2ap_enb_config_update_req_first
(
    oam_peer_enb_data_t peer_enb_data
);

oam_return_et
oam_send_x2ap_enb_config_update_req
(
 oam_x2ap_enb_config_update_req_t *ecu_req
);

oam_void_t
oam_process_add_resp_list
(
    x2ap_enb_comm_info_list_t *p_enb_list,
    oam_return_et status,
    UInt16 trans_id_resp,
    UInt16 dst_id
);

oam_return_et
oam_add_x2ap_enb_req
(
oam_x2ap_add_enb_req_t *p_req,
UInt16 transaction_id 
);

oam_void_t
oam_update_enb_list
(
SInt16 index,
SInt16 idx
);

/* SPR Fix 18741 Start */
SInt16
oam_get_new_enb_idx(void);
/* SPR Fix 18741 End */

SInt16
x2_get_idx_from_nbr_enb_add_list
(
oam_x2ap_add_enb_req_t *x2ap_info ,
x2_enb_comm_info_t  *tnl_addr
);

oam_void_t
oam_update_serving_enb_info
(
 oam_x2ap_enb_config_update_req_t *ecu_req
);

#endif
/* SPR 16334 END */



/* SPR 15238 START */
oam_access_mode_et
oam_init_nbr_cell_access_mode_pci_validation(
                                             XML_struct_cell_config *p_cell_spec_params,
                                             oam_eutran_nbr_cell_type_et nbr_cell_type,
                                             UInt32 eutra_carrier_arfcn,
                                             U16 nbr_pci,
                                             SInt32 access_mode);
oam_return_et 
oam_populate_eutran_intra_freq_pci_ranges(
                                             XML_struct_cell_config *p_cell_spec_params,
                                             per_freq_pci_ranges_t *per_freq_pci_ranges);

oam_return_et 
oam_populate_eutran_inter_freq_pci_ranges(
                                          XML_struct_cell_config *p_cell_spec_params,
                                          per_freq_pci_ranges_t *per_freq_pci_ranges, 
                                          UInt32 eutra_carrier_arfcn);
U16
oam_map_pci_range (rrm_oam_cell_id_range_et pci_range);
/* SPR Fix 17928 Start */
#ifdef OAM_SON_ENABLE
/*  coverity 94871/83280 fix start */
U16
son_oam_map_pci_range (son_oam_cell_id_range_et pci_range);
/*  coverity 94871/83280 fix end */
#endif
/* SPR Fix 17928 End */

/* SPR 15238 END */
/* SPR 15366 START */
void
oam_pm_handle_shutdown_command(UInt8 CellId);
/* SPR 15366 END */


oam_return_et
oam_validata_send_nw_scan_config_cmd(UInt8 *p_param_blk,
                                     UInt8 num_params,
                                     oam_error_code_et *p_err);

/* SPR 15527 START */
oam_return_et 
oam_add_qos_object(Char8 * add_obj_name,
                   UInt8 loop,
                   Char8 * buff);

oam_return_et
validate_qos_grandchild
(
 UInt8 *param_value,
 int loop,
 UInt8* get_value
);
/* SPR 15527 END */

oam_return_et oam_send_init_time_anr_meas_config_req(UInt8 cid);


/* BUG_FIXED_12828_START */

oam_return_et oam_send_init_time_nr_add_req(UInt8 src_cell_id );

/* BUG_FIXED_12828_START */
/* BUG_FIXED_13186_START */
void oam_persist_one_parameter(xmlNode  *parent,
        Char8    *tag_name,
        UInt8    *p_value);

/* BUG_FIXED_13186_END */
/* SPR 14498 START */
void oam_persist_one_multi_instance_parameter
(
     xmlNode  *parent,
     Char8    *tag_name,
     UInt8    *p_value,
     UInt8     index
 );
/* SPR 14498 END */
/* SPR 13542 START */
void
oam_update_plmnlist_from_mcc_mnc
(
    void * p_input_plmnid,
    UInt8 index,
    bc_plmn_list_t *p_bc_plmnList
);

oam_return_et
oam_update_s1ap_enb_config(void);
/* SPR 13542 END */

/* SPR 15266 Fix Start */
oam_bool_et
    /* SPR 18930  FIX START */
oam_add_neighbour_to_NL_n_check_if_update_req_at_son_nrt (SInt16 neigh_idx, SInt8);
    /* SPR 18930  FIX END */        
/* SPR 15266 Fix End */


/*eICIC_PHASE_1_2_CHANGES_START */  
oam_return_et
oam_conversion_abs_pattern_byte_string_to_bitstring
(Char8 *p_byte_string , UInt8* p_bit_string);

oam_return_et
oam_conversion_abs_pattern_bit_string_to_bytestring
(Char8 *p_byte_string , UInt8* p_bit_string);


void
set_bitmask_epc_info(int loop, void *p_cell_reconfig_req);


void
set_bitmask_eicic_provision_type(int loop, void *p_cell_reconfig_req);

void
set_bitmask_rsrp(int loop, void *p_cell_reconfig_req);

void
set_bitmask_rsrq(int loop, void *p_cell_reconfig_req);

void
set_bitmask_eligible_ue(int loop, void *p_cell_reconfig_req);

void
set_bitmask_num_abs_report(int loop, void *p_cell_reconfig_req);

void
set_bitmask_abs_usage_low_threshold(int loop, void *p_cell_reconfig_req);

void
set_bitmask_abs_usage_high_threshold(int loop, void *p_cell_reconfig_req);

void
set_bitmask_low_load_abs_pattern(int loop, void *p_cell_reconfig_req);

void
set_bitmask_victim_abs_pattern(int loop, void *p_cell_reconfig_req);

/*SPR 14180 start*/
void set_bitmask_victim_meas_subset(int loop, void *p_cell_reconfig_req);
/*SPR 14180 end*/

void
set_bitmask_mac_eicic_meas_report_periodicity(int loop, void *p_cell_reconfig_req);

void
set_bitmask_aggressor_sel_timer(int loop, void *p_cell_reconfig_req);

void
set_bitmask_load_information_guard_timer(int loop, void *p_cell_reconfig_req);

void
set_bitmask_load_information_collation_timer(int loop, void *p_cell_reconfig_req);

void
set_bitmask_dl_sinr_threshold_low_mark(int loop, void *p_cell_reconfig_req);

void
set_bitmask_dl_sinr_threshold_high_mark(int loop, void *p_cell_reconfig_req);

void
oam_copy_eICIC_parameter_to_reconfig(
        rrm_oam_operator_info_t *p_rrm_oam_cell_reconf_operator_info,
        rrm_oam_operator_info_t * p_rrm_oam_cell_conf_operator_info);
/*eICIC_PHASE_1_2_CHANGES_END */  
#ifdef OAM_SON_ENABLE
/*SPR 15438 START */
UInt32 oam_tokenize_son_pci_list(const Char8* p_str,
                                      const Char8* item_sep,  
                                      const Char8* range_sep,
                                      oam_son_pci_selection_req_t  *p_pci_selection_req,
                                      oam_cell_neighbourlist_info_t *p_nr_info,
                                      UInt32 max_tokens,
                                      oam_error_code_et *p_err);
/*SPR 15438 END */
/* SPR 18930  FIX START */
void oam_save_nr_add_req_with_trans_id
(
 oam_son_nr_add_req_t  * p_nr_add_req,
 UInt16 trans_id,
 UInt8 cell_idx
 );

void oam_find_nr_add_req
(
 UInt16 trans_id,
 UInt8 cell_idx
 );
void oam_prepare_nr_list_for_nliu
(
 oam_son_nr_add_resp_t * p_layer_resp,
 oam_son_nr_add_req_t  * p_nr_list,
 oam_son_nr_add_req_t  * p_nr_list_added,
 oam_son_nr_add_req_t  * p_nr_list_deleted,
 SInt8 nliu_cell_config_idx
 );
oam_return_et
oam_add_nbr_to_nl_and_send_delete_req_to_son
(               
 UInt8 * p_api_buf,
  SInt8  cell_arr_idx1
   ); 

/* SPR 18930  FIX END */
#endif

/* SPR_19279_START */
oam_return_et oam_handle_plmnlist_retrieve_req
(
 UInt16             dst_id,
 UInt16             resp_trans_id
); 
/* SPR_19279_END */

/* SPR 21889 Start */ 
void oam_send_kill_to_all_agent();
oam_return_et oam_send_log_req_for_agent(UInt8 i);
oam_return_et
oam_trigger_agent_fsm(void *p_api_buf,
         oam_agent_event_et event,
         UInt8 instance_id);
oam_return_et 
oam_collect_logs();
oam_return_et
oam_fill_process_name(Char8 *p_dst_name, Char8 *p_src_name);
oam_return_et
oam_fill_process_args(Char8 *p_bin_name, oam_process_info_t *p_proc_info, Char8 *p_args);
Char8* oam_agent_get_event_str(oam_agent_event_et event);
/* SPR 21889 End */ 
/* SPR 15527 START */
extern void oam_find_fap_index(Char8 *p_parent_name, 
                           UInt8 *p_fap_index_value);
/* SPR 15527 END */

/* Spr 17753 Changes Start */
oam_bool_et 
is_pci_available_for_seving_cell(UInt16 pci,
                                 oam_rrm_cell_config_req_t *p_rrm_cellconfig);

void oam_populate_serving_cell_phy_cell_id(oam_rrm_cell_config_req_t *p_rrm_cellconfig,
                                           XML_struct_cell_config    *p_cell_spec_params);
/* Spr 17753 Changes End*/
/*spr 22474 start*/
UInt16
oam_get_l3_debug_info_prints
(
 rrc_oam_get_debug_info_resp_t * p_rrc_resp,
 UInt8*** str_arr
);
UInt16
oam_get_rrm_debug_info_prints
(       
 rrm_oam_get_debug_info_resp_t * p_rrm_resp,
 UInt8*** str_arr 
);   
UInt16
oam_get_son_debug_info_prints
(       
 son_oam_get_debug_info_resp_t * p_son_resp,
 UInt8*** str_arr 
);   
UInt16
oam_get_oam_debug_info_prints
(       
 oam_get_debug_info_resp_t * p_oam_resp,
 UInt8*** str_arr 
);   

/*spr 22474 end*/


#endif
/* EMBMS Changes Start */
/* SPR 22248 MBMS Changes Start */
#define M2AP_OAM_PROVISION_REQ_MASK                          0x1000000000000000
#define M2AP_OAM_SET_LOG_LEVEL_REQ_MASK                      0x2000000000000000
/* SPR 22248 MBMS Changes Stop */
/* EMBMS Changes End */

#ifdef LTE_EMBMS_SUPPORTED
oam_return_et oam_copy_m2ap_config_to_reconfig(
        oam_m2ap_provision_req_t *p_m2ap_prov_req,
        m2ap_config_update_req_t *p_m2ap_config_update_req);
#endif

