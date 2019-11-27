/******************************************************************************
*
*   FILE NAME:
*       uecc_ue_ctx.h
*
*   DESCRIPTION:
*       This is the UECC UE context data definitions file.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   20 Jul 2009     GrygoriyS   ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef UECC_UE_CTX_H
#define UECC_UE_CTX_H

/****************************************************************************
 * Inculdes
 ****************************************************************************/

#include "rrc_defines.h"
#include "rrc_rrm_intf.h"
#include "rrc_s1ap_uecc_intf.h"
#include "rrc_uecc_llim_intf.h"
#include "uecc_fsm_common.h"
#include "rrc_s1ap_asn_intrl_3gpp.h"
#include "rrc_x2apEnb_intf.h"
#include "rrc_x2ap_uecc_intf.h"
#include "uecc_statistic.h"
#include "uecc_ue_timer_utils.h"
#include "ylib.h"
#include "rrc_s1u_intf.h"
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
typedef enum
{
    INVALID_HO,
    S1_HO,
    X2_HO,
    INTRA_CELL_HO,
    CCO_HO,
    INTER_CELL_HO
}ho_type_et;

/*SPR 5253 Start*/
typedef enum
{
    OLD_X2AP_UE_ID,
    NEW_X2AP_UE_ID
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    ,MENB_X2AP_UE_ID
    ,SGNB_X2AP_UE_ID
/*NR_DC Code Change Stop*/
#endif
}x2ap_id_type_et;
/*SPR 5253 Stop*/

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
typedef enum
{
    NORMAL_UE = 0,
    DC_UE = 1
}uecc_ue_type_et;
/*NR_DC Code Change Stop*/
#endif

typedef enum
{
    RRC_UECC_FAIL_TYPE4,
    RRC_UECC_FAIL_TYPE16,
    RRC_UECC_FAIL_TYPE21,
    RRC_UECC_FAIL_TYPE23,
    RRC_UECC_FAIL_TYPE24,
    RRC_UECC_FAIL_TYPE28,
    RRC_UECC_FAIL_TYPE34,
    /*SPR_16618_START*/
    /* SPR 16750 Fix Start */
    /* Code Removed */
    /* SPR 16750 Fix Stop */
}rrc_uecc_failtype_action_et;
#define RECONFIGURATION_Q_INIT(p_ue_reconfig_queue)\
       ylInit(p_ue_reconfig_queue)

#define RECONFIGURATION_Q_GET_COUNT(p_ue_reconfig_queue)\
       ylCount(p_ue_reconfig_queue)

#define RECONFIGURATION_Q_ENQUEUE(p_ue_reconfig_queue, \
        p_ue_reconfig_queue_node)\
       ylPushTail( p_ue_reconfig_queue, \
               &(p_ue_reconfig_queue_node->node))

#define RECONFIGURATION_Q_DEQUEUE(p_ue_reconfig_queue , snode) \
       snode = ylPopHead(p_ue_reconfig_queue);\

#define RECONFIGURATION_Q_MAX_LENGTH 16

/*SPR_17976_START*/
#define TENB_CRE_Q_GET_COUNT(p_ue_tenb_cre_queue)\
       ylCount(p_ue_tenb_cre_queue)
/*SPR_17976_END*/

typedef enum
{
    MEAS_CONFIG_REQUEST = 0,
    UE_INFORMATION_REQUEST,
    COUNTER_CHECK_REQUEST
}ue_reconfig_msg_type_et;    

typedef struct 
{
    YLNODE      node;
    
    ue_reconfig_msg_type_et 
                message_type;

    U8          message[0];
}ue_reconfig_queue_node_t;

/*SPR_16416_START*/
typedef YLIST uecc_event_queue_t;
/*SPR_16416_END*/
/****************************************************************************
 * Public types definitions
 ****************************************************************************/
#define DL_FWD_INFO_INDEX            0
#define UL_FWD_INFO_INDEX            1
/* coverity fix 25346 */
#define MAX_MSI_SIZE                 512
/* coverity fix 25346 */    
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
#define SGNB_SEC_KEY_SIZE            256
/*NR_DC Code Change Stop*/
#endif

/* handover changes_KB */
#define RRC_PDCP_MAX_SHORT_MAC_I         32 
#define MAX_NAS_SECURITY_SIZE            7
#define RRC_INVALID_PEER_ENODEB_ID       0xFF
#define RRC_INVALID_UECC_MODULE_ID       0xFF

#define SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_ue_context)\
    (p_ue_context->share_data.uecc_rcr_data.\
     ue_context_rel_comp_not_req_flag = RRC_TRUE)

struct _uecc_gb_context_t;

/* structures to store data from S1AP in UECC context */

typedef struct
{
   s1ap_BitRate uEaggregateMaximumBitRateDL;
   s1ap_BitRate uEaggregateMaximumBitRateUL;
   s1ap_BitRate uEaggregateMaximumBitRateDLExt;
   s1ap_BitRate uEaggregateMaximumBitRateULExt;
} rrc_s1ap_ue_aggregate_maximum_bitrate_t;

typedef struct
{
   s1ap_EncryptionAlgorithms encryptionAlgorithms;
   s1ap_IntegrityProtectionAlgorithms integrityProtectionAlgorithms;
} rrc_s1ap_ue_security_capabilities_t;

typedef struct  
{
    YLNODE      anchor;
    RAT_Type    rat_type;
    rrc_bool_et valid_rat_string;
    U32         numocts;
    /* SPR 13502 Fix Start */
    U8          *p_rat_string;
    /* SPR 13502 Fix Stop */
} UE_Radio_Capability_RAT_Container_t;

typedef struct
{
    x2ap_E_RAB_ID                       e_RAB_ID;
    x2ap_E_RAB_Level_QoS_Parameters     e_RABlevelQoSParameters;
    x2ap_TransportLayerAddress          transportLayerAddress;
    x2ap_GTP_TEI                        gTP_TEID;
    rrm_cause_t                         cause;
    U8                                  status;
} uecc_x2ap_drb_ctx_t;


/* Information for DRB that is stored in UECC context */
typedef struct
{
    struct {
       /* Set of bits which defines if IEs have valid values */
       unsigned nAS_PDUPresent : 1;
       /* lipa start */
       /* this flag indicates the presence of lipa rab */
       unsigned Lipa_RabPresent : 1;
       /* lipa end */
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
       /* this flag indicates the presence of DC Bearer Type */
       unsigned DC_BearerTypePresent : 1;
       unsigned is_release_ongoing : 1;
       unsigned is_intra_enb_data_fwd : 1;
       unsigned rlcModePresent : 1;
       unsigned pdcpSnSizePresent : 1;
/*NR_DC Code Change Stop*/
#endif
    } m;
    s1ap_E_RAB_ID e_RAB_ID;
    s1ap_E_RABLevelQoSParameters e_RABlevelQoSParameters;
    s1ap_TransportLayerAddress transportLayerAddress;
    /* SPR_8254 Start */
    U8                         self_transport_address_length;
    U8                         self_transport_address[RRC_MAX_IP_ADDR_LEN_IPV6];
    /* SPR_8254 Stop */
    s1ap_GTP_TEID gTP_TEID;
    s1ap_NAS_PDU nAS_PDU;
    rrc_gtp_teid_t teid_self;
    s1ap_TransportLayerAddress transportLayerAddress_self;
    rrm_drb_config_t       drb_config;
    uecc_x2ap_drb_ctx_t    uecc_x2ap_drb_ctx; 
    /* lipa start */
    s1ap_Correlation_ID *correlation_id;
    /* lipa end */


#ifdef ENDC_ENABLED
    /*NR_DC Code Change Start*/
    U8                              dc_bearer_type; /* rrc_dc_bearer_type_et */
    x2ap_GTP_TEI                    sgnb_s1ap_gtp_id;
    x2ap_TransportLayerAddress      sgnb_s1ap_transportLayerAddress;
    /*NR_DC Code Change Stop*/

    /*OPTION3X Changes Start*/
    x2ap_GTP_TEI                    ul_x2u_teid_peer;
    x2ap_TransportLayerAddress      ul_x2u_transport_layer_addr;
    rrc_gtp_teid_t                  dl_x2u_teid_self;
    rrc_s1u_ie_gsn_addr_t           dl_x2u_transport_layer_addr_self;
    /*OPTION3X Changes Stop*/
    U8                              rlc_mode_sgnb;
    U8                              pdcp_sn_size_sgnb;
#endif
} uecc_drb_ctx_t;

/* UECC FSM failure descriptor definition */
typedef struct
{
    /* Pointer on current failure actions list */
    uecc_fsm_fail_handler_f const*  p_failure_actions_list;

    /* Failure action number for execution */
    U16                         failure_action_number;
} uecc_fsm_failure_descriptor_t;


typedef struct
{
    U16                   t;
    union
    {
    } u;
} uecc_rcs_data_t;

/* ICS START */

typedef enum
{
 NO_FAIL,
 FAILED_AT_RRM,
 FAILED_AT_LL
}erab_id_failed_status_et;



/* qos changes start*/
#define BEARER_TYPE_QOS_FAILURE       0x01
/* qos changes stop*/
typedef struct
{
    s1ap_E_RAB_ID e_RAB_ID;
    s1ap_Cause    cause;
    U32           error_code;
    U8            qos_status;
    U8            failed_at;
    /*CR 101 changes start*/
    U8            lc_status;  /* for checking whether erab to be released was active or not */
    /*Logical_Error*/
    U8            new_lcId;
    /*Logical_Error*/
    /*CR 101 changes stop*/
    #ifdef ENDC_ENABLED
    U8    dc_bearer_type;
    #endif
} erab_item_t;


typedef struct
{
    /* Status will indicate either successful ERAB or failure ERAB */
    U32 status;
    U8  erab_id;
    U8  lc_id;
}uecc_rm_erab_info_t;

