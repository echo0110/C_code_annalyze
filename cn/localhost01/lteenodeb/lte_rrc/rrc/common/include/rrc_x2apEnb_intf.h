/*******************************************************************************
*
*  FILE NAME   : x2ap_enb_intf.h
*
*  DESCRIPTION : This file includes API definitions for X2 interface
*
*  DATE 22-July-2010 NAME Sudhanshu Kumar Srivastav 
*                    REFERENCE 36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _X2AP_ENB_INFT_H_ 
#define _X2AP_ENB_INFT_H_

#include "rrc_defines.h"
#include "x2ap_asn_enc_dec_3gpp.h"
#include "rrc_asn_enc_dec.h"
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
#include "rrc_asn_enc_dec_nr.h"
/*NR_DC Code Change End*/
#endif

#include "rrc_rrm_intf.h"

typedef struct
{
    U16                        num_srvd_cell;                  /*^ M,0,B,1,256 ^*/
    x2ap_srvd_cell_arry_t      srvd_cell_arr[MAX_SERVED_CELLS];/*^ M,0,N,0,0 ^*/
}x2ap_srvd_cells_t;

/* X2 API: X2 SETUP REQUEST */
#define X2_SETUP_REQ_NR_NEIGHBOUR_INFO_PRESENT      0x01
typedef struct
{
    //rrc_bitmask_t             bitmask;
    x2_gb_enb_id_t            global_enb_id;
    U16                       num_served_cell;
    x2ap_srvd_cell_arry_t     served_cell_arr[MAX_SERVED_CELLS];
    U8                        num_gu_group_id;
    x2ap_gu_group_id_t        group_id_arr[MAX_ENB_POOLS];
    //U16                       num_neigh_cell;
    //x2ap_nr_neigh_cell_arry_t nr_neigh_cell_arr[MAX_SERVED_CELLS];
}x2_setup_req_t;

#define    MAX_NR_OF_ERRORS      256

typedef enum {
    x2ap_INITIATING_MESSAGE = 0, 
    x2ap_SUCCESSFUL_OUTCOME, 
    x2ap_UNSUCCESSFUL_OUTCOME
}x2ap_trigg_msg_en;

typedef enum {
    REJECT = 0,
    IGNORE_, 
    NOTIFY
}x2ap_proc_critical_en;

typedef enum {
    NOT_UNDERSTOOD = 0, 
    MISSING
}x2ap_type_of_err_en;

typedef struct {
    x2ap_proc_critical_en    ie_ctiticality;
    U32                      ie_id;
    x2ap_type_of_err_en      type_of_error;
}x2ap_critic_diagno_info_t;

typedef struct {
    U32                           procedure_code;
    x2ap_trigg_msg_en             trigger_msg;
    x2ap_proc_critical_en         proc_critic;
    U16                           num_critic_diagno;
    x2ap_critic_diagno_info_t     critic_daigno_info[MAX_NR_OF_ERRORS];
}x2ap_crit_diagno_t;

/* X2 API: X2 SETUP RESPONSE */
typedef struct{
    x2_gb_enb_id_t                global_enb_id;
    U16                           num_served_cell;
    x2ap_srvd_cell_arry_t         served_cell_arr[MAX_SERVED_CELLS];
    U8                            num_gu_group_id;
    x2ap_gu_group_id_t            group_id_arr[MAX_ENB_POOLS];
    x2ap_crit_diagno_t            critc_diagno;
}x2_setup_resp_t;

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */

#define    X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT    0x01
#define    X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT 0x02
typedef struct
{
    U8                                       bitmask;
    /*^ BITMASK ^*/

    U16                                      nr_pci;
    /*^ M, 0, H, 0, NR_MAX_PCI ^*/

    x2ap_nr_ecgi_t                           nr_cell_id;
    /*^ M, 0, N, 0, 0 ^*/

    x2ap_broadcast_plmn_t                    broadcast_plmn_info;
    /*^ M, 0, N, 0, 0 ^*/

    x2ap_choice_nr_mode_info_t               choice_nr_mode;
    /*^ M, 0, N, 0, 0 ^*/

#ifndef LINUX_PC_TEST
/* ENDC_MEAS_CHANGES_START */
    nr_rrc_MeasurementTimingConfiguration    meas_timing_config;
    /*^ M, 0, N, 0, 0 ^*/
/* ENDC_MEAS_CHANGES_END */
#endif

    x2ap_five_gs_tac_t                       fiveGS_track_area_code;
    /*^ O, X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT, N, 0, 0 ^*/

    x2ap_tac_t                               configured_tac;
    /*^ O, X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT, N, 0, 0 ^*/

#ifdef LINUX_PC_TEST
    U32                              meas_timing_config_buf_len;
    /*^ M, 0, H, 0, MAX_ASN_BUFFER ^*/

    U8                               measurement_timing_config_asn_buff[MAX_ASN_BUFFER];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
#endif

}x2ap_srvd_nr_cell_info_t;

