/******************************************************************************
*
*   FILE NAME:
*       rrc_pdcp_intf.h
*
*   DESCRIPTION:
*       This file contains types used for representation of PDCP API inside RRC.
*       Based on LTE_PDCP_API_Rel_1.0_Rev_1.1.doc and LTE_RRC_API_0_31.doc.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   07 Apr 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_PDCP_INTF_H_
#define _RRC_PDCP_INTF_H_

#include "rrc_defines.h"

/******************************************************************************
*   PDCP API Internal Representation
******************************************************************************/
#define RRC_PDCP_MAX_SRB                2
#define RRC_PDCP_MAX_DRB                8
#define RRC_PDCP_MAX_RB                 10

/* Enum values for PDCP RLC mode */
typedef enum
{
    PDCP_RLC_MODE_UM = 0,
    PDCP_RLC_MODE_AM
} rrc_pdcp_rlc_mode_et;

typedef enum
{
    STOP_LWA_TRAFFIC = 0,
    START_LWA_TRAFFIC
}lwa_traffic_ind_et;
/* Enum values for PDCP discard timer */
#define RRC_PDCP_DISC_TIMER_MS_50       0
#define RRC_PDCP_DISC_TIMER_MS_100      1
#define RRC_PDCP_DISC_TIMER_MS_150      2
#define RRC_PDCP_DISC_TIMER_MS_300      3
#define RRC_PDCP_DISC_TIMER_MS_500      4
#define RRC_PDCP_DISC_TIMER_MS_750      5
#define RRC_PDCP_DISC_TIMER_MS_1500     6
#define RRC_PDCP_DISC_TIMER_MS_INFINITY 7

/* Enum values for PDCP SN size */
#define RRC_PDCP_SN_SIZE_7 7
#define RRC_PDCP_SN_SIZE_12 12

typedef enum
{
    PDCP_FAILURE,
    PDCP_SUCCESS,
    PDCP_PARTIAL_SUCCESS
} rrc_pdcp_return_et;

typedef enum 
{
    PDCP_ERR_CONTEXT_NOT_INITIALIZED = 512,/*0x200*/
    PDCP_ERR_CONTEXT_ALREADY_INITIALIZED ,
    PDCP_ERR_UE_CONTEXT_ALREADY_CREATED ,
    PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED ,
    PDCP_ERR_ENTITY_ALREADY_CREATED  ,
    PDCP_ERR_ENTITY_WRONG_DIRECTION  ,
    PDCP_ERR_ENTITY_WRONG_TYPE   ,
    PDCP_ERR_ENTITY_NOT_FOUND    ,
    PDCP_ERR_ENTITY_SET_SN_SIZE  ,
    PDCP_ERR_ENTITY_SET_STATUS_REPORT_REQUIRED   ,
    PDCP_ERR_TLV_PARSING_INVALID_LENGTH  ,
    PDCP_ERR_TLV_PARSING_INVALID_UE_ID = 768,/*0x300*/
    PDCP_ERR_TLV_PARSING_INVALID_OPTIONAL_PARAMETERS_LENGTH ,
    PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH ,
    PDCP_ERR_TLV_PARSING_INVALID_LC_ID  ,
    PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE  ,
    PDCP_ERR_TLV_PARSING_INVALID_CRNTI  ,
    PDCP_ERR_TLV_PARSING_INVALID_TAG_ID ,
    PDCP_ERR_TLV_PARSING_INVALID_RNTI_RANGE ,
    PDCP_ERR_TLV_PARSING_INVALID_API_ID 
}rrc_pdcp_error_code_et;
#pragma pack(push, 1)

/******************************************************************************
*   Common types
******************************************************************************/
typedef struct _rrc_pdcp_config_disc_timer_t
{
    U16 disc_timer;
} rrc_pdcp_config_disc_timer_t;

typedef struct _rrc_pdcp_config_sn_size_t
{
    U8 sn_size;
} rrc_pdcp_config_sn_size_t;

typedef struct _rrc_pdcp_config_st_rep_required_t
{
    U8 st_rep_required; /*^ M, 0, H, 0, 1 ^*/  /* rrc_bool_et */
} rrc_pdcp_config_st_rep_required_t;

/* PSR Start */
typedef struct _rrc_pdcp_config_ue_st_rep_required_t
{
    U8 ue_st_rep_required; /*^ M, 0, H, 0, 1 ^*/  /* rrc_bool_et */
} rrc_pdcp_config_ue_st_rep_required_t;
/* PSR End */

typedef struct _rrc_pdcp_cr_srb_entity_t
{
    rrc_lc_id_t                 srb_id;    /* RB Changes Start */
    U16                         rlc_mode;
/* rrc_pdcp_rlc_mode_et */

    rrc_rb_direction_t          rb_direction;
/* rrc_rb_direction_et */

} rrc_pdcp_cr_srb_entity_t;


typedef struct _rrc_pdcp_config_lwa_config_lwa_bearer_type_t
{
    U8 bearer_type;
}rrc_pdcp_config_lwa_config_lwa_bearer_type_t;

#define RRC_PDCP_DRB_CONFIG_LWA_BEARER_TYPE_PRESENT        0x01

typedef struct _lwa_config_t
{
    rrc_bitmask_t                                  optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U8                                             lwa_bearer;
    /*^ M, 0, N, 0, 0 ^*/

    rrc_pdcp_config_lwa_config_lwa_bearer_type_t  lwa_bearer_type;
    /*^ TLV, RRC_PDCP_LWA_CONFIG_BEARER_TYPE_TAG, RRC_PDCP_DRB_CONFIG_LWA_BEARER_TYPE_PRESENT ^*/

}rrc_pdcp_config_lwa_t;


#define RRC_PDCP_CR_DRB_CONFIG_ROHC_PRESENT                   0x01
#define RRC_PDCP_CR_DRB_CONFIG_DISC_TIMER_PRESENT             0x02
#define RRC_PDCP_CR_DRB_CONFIG_SN_SIZE_PRESENT                0x04
#define RRC_PDCP_CR_DRB_CONFIG_ST_REPORT_REQUIRED_PRESENT     0x08
/* PSR Start */
#define RRC_PDCP_CR_DRB_CONFIG_UE_ST_REPORT_REQUIRED_PRESENT  0x10 
/* PSR End */