typedef struct
{
    U16 outerResponse; /* i.e. SUCCESS, PARTIAL_SUCCESS or FAILURE */    
    /* No of ERABs that will be sent to RRM */
    U8       count;
    uecc_rm_erab_info_t erab_info[MAX_ERAB_COUNT];
}uecc_rm_erab_setup_info_t;

typedef struct
{
    U8                  count;
    /* Max ERAB count cannot be more than 16 */
            /*SPR_18582_FIX_START*/
    erab_item_t         erab_failed_to_setup_item[MAX_ERAB_COUNT];
            /*SPR_18582_FIX_END*/
}uecc_erab_failed_to_setup_list_t;


/* ICS STOP */



/* ERB RELEASE COMMAND START */
/*
typedef struct
{
    s1ap_E_RAB_ID e_RAB_ID;
    s1ap_Cause    cause;
} erab_item_t;
*/

typedef struct
{
    rrc_counter_t    counter;
    erab_item_t      erab_item[MAX_ERAB_COUNT];
} erab_list_t;

typedef struct 
{
    struct {
       /* Set of bits which defines if IEs have valid values */
       unsigned nAS_PDUPresent : 1;
    } m;
    s1ap_NAS_PDU nAS_PDU;
    erab_list_t     erab_release_list;
    erab_list_t     erab_to_be_released_list; 
    erab_list_t     erab_failed_to_release_list;
    U16             counter_after_rrm_resp;
} erab_release_command_data_t;

typedef enum
{
    RRC_NO_MSG,
    RRC_RE_ESTABLISH_MSG,
    RRC_HO_REQUIRED_MSG,
    RRC_RLF_MSG,
/* BUG_11163 start */
    RRC_UE_RECONFIG,
/* BUG_11163 stop */
    RRC_MAX_MSG
} uecc_msg_bfr_type_et;

typedef struct
{
    U8     t; /* Can rename this one!!*/
    uecc_msg_bfr_type_et  msg_bfr_type;
    void*                 msg_bfr_rcvd;

    union
    {
       /* erab_setup_request_data_t    erab_setup_request_data;
        erab_modify_request_data_t  erab_modify_request_data;*/
       /* This will be used for both MME or eNB initiated 
        * E-RAB release procedure */
        erab_release_command_data_t  erab_release_command_data;
    } u;
} uecc_curr_proc_data_t;

/* ERB RELEASE COMMAND END */


/*typedef struct
{
    s1ap_Cause      cause;
    rrc_bool_t      security_mode_command_resp_received;
    rrc_counter_t   create_drb_status_counter;
    rrc_uecc_llim_create_lc_status_t
                    create_drb_status[RRC_UECC_LLIM_MAX_DRB];
    uecc_rm_erab_setup_info_t
                    erab_info_data;
    uecc_erab_failed_to_setup_list_t
                    failed_erab_data;
} uecc_rm_data_t;*/

typedef struct
{
    rrc_counter_t       erab_failed_to_setup_list_counter;
    erab_item_t         erab_failed_to_setup_item[MAX_ERAB_COUNT];
} erab_failed_to_setup_list_t;


typedef struct
{
    struct {
       /* Set of bits which defines if IEs have valid values */
       unsigned physical_config_dedicated_present : 1;
       unsigned sps_config_present : 1;
       unsigned mac_config_present : 1;
       unsigned mac_main_config_present : 1;
       unsigned meas_config_present : 1;
       unsigned proximity_config_present : 1;


    } m;

    /* PhysicalConfigDedicated */
    rrc_phy_physical_config_dedicated_t      physical_config_dedicated;

    /* SPS-Config */
    rrc_sps_config_t            sps_config;

    /* MAC configuration */
    rrm_mac_config_t            mac_config;

    /* Measurement configuration */
    rrm_meas_config_t           meas_config;

    /* Proximity Configuration */
    rrm_report_proximity_config_r9_t       proximity_config;


} uecc_rm_erab_setup_op_data_t;

typedef struct
{
    s1ap_Cause      cause;
    rrc_bool_t      security_mode_complete_received;
    rrc_bool_t      security_mode_failure_received;
    rrc_bool_t      security_configure_ciphering_dl_failure;
    rrc_counter_t   create_drb_status_counter;
    rrc_uecc_llim_create_lc_status_t
                    create_drb_status[RRC_UECC_LLIM_MAX_DRB];
    rrc_bool_t      is_ul_dl_ciphering_configured;

} uecc_rm_data_t;

#define RRC_CONNECTION_RELEASE_NOT_REQUIRED 1


/*typedef struct
{
     Status will indicate either successful ERAB or failure ERAB 
    U8 status;
    U8 erab_id;
    U8 lc_id;
}uecc_rm_erab_info_t;

typedef struct
{
     No of ERABs that will be sent to RRM 
    rrc_counter_t       erab_to_be_setup_list_counter;
    uecc_rm_erab_info_t erab_info[MAX_ERAB_COUNT];
}uecc_rm_erab_setup_info_t;*/
    
typedef struct
{
    union
    {
        s1ap_Cause cause;
    } u;
    rrc_bool_t      release_performed;
    rrc_bool_t      rrc_connection_release_not_required_flag ;
    rrc_bool_t      rrm_rel_req_not_required_flag ;
    rrc_bool_t      reset_towards_mme_required ; 
    rrc_bool_t      rlf_detected;
    rrc_bool_t      ue_context_rel_comp_not_req_flag;
  /*SPR 15610 Fix Start*/
    rrc_bool_t      release_cause_valid;
  /*SPR 15610 Fix Stop*/
/*BUG 604 changes start*/
    rrc_bool_t      s1u_release_not_req_flag;
/*BUG 604 changes stop*/
    /* SPR 16750 Fix Start */
    rrc_bool_t      s1_error_ind_recv;
    /* SPR 16750 Fix Stop */
} uecc_rcr_data_t;

/* Share data for UE FSMs*/
typedef struct
{
    uecc_rm_data_t*  p_uecc_rm_data;
    uecc_rcr_data_t uecc_rcr_data;
} uecc_ue_share_data_t;


/* E-RAB Setup Procedure Start */

typedef struct
{
    U16 status;
    rrm_drb_config_t    drb_config;
}uecc_erab_setup_item_t;


typedef struct
{
    s1ap_E_RAB_ID               e_RAB_ID;
    s1ap_GTP_TEID               gTP_TEID;
    s1ap_TransportLayerAddress  transportLayerAddress;
}uecc_erab_to_be_switched_item_t;

typedef struct
{
    rrc_counter_t       count;
    uecc_erab_to_be_switched_item_t 
        erab_to_be_switched_item[MAX_ERAB_COUNT];    
}uecc_erab_to_be_switched_list_t; 

/*
typedef struct
{
    s1ap_Cause      cause;
    rrc_bool_t      security_mode_command_resp_received;
    rrc_counter_t   create_drb_status_counter;
    rrc_uecc_llim_create_lc_status_t
                    create_drb_status[RRC_UECC_LLIM_MAX_DRB];
    uecc_rm_erab_setup_info_t
                    erab_info_data;
    uecc_erab_failed_to_setup_list_t
                    failed_erab_data;

} uecc_rm_data_t;
*/

typedef struct
{
    rrc_counter_t                 count;
    uecc_erab_setup_item_t        erab_setup_item[MAX_ERAB_COUNT];    
}uecc_erab_setup_list_t; 

typedef struct
{
    /*
    uecc_erab_setup_list_t           erab_setup_list;
    uecc_erab_to_be_setup_list_t     erab_to_be_setup_list;
    uecc_erab_failed_to_setup_list_t erab_failed_to_setup_list;
    uecc_rm_erab_setup_op_data_t     p_erab_op_params;    
    */
    uecc_rm_erab_setup_info_t         erab_info_data;
    uecc_erab_failed_to_setup_list_t  failed_erab_data;
    uecc_rm_erab_setup_op_data_t      erab_op_params;
    erab_release_command_data_t  erab_release_command_data;
    erab_list_t                       erab_release_list;
    U8 is_erab_del_ongoing; /* ARP */

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    cg_config_info_t                  uecc_cg_config_info;
/*NR_DC Code Change Stop*/
#endif
}uecc_erab_setup_request_data_t;

/* E-RAB Modify Procedure */
typedef struct
{
    s1ap_E_RAB_ID   e_RAB_ID;
    s1ap_E_RABLevelQoSParameters e_RABlevelQoSParameters;
    s1ap_NAS_PDU     nAS_PDU;
}erab_to_modify_item_t;


typedef struct
{
    rrc_counter_t              erab_to_be_modified_list_counter;
    erab_to_modify_item_t   erab_to_be_modified_item[MAX_ERAB_COUNT];    
}erab_to_be_modified_list_t; 

typedef struct
{
    U8  status;
    s1ap_NAS_PDU      nAS_PDU; 
    rrm_drb_config_t      drb_config;
}erab_modify_item_t;

typedef struct
{
    rrc_counter_t      erab_modify_list_counter;
    erab_modify_item_t erab_modify_item[MAX_ERAB_COUNT];    
}erab_modify_list_t; 


typedef struct
{
    rrc_counter_t     erab_failed_to_modify_list_counter;
    erab_item_t       erab_failed_to_modify_item[MAX_ERAB_COUNT];    
}erab_failed_to_modify_list_t;

