/*! \file lte_rrm/common/include/rrm_cell_rrmim_intf.h
 *  \brief This file contains the structure for all from MIF.
 *  \date Feburary 29, 2012
 *  \author 
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   6 Jun, 2014     gur29990    11317           OLPC Design Change
 *   17 Jun,2014     gur30784    SPR 11564       DL forwarding list population

 */ 
/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
*/
/****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *  DATE         AUTHOR          REFERENCE          REASON
 * 01 May 2014    gur32417      CA STG 3       Carrier Aggregation Stage 3
 *                                             code changes
 * 6 June 2014    gur26515      SPR 10730      Added code changes for SPR 10730
 * 19 June 2014   gur27356      SPR 10489      Added DTM supported Param.
 * 24 Jul,2014    gur32417      SPR 12689      PCI conflict with serving issue changes 
****************************************************************************/

#ifndef _RRM_CELL_RRMIM_INTF_H_
#define _RRM_CELL_RRMIM_INTF_H_

/*! \headerfile rrm_defines.h <>
 */
#include "rrm_defines.h"
/*! \headerfile rrm_types.h <>
 */
#include "rrm_types.h"
/*! \headerfile rrm_oam_intf.h <>
 */
#include "rrm_oam_intf.h"
/*! \headerfile rrm_son_intf.h <>
 */
#include "rrm_son_intf.h"
/*! \headerfile rrm_oam_defines.h <>
 */
#include "rrm_oam_defines.h"
#include "rrm_oam_platform_intf.h"

#define RRM_MAX_PCI_VALUE       504
#ifdef LTE_EMBMS_SUPPORTED 
#include "rrc_m2apRrm_intf.h"
#endif

#ifdef LTE_EMBMS_SUPPORTED
/* SPR 22248 MBMS Changes Start */
typedef enum
{
   MBMS_NOTIF_REP_COEFF_N2 = 2,
   MBMS_NOTIF_REP_COEFF_N4 = 4,
}mbms_notif_rep_coeff_et;
/* SPR 22248 MBMS Changes Stop */

/* SPR 18705:start */
typedef m2ap_rrm_mbms_sf_info_req_t rrm_m2ap_enque_sf_info_req_t ; 
typedef struct _rrm_enqueued_sf_info_req_node_t
{
    YLNODE                        s_node;
    rrm_m2ap_enque_sf_info_req_t  enqueue_sf_info_req_node;
    U16                           api_id;
    U16                           trans_id;
}rrm_enqueued_sf_info_req_node_t;

typedef YLIST rrm_enqueued_sf_info_req_list_t ;/*rrm_enqueued_sf_info_req_node_t*/ 
/* SPR 18705:end */

typedef enum
{
    RRMCM_MIF_FRAME_ALLOCATION_PERIOD_N1 = 1,
    RRMCM_MIF_FRAME_ALLOCATION_PERIOD_N2 = 2,
    RRMCM_MIF_FRAME_ALLOCATION_PERIOD_N4 = 4,
    RRMCM_MIF_FRAME_ALLOCATION_PERIOD_N8 = 8,
    RRMCM_MIF_FRAME_ALLOCATION_PERIOD_N16 = 16,
    RRMCM_MIF_FRAME_ALLOCATION_PERIOD_N32 = 32 
}rrmcm_mif_frame_allocation_period_et;
typedef enum
{
 RRMCM_MCCH_REPETITION_PERIOD_RF32 = 32,
 RRMCM_MCCH_REPETITION_PERIOD_RF64 = 64,
 RRMCM_MCCH_REPETITION_PERIOD_RF128 = 128,
 RRMCM_MCCH_REPETITION_PERIOD_RF256 = 256
}rrm_m2ap_rep_period_mcch_et;
typedef struct _rrmcm_subframe_allocation_t
{
    U8  sf_alloc_choice;        
    U8  subframe_allocation[SUB_FRAME_ALLOC_FOUR_FRAME_OCTET_SIZE];
}rrmcm_subframe_allocation_t;
#define RRMCM_RMIF_SYNC_STRATUM_ID_PRESENT  0x01
typedef struct _rrmcm_mbsfn_subframe_config_info_t
{
    rrm_bitmask_t                            bitmask; /* BITMASK */
    rrmcm_mif_frame_allocation_period_et     radio_frame_allocation_period;  /*rrmcm_mif_frame_allocation_period_et*/ 
    U8                                       radio_frame_allocation_offset;          
    rrmcm_subframe_allocation_t              subframe_allocation;  
    U8                                       sync_stratum_id; /* RRMCM_RMIF_SYNC_STRATUM_ID_PRESENT */
}rrmcm_mbsfn_config_info_t;
typedef struct _rrmcm_mbsfn_subframe_config_list_t 
{
    U8                               count; 
    rrmcm_mbsfn_config_info_t     mbsfn_subframe_config_info[RRM_MAX_MBSFN_ALLOCATIONS]; 
}rrmcm_mbsfn_subframe_config_list_t;
typedef enum
{
    RRMCM_MIF_NOTIFICATION_REP_COEFF_N2 = 2 ,
    RRMCM_MIF_NOTIFICATION_REP_COEFF_N4 = 4 
}rrmcm_mif_notification_rep_coeff_et;
typedef struct _rrm_sib_type_13_info_t
{
/* SPR 18334 : START */
    m2ap_notification_config_t          embms_sib13_notification_config;
/* SPR 18334 : END */
/* SPR 18334 : START */
    rrmcm_mif_notification_rep_coeff_et  notification_repetition_coeff;    /* mandatory */ /*rrmcm_mif_notification_rep_coeff_et*/
//    U8                                   notification_offset;              /* mandatory */
//    U8                                   notification_SF_index;            /* mandatory */
    U8                                   num_valid_mcch_info;
/* SPR 18334 : END */
    m2ap_mcch_related_bcch_info_t        embms_sib13_mcch_data[RRM_MAX_MBSFN_ALLOCATIONS]; 
}rrm_sib_type_13_info_t;
#endif
/*! \typedef enum rrm_load_type 
 *  \brief Possible load type received from Platform and reported by RCC
 */
typedef enum _rrm_load_type
{
    RRM_LOW,
    RRM_MEDIUM,
    RRM_HIGH,
    RRM_OVERLOAD,
    INVALID_LOAD
}rrm_load_type;

/*! \struct rrc_phy_pusch_configuration_dedicated_ue_service_profile_t
 *  \brief This Struct contains information about PUSCH channel quality index 
 *  \      and ri index.
 *  \param  beta_offset_ack_index
 *  \param  beta_offset_ri_index
 *  \param  beta_offset_cqi_index
 */
typedef struct _rrc_phy_pusch_configuration_dedicated_ue_service_profile_t
{
  U8  beta_offset_ack_index; /*^ M, 0, H, 0, 15 ^*/
  U8  beta_offset_ri_index; /*^ M, 0, H, 0, 15 ^*/
  U8  beta_offset_cqi_index; /*^ M, 0, H, 0, 15 ^*/
} rrc_phy_pusch_configuration_dedicated_ue_service_profile_t;


/*! \struct rrc_phy_uplink_power_control_dedicated_ue_service_profile_t 
 *  \brief This Struct contains information about the setting of the UE Transmit power 
 *  \      PUSCH for the physical uplink shared channel (PUSCH) transmission in
 *  \      subframe i is calculated by paramters defined in this structure
 *  \param p0_ue_pusch
 *  \param delta_mcs_enabled
 *  \param accumulation_enabled
 *  \param p0_ue_pucch
 *  \param p_srs_offset
 *  \param filter_coefficient
 */
typedef struct _rrc_phy_uplink_power_control_dedicated_ue_service_profile_t
{
  S8  p0_ue_pusch;           /*^ M, 0, B, -8, 7 ^*/
  U8  delta_mcs_enabled;     /*^ M, 0, H, 0, 1 ^*/ /* delta_mcs_enabled_ue_service_profile_et */
  U8  accumulation_enabled;  /*^ M, 0, H, 0, 1 ^*/   /* rrc_bool_et */
  S8  p0_ue_pucch;           /*^ M, 0, B, -8, 7 ^*/
  U8  p_srs_offset;          /*^ M, 0, H, 0, 15 ^*/
  U8  filter_coefficient;    /*^ M, 0, H, 0, 14 ^*/ /* rrc_filter_coefficient_ue_service_profile_et */
} rrc_phy_uplink_power_control_dedicated_ue_service_profile_t;


/*! \struct rrc_phy_cqi_reporting_periodic_param_ue_service_profile_t 
 *  \brief This Struct contains information about ack/nack of periodic CQI reporting 
 *  \param simultaneous_ack_nack_and_cqi
 */
typedef struct _rrc_phy_cqi_reporting_periodic_param_ue_service_profile_t
{
	U8  simultaneous_ack_nack_and_cqi;  /*^ M, 0, H, 0, 1 ^*/  /* rrc_bool_et */
} rrc_phy_cqi_reporting_periodic_param_ue_service_profile_t;

/*! \struct rrc_phy_cqi_reporting_periodic_ue_service_profile_t 
 *  \brief This struct contains information about the CQI periodic reporting is enabled or not
 *  \param bitmask
 *  \param cqi_reporting_periodic_param
 */
typedef struct _rrc_phy_cqi_reporting_periodic_ue_service_profile_t
{
  rrm_bitmask_t bitmask;    /*^ BITMASK ^*/
  #define RRC_SERVICE_PROFILE_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT    0x01

  rrc_phy_cqi_reporting_periodic_param_ue_service_profile_t cqi_reporting_periodic_param;

  /*^ O, RRC_SERVICE_PROFILE_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT ^*/

} rrc_phy_cqi_reporting_periodic_ue_service_profile_t;

/*! \struct rrc_phy_cqi_reporting_ue_service_profile_t 
 *  \brief This struct contains information about CQI reporting from UE as configured by eNB(periodic)  
 *  \param nom_pdsch_rs_epre_offset 
 *  \param rrm_ul_dl_freq_selective_sched_et 
 *  \param rrm_cqi_reporting_mode_et 
 */
typedef struct _rrc_phy_cqi_reporting_ue_service_profile_t
{
  S8                                                            nom_pdsch_rs_epre_offset; /*^ M, 0, B, -1, 6 ^*/
  rrc_phy_cqi_reporting_periodic_ue_service_profile_t       	cqi_reporting_periodic;
  rrm_ul_dl_freq_selective_sched_et     rrm_ul_dl_freq_selective_sched; /*^ M, 0, B, 0, 3 ^*/
  rrm_cqi_reporting_mode_et             rrm_cqi_reporting_mode_configured; /*^ M, 0, B, 0, 3 ^*/
} rrc_phy_cqi_reporting_ue_service_profile_t;

/*! \struct rrc_phy_codebook_subset_restriction_ue_service_profile_t 
 *  \brief This struct contains information about the codebook subset restriction for UE.
 *  \      In order for the eNB to restrict PMI and RI fed back from the UE the CSR bitmap is send to each UE. 
 * \param type
 * \param value
 */
typedef struct _rrc_phy_codebook_subset_restriction_ue_service_profile_t
{
    U16 type;  /*^ M, 0, H, 0, 7 ^*/ /* codebook_subset_restriction_type_ue_service_profile_et */

U8  value[8];   /*^ M, O, OCTET_STRING, FIXED ^*/
} rrc_phy_codebook_subset_restriction_ue_service_profile_t;


  /* Additional parameters for Cell Config Added gur29990 STARTS*/
/*! \struct rrc_phy_codebook_subset_restriction_v90_ue_service_profile_t 
 *  \brief This struct contains information about the codebook subset restriction for UE.
 *  \      In order for the eNB to restrict PMI and RI fed back from the UE the CSR bitmap is send to each UE. 
 *  \param type
 *  \param value
 */
typedef struct _rrc_phy_codebook_subset_restriction_v90_ue_service_profile_t
{
    U16 type;  /*^ M, 0, H, 0, 7 ^*/ /* codebook_subset_restriction_type_ue_service_profile_et */

    U8  value[4];   /*^ M, O, OCTET_STRING, FIXED ^*/
} rrc_phy_codebook_subset_restriction_v90_ue_service_profile_t;
  /* Additional parameters for Cell Config Added gur29990 ENDS*/

/*! \struct rrc_phy_ue_transmit_antenna_selection_ue_service_profile_t 
 *  \brief This struct contains information about the transmit antenna selection for UE   
 *  \param bitmask
 *  \param ue_transmit_antenna_selection_type
 */
typedef struct _rrc_phy_ue_transmit_antenna_selection_ue_service_profile_t
{
  rrm_bitmask_t   bitmask;    /*^ BITMASK ^*/
  #define RRC_SERVICE_PROFILE_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT  0x01

  U8  ue_transmit_antenna_selection_type;
  /*^ O, RRC_SERVICE_PROFILE_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT, H, 0, 1 ^*/
  /* ue_transmit_antenna_selection_type_ue_service_profile_et */

} rrc_phy_ue_transmit_antenna_selection_ue_service_profile_t;

/*! \struct rrc_phy_antenna_information_dedicated_ue_service_profile_t 
 *  \brief This struct contains information about the antenna information for UE
 *  \param transmission_mode
 *  \param codebook_subset_restriction
 *  \param codebook_subset_restriction_v90
 *  \param ue_transmit_antenna_selection
*/
typedef struct _rrc_phy_antenna_information_dedicated_ue_service_profile_t
{
  rrm_bitmask_t   bitmask;    /*^ BITMASK ^*/
  #define RRC_SERVICE_PROFILE_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT 0x01
  #define RRC_SERVICE_PROFILE_PHY_CODEBOOK_SUBSET_RESTRICTION_V90_PRESENT 0x02


  rrc_phy_codebook_subset_restriction_ue_service_profile_t codebook_subset_restriction;
  /*^ O, RRC_SERVICE_PROFILE_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT ^*/

  /* Additional parameters for Cell Config Added gur29990 STARTS*/
  rrc_phy_codebook_subset_restriction_v90_ue_service_profile_t codebook_subset_restriction_v90;
  /*^ O, RRC_SERVICE_PROFILE_PHY_CODEBOOK_SUBSET_RESTRICTION_V90_PRESENT^*/
  /* Additional parameters for Cell Config Added gur29990 ENDS*/

  rrc_phy_ue_transmit_antenna_selection_ue_service_profile_t ue_transmit_antenna_selection;
}rrc_phy_antenna_information_dedicated_ue_service_profile_t;

/*! \struct rrc_phy_antenna_information_ue_service_profile_t 
 *  \brief This struct contains information about the antenna information of UE 
 *  \      In case bitmask is not set for this struct UE shall transmit from UE port 0
 *  \param bitmask
 *  \param antenna_information_explicit_value
 */
typedef struct _rrc_phy_antenna_information_ue_service_profile_t
{
  rrm_bitmask_t   bitmask;    /*^ BITMASK ^*/
  #define RRC_SERVICE_PROFILE_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT  0x01

  rrc_phy_antenna_information_dedicated_ue_service_profile_t antenna_information_explicit_value;
  /*^ O, RRC_SERVICE_PROFILE_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT ^*/

} rrc_phy_antenna_information_ue_service_profile_t;

/*! \struct rrc_phy_scheduling_request_config_param_ue_service_profile_t 
 *  \brief This struct contains information about physical scheduling request.
 *  \param dsr_trans_max
 */
typedef struct _rrc_phy_scheduling_request_config_param_ue_service_profile_t
{ 
   U8  dsr_trans_max;            /*^ M, 0, H, 0, 4 ^*/ /* dsr_trans_max_ue_service_profile_et */
} rrc_phy_scheduling_request_config_param_ue_service_profile_t;


/*! \struct rrc_phy_physical_config_dedicate_ue_service_profiled_t 
 *  \brief This struct contains information to specify the UE specific physical channel configuration.
 * \param bitmask
 * \param pusch_configuration_dedicated
 * \param uplink_power_control_dedicated
 * \param cqi_reporting
 * \param antenna_information
 * \param scheduling_request_config
 */
typedef struct _rrc_phy_physical_config_dedicate_ue_service_profiled_t
{
  rrm_bitmask_t     bitmask;    /*^ BITMASK ^*/
  #define RRC_SERVICE_PROFILE_PHY_PUSCH_CONFIGURATION_DEDICATED_PRESENT   0x001
  #define RRC_SERVICE_PROFILE_PHY_UPLINK_POWER_CONTROL_DEDICATED_PRESENT  0x002
  #define RRC_SERVICE_PROFILE_PHY_CQI_REPORTING_PRESENT                   0x004
  #define RRC_SERVICE_PROFILE_PHY_ANTENNA_INFORMATION_PRESENT             0x008

  rrc_phy_pusch_configuration_dedicated_ue_service_profile_t pusch_configuration_dedicated;
  /*^ O, RRC_SERVICE_PROFILE_PHY_PUSCH_CONFIGURATION_DEDICATED_PRESENT ^*/

  rrc_phy_uplink_power_control_dedicated_ue_service_profile_t uplink_power_control_dedicated;
  /*^ O, RRC_SERVICE_PROFILE_PHY_UPLINK_POWER_CONTROL_DEDICATED_PRESENT ^*/

  rrc_phy_cqi_reporting_ue_service_profile_t cqi_reporting;
  /*^ O, RRC_SERVICE_PROFILE_PHY_CQI_REPORTING_PRESENT ^*/

  rrc_phy_antenna_information_ue_service_profile_t antenna_information;
  /*^ O, RRC_SERVICE_PROFILE_PHY_ANTENNA_INFORMATION_PRESENT ^*/

} rrc_phy_physical_config_dedicated_ue_service_profile_t;
/* SPR 18641_18843 fix start */
typedef struct _rrm_tti_bundling_info
{
    U8 ul_sinr_threshold_to_actv_tti;
    /*^ M, 0, N, 0, 255 ^*/ 
    U8 ul_sinr_threshold_to_deactv_tti;
    /*^ M, 0, N, 0, 255 ^*/ 
    U8 rrm_ue_used_rb_threshold;
    /*^ M, 0, B, 1, 100 ^*/ 
}rrm_tti_bundling_info_t;
/* SPR 18641_18843 fix end */

/*! \struct rrc_phr_config_param_ue_service_profile_t 
 *  \brief This struct contains information PHR config parameters. 
 *  \param periodic_phr_timer 
 *  \param prohibit_phr_timer
 *  \param dl_pathloss_change
 */
typedef struct _rrc_phr_config_param_ue_service_profile_t
{
  U8        periodic_phr_timer;
  /*^ M, 0, H, 1, 7 ^*/      /* rrc_periodic_phr_timer_ue_service_profile_et */

  U8        prohibit_phr_timer;
  /*^ M, 0, H, 1, 7 ^*/      /* rrc_prohibit_phr_timer_ue_service_profile_et */

  U8        dl_pathloss_change;
  /*^ M, 0, H, 1, 3 ^*/      /* rrc_dl_pathloss_change_ue_service_profile_et */

} rrc_phr_config_param_ue_service_profile_t;

/*! \struct rrc_ul_sch_config_ue_service_profile_t 
 *  \brief This struct contains information about SCH config parameters.
 *  \param bitmask
 *  \param max_harq_tx
 *  \param periodic_bsr_timer
 *  \param retx_bsr_timer
 *  \param tti_bundling
 */
typedef struct _rrc_ul_sch_config_ue_service_profile_t
{
  rrm_bitmask_t         bitmask;                /*^ BITMASK ^*/
  #define RRC_SERVICE_PROFILE_UL_SCH_CONFIG_MAX_HARQ_TX_PRESENT          0x01
  #define RRC_SERVICE_PROFILE_UL_SCH_CONFIG_PERIODIC_BSR_TIMER_PRESENT   0x02

  U8                    max_harq_tx;
  U8                    max_ul_harq_tx;
  /*^ O, RRC_SERVICE_PROFILE_UL_SCH_CONFIG_MAX_HARQ_TX_PRESENT, H, 1, 13 ^*/
  /* rrc_max_harq_tx_ue_service_profile_et */

 U8                    periodic_bsr_timer;
  /*^ O, RRC_SERVICE_PROFILE_UL_SCH_CONFIG_PERIODIC_BSR_TIMER_PRESENT, H, 1, 14 ^*/
  /* rrc_periodic_bsr_timer_ue_service_profile_et */

  U8                    retx_bsr_timer;
  /*^ M, 0, H, 0, 5 ^*/ /* rrc_retx_bsr_timer_ue_service_profile_et */

  U8                    tti_bundling;
  /*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */

}rrc_ul_sch_config_ue_service_profile_t;

/*! \struct rrc_phr_config_ue_service_profile_t 
 *  \brief This struct contains information about PHR config. 
 *  \param bitmask 
 *  \param phr_config_param
 */
typedef struct _rrc_phr_config_ue_service_profile_t
{
  rrm_bitmask_t bitmask;    /*^ BITMASK ^*/
  #define RRC_SERVICE_PROFILE_PHR_CONFIG_PARAM_PRESENT    0x01

  rrc_phr_config_param_ue_service_profile_t phr_config_param;
  /*^ O, RRC_PHR_CONFIG_PARAM_PRESENT ^*/

} rrc_phr_config_ue_service_profile_t;

/*! \struct _rrc_mac_main_config_ue_service_profile_t
 *  \brief This struct contains information to specify the MAC main configuration for signalling and data radio bearers. 
 *  \param bitmask
 *  \param ul_sch_config
 *  \param phr_config
 */
typedef struct _rrc_mac_main_config_ue_service_profile_t
{
  rrm_bitmask_t         bitmask;                /*^ BITMASK ^*/
  #define RRC_SERVICE_PROFILE_MAC_MAIN_CONFIG_UL_SCH_CONFIG_PRESENT        0x01
  #define RRC_SERVICE_PROFILE_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT           0x02
  rrc_ul_sch_config_ue_service_profile_t ul_sch_config;
  /*^ O, RRC_SERVICE_PROFILE_MAC_MAIN_CONFIG_UL_SCH_CONFIG_PRESENT ^*/
  
  rrc_phr_config_ue_service_profile_t phr_config;
  /*^ O, RRC_SERVICE_PROFILE_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT ^*/
 
} rrc_mac_main_config_ue_service_profile_t;

/*! \struct _rrm_mac_config_ue_service_profile_t
 *  \brief This struct contains information to specify MAC configuration for UE. 
 *  \param bitmask
 *  \param mac_main_config
 */
typedef struct _rrm_mac_config_ue_service_profile_t
{
  rrm_bitmask_t             bitmask;     /*^ BITMASK ^*/
  #define RRM_SERVICE_PROFILE_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT         0x01
  rrc_mac_main_config_ue_service_profile_t mac_main_config;
  /*^ O, RRM_SERVICE_PROFILE_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT ^*/
} rrm_mac_config_ue_service_profile_t;

/*! \struct rrm_ue_adm_radio_res_config_ue_service_profile_t_
 *  \brief This struct contains information of radio resource configurtion required for Ue admission.
 *  \param bitmask
 *  \param mac_config
 *  \param physical_config_dedicated
 */
typedef struct _rrm_ue_adm_radio_res_config_ue_service_profile_t
{
  rrm_bitmask_t             bitmask;     /*^ BITMASK ^*/
  #define RRM_SERVICE_PROFILE_UE_ADM_RADIO_RESP_MAC_CONFIG_PRESENT         0x01
  #define RRM_SERVICE_PROFILE_UE_ADM_RADIO_RESP_PHY_CONFIG_DED_PRESENT     0x02


  rrm_mac_config_ue_service_profile_t mac_config;
  /*^ O, RRM_SERVICE_PROFILE_UE_ADM_RADIO_RESP_MAC_CONFIG_PRESENT ^*/

  rrc_phy_physical_config_dedicated_ue_service_profile_t physical_config_dedicated;
  /*^ O, RRM_SERVICE_PROFILE_UE_ADM_RADIO_RESP_PHY_CONFIG_DED_PRESENT*/
}  rrm_ue_adm_radio_res_config_ue_service_profile_t;

/*! \struct rrc_rrm_ue_service_profile_t
 *  \brief This struct contains information about service profile of UE. 
 *  \param extended_wait_time
 *  \param wait_time
 *  \param radio_res_config
 */
typedef struct _rrc_rrm_ue_service_profile_t
{
  /*Rel10 CR 588 and 679 changes start*/
  rrm_bitmask_t               bitmask;      /*^ BITMASK ^*/
#define RRM_SERVICE_PROFILE_UE_ADM_EXTENDED_WAIT_TIME_PRESENT       0x01  
  /* SPR 21429 Fix Start */
#define RRM_SERVICE_PROFILE_UE_ADM_WAIT_TIME_PRESENT       0x02  
#define RRM_SERVICE_PROFILE_UE_ADM_INITIAL_UE_LOC_PRESENT       0x04  
  /* SPR 21429 Fix End */
  U16                         extended_wait_time;   
  /*^ O, RRM_SERVICE_PROFILE_UE_ADM_EXTENDED_WAIT_TIME_PRESENT, B, 1, 1800 ^*/
  /*Rel10 CR 588 and 679 changes end*/
  U8                          wait_time;    /*^ M, 0, B, 1, 16 ^*/
  U8                          initial_ue_location; 
  rrm_ue_adm_radio_res_config_ue_service_profile_t radio_res_config;
} rrc_rrm_ue_service_profile_t;

/*! \struct rrm_s1u_qos_profile_erb_service_profile_t
 *  \brief This struct contains information about QOS profile of rab 
 *  \param allocation_retention_priority 
 */
typedef struct _rrm_s1u_qos_profile_erb_service_profile_t
{
    U8 allocation_retention_priority;
} rrm_s1u_qos_profile_erb_service_profile_t;

/*! \struct rrm_s1u_config_erb_service_profile_t
 *  \brief This struct contains information to specify s1u config rab service 
 *  \param qos_id
 *  \param sap_flags
 *  \param qos_profile
 *  \param seq_flag
 *  \param reordering_reqd
 */
typedef struct _rrm_s1u_config_erb_service_profile_t
{
    U32 qos_id;
    U8  sap_flags;
    rrm_s1u_qos_profile_erb_service_profile_t qos_profile;
    U8  seq_flag;   /*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */
    U8  reordering_reqd;    /*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */
} rrm_s1u_config_erb_service_profile_t;

/*! \struct rrc_ul_specific_parameters_erb_service_profile_t
 *  \brief This struct contains information to specify uplink specific parameters of rab  
 *  \param bitmask
 *  \param priority
 *  \param prioritized_bit_rate
 *  \param bucket_size_duration
 *  \param logical_channel_group
 */
typedef struct _rrc_ul_specific_parameters_erb_service_profile_t
{
  rrm_bitmask_t           bitmask;       /*^ BITMASK ^*/
#define RRC_UL_SPECIFIC_PARAMETERS_LOGICAL_CH_GROUP_ERB_SERVICE_PROFILE_PRESENT        0x01

  U8            priority;               /*^ M, 0, H, 1, 16 ^*/
  U8            prioritized_bit_rate;
/*^ M, 0, H, 1, 7 ^*/  /* rrc_prioritized_bit_rate_erb_service_profile_et */

  U8            bucket_size_duration;
/*^ M, 0, H, 1, 5 ^*/  /* rrc_bucket_size_duration_erb_service_profile_et */

  U8            logical_channel_group;
/*^ O, RRC_UL_SPECIFIC_PARAMETERS_LOGICAL_CH_GROUP_ERB_SERVICE_PROFILE_PRESENT, H, 1, 3 ^*/

} rrc_ul_specific_parameters_erb_service_profile_t;


/*! \struct rrc_downlink_um_rlc_erb_service_profile_t
 *  \brief this struct contains information about downlink UM rlc mode rab.
 *  \param sn_field_length
 *  \param t_reordering
 */
typedef struct _rrc_downlink_um_rlc_erb_service_profile_t
{
    U8        sn_field_length;
/*^ M, 0, H, 0, 1 ^*/  /* rrc_sn_field_length_et */

    U8        t_reordering;    /*^ M, 0, H, 1, 30 ^*/ /* rrc_t_reordering_erb_service_profile_et */
} rrc_downlink_um_rlc_erb_service_profile_t;

/*! \struct _rrc_um_uni_directional_dl_config_erb_service_profile_t 
 *  \brief This struct contains information about UM uni directional downlink rab service 
 *  \param dl_um_rlc
 */
typedef struct _rrc_um_uni_directional_dl_config_erb_service_profile_t
{
    rrc_downlink_um_rlc_erb_service_profile_t      dl_um_rlc;
} rrc_um_uni_directional_dl_config_erb_service_profile_t;

/*! \struct rrc_uplink_um_rlc_erb_service_profile_t 
 *  \brief This struct contains information about uplink UM rlc mode rab 
 *  \param sn_field_length
 */
typedef struct _rrc_uplink_um_rlc_erb_service_profile_t
{
    U8        sn_field_length;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_sn_field_length_erb_service_profile_et */

} rrc_uplink_um_rlc_erb_service_profile_t;

/*! \struct rrc_um_uni_directional_ul_config_erb_service_profile_t 
 *  \brief this struct contains information about UM uni directional uplink rab service 
 *  \param  ul_um_rlc 
*/
typedef struct rrc_um_uni_directional_ul_config_erb_service_profile_t
{
    rrc_uplink_um_rlc_erb_service_profile_t        ul_um_rlc;
} rrc_um_uni_directional_ul_config_erb_service_profile_t;

/*! \struct rrc_um_bi_directional_config_erb_service_profile_t 
 *  \brief This struct contains information about UM bi directional rab service for uplink and downlink 
 *  \param ul_um_rlc
 *  \param dl_um_rlc
 */
typedef struct _rrc_um_bi_directional_config_erb_service_profile_t
{
    rrc_uplink_um_rlc_erb_service_profile_t        ul_um_rlc;
    rrc_downlink_um_rlc_erb_service_profile_t      dl_um_rlc;
} rrc_um_bi_directional_config_erb_service_profile_t;

/*! \struct rrc_downlink_am_rlc_erb_service_profile_t 
 *  \brief This struct contains information about downlink AM rlc rab service.
 *  \param t_reordering 
 *  \param t_status_prohibit
 */
typedef struct _rrc_downlink_am_rlc_erb_service_profile_t
{
  U8        t_reordering;
/*^ M, 0, H, 1, 30 ^*/  /* rrc_t_reordering_erb_service_profile_et */

  U8        t_status_prohibit;
/*^ M, 0, H, 1, 55 ^*/  /* rrc_t_status_prohibit_erb_service_profile_et */

} rrc_downlink_am_rlc_erb_service_profile_t;

/*! \struct rrc_uplink_am_rlc_erb_service_profile_t 
 *  \brief This struct contains information about uplink AM rlc rab service 
 *  \param t_poll_retransmit 
 *  \param t_poll_retransmit_emtc 
 *  \param poll_pdu
 *  \param poll_byte
 *  \param max_retx_threshold
 */
typedef struct _rrc_uplink_am_rlc_erb_service_profile_t
{

  U8        t_poll_retransmit;
/*^ M, 0, H, 1, 54 ^*/      /* rrc_t_poll_retransmit_erb_service_profile_et */

  U8        poll_pdu;
/*^ M, 0, H, 1, 7 ^*/       /* rrc_poll_pdu_erb_service_profile_et */

  U8        poll_byte;
/*^ M, 0, H, 1, 14 ^*/      /* rrc_poll_byte_erb_service_profile_et */

  U8        max_retx_threshold;
/*^ M, 0, H, 1, 7 ^*/       /* rrc_max_retx_threshold_erb_service_profile_et */


} rrc_uplink_am_rlc_erb_service_profile_t;

/*! \struct rrc_am_config_erb_service_profile_t 
 *  \brief This struct contians information about AM configuration of rab.
 *  \param ul_am_rlc 
 *  \param dl_am_rlc
 */
typedef struct _rrc_am_config_erb_service_profile_t
{
    rrc_uplink_am_rlc_erb_service_profile_t        ul_am_rlc;
    rrc_downlink_am_rlc_erb_service_profile_t      dl_am_rlc;
} rrc_am_config_erb_service_profile_t;

/*! \struct rrc_rlc_config_erb_service_profile_t 
 *  \brief This struct contains information to specify the RLC configuration of SRBs and DRBs. 
 *  \param bitmask
 *  \param am_config
 *  \param um_bi_directional_config
 *  \param um_uni_directional_ul_config
 *  \param um_uni_directional_dl_config
 *  \param buffer_factor
 */
typedef struct _rrc_rlc_config_erb_service_profile_t
{
  rrm_bitmask_t                 bitmask;   /*^ BITMASK ^*/
#define RRC_RLC_CONFIG_AM_CONFIG_ERB_SERVICE_PROFILE_PRESENT                        0x01
#define RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_ERB_SERVICE_PROFILE_PRESENT         0x02
#define RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_ERB_SERVICE_PROFILE_PRESENT     0x04
#define RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_ERB_SERVICE_PROFILE_PRESENT     0x08

  rrc_am_config_erb_service_profile_t                       am_config;
/*^ O, RRC_RLC_CONFIG_AM_CONFIG_ERB_SERVICE_PROFILE_PRESENT ^*/

  rrc_um_bi_directional_config_erb_service_profile_t        um_bi_directional_config;
/*^ O, RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_ERB_SERVICE_PROFILE_PRESENT ^*/

  rrc_um_uni_directional_ul_config_erb_service_profile_t    um_uni_directional_ul_config;
/*^ O, RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_ERB_SERVICE_PROFILE_PRESENT ^*/

  rrc_um_uni_directional_dl_config_erb_service_profile_t    um_uni_directional_dl_config;
/*^ O, RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_ERB_SERVICE_PROFILE_PRESENT ^*/

  float buffer_factor;
} rrc_rlc_config_erb_service_profile_t;

/*! \struct rrc_pdcp_rohc_profile_erb_service_profile_t 
 *  \brief This struct contains information about ROHC profile of rab.
 *  \param profile0x0001
 *  \param profile0x0002
 *  \param profile0x0003
 *  \param profile0x0004
 *  \param profile0x0006
 *  \param profile0x0101
 *  \param profile0x0102
 *  \param profile0x0103
 *  \param profile0x0104
 */
typedef struct _rrc_pdcp_rohc_profile_erb_service_profile_t
{
  U8            profile0x0001;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0002;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0003;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0004;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0006;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0101;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0102;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0103;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0104;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
} rrc_pdcp_rohc_profile_erb_service_profile_t;

/*! \struct rrc_pdcp_rohc_config_erb_service_profile_t 
 *  \brief this struct contains information about PDCP ROHC profile of rab.
 *  \param max_cid
 *  \param rohc_profile
 */
typedef struct _rrc_pdcp_rohc_config_erb_service_profile_t
{
  U16                        max_cid;        /*^ M, 0, B, 1, 16383 ^*/
  rrc_pdcp_rohc_profile_erb_service_profile_t    rohc_profile;   /*^ M, 0, N, 0, 0 ^*/
} rrc_pdcp_rohc_config_erb_service_profile_t;

/*PDCP-Config, see headerCompression IE (notUsed)*/
/*! \struct rrc_pdcp_header_compression_config_erb_service_profile_t 
 *  \brief This struct contains inforamtion about PDCP header compression.  
 *  \param bitmask
 *  \param rohc_config
 */
typedef struct _rrc_pdcp_header_compression_config_erb_service_profile_t
{
    rrm_bitmask_t         bitmask;                 /*^ BITMASK ^*/
#define RRC_PDCP_HEADER_COMPRESSION_ROHC_CONFIG_ERB_SERVICE_PROFILE_PRESENT  0x01

    rrc_pdcp_rohc_config_erb_service_profile_t  rohc_config;
/*^ O, RRC_PDCP_HEADER_COMPRESSION_ROHC_CONFIG_ERB_SERVICE_PROFILE_PRESENT ^*/

} rrc_pdcp_header_compression_config_erb_service_profile_t;

/*! \struct rrc_pdcp_config_erb_service_profile_t 
 *  \brief This struct contains parameters required for PDCP.  
 *  \param bitmask
 *  \param discard_timer
 *  \param rlc_am_status_report_required
 *  \param rlc_um_pdcp_sn_size
 *  \param rlc_am_enb_status_report_required
 *  \param header_compression
 */
typedef struct _rrc_pdcp_config_erb_service_profile_t
{
    rrm_bitmask_t         bitmask;                 /*^ BITMASK ^*/
#define RRC_PDCP_CONFIG_DISCARD_TIMER_ERB_SERVICE_PROFILE_PRESENT                0x01
#define RRC_PDCP_CONFIG_RLC_AM_STATUS_REPORT_REQ_ERB_SERVICE_PROFILE_PRESENT     0x02
#define RRC_PDCP_CONFIG_RLC_UM_PDCP_SN_SIZE_ERB_SERVICE_PROFILE_PRESENT          0x04
#define RRC_PDCP_CONFIG_RLC_AM_ENB_STATUS_REPORT_REQ_ERB_SERVICE_PROFILE_PRESENT 0x08

  U8                      discard_timer;
/*^ O, RRC_PDCP_CONFIG_DISCARD_TIMER_ERB_SERVICE_PROFILE_PRESENT, H, 1, 7 ^*/
/* rrc_pdcp_discard_timer_erb_service_profile_et */

  U8                      rlc_am_status_report_required;
/*^ O, RRC_PDCP_CONFIG_RLC_AM_STATUS_REPORT_REQ_ERB_SERVICE_PROFILE_PRESENT, H, 0, 1 ^*/
/* rrc_bool_et */

  U8                      rlc_um_pdcp_sn_size;
/*^ O, RRC_PDCP_CONFIG_RLC_UM_PDCP_SN_SIZE_ERB_SERVICE_PROFILE_PRESENT, H, 1, 1 ^*/
/* rrc_pdcp_sn_size_erb_service_profile_et */
  
  U8                      rlc_am_enb_status_report_required;
  /*^ O, RRC_PDCP_CONFIG_RLC_AM_ENB_STATUS_REPORT_REQ_ERB_SERVICE_PROFILE_PRESENT, H, 0, 1 ^*/
  /* rrc_bool_et */
  rrc_pdcp_header_compression_config_erb_service_profile_t   header_compression;
} rrc_pdcp_config_erb_service_profile_t;

/* TS 36.331 - 6.3.2 RadioResourceConfigDedicated IE DRB-ToAddMod IE. */

/*SPR 5169 start*/
/*36.331 - RACH-ConfigDedicated*/
/*! \struct rrm_rrc_mac_config_service_profile_t
 *  \brief This struct contains parameters required for MAC.
 */
typedef struct _rrm_rrc_mac_config_service_profile_t
{
    U8    sr_prohibit_timer_r9;    /*^ M, 0, H, 0, 7 ^*/
    rrm_oam_sr_periodicity_et           sr_periodicity_start_val; /*^ M, 0, H, 0, 6 ^*/
    rrm_oam_sr_periodicity_et           sr_periodicity_end_val; /*^ M, 0, H, 0, 6 ^*/
    U8    sr_max_transmissions;    /*^ M, 0, H, 4, 8, 16, 32, 64 ^*/
} rrm_rrc_mac_config_service_profile_t;
/*SPR 5169 end*/

/*! \struct rrm_erb_service_profile_t 
 *  \brief This struct is used to setup/modify/release RBs, to modify the MAC main configuration, to
 *  \     modify the SPS configuration and to modify dedicated physical configuration.
 *  \param data_transfer_mode 
 *  \param enable_rohc
 *  \param pdcp_config
 *  \param rlc_config
 *  \param ul_specific_parameters
 *  \param rrc_ul_sch_config_profile
 *  \param rrm_s1u_config_erb_service_profile
 */
typedef struct _rrm_erb_service_profile_t
{
  U8 data_transfer_mode;  /*1-AM, 2-UM*/
  U8 enable_rohc;
  rrc_pdcp_config_erb_service_profile_t     pdcp_config;
  rrc_rlc_config_erb_service_profile_t      rlc_config;
  rrc_ul_specific_parameters_erb_service_profile_t  	ul_specific_parameters;
  rrc_ul_sch_config_ue_service_profile_t            	rrc_ul_sch_config_profile;
  /* For Contela Fixes */
  rrm_s1u_config_erb_service_profile_t      rrm_s1u_config_erb_service_profile;
/*SPR 5169 start*/
  rrm_rrc_mac_config_service_profile_t      mac_config;
/*SPR 5169 end*/
  rrc_phr_config_ue_service_profile_t phr_config;
} rrm_erb_service_profile_t;


#define RRMCM_MCC_PRESENCE_BITMASK 0x0001
/*! \struct rrm_cell_plmn_info_t 
 *  \brief This struct contains information about the PLMN id of eNB.
 *  \param bitmask
 *  \param mcc[MAX_MCC_DIGITS]
 *  \param num_mnc_digit
 *  \param mnc[MAX_MNC_DIGITS]
 */
typedef struct _rrm_cell_plmn_info_t
{
	rrm_bitmask_t bitmask;
	U8 mcc[MAX_MCC_DIGITS];
	U8 num_mnc_digit; /*This can 2 or 3*/
	U8 mnc[MAX_MNC_DIGITS];
}rrm_cell_plmn_info_t;

/*LA code changes start*/
typedef struct _rrm_ue_speed_category_data_t
{
    U32 ue_speed_category1_deltaSINR_thresold;
}rrm_ue_speed_category_data_t;

/*LA code changes end*/
/*! \struct antenna_info_t 
 *  \brief This struct contains information about the antenna information needed for UE.
 *  \param antenna_ports_count_number
 */
typedef struct _antenna_info_t 
{
    antenna_ports_count_et 	 antenna_ports_count_number;
    U8                           num_cbsr; 
    rrc_phy_codebook_subset_restriction_ue_service_profile_t 
                                 cbsr[RRM_OAM_MAX_CODEBOOK_SUBSET];
    /*TM mode 7-8 start*/
    U32                          sinr_threshold;
    U32                          pathloss_threshold;
    rrm_ue_speed_category_data_t ue_speed_catagory_data;
    /*TM mode 7-8 end*/
}antenna_info_t;

/*! \struct pdsch_t 
 *  \brief This struct contains information about the paramaeters p_a ans p_b needed for transmission power calculation. 
 *  \param p_b 
 *  \param p_a
 */
typedef struct _pdsch_t
{
	rrm_oam_pdsch_config_common_pb_et      p_b;
	rrm_oam_pdsch_config_dedicated_p_a_et  p_a;	
    /* DYNAMIC ICIC CHANGES START*/
    U16                                     p_a_for_cell_edge_ue;
    /* DYNAMIC ICIC CHANGES END */
}pdsch_t;


#define RRMCM_RMIF_SRS_MAX_UP_PTS_PRESENT 0x01
    /* SPR 21429 Fix Start */
#define RRMCM_SRS_DURATION_PRESENT  0x01
#define RRMCM_SRS_HOPPING_ENABLE_PRESENT  0x02
    /* SPR 21429 Fix End */
/*! \struct srs_t 
 *  \brief This struct contains information about the Sounding-RS SRS which can be used
 *  \      to estimate UL channel quality and provide input to UL scheduler, as well as help timing advance. 
 *  \param bitmask 
 *  \param srsEnabled
 *  \param srs_bandwidth_config
 *  \param srs_subframe_config
 *  \param srs_max_up_pts
 *  \param ack_nack_srs_simultaneous_transmission
 */
typedef struct _srs_t
{
	rrm_bitmask_t                   bitmask;
	rrm_bool_et                     srsEnabled;
	rrm_oam_srs_bw_config_et        srs_bandwidth_config;
	rrm_oam_srs_subframe_config_et	srs_subframe_config;
	rrm_oam_srs_max_up_pts_et 	    srs_max_up_pts;
	rrm_bool_et 			        ack_nack_srs_simultaneous_transmission;
    /*SPR 5887 fix*/
    U8                              srs_duration;
    /*SPR 5887 fix*/
/* SPR_8391_CHANGES_START*/
    U8                              srs_hopping_enable;
/* SPR_8391_CHANGES_END*/
}srs_t;

/*! \struct prach_t 
 *  \brief This struct contains information to specify the PRACH configuration in the system
 *  \      information and in the mobility control information, respectively.
 *  \param root_sequence_index
 *  \param configuration_index
 *  \param high_speed_flag
 *  \param zero_correlation_zone_config
 *  \param frequency_offset
 */
typedef struct _prach_t
{
	U16	root_sequence_index;
	U8	configuration_index;
	U8	high_speed_flag;
	U8	zero_correlation_zone_config;
	U8	frequency_offset; 
}prach_t;

/*! \struct pucch_t 
 *  \brief This struct contains information to specify the common and the UE specific
 *  \      PUCCH configuration respectively.
 *  \param bitmask
 *  \param delta_pucch_shift
 *  \param n_rb_cqi
 *  \param n_cs_an
 *  \param n1_pucch_an
 *  \param cqi_pucch_resource_index
 *  \param tdd_ack_nack_feedback_mode
 */
typedef struct _pucch_t
{
    rrm_bitmask_t bitmask;
#define PUCCH_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT 0x01
    U8	delta_pucch_shift;
	U8	n_rb_cqi;
	U8	n_cs_an;
	U16	n1_pucch_an; 
	U16	cqi_pucch_resource_index;
    U8  tdd_ack_nack_feedback_mode;
  /* BUG_371 FIX START */
  U8                               pucch_cqi_sinr_value;
  /* BUG_371 FIX END */
    /* SPR_8597_FIX_START*/
  U16                             sr_n1_pucch_an_last_indx;        /* last n1_pucch resource index for SR */ 
   /* SPR_8597_FIX_END*/
    
}pucch_t;

/*! \struct pusch_t 
 *  \brief This struct contains information to specify the common PUSCH configuration.
 *  \param n_sb
 *  \param pusch_hopping_mode
 *  \param hopping_offset
 *  \param enable_64_qam
 */
typedef struct _pusch_t
{
	U8			n_sb;
	rrm_oam_hopping_mode_et pusch_hopping_mode;
	U8			hopping_offset; 
    /* coverity : CID 29719,16499*/
    U8          enable_64_qam;
}pusch_t;

/*! \struct ul_reference_signal
 *  \brief This struct contains information to specify the reference signal
 *  \      configuration for PUSCH and PUCCH.
 *  \param group_hopping_enabled
 *  \param group_assignment_pusch
 *  \param sequence_hopping_enabled
 *  \param cyclic_shift
 */
typedef struct _ul_reference_signal_t
{
	rrm_bool_et		group_hopping_enabled;
	U8			group_assignment_pusch;
	rrm_bool_et		sequence_hopping_enabled;
	U8			cyclic_shift;
}ul_reference_signal_t;

/* CA_Stage_3_Scell_TM_change: Start */
#define RRMCM_RMIF_PUCCH_FORMAT_R10_PRESENT 0x0001
/* CA_Stage_3_Scell_TM_change: End */
/*! \struct uplink_power_control_t
 *  \brief This struct contains information to specify parameters for uplink
 *  \      power control in the system information and in the dedicated signalling, respectively.
 *  \param p_0_nominal_pusch
 *  \param alpha
 *  \param p_0_nominal_pucch
 */
typedef struct _uplink_power_control_t
{
	S8					  p_0_nominal_pusch; 
	rrm_oam_alpha_et			  alpha; 
	S8					  p_0_nominal_pucch; 
       /* BUG_371 FIX START */ 
       /* BUG_371 FIX END */  
}uplink_power_control_t;

/*! \struct prs_t 
 *  \brief This struct contains inforamtion about parameters related to Positioning Reference Signals (PRS) configuration. 
 *  \param num_prs_resource_blocks 
 *  \param prs_configuration_index
 *  \param num_consecutive_prs_subfames
 */
typedef struct _prs_t
{
    rrm_bitmask_t       bitmask;
    rrm_bool_et prs_enable;
	U8	num_prs_resource_blocks;
	U16	prs_configuration_index;
	rrm_oam_num_consecutive_prs_sf_et	num_consecutive_prs_subfames;
	rrm_oam_prs_muting_configration_t       prs_muting_configration;
    U16         prs_transmission_power;
}prs_t;

 /*! \struct tdd_frame_structure_t
  *  \brief This struct contains information about frame assignment and frame patterns
  *  \param sub_frame_assignment
  *  \param special_sub_frame_patterns
 */
typedef struct _tdd_frame_structure_t
{
    rrm_oam_sub_frame_asgmnt_et sub_frame_assignment;/*^ M, 0, N, 0, 6 ^*/
    rrm_oam_special_sub_frame_patterns_et   special_sub_frame_patterns;/*^ M, 0, N, 0, 8 ^*/
}tdd_frame_structure_t;

/* LTE_RRM_CELL_RECONFIG_START*/
#define RRMCM_RMIF_PHYSICAL_LAYER_PARAM_PDSCH_CONFIG_PRESENT  0x01
#define RRMCM_RMIF_PHYSICAL_LAYER_PARAM_SRS_CONFIG_PRESENT    0x02
#define RRMCM_RMIF_PHYSICAL_LAYER_PARAM_PRACH_CONFIG_PRESENT  0x04
#define RRMCM_RMIF_PHYSICAL_LAYER_PARAM_PUCCH_CONFIG_PRESENT  0x08
#define RRMCM_RMIF_PHYSICAL_LAYER_PARAM_PUSCH_CONFIG_PRESENT  0x10
#define RRMCM_RMIF_PHYSICAL_LAYER_PARAM_UL_REF_SIG_PRESENT    0x20
#define RRMCM_RMIF_PHYSICAL_LAYER_PARAM_UL_POWER_CTRL_PRESENT 0x40
#define RRMCM_RMIF_PHYSICAL_LAYER_PARAM_PRS_CONFIG_PRESENT    0x80
#define RRMCM_RMIF_PHYSICAL_LAYER_PARAM_TDD_PRESENT           0x100
/* SPR 22248 MBMS Changes Start */
#define RRMCM_RMIF_MBSFN_CONFIGURATION_INFO_PRESENT           0x200
/* SPR 22248 MBMS Changes Stop */
/* eMTC changes start */
#define RRMCM_RMIF_PHYSICAL_LAYER_PARAM_BR_PRESENT                0x400
/* eMTC changes stop */

/* LTE_RRM_CELL_RECONFIG_END*/

/*! \struct rrm_physical_layer_params_t 
 *  \brief This struct contains information about the set of proposed Physical Layer objects and parameters. 
 *  \param bitmask
 *  \param physical_layer_param_pdsch
 *  \param physical_layer_param_srs
 *  \param physical_layer_param_prach
 *  \param physical_layer_param_pucch
 *  \param physical_layer_param_pusch
 *  \param physical_layer_param_ul_reference_signal
 *  \param physical_layer_param_ul_power_control
 *  \param physical_layer_param_prs
 *  \param physical_layer_param_tdd_frame_structure
 */
typedef struct _rrm_physical_layer_params_t 
{
	rrm_bitmask_t 		bitmask;
        pdsch_t   physical_layer_param_pdsch;
        srs_t     physical_layer_param_srs;
        prach_t   physical_layer_param_prach;
        pucch_t   physical_layer_param_pucch;
        pusch_t   physical_layer_param_pusch;
        ul_reference_signal_t       physical_layer_param_ul_reference_signal;
        uplink_power_control_t      physical_layer_param_ul_power_control;
        prs_t                       physical_layer_param_prs;
        tdd_frame_structure_t       physical_layer_param_tdd_frame_structure; /*^ O, RRMCM_RMIF_PHYSICAL_LAYER_PARAM_TDD_PRESENT, N, 0, 0^*/
#ifdef LTE_EMBMS_SUPPORTED
        rrmcm_mbsfn_subframe_config_list_t physical_layer_param_mbsfn_config_info; /* RRMCM_RMIF_MBSFN_CONFIGURATION_INFO_PRESENT */
#endif
}rrm_physical_layer_params_t;

#define RRMCM_RMIF_RA_PREABLE_GROUPA_INFO_PRESENT     0x01
/*! \struct rrm_preamble_info_t 
 *  \brief This struct contains information about the number of non-dedicated random access
 *  \      preambles and number of non-dedicated random access preambles in Random Acces Preambles
 *  \      group A.
 *  \param bitmask
 *  \param number_of_ra_preambles
 *  \param ra_preamble_groupA_info
 */
typedef struct _rrm_preamble_info_t 
{
    rrm_bitmask_t    bitmask;
	rrm_oam_ra_preamble_count_et          number_of_ra_preambles;
    rrm_oam_preamble_groupA_info_t      ra_preamble_groupA_info;
}rrm_preamble_info_t;

/*! \struct rach_t 
 *  \brief This struct contains information about parameters related to RACH configuration at the MAC level across the cell.  
 *  \param preamble_info 
 *  \param power_ramping_step
 *  \param preamble_initial_received_target_power
 *  \param preamble_trans_max
 *  \param response_window_size
 *  \param contention_resolution_timer
 *  \param max_harq_msg_3tx
 */
typedef struct _rach_t
{
	rrm_preamble_info_t                     preamble_info;
	rrm_oam_power_ramping_step_et 		power_ramping_step;
	rrm_oam_preamble_init_rec_target_pow_et	preamble_initial_received_target_power;
	rrm_oam_preamble_trans_max_et 		preamble_trans_max;
	rrm_oam_ra_resp_win_size_et 		response_window_size;
	rrm_oam_mac_cont_resol_timer_et 	contention_resolution_timer;
	U8					max_harq_msg_3tx;
}rach_t;

/* CSR_00053551_CHANGES_START */
/*! \struct rrm_drx_mac_ce_cmd_trigger_info_t
 *  \brief This struct contains information about drx mac ce tigger command
 *  \param drx_mac_ce_timer_percentage
 *  \param drx_mac_ce_cmd_threshold
 */
typedef struct _rrm_drx_mac_ce_cmd_trigger_info_t
{
  rrm_bool_et                 drx_mac_ce_command_enable;  /*^ M, 0, N, 0, 0 ^*/
  U16        drx_mac_ce_timer_percentage;/*^ O, 0, B, 3, 2559 ^*/

  U16        drx_mac_ce_cmd_threshold;  /*^ O, 0, B, 1, 2559 ^*/
}rrm_drx_mac_ce_cmd_trigger_info_t;
/* CSR_00053551_CHANGES_END */

# define RRMCM_RMIF_SHORT_DRX_CYCLE_PRESENT           0x01
# define RRMCM_RMIF_SHORT_DRX_CYCLE_TIMER_PRESENT     0x02
/*! \struct drx_config_t 
 *  \brief This struct contains information about parameters related to DRX configuration.  
 *  \param bitmask
 *  \param num_qci
 *  \param qci[RRM_MAX_QCI]
 *  \param on_duration_timer
 *  \param drx_inactivity_timer
 *  \param drx_retransmission_timer
 *  \param long_drx_cycle 
 *  \param drx_start_offset
 *  \param short_drx_cycle
 *  \param drx_short_cycle_timer
 */
typedef struct _drx_config_t 
{
	rrm_bitmask_t                           bitmask;
	U8					num_qci;
	U8					qci[RRM_MAX_QCI];		
	rrm_oam_on_duration_timer_et 		on_duration_timer;
	rrm_oam_drx_inactivity_timer_et         drx_inactivity_timer;
	rrm_oam_drx_retransmission_timer_et	drx_retransmission_timer;
	rrm_oam_long_drx_cycle_start_offset_et	long_drx_cycle;	
	U16 				        drx_start_offset;			
	rrm_oam_short_drx_cycle_et              short_drx_cycle;
	U8				        drx_short_cycle_timer;

    #ifdef ENDC_ENABLED
	rrm_oam_long_drx_cycle_start_offset_r15_et	long_drx_cycle_r15;	
    #endif
}drx_config_t;

/* Fix for CSR: 00059124 */
/*SPR 20533 Fix Start*/
/*Code Removed*/
/*SPR 20533 Fix Stop*/
/*! \struct cdrx_config_t
 *  \brief This struct contains information about parameters related to C-DRX configuration.
 *  \param bitmask
 *  \param on_duration_timer
 *  \param drx_inactivity_timer
 *  \param drx_retransmission_timer
 *  \param long_drx_cycle
 *  \param drx_start_offset
 *  \param short_drx_cycle
 *  \param drx_short_cycle_timer
 */
 #ifdef ENDC_ENABLED
#define RRMCM_RMIF_CDRX_LONG_DRX_CYCLE_R15_PRESENT           0x01
 #endif
typedef struct _cdrx_config_t
{
    rrm_bitmask_t                          bitmask;
    rrm_oam_on_duration_timer_et           on_duration_timer;
    rrm_oam_drx_inactivity_timer_et        drx_inactivity_timer;
    rrm_oam_drx_retransmission_timer_et    drx_retransmission_timer;
    rrm_oam_long_drx_cycle_start_offset_et long_drx_cycle;
    U16                                    drx_start_offset;
    /*SPR 20533 Fix Start*/
    /*Code Removed*/
    /*SPR 20533 Fix Stop*/
    #ifdef ENDC_ENABLED
    rrm_oam_long_drx_cycle_start_offset_r15_et long_drx_cycle_r15;
    #endif
}cdrx_config_t;
/* Fix for CSR: 00059124 */

/*! \struct drx_t 
 *  \brief This struct contains information about DRX i.e DRX is enabled or not, valid DRX cycle
 *  \      and the parameters related to DRX configuration  
 */
typedef struct _drx_t 
{
    rrm_bool_et         drx_enabled;
    U8 					num_valid_drx_profiles;
	drx_config_t		drx_config[MAX_NO_DRX_PROFILE];
    /* CSR_00053551_CHANGES_START */
    rrm_drx_mac_ce_cmd_trigger_info_t   drx_mac_ce_cmd_trigger_info;
    /* CSR_00053551_CHANGES_END */
}drx_t;

/*! \struct srb_info_t 
 *  \brief This struct contains information about the parameters related to SRB 
 */
typedef struct _srb_info_t 
{
	rrm_oam_t_poll_retransmit_et 	t_poll_retransmit;
	rrm_oam_poll_pdu_et 		poll_pdu;
	rrm_oam_poll_byte_et		poll_byte;
	rrm_oam_max_retx_threshold_et	max_retx_threshold;
	rrm_oam_t_reordering_et		t_reordering;
	rrm_oam_t_status_prohibit_et	t_status_prohibit;
	U8			 	priority;/*Range 1---16*/
	rrc_prioritized_bit_rate_erb_service_profile_et		piroritized_bit_rate;
	rrc_bucket_size_duration_erb_service_profile_et		bucket_size_duration;
	U8				logical_channel_group;
	/* Removed By  Nupur
	 * U8				ul_lcg_id; //Range 0---3
	 * U8				dl_lch_priority;//Range 1---16
	*/
}srb_info_t;

#define RRMCM_RMIF_SRB_INFO_PRESENT 0x01
/*! \ struct srb_t 
 *  \brief This struct contains information about the paramters of SRB  
 */
typedef struct _srb_t
{
	rrm_bitmask_t bitmask;
	rrm_bool_et			default_configuration;	
	srb_info_t			srb_params;	
}srb_t;

/* BUG_371 FIX START */
/*! \struct rrm_cell_access_related_infomation_t 
 *  \brief This struct contains cell access related information parameters.
 */
typedef struct _rrm_cell_access_related_infomation_t 
{
	U8	intra_frequency_reselection;
}rrm_cell_access_related_infomation_t;
 /* BUG_371 FIX END */

/*! \struct cell_and_access_barring_t 
 *  \brief This struct contains information about the paramters related to cell access and cell barring.  
 */
typedef struct _cell_and_access_barring_t 
{
  /*BUG_866_CHANGES_START*/
    /* SPR 10730 Fix Start */
    rrm_bitmask_t                      bitmask; /*^ BITMASK ^*/
    /*BUG_866_CHANGES_END*/

	rrm_oam_cell_barred_et   	cell_barred;
    /* SPR 10730 Fix End */
	rrm_bool_et			barring_for_emergency;
  /* BUG_371 FIX START */
	rrm_cell_access_related_infomation_t cell_access_info;
  /* BUG_371 FIX END */
}cell_and_access_barring_t;

/*! \struct cell_restriction_params_t
 *  \brief This struct contains information about the parameters related with cell restriction.
 */
typedef struct _cell_restriction_params_t 
{
	cell_and_access_barring_t	cell_and_access_barring;
}cell_restriction_params_t;

/* MEAS_CONFIG Start */
/*! \struct rrm_common_params_for_eutra_t 
 *  \brief This struct contains information about the common parameters related to intra-EUTRA connected mode mobility
 *  \param filter_coefficient_rsrp value of filter coefficient rsrp
 *  \param filter_coefficient_rsrq value of filter coefficient rsrq
 *  \param a1_threshold_rsrp value of a1 threshold rsrp
 *  \param a1_threshold_rsrq value of a1 threshold rsrq
 *  \param a2_threshold_rsrp value of a2 threshold rsrp
 *  \param a2_threshold_rsrq value of a2 threshold rsrq
 *  \param a3_offset value of a3 offset
 *  \param report_on_leave value of report of leave enabled or disabled
 *  \param a4_threshold_rsrp value of a4 threshold rsrp
 *  \param a4_threshold_rsrq value of a4 threshold rsrq
 *  \param a5_threshold_1rsrp value of a5 threshold1 rsrp for serving cell
 *  \param a5_threshold_1rsrq value of a5 threshold1 rsrq for serving cell
 *  \param a5_threshold_2rsrp value of a5 threshold2 rsrp for target cell
 *  \param a5_threshold_2rsrq value of a5 threshold2 rsrq for target cell
 *  \param hysteresis value of hysteresis
 *  \param time_to_trigger specifies when to trigger the measurements
 *  \param trigger_quantity specifies the trigger quantity as RSRP or RSRQ
 *  \param report_quantity specifies UE to report RSRP or RSRQ or both
 *  \param report_interval specifies UE to time to report the measurements
 *  \param report_amount specifies the number of measurements to report
 *  \param max_report_cells value of max cells to report as part of measurements
 */
typedef struct _rrm_common_params_for_eutra_t
{
/* SPR 17529 Fix Start */
        rrm_bitmask_t                   bitmask;
#define RRMCM_RMIF_MIN_THRESHOLD_RSRQ_PRESENT                  0x000001
/* SPR 17529 Fix End */
        rrm_filter_coefficient_et       filter_coefficient_rsrp;/*^ M, 0, H, 0, 14 ^*/
        rrm_filter_coefficient_et       filter_coefficient_rsrq;/*^ M, 0, H, 0, 14 ^*/
        U8      a1_threshold_rsrp;/*^ M, 0, H, 0, 97 ^*/
        U8      a1_threshold_rsrq;/*^ M, 0, H, 0, 34^*/
        U8      a2_threshold_rsrp;/*^ M, 0, H, 0, 97 ^*/
        U8      a2_threshold_rsrq;/*^ M, 0, H, 0, 34 ^*/
        S8      a3_offset;/*^ M, 0, B, -30, 30 ^*/
        U8      report_on_leave;/*^ M, 0, H, 0, 1 ^*/
        U8      a4_threshold_rsrp;/*^ M, 0, H, 0, 97 ^*/
        U8      a4_threshold_rsrq;/*^ M, 0, H, 0, 34 ^*/
        U8      a5_threshold_1rsrp;/*^ M, 0, H, 0, 97 ^*/
        U8      a5_threshold_1rsrq;/*^ M, 0, H, 0, 34 ^*/
        U8      a5_threshold_2rsrp;/*^ M, 0, H, 0, 97 ^*/
        U8      a5_threshold_2rsrq;/*^ M, 0, H, 0, 34 ^*/
        U8      hysteresis;/*^ M, 0, H, 0, 30 ^*/
        rrmc_time_to_trigger_et         time_to_trigger;/*^ M, 0, H, 0, 15 ^*/
        rrmc_trigger_quantity_et        trigger_quantity;/*^ M, 0, H, 0, 1 ^*/
        rrmc_report_quantity_et         report_quantity;/*^ M, 0, H, 0, 1 ^*/
        rrmc_report_interval_et         report_interval;/*^ M, 0, H, 0, 12 ^*/
        rrmc_report_amount_et           report_amount;/*^ M, 0, H, 0, 7 ^*/
        U8                              max_report_cells; /*^ M, 0, H, 1, 8 ^*/
        /* Parameter from rrm_oam.cfg*/
        rrm_bool_et      ps_ho_enabled;
        /* Parameter from rrm_oam.cfg*/
        /* SPR 16880: start */
        /* Code deleted */
        /* SPR 16880: end */
/* SPR 17529 Fix Start */
        U8      min_threshold_rsrq;    /*^ O,RRMCM_RMIF_MIN_THRESHOLD_RSRQ_PRESENT , H, 0, 34 ^*/
/* SPR 17529 Fix End */
}rrm_common_params_for_eutra_t;

typedef struct _rrmc_ue_generic_cdma2000_params_t
{
    rrm_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define RRMCM_RMIF_AUTH_INCLUDED_PRESENT                   0x0000001
#define RRMCM_RMIF_MAX_NUM_ALT_SO_INCLUDED_PRESENT         0x0000002
#define RRMCM_RMIF_USE_SYNC_ID_INCLUDED_PRESENT            0x0000004
#define RRMCM_RMIF_MOB_QOS_INCLUDED_PRESENT                0x0000008
#define RRMCM_RMIF_BYPASS_REG_IND_INCLUDED_PRESENT         0x0000010
#define RRMCM_RMIF_MAX_ADD_SERV_INSTANCE_INCLUDED_PRESENT  0x0000020
#define RRMCM_RMIF_PARAMETER_REG_INCLUDED_PRESENT          0x0000040
#define RRMCM_RMIF_REG_DIST_INCLUDED_PRESENT               0x0000080
#define RRMCM_RMIF_PREF_MSID_TYPE_INCLUDED_PRESENT         0x0000100
#define RRMCM_RMIF_EXT_PREF_MSID_TYPE_INCLUDED_PRESENT     0x0000200
#define RRMCM_RMIF_MEID_REQD_INCLUDED_PRESENT              0x0000400
#define RRMCM_RMIF_MCC_INCLUDED_PRESENT                    0x0000800
#define RRMCM_RMIF_IMSI_11_12_INCLUDED_PRESENT             0x0001000
#define RRMCM_RMIF_IMSI_T_SUPPORTED_INCLUDED_PRESENT       0x0002000
#define RRMCM_RMIF_RECONNECT_MSG_IND_INCLUDED_PRESENT      0x0004000
#define RRMCM_RMIF_RER_MODE_SUPPORTED_INCLUDED_PRESENT     0x0008000
#define RRMCM_RMIF_PILOT_REPORT_INCLUDED_PRESENT           0x0010000
#define RRMCM_RMIF_SDB_SUPPORTED_INCLUDED_PRESENT          0x0020000
#define RRMCM_RMIF_AUTO_FCSO_ALLOWED_INCLUDED_PRESENT      0x0040000
#define RRMCM_RMIF_SDB_IN_RCNM_IND_INCLUDED_PRESENT        0x0080000
#define RRMCM_RMIF_DAYLT_INCLUDED_PRESENT                  0x0100000
#define RRMCM_RMIF_GCSNA_L2_ACK_TIMER_INCLUDED_PRESENT     0x0200000
#define RRMCM_RMIF_GCSNA_SEQUENCE_CONTEXT_TIMER_INCLUDED_PRESENT 0x0400000
/* Bug 12675 Fix Start */
#define RRMCM_RMIF_RAND_INCLUDED_PRESENT                   0x0800000
#define RRMCM_RMIF_LTM_OFF_INCLUDED_PRESENT                0x1000000
/* Bug 12675 Fix Stop */
    /*ue generic*/
    U8      auth;
    /*^ O,RRMCM_RMIF_AUTH_INCLUDED_PRESENT, H, 0, 3 ^*/
    U8      max_num_alt_so;
    /*^ O,RRMCM_RMIF_MAX_NUM_ALT_SO_INCLUDED_PRESENT, H, 0, 7 ^*/
    U8      use_sync_id;
    /*^ O,RRMCM_RMIF_USE_SYNC_ID_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      mob_qos;
    /*^ O,RRMCM_RMIF_MOB_QOS_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      bypass_reg_ind;
    /*^ O,RRMCM_RMIF_BYPASS_REG_IND_INCLUDED_PRESENT, H, 0, 31 ^*/
    U8      max_add_serv_instance;
    /*^ O,RRMCM_RMIF_MAX_ADD_SERV_INSTANCE_INCLUDED_PRESENT, H, 0, 7 ^*/
    U8      parameter_reg;
    /*^ O,RRMCM_RMIF_PARAMETER_REG_INCLUDED_PRESENT, H, 0, 1 ^*/
    U16     reg_dist;
    /*^ O,RRMCM_RMIF_REG_DIST_INCLUDED_PRESENT, H, 0, 2047 ^*/
    U8      pref_msid_type;
    /*^ O,RRMCM_RMIF_PREF_MSID_TYPE_INCLUDED_PRESENT, H, 0, 3 ^*/
    U8      ext_pref_msid_type;
    /*^ O,RRMCM_RMIF_EXT_PREF_MSID_TYPE_INCLUDED_PRESENT, H, 0, 3 ^*/
    U8      meid_reqd;
    /*^ O,RRMCM_RMIF_MEID_REQD_INCLUDED_PRESENT, H, 0, 1 ^*/
    U16     mcc;
    /*^ O,RRMCM_RMIF_MCC_INCLUDED_PRESENT, H, 0, 1023 ^*/
    U8      imsi_11_12;
    /*^ O,RRMCM_RMIF_IMSI_11_12_INCLUDED_PRESENT, H, 0, 127 ^*/
    U8      imsi_t_supported;
    /*^ O,RRMCM_RMIF_IMSI_T_SUPPORTED_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      reconnect_msg_ind;
    /*^ O,RRMCM_RMIF_RECONNECT_MSG_IND_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      rer_mode_supported;
    /*^ O,RRMCM_RMIF_RER_MODE_SUPPORTED_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      pilot_report;
    /*^ O,RRMCM_RMIF_PILOT_REPORT_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      sdb_supported;
    /*^ O,RRMCM_RMIF_SDB_SUPPORTED_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      auto_fcso_allowed;
    /*^ O,RRMCM_RMIF_AUTO_FCSO_ALLOWED_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      sdb_in_rcnm_ind;
    /*^ O,RRMCM_RMIF_SDB_IN_RCNM_IND_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      daylt;
    /*^ O,RRMCM_RMIF_DAYLT_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      gcsna_l2_ack_timer;
    /*^ O,RRMCM_RMIF_GCSNA_L2_ACK_TIMER_INCLUDED_PRESENT, H, 0, 255 ^*/
    U8      gcsna_sequence_context_timer;
    /*^ O,RRMCM_RMIF_GCSNA_SEQUENCE_CONTEXT_TIMER_INCLUDED_PRESENT, H, 0, 255 ^*/
    /*ue generic*/
}rrmc_ue_generic_cdma2000_params_t;

#define RRMCM_RMIF_B1_THRESHOLD_UTRA_RSCP_PRESENT  0x0001
#define RRMCM_RMIF_B1_THRESHOLD_UTRA_ECN0_PRESENT  0x0002
#define RRMCM_RMIF_B1_THRESHOLD_GERAN_PRESENT      0x0004
#define RRMCM_RMIF_B1_THRESHOLD_2UTRA_RSCP_PRESENT 0x0100
#define RRMCM_RMIF_B1_THRESHOLD_2UTRA_ECN0_PRESENT 0x0200
#define RRMCM_RMIF_B1_THRESHOLD_2GERAN_PRESENT     0x0400
#define RRM_OAM_MEAS_QUANUTRA_FDD_PRESENT          0x1000
#define RRM_OAM_MEAS_QUANUTRA_TDD_PRESENT          0x2000
#define RRMCM_RMIF_UE_GENERIC_PARAMS_PRESENT          0x4000

/* ENDC_MEAS_CHANGES_START */
#define  RRMCM_RMIF_B1_NR_THRESHOLD_RSRP_PRESENT       0x0080
#define RRMCM_RMIF_B1_NR_THRESHOLD_RSRQ_PRESENT      0x10000
#define  RRMCM_RMIF_B1_NR_THRESHOLD_SINR_PRESENT      0x20000
#define  RRMCM_RMIF_B2_NR_THRESHOLD2_RSRP_PRESENT     0x40000
#define  RRMCM_RMIF_B2_NR_THRESHOLD2_RSRQ_PRESENT     0x80000
#define  RRMCM_RMIF_B2_NR_THRESHOLD2_SINR_PRESENT    0x100000
#define  RRMCM_RMIF_B2_NR_THRESHOLD1_RSRP_EUTRA_PRESENT 0x200000
#define  RRMCM_RMIF_B2_NR_THRESHOLD1_RSRQ_EUTRA_PRESENT 0x400000
#define  RRMCM_RMIF_NR_OFFSET_FREQ_PRESENT              0x800000
#define  RRMCM_RMIF_NR_QUANTITY_CONFIG_SET_PRESENT      0x1000000
#define  RRMCM_RMIF_NR_REPORT_QUANTITY_PRESENT          0x2000000
/* NR_REP_CONFIG_FIX_START */
#define  RRMCM_RMIF_NR_TRIGGER_QUANTITY_PRESENT          0x4000000
/* NR_REP_CONFIG_FIX_END */
#define  RRMCM_RMIF_NR_FILTER_COEFF_RSRP_PRESENT        0x8000000
#define  RRMCM_RMIF_NR_FILTER_COEFF_RSRQ_PRESENT        0x10000000
#define  RRMCM_RMIF_NR_FILTER_COEFF_SINR_PRESENT        0x20000000
#define  RRMCM_RMIF_NR_REPORT_ON_LEAVE_PRESENT          0x40000000
/* ENDC_MEAS_CHANGES_END */

/*! \struct rrm_irat_params_t 
 *  \brief This struct contains information about parameters related to IRAT connected mode mobility.
 *  \param bitmask value of optional parameters present
 *  \param qoffset_tutra value of qoffset for utra
 *  \param filter_coefficient_utra value of filter coefficient utra
 *  \param meas_quantity_utra_fdd value of measurement quantity for utra 
 *  \param b1_threshold_utra_rscp value of b1 threshold rscp
 *  \param b1_threshold_utra_ecn0 value of b1 threshold ecno
 *  \param q_offset_geran value of qoffset for geran
 *  \param filter_coefficient_geran  value of filter coefficient geran
 *  \param b1_threshold_geran value of b1 threshold for geran
 *  \param b2_threshold_2utra_rscp value of b2 threshold2 rscp for utra
 *  \param b2_threshold_2utra_ecn0 value of b2 threshold2 ecno for utra
 *  \param b2_threshold_2geran value of b2 threshold for geran
 *  \param hysteresis value of inter RAT cells hysteresis
 *  \param time_to_trigger specifies when to trigger the measurements
 *  \param report_interval specifies UE to time to report the measurements
 *  \param report_amount specifies the number of measurements to report
 *  \param max_report_cells value of max cells to report as part of measurements
 */
typedef struct _rrm_irat_params_t
{
        rrm_bitmask_t                           bitmask; /*^ BITMASK ^*/
        S8                                      qoffset_tutra;/*^ M, 0, B, -15, 15 ^*/  /*INTEGER (-15..15)*/
        rrm_filter_coefficient_et               filter_coefficient_utra;/*^ M, 0, H, 0, 14 ^*/
        rrm_irat_measquanutrafdd_et             meas_quantity_utra_fdd;/*^ O, RRM_OAM_MEAS_QUANUTRA_FDD_PRESENT, H, 0, 1 ^*/
        rrm_irat_measquanutratdd_et             meas_quantity_utra_tdd;/*^ O, RRM_OAM_MEAS_QUANUTRA_TDD_PRESENT, H, 0, 1 ^*/
        S8                                      b1_threshold_utra_rscp;/*^ O, RRMCM_RMIF_B1_THRESHOLD_UTRA_RSCP_PRESENT, B, -5, 91 ^*/
        U8                                      b1_threshold_utra_ecn0;/*^ O, RRMCM_RMIF_B1_THRESHOLD_UTRA_ECN0_PRESENT, H, 0, 49 ^*/
        S8                                      q_offset_geran;/*^ M, 0, H, 0, 30 ^*//*Integer [-15:15]*/
        rrm_filter_coefficient_et               filter_coefficient_geran;/*^ M, 0, H, 0, 14 ^*/
        U8                                      b1_threshold_geran;/*^ O, RRMCM_RMIF_B1_THRESHOLD_GERAN_PRESENT, H, 0, 63 ^*/
        S8                                      b2_threshold_2utra_rscp;/*^ O, RRMCM_RMIF_B1_THRESHOLD_2UTRA_RSCP_PRESENT, B, -5, 91 ^*/
        U8                                      b2_threshold_2utra_ecn0;/*^ O, RRMCM_RMIF_B1_THRESHOLD_2UTRA_ECN0_PRESENT, H, 0, 49 ^*/
        U8                                      b2_threshold_2geran;/*^ O, RRMCM_RMIF_B1_THRESHOLD_2GERAN_PRESENT, H, 0, 63 ^*/
        U8                                      hysteresis;/*^ M, 0, H, 0, 30 ^*/
        rrmc_time_to_trigger_et                 time_to_trigger;/*^ M, 0, H, 0, 15 ^*/
        U8                                      max_report_cells;/*^ M, 0, B, 1, 8 ^*/
        rrmc_report_interval_et                 report_interval;/*^ M, 0, H, 0, 12 ^*/
        rrmc_report_amount_et                   report_amount;/*^ M, 0, H, 0, 7 ^*/
        U8                                      b2_threshold_cdma2000;
        rrmc_ue_generic_cdma2000_params_t       ue_generic_cdma2000_params;
        /* Bug 13021 fix start */
        U8                              b2_threshold_1rsrp;   /*^ M, 0, H, 0, 97 ^*/
        U8                              b2_threshold_1rsrq;   /*^ M, 0, H, 0, 34 ^*/
        /* Bug 13021 fix end */
        /* SPR 16880: start */
        U8                              meas_quantity_cdma2000; 
        U8                              b1_threshold_cdma2000;
        /* SPR 16880: end */
/* ENDC_MEAS_CHANGES_START */
    /* NR Threshholds */
    U8                b1_nr_threshold_rsrp; /*^ O, RRMCM_RMIF_B1_NR_THRESHOLD_RSRP_PRESENT, H, 0, 127 ^*/
    U8                b1_nr_threshold_rsrq; /*^ O, RRMCM_RMIF_B1_NR_THRESHOLD_RSRQ_PRESENT, H, 0, 127 ^*/
    U8                b1_nr_threshold_sinr; /*^ O, RRMCM_RMIF_B1_NR_THRESHOLD_SINR_PRESENT, H, 0, 127 ^*/
    U8                b2_nr_threshold2_rsrp; /*^ O, RRMCM_RMIF_B2_NR_THRESHOLD2_RSRP_PRESENT, H, 0, 127 ^*/
    U8                b2_nr_threshold2_rsrq; /*^ O, RRMCM_RMIF_B2_NR_THRESHOLD2_RSRQ_PRESENT, H, 0, 127 ^*/
    U8                b2_nr_threshold2_sinr; /*^ O, RRMCM_RMIF_B2_NR_THRESHOLD2_SINR_PRESENT, H, 0, 127 ^*/
    U8                b2_nr_threshold1_rsrp_eutra; /*^ O, RRMCM_RMIF_B2_NR_THRESHOLD1_RSRP_EUTRA_PRESENT, H, 0, 127 ^*/
    U8                b2_nr_threshold1_rsrq_eutra; /*^ O, RRMCM_RMIF_B2_NR_THRESHOLD1_RSRQ_EUTRA_PRESENT, H, 0, 127 ^*/
    S8                offset_freq_r15; /*^ O, RRMCM_RMIF_NR_OFFSET_FREQ_PRESENT, B, -15, 15 ^*/
    U8                quantity_config_set_r15; /*^ O, RRMCM_RMIF_NR_QUANTITY_CONFIG_SET_PRESENT, B, 1, 2 ^*/ 
    U8                report_quantity_nr; /*^ O, RRMCM_RMIF_NR_REPORT_QUANTITY_PRESENT, N, 0, 0 ^*/
    /* rrm_report_quantity_nr_et */
/* NR_REP_CONFIG_FIX_START */
    U8                trigger_quantity_nr; /*^ O, RRMCM_RMIF_NR_TRIGGER_QUANTITY_PRESENT, N, 0, 0 ^*/
    /* rrm_trigger_quantity_nr_et */
/* NR_REP_CONFIG_FIX_END */
    U8                filter_coeff_rsrp_r15;/*^ O, RRMCM_RMIF_NR_FILTER_COEFF_RSRP_PRESENT, H, 0, 14 ^*/ 
    /* rrm_filter_coefficient_et */
    U8                filter_coeff_rsrq_r15;/*^ O, RRMCM_RMIF_NR_FILTER_COEFF_RSRQ_PRESENT, H, 0, 14 ^*/ 
    /* rrm_filter_coefficient_et */
    U8                filter_coeff_sinr_r15;/*^ O, RRMCM_RMIF_NR_FILTER_COEFF_SINR_PRESENT, H, 0, 14 ^*/ 
    /* rrm_filter_coefficient_et */
    U8               report_on_leave_r15; /*^ O, RRMCM_RMIF_NR_REPORT_ON_LEAVE_PRESENT, H, 0, 1 ^*/
   /* rrm_bool_et */
/* ENDC_MEAS_CHANGES_END */
}rrm_irat_params_t;

#define RRMCM_RMIF_COMMON_PARAMS_FOR_EUTRA_PRESENT           0x01
#define RRMCM_RMIF_CONNECTED_MODE_MOBILITY_INTER_RAT_PRESENT 0x02
/*! \struct rrm_connected_mode_mobility_params_t 
 *  \brief This struct conatins information of the parameters related to connected mode mobility params.  
 *  \param bitmask store the value of presence of common & Inter RAT params
 *  \param common_params_for_eutra structure to store connected mode common params
 *  \param irat structure to store inter RAT params 
 */
typedef struct _rrm_connected_mode_mobility_params
{
        rrm_bitmask_t                           bitmask; /*^ BITMASK ^*/
        rrm_common_params_for_eutra_t           common_params_for_eutra;/*^ M, 0, N, 0, 0 ^*/
        rrm_irat_params_t                       irat;/*^ O, 0, N, 0, 0 ^*/
}rrm_connected_mode_mobility_params_t;
/* MEAS_CONFIG End */

/*! \struct speed_state_params_t 
 *  \brief This struct contains information of the parameters used for speed state.
 */
typedef struct _speed_state_params_t 
{
	rrm_oam_q_hyst_sf_et 	 q_hyst_sf_medium;
        rrm_oam_q_hyst_sf_et	 q_hyst_sf_high;
	rrm_oam_t_evaluation_et	 t_evaluation;
	rrm_oam_t_hyst_normal_et t_hyst_normal; 
	U8			 n_cell_change_medium;
	U8			 n_cell_change_high;
}speed_state_params_t;

/*! \struct speed_scale_factors_t 
 *  \brief This struct contains inforamtion of the parameters used for
 *  \      scaling a mobility control related parameter.
 */
typedef struct _speed_scale_factors_t 
{
	rrm_oam_speed_state_scale_factors_et  	t_reselection_eutra_sf_medium;
	rrm_oam_speed_state_scale_factors_et	t_reselection_eutra_sf_high;
}speed_scale_factors_t;

#define RRMCM_RMIF_Q_RX_LEV_MIN_OFFSET_PRESENT           0x01
#define RRMCM_RMIF_S_INTRA_SEARCH_PRESENT                0x02
#define RRMCM_RMIF_S_NON_INTRA_SEARCH_PRESENT            0x04
#define RRMCM_RMIF_P_MAX_SIB1_PRESENT                    0x08
#define RRMCM_RMIF_P_MAX_SIB3_PRESENT                    0x10
#define RRMCM_RMIF_T_RESELECTION_EUTR_SPEED_SCALE_FACTOR_PRESENT 0x20
/*! \struct intra_freq_params_t 
 *  \brief This struct contains information about the intra frequency parameters.
 */
typedef struct _intra_freq_params_t 
{
	rrm_bitmask_t bitmask;
	S8	q_rx_lev_min_sib_1;
	S8	q_rx_lev_min_sib_3;
	S8	q_rx_lev_min_offset;
	U8	s_intra_search;
	U8	t_reselection_eutra;
	U8	s_non_intra_search;
	U8	cell_reselection_priority;
	S8	p_max_sib_1;
	S8	p_max_sib_3;
	U8	thresh_serving_low;
	speed_scale_factors_t speed_scale_factors;
	U8 neigh_cell_config;
}intra_freq_params_t;

/*! \struct thresholdx_q_r9_t 
 *  \brief This struct contains information about threshold.   
 */
typedef struct _thresholdx_q_r9_t 
{
	U8	thresh_serving_highq_r9;				 
	U8	thresh_serving_lowq_r9;				 
}thresholdx_q_r9_t;

#define RRMCM_RMIF_Q_QUAL_MIN_R9_PRESENT        0x01
#define RRMCM_RMIF_THRESHOLDX_Q_R9_PRESENT      0x02

/*! \struct rrm_inter_freq_carrier_freq_list_t 
 *  \brief This struct contains information about the inter frequency carrier list.   
 */
typedef struct _rrm_inter_freq_carrier_freq_list_t 
{
	rrm_bitmask_t	bitmask;
	U8	allowed_meas_bandwidth;
	U8	presence_antenna_port1;
	S8 	q_qual_min_r9;
	thresholdx_q_r9_t threshx_q_r9;
}rrm_inter_freq_carrier_freq_list_t;

#define RRMCM_RMIF_CM_OFFSET_FREQ_PRESENT     0x01
#define RRMCM_RMIF_INTER_FREQ_CELL_RESELECTION_PRIORITY_PRESENT 0x02
#define RRMCM_RMIF_P_MAX_PRESENT 0x04
#define RRMCM_RMIF_T_RESELECTION_EUTR_SPEED_SCALE_FACTOR_PRESENT1 0x08
/*! \struct inter_freq_params_t 
 *  \brief This struct contains information about inter frequency parameters. 
 * \param bitmask
 * \param eutra_carrier_arfcn
 * \param q_rx_lev_min_sib_5
 * \param q_offset_freq
 * \param t_reselection_eutra
 * \param cell_reselection_priority
 * \param thresh_x_high 
 * \param thresh_x_low
 * \param p_max 
 * \param neigh_cell_config
 * \param speed_scale_factors
 * \param inter_freq_params_op_specific
 */
typedef struct _inter_freq_params_t 
{
	rrm_bitmask_t bitmask;
	U32     eutra_carrier_arfcn;
	S8	q_rx_lev_min_sib_5;
	U8 	q_offset_freq;
    U8  cm_offset_freq;
	U8	t_reselection_eutra;
	U8	cell_reselection_priority;
	U8	thresh_x_high;
	U8	thresh_x_low;
	S8	p_max;
	U8	neigh_cell_config;
	speed_scale_factors_t speed_scale_factors;	
    rrm_inter_freq_carrier_freq_list_t	inter_freq_params_op_specific; //Not from OAM
}inter_freq_params_t;


#define RRMCM_RMIF_T_RESELECTION_UTRA_SF_MED_PRESENT 0x01
#define RRMCM_RMIF_T_RESELECTION_UTRA_SF_HIGH_PRESENT 0x02
/*! \struct irat_eutran_to_utran 
 *  \brief This struct contains information about UTRA relevant for inter-RAT cell re-selection from E-UTRA to UTRA. 
 */
typedef struct _irat_eutran_to_utran
{
	rrm_bitmask_t bitmask;
	U8	t_reselection_utra;
	rrm_oam_speed_state_scale_factors_et 	t_reselection_utra_sf_medium;
	rrm_oam_speed_state_scale_factors_et	t_reselection_utra_sf_high;
}irat_eutran_to_utran_t;

#define RRMCM_RMIF_IRAT_CELL_RESELECTION_PRIORITY_PRESENT 0x01
#define RRMCM_RMIF_THRESHOLD_Q_R9_T_PRESENT          0x02
/*! \struct irat_eutran_to_utran_fdd_t 
 *  \brief This struct contains information about UTRA FDD frequency carriers relevant for inter-RAT cell re-selection from EUTRA
 *  \      to UTRAN FDD.
 */
typedef struct _irat_eutran_to_utran_fdd_t 
{
	rrm_bitmask_t bitmask;
	U16 	utra_carrier_arfcn;
	S8  	q_rx_lev_min;
	S8	q_qual_min; 
	U8 	cell_reselection_priority; 
	U8 	thresh_x_high; 
	U8 	thresh_x_low; 
	S8 	p_max_utra;
	thresholdx_q_r9_t threshx_q_r9[MAX_UTRA_CARRIER_FDD]; 
}irat_eutran_to_utran_fdd_t;


#define RRMCM_RMIF_SPEED_STATE_PARAMS_PRESENT 0x01
/*! \ struct common_params_t 
 *  \brief This struct contains information about common params    
 */
typedef struct _common_params_t 
{
	rrm_bitmask_t bitmask;
	rrm_oam_q_hyst_et     q_hyst;
	speed_state_params_t  speed_state_params;
}common_params_t;

#define RRMCM_RMIF_EUTRA_UTRA_RESELECTION_SPEED_STATE_PARAMS_PRESENT   0x01
/*! \ struct irat_eutra_utra_reselection_params_t 
 *  \brief This struct contains information about UTRA relevant for inter-RAT cell re-selection from E-UTRA to UTRA. 
 */
typedef struct _irat_eutra_utra_reselection_params_t 
{
 rrm_bitmask_t    bitmask;
 U8               t_reselection_utra;
 speed_scale_factors_t speed_state_scale_factor;
}irat_eutra_utra_reselection_params_t;

/*! \struct irat_eutran_utran_fdd_carriers_t 
 *  \brief This struct contains information about UTRA FDD frequency carriers relevant for inter-RAT cell re-selection from EUTRA
 *  \      to UTRAN FDD.
 */
#define RRMCM_RMIF_CELL_RESELECT_PRIORITY_PRESENT  0x01
#define RRMCM_RMIF_THRESHX_Q_R9_PRESENT            0x02
#define RRMCM_RMIF_OFFSET_FREQ_PRESENT             0X04
typedef struct irat_eutran_utran_fdd_carriers_t 
{
  rrm_bitmask_t  bitmask; 
  U16   utra_carrier_arfcn;
  S8    q_rx_lev_min;
  S8    q_qual_min;
  U8    cell_reselection_priority; 
  U8    thresh_x_high; 
  U8    thresh_x_low;
  S8    p_max_utra;
  thresholdx_q_r9_t threshx_q_r9;
  S8    offset_freq;
}irat_eutran_utran_fdd_carriers_t;

/*! \struct irat_eutran_utran_tdd_carriers_t
 *  \brief This struct contains information about UTRA TDD frequency carriers relevant for inter-RAT cell re-selection from EUTRA
 *  \      to UTRAN TDD.
 */

#define RRMCM_RMIF_TDD_OFFSET_FREQ_PRESENT             0X02
typedef struct irat_eutran_utran_tdd_carriers_t
{
    rrm_bitmask_t  bitmask; /*^ BITMASK ^*/
    U16   utra_carrier_arfcn; /*^ M, 0, H, 0, 16383 ^*/
    U8    cell_reselection_priority;  /*^ O, RRM_OAM_CELL_RESELECT_PRIORITY_PRESENT, H, 0, 7 ^*/
    U8    thresh_x_high; /*^ M, 0, H, 0, 31 ^*/
    U8    thresh_x_low; /*^ M, 0, H, 0, 31 ^*/
    S8    q_rx_lev_min; /*^ M, 0, B, -60, -13 ^*/
    S8    p_max_utra; /*^ M, 0, B, -50, 33 ^*/
    /* TDD Support SON Start */
    S8    offset_freq;/*^ O, RRMCM_RMIF_TDD_OFFSET_FREQ_PRESENT, B, -15, 15 ^*/
    /* TDD Support SON End */
}irat_eutran_utran_tdd_carriers_t;

/*! \struct irat_eutran_utran_fdd_list 
 *  \brief This struct contains information about IRAT euran to utran FDD list. 
 */
typedef struct irat_eutran_utran_fdd_list
{
 U8  num_irat_eutran_to_utran_fdd_carriers; 
 irat_eutran_utran_fdd_carriers_t  irat_eutran_to_utran_fdd_carriers[RRM_MAX_NUM_UTRA_FDD_CARRIERS];
}irat_eutran_utran_fdd_list_t;

/*! \struct irat_eutran_utran_tdd_list
 *  \brief This struct contains information about IRAT euran to utran TDD list.
 */
typedef struct irat_eutran_utran_tdd_list
{
    U8  num_irat_eutran_to_utran_tdd_carriers;
    irat_eutran_utran_tdd_carriers_t  irat_eutran_to_utran_tdd_carriers[RRM_MAX_NUM_UTRA_TDD_CARRIERS];
}irat_eutran_utran_tdd_list_t;

#define RRMCM_RMIF_IRAT_EUTRAN_UTRAN_FDD_LIST_PRESENT    0x01
#define RRMCM_RMIF_IRAT_EUTRAN_UTRAN_TDD_LIST_PRESENT    0x02
/*! \ struct idle_mode_mobility_irat_utra_params_t 
 *  \brief This struct contains information about the parameters of idle mode mobility IRAT utran FDD list. 
 */
typedef struct _idle_mode_mobility_irat_utra_params_t 
{
  rrm_bitmask_t                         bitmask;
  irat_eutra_utra_reselection_params_t  irat_eutra_to_utra_reselection_params;
  irat_eutran_utran_fdd_list_t          irat_eutran_to_utran_fdd_list;
  irat_eutran_utran_tdd_list_t          irat_eutran_to_utran_tdd_list;
}idle_mode_mobility_irat_utra_params_t;


#define RRMCM_RMIF_EUTRA_GERAN_RESELECTION_SPEED_STATE_PARAMS_PRESENT   0x01
/*! \ struct irat_eutra_geran_reselection_params_t
 *  \brief This struct contains information about GERAN relevant for inter-RAT cell re-selection from E-UTRA to GERAN.
 */
typedef struct _irat_eutra_geran_reselection_params_t
{
 rrm_bitmask_t    bitmask;
 U8               t_reselection_geran;
 speed_scale_factors_t speed_state_scale_factor;
}irat_eutra_geran_reselection_params_t;

#define RRMCM_GERAN_CFI_CELL_RESELECTION_PRIORITY_PRESENT       0x01
#define RRMCM_GERAN_P_MAX_GERAN_PRESENT           0x02
#define RRMCM_GERAN_OFFSET_FREQ_PRESENT           0x04
typedef struct _irat_eutran_geran_list_t
{
    rrm_bitmask_t                   bitmask;
    rrm_son_geran_carrier_freqs_t   geran_car_freqs;
    U8                              cell_reselection_priority;
    U8                              ncc_permitted;
    U8                              q_rx_lev_min;
    U8                              p_max_geran;
    U8                              threshX_high;
    U8                              threshX_low;
    S8                              offset_freq;
}irat_eutran_geran_carriers_t;

/*! \struct irat_eutran_geran_list_t
 *  \brief This struct contains information about IRAT eutran to geran list.
 */
typedef struct irat_eutran_geran_list
{
 U8  num_irat_eutran_to_geran_carriers;
 irat_eutran_geran_carriers_t  irat_eutran_to_geran_carriers[MAX_NO_GERAN_FREQ_GROUUP];
}irat_eutran_geran_list_t;

#define RRMCM_RMIF_IRAT_EUTRAN_GERAN_FREQ_LIST_PRESENT    0x01
/*! \ struct idle_mode_mobility_irat_geran_params_t
 *  \brief This struct contains information about the parameters of idle mode mobility IRAT geran FDD list.
 */
typedef struct _idle_mode_mobility_irat_geran_params_t
{
  rrm_bitmask_t                         bitmask;
  irat_eutra_geran_reselection_params_t  irat_eutra_to_geran_reselection_params;
  irat_eutran_geran_list_t          irat_eutran_to_geran_list;
}idle_mode_mobility_irat_geran_params_t;

/* CDMA2000 changes start */

#define RRMCM_RMIF_BAND_CLASS_INFO_CDMA2000_CELL_RESELECT_PRI_PRESENCE_FLAG  0x01
typedef struct rrmcm_rmif_band_class_info_cdma2000
{
    rrm_bitmask_t    presence_bitmask;          /*^ BITMASK ^*/
    U8     band_class;
    /*^ M, 0, H, 0, 31 ^*/   /* rrm_oam_bandclass_cdma2000_et */

    U8     cell_reselection_priority;
    /*^ O, RRMCM_RMIF_BAND_CLASS_INFO_CDMA2000_CELL_RESELECT_PRI_PRESENCE_FLAG, H, 1, 7 ^*/

    U8     thresh_x_high;             /*^ M, 0, H, 0, 63 ^*/
    U8     thresh_x_low;              /*^ M, 0, H, 0, 63 ^*/
}rrmcm_rmif_band_class_info_cdma2000_t;

typedef struct rrmcm_rmif_band_class_list_cdma2000
{
    U8                          count;                  /*^ M, 0, H, 1, 32 ^*/
    rrmcm_rmif_band_class_info_cdma2000_t  band_class_info_cdma2000[RRMCM_RMIF_MAX_CDMA_BAND_CLASS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

}rrmcm_rmif_band_class_list_cdma2000_t;

typedef struct
{
        rrm_bitmask_t                      bitmask;
#define RRMCM_RMIF_PZ_HYST_ENABLD_PRESENT                      0x01
#define RRMCM_RMIF_PZ_HYST_INFO_INCL_PRESENT                   0x02
#define RRMCM_RMIF_PZ_HYST_LIST_LEN_PRESENT                    0x04
#define RRMCM_RMIF_PZ_HYST_ACT_TIMER_PRESENT                   0x08
#define RRMCM_RMIF_PZ_HYST_TIMER_MUL_PRESENT                   0x10
#define RRMCM_RMIF_PZ_HYST_TIMER_EXP_PRESENT                   0x20
        U8                                pz_hyst_enabled;   
        /*^ O, RRMCM_RMIF_PZ_HYST_ENABLD_PRESENT, H, 0, 1 ^*/
        U8                                pz_hyst_info_incl; 
        /*^ O, RRMCM_RMIF_PZ_HYST_INFO_INCL_PRESENT, H, 0, 1 ^*/
        U8                                pz_hyst_list_len;  
        /*^ O, RRMCM_RMIF_PZ_HYST_LIST_LEN_PRESENT, H, 0, 1 ^*/
        U8                                pz_hyst_act_timer; 
        /*^ O, RRMCM_RMIF_PZ_HYST_ACT_TIMER_PRESENT, H, 0, 255 ^*/
        U8                                pz_hyst_timer_mul; 
        /*^ O, RRMCM_RMIF_PZ_HYST_TIMER_MUL_PRESENT, H, 0, 7 ^*/
        U8                                pz_hyst_timer_exp; 
        /*^ O, RRMCM_RMIF_PZ_HYST_TIMER_EXP_PRESENT, H, 0, 31 ^*/
}rrmcm_rmif_pz_hyst_parameters_included_t;

typedef struct
{
        rrm_bitmask_t                      bitmask;
#define RRMCM_RMIF_FPC_FCH_INIT_SETPT_RC3_PRESENT              0x01
#define RRMCM_RMIF_FPC_FCH_INIT_SETPT_RC4_PRESENT              0x02
#define RRMCM_RMIF_FPC_FCH_INIT_SETPT_RC5_PRESENT              0x04
/* Bug 12675 Fix Start */
#define RRMCM_RMIF_FPC_FCH_INIT_SETPT_RC11_PRESENT             0x08
#define RRMCM_RMIF_FPC_FCH_INIT_SETPT_RC12_PRESENT             0x10
/* Bug 12675 Fix stop */
        U8                                 fpc_fch_init_setpt_rc3; 
        /*^ O, RRMCM_RMIF_FPC_FCH_INIT_SETPT_RC3_PRESENT, H, 0, 255 ^*/
        U8                                 fpc_fch_init_setpt_rc4; 
        /*^ O, RRMCM_RMIF_FPC_FCH_INIT_SETPT_RC4_PRESENT, H, 0, 255 ^*/
        U8                                 fpc_fch_init_setpt_rc5; 
        /*^ O, RRMCM_RMIF_FPC_FCH_INIT_SETPT_RC5_PRESENT, H, 0, 255 ^*/
        /* Bug 12675 Fix Start */
        U8                                 fpc_fch_init_setpt_rc11;
        /*^ O, RRMCM_RMIF_FPC_FCH_INIT_SETPT_RC11_PRESENT, H, 0, 255 ^*/
        U8                                 fpc_fch_init_setpt_rc12;
        /*^ O, RRMCM_RMIF_FPC_FCH_INIT_SETPT_RC12_PRESENT, H, 0, 255 ^*/
        /* Bug 12675 Fix Stop */

}rrmcm_rmif_fpc_fch_included_t;

#define RRMCM_RMIF_PZ_ID_HYST_PARAMETERS_INCLUDED_PRESENT      0x00001
#define RRMCM_RMIF_P_REV_INCLUDED_PRESENT                      0x00002
#define RRMCM_RMIF_MIN_P_REV_INCLUDED_PRESENT                  0x00004
#define RRMCM_RMIF_NEG_SLOT_CYCLE_INDEX_SUP_INCLUDED_PRESENT   0x00008
#define RRMCM_RMIF_ENCRYPT_MODE_INCLUDED_PRESENT               0x00010
#define RRMCM_RMIF_ENC_SUPPORTED_INCLUDED_PRESENT              0x00020
#define RRMCM_RMIF_SIG_ENCRYPT_SUP_INCLUDED_PRESENT            0x00040
#define RRMCM_RMIF_MSG_INTEGRITY_SUP_INCLUDED_PRESENT          0x00080
#define RRMCM_RMIF_SIG_INTEGRITY_SUP_INCL_INCLUDED_PRESENT     0x00100
#define RRMCM_RMIF_SIG_INTEGRITY_SUP_INCLUDED_PRESENT          0x00200
#define RRMCM_RMIF_MS_INIT_POS_LOC_SUP_IND_INCLUDED_PRESENT    0x00400
#define RRMCM_RMIF_BAND_CLASS_INFO_REQ_INCLUDED_PRESENT        0x00800
#define RRMCM_RMIF_BAND_CLASS_INCLUDED_PRESENT                 0x01000
#define RRMCM_RMIF_ALT_BAND_CLASS_INCLUDED_PRESENT             0x02000
#define RRMCM_RMIF_TKZ_MODE_SUPPORTED_INCLUDED_PRESENT         0x04000
#define RRMCM_RMIF_TKZ_ID_INCLUDED_PRESENT                     0x08000
#define RRMCM_RMIF_FPC_FCH_INCLUDED_PRESENT                    0x10000
#define RRMCM_RMIF_T_ADD_INCLUDED_PRESENT                      0x20000
#define RRMCM_RMIF_PILOT_INC_INCLUDED_PRESENT                  0x40000
typedef struct _rrmcm_rmif_cell_specific_params_t
{
    rrm_bitmask_t                      bitmask; /*^ BITMASK ^*/
    /*base station*/
    rrmcm_rmif_pz_hyst_parameters_included_t 
        pz_hyst_parameters_included;
    /*^ O,RRMCM_RMIF_PZ_ID_HYST_PARAMETERS_INCLUDED_PRESENT , N, 0, 0 ^*/
    U8      p_rev;
    /*^ O,RRMCM_RMIF_P_REV_INCLUDED_PRESENT , H, 0, 255 ^*/
    U8      min_p_rev;
    /*^ O,RRMCM_RMIF_MIN_P_REV_INCLUDED_PRESENT , H, 0, 255 ^*/
    U8      neg_slot_cycle_index_sup;
    /*^ O,RRMCM_RMIF_NEG_SLOT_CYCLE_INDEX_SUP_INCLUDED_PRESENT , H, 0, 1 ^*/
    U8      encrypt_mode;
    /*^ O,RRMCM_RMIF_ENCRYPT_MODE_INCLUDED_PRESENT , H, 0, 255 ^*/
    U8      enc_supported;
    /*^ O,RRMCM_RMIF_ENC_SUPPORTED_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      sig_encrypt_sup;
    /*^ O,RRMCM_RMIF_SIG_ENCRYPT_SUP_INCLUDED_PRESENT, H, 0, 255 ^*/
    U8      msg_integrity_sup;
    /*^ O,RRMCM_RMIF_MSG_INTEGRITY_SUP_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      sig_integrity_sup_incl;
    /*^ O,RRMCM_RMIF_SIG_INTEGRITY_SUP_INCL_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      sig_integrity_sup;
    /*^ O,RRMCM_RMIF_SIG_INTEGRITY_SUP_INCLUDED_PRESENT, H, 0, 255 ^*/
    U8      ms_init_pos_loc_sup_ind;
    /*^ O,RRMCM_RMIF_MS_INIT_POS_LOC_SUP_IND_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      band_class_info_req;
    /*^ O,RRMCM_RMIF_BAND_CLASS_INFO_REQ_INCLUDED_PRESENT, H, 0, 1 ^*/
    /* Start Bug Fix 786 */
    U32      band_class;
    /*^ O,RRMCM_RMIF_BAND_CLASS_INCLUDED_PRESENT, H, 0, 31 ^*/
    U32      alt_band_class;
    /*^ O,RRMCM_RMIF_ALT_BAND_CLASS_INCLUDED_PRESENT, H, 0, 31 ^*/
    /* End Bug Fix 786 */
    U8      tkz_mode_supported;
    /*^ O,RRMCM_RMIF_TKZ_MODE_SUPPORTED_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8      tkz_id;
    /*^ O,RRMCM_RMIF_TKZ_ID_INCLUDED_PRESENT, H, 0, 1 ^*/
    rrmcm_rmif_fpc_fch_included_t
        fpc_fch_included;
    /*^ O,RRMCM_RMIF_FPC_FCH_INCLUDED_PRESENT, N, 0, 0 ^*/
    U8      t_add;
    /*^ O,RRMCM_RMIF_T_ADD_INCLUDED_PRESENT, H, 0, 63 ^*/
    U8      pilot_inc;
    /*^ O,RRMCM_RMIF_PILOT_INC_INCLUDED_PRESENT, H, 0, 15 ^*/
    /*base station*/
}rrmcm_rmif_cell_specific_params_t;

typedef struct rrmcm_rmif_secondary_pre_reg_zone_id_list_hrpd
{
    U8 count;              /*^ M, 0, B, 1, 2 ^*/
    U8 pre_reg_zone_id[RRMCM_RMIF_PRE_REG_ZONE_ID]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrmcm_rmif_secondary_pre_reg_zone_id_list_hrpd_t;

#define RRMCM_RMIF_PRE_REG_INFO_HRPD_ZONE_ID_PRESENCE_FLAG         0x01
#define RRMCM_RMIF_PRE_REG_INFO_HRPD_SECONDARY_LST_PRESENCE_FLAG   0x02
typedef struct rrmcm_rmif_pre_reg_info_hrpd
{
    rrm_bitmask_t                         presence_bitmask; /*^ BITMASK ^*/
    U8                                    pre_reg_allowed;
    /*^ M, 0, H, 0, 1 ^*/    /* rrm_bool_et */

    U8                                    pre_reg_zone_id;
    /*^ O, RRMCM_RMIF_PRE_REG_INFO_HRPD_ZONE_ID_PRESENCE_FLAG, N, 0, 255 ^*/

    rrmcm_rmif_secondary_pre_reg_zone_id_list_hrpd_t secondary_list;
    /*^ O, RRMCM_RMIF_PRE_REG_INFO_HRPD_SECONDARY_LST_PRESENCE_FLAG, N, 0, 0 ^*/

}rrmcm_rmif_pre_reg_info_hrpd_t;

typedef struct rrmcm_rmif_ac_barring_config_1_xrtt_r9
{
    U8   ac_barring_0_to_9_r9; /*^ M, 0, H, 0, 63 ^*/
    U8   ac_barring_10_r9;   /*^ M, 0, H, 0, 7 ^*/
    U8   ac_barring_11_r9;   /*^ M, 0, H, 0, 7 ^*/
    U8   ac_barring_12_r9;   /*^ M, 0, H, 0, 7 ^*/
    U8   ac_barring_13_r9;   /*^ M, 0, H, 0, 7 ^*/
    U8   ac_barring_14_r9;   /*^ M, 0, H, 0, 7 ^*/
    U8   ac_barring_15_r9;   /*^ M, 0, H, 0, 7 ^*/
    U8   ac_barring_msg_r9;  /*^ M, 0, H, 0, 7 ^*/
    U8   ac_barring_reg_r9;  /*^ M, 0, H, 0, 7 ^*/
    U8   ac_barring_emg_r9;  /*^ M, 0, H, 0, 7 ^*/

}rrmcm_rmif_ac_barring_config_1_xrtt_r9_t;


typedef struct _rrmcm_rmif_sib_type_8_params_t
{
    rrm_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define RRMCM_RMIF_PACKET_ZONE_ID_INCLUDED_PRESENT             0x01
#define RRMCM_RMIF_POWER_DOWN_REG_INCLUDED_PRESENT             0x02
    /* for Sib-8*/
    U8  sid[RRMCM_RMIF_XRTT_SID_OCTET_SIZE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    U8  nid[RRMCM_RMIF_XRTT_NID_OCTET_SIZE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    U8  multiple_sid;             /*^ M, 0, H, 0, 1 ^*/     /* rrm_bool_et */
    U8  multiple_nid;             /*^ M, 0, H, 0, 1 ^*/     /* rrm_bool_et */
    U8  reg_zone[RRMCM_RMIF_XRTT_REG_ZONE_OCTET_SIZE];
    /*^ M, 0, OCTET_STRING, FIXED ^*/

    U8  total_zone[RRMCM_RMIF_XRTT_TOTAL_ZONE_OCTET_SIZE];
    /*^ M, 0, OCTET_STRING, FIXED ^*/

    U8  zone_timer[RRMCM_RMIF_XRTT_ZONE_TIMER_OCTET_SIZE];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    U8      packet_zone_id;
    /*^ O,RRMCM_RMIF_PACKET_ZONE_ID_INCLUDED_PRESENT , H, 0, 255 ^*/
    U8  home_reg;                 /*^ M, 0, H, 0, 1 ^*/     /* rrm_bool_et */
    U8  foreign_sid_reg;          /*^ M, 0, H, 0, 1 ^*/     /* rrm_bool_et */
    U8  foreign_nid_reg;          /*^ M, 0, H, 0, 1 ^*/     /* rrm_bool_et */
    U8  parame_reg;               /*^ M, 0, H, 0, 1 ^*/     /* rrm_bool_et */
    U8  power_up_reg;             /*^ M, 0, H, 0, 1 ^*/     /* rrm_bool_et */

    /* changes CDMA2000 after review starts */
    U8      reg_prd;              /*^ M, 0, N, 0, 0 ^*/
    /* changes CDMA2000 after review ends */

    U8      power_down_reg;
    /*^ O,RRMCM_RMIF_POWER_DOWN_REG_INCLUDED_PRESENT, H, 0, 1 ^*/
    /* for Sib-8*/
}rrmcm_rmif_mobility_sib_8_params_t;

typedef struct
{
    U32              rand_seed;     /*^ M, 0, H, 0, 4294967295 ^*/
    U32              rand_min;     /*^ M, 0, H, 0, 4294967295 ^*/
    U32              rand_max;     /*^ M, 0, H, 0, 4294967295 ^*/
    /* timer duration in seconds to regenerate new Random Value*/
    U32              rand_regenerate_timer;     /*^ M, 0, B, 1, 4294967296 ^*/
}rrmcm_rmif_cdma2000_rand_t;

typedef struct
{
    U8 cdma2000_1xrtt_cell_id[RRMCM_RMIF_CDMA2000_1XRTT_CELL_ID_LENGTH];
    /*^ M,0,OCTET_STRING,FIXED ^*/
}rrmcm_rmif_cdma2000_1xrtt_cell_identifier_t;

typedef struct
{
    U8 cdma2000_hrpd_cell_id_length; /*^ M,0,H,0,16 ^*/
    U8 cdma2000_hrpd_cell_id[RRM_CDMA2000_HRPD_CELL_ID_LENGTH]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}rrmcm_rmif_cdma2000_hrpd_cell_identifier_t;

typedef struct _rrmcm_rmif_cdma2000_cell_param_t
{
    rrm_bitmask_t    bitmask; /*^ BITMASK ^*/
#define RRMCM_RMIF_CDMA2000_PARAM_1XRTT_CELL_ID_PRESENCE_FLAG             0x01
#define RRMCM_RMIF_CDMA2000_PARAM_HRPD_CELL_ID_PRESENCE_FLAG              0x02
/* CSR 00070260 FIX START */
#define RRMCM_RMIF_CDMA2000_PARAM_1XRTT_REFERENCE_CELL_ID_PRESENCE_FLAG   0x04
#define RRMCM_RMIF_CDMA2000_PARAM_HRPD_REFERENCE_CELL_ID_PRESENCE_FLAG    0x08
/* CSR 00070260 FIX END */
    rrmcm_rmif_cdma2000_rand_t    cdma2000_rand;     /*^ M, 0, N, 0, 0 ^*/
    rrmcm_rmif_cdma2000_1xrtt_cell_identifier_t     
                               cell_id_1xrtt;     
    /*^ O, RRMCM_RMIF_CDMA2000_PARAM_1XRTT_CELL_ID_PRESENCE_FLAG, N, 0, 0 ^*/
    rrmcm_rmif_cdma2000_hrpd_cell_identifier_t
                               cell_id_hrpd;    
    /*^ O, RRMCM_RMIF_CDMA2000_PARAM_HRPD_CELL_ID_PRESENCE_FLAG, N, 0, 0 ^*/
    /* CSR 00070260 FIX START */
    U8 cdma2000_1xrtt_reference_cell_id[RRM_CDMA2000_1XRTT_REFERENCE_CELL_ID_LENGTH]; 
    /*^ O, RRMCM_RMIF_CDMA2000_PARAM_1XRTT_REFERENCE_CELL_ID_PRESENCE_FLAG, OCTET_STRING, FIXED ^*/
    U8 cdma2000_hrpd_reference_cell_id[RRM_CDMA2000_HRPD_REFERENCE_CELL_ID_LENGTH];
    /*^ O, RRMCM_RMIF_CDMA2000_PARAM_HRPD_REFERENCE_CELL_ID_PRESENCE_FLAG , OCTET_STRING, FIXED ^*/
    /* CSR 00070260 FIX END */
}rrmcm_rmif_cdma2000_cell_param_t;

typedef struct _rrmcm_rmif_irat_parameters_cdma2000_v920_t
{
   #define RRMCM_RMIF_CDMA2000_PARAM_CONC_PS_MOBILITY_PRESENCE_FLAG     0x01
    rrm_bitmask_t    bitmask; /*^ BITMASK ^*/
    U8                    eCSFB_1xrtt_r9;/*^ M, 0, N, 0, 0 ^*/
    /*rrm_oam_eCSFB_1xrtt_r9_et*/
    U8                    eCSFB_conc_ps_mobility_1xrtt_r9;
      /*^O, RRMCM_RMIF_CDMA2000_PARAM_CONC_PS_MOBILITY_PRESENCE_FLAG, N, 0, 0 ^*/
    /*rrm_oam_eCSFB_conc_ps_mobility_1xrtt_r9_et*/
}rrmcm_rmif_irat_parameters_cdma2000_v920_t;

#define RRMCM_RMIF_CELL_RESELECT_CDMA2000_SF_PRESENCE_FLAG 0x01
typedef struct _rrmcm_rmif_cell_reselection_params_cdma2000_t
{
    rrm_bitmask_t               presence_bitmask;
    rrmcm_rmif_band_class_list_cdma2000_t  band_class_list;
    U8                          t_reselection_cdma2000;
    speed_scale_factors_t       t_reselection_cdma2000_sf;
}rrmcm_rmif_cell_reselection_params_cdma2000_t;

typedef struct _rrmcm_rmif_supported_cdma2000_arfcn_t
{
    U8     valid_count_cdma2000_arfcn;
    U16    supported_cdma2000_arfcn[RRMCM_RMIF_NUM_CDMA2000_FREQ_CELLS];
}rrmcm_rmif_supported_cdma2000_arfcn_list_t;

#define RRMCM_RMIF_SEARCH_WIN_SIZE_PRESENCE_FLAG                   0x001
#define RRMCM_RMIF_CSFB_SUPPORT_FOR_DUAL_RX_UES_R9_PRESENCE_FLAG   0x002
#define RRMCM_RMIF_CSFB_REG_PARAM_1XRTT_V920_PRESENCE_FLAG         0x004
#define RRMCM_RMIF_AC_BARRING_CONFIG_1_XRTT_R9_PRESENCE_FLAG       0x008
#define RRMCM_RMIF_PRE_REG_INFO_HRPD_PRESENCE_FLAG                 0x010
#define RRMCM_RMIF_MOBILITY_SIB_8_PARAMS_PRESENCE_FLAG             0x020
#define RRMCM_RMIF_CDMA2000_CELL_PARAMS_PRESENCE_FLAG              0x040
#define RRMCM_RMIF_IRAT_PARAMS_CDMA2000_V920_PRESENCE_FLAG         0x080
#define RRMCM_RMIF_CELL_RESEL_PARAMS_HRPD_CDMA2000_PRESENCE_FLAG   0x100
#define RRMCM_RMIF_CELL_RESEL_PARAMS_1XRTT_CDMA2000_PRESENCE_FLAG  0x200
#define RRMCM_RMIF_SUPPORTED_CDMA2000_ARFCN_LIST_PRESENCE_FLAG     0x400
#define RRMCM_RMIF_SYSTEM_TIME_INFO_PRESENCE_FLAG                  0x800
typedef struct _rrmcm_idle_mode_mobility_irat_cdma_params_t
{
    rrm_bitmask_t             presence_bitmask;
    U8                        search_window_size;
    rrm_bool_et               csfb_support_for_dual_rx_ues_r9;
    rrm_oam_csfb_registration_param_1xrtt_v920_et    
                              csfb_registration_param_1xrtt_v920;
    rrmcm_rmif_ac_barring_config_1_xrtt_r9_t    ac_barring_config_1_xrtt_r9;
    rrmcm_rmif_pre_reg_info_hrpd_t              pre_reg_info_hrpd;
    rrmcm_rmif_mobility_sib_8_params_t          mobility_sib_8_params;
    rrmcm_rmif_cdma2000_cell_param_t            cdma2000_cell_param;
    rrmcm_rmif_irat_parameters_cdma2000_v920_t  
                              inter_rat_parameters_cdma2000_v920;

    /* changes CDMA2000 after review starts */
    rrmcm_rmif_cell_reselection_params_cdma2000_t  cell_reselection_params_hrpd;
    rrmcm_rmif_cell_reselection_params_cdma2000_t  cell_reselection_params_1xrtt;
    /* changes CDMA2000 after review ends */

    /* List of ARFCN supported by RRM for SON starts */
    rrmcm_rmif_supported_cdma2000_arfcn_list_t  supported_cdma2000_arfcn;
    /* List of ARFCN supported by RRM for SON ends */

    /* SPR 6008 */
    rrm_oam_system_time_info_et  system_time_info;
    /* SPR 6008 */

}rrmcm_idle_mode_mobility_irat_cdma_params_t;

/* CDMA2000 changes ends */
#define RRMCM_RMIF_IDLE_MODE_MOBILITY_COMMON_PARAMS_PRESENT 0x0001
#define RRMCM_MIF_IDLE_MODE_MOBILITY_INTRA_FREQ_PARAMS      0x0002
#define RRMCM_MIF_IDLE_MODE_MOBILITY_INTER_FREQ_PARAMS      0x0004
#define RRMCM_RMIF_IDLE_MODE_MOBILITY_IRAT_UTRA_PRESENT     0x0008
#define RRMCM_RMIF_IDLE_MODE_MOBILITY_IRAT_GERAN_PRESENT    0x0010
#define RRMCM_RMIF_IDLE_MODE_MOBILITY_IRAT_CDMA_PRESENT     0x0020
/*! \ struct idle_mode_mobility_params_t 
 *  \brief This struct conatins informatin baout the parameters of idle mode mobility. 
 */
typedef struct _idle_mode_mobility_params_t 
{
     rrm_bitmask_t                      bitmask;
     common_params_t    	        idle_mode_mobility_common_params;
     intra_freq_params_t 	        idle_mode_mobility_intra_freq_params;
     U8                                 num_valid_inter_freq_list;
     inter_freq_params_t		idle_mode_mobility_inter_freq_params[MAX_NO_INTER_FREQ];
     idle_mode_mobility_irat_utra_params_t  idle_mode_mobility_irat_utra_params;
     idle_mode_mobility_irat_geran_params_t idle_mode_mobility_irat_geran_params;
     rrmcm_idle_mode_mobility_irat_cdma_params_t idle_mode_mobility_irat_cdma_params;
}idle_mode_mobility_params_t;

#define RRMCM_RMIF_IDLE_MODE_MOBILITY_PARAMS_PRESENT    0x01
/*! \struct mobility_params_t 
 *  \brief this struct contains the parameters of mobility param for idle mode and rrc connected mode.
 *  \param idle_mode_mobility_params structure to store idle mode mobility parameters
 *  \param connected_mode_mobility_params structure to store connected mode mobility parameters
 */
typedef struct _mobility_params_t 
{
     rrm_bitmask_t                           bitmask;
     idle_mode_mobility_params_t 	     idle_mode_mobility_params;
}mobility_params_t;

/*eMTC Changes stop*/


/*! \struct mac_layer_params_t 
 *  \brief This struct contains the parameters of mac layer for RACH and DRX
 */
#define RRMCM_RMIF_MAC_LAYER_PARAM_RACH_PRESENT    0x01
#define RRMCM_RMIF_MAC_LAYER_PARAM_DRX_PRESENT     0x02
/* Fix for CSR: 00059124 */
#define RRMCM_RMIF_MAC_LAYER_PARAM_CDRX_PRESENT    0x04
/* Fix for CSR: 00059124 */
typedef struct _mac_layer_params_t 
{
    rrm_bitmask_t           bitmask;
	rach_t	      mac_layer_param_rach;
	drx_t			mac_layer_param_drx;
  /* BUG_371 FIX START */
  rrm_ul_sync_loss_timer_et        ul_sync_loss_timer;
  rrm_n_gap_et                     n_gap;
  /* BUG_371 FIX END */
    /* Fix for CSR: 00059124 start */
    cdrx_config_t   rrm_cdrx_config;
    /* Fix for CSR: 00059124 end */

    /* SPR 19309 + */
    U8              cfi_value;
    /* SPR 19309 - */
    /* SPR 19310 + */
    U16              num_of_eul;
    U16              size_of_eul;
    /* SPR 19310 - */
    /* SPR 21958 PUSCH RAT1 Support Start */
    U8 is_uplink_rat1_supported ;
    /* SPR 21958 PUSCH RAT1 Support End */
}mac_layer_params_t;

/*! \struct rlc_layer_params_t 
 *  \brief This struct contains information about the parameters of RLC layer. 
 */
typedef struct _rlc_layer_params_t 
{
	U8 		num_valid_srb_info;
	srb_t 	rlc_layer_param_srb[MAX_NO_SRB];/* array of 2 */	
}rlc_layer_params_t;

/*! \struct rrc_timers_t 
 *  \brief This struct contains information about RRC timers. 
 */
typedef struct _rrc_timers_t 
{
	rrm_oam_timer_300_301_et       t300;			/* from ue_timers_and_constants_t */
	rrm_oam_timer_300_301_et       t301;			/* from ue_timers_and_constants_t */
        rrm_oam_timer_t302_et          t302;	
	rrm_oam_timer_t304_eutra_et    t304_eutra;
	rrm_oam_timer_t304_irat_et     t304_irat;
	rrm_oam_timer_310_et 	       t310; 			/* from ue_timers_and_constants_t */
	rrm_oam_timer_311_et	       t311;			/* from ue_timers_and_constants_t */
	rrm_oam_timer_t320_et	       t320;
}rrc_timers_t;

/*! \struct rrc_constants_t 
 *  \brief This struct contains information about RRC constants. 
 */
typedef struct _rrc_constants_t 
{
    /* Coverity 69745,69746 Fix Start  */
	rrm_oam_out_of_sync_n310_et 	n310;		/* from ue_timers_and_constants_t */
	rrm_oam_in_sync_n311_et	n311;		/* from ue_timers_and_constants_t */
    /* Coverity 69745,69746 Fix End  */
}rrc_constants_t;

/*! \struct rrc_timers_and_constants_t 
 *  \brief This struct contains information about RRC timers and RRC constants. 
 */
#define RRMCM_RMIF_RRC_TIMERS_PRESENT   0x01
#define RRMCM_RMIF_RRC_CONSTANTS_PRESENT   0x02
typedef struct _rrc_timers_and_constants_t 
{
        rrm_bitmask_t   bitmask;
	rrc_timers_t	rrc_timers;
	rrc_constants_t	rrc_constants;
}rrc_timers_and_constants_t;

/*! \struct rf_configurations_t 
 *  \brief This struct contains parameters relating to the RF configuration. 
 */
#define RRMCM_RMIF_RF_CONFIGURATION_UL_EARFCN_PRESENT   0x01
#define RRMCM_RMIF_RF_CONFIGURATION_UL_BW_PRESENT       0x02
#define RRMCM_RMIF_MAX_RS_EPRE_PRESENT                  0x04
typedef struct _rf_configurations_t 
{
        rrm_bitmask_t			bitmask;
	U8				frequency_band_indicator;    /* sib_type_1_Info_t */
	U32				dl_earfcn;
	U16				ul_earfcn;
	rrm_oam_band_width_et 	        dl_bandwidth;	        /*mib_info_t*/
	rrm_oam_band_width_et	        ul_bandwidth;		/*freq_info_t*/ 
	S8				reference_signal_power;  /*int[-60..50]*/
	U16				phy_cell_id;		/* 0 to 503 */
	S16				psch_power_offset;	/*-350 to 150*/
	S16				ssch_power_offset;	/*-350 to 150*/
	S16				pbch_power_offset;	/*-350 to 150*/
        S16                             max_rs_epre;
}rf_configurations_t;

/*! \struct rf_params_t 
 *  \brief This struct contains paramters for RF configuration. 
 */
typedef struct _rf_params_t 
{
	rf_configurations_t	rf_configurations;
}rf_params_t;

/*! \struct common_eutran_params_t 
 *  \brief This struct contains parameter for common eutran.
 */
typedef struct _common_eutran_params_t 
{
	U8 cell_identity[MAX_CELL_IDENTITY_OCTETS];	/*from cell_context header*/
}common_eutran_params_t;

/*! \struct s1ap_params_t 
 *  \brief This struct contains parameters related with S1AP. 
 */
typedef struct s1ap_params
{
	U8	t_reloc_prep;	
	U8	t_reloc_overall;
}s1ap_params_t;

/* Rel 1.2 x2 HO support */
typedef enum
{
    X2_ALLOWED = 1, /*!< X2 allowed */
    X2_PROHIBITED, /*!< X2 prohibited */
    X2_ALLOWED_CONNECTED, /*!< X2 allowed and link is connected*/
    X2_ALLOWED_DISCONNECTED, /*!< X2 allowed and link is not connected*/
    X2_PROHIBITED_CONNECTED, /*!< X2 prohibited and link is connected*/
    X2_PROHIBITED_DISCONNECTED, /*!< X2 prohibited and link is not connected*/
    /* CA: SCell Selection start */
    X2_LOCALLY_CONNECTED_CELL, /*!< Cells belong to the same eNB */
    /* CA: SCell Selection end */
    X2_UNDEFINED = 255 /*!< X2 unavailable */
}rrmcm_rmif_cell_x2_status_et;
/*HO support for MLB*/
typedef enum
{
    HO_ALLOWED = 1,                    /*!< HO allowed */
    HO_PROHIBITED,                     /*!< HO prohibited */
    HO_UNDEFINED = 255                 /*!< HO unavailable */
}rrmcm_rmif_cell_ho_status_et;

/* Struct for nbr cells load for MLB Feature start Rel1.3 */

typedef enum
{
    RRM_X2AP_RSU_LOAD_LOW    =   1,     /* Low load */
    RRM_X2AP_RSU_LOAD_MEDIUM,           /* Medium load */
    RRM_X2AP_RSU_LOAD_HIGH,             /* High load */
    RRM_X2AP_RSU_LOAD_OVER              /* Over load */
}rrm_x2ap_rsu_load_ind_et;
/*S1 transport network layer load */
typedef struct _rrm_rs_tnl_load_t
{
    rrm_x2ap_rsu_load_ind_et dl;  /*^ M,0,H,0,3 ^*/ 
    rrm_x2ap_rsu_load_ind_et ul;  /*^ M,0,H,0,3 ^*/
}rrm_rs_tnl_load_t;

/*Physical resource block usage */
typedef struct _rrm_nbr_rsu_prb_t
{
    U8 dl_gbr_prb_usage;        /*DL GBR PRB usage:     M, 0, H, 0, 100 */
    U8 ul_gbr_prb_usage;        /*UL GBR PRB usage:     M, 0, H, 0, 100 */
    U8 dl_non_gbr_prb_usage;    /*DL non-GBR PRB usage: M, 0, H, 0, 100 */
    U8 ul_non_gbr_prb_usage;    /*UL non-GBR PRB usage: M, 0, H, 0, 100 */
    U8 dl_total_prb_usage;  /*DL Total PRB usage:   M, 0, H, 0, 100 */
    U8 ul_total_prb_usage;  /*UL Total PRB usage:   M, 0, H, 0, 100 */
}rrm_rs_prb_t;


/*Hardware load */
typedef struct _rrm_nbr_rsu_hw_load_t
{
    rrm_x2ap_rsu_load_ind_et dl; /*^ M,0,H,0,3 ^*/
    rrm_x2ap_rsu_load_ind_et ul; /*^ M,0,H,0,3 ^*/
}rrm_rs_hw_load_t;

#define RRM_MAX_AVLB_CAPCTY     100
/*Composite available capacity*/
typedef struct _rrm_nbr_rsu_composite_avl_cap_t
{
#define RRM_CELL_CAPACITY_CLASS_VALUE_PRESENT   0x01
    U8 bitmask;                    /*^ BITMASK ^*/
    U8 cell_cap_class_val;  /*^ O,RRM_CELL_CAPACITY_CLASS_VALUE_PRESENT,B,1,100 ^*/
    U8 cell_cap_val;        /*^ M,0,H,0,100 ^*/
}rrm_nbr_rsu_composite_avl_cap_t;

/*Composite available capacity group*/
typedef struct _rrm_rs_cacg_t
{
    rrm_nbr_rsu_composite_avl_cap_t dl; /*^ M,0,N,0,0 ^*/
    rrm_nbr_rsu_composite_avl_cap_t ul; /*^ M,0,N,0,0 ^*/
}rrm_rs_cacg_t;

/* eICIC_PHASE_1_2_CHANGES_START */\

typedef enum
{
 RRM_ABS_NONE,
 RRM_ABS_INCREASE,
 RRM_ABS_DECREASE,
 /*SPR 14523 start*/
 RRM_DL_ABS_STATUS_ZERO
 /*SPR 14523 end*/
}rrm_abs_change_type_et;

typedef enum
{
/* bug_13935_start */
  ABS_USAGE_NONE,
/* bug_13935_end */
  ABS_USAGE_STOP,
  ABS_USAGE_UPDATE,
  ABS_USAGE_START
}rrm_abs_usage_rep_status_et;
#define RRM_NUM_OF_ABS_REPORT_PRESENT               0x01
typedef struct  _rrm_x2ap_abs_status_t
{
   U8         bitmask;  /*^ BITMASK ^*/
   U8         dl_abs_status;
   U16        max_num_abs_report;
   rrm_abs_change_type_et abs_change_type;
   rrm_abs_usage_rep_status_et abs_usage_rep_status;
   U8         usable_abs_pattern[RRM_MAX_PATTERN_BYTE]; 
   U16        num_abs_report;
   U8         tdd_fdd_config;            
   }rrm_x2ap_abs_status_t;
/* eICIC_PHASE_1_2_CHANGES_END */

/*! \struct rrm_nbr_rsu_info_t 
 *  \brief This struct contains load info of neighbour cells.
 */

/** eICIC_PHASE_1_2_CHANGES_START */
#define RRM_TDD_FDD_CONFIG_PRESENT      0x01
/* ! \rrm_son_abs_status_t
 *  \brief  This struct pass ABS status configuration to SON.
 *  \param  bitmask bitmask related infortion.
 *  \param  dl_abs_status Percentage usage of ABS resources
 *  \param  tdd_fdd_config TDD configuration of the serving cell. 
 *  \param  usable_abs_pattern_info_tdd Usage pattern reported from neighbor.In case of serving 
 *          cell it will be the value presently being applied on the serving cell
 */
typedef struct _rrm_abs_status_t
{
    rrm_bitmask_t                       bitmask;                    /*^ BITMASK ^*/
    U8                                  dl_abs_status;              /*^ M, 0, H, 0, 100 ^*/
    U8                                  tdd_fdd_config;             /*^ O, RRM_SON_TDD_FDD_CONFIG_PRESENT, H, 0, 3 ^*/
    U8                                  usable_abs_pattern_info_tdd[RRM_MAX_PATTERN_BYTE];         
                                                                  /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_abs_status_t;
/** eICIC_PHASE_1_2_CHANGES_END */

typedef struct _rrm_rs_info_t 
{
#define RRM_RR_LOAD_PERIODIC_REPORT_PRESENT         0x01
#define RRM_TNL_LOAD_PERIODIC_REPORT_PRESENT        0x02
#define RRM_HW_LOAD_PERIODIC_REPORT_PRESENT         0x04
#define RRM_COMP_AVL_CAP_PERIODIC_REPORT_PRESENT    0x08
#define RRM_ABS_STATUS_INFO_PRESENT                 0x10
    /* SPR 21020 Fix Start */
#define RRM_MEM_LOAD_PERIODIC_REPORT_PRESENT        0x20
#define RRM_CPU_LOAD_PERIODIC_REPORT_PRESENT        0x40
    /* SPR 21020 Fix Stop */

    U8                                 bitmask;  /*^ BITMASK ^*/
    rrm_bool_t                         is_reported; /* Indicate if the status is reported or not */
    rrm_rs_prb_t                       rr_load;
    /*^ O,RRM_RR_LOAD_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    rrm_rs_tnl_load_t                  s1_tnl_load;
    /*^ O,RRM_TNL_LOAD_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    rrm_rs_hw_load_t                   hw_load;
    /*^ O,RRM_HW_LOAD_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    rrm_rs_cacg_t                      comp_avl_cap_grp;
    /*^ O,RRM_COMP_AVL_CAP_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    /* eICIC_PHASE_1_2_CHANGES_START */\
    rrm_x2ap_abs_status_t       abs_status;
    /*^ O,RRM_ABS_STATUS_INFO_PRESENT,N,0,0 ^*/
    /* eICIC_PHASE_1_2_CHANGES_END */
}rrm_rs_info_t;

typedef struct _rrm_rs_srv_cell_ld_info_t 
{
    U8                          bitmask; /*^ BITMASK ^*/
    rrm_rs_prb_t                rr_load; /* PRB Load */
    /*^ O,RRM_RR_LOAD_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    U8                          s1_tnl_load; /* TNL Load */
    U8                          hw_load; /* HW Load */
    rrm_x2ap_rsu_load_ind_et    tnl_ld_lvl;
    /*^ O,RRM_TNL_LOAD_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    rrm_x2ap_rsu_load_ind_et    hw_ld_lvl;
    /*^ O,RRM_HW_LOAD_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    rrm_rs_cacg_t               comp_avl_cap_grp; /* Composite Load */
    /*^ O,RRM_COMP_AVL_CAP_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    /* SPR 14465 start */
    rrm_x2ap_abs_status_t       abs_status;
    /*^ O,RRM_ABS_STATUS_INFO_PRESENT,N,0,0 ^*/
    /* SPR 14465 end */
    /* SPR 21020 Fix Start */
    U8                          mem_load; /* MEMORY Load */
    rrm_x2ap_rsu_load_ind_et    mem_ld_lvl;
    /*^ O,RRM_MEM_LOAD_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    U8                          cpu_load; /* CPU Load */
    rrm_x2ap_rsu_load_ind_et    cpu_ld_lvl;
    /*^ O,RRM_CPU_LOAD_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    /* SPR 21020 Fix Stop */
}rrm_rs_srv_cell_ld_info_t;

#define RRM_OFFSET_ATTR_OFC_PRESENT 0x01
#define RRM_OFFSET_ATTR_OCS_PRESENT 0x02
#define RRM_OFFSET_ATTR_OFN_PRESENT 0x04
#define RRM_OFFSET_ATTR_OFF_PRESENT 0x08
#define RRM_OFFSET_ATTR_HYS_PRESENT 0x10
typedef struct _rrm_cell_offset_attr_t
{
    rrm_bitmask_t             bitmask;
    /*^ BITMASK ^*/
    S8    ofs;
    /*^ O, RRM_OFFSET_ATTR_OFC_PRESENT, B, -24, 24 ^*/
    S8    ocs;
    /*^ O, RRM_OFFSET_ATTR_OCS_PRESENT, B, -24, 24 ^*/
    S8    ofn;
    /*^ O, RRM_OFFSET_ATTR_OFN_PRESENT, B, -24, 24 ^*/
    S8    off;
    /*^ O, RRM_OFFSET_ATTR_OFF_PRESENT, B, -24, 24 ^*/
    U8    hys;
    /*^ O, RRM_OFFSET_ATTR_HYS_PRESENT, H, 0, 30 ^*/
}rrm_cell_offset_attr_t;

#define RRM_EUTRAN_STRENGTH_RSRP_PRESENT  0x01
#define RRM_EUTRAN_STRENGTH_RSRQ_PRESENT  0x02
#define RRM_EUTRAN_STRENGTH_OFFSET_ATT_PRESENT   0x04
typedef struct _rrm_eutran_meas_strength_t
{
    rrm_bitmask_t          bitmask;/*^ BITMASK ^*/
    U8                     rsrp;
    /*^ O, RRM_EUTRAN_STRENGTH_RSRP_PRESENT, H, 0, 128 ^*/
    U16                    rsrq;
    /*^ O, RRM_EUTRAN_STRENGTH_RSRQ_PRESENT, H, 0, 34 ^*/
    rrm_cell_offset_attr_t offset_attr;
    /*^ O, RRM_EUTRAN_STRENGTH_OFFSET_ATT_PRESENT, N, 0, 0 ^*/
}rrm_eutran_meas_strength_t;

typedef struct _rrm_trgt_cell_meas_strength_t
{
#define RRM_EUTRAN_TRGT_CELL_STRENGTH   0x01
    U8                          choice;
    union
    {
      rrm_eutran_meas_strength_t  eutran_cell_strength; /* RRM_EUTRAN_TRGT_CELL_STRENGTH */
    }u;
}rrm_trgt_cell_meas_strength_t;

/*SPR 10329:start*/
typedef rrm_oam_gu_group_id_t rrm_gu_group_id_t;
typedef rrm_gu_group_id_t rrm_global_mme_group_id_key_t ;
typedef struct _rrm_global_enb_id_key_t
{
   rrm_enb_type_et        enb_type; /*^M,0,B,1,2^*/
   rrm_oam_cell_plmn_info_t plmn_id;
   U8                       eNb_id[HOME_ENB_ID_OCTET_SIZE];
}rrm_global_enb_id_key_t;
typedef YLIST rrm_gu_group_id_list_t;  /*rrm_gu_group_id_node_*/

typedef struct _rrm_enb_gu_group_id_node_t
{
    YLNODE                      s_node;
    rrm_gu_group_id_list_t      gu_group_id_list; /*rrm_gu_group_id_node_t */ 
    rrm_global_enb_id_key_t     enb_key_id;
}rrm_enb_gu_group_id_node_t;

typedef struct _rrm_gu_group_id_node_t
{
    YLNODE                          s_node;
    rrm_global_mme_group_id_key_t   mme_group_id_key;
}rrm_gu_group_id_node_t;
/*SPR 16477 Fix Start*/
typedef s1ap_served_gummei_info_t rrm_connected_mme_gummei_info_t;
typedef struct _rrm_connected_mme_node_t
{
    YLNODE                                  s_node;
    U8                                      mme_key_id;
    rrm_connected_mme_gummei_info_t         connected_gummei_info; 
}rrm_connected_mme_node_t;
/*SPR 16477 Fix Stop*/

/*SPR 10329:end*/

/* eICIC_PHASE_1_2_CHANGES_START */
/* bug_13898_start */  
typedef enum
{
 RRM_EICIC_ABS_NOT_SENT,
 RRM_SEND_ABS_PATTERN,
 RRM_EICIC_ABS_PATTERN ,
 RRM_EICIC_ABS_INACTIVE 
}rrm_abs_info_status_et;

typedef enum
{
 RRM_EICIC_INVOKE_NOT_SENT,
 RRM_EICIC_SEND_INVOKE,
 RRM_EICIC_INVOKE_SENT
}rrm_invoke_ind_status_et;

typedef enum
{
 RRM_EICIC_NO_ABS,
 RRM_EICIC_TEMP_ABS,
 RRM_EICIC_UPDATE_ABS,
 RRM_EICIC_CNFRMD_ABS,
 /*SPR 15485 start*/
 RRM_EICIC_INACTIVE_ABS
 /*SPR 15485 end*/

}rrm_abs_pattern_status_et;

/* bug_13898_end */  

/* bug_13935_start */  
#define RRM_CELLM_HIGHEST_AGGRESSOR_PRIORITY          32
/* bug_13935_end */


/*! \struct _rrm_intra_ncl_eicic_data_t 
 *  \brief This struct contains eICIC for intra frequency neighbour cells.
 *  \param bitmask indicates which all info is apllicable for particular neighbour
 *  \ RRMCM_INTRA_NCL_INVOKE_IND_SENT_PRESENT : invoke_ind_sent is valid for cell
 *  \ RRMCM_INTRA_NCL_ABS_PATTERN_ACCEPTED_PRESENT : abs_pattern_accepted is valid for cell
 *  \ RRMCM_INTRA_NCL_INVOKE_IND_SENT_PRESENT : abs_pattern_sent is valid for cell
 *  \param invoke_ind_sent indicates whether victim has sent 
 *  \the invoke to this agressor or not
 *  \param abs_pattern_sent indicates whether aggressor has sent 
 *  \the ABS pattern for this victim or not. If abs_pattern_sent is present then
 *  \abs_pattern_shared will always be there.
 *  \param abs_pattern_accepted indicates whether victim has included
 *  \this aggressor's abs pattern in common ABS pattern for L2 or not
 *  \param abs_pattern_shared[RRM_MAX_PATTERN_BYTE] indicates the abs pattern
 *  \shared with particular victim
 */
typedef struct _rrm_intra_ncl_eicic_data_t
{
#define RRMCM_INTRA_NCL_INVOKE_IND_STATUS_PRESENT          0x01
#define RRMCM_INTRA_NCL_ABS_PATTERN_ACCEPTED_PRESENT       0x02
#define RRMCM_INTRA_NCL_ABS_PATTERN_SENT_PRESENT           0x04
#define RRMCM_INTRA_NCL_ABS_PATTERN_RECVD_PRESENT          0x08
#define RRMCM_INTRA_NCL_MEAS_PATTERN_RECVD_PRESENT         0x10
#define RRMCM_INTRA_NCL_ABS_PATTERN_STATUS_PRESENT         0x20
#define RRMCM_INTRA_NCL_RSR_STATUS_PRESENT                 0x40
	rrm_bitmask_t                    bitmask;
/* bug_13935_start */  
//	rrm_oam_intf_scheme_type_et      interference_scheme;
/* 13935_end */  
	rrm_invoke_ind_status_et         invoke_ind_status;
	rrm_abs_pattern_status_et        abs_pattern_status;
	rrm_abs_info_status_et           abs_info_sent;
	U8                               temp_abs_pattern[RRM_MAX_PATTERN_BYTE];
	U8                               temp_meas_pattern[RRM_MAX_PATTERN_BYTE];
	U8                               abs_pattern_sent[RRM_MAX_PATTERN_BYTE];
	U8                               abs_pattern_recvd[RRM_MAX_PATTERN_BYTE];
	U8                               meas_pattern_recvd[RRM_MAX_PATTERN_BYTE];
/* bug_13935_start */  
        U8                                aggressor_priority; 
/* bug_13935_end */  
}rrm_intra_ncl_eicic_data_t;
/* eICIC_PHASE_1_2_CHANGES_END */


/* Structures for nbr cells load for MLB feature end Rel1.3 */

/* Rel 1.2 x2 HO support */

#define RRMCM_RMIF_INTRA_FREQ_GLOBAL_CELL_ID_PRESENT         0x01
#define RRMCM_RMIF_INTRA_FREQ_CELL_INDIVIDUAL_OFFSET_PRESENT 0x02
#define RRMCM_RMIF_INTRA_FREQ_R_STX_POWER_PRESENT            0x04
/*csg start*/
#define RRMCM_RMIF_INTRA_FREQ_CSG_ID_PRESENT                 0x08
#define RRMCM_RMIF_INTRA_FREQ_CELL_ACCESS_MODE_PRESENT       0x010
#define RRMCM_RMIF_INTRA_FREQ_TAC_PRESENT                    0x020
#define RRMCM_RMIF_INTRA_FREQ_Q_OFFSET_PRESENT               0x040
#define RRMCM_RMIF_EUTRAN_STRENGTH_RSRP_PRESENT              0x080
#define RRMCM_RMIF_EUTRAN_STRENGTH_RSRQ_PRESENT              0x100
/*csg end*/
#define RRMCM_RMIF_INTRA_FREQ_HO_STATUS_PRESENT              0x200
/** eICIC_PHASE_1_2_CHANGES_START */
#define RRMCM_RMIF_INTRA_FREQ_INTERFERENCE_SCHEME_PRESENT    0x400
/**13867_fix_start*/
#define RRMCM_RMIF_INTRA_FREQ_X2_STATUS_PRESENT              0x800
/**13867_fix_end*/
#ifdef LTE_EMBMS_SUPPORTED
#define RRMCM_RMIF_INTRA_MBSFN_CONFIG_LIST_PRESENT           0x1000
/*SPR 18605:Fix Start*/
#define RRMCM_RMIF_INTRA_SF_ASSN_INFO_PRESENT                0x2000
/*SPR 18605:Fix End*/
#endif
/** eICIC_PHASE_1_2_CHANGES_END */
/*! \struct intra_freq_cells_t 
 *  \brief This struct contains intra frequency cells related parameters. 
 */

typedef struct _intra_freq_cells_t 
{
    rrm_bitmask_t                       bitmask;
    rrm_oam_eutran_global_cell_id_t     cell_id;
    U16                                 phy_cell_id;
/* Coverity 88477-80 Fix Start*/
    rrm_q_offsetrange_et                q_offset;
    rrm_q_offsetrange_et                cio;
/* Coverity 88477-80 Fix End */
    /* Added for release 1.2 x2 HO support */
    rrmcm_rmif_cell_x2_status_et        x2_status;
    S8	              	                r_stx_power;
    rrm_bool_t                         broadcast_status;
    /* Bug_12689_fix: Start */
    rrm_bool_t                         is_pci_conflict; /* This flag marks the intra freq ncl if it has same pci as serving cell cell */
    /* Bug_12689_fix: End */
    rrm_bool_t                         x2_link_up_status;
    S32                               ho_status;  /*^ O, RRMCM_RMIF_INTRA_FREQ_HO_STATUS_PRESENT, N, 0, 0 ^*/
    S32			   blacklisted;
    /*csg start*/
    U8                                csg_identity[RRM_OAM_MAX_NUM_CSG_OCTETS];  /*^ O, RRMCM_RMIF_INTRA_FREQ_CSG_ID_PRESENT, N, 0, 0 ^*/
    S32                               cell_access_mode;   /*^ O, RRMCM_RMIF_INTRA_FREQ_CELL_ACCESS_MODE_PRESENT, N, 0, 0 ^*/
    /* Added in SPR 5193 */
    /*csg end*/
    /* For nbr load info MLB feature start Rel1.3 */
    U16                               rsu_meas_id; /* Store measId in case of invalid value it will be set to 0xFFFF */
    rrm_rs_info_t                     resrc_report;
    /* For nbr load info MLB feature end Rel1.3*/
    /*SPR 10329:start*/
    rrm_enb_gu_group_id_node_t       *p_enb_gu_group_id; /*Store enb_id node if requested enb_id exists in global_list*/
    /*SPR 10329:end*/
    /* eICIC_PHASE_1_2_CHANGES_START */
    /** eICIC_PHASE_1_2_CHANGES_START */
    rrm_oam_intf_scheme_type_et       interference_scheme;
    /** eICIC_PHASE_1_2_CHANGES_END */
    rrm_intra_ncl_eicic_data_t       rrm_intra_ncl_eicic_data;
    /* eICIC_PHASE_1_2_CHANGES_END */
    U8                                tac[MAX_TAC_SIZE];  /*^ O, RRMCM_RMIF_INTRA_FREQ_TAC_PRESENT, N, 0, 0 ^*/
#ifdef LTE_EMBMS_SUPPORTED
    rrmcm_mbsfn_subframe_config_list_t mbsfn_subframe_config_list;
/*SPR 18605:Fix Start*/
    U8                                 subframe_assn ;
/*SPR 18605:Fix End*/
#endif
}intra_freq_cells_t;


#define RRMCM_RMIF_INTER_FREQ_GLOBAL_CELL_ID_PRESENT         0x01
#define RRMCM_RMIF_INTER_FREQ_CELL_INDIVIDUAL_OFFSET_PRESENT 0x02
#define RRMCM_RMIF_INTER_FREQ_R_STX_POWER_PRESENT            0x04
/*csg start*/
#define RRMCM_RMIF_CELL_ACCESS_MODE_PRESENT    0x0008
#define RRMCM_RMIF_CSG_ID_PRESENT              0x0010
#define RRMCM_RMIF_INTER_FREQ_TAC_PRESENT      0x20
/*csg end*/
#define RRMCM_RMIF_INTER_FREQ_HO_STATUS_PRESENT 0x40
/*Changes start */
/*Changes end */

#ifdef LTE_EMBMS_SUPPORTED
#define RRMCM_RMIF_INTER_MBSFN_CONFIG_LIST_PRESENT 0x80

/*SPR 18605:Fix Start*/
#define RRMCM_RMIF_INTER_SF_ASSN_INFO_PRESENT      0x100
/*SPR 18605:Fix End*/
#endif

/*! \struct inter_freq_cells_t 
 *  \brief This struct contains inter frequency cells related parameters. 
 */
typedef struct _inter_freq_cells_t 
{
    rrm_bitmask_t                       bitmask;
    rrm_oam_eutran_global_cell_id_t     cell_id;
	U32			                        eutra_carrier_arfcn;
/* Coverity 88477-80 Fix Start*/
    rrm_q_offsetrange_et                q_offset; /* inter_freq_carrier_freq_t */
    rrm_q_offsetrange_et                cio;
/* Coverity 88477-80 Fix End */
	S32                                 blacklisted;
    /* Added for release 1.2 x2 HO support */
	rrmcm_rmif_cell_x2_status_et        x2_status;
	U16			                        phy_cell_id;
    rrm_bool_t                         broadcast_status;
    rrm_bool_t                         x2_link_up_status;
    S32                             ho_status;  /*^ O, RRMCM_RMIF_INTER_FREQ_HO_STATUS_PRESENT, N, 0, 0 ^*/
    /*csg start*/
    U8     csg_identity[RRM_OAM_MAX_NUM_CSG_OCTETS]; /*^ O, RRMCM_RMIF_CSG_ID_PRESENT, N, 0, 0 ^*/
    S32    cell_access_mode;   /*^ O, RRMCM_RMIF_CELL_ACCESS_MODE_PRESENT, N, 0, 0 ^*/
    /* Added in SPR 5193 */ 
    /*csg end*/
    /* For nbr load info MLB feature Rel1.3*/
    rrm_rs_info_t                     resrc_report;
    /* For nbr load info MLB feature end Rel1.3*/
    /*SPR 10329:start*/
    rrm_enb_gu_group_id_node_t       *p_enb_gu_group_id; /*Store enb_id node if requested enb_id exists in global_list*/
    /*SPR 10329:end*/
    U16                               rsu_meas_id; /* Store measId in case of invalid value it will be set to 0xFFFF */
    U8     tac[MAX_TAC_SIZE];  /*^ O, RRMCM_RMIF_INTER_FREQ_TAC_PRESENT, N, 0, 0 ^*/  
	S8			                        r_stx_power;
#ifdef LTE_EMBMS_SUPPORTED
    rrmcm_mbsfn_subframe_config_list_t mbsfn_subframe_config_list;

/*SPR 18605:Fix Start*/
    U8                                 subframe_assn ;
/*SPR 18605:Fix End*/
#endif
}inter_freq_cells_t;

/*! \struct lte_ncl_t 
 *  \brief This struct conatins parameters related with NCL. 
 */
typedef struct _lte_ncl_t 
{
	U8 num_valid_intra_freq_cell;
	U8 num_valid_inter_freq_cell;
	intra_freq_cells_t	intra_freq_cells[MAX_INTRA_FREQ_CELLS/*Currently its 16*/];
	inter_freq_cells_t	inter_freq_cells[MAX_INTER_FREQ_CELLS/*Currently its 48*/];
}lte_ncl_t;
typedef struct _lai_info
{
    plmn_identity_t  plmn_identity;
    U8               lac[RRM_OAM_MAX_NUM_LAC_OCTETS];
}lai_info_t;

typedef struct _rai
{
    lai_info_t   lai;
    U8           rac;
}rai_t;

typedef struct _utran_cell_id
{
#define RRM_UTRAN_EXTN_RNC_ID_PRESENT	0x01
    rrm_bitmask_t bitmask;
    U8        cell_id[RRM_UTRAN_CELL_IDENTITY_OCTETS];
    U16       rnc_id;
    U16       extended_rnc_id;
    /*Start: SPR 9289 */
    U8       cell_id_merge[RRM_MERGED_UTRAN_CELL_IDENTITY_OCTETS];
    /*End: SPR 9289 */
}utran_cell_id_t;
/*RIM changes start*/
#define RRM_UTRAN_SI_PRESENT   0x01
typedef struct _rrm_rim_utran_info
{
    rrm_bitmask_t   bitmask;
    U16                  utran_si_len; /*length of UTRAN SI recived in RIM Info*/
    U8                   utran_si[RRM_RIM_MAX_UTRA_SI_PAYLOAD];
}rrm_rim_utran_info_t;
/*RIM changes end*/
#define RRMCM_RMIF_PCI_FDD_PRESENT  0x01
#define RRMCM_RMIF_PCI_TDD_PRESENT  0x02

/*! \  rrmcm_rmif_utran_physical_cell_id_t
 *  \brief Physical Cell ID
 *  \param bitmask - Bitmask Value
 *  \param pci_fdd - PCI (FDD Mode)
 *  \param pci_tdd - PCI (TDD Mode)
 */
typedef struct _rrmcm_rmif_utran_physical_cell_id_t
{
    rrm_bitmask_t  bitmask;        /*^ BITMASK ^*/
    U16            pci_fdd;    /*^ O, RRMCM_RMIF_PCI_FDD_PRESENT, H, 0, 511 ^*/
    U8             pci_tdd;        /*^ O, RRMCM_RMIF_PCI_TDD_PRESENT, H, 0, 127 ^*/
}rrmcm_rmif_utran_physical_cell_id_t;

typedef struct _utran_freq_cells
{
#define RRM_UTRAN_FREQ_UARFCNUL_PRESENT             0x01
/*csg start*/
#define RRM_UTRAN_FREQ_CELL_ACCESS_MODE_PRESENT     0x02
#define RRM_UTRAN_FREQ_CSG_ID_PRESENT               0x04
/*csg end*/
/* SRVCC SUPPORT START */
#define RRM_UTRAN_HO_STATUS_PRESENT                 0x08
/* SRVCC SUPPORT END */
    rrm_bitmask_t   bitmask;
    rai_t            rai;
    S8               offset_freq;
    utran_cell_id_t  uc_id;
    U16                  pci;
    U16                  ura;
    U16                  uarfcnul;
    U16                  uarfcndl;
    U16                  pcpich_scrambling_code;
    S16                  pcpich_tx_power;
    /*csg start*/
    S32                  cell_access_mode; /*^ O, RRM_UTRAN_FREQ_CELL_ACCESS_MODE_PRESENT, N, 0, 0 ^*/
    /*csg end*/
    S32                  blacklisted; /*^ M, 0, N, 0, 0 ^*/
    /*csg start*/
    U8                   csg_identity[RRM_OAM_MAX_NUM_CSG_OCTETS];  /*^ O, RRM_UTRAN_FREQ_CSG_ID_PRESENT, N, 0, 0 ^*/
    /*csg end*/
    /*RIM changes start*/
    U32                  last_recd_rsn;/*Last recived seq no  received RAN Info*/
    rrm_rim_utran_info_t       *rim_utran_info;
    /*RIM changes end*/
	S32                  ho_status;  /*^ O, RRM_UTRAN_HO_STATUS_PRESENT, N, 0, 0 ^*/
    /* SRVCC SUPPORT START */
    rrm_bool_t          ps_ho_supported; /*^ M, 0, H, 0, 1 ^*/
    rrm_bool_t          voip_capable;  /*^ M, 0, H, 0, 1 ^*/
    /* SRVCC SUPPORT END */

}utran_freq_cells_t;

/*! \  rrmcm_rmif_geran_physical_cell_id_t
 *  \brief This is GERAN pci inforamtion
 *  \param ncc                  Network Colour Code def in 23.003
 *  \param bscc                 Base station Colour Code def in 23.003
 */
typedef struct _rrmcm_rmif_geran_physical_cell_id_t
{
    U8          ncc;
    /*^ M, 0, H, 0, 7 ^*/
    U8          bscc;
    /*^ M, 0, H, 0, 7 ^*/
}rrmcm_rmif_geran_physical_cell_id_t;

/*RIM changes start*/
typedef struct _rrm_rim_geran_si_psi_t
{
    U8         si_psi[RRM_RIM_MAX_SI_PSI_PAYLOAD_INDIV];
}rrm_rim_geran_si_psi_t;

#define RRM_GERAN_SI_PRESENT 0x01
#define RRM_GERAN_PSI_PRESENT 0x02
typedef struct _rrm_rim_geran_info
{
    rrm_bitmask_t       bitmask ;
    U8                   num_si_psi;
    rrm_rim_geran_si_psi_t     geran_si_psi[RRM_RIM_MAX_SI_PSI_PAYLOAD];
}rrm_rim_geran_info_t;
/*RIM changes end*/

/* GERAN-HO changes start */
/*! \struct geran_freq_ncl_t
 *  \brief This struct conatins parameters related to GERAN NCL. 
 */
#define RRM_GERAN_FREQ_CELL_CSG_ID_PRESENT 0x01 
#define RRM_GERAN_FREQ_CELL_ACCESS_MODE_PRESENT 0x02
#define RRM_GERAN_FREQ_CELL_NW_CONTROL_ORDER_PRESENT 0x04

/* bug 1013: Start */
#define RRM_GERAN_FREQ_HO_STATUS_PRESENT                    0x08
/* bug 1013: End */

/* SPR_14067_Fix_Start */
#define RRM_GERAN_FREQ_CELL_DTM_SUPPORTED_PRESENT 0x08
#define RRM_GERAN_FREQ_CELL_DTM_HO_SUPPORTED_PRESENT 0x10
#define RRM_GERAN_FREQ_CELL_VOIP_CAPABLE_PRESENT 0x20
/* SPR_14067_Fix_End */
/* SPR 15393 Fix Start */
#define RRM_GERAN_FREQ_CELL_RAC_PRESENT                 0x40
/* SPR 15393 Fix Stop */


typedef struct _geran_freq_ncl_t
{
	rrm_bitmask_t bitmask ;
    lai_info_t 	lai;
    U8          bsic;
    U16         cell_id;
    U8          band_indicator;
    U16         bcch_arfcn;
    U16         pci;
    S8          offset_freq;
    U8                     csg_id[RRM_OAM_MAX_NUM_CSG_OCTETS];  /*^ O, RRM_GERAN_FREQ_CELL_CSG_ID_PRESENT, N, 0, 0 ^*/
    S32                    cell_access_mode;    /*^ O, RRM_GERAN_FREQ_CELL_ACCESS_MODE_PRESENT, N, 0, 0 ^*/
    S32                  blacklisted; /*^ M, 0, N, 0, 0 ^*/
    /* SPR-10489-fix start */
    U8                   dtm_supported; /*^ O, RRM_GERAN_FREQ_CELL_DTM_SUPPORTED_PRESENT, H, 0, 1 ^*/
    /* SPR-10489-fix end */

    /* bug 1013: Start */
    S32                  ho_status; /*^ O, RRM_GERAN_FREQ_HO_STATUS_PRESENT, N, 0, 0 ^*/
    /* bug 1013: End */
    /*RIM changes start*/
    rrm_oam_nco_ind_et         nw_ctrl_order;  /*^ O, RRM_GERAN_FREQ_CELL_NW_CONTROL_ORDER_PRESENT, H, 0, 3 ^*/
    U32                 last_recd_rsn;/*Last recived seq no  received RAN Info*/
    rrm_rim_geran_info_t       *rim_geran_info;
    /*RIM changes end*/
    /* SPR 15393 Fix Start */
    U8                           rac;
    /*^ O, RRM_GERAN_FREQ_CELL_RAC_PRESENT, N, 0, 0 ^*/
    /* SPR 15393 Fix Stop */
    /* SPR_14067_Fix_Start */
    U8                   dtm_ho_supported;  /*^ O, RRM_GERAN_FREQ_CELL_DTM_HO_SUPPORTED_PRESENT, H, 0, 1 ^*/
    U8                   voip_capable;   /*^ O, RRM_GERAN_FREQ_CELL_VOIP_CAPABLE_PRESENT, H, 0, 1 ^*/
    /* SPR_14067_Fix_End */
}geran_freq_ncl_t;
/* GERAN-HO changes end */

typedef struct _rrmcm_rmif_cdma2000_freq_cells_t
{
    rrm_bitmask_t  presence_bitmask;  /*^ BITMASK ^*/
#define RRMCM_RMIF_CELL_SPECIFIC_PARAMS_PRESENCE_FLAG          0x01
#define RRMCM_RMIF_CDMA_CELL_DAHO_IND_PRESENT                  0x02
#define RRMCM_RMIF_CDMA_HO_STATUS_PRESENT                      0x04
    U16   arfcn; /*^ M, 0, H, 0, 2047 ^*/
    /* Corresponds to phys_cell_id_cdma2000 of Sib-8 */
    U16   pn_offset; /*^M, 0, OCTET_STRING ,VARIABLE ^*/
    rrm_oam_ncl_type_et    type; /*^M, 0, B, 0, 1^*/
    U8    band_class; /*^ M, 0, H, 0, 31 ^*/   /* rrm_oam_bandclass_cdma2000_et */
    U8    num_valid_count_cid; /*^M, 0, B, 6, 16^*/
    /* Corresponds to cell_global_id_cdma2000 of Sib-8 */
    U8    CID[RRMCM_RMIF_MAX_CGI_CDMA2000]; /*^M, 0, OCTET_STRING ,VARIABLE ^*/
    rrmcm_rmif_cell_specific_params_t             cell_specific_params;
    /*^ O, RRMCM_RMIF_CELL_SPECIFIC_PARAMS_PRESENCE_FLAG, N, 0, 0 ^*/
    S32                             ho_status;  /*^ O, RRMCM_RMIF_CDMA_HO_STATUS_PRESENT, N, 0, 0 ^*/
    U8   daho_indication; /*^ O, RRMCM_RMIF_CDMA_CELL_DAHO_IND_PRESENT, H, 0, 1 ^*/ 
}rrmcm_rmif_cdma2000_freq_cells_t;

/* CDMA2000 changes ends */

/*! \struct inter_rat_ncl_t 
 *  \brief This struct conatins parameters related with NCL. 
 */
typedef struct _inter_rat_ncl_t
{
        U8 num_valid_utran_freq_cell;
	    U8 num_valid_geran_cell;
        U8 num_valid_cdma2000_freq_cells;
        utran_freq_cells_t      utran_freq_cells[RRM_MAX_UTRAN_NBRS];
		/* GERAN-HO changes start */
	    geran_freq_ncl_t        geran_freq_cells[RRM_MAX_GERAN_NBRS];	
		/* GERAN-HO changes end */

        /* CDMA2000 changes start */
        rrmcm_rmif_cdma2000_freq_cells_t  cdma2000_freq_cells[RRMCM_RMIF_NUM_CDMA2000_FREQ_CELLS];
        /* CDMA2000 changes ends */
}inter_rat_ncl_t;

typedef struct _rrm_sps_config_dl_t
{
    rrm_bitmask_t     bitmask;
    U8                semi_persist_sched_interval_dl;  /* rrm_oam_semi_persist_sched_interval_et */
    U8                number_of_conf_sps_processes;  
    U8                max_sps_harq_retx;
    U8                explicit_release_after;
}rrm_sps_config_dl_t;

#define RRMCM_RMIF_P_ZERO_NOMINAL_PUSCH_PERSISTENT_PRESENT  0x01
/* + SPS_TDD_Changes */
#ifdef TDD_MODE_FLAG
#define RRMCM_RMIF_TWO_INTERVAL_CONFIG 0x02 
#endif
/* - SPS_TDD_Changes */
typedef struct _rrm_sps_config_ul_t
{
    rrm_bitmask_t     bitmask;
    U8                semi_persist_sched_interval_ul; /* rrm_oam_semi_persist_sched_interval_et */
    U8                implicit_release_after;         /* rrm_oam_implicit_release_after_et */
    S8                p_zero_nominal_pusch_persistent;
/* + SPS_TDD_Changes */
#ifdef TDD_MODE_FLAG
    U8                two_intervals_config;
#endif
/* - SPS_TDD_Changes */
}rrm_sps_config_ul_t;

#define RRMCM_RMIF_SPS_CONFIG_DL_PRESENT  0x01
#define RRMCM_RMIF_SPS_CONFIG_UL_PRESENT  0x02
typedef struct _rrm_sps_config_data
{
    rrm_bitmask_t                 bitmask; 
    rrm_sps_config_dl_t       sps_config_dl;  
    rrm_sps_config_ul_t       sps_config_ul;
}rrm_sps_config_data_t;

/*! \struct qos_config_params_t 
 *  \brief This struct contains parameters related to QOS.
 */

#define RRMCM_RMIF_PACKET_ERROR_LOSS_PRESENT 0x0001
#define RRMCM_RMIF_DSCP_PRESENT              0x0002
#define RRMCM_RMIF_LOSSLESS_HO_REQ_PRESNET   0x0004
#define RRMCM_RMIF_UE_INACTIVE_TIMER_PRESENT 0x0008
#define RRMCM_RMIF_SUPPORTED_RAT_PRESENT     0x0010
#define RRMCM_RMIF_SPS_CONFIG_ENABLED_PRESENT 0x0020
#define RRMCM_RMIF_SPS_CONFIG_DATA_PRESENT    0x0040
/* SPR_13815_fix: start */
/* Lines deleted */
/* SPR_13815_fix: end */
/* SRVCC SUPPORT START */
#define RRMCM_RMIF_SRVCC_OP_REQUIRED_PRESENT         0x0200
/* SRVCC SUPPORT END */
/*BUG_11564_CHANGES_START*/
#define RRMCM_RMIF_DL_FWDING_PRESENT               0x0400
/*BUG_11564_CHANGES_END*/
#ifdef ENDC_ENABLED  
#define RRMCM_RMIF_SGNB_BEARER_REQUIRED             0x1000
#define RRMCM_RMIF_SGNB_BEARER_TYPE_PRESENT         0x2000
#endif    
typedef struct _qos_config_params_t 
{
    rrm_bitmask_t                           bitmask;
	U8					qci;
	rrm_oam_qos_type_et 			type;
	U8					priority;
	rrm_oam_packet_delay_budget_et   	packet_delay_budget;
	U64					packet_error_loss_rate;//Not from OAM
	rrm_erb_service_profile_t erb_service_profile;
    U8                        dscp;
    S32                 lossless_ho_required;
    /*BUG_11564_CHANGES_START*/
    rrm_bool_et         dl_forwarding_applicability;
    /*BUG_11564_CHANGES_END*/
    U32                 ue_inactivity_timer_config;
    U8                  supported_rat;//For liminting UE meas
    rrm_bool_et         sps_config_enabled;
    rrm_sps_config_data_t  sps_data;
    rrm_bool_et         sr_mask_flag;
    /* SPR_13815_fix: start */
    /* Lines deleted */
    /* SPR_13815_fix: end */
    /* SRVCC SUPPORT START */
    rrm_bool_et         srvcc_op_required;
    /* SRVCC SUPPORT END */
#ifdef ENDC_ENABLED
	rrm_bool_et        sgnb_bearer_required;
    rrm_drb_type_et    sgnb_bearer_type;
#endif
}qos_config_params_t;

/* SPR 10730 Fix Start */
/*! \struct plmn_list_t 
 *  \brief This struct contains information of PLMN list.
 *  \brief This struct contains information of Cell Use.
 */
typedef struct _plmn_list_t 
{
    /* SPR 10730 Fix Start */
    rrm_bitmask_t   bitmask;
	rrm_cell_plmn_info_t				plmn_id;	
    #define RRMCM_RMIF_CELL_RESERVED_FOR_OPERATOR_USE_PRESENT    0x01
	rrm_oam_cell_reserved_for_operatoruse_et 	cell_reserved_for_operator_use;
    /* SPR 10730 Fix End */
    /* SPR 23209 FIXED START */
    #define RRMCM_RMIF_MOCN_OPERATOR_INDENTITY_PRESENT           0x02
    U8 mocn_operator_identity;
    /* SPR 23209 FIXED END */

}plmn_list_t;
/* SPR 10730 Fix End */

#define RRMCM_RMIF_EA_ID_PRESENT  0x01
/*! \struct general_epc_params_t 
 *  \brief This struct contains general EPC parameters.
 */
typedef struct _general_epc_params_t 
{
    rrm_bitmask_t   bitmask;
    U8 num_valid_plmn; /*1 to 6*/
    /* SPR 10730 Fix Start */
	plmn_list_t plmn_list[MAX_PLMN_LIST/*its 6 now*/];
    /* SPR 10730 Fix End */
    U8		tac[MAX_TAC_SIZE];	/*TAC_OCTET_SIZE defined in rrc_rrm_intf.h*/
    U8		eaid[MAX_EAID];
}general_epc_params_t;

/*! \struct epc_params_t 
 *  \brief This struct conatins parameters for EPC. 
 */
#define RRMCM_RMIF_GENERAL_EPC_PARAMS_PRESENT 0x0001
typedef struct _epc_params_t 
{
	rrm_bitmask_t           bitmask;
    general_epc_params_t	general_epc_params;
	U8 num_valid_qos_profiles;
	qos_config_params_t	qos_config_params[RRM_MAX_QCI];/*its 9 now*/
        U8                      emergency_erab_arp;
}epc_params_t;


#define RRMCM_RMIF_LTE_NCL_PRESENT  0x01
#define RRMCM_RMIF_INTER_RAT_NCL_PRESENT 0x02
/*! \ struct ncl_params_t 
 *  \brief This struct contains parameters for NCL.  
 */
typedef struct _ncl_params_t 
{
        rrm_bitmask_t bitmask;
        lte_ncl_t       lte_ncl;
        inter_rat_ncl_t inter_rat_ncl;
}ncl_params_t;


/* SPR 10730 Fix Start */
/* Code Deleted*/
/* SPR 10730 Fix End */
/* LTE_RRM_CELL_RECONFIG_START*/
/* SPR 21417 FIXED START*/
#define RRMCM_RMIF_ANTENNA_PORTS_COUNT_NO_PRESENT   0x0001
#define RRMCM_RMIF_PHY_LAYER_PARAMS_PRESENT         0x0002
#define RRMCM_RMIF_MAC_LAYER_PARAMS_PRESENT         0x0004
#define RRMCM_RMIF_RLC_LAYER_PARAMS_PRESENT         0x0008
#define RRMCM_RMIF_CELL_RESTRICTION_PARAMS_PRESENT  0x0010
#define RRMCM_RMIF_MOBILITY_LAYER_PARAMS_PRESENT    0x0020
#define RRMCM_RMIF_RRC_TIMERS_CONST_PARAMS_PRESENT  0x0040
#define RRMCM_RMIF_RF_PARAMS_PRESENT                0x0080
#define RRMCM_RMIF_COMMON_EUTRAN_PARAMS_PRESENT     0x0100
#define RRMCM_RMIF_S1AP_PARAMS_PRESENT              0x0200
#define RRMCM_RMIF_NCL_PARAMS_PRESENT               0x0400
#define RRMCM_RMIF_CONNECTED_MODE_MOBILITY_PARAMS_PRESENT  0x0800
/* SPR 21417 FIXED END*/
/* LTE_RRM_CELL_RECONFIG_END*/

/*! \ struct ran_t 
 *  \brief This struct contains information of RAN. 
 */
typedef struct _ran_t 
{
	rrm_bitmask_t 			bitmask;
	antenna_info_t			antenna_info; /*Not from OAM*/
	rrm_physical_layer_params_t	physical_layer_params;
	mac_layer_params_t		mac_layer_params;
	rlc_layer_params_t		rlc_layer_params;
	cell_restriction_params_t	cell_restriction_params;
	mobility_params_t		mobility_params;
	rrc_timers_and_constants_t	rrc_timers_and_constants;
	rf_params_t			rf_params;
	common_eutran_params_t	        common_eutran_params;
	s1ap_params_t			s1ap_params;
	ncl_params_t			ncl_params;
    rrm_connected_mode_mobility_params_t    connected_mode_mobility_params;
}ran_t;

/*! \struct epc_t 
 *  \brief This struct contains parameters of EPC.
 */
typedef struct _epc_t 
{
	epc_params_t	epc_params;
}epc_t;

/*CLPC start*/
/*! \struct rrm_mac_downlink_power_control_common_t 
 *  \brief This struct contains MAC downlink power control parameters. 
 */
typedef struct _rrm_mac_downlink_power_control_common_t
{
    U16 pcfichPowerOffset; /*^ M, 0, H, 0, 10000 ^*/

    U16 phichPowerOffset;  /*^ M, 0, H, 0, 10000 ^*/

    U16 pdcchPowerOffset; /*^ M, 0, H, 0, 10000 ^*/

    U16 pbchTransmissionPower; /*^ M, 0, H, 0, 10000 ^*/

    U16 pchTransmissionpower; /*^ M, 0, H, 0, 10000 ^*/

}rrm_mac_downlink_power_control_common_t;

/*! \struct rrm_mac_tpc_rnti_range_t 
 *  \brief This struct contains MAC TPC RNTI range 
 */
typedef struct _rrm_mac_tpc_rnti_range_t
{
    U16 startTpcRntiPucch; /*^ M, 0, B, 61, 65523 ^*/

    U16 endTpcRntiPucch; /*^ M, 0, B, 61, 65523 ^*/

    U16 startTpcRntiPusch; /*^ M, 0, B, 61, 65523 ^*/

    U16 endTpcRntiPusch; /*^ M, 0, B, 61, 65523 ^*/

}rrm_mac_tpc_rnti_range_t;

/*! \struct rrm_mac_power_control_enable_t 
 *  \brief This struct contians MAC power control enable parameters. 
 */
typedef struct _rrm_mac_power_control_enable_t
{
    U8 harqBlerClpcPucchEnable; /*^ M, 0, H, 0, 1 ^*/

    U8 cqiSinrClpcPucchEnable; /*^ M, 0, H, 0, 1 ^*/

    U8 clpcPuschEnable;       /*^ M, 0, H, 0, 1 ^*/

    U8 pdcchPowOrAggregationEnable;  /*^ M, 0, H, 0, 1 ^*/

    U8 clpcPuschfreqSelectiveEnable; /*^ M, 0, H, 0, 1 ^*/
     
    U8 tpc_pdcch_ConfigPUCCH_enabled;
    U8 tpc_pdcch_ConfigPUSCH_enabled;

}rrm_mac_power_control_enable_t;

/*! \struct rrm_recfg_phy_cell_parameters_t 
 *  \brief This struct contains parameters for reconfig phy. 
 */
typedef struct _rrm_recfg_phy_cell_parameters_t
{
    U16 pri_sync_sig_epre_eprers;
    /*^ O, RRC_RRM_PRIMARY_SYNC_SIGNAL_EPRE_EPRERS, H, 0, 10000 ^*/

    U16 sec_sync_sig_epre_eprers;
    /*^ O, RRC_RRM_SECONDARY_SYNC_SIGNAL_EPRE_EPRERS, H, 0, 10000 ^*/
}rrm_recfg_phy_cell_parameters_t;

/* CLPC Stop */

/*! \struct rrm_cell_parameters_t 
 *  \brief This struct contains parameters for cell parameters. 
 */
typedef struct _rrm_cell_parameters_t 
{
    /* SPR 21429 Fix Start */
#define RRMCM_RB_SIZE_PRESENT 0x01
#define RRMCM_MAX_RS_EPRE_PRESENT 0x02
    rrm_bitmask_t                    bitmask;
    /* SPR 21429 Fix End */
	U8	                             sub_carrier_spacing;
	U8	                             dl_cyclic_prefix;
	U8	                             rb_size;
    /* Additional parameters for Cell Config Added gur29990 STARTS*/
    S8                               max_rs_epre;
    /* Additional parameters for Cell Config Added gur29990 ENDS*/
}rrm_cell_parameters_t;

/*! \struct rrm_phy_phich_configuration_t 
 *  \brief This struct contains PHICH configuration parameters.
 */
typedef struct _rrm_phy_phich_configuration_t
{
    U8  phich_resource;     /*^ M, 0, H, 0, 3 ^*/ /* phich_resource_et */
    U8  phich_duration;     /*^ M, 0, H, 0, 1 ^*/ /* phich_duration_et */
} rrm_phy_phich_configuration_t;

/*! \struct rrm_mac_enable_frequency_selective_scheduling_t 
 *  \brief This struct contains mac enable frequency selective scheduling paramters. 
 */
typedef struct _rrm_mac_enable_frequency_selective_scheduling_t 
{
	rrm_frequency_selective_sched_et ul_freq_selective_enable;
	rrm_frequency_selective_sched_et	dl_freq_selective_enable;
}rrm_mac_enable_frequency_selective_scheduling_t;

/*CLPC start*/
#define RRMCM_RMIF_TPC_RNTI_RANGE_PRESENT	0x01
#define RRMCM_RMIF_POWER_CONTROL_ENABLE_PRESENT	0x02

    /* + SPR 17665 + */
#define RRMCM_RMIF_DCI_FORMAT_FOR_SI_MSG_PRESENT 0x04
#define RRMCM_RMIF_NUM_OF_RB_DCI_FORMAT_1A_PRESENT 0x08
#define RRMCM_RMIF_NUM_OF_RB_DCI_FORMAT_1C_PRESENT	0x10

    /* - SPR 17665 - */
/*CLPC stop*/

/*! \struct rrmc_mac_config_t 
 *  \brief This struct contains MAC config parameters. 
 */
typedef struct _rrmc_mac_config_t 
{
	rrm_bitmask_t bitmask;
	U8	max_harq_retrans;
    U8  dl_num_harq_process;
	U8	start_rarnti_range; 
	U8	end_rarnti_range;
    /*SPR_19066_START*/
    /* Code deleted */
    /*SPR_19066_END*/
	rrm_mac_enable_frequency_selective_scheduling_t	enable_frequency_selective_scheduling;
/*CLPC start*/
	rrm_mac_downlink_power_control_common_t	mac_downlink_power_control_common;
	rrm_mac_tpc_rnti_range_t		mac_tpc_rnti_range;
	rrm_mac_power_control_enable_t	mac_power_control_enable;
/*CLPC stop*/
    /* Additional parameters for Cell Config Added gur29990 STARTS*/
    rrm_dci_format_for_si_msg_et     rrm_dci_format_for_si_msg;
    /* + SPR 17665 + */
    U8                               num_rbs_per_tbs_dci_1a[RBS_FOR_DCI_1A_SIZE];
    U8                               num_rbs_per_tbs_dci_1c[RBS_FOR_DCI_1C_SIZE];
    /* - SPR 17665 - */

    /* BUG_371 FIX START */
    //rrm_ul_sync_loss_timer_et        ul_sync_loss_timer;
    //U8                               pucch_cqi_sinr_value;
    //rrm_n_gap_et                     n_gap;
    /* BUG_371 FIX END */
    /* Additional parameters for Cell Config Added gur29990 ENDS*/
    /* SPR 8505  FIX START */
    U32                              periodic_report_required;
    U32                              periodic_report_periodicity;
    U32                              periodic_report_ue_batch_size; 
    /* SPR 8505  FIX END */
}rrmc_mac_config_t;


/*! \struct rrm_sib_mapping_info_t 
 *  \brief This struct contains sib mapping information i.e count and sib type. 
 */
typedef struct _rrm_sib_mapping_info_t 
{
	U8	count;
	U8	sib_type[MAX_SI_MSG_NUM];
}rrm_sib_mapping_info_t;

/*! \struct rrm_scheduling_info_t 
 *  \brief This struct contains scheduling information i.e si periodicity and sib mapping information.
 */
typedef struct _rrm_scheduling_info_t 
{
	rrm_sib_mapping_info_t	sib_mapping_info;
	U8	si_periodicity;
}rrm_scheduling_info_t;

/*! \struct rrm_scheduling_info_list 
 *  \brief This struct contains information about scheduling information.  
 */
typedef struct rrm_scheduling_info_list
{
	U8	count;
	rrm_scheduling_info_t	scheduling_info[MAX_SCHEDULING_INFO_LIST];
}rrm_scheduling_info_list_t;

#define RRMCM_RMIF_Q_QUAL_MIN_OFFSET_R9_PRESENT 0X01
/*! \struct rrm_cell_selection_info_v920_t 
 *  \brief This struct contains cell selection information according to Rel9.  
 */
typedef struct _rrm_cell_selection_info_v920_t 
{
	rrm_bitmask_t bitmask; 
	S8 q_qual_min_r9;
	U8 q_qual_min_offset_r9_present;				
}rrm_cell_selection_info_v920_t;



#define RRMCM_RMIF_CELL_SELECTION_INFO_R9_PRESENT 0x01
/*! \struct rrm_sib_type_1_info_t 
 *  \brief This struct contains sib type 1 related parameters. 
 */
typedef struct _rrm_sib_type_1_info_t 
{
	rrm_bitmask_t bitmask;
  /* BUG_371 FIX START */
	//rrm_cell_access_related_infomation_t cell_access_info;
  /* BUG_371 FIX END */
    /* coverity : CID 29725,29640,29641*/
    rrm_si_window_length_et	si_window_length;
	rrm_scheduling_info_list_t	scheduling_info_list;
	rrm_bool_et ims_emergency_support_r9;
	rrm_cell_selection_info_v920_t cell_selection_info;
}rrm_sib_type_1_info_t;

/*! \struct rrm_bcch_config_t 
 *  \brief This struct contains BCCH config related parameters. 
 */
typedef struct _rrm_bcch_config_t 
{
	rrm_oam_modification_period_coeff_et	modification_period_coeff;
}rrm_bcch_config_t;

/*! \struct rrm_pcch_config_t 
 *  \brief This struct contains PCCH config related parameters. 
 */
typedef struct _rrm_pcch_config_t 
{
	rrm_oam_default_paging_cycle_et	        default_paging_cycle;
	rrm_oam_nB_et	                        nB;
}rrm_pcch_config_t;

/*! \struct _rrm_access_class_barring_info_t
 *  \brief This struct contains access class barring information.
 */
typedef struct rrm_access_class_barring_info
{
     rrm_mif_ac_barring_factor_et ac_barring_factor;
     rrm_mif_ac_barring_time_et   ac_barring_time;
     U8                           ac_barring_for_special_ac;
}rrm_access_class_barring_info_t;

/*! \struct rrm_ssac_barring_for_mmtel_r9_t 
 *  \brief this struct contains information of ssac barring info for voice and video according to rel9 
 */
#define RRMCM_RMIF_SSAC_BARRING_MMTEL_VOICE_PRESENT   0x01
#define RRMCM_RMIF_SSAC_BARRING_MMTEL_VIDEO_PRESENT   0x02
typedef struct _rrm_ssac_barring_for_mmtel_r9
{
     rrm_bitmask_t                    bitmask;
     rrm_access_class_barring_info_t  ssac_barring_for_mmtel_voice_r9;
     rrm_access_class_barring_info_t  ssac_barring_for_mmtel_video_r9;
}rrm_ssac_barring_for_mmtel_r9_t;


#define RRMCM_RMIF_AC_BARRING_MO_SIGNALLING_PRESENT   0x01
#define RRMCM_RMIF_AC_BARRING_MO_DATA_PRESENT         0x02
#define RRMCM_RMIF_SSAC_BARRING_FOR_MMTEL_PRESENT     0x04
/*! \struct rrm_access_barring_info_t 
 *  \brief This struct contains information of access class barring information for mo signalling and mo data. 
 */
typedef struct _rrm_access_barring_info_t 
{
  rrm_bitmask_t 		    bitmask;
  rrm_access_class_barring_info_t   ac_barring_for_mo_signalling;
  rrm_access_class_barring_info_t   ac_barring_for_mo_data;
  rrm_ssac_barring_for_mmtel_r9_t   ssac_barring_r9;
}rrm_access_barring_info_t;


/*! \struct rrm_radio_resource_config_common_sib_t 
 *  \brief This struct conatins information of radio resource configuration i.e BCCH and PCCH. 
 */
#define RRMCM_RMIF_BARRING_INFO_PRESENT            0x01
typedef struct _rrm_radio_resource_config_common_sib_t 
{
        rrm_bitmask_t                        bitmask;
	rrm_bcch_config_t	             rrm_bcch_config;
	rrm_pcch_config_t	             rrm_pcch_config;
	rrm_oam_ul_cyclic_prefix_length_et   ul_cyclic_prefix_length;
        rrm_access_barring_info_t            access_barring_info;
}rrm_radio_resource_config_common_sib_t;

#define RRMCM_RMIF_UL_CARRIER_FREQ_PRESENT  0x01
/*! \struct rrm_freq_info_t 
 *  \brief This struct contains frequency information i.e uplink carrier frequency and additional spectrum emission. 
 */
typedef struct _rrm_freq_info_t 
{
	rrm_bitmask_t bitmask;
	U16	ul_carrier_freq;
	U8	additional_spectrum_emission;
}rrm_freq_info_t;

 
/*! \struct rrm_sib_type_2_info_t 
 *  \brief This struct contains information about sib type 2. 
 */
typedef struct _rrm_sib_type_2_info
{
   rrm_bitmask_t              presence_bitmask; /*^ BITMASK ^*/
#define RRMCM_RMIF_AC_BARRING_PARAM_CSFB_R10_PRESENCE_FLAG  0x01
   rrm_radio_resource_config_common_sib_t  radio_res_config_common_sib;
   rrm_freq_info_t                         rrm_freq_info;
   U8	                                   time_alignment_timer;
   /* CDMA2000 changes start */
   rrm_access_class_barring_info_t         ac_barring_for_csfb_r10;
   /* CDMA2000 changes ends */
}rrm_sib_type_2_info_t;

#define RRMCM_RMIF_MEASUREMENT_BW_PRESENT 0x01
/*! \struct rrm_intra_freq_cell_reselection_info_t 
 *  \brief This struct contains intra frequency cell reslection information.
 */
typedef struct _rrm_intra_freq_cell_reselection_info
{
	rrm_bitmask_t                           bitmask;
	rrm_oam_measurement_bandwidth_et	measurement_bandwidth;
	U8	                                presence_antenna_port1;
}rrm_intra_freq_cell_reselection_info_t;

/*! \struct s_intra_search_v920_t 
 *  \brief This struct contains param for intra search according to rel9. 
 */
typedef struct s_intra_search_v920
{
	U8 s_intra_search_p_r9;
	U8 s_intra_search_q_r9;	
}s_intra_search_v920_t;

/*! \struct s_non_intra_search_v920_t 
 *  \brief This struct contains param for non intra search param according to rel9. 
 */
typedef struct s_non_intra_search_v920
{
	U8 s_non_intra_search_p_r9;
	U8 s_non_intra_search_q_r9;	
}s_non_intra_search_v920_t;

#define RRMCM_RMIF_S_INTRA_SEARCH_V920_PRESENT        0x01
#define RRMCM_RMIF_S_NON_INTRA_SEARCH_V920_PRESENT    0x02
#define RRMCM_RMIF_Q_QUAL_MIN_R9_PRESENT1              0x04
#define RRMCM_RMIF_THRESHOLD_SERVING_LOW_PRESENT      0x08
/*! \struct rrm_sib_type_3_info_t 
 *  \brief This struct contains parameters related to sib type 3. 
 */
typedef struct rrm_sib_type_3_info
{
	rrm_bitmask_t                             bitmask;
	rrm_intra_freq_cell_reselection_info_t	  intra_freq_reselection_info;
	s_intra_search_v920_t                     s_intra_search;
	s_non_intra_search_v920_t                 s_non_intra_search;
	S8 	                                  q_qual_min_r9;
	U8	                                  thresh_serving_lowq_r9;
}rrm_sib_type_3_info_t;


#define RRMCM_RMIF_CELL_ID_RANGE_PRESENT        0x01
/*! \struct csg_cell_id_range_t 
 *  \brief This struct contains csg cell id range related parameters.
 */
typedef struct csg_cell_id_range
{
	rrm_bitmask_t             bitmask;
	U16	                  start;
	rrm_oam_cell_id_range_et  range;
}csg_cell_id_range_t;

/*! \struct rrm_sib_type_4_info_t 
 *  \brief This struct contains sib type 4 related parameters. 
 */
typedef struct rrm_sib_type_4_info
{
	csg_cell_id_range_t csg_id_range;
}rrm_sib_type_4_info_t;

/*! \struct uplink_snr_dep_params_t 
 *  \brief This struct contains information about uplink snr parameters. 
 */
typedef struct _uplink_snr_dep_params_t
{
	U8 ul_mcs; /*Values as per 36.213 Table 8.6.1-1*/	
	rrm_bitmask_t bitmask;
	#define PUSCH_INFO_SNR_MAP_PRESENT 0x0001
	#define UPLINK_POW_CONTROL_INFO_SNR_MAP_PRESENT 0x0002
	rrc_phy_pusch_configuration_dedicated_ue_service_profile_t pusch_config_dedicated;
        rrc_phy_uplink_power_control_dedicated_ue_service_profile_t uplink_pow_control;
        /* SPR 21429 Fix Start */
	#define UL_PRB_PER_BPS_PRESENT 0x0004
        /* SPR 21429 Fix End */
    float ul_prb_per_bps;
}uplink_snr_dep_params_t;

/*! \struct downlink_snr_dep_params_t 
 *  \brief This struct contains inforamtion about downlink snr parameters. 
 */
typedef struct _downlink_snr_dep_params_t
{
	U8 dl_mcs; /*Values as per 36.213 Table 7.1.7.1-1*/
	rrm_bitmask_t bitmask;
    /* SPR 21429 Fix Start */
#define DL_PRB_PER_BPS_PRESENT    0x01
    /* SPR 21429 Fix End */
	float dl_prb_per_bps;
}downlink_snr_dep_params_t;

/*! \struct snr_dep_params_t 
 *  \brief This struct contains information about SNR dep related parameters. 
 */
typedef struct _snr_dep_params_t
{
	uplink_snr_dep_params_t uplink_snr_dep_params;
	downlink_snr_dep_params_t downlink_snr_dep_params;
}snr_dep_params_t;

/*! \struct snr_map_t 
 *  \brief This struct contains information about SNR map related parameters.
 */
typedef struct _snr_map
{
	snr_dep_params_t snr_dep_params;
}snr_map_t;


/*GBR LIMIT START */

/*! \struct avail_gbr_limit_t 
 *  \brief This struct contains available amount of GBR limit in uplink and downlink.  
 */
typedef struct _avail_gbr_limit_t
{
   
   U16            dl_gbr_limit;
   U16            ul_gbr_limit;

}avail_gbr_limit_t;

/*GBR LIMIT END */

typedef struct _tnl_capacity_t
{
   U64             total_backhaul_capacity;   /*!< Total backhaul capacity */
   U8              capacity_threshold;        /*!< Capacity threshold      */
}tnl_capacity_t;

/*! \struct admission_control_info_t 
 *  \brief This struct conatins information related to admission control.  
 */
/* SPR 21429 Fix Start */
#define RRMCM_SRB_BIT_RATE_PRESENT        0x01
#define RRMCM_UE_SERVICE_PROFILE_PRESENT  0x02
#define RRMCM_MAX_NUM_UE_PER_CELL_PRESENT 0x04
#define RRMCM_MAX_NUM_GBR_DRBS_PER_UE_PRESENT 	0x08
#define RRMCM_MAX_NUM_NGBR_DRBS_PER_UE_PRESENT 	0x10
#define RRMCM_NGBR_THRESHOLD_PRESENT            0x20
#define RRMCM_MIN_BITRATE_PRESENT               0x40
#define RRMCM_NON_DEFAULT_BEARER_MULTIPLIER_PRESENT 0x80
#define RRMCM_MAX_TOTAL_DL_GBR_PRB_BUDGET 		0x0100
#define RRMCM_MAX_TOTAL_UL_GBR_PRB_BUDGET 		0x0200
#define RRMCM_MAX_TOTAL_DL_NGBR_PRB_BUDGET 	        0x0400
#define RRMCM_MAX_TOTAL_UL_NGBR_PRB_BUDGET 	        0x0800
#define RRMCM_DL_BW_FOR_CONTROL_PRBS              0x1000 
#define RRMCM_AVAILABLE_GBR_LIMIT_PRESENT         0x2000
#define RRMCM_RB_FACTOR_FOR_BITRATE                 0x4000
#define RRMCM_MIN_SR_PERIODICITY_PRESENT            0x8000
#define RRMCM_MIN_RI_PERIODICITY_PRESENT            0x010000
#define RRMCM_MIN_CQI_PERIODICITY_PRESENT           0x020000
#define RRMCM_UE_SINR_THRESHOLD_PRESENT             0x040000 
#define RRMCM_SRS_PERIODICITY_PRESENT               0x080000
#define RRMCM_SNR_MAP_PRESENT               0x200000
/* SPR 21429 Fix End */
 
typedef struct _admission_control_info_t 
{
        rrm_bitmask_t                bitmask;
	U64		             srb_bit_rate; 
    /* SPR 20653 Fix Start */ 
	U8		             min_sr_periodicity;
	U8 		             min_cqi_periodicity;
	U8 		             min_ri_periodicity;
    U16                  max_eicic_ues; 
    /* SPR 20653 Fix End */
	U8 		             srs_periodicity;
	rrc_rrm_ue_service_profile_t ue_service_profile;
        U8                           cpu_utilization_limit;
        /* +- SPR 18268 */
        U16                           max_num_ue_per_cell;
        /* +- SPR 18268 */
        U16                          max_sps_ues;
        U8                           max_num_gbr_drbs_per_ue;
        U8                           max_num_ngbr_drbs_per_ue;
	snr_map_t                    snr_map;
	U8                           dl_prb_budget_gbr;
	U8                           ul_prb_budget_gbr;
	U8                           dl_prb_budget_ngbr;
	U8                           ul_prb_budget_ngbr;
	U8                           ngbr_threshold;		//float from OAM. MIF to multiply by 10 and provide
    U64                          minimum_bitrate;
	U8                           non_default_bearer_multiplier;   //float from OAM. MIF to multiply by 10 and provide
    U8 dl_total_bw_prbs;
    U8 ul_total_bw_prbs;
    U8 dl_bw_for_control_prbs;
    /*SPR 18654 START*/
    /*code deleted*/
    /*SPR 18654 END*/
	U8                           ue_sinr_threshold;
    /** TNL feature */
    tnl_capacity_t               tnl_capacity;
 
 /*GBR LIMIT START */
    avail_gbr_limit_t        avail_gbr_limit;

/*GBR LIMIT END */      
    /*SPR 18654 START*/
    U16                            rb_factor_for_bitrate;
    /*SPR 18654 END*/

    /** TNL feature */
/* SPR 21429 Fix Start */
#define RRMCM_RMIF_RESOURCE_RESERVED_PRESENT 0x400000
    U16                             resource_reserved_for_existing_users;
#define RRMCM_RMIF_PROXIMITY_IND_STATUS_PRESENT 0x800000
/* SPR 21429 Fix End */
    rrm_bool_et proximity_indication_status;
}admission_control_info_t;

#define MAX_BUCKET_SIZE 3


/*! \struct rrm_bucket_t 
 *  \brief This struct contains information of bucket size. 
 */
typedef struct rrm_bucket
{
    U8  value;
    U8  max;
    U8  min;
} rrm_bucket_t;

/*! \struct rrm_priority_t 
 *  \brief This struct contains information about priority. 
 */
typedef  struct  rrm_priority
{
    U8 count;
    rrm_bucket_t bucket[MAX_BUCKET_SIZE];
} rrm_priority_t ;

/*! \typedef struct rrm_priority_cfg_t 
 *  \brief This struct contains information about priority cfg. 
 */
typedef  struct  rrm_priority_cfg
{
    rrm_priority_t  high_priority_cfg;
    rrm_priority_t    medium_priority_cfg;
    rrm_priority_t     low_priority_cfg;
   /* SPR 13205 Fix Start */
    rrm_priority_t     low_2_priority_cfg;
   /* SPR 13205 Fix End */
} rrm_priority_cfg_t;


#define MAX_FREQ  8
#define MAX_FREQ_UTRAN_FDD  16
#define MAX_FREQ_GERAN  16
#define MAX_FREQ_CDMA  32 
/*! \struct rrm_freq_priority_info_t 
 *  \brief This struct contains frequency priority information. 
 */
typedef struct rrm_freq_priority_info
{
    U32     carrier_freq;
    U8      cell_reselection_priority;
} rrm_freq_priority_info_t;


/*! \struct rrm_eutran_freq_priority_list_info_t 
 *  \brief This struct contains eutran frequency priority list information.
 */
typedef struct rrm_eutran_freq_priority_list_info
{
    U8 count;
    rrm_freq_priority_info_t  eutran_freq_priority_info[MAX_FREQ];
}rrm_eutran_freq_priority_list_info_t;

/*! \struct rrm_utran_freq_priority_list_info_t 
 *  \brief This struct contains utran frequnecy priority list information.
 */
typedef struct rrm_utran_freq_priority_list_info
{
    U8 count;
    rrm_freq_priority_info_t  utran_freq_priority_info[MAX_FREQ_UTRAN_FDD];
}rrm_utran_freq_priority_list_info_t;

/*! \struct rrm_freq_priority_geran_t 
 *  \brief This struct contains geran carrier frequnecy and priority information.
 */
typedef struct rrm_freq_priority_geran
{
    U8 cell_reselection_priority;
    rrm_son_geran_carrier_freqs_t  carrier_freq;
}rrm_freq_priority_geran_t;



/*! \struct rrm_geran_freq_priority_list_info_t 
 *  \brief This struct contains geran frequnecy priority list information.
 */
typedef struct rrm_geran_freq_priority_list_info
{
    U8 count;
    rrm_freq_priority_geran_t  geran_freq_priority_info[MAX_FREQ_GERAN];
}rrm_geran_freq_priority_list_info_t;

/*! \struct rrm_freq_priority_cdma_t 
 *  \brief This struct contains CDMA frequnecy priority information.
 */
typedef struct rrm_freq_priority_cdma
{
    U8  type;
    U8  band_class;
    U16 arfcn;
    U8  cell_reselection_priority;
}rrm_freq_priority_cdma_t;

/*! \struct rrm_cdma_freq_priority_list_info_t 
 *  \brief This struct contains CDMA frequnecy priority list information.
 */
typedef struct rrm_cdma_freq_priority_list_info
{
    U8 count;
    rrm_freq_priority_cdma_t  cdma_freq_priority_info[RRMCM_RMIF_NUM_CDMA2000_FREQ_CELLS];
}rrm_cdma_freq_priority_list_info_t;

/*! \struct rrm_freq_priority_list_t 
 *  \brief This struct contains frequency priorirty list information for UTRAN and EUTRAN.
 */
typedef struct rrm_freq_priority_list
{
    rrm_eutran_freq_priority_list_info_t  eutran_freq_priority_list_info;
    rrm_utran_freq_priority_list_info_t   utran_freq_priority_list_info;
    rrm_geran_freq_priority_list_info_t   geran_freq_priority_list_info;
    rrm_cdma_freq_priority_list_info_t    cdma_freq_priority_list_info;
} rrm_freq_priority_list_t;

/*! \struct rrm_utran_daho_cell_config_info
 *  \brief This struct contains frequency priorirty list information for UTRAN 
 */
typedef struct _rrm_lai_utran_info
{
  rrm_cell_plmn_info_t     plmn_id;
  U8                       lac[RRM_OAM_MAX_NUM_LAC_OCTETS];
}rrm_utran_lai_info_t;

typedef struct _rrm_utran_rai_info
{
    rrm_lai_info_t             lai;
    U8                         rac;
}rrm_utran_rai_info_t;

typedef struct _rrm_utran_daho_cell_id
{
    #define RRM_OAM_EXTENDED_RNC_ID      0x01
    rrm_bitmask_t                     bitmask; /*^ BITMASK ^*/
    /* CP:Fix for UTRAN Cell ID 2 bytes START */
    U8       cell_id[RRM_UTRAN_CELL_IDENTITY_OCTETS];
    /* CP:Fix for UTRAN Cell ID 2 bytes STOP */
    U16      rnc_id;
    U16      extended_rnc_id;
} rrm_utran_daho_cell_id_t;

typedef struct _rrm_utran_daho_cell_config_info_t{
    rrm_utran_rai_info_t      rai;
    rrm_utran_daho_cell_id_t  uc_id;
    U16                  pci;
    U16                  ura;
    U16                  uarfcnul;
    U16                  uarfcndl;
    U16                  pcpich_scrambling_code;
    S16                  pcpich_tx_power;
    /* SRVCC SUPPORT START */
    rrm_bool_et          ps_ho_supported;
    rrm_bool_et          voip_capable;
    /* SRVCC SUPPORT END */
}rrm_utran_daho_cell_config_info_t;

/*! \struct rrm_eutran_daho_cell_config_info
 *  \brief This struct contains frequency priorirty list information for  EUTRAN.
 */
 typedef struct rrm_eutran_daho_cell_config_info
{
    rrm_cell_plmn_info_t plmn_id;
    U8 cell_identity[RRM_MAX_NUM_CELL_IDENTITY_OCTETS];
    U16                               phy_cell_id;
    rrm_q_offset_range_et             q_offset;
    rrm_q_offset_range_et             cell_individual_offset;
    S8                                r_stx_power;
    rrm_bool_et                       blacklisted;
    U8                                tac[MAX_TAC_SIZE];
}rrm_eutran_daho_cell_config_info_t;


/* SPR 10498 Fix Start */
/*! \struct rrm_eutran_inter_freq_daho_cell_config_info
 *  \brief This struct contains the inter-freq DAHO cell configuration.
 */
typedef struct rrm_eutran_inter_freq_daho_cell_config_info
{
    rrm_oam_eutran_global_cell_id_t   cell_id;
    U16                               earfcn;
    U16                               phy_cell_id;
    rrm_q_offset_range_et             q_offset;
    rrm_q_offset_range_et             cell_individual_offset;
    S8                                r_stx_power;
    U8                                csg_identity[RRM_OAM_MAX_NUM_CSG_OCTETS];
    U8                                tac[MAX_TAC_SIZE];
}rrm_eutran_inter_freq_daho_cell_config_info_t;

/*! \struct rrm_eutran_inter_freq_daho_cells_config_info_list_t
 *  \brief This struct contains the list of DAHO cells for Inter-frequencies
 */ 
typedef struct _rrm_eutran_inter_freq_daho_cells_config_info_list_t
{
    U8                  count;
    rrm_eutran_inter_freq_daho_cell_config_info_t  
                        rrm_eutran_inter_freq_daho_cell_config_info[MAX_NO_INTER_FREQ];
}rrm_eutran_inter_freq_daho_cells_config_info_list_t;
/* SPR 10498 Fix End */

/* GERAN-HO changes start */
typedef struct _rrm_geran_daho_cell_config_info_t{

	lai_info_t 	lai;
	U8			bsic;
	U16			cell_id;
	U8			band_indicator;
	U16			bcch_arfcn;
    U16			pci;
    /* SPR_14067_Fix_Start */
    U8          dtm_ho_supported;
    U8          dtm_supported;
    U8          voip_capable;
    /* SPR_13721_fix : end */
    /* SPR 19327 Fix Start */
    U8          rac;
    /* SPR 19327 Fix Stop*/    
}rrm_geran_daho_cell_config_info_t;
/* GERAN-HO changes end */

/*! \struct rrm_daho_cell_config_info
 *  \brief This struct contains frequency priorirty list information for UTRAN and EUTRAN.
 *  \param eutran_daho_cell_config_info
 *  \param utran_daho_cell_config_info
 *  \param geran_daho_cell_config_info
*/
typedef  struct _rrm_daho_cell_config_info
{
    rrm_bitmask_t bitmask;
#define EUTRA_DAHO_CELL_PRESENT 0x01
#define UTRAN_DAHO_CELL_PRESENT 0x02
#define GERAN_DAHO_CELL_PRESENT 0x04
/* SPR 10498 Fix Start */
#define EUTRA_INTER_FREQ_DAHO_CELL_PRESENT 0x08
/* SPR 10498 Fix End */
    rrm_eutran_daho_cell_config_info_t eutran_daho_cell_config_info;
    /* SPR 10498 Fix Start */
    rrm_eutran_inter_freq_daho_cells_config_info_list_t 
                                       eutran_inter_freq_daho_cells_config_info_list;
    /* SPR 10498 Fix End */
    rrm_utran_daho_cell_config_info_t  utran_daho_cell_config_info;
	/* GERAN-HO changes start */
	rrm_geran_daho_cell_config_info_t  geran_daho_cell_config_info;
	/* GERAN-HO changes end */
}rrm_daho_cell_config_info_t;
typedef struct _rrm_cellm_radio_congestion_threshold_t
{
    /* SPR 22420 Fix Start */
    U16 sampling_normal_time; 
    /* SPR 22420 Fix End */
}rrm_cellm_radio_congestion_threshold_t;

/* SPR 20653 Fix Start */
/* Code removed */
/* SPR 20653 Fix End */
#define RRM_IRAT_TYPE 4

typedef struct _rrm_rat_priority_info_cfg_t
{
    rrm_irat_priority_et  irat_priority_list_cfg[RRM_IRAT_TYPE];
}rrm_rat_priority_info_cfg_t;

#define  RRMCM_RMIF_EUTRAN_GAP_OFFSET_TYPE_PRESENT     0x01
#define  RRMCM_RMIF_UTRAN_GAP_OFFSET_TYPE_PRESENT      0x02
#define  RRMCM_RMIF_GERAN_GAP_OFFSET_TYPE_PRESENT      0x04
#define  RRMCM_RMIF_CDMA2000_GAP_OFFSET_TYPE_PRESENT   0x08
/* ENDC_MEAS_CHANGES_START */
#define  RRMCM_RMIF_ENDC_GAP_OFFSET_TYPE_PRESENT       0x10
/* ENDC_MEAS_CHANGES_END */
typedef struct _rrm_meas_gap_config_t
{
    rrm_bitmask_t  bitmask; 
    U8             eutran_gap_offset_type;
    U8             utran_gap_offset_type;
    U8             geran_gap_offset_type;
    U8             cdma2000_gap_offset_type;
/* ENDC_MEAS_CHANGES_START */
#ifdef ENDC_ENABLED
    U8             endc_gap_offset_type;
#endif
/* ENDC_MEAS_CHANGES_END */
}rrm_meas_gap_config_t;

#define  RRMCM_RMIF_UTRAN_CSFB_TGT_SELECTION_PRESENT      0x01
#define  RRMCM_RMIF_GERAN_CSFB_TGT_SELECTION_PRESENT      0x02
#define  RRMCM_RMIF_CDMA2000_CSFB_TGT_SELECTION_PRESENT   0x04
typedef struct _rrm_cellm_csfb_tgt_selection_t
{
    rrm_bitmask_t  bitmask; /*^ BITMASK ^*/
    rrm_csfb_target_selection_et    utran_csfb_tgt_selection;  /*^ O, RRMCM_RMIF_UTRAN_CSFB_TGT_SELECTION_PRESENT, N, 0, 0^*/
    rrm_csfb_target_selection_et    geran_csfb_tgt_selection;  /*^ O, RRMCM_RMIF_GERAN_CSFB_TGT_SELECTION_PRESENT, N, 0, 0^*/
    rrm_csfb_target_selection_et    cdma2000_csfb_tgt_selection;  /*^ O, RRMCM_RMIF_CDMA2000_CSFB_TGT_SELECTION_PRESENT, N, 0, 0^*/
}rrm_cellm_csfb_tgt_selection_t;

/* SPR 14060 FIX : code removed */
#define RRMCM_RMIF_MEAS_GAP_CONFIG_PRESENT      0x01
#define RRMCM_RMIF_SI_GAP_ENABLE_PRESENT        0x02
#define RRMCM_RMIF_CSFB_TGT_SELECTION_PRESENT   0x04
typedef struct _rrm_meas_config_info_t
{
    rrm_bitmask_t   bitmask; 
/* SPR 14060 FIX : code removed */
    rrm_meas_gap_config_t  meas_gap_config; 
    S32             si_gap_enable;
    rrm_cellm_csfb_tgt_selection_t   csfb_tgt_selection;
}rrm_meas_config_info_t;

typedef struct _rrm_sps_crnti_range_t
{
    U16     start_sps_crnti_range;
    U16     end_sps_crnti_range;
}rrm_sps_crnti_range_t;

/* HO config params start */
#define  RRMCM_RMIF_NEIGHBOUR_CELL_LOAD_HO_ENABLE_PRESENT      0x0001
#define  RRMCM_RMIF_UE_HISTORY_BASED_HO_ENABLE_PRESENT         0x0002
#define  RRMCM_RMIF_SPID_BASED_HO_ENABLE_PRESENT               0x0004
#define  RRMCM_RMIF_UE_MEAS_BASED_HO_ENABLE_PRESENT            0x0008
#define  RRMCM_RMIF_DAHO_CELL_BASED_HO_ENABLE_PRESENT          0x0010
typedef struct _rrm_target_cell_selection_params_t
{
    rrm_bitmask_t  bitmask; /*^ BITMASK ^*/
    U8             neighboring_cell_load_based_ho_enable; /*^ O,RRMCM_RMIF_NEIGHBOUR_CELL_LOAD_HO_ENABLE_PRESENT , N, 0, 0^*/
    U8             ue_history_based_ho_enable; /*^ O,RRMCM_RMIF_UE_HISTORY_BASED_HO_ENABLE_PRESENT , N, 0, 0^*/
    U8             spid_based_ho_enable;  /*^ O,RRMCM_RMIF_SPID_BASED_HO_ENABLE_PRESENT , N, 0, 0^*/
    U8             ue_measurement_based_ho_enable;  /*^ O,RRMCM_RMIF_UE_MEAS_BASED_HO_ENABLE_PRESENT , N, 0, 0^*/
    U8             daho_cell_based_ho_enable; /*^ O,RRMCM_RMIF_DAHO_CELL_BASED_HO_ENABLE_PRESENT , N, 0, 0^*/
}rrm_target_cell_selection_params_t;

/* SPR 22311 Changes - Starts */
/* Code Deleted */
#define  RRMCM_RMIF_HO_RETRY_COUNT_PRESENT   0x0001
typedef struct _rrm_ho_retry_params_t
{
  rrm_bitmask_t  bitmask; /*^ BITMASK ^*/
  U8             ho_retry_count; /*^ O,RRMCM_RMIF_HO_RETRY_COUNT_PRESENT , N, 0, 0^*/
}rrm_ho_retry_params_t;

#define RRMCM_RMIF_TARGET_CELL_SELECTION_PARAMS_PRESENT 0x01
#define RRMCM_RMIF_HO_RETRY_PARAMS_PRESENT              0x02
#define RRMCM_RMIF_BLIND_HO_TIMER_PRESENT               0x04
/* SPR 20635 Fix - SRVCC to GERAN - Starts */
#define RRMCM_RMIF_UE_MEAS_DURATION_HO_PRESENT          0x08
/* SPR 20635 Fix - SRVCC to GERAN - Ends */
typedef struct _rrm_ho_config_params_t
{
    rrm_bitmask_t           bitmask;    /*^ BITMASK ^*/
    rrm_target_cell_selection_params_t  target_cell_selection_params;
    /*^ O,RRMCM_RMIF_TARGET_CELL_SELECTION_PARAMS_PRESENT , N, 0, 0^*/
    rrm_ho_retry_params_t   ho_retry_params;
    /*^ O,RRMCM_RMIF_HO_RETRY_PARAMS_PRESENT , N, 0, 0^*/
    U16                     blind_ho_timer;
    /*^ O,RRMCM_RMIF_BLIND_HO_TIMER_PRESENT , N, 0, 0^*/
    /* SPR 20635 Fix - SRVCC to GERAN - Starts */
    U8                      ue_meas_duration_for_ho;
    /*^ O,RRMCM_RMIF_UE_MEAS_DURATION_HO_PRESENT , N, 0, 0^*/
    /* SPR 20635 Fix - SRVCC to GERAN - Ends */
}rrm_ho_config_params_t;
/* HO config params end */
/* SPR 22311 Changes - Ends */

/* MLB START*/

/*! \struct rrmcm_rmif_watermark_t
 *  \brief  Struct contains watermark that needs to be taken under various load
 *  condition
 *  \param  high_watermark indicates upper threshold limit
 *  \param  low_watermark indicates lower threshold limit
 */
typedef struct _rrmcm_rmif_watermark_t
{
    U8                  high_watermark; /*^ M, 0, B, 1, 100 ^*/
    U8                  low_watermark; /*^ M, 0, B, 1, 100 ^*/
}rrmcm_rmif_watermark_t;

/* Bug_8718_start */
/*! \rrmcm_rmif_stop_adm_info_t
 *  \brief This Struct contains information regarding stop admission action.
 *  \param bitmask to check optional field is present or not
 *  \param soft_limit indicates the relaxed limit over stop admission load
 *  percentage limit in which RRM will allow handin calls but not the normal
 *  priority initiating calls in the cell
 */
#define RRMCM_RMIF_SOFT_LIMIT_PRESENT 0x01
typedef struct _rrmcm_rmif_stop_adm_info_t
{
    rrm_bitmask_t   bitmask;        /*^ BITMASK ^*/
    U8              soft_limit;   /*^ O, RRMCM_RMIF_SOFT_LIMIT_PRESENT, B, 5,10 ^*/
}rrmcm_rmif_stop_adm_info_t;
/* Bug_8718_end */

/* Bug_944_start */
/*! \rrmcm_rmif_redirect_info_t
 *  \brief This Struct contains MARD information that needs to be taken
 *  redirection of UE on load based scenario.
 *  \param bitmask to check optional field is present or not
 *  \param mard_present to check mard is present or not
 *  \param rwr_timer indicates measurement based rwr timer value 
 *  the duration for which RRM will wait for the measurement report 
 *  to select the suitable frequency.
 */
#define RRMCM_RMIF_MARD_PRESENT 0x01
#define RRMCM_RMIF_MEAS_BASED_RWR_TIMER_PRESENT 0x02

typedef struct _rrmcm_rmif_redirect_info_t
{
    rrm_bitmask_t   bitmask;    /*^ BITMASK ^*/
    rrm_bool_t      mard_present;    /*^ O, RRMCM_RMIF_MARD_PRESENT, B, 0,1 ^*/
    U8              rwr_timer;      /*^ O, RRMCM_RMIF_MEAS_BASED_RWR_TIMER_PRESENT, B, 5,120 ^*/
}rrmcm_rmif_redirect_info_t;
/* Bug_944_end */

/** eICIC_PHASE_1_2_CHANGES_START */
#define RRMCM_RMIF_ELIGIBLE_UE_PRESENT         0x01
#define RRMCM_RMIF_ABS_PATTERN_PRESENT         0x02
/* ! \rrmcm_rmif_eicic_load_config_info_t
 * \brief Contains information about the load configuration to be applied for eICIC in case of load.
 * \param   bitmask To check optional field is present or not
 * \param   eligible_ue Percentage of UEs to be selected for the eICIC out of total 
 *          UEs experiencing interferance.
 * \param   abs_pattern Used for providing ABS pattern to the victim node in case node 
 *          is acting as aggressor node.
 */
typedef struct _rrmcm_rmif_eicic_load_config_info_t
{
    rrm_bitmask_t   bitmask;            /*^ BITMASK ^*/
    U8              eligible_ue;        /*^ O, RRMCM_RMIF_ELIGIBLE_UE_PRESENT, B, 1, 100 ^*/
    U8              abs_pattern[RRM_MAX_PATTERN_BYTE];
                                        /*^ O, RRMCM_RMIF_ABS_PATTERN_PRESENT, OCTET_STRING, FIXED ^*/
}rrmcm_rmif_eicic_load_config_info_t;
/** eICIC_PHASE_1_2_CHANGES_END */


/*! \struct rrmcm_rmif_action_t
 *  \brief  Struct contains action that needs to be taken under various load
 *  condition
 *  \param  load_perctg indicates the percentage which defines the particular
 *  load
 *  \param  action indicates the action that needs to be taken
 *  \param  num_usr indicates the number of users that are being impacted due to
 *  the action. This value is valid only when action chosen is UE RELESE or
 *  HANDOVER
 *  \param  q_watermark indicates the threshold limit
 *  \param mard_info indicate mard information that need to be taken for
 *  redirection of UE on load based scenario.
 */

#define RRMCM_RMIF_PERCNTG_PRESENT              0x01
#define RRMCM_RMIF_ACTION_PRESENT               0x02
#define RRMCM_RMIF_NUM_USR_PRESENT              0x04
#define RRMCM_RMIF_Q_WATERMARK_PRESENT          0x08
#define RRMCM_RMIF_LD_AC_BARRING_PRESENT        0x10
/* Bug_944_start */
#define RRMCM_RMIF_REDIRECT_INFO_PRESENT        0x20
/* Bug_944_end */
/* Bug_8718_start */
#define RRMCM_RMIF_STOP_ADM_INFO_PRESENT        0x40
/* Bug_8718_end */
/** eICIC_PHASE_1_2_CHANGES_START */
#define RRMCM_RMIF_EICIC_LOAD_CONFIG_PRESENT    0x80
/** eICIC_PHASE_1_2_CHANGES_START */
typedef struct _rrmcm_rmif_action_t
{
    rrm_bitmask_t               bitmask;            /*^ BITMASK ^*/
    U8                          load_perctg;        /*^ O, RRMCM_RMIF_PERCNTG_PRESENT, B, 1, 100 ^*/
    U16                         action;             /*^ O, RRMCM_RMIF_ACTION_PRESENT, N, 0, 0 ^*/
    U8                          num_usr;            /*^ O, RRMCM_RMIF_NUM_USR_PRESENT, L, 1, 256 ^*/
    rrmcm_rmif_watermark_t      q_watermark;        /*^ O, RRMCM_RMIF_Q_WATERMARK_PRESENT, N, 0, 0 ^*/
    rrm_access_barring_info_t   ld_ac_bar;          /*^ O, RRMCM_RMIF_LD_AC_BARRING_PRESENT, N, 0, 0 ^*/ /* Added for MLB Ph2 */
    /* Bug_8718_start */
    rrmcm_rmif_stop_adm_info_t  stop_adm_info;      /*^ O, RRMCM_RMIF_STOP_ADM_INFO_PRESENT, N, 0, 0 ^*/
    /* Bug_8718_end */
    /* Bug_944_start */
    rrmcm_rmif_redirect_info_t   mard_info;         /*^ O, RRMCM_RMIF_MARD_INFO_PRESENT, N, 0, 0, ^*/
    /* Bug_944_end */
    /** eICIC_PHASE_1_2_CHANGES_START */
    rrmcm_rmif_eicic_load_config_info_t    
                                eicic_load_config;  
                                                    /*^ O, RRMCM_RMIF_EICIC_LOAD_CONFIG_PRESENT, N, 0, 0 ^*/
    /** eICIC_PHASE_1_2_CHANGES_END */
}rrmcm_rmif_action_t;

/*! \enum rrm_cellm_resrc_type_et
* \brief cellm resource type
*/
typedef enum 
{
    RRM_CELLM_HW_RESRC,
    RRM_CELLM_TNL_RESRC,
    RRM_CELLM_NONGRB_RESRC,
    RRM_CELLM_GBR_RESRC,
    RRM_CELLM_MAX_RESRC
}rrm_cellm_resrc_type_et;

/*! \struct rrmcm_rmif_resrc_info_t
 *  \brief  Struct contains resource information that needs
 *  to be taken under various load condition
 *  \param  resrc_type indicates the type of the resource
 *  for which the load information needs to be changed
 *  \param  overload Contain information about the overload
 *  \param  highload Contain information about the high load
 *  \param  midload Contain information about the mid load
 */
#define RRMCM_RMIF_ACTION_OVERLD_PRESENT  0x01
#define RRMCM_RMIF_ACTION_HIGHLD_PRESENT  0x02
#define RRMCM_RMIF_ACTION_MIDLD_PRESENT  0x04
typedef struct _rrmcm_rmif_resrc_info_t
{
    rrm_bitmask_t             bitmask;        /*^ BITMASK ^*/
    /* coverity : CID 29634*/
    rrm_cellm_resrc_type_et   resrc_type;     /*^ M, 0, N, 0, 0 ^*/
    rrmcm_rmif_action_t       overload;       /*^ O, RRMCM_RMIF_ACTION_OVERLD_PRESENT, N, 0, 0 ^*/
    rrmcm_rmif_action_t       highload;       /*^ O, RRMCM_RMIF_ACTION_HIGHLD_PRESENT, N, 0, 0 ^*/
    rrmcm_rmif_action_t       midload;        /*^ O, RRMCM_RMIF_ACTION_MIDLD_PRESENT, N, 0, 0 ^*/
}rrmcm_rmif_resrc_info_t;

/*! \struct  rrmcm_rmif_resource_load_info_t
 *  \brief  Struct contains resource information that needs
 *  to be taken under various load condition
 *  \param  count indicates the number of the resource
 *  \param  resrc_info indicates the resource information
 */
typedef struct _rrmcm_rmif_resource_load_info_t
{
    rrm_bitmask_t         bitmask;                                /*^ BITMASK ^*/
    U8                    count;                                  /*^ O, 0, B, 1, 4 ^*/
    rrmcm_rmif_resrc_info_t  resrc_info[RRM_OAM_MAX_RESOURCE];    /*^ O, 0, N, 0, 0 ^*/
}rrmcm_rmif_resource_load_info_t;

#define RRMCM_RMIF_OVER_LOAD_LVL_ACT_PRESENT  0x01
#define RRMCM_RMIF_HIGH_LOAD_LVL_ACT_PRESENT  0x02
#define RRMCM_RMIF_MID_LOAD_LVL_ACT_PRESENT   0x04
#define RRMCM_RMIF_RESRC_SPEC_PRESENT         0x08
typedef struct _rrmcm_rmif_srv_cell_info_t
{
    rrm_bitmask_t                       bitmask;            /*^ BITMASK ^*/
    U8                                  cellindex;
    rrmcm_rmif_action_t                 over_load_lvl_act;  /*^ O, RRMCM_RMIF_OVER_LOAD_LVL_ACT_PRESENT, N, 0, 0 ^*/
    rrmcm_rmif_action_t                 high_load_lvl_act;  /*^ O, RRMCM_RMIF_HIGH_LOAD_LVL_ACT_PRESENT, N, 0, 0 ^*/
    rrmcm_rmif_action_t                 mid_load_lvl_act;   /*^ O, RRMCM_RMIF_MID_LOAD_LVL_ACT_PRESENT, N, 0, 0 ^*/
    rrmcm_rmif_resource_load_info_t     resrc_spec;         /*^ O, RRMCM_RMIF_RESRC_SPEC_PRESENT, N, 0, 0 ^*/
}rrmcm_rmif_srv_cell_info_t;

/*! \struct rrmcm_rmif_load_config_req_t
 *  \brief  This Struct gives load config params
 */
#define RRMCM_RMIF_NCL_LOAD_IND_INTRVL     0x01
#define RRMCM_RMIF_LOAD_RPT_INTRVL         0x02
 /* Bug_8718_start */
#define RRMCM_RMIF_LOAD_CHK_PRDCTY_PRESENT         0x04
#define RRMCM_RMIF_HIGH_ACT_RPT_LMT_PRESENT        0x08
 /* Bug_8718_end */
/* SPR 20653 Fix Start */
#define RRMCM_RMIF_TNL_LOAD_SUPPORT_FLAG_PRESENT   0x10
#define RRMCM_RMIF_HW_LOAD_SUPPORT_FLAG_PRESENT    0x20
/* SPR 20653 Fix End */
typedef struct _rrmcm_rmif_load_config_req_t
{
    rrm_bitmask_t                         bitmask;                        /*^ M, 0, N, 0, 0 ^*/
    U8                                    ncl_load_ind_intrvl;
    U16                                   load_rpt_intrvl;
    U16                                   transaction_id;                 /*!< Transaction Id */
    U16                                   num_of_cell;                    /*!< Number of cells */
    rrmcm_rmif_srv_cell_info_t               srv_cell_info[RRM_MAX_NUM_CELLS];   /*^ O, 0, N, 0, 0 ^*/
    /* Bug_8718_start */
    U16                           load_chk_periodicity; /*^ O, RRMCM_RMIF_LOAD_CHK_PRDCTY_PRESENT, B, 5, 600 ^*/
    U8                            high_actn_repeat_lmt; /*^ O, RRMCM_RMIF_HIGH_ACT_RPT_LMT_PRESENT, B, 5, 10 ^*/
    /* Bug_8718_end */
    /* SPR 20653 Fix Start */
    /* coverity_35216 Start */
    rrm_bool_t                      tnl_load_support_flag; /*^ O, RRMCM_RMIF_TNL_LOAD_SUPPORT_FLAG_PRESENT, H, 0, 1 ^*/
    rrm_bool_t                      hw_load_support_flag;  /*^ O, RRMCM_RMIF_HW_LOAD_SUPPORT_FLAG_PRESENT, H, 0, 1 ^*/
    /* coverity_35216 Stop */
    /* SPR 20653 Fix End */
}rrmcm_rmif_load_config_req_t; /*^ API, RRMCM_RMIF_LOAD_CONFIG_REQ ^*/

/* MLB END*/
/*KPI: start*/
typedef rrm_oam_kpi_t rrmcm_rmif_kpi_t;
typedef struct _rrmcm_kpi_data_t
{
  rrmcm_rmif_kpi_t kpi_bit;
  U32 num_of_admitted_csg_user; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_admitted_non_csg_user; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_ue_admission_success; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_ue_admission_fail; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_erb_setup_success; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_erb_setup_fail; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_erb_modify_success; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_erb_modify_fail; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_erb_release_success; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_erb_release_fail; /*^ M, 0, N, -1, 0 ^*/
  U32 avg_dl_allocated_gbr_prb; /*^ M, 0, N, -1, 0 ^*/
  U32 avg_ul_allocated_gbr_prb; /*^ M, 0, N, -1, 0 ^*/
  U32 avg_dl_allocated_ngbr_prb; /*^ M, 0, N, -1, 0 ^*/
  U32 avg_ul_allocated_ngbr_prb; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_geran_ho_attempt; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_geran_ho_fail; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_utran_ho_attempt; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_utran_ho_fail; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_eutran_ho_attempt; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_eutran_ho_fail; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_geran_hi_success; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_geran_hi_fail; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_utran_hi_success; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_utran_hi_fail; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_eutran_hi_success; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_eutran_hi_fail; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_enb_init_ho_csg_usr; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_enb_init_ho_non_csg_usr; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_enb_init_ue_release; /*^ M, 0, N, -1, 0 ^*/
  U32 num_pucch_res_alloc_attempts; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_sr_res_alloc_fail; /*^ M, 0, N, -1, 0 ^*/
  U32 num_of_sr_cqi_alloc_fail; /*^ M, 0, N, -1, 0 ^*/
  /* BUG_857 start*/
  U32 cell_unavailable_time;    /*^ M, 0, N, -1, 0 ^*/
  /* BUG_857 end*/
  /*CA STAGE 2*/
  U32 num_of_ca_ue;
}rrmcm_kpi_data_t;
typedef struct _rrmcm_kpi_ind_t
{
  rrm_cell_index_t cell_index;
  rrmcm_kpi_data_t kpi_data;
}rrmcm_kpi_ind_t; 

#define RRMCM_KPI_CONFIG_DURATION_PRESENT 0x01
typedef struct _rrmcm_rmif_kpi_config_req_t
{
    rrm_bitmask_t  bitmask;
    rrm_cell_index_t cell_index;
    U16 trans_id;
    U16 duration;
    U8  periodic_reporting;
    rrmcm_rmif_kpi_t kpi_to_report;
}rrmcm_rmif_kpi_config_req_t;

typedef struct _rrmcm_rmif_get_kpi_req_t
{
    rrm_cell_index_t cell_index;
    U16 trans_id;
    rrm_bool_et reset;
    rrmcm_rmif_kpi_t kpi_to_report;
}rrmcm_rmif_get_kpi_req_t;
typedef struct _rrmcm_rmif_get_kpi_resp_t
{
  rrm_cell_index_t cell_index;
  U16 trans_id;
  U8 response;
  U16 fail_cause;
  rrmcm_kpi_data_t kpi;
}rrmcm_rmif_get_kpi_resp_t;

/*KPI: end*/
typedef struct _rrm_load_congestion_cfg_t
{
    U8                        soft_lmt;
    U8                        high_actn_applied_lmt;
    U16                       ld_process_timer;
    rrmcm_rmif_action_t       overload;
    rrmcm_rmif_action_t       highload;
    rrmcm_rmif_action_t       midload;
}rrm_load_congestion_cfg_t;

/* HO oscillation start */
typedef struct _rrm_ho_oscillation_cfg_t 
{
    /* SPR 21429 Fix Start */
#define RRMCM_TIME_OF_STAY_THRESHOLD_PRESENT 0x01
#define RRMCM_MIN_NUM_OF_OSCILLATION_LOOP_PRESENT 0x02
#define RRMCM_MAX_LAST_VISITED_CELLS_SEARCHED_PRESENT 0x04
#define RRMCM_HO_OSCILLATION_CIO_STEP_PRESENT 0x08
    rrm_bitmask_t             bitmask;
    /* SPR 21429 Fix End */
    U8                        time_of_stay_threshold;
    U8                        min_num_of_oscillation_loop;
    U8                        max_last_visited_cells_searched;
    /* SPR-20552 START */
    rrm_oam_q_offset_range_et ho_oscillation_cio_step;
    /* SPR-20552 END */
}rrm_ho_oscillation_cfg_t;
/* HO oscillation end */


/*  DYNAMIC ICIC CHANGES START  */
#define RRMCM_RMIF_MAX_CC_REGION 2
#define RRMCM_RMIF_MAX_NUM_TPC_FOR_PUCCH 4
#define RRMCM_RMIF_MAX_DELTA_SINR_TO_TCP_MAP_PUSCH 4
#define RRMCM_RMIF_MAX_AGGREGATION_LEVEL_POWER_OFFSET_COUNT 4
#define RRMCM_RMIF_NUM_DCI_SIZE_CATEGORY 3
#define RRMCM_RMIF_MAX_CQI_VAL 15
#define RRMCM_RMIF_MAX_SPS_INTERVALS 16
#define RRMCM_RMIF_MAX_POSSIBLE_AGGREGAITION_LEVELS 4
#define RRMCM_RMIF_MAX_QPSK_MCS_NUM 4
#define RRMCM_RMIF_MAX_NUM_PATHLOSS_TO_SINR_COUNT  10 
#define RRMCM_RMIF_MAX_NUM_ALPHA_COUNT 8 
#define RRMCM_RMIF_MAX_MCS  32
#define RRMCM_RMIF_MAX_REDUCED_MSC 32
typedef struct _resource_partition_t
{
    U32 start_rb;  
    U32 num_of_rb;  
}resource_partition_t;

#define RRMCM_RMIF_RESOURCE_PARTITION_CELL_EDGE_PRESENT 0x01
typedef struct _rrm_resource_partition_info_t
{
    U32  bitmask;
    U32  num_of_cell_center_region;  
    U32  num_of_cell_edge_region;  
    resource_partition_t cell_center_region[RRMCM_RMIF_MAX_CC_REGION];  
    resource_partition_t cell_edge_region;  /*^ O, RESOURCE_PARTITION_CELL_EDGE_PRESENT, N, 0, 0^*/
}rrm_resource_partition_info_t;


typedef struct _rrm_ul_power_mask_t
{
    S32 cell_center_user_power_mask;
    S32 cell_edge_user_power_mask;
    S32 qci_delta_power_mask[RRM_MAX_QCI];
}rrm_ul_power_mask_t;

#define RRMCM_RMIF_MAX_NOMINAL_EPRE_PRESENT        0x01
typedef  struct _rrm_rntp_report_config_info_t
{
    rrm_bitmask_t  bitmask;
    U32 rntp_report_on_X2_required; 
    U32 rntp_threshold; 
    S32 max_nominal_epre;
}rrm_rntp_report_config_info_t;

typedef struct _l2_rrm_layer2_meas_report_config_info_t
{
    U32 l2_icic_report_perodicity;
    U32 cell_edge_prb_usage_required;
}l2_rrm_layer2_meas_report_config_info_t;

typedef struct _l2_rrm_layer1_meas_report_config_info_t
{
    U32 layer1_meas_report_required;
    U32 layer1_meas_report_avg_enabled;
    U32 layer1_meas_report_perodicity;
}l2_rrm_layer1_meas_report_config_info_t;

#define RRM_L2_l1_MEAS_REPORT_CONFIG_INFO   0x01
#define RRM_L2_l2_MEAS_REPORT_CONFIG_INFO   0x02
typedef  struct _rrm_report_config_info_t
{
    rrm_bitmask_t  bitmask;
    l2_rrm_layer1_meas_report_config_info_t    l1_meas_report_config_info;
    l2_rrm_layer2_meas_report_config_info_t    l2_meas_report_config_info;
}rrm_report_config_info_t;

typedef enum
{
    ICIC_SCHEME_HFR,
    ICIC_SCHEME_FFR,
    ICIC_SCHEME_SFR,
    ICIC_SCHEME_NONE
}rrm_icic_scheme_type_et;


typedef struct _rrm_delta_sinr_to_tpc_map_pusch_info_t
{
    S32 start_delta_sinr;  /*^ M, 0, B, -255, 255^*/
    S32 end_delta_sinr;  /*^ M, 0, B, -255, 255^*/
    S32 pusch_tpc;  /*^ M, 0, B, -4, 4^*/
}rrm_delta_sinr_to_tpc_map_pusch_info_t;

typedef struct _rrm_delta_sinr_to_tpc_map_pusch_t
{
    U32 count;
    rrm_delta_sinr_to_tpc_map_pusch_info_t delta_sinr_to_tpc_map_pusch_info[RRMCM_RMIF_MAX_DELTA_SINR_TO_TCP_MAP_PUSCH];
}rrm_delta_sinr_to_tpc_map_pusch;

typedef struct _delta_bler_to_tpc_map_for_pucch_info_t
{
    S32 start_bler;  /* M, 0, N, 0, 0^*/
    S32 end_bler;  /* M, 0, N, 0, 0^*/
    S32 pucch_tpc;  /* M, 0, B, -1, 3^*/
}rrm_delta_bler_to_tpc_map_for_pucch_info_t;


typedef struct _rrm_bler_to_tpc_map_for_pucch_t
{
    U32 pucch_bler_target;  /*^ M, 0, B, 0, 100^*/
    U32 count;  /*^ M, 0, B, 1, 4^*/
    rrm_delta_bler_to_tpc_map_for_pucch_info_t delta_bler_tpc_map_pucch_info[RRMCM_RMIF_MAX_NUM_TPC_FOR_PUCCH];  /*^ M, 0, OCTET_STRING, VARIABLE^*/
}rrm_bler_to_tpc_map_for_pucch_t;

typedef struct _delta_sinr_to_tpc_map_forpucch_Info_t
{
    S32 start_sinr;  /*^ M, 0, N, 0, 0^*/
    S32 end_sinr;  /*^ M, 0, N, 0, 0^*/
    S32 pucch_tpc;  /*^ M, 0, B, -1, 3^*/
}delta_sinr_to_tpc_map_forpucch_Info_t;

typedef struct _rrm_delta_sinr_to_tpc_map_pucch_info_t
{
     U32 pucch_sint_target;  /*^ M, 0, B, 0, 255^*/
     U32 count;  /*^ M, 0, B, 1, 4^*/
     delta_sinr_to_tpc_map_forpucch_Info_t delta_sinr_tpc_map_for_pucch_Info[RRMCM_RMIF_MAX_NUM_TPC_FOR_PUCCH];  /*^ M, 0, N, 0, 0^*/
}rrm_delta_sinr_to_tpc_map_pucch_info_t;

typedef struct _aggregation_power_offset_info_t
{
    U32 aggregation_level;
    U32 power_offset;  /*^ M, 0, N, 0, 1000^*/
}aggregation_power_offset_info_t;

typedef struct _aggregation_power_offset_t
{
    U32 count;  /*^ M, 0, B, 1, 4^*/
    aggregation_power_offset_info_t  aggr_pwr_offset_tuples[RRMCM_RMIF_MAX_AGGREGATION_LEVEL_POWER_OFFSET_COUNT];  /*^ M, 0, N, 0, 0^*/
}rrm_aggregation_power_offset_t;

typedef struct _rrm_aggregation_power_offset_on_cqi_basis_t 
{
    rrm_aggregation_power_offset_t aggregation_power_offset_user[RRMCM_RMIF_MAX_CQI_VAL]; /*^ M, 0, OCTET_STRING, VARIABLE^*/
}rrm_aggregation_power_offset_on_cqi_basis_t;

#define RRMCM_RMIF_AGGREGATION_POWER_OFFSET_CE_USER_PRESENT 0x01
typedef struct _rrm_pdcch_aggregation_power_offset_t
{
    U32 bitmask;
    rrm_aggregation_power_offset_on_cqi_basis_t aggregation_power_offset_cc_user[RRM_OAM_NUM_DCI_SIZE_CATEGORY];  /*^ M, 0, OCTET_STRING, VARIABLE^*/
    rrm_aggregation_power_offset_on_cqi_basis_t aggregation_power_offset_ce_user[RRM_OAM_NUM_DCI_SIZE_CATEGORY];  /*^ O, AGGREGATION_POWER_OFFSET_CE_USER_PRESENT, OCTET_STRING, VARIABLE^*/
}rrm_pdcch_aggregation_power_offset_t;


typedef struct _rrm_cqi_to_phich_power_t
{
    U32    cqi_to_phich_power_info[RRMCM_RMIF_MAX_CQI_VAL];  /*^ M, 0, B, 0, 10000^*/
}rrm_cqi_to_phich_power_t;


typedef struct _rrm_sps_dl_scheduling_Info_per_tti_t
{
    U32 max_dl_sps_dci_per_tti;  /*^ M, 0, B, 1, 10^*/
    U32 max_dl_sps_Occasion_Per_tti;  /*^ M, 1, B, 0, 20^*/
    U32 max_dl_sps_rbs_per_tti;  /*^ M, 0, B, 1, 100^*/
    U32 max_dl_sps_rbs_per_tti_per_interval[RRMCM_RMIF_MAX_SPS_INTERVALS];  /*^ M, 0, B, 0, 100^*/
}rrm_sps_dl_scheduling_Info_per_tti_t;


typedef struct _rrm_sps_ul_scheduling_Info_per_tti_t
{
    U32 max_ul_sps_dci_per_tti;  /*^ M, 0, B, 1, 10^*/
    U32 max_ul_sps_occasion_per_tti;  /*^ M, 1, B, 0, 20^*/
    U32 max_ul_sps_rbs_per_tti;  /*^ M, 0, B, 1, 100^*/
    U32 max_ul_sps_rbs_tti_per_interval[RRMCM_RMIF_MAX_SPS_INTERVALS];  /*^ M, 0, B, 0, 100^*/
}rrm_sps_ul_scheduling_Info_per_tti_t;


typedef struct _rrm_hd_reduced_mcs_table_t
{
    U8 reduced_msc[RRMCM_RMIF_MAX_REDUCED_MSC];  /*^ M, 0, N, 0, 0^*/
}rrm_hd_reduced_mcs_table_t;


typedef struct _rrm_dynamic_cfi_extension_params_t
{
    U8 cce_correction_step_up_factor[RRMCM_RMIF_MAX_POSSIBLE_AGGREGAITION_LEVELS];
    U8 cce_correction_step_down_factor[RRMCM_RMIF_MAX_POSSIBLE_AGGREGAITION_LEVELS];
    U8 cce_adjust_factor[RRMCM_RMIF_MAX_POSSIBLE_AGGREGAITION_LEVELS];
}rrm_dynamic_cfi_extension_params_t;


//  Will be supported in rel 5.2
typedef struct _rrm_atb_config_t
{
    U16     min_mcs_index_for_atb;
    U16     min_prb_val_for_atb;
}rrm_atb_config_t;

typedef struct _rrm_pa_qpsk_info_t
{
    S32 pa_qpsk[RRMCM_RMIF_MAX_QPSK_MCS_NUM];  /*^ M, 0, N, 0, 0^*/
}rrm_pa_qpsk_info_t;

typedef struct _path_loss_to_target_sinr_map_info_t
{
    S32 start_PL;  /*^ M, 0, B, -230, 460^*/
    S32 end_PL;  /*^ M, 0, B, -230, 460^*/
    U32 target_SINR;  /*^ M, 0, B, 0, 255^*/
}path_loss_to_target_sinr_map_info_t;

typedef struct _rrm_path_loss_to_target_sinr_map_t
{
    U32 count;  /*^ M, 0, B, 1, 2^*/
    path_loss_to_target_sinr_map_info_t path_loss_to_target_sinr_map_info[RRMCM_RMIF_MAX_NUM_PATHLOSS_TO_SINR_COUNT];  /*^ M, 0, N, 0, 0^*/
}rrm_path_loss_to_target_sinr_map_t;

typedef struct _rrm_alpha_based_pathloss_target_sinr_map_t
{
    U32  default_path_loss;  /*^ M, 0, N, 0, 0^*/
    rrm_path_loss_to_target_sinr_map_t path_loss_target_sinr_map[RRMCM_RMIF_MAX_NUM_ALPHA_COUNT];  /*^ M, 0, N, 0, 0^*/
}rrm_alpha_based_pathloss_target_sinr_map_t;


#define RRMCM_RMIF_ICIC_SCHEME_TYPE_PRESENT                        0x00000001
#define RRMCM_RMIF_DL_RESOURCE_PARTITION_INFO_PRESENT              0x00000002
#define RRMCM_RMIF_UL_RESOURCE_PARTITION_INFO_PRESENT              0x00000004
#define RRMCM_RMIF_UL_POWER_MASK_PRESENT                           0x00000008
#define RRMCM_RMIF_RNTP_REPORT_CONFIG_INFO_PRESENT                 0x00000010
#define RRMCM_RMIF_TPC_FOR_MSG3_PRESENT                            0x00000020
#define RRMCM_RMIF_SRS_SUBBAND_GRP_SIZE_PRESENT                    0x00000040
#define RRMCM_RMIF_TPC_TRIGGER_WAIT_TIME_PUSCH_DCI0_PRESENT        0x00000080
#define RRMCM_RMIF_TPC_TRIGGER_WAIT_TIME_PUCCH_DCI1x2x_PRESENT     0x00000100
#define RRMCM_RMIF_TPC_NUM_UE_THRESHOLD_PUSCH_DCI3A_PRESENT        0x00000200
#define RRMCM_RMIF_TPC_NUM_UE_THRESHOLD_PUCCH_DCI3A_PRESENT        0x00000400
#define RRMCM_RMIF_COOL_OFF_PRD_FOR_BLER_BIT                       0x00000800
#define RRMCM_RMIF_COOL_OFF_PERIOD_FOR_PUCCHTPC_PRESENT            0x00001000
#define RRMCM_RMIF_COOL_OFF_PERIOD_PUSCHTPC_PRESENT                0x00002000
#define RRMCM_RMIF_ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_PRESENT       0x00004000
#define RRMCM_RMIF_DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_PRESENT         0x00008000
#define RRMCM_RMIF_BLER_TO_TPC_MAP_FOR_PUCCH_PRESENT               0x00010000
#define RRMCM_RMIF_DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_PRESENT         0x00020000
#define RRMCM_RMIF_PDCCH_AGG_AND_POW_OFFSET_PRESENT                0x00040000
#define RRMCM_RMIF_CQI_TO_PHICH_POW_PRESENT                        0x00080000
#define RRMCM_RMIF_MIN_RB_FOR_PL_PHR_CALC_BIT_PRESENT              0x00100000
#define RRMCM_RMIF_SPS_DL_SCHEDULING_INFO_PER_TTI_PRESENT          0x00200000
#define RRMCM_RMIF_SPS_UL_SCHEDULING_INFO_PER_TTI_PRESENT          0x00400000
#define RRMCM_RMIF_ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_SPS_PRESENT   0x00800000
#define RRMCM_RMIF_HD_REDUCED_MCS_TABLE_PRESENT                    0x01000000
#define RRMCM_RMIF_DYNAMIC_CFI_EXTENSION_PARAM_PRESENT             0x02000000
#define RRMCM_RMIF_ATB_CONFIG_PRESENT                              0x04000000
#define RRMCM_RMIF_UL_MU_MIMO_TYPE_PRESENT                         0x08000000
#define RRMCM_RMIF_MSC_THRESHOLD_UL_MU_MIMO_PRESENT                0x10000000
#define RRMCM_RMIF_PA_QPSK_INFO_PRESENT                            0x20000000
/* SPR 21429 Fix Start */
#define RRMCM_RMIF_REPORT_CONFIG_INFO_PRESENT                      0x40000000
/* SPR 21429 Fix End */

typedef struct _dynamic_icic_info_t
{
    rrm_bitmask_t                           bitmask; /*^ BITMASK ^*/
    rrm_icic_scheme_type_et                                     icic_scheme_type;
    rrm_resource_partition_info_t      dl_resource_partition_info;  /*^ O, RRMCM_RMIF_DL_RESOURCE_PARTITON_INFO_PRESENT, N, 0, 0^*/
    rrm_resource_partition_info_t      ul_resource_partition_info;  /*^ O, RRMCM_RMIF_UL_RESOURCE_PARTITION_INFO_PRESENT, N, 0, 0^*/
    
    rrm_resource_partition_info_t      original_configured_dl_resource_partition_info;
    rrm_resource_partition_info_t      original_configured_ul_resource_partition_info;  

    rrm_ul_power_mask_t             ul_power_mask;
    rrm_rntp_report_config_info_t   rntp_report_config_info;
    rrm_report_config_info_t        report_config_info;
    
    S32   tpc_for_msg3;  /*^ O, RRMCM_RMIF_TPC_FOR_MSG3_PRESENT, B, 0, 7^*/  
    U32   srs_subband_group_size;  /*^ O, RRMCM_RMIF_SRS_SUBBAND_GRP_SIZE_PRESENT, N, 0, 0^*/
    U32   tpc_trigger_waittime_for_pusch_DCI0;  /*^ O, RRMCM_RMIF_TPC_TRIGGER_WAIT_TIME_PUSCH_DCI0_PRESENT, N, 0, 0 ^*/
    U32   tpc_trigger_waittime_for_pucch_DCI1x2x;  /*^ O, RRMCM_RMIF_TPC_TRIGGER_WAIT_TIME_PUCCH_DCI1x2x_PRESENT, N, 0, 0 ^*/ 
    U32   tpc_num_ue_threshold_for_pusch_DCI3A;  /*^ O, RRMCM_RMIF_TPC_NUM_UE_THRESHOLD_PUSCH_DCI3A_PRESENT, B, 0, 100^*/
    U32   tpc_num_ue_threshold_for_pucch_DCI3A;  /*^ O, RRMCM_RMIF_TPC_NUM_UE_THRESHOLD_PUCCH_DCI3A_PRESENT, B, 0, 100^*/

	U32   cooloff_period_for_bler_triggered_pucchTPC;  /*^ O, RRMCM_RMIF_COOL_OFF_PRD_FOR_BLER_BIT, N, 0, 0^*/
	U32   cooloff_period_for_pucchTPC;  /*^ O, RRMCM_RMIF_COOL_OFF_PERIOD_FOR_PUCCHTPC_PRESENT, N, 0, 0^*/
	U32   cooloff_period_for_puschTPC;  /*^ O, RRMCM_RMIF_COOL_OFF_PERIOD_PUSCHTPC_PRESENT, N, 0, 0^*/

    rrm_alpha_based_pathloss_target_sinr_map_t      alpha_pathloss_target_sinr_map;  /*^ O, RRMCM_RMIF_ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_PRESENT , N, 0, 0^*/  
    rrm_delta_sinr_to_tpc_map_pusch                 delta_sinr_tpc_map_pusch_info;  /*^ O, RRMCM_RMIF_DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_PRESENT, N, 0, 0^*/
    rrm_bler_to_tpc_map_for_pucch_t                 bler_to_tpc_map_for_pucch;  /*^ O, RRMCM_RMIF_BLER_TO_TPC_MAP_FOR_PUCCH_PRESENT, N, 0, 0^*/
    rrm_delta_sinr_to_tpc_map_pucch_info_t          delta_sinr_tpc_map_pucch_info;  /*^ O, RRMCM_RMIF_DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_PRESENT, N, 0, 0^*/
    rrm_pdcch_aggregation_power_offset_t            pdcch_aggregation_power_offset;  /* O, RRMCM_RMIF_PDCCH_AGG_AND_POW_OFFSET_PRESENT, N, 0, 0^*/
    rrm_cqi_to_phich_power_t                        cqi_to_phich_power;  /*^ O, RRMCM_RMIF_CQI_TO_PHICH_POW_PRESENT, N, 0, 0^*/
    
    U32     min_rb_for_pl_phr_calc;  /*^ O, RRMCM_RMIF_MIN_RB_FOR_PL_PHR_CALC_BIT_PRESENT, N, 0, 0^*/

    rrm_sps_dl_scheduling_Info_per_tti_t            sps_dl_scheduling_Info_per_tti;  /*^ O, RRMCM_RMIF_SPS_DL_SCHEDULING_INFO_PER_TTI_PRESENT, N, 0, 0^*/
    rrm_sps_ul_scheduling_Info_per_tti_t            sps_ul_scheduling_Info_per_tti;  /*^ O, RRMCM_RMIF_SPS_UL_SCHEDULING_INFO_PER_TTI_PRESENT, N, 0, 0^*/

    rrm_alpha_based_pathloss_target_sinr_map_t      alpha_pathloss_target_sinr_map_sps;  /*^ O, RRMCM_RMIF_ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_SPS_PRESENT , N, 0, 0^*/
    U8              hd_reduced_mcs_table[RRMCM_RMIF_MAX_MCS];  /*^ O, RRMCM_RMIF_HD_REDUCED_MCS_TABLE_PRESENT, N, 0, 0^*/
    rrm_dynamic_cfi_extension_params_t      dynamic_cfi_extension_params;  /*^ O, RRMCM_RMIF_DYNAMIC_CFI_EXTENSION_PARAM_PRESENT, N, 0, 0^*/
    rrm_atb_config_t                        atb_config;  /*^ O, RRMCM_RMIF_ATB_CONFIG_PRESENT, N, 0, 0^*/

    U32     ul_mu_mimo_type;  /*^ O, RRMCM_RMIF_UL_MU_MIMO_TYPE_PRESENT, B, 0, 1^*/
    U32     msc_threshold_ul_mu_mimo;  /*^ O, RRMCM_RMIF_MSC_THRESHOLD_UL_MU_MIMO_PRESENT, B, 0, 28^*/
    U32     pa_qpsk;  /*^ O, RRMCM_RMIF_PA_QPSK_INFO_PRESENT, B, 0, 28^*/

}rrm_dynamic_icic_info_t;

/*  DYNAMIC ICIC CHANGES END  */


/* UE positioning start */
typedef struct _rrm_eutran_access_point_pos_t
{
    rrm_bitmask_t   bitmask;
    U32             latitude_sign;    /* rrm_oam_latitude_sign_et */
    U32             deg_of_latitude;
    S32             deg_of_longitude;
    U32             dir_of_altitude;  /* rrm_oam_altitude_dir_et */
    U16             altitude;
    U32             uncertainty_semi_major;
    U32             uncertainty_semi_minor;
    U8              orientation_of_major_axis;
    U16             uncertainty_altitude;
    U8              confidence;
}rrm_eutran_access_point_pos_t;
/* UE positioning end */

/*TM mode 7-8 start*/
typedef struct _rrm_sinr_to_beam_forming_config_t
{
    U16 beam_frmng_vector_for_A1;
}rrm_sinr_to_beam_forming_config_t;

#define RRM_AOA_TO_BEAM_FRMNG_VECTOR_MPPNG_PRESENT 0x01
#define RRM_SINR_TO_BEAM_FRMNG_CONFIG_PRESENT 0x02
typedef struct _rrm_mac_beam_forming_info_t
{
    rrm_bitmask_t bitmask;
    U16 rrm_aoa_to_beam_frmng_vctr_mppng[RRM_MAX_BF_VECTORS_TO_AOA_VAL];
    rrm_sinr_to_beam_forming_config_t sinr_to_beam_forming_config_t[RRM_MAX_BF_VECTORS_TO_SINR_VAL];
}rrm_mac_beam_forming_info_t;
/*TM mode 7-8 end*/

/*TM mode 7-8 start*/
typedef struct _rrm_tm_mode_table_row_t
{
    rrm_level_t sinr;
    rrm_level_t pathloss;
    rrm_level_t mobility;
    transmission_mode_ue_service_profile_et tm_mode;
}rrm_tm_mode_table_row_t;

#define RRM_TM_MODE_FOR_STRNGNT_QCI_PRESENT 0x01
typedef struct _rrm_transmission_mode_table_t
{
    rrm_bitmask_t bitmask;    
    transmission_mode_ue_service_profile_et tm_mode_for_strngnt_qci;
    U8 count;
    rrm_tm_mode_table_row_t rrm_tm_mode_table_row[RRM_OAM_MAX_TM_MODE_TABLE_ROW];
}rrm_transmission_mode_table_t;

typedef struct _rrm_tm_mode_additional_info_t
{
    S8 pdsch_epre_to_ue_rs_ratio;
    rrm_bool_et enable_pmi_ri_report_r9;
    rrm_beam_forming_algo_et beam_forming_algo;
}rrm_tm_mode_additional_info_t;
/*TM mode 7-8 end*/
/* BUG_371 FIX START */ 
/* CA_Stage3_PUCCH_Format_Enh: Start */
#define RRMCM_RMIF_PUCCH_FORMAT_R10_PRESENT 0x0001
/* CA_Stage3_PUCCH_Format_Enh: End */
typedef struct _rrm_operator_ul_pwr_ctrl_t 
{
	rrmc_delta_f_pucch_format_one_et	delta_f_pucch_format_1;
	rrmc_delta_f_pucch_format_one_b_et 	delta_f_pucch_format_1b;
	rrmc_delta_f_pucch_format_two_et	delta_f_pucch_format_2; 
	rrmc_delta_f_pucch_format_two_ab_et	delta_f_pucch_format_2a;
	rrmc_delta_f_pucch_format_two_ab_et	delta_f_pucch_format_2b;
    /* CA_Stage3_PUCCH_Format_Enh: Start */
    U16                                       bitmask;
    rrmc_delta_f_pucch_format_one_b_cs_r10_et delta_f_pucch_format_1b_cs_r10; 
    rrmc_delta_f_pucch_format_three_r10_et    delta_f_pucch_format_3_r10; 
    /* CA_Stage3_PUCCH_Format_Enh: End */
  S8 delta_preamble_msg_3; /* -1, 6 */
}rrm_operator_ul_pwr_ctrl_t;
  /* BUG_371 FIX END */ 


/** Dynamic UE Scheduling Start **/
#define RRM_UE_SCHEDULING_HYSTERESIS_PRESENT    0x01 
typedef struct _rrm_max_ue_schedule_dl_ul_t
{
    rrm_bitmask_t    bitmask;
    U8               max_ue_scheduled_dl; /* mandatory */
    U8               max_ue_scheduled_ul; /* mandatory*/
    U8               ue_scheduling_hysteresis; /* optional : RRM_UE_SCHEDULING_HYSTERESIS_PRESENT */
}rrm_max_ue_schedule_dl_ul_t;

/** Dynamic UE Scheduling End   **/

/* Carrier_Aggregation_Start */

#define RRM_LOAD_CALCULATION_WINDOW_SIZE_PRESENT    0x0001
#define RRM_LOAD_DISPARITY_THRESHOLD_PRESENT        0X0002
#define RRM_MCS_DIFF_THRESHOLD_PRESENT              0x0004
#define RRM_PRB_USAGE_WATER_MARK_PRESENT            0x0008
#define RRM_DL_DEACT_COMMAND_THRESH_PRESENT         0x0010
#define RRM_DL_DEACT_MCS_LOW_THRESH_PRESENT         0x0020
#define RRM_DL_Q_LOAD_UP_THRESH_PRESENT             0x0040
#define RRM_DL_Q_LOAD_LOW_THRESH_PRESENT            0x0080
#define RRM_ACT_DEACT_TRIG_COUNT_THRESH_PRESENT     0X0100
/* SPR 16422 START */
#define RRM_CQI_VALIDITY_TIMER_PRESENT              0X0200
/* SPR 16422 END */

/*! \ rrm_ca_mac_scheduler_params_t
 *  \brief This struct is used for MAC Scheduler related parameters
 *  \param bitmask To check optional field is present or not
 *  \param load_calcualtion_window_size Load calculation window size
 *  \param load_disparity_threshold  Load disparity threshold
 *  \param mcs_diff_threshold MCS diff threshold
 *  \param prb_usage_water_mark PRB usage water mark
 *  \param dl_deact_command_thresh DL deactivation command threshold
 *  \param dl_deact_mcs_low_thresh DL deactivation MCS low threshold
 *  \param dl_qload_up_threshold DL queue load up threshold
 *  \param dl_qload_low_threshold DL queue load low threshold
 *  \param act_deact_trigger_count_threshold Act deact trigger count threshold
 */
typedef struct _rrm_ca_mac_scheduler_params_t
{
    rrm_bitmask_t                       bitmask;                            /*^ BITMASK ^*/
    U16                                 load_calcualtion_window_size;       /*^ O, RRM_LOAD_CALCULATION_WINDOW_SIZE_PRESENT, B, 100, 5000 ^*/
    U8                                  load_disparity_threshold;           /*^ O, RRM_LOAD_DISPARITY_THRESHOLD_PRESENT, H, 0, 100 ^*/
    U8                                  mcs_diff_threshold;                 /*^ O, RRM_MCS_DIFF_THRESHOLD_PRESENT, H, 0, 28 ^*/
    U8                                  prb_usage_water_mark;               /*^ O, RRM_PRB_USAGE_WATER_MARK_PRESENT, H, 0, 100 ^*/
    U8                                  dl_deact_command_thresh;            /*^ O, RRM_DL_DEACT_COMMAND_THRESH_PRESENT, H, 0, 255 ^*/
    U8                                  dl_deact_mcs_low_thresh;            /*^ O, RRM_DL_DEACT_MCS_LOW_THRESH_PRESENT, H, 0, 28 ^*/
    U8                                  dl_qload_up_threshold;              /*^ O, RRM_DL_Q_LOAD_UP_THRESH_PRESENT, H, 0, 255 ^*/
    U8                                  dl_qload_low_threshold;             /*^ O, RRM_DL_Q_LOAD_LOW_THRESH_PRESENT, H, 0, 255 ^*/
    U8                                  act_deact_trigger_count_threshold;  /*^ O, RRM_ACT_DEACT_TRIG_COUNT_THRESH_PRESENT, H, 0, 255 ^*/
    /* SPR 16422 START */
    U8                                  cqi_validity_timer;                 /*^ O, RRM_OAM_CQI_VALIDITY_TIMER_PRESENT, N, 0, 255 ^*/
    /* SPR 16422 END */
}rrm_ca_mac_scheduler_params_t;

/*  LAA Rel13 Code + */
/*  LAA Rel13 Code - */
#define RRM_IS_CA_ELIGIBLE_INFO_PRESENT             0x0001
#define RRM_NUM_OF_REPORT_PRESENT                   0x0002
#define RRM_CA_APPLICABLE_BITRATE_DL_PRESENT        0x0004
#define RRM_IS_MASA_INFO_PRESENT                    0x0008
#define RRM_SCELL_DEACTIVATION_TIMER_PRESENT        0x0010
#define RRM_CA_MAC_SCHEDULER_PARAMS_INFO_PRESENT    0x0020
/*CA STAGE 2*/
#define RRM_CA_MEAS_CYCLE_SCELL_PRESENT             0x0080
/*! \ rrm_ca_config_t
 *  \brief This struct is used for Carrier Aggregation information of Scell
 *  \param bitmask To check optional field is present or not
 *  \param is_ca_eligible indicates Is this cell eligible for Scell
 *  \param num_of_report indicates the number of throughput reports
 *  \param ca_applicable_bitrate_dl indicates CA applicable downlink bitrate 
 *  \param is_masa indicates the scell addition is done based on UE measurements 
 *  \param scell_deactivation_timer enum variable indicates the deactivation time for the Scell
 *  \param ca_mac_scheduler_params Indicates the MAC Scheduler related parameters for CA
 *  \param l2_instance indicates number of l2 instances for CA
 */
typedef struct _rrm_ca_config_t
{
    rrm_bitmask_t                       bitmask;                    /*^ BITMASK ^*/
    rrm_bool_t                          is_ca_eligible;             /*^ O, RRM_IS_CA_ELIGIBLE_INFO_PRESENT, H, 0, 1 ^*/
    U8                                  num_of_report;              /*^ O, RRM_NUM_OF_REPORT_PRESENT, B, 1, 20 ^*/
    U64                                 ca_applicable_bitrate_dl;   /*^ O, RRM_CA_APPLICABLE_BITRATE_DL_PRESENT, L, 5, 0 ^*/
    rrm_bool_t                          is_masa;                    /*^ O, RRM_IS_MASA_INFO_PRESENT, H, 0, 1 ^*/
    rrm_scell_deactivation_timer_et     scell_deactivation_timer;   /*^ O, RRM_SCELL_DEACTIVATION_TIMER_PRESENT, B, 0, 7 ^*/
    rrm_ca_mac_scheduler_params_t       ca_mac_scheduler_params;    /*^ O, RRM_CA_MAC_SCHEDULER_PARAMS_INFO_PRESENT, N, 0, 0 ^*/
    /*CA STAGE 2*/
    /*SPR 21206 +-*/
    rrm_meas_cycle_scell_et             meas_cycle_scell;           /*^ O, RRM_CA_MEAS_CYCLE_SCELL_PRESENT,  N, 0, 0 ^*/

}rrm_ca_config_t;
/* Carrier_Aggregation_End */\
/** eICIC_PHASE_1_2_CHANGES_START */
#define RRMCM_RMIF_LOW_LOAD_ABS_PATTERN_PRESENT            0x0001
#define RRMCM_RMIF_VICTIM_ABS_PATTERN_PRESENT              0x0002
#define RRMCM_RMIF_VICTIM_MEAS_SUBSET_PRESENT              0x0004
/*! rrm_abs_pattern_info_t
 *  \brief  This struct used to get the ABS pattern configuration from OAM.
 *  \param  bitmask bitmask information for ABS pattern.
 *  \param  low_load_abs_pattern Used for providing ABS pattern to the victim node in 
 *          case node is acting as aggressor node. 
 *  \param  victim_abs_pattern Used for providing ABS pattern to the victim node in 
 *          case node is acting as aggressor node 
 *  \param  victim_meas_subset pattern is used for Measurement configuration
 */
typedef struct _rrm_abs_pattern_info_t
{
    rrm_bitmask_t                       bitmask;                    /*^ BITMASK ^*/
    U8                                  low_load_abs_pattern[RRM_MAX_PATTERN_BYTE];
                                                                    /*^ O, RRMCM_RMIF_LOW_LOAD_ABS_PATTERN_PRESENT, OCTET_STRING, FIXED ^*/
    U8                                  victim_abs_pattern[RRM_MAX_PATTERN_BYTE];
                                                                    /*^ O, RRMCM_RMIF_VICTIM_ABS_PATTERN_PRESENT, OCTET_STRING, FIXED ^*/
   /*eicic_subset_pattern_fix_start*/
    U8                                  victim_meas_subset[RRM_MAX_PATTERN_BYTE];
                                                                    /*^ O, RRMCM_RMIF_VICTIM_MEAS_SUBSET_PRESENT, OCTET_STRING, FIXED ^*/
   /*eicic_subset_pattern_fix_start*/
}rrm_abs_pattern_info_t;

#define RRMCM_RMIF_NUM_ABS_REPORT_PRESENT                  0x0001
#define RRMCM_RMIF_ABS_USAGE_LOW_THRESHOLD_PRESENT         0x0002
#define RRMCM_RMIF_ABS_USAGE_HIGH_THRESHOLD_PRESENT        0x0004
#define RRMCM_RMIF_ABS_PATTERN_INFO_PRESENT                0x0008
/*! rrm_abs_info_t
 *  \brief  This struct used for ABS information
 *  \param  bitmask bitmask information for ABS information.
 *  \param  num_abs_report  Number of ABS status report received from victim eNB.
 *          after which aggressor will take action to increase or decrease the ABS pattern.
 *  \param  abs_usage_low_threshold It indicates if the abs usage reported in resource status 
 *          update message from victim is less than this value then ABS pattern needs to be 
 *          decreased by one subframe.
 *  \param  abs_usage_high_threshold It indicates if the abs usage reported in resource status 
 *          update message from victim is greater than this value then ABS pattern needs to be 
 *          increased by one subframe.
 *  \param  abs_pattern Indicates the ABS patterns.
 */
typedef struct _rrm_abs_info_t
{
    rrm_bitmask_t                       bitmask;                    /*^ BITMASK ^*/
    U16                                 num_abs_report;             /*^ O, RRMCM_RMIF_NUM_ABS_REPORT_PRESENT, H, 0, 1440 ^*/
    U8                                  abs_usage_low_threshold;    /*^ O, RRMCM_RMIF_ABS_USAGE_LOW_THRESHOLD_PRESENT, B, 1, 100 ^*/
    U8                                  abs_usage_high_threshold;   /*^ O, RRMCM_RMIF_ABS_USAGE_HIGH_THRESHOLD_PRESENT, B, 1, 100 ^*/
    rrm_abs_pattern_info_t              abs_pattern;                /*^ O, RRMCM_RMIF_ABS_PATTERN_INFO_PRESENT, N, 0, 0 ^*/
}rrm_abs_info_t;

#define RRMCM_RMIF_MAC_EICIC_MEAS_REPORT_PERIODICITY_PRESENT   0x0001
#define RRMCM_RMIF_AGGRESSOR_SEL_TIMER_PRESENT                 0x0002
#define RRMCM_RMIF_LOAD_INFORMATION_GUAURD_TIMER_PRESENT       0x0004
#define RRMCM_RMIF_LOAD_INFORMATION_COLLATION_TIMER            0x0008
/*! rrm_eicic_timer_info_t
 * \brief   This struct used to get the configuration from OAM.
 * \param   bitmask store the bitmask information.
 * \param   mac_eicic_meas_report_periodicity store the measurement periodicity configured from OAM.
 * \param   aggressor_sel_timer Periodic timer that activate eICIC algorithm on the victim UE 
 *          present in the system
 * \param   load_information_guard_timer Guard timer started to consolidate load information sent 
 *          from different aggressors after sending invoke message to all aggressor nodes.
 * \param   load_information_collation_timer It indicates the timer interval for which RRM shall wait 
 *          for collation of subsequent load information message in which ABS Information update is 
 *          provided by aggressor without sending invoke by victim. 
 */
typedef struct _rrm_eicic_timer_info_t
{
    rrm_bitmask_t                       bitmask;                    /*^ BITMASK ^*/
    U32                                 mac_eicic_meas_report_periodicity;
                                                                    /*^ O, RRMCM_RMIF_MAC_EICIC_MEAS_REPORT_PERIODICITY_PRESENT, B, 20, 10000 ^*/
    U16                                 aggressor_sel_timer;        /*^ O, RRMCM_RMIF_AGGRESSOR_SEL_TIMER_PRESENT, B, 30, 7200 ^*/
    U16                                 load_information_guard_timer;
                                                                    /*^ O, RRMCM_RMIF_LOAD_INFORMATION_GUAURD_TIMER_PRESENT, B, 1, 15 ^*/
    U16                                 load_information_collation_timer;
                                                                    /*^ O, RRMCM_RMIF_LOAD_INFORMATION_COLLATION_TIMER, B, 1, 60 ^*/
}rrm_eicic_timer_info_t;

#define RRMCM_RMIF_EICIC_PROVISIONED_TYPE_PRESENT              0x0001
/*SPR 14165 start*/
//code deleted
/*SPR 14165 end*/
#define RRMCM_RMIF_ABS_INFO_PRESENT                            0x0002
/*eICIC_changes_start*/
#define RRMCM_RMIF_EICIC_ELIGIBLE_UE_PRESENT                   0x0008
/*eICIC_changes_end*/
#define RRMCM_RMIF_EICIC_TIMER_INFO_PRESENT                    0x0010
#define RRMCM_RMIF_DL_SINR_THRESHOLD_LOW_MARK_PRESENT          0x0020
#define RRMCM_RMIF_DL_SINR_THRESHOLD_HIGH_MARK_PRESENT         0x0040

/*! \rrm_eicic_info_t
 *  \breif  This structure contains all the elements required for eICIC feature. Support of 
 *          eICIC shall be provided in both FDD and TDD mode and applicable to intra frequency 
 *          LTE cells only.It will develop in Hetrogenous networks.
 *  \param  bitmask To indicate if the optional fields are present or not
 *  \param  eicic_provisioned_type This parameter is used for Activation/Deactivation of eICIC feature.
 *          In case value is deactivated then eICIC feature will not work on particular cell.
 *  \param  rsrp This parameter is used for Cell-specific offset applicable to a serving cell. 
 *          It is used for connected mode.   
 *  \param  abs_info contains the information about ABS pattern.
 *  \param  eicic_timer_info contains the information about EICIC timers.
 *  \param  rsrq Threshold to be used in EUTRA (intra frequency cell) measurement report 
 *          triggering condition for event a4 for interfearing cells detection.
 *  \param  dl_sinr_threshold_low_mark DL SINR value below which a UE is being identified as victim UE.
 *  \param  dl_sinr_threshold_high_mark DL SINR value below which a UE is being identified as victim UE.
 */

typedef struct _rrm_eicic_info_t
{
    rrm_bitmask_t                       bitmask;                    /*^ BITMASK ^*/
    rrm_oam_eicic_node_type_et          eicic_provisioned_type;     /*^ O, RRMCM_RMIF_EICIC_PROVISIONED_TYPE_PRESENT, H, 0, 3 ^*/
    /*SPR 14165 start*/
    //code deleted
    /*SPR 14165 end*/
    rrm_abs_info_t                      abs_info;                   /*^ O, RRMCM_RMIF_ABS_INFO_PRESENT, N, 0, 0 ^*/
    /*eICIC_changes_start*/
    U8                                  eligible_ue;                 /*^ O, RRMCM_RMIF_EICIC_ELIGIBLE_UE_PRESENT, H, 1, 100 ^*/
    /*eICIC_changes_end*/
    rrm_eicic_timer_info_t              eicic_timer_info;           /*^ O, RRMCM_RMIF_EICIC_TIMER_INFO_PRESENT, N, 0, 0 ^*/
    U32                                 dl_sinr_threshold_low_mark; /*^ O, RRMCM_RMIF_DL_SINR_THRESHOLD_LOW_MARK_PRESENT, H, 0, 255 ^*/
    U32                                 dl_sinr_threshold_high_mark;/*^ O, RRMCM_RMIF_DL_SINR_THRESHOLD_HIGH_MARK_PRESENT, H, 0, 255 ^*/
}rrm_eicic_info_t;
/** eICIC_PHASE_1_2_CHANGES_END */


#ifdef ENDC_ENABLED
/* NR_REP_CONFIG_FIX_START */
typedef struct _rrm_cellm_peer_gnb_pci_t
{
    U16    nr_pci;
    U8     is_valid;            /* 0 - No , 1 - yes */ 
}rrm_cellm_peer_gnb_pci_t;

typedef struct _rrm_cellm_peer_gnb_pci_list_t
{
    U8    count;
    /*^ M, 0, B, 0, 8 ^*/

    rrm_cellm_peer_gnb_pci_t peer_gnb_pci[8];
}rrm_cellm_peer_gnb_pci_list_t;

typedef struct _rrm_cellm_endc_info_t
{
    U8                sgnb_ambr_share;/*^ M, O, B, MIN_SGNB_AMBR_SHARE, MAX_SGNB_AMBR_SHARE ^*/
    U8                is_ma_sgnb_addition; /*^ M, 0, H, 0, 1 ^*/
    U8                min_num_ue_thp_report; /*^ M, 0, B, 1, 20 ^*/
    U64               split_bearer_applicable_bitrate_dl;/*^ M, 0, N, 5, 150 ^*/
    U64               bearer_throughput_threshold_ul;/*^ M, 0, N, 5, 150 ^*/
    U64               bearer_throughput_threshold_dl;/*^ M, 0, N, 5, 150 ^*/
    U64               endc_sinr_thershold_ul;/*^ M, 0, N, 5, 150 ^*/
    U64               endc_sinr_thershold_dl;/*^ M, 0, N, 5, 150 ^*/
    U64               endc_bler_thershold_ul;/*^ M, 0, N, 5, 150 ^*/
    U64               endc_bler_thershold_dl;/*^ M, 0, N, 5, 150 ^*/
    U8                max_meas_freq_scg_nr;
    /*^ M, 0, B, 1, 32 ^*/
    U16               max_meas_id_scg_nr;
    /*^ M, i0, B, 1, 62 ^*/
    rrm_cellm_peer_gnb_pci_list_t    peer_gnb_pci_list;
    /*^ M, 0, N, 0, 0 ^*/
}rrm_cellm_endc_info_t;
/* NR_REP_CONFIG_FIX_END */
#endif
#define RRMCM_RMIF_ADD_CELL_PARAMS_PRESENT                      0x0001
#define RRMCM_RMIF_MAC_CONFIG_PRESENT                           0x0002
#define RRMCM_RMIF_PHICH_CONFIG_PRESENT                         0x0004
#define RRMCM_RMIF_SIB_1_INFO_PRESENT                           0x0008
#define RRMCM_RMIF_SIB_2_INFO_PRESENT                           0x0010
#define RRMCM_RMIF_SIB_3_INFO_PRESENT                           0x0020
#define RRMCM_RMIF_SIB_4_INFO_PRESENT                           0x0040
#define RRMCM_RMIF_ADDMISION_CONTROL_INFO_PRESENT               0x0080
#define RRMCM_RMIF_FREQ_PRIORITY_LIST                           0x0100
#define RRMCM_RMIF_PRIORITY_CFG                                 0x0200
#define RRMCM_RMIF_DAHO_CELL_LIST                               0x0400
#define RRMCM_SPID_TABLE_PRESENT                                0x0800
#define RRMCM_RMIF_MEASUREMENT_CONFIG_PRESENT                   0x1000
#define RRMCM_RMIF_SPS_CRNTI_RANGE_PRESENT                      0x2000
#define RRMCM_RMIF_HO_CONFIG_PARAMS_PRESENT                     0x4000
#define RRMCM_RMIF_RAT_PRIORITY_INFO_CFG_PRESENT                0x8000
/*  DYNAMIC ICIC CHANGES START  */
 #define RRMCM_RMIF_DYNAMIC_ICIC_INFO_PRESENT                   0x10000
/*  DYNAMIC ICIC CHANGES END  */
#define RRMCM_RMIF_EUTRAN_ACCESS_POINT_POS_PRESENT              0x20000
/*TM mode 7-8 start*/
#define RRMCM_RMIF_TRANSMISSION_MODE_TABLE_PRESENT              0x40000
/*TM mode 7-8 end*/
/** Dynamic UE Scheduling Start **/
#define RRM_DYNAMIC_UE_SCHEDULING_INFO_PRESENT                  0x80000
/** Dynamic UE Scheduling End **/
/* BUG_371 FIX START */
#define RRM_RRM_OPERATOR_UL_PWR_CTRL                            0x100000
/* BUG_371 FIX END */
/* Carrier_Aggregation_Start */
#define RRMCM_RMIF_CA_CONFIG_INFO_PRESENT                       0x200000
/* Carrier_Aggregation_End */
#define RRMCM_RMIF_SUBBAND_CQI_PRESENT                          0x400000
#define RRMCM_RMIF_SUBBAND_CQI_PERIODCITY_FACTOR_PRESENT        0x800000
#define RRMCM_RMIF_EICIC_INFO_PRESENT                           0x1000000
/* SPR 18641_18843 fix start */
#define RRC_SERVICE_PROFILE_UL_SCH_CONFIG_TTI_BUNDLING_INFO_PRESENT     0x2000000
/* SPR 18641_18843 fix end */
/* SPR 21429 Fix Start */
#define RRMCM_RMIF_RADIO_CONG_THRESHOLD_PRESENT                 0x4000000
/* SPR 21429 Fix End */
#ifdef LTE_EMBMS_SUPPORTED
/* SPR 18334 : START */
/*deleted*/
/* SPR 18334 : END */
#define RRMCM_RMIF_MBMS_POWER_INFO_PRESENT                      0x8000000
#endif
#ifdef ENDC_ENABLED
#define RRMCM_RMIF_ENDC_INFO_PRESENT                 0x20000000
#endif


#define RRMCM_RMIF_CRAN_PDCP_COMM_INFO_PRESENT 		0x01
#define RRMCM_RMIF_NON_CRAN_PDCP_COMM_INFO_PRESENT 	0x02

#define RRMCM_RMIF_DATA_PORT_PRESENT					0x04
/*! \struct operator_info_t 
 *  \brief This struct contains information related to operator info. 
 */
typedef struct operator_info
{
    rrm_bitmask_t                                               bitmask;
    rrm_cell_parameters_t                                       cell_params;
    rrmc_mac_config_t                                           rrm_mac_config;
    rrm_phy_phich_configuration_t                               phich_config;
    rrm_mac_enable_frequency_selective_scheduling_t	            frequency_selective_scheduling;
    U8                                                          contention_free_rach_timer;
    rrm_sib_type_1_info_t                                       sib_1_info;
    rrm_sib_type_2_info_t                                       sib_2_info;
    rrm_sib_type_3_info_t                                       sib_3_info;
    rrm_sib_type_4_info_t                                       sib_4_info;
    admission_control_info_t                                    admission_control_info;
    rrm_freq_priority_list_t                                    freq_priority_list;
    rrm_priority_cfg_t                                          priority_cfg;
    rrm_daho_cell_config_info_t                                 daho_cell_config_info;
    rrm_cellm_radio_congestion_threshold_t                      radio_cong_threshold;
    rrm_oam_spid_table_t                                        spid_table; 
    rrm_meas_config_info_t                                      meas_config_info;
    rrm_sps_crnti_range_t                                       sps_crnti_range_info;
    rrm_load_congestion_cfg_t                                   load_congestion_cfg;
    rrm_ho_oscillation_cfg_t                                    ho_oscillation_cfg;
    rrm_ho_config_params_t                                      ho_config_params;
    rrm_rat_priority_info_cfg_t                                 rat_priority_info;
    /* DYNAMIC ICIC CHANGES START */
    rrm_dynamic_icic_info_t                                     dynamic_icic_info;  
    /* DYNAMIC ICIC Change End */
    rrm_eutran_access_point_pos_t                               eutran_access_point_pos;
    /*TM mode 7-8 start*/
    rrm_transmission_mode_table_t                               rrm_tm_mode_table;
    rrm_mac_beam_forming_info_t                                 mac_beam_forming_info;
    rrm_tm_mode_additional_info_t                               rrm_tm_mode_additional_info;
    /*TM mode 7-8 end*/
    /** Dynamic UE Scheduling Start **/
    rrm_max_ue_schedule_dl_ul_t                                 rrm_ue_scheduling_info; /*RRM_OAM_DYNAMIC_UE_SCHEDULING_INFO_PRESENT */ 
    /** Dynamic UE Scheduling End   **/
    /*spr 7984 start*/
    rrm_bool_et                                                 rrm_enable_cqi_mask_r9;
    /*spr 7984 end*/
    /* Carrier_Aggregation_Start */
    /* BUG_371 FIX START */ 
    rrm_operator_ul_pwr_ctrl_t                                  rrm_operator_ul_pwr_ctrl; 
    /* BUG_371 FIX END */
    rrm_ca_config_t                                             ca_config; /* RRMCM_RMIF_CA_CONFIG_INFO_PRESENT */ 
    /* Carrier_Aggregation_End */
    U8                                                          k;
    U8                                                          periodicity_factor;
    /** eICIC_PHASE_1_2_CHANGES_START */
    rrm_eicic_info_t                                            eicic_info;
    /** eICIC_PHASE_1_2_CHANGES_END */
    /* SPR 18641_18843 fix start */
    rrm_tti_bundling_info_t tti_bundling_info;
    /*^ O, RRC_SERVICE_PROFILE_UL_SCH_CONFIG_TTI_BUNDLING_INFO_PRESENT ^*/
    /* SPR 18641_18843 fix end */

    /* SPR 21206 Start */
    U8                  l2_instance;      
	/* SPR 21206 End */
#ifdef LTE_EMBMS_SUPPORTED
    rrm_sib_type_13_info_t                                      sib_13_info;
    rrm_oam_mbms_power_info_t                                   mbms_power_info;
    /*RRMCM_RMIF_MBMS_POWER_INFO_PRESENT */
#endif
#ifdef ENDC_ENABLED
	rrm_cellm_endc_info_t                endc_info;
#endif

}operator_info_t;

/* COMP_WARN_1_FEB:compilation warning removal */
#define RRMCM_RMIF_ACCESS_MGM_CSG_ID_PRESENT      0x0001
/*! \struct access_mgmt_params_t 
 *  \brief This struct contains information related to access mgmt parameters. 
 */
typedef struct access_mgmt_params
{
        rrm_bitmask_t            bitmask;
	rrm_oam_access_mode_et   access_mode;
	S16                      max_csg_members; 
	S16                      max_non_csg_members;
	U8                       max_resource_non_csg_members;
	U8                       csg_id[NUM_CSG_OCTETS];
	U8                       hnb_name_size;
	U8                       hnb_name[RRMCM_MAX_HNB_ID];	
}access_mgmt_params_t;


typedef rrm_oam_eutran_global_cell_id_t rrm_global_cell_id_t;


/*
 * Interface structures Starts
 */

/*! \struct rrmcm_rmif_init_ind_t 
 *  \brief This struct contians information of internal module id of RRM. 
 */
typedef struct _rrmcm_rmif_init_ind_t
{
    rrm_internal_module_id_et module_id;
}rrmcm_rmif_init_ind_t;

/*! \struct rrmcm_rmif_init_config_req_t 
 *  \brief This struct contains information init config request received from MIF to CELLM and UEM.
 */
/* Spr 16211 Changes Start*/
#define RRMCM_RMIF_INIT_RIM_INFO_PRESENT	0x01
/* Spr 16211 Changes End*/
typedef struct _rrmcm_rmif_init_config_req_t
{
    /* Spr 16211 Changes Start*/
    rrm_bitmask_t           bitmask;
    /* Spr 16211 Changes End*/
	U16			transaction_id;
        rrm_oam_log_on_off_et   log_on_off; 
        rrm_oam_log_level_et    log_level;   
        /* Spr 16211 Changes Start*/
        rrm_bool_et             rim_enable_disable;
        /* Spr 16211 Changes End*/
} rrmcm_rmif_init_config_req_t;    /*^ API, RRMCM_RMIF_INIT_CONFIG_REQ ^*/

/*! \struct rrmcm_rmif_log_enable_disable_req_t 
 *  \brief This struct contains information of log enable disable request from MIF to CELLM and UEM. 
 */
typedef struct _rrmcm_rmif_log_enable_disable_req_t
{
	U16			transaction_id;
        rrm_oam_log_on_off_et   log_on_off; 
        rrm_oam_log_level_et    log_level;  
} rrmcm_rmif_log_enable_disable_req_t;     /*^ API, RRMCM_RMIF_LOG_ENABLE_DISABLE_REQ ^*/

/*! \struct rrmcm_rmif_set_log_level_req_t 
 *  \brief This struct contains set log level requset from MIF to CELLM and UEM. 
 */
typedef struct _rrmcm_rmif_set_log_level_req_t
{
	U16			transaction_id;
        rrm_oam_log_level_et    log_level;      /*^ M, 0, H, 0, 3 ^*/
}rrmcm_rmif_set_log_level_req_t;   /*^ API, RRMCM_RMIF_SET_LOG_LEVEL_REQ ^*/

/*! \struct rrmcm_rmif_shutdown_req_t
 *  \brief   This struct send shutdoean request from MIF to CELLM 
 *  \param   time_to_shutdown - time to shutdown
 */
typedef struct rrmcm_rmif_shutdown_req
{
        U16 		          transaction_id;
	rrm_oam_shutdown_mode_et  shutdown_mode;             /*^ M, 0, B, 0, 1 ^*/
        U32                       time_to_shutdown;
}rrmcm_rmif_shutdown_req_t;

/*! \struct rrmcm_rmif_generic_non_cell_specific_api_resp_t 
 *  \brief This struct contains generic response from CELLM to MIF for all API except CELL_CONDFIG. 
 */
typedef struct _rrmcm_rmif_generic_non_cell_specific_api_resp_t
{
	U16 transaction_id;
	U8 response;
    /* coverity : CID 16508*/
	rrm_error_et fail_cause;
}rrmcm_rmif_generic_non_cell_specific_api_resp_t;    /*^ API, RRMCM_RMIF_INIT_CONFIG_RESP, RRMCM_RMIF_LOG_ENABLE_DISABLE_RESP, RRMCM_RMIF_SET_LOG_LEVEL_RESP, RRMCM_RMOF_SHUTDOWN_RESP*/

/* RRMCM_RMIF_CELL_START_REQ */
/*! \struct rrmcm_rmif_cell_start_req_t 
 *  \brief This struct contains cell start API request from MIF to CELLM. 
 */
typedef struct rrmcm_rmif_cell_start
{
    U16 transaction_id;
    rrm_cell_index_t cellindex;
}rrmcm_rmif_cell_start_req_t;


/* RRMCM_RMIF_CELL_STOP_REQ */
/*! \struct rrmcm_rmif_cell_stop_req_t 
 *  \brief This struct contains cell stop API request from MIF TO CELLM.
 */
typedef struct rrmcm_rmif_cell_stop
{
    U16 transaction_id;
    rrm_cell_index_t cellindex;
        /* SPR_17763_FIX_START */
    struct timeval  incoming_time_stamp;
        /* SPR_17763_FIX_END */
}rrmcm_rmif_cell_stop_req_t;

/* RRMCM_RMIF_CELL_DELETE_REQ */
/*! \struct rrmcm_rmif_cell_delete_req_t 
 *  \brief This struct contains cell delete API requset from MIF to CELLM. 
 */
/* SPR 21369 Start */
/* SPR 21632 +- */
typedef struct rrmcm_rmif_cell_delete_req
{
    U8 bitmask; /*^ BITMASK ^*/
    /* SPR 21369 End */
    U16 transaction_id;
    rrm_cell_index_t cellindex;
}rrmcm_rmif_cell_delete_req_t;


/* RRMCM_RMIF_CELL_STOP_ADM_REQ */
/*! \struct rrmcm_rmif_cell_stop_adm_req_t 
 *  \brief This struct contains cell stop admission API requset from MIF to CELLM. 
 */
typedef struct rrmcm_rmif_cell_stop_adm_req
{
    U16 transaction_id;
    rrm_cell_index_t cellindex;
} rrmcm_rmif_cell_stop_adm_req_t;

/* RRMCM_RMIF_CELL_STOP_ADM_REQ_FOR_CELL_BLOCK */
/*! \struct rrmcm_rmif_cell_stop_adm_req_for_cell_block_t 
 *  \brief This struct contains cell stop admission API requset from MIF to CELLM
 *  \for cell block request. 
 */
typedef struct rrmcm_rmif_cell_stop_adm_req_for_cell_block
{
    U16 transaction_id;
    rrm_cell_index_t cellindex;
        /* SPR_17763_FIX_START */
    struct timeval  incoming_time_stamp;
        /* SPR_17763_FIX_END */
} rrmcm_rmif_cell_stop_adm_req_for_cell_block_t;

/* RRMCM_RMIF_CELL_START_ADM_REQ_FOR_CELL_UNBLOCK */
/*! \struct rrmcm_rmif_cell_start_adm_req_for_cell_unblock_t 
 *  \brief This struct contains cell start admission for cell unblock API requset from MIF to CELLM. 
 */
typedef struct rrmcm_rmif_cell_start_adm_req_for_cell_unblock
{
    U16 transaction_id;
    rrm_cell_index_t cellindex;
    /*SPR 21322 START*/
    struct timeval  incoming_time_stamp;
    /*SPR 21322 END*/
} rrmcm_rmif_cell_start_adm_req_for_cell_unblock_t;

/* RRMCM_RMIF_CELL_START_ADM_REQ */
/*! \struct rrmcm_rmif_cell_start_adm_req_t 
 *  \brief This struct contains cell start admission API requset from MIF to CELLM. 
 */
typedef struct rrmcm_rmif_cell_start_adm_req
{
    U16 transaction_id;
    rrm_cell_index_t cellindex;
} rrmcm_rmif_cell_start_adm_req_t;

/* RRMCM_RMIF_GENERIC_RESP */
/*! \struct rrmcm_rmif_generic_resp_t 
 *  \brief This struct contains generic response from CELLM to MIF. 
 */
typedef struct
{
    rrm_cell_index_t cellindex;
    U16 transaction_id;
    U8  response;
    rrm_error_et fail_cause;/* Coverity_ID : 16486 */
}rrmcm_rmif_generic_resp_t;
/*RRMCM_RMIF_CELL_CONFIG_RESP*/
/*! \struct rrmcm_rmif_cell_config_resp 
 *  \brief This struct contains response from CELLM to MIF for CELL_CONFIG API.  
 */
typedef struct _rrmcm_rmif_cell_config_resp
{
    rrm_global_cell_id_t	global_cell_id;
    rrm_cell_index_t cellindex;
    /*SPR21663 fix start*/
    rrm_transaction_id_t  transaction_id;
    /*SPR21663 fix stop*/
    U8 response;
    rrm_error_et fail_cause;/* Coverity_ID : 55835 */
}rrmcm_rmif_cell_config_resp;

/*! \struct ho_params_t 
 *  \brief This struct contains information of Handover related parameters. 
 */
typedef struct _ho_params_t
{
    U16   ho_blind_timer_duration;
    U8    a3_a4_event_trigger_quantity;
    U8    a5_event_trigger_quantity;
    U8    delayed_a3_enabled;
    U8    a4_meas_cfg_enabled;
    U8    b1_event_trigger_quantity;
    U8    b1_threshold_cdma2000;
    U8    b2_event_utran_trigger_quantity;
    U16   csfb_blind_timer_duration;
    U8    b2_event_eutran_thresh_rsrx;
    S8    irat_offset;
    U8    s1_direct_fwd_path_available;
    U8    spid_for_cdma;
    rrm_enable_ue_measurement_et rrm_enable_ue_measurement;
    /*SPR 21365 fix start*/
    /*Code Deleted*/
    /*SPR 21365 fix stop*/
    /*fix 866 start*/
    U8    s_measure;
    /*fix 866 end*/
    /* Bug-10188 Fix: Start */
    U8   utra_rnc_direct_fwd_path_available;
    /* Bug-10188 Fix: End */
    /* SPR 17784 Start */
    U16   cgi_timer_duration;
    /* SPR 17784 End */
}ho_params_t;

/*! \struct rrm_block_cell_params_ho_t
 *  \brief This struct contains parameters used during cell blocking for A4 reports
 */
typedef struct _rrm_block_cell_params_ho_t
{
    /* coverity : CID 16538*/
    rrm_trigger_quantity_et trigger_quantity_for_block;
    U8          a4_threshold_rsrp_for_block;
    U8          a4_threshold_rsrq_for_block;
}rrm_block_cell_params_ho_t;


/*RRMCM_RMIF_RADIO_USAGE_THRESH_REQ*/
/*! \ struct rrmcm_rmif_cell_radio_usage_threshold_req 
 *  \brief This struct contains information of API cell congestion threshold request from MIF to CELLM.   
 */
typedef struct _rrmcm_rmif_cell_radio_usage_thres_req
{
    U8 cell_index;
    U16 transaction_id;
    U64 threshold_overload;
    U64 threshold_high;
    U64 threshold_normal;    
}rrmcm_rmif_cell_radio_usage_thres_req_t;

/* BUG_11317_FIX_START */

/* Uplink power control starts */
/*RRM_OLPC_PUSCH_CONFIG_PARAMS*/
/*! \ struct rrm_olpc_pusch_config_params_t
 *  \brief This struct contains information of open loop power control pusch config params .
 */
typedef struct rrm_olpc_pusch_config_params_t
{
    U8 olpc_pusch_supported_table[RRM_OLPC_CATEGORY_TABLE_SIZE];
    U8 olpc_pusch_snr_threshold_for_cell_center_ue;
    U8 olpc_pusch_snr_threshold_for_cell_edge_ue;
    S8 olpc_p0_ue_pusch_range_start;
    S8 olpc_p0_ue_pusch_range_end;
} rrm_olpc_pusch_config_params_t;

/*RRM_OLPC_PUCCH_CONFIG_PARAMS*/
/*! \ struct rrm_olpc_pucch_config_params_t
 *  \brief This struct contains information of open loop power control pucch config params .
 */
typedef struct rrm_olpc_pucch_config_params_t
{
    U8 olpc_pucch_supported_table[RRM_OLPC_CATEGORY_TABLE_SIZE];
    U8 olpc_pucch_snr_threshold;
    S8 olpc_p0_ue_pucch_range_start;
    S8 olpc_p0_ue_pucch_range_end;
} rrm_olpc_pucch_config_params_t;

/* BUG_11317_FIX_END */

/*RRM_OLPC_CONFIG_PARAMS*/
/*! \ struct rrm_olpc_config_params_t
 *  \brief This struct contains information of open loop power control params .
 */
typedef struct rrm_olpc_config_params_t
{
    rrm_olpc_pusch_config_params_t rrm_olpc_pusch_config_params;
    rrm_olpc_pucch_config_params_t rrm_olpc_pucch_config_params;
    U8 ul_pathloss_threshold;
    /*DYNAMIC ICIC START*/
    U8 ue_handover_count_threshold;
    S8 min_ue_power;
    /*DYNAMIC ICIC END*/
} rrm_olpc_config_params_t;
/* Uplink power control end */

/*DYNAMIC ICIC START*/
typedef struct _rrm_pdsch_dedicated_config_t
{
  U8  p_a_ramping_step;
  U8 pa_for_ce_ue;
}rrm_pdsch_dedicated_config_t;

typedef struct _rrm_pdsch_common_config_t
{
   U8 ref_sig_powr_ramping_step;
}rrm_pdsch_common_config_t;

typedef struct _rrm_pdsch_config_params_t
{
    rrm_pdsch_dedicated_config_t pdsch_dedicated_config;
    rrm_pdsch_common_config_t    pdsch_common_config;
}rrm_pdsch_config_params_t;

/*code changes start for LA feature*/

typedef struct _rrm_bler_schedular_config_t
{
    S32 min_n_samples_threshold;
    U32 window_duration;
    S32 delta_mcs_offset;
}rrm_bler_schedular_config_t;


/*DYNAMIC ICIC END*/

/* RRMCM_RMIF_CELL_CONFIG_REQ */
/* SPR 21632 Start */
/* bitmask which has been set in rrm_cell_context
 * should be defined in this file only. 
 */
/* SPR 21632 End */
#define RRMCM_RMIF_ACCESS_MGMT_PARAMS_PRESENT	0x01
#define RRMCM_RMIF_CELL_CAPACITY_CLASS_PRESENT  0x02
/* SPR 21632 Start */
#define RRMCM_RMIF_INSTANCE_RESET_FLAG_PRESENT	0x04
/* SPR 21632 End */
/*! \struct rrmcm_rmif_cell_config_req_t 
 *  \brief This struct contains information of API cell config request from MIF to CELLM.   
 */
typedef struct rrmcm_rmif_cell_config_req
{
    rrm_bitmask_t           bitmask;
    U16                     transaction_id;
    rrm_global_cell_id_t    global_cell_id;
    ran_t                   ran_info;
    epc_t                   epc_info;
    operator_info_t         operator_info;
    access_mgmt_params_t    access_mgmt_params;
    rrm_bool_et             immediate_start_needed;
    ho_params_t             ho_parameters;
    rrm_block_cell_params_ho_t block_cell_params_ho;
    U8                      dl_sinr_quality_threshold;
    /* uplink mu mimo change start*/
    U8                      uplink_sinr_threshold;
    U8                      uplink_prb_budget_threshold;
    U8                      uplink_prb_budget_down_threshold;
    /* uplink mu mimo change end*/
    /* Uplink power control starts */
    rrm_olpc_config_params_t rrm_olpc_config_params;
    /* Uplink power control end */
    U8 cell_cap_class;
    /*DYNAMIC ICIC START*/
    rrm_pdsch_config_params_t        rrm_pdsch_config_params;
    /*DYNAMIC ICIC END*/
    /* dynamic pdcch start*/
    U8                      dynamic_pdcch;
    /* dynamic pdcch end*/
    /*Rel10 CR 393 changes start*/
    U8  tnl_load_supported_flag;
    U8  hw_load_supported_flag;
    /*Rel10 CR 393 changes end*/
    U8  mac_ecid_meas_start_sfn;
    /* active ue reports configuration timer in second */
    U8 active_ue_reports_timer; 
    /* Fix for CSR: 00059124 start */
    /* SPR 20653 Fix Start */
    /* Code Removed */
    /* SPR 20653 Fix End */
    /* Fix for CSR: 00059124 end */
    /* BUG_11317_FIX_START */
    U8 pusch_bler_threshold;
    /* BUG_11317_FIX_END */
    /* cell_index_from_api_header_start */
    rrm_cell_index_t cell_index;
    /* cell_index_from_api_header_end */
    /* SPR 19309 + */
    U8              cfi_value;    
    /* SPR 19309 - */
    /* SPR 19310 + */
    U16              num_of_eul;    
    U16              size_of_eul;    
    /* SPR 19310 - */
    /* SPR 20908 Start */
    rrm_oam_lower_layer_comm_info_t l2_comm_info_params;      /*^ M, 0, N, 0, 0 ^*/
    /* SPR 20908 End */
}rrmcm_rmif_cell_config_req_t; 

/* RRMCM_RMIF_CELL_RECONFIG_REQ */
#define RRMCM_RMIF_RAN_INFO_PRESENT	                0x01
#define RRMCM_RMIF_EPC_INFO_PRESENT	                0x02
#define RRMCM_RMIF_OPERATOR_INFO_PRESENT	        0x04
#define RRMCM_RMIF_RRM_ACCESS_MGMT_PARAMS_PRESENT	0x08
#define RRMCM_RMIF_RECONFIG_PHY_CELL_PARAMS_PRESENT	0x10
#define RRMCM_RMIF_RECONFIG_CELL_CAPACITY_CLASS_PRESENT 0x20
/*! \struct rrmcm_rmif_cell_reconfig_req_t 
 *  \brief This struct contains information of API cell reconfig request from MIF to CELLM.   
 */
typedef struct rrmcm_rmif_cell_reconfig_req
{
    rrm_bitmask_t           bitmask;
    U16                     transaction_id;
    U8                      cellindex;
    ran_t                   ran_info;
    epc_t                   epc_info;
    operator_info_t         operator_info;
    access_mgmt_params_t    access_mgmt_params;
/*CLPC start*/
	rrm_recfg_phy_cell_parameters_t	recfg_phy_cell_parameters;
/*CLPC stop*/
    U8                      cell_cap_class;
    /* SPS related changes start - Global Variable removal */
    rrm_bool_t             cell_reconfig_sps_on; /* Variable storing sps_on info at the time of Cell_reconfig */
    /* SPS related changes end - Global Variable removal */
    /*Rel10 CR 393 changes start*/
    U8  tnl_load_supported_flag;
    U8  hw_load_supported_flag;
    /*Rel10 CR 393 changes end*/
        /* SPR_17763_FIX_START */
    struct timeval  incoming_time_stamp;
        /* SPR_17763_FIX_END */
}rrmcm_rmif_cell_reconfig_req_t;


/*! \struct rrm_mif_cellm_trigger_type_u 
 *  \brief This union contains information about trigger type received from MIF to CELLM, on getting register request from SON.
 */
typedef union _rrm_mif_cellm_trigger_type
{
    U32  periodic;
    U16  threshold;
    U8   event;
}rrm_mif_cellm_trigger_type_u;

/*! \struct rrm_cellm_trigger_type_t 
 *  \brief This struct contains trigger type received from MIF to CELLM.
 *  \      
 */
typedef struct _rrm_cellm_trigger_type
{   
    rrm_cellm_trigger_type_et     trigger_id;
    rrm_mif_cellm_trigger_type_u  trigger_type; /*bitmask decides the value of this union*/
}rrm_cellm_trigger_type_t;

/*! \struct rrm_cellm_attribute_list_t 
 *  \brief This struct contains attribute list for which CELLM is registered.
 *  \      MIF will receive this attribute list from SON.  
 */
typedef struct _rrm_cellm_attribute_list
{
   rrm_cellm_attribute_id_et id;
   U8 trigger_list;                                  /*zero in case of deregister*/
   rrm_cellm_trigger_type_t trigger_type[RRM_TRGR_LIST_SIZE];/*Number of valid values = trigger_list*/
   U32             value;
}rrm_cellm_attribute_list_t;

/* UE MEAS CHANGES : STARTS */

/* RRMCM_RMIF_CELL_SON_MEAS_CONFIG_REQ */
/*! \struct rrmcm_rmif_cell_son_meas_config_req_t 
 *  \brief This struct contains SON measurement config request from MIF to CELLM.  
 */
typedef struct _rrmcm_rmif_cell_son_meas_config_req
{
    rrm_cell_index_t             cellindex;
    U16                          transaction_id;
    rrm_son_meas_config_req_t    meas_config_req;
}rrmcm_rmif_cell_son_meas_config_req_t;

/*! \struct rrmcm_rmif_cell_son_meas_config_obj_remove_req_t
 *  \brief This struct contains request to remove meas objects recvd from MIF to CELLM.
 */
typedef struct _rrmcm_rmif_cell_son_meas_config_obj_remove_req
{
    rrm_cell_index_t                        cellindex;
    U16                                     transaction_id;
    rrm_son_meas_config_obj_remove_req_t    meas_config_obj_remove_req;
}rrmcm_rmif_cell_son_meas_config_obj_remove_req_t;


/* UE MEAS CHANGES : ENDS */

/* TNL DISCOVERY */
/*! \struct rrmcm_rmif_cell_son_tnl_discovery_req_t
 *  \brief  This structure contains TNL discovery request from mif to cellm.
 */

typedef struct _rrmcm_rmif_cell_son_tnl_discovery_req_t
{
    U16                          transaction_id;
    rrm_son_tnl_discovery_req_t  tnl_discovery_req;
}rrmcm_rmif_cell_son_tnl_discovery_req_t;

typedef struct _rrmcm_rmif_son_tnl_discovery_resp_t 
{
    U16                          transaction_id;
    rrm_son_tnl_discovery_res_t  tnl_discovery_resp;
}rrmcm_rmif_son_tnl_discovery_resp_t;

/* TNL DISCOVERY : ENDS */

/* Spr 16142 Fix Start */
typedef struct _rrmcm_rmif_cell_son_stop_tnl_discovery_ind_t
{
    rrm_global_enb_id_t     target_enb_id;
}rrmcm_rmif_cell_son_stop_tnl_discovery_ind_t;
/* Spr 16142 Fix End */

/* CELL UPDATE: START*/
/* RRMCM_RMIF_CELL_UPDATE_REQ */
#define RRMCM_RMIF_PCI_VALUE_PRESENT                0x01
#define RRMCM_RMIF_UPDATED_PLMN_INFO                0x02
/*! \struct rrmcm_rmif_cell_update_req_t
 *  \brief This struct contains information of API cell update request from MIF to CELLM.
 */
typedef struct rrmcm_rmif_cell_update_req
{
    rrm_bitmask_t                 bitmask;
    U16                           transaction_id;
    rrm_cell_index_t              cellindex;
    U16                           pci_value;
    rrm_oam_updated_plmn_info_t   updated_plmn_info;
}rrmcm_rmif_cell_update_req_t;

/* CELL UPDATE: STOP*/

/* RRMCM_RMIF_CELL_SON_REGISTER_REQ */
/* RRMCM_RMIF_CELL_SON_DEREGISTER_REQ */
/*! \struct rrmcm_rmif_cell_son_register_deregister_req_t 
 *  \brief This struct contains cell register deregister request from MIF to cellm. 
 */
typedef struct _rrmcm_rmif_cell_son_register_deregister_req
{
    rrm_cell_index_t cellindex;
    U16 transaction_id;
    U8 attr_list_size;   
    rrm_cellm_attribute_list_t   attribute_list[RRM_ATTR_LIST_SIZE]; /*Number of valid values = attr_list_size*/
}rrmcm_rmif_cell_son_register_deregister_req_t;


/* RRMCM_RMIF_SON_CELL_SET_ATTR_REQ*/
/*! \struct rrmcm_rmif_cell_set_attr_req 
 *  \brief This struct contains set attribute request from MIF to cellm.
 */
typedef struct _rrmcm_rmif_cell_set_attr_req
{
   rrm_cell_index_t cellindex;
   U16 transaction_id;
   /* SPR 20653 Fix Start */
   U8 attr_list_size;   
   rrm_cellm_attribute_list_t attribute[RRM_ATTR_LIST_SIZE]; /*Number of valid values = attr_list_size*/
   /* SPR 20653 Fix End */
}rrmcm_rmif_cell_set_attr_req;

/* RRMCM_RMIF_CELL_STATE_CHANGE_IND */
/*! \struct rrmcm_rmif_cell_state_change_ind_t 
 *  \brief This struct contain cell state change indication from CELLM to MIF. 
 */
typedef struct _rrmcm_rmif_cell_state_change_ind_t
{
    rrm_cell_index_t cellindex;
    rrm_mif_cellm_cell_operational_state_et cell_state;
    rrm_mif_cellm_cell_state_change_cause_et cell_state_change_cause;
}rrmcm_rmif_cell_state_change_ind_t;

/*RRMCM_RMIF_CELL_ACTIVE_UE_COUNT_REPORT_IND*/
/*! \struct rrmcm_rmif_cell_active_ue_report_ind_t 
 *  \brief This struct contain active ue report indication from CELLM to MIF.
 */
typedef struct _rrmcm_rmif_cell_active_ue_report_ind
{
    rrm_cell_index_t cellindex;
    /* +- SPR 18268 */
    U16 ue_cnt;
    /* +- SPR 18268 */
}rrmcm_rmif_cell_active_ue_report_ind_t;

/*RRMCM_RMIF_CELL_ACTIVE_THRESHOLD_REACHED_REPORT_IND*/
/*! \struct rrm_ue_count_threshold_reached_ind_t 
 *  \brief This struct contains threshold count indication from CELLM to MIF. 
 */
typedef struct _rrm_ue_count_threshold_reached_ind
{
     rrm_cell_index_t cellindex;
     rrm_cellm_threshold_hit_cause_et      cause;
}rrm_ue_count_threshold_reached_ind_t;

/* RRMCM_RMIF_NMM_PREPARE_REQ */
/*! \struct rrmcm_rmif_nmm_prepare_req_t 
 *  \brief This struct contains information about NMM prepare request.  
 */
typedef struct _rrmcm_rmif_nmm_prepare_req_t
{
    U16 transaction_id;
} rrmcm_rmif_nmm_prepare_req_t;

/* RRMCM_RMIF_NMM_COMPLETE_REQ */
/*! \struct rrmcm_rmif_nmm_complete_req_t 
 *  \brief This struct contains information about NMM complete request. 
 */
typedef struct _rrmcm_rmif_nmm_complete_req_t
{
    U16 transaction_id;
} rrmcm_rmif_nmm_complete_req_t;

/* RRMCM_RMIF_NMM_PREPARE_RESP */
/*! \struct rrmcm_rmif_nmm_prepare_resp_t 
 *  \brief This struct contains information about NMM prepare response from CELLM to MIF. 
 */
typedef struct _rrmcm_rmif_nmm_prepare_resp_t
{
    U16 transaction_id;
    U8  response;
    U16  fail_cause;
} rrmcm_rmif_nmm_prepare_resp_t;

/* RRMCM_RMIF_NMM_COMPLETE_RESP */
/*! \struct rrmcm_rmif_nmm_complete_resp_t 
 *  \brief This struct contains information about NMM complete response from CELLM to MIF.
 */
typedef struct _rrmcm_rmif_nmm_complete_resp_t
{
    U16 transaction_id;
    U8  response;
    U16  fail_cause;
} rrmcm_rmif_nmm_complete_resp_t;

#define RRMCM_RMIF_TOTAL_PRB_USAGE_IN_DL_PRESENT                0x0001
#define RRMCM_RMIF_TOTAL_PRB_USAGE_IN_UL_PRESENT                0x0002
#define RRMCM_RMIF_TOTAL_PRB_USAGE_PER_CQI_IN_DL_PRESENT        0x0004
#define RRMCM_RMIF_TOTAL_PRB_USAGE_PER_CQI_IN_UL_PRESENT        0x0008
#define RRMCM_RMIF_NUM_OF_ACTIVE_UE_PER_CQI_IN_DL_PRESENT       0x0010
#define RRMCM_RMIF_NUM_OF_ACTIVE_UE_PER_CQI_IN_UL_PRESENT       0x0020
#define RRMCM_RMIF_PACKET_DELAY_PER_CQI_IN_DL_PRESENT           0x0040
#define RRMCM_RMIF_RCVD_RAND_ACCESS_PREAMBLE_CONTENTION_FREE_PRESENT   0x0080
#define RRMCM_RMIF_RCVD_RAND_ACCESS_PREAMBLE_GROUP_A_PRESENT           0x0100
#define RRMCM_RMIF_RCVD_RAND_ACCESS_PREAMBLE_GROUP_B_PRESENT           0x0200
#define RRMCM_RMIF_TOTAL_PACKET_ADDEDPER_QCI_IN_DL_PRESENT             0x0400
#define RRMCM_RMIF_DURATION_TTI_PRESENT                                0x0800

/*! \struct rrmcm_rmif_prb_usage_update_t 
 *  \brief This struct contains information about PRB usage. 
 */
typedef struct rrmcm_rmif_prb_usage_update
{
	rrm_bitmask_t              bitmask;
	/*NR*/
	U16 			   transation_id;
	/*NR*/
	rrm_global_cell_id_t       global_cell_id;
        U32                        durationT;
	U64                        total_prb_usage_in_dl;
	U64                        total_prb_usage_in_ul;
	U64                        total_prb_usage_per_qci_in_dl[RRM_MAX_QCI];
	U64                        total_prb_usage_per_qci_in_ul[RRM_MAX_QCI];
	U64                        num_active_ue_per_qci_in_dl[RRM_MAX_QCI];
	U64                        num_active_ue_per_qci_in_ul[RRM_MAX_QCI];
	U64                        packet_delay_per_qci_in_dl[RRM_MAX_QCI];
	U64                        rcvd_random_access_preambles_contention_free;
	U64                        rcvd_random_access_preambles_group_a;
	U64                        rcvd_random_access_preambles_group_b;
	U64                        total_packet_acked_per_qci_in_dl[RRM_MAX_QCI];
	U32                        duration_tti;
} rrmcm_rmif_prb_usage_update_t;

/*! \struct rrmcm_rmif_rac_enable_disable_req_t 
 *  \brief This struct contains RAC enable disable request from MIF to CELLM.
 */
typedef struct _rrmcm_rmif_rac_enable_disable_req_t
{
	U16	             transaction_id;    /*!< Transaction Id */
	rrm_cell_index_t cell_index;        /*!< Cell Index */		
	rrm_bool_et      enable_rac;        /*!< RAC Functionality Enable Flag */
	rrm_bitmask_t    bitmask;           /*!< cell index present or not */ 
	rrm_bool_et is_cell_index_present;  /*!< Is cell index present */
}rrmcm_rmif_rac_enable_disable_req_t;   /*^ API, RRMUEM_RMIF_RAC_ENABLE_DISABLE_REQ ^*/

/*! \struct rrmcm_rmif_rac_enable_disable_res_t 
 *  \brief this struct contains information about RAC enable disable response from CELLM to MIF.
 */
typedef struct _rrmcm_rmif_rac_enable_disable_res_t
{
	U16	             transaction_id;    /*!< Transaction Id */
	rrm_cell_index_t cell_index;		/*!< Cell Index */		
    U8               response;          /*!< RAC Functionality Enable/Disable Response */
    U16               fail_cause;        /*!< Error Code */
	rrm_bitmask_t    bitmask;           /*!<  Btimask as received from OAM */
	rrm_bool_et is_cell_index_present;  /*!< is cell index present */
}rrmcm_rmif_rac_enable_disable_res_t;   /*^ API, RRMUEM_RMIF_RAC_ENABLE_DISABLE_RES ^*/


/*! \struct rrmcm_rmif_cell_context_print_req_t 
 *  \brief This struct contains information for print cell context from MIF to cellm.
 */
typedef struct _rrmcm_rmif_cell_context_print_req_t
{
        U8                      cell_index;    /*^ M, 0, H, 0, 0 ^*/
}rrmcm_rmif_cell_context_print_req_t;
typedef enum 
{
    RRMCM_PERIODICTY_FACTOR_N2,
    RRMCM_PERIODICTY_FACTOR_N4
}rrmcm_subband_cqi_periodcity_factor_et;

/* UPDATED NRT INFO CHANGES: START */

/*! \enum rrmcm_rmif_geran_band_indicator_et
 *  \brief An enum that specifies bands for GERAN carrier ARFCNs
 */
typedef enum 
{
  RRM_GERAN_DCS_1800,
  RRM_GERAN_PCS_1900
}rrmcm_rmif_geran_band_indicator_et;

#define RRMCM_RMIF_INTRA_RAT_NR_INFO_OFFSET_PRESENT         0x01
#define RRMCM_RMIF_INTRA_RAT_NR_INFO_X2_STATUS_PRESENT      0x02
#define RRMCM_RMIF_INTRA_RAT_NR_INFO_TAC_PRESENT            0x04
#define RRMCM_RMIF_INTRA_RAT_NR_INFO_CM_OFFSET_PRESENT    0x08
#define RRMCM_RMIF_INTRA_RAT_NR_INFO_ACCESS_MODE_PRESENT  0x10
#define RRMCM_RMIF_INTRA_RAT_NR_INFO_BROADCAST_STATUS     0x20
#define RRMCM_RMIF_INTRA_RAT_NR_INFO_CSG_IDENTITY         0x40
#ifdef LTE_EMBMS_SUPPORTED
#define RRMCM_RMIF_INTRA_RAT_MBSFN_INFO_PRESENT           0x80
/*SPR 18605:Fix Start*/
#define RRMCM_RMIF_INTRA_TDD_SF_ASSN_INFO_PRESENT         0x100
/*SPR 18605:Fix End*/
#endif
/*! \ struct rrmcm_rmif_intra_rat_neighbor_cell_t
 *  \brief This is neighbor cell information sent to RRM
 *  \param pci                    Physical cell id
 *  \param cell_specific_offset   Cell Specific Offset
 */
typedef struct _rrmcm_rmif_intra_rat_neighbor_cell_t
{
    rrm_bitmask_t                       bitmask;    /*^ BITMASK ^*/
    rrm_oam_eutran_global_cell_id_t     cgi;
    /*^ M, 0, N, 0, 0 ^*/
    U16                                 pci;        
    /*^ M, 0, H, 0, 503 ^*/
    U32                                 cell_specific_offset;    
    /*^ O, RRMCM_RMIF_INTRA_RAT_NR_INFO_OFFSET_PRESENT, H, 0, 30 ^*/
    rrmcm_rmif_cell_x2_status_et   		x2_status;
    /*^ O, RRMCM_RMIF_INTRA_RAT_NR_INFO_X2_STATUS_PRESENT, B, 1, 2 ^*/
    U8                                  tac[MAX_TAC_SIZE];
    /*^ O, RRMCM_RMIF_INTRA_RAT_NR_INFO_TAC_PRESENT, N, 0, 0 ^*/
    U32                                 cm_cell_specific_offset;
    /*^ O, RRMCM_RMIF_INTRA_RAT_NR_INFO_CM_OFFSET_PRESENT, H, 0, 30 ^*/
    rrm_oam_access_mode_et              access_mode;
    /*^ O, RRMCM_RMIF_INTRA_RAT_NR_INFO_ACCESS_MODE_PRESENT, H, 0, 2 ^*/
    rrm_bool_et                         broadcast_status;
    /*^ O, RRMCM_RMIF_INTRA_RAT_NR_INFO_BROADCAST_STATUS, H, 0, 1 ^*/
    U8                                  csg_identity[RRM_OAM_MAX_NUM_CSG_OCTETS];
    /*^ O, RRMCM_RMIF_INTRA_RAT_NR_INFO_CSG_IDENTITY, N, 0, 0 ^*/
    /*SPR 14209 start*/
    rrm_oam_intf_scheme_type_et       interference_scheme;
    /*SPR 14209 end*/
    
#ifdef LTE_EMBMS_SUPPORTED
    rrmcm_mbsfn_subframe_config_list_t mbsfn_config_info; /* RRMCM_RMIF_INTRA_RAT_MBSFN_INFO_PRESENT */
    U8                                 subframe_assn ;  /* RRMCM_RMIF_INTRA_TDD_SF_ASSN_INFO_PRESENT */ 
#endif
}rrmcm_rmif_intra_rat_neighbor_cell_t;

/*! \struct rrmcm_rmif_intra_rat_neighbor_list_t 
 *  \brief This struct provides intra rat neighbour list information. 
 */
typedef struct _rrmcm_rmif_intra_rat_neighbor_list_t
{
    U32                                     earfcn;     /*^ M, 0, N, 0, 0 ^*/
    U16                                     cell_list_size;   /*^ M, 0, H, 0, 64 ^*/
    rrmcm_rmif_intra_rat_neighbor_cell_t    cell_list[RRM_MAX_NO_CELLS_PER_EARFCN];  
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrmcm_rmif_intra_rat_neighbor_list_t;

#define RRM_MAX_NO_EARFCN       (MAX_NO_INTER_FREQ + 1) 
/*! \struct rrmcm_rmif_intra_rat_neighbor_cell_list_t 
 *  \brief This struct provides intra rat neighbour cell list information
 */
typedef struct _rrmcm_rmif_intra_rat_neighbor_cell_list_t
{
    U16                                       intra_rat_cells_to_add_mod_list_size;  
    /*^ M, 0, H, 0, 100 ^*/
    rrmcm_rmif_intra_rat_neighbor_list_t      intra_rat_cells_to_add_mod_list[RRM_MAX_NO_EARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                       intra_rat_cells_to_remove_list_size;   
    /*^ M, 0, H, 0, 100 ^*/
    rrmcm_rmif_intra_rat_neighbor_list_t      intra_rat_cells_to_remove_list[RRM_MAX_NO_EARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                       intra_rat_black_list_cells_to_add_mod_list_size;   
    /*^ M, 0, H, 0, 100 ^*/
    rrmcm_rmif_intra_rat_neighbor_list_t      intra_rat_black_list_cells_to_add_mod_list[RRM_MAX_NO_EARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                       intra_rat_black_list_cells_to_remove_list_size;    
    /*^ M, 0, H, 0, 100 ^*/
    rrmcm_rmif_intra_rat_neighbor_list_t      intra_rat_black_list_cells_to_remove_list[RRM_MAX_NO_EARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

}rrmcm_rmif_intra_rat_neighbor_cell_list_t;

#define RRMCM_RMIF_UTRAN_NR_INFO_OFFSET_PRESENT         0x01
#define RRMCM_RMIF_UTRAN_NR_INFO_ACCESS_MODE_PRESENT    0x02
#define RRMCM_RMIF_UTRAN_NR_INFO_LAC_PRESENT            0x04
#define RRMCM_RMIF_UTRAN_NR_INFO_RAC_PRESENT            0x08
#define RRMCM_RMIF_UTRAN_NR_INFO_CSG_IDENTITY_PRESENT   0x10
/* SRVCC SUPPORT START */
#define RRMCM_RMIF_UTRAN_NR_INFO_PS_HO_SUPPORTED        0x20
#define RRMCM_RMIF_UTRAN_NR_INFO_VOIP_CAPABLE           0x40
/* SRVCC SUPPORT END */
/*! \  struct rrmcm_rmif_utran_neighbor_cell_t
 *  \brief This is neighbor information sent to RRM
 *  \param pci                    Physical cell id
 *  \param cell_specific_offset   Cell Specific Offset
 */
typedef struct _rrmcm_rmif_utran_neighbor_cell_t
{
    rrm_bitmask_t                           bitmask;    /*^ BITMASK ^*/
    rrm_son_utran_geran_global_cell_id_t    cgi;
    /*^ M, 0, N, 0, 0 ^*/
    rrmcm_rmif_utran_physical_cell_id_t     pci;
    /*^ M, 0, N, 0, 0 ^*/
    S8                                  cell_specific_offset;
    /*^ O, RRMCM_RMIF_UTRAN_NR_INFO_OFFSET_PRESENT, B, -15, 15 ^*/
    rrm_oam_access_mode_et              access_mode;
    /*^ O, RRMCM_RMIF_UTRAN_NR_INFO_ACCESS_MODE_PRESENT, H, 0, 2 ^*/
    U8                                  lac[RRM_OAM_MAX_NUM_LAC_OCTETS];
    U8                                  rac;
    U8                                  csg_identity[RRM_OAM_MAX_NUM_CSG_OCTETS];
    /* SRVCC SUPPORT START */
    rrm_bool_et                         ps_ho_supported;
    rrm_bool_et                         voip_capable;
    /* SRVCC SUPPORT END */
}rrmcm_rmif_utran_neighbor_cell_t;

/*! \  struct rrmcm_rmif_utran_neighbor_list_t
 *  \brief  Structure containing neighbor cell list on a particular earfcn
 *  \param  uarfcn          Carrier Frequency
 *  \param  cell_list_size  Cell List Size
 *  \param  cell_list       Cell List
 */
typedef struct _rrmcm_rmif_utran_neighbor_list_t
{
    U16                                 uarfcn;     /*^ M, 0, H, 0, 16383 ^*/
    U16                                 cell_list_size;   /*^ M, 0, H, 0, 128 ^*/
    rrmcm_rmif_utran_neighbor_cell_t    cell_list[RRM_MAX_NUM_EARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrmcm_rmif_utran_neighbor_list_t;

/*! \  struct rrmcm_rmif_utran_neighbor_cell_list_t
 *  \brief Structure containing utran neighbor cell list information
 *  \param  cells_to_add_mod_list_size                Size of list of added or modified neighbors
 *  \param  cells_to_add_mod_list                     List of added or modified neighbors
 *  \param  cells_to_remove_list_size                 Size of list of removed neighbors
 *  \param  cells_to_remove_list                      List of removed neighbors
 *  \param  black_list_cells_to_add_mod_list_size     Size of list of added or modified blacklisted neighbors
 *  \param  black_list_cells_to_add_mod_list          List of added or modified blacklisted neighbors
 *  \param  black_list_cells_to_remove_list_size      Size of list of removed  blacklisted neighbors
 *  \param  black_list_cells_to_remove_list           List of removed  blacklisted neighbors
 */
typedef struct _rrmcm_rmif_utran_neighbor_cell_list_t
{
    U16                                       cells_to_add_mod_list_size;
    /*^ M, 0, H, 0, 100 ^*/
    rrmcm_rmif_utran_neighbor_list_t          cells_to_add_mod_list[RRM_MAX_NUM_UTRA_CARRIERS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                       cells_to_remove_list_size;
    /*^ M, 0, H, 0, 100 ^*/
    rrmcm_rmif_utran_neighbor_list_t          cells_to_remove_list[RRM_MAX_NUM_UTRA_CARRIERS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                       black_list_cells_to_add_mod_list_size;
    /*^ M, 0, H, 0, 100 ^*/
    rrmcm_rmif_utran_neighbor_list_t          black_list_cells_to_add_mod_list[RRM_MAX_NUM_UTRA_CARRIERS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                       black_list_cells_to_remove_list_size;
    /*^ M, 0, H, 0, 100 ^*/
    rrmcm_rmif_utran_neighbor_list_t          black_list_cells_to_remove_list[RRM_MAX_NUM_UTRA_CARRIERS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

}rrmcm_rmif_utran_neighbor_cell_list_t;

/* SPR 15393 Fix Start*/
#define RRMCM_RMIF_GERAN_NR_INFO_OFFSET_PRESENT    0x01
#define RRMCM_RMIF_GERAN_RAC_PRESENT               	    0x02
/* SPR-10489-fix start */
#define RRMCM_RMIF_GERAN_NR_INFO_DTM_SUPPORTED_PRESENT  0x04    
#define RRMCM_RMIF_GERAN_NR_INFO_ACCESS_MODE_PRESENT    0x08
#define RRMCM_RMIF_GERAN_NR_INFO_CSG_IDENTITY_PRESENT   0x10
/* SPR-10489-fix end */
/*! \  struct rrmcm_rmif_geran_neighbor_cell_t
 *  \brief This is neighbor information sent to RRM
 *  \param pci                    Physical cell id
 *  \param cell_specific_offset   Cell Specific Offset
 */
typedef struct _rrmcm_rmif_geran_neighbor_cell_t
{
    rrm_bitmask_t                           bitmask;    /*^ BITMASK ^*/
    rrm_son_utran_geran_global_cell_id_t    cgi;
    /*^ M, 0, N, 0, 0 ^*/
    rrmcm_rmif_geran_physical_cell_id_t     pci;
    /*^ M, 0, N, 0, 0 ^*/
    S8                                  cell_specific_offset;
    /*^ O, RRMCM_RMIF_GERAN_NR_INFO_OFFSET_PRESENT, B, -15, 15 ^*/
    U8                           rac;
    /*^ O, RRMCM_RMIF_GERAN_RAC_PRESENT, N, 0, 0 ^*/
    /* SPR-10489-fix start */
    U8                                  dtm_supported;
    /*^ O, RRMCM_RMIF_GERAN_NR_INFO_DTM_SUPPORTED_PRESENT, H, 0, 1 ^*/
    /* SPR-10489-fix end */
    rrm_oam_access_mode_et              access_mode;
    /*^ O, RRMCM_RMIF_GERAN_NR_INFO_ACCESS_MODE_PRESENT, H, 0, 2 ^*/
    U8                                  csg_identity[RRM_OAM_MAX_NUM_CSG_OCTETS];
}rrmcm_rmif_geran_neighbor_cell_t;
/* SPR 15393 Fix Stop*/

/*! \  struct rrmcm_rmif_geran_neighbor_list_t
 *  \brief  Structure containing neighbor cell list on a particular earfcn
 *  \param  uarfcn          Carrier Frequency
 *  \param  cell_list_size  Cell List Size
 *  \param  cell_list       Cell List
 */
typedef struct _rrmcm_rmif_geran_neighbor_list_t
{
    U16                                 arfcn;
    /*^ M, 0, H, 0, 1023 ^*/
    rrmcm_rmif_geran_band_indicator_et  band_ind;
    /*^ M, 0, H, 0, 1 ^*/
    U16                                 cell_list_size;   /*^ M, 0, H, 0, 128 ^*/
    rrmcm_rmif_geran_neighbor_cell_t    cell_list[RRM_MAX_NUM_EARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrmcm_rmif_geran_neighbor_list_t;

/*! \  struct rrmcm_rmif_geran_neighbor_cell_list_t
 *  \brief Structure containing geran neighbor cell list information
 *  \param  cells_to_add_mod_list_size                Size of list of added or modified neighbors
 *  \param  cells_to_add_mod_list                     List of added or modified neighbors
 *  \param  cells_to_remove_list_size                 Size of list of removed neighbors
 *  \param  cells_to_remove_list                      List of removed neighbors
 *  \param  black_list_cells_to_add_mod_list_size     Size of list of added or modified blacklisted neighbors
 *  \param  black_list_cells_to_add_mod_list          List of added or modified blacklisted neighbors
 *  \param  black_list_cells_to_remove_list_size      Size of list of removed  blacklisted neighbors
 *  \param  black_list_cells_to_remove_list           List of removed  blacklisted neighbors
 */
typedef struct _rrmcm_rmif_geran_neighbor_cell_list_t
{
    U16                                cells_to_add_mod_list_size;
    /*^ M, 0, H, 0, 100 ^*/
    rrmcm_rmif_geran_neighbor_list_t   cells_to_add_mod_list[MAX_NO_GERAN_FREQ_GROUUP];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                cells_to_remove_list_size;
    /*^ M, 0, H, 0, 100 ^*/
    rrmcm_rmif_geran_neighbor_list_t   cells_to_remove_list[MAX_NO_GERAN_FREQ_GROUUP];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                black_list_cells_to_add_mod_list_size;
    /*^ M, 0, H, 0, 100 ^*/
    rrmcm_rmif_geran_neighbor_list_t   black_list_cells_to_add_mod_list[MAX_NO_GERAN_FREQ_GROUUP];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                black_list_cells_to_remove_list_size;
    /*^ M, 0, H, 0, 100 ^*/
    rrmcm_rmif_geran_neighbor_list_t   black_list_cells_to_remove_list[RRM_MAX_NO_EARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

}rrmcm_rmif_geran_neighbor_cell_list_t;



#define RRM_SON_CDMA_CELL_GLOBAL_ID_1X_RTT_SIZE   6
#define RRM_SON_CDMA_CELL_GLOBAL_ID_HRPD_SIZE     16

#define RRM_SON_CDMA_CGI_1X_RTT_PRESENT 0x01
#define RRM_SON_CDMA_CGI_HRPD_PRESENT 0x02
/*! \enum  rrm_son_access_mode_et
 *  brief An enum that indicates ACCESS mode of the cell.
 *   */
typedef enum
{
    RRM_SON_CLOSED,
    RRM_SON_HYBRID,
    RRM_SON_OPEN
}rrm_son_access_mode_et;


typedef struct _rrmcm_rmif_cdma_global_cell_id_t
{
    U32                               bitmask; /*^ BITMASK ^*/
    U8                 cell_global_id_1X_RTT [RRM_SON_CDMA_CELL_GLOBAL_ID_1X_RTT_SIZE];
    /*^ O, RRM_SON_CDMA_CGI_1X_RTT_PRESENT, OCTET_STRING, FIXED ^*/
    U8                 cell_global_id_HRPD [RRM_SON_CDMA_CELL_GLOBAL_ID_HRPD_SIZE];
    /*^ O, RRM_SON_CDMA_CGI_HRPD_PRESENT, OCTET_STRING, FIXED ^*/
}rrmcm_rmif_cdma_global_cell_id_t;


#define RRM_SON_CDMA_NBR_CELL_HRPD_CSO      0x01
/* Start Bug Fix 789 */
#define RRM_SON_CDMA_HRPD_CELL_SPECIFIC_PARAMS_PRESENT 0x02
typedef struct _rrmcm_rmif_cdma_neighbor_cell_hrpd_t
{
    U32                          bitmask;   /*^ BITMASK ^*/
    U16                         pci;
     /*^ M, 0, N, 0, 0 ^*/
    rrmcm_rmif_cdma_global_cell_id_t    cgi;
    /*^ M, 0, N, 0, 0 ^*/
    U16                          arfcn;
    /*^ M, 0, N, 0, 0 ^*/
    S8                           cell_specific_offset;
    /*^ O, RRM_SON_CDMA_NBR_CELL_HRPD_CSO, B, -15, 15 ^*/
    rrmcm_rmif_cell_specific_params_t     cell_specific_params;
    /*^ O, RRM_SON_CDMA_HRPD_CELL_SPECIFIC_PARAMS_PRESENT, N, 0, 0 ^*/
/* End Bug Fix 789 */
}rrmcm_rmif_cdma_neighbor_cell_hrpd_t;

#define RRM_SON_CDMA_HRPD_NBR_SIZE      512

typedef struct _rrmcm_rmif_cdma_neighbor_list_hrpd_t
{
    U16     band_class;
    /*^ M, 0, H, 0, 1 ^*/
    U16     cell_list_size;
    /*^ M, 0, H, 0, 512 ^*/
    rrmcm_rmif_cdma_neighbor_cell_hrpd_t    cell_list[RRM_SON_CDMA_HRPD_NBR_SIZE];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrmcm_rmif_cdma_neighbor_list_hrpd_t;


#define RRM_SON_CDMA_NBR_CELL_XRTT_CSO      0x01
/* Start Bug Fix 789 */
#define RRM_SON_CDMA_XRTT_CELL_SPECIFIC_PARAMS_PRESENT 0x02
typedef struct _rrmcm_rmif_cdma_neighbor_cell_xrtt_t
{
    U32                          bitmask;   /*^ BITMASK ^*/
    U16                          pci;
     /*^ M, 0, N, 0, 0 ^*/
    rrmcm_rmif_cdma_global_cell_id_t    cgi;
    /*^ M, 0, N, 0, 0 ^*/
    U16                          arfcn;
    /*^ M, 0, N, 0, 0 ^*/
    S8                           cell_specific_offset;
    /*^ O, RRM_SON_CELL_XRTT_CSO, B, -15, 15 ^*/
    rrmcm_rmif_cell_specific_params_t     cell_specific_params;
    /*^ O, RRM_SON_CDMA_XRTT_CELL_SPECIFIC_PARAMS_PRESENT, N, 0, 0 ^*/
/* End Bug Fix 789 */
}rrmcm_rmif_cdma_neighbor_cell_xrtt_t;


#define RRM_SON_CDMA_XRTT_NBR_SIZE      640
typedef struct _rrmcm_rmif_cdma_neighbor_list_xrtt_t
{
    U16     band_class;
    /*^ M, 0, H, 0, 1 ^*/
    U16     cell_list_size;
    /*^ M, 0, H, 0, 640 ^*/
    rrmcm_rmif_cdma_neighbor_cell_xrtt_t    cell_list[RRM_SON_CDMA_XRTT_NBR_SIZE];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrmcm_rmif_cdma_neighbor_list_xrtt_t;

#define RRM_SON_CDMA_MAX_NO_ARFCN 16
typedef struct _rrmcm_rmif_cdma_neighbor_cell_list_hrpd_t
{
    U16                                        cells_to_add_mod_list_size;
    /*^ M, 0, H, 0, 16 ^*/
    rrmcm_rmif_cdma_neighbor_list_hrpd_t       cells_to_add_mod_list[RRM_SON_CDMA_MAX_NO_ARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                        cells_to_remove_list_size;
    /*^ M, 0, H, 0, 16 ^*/
    rrmcm_rmif_cdma_neighbor_list_hrpd_t       cells_to_remove_list[RRM_SON_CDMA_MAX_NO_ARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                        black_list_cells_to_add_mod_list_size;
    /*^ M, 0, H, 0, 16 ^*/
    rrmcm_rmif_cdma_neighbor_list_hrpd_t       black_list_cells_to_add_mod_list[RRM_SON_CDMA_MAX_NO_ARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                        black_list_cells_to_remove_list_size;
    /*^ M, 0, H, 0, 16 ^*/
    rrmcm_rmif_cdma_neighbor_list_hrpd_t          black_list_cells_to_remove_list[RRM_SON_CDMA_MAX_NO_ARFCN];

}rrmcm_rmif_cdma_neighbor_cell_list_hrpd_t;


typedef struct _rrmcm_rmif_cdma_neighbor_cell_list_xrtt_t
{
    U16                                      cells_to_add_mod_list_size;
    /*^ M, 0, H, 0, 16 ^*/
    rrmcm_rmif_cdma_neighbor_list_xrtt_t     cells_to_add_mod_list[RRM_SON_CDMA_MAX_NO_ARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                      cells_to_remove_list_size;
    /*^ M, 0, H, 0, 16 ^*/
    rrmcm_rmif_cdma_neighbor_list_xrtt_t     cells_to_remove_list[RRM_SON_CDMA_MAX_NO_ARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                      black_list_cells_to_add_mod_list_size;
    /*^ M, 0, H, 0, 16 ^*/
    rrmcm_rmif_cdma_neighbor_list_xrtt_t     black_list_cells_to_add_mod_list[RRM_SON_CDMA_MAX_NO_ARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                                      black_list_cells_to_remove_list_size;
    /*^ M, 0, H, 0, 16 ^*/
    rrmcm_rmif_cdma_neighbor_list_xrtt_t     black_list_cells_to_remove_list[RRM_SON_CDMA_MAX_NO_ARFCN];

}rrmcm_rmif_cdma_neighbor_cell_list_xrtt_t;


#define RRMCM_RMIF_UTRAN_NR_LIST_PRESENT    0x01
#define RRMCM_RMIF_GERAN_NR_LIST_PRESENT    0x02

#define RRMCM_RMIF_CDMA_NR_LIST_XRTT_PRESENT   0x04
#define RRMCM_RMIF_CDMA_NR_LIST_HRPD_PRESENT   0x08
/*! \struct rrmcm_rmif_inter_rat_neighbor_cell_list_t 
 *  \brief  This Struct gives inter rat neighbour list of a cell
 */
typedef struct _rrmcm_rmif_inter_rat_neighbor_cell_list_t
{
    rrm_bitmask_t            bitmask; /*^ BITMASK ^*/  
    rrmcm_rmif_utran_neighbor_cell_list_t      utran_neighbor_list;
    /*^ O, RRMCM_RMIF_UTRAN_NR_LIST_PRESENT, N, 0, 0 ^*/
    rrmcm_rmif_geran_neighbor_cell_list_t      geran_neighbor_list;
    /*^ O, RRMCM_RMIF_GERAN_NR_LIST_PRESENT, N, 0, 0 ^*/
    rrmcm_rmif_cdma_neighbor_cell_list_xrtt_t       cdma_neighbor_list_xrtt;
    /*^ O, RRMCM_RMIF_CDMA_NR_LIST_XRTT_PRESENT, N, 0, 0 ^*/
    rrmcm_rmif_cdma_neighbor_cell_list_hrpd_t       cdma_neighbor_list_hrpd;
    /*^ O, RRMCM_RMIF_CDMA_NR_LIST_HRPD_PRESENT, N, 0, 0 ^*/

}rrmcm_rmif_inter_rat_neighbor_cell_list_t;

/*
 * RRMCM_RMIF_UPDATED_NRT_INFO
 */

/*! \struct rrmcm_rmif_updated_nrt_info_t
 *  \brief It gives updated Neighbour Relation Table information.
 *  \param src_cgi          CGI of source cell
 *  \param intra_rat_ncl    Updated list of neighbours and its attribute
 *                          values of source cell id.
 *                          Max list size is 255.
 *  \param inter_rat_ncl    Updated list of neighbours and its attribute
 *                          values of source cell id.
 *                          Max list size is 255.
 */
typedef struct _rrmcm_rmif_updated_nrt_info_t
{
	U16	                                            transaction_id;    /*!< Transaction Id */
    U8                                              cellindex;
    rrm_oam_eutran_global_cell_id_t                        src_cgi; 
    /*^ M, 0, N, 0, 0 ^*/
    rrmcm_rmif_intra_rat_neighbor_cell_list_t       intra_rat_ncl;  
    /*^ M, 0, N, 0, 0 ^*/
    rrmcm_rmif_inter_rat_neighbor_cell_list_t       inter_rat_ncl;  
    /*^ M, 0, N, 0, 0 ^*/
        /* SPR_17763_FIX_START */
    struct timeval  incoming_time_stamp;
        /* SPR_17763_FIX_END */
}rrmcm_rmif_updated_nrt_info_t;    /*^ API, RRMCM_RMIF_UPDATED_NRT_INFO ^*/

/* UPDATED NRT INFO CHANGES: END */


/* RRM_MAC_RECONF_SCHEDULER_CHANGES_START */
typedef struct rrmcm_rmif_CqiToPHICHPowerT
{
 /* Contains the mapping of wideband CQI to delta PHICH power offset to be
 ** used on top of configured PHICH Power. Wideband CQI(range 1 to 15) is
 ** used as index to array cqiToPHICHPowerInfo to get the delta PHICH Power
 ** offset in the range 0 to 10000 power offset.
 ** Range(0,1,2,...,9998,9999,10000) representing -6 dB to 4 dB in
 ** 0.001 dB steps such that total PHICH power is in range 0 to 10000.
 **/
    U32 cqiToPHICHPowerInfo[RRM_MAX_CQI_VAL];
} rrmcm_rmif_CqiToPHICHPower;


typedef struct rrmcm_rmif_AggregationPowerOffsetInfoT
{
/* This field shall indicate the aggregation level to be used for
 ** CCE allocation. Range(1,2,4 or 8).
 **/
    U32   aggregationLevel;
/* This field shall indicate the value of delta power offset to be used for
 ** PDCCH on top of configured PDCCH power offset.
 ** Range(0,1,2,...,9998,9999,10000) representing -6 dB to 4 dB in
 ** 0.001 dB steps.
 **/
    U32  powerOffset;
}rrmcm_rmif_AggregationPowerOffsetInfo;

typedef struct rrmcm_rmif_AggregationPowerOffsetT
{
/* This field shall indicate the number of possible tuples
 ** that can be tried for aggregationLevel/powerOffset sets for
 ** PDCCH power control. Range(1,2,3,4)
 **/
   U32 count;
/* This is an array of the type AggregationPowerOffsetInfo  which
 ** shall store "n" tuples where n is given by count.
 **/
    rrmcm_rmif_AggregationPowerOffsetInfo  \
        aggregationPowerOffsetTuples[RRM_MAX_AGGREGATION_LEVEL_POWER_OFFSET_COUNT];
}rrmcm_rmif_AggregationPowerOffset;


#define RRMCM_RMIF_AGGRERATION_CELL_CENTER_PRESENT             0x0001
#define RRMCM_RMIF_AGGRERATION_CELL_EDGE_PRESENT               0x0002
typedef struct rrmcm_rmif_PdcchAggregationAndPowerOffsetInfoT
{
    U32     bitmask;
/* aggregationPowerOffset two dimensional array specifies the aggregation
 ** level and power offset tuples based on DCI category size(either of
 ** categories SMALL_DCI_STEP_SIZE, MEDIUM_DCI_STEP_SIZE and
 ** BIG_DCI_STEP_SIZE) and wideband CQI(Range 1 to 15).
 **/
    rrmcm_rmif_AggregationPowerOffset \
        aggregationPowerOffsetCCUser[RRM_NUM_DCI_SIZE_CATEGORY][RRM_MAX_CQI_VAL];

    rrmcm_rmif_AggregationPowerOffset \
        aggregationPowerOffsetCEUser[RRM_NUM_DCI_SIZE_CATEGORY][RRM_MAX_CQI_VAL];

}rrmcm_rmif_PdcchAggregationAndPowerOffsetInfo;

typedef struct rrmcm_rmif_DeltaSINRToTPCMapForPUCCHInfoT
{
/* Starting value of the range of delta SINR which maps to pucchTPC.
 ** It is always less than or equal to endDeltaSINR.
 ** Range (-pucchSINRTarget to (255 - pucchSINRTarget) in steps of 1)
 **/
    S32 startSINR;
/* End value of the range of delta SINR which maps to pucchTPC.
 ** It is always greater than or equal to startDeltaSINR.
 ** Range (-pucchSINRTarget to 255 - pucchSINRTarget in steps of 1)
 **/
    S32 endSINR;
/* TPC for PUCCH. Range (-1,0,1,2,3). These are mapped to exact TPC commands
 ** by PUCCH power control algorithm.
 **/
    S32 pucchTPC;
}rrmcm_rmif_DeltaSINRToTPCMapForPUCCHInfo;

typedef struct rrmcm_rmif_DeltaSINRToTPCMapForPUCCHT
{
/* Target SINR for PUCCH. Possible values are 0 to 255 representing
 ** -64dBm to 63.5dBm in steps of 0.5
 **/
    U32 pucchSINRTarget;
/* This field indicates the number of valid entries in the
 ** deltaSinrToTPCMapForPUCCHInfo array.
 ** Range (1 to 4).
 **/
    U32 count;
/* This field contains the mapping of ranges delta SINR
 ** (difference of SINR measured on PUCCH and pucchSINRTarget)
 ** to TPC for PUCCH.
 ** Range of delta sinr should cover all values from
 ** [-pucchSINRTarget to (255 - pucchSINRTarget)]. All sets of
 ** {startSINR, endSINR} should be continuous ranges without any gap between
 ** the sets.
 **/
    rrmcm_rmif_DeltaSINRToTPCMapForPUCCHInfo \
        deltaSinrToTPCMapForPUCCHInfo[RRM_MAX_NUM_TPC_FOR_PUCCH];
}rrmcm_rmif_DeltaSINRToTPCMapForPUCCH;


/******************************************************************************
 **                        PUCCH POWER CONTROL                                 *
 ******************************************************************************/
typedef struct rrmcm_rmif_DeltaBLERToTPCMapForPUCCHInfoT
{
/* Starting value of the range of delta BLER which maps to pucchTPC. It is
 ** always less than or equal to endDeltaBLER.
 ** Range (-pucchBLERTarget to 100 - pucchBLERTarget in steps of 1).
 **/
    S32 startBLER;
/* End value of the range of delta BLER which maps to pucchTPC. It is
 ** always greater than or equal to startDeltaBLER.
 ** Range (-pucchBLERTarget to (100 - pucchBLERTarget) in steps of 1).
 **/
    S32 endBLER;
/* TPC for PUCCH. Range (-1,0,1,2,3). These are mapped to exact TPC commands
 ** by PUCCH power control algorithm.
 **/
    S32 pucchTPC;
}rrmcm_rmif_DeltaBLERToTPCMapForPUCCHInfo;

typedef struct rrmcm_rmif_DeltaBLERToTPCMapForPUCCHT
{
/* Block Error Rate that needs to be maintained on PUCCH.
 ** Range (0,1,...,99,100)
 **/
    U32 pucchBLERTarget;
/* This field indicates the number of valid entries in
 ** deltaBlerToTPCMapForPUCCHInfo array.
 ** Range (1 to 4).
 **/
    U32 count;
/* This field contains the mapping of ranges delta BLER(difference of BLER
 ** measured on PUCCH and pucchBLERTarget) to TPC for PUCCH.
 ** Range of delta bler should cover all values from
 ** [-pucchBLERTarget to (100 - pucchBLERTarget)]. Any two sets of
 ** {startBLER, endBLER} should be continuous ranges without any gap between
 ** the sets.
 **/
    rrmcm_rmif_DeltaBLERToTPCMapForPUCCHInfo \
        deltaBlerToTPCMapForPUCCHInfo[RRM_MAX_NUM_TPC_FOR_PUCCH];
}rrmcm_rmif_DeltaBLERToTPCMapForPUCCH;

/******************************************************************************
 *  *                        PUSCH POWER CONTROL                                 *
 *   *****************************************************************************/
typedef struct rrmcm_rmif_DeltaSINRToTPCMapForPUSCHInfoT
{
 /* Starting value of the range of delta SINR which maps to puschTPC.
 ** It is always less than or equal to endDeltaSINR.
 ** Range (-255 to 255 in steps of 1).
 **/
    S32  startDeltaSINR;
 /* End value of the range of delta SINR which maps to puschTPC.
 ** It is always greater than or equal to startDeltaSINR.
 ** Range (-255 to 255 in steps of 1).
 **/
    S32 endDeltaSINR;
 /* TPC for PUSCH. Range (-4 to 4). These are mapped to exact TPC commands by
 ** PUSCH power control algorithm based on whether accumulation is
 ** enabled or not.
 **/
    S32 puschTPC;
}rrmcm_rmif_DeltaSINRToTPCMapForPUSCHInfo;

typedef struct rrmcm_rmif_DeltaSINRToTPCMapForPUSCHT
{
 /* This field indicates the number of valid entries in
 ** deltaSINRToTPCMapForPUSCHInfo array.
 ** Range (1 to 4).
 **/
    U32 count;
 /* This field contains the mapping of ranges delta SINR to TPC for
 ** PUSCH.
 ** Different ranges provided for each TPC should not overlap
 ** and must cover all values from -255 to 255. There should not be any gap
 ** between two sets of {startDeltaSINR, endDeltaSINR}. Two consecutive sets
 ** must represent contiguous values.
 **/
    rrmcm_rmif_DeltaSINRToTPCMapForPUSCHInfo \
        deltaSINRToTPCMapForPUSCHInfo[RRM_MAX_NUM_TPC_FOR_PUSCH];
}rrmcm_rmif_DeltaSINRToTPCMapForPUSCH;


typedef struct rrmcm_rmif_PathLossToTargetSINRMapInfoT
{
 /* Starting value of the range of pathloss which maps to targetSINR.
 ** It is always less than or equal to endPL.
 ** Range (-230 to 460 dB in steps of 10).
 **/
    S32 startPL;
 /* End value of the range of pathloss which maps to targetSINR.
 ** It is always greater than or equal to startPL.
 ** Range (-230 to 460dB in steps of 10).
 **/
    S32 endPL;
 /* Target SINR to be maintained when a UE is experiencing a pathloss given
 ** by the range startPL to endPL.
 ** Range (0 to 255) representing -64dBm to 63.5dBm in steps of 0.5.
 **/
    U32 targetSINR;
}rrmcm_rmif_PathLossToTargetSINRMapInfo;

typedef struct rrmcm_rmif_PathLossToTargetSINRMapT
{
 /* This field shall indicate the number of tuples in
 ** pathLossToTargetSINRMapInfo array. Range(1,2,..,9,10)
 **/
    U32 count;
 /* Stores the target SINR for a range of pathloss. Number of these
 ** ranges of pathloss is given by count above.
 ** Different ranges provided for each target SINR should not overlap
 ** and must cover all values from -230 to 460dB. There should not be
 ** any gap between two sets of {startPL, endPL}.  Two consecutive sets
 ** must represent contiguous values.
 **/
    rrmcm_rmif_PathLossToTargetSINRMapInfo \
        pathLossToTargetSINRMapInfo[RRM_MAX_NUM_PATHLOSS_TO_SINR_COUNT];
}rrmcm_rmif_PathLossToTargetSINRMap;

typedef struct rrmcm_rmif_AlphaBasedPathLossToTargetSINRMapT
{
/*This pathloss value shall be used a default pathloss in system
 *       and and target SINR based on this pathloss
 *             value shall be used as initial SINR */
    U32  defaultPathLoss;

/* This field is an array of 8 elements(one for each value of alpha)
 ** where index of the array corresponds to path loss to target SINR
 ** mapping for alpha = 0, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 and 1 respectively.
 **/
    rrmcm_rmif_PathLossToTargetSINRMap pathLossToTargetSINRMap[RRM_MAX_NUM_ALPHA_COUNT];
}rrmcm_rmif_AlphaBasedPathLossToTargetSINRMap;


//#define RRMCM_RMIF_TPC_FOR_MSG3_PRESENT                         0x0001    need to be deleted
#define RRMCM_RMIF_SUB_BAND_GROUP_SIZE_PRESENT                  0x0002
#define RRMCM_RMIF_WAIT_TIME_FOR_DCI0_PRESENT                   0x0004
#define RRMCM_RMIF_WAIT_TIME_FORDCI1x2x_PRESENT                 0x0008
#define RRMCM_RMIF_PUSCH_DCI3A_PRESENT                          0x0010
#define RRMCM_RMIF_PUCCH_DCI3A_PRESENT                          0x0020
#define RRMCM_RMIF_COOL_OFF_BLER_TRIGGERED_PUCCH_TPC_PRESENT    0x0040
#define RRMCM_RMIF_COOL_OFF_PUCCH_TPC_PRESENT                   0x0080
#define RRMCM_RMIF_COOL_OFF_PUSCH_TPC_PRESENT                   0x0100
#define RRMCM_RMIF_ALPHA_BASED_PATHLOSS_PRESENT                 0x0200
#define RRMCM_RMIF_DELTA_SINR_TO_TPC_FOR_PUSCH_MATCH_PRESENT    0x0400
#define RRMCM_RMIF_DELTA_BLER_TO_TPC_FOR_PUCCH_MATCH_PRESENT    0x0800
#define RRMCM_RMIF_DELTA_SINR_TO_TPC_FOR_PUCCH_MATCH_PRESENT    0x1000
#define RRMCM_RMIF_PDCCH_AGGREGATION_PRESENT                    0x2000
#define RRMCM_RMIF_CQI_TO_PHICH_POWER_PRESENT                   0x4000
#define RRMCM_RMIF_PL_PHR_CALC_PRESENT                          0x8000

typedef struct _rrmcm_rmif_reconf_scheduler_parameters_t
{

   U8                      cellindex;
   U16                     transaction_id;

   U32 bitmask;
   /* Specifies the TPC Command that needs to be sent in RAR for Msg3 power
    * Please control */
   S32 tpcForMsg3;

   /* used to group PRBs according to received SINR in order to reduce
    ** processing in MAC*/
   U32 srsSubBandGroupSize;

   /* Wait time for sending triggered TPC in DCI0 before DCI 3/3A is scheduled
    ** */
   U32 tpcTriggerWaitTimeForPUSCHDCI0;

   U32 tpcTriggerWaitTimeForPUCCHDCI1x2x;

   /*Percentage threshold used by MAC to decide whether to 
    * schedule DCI 3A TPC command for a TPC-PUSCH-RNTI group.
    * DCI     3 command is scheduled if the following condition holds true.
    **(Number of UEs sharing the TPC-PUSCH-RNTI for which TPC
    ** has been triggered  /  Total number of UEs configured to share
    ** the TPC- PUSCH- RNTI)  * 100   >=
    ** tpcNumUeThresholdForPUSCHDCI3A
    **/
   U32 tpcNumUeThresholdForPUSCHDCI3A;

   /* Percentage threshold used by MAC to decide whether to 
    * schedule DCI 3A TPC command for a TPC-PUCCH-RNTI group. 
    * DCI 3A command is scheduled if the following condition holds true.
    * (Number of UEs sharing the TPC-PUCCH-RNTI for which TPC
    * has been triggered  /  Total number of UEs configured to share
    * the TPC- PUCCH- RNTI)  * 100   >=
    *     tpcNumUeThresholdForPUCCHDCI3A
    **/
   U32 tpcNumUeThresholdForPUCCHDCI3A;


   U32  coolOffPeriodForBLERTriggeredPUCCHTPC;

   U32  coolOffPeriodForPUCCHTPC;

   U32  coolOffPeriodForPUSCHTPC;

   /* specifies the SINR target based on a range of pathloss. */
   rrmcm_rmif_AlphaBasedPathLossToTargetSINRMap alphaBasedPathLossToTargetSINRMap;

   /* specifies the TPC to be used for PUSCH based on delta SINR */
   rrmcm_rmif_DeltaSINRToTPCMapForPUSCH deltaSINRToTPCMapForPUSCH;

   /* specifies the PUCCH target BLER and the mapping of delta BLER to TPC for
    ** PUCCH */
   rrmcm_rmif_DeltaBLERToTPCMapForPUCCH deltaBlerToTPCMapForPUCCH;

   /* specifies the PUCCH target SINR and the mapping of delta SINR to TPC for
    * PUCCH */
   rrmcm_rmif_DeltaSINRToTPCMapForPUCCH deltaSINRToTPCMapForPUCCH;

   /* specifies the set of aggregation levels that need to be tried for CCE
    * allocation based on DCI size category and wideband CQI.*/
   rrmcm_rmif_PdcchAggregationAndPowerOffsetInfo pdcchAggregationAndPowerOffset;

   /* specifies the correction to PHICH power offset to be used for PHICH power
    * control based on wideband CQI */
   rrmcm_rmif_CqiToPHICHPower  cqiToPHICHPower;

   U32 minRbForPlPhrCalc;

}rrmcm_rmif_reconf_scheduler_parameters_t;

/* RRM_MAC_RECONF_SCHEDULER_CHANGES_ENDS */
typedef struct _rrmcm_bitrate_ul_dl_t
{
    U64 max_bitrate; /*^ M, 0, N, 0, 0 ^*/
    U64 min_bitrate; /*^ M, 0, N, 0, 0 ^*/
}rrmcm_bitrate_ul_dl_t;
#define RRMCM_RMIF_ECN_UL_BITRATE 0x01
#define RRMCM_RMIF_ECN_DL_BITRATE   0x02
typedef struct _rrmcm_configure_qci_bitratet
{
    rrm_bitmask_t bitmask; /*^ BITMASK ^*/
    U8 qci; /*^ M, 0, B, 1, 9 ^*/
    rrmcm_bitrate_ul_dl_t ul_bitrate; /*^ O, RRM_OAM_ECN_UL_BITRATE_PRESENT, N, 0, 0 ^*/
    rrmcm_bitrate_ul_dl_t dl_bitrate; /*^ O, RRM_OAM_ECN_DL_BITRATE_PRESENT, N, 0, 0 ^*/
}rrmcm_configure_qci_bitrate_t;
typedef struct _rrmcm_qci_bitrate_config_t
{
  rrm_bitmask_t bitmask;
  U8 count;
  rrmcm_configure_qci_bitrate_t bitrate_for_qci[RRM_MAX_QCI];
}rrmcm_qci_bitrate_config_t;
typedef struct _rrmcm_rmif_cell_ecn_config_req_t
{
    rrm_cell_index_t cell_index;
    U16 transaction_id;
    S8 num_of_ue;
    rrmcm_qci_bitrate_config_t bitrate;
}rrmcm_rmif_cell_ecn_config_req_t;


typedef enum
{
    RRMCM_ACTN_NONE = 0x01,
    RRMCM_ACTN_HANDOVER = 0x02,
    RRMCM_ACTN_AC_BARRING = 0x4,
    RRMCM_ACTN_STOP_ADM = 0x08,
    RRMCM_ACTN_REL_UE_CARR_REDIRECT = 0x10,
    RRMCM_ACTN_REL_UE = 0x20,
    RRMCM_ACTN_AQM = 0x40,
    RRMCM_ACTN_RRM_ECN_UL_DIR = 0x80,
    RRMCM_ACTN_RRM_ECN_DL_DIR = 0x100,
    RRMCM_ACTN_RRM_ECN_BI_DIR = 0x200,
    /* Carrier_Aggregation_Start */
    RRMCM_ACTN_RRM_SCELL_DEACTIVATE = 0x400,
    /* Carrier_Aggregation_End */
    RRMCM_ACTN_MAX = 0x800
}rrmcm_action_et;
typedef struct _rrmcm_rmif_cell_load_status_t
{
    rrm_oam_load_src_et load_src; /* SOURCR_TNL, SOURCE_MEMORY, SOURCE_CPU */
    /* coverity : CID 29632,29626*/
    rrm_load_type load_level; /* 0 for LOW_LOAD, MEDIUM_LOAD, HIGH_LOAD, OVER_LOAD  or 1-100 percent incase periodic reporting */
}rrmcm_rmif_cell_load_status_t;
typedef struct _rrmcm_rmif_cell_platform_t
{
   rrm_cell_index_t cell_index;
   U8 ind_type;
   U8 count;
   rrmcm_rmif_cell_load_status_t cell_load[RRM_MAX_SOURCE];
}rrmcm_rmif_cell_platform_t;

/*
 * RRMCM_RMIF_TTT_UPDATE_IND_REQ
 */

/*! \struct rrmcm_rmif_ttt_update_ind_t
 *  \brief It gives TTT update indication
 *  \param cell_index       cell index of source cell
 *  \param updated_ttt      updted TTT value 
 */
typedef struct _rrmcm_rmif_ttt_update_ind_t
{
    U8                      cell_index;
    rrm_time_to_trigger_et updated_ttt;
}rrmcm_rmif_ttt_update_ind_t;

/* RACH_OPTIMIZATION_CHANGES_START */
typedef struct _rrmcm_rmif_rach_info_ind_t
{
    rrm_cell_index_t    cell_index;
    U16                 l2_reports_periodicity;
        /* SPR_17763_FIX_START */
    struct timeval  incoming_time_stamp;
        /* SPR_17763_FIX_END */
}rrmcm_rmif_rach_info_ind_t;
/* RRMCM_RMIF_RACH_INFO_IND */

typedef struct _rrmcm_rmif_rach_l2_report_t
{
    rrm_cell_index_t    cell_index;
    U16                 num_of_l2_perf_accumulated_reports;
    U64                 total_rcvd_msg3_random_access_preambles_cbra;
    U64                 total_rcvd_msg3_random_access_preambles_cfra;
    U64                 total_failed_random_access_preambles_msg2_cbra;
    U64                 total_failed_random_access_preambles_msg2_cfra;
    U64                 total_allocated_random_access_preambles;
    U64                 total_succ_random_access_ta[MAX_TA_RANGE];
    U64                 total_unassigned_false_cfra;
    U64                 total_unassigned_sum_cfra;
    /* SPR-18438 START */
    U64                 num_random_access_preambles_group_a;
    U64                 num_random_access_preambles_group_b;
    /* SPR-18438 END */
}rrmcm_rmif_rach_l2_report_t;
/* RRMCM_RMIF_RACH_L2_REPORT */
/* RACH_OPTIMIZATION_CHANGES_END */

/*SPR 10329:start*/


typedef rrm_son_gu_group_id_list_t rrmcm_mif_gu_group_id_list_t;

#define RRMCM_RMIF_GU_GROUP_ID_TO_ADD_LIST_PRESENT      0x01
#define RRMCM_RMIF_GU_GROUP_ID_TO_DELETE_LIST_PRESENT   0x02
/*! \ rrm_son_enb_config_update_ind_t
 *  \brief This struct contains GU Group ID Info of nbr eNB 
 *  \param nbr_enb_id  Neighbor eNodeB ID 
 *  \param add_gu_id   GU Group Id To Add List
 *  \param delete_gu_id  GU Group Id To Delete List
*/

typedef struct _rrmcm_rmif_enb_config_update_ind_t
{
    rrm_bitmask_t             bitmask ; /*^ BITMASK ^*/
    rrm_global_enb_id_t       nbr_enb_id; /*^ M, 0, N, 0, 0 ^*/
    rrmcm_mif_gu_group_id_list_t     add_gu_id; /*^ O, RRMCM_RMIF_GU_GROUP_ID_TO_ADD_LIST_PRESENT, N, 0, 0 ^*/
    rrmcm_mif_gu_group_id_list_t   delete_gu_id; /*^ O, RRMCM_RMIF_GU_GROUP_ID_TO_DELETE_LIST_PRESENT, N, 0, 0 ^*/
}rrmcm_rmif_enb_config_update_ind_t;


typedef rrm_oam_enb_gu_group_id_t rrm_enb_gu_group_id_t;

typedef rrm_oam_gu_group_info_t rrm_gu_group_info_t;
 
/*SPR 10329:end*/

/* Carrier_Aggregation_Start */
/* RRMCM_RMIF_ENB_CONFIG_REQ */
/*! \rrmcm_rmif_enb_config_req_t
 *  \brief This struct contains the request for configuration information
     related to eNodeB
 *  \param bitmask Bitmask Value
 *  \param enb_type To indicate the eNodeB type
 *  \param cell_start_flag To indicate the cell start flag
 */
#define RRMCM_RMIF_GLOBAL_ENB_ID_PRESENT     0x01
#define RRMCM_RMIF_CELL_START_FLAG_PRESENT     0x02
/*SPR 10329:start*/
#define RRMCM_RMIF_GU_GROUP_INFO_PRESENT     0x04
/*SPR 10329:end*/
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
#define RRMCM_RMIF_QCI_FOR_SGNB_ADDITION_PRESENT  0x20
/*NR_DC Code Change Stop*/
#endif
typedef struct _rrmcm_rmif_enb_config_req_t
{
    rrm_bitmask_t bitmask;          /*^ BITMASK ^*/
    rrm_global_enb_id_t enb_id;   /*^ O, RRMCM_RMIF_GLOBAL_ENB_ID_PRESENT, N, 0, 0 ^*/
    U16 transaction_id;
    U8  cell_start_flag;       /*^ O, RRMCM_RMIF_CELL_START_FLAG_PRESENT,  H, 0, 1 ^*/
    /*SPR 10329:start*/
    rrm_gu_group_info_t gu_group_info; /*^ O, RRMCM_RMIF_GU_GROUP_INFO_PRESENT, N, 0, 0 ^*/
    /*SPR 10329:end*/
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    U8        qci_for_sgnb_addition; /*^0, RRMCM_RMIF_QCI_FOR_SGNB_ADDITION_PRESENT, N, 0, 0 ^*/
/*NR_DC Code Change Stop*/
#endif
}rrmcm_rmif_enb_config_req_t; /*^ API, RRMCM_RMIF_ENB_CONFIG_REQ ^*/


/*SPR 10329:start*/
/*CONFIG_RESP*/
typedef rrm_oam_enb_gu_group_id_success_failure_list_t rrm_enb_gu_group_id_success_failure_list_t;

typedef rrm_oam_gu_group_info_success_failure_list_t rrm_gu_group_info_success_failure_list_t;

/*SPR 10329:end*/

/* RRMCM_RMIF_ENB_CONFIG_RESP */
/*! \rrmcm_rmif_enb_config_resp_t
 *  \brief This struct contains the response for configuration of eNodeB request
 *  \param response success/failure
 *  \param fail_cause cause of failure
 */

/*SPR 10329:start*/
#define RRM_GU_GROUP_RESPONSE_LIST_PRESENT  0x01
/*SPR 10329:end*/
typedef struct _rrmcm_rmif_enb_config_resp_t
{
    /*SPR 10329:start*/
    rrm_bitmask_t bitmask;          /*^ BITMASK ^*/
    /*SPR 10329:end*/
    rrm_return_et       response;       /*^ M, 0, N, 0, 0 ^*/
    rrm_error_et        fail_cause;     /*^ M, 0, N, 0, 0 ^*/
    U16 transaction_id;
    /*SPR 10329:start*/
    rrm_gu_group_info_success_failure_list_t   gu_group_response_list;
                                        /*^ O, RRM_GU_GROUP_RESPONSE_LIST_PRESENT, N, 0, 0 ^*/
    /*SPR 10329:end*/
}rrmcm_rmif_enb_config_resp_t; /*^ API, RRMCM_RMIF_ENB_CONFIG_RESP ^*/
/* Carrier_Aggregation_End */


/* Carrier Aggregation start */

typedef struct _rrm_resource_load_info_t 
{
#define RRM_BROADCAST_RR_LOAD_PERIODIC_REPORT_PRESENT         0x01
#define RRM_BROADCAST_TNL_LOAD_PERIODIC_REPORT_PRESENT        0x02
#define RRM_BROADCAST_HW_LOAD_PERIODIC_REPORT_PRESENT         0x04
#define RRM_BROADCAST_COMP_AVL_CAP_PERIODIC_REPORT_PRESENT    0x08
    U8                                 bitmask;  /*^ BITMASK ^*/
    rrm_rs_prb_t                       rr_load;
    /*^ O,RRM_RR_LOAD_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    rrm_rs_tnl_load_t                  s1_tnl_load;
    /*^ O,RRM_TNL_LOAD_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    rrm_rs_hw_load_t                   hw_load;
    /*^ O,RRM_HW_LOAD_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    rrm_rs_cacg_t                      comp_avl_cap_grp;
    /*^ O,RRM_COMP_AVL_CAP_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
}rrm_resource_load_info_t;

typedef struct _rrm_cell_resource_load_info_t
{
    rrm_oam_eutran_global_cell_id_t     cell_id;
    rrm_resource_load_info_t            rsu_info;
}rrm_cell_resource_update_info_t;

typedef struct rrm_cell_rsu_load_update_info_t
{
    rrm_cell_index_t                 cell_index;
    rrm_cell_resource_update_info_t  rsu_load_info; 
}rrm_cell_rsu_load_update_info_t;
/* Carrier Aggregation end */

/*SPR 10329:start*/
const void *
rrmcm_enb_group_id_key
(
 const YLNODE    *p_ylnode
);
int rrmcm_enb_id_data_key_compare
(
 const rrm_void_t *p_key1,
  const rrm_void_t *p_key2
);

int rrmcm_gu_group_id_data_key_compare
(
 const rrm_void_t *p_key1,
  const rrm_void_t *p_key2
);

const void *
rrmcm_gu_group_id_key
(
 const YLNODE    *p_ylnode
  );

/*SPR 10329:end*/


#endif /* _RRM_CELL_RRMIM_INTF_H_ */
