/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrc_s1u_intf.h,v 1.5 2010/10/13 07:02:10 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description :
 *      rrc_s1u_intf.h
 *      This file contains types used for
 *      representation of S1-U API inside RRC.
 *      Based on LTE_Relay_API_30_Oct_RRC.doc
 *      and LTE_L3_API_Rel_1_0_Rev_0_37.doc.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rrc_s1u_intf.h,v $
 * Revision 1.5  2010/10/13 07:02:10  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.2.10.5  2010/08/10 10:01:18  gur18569
 * incorported review comments for ho
 *
 * Revision 1.2.10.4  2010/08/05 08:01:08  gur15697
 * merging round 2
 *
 * Revision 1.2.10.3  2010/07/21 08:16:56  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.2.10.2  2010/06/22 05:30:37  gur22047
 * Internal Interface update
 *
 * Revision 1.2.10.1  2010/06/21 09:37:17  gur15697
 * Updated for external interface
 *
 * Revision 1.2  2010/01/04 16:10:03  ukr15916
 * no message
 *
 * Revision 1.1.4.1  2009/11/26 18:33:42  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.9  2009/11/19 15:00:37  ukr16018
 * W80.
 *
 * Revision 1.1.2.8  2009/11/19 09:52:11  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.2.7  2009/11/16 11:47:03  ukr18877
 * Code comments applied in LLIM
 *
 * Revision 1.1.2.6  2009/11/09 10:17:39  ukr18877
 * S1-U interface changed
 *
 * Revision 1.1.2.5  2009/11/05 10:15:17  ukr16018
 * S1U interface updated
 *
 * Revision 1.1.2.4  2009/11/04 14:12:00  ukr16018
 * S1U interface updated
 *
 * Revision 1.1.2.3  2009/11/03 15:00:11  ukr18877
 * Bug in S1U interface fixed
 *
 * Revision 1.1.2.2  2009/11/03 14:55:08  ukr18877
 * S1U composer/parser added
 *
 * Revision 1.1.2.1  2009/11/03 13:08:19  ukr16018
 * Initial version of rrc_s1u_intf.h added
 *
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_RRC_S1U_INTF_H
#define INCLUDED_RRC_S1U_INTF_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrc_defines.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define RRC_S1U_MAX_NUM_LC      8

typedef enum
{
    S1U_SUCCESS,
    S1U_RESOURCES_NOT_AVAILABLE,
    S1U_CTXT_NOT_FOUND,
    S1U_DUPLICATE_PEER_TEID,
    S1U_INV_SAP_CFG,
    S1U_IPV6_ADDR_RECEIVED,
    /*SPR 21767 Start*/
    EGTPU_INV_SRC_ADDR_RECEIVED,
    EGTPU_PEER_ADDRESS_MISSING,
    EGTPU_MAX_ERR_CAUSE
    /*SPR 21767 End*/
} rrc_s1u_cause_et;

typedef enum
{
    S1U_SEQ_ENABLE,
    S1U_SEQ_DISABLE
} rrc_s1u_disable_et;

typedef enum
{
    S1U_PDCP_EXT_ENABLE,
    S1U_PDCP_EXT_DISABLE
} rrc_s1u_pdcp_ext_et;

/****************************************************************************
 * Exported Types
 ****************************************************************************/

#pragma pack(push, 1)

/******************************************************************************
*   S1U_DELETE_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_s1u_delete_ue_entity_req_t
{
    
    rrc_ue_index_t ue_index;
} rrc_s1u_delete_ue_entity_req_t; /*^ API, RRC_S1U_DELETE_UE_ENTITY_REQ ^*/

/******************************************************************************
*   S1U_DELETE_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_s1u_delete_ue_entity_cnf_t
{
    
    rrc_ue_index_t  ue_index;
    U8              response_code;              /* rrc_s1u_cause_et */
} rrc_s1u_delete_ue_entity_cnf_t; /*^ API, RRC_S1U_DELETE_UE_ENTITY_CNF ^*/

/******************************************************************************
*   S1U_RECONFIGURE_UE_ENTITY_REQ
******************************************************************************/

typedef struct _rrc_s1u_ie_gsn_addr_t
{
    rrc_counter_t   data_length;
/*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/

/* ipv6_changes */
    U8              data[RRC_MAX_IP_ADDR_LEN_IPV6];
    /*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/
/* ipv6_changes */
} rrc_s1u_ie_gsn_addr_t;

