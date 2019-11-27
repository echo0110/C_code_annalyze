/******************************************************************************
*
*   FILE NAME:
*       rrc_csc_llim_intf.h
*
*   DESCRIPTION:
*       This file contains types used for representation of CSC-LLIM API
*       inside RRC
*       Based on eNB_RRC_R1_0_DD_v0_21.doc.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   29 Apr 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_CSC_LLIM_INTF_H_
#define _RRC_CSC_LLIM_INTF_H_
#include "rrc_rrm_intf.h"
#include "rrc_defines.h"

#define MAX_RETRY_COUNT     1
#define MAX_SI_MSG_LENGTH   25000

typedef enum
{
    RRC_LLIM_CSC_FAILURE = 0,
    RRC_LLIM_CSC_SUCCESS = 1,
    RRC_LLIM_CELL_ALREADY_EXIST = 2,
    RRC_LLIM_CELL_NOT_ENOUGH_MEMORY,
    RRC_LLIM_CELL_IS_ABSENT,
    RRC_LLIM_CONFIG_CELL_FAIL_PHY,
    RRC_LLIM_CONFIG_CELL_TE_PHY,
    RRC_LLIM_CONFIG_CELL_FAIL_MAC,
    RRC_LLIM_CONFIG_CELL_TE_MAC,
    RRC_LLIM_SFN_FAIL,
    RRC_LLIM_SFN_TE,
    RRC_LLIM_RECONFIG_PHY_ONLY_MODE_SUCCESS,
    RRC_LLIM_RECONFIG_PHY_ONLY_MODE_FAIL,
    RRC_LLIM_RECONFIG_MAC_ONLY_MODE_SUCCESS,
    RRC_LLIM_RECONFIG_MAC_ONLY_MODE_FAIL,
    RRC_LLIM_RECONFIG_PHY_MAC_MODE_SUCCESS,
    RRC_LLIM_RECONFIG_PHY_MAC_MODE_MAC_FAIL,
    RRC_LLIM_RECONFIG_PHY_MAC_MODE_PHY_FAIL,
    RRC_LLIM_RECONFIG_PHY_ONLY_MODE_TIMER_EXP,
    RRC_LLIM_RECONFIG_PHY_MAC_MODE_PHY_TIMER_EXP,
    RRC_LLIM_RECONFIG_MAC_ONLY_MODE_TIMER_EXP,
    RRC_LLIM_RECONFIG_PHY_MAC_MODE_MAC_TIMER_EXP,
    RRC_LLIM_INVALID_CELL_INDEX,
    RRC_LLIM_CELL_START_SUCCESS,
    RRC_LLIM_CELL_START_PHY_FAIL,
    RRC_LLIM_CELL_START_MAC_FAIL,
    RRC_LLIM_CELL_STOP_SUCCESS,
    RRC_LLIM_CELL_STOP_PHY_FAIL,
    RRC_LLIM_CELL_STOP_MAC_FAIL,
    RRC_LLIM_INTERNAL_ERROR,
    RRC_LLIM_RECONFIG_MAC_RLC_MODE_SUCCESS,
    RRC_LLIM_RECONFIG_MAC_RLC_MODE_RLC_FAIL,
} rrc_llim_csc_response_et;

#pragma pack(push, 1)

/******************************************************************************
*   CSC_LLIM_CONFIG_CELL_REQ
******************************************************************************/
typedef struct _rrc_si_segment_data_t
{
    U16                     starting_offset;
}rrc_si_segment_data_t;


typedef struct _rrc_si_msg_info_t
{
    rrc_bitmask_t           bitmask;    /*^ BITMASK ^*/
#define RRC_CSC_LLIM_SI_START_OFFSET_INFO_PRESENT        0x01
#define RRC_CSC_LLIM_TIMESTAMP_REQUIRED_PRESENT          0x02
    U8                      si_index; 
    U8                      periodicity;
    rrc_sfn_t               starting_sfn;
    rrc_sf_t                starting_sf;
    U16                     num_of_repetitions;
    U16                     si_length;
    U8                      num_of_segment;
    rrc_si_segment_data_t   si_segment_data[MAX_SI_SEGMENTS];
    rrc_counter_t           si_msg_param_counter;
    rrc_mac_si_msg_param_t  si_msg_param[RRC_MAX_SI_MSG_PARAMS];
    U8                      num_of_transmission;
    rrc_counter_t           si_start_offset_counter;
    rrc_mac_si_start_offset_info_t  si_start_offset;
/*CDMA2000_CSFB_HO start*/
    U8                      rrc_mac_sched_reqd_for_sib8; /*sched_reqd_for_sib8_et*/
/*CDMA2000_CSFB_HO stop*/
}rrc_si_msg_info_t;


