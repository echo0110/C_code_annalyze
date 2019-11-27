/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_fsm_common.h,v 1.34 2010/10/13 07:03:16 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : UECC FSM data types declaration
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_fsm_common.h,v $
 * Revision 1.34.2.1  2010/12/02 06:08:57  gur21231
 * Reestablishment Code Review Req
 *
 * Revision 1.34  2010/10/13 07:03:16  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.9.4.14  2010/10/04 07:04:22  gur18569
 * adde ho cancel ack
 *
 * Revision 1.9.4.13  2010/09/13 13:03:49  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.9.4.12  2010/08/30 11:55:56  gur25381
 * Removed redundant UECC_EV_LLIM_END_MARKER_IND
 *
 * Revision 1.9.4.11  2010/08/10 13:55:43  gur16768
 * code review comments incorporated
 *
 * Revision 1.9.4.10  2010/08/10 10:02:26  gur18569
 * incorported review comments for ho
 *
 * Revision 1.9.4.9  2010/08/05 08:05:54  gur15697
 * merging round 2
 *
 * Revision 1.9.4.8  2010/07/28 06:52:10  gur15697
 * warnings removed
 *
 * Revision 1.9.4.7  2010/07/21 08:32:16  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.9.4.6  2010/07/15 05:49:10  gur16768
 * code review comments incorporate
 *
 * Revision 1.9.4.5  2010/06/29 14:22:21  gur22047
 * RRM interface API from RRM to UECC handling
 *
 * Revision 1.9.4.4  2010/06/29 10:02:05  gur22047
 * RRM interface API from RRM to UECC handling
 *
 * Revision 1.9.4.3  2010/06/28 12:58:35  gur16768
 * added functions to support source side handover related functionality
 *
 * Revision 1.9.4.2  2010/06/24 12:49:12  gur22047
 * internal source and s1ap code
 *
 * Revision 1.9.4.1  2010/06/23 10:37:41  gur18569
 * Added Target Handover related changes
 *
 * Revision 1.9  2010/04/16 08:38:05  gur10248
 * cell deltion changes
 *
 * Revision 1.8  2010/04/13 11:23:57  gur10248
 * SRB2 changes
 *
 * Revision 1.7  2010/04/05 09:48:23  gur10248
 * cell_delete_changes_05apr
 *
 * Revision 1.6  2010/03/24 09:50:42  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.5  2010/02/24 04:25:46  gur10248
 * comments_dpisposed_cell_del_cell_setup_1_1
 *
 * Revision 1.4  2010/02/18 04:19:02  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.3  2010/01/11 11:41:26  gur10248
 * *** empty log message ***
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.6.3  2009/12/26 12:08:53  gur21006
 * Warnings removed
 *
 * Revision 1.1.6.2  2009/12/08 11:22:51  ukr16032
 * UECC - inspection log items LG_20, LG_23, LG_24, LG_26, LG_27, LG_28, LG_29, LG_34, LG_40, LG_41 fixed
 *
 * Revision 1.1.6.1  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.4.9  2009/11/24 12:12:51  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.4.8  2009/11/24 11:32:34  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.4.6.2.3  2009/11/24 08:21:33  ukr18880
 * unused variable removed
 *
 * Revision 1.1.4.6.2.2  2009/11/23 16:43:50  ukr15916
 * beautification
 *
 * Revision 1.1.4.6.2.1  2009/11/20 15:38:46  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.4.6  2009/11/19 09:52:17  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.4.5  2009/11/13 13:14:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.4.4  2009/11/13 12:36:55  ukr18880
 * *** empty log message ***
 *
 * Revision 1.1.4.3  2009/11/13 12:23:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.4.2  2009/11/11 17:47:41  ukr16032
 * UECC
 *  -- S1AP UL NAS message generation updated
 *  -- FSM - review
 *  -- FSM - reset and rrm_release handling added to wait_for_s1_connection and
 * ue_connected states
 *  -- S1AP_RESET_MSG_IND message added
 *
 * Revision 1.1.4.1  2009/10/27 09:50:12  ukr15916
 * UECC added to Rel 1.0
 *
 * Revision 1.1.2.6  2009/10/27 07:59:20  ukr18880
 * UECC FSM failure processing was updated
 *
 * Revision 1.1.2.5  2009/10/26 16:14:46  ukr18880
 * UECC FSM failure processing engine initial creation
 *
 * Revision 1.1.2.4  2009/10/23 12:35:41  ukr18880
 * update of UECC FSM
 *
 * Revision 1.1.2.3  2009/10/22 13:10:04  ukr18880
 * handlers calls logic was changed in UECC FSM engine
 *
 * Revision 1.1.2.2  2009/10/22 09:43:16  ukr18880
 * bug fixing
 *
 * Revision 1.1.2.1  2009/10/22 07:03:19  ukr18880
 * New UECC FSM initial creation
 *19.10.2009    AndriyT -   initial creation
 *
 ****************************************************************************/

