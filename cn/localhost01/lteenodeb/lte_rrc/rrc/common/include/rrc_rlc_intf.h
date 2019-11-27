/******************************************************************************
*
*   FILE NAME:
*       rrc_rlc_intf.h
*
*   DESCRIPTION:
*       This file contains types used for representation of RLC API inside RRC.
*       Based on LTE_RLC_API_Rel_1.0_Rev_1.1.doc and LTE_RRC_API_0_31.doc.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   07 Apr 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_RLC_INTF_H_
#define _RRC_RLC_INTF_H_

#include "rrc_defines.h"

/* UE specific RRC defines */
#define RRC_RLC_MAX_NUM_LC  10

typedef enum
{
    RLC_FAILURE,
    RLC_SUCCESS,
    RLC_PARTIAL_SUCCESS
} rrc_rlc_return_et;

typedef enum
{
    RLC_NO_ACTION,
    RLC_FREEZE
} rrc_rlc_lc_state_et;

#pragma pack(push, 1)

/******************************************************************************
* RLC create UE tags
******************************************************************************/
typedef struct _rrc_rlc_qci_info_t
{
   U8           qci; /*^ M, 0, B, 1, 9 ^*/
} rrc_rlc_qci_info_t;

/*Bug_9632_Fix Start*/
typedef struct _rrc_rlc_tx_bit_rate_info_t
{
    U32         bit_rate_tx; /*^ M, 0, N, 0, 0 ^*/
} rrc_rlc_tx_bit_rate_info_t;

typedef struct _rrc_rlc_rx_bit_rate_info_t
{
    U32         bit_rate_rx; /*^ M, 0, N, 0, 0 ^*/
} rrc_rlc_rx_bit_rate_info_t;
/*Bug_9632_Fix Stop*/

typedef struct _rrc_rlc_queue_size_info_t
{
    U32         rlc_queue_size; /*^ M, 0, H, 0, 28311552 ^*/
} rrc_rlc_queue_size_info_t;

/*OPTION3X Changes Start*/
typedef struct _rrc_rlc_dc_bearer_type_info
{
    U8		dc_bearer_type; /*^ M, 0, H, 0, 2 ^*/ /* rrc_dc_bearer_type_et */
}rrc_rlc_dc_bearer_type_info;
/*OPTION3X Changes Stop*/

#define RRC_RLC_QCI_INFO_PRESENT        0x01
#define RRC_RLC_TX_BIT_RATE_INFO_PRESENT 0x02
#define RRC_RLC_RX_BIT_RATE_INFO_PRESENT 0x04
#define RRC_RLC_QUEUE_SIZE_INFO_PRESENT 0x08
/*OPTION3X Changes Start*/
#define RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_RX_UM 	 0x10
#define RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_UM 	 0x20
#define RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_RX_UM 0x40
#define RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_RX_AM 0x80
/*OPTION3X Changes Stop*/

typedef struct _rrc_rlc_create_tx_um_rlc_t
{
    rrc_bitmask_t               optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    
    rrc_lc_id_t         lc_id;
    rrc_sn_field_l_t    sn_field_length;
    rrc_rlc_qci_info_t  rrc_rlc_qci_info;    
    /*^ TLV, RRC_RLC_QCI_INFO, RRC_RLC_QCI_INFO_PRESENT ^*/

    rrc_rlc_tx_bit_rate_info_t rrc_rlc_tx_bit_rate_info;
    /*^ TLV, RRC_RLC_TX_BIT_RATE_INFO, RRC_RLC_TX_BIT_RATE_INFO_PRESENT ^*/
    
    rrc_rlc_queue_size_info_t  rrc_rlc_queue_size_info;
    /*^ TLV, RRC_RLC_QUEUE_SIZE_INFO, RRC_RLC_QUEUE_SIZE_INFO_PRESENT ^*/

/*OPTION3X Changes Start*/
    rrc_rlc_dc_bearer_type_info dc_bearer_type_info;
   /*^ TLV, RRC_RLC_DC_BEARER_TYPE_INFO , RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_UM ^*/
/*OPTION3X Changes Stop*/
} rrc_rlc_create_tx_um_rlc_t;