typedef struct _rrc_si_msg_req_t
{
    U8                  si_window_size;
    U8                  si_msg_buf[MAX_SI_MSG_LENGTH];
    U8                  num_si_message;
    rrc_si_msg_info_t   si_msg_info[RRC_MAX_SI_MSGS];
}rrc_si_msg_req_t;

typedef struct _rrc_cell_param_t
{
    mib_info_t                  mib_info;
    sib_type_1_Info_t           sib_type_1_Info;
    sib_type_2_Info_t           sib_type_2_Info;
    rrc_rrm_cell_config_t       cell_config_param;
}rrc_cell_param_t;


typedef struct _rrc_rrm_cell_setup_sib2_explicit_start_t
{

  U16                         presence_bitmask;         /*^ BITMASK ^*/
#define CELL_SETUP_REQ_API_EXPLICIT_START_REQUIRED_FLAG  0x01

  U8                          explicit_start_required;  /*^ O, 1, H, 0, 0 ^*/
}rrc_rrm_cell_setup_sib2_explicit_start_t;


typedef struct _rrc_csc_llim_config_cell_req_t
{
 
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    U32                     mod_period;
    U8                      sfn_gap;
    rrc_counter_t                       mib_msg_req_counter;
    rrc_mac_mib_msg_req_t               mib_msg_req[RRC_MIB_MSGS];
    rrc_mac_sibtype1_msg_req_t          sib1_msg_req;
    rrc_si_msg_req_t                    si_msg_req;
    rrc_cell_param_t                    llim_cell_param;
    rrc_rrm_cell_setup_sib2_explicit_start_t            llim_cell_setup_req;
} rrc_csc_llim_config_cell_req_t; /*^ API ^*/

/******************************************************************************
*   CSC_LLIM_CONFIG_CELL_RESP
******************************************************************************/

typedef struct _rrc_csc_llim_config_cell_resp_t
{
    rrc_transaction_id_t        transaction_id;

    rrc_bitmask_t               bitmask;    /*^ BITMASK ^*/
#define RRC_CSC_LLIM_SFN_INFO_PRESENT        0x01
#define RRC_CSC_LLIM_SF_INFO_PRESENT         0x02
#define RRC_CSC_LLIM_TIMESTAMP_INFO_PRESENT        0x04
    rrc_cell_index_t            cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    rrc_response_t              response;   /* rrc_llim_csc_response_et */

    rrc_sfn_t   sfn;
/*^ O, RRC_CSC_LLIM_SFN_INFO_PRESENT ^*/
/*CDMA2000_CSFB_HO start*/
    rrc_sf_t    sf;
/*^ O, RRC_CSC_LLIM_SF_INFO_PRESENT ^*/

    rrc_timestamp_t   timestamp;
/*^ O, RRC_CSC_LLIM_TIMESTAMP_INFO_PRESENT ^*/
/*CDMA2000_CSFB_HO stop*/

} rrc_csc_llim_config_cell_resp_t; /*^ API ^*/
/******************************************************************************
*   CSC_LLIM_RECONFIG_CELL_REQ
******************************************************************************/

