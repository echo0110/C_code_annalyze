/********************************************************************
 *
 *  FILE NAME   : rrc_intrl_api.h
 *
 *  DESCRIPTION : This file contains the interface message API
 *                IDs for all internal modules of RRC.
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

#ifndef _RRC_INTRL_API_H_
#define _RRC_INTRL_API_H_

/********************************************************************
 * CSC - LLIM APIs
 *******************************************************************/
#define CSC_LLIM_API_BASE               0x0000

#define CSC_LLIM_CONFIG_CELL_REQ            (CSC_LLIM_API_BASE + 1)
#define CSC_LLIM_CONFIG_CELL_RESP           (CSC_LLIM_API_BASE + 2)
#define CSC_LLIM_DELETE_CELL_REQ            (CSC_LLIM_API_BASE + 3)
#define CSC_LLIM_DELETE_CELL_RESP           (CSC_LLIM_API_BASE + 4)
#define CSC_LLIM_SFN_IND                    (CSC_LLIM_API_BASE + 5)
#define CSC_LLIM_BCCH_CONFIG_REQ            (CSC_LLIM_API_BASE + 6)
#define CSC_LLIM_PCCH_DATA_REQ              (CSC_LLIM_API_BASE + 7)
#define CSC_LLIM_SFN_REQ                    (CSC_LLIM_API_BASE + 8)
#define CSC_LLIM_SFN_RESP                   (CSC_LLIM_API_BASE + 9)
#define CSC_LLIM_RECONFIG_CELL_REQ          (CSC_LLIM_API_BASE + 10)
#define CSC_LLIM_RECONFIG_CELL_RESP         (CSC_LLIM_API_BASE + 11)
#define CSC_LLIM_PWS_REQ                    (CSC_LLIM_API_BASE + 12)
#define CSC_LLIM_PWS_RESP                   (CSC_LLIM_API_BASE + 13)

#define CSC_LLIM_UPDATED_PWS_SI_REQ         (CSC_LLIM_API_BASE + 14)
#define CSC_LLIM_UPDATED_PWS_SI_RESP        (CSC_LLIM_API_BASE + 15)
#define CSC_LLIM_CELL_START_REQ             (CSC_LLIM_API_BASE + 16)
#define CSC_LLIM_CELL_START_RESP            (CSC_LLIM_API_BASE + 17)
#define CSC_LLIM_CELL_STOP_REQ              (CSC_LLIM_API_BASE + 18)
#define CSC_LLIM_CELL_STOP_RESP             (CSC_LLIM_API_BASE + 19)

#define CSC_LLIM_MAX_API                    CSC_LLIM_CELL_STOP_RESP

/********************************************************************
 * UECC - LLIM APIs
 *******************************************************************/
#define UECC_LLIM_API_BASE                  0x0100

#define UECC_LLIM_CREATE_UE_ENTITY_REQ      (UECC_LLIM_API_BASE + 0)
#define UECC_LLIM_CREATE_UE_ENTITY_RESP     (UECC_LLIM_API_BASE + 1)
#define UECC_LLIM_DELETE_UE_ENTITY_REQ      (UECC_LLIM_API_BASE + 2)
#define UECC_LLIM_DELETE_UE_ENTITY_RESP     (UECC_LLIM_API_BASE + 3)
#define UECC_LLIM_ADD_LC_REQ                (UECC_LLIM_API_BASE + 4)
#define UECC_LLIM_ADD_LC_RESP               (UECC_LLIM_API_BASE + 5)
#define UECC_LLIM_SRB_DATA_REQ              (UECC_LLIM_API_BASE + 6)
#define UECC_LLIM_SRB_DATA_IND              (UECC_LLIM_API_BASE + 7)
#define UECC_LLIM_SRB_DATA_STATUS_IND       (UECC_LLIM_API_BASE + 8)
#define UECC_LLIM_CCCH_DATA_REQ             (UECC_LLIM_API_BASE + 9)
#define UECC_LLIM_CCCH_DATA_IND             (UECC_LLIM_API_BASE + 0xA)
#define UECC_LLIM_UE_CON_REJ_REQ            (UECC_LLIM_API_BASE + 0xB)
#define UECC_LLIM_CONFIGURE_SECURITY_REQ    (UECC_LLIM_API_BASE + 0xC)
#define UECC_LLIM_CONFIGURE_SECURITY_RESP   (UECC_LLIM_API_BASE + 0xD)
#define UECC_LLIM_S1U_ERROR_IND             (UECC_LLIM_API_BASE + 0xE)
#define UECC_LLIM_S1U_PATH_FAILURE_IND      (UECC_LLIM_API_BASE + 0xF)

