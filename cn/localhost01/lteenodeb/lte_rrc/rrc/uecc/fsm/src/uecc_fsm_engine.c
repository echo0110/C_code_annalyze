/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_fsm_engine.c,v 1.1.4.11.2.4 2009/11/24
 *   ukr18880 Exp $uecc_fsm_engine.c
 ****************************************************************************
 *
 *  File Description : UECC FSM engine functions implementation
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_fsm_engine.c,v $
 * Revision 1.5  2010/10/13 07:03:30  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.3.4.2  2010/09/22 14:48:08  gur25381
 * klocwork warning solved after merging
 *
 * Revision 1.3.4.1  2010/08/05 08:07:13  gur15697
 * merging round 2
 *
 * Revision 1.3  2010/05/14 09:36:58  gur19827
 * checklist,warning_removal,dst mod id,tdd,phy cell id added
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.6.2  2009/12/26 12:09:14  gur21006
 * Warnings removed
 *
 * Revision 1.1.6.1  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.4.14  2009/11/24 12:12:50  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.4.13  2009/11/24 11:32:35  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.4.11.2.5  2009/11/24 10:39:42  ukr15916
 * *** empty log message ***
 *
 * Revision 1.1.4.11.2.4  2009/11/24 09:50:47  ukr18880
 * UECC logger is up to date
 *
 * Revision 1.1.4.11.2.3  2009/11/24 08:21:33  ukr18880
 * unused variable removed
 *
 * Revision 1.1.4.11.2.2  2009/11/23 16:43:50  ukr15916
 * beautification
 *
 * Revision 1.1.4.11.2.1  2009/11/20 15:19:12  ukr18880
 * comments removed
 *
 * Revision 1.1.4.11  2009/11/19 09:52:17  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.4.10  2009/11/14 16:29:54  ukr16032
 * UECC UE Capability transfer implemented
 * UECC ICSP - security capability checking implemented
 *
 * Revision 1.1.4.9  2009/11/13 13:14:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.4.8  2009/11/13 12:36:55  ukr18880
 * *** empty log message ***
 *
 * Revision 1.1.4.7  2009/11/13 12:23:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.4.6  2009/11/11 17:47:41  ukr16032
 * UECC
 *  -- S1AP UL NAS message generation updated
 *  -- FSM - review
 *  -- FSM - reset and rrm_release handling added to wait_for_s1_connection and
 * ue_connected states
 *  -- S1AP_RESET_MSG_IND message added
 *
 * Revision 1.1.4.4  2009/11/03 11:43:37  ukr18880
 * UECC FSM engine was updated
 *
 * Revision 1.1.4.3  2009/10/28 09:20:27  ukr18880
 * UECC FSM engine small optimization
 *
 * Revision 1.1.4.2  2009/10/27 13:56:38  ukr18880
 * UECC FSM failure processing was optimized
 *
 * Revision 1.1.4.1  2009/10/27 09:50:13  ukr15916
 * UECC added to Rel 1.0
 *
 * Revision 1.1.2.11  2009/10/27 07:59:20  ukr18880
 * UECC FSM failure processing was updated
 *
 * Revision 1.1.2.10  2009/10/26 16:14:46  ukr18880
 * UECC FSM failure processing engine initial creation
 *
 * Revision 1.1.2.9  2009/10/26 14:25:05  ukr18880
 * UECC FSM engine update
 *
 * Revision 1.1.2.8  2009/10/26 13:34:05  ukr18880
 * UECC FSM engine modification
 *
 * Revision 1.1.2.7  2009/10/26 12:58:54  ukr18880
 * UECC FSM update
 *
 * Revision 1.1.2.6  2009/10/23 12:35:41  ukr18880
 * update of UECC FSM
 *
 * Revision 1.1.2.5  2009/10/23 09:39:57  ukr18880
 * important UECC FSM engine's modification
 *
 * Revision 1.1.2.4  2009/10/22 13:48:24  ukr18880
 * UECC timer handling was changed
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

/****************************************************************************
 * Includes
 ****************************************************************************/
#include "uecc_common.h"
#include "uecc_utils.h"
#include "uecc_fsm_common.h"
#include "uecc_fsm_engine.h"
#include "uecc_mainfsm.h"

#include "rrc_common_utils.h"
#include "uecc_logger.h"
#include "uecc_global_ctx.h"
#include "rrc_logging.h"
#include "uecc_rrm.h"

