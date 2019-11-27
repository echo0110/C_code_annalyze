/******************************************************************************
*
*   FILE NAME:
*       rrc_uecc_llim_intf.h
*
*   DESCRIPTION:
*       This file contains types used for representation of UECC-LLIM API
*       inside RRC
*       Based on LTE_RRC_DD_v0.1.doc.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   30 Apr 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_UECC_LLIM_INTF_H_
#define _RRC_UECC_LLIM_INTF_H_

#include "rrc_defines.h"
#include "rrc_rrm_intf.h"
/*SPR_17121_START*/
#include "rrc_pdcp_intf.h"
/*SPR_17121_END*/

#define RRC_UECC_LLIM_MAX_CREATE_DRB    10
#define RRC_UECC_LLIM_MAX_DRB                     8
/* Start E-RAB Modify */
#define RRC_UECC_LLIM_MODIFY_DRB_CONFIG_VERSION 0x02
#define RRC_UECC_OLD_DRB_CONFIG_VERSION         0x00
#define RRC_UECC_NEW_DRB_CONFIG_VERSION         0x01
/* End E-RAB Modify */

typedef enum
{
    RRC_LLIM_FAILURE = 0,
    RRC_LLIM_SUCCESS = 1,
    RRC_LLIM_UE_ALREADY_EXIST = 2,
    RRC_LLIM_UE_NOT_ENOUGH_MEMORY,
    RRC_LLIM_UE_IS_ABSENT,
    RRC_LLIM_INCORRECT_DATA_PASSED,
    RRC_LLIM_CREATE_UE_FAIL_PHY,
    RRC_LLIM_CREATE_UE_TE_PHY,
    RRC_LLIM_CREATE_UE_FAIL_MAC,
    RRC_LLIM_CREATE_UE_TE_MAC,
    RRC_LLIM_CREATE_UE_FAIL_RLC,
    RRC_LLIM_CREATE_UE_TE_RLC,
    RRC_LLIM_CREATE_UE_FAIL_PDCP,
    RRC_LLIM_CREATE_UE_TE_PDCP,
    RRC_LLIM_CREATE_UE_FAIL_S1U,
    RRC_LLIM_CREATE_UE_TE_S1U,
    RRC_LLIM_DELETE_UE_TE,

    RRC_LLIM_ADD_LC_FAIL_MAC,
    RRC_LLIM_ADD_LC_TE_MAC,
    RRC_LLIM_ADD_LC_FAIL_RLC,
    RRC_LLIM_ADD_LC_TE_RLC,
    RRC_LLIM_ADD_LC_FAIL_PDCP,
    RRC_LLIM_ADD_LC_TE_PDCP,
    RRC_LLIM_ADD_LC_FAIL_S1U,
    RRC_LLIM_ADD_LC_TE_S1U,
    RRC_LLIM_ADD_LC_FAIL_PHY,
    RRC_LLIM_ADD_LC_TE_PHY,
    RRC_LLIM_ADD_LC_ROLLBACK_TE,
    /* ERB RELEASE COMMAND START */
    RRC_LLIM_DELETE_LC_TE,
    RRC_LLIM_DELETE_LC_FAIL_MAC,
    RRC_LLIM_DELETE_LC_FAIL_RLC,
    RRC_LLIM_DELETE_LC_FAIL_PDCP,
    RRC_LLIM_DELETE_LC_FAIL_S1U,
    /* ERB RELEASE COMMAND END */

    /* E-RAB Modify Starts */
    RRC_LLIM_MODIFY_LC_FAIL_MAC,
    RRC_LLIM_MODIFY_LC_TE_MAC,
    RRC_LLIM_MODIFY_LC_FAIL_RLC,
    RRC_LLIM_MODIFY_LC_TE_RLC,
    RRC_LLIM_MODIFY_LC_FAIL_PDCP,
    RRC_LLIM_MODIFY_LC_TE_PDCP,
    RRC_LLIM_MODIFY_LC_FAIL_S1U,
    RRC_LLIM_MODIFY_LC_TE_S1U,
    RRC_LLIM_MODIFY_LC_FAIL_PHY,
    RRC_LLIM_MODIFY_LC_TE_PHY,
    RRC_LLIM_MODIFY_LC_ROLLBACK_TE,
    /* E-RAB Modify Ends */
    
    RRC_LLIM_SUSPEND_UE_TE_PDCP,
    RRC_LLIM_SUSPEND_UE_FAIL_PDCP,
    RRC_LLIM_SUSPEND_UE_TE_MAC,
    RRC_LLIM_SUSPEND_UE_FAIL_MAC,
    RRC_LLIM_REESTABLSIH_UE_TE_RLC,
    RRC_LLIM_REESTABLISH_UE_FAIL_RLC,
    RRC_LLIM_REESTABLSIH_UE_TE_PDCP,
    RRC_LLIM_REESTABLISH_UE_FAIL_PDCP,
    RRC_LLIM_RESUME_UE_TE_PDCP,
    RRC_LLIM_RESUME_UE_FAIL_PDCP,
    RRC_LLIM_CELL_NOT_EXIST,
    RRC_LLIM_RESPONSE_CODE_LAST
} rrc_llim_uecc_response_et;

typedef enum
{
    RRC_STOP_BUFFER,
    RRC_START_BUFFER
}rrc_llim_buffer_ind_et;

typedef enum
{
    RLC_LLIM_NO_ACTION,
    RLC_LLIM_FREEZE
} rrc_llim_uecc_lc_state_et;
#pragma pack(push, 1)

/******************************************************************************
*   UECC-LLIM Create SRB entity IE
******************************************************************************/
typedef struct _rrc_uecc_llim_create_srb_entity_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_SRB_CONFIGURE_INTEGRITY_PROTECTION_PRESENT    0x01
#define UECC_LLIM_SRB_CONFIGURE_CIPHERING_PRESENT               0x02

    rrc_lc_id_t                 lc_id;
    rrm_srb_config_t            srb_config;

    rrc_pdcp_config_int_t       configure_integrity_protection;
/*^ O, UECC_LLIM_SRB_CONFIGURE_INTEGRITY_PROTECTION_PRESENT ^*/

    rrc_pdcp_config_srb_ciph_t      configure_ciphering;
/*^ O, UECC_LLIM_SRB_CONFIGURE_CIPHERING_PRESENT ^*/

} rrc_uecc_llim_create_srb_entity_t;

/*REL 1.2:HANDOVER PROCEDURE START*/
/******************************************************************************
*   UECC-LLIM s1u gsn addr IE
******************************************************************************/
typedef struct _rrc_uecc_llim_s1u_gsn_addr_t
{
    rrc_counter_t   data_length;
/*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/
/* ipv6_changes */    
    U8              data[RRC_MAX_IP_ADDR_LEN_IPV6];
    /*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/
/* ipv6_changes */
} rrc_uecc_llim_s1u_gsn_addr_t;

/******************************************************************************
*   UECC-LLIM s1u tunnel info IE
******************************************************************************/
typedef struct _rrc_uecc_llim_s1u_tunnel_info_t
{
    rrc_bitmask_t                bitmask;
#define RRC_UECC_LLIM_S1U_TUNNEL_INFO_TEID_PEER_PRESENT            0x01
#define RRC_UECC_LLIM_S1U_TUNNEL_INFO_TRANSPORT_ADDRESS_PRESENT    0x02
    U8                        sap_flag;
    rrc_gtp_teid_t            teid_peer;
/*^ O, RRC_UECC_LLIM_S1U_TUNNEL_INFO_TEID_PEER_PRESENT ^*/
    rrc_uecc_llim_s1u_gsn_addr_t        transport_addr;
/*^ O, RRC_UECC_LLIM_S1U_TUNNEL_INFO_TRANSPORT_ADDRESS_PRESENT ^*/
} rrc_uecc_llim_s1u_tunnel_info_t;

/******************************************************************************
*   UECC-LLIM s1u release tunnel info IE
******************************************************************************/
typedef struct _rrc_uecc_llim_s1u_release_tunnel_info_t
{
    rrc_bitmask_t                bitmask;
#define RRC_UECC_LLIM_S1U_RELEASE_TUNNEL_INFO_TRANSPORT_ADDRESS_PRESENT    0x01
    rrc_gtp_teid_t               teid_self;
    rrc_uecc_llim_s1u_gsn_addr_t        transport_addr;
/*^ O, RRC_UECC_LLIM_S1U_RELEASE_TUNNEL_INFO_TRANSPORT_ADDRESS_PRESENT ^*/
} rrc_uecc_llim_s1u_release_tunnel_info_t;

/******************************************************************************
*   UECC-LLIM tunnel info cnf IE
******************************************************************************/
typedef struct _rrc_uecc_llim_s1u_tunnel_info_cnf_t
{
    rrc_bitmask_t               bitmask;

#define RRC_UECC_LLIM_S1U_TUNNEL_INFO_CNF_TEID_SELF_PRESENT            0x01
#define RRC_UECC_LLIM_S1U_TUNNEL_INFO_CNF_TEID_PEER_PRESENT            0x02
#define RRC_UECC_LLIM_S1U_TUNNEL_INFO_CNF_TRANSPORT_ADDRESS_PRESENT    0x04
    U8                          cause; /* rrc_s1u_cause_et */
    rrc_rb_direction_t          rb_direction;  /* rrc_rb_direction_et */ /*not used*/

    rrc_gtp_teid_t              teid_self;
/*^ O, RRC_UECC_LLIM_S1U_TUNNEL_INFO_CNF_TEID_SELF_PRESENT ^*/
    rrc_gtp_teid_t              teid_peer;/*not used*/
/*^ O, RRC_UECC_LLIM_S1U_TUNNEL_INFO_CNF_TEID_PEER_PRESENT ^*/
    rrc_uecc_llim_s1u_gsn_addr_t        transport_addr;
/*^ O, RRC_UECC_LLIM_S1U_TUNNEL_INFO_CNF_TRANSPORT_ADDRESS_PRESENT  ^*/
} rrc_uecc_llim_s1u_tunnel_info_cnf_t;

/******************************************************************************
*   UECC-LLIM create lc status IE
******************************************************************************/

typedef struct
{
    U8                     tunnel_info_cnf_count;
    rrc_uecc_llim_s1u_tunnel_info_cnf_t
        tunnel_info_cnf[RRC_S1U_MAX_TUNNELS_PER_LC];
} rrc_uecc_llim_s1u_response_t;

typedef struct _rrc_uecc_llim_create_lc_status_t
{
    rrc_bitmask_t           bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_CREATE_LC_STATUS_S1U_RESPONSE_PRESENT         0x01

    rrc_lc_id_t     lc_id;
    /*rrc_response_t  response;*/   /* rrc_return_et +
                                   rrc_llim_uecc_response_et */
    U32  response;   /* rrc_return_et +
                                   rrc_llim_uecc_response_et */
    rrc_uecc_llim_s1u_response_t s1u_response;
    /*^ O, UECC_LLIM_CREATE_LC_STATUS_S1U_RESPONSE_PRESENT ^*/

} rrc_uecc_llim_create_lc_status_t;

/******************************************************************************
*   UECC-LLIM Create DRB entity IE
******************************************************************************/
typedef struct
{
    rrc_bitmask_t   bitmask;   /*^ BITMASK ^*/
#define RRC_UECC_LLIM_DRB_S1U_TEID_SELF_PRESENT          0x01

    rrc_gtp_teid_t  teid_self;
/*^ O, RRC_UECC_LLIM_DRB_S1U_TEID_SELF_PRESENT ^*/
    U8              tunnel_list_count;
    rrc_uecc_llim_s1u_tunnel_info_t
        tunnel_info_list[RRC_S1U_MAX_TUNNELS_PER_LC];
} rrc_uecc_llim_drb_s1u_info_t;