typedef struct _broadcast_info_t
{
#define CELL_RECONFIG_REQ_MIB_INFO_PRESENCE_FLAG                0x01
#define CELL_RECONFIG_REQ_SIB_1_INFO_PRESENCE_FLAG              0x02
#define CELL_RECONFIG_REQ_SIB_2_INFO_PRESENCE_FLAG              0x04
#define CELL_RECONFIG_REQ_SIB_3_INFO_PRESENCE_FLAG              0x08
#define CELL_RECONFIG_REQ_SIB_4_INFO_PRESENCE_FLAG              0x10
#define CELL_RECONFIG_REQ_SIB_5_INFO_PRESENCE_FLAG              0x20
#define CELL_RECONFIG_REQ_SIB_6_INFO_PRESENCE_FLAG              0x40
#define CELL_RECONFIG_REQ_SIB_7_INFO_PRESENCE_FLAG              0x80
#define CELL_RECONFIG_REQ_SIB_8_INFO_PRESENCE_FLAG              0x100
#define CELL_RECONFIG_REQ_SIB_9_INFO_PRESENCE_FLAG              0x200
/* SPR 18002 fix Start */
#define CELL_RECONFIG_REQ_SIB_13_INFO_PRESENCE_FLAG             0x2000
/* SPR 18002 fix End */
#define CELL_RECONFIG_REQ_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG   0x800
/* SPR 18093 Fix Start */
#define CELL_RECONFIG_REQ_MCCH_UPDATED_AREA_INFO_LIST           0x1000
/* SPR 18093 Fix End */
  U16                           presence_bitmask;   /*^ BITMASK ^*/
  mib_info_t                    mib_info;    
  sib_type_1_Info_t             sib_type_1_Info;    
  sib_type_2_Info_t             sib_type_2_Info;
  sib_type_13_info_t            sib_type_13_info;
  mbms_updated_area_info_list_t mbms_updated_area_info_list;
  /* SPR 18672 Fix */
  m2ap_reserved_mcch_related_bcch_info_list_t    m2ap_reserved_mcch_related_bcch_info_list;
  sib13_scheduling_info_t       sib13_scheduling_info;                           
  /* SIB_2_DISABLE */
  U8                            is_mbsfn_disable_sib2;
  /* SA_FIX Start */
  U8                           reserved_area_index[MAX_MBMS_UPDATED_AREA_LIST];
  /* SA_FIX End */
  
}broadcast_info_t;


#define CELL_RECONFIG_REQ_BROADCAST_INFO_PRESENCE_FLAG           0x01
#define CELL_RECONFIG_REQ_NON_BROADCAST_INFO_PRESENCE_FLAG       0x02
#define CELL_RECONFIG_REQ_UE_INACTIVE_TIME_CONFIG_PRESENCE_FLAG  0x04 // corrected here from 0x03 to 0x04
/* CLPC Start */
#define CELL_RECONFIG_REQ_MAC_DOWNLINK_POWER_CONTROL_PRESENCE_FLAG         0x08
/* CLPC Stop */
#define CELL_RECONFIG_REQ_CONTENTION_FREE_RACH_TIMER_PRESENCE_FLAG         0x10
#define CELL_RECONFIG_REQ_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG      0x20

typedef struct _rrc_cell_reconfig_req_t
{
   U16                           presence_bitmask;
   broadcast_info_t              broadcast_info;
   non_broadcast_config_info_t   non_broadcast_info;
   U32                           ue_inactive_time_config;

   /* CLPC Start */
   rrc_mac_downlink_power_control_common_t downlink_power_control_common;
   /* CLPC Stop */

  U16  contention_free_rach_timer;
#ifdef LTE_EMBMS_SUPPORTED
  U16                          mbms_transmission_power;
#endif

}rrc_cell_reconfig_req_t;

typedef struct _rrc_csc_llim_reconfig_cell_req_t
{
#define RRC_CSC_MIB_MSG_REQ_PRESENT             0x01
#define RRC_CSC_SIB_TYPE_1_MSG_PRESENT          0x02
#define RRC_CSC_SI_MSG_REQ_PRESENT              0x04
#define RRC_CSC_MOD_PERIOD_PRESENT              0x08
#define RRC_CSC_SFN_GAP_PRESENT                 0x10
    rrc_bitmask_t                       bitmask;
    rrc_transaction_id_t                transaction_id;
    rrc_cell_index_t                    cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    /* SFN. SFN is 10 bits wide: 0, 1, 2, ... SFN_UPPER_LIMIT - 1 */
    rrc_sfn_t                           sfn;
    rrc_counter_t                       mib_msg_req_counter;
    /* eMTC changes start */
    rrc_mac_mib_msg_req_t               mib_msg_req[RRC_MIB_MSGS];
    /* eMTC changes stop */
    rrc_mac_sibtype1_msg_req_t          sib1_msg_req;
    rrc_si_msg_req_t                    si_msg_req;

    rrc_cell_reconfig_req_t             cell_reconfig;

    /* Reconfig Mode */
    rrc_reconfig_mode_et                mode;
    /*Updated Modification period*/
    U32                                 mod_period;
    U8                                  sfn_gap;

}rrc_csc_llim_reconfig_cell_req_t; /*^ API ^*/