static const S8* uecc_fsm_event_names[] =
{
    /* Common FSM events */
    (const S8*)"UECC_EV_STATE_INIT",
    (const S8*)"UECC_EV_TIMER_EXPIRY",
    (const S8*)"UECC_EV_ERB_ONGOING_HO_REQUIRED_INT",
    (const S8*)"UECC_EV_MME_GUARD_TIMER_EXPIRY",
    (const S8*)"UECC_EV_UE_RELEASE_REQ", /*UECC_EV_S1AP_UE_CTX_RELEASE_CMD_IND*/
    (const S8*)"UECC_EV_UE_ERROR_INDICATION", 

    (const S8*)"UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT",

    /* UE Connection Setup Control Procedure */
    (const S8*)"UECC_EV_UE_CONNECTION_SETUP_FAILURE_INT",
    (const S8*)"UECC_EV_UE_CONNECTION_SETUP_SUCCESS_INT",

    /* Initial UE Context Setup control procedure */
    (const S8*)"UECC_EV_S1AP_INITIAL_CTX_SETUP_IND",
    (const S8*)"UECC_EV_S1AP_INITIAL_CTX_SETUP_SUCCESS_INT",
    (const S8*)"UECC_EV_S1AP_INITIAL_CTX_SETUP_FAILURE_INT",
    (const S8*)"UECC_EV_S1AP_INITIAL_CTX_SETUP_RELEASE_REQ_INT",
    (const S8*)"UECC_EV_S1AP_INITIAL_CTX_SETUP_RESET_REQ_INT",
    (const S8*)"UECC_EV_S1AP_INITIAL_CTX_SETUP_RRM_RELEASE_REQ_INT",

    /* UE Connection Release */
    (const S8*)"UECC_EV_UE_RELEASED_INT",
    (const S8*)"UECC_EV_RRM_UE_CONNECTION_REL_IND_INT",

    /* HO procedure */
    (const S8*)"UECC_EV_HO_REQUIRED_INT",
    (const S8*)"UECC_EV_SRC_HO_SUCCESS_INT",
    (const S8*)"UECC_EV_SRC_HO_FAILURE_INT",
    (const S8*)"UECC_EV_SRC_HO_RE_EST_INT",
    (const S8*)"UECC_EV_SRC_HO_REL_REQ_INT",
    (const S8*)"UECC_EV_SRC_HO_RADIO_LINK_FAILURE_INT",
    (const S8*)"UECC_EV_TRG_HO_SUCCESS_INT",
    (const S8*)"UECC_EV_TRG_HO_FAILURE_INT",
    (const S8*)"UECC_EV_TRG_HO_RELEASE_REQ_INT",
    (const S8*)"UECC_EV_TRG_HO_RESET_REQ_INT",
    (const S8*)"UECC_EV_TRG_HO_CSC_DELETE_ALL_UE_REQ_INT",
    (const S8*)"UECC_EV_TRG_HO_CTX_REL_CMD_INT",
    (const S8*)"UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT",

    /* RRC-UE interface related events */
    (const S8*)"UECC_EV_RRC_CONNECTION_REQUEST",
    (const S8*)"UECC_EV_RRC_CONNECTION_SETUP_COMPLETE",
    (const S8*)"UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE",
    (const S8*)"UECC_EV_RRC_UE_CAPABILITY_INFORMATION",
    (const S8*)"UECC_EV_RRC_MEASUREMENT_REPORT_IND",
    (const S8*)"UECC_EV_RRC_SECURITY_MODE_COMPLETE",
    (const S8*)"UECC_EV_RRC_SECURITY_MODE_FAILURE",
    (const S8*)"UECC_EV_RRC_CONNECTION_RECONFIG_FAIL_INT",
    (const S8*)"UECC_EV_RRC_CONNECTION_HANDOVER_FAIL_INT",
    (const S8*)"UECC_EV_RRC_CONNECTION_OTHER_FAIL_INT",
    (const S8*)"UECC_EV_RRC_RE_ESTABLISH_REQ",
    (const S8*)"UECC_EV_RRC_RE_ESTABLISH_REQ_INT",
    (const S8*)"UECC_EV_RRC_PROXIMITY_INDICATION_IND",

    /* S1AP interface related events */
    (const S8*)"UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ",
    (const S8*)"UECC_EV_S1AP_DL_NAS_TRANSPORT_IND",
    (const S8*)"UECC_EV_S1AP_NAS_NON_DELIEVERY_IND",
    (const S8*)"UECC_EV_S1AP_RESET_MSG_IND",
    (const S8*)"UECC_EV_S1AP_DEL_UE_CONTEXT_IND",
    (const S8*)"UECC_EV_S1AP_UE_REL_REQ_INT",
/* SPR 15666 Fix Start */
    (const S8*)"UECC_EV_SIAP_ERROR_INDICATION",
/* SPR 15666 Fix Stop */

    /* Start: E-RAB Modify related events */
    (const S8*)"UECC_EV_S1AP_ERAB_RELEASE_COMMAND",
    (const S8*)"UECC_EV_S1AP_ERAB_MODIFY_REQ",
    (const S8*)"UECC_EV_S1AP_ERAB_MODIFY_RESP_SUCCESS_INT",
    (const S8*)"UECC_EV_S1AP_ERAB_MODIFY_RESP_FAILURE_INT",
    /* End: E-RAB Modify*/

    /* ERAB SETUP Start */
    (const S8*)"UECC_EV_S1AP_ERAB_SETUP_REQ",
    (const S8*)"UECC_EV_S1AP_ERAB_SETUP_SUCCESS_INT",    
    (const S8*)"UECC_EV_S1AP_ERAB_SETUP_FAILURE_INT",
    (const S8*)"UECC_EV_S1AP_ERAB_SETUP_FAILURE_REL_UE_CTX_INT",
    (const S8*)"UECC_EV_S1AP_ERAB_SETUP_PROC_TIMER_EXPIRY_INT",
    (const S8*)"UECC_EV_S1AP_ERAB_SETUP_ROLLBACK_FAILURE_REL_UE_CTX_INT",
    (const S8*)"UECC_EV_S1AP_ERAB_SETUP_UE_TIMER_EXPIRY_INT",
    (const S8*)"ERAB_SETUP_TIMER_EXPIRY",
    (const S8*)"UECC_EV_S1AP_ERAB_SETUP_RELEASE_REQ_INT",
    (const S8*)"UECC_EV_S1AP_ERAB_SETUP_RESET_REQ_INT",
    (const S8*)"UECC_EV_S1AP_ERAB_SETUP_RRM_RELEASE_REQ_INT",
    (const S8*)"UECC_EV_S1AP_ERAB_MODIFY_FAILURE_REL_UE_CTX_INT",
    (const S8*)"UECC_EV_S1AP_ERAB_RELEASE_COMMAND_SUCCESS_INT",
    (const S8*)"UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT",
    (const S8*)"UECC_EV_S1AP_RESET_MSG_IND_INT",
    (const S8*)"UECC_EV_S1AP_UE_CONTEXT_MOD_REQ",

    (const S8*)"UECC_EV_S1AP_HO_REQUEST",
    (const S8*)"UECC_EV_SRC_HO_RESET_REQ_INT",
    (const S8*)"UECC_EV_SRC_HO_CTX_REL_CMD_INT",
    (const S8*)"UECC_EV_S1AP_MME_STATUS_TRANSFER",
    (const S8*)"UECC_EV_S1AP_HO_CMD",
    (const S8*)"UECC_EV_S1AP_HO_CANCEL_ACK",
    (const S8*)"UECC_EV_S1AP_HO_PREPARATION_FAIL",

    /* RRM interface related events */
    (const S8*)"UECC_EV_RRM_UE_CONNECTION_RELEASE_IND_INT",
    (const S8*)"UECC_EV_RRM_UE_ADMISSION_RESP",
    (const S8*)"UECC_EV_RRM_UE_RELEASE_RESP",
    (const S8*)"UECC_EV_RRM_ERB_SETUP_RESP",
    (const S8*)"UECC_EV_RRM_UE_CONNECTION_RELEASE_IND",
    (const S8*)"UECC_EV_RRM_UE_CAPABILITY_ENQUIRY_REQ",
    (const S8*)"UECC_EV_RRM_HO_REQUIRED_IND",
    (const S8*)"UECC_EV_RRM_ERAB_MODIFY_RESP",
    /* End E-RAB Modify*/
    (const S8*)"UECC_EV_RRM_ERAB_RELEASE_IND_SUCCESS_INT",
    (const S8*)"UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT",
    (const S8*)"UECC_EV_RRM_ERB_RELEASE_RESP",
    (const S8*)"UECC_EV_RRM_HO_REQUIRED",
    (const S8*)"UECC_EV_RRM_ERAB_RELEASE_IND",
    (const S8*)"UECC_EV_RRM_HANDOVER_INIT",

    (const S8*)"UECC_EV_RRM_HO_ADM_RESP",      
    (const S8*)"UECC_EV_RRM_HO_CANCEL_REQ",
    (const S8*)"UECC_EV_RRM_HO_CMD_RESP",
    /* LLIM interface related events */
    (const S8*)"UECC_EV_LLIM_CREATE_UE_ENTITY_RESP",
    (const S8*)"UECC_EV_LLIM_DELETE_UE_ENTITY_RESP",
    (const S8*)"UECC_EV_LLIM_RECONFIGURE_UE_RESP",
    (const S8*)"UECC_EV_LLIM_CONFIGURE_SECURITY_RESP",
    (const S8*)"UECC_EV_LLIM_SRB_DATA_STATUS_IND",
    (const S8*)"UECC_EV_LLIM_MAC_I_RESP", 
    (const S8*)"UECC_EV_LLIM_SN_HFN_STATUS_RESP",
    (const S8*)"UECC_EV_LLIM_UE_INACTIVE_TIME_RESP",
    (const S8*)"UECC_EV_LLIM_S1U_RECONFIG_CNF",
    (const S8*)"UECC_EV_LLIM_S1U_END_MARKER_IND",
    (const S8*)"UECC_EV_LLIM_RE_EST_REQ", 
    (const S8*)"UECC_EV_LLIM_S1U_ERROR_IND",
    (const S8*)"UECC_EV_LLIM_RACH_RES_RESP",
    (const S8*)"UECC_EV_RRC_CON_RECONFIG_COMPLETE",
    (const S8*)"UECC_EV_UE_TRG_HO_S1U_RECONFIG_CNF",
    (const S8*)"UECC_EV_LLIM_RADIO_LINK_FAILURE_IND",
    (const S8*)"UECC_EV_LLIM_GUARD_TIMER_EXPIRY",
    /*SPR_19066_START*/
    (const S8*)"UECC_EV_LLIM_HO_PREP_INFO_RESP",
    /*SPR_19066_END*/
    /* Changes CSC interface related events */
    (const S8*)"UECC_EV_CSC_DELETE_ALL_UE_REQ",
    (const S8*)"UECC_EV_UE_CONNECTION_SETUP_CCSC_CELL_DELETE",
    (const S8*)"UECC_EV_CSC_CELL_DELETE_REQ_INT",

    /* RLF related*/
    (const S8*)"UECC_EV_RLF_HANDLING_FAILURE_INT",
    (const S8*)"UECC_EV_RLF_HANDLING_SUCCESS_INT",
    (const S8*)"UECC_EV_RADIO_LINK_FAILURE_IND",
    (const S8*)"UECC_EV_W_FOR_REEST_TIMER_EXPIRY",
    (const S8*)"UECC_EV_RADIO_LINK_FAILURE_INIT", 
    (const S8*)"UECC_EV_LLIM_SUSPEND_UE_ENTITY_CNF",

    (const S8*)"UECC_EV_LLIM_ADD_ROLLBACK_RESP",
    (const S8*)"UECC_EV_UE_CONNECTION_RELEASE_IND",

    /* End E-RAB Modify*/

    /* Ue context Modification Events */
    (const S8*)"UECC_EV_RRM_UE_CONTEXT_MOD_RESP",
    (const S8*)"UECC_EV_SRC_HO_CSC_DELETE_ALL_UE_REQ_INT",

    (const S8*)"UECC_EV_HO_PREP_TIMER_EXPIRY",
    (const S8*)"UECC_EV_HO_OVERALL_TIMER_EXPIRY",
    (const S8*)"UECC_EV_RRC_RE_ESTABLISH_INIT",
    (const S8*)"UECC_EV_UE_CONTXT_MOD_SUCCESS_INT",
    (const S8*)"UECC_EV_UE_CONTXT_MOD_FAIL_INT",
    (const S8*)"UECC_EV_UE_CONTXT_MOD_RE_ESTABLISH_INT",
    (const S8*)"UECC_EV_UE_CONTXT_MOD_FAIL_REL_UE_CONTXT_INT",
    (const S8*)"UECC_EV_UE_CONTXT_MOD_HANDOVER_INT",
    (const S8*)"UECC_EV_UE_CONTXT_MOD_RADIO_LINK_FAILURE_INT",
    /* Start E-RAB Modify*/
    (const S8*)"UECC_EV_LLIM_MODIFY_ROLLBACK_RESP",
    (const S8*)"UECC_EV_LLIM_SUSPEND_UE_ENTITY_RESP",
    (const S8*)"UECC_EV_LLIM_RADIO_LINK_FAILURE_INT",
    /* End E-RAB Modify*/
    (const S8*)"UECC_EV_ICS_RADIO_LINK_FAILURE_INT",    
    (const S8*)"UECC_EV_RADIO_LINK_FAILURE_INT",
    (const S8*)"UECC_EV_RRM_UE_CONNECTION_RELEASE_INT",
    (const S8*)"UECC_EV_CSC_DELETE_ALL_UE_REQ_INT",
    (const S8*)"UECC_EV_RRM_HO_REQUIRED_INT",
    (const S8*)"UECC_EV_S1AP_RESET_MSG_INT",
    (const S8*)"UECC_EV_RRC_RE_ESTABLISH_INT",
    (const S8*)"UECC_EV_UE_RELEASE_REQ_INT",
    (const S8*)"UECC_EV_UE_CONNECTION_REESTABLISH_SUCCESS_INT",
    (const S8*)"UECC_EV_UE_CONNECTION_REESTABLISH_FAILURE_INT",
    (const S8*)"UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP",
    (const S8*)"UECC_EV_LLIM_RECONFIGURE_UE_ENTITY_RESP",
    (const S8*)"UECC_EV_LLIM_RESUME_UE_ENTITY_RESP",
    (const S8*)"UECC_EV_RRC_CONNECTION_REESTABLISHMENT_COMPLETE",
    (const S8*)"UECC_EV_ICS_REESTABLISH_REQ_INT",
    (const S8*)"UECC_EV_LLIM_CHANGE_CRNTI_RESP",
    /* MME Overload */
    (const S8*)"UECC_EV_S1AP_OVERLOAD_MSG_IND",

    (const S8*)"UECC_EV_INVALID_TIMER",

    /* X2AP related events */ 
    (const S8*)"UECC_EV_X2AP_HO_REQ_ACK",
    (const S8*)"UECC_EV_X2AP_HO_PREPARATION_FAIL",
    (const S8*)"UECC_EV_X2AP_UE_CTX_RELEASE",
    (const S8*)"UECC_EV_X2AP_HO_REQUEST",
    (const S8*)"UECC_EV_X2AP_SN_STATUS_TRANSFER",
    
    (const S8*)"UECC_EV_X2AP_HO_CANCEL",
    
    (const S8*)"UECC_EV_SRC_X2AP_HO_CTX_REL_INT",

    (const S8*)"UECC_EV_X2AP_ERROR_INDICATION", 
    (const S8*)"UECC_EV_S1AP_PATH_SWITCH_REQ_ACK", 
    (const S8*)"UECC_EV_S1AP_PATH_SWITCH_REQ_FAIL", 
    /* SPR 16062 Start */
    (const S8*)"UECC_EV_HO_ONGOING_UE_CONNECTION_REESTABLISH_SUCCESS_INT", 
    /* SPR 16062 Stop */
    /*SPR_17348_START*/
    /*Code Removed */
    /*SPR_17348_END*/
    (const S8*)"UECC_EV_X2AP_RESET_MSG_IND", 
    (const S8*)"UECC_EV_INTRA_CELL_HO_TIMER_EXPIRY", 
    (const S8*)"UECC_EV_LLIM_RESET_UE_ENTITY_RESP", 

    /*Dynamic UE Reconfiguration : Start*/
    (const S8*)"UECC_EV_RRM_UE_RECONFIG_REQ", 
    (const S8*)"UECC_EV_RRM_UE_RECONFIG_SUCCESS_INT", 
    (const S8*)"UECC_EV_RRM_UE_RECONFIG_FAILURE_INT", 
    (const S8*)"UECC_EV_RRM_UE_RECONFIG_FAILURE_REL_UE_CTX_INT", 
    (const S8*)"UECC_EV_COUNTER_CHECK_TIMER_EXPIRY",
    (const S8*)"UECC_EV_NOTIFY_INTEGRITY_FAILURE",
    (const S8*)"UECC_UE_INTEGRITY_FAILURE_TIMER_EXPIRY",
    (const S8*)"UECC_EV_S1AP_ERAB_RELEASE_COMMAND_VALIDATION_FAILURE",
    (const S8*)"UECC_EV_S1AP_ALLOCATE_MME_RESP",
    (const S8*)"UECC_EV_UE_INFO_TIMER_EXPIRY",
    (const S8 *)"UECC_EV_RRM_CSFB_PARAMETERS_RESPONSE_CDMA2000",
    (const S8 *)"UECC_EV_CSFB_PARAMETERS_REQUEST_CDMA2000",
    (const S8 *)"UECC_EV_RRC_UL_HANDOVER_PREPARATION_TRANSFER",
    (const S8 *)"UECC_EV_S1AP_DL_CDMA2000_TUNNELING_HO_ONGOING",
    (const S8 *)"UECC_EV_S1AP_DL_CDMA2000_TUNNELING",
    (const S8 *)"UECC_EV_CCO_TIMER_EXPIRY",
    (const S8 *)"UECC_EV_LLIM_REESTAB_UE_ENTITY_COMP_CNF",
    (const S8 *)"UECC_EV_UE_CTX_MOD_ONGOING_HO_INT",
/*BUG 604 changes start*/
    (const S8 *)"UECC_EV_LLIM_S1U_INTRA_ENB_DATA_FWD_CNF",
    (const S8 *)"UECC_EV_LLIM_S1U_LOCAL_PATH_SWITCH_CNF",
    (const S8 *)"UECC_EV_S1AP_ALLOCATE_S1AP_UEID_RESP",
/*BUG 604 changes stop*/
    /*Bug 357 Fix Start*/
    (const S8 *)"UECC_EV_LLIM_PDCP_REESTABLISH_UE_ENTITY_RESP",
    /*Bug 357 Fix Stop*/
    /* Bug 817 Fix Start */
    (const S8 *)"UECC_EV_S1U_END_MARKER_IND_TIMER_EXPIRY",
    /* Bug 817 Fix Stop */
#ifdef ENDC_ENABLED  
    /*NR_DC Code Change START*/
    (const S8 *)"UECC_EV_X2AP_SGNB_ADDITION_REQ_ACK",
    (const S8 *)"UECC_EV_X2AP_SGNB_ADDITION_REQ_REJECT",
    /*NR_DC Code Change Stop*/
    /*EN-DC_changes_start*/
    (const S8 *)"UECC_EV_X2AP_SGNB_RELEASE_REL_ACK",
    /*EN-DC_changes_end*/
    /*NR_DC Code Change START*/
    (const S8 *)"UECC_EV_SCG_FAILURE_INFORMATION_NR",
    /*NR_DC Code Change Stop*/
    (const S8 *)"UECC_EV_X2AP_SGNB_RELEASE_REL_REQ",
/* OPTION3X Changes Start */
    (const S8 *)"UECC_EV_RRM_DC_BEARER_CHANGE_REQ",
    (const S8 *)"UECC_EV_RRM_DC_BEARER_CHANGE_CNF",
    (const S8 *)"UECC_EV_RRM_DC_BEARER_CHANGE_CNF_FAILURE_UE_REL",
    (const S8 *)"UECC_EV_S1AP_ERAB_MODIFICATION_CNF",
/* OPTION3X Changes End */
#endif
    (const S8*)"UECC_EV_LAST"

};



