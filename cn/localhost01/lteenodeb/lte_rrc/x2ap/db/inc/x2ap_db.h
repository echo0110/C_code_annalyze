/*******************************************************************************
*
*  FILE NAME   : x2ap_db.h
*
*  DESCRIPTION : This file contains the structures related to x2ap data base.
*
*  DATE 24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _X2AP_DB_H_
#define _X2AP_DB_H_

#include "commoninc.h"
#include "rrc_x2apEnb_intf.h"
#include "x2ap_search_tree.h"
#include "x2ap_sctp_sm.h"
#include "x2ap_enb_conf_sm.h"
#include "rrc_defines.h"
#include "rrc_x2ap_uecc_intf.h"

#define MAX_NUM_CELL_IN_X2AP   MAX_NUM_CELLS

/* SPR_14436_Fix Start */
#define MAX_NUM_X2AP_SCTP_STREAM 	10
#define SCTP_NON_UE_ASSOCIATED_STREAM 	0
/* SPR_14436_Fix Stop */

/* X2AP FSM states
*/
typedef enum
{
 X2AP_INIT_ST,
 X2AP_WF_PROVISIONING_REQ_ST,
 X2AP_ACTIVE_ST
}x2ap_sm_state_et;

/* x2ap_reset start */
typedef enum
{
    X2_RESET_IDLE_ST,
    X2_RESET_ACTIVE_ST,
    X2_RESET_W_RESET_RESP_ST
}x2ap_reset_sm_state_et;
/* x2ap_reset end */

typedef enum
{
    X2_CELL_ACT_IDLE_ST,
    X2_CELL_ACT_ACTIVE_ST,
    X2_CELL_ACT_W_PEER_ENB_RESP_ST,
    X2_CELL_ACT_W_RRM_RESP_ST
}x2ap_cell_act_sm_state_et;

typedef enum
{
    X2_MOBILITY_CHANGE_IDLE_ST,
    X2_MOBILITY_CHANGE_ACTIVE_ST,
    X2_W_MOBILITY_CHANGE_RRM_RESP_ST,
    X2_W_MOBILITY_CHANGE_PEER_RESP_ST
}x2ap_mobility_ch_sm_state_et;

typedef struct 
{
    x2ap_timer_t conn_timer_id;
    x2ap_timer_t conn_rec_timer_id;
    U32          connect_ret_count;
    /* State of SCTP SM */    
    x2ap_sctp_state_et cur_sctp_sm_state;
}x2ap_sctp_sm_cntx_t;

typedef struct 
{
    U32                    x2Setup_curr_state;
    x2ap_timer_t           x2Setup_timerId;
    x2ap_timer_t           x2setup_time_to_wait_id;
    U8                     x2setup_retry_count;
}x2ap_setup_sm_cntx_t;

/* ENB Status */
typedef enum
{
    NOT_ASSOCIATED = 0,
    ASSOCIATED,    
    OPERATIONAL    
}x2ap_enb_status_et;

/*x2ap_start*/
typedef struct 
{
    U32        x2ap_ue_asso_sig_curr_state;
}x2ap_ue_asso_sig_sm_ctx_t;

/* X2AP UE Context associated with Peer eNodeB */
typedef struct
{
    /* 
     * This id ia allocated by the source eNB and it uniquely 
     * identifies an UE over the X2 interface within an eNB 
     */
    U16  old_enb_ue_x2ap_id; 

    /* 
     * This id ia allocated by the Target eNB and it uniquely 
     * identifies an UE over the X2 interface within an eNB 
     */
    U16 new_enb_ue_x2ap_id;

    U8  cell_index;

    /* SPR_14436_Fix Start */
    U16 stream_id;
    /* SPR_14436_Fix Stop */
#ifdef ENDC_ENABLED
    /* 
     * This id is allocated by the MeNB and it uniquely 
     * identifies UE over the X2 association 
     */
    U32  menb_ue_x2ap_id;
#endif
}ue_x2ap_context_t;

typedef struct
{
    /*
     * This is a search tree whose key value is old_enb_ue_x2ap_id
     */
    x2ap_search_tree_t          old_enb_ue_x2ap_id_search_tree;
    /*
     * This is a search tree whose key value is new_enb_ue_x2ap_id
     */
    x2ap_search_tree_t          new_enb_ue_x2ap_id_search_tree;

}x2ap_ue_ctxt_srch_tree_t;