#define NR_NEIGHBOUR_INFO_PRESENT 0x01
typedef struct
{
    rrc_bitmask_t               bitmask;

    x2_gb_gnb_id_t              global_gnb_id;
    /*^ M, 0, N, 0, 0 ^*/

    U16                         num_served_nr_cell;
    /*^ M, 0, B, 1, MAX_SERVED_CELLS ^*/

    x2ap_srvd_nr_cell_info_t    served_nr_cell_arr[MAX_SERVED_CELLS]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                         num_neigh_cell;
    /*^ M, 0, B, 1, MAX_SERVED_CELLS ^*/

    x2ap_nr_neigh_cell_arry_t   nr_neigh_cell_arr[MAX_SERVED_CELLS];
    /*^ O, NR_NEIGHBOUR_INFO_PRESENT, N, 0, 0 ^*/
  
}x2ap_nr_cell_info_t;

#define X2_SETUP_NR_NEIGHBOUR_INFO_PRESENT 0x01
typedef struct
{
    rrc_bitmask_t                 bitmask;

    x2_gb_enb_id_t                global_enb_id;

    U16                           num_served_cell;

    x2ap_srvd_cell_arry_t         served_cell_arr[MAX_SERVED_CELLS];

    U16                           num_neigh_cell;

    x2ap_nr_neigh_cell_arry_t     nr_neigh_cell_arr[MAX_SERVED_CELLS];
    /*^ O, X2_SETUP_NR_NEIGHBOUR_INFO_PRESENT , N, 0, 0 ^*/

}x2ap_eutra_cell_info_t;

#define X2AP_ENDC_X2_SETUP_RESP_EUTRA_CELL_INFO_PRESENT    0x01
#define X2AP_ENDC_X2_SETUP_RESP_NR_CELL_INFO_PRESENT       0x02
typedef struct
{
    rrc_bitmask_t             bitmask;

    x2ap_eutra_cell_info_t    eutra_cell_info;
    /*^ O, X2AP_EUTRA_CELL_INFO_PRESENT, N, 0, 0 ^*/

    x2ap_nr_cell_info_t       nr_cell_info;
    /*^ O, X2AP_NR_CELL_INFO_PRESENT, N, 0, 0 ^*/

}en_dc_x2_setup_resp_t;

#define X2AP_ENDC_X2_SETUP_REQ_EUTRA_CELL_INFO_PRESENT    0x01
#define X2AP_ENDC_X2_SETUP_REQ_NR_CELL_INFO_PRESENT       0x02
typedef struct
{
    rrc_bitmask_t             bitmask;

    x2ap_eutra_cell_info_t    eutra_cell_info;
    /*^ O, X2AP_EUTRA_CELL_INFO_PRESENT, N, 0, 0 ^*/

    x2ap_nr_cell_info_t       nr_cell_info;
    /*^ O, X2AP_NR_CELL_INFO_PRESENT, N, 0, 0 ^*/

}en_dc_x2_setup_req_t;
/* MENB CHANGES - END */
#endif

#define RADIO_NW_LAYER_CAUSE    1
#define TRANSPORT_LAYER_CAUSE   2
#define PROTOCOL_CAUSE          3
#define MISC_CAUSE              4

#define MAX_STATUS_LENGTH       4096
#define MAX_NR_OF_ERRORS        256
#define MAX_NO_BEARERS          256
#define MAX_NO_OF_CELLS         16

typedef union {
    U32       radio_nw_cause;
    U32       transport_cause;
    U32       protocol_cause;
    U32       misc_cause;
}cause_info_ut;

typedef struct {
    U32    cause_type;
    cause_info_ut cause_info;
}x2_cause_t;

/* X2 API: X2 SETUP FAILURE */
typedef enum {
    SEC_1 = 0,
    SEC_2,
    SEC_5,
    SEC_10,
    SEC_20,
    SEC_60
}x2ap_time_to_wait_et;

typedef struct {
    /* SPR 10858 Fix Start */
#define X2_SETUP_FAIL_TIME_TO_WAIT_PRESENT      0x01
#define X2_SETUP_FAIL_CRITICAL_DIAGNO_PRESENT   0x02
    U16 bitmask;
    /* SPR 10858 Fix Stop */
    x2_cause_t                 cause;
    x2ap_crit_diagno_t         critc_diagno;
    x2ap_time_to_wait_et  time_to_wait;
}x2_setup_fail_t;

typedef struct
{
    x2ap_ecgi_t             old_ecgi;
    x2ap_srvd_cell_arry_t   served_cell; 
}x2ap_srvd_cells_to_mod_info_t;

typedef struct
{
    U16                              num_srvd_cell_to_mod;
    x2ap_srvd_cells_to_mod_info_t    srvd_cells_to_mod_info[MAX_SERVED_CELLS];   
}x2ap_srvd_cells_to_mod_t;

typedef struct
{
    U16                      num_srvd_cell_to_del;
    x2ap_ecgi_t              old_ecgi[MAX_SERVED_CELLS];
}x2ap_srvd_cells_to_del_t;

/* X2 API: ENB CONFIGURATION UPDATE */

typedef struct
{
	U16 bitmask;
    x2ap_srvd_cells_t         srvd_cells_to_add;
    x2ap_srvd_cells_to_mod_t  srvd_cells_to_modify;
    x2ap_srvd_cells_to_del_t  srvd_cells_to_delete;
    x2ap_gu_grp_ids_t         gu_grp_id_to_add;
    x2ap_gu_grp_ids_t         gu_grp_id_to_delete;
}x2ap_enb_conf_req_t;


