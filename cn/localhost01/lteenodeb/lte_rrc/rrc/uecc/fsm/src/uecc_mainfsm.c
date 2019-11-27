/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_mainfsm.c,v 1.53 2010/12/01 13:02:39 gur19827 Exp $
 ****************************************************************************
 *
 *  File Description : UECC Main FSM functions implementation
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * 21.10.2009   AndriyT -   initial creation
 * $Log: uecc_mainfsm.c,v $
 * Revision 1.53  2010/12/01 13:02:39  gur19827
 * Merged Inter RAT HO
 *
 * Revision 1.52  2010/10/29 14:52:01  gur04518
 * Updated source HO testing fwk
 *
 * Revision 1.51  2010/10/21 12:39:29  gur04518
 * Removed handling of HO CANCEL ACK from target HO ongoing state
 *
 * Revision 1.50  2010/10/13 07:03:34  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.11.4.38  2010/10/07 06:49:05  gur04518
 * added uecc_fsm_src_ho_ue_s1u_end_marker_ind_handler to handle end_marker_ind received in src_ho ongoing state
 *
 * Revision 1.11.4.37  2010/10/06 11:52:08  gur22047
 * compiler warnings removed
 *
 * Revision 1.11.4.36  2010/10/05 14:11:20  gur16768
 * updated for s1u_end_marker_ind handler
 *
 * Revision 1.11.4.35  2010/10/05 12:36:07  gur22047
 * compilation warning removed
 *
 * Revision 1.11.4.34  2010/10/05 10:21:39  gur18569
 * added ho cancel ack in main
 *
 * Revision 1.11.4.33  2010/10/05 05:53:26  gur16768
 * added handler for event UECC_EV_LLIM_S1U_RECONFIG_CNF in UECC_UE_CONNECTED state
 *
 * Revision 1.11.4.32  2010/10/04 14:57:03  gur04518
 * Fixed spelling mistake
 *
 * Revision 1.11.4.31  2010/10/04 13:31:51  gur04518
 * removed backslash
 *
 * Revision 1.11.4.30  2010/10/04 07:08:52  gur18569
 * added ho cancel ack
 *
 * Revision 1.11.4.29  2010/10/02 06:15:09  gur04518
 * Added HO failure handler
 *
 * Revision 1.11.4.28  2010/09/29 11:56:06  gur04518
 * integrated RRM ERAB release handler with HO handler
 *
 * Revision 1.11.4.27  2010/09/29 07:00:00  gur21231
 * uecc_fsm_ho_ongoing_erab_rel_ind_handler handler added
 *
 * Revision 1.11.4.26  2010/09/28 12:15:53  gur04518
 * Starting MME guard timer while waiting for UE release command from MME
 *
 * Revision 1.11.4.25  2010/09/28 07:06:27  gur16768
 * ADDED NEW uecc_fsm_src_ho_ue_release_req_handler to handle the ue_rel request received in child fsm also responsible to release the memory allocated in child fsm
 *
 * Revision 1.11.4.24  2010/09/25 12:59:28  gur16768
 * modified the way of assigning the p_api to handover command parameter
 *
 * Revision 1.11.4.23  2010/09/16 11:49:25  gur16768
 * updated the UECC_EV_LLIM_RADIO_LINK_FAILURE_INT to UECC_EV_RADIO_LINK_FAILURE_INT in main fsm for src_ho
 *
 * Revision 1.11.4.22  2010/09/15 04:44:09  gur18569
 *  Changed handling of rrm ue rel req for if ho is ongoing
 *
 * Revision 1.11.4.21  2010/09/13 13:03:59  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.11.4.20  2010/09/09 14:28:38  gur16768
 * erab_release_failure handler fixed
 *
 * Revision 1.11.4.19  2010/09/08 11:35:26  gur21231
 * ho_ongoing_erab_rel_cmd_handler added
 *
 * Revision 1.11.4.18  2010/09/08 10:49:34  gur21231
 * erab_rel_cmd_failure_handler_for_ho
 *
 * Revision 1.11.4.17  2010/09/08 10:13:10  gur21897
 * ERAB Failure Messages : ERAB SETUP and MODIFY HANDLERS
 *
 * Revision 1.11.4.16  2010/09/07 04:12:25  gur18569
 * added a log print
 *
 * Revision 1.11.4.15  2010/09/04 12:44:27  gur16768
 * update function uecc_fsm_ue_reset_msg_ind_handler
 *
 * Revision 1.11.4.14  2010/09/03 11:32:15  gur18569
 * bug fixed while HO UT
 *
 * Revision 1.11.4.13  2010/08/30 12:22:21  gur16768
 * modified for src ho
 *
 * Revision 1.11.4.12  2010/08/30 11:56:18  gur25381
 * Removed redundant UECC_EV_LLIM_END_MARKER_IND
 *
 * Revision 1.11.4.11  2010/08/20 14:57:40  gur18569
 * code fix for trg ho
 *
 * Revision 1.11.4.10  2010/08/12 13:45:17  gur18569
 * incoporated review comments for HO
 *
 * Revision 1.11.4.9  2010/08/10 10:02:42  gur18569
 * incorported review comments for ho
 *
 * Revision 1.11.4.8  2010/08/05 08:07:13  gur15697
 * merging round 2
 *
 * Revision 1.11.4.7  2010/07/21 11:54:53  gur18569
 * incorporated code comments
 *
 * Revision 1.11.4.6  2010/07/21 08:34:41  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.11.4.5  2010/07/16 05:18:48  gur22047
 * review comments incorporation
 *
 * Revision 1.11.4.4  2010/07/15 06:25:49  gur22047
 * code review comments incorporate
 *
 * Revision 1.11.4.3  2010/06/29 10:08:09  gur22047
 * UECC_EV_S1AP_DL_NAS_TRANSPORT_IND handling for ho restriction list
 *
 * Revision 1.11.4.2  2010/06/28 12:59:28  gur16768
 * added functions to support source side handover related functionality
 *
 * Revision 1.11.4.1  2010/06/23 10:38:12  gur18569
 * Added Target Handover related changes
 *
 * Revision 1.11  2010/04/13 11:23:58  gur10248
 * SRB2 changes
 *
 * Revision 1.10  2010/04/07 15:32:59  gur21006
 * warnings removed
 *
 * Revision 1.9  2010/04/06 04:01:34  gur18569
 * changes in args of uecc_csc_build_and_send_delete_all_ue_entity_resp
 *
 * Revision 1.8  2010/04/05 09:48:24  gur10248
 * cell_delete_changes_05apr
 *
 * Revision 1.7  2010/03/24 09:50:42  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.6  2010/03/12 05:50:42  gur22133
 * ut_ue_capability_changes
 *
 * Revision 1.5  2010/02/24 04:26:07  gur10248
 * comments_dpisposed_cell_del_cell_setup_1_1
 *
 * Revision 1.4  2010/02/18 04:19:02  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.3  2010/01/11 11:42:11  gur10248
 * *** empty log message ***
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.1.1.2.37  2009/12/22 16:39:44  gur18569
 * commented field name initialization in fsm activity structures
 *
 * Revision 1.1.1.1.2.36  2009/12/08 11:22:51  ukr16032
 * UECC - inspection log items LG_20, LG_23, LG_24, LG_26, LG_27, LG_28, LG_29, LG_34, LG_40, LG_41 fixed
 *
 * Revision 1.1.1.1.2.35  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.1.1.2.33.2.31  2009/11/24 12:12:50  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.1.1.2.33.2.30  2009/11/24 11:32:35  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.1.1.2.33.2.29.2.7  2009/11/24 09:54:05  ukr16032
 * UECC FT - new FT  tests added
 *
 * Revision 1.1.1.1.2.33.2.29.2.6  2009/11/23 16:43:50  ukr15916
 * beautification
 *
 * Revision 1.1.1.1.2.33.2.29.2.5  2009/11/23 14:54:06  ukr16032
 * UECC NAS PDU processing on reconfigure added
 *
 * Revision 1.1.1.1.2.33.2.29.2.4  2009/11/23 12:12:40  ukr16032
 * UECC NAS transfer - NAS non delivery indication processing updated
 *
 * Revision 1.1.1.1.2.33.2.29.2.3  2009/11/20 19:14:40  ukr16032
 * UECC ICS failure processing updated
 *
 * Revision 1.1.1.1.2.33.2.29.2.2  2009/11/20 15:38:46  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.1.1.2.33.2.29.2.1  2009/11/20 11:09:42  ukr15916
 * unnecessary is_incr removed from statistics
 *
 * Revision 1.1.1.1.2.33.2.29  2009/11/19 10:24:57  ukr15916
 * using memset_wrapper,l3_memcpy_wrapper,memcmp_wrapper
 *
 * Revision 1.1.1.1.2.33.2.28  2009/11/19 09:52:17  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.1.1.2.33.2.27  2009/11/18 17:06:14  ukr16032
 * UECC NAS processing updated
 * (except sending NAS PDU in non delivery msg).
 * Security capability processing fixed.
 * Transaction_id generetion updated.
 *
 * Revision 1.1.1.1.2.33.2.26  2009/11/18 13:42:50  ukr18880
 * comments removed
 *
 * Revision 1.1.1.1.2.33.2.25  2009/11/17 09:42:25  ukr18880
 * uecc_ue_ctx_get_mme_ue_id(p_ue_context) function is now used instead of
 * directly using mme_ue_id from UE context, in all cases except it initializing
 *
 * Revision 1.1.1.1.2.33.2.24  2009/11/16 14:53:07  ukr15916
 * mme_id check moved to uecc_s1ap
 *
 * Revision 1.1.1.1.2.33.2.23  2009/11/15 12:32:12  ukr15916
 * mme_s1ap_id is stored in uecc context now
 *
 * Revision 1.1.1.1.2.33.2.22  2009/11/15 10:00:23  ukr15916
 * uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler - nas_pdu is taken from from
 *  llim_ssrb_data_status_ind
 *
 * Revision 1.1.1.1.2.33.2.21  2009/11/14 16:29:54  ukr16032
 * UECC UE Capability transfer implemented
 * UECC ICSP - security capability checking implemented
 *
 * Revision 1.1.1.1.2.33.2.20  2009/11/14 10:33:04  ukr18880
 * update according to new FT addition
 *
 * Revision 1.1.1.1.2.33.2.19  2009/11/13 19:36:27  ukr18880
 * comment removed
 *
 * Revision 1.1.1.1.2.33.2.18  2009/11/13 13:14:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.1.1.2.33.2.17  2009/11/13 12:36:55  ukr18880
 * *** empty log message ***
 *
 * Revision 1.1.1.1.2.33.2.16  2009/11/13 12:23:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.1.1.2.33.2.15  2009/11/12 14:36:33  ukr16032
 * UECC - Initial UE context setup procedure moved to uecc_icsp.*
 *
 * Revision 1.1.1.1.2.33.2.14  2009/11/12 01:38:08  ukr15916
 * initial_context_setup_request and initial_context_setup_response
 * updated for ASN S1AP
 *
 * Revision 1.1.1.1.2.33.2.13  2009/11/11 19:34:14  ukr15916
 * nested comments fixed
 *
 * Revision 1.1.1.1.2.33.2.12  2009/11/11 19:05:02  ukr15916
 * U16     failure_ind = (U16)p_api;
 *
 * Revision 1.1.1.1.2.33.2.11  2009/11/11 17:47:41  ukr16032
 * UECC
 *  -- S1AP UL NAS message generation updated
 *  -- FSM - review
 *  -- FSM - reset and rrm_release handling added to wait_for_s1_connection and
 * ue_connected states
 *  -- S1AP_RESET_MSG_IND message added
 *
 * Revision 1.1.1.1.2.33.2.9  2009/11/10 20:31:19  ukr18880
 * UECC statistic was implemented
 *
 * Revision 1.1.1.1.2.33.2.8  2009/11/10 11:48:15  ukr15916
 * downlink_nas_transport and nas_non_delivery_indication updated for ASN S1AP
 *
 * Revision 1.1.1.1.2.33.2.7  2009/11/08 00:27:16  ukr15916
 * s1ap-related code updated in UECC
 *
 * Revision 1.1.1.1.2.33.2.6  2009/11/06 09:52:12  ukr15916
 * UECC ue_context_release_command message updated for ASN S1AP
 *
 * Revision 1.1.1.1.2.33.2.5  2009/11/03 14:36:35  ukr18880
 * sending of RRC_RRM_MEASUREMENT_RESULTS_IND message was added (UECC -> RRM)
 *
 * Revision 1.1.1.1.2.33.2.4  2009/11/02 15:55:10  ukr15916
 * UECC - UE connection setup procedure updated.
 *
 * Revision 1.1.1.1.2.33.2.3  2009/10/30 15:21:08  ukr15916
 * Define RRC_UECC_N_UE is removed. Dynamic UE number supporting improved.
 *
 * Revision 1.1.1.1.2.33.2.2  2009/10/27 12:10:39  ukr15916
 * header fixed
 *
 * Revision 1.1.1.1.2.33.6.14  2009/10/26 12:58:54  ukr18880
 * UECC FSM update
 *
 * Revision 1.1.1.1.2.33.6.13  2009/10/26 12:01:21  ukr18880
 * *** empty log message ***
 *
 * Revision 1.1.1.1.2.33.6.12  2009/10/26 10:37:20  ukr18880
 * UECC FSM was completely finished
 *
 * Revision 1.1.1.1.2.33.6.11  2009/10/26 10:21:47  ukr18880
 * ALL, except NAS were implemented
 *
 * Revision 1.1.1.1.2.33.6.10  2009/10/26 10:09:30  ukr18880
 * *** empty log message ***
 *
 * Revision 1.1.1.1.2.33.6.9  2009/10/26 09:53:07  ukr18880
 * UECC FSM was updated - RCS, RRM, RCR FSM were completely implemented
 *
 * Revision 1.1.1.1.2.33.6.8  2009/10/26 09:09:29  ukr18880
 * UECC RM FSM is now worked on new engine
 *
 * Revision 1.1.1.1.2.33.6.7  2009/10/26 08:27:59  ukr18880
 * input S1AP message should be stored in UE context
 *
 * Revision 1.1.1.1.2.33.6.6  2009/10/23 12:35:41  ukr18880
 * update of UECC FSM
 *
 * Revision 1.1.1.1.2.33.6.5  2009/10/23 08:55:47  ukr18880
 * UECC RCR FSM is now worked on new engine
 *
 * Revision 1.1.1.1.2.33.6.4  2009/10/22 13:10:04  ukr18880
 * handlers calls logic was changed in UECC FSM engine
 *
 * Revision 1.1.1.1.2.33.6.3  2009/10/22 08:38:56  ukr18880
 * UECC RCS FSM functional tests are passed successfully now
 *
 *
 ****************************************************************************/

/****************************************************************************
* Header File Includes
***************************************************************************/

#include "uecc_utils.h"
#include "uecc_logger.h"

#include "uecc_mainfsm.h"
#include "uecc_rcsfsm.h"
#include "uecc_rcrfsm.h"
#include "uecc_icsp.h"
#include "uecc_erabsp.h"
#include "uecc_ed.h"
#include "uecc_s1ap.h"
#include "uecc_llim.h"
#include "uecc_llim_utils.h"
#include "uecc_rrm.h"
#include "uecc_ct_a.h"
#include "uecc_fsm_engine.h"
#include "uecc_common.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "uecc_csc.h"
#include "uecc_rlf.h"
#include "uecc_ue_context_modfsm.h"
#include "uecc_erab_modfsm.h"
#include "uecc_rrm_ue_reconfig_fsm.h"
/* ERB RELEASE COMMAND START */
#include "uecc_erab_delfsm.h"
/* ERB RELEASE COMMAND END */
/* ERB RELEASE INDICATION START */
#include "uecc_rrm_erab_relfsm.h"
/* ERB RELEASE INDICATION END */
#include "uecc_isa_a.h"

#include "uecc_trg_hofsm.h"
#include "uecc_src_hofsm.h"
#include "rrc_common_utils.h"
#include "rrc_rrm_intf.h"
#include "rrc_rrm_il_composer.h"
#include "rrc_rrm_il_parser.h"
#include "rrc_phy_intf.h"
#include "rrc_shared_stats.h"

#include "uecc_src_hofsm.h"
#include "uecc_crefsm.h"
#include "uecc_logger.h"
#include "rrc_logging.h"
/*SPR_17348_START*/
#include "uecc_x2ap.h"
/*SPR_17348_END*/


#ifdef ENDC_ENABLED
/*OPTION3X CHanges Start*/
#include "uecc_rrm_dc_bearer_reconfig_fsm.h"
/*OPTION3X CHanges Stop*/
#endif
/****************************************************************************
* Global Variables
****************************************************************************/
 /* x2ap_drop2_rw_start */
 extern uecc_fsm_procedure_t uecc_fsm_erb_release_procedure;

/****************************************************************************
 * Macro definitions
 ****************************************************************************/
#define RRC_UECC_UE_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,\
        p_ue_context->ue_index,\
        p_ue_context->uecc_fsm.p_current_state->s_name,\
        ##__VA_ARGS__)


#define RRC_SET_UE_CTX_DEL_STATUS(_del_flag_) \
    _del_flag_.is_rrc_resp_expected = 1


/*SPR 20901 Start*/
/* Code Removed */
/*SPR 20901 Stop*/

#define RRC_UPDATE_UE_CTX_DEL_STATUS(_del_flag_) \
    _del_flag_.is_rrc_resp_expected = 0

#define S1AP_UPDATE_UE_CTX_DEL_STATUS(_del_flag_) \
    _del_flag_.is_s1ap_resp_expected = 0


/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
 static void uecc_fsm_s1ap_ho_cancel_ack_handler(
         uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
);
/* State UECC_UE_IDLE */
static void uecc_fsm_ue_rrc_connection_request_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);
static void uecc_fsm_ue_s1ap_ho_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_x2ap_ho_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

/* State UECC_UE_CONNECTION_SETUP_ONGOING */

static void uecc_fsm_ue_connection_setup_ongoing_init_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_rrc_connection_setup_success_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_rrc_connection_setup_failure_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

/* State UECC_UE_WAIT_FOR_S1_CONNECTION */
static void uecc_fsm_ue_s1ap_initial_ctx_setup_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_reset_msg_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);
static void uecc_fsm_ue_ics_ongoing_rrm_release_req_int_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);
static void uecc_fsm_ue_csc_del_msg_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_rlf_indication_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
void uecc_fsm_ue_radio_link_failure_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_rrm_ue_connection_release_ind_int_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

static void uecc_fsm_src_ho_ue_release_req_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

static void uecc_fsm_src_x2ap_ho_ue_ctx_release_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

static void uecc_fsm_s1ap_error_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);
static void uecc_fsm_x2ap_error_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
 );

static void uecc_fsm_ue_release_req_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);
static void uecc_fsm_ue_rrc_re_establish_req_int_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);
static void uecc_fsm_trg_ho_ue_rrc_re_establish_req_int_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);
static void uecc_fsm_ue_rrc_re_establish_req_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

/* State UECC_UE_S1_CONNECTION_SETUP_ONGOING */
static void uecc_fsm_ue_s1ap_initial_ctx_setup_success_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_s1ap_initial_ctx_setup_failure_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);


static void uecc_fsm_ue_s1ap_initial_ctx_release_req_int_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

/* State UECC_UE_CONNECTION_RELEASE_ONGOING */
static void uecc_fsm_ue_released_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

/* ERAB SETUP Start */
static void uecc_fsm_ue_s1ap_erab_setup_req_msg_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);
/* ERAB SETUP Stop */
/* NAS messages handling */
void uecc_fsm_ue_s1ap_ul_nas_transport_req_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

void uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

void uecc_fsm_ue_s1ap_dl_cdma2000_tunneling_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

 void uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

/* Measurement Report handling */
 void uecc_fsm_ue_rrc_measurement_report_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

/*Proximity Indication Report handling */
void uecc_fsm_ue_rrc_proximity_indication_ind_handler(
    uecc_ue_context_t   *p_ue_context,           /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

/*SPR_17382_START*/
static void uecc_fsm_ue_s1u_error_ind_at_trg_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
/*SPR_17382_END*/

static void uecc_fsm_ue_s1u_error_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_capability_enquiry_rrm_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

void uecc_fsm_connection_setup_ue_released_ind(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
void uecc_fsm_ue_context_mod_req_handler(
        uecc_ue_context_t   *p_ue_context,  /* UECC UE context */
        void                *p_api          /* Input API buffer */
);

void uecc_fsm_ue_context_mod_ongoing_init_handler(
        uecc_ue_context_t   *p_ue_context,  /* UECC UE context */
        void                *p_api          /* Input API buffer */
);

void uecc_fsm_ue_context_mod_success_int_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_context_mod_failure_int_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_context_mod_failure_rel_ue_contxt_int_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_context_mod_re_establish_req_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_context_mod_ho_required_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_context_mod_radio_link_failure_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_context_mod_ue_release_req_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_context_mod_s1ap_reset_msg_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_context_mod_csc_delete_all_ue_req_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_context_mod_ue_connection_release_ind_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_s1ap_overload_ue_release_ind_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

rrc_return_et uecc_rrm_build_and_send_proximity_indication_ind(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void               *p_data              /* Input API buffer */
);

/*Start: Dynamic UE Reconfiguration*/
static void uecc_fsm_ue_rrm_ue_reconfig_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
/* BUG_11163 start */
static void uecc_fsm_ue_rrm_rlf_ongoing_ue_reconfig_req_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        );
void uecc_fsm_llim_resume_all_srb_ue_entity_resp_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        );
/* BUG_11163 stop */
static void uecc_fsm_ue_reconfig_success_int_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_ue_reconfig_failure_int_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_ue_reconfig_failure_rel_ue_ctx_int_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

/* Start: E-RAB Modify */
static void uecc_fsm_ue_s1ap_erab_modify_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_erb_modify_success_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_modify_failure_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_modify_ue_context_release_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void 
uecc_fsm_erb_ongoing_ho_required_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

/* End: E-RAB Modify */

/* ERB RELEASE COMMAND START */
static void uecc_fsm_ue_s1ap_erab_release_command_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
);


/* State UECC_UE_CONNECTION_RECONFIGURATION_ONGOING */
static void uecc_fsm_ue_erab_release_success_int_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_erab_release_failure_int_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_erab_release_validation_failure_int_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_rrm_ho_required_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
);

/* State UECC_UE_CONNECTION_RECONFIGURATION_ONGOING */
/*SPR 15082 Fix Start*/
static void uecc_fsm_ho_ongoing_erab_rel_cmd_handler(
        uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
        void*                p_api              /* Input API buffer */
        );
static void uecc_fsm_ue_ho_ongoing_erab_setup_req_handler(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer*/
        );
static void uecc_ho_ongoing_erab_modify_send_failure_response_to_mme(
        uecc_ue_context_t*  p_ue_context, 		/* UECC UE context */
        void*               p_api			/* Input API buffer*/
        );
/*SPR 15082 Fix Stop*/
static void uecc_fsm_ho_ongoing_erab_rel_ind_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
);

/* ERB RELEASE COMMAND END */
/* ERB RELEASE INDICATION START */
static void uecc_fsm_ue_rrm_erab_release_ind_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
);
/* ERB RELEASE INDICATION END */

/* ERAB SETUP START */

static void uecc_fsm_ue_s1ap_erab_setup_success_int_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);       

static void uecc_fsm_ue_s1ap_erab_setup_failure_int_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);  

static void uecc_fsm_ue_s1ap_erab_setup_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);  

static void uecc_fsm_ue_s1ap_erab_setup_failure_rel_ue_ctx_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

/* ERAB SETUP STOP */



static void uecc_fsm_ue_rlf_rrm_erab_release_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_state_rlf_ue_release_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_ue_w_for_conn_rel_csc_del_msg_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_w_for_ue_conn_rel_reset_msg_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_ue_connected_radio_link_failure_ind_handler( 
 uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_w_for_reest_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);

static void uecc_fsm_ue_w_s1_con_radio_link_failure_ind_handler (
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);


static void uecc_fsm_ue_rrm_ho_required_failure_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
static void uecc_fsm_rlf_handling_success_int_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
static void uecc_fsm_rlf_handling_failure_init_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);

static void uecc_fsm_ue_connection_release_ongoing_init_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_wait_for_conn_release_ue_release_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_mme_guard_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
static void uecc_fsm_rlf_handling_success_int_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
static void uecc_fsm_rlf_handling_failure_init_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);

static void uecc_fsm_ue_connection_release_ongoing_init_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_wait_for_conn_release_ue_release_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_mme_guard_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_ics_radio_link_failure_int_handler (
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_ue_rlf_reset_msg_ind_handler(
     uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
     void*                p_api              /* Input API buffer */
);

static void uecc_fsm_ue_ics_reestablish_req_int_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_ue_connected_radio_link_failure_ind_handler (
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_rlf_rrc_reestablish_req_handler  (
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
     void                *p_api              /* Input API buffer */
);
void uecc_fsm_rlf_ue_context_mod_req_handler (
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);    
void uecc_fsm_mme_timer_expiry_handler_wait_for_s1ap_ue_ctx_rel_cmd(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
/*SPR_19717_start*/
void uecc_fsm_csc_cell_delete_handler_wait_for_s1ap_ue_ctx_rel_cmd(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
/*SPR_19717_stop*/

/* SPR 16087 Start */
static void uecc_fsm_trg_ue_s1u_end_marker_ind_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);
/* SPR 16087 Stop */

static void uecc_fsm_ue_s1u_end_marker_ind_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_rlf_csc_del_msg_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_ue_s1u_reconfig_cnf_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

static void uecc_fsm_ue_rrm_ho_required_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_src_ho_failure_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_trg_ho_failure_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_ho_rel_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_ho_re_est_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_ho_rlf_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_trg_ho_success_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_context_mod_handover_ongoing_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

/*SPR_18241_START*/
static void uecc_fsm_rrm_ue_context_mod_resp_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);
/*SPR_18241_END*/

/*SPR 18241 14JAN START*/
static void uecc_fsm_ue_context_mod_reconfig_ongoing_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

static void uecc_fsm_rrm_ue_context_mod_resp_during_handover_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

static void uecc_fsm_rrm_ue_context_mod_resp_during_rlf_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);
/*SPR 18241 14JAN END*/

static void uecc_fsm_ue_context_mod_cre_ongoing_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_rrm_ho_required_src_ho_ongoing_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

/*SPR 18758 Fix Start*/
static void uecc_fsm_ue_proc_ongoing_erab_setup_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer*/
);

static void uecc_fsm_ue_proc_ongoing_erab_modify_send_failure_response_to_mme(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);
static void uecc_fsm_ue_proc_ongoing_erab_rel_cmd_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
);
static void uecc_fsm_ue_reconfig_ongoing_erab_rel_cmd_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
);
static void uecc_fsm_ue_reconfig_ongoing_erab_setup_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer*/
);

static void uecc_fsm_ue_reconfig_ongoing_erab_modify_send_failure_response_to_mme(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);
/*SPR 18758 Fix Stop*/

void uecc_fsm_ho_ue_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

void uecc_fsm_cre_ongoing_ue_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_ue_wait_for_ics_mme_guard_timer_expiry_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);


static void uecc_fsm_ue_del_ctx_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_connection_reestablish_success_int_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_ue_connection_reestablish_failure_int_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);
/*SPR_17348_START*/
/* Code Removed */
/*SPR_17348_END*/

static void uecc_fsm_ue_s1ap_erab_setup_rollback_failure_rel_ue_ctx_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

/*X2AP FIX_HO_5 : Start*/
/* SPR 16062 Start */
static void uecc_fsm_ho_ongoing_ue_connection_reestablish_success_int_handler(
/* SPR 16062 Stop */
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);
/*X2AP FIX_HO_5 : End*/
/* SPR:5661 start */
void fill_s1ap_fail_cause(uecc_ue_context_t   *p_ue_context,
                          U32 *p_fail_cause);
/* SPR:5661 stop */

static void uecc_fsm_ue_rel_req_int_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

static void uecc_fsm_s1ap_error_ind_handler_wait_for_s1ap_ue_ctx_rel_cmd(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

/* SPR 16750 Fix Start */
/* Code Removed */
/* SPR 16750 Fix Stop */
static void uecc_fsm_ue_src_ho_success_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);
/* SPR 4118 Start */
static void uecc_fsm_ue_notify_integrity_failure_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

/*SPR_17143_START*/
/*Code Removed*/
/*SPR_17143_END*/
/* SPR 4118 Stop */
static void uecc_fsm_ue_csfb_parameters_request_CDMA2000_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

static void uecc_fsm_csfb_parameters_response_CDMA2000_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api         /* Input API buffer */
);

/*securitykey*/
static void uecc_fsm_ue_ctx_ongoing_ho_success_int_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);
    
/*securitykey*/

/* Bug 817 Fix Start */
static void uecc_fsm_end_marker_ind_timer_expiry_handler( 
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api         /* Input API buffer */
);
/* Bug 817 Fix Stop */
/*SPR 13560  Fix Start */
static void uecc_fsm_ue_error_indication_message_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

/*SPR 13560 Fix Stop */

/* SPR 9343 Start */
static void uecc_fsm_rrm_ue_reconfig_failure_resp_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);
/* SPR 9343 Stop */
/* Bug 875 Fix Start */
static void uecc_fsm_ue_release_csc_del_msg_ind_handler( 
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api         /* Input API buffer */
);
/* Bug 875 Fix Stop */

/* SPR 16401 Fix Start */
static void uecc_fsm_wait_for_s1ap_ue_ctx_rel_cmd_re_establish_req_handler (
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
/* SPR 16401 Fix Stop */
/*SPR_17062_START*/
static void uecc_fsm_rrm_ue_conn_rel_ind_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
/*SPR_17062_END*/

/*SPR_17976_START*/
static void uecc_fsm_re_estb_req_during_re_establish_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);
static void uecc_fsm_mme_timer_expiry_handler_for_re_estb_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);
static void uecc_fsm_ue_release_req_handler_for_reestb(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    s1ap_Cause           cause                  /* Cause */
);
/*SPR_17976_END*/
/*SPR_17893_START*/
void uecc_fsm_connected_ue_capability_enquiry_rrm_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
/*SPR_17893_END*/

/*SPR_19066_START*/
extern uecc_fsm_activity_state_t uecc_fsm_src_ho_get_ho_prep_info_ongoing;
/*SPR_19066_END*/

/* SPR 20704 Fix Start */
static void uecc_fsm_ue_csc_del_release_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);
/* SPR 20704 Fix End */
/* SPR 21114 Fix Start */
static void uecc_fsm_rrm_ue_conn_rel_ind_handler_wait_for_s1ap_ue_ctx_rel_cmd(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
/* SPR 21114 Fix End */

#ifdef ENDC_ENABLED
/*EN-DC_changes_start*/
void uecc_fsm_scg_failure_information_nr_handler(
    uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );
/*EN-DC_changes_end*/
/* OPTION3X Changes Start */
void uecc_fsm_dc_bearer_change_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_change_cnf_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*              - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives 
 *                  the UECC_EV_RRM_DC_BEARER_CHANGE_CNF event.
 ****************************************************************************/
void
uecc_fsm_rrm_dc_bearer_change_cnf_handler
(
    uecc_ue_context_t*   p_ue_context,
    void*                p_api
);

void
uecc_fsm_rrm_dc_bearer_change_cnf_failure_ue_rel_handler
(
 uecc_ue_context_t   *p_ue_context,
 void                *p_api
 );
/* OPTION3X Changes End */

/****************************************************************************
 * Function Name  : uecc_fsm_x2ap_sgnb_initiated_sgnb_release_required_handler
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_X2AP_SGNB_RELEASE_REL_REQ message
 *                  in UECC_UE_CONNECTED states.
 ****************************************************************************/
void uecc_fsm_x2ap_sgnb_initiated_sgnb_release_required_handler
(
    uecc_ue_context_t   *p_ue_context, 
    void                *p_api 
);

/****************************************************************************
 * Function Name  : uecc_fsm_x2ap_sgnb_release_timery_expiry_handler
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_X2AP_SGNB_RELEASE_TIMER_EXPIRY message
 *                  in uecc_fsm_ue_connection_release_ongoing state.
 ****************************************************************************/
void uecc_fsm_x2ap_sgnb_release_timery_expiry_handler
(
    uecc_ue_context_t   *p_ue_context, 
    void                *p_api 
);

/****************************************************************************
 * Function Name  : uecc_fsm_ue_x2ap_reset_msg_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  X2AP_RESET_MSG_IND message
 *                  in uecc_fsm_ue_connected state
 ****************************************************************************/
void uecc_fsm_ue_x2ap_reset_msg_ind_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
);

/*Error_Handling_changes*/
/****************************************************************************
 * Function Name  : uecc_fsm_endc_x2ap_error_ind_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_X2AP_ERROR_INDICATION
 ****************************************************************************/
void uecc_fsm_endc_x2ap_error_ind_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
    );
/*Error_Handling_changes*/
#endif
/****************************************************************************
 Private Variables
****************************************************************************/
/* UECC in Idle state - no SRB and no DRB */
static uecc_fsm_handler_record_t uecc_fsm_ue_idle_mht[] =
{
    { { UECC_EV_RRC_CONNECTION_REQUEST  },
            uecc_fsm_ue_rrc_connection_request_handler  },
    { { UECC_EV_S1AP_HO_REQUEST  },
            uecc_fsm_ue_s1ap_ho_req_handler  },
    { { UECC_EV_X2AP_HO_REQUEST  },
            uecc_fsm_ue_x2ap_ho_req_handler  },
    /* SPR 20704 Fix Start */
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ              },
	    uecc_fsm_ue_csc_del_release_handler    },
    /* SPR 20704 Fix End */
    { { UECC_LAST_ID                    },            PNULL         }
};

/* UECC is in process of SRB1 establishing */
static uecc_fsm_handler_record_t uecc_fsm_ue_connection_setup_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
            uecc_fsm_ue_connection_setup_ongoing_init_handler       },
    { { UECC_EV_UE_CONNECTION_SETUP_FAILURE_INT },
            uecc_fsm_ue_rrc_connection_setup_failure_ind_handler    },
    { { UECC_EV_UE_CONNECTION_SETUP_SUCCESS_INT },
            uecc_fsm_ue_rrc_connection_setup_success_ind_handler    },
    { { UECC_EV_UE_CONNECTION_SETUP_CCSC_CELL_DELETE              },
            uecc_fsm_connection_setup_ue_released_ind                       },
    { { UECC_LAST_ID                            },  PNULL           }
};

/* UECC has SRB1 established and no DRB established */
static uecc_fsm_handler_record_t uecc_fsm_ue_wait_for_s1_connection_mht[] =
{
    { { UECC_EV_S1AP_INITIAL_CTX_SETUP_IND      },
            uecc_fsm_ue_s1ap_initial_ctx_setup_ind_handler      },
    { { UECC_EV_UE_RELEASE_REQ                  },
            uecc_fsm_ue_release_req_handler                     },
    { { UECC_EV_S1AP_RESET_MSG_IND              },
            uecc_fsm_ue_reset_msg_ind_handler                   },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND   },
            uecc_fsm_ue_rrm_ue_connection_release_ind_handler   },
    { { UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ       },
            uecc_fsm_ue_s1ap_ul_nas_transport_req_handler       },
    { { UECC_EV_S1AP_DL_NAS_TRANSPORT_IND       },
            uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler       },
    { { UECC_EV_S1AP_NAS_NON_DELIEVERY_IND      },
            uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler      },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ              },
            uecc_fsm_ue_csc_del_msg_ind_handler                 },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND     },   
            uecc_fsm_ue_w_s1_con_radio_link_failure_ind_handler },
    { { UECC_EV_RRM_HO_REQUIRED              },
            uecc_fsm_ue_rrm_ho_required_failure_handler},
    { { UECC_EV_MME_GUARD_TIMER_EXPIRY          },
        uecc_fsm_ue_wait_for_ics_mme_guard_timer_expiry_handler},
    { { UECC_EV_S1AP_UE_REL_REQ_INT          },
        uecc_fsm_ue_rel_req_int_handler},
    { { UECC_EV_S1AP_OVERLOAD_MSG_IND        },
            uecc_fsm_ue_s1ap_overload_ue_release_ind_handler    },
    { { UECC_EV_CSFB_PARAMETERS_REQUEST_CDMA2000    },
            uecc_fsm_ue_csfb_parameters_request_CDMA2000_handler },
    /*SPR 13560 Fix Start*/
    { { UECC_EV_SIAP_ERROR_INDICATION    },
            uecc_fsm_ue_error_indication_message_handler },
    /*SPR 13560 Fix Stop*/
    { {UECC_EV_RRM_CSFB_PARAMETERS_RESPONSE_CDMA2000  },
            uecc_fsm_csfb_parameters_response_CDMA2000_handler },
    /* SPR 16750 Fix Start */
    { {UECC_EV_UE_ERROR_INDICATION   },
            uecc_fsm_s1ap_error_ind_handler },
    /* SPR 16750 Fix Stop */
/*SPR 18758 Fix Start*/
    { { UECC_EV_RRM_UE_RECONFIG_REQ          },
            uecc_fsm_rrm_ue_reconfig_failure_resp_handler },
/*SPR 18758 Fix Stop*/
    { { UECC_LAST_ID                            },  PNULL       }
};

/* UECC has SRB1 established and it is in process of DRB1 establishing */
static uecc_fsm_handler_record_t uecc_fsm_ue_s1_connection_setup_ongoing_mht[] =
{
    { { UECC_EV_S1AP_INITIAL_CTX_SETUP_SUCCESS_INT  },
            uecc_fsm_ue_s1ap_initial_ctx_setup_success_ind_handler  },
    { { UECC_EV_S1AP_INITIAL_CTX_SETUP_FAILURE_INT  },
            uecc_fsm_ue_s1ap_initial_ctx_setup_failure_ind_handler  },
    { { UECC_EV_S1AP_INITIAL_CTX_SETUP_RELEASE_REQ_INT  },
            uecc_fsm_ue_s1ap_initial_ctx_release_req_int_handler    },
    { { UECC_EV_S1AP_INITIAL_CTX_SETUP_RESET_REQ_INT    },
            uecc_fsm_ue_reset_msg_ind_handler                       },
    { { UECC_EV_S1AP_INITIAL_CTX_SETUP_RRM_RELEASE_REQ_INT       },
            uecc_fsm_ue_ics_ongoing_rrm_release_req_int_handler },
    { { UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT            },
            uecc_fsm_ue_s1ap_initial_ctx_setup_failure_ind_handler    },
    { { UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ           },
            uecc_fsm_ue_s1ap_ul_nas_transport_req_handler           },
    { { UECC_EV_S1AP_DL_NAS_TRANSPORT_IND           },
            uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler           },
    { { UECC_EV_S1AP_NAS_NON_DELIEVERY_IND          },
            uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler          },
    { { UECC_EV_RRC_MEASUREMENT_REPORT_IND          },
            uecc_fsm_ue_rrc_measurement_report_ind_handler          },
    { { UECC_EV_RRM_UE_CAPABILITY_ENQUIRY_REQ      },
            uecc_fsm_ue_capability_enquiry_rrm_req_handler  },
    { { UECC_EV_CSC_CELL_DELETE_REQ_INT             },
            uecc_fsm_ue_csc_del_msg_ind_handler                       },
    { { UECC_EV_ICS_RADIO_LINK_FAILURE_INT       },
            uecc_fsm_ue_ics_radio_link_failure_int_handler               },
    { { UECC_EV_RRM_HO_REQUIRED              },
            uecc_fsm_ue_rrm_ho_required_failure_handler},
    { { UECC_EV_ICS_REESTABLISH_REQ_INT      },
            uecc_fsm_ue_ics_reestablish_req_int_handler               },
    { { UECC_EV_RRC_PROXIMITY_INDICATION_IND          },
            uecc_fsm_ue_rrc_proximity_indication_ind_handler          },
    /* SPR 3840 starts */
    { { UECC_EV_S1AP_UE_REL_REQ_INT          },
        uecc_fsm_ue_rel_req_int_handler},
    /* SPR 3840 ends */
    { {UECC_EV_CSFB_PARAMETERS_REQUEST_CDMA2000  },
        uecc_fsm_ue_csfb_parameters_request_CDMA2000_handler },
    { {UECC_EV_RRM_CSFB_PARAMETERS_RESPONSE_CDMA2000  },
            uecc_fsm_csfb_parameters_response_CDMA2000_handler },
    /* SPR 16750 Fix Start */
    { {UECC_EV_UE_ERROR_INDICATION   },
            uecc_fsm_s1ap_error_ind_handler },
    /* SPR 16750 Fix Stop */
/*SPR 18758 Fix Start*/
    { { UECC_EV_RRM_UE_RECONFIG_REQ          },
            uecc_fsm_rrm_ue_reconfig_failure_resp_handler },
/*SPR 18758 Fix Stop*/
    { { UECC_LAST_ID                                },  PNULL       }
};