typedef struct
{
    struct {
       /* Set of bits which defines if IEs have valid values */
       unsigned ue_ambr_present : 1;
       unsigned physical_config_dedicated_updated : 1;
       unsigned sps_config_updated : 1;
       unsigned mac_config_updated : 1;
       unsigned meas_config_updated : 1;
       unsigned proximity_config_present : 1;
    } m;

    rrc_s1ap_ue_aggregate_maximum_bitrate_t  ue_aggregate_maximum_bitrate;
    
    erab_modify_list_t            erab_modify_list; /* Can do without this one*/
    erab_to_be_modified_list_t    erab_to_be_modified_list;
    erab_failed_to_modify_list_t  erab_failed_to_modify_list;
    
    /* PhysicalConfigDedicated */
    rrc_phy_physical_config_dedicated_t      physical_config_dedicated;

    /* SPS-Config */
    rrc_sps_config_t            sps_config;

    /* MAC configuration */
    rrm_mac_config_t            mac_config;

    /* Measurement configuration */
    rrm_meas_config_t           meas_config;

    /* Proximity Configuration */
    rrm_report_proximity_config_r9_t       proximity_config;
    /*SPR 22036 Fix Start*/
    U8                           is_erab_del_ongoing; /*Indicate that deletion of RAB 
                                                       * required during RAB modification procedure */
    erab_release_command_data_t  erab_release_command_data;
    /*SPR 22036 Fix Stop*/

}erab_modify_request_data_t;


#ifdef ENDC_ENABLED
typedef struct{
    struct{
        unsigned rrc_x2u_dl_teid_self_updated : 1;
        unsigned rrc_x2u_dl_trasport_addr_self_updated : 1;
        unsigned rrc_x2u_ul_teid_peer_updated : 1;
        unsigned rrc_x2u_ul_trasport_addr_updated: 1;
        unsigned rrc_x2u_sgnb_s1ap_gtp_id_updated: 1;
        unsigned rrc_x2u_sgnb_s1ap_transportLayerAddress_updated: 1;
    }m;

    U8                              erab_id;
    U8                              old_drb_id;
    U8                              new_drb_id;
    U8                              new_lc_id;
    U8                              old_lc_id;
    U8                              dc_bearer_type;
    U32                             qos_id;
    rrm_s1u_qos_profile_t           qos_profile;
    x2ap_GTP_TEI                    ul_x2u_teid_peer;
    x2ap_TransportLayerAddress      ul_x2u_transport_layer_addr;
    rrc_gtp_teid_t                  dl_x2u_teid_self;
    rrc_s1u_ie_gsn_addr_t           dl_x2u_transport_layer_addr_self;
    x2ap_GTP_TEI                    sgnb_s1ap_gtp_id;
    x2ap_TransportLayerAddress      sgnb_s1ap_transportLayerAddress;
    /*Logical_Error*/
    rrc_bool_et                     erab_success;
    /*Logical_Error*/
    rrc_bool_et                     erab_failed;
}rrc_drb_to_mod_item_t;

typedef struct{
    U8                     drb_count;
    rrc_drb_to_mod_item_t  drb_config[MAX_ERAB_COUNT];
}rrc_drb_to_modify_list_t;

typedef struct
{
    /* Bitmask to indicate presence of IEs */
    struct {
        unsigned drb_config_updated : 1;
        unsigned sgnb_add_config_updated : 1;
        unsigned endc_meas_config_updated : 1;
    }m;

    /* List of DRBs to be changed */
    rrc_drb_to_modify_list_t     drb_to_modify_list;
   
    /* SgNB addition configuration */
    rrc_rrm_sgnb_add_param_t     sgnb_add_param;

    /* ENDC meas configuration */
    rrc_rrm_endc_meas_config_t   endc_meas_config;

/* OPTION3X Changes Start */
    /*For failed bearer in sgnb addition*/
    uecc_erab_failed_to_setup_list_t  failed_erab_data;
/* OPTION3X Changes End */
}dc_bearer_change_request_data_t;
#endif

typedef struct
{
	struct {
		/* Set of bits which defines if IEs have valid values */
		unsigned physical_config_dedicated_updated : 1;
		unsigned mac_config_updated : 1;
		unsigned scell_config_updated : 1;
		unsigned fail_cause_updated : 1;
		/* eICIC changes start */
		unsigned meas_subframe_pattern_pcell_updated : 1;
		/* eICIC changes stop */
        /*SPR 20418 fix start*/
        unsigned sps_config_present :1 ;
        /*SPR 20418 fix end*/

	} m;

	/* PhysicalConfigDedicated */
	rrc_phy_physical_config_dedicated_t      physical_config_dedicated;

	/* MAC configuration */
	rrm_mac_config_t            mac_config;

	rrm_scell_config_t          *p_scell_config;
	U8 modify_lc_resp_code;
	U8 fail_cause;
	/* eICIC changes start */
	rrc_meas_subframe_pattern_pcell_r10_t    rrc_meas_subframe_pattern_pcell_r10;
	/* eICIC changes stop */
}rrm_ue_reconfig_request_data_t;


/* E-RAB Release Procedure */
typedef struct
{
    rrc_counter_t   erab_to_be_released_counter;
    erab_item_t     erab_to_be_released_item[MAX_ERAB_COUNT];
} erab_to_be_released_list_t;

typedef struct
{
    rrc_counter_t    erab_release_counter; /* Why is this static??*/
          s1ap_E_RAB_ID    eRAB_ID[MAX_ERAB_COUNT];
} erab_release_list_t;

typedef struct
{
    rrc_counter_t    erab_release_counter;
    erab_item_t      erab_failed_to_release_item[MAX_ERAB_COUNT];
} e_rab_failed_to_release_list_t;

/*typedef struct
{
    struct {
        Set of bits which defines if IEs have valid values 
       unsigned nAS_PDUPresent : 1;
    } m;
    
    s1ap_NAS_PDU      nAS_PDU; 

    erab_release_list_t           e_rab_release_list;
    erab_to_be_released_list_t    e_rab_to_be_released_list;
    erab_to_be_released_list_t    e_rab_failed_to_release_list;
} erab_release_command_data_t;

typedef enum
{
    RRC_RE_ESTABLISH_MSG,
    RRC_HO_REQUIRED_MSG,
        RRC_RLF_MSG,
    RRC_NO_MSG
}uecc_msg_bfr_type;*/

/* Used during ERAB MGMT Procedures */
typedef struct
{
    U8     t; /* Can rename this one!!*/
#define UECC_ICS_PROC                0x01
#define UECC_ERAB_SETUP_PROC         0x02
#define UECC_ERAB_MODIFY_PROC        0x04
#define UECC_ERAB_RELEASE_PROC       0x08
#ifdef ENDC_ENABLED
#define UECC_RRM_DC_BEARER_CHANGE_PROC  0x20
#endif

    uecc_msg_bfr_type_et   msg_bfr_type;
    void                  *msg_bfr_rcvd; 
    struct {
       /* Set of bits which defines if configuration element has valid values */
       unsigned AggMaxBitRate : 1;
       unsigned SpId : 1;
       unsigned CsfallbackIndicator : 1;
       unsigned registered_lai_present : 1;
       unsigned CsgMembershipStatus : 1;
       /*securitykey*/
       unsigned SecurityCapability_present : 1;
       unsigned SecurityKey_present : 1; 
       /*securitykey*/
       /* CSR#59144 fix start */
       unsigned T304_timer_rcvd_frm_rrm : 1; 
       /* CSR#59144 fix stop */
    } m;
    rrc_s1ap_ue_aggregate_maximum_bitrate_t  ue_aggregate_maximum_bitrate;
    /* Subscriber Profile ID */
    s1ap_SubscriberProfileIDforRFP  spid;
    /* Cs fallback Indicator */
    s1ap_CSFallbackIndicator        cs_fallback_indicator;
    registered_lai_t      *p_registered_lai;
    s1ap_CSGMembershipStatus         csg_membership_status;
    /*securitykey*/
    rrc_s1ap_ue_security_capabilities_t    ue_security_capabilities; 
    s1ap_SecurityKey                       security_key;
   /*IT_FIX*/
    U16                             ciphering_algo_id;
    U16                             integrity_algo_id; 
    U16                             userplane_ciphering_algo_id; 
   /*IT_FIX*/
    /*securitykey*/ 
    /* CSR#59144 fix start */
    U8                              t304_requested_from_rrm;
    U16                             t304_timer; 
    /* CSR#59144 fix stop */
    union
    {
        uecc_erab_setup_request_data_t    erab_setup_request_data;
        erab_modify_request_data_t   erab_modify_request_data;
        rrm_ue_reconfig_request_data_t   rrm_ue_reconfig_request_data;
        erab_release_command_data_t  erab_release_command_data;
#ifdef ENDC_ENABLED
        dc_bearer_change_request_data_t     rrm_dc_bearer_change_request_data;
#endif
    } u;
}curr_proc_data_t;

/* End  E-RAB Mgmt Code*/

/*SPR 18758 Fix Start*/
typedef struct
{
    union
    {
        uecc_erab_setup_request_data_t    erab_setup_request_data;
        erab_modify_request_data_t   erab_modify_request_data;
        erab_release_command_data_t  erab_release_command_data;
    } u;
}failed_curr_proc_data_t;
/*SPR 18758 Fix Stop*/


/* HO Related structures */
typedef struct  _uecc_ho_drb_forward_info_t
{
    s1ap_GTP_TEID             teid_self;
    s1ap_TransportLayerAddress transportLayerAddress;
} uecc_ho_drb_forward_info_t;

typedef struct  _uecc_ho_x2ap_drb_forward_info_t
{
    x2ap_GTP_TEI             teid_self;
    x2ap_TransportLayerAddress transportLayerAddress;
} uecc_ho_x2ap_drb_forward_info_t;

typedef struct _short_mac_i_info_list_t
{
    U8  cell_identity[CELL_ID_OCTET_SIZE];
    U16 short_mac_i;
} short_mac_i_info_list_t;

typedef struct
{
    s1ap_E_RAB_ID  e_rab_id;
    /*SPR 15086 Fix Start */
    U8                drb_id;
    /*SPR 15086 Fix Stop */
    s1ap_Cause        cause;
    x2ap_Cause        x2ap_cause;
} uecc_e_rab_failed_to_add_list_item_t;