/* X2 API: ENB CONFIGURATION UPDATE ACKNOWLEDGE */
typedef struct
{
#define X2AP_CRITICALITY_DIAGNOS_PRESENT 0x01
    U16 bitmask;
    x2ap_crit_diagno_t   criticality_diagnostics;
}x2ap_enb_conf_ack_t;

typedef struct
{
//#define X2AP_TIME_TO_WAIT_INDEX_PRESENT 0x01
#define X2AP_CRITICALITY_DIAGNOSTICS_PRESENT 0x02
	U16 bitmask;
	x2_cause_t            cause;
    x2ap_time_to_wait_et  time_to_wait;
    x2ap_crit_diagno_t    criticality_diagnostics;
}x2ap_enb_conf_upd_fail_t;

/* X2 API: HANDOVER PREPARATION FAILURE */
typedef struct
{
    rrc_bitmask_t           bitmask;
#define X2AP_HANDOVER_PREP_FAIL_CRIT_DIAGNO_PRESENT      0x01
    x2ap_UE_X2AP_ID         x2ap_Old_UE_X2AP_ID;
    x2ap_Cause              cause;
    x2ap_CriticalityDiagnostics criticality_diagnostics;
}rrc_x2ap_handover_prep_fail_t;

/* X2 API: HANDOVER CANCEL */
typedef struct
{
    rrc_bitmask_t             bitmask;
#define X2AP_HANDOVER_CANCEL_NEW_UE_X2AP_ID_PRESENT      0x01    
    x2ap_UE_X2AP_ID           x2ap_Old_UE_X2AP_ID;/*^M,0,H,0,0^*/
    x2ap_UE_X2AP_ID           x2ap_New_UE_X2AP_ID;
    x2ap_Cause                cause;
}rrc_x2ap_handover_cancel_t;

/* X2 API: UE CONTEXT RELEASE */
typedef struct
{
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    rrc_bitmask_t                       bitmask;    /*^ BITMASK ^*/
#define X2AP_UE_CONTEXT_RELEASE_NEW_UE_X2AP_ID_EXT_PRESENT  0x01
#define X2AP_UE_CONTEXT_RELEASE_SGNB_UE_X2AP_ID_PRESENT     0x04
/*NR_DC Code Change Stop*/
#endif
    x2ap_UE_X2AP_ID           x2ap_Old_UE_X2AP_ID;
    x2ap_UE_X2AP_ID           x2ap_New_UE_X2AP_ID;
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    x2ap_UE_X2AP_ID_Extension x2ap_New_UE_X2AP_ID_ext;
    x2ap_SgNB_UE_X2AP_ID      x2ap_SgNB_UE_X2AP_ID; /*X2AP_UE_CONTEXT_RELEASE_SGNB_UE_X2AP_ID_PRESENT*/
/*NR_DC Code Change Stop*/
#endif
}rrc_x2ap_ue_context_release_t;

/* X2 API: SN STATUS TRANSFER */
typedef struct
{
    x2ap_UE_X2AP_ID                   x2ap_Old_UE_X2AP_ID;
    x2ap_UE_X2AP_ID                   x2ap_New_UE_X2AP_ID; 
    x2ap_E_RABs_SubjectToStatusTransfer_List
                     erab_subject_to_status_transfer_list;
}rrc_x2ap_sn_status_transfer_t;


typedef struct
{
    HandoverCommand ho_command;
    x2ap_TargeteNBtoSource_eNBTransparentContainer
        target_to_source_transparent_container;
}target_to_source_enb_transparent_container_t;

/* X2 API: HANDOVER REQUEST ACKNOWLEDGE */
typedef struct
{
    rrc_bitmask_t                     bitmask;
#define X2AP_HANDOVER_REQ_ACK_ERAB_NOT_ADMITTED_LIST_PRESENT  0x01   
#define X2AP_HANDOVER_REQ_ACK_CRIT_DIAGNO_PRESENT  0x02    
    U16                               no_of_bearers;
    x2ap_UE_X2AP_ID                   old_ue_x2ap_id;
    x2ap_UE_X2AP_ID                   new_ue_x2ap_id;
    x2ap_E_RABs_Admitted_List         e_rab_admitted_list;
    x2ap_E_RAB_List                   e_rab_not_admitted_list;
    target_to_source_enb_transparent_container_t 
                    target_to_src_enb_transparent_cont;
    x2ap_CriticalityDiagnostics criticality_diagnostics;
}rrc_x2ap_handover_req_ack_t;    

typedef struct
{
    HandoverPreparationInformation_r8_IEs rRC_Container; 
    x2ap_UE_ContextInformation        ue_context_info;
} rrc_x2ap_UE_ContextInformation; 

typedef struct
{
    rrc_bitmask_t                     bitmask;
#define X2AP_HANDOVER_REQ_TRACE_ACTIVATION_PRESENT  0x01  
#define X2AP_HANDOVER_REQ_SRVCC_OPERATION_POSSIBLE_PRESENT  0x02   
/*36423_CR0407_start*/ 
#define X2AP_HANDOVER_REQ_CSG_MEMBERSHIP_STATUS_PRESENT  0x04    
/*36423_CR0407_stop*/
    x2ap_UE_X2AP_ID                   x2ap_Old_UE_X2AP_ID;
    x2ap_Cause                        cause;
    x2ap_ECGI                         target_cell_id;
    x2ap_GUMMEI                       gummei;
    rrc_x2ap_UE_ContextInformation    rrc_x2ap_ue_ctx_info;
    x2ap_UE_HistoryInformation        ue_history_info;
    x2ap_TraceActivation              trace_activation;
    x2ap_SRVCCOperationPossible       srv_operation_possible;
    /*36423_CR0407_start*/
    x2ap_CSGMembershipStatus          csg_membership_status;
    /*36423_CR0407_stop*/
} rrc_x2ap_handover_request_t;

