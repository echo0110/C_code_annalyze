/********************************************************************
 *
 *  FILE NAME   : rrc_ext_api.h
 *
 *  DESCRIPTION : This file contains the interface message API
 *                IDs for all modules external to RRC.
 *
 *  REVISION HISTORY :
 *
 *  DATE          Name        Reference         Comments
 *  ----          ----        ---------         --------
 *  11 MAY 2009   Yusuf R     API no. 408500003 Initial
 *
 *  Copyright (c) 2009, Aricent Inc.
 *
 *******************************************************************/

#ifndef _RRC_EXT_API_H_
#define _RRC_EXT_API_H_

/********************************************************************
 * RRC - MAC APIs
 *******************************************************************/
#define RRC_MAC_API_BASE                     0x0000

#define RRC_MAC_API_OFFSET_1_START           (RRC_MAC_API_BASE + 0x01)
#define RRC_MAC_CONFIG_CELL_REQ               RRC_MAC_API_OFFSET_1_START
#define RRC_MAC_SFN_REQ                      (RRC_MAC_API_BASE + 0x02)
#define RRC_MAC_RECONFIG_CELL_REQ            (RRC_MAC_API_BASE + 0x03)
#define RRC_MAC_CREATE_UE_ENTITY_REQ         (RRC_MAC_API_BASE + 0x04)
#define RRC_MAC_DELETE_UE_ENTITY_REQ         (RRC_MAC_API_BASE + 0x05)
#define RRC_MAC_RECONFIGURE_UE_ENTITY_REQ    (RRC_MAC_API_BASE + 0x06)
#define RRC_MAC_UE_DRX_CMD_REQ               (RRC_MAC_API_BASE + 0x07)
#define RRC_MAC_BCCH_CONFIG_REQ              (RRC_MAC_API_BASE + 0x08)
#define RRC_MAC_PCCH_MSG_REQ                 (RRC_MAC_API_BASE + 0x09)
#define RRC_MAC_CCCH_MSG_REQ                 (RRC_MAC_API_BASE + 0x0A)
#define RRC_MAC_UE_CON_REJ_REQ               (RRC_MAC_API_BASE + 0x0B)
#define RRC_MAC_DELETE_CELL_REQ              (RRC_MAC_API_BASE + 0x0C)
#define RRC_MAC_HO_RACH_RESOURCE_REQ         (RRC_MAC_API_BASE + 0x0D)
/*SPR_19066_START*/
/* API RRC_MAC_UE_INACTIVE_TIME_REQ has been deprecated */
/*SPR_19066_END*/
#define RRC_MAC_RESET_UE_ENTITY_REQ          (RRC_MAC_API_BASE + 0x0F)
#define RRC_MAC_CHANGE_CRNTI_REQ             (RRC_MAC_API_BASE + 0x10)
#define RRC_MAC_API_OFFSET_1_END             RRC_MAC_CHANGE_CRNTI_REQ

#define RRC_MAC_API_OFFSET_2_START           (RRC_MAC_API_BASE + 0x65)   
#define RRC_MAC_CONFIG_CELL_CNF              RRC_MAC_API_OFFSET_2_START 
#define RRC_MAC_SFN_CNF                      (RRC_MAC_API_BASE + 0x66)
#define RRC_MAC_RECONFIG_CELL_CNF            (RRC_MAC_API_BASE + 0x67)
#define RRC_MAC_CREATE_UE_ENTITY_CNF         (RRC_MAC_API_BASE + 0x68)
#define RRC_MAC_DELETE_UE_ENTITY_CNF         (RRC_MAC_API_BASE + 0x69)
#define RRC_MAC_RECONFIGURE_UE_ENTITY_CNF    (RRC_MAC_API_BASE + 0x6A)
#define RRC_MAC_DELETE_CELL_CNF              (RRC_MAC_API_BASE + 0x6B)
#define RRC_MAC_HO_RACH_RESOURCE_RESP        (RRC_MAC_API_BASE + 0x6C)
/*SPR_19066_START*/
/* API RRC_MAC_UE_INACTIVE_TIME_RESP has been deprecated */
/*SPR_19066_END*/
#define RRC_MAC_RESET_UE_ENTITY_CNF          (RRC_MAC_API_BASE + 0x6E)
#define RRC_MAC_CHANGE_CRNTI_CNF             (RRC_MAC_API_BASE + 0x6F)
#define RRC_MAC_API_OFFSET_2_END             RRC_MAC_CHANGE_CRNTI_CNF

#define RRC_MAC_API_OFFSET_3_START           (RRC_MAC_API_BASE + 0xC9)   
#define RRC_MAC_UE_ENTITY_POWER_HEADROOM_IND  RRC_MAC_API_OFFSET_3_START
#define RRC_MAC_SFN_IND                      (RRC_MAC_API_BASE + 0xCA)
#define RRC_MAC_CCCH_MSG_IND                 (RRC_MAC_API_BASE + 0xCB)
#define RRC_MAC_HO_REL_RACH_RESOURCE_IND     (RRC_MAC_API_BASE + 0xCC)     
#define RRC_MAC_RADIO_LINK_FAILURE_IND       (RRC_MAC_API_BASE + 0xCD)
/*SPR_19066_START*/
/* API RRC_MAC_INACTIVE_UES_IND has been deprecated */
/*SPR_19066_END*/
/* Reconfig feature Phase-II Start */

#define RRC_MAC_CELL_START_REQ                (RRC_MAC_API_BASE + 0xCF)
#define RRC_MAC_CELL_START_CNF                (RRC_MAC_API_BASE + 0xD0)
#define RRC_MAC_CELL_STOP_REQ                 (RRC_MAC_API_BASE + 0xD1)
#define RRC_MAC_CELL_STOP_CNF                 (RRC_MAC_API_BASE + 0xD2)
#define RRC_MAC_UE_SYNC_STATUS_IND            (RRC_MAC_API_BASE + 0xD3)
/* SPS Start */
#define RRC_MAC_RECONFIG_COMPLETE_IND         (RRC_MAC_API_BASE + 0xD4)
/* SPS Stop */

/* coverity 92915 */
#define M2AP_MAC_API_BASE RRC_MAC_RECONFIG_COMPLETE_IND
/* coverity 92915 */
#define RRC_M2AP_MAC_AREA_CONFIG_REQ          (RRC_MAC_API_BASE + 0xD5)
#define RRC_M2AP_MAC_AREA_CONFIG_CNF          (RRC_MAC_API_BASE + 0xD6)
#define RRC_M2AP_MAC_COUNTING_MSG             (RRC_MAC_API_BASE + 0xD7)
#define RRC_M2AP_MAC_COUNTING_MSG_CNF         (RRC_MAC_API_BASE + 0xD8)
#define RRC_M2AP_MAC_MCCH_IND                 (RRC_MAC_API_BASE + 0xD9)
#define RRC_M2AP_MAC_RESET_SESSION_REQ        (RRC_MAC_API_BASE + 0xDA)
#define RRC_M2AP_MAC_RESET_SESSION_CNF        (RRC_MAC_API_BASE + 0xDB)

/* eMTC changes start */
#define RRC_MAC_EMTC_CCCH_MSG_IND             (RRC_MAC_API_BASE + 0xDC)
#define RRC_MAC_API_OFFSET_3_END              RRC_MAC_EMTC_CCCH_MSG_IND
/* eMTC changes stop */

/* SPR 21660 changes start_mokshi */
#define RRC_MAC_API_OFFSET_NB_START              (RRC_MAC_API_BASE + 0x100)
#define RRC_MAC_CONFIG_NB_CELL_REQ               RRC_MAC_API_OFFSET_NB_START 
#define RRC_MAC_CONFIG_NB_CELL_CNF               (RRC_MAC_API_OFFSET_NB_START + 0x01)
#define RRC_MAC_CREATE_UE_ENTITY_NB_REQ          (RRC_MAC_API_OFFSET_NB_START + 0x02)
#define RRC_MAC_CREATE_UE_ENTITY_NB_CNF          (RRC_MAC_API_OFFSET_NB_START + 0x03)
#define RRC_MAC_CCCH_MSG_NB_IND                  (RRC_MAC_API_OFFSET_NB_START + 0x04)
#define RRC_MAC_DELETE_NB_UE_ENTITY_REQ          (RRC_MAC_API_OFFSET_NB_START + 0x05)
#define RRC_MAC_DELETE_NB_UE_ENTITY_CNF          (RRC_MAC_API_OFFSET_NB_START + 0x06)
#define RRC_MAC_API_OFFSET_NB_END                RRC_MAC_DELETE_NB_UE_ENTITY_CNF
/* SPR 21660 changes end_mokshi */

#define RRC_MAC_MAX_API                          RRC_MAC_DELETE_NB_UE_ENTITY_CNF

/******************************************************************************
 * RRC - MAC TAGs
 *****************************************************************************/
#define RRC_MAC_TAG_BASE                        0