typedef struct _rrc_gbr_qos_info_t
{
    U32                 dl_mbr;
    U32                 ul_mbr;
    U32                 dl_gbr;
    U32                 ul_gbr;
} rrc_gbr_qos_info_t;
                             
/******************************************************************************
*   UECC-LLIM drb config containg drb_config and qos_info for uecc and llim
*   interface
*******************************************************************************/
typedef struct _rrc_qos_info_t
{
    rrc_bitmask_t   bitmask;   /*^ BITMASK ^*/
#define RRC_GBR_QOS_INFO_PRESENT        0x01
    U8                  qci;
    rrc_gbr_qos_info_t  gbr_qos_info;
/*^ O, RRC_GBR_QOS_INFO_PRESENT ^*/
} rrc_qos_info_t;

typedef struct _rrc_drb_gbr_qos_info_t
{
    rrc_bitmask_t       bitmask;
#define UECC_LLIM_DRB_CONFIG_QOS_INFO_PRESENT          0x01

    rrm_drb_config_t    drb_config;
    rrc_qos_info_t      qos_info;
    /*^ O, UECC_LLIM_DRB_QOS_INFO_PRESENT ^*/


} rrc_uecc_llim_drb_config_t;


typedef struct _rrc_ambr_info_t
{
    U32                         dl_ambr;
    U32                         ul_ambr;
} rrc_ambr_info_t;

typedef struct _rrc_uecc_llim_tx_bit_rate_t
{
    U32         bit_rate_tx; /*^ M, 0, B, 0, 10000000000 ^*/
} rrc_uecc_llim_tx_bit_rate_t;

typedef struct _rrc_uecc_llim_rx_bit_rate_t
{
    U32         bit_rate_rx; /*^ M, 0, B, 0, 10000000000 ^*/
} rrc_uecc_llim_rx_bit_rate_t;

typedef struct _rrc_uecc_llim_create_drb_entity_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_DRB_CONFIGURE_CIPHERING_PRESENT       0x01
    rrc_uecc_llim_drb_config_t     uecc_llim_drb_config;
    rrc_uecc_llim_drb_s1u_info_t    drb_s1u_info;
    rrc_pdcp_config_drb_ciph_t          configure_ciphering;
/*^ O, UECC_LLIM_DRB_CONFIGURE_CIPHERING_PRESENT ^*/
    
} rrc_uecc_llim_create_drb_entity_t;

/******************************************************************************
*   UECC_LLIM_CREATE_UE_ENTITY_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_create_drb_entity_list_t
{
    rrc_counter_t                       create_drb_entity_counter;
    rrc_uecc_llim_create_drb_entity_t
        create_drb_entity[RRC_UECC_LLIM_MAX_CREATE_DRB];
}rrc_uecc_llim_create_drb_entity_list_t;

typedef struct _rrc_uecc_llim_create_ue_entity_req_t
{
/* eMTC changes start */
    rrc_bitmask_U32_t           bitmask;
    /*^ BITMASK ^*/
/* eMTC changes stop */

#define UECC_LLIM_CREATE_UE_ENTITY_CREATE_SRB2_INFO_PRESENT     0x01
#define UECC_LLIM_CREATE_DRB_ENTITY_LIST_PRESENT                0x02
#define UECC_LLIM_CREATE_UE_ENTITY_REQ_MAC_CONFIG_PRESENT       0x04
#define UECC_LLIM_CREATE_UE_ENTITY_REQ_SPS_CONFIG_PRESENT       0x08
#define UECC_LLIM_CREATE_UE_ENTITY_PHY_CONFIG_DEDICATED_PRESENT 0x10
#define UECC_LLIM_CREATE_UE_ENTITY_REQ_MEAS_CONFIG_PRESENT      0x20
#define UECC_LLIM_CREATE_UE_ENTITY_REQ_HO_TRIGGERED_FLAG        0x40
#define UECC_LLIM_CREATE_UE_ENTITY_UE_AMBR_PRESENT              0x80
    /* HD-FDD start */
#define UECC_LLIM_CREATE_UE_ENTITY_UE_HD_FDD_MODE_PRESENT       0x100
    /* HD-FDD stop */
    /*SPR:5084 start */
#define UECC_LLIM_CREATE_UE_ENTITY_UE_CATEGORY_PRESENT          0x200
    /*SPR:5084 stop */
    /* 36321_CR0538 start */
#define UECC_LLIM_CREATE_UE_ENTITY_ACCESS_STRATUM_REL_PRESENT   0x400
    /* 36321_CR0538 stop */
    /*BUG 604 changes start*/
#define UECC_LLIM_CREATE_UE_ENTITY_HO_TYPE_PRESENT              0x800
    /*BUG 604 changes stop*/
#define UECC_LLIM_CREATE_UE_ENTITY_SCELL_CONFIG_PRESENT         0x1000
#define UECC_LLIM_CREATE_UE_ENTITY_R10_UE_CATEGORY_PRESENT      0x2000
    /*SPR_17121_START*/
#define UECC_LLIM_CREATE_UE_ENTITY_MACI_REQ_PRESENT             0x4000
    /*SPR_17121_END*/
    /*SPR_18803_START*/
#define UECC_LLIM_CREATE_UE_ENTITY_RAT_1_SUPPORT_PRESENT        0x8000
    /*SPR_18803_END*/
    /*SPR_19066_START*/
#define UECC_LLIM_CREATE_UE_ENTITY_UE_INACTIVITY_TIMER_PRESENT  0x10000

    /*SPR_19066_END*/
/* SPR 23209 FIXED START */
#define UECC_LLIM_CREATE_UE_ENTITY_MOCN_OPERATOR_ID_PRESENT       0x400000 
/* SPR 23209 FIXED END */


    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
    rrc_rnti_t              crnti;

    rrc_uecc_llim_create_srb_entity_t       create_srb1_entity;
    rrc_uecc_llim_create_srb_entity_t       create_srb2_entity;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_CREATE_SRB2_INFO_PRESENT ^*/

    rrc_uecc_llim_create_drb_entity_list_t  create_drb_entity_info;
    /*^ O,UECC_LLIM_CREATE_DRB_ENTITY_LIST_PRESENT ^*/

    rrm_mac_config_t                        mac_config;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_REQ_MAC_CONFIG_PRESENT ^*/

    /*SPR_18803_START*/
    U8                           rat_1_support;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_RAT_1_SUPPORT_PRESENT ^*/
    /*RA1_SUPPORT_END*/

    rrc_sps_config_t                        sps_config;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_REQ_SPS_CONFIG_PRESENT ^*/

    rrc_phy_physical_config_dedicated_t     physical_config_dedicated;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_PHY_CONFIG_DEDICATED_PRESENT ^*/
    rrm_meas_config_t                       meas_config;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_REQ_MEAS_CONFIG_PRESENT ^*/

    /*SPR_19066_START*/
    rrc_pdcp_ho_triggered_t                 ho_triggered; 
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_REQ_HO_TRIGGERED_FLAG ^*/
    /*SPR_19066_END*/

    rrc_ambr_info_t                 ue_ambr;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_UE_AMBR_PRESENT ^*/
    /*SPR:5084 start */
    U8                                      ue_category;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_UE_CATEGORY_PRESENT, B, 1, 5 ^*/
    /*SPR:5084 stop */
    /* 36321_CR0538 start */
    U8                                      access_stratum_release;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_ACCESS_STRATUM_REL_PRESENT, H, 0, 2 ^*/
    /* 36321_CR0538 stop */
    /*BUG 604 changes start*/
    U8                                      ho_type;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_HO_TYPE_PRESENT, H, 0, 11 ^*/
    /*rrc_handover_type_et*/
    /*BUG 604 changes stop*/

    rrm_scell_config_t              scell_config;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_SCELL_CONFIG_PRESENT ^*/

    scells_operation_list_t         scells_operation_list[MAX_NUM_CELLS];
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_SCELL_CONFIG_PRESENT ^*/

    U8                              rel10_ue_category;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_R10_UE_CATEGORY_PRESENT ^*/
    /*SPR_17121_START*/
    rrc_mac_i_msg_t              mac_i_req;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_MACI_REQ_PRESENT ^*/
    /*SPR_17121_END*/

    /*SPR_19066_START*/
    rrc_pdcp_ue_inactivity_timer_t  ue_inactivity_timer;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_UE_INACTIVITY_TIMER_PRESENT ^*/
    /*SPR_19066_END*/
    /* SPR 23209 FIXED START */
    U8    operator_id;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_MOCN_OPERATOR_ID_PRESENT ^*/
    /* SPR 23209 FIXED END */
} rrc_uecc_llim_create_ue_entity_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_CREATE_UE_ENTITY_RESP
******************************************************************************/

typedef struct _rrc_uecc_llim_create_srb_entity_error_t
{
    rrc_lc_id_t             lc_id;
    rrc_response_t          response_code;
} rrc_uecc_llim_create_srb_entity_error_t;

typedef struct _rrc_uecc_llim_create_ue_entity_resp_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_CREATE_SRB_ENTITY_ERROR_INFO_PRESENT  0x01
/* SPS Start */
#define UECC_LLIM_CREATE_SRB_ENTITY_SPS_CRNTI_PRESENT   0x02
/* SPS Stop */
    /*SPR_17121_START*/
#define UECC_LLIM_CREATE_UE_ENTITY_RESP_MACI_PRESENT    0x04
    /*SPR_17121_END*/

    rrc_transaction_id_t                transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t                      ue_index;
    rrc_response_t                      response_code;
    rrc_uecc_llim_create_srb_entity_error_t         srb_entity_error_info;
/*^ O, UECC_LLIM_CREATE_SRB_ENTITY_ERROR_INFO_PRESENT ^*/

    rrc_counter_t                       create_drb_status_counter;
    rrc_uecc_llim_create_lc_status_t
        create_drb_status[RRC_UECC_LLIM_MAX_CREATE_DRB];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
    /* SPS Start */
    U16          sps_crnti;
    /*^ O, UECC_LLIM_CREATE_SRB_ENTITY_SPS_CRNTI_PRESENT, B, 61, 65523 ^*/
    /* SPS Stop */

    rrc_bool_et     scell_addition_failed;
    /*SPR_17121_START*/
    rrc_mac_i_t                         mac_i;
    /*^ O, UECC_LLIM_CREATE_UE_ENTITY_RESP_MACI_PRESENT ^*/
    /*SPR_17121_END*/
} rrc_uecc_llim_create_ue_entity_resp_t; /*^ API ^*/

/*REL 1.2:HANDOVER PROCEDURE END*/

/******************************************************************************
*   UECC_LLIM_DELETE_UE_ENTITY_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_delete_ue_entity_req_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
/*BUG 604 changes start*/
    rrc_bitmask_t           bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_DELETE_UE_ENTITY_HO_TYPE_PRESENT   0x01
    U8                      ho_type;
    /*^ O, UECC_LLIM_DELETE_UE_ENTITY_HO_TYPE_PRESENT, H, 0, 11 ^*/
    /*rrc_handover_type_et*/
/*BUG 604 changes stop*/
    /* SPR 21458 Start */
    U8                          l2_instance_reset_flag; /* Indicates the Layer2 presence during cell delete */ 
    /* SPR 21458 End */
} rrc_uecc_llim_delete_ue_entity_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_DELETE_UE_ENTITY_RESP
******************************************************************************/