#ifndef UECC_FSM_COMMON_H
#define UECC_FSM_COMMON_H

/****************************************************************************
 * Includes
 ****************************************************************************/
#include "rrc_defines.h"
#include "uecc_utils.h"

/****************************************************************************
 * External data types
 ****************************************************************************/
struct _uecc_ue_context_t;
/****************************************************************************
 * Public types definitions
 ****************************************************************************/
#define UE_CONTEXT_RELEASE_REQ 1
#define UE_CONTEXT_RELEASE_COMMAND 2
#define MEASUREMENT_REPORT 3
#define UE_CONTEXT_MOD_FAIL 4
#define HANDOVER_CANCEL     5
#define HANDOVER_REQUEST    6
#define HANDOVER_NOTIFY     7
#define ERAB_SETUP_REQ      8 
#define ERAB_SETUP_RESP     9
#define ERAB_MODIFY_REQ     10
#define ERAB_MODIFY_RESP    11
#define INITIAL_CONTEXT_SETUP_REQ  12
#define INITIAL_CONTEXT_SETUP_RESP  13
#define INITIAL_CONTEXT_SETUP_FAILURE 14
#define NAS_NON_DELIVERY_INDICATION   15
#define UPLINK_NAS_TRANSPORT          16
#define RRC_CONNECTION_REESTABLISHMENT_REQ 17
#define HANDOVER_COMMAND 18
#define HANDOVER_PREPARATION_FAILURE    19
#define HANDOVER_REQUIRED               20
#define INITIAL_UE_MESSAGE              21
#define HANDOVER_FAILURE                22
#define HANDOVER_REQUEST_ACK            23
#define ERAB_RELEASE_COMMAND            24
#define RRC_CONNECTION_SETUP_COMPLETE   25
#define RRC_CONNECTION_RELEASE          26
#define RRC_CONNECTION_REQUEST          27
#define PATH_SWITCH_REQUEST             28
#define DOWNLINK_S1_CDMA2000_TUNNELING  29
#define X2AP_HANDOVER_REQUEST           30
#define X2AP_HANDOVER_CANCEL            31
#define X2AP_HANDOVER_REQUEST_ACK       32
#define X2AP_HANDOVER_PREP_FAILURE      33
#define X2AP_SN_STATUS_TRANSFER         34
#define HO_FROM_EUTRA_PREP_REQ          35
typedef enum
{
    /* Common FSM events */
    UECC_EV_STATE_INIT          = 0,
    UECC_EV_TIMER_EXPIRY,
    UECC_EV_ERB_ONGOING_HO_REQUIRED_INT,
    UECC_EV_MME_GUARD_TIMER_EXPIRY,
    UECC_EV_UE_RELEASE_REQ, /*UECC_EV_S1AP_UE_CTX_RELEASE_CMD_IND*/
    UECC_EV_UE_ERROR_INDICATION, 

    UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT,
    /* UE Connection Setup Control Procedure */
    UECC_EV_UE_CONNECTION_SETUP_FAILURE_INT,
    UECC_EV_UE_CONNECTION_SETUP_SUCCESS_INT,

    /* Initial UE Context Setup control procedure */
    UECC_EV_S1AP_INITIAL_CTX_SETUP_IND,
    UECC_EV_S1AP_INITIAL_CTX_SETUP_SUCCESS_INT,
    UECC_EV_S1AP_INITIAL_CTX_SETUP_FAILURE_INT,
    UECC_EV_S1AP_INITIAL_CTX_SETUP_RELEASE_REQ_INT,
    UECC_EV_S1AP_INITIAL_CTX_SETUP_RESET_REQ_INT,
    UECC_EV_S1AP_INITIAL_CTX_SETUP_RRM_RELEASE_REQ_INT,

    /* UE Connection Release */
    UECC_EV_UE_RELEASED_INT,
    UECC_EV_RRM_UE_CONNECTION_REL_IND_INT,


    /* HO procedure */
    UECC_EV_HO_REQUIRED_INT,
    UECC_EV_SRC_HO_SUCCESS_INT,
    UECC_EV_SRC_HO_FAILURE_INT,
    UECC_EV_SRC_HO_RE_EST_INT,
    UECC_EV_SRC_HO_REL_REQ_INT,
    UECC_EV_SRC_HO_RADIO_LINK_FAILURE_INT,
    UECC_EV_TRG_HO_SUCCESS_INT,
    UECC_EV_TRG_HO_FAILURE_INT,
    UECC_EV_TRG_HO_RELEASE_REQ_INT,
    UECC_EV_TRG_HO_RESET_REQ_INT,
    UECC_EV_TRG_HO_CSC_DELETE_ALL_UE_REQ_INT,
    UECC_EV_TRG_HO_CTX_REL_CMD_INT,
    UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT,

    /* RRC-UE interface related events */
    UECC_EV_RRC_CONNECTION_REQUEST,
    UECC_EV_RRC_CONNECTION_SETUP_COMPLETE,
    UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE,
    UECC_EV_RRC_UE_CAPABILITY_INFORMATION,
    UECC_EV_RRC_MEASUREMENT_REPORT_IND,
    UECC_EV_RRC_SECURITY_MODE_COMPLETE,
    UECC_EV_RRC_SECURITY_MODE_FAILURE,
    UECC_EV_RRC_CONNECTION_RECONFIG_FAIL_INT,
    UECC_EV_RRC_CONNECTION_HANDOVER_FAIL_INT,
    UECC_EV_RRC_CONNECTION_OTHER_FAIL_INT,
    UECC_EV_RRC_RE_ESTABLISH_REQ,
    UECC_EV_RRC_RE_ESTABLISH_REQ_INT,
    UECC_EV_RRC_PROXIMITY_INDICATION_IND,/* for proximity */

    /* S1AP interface related events */
    UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ,
    UECC_EV_S1AP_DL_NAS_TRANSPORT_IND,
    UECC_EV_S1AP_NAS_NON_DELIEVERY_IND,
    UECC_EV_S1AP_RESET_MSG_IND,
    UECC_EV_S1AP_DEL_UE_CONTEXT_IND,
    UECC_EV_S1AP_UE_REL_REQ_INT,
    /* SPR 13560 Fix Start*/
    UECC_EV_SIAP_ERROR_INDICATION,
    /* SPR 13560 Fix Stop*/

    /* Start: E-RAB Modify related events */
    UECC_EV_S1AP_ERAB_RELEASE_COMMAND,
    UECC_EV_S1AP_ERAB_MODIFY_REQ,
    UECC_EV_S1AP_ERAB_MODIFY_RESP_SUCCESS_INT,
    UECC_EV_S1AP_ERAB_MODIFY_RESP_FAILURE_INT,
    /* End: E-RAB Modify*/

    /* ERAB SETUP Start */
    UECC_EV_S1AP_ERAB_SETUP_REQ,
    UECC_EV_S1AP_ERAB_SETUP_SUCCESS_INT,    
    UECC_EV_S1AP_ERAB_SETUP_FAILURE_INT,
    UECC_EV_S1AP_ERAB_SETUP_FAILURE_REL_UE_CTX_INT,
    UECC_EV_S1AP_ERAB_SETUP_PROC_TIMER_EXPIRY_INT,
    UECC_EV_S1AP_ERAB_SETUP_ROLLBACK_FAILURE_REL_UE_CTX_INT,
    UECC_EV_S1AP_ERAB_SETUP_UE_TIMER_EXPIRY_INT,
    ERAB_SETUP_TIMER_EXPIRY,
    UECC_EV_S1AP_ERAB_SETUP_RELEASE_REQ_INT,
    UECC_EV_S1AP_ERAB_SETUP_RESET_REQ_INT,
    UECC_EV_S1AP_ERAB_SETUP_RRM_RELEASE_REQ_INT,
    UECC_EV_S1AP_ERAB_MODIFY_FAILURE_REL_UE_CTX_INT,
    UECC_EV_S1AP_ERAB_RELEASE_COMMAND_SUCCESS_INT,
    UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT,
    UECC_EV_S1AP_RESET_MSG_IND_INT,
    UECC_EV_S1AP_UE_CONTEXT_MOD_REQ,

    UECC_EV_S1AP_HO_REQUEST,
    UECC_EV_SRC_HO_RESET_REQ_INT,
    UECC_EV_SRC_HO_CTX_REL_CMD_INT,
    UECC_EV_S1AP_MME_STATUS_TRANSFER,
    UECC_EV_S1AP_HO_CMD,
    UECC_EV_S1AP_HO_CANCEL_ACK,
    UECC_EV_S1AP_HO_PREPARATION_FAIL,

    /* RRM interface related events */
    UECC_EV_RRM_UE_CONNECTION_RELEASE_IND_INT,
    UECC_EV_RRM_UE_ADMISSION_RESP,
    UECC_EV_RRM_UE_RELEASE_RESP,
    UECC_EV_RRM_ERB_SETUP_RESP,
    UECC_EV_RRM_UE_CONNECTION_RELEASE_IND,
    UECC_EV_RRM_UE_CAPABILITY_ENQUIRY_REQ,
    UECC_EV_RRM_HO_REQUIRED_IND,
    UECC_EV_RRM_ERAB_MODIFY_RESP,
    /* End E-RAB Modify*/
    UECC_EV_RRM_ERAB_RELEASE_IND_SUCCESS_INT,
    UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT,
    UECC_EV_RRM_ERB_RELEASE_RESP,
    UECC_EV_RRM_HO_REQUIRED,
    UECC_EV_RRM_ERAB_RELEASE_IND,
    UECC_EV_RRM_HANDOVER_INIT,

    UECC_EV_RRM_HO_ADM_RESP,      
    UECC_EV_RRM_HO_CANCEL_REQ,
    UECC_EV_RRM_HO_CMD_RESP,
    /* LLIM interface related events */
    UECC_EV_LLIM_CREATE_UE_ENTITY_RESP,
    UECC_EV_LLIM_DELETE_UE_ENTITY_RESP,
    UECC_EV_LLIM_RECONFIGURE_UE_RESP,
    UECC_EV_LLIM_CONFIGURE_SECURITY_RESP,
    UECC_EV_LLIM_SRB_DATA_STATUS_IND,
    UECC_EV_LLIM_MAC_I_RESP, 
    UECC_EV_LLIM_SN_HFN_STATUS_RESP,
    UECC_EV_LLIM_UE_INACTIVE_TIME_RESP,
    UECC_EV_LLIM_S1U_RECONFIG_CNF,
    UECC_EV_LLIM_S1U_END_MARKER_IND,
    UECC_EV_LLIM_RE_EST_REQ, 
    UECC_EV_LLIM_S1U_ERROR_IND,
    UECC_EV_LLIM_RACH_RES_RESP,
    UECC_EV_RRC_CON_RECONFIG_COMPLETE,
    UECC_EV_UE_TRG_HO_S1U_RECONFIG_CNF,
    UECC_EV_LLIM_RADIO_LINK_FAILURE_IND,
    UECC_EV_LLIM_GUARD_TIMER_EXPIRY,
   /*SPR_19066_START*/
    UECC_EV_LLIM_HO_PREP_INFO_RESP,
    /*SPR_19066_END*/
    /* Changes CSC interface related events */
    UECC_EV_CSC_DELETE_ALL_UE_REQ,
    UECC_EV_UE_CONNECTION_SETUP_CCSC_CELL_DELETE,
    UECC_EV_CSC_CELL_DELETE_REQ_INT,

    /* Radio Link Failure Related events */
    UECC_EV_RLF_HANDLING_FAILURE_INT,
    UECC_EV_RLF_HANDLING_SUCCESS_INT,
    UECC_EV_RADIO_LINK_FAILURE_IND,
    UECC_EV_W_FOR_REEST_TIMER_EXPIRY,
    UECC_EV_RADIO_LINK_FAILURE_INIT,
    UECC_EV_LLIM_SUSPEND_UE_ENTITY_CNF,



    /* ADD ROLLBACK RESP*/
    UECC_EV_LLIM_ADD_ROLLBACK_RESP,
    /* ADD ROLLBACK RESP */
    
    UECC_EV_UE_CONNECTION_RELEASE_IND,

    /* End E-RAB Modify*/

    /* Ue context Modification Events */
    UECC_EV_RRM_UE_CONTEXT_MOD_RESP,
    UECC_EV_SRC_HO_CSC_DELETE_ALL_UE_REQ_INT,

    UECC_EV_HO_PREP_TIMER_EXPIRY,
    UECC_EV_HO_OVERALL_TIMER_EXPIRY,
    UECC_EV_RRC_RE_ESTABLISH_INIT,
    UECC_EV_UE_CONTXT_MOD_SUCCESS_INT,
    UECC_EV_UE_CONTXT_MOD_FAIL_INT,
    UECC_EV_UE_CONTXT_MOD_RE_ESTABLISH_INT,
    UECC_EV_UE_CONTXT_MOD_FAIL_REL_UE_CONTXT_INT,
    UECC_EV_UE_CONTXT_MOD_HANDOVER_INT,
    UECC_EV_UE_CONTXT_MOD_RADIO_LINK_FAILURE_INT,
    /* Start E-RAB Modify*/
    UECC_EV_LLIM_MODIFY_ROLLBACK_RESP,
    UECC_EV_LLIM_SUSPEND_UE_ENTITY_RESP,
    UECC_EV_LLIM_RADIO_LINK_FAILURE_INT,
    /* End E-RAB Modify*/
    UECC_EV_ICS_RADIO_LINK_FAILURE_INT,    
    UECC_EV_RADIO_LINK_FAILURE_INT,
    UECC_EV_RRM_UE_CONNECTION_RELEASE_INT,
    UECC_EV_CSC_DELETE_ALL_UE_REQ_INT,
    UECC_EV_RRM_HO_REQUIRED_INT,
    UECC_EV_S1AP_RESET_MSG_INT,
    UECC_EV_RRC_RE_ESTABLISH_INT,
    UECC_EV_UE_RELEASE_REQ_INT,
    UECC_EV_UE_CONNECTION_REESTABLISH_SUCCESS_INT,
    UECC_EV_UE_CONNECTION_REESTABLISH_FAILURE_INT,
    UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP,
    UECC_EV_LLIM_RECONFIGURE_UE_ENTITY_RESP,
    UECC_EV_LLIM_RESUME_UE_ENTITY_RESP,
    UECC_EV_RRC_CONNECTION_REESTABLISHMENT_COMPLETE,
    UECC_EV_ICS_REESTABLISH_REQ_INT,
    UECC_EV_LLIM_CHANGE_CRNTI_RESP,
    /* MME Overload */
    UECC_EV_S1AP_OVERLOAD_MSG_IND,

    UECC_EV_INVALID_TIMER,

    /* X2AP related events */
    UECC_EV_X2AP_HO_REQ_ACK,
    UECC_EV_X2AP_HO_PREPARATION_FAIL,
    UECC_EV_X2AP_UE_CTX_RELEASE,
    UECC_EV_X2AP_HO_REQUEST,
    UECC_EV_X2AP_SN_STATUS_TRANSFER,
    UECC_EV_X2AP_HO_CANCEL,
    UECC_EV_SRC_X2AP_HO_CTX_REL_INT,
    UECC_EV_X2AP_ERROR_INDICATION, 

    UECC_EV_S1AP_PATH_SWITCH_REQ_ACK,
    UECC_EV_S1AP_PATH_SWITCH_REQ_FAIL,
   
    /*X2AP FIX_HO_005 : Start*/ 
    /* SPR 16062 Start */
    UECC_EV_HO_ONGOING_UE_CONNECTION_REESTABLISH_SUCCESS_INT,
    /* SPR 16062 Stop */
    /*SPR_17348_START*/
    /*Code Removed*/
    /*SPR_17348_END*/
    UECC_EV_X2AP_RESET_MSG_IND,
    
    /* Intra_cell related events */
    UECC_EV_INTRA_CELL_HO_TIMER_EXPIRY,
    UECC_EV_LLIM_RESET_UE_ENTITY_RESP,

    /*Dynamic UE Reconfiguration : Start*/
    UECC_EV_RRM_UE_RECONFIG_REQ,
    UECC_EV_RRM_UE_RECONFIG_SUCCESS_INT,
    UECC_EV_RRM_UE_RECONFIG_FAILURE_INT,
    UECC_EV_RRM_UE_RECONFIG_FAILURE_REL_UE_CTX_INT,
    UECC_EV_COUNTER_CHECK_TIMER_EXPIRY,
    /* SPR 4118 Start */
    UECC_EV_NOTIFY_INTEGRITY_FAILURE,
    /*SPR_17143_START*/
    /* UECC_UE_INTEGRITY_FAILURE_TIMER_EXPIRY event has been deprecated */
    UECC_UE_INTEGRITY_FAILURE_TIMER_EXPIRY,
    /*SPR_17143_END*/
    /* SPR 4118 Stop */
    UECC_EV_S1AP_ERAB_RELEASE_COMMAND_VALIDATION_FAILURE,
    UECC_EV_S1AP_ALLOCATE_MME_RESP,    
    UECC_EV_UE_INFO_TIMER_EXPIRY,
    
    /*CDMA2000_CSFB_HO start*/
    UECC_EV_RRM_CSFB_PARAMETERS_RESPONSE_CDMA2000,
    UECC_EV_CSFB_PARAMETERS_REQUEST_CDMA2000,
    UECC_EV_RRC_UL_HANDOVER_PREPARATION_TRANSFER,
    UECC_EV_S1AP_DL_CDMA2000_TUNNELING_HO_ONGOING,
    UECC_EV_S1AP_DL_CDMA2000_TUNNELING,
   /* CDMA2000_CSFB_HO stop */
    UECC_EV_CCO_TIMER_EXPIRY,
    /*SPR:6462 start */
    UECC_EV_LLIM_REESTAB_UE_ENTITY_COMP_CNF,
    /*SPR:6462 stop */
    /*securitykey*/
    UECC_EV_UE_CTX_MOD_ONGOING_HO_INT,
    /*securitykey*/
    /*BUG 604 changes start*/
    UECC_EV_LLIM_S1U_INTRA_ENB_DATA_FWD_CNF,
    UECC_EV_LLIM_S1U_LOCAL_PATH_SWITCH_CNF,
    UECC_EV_S1AP_ALLOCATE_S1AP_UEID_RESP,    
    /*BUG 604 changes stop*/
    /*Bug 357 Fix Start*/
    UECC_EV_LLIM_PDCP_REESTABLISH_UE_ENTITY_RESP,
    /*Bug 357 Fix Stop*/
    /* Bug 817 Fix Start */
    UECC_EV_S1U_END_MARKER_IND_TIMER_EXPIRY,
    /* Bug 817 Fix Stop */
    /* XWAP related events */
#ifdef ENDC_ENABLED
/*NR_DC Code Change START*/
    UECC_EV_X2AP_SGNB_ADDITION_REQ_ACK,
    UECC_EV_X2AP_SGNB_ADDITION_REQ_REJECT,
/*NR_DC Code Change Stop*/
    /*EN-DC_changes_start*/
    UECC_EV_X2AP_SGNB_RELEASE_REL_ACK,
    /*EN-DC_changes_end*/
/*NR_DC Code Change START*/
    UECC_EV_SCG_FAILURE_INFORMATION_NR,
/*NR_DC Code Change Stop*/
    UECC_EV_X2AP_SGNB_RELEASE_REL_REQ,
/* OPTION3X Changes Start */
    UECC_EV_RRM_DC_BEARER_CHANGE_REQ,
    UECC_EV_RRM_DC_BEARER_CHANGE_CNF,
    UECC_EV_RRM_DC_BEARER_CHANGE_CNF_FAILURE_UE_REL,
    UECC_EV_S1AP_ERAB_MODIFICATION_CNF,
/* OPTION3X Changes End */
#endif
    UECC_EV_LAST

} uecc_fsm_event_et;

 typedef enum 
{
    EVENT_VALUE_MEASUREMENT_REPORT,             /* 0 */ 
    EVENT_VALUE_RRC_CONN_RECONFIG_COMPL,        /* 1 */     
    EVENT_VALUE_RRC_CONN_RECONFIG_FAIL,         /* 2 */
    EVENT_VALUE_RRC_CONN_REESTABL_COMPL,        /* 3 */
    EVENT_VALUE_RRC_CONN_SETUP_COMPL,           /* 4 */
    EVENT_VALUE_RRC_STATUS,                     /* 5 */
    EVENT_VALUE_SEC_MODE_COMPL,                 /* 6 */
    EVENT_VALUE_SEC_MODE_FAIL,                  /* 7 */
    EVENT_VALUE_UE_CAP_INFO,                    /* 8 */
    EVENT_VALUE_UL_INFO_TRANSFER,               /* 9 */
    EVENT_VALUE_RRC_CONN_REESTABL_REQ,          /* 10 */
    EVENT_VALUE_RRC_CONN_REQ,                   /* 11 */
    EVENT_VALUE_INVALID                         /* 12 */
}uecc_event_rrc_msg_id_et;