#define UECC_LLIM_MODIFY_LC_REQ             (UECC_LLIM_API_BASE + 0x10)
#define UECC_LLIM_MODIFY_LC_RESP            (UECC_LLIM_API_BASE + 0x11)
#define UECC_LLIM_ADD_ROLLBACK_REQ          (UECC_LLIM_API_BASE + 0x12)
#define UECC_LLIM_ADD_ROLLBACK_RESP         (UECC_LLIM_API_BASE + 0x13)
#define UECC_LLIM_MODIFY_ROLLBACK_REQ       (UECC_LLIM_API_BASE + 0x14)
#define UECC_LLIM_MODIFY_ROLLBACK_RESP      (UECC_LLIM_API_BASE + 0x15)
#define UECC_LLIM_RADIO_LINK_FAILURE_IND    (UECC_LLIM_API_BASE + 0x16)
/* End: E-RAB Modify API */
/* ERB RELEASE COMMAND START */ 
#define UECC_LLIM_DELETE_LC_REQ             (UECC_LLIM_API_BASE + 0x17)
#define UECC_LLIM_DELETE_LC_RESP            (UECC_LLIM_API_BASE + 0x18)
/* ERB RELEASE COMMAND END */

#define UECC_LLIM_SUSPEND_UE_ENTITY_REQ     (UECC_LLIM_API_BASE + 0x19)
#define UECC_LLIM_SUSPEND_UE_ENTITY_RESP     (UECC_LLIM_API_BASE + 0x1A)


/*REL 1.2 : HANDOVER PROCEDURE START*/
#define UECC_LLIM_SN_HFN_STATUS_REQ         (UECC_LLIM_API_BASE + 0x1B)
#define UECC_LLIM_SN_HFN_STATUS_RESP        (UECC_LLIM_API_BASE + 0x1C)
#define UECC_LLIM_DATA_BUFFER_STOP_IND      (UECC_LLIM_API_BASE + 0x1D)
#define UECC_LLIM_SN_HFN_STATUS_IND         (UECC_LLIM_API_BASE + 0x1E)
#define UECC_LLIM_MAC_I_REQ                 (UECC_LLIM_API_BASE + 0x1F)
#define UECC_LLIM_MAC_I_RESP                (UECC_LLIM_API_BASE + 0x20)
#define UECC_LLIM_S1U_RECONFIGURE_REQ       (UECC_LLIM_API_BASE + 0x21)
#define UECC_LLIM_S1U_RECONFIGURE_CNF       (UECC_LLIM_API_BASE + 0x22)
#define UECC_LLIM_HO_RACH_RESOURCE_REQ      (UECC_LLIM_API_BASE + 0x23)
#define UECC_LLIM_HO_RACH_RESOURCE_RESP     (UECC_LLIM_API_BASE + 0x24)
#define UECC_LLIM_HO_REL_RACH_RESOURCE_IND  (UECC_LLIM_API_BASE + 0x25)
#define UECC_LLIM_UE_INACTIVE_TIME_REQ      (UECC_LLIM_API_BASE + 0x26)
#define UECC_LLIM_UE_INACTIVE_TIME_RESP     (UECC_LLIM_API_BASE + 0x27)
#define UECC_LLIM_RE_ESTABLISH_UE_ENTITY_REQ (UECC_LLIM_API_BASE + 0x28)
#define UECC_LLIM_S1U_END_MARKER_IND        (UECC_LLIM_API_BASE + 0x29)
#define UECC_LLIM_RE_ESTABLISH_UE_ENTITY_RESP (UECC_LLIM_API_BASE + 0x2A)
#define UECC_LLIM_RESUME_UE_ENTITY_REQ        (UECC_LLIM_API_BASE + 0x2B)
#define UECC_LLIM_RESUME_UE_ENTITY_RESP       (UECC_LLIM_API_BASE + 0x2C)
#define UECC_LLIM_RECONFIG_SECURITY_REQ       (UECC_LLIM_API_BASE + 0x2D)
#define UECC_LLIM_RECONFIG_SECURITY_RESP      (UECC_LLIM_API_BASE + 0x2E)
#define UECC_LLIM_UE_CON_RE_ESTABLISH_REJ_REQ (UECC_LLIM_API_BASE + 0x2F)
#define UECC_LLIM_REESTABLISH_UE_ENTITY_REQ   (UECC_LLIM_API_BASE + 0x30)
#define UECC_LLIM_CHANGE_CRNTI_REQ            (UECC_LLIM_API_BASE + 0x31)
#define UECC_LLIM_CHANGE_CRNTI_RESP           (UECC_LLIM_API_BASE + 0x32)
#define UECC_LLIM_MEAS_CONFIG_REQ             (UECC_LLIM_API_BASE + 0x33)
#define UECC_LLIM_MEAS_CONFIG_RESP            (UECC_LLIM_API_BASE + 0x34)
#define UECC_LLIM_INACTIVE_UES_IND            (UECC_LLIM_API_BASE + 0x35)
#define UECC_LLIM_S1U_PATH_SUCCESS_IND        (UECC_LLIM_API_BASE + 0x36)
#define UECC_LLIM_COUNT_WRAPAROUND_IND        (UECC_LLIM_API_BASE + 0x37)
#define UECC_LLIM_RESET_UE_ENTITY_REQ         (UECC_LLIM_API_BASE + 0x38)
#define UECC_LLIM_RESET_UE_ENTITY_CNF         (UECC_LLIM_API_BASE + 0x39)
 