typedef struct _rrc_rlc_create_rx_um_rlc_t
{
    rrc_bitmask_t               optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    
    rrc_lc_id_t         lc_id;
    rrc_sn_field_l_t    sn_field_length;
    rrc_t_reordering_t  t_reordering;
    rrc_rlc_qci_info_t  rrc_rlc_qci_info;    
    /*^ TLV, RRC_RLC_QCI_INFO, RRC_RLC_QCI_INFO_PRESENT ^*/

    rrc_rlc_rx_bit_rate_info_t rrc_rlc_rx_bit_rate_info;
    /*^ TLV, RRC_RLC_RX_BIT_RATE_INFO, RRC_RLC_RX_BIT_RATE_INFO_PRESENT ^*/
    
/*OPTION3X Changes Start*/
    rrc_rlc_dc_bearer_type_info dc_bearer_type_info;
    /*^ TLV, RRC_RLC_DC_BEARER_TYPE_INFO , RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_RX_UM ^*/
/*OPTION3X Changes Stop*/
    
} rrc_rlc_create_rx_um_rlc_t;

typedef struct _rrc_rlc_create_tx_rx_um_rlc_t
{
    rrc_bitmask_t               optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    
    rrc_lc_id_t         lc_id;
    rrc_sn_field_l_t    sn_field_length_tx;
    rrc_sn_field_l_t    sn_field_length_rx;
    rrc_t_reordering_t  t_reordering;
    rrc_rlc_qci_info_t  rrc_rlc_qci_info;    
    /*^ TLV, RRC_RLC_QCI_INFO, RRC_RLC_QCI_INFO_PRESENT ^*/

    rrc_rlc_tx_bit_rate_info_t rrc_rlc_tx_bit_rate_info;
    /*^ TLV, RRC_RLC_TX_BIT_RATE_INFO, RRC_RLC_TX_BIT_RATE_INFO_PRESENT ^*/
    
    rrc_rlc_rx_bit_rate_info_t rrc_rlc_rx_bit_rate_info;
    /*^ TLV, RRC_RLC_RX_BIT_RATE_INFO, RRC_RLC_RX_BIT_RATE_INFO_PRESENT ^*/
    
    rrc_rlc_queue_size_info_t  rrc_rlc_queue_size_info;
    /*^ TLV, RRC_RLC_QUEUE_SIZE_INFO, RRC_RLC_QUEUE_SIZE_INFO_PRESENT ^*/

/*OPTION3X Changes Start*/
    rrc_rlc_dc_bearer_type_info dc_bearer_type_info;
    /*^ TLV, RRC_RLC_DC_BEARER_TYPE_INFO , RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_RX_UM ^*/
/*OPTION3X Changes Stop*/
} rrc_rlc_create_tx_rx_um_rlc_t;

typedef struct _rrc_rlc_create_tx_rx_am_rlc_t
{
    rrc_bitmask_t               optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    
    rrc_lc_id_t         lc_id;
    U16                 t_poll_retransmission;
    U16                 poll_pdu;
    U16                 poll_byte;
    U16                 max_re_tx_thrsh_ld;
    rrc_t_reordering_t  t_reordering;
    U16                 t_status_prohibit;
    rrc_rlc_qci_info_t  rrc_rlc_qci_info;    
    /*^ TLV, RRC_RLC_QCI_INFO, RRC_RLC_QCI_INFO_PRESENT ^*/

    rrc_rlc_tx_bit_rate_info_t rrc_rlc_tx_bit_rate_info;
    /*^ TLV, RRC_RLC_TX_BIT_RATE_INFO, RRC_RLC_TX_BIT_RATE_INFO_PRESENT ^*/
    
    rrc_rlc_rx_bit_rate_info_t rrc_rlc_rx_bit_rate_info;
    /*^ TLV, RRC_RLC_RX_BIT_RATE_INFO, RRC_RLC_RX_BIT_RATE_INFO_PRESENT ^*/
    
    rrc_rlc_queue_size_info_t  rrc_rlc_queue_size_info;
    /*^ TLV, RRC_RLC_QUEUE_SIZE_INFO, RRC_RLC_QUEUE_SIZE_INFO_PRESENT ^*/

/*OPTION3X Changes Start*/
    rrc_rlc_dc_bearer_type_info dc_bearer_type_info;
    /*^ TLV, RRC_RLC_DC_BEARER_TYPE_INFO , RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_RX_AM ^*/    
/*OPTION3X Changes Stop*/
} rrc_rlc_create_tx_rx_am_rlc_t;