static uecc_fsm_handler_record_t uecc_fsm_ue_connected_mht[] =
{
    { { UECC_EV_UE_RELEASE_REQ                  },
            uecc_fsm_ue_release_req_handler                         },
    { { UECC_EV_S1AP_RESET_MSG_IND              },
            uecc_fsm_ue_reset_msg_ind_handler                       },
    { { UECC_EV_S1AP_ERAB_SETUP_REQ             },
            uecc_fsm_ue_s1ap_erab_setup_req_msg_handler             },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ              },
            uecc_fsm_ue_csc_del_msg_ind_handler                       },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND   },
            uecc_fsm_ue_rrm_ue_connection_release_ind_handler       },
    { { UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ       },
            uecc_fsm_ue_s1ap_ul_nas_transport_req_handler           },
    { { UECC_EV_S1AP_DL_NAS_TRANSPORT_IND       },
            uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler           },
    { { UECC_EV_S1AP_NAS_NON_DELIEVERY_IND      },
            uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler          },
    { { UECC_EV_RRC_MEASUREMENT_REPORT_IND      },
            uecc_fsm_ue_rrc_measurement_report_ind_handler          },
    { { UECC_EV_RRM_UE_CAPABILITY_ENQUIRY_REQ      },
        /*SPR_17893_START*/
            uecc_fsm_connected_ue_capability_enquiry_rrm_req_handler          },
        /*SPR_17893_END*/
    { { UECC_EV_LLIM_S1U_ERROR_IND              },
            uecc_fsm_ue_s1u_error_ind_handler                   },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND     },
            uecc_fsm_ue_connected_radio_link_failure_ind_handler    },
    { { UECC_EV_RLF_HANDLING_SUCCESS_INT        },
            uecc_fsm_rlf_handling_success_int_handler              },
    { { UECC_EV_RLF_HANDLING_FAILURE_INT        },
            uecc_fsm_rlf_handling_failure_init_handler              },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ              },
            uecc_fsm_ue_rrc_re_establish_req_handler        },
    { { UECC_EV_S1AP_UE_CONTEXT_MOD_REQ         },
            uecc_fsm_ue_context_mod_req_handler                     },
    { { UECC_EV_S1AP_ERAB_MODIFY_REQ            },
            uecc_fsm_ue_s1ap_erab_modify_req_handler                },
    { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND       },
            uecc_fsm_ue_s1ap_erab_release_command_handler           },
    { { UECC_EV_RADIO_LINK_FAILURE_INT          },
           uecc_fsm_ue_connected_radio_link_failure_ind_handler     },

    { { UECC_EV_RRM_ERAB_RELEASE_IND            },
            uecc_fsm_ue_rrm_erab_release_ind_handler                },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_INT },
            uecc_fsm_ue_rrm_ue_connection_release_ind_handler }, 
    { { UECC_EV_RRC_RE_ESTABLISH_REQ_INT      },
            uecc_fsm_ue_rrc_re_establish_req_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ_INT     },
            uecc_fsm_ue_csc_del_msg_ind_handler },
    { { UECC_EV_UE_RELEASE_REQ_INT  },
            uecc_fsm_ue_release_req_handler },             
    { { UECC_EV_S1AP_RESET_MSG_INT },
            uecc_fsm_ue_reset_msg_ind_handler },      
    { { UECC_EV_RRM_HO_REQUIRED              },
            uecc_fsm_ue_rrm_ho_required_handler                   },
    { { UECC_EV_HO_REQUIRED_INT              },
            uecc_fsm_ue_rrm_ho_required_handler                   },
    { { UECC_EV_LLIM_S1U_END_MARKER_IND              },
            uecc_fsm_ue_s1u_end_marker_ind_handler                   },
    { { UECC_EV_LLIM_S1U_RECONFIG_CNF},
            uecc_fsm_ue_s1u_reconfig_cnf_handler                   },
    { { UECC_EV_S1AP_HO_CANCEL_ACK    },
            uecc_fsm_s1ap_ho_cancel_ack_handler},
    { {UECC_EV_UE_ERROR_INDICATION   },
            uecc_fsm_s1ap_error_ind_handler },
    { { UECC_EV_RRC_PROXIMITY_INDICATION_IND          },
            uecc_fsm_ue_rrc_proximity_indication_ind_handler          },
    { { UECC_EV_S1AP_DL_CDMA2000_TUNNELING            },
            uecc_fsm_ue_s1ap_dl_cdma2000_tunneling_handler            },
    { {UECC_EV_X2AP_ERROR_INDICATION   },
        uecc_fsm_x2ap_error_ind_handler },
    { { UECC_EV_RRM_UE_RECONFIG_REQ          },
            uecc_fsm_ue_rrm_ue_reconfig_req_handler          },
    /* SPR 3840 starts */
    { { UECC_EV_S1AP_UE_REL_REQ_INT          },
        uecc_fsm_ue_rel_req_int_handler},
    /* SPR 3840 ends */
    /* SPR 4118 Start */
    { {UECC_EV_NOTIFY_INTEGRITY_FAILURE   },
        uecc_fsm_ue_notify_integrity_failure_handler },
    /*SPR_17143_START*/
    /*Code Removed*/
    /*SPR_17143_END*/
    { {UECC_EV_CSFB_PARAMETERS_REQUEST_CDMA2000  },
        uecc_fsm_ue_csfb_parameters_request_CDMA2000_handler },
    { {UECC_EV_RRM_CSFB_PARAMETERS_RESPONSE_CDMA2000  },
            uecc_fsm_csfb_parameters_response_CDMA2000_handler },
    /* SPR 4118 Stop */
    /* Bug 817 Fix Start */
    { {UECC_EV_S1U_END_MARKER_IND_TIMER_EXPIRY  },
            uecc_fsm_end_marker_ind_timer_expiry_handler },
    /* Bug 817 Fix Stop */
    /*SPR_18241_START*/
    { { UECC_EV_RRM_UE_CONTEXT_MOD_RESP   },
        uecc_fsm_rrm_ue_context_mod_resp_handler},
    /*SPR_18241_END*/
#ifdef ENDC_ENABLED
    /*NR_DC Code Change START*/
    { { UECC_EV_SCG_FAILURE_INFORMATION_NR              },
       uecc_fsm_scg_failure_information_nr_handler      },
    /*NR_DC Code Change Stop*/
    /* OPTION3X Changes Start */
    { { UECC_EV_RRM_DC_BEARER_CHANGE_REQ                },
       uecc_fsm_dc_bearer_change_req_handler            },
    /* OPTION3X Changes End */
    { { UECC_EV_X2AP_SGNB_RELEASE_REL_REQ                },
       uecc_fsm_x2ap_sgnb_initiated_sgnb_release_required_handler            },
    { { UECC_EV_X2AP_RESET_MSG_IND              },
        uecc_fsm_ue_x2ap_reset_msg_ind_handler                       },
#endif
    { { UECC_LAST_ID                            },  PNULL           }
};

static uecc_fsm_handler_record_t uecc_fsm_ue_connection_release_ongoing_mht[] =
{
    { { UECC_EV_UE_RELEASED_INT },  uecc_fsm_ue_released_ind_handler        },
    { { UECC_EV_S1AP_HO_CANCEL_ACK    },
        uecc_fsm_s1ap_ho_cancel_ack_handler},
    /*SPR 21042 START*/
    /*code deleted*/
    /*SPR 21042 END*/
    /* Bug 875 Fix Start */
    /*SPR 18758 Fix Start*/
    { { UECC_EV_RRM_HO_REQUIRED              },
        uecc_fsm_ue_rrm_ho_required_failure_handler},
    { { UECC_EV_RRM_UE_RECONFIG_REQ          },
        uecc_fsm_rrm_ue_reconfig_failure_resp_handler },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND  },
        uecc_fsm_rrm_ue_conn_rel_ind_handler},
    /*SPR 18758 Fix Stop*/
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ              },
        uecc_fsm_ue_release_csc_del_msg_ind_handler                       },
    /* Bug 875 Fix Stop */
    /* SPR 19535 Fix Start */
    { { UECC_EV_S1AP_RESET_MSG_IND },  uecc_fsm_ue_del_ctx_ind_handler },
    /* SPR 19535 Fix End */
    /* SPR 22918 Fix Start */
    { { UECC_EV_S1AP_DEL_UE_CONTEXT_IND },  uecc_fsm_ue_del_ctx_ind_handler },
    /* SPR 22918 Fix  End */
    { { UECC_LAST_ID            },  PNULL                                   }
};

static uecc_fsm_handler_record_t uecc_fsm_ue_wait_for_s1ap_ue_ctx_rel_cmd_mht[] =
{
    { { UECC_EV_UE_RELEASE_REQ                  },
        uecc_fsm_ue_release_req_handler                     },
    { { UECC_EV_S1AP_RESET_MSG_IND              },
        uecc_fsm_ue_reset_msg_ind_handler                   },
    { { UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ           },
        uecc_fsm_ue_s1ap_ul_nas_transport_req_handler           },
    { { UECC_EV_S1AP_DL_NAS_TRANSPORT_IND           },
        uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler           },
    { { UECC_EV_S1AP_NAS_NON_DELIEVERY_IND          },
        uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler          },
    { { UECC_EV_MME_GUARD_TIMER_EXPIRY              },
        uecc_fsm_mme_timer_expiry_handler_wait_for_s1ap_ue_ctx_rel_cmd },
     /*SPR_19717_start*/
    { {UECC_EV_CSC_DELETE_ALL_UE_REQ                },
        uecc_fsm_csc_cell_delete_handler_wait_for_s1ap_ue_ctx_rel_cmd },
     /*SPR_19717_stop*/
    { { UECC_EV_S1AP_HO_CANCEL_ACK    },
            uecc_fsm_s1ap_ho_cancel_ack_handler},
    { {UECC_EV_UE_ERROR_INDICATION   },
        uecc_fsm_s1ap_error_ind_handler_wait_for_s1ap_ue_ctx_rel_cmd },/* SPR 16401 Fix Start */
    { { UECC_EV_RRC_RE_ESTABLISH_REQ  },
        uecc_fsm_wait_for_s1ap_ue_ctx_rel_cmd_re_establish_req_handler },/* SPR 16401 Fix Stop */
    /*SPR_17062_START*/
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND  },
        /* SPR 21114 Fix Start */
        uecc_fsm_rrm_ue_conn_rel_ind_handler_wait_for_s1ap_ue_ctx_rel_cmd},
        /* SPR 21114 Fix End */
    /*SPR_17062_END*/
/*SPR 18758 Fix Start*/
    { { UECC_EV_RRM_HO_REQUIRED              },
            uecc_fsm_ue_rrm_ho_required_failure_handler},
    { { UECC_EV_RRM_UE_RECONFIG_REQ          },
            uecc_fsm_rrm_ue_reconfig_failure_resp_handler },
/*SPR 18758 Fix Stop*/
/* SPR 20704 Fix Start */
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ              },
	    uecc_fsm_ue_w_for_conn_rel_csc_del_msg_ind_handler    },
/* SPR 20704 Fix End */
    /*Error_Handling_changes*/
#ifdef ENDC_ENABLED
    { {UECC_EV_X2AP_ERROR_INDICATION   },
        uecc_fsm_x2ap_error_ind_handler },
#endif
    /*Error_Handling_changes*/
    { { UECC_LAST_ID            },  PNULL                                   }
};

/* x2ap_drop2_rw_start -removed static */

uecc_fsm_handler_record_t
    uecc_fsm_ue_connection_reconfiguration_ongoing_mht[] =
{
/* ERAB SETUP START */
    { { UECC_EV_S1AP_ERAB_SETUP_SUCCESS_INT },
            uecc_fsm_ue_s1ap_erab_setup_success_int_handler }, 
    { { UECC_EV_S1AP_ERAB_SETUP_FAILURE_INT },
            uecc_fsm_ue_s1ap_erab_setup_failure_int_handler },
    { { UECC_EV_S1AP_ERAB_SETUP_PROC_TIMER_EXPIRY_INT },
            uecc_fsm_ue_s1ap_erab_setup_timer_expiry_handler },
    { { UECC_EV_S1AP_ERAB_SETUP_FAILURE_REL_UE_CTX_INT },
            uecc_fsm_ue_s1ap_erab_setup_failure_rel_ue_ctx_handler },
    { { UECC_EV_S1AP_ERAB_SETUP_ROLLBACK_FAILURE_REL_UE_CTX_INT },
            uecc_fsm_ue_s1ap_erab_setup_rollback_failure_rel_ue_ctx_handler },
/* ERAB SETUP STOP */

/* ERAB MODIFY_START */
    { { UECC_EV_S1AP_ERAB_MODIFY_RESP_SUCCESS_INT         },
            uecc_fsm_erb_modify_success_ind_handler                    },
    { { UECC_EV_S1AP_ERAB_MODIFY_RESP_FAILURE_INT         },
            uecc_fsm_erb_modify_failure_ind_handler                    },
    { { UECC_EV_S1AP_ERAB_MODIFY_FAILURE_REL_UE_CTX_INT   },
            uecc_fsm_erb_modify_ue_context_release_handler             },
/* ERAB MODIFY_STOP*/
    

/* ERAB REL CMD START*/
    { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND_SUCCESS_INT     },
            uecc_fsm_ue_erab_release_success_int_handler  },
    { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT     },
            uecc_fsm_ue_erab_release_failure_int_handler  },
    { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND_VALIDATION_FAILURE      },
            uecc_fsm_ue_erab_release_validation_failure_int_handler  },
/* ERAB REL CMD STOP*/

/* ERAB REL IND START*/
    { { UECC_EV_RRM_ERAB_RELEASE_IND_SUCCESS_INT           },
            uecc_fsm_ue_erab_release_success_int_handler       },
    { { UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT          },
            uecc_fsm_ue_erab_release_failure_int_handler  },
/* ERAB REL IND END */
    
/* Dynamic UE RECONFIGURATION START */
    { { UECC_EV_RRM_UE_RECONFIG_SUCCESS_INT          },
            uecc_fsm_ue_reconfig_success_int_handler  },
    { { UECC_EV_RRM_UE_RECONFIG_FAILURE_INT          },
            uecc_fsm_ue_reconfig_failure_int_handler  },
    { { UECC_EV_RRM_UE_RECONFIG_FAILURE_REL_UE_CTX_INT          },
            uecc_fsm_ue_reconfig_failure_rel_ue_ctx_int_handler  },
/* Dynamic UE RECONFIGURATION END */

/* External Events */    
    { { UECC_EV_RRM_HO_REQUIRED                           },
            uecc_fsm_ue_rrm_ho_required_handler                        },
    { { UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ                 },
            uecc_fsm_ue_s1ap_ul_nas_transport_req_handler              },
    { { UECC_EV_S1AP_DL_NAS_TRANSPORT_IND                 },
            uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler              },
    { { UECC_EV_S1AP_NAS_NON_DELIEVERY_IND                },
            uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler             },
    { { UECC_EV_RRC_MEASUREMENT_REPORT_IND                },
            uecc_fsm_ue_rrc_measurement_report_ind_handler             },
    { { UECC_EV_RRC_PROXIMITY_INDICATION_IND          },
            uecc_fsm_ue_rrc_proximity_indication_ind_handler          },

/* Internal Events */
    { { UECC_EV_UE_RELEASE_REQ_INT                  },
            uecc_fsm_ue_release_req_handler                         },
    { { UECC_EV_ERB_ONGOING_HO_REQUIRED_INT               },
            uecc_fsm_erb_ongoing_ho_required_handler        },
    { { UECC_EV_S1AP_RESET_MSG_IND_INT              },
            uecc_fsm_ue_reset_msg_ind_handler                   },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND_INT       },
            uecc_fsm_ue_rrm_ue_connection_release_ind_int_handler       },
    { { UECC_EV_CSC_CELL_DELETE_REQ_INT              },
            uecc_fsm_ue_csc_del_msg_ind_handler                       },
    { { UECC_EV_RADIO_LINK_FAILURE_INT     },
            uecc_fsm_ue_rlf_indication_handler },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ_INT     },
            uecc_fsm_ue_rrc_re_establish_req_int_handler },
    { { UECC_EV_S1AP_HO_CANCEL_ACK    },
            uecc_fsm_s1ap_ho_cancel_ack_handler},
    { { UECC_EV_S1AP_DL_CDMA2000_TUNNELING            },
            uecc_fsm_ue_s1ap_dl_cdma2000_tunneling_handler            },
    /* SPR 4118 Start */
    { {UECC_EV_NOTIFY_INTEGRITY_FAILURE   },
        uecc_fsm_ue_notify_integrity_failure_handler },
    /*SPR_17143_START*/
    /*Code Removed*/
    /*SPR_17143_END*/
    { {UECC_EV_CSFB_PARAMETERS_REQUEST_CDMA2000  },
        uecc_fsm_ue_csfb_parameters_request_CDMA2000_handler },
    { {UECC_EV_RRM_CSFB_PARAMETERS_RESPONSE_CDMA2000  },
            uecc_fsm_csfb_parameters_response_CDMA2000_handler },
/* CDMA2000_CSFB_HO stop */
    /* Dynamic_ue_reconfig + */
    { { UECC_EV_S1AP_UE_CONTEXT_MOD_REQ   },
            /*SPR 18241 14JAN START*/
            uecc_fsm_ue_context_mod_reconfig_ongoing_handler},
            /*SPR 18241 14JAN END*/
    /* Dynamic_ue_reconfig - */
    /* SPR 4118 Stop */
    /* SPR 16087 Start */
    { { UECC_EV_LLIM_S1U_END_MARKER_IND              },
            uecc_fsm_trg_ue_s1u_end_marker_ind_handler                   },
    /* SPR 16087 Stop */
    /* Bug 817 Fix Start */
    { {UECC_EV_S1U_END_MARKER_IND_TIMER_EXPIRY  },
            uecc_fsm_end_marker_ind_timer_expiry_handler },
    /* Bug 817 Fix Stop */
    /* SPR 9343 Start */
    { { UECC_EV_RRM_UE_RECONFIG_REQ          },
            uecc_fsm_rrm_ue_reconfig_failure_resp_handler },
    /* SPR 9343 Stop */
    /* SPR 16750 Fix Start */
    { {UECC_EV_UE_ERROR_INDICATION   },
            uecc_fsm_s1ap_error_ind_handler },
    /* SPR 16750 Fix Stop */
/*SPR 18758 Fix Start*/
    { { UECC_EV_S1AP_ERAB_SETUP_REQ   },
            uecc_fsm_ue_reconfig_ongoing_erab_setup_req_handler       },
    { { UECC_EV_S1AP_ERAB_MODIFY_REQ   },
            uecc_fsm_ue_reconfig_ongoing_erab_modify_send_failure_response_to_mme       },
    { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND   },
            uecc_fsm_ue_reconfig_ongoing_erab_rel_cmd_handler       },
/*SPR 18758 Fix Stop*/
#ifdef ENDC_ENABLED
    { {UECC_EV_RRM_DC_BEARER_CHANGE_CNF},
            uecc_fsm_rrm_dc_bearer_change_cnf_handler },
    { {UECC_EV_RRM_DC_BEARER_CHANGE_CNF_FAILURE_UE_REL},
            uecc_fsm_rrm_dc_bearer_change_cnf_failure_ue_rel_handler },
    /*Error_Handling_changes*/
    { {UECC_EV_X2AP_ERROR_INDICATION   },
        uecc_fsm_x2ap_error_ind_handler },
    /*Error_Handling_changes*/
#endif
    { { UECC_LAST_ID    },          PNULL   }

};

static uecc_fsm_handler_record_t 
   uecc_fsm_ue_context_mod_ongoing_mht[] = 
{
    { { UECC_EV_STATE_INIT },  
        uecc_fsm_ue_context_mod_ongoing_init_handler        },
    /*ambr*/
    { { UECC_EV_UE_CONTXT_MOD_SUCCESS_INT },
            uecc_fsm_ue_reconfig_success_int_handler  },
    { { UECC_EV_UE_CONTXT_MOD_FAIL_INT },  
            uecc_fsm_ue_reconfig_failure_int_handler  },
    { { UECC_EV_UE_CONTXT_MOD_FAIL_REL_UE_CONTXT_INT },
            uecc_fsm_ue_reconfig_failure_rel_ue_ctx_int_handler  },
    /*ambr*/
    { { UECC_EV_RRC_RE_ESTABLISH_REQ_INT },
            uecc_fsm_ue_context_mod_re_establish_req_handler        },
    /*BUG 604 changes start*/
    { { UECC_EV_RRM_HO_REQUIRED },
            uecc_fsm_ue_context_mod_ho_required_handler        },
    /*BUG 604 changes stop*/
    { { UECC_EV_RADIO_LINK_FAILURE_INT },
            uecc_fsm_ue_context_mod_radio_link_failure_handler        },
    { { UECC_EV_UE_RELEASE_REQ_INT },
            uecc_fsm_ue_context_mod_ue_release_req_handler        },
    { { UECC_EV_S1AP_RESET_MSG_INT },
            uecc_fsm_ue_context_mod_s1ap_reset_msg_handler        },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ_INT },
            uecc_fsm_ue_context_mod_csc_delete_all_ue_req_handler        },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_INT       },
            uecc_fsm_ue_context_mod_ue_connection_release_ind_handler       },
    { { UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ                 },
            uecc_fsm_ue_s1ap_ul_nas_transport_req_handler              },
    { { UECC_EV_S1AP_DL_NAS_TRANSPORT_IND                 },
            uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler              },
    { { UECC_EV_S1AP_NAS_NON_DELIEVERY_IND                },
            uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler             },
    { { UECC_EV_RRC_MEASUREMENT_REPORT_IND                },
            uecc_fsm_ue_rrc_measurement_report_ind_handler             },
    { { UECC_EV_S1AP_HO_CANCEL_ACK    },
            uecc_fsm_s1ap_ho_cancel_ack_handler},
    { { UECC_EV_RRC_PROXIMITY_INDICATION_IND          },
            uecc_fsm_ue_rrc_proximity_indication_ind_handler          },
    /* SPR 4118 Start */
    { {UECC_EV_NOTIFY_INTEGRITY_FAILURE   },
        uecc_fsm_ue_notify_integrity_failure_handler },
    /*SPR_17143_START*/
    /*Code Removed*/
    /*SPR_17143_END*/
    { {UECC_EV_CSFB_PARAMETERS_REQUEST_CDMA2000  },
        uecc_fsm_ue_csfb_parameters_request_CDMA2000_handler },
    { {UECC_EV_RRM_CSFB_PARAMETERS_RESPONSE_CDMA2000  },
            uecc_fsm_csfb_parameters_response_CDMA2000_handler },
    /*SPR 18758 Fix Start*/
    { { UECC_EV_S1AP_ERAB_SETUP_REQ   },
            uecc_fsm_ue_proc_ongoing_erab_setup_req_handler       },
    { { UECC_EV_S1AP_ERAB_MODIFY_REQ   },
            uecc_fsm_ue_proc_ongoing_erab_modify_send_failure_response_to_mme       },
    { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND   },
            uecc_fsm_ue_proc_ongoing_erab_rel_cmd_handler       },
    { { UECC_EV_S1AP_DL_CDMA2000_TUNNELING            },
            uecc_fsm_ue_s1ap_dl_cdma2000_tunneling_handler            },
    /*SPR 18758 Fix Stop*/

    /* SPR 4118 Stop */
    /* SPR 16087 Start */
    { { UECC_EV_LLIM_S1U_END_MARKER_IND              },
            uecc_fsm_trg_ue_s1u_end_marker_ind_handler                   },
    /* SPR 16087 Stop */
    /* Bug 817 Fix Start */
    { {UECC_EV_S1U_END_MARKER_IND_TIMER_EXPIRY  },
            uecc_fsm_end_marker_ind_timer_expiry_handler },
    /* Bug 817 Fix Stop */
    /* SPR 9343 Start */
    { { UECC_EV_RRM_UE_RECONFIG_REQ          },
            uecc_fsm_rrm_ue_reconfig_failure_resp_handler },
    /* SPR 9343 Stop */
    /* SPR 16750 Fix Start */
    { {UECC_EV_UE_ERROR_INDICATION   },
            uecc_fsm_s1ap_error_ind_handler },
    /* SPR 16750 Fix Stop */
    /*Error_Handling_changes*/
#ifdef ENDC_ENABLED
    { {UECC_EV_X2AP_ERROR_INDICATION   },
        uecc_fsm_x2ap_error_ind_handler },
#endif
    /*Error_Handling_changes*/
    { { UECC_LAST_ID            },  PNULL                         }
};

static uecc_fsm_handler_record_t uecc_fsm_ue_wait_for_connection_release_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
            uecc_fsm_ue_connection_release_ongoing_init_handler     },
    { { UECC_EV_UE_RELEASE_REQ         },
            uecc_fsm_wait_for_conn_release_ue_release_req_handler   },
    { { UECC_EV_S1AP_RESET_MSG_IND     },
            uecc_fsm_ue_w_for_ue_conn_rel_reset_msg_ind_handler     },
    { { UECC_EV_MME_GUARD_TIMER_EXPIRY        },
            uecc_fsm_mme_guard_timer_expiry_handler                 },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ  },
            uecc_fsm_ue_w_for_conn_rel_csc_del_msg_ind_handler      },
    { { UECC_EV_S1AP_HO_CANCEL_ACK    },
            uecc_fsm_s1ap_ho_cancel_ack_handler},
    { {UECC_EV_UE_ERROR_INDICATION   },
    /* SPR 16750 Fix Start */
            uecc_fsm_s1ap_error_ind_handler  },
    /* SPR 16750 Fix Stop */
/*SPR 18758 Fix Start*/
    { { UECC_EV_RRM_HO_REQUIRED              },
            uecc_fsm_ue_rrm_ho_required_failure_handler},
    { { UECC_EV_RRM_UE_RECONFIG_REQ          },              
            uecc_fsm_rrm_ue_reconfig_failure_resp_handler }, 
/*SPR 18758 Fix Stop*/
    /*SPR 20475 Fix Start*/
    { { UECC_EV_RRC_RE_ESTABLISH_REQ  },
        uecc_fsm_wait_for_s1ap_ue_ctx_rel_cmd_re_establish_req_handler },
    /*SPR 20475 Fix Stop*/
    /*Error_Handling_changes*/
#ifdef ENDC_ENABLED
    { {UECC_EV_X2AP_ERROR_INDICATION   },
        uecc_fsm_x2ap_error_ind_handler },
#endif
    /*Error_Handling_changes*/
    { { UECC_LAST_ID                   },  PNULL                    }
};

    uecc_fsm_handler_record_t 
    uecc_fsm_ue_radio_link_failure_handling_ongoing_mht[] =

{
    { { UECC_EV_RLF_HANDLING_SUCCESS_INT        },
            uecc_fsm_rlf_handling_success_int_handler              },
    { { UECC_EV_RLF_HANDLING_FAILURE_INT        },
            uecc_fsm_rlf_handling_failure_init_handler              },
    /*SPR_6348 Start*/
    { { UECC_EV_RRC_RE_ESTABLISH_REQ_INT              },
    /*SPR_6348 Stop*/
            uecc_fsm_ue_rrc_re_establish_req_handler        },
    { { UECC_EV_UE_RELEASE_REQ                  },
            uecc_fsm_state_rlf_ue_release_req_handler        },
    { { UECC_EV_S1AP_RESET_MSG_IND              },
            uecc_fsm_ue_rlf_reset_msg_ind_handler                       },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ              },
            uecc_fsm_ue_rlf_csc_del_msg_ind_handler                 },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND   },
            uecc_fsm_ue_rrm_ue_connection_release_ind_handler      },
    { { UECC_EV_S1AP_DL_NAS_TRANSPORT_IND       },
            uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler           },
    { { UECC_EV_LLIM_S1U_ERROR_IND              },
            uecc_fsm_ue_s1u_error_ind_handler                   },
    /* BUG_11163 start */
    { { UECC_EV_LLIM_RESUME_UE_ENTITY_RESP      },
        uecc_fsm_llim_resume_all_srb_ue_entity_resp_handler },
    /* BUG_11163 stop */
    { { UECC_EV_W_FOR_REEST_TIMER_EXPIRY                 },                     
            uecc_fsm_w_for_reest_timer_expiry_handler                        },
    { { UECC_EV_RRM_ERAB_RELEASE_IND           },
            uecc_fsm_ue_rlf_rrm_erab_release_ind_handler            },

    { { UECC_EV_RRC_RE_ESTABLISH_REQ         },
            uecc_fsm_ue_rlf_rrc_reestablish_req_handler              },
    { { UECC_EV_S1AP_UE_CONTEXT_MOD_REQ              },
            uecc_fsm_rlf_ue_context_mod_req_handler     },


    { { UECC_EV_S1AP_HO_CANCEL_ACK    },
            uecc_fsm_s1ap_ho_cancel_ack_handler},
    /* SPR 4118 Start */
    { {UECC_EV_NOTIFY_INTEGRITY_FAILURE   },
        uecc_fsm_ue_notify_integrity_failure_handler },
    /*SPR_17143_START*/
    /*Code Removed*/
    /*SPR_17143_END*/
    /* SPR 4118 Stop */
    /* SPR 9343 Start */
    /* BUG_11163 start */
    { { UECC_EV_RRM_UE_RECONFIG_REQ          },
            uecc_fsm_ue_rrm_rlf_ongoing_ue_reconfig_req_handler          },
    /* BUG_11163 stop */
    /* SPR 9343 Stop */
    /* SPR 16087 Start */
    { { UECC_EV_LLIM_S1U_END_MARKER_IND              },
            uecc_fsm_trg_ue_s1u_end_marker_ind_handler                   },
    /* SPR 16087 Stop */

    /* Bug 817 Fix Start */
    { {UECC_EV_S1U_END_MARKER_IND_TIMER_EXPIRY  },
            uecc_fsm_end_marker_ind_timer_expiry_handler },
    /* Bug 817 Fix Stop */
    /* SPR 16750 Fix Start */
    { {UECC_EV_UE_ERROR_INDICATION   },
            uecc_fsm_s1ap_error_ind_handler },
    /* SPR 16750 Fix Stop */
    /* + SPR_16046 */
    { { UECC_EV_S1AP_NAS_NON_DELIEVERY_IND      },
        uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler      },
    /* - SPR_16046 */
    /*SPR 18241 14JAN START*/
    { { UECC_EV_RRM_UE_CONTEXT_MOD_RESP   },
        uecc_fsm_rrm_ue_context_mod_resp_during_rlf_handler},
    /*Error_Handling_changes*/
#ifdef ENDC_ENABLED
    { {UECC_EV_X2AP_ERROR_INDICATION   },
        uecc_fsm_x2ap_error_ind_handler },
#endif
    /*Error_Handling_changes*/
    /*SPR 18241 14JAN END*/

    { { UECC_LAST_ID                            },  PNULL           }
};


static uecc_fsm_handler_record_t uecc_fsm_ue_src_ho_ongoing_mht[] =
{
    { { UECC_EV_SRC_HO_SUCCESS_INT      },
            uecc_fsm_ue_src_ho_success_handler      },
    { { UECC_EV_SRC_HO_FAILURE_INT      },
            uecc_fsm_ue_src_ho_failure_handler      },
    { { UECC_EV_UE_RELEASE_REQ                  },
            uecc_fsm_ue_release_req_handler                     },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND   },
            uecc_fsm_ho_ue_rrm_ue_connection_release_ind_handler       },
    { { UECC_EV_SRC_HO_REL_REQ_INT      },
            uecc_fsm_ue_ho_rel_req_handler      },
    { { UECC_EV_SRC_HO_RE_EST_INT      },
            uecc_fsm_ue_ho_re_est_handler     },
    { { UECC_EV_SRC_HO_RESET_REQ_INT      },
            uecc_fsm_ue_reset_msg_ind_handler},
    { { UECC_EV_SRC_HO_CSC_DELETE_ALL_UE_REQ_INT      },
            uecc_fsm_ue_csc_del_msg_ind_handler},
    { { UECC_EV_SRC_HO_CTX_REL_CMD_INT      },
        uecc_fsm_src_ho_ue_release_req_handler },
    { { UECC_EV_SRC_X2AP_HO_CTX_REL_INT      },
        uecc_fsm_src_x2ap_ho_ue_ctx_release_handler },
    { { UECC_EV_RRC_MEASUREMENT_REPORT_IND      },
            uecc_fsm_ue_rrc_measurement_report_ind_handler          },
    { { UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ   },
            uecc_fsm_ue_s1ap_ul_nas_transport_req_handler       },
    { { UECC_EV_S1AP_DL_NAS_TRANSPORT_IND       },
            uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler       },
    { { UECC_EV_S1AP_NAS_NON_DELIEVERY_IND      },
            uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler      },
    /*SPR 15082 Fix Start*/
    { { UECC_EV_S1AP_ERAB_SETUP_REQ   },
        uecc_fsm_ue_ho_ongoing_erab_setup_req_handler       },
    { { UECC_EV_S1AP_ERAB_MODIFY_REQ   },
        uecc_ho_ongoing_erab_modify_send_failure_response_to_mme       },
    { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND   },
        uecc_fsm_ho_ongoing_erab_rel_cmd_handler       },
    /*SPR 15082 Fix Stop*/
    { { UECC_EV_SRC_HO_RADIO_LINK_FAILURE_INT      },
            uecc_fsm_ue_ho_rlf_handler},
    { { UECC_EV_S1AP_UE_CONTEXT_MOD_REQ   },
            uecc_fsm_ue_context_mod_handover_ongoing_handler       },
    { { UECC_EV_RRM_HO_REQUIRED              },
            uecc_fsm_ue_rrm_ho_required_src_ho_ongoing_handler                   },
    { { UECC_EV_RRM_ERAB_RELEASE_IND    },
            uecc_fsm_ho_ongoing_erab_rel_ind_handler},
    { { UECC_EV_S1AP_HO_CANCEL_ACK    },
            uecc_fsm_s1ap_ho_cancel_ack_handler},
    { { UECC_EV_LLIM_S1U_END_MARKER_IND },
            /* BUG_8114 */
            uecc_fsm_ue_s1u_end_marker_ind_handler},
            /* BUG_8114 */
    { { UECC_EV_RRC_PROXIMITY_INDICATION_IND          },
            uecc_fsm_ue_rrc_proximity_indication_ind_handler          },
    { {	UECC_EV_S1AP_DL_CDMA2000_TUNNELING            },
	        uecc_fsm_ue_s1ap_dl_cdma2000_tunneling_handler	      },
    /* SPR 4118 Start */
    { {UECC_EV_NOTIFY_INTEGRITY_FAILURE   },
        uecc_fsm_ue_notify_integrity_failure_handler },
    /*SPR_17143_START*/
    /*Code Removed*/
    /*SPR_17143_END*/
    { {UECC_EV_CSFB_PARAMETERS_REQUEST_CDMA2000  },
        uecc_fsm_ue_csfb_parameters_request_CDMA2000_handler },
    { {UECC_EV_RRM_CSFB_PARAMETERS_RESPONSE_CDMA2000  },
            uecc_fsm_csfb_parameters_response_CDMA2000_handler },
    /*securitykey*/
    { { UECC_EV_UE_CTX_MOD_ONGOING_HO_INT },
            uecc_fsm_ue_ctx_ongoing_ho_success_int_handler },
    /*securitykey*/
    /* SPR 4118 Stop */
    /* SPR 9343 Start */
    { { UECC_EV_RRM_UE_RECONFIG_REQ          },
            uecc_fsm_rrm_ue_reconfig_failure_resp_handler },
    /* SPR 9343 Stop */
    /* SPR 16750 Fix Start */
    { {UECC_EV_UE_ERROR_INDICATION   },
            uecc_fsm_s1ap_error_ind_handler },
    /* SPR 16750 Fix Stop */
    /*SPR 18241 14JAN START*/
    { { UECC_EV_RRM_UE_CONTEXT_MOD_RESP   },
        uecc_fsm_rrm_ue_context_mod_resp_during_handover_handler},
    /*SPR 18241 14JAN END*/
    { { UECC_LAST_ID            },  PNULL                       }
};

static uecc_fsm_handler_record_t uecc_fsm_ue_trg_ho_ongoing_mht[] =
{
    { { UECC_EV_TRG_HO_SUCCESS_INT   },
            uecc_fsm_ue_trg_ho_success_handler       },
    { { UECC_EV_TRG_HO_FAILURE_INT      },
            uecc_fsm_ue_trg_ho_failure_handler      },
    { { UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT   },
            uecc_fsm_ue_ho_rel_req_handler},
    { { UECC_EV_TRG_HO_CTX_REL_CMD_INT      },
            uecc_fsm_ue_s1ap_initial_ctx_release_req_int_handler},
    /*SPR_17348_START*/
    /*Code Removed */
    /*SPR_17348_END*/
    { { UECC_EV_TRG_HO_CSC_DELETE_ALL_UE_REQ_INT      },
            uecc_fsm_ue_csc_del_msg_ind_handler},
    { { UECC_EV_TRG_HO_RELEASE_REQ_INT      },
            uecc_fsm_ue_s1ap_initial_ctx_release_req_int_handler},
    { { UECC_EV_TRG_HO_RESET_REQ_INT      },
            uecc_fsm_ue_reset_msg_ind_handler},
    { { UECC_EV_LLIM_S1U_END_MARKER_IND              },
            uecc_fsm_ue_s1u_end_marker_ind_handler                   },
    { { UECC_EV_RRM_ERAB_RELEASE_IND    },
            uecc_fsm_ho_ongoing_erab_rel_ind_handler},
    { { UECC_EV_RRM_HO_REQUIRED              },
            uecc_fsm_ue_rrm_ho_required_failure_handler},
    { { UECC_EV_RRC_RE_ESTABLISH_REQ_INT     },
            uecc_fsm_trg_ho_ue_rrc_re_establish_req_int_handler },
    /*securitykey*/
    { { UECC_EV_UE_CTX_MOD_ONGOING_HO_INT },
        uecc_fsm_ue_ctx_ongoing_ho_success_int_handler },
    /*securitykey*/
    { { UECC_EV_S1AP_UE_CONTEXT_MOD_REQ   },
        /*SPR_18491_START*/
        uecc_fsm_ue_context_mod_reconfig_ongoing_handler },
    /*SPR_18491_END*/
    /* Bug 817 Fix Start */
    { {UECC_EV_S1U_END_MARKER_IND_TIMER_EXPIRY  },
        uecc_fsm_end_marker_ind_timer_expiry_handler },
    /* Bug 817 Fix Stop */
    /* SPR 9343 Start */
    { { UECC_EV_RRM_UE_RECONFIG_REQ          },
            uecc_fsm_rrm_ue_reconfig_failure_resp_handler },
    /* SPR 9343 Stop */
    /*SPR_17143_START*/
    { {UECC_EV_NOTIFY_INTEGRITY_FAILURE   },
        uecc_fsm_ue_notify_integrity_failure_handler },
    /*SPR_17143_END*/
    /*SPR_17382_START*/
    { { UECC_EV_LLIM_S1U_ERROR_IND              },
            uecc_fsm_ue_s1u_error_ind_at_trg_handler },
    /*SPR_17382_END*/
    /*SPR 18758 Fix Start*/
    { { UECC_EV_S1AP_ERAB_SETUP_REQ   },
            uecc_fsm_ue_proc_ongoing_erab_setup_req_handler       },
    { { UECC_EV_S1AP_ERAB_MODIFY_REQ   },
            uecc_fsm_ue_proc_ongoing_erab_modify_send_failure_response_to_mme       },
    { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND   },
            uecc_fsm_ue_proc_ongoing_erab_rel_cmd_handler       },
    /*SPR 18758 Fix Stop*/
    { { UECC_LAST_ID            },  PNULL                       }
};