/* X2 API : RESOURCE STATUS REQUEST */

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
typedef struct
{
    rrc_bitmask_t                   bitmask;    /*^ BITMASK ^*/
#define X2AP_SGNB_ADD_REQ_SELECTED_PLMN_PRESENT             0x01
#define X2AP_SGNB_ADD_REQ_HO_RESTRICITION_LIST_PRESENT      0x02
#define X2AP_SGNB_ADD_REQ_SGNB_UE_X2AP_ID_PRESENT           0x04
#define X2AP_SGNB_ADD_REQ_EXPECTED_UE_BEHAVIOUR_PRESENT     0x08
#define X2AP_SGNB_ADD_REQ_MENB_UE_X2AP_ID_EXTENSION_PRESENT 0x10
#define X2AP_SGNB_ADD_REQ_MCG_SPLIT_SRBs_PRESENT            0x20
#define X2AP_SGNB_ADD_REQ_MENB_RESOURCE_COORDINATION_INFO_PRESENT   0x40
#define X2AP_SGNB_ADD_REQ_SGNB_ADDITION_TRIGGER_IND_PRESENT         0x80
#define X2AP_SGNB_ADD_REQ_SUBSCRIBER_PROFILE_ID_FOR_RFP_PRESENT     0x100

    x2ap_UE_X2AP_ID                 menb_ue_x2ap_id;                /*^ M,0,N,0,0 ^*/
    x2ap_NRUESecurityCapabilities   nr_ue_security_capability;      /*^ M,0,N,0,0 ^*/
    x2ap_SgNBSecurityKey            sgnb_security_key;              /*^ M,0,N,0,0 ^*/
    x2ap_UEAggregateMaximumBitRate  ue_aggregate_max_bitrate;       /*^ M,0,N,0,0 ^*/

    x2ap_PLMN_Identity              plmn_identity;                  /*^ O,X2AP_SGNB_ADD_REQ_SERVING_PLMN_PRESENT,N,0,0 ^*/
    x2ap_HandoverRestrictionList    ho_restricition_list;           /*^ O,X2AP_SGNB_ADD_REQ_HO_RESTRICITION_LIST_PRESENT,N,0,0 ^*/
    x2ap_E_RABs_ToBeAdded_SgNBAddReqList
                                    erab_to_add_list;               /*^ M,0,N,0,0 ^*/
    x2ap_MeNBtoSgNBContainer        menb_to_sgnb_container;         /*^ M,0,N,0,0 ^*/
    x2ap_SgNB_UE_X2AP_ID            sgnb_ue_x2ap_id;                /*^ O,X2AP_SGNB_ADD_REQ_SGNB_UE_X2AP_ID_PRESENT,N,0,0 ^*/
    x2ap_ExpectedUEBehaviour        expected_ue_behavior;           /*^ O,X2AP_SGNB_ADD_REQ_EXPECTED_UE_BEHAVIOUR_PRESENT,N,0,0 ^*/
    x2ap_UE_X2AP_ID_Extension       menb_ue_x2ap_id_ext;            /*^ O,X2AP_SGNB_ADD_REQ_MENB_UE_X2AP_ID_EXTENSION_PRESENT,N,0,0 ^*/
    x2ap_SplitSRBs                  split_SRBs;                     /*^ O,X2AP_SGNB_ADD_REQ_MCG_SPLIT_SRBs_PRESENT,N,0,0 ^*/
    x2ap_MeNBResourceCoordinationInformation
                                    menb_resource_coordination_info;/*^ O,X2AP_SGNB_ADD_REQ_MENB_RESOURCE_COORDINATION_INFO_PRESENT,N,0,0 ^*/
    x2ap_SGNB_Addition_Trigger_Ind  sgnb_addition_trigger_ind;      /*^ O,X2AP_SGNB_ADD_REQ_SGNB_ADDITION_TRIGGER_IND_PRESENT,N,0,0 ^*/
    x2ap_SubscriberProfileIDforRFP  subscriber_profile_id_for_rfp;  /*^ O,X2AP_SGNB_ADD_REQ_SUBSCRIBER_PROFILE_ID_FOR_RFP_PRESENT,N,0,0 ^*/
    x2ap_ECGI                       menb_cell_id;                   /*^ M,0,N,0,0 ^*/
}rrc_x2ap_sgnb_addition_request_t;



typedef struct _rrc_sn_cg_config_t
{
    rrc_bitmask_t       bitmask;                                /*^ BITMASK ^*/
#define X2AP_SN_CG_CONFIG_SCG_CELLGROUP_CONFIG_PRESENT              0x01
#define X2AP_SN_CG_CONFIG_SCG_RB_CONFIG_PRESENT                     0x02
    OSDynOctStr         scg_CellGroupConfig;
    OSDynOctStr         scg_RB_Config;
} rrc_sn_cg_config_t;