typedef struct _rrc_uecc_llim_delete_lc_status_t
{
    rrc_lc_id_t     lch_id;    
    U8              status;
} rrc_uecc_llim_delete_lc_status_t;

typedef struct _rrc_uecc_llim_delete_ue_entity_resp_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;

    rrc_response_t          response_code;
/* rrc_return_et + rrc_llim_uecc_response_et */
    
    /*CR 101 changes start*/
    
    rrc_counter_t           delete_lc_status_counter;
    rrc_uecc_llim_delete_lc_status_t delete_lc_status[RRC_UECC_LLIM_MAX_DRB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
    
    /*CR 101 changes stop*/

} rrc_uecc_llim_delete_ue_entity_resp_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_SUSPEND_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_lc_id_state_t
{
    rrc_lc_id_t               lc_id;
    U8                        state;
/*^ M, 0, H, 0, 1 ^*/                       /* rrc_llim_uecc_lc_state_et */
}rrc_lc_id_state_t;

typedef struct _rrc_uecc_llim_lc_id_list_t
{
    U8                        lc_count;
    rrc_lc_id_state_t         lc_id_state[MAX_ERAB_COUNT];
} rrc_uecc_llim_lc_id_list_t;


typedef struct _rrc_uecc_llim_suspend_ue_entity_req_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
    U8                      rlf_detected_flag; /* 1 - for rlf detected */
    rrc_uecc_llim_lc_id_list_t    lc_id_list;
} rrc_uecc_llim_suspend_ue_entity_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_SUSPEND_UE_ENTITY_RESP
******************************************************************************/

typedef struct _rrc_uecc_llim_suspend_ue_entity_resp_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
    rrc_response_t          response_code;
} rrc_uecc_llim_suspend_ue_entity_resp_t; /*^ API ^*/


#define RRC_UECC_LLIM_MAC_CONFIG_VERSION                0x02
#define UECC_LLIM_ADD_LC_REQ_OLD_MAC_CONFIG             0x00
#define UECC_LLIM_ADD_LC_REQ_NEW_MAC_CONFIG             0x01


#define RRC_UECC_LLIM_MEAS_CONFIG_VERSION                0x02
#define UECC_LLIM_ADD_LC_REQ_OLD_MEAS_CONFIG             0x00
#define UECC_LLIM_ADD_LC_REQ_NEW_MEAS_CONFIG             0x01

/******************************************************************************
*   UECC_LLIM_ADD_LC_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_add_lc_req_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_bitmask_t           bitmask;    /*^ BITMASK ^*/

#define UECC_LLIM_ADD_LC_REQ_CREATE_SRB2_INFO_PRESENT           0x01
#define UECC_LLIM_ADD_LC_REQ_OLD_MAC_CONFIG_PRESENT             0x02
#define UECC_LLIM_ADD_LC_REQ_NEW_MAC_CONFIG_PRESENT             0x04
#define UECC_LLIM_ADD_LC_REQ_PHY_CONFIG_DEDICATED_PRESENT       0x08
#define UECC_LLIM_ADD_LC_REQ_UE_CATEGORY_PRESENT                0x10
#define UECC_LLIM_ADD_LC_REQ_OLD_MEAS_CONFIG_PRESENT            0x20
#define UECC_LLIM_ADD_LC_REQ_NEW_MEAS_CONFIG_PRESENT            0x40
#define UECC_LLIM_ADD_LC_REQ_UE_AMBR_PRESENT                    0x80
/* SPS Start */
#define UECC_LLIM_ADD_LC_REQ_SPS_CONFIG_PRESENT                 0x100

/* SPS Stop */
/* HD-FDD start */
#define UECC_LLIM_ADD_LC_REQ_UE_HD_FDD_MODE_PRESENT             0x200
/* HD-FDD stop */
/* 36321_CR0538 start */
#define UECC_LLIM_ADD_LC_REQ_UE_ACCESS_STRATUM_REL_PRESENT      0x400
/* 36321_CR0538 stop */
/*SPR_18803_START*/
#define UECC_LLIM_ADD_LC_REQ_RAT_1_SUPPORT_PRESENT               0x800
/*SPR_18803_END*/
/*SPR_19066_START*/
#define UECC_LLIM_ADD_LC_REQ_UE_INACTIVITY_TIMER_PRESENT        0x1000
/*SPR_19066_END*/
/* SPR 23209 FIXED START */
#define  UECC_LLIM_ADD_LC_REQ_MOCN_OPERATOR_ID_PRESENT          0x4000
/* SPR 23209 FIXED END */
/*NR_DC Code Changes Stop*/
#define UECC_LLIM_ADD_LC_REQ_CREATE_DRB_INFO_PRESENT            0x2000
/*NR_DC Code Changes Stop*/

    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
    rrc_uecc_llim_create_srb_entity_t       create_srb2_entity;
    /*^ O, UECC_LLIM_ADD_LC_REQ_CREATE_SRB2_INFO_PRESENT ^*/

    rrc_counter_t         create_drb_entity_counter;
    rrc_uecc_llim_create_drb_entity_t 
        create_drb_entity[RRC_UECC_LLIM_MAX_DRB];
    /*^ O, UECC_LLIM_ADD_LC_REQ_CREATE_DRB_INFO_PRESENT ^*/

    rrm_mac_config_t
        mac_config[RRC_UECC_LLIM_MAC_CONFIG_VERSION];
    /*^ O, UECC_LLIM_ADD_LC_REQ_MAC_CONFIG_PRESENT ^*/

    rrc_phy_physical_config_dedicated_t
        physical_config_dedicated;
    /*^ O,UECC_LLIM_ADD_LC_REQ_PHY_CONFIG_DEDICATED_PRESENT ^*/

    U8                                      ue_category;
    /*^ O, UECC_LLIM_ADD_LC_REQ_UE_CATEGORY_PRESENT, B, 1, 5 ^*/

    rrm_meas_config_t
        meas_config[RRC_UECC_LLIM_MEAS_CONFIG_VERSION];
    /*^ O, UECC_LLIM_ADD_LC_REQ_MEAS_CONFIG_PRESENT ^*/
    
    rrc_ambr_info_t                 ue_ambr;
    /*^ O, UECC_LLIM_ADD_LC_REQ_UE_AMBR_PRESENT ^*/

    /*SPR_18803_START*/
    U8                           rat_1_support;
    /*^ O, UECC_LLIM_ADD_LC_REQ_RAT_1_SUPPORT_PRESENT ^*/
    /*RA1_SUPPORT_END*/

    /* SPS Start */
    rrc_sps_config_t                        sps_config;
    /*^ O, UECC_LLIM_ADD_LC_REQ_SPS_CONFIG_PRESENT ^*/

    /* SPS Stop */
/* 36321_CR0538 start */
    U8                              access_stratum_release;
    /*^ O, UECC_LLIM_ADD_LC_REQ_UE_ACCESS_STRATUM_REL_PRESENT ^*/
/* 36321_CR0538 stop */

     /* SPR 23209 FIXED START */
    U8                              operator_id;
    /*^ O, UECC_LLIM_ADD_LC_REQ_MOCN_OPERATOR_ID_PRESENT ^*/
     /* SPR 23209 FIXED END */

    /*SPR_19066_START*/
    rrc_pdcp_ue_inactivity_timer_t  ue_inactivity_timer; 
    /*^ O, UECC_LLIM_ADD_LC_REQ_UE_INACTIVITY_TIMER_PRESENT ^*/
    /*SPR_19066_END*/

} rrc_uecc_llim_add_lc_req_t; /*^ API ^*/

/******************************************************************************
 *   UECC_LLIM_ADD_LC_RESP
 ******************************************************************************/

typedef struct _rrc_uecc_llim_add_lc_resp_t
{
    rrc_transaction_id_t    transaction_id;

    rrc_bitmask_t           bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_ADD_LC_RESP_SRB2_STATUS_PRESENT           0x01
    /* SPS Start */
#define UECC_LLIM_ADD_LC_RESP_SPS_CRNTI_PRESENT             0x02
    /* SPS Stop */

    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
    rrc_response_t          response_code;
/* rrc_return_et + rrc_llim_uecc_response_et */
    rrc_uecc_llim_create_lc_status_t    create_srb2_status;
/*^ O, UECC_LLIM_ADD_LC_RESP_SRB2_STATUS_PRESENT ^*/

    rrc_counter_t                       create_drb_status_counter;
    rrc_uecc_llim_create_lc_status_t
        create_drb_status[RRC_UECC_LLIM_MAX_DRB];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

    /* SPS Start */
    U16          sps_crnti;
    /*^ O, UECC_LLIM_ADD_LC_RESP_SPS_CRNTI_PRESENT, B, 61, 65523 ^*/
    /* SPS Stop */


} rrc_uecc_llim_add_lc_resp_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_SRB_DATA_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_srb_data_req_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
    rrc_lc_id_t             lc_id;
    U8                      service_requested;
    rrc_counter_t           data_buffer_length;
    U8                      data_buffer[0];
} rrc_uecc_llim_srb_data_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_SRB_DATA_IND
******************************************************************************/

typedef struct _rrc_uecc_llim_srb_data_ind_t
{
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t  ue_index;
    rrc_lc_id_t     lc_id;
    rrc_counter_t   data_buffer_length;
    U8              data_buffer[0];
} rrc_uecc_llim_srb_data_ind_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_SRB_DATA_STATUS_IND
******************************************************************************/
typedef struct _rrc_uecc_llim_srb_data_status_error_t
{
    U16                     response;
    rrc_counter_t           data_buffer_length;
    U8                      data_buffer[0];
} rrc_uecc_llim_srb_data_status_error_t;

typedef struct _rrc_uecc_llim_srb_data_status_ind_t
{
    rrc_bitmask_t                           bitmask;
#define RRC_UECC_LLIM_DATA_STATUS_ERROR_PRESENT                     0x01

    rrc_transaction_id_t                    transaction_id;
    rrc_cell_index_t                        cell_index;
    rrc_ue_index_t                          ue_index;
    rrc_lc_id_t                             lc_id;
    rrc_response_t                          response_code;
    rrc_uecc_llim_srb_data_status_error_t   srb_data_status_error;
/*^ O, RRC_UECC_LLIM_DATA_STATUS_ERROR_PRESENT ^*/

} rrc_uecc_llim_srb_data_status_ind_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_CCCH_DATA_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_ccch_data_req_t
{
    rrc_cell_index_t        cell_index;
    rrc_rnti_t      rnti;
    rrc_counter_t   data_buffer_length;
    U8              data_buffer[0];
} rrc_uecc_llim_ccch_data_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_CCCH_DATA_IND
******************************************************************************/

typedef struct _rrc_uecc_llim_ccch_data_ind_t
{
    rrc_cell_index_t        cell_index;
    rrc_rnti_t      rnti;

    /* CLPC Start */

    U16         timing_advance;    
    U8          sinr;              

    /* CLPC Stop */
    /* SPR 16149 Fix Start*/
    U8              rnti_found;
    /* SPR 16149 Fix Stop*/

    rrc_counter_t   data_buffer_length;
    U8              data_buffer[0];
} rrc_uecc_llim_ccch_data_ind_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_UE_CON_REJ_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_con_rej_req_t
{
    rrc_cell_index_t        cell_index;
    rrc_rnti_t      crnti;
    rrc_counter_t   data_buffer_length;
    U8              data_buffer[0];
} rrc_uecc_llim_con_rej_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_UE_CON_RE_ESTABLISH_REJ_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_con_re_establish_rej_req_t
{
    rrc_cell_index_t        cell_index;
    rrc_rnti_t      crnti;
    rrc_counter_t   data_buffer_length;
    U8              data_buffer[0];
} rrc_uecc_llim_con_re_establish_rej_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_CONFIGURE_SECURITY_REQ
******************************************************************************/