static uecc_fsm_handler_record_t
    uecc_fsm_ue_connection_re_establishment_ongoing_mht[] =
{
    /*X2AP FIX_HO_5 : Start*/
    /* SPR 16062 Start */
    { { UECC_EV_HO_ONGOING_UE_CONNECTION_REESTABLISH_SUCCESS_INT },
            uecc_fsm_ho_ongoing_ue_connection_reestablish_success_int_handler }, 
    /* SPR 16062 Stop */
    /*X2AP FIX_HO_5 : End*/
    { { UECC_EV_UE_CONNECTION_REESTABLISH_SUCCESS_INT },
            uecc_fsm_ue_connection_reestablish_success_int_handler }, 
    { { UECC_EV_UE_CONNECTION_REESTABLISH_FAILURE_INT },
            uecc_fsm_ue_connection_reestablish_failure_int_handler },
    { { UECC_EV_S1AP_RESET_MSG_IND_INT              },
            uecc_fsm_ue_reset_msg_ind_handler                   },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND_INT       },
            uecc_fsm_ue_rrm_ue_connection_release_ind_int_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ_INT           },
            uecc_fsm_ue_csc_del_msg_ind_handler    },
    { { UECC_EV_UE_RELEASE_REQ_INT  },
            uecc_fsm_ue_release_req_handler },             
    { { UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ   },
            uecc_fsm_ue_s1ap_ul_nas_transport_req_handler       },
    { { UECC_EV_S1AP_DL_NAS_TRANSPORT_IND       },
            uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler       },
/*SPR 18758 Fix Start*/
    { { UECC_EV_S1AP_ERAB_SETUP_REQ   },
            uecc_fsm_ue_proc_ongoing_erab_setup_req_handler       },
    { { UECC_EV_S1AP_ERAB_MODIFY_REQ   },
            uecc_fsm_ue_proc_ongoing_erab_modify_send_failure_response_to_mme       },
    { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND   },
            uecc_fsm_ue_proc_ongoing_erab_rel_cmd_handler       },
/*SPR 18758 Fix Stop*/
    { { UECC_EV_S1AP_NAS_NON_DELIEVERY_IND                },
            uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler             },
    { { UECC_EV_RRC_MEASUREMENT_REPORT_IND                },
            uecc_fsm_ue_rrc_measurement_report_ind_handler             },
    { { UECC_EV_S1AP_UE_CONTEXT_MOD_REQ   },
            uecc_fsm_ue_context_mod_cre_ongoing_handler       },
    { { UECC_EV_RRM_HO_REQUIRED              },
            uecc_fsm_ue_rrm_ho_required_failure_handler},
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND   },
            uecc_fsm_cre_ongoing_ue_rrm_ue_connection_release_ind_handler },
    { { UECC_EV_RRC_PROXIMITY_INDICATION_IND          },
            uecc_fsm_ue_rrc_proximity_indication_ind_handler          },
    { { UECC_EV_S1AP_DL_CDMA2000_TUNNELING            },
            uecc_fsm_ue_s1ap_dl_cdma2000_tunneling_handler            },
    /* SPR 4118 Start */
    { {UECC_EV_NOTIFY_INTEGRITY_FAILURE   },
        uecc_fsm_ue_notify_integrity_failure_handler },
    /*SPR_17143_START*/
    /*Code Removed*/
    /*SPR_17143_END*/
    { {UECC_EV_CSFB_PARAMETERS_REQUEST_CDMA2000  },
        uecc_fsm_ue_csfb_parameters_request_CDMA2000_handler },
    { {UECC_EV_RRM_CSFB_PARAMETERS_RESPONSE_CDMA2000  },
            uecc_fsm_csfb_parameters_response_CDMA2000_handler },
    /* SPR 4118 Stop */
    /* SPR 16087 Start */
    { { UECC_EV_LLIM_S1U_END_MARKER_IND              },
            uecc_fsm_trg_ue_s1u_end_marker_ind_handler                   },
    /* SPR 16087 Stop */
    /* Bug 817 Fix Start */
    { {UECC_EV_S1U_END_MARKER_IND_TIMER_EXPIRY  },
            uecc_fsm_end_marker_ind_timer_expiry_handler },
    /* Bug 817 Fix Stop */
    /* SPR 9343 Start */
    { { UECC_EV_RRM_UE_RECONFIG_REQ          },
            uecc_fsm_rrm_ue_reconfig_failure_resp_handler },
    /* Bug_11958 Fix Start */
    { { UECC_EV_RRC_RE_ESTABLISH_REQ_INT     },
        uecc_fsm_ue_rrc_re_establish_req_int_handler },
    /* Bug_11958 Fix Stop */
    /* SPR 9343 Stop */
    /* SPR 16750 Fix Start */
    { {UECC_EV_UE_ERROR_INDICATION   },
            uecc_fsm_s1ap_error_ind_handler },
    /* SPR 16750 Fix Stop */
    /*SPR_17976_START*/
    { { UECC_EV_RRC_RE_ESTABLISH_REQ              },
            uecc_fsm_re_estb_req_during_re_establish_handler },
    { { UECC_EV_MME_GUARD_TIMER_EXPIRY              },
            uecc_fsm_mme_timer_expiry_handler_for_re_estb_handler },
    /*SPR_17976_END*/
    /*SPR_19937_START*/
    { { UECC_EV_S1AP_RESET_MSG_IND              },
            uecc_fsm_ue_reset_msg_ind_handler                   },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ },
            uecc_fsm_ue_csc_del_msg_ind_handler    },
    /*SPR_19937_END*/
    /*Error_Handling_changes*/
#ifdef ENDC_ENABLED
    { {UECC_EV_X2AP_ERROR_INDICATION   },
        uecc_fsm_x2ap_error_ind_handler },
#endif
    /*Error_Handling_changes*/
    { { UECC_LAST_ID    },          PNULL   }
};


uecc_fsm_activity_state_t uecc_fsm_ue_idle =
{
   /* .s_name         = */  (S8*)"UECC_UE_IDLE",
   /* .s_id           = */  (S8*)"M_IDLE",
   /* .is_need_init   = */  RRC_FALSE,
   /* .p_mht          = */  uecc_fsm_ue_idle_mht
};

static uecc_fsm_activity_state_t uecc_fsm_ue_connection_setup_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_UE_CONNECTION_SETUP_ONGOING",
   /* .s_id           = */  (S8*)"M_SRB1",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_ue_connection_setup_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_ue_wait_for_s1_connection =
{
   /* .s_name         = */  (S8*)"UECC_UE_WAIT_FOR_S1_CONNECTION",
   /* .s_id           = */  (S8*)"M_WAIT_ICS",
   /* .is_need_init   = */  RRC_FALSE,
   /* .p_mht          = */  uecc_fsm_ue_wait_for_s1_connection_mht
};

static uecc_fsm_activity_state_t uecc_fsm_ue_s1_connection_setup_ongoing =
{
   /* .s_name         = */ (S8*)"UECC_UE_S1_CONNECTION_SETUP_ONGOING",
   /* .s_id           = */ (S8*)"M_ICS_O",
   /* .is_need_init   = */  RRC_FALSE,
   /* .p_mht          = */  uecc_fsm_ue_s1_connection_setup_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_ue_connected =
{
   /* .s_name         = */  (S8*)"UECC_UE_CONNECTED",
   /* .s_id           = */  (S8*)"M_CONN",
   /* .is_need_init   = */  RRC_FALSE,
   /* .p_mht          = */  uecc_fsm_ue_connected_mht
};

static uecc_fsm_activity_state_t uecc_fsm_ue_connection_reconfiguration_ongoing=
{
   /* .s_name         = */  (S8*)"UECC_UE_CONNECTION_RECONFIGURATION_ONGOING",
   /* .s_id           = */  (S8*)"M_RECONF",
   /* .is_need_init   = */  RRC_FALSE,
   /* .p_mht          = */  uecc_fsm_ue_connection_reconfiguration_ongoing_mht
};

static 
uecc_fsm_activity_state_t uecc_fsm_ue_connection_re_establishment_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_UE_CONNECTION_RE_ESTABLISHMENT_ONGOING",
   /* .s_id           = */  (S8*)"M_CONN_RE_ESTABLISHMENT_O",
   /* .is_need_init   = */  RRC_FALSE,
   /* .p_mht          = */  uecc_fsm_ue_connection_re_establishment_ongoing_mht 
};

uecc_fsm_activity_state_t uecc_fsm_ue_connection_release_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_UE_CONNECTION_RELEASE_ONGOING",
   /* .s_id           = */  (S8*)"M_REL",
   /* .is_need_init   = */  RRC_FALSE,
   /* .p_mht          = */  uecc_fsm_ue_connection_release_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_ue_src_ho_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_UE_SRC_HO_ONGOING",
   /* .s_id           = */  (S8*)"M_SRC_HO_O",
   /* .is_need_init   = */  RRC_FALSE,
   /* .p_mht          = */  uecc_fsm_ue_src_ho_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_ue_context_mod_ongoing =
{
    /* .s_name         = */  (S8*)"UECC_UE_CONTEXT_MOD_ONGOING",
    /* .s_id           = */  (S8*)"CONTEXT_MOD",
    /* .is_need_init   = */  RRC_FALSE,
    /* .p_mht          = */  uecc_fsm_ue_context_mod_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_ue_wait_for_connection_release =
{
   /* .s_name         = */  (S8*)"UECC_UE_WAIT_FOR_CONNECTION_RELEASE",
   /* .s_id           = */  (S8*)"W_CONNECTION_RELEASE",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_ue_wait_for_connection_release_mht
};

static uecc_fsm_activity_state_t
    uecc_fsm_ue_radio_link_failure_handling_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_UE_RADIO_LINK_FAILURE_HANDLING_ONGOING",
   /* .s_id           = */  (S8*)"RLF_HANDLING",
   /* .is_need_init   = */  RRC_FALSE,
   /* .p_mht          = */  uecc_fsm_ue_radio_link_failure_handling_ongoing_mht
}; 

static uecc_fsm_activity_state_t uecc_fsm_ue_wait_for_s1ap_ue_ctx_rel_cmd =
{
   /* .s_name         = */  (S8*)"UECC_UE_WAIT_FOR_S1AP_UE_CTX_REL_CMD_ONGOING",
   /* .s_id           = */  (S8*)"M_MME",
   /* .is_need_init   = */  RRC_FALSE,
   /* .p_mht          = */  uecc_fsm_ue_wait_for_s1ap_ue_ctx_rel_cmd_mht
};

static uecc_fsm_activity_state_t uecc_fsm_ue_trg_ho_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_UE_TRG_HO_ONGOING",
   /* .s_id           = */  (S8*)"M_TRG_HO_O",
   /* .is_need_init   = */  RRC_FALSE,
   /* .p_mht          = */  uecc_fsm_ue_trg_ho_ongoing_mht
};

/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/
/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrc_connection_request_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UECC fsm receives the
*                   UECC_EV_RRC_CONNECTION_REQUEST message.
 ****************************************************************************/
void uecc_fsm_ue_rrc_connection_request_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRCConnectionRequest_r8_IEs     *p_rrcConnectionRequest_r8 = PNULL;
    rrc_return_et                   result = RRC_SUCCESS;
     time_t time1 ;
     time_wrapper(&time1); 

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
      
    /* Get pointer on API data */
    p_rrcConnectionRequest_r8 = (RRCConnectionRequest_r8_IEs*)p_api;

    /* Store rrcConnectionRequest data in UE context */
    /* disable masks by default */
    p_ue_context->rcs_stmsi.bitmask = 0;

    /* Store ue_Identity value in UE Context */
    if ( (T_InitialUE_Identity_s_TMSI==p_rrcConnectionRequest_r8->ue_Identity.t)
         && (PNULL != p_rrcConnectionRequest_r8->ue_Identity.u.s_TMSI) )
    {
        p_ue_context->rcs_stmsi.bitmask=RRC_INITIAL_UE_IDENTITY_S_TMSI_PRESENT;

        /* Store mmec */
        RRC_ASSERT((p_rrcConnectionRequest_r8->ue_Identity.u.s_TMSI->mmec.numbits) <= 
            (8*sizeof(p_ue_context->rcs_stmsi.s_tmsi.mmec)));

        l3_memcpy_wrapper(p_ue_context->rcs_stmsi.s_tmsi.mmec,
            p_rrcConnectionRequest_r8->ue_Identity.u.s_TMSI->mmec.data,
            sizeof(p_ue_context->rcs_stmsi.s_tmsi.mmec) );

        /* Store m_TMSI*/
        RRC_ASSERT(8*(sizeof(p_ue_context->rcs_stmsi.s_tmsi.m_tmsi)) >=
            p_rrcConnectionRequest_r8->ue_Identity.u.s_TMSI->m_TMSI.numbits);

        l3_memcpy_wrapper(p_ue_context->rcs_stmsi.s_tmsi.m_tmsi,
            p_rrcConnectionRequest_r8->ue_Identity.u.s_TMSI->m_TMSI.data,
            sizeof(p_ue_context->rcs_stmsi.s_tmsi.m_tmsi) );
    }
    else if ((T_InitialUE_Identity_randomValue ==
              p_rrcConnectionRequest_r8->ue_Identity.t) &&
              (PNULL != p_rrcConnectionRequest_r8->ue_Identity.u.randomValue) )
    {
        p_ue_context->rcs_stmsi.bitmask =
            RRC_INITIAL_UE_IDENTITY_RANDOM_VALUE_PRESENT;

        /* Store random_value */
        RRC_ASSERT(sizeof(p_ue_context->rcs_stmsi.random_value) ==
            sizeof(p_rrcConnectionRequest_r8->ue_Identity.u.randomValue->data));

        l3_memcpy_wrapper(p_ue_context->rcs_stmsi.random_value,
            p_rrcConnectionRequest_r8->ue_Identity.u.randomValue->data,
            sizeof(p_ue_context->rcs_stmsi.random_value) );
    }
    else
    {
        RRC_UECC_UE_FSM_TRACE(RRC_ERROR,
            "rrcConnectionRequest: ue_Identity invalid field value.");
        result = RRC_FAILURE;
    }
    if (RRC_SUCCESS == result)
    {
        /* Store establishment cause in UE Context */
        p_ue_context->rcs_establishmentCause =
            p_rrcConnectionRequest_r8->establishmentCause;
	/*SPR 15896 Fix Start*/
	if (p_ue_context->m.traceActivated)
	{
		/*SPR 15875 Fix Start*/
		if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
				(p_ue_context->p_ue_trace_activation_info != PNULL) &&
				(p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & UU) &&
				((MEDIUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_trace_activation_info->trace_depth) ||
				 (MEDIUM == p_ue_context->p_ue_trace_activation_info->trace_depth) ||
				 (MINIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_trace_activation_info->trace_depth) ||
				 (MINIMUM == p_ue_context->p_ue_trace_activation_info->trace_depth)))
		{
			P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), GEN_MME,"\n <msg function=\"Uu\" name=\"RRC CONNECTION REQUEST\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					difftime_wrapper(p_ue_context->p_ue_trace_activation_info->rawtime));
		}
		if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
				(p_ue_context->p_ue_cell_trace_activation_info != PNULL) &&
				(p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & UU) &&
				((MEDIUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_cell_trace_activation_info->trace_depth) ||
				 (MEDIUM == p_ue_context->p_ue_cell_trace_activation_info->trace_depth) ||
				 (MINIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_cell_trace_activation_info->trace_depth) ||
				 (MINIMUM == p_ue_context->p_ue_cell_trace_activation_info->trace_depth)))
		{
			P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), GEN_CELL,"\n <msg function=\"Uu\" name=\"RRC CONNECTION REQUEST\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					difftime_wrapper(p_ue_context->p_ue_cell_trace_activation_info->rawtime));
		}
		P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), UU,"\n<ie name=\"Cause\">%d</ie>",p_rrcConnectionRequest_r8->establishmentCause);

		/*SPR 15875 Fix Stop*/
	}
	/*SPR 15896 Fix Stop*/

        /* Start UE connection setup control procedure */
        UECC_FSM_SET_MAIN_STATE(p_ue_context,
            &uecc_fsm_ue_connection_setup_ongoing, PNULL);

			/*SPR 15896 Fix Start*/
        P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask (),UU,"\n </msg>");
			/*SPR 15896 Fix Stop*/
    }
    else
    {
        p_ue_context->rcs_reject_cause = OTHER_FAILURE;
        /* Failure - reject connection */
        /* send UECC_LLIM_UE_CON_REJ_REQ(rrcConnectionReject) message to LLIM */
        uecc_llim_build_and_send_rrc_connection_reject(
            p_ue_context->p_gb_context,
            p_ue_context->crnti,
            UECC_LTE_RRC_CONNECTION_REJECT_WAIT_TIME_FIELD_DEFAULT_VALUE,
            p_ue_context->rcs_establishmentCause,
            p_ue_context->cell_index,
            p_ue_context->rcs_reject_cause,
            0,
            p_ue_context);/*CR 026 changes stop*/

        /* Free UE context */
        uecc_ue_ctx_free(p_ue_context->p_gb_context, p_ue_context->ue_index );
    }


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_connection_setup_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_ue_connection_setup_ongoing_init_handler(
    uecc_ue_context_t   *p_ue_context,  /* UECC UE context */
    void                *p_api          /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    uecc_fsm_start_procedure(
        p_ue_context, &uecc_fsm_ue_connection_setup_procedure, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrc_connection_setup_success_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_UE_CONNECTION_SETUP_SUCCESS_INT message
 *                  from RCS FSM in UECC_UE_CONNECTION_SETUP_ONGOING state
 ****************************************************************************/
void uecc_fsm_ue_rrc_connection_setup_success_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* update statistic */
    uecc_status_ue_update_num_of_srb1(p_ue_context);

    /* Switch state */
    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_wait_for_s1_connection, PNULL);

#ifdef MEM_PROFILE_ON

   RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
           p_ue_context->p_gb_context->facility_name,
           RRC_WARNING,"\n **************UE CONTEXT after RRC CONNECTION SETUP COMPLETE ");
    qvPoolStats();

#endif    

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrc_connection_setup_failure_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_UE_CONNECTION_SETUP_FAILURE_INT message
 *                  from RCS FSM in UECC_UE_CONNECTION_SETUP_ONGOING state
 ****************************************************************************/
void uecc_fsm_ue_rrc_connection_setup_failure_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
/* SPR 20636 Changes Start*/
    U16     failure_ind = (U16)((U32_64_Bit)p_api);
/* SPR 20636 Changes End*/
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (UECC_RCS_FAILURE_RELEASE_REQ == failure_ind)
    {
        /* SPR 18649 Fix Start */
        RRC_SET_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
        /* SPR 18649 Fix Stop */
        /* SRB1 is established - release connection */
        p_ue_context->share_data.uecc_rcr_data.u.cause.t =
            T_s1ap_Cause_radioNetwork;
        p_ue_context->share_data.uecc_rcr_data.u.cause.u.radioNetwork =
            s1ap_failure_in_radio_interface_procedure;

        UECC_FSM_SET_MAIN_STATE(
            p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

        uecc_fsm_start_procedure(
            p_ue_context,
            &uecc_fsm_ue_connection_release_setup_failure_procedure, PNULL);
    }
    else
    {
        /* Free UE context */
        uecc_ue_ctx_free(p_ue_context->p_gb_context, p_ue_context->ue_index );
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* ERAB SETUP Start */
/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_erab_setup_req_msg_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_SETUP_REQ message
 ****************************************************************************/

void uecc_fsm_ue_s1ap_erab_setup_req_msg_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_connection_reconfiguration_ongoing, PNULL);

    uecc_fsm_start_procedure(p_ue_context,
        &uecc_fsm_ue_erab_setup_procedure, p_api);


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);


}
/* ERAB SETUP Stop */
/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_initial_ctx_setup_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_INITIAL_CTX_SETUP_IND message
 ****************************************************************************/
void uecc_fsm_ue_s1ap_initial_ctx_setup_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_s1_connection_setup_ongoing, PNULL);
   
/* SPR:5566 start */
    if ((PNULL != p_ue_context->p_gb_context) && 
          (PNULL != g_rrc_stats.p_rrc_stats) &&
            ( RRC_NULL == p_ue_context->m.mme_ue_id_present ))
/* SPR:5566 stop */
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_ue_assoc_s1_conn_related_stats.
                s1sig_connEstabSucc, 0, DUMMY_RANGE_CHECK_INDEX);
    }
    if ((PNULL !=
         p_ue_context->timers_data.timers_ids[UECC_UE_MME_GUARD_TIMER]) &&
        (PNULL != p_ue_context->p_gb_context))
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_MME_GUARD_TIMER, &p_ue_context->timers_data);
    }            
    uecc_fsm_start_procedure(p_ue_context,
                    &uecc_fsm_ue_initial_context_setup_procedure, p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_ue_release_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_UE_RELEASE_REQ message
 ****************************************************************************/
void uecc_fsm_src_ho_ue_release_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    rrc_s1ap_ue_context_release_command_t 
        *p_ue_context_release_command = PNULL;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    p_ue_context_release_command = (rrc_s1ap_ue_context_release_command_t*)p_api;

    /* check if handover is ongoing then stop the procedure first */
    if(PNULL != p_ue_context->ho_info.p_ho_info)
    {
        /* stop the handover procedure ongoing */
        uecc_fsm_finish_active_procedure(p_ue_context);
    }
 
    if (PNULL != p_ue_context_release_command)
    {
        /* SPR 11316 Fix Start */ 
        l3_memcpy_wrapper(&p_ue_context->share_data.uecc_rcr_data.u.cause,
                    &p_ue_context_release_command->cause,
                    sizeof(s1ap_Cause));
        /* SPR 11316 Fix Stop */ 
    /* releasing the memory allocated to carry ctx release command */
        rrc_mem_free(p_ue_context_release_command);
    }
       
    
    UECC_FSM_SET_MAIN_STATE(
        p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(
        p_ue_context, &uecc_fsm_ue_connection_release_procedure, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_x2ap_ho_ue_ctx_release_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_SRC_X2AP_HO_CTX_REL_INT message
 ****************************************************************************/
void uecc_fsm_src_x2ap_ho_ue_ctx_release_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* check if handover is ongoing then stop the procedure first */
    if(PNULL != p_ue_context->ho_info.p_ho_info)
    {
        /* stop the handover procedure ongoing */
        uecc_fsm_finish_active_procedure(p_ue_context);
    }
       
    UECC_FSM_SET_MAIN_STATE(
        p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(
        p_ue_context, &uecc_fsm_ue_connection_release_procedure, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_release_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_UE_RELEASE_REQ message
 ****************************************************************************/
void uecc_fsm_ue_release_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    rrc_s1ap_ue_context_release_command_t 
        *p_ue_context_release_command = PNULL;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /* Coverity 110794 Fix Start */
    if(PNULL != p_api)
    {
        p_ue_context_release_command = (rrc_s1ap_ue_context_release_command_t*)p_api;
    }
    else
    {
        return;
    }
    /* Coverity 110794 Fix End */

    /* check if handover is ongoing then stop the procedure first */
    if(PNULL != p_ue_context->ho_info.p_ho_info)
    {
        /* stop the handover procedure ongoing */
        uecc_fsm_finish_active_procedure(p_ue_context);
    }
    p_ue_context->share_data.uecc_rcr_data.u.cause =
        p_ue_context_release_command->cause;
    if (PNULL !=
    p_ue_context->timers_data.timers_ids[UECC_UE_MME_GUARD_TIMER])
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_MME_GUARD_TIMER, &p_ue_context->timers_data);

    }
    /* SPR 20264 Fix Start */
    if(PNULL != p_api)
    {
        rrc_mem_free(p_api);
        p_api = PNULL; 
    }
    /* SPR 20264 Fix End */
    UECC_FSM_SET_MAIN_STATE(
        p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(
        p_ue_context, &uecc_fsm_ue_connection_release_procedure, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
* Function Name  : uecc_fsm_s1ap_error_ind_handler
* Inputs         : uecc_ue_context_t* - pointer to UE context
*                  void*   - api data
* Outputs        : None
* Returns        : None
* Description    : This action routine is called when UE Main FSM
*                  receives the UECC_EV_UE_ERROR_INDICATION message
****************************************************************************/
void uecc_fsm_s1ap_error_ind_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        )
{
    /* SPR 16750 Fix Start */
    /* Code Removed */
    /* SPR 16750 Fix Stop */
    rrc_s1ap_error_indication_t *p_error_ind = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* SPR 16750 Fix Start */
    /* Code Removed */
    /* SPR 16750 Fix Stop */

    p_error_ind = (rrc_s1ap_error_indication_t*)p_api;

    /* filling cause */
    if (RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT &
       p_error_ind->bitmask)
    {
        p_ue_context->share_data.uecc_rcr_data.u.cause =
           p_error_ind->cause;
    }
    /* SPR 16750 Fix Start */
    else
    {
        p_ue_context->share_data.uecc_rcr_data.u.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->share_data.uecc_rcr_data.u.cause.u.radioNetwork = s1ap_unspecified_2;
    }
   
    /* If radiolink failure is detected, no need to send rrcConnectionRelease */
    if ( (RRC_TRUE == p_ue_context->share_data.uecc_rcr_data.rlf_detected) ||
            ((PNULL != p_ue_context->ho_info.p_ho_info) && 
		(p_ue_context->ho_info.p_ho_info->p_src_ho_info) && 
            (RRC_TRUE == p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                        ho_command_sent_to_ue)) )
    {
        p_ue_context->share_data.uecc_rcr_data.
             rrc_connection_release_not_required_flag = RRC_TRUE;
    }

    if (p_error_ind->criticality_diagnostics.procedureCode ==
                    ASN1V_s1ap_id_UEContextReleaseRequest)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_MME_GUARD_TIMER, &p_ue_context->timers_data);
    }

    uecc_fsm_finish_active_procedure(p_ue_context);

    UECC_FSM_SET_MAIN_STATE(
            p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(
            p_ue_context, &uecc_fsm_ue_connection_release_procedure, PNULL);

    /* SPR 16750 Fix Stop */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
* Function Name  : uecc_fsm_x2ap_error_ind_handler 
* Inputs         : uecc_ue_context_t* - pointer to UE context
*                  void*   - api data
* Outputs        : None
* Returns        : None
* Description    : This action routine is called when UE Main FSM
*                  receives the UECC_EV_X2AP_ERROR_INDICATION message
****************************************************************************/
void uecc_fsm_x2ap_error_ind_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);     

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

#ifdef ENDC_ENABLED
    /*Error_Handling_changes*/
    /*check if ue has any scg bearer count*/
    if(RRC_ZERO < rrc_uecc_count_scg_bearer(p_ue_context))
    {
        /*triiger error ind for endc*/
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[uecc_fsm_x2ap_error_ind_handler] ENDC X2 Error-Indication");

         uecc_fsm_endc_x2ap_error_ind_handler(p_ue_context, p_api); 
    }
    else
    {
#endif
    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
             "[uecc_fsm_x2ap_error_ind_handler] X2 HO Error-Indication");
#ifdef ENDC_ENABLED
    }
    /*Error_Handling_changes*/
#endif

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* ERAB SETUP START */

/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_erab_setup_success_int_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_SETUP_SUCCESS_INT message
 ****************************************************************************/
void uecc_fsm_ue_s1ap_erab_setup_success_int_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* update statistic */
    /* UF: calculate number of successful DRB in future here */

    /* If HO Message is present in UE Ctx then push HO INT EVENT */ 
    /* coverity_fix_62099 start */    
    if ( PNULL != p_ue_context->p_curr_proc_data)
    {
        if ( p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
        {
            /* Send event to UE Main FSM */
            uecc_event_queue_push_event(
                    UECC_EV_HO_REQUIRED_INT,
                    p_ue_context,
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd); 

            /* Clear the HO Buffer SET TO PNULL */
            p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
    }
    /* coverity_fix_62099 stop */    
    /*valgrind_fix start*/
    if ( PNULL != p_ue_context->p_curr_proc_data)
    {
	    rrc_mem_free(p_ue_context->p_curr_proc_data);
	    p_ue_context->p_curr_proc_data = PNULL;

    }	
    /*valgrind_fix start*/


    /* Change UE Main FSM State to UE CONNECTED */
    UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);

#ifdef MEM_PROFILE_ON

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_INFO,"\n ***************Sizeof UE CONTEXT after ERAB SETUP is successful %d ",sizeof(uecc_ue_context_t));
    qvPoolStats();

#endif
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}       

/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_erab_setup_failure_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_SETUP_FAILURE_INT message
 ****************************************************************************/
void uecc_fsm_ue_s1ap_erab_setup_failure_int_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",     
            p_api);

    /* UF : update statistic */

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* If HO Message is present in UE Ctx then push HO INT EVENT */ 
        if ( p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
        {
            /* Send event to UE Main FSM */
            uecc_event_queue_push_event(
                    UECC_EV_HO_REQUIRED_INT,
                    p_ue_context,
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd); 

            p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
        else if ( p_ue_context->p_curr_proc_data->msg_bfr_type == 
                RRC_RE_ESTABLISH_MSG)
        {
            /* Send event to UE Main FSM */
            uecc_event_queue_push_event(
                    UECC_EV_RRC_RE_ESTABLISH_REQ_INT,
                    p_ue_context,
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd); 

            p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;

        }
        else if ( p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_RLF_MSG)
        {
            /* Send event to UE Main FSM */
            uecc_event_queue_push_event(
                    UECC_EV_LLIM_RADIO_LINK_FAILURE_IND,
                    p_ue_context,
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd); 

            p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
        /* Freed if any message is buffered */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }

        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    /* Change UE Main FSM State to UE CONNECTED */
    UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}  

/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_erab_setup_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_SETUP_PROC_TIMER_EXPIRY_INT message
 ****************************************************************************/
void uecc_fsm_ue_s1ap_erab_setup_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",     
            p_api);
    /* SPR 2510 Start */
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Freed if any message is buffered */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
                rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
                p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }

        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
        
    }

    /* CHANGE STATE TO WAIT FOR CONNECTION RELEASE */
    UECC_FSM_SET_MAIN_STATE(
            p_ue_context, &uecc_fsm_ue_wait_for_connection_release, PNULL);

    /* SPR 2510 Stop */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}  

/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_erab_setup_failure_rel_ue_ctx_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_SETUP_FAILURE_REL_UE_CTX_INT message
 ****************************************************************************/
void uecc_fsm_ue_s1ap_erab_setup_failure_rel_ue_ctx_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",     
            p_api);

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* If HO Message is present in UE Ctx then push HO INT EVENT */ 
        if ( p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
        {
            /* Send event to UE Main FSM */
            uecc_event_queue_push_event(
                    UECC_EV_HO_REQUIRED_INT,
                    p_ue_context,
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd); 

            p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
        else if ( p_ue_context->p_curr_proc_data->msg_bfr_type == 
                RRC_RE_ESTABLISH_MSG)
        {
            /* Send event to UE Main FSM */
            uecc_event_queue_push_event(
                    UECC_EV_RRC_RE_ESTABLISH_REQ_INT,
                    p_ue_context,
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd); 

            p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
        else if ( p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_RLF_MSG)
        {
            /* Send event to UE Main FSM */
            uecc_event_queue_push_event(
                    UECC_EV_LLIM_RADIO_LINK_FAILURE_IND,
                    p_ue_context,
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd); 

            p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
        /* Freed if any message is buffered */
        else
        {
            if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
            {
                rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
                p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
            }

            rrc_mem_free(p_ue_context->p_curr_proc_data);
            p_ue_context->p_curr_proc_data = PNULL;
        }
    }

    
    if ( PNULL == p_ue_context->p_curr_proc_data)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_FATAL,
                "p_ue_context->p_curr_proc_data is NULL"); 
        return;
    }
    /* If HO Message is present in UE Ctx then push HO INT EVENT */ 
    /* Change UE Main FSM State to UE CONNECTED */
    UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/* ERAB SETUP STOP */
/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_initial_ctx_setup_success_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_INITIAL_CTX_SETUP_SUCCESS_INT message
 ****************************************************************************/
void uecc_fsm_ue_s1ap_initial_ctx_setup_success_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    
    UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);
#ifdef MEM_PROFILE_ON

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,RRC_WARNING,
            "\n ***************Sizeof UE CONTEXT after ICS COMPLETE is = %d \n ",
            sizeof(uecc_ue_context_t));
    qvPoolStats();
#endif

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_initial_ctx_setup_failure_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_INITIAL_CTX_SETUP_FAILURE_INT message
 ****************************************************************************/
void uecc_fsm_ue_s1ap_initial_ctx_setup_failure_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    s1ap_Cause cause;
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    memset_wrapper(&cause, 0, sizeof(s1ap_Cause));
    cause.t= T_s1ap_Cause_radioNetwork;
    if(PNULL != p_ue_context->share_data.p_uecc_rm_data)
    {
        cause.u.radioNetwork = p_ue_context->share_data.p_uecc_rm_data->cause.u.radioNetwork;
    }

    /* END FIX SPR 1932 */

    if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                p_ue_context,
                &cause ))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[s1ap_UEContextReleaseRequest] Delivery failed");
    }                       

    /* Clear the share data in UE Context */
    if (p_ue_context->share_data.p_uecc_rm_data != PNULL)
    {
        rrc_mem_free(p_ue_context->share_data.p_uecc_rm_data);
        p_ue_context->share_data.p_uecc_rm_data = PNULL;
    }

    if ( RRC_SUCCESS != uecc_ue_timer_start(
            p_ue_context->p_gb_context,
            UECC_UE_MME_GUARD_TIMER,
            &p_ue_context->p_gb_context->ue_timers_data,
            &p_ue_context->timers_data,
            p_ue_context->ue_index))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[UECC_UE_MME_GUARD_TIMER]"
                "Timer Start Failed");
    }

    UECC_FSM_SET_MAIN_STATE(
            p_ue_context, &uecc_fsm_ue_wait_for_s1ap_ue_ctx_rel_cmd, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_initial_ctx_release_req_int_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_INITIAL_CTX_SETUP_RELEASE_REQ_INT message
 ****************************************************************************/
void uecc_fsm_ue_s1ap_initial_ctx_release_req_int_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(p_ue_context,
        &uecc_fsm_ue_connection_release_procedure, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_released_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_UE_RELEASED_INT message
 ****************************************************************************/
void uecc_fsm_ue_released_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    /* valgrind_fix start*/
    struct _uecc_gb_context_t*      p_gb_ctx = PNULL;
    /* valgrind_fix stop*/
    /* SPR 21412 Fix Start */
    /* SPR 21412 Fix End */
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* valgrind_fix start*/
    /* Save the gb_context in local pointer to pass in EXIT TRACE*/ 
    p_gb_ctx = p_ue_context->p_gb_context;
    /* valgrind_fix stop*/
    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:[%p][%p]",
            p_api,p_gb_ctx);
    /* decrement the count_use_ue_del_cell_req parameter by 1 after getting resp from LLIM.
     * This count will be used during UECC_CSC_DEL_ALL_UE_RESP */
    if(PNULL != p_ue_context->p_gb_context->p_p_csc_context)
    {
        if (p_ue_context->p_gb_context->
                p_p_csc_context[p_ue_context->cell_index] != PNULL)
        {
         /* SPR 22004 Fix Start */
            /* Code Deleted */
         /* SPR 22004 Fix End */
	   /* BUG 12722 Fix Start */
            if (p_ue_context->p_gb_context->reset_ongoing)
            {
                if(p_ue_context->p_gb_context->count_use_ue_reset_req > 0)
                    p_ue_context->p_gb_context->count_use_ue_reset_req --;

                if(p_ue_context->p_gb_context->count_use_ue_reset_req == 0)
                {
                    p_ue_context->p_gb_context->reset_ongoing = 0;
                }
            }
            /* SPR 21412 Fix Start */
            /* SPR 21412 Fix End */
	    /* SPR 22004 Fix Start */  
          	 if(p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                    del_cell_ongoing == 1)
 	    /* SPR 22004 Fix End */  
                /* BUG 12722 Fix Stop */
            {
                if(p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                        count_use_ue_del_cell_req > 0)
                    p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                        count_use_ue_del_cell_req --;

                if(p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                        count_use_ue_del_cell_req == 0)
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,RRC_INFO,
                            "%s sending CSC DEL ALL UE ENTITY RESP",__FUNCTION__);
                    p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                        del_cell_ongoing = 0;
                    uecc_csc_build_and_send_delete_all_ue_entity_resp(RRC_SUCCESS,
                            p_ue_context->p_gb_context,p_ue_context->cell_index);
                    if (RRC_TRUE == p_ue_context->p_gb_context->
                            p_p_csc_context[p_ue_context->cell_index]->is_cell_stop_ongoing)
                    {
                        p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                            is_cell_stop_ongoing = RRC_FALSE;
                    }
                    else
                    {
                        /* Clean Cell Context */
                        if (p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                                p_csc_init_setup_ind != PNULL)
                        {
                            rrc_mem_free (p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->
                                    cell_index]->p_csc_init_setup_ind);
                            p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                                p_csc_init_setup_ind = PNULL;
                        }
                        if (p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                                p_rrc_uecc_statistic != PNULL)
                        {
                            rrc_mem_free (p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->
                                    cell_index]->p_rrc_uecc_statistic);
                            p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                                p_rrc_uecc_statistic = PNULL;
                        }
                        if (p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                                p_cell_traffic_trace_info != PNULL)
                        {
                            rrc_mem_free (p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->
                                    cell_index]->p_cell_traffic_trace_info);
                            p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                                p_cell_traffic_trace_info = PNULL;
                        }

                        rrc_mem_free(p_ue_context->p_gb_context->
                                p_p_csc_context[p_ue_context->cell_index]);
                        p_ue_context->p_gb_context->
                            p_p_csc_context[p_ue_context->cell_index] = PNULL;
                    }
                }
            }
        }
    }

    RRC_UPDATE_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);

    if (p_ue_context->ue_ctx_del_status.is_s1ap_resp_expected == RRC_NULL)
    {
        /* Free UE context */
        uecc_ue_ctx_free(p_ue_context->p_gb_context, p_ue_context->ue_index );
    }
    /* valgrind_fix start*/
    RRC_UECC_UT_TRACE_EXIT(p_gb_ctx);
    /* valgrind_fix stop*/
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_del_ctx_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_DEL_UE_CONTEXT_IND message
 ****************************************************************************/