/* SPR 4118 Start */
#define UECC_LLIM_NOTIFY_INTEGRITY_FAILURE    (UECC_LLIM_API_BASE + 0x3A)

/* SPR 4118 Stop */
#define UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND   (UECC_LLIM_API_BASE + 0x3B)
 
#define UECC_LLIM_DRB_COUNT_MSB_REQ           (UECC_LLIM_API_BASE + 0x3C)
#define UECC_LLIM_DRB_COUNT_MSB_RESP          (UECC_LLIM_API_BASE + 0x3D)
/* SPS Start */
#define UECC_LLIM_MAC_RECONFIG_COMPLETE_IND   (UECC_LLIM_API_BASE + 0x3E)
/* SPS Stop */

/*SPR:6462 start*/
#define UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF   (UECC_LLIM_API_BASE + 0x3F)
/*BUG 604 Fixes start*/
#define UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_REQ            (UECC_LLIM_API_BASE + 0x40)
#define UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_CNF            (UECC_LLIM_API_BASE + 0x41)
#define UECC_LLIM_S1U_LOCAL_PATH_SWITCH_REQ             (UECC_LLIM_API_BASE + 0x42)
#define UECC_LLIM_S1U_LOCAL_PATH_SWITCH_CNF             (UECC_LLIM_API_BASE + 0x43)
/*BUG 604 Fixes stop*/
/*SPR 6462 stop*/
 /*Bug 357 Fix Start*/
#define UECC_LLIM_PDCP_RE_ESTABLISH_UE_ENTITY_REQ       (UECC_LLIM_API_BASE + 0x44)
#define UECC_LLIM_PDCP_RE_ESTABLISH_UE_ENTITY_RESP      (UECC_LLIM_API_BASE + 0x45)
/*SPR_19066_START*/
#define UECC_LLIM_HO_PREP_INFO_REQ                      (UECC_LLIM_API_BASE + 0x46)
#define UECC_LLIM_HO_PREP_INFO_RESP                     (UECC_LLIM_API_BASE + 0x47)
/*SPR_19066_END*/
 /*Bug 357 Fix Stop*/

#define UECC_LLIM_MAX_API                               UECC_LLIM_HO_PREP_INFO_RESP 

/********************************************************************
 * OAMH APIs
 *******************************************************************/
#define OAMH_API_BASE                   0x0200

#define UECC_OAMH_INIT_IND              (OAMH_API_BASE + 0)
#define UECC_OAMH_INIT_CNF              (OAMH_API_BASE + 1)
#define UECC_OAMH_PROV_REQ              (OAMH_API_BASE + 2)
#define UECC_OAMH_PROV_RESP             (OAMH_API_BASE + 3)
#define CSC_OAMH_INIT_IND               (OAMH_API_BASE + 4)
#define CSC_OAMH_INIT_CNF               (OAMH_API_BASE + 5)
#define CSC_OAMH_PROV_REQ               (OAMH_API_BASE + 6)
#define CSC_OAMH_PROV_RESP              (OAMH_API_BASE + 7)
#define LLIM_OAMH_INIT_IND              (OAMH_API_BASE + 8)
#define LLIM_OAMH_INIT_CNF              (OAMH_API_BASE + 9)
#define LLIM_OAMH_PROV_REQ              (OAMH_API_BASE + 0xA)
#define LLIM_OAMH_PROV_RESP             (OAMH_API_BASE + 0xB)