/*x2ap-end*/

/*x2ap reset start*/
typedef struct 
{
   x2ap_reset_sm_state_et  state;
   U8 retry_count;
   x2ap_timer_t timer_id;
   U16 trans_id;
   x2ap_reset_req_t reset_request_msg;
}x2ap_reset_sm_ctx_t;
/*x2ap reset end*/

typedef struct 
{
   x2ap_cell_act_sm_state_et  state;
   U8 retry_count;
   x2ap_timer_t timer_id;
   U16 trans_id;
   x2ap_peer_cell_activation_req_t cell_act_request_msg;
}x2ap_cell_act_sm_ctx_t;
typedef struct 
{
   x2ap_mobility_ch_sm_state_et  state;
   U8 retry_count;
   x2ap_timer_t timer_id;
   U16 trans_id;
   x2ap_mobility_change_req_t mobility_change_req;
}x2ap_mobility_ch_sm_ctx_t;

/* SPR_14436_Fix Start */
typedef struct _x2ap_stream_info_t
{
    /*The array holds the stream ids of an SCTP association. */
    U16 stream_ids[MAX_NUM_X2AP_SCTP_STREAM];

    /*This field is used to distribute streams among UEs in a 
     *Round Robin fashion.
     */
    U8 stream_idx_returned;

    /*The field represents total number of streams present between 
     * eNB and peer eNB.
     */
    U8  number_of_streams;

}x2ap_stream_info_t;
/* SPR_14436_Fix Stop */

/* SPR_14436_Fix Start */
typedef struct
{
    /* 
     * This id ia allocated by the source eNB and it uniquely 
     * identifies an UE over the X2 interface within an eNB 
     */
    U16  old_enb_ue_x2ap_id;

    /* This ID uniquely identifies the stream in an SCTP association between eNB
     * and Peer eNB.
     */
    U16 stream_id;

}x2ap_ho_ongoing_ue_ctx_t;

typedef struct _x2ap_ho_ongoing_ue_ctx_node_t
{
    /* List node Anchor */
    YLNODE node;

    /* ho ongoing ue context */
    x2ap_ho_ongoing_ue_ctx_t ho_ongoing_ue_ctx;
}x2ap_ho_ongoing_ue_ctx_node_t;
/* SPR_14436_Fix Stop */

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
#define X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT    0x01
/* MENB CHANGES - END */
#endif
/*  Peer EnodeB Context */
typedef struct
{
#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
    U8                  bitmask; 

    U8                  peer_enb_type;

    x2ap_endc_config_sm_context_t     endc_config_sm_ctx;

/* MENB CHANGES - END */
#endif
    U8                  peer_enodeb_id;
    U8                  is_enb_conf;
    U8                  is_del_enb_ongoing;
    x2_enb_comm_info_t  enb_comm_info;
    x2_gb_enb_id_t         gb_enb_id;
    sctp_sd_t           sctp_sd;
    x2ap_gu_grp_ids_t   x2ap_gu_grp_id;
    x2ap_enb_status_et  op_status;
    /* SCTP SM Context */
    x2ap_sctp_sm_cntx_t            sctp_sm_ctx;
    /* X2AP eNB CONFIGURATION UPDATE SM Context */
    x2ap_enb_config_sm_context_t     enb_config_sm_ctx;
    /* X2AP SETUP SM Context */
    x2ap_setup_sm_cntx_t       x2_setup_sm_ctx; 
    /* X2AP RESET SM Context */
    x2ap_reset_sm_ctx_t     x2ap_reset_sm_ctx;
    /* X2AP MOBILITY SETTING CHANGE Context */
    x2ap_mobility_ch_sm_ctx_t  x2ap_mobility_ch_sm_ctx;
    x2ap_bool_et               is_x2setup_sent;
    /*UE Associated Signalling Message SM  */
    x2ap_ue_asso_sig_sm_ctx_t  x2ap_ue_asso_sig_sm_ctx;
/* CSR_00040799 Fix Start */
    /*Array of pointers of type ue_x2ap_context_t*/
    ue_x2ap_context_t * *ue_x2ap_contexts;
/* CSR_00040799 Fix End */
    x2ap_ue_ctxt_srch_tree_t    ue_ctxt_trees;   
    /* X2AP CELL ACTIVATION SM Context */
    x2ap_cell_act_sm_ctx_t    x2ap_cell_act_sm_ctx;
    /* SPR_14436_Fix Start */
    /* This contains description of streams present between a 
     * particular eNB and peer eNB.
     * */
    x2ap_stream_info_t stream_info;
    /* SPR_14436_Fix Stop */
    /*SPR_16533_START*/
        /* Code Removed */
    /*SPR_16533_END*/
    /*SPR_18154_START*/
    x2ap_bool_et flag_for_consecutive_x2setup_resp_failure;
    /*SPR_18154_END*/
#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
    x2_gb_gnb_id_t                    gb_gnb_id;
    x2ap_bool_et flag_for_consecutive_endc_x2setup_resp_failure;
/* MENB CHANGES - END */
#endif
}x2ap_peer_enb_context_t ;