#define RRC_MAC_RACH_CONFIG_INFO                (RRC_MAC_TAG_BASE + 0x01)
#define RRC_MAC_PHICH_CONFIG_INFO               (RRC_MAC_TAG_BASE + 0x02)
#define RRC_MAC_MIB_MSG_REQ                     (RRC_MAC_TAG_BASE + 0x03)
#define RRC_MAC_SIBTYPE1_MSG_REQ                (RRC_MAC_TAG_BASE + 0x04) 
#define RRC_MAC_SI_MSG_INFO                     (RRC_MAC_TAG_BASE + 0x05)
#define RRC_MAC_SI_MSG_REQ                      (RRC_MAC_TAG_BASE + 0x06)
#define RRC_MAC_CONFIG_CELL_ERROR_CODE          (RRC_MAC_TAG_BASE + 0x07)
//CELL RECONFIGURAION PHASE 2 : RRC_MAC_SFN_SF_INFO RRC_MAC_SFN_INFO has been changed
#define RRC_MAC_SFN_INFO                        (RRC_MAC_TAG_BASE + 0x08)
#define RRC_MAC_ADD_UE_INFO                     (RRC_MAC_TAG_BASE + 0x09)
#define RRC_MAC_CREATE_LC_INFO                  (RRC_MAC_TAG_BASE + 0x0A)
#define RRC_MAC_SR_CONFIG_INFO                  (RRC_MAC_TAG_BASE + 0x0B)
#define RRC_MAC_CQI_INFO                        (RRC_MAC_TAG_BASE + 0x0C)
#define RRC_MAC_UL_LC_CREATE_REQ                (RRC_MAC_TAG_BASE + 0x0D)
#define RRC_MAC_DL_LC_CREATE_REQ                (RRC_MAC_TAG_BASE + 0x0E)
#define RRC_MAC_UL_LC_RECONFIGURE_REQ           (RRC_MAC_TAG_BASE + 0x0F)
#define RRC_MAC_DL_LC_RECONFIGURE_REQ           (RRC_MAC_TAG_BASE + 0x10)
#define RRC_MAC_SR_SETUP_INFO                   (RRC_MAC_TAG_BASE + 0x11)
#define RRC_MAC_CREATE_LC_ERROR                 (RRC_MAC_TAG_BASE + 0x12)
#define RRC_MAC_UL_LC_CONFIG_RESP               (RRC_MAC_TAG_BASE + 0x13)
#define RRC_MAC_DL_LC_CONFIG_RESP               (RRC_MAC_TAG_BASE + 0x14)
#define RRC_MAC_RECONFIG_UE_INFO                (RRC_MAC_TAG_BASE + 0x15)
#define RRC_MAC_RECONFIGURE_LC_REQ              (RRC_MAC_TAG_BASE + 0x16)
#define RRC_MAC_CREATE_LC_REQ                   (RRC_MAC_TAG_BASE + 0x17)
#define RRC_MAC_DELETE_LC_REQ                   (RRC_MAC_TAG_BASE + 0x18)
#define RRC_MAC_DL_MAX_RB_INFO                  (RRC_MAC_TAG_BASE + 0x19)
#define RRC_MAC_UL_MAX_RB_INFO                  (RRC_MAC_TAG_BASE + 0x1A)
#define RRC_MAC_TX_MODE_INFO                    (RRC_MAC_TAG_BASE + 0x1B)
#define RRC_MAC_NUM_OF_LAYER_INFO               (RRC_MAC_TAG_BASE + 0x1C)
#define RRC_MAC_CODE_BOOK_INDEX_INFO            (RRC_MAC_TAG_BASE + 0x1D)
#define RRC_MAC_SIMULTANEOUS_ACK_NACK_CQI_INFO  (RRC_MAC_TAG_BASE + 0x1E)
#define RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO       (RRC_MAC_TAG_BASE + 0x1F)
#define RRC_MAC_RECONFIGURE_LC_ERROR            (RRC_MAC_TAG_BASE + 0x20)
#define RRC_MAC_DELETE_LC_ERROR                 (RRC_MAC_TAG_BASE + 0x21)
#define RRC_MAC_PUCCH_CONFIG_INFO               (RRC_MAC_TAG_BASE + 0x22)
#define RRC_MAC_SI_MSG_INFO_PARAM               (RRC_MAC_TAG_BASE + 0x23)
#define RRC_MAC_PUSCH_CONFIG_INFO               (RRC_MAC_TAG_BASE + 0x24)
/**** TDD changes start *****/
#define RRC_MAC_TDD_CONFIG                      (RRC_MAC_TAG_BASE + 0x25)
#define RRC_MAC_TDD_ACK_NACK_FEEDBACK_MODE      (RRC_MAC_TAG_BASE + 0x26)
#define RRC_MAC_CQI_FMT_IND_PERIODIC_SUBBAND_K  (RRC_MAC_TAG_BASE + 0x27)
/**** TDD changes stop *****/

#define RRC_MAC_CQI_APERIODIC_CONFIG_INFO       (RRC_MAC_TAG_BASE + 0x32)
#define RRC_MAC_CQI_PERIODIC_CONFIG_INFO        (RRC_MAC_TAG_BASE + 0x33)
#define RRC_MAC_RI_CONFIG_INDEX_INFO            (RRC_MAC_TAG_BASE + 0x34)
#define RRC_MAC_MOD_PERIOD_INFO                 (RRC_MAC_TAG_BASE + 0x35)
#define RRC_MAC_SFN_GAP_INFO                    (RRC_MAC_TAG_BASE + 0x36)

#define RRC_MAC_CRNTI_INFO                      (RRC_MAC_TAG_BASE + 0x37)
#define RRC_MAC_PREAMBLE_INFO                   (RRC_MAC_TAG_BASE + 0x38)
#define RRC_MAC_UE_INACTIVE_TIME_VAL            (RRC_MAC_TAG_BASE + 0x39)
#define RRC_MAC_UE_CATEGORY_INFO                (RRC_MAC_TAG_BASE + 0x40)

/* SRS Start */
#define RRC_MAC_SRS_COMMON_CONFIG               (RRC_MAC_TAG_BASE + 0x41)
#define RRC_MAC_SRS_COMMON_SETUP_INFO           (RRC_MAC_TAG_BASE + 0x42)
#define RRC_MAC_SRS_MAX_UPPTS                   (RRC_MAC_TAG_BASE + 0x43)
#define RRC_MAC_SRS_DEDICATED_CONFIG            (RRC_MAC_TAG_BASE + 0x44)
#define RRC_MAC_SRS_DEDICATED_SETUP_INFO        (RRC_MAC_TAG_BASE + 0x45)
/* CBSR Changes Start */
#define RRC_MAC_CODEBOOK_SUBSET_RESTRICTION     (RRC_MAC_TAG_BASE + 0x46)
/* CBSR Changes Stop */
/* SRS Stop */
#define RRC_MAC_DRX_CONFIG_INFO                 (RRC_MAC_TAG_BASE + 0x47)
#define RRC_MAC_DRX_SETUP_CONFIG_INFO           (RRC_MAC_TAG_BASE + 0x48)
#define RRC_MAC_SHORT_DRX_CONFIG                (RRC_MAC_TAG_BASE + 0x49)
#define RRC_MAC_MEAS_GAP_CONFIG_INFO            (RRC_MAC_TAG_BASE + 0x50)
#define RRC_MAC_MEAS_GAP_SETUP_CONFIG_INFO      (RRC_MAC_TAG_BASE + 0x51)
#define RRC_MAC_INACTIVE_UE_INFO                (RRC_MAC_TAG_BASE + 0x53)
#define RRC_MAC_TIME_ALIGNMENT_TIME_INFO        (RRC_MAC_TAG_BASE + 0X54)
#define RRC_MAC_CONTN_FREE_RACH_TIMER_INFO      (RRC_MAC_TAG_BASE + 0x55)
#define RRC_MAC_FREQ_SELECTIVE_ENABLE           (RRC_MAC_TAG_BASE + 0x56)
#define RRC_MAC_AMBR_QOS_INFO                   (RRC_MAC_TAG_BASE + 0x57)
#define RRC_MAC_QOS_INFO                        (RRC_MAC_TAG_BASE + 0x58)
#define RRC_MAC_GBR_QOS_INFO                    (RRC_MAC_TAG_BASE + 0x5A)
#define RRC_MAC_SI_MSG_SEGMENT_REQ              (RRC_MAC_TAG_BASE + 0x5B)
/* Release 9 upgrade */
#define RRC_MAC_CQI_REPORT_CONFIG_V920             (RRC_MAC_TAG_BASE + 0x5C)
#define RRC_MAC_CQI_REPORT_CONFIG_CQI_MASK_R9      (RRC_MAC_TAG_BASE + 0x5D)
#define RRC_MAC_CQI_REPORT_CONFIG_PMI_RI_REPORT_R9 (RRC_MAC_TAG_BASE + 0x5E)
#define RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V920   (RRC_MAC_TAG_BASE + 0x5F)
#define RRC_NUM_SI_REPETITIONS                     (RRC_MAC_TAG_BASE + 0x60)
/*SPR_19066_START*/
/* RRC_MAC_UE_INACTIVITY_TIMER_CONFIG has been deprecated */
/*SPR_19066_END*/
#define RRC_MAC_PREAMBLE_GROUP_A_CONFIG         (RRC_MAC_TAG_BASE + 0x62)

/* CLPC Start */
#define RRC_MAC_TPC_RNTI_RANGE                  (RRC_MAC_TAG_BASE + 0x63)
#define RRC_MAC_PDSCH_CONFIG_COMMON_INFO        (RRC_MAC_TAG_BASE + 0x64)
#define RRC_MAC_DOWNLINK_POWER_CONTROL_COMMON   (RRC_MAC_TAG_BASE + 0x65)
#define RRC_MAC_UPLINK_POWER_CONTROL_COMMON     (RRC_MAC_TAG_BASE + 0x66)
#define RRC_MAC_POWER_CONTROL_ENABLE            (RRC_MAC_TAG_BASE + 0x67)

#define RRC_MAC_UPLINK_POWER_CONTROL_DEDICATED  (RRC_MAC_TAG_BASE + 0x68)
#define RRC_MAC_TPC_PDCCH_CONFIG_PUCCH          (RRC_MAC_TAG_BASE + 0x69)
#define RRC_MAC_PDSCH_CONFIG_INFO               (RRC_MAC_TAG_BASE + 0x6A)
#define RRC_MAC_P0_UE_PUSCH                     (RRC_MAC_TAG_BASE + 0x6B)

#define RRC_MAC_TPC_RNTI_INFO                   (RRC_MAC_TAG_BASE + 0x6C)

/* Rel Upgrade Changes Start */
#define RRC_MAC_SR_PROHBIT_TIMER_VALUE_V920	(RRC_MAC_TAG_BASE + 0x6D)
/* Rel9 Upgrade Changes Stop */

#define RRC_MAC_TPC_PDCCH_CONFIG_PUSCH		(RRC_MAC_TAG_BASE + 0x6E)

#define RRC_MAC_EXPLICIT_CELL_START_REQUIRED    (RRC_MAC_TAG_BASE + 0x6F)

#define RRC_NUM_SI_TRANSMISSIONS                (RRC_MAC_TAG_BASE + 0x70)
#define RRC_MAC_SI_START_OFFSET                 (RRC_MAC_TAG_BASE + 0x71)
#define RRC_MAC_UL_SYNC_LOSS_TIMER_INFO         (RRC_MAC_TAG_BASE + 0x73)
#define RRC_MAC_PUCCH_CQI_SINR_THRESHOLD_INFO   (RRC_MAC_TAG_BASE + 0x74)
#define RRC_MAC_RBS_FOR_DCI_1A                  (RRC_MAC_TAG_BASE + 0x81)
#define RRC_MAC_RBS_FOR_DCI_1C                  (RRC_MAC_TAG_BASE + 0x82)
#define RRC_MAC_DCI_FORMAT_FOR_SI               (RRC_MAC_TAG_BASE + 0x83)