#define UECC_OAMH_UNUSED_1              (OAMH_API_BASE + 0xC)
#define UECC_OAMH_UNUSED_2              (OAMH_API_BASE + 0xD)

/* Depricated APIs due to PC implementation 
#define UECC_OAMH_GET_CELL_STATS_REQ    (OAMH_API_BASE + 0xE)
#define UECC_OAMH_GET_CELL_STATS_RESP   (OAMH_API_BASE + 0xF)
*/
#define UECC_OAMH_GET_CELL_STATUS_REQ   (OAMH_API_BASE + 0x10)
#define UECC_OAMH_GET_CELL_STATUS_RESP  (OAMH_API_BASE + 0x11)
#define UECC_OAMH_GET_UE_STATUS_REQ     (OAMH_API_BASE + 0x12)
#define UECC_OAMH_GET_UE_STATUS_RESP    (OAMH_API_BASE + 0x13)
/* Depricated APIs due to PC implementation 
#define UECC_OAMH_RESET_CELL_STATS_REQ  (OAMH_API_BASE + 0x14)
#define UECC_OAMH_RESET_CELL_STATS_RESP (OAMH_API_BASE + 0x15)
*/

#define UECC_OAMH_CLEANUP_REQ           (OAMH_API_BASE + 0x16)
#define UECC_OAMH_CLEANUP_RESP          (OAMH_API_BASE + 0x17)
#define CSC_OAMH_CLEANUP_REQ            (OAMH_API_BASE + 0x18)
#define CSC_OAMH_CLEANUP_RESP           (OAMH_API_BASE + 0x19)
#define LLIM_OAMH_CLEANUP_REQ           (OAMH_API_BASE + 0x1A)
#define LLIM_OAMH_CLEANUP_RESP          (OAMH_API_BASE + 0x1B)
#define UECC_OAMH_CELL_TRAFFIC_TRACE_START (OAMH_API_BASE + 0x1C)
#define UECC_OAMH_CELL_TRAFFIC_TRACE_STOP (OAMH_API_BASE + 0x1D)
#define UECC_OAMH_CELL_TRAFFIC_TRACE_START_ERROR_IND (OAMH_API_BASE + 0x1E)
#define UECC_OAMH_TRACE_FILE_TRANSFER_START_IND (OAMH_API_BASE + 0x1F)
#define UECC_OAMH_TRACE_FILE_TRANSFER_COMPLETE_IND (OAMH_API_BASE + 0x20)
#define UECC_OAMH_TRACE_START_ERROR_IND   (OAMH_API_BASE + 0x21)
/* Depricated APIs due to PC implementation 
#define CSC_OAMH_GET_CELL_STATS_REQ     (OAMH_API_BASE + 0x22)
#define CSC_OAMH_GET_CELL_STATS_RESP    (OAMH_API_BASE + 0x23)
#define CSC_OAMH_RESET_CELL_STATS_REQ   (OAMH_API_BASE + 0x24)
#define CSC_OAMH_RESET_CELL_STATS_RESP  (OAMH_API_BASE + 0x25)
*/
#define CSC_OAMH_CELL_START_IND         (OAMH_API_BASE + 0x26)
#define CSC_OAMH_CELL_STOP_IND          (OAMH_API_BASE + 0x27)
#define UECCMD_OAMH_INIT_IND            (OAMH_API_BASE + 0x28)
#define UECCMD_OAMH_INIT_CNF            (OAMH_API_BASE + 0x29)
#define UECCMD_OAMH_PROV_REQ            (OAMH_API_BASE + 0x2A)
#define UECCMD_OAMH_PROV_RESP           (OAMH_API_BASE + 0x2B)
#define UECCMD_OAMH_CLEANUP_REQ         (OAMH_API_BASE + 0x2C)
#define UECCMD_OAMH_CLEANUP_RESP        (OAMH_API_BASE + 0x2D)
/* lipa start */
#define UECCMD_OAMH_ADD_DEL_LGW_IND     	(OAMH_API_BASE + 0x2E)
/* SPR 20908 Start */
#define CSC_OAMH_OAM_COMMUNICATION_INFO_IND     (OAMH_API_BASE + 0x2F)
#define CSC_OAMH_OAM_COMMUNICATION_INFO_RESP    (OAMH_API_BASE + 0x30)
/* SPR 20908 End */
#define OAMH_MAX_API                    CSC_OAMH_OAM_COMMUNICATION_INFO_RESP
/* lipa end*/ 
/********************************************************************
 * CSC - UECC APIs
 *******************************************************************/