/* ISA is always done on SRB1 */
typedef struct _rrc_uecc_llim_configure_ciphering_t
{
    rrc_rb_direction_t         rb_direction;        /* rrc_rb_direction_et */
    rrc_pdcp_config_srb_ciph_t     configure_ciphering;
} rrc_uecc_llim_configure_ciphering_t;

typedef struct _rrc_uecc_llim_configure_security_req_t
{
    rrc_bitmask_t             bitmask;            /*^ BITMASK ^*/
#define RRC_UECC_LLIM_SECURITY_CONFIG_INTEGRITY_PROTECTION_PRESENT      0x01
#define RRC_UECC_LLIM_SECURITY_CONFIG_CIPHERING_PRESENT     0x02
    /*SPR_17121_START*/
#define UECC_LLIM_CONFIGURE_SECURITY_MACI_REQ_PRESENT	0x04
    /*SPR_17121_END*/

    rrc_transaction_id_t      transaction_id;
    rrc_cell_index_t          cell_index;
    rrc_ue_index_t            ue_index;
    rrc_pdcp_config_int_t      configure_integrity;
/*^ O, RRC_UECC_LLIM_SECURITY_CONFIG_INT_PRESENT ^*/

    rrc_uecc_llim_configure_ciphering_t configure_ciphering_info;
/*^ O, RRC_UECC_LLIM_SECURITY_CONFIG_CIPH_PRESENT ^*/

    /*SPR_17121_START*/
    rrc_mac_i_msg_t              mac_i_req;
    /*^ O, UECC_LLIM_CONFIGURE_SECURITY_MACI_REQ_PRESENT ^*/
    /*SPR_17121_END*/

} rrc_uecc_llim_configure_security_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_CONFIGURE_SECURITY_RESP
******************************************************************************/

typedef struct _rrc_uecc_llim_configure_security_resp_t
{
    /*SPR_17121_START*/
    rrc_bitmask_t             bitmask;            /*^ BITMASK ^*/
#define UECC_LLIM_CONFIGURE_SECURITY_MACI_RESP_PRESENT	0x01
    /*SPR_17121_END*/
    rrc_transaction_id_t      transaction_id;
    rrc_cell_index_t          cell_index;
    rrc_ue_index_t            ue_index;
    rrc_response_t            response_code;
    /*SPR_17121_START*/
    rrc_mac_i_t                         mac_i;
    /*^ O, UECC_LLIM_CONFIGURE_SECURITY_MACI_RESP_PRESENT ^*/
    /*SPR_17121_END*/
/* rrc_return_et + rrc_llim_uecc_response_et */

} rrc_uecc_llim_configure_security_resp_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_S1U_ERROR_IND
******************************************************************************/

typedef struct _rrc_uecc_llim_s1u_error_ind_t
{
    rrc_cell_index_t          cell_index;
    rrc_ue_index_t  ue_index;
    rrc_lc_id_t     lc_id;
} rrc_uecc_llim_s1u_error_ind_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_S1U_PATH_FAILURE_IND
******************************************************************************/

typedef struct _rrc_uecc_llim_s1u_path_failure_ind_t
{
    rrc_cell_index_t          cell_index;
    U8              ip_address_octet_size; /*^ M, 0, B, 4, 20 ^*/
    /*lipa start*/
    U8              ip_address[RRC_MAX_IP_ADDR_LEN_IPV6];
                    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    /* lipa end*/
} rrc_uecc_llim_s1u_path_failure_ind_t; /*^ API ^*/

/* AK Start */

/******************************************************************************
*   UECC_LLIM_S1U_PATH_SUCCESS_IND
******************************************************************************/

typedef struct _rrc_uecc_llim_s1u_path_success_ind_t
{
    rrc_bitmask_t             bitmask;            /*^ BITMASK ^*/
    rrc_cell_index_t          cell_index;
} rrc_uecc_llim_s1u_path_success_ind_t; /*^ API ^*/

/* AK Stop */

/*REL 1.2:HANDOVER PROCEDURE START*/

/******************************************************************************
*   UECC-LLIM lc sn hfn status IE
******************************************************************************/

#define MAX_UL_PDCP_SDU                 512

typedef struct _rrc_uecc_llim_receive_status_of_ul_pdcp_sdu_t
{
    U8              receive_status_of_ul_pdcp_sdu[MAX_UL_PDCP_SDU];
} rrc_uecc_llim_receive_status_of_ul_pdcp_sdu_t;


typedef struct _rrc_uecc_llim_sn_hfn_count_value_t
{
    U16            sn_count;
    U32            hfn_count;
} rrc_uecc_llim_sn_hfn_count_value_t;


typedef struct _rrc_uecc_llim_lc_sn_hfn_status_t
{
    rrc_bitmask_t                       bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_RECEIVE_STATUS_OF_UL_PDCP_SDU_PRESENT  0x01

    rrc_lc_id_t                         lc_id;
    rrc_uecc_llim_sn_hfn_count_value_t  ul_count_value;
    rrc_uecc_llim_sn_hfn_count_value_t  dl_count_value;

    rrc_uecc_llim_receive_status_of_ul_pdcp_sdu_t  status_of_ul_pdcp_sdu;
/*^ O, UECC_LLIM_RECEIVE_STATUS_OF_UL_PDCP_SDU_PRESENT ^*/
} rrc_uecc_llim_lc_sn_hfn_status_t;

/******************************************************************************
*   UECC_LLIM_SN_HFN_STATUS_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_sn_hfn_status_req_t
{
    rrc_transaction_id_t      transaction_id;
    rrc_cell_index_t          cell_index;
    rrc_ue_index_t            ue_index;
    rrc_bool_t                sn_hfn_required;
    /* lossy ho changes start */
    rrc_counter_t             lossy_drb_counter;
    rrc_lc_id_t               lossy_lc_id[RRC_UECC_LLIM_MAX_DRB];
    /* lossy ho changes stop */
} rrc_uecc_llim_sn_hfn_status_req_t; /*^ API ^*/


/******************************************************************************
*   UECC_LLIM_SN_HFN_STATUS_RESP
******************************************************************************/

typedef struct _rrc_uecc_llim_lc_sn_hfn_status_list_t
{
    U8                                  count;
    rrc_uecc_llim_lc_sn_hfn_status_t    sn_hfn_status[MAX_ERAB_COUNT];
} rrc_uecc_llim_lc_sn_hfn_status_list_t;


typedef struct _rrc_uecc_llim_sn_hfn_status_resp_t
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define  UECC_LLIM_SN_HFN_STATUS_PRESENT  0x01
    rrc_transaction_id_t      transaction_id;
    rrc_cell_index_t          cell_index;
    rrc_ue_index_t            ue_index;
    rrc_response_t            response_code;
/* rrc_return_et + rrc_llim_uecc_response_et */

    rrc_uecc_llim_lc_sn_hfn_status_list_t    sn_hfn_status_list;
/*^ O, UECC_LLIM_SN_HFN_STATUS_PRESENT ^*/
} rrc_uecc_llim_sn_hfn_status_resp_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_DATA_BUFFER_STOP_IND
*******************************************************************************/

typedef struct _rrc_uecc_llim_data_buffer_stop_ind_t
{
    rrc_transaction_id_t      transaction_id;
    rrc_cell_index_t          cell_index;
    rrc_ue_index_t            ue_index;
} rrc_uecc_llim_data_buffer_stop_ind_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_SN_HFN_STATUS_IND
*******************************************************************************/

typedef struct _rrc_uecc_llim_sn_hfn_status_ind_t
{
    rrc_transaction_id_t                transaction_id;
    rrc_cell_index_t                    cell_index;
    rrc_ue_index_t                      ue_index;
    U8                                  count ;      /* Num of ERABs */
    rrc_uecc_llim_lc_sn_hfn_status_t    sn_hfn_status[MAX_ERAB_COUNT];
} rrc_uecc_llim_sn_hfn_status_ind_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_MAC_I_REQ
*******************************************************************************/
typedef struct _rrc_uecc_llim_message_t
{
    U32                       cell_identity;
    rrc_counter_t             num_of_bytes;
    U8                        encodedVarShortMac[UECC_LLIM_MAX_BUF_LEN];
} rrc_uecc_llim_message_t;
typedef struct _rrc_uecc_llim_mac_i_req_t
{
    rrc_transaction_id_t                transaction_id;
    rrc_cell_index_t                    cell_index;
    rrc_ue_index_t                      ue_index;
    rrc_lc_id_t                         lc_id;
    rrc_rb_direction_t                  rb_direction; 
    rrc_rnti_t                          rnti;
    rrc_counter_t                       message_count;
    rrc_uecc_llim_message_t             message[MAX_NO_OF_MESSAGES];
} rrc_uecc_llim_mac_i_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_MAC_I_RESP
*******************************************************************************/

typedef struct _rrc_uecc_llim_mac_i_t
{
    rrc_bitmask_t                       bitmask;
#define RRC_UECC_LLIM_ENCODED_MAC_I_PRESENT  0x01
    rrc_response_t                      response_code;/* rrc_return_et */
    U32                                 cell_identity;
    U32                                 encoded_mac_i;
/*^ O, RRC_UECC_LLIM_ENCODED_MAC_I_PRESENT ^*/
} rrc_uecc_llim_mac_i_t;

typedef struct _rrc_uecc_llim_msg_auth_code_list_t
{
    U8                                  mac_i_count;
    rrc_uecc_llim_mac_i_t               mac_i[MAX_NO_OF_MESSAGES];
} rrc_uecc_llim_msg_auth_code_list_t;

typedef struct _rrc_uecc_llim_mac_i_resp_t
{
    rrc_bitmask_t                       bitmask;
#define RRC_UECC_LLIM_MSG_AUTH_CODE_LIST_PRESENT  0x01
    rrc_transaction_id_t                transaction_id;
    rrc_cell_index_t                    cell_index;
    rrc_ue_index_t                      ue_index;
    rrc_response_t                      response_code;/* rrc_return_et */
/* rrc_return_et + rrc_llim_uecc_response_et */
    rrc_uecc_llim_msg_auth_code_list_t  msg_auth_code_list;
/*^ O, RRC_UECC_LLIM_MSG_AUTH_CODE_LIST_PRESENT ^*/
} rrc_uecc_llim_mac_i_resp_t; /*^ API ^*/

/*SPR_19066_START*/
/******************************************************************************
*   UECC_LLIM_HO_PREP_INFO_REQ
*******************************************************************************/
typedef struct _rrc_uecc_llim_mac_i_msg_req_t
{
    rrc_lc_id_t                         lc_id;
    rrc_rb_direction_t                  rb_direction; 
    rrc_rnti_t                          rnti;
    rrc_counter_t                       message_count;
    rrc_uecc_llim_message_t             message[MAX_NO_OF_MESSAGES];
} rrc_uecc_llim_mac_i_msg_req_t;

typedef struct _rrc_uecc_llim_ho_prep_info_req_t
{
    rrc_bitmask_t                       bitmask;
#define RRC_UECC_LLIM_MACI_MSG_REQ_PRESENT                 0x01
#define RRC_UECC_LLIM_HO_PREP_INACTIVE_TIME_REQ_PRESENT    0x02
    rrc_transaction_id_t                transaction_id;
    rrc_cell_index_t                    cell_index;
    rrc_ue_index_t                      ue_index;
    rrc_uecc_llim_mac_i_msg_req_t       mac_i_msg_req;
/*^ O, RRC_UECC_LLIM_MACI_MSG_REQ_PRESENT ^*/
    rrc_ue_inactivity_time_val_t        ue_inactive_time_val; 
/*^ O, RRC_UECC_LLIM_HO_PREP_INACTIVE_TIME_REQ_PRESENT ^*/
} rrc_uecc_llim_ho_prep_info_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_HO_PREP_INFO_RESP
*******************************************************************************/