typedef struct _rrc_pdcp_cr_drb_entity_t
{
    rrc_bitmask_t                   optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_lc_id_t                     lc_id;
    /* RB Changes Start */
    U8                          drb_id;
    /* RB Changes Stop */
    U16                              rlc_mode; /* rrc_pdcp_rlc_mode_et */
    rrc_rb_direction_t          rb_direction; /* rrc_rb_direction_et */

    U8                      qci;
    /*^ M, 0, B, 1, 9 ^*/
    
    rrc_pdcp_config_rohc_t          config_rohc;
/*^ TLV, RRC_PDCP_CONFIGURE_ROHC_TAG, RRC_PDCP_CR_DRB_CONFIG_ROHC_PRESENT ^*/

    rrc_pdcp_config_disc_timer_t    config_disc_timer;
/*^ TLV, RRC_PDCP_CONFIGURE_DISCARD_TIMER_TAG, RRC_PDCP_CR_DRB_CONFIG_DISC_TIMER_PRESENT ^*/

    rrc_pdcp_config_sn_size_t       config_sn_size;
/*^ TLV, RRC_PDCP_CONFIGURE_SN_SIZE_TAG, RRC_PDCP_CR_DRB_CONFIG_SN_SIZE_PRESENT ^*/

    rrc_pdcp_config_st_rep_required_t st_rep_required;
/*^ TLV, RRC_PDCP_CONFIGURE_ST_REPORT_REQUIRED_TAG, RRC_PDCP_CR_DRB_CONFIG_ST_REPORT_REQUIRED_PRESENT ^*/
/* PSR Start */
    rrc_pdcp_config_ue_st_rep_required_t ue_st_rep_required;
/*^ TLV, RRC_PDCP_CONFIGURE_UE_PSR_TAG, RRC_PDCP_CR_DRB_CONFIG_UE_ST_REPORT_REQUIRED_PRESENT ^*/
/* PSR End */
} rrc_pdcp_cr_drb_entity_t;

typedef struct _rrc_pdcp_del_srb_entity_t
{
    rrc_lc_id_t lc_id;
} rrc_pdcp_del_srb_entity_t;

typedef struct _rrc_pdcp_del_drb_entity_t
{
    rrc_lc_id_t lc_id;
} rrc_pdcp_del_drb_entity_t;


typedef struct _rrc_pdcp_rcfg_srb_entity_t
{
    rrc_bitmask_t               optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/


    rrc_lc_id_t                 lc_id;
    rrc_rb_direction_t          rb_direction;
/* rrc_rb_direction_et */

} rrc_pdcp_rcfg_srb_entity_t;


#define RRC_PDCP_RCFG_DRB_CONFIG_ROHC_PRESENT                   0x01
#define RRC_PDCP_RCFG_DRB_CONFIG_ST_REPORT_REQUIRED_PRESENT     0x02
/* PSR Start */
#define RRC_PDCP_RCFG_DRB_CONFIG_UE_ST_REPORT_REQUIRED_PRESENT  0x04
/* PSR End */

#define RRC_PDCP_RCFG_DRB_CONFIG_LWA_CONFIG_PRESENT             0x08

typedef struct _rrc_pdcp_rcfg_drb_entity_t
{
    rrc_bitmask_t               optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/


    rrc_lc_id_t                 lc_id;
    rrc_rb_direction_t          rb_direction;       /* rrc_rb_direction_et */

    rrc_pdcp_config_lwa_t       lwa_config;
    /*^ TLV, RRC_PDCP_CONFIGURE_LWA_TAG, RRC_PDCP_RCFG_DRB_CONFIG_LWA_CONFIG_PRESENT ^*/

    rrc_pdcp_config_rohc_t      config_rohc;
/*^ TLV, RRC_PDCP_CONFIGURE_ROHC_TAG, RRC_PDCP_RCFG_DRB_CONFIG_ROHC_PRESENT ^*/

    rrc_pdcp_config_st_rep_required_t st_rep_required;
/*^ TLV, RRC_PDCP_CONFIGURE_ST_REPORT_REQUIRED_TAG, RRC_PDCP_RCFG_DRB_CONFIG_ST_REPORT_REQUIRED_PRESENT ^*/

/* PSR Start */
    rrc_pdcp_config_ue_st_rep_required_t ue_st_rep_required;
/*^ TLV, RRC_PDCP_CONFIGURE_UE_PSR_TAG, RRC_PDCP_RCFG_DRB_CONFIG_UE_ST_REPORT_REQUIRED_PRESENT ^*/
/* PSR End */
} rrc_pdcp_rcfg_drb_entity_t;


typedef struct _rrc_pdcp_entity_error_t
{
    rrc_lc_id_t lc_id;
    rrc_response_t  response;                       /* rrc_pdcp_return_et */
} rrc_pdcp_entity_error_t;

typedef struct _rrc_pdcp_cr_srb_entity_error_t
{
    rrc_lc_id_t lc_id;
    rrc_response_t  response;                       /* rrc_pdcp_return_et */
} rrc_pdcp_cr_srb_entity_error_t;

typedef struct _rrc_pdcp_cr_drb_entity_error_t
{
    rrc_lc_id_t lc_id;
    rrc_response_t  response;                       /* rrc_pdcp_return_et */
} rrc_pdcp_cr_drb_entity_error_t;

typedef struct _rrc_pdcp_del_srb_entity_error_t
{
    rrc_lc_id_t lc_id;
    rrc_response_t  response;                       /* rrc_pdcp_return_et */
} rrc_pdcp_del_srb_entity_error_t;

typedef struct _rrc_pdcp_del_drb_entity_error_t
{
    rrc_lc_id_t lc_id;
    rrc_response_t  response;                       /* rrc_pdcp_return_et */
} rrc_pdcp_del_drb_entity_error_t;

typedef struct _rrc_pdcp_rcfg_srb_entity_error_t
{
    rrc_lc_id_t lc_id;
    rrc_response_t  response;                       /* rrc_pdcp_return_et */
} rrc_pdcp_rcfg_srb_entity_error_t;

typedef struct _rrc_pdcp_rcfg_drb_entity_error_t
{
    rrc_lc_id_t lc_id;
    rrc_response_t  response;                       /* rrc_pdcp_return_et */
} rrc_pdcp_rcfg_drb_entity_error_t;

typedef struct _rrc_pdcp_rb_entity_t
{
    rrc_lc_id_t                 lc_id;
} rrc_pdcp_rb_entity_t;