void uecc_fsm_ue_del_ctx_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    S1AP_UPDATE_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);

    if (p_ue_context->ue_ctx_del_status.is_rrc_resp_expected == RRC_NULL)
    {
        /* Free UE context */
        uecc_ue_ctx_free(p_ue_context->p_gb_context, p_ue_context->ue_index );
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}




/****************************************************************************
 * Function Name  : uecc_fsm_connection_setup_ue_released_ind
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_UE_RELEASED_INT message
 ****************************************************************************/
void uecc_fsm_connection_setup_ue_released_ind(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    uecc_fsm_finish_active_procedure(p_ue_context);

       UECC_FSM_SET_MAIN_STATE(
        p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(
        p_ue_context,
        &uecc_fsm_ue_connection_release_csc_cel_del_procedure, PNULL);


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_ul_nas_transport_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ message
 ****************************************************************************/
void uecc_fsm_ue_s1ap_ul_nas_transport_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    ULInformationTransfer_r8_IEs_dedicatedInfoType *p_dedicatedInfoType = 
	    (ULInformationTransfer_r8_IEs_dedicatedInfoType*)p_api;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    switch(p_dedicatedInfoType->t)
    {
	    case T_ULInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoNAS:
	    {
		    /* Pass NAS data to S1AP using RRC_S1AP_UL_NAS_TRANSPORT_REQ message */
    	   	if (RRC_SUCCESS != uecc_s1ap_build_and_send_ul_nas_transport_req(
            		p_ue_context, p_dedicatedInfoType->u.dedicatedInfoNAS ))
    		{
        	   	RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
            		"[s1ap_uplinkNASTransport] delivery fail");
    		}	
	}
	break;	
	case T_ULInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_1XRTT:
	{
	        if(RRC_SUCCESS != uecc_s1ap_build_and_send_uplink_s1_cdma2000_tunneling(
                        p_ue_context,  p_dedicatedInfoType->u.dedicatedInfoCDMA2000_1XRTT,
			T_ULInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_1XRTT))
		{

                        RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                        "CDMA2000:[s1ap_cdma2000Tunneling] delivery Fail");
                }
	}
	break;
	case T_ULInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_HRPD:
	{
		 if(RRC_SUCCESS != uecc_s1ap_build_and_send_uplink_s1_cdma2000_tunneling(
			p_ue_context, p_dedicatedInfoType->u.dedicatedInfoCDMA2000_HRPD,
			T_ULInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_HRPD))
		{
	
			RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
			"CDMA2000:[s1ap_cdma2000Tunneling] delivery Fail");
		}			
	} 
	break;
	default:
	   RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
               p_ue_context->p_gb_context->facility_name,
               RRC_WARNING,"CDMA2000:[s1ap_uplinkNASTransport] Invalid data value type");		
    }

#ifdef MEM_PROFILE_ON

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_INFO,"\n ***************Sizeof UE CONTEXT after UL NAS TRANSPORT REQUEST is successful %d ",sizeof(uecc_ue_context_t));
    qvPoolStats();

#endif
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  :uecc_fsm_ue_s1ap_dl_cdma2000_tunneling_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_DL_CDMA2000_TUNNELING message
 *****************************************************************************/
void uecc_fsm_ue_s1ap_dl_cdma2000_tunneling_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
   rrc_s1ap_downlink_s1_cdma2000_tunneling_t *p_downlink_s1_cdma2000_tunneling = PNULL;
   s1ap_Cause s1ap_cause;

   RRC_ASSERT(PNULL != p_ue_context);
   RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
   RRC_ASSERT(PNULL != p_api);
   RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
	
   p_downlink_s1_cdma2000_tunneling = 
	(rrc_s1ap_downlink_s1_cdma2000_tunneling_t*)p_api;
   if(s1ap_onexRTT == p_downlink_s1_cdma2000_tunneling->cdma2000_rat_type)
   {
       if(RRC_SUCCESS != uecc_llim_build_and_send_rrc_dl_information_transfer(
                                 p_ue_context,
                                 &p_downlink_s1_cdma2000_tunneling->cdma2000_pdu,
       T_DLInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_1XRTT))
       {
           s1ap_cause.t = T_s1ap_Cause_radioNetwork;
           s1ap_cause.u.radioNetwork = s1ap_unspecified_2;

           if (RRC_SUCCESS != uecc_s1ap_build_and_send_nas_non_delivery_ind(
                                 p_ue_context->p_gb_context,
                                 p_downlink_s1_cdma2000_tunneling->mme_ue_s1ap_id,
                                 p_downlink_s1_cdma2000_tunneling->enb_ue_s1ap_id,
                                 &p_downlink_s1_cdma2000_tunneling->cdma2000_pdu,
                                 &s1ap_cause))
           {
               RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                    "[s1ap_NASNonDeliveryIndication] delivery fail");
           }
       }
   }
   else if(s1ap_hRPD == p_downlink_s1_cdma2000_tunneling->cdma2000_rat_type)
   {
       if(RRC_SUCCESS != uecc_llim_build_and_send_rrc_dl_information_transfer(
                                p_ue_context,
                                &p_downlink_s1_cdma2000_tunneling->cdma2000_pdu,
       T_DLInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_HRPD))
       {
           s1ap_cause.t = T_s1ap_Cause_radioNetwork;
           s1ap_cause.u.radioNetwork = s1ap_unspecified_2;

           if (RRC_SUCCESS != uecc_s1ap_build_and_send_nas_non_delivery_ind(
                                 p_ue_context->p_gb_context,
                                 p_downlink_s1_cdma2000_tunneling->mme_ue_s1ap_id,
                                 p_downlink_s1_cdma2000_tunneling->enb_ue_s1ap_id,
                                 &p_downlink_s1_cdma2000_tunneling->cdma2000_pdu,
                                 &s1ap_cause))
           {
               RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                    "[s1ap_NASNonDeliveryIndication] delivery fail");
           }
       }
   }

    /* Bug 11814 Fix Start */    
#ifdef MEM_PROFILE_ON
    /* Bug 11814 Fix Stop */    

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_INFO,"\n ************* UE CONTEXT after DL S1 CDMA2000 TUNNELING is successful ");
    qvPoolStats();

#endif

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_DL_NAS_TRANSPORT_IND message
 ****************************************************************************/
void uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    rrc_s1ap_downlink_nas_transport_t *p_downlink_nas_transport = PNULL;
    /*REL 1.2:HANDOVER PROCEDURE START*/
    static rrc_rrm_ho_restriction_list_ind_t rrc_rrm_ho_restriction_list_ind;
    /*REL 1.2:HANDOVER PROCEDURE END*/
    rrc_rrm_dl_nas_transport_ind_t rrc_rrm_dl_nas_transport_ind;
    s1ap_Cause s1ap_cause;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /*REL 1.2:HANDOVER PROCEDURE START*/
    memset_wrapper(&rrc_rrm_ho_restriction_list_ind, RRC_NULL,
        sizeof(rrc_rrm_ho_restriction_list_ind_t));
    /*REL 1.2:HANDOVER PROCEDURE END*/

    memset_wrapper(&rrc_rrm_dl_nas_transport_ind, RRC_NULL,
        sizeof(rrc_rrm_dl_nas_transport_ind));

    p_downlink_nas_transport = (rrc_s1ap_downlink_nas_transport_t*)p_api;

    /*BUG 604 changes start*/
    if ((p_ue_context->uecc_fsm.proc_descriptor.p_current_state) && 
         (p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name) && 
         (p_ue_context->m.srb2_created == RRC_FALSE) &&
        ((!strcmp_wrapper(p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name,
                           (const S8 *)("UECC_CRE_W_FOR_REESTABLISH_COMPLETE"))))) /*CR 845 changes stop*/
    /*BUG 604 changes stop*/
    {
        s1ap_cause.t = T_s1ap_Cause_radioNetwork;
        s1ap_cause.u.radioNetwork = s1ap_unspecified_2;

        if (RRC_SUCCESS != uecc_s1ap_build_and_send_nas_non_delivery_ind(
                                p_ue_context->p_gb_context,
                                p_downlink_nas_transport->mme_ue_s1ap_id,
                                p_downlink_nas_transport->enb_ue_s1ap_id,
                                &p_downlink_nas_transport->nas_pdu,
                                &s1ap_cause))
        {
            RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                "[s1ap_NASNonDeliveryIndication] delivery fail as PDCP Reconfiguration is ongoing");
        }
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /*REL 1.2:HANDOVER PROCEDURE START*/
    /* Ho Restriction List */
    if (p_downlink_nas_transport->bitmask &
            RRC_S1AP_DOWNLINK_NAS_TRANSPORT_HANDOVER_RESTRICTION_LIST_PRESENT)  
    {
        /* Fill message */
        /*ue_index*/
        rrc_rrm_ho_restriction_list_ind.ue_index = p_ue_context->ue_index;
        /*ho_restriction_list*/

        /*SPR 20173 START*/   
        p_ue_context->m.ho_restriction_list_present = RRC_TRUE;
        asn1Copy_s1ap_HandoverRestrictionList(
            &p_ue_context->s1ap_asn1_ctx,
            &p_downlink_nas_transport->handover_restriction_list,
            &p_ue_context->ics_handover_restriction_list);
        /*SPR 20173 END*/   

        if (RRC_SUCCESS == process_fill_ho_restriction_list(
                    p_ue_context,
                    &rrc_rrm_ho_restriction_list_ind.ho_restriction_list,
                    &p_downlink_nas_transport->handover_restriction_list))
        {           
            if (RRC_FAILURE == rrc_rrm_il_send_rrc_rrm_ho_restriction_list_ind(
                        &rrc_rrm_ho_restriction_list_ind,
                        RRC_UECC_MODULE_ID,
                        RRC_RRM_MODULE_ID,
                        uecc_ue_ctx_get_op_transction_id(p_ue_context),
                        p_ue_context->cell_index))
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                        "rrc_rrm_il_send_rrc_rrm_ho_restriction_list_ind failed");
            }
        }
        else
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "process_fill_ho_restriction_list failed");
        }


        /* Overwrite plmn_identity in UE Context */
        memset_wrapper(&p_ue_context->plmn_identity.plmn_identity,
                RRC_NULL,
                sizeof(p_ue_context->plmn_identity.plmn_identity));
        uecc_s1ap_generate_plmn_identity_to_int(
                &p_ue_context->plmn_identity.plmn_identity,
                p_downlink_nas_transport->handover_restriction_list.servingPLMN.data);

    }
    /*REL 1.2:HANDOVER PROCEDURE END*/
    
    
    /* DownlinkNASTransport Ind Start  */
    if (p_downlink_nas_transport->bitmask &
            RRC_S1AP_DOWNLINK_NAS_TRANSPORT_SUBSCRIBER_PROFILE_ID_PRESENT)  
    {
        /* Fill message */
        /*ue_index*/
        rrc_rrm_dl_nas_transport_ind.bitmask |= RRC_RRM_DL_NAS_TRANSPORT_IND_SUBS_PROF_ID_PRESENT;
        rrc_rrm_dl_nas_transport_ind.ue_index = p_ue_context->ue_index;
        rrc_rrm_dl_nas_transport_ind.subscriber_profile_id_for_rfp = p_downlink_nas_transport->subs_prof_id;
        
        if (RRC_FAILURE == rrc_rrm_il_send_rrc_rrm_dl_nas_transport_ind(
                        &rrc_rrm_dl_nas_transport_ind,
                        RRC_UECC_MODULE_ID,
                        RRC_RRM_MODULE_ID,
                        uecc_ue_ctx_get_op_transction_id(p_ue_context),
                        p_ue_context->cell_index))
        {
             RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                        "rrc_rrm_il_send_rrc_rrm_dl_nas_transport_ind failed");
        }
    
    }
    /* DownlinkNASTransport Ind End  */

    /* Pass NAS data to UE through MAC using
     * UECC_LLIM_SRB_DATA_REQ(DLInformationTransfer) message */
    if (RRC_SUCCESS != uecc_llim_build_and_send_rrc_dl_information_transfer(
                            p_ue_context,
                            &p_downlink_nas_transport->nas_pdu,
	T_DLInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoNAS) )
    {
        s1ap_cause.t = T_s1ap_Cause_radioNetwork;
        s1ap_cause.u.radioNetwork = s1ap_unspecified_2;

        if (RRC_SUCCESS != uecc_s1ap_build_and_send_nas_non_delivery_ind(
                                p_ue_context->p_gb_context,
                                p_downlink_nas_transport->mme_ue_s1ap_id,
                                p_downlink_nas_transport->enb_ue_s1ap_id,
                                &p_downlink_nas_transport->nas_pdu,
                                &s1ap_cause))
        {
            RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                    "[s1ap_NASNonDeliveryIndication] delivery fail");
        }
    }
/* SPR 12324 Fix Start */
#ifdef MEM_PROFILE_ON
/* SPR 12324 Fix Stop */

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_INFO,"\n ************* UE CONTEXT after DL NAS TRANSPORT REQUEST is successful ");
    qvPoolStats();

#endif

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_NAS_NON_DELIEVERY_IND message
 ****************************************************************************/
void uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    rrc_uecc_llim_srb_data_status_ind_t *p_llim_srb_data_status_ind = PNULL;
    s1ap_Cause          s1ap_cause;
    s1ap_NAS_PDU        s1ap_nas_pdu;
    DedicatedInfoNAS*   p_nas_pdu = PNULL;
    OSCTXT              asn1_ctx;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_llim_srb_data_status_ind = (rrc_uecc_llim_srb_data_status_ind_t*)p_api;

    if ( RRC_SUCCESS != p_llim_srb_data_status_ind->response_code )
    {
        RRC_UECC_UE_FSM_TRACE(RRC_WARNING, "NAS DL message delivery fail.");

        s1ap_cause.t = T_s1ap_Cause_radioNetwork;
        s1ap_cause.u.radioNetwork = s1ap_unspecified_2;

        /* Init nas_pdu */
        asn1Init_s1ap_NAS_PDU(&s1ap_nas_pdu);

        if ( (RRC_UECC_LLIM_DATA_STATUS_ERROR_PRESENT
                & p_llim_srb_data_status_ind->bitmask)
             &&
             (0 < p_llim_srb_data_status_ind->
                     srb_data_status_error.data_buffer_length))
        {
            p_nas_pdu = uecc_llim_get_nas_pdu_from_srb_data_status_ind_msg(
                p_ue_context,
                &asn1_ctx,
                p_llim_srb_data_status_ind);

            /* nas_pdu */
            if (PNULL != p_nas_pdu)
            {
                /* We need to free ASN1 context in this case */
                s1ap_nas_pdu = *p_nas_pdu;
            }
        }

        /* Send RRC_S1AP_NAS_NON_DEL_IND request to S1AP */
        if (RRC_SUCCESS != uecc_s1ap_build_and_send_nas_non_delivery_ind(
                p_ue_context->p_gb_context,
                uecc_ue_ctx_get_mme_ue_id(p_ue_context),
                p_ue_context->s1ap_ue_id,
                &s1ap_nas_pdu,
                &s1ap_cause) )
        {
            RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                "[s1ap_NASNonDeliveryIndication] delivery fail");
        }

        if (PNULL != p_nas_pdu)
        {
            /* If NAS PDU was retrieved - Free ASN Context */
            rtFreeContext(&asn1_ctx);
        }
        else
        {
            RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                "[s1ap_NASNonDeliveryIndication] NAS PDU has not been found");
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrc_measurement_report_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRC_MEASUREMENT_REPORT_IND message
 ****************************************************************************/
void uecc_fsm_ue_rrc_measurement_report_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    uecc_rrm_build_and_send_measurment_results_ind(p_ue_context, p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrc_proximity_indication_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRC_PROXIMITY_INDICATION_IND message
 ****************************************************************************/
void uecc_fsm_ue_rrc_proximity_indication_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    uecc_rrm_build_and_send_proximity_indication_ind(p_ue_context, p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_reset_msg_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  S1AP_RESET_MSG_IND message
 *                  in uecc_fsm_ue_wait_for_s1_connection state
 ****************************************************************************/
void uecc_fsm_ue_reset_msg_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Start UE connection release procedure
     * without interaction with S1AP  */
    p_ue_context->share_data.uecc_rcr_data.u.cause.t =
        T_s1ap_Cause_transport;
    p_ue_context->share_data.uecc_rcr_data.u.cause.u.radioNetwork =
        s1ap_transport_resource_unavailable;

    /* SPR 12310 Fix Start */
    RRC_SET_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
    /* SPR 12310 Fix Stop */

    UECC_FSM_SET_MAIN_STATE(
        p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(
        p_ue_context,
        &uecc_fsm_ue_connection_release_setup_failure_procedure, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* SPR 13560 Fix Start*/
/****************************************************************************
 * Function Name  : uecc_fsm_ue_error_indication_message_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  ERROR INDICATION MSG 
 *                  in uecc_fsm_ue_wait_for_s1_connection state
 ****************************************************************************/
void uecc_fsm_ue_error_indication_message_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    rrc_s1ap_error_indication_t *p_s1ap_error_ind = PNULL;
 
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_s1ap_error_ind = (rrc_s1ap_error_indication_t *)p_api;

    /* Start UE connection release procedure
     * without interaction with S1AP  */
    if (RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT & p_s1ap_error_ind->bitmask)
    {
        l3_memcpy_wrapper (&p_ue_context->share_data.uecc_rcr_data.u.cause,
                      &p_s1ap_error_ind->cause,
                      sizeof (s1ap_Cause));
    }
    else
    {
        p_ue_context->share_data.uecc_rcr_data.u.cause.t =
             T_s1ap_Cause_radioNetwork;
        p_ue_context->share_data.uecc_rcr_data.u.cause.u.radioNetwork =
             s1ap_unspecified_2;
    }

    if (RRC_SUCCESS != uecc_s1ap_build_and_send_ue_release_msg_req(
			    p_ue_context->s1ap_ue_id,
			    RRC_FALSE,
			    p_ue_context))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
	   p_ue_context->p_gb_context->facility_name,RRC_WARNING,
	   "ue_release_msg delivery failure");
    }

    UECC_FSM_SET_MAIN_STATE(
        p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(
        p_ue_context,
        &uecc_fsm_ue_connection_release_setup_failure_procedure, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR 13560 Fix Stop*/

/****************************************************************************
 * Function Name  : uecc_fsm_ue_ics_ongoing_rrm_release_req_int_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_INITIAL_CTX_SETUP_RRM_RELEASE_REQ_INT message
 *                  during ICS
 ****************************************************************************/
void uecc_fsm_ue_ics_ongoing_rrm_release_req_int_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    UECC_FSM_SET_MAIN_STATE(
            p_ue_context,
            &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(
            p_ue_context,
            &uecc_fsm_ue_rrm_connection_release_procedure, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1u_error_ind_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_LLIM_S1U_ERROR_IND message
 *                  in uecc_fsm_ue_connected state.
 *                  It calls the another function to build and send 
 *                  local error indication to RRM.
 ****************************************************************************/
void uecc_fsm_ue_s1u_error_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    if (RRC_SUCCESS != uecc_rrm_build_and_send_rrc_rrm_local_error_ind_for_s1u_error(
                       p_ue_context, p_api))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[RRC_RRM_LOCAL_ERROR_IND] Delivery failed");
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR_17382_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1u_error_ind_at_trg_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_LLIM_S1U_ERROR_IND message
 *                  in UECC_LLIM_S1U_ERROR_IND state.
 *                  It ignores the received S1U_ERROR_IND.
 ****************************************************************************/
void uecc_fsm_ue_s1u_error_ind_at_trg_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    rrc_uecc_llim_s1u_error_ind_t *p_s1u_error_ind = PNULL;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    p_s1u_error_ind = p_api;
    /*
     * If there is an issue with the tunnel and if it persists even
     * after Path Switch ACK (for X2), S1U ERROR Ind will be received
     * even after completion of HO and then it will be forwarded to
     * RRM for the required action.
     */
    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "S1U ERROR IND received at TeNB for "
                "UE[%u] and LcId[%u] is ignored.", 
                p_s1u_error_ind->ue_index, p_s1u_error_ind->lc_id);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR_17382_END*/

/****************************************************************************
 * Function Name  : uecc_fsm_ue_csc_del_msg_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_CSC_DELETE_ALL_UE_REQ  message
 *                  in uecc_fsm_ue_connected_state state
 ****************************************************************************/
void uecc_fsm_ue_csc_del_msg_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    UECC_FSM_SET_MAIN_STATE(
        p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(
        p_ue_context,
        &uecc_fsm_ue_connection_release_csc_cel_del_procedure, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* SPR 20704 Fix Start */
/****************************************************************************
 * Function Name  : uecc_fsm_ue_csc_del_release_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_CSC_DELETE_ALL_UE_REQ  message
 *                  in uecc_fsm_ue_connected_state state
 ****************************************************************************/
void uecc_fsm_ue_csc_del_release_handler(
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%x",
			p_api);
	RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"No Procedure has been initiated for UE:%d Hence freeing the UE context ",
			p_ue_context->ue_index);
	/* Free UE context */
	uecc_ue_ctx_free(p_ue_context->p_gb_context, p_ue_context->ue_index );

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* SPR 20704 Fix End */

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rlf_indication_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RADIO_LINK_FAILURE_IND_INT  message
 *                  in uecc_fsm_ue_connection_reconfiguartion_ongoing
 ****************************************************************************/
void uecc_fsm_ue_rlf_indication_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    UECC_FSM_SET_MAIN_STATE(
        p_ue_context, &uecc_fsm_ue_connected, PNULL);

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
            UECC_EV_RADIO_LINK_FAILURE_INT,
            p_ue_context,
            p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_capability_enquiry_rrm_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRM_UE_CAPABILITY_ENQUIRY_REQ message
 *                  in uecc_fsm_ue_connected state
 ****************************************************************************/

void uecc_fsm_ue_capability_enquiry_rrm_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    uecc_fsm_start_procedure(p_ue_context,
        &uecc_fsm_ue_capability_rrm_enq_procedure, p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR_17893_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_connected_ue_capability_enquiry_rrm_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRM_UE_CAPABILITY_ENQUIRY_REQ message
 *                  in uecc_fsm_ue_connected state
 ****************************************************************************/

void uecc_fsm_connected_ue_capability_enquiry_rrm_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    uecc_rrm_ue_capability_enq_handler(p_ue_context, p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR_17893_END*/


/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_ue_connection_release_ind_int_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND_INT event  in 
 *                  ue connection reconfiguration ongoing state
 ****************************************************************************/
void uecc_fsm_ue_rrm_ue_connection_release_ind_int_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_connected, PNULL);

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_RRM_UE_CONNECTION_RELEASE_IND,
        p_ue_context,
        p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR_17062_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_conn_rel_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  RRC_RRM_UE_CONNECTION_RELEASE_IND message while waiting
 *                  for UE Context Release Command
 ****************************************************************************/
void uecc_fsm_rrm_ue_conn_rel_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
    )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);         

    p_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag
        = RRC_TRUE;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR_17062_END*/

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_ue_connection_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  RRC_RRM_UE_CONNECTION_RELEASE_IND message in stable states
 ****************************************************************************/
void uecc_fsm_ue_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
    )
{
    s1ap_Cause cause;
    rrc_rrm_ue_connection_release_ind_t *p_ue_connection_release_ind=
        (rrc_rrm_ue_connection_release_ind_t *)p_api;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (RRC_SUCCESS == uecc_s1ap_build_s1ap_cause(
                &cause,&p_ue_connection_release_ind->release_cause,
                p_ue_context))
    {
        if (RRC_RRM_UE_CONNECTION_RELEASE_IND_REDIRECTED_CARRIER_INFO_PRESENT &
                p_ue_connection_release_ind->bitmask)
        {
            /* store in UE context */
            p_ue_context->p_redirected_carrier_info = PNULL;

            p_ue_context->p_redirected_carrier_info =
                (rrm_redirected_carrier_info_t*) 
                rrc_mem_get(sizeof(rrm_redirected_carrier_info_t));
            if (p_ue_context->p_redirected_carrier_info == PNULL)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                        "Memory Allocation Failure!!");
                return;
            }


            l3_memcpy_wrapper(p_ue_context->p_redirected_carrier_info,
                    &p_ue_connection_release_ind->redirected_carrier_info,
                    sizeof(rrm_redirected_carrier_info_t));
            p_ue_context->m.redirected_carrier_info_present = 1;
        }

        if (
                RRC_RRM_UE_CONNECTION_RELEASE_IND_IDLE_MODE_MOBILITY_CONTROL_PRESENT 
                & p_ue_connection_release_ind->bitmask)
        {
            /* store in UE context */
            p_ue_context->p_idle_mode_mobility_control = PNULL;
            p_ue_context->p_idle_mode_mobility_control =
                (rrm_idle_mode_mobility_control_info_t*)
                rrc_mem_get(sizeof(rrm_idle_mode_mobility_control_info_t));
            if (p_ue_context->p_idle_mode_mobility_control == PNULL)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                        "Memory Allocation Failure!!");
                return;
            }

            l3_memcpy_wrapper(p_ue_context->p_idle_mode_mobility_control,
                    &p_ue_connection_release_ind->idle_mode_mobility_control,
                    sizeof(rrm_idle_mode_mobility_control_info_t));

            p_ue_context->m.idle_mode_mobility_control_present = 1;
        }

        if (RRC_RRM_UE_CONNECTION_RELEASE_IND_RRC_CONN_REL_V890_IES_PRESENT &
                p_ue_connection_release_ind->bitmask)
        {
            /* store in UE context */
            p_ue_context->rrc_connection_release_v890_ies =
                p_ue_connection_release_ind->rrc_connection_release_v890_ies;
            p_ue_context->m.connection_release_v890_ies = 1;
        }

        if ( 0 == p_ue_context->m.mme_ue_id_present )
        {
            p_ue_context->share_data.uecc_rcr_data.u.cause = cause;

            UECC_FSM_SET_MAIN_STATE(
                    p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

            /* release with no_mme_id procedure */
            uecc_fsm_start_procedure(
                    p_ue_context,
                    &uecc_fsm_ue_rrm_connection_release_no_mme_id_procedure,
                    PNULL);
        }
        else
        {
            if(
                    (
                     (RRC_RRM_UE_CONNECTION_RELEASE_IND_RRC_CONN_REL_V890_IES_PRESENT 
                      & p_ue_connection_release_ind->bitmask) && 
                     (RRC_RRM_UE_CONNECTION_RELEASE_IND_RRC_CONN_REL_V920_IES_PRESENT 
                      & p_ue_connection_release_ind->rrc_connection_release_v890_ies.bitmask) 
                     && (RRC_RRM_CELL_INFO_LIST_R9_PRESENT 
                         & p_ue_connection_release_ind->rrc_connection_release_v890_ies.
                         rrc_connection_release_v920_ies.bitmask)
                    ) && 
                    (!(RRC_RRM_UE_CONNECTION_RELEASE_IND_REDIRECTED_CARRIER_INFO_PRESENT & 
                       p_ue_connection_release_ind->bitmask))
              )
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                        "[RRC_RRM_UE_CONNECTION_RELEASE_IND] "
                        "RRC Connection Release cellInfoList-r9 is received without "
                        "Redirect Carrier Info, ignoring message ");
            }
            else
            {
                do {
                    /* CR 713 changes start*/

                    if ((p_ue_connection_release_ind->bitmask &
                                RRC_RRM_UE_CONNECTION_RELEASE_IND_REDIRECTED_CARRIER_INFO_PRESENT) &&
                            (p_ue_connection_release_ind->redirected_carrier_info.bitmask &
                             RRM_REDIRECTED_CARRIER_INFO_UTRA_TDD_R10_PRESENT) &&
                            (p_ue_connection_release_ind->bitmask &
                             RRC_RRM_UE_CONNECTION_RELEASE_IND_RRC_CONN_REL_V890_IES_PRESENT) &&
                            (p_ue_connection_release_ind->rrc_connection_release_v890_ies.bitmask &
                             RRC_RRM_UE_CONNECTION_RELEASE_IND_RRC_CONN_REL_V920_IES_PRESENT) &&
                            (p_ue_connection_release_ind->rrc_connection_release_v890_ies.
                             rrc_connection_release_v920_ies.bitmask & 
                             RRC_RRM_CELL_INFO_LIST_R9_PRESENT) &&
                            (!(p_ue_connection_release_ind->rrc_connection_release_v890_ies.
                               rrc_connection_release_v920_ies.cell_info_list_r9.bitmask &
                               RRC_RRM_CELL_INFO_LIST_UTRA_TDD_R10_PRESENT)))
                    {
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                                " CellInfoList-r9 is not set to 'utra-TDD-r10'"
                                " RedirectedCarrierInfo cannot be set to 'utra-TDD -r10' - Ignoring Message");

                        break;
                    }
                    /* CR 713 changes stop*/
                    /*bug 8340 fix start*/
                    
                    if(RRC_RRM_UE_CONNECTION_RELEASE_IND_REDIRECTED_CARRIER_INFO_PRESENT  &
                             p_ue_connection_release_ind->bitmask)
                    /*bug 8340 fix stop*/
                    {
                        p_ue_context->share_data.uecc_rcr_data.u.cause = cause; 

                        /* RRM FIX Start */
                        p_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag
                            = RRC_TRUE;
                        /* RRM FIX Stop */ 
                        UECC_FSM_SET_MAIN_STATE(
                                p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);
                        uecc_fsm_start_procedure(
                                p_ue_context,
                                &uecc_fsm_ue_rrm_connection_release_procedure,
                                PNULL);
                        if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                                    p_ue_context,
                                    &cause))
                        {
                            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                    p_ue_context->p_gb_context->facility_name,
                                    RRC_WARNING,"[s1ap_UEContextReleaseRequest] Delivery failed");
                        }                       
                    }
                    /* SPR:5229 start */
                    else
                    {
                        p_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag
                            = RRC_TRUE;
                        /* SPR:5229 stop */

                        /* BUG 12722 Fix Start */
                        p_ue_context->share_data.uecc_rcr_data.reset_towards_mme_required =
                           RRC_TRUE;
                        /* BUG 12722 Fix Stop */

                        /*SPR:5645 start*/
                        if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                                p_ue_context,
                                &cause))
                        {
                            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                    p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                                    "uecc_s1ap_build_and_send_enb_ue_ctx_release_req failed.");
                        }

                        if ( RRC_SUCCESS !=
                                uecc_ue_timer_start(
                                    p_ue_context->p_gb_context,
                                    UECC_UE_MME_GUARD_TIMER,
                                    &p_ue_context->p_gb_context->ue_timers_data,
                                    &p_ue_context->timers_data,
                                    p_ue_context->ue_index)
                           )
                        {
                            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                                    "[UECC_UE_MME_GUARD_TIMER]"
                                    "Timer Start Failed");
                        }

                        UECC_FSM_SET_MAIN_STATE(
                                p_ue_context,
                                &uecc_fsm_ue_wait_for_s1ap_ue_ctx_rel_cmd,
                                PNULL);
                        /*SPR:5645
                         * end*/
                    }
                }while(0);
                /* CR 490 Changes End*/
            }

        }


    }
    else
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[RRC_RRM_UE_CONNECTION_RELEASE_IND] "
                "invalid cause, ignoring message");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*Pranab */

/****************************************************************************
 * Function Name  : uecc_fsm_ue_ics_radio_link_failure_int_handler
 *
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_LLIM_RADIO_LINK_FAILURE_IND  message
 *                  in UECC_UE_WAIT_FOR_S1_CONNECTION and UECC_UE_S1_CONNECTION_
 *                  SETUP_ONGOING states.
 ****************************************************************************/
void uecc_fsm_ue_ics_radio_link_failure_int_handler (
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    s1ap_Cause cause;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    cause.t = T_s1ap_Cause_radioNetwork ;
    cause.u.radioNetwork= s1ap_radio_connection_with_ue_lost ;
    /*This flag indicates that RRC connection Release will not be
     Sent to UE */

    p_ue_context->
    share_data.uecc_rcr_data.rrc_connection_release_not_required_flag = 
    RRC_TRUE ;

    p_ue_context->share_data.uecc_rcr_data.rlf_detected = RRC_TRUE;

    if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                            p_ue_context,
                            &cause ) )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[s1ap_UEContextReleaseRequest] Delivery failed");
    }
    UECC_FSM_SET_MAIN_STATE(
        p_ue_context, &uecc_fsm_ue_wait_for_connection_release, PNULL);
    
  RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);  
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_ics_reestablish_req_int_handler
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ  message
 *                  in UECC_UE_S1_CONNECTION_SETUP_ONGOING state.
 ****************************************************************************/
void uecc_fsm_ue_ics_reestablish_req_int_handler( 
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    s1ap_Cause cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    cause.t = T_s1ap_Cause_radioNetwork ;
    cause.u.radioNetwork= s1ap_interaction_with_other_procedure;

    /*This flag indicates that RRC connection Release will not be
     Sent to UE */

    p_ue_context->
    share_data.uecc_rcr_data.rrc_connection_release_not_required_flag =
    RRC_TRUE ;
    
    if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                            p_ue_context,
                            &cause ) )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[s1ap_UEContextReleaseRequest] Delivery failed");
    }

    UECC_FSM_SET_MAIN_STATE(
        p_ue_context, &uecc_fsm_ue_wait_for_connection_release, PNULL);

  RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_connected_radio_link_failure_ind_handler
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_LLIM_RADIO_LINK_FAILURE_IND  message
 *                  in UECC_UE_CONNECTED states.
 ****************************************************************************/
void uecc_fsm_ue_connected_radio_link_failure_ind_handler (
 uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*This flag indicates that RRC connection Release will not be
     Sent to UE */
    p_ue_context->
    share_data.uecc_rcr_data.rrc_connection_release_not_required_flag =
    RRC_TRUE ;

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,RRC_INFO,
            "Inside the Handler for Radio Link failure"
            " in UE Connected state ");

#ifdef ENDC_ENABLED
    /*EN-DC_changes_start*/
    uecc_fsm_x2ap_ue_context_release_handler(p_ue_context);
    /*EN-DC_changes_end*/
#endif

    /* SPR 2416 Start */
    UECC_FSM_SET_MAIN_STATE(
            p_ue_context,
            &uecc_fsm_ue_radio_link_failure_handling_ongoing,
            PNULL);
    /* SPR 2416 Stop */


    uecc_fsm_start_procedure(
                  p_ue_context,
                  &uecc_fsm_ue_radio_link_failure_handling_procedure ,
                  p_api);
    
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Freed if any message is buffered */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {       
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }       
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_mme_guard_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  in the RADIO LINK Failure handling state recieve the timer 
 *                  expiry message.
 *                  in uecc_fsm_ue_connected_state state
 ****************************************************************************/
void uecc_fsm_mme_guard_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    UECC_FSM_SET_MAIN_STATE(
        p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);
  
    /*Adding flags for Initiating the MME RESET and Suppression of
    RRC Connection Release */
    p_ue_context->
    share_data.uecc_rcr_data.reset_towards_mme_required = RRC_TRUE ; 
    /* release with no_mme_id procedure */
   
    uecc_fsm_finish_active_procedure(p_ue_context);   
    uecc_fsm_start_procedure(
            p_ue_context,
            &uecc_fsm_ue_rrm_connection_release_no_mme_id_procedure,
            PNULL);
   RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_rlf_handling_success_int_handler.
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                   UECC_EV_RLF_HANDLING_SUCCESS_INT from LLIM.
 *
 ****************************************************************************/
void uecc_fsm_rlf_handling_success_int_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);


    uecc_fsm_finish_active_procedure(p_ue_context);

    if ((PNULL != p_ue_context->p_curr_proc_data) 
            && (PNULL != 
        p_ue_context->p_curr_proc_data->msg_bfr_rcvd) &&
        (p_ue_context->p_curr_proc_data->msg_bfr_type == 
        RRC_RE_ESTABLISH_MSG))
    {
        uecc_event_queue_push_event(
                UECC_EV_RRC_RE_ESTABLISH_REQ_INT,
                p_ue_context,
                p_ue_context->p_curr_proc_data->msg_bfr_rcvd);

        p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
        p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_INFO,
                "[RLF]: RE-EST MSG PUSHED INTO UE Main FSM");
    }
    else
    {

/*Not starting timer for testing purpose ,mustbe opened later */
    /* Start timer*/
    if ( RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_RLF_W_FOR_REEST_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        (rrc_ue_index_t)p_ue_context->ue_index))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[UECC_RLF_W_FOR_REEST_TIMER]"
                "Timer Start Failed");
    }

    UECC_FSM_SET_MAIN_STATE(
        p_ue_context,
        &uecc_fsm_ue_radio_link_failure_handling_ongoing,
        PNULL);
    }

    /* FREE p_curr_proc_data */
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_rlf_handling_failure_init_handler.
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RLF_HANDLING_FAILURE_INT  from LLIM.
 *
 ****************************************************************************/
void uecc_fsm_rlf_handling_failure_init_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{   
    s1ap_Cause cause;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    /* Send Re-establishment Reject */

    if ((PNULL != p_ue_context->p_curr_proc_data) 
            && (PNULL != 
        p_ue_context->p_curr_proc_data->msg_bfr_rcvd) &&
        (p_ue_context->p_curr_proc_data->msg_bfr_type == 
        RRC_RE_ESTABLISH_MSG))
    {
        if (RRC_SUCCESS !=
                uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                    p_ue_context->p_gb_context, p_ue_context->crnti, 0,
                    p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_ERROR,
                    "Fail to send the RRCConnectionReestablishmentReject to UE");
        }
        else
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                    p_ue_context->p_gb_context->facility_name,RRC_INFO,
                    "[RLF]: RE-EST REJECT MSG SEND SUCCESSFULLY");
        }

    }


    /* FREE p_curr_proc_data */
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }


    cause.t = T_s1ap_Cause_radioNetwork ;
    cause.u.radioNetwork= s1ap_radio_connection_with_ue_lost ;
    if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                            p_ue_context,
                            &cause ) )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[s1ap_UEContextReleaseRequest] Delivery failed");
    }

    uecc_fsm_finish_active_procedure(p_ue_context);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_wait_for_connection_release, PNULL);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_UE_CONTEXT_MOD_REQ message in stable states
 ****************************************************************************/
void uecc_fsm_ue_context_mod_req_handler(
        uecc_ue_context_t   *p_ue_context,  /* UECC UE context */
        void                *p_api          /* Input API buffer */
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*SPR_18241_START*/
    if(PNULL != p_ue_context->p_temp_ue_ctx_mod_proc_data)
    {
        RRC_UECC_UE_FSM_TRACE(RRC_ERROR,
                "UE Context Modification proceduce already ongoing. "
                "Ignoring new UE Context Modification Request for UE:%u",
                p_ue_context->ue_index);
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    /*SPR_18241_END*/

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
            &uecc_fsm_ue_context_mod_ongoing, PNULL);

    uecc_fsm_start_procedure(
        p_ue_context, &uecc_fsm_ue_context_mod_procedure, p_api);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_w_for_reest_timer_expiry_handler
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_T2_TIMER_EXPIRY message.
 ****************************************************************************/
void uecc_fsm_w_for_reest_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    s1ap_Cause cause;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    p_ue_context->
    share_data.uecc_rcr_data.rrc_connection_release_not_required_flag =
    RRC_TRUE ;

    cause.t = T_s1ap_Cause_radioNetwork ;
    cause.u.radioNetwork= s1ap_radio_connection_with_ue_lost ;
    if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                            p_ue_context,
                            &cause ) )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[s1ap_UEContextReleaseRequest] Delivery failed");
    }

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_wait_for_connection_release, PNULL);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_w_s1_con_radio_link_failure_ind_handler
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called whenin wait for 
 *                  S1connection states  FSM receives the
 *                  UECC_UE_RADIO_LINK_FAILURE_IND message.
 ****************************************************************************/