typedef struct _rrc_uecc_llim_ho_prep_info_resp_t
{
    rrc_bitmask_t                       bitmask;
#define RRC_UECC_LLIM_MACI_MSG_RESP_PRESENT                 0x01
#define RRC_UECC_LLIM_HO_PREP_INACTIVE_TIME_RESP_PRESENT    0x02
    rrc_transaction_id_t                transaction_id;
    rrc_cell_index_t                    cell_index;
    rrc_ue_index_t                      ue_index;
    rrc_response_t                      response_code;/* rrc_return_et */
/* rrc_return_et + rrc_llim_uecc_response_et */
    rrc_uecc_llim_msg_auth_code_list_t  msg_auth_code_list;
/*^ O, RRC_UECC_LLIM_MACI_MSG_RESP_PRESENT ^*/
    rrc_ue_inactive_time_t              ue_inactivity_time_resp; 
/*^ O, RRC_UECC_LLIM_HO_PREP_INACTIVE_TIME_RESP_PRESENT ^*/
} rrc_uecc_llim_ho_prep_info_resp_t; /*^ API ^*/
/*SPR_19066_END*/

/******************************************************************************
*   UECC-LLIM s1u seq num IE
******************************************************************************/
typedef struct _rrc_uecc_llim_s1u_seq_num_t
{
    U16 ul_sequence_number;                     /*^ M, 0, H, 0, 65535 ^*/
    U16 dl_sequence_number;                     /*^ M, 0, H, 0, 65535 ^*/
} rrc_uecc_llim_s1u_seq_num_t;

/******************************************************************************
*   UECC_LLIM_S1U_RECONFIGURE_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_s1u_rel_sap_req_t
{
    rrc_lc_id_t                         lc_id;
    U8                                  tunnel_count;
    rrc_uecc_llim_s1u_release_tunnel_info_t
        release_tunnel_info[RRC_S1U_MAX_TUNNELS_PER_LC];
} rrc_uecc_llim_s1u_rel_sap_req_t;

typedef struct _rrc_uecc_llim_s1u_rel_sap_req_list_t
{
    rrc_counter_t                       num_rel_sap_req;
    rrc_uecc_llim_s1u_rel_sap_req_t     rel_sap_req[RRC_S1U_MAX_NUM_LC];
} rrc_uecc_llim_s1u_rel_sap_req_list_t;

typedef struct _rrc_uecc_llim_s1u_reordering_reqd_t
{
    U8  reordering_reqd;  /*^ M, 0, H, 0, 1 ^*/
} rrc_uecc_llim_s1u_reordering_reqd_t;

typedef struct _rrc_uecc_llim_s1u_seq_disable_t
{
    U8  disable;
/*^ M, 0, H, 0, 1 ^*/
} rrc_uecc_llim_s1u_seq_disable_t;

typedef struct _rrc_uecc_llim_s1u_pdcp_ext_t
{
    U8  pdcp_ext_flag;
/*^ M, 0, H, 0, 1 ^*/
} rrc_uecc_llim_s1u_pdcp_ext_t;

typedef struct _rrc_uecc_llim_s1u_qos_profile_t
{
    U8              allocation_retention_priority;
    rrc_counter_t   qos_profile_data_length;
/*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/
    U8              qos_profile_data[RRC_S1U_MAX_QOS_PROFILE_DATA];
/*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/
} rrc_uecc_llim_s1u_qos_profile_t;
typedef struct _rrc_uecc_llim_s1u_setup_sap_req_t
{
    rrc_bitmask_t                bitmask;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_UECC_LLIM_S1U_BUFFER_IND_PRESENT         0x01
#define RRC_UECC_LLIM_S1U_TEID_SELF_PRESENT          0x02
#define RRC_UECC_LLIM_S1U_SEQ_NUM_PRESENT            0x04
#define RRC_UECC_LLIM_S1U_SEQ_FLAG_PRESENT           0x08
#define RRC_UECC_LLIM_S1U_REORDERING_REQ_PRESENT     0x10
#define RRC_UECC_LLIM_S1U_PDCP_EXT_FLAG_PRESENT      0x20
#ifdef ENDC_ENABLED
/*OPTION3X Changes Start*/
#define RRC_UECC_LLIM_S1U_DC_BEARER_TYPE_PRESENT     0x40
#define RRC_UECC_LLIM_S1U_PDCP_SN_SIZE_ENDC_PRESENT  0x80
#define RRC_UECC_LLIM_S1U_RLC_MODE_ENDC_PRESENT      0x100
/*OPTION3X Changes Stop*/
#endif
    rrc_lc_id_t                         lc_id;          /*^ M, 0, B, 3, 10 ^*/
    U32                                 qos_id;
    U8                                  tunnel_count;
    rrc_uecc_llim_s1u_tunnel_info_t     tunnel_info[RRC_S1U_MAX_TUNNELS_PER_LC];
    rrc_uecc_llim_s1u_qos_profile_t     qos_profile;
    U8                                  buffer_ind;
/*^ O, 8, B, 0, 1 ^*/ /* rrc_llim_buffer_ind_et */
    rrc_gtp_teid_t                      teid_self;
/*^ O, RRC_UECC_LLIM_S1U_TEID_SELF_PRESENT ^*/

    rrc_uecc_llim_s1u_seq_num_t        seq_num;
/*^ O, RRC RRC_UECC_LLIM_S1U_SEQ_NUM_PRESENT ^*/

    rrc_uecc_llim_s1u_seq_disable_t    seq_flag;
/*^ O, RRC RRC_UECC_LLIM_S1U_SEQ_FLAG_PRESENT ^*/

    rrc_uecc_llim_s1u_reordering_reqd_t   reordering_reqd;
/*^ O, RRC_UECC_LLIM_S1U_REORDERING_REQ_PRESENT ^*/

    rrc_uecc_llim_s1u_pdcp_ext_t      pdcp_ext;
/*^ O, RRC RRC_UECC_LLIM_S1U_PDCP_EXT_FLAG_PRESENT ^*/
#ifdef ENDC_ENABLED
/*OPTION3X Changes Start*/
    /* coverity fix 277523 */
    /*rrc_dc_bearer_type_et       dc_bearer_type;*/
    U8                            dc_bearer_type;
    /* coverity fix 277523 */
/*^ O, RRC_UECC_LLIM_S1U_DC_BEARER_TYPE_PRESENT^*/
    U8                          pdcp_sn_size_endc;
/*^ O, RRC_UECC_LLIM_S1U_PDCP_SN_SIZE_ENDC_PRESENT^*/

    U8                          rlc_mode_endc;
/*^ O, RRC_UECC_LLIM_S1U_RLC_MODE_ENDC_PRESENT^*/
/*OPTION3X Changes Stop*/
#endif
} rrc_uecc_llim_s1u_setup_sap_req_t;

typedef struct _rrc_uecc_llim_s1u_setup_sap_req_list_t
{
    rrc_counter_t                       num_setup_sap_req;
    rrc_uecc_llim_s1u_setup_sap_req_t   setup_sap_req[RRC_S1U_MAX_NUM_LC];
} rrc_uecc_llim_s1u_setup_sap_req_list_t;

typedef struct _rrc_uecc_llim_s1u_reconfigure_req_t
{
    rrc_bitmask_t                       bitmask;
#define RRC_UECC_LLIM_S1U_SETUP_SAP_REQ_LIST_PRESENT    0x01
#define RRC_UECC_LLIM_S1U_REL_SAP_REQ_LIST_PRESENT      0x02

    rrc_transaction_id_t        transaction_id;
    rrc_cell_index_t            cell_index;

    rrc_ue_index_t              ue_index;

    rrc_uecc_llim_s1u_setup_sap_req_list_t     setup_sap_req_list;
/*^ O, RRC_UECC_LLIM_S1U_SETUP_SAP_REQ_LIST_PRESENT ^*/

    rrc_uecc_llim_s1u_rel_sap_req_list_t       rel_sap_req_list;
/*^ O, RRC_UECC_LLIM_S1U_REL_SAP_REQ_LIST_PRESENT ^*/
} rrc_uecc_llim_s1u_reconfigure_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_S1U_RECONFIGURE_CNF
******************************************************************************/

typedef struct _rrc_uecc_llim_s1u_rel_sap_cnf_t
{
    rrc_bitmask_t               bitmask;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_UECC_LLIM_S1U_RELEASE_TUNNEL_INFO_PRESENT      0x01
#define RRC_UECC_LLIM_S1U_CNF_SEQ_NUM_PRESENT              0x02
    rrc_lc_id_t                 lc_id;          /*^ M, 0, B, 3, 10 ^*/
    U8                          cause;          /* rrc_s1u_cause_et */
    U8                          release_tunnel_count;
    rrc_uecc_llim_s1u_release_tunnel_info_t
        release_tunnel_info[RRC_S1U_MAX_TUNNELS_PER_LC];
/*^ O, RRC_UECC_LLIM_S1U_RELEASE_TUNNEL_INFO_PRESENT ^*/

    rrc_uecc_llim_s1u_seq_num_t        seq_num;
/*^ O, RRC_UECC_LLIM_S1U_REL_CNF_SEQ_NUM_PRESENT ^*/
    /* present if cause contains success */
} rrc_uecc_llim_s1u_rel_sap_cnf_t;

typedef struct _rrc_uecc_llim_s1u_rel_sap_cnf_list_t
{
    rrc_counter_t                       num_rel_sap_cnf;
    rrc_uecc_llim_s1u_rel_sap_cnf_t     rel_sap_cnf[RRC_S1U_MAX_NUM_LC];
}rrc_uecc_llim_s1u_rel_sap_cnf_list_t;

typedef struct _rrc_uecc_llim_s1u_setup_sap_cnf_t
{
    rrc_bitmask_t               bitmask;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define UECC_LLIM_S1U_TUNNEL_INFO_CNF_PRESENT    0x01
#define UECC_LLIM_S1U_CNF_SEQ_NUM_PRESENT    0x02
    rrc_lc_id_t                 lc_id;          /*^ M, 0, B, 3, 10 ^*/
    U8                          cause;          /* rrc_s1u_cause_et */
    U8                          tunnel_info_cnf_count;
    rrc_uecc_llim_s1u_tunnel_info_cnf_t
        tunnel_info_cnf[RRC_S1U_MAX_TUNNELS_PER_LC];
/*^ O, UECC_LLIM_S1U_TUNNEL_INFO_CNF_PRESENT  ^*/
    rrc_uecc_llim_s1u_seq_num_t        seq_num;
/*^ O, UECC_LLIM_S1U_CNF_SEQ_NUM_PRESENT  ^*/ /* present if cause contains
success */
} rrc_uecc_llim_s1u_setup_sap_cnf_t;

typedef struct _rrc_uecc_llim_s1u_setup_sap_cnf_list_t
{
    rrc_counter_t                       num_setup_sap_cnf;
    rrc_uecc_llim_s1u_setup_sap_cnf_t   setup_sap_cnf[RRC_S1U_MAX_NUM_LC];
} rrc_uecc_llim_s1u_setup_sap_cnf_list_t;