/******************************************************************************
*   CSC_LLIM_RECONFIG_CELL_RESP
******************************************************************************/
typedef struct _rrc_csc_llim_reconfig_cell_resp_t
{
    rrc_transaction_id_t        transaction_id;
    rrc_cell_index_t            cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    rrc_llim_csc_response_et    response;   /* rrc_llim_csc_response_et */
}rrc_csc_llim_reconfig_cell_resp_t; /*^ API, CSC_LLIM_RECONFIG_CELL_RESP ^*/

/******************************************************************************
*   CSC_LLIM_DELETE_CELL_REQ
******************************************************************************/
typedef struct _rrc_csc_llim_delete_cell_req_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    /* SPR 21369 Start */
    U8                      l2_instance_reset_flag; /* Indicates the Layer2 presence during cell delelte */ 
    /* SPR 21369 End */
    /* SPR 18354 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
    mbms_updated_area_info_list_t mbms_updated_area_info_list;
    m2ap_reserved_mcch_related_bcch_info_list_t m2ap_reserved_mcch_related_bcch_info_list;
#endif
    /* SPR 18354 Fix End */
} rrc_csc_llim_delete_cell_req_t; /*^ API ^*/

/******************************************************************************
*   CSC_LLIM_DELETE_CELL_RESP
******************************************************************************/
typedef struct _rrc_csc_llim_delete_cell_resp_t
{
    rrc_transaction_id_t        transaction_id;
    rrc_cell_index_t            cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    rrc_response_t              response;   /* rrc_llim_csc_response_et */
}rrc_csc_llim_delete_cell_resp_t; /*^ API ^*/

/******************************************************************************
*   CSC_LLIM_SFN_IND
******************************************************************************/
typedef struct _rrc_csc_llim_sfn_ind_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */ 
    rrc_sfn_t               sfn;
/*CDMA2000_CSFB_HO start*/
    rrc_sf_t                sf;
    rrc_timestamp_t         timestamp;
/*CDMA2000_CSFB_HO stop*/
}rrc_csc_llim_sfn_ind_t; /*^ API ^*/

/******************************************************************************
*   CSC_LLIM_SFN_REQ
******************************************************************************/
typedef struct _rrc_csc_llim_sfn_req_t
{
    rrc_cell_index_t        cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */ 
}rrc_csc_llim_sfn_req_t; /*^ API ^*/

/******************************************************************************
*   CSC_LLIM_SFN_RESP
******************************************************************************/
typedef rrc_csc_llim_sfn_ind_t rrc_csc_llim_sfn_resp_t;

/******************************************************************************
*   CSC_LLIM_BCCH_CONFIG_REQ
******************************************************************************/
typedef struct _rrc_csc_llim_bcch_config_req_t
{
    rrc_bitmask_t               bitmask;    /*^ BITMASK ^*/
#define RRC_CSC_LLIM_BCCH_SIBTYPE1_MSG_REQ_PRESENT  0x01
#define RRC_CSC_LLIM_BCCH_SI_MSG_REQ_PRESENT        0x02

    rrc_cell_index_t            cell_index;
    /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    rrc_counter_t               mib_msg_req_counter;
    rrc_mac_mib_msg_req_t       mib_msg_req[RRC_MIB_MSGS];
    rrc_mac_sibtype1_msg_req_t  sib1_msg_req;
    /*^ O, RRC_CSC_LLIM_BCCH_SIBTYPE1_MSG_REQ_PRESENT ^*/

    rrc_si_msg_req_t            si_msg_req;
/*^ O, RRC_CSC_LLIM_BCCH_SI_MSG_REQ_PRESENT ^*/

}rrc_csc_llim_bcch_config_req_t; /*^ API ^*/