typedef struct _rrc_s1u_ie_qos_profile_t
{
    U8              allocation_retention_priority;
    rrc_counter_t   qos_profile_data_length;
/*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/

    U8              qos_profile_data[RRC_S1U_MAX_QOS_PROFILE_DATA];
/*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/

} rrc_s1u_ie_qos_profile_t;

typedef struct _rrc_s1u_ie_seq_num_t
{
    U16 ul_sequence_number;                     /*^ M, 0, N, 0, 0 ^*/
    U16 dl_sequence_number;                     /*^ M, 0, N, 0, 0 ^*/
} rrc_s1u_ie_seq_num_t;

typedef struct _rrc_s1u_ie_seq_disable_t
{
    U8  disable;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_s1u_disable_et */

} rrc_s1u_ie_seq_disable_t;

typedef struct _rrc_s1u_ie_pdcp_ext_flag_t
{
    U8  pdcp_ext_flag;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_s1u_pdcp_ext_et */
} rrc_s1u_ie_pdcp_ext_t;

typedef struct _rrc_s1u_reordering_reqd_t
{
    U8  reordering_reqd;                        /*^ M, 0, H, 0, 1 ^*/
} rrc_s1u_reordering_reqd_t;


typedef struct _rrc_s1u_ie_drb_id_t
{
    U8  drb_id;        /*^ M, 0, N, 0, 0 ^*/
} rrc_s1u_ie_drb_id_t;

/*OPTION3X Changes START*/
typedef struct _rrc_s1u_sap_param_t
{
    U8    sn_size;
    /*^ M, 0, B, 12, 18 ^*/

    U8    rlc_mode;
    /*^ M, 0, H, 0, 2 ^*/
}rrc_s1u_sap_param_t;

#define RRC_S1U_IE_SAP_PARAM_PRESENT    0x01
typedef struct _rrc_dc_bearer_type_t
{
    rrc_bitmask_t            bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                       dc_bearer_type;
    /*^ M, 0, H, 1, 2 ^*/ /* rrc_dc_bearer_type_et*/

    rrc_s1u_sap_param_t      sap_param;
    /*^ TLV, RRC_S1U_IE_SAP_PARAM_TAG, RRC_S1U_IE_SAP_PARAM_PRESENT ^*/
}rrc_dc_bearer_type_t;
/*OPTION3X Changes STOP*/

typedef struct  _rrc_s1u_ie_buffer_ind_t
{
    U8  buffer_ind;    /*^ M, 0, H, 0, 0 ^*/
}rrc_s1u_ie_buffer_ind_t;

/*
typedef struct _rrc_s1u_ie_teid_t
{
    rrc_gtp_teid_t teid;
} rrc_s1u_ie_teid_t;
*/

typedef struct _rrc_s1u_ie_teid_peer_t
{
    rrc_gtp_teid_t teid;
} rrc_s1u_ie_teid_peer_t;

typedef struct _rrc_s1u_ie_teid_self_t
{
    rrc_gtp_teid_t teid;  /*^ M, 0, L, 1, 0 ^*/
} rrc_s1u_ie_teid_self_t;


#define RRC_S1U_IE_TUNNEL_INFO_TEID_PEER_PRESENT              0x01
#define RRC_S1U_IE_TUNNEL_INFO_TRANSPORT_ADDR_PRESENT  0x02

typedef struct  _rrc_s1u_ie_tunnel_info_t
{
    rrc_bitmask_t               bitmask; /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                               sap_flag;
    rrc_s1u_ie_teid_peer_t      teid_peer; 
    /*^ TLV, RRC_S1U_IE_TEID_PEER_TAG, RRC_S1U_IE_TUNNEL_INFO_TEID_PEER_PRESENT ^*/
    
    rrc_s1u_ie_gsn_addr_t  transport_addr; 
    /*^ TLV, RRC_S1U_IE_GSN_ADDR_TAG, RRC_S1U_IE_TUNNEL_INFO_TRANSPORT_ADDR_PRESENT ^*/
}rrc_s1u_ie_tunnel_info_t;