typedef struct _rrc_uecc_llim_s1u_reconfigure_cnf_t
{
    rrc_bitmask_t                       bitmask;
#define RRC_UECC_LLIM_S1U_SETUP_SAP_CNF_PRESENT    0x01
#define RRC_UECC_LLIM_S1U_REL_SAP_CNF_PRESENT      0x02
    rrc_transaction_id_t      transaction_id;
    rrc_cell_index_t          cell_index;
    rrc_ue_index_t            ue_index;
    rrc_response_t            response_code;

    rrc_uecc_llim_s1u_setup_sap_cnf_list_t     setup_sap_cnf_list;
/*^ O, RRC_UECC_LLIM_S1U_SETUP_SAP_CNF_PRESENT ^*/

    rrc_uecc_llim_s1u_rel_sap_cnf_list_t       rel_sap_cnf_list;
/*^ O, RRC_UECC_LLIM_S1U_REL_SAP_CNF_PRESENT ^*/
} rrc_uecc_llim_s1u_reconfigure_cnf_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_HO_RACH_RESOURCE_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_intra_enb_ho_req_t
{
    U8     enable_intra_enb_ho;   /*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */
} rrc_uecc_llim_intra_enb_ho_req_t;

typedef struct _rrc_uecc_llim_ho_rach_resource_req_t
{
    rrc_bitmask_t             bitmask;
#define RRC_UECC_LLIM_INTRA_ENB_HO_REQ_PRESENT    0x01
    rrc_transaction_id_t      transaction_id;
    rrc_cell_index_t          cell_index;
    rrc_ue_index_t            ue_index;

    rrc_uecc_llim_intra_enb_ho_req_t      intra_enb_ho_req;
/*^ O, RRC_UECC_LLIM_INTRA_ENB_HO_REQ_PRESENT ^*/
    
} rrc_uecc_llim_ho_rach_resource_req_t;  /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_HO_RACH_RESOURCE_RESP
******************************************************************************/

typedef struct _rrc_uecc_llim_preamble_info_t
{
    U8                        preamble_idx ;
    U8                        prach_mask_idx;
} rrc_uecc_llim_preamble_info_t;

typedef struct _rrc_uecc_llim_ho_rach_resource_resp_t
{
    rrc_bitmask_t             bitmask;
#define RRC_UECC_LLIM_PREAMBLE_INFO_PRESENT    0x01
#define RRC_UECC_LLIM_CRNTI_PRESENT            0x02
    rrc_transaction_id_t      transaction_id;
    rrc_cell_index_t          cell_index;
    rrc_ue_index_t            ue_index;
    rrc_response_t            response_code;

    rrc_rnti_t                crnti;
/*^ O, RRC_UECC_LLIM_PREAMBLE_INFO_PRESENT ^*/
    rrc_uecc_llim_preamble_info_t       preamble_info;
/*^ O, RRC_UECC_LLIM_CRNTI_PRESENT ^*/
} rrc_uecc_llim_ho_rach_resource_resp_t;  /*^ API ^*/
/******************************************************************************
*   UECC_LLIM_HO_REL_RACH_RESOURCE_IND
******************************************************************************/

typedef struct _rrc_uecc_llim_ho_rach_rel_resource_ind_t
{
    rrc_transaction_id_t                transaction_id;
    rrc_bitmask_t             bitmask;
#define RRC_UECC_LLIM_HO_RACH_REL_RESOURCE_IND_CRNTI_PRESENT    0x01

    rrc_cell_index_t          cell_index;
    rrc_ue_index_t            ue_index; /*^ M ^*/
    rrc_rnti_t                crnti;   /*^ O ^*/
} rrc_uecc_llim_ho_rach_rel_resource_ind_t;  /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_UE_INACTIVE_TIME_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_ue_inactive_time_req_t
{
    rrc_transaction_id_t      transaction_id;
    rrc_cell_index_t          cell_index;
    rrc_ue_index_t            ue_index;
} rrc_uecc_llim_ue_inactive_time_req_t;  /*^ API ^*/


/******************************************************************************
*   UECC_LLIM_UE_INACTIVE_TIME_RESP
******************************************************************************/

typedef struct _rrc_uecc_llim_ue_inactive_time_resp_t
{
    rrc_bitmask_t             bitmask;
#define RRC_UECC_LLIM_INACTIVE_TIME_VAL_PRESENT    0x01
    rrc_transaction_id_t      transaction_id;
    rrc_cell_index_t          cell_index;
    rrc_ue_index_t            ue_index;
    rrc_response_t            response_code;
    U16                       inactTimeVal;/* rrc_ue_inactive_time_et*/
/*^ O, RRC_UECC_LLIM_INACTIVE_TIME_VAL_PRESENT ^*/
} rrc_uecc_llim_ue_inactive_time_resp_t;  /*^ API ^*/
/******************************************************************************
*   UECC_LLIM_RE_ESTABLISH_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_uecc_llim_re_establish_ue_entity_req_t
{
    rrc_transaction_id_t          transaction_id;
    rrc_cell_index_t              cell_index;
    rrc_ue_index_t                ue_index;
    rrc_uecc_llim_lc_id_list_t    lc_id_list;
    /* SPR 18926 Fix Start */
    U8                            rlc_reestablish_required;
    /* SPR 18926 Fix Stop */
} rrc_uecc_llim_re_establish_ue_entity_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_RE_ESTABLISH_UE_ENTITY_RESP
******************************************************************************/

typedef struct _rrc_uecc_llim_re_establish_rb_entity_error_t
{
    rrc_lc_id_t               lc_id;
    U32  response;   
} rrc_uecc_llim_re_establish_rb_entity_error_t;

typedef struct _rrc_uecc_llim_re_establish_ue_entity_resp_t
{
    rrc_bitmask_t                 bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_RE_ESTABLISH_UE_ENTITY_RESP_RB_ERROR_INFO_PRESENT  0x01
    rrc_transaction_id_t          transaction_id;
    rrc_cell_index_t              cell_index;
    rrc_ue_index_t                ue_index;
    rrc_response_t                response_code;

    rrc_counter_t                 rb_error_counter;
    rrc_uecc_llim_re_establish_rb_entity_error_t     rb_error_info[RRC_MAX_NUM_RB];
/*^ O, UECC_LLIM_RE_ESTABLISH_UE_ENTITY_RESP_RB_ERROR_INFO_PRESENT ^*/
} rrc_uecc_llim_re_establish_ue_entity_resp_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_RESUME_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_uecc_llim_resume_rb_entity_list_t
{
    U8                        lc_count;
    rrc_lc_id_t               lc_id[RRC_MAX_NUM_RB];
} rrc_uecc_llim_resume_rb_entity_list_t;

typedef struct _rrc_uecc_llim_resume_ue_entity_req_t
{
    rrc_transaction_id_t          transaction_id;
    rrc_cell_index_t              cell_index;
    rrc_ue_index_t                ue_index;
    rrc_rb_direction_t            resume_direction;
    rrc_uecc_llim_resume_rb_entity_list_t     rb_info;
} rrc_uecc_llim_resume_ue_entity_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_RESUME_UE_ENTITY_RESP
******************************************************************************/
typedef struct _rrc_uecc_llim_resume_rb_entity_error_t
{
    rrc_lc_id_t               lc_id;
    U32  response;   /*  rrc_llim_uecc_response_et */
} rrc_uecc_llim_resume_rb_entity_error_t;

typedef struct _rrc_uecc_llim_resume_ue_entity_resp_t
{
    rrc_bitmask_t                 bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_RESUME_UE_ENTITY_RESP_RB_ERROR_INFO_PRESENT  0x01
    rrc_transaction_id_t          transaction_id;
    rrc_cell_index_t              cell_index;
    rrc_ue_index_t                ue_index;
    rrc_response_t                response_code;

    rrc_counter_t                 rb_error_counter;
    rrc_uecc_llim_resume_rb_entity_error_t     rb_error_info[RRC_MAX_NUM_RB];
/*^ O, UECC_LLIM_RESUME_UE_ENTITY_RESP_RB_ERROR_INFO_PRESENT ^*/
} rrc_uecc_llim_resume_ue_entity_resp_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_RECONFIG_SECURITY_REQ
******************************************************************************/
typedef struct _rrc_uecc_llim_reconfig_security_req_t
{
    rrc_bitmask_t               bitmask;            /*^ BITMASK ^*/
#define RRC_UECC_LLIM_RECONFIG_SECURITY_REQ_RECONFIG_INTEGRITY_PRESENT        0x01
#define RRC_UECC_LLIM_RECONFIG_SECURITY_REQ_RECONFIG_SRB_CIPHERING_PRESENT    0x02
#define RRC_UECC_LLIM_RECONFIG_SECURITY_REQ_RECONFIG_DRB_CIPHERING_PRESENT    0x04
#define RRC_UECC_LLIM_RECONFIG_SECURITY_REQ_CONFIGURE_SECURITY_OFFSET_PRESENT 0x08
    /*SPR_17121_START*/
#define UECC_LLIM_RECONFIGURE_SECURITY_MACI_REQ_PRESENT 0x10
    /*SPR_17121_END*/
    rrc_transaction_id_t        transaction_id;
    rrc_cell_index_t            cell_index;
    rrc_ue_index_t              ue_index;

    rrc_pdcp_config_int_t       reconfig_integrity;
/*^ O, RRC_UECC_LLIM_RECONFIG_SECURITY_REQ_RECONFIG_INTEGRITY_PRESENT ^*/

    rrc_pdcp_config_srb_ciph_t      reconfig_srb_ciphering;
/*^ O, RRC_UECC_LLIM_RECONFIG_SECURITY_REQ_RECONFIG_SRB_CIPHERING_PRESENT ^*/

    rrc_pdcp_config_drb_ciph_t      reconfig_drb_ciphering;
/*^ O, RRC_UECC_LLIM_RECONFIG_SECURITY_REQ_RECONFIG_DRB_CIPHERING_PRESENT ^*/

    U8                              configure_security_offset;
/*^ O, RRC_UECC_LLIM_RECONFIG_SECURITY_REQ_CONFIGURE_SECURITY_OFFSET_PRESENT ^*/
    /*SPR_17121_START*/
    rrc_mac_i_msg_t              mac_i_req;
    /*^ O, UECC_LLIM_RECONFIGURE_SECURITY_MACI_REQ_PRESENT ^*/
    /*SPR_17121_END*/

} rrc_uecc_llim_reconfig_security_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_RECONFIG_SECURITY_RESP
******************************************************************************/
typedef struct _rrc_uecc_llim_reconfig_security_resp_t
{
    /*SPR_17121_START*/
    rrc_bitmask_t             bitmask;            /*^ BITMASK ^*/
#define UECC_LLIM_RECONFIGURE_SECURITY_MACI_RESP_PRESENT	0x01
    /*SPR_17121_END*/
    rrc_transaction_id_t      transaction_id;
    rrc_cell_index_t          cell_index;
    rrc_ue_index_t            ue_index;
    rrc_response_t            response_code; /* rrc_response_et */
    /*SPR_17121_START*/
    rrc_mac_i_t                         mac_i;
    /*^ O, UECC_LLIM_RECONFIGURE_SECURITY_MACI_RESP_PRESENT ^*/
    /*SPR_17121_END*/
} rrc_uecc_llim_reconfig_security_resp_t; /*^ API ^*/
/******************************************************************************
*   UECC_LLIM_S1U_END_MARKER_IND
******************************************************************************/
#define UECC_LLIM_S1U_END_MARKER_TRANSPORT_ADDR_PRESENT 0x01
typedef struct _rrc_uecc_llim_s1u_end_marker_ind_t
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
    rrc_cell_index_t          cell_index;
    rrc_ue_index_t            ue_index;
    rrc_lc_id_t               lc_id;
    rrc_gtp_teid_t            teid_self;
    rrc_uecc_llim_s1u_gsn_addr_t        transport_addr;