/****************************************************************************
 * Private functions declaration
 ****************************************************************************/
/*SPR_18241_START*/
/* Code Removed */
/*SPR_18241_END*/

/****************************************************************************
 * Public functions implementation
 ****************************************************************************/
/****************************************************************************
 * Function Name  : uecc_fsm_init_engine
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Inits UECC FSM engine
 ****************************************************************************/
void uecc_fsm_init_engine(uecc_ue_context_t*    p_ue_context)
{
    p_ue_context->uecc_fsm.p_current_state = &uecc_fsm_ue_idle;
    p_ue_context->uecc_fsm.proc_descriptor.p_active_proc = PNULL;
}

/****************************************************************************
 * Function Name  : uecc_fsm_start_procedure
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  uecc_fsm_procedure_t*  - pointer to
 *                      procedure for activation
 *                  void* p_api_data - pointer on procedure
 *                                     constructor parameters
 *
 * Outputs        : None
 * Returns        : None
 * Description    : Activates specified procedure
 ****************************************************************************/
void uecc_fsm_start_procedure(uecc_ue_context_t*                p_ue_context,
                              uecc_fsm_procedure_t*       p_procedure,
                              void*                      p_api_data
                              )
{
    uecc_fsm_procedure_descriptor_t* p_proc_descriptor = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_procedure);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_ASSERT(RRC_TRUE != uecc_fsm_is_procedure_active(p_ue_context));

    p_proc_descriptor = &p_ue_context->uecc_fsm.proc_descriptor;

    if (PNULL != p_proc_descriptor)
    {
       p_proc_descriptor->p_active_proc = p_procedure;
       p_proc_descriptor->p_activity = PNULL;
       p_proc_descriptor->p_current_state = PNULL;
       p_proc_descriptor->num_activity = 0;

       if (PNULL != p_proc_descriptor->p_active_proc)
       { 
          /* call procedure constructor */
          if (PNULL != p_proc_descriptor->p_active_proc->constructor_f)
         {
            p_proc_descriptor->p_active_proc->constructor_f(
                 p_ue_context, p_api_data);
         }

         if (PNULL != p_proc_descriptor->p_active_proc)
         {
           /* Start activity */
           uecc_fsm_start_next_activity(p_ue_context);
         }
       }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_finish_active_procedure
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Finishes current active procedure
 ****************************************************************************/
void uecc_fsm_finish_active_procedure(uecc_ue_context_t*    p_ue_context)
{
    uecc_fsm_procedure_descriptor_t* p_proc_descriptor = PNULL;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_proc_descriptor = &p_ue_context->uecc_fsm.proc_descriptor;
    p_proc_descriptor->p_active_proc = PNULL;
    p_proc_descriptor->p_activity = PNULL;
    p_proc_descriptor->p_current_state = PNULL;
    p_proc_descriptor->num_activity = 0;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_store_ongoing_active_procedure
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Stores ongoing active procedure and state 
 ****************************************************************************/
void uecc_fsm_store_ongoing_active_procedure(uecc_ue_context_t*    p_ue_context)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Copy Proc Descriptor information */  
    l3_memcpy_wrapper(&p_ue_context->prev_ongoing_uecc_fsm.proc_descriptor,
                 &p_ue_context->uecc_fsm.proc_descriptor, 
                                  sizeof(uecc_fsm_procedure_descriptor_t));

    /* Store the pointer to current FSM state */
    p_ue_context->prev_ongoing_uecc_fsm.p_current_state =  
                        p_ue_context->uecc_fsm.p_current_state; 
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_start_next_activity
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Starts next activity from procedure activities list
 ****************************************************************************/
rrc_bool_et uecc_fsm_start_next_activity(uecc_ue_context_t* p_ue_context)
{
    uecc_fsm_procedure_descriptor_t*    p_proc_descriptor = PNULL;
    U8*                                 p_num_activity = PNULL;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->uecc_fsm.proc_descriptor.p_active_proc);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_proc_descriptor = &p_ue_context->uecc_fsm.proc_descriptor;
    p_num_activity = &p_proc_descriptor->num_activity;

    /* if no more activities */
    if(PNULL==p_proc_descriptor->p_active_proc->activity_list[*p_num_activity])
    {
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FALSE;
    }

    p_proc_descriptor->p_activity =
        p_proc_descriptor->p_active_proc->activity_list[*p_num_activity];

    UECC_FSM_SET_CHILD_STATE(p_ue_context,
        p_proc_descriptor->p_active_proc->
            activity_list[*p_num_activity]->p_first_state,
        PNULL);

    (*p_num_activity)++;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_TRUE;
}

/****************************************************************************
 * Function Name  : uecc_fsm_finish_activity_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void* - pointer to data
 * Outputs        : None
 * Returns        : None
 * Description    : Finishes current activity of current active procedure
 ****************************************************************************/
void uecc_fsm_finish_activity_success(uecc_ue_context_t*    p_ue_context,
                                      void*                 p_data)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* try to start next activity from activity list */
    if(RRC_FALSE == uecc_fsm_start_next_activity(p_ue_context))
    {
        /* no activities in list - finish procedure with success */
        if(PNULL != p_ue_context->uecc_fsm.proc_descriptor.
            p_active_proc->finish_success_f)
        {
            p_ue_context->uecc_fsm.proc_descriptor.
                p_active_proc->finish_success_f(p_ue_context, p_data);
        }
        uecc_fsm_finish_active_procedure(p_ue_context);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_finish_activity_failure
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void* - pointer to data
 * Outputs        : None
 * Returns        : None
 * Description    : Finishes current activity of current active procedure
 ****************************************************************************/
void uecc_fsm_finish_activity_failure(uecc_ue_context_t*    p_ue_context,
                                      void*                 p_data)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if(PNULL != p_ue_context->uecc_fsm.proc_descriptor.
        p_active_proc->finish_fail_f)
    {
        p_ue_context->uecc_fsm.proc_descriptor.
            p_active_proc->finish_fail_f(p_ue_context, p_data);
    }
    uecc_fsm_finish_active_procedure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_set_state
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  uecc_fsm_activity_state_t** - double pointer to current
 *                      state
 *                  uecc_fsm_activity_state_t* - pointer to new state
 *                  void* - pointer to data
 * Outputs        : uecc_fsm_activity_state_t** - new state
 * Returns        : None
 * Description    : Unified function for state switching in Main FSM and
 *                      activity FSM
 ****************************************************************************/
void uecc_fsm_set_state(uecc_ue_context_t*          p_ue_context,
                        uecc_fsm_activity_state_t** p_current_state,
                        uecc_fsm_activity_state_t*  p_new_state,
                        void*                       p_data)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_new_state);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    YLIST                           *p_ue_reconfig_queue = PNULL;
    YLNODE                          *snode = PNULL;

    *p_current_state = p_new_state;
    if(RRC_TRUE == p_new_state->is_need_init)
    {
        uecc_fsm_push_init_msg(p_ue_context, p_data);
    }

    if (!strcmp_wrapper(p_new_state->s_name, (const S8 *)("UECC_UE_CONNECTED")))
    {        
        p_ue_reconfig_queue = &p_ue_context->ue_reconfig_queue;
        
        if (RECONFIGURATION_Q_GET_COUNT(p_ue_reconfig_queue))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_INFO,
                    "Dequeuing Reconfiguration Queue");
        }

        while ((RECONFIGURATION_Q_GET_COUNT(p_ue_reconfig_queue)) > 0)
        {
            RECONFIGURATION_Q_DEQUEUE(p_ue_reconfig_queue, snode);
            if(PNULL == snode)
            {
                 RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                     p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                     "[%s]snode is NULL", __FUNCTION__);
                 break;
            }

            switch((((ue_reconfig_queue_node_t *)snode)->message_type))
            {
                case MEAS_CONFIG_REQUEST :
                    {
                        uecc_rrm_process_meas_config_req(
                                (((ue_reconfig_queue_node_t *)snode)->message) , 
                                p_ue_context->p_gb_context); 
                    }
                    break;

                case UE_INFORMATION_REQUEST :
                    {
                        uecc_rrm_process_ue_information_req( 
                                (((ue_reconfig_queue_node_t *)snode)->message),
                                p_ue_context->p_gb_context);
                    }
                    break;

                case COUNTER_CHECK_REQUEST :
                    {
                        uecc_rrm_process_counter_check_req(
                                (((ue_reconfig_queue_node_t *)snode)->message),
                                p_ue_context->p_gb_context);
                    }
                    break;

                default :
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, RRC_INFO,
                            "Invalid Event Received ");
            }
            rrc_mem_free(snode);
            snode = PNULL;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_push_init_msg
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void* - pointer to init data
 * Outputs        : None
 * Returns        : None
 * Description    : Pushes initial message to queue
 ****************************************************************************/