typedef struct _rrc_pdcp_rb_entity_error_t
{
    rrc_lc_id_t lc_id;
    rrc_response_t  response;               /* rrc_pdcp_error_code_et */
} rrc_pdcp_rb_entity_error_t;
typedef struct _rrc_pdcp_data_status_error_t
{
    U16  error_code;
    U8  p_buffer[0]; /*^ M, 0, OCTET_STRING, TILL_THE_END ^*/
} rrc_pdcp_data_status_error_t;

#define RRC_PDCP_CR_SRB_CONFIG_INT_PRESENT          0x01
#define RRC_PDCP_CR_SRB_CONFIG_CIPH_PRESENT         0x02
#define RRC_PDCP_CR_DRB_CONFIG_CIPH_PRESENT         0x04
#define RRC_PDCP_SECURITY_OFFSET_PRESENT            0x08
/* SPS Start */
#define RRC_PDCP_SPS_CONFIG_PRESENT                 0x10
/* SPS Stop */
/*SPR_17121_START*/
#define RRC_PDCP_CONFIG_MACI_REQ_PRESENT            0x20
/*SPR_17121_END*/
/*SPR_19066_START*/
#define RRC_PDCP_HO_TRIGGER_PRESENT                 0x40
#define RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_PRESENT 0x80
/******************************************************************************
*   PDCP_CREATE_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_ue_inactive_time_t
{
    U32   ue_inactive_time_val; /*^ M, 0, H, 0, 63 ^*/ /* rrc_ue_inactivity_time_et */
}rrc_ue_inactive_time_t;

typedef struct _rrc_pdcp_ho_triggered_t
{
    rrc_bitmask_t            optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_PDCP_UE_INACTIVE_TIME_LAPSED_PRESENT   0x01

    U8 ho_trigger; /*^ M, 0, H, 1, 1^*/

    rrc_ue_inactive_time_t  rrc_ue_inactive_time_lapsed; 
    /*^ TLV, RRC_PDCP_UE_INACTIVE_TIME_LAPSED_TAG, RRC_PDCP_UE_INACTIVE_TIME_LAPSED_PRESENT ^*/
}rrc_pdcp_ho_triggered_t;
/*SPR_19066_END*/

/* SPS Start */
typedef struct _rrc_pdcp_sps_ul_setup_info_t
{
    U8 semi_persistent_sched_interval_ul;
    /*^ M, 0, H, 0, 9^*/
}rrc_pdcp_sps_ul_setup_info_t;

typedef struct _rrc_pdcp_sps_ul_info_t
{
    rrc_bitmask_t            optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_PDCP_SPS_UL_SETUP_INFO_PRESENT    0x01

    U8            request_type;
    /*^ M, 0, H, 0, 1^*/

    rrc_pdcp_sps_ul_setup_info_t    sps_ul_setup_info;
    /*^ TLV, RRC_PDCP_SPS_UL_SETUP_INFO_TAG, RRC_PDCP_SPS_UL_SETUP_INFO_PRESENT ^*/
}rrc_pdcp_sps_ul_info_t;


typedef struct _rrc_pdcp_sps_dl_setup_info_t
{
    U8 semi_persistent_sched_interval_dl;
    /*^ M, 0, H, 0, 9^*/
}rrc_pdcp_sps_dl_setup_info_t;

typedef struct _rrc_pdcp_sps_dl_info_t
{
    rrc_bitmask_t            optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_PDCP_SPS_DL_SETUP_INFO_PRESENT    0x01

    U8            request_type;
    /*^ M, 0, H, 0, 1^*/

    rrc_pdcp_sps_dl_setup_info_t    sps_dl_setup_info;
    /*^ TLV, RRC_PDCP_SPS_DL_SETUP_INFO_TAG, RRC_PDCP_SPS_DL_SETUP_INFO_PRESENT ^*/
}rrc_pdcp_sps_dl_info_t;

typedef struct _rrc_pdcp_sps_config_t
{

    rrc_bitmask_t             optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
#define RRC_PDCP_SPS_DL_INFO_PRESENT        0x01
#define RRC_PDCP_SPS_UL_INFO_PRESENT        0x02

    rrc_pdcp_sps_dl_info_t    sps_dl_info;
    /*^ TLV, RRC_PDCP_SPS_DL_INFO_TAG, RRC_PDCP_SPS_DL_INFO_PRESENT ^*/

    rrc_pdcp_sps_ul_info_t    sps_ul_info;
    /*^ TLV, RRC_PDCP_SPS_UL_INFO_TAG, RRC_PDCP_SPS_UL_INFO_PRESENT ^*/
}rrc_pdcp_sps_config_t;
/* SPS Stop */