typedef struct  _uecc_e_rab_failed_to_add_list_t
{ 
    U8                          count;
    uecc_e_rab_failed_to_add_list_item_t      
        e_rab_failed_to_add_list[MAX_ERAB_COUNT];
    U8                          llim_index;
} uecc_e_rab_failed_to_add_list_t;

typedef struct  _uecc_ho_src_drb_forward_info_t
{
    rrm_forwarding_direction_et  forward_direction;
    s1ap_GTP_TEID             dl_teid_peer;
    s1ap_GTP_TEID             dl_teid_self;
    s1ap_TransportLayerAddress dl_transportLayerAddress;
    s1ap_GTP_TEID             ul_teid_peer;
    s1ap_GTP_TEID             ul_teid_self;
    s1ap_TransportLayerAddress ul_transportLayerAddress;
    U8 teid_count;
    rrc_gtp_teid_t              s1u_fwd_teid[RRC_S1U_MAX_TUNNELS_PER_LC];
} uecc_ho_src_drb_forward_info_t;


typedef struct _uecc_ho_drb_forward_list_t
{
    U8                           count;
    uecc_ho_src_drb_forward_info_t   
        *erab_ho_src_drb_forward_list[MAX_ERAB_COUNT];
}uecc_ho_src_drb_forward_list_t;

typedef struct _uecc_inter_rat_ho_info_t
{
    U16                    bitmask;
#define TARGET_TO_SOURCE_TRANSPARNET_CONTAINER_SECONDARY_PRESENT    0x01
    U8                    nas_security_param_from_E_UTRAN_numocts;
    U8*                  p_nas_security_param_from_E_UTRAN_data;
    U32                    target_to_source_transparent_container_numocts;
    U8*                  p_target_to_source_transparent_container_data;
    U32                    target_to_source_transparent_container_secondary_numocts;
    U8*                  p_target_to_source_transparent_container_secondary_data;
}uecc_inter_rat_ho_info_t;

/*BUG 604 changes start*/
/*BUG 604 changes stop*/

typedef struct _uecc_ho_src_gtp_tunnel_end_point_t
{
   x2ap_GTP_TEI                   teid_peer; 
   x2ap_TransportLayerAddress     transportLayerAddress;
}uecc_ho_src_gtp_tunnel_end_point_t;

typedef struct  _uecc_ho_src_drb_admitted_info_t
{
    rrm_forwarding_direction_et   forward_direction;

    uecc_ho_src_gtp_tunnel_end_point_t 
                                  ul_gtp_tunnel_end_point;

    uecc_ho_src_gtp_tunnel_end_point_t 
                                  dl_gtp_tunnel_end_point;
    U8                            teid_count;
    rrc_gtp_teid_t                s1u_fwd_teid[RRC_S1U_MAX_TUNNELS_PER_LC];
} uecc_ho_src_drb_admitted_info_t;


typedef struct _uecc_ho_src_drb_admitted_list_t
{
    U8                           drb_fwd_count;
    U8                           count;
    uecc_ho_src_drb_admitted_info_t   
        *erab_ho_src_drb_admitted_info[MAX_ERAB_COUNT];
}uecc_ho_src_drb_admitted_list_t;

typedef struct
{
    x2ap_E_RAB_ID  e_rab_id;
    x2ap_Cause        cause;
} uecc_e_rab_not_admitted_to_add_list_item_t;

typedef struct  _uecc_e_rab_not_admitted_to_add_list_t
{ 
    U8                          count;
    uecc_e_rab_not_admitted_to_add_list_item_t      
        e_rab_not_admitted_to_add_list[MAX_ERAB_COUNT];
} uecc_e_rab_not_admitted_to_add_list_t;

/* lossy ho changes start */
typedef struct
{
        U16        count;
        U16        erab_id[MAX_ERAB_COUNT];
        U8         lc_id[RRC_UECC_LLIM_MAX_DRB];
}erab_lossy_ho_list_t;
/* lossy ho changes stop */

typedef struct  _uecc_s1_src_ho_info_t
{
    U16     bitmask;
#define ERAB_SUBJECT_TO_DATA_FORWARDING_LIST_PRESENT  0x01  
#define E_RAB_FAILED_LIST_PRESENT                     0x02  
#define X2AP_ERAB_ADMITTED_LIST_PRESENT               0x04  
#define X2AP_ERAB_NOT_ADMITTED_LIST_PRESENT           0x08 
#define ERAB_LOSSY_HO_LIST_PRESENT                    0x10 
    uecc_ho_src_drb_forward_list_t   src_drb_fwd_list;
    uecc_e_rab_failed_to_add_list_t  erab_failed_list;
    rrc_rrm_ho_required_t   *p_rrc_rrm_ho_required;
    U16                      ho_cmd_length;
    void*                   p_ho_command;
    /* Added for inter-rat HO */
    uecc_inter_rat_ho_info_t inter_rat_ho_info;
    /*x2ap-start*/
    uecc_ho_src_drb_admitted_list_t src_erab_admitted_list;
    uecc_e_rab_not_admitted_to_add_list_t  erab_not_admitted_list;
    /*x2ap-end*/
/*BUG 604 changes start*/
    /* Added for intra_cell_ho */
    //uecc_intra_cell_ho_info_t intra_cell_ho_info;
/*BUG 604 changes stop*/
    /* lossy ho changes start */
    erab_lossy_ho_list_t erab_lossy_ho_list;
    /* lossy ho changes stop */
} uecc_s1_src_ho_info_t; 

typedef struct  _uecc_src_ho_info_t
{
    U16 bitmask;
#define  PDCP_MAC_I_PRESENT             0x01  
#define  MAC_UE_INACTIVE_TIME_PRESENT   0x02
#define  PROCESS_MAC_UE_INACTIVE_TIME_RESP   0x04
    rrc_bool_et ho_command_sent_to_ue; /*Trigger sent to UE for HO*/
    rrc_bool_et      is_srb_data_ind_received;
    rrc_bool_et      s1u_reconfigured;
    short_mac_i_info_list_t  target_cell_short_mac_i;
    U8          count_short_mac_i; 
    short_mac_i_info_list_t short_mac_I[RRC_PDCP_MAX_SHORT_MAC_I];
    void *p_re_establishment_req;
    void *p_radio_link_failure_ind;
    U8 is_rrm_ho_cancel_receive;
    void *p_ue_ctx_rel_cmd;
    U16  ue_inactive_time;
    rrc_bool_et is_sn_hfn_req_sent;
    uecc_s1_src_ho_info_t   s1_src_ho_info; 
} uecc_src_ho_info_t;

typedef struct _uecc_ho_drb_forward_info_list_t
{
    rrm_forwarding_direction_et  forward_direction;/*0-DL/1-UL/2-BOTH*/
    uecc_ho_drb_forward_info_t drb_forward_info[MAX_FWD_TUNNELS_PER_ERAB];
    uecc_ho_x2ap_drb_forward_info_t x2ap_drb_forward_info[MAX_FWD_TUNNELS_PER_ERAB];
}uecc_ho_drb_forward_info_list_t;

typedef struct  _uecc_s1_trg_ho_info_t
{
    U16    bitmask;    
#define ERAB_TO_BE_SWITCHED_UL_LIST_PRESENT     0x01
#define ERAB_TO_BE_RELEASED_LIST_PRESENT        0x02
    U8 trg_ho_forward_list_count;
    uecc_ho_drb_forward_info_list_t          
        *p_erab_trg_ho_forward_list[MAX_ERAB_COUNT];
    uecc_e_rab_failed_to_add_list_t    erab_failed_list;
    uecc_erab_to_be_switched_list_t    erab_switched_list;
} uecc_s1_trg_ho_info_t; 

typedef struct  _uecc_trg_ho_info_t
{
    struct {
#define RA_PREAMBLE_INDEX_PRESENT 1
#define RA_PRACH_MASK_INDEX_PRESENT 1
        /* Set of bits which defines if configuration element has valid values */
        unsigned     ra_preamble_index_present: 1;
        unsigned     ra_prach_mask_index_present: 1;
    } m;

    U8 mme_id;
    U8 ra_PRACH_mask_index;
    U8 ra_preamble_index;
    rrc_s1ap_handover_request_t rrc_s1ap_ho_request;
    rrc_x2ap_handover_request_t rrc_x2ap_ho_request;
    rrc_rrm_ue_ho_adm_resp_t    *p_rrc_rrm_ho_adm_resp;
    uecc_s1_trg_ho_info_t    s1_trg_ho_info;
    rrc_bool_et              is_ho_req_ack_sent;
    U8 is_rrc_conn_reconf_comp_recv; 
    U8 is_mme_status_transfer_ind_recv; 
    U8 is_sn_status_transfer_ind_recv; 
    U8 is_path_switch_req_ack_recv;  
    rrm_drb_to_add_info_list_t  drb_info_recv_from_src;
    /* SPR 20747 Fix Start */
    #define SRC_SPS_CONFIG_UL_PRESENT 0x01
    #define SRC_SPS_CONFIG_DL_PRESENT 0x02
    U8  is_sps_recv_from_src;
    /* SPR 20747 Fix End */
} uecc_trg_ho_info_t;

typedef struct  _uecc_ho_info_t
{
    U8     ho_type; /* As specified in 33.413 */
    rrm_ho_status_et ho_status; /* rrm_ho_status_et */
    s1ap_EUTRAN_CGI targetCell_ID;
    x2ap_ECGI       x2ap_targetCell_ID;

    OSCTXT              ho_s1ap_asn1_ctx;
    uecc_src_ho_info_t  *p_src_ho_info;
    uecc_trg_ho_info_t  *p_trg_ho_info;
} uecc_ho_info_t;