/******************************************************************************
* RLC delete UE tags
******************************************************************************/
typedef struct _rrc_rlc_delete_tx_um_rlc_t
{
    rrc_lc_id_t lc_id;
} rrc_rlc_delete_tx_um_rlc_t;

typedef struct _rrc_rlc_delete_rx_um_rlc_t
{
    rrc_lc_id_t lc_id;
} rrc_rlc_delete_rx_um_rlc_t;

typedef struct _rrc_rlc_delete_tx_rx_um_rlc_t
{
    rrc_lc_id_t lc_id;
} rrc_rlc_delete_tx_rx_um_rlc_t;

typedef struct _rrc_rlc_delete_tx_rx_am_rlc_t
{
    rrc_lc_id_t lc_id;
} rrc_rlc_delete_tx_rx_am_rlc_t;

/******************************************************************************
* RLC reconfigure UE tags
******************************************************************************/
typedef struct _rrc_rlc_reconfig_tx_um_rlc_t
{
    rrc_bitmask_t               optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rrc_lc_id_t lc_id;
    rrc_rlc_qci_info_t  rrc_rlc_qci_info;    
    /*^ TLV, RRC_RLC_QCI_INFO, RRC_RLC_QCI_INFO_PRESENT ^*/

    rrc_rlc_tx_bit_rate_info_t rrc_rlc_tx_bit_rate_info;
    /*^ TLV, RRC_RLC_TX_BIT_RATE_INFO, RRC_RLC_TX_BIT_RATE_INFO_PRESENT ^*/
    
    rrc_rlc_queue_size_info_t  rrc_rlc_queue_size_info;
    /*^ TLV, RRC_RLC_QUEUE_SIZE_INFO, RRC_RLC_QUEUE_SIZE_INFO_PRESENT ^*/
} rrc_rlc_reconfig_tx_um_rlc_t;

typedef struct _rrc_rlc_reconfig_rx_um_rlc_t
{
    rrc_bitmask_t               optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rrc_lc_id_t lc_id;
    rrc_t_reordering_t  t_reordering;
    rrc_rlc_qci_info_t  rrc_rlc_qci_info;    
    /*^ TLV, RRC_RLC_QCI_INFO, RRC_RLC_QCI_INFO_PRESENT ^*/

    rrc_rlc_rx_bit_rate_info_t rrc_rlc_rx_bit_rate_info;
    /*^ TLV, RRC_RLC_RX_BIT_RATE_INFO, RRC_RLC_RX_BIT_RATE_INFO_PRESENT ^*/
} rrc_rlc_reconfig_rx_um_rlc_t;

typedef struct _rrc_rlc_reconfig_tx_rx_um_rlc_t
{
    rrc_bitmask_t               optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rrc_lc_id_t lc_id;
    rrc_t_reordering_t  t_reordering;
    rrc_rlc_qci_info_t  rrc_rlc_qci_info;    
    /*^ TLV, RRC_RLC_QCI_INFO, RRC_RLC_QCI_INFO_PRESENT ^*/

    rrc_rlc_tx_bit_rate_info_t rrc_rlc_tx_bit_rate_info;
    /*^ TLV, RRC_RLC_TX_BIT_RATE_INFO, RRC_RLC_TX_BIT_RATE_INFO_PRESENT ^*/
    
    rrc_rlc_rx_bit_rate_info_t rrc_rlc_rx_bit_rate_info;
    /*^ TLV, RRC_RLC_RX_BIT_RATE_INFO, RRC_RLC_RX_BIT_RATE_INFO_PRESENT ^*/
    
    rrc_rlc_queue_size_info_t  rrc_rlc_queue_size_info;
    /*^ TLV, RRC_RLC_QUEUE_SIZE_INFO, RRC_RLC_QUEUE_SIZE_INFO_PRESENT ^*/
} rrc_rlc_reconfig_tx_rx_um_rlc_t;