#define RRC_MAC_PUSCH_CONFIG_COMMON             (RRC_MAC_TAG_BASE + 0x84)
#define RRC_MAC_UE_PSR_REQ                      (RRC_MAC_TAG_BASE + 0x85)
#define RRC_MAC_ENB_PSR_REQ                     (RRC_MAC_TAG_BASE + 0x86)
#define RRC_MAC_INTRA_ENB_HO_REQ                (RRC_MAC_TAG_BASE + 0x87)
#define RRC_MAC_USER_LOCATION_TYPE              (RRC_MAC_TAG_BASE + 0x88)
#define RRC_MAC_N_GAP                           (RRC_MAC_TAG_BASE + 0x89)
#define RRC_MAC_SPS_CRNTI_RANGE                 (RRC_MAC_TAG_BASE + 0x8A)
#define RRC_MAC_UE_CAPABILITY_ADDITIONAL_PARAMETERS  (RRC_MAC_TAG_BASE + 0x90)
#define RRC_MAC_DELETE_LC_STATUS                (RRC_MAC_TAG_BASE + 0x91)
/* SPS Start */
#define RRC_MAC_SPS_CRNTI_INFO                  (RRC_MAC_TAG_BASE + 0x94)
#define RRC_MAC_SPS_CONFIG                      (RRC_MAC_TAG_BASE + 0x95)
#define RRC_MAC_SPS_DL_INFO                     (RRC_MAC_TAG_BASE + 0x96)
#define RRC_MAC_SPS_DL_SETUP_INFO               (RRC_MAC_TAG_BASE + 0x97)
#define RRC_MAC_SPS_N1_PUCCH_AN_PERSIST_LIST    (RRC_MAC_TAG_BASE + 0x98)
#define RRC_MAC_SPS_UL_INFO                     (RRC_MAC_TAG_BASE + 0x99)
#define RRC_MAC_SPS_UL_SETUP_INFO               (RRC_MAC_TAG_BASE + 0x9A)
#define RRC_MAC_P_ZERO_PERSISTENT               (RRC_MAC_TAG_BASE + 0x9B)
#define RRC_MAC_TWO_INTERVALS_CONFIG            (RRC_MAC_TAG_BASE + 0x9C)     
#define RRC_MAC_ROHC_PROFILE                    (RRC_MAC_TAG_BASE + 0x9D)
/* HD-FDD start */
#define RRC_MAC_RF_PARAMS                       (RRC_MAC_TAG_BASE + 0x9E)
/* HD-FDD stop */
/* min_ue_power start */
#define RRC_MAC_PHR_TIMER_CONFIG		(RRC_MAC_TAG_BASE + 0x9F)
#define RRC_MAC_MIN_UE_POWER			(RRC_MAC_TAG_BASE + 0xA0)
/* min_ue_power stop */
/* UL_MU_MIMO Start */
#define RRC_MAC_UL_MU_MIMO_INFO			(RRC_MAC_TAG_BASE + 0xA1)
/* UL_MU_MIMO Stop */
#define RRC_MAC_MAX_SPS_HARQ_RETX               (RRC_MAC_TAG_BASE + 0xA2)
#define RRC_MAC_EXPLICIT_RELEASE_AFTER          (RRC_MAC_TAG_BASE + 0xA3)
/* SPS Stop */

/* DRX_MAC_CE_CMD  Start */
#define RRC_MAC_DRX_MAC_CE_CMD_TRIGGER_INFO	(RRC_MAC_TAG_BASE + 0xA4)
#define RRC_MAC_DYNAMIC_PDCCH_INFO		(RRC_MAC_TAG_BASE + 0xA5)

/* DRX_MAC_CE_CMD  End */
/*CDMA2000_CSFB_HO start*/
#define RRC_MAC_SF_INFO                         (RRC_MAC_TAG_BASE + 0xA7)
#define RRC_MAC_TIMESTAMP_REQD_INFO             (RRC_MAC_TAG_BASE + 0xA8)
#define RRC_MAC_TIMESTAMP_INFO                  (RRC_MAC_TAG_BASE + 0xA9)
#define RRC_SCHED_REQD_FOR_SIB8                 (RRC_MAC_TAG_BASE + 0xAA)
/*CDMA2000_CSFB_HO stop*/
/* min_ue_power start */
#define RRC_MAC_PHR_TIMER_INFO                  (RRC_MAC_TAG_BASE + 0xAB)
/* min_ue_power stop */
/* 36321_CR0538 start */
#define RRC_MAC_UE_COMPLIANCE_INFO              (RRC_MAC_TAG_BASE + 0xAC)
/* 36321_CR0538 stop */
/*Transmission_mode_7_8 Start*/
#define RRC_MAC_PDSCH_EPRE_TO_UE_RS_RATIO       (RRC_MAC_TAG_BASE + 0xAF)
/*Transmission_mode_7_8 Stop*/
/* PRS + */
#define RRC_MAC_PRS_CONFIG_INFO			(RRC_MAC_TAG_BASE + 0xB0)
#define	RRC_MAC_PRS_BANDWIDTH_INFO		(RRC_MAC_TAG_BASE + 0xB1)
#define RRC_MAC_PRS_SUBFRAMES_INFO		(RRC_MAC_TAG_BASE + 0xB2)
#define RRC_MAC_PRS_CONFIG_INDEX_INFO		(RRC_MAC_TAG_BASE + 0xB3)
#define RRC_MAC_PRS_TRANSMISSION_POWER_INFO	(RRC_MAC_TAG_BASE + 0xB4)
#define RRC_MAC_PRS_MUTING_CONFIG_INFO		(RRC_MAC_TAG_BASE + 0xB5)
/* PRS - */
/*TTI_BUNDLING Start*/
#define RRC_MAC_TTI_BUNDLING_INFO               (RRC_MAC_TAG_BASE + 0xB6)
/*TTI_BUNDLING Stop*/
/* SPR 23209 FIXED START */
#define RRC_MAC_OPERATOR_INFO                   (RRC_MAC_TAG_BASE + 0xB7)
/* SPR 23209 FIXED END */

/*Dynamic_ue_reconfig +*/
#define RRC_MAC_MAX_UL_HARQ_TX_INFO		(RRC_MAC_TAG_BASE + 0xB8)
#define RRC_MAC_PC_MAX_INFO			(RRC_MAC_TAG_BASE + 0xB9)
/*Dynamic_ue_reconfig -*/
/*CA_START*/
#define RRC_MAC_UE_CATEGORY_V1020		(RRC_MAC_TAG_BASE + 0xBA) 
#define RRC_MAC_PUCCH_FORMAT_V1020		(RRC_MAC_TAG_BASE + 0xBB) 
#define RRC_MAC_PUCCH_CHANNEL_SELECTION_CONFIG_V1020	(RRC_MAC_TAG_BASE + 0xBC)
#define RRC_MAC_N1PUCCH_AN_CS_LIST_R10          (RRC_MAC_TAG_BASE + 0xBD) 
#define RRC_MAC_PUCCH_AN_CS_V1020		(RRC_MAC_TAG_BASE + 0xBE) 
#define RRC_MAC_CA_CONFIG			(RRC_MAC_TAG_BASE + 0xBF) 
#define RRC_MAC_SCELL_DEACTIVATION_TIMER_INFO   (RRC_MAC_TAG_BASE + 0xC0) 
#define RRC_MAC_SCELL_INFO			(RRC_MAC_TAG_BASE + 0xC1) 
#define RRC_MAC_MIMO_CAPABILITY_DL_V1020        (RRC_MAC_TAG_BASE + 0xC2) 
#define RRC_MAC_CODEBOOK_SUBSET_RESTRICTION_V1020   (RRC_MAC_TAG_BASE + 0xC3) 
#define RRC_MAC_CQI_PMI_CONFIG_INDEX_INFO_V1020 (RRC_MAC_TAG_BASE + 0xC4)
#define RRC_MAC_APERIODIC_CSI_TRIGGER_R10       (RRC_MAC_TAG_BASE + 0xC5)
#define RRC_MAC_CREATE_CA_CONFIG_ERROR          (RRC_MAC_TAG_BASE + 0xC6) 
#define RRC_MAC_RECONFIGURE_CA_CONFIG_ERROR     (RRC_MAC_TAG_BASE + 0xC7) 
#define RRC_MAC_DELETE_CA_CONFIG_ERROR          (RRC_MAC_TAG_BASE + 0xC8) 
#define RRC_MAC_DL_NUM_HARQ_PROC                (RRC_MAC_TAG_BASE + 0xC9) 
#define RRC_MAC_DL_INIT_MCS                     (RRC_MAC_TAG_BASE + 0xCA) 
#define RRC_MAC_RESET_UE_ENTITY_SCELL_REL_FLAG  (RRC_MAC_TAG_BASE + 0xCB) 
#define RRC_MAC_SCELL_ACTIVATE_STATUS           (RRC_MAC_TAG_BASE + 0xCC) 
#define RRC_MAC_CA_LC_BANDWIDTH_DIST_INFO       (RRC_MAC_TAG_BASE + 0xCD) 
#define RRC_MAC_DL_INTERFERENCE_INFO            (RRC_MAC_TAG_BASE + 0xCE)
#define RRC_MAC_CQI_CSI_SUBFRAME_PATTERN_CONFIG_V10	(RRC_MAC_TAG_BASE + 0xCF )
#define RRC_MAC_CSI_MEAS_SUBFRAME_SET1_R10      (RRC_MAC_TAG_BASE + 0xD0)
#define RRC_MAC_CSI_CONFIG_INDEX_V10            (RRC_MAC_TAG_BASE + 0xD1)
#define RRC_MAC_CQI_PMI_CONFIG_INDEX2_R10       (RRC_MAC_TAG_BASE + 0xD2)
#define RRC_MAC_CSI_MEAS_SUBFRAME_SET2_R10      (RRC_MAC_TAG_BASE + 0xD3)
#define RRC_MAC_LAA_CELL_CONFIG_EARFCN          (RRC_MAC_TAG_BASE + 0xD4)
#define RRC_MAC_DL_EARFCN                       (RRC_MAC_TAG_BASE + 0xD5)
#define RRC_MAC_LAA_SCELL_CONFIG_INFO           (RRC_MAC_TAG_BASE + 0xD6)
#define RRC_MAC_DMTC_CONFIG_INFO                (RRC_MAC_TAG_BASE + 0xD7)
/* SPR 21958 PUSCH RAT1 Support Start */
#define RRC_MAC_RAT1_INFO			(RRC_MAC_TAG_BASE + 0xD8)
/* SPR 21958 PUSCH RAT1 Support End */
#define RRC_M2AP_MAC_SIB13_INFO                             (RRC_MAC_TAG_BASE + 0xD9)
#define RRC_M2AP_MBMS_CREATE_AREA_INFO                      (RRC_MAC_TAG_BASE + 0xDA)
#define RRC_M2AP_MAC_MCCH_CONFIG                            (RRC_MAC_TAG_BASE + 0xDB)
#define RRC_M2AP_MAC_NOTIFICATION_CONFIG                    (RRC_MAC_TAG_BASE + 0xDC)
#define RRC_M2AP_MAC_AREA_CONFIG_INFO                       (RRC_MAC_TAG_BASE + 0xDD)
#define RRC_M2AP_MAC_CSA_PATTERN_LIST                       (RRC_MAC_TAG_BASE + 0xDE)
#define RRC_M2AP_MAC_PMCH_CONFIG                            (RRC_MAC_TAG_BASE + 0xDF)
#define RRC_M2AP_MAC_CREATE_MBMS_AREA_ENTITY_INFO           (RRC_MAC_TAG_BASE + 0xE0)
#define RRC_M2AP_MAC_DELETE_MBMS_AREA_ENTITY_INFO           (RRC_MAC_TAG_BASE + 0xE1)
#define RRC_M2AP_MAC_EMBMS_CONFIG                           (RRC_MAC_TAG_BASE + 0xE2)
#define RRC_M2AP_MAC_EMBMS_TRANSMISSION_POWER_INFO          (RRC_MAC_TAG_BASE + 0xE3)
#define RRC_M2AP_MAC_RESET_SESSION_INFO                     (RRC_MAC_TAG_BASE + 0xE4)
#define RRC_M2AP_MAC_PMCH_RESET_LC_INFO                     (RRC_MAC_TAG_BASE + 0xE5)
#define RRC_M2AP_MAC_EMBMS_SIB2_INFO                        (RRC_MAC_TAG_BASE + 0xE6)