/* for procedure's constructor and message handling */
typedef void (*uecc_fsm_handler_f)(
    struct _uecc_ue_context_t*   p_ue_context,  /* UECC UE context data */
    void*                        p_api_data);   /* Input API buffer */

/* Record: Event -> Handler */
typedef const struct
{
    uecc_id_t                   id;
    const uecc_fsm_handler_f    handler;
} uecc_fsm_handler_record_t;

/* Descriptor of Activity state */
typedef const struct
{
    S8*                         s_name;
    S8*                         s_id;
    rrc_bool_et                   is_need_init;
    uecc_fsm_handler_record_t *   p_mht;
} uecc_fsm_activity_state_t;

/* Descriptor of Activity */
typedef const struct
{
    S8*                         s_name;
    S8*                         s_id;
    uecc_fsm_activity_state_t *   p_first_state;
} uecc_fsm_activity_t;

typedef uecc_fsm_activity_t**           uecc_fsm_activity_list_t;

/* Descriptor of Control Procedure */
typedef const   struct
{
    S8*                         s_name;
    S8*                         s_id;
    uecc_fsm_handler_f            constructor_f;
    uecc_fsm_handler_f            finish_success_f;
    uecc_fsm_handler_f            finish_fail_f;
    uecc_fsm_activity_list_t      activity_list;
    uecc_fsm_handler_record_t*    p_mht;
} uecc_fsm_procedure_t;


typedef struct
{
    /* points to current active procedure */
    uecc_fsm_procedure_t*                       p_active_proc;

    /* points to current activity */
    /* inits in uecc_fsm_start_next_activity() function */
    uecc_fsm_activity_t*                        p_activity;

    /* points to current activity state */
    /* inits in uecc_fsm_start_next_activity() function */
    uecc_fsm_activity_state_t*                  p_current_state;

    /* number of current activity in list of activities for procedure */
    /* inits in uecc_fsm_start_next_activity() function */
    U8                                          num_activity;
} uecc_fsm_procedure_descriptor_t;

typedef struct _uecc_fsm_main_t
{
    uecc_fsm_procedure_descriptor_t             proc_descriptor;
    uecc_fsm_activity_state_t*                  p_current_state;
} uecc_fsm_main_t;

typedef void (*uecc_fsm_fail_handler_f)(
    struct _uecc_ue_context_t*  p_ue_context,  /* UECC UE context data */
    rrc_bool_et*                p_break);

typedef enum
{
    ECM_IDLE =0,
    ECM_CONNECTED
}ue_state_et;

#endif /* UECC_FSM_COMMON_H */
