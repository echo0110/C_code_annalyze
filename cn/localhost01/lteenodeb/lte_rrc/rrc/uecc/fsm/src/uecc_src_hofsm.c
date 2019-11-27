/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2010 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_src_hofsm.c,v 1.5 2010/12/01 13:03:09 gur19827 Exp $
 ****************************************************************************
 *
 *  File Description : The file uecc_src_hofsm.c contains the definitions of
 *                     Source Handover FSM procedure, activities, states and
 *                     FSM failure Handlers. Source handover FSM deals with
 *                     management of Resource Allocation at Source ENB during
 *                     Handover.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_src_hofsm.c,v $
 * Revision 1.5  2010/12/01 13:03:09  gur19827
 * Merged Inter RAT HO
 *
 * Revision 1.4  2010/10/29 14:52:02  gur04518
 * Updated source HO testing fwk
 *
 * Revision 1.3  2010/10/14 12:53:14  gur18569
 * fixed memory leaks
 *
 * Revision 1.1.2.45  2010/10/14 10:22:18  gur25381
 * fixed memory leak
 *
 * Revision 1.1.2.44  2010/10/08 13:08:55  gur18569
 * updated failure causes
 *
 * Revision 1.1.2.43  2010/10/08 05:08:10  gur04518
 * added check for erab Id extracted from LC Id
 *
 * Revision 1.1.2.42  2010/10/07 11:25:15  gur04518
 * updated for kclockwork warning
 *
 * Revision 1.1.2.41  2010/10/07 08:51:44  gur04518
 * updated and put overall timer stop
 *
 * Revision 1.1.2.40  2010/10/07 06:51:01  gur04518
 * added timer stop activity in finish_fail and finish_success action
 *
 * Revision 1.1.2.39  2010/10/06 11:45:41  gur22047
 * compiler warnings removed
 *
 * Revision 1.1.2.38  2010/10/06 06:04:14  gur04518
 * removed the assert check
 *
 * Revision 1.1.2.37  2010/10/05 12:37:08  gur22047
 * compilation warning removed
 *
 * Revision 1.1.2.36  2010/10/04 07:32:24  gur25381
 * Included checks of Direct Path and missing enb id
 *
 * Revision 1.1.2.35  2010/10/02 06:17:16  gur04518
 * Fixed spelling mistake
 *
 * Revision 1.1.2.34  2010/10/01 11:36:40  gur16768
 * updated the cause value
 *
 * Revision 1.1.2.33  2010/10/01 11:03:12  gur16768
 * modified failure action at received failure cnf for delete for forwarding request
 *
 * Revision 1.1.2.32  2010/09/29 11:56:34  gur04518
 * updated handling on HO Cancel from RRM
 *
 * Revision 1.1.2.31  2010/09/28 18:04:52  gur16768
 * updated to remove the p_api check
 *
 * Revision 1.1.2.30  2010/09/28 17:53:45  gur16768
 * updated for pushing ue_rel_int to child FSM
 *
 * Revision 1.1.2.29  2010/09/28 07:05:01  gur16768
 * update for allocating memory to hold ctx rel command
 *
 * Revision 1.1.2.28  2010/09/28 06:12:11  gur04518
 * Fixed traces
 *
 * Revision 1.1.2.27  2010/09/28 05:20:27  gur04518
 * Corrected ERAB array loop
 *
 * Revision 1.1.2.26  2010/09/23 04:44:46  gur18569
 * removed non gbr bearer check at start of ho proc in src
 *
 * Revision 1.1.2.25  2010/09/22 15:11:21  gur25381
 * updated for kclockwork warning fixes
 *
 * Revision 1.1.2.24  2010/09/22 14:39:52  gur16768
 * put the bitmaks check for failed to establish erab list before processing the list
 *
 * Revision 1.1.2.23  2010/09/21 08:29:51  gur16768
 * updated to change against the log senity type
 *
 * Revision 1.1.2.22  2010/09/17 05:05:53  gur22047
 * updated for stopping overall timer expiry
 *
 * Revision 1.1.2.21  2010/09/16 11:51:28  gur16768
 * update for copying the asn1_transportLayerAddress
 *
 * Revision 1.1.2.20  2010/09/16 10:39:17  gur18569
 * fixed UT bug
 *
 * Revision 1.1.2.19  2010/09/13 13:06:49  gur16768
 * updated for kclockwork warnings
 *
 * Revision 1.1.2.18  2010/09/11 16:17:24  gur16768
 * updated
 *
 * Revision 1.1.2.17  2010/09/09 12:21:27  gur18569
 * ut bug fix in src ho
 *
 * Revision 1.1.2.16  2010/09/09 06:55:45  gur16768
 * updated for delete_for_forwarding_tunnel_cnf radio_link_failure
 *
 * Revision 1.1.2.15  2010/09/09 06:29:25  gur16768
 * changed for timer expiry at delete_forwarding_tunnel_ongoing
 *
 * Revision 1.1.2.14  2010/09/08 12:41:09  gur16768
 * updated for sn_hfn_status_response
 *
 * Revision 1.1.2.13  2010/09/07 16:37:53  gur16768
 * added is_rrm_ho_cancel_receive in ho_info present ho_context
 *
 * Revision 1.1.2.12  2010/09/07 15:04:07  gur16768
 * added function uecc_fsm_valiating_erab_config_for_ho_procedure
 *
 * Revision 1.1.2.11  2010/09/07 05:32:17  gur16768
 * changed to handle cell delete from csc
 *
 * Revision 1.1.2.10  2010/09/04 12:47:05  gur16768
 * updated for UT cases
 *
 * Revision 1.1.2.9  2010/08/30 12:22:29  gur16768
 * modified for src ho
 *
 * Revision 1.1.2.8  2010/08/13 11:40:15  gur16768
 * update uecc_fsm_src_ho_sn_hfn_status_resp_handler
 *
 * Revision 1.1.2.7  2010/08/10 14:19:28  gur16768
 * code review comments incorporate
 *
 * Revision 1.1.2.6  2010/08/10 13:56:05  gur16768
 * code review comments incorporated
 *
 * Revision 1.1.2.5  2010/08/10 10:03:06  gur18569
 * incorported review comments for ho
 *
 * Revision 1.1.2.4  2010/07/21 08:34:41  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.1.2.3  2010/07/16 05:19:07  gur22047
 * review comments incorporation
 *
 * Revision 1.1.2.2  2010/07/15 05:50:37  gur16768
 * code review comments incorporate
 *
 * Revision 1.1.2.1  2010/06/28 12:39:59  gur16768
 * file to handle uecc ho realted child fsm events at source enb side
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Header File Includes
 ****************************************************************************/
#include "rrc_asn_enc_dec.h"
#include "uecc_logger.h"
#include "uecc_rcrfsm.h"
#include "uecc_llim.h"
#include "uecc_rrm.h"
#include "uecc_s1ap.h"
#include "uecc_x2ap.h"
#include "uecc_ue_timer_utils.h"
#include "uecc_common.h"
#include "uecc_fsm_engine.h"
#include "rrc_s1ap_asn_intrl_dec_3gpp.h"
#include "uecc_icsp.h"
#include "rrc_oam_intf.h"
#include "uecc_src_hofsm.h"
#include "uecc_trg_hofsm.h"
#include "uecc_defaults.h"
#include "rrc_s1u_intf.h"
#include "uecc_utils.h"

#include "rrc_intrl_api.h"
#include "uecc_ue_timer_utils.h"
#include "rrc_msg_mgmt.h"
#include "rrc_shared_stats.h"

/*securitykey*/
#include "uecc_ue_context_modfsm.h"
/*securitykey*/


/****************************************************************************
  Private Definitions
 ****************************************************************************/

#define RRC_UECC_SRC_HO_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,\
            p_ue_context->ue_index,\
            p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
##__VA_ARGS__)

#define IS_RE_ESTABLISHMENT_REQ_RECEIVED( p_ue_context ) \
    ((PNULL != (p_ue_context->ho_info.p_ho_info->\
                p_src_ho_info->p_re_establishment_req))? RRC_TRUE : RRC_FALSE )

#define IS_SRB_DATA_STATUS_IND_RECEIVED( p_ue_context ) \
    (p_ue_context->ho_info.p_ho_info->p_src_ho_info->\
     is_srb_data_ind_received )

#define SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context)\
    (p_ue_context->share_data.uecc_rcr_data.\
     rrc_connection_release_not_required_flag = RRC_TRUE)

#define IS_S1U_RECONFIGURED( p_ue_context ) \
    (p_ue_context->ho_info.p_ho_info->p_src_ho_info->\
     s1u_reconfigured )

#define IS_RLF_RECEIVED( p_ue_context ) \
    (p_ue_context->ho_info.p_ho_info->p_src_ho_info->\
     p_radio_link_failure_ind )

#define IS_RRM_HO_CANCEL_RECEIVED( p_ue_context ) \
    (p_ue_context->ho_info.p_ho_info->p_src_ho_info->\
     is_rrm_ho_cancel_receive )

#define IS_RESET_RECEIVED( p_ue_context ) \
     (p_ue_context->x2ap_reset_recvd_flag) 

/*BUG 604 changes start*/
#define SET_S1U_RELEASE_NOT_REQ_FLAG(p_ue_context)\
    (p_ue_context->share_data.uecc_rcr_data.\
     s1u_release_not_req_flag = RRC_TRUE)
/*BUG 604 changes stop*/

/*SPR_17151_START*/
#define INVALID_OLD_ENB_UE_X2AP_ID      0xFFFF
/*SPR_17151_END*/
extern rrc_return_et uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind
(
    uecc_ue_context_t*  p_uecc_ue_context
);
/*SPR_19166_START*/
void rrc_fill_inter_rat_cgi_info(
        rrc_inter_rat_global_cgi_t *p_target_cgi,
        rrc_rrm_ho_required_t *p_rrc_rrm_ho_required);

S32 rrc_compare_inter_rat_target_cgi(
        rrc_inter_rat_global_cgi_t target_cgi,
        rrc_rrm_ho_required_t *p_rrc_rrm_ho_required); 
/*SPR_19166_END*/
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static void uecc_fsm_source_handover_procedure_constructor(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
        );

static void uecc_fsm_source_handover_procedure_finish_success(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
        );

static void uecc_fsm_source_handover_procedure_finish_fail(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
        );

/*SPR_19066_START*/
/* State UECC_SRC_HO_GET_HO_PREP_INFO_ONGOING */
static void uecc_fsm_src_ho_get_ho_prep_info_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
        );

/* Code Removed */

static void 
uecc_fsm_src_ho_get_ho_prep_info_ongoing_timer_expiry_handler(
        uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
        void                *p_api_data             /* Input API buffer */
        );

static void uecc_fsm_src_ho_llim_get_ho_prep_info_ongoing_re_establishment_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void uecc_fsm_src_ho_get_ho_prep_info_ongoing_csc_cell_delete_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );
/*SPR_19066_END*/

static void uecc_fsm_src_ho_prep_ongoing_csc_cell_delete_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_csc_cell_delete_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void uecc_fsm_src_ho_wait_for_ctx_rel_cmd_csc_cell_delete_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );
static void uecc_fsm_src_ho_delete_fwding_tunnel_csc_cell_delete_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

/*SPR_19066_START*/
static void  uecc_fsm_src_ho_get_ho_prep_info_ongoing_s1u_reset_req_handler(
/*SPR_19066_END*/
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_prep_ongoing_s1ap_reset_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_s1u_reset_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_wait_for_ctx_rel_cmd_s1_reset_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_delete_fwding_tunnel_s1_reset_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

/*SPR_19066_START*/
static void  uecc_fsm_src_ho_get_ho_prep_info_ongoing_ue_release_req_handler(
/*SPR_19066_END*/
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_prep_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_rrm_ho_cmd_enb_status_s1u_reconfig_wait_for_srb_data_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_wait_for_ctx_rel_cmd_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_wait_for_x2ap_ctx_rel_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_delete_fwd_tunnel_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

/*SPR_19066_START*/
static void  uecc_fsm_src_ho_get_ho_prep_info_ongoing_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_prep_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_rrm_ho_cmd_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_enb_status_transfer_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_s1u_reconfig_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_wait_for_srb_data_wait_for_ctx_rel_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_wait_for_s1u_roll_back_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_get_ho_prep_info_ongoing_llim_radio_link_failure_ind_handler(
/*SPR_19066_END*/
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_ho_prep_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_rrm_ho_cmd_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_enb_status_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_s1u_reconfig_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

/* SPR 16750 Fix Start */
/* Code Removed */
/* SPR 16750 Fix Stop */

static void  uecc_fsm_src_ho_wait_for_ctx_rel_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_wait_for_srb_data_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void uecc_fsm_src_ho_wait_for_srb_data_x2ap_error_indication_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_s1u_delete_fwd_tunnel_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

/* State UECC_SRC_HO_PREP_ONGOING */
static void uecc_fsm_src_ho_prep_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
        );

static void uecc_fsm_src_ho_s1ap_ho_cmd_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        );

static void uecc_fsm_src_ho_x2ap_ho_req_ack_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        );

static void uecc_fsm_src_ho_prep_ongoing_timer_expiry_handler(
        uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
        void                *p_api_data             /* Input API buffer */
        );

static void uecc_fsm_src_ho_llim_ho_prep_re_establishment_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

/* state UECC_SRC_HO_CMD_ONGOING */
static void uecc_fsm_src_ho_cmd_ongoing_init_handler( 
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void uecc_fsm_src_ho_rrm_ho_cmd_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        );

static void uecc_fsm_src_ho_cmd_ongoing_timer_expiry_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void uecc_fsm_src_ho_prep_failure_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        );

static void uecc_fsm_src_x2ap_ho_prep_failure_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        );

/* SPR 16750 Fix Start */
/* Code Removed */
/* SPR 16750 Fix Stop */
static void uecc_fsm_src_x2ap_error_indication_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        );

static void 
uecc_fsm_src_ho_llim_ho_cmd_enb_status_transfer_re_establishment_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

/* state UECC_SRC_HO_RLC_RE_EST_TRANSFER_ONGOING */
static void uecc_fsm_src_ho_llim_re_est_ongoing_init_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );


static void uecc_fsm_src_ho_llim_re_est_resp_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

/* state UECC_SRC_HO_ENB_STATUS_TRANSFER_OR_SN_STATUS_TRANSFER_ONGOING */
static void uecc_fsm_src_ho_enb_status_transfer_ongoing_init_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void uecc_fsm_src_ho_sn_hfn_status_resp_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void uecc_fsm_src_ho_enb_status_transfer_ongoing_timer_expiry_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

/* SPR 16750 Fix Start */
/* Code Removed */
/* SPR 16750 Fix Stop */

/* state UECC_SRC_HO_S1U_RECONFIG_FOR_FORWARDING_ONGOING */
static void uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing_init_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void 
uecc_fsm_src_ho_llim_s1u_reconfig_for_forwarding_tunnel_cnf_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        );

static 
void uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing_timer_expiry_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void 
uecc_fsm_src_ho_llim_s1u_reconfig_for_forwarding_re_establishment_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );


/* state UECC_SRC_HO_WAIT_FOR_SRB_DATA_STATUS_IND */
static void uecc_fsm_src_ho_wait_for_srb_data_status_ind_init_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api
        );

static void uecc_fsm_src_ho_wait_for_srb_data_status_ind_resp_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void
uecc_fsm_src_ho_llim_wait_for_srb_data_status_ctx_release_cmd_re_establishment_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void
uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_re_establishment_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);


/* state UECC_SRC_HO_WAIT_FOR_S1AP_UE_CTX_RELEASE_CMD_OR_X2AP_UE_CTX_RELEASE * */

static void uecc_fsm_src_ho_wait_for_ctx_release_cmd_timer_expiry_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );


/* state UECC_SRC_HO_S1U_DELETE_FORWARDING_TUNNEL_ONGOING */
static void uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_init_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );
static
void uecc_fsm_src_ho_llim_s1u_delete_for_forwarding_cnf_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        );

static 
void uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_timer_expiry_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

/*SPR_19066_START*/
static void uecc_fsm_src_ho_get_ho_prep_info_ongoing_x2ap_reset_req_handler(
/*SPR_19066_END*/
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void uecc_fsm_src_ho_prep_ongoing_x2ap_reset_req_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void uecc_fsm_src_ho_cmd_ongoing_x2ap_reset_req_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void uecc_fsm_src_ho_llim_re_est_ongoing_x2ap_reset_msg_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void uecc_fsm_src_ho_enb_status_transfer_x2ap_reset_msg_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void uecc_fsm_src_ho_s1u_reconfig_x2ap_reset_msg_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void uecc_fsm_src_ho_wait_for_srb_data_and_wait_for_ctx_release_x2ap_reset_msg_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void uecc_fsm_src_s1ap_dl_cdma2000_tunneling_ho_ongoing_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );


/*******Failure processing function prototype******/

static void 
uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_resp(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void 
uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_failure_resp(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void 
uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void uecc_fsm_src_ho_fail_action_send_ho_cancel_ind(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void uecc_fsm_src_ho_fail_action_send_llim_data_buffer_stop_ind(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void uecc_fsm_src_ho_fail_action_send_s1ap_context_release_cmd(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );


static void uecc_fsm_src_ho_fail_action_last(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void uecc_fsm_src_ho_fail_action_last1(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void uecc_fsm_src_ho_fail_action_post_event1(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void uecc_fsm_src_ho_fail_action_post_event2(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void uecc_fsm_src_ho_fail_action_post_event3(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void uecc_fsm_src_ho_fail_action_post_event4(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void uecc_fsm_src_ho_fail_action_post_event5(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void uecc_fsm_src_ho_fail_action_post_event6(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

/* Function to release the ho_info from ue_context */
/*SPR_17348_START*/
void uecc_fsm_src_ho_release_ho_info(
/*SPR_17348_END*/
        uecc_ue_context_t*  p_ue_context
        );

/* Message validation function */

static rrc_bool_t uecc_fsm_valiating_erab_config_for_ho_procedure(
                uecc_ue_context_t*  p_ue_context);

static rrc_bool_t uecc_fsm_valiating_rrm_ho_required_msg(
        uecc_ue_context_t*  p_ue_context,
        void* p_api);

static 
rrc_return_et rrc_validate_peer_cell_info_and_get_peer_enb_context(
        uecc_ue_context_t* p_ue_context,
        rrc_rrm_ho_required_t* p_ho_req,
        U8 *peer_enodeb_id,
        U8 *serving_cell_index
        );

static rrc_bool_t uecc_fsm_valiate_s1ap_ho_command_msg(
        uecc_ue_context_t   *p_ue_context,
        rrc_s1ap_handover_command_t *p_ho_cmd
        );
static void uecc_fsm_src_ho_fail_action_send_error_indication
(
 uecc_ue_context_t*  p_ue_context,
 rrc_bool_et*        p_break
);

static rrc_bool_t uecc_fsm_validate_x2ap_ho_req_ack_msg(
        uecc_ue_context_t   *p_ue_context,
        rrc_x2ap_handover_req_ack_t *p_ho_req_ack
        );

static rrc_return_et uecc_fsm_validate_and_fill_erab_admitted_list(
        uecc_ue_context_t* p_ue_context,
        x2ap_E_RABs_Admitted_List *p_erab_admitted_list
        );

static rrc_return_et uecc_fsm_fill_erab_not_admitted_list(
        uecc_ue_context_t* p_ue_context,
        x2ap_E_RAB_List *p_erab_not_admitted_list
        );

static void uecc_populate_drb_context(
        uecc_ue_context_t  *p_ue_context           /* UECC UE Context */
        );

void convert_rrm_cause_to_x2ap_cause(rrm_cause_t *p_rrmCause, x2ap_Cause *p_cause);

void rrc_src_ho_process_s1u_cnf(
        rrc_uecc_llim_s1u_reconfigure_cnf_t *p_rrc_uecc_llim_s1u_reconfigure_cnf,
        uecc_ue_context_t  *p_ue_context);

/*BUG 604 changes start*/
/*BUG 604 changes stop*/

/*BUG 604 changes start*/
/*BUG 604 changes stop*/

static void uecc_fsm_cco_ongoing_timer_expiry_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

static void uecc_fsm_cco_wait_for_srb_data_timer_expiry_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

/*BUG 604 changes start*/
/*BUG 604 changes stop*/
static void uecc_fsm_reset_ue_entity_ongoing_init_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );
static void uecc_fsm_reset_ue_entity_resp_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );
/*BUG 604 changes start*/
/*BUG 604 changes stop*/

static void uecc_fsm_src_ho_wait_for_data_status_ind_timer_expiry_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        );

/*BUG 604 changes start*/
/*BUG 604 changes stop*/

/* coverity_fix_62556_start */
/* coverity_fix_62556_stop */

static void 
uecc_fsm_src_ho_fail_action_send_enb_ue_ctx_release_req(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void uecc_fsm_src_ho_fail_action_send_llim_reestab_complete_ind(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

static void uecc_fsm_src_ho_llim_ho_src_ho_cmd_ongoing_re_establishment_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_llim_re_est_ongoing_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void  uecc_fsm_src_ho_cmd_ongoing_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

static void uecc_fsm_src_ho_llim_re_est_ongoing_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );


static void  uecc_fsm_rrc_ul_handover_preparation_transfer_handler (
	uecc_ue_context_t  *p_ue_context,
	void               *p_api
	);


static rrc_bool_t uecc_fsm_validate_target_rat_geran_params (
        uecc_ue_context_t           *p_ue_context,
        rrc_target_rat_type_geran_t *p_geran_params);

/* lossy ho changes start */
static void uecc_fsm_validate_lossy_ho_list(
        uecc_ue_context_t *p_ue_context,
        erab_item_list_t *p_erab_lossy_ho_list
        );
/* lossy ho changes stop */
/* SPR:6462 start*/
static void uecc_fsm_src_ho_rlc_reestab_comp_cnf_msg_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );
/* SPR:6462 stop*/

/*BUG 604 changes start*/
/*Bug 357 Fix Start*/
/*Bug 357 Fix Stop*/
/*BUG 604 changes start*/


/*securitykey*/
static void uecc_fsm_src_ho_cleanup_ue_ctx_mod_fsm_data(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
);
/*securitykey*/

/*BUG 604 changes start*/
static void uecc_fsm_s1u_intra_enb_data_fwd_cnf_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );
static void uecc_fsm_s1u_local_path_switch_delete_forwarding_cnf_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

/* SPR 16750 Fix Start */
static void uecc_fsm_src_ho_llim_re_est_x2ap_error_indication_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );
/* SPR 16750 Fix Stop */

/*BUG 604 changes stop*/
/*SPR_19066_START*/
void uecc_fsm_src_ho_llim_ho_prep_info_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        );

static 
rrc_return_t uecc_fsm_src_ho_process_llim_ho_prep_info_resp_data(
        uecc_ue_context_t*           p_ue_context,
        rrc_uecc_llim_ho_prep_info_resp_t* p_rrc_uecc_llim_ho_prep_info_resp 
        );
/*SPR_19066_END*/

/* BUG 12647 Fix Start */
static void uecc_src_lte_intra_enb_ho_hdlr(
       uecc_ue_context_t*              p_ue_context
);

static void  uecc_ue_release_req_lte_intra_enb_ho_hndlr(
        uecc_ue_context_t*  p_ue_context);

static void uecc_ue_rel_lte_intra_enb_ho_update_performance_counter(
        uecc_ue_context_t*  p_ue_context,
        x2ap_Cause          x2apCause,
        U8                  index,
        S32                 cmp_result);

static void uecc_src_lte_intra_enb_ho_update_performance_counter(
                                uecc_ue_context_t*              p_ue_context,
                                x2ap_Cause                      x2apCause,
                                U8                              index);
/* BUG 12647 Fix Stop */

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

static uecc_fsm_handler_record_t 
/*SPR_19066_START*/
uecc_fsm_src_ho_get_ho_prep_info_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
    /*SPR_19066_START*/
        uecc_fsm_src_ho_get_ho_prep_info_ongoing_init_handler},
    { { UECC_EV_LLIM_HO_PREP_INFO_RESP },
        uecc_fsm_src_ho_llim_ho_prep_info_resp_handler  },    
    /*SPR_19066_END*/
    { { UECC_EV_HO_PREP_TIMER_EXPIRY            },
        uecc_fsm_src_ho_get_ho_prep_info_ongoing_timer_expiry_handler},
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_src_ho_get_ho_prep_info_ongoing_csc_cell_delete_req_handler  },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ          },
        uecc_fsm_src_ho_llim_get_ho_prep_info_ongoing_re_establishment_handler},
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_src_ho_get_ho_prep_info_ongoing_s1u_reset_req_handler  },
    { { UECC_EV_UE_RELEASE_REQ            },
        uecc_fsm_src_ho_get_ho_prep_info_ongoing_ue_release_req_handler  },
    { { UECC_EV_RRM_HO_CANCEL_REQ        },
        uecc_fsm_src_ho_get_ho_prep_info_ongoing_rrm_ho_cancel_req_handler  },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND    },
        uecc_fsm_src_ho_get_ho_prep_info_ongoing_llim_radio_link_failure_ind_handler  },
    { { UECC_EV_X2AP_RESET_MSG_IND          },
        uecc_fsm_src_ho_get_ho_prep_info_ongoing_x2ap_reset_req_handler  },
/*SPR_19066_END*/
    /*BUG 604 changes start*/
    /*This state will not be handled in case of INTRAcELL hO- so should be
     * removed*/
    /*{ { UECC_EV_LLIM_RACH_RES_RESP      },
        uecc_fsm_src_ho_mac_rach_resp_handler             },    */
    /*{ { UECC_EV_INTRA_CELL_HO_TIMER_EXPIRY            },
        uecc_fsm_intra_cell_ho_ongoing_timer_expiry_handler},*/
    /*BUG 604 changes stop*/
    { { UECC_EV_RRC_UL_HANDOVER_PREPARATION_TRANSFER  },
	uecc_fsm_rrc_ul_handover_preparation_transfer_handler}, 
    { { UECC_LAST_ID                    },  PNULL            }
};

static uecc_fsm_handler_record_t uecc_fsm_src_ho_prep_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT          },
        uecc_fsm_src_ho_prep_ongoing_init_handler       },
    { { UECC_EV_S1AP_HO_CMD         },
        uecc_fsm_src_ho_s1ap_ho_cmd_resp_handler               },
    { { UECC_EV_X2AP_HO_REQ_ACK         },
        uecc_fsm_src_ho_x2ap_ho_req_ack_resp_handler               },
    { { UECC_EV_S1AP_HO_PREPARATION_FAIL            },
        uecc_fsm_src_ho_prep_failure_handler},
    { { UECC_EV_X2AP_HO_PREPARATION_FAIL            },
        uecc_fsm_src_x2ap_ho_prep_failure_handler},
    { { UECC_EV_HO_PREP_TIMER_EXPIRY            },
        uecc_fsm_src_ho_prep_ongoing_timer_expiry_handler},
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_src_ho_prep_ongoing_csc_cell_delete_req_handler  },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_src_ho_llim_ho_prep_re_establishment_handler},
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_src_ho_prep_ongoing_s1ap_reset_req_handler  },
    { { UECC_EV_UE_RELEASE_REQ            },
        uecc_fsm_src_ho_prep_ue_release_req_handler  },
    { { UECC_EV_RRM_HO_CANCEL_REQ        },
        uecc_fsm_src_ho_prep_rrm_ho_cancel_req_handler  },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND    },
        uecc_fsm_src_ho_ho_prep_llim_radio_link_failure_ind_handler  },
    /* SPR 16750 Fix Start */
    /* Code Removed */
    /* SPR 16750 Fix Stop */
    { { UECC_EV_X2AP_RESET_MSG_IND        },
        uecc_fsm_src_ho_prep_ongoing_x2ap_reset_req_handler  },
    { { UECC_EV_X2AP_ERROR_INDICATION            },
        uecc_fsm_src_x2ap_error_indication_handler},	
    { { UECC_EV_S1AP_DL_CDMA2000_TUNNELING_HO_ONGOING        },
        uecc_fsm_src_s1ap_dl_cdma2000_tunneling_ho_ongoing_handler  },	

    { { UECC_LAST_ID                    },  PNULL                       }
};

static uecc_fsm_handler_record_t uecc_fsm_src_ho_cmd_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT          },
        uecc_fsm_src_ho_cmd_ongoing_init_handler       },
    { { UECC_EV_RRM_HO_CMD_RESP         },
        uecc_fsm_src_ho_rrm_ho_cmd_resp_handler               },
    { { UECC_EV_HO_OVERALL_TIMER_EXPIRY            },
        uecc_fsm_src_ho_cmd_ongoing_timer_expiry_handler},
   { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_csc_cell_delete_req_handler  },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_src_ho_llim_ho_src_ho_cmd_ongoing_re_establishment_handler},
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_s1u_reset_req_handler  },
    { { UECC_EV_UE_RELEASE_REQ            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_s1u_reconfig_wait_for_srb_data_ue_release_req_handler  },
    { { UECC_EV_RRM_HO_CANCEL_REQ        },
        uecc_fsm_src_ho_rrm_ho_cmd_rrm_ho_cancel_req_handler  },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND    },
        uecc_fsm_src_ho_cmd_ongoing_llim_radio_link_failure_ind_handler  },
    { { UECC_EV_X2AP_RESET_MSG_IND       },
        uecc_fsm_src_ho_cmd_ongoing_x2ap_reset_req_handler  },
    /* SPR 16750 Fix Start */
    { { UECC_EV_X2AP_ERROR_INDICATION            },
        uecc_fsm_src_x2ap_error_indication_handler},/* SPR 16750 Fix Stop */ 	
    { { UECC_LAST_ID                    },  PNULL                       }
};

static
uecc_fsm_handler_record_t uecc_fsm_src_ho_llim_re_est_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT          },
        uecc_fsm_src_ho_llim_re_est_ongoing_init_handler       },
    { { UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP         },
        uecc_fsm_src_ho_llim_re_est_resp_handler               },
    { { UECC_EV_HO_OVERALL_TIMER_EXPIRY            },
        uecc_fsm_src_ho_llim_re_est_ongoing_timer_expiry_handler},
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_csc_cell_delete_req_handler  },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_src_ho_llim_ho_cmd_enb_status_transfer_re_establishment_handler},
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_s1u_reset_req_handler  },
    { { UECC_EV_UE_RELEASE_REQ            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_s1u_reconfig_wait_for_srb_data_ue_release_req_handler  },
    { { UECC_EV_RRM_HO_CANCEL_REQ        },
        uecc_fsm_src_ho_llim_re_est_ongoing_rrm_ho_cancel_req_handler  },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND    },
        uecc_fsm_src_ho_rrm_ho_cmd_llim_radio_link_failure_ind_handler  },
    { { UECC_EV_X2AP_RESET_MSG_IND     },
        uecc_fsm_src_ho_llim_re_est_ongoing_x2ap_reset_msg_handler  },
    /*BUG 604 changes start*/
    /*{ { UECC_EV_INTRA_CELL_HO_TIMER_EXPIRY            },
        uecc_fsm_intra_cell_ho_ongoing_timer_expiry_handler},*/
    /*BUG 604 changes stop*/
    /* SPR 16750 Fix Start */
    { { UECC_EV_X2AP_ERROR_INDICATION            },
        uecc_fsm_src_ho_llim_re_est_x2ap_error_indication_handler},/* SPR 16750 Fix Stop */ 	
    { { UECC_EV_CCO_TIMER_EXPIRY      },
        uecc_fsm_cco_ongoing_timer_expiry_handler},
    { { UECC_LAST_ID                    },  PNULL                       }
};

static 
uecc_fsm_handler_record_t uecc_fsm_src_ho_enb_status_transfer_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT          },
        uecc_fsm_src_ho_enb_status_transfer_ongoing_init_handler       },
    { { UECC_EV_LLIM_SN_HFN_STATUS_RESP         },
        uecc_fsm_src_ho_sn_hfn_status_resp_handler               },
    { { UECC_EV_HO_OVERALL_TIMER_EXPIRY            },
        uecc_fsm_src_ho_enb_status_transfer_ongoing_timer_expiry_handler},
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_csc_cell_delete_req_handler  },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_src_ho_llim_ho_cmd_enb_status_transfer_re_establishment_handler},
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_s1u_reset_req_handler  },
    { { UECC_EV_UE_RELEASE_REQ            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_s1u_reconfig_wait_for_srb_data_ue_release_req_handler  },
    { { UECC_EV_RRM_HO_CANCEL_REQ        },
        uecc_fsm_src_ho_enb_status_transfer_rrm_ho_cancel_req_handler  },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND    },
        uecc_fsm_src_ho_enb_status_llim_radio_link_failure_ind_handler  },
    { { UECC_EV_X2AP_RESET_MSG_IND    },
        uecc_fsm_src_ho_enb_status_transfer_x2ap_reset_msg_handler  },
    /*BUG 604 changes start*/
   /* { { UECC_EV_INTRA_CELL_HO_TIMER_EXPIRY            },
        uecc_fsm_intra_cell_ho_ongoing_timer_expiry_handler},*/
    /*BUG 604 changes stop*/
    { { UECC_EV_X2AP_ERROR_INDICATION    },
    /* SPR 16750 Fix Start */
        uecc_fsm_src_ho_llim_re_est_x2ap_error_indication_handler},/* SPR 16750 Fix Stop */ 	
    { { UECC_EV_CCO_TIMER_EXPIRY      },
        uecc_fsm_cco_ongoing_timer_expiry_handler},
    /* SPR:6462 start*/
    { { UECC_EV_LLIM_REESTAB_UE_ENTITY_COMP_CNF      },
        uecc_fsm_src_ho_rlc_reestab_comp_cnf_msg_handler},
    /* SPR:6462 stop*/
    { { UECC_LAST_ID                    },  PNULL                       }
};

static uecc_fsm_handler_record_t 
uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT          },
        uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing_init_handler    },
    { { UECC_EV_LLIM_S1U_RECONFIG_CNF         },
        uecc_fsm_src_ho_llim_s1u_reconfig_for_forwarding_tunnel_cnf_handler },
    { { UECC_EV_HO_OVERALL_TIMER_EXPIRY            },
        uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing_timer_expiry_handler},
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_csc_cell_delete_req_handler  },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_src_ho_llim_s1u_reconfig_for_forwarding_re_establishment_handler},
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_s1u_reset_req_handler  },
    { { UECC_EV_UE_RELEASE_REQ            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_s1u_reconfig_wait_for_srb_data_ue_release_req_handler  },
    { { UECC_EV_RRM_HO_CANCEL_REQ        },
        uecc_fsm_src_ho_s1u_reconfig_rrm_ho_cancel_req_handler  },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND    },
        uecc_fsm_src_ho_s1u_reconfig_llim_radio_link_failure_ind_handler },
    { { UECC_EV_X2AP_RESET_MSG_IND     },
        uecc_fsm_src_ho_s1u_reconfig_x2ap_reset_msg_handler  },
    { { UECC_EV_X2AP_ERROR_INDICATION            },
    /* SPR 16750 Fix Start */
        uecc_fsm_src_ho_llim_re_est_x2ap_error_indication_handler},/* SPR 16750 Fix Stop */ 	
    /* SPR:6462 start*/
    { { UECC_EV_LLIM_REESTAB_UE_ENTITY_COMP_CNF      },
        uecc_fsm_src_ho_rlc_reestab_comp_cnf_msg_handler},
/*BUG 604 changes start*/
    { { UECC_EV_LLIM_S1U_INTRA_ENB_DATA_FWD_CNF      },
        uecc_fsm_s1u_intra_enb_data_fwd_cnf_handler},
/*BUG 604 changes stop*/
    /* SPR:6462 stop*/
    { { UECC_LAST_ID                    },  PNULL                       }
};

static uecc_fsm_handler_record_t 
uecc_fsm_src_ho_wait_for_srb_data_status_ind_mht[] =
{
    { { UECC_EV_STATE_INIT          },
        uecc_fsm_src_ho_wait_for_srb_data_status_ind_init_handler       },
    { { UECC_EV_LLIM_SRB_DATA_STATUS_IND         },
        uecc_fsm_src_ho_wait_for_srb_data_status_ind_resp_handler},
    { { UECC_EV_HO_OVERALL_TIMER_EXPIRY            },
        /* CSR 11234 Start */
        uecc_fsm_src_ho_wait_for_data_status_ind_timer_expiry_handler},
    /* CSR 11234 Stop */
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_src_ho_wait_for_ctx_rel_cmd_csc_cell_delete_req_handler  },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_src_ho_llim_wait_for_srb_data_status_ctx_release_cmd_re_establishment_handler},
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_s1u_reset_req_handler  },
    { { UECC_EV_UE_RELEASE_REQ            },
        uecc_fsm_src_ho_wait_for_ctx_rel_cmd_ue_release_req_handler  },
    { { UECC_EV_X2AP_UE_CTX_RELEASE            },
        uecc_fsm_src_ho_wait_for_x2ap_ctx_rel_ue_release_req_handler  },
    { { UECC_EV_RRM_HO_CANCEL_REQ        },
        uecc_fsm_src_ho_wait_for_srb_data_wait_for_ctx_rel_rrm_ho_cancel_req_handler  },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND    },
        uecc_fsm_src_ho_wait_for_srb_data_llim_radio_link_failure_ind_handler  },
    { { UECC_EV_X2AP_RESET_MSG_IND     },
        uecc_fsm_src_ho_wait_for_srb_data_and_wait_for_ctx_release_x2ap_reset_msg_handler },
    /*BUG 604 changes start*/
    /*{ { UECC_EV_INTRA_CELL_HO_TIMER_EXPIRY            },
        uecc_fsm_intra_cell_ho_ongoing_timer_expiry_handler},
    { { UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE                  },
        uecc_fsm_src_ho_wait_for_srb_data_rrc_conn_reconf_comp_handler   },*/
    /*BUG 604 changes stop*/
    { { UECC_EV_X2AP_ERROR_INDICATION            },
        uecc_fsm_src_ho_wait_for_srb_data_x2ap_error_indication_handler}, 
    { { UECC_EV_CCO_TIMER_EXPIRY      },
        uecc_fsm_cco_wait_for_srb_data_timer_expiry_handler},
    /* SPR:6462 start*/
    { { UECC_EV_LLIM_REESTAB_UE_ENTITY_COMP_CNF      },
        uecc_fsm_src_ho_rlc_reestab_comp_cnf_msg_handler},
    /* SPR:6462 stop*/
/*BUG 604 changes start*/
    /*Bug 357 Fix Start*/
    /*{ { UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP         },
        uecc_fsm_src_ho_wait_for_llim_re_est_resp_handler  },
    { { UECC_EV_LLIM_PDCP_REESTABLISH_UE_ENTITY_RESP         },
        uecc_fsm_src_ho_wait_for_llim_pdcp_re_est_resp_handler  },*/
    /*Bug 357 Fix Stop*/
/*BUG 604 changes stop*/
    { { UECC_LAST_ID                    },  PNULL                       }
};

static uecc_fsm_handler_record_t uecc_fsm_src_ho_wait_for_ctx_release_cmd_mht[] =
{
    { { UECC_EV_HO_OVERALL_TIMER_EXPIRY            },
        uecc_fsm_src_ho_wait_for_ctx_release_cmd_timer_expiry_handler},
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_src_ho_delete_fwding_tunnel_csc_cell_delete_req_handler  },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_src_ho_llim_wait_for_srb_data_status_ctx_release_cmd_re_establishment_handler},
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_src_ho_wait_for_ctx_rel_cmd_s1_reset_req_handler  },
    { { UECC_EV_UE_RELEASE_REQ            },
        uecc_fsm_src_ho_wait_for_ctx_rel_cmd_ue_release_req_handler  },
    { { UECC_EV_X2AP_UE_CTX_RELEASE            },
        uecc_fsm_src_ho_wait_for_x2ap_ctx_rel_ue_release_req_handler  },
    { { UECC_EV_RRM_HO_CANCEL_REQ        },
        uecc_fsm_src_ho_wait_for_srb_data_wait_for_ctx_rel_rrm_ho_cancel_req_handler  },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND    },
        uecc_fsm_src_ho_wait_for_ctx_rel_llim_radio_link_failure_ind_handler  },
    { { UECC_EV_X2AP_RESET_MSG_IND   },
        uecc_fsm_src_ho_wait_for_srb_data_and_wait_for_ctx_release_x2ap_reset_msg_handler  },
    /* SPR 16750 Fix Start */
    { { UECC_EV_X2AP_ERROR_INDICATION    },
        uecc_fsm_src_ho_wait_for_srb_data_x2ap_error_indication_handler  },/* SPR 16750 Fix Stop */
    { { UECC_LAST_ID                    },  PNULL                       }
};

static uecc_fsm_handler_record_t 
uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT          },
        uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_init_handler    },
    { { UECC_EV_LLIM_S1U_RECONFIG_CNF   },
        uecc_fsm_src_ho_llim_s1u_delete_for_forwarding_cnf_handler },
    { { UECC_EV_LLIM_GUARD_TIMER_EXPIRY            },
        uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_timer_expiry_handler},
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_src_ho_delete_fwding_tunnel_csc_cell_delete_req_handler  },
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_src_ho_delete_fwding_tunnel_s1_reset_req_handler  },
    { { UECC_EV_UE_RELEASE_REQ            },
        uecc_fsm_src_ho_delete_fwd_tunnel_ue_release_req_handler  },
    { { UECC_EV_RRM_HO_CANCEL_REQ        },
        uecc_fsm_src_ho_wait_for_s1u_roll_back_rrm_ho_cancel_req_handler},
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND    },
        uecc_fsm_src_ho_s1u_delete_fwd_tunnel_llim_radio_link_failure_ind_handler  },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_re_establishment_handler},
    /* SPR:6462 start*/
    { { UECC_EV_LLIM_REESTAB_UE_ENTITY_COMP_CNF      },
        uecc_fsm_src_ho_rlc_reestab_comp_cnf_msg_handler},
    /* SPR:6462 stop*/
/*BUG 604 changes start*/
    { { UECC_EV_LLIM_S1U_LOCAL_PATH_SWITCH_CNF                  },
        uecc_fsm_s1u_local_path_switch_delete_forwarding_cnf_handler},
/*BUG 604 changes stop*/
    { { UECC_LAST_ID                    },  PNULL                       }
};

static 
uecc_fsm_handler_record_t uecc_fsm_reset_ue_entity_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
        uecc_fsm_reset_ue_entity_ongoing_init_handler   },
    { { UECC_EV_LLIM_RESET_UE_ENTITY_RESP   },
        uecc_fsm_reset_ue_entity_resp_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_csc_cell_delete_req_handler  },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_src_ho_llim_ho_cmd_enb_status_transfer_re_establishment_handler},
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_s1u_reset_req_handler  },
    { { UECC_EV_UE_RELEASE_REQ            },
        uecc_fsm_src_ho_rrm_ho_cmd_enb_status_s1u_reconfig_wait_for_srb_data_ue_release_req_handler  },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND    },
        uecc_fsm_src_ho_rrm_ho_cmd_llim_radio_link_failure_ind_handler  },
    /*BUG 604 changes start*/
    /*{ { UECC_EV_INTRA_CELL_HO_TIMER_EXPIRY            },
        uecc_fsm_intra_cell_ho_ongoing_timer_expiry_handler},*/
    /*BUG 604 changes stop*/
    { { UECC_EV_CCO_TIMER_EXPIRY      },
        uecc_fsm_cco_ongoing_timer_expiry_handler},
    { { UECC_LAST_ID                    },  PNULL                       }
};

/*BUG 604 changes start*/
/*BUG 604 changes stop*/


/* Activity states */
/*SPR_17348_START*/
/*SPR_19066_START*/
uecc_fsm_activity_state_t uecc_fsm_src_ho_get_ho_prep_info_ongoing =
/*SPR_17348_END*/
{
    /* .s_name         =  */ (S8*)"UECC_SRC_HO_GET_HO_PREP_INFO_ONGOING",
    /* .s_id           =  */ (S8*)"SRC_HO_LLIM",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */
    uecc_fsm_src_ho_get_ho_prep_info_ongoing_mht 
/*SPR_19066_END*/
};

static uecc_fsm_activity_state_t uecc_fsm_src_ho_prep_ongoing =
{
    /* .s_name         =  */ (S8*)"UECC_SRC_HO_PREP_ONGOING",
    /* .s_id           =  */ (S8*)"SRC_HO",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */ uecc_fsm_src_ho_prep_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_src_ho_cmd_ongoing =
{
    /* .s_name         =  */ (S8*)"UECC_SRC_HO_CMD_ONGOING",
    /* .s_id           =  */ (S8*)"SRC_HO_RRM",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */ uecc_fsm_src_ho_cmd_ongoing_mht
};


static uecc_fsm_activity_state_t uecc_fsm_src_ho_llim_re_est_ongoing =
{
    /* .s_name         =  */ (S8*)"UECC_SRC_HO_LLIM_RE_ESTABLISH_ONGOING",      
    /* .s_id           =  */ (S8*)"SRC_HO",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */ uecc_fsm_src_ho_llim_re_est_ongoing_mht
};



static uecc_fsm_activity_state_t uecc_fsm_src_ho_enb_status_transfer_ongoing =
{
    /* .s_name         =  */ (S8*)"UECC_SRC_HO_ENB_STATUS_TRANSFER_OR_SN_STATUS_TRANSFER_ONGOING",      
    /* .s_id           =  */ (S8*)"SRC_HO",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */ uecc_fsm_src_ho_enb_status_transfer_ongoing_mht
};

static uecc_fsm_activity_state_t 
uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing =
{
    /* .s_name         =  */ (S8*)"UECC_SRC_HO_S1U_RECONFIG_FOR_FORWARDING_ONGOING",
    /* .s_id           =  */ (S8*)"SRC_HO",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */ 
    uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing_mht 
};

static uecc_fsm_activity_state_t uecc_fsm_src_ho_wait_for_srb_data_status_ind =
{
    /* .s_name         =  */ (S8*)"UECC_SRC_HO_WAIT_FOR_SRB_DATA_STATUS_IND",
    /* .s_id           =  */ (S8*)"SRC_HO_LLIM",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */ uecc_fsm_src_ho_wait_for_srb_data_status_ind_mht
};

static uecc_fsm_activity_state_t uecc_fsm_src_ho_wait_for_ctx_release_cmd =
{
    /* .s_name         =  */ (S8*)"UECC_SRC_HO_WAIT_FOR_S1AP_UE_CTX_RELEASE_CMD_OR_X2AP_UE_CTX_RELEASE",
    /* .s_id           =  */ (S8*)"SRC_HO",
    /* .is_need_init   =  */ RRC_FALSE,
    /* .p_mht          =  */ uecc_fsm_src_ho_wait_for_ctx_release_cmd_mht
};

static uecc_fsm_activity_state_t 
uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing =
{
    /* .s_name         =  */ (S8*)"UECC_SRC_HO_S1U_DELETE_FORWARDING_TUNNEL_ONGOING",
    /* .s_id           =  */ (S8*)"SRC_HO_LLIM",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */ 
    uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_reset_ue_entity_ongoing =
{
    /* .s_name         =  */ (S8*)"UECC_SRC_HO_RESET_UE_ENTITY_ONGOING",      
    /* .s_id           =  */ (S8*)"UE_SRC_HO_A",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */ uecc_fsm_reset_ue_entity_ongoing_mht
};

/*BUG 604 changes start*/
/*BUG 604 changes stop*/

/* Activity */
static uecc_fsm_activity_t uecc_fsm_source_handover_activity =
{
    /*  .s_name             = */(S8*)"RRM SRC Handover",
    /* .s_id               = */(S8*)"UE_SRC_HO_A",
    /* .p_first_state      = */
/*SPR_19066_START*/
    &uecc_fsm_src_ho_get_ho_prep_info_ongoing
/*SPR_19066_END*/
};


static uecc_fsm_activity_t*
uecc_fsm_source_handover_procedure_activity_list[] =
{
    &uecc_fsm_source_handover_activity,
    PNULL
};

/* Procedure */
uecc_fsm_procedure_t uecc_fsm_source_handover_procedure =
{
    /*.s_name             = */(S8*)"UE source handover",
    /*.s_id               = */(S8*)"UE_SRC_HO_P",
    /*.constructor_f      = */uecc_fsm_source_handover_procedure_constructor,
    /*.finish_success_f   = */uecc_fsm_source_handover_procedure_finish_success,
    /*.finish_fail_f      = */uecc_fsm_source_handover_procedure_finish_fail,
    /*.activity_list      = */uecc_fsm_source_handover_procedure_activity_list,
/*SPR_19066_START*/
    /*.p_mht              = */uecc_fsm_src_ho_get_ho_prep_info_ongoing_mht
/*SPR_19066_END*/
};
/* coverity_88493 start */
/* Fail actions */
/* SRC HO failure actions lists */
/* Re Establishment from UE at state UECC_SRC_HO_GET_MAC_I */
/* receive CSC delete all ue from CSC */
/* receive reset request from S1AP */
/* Received UE Context delete command from MME */
/* Received Radio Link failure from MAC */
/* work out for any state except if reestab has been sent then send 
   reestabrej request */

/* coverity_88493 stop */
static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type2_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind,
    uecc_fsm_src_ho_fail_action_last, 
    PNULL
};


static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type3_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type4_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_data_buffer_stop_ind,
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type7_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event1,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type8_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_reestab_complete_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event2,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type9_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event2,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type10_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_reestab_complete_ind,
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event2,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type11_actions[] =
{
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event3,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type12_actions[] =
{
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event4,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type13_actions[] =
{
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event5,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type14_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_resp,
    uecc_fsm_src_ho_fail_action_last, 
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type15_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_resp,
    uecc_fsm_src_ho_fail_action_last, 
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type16_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_data_buffer_stop_ind, 
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_resp,
    uecc_fsm_src_ho_fail_action_last, 
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type17_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind,
    uecc_fsm_src_ho_fail_action_last1, 
    uecc_fsm_src_ho_fail_action_post_event6,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type18_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind,
    uecc_fsm_src_ho_fail_action_last1, 
    uecc_fsm_src_ho_fail_action_post_event6,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type19_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_reestab_complete_ind,
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind,
    uecc_fsm_src_ho_fail_action_last1, 
    uecc_fsm_src_ho_fail_action_post_event6,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type20_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_s1ap_context_release_cmd,
/* SPR 15666 Fix Start */
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind,
/* SPR 15666 Fix Stop */
    uecc_fsm_src_ho_fail_action_last1, 
    uecc_fsm_src_ho_fail_action_post_event1,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type21_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_data_buffer_stop_ind,
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event1,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type23_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_data_buffer_stop_ind,
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_resp,
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event1,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type24_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_data_buffer_stop_ind,
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_send_s1ap_context_release_cmd,
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event1,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type25_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_error_indication,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type26_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_failure_resp,
    uecc_fsm_src_ho_fail_action_last, 
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type27_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_failure_resp,
    uecc_fsm_src_ho_fail_action_last, 
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type28_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_data_buffer_stop_ind, 
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_failure_resp,
    uecc_fsm_src_ho_fail_action_last, 
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type29_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event3,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type30_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event5,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type31_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event4,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type34_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_data_buffer_stop_ind, 
    uecc_fsm_src_ho_fail_action_last, 
    PNULL
};

/* coverity_fix_61159_start */
/*
static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type32_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_enb_ue_ctx_release_req,
    uecc_fsm_ue_src_ho_fail_action_send_rrm_ho_adm_cnf_failure,
    uecc_fsm_src_ho_fail_action_last,
    PNULL
};
*/
/* coverity_fix_61159_stop */

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type33_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_enb_ue_ctx_release_req,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind,
    uecc_fsm_src_ho_fail_action_last, 
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type35_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event2,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type36_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last1,
    uecc_fsm_src_ho_fail_action_post_event2,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type37_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_reestab_complete_ind,
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_failure_resp,
    uecc_fsm_src_ho_fail_action_last, 
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type38_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_reestab_complete_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind,
    uecc_fsm_src_ho_fail_action_last1, 
    uecc_fsm_src_ho_fail_action_post_event6,
    PNULL
};


static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type39_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_reestab_complete_ind,
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind,
    uecc_fsm_src_ho_fail_action_last1, 
    uecc_fsm_src_ho_fail_action_post_event6,
    PNULL
};


static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type40_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_reestab_complete_ind,
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind, 
    uecc_fsm_src_ho_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type41_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_reestab_complete_ind,
    uecc_fsm_src_ho_fail_action_send_ho_cancel_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_resp,
    uecc_fsm_src_ho_fail_action_last, 
    PNULL
};

/*securitykey*/
/* following fail actions are applicable for scenario where UE Context
 * Modification was ongoing and it has trigerred SRC HO FSM for Security
 * Rekeying using Intra-Cell HO Procedure and some failure condition happened
 * during Intra-Cell HO signalling procedure */
static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type42_actions[] =
{
    uecc_fsm_ue_ctx_mod_fail_action_send_rrm_cnf,
    uecc_fsm_ue_ctx_mod_fail_action_send_fail_resp_to_s1ap,
    uecc_fsm_src_ho_fail_action_send_enb_ue_ctx_release_req,
    uecc_fsm_src_ho_cleanup_ue_ctx_mod_fsm_data,
    uecc_fsm_src_ho_fail_action_last, 
    PNULL
};
/*securitykey*/
/*SPR_17606_START*/
static const uecc_fsm_fail_handler_f uecc_fsm_src_ho_fail_type43_actions[] =
{
    uecc_fsm_src_ho_fail_action_send_llim_reestab_complete_ind,
    uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind,
    uecc_fsm_src_ho_fail_action_last, 
    PNULL
};
/*SPR_17606_END*/


/* SPR 16750 Fix Start */
/* Code Removed */
/* SPR 16750 Fix Stop */

/****************************************************************************
 * Private Functions Implementation
 ****************************************************************************/
/*securitykey*/
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_ue_ctx_mod_pending_buffer_new_msg 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 *                  U16     - len,
 *                  uecc_msg_bfr_type_et -  msg_bfr_type
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the high priority messages (HO, RLF,
 *                  Re-Establishment when Intra Cell HO is ongoing due to
 *                  Security Rekeying triggered from UE Context Modification FSM
 ****************************************************************************/
static void uecc_fsm_src_ho_ue_ctx_mod_pending_buffer_new_msg(
        uecc_ue_context_t*     p_ue_context,
        void*                  p_api,
        U16                    len,
        uecc_msg_bfr_type_et   msg_bfr_type
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* Buffered Received Re-establishment Msg */
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
                rrc_mem_get(len)))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL,
                "Memory Allocation Failed to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* Storing re_establishment contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
            p_api,len);

    p_ue_context->p_curr_proc_data->msg_bfr_type = msg_bfr_type;

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_UE_CTX_MOD_ONGOING_HO_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}    
/*securitykey*/


/****************************************************************************
 * Function Name  : uecc_fsm_source_handover_procedure_constructor
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Constructor for UE Handover procedure at source eNB
 ****************************************************************************/
static 
void uecc_fsm_source_handover_procedure_constructor(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    /* SPR 24021/SES-452 Coverity 209473 Fix Start */
    rrc_rrm_ho_required_t *p_ho_req = PNULL;
    /* SPR 24021/SES-452 Coverity 209473 Fix End */
    /*SPR 21653 changes start*/
    U8 index = RRC_NULL;
    U8 rrc_cells_avail = RRC_NULL;
/*SPR 21653 changes end*/

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* SPR 24021/SES-452 Coverity 209473 Fix Start */
    if(PNULL == p_api)
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
            "[RRC_RRM_HO_REQUIRED] p_api is NULL,"
            " Returning from source_handover_procedure_constructor\n");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    p_ho_req = (rrc_rrm_ho_required_t*)p_api;
    /* SPR 24021/SES-452 Coverity 209473 Fix End */

    /* set ho failure cause to default in p_ue_context */
    p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;


    do
    {
        if(HANDOVER_TYPE_INTER_CELL == p_ho_req->ho_type)
        {
/*SPR 21653 changes start*/
            if(PNULL != p_ue_context->p_gb_context->p_p_csc_context)
            {
                for(index = 0;index < MAX_NUM_CELLS;index++) 
                {
                    if (PNULL != p_ue_context->p_gb_context->p_p_csc_context[index])
                    {
                        ++rrc_cells_avail;
                    }
                }
             }
            if(rrc_cells_avail <= 1)
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "INTER_CELL_HO is not possible when only one cell is present,so sending failure");
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type2_actions);
                break;
            }
/*SPR 21653 changes end*/
        }

        /*SPR_18241_START*/
        if ((INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) &&
                (RRC_TRUE == p_ue_context->ue_ctx_mod_security_flag) &&
                (PNULL == p_ue_context->p_curr_proc_data) &&
                (PNULL != p_ue_context->p_temp_ue_ctx_mod_proc_data))
        {
            p_ue_context->p_curr_proc_data = p_ue_context->p_temp_ue_ctx_mod_proc_data;
            p_ue_context->p_temp_ue_ctx_mod_proc_data = PNULL;
        }
        /*SPR_18241_END*/
            /*BUG 604 changes start*/
        if ( (HANDOVER_TYPE_INTER_CELL == p_ho_req->ho_type) ||
               (HANDOVER_TYPE_INTRA_LTE_X2 ==  p_ho_req->ho_type) ||
               (HANDOVER_TYPE_INTRA_CELL == p_ho_req->ho_type))
            /*BUG 604 changes stop*/
        {
            if (RRC_FAILURE == validate_ho_cause( &p_ho_req->cause,
                         p_ue_context->p_gb_context))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "[RRC_RRM_HO_REQUIRED] Invalid Cause Received.");
                uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type2_actions);
                break;
            }
	    /*SPR_16618_START*/
	    p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present = RRC_FALSE;
	    /*SPR_16618_END*/
        /*SPR_17151_START*/
        p_ue_context->x2ap_context.old_enb_ue_x2ap_id = INVALID_OLD_ENB_UE_X2AP_ID;
        /*SPR_17151_END*/
	}
        /*BUG 604 changes start*/
        if(HANDOVER_TYPE_CCO != p_ho_req->ho_type)
        /*BUG 604 changes stop*/
        {
            /* validate the HO REQUIRED MSG */
            /* checking for the validity of handover procedure according to present configuration */
            if ( RRC_TRUE != uecc_fsm_valiating_erab_config_for_ho_procedure (p_ue_context) )
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "No NON GBR ERAB present");
                /* Process failure according to selected failure actions list */
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type2_actions);
                break;
            }
        }
        if ( RRC_TRUE != uecc_fsm_valiating_rrm_ho_required_msg (p_ue_context, p_api) )
        {
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type2_actions);
            break;
        }
        /* check if prep timer is running */
        /*check if prep timer is running for X2 based handover*/
        /*BUG 604 changes start*/
        if((X2_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
        /*BUG 604 changes stop*/
        {
          if ( PNULL == p_ue_context->timers_data .timers_ids[UECC_UE_X2_HO_PREP_TIMER] )
          {
            /* Start timer*/
            if ( RRC_SUCCESS != uecc_ue_timer_start(
                  p_ue_context->p_gb_context, 
                  UECC_UE_X2_HO_PREP_TIMER,
                  &p_ue_context->p_gb_context->ue_timers_data,
                  &p_ue_context->timers_data,
                  p_ue_context->ue_index))
            {
              /* Process failure according to selected failure actions list */
              uecc_fsm_start_failure_processing(
                  p_ue_context,
                  uecc_fsm_src_ho_fail_type2_actions);
              break;

            }
          }
        }    
        else if(S1_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
            /*check if prep timer is running for S1 based handover*/
            if ( PNULL == p_ue_context->timers_data .timers_ids[UECC_UE_HO_PREP_TIMER] )
            {
                /* Start timer*/
                if ( RRC_SUCCESS != uecc_ue_timer_start(
                            p_ue_context->p_gb_context,
                            UECC_UE_HO_PREP_TIMER,
                            &p_ue_context->p_gb_context->ue_timers_data,
                            &p_ue_context->timers_data,
                            p_ue_context->ue_index))
                {
                    /* Process failure according to selected failure actions list */
                    uecc_fsm_start_failure_processing(
                            p_ue_context,
                            uecc_fsm_src_ho_fail_type2_actions);
                    break;

                }
            }

        }
            /*BUG 604 changes start*/
            /*BUG 604 changes stop*/
        else if(CCO_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
            /*check if prep timer is running for intra_cell based handover*/
            if ( PNULL == p_ue_context->timers_data .timers_ids[UECC_UE_CCO_GUARD_TIMER] )
            {
                /* Start timer*/
                if ( RRC_SUCCESS != uecc_ue_timer_start(
                            p_ue_context->p_gb_context,
                            UECC_UE_CCO_GUARD_TIMER,
                            &p_ue_context->p_gb_context->ue_timers_data,
                            &p_ue_context->timers_data,
                            p_ue_context->ue_index))
                {
                    /* Process failure according to selected failure actions list */
                    uecc_fsm_start_failure_processing(
                            p_ue_context,
                            uecc_fsm_src_ho_fail_type2_actions);
                    break;
                }
                p_ue_context->cs_fallback_indicator = p_ho_req->cco_params.cs_fallback_indicator;
            }
        }

        /* allocating memory */
        p_ue_context->ho_info.p_ho_info 
            = (uecc_ho_info_t*)rrc_mem_get(sizeof(uecc_ho_info_t));

        /* check if memory allocation failure */
        if( PNULL == p_ue_context->ho_info.p_ho_info )
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL, "" 
                    "Memory allocation failure");
            break;

        }    

        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED, "ho_info initialized.");

        p_ue_context->ho_info.p_ho_info->p_src_ho_info 
            = (uecc_src_ho_info_t*)rrc_mem_get(sizeof(uecc_src_ho_info_t));

        if ( PNULL == p_ue_context->ho_info.p_ho_info->p_src_ho_info)
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL, "" 
                    "Memory allocation failure");
            break;
        }
        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED, "src_ho_info initialized.");
        p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info
            .p_rrc_rrm_ho_required 
            = (rrc_rrm_ho_required_t*)rrc_mem_get(sizeof(rrc_rrm_ho_required_t));
        if( PNULL == p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                s1_src_ho_info.p_rrc_rrm_ho_required )
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL, "" 
                    "Memory allocation failure");
            break;
        }
        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED, 
                "rrc_rrm_ho_required initialized.");

        /* storing the ho_required message received from RRM */
        l3_memcpy_wrapper(p_ue_context->ho_info.p_ho_info->p_src_ho_info
                ->s1_src_ho_info.p_rrc_rrm_ho_required,
                ((rrc_rrm_ho_required_t*)p_api) ,sizeof(rrc_rrm_ho_required_t));

        /* Popluating X2AP DRB Context for UE */
        /*BUG 604 changes start*/
        if((p_ue_context->ho_info.s1_or_x2_handover == X2_HO) ||
            (p_ue_context->ho_info.s1_or_x2_handover == INTER_CELL_HO) ||
            (p_ue_context->ho_info.s1_or_x2_handover == INTRA_CELL_HO))
        /*BUG 604 changes stop*/
        {
             uecc_populate_drb_context(p_ue_context);
        }

        /*BUG 604 changes start*/
        /* Init ASN1 context */
        if ( RT_OK != rtInitContext(&p_ue_context->ho_info.p_ho_info->ho_s1ap_asn1_ctx))
        {
             /* Drop message */
             RRC_TRACE(RRC_ERROR,
                 "rrc_s1ap_handover_command_container_intrl_dec:"
                 "ASN context initialization failed.");
             RRC_UT_TRACE_EXIT();
             break;
        }
        /*BUG 604 changes stop*/
        /* Store transaction id and use it for Release operation */
#ifdef RRC_UECC_TR_ID_CONST
        uecc_ue_ctx_set_op_transction_id(p_ue_context, 1); 
#else
        uecc_ue_ctx_set_op_transction_id(p_ue_context,
                uecc_ue_ctx_get_next_transction_id(p_ue_context));
#endif /*RRC_UECC_TR_ID_CONST*/
        
    }while(0);
    /* SPR 24021/SES-452 Fix Start */
    if (PNULL != p_api)
    {
	    rrc_mem_free(p_api);
        p_api = PNULL;
    }
    /* SPR 24021/SES-452 Fix End */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return;
}

/****************************************************************************
 * Function Name  : uecc_fsm_source_handover_procedure_finish_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Success handler for UE Handover procedure at source eNB
 *                  Calls on procedure's finishing
 ****************************************************************************/
static void uecc_fsm_source_handover_procedure_finish_success(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*x2_ho_timer*/
    /* stop timer */
    /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
    uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);*/

    p_ue_context->x2ap_reset_recvd_flag = RRC_FALSE;

    /* Send event to UE Main FSM */
    /* For X2AP or INTER_CELL Handover*/
/*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
            (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
	    /*SPR_16618_START*/
	    p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present = RRC_FALSE;
	    /*SPR_16618_END*/
	    /*x2_ho_timer*/
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        /*BUG 604 changes start*/
        if ((INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) &&
            (RRC_TRUE == p_ue_context->ue_ctx_mod_security_flag))
        {
            p_ue_context->ue_ctx_mod_security_flag = RRC_FALSE;

            /*Send UE Context Modification Response to MME with SUCCESS*/
            if (RRC_SUCCESS !=
                    uecc_s1ap_build_and_send_ue_context_modification_response(p_ue_context))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                        "[s1ap_UEContextModificationResponse]Delivery failed");
            }

            /* FREE p_curr_proc_data */
            if (PNULL != p_ue_context->p_curr_proc_data)
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
        /*BUG 604 changes stop*/

        SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_ue_context);        
        uecc_event_queue_push_event(
                UECC_EV_SRC_X2AP_HO_CTX_REL_INT,
                p_ue_context,
                PNULL);

    }
    else if(S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover) 
    {
        /*x2_ho_timer*/
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        /* CR 487 changes start*/
        if ((HANDOVER_TYPE_LTE_TO_CDMA2000_1XRTT == p_ue_context->ho_info.p_ho_info->
                    p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) &&
                (RRC_RRM_HO_REQUIRED_HO_FROM_EUTRA_PREPERATION_REQ_PRESENT & 
                 p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                 p_rrc_rrm_ho_required->cdma2000_information.bitmask) &&
                (RRC_RRM_HO_FROM_EUTRA_DUAL_RX_TX_REDIRECT_IND_PRESENT &
                 p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                 p_rrc_rrm_ho_required->cdma2000_information.ho_from_eutra_preperation_req_r10.
                 bitmask) &&
                (RRC_RRM_HO_FROM_EUTRA_REDIRECT_CARRIER_CDMA2000_1XRTT_PRESENT &
                 p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                 p_rrc_rrm_ho_required->cdma2000_information.ho_from_eutra_preperation_req_r10.
                 bitmask))
        {
            uecc_event_queue_push_event(
                    UECC_EV_SRC_HO_SUCCESS_INT,
                    p_ue_context,
                    PNULL);
        }
        else
        {/* CR 487 changes stop*/
            uecc_event_queue_push_event(
                    UECC_EV_SRC_HO_CTX_REL_CMD_INT,
                    p_ue_context,
                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd);
        }
    }
    else if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_event_queue_push_event(
                UECC_EV_SRC_HO_REL_REQ_INT,
                p_ue_context,
                PNULL);
    }
    else 
    {
        uecc_event_queue_push_event(
                UECC_EV_SRC_HO_SUCCESS_INT,
                p_ue_context,
                PNULL);
    }

    /* releasing ho_info in Ue context */
    uecc_fsm_src_ho_release_ho_info(p_ue_context);

    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_release_ho_info 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Release the memory allocated within the ho_info of ue 
 *                  context
 ****************************************************************************/
/*SPR_17348_START*/
void uecc_fsm_src_ho_release_ho_info(
/*SPR_17348_END*/
        uecc_ue_context_t*  p_ue_context
        )
{
    uecc_ho_src_drb_forward_list_t *p_src_drb_fwd_list = PNULL; 
    uecc_ho_src_drb_admitted_list_t *p_src_drb_admitted_list = PNULL; 
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if ( PNULL != p_ue_context->ho_info.p_ho_info )
    {
        rtFreeContext(&p_ue_context->
                ho_info.p_ho_info->ho_s1ap_asn1_ctx);
        if ( PNULL != p_ue_context->ho_info.p_ho_info->p_src_ho_info )
        {
            if ( PNULL !=  p_ue_context->ho_info.p_ho_info
                    ->p_src_ho_info->p_re_establishment_req )
            {
                /* assigning PNULL to p_re_establishment_req without release of memory
                   because main FSM may use this memory reference */
                p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    p_re_establishment_req = PNULL;
            }
            if ( PNULL !=  p_ue_context->ho_info.p_ho_info
                    ->p_src_ho_info->p_radio_link_failure_ind)
            {
                /* assigning PNULL to p_radio_link_failure_ind without release of memory
                   because main FSM may use this memory reference */
                p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    p_radio_link_failure_ind = PNULL;
            }
            if ( PNULL !=  p_ue_context->ho_info.p_ho_info
                    ->p_src_ho_info->p_ue_ctx_rel_cmd)
            {
                /* assigning PNULL to p_ho_ue_ctx_rel_cmd without release of memory
                   because main FSM may use this memory reference */
                p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    p_ue_ctx_rel_cmd = PNULL;
            }

            if (PNULL != p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    s1_src_ho_info.p_rrc_rrm_ho_required)
            {
                rrc_mem_free( p_ue_context->ho_info.p_ho_info->
                        p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required );
                p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    s1_src_ho_info.p_rrc_rrm_ho_required = PNULL ;
            }
            if (PNULL != p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    s1_src_ho_info.p_ho_command )
            {
                rrc_mem_free(p_ue_context->ho_info.p_ho_info->
                        p_src_ho_info->s1_src_ho_info.p_ho_command );
                p_ue_context->ho_info.p_ho_info->
                    p_src_ho_info->s1_src_ho_info.p_ho_command = PNULL;
            }
            /*SPR 21772 Changes Start*/
            if(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    s1_src_ho_info.inter_rat_ho_info.p_nas_security_param_from_E_UTRAN_data != PNULL)
            {
                rrc_mem_free(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                        s1_src_ho_info.inter_rat_ho_info.p_nas_security_param_from_E_UTRAN_data);
                p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    s1_src_ho_info.inter_rat_ho_info.p_nas_security_param_from_E_UTRAN_data = PNULL;                 
            }
            if(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    s1_src_ho_info.inter_rat_ho_info.p_target_to_source_transparent_container_secondary_data != PNULL)
            {
                rrc_mem_free(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                        s1_src_ho_info.inter_rat_ho_info.p_target_to_source_transparent_container_secondary_data);
                p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    s1_src_ho_info.inter_rat_ho_info.p_target_to_source_transparent_container_secondary_data = PNULL;
            }
            if(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    s1_src_ho_info.inter_rat_ho_info.p_target_to_source_transparent_container_data != PNULL)
            {
                rrc_mem_free(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                        s1_src_ho_info.inter_rat_ho_info.p_target_to_source_transparent_container_data);
                p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    s1_src_ho_info.inter_rat_ho_info.p_target_to_source_transparent_container_data = PNULL;
            }

            /*SPR 21772 Changes Stop*/


            /* memory releasing from erab_ho_src_drb_forward_list */
            if(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    s1_src_ho_info.bitmask &
                    ERAB_SUBJECT_TO_DATA_FORWARDING_LIST_PRESENT )
            {
                p_src_drb_fwd_list = &p_ue_context->ho_info.p_ho_info->
                                     p_src_ho_info->s1_src_ho_info.
                                     src_drb_fwd_list;
                {
                    rrc_counter_t erab_count = 0;
                    for ( erab_count = 0; erab_count < MAX_ERAB_COUNT;
                            erab_count++ )
                    {
                        if(PNULL != p_src_drb_fwd_list->
                                erab_ho_src_drb_forward_list[erab_count])
                        {
                            rrc_mem_free(p_src_drb_fwd_list->
                                    erab_ho_src_drb_forward_list[erab_count]);
                            p_src_drb_fwd_list->erab_ho_src_drb_forward_list
                                [erab_count] = PNULL;
                        }
                    }
                }
            }
            /*x2ap-start*/
            if(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                s1_src_ho_info.bitmask &
                X2AP_ERAB_ADMITTED_LIST_PRESENT )
            {
              p_src_drb_admitted_list = &p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.
                src_erab_admitted_list;
              {
                rrc_counter_t erab_count = 0;
                for ( erab_count = 0; erab_count < MAX_ERAB_COUNT;
                    erab_count++ )
                {
                  if(PNULL != p_src_drb_admitted_list->
                      erab_ho_src_drb_admitted_info[erab_count])
                  {
                    rrc_mem_free(p_src_drb_admitted_list->
                        erab_ho_src_drb_admitted_info[erab_count]);
                    p_src_drb_admitted_list->erab_ho_src_drb_admitted_info
                      [erab_count] = PNULL;
                  }
                }
              }
            }
            /*x2ap-end*/ 
            if (PNULL != p_ue_context->ho_info.p_ho_info->p_src_ho_info)
            {
                rrc_mem_free( p_ue_context->ho_info.p_ho_info->p_src_ho_info );
                p_ue_context->ho_info.p_ho_info->p_src_ho_info = PNULL ;
            }
        }

        if (PNULL != p_ue_context->ho_info.p_ho_info)
        {
        rrc_mem_free( p_ue_context->ho_info.p_ho_info );
        p_ue_context->ho_info.p_ho_info = PNULL;
    }
    }

    /* For X2AP or INTER_CELL Handover*/
/*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
            (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        /*SPR_16604_START*/
        p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present = 0;
        /*SPR_16604_END*/

        /* SPR 16750 Fix Start */
        p_ue_context->x2ap_context.m.error_indication_ongoing = 0;
        /* SPR 16750 Fix Stop */

        /*Release old X2AP_UEID */
        /*SPR_17151_START*/
        if(INVALID_OLD_ENB_UE_X2AP_ID != p_ue_context->x2ap_context.old_enb_ue_x2ap_id)
        {
            uecc_x2ap_id_free(p_ue_context->p_gb_context,
                    p_ue_context->x2ap_context.old_enb_ue_x2ap_id);
        }
        /*SPR_17151_END*/
    }

    /* X2AP FIX_RW_3 : Start*/ 
/*BUG 604 changes start*/
    if (INTRA_CELL_HO != p_ue_context->ho_info.s1_or_x2_handover)
    {
        /* Reset the HO Flag in UE Context */	
        p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
    }
/*BUG 604 changes stop*/
    /* X2AP FIX_RW_3 : End*/
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_source_handover_procedure_finish_fail
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Failure handler for UE handover at source eNB
 *                  Calls on procedure's finishing
 ****************************************************************************/
static void uecc_fsm_source_handover_procedure_finish_fail(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
	/*BUG 604 changes start*/
	s1ap_Cause cause;
	/*BUG 604 changes stop*/
/*SPR 20636 Changes Start*/
	uecc_fsm_event_et failure_ev = (uecc_fsm_event_et)((U32_64_Bit)p_api);
/*SPR 20636 Changes End*/
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	/*x2_ho_timer :start*/
	/* stop timer */
	/*check if timer is running for X2 based handover*/
	/* For X2AP or INTER_CELL Handover*/
	/*BUG 604 changes start*/
	if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
			(INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
			(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
		/*BUG 604 changes stop*/
	{
                /*SPR_16618_START*/
                p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present = RRC_FALSE;
                /*SPR_16618_END*/
		uecc_ue_timer_stop(p_ue_context->p_gb_context, 
				UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
		uecc_ue_timer_stop(p_ue_context->p_gb_context, 
				UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);
	}
	else if(S1_HO == p_ue_context->ho_info.s1_or_x2_handover) 
	{
		uecc_ue_timer_stop(p_ue_context->p_gb_context, 
				UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
		uecc_ue_timer_stop(p_ue_context->p_gb_context, 
				UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);
	}
	/*BUG 604 changes start*/
	/*else if(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)
	  {
	  uecc_ue_timer_stop(p_ue_context->p_gb_context,
	  UECC_UE_INTRA_CELL_HO_GUARD_TIMER, &p_ue_context->timers_data);
	  }*/
	/*BUG 604 changes stop*/
	/*x2_ho_timer :end*/
	else if(CCO_HO == p_ue_context->ho_info.s1_or_x2_handover)
	{
		uecc_ue_timer_stop(p_ue_context->p_gb_context,
				UECC_UE_CCO_GUARD_TIMER, &p_ue_context->timers_data);
	}

	p_ue_context->x2ap_reset_recvd_flag = RRC_FALSE;

	/*BUG 604 changes start*/
	/*securitykey*/
	if ((INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) &&
			(RRC_TRUE == p_ue_context->ue_ctx_mod_security_flag))
	{
		/*setting ho_sucess_flag to RRC_FALSE, so that it can be handled
		 * accordingly when control returns back to UE_CTX_MOD FSM*/
		p_ue_context->ue_ctx_mod_security_flag = RRC_FALSE;

		cause.t = T_s1ap_Cause_radioNetwork ;

		if ((UECC_EV_SRC_HO_RE_EST_INT == failure_ev) || 
				(UECC_EV_SRC_HO_CTX_REL_CMD_INT == failure_ev))
		{
			cause.u.radioNetwork = s1ap_interaction_with_other_procedure;
		}
		else if (UECC_EV_SRC_HO_RADIO_LINK_FAILURE_INT == failure_ev)
		{
			cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost;
		}
		else
		{
			cause.u.radioNetwork = s1ap_unspecified_2;
		}

		/* Coverity Fix 64233 Start */
		if((PNULL != p_ue_context->p_curr_proc_data) &&
				(p_ue_context->p_curr_proc_data->m.SecurityCapability_present))
		{
			/* Coverity Fix 64233 Stop */
			p_ue_context->configure_ciphering_user_plane.algorithm_id = 
				p_ue_context->p_curr_proc_data->userplane_ciphering_algo_id;
			p_ue_context->configure_ciphering.algorithm_id =
				p_ue_context->p_curr_proc_data->ciphering_algo_id;
			p_ue_context->configure_integrity_protection.algorithm_id = 
				p_ue_context->p_curr_proc_data->integrity_algo_id;
		}

		if (RRC_SUCCESS !=
				uecc_s1ap_build_and_send_ue_context_modification_failure(
					p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
		{
			RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
					"[s1ap_UEContextModificationFailure]Delivery failed");
		}

		/* FREE p_curr_proc_data */
		if (PNULL != p_ue_context->p_curr_proc_data)
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
	/*securitykey*/
	/*BUG 604 changes stop*/

	/* Send event to UE Main FSM */
	if( UECC_EV_SRC_HO_RE_EST_INT== failure_ev )
	{
		uecc_event_queue_push_event(
				failure_ev,
				p_ue_context,
				p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_re_establishment_req);

	}
	else if(UECC_EV_SRC_HO_RADIO_LINK_FAILURE_INT == failure_ev)
	{
		uecc_event_queue_push_event(
				failure_ev,
				p_ue_context,
				p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_radio_link_failure_ind);
	}
	else if(UECC_EV_SRC_HO_CTX_REL_CMD_INT == failure_ev )
	{
		uecc_event_queue_push_event(
				failure_ev,
				p_ue_context,
				p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd);
	}
	else
	{
		/*BUG 604 changes start*/
		if ((CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)&&
				(UECC_EV_SRC_HO_FAILURE_INT == failure_ev))
			/*BUG 604 changes start*/
		{
			uecc_event_queue_push_event(
					UECC_EV_SRC_HO_REL_REQ_INT,
					p_ue_context,
					PNULL);
		}
		else
		{
			uecc_event_queue_push_event(
					failure_ev,
					p_ue_context,
					PNULL);
		}
	}
	if (PNULL != g_rrc_stats.p_rrc_stats)
	{
		RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
				g_rrc_stats.p_rrc_stats->
				rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
				p_gb_context->offset_uecc_mod_id].
				rrc_uecc_cell_stats[p_ue_context->cell_index].
				rrc_handover_related_stats.
				ho_failure_num, 0, DUMMY_RANGE_CHECK_INDEX);
	}

	/* releasing ho_info in Ue context */
	uecc_fsm_src_ho_release_ho_info(p_ue_context);

	/* Reset the HO Flag in UE Context */	
	p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;


	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_re_establishment_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/
static 
/*SPR_19066_START*/
void uecc_fsm_src_ho_llim_get_ho_prep_info_ongoing_re_establishment_handler(
/*SPR_19066_END*/
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    U16     len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* checking the current child fsm state */
    /* filling the cause in p_ue_context */
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        /*x2_ho_timer*/
        /* stop preparation timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                   UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);

        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
    }
    else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        /* stop preparation timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
               UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);

        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost; /* SPR 2591 Start */
    }
    /*BUG 604 changes start*/
    /*else if (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                 UECC_UE_INTRA_CELL_HO_GUARD_TIMER, &p_ue_context->timers_data);
    }*/
    /*BUG 604 changes stop*/
    
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "RE_EST MSG Received during SRC_HO");

    len = (U16)(sizeof(RRCConnectionReestablishmentRequest_r8_IEs));
    if(!p_ue_context->ue_ctx_mod_security_flag)
    {
        if (PNULL == (p_ue_context->ho_info.p_ho_info->
                    p_src_ho_info->p_re_establishment_req =
                    rrc_mem_get(len)))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL,
                    "Memory Allocation Failed to store Reestablishment Req msg");
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return;
        }

        /* storing re_establishment contents into p_re_establishment_req */
        l3_memcpy_wrapper(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                p_re_establishment_req,
                p_api,
                len);

        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type35_actions);

    }
    /*securitykey*/
    else
    {
        uecc_fsm_src_ho_ue_ctx_mod_pending_buffer_new_msg(p_ue_context, p_api,
                                                   len, RRC_RE_ESTABLISH_MSG);
    }
    /*securitykey*/
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_ho_prep_re_establishment_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static 
void uecc_fsm_src_ho_llim_ho_prep_re_establishment_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{

    U16     len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* checking the current child fsm state */

    /*x2_ho_timer*/
    /* stop preparation timer */
    /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);*/

    /* filling the cause in p_ue_context */
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        /* stop preparation timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
        /*Fill cause*/
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
    }
    else
    {
        /* stop preparation timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
        /*Fill cause*/
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost; /* SPR 2591 Start */
    }

    len = (U16)(sizeof(RRCConnectionReestablishmentRequest_r8_IEs));
    if (PNULL == (p_ue_context->ho_info.p_ho_info->
                  p_src_ho_info->p_re_establishment_req =
                  rrc_mem_get(len)))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL,
                "Memory Allocation Failed to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* storing re_establishment contents into p_re_establishment_req */
    l3_memcpy_wrapper(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
               p_re_establishment_req,
               p_api,
               len);


    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type9_actions);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_ho_cmd_enb_status_transfer_re_establishment_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static 
void uecc_fsm_src_ho_llim_ho_cmd_enb_status_transfer_re_establishment_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{

    U16     len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* checking the current child fsm state */

    /*x2_ho_timer*/
    /* stop overall timer */
    /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);*/

    /* filling the cause in p_ue_context */
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        /*x2_ho_timer*/
        /* stop overall timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
    }
    else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        /*x2_ho_timer*/
        /* stop overall timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost; /* SPR 2591 Start */
    }
    /*BUG 604 changes start*/
    /*else if (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_INTRA_CELL_HO_GUARD_TIMER, &p_ue_context->timers_data);
    }*/
    /*BUG 604 changes stop*/
    else if (CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_CCO_GUARD_TIMER, &p_ue_context->timers_data);
    }


    len = (U16)(sizeof(RRCConnectionReestablishmentRequest_r8_IEs));

        if (PNULL == (p_ue_context->ho_info.p_ho_info->
                    p_src_ho_info->p_re_establishment_req =
                    rrc_mem_get(len)))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL,
                    "Memory Allocation Failed to store Reestablishment Req msg");
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return;
        }

        /* storing re_establishment contents into p_re_establishment_req */
        l3_memcpy_wrapper(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                p_re_establishment_req,
                p_api,
                len);

    /*BUG 604 changes start*/
        if (CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    /*BUG 604 changes stop*/
        {
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type8_actions);
        }
        else
        {
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type10_actions);
        }
 
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_s1u_reconfig_for_forwarding_re_establishment_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static 
void uecc_fsm_src_ho_llim_s1u_reconfig_for_forwarding_re_establishment_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{

    U16     len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* checking the current child fsm state */

    /*x2_ho_timer*/
    /* stop overall timer */
    /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);*/

    /* need to define RE_EST_TRIGGERED */

    /* filling the cause in p_ue_context */
/*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        /*x2_ho_timer*/
        /* stop overall timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
    }
    else
    {
        /*x2_ho_timer*/
        /* stop overall timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost; /* SPR 2591 Start */
    }

    len = (U16)(sizeof(RRCConnectionReestablishmentRequest_r8_IEs));
    if (PNULL == (p_ue_context->ho_info.p_ho_info->
                  p_src_ho_info->p_re_establishment_req =
                  rrc_mem_get(len)))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL,
                "Memory Allocation Failed to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* storing re_establishment contents into p_re_establishment_req */
    l3_memcpy_wrapper(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
               p_re_establishment_req,
               p_api,
               len);


    /* state changed to UECC_SRC_HO_S1U_DELETE_FORWARDING_TUNNEL_ONGOING */
    UECC_FSM_SET_CHILD_STATE(p_ue_context,
            &uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing, PNULL);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_wait_for_srb_data_status_ctx_release_cmd_re_establishment_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/


static 
void uecc_fsm_src_ho_llim_wait_for_srb_data_status_ctx_release_cmd_re_establishment_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
	/* coverity_fix_62462_start */
	U16     len = RRC_NULL;


	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


	/*x2_ho_timer*/
	/* stop preparation timer */
	/*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);*/

	/* need to define RE_EST_TRIGGERED */

	/* filling the cause in p_ue_context */
	/*BUG 604 changes start*/
	if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
			(INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
			(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
		/*BUG 604 changes stop*/
	{
		/*x2_ho_timer*/
		/* stop preparation timer */
		uecc_ue_timer_stop(p_ue_context->p_gb_context, 
				UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);

		p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
		p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
	}
	else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
	{
		/*x2_ho_timer*/
		/* stop preparation timer */
		uecc_ue_timer_stop(p_ue_context->p_gb_context, 
				UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);

		p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
		p_ue_context->ho_info.cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost; /* SPR 2591 Start */
	}

	/*BUG 604 changes start*/
	len = (U16)(sizeof(RRCConnectionReestablishmentRequest_r8_IEs));
	if (PNULL == (p_ue_context->ho_info.p_ho_info->
				p_src_ho_info->p_re_establishment_req =
				rrc_mem_get(len)))
	{
		RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL,
				"Memory Allocation Failed to store Reestablishment Req msg");
		RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
		return;
	}

	/* storing re_establishment contents into p_re_establishment_req */
	l3_memcpy_wrapper(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
			p_re_establishment_req,
			p_api,
			len);


	if ( IS_S1U_RECONFIGURED(p_ue_context))
	{
		/* state changed to UECC_SRC_HO_S1U_DELETE_FORWARDING_TUNNEL_ONGOING */
		UECC_FSM_SET_CHILD_STATE(p_ue_context,
				&uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing, PNULL);
	}
	else
	{
		if (CCO_HO == p_ue_context->ho_info.s1_or_x2_handover)
		{
			/* Process failure according to selected failure actions list */
			uecc_ue_timer_stop(p_ue_context->p_gb_context, 
					UECC_UE_CCO_GUARD_TIMER, &p_ue_context->timers_data);
			uecc_fsm_start_failure_processing(
					p_ue_context,
					uecc_fsm_src_ho_fail_type8_actions);
		}
		else
		{
				/* Process failure according to selected failure actions list */
				uecc_fsm_start_failure_processing(
						p_ue_context,
						uecc_fsm_src_ho_fail_type10_actions);
			/*BUG 604 changes stop*/
		}
	}
	/*BUG 604 changes start*/
	//}
	/*BUG 604 changes stop*/
	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}
/* coverity_fix_62462_stop */


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_re_
 *                  establishment_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/
static void 
uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_re_establishment_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
)
{
    U16     len = RRC_NULL;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* checking the current child fsm state */

    /* stop preparation timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_LL_GUARD_TIMER, &p_ue_context->timers_data);

    /* need to define RE_EST_TRIGGERED */

    /* filling the cause in p_ue_context */
/*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
    }
    else
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost; /* SPR 2591 Start */
    }

    len = (U16)(sizeof(RRCConnectionReestablishmentRequest_r8_IEs));
    if (PNULL == (p_ue_context->ho_info.p_ho_info->
                  p_src_ho_info->p_re_establishment_req =
                  rrc_mem_get(len)))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL,
                "Memory Allocation Failed to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* storing re_establishment contents into p_re_establishment_req */
    l3_memcpy_wrapper(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
               p_re_establishment_req,
               p_api,
               len);


    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type10_actions);

    /* SPR 16750 Fix Start */
    if (p_ue_context->x2ap_context.m.error_indication_ongoing)
    {
        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type20_actions);
    }
    /* SPR 16750 Fix Stop */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}
/*SPR_19066_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_get_ho_prep_info_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static 
void uecc_fsm_src_ho_get_ho_prep_info_ongoing_init_handler(
/*SPR_19066_END*/
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* for inter-RAT, no need to send MAC I Request and Inactive Time Req  */
    if ((HANDOVER_TYPE_LTE_TO_UTRAN == p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) ||
            (HANDOVER_TYPE_LTE_TO_GERAN == p_ue_context->ho_info.p_ho_info->
             p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type ))

    {
        
/*SPR_19066_START*/
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        " for inter-RAT, no need to send"
                        "HO PREP INFO Req");
/*SPR_19066_END*/
        /* moving to next child state */
        UECC_FSM_SET_CHILD_STATE(p_ue_context,
                &uecc_fsm_src_ho_prep_ongoing, PNULL);
    }
    else if(HANDOVER_TYPE_INTRA_LTE_S1 ==  p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type ||
            HANDOVER_TYPE_INTRA_LTE_X2 ==  p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type ||
            HANDOVER_TYPE_INTER_CELL == p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type  || 
            /*BUG 604 changes start*/
            HANDOVER_TYPE_INTRA_CELL == p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
            /*BUG 604 changes stop*/
    {
        /*SPR_16653_START*/
        /* For MAC-I generation SeNB CRNTI has to be used.
         * Copy crnti to old_crnti which is used for generation
         * of ASN encoded VarShortMAC-I*/
        p_ue_context->old_crnti = p_ue_context->crnti; 
        /*SPR_16653_END*/
/*SPR_19066_START*/
        /* Build and sent the HO PREP INFO REQ to LLIM */
        if (RRC_SUCCESS != uecc_llim_build_and_send_ho_prep_info_req(p_ue_context))
/*SPR_19066_END*/
        {
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type2_actions);
        }
    }
            /*BUG 604 changes start*/
            /*BUG 604 changes stop*/
    else if ((HANDOVER_TYPE_LTE_TO_CDMA2000_1XRTT == 
		p_ue_context->ho_info.p_ho_info->p_src_ho_info->
		s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) ||
	      (HANDOVER_TYPE_LTE_TO_CDMA2000_HRPD == 
		p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) ||
	       (HANDOVER_TYPE_LTE_TO_CDMA2000_CONC_1XRTT_HRPD == 
		p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
	    )
     {
		/* Build and sent the HO from EUTRA Req to LLIM */
        if (RRC_SUCCESS != uecc_llim_build_and_send_ho_from_eutra_prep_req(p_ue_context))
        {
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type2_actions);
        }
     } 
    else if (HANDOVER_TYPE_CCO ==  p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO, 
                "CCO handover ongoing, no need to send HO prep");

        /* moving to next child state */
        UECC_FSM_SET_CHILD_STATE(p_ue_context,
                &uecc_fsm_src_ho_llim_re_est_ongoing, PNULL);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/*SPR_19066_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_ho_prep_info_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_LLIM_HO_PREP_INFO_RESP from a the LLIM.
 ****************************************************************************/
void uecc_fsm_src_ho_llim_ho_prep_info_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    rrc_uecc_llim_ho_prep_info_resp_t*  p_rrc_uecc_llim_ho_prep_info_resp = PNULL;
    rrc_return_t                    result = RRC_FAILURE;
    uecc_fsm_fail_handler_f const*  p_failure_actions_list =
        uecc_fsm_src_ho_fail_type2_actions;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    p_rrc_uecc_llim_ho_prep_info_resp = (rrc_uecc_llim_ho_prep_info_resp_t*)p_api_data;

    switch (p_rrc_uecc_llim_ho_prep_info_resp->response_code)
    {
        case RRC_SUCCESS:

            if(RRC_UECC_LLIM_MACI_MSG_RESP_PRESENT & 
                        p_rrc_uecc_llim_ho_prep_info_resp->bitmask)
            {
                /* Store message data in UE context for future processing */
                /* Process UECC_LLIM_ho_prep_info_resp message data */
                result = uecc_fsm_src_ho_process_llim_ho_prep_info_resp_data(
                        p_ue_context,
                        p_rrc_uecc_llim_ho_prep_info_resp);

                if ( RRC_SUCCESS == result)
                {
                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->bitmask |=
                        PDCP_MAC_I_PRESENT;
                        /* moving to next child state */
                        UECC_FSM_SET_CHILD_STATE(p_ue_context,
                                &uecc_fsm_src_ho_prep_ongoing, PNULL);
                    }
                }
            else
            {
                /* It should be present in case of success
                 * See LTE RRC API Reference Manual UECC_LLIM_HO_PREP_INFO_RESP */
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                        "[UECC_LLIM_HO_PREP_INFO_RESP] MAC-I is absent");
            }
            break;

        case RRC_PARTIAL_SUCCESS:
            /* RRM rejected connection */
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "[UECC_LLIM_HO_PREP_INFO_RESP] RRC_PARTIAL_SUCCESS response received from LLIM");
            break;

        case RRC_FAILURE:
            /* RRM rejected connection */
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_HO_PREP_INFO_RESP] RRC_FAILURE response received from LLIM");
            break;

        default:
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_HO_PREP_INFO_RESP] Invalid Response=%i",
                    p_rrc_uecc_llim_ho_prep_info_resp->response_code);
            break;
    }

    /* Process failure */
    if (RRC_SUCCESS != result)
    {
        /* filling rrm_cause using s1ap_cause received in ue_context*/
    /*BUG 604 changes start*/
        if((X2_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
        {
            p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
        
            /* stopping X2 HO Prep Timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
        }
        else
        {
            p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
        
            /* stopping S1 HO Prep Timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
        }
        /*x2_ho_timer*/
        /* stopping HO Prep Timer */
        /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);*/
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(p_ue_context,
                p_failure_actions_list);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_process_llim_ho_prep_info_resp_data 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  p_rrc_uecc_llim_ho_prep_info_resp* - pointer on message data
 * Outputs        : None
 * Returns        : rrc_return_t
 * Description    : Process data from p_rrc_uecc_llim_ho_prep_inf_resp message and
 *                  update UE context.
 ****************************************************************************/
    static 
rrc_return_t uecc_fsm_src_ho_process_llim_ho_prep_info_resp_data(
        uecc_ue_context_t*           p_ue_context,
        rrc_uecc_llim_ho_prep_info_resp_t* p_rrc_uecc_llim_ho_prep_info_resp 
        )
{
    rrc_return_t result = RRC_SUCCESS;
    rrc_counter_t i = 0, j = 0;
    rrc_bool_t targetCellReceived = RRC_FALSE;
    /* Bug 9299 Fix Start */
    U32  cell_identity = RRC_NULL;
    /* Bug 9299 Fix End */


    /* Store UE parameters in UE context and apply default values for them
     * if its needed.*/
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_rrc_uecc_llim_ho_prep_info_resp);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    do {

        if (!( RRC_UECC_LLIM_MACI_MSG_RESP_PRESENT
                    & p_rrc_uecc_llim_ho_prep_info_resp->bitmask))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                    "[UECC_LLIM_HO_PREP_INFO_RESP] MSG AUTH CODE LIST is absent");
            break;
        }

        /* verify for short mac I count received adding 1 into target_cell_count because 1 
         is for actual target cell id related cell_identity */
        if (p_rrc_uecc_llim_ho_prep_info_resp->msg_auth_code_list.mac_i_count !=
                p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->source_to_target_container.eNBContainer.
                rrc_container.target_cell_identity_list.target_cell_count + 1 )
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                    "[UECC_LLIM_HO_PREP_INFO_RESP] number of recevied mac_i is invalid");
            result = RRC_FAILURE;
            break;
        }
        /* filling short_mac_i */
        for ( i = 0, j=0; 
                (i < p_rrc_uecc_llim_ho_prep_info_resp->msg_auth_code_list.mac_i_count) 
                && (p_rrc_uecc_llim_ho_prep_info_resp->msg_auth_code_list.mac_i_count < 
                  MAX_NO_OF_MESSAGES) ; i++ )
        {
            /* find the target cell identity to fill the short 
               mac i for target cell */
            if(!targetCellReceived )
            {
                /* Bug 9299 Fix Start */
                rrc_unpack_U32(&cell_identity,
                       &p_rrc_uecc_llim_ho_prep_info_resp->msg_auth_code_list.mac_i[i].cell_identity);
                //SPR 19 sep 2011
                if(!strncmp_wrapper((const S8 *)(p_ue_context->ho_info.p_ho_info->
                            p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->
                            target_id.enb_id.global_enb_id.cell_identity),
                            /* SPR 11711 Fix Start */
                            (const S8 *)(&p_rrc_uecc_llim_ho_prep_info_resp->
                            msg_auth_code_list.mac_i[i].cell_identity),
                            /* SPR 11711 Fix Stop */
                            sizeof(p_rrc_uecc_llim_ho_prep_info_resp->
                                msg_auth_code_list.mac_i[i].cell_identity)))
                /* Bug 9299 Fix End */
                {
                    
                    rrc_pack_U32(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                            target_cell_short_mac_i.cell_identity,
                            &p_rrc_uecc_llim_ho_prep_info_resp->
                            msg_auth_code_list.mac_i[i].cell_identity);
                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                        target_cell_short_mac_i.short_mac_i
                        = (U16) p_rrc_uecc_llim_ho_prep_info_resp->
                        msg_auth_code_list.mac_i[i].
                        encoded_mac_i;
                    targetCellReceived = RRC_TRUE;
                    continue;
                }
            }

            rrc_pack_U32(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    short_mac_I[j].cell_identity,
                    &p_rrc_uecc_llim_ho_prep_info_resp->
                    msg_auth_code_list.mac_i[i].
                    cell_identity);

            p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                short_mac_I[j].short_mac_i 
                = (U16) p_rrc_uecc_llim_ho_prep_info_resp->msg_auth_code_list.
                mac_i[i].encoded_mac_i;
            j++;
        }
        if(RRC_UECC_LLIM_HO_PREP_INACTIVE_TIME_RESP_PRESENT &
                p_rrc_uecc_llim_ho_prep_info_resp->bitmask)
        {
            p_ue_context->ho_info.p_ho_info->p_src_ho_info->ue_inactive_time 
                = p_rrc_uecc_llim_ho_prep_info_resp->ue_inactivity_time_resp.ue_inactive_time_val;
            p_ue_context->ho_info.p_ho_info->p_src_ho_info->bitmask |=
                MAC_UE_INACTIVE_TIME_PRESENT;
        }
    }while(0);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}
/*SPR_19066_END*/

/*SPR_19066_START*/
/* Code Removed */
/*SPR_19066_END*/

/****************************************************************************
 * Function Name  : uecc_fsm_valiating_erab_config_for_ho_procedure
 * Inputs         : uecc_ue_context_t - p_ue_context
 * Outputs        : None  
 * Returns        : rrc_bool_t
 * Description    : This is the function for to validate the ERAB Config 
 *                  in order to process handover procedure
 ****************************************************************************/
static 
rrc_bool_t uecc_fsm_valiating_erab_config_for_ho_procedure(
        uecc_ue_context_t*  p_ue_context)
{
    rrc_bool_t rrc_return = RRC_FALSE;
    rrc_counter_t erab_id = 0;


    RRC_ASSERT(PNULL!=p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    for(erab_id = 0; erab_id < MAX_ERAB_COUNT; erab_id ++)
    {
        if(p_ue_context->p_e_rab_list[erab_id])
        {
            if(p_ue_context->p_e_rab_list[erab_id]->e_RABlevelQoSParameters.qCI >= RRC_MIN_QCI_NON_GBR_LIMIT 
                    && p_ue_context->p_e_rab_list[erab_id]->e_RABlevelQoSParameters.qCI <= RRC_MAX_QCI_NON_GBR_LIMIT)
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        "At least one non GBR ERAB ID[%d] is present",erab_id);
                    rrc_return = RRC_TRUE;
                break;
            }

        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return rrc_return;
}

/******************************************************************************
 *   FUNCTION NAME: rrc_validate_peer_cell_info_and_get_peer_enb_context 
 *   INPUT        : uecc_ue_context_t* p_ue_context
 *                  rrc_rrm_ho_required_t* p_ho_req
 *                  U8 *peer_enodeb_id
 *                  U8 *serving_cell_index
 *   OUTPUT       : None               
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE 
 *   DESCRIPTION  : This function validate the ecgi for the target cell of peer eNB, 
 *                  received in RRC_RRM_HO_REQUIRED 
 *******************************************************************************/
static 
rrc_return_et rrc_validate_peer_cell_info_and_get_peer_enb_context(
        uecc_ue_context_t* p_ue_context,
        rrc_rrm_ho_required_t* p_ho_req,
        U8 *peer_enodeb_id,
        U8 *serving_cell_index
        )
{

    U8 o_c, i_c = 0;
    U16 num_served_cells = 0;
    void* p_srvd_cell_info = PNULL;

    rrc_return_et retVal = RRC_FAILURE;

    RRC_ASSERT(PNULL!=p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL!=p_ho_req);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    for(o_c = 0; o_c < MAX_PEER_ENB; o_c ++)
    {
        if(RRC_TRUE == 
                p_ue_context->p_gb_context->x2ap_ctx.x2ap_link_info[o_c].
                connection_status)
        {

            if( PNULL != (p_srvd_cell_info = p_ue_context->p_gb_context->x2ap_ctx.
                        x2ap_link_info[o_c].p_srvd_cell_info))
            {
                num_served_cells = p_ue_context->p_gb_context->x2ap_ctx.
                    x2ap_link_info[o_c].num_served_cells;

                for(i_c = 0 ;i_c < num_served_cells; i_c ++)
                {
                    if (!memcmp_wrapper(p_ho_req->target_id.enb_id.global_enb_id.ecgi,
                                ((x2ap_srvd_cell_info_t*)((U8*)p_srvd_cell_info +
                                 (sizeof(x2ap_srvd_cell_info_t)* i_c)))->
                                cell_id.eutran_cell_id,
                                HOME_ENB_ID_OCTET_SIZE*sizeof(U8)))
                    {
                        *peer_enodeb_id = o_c;
                        *serving_cell_index = i_c;
                        retVal  =  RRC_SUCCESS;
                        break;
                    }
                }

                if(retVal  ==  RRC_SUCCESS)
                {
                    break;
                }
            }
        }
    }
    if(MAX_PEER_ENB == o_c)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_ERROR,"No serving cell info present in peer eNodeB Context"
                "ECGI value received is invalid");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return retVal;

}

/****************************************************************************
 * Function Name  : uecc_validating_lte_to_geran_ho_hndlr
 * Inputs         : uecc_ue_context_t - p_ue_context
 *		    rrc_rrm_ho_required_t - pointer to ho_req 
 * Outputs        : None  
 * Returns        : rrm_ho_req_valid 
 * Description    : This is the function to validate the HO REQUIRED 
 *                  message recevied from RRM in case of LTE_TO_GERAN.
 ****************************************************************************/
 static rrc_bool_t uecc_validating_lte_to_geran_ho_hndlr(
        uecc_ue_context_t*  p_ue_context,
        rrc_rrm_ho_required_t *p_ho_req 
)
{
    rrc_bool_t rrm_ho_req_valid = RRC_TRUE;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    if(RRC_RRM_UE_HO_ADM_REQ_SRVCC_HO_INDICATION_PRESENT & p_ho_req->bitmask)
    {
        /* SPR 20635 Fix - SRVCC to GERAN - Starts */
        /* In case CS Only SRVCC to GERAN, 
         * Secondary Transparent Container should not present AND
         * Primary containter should contain Old BSS to New BSS information
         */
        if((CS_ONLY_TYPE== p_ho_req->srvcc_ho_indication)
            &&(((RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_SECONDARY_PRESENT
                & p_ho_req->bitmask)) ||
            (!(RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_PRESENT
                & p_ho_req->bitmask)) ||
            (!(OLD_TO_NEW_TRANSPARENT_BSS_CONTAINER_PRESENT
                & p_ho_req->source_to_target_container.bitmask))))
            /*SPR 13721 Fix Start*/
            /* Check for Source BSS to Target BSS - Removed */
            /*SPR 13721 Fix Stop*/

        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                "[uecc_fsm_valiating_rrm_ho_required_msg]"
                "either secondary container is present in RRC RRM HO Required"
                "or primary container is not old bss to new bss container");

            rrm_ho_req_valid = RRC_FALSE;
        }
        /* In case CS + PS SRVCC to GERAN, 
         * Primary containter should contain Source BSS to Target BSS Container AND
         * Secondary containter should contain Old BSS to New BSS Information
         */
        else if(CS_PS_TYPE == p_ho_req->srvcc_ho_indication)
        {
            /*SPR 13721 Fix Start*/
            /*Code removed*/
            /*SPR 13721 Fix Stop*/
            if((!(RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_PRESENT
                            & p_ho_req->bitmask))
                    ||(!(SOURCE_TO_TARGET_TRANSPARENT_BSS_CONTAINER_PRESENT
                            & p_ho_req->source_to_target_container.bitmask))
                    ||(!(RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_SECONDARY_PRESENT
                        & p_ho_req->bitmask)))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                        "[uecc_fsm_valiating_rrm_ho_required_msg]"
                        "Either Primary container is not present with Source BSS to Target BSS"
                        " or Secondary Container not present");
                rrm_ho_req_valid = RRC_FALSE;
            }
        }
        /* SPR 20635 Fix - SRVCC to GERAN - Ends */
    }
    else if((!(RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_PRESENT
                    & p_ho_req->bitmask))
            ||(!(SOURCE_TO_TARGET_TRANSPARENT_BSS_CONTAINER_PRESENT
                    & p_ho_req->source_to_target_container.bitmask)))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                "[uecc_fsm_valiating_rrm_ho_required_msg]"
                "Primary container is not present in PS domain");
        rrm_ho_req_valid = RRC_FALSE;
    }
    p_ue_context->ho_info.s1_or_x2_handover = S1_HO;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return rrm_ho_req_valid;
}

/********************************************************************************
 * Function Name  : uecc_validating_intra_lte_x2_ho_hndlr
 * Inputs         : uecc_ue_context_t - p_ue_context
 *		    rrc_rrm_ho_required_t - pointer to ho_req 
 * Outputs        : None  
 * Returns        : rrm_ho_req_valid
 * Description    : This is the function to validate the HO REQUIRED 
 *                  message recevied from RRM in case of X2 based handover
 ********************************************************************************/
static rrc_bool_t uecc_validating_intra_lte_x2_ho_hndlr(
        uecc_ue_context_t*  p_ue_context,
        rrc_rrm_ho_required_t *p_ho_req 
)
{
    rrc_bool_t rrm_ho_req_valid = RRC_TRUE;
    U8 peer_enodeb_id;
    U8 serving_cell_index;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    if((!(RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_PRESENT
                    & p_ho_req->bitmask))
            ||(!(SOURCE_TO_TARGET_TRANSPARENT_ENB_CONTAINER_PRESENT
                    & p_ho_req->source_to_target_container.bitmask))||
            (!(RRM_TARGET_ID_ENB_ID_PRESENT & p_ho_req->target_id.
               bitmask)))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                "[uecc_fsm_valiating_rrm_ho_required_msg]"
                "either enb container, srvcc info or enb id is not present");
        rrm_ho_req_valid = RRC_FALSE;
    }
    if (!(EUTRAN_CELL_GLOBAL_IDENTITY_PRESENT
                & p_ho_req->target_id.enb_id.global_enb_id.bitmask))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                "[uecc_fsm_valiating_rrm_ho_required_msg]"
                "ECGI is not present");
        rrm_ho_req_valid = RRC_FALSE;
    }
    if(RRC_SUCCESS == rrc_validate_peer_cell_info_and_get_peer_enb_context(
                p_ue_context,
                p_ho_req,
                &peer_enodeb_id,
                &serving_cell_index))
    {
        /* Store peer_enodeb_id in X2AP UE context */
        p_ue_context->x2ap_context.peer_enodeb_id
            = peer_enodeb_id;
        p_ue_context->x2ap_context.serving_cell_index
            = serving_cell_index;
    }
    else
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                "[uecc_fsm_valiating_rrm_ho_required_msg] "
                "rrc_validate_peer_cell_info_and_get_peer_enb_context failed");
        rrm_ho_req_valid = RRC_FALSE;
    }
    p_ue_context->ho_info.s1_or_x2_handover = X2_HO;
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return rrm_ho_req_valid;
}

/****************************************************************************
 * Function Name  : uecc_fsm_valiating_rrm_ho_required_msg 
 * Inputs         : void* - pointer to api
 *                  uecc_ue_context_t - p_ue_context
 * Outputs        : None  
 * Returns        : rrc_bool_t
 * Description    : This is the function for to validate the HO REQUIRED 
 *                  message recevied from RRM.
 ****************************************************************************/
static 
rrc_bool_t uecc_fsm_valiating_rrm_ho_required_msg(
        uecc_ue_context_t*  p_ue_context,
        void* p_api)
{
    rrc_rrm_ho_required_t *p_ho_req = (rrc_rrm_ho_required_t*)p_api;
    rrc_bool_t rrm_ho_req_valid = RRC_TRUE;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL!=p_api);

    /* validating the handover type */
    /*srvcc-u start*/
    if((HANDOVER_TYPE_INTRA_LTE_X2 != p_ho_req->ho_type) &&
            (RRC_RRM_UE_HO_ADM_REQ_SRVCC_HO_OP_POSSIBLE_PRESENT &
               p_ho_req->bitmask))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                "[uecc_fsm_valiating_rrm_ho_required_msg]"
                "SRVCC ho operation should not present if ho_type is not X2AP");
        rrm_ho_req_valid = RRC_FALSE;
    }
    /*srvcc-u staop*/

    switch(p_ho_req->ho_type)
    {
        case HANDOVER_TYPE_INTRA_LTE_S1:
            if((!(RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_PRESENT
                            & p_ho_req->bitmask))
                    ||(!(SOURCE_TO_TARGET_TRANSPARENT_ENB_CONTAINER_PRESENT
                            & p_ho_req->source_to_target_container.bitmask))||
                    (!(RRM_TARGET_ID_ENB_ID_PRESENT & p_ho_req->target_id.bitmask)) ||
                    (RRC_RRM_UE_HO_ADM_REQ_SRVCC_HO_INDICATION_PRESENT 
                     &p_ho_req->bitmask))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        "[uecc_fsm_valiating_rrm_ho_required_msg]"
                        "either enb container or enb id is not present");
                rrm_ho_req_valid = RRC_FALSE;
            }
            p_ue_context->ho_info.s1_or_x2_handover = S1_HO; 
            break;

        case HANDOVER_TYPE_LTE_TO_UTRAN:

            if((!(RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_PRESENT
                            & p_ho_req->bitmask))
                    ||(!(SOURCE_TO_TARGET_TRANSPARENT_RNC_CONTAINER_PRESENT
                            & p_ho_req->source_to_target_container.bitmask)))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        "[uecc_fsm_valiating_rrm_ho_required_msg]"
                        "rnc container is not present");
                rrm_ho_req_valid = RRC_FALSE;
            }
            p_ue_context->ho_info.s1_or_x2_handover = S1_HO; 
            break;

        case HANDOVER_TYPE_LTE_TO_GERAN:
            rrm_ho_req_valid = uecc_validating_lte_to_geran_ho_hndlr(p_ue_context,p_ho_req);
            break;

        case HANDOVER_TYPE_INTRA_LTE_X2:
            rrm_ho_req_valid = uecc_validating_intra_lte_x2_ho_hndlr(p_ue_context,p_ho_req);
            break;
        
        case HANDOVER_TYPE_INTRA_CELL:
    /*BUG 604 changes start*/
            if((!(RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_PRESENT
                            & p_ho_req->bitmask))
                    ||(!(SOURCE_TO_TARGET_TRANSPARENT_ENB_CONTAINER_PRESENT
                            & p_ho_req->source_to_target_container.bitmask))||
                    (!(RRM_TARGET_ID_ENB_ID_PRESENT & p_ho_req->target_id.
                       bitmask))||
                    (!(RRC_RRM_UE_HO_ADM_REQ_MOBILITY_CONTROL_INFO_PRESENT
                     & p_ho_req->bitmask)))
    /*BUG 604 changes stop*/
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        "[uecc_fsm_valiating_rrm_ho_required_msg]"
                        "T_304 in MOBILITY_CONTROL_INFO not present in intra_cell ho"
                        "or either enb container or enb id is not present");
                rrm_ho_req_valid = RRC_FALSE;
            }
            else
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        "[uecc_fsm_valiating_rrm_ho_required_msg in intra_cell ho]");
                p_ue_context->ho_info.s1_or_x2_handover = INTRA_CELL_HO; 
            }
           break; 
        
        case HANDOVER_TYPE_LTE_TO_CDMA2000_1XRTT:
        {  
            if(!(RRC_RRM_CDMA2000_INFO_PRESENT & p_ho_req->bitmask))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        "[uecc_fsm_valiating_rrm_ho_required_msg]"
                        "cdma2000_information is not present in CDMA2000_1XRTT HO");
                rrm_ho_req_valid = RRC_FALSE;
            }
            else if ((RRC_RRM_CDMA2000_INFO_PRESENT & p_ho_req->bitmask) &&
                    ((!(RRC_RRM_HO_REQUIRED_MOBILITY_PARAMETERS_PRESENT & 
                        p_ho_req->cdma2000_information.bitmask)) ||
                     (!(RRC_RRM_HO_REQUIRED_ADDITIONAL_MOBILITY_PARAMETERS_PRESENT &
                        p_ho_req->cdma2000_information.bitmask))))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        "[uecc_fsm_valiating_rrm_ho_required_msg]"
                        "mobility_parameters is not present in CDMA2000_1XRTT HO");
                rrm_ho_req_valid = RRC_FALSE;
            }
            else
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        "[uecc_fsm_valiating_rrm_ho_required_msg]"
                        "mobility_parameters is present in CDMA2000_1XRTT HO");
                p_ue_context->ho_info.s1_or_x2_handover = S1_HO; 
                p_ue_context->ho_info.cdma_ho_ongoing = RRC_TRUE;
                p_ue_context->cdma2000_info.w_f_1xrtt_msg = RRC_FALSE;
            }
            break; 
        }
        case HANDOVER_TYPE_LTE_TO_CDMA2000_HRPD:
        {
           if(!(RRC_RRM_CDMA2000_INFO_PRESENT & p_ho_req->bitmask))
           {
               RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                       "[uecc_fsm_valiating_rrm_ho_required_msg]"
                       "cdma2000_information is not present in CDMA2000_1XRTT HO");
               rrm_ho_req_valid = RRC_FALSE;
           }
           else if((RRC_RRM_CDMA2000_INFO_PRESENT & p_ho_req->bitmask) && 
                   ((RRC_RRM_HO_REQUIRED_MOBILITY_PARAMETERS_PRESENT & 
                    p_ho_req->cdma2000_information.bitmask)||
                   (RRC_RRM_HO_REQUIRED_ADDITIONAL_MOBILITY_PARAMETERS_PRESENT &
                    p_ho_req->cdma2000_information.bitmask)))
           {
               RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                       "[uecc_fsm_valiating_rrm_ho_required_msg]"
                       "mobility_parameters is present in CDMA2000_HRPD HO");
               rrm_ho_req_valid = RRC_FALSE;
           }
           else
           {
               RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                       "[uecc_fsm_valiating_rrm_ho_required_msg]"
                       "mobility_parameters is not present in CDMA2000_HRPD HO");
               p_ue_context->ho_info.s1_or_x2_handover = S1_HO; 
               p_ue_context->ho_info.cdma_ho_ongoing = RRC_TRUE;
               p_ue_context->cdma2000_info.w_f_hrpd_msg = RRC_FALSE;
           }
           break; 
        }
	    case HANDOVER_TYPE_CCO:
        {
           if(!(RRC_RRM_UE_HO_ADM_REQ_MOBILITY_CONTROL_INFO_PRESENT & p_ho_req->bitmask) ||
              !(RRC_RRM_UE_HO_CCO_PARAMS_PRESENT & p_ho_req->bitmask) || 
              !(RRC_RRM_UE_CCO_TARGET_RAT_GERAN_TYPE_PRESENT & 
                             p_ho_req->cco_params.target_rat_params.bitmask ) ||
               (RRC_TRUE != uecc_fsm_validate_target_rat_geran_params(p_ue_context,&p_ho_req->cco_params.target_rat_params.geran_params)))
               /*SPR 17777 +-*/
           {
               RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                       "[uecc_fsm_valiating_rrm_ho_required_msg]"
                       "incomplete CCO Parameters received");
               rrm_ho_req_valid = RRC_FALSE;
           }
           else
           {
               RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                               "[uecc_fsm_valiating_rrm_ho_required_msg]"
                               "CCO parameters are present in HO_REQUIRED");
               p_ue_context->ho_info.s1_or_x2_handover = CCO_HO; 
           }
           break; 
        }
        case HANDOVER_TYPE_LTE_TO_CDMA2000_CONC_1XRTT_HRPD:
        {
            if(!(RRC_RRM_CDMA2000_INFO_PRESENT & p_ho_req->bitmask))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        "[uecc_fsm_valiating_rrm_ho_required_msg]"
                        "cdma2000_information is not present in CDMA2000_1XRTT HO");
                rrm_ho_req_valid = RRC_FALSE;
            }
            else if((RRC_RRM_CDMA2000_INFO_PRESENT & p_ho_req->bitmask) && 
                    ((!(RRC_RRM_HO_REQUIRED_MOBILITY_PARAMETERS_PRESENT & 
                        p_ho_req->cdma2000_information.bitmask)) ||
                     (!(RRC_RRM_HO_REQUIRED_ADDITIONAL_MOBILITY_PARAMETERS_PRESENT &
                        p_ho_req->cdma2000_information.bitmask))))	
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        "[uecc_fsm_valiating_rrm_ho_required_msg]"
                        "mobility_parameters is not present in CDMA2000_CONC_1XRTT_HRPD");
                rrm_ho_req_valid = RRC_FALSE;
            }
            else
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        "[uecc_fsm_valiating_rrm_ho_required_msg]"
                        "mobility_parameters is present in CDMA2000_CONC_1XRTT_HRPD");
                p_ue_context->ho_info.s1_or_x2_handover = S1_HO; 
                p_ue_context->ho_info.cdma_ho_ongoing = RRC_TRUE;
                p_ue_context->ho_info.dl_s1_cdma2000_tunneling_received_once = RRC_FALSE;
                p_ue_context->cdma2000_info.w_f_hrpd_msg = RRC_FALSE;
                p_ue_context->cdma2000_info.w_f_1xrtt_msg = RRC_FALSE;
            }
            break; 
        }
        case HANDOVER_TYPE_INTER_CELL:
            if((!(RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_PRESENT
                            & p_ho_req->bitmask))
                    ||(!(SOURCE_TO_TARGET_TRANSPARENT_ENB_CONTAINER_PRESENT
                            & p_ho_req->source_to_target_container.bitmask))||
                    (!(RRM_TARGET_ID_ENB_ID_PRESENT & p_ho_req->target_id.
                       bitmask)))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        "[uecc_fsm_valiating_rrm_ho_required_msg in inter_cell ho]"
                        "either enb container or enb id is not present");
                rrm_ho_req_valid = RRC_FALSE;
            }
            p_ue_context->ho_info.s1_or_x2_handover = INTER_CELL_HO; 
            break;
       
        default:
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                "INVALID HANDOVER TYPE RECEIVED %u", p_ho_req->ho_type);
        rrm_ho_req_valid = RRC_FALSE;
        p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO; 
        break;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return rrm_ho_req_valid;
}

/****************************************************************************
 * Function Name  : uecc_fsm_valiate_s1ap_ho_command_msg 
 * Inputs         : uecc_ue_context_t* p_ue_context
 *                  rrc_s1ap_handover_command_t *p_ho_cmd
 * Outputs        : None  
 * Returns        : rrc_bool_t
 * Description    : This is the function for to validate the HO COMMAND 
 *                  message recevied from S1AP.
 ****************************************************************************/
static 
rrc_bool_t uecc_fsm_valiate_s1ap_ho_command_msg(
        uecc_ue_context_t* p_ue_context,
        rrc_s1ap_handover_command_t *p_ho_cmd
        )
{
    RRC_ASSERT(PNULL!=p_ho_cmd);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    if( p_ho_cmd->bitmask &
            RRC_S1AP_HANDOVER_COMMAND_ERAB_SUBJECT_TO_DATA_FORWARDING_LIST_PRESENT )
    {
        if ( RRC_SUCCESS != uecc_fsm_validate_and_fill_erab_forwarding_list(
                    p_ue_context,
                    &(p_ho_cmd->erab_subject_to_data_forwarding_list)))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                    "INVALID HANDOVER COMMAND RECEIVED FROM S1AP");
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return RRC_FALSE;
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_TRUE;
}


/******************************************************************************
 *   FUNCTION NAME: uecc_fsm_fill_erab_fail_list 
 *   INPUT        : uecc_ue_context_t* p_ue_context
 *                  s1ap_E_RABList *p_erab_fail_list
 *   OUTPUT       : None               
 *   RETURNS      : RRC_SUCCESS/RRC_PARTIAL_SUCCESS/RRC_FAILURE
 *   DESCRIPTION  : This function fill the  the erab_fail_list 
 *******************************************************************************/
static rrc_return_et uecc_fsm_fill_erab_fail_list(
        uecc_ue_context_t* p_ue_context,
        s1ap_E_RABList *p_erab_fail_list
        )
{
    OSRTDListNode* p_node = PNULL;
    s1ap_E_RABList_element *p_elem = PNULL;
    rrc_return_et ret = RRC_SUCCESS;
    uecc_e_rab_failed_to_add_list_t *p_uecc_e_rab_failed_to_add_list = PNULL;

    RRC_ASSERT(PNULL!=p_ue_context);
    RRC_ASSERT(PNULL!=p_erab_fail_list);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    p_uecc_e_rab_failed_to_add_list = &(p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.erab_failed_list);

    for (p_node = p_erab_fail_list->head, 
            p_uecc_e_rab_failed_to_add_list->count = 0; PNULL!=p_node;
            p_node=p_node->next, p_uecc_e_rab_failed_to_add_list->count++ )
    {
        p_elem = (s1ap_E_RABList_element*)p_node->data;
        RRC_ASSERT(PNULL!=p_elem);
        RRC_ASSERT(PNULL!=p_elem->value.u._E_RABItemIEs_1);
        if (ASN1V_s1ap_id_E_RABItem != p_elem->id)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                    "ASN1V_s1ap_id_E_RABItem ! =p_elem"
                    "->id (%i)",
                    p_elem->id);
            /* filling the cause in p_ue_context */
            p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
            ret = RRC_FAILURE;
            break;
        }

        /* filling erab ID */
        p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list
            [p_uecc_e_rab_failed_to_add_list->count].e_rab_id =
            p_elem->value.u._E_RABItemIEs_1->e_RAB_ID;

        /* filling cause */
        p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list
            [p_uecc_e_rab_failed_to_add_list->count].cause =
            p_elem->value.u._E_RABItemIEs_1->cause;
    }


    if(p_uecc_e_rab_failed_to_add_list->count)
    {
        p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.bitmask |=
            E_RAB_FAILED_LIST_PRESENT;
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return ret;
}

/****************************************************************************
 * Function Name  : uecc_fsm_validate_x2ap_ho_req_ack_msg 
 * Inputs         : uecc_ue_context_t* p_ue_context
 *                  rrc_x2ap_handover_req_ack_t *p_ho_req_ack
 * Outputs        : None  
 * Returns        : rrc_bool_t
 * Description    : This is the function for to validate the HO REQUEST ACK 
 *                  message recevied from X2AP.
 ****************************************************************************/
static 
rrc_bool_t uecc_fsm_validate_x2ap_ho_req_ack_msg(
        uecc_ue_context_t* p_ue_context,
        rrc_x2ap_handover_req_ack_t *p_ho_req_ack
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL!=p_ho_req_ack);

    if ( RRC_SUCCESS != uecc_fsm_validate_and_fill_erab_admitted_list(
                p_ue_context,
                &(p_ho_req_ack->e_rab_admitted_list)))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                "INVALID HANDOVER REQ ACK RECEIVED FROM X2AP");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FALSE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_TRUE;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_fsm_validate_and_fill_erab_admitted_list 
 *   INPUT        : uecc_ue_context_t* p_ue_context
 *                  x2ap_E_RABs_Admitted_List *p_erab_admitted_list
 *   OUTPUT       : None               
 *   RETURNS      : RRC_SUCCESS/RRC_PARTIAL_SUCCESS/RRC_FAILURE
 *   DESCRIPTION  : This function validate the erab_fowarding_list 
 *******************************************************************************/
static 
rrc_return_et uecc_fsm_validate_and_fill_erab_admitted_list(
        uecc_ue_context_t* p_ue_context,
        x2ap_E_RABs_Admitted_List *p_erab_admitted_list
        )
{
    OSRTDListNode* p_node = PNULL;
    rrc_return_et ret = RRC_SUCCESS;
    x2ap_E_RABs_Admitted_List_element *p_elem = PNULL;
    uecc_ho_src_drb_admitted_list_t *p_uecc_ho_src_drb_admitted_list = PNULL;
    rrc_bool_et     tunnel_info_present = RRC_FALSE;
    U8      erab_idx = 0;
    RRC_ASSERT(PNULL!=p_ue_context);
    RRC_ASSERT(PNULL!=p_erab_admitted_list);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_uecc_ho_src_drb_admitted_list = &(p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.src_erab_admitted_list);
    
    if ( PNULL != p_erab_admitted_list->head )
    {
        p_ue_context->ho_info.p_ho_info->p_src_ho_info->
          s1_src_ho_info.bitmask |= X2AP_ERAB_ADMITTED_LIST_PRESENT;
    }

    for( p_node = p_erab_admitted_list->head,
            p_uecc_ho_src_drb_admitted_list->count = 0; PNULL!=p_node;
            p_node=p_node->next,p_uecc_ho_src_drb_admitted_list->count++) 
    {
        p_elem = (x2ap_E_RABs_Admitted_List_element*)p_node->data;
        if (PNULL == p_elem)
        {
            continue;
        }
        if (ASN1V_x2ap_id_E_RABs_Admitted_Item != p_elem->id)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                    "ASN1V_x2ap_id_E_RABs_Admitted_Item != p_elem"
                    "->id (%i)",
                    p_elem->id);
            p_ue_context->ho_info.cause.u.protocol = x2ap_transfer_syntax_error;
            ret = RRC_FAILURE;
            break;

        }
        /* memory allocation */
        erab_idx = (U8)p_elem->value.e_RAB_ID;
        if(INVALID_ERAB_ID <= erab_idx )
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                    "The e_RAB_ID[%d] present is invalid",erab_idx);
            ret = RRC_FAILURE;
            continue;
        }
        if( PNULL == 
                (p_uecc_ho_src_drb_admitted_list->
                 erab_ho_src_drb_admitted_info[erab_idx]
                 = (uecc_ho_src_drb_admitted_info_t *)
                 rrc_mem_get(sizeof(uecc_ho_src_drb_admitted_info_t))))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL, "Memory " 
                    "allocation failure");
            return RRC_FAILURE;
        }

        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */

        /* Check if at least one DL transport address, TEID pair or
           UL transport address, TEID pair shall present */

        tunnel_info_present = RRC_FALSE;

        if (p_elem->value.m.dL_GTP_TunnelEndpointPresent)
        {
            p_uecc_ho_src_drb_admitted_list->
                erab_ho_src_drb_admitted_info[erab_idx]
                ->forward_direction = UECC_FORWARDING_DIRECTION_DL;
            p_uecc_ho_src_drb_admitted_list->erab_ho_src_drb_admitted_info
                [erab_idx]->dl_gtp_tunnel_end_point.teid_peer =  
                    p_elem->value.dL_GTP_TunnelEndpoint.gTP_TEID;
            /*SPR_19033_START*/
	        if (RRC_SUCCESS != rrc_copy_transportlayer_address(&p_uecc_ho_src_drb_admitted_list->
                erab_ho_src_drb_admitted_info[erab_idx]->dl_gtp_tunnel_end_point.transportLayerAddress,
                &p_elem->value.dL_GTP_TunnelEndpoint. 
                transportLayerAddress,
                p_ue_context))
             {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
			        p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                        "[%s]mem copy of transport layer address failed  !!",__FUNCTION__);
                break;
             }
            /*SPR_19033_START*/

            tunnel_info_present = RRC_TRUE;
        }
        if(p_elem->value.m.uL_GTP_TunnelEndpointPresent)
        {
            if (p_elem->value.m.dL_GTP_TunnelEndpointPresent)
            {
                p_uecc_ho_src_drb_admitted_list->erab_ho_src_drb_admitted_info
                    [erab_idx]->forward_direction = 
                    UECC_FORWARDING_DIRECTION_BOTH; 
            }
            else
            {
                p_uecc_ho_src_drb_admitted_list->erab_ho_src_drb_admitted_info
                    [erab_idx]->forward_direction = UECC_FORWARDING_DIRECTION_UL;
            }

            p_uecc_ho_src_drb_admitted_list->erab_ho_src_drb_admitted_info
                [erab_idx]->ul_gtp_tunnel_end_point.teid_peer = 
                p_elem->value.uL_GTP_TunnelEndpoint.gTP_TEID;

            /*SPR_19033_START*/
	        if (RRC_SUCCESS != rrc_copy_transportlayer_address(&p_uecc_ho_src_drb_admitted_list->
                erab_ho_src_drb_admitted_info[erab_idx]->ul_gtp_tunnel_end_point.transportLayerAddress,
                &p_elem->value.uL_GTP_TunnelEndpoint. 
                transportLayerAddress,
                p_ue_context))
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
			        p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                    "[%s]mem copy of transport layer address failed  !!",__FUNCTION__);
                break;
            }

            /*SPR_19033_START*/
            /*InterCell_Ho_Fix Start*/
            /*InterCell_Ho_Fix Stop*/
        }
        else if( RRC_FALSE == tunnel_info_present ) 
        {
            p_uecc_ho_src_drb_admitted_list->erab_ho_src_drb_admitted_info
                    [erab_idx]->forward_direction = UECC_FORWARDING_DIRECTION_ABSENT;
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_INFO,
                    "The e_RAB_ID[%i] does not contain tunnel info",
                    erab_idx);
        }
    }
    if(RRC_FAILURE == ret )
    {
        /* releasing the allocated memory */
        for( erab_idx = 0; erab_idx < MAX_ERAB_COUNT; erab_idx++)
        {
            if(p_uecc_ho_src_drb_admitted_list->erab_ho_src_drb_admitted_info[erab_idx])
            {
                rrc_mem_free(p_uecc_ho_src_drb_admitted_list->
                        erab_ho_src_drb_admitted_info[erab_idx]);
                p_uecc_ho_src_drb_admitted_list->erab_ho_src_drb_admitted_info
                    [erab_idx] = PNULL;
            }
        }
        /* setting the count as zero */
        p_uecc_ho_src_drb_admitted_list->count = RRC_NULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return ret;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_fsm_fill_erab_not_admitted_list 
 *   INPUT        : uecc_ue_context_t* p_ue_context
 *                  x2ap_E_RAB_List *p_erab_not_admitted_list
 *   OUTPUT       : None               
 *   RETURNS      : RRC_SUCCESS/RRC_PARTIAL_SUCCESS/RRC_FAILURE
 *   DESCRIPTION  : This function fill the  the erab_not_admitted_list 
 *******************************************************************************/
static rrc_return_et uecc_fsm_fill_erab_not_admitted_list(
        uecc_ue_context_t* p_ue_context,
        x2ap_E_RAB_List *p_erab_not_admitted_list
        )
{
    OSRTDListNode* p_node = PNULL;
    x2ap_E_RAB_List_element *p_elem = PNULL;
    rrc_return_et ret = RRC_SUCCESS;
    uecc_e_rab_not_admitted_to_add_list_t 
            * p_uecc_e_rab_not_admitted_to_add_list = PNULL;


    RRC_ASSERT(PNULL!=p_ue_context);
    RRC_ASSERT(PNULL!=p_erab_not_admitted_list);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_uecc_e_rab_not_admitted_to_add_list = &(p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.erab_not_admitted_list);

    for (p_node = p_erab_not_admitted_list->head, 
            p_uecc_e_rab_not_admitted_to_add_list->count = 0; PNULL!=p_node;
            p_node=p_node->next, p_uecc_e_rab_not_admitted_to_add_list->count++ )
    {
        p_elem = (x2ap_E_RAB_List_element*)p_node->data;

        RRC_ASSERT(PNULL!=p_elem);
        RRC_ASSERT(PNULL!=p_elem->value.u._x2ap_E_RAB_ItemIEs_1);

        if (ASN1V_x2ap_id_E_RAB_Item!=p_elem->id)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                    "ASN1V_x2ap_id_E_RAB_Item! =p_elem"
                    "->id (%i)",
                    p_elem->id);
            /* filling the cause in p_ue_context */
            p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
            ret = RRC_FAILURE;
            break;
        }
        /* filling erab ID */
        p_uecc_e_rab_not_admitted_to_add_list->e_rab_not_admitted_to_add_list
            [p_uecc_e_rab_not_admitted_to_add_list->count].e_rab_id =
            p_elem->value.u._x2ap_E_RAB_ItemIEs_1->e_RAB_ID;

        /* filling cause */
        p_uecc_e_rab_not_admitted_to_add_list->e_rab_not_admitted_to_add_list
            [p_uecc_e_rab_not_admitted_to_add_list->count].cause =
            p_elem->value.u._x2ap_E_RAB_ItemIEs_1->cause;
    }
    if(p_uecc_e_rab_not_admitted_to_add_list->count)
    {
        p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.bitmask |=
            X2AP_ERAB_NOT_ADMITTED_LIST_PRESENT;
    }
 
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return ret;
}

/*x2ap-end*/

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_last
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Fail action.
 ****************************************************************************/
static
void uecc_fsm_src_ho_fail_action_last(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO, "FAILURE: END.");

    /*SPR_16618_START*/
    /* SPR 16750 Fix Start */
    /* SPR 19923 Fix Start */
    if ((CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover) ||
		    (((p_ue_context->ho_info.x2ap_cause.t == T_x2ap_Cause_radioNetwork) &&
		      (p_ue_context->ho_info.x2ap_cause.u.radioNetwork == x2ap_tx2relocoverall_expiry)) ||
		     ((p_ue_context->ho_info.cause.t == T_s1ap_Cause_radioNetwork) &&
		      (p_ue_context->ho_info.cause.u.radioNetwork == s1ap_tS1relocoverall_expiry))))
    /* SPR 19923 Fix Stop */
    {
        uecc_fsm_finish_activity_failure(
		    p_ue_context, (void*)UECC_EV_SRC_HO_FAILURE_INT);
    }
    else
    {
        uecc_fsm_finish_activity_failure(
		    p_ue_context, (void*)UECC_EV_SRC_HO_SUCCESS_INT);
    }
    /* SPR 16750 Fix Stop */
    /*SPR_16618_END*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_last1
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Fail action.
 ****************************************************************************/
static 
void uecc_fsm_src_ho_fail_action_last1(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO, "FAILURE: END.");

    /*uecc_fsm_finish_activity_failure(
      p_ue_context, "UECC_EV_SRC_HO_FAILURE_INT"); */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_post_event1
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Fail action.
 ****************************************************************************/
static 
void uecc_fsm_src_ho_fail_action_post_event1(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO, "POSTING EVENT TO MAIN FSM.");

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_SRC_HO_REL_REQ_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_post_event2
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Fail action.
 ****************************************************************************/
static 
void uecc_fsm_src_ho_fail_action_post_event2(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO, "POSTING EVENT TO MAIN FSM.");

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_SRC_HO_RE_EST_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_post_event3
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Fail action.
 ****************************************************************************/
static 
void uecc_fsm_src_ho_fail_action_post_event3(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO, "POSTING EVENT TO MAIN FSM.");

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_SRC_HO_CSC_DELETE_ALL_UE_REQ_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_post_event4
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Fail action.
 ****************************************************************************/
static 
void uecc_fsm_src_ho_fail_action_post_event4(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO, "POSTING EVENT TO MAIN FSM.");

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_SRC_HO_CTX_REL_CMD_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_post_event5
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Fail action.
 ****************************************************************************/
static 
void uecc_fsm_src_ho_fail_action_post_event5(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO, "POSTING EVENT TO MAIN FSM.");

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_SRC_HO_RESET_REQ_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_post_event6
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Fail action.
 ****************************************************************************/
static 
void uecc_fsm_src_ho_fail_action_post_event6(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO, "POSTING EVENT TO MAIN FSM.");

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_SRC_HO_RADIO_LINK_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/*SPR_19066_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_get_ho_prep_info_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_HO_PREP_TIMER_EXPIRY
 ****************************************************************************/
static 
void uecc_fsm_src_ho_get_ho_prep_info_ongoing_timer_expiry_handler(
/*SPR_19066_END*/
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

   RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);
    /*BUG 604 changes start*/
  /*  if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    { 
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"invalid event received");
        return;
    }*/
    /*BUG 604 changes stop*/
    
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "HO Preparation Time out. " 
            "Please check the LLIM Guard timer configured at LLIM. The timer value " 
            "might exceed the HO Prep Timer value");

    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_trelocprep_expiry;
    }
    else
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_tS1relocprep_expiry;
    }
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type2_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_prep_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static 
void uecc_fsm_src_ho_prep_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /*srvcc-u start*/
    /*if (((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
        (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) &&
        (HO_REQD_SRVCC_HO_IND_PRESENT & p_ho_reqd->bitmask)) */
    /*srvcc-u stop*/
    /*BUG 604 changes start*/
    if ((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
        (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
    /*BUG 604 changes stop*/
    {
        /* Build and Send HO REQUEST to X2AP */
        if (RRC_SUCCESS != uecc_x2ap_build_and_send_handover_request(p_ue_context))
        {
            /* Stopping X2 HO PREP Timer */
            /*x2_ho_timer*/
            /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);*/
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type2_actions);
        }
        else
        {   
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
                if(HANDOVER_TYPE_INTRA_LTE_X2 == p_ue_context->ho_info.p_ho_info->
                        p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
                {
                    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[p_ue_context->
                            p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].
                            rrc_handover_related_stats.inter_enb_ho_stats.
                            ho_InterEnbOutPrepAtt,
                            RRC_NULL ,DUMMY_RANGE_CHECK_INDEX );
                }
            }
        }
    }
    else
    {
	if (RRC_TRUE != p_ue_context->ho_info.cdma_ho_ongoing)
	{
        /* Build and sent the HO Required to S1AP */
        if (RRC_SUCCESS != uecc_s1ap_build_and_send_handover_required(p_ue_context))
        {
            /* stopping HO Prep Timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type2_actions);
        }
        else
        {
            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
                if(HANDOVER_TYPE_INTRA_LTE_S1 == p_ue_context->ho_info.p_ho_info->
                        p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
                {
                    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[p_ue_context->
                            p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].
                            rrc_handover_related_stats.inter_enb_ho_stats.
                            ho_InterEnbOutPrepAtt,
                            RRC_NULL, DUMMY_RANGE_CHECK_INDEX);
                }
            }
        }
	}
    } 

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_prep_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static 
void uecc_fsm_src_ho_prep_ongoing_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    rrc_rrm_ho_required_t *p_ho_reqd = PNULL;
    U8 result = RRC_FALSE;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    p_ho_reqd = p_ue_context->ho_info.p_ho_info->p_src_ho_info->
            s1_src_ho_info.p_rrc_rrm_ho_required;

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "HO Preparation Time out.");

    if (RRC_TRUE == p_ue_context->ho_info.cdma_ho_ongoing)
    {
        /*If 2 DL Tunneling are expected AND none is received OR
        *If 1 DL Tunneling are expected AND none is received */
            /* Pack HO_FAILURE message to RRM*/
            /*2 DL msg - fill cdma2000_ho_status = CDMA2000_HO_FAILURE at both indexes*/
            /*1 DL msg - fill cdma2000_ho_status = CDMA2000_HO_FAILURE at appropriate indexes
              and CDMA2000_HO_NO_REPORT is at other index*/
        
        if ((HANDOVER_TYPE_LTE_TO_CDMA2000_1XRTT == p_ho_reqd->ho_type) && 
           (RRC_FALSE == p_ue_context->cdma2000_info.w_f_1xrtt_msg))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "HO Preparation Time out."
                "for 1XRTT HO");
            result = RRC_TRUE;
        }
        if ((HANDOVER_TYPE_LTE_TO_CDMA2000_HRPD == p_ho_reqd->ho_type) && 
           (RRC_FALSE == p_ue_context->cdma2000_info.w_f_hrpd_msg))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "HO Preparation Time out."
                "for HRPD HO");
            result = RRC_TRUE;
        }
        if ((HANDOVER_TYPE_LTE_TO_CDMA2000_CONC_1XRTT_HRPD == p_ho_reqd->ho_type) && 
           (RRC_FALSE == p_ue_context->cdma2000_info.w_f_1xrtt_msg) &&
           (RRC_FALSE == p_ue_context->cdma2000_info.w_f_hrpd_msg))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "HO Preparation Time out."
                "for HRPD HO and 1XRTT BOTH");
            result = RRC_TRUE;
        }
        else
        {
            if ((HANDOVER_TYPE_LTE_TO_CDMA2000_CONC_1XRTT_HRPD == p_ho_reqd->ho_type) &&
                    (RRC_FALSE == p_ue_context->cdma2000_info.w_f_1xrtt_msg))
            {
                if (RRC_TRUE == p_ue_context->cdma2000_info.w_f_hrpd_msg)
                {
                    p_ue_context->cdma2000_info.timer_expired_f_1xrtt = RRC_TRUE;
                    uecc_fsm_process_event(
                            p_ue_context,
                            UECC_EV_S1AP_DL_CDMA2000_TUNNELING_HO_ONGOING,
                            PNULL);
                }
            }
            if ((HANDOVER_TYPE_LTE_TO_CDMA2000_CONC_1XRTT_HRPD == p_ho_reqd->ho_type) &&
                    (RRC_FALSE == p_ue_context->cdma2000_info.w_f_hrpd_msg))
            {
                if (RRC_TRUE == p_ue_context->cdma2000_info.w_f_1xrtt_msg)
                {
                    p_ue_context->cdma2000_info.timer_expired_f_hrpd = RRC_TRUE;
                    uecc_fsm_process_event(
                            p_ue_context,
                            UECC_EV_S1AP_DL_CDMA2000_TUNNELING_HO_ONGOING,
                            PNULL);
                }
            }
        }
        /*If 2 DL Tunneling are expected AND only one is received */
        {
            /* Pack rrc_rrm_ue_ho_command_request_t message */
            /*for DL msg recd - fill cdma2000_ho_status = CDMA2000_HO_SUCCESS at appropriate indexes*/
            /*for DL msg not recd - fill cdma2000_ho_status = CDMA2000_HO_FAILURE at appropriate indexes*/
            
            //send ho_cmd_request message to RRM
            //received ho_cmd_response message to RRM -- go to other place
        }
        if (RRC_TRUE == result)
        { 
            /* Continue failure processing */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type2_actions);
        }
        //send the packed message to RRM and wait for RRC_RRM_UE_HO_COMMAND_RESPONSE_T
    }
    else
    {
    /*BUG 604 changes start*/
        if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
        {
            /* filling cause value */
            p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_trelocprep_expiry;

            /* Build and send the HO Cancel to X2AP */    
            if (RRC_SUCCESS != uecc_x2ap_build_and_send_handover_cancel(p_ue_context))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR, "Fail to send HO Cancel "
                        " to X2AP.");
            }
        }
        else 
        {
            /* filling cause value */
            p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_ue_context->ho_info.cause.u.radioNetwork = s1ap_tS1relocprep_expiry;

            /* Build and sent the HO Cancel to S1AP */    
            if (RRC_SUCCESS != uecc_s1ap_build_and_send_handover_cancel(p_ue_context))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR, "Fail to send HO Cancel " 
                        "Request to S1AP.");
            }
        }
        /* Continue failure processing */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type2_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_s1ap_ho_cmd_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                    void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       uecc_fsm_src_ho_s1ap_ho_cmd_resp_handler from a S1AP.
 ****************************************************************************/
static
void uecc_fsm_src_ho_s1ap_ho_cmd_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    rrc_s1ap_handover_command_t *
        p_rrc_s1ap_handover_command = PNULL;
    uecc_s1_src_ho_info_t *p_s1_src_ho_info = PNULL;
    OSOCTET* p_data = PNULL;
    

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    do{

        /* stopping HO Prep Timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
        /* starting HO Overall Timer */

        if (  RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context,
                    UECC_UE_HO_OVERALL_TIMER, 
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type2_actions);
            break;
        }
        /* storing the handover command received */
        p_rrc_s1ap_handover_command = (rrc_s1ap_handover_command_t*)p_api_data;

        /* in case of any validation required */
        if ( RRC_SUCCESS != uecc_fsm_valiate_s1ap_ho_command_msg (
                    p_ue_context,
                    p_rrc_s1ap_handover_command) )
        {
            /* Process failure according to selected failure actions list */
            /* stopping HO overall Timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);
                    
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type25_actions);
            break;
        }
        p_s1_src_ho_info = &(p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info);

        if( p_rrc_s1ap_handover_command->bitmask &
                RRC_S1AP_HANDOVER_COMMAND_E_RAB_FAILED_LIST_PRESENT)
        {
            if ( RRC_SUCCESS != uecc_fsm_fill_erab_fail_list(p_ue_context, 
                        &p_rrc_s1ap_handover_command->erab_failed_list  ))
            {
                /* filling the cause in p_ue_context */
                p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;

                /* Process failure according to selected failure actions list */
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type3_actions);
                break;
            }
        }

        if(HANDOVER_TYPE_INTRA_LTE_S1 == 
           p_s1_src_ho_info->p_rrc_rrm_ho_required->ho_type) 
        {
            if( p_rrc_s1ap_handover_command->bitmask
                    & RRC_S1AP_HANDOVER_COMMAND_TARGET_TO_SOURCE_TRANSPARENT_CONTAINER_SECONDARY_PRESENT )
            {
                /* filling the cause in p_ue_context */
                p_ue_context->ho_info.cause.t = T_s1ap_Cause_protocol;
                p_ue_context->ho_info.cause.u.protocol = s1ap_semantic_error;

                /* Process failure according to selected failure actions list */
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type3_actions);
                break;
            }
            /* storing the ho_command */
            if( p_rrc_s1ap_handover_command->
                    target_to_source_transparent_container.bitmask
                    & RRC_S1AP_TARGET_TO_SOURCE_TRANSPARENT_CONTAINER_HO_COMMAND_PRESENT )
            {
                if (T_HandoverCommand_criticalExtensions_c1 
                        == p_rrc_s1ap_handover_command->target_to_source_transparent_container.ho_command.criticalExtensions.t)
                {
                    if (T_HandoverCommand_criticalExtensions_c1_handoverCommand_r8 
                            == p_rrc_s1ap_handover_command->target_to_source_transparent_container.ho_command.criticalExtensions.u.c1->t)
                    {
                        if( PNULL == (p_s1_src_ho_info->p_ho_command = 
                                    (U8*)rrc_mem_get(p_rrc_s1ap_handover_command->
                                        target_to_source_transparent_container.
                                        ho_command.criticalExtensions.u.c1->
                                        u.handoverCommand_r8->
                                        handoverCommandMessage.numocts)))
                        {
                            RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL, 
                                    "Memory allocation failure.");
                            break;
                        }
                        /* filling ho_cmd_length  */
                        p_s1_src_ho_info->ho_cmd_length = (U8)(p_rrc_s1ap_handover_command->
                                target_to_source_transparent_container.
                                ho_command.criticalExtensions.u.c1->
                                u.handoverCommand_r8->
                                handoverCommandMessage.numocts);
                        /*storing ho_command received */
                        l3_memcpy_wrapper(p_s1_src_ho_info->p_ho_command,
                                p_rrc_s1ap_handover_command->
                                target_to_source_transparent_container.
                                ho_command.criticalExtensions.u.c1->u.
                                handoverCommand_r8->handoverCommandMessage.data,
                                p_s1_src_ho_info->ho_cmd_length);

                        /* moving to next child state */
                        UECC_FSM_SET_CHILD_STATE(p_ue_context,
                                &uecc_fsm_src_ho_cmd_ongoing, PNULL);
                        break;
                    }
                }
            }
            /* if the upper condition were false then it means ho_command was received 
               invalid so need to exit with failure  */
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR, 
                    "Does not received the HO Command");
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type3_actions);
            break;
        }

        else if((HANDOVER_TYPE_LTE_TO_UTRAN == p_s1_src_ho_info->\
                    p_rrc_rrm_ho_required->ho_type) || \
                (HANDOVER_TYPE_LTE_TO_GERAN == p_s1_src_ho_info->\
                 p_rrc_rrm_ho_required->ho_type))
        {
            /* storing the required information into Ue Context */
            if(RRC_S1AP_HANDOVER_COMMAND_NAS_SECURITY_PARAMETERS_FROM_E_UTRAN_PRESENT &\
                    p_rrc_s1ap_handover_command->bitmask)
            {
                if( PNULL == (p_s1_src_ho_info->inter_rat_ho_info.\
                            p_nas_security_param_from_E_UTRAN_data = 
                            rrc_mem_get(sizeof(U8)*NAS_SECURITY_PARAM_INTER_RAT_HO_LEN)))
                {
                    RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL, 
                            "Memory allocation failure.");
                    break;
                } 

        		/* CSR 101813 Fix Start */
                if (p_rrc_s1ap_handover_command->nas_security_param_from_E_UTRAN.
                        numocts != NAS_SECURITY_PARAM_INTER_RAT_HO_LEN )
                {
                    RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                            "NAS SECUTIRY PARAM length should be equal to 1 "
                            "in case of Inter-RAT HO");
		        /* CSR 101813 Fix Stop */
                    /* Fill Cause */
                    p_ue_context->ho_info.cause.t = T_s1ap_Cause_protocol; 
                    p_ue_context->ho_info.cause.u.protocol = s1ap_abstract_syntax_error_falsely_constructed_message;

                    uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type3_actions);
                    break;
                }

                /* storing nas security numocts */
                p_s1_src_ho_info->inter_rat_ho_info.\
                    nas_security_param_from_E_UTRAN_numocts = NAS_SECURITY_PARAM_INTER_RAT_HO_LEN;


                /* storing nas security data */     
        		/* CSR 101813 Fix Start */ 
                l3_memcpy_wrapper(p_s1_src_ho_info->inter_rat_ho_info.\
                        p_nas_security_param_from_E_UTRAN_data,
                        &(p_rrc_s1ap_handover_command->\
                        nas_security_param_from_E_UTRAN.data[0]),/* CSR 101813 Fix Stop */
                        p_s1_src_ho_info->inter_rat_ho_info.\
                        nas_security_param_from_E_UTRAN_numocts);
            }
            if(RRC_S1AP_HANDOVER_COMMAND_TARGET_TO_SOURCE_TRANSPARENT_CONTAINER_SECONDARY_PRESENT &\
                    p_rrc_s1ap_handover_command->bitmask)
            {
                if((HANDOVER_TYPE_LTE_TO_UTRAN == p_s1_src_ho_info->\
                            p_rrc_rrm_ho_required->ho_type) || 
                  ((RRC_RRM_UE_HO_ADM_REQ_SRVCC_HO_INDICATION_PRESENT & 
                        p_s1_src_ho_info->p_rrc_rrm_ho_required->bitmask) &&
                        CS_ONLY_TYPE == p_s1_src_ho_info->p_rrc_rrm_ho_required->srvcc_ho_indication))
                {
                    /* filling the cause in p_ue_context */
                    p_ue_context->ho_info.cause.t = T_s1ap_Cause_protocol;
                    p_ue_context->ho_info.cause.u.protocol = s1ap_semantic_error;

                    /* Process failure according to selected failure actions list */
                    uecc_fsm_start_failure_processing(
                            p_ue_context,
                            uecc_fsm_src_ho_fail_type3_actions);
                    break;
                }

                if( PNULL == (p_s1_src_ho_info->inter_rat_ho_info.
                            p_target_to_source_transparent_container_secondary_data = \
                            rrc_mem_get(p_rrc_s1ap_handover_command->\
                                target_to_source_transparent_container_secondary.numocts)))
                {
                    RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL, 
                            "Memory allocation failure.");
                    break;
                } 

                /* Storing secondary container numocts */
                p_s1_src_ho_info->inter_rat_ho_info.\
                    target_to_source_transparent_container_secondary_numocts=\
                    p_rrc_s1ap_handover_command->\
                    target_to_source_transparent_container_secondary.numocts;

                /* Storing secondary container data */
                l3_memcpy_wrapper(p_s1_src_ho_info->inter_rat_ho_info.
                        p_target_to_source_transparent_container_secondary_data,
                        p_rrc_s1ap_handover_command->\
                        target_to_source_transparent_container_secondary.data,
                        p_s1_src_ho_info->inter_rat_ho_info.
                        target_to_source_transparent_container_secondary_numocts);

                p_s1_src_ho_info->inter_rat_ho_info.bitmask =\
                        TARGET_TO_SOURCE_TRANSPARNET_CONTAINER_SECONDARY_PRESENT;
            }

            if(RRC_S1AP_TARGET_TO_SOURCE_TRANSPARENT_CONTAINER_FOR_GERAN_PRESENT &\
                    p_rrc_s1ap_handover_command->target_to_source_transparent_container.\
                    bitmask)
            {
                if( PNULL == (p_s1_src_ho_info->inter_rat_ho_info.
                            p_target_to_source_transparent_container_data= 
                            rrc_mem_get(p_rrc_s1ap_handover_command->\
                                target_to_source_transparent_container.\
                                target_to_source_transparent_container_for_geran.numocts)))
                {
                    RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL, 
                            "Memory allocation failure.");
                    break;
                } 

                /* Storing Primary container numocts */
                p_s1_src_ho_info->inter_rat_ho_info.\
                    target_to_source_transparent_container_numocts =\
                    p_rrc_s1ap_handover_command->target_to_source_transparent_container.\
                    target_to_source_transparent_container_for_geran.numocts;

                /* Storing Primary Container data */
                l3_memcpy_wrapper(p_s1_src_ho_info->inter_rat_ho_info.\
                        p_target_to_source_transparent_container_data,
                        p_rrc_s1ap_handover_command->\
                        target_to_source_transparent_container.\
                        target_to_source_transparent_container_for_geran.data,
                        p_s1_src_ho_info->inter_rat_ho_info.
                        target_to_source_transparent_container_numocts);
            }

            else if(RRC_S1AP_TARGET_RNC_TO_SOURCE_RNC_TRANSPARENT_CONTAINER_PRESENT &\
                    p_rrc_s1ap_handover_command->target_to_source_transparent_container.\
                    bitmask)
            {
                /* Storing Primary container numocts */	/* CSR 101813 Fix Start */
                p_s1_src_ho_info->inter_rat_ho_info.\
                    target_to_source_transparent_container_numocts =
                p_rrc_s1ap_handover_command->target_to_source_transparent_container.\
                    targetRNC_ToSourceRNC_TransparentContainer.rRC_Container.numocts;
                p_data = (U8*)rrc_mem_get(p_s1_src_ho_info->inter_rat_ho_info.\
                        target_to_source_transparent_container_numocts);	/* CSR 101813 Fix Stop */
                if(PNULL == p_data)
                {
                    RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL,"Memory allocation failure.");
                    break;
                }    
                /* Storing Primary container data */
        		/* CSR 101813 Fix Start */
                l3_memcpy_wrapper(p_data,  
                    p_rrc_s1ap_handover_command->
                    target_to_source_transparent_container.\
                    targetRNC_ToSourceRNC_TransparentContainer.rRC_Container.data,
                    p_s1_src_ho_info->inter_rat_ho_info.\
                    target_to_source_transparent_container_numocts);/* CSR 101813 Fix Stop */
                p_s1_src_ho_info->inter_rat_ho_info.\
                    p_target_to_source_transparent_container_data = p_data;
            }
            else
            {
                /* if the upper condition were false then it means primary 
                 * container was received invalid so need to exit with failure  */
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL, 
                        "Does not received the Primary Container");
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type3_actions);
                break;

            }
            /* moving to next child state */
            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                    &uecc_fsm_src_ho_cmd_ongoing, PNULL);
            break;
        }
    }while(0);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return;
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_x2ap_ho_req_ack_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
                    uecc_fsm_src_ho_x2ap_ho_req_ack_resp_handler from a X2AP.
 ****************************************************************************/
static
void uecc_fsm_src_ho_x2ap_ho_req_ack_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    rrc_x2ap_handover_req_ack_t *
        p_rrc_x2ap_handover_req_ack = PNULL; 
    uecc_s1_src_ho_info_t*     p_s1_src_ho_info = PNULL;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    do{
        /* stopping HO Prep Timer */
        /*x2_ho_timer*/
        /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);*/
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);

        /* starting HO Overall Timer */

        /*x2_ho_timer*/
        if (  RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context,
                    UECC_UE_X2_HO_OVERALL_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type2_actions);
            break;
        }
        /* storing the handover command received */
        p_rrc_x2ap_handover_req_ack = (rrc_x2ap_handover_req_ack_t*)p_api_data;      

        p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present = 1;
        p_ue_context->x2ap_context.new_enb_ue_x2ap_id
            =  p_rrc_x2ap_handover_req_ack->new_ue_x2ap_id;

        /* in case of any validation required */
        if ( RRC_TRUE != uecc_fsm_validate_x2ap_ho_req_ack_msg (
                    p_ue_context,
                    p_rrc_x2ap_handover_req_ack) )
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "Validation of X2AP_HO_REQ message fail.");
            /* filling the cause in p_ue_context */
            p_ue_context->ho_info.cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->ho_info.cause.u.radioNetwork = x2ap_unspecified_2;
            /* Process failure according to selected failure actions list */
            /* stopping HO overall Timer */
            /*x2_ho_timer*/
            /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);*/
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);

            if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
            {
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type25_actions);
            }
    /*BUG 604 changes start*/
            else if((INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
                    (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes start*/
            {
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type2_actions);
            }
            break;
        }

        p_s1_src_ho_info = &(p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info);

        if( p_rrc_x2ap_handover_req_ack->bitmask &
                X2AP_HANDOVER_REQ_ACK_ERAB_NOT_ADMITTED_LIST_PRESENT)
        {
            if ( RRC_SUCCESS != uecc_fsm_fill_erab_not_admitted_list(p_ue_context,
                        &p_rrc_x2ap_handover_req_ack->e_rab_not_admitted_list  ))
            {
                /* filling the cause in p_ue_context */
                p_ue_context->ho_info.cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->ho_info.cause.u.radioNetwork = x2ap_unspecified_2;

                /* Process failure according to selected failure actions list */
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type3_actions);
                break;
            }
        }
        if (T_HandoverCommand_criticalExtensions_c1 == 
                p_rrc_x2ap_handover_req_ack->
                target_to_src_enb_transparent_cont.
                ho_command.criticalExtensions.t)
        {
            if (T_HandoverCommand_criticalExtensions_c1_handoverCommand_r8
                    == p_rrc_x2ap_handover_req_ack->
                    target_to_src_enb_transparent_cont.
                    ho_command.criticalExtensions.u.c1->t)
            {
                if( PNULL == (p_s1_src_ho_info->p_ho_command =
                            (U8*)rrc_mem_get(p_rrc_x2ap_handover_req_ack->
                                             target_to_src_enb_transparent_cont.
                                             ho_command.criticalExtensions.u.c1->
                                             u.handoverCommand_r8->
                                             handoverCommandMessage.numocts)))
                {
                    RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL,
                            "Memory allocation failure.");
                    break;
                }

                /* filling ho_cmd_length  */
                p_s1_src_ho_info->ho_cmd_length = 
                    (U16)(p_rrc_x2ap_handover_req_ack->
                         target_to_src_enb_transparent_cont.
                         ho_command.criticalExtensions.u.c1->
                         u.handoverCommand_r8->
                         handoverCommandMessage.numocts);

                /*storing ho_command received */
                l3_memcpy_wrapper(p_s1_src_ho_info->p_ho_command,
                        p_rrc_x2ap_handover_req_ack->
                        target_to_src_enb_transparent_cont.
                        ho_command.criticalExtensions.u.c1->u.
                        handoverCommand_r8->handoverCommandMessage.data,
                        p_s1_src_ho_info->ho_cmd_length);

                /* moving to next child state */
                UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_src_ho_cmd_ongoing, PNULL);
                break;
            }
        }
    }while(0);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return;
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_prep_failure_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_S1AP_HO_PREPARATION_FAIL from a S1AP.
 ****************************************************************************/
static void uecc_fsm_src_ho_prep_failure_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    rrc_s1ap_handover_preparation_failure_t*
        p_rrc_s1ap_handover_preparation_failure = PNULL;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* stopping HO Prep Timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);

    p_rrc_s1ap_handover_preparation_failure 
        = (rrc_s1ap_handover_preparation_failure_t*)p_api_data;

    /* filling cause */
    p_ue_context->ho_info.cause = p_rrc_s1ap_handover_preparation_failure
        ->cause;

    if (RRC_S1AP_HANDOVER_PREPARATION_FAILURE_DIAGNOSTICS_PRESENT &
           p_rrc_s1ap_handover_preparation_failure->bitmask)
    {
        log_uecc_criticality_diagostics(p_ue_context->p_gb_context,
          &p_rrc_s1ap_handover_preparation_failure->criticality_diagnostics);
    }
    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type2_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_x2ap_ho_prep_failure_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_S1AP_HO_PREPARATION_FAIL from a X2AP.
 ****************************************************************************/

static void uecc_fsm_src_x2ap_ho_prep_failure_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    rrc_x2ap_handover_prep_fail_t*
        p_rrc_x2ap_handover_preparation_failure = PNULL;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*x2_ho_timer*/
    /* stopping HO Prep Timer */
    /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);*/
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);

    p_rrc_x2ap_handover_preparation_failure
        = (rrc_x2ap_handover_prep_fail_t*)p_api_data;

    /* filling cause */
    p_ue_context->ho_info.x2ap_cause = p_rrc_x2ap_handover_preparation_failure->
                                  cause;

    /* SPR 16791 Fix Start */
    if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
    {
        /* Send Release Ind to X2AP Module */
        uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
    }

    /*SPR_16618_START*/
    /* Process failure according to selected failure actions list */
    /*uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type3_actions);*/
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type2_actions);
    /*SPR_16618_END*/
    /* SPR 16791 Fix Stop */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_cmd_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_cmd_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and send handover command to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_handover_command(p_ue_context))
    {
	    /*SPR_16778_START*/
	    if ((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
			    (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
			    (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
	    {
		    p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
		    p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
	    }
	    /*SPR_16778_END*/
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type3_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* SPR 16750 Fix Start */
/* Code Removed */
/* SPR 16750 Fix Stop */

/****************************************************************************
 * Function Name  : uecc_fsm_src_x2ap_error_indication_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_X2AP_ERROR_INDICATION from a S1AP.
 ****************************************************************************/
static void uecc_fsm_src_x2ap_error_indication_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    rrc_x2ap_error_indication_t* p_rrc_x2ap_error_indication= PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api_data);

    /*BUG 604 changes start*/
    if((INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    /* stopping HO Prep Timer */
    if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) 
    {
        /*x2_ho_timer*/
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
        /* SPR 16750 Fix Start */
        /* Send Release Ind to X2AP Module */
        uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
        /* SPR 16750 Fix Stop */
    }

    p_rrc_x2ap_error_indication
        = (rrc_x2ap_error_indication_t*)p_api_data;
    /* filling cause */
    p_ue_context->ho_info.x2ap_cause = p_rrc_x2ap_error_indication->cause;

    /* Process failure according to selected
     * failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type2_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_cmd_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_cmd_ongoing_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*BUG 604 changes start*/
    /*BUG 604 changes stop*/
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "HO Overall Time out.");
        /* filling the cause in p_ue_context */
    /*BUG 604 changes start*/
        if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
        {
            p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_tx2relocoverall_expiry;
            /*SPR_16618_START*/
            /* Send Release Ind to X2AP Module */
            /*if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
            {
                uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
            }*/
            /*SPR_16618_END*/
        }
        else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
            p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_ue_context->ho_info.cause.u.radioNetwork = s1ap_tS1relocoverall_expiry;
        }
/* BUG: 7882 Start */
        /*SPR_16618_START*/
        /* Continue failure processing */
        /*uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type1_actions);*/
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type3_actions);
	/*SPR_16618_END*/
       
/* BUG: 7882 End */
    /*BUG 604 changes stop*/
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_rrm_ho_cmd_resp_handler_s1ap_ho_hndlr
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *		    rrm_ue_ho_command_response_t* - pointer to rrc_rrm_ue_ho_command_response
 * Outputs        : None
 * Returns        : RRC_SUCCESS
 *		    RRC_FAILURE
 * Description    : Inter RAT handover handler
 *                       
 ****************************************************************************/
static rrc_return_t uecc_fsm_src_ho_rrm_ho_cmd_resp_handler_s1ap_ho_hndlr(
        uecc_ue_context_t *p_ue_context,
	rrc_rrm_ue_ho_command_response_t *p_rrc_rrm_ue_ho_command_response
        )
{
	U8  erab_id = 0, erab_idx = 0;
	U8 erab_id_match = RRC_FALSE;
	uecc_ho_src_drb_forward_info_t  *srb_drb_fwd_info = PNULL;
	uecc_s1_src_ho_info_t *s1_src_ho_info = PNULL;
	rrc_erab_info_list_t *erab_item = PNULL;
	rrc_bool_et is_erb_fwd_list_valid = RRC_FALSE;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
	if(HO_RESPONSE_CARRIER_FREQ_CDMA2000_PRESENT & p_rrc_rrm_ue_ho_command_response->ho_command_response.bitmask)
	{
		if (RRC_TRUE == p_ue_context->ho_info.cdma_ho_ongoing)
		{
			p_ue_context->cdma2000_info.redirection_present_f_hrpd = RRC_TRUE;
			p_ue_context->cdma2000_info.carrier_freq_cdma2000 =
				p_rrc_rrm_ue_ho_command_response->ho_command_response.carrier_freq_cdma2000;
		}
	}

	if ((HANDOVER_TYPE_LTE_TO_UTRAN == p_ue_context->ho_info.p_ho_info->\
				p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)||
			(HANDOVER_TYPE_LTE_TO_GERAN == p_ue_context->ho_info.p_ho_info->\
			 p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type))
	{
		if(HO_RESPONSE_CS_FALLBACK_INDICATOR_PRESENT & p_rrc_rrm_ue_ho_command_response->\
				ho_command_response.bitmask)
		{
			/*  save cs_fallback_indicator in Ue Context */
			p_ue_context->cs_fallback_indicator = p_rrc_rrm_ue_ho_command_response->\
							      ho_command_response.cs_fallback_indicator;
		}
		else
		{
                        /* CS_fallback_fix start */
			RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
					"[RRC_RRM_HO_CMD_RESP] CS FALLBACK Indicator Missing");
                        /* CS_fallback_fix stop */
		}
	}
	/* updating the ue_context for UL forwarding erabs based on the response
	   received from RRM */
	s1_src_ho_info = &(p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info);
	erab_item =
		&(p_rrc_rrm_ue_ho_command_response->ho_command_response.erab_sub_to_fwd_list);
	if(p_rrc_rrm_ue_ho_command_response->ho_command_response.bitmask &
			ERAB_LIST_PRESENT )
	{
		for(erab_id =0; erab_id < MAX_ERAB_COUNT; erab_id ++)
		{
			if((PNULL !=(srb_drb_fwd_info = s1_src_ho_info->src_drb_fwd_list.

							erab_ho_src_drb_forward_list[erab_id])) &&
					(erab_item->erab_count <= MAX_ERAB_COUNT))
			{
				erab_id_match = RRC_FALSE;
				for( erab_idx = 0; erab_idx < erab_item->erab_count; erab_idx++)
				{
					if(erab_id ==
							erab_item->drb_sub_to_fwd[erab_idx].erab_id)
					{
						erab_id_match = RRC_TRUE;
						break;
					}
				}
				if(RRC_FALSE == erab_id_match)
				{
					RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
							"E_RAB ID NOT MATCHED : E-RAB ID In UE"
							"Context : %u", erab_id);
					/* rleasing the memory for the drb forwarding
					 * info is not come from RRM
					 */
					if (PNULL != srb_drb_fwd_info)
					{
						rrc_mem_free(srb_drb_fwd_info);
						srb_drb_fwd_info = PNULL;
					}
					s1_src_ho_info->src_drb_fwd_list.
						erab_ho_src_drb_forward_list[erab_id]=PNULL;
					s1_src_ho_info->src_drb_fwd_list.count--;
				}
				else /* If erab_id_match is RRC_TRUE */
				{
					/* Check forward direction that came in
					 * HO COM RESP from RRM with stored in context
					 */
					if (((srb_drb_fwd_info->forward_direction ==
									UECC_FORWARDING_DIRECTION_DL) &&
								(erab_item->drb_sub_to_fwd[erab_idx].forward_direction
								 ==
								 UECC_FORWARDING_DIRECTION_UL))
							||
							((srb_drb_fwd_info->forward_direction

							  ==
							  UECC_FORWARDING_DIRECTION_UL)
							 &&
							 (erab_item->drb_sub_to_fwd[erab_idx].forward_direction
							  ==
							  UECC_FORWARDING_DIRECTION_DL))
							||
							((srb_drb_fwd_info->forward_direction
							  !=
							  UECC_FORWARDING_DIRECTION_BOTH)
							 &&
							 (erab_item->drb_sub_to_fwd[erab_idx].forward_direction
							  ==
							  UECC_FORWARDING_DIRECTION_BOTH)))
							  {
								  RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
										  "Wrong Forwarding Direction Combination"
										  "For E-RAB ID = %u",
										  erab_id);

								  rrc_mem_free(srb_drb_fwd_info);
								  srb_drb_fwd_info = PNULL;
								  s1_src_ho_info->src_drb_fwd_list.
									  erab_ho_src_drb_forward_list[erab_id]=PNULL;
							  }
					else
					{
						/* Forward Direction should get copied.*/
						srb_drb_fwd_info->forward_direction =
                        (rrm_forwarding_direction_et)erab_item->drb_sub_to_fwd[erab_idx].forward_direction;
						is_erb_fwd_list_valid = RRC_TRUE;
					}

				}

			}
		}
	}
	else
	{
		RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
				"ERAB INFO LIST is ABSENT. Now Forwarding is deleted");
		for(erab_id =0; erab_id < MAX_ERAB_COUNT; erab_id ++)
		{
			if(PNULL
					!=(srb_drb_fwd_info =
						s1_src_ho_info->src_drb_fwd_list.
						erab_ho_src_drb_forward_list[erab_id]))
			{
				rrc_mem_free(srb_drb_fwd_info);
				srb_drb_fwd_info = PNULL;
				s1_src_ho_info->src_drb_fwd_list.
					erab_ho_src_drb_forward_list[erab_id]=PNULL;
			}
		}
	}
	if (RRC_FALSE == is_erb_fwd_list_valid)
	{
		if(ERAB_SUBJECT_TO_DATA_FORWARDING_LIST_PRESENT &
				p_ue_context->ho_info.p_ho_info->p_src_ho_info->
				s1_src_ho_info.bitmask)
		{
			/* Re-setting the FWD LIST bitmask */
			p_ue_context->ho_info.p_ho_info->p_src_ho_info->
				s1_src_ho_info.bitmask ^= ERAB_SUBJECT_TO_DATA_FORWARDING_LIST_PRESENT;
		}

	}
	/* HO Intergartion fix taken from rel 3.3*/
	/* HO Intergration Fix: HFN_SN_REQ should be sent to PDCP in UM Mode also */
	UECC_FSM_SET_CHILD_STATE(p_ue_context,&uecc_fsm_src_ho_llim_re_est_ongoing, PNULL);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
	return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_rrm_ho_cmd_resp_handler_x2_based_ho_hndlr
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *		    rrm_ue_ho_command_response_t* - pointer to rrc_rrm_ue_ho_command_response
 * Outputs        : None
 * Returns        : RRC_SUCCESS
 *		    RRC_FAILURE
 * Description    : Inter Cell handover handler              
 ****************************************************************************/
static rrc_return_t uecc_fsm_src_ho_rrm_ho_cmd_resp_handler_x2_based_ho_hndlr(
        uecc_ue_context_t *p_ue_context,
	rrc_rrm_ue_ho_command_response_t *p_rrc_rrm_ue_ho_command_response
        )
{

    U8  erab_id = 0, erab_idx = 0;
    U8 erab_id_match = RRC_FALSE;
    uecc_ho_src_drb_admitted_info_t *src_drb_admitted_info = PNULL;
    uecc_s1_src_ho_info_t *s1_src_ho_info = PNULL;
    rrc_erab_info_list_t *erab_item = PNULL;
    rrc_bool_et is_erb_fwd_list_valid = RRC_FALSE;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /* updating the ue_context for UL forwarding erabs based on the response
       received from RRM */
    s1_src_ho_info = &(p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info);
    erab_item = &(p_rrc_rrm_ue_ho_command_response->ho_command_response.erab_sub_to_fwd_list);
    if(p_rrc_rrm_ue_ho_command_response->ho_command_response.bitmask & ERAB_LIST_PRESENT )
    {
        for(erab_id =0; erab_id < MAX_ERAB_COUNT; erab_id ++)
        {
            if ((PNULL !=(src_drb_admitted_info = s1_src_ho_info->src_erab_admitted_list.
                            erab_ho_src_drb_admitted_info[erab_id])) &&
                    (erab_item->erab_count <= MAX_ERAB_COUNT))
            {
                erab_id_match = RRC_FALSE;
                for( erab_idx = 0; (erab_idx < erab_item->erab_count) &&
                        (erab_idx < MAX_ERAB_COUNT); erab_idx++)
                {
                    if(erab_id ==
                            erab_item->drb_sub_to_fwd[erab_idx].erab_id)
                    {
                        erab_id_match = RRC_TRUE;
                        break;
                    }
                }
                if((RRC_FALSE == erab_id_match) &&
                        (UECC_FORWARDING_DIRECTION_ABSENT !=
                         src_drb_admitted_info->forward_direction))
                {
                    RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                            "E_RAB ID NOT MATCHED : E-RAB ID In UE"
                            "Context : %u", erab_id);
                    /* rleasing the memory for the drb forwarding
                     * info is not come from RRM
                     */
                    if (PNULL != src_drb_admitted_info)
                    {
                        rrc_mem_free(src_drb_admitted_info);
                        src_drb_admitted_info = PNULL;
                    }
                    s1_src_ho_info->src_erab_admitted_list.
                        erab_ho_src_drb_admitted_info[erab_id]=PNULL;
                }
                /*SPR 7044 Start */
                else if ((erab_idx < MAX_ERAB_COUNT) &&
                       (erab_idx < erab_item->erab_count))/* If erab_id_match is RRC_TRUE */
                /*SPR 7044 Stop */
                {
                    /* Check forward direction that came in
                     * HO COM RESP from RRM with stored in context
                     */
                    if (((src_drb_admitted_info->forward_direction ==
                                    UECC_FORWARDING_DIRECTION_DL) &&
                                (erab_item->drb_sub_to_fwd[erab_idx].forward_direction
                                 ==
                                 UECC_FORWARDING_DIRECTION_UL))
                            ||
                            ((src_drb_admitted_info->forward_direction
                              ==
                              UECC_FORWARDING_DIRECTION_UL)
                             &&
                             (erab_item->drb_sub_to_fwd[erab_idx].forward_direction
                              ==
                              UECC_FORWARDING_DIRECTION_DL))
                            ||
                            ((src_drb_admitted_info->forward_direction
                              !=
                              UECC_FORWARDING_DIRECTION_BOTH)
                             &&
                             (erab_item->drb_sub_to_fwd[erab_idx].forward_direction
                              ==
                              UECC_FORWARDING_DIRECTION_BOTH)))
                              {
                                  RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                                          "Wrong Forwarding Direction Combination"
                                          "For E-RAB ID = %u",
                                          erab_id);

                                  rrc_mem_free(src_drb_admitted_info);
                                  src_drb_admitted_info = PNULL;
                                  s1_src_ho_info->src_erab_admitted_list.
                                      erab_ho_src_drb_admitted_info[erab_id]=PNULL;
                              }
                    else
                    {
                        src_drb_admitted_info->forward_direction =
                            (rrm_forwarding_direction_et)erab_item->drb_sub_to_fwd[erab_idx].forward_direction;
                        is_erb_fwd_list_valid = RRC_TRUE;
                    }
                }

            }
        }
    }
    else
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                "ERAB INFO LIST is ABSENT. Now Forwarding is deleted");
        for(erab_id =0; erab_id < MAX_ERAB_COUNT; erab_id ++)
        {
            if(PNULL !=(src_drb_admitted_info =
                        s1_src_ho_info->src_erab_admitted_list.
                        erab_ho_src_drb_admitted_info[erab_id]))
            {
                rrc_mem_free(src_drb_admitted_info);
                src_drb_admitted_info = PNULL;
                s1_src_ho_info->src_erab_admitted_list.
                    erab_ho_src_drb_admitted_info[erab_id]=PNULL;
            }
        }
    }
    if (RRC_FALSE == is_erb_fwd_list_valid)
    {
        if(X2AP_ERAB_ADMITTED_LIST_PRESENT &
                p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                s1_src_ho_info.bitmask)
        {
            /* Re-setting the FWD LIST bitmask */
            p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                s1_src_ho_info.bitmask ^= X2AP_ERAB_ADMITTED_LIST_PRESENT;
        }
    }
    /* HO Intergartion fix taken from rel 3.3*/
    /* HO Intergration Fix: HFN_SN_REQ should be sent to PDCP in UM Mode also */
    UECC_FSM_SET_CHILD_STATE(p_ue_context,&uecc_fsm_src_ho_llim_re_est_ongoing, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS; 
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_rrm_ho_cmd_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       uecc_fsm_src_ho_rrm_ho_cmd_resp_handler from a RRM.
 ****************************************************************************/
static void uecc_fsm_src_ho_rrm_ho_cmd_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    rrc_rrm_ue_ho_command_response_t*    
        p_rrc_rrm_ue_ho_command_response = PNULL;
    rrc_return_t                    result = RRC_FAILURE;
    uecc_fsm_fail_handler_f const*  p_failure_actions_list =  uecc_fsm_src_ho_fail_type3_actions;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_rrm_ue_ho_command_response 
        = (rrc_rrm_ue_ho_command_response_t*)p_api_data;

    switch (p_rrc_rrm_ue_ho_command_response->ho_command_response.response)
    {
        case RRC_SUCCESS:
        {
	  /* lossy ho changes start */
          if( HO_RESPONSE_ERAB_LOSSY_HO_LIST_PRESENT &
                  p_rrc_rrm_ue_ho_command_response->ho_command_response.
                  bitmask)
          {
             uecc_fsm_validate_lossy_ho_list(
                    p_ue_context,
                    &(p_rrc_rrm_ue_ho_command_response->ho_command_response.erab_lossy_ho_list));
          }
	  /* lossy ho changes stop */

            /* Added for inter-Rat HO */
          if ( S1_HO == p_ue_context->ho_info.s1_or_x2_handover )
          {
              result=uecc_fsm_src_ho_rrm_ho_cmd_resp_handler_s1ap_ho_hndlr(
                            p_ue_context,p_rrc_rrm_ue_ho_command_response);
          }
    /*BUG 604 changes start*/
          else if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                    (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
                    (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
          {
	        result=uecc_fsm_src_ho_rrm_ho_cmd_resp_handler_x2_based_ho_hndlr(
                            p_ue_context,p_rrc_rrm_ue_ho_command_response);
          }
          break;
        }
        case RRC_PARTIAL_SUCCESS:
        {
            result = RRC_PARTIAL_SUCCESS;
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "[RRC_RRM_HO_CMD_RESP]  RRC_PARTIAL_SUCCESS Response received");
            /* RRM rejected connection */
            p_failure_actions_list = uecc_fsm_src_ho_fail_type3_actions;
            break;
        }
        case RRC_FAILURE:
        {
            /* RRM rejected connection */
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[RRC_RRM_HO_CMD_RESP]  RRC_FAILURE Response received");

            p_failure_actions_list = uecc_fsm_src_ho_fail_type3_actions;
    /*BUG 604 changes start*/
            if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
               (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
               (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
    /*BUG 604 changes stop*/
            {
              /* updating the cause recevied from RRM */
              p_ue_context->ho_info.x2ap_cause.t = p_rrc_rrm_ue_ho_command_response->
                ho_command_response.rrm_cause.type;
              if(RRC_SUCCESS != uecc_x2ap_build_x2ap_cause(
                          p_ue_context->p_gb_context, 
                          &p_ue_context->ho_info.x2ap_cause,
                    &p_rrc_rrm_ue_ho_command_response->ho_command_response.rrm_cause))
              {
                  RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                          p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                          "[RRC_RRM_RECEIVED_MSG] Invalid cause received");
              }
            }
            else
            {
              /* updating the cause recevied from RRM */
              p_ue_context->ho_info.cause.t = p_rrc_rrm_ue_ho_command_response->
                ho_command_response.rrm_cause.type;
              if(RRC_SUCCESS != uecc_s1ap_build_s1ap_cause(&p_ue_context->ho_info.cause,
                    &p_rrc_rrm_ue_ho_command_response-> ho_command_response.rrm_cause,
                    p_ue_context))
              {
                  RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                          p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                          "[RRC_RRM_RECEIVED_MSG] Invalid cause received");
              }
            }
            break;
        }
        default:
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[RRC_RRM_HO_CMD_RESP] Invalid Response=%i",
                    p_rrc_rrm_ue_ho_command_response->ho_command_response.
                    response);
            break;
    }

    /* Process failure */
    if (RRC_SUCCESS != result)
    {
        /*x2_ho_timer*/
        /*BUG 604 changes start*/
        if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
           (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
           (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
        /*BUG 604 changes stop*/
        {
          /* Stop timer */
          uecc_ue_timer_stop(p_ue_context->p_gb_context,
                  UECC_UE_X2_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data );
        }
        else
        {
          /* Stop timer */
          uecc_ue_timer_stop(p_ue_context->p_gb_context,
                  UECC_UE_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data );
        }
      

        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(p_ue_context,
                p_failure_actions_list);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/* lossy ho changes start */
/******************************************************************************
 *   FUNCTION NAME: uecc_fsm_validate_lossy_ho_list 
 *   INPUT        : uecc_ue_context_t* p_ue_context
 *                  erab_item_list_t *p_erab_lossy_ho_list
 *   OUTPUT       : None               
 *   RETURNS      : None
 *   DESCRIPTION  : This function validate the lossy_ho_list 
 *******************************************************************************/
static void uecc_fsm_validate_lossy_ho_list(
        uecc_ue_context_t* p_ue_context,
        erab_item_list_t *p_erab_lossy_ho_list
        )
{
    U8 drbId = 0;
    U8 erab_id = 0;
    U8 drb_index = 0;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_erab_lossy_ho_list);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (PNULL == p_ue_context->ho_info.p_ho_info) 
    {
        return; 
    }
    if (PNULL == p_ue_context->ho_info.p_ho_info->p_src_ho_info) 
    {
        return; 
    }

    p_ue_context->ho_info.p_ho_info->p_src_ho_info->
               s1_src_ho_info.erab_lossy_ho_list.count = 0;

    for (drbId = 0; drbId < (p_erab_lossy_ho_list->count); drbId++)
    {
        erab_id = (U8)p_erab_lossy_ho_list->erab_id[drbId];
        if(PNULL != p_ue_context->p_e_rab_list[erab_id])
        {
            if (p_ue_context->p_e_rab_list[erab_id]->drb_config.bitmask
                    & RRM_DRB_CONFIG_RLC_CONFIG_PRESENT )
            {
                if (p_ue_context->p_e_rab_list[erab_id]->drb_config.
                        rlc_config_enb.rlc_config.bitmask
                        & RRC_RLC_CONFIG_AM_CONFIG_PRESENT )
                {
            	    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "DRB:[%d] mentioned for lossy HO is not configured for UM mode.",
                    erab_id);
                } 
                else
                {
                    drb_index = (U8)p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                                          s1_src_ho_info.erab_lossy_ho_list.count;

		    p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                        s1_src_ho_info.erab_lossy_ho_list.erab_id[drb_index] = 
                                  p_erab_lossy_ho_list->erab_id[drbId];

		    p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                        s1_src_ho_info.erab_lossy_ho_list.lc_id[drb_index] = 
                            p_ue_context->p_e_rab_list[erab_id]->drb_config.logical_channel_identity;

                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                            s1_src_ho_info.erab_lossy_ho_list.count++;
                }
            }
            else
            {
        	RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"RLC config not present for DRB:[%d].", erab_id);
            }
        }
        else
        {
        	RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"DRB:[%d]] doesn't exist.", erab_id);
        }
    }
    if(RRC_NULL < p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                            s1_src_ho_info.erab_lossy_ho_list.count)
    {
        p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.bitmask |=
            ERAB_LOSSY_HO_LIST_PRESENT;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* lossy hoi changes stop */
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_enb_status_transfer_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_enb_status_transfer_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    
    /* in case of CDMA HO */
    if (RRC_TRUE == p_ue_context->ho_info.cdma_ho_ongoing)
    {
        /* if data forwarding is required */
        if( p_ue_context->ho_info.p_ho_info->p_src_ho_info->
            s1_src_ho_info.bitmask
            & ERAB_SUBJECT_TO_DATA_FORWARDING_LIST_PRESENT )
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
               "Data forwarding is required");

             UECC_FSM_SET_CHILD_STATE(p_ue_context,
             &uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing,
              PNULL);

        }

        else 
        {
            /* if forwarding in not required  */
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "Data forwarding is not required");
            /* child state switching to uecc_fsm_src_ho_wait_for_srb_data_status_ind */
            UECC_FSM_SET_CHILD_STATE(p_ue_context,
            &uecc_fsm_src_ho_wait_for_srb_data_status_ind, PNULL);
        }
    }
    else
    { 
         /* Build and sent the SN HFN status request to LLIM */
         if (RRC_SUCCESS != uecc_llim_build_and_send_sn_hfn_status_transfer_req(p_ue_context))
         {
    /*BUG 604 changes start*/
             if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
             { 
                     /* Process failure according to selected failure actions list */
                     uecc_fsm_start_failure_processing(
                             p_ue_context,
                             uecc_fsm_src_ho_fail_type33_actions);
                 }
             else
             {  
                 /*SPR_16778_START*/
                 if ((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
                         (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
                         (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
                 {
                     p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
                     p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
                 }
                 /*SPR_16778_END*/
                 /* Process failure according to selected failure actions list */
                 uecc_fsm_start_failure_processing(
                                   p_ue_context,
                          uecc_fsm_src_ho_fail_type40_actions);
             }
    /*BUG 604 changes stop*/

         }
    
         /*setting its to true */
    /*BUG 604 changes start*/
         if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    /*BUG 604 changes stop*/
         {  
             p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                  is_sn_hfn_req_sent = RRC_FALSE;
         } 
         else
         {
            p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                is_sn_hfn_req_sent = RRC_TRUE;
         }
    }
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_enb_status_transfer_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_enb_status_transfer_ongoing_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*BUG 604 changes start*/
    if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    /*BUG 604 changes stop*/
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"invalid event received");
        return;
    }

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "HO Overall Time out.");

    /* filling the cause in p_ue_context */
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_tx2relocoverall_expiry;

        /*SPR_16618_START*/
        /* Send Release Ind to X2AP Module */

       /* if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
            uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
        }*/
	/*SPR_16618_END*/
    }
    else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_tS1relocoverall_expiry;
    }
    /*SPR:6462 start*/
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

    if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                p_ue_context))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                "Failed to send the reestablish complete indication to LLIM");
    }
    p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;
    /*SPR:6462 stop*/
    
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type4_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* SPR 16750 Fix Start */
/* Code Removed */
/* SPR 16750 Fix Stop */

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_sn_hfn_resp_s1_ho_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
    		    uecc_fsm_fail_handler_f const* pointer to failure_actions_list  
 *	            rrc_uecc_llim_sn_hfn_status_resp_t* - pointer to rrc_uecc_llim_sn_hfn status response
 * Outputs        : None
 * Returns        : RRC_SUCCESS
 *		    RRC_FAILURE 
 * Description    : 
 *                     
 ****************************************************************************/
static rrc_return_t uecc_fsm_src_ho_sn_hfn_resp_s1_ho_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data,
	rrc_uecc_llim_sn_hfn_status_resp_t*  p_rrc_uecc_llim_sn_hfn_status_resp
        )
{
	U8 sn_hfn_cnt = 0, erabCnt= 0, erab_id = 0;
	uecc_ho_src_drb_forward_info_t  *p_src_drb_forward_info = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
	if(HANDOVER_TYPE_INTRA_LTE_S1 ==  p_ue_context->ho_info.p_ho_info->
			p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
	{
		if (RRC_TRUE == (UECC_LLIM_SN_HFN_STATUS_PRESENT & p_rrc_uecc_llim_sn_hfn_status_resp->bitmask))
		{
			sn_hfn_cnt =(p_ue_context->e_rab_list_count <
					p_rrc_uecc_llim_sn_hfn_status_resp->sn_hfn_status_list.count?
					p_ue_context->e_rab_list_count :
					p_rrc_uecc_llim_sn_hfn_status_resp->sn_hfn_status_list.count);

			/* remove the UL status for the erabs for which UL forwading was not allowed */
			for(erabCnt = 0; erabCnt < sn_hfn_cnt; erabCnt++ )
			{
				if (p_rrc_uecc_llim_sn_hfn_status_resp->sn_hfn_status_list.
						sn_hfn_status[erabCnt].bitmask &
						UECC_LLIM_RECEIVE_STATUS_OF_UL_PDCP_SDU_PRESENT)
				{
					erab_id = (U8) (get_erab_id_from_lc_id(p_ue_context,
								p_rrc_uecc_llim_sn_hfn_status_resp->
								sn_hfn_status_list.sn_hfn_status[erabCnt].lc_id));
					if(INVALID_ERAB_ID == erab_id)
					{
						RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
								p_ue_context->p_gb_context->facility_name,RRC_WARNING,
								"SN HFN STATUS response received with Invalid LC Id %u",
								p_rrc_uecc_llim_sn_hfn_status_resp->
								sn_hfn_status_list.sn_hfn_status[erabCnt].lc_id);
						continue;
					}
					/* checking for erabs with UL allowed or restricted */
					if(PNULL != (p_src_drb_forward_info = p_ue_context->ho_info.
								p_ho_info->p_src_ho_info->s1_src_ho_info.
								src_drb_fwd_list.erab_ho_src_drb_forward_list
								[erab_id]))
					{
						if(!(p_src_drb_forward_info->forward_direction ==
									UECC_FORWARDING_DIRECTION_UL
									|| p_src_drb_forward_info->forward_direction
									== UECC_FORWARDING_DIRECTION_BOTH ))
						{
							/* In case if UL is not allowed then no need to send
							   UL_PDCP_SDU status */
							p_rrc_uecc_llim_sn_hfn_status_resp->sn_hfn_status_list.
								sn_hfn_status[erabCnt].bitmask ^=
								UECC_LLIM_RECEIVE_STATUS_OF_UL_PDCP_SDU_PRESENT;
						}

					}
					else
					{
						p_rrc_uecc_llim_sn_hfn_status_resp->sn_hfn_status_list.
							sn_hfn_status[erabCnt].bitmask ^=
							UECC_LLIM_RECEIVE_STATUS_OF_UL_PDCP_SDU_PRESENT;
					}
				}
			}

			if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_status_transfer(
						p_ue_context,
						p_rrc_uecc_llim_sn_hfn_status_resp))
			{
				RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
						"[UECC_LLIM_SN_HFN_STATUS_RESP] RRC_FAILURE "
						"response has been received;Unused variable"
                       " p_api_data=%p",p_api_data);/*SPR 17777 +-*/
                                /* SPR:6462 start*/
                                p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;
                                //p_failure_actions_list = uecc_fsm_src_ho_fail_type4_actions;
                                /* SPR:6462 stop*/
				return RRC_FAILURE;
			}
		}
		else
		{
			RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
					"[UECC_LLIM_SN_HFN_STATUS_RESP]"
					"SN_HFN_STATUS NOT PRESENT");
		}
	}
	/* if data forwarding is required */
	if( p_ue_context->ho_info.p_ho_info->p_src_ho_info->
			s1_src_ho_info.bitmask
			& ERAB_SUBJECT_TO_DATA_FORWARDING_LIST_PRESENT )
	{
		RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
				"[UECC_LLIM_SN_HFN_STATUS_RESP] "
				"Data forwarding is required");

		UECC_FSM_SET_CHILD_STATE(p_ue_context,
				&uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing,
				PNULL);

	}
	else
	{
		/* if forwarding in not required  */
		RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
				"[UECC_LLIM_SN_HFN_STATUS_RESP] "
				"Data forwarding is not required");
		/* child state switching to uecc_fsm_src_ho_wait_for_srb_data_status_ind */
		UECC_FSM_SET_CHILD_STATE(p_ue_context,
				&uecc_fsm_src_ho_wait_for_srb_data_status_ind, PNULL);
	}
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
	return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_sn_hfn_resp_x2_ho_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
    		    uecc_fsm_fail_handler_f const*  pointer to failure_actions_list 
 *		    rrc_uecc_llim_sn_hfn_status_resp_t*  - pointer to rrc_uecc_llim_sn_hfn status response
 * Outputs        : None
 * Returns        : RRC_FAILURE 
 * Description    : 
 *                       
 ****************************************************************************/
static rrc_return_t uecc_fsm_src_ho_sn_hfn_resp_x2_ho_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data,
	rrc_uecc_llim_sn_hfn_status_resp_t*  p_rrc_uecc_llim_sn_hfn_status_resp)
{
    U8 sn_hfn_cnt = 0, erabCnt= 0, erab_id = 0;
    uecc_ho_src_drb_admitted_info_t *p_src_drb_admitted_info = PNULL;
    rrc_bool_et is_s1u_recf_ongoing = RRC_FALSE;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    if (RRC_TRUE == (UECC_LLIM_SN_HFN_STATUS_PRESENT &
                p_rrc_uecc_llim_sn_hfn_status_resp->bitmask))
    {
        sn_hfn_cnt =(p_ue_context->e_rab_list_count <
                p_rrc_uecc_llim_sn_hfn_status_resp->sn_hfn_status_list.count?
                p_ue_context->e_rab_list_count :
                p_rrc_uecc_llim_sn_hfn_status_resp->sn_hfn_status_list.count);

        /* remove the UL status for the erabs for which UL forwading was not allowed */
        for(erabCnt = 0; erabCnt < sn_hfn_cnt; erabCnt++ )
        {
            if (p_rrc_uecc_llim_sn_hfn_status_resp->sn_hfn_status_list.
                    sn_hfn_status[erabCnt].bitmask &
                    UECC_LLIM_RECEIVE_STATUS_OF_UL_PDCP_SDU_PRESENT)
            {
                erab_id = (U8) (get_erab_id_from_lc_id(p_ue_context,
                            p_rrc_uecc_llim_sn_hfn_status_resp->
                            sn_hfn_status_list.sn_hfn_status[erabCnt].lc_id));
                if(INVALID_ERAB_ID == erab_id)
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                            "SN HFN STATUS response received with Invalid LC Id %u",
                            p_rrc_uecc_llim_sn_hfn_status_resp->
                            sn_hfn_status_list.sn_hfn_status[erabCnt].lc_id);
                    continue;
                }
                /* checking for erabs with UL allowed or restricted */
                if(PNULL != (p_src_drb_admitted_info = p_ue_context->ho_info.
                            p_ho_info->p_src_ho_info->s1_src_ho_info.
                            src_erab_admitted_list.erab_ho_src_drb_admitted_info
                            [erab_id]))
                {
                    if(!(p_src_drb_admitted_info->forward_direction ==
                                UECC_FORWARDING_DIRECTION_UL
                                || p_src_drb_admitted_info->forward_direction
                                == UECC_FORWARDING_DIRECTION_BOTH ))
                    {
                        /* In case if UL is not allowed then no need to send
                           UL_PDCP_SDU status */
                        p_rrc_uecc_llim_sn_hfn_status_resp->sn_hfn_status_list.
                            sn_hfn_status[erabCnt].bitmask ^=
                            UECC_LLIM_RECEIVE_STATUS_OF_UL_PDCP_SDU_PRESENT;
                    }
                }
                else
                {
                    p_rrc_uecc_llim_sn_hfn_status_resp->sn_hfn_status_list.
                        sn_hfn_status[erabCnt].bitmask ^=
                        UECC_LLIM_RECEIVE_STATUS_OF_UL_PDCP_SDU_PRESENT;
                }
            }
        }

        if (RRC_SUCCESS != uecc_x2ap_build_and_send_sn_status_transfer(
                    p_ue_context,
                    p_rrc_uecc_llim_sn_hfn_status_resp))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_SN_HFN_STATUS_RESP] RRC_FAILURE "
                    "response has been received;Unused variable"
                    "p_api_data=%p",p_api_data);/*SPR 17777 +-*/
            /*SPR_16778_START*/
            p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
            /*SPR_16778_END*/
            /* SPR:6462 start*/
            p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;
            //p_failure_actions_list = uecc_fsm_src_ho_fail_type4_actions;
            /* SPR:6462 stop*/

            return RRC_FAILURE;		                    
        }
    }
    else
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                "[UECC_LLIM_SN_HFN_STATUS_RESP]"
                "SN_HFN_STATUS NOT PRESENT");
    }

    /*BUG 604 changes start*/
    if(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)
    {

        UECC_FSM_SET_CHILD_STATE(p_ue_context,
                &uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing,
                PNULL);
    }
    else
    {
	    /*BUG 604 changes stop*/
	    /* if data forwarding is required */
	    if( p_ue_context->ho_info.p_ho_info->p_src_ho_info->
			    s1_src_ho_info.bitmask
			    & X2AP_ERAB_ADMITTED_LIST_PRESENT )
	    {
		    for(erab_id =0; erab_id < MAX_ERAB_COUNT; erab_id ++)
		    {
			    if(PNULL != (p_src_drb_admitted_info = p_ue_context->ho_info.
						    p_ho_info->p_src_ho_info->s1_src_ho_info.
						    src_erab_admitted_list.erab_ho_src_drb_admitted_info
						    [erab_id]))
			    {
				    if ( UECC_FORWARDING_DIRECTION_ABSENT !=
						    p_src_drb_admitted_info->forward_direction )
				    {
					    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
							    "[UECC_LLIM_SN_HFN_STATUS_RESP] "
							    "Data forwarding is required");

					    UECC_FSM_SET_CHILD_STATE(p_ue_context,
							    &uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing,
							    PNULL);
					    is_s1u_recf_ongoing = RRC_TRUE;
					    break;
				    }	 
			    }
		    }
	    }
    /*BUG 604 changes start*/
    }
    if ((RRC_FALSE == is_s1u_recf_ongoing)&&
            (INTRA_CELL_HO != p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        /* if forwarding in not required  */
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                "[UECC_LLIM_SN_HFN_STATUS_RESP] "
                "Data forwarding is not required");
        /* child state switching to uecc_fsm_src_ho_wait_for_srb_data_status_ind */
        UECC_FSM_SET_CHILD_STATE(p_ue_context,
                &uecc_fsm_src_ho_wait_for_srb_data_status_ind, PNULL);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_sn_hfn_status_resp_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_SRC_HO_EVENT_UE_ADMISSION_RESP from a RRM.
 ****************************************************************************/
static void uecc_fsm_src_ho_sn_hfn_status_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    rrc_uecc_llim_sn_hfn_status_resp_t* p_rrc_uecc_llim_sn_hfn_status_resp = PNULL;
    rrc_return_t                    result = RRC_FAILURE;
    uecc_fsm_fail_handler_f const*  p_failure_actions_list =
        uecc_fsm_src_ho_fail_type4_actions;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_uecc_llim_sn_hfn_status_resp 
        = (rrc_uecc_llim_sn_hfn_status_resp_t*)p_api_data;

    switch (p_rrc_uecc_llim_sn_hfn_status_resp->response_code)
    {
        case RRC_SUCCESS:
            result = RRC_SUCCESS;
            if( S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover )
            {
                result=uecc_fsm_src_ho_sn_hfn_resp_s1_ho_handler(
                        p_ue_context,p_api_data,
                        p_rrc_uecc_llim_sn_hfn_status_resp);
                if (RRC_FAILURE == result)
                {
                    p_failure_actions_list = uecc_fsm_src_ho_fail_type4_actions;
                }
            }
    /*BUG 604 changes start*/
            else if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                    (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
                    (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes start*/
            {
                result=uecc_fsm_src_ho_sn_hfn_resp_x2_ho_handler(
                        p_ue_context,p_api_data,
                        p_rrc_uecc_llim_sn_hfn_status_resp);
                if (RRC_FAILURE == result)
                {
                    p_failure_actions_list = uecc_fsm_src_ho_fail_type4_actions;
                }
            }
    /*BUG 604 changes start*/
            else if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover )
    /*BUG 604 changes stop*/
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                        "[UECC_LLIM_SN_HFN_STATUS_RESP] " 
                        "received in Handover Type: %d",
                        p_ue_context->ho_info.s1_or_x2_handover);
                UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_reset_ue_entity_ongoing, PNULL);
            }
            break;

        case RRC_PARTIAL_SUCCESS:
            result = RRC_PARTIAL_SUCCESS;
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "[UECC_LLIM_SN_HFN_STATUS_RESP] RRC_PARTIAL_SUCCESS "
                    "response has been received");
    /*BUG 604 changes start*/
            if (CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover )
            {
                p_failure_actions_list = uecc_fsm_src_ho_fail_type33_actions;
            }
            else
            {
                /*BUG 604 changes start*/
                    /* SPR:6462 start*/
                    p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;
                    /* SPR:6462 stop*/
            /*BUG 604 changes stop*/
            }
    /*BUG 604 changes stop*/
            break;

        case RRC_FAILURE:
            /* RRM rejected connection */
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_SN_HFN_STATUS_RESP] RRC_FAILURE "
                    "response has been received");
    /*BUG 604 changes start*/
            if (CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover )
            {
                p_failure_actions_list = uecc_fsm_src_ho_fail_type33_actions;
            }
            else
            {
                /*BUG 604 changes start*/
                    /* SPR:6462 start*/
                    p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;
                    //p_failure_actions_list = uecc_fsm_src_ho_fail_type4_actions;
                    /* SPR:6462 stop*/
                /*BUG 604 changes stop*/
            }
    /*BUG 604 changes stop*/
            break;

        default:
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_SN_HFN_STATUS_RESP] Invalid Response "
                    "=%d",p_rrc_uecc_llim_sn_hfn_status_resp->response_code);
            break;
    }

    /* Process failure */
    if (RRC_SUCCESS != result)
    {
        /*x2_ho_timer*/
        /* Stop timer */
        /*uecc_ue_timer_stop( UECC_UE_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data );*/

        /* updating the cause */
    /*BUG 604 changes start*/
        if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
           (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
           (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
    /*BUG 604 changes stop*/
        {
            /*x2_ho_timer*/
            /* Stop timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_X2_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data );

            p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
        }
        else if(S1_HO == p_ue_context->ho_info.s1_or_x2_handover) 
        {
            /*x2_ho_timer*/
            /* Stop timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_HO_OVERALL_TIMER, 
                    &p_ue_context->timers_data );

            p_ue_context->ho_info.cause.t= T_s1ap_Cause_radioNetwork; 
            p_ue_context->ho_info.cause.u.radioNetwork= s1ap_unspecified_2; 
        }
    /*BUG 604 changes start*/
        /*else if(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) 
        {
            uecc_ue_timer_stop( p_ue_context->p_gb_context,
                    UECC_UE_INTRA_CELL_HO_GUARD_TIMER,
                    &p_ue_context->timers_data );
        }*/
    /*BUG 604 changes stop*/
        else if (CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover )
        { 
            uecc_ue_timer_stop( p_ue_context->p_gb_context,
                    UECC_UE_CCO_GUARD_TIMER,
                    &p_ue_context->timers_data );
        }
        /*SPR:6462 start*/
        if(RRC_UECC_FAIL_TYPE4 != p_ue_context->fail_action_type)
        {
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(p_ue_context,
                    p_failure_actions_list);
        }
        else
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

            if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                        p_ue_context))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "Failed to send the reestablish complete indication to LLIM");
            }
        }
        /*SPR:6462 stop*/
      
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*BUG 604 changes start*/
    if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover )
    {
        if (RRC_SUCCESS != uecc_llim_build_and_send_s1u_intra_enb_data_fwd_req(p_ue_context))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "Fail to send S1U_DATA_FWD_REQ to LLIM");
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

            if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                        p_ue_context))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "Failed to send the reestablish complete indication to LLIM");
            }
            p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;
        }
    }
    else
    {
        /*BUG 604 changes stop*/
    /* Build and sent the S1U Reconfig request to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_s1u_reconfig_req(p_ue_context))
    {
	    /*SPR_16778_START*/
	    if ((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
			    (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
			    (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
	    {
		    p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
		    p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
	    }
	    /*SPR_16778_END*/
        /*SPR:6462 start*/
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

        if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                    p_ue_context))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                    "Failed to send the reestablish complete indication to LLIM");
        }
        p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;
        /*SPR:6462 stop*/
    }
  }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_s1u_reconfig_for_forwarding_ongoing_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "HO Overall Time out.");
    /* filling the cause in p_ue_context */
/*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = 
            x2ap_tx2relocoverall_expiry;
    
        /*SPR_16618_START*/
        /* Send Release Ind to X2AP Module */
        /*if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
            uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
        }*/
	/*SPR_16618_END*/
    }
    else
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = 
                                        s1ap_tS1relocoverall_expiry;
    }
    /* Continue failure processing */
    UECC_FSM_SET_CHILD_STATE(p_ue_context,
            &uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing,
            PNULL);

    /*SPR_16618_START*/
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
		    p_ue_context,
		    uecc_fsm_src_ho_fail_type4_actions);
    /*SPR_16618_END*/


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* SPR 16750 Fix Start */
/* Code Removed */
/* SPR 16750 Fix Stop */

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_s1u_reconfig_for_forwarding_tunnel_cnf_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_SRC_HO_EVENT_UE_ADMISSION_RESP from a RRM.
 ****************************************************************************/
static void uecc_fsm_src_ho_llim_s1u_reconfig_for_forwarding_tunnel_cnf_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    rrc_uecc_llim_s1u_reconfigure_cnf_t*
        p_rrc_uecc_llim_s1u_reconfigure_cnf = PNULL;
    rrc_return_t                    result = RRC_FAILURE;
    uecc_fsm_fail_handler_f const*  p_failure_actions_list =
        uecc_fsm_src_ho_fail_type4_actions;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_uecc_llim_s1u_reconfigure_cnf
        = (rrc_uecc_llim_s1u_reconfigure_cnf_t*)p_api_data;

/*BUG 604 changes start*/
    if(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
/*BUG 604 changes stop*/
    switch (p_rrc_uecc_llim_s1u_reconfigure_cnf->response_code)
    {
        case RRC_SUCCESS:
            result = RRC_SUCCESS;

            /* Set S1U Reconfigured  */
            RRC_ASSERT(PNULL != p_ue_context->ho_info.p_ho_info);
            RRC_ASSERT(PNULL != p_ue_context->ho_info.p_ho_info->p_src_ho_info);
            p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1u_reconfigured = RRC_TRUE;

            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "[UECC_LLIM_S1U_RECONFIG_CNF] S1U Reconfigured "
                    "successfully");
            /*setting s1u_reconfigured is to true */
            p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1u_reconfigured =
                RRC_TRUE;

            /* Storing the GTP-Self TEID */
            rrc_src_ho_process_s1u_cnf(
                    p_rrc_uecc_llim_s1u_reconfigure_cnf,
                    p_ue_context);

            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                    &uecc_fsm_src_ho_wait_for_srb_data_status_ind, PNULL);
            break;

        case RRC_PARTIAL_SUCCESS:
            result = RRC_PARTIAL_SUCCESS;
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "[UECC_LLIM_S1U_RECONFIG_CNF] RRC_PARTIAL_SUCCESS "
                    "response has been received");
            /* Storing the GTP-Self TEID */
            rrc_src_ho_process_s1u_cnf(
                p_rrc_uecc_llim_s1u_reconfigure_cnf,
                p_ue_context);
            /*setting s1u_reconfigured is to true */
            p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1u_reconfigured =
                RRC_TRUE;
            
            /*x2_ho_timer*/
            if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
               (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
            {
                /* Stop timer */
                uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                        UECC_UE_X2_HO_OVERALL_TIMER,
                    &p_ue_context->timers_data );
            }
            else
            {
                /* Stop timer */
                uecc_ue_timer_stop(p_ue_context->p_gb_context,
                        UECC_UE_HO_OVERALL_TIMER,
                    &p_ue_context->timers_data );
            }
            /* state changed to UECC_SRC_HO_S1U_DELETE_FORWARDING_TUNNEL_ONGOING in order
               to delete the rest of successfully created tunnels */
            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                    &uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing, PNULL);
            break;


        case RRC_FAILURE:
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_S1U_RECONFIG_FOR_FORWARDING_RESP] RRC_FAILURE " 
                    "response has been received");
            if(IS_RLF_RECEIVED(p_ue_context))
            {
                p_failure_actions_list = uecc_fsm_src_ho_fail_type19_actions;
            }        
            else
            {
                /* fail to reconfigure the s1u */
                /*SPR:6462 start*/
                p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;  
                //p_failure_actions_list = uecc_fsm_src_ho_fail_type4_actions;
                /*SPR:6462 stop*/
            }
            break;

        default:
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_S1U_RECONFIG_CNF] Invalid Response=%i",
                    p_rrc_uecc_llim_s1u_reconfigure_cnf->response_code);
            break;
    }

    /* Process failure */
    if (RRC_FAILURE == result)
    {
        /*x2_ho_timer*/
        /* Stop timer */
        /*uecc_ue_timer_stop( UECC_UE_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data );*/
        /* filling the cause in p_ue_context */
        if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
           (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
        {
            /*x2_ho_timer*/
            /* Stop timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_X2_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data );

            p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
        }
        else
        {
            /*x2_ho_timer*/
            /* Stop timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_UE_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data );

            p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
        }
        /* Process failure according to selected failure actions list */
        /*SPR:6462 start*/
        if(RRC_UECC_FAIL_TYPE4 != p_ue_context->fail_action_type)
        {
            uecc_fsm_start_failure_processing(p_ue_context,
                    p_failure_actions_list);
        }
        else
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

            if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                        p_ue_context))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "Failed to send the reestablish complete indication to LLIM");
            }
        }
        /*SPR:6462 stop */
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_src_ho_wait_for_srb_init_lte_to_utran_ho_hdlr
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 *   		    rrc_uecc_llim_srb_data_req_t*   -    pointer to srb data required	
 *    		    OSCTXT*   -   pointer to asn1 context
 * Outputs        : None
 * Returns        : RRC_SUCCESS
 *		    RRC_FAILURE
 * Description    : For inter-RAT, send MobilityFromEutraCommand
 ****************************************************************************/
static rrc_return_t uecc_src_ho_wait_for_srb_init_lte_to_utran_ho_hdlr(
        uecc_ue_context_t*  p_ue_context,
        void*       p_api,
    	rrc_uecc_llim_srb_data_req_t*           p_srb_data_req,	
    	OSCTXT* 	                        p_asn1_ctx
        )
{
    s1ap_Cause s1apCause;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /* BUG 12662 Fix Start */
    U8 index = RRC_NULL;
    S32 cmp_result = -1;
    /* BUG 12662 Fix Stop */


    /* CR 487 changes start*/
    if ((HANDOVER_TYPE_LTE_TO_CDMA2000_1XRTT == p_ue_context->ho_info.p_ho_info->
                    p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) &&
                (RRC_RRM_HO_REQUIRED_HO_FROM_EUTRA_PREPERATION_REQ_PRESENT & 
                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                    p_rrc_rrm_ho_required->cdma2000_information.bitmask) &&
                (RRC_RRM_HO_FROM_EUTRA_DUAL_RX_TX_REDIRECT_IND_PRESENT & 
                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                    p_rrc_rrm_ho_required->cdma2000_information.ho_from_eutra_preperation_req_r10.
                    bitmask) &&
                (RRC_RRM_HO_FROM_EUTRA_REDIRECT_CARRIER_CDMA2000_1XRTT_PRESENT & 
                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                    p_rrc_rrm_ho_required->cdma2000_information.ho_from_eutra_preperation_req_r10.
                    bitmask))
    {
        /*send dl_information_transfer for 1xrtt*/
        if(RRC_SUCCESS != uecc_llim_build_and_send_rrc_dl_information_transfer(
                            p_ue_context,
                            &p_ue_context->cdma2000_info.
                            dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu,
                            T_DLInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_1XRTT))
        {
            s1apCause.t = T_s1ap_Cause_radioNetwork;
            s1apCause.u.radioNetwork = s1ap_unspecified_2;

            if (RRC_SUCCESS != uecc_s1ap_build_and_send_nas_non_delivery_ind(
                                p_ue_context->p_gb_context,
                                p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.mme_ue_s1ap_id,
                                p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.enb_ue_s1ap_id,
                                &p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu,
                                &s1apCause))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                                "[s1ap_NASNonDeliveryIndication] delivery fail");
            }
        }
    }
    else
    {
    /* CR 487 changes stop*/

        /* SPR_7888_fix */
        if(RRC_SUCCESS != convert_rrm_cause_to_s1ap_cause(
                    &p_ue_context->ho_info.p_ho_info->
                    p_src_ho_info->s1_src_ho_info.
                    p_rrc_rrm_ho_required->cause, &s1apCause))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                               p_ue_context->p_gb_context->facility_name, 
                               RRC_WARNING, "Invalid Cause received from"
                              "RRM;Unused variable p_api=%p",p_api);/*SPR 17777 +-*/
        }
        /* SPR_7888_fix */

    if(RRC_SUCCESS != uecc_s1ap_fillMobilityFromEUTRACommand(p_ue_context,
                p_srb_data_req->data_buffer,
                &(p_srb_data_req->data_buffer_length),
                p_asn1_ctx))				
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_WARNING,"[S1AP Handover Command]"
                "uecc_s1ap_fillMobilityFromEUTRACommand Return Failure");
        /* Process failure according to selected failure actions list */

        if (HANDOVER_TYPE_CCO == p_ue_context->ho_info.p_ho_info->\
                p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
        {
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type33_actions);
        }
        else
        {
            /*SPR:6462 start*/
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                            "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

            if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                                p_ue_context))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                                "Failed to send the reestablish complete indication to LLIM");
            }
            p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;
            /*SPR:6462 stop*/

        }
        return RRC_FAILURE;
    }
    else
    {
	    /* BUG 12662 Fix Start */
        /* Checking whether ho attempt for the particular target ID
        * already present or not . If yes, then incrementing the count
        * for this new ho request for this same target ID */
        if (PNULL != g_rrc_stats.p_rrc_stats)
        {
    	    for (index = RRC_NULL; index < (g_rrc_stats.p_rrc_stats->
		        rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
		        p_gb_context->offset_uecc_mod_id].
		        rrc_uecc_cell_stats[p_ue_context->cell_index].
		        rrc_handover_related_stats.inter_rat_ho_stats_list.count);
		        index++) 
	        {
                /*SPR_19166_START*/
                cmp_result = rrc_compare_inter_rat_target_cgi( 
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.inter_rat_ho_stats_list.
                    inter_rat_ho_stats[index].target_cgi,
                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                    p_rrc_rrm_ho_required);
                /*SPR_19166_END*/

                if (RRC_NULL == cmp_result)
                {
                    break;
                }
	        }   
 	
	    if (cmp_result != RRC_NULL)
	    {
		    /*SPR_16595_START*/
		    if(RRC_MAX_CONNECTION > g_rrc_stats.p_rrc_stats->
				    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
				    p_gb_context->offset_uecc_mod_id].
				    rrc_uecc_cell_stats[p_ue_context->cell_index].
				    rrc_handover_related_stats.inter_rat_ho_stats_list.count)
		    {
		    /*SPR_16595_END*/
			    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
					    g_rrc_stats.p_rrc_stats->
					    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
					    p_gb_context->offset_uecc_mod_id].
					    rrc_uecc_cell_stats[p_ue_context->cell_index].
					    rrc_handover_related_stats.inter_rat_ho_stats_list.count,
					    0, DUMMY_RANGE_CHECK_INDEX);

                index = (U8)(g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.inter_rat_ho_stats_list.count - 1);

                        /*SPR_19166_START*/
                        rrc_fill_inter_rat_cgi_info(
                                &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.inter_rat_ho_stats_list.
                                inter_rat_ho_stats[index].target_cgi,
                                p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                                p_rrc_rrm_ho_required);
                        /*SPR_19166_END*/

                    /*SPR_16595_START*/
                    }
                    else
                    {
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                                "[%s]: Maximum target Id count reached. L3 is ignoring "
                                "performance counters for this target Id.",__FUNCTION__);
                    }
                    /*SPR_16595_END*/
	    }

	    /*SPR_16595_START*/
	    if(RRC_MAX_CONNECTION > index)
	    {
	    /*SPR_16595_END*/
		    RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
				    g_rrc_stats.p_rrc_stats->
				    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
				    p_gb_context->offset_uecc_mod_id].
				    rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_handover_related_stats.
				    inter_rat_ho_stats_list.inter_rat_ho_stats[index].ho_IratOutAtt_cause.
				    ho_IratOutAtt_Cause,
				    s1apCause);

		    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
				    g_rrc_stats.p_rrc_stats->
				    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
				    p_gb_context->offset_uecc_mod_id].
				    rrc_uecc_cell_stats[p_ue_context->cell_index].
				    rrc_handover_related_stats.inter_rat_ho_stats_list.
				    inter_rat_ho_stats[index].ho_IratOutAtt_sum, 0,
				    DUMMY_RANGE_CHECK_INDEX);
	    /*SPR_16595_START*/
	    }
	    /*SPR_16595_END*/
	    /* BUG 12662 Fix Stop */
	}
    }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_src_intra_lte_s1_ho_update_performance_counter
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    :
 *****************************************************************************/
static void uecc_src_intra_lte_s1_ho_update_performance_counter(
                        uecc_ue_context_t*              p_ue_context,
                        s1ap_Cause s1apCause,
                        U8 index)

{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
            meas_on_neigh_cell_ho_stats[index].ho_OutAttTarget_cause.
            ho_OutAttTarget_Cause, s1apCause);

    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,g_rrc_stats.
            p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_handover_related_stats.inter_enb_ho_stats.
            ho_InterEnbOutAtt_sum, 0, DUMMY_RANGE_CHECK_INDEX);

    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,g_rrc_stats.
            p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
            meas_on_neigh_cell_ho_stats[index].ho_OutAttTarget_sum,
            0, DUMMY_RANGE_CHECK_INDEX);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_src_intra_lte_s1_ho_hdlr
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Update Performance Counters
 ****************************************************************************/

static void uecc_src_intra_lte_s1_ho_hdlr(
                uecc_ue_context_t*              p_ue_context)
{
    U8 index = 0;
    S32 cmp_result = -1;
    s1ap_Cause s1apCause;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    memset_wrapper(&s1apCause, 0, sizeof(s1apCause));
    
    /* SPR_7888_fix */
    if(RRC_SUCCESS != convert_rrm_cause_to_s1ap_cause(
                        &p_ue_context->ho_info.p_ho_info->
                        p_src_ho_info->s1_src_ho_info.
                        p_rrc_rrm_ho_required->cause, &s1apCause))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "Invalid Cause received from RRM");
    }
    /* SPR_7888_fix */

    RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,g_rrc_stats.
            p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause, s1apCause);

    /* Checking whether ho attempt for the particular target ID
     * already present or not . If yes, then incrementing the count
     * for this new ho request for this same target ID */
    for (index = 0; index < (g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count);
            index++)
    {
        cmp_result = memcmp_wrapper(&g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                /*SPR_19166_START*/
        /*CID 110081 Fix Start*/
                meas_on_neigh_cell_ho_stats[index].ecgi.plmn_identity,
                /*SPR_19166_END*/
                &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.plmn_identity, 
                sizeof(plmn_identity_t));

        if (RRC_NULL == cmp_result)
        {
            cmp_result = memcmp_wrapper(&g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[index].ecgi.cell_identity,
                    &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                    p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.cell_identity, 
                    MAX_CELL_IDENTITY_NUMBYTES);
            /*SPR_19166_END*/
            if (RRC_NULL == cmp_result)
            {
                break;
            }
        }
        /*CID 110081 Fix Stop*/
    }

    if (cmp_result != RRC_NULL)
    {
        /*SPR_16595_START*/
        if(RRC_MAX_CONNECTION > g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count)
        {
        /*SPR_16595_END*/
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count,
                0, DUMMY_RANGE_CHECK_INDEX);

        index = (U8)(g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count - 1);

        l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
        /*SPR_19166_START*/
        /*CID 110081 Fix Start*/
                meas_on_neigh_cell_ho_stats[index].ecgi.plmn_identity,
                &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.plmn_identity, sizeof(plmn_identity_t));
        /*SPR_19166_END*/

        l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[index].ecgi.cell_identity,
                &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.cell_identity,MAX_CELL_IDENTITY_NUMBYTES);
        /*CID 110081 Fix Start*/



        /*SPR_16595_START*/
        }
        else
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[%s]: Maximum target Id count reached. L3 is ignoring "
                    "performance counters for this target Id.",__FUNCTION__);
        }
        /*SPR_16595_END*/
    }
    /*SPR_16595_START*/
    if(RRC_MAX_CONNECTION > index)
    {
        uecc_src_intra_lte_s1_ho_update_performance_counter(p_ue_context,s1apCause,index);
    }
    /*SPR_16595_END*/
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_src_intra_lte_x2_ho_update_performance_counter
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    :
 ****************************************************************************/

static void uecc_src_intra_lte_x2_ho_update_performance_counter(
                                uecc_ue_context_t*              p_ue_context,
                                x2ap_Cause                      x2apCause,
                                U8                              index)
{
	/*SPR_17284_START*/
    s1ap_Cause s1apCause;
    memset_wrapper(&s1apCause, 0, sizeof(s1apCause));

    map_x2ap_cause_to_s1ap_cause(
			x2apCause, &s1apCause);

    RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,g_rrc_stats.
            p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
            meas_on_neigh_cell_ho_stats[index].
            ho_OutAttTarget_cause.ho_OutAttTarget_Cause, s1apCause);
	/*SPR_17284_END*/

    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_handover_related_stats.inter_enb_ho_stats.
            ho_InterEnbOutAtt_sum, 0, DUMMY_RANGE_CHECK_INDEX);

    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,g_rrc_stats.
            p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
            meas_on_neigh_cell_ho_stats[index].ho_OutAttTarget_sum,
            0, DUMMY_RANGE_CHECK_INDEX);
    
}
/****************************************************************************
 * Function Name  : uecc_src_intra_lte_x2_ho_hdlr
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Update Performance Counters for X2
 *****************************************************************************/

static void uecc_src_intra_lte_x2_ho_hdlr(
                        uecc_ue_context_t*              p_ue_context)
{
    x2ap_Cause x2apCause;
    U8 index = 0;
    S32 cmp_result = -1;
    
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    memset_wrapper(&x2apCause, 0, sizeof(x2apCause));

    convert_rrm_cause_to_x2ap_cause(
            &p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.
            p_rrc_rrm_ho_required->cause, &x2apCause);

    /*SPR_17284_START*/
    s1ap_Cause s1apCause;
    memset_wrapper(&s1apCause, 0, sizeof(s1apCause));

    map_x2ap_cause_to_s1ap_cause(
		    x2apCause, &s1apCause);

    RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_cause.
            ho_InterEnbOutAtt_Cause, s1apCause);
    /*SPR_17284_END*/

    /* Checking whether ho attempt for the particular target ID
     * already present or not . If yes, then incrementing the count
     * for this new ho request for this same target ID and setting
     * the flag*/
    for (index = 0; index < (g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count);
            index++)
    {
        cmp_result = memcmp_wrapper(&g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
        /*SPR_19166_START*/
        /*CID 110085 Fix Start*/
                meas_on_neigh_cell_ho_stats[index].ecgi.plmn_identity,
        /*SPR_19166_END*/
                &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.plmn_identity, 
                sizeof(plmn_identity_t));

        if (RRC_NULL == cmp_result)
        {
            cmp_result = memcmp_wrapper(&g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[index].ecgi.cell_identity,
                    &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                    p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.cell_identity, 
                    MAX_CELL_IDENTITY_NUMBYTES);
            /*SPR_19166_END*/
            if (RRC_NULL == cmp_result)
            {
                break;
            }
        }
        /*CID 110085 Fix Stop*/
    }

    if (cmp_result != RRC_NULL)
    {
        /*SPR_16595_START*/
        if(RRC_MAX_CONNECTION > g_rrc_stats.p_rrc_stats->
             rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
             p_gb_context->offset_uecc_mod_id].
             rrc_uecc_cell_stats[p_ue_context->cell_index].
             rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count)
        {
        /*SPR_16595_END*/
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,g_rrc_stats.
                p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count,
                0, DUMMY_RANGE_CHECK_INDEX);

        index = (U8)(g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count - 1);

        l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
        /*SPR_19166_START*/
        /*CID 110085 Fix Start*/
                meas_on_neigh_cell_ho_stats[index].ecgi.plmn_identity,
                &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.plmn_identity, 
                sizeof(plmn_identity_t));
        /*SPR_19166_END*/
        l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[index].ecgi.cell_identity,
                &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.cell_identity,
                MAX_CELL_IDENTITY_NUMBYTES);
        /*CID 110085 Fix Start*/


        /*SPR_16595_START*/
        }
        else
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "[%s]: Maximum target Id count reached. L3 is ignoring "
                "performance counters for this target Id.",__FUNCTION__);
        }
        /*SPR_16595_END*/
    }
    /*SPR_16595_START*/
    if(RRC_MAX_CONNECTION > index)
    {
        uecc_src_intra_lte_x2_ho_update_performance_counter(p_ue_context,x2apCause,index);
    }
    /*SPR_16595_END*/
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_src_ho_adm_re_ho_freq_present
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Update performance counters for IntraFreq stats
 ****************************************************************************/
static void uecc_src_ho_adm_re_ho_freq_present(
                uecc_ue_context_t*              p_ue_context)
                 
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    if ( HANDOVER_INTRA_FREQ == p_ue_context->ho_info.p_ho_info->p_src_ho_info->
            s1_src_ho_info.p_rrc_rrm_ho_required->ho_freq_type )
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.
                rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.
                intra_freq_ho_stats.ho_IntraFreqOutAtt, RRC_NULL,
                DUMMY_RANGE_CHECK_INDEX);
    }
    else if ( HANDOVER_INTER_FREQ == p_ue_context->ho_info.p_ho_info->p_src_ho_info->
            s1_src_ho_info.p_rrc_rrm_ho_required->ho_freq_type )
    {
        /* Bug 881 Fix Start */
        /* SPR_17047_START */
        if ((p_ue_context->curr_meas_config.m.curr_meas_gap_config_present) &&
            (1 == p_ue_context->curr_meas_config.curr_meas_gap_config.meas_gap_config_type))
        /* SPR_17047_END */
	/* Bug 881 Fix Stop */
        {
            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.
                    inter_freq_ho_stats.ho_InterFreqMeasGapOutAtt,
                    RRC_NULL, DUMMY_RANGE_CHECK_INDEX);

        }
        else
        {
            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.
                    inter_freq_ho_stats.ho_InterFreqNoMeasGapOutAtt,
                    RRC_NULL, DUMMY_RANGE_CHECK_INDEX);

        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_src_ho_wait_for_srb_init_intra_cell_ho_hdlr
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 *   		    rrc_uecc_llim_srb_data_req_t*   -    pointer to srb data required	
 *    		    OSCTXT*   -   pointer to asn1 context
 * Outputs        : None
 * Returns        : None
 * Description    : Update HO related performance counters 
 *                         
 ****************************************************************************/
static void uecc_src_ho_wait_for_srb_init_intra_cell_ho_hdlr(
        uecc_ue_context_t*              p_ue_context,
    	rrc_uecc_llim_srb_data_req_t*   p_srb_data_req)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_srb_data_req->ue_index = p_ue_context->ue_index;
    p_srb_data_req->cell_index = p_ue_context->cell_index;
    p_srb_data_req->lc_id = UECC_LTE_RRC_SRB1_LC_ID;
    /* SPR 13736 Fix Start */
    p_srb_data_req->service_requested = UECC_LLIM_SRB_SERVICEREQUESTED_DELIVERY_STATUS_FALSE;
    /* SPR 13736 Fix Stop */

    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
        if((HANDOVER_TYPE_INTRA_LTE_S1 == p_ue_context->ho_info.p_ho_info->
                    p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) ||
                (HANDOVER_TYPE_INTRA_LTE_X2 == p_ue_context->ho_info.p_ho_info->
                 p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) || /* BUG 12647 Fix Start */
                (HANDOVER_TYPE_INTER_CELL == p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) ||
                (HANDOVER_TYPE_INTRA_CELL == p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)) /* BUG 12647 Fix Stop */
        {
            if(HANDOVER_TYPE_INTRA_LTE_S1 == p_ue_context->ho_info.p_ho_info->
                    p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
            {
                uecc_src_intra_lte_s1_ho_hdlr(p_ue_context);
            }
            else if(HANDOVER_TYPE_INTRA_LTE_X2 == p_ue_context->ho_info.p_ho_info->
                    p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
            {
                uecc_src_intra_lte_x2_ho_hdlr(p_ue_context);
            }/* BUG 12647 Fix Start */
	    else
	    {
		    uecc_src_lte_intra_enb_ho_hdlr(p_ue_context);
	    }/* BUG 12647 Fix Stop */
	
            if ((RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT &
                        p_ue_context->mac_config.bitmask) &&
                    (RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT &
                     /* Bug 880 Fix Start */
                     p_ue_context->mac_config.mac_main_config.bitmask) &&
                     (RRC_DRX_CONFIG_TYPE_SETUP == p_ue_context->
                       mac_config.mac_main_config.drx_config.drx_config_type))
                     /* Bug 880 Fix Stop */
            {
                RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_handover_related_stats.
                        drx_non_drx_ho_stats.ho_DrxOutAtt,
                        RRC_NULL, DUMMY_RANGE_CHECK_INDEX);
            }
            else
            {
                RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_handover_related_stats.
                        drx_non_drx_ho_stats.ho_NoDrxOutAtt, RRC_NULL,
                        DUMMY_RANGE_CHECK_INDEX);
            }
            

            if (( RRC_RRM_UE_HO_ADM_RE_HO_FREQ_TYPE_PRESENT &
                        p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                        p_rrc_rrm_ho_required->bitmask))
            {
                uecc_src_ho_adm_re_ho_freq_present(p_ue_context);
            }
            else
            {
                RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].
                        rrc_handover_related_stats.
                        intra_freq_ho_stats.ho_IntraFreqOutAtt, RRC_NULL,
                        DUMMY_RANGE_CHECK_INDEX);
            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_srb_data_status_ind_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_wait_for_srb_data_status_ind_init_handler(
        uecc_ue_context_t*  p_ue_context,
        void*       p_api
        )
{
    rrc_bool_t ho_command_sent = RRC_TRUE;
    void *p_ho_cmd = PNULL;
    rrc_uecc_llim_srb_data_req_t*           p_srb_data_req = PNULL;
    static U8  msg[sizeof(rrc_uecc_llim_srb_data_req_t)+ 10000];
    OSCTXT                           asn1_ctx;
    s1ap_Cause s1apCause;
    x2ap_Cause x2apCause;
    rrc_return_t result=RRC_FAILURE;
    /* SPR_9674_MRO_changes */
    ue_context_info_list_node_t  *p_ue_context_info_list_node = PNULL;
    /* SPR_9674_MRO_changes */
    /* coverity_fix_66601_start */
    list_node_t *p_node = PNULL;
    /* coverity_fix_66601_stop */

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "[uecc_fsm_src_ho_wait_for_srb_data_status_ind_init_handler]"
                "ASN context initialization failed");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return ;
    }

    memset_wrapper(msg, 0, (sizeof(rrc_uecc_llim_srb_data_req_t)+ 10000)/*sizeof(msg)*/);
    memset_wrapper(&s1apCause, 0, sizeof(s1apCause));
    memset_wrapper(&x2apCause, 0, sizeof(x2apCause));

    p_ho_cmd = (p_ue_context->ho_info.p_ho_info->p_src_ho_info->
            s1_src_ho_info.p_ho_command);
    p_srb_data_req = (rrc_uecc_llim_srb_data_req_t*)msg;

    do{
        p_srb_data_req->transaction_id =
            uecc_ue_ctx_get_op_transction_id(p_ue_context);

        /* for inter-RAT, send the stored MobilityFromEutraCommand */
        if ((HANDOVER_TYPE_LTE_TO_UTRAN == p_ue_context->ho_info.p_ho_info->\
                    p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) ||
                (HANDOVER_TYPE_LTE_TO_GERAN == p_ue_context->ho_info.p_ho_info->\
                 p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)||
                (HANDOVER_TYPE_LTE_TO_CDMA2000_1XRTT == p_ue_context->ho_info.p_ho_info->\
                 p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) ||
                (HANDOVER_TYPE_LTE_TO_CDMA2000_HRPD == p_ue_context->ho_info.p_ho_info->\
                 p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) ||
                (HANDOVER_TYPE_LTE_TO_CDMA2000_CONC_1XRTT_HRPD == p_ue_context->ho_info.p_ho_info->\
                 p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) ||
                (HANDOVER_TYPE_CCO == p_ue_context->ho_info.p_ho_info->\
                 p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type))
        {
            result=uecc_src_ho_wait_for_srb_init_lte_to_utran_ho_hdlr(
                    p_ue_context,p_api,p_srb_data_req,&asn1_ctx);
            if(RRC_FAILURE==result)
            {
                /* Free ASN Context */
                rtFreeContext(&asn1_ctx);
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
            } 
        }
        else if(HANDOVER_TYPE_INTRA_LTE_S1 == p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type ||
                HANDOVER_TYPE_INTRA_LTE_X2 == p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type ||
                HANDOVER_TYPE_INTER_CELL == p_ue_context->ho_info.p_ho_info->
/*BUG 604 changes start*/
                p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type ||
                HANDOVER_TYPE_INTRA_CELL == p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
/*BUG 604 changes start*/
        {
            l3_memcpy_wrapper(p_srb_data_req->data_buffer, (OSCTXT*)p_ho_cmd,
                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    s1_src_ho_info.ho_cmd_length);
            
            /* CR 019 changes start*/
            /* Init ASN1 context */
            /*if ( RT_OK != rtInitContext(&asn1_ctx_2) )
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,
                        RRC_ERROR,
                        "uecc_llim_process_ccch_data_ind:"
                        "ASN context initialization failed.");
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
            }*/
            /*get pointer on LLIM message body*/
            /*pu_setBuffer(&asn1_ctx_2, p_srb_data_req->data_buffer,
                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    s1_src_ho_info.ho_cmd_length, ASN_RRC_BUF_TYPE);

            if ( RT_OK != asn1PD_RRCConnectionReconfiguration(&asn1_ctx_2, &rrc_conn_reconf_message) )
            {
              
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,
                        RRC_WARNING,
                        "uecc_fsm_src_ho_wait_for_srb_data_status_ind_init_handler :"
                        "ASN decode failed for inter cell HO rrcconnectionreconf.");
            }*/
	    /* BUG 12647 Fix Start */
            if (((HANDOVER_TYPE_INTER_CELL == p_ue_context->ho_info.p_ho_info->
                        p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) ||
		 ( HANDOVER_TYPE_INTRA_CELL == p_ue_context->ho_info.p_ho_info->
			p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)) && 
                    (PNULL != g_rrc_stats.p_rrc_stats))/* BUG 12647 Fix Stop */
            {
                convert_rrm_cause_to_x2ap_cause(
                        &p_ue_context->ho_info.p_ho_info->
                        p_src_ho_info->s1_src_ho_info.
                        p_rrc_rrm_ho_required->cause, &x2apCause);

		        /*SPR_17284_START*/

        		map_x2ap_cause_to_s1ap_cause(x2apCause, &s1apCause);

		        RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_handover_related_stats.
                        intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause,
                        s1apCause);
        		/*SPR_17284_END*/

                RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_handover_related_stats.
                        intra_enb_ho_stats.ho_IntraEnbOutAtt_sum, 0, 
                        DUMMY_RANGE_CHECK_INDEX); 
            }
            /*SPR_17284_START*/
            else if(HANDOVER_TYPE_INTRA_CELL == p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
            {
                if (PNULL != g_rrc_stats.p_rrc_stats)
                {
                    /*SPR_17284_START*/
        		    convert_rrm_cause_to_x2ap_cause(
                        &p_ue_context->ho_info.p_ho_info->
                        p_src_ho_info->s1_src_ho_info.
                        p_rrc_rrm_ho_required->cause, &x2apCause);

                    map_x2ap_cause_to_s1ap_cause(
                        x2apCause, &s1apCause);
                    /*SPR_17284_END*/

                    RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
                            g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                            p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_handover_related_stats.
                            intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause,
                    /*SPR_17284_START*/
                            s1apCause);
                    /*SPR_17284_END*/

                    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                            g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                            p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_handover_related_stats.
                            intra_enb_ho_stats.ho_IntraEnbOutAtt_sum, 0, 
                            DUMMY_RANGE_CHECK_INDEX); 
                }
            }
            /*SPR_17284_END*/
            /* CR 019 changes stop*/

            p_srb_data_req->data_buffer_length = p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                s1_src_ho_info.ho_cmd_length;

            /* SPR_9674_MRO_changes */
            if(HANDOVER_TYPE_INTRA_LTE_X2 == p_ue_context->ho_info.p_ho_info->
                    p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                               p_ue_context->p_gb_context->facility_name,
                               RRC_INFO,"ue_context_info_list List Size: %d. Max list size=%d.",
                               list_count(&p_ue_context->p_gb_context->ue_context_info_list),
                               p_ue_context->p_gb_context->max_ue_context_list_size);
                p_ue_context_info_list_node = (ue_context_info_list_node_t*)
                    rrc_mem_get(sizeof(ue_context_info_list_node_t));
                if(PNULL != p_ue_context_info_list_node)
                {
                    /* SES-508 Fix Start */
                    /* Code Removed */
                    /* SES-508 Fix End */
                    p_ue_context_info_list_node->ue_ctx_info.ue_index = 
                        p_ue_context->ue_index;
                    p_ue_context_info_list_node->ue_ctx_info.pci = 
                        p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                        p_csc_init_setup_ind->phys_cell_id;
                    p_ue_context_info_list_node->ue_ctx_info.crnti = p_ue_context->crnti;

                    p_ue_context_info_list_node->ue_ctx_info.short_mac_i =
                     p_ue_context->ho_info.p_ho_info->p_src_ho_info->short_mac_I[0].short_mac_i;
                    p_ue_context_info_list_node->ue_ctx_info.ue_context_released = RRC_FALSE;

                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,
                            RRC_INFO,"ue_index:%d pci:%d crnti:%d short_mac_i:%d ue_context_released:%d",
                            p_ue_context_info_list_node->ue_ctx_info.ue_index,
                            p_ue_context_info_list_node->ue_ctx_info.pci,
                            p_ue_context_info_list_node->ue_ctx_info.crnti,
                            p_ue_context_info_list_node->ue_ctx_info.short_mac_i,
                            p_ue_context_info_list_node->ue_ctx_info.ue_context_released);

                    if (p_ue_context->p_gb_context->max_ue_context_list_size > 
                         list_count(&p_ue_context->p_gb_context->ue_context_info_list))
                    {
                        list_push_head(&p_ue_context->p_gb_context->ue_context_info_list,
                                &p_ue_context_info_list_node->node);

                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name,
                                RRC_INFO,"ue_context_info_list inserted. List Size: %d",
                                list_count(&p_ue_context->p_gb_context->ue_context_info_list));
                    }
                    else if (p_ue_context->p_gb_context->max_ue_context_list_size ==
                            list_count(&p_ue_context->p_gb_context->ue_context_info_list))
                    {
                        /*list size has reached its max value. Delete the first node
                         * and Insert the new one */
                        /* coverity_fix_66601_start */
                        p_node = list_pop_tail(&p_ue_context->p_gb_context->ue_context_info_list);
                        if(PNULL != p_node)
                        {
                            list_delete_node(&p_ue_context->p_gb_context->ue_context_info_list, p_node);
                            rrc_mem_free(p_node);
                            /* Coverity ID 85902 Fix Start*/
                            /* p_node = PNULL; */
                            /* Coverity ID 85902 Fix End*/
                            list_push_head(&p_ue_context->p_gb_context->ue_context_info_list,
                                    &p_ue_context_info_list_node->node);
                            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                    p_ue_context->p_gb_context->facility_name,
                                    RRC_INFO,"ue_context_info_list inserted, after deleting the "
                                    "oldest ue_context_info_list node. List Size: %d",
                                    list_count(&p_ue_context->p_gb_context->ue_context_info_list));
                        }   
                        else
                        {
                            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                    p_ue_context->p_gb_context->facility_name,
                                    RRC_WARNING,"ue_context_info_list node not present, received NULL.");
                        }
                        /* coverity_fix_66601_stop */
                    }
                }
                else
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,
                            RRC_WARNING,"memory allocation for ue_context_info_list failed.");
                }
            }
            /* SPR_9674_MRO_changes */
        }
        
	uecc_src_ho_wait_for_srb_init_intra_cell_ho_hdlr(
                p_ue_context,p_srb_data_req); 

        /* TRACE_changes_start */
        /* SPR 16113 Start */
        uecc_rrc_ue_assoc_msg_max_trace_handler(p_ue_context, p_srb_data_req->data_buffer,
                p_srb_data_req->data_buffer_length, "RRC CONNECTION RECONFIGURATION", RRC_DL_DCCH);
        /* SPR 16113 Stop */
        /* TRACE_changes_stop */
        
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_INFO, "RRC_MSG: RRCConnectionReconfiguration(handover) %s",
                change_data_to_str(p_srb_data_req->data_buffer_length, p_srb_data_req->data_buffer));

        if( RRC_SUCCESS != uecc_rrc_send_internal_msg(
                    RRC_LLIM_MODULE_ID,
                    UECC_LLIM_SRB_DATA_REQ,
                    (U16)( p_srb_data_req->data_buffer_length+
                        sizeof(rrc_uecc_llim_srb_data_req_t)),
                    msg))
        {
            
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_INFO, "RRC_MSG: RRCConnectionReconfiguration send fail");
            
            ho_command_sent = RRC_FALSE;
/*BUG 604 changes start*/
            if (HANDOVER_TYPE_CCO !=  p_ue_context->ho_info.p_ho_info->
                    p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
/*BUG 604 changes stop*/
            {
                if(IS_S1U_RECONFIGURED(p_ue_context))
                {
                    /*x2_ho_timer*/
/*BUG 604 changes start*/
                    if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
                       (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
                       (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
/*BUG 604 changes stop*/
                    {
                        /* Stop timer */
                        uecc_ue_timer_stop( p_ue_context->p_gb_context,
                                UECC_UE_X2_HO_OVERALL_TIMER,
                                &p_ue_context->timers_data);
                    }
                    else
                    {
                        /* Stop timer */
                        uecc_ue_timer_stop( p_ue_context->p_gb_context,
                                UECC_UE_HO_OVERALL_TIMER,
                                &p_ue_context->timers_data);
                    }

                    UECC_FSM_SET_CHILD_STATE(p_ue_context,
                            &uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing, PNULL);
                }
                else
                {
                    /*SPR:6462 start*/
                    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                            "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

                    if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                                p_ue_context))
                    {
                        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                                "Failed to send the reestablish complete indication to LLIM");
                    }
                    p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;
                    /*SPR:6462 stop*/
                    break;
                }
            }
            else
            {
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type33_actions);
            }
        }
        if ( RRC_TRUE == ho_command_sent )
        {   /*klockwork Fix*/
            
                p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    ho_command_sent_to_ue = RRC_TRUE;
            /*klockwork Fix*/
            /*CDMA2000_FIX */
            /* Send ue Context Relaese Req to MME in case of ecsfb to 1XRTT
             * or in CONC_1XRTT_HRPD when HRPD fails due to timer expiry or 
             * ho_status is reported as failure from MME */

             if (/* CR 487 changes start*/ 
                    ((HANDOVER_TYPE_LTE_TO_CDMA2000_1XRTT == p_ue_context->ho_info.p_ho_info->\
                    p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) &&
                     (!((RRC_RRM_HO_REQUIRED_HO_FROM_EUTRA_PREPERATION_REQ_PRESENT &
                        p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                        p_rrc_rrm_ho_required->cdma2000_information.bitmask) &&
                      (RRC_RRM_HO_FROM_EUTRA_DUAL_RX_TX_REDIRECT_IND_PRESENT &
                       p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                       p_rrc_rrm_ho_required->cdma2000_information.ho_from_eutra_preperation_req_r10.
                       bitmask) &&
                      (RRC_RRM_HO_FROM_EUTRA_REDIRECT_CARRIER_CDMA2000_1XRTT_PRESENT &
                       p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                       p_rrc_rrm_ho_required->cdma2000_information.ho_from_eutra_preperation_req_r10.
                       bitmask)))) || /* CR 487 changes stop*/
                    ((HANDOVER_TYPE_LTE_TO_CDMA2000_CONC_1XRTT_HRPD == p_ue_context->ho_info.p_ho_info->\
                      p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) 
                     && 
                     (HO_SUCCESS == p_ue_context->cdma2000_info.dl_s1_cdma2000_tunneling_1xrtt.cdma2000_ho_status)
                     &&
                     (RRC_FALSE == p_ue_context->cdma2000_info.timer_expired_f_1xrtt)
                     &&
                     ((HO_FAILURE == p_ue_context->cdma2000_info.dl_s1_cdma2000_tunneling_hrpd.cdma2000_ho_status )
                      || 
                      ( RRC_TRUE == p_ue_context->cdma2000_info.timer_expired_f_hrpd) )))
            {
                p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                p_ue_context->ho_info.cause.u.radioNetwork =
                                             s1ap_redirection_towards_1xRTT;

                 if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                            p_ue_context, &p_ue_context->ho_info.cause))
                 {
                    RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                         "Fail to send the context release request to s1ap");
                 }


            }    

        }
    }while(0);
    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_srb_data_status_ind_resp_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_SRC_HO_EVENT_UE_ADMISSION_RESP from a RRM.
 ****************************************************************************/
static void uecc_fsm_src_ho_wait_for_srb_data_status_ind_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
	/* coverity_fix_64027_start */
	rrc_uecc_llim_srb_data_status_ind_t*
		p_rrc_uecc_llim_srb_data_status_ind = PNULL;


	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api_data);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	p_rrc_uecc_llim_srb_data_status_ind
		= (rrc_uecc_llim_srb_data_status_ind_t*) p_api_data;
	/* setting the flag to is_srb_data_ind_received to RRC_TRUE */
	p_ue_context->ho_info.p_ho_info->p_src_ho_info->
		is_srb_data_ind_received = RRC_TRUE;
	switch (p_rrc_uecc_llim_srb_data_status_ind->response_code)
	{
		case RRC_SUCCESS:
			RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
					"[UECC_LLIM_SRB_DATA_STATUS_IND] SUCCESS indication");
			if(IS_RLF_RECEIVED(p_ue_context))
			{
				RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
						"Event RLF received in unexpected state");
			}
			/*Bug 357 Fix Start*/
			/*BUG 604 changes start*/
#ifdef MEM_PROFILE_ON
			RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,"*******************Memory Usage After Source handover ***");
			qvPoolStats();
#endif
			/*Bug 357 Fix Stop*/
			/*if hrpd case then send s1ap_ue_ctx_rel_req*/
			if (CCO_HO == p_ue_context->ho_info.s1_or_x2_handover)
				/*BUG 604 changes stop*/
			{
				RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,"CCO Handover procedure.");
			}
			else
			{  
				/* CR 487 changes start*/
				if ((HANDOVER_TYPE_LTE_TO_CDMA2000_1XRTT == p_ue_context->ho_info.p_ho_info->
							p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) &&
						(RRC_RRM_HO_REQUIRED_HO_FROM_EUTRA_PREPERATION_REQ_PRESENT & 
						 p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
						 p_rrc_rrm_ho_required->cdma2000_information.bitmask) &&
						(RRC_RRM_HO_FROM_EUTRA_DUAL_RX_TX_REDIRECT_IND_PRESENT &
						 p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
						 p_rrc_rrm_ho_required->cdma2000_information.ho_from_eutra_preperation_req_r10.
						 bitmask) &&
						(RRC_RRM_HO_FROM_EUTRA_REDIRECT_CARRIER_CDMA2000_1XRTT_PRESENT &
						 p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
						 p_rrc_rrm_ho_required->cdma2000_information.ho_from_eutra_preperation_req_r10.
						 bitmask))
				{
					uecc_fsm_finish_activity_success(p_ue_context,
							(void*)UECC_EV_SRC_HO_SUCCESS_INT);
				}
				else
				{/* CR 487 changes stop*/
					UECC_FSM_SET_CHILD_STATE(p_ue_context,
							&uecc_fsm_src_ho_wait_for_ctx_release_cmd, PNULL);
				}
			}
			break;

		case RRC_FAILURE:

			p_ue_context->ho_info.p_ho_info->p_src_ho_info->
				ho_command_sent_to_ue = RRC_FALSE;

			/*BUG 604 changes start*/
			if(CCO_HO != p_ue_context->ho_info.s1_or_x2_handover)
				/*BUG 604 changes stop*/
			{
				if(IS_RLF_RECEIVED(p_ue_context))
				{
					/*BUG 604 changes start*/
					/*x2_ho_timer*/
					if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
							(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
							(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
						/*BUG 604 changes stop*/
					{
						/*SPR_16778_START*/
						p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
						p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
						/*SPR_16778_END*/
						/* stop timer UECC_UE_X2_HO_OVERALL_TIMER */
						uecc_ue_timer_stop(p_ue_context->p_gb_context,
								UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->
								timers_data);
					}
					else
					{
						/*SPR_16778_START*/
						p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
						p_ue_context->ho_info.cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost;
						/*SPR_16778_END*/
						/* stop timer UECC_UE_HO_OVERALL_TIMER */
						uecc_ue_timer_stop(p_ue_context->p_gb_context,
								UECC_UE_HO_OVERALL_TIMER, &p_ue_context->
								timers_data);
					}

					if ( IS_S1U_RECONFIGURED(p_ue_context))
					{
						UECC_FSM_SET_CHILD_STATE(p_ue_context,
								&uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing, 
								PNULL);
					}
					else
					{
						uecc_fsm_start_failure_processing(
								p_ue_context,
								uecc_fsm_src_ho_fail_type19_actions);
					}
				}    
				else if ( IS_S1U_RECONFIGURED(p_ue_context) )
				{
					/* modify the cause in ho_context */
					/*BUG 604 changes start*/
					if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
							(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
							(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
						/*BUG 604 changes stop*/
					{
						p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
						p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 

						uecc_ue_timer_stop( p_ue_context->p_gb_context,
								UECC_UE_X2_HO_OVERALL_TIMER, 
								&p_ue_context->timers_data );
					}
					else
					{
						p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
						p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;

						uecc_ue_timer_stop( p_ue_context->p_gb_context,
								UECC_UE_HO_OVERALL_TIMER, 
								&p_ue_context->timers_data );
					}
					/*x2_ho_timer*/
					/*uecc_ue_timer_stop( UECC_UE_HO_OVERALL_TIMER, 
					  &p_ue_context->timers_data );*/
					UECC_FSM_SET_CHILD_STATE(p_ue_context,
							&uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing,
							PNULL);
					/*SPR_16778_START*/
					/* Continue failure processing */
					uecc_fsm_start_failure_processing(
							p_ue_context,
							uecc_fsm_src_ho_fail_type4_actions);
					/*SPR_16778_END*/
				}
				else 
				{
					/*x2_ho_timer*/
					/* Stop timer */
					/*uecc_ue_timer_stop( UECC_UE_HO_OVERALL_TIMER,
					  &p_ue_context->timers_data);*/

					/*BUG 604 changes start*/
					if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
							(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
							(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
						/*BUG 604 changes stop*/
					{
						uecc_ue_timer_stop( p_ue_context->p_gb_context,
								UECC_UE_X2_HO_OVERALL_TIMER,
								&p_ue_context->timers_data);

						p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
						p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
					}
					else
					{
						uecc_ue_timer_stop( p_ue_context->p_gb_context,
								UECC_UE_HO_OVERALL_TIMER,
								&p_ue_context->timers_data);

						p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
						p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
					}
					/* Process failure */
					/*SPR:6462 start*/
					RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
							"FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

					/*BUG 604 changes start*/
					/*BUG 604 changes start*/
					/*SPR:6462 start*/
					RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
							"FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

					if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
								p_ue_context))
					{
						RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
								"Failed to send the reestablish complete indication to LLIM");
					}
					p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;
					/*SPR:6462 stop*/
					/*BUG 604 changes start*/
				}
			}
			else
			{
				uecc_fsm_start_failure_processing(
						p_ue_context,
						uecc_fsm_src_ho_fail_type33_actions);
			}
			/*BUG 604 changes stop*/
			break;

		default:
			RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
					"[UECC_LLIM_SRB_DATA_STATUS_IND] Invalid Response=%i",
					p_rrc_uecc_llim_srb_data_status_ind->response_code);
			/* considering it as a failure response */
			p_ue_context->ho_info.p_ho_info->p_src_ho_info->
				ho_command_sent_to_ue = RRC_FALSE;

			/*BUG 604 changes start*/
			if(CCO_HO != p_ue_context->ho_info.s1_or_x2_handover)
				/*BUG 604 changes stop*/
			{
				if(IS_RLF_RECEIVED(p_ue_context))
				{
					/* stop timer UECC_UE_HO_OVERALL_TIMER */
					/*x2_ho_timer*/
					/*BUG 604 changes start*/
					if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
							(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
							(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
						/*BUG 604 changes stop*/
					{
						/*SPR_16778_START*/
						p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
						p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
						/*SPR_16778_END*/
						uecc_ue_timer_stop(p_ue_context->p_gb_context,
								UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->
								timers_data);
					}
					else
					{
						/*SPR_16778_START*/
						p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
						p_ue_context->ho_info.cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost;
						/*SPR_16778_END*/
						uecc_ue_timer_stop(p_ue_context->p_gb_context,
								UECC_UE_HO_OVERALL_TIMER, &p_ue_context->
								timers_data);
					}


					if ( IS_S1U_RECONFIGURED(p_ue_context))
					{
						UECC_FSM_SET_CHILD_STATE(p_ue_context,
								&uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing, 
								PNULL);
					}
					else
					{
						uecc_fsm_start_failure_processing(
								p_ue_context,
								uecc_fsm_src_ho_fail_type19_actions);
					}
				}    
				else if ( IS_S1U_RECONFIGURED(p_ue_context) )
				{
					/*x2_ho_timer*/
					/*uecc_ue_timer_stop( UECC_UE_HO_OVERALL_TIMER,
					  &p_ue_context->timers_data );*/
					/*BUG 604 changes start*/
					if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
							(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
							(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
						/*BUG 604 changes stop*/
					{
						/*SPR_16778_START*/
						p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
						p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
						/*SPR_16778_END*/
						uecc_ue_timer_stop(p_ue_context->p_gb_context,
								UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->
								timers_data);
					}
					else
					{
						/*SPR_16778_START*/
						p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
						p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
						/*SPR_16778_END*/
						uecc_ue_timer_stop(p_ue_context->p_gb_context,
								UECC_UE_HO_OVERALL_TIMER, &p_ue_context->
								timers_data);
					}

					UECC_FSM_SET_CHILD_STATE(p_ue_context,
							&uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing,
							PNULL);
					/*SPR_16778_START*/
					/* Continue failure processing */
					uecc_fsm_start_failure_processing(
							p_ue_context,
							uecc_fsm_src_ho_fail_type4_actions);
					/*SPR_16778_END*/
				}
				else
				{
					/*x2_ho_timer*/
					/* Stop timer */
					/*uecc_ue_timer_stop( UECC_UE_HO_OVERALL_TIMER,
					  &p_ue_context->timers_data);*/

					/*BUG 604 changes start*/
					if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
							(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
							(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
						/*BUG 604 changes stop*/
					{
						/* Stop timer */
						uecc_ue_timer_stop( p_ue_context->p_gb_context,
								UECC_UE_X2_HO_OVERALL_TIMER,
								&p_ue_context->timers_data);

						p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
						p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
					}
					else
					{
						/* Stop timer */
						uecc_ue_timer_stop( p_ue_context->p_gb_context,
								UECC_UE_HO_OVERALL_TIMER,
								&p_ue_context->timers_data);

						p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
						p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
					}
					/*BUG 604 changes start*/
					/*SPR:6462 start*/
					RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
							"FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

					if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
								p_ue_context))
					{
						RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
								"Failed to send the reestablish complete indication to LLIM");
					}
					p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;
					/*SPR:6462 stop*/
					/*BUG 604 changes stop*/
				}
			}
			else
			{
				/* Process failure according to selected failure actions list */
				uecc_fsm_start_failure_processing(
						p_ue_context,
						uecc_fsm_src_ho_fail_type33_actions);
			}
			/*BUG 604 changes stop*/
			break;
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* coverity_fix_64027_stop */


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_ctx_release_cmd_timer_expiry_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_wait_for_ctx_release_cmd_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "HO Overall Time out.");

    p_ue_context->ho_info.p_ho_info->ho_status = HO_SUCCESS;

    /* enabling the rrc_connection_release_not_req_flag */
    SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

    /* filling the cause in p_ue_context */
/*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
            (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = 
                                        s1ap_tx2relocoverall_expiry;
    
        /* Send Release Ind to X2AP Module */
        if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
            uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
        }
        
        /* failure action processing */
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_src_ho_fail_type20_actions);
    }
    else
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork =
            s1ap_tS1relocoverall_expiry;

        /* failure action processing */
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_src_ho_fail_type20_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_srb_data_x2ap_error_indication_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_wait_for_srb_data_x2ap_error_indication_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    rrc_x2ap_error_indication_t* p_rrc_x2ap_error_indication= PNULL;
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    p_rrc_x2ap_error_indication
        = (rrc_x2ap_error_indication_t*)p_api;

/*BUG 604 changes start*/
    if((INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
/*BUG 604 changes stop*/
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "X2 HO Error-Indication.");

    p_ue_context->ho_info.p_ho_info->ho_status = HO_SUCCESS;

    /* enabling the rrc_connection_release_not_req_flag */
    SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

    /*Handling this function only if the procedureCode is sn status transfer*/
    /* SPR 16750 Fix Start */
    if ( ((T_x2ap_Cause_radioNetwork &  p_rrc_x2ap_error_indication->cause.t) &&
                ((x2ap_unknown_new_eNB_UE_X2AP_ID &  p_rrc_x2ap_error_indication->cause.u.radioNetwork) ||
                 (x2ap_unknown_old_eNB_UE_X2AP_ID &  p_rrc_x2ap_error_indication->cause.u.radioNetwork) ||
                 (x2ap_unknown_pair_of_UE_X2AP_ID &  p_rrc_x2ap_error_indication->cause.u.radioNetwork))) ||
            (ASN1V_x2ap_id_snStatusTransfer == p_rrc_x2ap_error_indication->criticality_diagnostics.procedureCode)) /* SPR 16750 Fix Stop */

    {
        if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) 
        {
            /* Stop timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_X2_HO_OVERALL_TIMER,
                    &p_ue_context->timers_data );

            /* filling cause */
            p_ue_context->ho_info.x2ap_cause= p_rrc_x2ap_error_indication->cause;

            /* Send Release Ind to X2AP Module */
            uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);

            /* failure action processing */
            uecc_fsm_start_failure_processing(p_ue_context,
                    uecc_fsm_src_ho_fail_type20_actions);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_
 init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* Start timer*/
    do{
        if ( RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context,
                    UECC_UE_LL_GUARD_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type7_actions);
            break;
        }
/*BUG 604 changes start*/
        if(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
            if (RRC_SUCCESS != uecc_llim_build_and_send_s1u_path_switch_req(
                        p_ue_context))
            {
                /* Process failure according to selected failure actions list */
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type7_actions);
            }
        }
        else
        {
/*BUG 604 changes stop*/

        /* Build and sent the SN HFN STATUS transfer request to LLIM */
        if (RRC_SUCCESS != 
                uecc_llim_build_and_send_s1u_delete_forwarding_tunnel_req(
                    p_ue_context))
        {
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type7_actions);
        }
/*BUG 604 changes start*/
        }
/*BUG 604 changes stop*/
    }while(0);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return;
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_s1u_delete_for_forwarding_cnf_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_LLIM_S1U_RECONFIG_CNF from the LLIM.
 ****************************************************************************/
static void uecc_fsm_src_ho_llim_s1u_delete_for_forwarding_cnf_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    rrc_uecc_llim_s1u_reconfigure_cnf_t*
        p_rrc_uecc_llim_s1u_reconfigure_cnf = PNULL;
    uecc_fsm_fail_handler_f const*  p_failure_actions_list =
        uecc_fsm_src_ho_fail_type4_actions;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_uecc_llim_s1u_reconfigure_cnf
        = (rrc_uecc_llim_s1u_reconfigure_cnf_t*)p_api_data;

    /* stop LL Guard timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,  UECC_UE_LL_GUARD_TIMER,
            &p_ue_context->timers_data );
    switch (p_rrc_uecc_llim_s1u_reconfigure_cnf->response_code)
    {
        case RRC_SUCCESS:
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "[UECC_LLIM_S1U_RECONFIG_CNF] S1U Successfully reconfigured " 
                    "for delete forwarding tunnels");
            /* if RE_EST_REQ_RECEIVED */
            if( IS_RE_ESTABLISHMENT_REQ_RECEIVED(p_ue_context) )
            {
                p_failure_actions_list = uecc_fsm_src_ho_fail_type10_actions;                                
                /*SPR:6462 start*/
                
                uecc_fsm_start_failure_processing(p_ue_context,
                        p_failure_actions_list);
                
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
                /*SPR:6462 stop*/
            }
            else if(IS_RLF_RECEIVED(p_ue_context))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                        "Failure action for RLF recieved");
                p_failure_actions_list = uecc_fsm_src_ho_fail_type19_actions;    
                /*SPR:6462 start*/
                
                uecc_fsm_start_failure_processing(p_ue_context,
                        p_failure_actions_list);
                
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
                /*SPR:6462 stop*/
            }
            else if(IS_RRM_HO_CANCEL_RECEIVED(p_ue_context))
            {
                /*SPR:6462 start*/
                p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE28;
                //p_failure_actions_list = uecc_fsm_src_ho_fail_type16_actions;
                /*SPR:6462 stop*/
            }
            else if(IS_RESET_RECEIVED(p_ue_context))
            {
                /*SPR:6462 start*/
                p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE34;
                //p_failure_actions_list = uecc_fsm_src_ho_fail_type34_actions;
                /*SPR:6462 stop*/
                p_ue_context->x2ap_reset_recvd_flag = RRC_FALSE;
            }
            /* SPR 16750 Fix Start */
            else if (p_ue_context->x2ap_context.m.error_indication_ongoing)
            {
                p_failure_actions_list = uecc_fsm_src_ho_fail_type20_actions;    
                
                uecc_fsm_start_failure_processing(p_ue_context,
                        p_failure_actions_list);
                
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
            }
            /* SPR 16750 Fix Stop */
	   /* SPR 19212 Fix Start */
            else
            {
                p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;
            }
	    /* SPR 19212 Fix Stop */
            break;
        case RRC_PARTIAL_SUCCESS:
        case RRC_FAILURE:
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_S1U_RECONFIG_CNF] S1U failure/partial success reconfigured for "
                    "delete forwarding tunnels");
            /*  set rrc_connection_release_not_required_flag */
            SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);
            /*SPR:6462 start*/
            p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE24;
            //p_failure_actions_list = uecc_fsm_src_ho_fail_type24_actions;                      
                /*SPR:6462 stop*/
            if ( IS_RE_ESTABLISHMENT_REQ_RECEIVED(p_ue_context) ) 
            {
                if( RRC_SUCCESS != 
                        uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                            p_ue_context->p_gb_context, p_ue_context->crnti,
                            p_ue_context->reestablishmentCause,
                            p_ue_context->cell_index, RRC_UECCMD_MODULE_ID) )
                {
                    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                            "[UECC_LLIM_S1U_RECONFIG_CNF] FAIL TO SEND RRC " 
                            "Connection ReEstablishment reject to UE"); 
                }
                /*+ SPR 22111 Fix*/
                /*SPR_19778_START*/
                if(PNULL != p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_re_establishment_req)
                {
                    rrc_mem_free(p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_re_establishment_req); 
                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_re_establishment_req = PNULL;
                }
                /*SPR_19778_END*/
                /*- SPR 22111 Fix*/
            }
            else if(IS_RRM_HO_CANCEL_RECEIVED(p_ue_context))
            {
            /*SPR:6462 start*/
                p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE23;
                //    p_failure_actions_list = uecc_fsm_src_ho_fail_type23_actions;
                /*SPR:6462 stop*/
            }
            /* SPR 16750 Fix Start */
            else if (p_ue_context->x2ap_context.m.error_indication_ongoing)
            {
                p_failure_actions_list = uecc_fsm_src_ho_fail_type20_actions;    
                
                uecc_fsm_start_failure_processing(p_ue_context,
                        p_failure_actions_list);
                
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
            }
            /* SPR 16750 Fix Stop */
            break;
        default:
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_S1U_RECONFIG_CNF] INVALID RESPONSE CODE");
            break;
    }
        /*uecc_fsm_start_failure_processing(p_ue_context,
          p_failure_actions_list);*/
    /*SPR:6462 start*/
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

    if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                p_ue_context))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                "Failed to send the reestablish complete indication to LLIM");
    }
    /*SPR:6462 stop*/
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_timer_expiry_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    /*uecc_fsm_fail_handler_f const*         p_failure_actions_list =
        uecc_fsm_src_ho_fail_type21_actions;*/

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "HO Overall Time out.");

    p_ue_context->ho_info.p_ho_info->ho_status = HO_SUCCESS;

    /* enablling the flag */
    SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);
    /* checking whether re_establishment request were received */
    if ( IS_RE_ESTABLISHMENT_REQ_RECEIVED(p_ue_context) ) 
    {
        if( RRC_SUCCESS != 
                uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                    p_ue_context->p_gb_context, p_ue_context->crnti,
                    p_ue_context->reestablishmentCause,
                    p_ue_context->cell_index, RRC_UECCMD_MODULE_ID) )
        {

            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR, "Fail to send the rrc connection " 
                    "reestablishment reject to UE");
        }
    }
    /*SPR_16778_START*/
    if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
		    (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
		    (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
	    /*BUG 604 changes stop*/
    {
	    p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
	    p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
    }
    /*SPR_16778_END*/
    /*SPR:6462 start*/
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

    if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                p_ue_context))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                "Failed to send the reestablish complete indication to LLIM");
    }
    p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE21;
    /*SPR:6462 stop*/
    /*uecc_fsm_start_failure_processing(p_ue_context,
            p_failure_actions_list);*/
    /* SPR 16750 Fix Start */
    /* Code Removed */
    /* SPR 16750 Fix Stop */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/*SPR_19066_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_get_ho_prep_info_ongoing_csc_cell_delete_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void uecc_fsm_src_ho_get_ho_prep_info_ongoing_csc_cell_delete_req_handler(
/*SPR_19066_END*/
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*x2_ho_timer*/
    /* stopping timer UECC_UE_HO_PREP_TIMER */
    /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);*/

    /* filling the cause in p_ue_context */
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        /* stopping timer UECC_UE_X2_HO_PREP_TIMER */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
        /* filling the cause in p_ue_context */
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        /*SPR_17484_START*/
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_cell_not_available;
        /*SPR_17484_END*/
    }
    else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        /* stopping timer UECC_UE_HO_PREP_TIMER */
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
        /* filling the cause in p_ue_context */
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        /*SPR_17484_START*/
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_cell_not_available;
        /*SPR_17484_END*/
    }
    /*BUG 604 changes start*/
    /*else if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
             UECC_UE_INTRA_CELL_HO_GUARD_TIMER, &p_ue_context->timers_data);
    }*/
    /*BUG 604 changes stop*/

    /* updating ho_status */
    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type11_actions);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_prep_ongoing_csc_cell_delete_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void uecc_fsm_src_ho_prep_ongoing_csc_cell_delete_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /*x2_ho_timer*/
    /* stopping timer UECC_UE_HO_PREP_TIMER */
    /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);*/
    /* updating ho_status */
    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;

    /* filling the cause in p_ue_context */
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        /*x2_ho_timer*/
        /* stopping timer UECC_UE_X2_HO_PREP_TIMER */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
        /* filling the cause in p_ue_context */
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        /*SPR_17484_START*/
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_cell_not_available;
        /*SPR_17484_END*/
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type29_actions);
    }
    else
    {
        /*x2_ho_timer*/
        /* stopping timer UECC_UE_HO_PREP_TIMER */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
        /* filling the cause in p_ue_context */
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        /*SPR_17484_START*/ 
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_cell_not_available;
        /*SPR_17484_END*/
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type11_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_csc_cell_delete_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_csc_cell_delete_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*x2_ho_timer*/
    /* stopping UECC_UE_HO_OVERALL_TIMER timer */
    /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);*/

    /* filling the cause in p_ue_context */
    /*BUG 604 changes start*/
    /*SPR_17484_START*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        /*x2_ho_timer*/
        /* stopping UECC_UE_X2_HO_OVERALL_TIMER timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_cell_not_available;
    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type29_actions);
    }
    else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        /*x2_ho_timer*/
        /* stopping UECC_UE_HO_OVERALL_TIMER timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_cell_not_available;
    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type11_actions);
    }
    /*BUG 604 changes start*/
    /*else if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                 UECC_UE_INTRA_CELL_HO_GUARD_TIMER, &p_ue_context->timers_data);
    }*/
    /*BUG 604 changes stop*/
    else if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
               UECC_UE_CCO_GUARD_TIMER, &p_ue_context->timers_data);
    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type11_actions);
    }
    /*SPR_17484_END*/
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_ctx_rel_cmd_csc_cell_delete_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void uecc_fsm_src_ho_wait_for_ctx_rel_cmd_csc_cell_delete_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*x2_ho_timer*/
    /* stopping the UECC_UE_HO_OVERALL_TIMER timer */
    /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);*/

    /* filling the cause in p_ue_context */
/*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        /*x2_ho_timer*/
        /* stopping the UECC_UE_X2_HO_OVERALL_TIMER timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        /*SPR_17484_START*/
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_cell_not_available; 
        /*SPR_17484_END*/
    }
    else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        /*x2_ho_timer*/
        /* stopping the UECC_UE_HO_OVERALL_TIMER timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        /*SPR_17484_START*/
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_cell_not_available;
        /*SPR_17484_END*/
    }
/*BUG 604 changes start*/
    /*else if(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) 
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_INTRA_CELL_HO_GUARD_TIMER, 
                &p_ue_context->timers_data);
    }*/
/*BUG 604 changes start*/
    else if(CCO_HO == p_ue_context->ho_info.s1_or_x2_handover) 
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_CCO_GUARD_TIMER, 
                &p_ue_context->timers_data);
    }
    
    /* updating the ho_status */
    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;
/*BUG 604 changes start*/
    /*if(INTRA_CELL_HO != p_ue_context->ho_info.s1_or_x2_handover)
    {
        SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);
    }*/
/*BUG 604 changes stop*/

    if( IS_RE_ESTABLISHMENT_REQ_RECEIVED(p_ue_context))
    {
        if (RRC_SUCCESS != 
                uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                    p_ue_context->p_gb_context, p_ue_context->crnti,
                    p_ue_context->reestablishmentCause,
                    p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR, "Fail to send rrc " 
                    "connection rejection message to UE.");
        }
    }

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type11_actions);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_delete_fwding_tunnel_csc_cell_delete_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void uecc_fsm_src_ho_delete_fwding_tunnel_csc_cell_delete_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* stopping timer UECC_UE_LL_GUARD_TIMER */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_LL_GUARD_TIMER, &p_ue_context->timers_data);

    /* filling the cause in p_ue_context */
/*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        /*SPR_17484_START*/
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_cell_not_available; 
        /*SPR_17484_END*/
    }
    else
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        /*SPR_17484_START*/
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_cell_not_available;
        /*SPR_17484_END*/
    }
    /* updating the ho_status */
    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;

    SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);
    if( IS_RE_ESTABLISHMENT_REQ_RECEIVED(p_ue_context))
    {
        if (RRC_SUCCESS != 
                uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                p_ue_context->p_gb_context, p_ue_context->crnti,
                p_ue_context->reestablishmentCause,
                p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR, "Fail to send rrc " 
                    "connection rejection message to UE.");
        }
    }

    /* Bug 4450 Start */
    /* Release forwarding tunnels during CELL delete  */
    if (RRC_SUCCESS !=
            uecc_llim_build_and_send_s1u_delete_forwarding_tunnel_req(p_ue_context))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
            "Fail to send s1u delete forwarding tunnel request message.");
    }
    /* Bug 4450 Stop */

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type11_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/*SPR_19066_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_get_ho_prep_info_ongoing_ue_release_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void  uecc_fsm_src_ho_get_ho_prep_info_ongoing_ue_release_req_handler(
/*SPR_19066_END*/
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;
    /* storing ue_ctx_rel_cmd message */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd 
        = (rrc_s1ap_ue_context_release_command_t*)
        rrc_mem_get(sizeof(rrc_s1ap_ue_context_release_command_t));
    /* SES-508 Fix Start */
    /* Code Removed */
    /* SES-508 Fix End */
    l3_memcpy_wrapper(p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd,
            p_api,
            sizeof(rrc_s1ap_ue_context_release_command_t));
    /* filling cause for failure */
    p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;

    /* checking for timer to stop */
    /*x2_ho_timer*/
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
            (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
        /*BUG 604 changes stop*/
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
    }
    else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
    }
    /*BUG 604 changes start*/
    /*else if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
      {
      uecc_ue_timer_stop(p_ue_context->p_gb_context,
      UECC_UE_INTRA_CELL_HO_GUARD_TIMER, &p_ue_context->timers_data);
      }*/
    /*BUG 604 changes stop*/
    /*SPR 21947 Start */
    if(PNULL != p_api)
    {
        rrc_mem_free(p_api);
        p_api = PNULL; 
    }

    /*SPR 21947 Stop */
    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type12_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_prep_ue_release_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void  uecc_fsm_src_ho_prep_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;
    /* storing ue_ctx_rel_cmd message */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd 
        = (rrc_s1ap_ue_context_release_command_t*)
        rrc_mem_get(sizeof(rrc_s1ap_ue_context_release_command_t));
    /* SES-508 Fix Start */
    /* Code Removed */
    /* SES-508 Fix End */
    l3_memcpy_wrapper(p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd,
            p_api,
            sizeof(rrc_s1ap_ue_context_release_command_t));

    /*x2_ho_timer*/
    /* checking for timer to stop */
    /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);*/

    /* filling the cause in p_ue_context */
    /*BUG 604 changes stop*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        /*x2_ho_timer*/
        /* checking for timer to stop */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
        /* filling cause for failure */
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type31_actions);
    }
    else
    {
        /*x2_ho_timer*/
        /* checking for timer to stop */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
        /* filling cause for failure */
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
        
       /*SPR 21947 Start */
        if(PNULL != p_api)
        {
            rrc_mem_free(p_api);
            p_api = PNULL; 
        }
        
       /*SPR 21947 Stop */
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type12_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_rrm_ho_cmd_enb_status_s1u_reconfig_wait_for_srb_data_ue_release_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void  uecc_fsm_src_ho_rrm_ho_cmd_enb_status_s1u_reconfig_wait_for_srb_data_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* checking for timer to stop */

    /*x2_ho_timer*/
    /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);*/

    /* storing ue_ctx_rel_cmd message */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd
        = (rrc_s1ap_ue_context_release_command_t*)
        rrc_mem_get(sizeof(rrc_s1ap_ue_context_release_command_t));
    /* SES-508 Fix Start */
    /* Code Removed */
    /* SES-508 Fix End */
    l3_memcpy_wrapper(p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd,
            p_api,
            sizeof(rrc_s1ap_ue_context_release_command_t));

    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;


    /* Send HO CANCEL to Target eNB in case of X2AP HO */
    /* Send HO CANCEL to Target cell in case of INTER CELL HO */
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        /*x2_ho_timer*/
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                "FAILURE: Send HO CANCEL to X2AP");

        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;

        if (RRC_SUCCESS != uecc_x2ap_build_and_send_handover_cancel(p_ue_context))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                    "Fail to send the handover cancel to X2AP");
        }
    }
    else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        /*x2_ho_timer*/
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);
    }
    /*BUG 604 changes start*/
    /*else if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                 UECC_UE_INTRA_CELL_HO_GUARD_TIMER, &p_ue_context->timers_data);
    }*/
    /*BUG 604 changes stop*/
    else if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                 UECC_UE_CCO_GUARD_TIMER, &p_ue_context->timers_data);
    }

    /* filling the cause in p_ue_context */
    p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;

   /*SPR 21947 Start */
    if(PNULL != p_api)
    {
        rrc_mem_free(p_api);
        p_api = PNULL; 
    }

   /*SPR 21947 Stop */
    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type12_actions);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_ue_release_intra_lte_ho_adm_re_ho_freq_present
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request
 ****************************************************************************/

static void uecc_ue_release_intra_lte_ho_adm_re_ho_freq_present(
                uecc_ue_context_t*  p_ue_context)
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    if ( HANDOVER_INTRA_FREQ == p_ue_context->ho_info.p_ho_info->p_src_ho_info->
            s1_src_ho_info.p_rrc_rrm_ho_required->ho_freq_type )
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.
                intra_freq_ho_stats.ho_IntraFreqOutSucc, RRC_NULL,
                DUMMY_RANGE_CHECK_INDEX);
    }
    else
    {
        /* Bug 881 Fix Start */
	/* SPR_17047_START */
        if ((p_ue_context->curr_meas_config.m.curr_meas_gap_config_present) &&
            (1 == p_ue_context->curr_meas_config.curr_meas_gap_config.meas_gap_config_type))
        /* SPR_17047_END */
        /* Bug 881 Fix Stop */
        {
            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.
                    inter_freq_ho_stats.ho_InterFreqMeasGapOutSucc,
                    RRC_NULL, DUMMY_RANGE_CHECK_INDEX);
        }
        else
        {
            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.
                    inter_freq_ho_stats.ho_InterFreqNoMeasGapOutSucc,
                    RRC_NULL, DUMMY_RANGE_CHECK_INDEX);

        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_ue_rel_intra_lte_ho_update_performance_counter
 * Inputs         : uecc_ue_context_t*     -     pointer to UE context
 *                  s1ap_Cause                   s1apCause
 *                  Unsigned character           index
 *                  Signed integer               cmp_result
 * Outputs        : None
 * Returns        : None
 * Description    : handler for some of the update performance counters
 ****************************************************************************/

static void  uecc_ue_rel_intra_lte_ho_update_performance_counter(
        uecc_ue_context_t*  p_ue_context,
        s1ap_Cause          s1apCause,
        U8                  index,
        S32                 cmp_result)
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    if ((PNULL != (&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[index])) && (cmp_result == RRC_NULL))
    {
        RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[index].ho_OutSuccTarget_cause.
                ho_OutSuccTarget_Cause, s1apCause);
    }
    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_handover_related_stats.inter_enb_ho_stats.
            ho_InterEnbOutSucc_sum, 0, DUMMY_RANGE_CHECK_INDEX);

    if ((PNULL != (&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[index])) && (cmp_result == RRC_NULL))
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[index].ho_OutSuccTarget_sum,
                0, DUMMY_RANGE_CHECK_INDEX);
    }

    if ((RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT &
                p_ue_context->mac_config.bitmask) &&
            (RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT &
            /* Bug 880 Fix Start */
            p_ue_context->mac_config.mac_main_config.bitmask) &&
            (RRC_DRX_CONFIG_TYPE_SETUP == p_ue_context->
            mac_config.mac_main_config.drx_config.drx_config_type))
            /* Bug 880 Fix Stop */
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.drx_non_drx_ho_stats.ho_DrxOutSucc,
                RRC_NULL, DUMMY_RANGE_CHECK_INDEX);

    }
    else
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.drx_non_drx_ho_stats.ho_NoDrxOutSucc,
                RRC_NULL, DUMMY_RANGE_CHECK_INDEX);

    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_ue_release_intra_lte_ho_hndlr
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void  uecc_ue_release_intra_lte_ho_hndlr(
        uecc_ue_context_t*  p_ue_context
        )
{
    s1ap_Cause   s1apCause;
    S32 cmp_result = -1;
    U8  index = 0;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /* SPR_7888_fix */
    if(RRC_SUCCESS != convert_rrm_cause_to_s1ap_cause(
                &p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->cause, &s1apCause))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                           p_ue_context->p_gb_context->facility_name, 
                           RRC_WARNING, "Invalid Cause received from RRM");
    }
    /* SPR_7888_fix */

    /* SPR 15927 Start */
    if((PNULL != p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd) &&
        (T_s1ap_Cause_radioNetwork ==
         ((rrc_s1ap_ue_context_release_command_t *)(p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd))->cause.t) && 
            (s1ap_successful_handover ==
             (((rrc_s1ap_ue_context_release_command_t *)(p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd))->cause.u.radioNetwork)))
    {
    RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause, s1apCause);
    }
    /* SPR 15927 Stop */

    /*Comparing the terget ID onorder to get the index at which the stats were
     *       stored in rrc_oam_meas_on_neigh_cell_ho_stats_t at the time of ho
     *       attempt */
    for (index = 0; index < (g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count);
            index++)
    {
        cmp_result = memcmp_wrapper(&g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                /*SPR_19166_START*/
        /*CID 110084 Fix Start*/
                meas_on_neigh_cell_ho_stats[index].ecgi.plmn_identity,
                &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.plmn_identity, 
                sizeof(plmn_identity_t));
                /*SPR_19166_END*/

        if (RRC_NULL == cmp_result)
        {
            cmp_result = memcmp_wrapper(&g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[index].ecgi.cell_identity,
                    &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                    p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.cell_identity, 
                    MAX_CELL_IDENTITY_NUMBYTES);
            /*SPR_19166_END*/
            if (RRC_NULL == cmp_result)
            {
                break;
            }
        }
        /*CID 110084 Fix Stop*/
    }
 
    /* SPR 15927 Start */
    if((PNULL != p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd) &&
            (T_s1ap_Cause_radioNetwork ==
             (((rrc_s1ap_ue_context_release_command_t *)(p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd))->cause.t)) && 
            (s1ap_successful_handover ==
             (((rrc_s1ap_ue_context_release_command_t *)(p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd))->cause.u.radioNetwork)))
    {
 
        /*SPR_17462_START*/
        if(cmp_result != RRC_NULL)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                    p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                    "S1 HO Target Id not found. meas_on_neigh_cell_ho_stats_list.count = %u",
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count);
        }
        /*SPR_17462_END*/
        uecc_ue_rel_intra_lte_ho_update_performance_counter(p_ue_context,s1apCause,index,cmp_result);

    if (( RRC_RRM_UE_HO_ADM_RE_HO_FREQ_TYPE_PRESENT &
                p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->bitmask))
    {
        uecc_ue_release_intra_lte_ho_adm_re_ho_freq_present(p_ue_context);
    }
    else
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.
                intra_freq_ho_stats.ho_IntraFreqOutSucc, RRC_NULL,
                DUMMY_RANGE_CHECK_INDEX);
    }
    }
    /* SPR 15927 Stop */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_ctx_rel_cmd_ue_release_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void  uecc_fsm_src_ho_wait_for_ctx_rel_cmd_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    s1ap_Cause   s1apCause;
    /* BUG 12662 Fix Start */
    S32 cmp_result = -1;
    U8  index = 0;
    /* BUG 12662 Fix Stop */
 
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    memset_wrapper(&s1apCause, 0, sizeof(s1apCause));

    /* checking for timer to stop */

    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);
    p_ue_context->ho_info.p_ho_info->ho_status = HO_SUCCESS;
    /* enabling flag */
    SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

/*BUG 604 changes start*/
    /* SPR Fix 8319 */
    /* In case of Intra Cell HO, since the RRCConnectionReconfigurationComplete
     * is received at Src Enb only, RRCConnectionRelease will be triggerred from
     * Src Enb only*/
    /* SPR Fix 8319 */
/*BUG 604 changes stop*/

    /* storing ue_ctx_rel_cmd message */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd
        = (rrc_s1ap_ue_context_release_command_t*)
        rrc_mem_get(sizeof(rrc_s1ap_ue_context_release_command_t));
    /* SES-508 Fix Start */
    /* Code Removed */
    /* SES-508 Fix End */
    l3_memcpy_wrapper(p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd,
            p_api,
            sizeof(rrc_s1ap_ue_context_release_command_t));

    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
      if(HANDOVER_TYPE_INTRA_LTE_S1 == p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
      {
		uecc_ue_release_intra_lte_ho_hndlr(p_ue_context);
      }
      else if ((HANDOVER_TYPE_LTE_TO_UTRAN == p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) ||
            (HANDOVER_TYPE_LTE_TO_GERAN == p_ue_context->ho_info.p_ho_info->
             p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type))
      {
        /* SPR_7888_fix */
        if(RRC_SUCCESS != convert_rrm_cause_to_s1ap_cause(
                &p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->cause, &s1apCause))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                           p_ue_context->p_gb_context->facility_name, 
                           RRC_WARNING, "Invalid Cause received from RRM");
        }
        /* SPR_7888_fix */
	    /* BUG 12662 Fix Start */
        /* Checking whether ho attempt for the particular target ID
        * already present or not . If yes, then incrementing the count
        * for this new ho request for this same target ID */
    	for (index = RRC_NULL; index < (g_rrc_stats.p_rrc_stats->
		    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
    		p_gb_context->offset_uecc_mod_id].
	    	rrc_uecc_cell_stats[p_ue_context->cell_index].
    		rrc_handover_related_stats.inter_rat_ho_stats_list.count);
	    	index++) 
	    {
                /*SPR_19166_START*/
                cmp_result = rrc_compare_inter_rat_target_cgi( 
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.inter_rat_ho_stats_list.
                    inter_rat_ho_stats[index].target_cgi,
                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                    p_rrc_rrm_ho_required);
                /*SPR_19166_END*/

                if (RRC_NULL == cmp_result)
                {
                    break;
                }
	    }    	

	/*SPR_17462_START*/
	if (cmp_result != RRC_NULL)
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
				p_ue_context->p_gb_context->facility_name, RRC_WARNING,
				"S1 HO Target Id not found. inter_rat_ho_stats_list.count = %u",
				g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
				rrc_uecc_instance_stats[p_ue_context->
				p_gb_context->offset_uecc_mod_id].
				rrc_uecc_cell_stats[p_ue_context->cell_index].
				rrc_handover_related_stats.inter_rat_ho_stats_list.count);
	}
    /*SPR 14773 Fix Start*/
    /*SPR_16595_START*/
    if (cmp_result == RRC_NULL)
        /*SPR_17462_END*/
    {
        /*SPR_16595_START*/
        /*SPR 14773 Fix Start*/
        /* SPR 15927 Start */
        if ( (T_s1ap_Cause_radioNetwork == ((rrc_s1ap_ue_context_release_command_t *)p_api)->cause.t) && 
                (s1ap_successful_handover == ((rrc_s1ap_ue_context_release_command_t *)p_api)->cause.u.radioNetwork))
        {
            /* SPR 15927 Stop */
            /*SPR 14773 Fix Stop*/

            if(RRC_MAX_CONNECTION > g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.inter_rat_ho_stats_list.count)
            {
                RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].
                        rrc_handover_related_stats.inter_rat_ho_stats_list.count,
                        0, DUMMY_RANGE_CHECK_INDEX);

                index = (U8)(g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].
                        rrc_handover_related_stats.inter_rat_ho_stats_list.count - 1);
               
                /* CID 111524 Fix Start */
                rrc_fill_inter_rat_cgi_info(
                        &g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].
                        rrc_handover_related_stats.inter_rat_ho_stats_list.
                        inter_rat_ho_stats[index].target_cgi,
                        p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                        p_rrc_rrm_ho_required);
            
                /* CID 111524 Fix Stop */
            }
            else
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[%s]: Maximum target Id count reached. L3 is ignoring "
                        "performance counters for this target Id.",__FUNCTION__);
            }
        }
        /*SPR_16595_END*/
    }
    /*SPR 14773 Fix Start*/
    /*SPR_16595_START*/
    if(RRC_MAX_CONNECTION > index)
    {
        if ( (T_s1ap_Cause_radioNetwork == ((rrc_s1ap_ue_context_release_command_t *)p_api)->cause.t) && 
                (s1ap_successful_handover == ((rrc_s1ap_ue_context_release_command_t *)p_api)->cause.u.radioNetwork))
        {
            /*SPR_16595_END*/
            /*SPR 14773 Fix Stop*/

            RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.inter_rat_ho_stats_list.
                    inter_rat_ho_stats[index].ho_IratOutSucc_cause.ho_IratOutSucc_Cause,
                    s1apCause);

            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.inter_rat_ho_stats_list.
                    inter_rat_ho_stats[index].ho_IratOutSucc_sum, 0, 
                    DUMMY_RANGE_CHECK_INDEX);
            /* BUG 12662 Fix Stop */
            /*SPR 14773 Fix Start*/
        }
        /*SPR 14773 Fix Stop*/
        /*SPR_16595_START*/
    }
    /*SPR_16595_END*/
      }
    }

   /*SPR 21947 Start */
    if(PNULL != p_api)
    {

        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                        p_ue_context->p_gb_context->facility_name, RRC_DETAILED,"value of p_api[%d]",p_api);
        rrc_mem_free(p_api);
        p_api = PNULL; 
    }
  /*SPR 21947 Stop */
    /* SPR Fix 8319 */
    /*BUG 604 changes start*/
    /*BUG 604 changes stop*/
         /* finish fsm successfully */
         uecc_fsm_finish_activity_success(p_ue_context,PNULL);
    //}
    /* SPR Fix 8319 */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/******************************************************************************
 * Function Name  : uecc_ue_rel_req_intra_lte_x2_hndlr_amd_re_ho_freq_present
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : x2ap ue context release request for intra lte x2 handover
 ******************************************************************************/

static void  uecc_ue_rel_req_intra_lte_x2_hndlr_amd_re_ho_freq_present(
             uecc_ue_context_t*  p_ue_context)
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    if ( HANDOVER_INTRA_FREQ == p_ue_context->ho_info.p_ho_info->p_src_ho_info->
            s1_src_ho_info.p_rrc_rrm_ho_required->ho_freq_type )
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.
                intra_freq_ho_stats.ho_IntraFreqOutSucc, RRC_NULL,
                DUMMY_RANGE_CHECK_INDEX);
    }
    else
    {
        /* Bug 881 Fix Start */
        /* SPR_17047_START */
        if ((p_ue_context->curr_meas_config.m.curr_meas_gap_config_present) &&
           (1 == p_ue_context->curr_meas_config.curr_meas_gap_config.meas_gap_config_type))
         /*SPR_17047_END */
	/* Bug 881 Fix Stop */
        {
            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.
                    inter_freq_ho_stats.ho_InterFreqMeasGapOutSucc,
                    RRC_NULL, DUMMY_RANGE_CHECK_INDEX);
        }
        else
        {
            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.
                    inter_freq_ho_stats.ho_InterFreqNoMeasGapOutSucc,
                    RRC_NULL, DUMMY_RANGE_CHECK_INDEX);
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_ue_rel_intra_lte_x2_ho_update_performance_counter
 * Inputs         : uecc_ue_context_t*     -     pointer to UE context
 *                  s1ap_Cause                   s1apCause
 *                  Unsigned character           index
 *                  Signed integer               cmp_result
 * Outputs        : None
 * Returns        : None
 * Description    : handler for some of the update performance counters
 ****************************************************************************/

static void uecc_ue_rel_intra_lte_x2_ho_update_performance_counter(
        uecc_ue_context_t*  p_ue_context,
        x2ap_Cause          x2apCause,
        U8                  index,
        S32                 cmp_result)
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    if ((PNULL != (&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[index]) && (cmp_result == RRC_NULL)))
    {
        /*SPR_17284_START*/
        s1ap_Cause s1apCause;
        memset_wrapper(&s1apCause, 0, sizeof(s1apCause));
        map_x2ap_cause_to_s1ap_cause(
                x2apCause, &s1apCause);

        RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[index].ho_OutSuccTarget_cause.
                ho_OutSuccTarget_Cause, s1apCause);
        /*SPR_17284_END*/
    }

    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_handover_related_stats.inter_enb_ho_stats.
            ho_InterEnbOutSucc_sum, 0, DUMMY_RANGE_CHECK_INDEX);

    if ((PNULL != (&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[index])) && (cmp_result == RRC_NULL))
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                meas_on_neigh_cell_ho_stats[index].ho_OutSuccTarget_sum,
                0, DUMMY_RANGE_CHECK_INDEX);
    }

    if ((RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT &
                p_ue_context->mac_config.bitmask) &&
            (RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT &
            /* Bug 880 Fix Start */
            p_ue_context->mac_config.mac_main_config.bitmask) &&
            (RRC_DRX_CONFIG_TYPE_SETUP == p_ue_context->
            mac_config.mac_main_config.drx_config.drx_config_type))
            /* Bug 880 Fix Stop */
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.drx_non_drx_ho_stats.ho_DrxOutSucc,
                RRC_NULL, DUMMY_RANGE_CHECK_INDEX);
    }
    else
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.drx_non_drx_ho_stats.ho_NoDrxOutSucc,
                RRC_NULL, DUMMY_RANGE_CHECK_INDEX);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/******************************************************************************
 * Function Name  : uecc_ue_release_req_intra_lte_x2_hndlr
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : x2ap ue context release request for intra lte x2 handover
 ******************************************************************************/

static void  uecc_ue_release_req_intra_lte_x2_hndlr(
        uecc_ue_context_t*  p_ue_context)
{
    U8 index = 0;
    S32 cmp_result = -1;
    x2ap_Cause x2apCause;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    memset_wrapper(&x2apCause, 0, sizeof(x2apCause));

	convert_rrm_cause_to_x2ap_cause(
                &p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->cause, &x2apCause);

	/*SPR_17284_START*/
    s1ap_Cause s1apCause;
    memset_wrapper(&s1apCause, 0, sizeof(s1apCause));

	map_x2ap_cause_to_s1ap_cause(
					x2apCause, &s1apCause);
    RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutSucc_cause.
            ho_InterEnbOutSucc_Cause, s1apCause);
	/*SPR_17284_END*/

    /*Comparing the terget ID onorder to get the index at which the stats were
     * stored in rrc_oam_meas_on_neigh_cell_ho_stats_t at the time of ho attempt */
    for (index = 0; index < (g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count);
            index++)
    {
        cmp_result = memcmp_wrapper(&g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                /*SPR_19166_START*/
        /*CID 110086 Fix Start*/
                meas_on_neigh_cell_ho_stats[index].ecgi.plmn_identity,
                &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.plmn_identity, 
                sizeof(plmn_identity_t));
                /*SPR_19166_END*/

        if (RRC_NULL == cmp_result)
        {
            cmp_result = memcmp_wrapper(&g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    meas_on_neigh_cell_ho_stats[index].ecgi.cell_identity,
                    &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                    p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.cell_identity, 
                    MAX_CELL_IDENTITY_NUMBYTES);
            /*SPR_19166_END*/
            if (RRC_NULL == cmp_result)
            {
                break;
            }
        }
        /*CID 110086 Fix Stop*/
    }

    /*SPR_17462_START*/
    if(cmp_result != RRC_NULL)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                "X2 HO Target Id not found. meas_on_neigh_cell_ho_stats_list.count = %u",
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count);
    }
    /*SPR_17462_END*/
    uecc_ue_rel_intra_lte_x2_ho_update_performance_counter(p_ue_context,x2apCause,index,cmp_result);

    if (( RRC_RRM_UE_HO_ADM_RE_HO_FREQ_TYPE_PRESENT &
                p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->bitmask))
    {
        uecc_ue_rel_req_intra_lte_x2_hndlr_amd_re_ho_freq_present(p_ue_context);
    }
    else
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_handover_related_stats.
                intra_freq_ho_stats.ho_IntraFreqOutSucc, RRC_NULL,
                DUMMY_RANGE_CHECK_INDEX);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/******************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_x2ap_ctx_rel_ue_release_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for x2ap ue context release 
 ******************************************************************************/

static void  uecc_fsm_src_ho_wait_for_x2ap_ctx_rel_ue_release_req_handler(
	uecc_ue_context_t*  p_ue_context,
	void*               p_api
	)
{
    /* BUG_12786 start */
    s1ap_Cause s1apcause;
    /* BUG_12786 stop */
    /* BUG 12662 Fix Start */
    U8 index = 0;
    S32 cmp_result = -1;
    /* BUG 12662 Fix Stop */
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* BUG_12786 start */
    memset_wrapper(&s1apcause, 0, sizeof(s1apcause));
    /* SPR_17377_Warning_fix + */
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
    /* SPR_17377_Warning_fix - */
            p_api);
    /* BUG_12786 stop */

    /* checking for timer to stop */
    /*x2_ho_timer*/
    /*BUG 604 changes start*/
    if ((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
	    (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
	    (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
	/*BUG 604 changes stop*/
    {
	uecc_ue_timer_stop(p_ue_context->p_gb_context, 
		UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);
    }
    else
    {
	uecc_ue_timer_stop(p_ue_context->p_gb_context, 
		UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);
    }

    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
	if(HANDOVER_TYPE_INTRA_LTE_X2 == p_ue_context->ho_info.p_ho_info->
		p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
	{
	    uecc_ue_release_req_intra_lte_x2_hndlr(p_ue_context);	
	}/* BUG 12647 Fix Start */
	else if (( HANDOVER_TYPE_INTRA_CELL == p_ue_context->ho_info.p_ho_info->
		    p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type ) ||
		( HANDOVER_TYPE_INTER_CELL == p_ue_context->ho_info.p_ho_info->
		  p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type )) 
	{
	    uecc_ue_release_req_lte_intra_enb_ho_hndlr(p_ue_context);	 
	}/* BUG 12647 Fix Stop */
	else if ((HANDOVER_TYPE_LTE_TO_UTRAN == p_ue_context->ho_info.p_ho_info->
		    p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) ||
		(HANDOVER_TYPE_LTE_TO_GERAN == p_ue_context->ho_info.p_ho_info->
		 p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type))
	{
	    /* BUG_12786 start */
	    /* coverity_fix_67003_start */
	    if(RRC_SUCCESS != convert_rrm_cause_to_s1ap_cause(
			&p_ue_context->ho_info.p_ho_info->
			p_src_ho_info->s1_src_ho_info.
			p_rrc_rrm_ho_required->cause, &s1apcause))
	    {
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
			p_ue_context->p_gb_context->facility_name, 
			RRC_WARNING, "Invalid Cause received from RRM");
	    }
	    /* coverity_fix_67003_stop */
	    /* BUG_12786 stop */
	    /* BUG 12662 Fix Start */

	    /* Comparing the terget ID onorder to get the index at which the stats were
	     * stored in rrc_oam_meas_on_neigh_cell_ho_stats_t at the time of ho
	     * attempt */
	    for (index = 0; index < (g_rrc_stats.p_rrc_stats->
			rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
			rrc_uecc_cell_stats[p_ue_context->cell_index].
			rrc_handover_related_stats.inter_rat_ho_stats_list.count);
		    index++)
	    {
                /*SPR_19166_START*/
                cmp_result = rrc_compare_inter_rat_target_cgi( 
                        g_rrc_stats.p_rrc_stats->
			rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
			p_gb_context->offset_uecc_mod_id].
			rrc_uecc_cell_stats[p_ue_context->cell_index].
			rrc_handover_related_stats.inter_rat_ho_stats_list.
                        inter_rat_ho_stats[index].target_cgi,
                        p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                        p_rrc_rrm_ho_required);
                /*SPR_19166_END*/

		if (cmp_result == RRC_NULL)
		    break;
	    }

	    /*SPR_17462_START*/
	    if (cmp_result != RRC_NULL)
	    {
		    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
				    p_ue_context->p_gb_context->facility_name, RRC_WARNING,
				    "X2 HO Target Id not found. inter_rat_ho_stats_list.count = %u",
				    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
				    rrc_uecc_instance_stats[p_ue_context->
				    p_gb_context->offset_uecc_mod_id].
				    rrc_uecc_cell_stats[p_ue_context->cell_index].
				    rrc_handover_related_stats.inter_rat_ho_stats_list.count);
	    }
	    /*SPR_16595_START*/
	    if (cmp_result == RRC_NULL)
		    /*SPR_17462_END*/
	    {
		    /*SPR_16595_END*/
		    /* BUG_12786 start */
		    RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
				    g_rrc_stats.p_rrc_stats->
				    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
				    p_gb_context->offset_uecc_mod_id].
				    rrc_uecc_cell_stats[p_ue_context->cell_index].
				    rrc_handover_related_stats.inter_rat_ho_stats_list.
				    inter_rat_ho_stats[index].ho_IratOutSucc_cause.ho_IratOutSucc_Cause,
				    s1apcause);
		    /* BUG_12786 stop */

		    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
				    g_rrc_stats.p_rrc_stats->
				    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
				    p_gb_context->offset_uecc_mod_id].
				    rrc_uecc_cell_stats[p_ue_context->cell_index].
				    rrc_handover_related_stats.inter_rat_ho_stats_list.
				    inter_rat_ho_stats[index].ho_IratOutSucc_sum, 0, 
				    DUMMY_RANGE_CHECK_INDEX);
		    /* BUG 12662 Fix Stop */ 
		    /*SPR_16595_START*/
	    }
	    /*SPR_16595_END*/
	}
    }
    p_ue_context->ho_info.p_ho_info->ho_status = HO_SUCCESS;

    if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
    {
	    /* Send Release Ind to X2AP Module */
	    uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
    }

    /* Send Release Ind to X2AP Module */
    /* SPR 16312 Start */
    /* Code to send message to S1AP module moved after
     * uecc_s1ap_build_and_send_enb_ue_ctx_release_req */
    /* SPR 16312 Stop */
    /*SPR 14254 Fix Start*/
    if(p_ue_context->m.mme_ue_id2_present)
    {
	    p_ue_context->m.gw_context_release_indication_present = 1;
	    s1apcause.t = 1;
	    /* SPR 15784 Start */
	    s1apcause.u.radioNetwork = s1ap_successful_handover; 
	    /* SPR 15784 Stop */
	    if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
				    p_ue_context,
				    &s1apcause))
	    {
		    RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
				    "Fail to send the context release request to s1ap");
	    }
    }
    /*SPR 14254 Fix Stop*/

    /* SPR 16312 Start */
    /*BUG 604 changes start*/
    if(INTRA_CELL_HO !=  p_ue_context->ho_info.s1_or_x2_handover)
    {
	    if(RRC_FAILURE == uecc_s1ap_build_and_send_ue_release_ind(p_ue_context))
	    {
		    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
				    "[%s] Failed to send UE Release Indication to S1AP "
				    ,__FUNCTION__);
	    }
    }
    /*BUG 604 changes stop*/
    /* SPR 16312 Stop */
    /* enabling flag */
    SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);
    /*BUG 604 changes start*/
    if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
	SET_S1U_RELEASE_NOT_REQ_FLAG(p_ue_context);
    }
    /*BUG 604 changes stop*/
    /* finish fsm successfully */
    uecc_fsm_finish_activity_success(p_ue_context,PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_delete_fwd_tunnel_ue_release_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void  uecc_fsm_src_ho_delete_fwd_tunnel_ue_release_req_handler(
	uecc_ue_context_t*  p_ue_context,
	void*               p_api
	)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
	    UECC_UE_LL_GUARD_TIMER, &p_ue_context->timers_data);
    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;

    /* storing ue_ctx_rel_cmd message */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd
	= (rrc_s1ap_ue_context_release_command_t*)
	rrc_mem_get(sizeof(rrc_s1ap_ue_context_release_command_t));
    /* SES-508 Fix Start */
    /* Code Removed */
    /* SES-508 Fix End */
    l3_memcpy_wrapper(p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_ue_ctx_rel_cmd,
	    p_api,
	    sizeof(rrc_s1ap_ue_context_release_command_t));

    /* filling the cause in p_ue_context */
    p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;

    SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

    if( IS_RE_ESTABLISHMENT_REQ_RECEIVED(p_ue_context))
    {
        if (RRC_SUCCESS != 
                uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                    p_ue_context->p_gb_context, p_ue_context->crnti,
                    p_ue_context->reestablishmentCause,
                    p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "build and send rrc connection reestablishment reject)");
        }
    }
    
   /*SPR 21947 Start */
    if(PNULL != p_api)
    {
        rrc_mem_free(p_api);
        p_api = PNULL; 
    }
   /*SPR 21947 Stop */

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type12_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/*SPR_19066_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_get_ho_prep_info_ongoing_s1u_reset_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void uecc_fsm_src_ho_get_ho_prep_info_ongoing_s1u_reset_req_handler(
/*SPR_19066_END*/
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* stopping timer UECC_UE_HO_PREP_TIMER */
    /*x2_ho_timer and INTER_CELL_HO timer*/
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes start*/
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
    }
    else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
    }
    /*BUG 604 changes start*/
    /*else if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_INTRA_CELL_HO_GUARD_TIMER, &p_ue_context->timers_data);
    }*/
    /*BUG 604 changes stop*/

    /* filling the cause in p_ue_context */
    p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
    /* updating ho_status */
    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type13_actions);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_prep_ongoing_s1ap_reset_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void uecc_fsm_src_ho_prep_ongoing_s1ap_reset_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /*x2_ho_timer*/
    /* stopping timer UECC_UE_HO_PREP_TIMER */
    /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);*/

    /* updating ho_status */
    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;

    /* Process failure according to selected failure actions list */
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        /*x2_ho_timer*/
        /* stopping timer UECC_UE_X2_HO_PREP_TIMER */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);

        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
        
        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type30_actions);
    }
    else
    {
      /*x2_ho_timer*/
      /* stopping timer UECC_UE_HO_PREP_TIMER */
      uecc_ue_timer_stop(p_ue_context->p_gb_context, 
              UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);

      /* filling the cause in p_ue_context */
      p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
      p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;

      uecc_fsm_start_failure_processing(
          p_ue_context,
          uecc_fsm_src_ho_fail_type13_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_s1u_reset_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void uecc_fsm_src_ho_rrm_ho_cmd_enb_status_transfer_s1u_reconfig_wait_for_srb_data_status_s1u_reset_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /*x2_ho_timer*/
    /* stopping UECC_UE_HO_OVERALL_TIMER timer */
    /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);*/

    /* Send HO CANCEL to Target eNB in case of X2AP HO */
    /* Send HO CANCEL to Target cell in case of INTER CELL HO */
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        /*x2_ho_timer*/
        /* stopping UECC_UE_X2_HO_OVERALL_TIMER timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                "FAILURE: Send HO CANCEL to X2AP");

        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;

        if (RRC_SUCCESS != uecc_x2ap_build_and_send_handover_cancel(p_ue_context))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                    "Fail to send the handover cancel to X2AP");
        }
    }
    else if(S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        /*x2_ho_timer*/
        /* stopping UECC_UE_HO_OVERALL_TIMER timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);
    }
    /*BUG 604 changes start*/
    /*else if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
              UECC_UE_INTRA_CELL_HO_GUARD_TIMER, &p_ue_context->timers_data);
    }*/
    /*BUG 604 changes stop*/
    else if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
              UECC_UE_CCO_GUARD_TIMER, &p_ue_context->timers_data);
    }

    /* filling the cause in p_ue_context */
    p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;

    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type13_actions);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_ctx_rel_cmd_s1_reset_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void uecc_fsm_src_ho_wait_for_ctx_rel_cmd_s1_reset_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* stopping the UECC_UE_HO_OVERALL_TIMER timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);

    /* filling the cause in p_ue_context */
    p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
    /* updating the ho_status */
    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;

    SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

    if( IS_RE_ESTABLISHMENT_REQ_RECEIVED(p_ue_context))
    {
        if (RRC_SUCCESS != 
                uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                    p_ue_context->p_gb_context, p_ue_context->crnti,
                    p_ue_context->reestablishmentCause,
                    p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR, "Fail to send rrc " 
                    "connection rejection message to UE.");
        }
    }

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type13_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_delete_fwding_tunnel_s1_reset_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static void uecc_fsm_src_ho_delete_fwding_tunnel_s1_reset_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* stopping timer UECC_UE_LL_GUARD_TIMER */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_LL_GUARD_TIMER, &p_ue_context->timers_data);

    /* filling the cause in p_ue_context */
    p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
    /* updating the ho_status */
    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;

    SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);
    if( IS_RE_ESTABLISHMENT_REQ_RECEIVED(p_ue_context))
    {
        if (RRC_SUCCESS != 
                uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                    p_ue_context->p_gb_context, p_ue_context->crnti,
                    p_ue_context->reestablishmentCause,
                    p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR, "Fail to send rrc " 
                    "connection rejection message to UE.");
        }
    }
    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type13_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR_19066_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_get_ho_prep_info_ongoing_rrm_ho_cancel_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for handover cancel request from the RRM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_get_ho_prep_info_ongoing_rrm_ho_cancel_req_handler(
/*SPR_19066_END*/
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    rrc_rrm_ho_cancel_req_t *rrc_rrm_ho_cancel_req
        =(rrc_rrm_ho_cancel_req_t*)p_api;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*BUG 604 changes start*/
    /*if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"invalid event received");
        return;
    }
    else
    {*/
    /*BUG 604 changes stop*/
        /* updating the cause in p_ue_context */
    /*BUG 604 changes start*/
        if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
        {
            if (RRC_SUCCESS != fill_x2ap_cause(p_ue_context->p_gb_context,
                        &(p_ue_context->ho_info.x2ap_cause),
                        &(rrc_rrm_ho_cancel_req->rrm_cause)))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "Fail to fill x2ap cause received from RRM");
                /*x2_ho_timer*/
                /* stop timer UECC_UE_X2_HO_PREP_TIMER */
                uecc_ue_timer_stop(p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->
                        timers_data);
                /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->
                  timers_data);*/
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type26_actions);

                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
            }
            /*x2_ho_timer*/
            /* stop timer UECC_UE_X2_HO_PREP_TIMER */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->
                    timers_data);
        }
        else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
            p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_ue_context->ho_info.cause.u.radioNetwork = s1ap_handover_cancelled;
            /*x2_ho_timer*/
            /* stop timer UECC_UE_HO_PREP_TIMER */
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_HO_PREP_TIMER, &p_ue_context->
                    timers_data);
        }
        /* setting is_rrm_ho_cancel_receive variable */
        p_ue_context->ho_info.p_ho_info->p_src_ho_info->is_rrm_ho_cancel_receive = 1;

        /*x2_ho_timer*/
        /* stop timer UECC_UE_HO_PREP_TIMER */
        /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->
          timers_data);*/
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type14_actions);
    /*BUG 604 changes start*/
    //}
    /*BUG 604 changes stop*/
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_prep_rrm_ho_cancel_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for handover cancel request from the RRM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_prep_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
     rrc_rrm_ho_cancel_req_t *rrc_rrm_ho_cancel_req
                 =(rrc_rrm_ho_cancel_req_t*)p_api;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* updating the cause in p_ue_context */
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        if (RRC_SUCCESS != fill_x2ap_cause(
                    p_ue_context->p_gb_context,
                    &(p_ue_context->ho_info.x2ap_cause),
                    &(rrc_rrm_ho_cancel_req->rrm_cause)))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                    "Fail to fill x2ap cause received from RRM");
            /*SPR_16778_START*/
            p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
            /*SPR_16778_END*/

            /*x2_ho_timer*/
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->
                    timers_data);
            /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->
                    timers_data);*/
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type27_actions);

            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return;
        }
        /*x2_ho_timer*/
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->
                    timers_data);
    }
    else
    {
        /*SPR_16778_START*/
        if(RRC_SUCCESS != fill_s1ap_cause(&(p_ue_context->ho_info.cause), 
                                  &(rrc_rrm_ho_cancel_req->rrm_cause),p_ue_context))
        {    
            p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
        }
    	/*SPR_16778_END*/
        /*x2_ho_timer*/
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_PREP_TIMER, &p_ue_context->
            timers_data);
    }
    /* setting is_rrm_ho_cancel_receive variable */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->is_rrm_ho_cancel_receive = 1;

    /*x2_ho_timer*/
    /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, &p_ue_context->
            timers_data);*/

    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type15_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_rrm_ho_cmd_rrm_ho_cancel_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for handover cancel request from the RRM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_rrm_ho_cmd_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    rrc_rrm_ho_cancel_req_t *rrc_rrm_ho_cancel_req
                         =(rrc_rrm_ho_cancel_req_t*)p_api;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*BUG 604 changes stop*/
    /*if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    { 
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        return;
    }
    else
    {*/
    /*BUG 604 changes stop*/
        /* updating the cause in p_ue_context */
    /*BUG 604 changes start*/
        if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
        {
            if (RRC_SUCCESS != fill_x2ap_cause(p_ue_context->p_gb_context,
                        &(p_ue_context->ho_info.x2ap_cause),
                        &(rrc_rrm_ho_cancel_req->rrm_cause)))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "Fail to fill x2ap cause received from RRM");
                /*SPR_16778_START*/
                p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
                /*SPR_16778_END*/
                /*x2_ho_timer*/
                uecc_ue_timer_stop(p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_OVERALL_TIMER, 
                        &p_ue_context->timers_data);
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type27_actions);

                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
            }
            /*x2_ho_timer*/
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_X2_HO_OVERALL_TIMER, 
                    &p_ue_context->timers_data);
        }
        else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
            /*SPR_16778_START*/
            if(RRC_SUCCESS != fill_s1ap_cause(&(p_ue_context->ho_info.cause), 
                                      &(rrc_rrm_ho_cancel_req->rrm_cause),p_ue_context))
            {    
                p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
            }
    	    /*SPR_16778_END*/
            /*x2_ho_timer*/
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_HO_OVERALL_TIMER, 
                    &p_ue_context->timers_data);
        }
        /* setting is_rrm_ho_cancel_receive variable */
        p_ue_context->ho_info.p_ho_info->p_src_ho_info->is_rrm_ho_cancel_receive = 1;

        /*x2_ho_timer*/
        /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, 
          &p_ue_context->timers_data);*/

        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type15_actions);
    /*BUG 604 changes start*/
    //}
    /*BUG 604 changes stop*/
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_enb_status_transfer_rrm_ho_cancel_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for handover cancel request from the RRM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_enb_status_transfer_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    rrc_rrm_ho_cancel_req_t *rrc_rrm_ho_cancel_req
                            =(rrc_rrm_ho_cancel_req_t*)p_api;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /*BUG 604 changes start*/
    if (CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        return;
    }
    /*BUG 604 changes stop*/
    else
    {
        /* updating the cause in p_ue_context */
    /*BUG 604 changes start*/
        if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
        {
            if (RRC_SUCCESS != fill_x2ap_cause(p_ue_context->p_gb_context,
                        &(p_ue_context->ho_info.x2ap_cause),
                        &(rrc_rrm_ho_cancel_req->rrm_cause)))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "Fail to fill x2ap cause received from RRM");
                /*SPR_16778_START*/
                p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
                /*SPR_16778_END*/

                /*x2_ho_timer*/
                uecc_ue_timer_stop(p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_OVERALL_TIMER, 
                        &p_ue_context->timers_data);
                 /*SPR:6462 start*/
                 p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE28;
                /*uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type28_actions);*/
                 RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                         "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

                 if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                             p_ue_context))
                 {
                     RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                             "Failed to send the reestablish complete indication to LLIM");
                 }
                 /*SPR:6462 stop*/

                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
            }
            /*x2_ho_timer*/
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_X2_HO_OVERALL_TIMER, 
                    &p_ue_context->timers_data);
        }
        else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
            /*SPR_16778_START*/
            if(RRC_SUCCESS != fill_s1ap_cause(&(p_ue_context->ho_info.cause), 
                                      &(rrc_rrm_ho_cancel_req->rrm_cause),p_ue_context))
            {    
                p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
            }
    	    /*SPR_16778_END*/
            /*x2_ho_timer*/
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_HO_OVERALL_TIMER, &p_ue_context->
                    timers_data);
        }
        /* setting is_rrm_ho_cancel_receive variable */
        p_ue_context->ho_info.p_ho_info->p_src_ho_info->is_rrm_ho_cancel_receive = 1;

        /*x2_ho_timer*/
        /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, &p_ue_context->
          timers_data);*/
         /*SPR:6462 start*/
         p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE16;
        /*uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type16_actions);*/
         RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                 "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

         if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                     p_ue_context))
         {
             RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                     "Failed to send the reestablish complete indication to LLIM");
         }
         /*SPR:6462 stop*/
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_s1u_reconfig_rrm_ho_cancel_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for handover cancel request from the RRM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_s1u_reconfig_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    rrc_rrm_ho_cancel_req_t *rrc_rrm_ho_cancel_req
        =(rrc_rrm_ho_cancel_req_t*)p_api;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* updating the cause in p_ue_context */
/*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        if (RRC_SUCCESS != fill_x2ap_cause(
                    p_ue_context->p_gb_context,
                    &(p_ue_context->ho_info.x2ap_cause),
                    &(rrc_rrm_ho_cancel_req->rrm_cause)))
	{
		/*SPR_16778_START*/
		RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
				"Fail to fill x2ap cause received from RRM."
				" Filling cause unspecified.");
		p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
		p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 

		/*SPR_16778_END*/
	}
        /*x2_ho_timer*/
        /* stop timer UECC_UE_X2_HO_OVERALL_TIMER */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);
    }
    else
    {
         /*SPR_16778_START*/
        if(RRC_SUCCESS != fill_s1ap_cause(&(p_ue_context->ho_info.cause), 
                                  &(rrc_rrm_ho_cancel_req->rrm_cause),p_ue_context))
        {  
            p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
        }
    	/*SPR_16778_END*/
        /*x2_ho_timer*/
        /* stop timer UECC_UE_HO_OVERALL_TIMER */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);
    }
    /* setting is_rrm_ho_cancel_receive variable */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->is_rrm_ho_cancel_receive = 1;
    /*x2_ho_timer*/
    /* stop timer UECC_UE_HO_OVERALL_TIMER */
    /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);*/

    /* state changed to UECC_SRC_HO_S1U_DELETE_FORWARDING_TUNNEL_ONGOING */
    UECC_FSM_SET_CHILD_STATE(p_ue_context,
            &uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_s1u_roll_back_rrm_ho_cancel_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for handover cancel request from the RRM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_wait_for_s1u_roll_back_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "Ignoring RRC_RRM_HO_CANCEL_REQ because "
            "HO failure is already going on;Unused variable"
            "p_api=%p",p_api);/*SPR 17777 +-*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_srb_data_wait_for_ctx_rel_rrm_ho_cancel_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for handover cancel request from the RRM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_wait_for_srb_data_wait_for_ctx_rel_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    rrc_rrm_ho_cancel_req_t *rrc_rrm_ho_cancel_req
                             =(rrc_rrm_ho_cancel_req_t*)p_api;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

/*BUG 604 changes start*/
    if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"invalid event received");
        return;
    }
/*BUG 604 changes stop*/
    else
    {
        /* updating the cause in p_ue_context */
/*BUG 604 changes start*/
        if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
        {
            if (RRC_SUCCESS != fill_x2ap_cause(p_ue_context->p_gb_context,
                        &(p_ue_context->ho_info.x2ap_cause),
                        &(rrc_rrm_ho_cancel_req->rrm_cause)))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "Fail to fill x2ap cause received from RRM");
            }
        }
        else
        {
            p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_ue_context->ho_info.cause.u.radioNetwork = s1ap_handover_cancelled;
        }
        /* setting is_rrm_ho_cancel_receive variable */
        p_ue_context->ho_info.p_ho_info->p_src_ho_info->is_rrm_ho_cancel_receive = 1;


        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                "FAILURE: Sending RRC_RRM_HO_CANCEL_RESP (Failure)");

        if( RRC_SUCCESS != uecc_rrm_build_and_send_ho_cancel_resp( p_ue_context, RRC_FAILURE))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                    "Fail to send the handover cancel response to RRM");
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/*SPR_19066_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_get_ho_prep_info_ongoing_llim_radio_link_failure_ind_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for radio link failure event received from LLIM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_get_ho_prep_info_ongoing_llim_radio_link_failure_ind_handler(
/*SPR_19066_END*/
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{

	/* coverity_fix_64028_start */
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	/* storing re_establishment contents into p_re_establishment_req */
	p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_radio_link_failure_ind
		= p_api;

	/* filling the cause in p_ue_context */
	/*BUG 604 changes start*/
	if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
			(INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
			(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
		/*BUG 604 changes stop*/
	{
		p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
		p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;

		/*x2_ho_timer*/
		/* stop timer UECC_UE_X2_HO_PREP_TIMER */
		uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_X2_HO_PREP_TIMER, 
				&p_ue_context->timers_data);
	}
	else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
	{
		p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
		p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;

		/* stop timer UECC_UE_HO_PREP_TIMER */
		uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_HO_PREP_TIMER, 
				&p_ue_context->timers_data);
	}
	/*BUG 604 changes start*/
	/*else if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
	  {
	  uecc_ue_timer_stop(p_ue_context->p_gb_context,
	  UECC_UE_INTRA_CELL_HO_GUARD_TIMER,
	  &p_ue_context->timers_data);
	  }*/
	/*BUG 604 changes stop*/
	/*x2_ho_timer*/
	/* stop timer UECC_UE_HO_PREP_TIMER */
	/*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, 
	  &p_ue_context->timers_data);*/
	uecc_fsm_start_failure_processing(
			p_ue_context,
			uecc_fsm_src_ho_fail_type17_actions);
	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* coverity_fix_64028_stop */

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_ho_prep_llim_radio_link_failure_ind_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for radio link failure event received from LLIM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_ho_prep_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* storing re_establishment contents into p_re_establishment_req */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_radio_link_failure_ind
        = p_api;
    /* filling the cause in p_ue_context */
    /*BUG 604 changes stop*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 

        /*x2_ho_timer*/
        /* stop timer UECC_UE_X2_HO_PREP_TIMER */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_X2_HO_PREP_TIMER, 
            &p_ue_context->timers_data);
    }
    else
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
        
        /*x2_ho_timer*/
        /* stop timer UECC_UE_HO_PREP_TIMER */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_HO_PREP_TIMER, 
            &p_ue_context->timers_data);
    }
    /*x2_ho_timer*/
    /* stop timer UECC_UE_HO_PREP_TIMER */
    /*uecc_ue_timer_stop(UECC_UE_HO_PREP_TIMER, 
            &p_ue_context->timers_data);*/

    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type18_actions);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_rrm_ho_cmd_llim_radio_link_failure_ind_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for radio link failure event received from LLIM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_rrm_ho_cmd_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* filling the cause in p_ue_context */
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
        /*x2_ho_timer*/
        /* stopping timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_X2_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data);
    }
    else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
        /*x2_ho_timer*/
        /* stopping timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data);
    }
    /*BUG 604 changes start*/
    /*else if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_INTRA_CELL_HO_GUARD_TIMER,
                &p_ue_context->timers_data);
    }*/
    /*BUG 604 changes stop*/
    else if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_CCO_GUARD_TIMER,
                &p_ue_context->timers_data);
    }
    /* storing re_establishment contents into p_re_establishment_req */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_radio_link_failure_ind
        = p_api;

    /*x2_ho_timer*/
    /* stopping timer */
    /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, 
      &p_ue_context->timers_data);*/
    
    /*BUG 604 changes start*/
        if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    /*BUG 604 changes stop*/
        {
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type38_actions);
        }
        else
        {
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type39_actions);
        }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_s1u_reconfig_llim_radio_link_failure_ind_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for radio link failure event received from LLIM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_s1u_reconfig_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* filling the cause in p_ue_context */
/*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
        /*x2_ho_timer*/
        /* stopping timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_X2_HO_OVERALL_TIMER,
            &p_ue_context->timers_data);
    }
    else
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
        /*x2_ho_timer*/
        /* stopping timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_HO_OVERALL_TIMER,
            &p_ue_context->timers_data);
    }
    /* storing re_establishment contents into p_re_establishment_req */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_radio_link_failure_ind
        = p_api;

    /*x2_ho_timer*/
    /* stopping timer */
    /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER,
            &p_ue_context->timers_data);*/

    /* reconfigure the tunnels created during s1u_reconfig ongoing */
    UECC_FSM_SET_CHILD_STATE(p_ue_context,
            &uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_enb_status_llim_radio_link_failure_ind_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for radio link failure event received from LLIM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_enb_status_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* filling the cause in p_ue_context */
    /*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
        /*x2_ho_timer*/
        /* stop timer UECC_UE_X2_HO_OVERALL_TIMER */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->
                timers_data);
    }
    else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
        /*x2_ho_timer*/
        /* stop timer UECC_UE_HO_OVERALL_TIMER */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_OVERALL_TIMER, &p_ue_context->
                timers_data);
    }
    /*BUG 604 changes start*/
    /*else if (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_INTRA_CELL_HO_GUARD_TIMER, &p_ue_context->
                timers_data);
    }*/
    /*BUG 604 changes start*/
    else if (CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_CCO_GUARD_TIMER, &p_ue_context->
                timers_data);
    }
    
    /* storing re_establishment contents into p_re_establishment_req */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_radio_link_failure_ind
        = p_api;

    /*x2_ho_timer*/
    /* stop timer UECC_UE_HO_OVERALL_TIMER */
    /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, &p_ue_context->
      timers_data);*/

    /*BUG 604 changes start*/
        if (CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    /*BUG 604 changes stop*/
        {
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type38_actions);
        }
        else
        {
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type19_actions);
        }
 
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_ctx_rel_llim_radio_link_failure_ind_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for radio link failure event received from LLIM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_wait_for_ctx_rel_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* filling the cause in p_ue_context */
/*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
    }
    else
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
    }
    /* storing re_establishment contents into p_re_establishment_req */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_radio_link_failure_ind
        = p_api;

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
            "Event received in unexpected state");

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_srb_data_llim_radio_link_failure_ind_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for radio link failure event received from LLIM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_wait_for_srb_data_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "Waiting for SRB Data status Indication response from PDCP. Ignoring"
            "RLF;Unused variable p_api=%p",p_api);/*SPR 17777 +-*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_s1u_delete_fwd_tunnel_llim_radio_link_failure_ind_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for radio link failure event received from LLIM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_s1u_delete_fwd_tunnel_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* filling the cause in p_ue_context */
/*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
    }
    else
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
    }
    /* storing re_establishment contents into p_re_establishment_req */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_radio_link_failure_ind
        = p_api;

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "Waiting for delete fowarding tunnel response from S1U");

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_resp
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et* - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of RRC_RRM_HO_CANCEL_REQ,
 *                  RRC_RRM_HO_CANCEL_RESP message to RRM
 ****************************************************************************/
static void uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_resp(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "FAILURE: Send RRC_RRM_HO_CANCEL_RESP (Success)");

    if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_cancel_resp( p_ue_context, RRC_SUCCESS))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                "Fail to send the handover cancel response to RRM");
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/******************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_failure_resp 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et* - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of RRC_RRM_HO_CANCEL_REQ,
 *                  Sending RRC_RRM_HO_CANCEL_RESP(FAILURE) message to RRM
 *****************************************************************************/
static void uecc_fsm_src_ho_fail_action_send_rrm_ho_cancel_failure_resp(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "FAILURE: Send RRC_RRM_HO_CANCEL_RESP (Failure)");

    if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_cancel_resp( p_ue_context, RRC_FAILURE))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                "Fail to send the handover cancel response to RRM");
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of RRC_RRM_HO_REQUIRED,
 *                  RRC_RRM_HO_FAILURE message to RRM
 ****************************************************************************/
static void uecc_fsm_src_ho_fail_action_send_rrm_ho_failure_ind(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "FAILURE: Send RRC_RRM_HO_FAILURE");

    if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_failure( p_ue_context))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                "Fail to send the handover failure indication to RRM");
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_send_ho_cancel_ind 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : This is to send the ho cancel request to S1AP/X2AP
 ****************************************************************************/
static void uecc_fsm_src_ho_fail_action_send_ho_cancel_ind(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    if (RRC_FALSE == p_ue_context->ho_info.cdma_ho_ongoing)
    {
    /*BUG 604 changes start*/
        if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "FAILURE: Send HO CANCEL to X2AP");

            if (RRC_SUCCESS != uecc_x2ap_build_and_send_handover_cancel(p_ue_context))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "Fail to send the handover cancel to X2AP");
            }
        }
        else
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "FAILURE: Send HO CANCEL IND to S1AP");

            if (RRC_SUCCESS != uecc_s1ap_build_and_send_handover_cancel( p_ue_context))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "Fail to send the handover cancel indication to S1AP");
            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_send_llim_data_buffer_stop_ind 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Sending data buffer stop indication to LLIM 
 ****************************************************************************/
static void uecc_fsm_src_ho_fail_action_send_llim_data_buffer_stop_ind(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    if( p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                    is_sn_hfn_req_sent == RRC_TRUE )
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                "FAILURE: Send UECC_LLIM_DATA_BUFFER_STOP_IND to LLIM");

        if (RRC_SUCCESS != uecc_llim_build_and_send_stop_data_buffer_ind( 
                    p_ue_context))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                    "Failed to send the data buffer stop indication to LLIM");
        }
    }
    else
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                "NOT sending UECC_LLIM_DATA_BUFFER_STOP_IND to LLIM");


    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_send_s1ap_context_release_cmd 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : send context release command to s1ap 
 ****************************************************************************/
static void uecc_fsm_src_ho_fail_action_send_s1ap_context_release_cmd(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "FAILURE: Send CTX RELEASE REQUEST to S1AP");

    if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req( 
                p_ue_context, &p_ue_context->ho_info.cause))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                "Fail to send the context release request to s1ap");
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_send_error_indication 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : send context release command to s1ap 
 ****************************************************************************/
static void uecc_fsm_src_ho_fail_action_send_error_indication
(
 uecc_ue_context_t*  p_ue_context,
 rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        if (RRC_SUCCESS !=
                (uecc_x2ap_build_and_send_error_indication(p_ue_context->p_gb_context, 
                        p_ue_context->x2ap_context.peer_enodeb_id,
                        p_ue_context->x2ap_context.old_enb_ue_x2ap_id,
                        RRC_TRUE,
                        p_ue_context->x2ap_context.new_enb_ue_x2ap_id,
                        (rrc_bool_et)p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present,
                        (x2ap_CriticalityDiagnostics*)PNULL,
                        RRC_FALSE,
                        &p_ue_context->ho_info.x2ap_cause,
                        p_ue_context
                        )))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING, 
                    "[X2AP Handover Command] Error Indication encountered  ");
        }
    }
    else
    {
        if (RRC_SUCCESS !=
                (uecc_s1ap_build_and_send_error_indication(
                         p_ue_context->mme_id,
                         p_ue_context->mme_ue_id,
                         (rrc_bool_et)p_ue_context->m.mme_ue_id_present,
                         p_ue_context->s1ap_ue_id,
                         RRC_TRUE,
                         (s1ap_CriticalityDiagnostics*)PNULL,
                         RRC_FALSE,
                         &p_ue_context->ho_info.cause,
                         p_ue_context)))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING, 
                    "[S1AP Handover Command] Logical error encountered  ");
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_re_est_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_llim_re_est_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and sent the ReEstablishment request to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_re_est_req(p_ue_context))
    {
    /*BUG 604 changes start*/
        if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type33_actions);
        }
        else
        {
                /*SPR_16778_START*/
                if ((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
                        (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
                        (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
                {
                    p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
                }
                /*SPR_16778_END*/
            /*BUG 604 changes start*/
            	uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type3_actions);
	    }
    /*BUG 604 changes stop*/
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_re_est_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP message.
 ****************************************************************************/
void uecc_fsm_src_ho_llim_re_est_resp_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        )
{
    rrc_uecc_llim_re_establish_ue_entity_resp_t
        *p_re_establish_ue_entity_rest = 
        (rrc_uecc_llim_re_establish_ue_entity_resp_t *)p_api_data;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    switch(p_re_establish_ue_entity_rest->response_code)
    {
        case RRC_SUCCESS:
            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                &uecc_fsm_src_ho_enb_status_transfer_ongoing, PNULL);
            break;
        case RRC_FAILURE:
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING, "[UECC_LLIM_RE_ESTABLISHMENT_RESP]"
                    " Failure Response received. Start Failure Processing");
            /* updating the cause */
    /*BUG 604 changes start*/
            if ((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
                    (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
                    (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
    /*BUG 604 changes stop*/
            {
                p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
            }
            else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
            {
                p_ue_context->ho_info.cause.t= T_s1ap_Cause_radioNetwork;
                p_ue_context->ho_info.cause.u.radioNetwork= s1ap_unspecified_2;
            }
    /*BUG 604 changes start*/
            if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
            {
                    uecc_fsm_start_failure_processing(
                            p_ue_context,
                            uecc_fsm_src_ho_fail_type33_actions);
            }
                /*securitykey*/      
                else 
                {
                /*BUG 604 changes start*/
                    uecc_fsm_start_failure_processing(
                            p_ue_context,
                            uecc_fsm_src_ho_fail_type3_actions);
                /*BUG 604 changes stop*/

            }
    /*BUG 604 changes stop*/
            break;
        default:
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING,"[UECC_LLIM_RE_ESTABLISHMENT_RESP]"
                    " Invalid Response Code = [%u]. Start failure processing",
                    p_re_establish_ue_entity_rest->response_code);
            /* updating the cause */
    /*BUG 604 changes start*/
            if ((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
                (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
    /*BUG 604 changes start*/
            {
                p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
            }
            else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
            {
                p_ue_context->ho_info.cause.t= T_s1ap_Cause_radioNetwork;
                p_ue_context->ho_info.cause.u.radioNetwork= s1ap_unspecified_2;
            }
    /*BUG 604 changes start*/
            if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
            {
                    uecc_fsm_start_failure_processing(
                            p_ue_context,
                            uecc_fsm_src_ho_fail_type33_actions);
            }
                /*securitykey*/      
                else 
                {
                /*BUG 604 changes start*/
                    uecc_fsm_start_failure_processing(
                            p_ue_context,
                            uecc_fsm_src_ho_fail_type3_actions);
            }
    /*BUG 604 changes stop*/
            break;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/* SPR_7888_fix */
/****************************************************************************
 * Function Name  : convert_rrm_cause_to_s1ap_cause
 * Inputs         : s1ap_Cause *p_cause
 *                  rrm_cause_t *p_rrmCause
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This action routine is called to convert rrm_cause to
                    s1ap_cause
 ****************************************************************************/
rrc_return_t convert_rrm_cause_to_s1ap_cause(rrm_cause_t *p_rrmCause, s1ap_Cause *p_cause)
{
    rrc_return_t result = RRC_SUCCESS;
    switch(p_rrmCause->type)
    {
        case T_s1ap_Cause_radioNetwork:
            p_cause->t = p_rrmCause->type;
            if (S1AP_CAUSE_RADIO_NETWORK_MAX_VALUE >= p_rrmCause->value)  
            { 
                p_cause->u.radioNetwork = p_rrmCause->value; 
            }
            else 
            { 
                result = RRC_FAILURE; 
            }
            break;
        case T_s1ap_Cause_transport:
            p_cause->t = p_rrmCause->type;
            if (S1AP_CAUSE_TRANSPORT_MAX_VALUE >= p_rrmCause->value)
            { 
                p_cause->u.transport = p_rrmCause->value; 
            }
            else 
            { 
                result = RRC_FAILURE; 
            }
            break;
        case T_s1ap_Cause_nas:
            p_cause->t = p_rrmCause->type;
            if (S1AP_CAUSE_NAS_MAX_VALUE >= p_rrmCause->value)
            { 
                p_cause->u.nas = p_rrmCause->value; 
            }
            else 
            { 
                result = RRC_FAILURE; 
            }
            break;
        case T_s1ap_Cause_protocol:
            p_cause->t = p_rrmCause->type;
            if (S1AP_CAUSE_PROTOCOL_MAX_VALUE >= p_rrmCause->value)
            { 
                p_cause->u.protocol = p_rrmCause->value;
            }
            else 
            { 
                result = RRC_FAILURE; 
            }
            break;
        case T_s1ap_Cause_misc:
            p_cause->t = p_rrmCause->type;
            if (S1AP_CAUSE_MISC_MAX_VALUE >= p_rrmCause->value)
            { 
                p_cause->u.misc = p_rrmCause->value; 
            }
            else 
            { 
                result = RRC_FAILURE; 
            }
            break;
        default:
                result = RRC_FAILURE; 
    }
    return result;
}
/* SPR_7888_fix */

/****************************************************************************
 * Function Name  : convert_rrm_cause_to_x2ap_cause 
 * Inputs         : rrm_cause_t & x2ap_Cause
 *                  x2ap_Cause - *p_cause               
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called to convert rrm cause to x2ap
 *                  cause
 ****************************************************************************/
void convert_rrm_cause_to_x2ap_cause(rrm_cause_t *p_rrmCause, x2ap_Cause *p_cause)
{
    switch(p_rrmCause->type)
    {
        case T_x2ap_Cause_radioNetwork:
            p_cause->t = p_rrmCause->type;
            /*SPR 16609 FIX START*/
            p_cause->u.radioNetwork = p_rrmCause->value;
            /*SPR 16609 FIX END*/
	    break;
        case T_x2ap_Cause_transport:
            p_cause->t = p_rrmCause->type;
            p_cause->u.transport = p_rrmCause->value;
            break;
        case T_x2ap_Cause_protocol:
            p_cause->t = p_rrmCause->type;
            p_cause->u.protocol = p_rrmCause->value;
            break;
        case T_x2ap_Cause_misc:
            p_cause->t = p_rrmCause->type;
            p_cause->u.misc = p_rrmCause->value;
            break;
        default:
           break;
    }
}
/*SPR_17284_START*/
/****************************************************************************
 * Function Name  : map_x2ap_cause_to_s1ap_cause 
 * Inputs         : rrm_cause_t & s1ap_Cause               
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called to convert x2ap cause 
 *                  recieved from rrm to s1ap cause.
 ****************************************************************************/
void map_x2ap_cause_to_s1ap_cause(x2ap_Cause x2apCause, s1ap_Cause *p_cause)
{
    switch(x2apCause.t)
    {
        case T_x2ap_Cause_radioNetwork:
            p_cause->t = T_s1ap_Cause_radioNetwork;
            switch(x2apCause.u.radioNetwork)
            {
                case x2ap_handover_desirable_for_radio_reasons:
                    p_cause->u.radioNetwork = s1ap_handover_desirable_for_radio_reason;
                    break;

                case x2ap_time_critical_handover:
                    p_cause->u.radioNetwork = s1ap_time_critical_handover;
                    break;

                case x2ap_resource_optimisation_handover:
                    p_cause->u.radioNetwork = s1ap_resource_optimisation_handover;
                    break;

                case x2ap_reduce_load_in_serving_cell:
                    p_cause->u.radioNetwork = s1ap_reduce_load_in_serving_cell;
                    break;

                case x2ap_partial_handover:
                    p_cause->u.radioNetwork = s1ap_partial_handover;
                    break;

                case x2ap_unknown_new_eNB_UE_X2AP_ID:
                    p_cause->u.radioNetwork = s1ap_unknown_enb_ue_s1ap_id;
                    break;

                case x2ap_unknown_old_eNB_UE_X2AP_ID:
                    p_cause->u.radioNetwork = s1ap_unknown_enb_ue_s1ap_id;
                    break;

                case x2ap_unknown_pair_of_UE_X2AP_ID:
                    p_cause->u.radioNetwork = s1ap_unknown_pair_ue_s1ap_id;
                    break;

                case x2ap_ho_target_not_allowed:
                    p_cause->u.radioNetwork = s1ap_ho_target_not_allowed;
                    break;

                case x2ap_tx2relocoverall_expiry:
                    p_cause->u.radioNetwork = s1ap_tx2relocoverall_expiry;
                    break;

                case x2ap_trelocprep_expiry:
                    p_cause->u.radioNetwork = s1ap_tS1relocprep_expiry;
                    break;

                case x2ap_cell_not_available:
                    p_cause->u.radioNetwork = s1ap_cell_not_available;
                    break;

                case x2ap_no_radio_resources_available_in_target_cell:
                    p_cause->u.radioNetwork = s1ap_no_radio_resources_available_in_target_cell;
                    break;

                case x2ap_encryption_and_or_integrity_protection_algorithms_not_supported:
                    p_cause->u.radioNetwork = 
                        s1ap_encryption_and_or_integrity_protection_algorithms_not_supported;
                    break;

                case x2ap_invalid_MME_GroupID:
                case x2ap_unknown_MME_Code:
                case x2ap_reportCharacteristicsEmpty:
                case x2ap_noReportPeriodicity:
                case x2ap_existingMeasurementID:
                case x2ap_unknown_eNB_Measurement_ID:
                case x2ap_measurement_temporarily_not_available:
                case x2ap_unspecified_2:
                default:
                    p_cause->u.radioNetwork = s1ap_unspecified_2;
                    break;

            }
            break;

        case T_x2ap_Cause_transport:
            p_cause->t = T_s1ap_Cause_transport;
            switch(x2apCause.u.transport)
            {
                case x2ap_transport_resource_unavailable:
                    p_cause->u.transport = s1ap_transport_resource_unavailable;

                    break;

                case x2ap_unspecified_3:
                default:
                    p_cause->u.transport = s1ap_unspecified_3;
                    break;
            }
            break;

        case T_x2ap_Cause_protocol:
            p_cause->t = T_s1ap_Cause_protocol;
            switch(x2apCause.u.protocol)
            {
                case x2ap_transfer_syntax_error:
                    p_cause->u.protocol = s1ap_transfer_syntax_error;
                    break;
                case x2ap_abstract_syntax_error_reject:
                    p_cause->u.protocol = s1ap_abstract_syntax_error_reject;
                    break;
                case x2ap_abstract_syntax_error_ignore_and_notify:
                    p_cause->u.protocol = s1ap_abstract_syntax_error_ignore_and_notify;
                    break;
                case x2ap_message_not_compatible_with_receiver_state:
                    p_cause->u.protocol = s1ap_message_not_compatible_with_receiver_state;
                    break;
                case x2ap_semantic_error:
                    p_cause->u.protocol = s1ap_semantic_error;
                    break;
                case x2ap_abstract_syntax_error_falsely_constructed_message:
                    p_cause->u.protocol = s1ap_abstract_syntax_error_falsely_constructed_message;
                    break;
                case x2ap_unspecified_1:
                default:
                    p_cause->u.protocol = s1ap_unspecified_1;
                    break;
            }

            break;

        case T_x2ap_Cause_misc:
            p_cause->t = T_s1ap_Cause_misc;
            switch(x2apCause.u.misc)
            {
                case x2ap_control_processing_overload:
                    p_cause->u.misc = s1ap_control_processing_overload;
                    break;
                case x2ap_hardware_failure:
                    p_cause->u.misc = s1ap_hardware_failure;
                    break;
                case x2ap_om_intervention:
                    p_cause->u.misc = s1ap_om_intervention;
                    break;
                case x2ap_not_enough_user_plane_processing_resources:
                    p_cause->u.misc = s1ap_not_enough_user_plane_processing_resources;
                    break;
                case x2ap_unspecified:
                default:
                    p_cause->u.misc = s1ap_unspecified;
                    break;
            }
            break;

        default:
            p_cause->t = T_s1ap_Cause_radioNetwork;
            p_cause->u.radioNetwork = s1ap_unspecified_2;
            break;
    }
}
/*SPR_17284_END*/

/*****************************************************************************
 * Function Name  : rrc_src_ho_process_s1u_cnf 
 * Inputs         : rrc_uecc_llim_s1u_reconfigure_cnf_t - 
 *                                          p_rrc_uecc_llim_s1u_reconfigure_cnf
 *                  uecc_ue_context_t - *p_ue_context
 * Outputs        : None
 * Returns        : None
 * Description    : This function _ho_process_s1u_cnf
 ********************************************************************************/
void rrc_src_ho_process_s1u_cnf(
        rrc_uecc_llim_s1u_reconfigure_cnf_t *p_rrc_uecc_llim_s1u_reconfigure_cnf,
        uecc_ue_context_t  *p_ue_context)
{
    uecc_ho_src_drb_forward_info_t  *p_drb_forward_info = PNULL;
    uecc_ho_src_drb_admitted_info_t *p_drb_admitted_info = PNULL;
    rrc_counter_t           erab_count = RRC_NULL;
    /* AK Start */
    U8 s1u_erab_id = RRC_NULL;
    U8 s1u_teid_count    = RRC_NULL;
    U8 num_of_erab_configured = RRC_NULL;

            /* AK Start */
            /* update in the HO Context */
            if (p_rrc_uecc_llim_s1u_reconfigure_cnf->bitmask
                    & RRC_UECC_LLIM_S1U_SETUP_SAP_CNF_PRESENT )
            {
                for(erab_count = 0; 
                    erab_count< p_rrc_uecc_llim_s1u_reconfigure_cnf->
                    setup_sap_cnf_list.num_setup_sap_cnf;
                    erab_count++ )
                {
                    s1u_erab_id = 
                            (U8)get_erab_id_from_lc_id(
                                p_ue_context,
                                p_rrc_uecc_llim_s1u_reconfigure_cnf->setup_sap_cnf_list.
                                setup_sap_cnf[erab_count].lc_id);

                    if (INVALID_ERAB_ID != s1u_erab_id)
                    {
                        if ( S1_HO == p_ue_context->ho_info.s1_or_x2_handover )
                        {

                            if( PNULL != (p_drb_forward_info
                                        = (p_ue_context->ho_info.p_ho_info->
                                            p_src_ho_info->s1_src_ho_info.
                                            src_drb_fwd_list.
                                            erab_ho_src_drb_forward_list[s1u_erab_id])))
                            {
                                if (S1U_SUCCESS !=
                                        p_rrc_uecc_llim_s1u_reconfigure_cnf->setup_sap_cnf_list.
                                        setup_sap_cnf[erab_count].cause)
                                {
                                    /* releasing those erabs which was not configured */
                                    rrc_mem_free(p_drb_forward_info);
                                    p_drb_forward_info = PNULL;
                                    /* SPR 19212 Fix Start */
                                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                                         src_drb_fwd_list.erab_ho_src_drb_forward_list[s1u_erab_id] = PNULL;
                                    /* SPR 19212 Fix Stop */
                                }
                                else
                                {
                                    /* Update Tunnel ID's in any order */
                                    if (p_rrc_uecc_llim_s1u_reconfigure_cnf->setup_sap_cnf_list.
                                            setup_sap_cnf[erab_count].bitmask &
                                            UECC_LLIM_S1U_TUNNEL_INFO_CNF_PRESENT)
                                    {
                                        p_drb_forward_info->teid_count = 
                                            p_rrc_uecc_llim_s1u_reconfigure_cnf->setup_sap_cnf_list.
                                            setup_sap_cnf[erab_count].tunnel_info_cnf_count;

                                        for (s1u_teid_count = 0;
                                                s1u_teid_count < 
                                                p_drb_forward_info->teid_count;
                                                s1u_teid_count++)
                                        {
                                            p_drb_forward_info->s1u_fwd_teid[s1u_teid_count] = 
                                                p_rrc_uecc_llim_s1u_reconfigure_cnf->setup_sap_cnf_list.
                                                setup_sap_cnf[erab_count].tunnel_info_cnf[s1u_teid_count].
                                                teid_self;
                                        }
                                        num_of_erab_configured++;
                                    }
                                }
                            }
                        }
                        else if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                                 (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
                        {
                            /*BUG 540 Fix start*/
                            if( PNULL != (p_drb_admitted_info
                                        = (p_ue_context->ho_info.p_ho_info->
                                            p_src_ho_info->s1_src_ho_info.
                                            src_erab_admitted_list.
                                            erab_ho_src_drb_admitted_info[s1u_erab_id])))
                            /*BUG 540 Fix stop*/
                            {
                                if (S1U_SUCCESS !=
                                        p_rrc_uecc_llim_s1u_reconfigure_cnf->setup_sap_cnf_list.
                                        setup_sap_cnf[erab_count].cause)
                                {
                                    /* releasing those erabs which was not configured */
                                    rrc_mem_free(p_drb_admitted_info);
                                    p_drb_admitted_info = PNULL;
                                    /* SPR 19212 Fix Start */
                                    p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                                         src_erab_admitted_list.erab_ho_src_drb_admitted_info[s1u_erab_id] = PNULL;
                                    /* SPR 19212 Fix Stop */
                                }
                                else
                                {
                                    /* Update Tunnel ID's in any order */
                                    if (p_rrc_uecc_llim_s1u_reconfigure_cnf->setup_sap_cnf_list.
                                            setup_sap_cnf[erab_count].bitmask &
                                            UECC_LLIM_S1U_TUNNEL_INFO_CNF_PRESENT)
                                    {
                                        p_drb_admitted_info->teid_count = 
                                            p_rrc_uecc_llim_s1u_reconfigure_cnf->setup_sap_cnf_list.
                                            setup_sap_cnf[erab_count].tunnel_info_cnf_count;

                                        for (s1u_teid_count = 0;
                                                s1u_teid_count < 
                                                p_drb_admitted_info->teid_count;
                                                s1u_teid_count++)
                                        {
                                            p_drb_admitted_info->s1u_fwd_teid[s1u_teid_count] = 
                                                p_rrc_uecc_llim_s1u_reconfigure_cnf->setup_sap_cnf_list.
                                                setup_sap_cnf[erab_count].tunnel_info_cnf[s1u_teid_count].
                                                teid_self;
                                        }
                                        num_of_erab_configured++;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                s1_src_ho_info.src_drb_fwd_list.count = num_of_erab_configured;
}
/****************************************************************************
 * Function Name  : uecc_populate_drb_context
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : This populates the UE DRB context for X2AP
 ****************************************************************************/
static void uecc_populate_drb_context(
        uecc_ue_context_t  *p_ue_context           /* UECC UE Context */
        )
{
    U32 erab_count = 0;

    for(erab_count = 0; erab_count < MAX_ERAB_COUNT; erab_count++)
    {
        if(PNULL != p_ue_context->p_e_rab_list[erab_count])
        {
            p_ue_context->p_e_rab_list[erab_count]->uecc_x2ap_drb_ctx.e_RAB_ID = 
                p_ue_context->p_e_rab_list[erab_count]->e_RAB_ID;
        }
    }
}


/*SPR_19066_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_get_ho_prep_info_ongoing_x2ap_reset_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_X2AP_RESET_MSG_IND
 ****************************************************************************/
static 
void uecc_fsm_src_ho_get_ho_prep_info_ongoing_x2ap_reset_req_handler(
/*SPR_19066_END*/
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);
    /*BUG 604 changes start*/
    if((INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "X2ap Reset Indication received. "); 
    
    if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
 
        /*SPR_16618_START*/
        /* Continue failure processing */
        /*uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type1_actions);*/
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type2_actions);
        /*SPR_16618_END*/
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_prep_ongoing_x2ap_reset_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_X2AP_RESET_MSG_IND message.
 ****************************************************************************/
static 
void uecc_fsm_src_ho_prep_ongoing_x2ap_reset_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*BUG 604 changes start*/
    if((INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "X2AP Reset message received during HO Preparation.");

    /* filling cause value  and stopping HO Prep Timer */
    if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
    
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
        /* Continue failure processing */
        /*SPR_16618_START*/
        /* Send Release Ind to X2AP Module */
        uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
        /*uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_src_ho_fail_type1_actions);*/
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type2_actions);
        /*SPR_16618_END*/
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_cmd_ongoing_x2ap_reset_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_X2AP_RESET_MSG_IND message.
 ****************************************************************************/
static void uecc_fsm_src_ho_cmd_ongoing_x2ap_reset_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*BUG 604 changes start*/
    if((INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
          (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "X2ap Reset recieved.");
    /* filling the cause in p_ue_context */
    if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
        
        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_X2_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data );

        /* Send Release Ind to X2AP Module */
        uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
        /*SPR_16618_START*/
        /* Continue failure processing */
        /*uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type1_actions);*/
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type2_actions);
        /*SPR_16618_END*/
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_re_est_ongoing_x2ap_reset_msg_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_llim_re_est_ongoing_x2ap_reset_msg_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*BUG 604 changes start*/
    if((INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "X2ap Reset recieved.");
    /* filling the cause in p_ue_context */
    if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;

        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_X2_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data );

        /* Send Release Ind to X2AP Module */
        uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
        /*SPR_16618_START*/
        /* Continue failure processing */
        /*uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type1_actions);*/
        uecc_fsm_start_failure_processing(
                p_ue_context,
        /*SPR_17606_START*/
                uecc_fsm_src_ho_fail_type43_actions);
        /*SPR_17606_END*/
        /*SPR_16618_END*/
    }
        
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_enb_status_transfer_x2ap_reset_msg_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_X2AP_RESET_MSG_IND message.
 ****************************************************************************/
static void uecc_fsm_src_ho_enb_status_transfer_x2ap_reset_msg_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);


    /*BUG 604 changes start*/
    if((INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "X2ap Reset recieved.");
    /* filling the cause in p_ue_context */
    if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) 
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;

        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_X2_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data );

        /* Send Release Ind to X2AP Module */
        uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);

        /*SPR:6462 start*/
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

        if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                    p_ue_context))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                    "Failed to send the reestablish complete indication to LLIM");
        }
        /*SPR_16618_START*/
	RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
			"FAILURE: Send RRC_RRM_HO_FAILURE");

	if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_failure( p_ue_context))
	{
		RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
				"Fail to send the handover failure indication to RRM");
	}
        /*SPR_16618_END*/
        p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE34;
        /*SPR:6462 stop*/
        /*uecc_fsm_start_failure_processing(
          p_ue_context,
          uecc_fsm_src_ho_fail_type34_actions);*/
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_s1u_reconfig_x2ap_reset_msg_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_X2AP_RESET_MSG_IND message.
 ****************************************************************************/
static void uecc_fsm_src_ho_s1u_reconfig_x2ap_reset_msg_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

/*BUG 604 changes start*/
    if((INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
          (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes start*/
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "X2ap Reset recieved.");
    /* filling the cause in p_ue_context */
    
    /*x2_ho_timer*/
    /* stop timer UECC_UE_X2_HO_OVERALL_TIMER */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);

    if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) 
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = 
            x2ap_unspecified_2;
    
        p_ue_context->x2ap_reset_recvd_flag = RRC_TRUE;

        /* Continue failure processing */
        UECC_FSM_SET_CHILD_STATE(p_ue_context,
                &uecc_fsm_src_ho_s1u_delete_forwarding_tunnel_ongoing,
                PNULL);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_srb_data_and_wait_for_ctx_release_x2ap_reset_msg_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_X2AP_RESET_MSG_IND message.
 ****************************************************************************/
static void uecc_fsm_src_ho_wait_for_srb_data_and_wait_for_ctx_release_x2ap_reset_msg_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

/*BUG 604 changes start*/
    if((INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    /*SPR_17606_START*/
    /*Code Removed */
    /*SPR_17606_END*/

    if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        /* Send Release Ind to X2AP Module */
        uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*BUG 604 changes start*/
/*BUG 604 changes stop*/
/****************************************************************************
 * Function Name  : uecc_fsm_cco_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_CCO_TIMER_EXPIRY
 ****************************************************************************/
static 
void uecc_fsm_cco_ongoing_timer_expiry_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    { 
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"CCO HO Time out."); 

        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type33_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cco_wait_for_srb_data_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_CCO_TIMER_EXPIRY
 ****************************************************************************/
static 
void uecc_fsm_cco_wait_for_srb_data_timer_expiry_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    s1ap_Cause s1ap_cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);


    if(CCO_HO == p_ue_context->ho_info.s1_or_x2_handover)
    { 
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"CCO procedure complete."); 

        memset_wrapper(&s1ap_cause, RRC_NULL, sizeof(s1ap_Cause));
        /* SPR_7888_fix */
        if(RRC_SUCCESS != convert_rrm_cause_to_s1ap_cause(
                &p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                  s1_src_ho_info.p_rrc_rrm_ho_required->cause, 
                &s1ap_cause))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid Cause received from RRM");
        }
        /* SPR_7888_fix */
        /* sending UE Context Release to MME */
        if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                    p_ue_context, &s1ap_cause))
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                    "[%s]Fail to send the context release request to s1ap", __FUNCTION__);
        }

        p_ue_context->ho_info.p_ho_info->ho_status = HO_SUCCESS;
        SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);
        /* finish fsm successfully */
        uecc_fsm_finish_activity_success(p_ue_context,PNULL);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_reset_ue_entity_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_reset_ue_entity_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);


    /* Build and sent the reset ue entity request to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_reset_ue_entity_req(p_ue_context))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, 
                "[UECC_LLIM_RESET_UE_ENTITY_REQ]"
                "Message Delivery Failed");
        if(!p_ue_context->ue_ctx_mod_security_flag)
        {
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type33_actions);
        }
        /*securitykey*/
        else
        {
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type42_actions);
        }
        /*securitykey*/
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_reset_ue_entity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_LLIM_RESET_UE_ENTITY_RESP message.
 ****************************************************************************/
void uecc_fsm_reset_ue_entity_resp_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_reset_ue_entity_cnf_t*
        p_uecc_llim_reset_ue_entity_cnf = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_uecc_llim_reset_ue_entity_cnf =
        (rrc_uecc_llim_reset_ue_entity_cnf_t*)p_api;

    switch (p_uecc_llim_reset_ue_entity_cnf->response)
    {

        case RRC_SUCCESS:
            
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                "[UECC_EV_LLIM_RESET_UE_ENTITY_RESP]"
                " Successful Response");
/*BUG 604 changes start*/
            if (CCO_HO == p_ue_context->ho_info.s1_or_x2_handover)
            {
                UECC_FSM_SET_CHILD_STATE(p_ue_context,
                 &uecc_fsm_src_ho_wait_for_srb_data_status_ind, PNULL);
            }
/*BUG 604 changes stop*/
            break ;
        
        case RRC_FAILURE :
            
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RESET_UE_ENTITY_RESP] Failure Response");

            break;
        
        default :
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RESET_UE_ENTITY_RESP] Invalid Response=%i",
                 p_uecc_llim_reset_ue_entity_cnf->response);
    }

    /* Process failure */
    if (RRC_SUCCESS != p_uecc_llim_reset_ue_entity_cnf->response)
    {
        /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(p_ue_context,
                    uecc_fsm_src_ho_fail_type33_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*BUG 604 changes start*/
/*BUG 604 changes stop*/
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_wait_for_data_status_ind_timer_expiry_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_wait_for_data_status_ind_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "HO Overall Time out.");

/*BUG 604 changes start*/
    if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
/*BUG 604 changes stop*/
    { 
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        return;
    }

    p_ue_context->ho_info.p_ho_info->ho_status = HO_SUCCESS;

    /* enabling the rrc_connection_release_not_req_flag */
    SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

    /* filling the cause in p_ue_context */
/*BUG 604 changes start*/
    if ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        /*SPR_16778_START*/
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = 
            s1ap_tx2relocoverall_expiry;
        /*SPR_16778_END*/
        if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
            /* Send Release Ind to X2AP Module */
            uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
        }
        
        /* failure action processing */
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_src_ho_fail_type20_actions);
    }
    else
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork =
            s1ap_tS1relocoverall_expiry;

        /* failure action processing */
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_src_ho_fail_type20_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*BUG 604 changes start*/
/*BUG 604 changes stop*/
/* coverity_fix_62556_start */
/* coverity_fix_62556_stop */

/**********************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_send_enb_ue_ctx_release_req 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : The funtion sends UE Context release request to S1AP.
 **********************************************************************************/
void uecc_fsm_src_ho_fail_action_send_enb_ue_ctx_release_req(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    s1ap_Cause s1ap_cause;

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO, "SENDING UE context Release Request");

    /* Filling s1ap_cause */
    s1ap_cause.t = T_s1ap_Cause_radioNetwork;
    s1ap_cause.u.radioNetwork = s1ap_unspecified_2;

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    if(RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                p_ue_context, &s1ap_cause))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR, "[%s] Fail to send UE CONTEXT RELEASE to S1AP",__func__);      
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/*securitykey*/
/**********************************************************************************
 * Function Name  : uecc_fsm_src_ho_cleanup_ue_ctx_mod_fsm_data 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : The funtion does the cleanup of procedural data of UE
 *                  Context Modification FSM and resets any flags set. 
 **********************************************************************************/
static void uecc_fsm_src_ho_cleanup_ue_ctx_mod_fsm_data(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_break);

    /* Reset the UE Context Modificatin Security Flag */
    p_ue_context->ue_ctx_mod_security_flag = RRC_FALSE;

    /* Reset the Intra-Cell HO Success Flag as Security Rekeying has
     * successfully done */
    /*BUG 604 changes start*/
    p_ue_context->ue_ctx_mod_security_flag = RRC_FALSE;
    /*BUG 604 changes stop*/

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /*Reset flags from current procedure data */
        p_ue_context->p_curr_proc_data->m.AggMaxBitRate = RRC_FALSE;
        p_ue_context->p_curr_proc_data->m.SecurityCapability_present = RRC_FALSE;
        p_ue_context->p_curr_proc_data->m.SecurityKey_present = RRC_FALSE;
        /* CSR#59144 fix start */
        p_ue_context->p_curr_proc_data->m.T304_timer_rcvd_frm_rrm = RRC_FALSE;
        /* CSR#59144 fix stop */

        /* Freed if any message is buffered */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
        rrc_mem_free(p_ue_context->p_curr_proc_data);
    }
    p_ue_context->p_curr_proc_data = PNULL;
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*securitykey*/

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_fail_action_send_llim_reestab_complete_ind 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Sending reestablish complete indication to LLIM 
 ****************************************************************************/
static void uecc_fsm_src_ho_fail_action_send_llim_reestab_complete_ind(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_ to LLIM");

    if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                p_ue_context))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                "Failed to send the reestablish complete indication to LLIM");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_ho_src_ho_cmd_ongoing_re_establishment_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for re establishment request 
 ****************************************************************************/

static 
void uecc_fsm_src_ho_llim_ho_src_ho_cmd_ongoing_re_establishment_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{

    U16     len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* checking the current child fsm state */

    /*x2_ho_timer*/
    /* stop overall timer */
    /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);*/

    /* filling the cause in p_ue_context */
    /*BUG 604 changes start*/
    if ((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
        (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
    /*BUG 604 changes stop*/
    {
        /*x2_ho_timer*/
        /* stop overall timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_X2_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
    }
    else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        /*x2_ho_timer*/
        /* stop overall timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_OVERALL_TIMER, &p_ue_context->timers_data);

        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost; /* SPR 2591 Start */
    }
    /*BUG 604 changes start*/
    /*else if (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_INTRA_CELL_HO_GUARD_TIMER, &p_ue_context->timers_data);
    }*/
    /*BUG 604 changes stop*/


    len = (U16)(sizeof(RRCConnectionReestablishmentRequest_r8_IEs));
    if (PNULL == (p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->p_re_establishment_req =
                rrc_mem_get(len)))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL,
                "Memory Allocation Failed to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* storing re_establishment contents into p_re_establishment_req */
    l3_memcpy_wrapper(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
            p_re_establishment_req,
            p_api,
            len);

    /*BUG 604 changes start*/
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type36_actions);
    //}
    /*BUG 604 changes stop*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_re_est_ongoing_rrm_ho_cancel_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for handover cancel request from the RRM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_llim_re_est_ongoing_rrm_ho_cancel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    rrc_rrm_ho_cancel_req_t *rrc_rrm_ho_cancel_req
                         =(rrc_rrm_ho_cancel_req_t*)p_api;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*BUG 604 changes start*/
    if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    /*BUG 604 changes stop*/
    { 
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        return;
    }
    else
    {
        /* updating the cause in p_ue_context */
    /*BUG 604 changes start*/
        if ((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
                (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
    /*BUG 604 changes stop*/
        {
            if (RRC_SUCCESS != fill_x2ap_cause(p_ue_context->p_gb_context,
                        &(p_ue_context->ho_info.x2ap_cause),
                        &(rrc_rrm_ho_cancel_req->rrm_cause)))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "Fail to fill x2ap cause received from RRM");
                /*SPR_16778_START*/
                p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
                /*SPR_16778_END*/
                /*x2_ho_timer*/
                uecc_ue_timer_stop(p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_OVERALL_TIMER, 
                        &p_ue_context->timers_data);
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type37_actions);

                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
            }
            /*x2_ho_timer*/
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_X2_HO_OVERALL_TIMER, 
                    &p_ue_context->timers_data);
        }
        else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
            /*SPR_16778_START*/
            if(RRC_SUCCESS != fill_s1ap_cause(&(p_ue_context->ho_info.cause), 
                        &(rrc_rrm_ho_cancel_req->rrm_cause),p_ue_context))
            {    
                p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                p_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
            }
            /*SPR_16778_END*/
            /*x2_ho_timer*/
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_HO_OVERALL_TIMER, 
                    &p_ue_context->timers_data);
        }
        /* setting is_rrm_ho_cancel_receive variable */
        p_ue_context->ho_info.p_ho_info->p_src_ho_info->is_rrm_ho_cancel_receive = 1;

        /*x2_ho_timer*/
        /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, 
          &p_ue_context->timers_data);*/

        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type41_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_cmd_ongoing_llim_radio_link_failure_ind_handler  
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for radio link failure event received from LLIM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_cmd_ongoing_llim_radio_link_failure_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* filling the cause in p_ue_context */
    /*BUG 604 changes start*/
    if ((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
        (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
    /*BUG 604 changes stop*/
    {
        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
        /*x2_ho_timer*/
        /* stopping timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_X2_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data);
    }
    else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        /*SPR_16778_START*/
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost;
        /*SPR_16778_END*/
        /*x2_ho_timer*/
        /* stopping timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data);
    }
    /*BUG 604 changes start*/
    /*else if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_INTRA_CELL_HO_GUARD_TIMER,
                &p_ue_context->timers_data);
    }*/
    /*BUG 604 changes stop*/
    /* storing re_establishment contents into p_re_establishment_req */
    p_ue_context->ho_info.p_ho_info->p_src_ho_info->p_radio_link_failure_ind
        = p_api;

    /*x2_ho_timer*/
    /* stopping timer */
    /*uecc_ue_timer_stop(UECC_UE_HO_OVERALL_TIMER, 
      &p_ue_context->timers_data);*/
    
    /*BUG 604 changes start*/
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type18_actions);
    //}
    /*BUG 604 changes stop*/
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_re_est_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_src_ho_llim_re_est_ongoing_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);


    /*BUG 604 changes start*/
    if(CCO_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    /*BUG 604 changes stop*/
    { 
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        return;
    }
    else
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "HO Overall Time out.");
        /* filling the cause in p_ue_context */
    /*BUG 604 changes start*/
        if ((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) || 
                (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)) 
    /*BUG 604 changes stop*/
        {
            p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_tx2relocoverall_expiry;

            /*SPR_16618_START*/
            /* Send Release Ind to X2AP Module */
            /*Code Removed */
            /*SPR_16618_END*/
        }
        else if (S1_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
            p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_ue_context->ho_info.cause.u.radioNetwork = s1ap_tS1relocoverall_expiry;
        }
        /* Continue failure processing */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_src_ho_fail_type40_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/**********************************************************************************
 *  Function Name  : uecc_fsm_rrc_ul_handover_preparation_transfer_handler
 *  Inputs         : uecc_ue_context_t* - pointer to UE context
 *                   void               *p_api
 *  Outputs        : None
 *  Returns        : None
 *  Description    : The funtion sends Handover Preparation transfer to uecc.
 ***********************************************************************************/

static void  uecc_fsm_rrc_ul_handover_preparation_transfer_handler (
        uecc_ue_context_t  *p_ue_context,
        void               *p_api
        )
{
    ULHandoverPreparationTransfer_r8_IEs *p_asn_ul_hand_resp = PNULL;
    rrc_rrm_ho_required_t           *p_ho_reqd = PNULL;
    
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api);
    
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    p_asn_ul_hand_resp = (ULHandoverPreparationTransfer_r8_IEs*)p_api;

    p_ho_reqd = p_ue_context->ho_info.p_ho_info->p_src_ho_info->
            s1_src_ho_info.p_rrc_rrm_ho_required;

    if (type1XRTT == p_asn_ul_hand_resp->cdma2000_Type)
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "CDMA2000: UL handover preparation transfer for 1xrtt");
        p_ue_context->cdma2000_info.w_f_1xrtt_msg = RRC_TRUE;

        p_ue_context->cdma2000_info.ho_prep_transfer_1xrtt = *p_asn_ul_hand_resp;

        p_ue_context->cdma2000_info.ho_prep_transfer_1xrtt.dedicatedInfo.data = 
            (void *)rrc_mem_get (p_asn_ul_hand_resp->dedicatedInfo.numocts);
        if (PNULL != p_ue_context->cdma2000_info.ho_prep_transfer_1xrtt.dedicatedInfo.data)
        {
            /* SES-508 Fix Start */
            /* Code Removed */
            /* SES-508 Fix End */
            l3_memcpy_wrapper ((void *)p_ue_context->cdma2000_info.ho_prep_transfer_1xrtt.dedicatedInfo.data,
                    (void *)p_asn_ul_hand_resp->dedicatedInfo.data,
                    p_asn_ul_hand_resp->dedicatedInfo.numocts);
        }
    }
	
    if (typeHRPD == p_asn_ul_hand_resp->cdma2000_Type)
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "CDMA2000: UL handover preparation transfer for hrpd");
        p_ue_context->cdma2000_info.w_f_hrpd_msg = RRC_TRUE;
        
        p_ue_context->cdma2000_info.ho_prep_transfer_hrpd = *p_asn_ul_hand_resp;
        
        p_ue_context->cdma2000_info.ho_prep_transfer_hrpd.dedicatedInfo.data = 
		(void *)rrc_mem_get (p_asn_ul_hand_resp->dedicatedInfo.numocts);
        
        if (PNULL != p_ue_context->cdma2000_info.ho_prep_transfer_hrpd.dedicatedInfo.data)
        {
            /* SES-508 Fix Start */
            /* Code Removed */
            /* SES-508 Fix End */

            l3_memcpy_wrapper ((void *)p_ue_context->cdma2000_info.ho_prep_transfer_hrpd.dedicatedInfo.data,
                    (void *)p_asn_ul_hand_resp->dedicatedInfo.data,
                    p_asn_ul_hand_resp->dedicatedInfo.numocts);
        }
    }

    if ((RRC_TRUE == p_ue_context->cdma2000_info.w_f_1xrtt_msg) && 
            (RRC_TRUE == p_ue_context->cdma2000_info.w_f_hrpd_msg))
    {
        if ((HANDOVER_TYPE_LTE_TO_CDMA2000_1XRTT == p_ho_reqd->ho_type) ||
                (HANDOVER_TYPE_LTE_TO_CDMA2000_CONC_1XRTT_HRPD == p_ho_reqd->ho_type))
        {
            /*Fill meid in UE Context for UL Handover Preparation Transfer */
            p_ue_context->cdma2000_info.ulhandoverPrepTransfer_r8_IEs_meid_numbits =
                MEID_NBITS;
            l3_memcpy_wrapper(&p_ue_context->\
                    cdma2000_info.ulhandoverPrepTransfer_r8_IEs_meid_data,
                    &p_asn_ul_hand_resp->meid.data,
                    (MEID_NBITS/8));

            /* build and send upling s1 cdma2000 tunneling to s1ap module */
            if(RRC_SUCCESS != uecc_s1ap_build_and_send_uplink_s1_cdma2000_tunneling(
                        p_ue_context,
                        &p_ue_context->cdma2000_info.ho_prep_transfer_1xrtt.dedicatedInfo,
                        T_ULInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_1XRTT))	
            { 
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                        "CDMA2000:[uplink_s1_cdma2000_tunneling] delivery Fail");
                /* Continue failure processing */
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type2_actions);
	            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
            }	
            else
            {
                p_ue_context->cdma2000_info.w_f_1xrtt_msg = RRC_FALSE;
            }
	   
           /*free context*/
           if (PNULL != p_ue_context->cdma2000_info.ho_prep_transfer_1xrtt.dedicatedInfo.data)
          {
                 rrc_mem_free((void *)p_ue_context->cdma2000_info.
                              ho_prep_transfer_1xrtt.dedicatedInfo.data);
                 
                 p_ue_context->cdma2000_info.ho_prep_transfer_1xrtt.
                                       dedicatedInfo.data = PNULL ;       
          }
        }
        if ((HANDOVER_TYPE_LTE_TO_CDMA2000_HRPD == p_ho_reqd->ho_type) ||
                (HANDOVER_TYPE_LTE_TO_CDMA2000_CONC_1XRTT_HRPD == p_ho_reqd->ho_type))
        {
            /* build and send upling s1 cdma2000 tunneling to s1ap module */
            if(RRC_SUCCESS != uecc_s1ap_build_and_send_uplink_s1_cdma2000_tunneling(
                        p_ue_context,
                      &p_ue_context->cdma2000_info.ho_prep_transfer_hrpd.dedicatedInfo,
                        T_ULInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_HRPD))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                        "CDMA2000:[uplink_s1_cdma2000_tunneling] delivery Fail");
                /* Continue failure processing */
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_src_ho_fail_type2_actions);
	            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
            }
            else
            {
                p_ue_context->cdma2000_info.w_f_hrpd_msg = RRC_FALSE;
            }
           
           /*free context*/
           if (PNULL != p_ue_context->cdma2000_info.ho_prep_transfer_hrpd.dedicatedInfo.data )
           {
              rrc_mem_free((void *)p_ue_context->cdma2000_info.
                            ho_prep_transfer_hrpd.dedicatedInfo.data);
           
              p_ue_context->cdma2000_info.ho_prep_transfer_hrpd.
                                    dedicatedInfo.data = PNULL ;       
          }
        }
        /* moving to next state */
        UECC_FSM_SET_CHILD_STATE(p_ue_context,
                &uecc_fsm_src_ho_prep_ongoing, PNULL);
    } 
    else
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                "CDMA2000:waiting for ho_prep_transfer_msg for other domain");
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/**********************************************************************************
 *  Function Name  : uecc_fsm_src_s1ap_dl_cdma2000_tunneling_ho_ongoing_handler
 *  Inputs         : uecc_ue_context_t* - pointer to UE context
 *                   void               *p_api_data
 *  Outputs        : None
 *  Returns        : None
 *  Description    : The funtion sends Handover Preparation transfer to uecc.
 ***********************************************************************************/
static void uecc_fsm_src_s1ap_dl_cdma2000_tunneling_ho_ongoing_handler(
        uecc_ue_context_t  *p_ue_context,           /* UECC UE Context */
        void               *p_api_data              /* Input API Buffer */
        )
{
    rrc_rrm_ho_required_t   *p_ho_reqd = PNULL;
    U8 result = RRC_SUCCESS;
    s1ap_Cause s1ap_cause;

    rrc_s1ap_downlink_s1_cdma2000_tunneling_t *p_dl_s1_cdma2000_tunneling = PNULL;

    RRC_ASSERT(PNULL!=p_ue_context->p_gb_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_dl_s1_cdma2000_tunneling =
        (rrc_s1ap_downlink_s1_cdma2000_tunneling_t*)p_api_data;

    p_ho_reqd = p_ue_context->ho_info.p_ho_info->p_src_ho_info->
        s1_src_ho_info.p_rrc_rrm_ho_required;


    if (RRC_TRUE == p_ue_context->cdma2000_info.timer_expired_f_1xrtt)
    {
        p_ue_context->cdma2000_info.w_f_1xrtt_msg = RRC_TRUE;
    }
    else if (RRC_TRUE == p_ue_context->cdma2000_info.timer_expired_f_hrpd)
    {
        p_ue_context->cdma2000_info.w_f_hrpd_msg = RRC_TRUE;
    }
    else
    {
        if (s1ap_onexRTT == p_dl_s1_cdma2000_tunneling->cdma2000_rat_type)
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "CDMA2000: DL S1 CDMA2000 Tunneling for 1xrtt");
            p_ue_context->cdma2000_info.w_f_1xrtt_msg = RRC_TRUE;
            p_ue_context->cdma2000_info.dl_s1_cdma2000_tunneling_1xrtt = 
                *p_dl_s1_cdma2000_tunneling;

            p_ue_context->cdma2000_info.dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu.data = 
                (void *)rrc_mem_get (p_dl_s1_cdma2000_tunneling->cdma2000_pdu.numocts);
            if (PNULL != p_ue_context->cdma2000_info.dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu.data)
            {
                /* SES-508 Fix Start */
                /* Code Removed */
                /* SES-508 Fix End */
                l3_memcpy_wrapper ((void *)p_ue_context->cdma2000_info.dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu.data,
                        (void *)p_dl_s1_cdma2000_tunneling->cdma2000_pdu.data,
                        p_dl_s1_cdma2000_tunneling->cdma2000_pdu.numocts);
            }
        }
        if (s1ap_hRPD == p_dl_s1_cdma2000_tunneling->cdma2000_rat_type)
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "CDMA2000: DL S1 CDMA2000 Tunneling for hrpd");
            p_ue_context->cdma2000_info.w_f_hrpd_msg = RRC_TRUE;
            p_ue_context->cdma2000_info.dl_s1_cdma2000_tunneling_hrpd = 
                *p_dl_s1_cdma2000_tunneling;

            p_ue_context->cdma2000_info.dl_s1_cdma2000_tunneling_hrpd.cdma2000_pdu.data = 
                (void *)rrc_mem_get (p_dl_s1_cdma2000_tunneling->cdma2000_pdu.numocts);
            if (PNULL == p_ue_context->cdma2000_info.dl_s1_cdma2000_tunneling_hrpd.cdma2000_pdu.data)
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_FATAL, "[%s]Memory allocation failed", __FUNCTION__);
                return;
            }
            
            /* SES-508 Fix Start */
            /* Code Removed */
            /* SES-508 Fix End */
            l3_memcpy_wrapper ((void *)p_ue_context->cdma2000_info.dl_s1_cdma2000_tunneling_hrpd.cdma2000_pdu.data,
                    (void *)p_dl_s1_cdma2000_tunneling->cdma2000_pdu.data,
                    p_dl_s1_cdma2000_tunneling->cdma2000_pdu.numocts);


        }
    }
    if ((RRC_TRUE == p_ue_context->cdma2000_info.w_f_1xrtt_msg) && 
            (RRC_TRUE == p_ue_context->cdma2000_info.w_f_hrpd_msg))
    {    
        /* stopping HO Prep Timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);

        /* starting HO Overall Timer */
        if ( RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context,
                    UECC_UE_HO_OVERALL_TIMER, 
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type2_actions);
            return;
        }

        /* 1xrtt HO*/
        if (HANDOVER_TYPE_LTE_TO_CDMA2000_1XRTT == 
                p_ho_reqd->ho_type)
        {
            if (HO_FAILURE == p_ue_context->cdma2000_info.
                    dl_s1_cdma2000_tunneling_1xrtt.cdma2000_ho_status)
            {
                /*send dl_information_transfer for 1xrtt*/
                if(RRC_SUCCESS != uecc_llim_build_and_send_rrc_dl_information_transfer(
                            p_ue_context,
                            &p_ue_context->cdma2000_info.
                            dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu,
                            T_DLInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_1XRTT))
                {
                    s1ap_cause.t = T_s1ap_Cause_radioNetwork;
                    s1ap_cause.u.radioNetwork = s1ap_unspecified_2;

                    if (RRC_SUCCESS != uecc_s1ap_build_and_send_nas_non_delivery_ind(
                                p_ue_context->p_gb_context,
                                p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.mme_ue_s1ap_id,
                                p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.enb_ue_s1ap_id,
                                &p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu,
                                &s1ap_cause))
                    {
                        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                                "[s1ap_NASNonDeliveryIndication] delivery fail");
                    }
                }
                /*free context*/
                if (PNULL != p_ue_context->cdma2000_info.
                        dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu.data)
                {
                    rrc_mem_free((void *)p_ue_context->cdma2000_info.
                            dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu.data);

                    p_ue_context->cdma2000_info.
                        dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu.data = PNULL ;       
                }
                result = RRC_FAILURE;
            }
        }
        /* hrpd HO*/
        else if (HANDOVER_TYPE_LTE_TO_CDMA2000_HRPD == 
                p_ho_reqd->ho_type)
        {
            if (HO_FAILURE == p_ue_context->cdma2000_info.
                    dl_s1_cdma2000_tunneling_hrpd.cdma2000_ho_status)
            {
                /*send dl_information_transfer for hrpd*/
                if(RRC_SUCCESS != uecc_llim_build_and_send_rrc_dl_information_transfer(
                            p_ue_context,
                            &p_ue_context->cdma2000_info.
                            dl_s1_cdma2000_tunneling_hrpd.cdma2000_pdu,
                            T_DLInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_HRPD))
                {
                    s1ap_cause.t = T_s1ap_Cause_radioNetwork;
                    s1ap_cause.u.radioNetwork = s1ap_unspecified_2;

                    if (RRC_SUCCESS != uecc_s1ap_build_and_send_nas_non_delivery_ind(
                                p_ue_context->p_gb_context,
                                p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.mme_ue_s1ap_id,
                                p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.enb_ue_s1ap_id,
                                &p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu,
                                &s1ap_cause))
                    {
                        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                                "[s1ap_NASNonDeliveryIndication] delivery fail");
                    }
                }
                /*free context*/
                if (PNULL != p_ue_context->cdma2000_info.
                        dl_s1_cdma2000_tunneling_hrpd.cdma2000_pdu.data)
                {
                    rrc_mem_free((void *)p_ue_context->cdma2000_info.
                            dl_s1_cdma2000_tunneling_hrpd.cdma2000_pdu.data);

                    p_ue_context->cdma2000_info.
                        dl_s1_cdma2000_tunneling_hrpd.cdma2000_pdu.data = PNULL ;       
                }
                result = RRC_FAILURE;
            }
        }
        /* concurrent 1xrtt-hrpd HO*/
        else if (HANDOVER_TYPE_LTE_TO_CDMA2000_CONC_1XRTT_HRPD == 
                p_ho_reqd->ho_type)
        {
            if ((HO_FAILURE == p_ue_context->cdma2000_info.
                        dl_s1_cdma2000_tunneling_1xrtt.cdma2000_ho_status) &&
                    (HO_FAILURE == p_ue_context->cdma2000_info.
                     dl_s1_cdma2000_tunneling_hrpd.cdma2000_ho_status))
            {
                /*send dl_information_transfer for 1xrtt*/
                if(RRC_SUCCESS != uecc_llim_build_and_send_rrc_dl_information_transfer(
                            p_ue_context,
                            &p_ue_context->cdma2000_info.
                            dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu,
                            T_DLInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_1XRTT))
                {
                    s1ap_cause.t = T_s1ap_Cause_radioNetwork;
                    s1ap_cause.u.radioNetwork = s1ap_unspecified_2;

                    if (RRC_SUCCESS != uecc_s1ap_build_and_send_nas_non_delivery_ind(
                                p_ue_context->p_gb_context,
                                p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.mme_ue_s1ap_id,
                                p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.enb_ue_s1ap_id,
                                &p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu,
                                &s1ap_cause))
                    {
                        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                                "[s1ap_NASNonDeliveryIndication] delivery fail");
                    }
                    /*free context*/
                    if (PNULL != p_ue_context->cdma2000_info.
                            dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu.data)
                    {

                        rrc_mem_free((void *)p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu.data);

                        p_ue_context->cdma2000_info.
                            dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu.data = PNULL ;       
                    }
                }
                /*send dl_information_transfer for hrpd*/
                if(RRC_SUCCESS != uecc_llim_build_and_send_rrc_dl_information_transfer(
                            p_ue_context,
                            &p_ue_context->cdma2000_info.
                            dl_s1_cdma2000_tunneling_hrpd.cdma2000_pdu,
                            T_DLInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_HRPD))
                {
                    s1ap_cause.t = T_s1ap_Cause_radioNetwork;
                    s1ap_cause.u.radioNetwork = s1ap_unspecified_2;

                    if (RRC_SUCCESS != uecc_s1ap_build_and_send_nas_non_delivery_ind(
                                p_ue_context->p_gb_context,
                                p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.mme_ue_s1ap_id,
                                p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.enb_ue_s1ap_id,
                                &p_ue_context->cdma2000_info.
                                dl_s1_cdma2000_tunneling_1xrtt.cdma2000_pdu,
                                &s1ap_cause))
                    {
                        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                                "[s1ap_NASNonDeliveryIndication] delivery fail");
                    }
                }
                /*free context*/
                if (PNULL != p_ue_context->cdma2000_info.
                        dl_s1_cdma2000_tunneling_hrpd.cdma2000_pdu.data)
                {
                    rrc_mem_free((void *)p_ue_context->cdma2000_info.
                            dl_s1_cdma2000_tunneling_hrpd.cdma2000_pdu.data);

                    p_ue_context->cdma2000_info.
                        dl_s1_cdma2000_tunneling_hrpd.cdma2000_pdu.data = PNULL ;       
                }

                result = RRC_FAILURE;
            }
        }

        if (result == RRC_SUCCESS)
        {
            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                    &uecc_fsm_src_ho_cmd_ongoing,
                    PNULL);
        }
        else
        {
            /* Continue failure processing */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_src_ho_fail_type2_actions);
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_valiating_rrm_ho_required_msg 
 * Inputs         : rrc_target_rat_type_geran_t
 *                  uecc_ue_context_t* - pointer to UE context
 * Outputs        : None  
 * Returns        : rrc_bool_t
 * Description    : This is the function for to validate the geran_params
 *                  message recevied from RRM.
 ****************************************************************************/
static rrc_bool_t uecc_fsm_validate_target_rat_geran_params (
        uecc_ue_context_t           *p_ue_context,         
        rrc_target_rat_type_geran_t *p_geran_params)
{
    U32 count = 0;
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_DETAILEDALL,"Unused variable in FSM:p_ue_context=%p",
            p_ue_context);/*SPR 17777 +-*/

    if (RRC_RRM_UE_CCO_TARGET_RAT_GERAN_TYPE_SI_PSI_PRESENT & p_geran_params->bitmask)
    {
        if (SYSTEM_INFO_LIST_GERAN_SI_PRESENT & p_geran_params->si_list.bitmask)
        {
            for(count = 0; count < p_geran_params->si_list.si.count; count++)
            {
                if (RRC_RRM_SYS_INFO_CONTAINTER_EXPANDED_PRESENT & 
                    p_geran_params->si_list.si.system_info_geran_elem[count].bitmask)
                {
                    return RRC_FALSE;
                }
            }
        }
        else if (SYSTEM_INFO_LIST_GERAN_PSI_PRESENT & p_geran_params->si_list.bitmask)
        {
            for(count = 0; count < p_geran_params->si_list.psi.count; count++)
            {
                if (RRC_RRM_SYS_INFO_CONTAINTER_EXPANDED_PRESENT & 
                    p_geran_params->si_list.psi.system_info_geran_elem[count].bitmask)
                {
                    return RRC_FALSE;
                }
            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_TRUE;
}
/*SPR:6462 start*/
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_rlc_reestab_comp_cnf_msg_handler  
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler for rlc_reestab_comp_cnf event received from LLIM 
 ****************************************************************************/

static void  uecc_fsm_src_ho_rlc_reestab_comp_cnf_msg_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    uecc_fsm_fail_handler_f const*  p_failure_actions_list =
        uecc_fsm_src_ho_fail_type4_actions;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    

    switch (p_ue_context->fail_action_type)
    {
        case RRC_UECC_FAIL_TYPE4:
            p_failure_actions_list = uecc_fsm_src_ho_fail_type4_actions;
            break;

        case RRC_UECC_FAIL_TYPE16:
            p_failure_actions_list = uecc_fsm_src_ho_fail_type16_actions;
            break;

        case RRC_UECC_FAIL_TYPE21:
            p_failure_actions_list = uecc_fsm_src_ho_fail_type21_actions;
            break;

        case RRC_UECC_FAIL_TYPE23:
            p_failure_actions_list = uecc_fsm_src_ho_fail_type23_actions;
            break;

        case RRC_UECC_FAIL_TYPE24:
            p_failure_actions_list = uecc_fsm_src_ho_fail_type24_actions;
            break;

        case RRC_UECC_FAIL_TYPE28:
            p_failure_actions_list = uecc_fsm_src_ho_fail_type28_actions;
            break;

        case RRC_UECC_FAIL_TYPE34:
            p_failure_actions_list = uecc_fsm_src_ho_fail_type34_actions;
            break;

        /* SPR 16750 Fix Start */
        /* Code Removed */
        /* SPR 16750 Fix Stop */

        default:   
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,"No failure action to be"
                   "called;Unused variable p_api=%p",p_api);/*SPR 17777 +-*/
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return;
    }

    /*Continue Failure Processing*/
    uecc_fsm_start_failure_processing(
            p_ue_context,
            p_failure_actions_list);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR:6462 stop*/

/*BUG 604 changes start*/
/*Bug 357 Fix Start*/
/*Bug 357 Fix Stop*/
/*BUG 604 changes stop*/

/*BUG 604 changes start*/
/****************************************************************************
 * Function Name  : uecc_fsm_s1u_intra_enb_data_fwd_cnf_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_LLIM_S1U_INTRA_ENB_DATA_FWD_CNF from LLIM.
 ****************************************************************************/
static void uecc_fsm_s1u_intra_enb_data_fwd_cnf_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    rrc_uecc_llim_s1u_intra_enb_data_fwd_cnf_t*
        p_rrc_uecc_llim_s1u_data_fwd_cnf = PNULL;
    rrc_return_t                    result = RRC_FAILURE;
    uecc_fsm_fail_handler_f const*  p_failure_actions_list =
        uecc_fsm_src_ho_fail_type4_actions;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_uecc_llim_s1u_data_fwd_cnf
        = (rrc_uecc_llim_s1u_intra_enb_data_fwd_cnf_t*)p_api_data;

    switch (p_rrc_uecc_llim_s1u_data_fwd_cnf->response)
    {
        case RRC_SUCCESS:
            /* Set S1U Reconfigured  */
            RRC_ASSERT(PNULL != p_ue_context->ho_info.p_ho_info);
            RRC_ASSERT(PNULL != p_ue_context->ho_info.p_ho_info->p_src_ho_info);
            p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1u_reconfigured = RRC_TRUE;

            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "[UECC_LLIM_S1U_RECONFIG_CNF] S1U Reconfigured "
                    "successfully");
            result = RRC_SUCCESS;

            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "[UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_CNF] is success");

            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                    &uecc_fsm_src_ho_wait_for_srb_data_status_ind, PNULL);
            break;

        case RRC_FAILURE:
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "[UECC_LLIM_S1U_RECONFIG_CNF] Failure "
                    "response has been received");
            if(IS_RLF_RECEIVED(p_ue_context))
            {
                p_failure_actions_list = uecc_fsm_src_ho_fail_type19_actions;
            }        
            else
            {
                /* fail to reconfigure the s1u */
                /*SPR:6462 start*/
                p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE4;  
                //p_failure_actions_list = uecc_fsm_src_ho_fail_type4_actions;
                /*SPR:6462 stop*/
            }
            break;

        default:
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_S1U_RECONFIG_CNF] Invalid Response=%i",
                    p_rrc_uecc_llim_s1u_data_fwd_cnf->response);
            break;
    }

    /* Process failure */
    if (RRC_FAILURE == result)
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_X2_HO_OVERALL_TIMER, 
                &p_ue_context->timers_data );

        p_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
        /* Process failure according to selected failure actions list */
        /*SPR:6462 start*/
        if(RRC_UECC_FAIL_TYPE4 != p_ue_context->fail_action_type)
        {
            uecc_fsm_start_failure_processing(p_ue_context,
                    p_failure_actions_list);
        }
        else
        {
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

            if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                        p_ue_context))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                        "Failed to send the reestablish complete indication to LLIM");
            }
        }
        /*SPR:6462 stop */
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_s1u_local_path_switch_delete_forwarding_cnf_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_LLIM_S1U_LOCAL_PATH_SWITCH_CNF from the LLIM.
 ****************************************************************************/
static void uecc_fsm_s1u_local_path_switch_delete_forwarding_cnf_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    rrc_uecc_llim_s1u_local_path_switch_cnf_t*
        p_rrc_uecc_llim_s1u_path_switch_cnf = PNULL;
    uecc_fsm_fail_handler_f const*  p_failure_actions_list =
        uecc_fsm_src_ho_fail_type4_actions;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_uecc_llim_s1u_path_switch_cnf
        = (rrc_uecc_llim_s1u_local_path_switch_cnf_t*)p_api_data;

    switch (p_rrc_uecc_llim_s1u_path_switch_cnf->response)
    {
        case RRC_SUCCESS:
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
                    "[UECC_LLIM_S1U_LOCAL_PATH_SWITCH_CNF] Sending LOCAL_PATH_SWITCH " 
                    "for delete forwarding tunnels");
            /* if RE_EST_REQ_RECEIVED */
            if( IS_RE_ESTABLISHMENT_REQ_RECEIVED(p_ue_context) )
            {
                p_failure_actions_list = uecc_fsm_src_ho_fail_type10_actions;                                
                /*SPR:6462 start*/
                
                uecc_fsm_start_failure_processing(p_ue_context,
                        p_failure_actions_list);
                
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
                /*SPR:6462 stop*/
            }
            else if(IS_RLF_RECEIVED(p_ue_context))
            {
                RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                        "Failure action for RLF recieved");
                p_failure_actions_list = uecc_fsm_src_ho_fail_type19_actions;    
                /*SPR:6462 start*/
                
                uecc_fsm_start_failure_processing(p_ue_context,
                        p_failure_actions_list);
                
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
                /*SPR:6462 stop*/
            }
            else if(IS_RRM_HO_CANCEL_RECEIVED(p_ue_context))
            {
                /*SPR:6462 start*/
                p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE28;
                //p_failure_actions_list = uecc_fsm_src_ho_fail_type16_actions;
                /*SPR:6462 stop*/
            }
            else if(IS_RESET_RECEIVED(p_ue_context))
            {
                /*SPR:6462 start*/
                p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE34;
                //p_failure_actions_list = uecc_fsm_src_ho_fail_type34_actions;
                /*SPR:6462 stop*/
                p_ue_context->x2ap_reset_recvd_flag = RRC_FALSE;
            }
            break;
        case RRC_FAILURE:
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_S1U_LOCAL_PATH_SWITCH_CNF] S1U PATH SWITCH failure for "
                    "delete forwarding tunnels");
            /*  set rrc_connection_release_not_required_flag */
            SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);
            /*SPR:6462 start*/
            p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE24;
            //p_failure_actions_list = uecc_fsm_src_ho_fail_type24_actions;                      
                /*SPR:6462 stop*/
            if ( IS_RE_ESTABLISHMENT_REQ_RECEIVED(p_ue_context) ) 
            {
                if( RRC_SUCCESS != 
                        uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                            p_ue_context->p_gb_context, p_ue_context->crnti,
                            p_ue_context->reestablishmentCause,
                            p_ue_context->cell_index, RRC_UECCMD_MODULE_ID) )
                {
                    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                            "[UECC_LLIM_S1U_RECONFIG_CNF] FAIL TO SEND RRC " 
                            "Connection ReEstablishment reject to UE");
                }
            }
            else if(IS_RRM_HO_CANCEL_RECEIVED(p_ue_context))
            {
            /*SPR:6462 start*/
                p_ue_context->fail_action_type = RRC_UECC_FAIL_TYPE23;
                //    p_failure_actions_list = uecc_fsm_src_ho_fail_type23_actions;
                /*SPR:6462 stop*/
            }
            break;
        default:
            RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_S1U_RECONFIG_CNF] INVALID RESPONSE CODE");
            break;
    }
        /*uecc_fsm_start_failure_processing(p_ue_context,
          p_failure_actions_list);*/
    /*SPR:6462 start*/
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_INFO,
            "FAILURE: Send UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND_RESP to LLIM");

    if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind( 
                p_ue_context))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_ERROR,
                "Failed to send the reestablish complete indication to LLIM");
    }
    /*SPR:6462 stop*/
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*BUG 604 changes stop*/
/* SPR 16750 Fix Start */
/****************************************************************************
 * Function Name  : uecc_fsm_src_ho_llim_re_est_x2ap_error_indication_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_X2AP_ERROR_INDICATION message.
 ****************************************************************************/
static void uecc_fsm_src_ho_llim_re_est_x2ap_error_indication_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    rrc_x2ap_error_indication_t* p_rrc_x2ap_error_indication= PNULL;
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    p_rrc_x2ap_error_indication
        = (rrc_x2ap_error_indication_t*)p_api;

    if((INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
    {
        RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    RRC_UECC_SRC_HO_FSM_TRACE(RRC_WARNING, "X2 HO Error-Indication.");

    p_ue_context->ho_info.p_ho_info->ho_status = HO_FAILURE;

    /*Handling this function only if the procedureCode is sn status transfer*/
    if ( ((T_x2ap_Cause_radioNetwork &  p_rrc_x2ap_error_indication->cause.t) &&
                ((x2ap_unknown_new_eNB_UE_X2AP_ID &  p_rrc_x2ap_error_indication->cause.u.radioNetwork) ||
                 (x2ap_unknown_old_eNB_UE_X2AP_ID &  p_rrc_x2ap_error_indication->cause.u.radioNetwork) ||
                 (x2ap_unknown_pair_of_UE_X2AP_ID &  p_rrc_x2ap_error_indication->cause.u.radioNetwork))) ||
            (ASN1V_x2ap_id_snStatusTransfer == p_rrc_x2ap_error_indication->criticality_diagnostics.procedureCode))

    {
        if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) 
        {
            /* Stop timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_X2_HO_OVERALL_TIMER,
                    &p_ue_context->timers_data );

            /* filling cause */
            p_ue_context->ho_info.x2ap_cause= p_rrc_x2ap_error_indication->cause;

            /* Send Release Ind to X2AP Module */
            uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);

            /* failure action processing */
            uecc_fsm_start_failure_processing(p_ue_context,
                    uecc_fsm_src_ho_fail_type20_actions);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* SPR 16750 Fix Stop */

/*SPR_19166_START*/
/****************************************************************************
 * Function Name  : rrc_compare_inter_rat_target_cgi 
 * Inputs         : target_cgi  - cgi of inter rat cells
 *                  p_rrc_rrm_ho_required - pointer to ho_required msg
 * Outputs        : None
 * Returns        : result of comparison
 * Description    : This function compares the target cgi from the cgi details 
 *                  present in ho required msg
 ****************************************************************************/
S32 rrc_compare_inter_rat_target_cgi(
        rrc_inter_rat_global_cgi_t target_cgi,
        rrc_rrm_ho_required_t *p_rrc_rrm_ho_required) 
{
    rrm_target_id_t target_id = p_rrc_rrm_ho_required->target_id;
    U8 temp_cell_identity[4]={0};

    if((RRM_TARGET_ID_RNC_ID_PRESENT & target_id.bitmask) && (RRC_TARGET_ID_UTRAN_GERAN_CGI_PRESENT & target_cgi.bitmask))
    {
        if(!memcmp_wrapper(&target_cgi.cgi_info.plmn_identity,&target_id.rnc_id.lai.plmn_identity,
                sizeof(plmn_identity_t)))
        {
            temp_cell_identity[0] = 
                (U8)((p_rrc_rrm_ho_required->source_to_target_container.rncContainer.target_cell_id & 0xFF000000) >> 24);
            temp_cell_identity[1] = 
                (U8)((p_rrc_rrm_ho_required->source_to_target_container.rncContainer.target_cell_id & 0x00FF0000) >> 16);
            temp_cell_identity[2] = 
                (U8)((p_rrc_rrm_ho_required->source_to_target_container.rncContainer.target_cell_id & 0x0000FF00) >> 8);
            temp_cell_identity[3] = 
                (U8)((p_rrc_rrm_ho_required->source_to_target_container.rncContainer.target_cell_id & 0x000000FF));
            
            return memcmp_wrapper(&target_cgi.cgi_info.cell_identity,
                    &temp_cell_identity,
                    MAX_CELL_IDENTITY_NUMBYTES);
        }
        else
            return -1;
    }
    else if((RRM_TARGET_ID_CGI_PRESENT & target_id.bitmask) && (RRC_TARGET_ID_UTRAN_GERAN_CGI_PRESENT & target_cgi.bitmask))
    {
        if(!memcmp_wrapper(&target_cgi.cgi_info.plmn_identity,&target_id.cgi.plmn_identity,
                sizeof(plmn_identity_t)))
        {
            temp_cell_identity[0] = target_id.cgi.location_area_code[1];
            temp_cell_identity[1] = target_id.cgi.location_area_code[0];
            temp_cell_identity[2] = target_id.cgi.ci_info[1];
            temp_cell_identity[3] = target_id.cgi.ci_info[0];
            temp_cell_identity[3] &= 0x0F;

            return memcmp_wrapper(&target_cgi.cgi_info.cell_identity,
                    &temp_cell_identity,MAX_CELL_IDENTITY_NUMBYTES);
        }
        else
            return -1;
    }
    else if((RRM_TARGET_ID_CDMA2000_1XRTT_CELL_ID_PRESENT & target_id.bitmask) && (RRC_TARGET_ID_UTRAN_GERAN_CGI_PRESENT & target_cgi.bitmask))
    {
        return memcmp_wrapper(&target_cgi.cell_id_1xrtt, &target_id.cell_id_1xrtt, 
                sizeof(cdma2000_1xrtt_cell_identifier_t));
    }
    else if(RRM_TARGET_ID_CDMA2000_HRPD_SECTOR_ID_PRESENT & target_id.bitmask && (RRC_TARGET_ID_UTRAN_GERAN_CGI_PRESENT & target_cgi.bitmask))
    {
        return memcmp_wrapper(&target_cgi.sector_id_hrpd, &target_id.sector_id_hrpd, 
                sizeof(cdma2000_hrpd_sector_identifier_t));
    }
    else
    {
        return -1;
    }

}
/****************************************************************************
 * Function Name  : rrc_fill_inter_rat_cgi_info 
 * Inputs         : p_target_cgi  - cgi of inter rat cells
 *                  p_rrc_rrm_ho_required - pointer to ho_required msg
 * Outputs        : p_target_cgi
 * Returns        : None
 * Description    : This function fills the target cgi from the cgi details 
 *                  present in ho required msg
 ****************************************************************************/
void rrc_fill_inter_rat_cgi_info(
        rrc_inter_rat_global_cgi_t *p_target_cgi,
        rrc_rrm_ho_required_t *p_rrc_rrm_ho_required) 
{

    rrm_target_id_t target_id = p_rrc_rrm_ho_required->target_id;
    U8 *p_temp_cell_identity = p_target_cgi->cgi_info.cell_identity;

    if(RRM_TARGET_ID_RNC_ID_PRESENT & target_id.bitmask)
    {
        l3_memcpy_wrapper(&p_target_cgi->cgi_info.plmn_identity,&target_id.rnc_id.lai.plmn_identity,
                sizeof(plmn_identity_t));

        p_temp_cell_identity[0] = 
            (U8)((p_rrc_rrm_ho_required->source_to_target_container.rncContainer.target_cell_id & 0xFF000000) >> 24);
        p_temp_cell_identity[1] = 
            (U8)((p_rrc_rrm_ho_required->source_to_target_container.rncContainer.target_cell_id & 0x00FF0000) >> 16);
        p_temp_cell_identity[2] = 
            (U8)((p_rrc_rrm_ho_required->source_to_target_container.rncContainer.target_cell_id & 0x0000FF00) >> 8);
        p_temp_cell_identity[3] = 
            (U8)((p_rrc_rrm_ho_required->source_to_target_container.rncContainer.target_cell_id & 0x000000FF));


        p_target_cgi->bitmask |= RRC_TARGET_ID_UTRAN_GERAN_CGI_PRESENT;
        p_target_cgi->cgi_info.rat_type = RRC_RAT_TYPE_UTRAN;
    }
    else if(RRM_TARGET_ID_CGI_PRESENT & target_id.bitmask)
    {
        l3_memcpy_wrapper(&p_target_cgi->cgi_info.plmn_identity,&target_id.cgi.plmn_identity,
                sizeof(plmn_identity_t));
        p_temp_cell_identity[0] = target_id.cgi.location_area_code[1];
        p_temp_cell_identity[1] = target_id.cgi.location_area_code[0];
        p_temp_cell_identity[2] = target_id.cgi.ci_info[1];
        p_temp_cell_identity[3] = target_id.cgi.ci_info[0];
        p_temp_cell_identity[3] &= 0x0F;


        p_target_cgi->bitmask |= RRC_TARGET_ID_UTRAN_GERAN_CGI_PRESENT;
        p_target_cgi->cgi_info.rat_type = RRC_RAT_TYPE_GERAN;
    }
    else if(RRM_TARGET_ID_CDMA2000_1XRTT_CELL_ID_PRESENT & target_id.bitmask)
    {
        l3_memcpy_wrapper(&p_target_cgi->cell_id_1xrtt, &target_id.cell_id_1xrtt, 
                sizeof(cdma2000_1xrtt_cell_identifier_t));

        p_target_cgi->bitmask |= RRC_TARGET_ID_CDMA2000_1XRTT_CELL_ID_PRESENT;
    }
    else if(RRM_TARGET_ID_CDMA2000_HRPD_SECTOR_ID_PRESENT & target_id.bitmask)
    {
        l3_memcpy_wrapper(&p_target_cgi->sector_id_hrpd, &target_id.sector_id_hrpd, 
                sizeof(cdma2000_hrpd_sector_identifier_t));

        p_target_cgi->bitmask |= RRC_TARGET_ID_CDMA2000_HRPD_SECTOR_ID_PRESENT;
    }


}
/*SPR_19166_END*/



/* BUG 12647 Fix Start */
/******************************************************************************
 * Function Name  : uecc_ue_release_req_lte_intra_enb_ho_hndlr
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : updating counters for Inter/Intra Cell Handover
 ******************************************************************************/
static void  uecc_ue_release_req_lte_intra_enb_ho_hndlr(
        uecc_ue_context_t*  p_ue_context)
{
    U8 index = 0;
    S32 cmp_result = -1;
    x2ap_Cause x2apCause;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    memset_wrapper(&x2apCause, 0, sizeof(x2apCause));

    convert_rrm_cause_to_x2ap_cause(
                &p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->cause, &x2apCause);


    if ( HANDOVER_TYPE_INTER_CELL == p_ue_context->ho_info.p_ho_info->
	  p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
    {
        /*Comparing the terget ID onorder to get the index at which the stats were
         * stored in rrc_oam_meas_on_neigh_cell_ho_stats_t at the time of ho attempt */
        for (index = 0; index < (g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count);
                index++)
        {
            cmp_result = memcmp_wrapper(&g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                    /*SPR_19166_START*/
            /*CID 111521 Fix Start*/
                    meas_on_neigh_cell_ho_stats[index].ecgi.plmn_identity,
                    /*SPR_19166_END*/
                    &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                    p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.plmn_identity, 
                    sizeof(plmn_identity_t));

            if (RRC_NULL == cmp_result)
            {
                cmp_result = memcmp_wrapper(&g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].
                        rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                        meas_on_neigh_cell_ho_stats[index].ecgi.cell_identity,
                        &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                        p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.cell_identity, 
                        MAX_CELL_IDENTITY_NUMBYTES);
                /*SPR_19166_END*/
                if (RRC_NULL == cmp_result)
                {
                    break;
                }
            }
            /*CID 111521 Fix Stop*/
        }
    }

    uecc_ue_rel_lte_intra_enb_ho_update_performance_counter(p_ue_context,x2apCause,index,cmp_result);

    if (( RRC_RRM_UE_HO_ADM_RE_HO_FREQ_TYPE_PRESENT &
                p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                p_rrc_rrm_ho_required->bitmask))
    {
        uecc_ue_rel_req_intra_lte_x2_hndlr_amd_re_ho_freq_present(p_ue_context);
    }
    else
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_handover_related_stats.
                intra_freq_ho_stats.ho_IntraFreqOutSucc, RRC_NULL,
                DUMMY_RANGE_CHECK_INDEX);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/****************************************************************************
 * Function Name  : uecc_ue_rel_lte_intra_enb_ho_update_performance_counter
 * Inputs         : uecc_ue_context_t*     -     pointer to UE context
 *                  s1ap_Cause                   s1apCause
 *                  Unsigned character           index
 *                  Signed integer               cmp_result
 * Outputs        : None
 * Returns        : None
 * Description    : handler for some of the update performance counters
 ****************************************************************************/

static void uecc_ue_rel_lte_intra_enb_ho_update_performance_counter(
        uecc_ue_context_t*  p_ue_context,
        x2ap_Cause          x2apCause,
        U8                  index,
        S32                 cmp_result)
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if ( HANDOVER_TYPE_INTER_CELL == p_ue_context->ho_info.p_ho_info->
	  p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type)
    {
        if ((PNULL != (&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
		    rrc_uecc_instance_stats[p_ue_context->
	 	    p_gb_context->offset_uecc_mod_id].
		    rrc_uecc_cell_stats[p_ue_context->cell_index].
		    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
		    meas_on_neigh_cell_ho_stats[index]) && (cmp_result == RRC_NULL)))
        {
	     RRC_UPDATE_PERFORMANCE_COUNTER_X2AP_CAUSE(p_ue_context,
		    g_rrc_stats.p_rrc_stats->
		    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
		    p_gb_context->offset_uecc_mod_id].
		    rrc_uecc_cell_stats[p_ue_context->cell_index].
		    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
		    meas_on_neigh_cell_ho_stats[index].ho_OutSuccTarget_cause.
		    ho_OutSuccTarget_Cause, x2apCause);
	}

	if ((PNULL != (&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
		    rrc_uecc_instance_stats[p_ue_context->
		    p_gb_context->offset_uecc_mod_id].
		    rrc_uecc_cell_stats[p_ue_context->cell_index].
		    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
		    meas_on_neigh_cell_ho_stats[index])) && (cmp_result == RRC_NULL))
	{
	     RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
		    g_rrc_stats.p_rrc_stats->
		    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
		    p_gb_context->offset_uecc_mod_id].
		    rrc_uecc_cell_stats[p_ue_context->cell_index].
		    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
		    meas_on_neigh_cell_ho_stats[index].ho_OutSuccTarget_sum,
		    0, DUMMY_RANGE_CHECK_INDEX);
	}
    }

    if ((RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT &
                p_ue_context->mac_config.bitmask) &&
            (RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT &
            /* Bug 880 Fix Start */
            p_ue_context->mac_config.mac_main_config.bitmask) &&
            (RRC_DRX_CONFIG_TYPE_SETUP == p_ue_context->
            mac_config.mac_main_config.drx_config.drx_config_type))
            /* Bug 880 Fix Stop */
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.drx_non_drx_ho_stats.ho_DrxOutSucc,
                RRC_NULL, DUMMY_RANGE_CHECK_INDEX);
    }
    else
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.drx_non_drx_ho_stats.ho_NoDrxOutSucc,
                RRC_NULL, DUMMY_RANGE_CHECK_INDEX);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_src_lte_intra_enb_ho_hdlr
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Update Performance Counters for Inter/Intra Cell Ho
 *****************************************************************************/

static void uecc_src_lte_intra_enb_ho_hdlr(
        uecc_ue_context_t*              p_ue_context)
{
    U8 index = 0;
    S32 cmp_result = -1;
    x2ap_Cause x2apCause;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    memset_wrapper(&x2apCause, 0, sizeof(x2apCause));

    convert_rrm_cause_to_x2ap_cause(
            &p_ue_context->ho_info.p_ho_info->
            p_src_ho_info->s1_src_ho_info.
            p_rrc_rrm_ho_required->cause, &x2apCause);

    if (( HANDOVER_TYPE_INTER_CELL == p_ue_context->ho_info.p_ho_info->
                p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required->ho_type) &&
            ( g_rrc_stats.p_rrc_stats != PNULL ))
    {
        /* Checking whether ho attempt for the particular target ID
         * already present or not . If yes, then incrementing the count
         * for this new ho request for this same target ID and setting
         * the flag*/
        for (index = 0; index < (g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count);
                index++)
        {
            cmp_result = memcmp_wrapper(&g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                /*SPR_19166_START*/
            /*CID 111519 Fix Start*/
                    meas_on_neigh_cell_ho_stats[index].ecgi.plmn_identity,
                    /*SPR_19166_END*/
                    &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                    p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.plmn_identity, 
                    sizeof(plmn_identity_t));

            if (RRC_NULL == cmp_result)
            {
                cmp_result = memcmp_wrapper(&g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].
                        rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                        meas_on_neigh_cell_ho_stats[index].ecgi.cell_identity,
                        &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                        p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.cell_identity, 
                        MAX_CELL_IDENTITY_NUMBYTES);
                /*SPR_19166_END*/
                if (RRC_NULL == cmp_result)
                {
                    /*SPR 21601 Start*/
                    /*Code Deleted*/
                    /*SPR 21601 Stop*/
                /*SPR 21936 Start*/
                    break;
                /*SPR 21936 Start*/
                }
            }
            /*CID 111519 Fix Stop*/
        }

        if (cmp_result != RRC_NULL)
        {
            /*SPR 21601 start*/
            if(RRC_MAX_CONNECTION > g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count)
            {

                RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,g_rrc_stats.
                        p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].
                        rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count,
                        0, DUMMY_RANGE_CHECK_INDEX);

                index = (U8)(g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].
                        rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count - 1);

                l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].
                        rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                        /*SPR_19166_START*/
                        /* CID 111525 Fix Start */
                        meas_on_neigh_cell_ho_stats[index].ecgi.plmn_identity,
                        &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                        p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.plmn_identity, 
                        sizeof(plmn_identity_t));
                /*SPR_19166_STOP*/
                l3_memcpy_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].
                        rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
                        meas_on_neigh_cell_ho_stats[index].ecgi.cell_identity,
                        &p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
                        p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.cell_identity,
                        MAX_CELL_IDENTITY_NUMBYTES);
                /* CID 111525 Fix Stop */
            }
        }
        if(RRC_MAX_CONNECTION > index)
        {
            uecc_src_lte_intra_enb_ho_update_performance_counter(p_ue_context,x2apCause,index);
        }
        /*SPR 21601 Stop*/    
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_src_lte_intra_enb_ho_update_performance_counter
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    :
 ****************************************************************************/

static void uecc_src_lte_intra_enb_ho_update_performance_counter(
                                uecc_ue_context_t*              p_ue_context,
                                x2ap_Cause                      x2apCause,
                                U8                              index)
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    RRC_UPDATE_PERFORMANCE_COUNTER_X2AP_CAUSE(p_ue_context,g_rrc_stats.
         p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
         p_gb_context->offset_uecc_mod_id].
         rrc_uecc_cell_stats[p_ue_context->cell_index].
         rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
         meas_on_neigh_cell_ho_stats[index].
         ho_OutAttTarget_cause.ho_OutAttTarget_Cause, x2apCause);
    
    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,g_rrc_stats.
         p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
	 p_gb_context->offset_uecc_mod_id].
         rrc_uecc_cell_stats[p_ue_context->cell_index].
         rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.
         meas_on_neigh_cell_ho_stats[index].ho_OutAttTarget_sum,
         0, DUMMY_RANGE_CHECK_INDEX);
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* BUG 12647 Fix Stop */
