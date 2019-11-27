/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrc_x2ap_uecc_intf.h,v 1.4 2011/09/16 15:45:09 gur22047 Exp $
 ****************************************************************************
 *
 *  File Description :
 *      This file contains types used for
 *      representation of X2AP - UECC API.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rrc_x2ap_uecc_intf.h,v $
 *
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_RRC_X2AP_UECC_INTF_H
#define INCLUDED_RRC_X2AP_UECC_INTF_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrc_defines.h"
#include "x2ap_asn_enc_dec_3gpp.h"
#include "rrc_x2apCommon_intf.h"

/* MENB CHANGES - START */
#include "rrc_x2apEnb_intf.h"
/* MENB CHANGES - END */

#include "rrc_asn_enc_dec.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/* Values for s1ap_message_type_et */
typedef enum
{
    X2AP_INITIATING_MESSAGE,
    X2AP_SUCCESSFUL_OUTCOME,
    X2AP_UNSUCCESSFUL_OUTCOME
} x2ap_message_type_et;

/****************************************************************************
 * Exported Types
 ****************************************************************************/

#pragma pack(push, 1)


typedef enum
{
    X2AP_LINK_DOWN,
    X2AP_LINK_UP
}x2ap_uecc_x2ap_link_status_et;

/******************************************************************************
*   X2AP_STATUS_MSG_IND
******************************************************************************/
typedef struct
{
    U16                      num_served_cells;
    x2ap_srvd_cell_info_t   srvd_cell[MAX_SERVED_CELLS]; /*^ O,0,N,0,0^*/
}x2ap_served_cells_t;

/* MENB CHANGES - START */
typedef struct
{
    U16                         num_served_nr_cells;

    x2ap_srvd_nr_cell_info_t    srvd_cell[MAX_SERVED_NR_CELLS];
    /*^ M,0,N,0,0^*/
}x2ap_nr_served_cells_t;
/* MENB CHANGES - END */


typedef struct
{
#define X2AP_STATUS_MSG_IND_SERVED_CELL_ARRAY_PRESENT 0x01
#define X2AP_STATUS_MSG_IND_ENB_COMM_INFO_PRESENT     0x02

/* MENB CHANGES - START */
#define X2AP_STATUS_MSG_IND_PEER_GB_GNB_ID_PRESENT       0x08
#define X2AP_STATUS_MSG_IND_NR_SERVED_CELL_ARRAY_PRESENT 0x10
/* MENB CHANGES - END */
    U8      bitmask;             /*^ M  ^*/
    U8    peer_enodeb_id;  /*M,0,N,0,0^*/
    x2_gb_enb_id_t           peer_gb_enb_id;
    /*^ O,0,N,0,0^*/
    U8                       connection_status; 
    x2ap_served_cells_t      srvd_cell; /*^ O,0,N,0,0^*/
    x2_enb_comm_info_t       enb_comm_info; /*^ O,0,N,0,0^*/

/* MENB CHANGES - START */
    x2_gb_gnb_id_t           peer_gb_gnb_id;
    /*^O,X2AP_STATUS_MSG_IND_PEER_GB_GNB_ID_PRESENT,N,0,0^*/
    x2ap_nr_served_cells_t   nr_srvd_cell;
    /*^ O,X2AP_STATUS_MSG_IND_NR_SERVED_CELL_ARRAY_PRESENT,N,0,0^*/
/* MENB CHANGES - END */
} x2ap_status_msg_ind_t;


typedef struct 
{
    U8    peer_enodeb_id;  /*M,0,N,0,0^*/
    x2_gb_enb_id_t           peer_gb_enb_id;
    /*^ M,0,N,0,0^*/
}x2ap_reset_msg_ind_t;

typedef struct
{
    U16     message_buff_size;
    U8      message_buff_p[0];  /* ASN-encoded message */
}x2ap_RLF_Report_r9_t;

typedef struct
{
#define X2AP_RLF_REPORT_CONTAINER_PRESENT  0x01
#define X2AP_RLF_CONN_SETUP_INDICATOR      0x02
    U16 bitmask;     /*^ M  ^*/
    U8  cell_index;  /*M,0,N,0,0^*/
    U16 crnti;
    U16 pci;
    U16 short_mac_i;
    U8                  conn_setup_indicator;/*^ O,X2AP_RLF_CONN_SETUP_INDICATOR,H,0,0 ^*/
    /*CA_PHASE3_INTER_CELL_MRO_START*/
    U8                  too_late_inter_cell_ho_flg;//Flg for too late handover
    /*CA_PHASE3_INTER_CELL_MRO_STOP*/
    x2ap_RLF_Report_r9_t  rlf_report;  /*^ O,X2AP_RLF_REPORT_CONTAINER_PRESENT,N,0,0 ^*/
}x2ap_ue_reestab_conn_setup_reconfig_t;