/*^ O, UECC_LLIM_S1U_END_MARKER_TRANSPORT_ADDR_PRESENT ^*/
} rrc_uecc_llim_s1u_end_marker_ind_t; /*^ API ^*/

/*REL 1.2:HANDOVER PROCEDURE END*/

/* Start: E-RAB Modify*/
/******************************************************************************
*   UECC-LLIM Modify DRB entity IE
******************************************************************************/


/******************************************************************************
*   UECC-LLIM Modify DRB Old and New configuration version
******************************************************************************/
typedef struct _rrc_uecc_llim_modify_drb_version
{
    rrc_bitmask_t           bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_MODIFY_LC_REQ_MAC_CONFIG_PRESENT             0x01
#define UECC_LLIM_MODIFY_LC_REQ_PHY_CONFIG_DEDICATED_PRESENT   0x02
#define UECC_LLIM_MODIFY_LC_REQ_MEAS_GAP_CONFIG_PRESENT            0x04
#define UECC_LLIM_MODIFY_LC_REQ_UE_AMBR_PRESENT                0x08
#define UECC_LLIM_MODIFY_LC_REQ_SCELL_CONFIG_PRESENT           0x10

    rrc_counter_t                   drb_counter;
    
    rrc_uecc_llim_drb_config_t      uecc_llim_drb_config[RRC_UECC_LLIM_MAX_DRB]; 
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
    rrm_mac_config_t             mac_config;                
/*^ O, UECC_LLIM_MODIFY_LC_REQ_MAC_CONFIG_PRESENT ^*/

    rrc_phy_physical_config_dedicated_t physical_config_dedicated; 
/*^ O, UECC_LLIM_MODIFY_LC_REQ_PHY_CONFIG_DEDICATED_PRESENT ^*/

     meas_gap_config_t                 meas_gap_config;
    /*^ O, UECC_LLIM_MODIFY_LC_REQ_MEAS_GAP_CONFIG_PRESENT ^*/
    
    rrc_ambr_info_t                 ue_ambr;
    /*^ O, UECC_LLIM_MODIFY_LC_REQ_UE_AMBR_PRESENT ^*/

    /*CA_START*/
    rrm_scell_config_t              scell_config;
    /*^ O, UECC_LLIM_MODIFY_LC_REQ_SCELL_CONFIG_PRESENT ^*/
    /*CA_STOP*/
    
} rrc_uecc_llim_modify_drb_version_t;

/******************************************************************************
*   UECC_LLIM_MODIFY_LC_REQ
******************************************************************************/
typedef struct _rrc_uecc_llim_modify_lc_req_t
{
    rrc_bitmask_t           bitmask;    /*^ BITMASK ^*/
    /* SPS Start */
#define UECC_LLIM_MODIFY_LC_REQ_SPS_CONFIG_PRESENT        0x01
    /* SPS Stop */
    /*SPR_19066_START*/
    /*CA_START*/
#define UECC_LLIM_MODIFY_LC_REQ_R10_UE_CATEGORY_PRESENT   0x02
    /*CA_STOP*/
#define UECC_LLIM_MODIFY_LC_REQ_UE_INACTIVITY_TIMER_PRESENT 0x04
/*SPR_19066_END*/
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;

    rrc_uecc_llim_modify_drb_version_t  
        modify_drb_entity[RRC_UECC_LLIM_MODIFY_DRB_CONFIG_VERSION];

    /* SPS Start */
    rrc_sps_config_t                        sps_config;
    /*^ O, UECC_LLIM_MODIFY_LC_REQ_SPS_CONFIG_PRESENT ^*/

    /* SPS Stop */
   /*SPR_19066_START*/
    rrc_pdcp_ue_inactivity_timer_t  ue_inactivity_timer; 
    /*^ O, UECC_LLIM_MODIFY_LC_REQ_UE_INACTIVITY_TIMER_PRESENT ^*/
    /*SPR_19066_END*/

    /*CA_START*/
    scells_operation_list_t    scells_operation_list[MAX_NUM_CELLS];

    U8                              rel10_ue_category;
    /*^ O, UECC_LLIM_MODIFY_LC_REQ_R10_UE_CATEGORY_PRESENT ^*/
    U8    add_scell_info_mac_phy;
    /*CA_STOP*/
} rrc_uecc_llim_modify_lc_req_t; 

/******************************************************************************
*   UECC_LLIM_MODIFY_LC_RESP
******************************************************************************/
typedef struct _rrc_uecc_llim_modify_lc_status_t
{
    rrc_lc_id_t     lc_id;
    U32  response;    /* rrc_return_et + rrc_llim_uecc_response_et */
} rrc_uecc_llim_modify_lc_status_t;

typedef struct _rrc_uecc_llim_modify_lc_resp_t
{
    /* SPS Start */
    rrc_bitmask_t           bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_MODIFY_LC_RESP_SPS_CRNTI_PRESENT        0x01
#define UECC_LLIM_MODIFY_LC_RESP_FAIL_CAUSE_PRESENT        0x02
    /* SPS Stop */
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
    
    rrc_response_et          response_code; 

    rrc_counter_t                    modify_drb_status_counter;
    rrc_uecc_llim_modify_lc_status_t modify_drb_status[RRC_UECC_LLIM_MAX_DRB]; 
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
    /* SPS Start */
    U16          sps_crnti;
    /*^ O, UECC_LLIM_MODIFY_LC_RESP_SPS_CRNTI_PRESENT, B, 61, 65523 ^*/
    /* SPS Stop */
    /* CA start */
    U8 fail_cause ;
    /*^ O,  UECC_LLIM_MODIFY_LC_RESP_FAIL_CAUSE_PRESENT*/
    /* rrc_mac_scell_config_error_code_et*/
    /* CA end */
} rrc_uecc_llim_modify_lc_resp_t; /*^ API ^*/  
/* End: E-RAB Modify*/

/* ERB RELEASE COMMAND START */

/******************************************************************************
*   UECC-LLIM Delete DRB entity IE
******************************************************************************/
typedef enum
{
    RLC_CONFIG_AM_MODE,
    RLC_CONFIG_UM_BI_DIR_MODE,
    RLC_CONFIG_UM_UL_UNI_DIR_MODE,
    RLC_CONFIG_UM_DL_UNI_DIR_MODE
} rrc_rlc_lc_mode_et;

typedef struct _rrc_uecc_llim_delete_drb_entity_t
{
    /*OPTION#X Changes Start*/
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_DELETE_DRB_ENTITY_DC_BEARER_TYPE_PRESENT      0x01
    /*OPTION3X Changes Stop*/
    rrc_lc_id_t      lc_id;
    rrc_rlc_lc_mode_et  rlc_mode;
    /*OPTION#X Changes Start*/
    /* coverity fix 277521 */
    /*rrc_dc_bearer_type_et dc_bearer_type;*/
    U8               dc_bearer_type;
    /* coverity fix 277521 */
    /*^ O, UECC_LLIM_DELETE_DRB_ENTITY_DC_BEARER_TYPE_PRESENT ^*/
    /*OPTION3X Changes Stop*/
} rrc_uecc_llim_delete_drb_entity_t;


/******************************************************************************
*   UECC_LLIM_DELETE_LC_REQ
******************************************************************************/
typedef struct _rrc_uecc_llim_delete_lc_req_t
{
    /* SPS Start */
    rrc_bitmask_t           bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_DELETE_LC_REQ_SPS_CONFIG_PRESENT        0x01
    /* SPS Stop */
    /*SPR_19066_START*/
#define UECC_LLIM_DELETE_LC_REQ_UE_INACTIVITY_TIMER_PRESENT 0x02
    /*SPR_19066_END*/
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
    rrc_counter_t           delete_drb_entity_counter;

    rrc_uecc_llim_delete_drb_entity_t        
        delete_drb_entity[RRC_UECC_LLIM_MAX_DRB];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
    /* SPS Start */
    rrc_sps_config_t                        sps_config;
    /*^ O, UECC_LLIM_DELETE_LC_REQ_SPS_CONFIG_PRESENT ^*/

    /* SPS Stop */
    /*SPR_19066_START*/
    rrc_pdcp_ue_inactivity_timer_t  ue_inactivity_timer; 
    /*^ O, UECC_LLIM_DELETE_LC_REQ_UE_INACTIVITY_TIMER_PRESENT ^*/
    /*SPR_19066_END*/
} rrc_uecc_llim_delete_lc_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_DELETE_LC_RESP
******************************************************************************/
/*typedef struct _rrc_uecc_llim_delete_drb_entity_resp_t
{
    rrc_lc_id_t     lc_id;
    rrc_response_t    response; 
} rrc_uecc_llim_delete_drb_entity_resp_t; */
typedef struct _rrc_uecc_llim_delete_drb_entity_resp_t
{
    rrc_lc_id_t     lc_id;
    U32             response; 
} rrc_uecc_llim_delete_drb_entity_resp_t; 

typedef struct
{   
    /* SPS Start */
    rrc_bitmask_t           bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_DELETE_LC_RESP_SPS_CRNTI_PRESENT        0x01
    /* SPS Stop */
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
    rrc_response_et          response_code;
    rrc_counter_t           delete_drb_entity_resp_counter;
    
    rrc_uecc_llim_delete_drb_entity_resp_t        
        delete_drb_entity_resp[RRC_UECC_LLIM_MAX_DRB];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
    
    rrc_counter_t           delete_lc_status_counter;
    rrc_uecc_llim_delete_lc_status_t delete_lc_status[RRC_UECC_LLIM_MAX_DRB];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/    
    /* SPS Start */
    U16          sps_crnti;
    /*^ O, UECC_LLIM_DELETE_LC_RESP_SPS_CRNTI_PRESENT, B, 61, 65523 ^*/
    /* SPS Stop */
} rrc_uecc_llim_delete_lc_resp_t; /*^ API ^*/
/* ERB RELEASE COMMAND END */

/******************************************************************************
*   UECC_LLIM_ADD_ROLLBACK_REQ
******************************************************************************/


typedef struct _rrc_uecc_llim_add_rollback_req_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_bitmask_t           bitmask;    /*^ BITMASK ^*/
#define UECC_LLIM_ADD_ROLLBACK_REQ_OLD_MAC_CONFIG_PRESENT  0x01
#define UECC_LLIM_ADD_ROLLBACK_REQ_OLD_MEAS_CONFIG_PRESENT 0x02

    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
    rrc_counter_t           add_rollback_drb_count;

    rrc_uecc_llim_delete_drb_entity_t 
        add_drb_entity[RRC_UECC_LLIM_MAX_DRB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    rrm_mac_config_t   mac_config; 
   /*^ O, UECC_LLIM_ADD_ROLLBACK_REQ_OLD_MAC_CONFIG_PRESENT ^*/
    
    rrm_meas_config_t      meas_config;
    /*^ O, UECC_LLIM_ADD_ROLLBACK_REQ_OLD_MEAS_CONFIG_PRESENT ^*/
} rrc_uecc_llim_add_rollback_req_t; /*^ API ^*/