#define RRC_S1U_IE_SETUP_BUFFER_IND_PRESENT    0x01
#define RRC_S1U_IE_SETUP_TEID_SELF_PRESENT          0x02
#define RRC_S1U_IE_SETUP_QOS_PROFILE_PRESENT        0x04
#define RRC_S1U_IE_SETUP_SEQ_NUM_PRESENT            0x08
#define RRC_S1U_IE_SETUP_SEQ_FLAG_PRESENT           0x10
#define RRC_S1U_IE_SETUP_REORDERING_REQD_PRESENT    0x20
#define RRC_S1U_IE_SETUP_PDCP_EXT_FLAG_PRESENT      0x40
#define RRC_S1U_IE_SETUP_DRBID_PRESENT              0x80
/*OPTION3X Changes START*/
#define RRC_S1U_IE_SETUP_DC_BEARER_TYPE_PRESENT     0x100
/*OPTION3X Changes STOP*/

typedef struct _rrc_s1u_ie_relay_setup_sap_req_t
{
    rrc_bitmask_t     bitmask; /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_lc_id_t          lc_id;          /*^ M, 0, B, 3, 10 ^*/
    U32                   qos_id;

    rrc_s1u_ie_drb_id_t       drb_id;
    /*^ TLV, RRC_S1U_IE_DRBID_SELF_TAG, RRC_S1U_IE_SETUP_DRBID_PRESENT ^*/

    rrc_s1u_ie_buffer_ind_t   buffer_ind; 
   /*^ TLV, RRC_S1U_IE_BUFFER_IND_TAG, RRC_S1U_IE_SETUP_BUFFER_IND_PRESENT ^*/
   
    rrc_s1u_ie_teid_self_t                   teid_self; 
   /*^ TLV, RRC_S1U_IE_TEID_SELF_TAG, RRC_S1U_IE_SETUP_TEID_SELF_PRESENT ^*/
      
    rrc_s1u_ie_qos_profile_t    qos_profile; 
   /*^ TLV, RRC_S1U_IE_QOS_PROFILE_TAG, RRC_S1U_IE_SETUP_QOS_PROFILE_PRESENT ^*/
   
    rrc_s1u_ie_seq_num_t       seq_num; 
   /*^ TLV, RRC_S1U_IE_SEQ_NUM_TAG, RRC_S1U_IE_SETUP_SEQ_NUM_PRESENT ^*/
   
    rrc_s1u_ie_seq_disable_t    seq_flag; 
   /*^ TLV, RRC_S1U_IE_SEQ_DISABLE_TAG, RRC_S1U_IE_SETUP_SEQ_FLAG_PRESENT ^*/
   
    rrc_s1u_reordering_reqd_t   reordering_reqd; 
   /*^ TLV, RRC_S1U_IE_REORDERING_REQD_TAG, RRC_S1U_IE_SETUP_REORDERING_REQD_PRESENT ^*/
 
/* SPR 10355 Fix Start */  

    rrc_s1u_ie_pdcp_ext_t        pdcp_ext;
   /*^ TLV, RRC_S1U_IE_PDCP_EXT_TAG, RRC_S1U_IE_SETUP_PDCP_EXT_FLAG_PRESENT ^*/

    /*OPTION3X Changes START*/
    rrc_dc_bearer_type_t       dc_bearer_type;
   /*^ TLV, RRC_S1U_IE_DC_BEARER_FLAG, RRC_S1U_IE_SETUP_DC_BEARER_TYPE_PRESENT^*/
    /*OPTION3X Changes STOP*/

    rrc_counter_t             num_tunnel_info; 
   /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/
   
    rrc_s1u_ie_tunnel_info_t        tunnel_info[RRC_S1U_MAX_TUNNELS_PER_LC];
   /*^ TLV, SEQUENCE, RRC_S1U_IE_TUNNEL_INFO_TAG ^*/

/* SPR 10355 Fix End */  
} rrc_s1u_ie_relay_setup_sap_req_t;


#define RRC_S1U_IE_RELEASE_TUNNEL_INFO_TRANSPORT_ADDR_PRESENT 0x01

typedef struct _rrc_release_tunnel_info_t
{
    rrc_bitmask_t               bitmask; 
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_gtp_teid_t                 teid_self; /*^ M, 0, L, 1, 0 ^*/
    rrc_s1u_ie_gsn_addr_t    transport_addr; 
    /*^ TLV, RRC_S1U_IE_GSN_ADDR_TAG, RRC_S1U_IE_RELEASE_TUNNEL_INFO_TRANSPORT_ADDR_PRESENT ^*/

}rrc_release_tunnel_info_t;