typedef struct  _handover_info_t
{ 
    s1ap_Cause  cause; /*handover failure cause*/
    x2ap_Cause  x2ap_cause; /* handover failure x2ap cause */
	/*SPR_17284_START*/
    x2ap_Cause  intra_cell_ho_cause; /*Intra Cell handover cause rec from RRM*/
	/*SPR_17284_END*/
    /*CR 019 changes start*/
    x2ap_Cause  inter_cell_ho_cause; /*Inter Cell handover cause rec from RRM*/
    /*CR 019 changes stop*/
    U8 s1_or_x2_handover; /*S1 or X2 hadover*/
    /* x2ap_drop2_rw_start */
    U8 is_x2_ho_erab_rel_reqd; /* Erabs to be released X2 ho success */
    /* x2ap_drop2_rw_end */
    uecc_ho_info_t *p_ho_info;
    rrc_bool_et cdma_ho_ongoing;
    /* only used in case of conc_ho*/
    rrc_bool_et dl_s1_cdma2000_tunneling_received_once;
    /*BUG 604 changes start*/
    /* Added for intra_cell_ho */
    U16    target_ue_index;
    U16    source_ue_index;
    U8     security_key_present;
    /*BUG 604 changes stop*/
} handover_info_t;
/* Re-establishment Info */
typedef struct _reestab_ue_identity_t
{
    U8      cell_index;
    U16                 short_mac_i;
    U16     crnti;
    U16     pci;
} reestab_ue_identity_t;

/* Trace Start Procedure start */
#define EUTRAN_TRACE_ID 8
#define TRACE_RECORDING_SESSION_REFERENCE 2

#define TRACE_S1AP_INTERFACE 0x80
#define TRACE_X2AP_INTERFACE 0x40
#define TRACE_UU_INTERFACE 0x20
/*SPR 15896 Fix Start*/
#define MINIMUM_TRACE 0x01
#define MEDIUM_TRACE 0x02
#define MAXIMUM_TRACE 0x04
/*SPR 15896 Fix Stop*/

typedef enum
{
     S1=1, 
    X2=2 , 
    UU=4,
   /*SPR 15896 Fix Start*/
    GEN_MME = 10,
    GEN_CELL = 20
   /*SPR 15896 Fix Stop*/
   
} trace_interface_et;


typedef enum
{
     MINIMUM, 
     MEDIUM,
     MAXIMUM,
     MINIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION,
     MEDIUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION,
     MAXIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION
} trace_depth_et;

typedef struct _eutran_trace_id_t
{
    U32   numoctet;
    U8    data[EUTRAN_TRACE_ID];
} eutran_trace_id_t;
typedef struct _cell_eutran_trace_id_t
{
    U32   numoctet;
    /* allocated on receiving cell traffic trace*/
    U8    trace_reference[RRC_TRACE_REFERENCE_LENGTH];
    /* allocated on receiving cell traffic trace*/
    U8    trace_recording_session_reference[TRACE_RECORDING_SESSION_REFERENCE];
} cell_eutran_trace_id_t;

/* At a time only one will be applicable */
typedef union
{
    eutran_trace_id_t eutran_trace_id;      /* received in trace start */
    cell_eutran_trace_id_t cell_eutran_trace_id;
} trace_id_info_t;

typedef struct _trace_collection_entity_info_t
{
    U8    length; /* length of IP address */
    U8    data[MAX_TRANSPORT_LAYER_ADDR_SIZE];
} trace_collection_entity_info_t;

typedef struct _ue_trace_activation_info_t
{
    trace_id_info_t trace_id_info;
    U8  interfaces_to_trace;  /* trace_interface_et */
    trace_depth_et trace_depth;
    trace_collection_entity_info_t trace_collection_entity_info;
    U8      trace_file[MAX_FILE_NAME_SIZE];
    U8*      p_shm;
    /*SPR 15896 Fix Start*/
    FILE *fp ;
    struct timespec rawtime ;
    /*SPR 15896 Fix Stop*/
} ue_trace_activation_info_t;

typedef struct _counter_check_info_t
{
    rrc_bool_t                           counter_check_proc_running;
    drb_countMSB_info_list_t             drb_countMSB_info_list;
    rrc_transaction_id_t                 counter_check_trans_id;
    rrc_transaction_id_t                 ue_counter_check_trans_id;
    rrc_bool_t                           counter_check_req_sent;

}counter_check_info_t;

typedef struct _rrc_uecc_cell_traffic_trace_info_t
{
    U8         interface_to_trace;
    U8         trace_depth;
    trace_collection_entity_info_t  trace_collection_entity;
} rrc_uecc_cell_traffic_trace_info_t;

/* Trace Start Procedure end */
#define RRC_MAX_LATE_CRITICAL_EXT            48

/* cr_392 enums*/
typedef enum {
   RRC_UE_REL_8 = 0,
   RRC_UE_REL_9,
   RRC_UE_REL_10,
   RRC_UE_REL_INVALID
}rrc_ue_access_stratum_release_et;

typedef enum
{
    RRC_MME_REL8=0,
    RRC_MME_REL9,
    RRC_MME_REL10
} rel_info_of_mme_et;

typedef enum
{
    RRM_FAILURE = 0,
    OTHER_FAILURE
}rcs_reject_cause_et;

typedef struct
{
    
  YLNODE               anchor;  
  U8                                            report_config_id;

  report_config_to_add_mod_report_config_t      report_config;
}uecc_report_config_to_add_mod_t;


typedef struct
{
    struct {
      unsigned new_enb_ue_x2ap_id_present : 1;
      /* SPR 16750 Fix Start */
      unsigned error_indication_ongoing : 1;
      /* SPR 16750 Fix Stop */
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
      unsigned sgnb_ue_x2ap_id_present : 1;
/*NR_DC Code Change Stop*/
#endif
    } m;
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    U32                              menb_ue_x2ap_id;
    U8                               is_menb_ext_present;
    x2ap_UE_X2AP_ID_Extension        menb_ue_x2ap_id_ext; 
    U32                              sgnb_ue_x2ap_id;
/*NR_DC Code Change Stop*/
#endif
    U16         old_enb_ue_x2ap_id;
    U16         new_enb_ue_x2ap_id;
    U8          peer_enodeb_id;
    U8          serving_cell_index;
} x2ap_context_t;



typedef struct
{
    OSUINT32  numbits;
    OSOCTET data[32];
} rrc_security_key_t;

typedef struct
{
    rrc_bool_et used_flag;
    rrc_security_key_t nh;
    U8 ncc; 
}nh_ncc_pair_t;
typedef struct _rrc_uecc_erab_data_ics_t
{
    U8 count;
    s1ap_Cause cause[MAX_ERAB_COUNT];
} rrc_uecc_erab_data_ics_t;



typedef struct
{

    U8 t;
#define UECC_MEAS_OBJECT_TO_ADD_EUTRA_PRESENT            0x01
#define UECC_MEAS_OBJECT_TO_ADD_UTRA_PRESENT             0x02
#define UECC_MEAS_OBJECT_TO_ADD_GERAN_PRESENT            0x04
#define UECC_MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT         0x08
#ifdef ENDC_ENABLED
#define UECC_MEAS_OBJECT_TO_ADD_NR_PRESENT         0x20
#endif

    union
    {
        meas_object_eutra_t       meas_object_eutra;
        meas_object_utra_t        meas_object_utra;
        meas_object_geran_t       meas_object_geran;
        meas_object_cdma2000_t    meas_object_cdma2000;
        #ifdef ENDC_ENABLED
        meas_object_nr_t          meas_object_nr;
        #endif
    }u;

}uecc_meas_object_to_add_mod_meas_object_t;




typedef struct
{
  YLNODE               anchor;  

  U8                   meas_object_id; 
  uecc_meas_object_to_add_mod_meas_object_t  meas_object;
}uecc_meas_object_to_add_mod_t;


typedef YLIST   uecc_meas_object_to_add_mod_list_t;
typedef YLIST   uecc_report_config_to_add_mod_list_t;

/*current Meas-config*/
/* SPR 15905 Fix Start */
typedef struct
{
  U8    	meas_id;          
  U8    	meas_object_id;    
  U8    	report_config_id;
  rrc_bool_et   meas_id_deleted;    
}curr_meas_id_to_add_mod_t;

typedef struct
{
  U8                    count;  
  curr_meas_id_to_add_mod_t  meas_id_to_add_mod[MAX_MEAS_ID];

}curr_meas_id_to_add_mod_list_t;
/* SPR 15905 Fix Stop */

typedef struct
{
  struct
  {
      unsigned curr_meas_object_to_add_mod_list_present : 1;
      unsigned curr_meas_report_config_to_add_mod_list_present : 1;
      unsigned curr_meas_id_to_add_mod_list_present : 1;
      unsigned curr_meas_quantity_config_present : 1;
      unsigned curr_meas_gap_config_present : 1;
      unsigned curr_meas_s_measure_present : 1;
      unsigned curr_meas_pre_registration_info_present : 1;
      unsigned curr_meas_speed_state_pars_present : 1;
#ifdef ENDC_ENABLED
      unsigned curr_fr1_gap_r15_present : 1;
      unsigned curr_mgta_r15_present : 1;
#endif
  }m;

   U8                    is_meas_config_valid;
   uecc_meas_object_to_add_mod_list_t     curr_meas_object_to_add_mod_list;

   uecc_report_config_to_add_mod_list_t   curr_report_config_to_add_mod_list;
   /* SPR 15905 Fix Start */
   curr_meas_id_to_add_mod_list_t         curr_meas_id_to_add_mod_list;
   /* SPR 15905 Fix Stop */


   quantity_config_t                 curr_quantity_config;

   meas_gap_config_t                 curr_meas_gap_config;

   U8                                curr_s_measure;

   pre_registration_info_hrpd_t      curr_pre_registration_info_hrpd;

   meas_config_speed_state_pars_t    curr_meas_config_speed_state_pars;

   #ifdef ENDC_ENABLED
   U8                                curr_fr1_gap_r15;

   U8                                curr_mgta_r15;
   #endif
} uecc_ue_curr_meas_config_t;
/* Measurment Re-config END */