/*
 * CSPL Node for   
 * Search tree : Neighbour cell
 * Key         : ecgi 
 */
typedef struct
{
    /* CSPL nodeAnchor */
    x2ap_search_tree_node_t nodeAnchor;
    x2ap_ecgi_t      ecgi; 
    /* This is logical id of the neighbour cell used for indexing
     */
    U32             logical_id;
    U8 serving_cell_index;
}x2ap_nbr_ecgi_search_node_t;

#ifdef ENDC_ENABLED
/*
 * CSPL Node for   
 * Search tree : Neighbour cell
 * Key         : ecgi 
 */
typedef struct
{
    /* CSPL nodeAnchor */
    x2ap_search_tree_node_t nodeAnchor;
    x2ap_nr_ecgi_t      nr_cgi; 
    /* This is logical id of the neighbour cell used for indexing
     */
    U32             logical_id;
    U8 serving_cell_index;
}x2ap_nbr_nr_cgi_search_node_t;
#endif

typedef struct
{
    U32 server_sctp_fd;
#if 0	
#ifdef ARICENT_SCTP_IPR
    sctp_fd_set_st           sctp_fd_set;
#endif
#endif	

#ifdef LKSCTP
    fd_set                  sctp_fd_set;
#endif  
}x2ap_sctp_fd_info_t ;

/* X2AP Init SM Block */
typedef struct
{
    x2ap_timer_t        oam_init_ind_tim_id;    
    x2ap_timer_t        csi_guard_tim_id;
    x2ap_sm_state_et    x2ap_current_state;
}x2ap_init_blk_t;

/* Peer ENB Context*/
typedef enum 
{
    NOT_NEIGHBOUR = 0,
    INDIRECT_NEIGHBOUR,
    DIRECT_NEIGHBOUR_INACTIVE,
    DIRECT_NEIGHBOUR_ACTIVE
}x2ap_cell_relation_table_et;

typedef struct
{
	x2ap_cell_relation_table_et relation;
	x2ap_ecgi_t ecgi;
}x2ap_cell_relation_table_t;

typedef struct
{
/*#define X2AP_NUM_ANTENNA_INFO_PORT_PRESENT 0x01
#define X2AP_PRACH_CONFIG_INFO_PRESENT 0x02
#define X2AP_MBSFN_SUBFRAME_INFO_PRESENT 0x04*/
    U8                                bitmask; 
    U32                               pci; 
    x2ap_ecgi_t                       ecgi;
    x2ap_tac_t                        track_area_code; 
    x2ap_broadcast_plmn_t             broadcast_plmn_info; 
    x2ap_choice_eutra_mode_info_t     choice_eutra_mode;
    U8                                num_antenna_port; 
    x2ap_prach_configuration_info_t prach_config;
    x2ap_mbsfn_subframe_info_t mbsfn_subframe_info;
    x2ap_earfcn_t                     earfcn;
    U16 num_nbour;
    U8				      csg_identity[4];
#ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
#define NR_CELL_INFO_FIVE_GS_TAC_PRESENT 0x01
#define NR_CELL_INFO_CONFIGURED_TAC_PRESENT 0x02
    U16                               nr_pci;
    x2ap_nr_ecgi_t                    nr_ecgi;
    x2ap_choice_nr_mode_info_t        choice_nr_mode;
    x2ap_five_gs_tac_t                five_gs_tac;
    x2ap_tac_t                        configured_tac;
    x2ap_nr_freq_info_t               nr_freq_info;
    U8                                meas_timing_config_count;
    x2ap_meas_timing_config_t         x2ap_meas_timing_config[RRC_MAX_MEAS_FREQ_MN];
    /* MENB CHANGES - END */
#endif
}x2ap_cell_info_t;