void uecc_fsm_push_init_msg(
    uecc_ue_context_t*          p_ue_context,
    void*                       p_data)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    uecc_event_queue_push_event(UECC_EV_STATE_INIT, p_ue_context, p_data);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_process_message
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  event_id - type of uecc_fsm_event_et
 *                  void* - pointer to message
 * Outputs        : None
 * Returns        : None
 * Description    : Processes message
 ****************************************************************************/
void uecc_fsm_process_event(
    uecc_ue_context_t*          p_ue_context,
    uecc_fsm_event_et           event_id,
    void*                       p_msg)
{
    rrc_bool_et                         is_found = RRC_FALSE;
    rrc_bool_et                         is_proc_active = RRC_FALSE;
    uecc_fsm_procedure_descriptor_t*    p_proc_descriptor = PNULL;
    /*valgrind_fix start*/
    struct _uecc_gb_context_t*      p_gb_ctx = PNULL;
    /*valgrind_fix stop*/


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    if(PNULL == p_ue_context->p_gb_context)
    {
        /* global context is null return */
        return;
    }
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*valgrind_fix start*/
    /* Save the gb_context in local pointer for passing in EXIT TRACE */
    p_gb_ctx = p_ue_context->p_gb_context;
    /*valgrind_fix stop*/
    p_proc_descriptor = &p_ue_context->uecc_fsm.proc_descriptor;
    is_proc_active = uecc_fsm_is_procedure_active(p_ue_context);

    /* search handler in MHT of active state of active procedure */
    if(RRC_TRUE == is_proc_active)
    {
        RRC_ASSERT(PNULL != p_proc_descriptor->p_current_state->p_mht);
        is_found = uecc_fsm_call_handler(p_ue_context,
                                    p_proc_descriptor->p_current_state->p_mht,
                                    event_id,
                                    p_msg);

    }
    /* search handler in MHT of active procedure if any procedure is active */
    if( (RRC_TRUE == is_proc_active) && (RRC_FALSE == is_found) )
    {
        if(PNULL != p_proc_descriptor->p_active_proc->p_mht)
        {
            is_found = uecc_fsm_call_handler(p_ue_context,
                                    p_proc_descriptor->p_active_proc->p_mht,
                                    event_id,
                                    p_msg);
        }
    }
    /* search handler in MHT of Main FSM */
    if(RRC_FALSE == is_found)
    {
        RRC_ASSERT(PNULL != p_ue_context->uecc_fsm.p_current_state->p_mht);

        /*SPR 21220 Start*/
        /*SPR_19937_START*/
        if((UECC_EV_CSC_DELETE_ALL_UE_REQ == event_id ||
            UECC_EV_S1AP_RESET_MSG_IND == event_id) &&
            (!(strcmp_wrapper(p_ue_context->uecc_fsm.p_current_state->s_name,
            (const S8 *)("UECC_UE_CONNECTION_RE_ESTABLISHMENT_ONGOING")))))
        {
            if(RRC_FALSE == is_proc_active || 
                    PNULL == p_proc_descriptor->p_current_state->p_mht)
            {
                RRC_UECC_TRACE(p_gb_ctx->context_index, 
                    p_gb_ctx->facility_name,RRC_ERROR,
                    "uecc_fsm_process_event:[UE:%d] : [EVENT:%s] "
                    "[Main FSM STATE:%s]. This is unexpected behavior."
                    " This event should have been handled in procedural FSM."
                    " Value of is_proc_active = %u.",
                    p_ue_context->ue_index , uecc_fsm_event_names[event_id], 
                    p_ue_context->uecc_fsm.p_current_state->s_name, 
                    is_proc_active);
            }
            else
            {
                RRC_UECC_TRACE(p_gb_ctx->context_index, 
                    p_gb_ctx->facility_name,RRC_ERROR,
                    "uecc_fsm_process_event:[UE:%d] : [EVENT:%s] "
                    "[Main FSM STATE:%s] [Procedural FSM STATE:%s]."
                    " This is unexpected behavior."
                    " This event should have been handled in procedural FSM."
                    " Value of is_proc_active = %u and "
                    "p_proc_descriptor->p_current_state->p_mht = %p",
                    p_ue_context->ue_index , uecc_fsm_event_names[event_id], 
                    p_ue_context->uecc_fsm.p_current_state->s_name, 
                    p_proc_descriptor->p_current_state->s_name,
                    is_proc_active, p_proc_descriptor->p_current_state->p_mht);
            }

        }
        /*SPR_19937_END*/

        is_found = uecc_fsm_call_handler(p_ue_context,
                p_ue_context->uecc_fsm.p_current_state->p_mht,
                event_id,
                p_msg);

    }
    if(RRC_FALSE == is_found)
    {
        /*SPR_19937_START*/
        if(RRC_FALSE == is_proc_active || 
                PNULL == p_proc_descriptor->p_current_state->p_mht)
        {
            RRC_UECC_TRACE(p_gb_ctx->context_index, 
                    p_gb_ctx->facility_name,RRC_WARNING,
                    "uecc_fsm_process_event:[UE:%d] : [EVENT:%s] "
                    "received in invalid [STATE:%s]." 
                    " Value of is_proc_active = %u",
                    p_ue_context->ue_index , uecc_fsm_event_names[event_id], 
                    p_ue_context->uecc_fsm.p_current_state->s_name,
                    is_proc_active);
        }
        else
        {
            RRC_UECC_TRACE(p_gb_ctx->context_index, 
                    p_gb_ctx->facility_name,RRC_WARNING,
                    "uecc_fsm_process_event:[UE:%d] : [EVENT:%s] "
                    "received in invalid " 
                    "[Main FSM STATE:%s] and [Procedural FSM STATE:%s]", 
                    p_ue_context->ue_index , uecc_fsm_event_names[event_id], 
                    p_ue_context->uecc_fsm.p_current_state->s_name, 
                    p_proc_descriptor->p_current_state->s_name);
        }
        /*SPR_19937_END*/
        /*SPR 21220 Stop*/
        
        rrc_generate_unexpected_rrc_msg_event(p_ue_context, event_id);

    }
    /*valgrind_fix start*/
    RRC_UECC_UT_TRACE_EXIT(p_gb_ctx);
    /*valgrind_fix stop*/
}