typedef struct _rrc_rlc_reconfig_tx_rx_am_rlc_t
{
    rrc_bitmask_t               optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rrc_lc_id_t lc_id;
    U16                 t_poll_retransmission;
    U16                 poll_pdu;
    U16                 poll_byte;
    U16                 max_re_tx_thrsh_ld;
    rrc_t_reordering_t  t_reordering;
    U16                 t_status_prohibit;
    rrc_rlc_qci_info_t  rrc_rlc_qci_info;    
    /*^ TLV, RRC_RLC_QCI_INFO, RRC_RLC_QCI_INFO_PRESENT ^*/

    rrc_rlc_tx_bit_rate_info_t rrc_rlc_tx_bit_rate_info;
    /*^ TLV, RRC_RLC_TX_BIT_RATE_INFO, RRC_RLC_TX_BIT_RATE_INFO_PRESENT ^*/
    
    rrc_rlc_rx_bit_rate_info_t rrc_rlc_rx_bit_rate_info;
    /*^ TLV, RRC_RLC_RX_BIT_RATE_INFO, RRC_RLC_RX_BIT_RATE_INFO_PRESENT ^*/
    
    rrc_rlc_queue_size_info_t  rrc_rlc_queue_size_info;
    /*^ TLV, RRC_RLC_QUEUE_SIZE_INFO, RRC_RLC_QUEUE_SIZE_INFO_PRESENT ^*/
} rrc_rlc_reconfig_tx_rx_am_rlc_t;

/*Bug_9632_Fix Start*/
typedef struct _rrc_rlc_ue_bit_rate_info_t
{
    U32             bit_rate_tx; /*^ M, 0, N, 0, 0 ^*/    
    U32             bit_rate_rx; /*^ M, 0, N, 0, 0 ^*/
} rrc_rlc_ue_bit_rate_info_t;
/*Bug_9632_Fix Stop*/

/******************************************************************************
* XXX_ENTITY_ERROR
******************************************************************************/
typedef struct _rrc_rlc_create_entity_error_t
{
    rrc_lc_id_t     lc_id;
    rrc_response_t  response;               /* rrc_rlc_error_code_et */
} rrc_rlc_create_entity_error_t;

typedef struct _rrc_rlc_delete_entity_error_t
{
    rrc_lc_id_t     lc_id;
    rrc_response_t  response;               /* rrc_rlc_error_code_et */
} rrc_rlc_delete_entity_error_t;

typedef struct _rrc_rlc_reconfig_entity_error_t
{
    rrc_lc_id_t     lc_id;
    rrc_response_t  response;               /* rrc_rlc_error_code_et */
} rrc_rlc_reconfig_entity_error_t;

typedef struct _rrc_rlc_entity_lc_id_error_t
{
    rrc_lc_id_t     lc_id;
    rrc_response_t  response;               /* rrc_rlc_error_code_et */
} rrc_rlc_entity_lc_id_error_t;

/******************************************************************************
* RLC reetablish UE tags
******************************************************************************/

typedef struct _rrc_rlc_lc_state_t
{
    U8  state;
/*^ M, 0, H, 0, 1 ^*/                       /* rrc_rlc_lc_state_et */
} rrc_rlc_lc_state_t;

#define                     RRC_RLC_LC_STATE_PRESENT    0x01
typedef struct _rrc_rlc_entity_lcid_t
{
    rrc_bitmask_t                   bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rrc_lc_id_t     lc_id;
    
    rrc_rlc_lc_state_t   lc_state;
/*^ TLV, RRC_RLC_LC_STATE, RRC_RLC_LC_STATE_PRESENT ^*/
} rrc_rlc_entity_lcid_t;


/* SPS Start */
typedef struct _rrc_rlc_sps_ul_setup_info_t
{
    U8 semi_persistent_sched_interval_ul;
    /*^ M, 0, H, 0, 9^*/
}rrc_rlc_sps_ul_setup_info_t;