/* Neighbouring Cell Info  
 * Note : enb_id info is updated after x2setup response
 *        Neighbour info is updated from OAM PROVISION REQ 
 */
typedef struct
{
    U8                     enb_type;  /* Macro or Home */
    x2_gb_enb_id_t            peer_gb_enb_id; 
    x2ap_cell_info_t       peer_cell_info;
    /* enb_configured changes start */
    U8                     enb_configured;
    /* enb_configured changes stop */

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    x2_gb_gnb_id_t            peer_gb_gnb_id;
    rrc_bool_et               current_cell_info; 
    rrc_bool_et               oam_configured; 
    /* MENB CHANGES - END */
    #endif
}x2ap_peer_enb_cell_info_t;

/* 
 * CSPL Node for   
 * Search tree  : Peer EnodeB
 * Key          : Socket descriptor sd
 */
typedef struct
{
    /* CSPL nodeAnchor */
    x2ap_search_tree_node_t  nodeAnchor;
    sctp_sd_t   sctp_sd;  /* Key */
    x2ap_peer_enb_context_t  *p_enb_context;
}x2_sctp_sd_search_node_t;


/* 
 * CSPL Node for   
 * Search tree  : Peer EnodeB
 * Key          : gb_enb_id
 */
typedef struct 
{
    /* CSPL nodeAnchor */
    x2ap_search_tree_node_t nodeAnchor;
    x2_gb_enb_id_t     gb_enb_id; /* Key */
    x2ap_peer_enb_context_t  *p_enb_context;
}sctp_gb_enb_id_search_node_t;

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
/* 
 * CSPL Node for   
 * Search tree  : Peer gnodeB
 * Key          : gb_gnb_id
 */
typedef struct 
{
    /* CSPL nodeAnchor */
    x2ap_search_tree_node_t    nodeAnchor;
    x2_gb_gnb_id_t             gb_gnb_id;
    /* Key */
    x2ap_peer_enb_context_t    *p_enb_context;
}sctp_gb_gnb_id_search_node_t;
/* MENB CHANGES - END */
#endif

/*
 * CSPL Node for   
 * Search tree   : Peer EnodeB
 * Key           : ip_address 
 */
typedef struct
{
    /* CSPL nodeAnchor */
    x2ap_search_tree_node_t    nodeAnchor;
    U8  enb_ip_address[MAX_X2AP_IP_ADDRESS_LENGTH]; /* Key */
    x2ap_peer_enb_context_t    *p_enb_context;
    U8  enb_ipv6_address[MAX_X2AP_IPV6_ADDRESS_LENGTH]; /* Key */

}sctp_ip_search_node_t;

/* List of Peer EnodeB Context Search Trees */
typedef struct
{
    /*
     * This is a search tree whose key value is sctp_sd
     * allocated while opening association with peer eNB
     */
    x2ap_search_tree_t          sd_search_tree;
    /*
     * This is a search tree whose key value is ip_address
     * allocated while opening association with peer eNB
     */
    x2ap_search_tree_t          ip_search_tree;

    /*
     * This is a search tree whose key value is peer enbID
     */
    x2ap_search_tree_t          enb_id_search_tree;
    /*spr8341 IPv6 fix start*/

    x2ap_search_tree_t          ipv6_search_tree;
    /*
     * This is a search tree whose key value is ipv6_address
     * allocated while opening association with peer eNB
     */
    /*spr8341 IPv6 fix end*/

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    x2ap_search_tree_t          gnb_id_search_tree;
    /* MENB CHANGES - END */
    #endif

}x2ap_enb_ctxt_srch_tree_t;