/*SPR_17121_START*/
typedef struct _rrc_mac_i_msg_t
{
    rrc_lc_id_t          lc_id;
    rrc_rb_direction_t   rb_direction; /* rrc_rb_direction_et */

    rrc_counter_t       encoded_var_short_mac_i_data_length;  
/*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/

    U8                  encoded_var_short_mac_i[UECC_LLIM_MAX_BUF_LEN];  
/*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/

}rrc_mac_i_msg_t; 

typedef struct _rrc_mac_i_t
{
    U32     mac_i;
}rrc_mac_i_t;
/*SPR_17121_END*/


/*SPR_19066_START*/
typedef struct _rrc_pdcp_ue_inactivity_timer_t
{
    U32     ue_inactive_time_config;
}rrc_pdcp_ue_inactivity_timer_t; 
/*SPR_19066_END*/

typedef struct _rrc_pdcp_cr_ue_entity_req_t
{
    rrc_bitmask_t                   optional_elems_present;
    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_ue_index_t                  ue_index;
    U16                             crnti;

    rrc_counter_t                   num_cr_srb_entity;
    /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_cr_srb_entity_t        cr_srb_entities[RRC_PDCP_MAX_SRB];
    /*^ TLV, SEQUENCE, RRC_PDCP_CREATE_SRB_ENTITY_TAG ^*/

    rrc_counter_t                   num_cr_drb_entity;
    /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_cr_drb_entity_t        cr_drb_entities[RRC_PDCP_MAX_DRB];
    /*^ TLV, SEQUENCE, RRC_PDCP_CREATE_DRB_ENTITY_TAG ^*/

    rrc_pdcp_config_int_t           config_integrity_protection;
    /*^ TLV, RRC_PDCP_CONFIGURE_INTEGRITY_PROTECTION_TAG, RRC_PDCP_CR_SRB_CONFIG_INT_PRESENT ^*/

    rrc_pdcp_config_srb_ciph_t      config_srb_ciphering;
    /*^ TLV, RRC_PDCP_CONFIGURE_SRB_CIPHERING_TAG, RRC_PDCP_CR_SRB_CONFIG_CIPH_PRESENT ^*/

    rrc_pdcp_config_drb_ciph_t      config_drb_ciphering;
    /*^ TLV, RRC_PDCP_CONFIGURE_DRB_CIPHERING_TAG, RRC_PDCP_CR_DRB_CONFIG_CIPH_PRESENT ^*/

    rrc_pdcp_ho_triggered_t         ho_triggered; 
    /*^ TLV, RRC_PDCP_HO_TRIGGER_TAG, RRC_PDCP_HO_TRIGGER_PRESENT ^*/  

    /* SPS Start */
    rrc_pdcp_sps_config_t           sps_config;
    /*^ TLV, RRC_PDCP_SPS_CONFIG_TAG, RRC_PDCP_SPS_CONFIG_PRESENT ^*/
    /* SPS Stop */

    /*SPR_17121_START*/
    rrc_mac_i_msg_t                 mac_i_req;
    /*^ TLV, RRC_PDCP_MACI_REQ_TAG, RRC_PDCP_CONFIG_MACI_REQ_PRESENT ^*/
    /*SPR_17121_END*/

    /*SPR_19066_START*/
    rrc_pdcp_ue_inactivity_timer_t  ue_inactivity_timer;
    /*^ TLV, RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_TAG, RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_PRESENT ^*/
    /*SPR_19066_END*/
} rrc_pdcp_cr_ue_entity_req_t; /*^ API, RRC_PDCP_CREATE_UE_ENTITY_REQ ^*/

/******************************************************************************
*   PDCP_CREATE_UE_ENTITY_CNF
******************************************************************************/
/*SPR_17121_START*/
#define RRC_PDCP_CONFIG_MACI_RESP_PRESENT 0x01
/*SPR_17121_END*/
typedef struct _rrc_pdcp_cr_ue_entity_cnf_t
{
    rrc_bitmask_t                   optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_ue_index_t                  ue_index;
    rrc_response_t                  response_code;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_pdcp_return_et */


    rrc_counter_t                   num_cr_srb_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_cr_srb_entity_error_t  cr_srb_error_entities[RRC_PDCP_MAX_SRB];
/*^ TLV, SEQUENCE, RRC_PDCP_CREATE_SRB_ENTITY_ERROR_TAG ^*/


    rrc_counter_t                   num_cr_drb_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_cr_drb_entity_error_t  cr_drb_error_entities[RRC_PDCP_MAX_DRB];
/*^ TLV, SEQUENCE, RRC_PDCP_CREATE_DRB_ENTITY_ERROR_TAG ^*/

    /*SPR_17121_START*/
    rrc_mac_i_t                         mac_i;
/*^ TLV, RRC_PDCP_MACI_RESP_TAG, RRC_PDCP_CONFIG_MACI_RESP_PRESENT ^*/
    /*SPR_17121_END*/

} rrc_pdcp_cr_ue_entity_cnf_t; /*^ API, RRC_PDCP_CREATE_UE_ENTITY_CNF ^*/

/******************************************************************************
*   PDCP_DELETE_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_pdcp_del_ue_entity_req_t
{
    rrc_ue_index_t  ue_index;
} rrc_pdcp_del_ue_entity_req_t; /*^ API, RRC_PDCP_DELETE_UE_ENTITY_REQ ^*/

/******************************************************************************
*   PDCP_DELETE_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_pdcp_del_ue_entity_cnf_t
{
    rrc_ue_index_t  ue_index;
    rrc_response_t  response;   /*^ M, 0, H, 0, 1 ^*/ /* rrc_pdcp_return_et */
} rrc_pdcp_del_ue_entity_cnf_t; /*^ API, RRC_PDCP_DELETE_UE_ENTITY_CNF ^*/

/******************************************************************************
*   PDCP_RECONFIG_UE_ENTITY_REQ
******************************************************************************/

/******************************************************************************
*   PDCP_UPDATE_LWA_TRAFFIC_IND
******************************************************************************/
typedef struct _rrc_pdcp_update_lwa_traffic_ind_t
{
    rrc_ue_index_t              ue_index;
    rrc_lc_id_t                 lc_id;
    U16                         traffic_ind;
/* lwa_traffic_ind_et */
}rrc_pdcp_update_lwa_traffic_ind_t; /*^ API, RRC_PDCP_UPDATE_LWA_TRAFFIC_IND ^*/


typedef struct _rrc_pdcp_sec_offset_t
{
    U8  configure_sec_offset;  
    /*^ M, 0, B, 1, 1 ^*/
} rrc_pdcp_sec_offset_t;