/*eMTC Changes start*/

#define RRC_MAC_RACH_CE_LEVEL_INFO_LIST                 (RRC_MAC_TAG_BASE + 0x152)
#define RRC_MAC_PRACH_CONFIG_V1310			            (RRC_MAC_TAG_BASE + 0x153)
#define RRC_MAC_PRACH_HOPPING_OFFSET_R13	            (RRC_MAC_TAG_BASE + 0x154)
#define RRC_MAC_PUSCH_CONFIG_COMMON_V1310			    (RRC_MAC_TAG_BASE + 0x155)
#define RRC_MAC_PDSCH_CONFIG_COMMON_V1310			    (RRC_MAC_TAG_BASE + 0x156)
#define RRC_MAC_PRACH_PARAMETERS_CE_R13_LIST            (RRC_MAC_TAG_BASE + 0x157)
#define RRC_MAC_CE_LEVEL                                (RRC_MAC_TAG_BASE + 0x158)
#define RRC_MAC_CE_MODE                                 (RRC_MAC_TAG_BASE + 0x159)
#define RRC_MAC_SFN_GAP_INFO_V1310                      (RRC_MAC_TAG_BASE + 0x15A)
#define RRC_MAC_PRACH_STARTING_SUBFRAME_R13		        (RRC_MAC_TAG_BASE + 0x15B)
#define RRC_MAC_FDD_UPNLINK_SF_BITMAP_BR                (RRC_MAC_TAG_BASE + 0x15C)
#define RRC_MAC_MODIFICATION_PERIOD_V1310               (RRC_MAC_TAG_BASE + 0x15D)
#define RRC_MAC_MAX_NUM_PREAMBLE_ATTEMPT_CE_R13		    (RRC_MAC_TAG_BASE + 0x15E)
#define RRC_MAC_MPDCCH_START_SF_CSS_RA		            (RRC_MAC_TAG_BASE + 0x15F)
#define RRC_MAC_N1_PUCCH_AN_INFO_LIST                   (RRC_MAC_TAG_BASE + 0x160)
#define RRC_MAC_PUCCH_CONFIG_COMMON_V1310			    (RRC_MAC_TAG_BASE + 0x161)
#define RRC_MAC_PUCCH_CONFIG_DEDICATED_R13              (RRC_MAC_TAG_BASE + 0x162)
#define RRC_MAC_PUCCH_NUM_REPETITION_CE_R13             (RRC_MAC_TAG_BASE + 0x163)
#define RRC_MAC_PUCCH_NUM_REPETITION_CE_SETUP_R13       (RRC_MAC_TAG_BASE + 0x164)
#define RRC_MAC_ACK_NACK_REPETITION_R13                 (RRC_MAC_TAG_BASE + 0x165)
#define RRC_MAC_ACK_NACK_REPETITION_SETUP_R13           (RRC_MAC_TAG_BASE + 0x166)
#define RRC_MAC_EPDCCH_SET_CONFIG_R11                   (RRC_MAC_TAG_BASE + 0x167)
#define RRC_MAC_MPDCCH_CONFIG_R13                       (RRC_MAC_TAG_BASE + 0x168)
#define RRC_MAC_MPDCCH_CONFIG_SETUP_R13                 (RRC_MAC_TAG_BASE + 0x169)
#define RRC_MAC_SIBTYPE1_BR_MSG_INFO                	(RRC_MAC_TAG_BASE + 0x16A) 
#define RRC_MAC_SI_MSG_BR_INFO                     	    (RRC_MAC_TAG_BASE + 0x16B)
#define RRC_MAC_SI_BR_MSG_REQ                           (RRC_MAC_TAG_BASE + 0x16C)
#define RRC_MAC_FDD_DOWNLINK_TDD_SF_BITMAP_BR	        (RRC_MAC_TAG_BASE + 0x16D)
#define RRC_MAC_UE_CATEGORY_V1310                       (RRC_MAC_TAG_BASE + 0x16E)
#define RRC_MAC_PAGING_NARROWBAND                       (RRC_MAC_TAG_BASE + 0x16F)
#define RRC_MAC_MPDCCH_NUM_REPETITION_PAGING            (RRC_MAC_TAG_BASE + 0x170)
#define RRC_MAC_PDSCH_NUM_REPETITION_PAGING             (RRC_MAC_TAG_BASE + 0x171) 
#define RRC_MAC_RACH_CONFIGURATION_R13                  (RRC_MAC_TAG_BASE + 0x172)
#define RRC_MAC_FREQUANCY_HOPPING_PARAMETERS_R13        (RRC_MAC_TAG_BASE + 0x173)
#define RRC_MAC_EMTC_DATA_CONFIG                        (RRC_MAC_TAG_BASE + 0x174)
#define RRC_MAC_DIRECT_INDICATION                       (RRC_MAC_TAG_BASE + 0x175)
#define RRC_MAC_PCCH_CONFIG_V1310                       (RRC_MAC_TAG_BASE + 0x176)
#define RRC_MAC_SFN_EMTC                                (RRC_MAC_TAG_BASE + 0x177)
#define RRC_MAC_MAX_TAG                                 RRC_MAC_SFN_EMTC
/*eMTC Changes stop*/

/********************************************************************
 * RRC - RLC APIs
 *******************************************************************/
#define RRC_RLC_API_BASE                     0x0000

#define RRC_RLC_CREATE_UE_ENTITY_REQ         (RRC_RLC_API_BASE + 1)
#define RRC_RLC_CREATE_UE_ENTITY_CNF         (RRC_RLC_API_BASE + 2)
#define RRC_RLC_RECONFIG_UE_ENTITY_REQ       (RRC_RLC_API_BASE + 3)
#define RRC_RLC_RECONFIG_UE_ENTITY_CNF       (RRC_RLC_API_BASE + 4)
#define RRC_RLC_DELETE_UE_ENTITY_REQ         (RRC_RLC_API_BASE + 5)
#define RRC_RLC_DELETE_UE_ENTITY_CNF         (RRC_RLC_API_BASE + 6)
#define RRC_RLC_RE_ESTABLISH_UE_ENTITY_REQ   (RRC_RLC_API_BASE + 7)
#define RRC_RLC_RE_ESTABLISH_UE_ENTITY_CNF   (RRC_RLC_API_BASE + 8)
#define RRC_RLC_COMMON_CHANNEL_DATA_REQ      (RRC_RLC_API_BASE + 9)
#define RRC_RLC_COMMON_CHANNEL_DATA_IND      (RRC_RLC_API_BASE + 10)
#define RRC_RLC_UE_ENTITY_ERROR_IND          (RRC_RLC_API_BASE + 12)
#define RRC_RLC_CHANGE_CRNTI_REQ             (RRC_RLC_API_BASE + 13)
#define RRC_RLC_CHANGE_CRNTI_CNF             (RRC_RLC_API_BASE + 14)
#define RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND (RRC_RLC_API_BASE + 15)
/*SPR:6462 start*/
#define RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF (RRC_RLC_API_BASE + 16)

/*SPR:6462 stop*/
#define RRC_RLC_CELL_CONFIG_REQ			(RRC_RLC_API_BASE + 17)
#define RRC_RLC_CELL_CONFIG_CNF			(RRC_RLC_API_BASE + 18)
#define RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_REQ    (RRC_RLC_API_BASE + 19)
#define RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_CNF    (RRC_RLC_API_BASE + 20)
#define RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_REQ    (RRC_RLC_API_BASE + 21)
#define RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_CNF    (RRC_RLC_API_BASE + 22)
#define RRC_M2AP_RLC_AREA_CONFIG_REQ                (RRC_RLC_API_BASE + 23)
#define RRC_M2AP_RLC_AREA_CONFIG_RESP               (RRC_RLC_API_BASE + 24)
#define RRC_M2AP_RLC_MCCH_IND                       (RRC_RLC_API_BASE + 25)
#ifdef LTE_EMBMS_SUPPORTED
#define RRC_RLC_MAX_API                              RRC_M2AP_RLC_MCCH_IND
#else
#define RRC_RLC_MAX_API                      RRC_RLC_CELL_CONFIG_CNF
#endif
/* coverity 92915 */
#define M2AP_RLC_API_BASE RRC_RLC_CELL_CONFIG_CNF
/* coverity 92915 */