/* Active cell Info */
typedef struct
{
    U8  cell_id;
    x2ap_bool_et is_cell_active;
}x2ap_act_cell_info_t;

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
#define X2AP_GB_CONTEXT_GB_GNB_ID_PRESENT    0x01
/* MENB CHANGES - END */
#endif
typedef struct
{
    /*
     * Local ENB Context 
     */
    x2_enb_comm_info_t          local_enb_info;
    x2_gb_enb_id_t                 global_enb_id;

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    U8                          bitmask;

    x2_gb_gnb_id_t              global_gnb_id;

    U8                          num_peer_gnb;

    /* Neighbour NR_CGI to Neighbour Cell Logical Id */
    x2ap_search_tree_t          nbr_nr_cgi_search_tree;
    /* MENB CHANGES - END */
    #endif

    /* SPR 20633 changes start */
//#ifdef ARICENT_SCTP_IPR
//    sctp_sd_t                   server_fd;
//#else
    sctp_sd_t                   server_fd_ipv4;
    sctp_sd_t                   server_fd_ipv6;
//#endif
    /* SPR 20633 changes end */
    /* Local Group Id */
    x2ap_gu_grp_ids_t           group_ids;

    /* 
     * Peer ENB Context 
     */
    U8                           num_peer_enb;
    x2ap_peer_enb_context_t      peer_enb_cntxt_arr[MAX_PEER_ENB];
    x2ap_enb_ctxt_srch_tree_t    enb_ctxt_trees;

    /* 
     * Local Served Cell Context 
     */    
    U8                          num_served_cells;
    /* Served Cell Info */
    x2ap_cell_info_t            served_cell_info[MAX_NUM_CELL_IN_X2AP];

    /* Active Served Cell Info, list of cell in the order in which they are added, contains cell_id + active status */
    x2ap_act_cell_info_t        act_cell_info[MAX_NUM_CELL_IN_X2AP];
 
    /*
     * Neighbour Cell Context for all serving cells 
     */
    /*Number Total (Direct/Indirect) Neighbour Cell*/
    U32                         num_peer_enb_cells;

    /*List of nbour cell in the order in which they are added, contains ecgi + earfcn + pci*/
    x2ap_peer_enb_cell_info_t   peer_enb_cell_info[MAX_NEIGHBOURS];

    /* Neighbour ECGI to Neighbour Cell Logical Id */
    x2ap_search_tree_t          nbr_ecgi_search_tree;

    /* 
     * Column Index :   logical id of the Neighbour cell
     * Row index    :   Cell Id of the served Cell
     */
    x2ap_cell_relation_table_t  relation_table[MAX_NUM_CELL_IN_X2AP][MAX_NEIGHBOURS];

    /* Other Configuration Parameters (SCTP/Timer/Retry Count)*/
    x2ap_conf_param_t            config_param;

    /* X2ap Init SM Block */
    x2ap_init_blk_t              x2ap_init_sm_blk;

    /* Cell UP Guard Timer Id */
    x2ap_timer_t                 cell_up_guard_tim_id;

    /* To update the structure with the served cell 
       at the Peer EnB for RSU*/
    x2ap_link_info_t               x2ap_link_info[MAX_PEER_ENB];
    
    /* RSU search tree */
    x2ap_search_tree_t                 x2ap_rsu_context_tree;
 
    U8                                is_prov_done;

    U16                               global_x2ap_rsu_meas_id ;

    U8                                no_of_uecc_instances;

    void* p_comm_info_int;/* Pointer on internal representation of communication info */
    U8 is_son_present;
    /*sctp parameters start*/
    rrc_timeval_t                 sctp_start_time;
    /*sctp parameters stop*/

/* CSR_00040799 Fix Start */
    /* Sum of UE's supported in all the cells */
    U16                           total_ue_supported;
/* CSR_00040799 Fix End */

    /* SPR_14436_Fix Start */
    YLIST 			  ho_ongoing_ue_ctx_list;
    /* SPR_14436_Fix Stop */
    x2ap_bool_et    log_enabled;  /* current state of logger(on/off) */
    U8                   log_level;  
}x2ap_gb_context_t;
 
#endif