/* SPS Start */
#define RRC_PDCP_RECONFIG_UE_SPS_CONFIG_PRESENT    0x20
/* SPS Stop */
/*SPR_17121_START*/
#define RRC_PDCP_RECONFIG_MACI_REQ_PRESENT  0x40
/*SPR_17121_END*/
/*SPR_19066_START*/
#define RRC_PDCP_RECONFIG_UE_INACTIVITY_TIMER_CONFIG_PRESENT    0x80
/*SPR_19066_END*/
typedef struct _rrc_pdcp_reconf_ue_entity_req_t
{
    rrc_bitmask_t               optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
     
    rrc_ue_index_t              ue_index;

    rrc_counter_t               num_cr_srb_entity;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_cr_srb_entity_t    cr_srb_entities[RRC_PDCP_MAX_SRB];
/*^ TLV, SEQUENCE, RRC_PDCP_CREATE_SRB_ENTITY_TAG ^*/


    rrc_counter_t               num_cr_drb_entity;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_cr_drb_entity_t    cr_drb_entities[RRC_PDCP_MAX_DRB];
/*^ TLV, SEQUENCE, RRC_PDCP_CREATE_DRB_ENTITY_TAG ^*/

    rrc_counter_t               num_reconfig_srb_entity;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_rcfg_srb_entity_t  rcfg_srb_entities[RRC_PDCP_MAX_SRB];
/*^ TLV, SEQUENCE, RRC_PDCP_RECONFIG_SRB_ENTITY_TAG ^*/

    rrc_counter_t               num_reconfig_drb_entity;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_rcfg_drb_entity_t  rcfg_drb_entities[RRC_PDCP_MAX_DRB];
/*^ TLV, SEQUENCE, RRC_PDCP_RECONFIG_DRB_ENTITY_TAG ^*/

    rrc_pdcp_sec_offset_t       pdcp_security_offset;
/*^ TLV, RRC_PDCP_SECURITY_OFFSET_TAG, RRC_PDCP_SECURITY_OFFSET_PRESENT ^*/

    rrc_pdcp_config_int_t       config_integrity_protection;
/*^ TLV, RRC_PDCP_CONFIGURE_INTEGRITY_PROTECTION_TAG, RRC_PDCP_CR_SRB_CONFIG_INT_PRESENT ^*/
 
    rrc_pdcp_config_srb_ciph_t      config_srb_ciphering;
/*^ TLV, RRC_PDCP_CONFIGURE_SRB_CIPHERING_TAG, RRC_PDCP_CR_SRB_CONFIG_CIPH_PRESENT ^*/

    rrc_pdcp_config_drb_ciph_t      config_drb_ciphering;
/*^ TLV, RRC_PDCP_CONFIGURE_DRB_CIPHERING_TAG, RRC_PDCP_CR_DRB_CONFIG_CIPH_PRESENT ^*/

    rrc_counter_t               num_del_srb_entity;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_del_srb_entity_t   del_srb_entities[RRC_PDCP_MAX_SRB];
/*^ TLV, SEQUENCE, RRC_PDCP_DELETE_SRB_ENTITY_TAG ^*/


    rrc_counter_t               num_del_drb_entity;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_del_drb_entity_t   del_drb_entities[RRC_PDCP_MAX_DRB];
/*^ TLV, SEQUENCE, RRC_PDCP_DELETE_DRB_ENTITY_TAG ^*/

    /* SPS Start */
    rrc_pdcp_sps_config_t        sps_config;
    /*^ TLV, RRC_PDCP_SPS_CONFIG_TAG, RRC_PDCP_RECONFIG_UE_SPS_CONFIG_PRESENT ^*/
    /* SPS Stop */

    /*SPR_17121_START*/
    rrc_mac_i_msg_t              mac_i_req;
/*^ TLV, RRC_PDCP_MACI_REQ_TAG, RRC_PDCP_RECONFIG_MACI_REQ_PRESENT ^*/
    /*SPR_17121_END*/

    /*SPR_19066_START*/
    rrc_pdcp_ue_inactivity_timer_t  ue_inactivity_timer; 
/*^ TLV, RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_TAG, RRC_PDCP_RECONFIG_UE_INACTIVITY_TIMER_CONFIG_PRESENT ^*/
    /*SPR_19066_END*/
} rrc_pdcp_reconf_ue_entity_req_t; /*^ API, RRC_PDCP_RECONFIG_UE_ENTITY_REQ ^*/

/******************************************************************************
*   PDCP_RECONFIG_UE_ENTITY_CNF
******************************************************************************/
/*SPR_17121_START*/
#define RRC_PDCP_RECONFIG_MACI_RESP_PRESENT 0x01
typedef struct _rrc_pdcp_reconfig_ue_entity_cnf_t
{
    rrc_bitmask_t               optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
/*SPR_17121_END*/

    rrc_ue_index_t                      ue_index;
    rrc_response_t                      response_code;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_pdcp_return_et */


    rrc_counter_t                       num_cr_srb_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_cr_srb_entity_error_t      cr_srb_error_entities[RRC_PDCP_MAX_SRB];
/*^ TLV, SEQUENCE, RRC_PDCP_CREATE_SRB_ENTITY_ERROR_TAG ^*/


    rrc_counter_t                       num_cr_drb_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_cr_drb_entity_error_t      cr_drb_error_entities[RRC_PDCP_MAX_DRB];
/*^ TLV, SEQUENCE, RRC_PDCP_CREATE_DRB_ENTITY_ERROR_TAG ^*/


    rrc_counter_t                       num_del_srb_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_del_srb_entity_error_t    del_srb_error_entities[RRC_PDCP_MAX_SRB];
/*^ TLV, SEQUENCE, RRC_PDCP_DELETE_SRB_ENTITY_ERROR_TAG ^*/


    rrc_counter_t                       num_del_drb_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_del_drb_entity_error_t    del_drb_error_entities[RRC_PDCP_MAX_DRB];
/*^ TLV, SEQUENCE, RRC_PDCP_DELETE_DRB_ENTITY_ERROR_TAG ^*/


    rrc_counter_t                       num_rcfg_srb_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_rcfg_srb_entity_error_t  rcfg_srb_error_entities[RRC_PDCP_MAX_SRB];
/*^ TLV, SEQUENCE, RRC_PDCP_RECONFIG_SRB_ENTITY_ERROR_TAG ^*/


    rrc_counter_t                       num_rcfg_drb_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_rcfg_drb_entity_error_t  rcfg_drb_error_entities[RRC_PDCP_MAX_DRB];
/*^ TLV, SEQUENCE, RRC_PDCP_RECONFIG_DRB_ENTITY_ERROR_TAG ^*/

    /*SPR_17121_START*/
    rrc_mac_i_t                         mac_i;
/*^ TLV, RRC_PDCP_MACI_RESP_TAG, RRC_PDCP_RECONFIG_MACI_RESP_PRESENT ^*/
    /*SPR_17121_END*/

} rrc_pdcp_reconfig_ue_entity_cnf_t;
/*^ API, RRC_PDCP_RECONFIG_UE_ENTITY_CNF ^*/


/******************************************************************************
*   PDCP_SRB_DATA_REQ
******************************************************************************/
typedef struct _rrc_pdcp_srb_data_req_t
{
    U32             transaction_id;
    rrc_ue_index_t  ue_index;
    rrc_lc_id_t     lc_id;
    U8              service_requested;
    U8              p_buffer[0]; /*^ M, 0, OCTET_STRING, TILL_THE_END ^*/
} rrc_pdcp_srb_data_req_t; /*^ API, ONLY_PUP, RRC_PDCP_SRB_DATA_REQ ^*/

/******************************************************************************
*   PDCP_SRB_DATA_IND
******************************************************************************/
typedef struct _rrc_pdcp_srb_data_ind_t
{
    rrc_ue_index_t  ue_index;
    rrc_lc_id_t     lc_id;
    U8              p_buffer[0]; /*^ M, 0, OCTET_STRING, TILL_THE_END ^*/
} rrc_pdcp_srb_data_ind_t; /*^ API, ONLY_PUP, RRC_PDCP_SRB_DATA_IND ^*/