void uecc_fsm_ue_w_s1_con_radio_link_failure_ind_handler (
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);
    /* The flags are for indication the RRC connection release
       message will not be sent to UE and RESET message will be
       sent to MME instead of the UE Context release complete */

    p_ue_context->
    share_data.uecc_rcr_data.reset_towards_mme_required = RRC_TRUE ;
    /*SPR 15610 Fix Start*/
    p_ue_context->share_data.uecc_rcr_data.release_cause_valid = RRC_TRUE ;
    p_ue_context->share_data.uecc_rcr_data.u.cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->share_data.uecc_rcr_data.u.cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost; 
    /*SPR 15610 Fix Stop*/

    p_ue_context->
    share_data.uecc_rcr_data.rrc_connection_release_not_required_flag =
    RRC_TRUE ;

    UECC_FSM_SET_MAIN_STATE(
        p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(
        p_ue_context, &uecc_fsm_ue_connection_release_procedure, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);


}  

/****************************************************************************
 * Function Name  : uecc_fsm_ue_connection_release_ongoing_init_handler
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called to start the MME 
 *                  guard timer.
 ****************************************************************************/
void uecc_fsm_ue_connection_release_ongoing_init_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    if (RRC_SUCCESS != uecc_ue_timer_start(
            p_ue_context->p_gb_context,
            UECC_UE_MME_GUARD_TIMER,
            &p_ue_context->p_gb_context->ue_timers_data,
            &p_ue_context->timers_data,
            p_ue_context->ue_index))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "Failure to start MME Guard Timer.");

    }
   
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_wait_for_conn_release_ue_release_req_handler
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when wait 
 *                  for connection release FSM receives the
 *                  UECC_EV_UE_RELEASE_REQ message
 ****************************************************************************/
void uecc_fsm_wait_for_conn_release_ue_release_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    rrc_s1ap_ue_context_release_command_t *p_ue_context_release_command =
        (rrc_s1ap_ue_context_release_command_t*)p_api;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    uecc_ue_timer_stop(p_ue_context->p_gb_context,
            UECC_UE_MME_GUARD_TIMER, &p_ue_context->timers_data);
    /* Store data in UE context for future processing */
    p_ue_context->share_data.uecc_rcr_data.u.cause =
        p_ue_context_release_command->cause;

   uecc_fsm_ue_release_req_handler(p_ue_context,p_api);
   
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_w_for_ue_conn_rel_reset_msg_ind_handler
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  S1AP_RESET_MSG_IND message
 *                  in uecc_fsm_ue_wait_for_s1_connection state
 ****************************************************************************/
void uecc_fsm_ue_w_for_ue_conn_rel_reset_msg_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Start UE connection release procedure
     * without interaction with S1AP  */
    p_ue_context->share_data.uecc_rcr_data.u.cause.t =
        T_s1ap_Cause_transport;
    p_ue_context->share_data.uecc_rcr_data.u.cause.u.radioNetwork =
        s1ap_transport_resource_unavailable;

    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_MME_GUARD_TIMER, &p_ue_context->timers_data);
    
    uecc_fsm_ue_reset_msg_ind_handler(p_ue_context,p_api);
  
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_w_for_conn_rel_csc_del_msg_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_CSC_DELETE_ALL_UE_REQ  message
 *                  in uecc_fsm_ue_connected_state state
 ****************************************************************************/
void uecc_fsm_ue_w_for_conn_rel_csc_del_msg_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_MME_GUARD_TIMER, &p_ue_context->timers_data);
     
    uecc_fsm_ue_csc_del_msg_ind_handler(p_ue_context,p_api); 
      
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_state_rlf_ue_release_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_UE_RELEASE_REQ message
 ****************************************************************************/
void uecc_fsm_state_rlf_ue_release_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
 
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                      UECC_RLF_W_FOR_REEST_TIMER,
                      &p_ue_context->timers_data);
   
   uecc_fsm_ue_release_req_handler(p_ue_context,p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rlf_csc_del_msg_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_CSC_DELETE_ALL_UE_REQ  message
 *                  in uecc_fsm_ue_connected_state state
 ****************************************************************************/
void uecc_fsm_ue_rlf_csc_del_msg_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
  
   uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                      UECC_RLF_W_FOR_REEST_TIMER,
                      &p_ue_context->timers_data);

    
    uecc_fsm_ue_csc_del_msg_ind_handler(p_ue_context,p_api);
    


    /* Send event to UE Main FSM */


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rlf_rrm_erab_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRM_ERAB_RELEASE_IND   message
 ****************************************************************************/
void uecc_fsm_ue_rlf_rrm_erab_release_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);


    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,RRC_INFO,
            "[UECC_EV_RRM_ERAB_RELEASE_IND] Discarding "
            "the Request ");

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);


}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rlf_rrc_reestablish_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ  message
 ****************************************************************************/
void uecc_fsm_ue_rlf_rrc_reestablish_req_handler  (
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                     UECC_RLF_W_FOR_REEST_TIMER,
                     &p_ue_context->timers_data);

    uecc_fsm_ue_rrc_re_establish_req_handler(p_ue_context,p_api); 


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);


} 


  
/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_ue_context_mod_ongoing_init_handler(
    uecc_ue_context_t   *p_ue_context,  /* UECC UE context */
    void                *p_api          /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    uecc_fsm_start_procedure(
        p_ue_context, &uecc_fsm_ue_context_mod_procedure, p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_success_int_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the received Success for API
 *                  RRC_RRM_UE_CONTEXT_MOD_RESP
 ****************************************************************************/
void uecc_fsm_ue_context_mod_success_int_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        )
{


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    UECC_FSM_SET_MAIN_STATE(p_ue_context, 
            &uecc_fsm_ue_connected, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_failure_int_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the Failure from RRM for UE_CONTEXT_MOD_RESP
 ****************************************************************************/
void uecc_fsm_ue_context_mod_failure_int_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        )
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
                        &uecc_fsm_ue_connected, PNULL);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_failure_rel_ue_contxt_int_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the Failure from RRM for UE_CONTEXT_MOD_RESP
 ****************************************************************************/
void uecc_fsm_ue_context_mod_failure_rel_ue_contxt_int_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        )
{

    s1ap_Cause cause;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_unspecified_2;

    /* build and send UE CONTEXT RELEASE REQ to MME */
    if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                p_ue_context,
                &cause))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_WARNING,"[s1ap_UEContextReleaseRequest] Delivery failed");
    }                       

    /* Change UE Main FSM State to UE CONNECTED */
    UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_re_establish_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the Failure from RRM for UE_CONTEXT_MOD_RESP
 ****************************************************************************/
void uecc_fsm_ue_context_mod_re_establish_req_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_connected, PNULL);

    /* Send event to UE Main FSM */
   uecc_event_queue_push_event(
        UECC_EV_RRC_RE_ESTABLISH_REQ,
        p_ue_context,
        p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_ho_required_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the Failure from RRM for UE_CONTEXT_MOD_RESP
 ****************************************************************************/
void uecc_fsm_ue_context_mod_ho_required_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        )
{

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    UECC_FSM_SET_MAIN_STATE(p_ue_context,
            &uecc_fsm_ue_connected, PNULL);

    /* Send event to UE Main FSM */

    uecc_event_queue_push_event(
      /*BUG 604 changes start*/
      UECC_EV_RRM_HO_REQUIRED,
      /*BUG 604 changes stop*/
      p_ue_context,
      p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_radio_link_failure_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the Failure from RRM for UE_CONTEXT_MOD_RESP
 ****************************************************************************/
void uecc_fsm_ue_context_mod_radio_link_failure_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_connected, PNULL);

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_RADIO_LINK_FAILURE_INT,
        p_ue_context,
        p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_ue_release_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the Failure from RRM for UE_CONTEXT_MOD_RESP
 ****************************************************************************/
void uecc_fsm_ue_context_mod_ue_release_req_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_connected, PNULL);

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_UE_RELEASE_REQ,
        p_ue_context,
        p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_s1ap_reset_msg_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the Failure from RRM for UE_CONTEXT_MOD_RESP
 ****************************************************************************/
void uecc_fsm_ue_context_mod_s1ap_reset_msg_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_connected, PNULL);

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_S1AP_RESET_MSG_IND,
        p_ue_context,
        p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_csc_delete_all_ue_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the Failure from RRM for UE_CONTEXT_MOD_RESP
 ****************************************************************************/
void uecc_fsm_ue_context_mod_csc_delete_all_ue_req_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_connected, PNULL);

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_CSC_DELETE_ALL_UE_REQ,
        p_ue_context,
        p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_ue_connection_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the Failure from RRM for UE_CONTEXT_MOD_RESP
 ****************************************************************************/
void uecc_fsm_ue_context_mod_ue_connection_release_ind_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_connected, PNULL);

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_RRM_UE_CONNECTION_RELEASE_IND,
        p_ue_context,
        p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_erab_modify_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_MODIFY_REQ message
 ****************************************************************************/
void uecc_fsm_ue_s1ap_erab_modify_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* Set the State of Main FSM to UE CONNECTION RECONFIGURATION ONGOING */
    UECC_FSM_SET_MAIN_STATE (p_ue_context, 
                             &uecc_fsm_ue_connection_reconfiguration_ongoing, 
                             PNULL);

    /* Start the E-RAB Modify Procedure*/
    uecc_fsm_start_procedure (p_ue_context,
                              &uecc_fsm_erb_modify_procedure, 
                              p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_modify_success_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_MODIFY_RESP_SUCCESS_INT message
 ****************************************************************************/
void uecc_fsm_erb_modify_success_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* Set the state of main FSM to UE CONNECTED */
    UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);
    if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
    {
        /* Start the HO Procedure */
        uecc_event_queue_push_event(
                    UECC_EV_HO_REQUIRED_INT,
                    p_ue_context,
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
        p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
        p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;   

        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
        
    }

    /* Stop Procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);
#ifdef MEM_PROFILE_ON

     RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
             p_ue_context->p_gb_context->facility_name, 
             RRC_INFO,"\n ***************Sizeof UE CONTEXT after ERAB MODIFY is successful %d ",sizeof(uecc_ue_context_t));
    qvPoolStats();

#endif
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_erb_modify_failure_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_MODIFY_RESP_FAILURE_INT message
 ****************************************************************************/
void uecc_fsm_erb_modify_failure_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    if ( PNULL == p_ue_context->p_curr_proc_data)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_FATAL,
                "p_ue_context->p_curr_proc_data is NULL"); 
        return;
    }
    if ( PNULL != p_ue_context->p_curr_proc_data )
    {
        /* Failure on E-RAB Modify Procedure */
        /* If HO Message is present  */ 
        if ( p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
        {
            /* Send event to UE Main FSM */
            uecc_event_queue_push_event(
                    UECC_EV_HO_REQUIRED_INT,
                    p_ue_context,
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd); 

            p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
        else if (p_ue_context->p_curr_proc_data->msg_bfr_type == 
                RRC_RE_ESTABLISH_MSG)
        {
            /* Send event to UE Main FSM */
            uecc_event_queue_push_event(
                    UECC_EV_RRC_RE_ESTABLISH_REQ_INT,
                    p_ue_context,
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd); 

            p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
        else if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_RLF_MSG)
        {
            /* Send event to UE Main FSM */
            uecc_event_queue_push_event(
                    UECC_EV_LLIM_RADIO_LINK_FAILURE_IND,
                    p_ue_context,
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd); 

            p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }

        /* Free memory for curr_proc_data_t */
        /* Freed if any message is buffered */
        else
        {
            if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
            {
                rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
                p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
            }

            rrc_mem_free(p_ue_context->p_curr_proc_data);
            p_ue_context->p_curr_proc_data = PNULL;
        }
    }

    /* Change UE Main FSM State to UE CONNECTED */
    UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);
    /* Stop Procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_modify_ue_context_release_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_MODIFY_FAILURE_REL_UE_CTX_INT message
 ****************************************************************************/
void uecc_fsm_erb_modify_ue_context_release_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    U8                            fail_idx = RRC_NULL;
    rrc_bool_et                   found    = RRC_FALSE;

    erab_failed_to_modify_list_t  *p_fail_list;
    
    s1ap_Cause cause;
    
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    p_fail_list = &(p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
                  erab_failed_to_modify_list);
   
    /* Failure on E-RAB Modify Procedure */
    for (fail_idx = 0;
         fail_idx < p_fail_list->erab_failed_to_modify_list_counter;
         fail_idx++)
    {
        if (p_fail_list->erab_failed_to_modify_item[fail_idx].
            cause.u.radioNetwork  == 
            s1ap_failure_in_radio_interface_procedure)
        {
            found = RRC_TRUE;
            break;
        }
    }
   
    /* Failure on E-RAB Modify Procedure */
    cause.t = T_s1ap_Cause_radioNetwork;
    if (found == RRC_TRUE)
    {
        cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;
    }
    else
    {
        cause.u.radioNetwork = s1ap_unspecified_2;
    }
   
    /* Now build and send UE CONTEXT RELEASE REQ to MME */
    if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                p_ue_context,
                &cause))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_WARNING,"[s1ap_UEContextReleaseRequest] Delivery failed");
    }                       

    /* Change UE Main FSM State to UE CONNECTED */
    UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);
    
    /* Free memory for curr_proc_data_t */
    if ( PNULL != p_ue_context->p_curr_proc_data )
    {
        /* Freed if any message is buffered */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd=PNULL;
        }

        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    /* Stop Procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);

    /* SPR 2510 Start */

    /* CHANGE STATE TO WAIT FOR CONNECTION RELEASE */
    UECC_FSM_SET_MAIN_STATE(
            p_ue_context, &uecc_fsm_ue_wait_for_connection_release, PNULL);

    /* SPR 2510 Stop */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_erb_ongoing_ho_required_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_ERB_ONGOING_HO_REQUIRED_INT message
 ****************************************************************************/
void 
uecc_fsm_erb_ongoing_ho_required_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    UECC_FSM_SET_MAIN_STATE(p_ue_context, 
            &uecc_fsm_ue_connected,
            PNULL);

    /* SPR 2532 Start */

    /* Send event to UE Main FSM */

    uecc_event_queue_push_event(
      UECC_EV_HO_REQUIRED_INT,
      p_ue_context,
      p_api);

    /* SPR 2532 Stop */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/* ERB RELEASE COMMAND START */
/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_erab_release_command_handler
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 *
 * Outputs        : None
 
 * Returns        : None
 *
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_RELEASE_COMMAND message
 *
 ****************************************************************************/
void uecc_fsm_ue_s1ap_erab_release_command_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    UECC_FSM_SET_MAIN_STATE(p_ue_context, 
        &uecc_fsm_ue_connection_reconfiguration_ongoing, PNULL);

    uecc_fsm_start_procedure(p_ue_context,
        &uecc_fsm_ue_erb_release_procedure, p_api);


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR 18758 Fix Start*/
/****************************************************************************
 * Function Name  : uecc_fsm_ue_proc_ongoing_erab_rel_cmd_handler
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 *
 * Description    : This action routine is called when Connection Reestablishment procedure 
 *                  is ongoing and UECC_EV_S1AP_ERAB_RELEASE_COMMAND message is received
 *
 ****************************************************************************/
void uecc_fsm_ue_proc_ongoing_erab_rel_cmd_handler(
    uecc_ue_context_t*   p_ue_context,
    void*                p_api
)
{
    OSRTDListNode*                      p_node = PNULL;
    erab_list_t*                        p_erab_failed_list = PNULL;
    rrc_s1ap_erab_release_command_t*    p_msg = PNULL;
    s1ap_E_RABList_element*             p_erab_elem = PNULL;
    /*SPR_18582_FIX_START*/
    U8                                  index = RRC_NULL;
    /* code removed */
    /*SPR_18582_FIX_END*/
    U16                                 e_rab_to_be_released_list_count = RRC_NULL;
    /* SES-521 Changes - Starts */
    rrc_bool_et                         free_curr_proc = RRC_FALSE;
    /* SES-521 Changes - Ends */

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Allocate memory for erab_rel_cmd */
    p_msg = (rrc_s1ap_erab_release_command_t*)p_api;

    /* SES-521 Changes - Starts */
    /* Check Curr Proc Data, if not allocated then
     * Allocate and Free after sending appropriate response
     */
    if(PNULL == p_ue_context->p_curr_proc_data)
    {
        p_ue_context->p_curr_proc_data =
            (curr_proc_data_t*)rrc_mem_get(sizeof(curr_proc_data_t));
        free_curr_proc = RRC_TRUE;
        RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Allocated for Current PROC data.");
    }

    if (PNULL != p_ue_context->p_curr_proc_data)
    /* SES-521 Changes - Ends */
    {
        /* points to erab_failed_to_release_list */
        p_erab_failed_list = &p_ue_context->p_curr_proc_data->u.
            erab_release_command_data.erab_failed_to_release_list;
        /*SPR_18582_FIX_START*/
        for(index = RRC_NULL;index < MAX_ERAB_COUNT;index++)
        {
            p_erab_failed_list->erab_item[index].e_RAB_ID = INVALID_ERAB_ID;
        }
        for (p_node = p_msg->e_rab_to_be_released_list.head;
            (PNULL != p_node) &&
            (e_rab_to_be_released_list_count < MAX_MME_ERAB_LIST_COUNT) &&
            (p_erab_failed_list->counter < MAX_ERAB_COUNT);
            p_node = p_node->next, e_rab_to_be_released_list_count++)
            /*SPR_18582_FIX_END*/
        {
            p_erab_elem = (s1ap_E_RABList_element*)p_node->data;
            /*SPR_18582_FIX_START*/
            e_rab_to_be_released_list_count++;
            /*SPR_18582_FIX_END*/

            if ( (p_erab_elem == PNULL) || (p_erab_elem->value.u._E_RABItemIEs_1 == PNULL))
            {
                continue;
            }
            /*SPR_18582_FIX_START*/
            if (RRC_SUCCESS == check_if_erab_id_already_present_in_failed_list(
                        p_erab_failed_list->erab_item,
                        p_erab_elem->value.u._E_RABItemIEs_1->e_RAB_ID,
                        p_ue_context))
            {
                RRC_UECC_UE_FSM_TRACE(RRC_INFO,
                    "Duplicate occurance, E_RAB ID [%u] already present in failed list",
                    p_erab_elem->value.u._E_RABItemIEs_1->e_RAB_ID);
                continue;
            }
            /* check for valid IDs was performed before when decoding */

            p_erab_failed_list->erab_item[p_erab_failed_list->counter].e_RAB_ID =
                p_erab_elem->value.u._E_RABItemIEs_1->e_RAB_ID;
            /*SPR_18582_FIX_END*/
            p_erab_failed_list->erab_item[p_erab_failed_list->counter].cause.t =
                T_s1ap_Cause_radioNetwork ;
            p_erab_failed_list->erab_item[p_erab_failed_list->counter].cause.u.radioNetwork =
                s1ap_interaction_with_other_procedure;

            p_erab_failed_list->counter++;
        }
        /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                    RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
        {
            RRC_UECC_UE_FSM_TRACE(RRC_FATAL,
                "[s1ap_ERABReleaseResponse] delivery failure");
        }

        /* SES-521 Changes - Starts */
        /* Free memory for curr_proc_data_t, only if allocated here */
        if(RRC_TRUE == free_curr_proc)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data);
            p_ue_context->p_curr_proc_data = PNULL;
            RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Free for Current PROC data.");
        }
    }
    else
    {
        RRC_UECC_UE_FSM_TRACE(RRC_FATAL, "p_curr_proc_data is NULL");
    }
    /* SES-521 Changes - Ends */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR 18758 Fix Stop*/

/****************************************************************************
 * Function Name  : uecc_fsm_ue_erab_release_success_int_handler 
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_RELEASE_COMMAND_SUCCESS_INT/
 *                  UECC_EV_RRM_ERAB_RELEASE_IND_SUCCESS_INT    event
 *
 *
 ****************************************************************************/
void uecc_fsm_ue_erab_release_success_int_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api                  /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);

#ifdef MEM_PROFILE_ON

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_INFO,"\n ***************Sizeof UE CONTEXT after ERAB RELEASE is successful %d ",sizeof(uecc_ue_context_t));
    qvPoolStats();
#endif

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_erab_release_failure_int_handler
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT/
 *                  message
 ****************************************************************************/
void uecc_fsm_ue_erab_release_failure_int_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* SPR 2510 Start */

    /* CHANGE STATE TO WAIT FOR CONNECTION RELEASE */ 
    UECC_FSM_SET_MAIN_STATE(
            p_ue_context, &uecc_fsm_ue_wait_for_connection_release, PNULL);

    /* SPR 2510 Stop */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_erab_release_validation_failure_int_handler
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_RELEASE_COMMAND_VALIDATION_FAILURE/
 *                   message
 ****************************************************************************/
void uecc_fsm_ue_erab_release_validation_failure_int_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/* ERB RELEASE COMMAND END */

/* ERB RELEASE INDICATION START */
/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_erab_release_ind_handler
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRM_ERAB_RELEASE_REQ message
 ****************************************************************************/
void uecc_fsm_ue_rrm_erab_release_ind_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    UECC_FSM_SET_MAIN_STATE(p_ue_context,
            &uecc_fsm_ue_connection_reconfiguration_ongoing,
            PNULL);

    uecc_fsm_start_procedure(p_ue_context,
            &uecc_fsm_ue_rrm_erb_release_procedure, p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* ERB RELEASE INDICATION END */
/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrc_re_establish_req_int_handler
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ_INT and message
 ****************************************************************************/
void uecc_fsm_ue_rrc_re_establish_req_int_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_connected, PNULL);

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_RRC_RE_ESTABLISH_REQ,
        p_ue_context,
        p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_ue_rrc_re_establish_req_int_handler
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ_INT and message
 ****************************************************************************/
void uecc_fsm_trg_ho_ue_rrc_re_establish_req_int_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_connection_re_establishment_ongoing, PNULL);

    uecc_fsm_start_procedure(p_ue_context,
        &uecc_fsm_ue_connection_re_establishment_procedure, p_api);

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Freed if any message is buffered */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {       
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }       
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrc_re_establish_req_handler
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ and message
 ****************************************************************************/
void uecc_fsm_ue_rrc_re_establish_req_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_connection_re_establishment_ongoing, PNULL);

    uecc_fsm_start_procedure(p_ue_context,
        &uecc_fsm_ue_connection_re_establishment_procedure, p_api);

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Freed if any message is buffered */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {       
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }       
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR_17976_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_re_estb_req_during_re_establish_handler
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when 
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ message is received in
 *                  re_establishment ongoing state
 ****************************************************************************/
void uecc_fsm_re_estb_req_during_re_establish_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
)
{
    s1ap_Cause cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_INFO,
        "Re-establishment request received when re-establishment procedure"
        " was already ongoing for UE:%u", p_ue_context->ue_index);

    if (RRC_SUCCESS !=
            uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                p_ue_context->p_gb_context, p_ue_context->crnti, 0,
                p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
    {
        RRC_UECC_UE_FSM_TRACE(RRC_ERROR,
                "Fail to send the RRCConnectionReestablishmentReject to UE");
    }

    p_ue_context->m.reestb_during_reestb = RRC_TRUE;

    p_ue_context->
        share_data.uecc_rcr_data.rrc_connection_release_not_required_flag =
        RRC_TRUE;

    cause.t= T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;

    if ((RRC_TRUE != p_ue_context->trg_ho_fail_rrc_cre_flag) ||
        (X2_HO != p_ue_context->ho_info.s1_or_x2_handover) ||
        (RRC_TRUE == p_ue_context->ho_info.p_ho_info->
                         p_trg_ho_info->is_path_switch_req_ack_recv))
    {
        /* SPR-21471 START */
        /* Stop timer */
        uecc_ue_timer_stop(
                p_ue_context->p_gb_context,
                UECC_UE_CRE_TIMER,
                &p_ue_context->timers_data);
        /* SPR-21471 END */

        if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                    p_ue_context,
                    &cause ))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING,"[s1ap_UEContextReleaseRequest] Delivery failed");
        }                       

        if (RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context,
                    UECC_UE_MME_GUARD_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "Failure to start MME Guard Timer.");
        }
    }
    else
    {
        /* This else part handles the case where X2 HO is ongoing and
         * path switch req ack has not yet been received.
         */
        if (RRC_FAILURE == uecc_rrm_build_and_send_ue_reestab_complete_ind
                (p_ue_context, RRC_FAILURE))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,p_ue_context->
                    p_gb_context->facility_name,RRC_WARNING,
                    "Unable to send RRC_RRM_UE_REESTAB_COMPLETE_IND");
        }

        /*RRC_RRM_HO_ADM_CNF to RRM*/
        uecc_rrm_build_and_send_ue_ho_adm_cnf(
                p_ue_context,
                RRC_FAILURE);

        if(uecc_trg_is_path_switch_ongoing(p_ue_context))
        {
            if(RRC_FAILURE == uecc_s1ap_build_and_send_ue_release_ind(p_ue_context))
            {
                RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                        "[%s] Failed to send UE Release Indication to S1AP for UE:%u"
                        ,__FUNCTION__, p_ue_context->ue_index);
            }
        }

        /*Release UE context maintained at X2AP module*/
        uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);

        /*Adding flags to avoid sending UE Context Release
         * Complete to MME
         */
        SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_ue_context);

        /* Stop timer */
        uecc_ue_timer_stop(
                p_ue_context->p_gb_context,
                UECC_UE_CRE_TIMER, 
                &p_ue_context->timers_data);    

        if(RRC_FAILURE == x2ap_build_and_send_reestablishment_ind(
                    p_ue_context))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_ERROR,
                    "x2ap_build_and_send_reestablishment_ind failed.");
        }

        /* Finish Current procedure */
        uecc_fsm_finish_active_procedure(p_ue_context);
        
        uecc_fsm_ue_release_req_handler_for_reestb(p_ue_context, cause);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_mme_timer_expiry_handler_for_re_estb_handler
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called on
 *                  MME_GUARD_TIMER expiry in re_establishment ongoing state
 ****************************************************************************/
void uecc_fsm_mme_timer_expiry_handler_for_re_estb_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
)
{
    s1ap_Cause cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_INFO,
            "MME Guard Timer expired during re-establishment procedure"
            " for UE:%u", p_ue_context->ue_index);

    /*Adding flags for Initiating the MME RESET on
     * completion of UE release
     */
    p_ue_context->
    share_data.uecc_rcr_data.reset_towards_mme_required = RRC_TRUE ; 

    if (RRC_FAILURE == uecc_rrm_build_and_send_ue_reestab_complete_ind
            (p_ue_context, RRC_FAILURE))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,p_ue_context->
                p_gb_context->facility_name,RRC_WARNING,
                "Unable to send RRC_RRM_UE_REESTAB_COMPLETE_IND");
    }

    if (RRC_TRUE ==
            p_ue_context->trg_ho_fail_rrc_cre_flag)
    {
        /*RRC_RRM_HO_ADM_CNF to RRM*/
        uecc_rrm_build_and_send_ue_ho_adm_cnf(
                p_ue_context,
                RRC_FAILURE);
    }

    /* Stop timer */
    uecc_ue_timer_stop(
            p_ue_context->p_gb_context,
            UECC_UE_CRE_TIMER, 
            &p_ue_context->timers_data);    

    if(RRC_FAILURE == x2ap_build_and_send_reestablishment_ind(
                p_ue_context))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_ERROR,
                "x2ap_build_and_send_reestablishment_ind failed.");
    }

    /* Finish Current procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);

    cause.t= T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;

    uecc_fsm_ue_release_req_handler_for_reestb(p_ue_context, cause);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_release_req_handler_for_reestb
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 *                  s1ap_Cause cause          
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called during handling of UE 
 *                  release in reestablishment procedure
 ****************************************************************************/
void uecc_fsm_ue_release_req_handler_for_reestb(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    s1ap_Cause           cause              /* Cause */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* check if handover is ongoing then stop the procedure first */
    if(PNULL != p_ue_context->ho_info.p_ho_info)
    {
        /* stop the handover procedure ongoing */
        uecc_fsm_finish_active_procedure(p_ue_context);
    }
    p_ue_context->share_data.uecc_rcr_data.u.cause = cause;

    if (PNULL !=
    p_ue_context->timers_data.timers_ids[UECC_UE_MME_GUARD_TIMER])
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_MME_GUARD_TIMER, &p_ue_context->timers_data);

    }


 
    UECC_FSM_SET_MAIN_STATE(
        p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(
        p_ue_context, &uecc_fsm_ue_connection_release_procedure, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR_17976_END*/

/* ERB RELEASE INDICATION END */

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rlf_reset_msg_ind_handler
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_RESET_MSG_IND message
 ****************************************************************************/
void uecc_fsm_ue_rlf_reset_msg_ind_handler(
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

     uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_RLF_W_FOR_REEST_TIMER,
                      &p_ue_context->timers_data);
    uecc_fsm_ue_reset_msg_ind_handler(p_ue_context,p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_rlf_ue_context_mod_req_handler
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UE CONTEXT MODIFICATION REQUEST  message.
 ****************************************************************************/
void uecc_fsm_rlf_ue_context_mod_req_handler (
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);         

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_INFO,"Discarding the UE CONTEXT MODIFICTAION REQUEST");


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
* Function Name   : uecc_fsm_mme_timer_expiry_handler_wait_for_s1ap_ue_ctx_rel_cmd
** Inputs         : uecc_ue_context_t* - pointer to UE context
**                  void*   - api data
** Outputs        : None
** Returns        : None
** Description    : This is the handler function for the message
 *                  UECC_EV_MME_GUARD_TIMER_EXPIRY
*****************************************************************************/
void uecc_fsm_mme_timer_expiry_handler_wait_for_s1ap_ue_ctx_rel_cmd(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_SET_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);

    /*SPR 20901 Start*/
    /* Code Removed */
    /*SPR 20901 Stop*/

    /*SPR 21220 Start*/
        if (RRC_SUCCESS != uecc_s1ap_build_and_send_ue_release_msg_req(
                    p_ue_context->s1ap_ue_id,
                    RRC_TRUE,
                    p_ue_context))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "ue_release_msg delivery failure");
        }
    /*SPR 21220 Stop*/
    p_ue_context->share_data.uecc_rcr_data.u.cause.t =
            T_s1ap_Cause_radioNetwork;
    p_ue_context->share_data.uecc_rcr_data.u.cause.u.radioNetwork =
            s1ap_failure_in_radio_interface_procedure;

    /* Bug 873 Fix Start */
    if (p_ue_context->m.idle_mode_mobility_control_present)
    {
        S1AP_UPDATE_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
    }
    /* Bug 873 Fix Stop */

    UECC_FSM_SET_MAIN_STATE(
                    p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(
                    p_ue_context,
                    &uecc_fsm_ue_connection_release_setup_failure_procedure, PNULL);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR_19717_start*/
/*******************************************************************************
** Function Name  : uecc_fsm_csc_cell_delete_handler_wait_for_s1ap_ue_ctx_rel_cmd
** Inputs         : uecc_ue_context_t* - pointer to UE context
**                  void*   - api data
** Outputs        : None
** Returns        : None
** Description    : This is the handler function for the message
**                  UECC_EV_CSC_DELETE_ALL_UE_REQ
*****************************************************************************/
void uecc_fsm_csc_cell_delete_handler_wait_for_s1ap_ue_ctx_rel_cmd(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

   if (PNULL !=
    p_ue_context->timers_data.timers_ids[UECC_UE_MME_GUARD_TIMER])
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_MME_GUARD_TIMER, &p_ue_context->timers_data);
    }

   RRC_SET_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
    
    S1AP_UPDATE_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);

    UECC_FSM_SET_MAIN_STATE(
                    p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

    uecc_fsm_start_procedure(
                    p_ue_context,
                    &uecc_fsm_ue_connection_release_csc_cel_del_procedure, PNULL);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR_19717_stop*/

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_ho_required_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  RRC_RRM_UE_HO_REQUIRED/UECC_EV_HO_REQUIRED_INT
 *                  message in UECC_UE_CONNECTED state.
 ****************************************************************************/
void uecc_fsm_ue_rrm_ho_required_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(
            p_ue_context, &uecc_fsm_ue_src_ho_ongoing, p_api);

    uecc_fsm_start_procedure(
            p_ue_context,
            &uecc_fsm_source_handover_procedure, p_api);

    /*securitykey*/
    if ((PNULL != p_ue_context->p_curr_proc_data) && (!p_ue_context->ue_ctx_mod_security_flag))
    {
        /* Freed if any message is buffered */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {       
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }       
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_src_ho_failure_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_SRC_HO_FAILURE_INT in UECC_UE_SRC_HO_ONGOING state.
 ****************************************************************************/
void uecc_fsm_ue_src_ho_failure_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    s1ap_Cause cause;
    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    memset_wrapper( &cause, RRC_NULL, sizeof(s1ap_Cause));

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
/* BUG: 7882 Start */
    if ( ((p_ue_context->ho_info.x2ap_cause.t == T_x2ap_Cause_radioNetwork) && 
                (p_ue_context->ho_info.x2ap_cause.u.radioNetwork == x2ap_tx2relocoverall_expiry)) ||
            ((p_ue_context->ho_info.cause.t == T_s1ap_Cause_radioNetwork) && 
             (p_ue_context->ho_info.cause.u.radioNetwork == s1ap_tS1relocoverall_expiry)))
    {
        if ( (p_ue_context->ho_info.x2ap_cause.t == T_x2ap_Cause_radioNetwork) && 
                (p_ue_context->ho_info.x2ap_cause.u.radioNetwork == x2ap_tx2relocoverall_expiry)) 

        {
            cause.t= T_s1ap_Cause_radioNetwork;
            cause.u.radioNetwork = s1ap_tx2relocoverall_expiry;
        }
        else if ((p_ue_context->ho_info.cause.t == T_s1ap_Cause_radioNetwork) && 
                (p_ue_context->ho_info.cause.u.radioNetwork == s1ap_tS1relocoverall_expiry))
        {
            cause.t= p_ue_context->ho_info.cause.t;
            cause.u.radioNetwork = p_ue_context->ho_info.cause.u.radioNetwork;
        }

        if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                    p_ue_context,
                    &cause ))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING,"[s1ap_UEContextReleaseRequest] Delivery failed");
        }                       

        if ( RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context, 
                    UECC_UE_MME_GUARD_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index) )
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[UECC_UE_MME_GUARD_TIMER]"
                    "Timer Start Failed");
        }

        p_ue_context->
            share_data.uecc_rcr_data.rrc_connection_release_not_required_flag =
            RRC_TRUE;


        UECC_FSM_SET_MAIN_STATE(
                p_ue_context, &uecc_fsm_ue_wait_for_s1ap_ue_ctx_rel_cmd, PNULL);
    }
    else
    {
        UECC_FSM_SET_MAIN_STATE(
                p_ue_context, &uecc_fsm_ue_connected, PNULL);
    }
    /* BUG: 7882 End */

    memset_wrapper (&p_ue_context->ho_info.intra_cell_ho_cause, RRC_NULL, sizeof(s1ap_Cause));

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_failure_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_TRG_HO_FAILURE_INT in UECC_UE_TRG_HO_ONGOING state.
 ****************************************************************************/
void uecc_fsm_ue_trg_ho_failure_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);         
    /* SPR 16750 Fix Start */
    if (RRC_TRUE == 
                p_ue_context->share_data.uecc_rcr_data.s1_error_ind_recv)
    {
        if(RRC_FAILURE == uecc_s1ap_build_and_send_ue_release_ind(p_ue_context))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[%s] Failed to send UE Release Indication to S1AP "
                    ,__FUNCTION__);
        }
    }
    /* SPR 16750 Fix Stop */
    /*SPR_17484_START*/
    if ((PNULL != p_ue_context->p_gb_context->p_p_csc_context) &&
            (PNULL != p_ue_context->p_gb_context->
               p_p_csc_context[p_ue_context->cell_index]))	/* Coverity Fix 63642 Stop */
    {
        /* SPR 18037 Fix Start */
        if ((p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                    del_cell_ongoing == 1) &&
            (p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                count_use_ue_del_cell_req == 0))
        /* SPR 18037 Fix Stop */
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_INFO,
                    "%s sending CSC DEL ALL UE ENTITY RESP",__FUNCTION__);
            p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                del_cell_ongoing = 0;
            uecc_csc_build_and_send_delete_all_ue_entity_resp(RRC_SUCCESS,
                    p_ue_context->p_gb_context,p_ue_context->cell_index);
            if (RRC_TRUE == p_ue_context->p_gb_context->
                    p_p_csc_context[p_ue_context->cell_index]->is_cell_stop_ongoing)
            {
                p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                    is_cell_stop_ongoing = RRC_FALSE;
            }
            else
            {
                if (p_ue_context->p_gb_context->p_p_csc_context != PNULL )
                {
                    if (p_ue_context->p_gb_context->
                            p_p_csc_context[p_ue_context->cell_index] != PNULL )
                    {
                        if (p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->
                                cell_index]->p_csc_init_setup_ind != PNULL)
                        {
                            rrc_mem_free (p_ue_context->p_gb_context->
                                    p_p_csc_context[p_ue_context->cell_index]->
                                    p_csc_init_setup_ind);
                            p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->
                                cell_index]->p_csc_init_setup_ind = PNULL;
                        }
                        if (p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->
                                cell_index]->p_rrc_uecc_statistic != PNULL)
                        {
                            rrc_mem_free (p_ue_context->p_gb_context->
                                    p_p_csc_context[p_ue_context->cell_index]->p_rrc_uecc_statistic);
                            p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->
                                cell_index]->p_rrc_uecc_statistic = PNULL;
                        }
                        if (p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->
                                cell_index]->p_cell_traffic_trace_info != PNULL)
                        {
                            rrc_mem_free (p_ue_context->p_gb_context->
                                    p_p_csc_context[p_ue_context->cell_index]->p_cell_traffic_trace_info);
                            p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->
                                cell_index]->p_cell_traffic_trace_info = PNULL;
                        }

                        rrc_mem_free(p_ue_context->p_gb_context->
                                p_p_csc_context[p_ue_context->cell_index]);
                        p_ue_context->p_gb_context->
                            p_p_csc_context[p_ue_context->cell_index] = PNULL;
                    }
                }
            }
        }
    }
    /*SPR_17484_END*/
    /* Free UE context */
    uecc_ue_ctx_free(p_ue_context->p_gb_context, p_ue_context->ue_index );

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_ho_rel_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_SRC_HO_REL_REQ_INT in UECC_UE_SRC_HO_ONGOING state.
 ****************************************************************************/