typedef struct _rrc_rlc_sps_ul_info_t
{
    rrc_bitmask_t            optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_RLC_SPS_UL_SETUP_INFO_PRESENT    0x01

    U8            request_type;
    /*^ M, 0, H, 0, 1^*/

    rrc_rlc_sps_ul_setup_info_t    sps_ul_setup_info;
    /*^ TLV, RRC_RLC_SPS_UL_SETUP_INFO_TAG, RRC_RLC_SPS_UL_SETUP_INFO_PRESENT ^*/
}rrc_rlc_sps_ul_info_t;


typedef struct _rrc_rlc_sps_dl_setup_info_t
{
    U8 semi_persistent_sched_interval_dl;
    /*^ M, 0, H, 0, 9^*/
}rrc_rlc_sps_dl_setup_info_t;

typedef struct _rrc_rlc_sps_dl_info_t
{
    rrc_bitmask_t            optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_RLC_SPS_DL_SETUP_INFO_PRESENT    0x01

    U8            request_type;
    /*^ M, 0, H, 0, 1^*/

    rrc_rlc_sps_dl_setup_info_t    sps_dl_setup_info;
    /*^ TLV, RRC_RLC_SPS_DL_SETUP_INFO_TAG, RRC_RLC_SPS_DL_SETUP_INFO_PRESENT ^*/
}rrc_rlc_sps_dl_info_t;

typedef struct _rrc_rlc_sps_config_t
{

    rrc_bitmask_t             optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_RLC_SPS_DL_INFO_PRESENT        0x01
#define RRC_RLC_SPS_UL_INFO_PRESENT        0x02

    rrc_rlc_sps_dl_info_t    sps_dl_info;
    /*^ TLV, RRC_RLC_SPS_DL_INFO_TAG, RRC_RLC_SPS_DL_INFO_PRESENT ^*/

    rrc_rlc_sps_ul_info_t    sps_ul_info;
    /*^ TLV, RRC_RLC_SPS_UL_INFO_TAG, RRC_RLC_SPS_UL_INFO_PRESENT ^*/
}rrc_rlc_sps_config_t;
/* SPS Stop */

/******************************************************************************
*   RLC_CREATE_UE_ENTITY_REQ
******************************************************************************/
#define                     RRC_RLC_UE_BIT_RATE_PRESENT    0x01
/* SPS Start */
#define                     RRC_RLC_SPS_CONFIG_PRESENT     0x02
/* SPS Stop */
typedef struct _rrc_rlc_create_ue_entity_req_t
{
    rrc_bitmask_t                   bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    
    rrc_ue_index_t                  ue_index;
    U16                             rnti;

    rrc_counter_t                   num_create_tx_um_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_create_tx_um_rlc_t      create_tx_um_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_CREATE_TX_UM_RLC_ENTITY ^*/


    rrc_counter_t                   num_create_rx_um_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_create_rx_um_rlc_t      create_rx_um_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_CREATE_RX_UM_RLC_ENTITY ^*/


    rrc_counter_t                   num_create_tx_rx_um_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_create_tx_rx_um_rlc_t   create_tx_rx_um_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_CREATE_TX_RX_UM_RLC_ENTITY ^*/


    rrc_counter_t                   num_create_tx_rx_am_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_create_tx_rx_am_rlc_t   create_tx_rx_am_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_CREATE_TX_RX_AM_RLC_ENTITY ^*/

    rrc_rlc_ue_bit_rate_info_t   rlc_ue_bit_rate_info;
/*^ TLV, RRC_RLC_UE_BIT_RATE_INFO, RRC_RLC_UE_BIT_RATE_PRESENT ^*/

    /* SPS Start */
    rrc_rlc_sps_config_t        sps_config;
    /*^ TLV, RRC_RLC_SPS_CONFIG_TAG, RRC_RLC_SPS_CONFIG_PRESENT ^*/
    /* SPS Stop */

} rrc_rlc_create_ue_entity_req_t; /*^ API, RRC_RLC_CREATE_UE_ENTITY_REQ ^*/