/******************************************************************************
*   PDCP_SRB_DATA_STATUS_IND
******************************************************************************/

#define RRC_PDCP_SRB_DATA_STATUS_ERROR_PRESENT  0x01

typedef struct _rrc_pdcp_srb_data_status_ind_t
{
    rrc_bitmask_t                       optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    U32                                 transaction_id;
    rrc_ue_index_t                      ue_index;
    rrc_lc_id_t                         lc_id;
    rrc_response_t                      response_code;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_pdcp_return_et */

    rrc_pdcp_data_status_error_t        srb_data_status_error;
/*^ TLV, RRC_PDCP_SRB_DATA_STATUS_ERROR_TAG, RRC_PDCP_SRB_DATA_STATUS_ERROR_PRESENT ^*/

} rrc_pdcp_srb_data_status_ind_t; /*^ API, RRC_PDCP_SRB_DATA_STATUS_IND ^*/


/******************************************************************************
*   PDCP_SUSPEND_UE_ENTITY_REQ
******************************************************************************/

typedef struct _rrc_pdcp_suspend_ue_entity_req_t
{
    rrc_ue_index_t                      ue_index;
} rrc_pdcp_suspend_ue_entity_req_t; /*^ API, RRC_PDCP_SUSPEND_UE_ENTITY_REQ ^*/

/******************************************************************************
*   PDCP_SUSPEND_UE_ENTITY_CNF
******************************************************************************/

typedef struct _rrc_pdcp_suspend_ue_entity_cnf_t
{
    rrc_ue_index_t                      ue_index;
    rrc_response_t                      response_code;
} rrc_pdcp_suspend_ue_entity_cnf_t; /*^ API, RRC_PDCP_SUSPEND_UE_ENTITY_CNF ^*/

/* lossy ho changes start */
typedef struct _rrc_lossy_ho_required_t
{
    rrc_lc_id_t   lc_id;
}rrc_lossy_ho_required_t;
/* lossy ho changes stop */

/******************************************************************************
*   RRC_PDCP_SN_HFN_STATUS_REQ
******************************************************************************/
typedef struct _rrc_pdcp_sn_hfn_status_req_t
{
    rrc_ue_index_t                      ue_index;
    rrc_bool_t                          sn_hfn_required;
    
    /* lossy ho changes start */
    rrc_counter_t                       num_lossy_drb_entity;
    /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_lossy_ho_required_t             lossy_ho_required[RRC_PDCP_MAX_DRB];
    /*^ TLV, SEQUENCE, RRC_PDCP_LOSSY_HO_REQUIRED_TAG ^*/
    /* lossy ho changes stop */

}rrc_pdcp_sn_hfn_status_req_t; /*^ API, RRC_PDCP_SN_HFN_STATUS_REQ ^*/


/******************************************************************************
*   RRC_PDCP_SN_HFN_STATUS_RESP
******************************************************************************/

typedef struct _ul_rcv_sn_val_t
{
    U8    ul_rcv_sn[512]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}ul_rcv_sn_val_t;

#define RRC_PDCP_UL_RECV_SN_PRESENT   0x01

typedef struct _rrc_lc_sn_hfn_status_t
{
    rrc_bitmask_t       optional_elems_present; 
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_lc_id_t   lc_id;
    U16             dl_sn_cnt;   /*^ M, 0, H, 0, 4095 ^*/
    U32             dl_hfn_cnt; /*^ M, 0, H, 0, 1048575 ^*/
    U16             ul_sn_cnt;   /*^ M, 0, H, 0, 4095 ^*/
    U32             ul_hfn_cnt;  /*^ M, 0, H, 0, 1048575 ^*/
    ul_rcv_sn_val_t    ul_rcv_sn_val; 
/*^ TLV, RRC_PDCP_UL_RECV_SN_TAG, RRC_PDCP_UL_RECV_SN_PRESENT ^*/
}rrc_lc_sn_hfn_status_t;

typedef struct _rrc_pdcp_sn_hfn_status_resp_t
{
    rrc_ue_index_t           ue_index;
    rrc_response_t           response_code; 
/*^ M, 0, H, 0, 2 ^*/ /* rrc_pdcp_return_et */

    rrc_counter_t            num_lc_sn_hfn_status;
 /*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_lc_sn_hfn_status_t   lc_sn_hfn_status[RRC_PDCP_MAX_DRB];  
/*^ TLV, SEQUENCE, RRC_PDCP_LC_SN_HFN_STATUS_TAG ^*/

}rrc_pdcp_sn_hfn_status_resp_t; /*^ API, RRC_PDCP_SN_HFN_STATUS_RESP ^*/


/******************************************************************************
*   RRC_PDCP_SN_HFN_STATUS_IND
******************************************************************************/
typedef struct _rrc_pdcp_sn_hfn_status_ind_t
{
    rrc_ue_index_t           ue_index;
    
    rrc_counter_t                     num_lc_sn_hfn_status;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/
    rrc_lc_sn_hfn_status_t   lc_sn_hfn_status[RRC_PDCP_MAX_DRB]; 
/*^ TLV, SEQUENCE, RRC_PDCP_LC_SN_HFN_STATUS_TAG ^*/

}rrc_pdcp_sn_hfn_status_ind_t; /*^ API, RRC_PDCP_SN_HFN_STATUS_IND ^*/

/******************************************************************************
*   RRC_PDCP_DATA_BUFFER_STOP_IND
******************************************************************************/
typedef struct _rrc_pdcp_data_buffer_stop_ind_t
{
    rrc_ue_index_t           ue_index;
}rrc_pdcp_data_buffer_stop_ind_t; /*^ API, RRC_PDCP_DATA_BUFFER_STOP_IND ^*/


/******************************************************************************
*   RRC_PDCP_MAC_I_REQ
******************************************************************************/

typedef struct _rrc_message_t
{
    rrc_counter_t   cell_identity_data_length; 
/*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/

    U8   cell_identity[4]; 
/*^ M, 0, OCTET_STRING, FIXED ^*/

    rrc_counter_t   encoded_var_short_mac_i_data_length;  
/*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/

    U8   encoded_var_short_mac_i[UECC_LLIM_MAX_BUF_LEN];  
/*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/
    
}rrc_message_t;

typedef struct _rrc_pdcp_mac_i_req_t
{
    rrc_ue_index_t      ue_index;
    rrc_lc_id_t             lc_id;
    rrc_rb_direction_t   rb_direction; /* rrc_rb_direction_et */

    rrc_counter_t      num_message; 
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_message_t    message[MAX_NO_OF_MESSAGES]; 
/*^ TLV, SEQUENCE, RRC_PDCP_MESSAGE_TAG ^*/

}rrc_pdcp_mac_i_req_t; /*^ API, RRC_PDCP_MAC_I_REQ ^*/