void uecc_fsm_ue_ho_rel_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Bug 814 Fix Start */
    RRC_SET_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
    /* Bug 814 Fix Stop */
    UECC_FSM_SET_MAIN_STATE(
            p_ue_context, &uecc_fsm_ue_wait_for_connection_release, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_ho_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_HO_REQUEST message in UECC_UE_IDLE state.
 ****************************************************************************/
void uecc_fsm_ue_s1ap_ho_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(
            p_ue_context, &uecc_fsm_ue_trg_ho_ongoing, p_api);

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_INFO,"[%s]Starting TRG HO procedure \
            Received S1AP Handover Request",
            __FUNCTION__);

    uecc_fsm_start_procedure(
            p_ue_context,
            &uecc_fsm_ue_trg_ho_procedure, p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_x2ap_ho_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_X2AP_HO_REQUEST message in UECC_UE_IDLE state.
 ****************************************************************************/
void uecc_fsm_ue_x2ap_ho_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(
            p_ue_context, &uecc_fsm_ue_trg_ho_ongoing, p_api);

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_INFO,"Starting TRG HO procedure "
            " Received X2AP Handover Request");

    uecc_fsm_start_procedure(
            p_ue_context,
            &uecc_fsm_ue_trg_ho_procedure, p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_success_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_TRG_HO_SUCCESS_INT message in 
 *                  UECC_UE_TRG_HO_ONGOING state.
 ****************************************************************************/
void uecc_fsm_ue_trg_ho_success_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_UE_FSM_TRACE(RRC_DETAILEDALL,"Unused variable in FSM:%p",
            p_api);
    
    /* x2ap_drop2_rw_start: Start Erab release procedure*/
    if(RRC_TRUE == p_ue_context->ho_info.is_x2_ho_erab_rel_reqd)
    {
        UECC_FSM_SET_MAIN_STATE(
                                p_ue_context,&uecc_fsm_ue_connection_reconfiguration_ongoing,
                                PNULL);

        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_INFO,"[%s]Starting Erab release procedure ",
                __FUNCTION__);

        uecc_fsm_start_procedure(
                                  p_ue_context, &uecc_fsm_erb_release_procedure,
                                  PNULL);
    }
    else
    {
        UECC_FSM_SET_MAIN_STATE(
                                p_ue_context, &uecc_fsm_ue_connected, PNULL);
        memset_wrapper (&p_ue_context->ho_info.intra_cell_ho_cause, RRC_NULL, sizeof(s1ap_Cause));
	
	    /*SPR 14160 Fix Start*/
	    /* Reset the HO Flag in UE Context */
        /* SPR Fix 19074 Start */
        /* Code Deleted */
        /* SPR Fix 19074 End */
	/*SPR 14160 Fix Stop*/
    }
    /* x2ap_drop2_rw_end */

#ifdef MEM_PROFILE_ON
    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_INFO,"\n ***************After Target Hanodver is successful %d ",sizeof(uecc_ue_context_t));
    qvPoolStats();

#endif

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_ho_rlf_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_SRC_HO_RADIO_LINK_FAILURE_INT message in 
 *                  Source or Target HO ongoing state.
 ****************************************************************************/
void uecc_fsm_ue_ho_rlf_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(
            p_ue_context, &uecc_fsm_ue_connected, PNULL);

    uecc_event_queue_push_event(
            UECC_EV_RADIO_LINK_FAILURE_INT,
            p_ue_context,
            p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_ho_re_est_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_SRC_HO_RE_EST_INT message in 
 *                  Source or Target HO ongoing state.
 ****************************************************************************/
void uecc_fsm_ue_ho_re_est_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    UECC_FSM_SET_MAIN_STATE(
            p_ue_context, &uecc_fsm_ue_connected, PNULL);

    uecc_event_queue_push_event(
            UECC_EV_RRC_RE_ESTABLISH_REQ,
            p_ue_context,
            p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR 18241 14JAN START*/
/*SPR_18241_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_handover_ongoing_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives UE Context Modification during HO.
 ****************************************************************************/
void uecc_fsm_ue_context_mod_handover_ongoing_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
)
{

   s1ap_Cause cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Send UE Context Modfication Failure */
    cause.t = T_s1ap_Cause_radioNetwork;
    if (X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
    {
        cause.u.transport = s1ap_x2_handover_triggered;
    }
    else if((S1_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
            (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    {
        cause.u.transport = s1ap_s1_intra_system_handover_triggered;
    }
    else if(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)
    {
        cause.u.transport = s1ap_interaction_with_other_procedure;
    }

    if (RRC_SUCCESS !=
         uecc_s1ap_build_and_send_ue_context_modification_failure (
             p_ue_context->p_gb_context, p_ue_context,
             RRC_NULL,RRC_NULL,&cause,PNULL))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[s1ap_UEContextModificationFailure]"
                "Delivery failed");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR_18241_END*/

/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_reconfig_ongoing_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives UE Context Modification during reconfiguration.
 ****************************************************************************/
void uecc_fsm_ue_context_mod_reconfig_ongoing_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
)
{
    s1ap_Cause cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_ue_context->ue_ctx_mod_security_flag = RRC_FALSE;
    p_ue_context->ue_ctx_mod_intra_ho_success_flag = RRC_FALSE;

    if(PNULL != p_ue_context->p_temp_ue_ctx_mod_proc_data)
    {
        rrc_mem_free(p_ue_context->p_temp_ue_ctx_mod_proc_data);
        p_ue_context->p_temp_ue_ctx_mod_proc_data = PNULL;
    }

    /* Allocate memory for the p_curr_proc_data */
    p_ue_context->p_temp_ue_ctx_mod_proc_data = (curr_proc_data_t *)
        rrc_mem_get(sizeof(curr_proc_data_t));
    if (p_ue_context->p_temp_ue_ctx_mod_proc_data == PNULL)
    {
        /* Memory Allocation Failed */
        RRC_UECC_UE_FSM_TRACE(RRC_FATAL,
                "Memory Allocation Failure");
        return;
    }    

    if(RRC_SUCCESS != uecc_fsm_process_ue_ctx_mod_msg(p_ue_context,
                p_ue_context->p_temp_ue_ctx_mod_proc_data,
                p_api))
    {
        RRC_UECC_UE_FSM_TRACE(RRC_ERROR,
                "UE CONTEXT MOD Process Msg failed");
        if(!p_ue_context->p_temp_ue_ctx_mod_proc_data->m.SecurityCapability_present)
        {
            cause.t = T_s1ap_Cause_radioNetwork;
            cause.u.radioNetwork = s1ap_unspecified_2;
        }
        else
        {
            cause.t = T_s1ap_Cause_radioNetwork;
            cause.u.protocol=
                s1ap_encryption_and_or_integrity_protection_algorithms_not_supported;
        }

        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_ue_context_modification_failure(
                    p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
        {
            RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                    "s1ap_UEContextModificationFailure Delivery failed");
        }

        rrc_mem_free(p_ue_context->p_temp_ue_ctx_mod_proc_data);
        p_ue_context->p_temp_ue_ctx_mod_proc_data = PNULL;
        return;
    }

    /* Invoke Intra cell HO to RRM  if security is present*/
    if ((p_ue_context->p_temp_ue_ctx_mod_proc_data->m.SecurityKey_present) ||
            ( (p_ue_context->p_temp_ue_ctx_mod_proc_data->m.SecurityCapability_present) &&
              ((p_ue_context->p_temp_ue_ctx_mod_proc_data->userplane_ciphering_algo_id !=
                p_ue_context->configure_ciphering_user_plane.algorithm_id) ||
               (p_ue_context->p_temp_ue_ctx_mod_proc_data->ciphering_algo_id !=
                p_ue_context->configure_ciphering.algorithm_id) ||
               (p_ue_context->p_temp_ue_ctx_mod_proc_data->integrity_algo_id !=
                p_ue_context->configure_integrity_protection.algorithm_id)) ) )
    {
        if (p_ue_context->rrc_trg_ho_fwd_tnl_info.drb_tunnel_info_count == 0)
        {
            /* Set the flag, which if TURE will indicate that Intra-Cell HO procedure
             * for Security Rekeying was originally invoked from UE Ctx Mod procedure*/
            p_ue_context->ue_ctx_mod_security_flag = RRC_TRUE;
            /* Build and send the RRC_RRM_INTRA_ENB_HO_IND to RRM */
            if (RRC_SUCCESS != uecc_llim_build_and_send_rrc_rrm_intra_enb_ho_ind(p_ue_context))
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                        "[RRC_RRM_INTRA_ENB_HO_IND] message sending failed for UE %u",
                        p_ue_context->ue_index);
                rrc_mem_free(p_ue_context->p_temp_ue_ctx_mod_proc_data);
                p_ue_context->p_temp_ue_ctx_mod_proc_data = PNULL;
            }
        }
        else
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING,
                    "[RRC_RRM_INTRA_ENB_HO_IND] Forwarding of previous HO is ongoing for UE %u.",
                    p_ue_context->ue_index);
            /* Send UE Context Modfication Failure */
            cause.t = T_s1ap_Cause_radioNetwork;
            cause.u.radioNetwork = s1ap_interaction_with_other_procedure;

            if (RRC_SUCCESS !=
                    uecc_s1ap_build_and_send_ue_context_modification_failure(
                        p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
            {
                RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                        "[s1ap_UEContextModificationFailure]Delivery failed");
            }
            rrc_mem_free(p_ue_context->p_temp_ue_ctx_mod_proc_data);
            p_ue_context->p_temp_ue_ctx_mod_proc_data = PNULL;
        }
        /*SPR_17041_END*/
    }
    else
    {
        /*BUG 604 changes stop*/
        /* Build and sent the RRC_RRM_UE_CONTXT_MOD_REQ to RRM */
        if (RRC_SUCCESS !=
                uecc_rrm_build_and_send_ue_context_mod_req(p_ue_context))
        {
            /* Send UE Context Modfication Failure */
            cause.t = T_s1ap_Cause_radioNetwork;
            cause.u.radioNetwork = s1ap_unspecified_2;

            if (RRC_SUCCESS !=
                    uecc_s1ap_build_and_send_ue_context_modification_failure(
                        p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
            {
                RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                        "[s1ap_UEContextModificationFailure]Delivery failed");
            }
            rrc_mem_free(p_ue_context->p_temp_ue_ctx_mod_proc_data);
            p_ue_context->p_temp_ue_ctx_mod_proc_data = PNULL;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR 18241 14JAN END*/

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_context_mod_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the UE_CONTEXT_MOD_RESP from RRM
 ****************************************************************************/
void uecc_fsm_rrm_ue_context_mod_resp_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    if(PNULL == p_ue_context->p_temp_ue_ctx_mod_proc_data)
    {
        RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                "Unexpected message received for UE:%u", 
                p_ue_context->ue_index);
        return;
    }
    

    UECC_FSM_SET_MAIN_STATE(p_ue_context,
            &uecc_fsm_ue_context_mod_ongoing, PNULL);

    uecc_fsm_start_procedure(
        p_ue_context, &uecc_fsm_queued_ue_context_mod_procedure, p_api);

    if(RRC_TRUE == uecc_fsm_is_procedure_active(p_ue_context))
    {/* UE Context Modification Procedure is active */
        p_ue_context->p_curr_proc_data = p_ue_context->p_temp_ue_ctx_mod_proc_data;
        p_ue_context->p_temp_ue_ctx_mod_proc_data = PNULL;

        uecc_event_queue_push_event(
                UECC_EV_RRM_UE_CONTEXT_MOD_RESP,
                p_ue_context,
                p_api);
        RRC_UECC_UE_FSM_TRACE(RRC_INFO,
                "Pushing event UECC_EV_RRM_UE_CONTEXT_MOD_RESP [%u]",UECC_EV_RRM_UE_CONTEXT_MOD_RESP); 
    }
    else
    {
        RRC_UECC_UE_FSM_TRACE(RRC_INFO,
                "Ignoring received event UECC_EV_RRM_UE_CONTEXT_MOD_RESP"); 
        rrc_mem_free(p_ue_context->p_temp_ue_ctx_mod_proc_data);
        p_ue_context->p_temp_ue_ctx_mod_proc_data = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR 18241 14JAN START*/
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_context_mod_resp_during_handover_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the UE_CONTEXT_MOD_RESP from RRM during HO
 *                  at Source eNB.
 ****************************************************************************/
void uecc_fsm_rrm_ue_context_mod_resp_during_handover_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
)
{
    s1ap_Cause cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    if(PNULL != p_ue_context->p_temp_ue_ctx_mod_proc_data)
    {
        RRC_UECC_UE_FSM_TRACE(RRC_INFO,
                "For queued UE Context Modification, UE_CONTEXT_MOD_RESP "
                "received from RRM during HO for UE:%u."
                " Sending UE Context Modification failure to MME."
                , p_ue_context->ue_index); 

        /* Send UE Context Modfication Failure */
        cause.t = T_s1ap_Cause_radioNetwork;
        if (X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
            cause.u.transport = s1ap_x2_handover_triggered;
        }
        else if((S1_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
        {
            cause.u.transport = s1ap_s1_intra_system_handover_triggered;
        }
        else if(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
            cause.u.transport = s1ap_interaction_with_other_procedure;
        }

        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_ue_context_modification_failure (
                    p_ue_context->p_gb_context, p_ue_context,
                    RRC_NULL,RRC_NULL,&cause,PNULL))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[s1ap_UEContextModificationFailure]"
                    "Delivery failed");
        }
        rrc_mem_free(p_ue_context->p_temp_ue_ctx_mod_proc_data);
        p_ue_context->p_temp_ue_ctx_mod_proc_data = PNULL;
    }
    else
    {
        RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                "Unexpected event UE_CONTEXT_MOD_RESP received from RRM during"
                " HO for UE:%u. Ignoring it.", p_ue_context->ue_index); 
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_context_mod_resp_during_rlf_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the UE_CONTEXT_MOD_RESP from RRM during RLF.
 ****************************************************************************/
void uecc_fsm_rrm_ue_context_mod_resp_during_rlf_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
)
{
    rrc_rrm_ue_contxt_mod_cnf_t     rrc_rrm_ue_contxt_mod_cnf; 
    rrc_rrm_ue_contxt_mod_resp_t*   p_rrc_rrm_ue_contxt_mod_resp = PNULL;
    s1ap_Cause                      cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if(PNULL != p_ue_context->p_temp_ue_ctx_mod_proc_data)
    {
        p_rrc_rrm_ue_contxt_mod_resp =
            (rrc_rrm_ue_contxt_mod_resp_t*)p_api;
        if(RRC_SUCCESS == p_rrc_rrm_ue_contxt_mod_resp->response)
        {
            rrc_rrm_ue_contxt_mod_cnf.ue_index = p_rrc_rrm_ue_contxt_mod_resp->ue_index ; 
            if(!p_ue_context->p_temp_ue_ctx_mod_proc_data->m.AggMaxBitRate)
            {
                rrc_rrm_ue_contxt_mod_cnf.response = RRC_SUCCESS;

                if (RRC_SUCCESS !=
                        uecc_s1ap_build_and_send_ue_context_modification_response(p_ue_context))
                {
                    RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                            "[s1ap_UEContextModificationResponse]Delivery failed");
                    rrc_rrm_ue_contxt_mod_cnf.response = RRC_FAILURE;
                }                   
                /*SPR_18241_START*/
                if (RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(&rrc_rrm_ue_contxt_mod_cnf,
                            RRC_UECC_MODULE_ID,
                            RRC_RRM_MODULE_ID,
                            uecc_ue_ctx_get_op_transction_id(p_ue_context),p_ue_context->cell_index))
                {
                    RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                            "[,p_ue_context->cell_indexrrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf] Sending failed");
                }
            }
            else
            {
                RRC_UECC_UE_FSM_TRACE(RRC_INFO,
                        "For queued UE Context Modification for AggMaxBitRate change, "
                        "UE_CONTEXT_MOD_RESP received from RRM in RLF state for UE:%u."
                        " Sending UE Context Modification failure to MME."
                        , p_ue_context->ue_index); 
                rrc_rrm_ue_contxt_mod_cnf.response = RRC_FAILURE;
                if (RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(&rrc_rrm_ue_contxt_mod_cnf,
                            RRC_UECC_MODULE_ID,
                            RRC_RRM_MODULE_ID,
                            uecc_ue_ctx_get_op_transction_id(p_ue_context),p_ue_context->cell_index))
                {
                    RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                            "[rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf] Sending failed");
                }


                /*SPR_18241_END*/
                /* Send UE Context Modfication Failure */
                cause.t = T_s1ap_Cause_radioNetwork;
                cause.u.transport = s1ap_interaction_with_other_procedure;

                if (RRC_SUCCESS !=
                        uecc_s1ap_build_and_send_ue_context_modification_failure (
                            p_ue_context->p_gb_context, p_ue_context,
                            RRC_NULL,RRC_NULL,&cause,PNULL))
                {
                    RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                            "[s1ap_UEContextModificationFailure]"
                            "Delivery failed");
                }
            }
        }
        else
        {
            /* Send UE Context Modfication Failure */

            if (RRC_SUCCESS != uecc_s1ap_build_s1ap_cause(
                        &cause,
                        &(p_rrc_rrm_ue_contxt_mod_resp->cause),
                        p_ue_context))
            {
                RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                        "[s1ap_UEContextModificationFailure]invalid cause received from RRM");
            }

            if (RRC_SUCCESS !=
                    uecc_s1ap_build_and_send_ue_context_modification_failure(
                        p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
            {
                RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                        "[s1ap_UEContextModificationFailure]Delivery failed");
            }
        }

        rrc_mem_free(p_ue_context->p_temp_ue_ctx_mod_proc_data);
        p_ue_context->p_temp_ue_ctx_mod_proc_data = PNULL;
    }
    else
    {
        RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                "Unexpected event UE_CONTEXT_MOD_RESP received from RRM in"
                " RLF state for UE:%u. Ignoring it.", p_ue_context->ue_index); 
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR 18241 14JAN END*/

/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_cre_ongoing_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the UE_CONTEXT_MOD_REQ during Connection Reestablishment 
 ****************************************************************************/
void uecc_fsm_ue_context_mod_cre_ongoing_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
)
{
    s1ap_Cause cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Send UE Context Modfication Failure */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.transport = s1ap_interaction_with_other_procedure;

    if (RRC_SUCCESS !=
         uecc_s1ap_build_and_send_ue_context_modification_failure (
             p_ue_context->p_gb_context, p_ue_context,
             RRC_NULL,RRC_NULL,&cause,PNULL))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[s1ap_UEContextModificationFailure]"
                "Delivery failed");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* SPR 16087 Start */
/****************************************************************************
 * Function Name  : uecc_fsm_trg_ue_s1u_end_marker_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called to handle
 *                  UECC_EV_LLIM_S1U_END_MARKER_IND at TeNB.
 *                  
 ****************************************************************************/
void uecc_fsm_trg_ue_s1u_end_marker_ind_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (p_ue_context->rrc_trg_ho_fwd_tnl_info.drb_tunnel_info_count > RRC_NULL)
    {
        uecc_fsm_ue_s1u_end_marker_ind_handler(p_ue_context, p_api);
    }
    else
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_INFO, "If End Marker Ind is received at SeNB - ignoring this "
                "as no handling is required for this in received state. If "
                "End Market Ind is received at TeNB - Ignoring this because "
                "no forwarding tunnel present.");
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* SPR 16087 Stop */

/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1u_end_marker_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the UECC_EV_LLIM_S1U_END_MARKER_IND
 *                  
 ****************************************************************************/
void uecc_fsm_ue_s1u_end_marker_ind_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
)
{
    U32 erab_id = 0;
    rrc_bool_et send_req = RRC_TRUE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_INFO, "Recived UECC_EV_LLIM_S1U_END_MARKER_IND");
    
    if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    { 
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
               p_ue_context->p_gb_context->facility_name,
               RRC_WARNING,"Invalid event received");
        return;
    }
    
    erab_id = get_erab_id_from_lc_id(p_ue_context,(
                (rrc_uecc_llim_s1u_end_marker_ind_t*)p_api)->lc_id);

    if (erab_id == INVALID_ERAB_ID)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "End marker Ind received for Invalid ERAB Id");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    if(((rrc_uecc_llim_s1u_end_marker_ind_t*)p_api)->teid_self
                == p_ue_context->p_e_rab_list[erab_id]->teid_self)
    {
        if(((rrc_uecc_llim_s1u_end_marker_ind_t*)p_api)->bitmask
                & UECC_LLIM_S1U_END_MARKER_TRANSPORT_ADDR_PRESENT) 
        {
            if(!memcmp_wrapper(((rrc_uecc_llim_s1u_end_marker_ind_t*)p_api)->transport_addr.data,
                        p_ue_context->p_e_rab_list[erab_id]->transportLayerAddress_self.data,
                        p_ue_context->p_e_rab_list[erab_id]->transportLayerAddress_self.numbits/8))
            {
                send_req = RRC_FALSE;
            }
        }
        else
        {
            send_req = RRC_FALSE;
        }
    }
    if(RRC_TRUE == send_req )
    {
        if (RRC_SUCCESS !=
                uecc_delete_fwd_tunnels_llim_build_and_send_s1u_reconfigure_req(
                    (rrc_uecc_llim_s1u_end_marker_ind_t*)p_api, p_ue_context))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[uecc_fsm_ue_s1u_end_marker_ind_handler]"
                    "Delivery failed");
        }
    }

    else
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "END_MARKER_IND received for non Forwarding tunnel");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1u_reconfig_cnf_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM
 *                  receives the UECC_EV_LLIM_S1U_RECONFIG_CNF in 
 *                  UECC_UE_CONNECTED state.
 *                  
 ****************************************************************************/
void uecc_fsm_ue_s1u_reconfig_cnf_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,RRC_INFO, 
            "[UECC_EV_LLIM_S1U_RECONFIG_CNF] "
            "received s1u reconfig confirmation; Unused variable p_api=%p",p_api);/*SPR 17777 +-*/ 

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_ho_required_failure_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRM_HO_REQUIRED message in 
 *                  unexpected state.
 ****************************************************************************/
void uecc_fsm_ue_rrm_ho_required_failure_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
            "RRM HO REQ received in unexpected state.");
/*SPR 18758 Fix Start*/
    p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->ho_info.cause.u.radioNetwork = 
            s1ap_interaction_with_other_procedure;
/*SPR 18758 Fix Stop*/

    if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_failure( p_ue_context))
    {
        RRC_UECC_UE_FSM_TRACE(RRC_ERROR,
                "Fail to send the handover failure indication to RRM");
    }
    /* SPR 24021/SES-452 Fix Start */
    if(PNULL != p_api)
    {
        rrc_mem_free(p_api);
        p_api = PNULL;
    }
    /* SPR 24021/SES-452 Fix End */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_ho_required_src_ho_ongoing_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  RRC_RRM_UE_HO_REQUIRED/UECC_EV_HO_REQUIRED_INT
 *                  message in UECC_UE_SRC_HO_ONGOING state.
 ****************************************************************************/
void uecc_fsm_ue_rrm_ho_required_src_ho_ongoing_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
            "[uecc_fsm_ue_rrm_ho_required_src_ho_ongoing_handler]"
            "Dropping RRM HO REQ message as already handling one HO request;Unused variable p_api=%p",p_api);/*SPR 17777 +-*/

    /* SPR 24021/SES-452 Fix Start */
    if(PNULL != p_api)
    {
        rrc_mem_free(p_api);
        p_api = PNULL;
    }
    /* SPR 24021/SES-452 Fix End */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/*SPR 18758 Fix Start*/
/****************************************************************************
 * Function Name  : uecc_fsm_ue_proc_ongoing_erab_setup_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Send ERAB SETUP RESPONSE containing all failure while
 *                  Connection Reestablishment procedure is ongoing.
 ****************************************************************************/
void uecc_fsm_ue_proc_ongoing_erab_setup_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_s1ap_erab_setup_request_t   *p_msg = (rrc_s1ap_erab_setup_request_t *)p_api;
    OSRTDListNode*                  p_node = PNULL;
    U8                              fail_list_count = 0;
    s1ap_E_RABToBeSetupListBearerSUReq  *p_e_rab_to_be_setup_list = &p_msg->e_rab_to_be_setup_list;
    s1ap_E_RABToBeSetupListBearerSUReq_element  *p_erab_elem = PNULL;
    uecc_erab_failed_to_setup_list_t            *p_failed_erab_data = PNULL;
    /*SPR_18582_FIX_START*/
    U8                              index = RRC_NULL;
    /*SPR_18582_FIX_END*/
    /* SES-521 Changes - Starts */
    rrc_bool_et                     free_curr_proc = RRC_FALSE;
    /* SES-521 Changes - Ends */

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* SES-521 Changes - Starts */
    /* Check Curr Proc Data, if not allocated then
     * Allocate and Free after sending appropriate response
     */
    if(PNULL == p_ue_context->p_curr_proc_data)
    {
        p_ue_context->p_curr_proc_data =
            (curr_proc_data_t *)rrc_mem_get(sizeof(curr_proc_data_t));
        free_curr_proc = RRC_TRUE;
        RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Allocated for Current PROC data.");
    }
    /* SES-521 Changes - Ends */
    do
    {
        /* SES-521 Changes - Starts */
        if (PNULL != p_ue_context->p_curr_proc_data)
        /* SES-521 Changes - Ends */
        {
            p_failed_erab_data = &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.failed_erab_data;

            /* Find the mutiple occurences of ERAB IDs in temp_erab_list array */
            /*SPR_18582_FIX_START*/
            for(index = RRC_NULL;index < MAX_ERAB_COUNT;index++)
            {
                p_failed_erab_data->erab_failed_to_setup_item[index].e_RAB_ID = INVALID_ERAB_ID;
            }
            for (p_node = p_e_rab_to_be_setup_list->head;
                (PNULL != p_node) &&
                (p_e_rab_to_be_setup_list->count < MAX_MME_ERAB_LIST_COUNT) &&
                (fail_list_count < MAX_ERAB_COUNT);
                p_node=p_node->next )
            /*SPR_18582_FIX_END*/
            {
                p_erab_elem=(s1ap_E_RABToBeSetupListBearerSUReq_element*)p_node->data;
                if ((p_erab_elem == PNULL) || ( p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1 == PNULL))
                {
                    continue;
                }
                /*SPR_18582_FIX_START*/
                if (RRC_SUCCESS == check_if_erab_id_already_present_in_failed_list(
                                p_failed_erab_data->erab_failed_to_setup_item,
                                p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID,
                                p_ue_context))
                {
                    RRC_UECC_UE_FSM_TRACE(RRC_INFO,
                        "Duplicate occurance, E_RAB ID [%u] already present in failed list",
                        p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID);
                    continue;
                }
                /*SPR_18582_FIX_END*/

                p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].e_RAB_ID =
                    p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID;
                p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].cause.t =
                    T_s1ap_Cause_radioNetwork;
                p_failed_erab_data->erab_failed_to_setup_item[fail_list_count++].cause.u.radioNetwork =
                    s1ap_interaction_with_other_procedure;

                RRC_UECC_UE_FSM_TRACE(RRC_ERROR,
                    "E_RAB ID Put in Failed List : %u",
                    p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID);
            }
            p_failed_erab_data->count = fail_list_count;

            /* Send ASN.1 Encoded ERAB SETUP Response to MME */
            if(RRC_SUCCESS !=
                    uecc_s1ap_build_and_send_erab_setup_response(p_ue_context))
            {
                RRC_UECC_UE_FSM_TRACE(RRC_FATAL,
                    "[s1ap_E_RABSetupResponse] delivery failure");
            }

            /* SES-521 Changes - Starts */
            /* Free memory for curr_proc_data_t, only if allocated here */
            if(RRC_TRUE == free_curr_proc)
            /* SES-521 Changes - Ends */
            {
                rrc_mem_free(p_ue_context->p_curr_proc_data);
                p_ue_context->p_curr_proc_data = PNULL;
                RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Free for Current PROC data.");
            }
        }
        /* SES-521 Changes - Starts */
        else
        {
            RRC_UECC_UE_FSM_TRACE(RRC_FATAL, "p_curr_proc_data is NULL");
        }
        /* SES-521 Changes - Ends */
    }while(0);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_proc_ongoing_erab_modify_send_failure_response_to_mme
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * RETURNS       : void
 * DESCRIPTION   : This action routine is called when Connection Reestablishment 
 *                 procedure is ongoing and UECC_EV_S1AP_ERAB_MODIFY_REQ message
 *                 is received.
 ****************************************************************************/
void uecc_fsm_ue_proc_ongoing_erab_modify_send_failure_response_to_mme(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_s1ap_erab_modify_request_t  *p_msg;
    U16                             e_rab_list_count = RRC_NULL;
    /*SPR_18582_FIX_START*/
    /* code removed */
    U8                              index = RRC_NULL;
    /*SPR_18582_FIX_END*/
    /* SES-521 Changes - Starts */
    rrc_bool_et                     free_curr_proc = RRC_FALSE;
    /* SES-521 Changes - Ends */

    s1ap_E_RABToBeModifiedItemBearerModReq  *p_e_rab_list[MAX_MME_ERAB_LIST_COUNT] = {PNULL};
    OSRTDListNode                           *p_node = PNULL;
    s1ap_E_RABToBeModifiedListBearerModReq_element  *p_erab_elem = PNULL;
    erab_failed_to_modify_list_t                    *p_failed_modify_list = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    p_msg = (rrc_s1ap_erab_modify_request_t*)p_api;

    /* SES-521 Changes - Starts */
    /* Check Curr Proc Data, if not allocated then
     * Allocate and Free after sending appropriate response
     */
    if(PNULL == p_ue_context->p_curr_proc_data)
    {
        p_ue_context->p_curr_proc_data =
            (curr_proc_data_t *)rrc_mem_get(sizeof(curr_proc_data_t));
        free_curr_proc = RRC_TRUE;
        RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Allocated for Current PROC data.");
    }
    if (PNULL != p_ue_context->p_curr_proc_data)
    /* SES-521 Changes - Ends */
    {
        p_failed_modify_list = &(p_ue_context->p_curr_proc_data->u.
            erab_modify_request_data.erab_failed_to_modify_list);
        /*SPR_18582_FIX_START*/
        for(index = RRC_NULL;index < MAX_ERAB_COUNT;index++)
        {
            p_failed_modify_list->erab_failed_to_modify_item[index].e_RAB_ID = INVALID_ERAB_ID;
        }

        for (p_node = p_msg->e_rab_to_be_modified_list.head;
            (PNULL!=p_node) && (e_rab_list_count < MAX_MME_ERAB_LIST_COUNT) &&
            (p_failed_modify_list->erab_failed_to_modify_list_counter < MAX_ERAB_COUNT);
            p_node = p_node->next,e_rab_list_count++)
        /*SPR_18582_FIX_END*/
        {
            p_erab_elem = (s1ap_E_RABToBeModifiedListBearerModReq_element*)p_node->data;
            if ((p_erab_elem == PNULL) || (p_erab_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1 == PNULL))
            {
                RRC_UECC_UE_FSM_TRACE(RRC_WARNING, "Received RABToBeModifiedListBearerModReq_element null.");
                break;
            }
            /* check for valid IDs was performed before when decoding */
            p_e_rab_list[e_rab_list_count] = p_erab_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1;
            /*SPR_18582_FIX_START*/
            if (RRC_SUCCESS == check_if_erab_id_already_present_in_failed_list(
                        p_failed_modify_list->erab_failed_to_modify_item,
                        p_e_rab_list[e_rab_list_count]->e_RAB_ID, p_ue_context))
            {
                RRC_UECC_UE_FSM_TRACE(RRC_INFO,
                    "Duplicate occurance, E_RAB ID [%u] already present in failed list",
                    p_e_rab_list[e_rab_list_count]->e_RAB_ID);
                continue;
            }
            /* check for valid IDs was performed before when decoding */
            p_failed_modify_list->erab_failed_to_modify_item
                [p_failed_modify_list->erab_failed_to_modify_list_counter].e_RAB_ID =
                p_e_rab_list[e_rab_list_count]->e_RAB_ID;
            /* Update the cause. */
            p_failed_modify_list->erab_failed_to_modify_item
                [p_failed_modify_list->erab_failed_to_modify_list_counter].cause.t = T_s1ap_Cause_radioNetwork;
            p_failed_modify_list->erab_failed_to_modify_item[p_failed_modify_list->erab_failed_to_modify_list_counter].
                cause.u.radioNetwork = s1ap_interaction_with_other_procedure;
            p_failed_modify_list->erab_failed_to_modify_item
                [p_failed_modify_list->erab_failed_to_modify_list_counter].error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;
            /*  Update the failed list counter */
            p_failed_modify_list->erab_failed_to_modify_list_counter++;
        }
        /*SPR_18582_FIX_END*/

        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
        {
            RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                    "[s1ap_E_RABModifyResponse] Delivery failure");
        }
        /* SES-521 Changes - Starts */
        /* Free memory for curr_proc_data_t, only if allocated here */
        if(RRC_TRUE == free_curr_proc)
        /* SES-521 Changes - Ends */
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data);
            p_ue_context->p_curr_proc_data = PNULL;
            RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Free for Current PROC data.");
        }
    }
    /* SES-521 Changes - Starts */
    else
    {
        RRC_UECC_UE_FSM_TRACE(RRC_FATAL, "p_curr_proc_data is NULL");
    }
    /* SES-521 Changes - Ends */
}
/*SPR 18758 Fix Stop*/

/****************************************************************************
 * Function Name  : uecc_fsm_cre_ongoing_ue_rrm_ue_connection_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  RRC_RRM_UE_CONNECTION_RELEASE_IND message during CRE Proc.
 ****************************************************************************/
void uecc_fsm_cre_ongoing_ue_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* SPR 17781 Fix Start */
    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_UE_CRE_TIMER,
            &p_ue_context->timers_data);

    if (PNULL != p_ue_context->p_reestab_ue_identity)
			{
        rrc_mem_free(p_ue_context->p_reestab_ue_identity);
        p_ue_context->p_reestab_ue_identity = PNULL;
		}

    /* Send Re-establishment Reject Message if Re-establishment Complete 
       has not received from UE */
    if (RRC_TRUE == p_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag)
		{
        if (RRC_SUCCESS !=
                uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                    p_ue_context->p_gb_context, p_ue_context->crnti, 0,
                    p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
		{
			RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_ERROR,
                    "Fail to send the RRCConnectionReestablishmentReject to UE");
	}
	else
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_INFO,
                    "RRCConnectionReestablishmentReject MSG SEND SUCCESSFULLY");
	}
	}

    /* Finish Current procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);

    /* Change UE Main FSM State to UE CONNECTED */
    UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);

    uecc_fsm_ue_rrm_ue_connection_release_ind_handler(p_ue_context, p_api);
     
    /* SPR 17781 Fix Stop */

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR_17348_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_ho_ue_rrm_ue_connection_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  RRC_RRM_UE_CONNECTION_RELEASE_IND message during ongoing HO 
 *                  at Source eNB.
 ****************************************************************************/
void uecc_fsm_ho_ue_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    /* Code Removed */

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag
        = RRC_TRUE;

    if ((PNULL != p_ue_context->ho_info.p_ho_info) &&
            (PNULL != p_ue_context->ho_info.p_ho_info->p_src_ho_info) &&
            (RRC_FALSE == p_ue_context->ho_info.p_ho_info->p_src_ho_info->
             ho_command_sent_to_ue))
    {
        /*If HO Preparation has been sent but HO Command has not been
         * sent to Ue */
/*SPR_19066_START*/
        if((&uecc_fsm_src_ho_get_ho_prep_info_ongoing
/*SPR_19066_END*/
                    != p_ue_context->uecc_fsm.proc_descriptor.p_current_state))
        {
            if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                    (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
                    (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,RRC_INFO,
                        "Send HO CANCEL to X2AP");

                p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;

                if (RRC_SUCCESS != uecc_x2ap_build_and_send_handover_cancel(p_ue_context))
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,RRC_ERROR,
                            "Fail to send the handover cancel to X2AP");
                }
            }
        }
        if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
        {
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);
            if ((INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) &&
                    (RRC_TRUE == p_ue_context->ue_ctx_mod_security_flag))
            {
                p_ue_context->ue_ctx_mod_security_flag = RRC_FALSE;
                /* FREE p_curr_proc_data */
                if ((PNULL != p_ue_context->p_curr_proc_data) &&
                        (p_ue_context->p_curr_proc_data->m.SecurityCapability_present))
                {
                    /* Freed if any message is buffered */
                    if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
                    {
                        rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
                        p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
                    }

                    if (PNULL != p_ue_context->p_curr_proc_data->p_registered_lai)
                    {
                        rrc_mem_free(p_ue_context->p_curr_proc_data->p_registered_lai);
                        p_ue_context->p_curr_proc_data->p_registered_lai = PNULL;
                    }

                    rrc_mem_free(p_ue_context->p_curr_proc_data);
                    p_ue_context->p_curr_proc_data = PNULL;
                }
            }
        }
        else if(S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover) 
        {
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);
        }
        else if (CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
            uecc_ue_timer_stop( p_ue_context->p_gb_context,
                    UECC_UE_CCO_GUARD_TIMER,
                    &p_ue_context->timers_data );
        }
    }
    else
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_ERROR,
                "HO CMD already sent to UE. Ignoring RRC_RRM_UE_CONNECTION_RELEASE_IND");
    }
    /*SPR_17348_END*/

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ho_ongoing_erab_rel_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when Handover procedure is ongoing and
 *                  UECC_EV_RRM_ERAB_RELEASE_IND message is received
 ****************************************************************************/
void uecc_fsm_ho_ongoing_erab_rel_ind_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    erab_list_t*                p_erab_failed_list = PNULL;
    rrc_rrm_erb_release_ind_t*  p_msg = PNULL;
    U8                          e_rab_index = RRC_NULL;
    U32                         error_code = RRC_NULL;
    /* SES-521 Changes - Starts */
    rrc_bool_et                 free_curr_proc = RRC_FALSE;
    /* SES-521 Changes - Ends */

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* SES-521 Changes - Starts */
    /* Check Curr Proc Data, if not allocated then
     * Allocate and Free after sending appropriate response
     */
    if(PNULL == p_ue_context->p_curr_proc_data)
    {
        p_ue_context->p_curr_proc_data =
            (curr_proc_data_t *)rrc_mem_get(sizeof(curr_proc_data_t));
        free_curr_proc = RRC_TRUE;
        RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Allocated for Current PROC data.");
    }

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Allocate memory for RRM initiated erab_rel_ind */
        p_msg = (rrc_rrm_erb_release_ind_t*)p_api;
        /* points to erab_failed_to_release_list */
        p_erab_failed_list = &p_ue_context->p_curr_proc_data->u.
            erab_release_command_data.erab_failed_to_release_list;

        error_code |= RRC_MODULE_ID << 16;
        error_code |= RRC_HANDOVER_TRIGGERED;

        for(e_rab_index = RRC_NULL;
            e_rab_index < p_msg->erab_to_be_released_item_list.count;
            e_rab_index++)
        {
            p_erab_failed_list->erab_item[e_rab_index].e_RAB_ID =
                p_msg->erab_to_be_released_item_list.erab_to_be_release_item[e_rab_index].erab_id;
            p_erab_failed_list->erab_item[e_rab_index].error_code =
                error_code;
            p_erab_failed_list->counter++;
        }

        /* Build and send RRC_RRM_ERB_RELEASE_CNF with FAILURE to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                    p_ue_context, FAILURE))
        {
            RRC_UECC_UE_FSM_TRACE(RRC_FATAL,
                "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }

        /* Free memory for curr_proc_data_t, only if allocated here */
        if(RRC_TRUE == free_curr_proc)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data);
            p_ue_context->p_curr_proc_data = PNULL;
            RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Free for Current PROC data.");
        }
    }
    else
    {
        RRC_UECC_UE_FSM_TRACE(RRC_FATAL, "p_curr_proc_data is NULL");
    }
    /* SES-521 Changes - Ends */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_s1ap_ho_cancel_ack_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when  HO CANCEL ACK is received
 *
 ****************************************************************************/
void uecc_fsm_s1ap_ho_cancel_ack_handler(
		uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
		void*                p_api              /* Input API buffer */
		)
{
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

        /*SPR 17777 +-*/
	RRC_UECC_UE_FSM_TRACE(RRC_INFO,
			"HO CANCEL ACK received;Unused variable p_api=%p",p_api);
        /*SPR 17777 +-*/
}



/****************************************************************************
 **  Function Name  : uecc_fsm_ue_wait_for_ics_mme_guard_timer_expiry_handler
 **  Inputs         : uecc_ue_context_t* - pointer to UE context
 **                   void*   - api data
 **  Outputs        : None
 **  Returns        : None
 **  Description    : This action routine is called when UE Main FSM receives the
 **                   UECC_EV_MME_GUARD_TIMER_EXPIRY message
 **                   in UECC_UE_CONNECTION_SETUP_ONGOING state
 ******************************************************************************/