/******************************************************************************
 * RRC - RLC TAGs
 *****************************************************************************/
#define RRC_RLC_TAG_BASE                        11

#define RRC_RLC_CREATE_TX_UM_RLC_ENTITY         (RRC_RLC_TAG_BASE + 0)
#define RRC_RLC_CREATE_RX_UM_RLC_ENTITY         (RRC_RLC_TAG_BASE + 1)
#define RRC_RLC_CREATE_TX_RX_UM_RLC_ENTITY      (RRC_RLC_TAG_BASE + 2)
#define RRC_RLC_CREATE_TX_RX_AM_RLC_ENTITY      (RRC_RLC_TAG_BASE + 3)

#define RRC_RLC_DELETE_TX_UM_RLC_ENTITY         (RRC_RLC_TAG_BASE + 6)
#define RRC_RLC_DELETE_RX_UM_RLC_ENTITY         (RRC_RLC_TAG_BASE + 7)
#define RRC_RLC_DELETE_TX_RX_UM_RLC_ENTITY      (RRC_RLC_TAG_BASE + 8)
#define RRC_RLC_DELETE_TX_RX_AM_RLC_ENTITY      (RRC_RLC_TAG_BASE + 9)

#define RRC_RLC_RECONFIG_TX_UM_RLC_ENTITY       (RRC_RLC_TAG_BASE + 10)
#define RRC_RLC_RECONFIG_RX_UM_RLC_ENTITY       (RRC_RLC_TAG_BASE + 11)
#define RRC_RLC_RECONFIG_TX_RX_UM_RLC_ENTITY    (RRC_RLC_TAG_BASE + 12)
#define RRC_RLC_RECONFIG_TX_RX_AM_RLC_ENTITY    (RRC_RLC_TAG_BASE + 13)

#define RRC_RLC_ENTITY_LCID                     (RRC_RLC_TAG_BASE + 17)

#define RRC_RLC_CREATE_ENTITY_ERROR             (RRC_RLC_TAG_BASE + 18)
#define RRC_RLC_DELETE_ENTITY_ERROR             (RRC_RLC_TAG_BASE + 19)
#define RRC_RLC_RECONFIG_ENTITY_ERROR           (RRC_RLC_TAG_BASE + 20)
#define RRC_RLC_ENTITY_LC_ID_ERROR              (RRC_RLC_TAG_BASE + 21)

#define RRC_RLC_UE_BIT_RATE_INFO                (RRC_RLC_TAG_BASE + 22)
#define RRC_RLC_QCI_INFO                        (RRC_RLC_TAG_BASE + 23)
#define RRC_RLC_TX_BIT_RATE_INFO                (RRC_RLC_TAG_BASE + 24)
#define RRC_RLC_RX_BIT_RATE_INFO                (RRC_RLC_TAG_BASE + 25)
#define RRC_RLC_QUEUE_SIZE_INFO                 (RRC_RLC_TAG_BASE + 26)

/* SPS Start */
#define RRC_RLC_SPS_CONFIG_TAG                  (RRC_RLC_TAG_BASE + 27)
#define RRC_RLC_SPS_DL_INFO_TAG                 (RRC_RLC_TAG_BASE + 28)
#define RRC_RLC_SPS_DL_SETUP_INFO_TAG           (RRC_RLC_TAG_BASE + 29)
#define RRC_RLC_SPS_UL_INFO_TAG                 (RRC_RLC_TAG_BASE + 30)
#define RRC_RLC_SPS_UL_SETUP_INFO_TAG           (RRC_RLC_TAG_BASE + 31)
/* SPS Stop */
#define RRC_RLC_LC_STATE                        (RRC_RLC_TAG_BASE + 32)

#define  M2AP_RLC_AREA_CONFIG_INFO              (RRC_RLC_TAG_BASE + 33)
#define  M2AP_RLC_ADD_LC_INFO                   (RRC_RLC_TAG_BASE + 34)
#define  M2AP_RLC_DELETE_LC_INFO                (RRC_RLC_TAG_BASE + 35)
/* OPTION3X Changes */
#define RRC_RLC_DC_BEARER_TYPE_INFO		        (RRC_RLC_TAG_BASE + 37)
#define RRC_RLC_MAX_TAG                         RRC_RLC_DC_BEARER_TYPE_INFO 
/* OPTION3X Changes */


//#define RRC_RLC_MAX_TAG                         M2AP_RLC_DELETE_LC_INFO

/********************************************************************
 * RRC - PDCP APIs
 *******************************************************************/
#define RRC_PDCP_API_BASE                     0x0000

#define RRC_PDCP_REQ_RESP_OFFSET              (RRC_PDCP_API_BASE + 4) 

#define RRC_PDCP_CREATE_UE_ENTITY_CNF         (RRC_PDCP_API_BASE + 0)
#define RRC_PDCP_RECONFIG_UE_ENTITY_CNF       (RRC_PDCP_API_BASE + 1)
#define RRC_PDCP_DELETE_UE_ENTITY_CNF         (RRC_PDCP_API_BASE + 2)
#define RRC_PDCP_SRB_DATA_IND                 (RRC_PDCP_API_BASE + 3)
#define RRC_PDCP_SRB_DATA_STATUS_IND          (RRC_PDCP_API_BASE + 4)
#define RRC_PDCP_SN_HFN_STATUS_REQ            (RRC_PDCP_API_BASE + 5)
#define RRC_PDCP_SN_HFN_STATUS_RESP           (RRC_PDCP_API_BASE + 6)
#define RRC_PDCP_SN_HFN_STATUS_IND            (RRC_PDCP_API_BASE + 7)
#define RRC_PDCP_DATA_BUFFER_STOP_IND         (RRC_PDCP_API_BASE + 8) 
#define RRC_PDCP_MAC_I_REQ                    (RRC_PDCP_API_BASE + 9)
#define RRC_PDCP_MAC_I_RESP                   (RRC_PDCP_API_BASE + 10)
#define RRC_PDCP_SUSPEND_UE_ENTITY_REQ        (RRC_PDCP_API_BASE + 11)
#define RRC_PDCP_SUSPEND_UE_ENTITY_CNF        (RRC_PDCP_API_BASE + 12)
#define RRC_PDCP_RE_ESTABLISH_UE_ENTITY_REQ   (RRC_PDCP_API_BASE + 13)
#define RRC_PDCP_RE_ESTABLISH_UE_ENTITY_CNF   (RRC_PDCP_API_BASE + 14)
#define RRC_PDCP_RESUME_UE_ENTITY_REQ         (RRC_PDCP_API_BASE + 15)
#define RRC_PDCP_RESUME_UE_ENTITY_CNF         (RRC_PDCP_API_BASE + 16)
#define RRC_PDCP_CHANGE_CRNTI_REQ             (RRC_PDCP_API_BASE + 17)
#define RRC_PDCP_CHANGE_CRNTI_CNF             (RRC_PDCP_API_BASE + 18)
#define RRC_PDCP_COUNT_WRAPAROUND_IND         (RRC_PDCP_API_BASE + 19)

/* SPR 4118 Start */
#define RRC_PDCP_NOTIFY_INTEGRITY_FAILURE     (RRC_PDCP_API_BASE + 20)
/* SPR 4118 Stop */
#define RRC_PDCP_DRB_COUNT_MSB_REQ            (RRC_PDCP_API_BASE + 21)
#define RRC_PDCP_DRB_COUNT_MSB_RESP           (RRC_PDCP_API_BASE + 22)

#define RRC_PDCP_CREATE_UE_ENTITY_REQ         (RRC_PDCP_API_BASE + 23)
#define RRC_PDCP_RECONFIG_UE_ENTITY_REQ       (RRC_PDCP_API_BASE + 24)
#define RRC_PDCP_DELETE_UE_ENTITY_REQ         (RRC_PDCP_API_BASE + 25)
#define RRC_PDCP_SRB_DATA_REQ                 (RRC_PDCP_API_BASE + 26)
#define RRC_PDCP_UPDATE_LWA_TRAFFIC_IND       (RRC_PDCP_API_BASE + 27)
#define RRC_PDCP_CELL_CONFIG_REQ              (RRC_PDCP_API_BASE + 28)
#define RRC_PDCP_CELL_CONFIG_CNF              (RRC_PDCP_API_BASE + 29)
/*SPR_19066_START*/
#define RRC_PDCP_HO_PREP_INFO_REQ             (RRC_PDCP_API_BASE + 30)
#define RRC_PDCP_HO_PREP_INFO_RESP            (RRC_PDCP_API_BASE + 31)
#define RRC_PDCP_INACTIVE_UES_IND             (RRC_PDCP_API_BASE + 32)
#define RRC_PDCP_MAX_API                      RRC_PDCP_INACTIVE_UES_IND
/*SPR_19066_END*/

/******************************************************************************
 * RRC - PDCP TAGs
 *****************************************************************************/
#define RRC_PDCP_TAG_BASE                           0