/******************************************************************************
*   RRC_PDCP_MAC_I_RESP
******************************************************************************/

typedef struct _rrc_msg_authentication_code_t
{
    rrc_counter_t   cell_identity_data_length; 
/*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/

    U8   cell_identity[4]; 
/*^ M, 0, OCTET_STRING, FIXED ^*/

    rrc_response_t      response_code; 
/*^ M, 0, H, 0, 1 ^*/ /* rrc_pdcp_return_et */

    U32                      mac_i;
}rrc_msg_authentication_code_t;

typedef struct _rrc_pdcp_mac_i_resp_t
{
    rrc_ue_index_t           ue_index;
    rrc_response_t           response_code; 
/*^ M, 0, H, 0, 2 ^*/ /* rrc_pdcp_return_et */

    rrc_counter_t             num_var_short_mac_i; 
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_msg_authentication_code_t  rrc_msg_authentication_code[MAX_NO_OF_MESSAGES]; 
/*^ TLV, SEQUENCE, RRC_PDCP_MSG_AUTHENTICATION_CODE_TAG ^*/

}rrc_pdcp_mac_i_resp_t; /*^ API, RRC_PDCP_MAC_I_RESP ^*/

/******************************************************************************
*   PDCP_RE_ESTABLISH_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_pdcp_re_establish_ue_entity_req_t
{
    rrc_ue_index_t                  ue_index;

    rrc_counter_t                   num_re_est_rb_entity;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_rb_entity_t    re_est_rb_entities[RRC_PDCP_MAX_RB];
/*^ TLV, SEQUENCE, RRC_PDCP_RB_ENTITY_TAG ^*/
} rrc_pdcp_re_establish_ue_entity_req_t; /*^ API, RRC_PDCP_RE_ESTABLISH_UE_ENTITY_REQ ^*/

/******************************************************************************
*   PDCP_RE_ESTABLISH_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_pdcp_re_establish_ue_entity_cnf_t
{
    rrc_ue_index_t                      ue_index;
    rrc_response_t                      response_code;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_pdcp_return_et */

    rrc_counter_t                       num_re_est_rb_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_rb_entity_error_t   re_est_rb_error_entities[RRC_PDCP_MAX_RB];
/*^ TLV, SEQUENCE, RRC_PDCP_RB_ENTITY_ERROR_TAG ^*/

} rrc_pdcp_re_establish_ue_entity_cnf_t;
/*^ API, RRC_PDCP_RE_ESTABLISH_UE_ENTITY_CNF ^*/

/******************************************************************************
*   PDCP_RESUME_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_pdcp_resume_ue_entity_req_t
{
    rrc_ue_index_t                  ue_index;
    rrc_rb_direction_t              resume_direction;
    /* rrc_rb_direction_et */

    rrc_counter_t                   num_resume_rb_entity;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_rb_entity_t    resume_rb_entities[RRC_PDCP_MAX_RB];
/*^ TLV, SEQUENCE, RRC_PDCP_RB_ENTITY_TAG ^*/
} rrc_pdcp_resume_ue_entity_req_t; /*^ API, RRC_PDCP_RESUME_UE_ENTITY_REQ ^*/

/******************************************************************************
*   PDCP_RESUME_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_pdcp_resume_ue_entity_cnf_t
{
    rrc_ue_index_t                      ue_index;
    rrc_response_t                      response_code;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_pdcp_return_et */

    rrc_counter_t                       num_resume_rb_entity_error;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_rb_entity_error_t  resume_rb_error_entities[RRC_PDCP_MAX_RB];
/*^ TLV, SEQUENCE, RRC_PDCP_RB_ENTITY_ERROR_TAG ^*/

} rrc_pdcp_resume_ue_entity_cnf_t;
/*^ API, RRC_PDCP_RESUME_UE_ENTITY_CNF ^*/

/******************************************************************************
*  RRC_PDCP_CHANGE_CRNTI_REQ
******************************************************************************/
typedef struct _rrc_pdcp_change_crnti_req_t
{
    rrc_ue_index_t  ue_index;
    rrc_rnti_t      old_crnti;
    rrc_rnti_t      new_crnti;
} rrc_pdcp_change_crnti_req_t; /*^ API, RRC_PDCP_CHANGE_CRNTI_REQ ^*/

/******************************************************************************
* RRC_PDCP_CHANGE_CRNTI_CNF
******************************************************************************/
typedef struct _rrc_pdcp_change_crnti_cnf_t
{
    rrc_ue_index_t  ue_index;

    rrc_response_t  response;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_pdcp_return_et */

} rrc_pdcp_change_crnti_cnf_t; /*^ API, RRC_PDCP_CHANGE_CRNTI_CNF ^*/

/******************************************************************************
* RRC_PDCP_COUNT_WRAPAROUND_IND
******************************************************************************/
typedef struct _rrc_pdcp_count_wraparound_ind_t
{
    rrc_ue_index_t  ue_index;
} rrc_pdcp_count_wraparound_ind_t; /*^ API, RRC_PDCP_COUNT_WRAPAROUND_IND ^*/

/* SPR 4118 Start */
/******************************************************************************
* RRC_PDCP_NOTIFY_INTEGRITY_FAILURE
******************************************************************************/
typedef struct _rrc_pdcp_notify_integrity_failure_t
{
    rrc_ue_index_t  ue_index;

    rrc_lc_id_t     lc_id;
    /*^ M, 0, B, 1, 2 ^*/

}rrc_pdcp_notify_integrity_failure_t; /*^ API, RRC_PDCP_NOTIFY_INTEGRITY_FAILURE ^*/
/* SPR 4118 Stop */

/******************************************************************************
* RRC_PDCP_DRB_COUNT_MSB_REQ
******************************************************************************/
typedef struct _rrc_pdcp_drb_count_msb_req_t
{
    rrc_ue_index_t  ue_index;
    rrc_counter_t                   num_drb;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_drb_list_t    drb_list[MAX_LC_COUNT];
/*^ TLV, SEQUENCE, RRC_PDCP_DRB_COUNTMSB_DRB_LIST_TAG ^*/
} rrc_pdcp_drb_count_msb_req_t; /*^ API, RRC_PDCP_DRB_COUNT_MSB_REQ ^*/