typedef struct 
{
    rrc_bitmask_t                   bitmask;                                /*^ BITMASK ^*/
#define X2AP_SGNB_ADD_REQ_ACK_ERABS_NOT_ADMITTED_LIST_PRESENT                   0x01
#define X2AP_SGNB_ADD_REQ_ACK_CRITICALITY_DIAGNOSTICS_PRESENT                   0x02
#define X2AP_SGNB_ADD_REQ_ACK_MENB_UE_X2AP_ID_EXT_PRESENT                       0x04
#define X2AP_SGNB_ADD_REQ_ACK_ADMITTED_SPLIT_SRBS_PRESENT                       0x08
#define X2AP_SGNB_ADD_REQ_ACK_SGNB_RESOURCE_COORDINATION_INFO_PRESENT           0x10
#define X2AP_SGNB_ADD_REQ_ACK_RRC_CONFIG_INDICATION_PRESENT                     0x20

    x2ap_UE_X2AP_ID                 menb_ue_x2ap_id;                        /*^ M,0,N,0,0 ^*/
    x2ap_SgNB_UE_X2AP_ID            sgnb_ue_x2ap_id;                        /*^ M,0,N,0,0 ^*/
    x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAckList
                                    erabs_admittied_list;                   /*^ M,0,N,0,0 ^*/
    x2ap_E_RAB_List                 erabs_not_admitted_list;                /*^ 0,X2AP_SGNB_ADD_REQ_ACK_ERABS_NOT_ADMITTED_LIST_PRESENT,N,0,0 ^*/
    nr_rrc_CG_Config                sgnb_to_menb_container;                 /*^ M,0,N,0,0 ^*/ 
    x2ap_CriticalityDiagnostics     criticality_diagnostics;                /*^ 0,X2AP_SGNB_ADD_REQ_ACK_CRITICALITY_DIAGNOSTICS_PRESENT,N,0,0 ^*/
    x2ap_UE_X2AP_ID_Extension       menb_ue_x2ap_id_ext;                    /*^ 0,X2AP_SGNB_ADD_REQ_ACK_MENB_UE_X2AP_ID_EXT_PRESENT,N,0,0 ^*/
    x2ap_SplitSRBs                  admitted_split_srbs;                    /*^ 0,X2AP_SGNB_ADD_REQ_ACK_ADMITTED_SPLIT_SRBS_PRESENT,N,0,0 ^*/
    x2ap_SgNBResourceCoordinationInformation
                                    sgnb_resource_coordination_info;        /*^ 0,X2AP_SGNB_ADD_REQ_ACK_SGNB_RESOURCE_COORDINATION_INFO_PRESENT,N,0,0 ^*/
    x2ap_RRC_Config_Ind             rrc_config_indication;                  /*^ 0,X2AP_SGNB_ADD_REQ_ACK_RRC_CONFIG_INDICATION_PRESENT,N,0,0  ^*/
} rrc_x2ap_sgnb_addition_req_ack_t;

typedef struct
{
    x2ap_UE_X2AP_ID                 menb_ue_x2ap_id;                        /*^ M,0,N,0,0 ^*/
    x2ap_SgNB_UE_X2AP_ID            sgnb_ue_x2ap_id;                        /*^ M,0,N,0,0 ^*/
    x2ap_Cause                  cause;
}rrc_x2ap_sgnb_addition_req_rej_t;

typedef struct
{
    rrc_bitmask_t                   bitmask;                                /*^ BITMASK ^*/
#define X2AP_SGNB_RECONFIG_COMPLETE_MENB_UE_X2AP_ID_EXT_PRESENT                       0x01

    x2ap_UE_X2AP_ID                 menb_ue_x2ap_id;                        /*^ M,0,N,0,0 ^*/
    x2ap_SgNB_UE_X2AP_ID            sgnb_ue_x2ap_id;                        /*^ M,0,N,0,0 ^*/
    x2ap_ResponseInformationSgNBReconfComp
                                    resp_info_sgnb_reconfig_comp;           /*^ M,0,N,0,0 ^*/
    x2ap_UE_X2AP_ID_Extension       menb_ue_x2ap_id_ext;                    /*^ 0,X2AP_SGNB_RECONFIG_COMPLETE_MENB_UE_X2AP_ID_EXT_PRESENT,N,0,0 ^*/
} rrc_x2ap_sgnb_reconfig_complete_t;

typedef struct 
{
#define X2AP_SECONDARY_RAT_DATA_USAGE_REP_MENB_UE_X2AP_ID_EXT_PRESENT                   0x01
    rrc_bitmask_t                   bitmask;                                /*^ BITMASK ^*/

    x2ap_UE_X2AP_ID                 menb_ue_x2ap_id;                        /*^ M,0,N,0,0 ^*/
    x2ap_SgNB_UE_X2AP_ID            sgnb_ue_x2ap_id;                        /*^ M,0,N,0,0 ^*/
    x2ap_SecondaryRATUsageReportList secondary_rat_usage_rep_list;  /*^ M,0,N,0,0 ^*/
    x2ap_UE_X2AP_ID_Extension       menb_ue_x2ap_id_ext;                    /*^ 0,X2AP_SECONDARY_RAT_DATA_USAGE_REP_MENB_UE_X2AP_ID_EXT_PRESENT,N,0,0 ^*/
} rrc_x2ap_secondary_rat_data_usage_report_t;

/*NR_DC Code Change End*/
#endif