#define RRC_PDCP_CREATE_SRB_ENTITY_TAG              (RRC_PDCP_TAG_BASE + 0)
#define RRC_PDCP_CREATE_DRB_ENTITY_TAG              (RRC_PDCP_TAG_BASE + 1)
#define RRC_PDCP_CONFIGURE_INTEGRITY_PROTECTION_TAG (RRC_PDCP_TAG_BASE + 2)
#define RRC_PDCP_CONFIGURE_DISCARD_TIMER_TAG        (RRC_PDCP_TAG_BASE + 3)
#define RRC_PDCP_CONFIGURE_SN_SIZE_TAG              (RRC_PDCP_TAG_BASE + 4)
#define RRC_PDCP_CONFIGURE_ROHC_TAG                 (RRC_PDCP_TAG_BASE + 5)
#define RRC_PDCP_CONFIGURE_SRB_CIPHERING_TAG        (RRC_PDCP_TAG_BASE + 6)
#define RRC_PDCP_CONFIGURE_ST_REPORT_REQUIRED_TAG   (RRC_PDCP_TAG_BASE + 7)
#define RRC_PDCP_DELETE_SRB_ENTITY_TAG              (RRC_PDCP_TAG_BASE + 8)
#define RRC_PDCP_DELETE_DRB_ENTITY_TAG              (RRC_PDCP_TAG_BASE + 9)
#define RRC_PDCP_RECONFIG_SRB_ENTITY_TAG            (RRC_PDCP_TAG_BASE + 10)
#define RRC_PDCP_RECONFIG_DRB_ENTITY_TAG            (RRC_PDCP_TAG_BASE + 11)
#define RRC_PDCP_CONFIGURE_DRB_CIPHERING_TAG        (RRC_PDCP_TAG_BASE + 12)
#define RRC_PDCP_HO_TRIGGER_TAG                     (RRC_PDCP_TAG_BASE + 13)
#define RRC_PDCP_LC_SN_HFN_STATUS_TAG               (RRC_PDCP_TAG_BASE + 14)
#define RRC_PDCP_UL_RECV_SN_TAG                     (RRC_PDCP_TAG_BASE + 15)
#define RRC_PDCP_MESSAGE_TAG                        (RRC_PDCP_TAG_BASE + 16)
#define RRC_PDCP_MSG_AUTHENTICATION_CODE_TAG        (RRC_PDCP_TAG_BASE + 17)
#define RRC_PDCP_RB_ENTITY_TAG                      (RRC_PDCP_TAG_BASE + 18)
/* PSR Start */
#define RRC_PDCP_CONFIGURE_UE_PSR_TAG               (RRC_PDCP_TAG_BASE + 19)
#define RRC_PDCP_SECURITY_OFFSET_TAG                (RRC_PDCP_TAG_BASE + 20)
/* PSR End */
/* SPS Start */
#define RRC_PDCP_SPS_CONFIG_TAG                     (RRC_PDCP_TAG_BASE + 21)
#define RRC_PDCP_SPS_DL_INFO_TAG                    (RRC_PDCP_TAG_BASE + 22)
#define RRC_PDCP_SPS_DL_SETUP_INFO_TAG              (RRC_PDCP_TAG_BASE + 23)
#define RRC_PDCP_SPS_UL_INFO_TAG                    (RRC_PDCP_TAG_BASE + 24)
#define RRC_PDCP_SPS_UL_SETUP_INFO_TAG              (RRC_PDCP_TAG_BASE + 25)
/* SPS Stop */
/* Tags 21 to 25 Reserved for SPS */
#define RRC_PDCP_DRB_COUNTMSB_DRB_LIST_TAG          (RRC_PDCP_TAG_BASE + 26)
#define RRC_PDCP_DRB_COUNTMSB_INFO_TAG              (RRC_PDCP_TAG_BASE + 27)
#define RRC_PDCP_COUNTMSB_UPLINK_TAG                (RRC_PDCP_TAG_BASE + 28)
#define RRC_PDCP_COUNTMSB_DOWNLINK_TAG              (RRC_PDCP_TAG_BASE + 29)
/* lossy ho changes start */
#define RRC_PDCP_LOSSY_HO_REQUIRED_TAG              (RRC_PDCP_TAG_BASE + 30)
/* lossy ho changes stop */
/*SPR_17121_START*/
#define RRC_PDCP_MACI_REQ_TAG                       (RRC_PDCP_TAG_BASE + 31)
/*SPR_17121_END*/

#define RRC_PDCP_CONFIGURE_LWA_TAG                  (RRC_PDCP_TAG_BASE + 32)
#define RRC_PDCP_LWA_CONFIG_BEARER_TYPE_TAG         (RRC_PDCP_TAG_BASE + 33)
/*SPR_19066_START*/
#define RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_TAG     (RRC_PDCP_TAG_BASE + 34)
#define RRC_PDCP_UE_INACTIVE_TIME_LAPSED_TAG        (RRC_PDCP_TAG_BASE + 35)
#define RRC_PDCP_UE_INACTIVE_TIME_REQ_TAG           (RRC_PDCP_TAG_BASE + 36)
#define RRC_PDCP_MACI_HO_REQ_TAG                    (RRC_PDCP_TAG_BASE + 37)
/*SPR_19066_END*/

#define RRC_PDCP_CREATE_SRB_ENTITY_ERROR_TAG        (RRC_PDCP_TAG_BASE + 0)
#define RRC_PDCP_CREATE_DRB_ENTITY_ERROR_TAG        (RRC_PDCP_TAG_BASE + 1)
#define RRC_PDCP_DELETE_SRB_ENTITY_ERROR_TAG        (RRC_PDCP_TAG_BASE + 2)
#define RRC_PDCP_DELETE_DRB_ENTITY_ERROR_TAG        (RRC_PDCP_TAG_BASE + 3)
#define RRC_PDCP_RECONFIG_SRB_ENTITY_ERROR_TAG      (RRC_PDCP_TAG_BASE + 4)
#define RRC_PDCP_RECONFIG_DRB_ENTITY_ERROR_TAG      (RRC_PDCP_TAG_BASE + 5)
#define RRC_PDCP_SRB_DATA_STATUS_ERROR_TAG          (RRC_PDCP_TAG_BASE + 6)
#define RRC_PDCP_RB_ENTITY_ERROR_TAG                (RRC_PDCP_TAG_BASE + 7)
/*SPR_17121_START*/
#define RRC_PDCP_MACI_RESP_TAG                      (RRC_PDCP_TAG_BASE + 8)
/*SPR_17121_END*/
/*SPR_19066_START*/
#define RRC_PDCP_UE_INACTIVE_TIME_RESP_TAG          (RRC_PDCP_TAG_BASE + 9)
#define RRC_PDCP_INACTIVE_UE_INFO_TAG               (RRC_PDCP_TAG_BASE + 10)

#define RRC_PDCP_MAX_TAG                            RRC_PDCP_MACI_HO_REQ_TAG
/*SPR_19066_END*/

/********************************************************************
 * RRC - PHY APIs
 *******************************************************************/
#define RRC_PHY_API_BASE                      0x0000

#define RRC_PHY_CONFIG_CELL_REQ               (RRC_PHY_API_BASE + 1)
#define RRC_PHY_CONFIG_CELL_CNF               (RRC_PHY_API_BASE + 2)
#define RRC_PHY_DELETE_CELL_REQ               (RRC_PHY_API_BASE + 3)
#define RRC_PHY_DELETE_CELL_CNF               (RRC_PHY_API_BASE + 4)
#define RRC_PHY_CREATE_UE_ENTITY_REQ          (RRC_PHY_API_BASE + 5)
#define RRC_PHY_CREATE_UE_ENTITY_CNF          (RRC_PHY_API_BASE + 6)
#define RRC_PHY_DELETE_UE_ENTITY_REQ          (RRC_PHY_API_BASE + 7)
#define RRC_PHY_DELETE_UE_ENTITY_CNF          (RRC_PHY_API_BASE + 8)
#define RRC_PHY_RECONFIG_UE_ENTITY_REQ        (RRC_PHY_API_BASE + 9)
#define RRC_PHY_RECONFIG_UE_ENTITY_CNF        (RRC_PHY_API_BASE + 10)
#define RRC_PHY_RECONFIG_CELL_REQ             (RRC_PHY_API_BASE + 11)
#define RRC_PHY_RECONFIG_CELL_CNF             (RRC_PHY_API_BASE + 12)
#define RRC_PHY_CHANGE_CRNTI_REQ              (RRC_PHY_API_BASE + 13) 
#define RRC_PHY_CHANGE_CRNTI_CNF              (RRC_PHY_API_BASE + 14) 
#define RRC_PHY_CELL_START_REQ                (RRC_PHY_API_BASE + 15) 
#define RRC_PHY_CELL_START_CNF                (RRC_PHY_API_BASE + 16) 
#define RRC_PHY_CELL_STOP_REQ                 (RRC_PHY_API_BASE + 17) 
#define RRC_PHY_CELL_STOP_CNF                 (RRC_PHY_API_BASE + 18) 

#define RRC_PHY_MAX_API                       RRC_PHY_CELL_STOP_CNF

/********************************************************************
 * RRC - S1U APIs
 *******************************************************************/
#define RRC_S1U_API_BASE                    0x0000

#define RRC_S1U_CREATE_UE_ENTITY_REQ        (RRC_S1U_API_BASE + 1)
#define RRC_S1U_CREATE_UE_ENTITY_CNF        (RRC_S1U_API_BASE + 2)
#define RRC_S1U_DELETE_UE_ENTITY_REQ        (RRC_S1U_API_BASE + 3)
#define RRC_S1U_DELETE_UE_ENTITY_CNF        (RRC_S1U_API_BASE + 4)
#define RRC_S1U_RECONFIGURE_UE_ENTITY_REQ   (RRC_S1U_API_BASE + 5)
#define RRC_S1U_RECONFIGURE_UE_ENTITY_CNF   (RRC_S1U_API_BASE + 6)
#define RRC_S1U_ERROR_IND                   (RRC_S1U_API_BASE + 7)
#define RRC_S1U_PATH_FAILURE_IND            (RRC_S1U_API_BASE + 8)
#define RRC_S1U_END_MARKER_IND              (RRC_S1U_API_BASE + 9)
#define RRC_S1U_PATH_SUCCESS_IND            (RRC_S1U_API_BASE + 10)
/*BUG 604 changes start*/
#define RRC_S1U_INTRA_ENB_DATA_FWD_REQ      (RRC_S1U_API_BASE + 11)
#define RRC_S1U_INTRA_ENB_DATA_FWD_CNF      (RRC_S1U_API_BASE + 12)
#define RRC_S1U_LOCAL_PATH_SWITCH_REQ       (RRC_S1U_API_BASE + 13)
#define RRC_S1U_LOCAL_PATH_SWITCH_CNF       (RRC_S1U_API_BASE + 14)