/******************************************************************************
*   CSC_LLIM_PCCH_DATA_REQ
******************************************************************************/
typedef struct _rrc_csc_llim_pcch_data_req_t
{
    rrc_transaction_id_t    transaction_id;
    rrc_cell_index_t        cell_index; 
    /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    rrc_sfn_t               paging_frame;
    rrc_sf_t                paging_sub_frame;
    U16                     num_of_sfn;


    rrc_counter_t           paging_msg_buffer_length;
    U8                      paging_msg_buffer[0];
}rrc_csc_llim_pcch_data_req_t; /*^ API ^*/

/********************************************************************************
 *  CSC_LLIM_PWS_REQ 
 ******************************************************************************/
typedef struct _rrc_csc_llim_pws_req_t
{
    rrc_transaction_id_t                transaction_id;
    rrc_cell_index_t                    cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    rrc_sfn_t                           sfn;
    rrc_mac_sibtype1_msg_req_t          sib1_msg_req;
    rrc_si_msg_req_t                    si_msg_req;
}rrc_csc_llim_pws_req_t;

/********************************************************************************
 *  CSC_LLIM_PWS_RESP 
 ******************************************************************************/
typedef struct _rrc_csc_llim_pws_resp_t
{
    rrc_transaction_id_t                transaction_id;
    rrc_cell_index_t                    cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    rrc_llim_csc_response_et            response;
}rrc_csc_llim_pws_resp_t;

/********************************************************************************
 *  CSC_LLIM_UPDATED_PWS_SI_REQ 
 ******************************************************************************/
typedef struct _rrc_csc_llim_updated_pws_si_req_t
{
    rrc_transaction_id_t                transaction_id;
    rrc_cell_index_t                    cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    rrc_sfn_t                           sfn;
    rrc_mac_sibtype1_msg_req_t          sib1_msg_req;
    rrc_si_msg_req_t                    si_msg_req;
}rrc_csc_llim_updated_pws_si_req_t;

/********************************************************************************
 *  CSC_LLIM_UPDATED_PWS_SI_RESP
 ******************************************************************************/
typedef struct _rrc_csc_llim_updated_pws_si_resp_t
{
    rrc_transaction_id_t                transaction_id;
    rrc_cell_index_t                    cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    rrc_llim_csc_response_et            response;
}rrc_csc_llim_updated_pws_si_resp_t;

/*******************************************************************************
 *  CSC_LLIM_CELL_START_REQ
 *****************************************************************************/ 
typedef struct _rrc_csc_llim_cell_start_req_t
{
    rrc_transaction_id_t                transaction_id;
    rrc_cell_index_t                    cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
}rrc_csc_llim_cell_start_req_t; /*^ API, CSC_LLIM_CELL_START_REQ ^*/

/*****************************************************************************
 *   CSC_LLIM_CELL_START_RESP
 **************************************************************************/ 
typedef struct _rrc_csc_llim_cell_start_resp_t
{
    rrc_transaction_id_t                transaction_id;
    rrc_cell_index_t                    cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    rrc_response_t                      response;    /* rrc_llim_csc_response_et */
    rrc_sfn_t                           sfn;
    /* cdma_fix bug 10186 start */
    rrc_sf_t                            sf;
    rrc_timestamp_t                     timestamp;
    /*cdma_fix bug 10186 stop*/
        
}rrc_csc_llim_cell_start_resp_t;/*^ API, CSC_LLIM_CELL_START_RESP ^*/

/****************************************************************************
 *   CSC_LLIM_CELL_STOP_REQ
 **************************************************************************/ 
typedef struct _rrc_csc_llim_cell_stop_req_t
{
    rrc_transaction_id_t                 transaction_id;
    rrc_cell_index_t                     cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
}rrc_csc_llim_cell_stop_req_t; /*^ API, CSC_LLIM_CELL_STOP_REQ ^*/

/***************************************************************************
 *    CSC_LLIM_CELL_STOP_RESP
 *************************************************************************/
typedef struct _rrc_csc_llim_cell_stop_resp_t
{
    rrc_transaction_id_t                 transaction_id;
    rrc_cell_index_t                     cell_index; /*^ M, 0, H, 0, 0 ^*/    /* MAX_NUM_CELLS - 1 */
    rrc_response_t                       response;    /* rrc_llim_csc_response_et*/
}rrc_csc_llim_cell_stop_resp_t; /*^ API, CSC_LLIM_CELL_STOP_RESP ^*/

#pragma pack(pop)

#endif /* _RRC_CSC_LLIM_INTF_H_ */