/* CDMA2000_CSFB_HO start */
typedef struct
{
    rrc_bool_et     w_f_1xrtt_msg;
    rrc_bool_et     w_f_hrpd_msg;
    ULHandoverPreparationTransfer_r8_IEs ho_prep_transfer_1xrtt;
    ULHandoverPreparationTransfer_r8_IEs ho_prep_transfer_hrpd;
    rrc_s1ap_downlink_s1_cdma2000_tunneling_t 
                        dl_s1_cdma2000_tunneling_1xrtt;
    rrc_s1ap_downlink_s1_cdma2000_tunneling_t 
                        dl_s1_cdma2000_tunneling_hrpd;
    rrc_bool_et    timer_expired_f_1xrtt;
    rrc_bool_et    timer_expired_f_hrpd;
    rrc_bool_et    redirection_present_f_hrpd;
    carrier_freq_cdma2000_t   carrier_freq_cdma2000;

    U32        ulhandoverPrepTransfer_r8_IEs_meid_numbits;
    U8         ulhandoverPrepTransfer_r8_IEs_meid_data[7];
    U8          ulhandoverPrepTransfer_r8_IEs_msi_numocts;
    U8         ulhandoverPrepTransfer_r8_IEs_msi_data[MAX_MSI_SIZE];
    U8         rand_CDMA2000_numbits;
    U8*        rand_CDMA2000_data[4];

}cdma2000_info_t;
/* CDMA2000_CSFB_HO stop */

typedef struct
{
    struct {
        unsigned rrc_reestab_comp_rlf_info_present : 1;
        unsigned rlf_report_present : 1;
        unsigned rrc_conn_setup_reconfigure_rlf_info_present : 1;
    } m;
    U8 ue_info_proc_intrl;
    RLF_Report_r9 rlf_report;
    U8 ue_info_proc_setup_reconfig_rlf;
}x2ap_rlf_proc_t;

typedef YLIST    UE_Radio_Capability_RAT_Container_list_t;

/* Bug 817 Fix Start */
typedef struct
{
    rrc_rb_direction_t            rb_direction;  /* rrc_rb_direction_et */
    rrc_gtp_teid_t                teid_self;
    rrc_uecc_llim_s1u_gsn_addr_t  transport_addr;
}rrc_uecc_fwd_tunnel_info_list_t;


typedef struct
{
    U8                               tunnel_info_count;
    rrc_uecc_fwd_tunnel_info_list_t  uecc_fwd_tnl_list[RRC_S1U_MAX_TUNNELS_PER_LC];
}rrc_uecc_drb_fwd_tunnel_info_list_t;

typedef struct
{
    U8                                   drb_tunnel_info_count;
    /* Index of p_uecc_drb_fwd_tnl_list represents the lc_id */
    rrc_uecc_drb_fwd_tunnel_info_list_t  *p_uecc_drb_fwd_tnl_list[MAX_ERAB_COUNT];
}rrc_uecc_fwd_tunnel_info_t;
/* Bug 817 Fix Stop */

/* SPR 17022 Fix Start */
typedef struct
{ 
    U8 erab_id;
    U8 drb_to_add_item_flag; /* Set when the corresponding ERAB is present in drb_to_add_mod list in erb setup resp from RRM */
}uecc_erab_info_t;

typedef struct 
{
    U8       count;
    uecc_erab_info_t erab_info[MAX_ERAB_COUNT];
}rrc_temp_erb_info_t;
/* SPR 17022 Fix Stop */
/*SPR_18803_START*/
#define FGI_BIT_2 0x40000000
/*SPR_18803_END*/


#ifdef ENDC_ENABLED
/* MENB_CHANGES_START */
#define RRC_UECC_LLIM_ENDC_R15_PRESENT     0x01
typedef struct _RrcEndcConfigInfo_t
{
    rrc_bitmask_t    bitmask;

    U8               enDcR15Supported;
}RrcEndcConfigInfo_t;

typedef enum
{
    UE_REL = 1,
    ERAB_REL,
    INVALID_REL
}rrc_sgnb_release_type_et;
/* MENB_CHANGES_END */
#endif