typedef enum
{
   X2AP_START = 0, 
   X2AP_STOP 
}x2ap_rsu_registration_request_en;

/*RSU Periodicity*/
typedef enum
{
    X2AP_RSU_PERIOD_1000_MS,    /*  1000 milliseconds */
    X2AP_RSU_PERIOD_2000_MS,    /*  2000 milliseconds */
    X2AP_RSU_PERIOD_5000_MS,    /*  5000 milliseconds */
    X2AP_RSU_PERIOD_10000_MS    /* 10000 milliseconds */
}x2ap_rsu_periodicity_en;

/*X2AP API : RESOURCE STATUS REQUEST(START/STOP) */

typedef struct _cell_to_report_item_t
{
    rrm_cell_global_id_eutra_t     cell_info; 
}cell_to_report_item_t;


typedef struct _cell_to_report_t
{
 U16                    num_served_cell;
 cell_to_report_item_t    cell_id[MAX_SERVED_CELLS];

}cell_to_report_t;

typedef struct _rrc_x2ap_rsu_start_req_t
{
    rrc_bitmask_t               bitmask;
#define X2AP_RSU_START_ENB2_MEAS_ID_PRESENT      0x01
#define X2AP_PARTIAL_SUCCESS_IND_PRESENT         0X02
    x2ap_Measurement_ID              eNB1_measurement_Id;
    x2ap_Measurement_ID              eNB2_measurement_Id;
    x2ap_Registration_Request        registration_reqd;
    U32                              reportcharacteristics_bitmask;
    cell_to_report_t                cell_to_report_id;    
    x2ap_ReportingPeriodicity        rsu_periodicity;
    x2ap_PartialSuccessIndicator     partial_success_indicator;

}rrc_x2ap_rsu_start_req_t;

/*X2AP API : RESOURCE STATUS FAILURE */

typedef struct
{
    rrc_bitmask_t               bitmask;
#define X2AP_RSU_FAILURE_CRIT_DIAGNO_PRESENT      0x01
#define X2AP_RSU_COMP_FAILURE_INF_LIST            0x02
    x2ap_Measurement_ID         eNB1_measurement_Id;
    x2ap_Measurement_ID         eNB2_measurement_Id;
    x2ap_Cause                  cause;
    x2ap_CriticalityDiagnostics criticality_diagnostics;
    x2ap_CompleteFailureCauseInformation_List complete_failure_cause_inf_list;

}rrc_x2ap_rsu_failure_t;

/* X2AP API : RESOURCE STATUS RESPONSE */
typedef struct 
{
   rrc_bitmask_t              bitmask;
#define X2AP_RSU_RESPONSE_CRIT_DIAGNO_PRESENT      0x01
#define X2AP_RSU_RESPONSE_MSRMNT_INIT_RESULT       0x02
   x2ap_Measurement_ID         eNB1_measurement_Id;
   x2ap_Measurement_ID         eNB2_measurement_Id;
   x2ap_CriticalityDiagnostics criticality_diagnostics;
   x2ap_MeasurementInitiationResult_List measurement_initiation_result_list;

}rrc_x2ap_rsu_response_t;

/* X2AP API : RESOURCE STATUS UPDATE */

typedef struct
{
 x2ap_Measurement_ID    eNB1_measurement_Id;
 x2ap_Measurement_ID    eNB2_measurement_Id;
 x2ap_CellMeasurementResult_List cell_meas_result_list;
}rrc_x2ap_rsu_update_t;

/*x2ap reset start*/
typedef struct
{
    x2ap_Cause                  cause;
}rrc_x2ap_reset_req_t;

typedef struct
{
    rrc_bitmask_t              bitmask;
    #define X2AP_RESET_RESPONSE_CRIT_DIAGNO_PRESENT      0x01
    x2ap_CriticalityDiagnostics criticality_diagnostics;

}rrc_x2ap_reset_response_t;

/*x2ap reset end*/


typedef struct
{
    rrc_bitmask_t              bitmask;
#define RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT       0x01
#define RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT       0x02
#define RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT               0x04
#define RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT  0x08
/*Error_Handling_changes*/
#ifdef ENDC_ENABLED
#define RRC_X2AP_ERROR_IND_OLD_SGNB_UE_X2AP_ID_PRESENT      0x10
#endif
/*Error_Handling_changes*/
    x2ap_UE_X2AP_ID             old_enb_ue_x2ap_id;
    x2ap_UE_X2AP_ID             new_enb_ue_x2ap_id;
    x2ap_Cause                  cause;
    x2ap_CriticalityDiagnostics criticality_diagnostics;
/*Error_Handling_changes*/
#ifdef ENDC_ENABLED
    x2ap_UE_X2AP_ID             old_sgnb_ue_x2ap_id;
#endif
/*Error_Handling_changes*/
}rrc_x2ap_error_indication_t;

typedef struct
{
    rrc_bitmask_t               bitmask;
    #define RRC_X2AP_CELL_ACT_RESPONSE_CRIT_DIAGNO_PRESENT      0x01
    x2ap_ActivatedCellList      activated_cell_list;
    x2ap_CriticalityDiagnostics criticality_diagnostics;
}rrc_x2ap_cell_activation_response_t;