/******************************************************************************
*   RLC_CREATE_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_rlc_create_ue_entity_cnf_t
{
    
    rrc_ue_index_t                  ue_index;
    rrc_response_t                  response_code;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_rlc_return_et */


    rrc_counter_t                   num_create_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_create_entity_error_t   create_error_entities[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_CREATE_ENTITY_ERROR ^*/

} rrc_rlc_create_ue_entity_cnf_t; /*^ API, RRC_RLC_CREATE_UE_ENTITY_CNF ^*/

/******************************************************************************
*   RLC_RECONFIG_UE_ENTITY_REQ
******************************************************************************/
#define RRC_RLC_RECONFIG_UE_BIT_RATE_INFO_PRESENT   0x01
/* SPS Start */
#define RRC_RLC_RECONFIG_UE_SPS_CONFIG_PRESENT      0x02
/* SPS Stop */
typedef struct _rrc_rlc_reconfig_ue_entity_req_t
{
    rrc_bitmask_t                   bitmask;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/


    rrc_ue_index_t                  ue_index;

    rrc_counter_t                   num_create_tx_um_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_create_tx_um_rlc_t      create_tx_um_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_CREATE_TX_UM_RLC_ENTITY ^*/


    rrc_counter_t                   num_create_rx_um_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_create_rx_um_rlc_t      create_rx_um_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_CREATE_RX_UM_RLC_ENTITY ^*/


    rrc_counter_t                   num_create_tx_rx_um_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_create_tx_rx_um_rlc_t   create_tx_rx_um_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_CREATE_TX_RX_UM_RLC_ENTITY ^*/


    rrc_counter_t                   num_create_tx_rx_am_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_create_tx_rx_am_rlc_t   create_tx_rx_am_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_CREATE_TX_RX_AM_RLC_ENTITY ^*/


    rrc_counter_t                   num_delete_tx_um_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_delete_tx_um_rlc_t      delete_tx_um_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_DELETE_TX_UM_RLC_ENTITY ^*/


    rrc_counter_t                   num_delete_rx_um_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_delete_rx_um_rlc_t      delete_rx_um_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_DELETE_RX_UM_RLC_ENTITY ^*/


    rrc_counter_t                   num_delete_tx_rx_um_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_delete_tx_rx_um_rlc_t   delete_tx_rx_um_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_DELETE_TX_RX_UM_RLC_ENTITY ^*/


    rrc_counter_t                   num_delete_tx_rx_am_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_delete_tx_rx_am_rlc_t   delete_tx_rx_am_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_DELETE_TX_RX_AM_RLC_ENTITY ^*/


    rrc_counter_t                   num_reconfig_tx_um_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_reconfig_tx_um_rlc_t    reconfig_tx_um_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_RECONFIG_TX_UM_RLC_ENTITY ^*/


    rrc_counter_t                   num_reconfig_rx_um_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_reconfig_rx_um_rlc_t    reconfig_rx_um_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_RECONFIG_RX_UM_RLC_ENTITY ^*/


    rrc_counter_t                   num_reconfig_tx_rx_um_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_reconfig_tx_rx_um_rlc_t reconfig_tx_rx_um_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_RECONFIG_TX_RX_UM_RLC_ENTITY ^*/


    rrc_counter_t                   num_reconfig_tx_rx_am_rlc;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_reconfig_tx_rx_am_rlc_t reconfig_tx_rx_am_rlc[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_RECONFIG_TX_RX_AM_RLC_ENTITY ^*/

    rrc_counter_t                   num_rlc_ue_bit_rate_info;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/
    
    rrc_rlc_ue_bit_rate_info_t   rlc_ue_bit_rate_info;
/*^ TLV, RRC_RLC_UE_BIT_RATE_INFO, RRC_RLC_RECONFIG_UE_BIT_RATE_INFO_PRESENT ^*/

    /* SPS Start */
    rrc_rlc_sps_config_t        sps_config;
    /*^ TLV, RRC_RLC_SPS_CONFIG_TAG, RRC_RLC_RECONFIG_UE_SPS_CONFIG_PRESENT ^*/
    /* SPS Stop */
} rrc_rlc_reconfig_ue_entity_req_t; /*^ API, RRC_RLC_RECONFIG_UE_ENTITY_REQ ^*/

/******************************************************************************
*   RLC_RECONFIG_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_rlc_reconfig_ue_entity_cnf_t
{
    
    rrc_ue_index_t                  ue_index;
    rrc_response_t                  response_code;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_rlc_return_et */


    rrc_counter_t                   num_create_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_create_entity_error_t   create_error_entities[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_CREATE_ENTITY_ERROR ^*/


    rrc_counter_t                   num_delete_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_delete_entity_error_t   delete_error_entities[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_DELETE_ENTITY_ERROR ^*/


    rrc_counter_t                   num_reconfig_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_reconfig_entity_error_t reconfig_error_entities[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_RECONFIG_ENTITY_ERROR ^*/

} rrc_rlc_reconfig_ue_entity_cnf_t; /*^ API, RRC_RLC_RECONFIG_UE_ENTITY_CNF ^*/

/******************************************************************************
*   RLC_DELETE_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_rlc_delete_ue_entity_req_t
{
    
    rrc_ue_index_t ue_index;
} rrc_rlc_delete_ue_entity_req_t; /*^ API, RRC_RLC_DELETE_UE_ENTITY_REQ ^*/

/******************************************************************************
*   RLC_DELETE_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_rlc_delete_ue_entity_cnf_t
{
    
    rrc_ue_index_t ue_index;
    rrc_response_t response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_rlc_return_et */

} rrc_rlc_delete_ue_entity_cnf_t; /*^ API, RRC_RLC_DELETE_UE_ENTITY_CNF ^*/

/******************************************************************************
*   RLC_RE_ESTABLISH_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_rlc_re_establish_ue_entity_req_t
{

    rrc_ue_index_t ue_index;

    rrc_counter_t           num_entity_lc_id;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_entity_lcid_t   entity_lcids[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_ENTITY_LCID ^*/

} rrc_rlc_re_establish_ue_entity_req_t;
/*^ API, RRC_RLC_RE_ESTABLISH_UE_ENTITY_REQ ^*/


/******************************************************************************
*   RLC_RE_ESTABLISH_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_rlc_re_establish_ue_entity_cnf_t
{
    
    rrc_ue_index_t ue_index;
    rrc_response_t                      response_code;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_rlc_return_et */


    rrc_counter_t                       num_error_entity;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_entity_lc_id_error_t error_entities[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_ENTITY_LC_ID_ERROR ^*/

} rrc_rlc_re_establish_ue_entity_cnf_t;
/*^ API, RRC_RLC_RE_ESTABLISH_UE_ENTITY_CNF ^*/
/******************************************************************************
*
*   RRC_RLC_UE_ENTITY_ERROR_IND
******************************************************************************/
typedef struct _rlc_ue_entity_error_ind_t
{
    
    rrc_ue_index_t ue_index;
    rrc_lc_id_t     lc_id;
}rlc_ue_entity_error_ind_t; /*^ API, RRC_RLC_UE_ENTITY_ERROR_IND ^*/

/******************************************************************************
*  RRC_RLC_CHANGE_CRNTI_REQ
*******************************************************************************/
typedef struct _rrc_rlc_change_crnti_req_t
{
    
    rrc_ue_index_t  ue_index;
    rrc_rnti_t      old_crnti;
    rrc_rnti_t      new_crnti;
} rrc_rlc_change_crnti_req_t; /*^ API, RRC_RLC_CHANGE_CRNTI_REQ ^*/

/******************************************************************************
**  RRC_RLC_CHANGE_CRNTI_CNF
*******************************************************************************/
typedef struct _rrc_rlc_change_crnti_cnf_t
{

    rrc_ue_index_t  ue_index;

    rrc_response_t  response;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_rlc_return_et */
} rrc_rlc_change_crnti_cnf_t; /*^ API, RRC_RLC_CHANGE_CRNTI_CNF ^*/


/******************************************************************************
*   RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND
******************************************************************************/
typedef struct _rrc_rlc_re_establish_ue_entity_complete_ind_t
{

    rrc_ue_index_t ue_index;

    rrc_counter_t           num_entity_lc_id;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_rlc_entity_lcid_t   entity_lcids[RRC_RLC_MAX_NUM_LC];
/*^ TLV, SEQUENCE, RRC_RLC_ENTITY_LCID ^*/

} rrc_rlc_re_establish_ue_entity_complete_ind_t;
/*^ API, RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND ^*/

/* SPR:6462 start */
/******************************************************************************
*   RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF
******************************************************************************/
typedef struct _rrc_rlc_re_establish_ue_entity_complete_cnf_t
{
    rrc_ue_index_t ue_index;
    
    rrc_response_t  response;
    /*^ M, 0, H, 0, 2 ^*/ /* rrc_rlc_return_et */

} rrc_rlc_re_establish_ue_entity_complete_cnf_t;
/*^ API, RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF ^*/
/* SPR:6462 stop */


typedef enum
{
    RLC_INVALID_UE_INDEX =   3,
    RLC_UE_ID_EXISTS =   4,
    RLC_UE_NOT_EXISTS =  5,
    RLC_INTERNAL_ERROR = 11,
    RLC_SYNTAX_ERROR  =  12,
    RLC_INVALID_LC_ID =  31,
    RLC_ENTITY_EXISTS  = 32,
    RLC_ENTITY_NOT_EXISTS =  33,
    RLC_UE_ENTITY_IN_USE  =  34

}rrc_rlc_error_code_et;

typedef struct _rrc_rlc_create_mbms_area_entity_req_t
{
    U8                num_of_area; /*^ M, 0, B, 1, 16 ^*/
    /* COV_94546_FIX_START */
    U8                area_id[MAX_MBSFN_AREA_PER_ENB];   /*^ M,0,OCTET_STRING,VARIABLE ^*/
    /* COV_94546_FIX_START */

}rrc_rlc_create_mbms_area_entity_req_t; /*^ API, RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_REQ ^*/

typedef struct _rrc_rlc_create_mbms_area_entity_cnf_t
{
    rrc_response_t   response_code; /*^ M, 0, H, 0, 2 ^*/ /* rrc_rlc_return_et */
}rrc_rlc_create_mbms_area_entity_cnf_t; /*^ API, RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_CNF ^*/

typedef struct _rrc_rlc_delete_mbms_area_entity_req_t
{
    U8                num_of_area; /*^ M, 0, B, 1, 16 ^*/
    U8                area_id[MAX_MBSFN_AREA_PER_ENB];   /*^ M,0,OCTET_STRING,VARIABLE ^*/
}rrc_rlc_delete_mbms_area_entity_req_t; /*^ API, RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_REQ ^*/


typedef struct _rrc_rlc_delete_mbms_area_entity_cnf_t
{
    rrc_response_t   response_code; /*^ M, 0, H, 0, 2 ^*/ /* rrc_rlc_return_et */
}rrc_rlc_delete_mbms_area_entity_cnf_t; /*^ API, RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_CNF ^*/

/******************************************************************************
*   RRC_RLC_CELL_CONFIG_REQ
******************************************************************************/
typedef struct _rlc_communication_info_t
{
    U8     pdcpInstance;    /*^ M, 0, N, 0, 0 ^*/
    U16   pdcpRxPort;       /*^ M, 0, N, 0, 0 ^*/
    U8    pdcpIpAddr[16];   /*^ M, 0, OCTET_STRING, FIXED ^*/
}rlc_communication_info_t;


typedef struct _rrc_rlc_config_cell_req_t
{
U16			Cell_index;         /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
rlc_communication_info_t	rlc_communication_info; 		 /*^ M, 0, N, 0, 0 ^*/
} rrc_rlc_config_cell_req_t; /*^ API, RRC_RLC_CELL_CONFIG_REQ ^*/


/******************************************************************************
*   RRC_RLC_CELL_CONFIG_CNF
******************************************************************************/
typedef struct _rrc_rlc_config_cell_cnf_t
{
 rrc_cell_index_t    cell_index;
 /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
U8	response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */
} rrc_rlc_config_cell_cnf_t; /*^ API, RRC_RLC_CELL_CONFIG_CNF ^*/


#pragma pack(pop)

#endif /* _RRC_RLC_INTF_H_ */