typedef struct
{
#define X2AP_SHORT_MAC_I_PRESENT               0x01
#define X2AP_RLF_REPORT_PRESENT                0x02
#define X2AP_RLF_CONN_SETUP_INDICATOR_PRESENT  0x04
    U16 bitmask;                            /*^ BITMASK ^*/
    x2_gb_enb_id_t enb1_gb_id;  /*^ M,0,N,0,0 ^*/
    x2_gb_enb_id_t enb2_gb_id;  /*^ M,0,N,0,0 ^*/
    U16 failure_cell_pci;               /*^ M,0,H,0,503 ^*/
    x2ap_ecgi_t re_est_cell_ecgi;   /*^ M,0,N,0,0 ^*/
    U16 crnti;                                  /*^ M,0,N,0,0 ^*/
    U16 short_mac_i;                        /*^ O,X2AP_SHORT_MAC_I_PRESENT,N,0,0 ^*/
    U8                   conn_setup_indicator;/*^ O,X2AP_RLF_CONN_SETUP_INDICATOR_PRESENT,H,0,0 ^*/
    /*CA_PHASE3_INTER_CELL_MRO_START*/
    U8                  too_early_inter_cell_ho_flg;//Flg for too early handover
    U8                  too_late_inter_cell_ho_flg;//Flg for too late handover
    /*CA_PHASE3_INTER_CELL_MRO_STOP*/
    x2ap_RLF_Report_r9_t  rlf_report;  /*^ O,X2AP_RLF_REPORT_PRESENT,N,0,0 ^*/
}x2ap_rlf_ind_info_t;

typedef struct
{
#define X2AP_HO_REPORT_REEST_CELL_ECGI_PRESENT  0x01
    U16 bitmask;  /*^ BITMASK ^*/
    x2_gb_enb_id_t enb1_gb_id;   /*^ M,0,N,0,0 ^*/
    x2_gb_enb_id_t enb2_gb_id;   /*^ M,0,N,0,0 ^*/
    U8           peer_enodeb_id; /*^ M,0,N,0,0 ^*/
    x2ap_ho_report_type_et  ho_report_type; /*^ M,0,H,0,1 ^*/ /* x2ap_ho_report_type_et */
    x2ap_cause_t ho_cause;          /*^ M,0,N,0,0 ^*/
    x2ap_ecgi_t  src_ecgi;       /*^ M,0,N,0,0 ^*/
    x2ap_ecgi_t  fail_cell_ecgi;  /*^ M,0,N,0,0 ^*/
    x2ap_ecgi_t  re_est_cell_ecgi; 
        /*^ O,X2AP_HO_REPORT_REEST_CELL_ECGI_PRESENT,N,0,0 ^*/
}x2ap_ho_report_info_t; /*^ API, X2AP_HO_REPORT_IND ^*/

typedef struct
{
    U8 rlf_ind_flag;
    U8 ho_report_flag;
    //x2ap_rlf_ind_info_t rlf_ind;//移到最后，有0个数组
    x2ap_ho_report_info_t ho_report;    
    rrc_rlf_report_t rlf_report;
    U16   ue_index;
    U8    ue_context_released;
    /*CA_PHASE3_INTER_CELL_MRO_START*/
    U8    cell_index;//used to derive ecgi info using x2ap glb context
    /*CA_PHASE3_INTER_CELL_MRO_STOP*/
	x2ap_rlf_ind_info_t rlf_ind;
}x2ap_ue_rlf_resp_t;

/******************************************************************************
*   X2AP_UE_ASSOCIATED_SIG_MSG_REQ
******************************************************************************/
typedef struct
{
    U8      bitmask;             /*^ M  ^*/
#define X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT 0x01
#define X2AP_UE_ASSOC_SIG_MSG_REQ_NEW_ENB_UE_X2AP_ID_PRESENT 0x02
#define X2AP_UE_ASSOC_SIG_MSG_REQ_ECGI_PRESENT               0x04
/* MENB CHANGES - START */
#define X2AP_UE_ASSOC_SIG_MSG_REQ_MENB_UE_X2AP_ID_PRESENT    0x08
#define X2AP_UE_ASSOC_SIG_MSG_REQ_SGNB_UE_X2AP_ID_PRESENT    0x10
/* MENB CHANGES - END */
    U8      procedure_code;

    U8      message_type; /* x2ap_message_type_et */

    U16     ue_id;

    U8     cell_index;

    U16     old_enb_ue_x2ap_id;
    /*^ O, X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT ^*/
    U16     new_enb_ue_x2ap_id;
    /*^ O, X2AP_UE_ASSOC_SIG_MSG_REQ_NEW_ENB_UE_X2AP_ID_PRESENT ^*/ 
    U8      peer_enodeb_id;

    x2ap_ecgi_t  ecgi;
    /*^ O, X2AP_UE_ASSOC_SIG_MSG_REQ_ECGI_PRESENT ^*/ 

/* MENB CHANGES - START */
    U32     menb_ue_x2ap_id;
    /*^ O, X2AP_UE_ASSOC_SIG_MSG_REQ_MENB_UE_X2AP_ID_PRESENT ^*/

    U32     sgnb_ue_x2ap_id;
    /*^ O, X2AP_UE_ASSOC_SIG_MSG_REQ_SGNB_UE_X2AP_ID_PRESENT ^*/ 
/* MENB CHANGES - END */
    U16     message_buff_size;

    U8      message_buff_p[0];
/*^ M, 0, OCTET_STRING, TILL_THE_END ^*/ /* ASN-encoded message */
} x2ap_ue_associated_sig_msg_req_t;
/* from UECC to X2AP */