void uecc_fsm_ue_wait_for_ics_mme_guard_timer_expiry_handler(
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{

	rrc_bool_et mme_release_required = RRC_TRUE;

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);
	RRC_SET_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
    /* SPR 18363 Fix Start */
    if (RRC_NULL == p_ue_context->p_gb_context->
            mme_ctx[p_ue_context->mme_id].mme_connection_status)
    {
        mme_release_required = RRC_FALSE;
    }
    /* SPR 18363 Fix End */
	if (RRC_SUCCESS != uecc_s1ap_build_and_send_ue_release_msg_req(
				p_ue_context->s1ap_ue_id,
				mme_release_required,
				p_ue_context))
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
				p_ue_context->p_gb_context->facility_name,RRC_WARNING,
				"ue_release_msg delivery failure");
	}

	/* SRB1 is established - release connection */
	p_ue_context->share_data.uecc_rcr_data.u.cause.t =
		T_s1ap_Cause_radioNetwork;
	p_ue_context->share_data.uecc_rcr_data.u.cause.u.radioNetwork =
		s1ap_failure_in_radio_interface_procedure;

	UECC_FSM_SET_MAIN_STATE(
			p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

	uecc_fsm_start_procedure(
			p_ue_context,
			&uecc_fsm_ue_connection_release_setup_failure_procedure, PNULL);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_connection_reestablish_success_int_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when Ue Connection Re-establishment 
 *                  procedure finishes successfully and
 *                  UECC_EV_UE_CONNECTION_REESTABLISH_SUCCESS_INT message is received
 ****************************************************************************/
void uecc_fsm_ue_connection_reestablish_success_int_handler(
		uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
		void*                p_api              /* Input API buffer */
		)
{
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*X2AP FIX_HO_5 : Start*/
/* SPR 16062 Start */
/****************************************************************************
 * Function Name  : uecc_fsm_ho_ongoing_ue_connection_reestablish_success_int_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when Ue Connection Re-establishment 
 *                  procedure finishes successfully and
 *                  UECC_EV_HO_ONGOING_UE_CONNECTION_REESTABLISH_SUCCESS_INT message is received
 ****************************************************************************/
void uecc_fsm_ho_ongoing_ue_connection_reestablish_success_int_handler(
/* SPR 16062 Stop */
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
)
{
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	if( //PNULL!= p_ue_context->prev_ongoing_uecc_fsm.proc_descriptor &&
			PNULL != p_ue_context->prev_ongoing_uecc_fsm.p_current_state )
	{ 

		UECC_FSM_SET_MAIN_STATE(p_ue_context, 
				p_ue_context->prev_ongoing_uecc_fsm.p_current_state, 
				PNULL);

		uecc_fsm_start_procedure(
				p_ue_context,
				&uecc_fsm_reest_success_ue_trg_ho_procedure, PNULL);
	}
	else
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
				p_ue_context->p_gb_context->facility_name,
				RRC_WARNING,"FSM Info for the preempted procedure to"
				"be resumed is absent in UE Context "); 
	}
	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*X2AP FIX_HO_5 : End*/

/****************************************************************************
 * Function Name  : uecc_fsm_ue_connection_reestablish_failure_int_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when Ue Connection Re-establishment 
 *                  procedure finishes with failure and
 *                  UECC_EV_UE_CONNECTION_REESTABLISH_FAILURE_INT message is received
 ****************************************************************************/
void uecc_fsm_ue_connection_reestablish_failure_int_handler(
		uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
		void*                p_api              /* Input API buffer */
		)
{
	s1ap_Cause cause;
	RRC_ASSERT(PNULL != p_ue_context);

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	cause.t= T_s1ap_Cause_radioNetwork;
    /*SPR 14530 Fix Start*/
    cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;
    /*SPR 14530 Fix Start*/

	/* Bug 793 Fix Start */
	if ( (X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
			(INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ) 
	{
		SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_ue_context);
		UECC_FSM_SET_MAIN_STATE(p_ue_context,
				&uecc_fsm_ue_connection_release_ongoing, PNULL);

		uecc_fsm_start_procedure(p_ue_context,
				&uecc_fsm_ue_connection_release_procedure, PNULL);

	}
	else
	{
		if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
					p_ue_context,
					&cause ))
		{
			RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
					p_ue_context->p_gb_context->facility_name,RRC_WARNING,
					"[s1ap_UEContextReleaseRequest] Delivery failed");

		}

		if ( RRC_SUCCESS != uecc_ue_timer_start(
					p_ue_context->p_gb_context,
					UECC_UE_MME_GUARD_TIMER,
					&p_ue_context->p_gb_context->ue_timers_data,
					&p_ue_context->timers_data,
					p_ue_context->ue_index) )
		{
			RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
					p_ue_context->p_gb_context->facility_name,RRC_WARNING,
					"[UECC_UE_MME_GUARD_TIMER]"
					"Timer Start Failed");
		}

		UECC_FSM_SET_MAIN_STATE(
				p_ue_context, &uecc_fsm_ue_wait_for_s1ap_ue_ctx_rel_cmd, PNULL);
	}
	/* Bug 793 Fix Stop */

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_overload_ue_release_ind_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when overload ind is received from S1AP 
 ****************************************************************************/
void uecc_fsm_ue_s1ap_overload_ue_release_ind_handler(
		uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
		void*                p_api              /* Input API buffer */
		)
{
	s1ap_overload_msg_ind_t* p_s1ap_overload_msg_ind = PNULL;

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	RRC_UECC_UE_FSM_TRACE(RRC_INFO,
			"Processing OVERLOAD INDICATION");

	do
	{
		p_s1ap_overload_msg_ind = (s1ap_overload_msg_ind_t *)p_api;

		/* If p_s1ap_overload_msg_ind is NULL fill the default
		 * values and continue the release process
		 */
		if (PNULL != p_s1ap_overload_msg_ind)
		{
			p_ue_context->share_data.uecc_rcr_data.u.cause.t = 
				p_s1ap_overload_msg_ind->cause.t;
			p_ue_context->share_data.uecc_rcr_data.u.cause.u = 
				p_s1ap_overload_msg_ind->cause.u;
		}
		else
		{
			RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
					p_ue_context->p_gb_context->facility_name, 
					RRC_WARNING, "p_s1ap_overload_msg_ind is NULL"
					", Filling default values");

			/* Fill some default values and continue */
			p_ue_context->share_data.uecc_rcr_data.u.cause.t = 
				T_s1ap_Cause_misc;
			p_ue_context->share_data.uecc_rcr_data.u.cause.u.misc = 
				s1ap_control_processing_overload; 
		}

		UECC_FSM_SET_MAIN_STATE(
				p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

		uecc_fsm_start_procedure(
				p_ue_context,
				&uecc_fsm_overload_ue_connection_release_setup_failure_procedure, PNULL);
	}while(0);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
	return;
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_s1ap_erab_setup_rollback_failure_rel_ue_ctx_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_S1AP_ERAB_SETUP_FAILURE_REL_UE_CTX_INT message
 ****************************************************************************/
void uecc_fsm_ue_s1ap_erab_setup_rollback_failure_rel_ue_ctx_handler(
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	/* update statistic */
	/* UF: calculate number of successful DRB in future here */
	uecc_status_ue_update_num_of_drb(p_ue_context,1);

	/* Free memory for curr_proc_data_t */
	if ( PNULL != p_ue_context->p_curr_proc_data )
	{
		rrc_mem_free(p_ue_context->p_curr_proc_data);
		p_ue_context->p_curr_proc_data = PNULL;
	}

	/* Start MME GURAD TIMER */
	if (RRC_SUCCESS != uecc_ue_timer_start(
				p_ue_context->p_gb_context,
				UECC_UE_MME_GUARD_TIMER,
				&p_ue_context->p_gb_context->ue_timers_data,
				&p_ue_context->timers_data,
				p_ue_context->ue_index))
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
				p_ue_context->p_gb_context->facility_name,RRC_WARNING,
				"Failure to start MME Guard Timer.");
	}

	/* Change UE Main FSM State to UECC_UE_WAIT_FOR_CONNECTION_RELEASE */
	UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_wait_for_s1ap_ue_ctx_rel_cmd, PNULL);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/* CBSR Changes Start */

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_validate_antenna_codebook_subset_restriction
 * Inputs         : rrc_phy_antenna_information_dedicated_t - pointer to antenna
 *                   info
 *                  uecc_ue_context_t   *p_ue_context
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function validates codebook subset restriction
 *******************************************************************************/
rrc_return_t uecc_fsm_validate_antenna_codebook_subset_restriction(
		rrc_phy_antenna_information_dedicated_t *p_antenna_info,
		uecc_ue_context_t   *p_ue_context      /* UECC UE context */
		)
{
	rrc_return_t result = RRC_SUCCESS;
	U8 transmission_mode = p_antenna_info->transmission_mode;

	if (transmission_mode >= TRANSMISSION_MODE_TM3 &&
			transmission_mode <= TRANSMISSION_MODE_TM6)
	{
		/* Codebook Subset Restriction info must be present in case of tm3, tm4, tm5, tm6 */
		if (!(p_antenna_info->bitmask &
					RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT))
		{
			RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
					p_ue_context->p_gb_context->facility_name,RRC_ERROR,
					"CODE SUBSET RESTRICT MUST BE PRESENT"
					" TRANSMISSION MODE = %u", transmission_mode);
			return RRC_FAILURE;
		}

	}
	else
	{
		if (p_antenna_info->bitmask &
				RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
		{
			RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
					p_ue_context->p_gb_context->facility_name,RRC_INFO,
					"CODEBOOK SUBSET RESTRICTION PRESENT in case of other TM's"
					" Now Resetting this value.");
			p_antenna_info->bitmask ^=
				RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT;
		}

	}

	return result;

}

/* CBSR Changes Stop */

/*Bug_12240_fix_start */

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_validate_antenna_codebook_subset_restriction
 * Inputs         : rrc_antenna_info_dedicated_r10_t - pointer to antenna
 *                   info
 *                  uecc_ue_context_t   *p_ue_context
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function validates codebook subset restriction for r10
 *******************************************************************************/
rrc_return_t uecc_fsm_validate_antenna_codebook_subset_restriction_r10(
		rrc_antenna_info_dedicated_r10_t *p_antenna_info,
		uecc_ue_context_t   *p_ue_context      /* UECC UE context */
		)
{
	rrc_return_t result = RRC_SUCCESS;
	U8 transmission_mode = p_antenna_info->transmission_mode;

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	if ((transmission_mode >= RRM_RRC_TM3 &&
				transmission_mode <= RRM_RRC_TM6) ||
			(transmission_mode == RRM_RRC_TM8_V920) ||
			(transmission_mode == RRM_RRC_TM9_V1020))
	{
		/* Codebook Subset Restriction info must be present in case of tm3, tm4, tm5, tm6 */
		if (!(p_antenna_info->bitmask &
					RRM_CODEBOOK_SUBSET_RESTRICTION_PRESENT))
		{
			RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
					p_ue_context->p_gb_context->facility_name,RRC_ERROR,
					"CODE SUBSET RESTRICT MUST BE PRESENT"
					" TRANSMISSION MODE = %u", transmission_mode);
			return RRC_FAILURE;
		}

	}
	else
	{
		if (p_antenna_info->bitmask &
				RRM_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
		{
			RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
					p_ue_context->p_gb_context->facility_name,RRC_INFO,
					"CODEBOOK SUBSET RESTRICTION PRESENT in case of other TM's"
					" Now Resetting this value.");
			p_antenna_info->bitmask ^=
				RRM_CODEBOOK_SUBSET_RESTRICTION_PRESENT;
		}

	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

	return result;

}

/* Bug_12240_fix_stop */

/*****************************************************************************
 * Function Name  : fill_s1ap_fail_cause 
 * Inputs         : uecc_ue_context_t -  *p_ue_context
 *                  U32 *p_fail_cause
 * Outputs        : None
 * Returns        : None
 * Description    : THis function fills s1ap failure cause
 ********************************************************************************/
void fill_s1ap_fail_cause(uecc_ue_context_t   *p_ue_context,
		U32 *p_fail_cause)
{
	S8 *p_proc_id = PNULL;

	if ( PNULL != p_ue_context->uecc_fsm.proc_descriptor.
			p_active_proc)
	{
		p_proc_id = p_ue_context->uecc_fsm.proc_descriptor.
			p_active_proc->s_id;
	}

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	if ( PNULL != p_proc_id )
	{
		if (!strcmp_wrapper(p_proc_id,
					(const S8 *)("UE_SRC_HO_P")))
		{
			if ( (HANDOVER_TYPE_INTRA_LTE_S1 ==
						p_ue_context->ho_info.p_ho_info->p_src_ho_info
						->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) ||
					( p_ue_context->ho_info.s1_or_x2_handover ==
					  INTER_CELL_HO) )
			{
				*p_fail_cause = s1ap_s1_intra_system_handover_triggered;
			}
			else if ( p_ue_context->ho_info.s1_or_x2_handover == X2_HO) 
			{
				*p_fail_cause = s1ap_x2_handover_triggered;
			}
			else if ( INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover )
			{
				*p_fail_cause = s1ap_interaction_with_other_procedure;
			}
			else
			{
				*p_fail_cause = s1ap_s1_inter_system_handover_triggered;
			}
		}
		else if (!strcmp_wrapper(p_proc_id,
					(const S8 *)("UE_TRG_HO_P")))
		{
			if ( X2_HO == p_ue_context->ho_info.s1_or_x2_handover )
			{
				*p_fail_cause = s1ap_x2_handover_triggered;
			}
			else if ((s1ap_intralte ==
						p_ue_context->ho_info.p_ho_info->p_trg_ho_info->
						rrc_s1ap_ho_request.handover_type) ||
					(INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
			{
				*p_fail_cause = s1ap_s1_intra_system_handover_triggered;
			}
			else if ( INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover )
			{
				*p_fail_cause = s1ap_interaction_with_other_procedure;
			}
			else
			{
				*p_fail_cause = s1ap_s1_inter_system_handover_triggered;
			}
		}
		else if (!strcmp_wrapper(p_proc_id,
					(const S8 *)("UE_RLF_A")))
		{
			*p_fail_cause = s1ap_radio_connection_with_ue_lost;
		}
		else if (!strcmp_wrapper(p_proc_id,
					(const S8 *)("UE_CONTXTMOD_P")))
		{
			*p_fail_cause = s1ap_interaction_with_other_procedure;
		}
		else 
		{
			*p_fail_cause = s1ap_interaction_with_other_procedure;
		}
	}
	else 
	{
		*p_fail_cause = s1ap_interaction_with_other_procedure;
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_rel_req_int_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  S1AP_UE_REL_REQ_INT message
 *                  in uecc_fsm_ue_wait_for_s1_connection state
 ****************************************************************************/
void uecc_fsm_ue_rel_req_int_handler(
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);
	/*SPR 3840 Start*/
	uecc_fsm_finish_active_procedure(p_ue_context);
	/*SPR 3840 End*/

	/* Stop MME_GUARD_TIMER */
	if (PNULL !=
			p_ue_context->timers_data.timers_ids[UECC_UE_MME_GUARD_TIMER])
	{
		uecc_ue_timer_stop(p_ue_context->p_gb_context,
				UECC_UE_MME_GUARD_TIMER, &p_ue_context->timers_data);
	}           

	UECC_FSM_SET_MAIN_STATE(
			p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

	uecc_fsm_start_procedure(
			p_ue_context,
			&uecc_fsm_ue_connection_release_setup_failure_procedure, PNULL);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* SPR 4118 Start */
/*****************************************************************************
 * Function Name  : uecc_fsm_ue_notify_integrity_failure_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles UECC_UE_INTEGRITY_FAILURE 
 ********************************************************************************/
void uecc_fsm_ue_notify_integrity_failure_handler(
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{
	LOCAL_INTEGRITY_VER_FAIL_RRC_MSG_STRUCT *p_local_integrity_ver_fail = PNULL;

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_ue_context->p_gb_context);

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);
	p_local_integrity_ver_fail = rrc_mem_get(sizeof(LOCAL_INTEGRITY_VER_FAIL_RRC_MSG_STRUCT));
	if (PNULL != p_local_integrity_ver_fail)
	{
		memset_wrapper(p_local_integrity_ver_fail, RRC_NULL, 
				sizeof(LOCAL_INTEGRITY_VER_FAIL_RRC_MSG_STRUCT));
		p_local_integrity_ver_fail->header.length = sizeof(LOCAL_INTEGRITY_VER_FAIL_RRC_MSG_STRUCT);
		p_local_integrity_ver_fail->header.event_id = LOCAL_INTEGRITY_VER_FAIL_RRC_MSG;

		if ((PNULL != p_ue_context->p_gb_context->p_p_csc_context) && 
				(PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index])
				&& (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
					p_csc_init_setup_ind))
		{
			l3_memcpy_wrapper(&p_local_integrity_ver_fail->header.EVENT_PARAM_CELL_ID, 
					p_ue_context->p_gb_context->
					p_p_csc_context[p_ue_context->cell_index]->
					p_csc_init_setup_ind->sib_type_1_Info.
					cell_access_related_info.cell_Id, sizeof(U32));
		}
		p_local_integrity_ver_fail->header.EVENT_PARAM_EUTRANCELL_FROID = 
			p_local_integrity_ver_fail->header.EVENT_PARAM_CELL_ID;

		if (p_ue_context->m.mme_ue_id_present)
		{
			p_local_integrity_ver_fail->header.EVENT_PARAM_MMES1APID = p_ue_context->mme_ue_id;
		}

		p_local_integrity_ver_fail->header.EVENT_PARAM_RAC_UE_REF = p_ue_context->s1ap_ue_id;

		rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_integrity_ver_fail);
	}

	/*SPR_17143_START*/
	/* Log the Integrity Failure Received and ignore it */
	RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
			p_ue_context->p_gb_context->facility_name,RRC_WARNING,
			"RRC_MSG: Integrity Failure Received for UE=%u, ignore", p_ue_context->ue_index);
	/*SPR_17143_END*/

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR_17143_START*/
/*Code Removed*/
/*SPR_17143_END*/
/* SPR 4118 Stop */
/****************************************************************************
 * Function Name  : uecc_fsm_ue_src_ho_success_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                   UECC_EV_SRC_HO_SUCCESS_INT in UECC_UE_SRC_HO_ONGOING state.
 ****************************************************************************/
void uecc_fsm_ue_src_ho_success_handler(
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{

	RRC_ASSERT(PNULL != p_ue_context);

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	if(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)
	{
		UECC_FSM_SET_MAIN_STATE(
				p_ue_context, &uecc_fsm_ue_trg_ho_ongoing, p_api);

		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
				p_ue_context->p_gb_context->facility_name,
				RRC_INFO,"[%s]Starting TRG HO procedure "
				" Received INTRA_CELL Handover Request",
				__FUNCTION__);

		uecc_fsm_start_procedure(
				p_ue_context,
				&uecc_fsm_ue_trg_ho_procedure, p_api);

	}
	else
	{
		UECC_FSM_SET_MAIN_STATE( p_ue_context, &uecc_fsm_ue_connected, PNULL);
	}
	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name   : uecc_fsm_s1ap_error_ind_handler_wait_for_s1ap_ue_ctx_rel_cmd 
 ** Inputs         : uecc_ue_context_t* - pointer to UE context
 **                  void*   - api data
 ** Outputs        : None
 ** Returns        : None
 ** Description    : This is the handler function for the event
 *                  UECC_EV_UE_ERROR_INDICATION in 
 **                  uecc_fsm_ue_wait_for_s1ap_ue_ctx_rel_cmd. This function will 
 **                  send internal s1ap_ue_release_msg_req_t msg to s1ap which 
 **                  then initiate reset towards MME. RESET ACK will not be
 **                  forwarded to uecc/OAM/RRM
 *****************************************************************************/
static void uecc_fsm_s1ap_error_ind_handler_wait_for_s1ap_ue_ctx_rel_cmd(
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
	RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	/* Stop MME_GUARD_TIMER */
	if (PNULL !=
			p_ue_context->timers_data.timers_ids[UECC_UE_MME_GUARD_TIMER])
	{
		uecc_ue_timer_stop(p_ue_context->p_gb_context,
				UECC_UE_MME_GUARD_TIMER, &p_ue_context->timers_data);

	}

	RRC_SET_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
	/* SPR 16750 Fix Start */
	/* Code Removed */
	/* SPR 16750 Fix Stop */

    /*SPR_21818_START*/
    /* As a preventive check uecc will send "ue release req" to s1ap module.
     * If context at s1ap is already released then s1ap will return
     * after printing message */
    if (RRC_SUCCESS != uecc_s1ap_build_and_send_ue_release_msg_req(
                p_ue_context->s1ap_ue_id,
                RRC_FALSE,
                p_ue_context))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "ue_release_msg delivery failure");
    }
    /*SPR_21818_END*/

	p_ue_context->share_data.uecc_rcr_data.u.cause.t =
		T_s1ap_Cause_radioNetwork;
	p_ue_context->share_data.uecc_rcr_data.u.cause.u.radioNetwork =
		s1ap_failure_in_radio_interface_procedure;

	UECC_FSM_SET_MAIN_STATE(
			p_ue_context, &uecc_fsm_ue_connection_release_ongoing, PNULL);

	uecc_fsm_start_procedure(
			p_ue_context,
			&uecc_fsm_ue_connection_release_setup_failure_procedure, PNULL);
	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/* SPR 16750 Fix Start */
/* Code Removed */
/* SPR 16750 Fix Stop */

/****************************************************************************
 * Function Name  : uecc_fsm_ue_reconfig_success_int_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRM_UE_RECONFIG_SUCCESS_INT message
 ****************************************************************************/
static void uecc_fsm_ue_reconfig_success_int_handler(
		uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
		void                *p_api                  /* Input API buffer */
		)
{
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
	RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	/* Set the state of main FSM to UE CONNECTED */
	UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);

	if (PNULL != p_ue_context->p_curr_proc_data)
	{
		if (RRC_HO_REQUIRED_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type)
		{
			/* Start the HO Procedure */
			uecc_event_queue_push_event(
					UECC_EV_HO_REQUIRED_INT,
					p_ue_context,
					p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
			p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
			p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;   

			rrc_mem_free(p_ue_context->p_curr_proc_data);
			p_ue_context->p_curr_proc_data = PNULL;
		}
	}
	//check - do we need to handle RRC Re-establishment & RLF also

	/* Bug 175 */
	/* Deallocate memory previously allocated for UE Reconfiguration
	 * procedure in the UE Context */    
	if (PNULL != p_ue_context->p_curr_proc_data)
	{
		rrc_mem_free(p_ue_context->p_curr_proc_data);
		p_ue_context->p_curr_proc_data = PNULL;
	}
	/* Bug 175 */

	/* Stop Procedure */
	uecc_fsm_finish_active_procedure(p_ue_context);

#ifdef MEM_PROFILE_ON
	RRC_TRACE(RRC_INFO,"\n Sizeof UE CONTEXT after UE RECONFIGURATION is successful %d ",\
			sizeof(uecc_ue_context_t));
	qvPoolStats();
#endif
	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_ue_reconfig_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRM_UE_RECONFIG_REQ message
 ****************************************************************************/
static void uecc_fsm_ue_rrm_ue_reconfig_req_handler(
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);


	/* Set the State of Main FSM to UE CONNECTION RECONFIGURATION ONGOING */
	UECC_FSM_SET_MAIN_STATE (p_ue_context, 
			&uecc_fsm_ue_connection_reconfiguration_ongoing, 
			PNULL);

	/* Start the RRM Initiated UE Reconfiguration Procedure*/
	uecc_fsm_start_procedure (p_ue_context,
			&uecc_fsm_ue_rrm_ue_reconfig_procedure, 
			p_api);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_reconfig_failure_rel_ue_ctx_int_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_UE_RECONFIG_FAILURE_REL_UE_CTX_INT message
 ****************************************************************************/
static void uecc_fsm_ue_reconfig_failure_rel_ue_ctx_int_handler(
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{

	s1ap_Cause cause;

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
	RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	/* Failure on UE Reconfiguration Procedure */
	cause.t = T_s1ap_Cause_radioNetwork;
	cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;

	/* Now build and send UE CONTEXT RELEASE REQ to MME */
	if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
				p_ue_context,
				&cause))
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
				p_ue_context->p_gb_context->facility_name,
				RRC_WARNING,"[s1ap_UEContextReleaseRequest] Delivery failed");
	}                       

	/* Free memory for curr_proc_data_t */
	if ( PNULL != p_ue_context->p_curr_proc_data )
	{
		/* Freed if any message is buffered */
		if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
		{
			rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
			p_ue_context->p_curr_proc_data->msg_bfr_rcvd=PNULL;
		}

		rrc_mem_free(p_ue_context->p_curr_proc_data);
		p_ue_context->p_curr_proc_data = PNULL;
	}

	/* Stop Procedure */
	uecc_fsm_finish_active_procedure(p_ue_context);

	/* CHANGE STATE TO WAIT FOR CONNECTION RELEASE */
	UECC_FSM_SET_MAIN_STATE(
			p_ue_context, &uecc_fsm_ue_wait_for_connection_release, PNULL);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_reconfig_failure_int_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_UE_RECONFIG_FAILURE_INT message
 ****************************************************************************/
static void uecc_fsm_ue_reconfig_failure_int_handler(
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{
	uecc_fsm_event_et uecc_event_id = UECC_EV_LAST;
	rrc_bool_t valid_cause = RRC_FALSE;
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
	RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	if ( PNULL == p_ue_context->p_curr_proc_data)
	{
		RRC_TRACE(RRC_FATAL,
				"p_ue_context->p_curr_proc_data is NULL"); 
		return;
	}
	if ( PNULL != p_ue_context->p_curr_proc_data && 
			PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
	{
		/* Failure on UE Reconfiguration Procedure */
		switch (p_ue_context->p_curr_proc_data->msg_bfr_type)
		{
			case RRC_HO_REQUIRED_MSG:
				uecc_event_id = UECC_EV_HO_REQUIRED_INT;
				valid_cause = RRC_TRUE;
				break;
			case RRC_RE_ESTABLISH_MSG:
				uecc_event_id = UECC_EV_RRC_RE_ESTABLISH_REQ_INT;
				valid_cause = RRC_TRUE;
				break;
			case RRC_RLF_MSG:
				uecc_event_id = UECC_EV_LLIM_RADIO_LINK_FAILURE_IND;
				valid_cause = RRC_TRUE;
				break;
			default:
				rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
				p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
				p_ue_context->p_curr_proc_data = PNULL;
		}
		if(RRC_TRUE == valid_cause)
		{
			/* Send event to UE Main FSM */
			uecc_event_queue_push_event(
					uecc_event_id,
					p_ue_context,
					p_ue_context->p_curr_proc_data->msg_bfr_rcvd); 

			p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;
			p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
			p_ue_context->p_curr_proc_data = PNULL;
		}
	}

	/* Change UE Main FSM State to UE CONNECTED */
	UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);

	/* Bug 175 */
	/* Deallocate memory previously allocated for UE Reconfiguration
	 * procedure in the UE Context */    
	if (PNULL != p_ue_context->p_curr_proc_data)
	{
		rrc_mem_free(p_ue_context->p_curr_proc_data);
		p_ue_context->p_curr_proc_data = PNULL;
	}
	/* Bug 175 */

	/* Stop Procedure */
	uecc_fsm_finish_active_procedure(p_ue_context);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_csfb_parameters_request_CDMA2000_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives
 *                  UECC_EV_CSFB_PARAMETERS_REQUEST_CDMA2000 event
 ****************************************************************************/
void uecc_fsm_ue_csfb_parameters_request_CDMA2000_handler(
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	/* Build and send the RRC_RRM_CSFB_PARAMETER_TRANSFER_REQ to RRM */
	if (RRC_SUCCESS !=
			uecc_rrm_build_and_send_csfb_parameters_req_cdma2000(
				p_ue_context)
	   )
	{
		RRC_UECC_TRACE( p_ue_context->p_gb_context->context_index,
				p_ue_context->p_gb_context->facility_name,
				RRC_WARNING,
				"CSFB PARAMETER TRANSFER REQ to RRM.");
	}

	/* Stay in same state and wait for response*/
	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_csfb_parameters_response_CDMA2000_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  uecc_fsm_csfb_parameters_response_CDMA2000_handler from RRM.
 *****************************************************************************/
void uecc_fsm_csfb_parameters_response_CDMA2000_handler(
		uecc_ue_context_t   *p_ue_context,  /* UECC UE context */
		void                *p_api     /* Input API buffer */
		)
{
	rrc_rrm_csfb_parameters_resp_cdma2000_t*
		p_rrc_rrm_csfb_parameters_resp_cdma2000= PNULL;


	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	p_rrc_rrm_csfb_parameters_resp_cdma2000
		= (rrc_rrm_csfb_parameters_resp_cdma2000_t*)p_api;

	/*Encode and send response to LLIM*/
	if(RRC_SUCCESS != uecc_llim_build_and_send_csfb_parameters_response_CDMA2000(
				p_ue_context,
				p_rrc_rrm_csfb_parameters_resp_cdma2000)) 

	{
		RRC_UECC_UE_FSM_TRACE(RRC_FATAL,
				"[csfb_parameters_response] delivery failure");
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*securitykey*/
/****************************************************************************
 * Function Name  : uecc_fsm_ue_ctx_ongoing_ho_success_int_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    :This is the handler function that set main state to ue 
 *		context modification ongoing state which is saved in previous 
 *		ongoing state during rekeying procedure and set activity procedure accordingly  
 *****************************************************************************/
static void uecc_fsm_ue_ctx_ongoing_ho_success_int_handler(
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

	if(PNULL != p_ue_context->prev_ongoing_uecc_fsm.p_current_state )
	{ 
		UECC_FSM_SET_MAIN_STATE(p_ue_context, 
				p_ue_context->prev_ongoing_uecc_fsm.p_current_state, 
				PNULL);

		uecc_fsm_start_procedure(
				p_ue_context,
				&uecc_fsm_ue_context_mod_after_security_rekeying_procedure, PNULL);
	}
	else
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
				p_ue_context->p_gb_context->facility_name,
				RRC_WARNING,"FSM Info for the preempted procedure to"
				"be resumed is absent in UE Context "); 
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}
/*securitykey*/

/* Bug 817 Fix Start */
/****************************************************************************
 * Function Name  : uecc_fsm_end_marker_ind_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the timer expiry
 *                event to delete forwarding tunnels.
 *****************************************************************************/
void uecc_fsm_end_marker_ind_timer_expiry_handler (
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{
	RRC_ASSERT(PNULL != p_ue_context);

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	p_ue_context->timers_data.timers_ids[UECC_UE_INTRA_CELL_HO_GUARD_TIMER] = PNULL;

	if ( RRC_SUCCESS != uecc_llim_build_and_send_delete_trg_forwarding_tunnel_req
			(p_ue_context))
	{
		RRC_UECC_UE_FSM_TRACE(RRC_ERROR,
				"Either no forwarding tunnel exist or"
				" delivery of S1U_RECONFIGURE_REQ gets failed;Unused variable p_api=%p",p_api);/*SPR 17777 +-*/
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}
/* Bug 817 Fix Stop */
/* SPR 9343 Start */
/* BUG_11163 start */
/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_rlf_ongoing_ue_reconfig_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRM_UE_RECONFIG_REQ message
 ****************************************************************************/
static void uecc_fsm_ue_rrm_rlf_ongoing_ue_reconfig_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    /*spr_22705/SES-492_changes_start*/
    U32    error_code = RRC_INVALID_RRM_MESSAGE_RECV;
    /*spr_22705/SES-492_changes_end*/

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    /*spr_22705/SES-492_changes_start*/
    /*code removed*/
    /* Failure response is sent because in this state eNB has detected that Radio link
     * with UE is completely lost and there is no other way to recover it. eNB
     * is now waiting for the UE to recover the radio link with the help of
     * reestablishment procedure
     * */
    /*spr_22705/SES-492_changes_end*/

    /* Build and send RRC_RRM_UE_RECONFIG_RESP with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_reconfig_resp(
                p_ue_context, FAILURE, error_code))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_ERROR, "[RRC_RRM_UE_RECONFIG_RESP] Message delivery failure");
    }
    /*spr_22705/SES-492_changes_start*/
    /*code removed*/
    /*spr_22705/SES-492_changes_end*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_re_establish_ue_entity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP message.
 ****************************************************************************/
void uecc_fsm_llim_resume_all_srb_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_resume_ue_entity_resp_t
           *uecc_llim_resume_ue_entity_resp = PNULL;
    U32    error_code = RRC_NO_ERROR;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    uecc_llim_resume_ue_entity_resp = (rrc_uecc_llim_resume_ue_entity_resp_t*) p_api;

    switch (uecc_llim_resume_ue_entity_resp->response_code)
    {
        case RRC_SUCCESS:
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_INFO, "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP]"
                    " Successful Response");

            if ( (PNULL != p_ue_context->p_curr_proc_data) &&
                  (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd) &&
                   (RRC_UE_RECONFIG == p_ue_context->p_curr_proc_data->msg_bfr_type) )
            {
                UECC_FSM_SET_MAIN_STATE(
                    p_ue_context, &uecc_fsm_ue_connected, PNULL);

                /* Send event to UE Main FSM and pass API data to FSM. */
                uecc_event_queue_push_event(
                    UECC_EV_RRM_UE_RECONFIG_REQ,
                    p_ue_context,
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            }
            else
            {
                uecc_llim_resume_ue_entity_resp->response_code =
                    RRC_FAILURE;
            }
            break ;
        case RRC_FAILURE :
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING, "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP] Failure Response");

            break;
        default :
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING, "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP] Invalid Response=%i",
                    uecc_llim_resume_ue_entity_resp->response_code);
            break;
    }
    /* Process failure */
    if (RRC_SUCCESS != uecc_llim_resume_ue_entity_resp->response_code)
    {
        error_code = RRC_LLIM_RECONFIG_FAILURE;
        /* Build and send RRC_RRM_UE_RECONFIG_RESP with RRC_FAILURE to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_reconfig_resp(
                    p_ue_context, FAILURE, error_code))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_ERROR, "[RRC_RRM_UE_RECONFIG_RESP] Message delivery failure");
        }
	/* Send event to UE Main FSM and pass API data to FSM. */
	/* Coverity Fix 69683 Start */
	if (( PNULL != p_ue_context->p_curr_proc_data) &&
		( PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd ))
	{
	    uecc_event_queue_push_event(
		    UECC_EV_W_FOR_REEST_TIMER_EXPIRY,
		    p_ue_context,
		    p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
	}
	else
	{
	    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
		    p_ue_context->p_gb_context->facility_name,
		    RRC_ERROR, "[RRC_RRM_UE_RECONFIG_RESP] Invalid Current Procedure Data");
	}
	/* Coverity Fix 69683 Stop */
    }

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;

        /* Freed if any message is buffered */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }

        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* BUG_11163 stop */
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_failure_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    :This is the handler function that send failure in
 * 		RRC_RRM_UE_RECONFIG_RESP if UE is not in Connected State. 
 *****************************************************************************/
static void uecc_fsm_rrm_ue_reconfig_failure_resp_handler(
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{
	U32    error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;

	RRC_ASSERT(PNULL != p_ue_context);

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);         

	/* Build and send RRC_RRM_UE_RECONFIG_RESP with RRC_FAILURE to RRM */
	if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_reconfig_resp(
				p_ue_context, FAILURE, error_code))
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
				p_ue_context->p_gb_context->facility_name,
				RRC_WARNING,
				"[RRC_RRM_UE_RECONFIG_RESP] Message Delivery failure");
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}  
/* SPR 9343 Stop */
/* Bug 875 Fix Start */
/****************************************************************************
 * Function Name  : uecc_fsm_ue_release_csc_del_msg_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the cell delete/stop
 *                received during UE release.
 *****************************************************************************/
void uecc_fsm_ue_release_csc_del_msg_ind_handler (
		uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
		void                *p_api              /* Input API buffer */
		)
{
	RRC_ASSERT(PNULL != p_ue_context);

	RRC_UECC_UT_TRACE_ENTER (p_ue_context->p_gb_context);

	RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,
			"Entering Function %s in File %s; Unused variable p_api=%p",
			__FUNCTION__,__FILE__,p_api);/*SPR 17777 +-*/

	S1AP_UPDATE_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);

	if (p_ue_context->ue_ctx_del_status.is_rrc_resp_expected == RRC_NULL)
	{
		/* Send event to UE Main FSM */
		uecc_event_queue_push_event(
				UECC_EV_UE_RELEASED_INT,
				p_ue_context,
				PNULL);
	}
	RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,
			"Exiting Function %s in File %s",
			__FUNCTION__,__FILE__);

	RRC_UECC_UT_TRACE_EXIT (p_ue_context->p_gb_context);
}
/* Bug 875 Fix Stop */

/*SPR 15082 Fix Start*/
/****************************************************************************
 * Function Name  : uecc_fsm_ho_ongoing_erab_rel_cmd_handler
 *
 * Description    :
 *     This action routine is called when Handover procedure is ongoing and
 *     UECC_EV_S1AP_ERAB_RELEASE_COMMAND message is received
 *
 * Returns        : None
 *
 **************************************************************************************/