#define RRC_S1U_CELL_CONFIG_REQ 			(RRC_S1U_API_BASE + 15)
#define RRC_S1U_CELL_CONFIG_CNF 			(RRC_S1U_API_BASE + 16)
#define RRC_M2AP_M1U_SESSION_START_REQ      (RRC_S1U_API_BASE + 17)
#define RRC_M2AP_M1U_SESSION_START_CNF      (RRC_S1U_API_BASE + 18)
#define RRC_M2AP_M1U_SESSION_STOP_REQ       (RRC_S1U_API_BASE + 19)
#define RRC_M2AP_M1U_SESSION_STOP_CNF       (RRC_S1U_API_BASE + 20)
#define RRC_M2AP_M1U_SESSION_UPDATE_REQ     (RRC_S1U_API_BASE + 21)
#define RRC_M2AP_M1U_SESSION_UPDATE_CNF     (RRC_S1U_API_BASE + 22)
#define RRC_M2AP_M1U_CREATE_AREA_ENTITY_REQ (RRC_S1U_API_BASE + 23)
#define RRC_M2AP_M1U_CREATE_AREA_ENTITY_CNF (RRC_S1U_API_BASE + 24)
#define RRC_M2AP_M1U_DELETE_AREA_ENTITY_REQ (RRC_S1U_API_BASE + 25)
#define RRC_M2AP_M1U_DELETE_AREA_ENTITY_CNF (RRC_S1U_API_BASE + 26)
#define RRC_M2AP_M1U_MODIFY_AREA_ENTITY_REQ (RRC_S1U_API_BASE + 27)
#define RRC_M2AP_M1U_MODIFY_AREA_ENTITY_CNF (RRC_S1U_API_BASE + 28)

/* SPR 1069 Start */
#define RRC_S1U_MAX_API                    RRC_M2AP_M1U_MODIFY_AREA_ENTITY_CNF
/*BUG 604 changes stop*/
/* SPR 1069 Stop */
/* coverity 92910 */
#define M2AP_S1U_API_BASE RRC_S1U_LOCAL_PATH_SWITCH_CNF
/* coverity 92910 */


/********************************************************************
 * RRC - S1U TAGs
 *******************************************************************/
#define RRC_S1U_TAG_BASE                    0

#define RRC_S1U_IE_SEQ_NUM_TAG              (RRC_S1U_TAG_BASE + 1)
#define RRC_S1U_IE_REORDERING_REQD_TAG      (RRC_S1U_TAG_BASE + 2)
#define RRC_S1U_IE_SEQ_DISABLE_TAG          (RRC_S1U_TAG_BASE + 3)
#define RRC_S1U_IE_TEID_DATA_I_TAG          (RRC_S1U_TAG_BASE + 4)
#define RRC_S1U_IE_GSN_ADDR_TAG             (RRC_S1U_TAG_BASE + 5)
#define RRC_S1U_IE_QOS_PROFILE_TAG          (RRC_S1U_TAG_BASE + 6)
#define RRC_S1U_IE_RELAY_SETUP_SAP_REQ_TAG  (RRC_S1U_TAG_BASE + 7)
#define RRC_S1U_IE_RELAY_SETUP_SAP_CNF_TAG  (RRC_S1U_TAG_BASE + 8)
#define RRC_S1U_IE_RELAY_REL_SAP_REQ_TAG    (RRC_S1U_TAG_BASE + 9)
#define RRC_S1U_IE_RELAY_REL_SAP_CNF_TAG    (RRC_S1U_TAG_BASE + 10)

#define RRC_S1U_IE_TUNNEL_INFO_TAG          (RRC_S1U_TAG_BASE + 11)
#define RRC_S1U_IE_TEID_SELF_TAG            (RRC_S1U_TAG_BASE + 12)
#define RRC_S1U_IE_BUFFER_IND_TAG           (RRC_S1U_TAG_BASE + 13)
#define RRC_S1U_IE_TEID_PEER_TAG            (RRC_S1U_TAG_BASE + 14)
#define RRC_S1U_IE_RELEASE_TUNNEL_INFO_TAG  (RRC_S1U_TAG_BASE + 15)
#define RRC_S1U_IE_TUNNEL_INFO_CNF_TAG      (RRC_S1U_TAG_BASE + 16)
#define RRC_S1U_IE_PDCP_EXT_TAG             (RRC_S1U_TAG_BASE + 17)
#define RRC_S1U_IE_DRBID_SELF_TAG           (RRC_S1U_TAG_BASE + 18)
#define RRC_M2AP_M1U_TUNNEL_INFO_LIST       (RRC_S1U_TAG_BASE + 19)
#define RRC_M2AP_M1U_AREA_LC_LIST           (RRC_S1U_TAG_BASE + 20)
#define RRC_M2AP_M1U_TEID_CONFIG_INFO_LIST  (RRC_S1U_TAG_BASE + 21)
//#ifdef ENDC_ENABLED
/* OPTION3X Changes */
#define RRC_S1U_IE_DC_BEARER_FLAG           (RRC_S1U_API_BASE + 22)
#define RRC_S1U_IE_SAP_PARAM_TAG            (RRC_S1U_API_BASE + 23)
/* OPTION3X Changes */
//#endif
//#define RRC_S1U_MAX_TAG                     SESSION_STOP_INFO_LIST

/********************************************************************
 * RRC - RRM APIs
 *******************************************************************/
#define RRC_RRM_API_BASE                      0x0000

#define RRC_RRM_UE_RELEASE_REQ                (RRC_RRM_API_BASE + 1)
#define RRC_RRM_UE_RELEASE_RESP               (RRC_RRM_API_BASE + 2)
#define RRC_RRM_CELL_SETUP_REQ                (RRC_RRM_API_BASE + 3)
#define RRC_RRM_CELL_SETUP_RESP               (RRC_RRM_API_BASE + 4)
#define RRC_RRM_CELL_DELETE_REQ               (RRC_RRM_API_BASE + 5)
#define RRC_RRM_CELL_DELETE_RESP              (RRC_RRM_API_BASE + 6)
#define RRC_RRM_UE_ADMISSION_REQ              (RRC_RRM_API_BASE + 7)
#define RRC_RRM_UE_ADMISSION_RESP             (RRC_RRM_API_BASE + 8)
#define RRC_RRM_UE_ADMISSION_CNF              (RRC_RRM_API_BASE + 9)
#define RRC_RRM_ERB_SETUP_REQ                 (RRC_RRM_API_BASE + 10)
#define RRC_RRM_ERB_SETUP_RESP                (RRC_RRM_API_BASE + 11)
#define RRC_RRM_ERB_SETUP_CNF                 (RRC_RRM_API_BASE + 12)
#define RRC_RRM_ERB_RELEASE_REQ               (RRC_RRM_API_BASE + 13)
#define RRC_RRM_ERB_RELEASE_RESP              (RRC_RRM_API_BASE + 14)
#define RRC_RRM_UE_CONNECTION_RELEASE_IND     (RRC_RRM_API_BASE + 15)
#define RRC_RRM_MEASURMENT_RESULTS_IND        (RRC_RRM_API_BASE + 16)
#define RRC_RRM_CELL_RECONFIGURE_REQ          (RRC_RRM_API_BASE + 17)
#define RRC_RRM_CELL_RECONFIG_RESP            (RRC_RRM_API_BASE + 18)
#define RRC_RRM_UE_CAPABILITY_ENQUIRY_REQ     (RRC_RRM_API_BASE + 19)
#define RRC_RRM_UE_CAPABILITY_ENQUIRY_RESP    (RRC_RRM_API_BASE + 20)
#define RRC_RRM_UE_CAPABILITY_IND             (RRC_RRM_API_BASE + 21)

/* Start: E-RAB Modify API */
#define RRC_RRM_ERB_MODIFY_REQ                (RRC_RRM_API_BASE + 22)
#define RRC_RRM_ERB_MODIFY_RESP               (RRC_RRM_API_BASE + 23)
#define RRC_RRM_ERB_MODIFY_CNF                (RRC_RRM_API_BASE + 24)
/* End: E-RAB Modify API */

/* ERB RELEASE COMMAND START */
#define RRC_RRM_ERB_RELEASE_CNF               (RRC_RRM_API_BASE + 25)
#define RRC_RRM_UE_CONTEXT_MOD_REQ              (RRC_RRM_API_BASE + 26)
#define RRC_RRM_UE_CONTEXT_MOD_RESP              (RRC_RRM_API_BASE + 27)
#define RRC_RRM_ERB_RELEASE_IND                  (RRC_RRM_API_BASE + 28)
#define RRC_RRM_UE_CONTEXT_MOD_CNF             (RRC_RRM_API_BASE + 29)

/*Handover Start*/
#define RRC_RRM_HO_REQUIRED                   (RRC_RRM_API_BASE + 30)
#define RRC_RRM_UE_HO_ADM_REQ                 (RRC_RRM_API_BASE + 31)
#define RRC_RRM_UE_HO_ADM_RESP                (RRC_RRM_API_BASE + 32)
#define RRC_RRM_UE_HO_ADM_CNF                 (RRC_RRM_API_BASE + 33)
#define RRC_RRM_HO_RESTRICTION_LIST_IND       (RRC_RRM_API_BASE + 34)
#define RRC_RRM_UE_HO_CMD_REQ                 (RRC_RRM_API_BASE + 35)
#define RRC_RRM_UE_HO_CMD_RESP                (RRC_RRM_API_BASE + 36)
#define RRC_RRM_MEAS_CONFIG_REQ               (RRC_RRM_API_BASE + 37)
#define RRC_RRM_MEAS_CONFIG_RESP              (RRC_RRM_API_BASE + 38)
#define RRC_RRM_HO_FAILURE                    (RRC_RRM_API_BASE + 39)
#define RRC_RRM_HO_CANCEL_REQ                 (RRC_RRM_API_BASE + 40)
#define RRC_RRM_HO_CANCEL_RESP                (RRC_RRM_API_BASE + 41)
/*Handover End*/

/*PWS start*/
#define RRC_RRM_UPDATED_PWS_SI_LIST_REQ       (RRC_RRM_API_BASE + 42)
#define RRC_RRM_UPDATED_PWS_SI_LIST_RESP      (RRC_RRM_API_BASE + 43)
#define RRC_RRM_UPDATED_PWS_SI_LIST_CNF       (RRC_RRM_API_BASE + 44)
/*PWS stop*/
#define RRC_RRM_INACTIVE_UES_IND                (RRC_RRM_API_BASE + 45)

/* Reconfig Feature Phase-II Start */