#define CSC_UECC_API_BASE               0x0300

#define CSC_UECC_INIT_SETUP_IND         (CSC_UECC_API_BASE + 0)
#define CSC_UECC_DELETE_ALL_UE_REQ      (CSC_UECC_API_BASE + 1)
#define UECC_CSC_DELETE_ALL_UE_RESP     (CSC_UECC_API_BASE + 2)
#define CSC_UECC_CELL_RECONFIG_IND      (CSC_UECC_API_BASE + 3)
#define CSC_UECC_GET_CELL_STATS_REQ     (CSC_UECC_API_BASE + 4)
#define CSC_UECC_GET_CELL_STATS_RESP    (CSC_UECC_API_BASE + 5)
#define CSC_UECC_RESET_CELL_STATS_REQ   (CSC_UECC_API_BASE + 6)
#define CSC_UECC_RESET_CELL_STATS_RESP  (CSC_UECC_API_BASE + 7)
#define CSC_UECC_CELL_START_IND         (CSC_UECC_API_BASE + 8)
#define CSC_UECC_CELL_STOP_IND          (CSC_UECC_API_BASE + 9)

#define CSC_UECC_MAX_API                 CSC_UECC_CELL_STOP_IND
/********************************************************************
 * S1AP - UECC APIs
 *******************************************************************/
#define S1AP_UECC_API_BASE               0x0400

#define S1AP_STATUS_MSG_IND             (S1AP_UECC_API_BASE + 0)
#define S1AP_UE_ASSOCIATED_SIG_MSG_REQ  (S1AP_UECC_API_BASE + 1)
#define S1AP_UE_ASSOCIATED_SIG_MSG_IND  (S1AP_UECC_API_BASE + 2)
#define S1AP_RESET_MSG_IND              (S1AP_UECC_API_BASE + 3)
#define S1AP_UE_RELEASE_MSG_REQ         (S1AP_UECC_API_BASE + 4)
#define S1AP_FAILURE_MSG_REQ            (S1AP_UECC_API_BASE + 5)
#define S1AP_DEL_CONTEXT_MSG_IND        (S1AP_UECC_API_BASE + 6)
#define S1AP_OVERLOAD_MSG_IND           (S1AP_UECC_API_BASE + 7)
#define S1AP_UE_ASSOCIATED_MME_UPDATE_STATUS_IND (S1AP_UECC_API_BASE + 8) 
#define S1AP_UE_RELEASE_IND             (S1AP_UECC_API_BASE + 9) 
#define S1AP_UE_REL_REQ_INT             (S1AP_UECC_API_BASE + 10)
#define S1AP_HO_ONGOING_UE_RELEASE_IND  (S1AP_UECC_API_BASE + 11) 
#define S1AP_UECCMD_UECC_INFO           (S1AP_UECC_API_BASE + 12) 
#define S1AP_ALLOCATE_MME_REQ           (S1AP_UECC_API_BASE + 13) 
#define S1AP_ALLOCATE_MME_RESP          (S1AP_UECC_API_BASE + 14) 
/*BUG 604 Fixes start*/
#define S1AP_INTRA_CELL_HO_SUCCESS_IND  (S1AP_UECC_API_BASE + 15)
#define S1AP_ALLOCATE_S1AP_UEID_REQ     (S1AP_UECC_API_BASE + 16)
#define S1AP_ALLOCATE_S1AP_UEID_RESP    (S1AP_UECC_API_BASE + 17)
#define S1AP_UECC_MAX_API               S1AP_ALLOCATE_S1AP_UEID_RESP 
/*BUG 604 Fixes stop*/


/*******************************************************************************
 * S1AP - CSC APIs
 *******************************************************************/
#define S1AP_CSC_API_BASE               0x0500