void uecc_fsm_ho_ongoing_erab_rel_cmd_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    OSRTDListNode*                      p_node = PNULL;
    erab_list_t*                        p_erab_failed_list = PNULL;
    rrc_s1ap_erab_release_command_t*    p_msg = PNULL;
    s1ap_E_RABList_element*             p_erab_elem = PNULL;
    /*SPR_18582_FIX_START*/
    /* code removed */
    U8                                  index = RRC_NULL;
    /*SPR_18582_FIX_END*/
    U16                                 e_rab_to_be_released_list_count = RRC_NULL;
    U32                                 s1ap_cause = RRC_NULL;
    /* SES-521 Changes - Starts */
    rrc_bool_et                         free_curr_proc = RRC_FALSE;
    /* SES-521 Changes - Ends */

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    fill_s1ap_fail_cause(p_ue_context, &s1ap_cause);

    /* SES-521 Changes - Starts */
    /* Check Curr Proc Data, if not allocated then
     * Allocate and Free after sending appropriate response
     */
    if(PNULL == p_ue_context->p_curr_proc_data)
    {
        p_ue_context->p_curr_proc_data =
            (curr_proc_data_t *)rrc_mem_get(sizeof(curr_proc_data_t));
        free_curr_proc = RRC_TRUE;
        RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Allocated for Current PROC data.");
    }
    /* SES-521 Changes - Ends */
    do
    {
        /* SES-521 Changes - Starts */
        if (PNULL != p_ue_context->p_curr_proc_data)
        /* SES-521 Changes - Ends */
        {
            /* Allocate memory for erab_rel_cmd */
            p_msg = (rrc_s1ap_erab_release_command_t*)p_api;

            /* points to erab_failed_to_release_list */
            p_erab_failed_list = &p_ue_context->p_curr_proc_data->u.
                erab_release_command_data.erab_failed_to_release_list;
            /*SPR_18582_FIX_START*/
            for(index = RRC_NULL;index < MAX_ERAB_COUNT;index++)
            {
                p_erab_failed_list->erab_item[index].e_RAB_ID = INVALID_ERAB_ID;
            }
            for (p_node = p_msg->e_rab_to_be_released_list.head;
                (PNULL != p_node) &&
                (e_rab_to_be_released_list_count < MAX_MME_ERAB_LIST_COUNT) &&
                (p_erab_failed_list->counter < MAX_ERAB_COUNT);
                p_node = p_node->next, e_rab_to_be_released_list_count++)
            /*SPR_18582_FIX_END*/
            {
                p_erab_elem = (s1ap_E_RABList_element*)p_node->data;
                /* check for valid IDs was performed before when decoding */
                /*SPR_18582_FIX_START*/
                if ((RRC_PNULL != p_erab_elem ) &&
                    (RRC_PNULL != p_erab_elem->value.u._E_RABItemIEs_1) &&
                    (RRC_SUCCESS != check_if_erab_id_already_present_in_failed_list(
                        p_erab_failed_list->erab_item,
                        p_erab_elem->value.u._E_RABItemIEs_1->e_RAB_ID, p_ue_context)))
                {
                    p_erab_failed_list->erab_item[p_erab_failed_list->counter].e_RAB_ID =
                        p_erab_elem->value.u._E_RABItemIEs_1->e_RAB_ID;
                }
                else
                {
                    RRC_UECC_UE_FSM_TRACE(RRC_INFO,
                        "Duplicate occurance or p_erab_elem or p_erab_elem->value.u._E_RABItemIEs_1 NULL");
                    continue;
                }
                /*SPR_18582_FIX_END*/
                p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                    cause.t = T_s1ap_Cause_radioNetwork ;
                p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                    cause.u.radioNetwork = s1ap_cause;
                p_erab_failed_list->counter++;
            }
            /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
            if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                    RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
            {
                RRC_UECC_UE_FSM_TRACE(RRC_FATAL,
                    "[s1ap_ERABReleaseResponse] delivery failure");
            }

            /* SES-521 Changes - Starts */
            /* Free memory for curr_proc_data_t, only if allocated here */
            if(RRC_TRUE == free_curr_proc)
            {
                rrc_mem_free(p_ue_context->p_curr_proc_data);
                p_ue_context->p_curr_proc_data = PNULL;
                RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Free for Current PROC data.");
            }
        }
        else
        {
            RRC_UECC_UE_FSM_TRACE(RRC_FATAL, "p_curr_proc_data is NULL");
        }
        /* SES-521 Changes - Ends */
    } while(0);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_ho_ongoing_erab_setup_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Send ERAB SETUP RESPONSE containing all failure.
 *****************************************************************************/
void uecc_fsm_ue_ho_ongoing_erab_setup_req_handler(
    uecc_ue_context_t*  p_ue_context,
        void*           p_api
)
{
    OSRTDListNode   *p_node = PNULL;
    U8              fail_list_count = RRC_NULL;
    U32             s1ap_cause = RRC_NULL;
    /*SPR_18582_FIX_START*/
    U8              index = RRC_NULL;
    /*SPR_18582_FIX_END*/
    /* SES-521 Changes - Starts */
    rrc_bool_et     free_curr_proc = RRC_FALSE;
    /* SES-521 Changes - Ends */
    s1ap_E_RABToBeSetupListBearerSUReq_element  *p_erab_elem = PNULL;
    uecc_erab_failed_to_setup_list_t            *p_failed_erab_data = PNULL;

    rrc_s1ap_erab_setup_request_t       *p_msg = (rrc_s1ap_erab_setup_request_t *)p_api;
    s1ap_E_RABToBeSetupListBearerSUReq  *p_e_rab_to_be_setup_list = &p_msg->e_rab_to_be_setup_list;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    fill_s1ap_fail_cause(p_ue_context, &s1ap_cause);

    /* SES-521 Changes - Starts */
    /* Check Curr Proc Data, if not allocated then
     * Allocate and Free after sending appropriate response
     */
    if(PNULL == p_ue_context->p_curr_proc_data)
    {
        p_ue_context->p_curr_proc_data =
            (curr_proc_data_t *)rrc_mem_get(sizeof(curr_proc_data_t));
        free_curr_proc = RRC_TRUE;
        RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Allocated for Current PROC data.");
    }
    /* SES-521 Changes - Ends */
    do
    {
        /* SES-521 Changes - Starts */
        if (PNULL != p_ue_context->p_curr_proc_data)
        /* SES-521 Changes - Ends */
        {
            p_failed_erab_data = &p_ue_context->p_curr_proc_data->u.
                erab_setup_request_data.failed_erab_data;
            /*SPR_18582_FIX_START*/
            for(index = RRC_NULL;index < MAX_ERAB_COUNT;index++)
            {
                p_failed_erab_data->erab_failed_to_setup_item[index].e_RAB_ID =
                    INVALID_ERAB_ID;
            }
            /* Find the mutiple occurences of ERAB IDs in temp_erab_list array */
            for (p_node = p_e_rab_to_be_setup_list->head;
                (PNULL != p_node) && (p_e_rab_to_be_setup_list->count < MAX_MME_ERAB_LIST_COUNT) &&
                (fail_list_count < MAX_ERAB_COUNT);
                p_node = p_node->next)
            /*SPR_18582_FIX_END*/
            {
                p_erab_elem=(s1ap_E_RABToBeSetupListBearerSUReq_element*)p_node->data;
                /*SPR_18582_FIX_START*/
                if((RRC_PNULL != p_erab_elem) &&
                    (RRC_PNULL != p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1))
                {
                    if (RRC_SUCCESS == check_if_erab_id_already_present_in_failed_list(
                            p_failed_erab_data->erab_failed_to_setup_item,
                            p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID,
                            p_ue_context))
                    {
                        RRC_UECC_UE_FSM_TRACE(RRC_INFO,
                            "Duplicate occurance, E_RAB ID [%u] already present in failed list",
                            p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID);
                        continue;
                    }
                    /*SPR_18582_FIX_END*/
                    p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                        e_RAB_ID = p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID;
                    p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].cause.t
                        =  T_s1ap_Cause_radioNetwork;
                    p_failed_erab_data->erab_failed_to_setup_item[fail_list_count++].cause.u.
                        radioNetwork = s1ap_cause;

                    RRC_UECC_UE_FSM_TRACE(RRC_ERROR,
                        "E_RAB ID Put in Failed List : %u",
                        p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID);
                }
            }
            p_failed_erab_data->count = fail_list_count;

            /* Send ASN.1 Encoded ERAB SETUP Response to MME */
            if(RRC_SUCCESS !=
                uecc_s1ap_build_and_send_erab_setup_response(p_ue_context))
            {
                RRC_UECC_UE_FSM_TRACE(RRC_FATAL,
                    "[s1ap_E_RABSetupResponse] delivery failure");
            }

            /* SES-521 Changes - Starts */
            /* Free memory for curr_proc_data_t, only if allocated here */
            if(RRC_TRUE == free_curr_proc)
            /* SES-521 Changes - Ends */
            {
                rrc_mem_free(p_ue_context->p_curr_proc_data);
                p_ue_context->p_curr_proc_data = PNULL;
                RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Free for Current PROC data.");
            }
        }
        /* SES-521 Changes - Starts */
        else
        {
            RRC_UECC_UE_FSM_TRACE(RRC_FATAL, "p_curr_proc_data is NULL");
        }
        /* SES-521 Changes - Ends */
    }while(0);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_ho_onoing_erab_modify_send_failure_response_to_mme
 *
 * DESCRIPTION   : This function updates the failed list from the
 *                 response received from RRM.
 *
 * RETURNS       : void
 *
 *****************************************************************************/
void uecc_ho_ongoing_erab_modify_send_failure_response_to_mme(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_s1ap_erab_modify_request_t  *p_msg;
    U16                             e_rab_list_count = RRC_NULL;
    /*SPR_18582_FIX_START*/
    /* code removed */
    U8                              index = RRC_NULL;
    /*SPR_18582_FIX_END*/
    OSRTDListNode                   *p_node      = PNULL;
    erab_failed_to_modify_list_t    *p_failed_modify_list = PNULL;
    U32                             s1ap_cause = RRC_NULL;
    s1ap_E_RABToBeModifiedListBearerModReq_element *p_erab_elem = PNULL;
    /* SES-521 Changes - Starts */
    rrc_bool_et                     free_curr_proc = RRC_FALSE;
    /* SES-521 Changes - Ends */

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    fill_s1ap_fail_cause(p_ue_context, &s1ap_cause);
    p_msg = (rrc_s1ap_erab_modify_request_t*)p_api;

    /* SES-521 Changes - Starts */
    /* Check Curr Proc Data, if not allocated then
     * Allocate and Free after sending appropriate response
     */
    if(PNULL == p_ue_context->p_curr_proc_data)
    {
        p_ue_context->p_curr_proc_data =
            (curr_proc_data_t *)rrc_mem_get(sizeof(curr_proc_data_t));
        free_curr_proc = RRC_TRUE;
        RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Allocated for Current PROC data.");
    }

    if (PNULL != p_ue_context->p_curr_proc_data)
    /* SES-521 Changes - Ends */
    {
        p_failed_modify_list = &(p_ue_context->p_curr_proc_data->u.
            erab_modify_request_data.erab_failed_to_modify_list);
        /*SPR_18582_FIX_START*/
        for(index = RRC_NULL;index < MAX_ERAB_COUNT;index++)
        {
            p_failed_modify_list->erab_failed_to_modify_item[index].e_RAB_ID =
                INVALID_ERAB_ID;
        }
        for (p_node = p_msg->e_rab_to_be_modified_list.head;
            (PNULL!=p_node) && (e_rab_list_count < MAX_MME_ERAB_LIST_COUNT) &&
            (p_failed_modify_list->erab_failed_to_modify_list_counter < MAX_ERAB_COUNT);
            p_node = p_node->next,e_rab_list_count++)
        /*SPR_18582_FIX_END*/
        {
            p_erab_elem = (s1ap_E_RABToBeModifiedListBearerModReq_element*)p_node->data;
            /* check for valid IDs was performed before when decoding */
            /*SPR_18582_FIX_START*/
            if((RRC_PNULL != p_erab_elem) &&
                (RRC_PNULL != p_erab_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1))
            {
                if (RRC_SUCCESS == check_if_erab_id_already_present_in_failed_list(
                        p_failed_modify_list->erab_failed_to_modify_item,
                        p_erab_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1->e_RAB_ID,
                        p_ue_context))
                {
                    RRC_UECC_UE_FSM_TRACE(RRC_INFO,
                        "Duplicate occurance, E_RAB ID [%u] already present in failed list",
                        p_erab_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1->e_RAB_ID);
                    continue;
                }
                /* Save the e_rab_id in the failed list */
                p_failed_modify_list->erab_failed_to_modify_item
                    [p_failed_modify_list->erab_failed_to_modify_list_counter].e_RAB_ID =
                    p_erab_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1->e_RAB_ID;
                /* Update the cause. */
                p_failed_modify_list->erab_failed_to_modify_item
                    [p_failed_modify_list->erab_failed_to_modify_list_counter].
                    cause.t = T_s1ap_Cause_radioNetwork;
                p_failed_modify_list->erab_failed_to_modify_item
                    [p_failed_modify_list->erab_failed_to_modify_list_counter].
                    cause.u.radioNetwork = s1ap_cause;
                p_failed_modify_list->erab_failed_to_modify_item
                    [p_failed_modify_list->erab_failed_to_modify_list_counter].
                    error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;
                p_failed_modify_list->erab_failed_to_modify_list_counter++;
            }
        }
        /*SPR_18582_FIX_END*/

        if (RRC_SUCCESS !=
            uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
        {
            RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                "[s1ap_E_RABModifyResponse] Delivery failure");
        }

        /* SES-521 Changes - Starts */
        /* Free memory for curr_proc_data_t, only if allocated here */
        if(RRC_TRUE == free_curr_proc)
        /* SES-521 Changes - Ends */
        {
            /* Free the memory taken for current procedure data */
            rrc_mem_free(p_ue_context->p_curr_proc_data);
            p_ue_context->p_curr_proc_data = PNULL;
            RRC_UECC_UE_FSM_TRACE(RRC_INFO, "Memory Free for Current PROC data.");
        }
    }
    /* SES-521 Changes - Starts */
    else
    {
        RRC_UECC_UE_FSM_TRACE(RRC_FATAL, "p_curr_proc_data is NULL");
    }
    /* SES-521 Changes - Ends */
}
/*SPR 15082 Fix Stop*/

/*SPR_18582_FIX_START*/
/***********************************************************************************
 * Function Name  : check_if_erab_id_already_present_in_failed_list
 * Input          : erab_item_t *p_erab_item
 *                : s1ap_E_RAB_ID erab_id
 *                : uecc_ue_context_t *p_ue_context
 * Output         : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This is the function to find if an erabId is already populated in
 *                :  erabList. This function returns RRC_SUCCESS in case matching erabid
 *                : already populated in list passed.
 ***********************************************************************************/
rrc_return_et check_if_erab_id_already_present_in_failed_list( erab_item_t *p_erab_item,
        s1ap_E_RAB_ID erab_id, uecc_ue_context_t *p_ue_context )
{
    U8             erab_index = RRC_NULL;
   /*Coverity 107108 start*/
    rrc_return_et   ret_val = RRC_FAILURE;
   /*Coverity 107108 stop*/

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    for (erab_index = RRC_NULL; erab_index < MAX_ERAB_COUNT; erab_index++)
    {
        if (p_erab_item[erab_index].e_RAB_ID == erab_id)
        {
            ret_val = RRC_SUCCESS;
            break;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return ret_val;
}
/*SPR_18582_FIX_END*/

/* SPR 16401 Fix Start */
/***********************************************************************************
 * Function Name  : uecc_fsm_wait_for_s1ap_ue_ctx_rel_cmd_re_establish_req_handler 
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ in 
 *                  UECC_UE_WAIT_FOR_S1AP_UE_CTX_REL_CMD_ONGOING state .
 *
 **********************************************************************************/
void uecc_fsm_wait_for_s1ap_ue_ctx_rel_cmd_re_establish_req_handler (
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{   
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /* SPR_17377_Warning_fix + */
    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
     /* SPR_17377_Warning_fix - */
            p_api_data);

    /* Send Re-establishment Reject */
    if (RRC_SUCCESS !=
                uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                    p_ue_context->p_gb_context, p_ue_context->crnti, 0,
                    p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
    {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_ERROR,
                    "Fail to send the RRCConnectionReestablishmentReject to UE");
    }
    else
    {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                    p_ue_context->p_gb_context->facility_name,RRC_INFO,
                    "[uecc_fsm_wait_for_s1ap_ue_ctx_rel_cmd_re_establish_req_handler]:"
	   " RE-EST REJECT MSG SEND SUCCESSFULLY");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* SPR 16401 Fix Stop */

/*SPR 18758 Fix Start*/
/****************************************************************************
 * Function Name  : uecc_fsm_ue_reconfig_ongoing_erab_setup_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Send ERAB SETUP RESPONSE containing all failure while
 *                  Connection Reestablishment procedure is ongoing.
 ****************************************************************************/
void uecc_fsm_ue_reconfig_ongoing_erab_setup_req_handler( 
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer*/
)
{

    rrc_s1ap_erab_setup_request_t    *p_msg = 
         (rrc_s1ap_erab_setup_request_t *)p_api;
    OSRTDListNode* p_node = PNULL;
    U8 fail_list_count = 0;        
    s1ap_E_RABToBeSetupListBearerSUReq *p_e_rab_to_be_setup_list = 
            &p_msg->e_rab_to_be_setup_list;
    s1ap_E_RABToBeSetupListBearerSUReq_element *p_erab_elem = PNULL;    
    uecc_erab_failed_to_setup_list_t   *p_failed_erab_data = PNULL;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Memory allocated dynamically for curr_proc_data */
    p_ue_context->p_failed_curr_proc_data = (failed_curr_proc_data_t *)rrc_mem_get(sizeof(
                                     failed_curr_proc_data_t));
    do
    {
        if (PNULL == p_ue_context->p_failed_curr_proc_data)
        {
            RRC_UECC_UE_FSM_TRACE(RRC_FATAL,
                "Memory Allocation failed for Failed Current PROC data.");
                break;
        }
        memset_wrapper(p_ue_context->p_failed_curr_proc_data,
                   0,
                   sizeof(failed_curr_proc_data_t));
        p_failed_erab_data = &p_ue_context->p_failed_curr_proc_data->u.
                             erab_setup_request_data.failed_erab_data;

        /* Find the mutiple occurences of ERAB IDs in temp_erab_list array */
        for (p_node = p_e_rab_to_be_setup_list->head;
            (PNULL != p_node) && (p_e_rab_to_be_setup_list->count < MAX_MME_ERAB_LIST_COUNT);
            p_node=p_node->next )
        {
            p_erab_elem=(s1ap_E_RABToBeSetupListBearerSUReq_element*)p_node->data;
            if ((p_erab_elem == PNULL) || ( p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1 == PNULL))
            {
                continue;
            }
        
            p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                        e_RAB_ID = p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID;
            p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].cause.t 
                        =  T_s1ap_Cause_radioNetwork;
            p_failed_erab_data->erab_failed_to_setup_item[fail_list_count++].cause.u.
                        radioNetwork = s1ap_interaction_with_other_procedure;
            
            RRC_UECC_UE_FSM_TRACE(RRC_ERROR,
                    "E_RAB ID Put in Failed List : %u",
                    p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID);
        } 
        p_failed_erab_data->count = fail_list_count;

        /* Send ASN.1 Encoded ERAB SETUP Response to MME */
        if(RRC_SUCCESS !=
           uecc_s1ap_build_and_send_erab_setup_response(p_ue_context))
        {
            RRC_UECC_UE_FSM_TRACE(RRC_FATAL,
                "[s1ap_E_RABSetupResponse] delivery failure");
        }

        /* Free memory for failed_curr_proc_data_t */
        if ( PNULL != p_ue_context->p_failed_curr_proc_data )
        {
            rrc_mem_free(p_ue_context->p_failed_curr_proc_data);
            p_ue_context->p_failed_curr_proc_data = PNULL;
        }
    
    }while(0);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_reconfig_ongoing_erab_modify_send_failure_response_to_mme
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * RETURNS       : void
 * DESCRIPTION   : This action routine is called when Connection Reestablishment 
 *                 procedure is ongoing and UECC_EV_S1AP_ERAB_MODIFY_REQ message
 *                 is received.
 ****************************************************************************/
void uecc_fsm_ue_reconfig_ongoing_erab_modify_send_failure_response_to_mme(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_s1ap_erab_modify_request_t *p_msg; 
    U16             e_rab_list_count      = RRC_NULL;
    U16             e_rab_index           = RRC_NULL;

    s1ap_E_RABToBeModifiedItemBearerModReq 
                 *p_e_rab_list[MAX_MME_ERAB_LIST_COUNT] = {PNULL};
    OSRTDListNode                                  *p_node      = PNULL;
    s1ap_E_RABToBeModifiedListBearerModReq_element *p_erab_elem = PNULL;
    erab_failed_to_modify_list_t  *p_failed_modify_list = PNULL;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    p_msg = (rrc_s1ap_erab_modify_request_t*)p_api;
    /* Allocate memory for the p_failed_curr_proc_data for the E-RAB procedure */
    p_ue_context->p_failed_curr_proc_data = (failed_curr_proc_data_t *) 
                                     rrc_mem_get(sizeof(failed_curr_proc_data_t));   
    if (p_ue_context->p_failed_curr_proc_data == PNULL)
    {
        /* Memory Allocation Failed */
        RRC_UECC_UE_FSM_TRACE(RRC_FATAL,
                "Memory Allocation Failure");     
        RRC_ASSERT(PNULL != p_ue_context->p_failed_curr_proc_data);
        return;
    }

    /* Set the E-RAB Modify Procedure Data to 0x00. */
    memset_wrapper(p_ue_context->p_failed_curr_proc_data, 0x00, sizeof(failed_curr_proc_data_t));
    p_failed_modify_list = &(p_ue_context->p_failed_curr_proc_data->u.
        erab_modify_request_data.erab_failed_to_modify_list);

    for (p_node = p_msg->e_rab_to_be_modified_list.head;
            (PNULL!=p_node) && (e_rab_list_count < MAX_MME_ERAB_LIST_COUNT); 
            p_node = p_node->next,e_rab_list_count++)
    {
        p_erab_elem=
            (s1ap_E_RABToBeModifiedListBearerModReq_element*)p_node->data;
        if ( (p_erab_elem == PNULL) || (p_erab_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1 == PNULL))
        {
            RRC_UECC_UE_FSM_TRACE(RRC_WARNING, "Received RABToBeModifiedListBearerModReq_element null.");
            break;
        }
        /* check for valid IDs was performed before when decoding */
        p_e_rab_list[e_rab_list_count] = p_erab_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1;
    }
    for (e_rab_index = 0;(e_rab_index <  e_rab_list_count) && (MAX_ERAB_COUNT > e_rab_index); e_rab_index++)
    {
        /* Save the e_rab_id in the failed list */
        p_failed_modify_list->erab_failed_to_modify_item
                    [e_rab_index].e_RAB_ID =  
        p_e_rab_list[e_rab_index]->e_RAB_ID;
        /* Update the cause. */
        p_failed_modify_list->erab_failed_to_modify_item
              [e_rab_index].cause.t = T_s1ap_Cause_radioNetwork;

        p_failed_modify_list->erab_failed_to_modify_item[e_rab_index].
                cause.u.radioNetwork = s1ap_interaction_with_other_procedure;                      

        p_failed_modify_list->erab_failed_to_modify_item
                    [e_rab_index].error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;
    }
    /*  Update the failed list counter */
    p_failed_modify_list->erab_failed_to_modify_list_counter = e_rab_list_count;     

    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
    {
        RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                "[s1ap_E_RABModifyResponse] Delivery failure");
    }

    if (PNULL != p_ue_context->p_failed_curr_proc_data)
    {
    /* Free the memory taken for current procedure data */
        rrc_mem_free(p_ue_context->p_failed_curr_proc_data);
        p_ue_context->p_failed_curr_proc_data = PNULL;
    }
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_reconfig_ongoing_erab_rel_cmd_handler
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 *
 * Description    : This action routine is called when Connection Reestablishment procedure 
 *                  is ongoing and UECC_EV_S1AP_ERAB_RELEASE_COMMAND message is received
 *
 ****************************************************************************/
void uecc_fsm_ue_reconfig_ongoing_erab_rel_cmd_handler( 
    uecc_ue_context_t*   p_ue_context,      /* UECC UE context */
    void*                p_api              /* Input API buffer */
)
{
    OSRTDListNode* p_node = PNULL;
    erab_list_t*   p_erab_failed_list = PNULL;
    rrc_s1ap_erab_release_command_t* p_msg = PNULL;

    s1ap_E_RABList_element*  p_erab_elem = PNULL;
    s1ap_E_RABItem* p_e_rab_to_be_released_list[MAX_MME_ERAB_LIST_COUNT];
    U16  e_rab_to_be_released_list_count = RRC_NULL;
    rrc_return_t   result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    do {
        /* Allocate memory for erab_rel_cmd */
        p_msg = (rrc_s1ap_erab_release_command_t*)p_api;
        p_ue_context->p_failed_curr_proc_data =
            (failed_curr_proc_data_t*)rrc_mem_get(sizeof(failed_curr_proc_data_t));

        if (PNULL == p_ue_context->p_failed_curr_proc_data)
        {
            RRC_UECC_UE_FSM_TRACE(RRC_FATAL,
                    "[FAILED_CURR_PROC_DATA] Memory Alocation failed(E-RAB Release Command)");
            break;
        }
        memset_wrapper(p_ue_context->p_failed_curr_proc_data, 0,
                sizeof(failed_curr_proc_data_t));
        /* points to erab_failed_to_release_list */
        p_erab_failed_list = &p_ue_context->p_failed_curr_proc_data->u.
            erab_release_command_data.erab_failed_to_release_list;

        for (p_node = p_msg->e_rab_to_be_released_list.head;
                (PNULL != p_node) &&
                (e_rab_to_be_released_list_count < MAX_MME_ERAB_LIST_COUNT);
                p_node = p_node->next, e_rab_to_be_released_list_count++)
        {
            p_erab_elem = (s1ap_E_RABList_element*)p_node->data;
            if ( (p_erab_elem == PNULL) || (p_erab_elem->value.u._E_RABItemIEs_1 == PNULL))
            {
                continue;
            }
            /* check for valid IDs was performed before when decoding */
            p_e_rab_to_be_released_list[e_rab_to_be_released_list_count] =
                p_erab_elem->value.u._E_RABItemIEs_1;

            p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                e_RAB_ID =
                p_e_rab_to_be_released_list[e_rab_to_be_released_list_count]->e_RAB_ID;

            p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                cause.t = T_s1ap_Cause_radioNetwork ;

            p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                cause.u.radioNetwork =
                s1ap_interaction_with_other_procedure;

            p_erab_failed_list->counter++;
        }
        /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                    RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
        {
            RRC_UECC_UE_FSM_TRACE(RRC_FATAL,
                    "[s1ap_ERABReleaseResponse] delivery failure");
        }
        result = RRC_SUCCESS;
    } while(0);

    if (RRC_SUCCESS == result)
    {
        if (PNULL != p_ue_context->p_failed_curr_proc_data)
        {
        /* Deallocate memory previously allocated for E-RAB Release Command */
            rrc_mem_free(p_ue_context->p_failed_curr_proc_data);
            p_ue_context->p_failed_curr_proc_data = PNULL;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR 18758 Fix Stop*/
/* SPR 21114 Fix Start */
/****************************************************************************
 * Function Name  : 
 *        uecc_fsm_rrm_ue_conn_rel_ind_handler_wait_for_s1ap_ue_ctx_rel_cmd
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  RRC_RRM_UE_CONNECTION_RELEASE_IND message while waiting
 *                  for UE Context Release Command
 *****************************************************************************/
void uecc_fsm_rrm_ue_conn_rel_ind_handler_wait_for_s1ap_ue_ctx_rel_cmd(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
    )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    p_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag
        = RRC_TRUE;
    p_ue_context->share_data.uecc_rcr_data.reset_towards_mme_required
        = RRC_TRUE;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* SPR 21114 Fix End */


#ifdef ENDC_ENABLED
/*EN-DC_changes_start*/
/****************************************************************************
 *   Function Name  : uecc_fsm_scg_failure_information_nr_handler
 *   Inputs         : uecc_ue_context_t* - pointer to UE context
 *                    void*   - api data
 *   Outputs        : None
 *   Returns        : None
 *   Description    : This is the handler function for the Event
 *                    UECC_EV_SCG_FAILURE_INFORMATION_NR
 *****************************************************************************/
void uecc_fsm_scg_failure_information_nr_handler(
    uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    uecc_event_queue_push_event(
            UECC_EV_RLF_HANDLING_FAILURE_INT,
            p_ue_context,
            PNULL);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

void  uecc_fsm_x2ap_ue_context_release_handler(
        uecc_ue_context_t*  p_ue_context
        )
{
  
    p_ue_context->x2apUecontextReleased = RRC_TRUE;
    /* Send UE CONTEXT RELEASE to SgNB */
    if (RRC_SUCCESS !=
            uecc_x2ap_build_and_send_ue_context_release(p_ue_context
                ))
    {
        RRC_UECC_UE_FSM_TRACE(RRC_WARNING,
                "[%s]uecc_x2ap_build_and_send_ue_context_release"
                " failed !!",__FUNCTION__);
    }
}
/*EN-DC_changes_end*/
/* OPTION3X Changes Start */
/****************************************************************************
 *   Function Name  : uecc_fsm_dc_bearer_change_req_handler
 *   Inputs         : uecc_ue_context_t* - pointer to UE context
 *                    void*   - api data
 *   Outputs        : None
 *   Returns        : None
 *   Description    : This is the handler function for the Event
 *                    UECC_EV_RRM_DC_BEARER_CHANGE_REQ
 *****************************************************************************/
void uecc_fsm_dc_bearer_change_req_handler(
    uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*
     * Set the State of Main FSM to UE CONNECTION RECONFIGURATION ONGOING
     */
    UECC_FSM_SET_MAIN_STATE(p_ue_context,
        &uecc_fsm_ue_connection_reconfiguration_ongoing, PNULL);

    /*
     * Start the DC bearer change Procedure
     */
    uecc_fsm_start_procedure(p_ue_context,
            &uecc_fsm_rrm_dc_bearer_change_procedure, p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_change_cnf_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*              - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives 
 *                  the UECC_EV_RRM_DC_BEARER_CHANGE_CNF event.
 ****************************************************************************/
void
uecc_fsm_rrm_dc_bearer_change_cnf_handler
(
    uecc_ue_context_t*   p_ue_context,
    void*                p_api
)
{
    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",     
            p_api);

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Set the state of main FSM to UE CONNECTED */
    UECC_FSM_SET_MAIN_STATE(p_ue_context, &uecc_fsm_ue_connected, PNULL);

#ifdef MEM_PROFILE_ON
    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                   p_ue_context->p_gb_context->facility_name,RRC_INFO,
                   "\n *****Sizeof UE CONTEXT after ERAB RECONFIG is successful %d ",
                   sizeof(uecc_ue_context_t));

    qvPoolStats();
#endif

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_change_cnf_failure_ue_rel_handler
 *
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRM_DC_BEARER_CHANGE_CNF_FAILURE_UE_REL message
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_cnf_failure_ue_rel_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    s1ap_Cause cause;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    cause.t = T_s1ap_Cause_radioNetwork ;
    cause.u.radioNetwork= s1ap_unspecified_2;

    if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                p_ue_context,
                &cause ) )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[s1ap_UEContextReleaseRequest] Delivery failed");
    }
    UECC_FSM_SET_MAIN_STATE(
            p_ue_context, &uecc_fsm_ue_wait_for_connection_release, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* OPTION3X Changes End */

/****************************************************************************
 * Function Name  : rrc_check_erab_for_bearer_or_ue_release_trigger
 * Input          : uecc_ue_context_t* - pointer to UE context
 * Output         : None
 * Returns        : release_type - UE RELEASE/ERAB RELEASE
 * Description    : This function checks erab list to trigger either UE release
 *                  or ERAB release procedure
 *                  in UECC_UE_CONNECTED states.
 ****************************************************************************/
U8
rrc_check_erab_for_bearer_or_ue_release_trigger
(
    uecc_ue_context_t   *p_ue_context
)
{
    U8    release_type = RRC_NULL;
    U8    erab_count = RRC_NULL;
    U8    sn_term_scg_erab_count = RRC_NULL;
    U8    sn_term_split_erab_count = RRC_NULL;
    U8    dc_erab_count = RRC_NULL;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    for ( erab_count = RRC_NULL; erab_count < MAX_ERAB_COUNT;
        erab_count++ )
    {
        if ( PNULL != p_ue_context->p_e_rab_list[erab_count] )
        {
            if ( p_ue_context->p_e_rab_list[erab_count]->m.DC_BearerTypePresent )
            {
                RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"DC_BearerTypePresent");
                if ( SN_TERMINATED_SCG_BEARER == p_ue_context->p_e_rab_list[erab_count]->
                        dc_bearer_type )
                {
                    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"SN terminated SCG bearer present");
                    sn_term_scg_erab_count++;
                }
                else if ( SN_TERMINATED_SPLIT_BEARER == p_ue_context->p_e_rab_list[erab_count]->
                        dc_bearer_type )
                {
                    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"SN terminated split bearer present");
                    sn_term_split_erab_count++;
                }
                dc_erab_count++;
            }
        }
    }

    if ( dc_erab_count && ( p_ue_context->e_rab_list_count == sn_term_scg_erab_count ) )
    {
        /*
         * Trigger UE release
         */
        RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"All erabs are SN terminated scg bearer,"
            "trigger UE release");
        release_type = UE_REL;
    }
    else if ( dc_erab_count && ( p_ue_context->e_rab_list_count == sn_term_split_erab_count ) )
    {
        /*
         * Trigger UE release
         */
        RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"All erabs are SN terminated split bearer,"
            "trigger UE release");
       release_type = UE_REL;
    }
    else if ( dc_erab_count && ( p_ue_context->e_rab_list_count == ( sn_term_split_erab_count +
        sn_term_scg_erab_count ) ) )
    {
        /*
         * Trigger UE release
         */
        RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"All erabs are either SN terminated split or scg bearer,"
            "trigger UE release");
       release_type = UE_REL;
    }
    else
    {
        /*
         * Trigger ERAB release
         */
        RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Some erabs are either SN terminated split or scg bearer"
            "or MCG beareri, trigger ERAB release");
       release_type = ERAB_REL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return release_type;
}

/****************************************************************************
 * Function Name  : uecc_fsm_x2ap_sgnb_initiated_sgnb_release_required_handler
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_X2AP_SGNB_RELEASE_REL_REQ message
 *                  in UECC_UE_CONNECTED states.
 ****************************************************************************/
void uecc_fsm_x2ap_sgnb_initiated_sgnb_release_required_handler
(
    uecc_ue_context_t   *p_ue_context, 
    void                *p_api 
)
{
    s1ap_Cause    cause;
    U8            release_type = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,RRC_INFO,
            "Inside the Handler for SgNB Initiated SgNB release required"
            " in UE Connected state ");

    /*
     * Build and send SgNB release confirm to SgNB
     */
    uecc_x2ap_build_and_send_sgnb_initiated_sgnb_release_confirm(p_ue_context);

    release_type = rrc_check_erab_for_bearer_or_ue_release_trigger(p_ue_context);
    p_ue_context->release_type = release_type;
    p_ue_context->sgnbInitiatedSgnbReleaseFlag = RRC_TRUE;

    if ( UE_REL == release_type )
    {
        /*
         * start UE release procedure
         */
        cause.u.radioNetwork = RRC_THREE;
        cause.t = RRC_ONE;
        if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                    p_ue_context,
                    &cause ))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[s1ap_UEContextReleaseRequest] Delivery failed");
        }                       

        /*
         * Clear the share data in UE Context
         */
        if (p_ue_context->share_data.p_uecc_rm_data != PNULL)
        {
            rrc_mem_free(p_ue_context->share_data.p_uecc_rm_data);
            p_ue_context->share_data.p_uecc_rm_data = PNULL;
        }

        if ( RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context,
                    UECC_UE_MME_GUARD_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[UECC_UE_MME_GUARD_TIMER]"
                    "Timer Start Failed");
        }

        p_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag =
            RRC_FALSE;

        UECC_FSM_SET_MAIN_STATE(
                p_ue_context, &uecc_fsm_ue_wait_for_s1ap_ue_ctx_rel_cmd, PNULL);
    }
    else if ( ERAB_REL == release_type )
    {
        UECC_FSM_SET_MAIN_STATE(p_ue_context, 
                &uecc_fsm_ue_connection_reconfiguration_ongoing, PNULL);

        uecc_fsm_start_procedure(p_ue_context,
             &uecc_fsm_ue_endc_erb_release_procedure, p_api);
    }

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Freed if any message is buffered */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {       
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }       
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_x2ap_sgnb_release_timery_expiry_handler
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_X2AP_SGNB_RELEASE_TIMER_EXPIRY message
 *                  in uecc_fsm_ue_connection_release_ongoing state.
 ****************************************************************************/
void uecc_fsm_x2ap_sgnb_release_timery_expiry_handler
(
    uecc_ue_context_t   *p_ue_context, 
    void                *p_api 
)
{
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,RRC_INFO,
            "Inside the Handler for SgNB release timer expiry");

    uecc_fsm_x2ap_ue_context_release_handler(p_ue_context);

    RRC_UECC_UPDATE_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);

    if ( UE_REL == p_ue_context->release_type )
    {
        /*
         * Send event to UE Main FSM
         */
        uecc_event_queue_push_event(
                UECC_EV_UE_RELEASED_INT,
                p_ue_context,
                PNULL);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_x2ap_reset_msg_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  X2AP_RESET_MSG_IND message
 *                  in uecc_fsm_ue_connected state
 ****************************************************************************/
void uecc_fsm_ue_x2ap_reset_msg_ind_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    s1ap_Cause    cause;
    U8            release_type = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
       
    release_type = rrc_check_erab_for_bearer_or_ue_release_trigger(p_ue_context);
    p_ue_context->release_type = release_type;
    p_ue_context->sgnbInitiatedSgnbReleaseFlag = RRC_TRUE;

    if ( UE_REL == release_type )
    {
        /*
         * start UE release procedure
         */
        cause.u.radioNetwork = RRC_THREE;
        cause.t = RRC_ONE;
        if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                    p_ue_context,
                    &cause ))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[s1ap_UEContextReleaseRequest] Delivery failed");
        }                       

        /*
         * Clear the share data in UE Context
         */
        if (p_ue_context->share_data.p_uecc_rm_data != PNULL)
        {
            rrc_mem_free(p_ue_context->share_data.p_uecc_rm_data);
            p_ue_context->share_data.p_uecc_rm_data = PNULL;
        }

        if ( RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context,
                    UECC_UE_MME_GUARD_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[UECC_UE_MME_GUARD_TIMER]"
                    "Timer Start Failed");
        }

        p_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag =
            RRC_FALSE;

        UECC_FSM_SET_MAIN_STATE(
                p_ue_context, &uecc_fsm_ue_wait_for_s1ap_ue_ctx_rel_cmd, PNULL);
    }
    else if ( ERAB_REL == release_type )
    {
        UECC_FSM_SET_MAIN_STATE(p_ue_context, 
                &uecc_fsm_ue_connection_reconfiguration_ongoing, PNULL);

        uecc_fsm_start_procedure(p_ue_context,
             &uecc_fsm_ue_endc_erb_release_procedure, p_api);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* Error_Handling_changes*/
/****************************************************************************
 * Function Name  : uecc_fsm_endc_x2ap_error_ind_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_X2AP_ERROR_INDICATION
 ****************************************************************************/
void uecc_fsm_endc_x2ap_error_ind_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
    )
{
    U8 release_type = RRC_NULL;
    s1ap_Cause  cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*release the x2ap context for this perticular UE*/
    uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);

    /*check if all the RABS in the UE are SCG or combination of both SCG and MCG,
     * if all SCG then releasethe UE, eles release RAB only*/
    release_type = rrc_check_erab_for_bearer_or_ue_release_trigger(p_ue_context);

    /*Finish the current ongoing activity*/
    uecc_fsm_finish_active_procedure(p_ue_context);

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Freed if any message is buffered */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    /*since release type is UE_REL, release the UE as all bearers are SCG Bearer*/
    if(UE_REL == release_type)
    {
        /*initiate the ENB initiated Ue Release proc*/
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_release_due_to_eutran_generated_reason;  

        if(RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                    p_ue_context, &cause))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[s1ap_UEContextReleaseRequest] Delivery failed");
        }

        /*Clear the share data in UE Context*/
        if (p_ue_context->share_data.p_uecc_rm_data != PNULL)
        {
            rrc_mem_free(p_ue_context->share_data.p_uecc_rm_data);
            p_ue_context->share_data.p_uecc_rm_data = PNULL;
        }

        if ( RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context,
                    UECC_UE_MME_GUARD_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[UECC_UE_MME_GUARD_TIMER]"
                    "Timer Start Failed");
        }

        p_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag =
            RRC_FALSE;
        
        UECC_FSM_SET_MAIN_STATE(
                p_ue_context, &uecc_fsm_ue_wait_for_s1ap_ue_ctx_rel_cmd, PNULL);
    }
    else if (ERAB_REL == release_type)
    {
        p_ue_context->release_type = ERAB_REL;

        UECC_FSM_SET_MAIN_STATE(p_ue_context,
                &uecc_fsm_ue_connection_reconfiguration_ongoing, PNULL);

        uecc_fsm_start_procedure(p_ue_context,
                &uecc_fsm_ue_endc_erb_release_procedure, p_api);
    }

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Freed if any message is buffered */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* Error_Handling_changes*/
#endif