#define RRC_RRM_CELL_START_REQ                (RRC_RRM_API_BASE + 46)
#define RRC_RRM_CELL_START_RESP               (RRC_RRM_API_BASE + 47)
#define RRC_RRM_CELL_STOP_REQ                 (RRC_RRM_API_BASE + 48)
#define RRC_RRM_CELL_STOP_RESP                (RRC_RRM_API_BASE + 49)
#define RRC_RRM_PROXIMITY_IND          (RRC_RRM_API_BASE + 50)
#define RRC_RRM_INTRA_ENB_HO_IND       (RRC_RRM_API_BASE + 51)

/* Reconfig Feature Phase -II End */

/* Dynamic UE Reconfiguration Start */
#define RRC_RRM_UE_RECONFIG_REQ                (RRC_RRM_API_BASE + 52)
#define RRC_RRM_UE_RECONFIG_RESP               (RRC_RRM_API_BASE + 53)
/* Dynamic UE Reconfiguration End */

/*CDMA2000_CSFB:HO-start*/
#define RRC_RRM_CSFB_PARAMETERS_REQ_CDMA2000   (RRC_RRM_API_BASE + 54)
#define RRC_RRM_CSFB_PARAMETERS_RESP_CDMA2000  (RRC_RRM_API_BASE + 55)
/*CDMA2000_CSFB:HO-end*/

/* RIM start*/
#define RRC_RRM_RAN_RIM_INFORMATION_REQ        (RRC_RRM_API_BASE + 0x070b)
#define RRC_RRM_RAN_RIM_INFORMATION            (RRC_RRM_API_BASE + 57)
/* RIM end*/

/* temporary RIM PDU's*/
#define RRC_RAN_INFO_REQ_RIM                    (RRC_RRM_API_BASE + 58)
#define RRC_RAN_INFO_RIM                        (RRC_RRM_API_BASE + 59)
#define RRC_RAN_ACK_RIM                         (RRC_RRM_API_BASE + 60)
#define RRC_RAN_INFO_ERROR_RIM                  (RRC_RRM_API_BASE + 61)
#define RRC_RAN_INFO_APP_ERROR_RIM              (RRC_RRM_API_BASE + 62)
/* temporary RIM PDU's end*/

#define RRC_RRM_COUNTER_CHECK_REQ             (RRC_RRM_API_BASE + 63)
#define RRC_RRM_COUNTER_CHECK_RESP            (RRC_RRM_API_BASE + 64)
/*UE Information Feature Start*/
#define RRC_RRM_UE_INFORMATION_REQ            (RRC_RRM_API_BASE + 65)
#define RRC_RRM_UE_INFORMATION_RESP           (RRC_RRM_API_BASE + 66)
/*UE Information Feature Stop*/

/* Downlink NAS Transport Indication */
#define RRC_RRM_DL_NAS_TRANSPORT_IND          (RRC_RRM_API_BASE + 67)
#define RRC_RRM_INTER_FREQ_MEASUREMENT_IND    (RRC_RRM_API_BASE + 68)
/* sync changes start */
#define RRC_RRM_UE_SYNC_STATUS                (RRC_RRM_API_BASE + 69)
/* sync changes stop */
#define RRC_RRM_LOCAL_ERROR_IND               (RRC_RRM_API_BASE + 70)
/* re-estab changes start */
#define RRC_RRM_UE_REESTAB_CONFIG_REQ         (RRC_RRM_API_BASE + 71)
#define RRC_RRM_UE_REESTAB_CONFIG_RESP        (RRC_RRM_API_BASE + 72)
#define RRC_RRM_UE_REESTAB_CONFIG_CNF         (RRC_RRM_API_BASE + 73)
/* re-estab changes stop */
/* SPR 16053 Fix Start */
#define RRC_RRM_UE_REESTAB_COMPLETE_IND       (RRC_RRM_API_BASE + 74)

#define RRC_RRM_WLAN_CONNECTION_STATUS_IND    (RRC_RRM_API_BASE + 75)
#define RRC_RRM_ERB_RECONFIG_REQ              (RRC_RRM_API_BASE + 76)
#define RRC_RRM_ERB_RECONFIG_RESP             (RRC_RRM_API_BASE + 77)
/* SPR 16053 Fix Stop */

/* OPTION3X Changes Start */
//#ifdef ENDC_ENABLED
#define RRC_RRM_DC_BEARER_CHANGE_REQ          (RRC_RRM_API_BASE + 78)
#define RRC_RRM_DC_BEARER_CHANGE_CNF          (RRC_RRM_API_BASE + 79)
#define RRC_RRM_MAX_API                       RRC_RRM_DC_BEARER_CHANGE_CNF
//#else
//#define RRC_RRM_MAX_API                       RRC_RRM_ERB_RECONFIG_RESP 
//#endif
/* OPTION3X Changes End */
/********************************************************************
 * RRC - OAM APIs
 *******************************************************************/
#define RRC_OAM_API_BASE                      0x0000

#define RRC_OAM_INIT_IND                      (RRC_OAM_API_BASE + 1)
#define RRC_OAM_INIT_CNF                      (RRC_OAM_API_BASE + 2)
#define RRC_OAM_COMMUNICATION_INFO_REQ        (RRC_OAM_API_BASE + 3)
#define RRC_OAM_COMMUNICATION_INFO_RESP       (RRC_OAM_API_BASE + 4)
#define RRC_OAM_PROVISION_REQ                 (RRC_OAM_API_BASE + 5)
#define RRC_OAM_PROVISION_RESP                (RRC_OAM_API_BASE + 6)
#define RRC_OAM_S1AP_INFO_REQ                 (RRC_OAM_API_BASE + 7)
#define RRC_OAM_S1AP_INFO_RESP                (RRC_OAM_API_BASE + 8)
#define RRC_OAM_SET_LOG_LEVEL_REQ             (RRC_OAM_API_BASE + 9)
#define RRC_OAM_SET_LOG_LEVEL_RESP            (RRC_OAM_API_BASE + 10)
#define RRC_OAM_LOG_ENABLE_REQ                (RRC_OAM_API_BASE + 11)
#define RRC_OAM_LOG_ENABLE_RESP               (RRC_OAM_API_BASE + 12)
#define RRC_OAM_GET_CELL_STATS_REQ            (RRC_OAM_API_BASE + 13)
#define RRC_OAM_GET_CELL_STATS_RESP           (RRC_OAM_API_BASE + 14)
#define RRC_OAM_GET_CELL_STATUS_REQ           (RRC_OAM_API_BASE + 15)
#define RRC_OAM_GET_CELL_STATUS_RESP          (RRC_OAM_API_BASE + 16)
#define RRC_OAM_GET_UE_STATUS_REQ             (RRC_OAM_API_BASE + 17)
#define RRC_OAM_GET_UE_STATUS_RESP            (RRC_OAM_API_BASE + 18)
#define RRC_OAM_RESET_CELL_STATS_REQ          (RRC_OAM_API_BASE + 19)
#define RRC_OAM_RESET_CELL_STATS_RESP         (RRC_OAM_API_BASE + 20)
#define RRC_OAM_CLEANUP_REQ                   (RRC_OAM_API_BASE + 21)
#define RRC_OAM_CLEANUP_RESP                  (RRC_OAM_API_BASE + 22)
#define RRC_OAM_CELL_TRAFFIC_TRACE_START      (RRC_OAM_API_BASE + 23)
#define RRC_OAM_CELL_TRAFFIC_TRACE_STOP       (RRC_OAM_API_BASE + 24)
#define RRC_OAM_CELL_TRAFFIC_TRACE_START_ERROR_IND  (RRC_OAM_API_BASE + 25)
#define RRC_OAM_TRACE_FILE_TRANSFER_START_IND (RRC_OAM_API_BASE + 26)
#define RRC_OAM_TRACE_FILE_TRANSFER_COMPLETE_IND (RRC_OAM_API_BASE + 27)
#define RRC_OAM_TRACE_START_ERROR_IND            (RRC_OAM_API_BASE + 28)
#define RRC_OAM_CONFIG_STATS_REQ              (RRC_OAM_API_BASE + 29)
#define RRC_OAM_CONFIG_STATS_RESP             (RRC_OAM_API_BASE + 30)
#define RRC_OAM_RRC_CONNECTION_STATS_IND      (RRC_OAM_API_BASE + 31)
#define RRC_OAM_ERAB_STATS_IND                (RRC_OAM_API_BASE + 32)
#define RRC_OAM_HANDOVER_STATS_IND            (RRC_OAM_API_BASE + 33)
#define RRC_OAM_PAGING_STATS_IND              (RRC_OAM_API_BASE + 34)
#define RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND    (RRC_OAM_API_BASE + 35)
#define RRC_OAM_IRAT_MOBILITY_STATS_IND       (RRC_OAM_API_BASE + 36)
#define RRC_OAM_PROC_SUPP_REQ                 (RRC_OAM_API_BASE + 37)   
#define RRC_OAM_PROC_SUPP_RESP                (RRC_OAM_API_BASE + 38)    
#define RRC_OAM_ADD_LGW_REQ                   (RRC_OAM_API_BASE + 39)
#define RRC_OAM_ADD_LGW_RESP                  (RRC_OAM_API_BASE + 40)
#define RRC_OAM_DELETE_LGW_REQ                (RRC_OAM_API_BASE + 41)
#define RRC_OAM_DELETE_LGW_RESP               (RRC_OAM_API_BASE + 42)
/*SPR 13115 Start */
#define RRC_OAM_GET_LOG_LEVEL_REQ             (RRC_OAM_API_BASE + 43)
#define RRC_OAM_GET_LOG_LEVEL_RESP            (RRC_OAM_API_BASE + 44)
#define RRC_OAM_GET_PS_STATS_REQ                  (RRC_OAM_API_BASE + 45)
#define RRC_OAM_GET_PS_STATS_RESP                  (RRC_OAM_API_BASE + 46)
#define RRC_OAM_PS_INIT_IND                  (RRC_OAM_API_BASE + 47)
/*spr 22474 start*/
#define RRC_OAM_GET_DEBUG_INFO_REQ           (RRC_OAM_API_BASE + 48)
#define RRC_OAM_GET_DEBUG_INFO_RESP          (RRC_OAM_API_BASE + 49)
#define RRC_OAM_MAX_API                       RRC_OAM_GET_DEBUG_INFO_RESP
/*spr 22474 end*/
/*SPR 13115 Stop */
#endif /* _RRC_EXT_API_H_ */