/******************************************************************************
*   UECC_LLIM_ADD_ROLLBACK_RESP
******************************************************************************/
typedef struct _rrc_uecc_llim_add_rollback_resp_t
{
    rrc_transaction_id_t     transaction_id;
    rrc_cell_index_t         cell_index;
    rrc_ue_index_t           ue_index;
    rrc_response_t           response;
} rrc_uecc_llim_add_rollback_resp_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_MODIFY_ROLLBACK_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_modify_rollback_req_t
{
    rrc_bitmask_t           bitmask;    /*^ BITMASK ^*/
    /*CA_START*/
#define UECC_LLIM_MODIFY_LC_REQ_R10_UE_CATEGORY_PRESENT   0x02
    /*CA_STOP*/
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;

    rrc_uecc_llim_modify_drb_version_t  modify_drb_entity;
    /*CA_START*/
    scells_operation_list_t    scells_operation_list[MAX_NUM_CELLS];
    U8                              rel10_ue_category;
    /*^ O, UECC_LLIM_MODIFY_LC_REQ_R10_UE_CATEGORY_PRESENT ^*/
    /*CA_STOP*/
} rrc_uecc_llim_modify_rollback_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_MODIFY_ROLLBACK_RESP
******************************************************************************/
typedef struct _rrc_uecc_llim_modify_rollback_resp_t
{
    rrc_transaction_id_t     transaction_id;
    rrc_cell_index_t         cell_index;
    rrc_ue_index_t           ue_index;
    rrc_response_t           response;
} rrc_uecc_llim_modify_rollback_resp_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_CHANGE_CRNTI_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_change_crnti_req_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
    rrc_rnti_t              old_crnti;
    rrc_rnti_t              new_crnti;
} rrc_uecc_llim_change_crnti_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_CHANGE_CRNTI_RESP
******************************************************************************/
typedef struct _rrc_uecc_llim_change_crnti_resp_t
{
    rrc_transaction_id_t     transaction_id;
    rrc_cell_index_t         cell_index;
    rrc_ue_index_t           ue_index;
    rrc_response_t           response_code;
} rrc_uecc_llim_change_crnti_resp_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_RADIO_LINK_FAILURE_IND
******************************************************************************/
typedef struct _rrc_uecc_llim_radio_link_failure_ind_t
{
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
    /* sync changes start */
    U32                     ue_sync_status;
    /* sync changes stop */
} rrc_uecc_llim_radio_link_failure_ind_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_MEAS_CONFIG_REQ
******************************************************************************/
typedef struct _rrc_uecc_llim_meas_config_req_t
{
    rrc_transaction_id_t     transaction_id;
    rrc_cell_index_t         cell_index;
    rrc_ue_index_t           ue_index;
    rrm_meas_config_t        meas_config;
} rrc_uecc_llim_meas_config_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_MEAS_CONFIG_RESP
******************************************************************************/
typedef struct _rrc_uecc_llim_meas_config_resp_t
{
    rrc_transaction_id_t     transaction_id;
    rrc_cell_index_t         cell_index;
    rrc_ue_index_t           ue_index;
    rrc_response_t           response_code;
} rrc_uecc_llim_meas_config_resp_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_INACTIVE_UES_IND
******************************************************************************/
typedef struct _rrc_uecc_llim_inactive_ue_info_t
{
    rrc_ue_index_t    ue_index;
    U8                inact_time_val;
} rrc_uecc_llim_inactive_ue_info_t;

typedef struct _rrc_uecc_llim_inactive_ues_ind_t
{
    rrc_cell_index_t        cell_index;

	/* SPR_17664_start*/
    U8           num_of_inactive_ue; /*^ M, 0, B, 1, RRC_MAX_INACTIVE_UES ^*/
    
	/* SPR_17664_end*/
rrc_uecc_llim_inactive_ue_info_t     inactive_ue_info[RRC_MAX_INACTIVE_UES];
} rrc_uecc_llim_inactive_ues_ind_t; /*^ API ^*/

/* SPR 4118 Start */ 
/******************************************************************************
*   UECC_LLIM_NOTIFY_INTEGRITY_FAILURE
******************************************************************************/
typedef struct _rrc_uecc_llim_notify_integrity_failure_t
{
    rrc_cell_index_t  cell_index;
    
    rrc_ue_index_t    ue_index;

    rrc_lc_id_t     lc_id;
}rrc_uecc_llim_notify_integrity_failure_t; /*^ API ^*/
/* SPR 4118 Stop */

/******************************************************************************
*   UECC_LLIM_COUNT_WRAPAROUND_IND
******************************************************************************/
typedef struct _rrc_uecc_llim_count_wraparound_ind_t
{
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t           ue_index;
} rrc_uecc_llim_count_wraparound_ind_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_RESET_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_uecc_llim_reset_ue_entity_req_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index;
} rrc_uecc_llim_reset_ue_entity_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_RESET_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_uecc_llim_reset_ue_entity_cnf_t
{
    rrc_transaction_id_t  transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t        ue_index;
    rrc_response_t        response;     
} rrc_uecc_llim_reset_ue_entity_cnf_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND
*******************************************************************************/

typedef struct _rrc_uecc_llim_lc_id_complete_list_t
{
    U8                        lc_count;
    rrc_lc_id_t               lc_id[MAX_ERAB_COUNT];
} rrc_uecc_llim_lc_id_complete_list_t;

typedef struct _rrc_uecc_llim_re_establish_ue_entity_complete_ind_t
{
    rrc_transaction_id_t          transaction_id;
    rrc_cell_index_t              cell_index;
    rrc_ue_index_t                ue_index;
    rrc_uecc_llim_lc_id_complete_list_t    lc_id_list;
} rrc_uecc_llim_re_establish_ue_entity_complete_ind_t; /*^ API ^*/

/* SPS Start */
/******************************************************************************
*   UECC_LLIM_MAC_RECONFIGURE_IND
******************************************************************************/
typedef struct _rrc_uecc_llim_mac_reconfig_complete_ind_t
{
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t        ue_index;
    rrc_response_t        response;
}rrc_uecc_llim_mac_reconfig_complete_ind_t;
/* SPS Stop */

typedef struct _rrc_pdcp_drb_info_t
{
    rrc_lc_id_t         lc_id;
    rrc_rb_direction_t  rb_direction;
} rrc_pdcp_drb_info_t; 

typedef struct _rrc_uecc_llim_drb_list_t
{

	/* SPR_17664_start*/
    U8                         drb_count;    /*^ M, 0, B, 1, MAX_LC_COUNT ^*/
    
	/* SPR_17664_end*/
rrc_pdcp_drb_info_t        drb_info[MAX_LC_COUNT];  /*^ M,0,OCTET_STRING,VARIABLE ^*/
}rrc_uecc_llim_drb_list_t;

typedef struct _rrc_uecc_llim_drb_count_msb_req_t
{
    rrc_transaction_id_t       transaction_id;
    rrc_cell_index_t           cell_index;
    rrc_ue_index_t             ue_index;
    rrc_uecc_llim_drb_list_t   drb_list; 
    /*^ M, 0, N, 0, 0 ^*/
} rrc_uecc_llim_drb_count_msb_req_t; /*^ API ^*/

#define UECC_LLIM_DRB_COUNTMSB_UPLINK_PRESENT   0x01    
#define UECC_LLIM_DRB_COUNTMSB_DOWNLINK_PRESENT 0x02    
typedef struct _rrc_uecc_llim_drb_countMSB_info_t
{
    rrc_bitmask_t  bitmask;    /*^ BITMASK ^*/
    rrc_lc_id_t    lc_id;
    U32            countMSB_Uplink; /*^ 0, UECC_LLIM_DRB_COUNTMSB_UPLINK_PRESENT, H, 0, 33554431 ^*/
    U32            countMSB_Downlink; /*^ O, UECC_LLIM_DRB_COUNTMSB_DOWNLINK_PRESENT, H, 0, 33554431 ^*/
}rrc_uecc_llim_drb_countMSB_info_t;

typedef struct _rrc_uecc_llim_drb_countMSB_info_list_t
{
	
	/* SPR_17664_start*/
        U8                                   drb_count;    /*^ M, 0, B, 1, MAX_LC_COUNT ^*/
        
	/* SPR_17664_end*/
	rrc_uecc_llim_drb_countMSB_info_t    drb_countMSB_info[MAX_LC_COUNT];  /*^ M,0,OCTET_STRING,VARIABLE ^*/  
}rrc_uecc_llim_drb_countMSB_info_list_t;

#define UECC_LLIM_DRB_COUNTMSB_INFO_LIST_PRESENT 0x01    
typedef struct _rrc_uecc_llim_drb_count_msb_resp_t
{
    rrc_bitmask_t                bitmask;    /*^ BITMASK ^*/
    rrc_transaction_id_t         transaction_id;
    rrc_cell_index_t             cell_index;
    U16                          ue_index;
    U8                           response;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_return_et */
    rrc_uecc_llim_drb_countMSB_info_list_t     drb_countMSB_info_list; 
/*^ O, UECC_LLIM_DRB_COUNTMSB_INFO_LIST_PRESENT, N, 0, 0 ^*/
}rrc_uecc_llim_drb_count_msb_resp_t;

/*SPR:6462 start*/
/******************************************************************************
*   UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF
******************************************************************************/

typedef struct _rrc_uecc_llim_re_establish_ue_entity_complete_cnf_t
{
    rrc_transaction_id_t          transaction_id;
    rrc_cell_index_t              cell_index;
    rrc_ue_index_t                ue_index;
    rrc_response_t                response_code;
} rrc_uecc_llim_re_establish_ue_entity_complete_cnf_t; /*^ API ^*/

/*SPR:6462 stop */

/*BUG 604 changes start*/
/******************************************************************************
*   UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_s1u_intra_enb_data_fwd_req_t
{
    rrc_transaction_id_t     transaction_id;
    rrc_cell_index_t         cell_index;
    rrc_ue_index_t           ue_index;        /*Source UeIndex*/
    rrc_ue_index_t           target_ue_index; /*Target UeIndex */

} rrc_uecc_llim_s1u_intra_enb_data_fwd_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_CNF
******************************************************************************/

typedef struct _rrc_uecc_llim_s1u_intra_enb_data_fwd_cnf_t
{
    rrc_transaction_id_t     transaction_id;
    rrc_cell_index_t         cell_index;
    rrc_ue_index_t           ue_index;        /*Source UeIndex*/

    rrc_response_t           response;        

} rrc_uecc_llim_s1u_intra_enb_data_fwd_cnf_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_S1U_LOCAL_PATH_SWITCH_REQ
******************************************************************************/

typedef struct _rrc_uecc_llim_s1u_local_path_switch_req_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index;
    rrc_ue_index_t          ue_index; /*Target UeIndex*/

    rrc_ue_index_t          ue_index_for_path_switch;        /*Source UeIndex */

} rrc_uecc_llim_s1u_local_path_switch_req_t; /*^ API ^*/

/******************************************************************************
*   UECC_LLIM_S1U_LOCAL_PATH_SWITCH_CNF
******************************************************************************/

typedef struct _rrc_uecc_llim_s1u_local_path_switch_cnf_t
{
    rrc_transaction_id_t     transaction_id;
    rrc_cell_index_t         cell_index;
    rrc_ue_index_t           ue_index;        /*Target UeIndex*/
    rrc_response_t           response;        

} rrc_uecc_llim_s1u_local_path_switch_cnf_t; /*^ API ^*/

/*BUG 604 changes stop*/

/*SPR_17727_START*/
/******************************************************************************
*   UECCMD_UE_CON_RE_ESTABLISH_ONGOING
******************************************************************************/

typedef struct _rrc_uecc_ueccmd_ue_con_re_establish_ongoing_t
{
    rrc_cell_index_t        cell_index;
    rrc_rnti_t              rnti;
}rrc_uecc_ueccmd_ue_con_re_establish_ongoing_t;  /*^ API ^*/
/*SPR_17727_END*/


#pragma pack(pop)

#endif /* _RRC_UECC_LLIM_INTF_H_ */