typedef struct _uecc_ue_context_t
{
    struct {
       /* Set of bits which defines if configuration element has valid values */
       unsigned physical_config_dedicated_present : 1;
       /* eICIC ue reconfig changes start */
       unsigned meas_subframe_pattern_pcell_present : 1;
       /* eICIC ue reconfig changes stop */
       unsigned sps_config_present : 1;
       unsigned mac_config_present : 1;
       unsigned mac_main_config_present : 1;
       unsigned meas_config_present : 1;
       unsigned srb1_config_present : 1;
       unsigned srb2_config_present : 1;
       unsigned mme_ue_id_present : 1;
       unsigned redirected_carrier_info_present : 1;
       unsigned idle_mode_mobility_control_present : 1;

       /* Set of bits which defines if configuration element has been updated
        * and new configuration should be applied on eNB LL and UE*/
       unsigned physical_config_dedicated_updated : 1;
       /* eICIC ue reconfig changes start */
       unsigned meas_subframe_pattern_pcell_updated : 1;
       /* eICIC ue reconfig changes stop */
       unsigned sps_config_updated : 1;
       /* SPS Start */
       unsigned is_sps_recv_frm_rrm : 1;
       unsigned is_sps_setup_or_rel_recv : 1; /* 0 - Setup and 1 - Release */
       /* SPS Stop */
       unsigned mac_config_updated : 1;
       unsigned meas_config_updated : 1;
       unsigned srb1_config_updated : 1;
       unsigned srb2_config_updated : 1;

       /* ERAB SETUP START */
       unsigned ue_agg_max_bit_rate_updated : 1;

       /* ERAB SETUP STOP */
       unsigned srb1_created: 1;
       unsigned srb2_created: 1;
       unsigned measObjectEutraPresent : 1;
       /* UE Context Modifcation */
       unsigned CsfallbackIndicator: 1;
       unsigned SpId: 1;
       unsigned SrvccOP:1;
       unsigned CsgMembershipStatus:1;
       unsigned AggMaxBitRate: 1;
       /* Trace Start Procedure start */
       unsigned traceActivated:2;
       /* Trace Start Procedure end */
    
       /* connection release v890 IEs*/
       unsigned connection_release_v890_ies : 1;
       unsigned e_redirection_utra_r9_present : 1;

       /* Release of MME */
       unsigned rel_of_mme_present : 1;
       unsigned registered_lai_present : 1;
       unsigned dtm_r9_present:1;
       unsigned rlf_timers_and_constants_r9_present:1;
       /* cr_311 bitmask added */
       unsigned access_stratum_release: 1;
       /* HO Restriction List */
       unsigned ho_restriction_list_present : 1;

       /* Location Reporting Information */
       unsigned location_reporting_info_present : 1;

       unsigned proximity_config_present : 1;
	/*SPR 13469 Fix Start*/
	unsigned full_config_present :1;
	/*SPR 13469 Fix Stop*/
       /*CR 026 changes start*/
       /* Determines whether failure cause is present in
        * rrc_rrm_ue_admission_resp*/
       unsigned rrc_conn_estab_rrm_fail_cause_present : 1;
       /*CR 026 changes start*/
       unsigned extended_wait_time_present :1;
      /*36413_CR0798*/
       unsigned mme_ue_id2_present : 1;
       unsigned source_gummei_present : 1;
       unsigned gw_context_release_indication_present :1;
      /*36413_CR0798*/
       unsigned scell_config_present:1;
       unsigned scell_add_fail_at_lower_layer:1;
       unsigned rel10_ue_category_present:1;
/* SPR 14984 Fix Start */
       /* Default SRB configuration */
       unsigned default_srb1_rlc_cfg_present : 1;
       unsigned default_srb1_lc_cfg_present : 1;
       unsigned default_srb2_rlc_cfg_present : 1;
       unsigned default_srb2_lc_cfg_present : 1;
/* SPR 14984 Fix Stop */
       /* SPR 17391 Fix Start */
       unsigned rrc_connection_count_updated : 1;
       /* SPR 17391 Fix Stop */
       /*SPR_17976_START*/
       /* Set to RRC_TRUE on receiving re-establishment
        * request when re-establishment procedure is 
        * already ongoing for this UE.
        */
       unsigned reestb_during_reestb: 1;
       /*SPR_17976_END*/
       /*SPR_19066_START*/
       unsigned ue_inactive_time_updated: 1;
       /*SPR_19066_END*/
       unsigned  erb_setup_resp_fail_cause_present: 1;
       /*SPR 23209 FIXED START */
       unsigned mocn_operator_id_already_configured: 1;
       /*SPR 23209 FIXED END */
#ifdef ENDC_ENABLED
       /* MENB_CHANGES_START */
       /* 
        * Is scg_config_response_nr received message 
        * RRCConnectionReconfiguration Complete
        */
       unsigned scg_config_response_nr_present : 1;
       unsigned rrcEndcConfigInfoPresent: 1;
       unsigned sgnbCausePresent: 1;
       unsigned ext_ue_agg_max_bit_rate_updated : 1;
	   unsigned secondary_rat_data_usage_report_list_updated : 1;
       /* MENB_CHANGES_END */
#endif
    } m;

    /* Current value of UECC Transaction id which will be used
     * for generation of current operation Transaction id value */
    rrc_transaction_id_t            transaction_id;

    /* Current operation Transaction id value */
    rrc_transaction_id_t            op_transaction_id;
    /* Measurment Re-config START */
    
    /* Meas Transaction id value */
    rrc_transaction_id_t            meas_transaction_id;

    rrc_bool_et      is_meas_transaction_id_valid;
    /* variable to collect the transaction id for sending the same id in final
     * meas_config response */
    U16              meas_proc_trans_id ;

    /* variable to collect the transaction id for sending the same id in final
     * response */
    U16 ue_info_transaction_id;
    /*SPR_17893_START*/
    rrc_transaction_id_t            ue_capability_transaction_id;
    /*SPR_17893_END*/

    /*SPR 14499 Changes Start */
    /* Save the current transaction ID */
    U16 rrm_curr_trans_id;
    /*SPR 14499 Changes Stop */

    /* Measurment Re-config END */
    /* UE index allocated by UECC */
    rrc_ue_index_t                  ue_index;

    /* UE rnti from rrc_uecc_llim_ccch_data_ind_t */
    rrc_rnti_t                      crnti;
    rrc_rnti_t                      old_crnti;


    /* CLPC Start */
    U16           timing_advance;
    U8            sinr;

    /* CLPC Stop */

    /* TS 3GPP TS 36.413 V8.5.1
     *      - 8.6.2.1 Initial UE Message
     *      - 9.2.3.4 eNB UE S1AP ID  */
    U32                             s1ap_ue_id;

    /* TS 3GPP TS 36.413 V8.5.1 - 9.2.3.3   MME UE S1AP ID */
    U32                             mme_ue_id;


    /* PhysicalConfigDedicated */
    rrc_phy_physical_config_dedicated_t      physical_config_dedicated;

    /* eICIC changes start */
    rrc_meas_subframe_pattern_pcell_r10_t    rrc_meas_subframe_pattern_pcell_r10;
    /* eICIC changes stop */
    /* SPS-Config */
    /* SPS Start */
    rrc_sps_config_t            *p_sps_config;
    /* SPS Stop */

    /* MAC configuration */
    rrm_mac_config_t            mac_config;

    /* Measurement configuration */
    rrm_meas_config_t*          p_meas_config;

    /* Proximity Configuration */
    rrm_report_proximity_config_r9_t       proximity_config;

    /* cr_220 RLF TIMERS AND CONSTANTS */
    rlf_timers_and_constants_r9_t *p_rlf_timers_and_constants_r9;

    /* Measurment Re-config START */
    /* A new current Measurment configuration*/
    uecc_ue_curr_meas_config_t      curr_meas_config;
    /* Measurment Re-config END */

    /* SRB1 configuration */
    rrm_srb_config_t            srb1_config;

    /* SRB2 configuration */
    rrm_srb_config_t            srb2_config;

    /* selectedPLMN_Identity IE*/
    U8                          selected_plmn_identity;

    /* PLMN_Identity*/
    plmn_identity_info_t        plmn_identity;

    /* Registered MME info */
    U8                          registered_mme_info_present;
    RegisteredMME               registered_mme;

    /* redirected_carrier_info */
    rrm_redirected_carrier_info_t* p_redirected_carrier_info;

    /* idle_mode_mobility_control */
    rrm_idle_mode_mobility_control_info_t* p_idle_mode_mobility_control;
    
    rrm_rrc_connection_release_v890_ies_t    rrc_connection_release_v890_ies;

    /* data received from s1ap_initial_context_setup_request */
    rrc_s1ap_ue_aggregate_maximum_bitrate_t  ue_aggregate_maximum_bitrate;
    rrc_s1ap_ue_security_capabilities_t      ue_security_capabilities;
    s1ap_SecurityKey                 security_key;
    UE_Radio_Capability_RAT_Container_list_t ue_radio_capability;
    /* indicates UE capability enquiry is requested by RRM */
    rrc_bool_et                      rrm_ue_capability_enq_ind;
    U8                          rrm_ue_capability_enq_list[MAX_RAT_CAPABILITY];
    U8                                  rrm_ue_capability_enq_count;
    /*SPR_17893_START*/
     
    rrc_bool_et                 rrm_ue_capability_enq_flag_after_attach;
    /*SPR_17893_END*/

    uecc_drb_ctx_t                   *p_e_rab_list[MAX_ERAB_COUNT];
    U8                               e_rab_list_count;
    
    /* Single E-RAB ID TO BE USED IN ICS */
    U8                              e_rab_id_ics;

    /* Security configuration */
    rrc_pdcp_config_int_t       configure_integrity_protection; /*KRRCint*/
    rrc_pdcp_config_srb_ciph_t      configure_ciphering;            /*KRRCenc*/
    rrc_pdcp_config_drb_ciph_t      configure_ciphering_user_plane; /*KUPenc*/

    /* UE share data for current operation */
    uecc_ue_share_data_t            share_data;

    /* ASN context for s1ap IEs that are stored in UE context */
    OSCTXT                          s1ap_asn1_ctx;

    /* It'll be sent in:
     *   3GPP TS 36331-850: 6.2.2   RRCConnectionReject data - waitTime field */
    U8                              wait_time;
    
    U16                             extended_wait_time;

    /* 3GPP TS 36331-850: 6.2.2 - RRCConnectionRequest data */
    /* - EstablishmentCause */
    U32                             rcs_establishmentCause;
    U32                             reestablishmentCause;
    /* - InitialUE-Identity */
    rrc_initial_ue_identity_t       rcs_stmsi;

    /* UE FSM Procedural timers */
    uecc_ue_timer_data_t            timers_data;

    /* Data for UECC FSM failure processing */
    uecc_fsm_failure_descriptor_t   failure_descriptor;

    /* cell_index */
    rrc_cell_index_t        cell_index;

    /* Pointer to global context */
    struct _uecc_gb_context_t*      p_gb_context;

    /* Pointer to UECC FSM */
    uecc_fsm_main_t                 uecc_fsm;
    
    /* Pointer to previous state of UECC FSM */
    uecc_fsm_main_t                 prev_ongoing_uecc_fsm;

    /* Statistic for current UE context */
    rrc_ue_status_t                 ue_status;

    U16 subscriber_profiler_id;

    U8 ncc; 
    nh_ncc_pair_t nh_ncc_pair;
   

    /* The mme id of the MME with which UE's S1AP connection
       exists, Used by TRG HO FSM */
    U8 mme_id;

    /* The release of MME with which UE's S1AP connection
       exists, Used by ICSP FSM */
    rel_info_of_mme_et  rel_of_mme;

    /* Used to store HO related info , contains both
       SRC and TRG HO related info */
    handover_info_t    ho_info;
    /* Cs fallback Indicator */
    s1ap_CSFallbackIndicator        cs_fallback_indicator;
    
    /* Subscriber Profile ID */
    s1ap_SubscriberProfileIDforRFP  spid;

    /* SRVCC Possible Operation */
    s1ap_SRVCCOperationPossible      srvcc_operation_possible;

    s1ap_CSGMembershipStatus         csg_membership_status;
    curr_proc_data_t*               p_curr_proc_data;
    /*SPR_18241_START*/
    curr_proc_data_t*               p_temp_ue_ctx_mod_proc_data;
    /*SPR_18241_END*/
/*SPR 18758 Fix Start*/
    failed_curr_proc_data_t*        p_failed_curr_proc_data;
/*SPR 18758 Fix Stop*/

    /* ICS handover restriction list */
    s1ap_HandoverRestrictionList     ics_handover_restriction_list;
    /* Added for inter- rat HO */
    U32                             nas_security_eutra_numocts;
    U8                              nas_security_eutra[MAX_NAS_SECURITY_SIZE];

    /* cr_592 For Target HO request */
    U32                             trg_ho_req_csg_id_numbits;
    U8                              trg_ho_req_csg_id_data[CSG_ID_OCTET_SIZE];
    U32                             trg_ho_req_csg_membership_status; 
    U8                              trg_ho_csg_id_fail_flag;
    U8                              ue_category; /* UE CATEGORY In Radio Capability */
/* CR 503 changes */
    U8                              rel10_ue_category;/* Release 10 UE CATEGORY in Radio Cappability */
/* CR 503 changes */
/* HD-FDD start */
    U8                              ue_mode;
/* HD-FDD stop */
    /* 36321_CR0538 start */
    U8                              access_stratum_release;
    /* 36321_CR0538 stop */

    /* Delete UE Context */
    struct {
       unsigned is_s1ap_resp_expected : 1;
       unsigned is_rrc_resp_expected : 1;
    }ue_ctx_del_status;
    /* Pointer to Re-establishment Ue Identity */
    reestab_ue_identity_t*          p_reestab_ue_identity;
    U8                              trg_ho_fail_rrc_cre_flag;

    /* Trace Start Procedure start */
    ue_trace_activation_info_t*     p_ue_trace_activation_info;
    /*SPR 15896 Fix Start*/
    ue_trace_activation_info_t*     p_ue_cell_trace_activation_info;
    /*SPR 15896 Fix Stop*/
    /* Trace Start Procedure end */
    
    s1ap_gummei_t    gummei; 
    x2ap_context_t  x2ap_context;
    U32             location_reporting_event_type; 
    U32             location_reporting_report_area; 
    
 
    /* cr_761 lai structure added */
    registered_lai_t      *p_registered_lai;

    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/

    rrc_uecc_ue_statistic_t ue_stats;

    rrc_uecc_erab_data_ics_t erab_ics_data;

    /* SPR 13614 Fix Start */
    /* Used only in the case when Multiple ERABs are requested in ICS request with the same ERAB ID */ 
    rrc_uecc_erab_data_ics_t erab_ics_fail_data;
    /* SPR 13614 Fix Stop */

    /* SPR:7369 start */
    rrc_bool_et           erb_modify_running;
    /* SPR:7369 stop */
    U8 x2ap_reset_recvd_flag ;
    U8 x2ap_rlf_ind_recvd_flag;
    x2ap_rlf_ind_info_t x2ap_rlf_ind;
    U8                  *p_rlf_report_buff;
    U8                  ho_report_flag;
    x2ap_cause_t        ho_report_cause;
    x2ap_rlf_proc_t     rlf_proc_flags;
    rcs_reject_cause_et rcs_reject_cause;

    /* SPR 4118 Start */
    U8  integrity_failure_start_flag;
    /*SPR_17143_START*/
    /*Code Removed*/
    /*SPR_17143_END*/

    /* SPR 4118 Stop */
    s1ap_NAS_PDU        nas_pdu;
    
    /* CDMA2000_CSFB_HO start */
    cdma2000_info_t     cdma2000_info;
    /* CDMA2000_CSFB_HO stop */

    counter_check_info_t counter_check_info;
    /* indicates UE Information Request is requested by RRM */
    rrc_bool_et          ue_info_proc_running;
    
    list_t               ue_reconfig_queue;
    rrc_uecc_failtype_action_et fail_action_type; /*rrc_uecc_failtype_action_et */

    /* Bug 817 Fix Start */
    rrc_uecc_fwd_tunnel_info_t     rrc_trg_ho_fwd_tnl_info;
    /* Bug 817 Fix Stop */

    /*36413_CR0798*/
    U32                             mme_ue_id2;
    /*36413_CR0798*/

    /*CR 026 changes start*/
    U8                  rrc_conn_estab_rrm_fail_cause;
    /* CR 026 changes stop*/
    /* CR 101 changes start*/
    /* Cause of ue_ctx_rel_req send for updation of release active erabs counter*/
    s1ap_Cause          ue_ctx_rel_req_cause;
    /* CR 101 changes stop*/
    /* securitykey */
    /* Ue context Modification flag for security */
    U8                  ue_ctx_mod_security_flag;
    U8                  ue_ctx_mod_intra_ho_success_flag;
    /* securitykey */
    /* CELL_TRACE_fix */
    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/
    /* CELL_TRACE_fix */
    /*CA_START*/
    rrm_scell_config_t          *p_scell_config;
    /* SPR 16149 Fix Start */
    U8 is_adm_cnf_sent;
    /* SPR 16149 Fix Stop */

    scells_operation_list_t scells_operation_list[MAX_NUM_CELLS];
    /*CA_STOP*/
    /*Scell configuration provided by RRM (used only during HO)*/
    rrm_scell_config_t          *p_rrm_scell_config;   
/*CA_PHASE3_INTER_CELL_MRO_START*/
    /*flag for ho report in case of too early inter cell handover*/
    U8   ho_report_internal_flag; 
    /*flag for rlf ind incase of too late inter cell handover*/
    U8   too_late_inter_cell_ho_flg;
    /*CA_PHASE3_INTER_CELL_MRO_STOP*/
    /*CA_RE_ESTAB_START*/
    U8   reestab_lower_layer_scell_config_ongoing;
    /*CA_RE_ESTAB_STOP*/
    /* mme grp id feature changes start */
    U8 gummei_to_be_sent;
    /* mme grp id feature changes stop */
    /*SPR_16416_START*/
    /* UL NAS Transport Event queue */
    uecc_event_queue_t p_ul_nas_transport_queue; 
    /*SPR_16416_END*/
    /*SPR_17976_START*/
    /* Stores event received during re-establishment
     * at TeNB
     */
    uecc_event_queue_t p_tenb_cre_event_queue; 
    /*SPR_17976_END*/
    /*SPR_17121_START*/
    U32 mac_i;
    /*SPR_17121_END*/

    /* SPR 17022 Fix Start */
    rrc_temp_erb_info_t		erab_info_temp;
    /* SPR 17022 Fix Stop */

    /*SPR_19066_START*/
    U32     ue_inactive_time;
    /*SPR_19066_END*/
    /*SPR_18803_START*/
    U32                   feature_group_indicators;
    /*SPR_18803_END*/
    /*spr_22003_changes_start*/
    rrc_bool_et     is_erab_relSuccNbr_qci_counter_pegging_reqd;
    /*spr_22003_changes_end*/
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    U16                                  sk_counter_r15;
    rrc_bool_et                          is_sgnb_add_reqd;
    rrc_sn_cg_config_t                   *p_sn_cg_config;
    rrm_ue_agg_max_bit_rate_t            ue_agg_max_bit_rate_sgnb;
    OSDynOctStr                          scg_config_response_nr;
/*NR_DC Code Change Stop*/
    /*EN-DC_changes_start*/
    x2ap_Cause                           sgnb_cause;
    rrc_bool_et                          partial_sgnb_rab_release; /* true: Not all the rabs of SGNB are released */
    U8                                   rel_proc_fail;
    U8                                   x2apUecontextReleased;
    U8                                   nr_capab_req_flag;
	rrc_s1ap_SecRATDataReport_list_t     secondary_rat_data_usage_report_list;
    /*EN-DC_changes_end*/
    /* MENB_CHANGES_START */
    RrcEndcConfigInfo_t                  endcConfigInfo;
    rrc_bool_et                          sgnbInitiatedSgnbReleaseFlag;
    erab_list_t                          endc_erab_release_list;
    erab_list_t                          endc_sgnb_add_erab_list;
    U8                                   release_type;
    /* rrc_sgnb_release_type_et */
    U8                                   msg_type;
    /* msg_type_et */
    /* MENB_CHANGES_END */
#endif
} uecc_ue_context_t;