/****************************************************************************
 * Function Name  : uecc_fsm_call_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  uecc_fsm_handler_record_t*  -pointer to handlers table
 *                  uecc_fsm_event_et
 *                  void* - pointer to message
 * Outputs        : RRC_TRUE/RRC_FALSE
 * Returns        : rrc_bool_et
 * Description    : Processes message
 ****************************************************************************/
rrc_bool_et uecc_fsm_call_handler(
    uecc_ue_context_t*  p_ue_context,
    uecc_fsm_handler_record_t*    p_mht,
    uecc_fsm_event_et                   event_id,
    void*                               p_data)
{
    U16                         index = UECC_LAST_ID;
    S8*                         Current_State = (p_ue_context)->uecc_fsm.p_current_state->s_name;
    /*valgrind_fix start*/
    struct _uecc_gb_context_t*      p_gb_ctx = PNULL;
    /*valgrind_fix stop*/
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_mht);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /*valgrind_fix start*/
    p_gb_ctx = p_ue_context->p_gb_context;
    if(PNULL == p_gb_ctx)
    {
        return RRC_FALSE;
    }
    /*valgrind_fix stop*/
    
    index = uecc_find_index_by_id((U16)event_id,
                                 (uecc_id_t*)p_mht,
                                 (U16)sizeof (uecc_fsm_handler_record_t));

    if( (UECC_LAST_ID == index) || (PNULL == p_mht[index].handler) )
    {
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FALSE;
    }

    /* SPR 22491 Fix Start */
    RRC_UECC_TRACE(p_gb_ctx->context_index, p_gb_ctx->facility_name, RRC_INFO,
                "### UECC_FSM Processing State %s Event: %s ###", 
                Current_State,
                uecc_fsm_event_names[event_id]);
    /* SPR 22491 Fix End */

    p_mht[index].handler(p_ue_context, p_data);

    /* SPR 22491 Fix Start */
    RRC_UECC_TRACE(p_gb_ctx->context_index, p_gb_ctx->facility_name, RRC_INFO,
                "### UECC_FSM Exiting ###");
    /* SPR 22491 Fix End */

    /*valgrind_fix start*/
    RRC_UECC_UT_TRACE_EXIT(p_gb_ctx);
    /*valgrind_fix stop*/
    return RRC_TRUE;
}