#define S1AP_PAGING_IND                 (S1AP_CSC_API_BASE + 0x00)
#define S1AP_CELL_REG_REQ               (S1AP_CSC_API_BASE + 0x01)
#define S1AP_CELL_DEREG_REQ             (S1AP_CSC_API_BASE + 0x02)
#define S1AP_PWS_REQ                    (S1AP_CSC_API_BASE + 0x03)
#define S1AP_PWS_RESP                   (S1AP_CSC_API_BASE + 0x04)
#define S1AP_CELL_AREA_INFO_UPDATE_IND  (S1AP_CSC_API_BASE + 0x05)
#define S1AP_PWS_FAILURE_REQ            (S1AP_CSC_API_BASE + 0x06)
#define S1AP_PWS_FAILURE_RESP           (S1AP_CSC_API_BASE + 0x07)
#define S1AP_KILL_REQ                   (S1AP_CSC_API_BASE + 0x08)
#define S1AP_KILL_RESP                  (S1AP_CSC_API_BASE + 0x09)
#define S1AP_KILL_FAILURE_IND           (S1AP_CSC_API_BASE + 0x0A)

#define S1AP_CSC_MAX_API                 S1AP_KILL_FAILURE_IND 

/*x2ap-start*/

/********************************************************************
 * X2AP - CSC APIs
 *******************************************************************/
#define X2AP_CSC_IF_API_BASE            0x0600

#define X2AP_CSC_CELL_ADD_IND          (X2AP_CSC_IF_API_BASE + 0)
#define X2AP_CSC_CELL_DEL_IND          (X2AP_CSC_IF_API_BASE + 1)
#define X2AP_CSC_CELL_UPD_IND          (X2AP_CSC_IF_API_BASE + 2)
#define X2AP_CSC_CELL_STATUS_REQ       (X2AP_CSC_IF_API_BASE + 3)
#define X2AP_CSC_CELL_STATUS_RESP      (X2AP_CSC_IF_API_BASE + 4)
#define X2AP_CSC_MAX_API               X2AP_CSC_CELL_STATUS_RESP

/********************************************************************
 * X2AP - UECC APIs
 *******************************************************************/
#define X2AP_UECC_API_BASE              0x0700

#define X2AP_STATUS_MSG_IND             (X2AP_UECC_API_BASE + 0)
#define X2AP_UE_ASSOCIATED_SIG_MSG_REQ  (X2AP_UECC_API_BASE + 1)
#define X2AP_UE_ASSOCIATED_SIG_MSG_IND  (X2AP_UECC_API_BASE + 2)
#define X2AP_UE_RELEASE_IND             (X2AP_UECC_API_BASE + 3)
#define X2AP_RESET_MSG_IND              (X2AP_UECC_API_BASE + 4)
#define X2AP_UE_REESTABLISHMENT_CONN_SETUP_RECONFIGURE     (X2AP_UECC_API_BASE + 5)
#define X2AP_UE_RLF_MSG_IND             (X2AP_UECC_API_BASE + 6)
#define X2AP_UE_RLF_RESP                (X2AP_UECC_API_BASE + 7)
/* CSR_00040799 Fix Start */
#define X2AP_UECCMD_UE_INFO             (X2AP_UECC_API_BASE + 8) 
/* Bug 4691 Changes Start */
#define X2AP_UPDATE_NBOUR_INFO          (X2AP_UECC_API_BASE + 9)
#define X2AP_UPDATE_NR_NBOUR_INFO   (X2AP_UECC_API_BASE + 10)

#define X2AP_UECC_MAX_API               X2AP_UPDATE_NR_NBOUR_INFO
/* Bug 4691 Changes End */
/* CSR_00040799 Fix End */

/*x2ap-end*/


/********************************************************************
 * UECCMD - UECC APIs
 *******************************************************************/
#define UECCMD_UECC_API_BASE                      0x0800

#define UECCMD_UE_CONTEXT_RELEASE_ONGOING_IND     (UECCMD_UECC_API_BASE + 0)
#define UECCMD_UECC_PROC_STATUS_IND               (UECCMD_UECC_API_BASE + 1)
#define UECC_OAMH_ADD_DEL_LGW_IND                 (UECCMD_UECC_API_BASE + 2)
/*SPR_17727_START*/
#define UECCMD_UE_CON_RE_ESTABLISH_ONGOING        (UECCMD_UECC_API_BASE + 3)
#define UECCMD_UECC_MAX_API                 UECCMD_UE_CON_RE_ESTABLISH_ONGOING
/*SPR_17727_END*/


/********************************************************************
 * X2AP-S1AP APIs
 *******************************************************************/
#define X2AP_S1AP_API_BASE                      0x0900

#define X2AP_S1AP_COMM_INFO         (X2AP_S1AP_API_BASE + 0)
#define X2AP_S1AP_MAX_API           X2AP_S1AP_COMM_INFO


#endif /* _RRC_INTRL_API_H_ */