typedef struct _rrc_s1u_ie_relay_rel_sap_req_t
{
    rrc_lc_id_t     lc_id;

    rrc_counter_t                   num_release_tunnel_info; 
    /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/    
    
    rrc_release_tunnel_info_t  release_tunnel_info[RRC_S1U_MAX_TUNNELS_PER_LC]; 
    /*^ TLV, SEQUENCE, RRC_S1U_IE_RELEASE_TUNNEL_INFO_TAG ^*/

} rrc_s1u_ie_relay_rel_sap_req_t;

typedef struct _rrc_s1u_reconfigure_ue_entity_req_t
{

    rrc_ue_index_t ue_index; 

    rrc_counter_t                       num_setup_sap_req;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_s1u_ie_relay_setup_sap_req_t    setup_sap_req[RRC_S1U_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_S1U_IE_RELAY_SETUP_SAP_REQ_TAG ^*/


    rrc_counter_t                       num_rel_sap_req;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_s1u_ie_relay_rel_sap_req_t      rel_sap_req[RRC_S1U_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_S1U_IE_RELAY_REL_SAP_REQ_TAG ^*/

} rrc_s1u_reconfigure_ue_entity_req_t;
/*^ API, RRC_S1U_RECONFIGURE_UE_ENTITY_REQ ^*/


/******************************************************************************
*   RRC_S1U_RECONFIGURE_UE_ENTITY_CNF
******************************************************************************/
#define RRC_S1U_IE_TUNNEL_INFO_CNF_TEID_SELF_PRESENT  0x01
#define RRC_S1U_IE_TUNNEL_INFO_CNF_TEID_PEER_PRESENT  0x02
#define RRC_S1U_IE_TUNNEL_INFO_CNF_TRANSPORT_ADDR_PRESENT  0x04

typedef struct _rrc_s1u_ie_tunnel_info_cnf_t
{
    rrc_bitmask_t               bitmask; 
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                  cause;   /*^ M, 0, H, 0, 5 ^*/  /* rrc_s1u_cause_et */
    U16                 rb_direction; /*^ M, 0, H, 0, 2 ^*/
    
    rrc_s1u_ie_teid_self_t  teid_self;  
    /*^ TLV, RRC_S1U_IE_TEID_SELF_TAG, RRC_S1U_IE_TUNNEL_INFO_CNF_TEID_SELF_PRESENT ^*/  
    
    rrc_s1u_ie_teid_peer_t  teid_peer; 
    /*^ TLV, RRC_S1U_IE_TEID_PEER_TAG, RRC_S1U_IE_TUNNEL_INFO_CNF_TEID_PEER_PRESENT ^*/
    
    rrc_s1u_ie_gsn_addr_t     transport_addr; 
    /*^ TLV, RRC_S1U_IE_GSN_ADDR_TAG, RRC_S1U_IE_TUNNEL_INFO_CNF_TRANSPORT_ADDR_PRESENT ^*/

}rrc_s1u_ie_tunnel_info_cnf_t;


#define RRC_S1U_IE_SETUP_SAP_CNF_SEQ_NUM_PRESENT 0x01

typedef struct _rrc_s1u_ie_relay_setup_sap_cnf_t
{
    rrc_bitmask_t         bitmask; /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    
    rrc_lc_id_t           lc_id;   /*^ M, 0, B, 3, 10 ^*/
    /*SPR 21767 Start*/
    U8                    cause;   /*^ M, 0, H, 0, EGTPU_PEER_ADDRESS_MISSING ^*/ /* rrc_s1u_cause_et */
    /*SPR 21767 End*/

    rrc_counter_t                  num_sap_cnf; 
    /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/
    
    rrc_s1u_ie_tunnel_info_cnf_t   tunnel_info_cnf[RRC_S1U_MAX_TUNNELS_PER_LC]; 
    /*^ TLV, SEQUENCE, RRC_S1U_IE_TUNNEL_INFO_CNF_TAG ^*/

    rrc_s1u_ie_seq_num_t        seq_num; 
    /*^ TLV, RRC_S1U_IE_SEQ_NUM_TAG, RRC_S1U_IE_SETUP_SAP_CNF_SEQ_NUM_PRESENT ^*/

} rrc_s1u_ie_relay_setup_sap_cnf_t;

#define RRC_S1U_IE_REL_CNF_SEQ_NUM_PRESENT      0x01

typedef struct _rrc_s1u_ie_relay_rel_sap_cnf_t
{
    rrc_bitmask_t               bitmask;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/


    rrc_lc_id_t               lc_id;    /*^ M, 0, B, 3, 10 ^*/
    U8                        cause;  /*^ M, 0, H, 0, 5 ^*/ /* rrc_s1u_cause_et */

    rrc_s1u_ie_seq_num_t      seq_num;
/*^ TLV, RRC_S1U_IE_SEQ_NUM_TAG, RRC_S1U_IE_REL_CNF_SEQ_NUM_PRESENT ^*/
    /* present if cause contains success */

    rrc_counter_t         num_release_tunnel_info; 
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_release_tunnel_info_t  release_tunnel_info[RRC_S1U_MAX_TUNNELS_PER_LC]; 
/*^ TLV, SEQUENCE, RRC_S1U_IE_RELEASE_TUNNEL_INFO_TAG ^*/

} rrc_s1u_ie_relay_rel_sap_cnf_t;

typedef struct _rrc_s1u_reconfigure_ue_entity_cnf_t
{

    rrc_ue_index_t  ue_index;

    rrc_counter_t                       num_setup_sap_cnf;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_s1u_ie_relay_setup_sap_cnf_t    setup_sap_cnf[RRC_S1U_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_S1U_IE_RELAY_SETUP_SAP_CNF_TAG ^*/


    rrc_counter_t                       num_rel_sap_cnf;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_s1u_ie_relay_rel_sap_cnf_t      rel_sap_cnf[RRC_S1U_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_S1U_IE_RELAY_REL_SAP_CNF_TAG ^*/

} rrc_s1u_reconfigure_ue_entity_cnf_t; /*^ API, RRC_S1U_RECONFIGURE_UE_ENTITY_CNF ^*/


/******************************************************************************
*   S1U_CREATE_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_s1u_create_ue_entity_req_t
{

    rrc_ue_index_t ue_index;   

    rrc_counter_t                       num_setup_sap_req; 
      /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/
      
    rrc_s1u_ie_relay_setup_sap_req_t    setup_sap_req[RRC_S1U_MAX_NUM_LC];
      /*^ TLV, SEQUENCE, RRC_S1U_IE_RELAY_SETUP_SAP_REQ_TAG ^*/

} rrc_s1u_create_ue_entity_req_t; /*^ API, RRC_S1U_CREATE_UE_ENTITY_REQ ^*/

/******************************************************************************
*   S1U_CREATE_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_s1u_create_ue_entity_cnf_t
{

    rrc_ue_index_t  ue_index;
    U8              response_code;              /* rrc_s1u_cause_et */

    rrc_counter_t                       num_setup_sap_cnf;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_s1u_ie_relay_setup_sap_cnf_t    setup_sap_cnf[RRC_S1U_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_S1U_IE_RELAY_SETUP_SAP_CNF_TAG ^*/

} rrc_s1u_create_ue_entity_cnf_t; /*^ API, RRC_S1U_CREATE_UE_ENTITY_CNF ^*/

/******************************************************************************
*   S1U_ERROR_IND
******************************************************************************/
typedef struct _rrc_s1u_error_ind_t
{
    
    rrc_ue_index_t  ue_index;
    rrc_lc_id_t     lc_id;                      /*^ M, 0, B, 3, 10 ^*/
} rrc_s1u_error_ind_t; /*^ API, RRC_S1U_ERROR_IND ^*/

/******************************************************************************
*   S1U_PATH_FAILURE_IND
******************************************************************************/
#define RRC_TRANSPORT_ADDR_PRESENT      0x01

typedef struct _rrc_s1u_path_failure_ind_t
{
    rrc_bitmask_t           bitmask;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    
    rrc_s1u_ie_gsn_addr_t   transport_addr;
/*^ TLV, RRC_S1U_IE_GSN_ADDR_TAG, RRC_TRANSPORT_ADDR_PRESENT ^*/

} rrc_s1u_path_failure_ind_t; /*^ API, RRC_S1U_PATH_FAILURE_IND ^*/


/******************************************************************************
*   S1U_END_MARKER_IND
******************************************************************************/
#define RRC_S1U_END_MARKER_IND_TRANSPORT_ADDR_PRESENT      0x01

typedef struct _rrc_s1u_end_marker_ind_t
{
    rrc_bitmask_t           bitmask;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    
    rrc_ue_index_t  ue_index;
    rrc_lc_id_t        lc_id;              /*^ M, 0, B, 3, 10 ^*/
    rrc_gtp_teid_t   teid_self;            /*^ M, 0, L, 1, 0 ^*/
    rrc_s1u_ie_gsn_addr_t   transport_addr;
/*^ TLV, RRC_S1U_IE_GSN_ADDR_TAG, RRC_S1U_END_MARKER_IND_TRANSPORT_ADDR_PRESENT ^*/
}rrc_s1u_end_marker_ind_t;  /*^ API, RRC_S1U_END_MARKER_IND ^*/

/******************************************************************************
*   S1U_PATH_SUCCESS_IND
******************************************************************************/

typedef struct _rrc_s1u_path_success_ind_t
{
    rrc_bitmask_t           bitmask;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

} rrc_s1u_path_success_ind_t; /*^ API, RRC_S1U_PATH_SUCCESS_IND ^*/

/*BUG 604 changes start*/
/******************************************************************************
*   RRC_S1U_INTRA_ENB_DATA_FWD_REQ
******************************************************************************/

typedef struct _rrc_s1u_intra_enb_data_fwd_req_t
{
    rrc_ue_index_t    ue_index;        /*Source UeIndex*/
    rrc_ue_index_t    target_ue_index; /*Target UeIndex for data forwarding */

} rrc_s1u_intra_enb_data_fwd_req_t; /*^ API, RRC_S1U_INTRA_ENB_DATA_FWD_REQ ^*/

/******************************************************************************
*   RRC_S1U_INTRA_ENB_DATA_FWD_CNF
******************************************************************************/

typedef struct _rrc_s1u_intra_enb_data_fwd_cnf_t
{
    rrc_ue_index_t    ue_index;        /*Source UeIndex*/

    U8                response;        /* rrc_s1u_cause_et */

} rrc_s1u_intra_enb_data_fwd_cnf_t; /*^ API, RRC_S1U_INTRA_ENB_DATA_FWD_CNF ^*/

/******************************************************************************
*   RRC_S1U_LOCAL_PATH_SWITCH_REQ
******************************************************************************/

typedef struct _rrc_s1u_local_path_switch_req_t
{
    rrc_ue_index_t    ue_index; /*Target UeIndex*/

    rrc_ue_index_t    ue_index_for_path_switch;        /*UeIndex - UeIndex for path switch*/

} rrc_s1u_local_path_switch_req_t; /*^ API, RRC_S1U_LOCAL_PATH_SWITCH_REQ ^*/

/******************************************************************************
*   RRC_S1U_LOCAL_PATH_SWITCH_CNF
******************************************************************************/

typedef struct _rrc_s1u_local_path_switch_cnf_t
{
    rrc_ue_index_t    ue_index;        /*Target UeIndex*/

    U8                response;        /* rrc_s1u_cause_et */

} rrc_s1u_local_path_switch_cnf_t; /*^ API, RRC_S1U_LOCAL_PATH_SWITCH_CNF ^*/

/*BUG 604 changes stop*/

/******************************************************************************
*   RRC_S1U_CELL_CONFIG_REQ
******************************************************************************/
typedef struct _s1u_communication_info_t
{
    U8     pdcpInstance;     /*^ M, 0, N, 0, 0 ^*/
    U16   pdcpRxPort;        /*^ M, 0, N, 0, 0 ^*/
    U8    pdcpIpAddr[16];   /*^ M,0, OCTET_STRING, FIXED ^*/
}s1u_communication_info_t;


typedef struct _rrc_s1u_config_cell_req_t
{
U16			Cell_index;     /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
s1u_communication_info_t	s1u_communication_info; 	/*^ M, 0, N, 0, 0 ^*/	
} rrc_s1u_config_cell_req_t; /*^ API, RRC_S1U_CELL_CONFIG_REQ ^*/


/******************************************************************************
*   RRC_S1U_CELL_CONFIG_CNF
******************************************************************************/
typedef struct _rrc_s1u_config_cell_cnf_t
{
    rrc_cell_index_t    cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
    U8		response;
    /*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */
} rrc_s1u_config_cell_cnf_t; /*^ API, RRC_S1U_CELL_CONFIG_CNF ^*/


#pragma pack(pop)

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif  /* INCLUDED_RRC_S1U_INTF_H */