/****************************************************************************
 * Function Name  : uecc_fsm_start_failure_processing
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  uecc_fsm_fail_handler_f const*  - Pointer on the current 
 *                  failure actions list
 * Outputs        : None
 * Returns        : None
 * Description    : Activates failure processing
 ****************************************************************************/
void uecc_fsm_start_failure_processing(
    uecc_ue_context_t*              p_ue_context,
    uecc_fsm_fail_handler_f const*  p_failure_actions_list
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_failure_actions_list);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name, 
            RRC_INFO, "FAILURE: BEGIN.");

    /* Store failure actions list and list size in UE context */
    p_ue_context->failure_descriptor.failure_action_number = 0;
    p_ue_context->failure_descriptor.p_failure_actions_list =
        p_failure_actions_list;

    /* Process failure */
    uecc_fsm_process_failure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_process_failure
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Executes actions from failure actions list
 *                      for proper failure handling
 ****************************************************************************/
void uecc_fsm_process_failure(
    uecc_ue_context_t*          p_ue_context
)
{
    uecc_fsm_fail_handler_f const*   p_list;
    U16*                             p_number;
    rrc_bool_et                      break_processing = RRC_FALSE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL!=p_ue_context->failure_descriptor.p_failure_actions_list);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_list = p_ue_context->failure_descriptor.p_failure_actions_list;
    p_number = &p_ue_context->failure_descriptor.failure_action_number;

    /* Loop */
    /* If current action expects response from other RRC units -
     * break processing */
    /* If current action is last - stop failure processing */
    while((PNULL != p_list[*p_number]) && (RRC_FALSE == break_processing) )
    {
        /* Run next action form failure actions list */
        p_list[(*p_number)++](p_ue_context, &break_processing);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : rrc_generate_unexpected_rrc_msg_event
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  uecc_fsm_event_et - event_id
 * Outputs        : None
 * Returns        : None
 * Description    : Determines the message_id of a RRC message received unexpectedly
 ****************************************************************************/
void rrc_generate_unexpected_rrc_msg_event(
        uecc_ue_context_t*          p_ue_context,
        uecc_fsm_event_et           event_id
        )
{
    LOCAL_UNEXPECTED_RRC_MSG_STRUCT *p_local_unpected_rrc_msg = PNULL;
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_local_unpected_rrc_msg = rrc_mem_get(sizeof(LOCAL_UNEXPECTED_RRC_MSG_STRUCT));
    if (PNULL != p_local_unpected_rrc_msg)
    {
        memset_wrapper(p_local_unpected_rrc_msg, RRC_NULL, sizeof(LOCAL_UNEXPECTED_RRC_MSG_STRUCT));

        p_local_unpected_rrc_msg->header.length = sizeof(LOCAL_UNEXPECTED_RRC_MSG_STRUCT);
        p_local_unpected_rrc_msg->header.event_id = LOCAL_UNEXPECTED_RRC_MSG;

        if ((PNULL != p_ue_context->p_gb_context->p_p_csc_context) && 
                (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index])
                && (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                    p_csc_init_setup_ind))
        {
            l3_memcpy_wrapper(&p_local_unpected_rrc_msg->header.EVENT_PARAM_CELL_ID, 
                    p_ue_context->p_gb_context->
                    p_p_csc_context[p_ue_context->cell_index]->
                    p_csc_init_setup_ind->sib_type_1_Info.
                    cell_access_related_info.cell_Id, sizeof(U32));
        }
        p_local_unpected_rrc_msg->header.EVENT_PARAM_EUTRANCELL_FROID = 
            p_local_unpected_rrc_msg->header.EVENT_PARAM_CELL_ID;

        if (p_ue_context->m.mme_ue_id_present)
        {
            p_local_unpected_rrc_msg->header.EVENT_PARAM_MMES1APID = p_ue_context->mme_ue_id;
        }

        p_local_unpected_rrc_msg->header.EVENT_PARAM_RAC_UE_REF = p_ue_context->s1ap_ue_id;

        switch(event_id)
        {
            case UECC_EV_RRC_MEASUREMENT_REPORT_IND: 
                p_local_unpected_rrc_msg->EVENT_PARAM_RRC_MESSAGE_ID = 
                    EVENT_VALUE_MEASUREMENT_REPORT;
                break;
            case UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE:
                p_local_unpected_rrc_msg->EVENT_PARAM_RRC_MESSAGE_ID =
                    EVENT_VALUE_RRC_CONN_RECONFIG_COMPL;
                break;
            case UECC_EV_RRC_CONNECTION_REESTABLISHMENT_COMPLETE:
                p_local_unpected_rrc_msg->EVENT_PARAM_RRC_MESSAGE_ID =
                    EVENT_VALUE_RRC_CONN_REESTABL_COMPL;
                break;
            case UECC_EV_RRC_CONNECTION_SETUP_COMPLETE:
                p_local_unpected_rrc_msg->EVENT_PARAM_RRC_MESSAGE_ID =
                    EVENT_VALUE_RRC_CONN_SETUP_COMPL;
                break;
            case UECC_EV_RRC_SECURITY_MODE_COMPLETE:
                p_local_unpected_rrc_msg->EVENT_PARAM_RRC_MESSAGE_ID =
                    EVENT_VALUE_SEC_MODE_COMPL;
                break;
            case UECC_EV_RRC_SECURITY_MODE_FAILURE:
                p_local_unpected_rrc_msg->EVENT_PARAM_RRC_MESSAGE_ID =
                    EVENT_VALUE_SEC_MODE_FAIL;
                break;
            case UECC_EV_RRC_UE_CAPABILITY_INFORMATION:
                p_local_unpected_rrc_msg->EVENT_PARAM_RRC_MESSAGE_ID =
                    EVENT_VALUE_UE_CAP_INFO;
                break;
            case UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ:
                p_local_unpected_rrc_msg->EVENT_PARAM_RRC_MESSAGE_ID =
                    EVENT_VALUE_UL_INFO_TRANSFER;
                break;
            case UECC_EV_RRC_RE_ESTABLISH_REQ:
                p_local_unpected_rrc_msg->EVENT_PARAM_RRC_MESSAGE_ID =
                    EVENT_VALUE_RRC_CONN_REESTABL_REQ;
                break;
            case UECC_EV_RRC_CONNECTION_REQUEST:
                p_local_unpected_rrc_msg->EVENT_PARAM_RRC_MESSAGE_ID =
                    EVENT_VALUE_RRC_CONN_REQ;
                break;
            default:
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                        p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                        "UNEXPECTED MSG RECEIVED");
                p_local_unpected_rrc_msg->EVENT_PARAM_RRC_MESSAGE_ID =
                    EVENT_VALUE_INVALID;
        }
        rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_unpected_rrc_msg);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Private functions implementation
 ****************************************************************************/
/****************************************************************************
 * Function Name  : uecc_fsm_is_procedure_active
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : RRC_TRUE/RRC_FALSE
 * Returns        : rrc_bool_et
 * Description    : Checks is any procedure is active in UECC Main FSM
 ****************************************************************************/
/*SPR_18241_START*/
rrc_bool_et uecc_fsm_is_procedure_active(uecc_ue_context_t* p_ue_context)
/*SPR_18241_END*/
{
    if(PNULL == p_ue_context->uecc_fsm.proc_descriptor.p_active_proc)
    {
        return RRC_FALSE;
    }
    return RRC_TRUE;
}

#ifdef RRC_DEBUG

/****************************************************************************
 * Function Name  : uecc_child_fsm_get_event_name
 * Inputs         : uecc_fsm_event_et - event_id
 * Outputs        : None
 * Returns        : event name
 * Description    : This function returns the name of event using its event id.
 ****************************************************************************/
const S8* uecc_child_fsm_get_event_name(uecc_fsm_event_et event_id)
{
    RRC_ASSERT(event_id < UECC_EV_LAST);
    return uecc_fsm_event_names[event_id];
}
#endif