/****************************************************************************
 * Public functions definitions
 ****************************************************************************/

rrc_return_t uecc_ue_ctx_db_init(
   struct _uecc_gb_context_t*  p_uecc_gb_context  /* UECC global context */
);

void uecc_ue_ctx_db_free(
   struct _uecc_gb_context_t*  p_uecc_gb_context   /* UECC global context */
);

rrc_ue_index_t uecc_ue_ctx_allocate(
    struct _uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    rrc_module_id_t     uecc_module_id     /* Module Id for UECC Instance */
);

void uecc_ue_ctx_init(
    struct _uecc_gb_context_t*  p_uecc_gb_context, /* UECC global context */
    rrc_ue_index_t              ue_index,           /* UE index */
    rrc_rnti_t                  rnti,               /* UE rnti */
    rrc_cell_index_t            cell_index          /* Cell Index */
);

void uecc_ue_ctx_free(
    struct _uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    rrc_ue_index_t ue_index  /* UE index  */
);

uecc_ue_context_t* uecc_ue_ctx_get(
    struct _uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    rrc_ue_index_t ue_index  /* UE index  */
);

uecc_ue_context_t* uecc_ue_ctx_get_by_crnti(
    struct _uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    rrc_rnti_t    crnti  /* crnti in RRCConnectionReestablishmentRequest */
);
uecc_ue_context_t* uecc_ue_ctx_get_by_s1ap_id(
    struct _uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    U32                 s1ap_id                     /* S1AP id  */
);

uecc_ue_context_t* uecc_ue_ctx_get_by_mme_s1ap_id(
    struct _uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    U32                 s1ap_id             /* S1AP id */
);


rrc_transaction_id_t uecc_ue_ctx_get_next_transction_id(
    uecc_ue_context_t*  p_ue_context       /* UECC UE context */
);

void uecc_ue_ctx_set_op_transction_id(
    uecc_ue_context_t       *p_ue_context, /* UECC UE context */
    rrc_transaction_id_t    value
);

rrc_transaction_id_t uecc_ue_ctx_get_op_transction_id(
    uecc_ue_context_t*  p_ue_context       /* UECC UE context */
);


void uecc_ue_ctx_set_meas_transction_id(
    uecc_ue_context_t       *p_ue_context,       /* UECC UE context */
    rrc_transaction_id_t    value
);

rrc_transaction_id_t uecc_ue_ctx_get_meas_transction_id(
    uecc_ue_context_t*  p_ue_context       /* UECC UE context */
);

U32 uecc_ue_ctx_get_mme_ue_id(
    uecc_ue_context_t*  p_ue_context       /* UECC UE context */
);


rrc_bool_t uecc_ue_ctx_look_for_transport_addr(
    uecc_ue_context_t*  p_ue_context,       /* UECC UE context */
    U8                  ip_address_octet_size,
    U8                  *ip_address
);

void uecc_fsm_free_trg_ho_context(
    uecc_ue_context_t*  p_uecc_ue_context
);

U16 uecc_x2ap_ueid_allocate(
    struct _uecc_gb_context_t*  p_uecc_gb_context,  
    rrc_ue_index_t      ue_idx 
);

void uecc_x2ap_id_free(
    struct _uecc_gb_context_t*  p_uecc_gb_context,
    U16    x2ap_ueid                     
);
void uecc_ue_ctx_set_counter_check_transction_id(
    uecc_ue_context_t       *p_ue_context,       /* UECC UE context */
    rrc_transaction_id_t    value
);
rrc_transaction_id_t uecc_ue_ctx_get_counter_check_transction_id(
    uecc_ue_context_t*  p_ue_context       /* UECC UE context */
);

void stop_trace_and_send_file_transfer_start_ind(
        struct _uecc_gb_context_t*  p_uecc_gb_context,
        U16 ue_id);

/*SPR_16416_START*/
void uecc_delete_ul_nas_transport_queue
(
    uecc_ue_context_t*      p_uecc_ue_context,
    uecc_event_queue_t*     p_uecc_event_queue  /* Pointer to UECC queue */
);
/*SPR_16416_END*/
/*SPR_17976_START*/
void rrc_delete_ho_ue_identity_node(
    uecc_ue_context_t*  p_uecc_ue_context
);
/*19078 +*/
rrc_return_t rrc_copy_transportlayer_address(
        s1ap_TransportLayerAddress *p_trg,        /* Target */
        s1ap_TransportLayerAddress *p_src,        /* Source */
        uecc_ue_context_t          *p_ue_context /* UECC UE context */
        );      
/*19078-*/
/*SPR_17976_END*/
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
void fill_sgnb_release_cause(x2ap_Cause *p_x2apCause ,s1ap_Cause *p_s1apCause);
void fill_sgnb_cause_for_scgFailureInformationNR_failure_type(
    uecc_ue_context_t*  p_ue_context,
    UInt32              *p_scg_failure_type
);
#define MAX_RADIO_NETWORK_CAUSE 40
#define MAX_MISC_CAUSE 6
/*NR_DC Code Change Stop*/
#endif
#endif /* UECC_UE_CTX_H */