typedef struct
{
    rrc_bitmask_t               bitmask;
    #define RRC_X2AP_CELL_ACT_FAILURE_CRIT_DIAGNO_PRESENT      0x01
    x2ap_Cause                  cause;
    x2ap_CriticalityDiagnostics criticality_diagnostics;
}rrc_x2ap_cell_activation_failure_t;

typedef struct
{
    rrc_bitmask_t              bitmask;
#define RRC_X2AP_ENB1_MOBILITY_PARAMETER_PRESENT       0x01
    x2ap_ECGI    enb1_cell_id;
    x2ap_ECGI    enb2_cell_id;
    x2ap_MobilityParametersInformation  enb1_mobility_param;
    x2ap_MobilityParametersInformation  enb2_proposed_mob_param;
    x2ap_Cause   cause;
}rrc_x2ap_mobility_change_req_t;

typedef struct
{
    rrc_bitmask_t              bitmask;
#define RRC_X2AP_CRITICALITY_DIAGNOSTICS_PRESENT       0x01
    x2ap_ECGI    enb1_cell_id;
    x2ap_ECGI    enb2_cell_id;
    x2ap_CriticalityDiagnostics criticality_diagnostics;
}rrc_x2ap_mobility_change_ack_t;

typedef struct
{
    rrc_bitmask_t              bitmask;
#define RRC_X2AP_MOB_FAIL_CRITICALITY_DIAGNOSTICS_PRESENT     0x01
#define RRC_X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT             0x02
    x2ap_ECGI    enb1_cell_id;
    x2ap_ECGI    enb2_cell_id;
    x2ap_Cause   cause;
    x2ap_MobilityParametersModificationRange  mob_param_mod_range;
    x2ap_CriticalityDiagnostics criticality_diagnostics;
}rrc_x2ap_mobility_change_fail_t;

#ifdef ENDC_ENABLED
/*EN-DC_changes_start*/
typedef struct {
    rrc_bitmask_t                       bitmask;    /*^ BITMASK ^*/
/*NR_DC Code Change Start*/
#define X2AP_SGNB_REL_REQ_SGNB_UE_X2AP_ID_PRESENT                      0x01
#define X2AP_SGNB_REL_REQ_ERB_REL_LIST_PRESENT                      0x02
/*NR_DC Code Change Stop*/
    x2ap_UE_X2AP_ID                     menb_ue_x2ap_id;
    x2ap_SgNB_UE_X2AP_ID                sgnb_ue_x2ap_id; /*X2AP_SGNB_REL_REQ_SGNB_UE_X2AP_ID_PRESENT*/
    x2ap_Cause                          cause;
    x2ap_E_RABs_ToBeReleased_SgNBRelReqList erab_to_release_list;
}rrc_x2ap_sgnb_release_request_t;

#define RRC_X2AP_SGNB_REL_ACK_CRITICAL_DIAGNOSTIC_PRESENT        0x01
#define RRC_X2AP_SGNB_REL_ACK_MENB_UE_X2AP_ID_EXTN_PRESENT       0x02
typedef struct {
    rrc_bitmask_t                       bitmask;    /*^ BITMASK ^*/
    x2ap_UE_X2AP_ID                     menb_ue_x2ap_id;
    x2ap_SgNB_UE_X2AP_ID                sgnb_ue_x2ap_id;
    x2ap_CriticalityDiagnostics         criticality_diagnostics;
    x2ap_UE_X2AP_ID_Extension           menb_ue_x2ap_id_ext;
}rrc_x2ap_sgnb_release_request_ack_t;

typedef struct {
    x2ap_UE_X2AP_ID                     menb_ue_x2ap_id;
    x2ap_SgNB_UE_X2AP_ID                sgnb_ue_x2ap_id;
    x2ap_Cause                          cause;
}rrc_x2ap_sgnb_release_required_t;

typedef struct {
    rrc_bitmask_t              bitmask;
#define RRC_X2AP_SGNB_REL_ERAB_RELEASE_CNF_LIST_PRESENT       0x01
#define RRC_X2AP_SGNB_REL_CRITICAL_DIAGNOSTIC_PRESENT         0x02
    x2ap_UE_X2AP_ID                     menb_ue_x2ap_id;
    x2ap_SgNB_UE_X2AP_ID                sgnb_ue_x2ap_id; 
    x2ap_E_RABs_ToBeReleased_SgNBRelConfList         E_RABs_ToBeReleased_SgNBRelConfList;
    x2ap_CriticalityDiagnostics        CriticalityDiagnostics;
}rrc_x2ap_sgnb_release_confirm_t;

typedef enum {
    X2AP_DEACTIVATED
}x2ap_deact_et;

typedef struct
{
	rrc_bitmask_t              bitmask;
    #define X2AP_ENDC_X2_NR_DEACT_IND_PRESENT    0x01
	
	x2ap_nr_ecgi_t                      old_nr_cgi;
	U8     			                deact_ind;         /*^ O, X2AP_ENDC_X2_NR_DEACT_IND_PRESENT,N,0,0,x2ap_deact_et ^*/
	x2ap_srvd_nr_cell_info_t	srvd_nr_cell_info;
}x2ap_nr_cell_info_to_modify_t;

typedef struct
{
    UInt16            num_delete_nrcells_endc;
    x2ap_nr_ecgi_t    eutra_nr_cell_delete[MAX_SERVED_CELLS];
}x2ap_nr_srvd_cell_to_delete_t; 