/******************************************************************************
* RRC_PDCP_DRB_COUNT_MSB_RESP
******************************************************************************/

typedef struct _rrc_pdcp_drb_count_msb_resp_t
{
    rrc_ue_index_t  ue_index;

    rrc_response_t  response;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_pdcp_return_et */

    rrc_counter_t                   num_drb;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_drb_countMSB_info_t    drb_countMSB_info[MAX_LC_COUNT];
/*^ TLV, SEQUENCE, RRC_PDCP_DRB_COUNTMSB_INFO_TAG ^*/

} rrc_pdcp_drb_count_msb_resp_t; /*^ API, RRC_PDCP_DRB_COUNT_MSB_RESP ^*/


/******************************************************************************
*   RRC_PDCP_CELL_CONFIG_REQ
******************************************************************************/
typedef struct _pdcp_communication_info_t
{
    U8     rlcInstance; /*^ M, 0, N, 0, 0 ^*/
    U16   rlcRxPort;    /*^ M, 0, N, 0, 0 ^*/
    U8    rlcIpAddr[16];  /*^ M,0, OCTET_STRING, FIXED ^*/
}pdcp_communication_info_t;


typedef struct _rrc_pdcp_config_cell_req_t
{
U16			Cell_index; /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
pdcp_communication_info_t	pdcp_communication_info; /*^ M, 0, N, 0, 0 ^*/		
} rrc_pdcp_config_cell_req_t; /*^ API, RRC_PDCP_CELL_CONFIG_REQ ^*/


/******************************************************************************
*   RRC_PDCP_CELL_CONFIG_CNF
******************************************************************************/
typedef struct _rrc_pdcp_config_cell_cnf_t
{
    rrc_cell_index_t    cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
    U8	response;
    /*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */
} rrc_pdcp_config_cell_cnf_t; /*^ API, RRC_PDCP_CELL_CONFIG_CNF ^*/


/*SPR_19066_START*/
/******************************************************************************
* RRC_PDCP_HO_PREP_INFO_REQ
******************************************************************************/

#define RRC_PDCP_MACI_MSG_REQ_PRESENT                   0x01
#define RRC_PDCP_HO_PREP_UE_INACTIVE_TIME_REQ_PRESENT   0x02
typedef struct _rrc_mac_i_msg_req_t
{
    rrc_lc_id_t                         lc_id;
    rrc_rb_direction_t                  rb_direction; /* rrc_rb_direction_et */

    rrc_counter_t                       num_message; 
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_message_t                       message[MAX_NO_OF_MESSAGES]; 
/*^ TLV, SEQUENCE, RRC_PDCP_MESSAGE_TAG ^*/
}rrc_mac_i_msg_req_t;

typedef struct _rrc_ue_inactivity_time_val_t
{
    U8                                  ue_inactive_time_val_flag;  /*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */
}rrc_ue_inactivity_time_val_t;

typedef struct _rrc_pdcp_ho_prep_info_req_t
{
    rrc_bitmask_t                       optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
     
    rrc_ue_index_t                      ue_index;

    rrc_mac_i_msg_req_t                 mac_i_msg_req;
/*^ TLV, RRC_PDCP_MACI_HO_REQ_TAG, RRC_PDCP_MACI_MSG_REQ_PRESENT ^*/

    rrc_ue_inactivity_time_val_t              ue_inactive_time_val; 
/*^ TLV, RRC_PDCP_UE_INACTIVE_TIME_REQ_TAG, RRC_PDCP_HO_PREP_UE_INACTIVE_TIME_REQ_PRESENT ^*/
} rrc_pdcp_ho_prep_info_req_t; /*^ API, RRC_PDCP_HO_PREP_INFO_REQ ^*/

/******************************************************************************
* RRC_PDCP_HO_PREP_INFO_RESP
******************************************************************************/

#define RRC_PDCP_HO_PREP_UE_INACTIVE_TIME_RESP_PRESENT      0x01
typedef struct _rrc_pdcp_ho_prep_info_resp_t
{
    rrc_bitmask_t                       optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
     
    rrc_ue_index_t                      ue_index;

    rrc_response_t                      response_code;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_pdcp_return_et */

    rrc_counter_t                       num_var_short_mac_i; 
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_msg_authentication_code_t       rrc_msg_authentication_code[MAX_NO_OF_MESSAGES]; 
/*^ TLV,SEQUENCE, RRC_PDCP_MSG_AUTHENTICATION_CODE_TAG ^*/

    rrc_ue_inactive_time_t              ue_inactivity_time_resp; 
/*^ TLV, RRC_PDCP_UE_INACTIVE_TIME_RESP_TAG, RRC_PDCP_HO_PREP_UE_INACTIVE_TIME_RESP_PRESENT ^*/
} rrc_pdcp_ho_prep_info_resp_t; /*^ API, RRC_PDCP_HO_PREP_INFO_RESP ^*/

/******************************************************************************
* RRC_PDCP_INACTIVE_UES_IND
******************************************************************************/

#define RRC_PDCP_MAX_INACTIVE_UES               10
typedef struct _rrc_pdcp_inactive_ue_info_t
{
    rrc_ue_index_t   ue_index;
}rrc_pdcp_inactive_ue_info_t;

typedef struct _rrc_pdcp_inactive_ues_ind_t
{
    rrc_bitmask_t                       optional_elems_present;
/*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
     
    U8    num_of_inactive_ue; /*^ M, 0, B, 1, 10 ^*/

    rrc_counter_t                 num_of_inactive_ue_counter;
/*^ M, 0, TLV_SEQUENCE_COUNTER, NOT_PRESENT_IN_MESSAGE ^*/

    rrc_pdcp_inactive_ue_info_t    inactive_ue_info[RRC_MAX_INACTIVE_UES];
/*^ TLV, SEQUENCE, RRC_PDCP_INACTIVE_UE_INFO_TAG ^*/
} rrc_pdcp_inactive_ues_ind_t; /*^ API, RRC_PDCP_INACTIVE_UES_IND ^*/
/*SPR_19066_END*/
#pragma pack(pop)

#endif /* _RRC_PDCP_INTF_H_ */