/******************************************************************************
*   X2AP_UE_ASSOCIATED_SIG_MSG_IND
******************************************************************************/
typedef struct
{
    /*BUG 604 changes start*/
    U16      bitmask;             /*^ M  ^*/
#define X2AP_UE_ASSOC_SIG_MSG_IND_S1_PEER_PRESENT 0x01
#define X2AP_UE_ASSOC_SIG_MSG_IND_SRC_UE_INDEX_PRESENT 0x02
#define X2AP_UE_ASSOC_SIG_MSG_IND_TRG_UE_INDEX_PRESENT 0x04
#define X2AP_UE_ASSOC_SIG_MSG_REQ_SECURITY_FLAG_PRESENT      0x08
    /*BUG 604 changes stop*/

    rrc_cell_index_t         served_cell_id;
    U8      peer_enodeb_id;

    U8      x2ap_procedure_code;
    /*SPR 22723 Fix Start*/
    /*BUG 604 changes start*/
    U8     is_security_key_present;
    /*^ O, X2AP_UE_ASSOC_SIG_MSG_REQ_SECURITY_FLAG_PRESENT ^*/
    /*SPR 22723 Fix Stop*/
    U16     enb_ue_s1ap_id;
    /*^ O, X2AP_UE_ASSOC_SIG_MSG_IND_S1_PEER_PRESENT ^*/
    U16     source_ue_index;
    /*^ O, X2AP_UE_ASSOC_SIG_MSG_IND_SRC_UE_INDEX_PRESENT ^*/
    U16     target_ue_index;
    /*^ O, X2AP_UE_ASSOC_SIG_MSG_IND_TRG_UE_INDEX_PRESENT ^*/
    /*SPR 22723 Fix Start*/
    /*Code Removed*/
    /*SPR 22723 Fix Stop*/
    /*BUG 604 changes stop*/

    U8      message_buff_p[0];
    /*^ M, 0, OCTET_STRING, TILL_THE_END ^*/ /* ASN-encoded message */
} x2ap_ue_associated_sig_msg_ind_t;
/* from X2AP to UECC */


/******************************************************************************
*   X2AP_UE_ASSOCIATED_RELEASE_IND
******************************************************************************/
typedef struct
{
    rrc_cell_index_t         served_cell_id;
    U8      peer_enodeb_id;
    U16     ue_id;
}x2ap_ue_release_ind_t;

/* MENB CHANGES - START */
#define X2AP_PEER_GB_GNB_ID_PRESENT         0x01
#define X2AP_NR_SERVED_CELL_INFO_PRESENT    0x02
/* MENB CHANGES - END */
typedef struct
{
    U8                      presence_flag; /*^ M,0,N,0,0 ^*/
    x2_gb_enb_id_t          peer_gb_enb_id; /*^ M,0,N,0,0 ^*/
    rrc_bool_t              connection_status; /*^ M,0,N,0,0 ^*/
    U16                     num_served_cells; /*^ M,0,N,0,0 ^*/
    void*                   p_srvd_cell_info; /*^ M,0,N,0,0 ^*/
    x2_enb_comm_info_t      enb_comm_info;

    /* MENB CHANGES - START */
    x2_gb_gnb_id_t          peer_gb_gnb_id;
    /*^ O,X2AP_PEER_GB_GNB_ID_PRESENT,N,0,0 ^*/

    void*                   p_nr_srvd_cell_info;
    /*^ O,X2AP_NR_SERVED_CELL_INFO_PRESENT,N,0,0 ^*/
    /* MENB CHANGES - END */
} x2ap_link_info_t;

/* CSR_00040799 Fix Start */
/******************************************************************************
*   X2AP_UECCMD_UE_IFO
******************************************************************************/
typedef struct
{
    U16     total_ue_supported;
    U8      peer_enodeb_id;  
}x2ap_ueccmd_ue_info_t;
/* CSR_00040799 Fix End */

#pragma pack(pop)

#endif /*INCLUDED_RRC_X2AP_UECC_INTF_H*/ 