typedef struct
{
	UInt16                      	num_nr_srvd_cell_modify;			/* 0..MAX_SERVED_CELLS */
	x2ap_nr_cell_info_to_modify_t   nr_cell_infos[MAX_SERVED_CELLS];
}x2ap_nr_srvd_cell_to_modify_t; 

typedef struct
{
    UInt16         num_eutra_cell_to_delete;
    x2ap_ecgi_t    eutra_cells_delete[MAX_SERVED_CELLS];
}x2ap_eutra_served_cells_to_delete_t; 

typedef struct
{
    UInt16						num_ecgi;
    x2ap_ecgi_t              	            ecgis[MAX_SERVED_CELLS];
    x2ap_eutra_cell_info_t		eutra_served_cell_info;
}x2ap_eutra_served_cells_to_modify_t;

typedef enum {
   X2AP_ALL_SERVED_NR_CELLS
}complete_info_req_ind_et;

typedef struct
{
    complete_info_req_ind_et complete_info_req_ind; 
}x2ap_full_list_t;

#define X2AP_CELL_ASSISTANCE_LIMITED_LIST_PRESENT 0x01
#define X2AP_CELL_ASSISTANCE_FULL_LIST_PRESENT    0x02
typedef struct
{
    rrc_bitmask_t               bitmask;
    x2ap_partial_list_t         partial_nr_cells;
    x2ap_full_list_t            full_nr_cells; 
}x2ap_cell_assistance_info_t;

#define X2AP_ENDC_CONFIGUPDATE_ENB_CELLASSISTANCE_INFO_PRESENT           0x01
#define X2AP_ENDC_CONFIGUPDATE_ENB_SERVEDCELL_MANAGEMENTLIST_PRESENT     0x02
#define X2AP_ENDC_CONFIGUPDATE_ENB_SERVEDCELL_MODIFYLIST_PRESENT         0x04
#define X2AP_ENDC_CONFIGUPDATE_ENB_SERVEDCELL_DELETELIST_PRESENT         0x08
typedef struct
{
    rrc_bitmask_t              			bitmask;
    x2_gb_enb_id_t                      global_enb_id;
    x2_gb_gnb_id_t                      global_en_gnb_id;
    x2ap_eutra_cell_info_t        		eutra_served_cell_management;    
    x2ap_cell_assistance_info_t			cell_assistance_info;
    x2ap_eutra_served_cells_to_modify_t eutra_served_cell_modify;
    x2ap_eutra_served_cells_to_delete_t eutra_served_cell_delete;

}x2_config_update_endc_enb_t;

#define X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MANAGEMENTLIST_PRESENT     0x01
#define X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MODIFYLIST_PRESENT         0x02
#define X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_DELETELIST_PRESENT         0x04
typedef struct
{
    U16              		               bitmask;
    x2_gb_enb_id_t				global_enb_id;
    x2_gb_gnb_id_t				global_en_gnb_id;
    x2ap_nr_cell_info_t		        eutra_nr_cell_management;
    x2ap_nr_srvd_cell_to_modify_t   eutra_nr_cell_modify;
    x2ap_nr_srvd_cell_to_delete_t	eutra_nr_cell_delete;
}x2_config_update_endc_en_gnb_t;

#define X2AP_ENDC_X2_CONFIG_UPDATE_INIT_ENB_PRESENT     0x01
#define X2AP_ENDC_X2_CONFIG_UPDATE_INIT_EN_GNB_PRESENT  0x02      
typedef struct
{
    rrc_bitmask_t                   bitmask;
    x2_config_update_endc_enb_t     eNB;        /*^ O, X2AP_ENDC_X2_CONFIG_UPDATE_INIT_ENB_PRESENT,N,0,0 ^*/
    x2_config_update_endc_en_gnb_t       en_gNB;     /*^ O, X2AP_ENDC_X2_CONFIG_UPDATE_INIT_EN_GNB_PRESENT,N,0,0 ^*/ 
}x2ap_endc_config_update_en_gnb_req_t;

typedef struct
{
#define X2AP_ENDC_CONFIG_UPDATE_RESPOND_EN_GNB_PRESENT  0x01
#define X2AP_ENDC_CONFIG_UPDATE_RESPOND_ENB_PRESENT  0x02
#define X2AP_ENDC_CONFIG_UPDATE_RESPOND_CRITICALITY_DIAGNOS_PRESENT 0x04
    rrc_bitmask_t               bitmask;
    
    x2ap_nr_cell_info_t     respond_en_gnb;
    x2ap_crit_diagno_t      criticality_diagnostics;
}x2ap_endc_config_update_ack_t;

typedef struct 
{
#define X2AP_TIME_TO_WAIT_INDEX_PRESENT 0x01
#define X2AP_ENDC_CONFIG_UPDATE_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT     0x02
    rrc_bitmask_t           bitmask;                /*^ BITMASK ^*/

    x2_cause_t              cause;                  /*^ M,0,N,0,0 ^*/
    x2ap_time_to_wait_et  time_to_wait;
    x2ap_crit_diagno_t  criticality_diagnostics;            /*^ O,X2AP_ENDC_CONFIG_UPDATE_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT,N,0,0 ^*/
}x2ap_endc_config_update_failure_t;

/* x2ap-rrc/son struct */

/*EN-DC_changes_end*/
#endif
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/


#endif /*_X2AP_ENB_INFT_H_*/
