/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: uecc_trg_hofsm.c,v 1.4 2010/12/01 13:03:17 gur19827 Exp $
 *
 ******************************************************************************
 *
 *  File Description : The file uecc_trg_hofsm.c contains the definitions of
                       Target Handover FSM procedure, activities, states and
                       FSM failure Handlers. Target handover FSM deals with
                       management of Resource Allocation at Target ENB during
                       Handover.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_trg_hofsm.c,v $
 * Revision 1.5  2011/01/01 13:03:17  gur21231
 * Insert code for Re-establishment
 *
 * Revision 1.4  2010/12/01 13:03:17  gur19827
 * Merged Inter RAT HO
 *
 * Revision 1.3  2010/10/14 12:53:14  gur18569
 * fixed memory leaks
 *
 * Revision 1.1.2.30  2010/10/14 10:22:43  gur25381
 * fixed memory leak
 *
 * Revision 1.1.2.29  2010/10/08 15:42:23  gur21006
 * bitmask corrected
 *
 * Revision 1.1.2.28  2010/10/08 12:36:40  gur21006
 * TDD changes merged
 *
 * Revision 1.1.2.27  2010/10/06 12:11:06  gur22047
 * compiler warnings removed
 *
 * Revision 1.1.2.26  2010/10/06 08:49:36  gur18569
 * fixed ssit bug
 *
 * Revision 1.1.2.25  2010/10/05 12:54:20  gur22047
 * compilation warning removed
 *
 * Revision 1.1.2.24  2010/10/05 12:37:38  gur22047
 * compilation warning removed
 *
 * Revision 1.1.2.23  2010/10/05 10:45:42  gur25381
 * Updated uecc_fsm_ue_trg_ho_procedure_finish_success for Erab Setup
 *
 * Revision 1.1.2.22  2010/10/05 10:21:53  gur18569
 * bug fix
 *
 * Revision 1.1.2.21  2010/10/05 08:05:39  gur15628
 * handling added on rrc_reconf timer expiry
 *
 * Revision 1.1.2.20  2010/10/04 07:04:46  gur18569
 * fixed ssit bugs
 *
 * Revision 1.1.2.19  2010/10/02 11:21:40  gur18569
 * fixed a log
 *
 * Revision 1.1.2.18  2010/10/01 14:08:19  gur18569
 * fixed ssit errors
 *
 * Revision 1.1.2.17  2010/09/28 15:56:37  gur18569
 * fixed ssit bugs
 *
 * Revision 1.1.2.16  2010/09/16 10:39:31  gur18569
 * fixed UT bug
 *
 * Revision 1.1.2.15  2010/09/13 13:04:05  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.1.2.14  2010/09/07 12:32:58  gur18569
 * ut bug fixes and code review comment incorporation for ho failure changes
 *
 * Revision 1.1.2.13  2010/09/07 04:14:37  gur18569
 * added handling to send ho failure to RRM in all failure cases
 *
 * Revision 1.1.2.12  2010/09/06 10:20:02  gur25381
 * Changed the timer expiry from UECC_RRC_MME_GUARD_TIMER to UECC_UE_MME_GUARD_TIMER
 *
 * Revision 1.1.2.11  2010/09/03 11:32:58  gur18569
 * bug fixed while HO UT
 *
 * Revision 1.1.2.10  2010/08/30 11:56:56  gur25381
 * updated log message
 *
 * Revision 1.1.2.9  2010/08/20 14:57:56  gur18569
 * code fix for trg ho
 *
 * Revision 1.1.2.8  2010/08/12 13:45:25  gur18569
 * incoporated review comments for HO
 *
 * Revision 1.1.2.7  2010/08/10 10:03:14  gur18569
 * incorported review comments for ho
 *
 * Revision 1.1.2.6  2010/08/05 08:07:14  gur15697
 * merging round 2
 *
 * Revision 1.1.2.5  2010/07/21 11:55:03  gur18569
 * incorporated code comments
 *
 * Revision 1.1.2.4  2010/07/21 08:34:41  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.1.2.3  2010/07/16 05:19:27  gur22047
 * review comments incorporation
 *
 * Revision 1.1.2.2  2010/07/15 06:26:23  gur22047
 * code review comments incorporate
 *
 * Revision 1.1.2.1  2010/06/23 10:38:41  gur18569
 * Target ho fsm addtion
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "uecc_ue_ctx.h"
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
#include "uecc_trg_hofsm.h"
#include "uecc_defaults.h"
#include "rrc_s1u_intf.h"
#include "uecc_utils.h"
#include "uecc_csc.h"
#include "rrc_rrm_intf.h"
#include "rrc_uecc_error_indication.h"
#include "uecc_mainfsm.h"
#include "rrc_phy_intf.h" 
#include "uecc_llim_utils.h" 
#include "rrc_shared_stats.h" 
#include "uecc_src_hofsm.h"
#include "rrc_logging.h" 

#define CSG_RESET_VAL 0xE0
/******************************************************************************
 Private Definitions
 *****************************************************************************/
#define RRC_UECC_TRG_HO_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,\
        p_uecc_ue_context->ue_index,\
        p_uecc_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
        ##__VA_ARGS__)

#define MIN_NON_GBR_QCI_VALUE 5
#define MAX_NON_GBR_QCI_VALUE 9

#define RRC_IS_GBR_BEARER(qci) ((qci >= 1 && qci <= 4) ? 1 :0)

/* presently,Only Standardized QCI values [TS 23.203,6.1.7.2]
   i.e for 1 to 9 are supported */
#define RRC_IS_VALID_QCI_VALUE(qci) ((qci == 0 || qci > 9) ? 0 : 1)

/******************************************************************************
 Private Types
 *****************************************************************************/
/* x2ap_drop2_rw_start */
 extern void uecc_fsm_ue_rrm_erb_release_procedure_finish_success(
        uecc_ue_context_t*, void*);

extern void uecc_fsm_ue_rrm_erb_release_procedure_finish_fail(
        uecc_ue_context_t*, void*);

extern uecc_fsm_activity_t* uecc_fsm_ue_rrm_erb_release_procedure_activity_list[];

extern uecc_fsm_handler_record_t uecc_fsm_ue_rrm_erb_release_procedure_mht[];
/* x2ap_drop2_rw_end */

extern rrc_return_et uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind
(
    uecc_ue_context_t*  p_uecc_ue_context
    );

extern void x2ap_reset_parse_asn_x2ap_cause(
        x2ap_Cause asn_cause,
        x2ap_cause_t* p_cause);


 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/
static void uecc_fsm_trg_mac_rach_res_get_ongoing_ue_rel_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);
static void uecc_fsm_trg_rrm_adm_ongoing_ue_rel_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);
static rrc_return_t uecc_fsm_trg_ho_process_s1ap_handover_request(
        uecc_ue_context_t   *p_uecc_ue_context,      /* UECC UE context */
        rrc_s1ap_handover_request_t  *p_rrc_s1ap_ho_request/*RRC S1AP HO Req */ 
);

static rrc_return_t uecc_fsm_trg_ho_process_x2ap_handover_request(
        uecc_ue_context_t   *p_uecc_ue_context,      /* UECC UE context */
       rrc_x2ap_handover_request_t  *p_rrc_x2ap_ho_request/*RRC X2AP HO Req */ 
);

static void uecc_fsm_trg_ho_rrm_adm_response_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_csc_cell_del_handler_type1(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_csc_cell_del_handler_type2(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_rrm_adm_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_mac_rach_res_get_ongoing_init_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_mac_rach_res_get_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_create_ue_entity_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ue_rel_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

/* SPR 22500 Fix - Starts */
static void uecc_fsm_trg_ue_re_establish_req_before_recofig_complete_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);
/* SPR 22500 Fix - Ends */

static void uecc_fsm_trg_ue_re_establish_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_error_indication_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_x2ap_error_indication_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

/* SPR 16750 Fix Start */
static void uecc_fsm_trg_rrm_adm_error_indication_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_mac_rach_res_error_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_create_ue_entity_error_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);
/* SPR 16750 Fix Stop */

static void uecc_fsm_trg_ho_create_ue_entity_ongoing_init_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_create_ue_entity_ongoing_llim_create_ue_resp_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static rrc_return_t uecc_fsm_trg_ho_process_rrm_ho_ue_adm_resp_data(
    uecc_ue_context_t*           p_uecc_ue_context,
    rrc_rrm_ue_ho_adm_resp_t* p_rrc_rrm_ho_adm_resp
);

static void uecc_fsm_trg_ho_delete_ue_entity_ongoing_init_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_path_switch_ongoing_init_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_uplink_tunnel_reconfig_init_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_delete_ue_entity_resp_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_delete_ue_entity_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_mme_status_tranfer_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_sn_status_tranfer_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_w_for_mme_status_transfer_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_w_rrc_conn_reconfig_rrc_conn_reconf_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

/*SPR_17893_START*/
/*Code Removed*/
/*SPR_17893_END*/

static void uecc_fsm_trg_ho_s1ap_path_switch_req_ack_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_s1ap_path_switch_req_fail_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_w_for_rrc_s1u_reconfig_cnf_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_w_for_path_switch_req_ack_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_llim_s1u_reconfig_for_switched_uplink_tunnel_cnf_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_w_for_rrc_conn_reconfig_complete_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_reset_handler_type1(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_mac_rach_res_get_ongoing_mac_rach_resp_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_reset_handler_type2(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_reset_handler_type3(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);
/* X2AP RW_2 : Start*/
static void uecc_fsm_trg_ho_reset_handler_type4(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);
/* X2AP RW_2 : Start*/
/*SPR_17348_START*/
static void uecc_trg_ho_rach_res_ongoing_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context
);
static void uecc_trg_ho_llim_ongoing_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context
);
static void uecc_trg_ho_exec_ongoing_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context
);

/*SPR_17348_END*/

static void uecc_fsm_trg_ho_csc_cell_del_handler_type3(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);
/* X2AP HO_3 : Start*/
static void uecc_fsm_trg_ho_csc_cell_del_handler_type4(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);
/* X2AP HO_3 : Start*/

static rrc_return_t uecc_fsm_trg_ho_llim_process_create_ue_entity_response(
    uecc_ue_context_t   *p_uecc_ue_context,
    rrc_uecc_llim_create_ue_entity_resp_t* p_llim_create_ue_entity_resp
);

static rrc_return_t uecc_fsm_trg_ho_is_atleast_one_non_gbr_bearer_present(
    uecc_ue_context_t   *p_uecc_ue_context
);

static rrc_return_t uecc_fsm_trg_ho_is_atleast_one_rlc_am_mode_drb_present(
    uecc_ue_context_t   *p_uecc_ue_context
);


static rrc_return_t uecc_fsm_trg_ho_llim_process_create_ue_entity_response_success(
    uecc_ue_context_t   *p_uecc_ue_context,
    rrc_uecc_llim_create_lc_status_t * p_create_drb_status,
    uecc_e_rab_failed_to_add_list_t *p_uecc_e_rab_failed_to_add_list,
    U32 erab_id
);


static void uecc_fsm_trg_ho_compl_ongoing_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_rach_res_ongoing_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_llim_ongoing_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_exec_ongoing_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);


void uecc_ho_fsm_build_criticality_diagnostics
(
   uecc_ue_context_t*  p_uecc_ue_context,
   s1ap_Criticality    iECriticality,
   s1ap_ProtocolIE_ID  iE_ID,
   s1ap_TypeOfError    typeOfError     
);

static void uecc_fsm_trg_ho_w_mme_status_and_rrc_conn_reconfig_comp_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static rrc_return_et uecc_fsm_fill_erab_to_be_released_list
(
    uecc_ue_context_t*  p_ue_context,
    s1ap_E_RABList*     p_erab_fail_list
);

static
rrc_bool_t uecc_fsm_check_erab_switched_ul_list
(
    uecc_ue_context_t*                  p_ue_context,
    rrc_s1ap_path_switch_request_ack_t* p_path_switch_req_ack
);

static
rrc_return_et uecc_fsm_validate_and_fill_erab_switched_list
(
    uecc_ue_context_t*              p_ue_context,
     s1ap_E_RABToBeSwitchedULList*  p_erab_switched_list
);

static void uecc_fsm_trg_rrm_adm_ongoing_x2ap_ho_cancel_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_mac_rach_res_get_ongoing_x2ap_ho_cancel_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

void uecc_fsm_trg_x2ap_ho_cancel_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static
rrc_return_t uecc_fsm_trg_ho_check_and_process_x2ap_security_capability(
        uecc_ue_context_t*  p_ue_context,
        void*                                   p_api,
        HandoverPreparationInformation_r8_IEs*  p_rrc_container
);

static void uecc_fsm_erb_release_procedure_constructor(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_rrm_adm_ongoing_x2ap_reset_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_mac_rach_res_get_ongoing_x2ap_reset_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_x2ap_reset_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_trg_ho_path_switch_ongoing_uplink_tunnel_reconfig_x2ap_reset_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

/*BUG 604 changes start*/
static void uecc_fsm_trg_s1u_local_path_switch_cnf_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

/*BUG 604 changes stop*/


/* UL NAS START */
static void uecc_fsm_trg_ho_ul_nas_transport_handler(
		uecc_ue_context_t*  p_uecc_ue_context,
		void*               p_api
		);
/* UL NAS STOP */

/*SPR_16416_START*/
static void uecc_fsm_trg_ho_ul_nas_transport_before_psa_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);
/*SPR_16416_END*/
/*SPR 18758 Fix Start*/

static void uecc_fsm_trg_ho_dl_nas_transport_handler(
		uecc_ue_context_t*  p_uecc_ue_context,
		void*               p_api
		);
static void uecc_fsm_trg_ho_nas_non_delievery_handler(
		uecc_ue_context_t*  p_uecc_ue_context,
		void*               p_api
		);
static void uecc_fsm_trg_ho_dl_cdma2000_tunneling_handler(
		uecc_ue_context_t*  p_uecc_ue_context,
		void*               p_api
		);
/*SPR 18758 Fix Stop*/


/*SPR_17143_START*/
/*Code Removed*/
/*SPR_17143_END*/

/*SPR:6462 start*/
static void uecc_fsm_trg_ho_reestablish_ue_entity_complete_cnf_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);
/*SPR:6462 stop*/

/* Bug 8198 Fix Start */
void uecc_fsm_trg_ho_s1ap_allocate_mme_resp_handler(
        uecc_ue_context_t   *p_uecc_ue_context,
        void                *p_api_data
);
/* Bug 8198 Fix End */

/*SPR_17976_START*/
/* Code Removed */
/*SPR_17976_END*/

/*SPR_16416_START*/
void uecc_process_ul_nas_transport_queue
(
    uecc_ue_context_t*      p_uecc_ue_context,
    uecc_event_queue_t*     p_uecc_event_queue  /* Pointer to UECC queue */
);
/*SPR_16416_END*/
/*SPR_20135_START*/
static void uecc_fsm_ue_trg_ho_path_switch_radio_link_failure_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);
static void uecc_fsm_ue_trg_ho_radio_link_failure_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

/*SPR_20135_END*/


#ifdef RRC_DEBUG
/**** FOR TESTING PURPOSE ONLY ***********/
static void trg_ho_debug_print_erab_info(uecc_ue_context_t * p_uecc_ue_context);
#endif




/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/**************************************
 * States definition
 **************************************/

static uecc_fsm_handler_record_t uecc_fsm_trg_ho_rrm_adm_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                  },
        /*BUG 604 changes start*/
        PNULL                               },
        /*BUG 604 changes stop*/
    { { UECC_EV_RRM_HO_ADM_RESP                  },
        uecc_fsm_trg_ho_rrm_adm_response_handler           },
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_trg_ho_reset_handler_type1  },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_trg_ho_csc_cell_del_handler_type1  },
    { { UECC_EV_TIMER_EXPIRY                  },
        uecc_fsm_trg_ho_rrm_adm_ongoing_timer_expiry_handler},
    { { UECC_EV_UE_RELEASE_REQ                  },
        uecc_fsm_trg_rrm_adm_ongoing_ue_rel_req_handler},
    { { UECC_EV_X2AP_HO_CANCEL                  },
        uecc_fsm_trg_rrm_adm_ongoing_x2ap_ho_cancel_handler},
    { { UECC_EV_X2AP_RESET_MSG_IND    },
        uecc_fsm_trg_ho_rrm_adm_ongoing_x2ap_reset_req_handler   },
    /* SPR 16750 Fix Start */
    { { UECC_EV_X2AP_ERROR_INDICATION                  },
        uecc_fsm_trg_rrm_adm_error_indication_handler},
    { { UECC_EV_UE_ERROR_INDICATION                  },
        uecc_fsm_trg_rrm_adm_error_indication_handler},
    /* SPR 16750 Fix Stop */
    { { UECC_LAST_ID                        },  PNULL           }
};

static uecc_fsm_handler_record_t uecc_fsm_trg_ho_mac_rach_res_get_ongoing_mht[]=
{
    { { UECC_EV_STATE_INIT                  },
        uecc_fsm_trg_ho_mac_rach_res_get_ongoing_init_handler           },
    { { UECC_EV_LLIM_RACH_RES_RESP                  },
        uecc_fsm_trg_ho_mac_rach_res_get_ongoing_mac_rach_resp_handler   },
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_trg_ho_reset_handler_type3  },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_trg_ho_csc_cell_del_handler_type3  },
    { { UECC_EV_TIMER_EXPIRY                  },
        uecc_fsm_trg_ho_mac_rach_res_get_ongoing_timer_expiry_handler},
    { { UECC_EV_UE_RELEASE_REQ                  },
        uecc_fsm_trg_mac_rach_res_get_ongoing_ue_rel_req_handler},
    { { UECC_EV_X2AP_HO_CANCEL                  },
        uecc_fsm_trg_mac_rach_res_get_ongoing_x2ap_ho_cancel_handler},
    /*SPR_17348_START*/
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
    /*SPR_17348_END*/
        uecc_fsm_trg_ho_rach_res_ongoing_rrm_ue_connection_release_ind_handler},
    { { UECC_EV_X2AP_RESET_MSG_IND    },
        uecc_fsm_trg_ho_mac_rach_res_get_ongoing_x2ap_reset_req_handler },
    /* SPR 16750 Fix Start */
    { { UECC_EV_X2AP_ERROR_INDICATION                  },
        uecc_fsm_trg_ho_mac_rach_res_error_ind_handler},
    { { UECC_EV_UE_ERROR_INDICATION                  },
        uecc_fsm_trg_ho_mac_rach_res_error_ind_handler},
    /* SPR 16750 Fix Stop */
    { { UECC_LAST_ID                        },  PNULL           }
};

static uecc_fsm_handler_record_t uecc_fsm_trg_ho_create_ue_entity_ongoing_mht[]=
{
    { { UECC_EV_STATE_INIT                  },
        uecc_fsm_trg_ho_create_ue_entity_ongoing_init_handler           },
    { { UECC_EV_LLIM_CREATE_UE_ENTITY_RESP                  },
        uecc_fsm_trg_ho_create_ue_entity_ongoing_llim_create_ue_resp_handler  },
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_trg_ho_reset_handler_type4  },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_trg_ho_csc_cell_del_handler_type4  },
    { { UECC_EV_TIMER_EXPIRY                  },
        uecc_fsm_trg_ho_create_ue_entity_ongoing_timer_expiry_handler},
    { { UECC_EV_UE_RELEASE_REQ                  },
        uecc_fsm_trg_ue_rel_req_handler},
    { { UECC_EV_X2AP_HO_CANCEL                  },
        uecc_fsm_trg_x2ap_ho_cancel_handler},
    /*SPR_17348_START*/
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
    /*SPR_17348_END*/
        uecc_fsm_trg_ho_llim_ongoing_rrm_ue_connection_release_ind_handler},
    { { UECC_EV_X2AP_RESET_MSG_IND     },
        uecc_fsm_trg_ho_x2ap_reset_req_handler },
    /* SPR 16750 Fix Start */
    { { UECC_EV_X2AP_ERROR_INDICATION                  },
        uecc_fsm_trg_ho_create_ue_entity_error_ind_handler},
    { { UECC_EV_UE_ERROR_INDICATION                  },
        uecc_fsm_trg_ho_create_ue_entity_error_ind_handler},
    /* SPR 16750 Fix Stop */
    { { UECC_LAST_ID                        },  PNULL               }
};

static uecc_fsm_handler_record_t uecc_fsm_trg_ho_delete_ue_entity_ongoing_mht[]=
{
    { { UECC_EV_STATE_INIT                  },
        uecc_fsm_trg_ho_delete_ue_entity_ongoing_init_handler           },
    { { UECC_EV_LLIM_DELETE_UE_ENTITY_RESP                  },
        uecc_fsm_trg_ho_delete_ue_entity_resp_handler           },
    { { UECC_EV_TIMER_EXPIRY                  },
        uecc_fsm_trg_ho_delete_ue_entity_ongoing_timer_expiry_handler},
    { { UECC_LAST_ID                        },  PNULL           }
};

static uecc_fsm_handler_record_t uecc_fsm_trg_ho_w_for_mme_status_transfer_mht[]=
{
    { { UECC_EV_STATE_INIT                  },
        PNULL           },
    { { UECC_EV_S1AP_MME_STATUS_TRANSFER                  },
        uecc_fsm_trg_ho_mme_status_tranfer_handler           },
    { { UECC_EV_X2AP_SN_STATUS_TRANSFER                  },
        uecc_fsm_trg_ho_sn_status_tranfer_handler           },
    { { UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE                  },
        uecc_fsm_trg_ho_w_mme_status_and_rrc_conn_reconfig_comp_handler   },
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_trg_ho_reset_handler_type2  },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_trg_ho_csc_cell_del_handler_type2  },
    { { UECC_EV_TIMER_EXPIRY                  },
        uecc_fsm_trg_ho_w_for_mme_status_transfer_timer_expiry_handler},
    { { UECC_EV_UE_RELEASE_REQ                  },
        uecc_fsm_trg_ue_rel_req_handler},
    /* SPR 22500 Fix - Starts */
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_trg_ue_re_establish_req_before_recofig_complete_handler},
    /* SPR 22500 Fix - Ends */
    { { UECC_EV_UE_ERROR_INDICATION                  },
        uecc_fsm_trg_error_indication_handler},
    { { UECC_EV_X2AP_HO_CANCEL                  },
        uecc_fsm_trg_x2ap_ho_cancel_handler},
    /*SPR_17348_START*/
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
    /*SPR_17348_END*/
        uecc_fsm_trg_ho_exec_ongoing_rrm_ue_connection_release_ind_handler},
    { { UECC_EV_X2AP_RESET_MSG_IND     },
        uecc_fsm_trg_ho_x2ap_reset_req_handler },
    { { UECC_EV_X2AP_ERROR_INDICATION                  },
        uecc_fsm_trg_x2ap_error_indication_handler},
    /*SPR_16416_START*/
    { { UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ },
        uecc_fsm_trg_ho_ul_nas_transport_before_psa_handler},
    /*SPR_16416_END*/
    /*SPR_20135_START*/
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND     },
            uecc_fsm_ue_trg_ho_radio_link_failure_ind_handler    },
    /*SPR_20135_END*/
    { { UECC_LAST_ID                        },  PNULL           }
};

static uecc_fsm_handler_record_t 
         uecc_fsm_trg_ho_w_for_rrc_conn_reconfig_complete_mht[]=
{
    { { UECC_EV_STATE_INIT                  },
        PNULL           },
    { { UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE                  },
        uecc_fsm_trg_ho_w_rrc_conn_reconfig_rrc_conn_reconf_handler           },
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_trg_ho_reset_handler_type2  },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_trg_ho_csc_cell_del_handler_type2  },
    { { UECC_EV_TIMER_EXPIRY                  },
        uecc_fsm_trg_ho_w_for_rrc_conn_reconfig_complete_timer_expiry_handler},
    { { UECC_EV_UE_RELEASE_REQ                  },
        uecc_fsm_trg_ue_rel_req_handler},
    /* SPR 22500 Fix - Starts */
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_trg_ue_re_establish_req_before_recofig_complete_handler},
    /* SPR 22500 Fix - Ends */
    { { UECC_EV_UE_ERROR_INDICATION                  },
        uecc_fsm_trg_error_indication_handler},
    { { UECC_EV_X2AP_HO_CANCEL                  },
        uecc_fsm_trg_x2ap_ho_cancel_handler},
    /*SPR_17348_START*/
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
    /*SPR_17348_END*/
        uecc_fsm_trg_ho_exec_ongoing_rrm_ue_connection_release_ind_handler},
    { { UECC_EV_X2AP_RESET_MSG_IND     },
        uecc_fsm_trg_ho_x2ap_reset_req_handler },
/*BUG 604 changes start*/
/*    { { UECC_EV_INTRA_CELL_HO_TIMER_EXPIRY            },
        uecc_fsm_intra_cell_ho_ongoing_timer_expiry_handler},*/
/*BUG 604 changes stop*/
    { { UECC_EV_X2AP_ERROR_INDICATION                  },
        uecc_fsm_trg_x2ap_error_indication_handler},
    /* UL NAS START */
    { { UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ },
        uecc_fsm_trg_ho_ul_nas_transport_handler},
    /* UL NAS STOP */
    /*SPR_17143_START*/
    /*Code Removed*/
    /*SPR_17143_END*/
    /*SPR 18758 Fix Start*/
    { { UECC_EV_S1AP_DL_NAS_TRANSPORT_IND },
        uecc_fsm_trg_ho_dl_nas_transport_handler},
    { { UECC_EV_S1AP_NAS_NON_DELIEVERY_IND },
        uecc_fsm_trg_ho_nas_non_delievery_handler},
    { { UECC_EV_S1AP_DL_CDMA2000_TUNNELING },
        uecc_fsm_trg_ho_dl_cdma2000_tunneling_handler},
    /*SPR 18758 Fix Stop*/
    /*SPR:6462 start*/
    { { UECC_EV_LLIM_REESTAB_UE_ENTITY_COMP_CNF },
        uecc_fsm_trg_ho_reestablish_ue_entity_complete_cnf_handler},
    /*SPR:6462 stop*/
    /*SPR_20135_START*/
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND     },
            uecc_fsm_ue_trg_ho_radio_link_failure_ind_handler    },
    /*SPR_20135_END*/
    { { UECC_LAST_ID                        },  PNULL           }
};

/*SPR_17893_START*/
/*Code Removed*/
/*SPR_17893_END*/

static uecc_fsm_handler_record_t uecc_fsm_trg_ho_path_switch_ongoing_mht[]= 
{
    { { UECC_EV_STATE_INIT                  },
        uecc_fsm_trg_ho_path_switch_ongoing_init_handler           },
/* Bug 8198 Fix Start */
    { { UECC_EV_S1AP_ALLOCATE_MME_RESP   },
        uecc_fsm_trg_ho_s1ap_allocate_mme_resp_handler              },
/* Bug 8198 Fix End */
    { { UECC_EV_S1AP_PATH_SWITCH_REQ_ACK                  },
        uecc_fsm_trg_ho_s1ap_path_switch_req_ack_handler           },
    { { UECC_EV_S1AP_PATH_SWITCH_REQ_FAIL                  },
        uecc_fsm_trg_ho_s1ap_path_switch_req_fail_handler           },
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_trg_ho_reset_handler_type2  },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_trg_ho_csc_cell_del_handler_type2  },
    { { UECC_EV_TIMER_EXPIRY                  },
        uecc_fsm_trg_ho_w_for_path_switch_req_ack_timer_expiry_handler},
    { { UECC_EV_UE_RELEASE_REQ                  },
        uecc_fsm_trg_ue_rel_req_handler},
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_trg_ue_re_establish_req_handler},
    { { UECC_EV_UE_ERROR_INDICATION                  },
        uecc_fsm_trg_error_indication_handler},
    /*SPR_17348_START*/
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
    /*SPR_17348_END*/
        uecc_fsm_trg_ho_compl_ongoing_rrm_ue_connection_release_ind_handler},
    { { UECC_EV_X2AP_RESET_MSG_IND    },
        uecc_fsm_trg_ho_path_switch_ongoing_uplink_tunnel_reconfig_x2ap_reset_req_handler },
    /* SPR 13800 Fix Start */
    { { UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ },
        /*SPR_16416_START*/
        uecc_fsm_trg_ho_ul_nas_transport_before_psa_handler},
        /*SPR_16416_END*/
    /* SPR 13800 Fix Stop */
    /*SPR_16590_START*/
    { { UECC_EV_S1AP_DL_NAS_TRANSPORT_IND       },
            uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler       },
    /*SPR_16590_END*/
/*BUG 604 changes start*/
    { { UECC_EV_LLIM_S1U_LOCAL_PATH_SWITCH_CNF                  },
        uecc_fsm_trg_s1u_local_path_switch_cnf_handler},
/*BUG 604 changes stop*/
    /* SPR 16750 Fix Start */
    { { UECC_EV_X2AP_ERROR_INDICATION                  },
        uecc_fsm_trg_x2ap_error_indication_handler},
    /* SPR 16750 Fix Stop */
    /*SPR_20135_START*/
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND     },
            uecc_fsm_ue_trg_ho_path_switch_radio_link_failure_ind_handler    },
    /*SPR_20135_END*/
    { { UECC_LAST_ID                        },  PNULL           }
};

static uecc_fsm_handler_record_t uecc_fsm_trg_ho_uplink_tunnel_reconfig_ongoing_mht[]= 
{
    { { UECC_EV_STATE_INIT                  },
        uecc_fsm_trg_ho_uplink_tunnel_reconfig_init_handler           },
    { { UECC_EV_LLIM_S1U_RECONFIG_CNF                 },
        uecc_fsm_trg_ho_llim_s1u_reconfig_for_switched_uplink_tunnel_cnf_handler   },
    { { UECC_EV_S1AP_RESET_MSG_IND            },
        uecc_fsm_trg_ho_reset_handler_type2  },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
        uecc_fsm_trg_ho_csc_cell_del_handler_type2  },
    { { UECC_EV_TIMER_EXPIRY                  },
        uecc_fsm_trg_ho_w_for_rrc_s1u_reconfig_cnf_timer_expiry_handler},
    { { UECC_EV_UE_RELEASE_REQ                  },
        uecc_fsm_trg_ue_rel_req_handler},
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_trg_ue_re_establish_req_handler},
    { { UECC_EV_UE_ERROR_INDICATION                  },
        uecc_fsm_trg_error_indication_handler},
    /*SPR_17348_START*/
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
    /*SPR_17348_END*/
        uecc_fsm_trg_ho_compl_ongoing_rrm_ue_connection_release_ind_handler},
    { { UECC_EV_X2AP_RESET_MSG_IND    },
        uecc_fsm_trg_ho_path_switch_ongoing_uplink_tunnel_reconfig_x2ap_reset_req_handler },
    /* SPR 13800 Fix Start */
    { { UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ },
        uecc_fsm_trg_ho_ul_nas_transport_handler},
    /* SPR 13800 Fix Stop */
    /* SPR 16750 Fix Start */
    { { UECC_EV_X2AP_ERROR_INDICATION                  },
        uecc_fsm_trg_x2ap_error_indication_handler},
     /* SPR 16750 Fix Stop */
    /*SPR_16590_START*/
    { { UECC_EV_S1AP_DL_NAS_TRANSPORT_IND       },
            uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler       },
    /*SPR_16590_END*/
    /*SPR 18758 Fix Start*/
    { { UECC_EV_S1AP_NAS_NON_DELIEVERY_IND },
        uecc_fsm_trg_ho_nas_non_delievery_handler},
    { { UECC_EV_S1AP_DL_CDMA2000_TUNNELING },
        uecc_fsm_trg_ho_dl_cdma2000_tunneling_handler},
    /*SPR 18758 Fix Stop*/
    /*SPR_20135_START*/
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND     },
            uecc_fsm_ue_trg_ho_radio_link_failure_ind_handler },
    /*SPR_20135_END*/
    { { UECC_LAST_ID                        },  PNULL           }
};

static uecc_fsm_activity_state_t uecc_fsm_trg_ho_rrm_adm_ongoing =
{
   /* .s_name         =*/   (S8*)"UECC_TRG_HO_RRM_ADM_ONGOING",
   /* .s_id           =*/   (S8*)"TRG_HO_UE",
/*BUG 604 changes start*/
   /* .is_need_init   =*/   RRC_FALSE,
/*BUG 604 changes stop*/
   /* .p_mht          =*/   uecc_fsm_trg_ho_rrm_adm_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_trg_ho_mac_rach_res_get_ongoing =
{
   /* .s_name         =*/   (S8*)"UECC_TRG_HO_MAC_RACH_RES_GET_ONGOING",
   /* .s_id           =*/   (S8*)"TRG_HO_UE",
   /* .is_need_init   =*/   RRC_TRUE,
   /* .p_mht          =*/   uecc_fsm_trg_ho_mac_rach_res_get_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_trg_ho_create_ue_entity_ongoing =
{
   /* .s_name         =*/   (S8*)"UECC_TRG_HO_CREATE_UE_ENTITY_ONGOING",
   /* .s_id           =*/   (S8*)"TRG_HO_UE",
   /* .is_need_init   =*/   RRC_TRUE,
   /* .p_mht          =*/   uecc_fsm_trg_ho_create_ue_entity_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_trg_ho_delete_ue_entity_ongoing =
{
   /* .s_name         =*/   (S8*)"UECC_TRG_HO_DELETE_UE_ENTITY_ONGOING",
   /* .s_id           =*/   (S8*)"TRG_HO_UE",
   /* .is_need_init   =*/   RRC_TRUE,
   /* .p_mht          =*/   uecc_fsm_trg_ho_delete_ue_entity_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_trg_ho_w_for_mme_status_transfer =
{
   /* .s_name         =*/   (S8*)"UECC_TRG_HO_W_FOR_MME_STATUS_TRANSFER_OR_SN_STATUS_TRANSFER",
   /* .s_id           =*/   (S8*)"TRG_HO_UE",
   /* .is_need_init   =*/   RRC_FALSE,
   /* .p_mht          =*/   uecc_fsm_trg_ho_w_for_mme_status_transfer_mht
};

static uecc_fsm_activity_state_t 
     uecc_fsm_trg_ho_w_for_rrc_conn_reconfig_complete =
{
   /* .s_name         =*/   (S8*)"UECC_TRG_HO_W_FOR_RRC_CONN_RECONFIG_COMPLETE",
   /* .s_id           =*/   (S8*)"TRG_HO_UE",
   /* .is_need_init   =*/   RRC_FALSE,
   /* .p_mht          =*/   uecc_fsm_trg_ho_w_for_rrc_conn_reconfig_complete_mht
};

/*SPR_17893_START*/
/*Code Removed*/
/*SPR_17893_END*/

static uecc_fsm_activity_state_t uecc_fsm_trg_ho_path_switch_ongoing =
{
   /* .s_name         =*/   (S8*)"UECC_TRG_HO_PATH_SWITCH_ONGOING",
   /* .s_id           =*/   (S8*)"TRG_HO_PATH_SWITCH",
   /* .is_need_init   =*/   RRC_TRUE,
   /* .p_mht          =*/   uecc_fsm_trg_ho_path_switch_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_trg_ho_uplink_tunnel_reconfig_ongoing =
{
   /* .s_name         =*/   (S8*)"UECC_TRG_HO_UPLINK_TUNNEL_RECONFIG_ONGOING",
   /* .s_id           =*/   (S8*)"TRG_HO_UPLINK_TUNNEL_RECONFIG",
   /* .is_need_init   =*/   RRC_TRUE,
   /* .p_mht          =*/   uecc_fsm_trg_ho_uplink_tunnel_reconfig_ongoing_mht 
};

static uecc_fsm_activity_state_t uecc_fsm_resume_proc_trg_ho_path_switch_ongoing =
{
   /* .s_name         =*/   (S8*)"UECC_RESUME_TRG_HO_PATH_SWITCH_ONGOING",
   /* .s_id           =*/   (S8*)"RESUME_TRG_HO_PATH_SWITCH",
   /* .is_need_init   =*/   RRC_FALSE,
   /* .p_mht          =*/   uecc_fsm_trg_ho_path_switch_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_resume_proc_trg_ho_uplink_tunnel_reconfig_ongoing =
{
   /* .s_name         =*/   (S8*)"UECC_RESUME_TRG_HO_UPLINK_TUNNEL_RECONFIG_ONGOING",
   /* .s_id           =*/   (S8*)"RESUME_TRG_HO_UPLINK_TUNNEL_RECONFIG",
   /* .is_need_init   =*/   RRC_FALSE,
   /* .p_mht          =*/   uecc_fsm_trg_ho_uplink_tunnel_reconfig_ongoing_mht 
};

/**************************************
 * Activity definition
 **************************************/
static uecc_fsm_activity_t uecc_fsm_trg_ho_activity =
{
  /*  .s_name             =*/ (S8*)"Target Ho Activity",
  /*  .s_id               =*/ (S8*)"UE_TRG_HO_A",
  /*  .p_first_state      =*/ &uecc_fsm_trg_ho_rrm_adm_ongoing
};

static uecc_fsm_activity_t* uecc_fsm_ue_trg_ho_procedure_activity_list[] =
{
    &uecc_fsm_trg_ho_activity,
    PNULL
};
/**************************************
 * Activity definition
 **************************************/

/* x2ap_drop2_rw_start */
/**************************************
* Activity definition
**************************************/
/*rw_start*/
/* Procedure */
uecc_fsm_procedure_t uecc_fsm_erb_release_procedure =
{
  /*.s_name           = */(S8*)"ERAB Release",
  /*.s_id             = */(S8*)"UE_ERABREL_P",
  /*.constructor_f    = */uecc_fsm_erb_release_procedure_constructor,
  /*.finish_success_f = */uecc_fsm_ue_rrm_erb_release_procedure_finish_success,
  /*.finish_fail_f    = */uecc_fsm_ue_rrm_erb_release_procedure_finish_fail,
  /*.activity_list    = */uecc_fsm_ue_rrm_erb_release_procedure_activity_list,
  /*.p_mht            = */uecc_fsm_ue_rrm_erb_release_procedure_mht
};
/* x2ap_drop2_rw_end */

/*static uecc_fsm_activity_t* uecc_fsm_reestb_success_ue_trg_ho_procedure_activity_list[] =
{
    &uecc_fsm_reestb_success_trg_ho_activity,
    PNULL
};*/

/**************************************
 * Procedures definition
 **************************************/
static void uecc_fsm_ue_trg_ho_procedure_constructor(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_reestb_success_ue_trg_ho_procedure_constructor(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_ue_trg_ho_procedure_finish_success(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);

static void uecc_fsm_ue_trg_ho_procedure_finish_fail(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
);


static uecc_fsm_handler_record_t uecc_fsm_ue_trg_ho_procedure_mht[] =
{
    { { UECC_LAST_ID                      }, PNULL    }
};

/* eNB Target HO Procedure */
uecc_fsm_procedure_t uecc_fsm_ue_trg_ho_procedure =
{
  /*  .s_name           =*/ (S8*)"UE Target eNB HO procedure",
  /*  .s_id             =*/ (S8*)"UE_TRG_HO_P",
  /*  .constructor_f    =*/
      uecc_fsm_ue_trg_ho_procedure_constructor,
  /*  .finish_success_f =*/
      uecc_fsm_ue_trg_ho_procedure_finish_success,
  /*  .finish_fail_f    =*/
      uecc_fsm_ue_trg_ho_procedure_finish_fail,
  /*  .activity_list    =*/
      uecc_fsm_ue_trg_ho_procedure_activity_list,
  /*  .p_mht            =*/ 
      uecc_fsm_ue_trg_ho_procedure_mht
};

/* eNB Re-estbalishment sucess Target HO Ongoing Procedure */
uecc_fsm_procedure_t uecc_fsm_reest_success_ue_trg_ho_procedure =
{
  /*  .s_name           =*/ (S8*)"UE Target eNB HO procedure",
  /*  .s_id             =*/ (S8*)"UE_TRG_HO_P",
  /*  .constructor_f    =*/
      uecc_fsm_reestb_success_ue_trg_ho_procedure_constructor,
  /*  .finish_success_f =*/
      PNULL,
  /*  .finish_fail_f    =*/
      PNULL,
  /*  .activity_list    =*/
      PNULL,
  /*  .p_mht            =*/ PNULL
};

/* TRG HO failure actions */
static void uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
);
static void uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf_failure(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
);
/*This function is not used any where */
static void uecc_fsm_ue_trg_ho_fail_action_send_ho_failure(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
);

static void uecc_fsm_ue_trg_ho_fail_action_last(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
);

static void uecc_fsm_ue_trg_ho_fail_action_last_wait_for_s1ap_ue_rel_cmd(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
);


static void uecc_fsm_ue_trg_ho_fail_action_send_rach_res_rel_ind(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
);

void uecc_fsm_ue_trg_ho_fail_action_set_state_delete_ue(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
);

void uecc_fsm_ue_trg_ho_fail_action_send_enb_ue_ctx_release_req(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
);

/* TRG HO failure actions lists */
static const uecc_fsm_fail_handler_f uecc_fsm_ue_trg_ho_fail_type1_actions[] =
{
    uecc_fsm_ue_trg_ho_fail_action_send_ho_failure,
    uecc_fsm_ue_trg_ho_fail_action_last,
    PNULL
};
static const uecc_fsm_fail_handler_f uecc_fsm_ue_trg_ho_fail_type2_actions[] =
{
    uecc_fsm_ue_trg_ho_fail_action_send_ho_failure,
    uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf,
    uecc_fsm_ue_trg_ho_fail_action_last,
    PNULL
};
static const uecc_fsm_fail_handler_f uecc_fsm_ue_trg_ho_fail_type3_actions[] =
{
    uecc_fsm_ue_trg_ho_fail_action_send_rach_res_rel_ind,
    uecc_fsm_ue_trg_ho_fail_action_send_ho_failure,
    uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf,
    uecc_fsm_ue_trg_ho_fail_action_last,
    PNULL
};
static const uecc_fsm_fail_handler_f uecc_fsm_ue_trg_ho_fail_type5_actions[] =
{
    uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf,
    uecc_fsm_ue_trg_ho_fail_action_last_wait_for_s1ap_ue_rel_cmd,
    PNULL
};
static const uecc_fsm_fail_handler_f uecc_fsm_ue_trg_ho_fail_type6_actions[] =
{
    uecc_fsm_ue_trg_ho_fail_action_send_ho_failure,
    uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf,
    uecc_fsm_ue_trg_ho_fail_action_set_state_delete_ue,
    PNULL
};
static const uecc_fsm_fail_handler_f uecc_fsm_ue_trg_ho_fail_type7_actions[] =
{
    uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf,
    uecc_fsm_ue_trg_ho_fail_action_last,
    PNULL
};
static const uecc_fsm_fail_handler_f uecc_fsm_ue_trg_ho_fail_type8_actions[] =
{
    uecc_fsm_ue_trg_ho_fail_action_send_rach_res_rel_ind,
    uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf_failure,
    uecc_fsm_ue_trg_ho_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_ue_trg_ho_fail_type10_actions[] =
{
    uecc_fsm_ue_trg_ho_fail_action_send_ho_failure,
    uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf_failure,
    uecc_fsm_ue_trg_ho_fail_action_last,
    PNULL
};
static const uecc_fsm_fail_handler_f uecc_fsm_ue_trg_ho_fail_type11_actions[] =
{
    uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf_failure,
    uecc_fsm_ue_trg_ho_fail_action_last,
    PNULL
};
static const uecc_fsm_fail_handler_f uecc_fsm_ue_trg_ho_fail_type14_actions[] =
{
    uecc_fsm_ue_trg_ho_fail_action_send_enb_ue_ctx_release_req,
    uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf_failure,
    uecc_fsm_ue_trg_ho_fail_action_last,
    PNULL
};
/*SPR_17591_START*/
static const uecc_fsm_fail_handler_f uecc_fsm_ue_trg_ho_fail_type15_actions[] =
{
    uecc_fsm_ue_trg_ho_fail_action_send_rach_res_rel_ind,
    uecc_fsm_ue_trg_ho_fail_action_send_enb_ue_ctx_release_req,
    uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf_failure,
    uecc_fsm_ue_trg_ho_fail_action_last,
    PNULL
};
/*SPR_17591_END*/
static const uecc_fsm_fail_handler_f uecc_fsm_ue_trg_ho_fail_type12_actions[] =
{
    uecc_fsm_ue_trg_ho_fail_action_last,
    PNULL
};

/* x2ap_drop2_rw_start */
/****************************************************************************
* Function Name  : uecc_fsm_erb_release_procedure_constructor 
* Inputs         : uecc_ue_context_t* - pointer to UE context
*                  void*   - api data
* Outputs        : None
* Returns        : None
* Description    : Constructor for Erab release during Target Handover procedure
****************************************************************************/
void uecc_fsm_erb_release_procedure_constructor(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    U8 erab_index = 0;
    /*U8 max_erab_count = 0;*/
    U8 failed_erab_count = 0;
    erab_list_t* p_erab_list = PNULL;
    
    rrc_return_t result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    /*RRC_ASSERT(PNULL != p_api);*/

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /*SPR_17382_START*/
    /* Code Removed */
    /*SPR_17382_END*/
    
    /*Allocate memory for the curr_proc_data */
    p_uecc_ue_context->p_curr_proc_data =
                      (curr_proc_data_t*)rrc_mem_get(sizeof(curr_proc_data_t));
         
    if (PNULL == p_uecc_ue_context->p_curr_proc_data)
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name,
                RRC_BRIEF, "[CURR_PROC_DATA] rrc_mem_get failed");
    }
    else
    {
        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */
        do
        {
            /* Start timer*/
            if (RRC_SUCCESS !=
                        uecc_ue_timer_start(
                            p_uecc_ue_context->p_gb_context, 
                            UECC_ERAB_RELEASE_IND_GUARD_TIMER,
                            &p_uecc_ue_context->p_gb_context->ue_timers_data,
                                &p_uecc_ue_context->timers_data,p_uecc_ue_context->ue_index))
            {
                break;
            }

            /*Update the Erabs to be released info */
            p_erab_list = &p_uecc_ue_context->p_curr_proc_data->u.
                erab_release_command_data.erab_release_list;

            /*max_erab_count = p_uecc_ue_context->e_rab_list_count;*/
        
            for(erab_index=0; erab_index<MAX_ERAB_COUNT; erab_index++)
            {
                if(PNULL != p_uecc_ue_context->p_e_rab_list[erab_index])
                {
                    /*SPR_17382_START*/
                    if((RRC_ERAB_FAILURE_PATH_SWITCH_REQ_ACK == p_uecc_ue_context->
                        p_e_rab_list[erab_index]->uecc_x2ap_drb_ctx.status) ||
                        (RRC_ERAB_FAILURE_S1U_RECONFIGURE_CNF == p_uecc_ue_context->
                         p_e_rab_list[erab_index]->uecc_x2ap_drb_ctx.status))
                    /*SPR_17382_END*/
                    {
                        /*Fill Erab Id*/
                        p_erab_list->erab_item[failed_erab_count].e_RAB_ID = 
                                p_uecc_ue_context->p_e_rab_list[erab_index]->
                                uecc_x2ap_drb_ctx.e_RAB_ID;
                        
                        /*Fill Cause*/
                        if(RRC_SUCCESS !=
                                uecc_s1ap_build_s1ap_cause(&p_erab_list->
                                    erab_item[failed_erab_count].cause,
                                        &p_uecc_ue_context->
                                                p_e_rab_list[erab_index]->
                                                    uecc_x2ap_drb_ctx.cause,
                                                    p_uecc_ue_context))
                        {
                            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                                    p_uecc_ue_context->p_gb_context->facility_name,RRC_WARNING,
                                    "[RRC_ERB_RELEASE]"
                                    "invalid cause value [%u][%u]",
                                    p_uecc_ue_context->p_e_rab_list[erab_index]->
                                    uecc_x2ap_drb_ctx.cause.type,
                                    p_uecc_ue_context->p_e_rab_list[erab_index]->
                                    uecc_x2ap_drb_ctx.cause.value);
                        }

                        failed_erab_count++;
                        p_erab_list->counter++;
                    }
                }
            }

            result = RRC_SUCCESS;
        }while(0);
    
    if (RRC_SUCCESS != result)
    {
        /*SPR_17382_START*/
        p_uecc_ue_context->ho_info.is_x2_ho_erab_rel_reqd = RRC_FALSE;
        /* stop timer if it has been started */
        if (PNULL !=
                p_uecc_ue_context->timers_data.
                    timers_ids[UECC_ERAB_RELEASE_IND_GUARD_TIMER])
        {
            /* Stop timer */
            uecc_ue_timer_stop(p_uecc_ue_context->p_gb_context, 
                                UECC_ERAB_RELEASE_IND_GUARD_TIMER,
                                &p_uecc_ue_context->timers_data);
        }

        /* Process failure*/
        uecc_fsm_finish_activity_failure(p_uecc_ue_context,
                (void*)"UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT");
        /*SPR_17382_END*/
    }
    
  }
  
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/* x2ap_drop2_rw_end */
/****************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_procedure_constructor
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Constructor for Target Handover procedure
 ****************************************************************************/
void uecc_fsm_ue_trg_ho_procedure_constructor(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    ho_ongoing_ue_identity_search_node_t *p_node = PNULL;
    U16          crnti = RRC_NULL;
    rrc_return_t result = RRC_FAILURE;


    /*SPR_16680_START*/
    /* Code Removed */
    /*SPR_16680_END*/


    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    
        /*BUG 604 changes start*/
    //if (INTRA_CELL_HO != p_uecc_ue_context->ho_info.s1_or_x2_handover)
    //{
        /*BUG 604 changes stop*/
        RRC_ASSERT(PNULL != p_api);

#ifdef RRC_UECC_TR_ID_CONST
        uecc_ue_ctx_set_op_transction_id(p_uecc_ue_context, 1); 
#else
        uecc_ue_ctx_set_op_transction_id(p_uecc_ue_context,
                uecc_ue_ctx_get_next_transction_id(p_uecc_ue_context));
#endif /*RRC_UECC_TR_ID_CONST*/

        /* Setting rrm rel not required here , this will be unset
           in case of successful ho proc*/
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "[%s] Setting rrc_connection_release_not_required_flag"
                " Setting rrm_rel_req_not_required_flag",
                __FUNCTION__);

        p_uecc_ue_context->share_data.uecc_rcr_data.
            rrm_rel_req_not_required_flag = RRC_TRUE;

        /* Start Procedural timer */
        if (RRC_FAILURE == uecc_ue_timer_start(
                    p_uecc_ue_context->p_gb_context,
                    UECC_UE_TRG_HO_GUARD_TIMER,
                    &p_uecc_ue_context->p_gb_context->ue_timers_data,
                    &p_uecc_ue_context->timers_data,
                    p_uecc_ue_context->ue_index))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "uecc_ue_timer_start failed !!!");

            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_uecc_ue_context,
                    uecc_fsm_ue_trg_ho_fail_type1_actions);
        }

        /* Allocate Memory for HO specific information in UECC UE context */
        p_uecc_ue_context->ho_info.p_ho_info = (uecc_ho_info_t*)
            rrc_mem_get(sizeof(uecc_ho_info_t));

        if (PNULL == p_uecc_ue_context->ho_info.p_ho_info)
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                    "[%s]Memory allocation failed !!",__FUNCTION__);
            return;
        }
        else
        {
            memset_wrapper( p_uecc_ue_context->ho_info.p_ho_info,
                    0,sizeof(uecc_ho_info_t));
        }
        p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info = 
            (uecc_trg_ho_info_t*)
            rrc_mem_get(sizeof(uecc_trg_ho_info_t));

        if (PNULL == 
                p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info)
        {
            /* Memory allocated will be freed in main fsm in case
             * of failure*/
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                    "[%s]Memory allocation failed !!",__FUNCTION__);
            return;
        }

        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */

        /* Init S1AP ASN context */
        if (RT_OK != rtInitContext(&p_uecc_ue_context->s1ap_asn1_ctx))
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                     p_uecc_ue_context->p_gb_context->facility_name,
                     RRC_ERROR,
                    "ASN context initialization failed" );
        }
        /*BUG 604 changes start*/
        if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) ||
           (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
           (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
        /*BUG 604 changes stop*/
        {
            /* Check whether received HO Request message is valid */
            if (RRC_SUCCESS == uecc_fsm_trg_ho_process_x2ap_handover_request(
                        p_uecc_ue_context,
                        (rrc_x2ap_handover_request_t*)p_api))
            {
                /* Allocate a node and store info about Ue (phyCellId+C-RNTI) at 
                   target  eNB side */
                p_node = (ho_ongoing_ue_identity_search_node_t*)rrc_mem_get
                    (sizeof(ho_ongoing_ue_identity_search_node_t));

                if (PNULL == p_node)
                {
                    /* Memory allocated will be freed in main fsm in case
                     * of failure*/
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                            "[%s]Memory allocation failed !!",__FUNCTION__);
                    return;
                }

 
                if(PNULL == p_node)
                {
                    RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
                    return;
                }

                /* SES-508 Fix Start */
                /* Code Removed */
                /* SES-508 Fix End */

                /* 16 MSb's store as phyCellId */       
                if(p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                        rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                        m.as_ContextPresent)
                {
                    if(p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                            rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                            as_Context.m.reestablishmentInfoPresent)
                    {

                        p_node->ue_identity = p_uecc_ue_context->ho_info.p_ho_info->
                            p_trg_ho_info->rrc_x2ap_ho_request.
                            rrc_x2ap_ue_ctx_info.rRC_Container.
                            as_Context.reestablishmentInfo.
                            sourcePhysCellId;
                        p_node->ue_identity = (p_node->ue_identity)<<16;
                    }
                    else
                    {
                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                                "ReestablishmentInfo is not present in RRC Context");
                        result = RRC_FAILURE;
                    }
                }
                else
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                            "AS_Context is not present in RRC Context");
                    result = RRC_FAILURE;
                }

                if(p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                        rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                        m.as_ConfigPresent)
                {
                    crnti = p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                        rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                        as_Config.sourceUE_Identity.data[0];
                    crnti = (U16)(crnti <<8);
                    crnti =(U16)(crnti | p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                            rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                            as_Config.sourceUE_Identity.data[1]);
                }
                else
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                            "AS_Config is not present in RRC Context");
                    result = RRC_FAILURE;
                }
                if((p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                            rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                            m.as_ContextPresent)
                        && (p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                            rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                            as_Context.m.reestablishmentInfoPresent)
                        && (p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                            rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                            m.as_ConfigPresent))
                {
                    /* 16 LSb's store as C-RNTI */        
                    p_node->ue_identity |= crnti;
                    p_node->ue_index = p_uecc_ue_context->ue_index;

                    /*SPR_16680_START*/
                    st_insert_duplicate_node (
                            &(p_uecc_ue_context->p_gb_context->
                                ho_ongoing_ue_identity_search_tree),
                            &(p_node->nodeAnchor));
                    /*SPR_16680_END*/
#ifdef RRC_DEBUG
                    /* Crash observed so commented */
                    /*    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,"[%s]",__FUNCTION__);
                          trg_ho_debug_print_erab_info(p_uecc_ue_context);*/
#endif
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,
                            "Sending RRM UE HO ADM REQUEST");
                    /* Send RRC_RRM_UE_HO_ADM_REQ */
                    result = uecc_rrm_build_and_send_x2ap_ue_ho_adm_req(
                            p_uecc_ue_context,
                            (rrc_x2ap_handover_request_t*)p_api);
                }
                else
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                            "Failed to insert node in ho_ongoing_ue_identity_search_tree ");
                    result = RRC_FAILURE;
                }
            }
        }
        else
        {
            /* Check whether received HO Request message is valid */
            if (RRC_SUCCESS == uecc_fsm_trg_ho_process_s1ap_handover_request(
                        p_uecc_ue_context,
                        (rrc_s1ap_handover_request_t*)p_api))
            {
                /* Allocate a node and store info about Ue (phyCellId+C-RNTI) at 
                   souce eNB side */
                p_node = (ho_ongoing_ue_identity_search_node_t*)rrc_mem_get
                    (sizeof(ho_ongoing_ue_identity_search_node_t));

                if (PNULL == p_node)
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                            "[%s]Memory allocation failed !!",__FUNCTION__);
                    return;
                }

                /* SES-508 Fix Start */
                /* Code Removed */
                /* SES-508 Fix End */

                /* 16 MSb's store as phyCellId */        
                p_node->ue_identity = p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                    rrc_s1ap_ho_request.source_to_target_transparent_container.
                    source_enb_to_target_enb_transparent_container.rRC_Container.
                    as_Context.reestablishmentInfo.sourcePhysCellId;

                p_node->ue_identity = (p_node->ue_identity)<<16;

                crnti = p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                    rrc_s1ap_ho_request.source_to_target_transparent_container.
                    source_enb_to_target_enb_transparent_container.rRC_Container.
                    as_Config.sourceUE_Identity.data[0];

                crnti = (U16)(crnti <<8);
                crnti =(U16)(crnti | p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                        rrc_s1ap_ho_request.source_to_target_transparent_container.
                        source_enb_to_target_enb_transparent_container.rRC_Container.
                        as_Config.sourceUE_Identity.data[1]);

                /* 16 LSb's store as C-RNTI */        
                p_node->ue_identity |= crnti;
                p_node->ue_index = p_uecc_ue_context->ue_index;
                /*SPR_16680_START*/
                st_insert_duplicate_node (
                        &(p_uecc_ue_context->p_gb_context->
                            ho_ongoing_ue_identity_search_tree),
                        &(p_node->nodeAnchor));
                /*SPR_16680_END*/

#ifdef RRC_DEBUG
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,"[%s]",__FUNCTION__);
                trg_ho_debug_print_erab_info(p_uecc_ue_context);
#endif

                RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,
                        "Sending RRM UE HO ADM REQUEST");
                /* Send RRC_RRM_UE_HO_ADM_REQ */
                result = uecc_rrm_build_and_send_ue_ho_adm_req(
                        p_uecc_ue_context,
                        (rrc_s1ap_handover_request_t*)p_api);
            }
        }
        if (result == RRC_FAILURE)
        {
            if (X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "Processing of X2AP HO REQ failed");
            }
            else if(INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) 
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "Processing of INTER CELL HO REQ failed");
            }
        /*BUG 604 changes start*/
            else if(INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) 
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "Processing of INTRA CELL HO REQ failed");
            }
        /*BUG 604 changes stop*/
            else
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "Processing of S1AP HO REQ failed");
            }
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_uecc_ue_context,
                    uecc_fsm_ue_trg_ho_fail_type1_actions);
        }

        /*BUG 604 changes start*/
    //}
        /*BUG 604 changes stop*/
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_reestb_success_ue_trg_ho_procedure_constructor
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Constructor for UE Connection Release procedure
 ****************************************************************************/
void uecc_fsm_reestb_success_ue_trg_ho_procedure_constructor(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    uecc_fsm_activity_state_t* p_prev_current_state = PNULL;
    /*SPR_17976_START*/
    uecc_event_t*       p_event = PNULL;
    /*SPR_17976_END*/

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
#ifdef RRC_UECC_TR_ID_CONST
    uecc_ue_ctx_set_op_transction_id(p_uecc_ue_context, 1); 
#else
    uecc_ue_ctx_set_op_transction_id(p_uecc_ue_context,
            uecc_ue_ctx_get_next_transction_id(p_uecc_ue_context));
#endif /*RRC_UECC_TR_ID_CONST*/

    /* Setting rrm rel not required here , this will be unset
       in case of successful ho proc*/
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "[%s] Setting rrc_connection_release_not_required_flag"
            " Setting rrm_rel_req_not_required_flag",
            __FUNCTION__);

    p_uecc_ue_context->share_data.uecc_rcr_data.
        rrm_rel_req_not_required_flag = RRC_TRUE;

    p_prev_current_state = 
        p_uecc_ue_context->prev_ongoing_uecc_fsm.proc_descriptor.p_current_state;

    /* Copy Proc Descriptor information */
    l3_memcpy_wrapper(&p_uecc_ue_context->uecc_fsm.proc_descriptor,
              &p_uecc_ue_context->prev_ongoing_uecc_fsm.proc_descriptor,
      sizeof(uecc_fsm_procedure_descriptor_t));

    /*SPR_17976_START*/
 
    /* Start Procedural timer */
    if (RRC_FAILURE == uecc_ue_timer_start(
            p_uecc_ue_context->p_gb_context, 
            UECC_UE_TRG_HO_GUARD_TIMER,
            &p_uecc_ue_context->p_gb_context->ue_timers_data,
            &p_uecc_ue_context->timers_data,
            p_uecc_ue_context->ue_index))
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                "uecc_ue_timer_start failed !!!");

        /* Process failure according to selected failure actions list */
        /* SPR 16062 Start */
        if (S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) 
        {
            uecc_fsm_start_failure_processing(
                    p_uecc_ue_context,
                    uecc_fsm_ue_trg_ho_fail_type5_actions);
        }
        else
        {
            uecc_fsm_start_failure_processing(
                    p_uecc_ue_context,
                    uecc_fsm_ue_trg_ho_fail_type7_actions);
        }
        /* SPR 16062 Stop */
        return;
    }

    if (p_prev_current_state == 
          &uecc_fsm_trg_ho_w_for_rrc_conn_reconfig_complete)
    {
        uecc_fsm_trg_ho_w_rrc_conn_reconfig_rrc_conn_reconf_handler(
                p_uecc_ue_context, 
                p_api); 
    }
    else if (p_prev_current_state == 
          &uecc_fsm_trg_ho_w_for_mme_status_transfer )
    {
        uecc_fsm_trg_ho_w_mme_status_and_rrc_conn_reconfig_comp_handler(
                p_uecc_ue_context, 
                p_api); 
    }
    else if(p_prev_current_state == &uecc_fsm_trg_ho_path_switch_ongoing)
    {
        p_uecc_ue_context->uecc_fsm.proc_descriptor.p_current_state =
                 &uecc_fsm_resume_proc_trg_ho_path_switch_ongoing; 
    }
    else if (p_prev_current_state == &uecc_fsm_trg_ho_uplink_tunnel_reconfig_ongoing)
    {
        p_uecc_ue_context->uecc_fsm.proc_descriptor.p_current_state =
                 &uecc_fsm_resume_proc_trg_ho_uplink_tunnel_reconfig_ongoing; 
    }

    if(TENB_CRE_Q_GET_COUNT(&p_uecc_ue_context->p_tenb_cre_event_queue))
    {
        /* Max one event is stored in p_tenb_cre_event_queue.
         * So, poping out only the one event and not running any loop.
         */
        if(PNULL != 
                (p_event = 
                uecc_event_queue_pop_event(
                    &p_uecc_ue_context->p_tenb_cre_event_queue)))
        {
            if(PNULL != p_event->p_msg_info)
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "uecc_fsm_reestb_success_ue_trg_ho_procedure_constructor: "
                    "De-queued Event:%u for UE:%u", 
                    p_event->event_id, p_uecc_ue_context->ue_index);
                switch(p_event->event_id)
                {
                    case ASN1V_s1ap_id_MMEStatusTransfer:
                    case ASN1V_s1ap_id_PathSwitchRequest:
                        uecc_s1ap_process_ue_associated_sig_msg_ind(
                                p_event->p_msg_info, p_uecc_ue_context->p_gb_context);
                        break;
                    case ASN1V_x2ap_id_snStatusTransfer:
                        uecc_x2ap_process_ue_associated_sig_msg_ind(
                                p_event->p_msg_info, p_uecc_ue_context->p_gb_context);
                        break;
                    case UECC_EV_S1AP_ALLOCATE_MME_RESP:
                        uecc_s1ap_process_alloc_mme_resp(
                                p_event->p_msg_info, p_uecc_ue_context->p_gb_context);
                        break;
                    case UECC_EV_LLIM_S1U_RECONFIG_CNF:
                        uecc_llim_process_s1u_reconfigure_cnf(
                                p_event->p_msg_info, p_uecc_ue_context->p_gb_context);
                        break;
                    default:
                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                                "Unexpected event:%u is received. "
                                "Ignoring it.",
                                p_event->event_id);
                        break;
                }

                rrc_mem_free(p_event->p_msg_info);
                p_event->p_msg_info = PNULL;
            }
            /* Free event */
            rrc_mem_free(p_event);
            p_event = PNULL;
        }
    }
    /*SPR_17976_END*/
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_procedure_finish_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Success handler for UE Target HO procedure
 *                  Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_trg_ho_procedure_finish_success(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    /* RRC Crash Fix Start */
    /* SPR 17213 Fix Start */
    /* code deleted */
    /* SPR 17213 Fix End */
    /* RRC Crash Fix End */
    /* SPR 11462 Fix Start */
    U8 qci_value = 0;
    U8 counter = 0;
    /* SPR 11462 Fix End */

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);


    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Proc Success handler");
    /* SPR 19221: CID 108600 Fix Start */
    /*BUG 604 changes start*/
    /*BUG 604 changes stop*/
    /* Stop timer */
    uecc_ue_timer_stop(p_uecc_ue_context->p_gb_context,
            UECC_UE_TRG_HO_GUARD_TIMER, 
            &p_uecc_ue_context->timers_data);
    /* Bug 799 Fix Start */
    rrc_delete_ho_ue_identity_node (p_uecc_ue_context);
    /* Bug 799 Fix Stop */
    /* SPR 19221: CID 108600 Fix Stop */
    /*SPR_17976_START*/
    /* Delete nodes stored in TeNB re-establishment queue */
    uecc_delete_tenb_cre_queue(p_uecc_ue_context,
            &(p_uecc_ue_context->p_tenb_cre_event_queue));
    /*SPR_17976_END*/

        /* SPR 16062 Start */
        /* Release all TRG HO related memory */
    /* SPR Fix 19074 Start */
    /* During Target HO, if ue trg_ho_context is not relased after store_ue_context_timer expiry
     * then context will be cleaned here */
    if (!( uecc_check_store_ue_context_timer(p_uecc_ue_context)))
    {
        uecc_fsm_free_trg_ho_context(p_uecc_ue_context);
        p_uecc_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
    }
    /* SPR Fix 19074 End */
        /* SPR 16062 Stop */
    /* SPR 19221: CID 108600 Fix Start */
    /* Unsetting the flag set in proc constructor */
    p_uecc_ue_context->share_data.uecc_rcr_data.
        rrm_rel_req_not_required_flag = RRC_FALSE;

    p_uecc_ue_context->m.physical_config_dedicated_updated = 0;
    p_uecc_ue_context->m.mac_config_updated = 0;
    p_uecc_ue_context->m.srb1_config_updated = 0;
    p_uecc_ue_context->m.srb2_config_updated = 0;
    p_uecc_ue_context->m.meas_config_updated = 0;

    /* set SRB2 create flag in UE context as one */
    p_uecc_ue_context->m.srb2_created = 1;
        
    /* CSR 00070192 Fix Start */
    p_uecc_ue_context->m.srb1_created = 1;
    /* CSR 00070192 Fix Stop */
    uecc_status_ue_update_num_of_srb1(p_uecc_ue_context);

    uecc_status_ue_update_num_of_srb2(p_uecc_ue_context);
    uecc_status_ue_update_num_of_drb(p_uecc_ue_context,
            (S8)p_uecc_ue_context->e_rab_list_count);
    /* SPS Start */
    /* SPR 20747 Fix Start */  
    if ((p_uecc_ue_context->m.is_sps_recv_frm_rrm) && (p_uecc_ue_context->m.sps_config_updated))
    {
            if (p_uecc_ue_context->m.is_sps_setup_or_rel_recv)
            {
                if (p_uecc_ue_context->p_sps_config != PNULL)
                {
                    rrc_mem_free(p_uecc_ue_context->p_sps_config);
                    p_uecc_ue_context->m.sps_config_present = 0;
                    p_uecc_ue_context->p_sps_config = PNULL;
                }
            }
        /* Send to MAC - RRC_MAC_RECONFIG_COMPLETE_IND */
        uecc_llim_build_and_send_mac_reconfig_complete_ind(
                p_uecc_ue_context->p_gb_context,
                p_uecc_ue_context,
                RRC_SUCCESS);
    }
    p_uecc_ue_context->m.sps_config_updated = 0;
    p_uecc_ue_context->m.is_sps_recv_frm_rrm = 0;
   p_uecc_ue_context->m.is_sps_setup_or_rel_recv = 0;
    /* SPR 20747 Fix End */  

    /* SPS Stop */
    if (p_uecc_ue_context->p_meas_config !=  PNULL)
    {
        rrc_mem_free(p_uecc_ue_context->p_meas_config);
        p_uecc_ue_context->p_meas_config = PNULL;
    }
    /* SPR 16750 Fix Start */
    p_uecc_ue_context->x2ap_context.m.error_indication_ongoing = 0;
    /* SPR 16750 Fix Stop */

    if(p_uecc_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present == 1)
    {
        /*Release new X2AP_UEID */
        uecc_x2ap_id_free(p_uecc_ue_context->p_gb_context,
                p_uecc_ue_context->x2ap_context.new_enb_ue_x2ap_id);
    }
    /*BUG 604 changes start*/
    //}
    /*BUG 604 changes stop*/
    /* SPR 11462 Fix Start */
    if ( PNULL != g_rrc_stats.p_rrc_stats)
    {
       for (counter = 0; counter < MAX_ERAB_COUNT; counter++)
       { 
           if (PNULL != p_uecc_ue_context->p_e_rab_list[counter])
           {     
                qci_value = p_uecc_ue_context->p_e_rab_list[counter]->
                    e_RABlevelQoSParameters.qCI;
                
                RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].rrc_erab_related_stats.
                        erab_sim_num_stats.erabSimNumber_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
                        RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

                RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].rrc_erab_related_stats.
                        erab_sim_num_stats.erabSimNumber_sum,
                        RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

           }
       }
   }/* SPR 11462 Fix End */

#ifdef MEM_PROFILE_ON
  RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,"Memory usage at the end of the target HO procedure");
  qvPoolStats();
#endif

   /* Bug 817 Fix Start */
   /* Start timer to delete forwarding tunnels if S1U_END_MARKER_IND
      is not received. Intra-Cell-HO guard timer is used to guard this procedure.
      Timer will be renamed later on */
   if (p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
          drb_tunnel_info_count > RRC_NULL)
   {
       /*check if prep timer is running for intra_cell based handover*/
       if ( PNULL == p_uecc_ue_context->timers_data.timers_ids[UECC_UE_INTRA_CELL_HO_GUARD_TIMER] )
       {
           /* Start timer*/
           if ( RRC_SUCCESS != uecc_ue_timer_start(
                       p_uecc_ue_context->p_gb_context,
                       UECC_UE_INTRA_CELL_HO_GUARD_TIMER,
                       &p_uecc_ue_context->p_gb_context->ue_timers_data,
                       &p_uecc_ue_context->timers_data,
                       p_uecc_ue_context->ue_index))
           {
               /* Delete all the forwarding tunnels */
               if ( RRC_SUCCESS != uecc_llim_build_and_send_delete_trg_forwarding_tunnel_req
                       (p_uecc_ue_context))
               {
                   RRC_UECC_TRG_HO_FSM_TRACE (RRC_ERROR,
                           "Either no forwarding tunnel exist or"
                           " delivery of S1U_RECONFIGURE_REQ gets failed.");
               }
           }
       }
   }
   /*SPR_19387_START*/
    if (p_uecc_ue_context->m.is_sps_recv_frm_rrm)
    {
        if (p_uecc_ue_context->m.is_sps_setup_or_rel_recv)
        {
            if (p_uecc_ue_context->p_sps_config != PNULL)
            {
                rrc_mem_free(p_uecc_ue_context->p_sps_config);
                p_uecc_ue_context->m.sps_config_present = 0;
                p_uecc_ue_context->p_sps_config = PNULL;
            }
        }

        /* Send to MAC - RRC_MAC_RECONFIG_COMPLETE_IND */
        uecc_llim_build_and_send_mac_reconfig_complete_ind(
                p_uecc_ue_context->p_gb_context,
                p_uecc_ue_context,
                RRC_SUCCESS);
    }

    p_uecc_ue_context->m.sps_config_updated = 0;
    p_uecc_ue_context->m.is_sps_recv_frm_rrm = 0;
    p_uecc_ue_context->m.is_sps_setup_or_rel_recv = 0;

   /*SPR_19387_END*/

   /* Bug 817 Fix Stop */
    /* SPR 19221: CID 108600 Fix Stop */
   p_uecc_ue_context->x2ap_reset_recvd_flag = RRC_FALSE;
    /*SPR_16618_START*/
    p_uecc_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present = RRC_FALSE;
    /*SPR_16618_END*/

    /*Bug_12800_fix_start*/
    if (PNULL != p_uecc_ue_context->p_scell_config)
    {
        if (RRC_NULL == 
            p_uecc_ue_context->p_scell_config->scell_add_mod_list.count)
        {
            rrc_mem_free(p_uecc_ue_context->p_scell_config);
            p_uecc_ue_context->p_scell_config = PNULL;
        }
    }
    /*Bug_12800_fix_stop*/

      /* Send event to UE Main FSM */
      uecc_event_queue_push_event(
              UECC_EV_TRG_HO_SUCCESS_INT,
              p_uecc_ue_context,
              p_api);
  RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_rrm_adm_response_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRM_HO_ADM_RESP message.
 ****************************************************************************/
void uecc_fsm_trg_ho_rrm_adm_response_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    rrc_return_t                    result = RRC_FAILURE;
    rrc_rrm_ue_ho_adm_resp_t *p_rrc_rrm_ue_ho_adm_resp=
        (rrc_rrm_ue_ho_adm_resp_t *)p_api;
    uecc_fsm_fail_handler_f const*         p_failure_actions_list =
        uecc_fsm_ue_trg_ho_fail_type1_actions;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_BRIEF,
            "Received RRM UE HO ADM RESPONSE");
    /* Bug_10942 Fix Start */
    /* Coverity Fix 97026 Start */
    if (RRM_RESPONSE_CAUSE_PRESENT & p_rrc_rrm_ue_ho_adm_resp->response.bitmask)
    {
      result = fill_s1ap_cause(&p_uecc_ue_context->ho_info.cause ,&p_rrc_rrm_ue_ho_adm_resp->response.cause,p_uecc_ue_context);
      if (RRC_FAILURE == result)
      {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED, "fill_s1ap_cause returned failure");
      }
    }

    /* Coverity Fix 97026 End */
    /* Bug_10942 Fix Stop */
    do {
/* SPR Fix 5194 Start */
        if (p_rrc_rrm_ue_ho_adm_resp->response.response != RRC_FAILURE)
/* SPR Fix 5194 End */
        {
            if ((RRC_RRM_UE_HO_ADM_RESP_RADIO_REOURCE_CONFIG_PRESENT &
                    p_rrc_rrm_ue_ho_adm_resp->bitmask )&&
                    (RRC_RRM_UE_HO_ADM_RESP_MOBILITY_CONTROL_INFO_PRESENT &
                    p_rrc_rrm_ue_ho_adm_resp->bitmask )&&
                    (RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT &
                    p_rrc_rrm_ue_ho_adm_resp->radio_resource_config.
                    mac_config.bitmask ))
            {
                if (RRC_SUCCESS != 
                        uecc_fsm_trg_ho_process_rrm_ho_ue_adm_resp_data(
                            p_uecc_ue_context,p_rrc_rrm_ue_ho_adm_resp))
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                            "[%s]Processing or RRM HO UE ADM RSP failed!!",
                            __FUNCTION__);
                    p_failure_actions_list = 
                        uecc_fsm_ue_trg_ho_fail_type10_actions;
                    break;
                }
                else
                {

#ifdef RRC_DEBUG
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,"[%s]",__FUNCTION__);
                    trg_ho_debug_print_erab_info(p_uecc_ue_context);
#endif

                    /* Allocate Memory for HO Resp */
                    p_uecc_ue_context->ho_info.p_ho_info->
                        p_trg_ho_info->p_rrc_rrm_ho_adm_resp = 
                        (rrc_rrm_ue_ho_adm_resp_t*) 
                        rrc_mem_get(sizeof(rrc_rrm_ue_ho_adm_resp_t));

                    if (PNULL == p_uecc_ue_context->ho_info.p_ho_info->
                            p_trg_ho_info->p_rrc_rrm_ho_adm_resp)
                    {
                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                                "[%s]Memory allocation failed !!",__FUNCTION__);
                        break;
                    }
                    else
                    {
                        /* SES-508 Fix Start */
                        /* Code Removed */
                        /* SES-508 Fix End */

                        l3_memcpy_wrapper(p_uecc_ue_context->ho_info.p_ho_info->
                                p_trg_ho_info->p_rrc_rrm_ho_adm_resp,
                                p_rrc_rrm_ue_ho_adm_resp,
                                sizeof(rrc_rrm_ue_ho_adm_resp_t)); 
                    }
                    
                    
                    if (p_rrc_rrm_ue_ho_adm_resp->bitmask &
                            RRC_RRM_UE_HO_ADM_RESP_CONFIG_SCELL_PRESENT)
                    {
                        p_uecc_ue_context->p_rrm_scell_config = 
                            (rrm_scell_config_t*)rrc_mem_get(sizeof(rrm_scell_config_t));

                        if (PNULL == p_uecc_ue_context->p_rrm_scell_config)
                        {

                            RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                                    "[%s]Memory allocation failed !!",__FUNCTION__);
                            break;
                        }
                        else
                        {
                            /* SES-508 Fix Start */
                            /* Code Removed */
                            /* SES-508 Fix End */

                            result = uecc_rrm_process_scell_config (p_uecc_ue_context, 
                                    &p_rrc_rrm_ue_ho_adm_resp->scell_config);
                            if (RRC_FAILURE == result)
                            {
                                RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                                        "[%s] [RRC_RRM_UE_HO_ADM_RESP]Invalid SCell "
                                        "Configuration Received",__FUNCTION__);

                                break;
                            }

                            l3_memcpy_wrapper(p_uecc_ue_context->p_rrm_scell_config, 
                                    &p_rrc_rrm_ue_ho_adm_resp->scell_config,
                                    sizeof(rrm_scell_config_t));
                            
                            rrc_rrm_update_ue_ctx_scell_config(&p_rrc_rrm_ue_ho_adm_resp->
                                    scell_config, p_uecc_ue_context);                            
                        }
                    }

                    UECC_FSM_SET_CHILD_STATE(
                            p_uecc_ue_context,
                            &uecc_fsm_trg_ho_mac_rach_res_get_ongoing, 
                            PNULL);
                    result = RRC_SUCCESS;
                }
            }
            else
            {
                /* radio_resource_config should be present in case of
                   success*/
                p_failure_actions_list = 
                    uecc_fsm_ue_trg_ho_fail_type10_actions;
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s]Incorrect Admission Resp received: "
                        "RRM HO UE ADM RESP",__FUNCTION__);
                break;
            }
        }
        else
        {

            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s]RRM UE HO ADM RESP failure returned from RRM!!"
                    ,__FUNCTION__);
            
	    /* SPR 13448 Fix Start */
	    if (S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
	    {
		if (RRM_RESPONSE_CAUSE_PRESENT & p_rrc_rrm_ue_ho_adm_resp->response.bitmask)
		{
		    /* Coverity Fix 83171 Start */
		    if ( RRC_SUCCESS != convert_rrm_cause_to_s1ap_cause (&p_rrc_rrm_ue_ho_adm_resp->response.cause,
				&p_uecc_ue_context->ho_info.cause)) 
		    {
			RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING, "[%s] Invalid Cause received from RRM!!",
				__FUNCTION__);
			break;
		    }	
		    /* Coverity Fix 83171 Stop */
		}
		else
		{
		    p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
		    p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
		}
	    }
            else
            {
		if (RRM_RESPONSE_CAUSE_PRESENT & p_rrc_rrm_ue_ho_adm_resp->response.bitmask)
		{ 
		    p_uecc_ue_context->ho_info.x2ap_cause.t = 
			p_rrc_rrm_ue_ho_adm_resp->response.cause.type;
		    switch(p_rrc_rrm_ue_ho_adm_resp->response.cause.type)
		    {
			case T_x2ap_Cause_radioNetwork:
			    /*SPR 16609 FIX START*/
                            p_uecc_ue_context->ho_info.x2ap_cause.u.radioNetwork =
                                           p_rrc_rrm_ue_ho_adm_resp->response.cause.value;
                            /*SPR 16609 FIX END*/
			    break;
			case T_x2ap_Cause_transport:
			    p_uecc_ue_context->ho_info.x2ap_cause.u.transport =
				p_rrc_rrm_ue_ho_adm_resp->response.cause.value;
			    break;
                        case T_x2ap_Cause_protocol:
			    p_uecc_ue_context->ho_info.x2ap_cause.u.protocol =
				p_rrc_rrm_ue_ho_adm_resp->response.cause.value;
			    break;
			case T_x2ap_Cause_misc:
			    p_uecc_ue_context->ho_info.x2ap_cause.u.misc =
				p_rrc_rrm_ue_ho_adm_resp->response.cause.value;
                            break;
                        default:
                            p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
			    p_uecc_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
			    break;
		    }
		}
		else
		{
                    p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
                    p_uecc_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2;
                }
            }
            /* SPR 13448 Fix Stop */
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s]RRM UE HO ADM RESP failure returned from RRM!!"
                    ,__FUNCTION__);
            break;
        }
    }while(0);

    /* SPR 20889 Fix Start */
    if ((RRC_FAILURE == result) ||
           (p_rrc_rrm_ue_ho_adm_resp->response.response == RRC_FAILURE)) 
    /* SPR 20889 Fix Stop */
    {
        uecc_fsm_start_failure_processing(
                p_uecc_ue_context,
                p_failure_actions_list);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_x2_ho_process_store_erab_config
 * Inputs         : p_gb_context - UECC global context
                    p_rrc_x2ap_ho_request - Pointer to X2AP HO Request
 * Outputs        : None
 * Returns        : None
 * Description    : This validates the X2AP HO Request and stores the
                    necessary information in UECC UE context.
 ****************************************************************************/
static void uecc_fsm_trg_x2_ho_process_store_erab_config(
        uecc_ue_context_t             *p_uecc_ue_context,
        rrc_x2ap_handover_request_t   *p_rrc_x2ap_ho_request,
        uecc_e_rab_failed_to_add_list_t *p_uecc_e_rab_failed_to_add_list)
{
    OSRTDListNode* p_node = PNULL;
    /*SPR_18125_START*/
    /* Code Removed */
    /*SPR_18125_END*/
    U16 e_rab_list_count = RRC_NULL;
/*SPR 13844 Fix Start*/
    U16 e_rab_net_count = RRC_NULL;
/*SPR 13844 Fix Stop*/
    U16 e_rab_index = RRC_NULL;
    U32 e_rab_id = RRC_NULL;
    x2ap_E_RABs_ToBeSetup_List_element *p_erab_elem = PNULL;
    x2ap_E_RABs_ToBeSetup_Item *p_e_rab_list[ASN1V_x2ap_maxnoofBearers] = {PNULL};
    U32 add_once_duplicate_e_rab_list[INVALID_ERAB_ID];
    /* SPR_18778 start */
    U8 size;
    /* SPR_18778 stop */
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    memset_wrapper(&add_once_duplicate_e_rab_list,0,
            sizeof(add_once_duplicate_e_rab_list));
    for (p_node = p_rrc_x2ap_ho_request->rrc_x2ap_ue_ctx_info.
            ue_context_info.e_RABs_ToBeSetup_List.head;
            (e_rab_list_count<(p_rrc_x2ap_ho_request->
                               rrc_x2ap_ue_ctx_info.ue_context_info.
                               e_RABs_ToBeSetup_List.count));
            e_rab_list_count++)
    {
        RRC_ASSERT(PNULL != p_node); 
        RRC_ASSERT(PNULL != p_node->data); 

        p_erab_elem=(x2ap_E_RABs_ToBeSetup_List_element*)p_node->data;
        if ( p_erab_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1 == PNULL)
        {
            continue;
        }
        p_e_rab_list[e_rab_list_count] = p_erab_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1;

        p_node=p_node->next;
    }
/*SPR 13844 Fix Start*/
    e_rab_net_count = e_rab_list_count;
/*SPR 13844 Fix Stop*/
    for (e_rab_index=0;e_rab_index < e_rab_list_count;e_rab_index++)
    {
        e_rab_id = p_e_rab_list[e_rab_index]->e_RAB_ID;

        /* [TS 36.423-960, 8.2.1.4 Abnormal Conditions]
           If the eNB receives a HANDOVER REQUEST message containing 
           a E-RAB Level QoS Parameters IE which contains a QCI IE 
           indicating a GBR bearer (as defined in [13]), and which does 
           not contain the GBR QoS Information IE, the eNB shall not 
           admit the corresponding E-RAB*/
        /* Also presently,Only Standardized QCI values [TS 23.203,6.1.7.2]
           i.e for 1 to 9 are supported */

        if ((!RRC_IS_VALID_QCI_VALUE(p_e_rab_list[e_rab_index]->
                        e_RAB_Level_QoS_Parameters.qCI)) || 
                (RRC_IS_GBR_BEARER(p_e_rab_list[e_rab_index]->
                                   e_RAB_Level_QoS_Parameters.qCI) 
                 && !p_e_rab_list[e_rab_index]->
                 e_RAB_Level_QoS_Parameters.m.gbrQosInformationPresent))
        {
            /* Add to GBR failed to add list */
            /*Erab id */
            p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list
                [p_uecc_e_rab_failed_to_add_list->count].e_rab_id 
                = e_rab_id;
            /*cause */
            p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list
                [p_uecc_e_rab_failed_to_add_list->count].x2ap_cause.t 
                = T_x2ap_Cause_radioNetwork;
            p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list
                [p_uecc_e_rab_failed_to_add_list->count].x2ap_cause.
                u.radioNetwork = x2ap_unspecified_2; 

            p_uecc_e_rab_failed_to_add_list->count++;

            continue;
        }
        /*[TS 36.423-960, 8.2.1.4 Abnormal Conditions]
          If the eNB receives a HANDOVER REQUEST message containing 
          several E-RAB ID IEs (in the E-RABs To Be Setup List IE)
          set to the same value, the eNB shall not admit the 
          corresponding E-RABs.*/

        if (p_uecc_ue_context->p_e_rab_list[e_rab_id])
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s]Duplicated ERAB ID IEs found %d !!",
                    __FUNCTION__,e_rab_id);
            /* Adding Duplicated ERABs only Once to failed to
               add list */
            if (!add_once_duplicate_e_rab_list[e_rab_id])
            {
                /*Erab id */
                p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list
                    [p_uecc_e_rab_failed_to_add_list->count].e_rab_id 
                    = e_rab_id;
                /*cause */
/*SPR 13844 Fix Start*/
                p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list
                    [p_uecc_e_rab_failed_to_add_list->count].x2ap_cause.t 
                    = T_x2ap_Cause_radioNetwork;
                p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list
                    [p_uecc_e_rab_failed_to_add_list->count].x2ap_cause.u.
                    radioNetwork = x2ap_multiple_E_RAB_ID_instances;

/*SPR 13844 Fix Stop*/
                p_uecc_e_rab_failed_to_add_list->count++;

                add_once_duplicate_e_rab_list[e_rab_id] = 1;

            }
/*SPR 13844 Fix Start*/
		e_rab_net_count--;
	    if (e_rab_list_count == e_rab_index + 1 )
	    {
	    	if (e_rab_net_count == p_uecc_e_rab_failed_to_add_list->count)
	    	{
 			p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
			p_uecc_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_multiple_E_RAB_ID_instances;		
    	    	}
	    } 
/*SPR 13844 Fix Stop*/
            continue;
        }
        else
        {
            p_uecc_ue_context->p_e_rab_list[e_rab_id] =
                (uecc_drb_ctx_t*)rrc_mem_get(sizeof(uecc_drb_ctx_t));

            if (PNULL == p_uecc_ue_context->p_e_rab_list[e_rab_id])
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                        "[%s]Memory allocation failed !!",__FUNCTION__);
                break;
            }
            p_uecc_ue_context->e_rab_list_count++;
        }

        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */

        /* filling e_RAB_ID */
        p_uecc_ue_context->p_e_rab_list[e_rab_id]->e_RAB_ID = e_rab_id;
        p_uecc_ue_context->p_e_rab_list[e_rab_id]->uecc_x2ap_drb_ctx.
            e_RAB_ID = e_rab_id;

        /* filling e_RABlevelQosParameters */
        l3_memcpy_wrapper((void*)(&p_uecc_ue_context->p_e_rab_list[e_rab_id]->
                    e_RABlevelQoSParameters),
                (const void *)(&p_e_rab_list[e_rab_index]->
                    e_RAB_Level_QoS_Parameters),
                sizeof(x2ap_E_RAB_Level_QoS_Parameters));
        asn1Copy_x2ap_E_RAB_Level_QoS_Parameters(&p_uecc_ue_context->
                s1ap_asn1_ctx,
                &p_e_rab_list[e_rab_index]->e_RAB_Level_QoS_Parameters,
                &p_uecc_ue_context->p_e_rab_list[e_rab_id]->
                uecc_x2ap_drb_ctx.e_RABlevelQoSParameters);
       /* SPR_18778 start */
       /* filling transportLayerAddress */
        p_uecc_ue_context->p_e_rab_list[e_rab_id]->transportLayerAddress.numbits
            = p_e_rab_list[e_rab_index]->uL_GTPtunnelEndpoint.
            transportLayerAddress.numbits;
        if((p_e_rab_list[e_rab_index]->uL_GTPtunnelEndpoint.
                    transportLayerAddress.numbits)%8)
        {
            size = (p_e_rab_list[e_rab_index]->uL_GTPtunnelEndpoint.
                    transportLayerAddress.numbits/8)+1;
        }
        else
        {
            size = p_e_rab_list[e_rab_index]->uL_GTPtunnelEndpoint.
                transportLayerAddress.numbits/8;
        }

        p_uecc_ue_context->p_e_rab_list[e_rab_id]->transportLayerAddress.data
            = (U8 *)rrc_mem_get(size);

        if (PNULL == p_uecc_ue_context->p_e_rab_list[e_rab_id]->
                transportLayerAddress.data)
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                    "[%s]Memory allocation failed !!",__FUNCTION__);
            break;
        }
        l3_memcpy_wrapper((void*)(p_uecc_ue_context->p_e_rab_list[e_rab_id]->
                    transportLayerAddress.data),
                (const void *)(p_e_rab_list[e_rab_index]->
                    uL_GTPtunnelEndpoint.transportLayerAddress.data),
                size);
       /* SPR_18778 stop */
        asn1Copy_x2ap_TransportLayerAddress(&p_uecc_ue_context->
                s1ap_asn1_ctx,
                &p_e_rab_list[e_rab_index]->uL_GTPtunnelEndpoint.
                transportLayerAddress,
                &p_uecc_ue_context->p_e_rab_list[e_rab_id]->
                uecc_x2ap_drb_ctx.transportLayerAddress);

        /* filling gTP_TEID */
        p_uecc_ue_context->p_e_rab_list[e_rab_id]->gTP_TEID.numocts
            = p_e_rab_list[e_rab_index]->uL_GTPtunnelEndpoint.gTP_TEID.numocts;
        l3_memcpy_wrapper((void *)(p_uecc_ue_context->p_e_rab_list[e_rab_id]->gTP_TEID.data),
                (const void *)(p_e_rab_list[e_rab_index]->uL_GTPtunnelEndpoint.gTP_TEID.data),
                p_e_rab_list[e_rab_index]->uL_GTPtunnelEndpoint.gTP_TEID.numocts);

        p_uecc_ue_context->p_e_rab_list[e_rab_id]->uecc_x2ap_drb_ctx.
            gTP_TEID = p_e_rab_list[e_rab_index]->
            uL_GTPtunnelEndpoint.gTP_TEID;

    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_x2_ho_process_store_radio_cap_info
 * Inputs         : p_gb_context - UECC global context
                    p_rrc_x2ap_ho_request - Pointer to X2AP HO Request
 * Outputs        : None
 * Returns        : RRC_SUCCESS - If X2AP HO Request is valid.
                    RRC_FAILURE - If X2AP HO Request is not valid.
 * Description    : This validates the X2AP HO Request and stores the
                    necessary information in UECC UE context.
 ****************************************************************************/
static rrc_return_t uecc_fsm_trg_x2_ho_process_store_radio_cap_info(
        uecc_ue_context_t             *p_uecc_ue_context,
        rrc_radio_capability_info_t   *p_radio_capability_info,
        rrc_x2ap_handover_request_t   *p_rrc_x2ap_ho_request,
        HandoverPreparationInformation_r8_IEs * p_rrc_container)
{
    U8 count;
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    /* SPR 11997 Fix Start */
    if (DUPLEXING_MODE_TDD != p_uecc_ue_context->p_gb_context->
                p_p_csc_context[p_uecc_ue_context->cell_index]->p_csc_init_setup_ind->duplexing_mode)
    {
    for(count = 0; count< p_radio_capability_info->eutra_radio_capability_info.
            eutra_radio_capability_info_def.rf_parameters.supported_band_list_eutra.count; count++)
    {
        if(p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                p_csc_init_setup_ind->sib_type_1_Info.freq_band_indicator == 
                p_radio_capability_info->eutra_radio_capability_info.
                eutra_radio_capability_info_def.rf_parameters.supported_band_list_eutra.
                supported_band_eutra[count].band_eutra)
        {
            if(p_radio_capability_info->eutra_radio_capability_info.
                    eutra_radio_capability_info_def.rf_parameters.supported_band_list_eutra.
                    supported_band_eutra[count].half_duplex == 1)
            {
                p_uecc_ue_context->ue_mode = 1;
                break;
            }
        }
     }

    }
    /* SPR 11997 Fix Stop */

    /*HD-FDD stop */
    /* 36321_CR0538 start */
    p_uecc_ue_context->access_stratum_release = p_radio_capability_info->eutra_radio_capability_info.
        eutra_radio_capability_info_def.access_stratum_release;
    /* 36321_CR0538 stop */

    x2ap_reset_parse_asn_x2ap_cause(p_rrc_x2ap_ho_request->cause,
            &(p_uecc_ue_context->ho_report_cause));

    /* Fill Source Cell C-RNTI */
    if (p_rrc_container->m.as_ConfigPresent == 1)
    {
        p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
            rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
            m.as_ConfigPresent = 1;

        p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
            rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
            as_Config = p_rrc_container->as_Config;

        /* meas_config change start */
        /* fill meas_config in curr_meas_config */
        p_uecc_ue_context->curr_meas_config.is_meas_config_valid = 
            RRM_RRC_MC_VALIDATE_FAILURE;

        if (RRC_FAILURE == uecc_build_rrm_curr_meas_config(
                    p_uecc_ue_context,
                    &p_uecc_ue_context->curr_meas_config,
                    &p_rrc_container->as_Config.sourceMeasConfig))
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[%s] uecc_rrm_build_rrm_curr_meas_config "
                    "returned failure!",__FUNCTION__);
            return RRC_FAILURE;
        }
        else
        {
            p_uecc_ue_context->curr_meas_config.is_meas_config_valid =
                RRM_RRC_MC_VALIDATE_SUCCESS;
        }

        if (p_uecc_ue_context->p_meas_config == PNULL)
        {
            p_uecc_ue_context->p_meas_config = (rrm_meas_config_t*) 
                rrc_mem_get (sizeof (rrm_meas_config_t));

            if (PNULL == p_uecc_ue_context->p_meas_config)
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                        "[%s]Memory allocation failed !!",__FUNCTION__);
                return RRC_FAILURE;
            }

            /* SES-508 Fix Start */
            /* Code Removed */
            /* SES-508 Fix End */
        }

        if ((p_rrc_container->as_Config.sourceMeasConfig.m.
                    measObjectToRemoveListPresent == 1) ||
                (p_rrc_container->as_Config.sourceMeasConfig.m.
                 reportConfigToRemoveListPresent == 1) || 
                (p_rrc_container->as_Config.sourceMeasConfig.m.
                 measIdToRemoveListPresent == 1))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "[%s] Remove Meas-config IE are present",
                    __FUNCTION__);

            if (RRC_FAILURE == uecc_build_rrm_meas_config_for_remove_list(
                        p_uecc_ue_context,
                        p_uecc_ue_context->p_meas_config,
                        &p_rrc_container->as_Config.sourceMeasConfig))
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,RRC_WARNING,
                        "[%s]uecc_build_rrm_meas_config_for_remove_list "
                        "returned failure!",__FUNCTION__);
                return RRC_FAILURE;
            }
        }
        if (p_uecc_ue_context->curr_meas_config.is_meas_config_valid ==
                RRM_RRC_MC_VALIDATE_SUCCESS)
        {
            if (RRC_SUCCESS != uecc_rrc_update_CurrMeasConfigList(
                        p_uecc_ue_context->p_meas_config,
                        &p_uecc_ue_context->curr_meas_config,
                        p_uecc_ue_context->p_gb_context))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s] Failure to update CurrMeasConfigList.!!",
                        __FUNCTION__);
            }
        }

        /* meas_config change end */
    }
    if (p_rrc_container->m.as_ContextPresent == 1)
    {
        p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
            rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
            m.as_ContextPresent = 1;

        if (p_rrc_container->as_Context.m.reestablishmentInfoPresent == 1)
        {
            p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                as_Context.m.reestablishmentInfoPresent = 1;

            /* Fill Souce phyCellId */
            p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                as_Context.reestablishmentInfo.sourcePhysCellId  
                = p_rrc_container->as_Context.reestablishmentInfo.
                sourcePhysCellId;
            /* Fill short MAC-I */
            p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                as_Context.reestablishmentInfo.targetCellShortMAC_I  
                = p_rrc_container->as_Context.reestablishmentInfo.
                targetCellShortMAC_I;
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_process_x2ap_handover_request
 * Inputs         : uecc_ue_context_t   *p_uecc_ue_context
                    rrc_x2ap_handover_request_t  *p_rrc_x2ap_ho_request
 * Outputs        : None
 * Returns        : RRC_SUCCESS - If X2AP HO Request is valid.
                    RRC_FAILURE - If X2AP HO Request is not valid.
 * Description    : This validates the X2AP HO Request and stores the
                    necessary information in UECC UE context.
 ****************************************************************************/
rrc_return_t uecc_fsm_trg_ho_process_x2ap_handover_request(
        uecc_ue_context_t   *p_uecc_ue_context,      
        rrc_x2ap_handover_request_t  *p_rrc_x2ap_ho_request 
)
{
    HandoverPreparationInformation_r8_IEs * p_rrc_container = PNULL;
    rrc_return_t    result = RRC_FAILURE;
    U16 e_rab_index = RRC_NULL;
    uecc_e_rab_failed_to_add_list_t *p_uecc_e_rab_failed_to_add_list = PNULL;
    /* HD-FDD start */
    OSRTDListNode*      p_node1 = PNULL;
    /* SPR 13502 Fix Start */
    rrc_radio_capability_info_t  *p_radio_capability_info = PNULL;
    U8 counter;
    /* HD-FDD stop */

    RRC_ASSERT(PNULL != p_uecc_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    p_radio_capability_info = (rrc_radio_capability_info_t *)rrc_mem_get
        (sizeof(rrc_radio_capability_info_t));
    if (PNULL == p_radio_capability_info)
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
            "[%s]Memory allocation failed !!",__FUNCTION__);
        return RRC_FAILURE;
    }
    /* SES-508 Fix Start */
    /* Code Removed */
    /* SES-508 Fix End */

    if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(p_uecc_ue_context->p_gb_context,
                p_uecc_ue_context->cell_index))
    {
        /* SPR 14790 Fix Start */
        /* Set HO failure cause */
        p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_uecc_ue_context->ho_info.x2ap_cause.u.protocol = x2ap_cell_not_available; 
        /* SPR 14790 Fix Stop */

        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
            p_uecc_ue_context->p_gb_context->facility_name,
            RRC_WARNING,"cell not active");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
        /*SPR_17484_START*/
        p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_uecc_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_cell_not_available; 
        /*SPR_17484_END*/
        /* Coverity Fix 83408 Fix Start */
        if (PNULL != p_radio_capability_info)
        {
            rrc_mem_free (p_radio_capability_info);
            p_radio_capability_info = PNULL;
        }
        /* Coverity Fix 83408 Fix Stop */
        return RRC_FAILURE;
    }

    /* SPR 14790 Fix Start */
    if (RRM_RRC_CELL_BARRED == p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
            p_csc_init_setup_ind->sib_type_1_Info.cell_access_related_info.cell_barred )
    {
        /* Set HO failure cause */
        p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_uecc_ue_context->ho_info.x2ap_cause.u.protocol = x2ap_cell_not_available; 

        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
            p_uecc_ue_context->p_gb_context->facility_name,
            RRC_WARNING,"Cell is Barred");
        /* Coverity Fix 83408 Fix Start */
        if (PNULL != p_radio_capability_info)
        {
            rrc_mem_free (p_radio_capability_info);
            p_radio_capability_info = PNULL;
        }
        /* Coverity Fix 83408 Fix Stop */
        RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
        return RRC_FAILURE;
    }
    /* SPR 14790 Fix Stop */
    /* Set HO failure cause to default */
    p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
    p_uecc_ue_context->ho_info.x2ap_cause.u.protocol = x2ap_unspecified_2; 

    p_uecc_ue_context->x2ap_context.old_enb_ue_x2ap_id 
        = p_rrc_x2ap_ho_request->x2ap_Old_UE_X2AP_ID;

    do {
        /*[TS 36.423-960, 8.2.1.4 Abnormal Conditions] 
          If the supported algorithms for encryption defined in the Encryption 
          Algorithms IE in the UE Security Capabilities IE, plus the mandated 
          support of EEA0 in all UEs [15], do not match any allowed algorithms 
          defined in the configured list of allowed encryption algorithms in 
          the eNB [15], the eNB shall reject the procedure using the HANDOVER 
          FAILURE message.
          If the supported algorithms for integrity defined in the Integrity 
          Protection Algorithms IE in the UE Security Capabilities IE, do not
          match any allowed algorithms defined in the configured list of 
          allowed integrity protection algorithms in the eNB [15] or if 
          all bits in Integrity Protection Algorithms IE are equal to 0, 
          the eNB shall reject the procedure using the HANDOVER FAILURE 
          message.*/

        /* Check security capabilities */
        result = uecc_fsm_trg_ho_check_and_process_x2ap_security_capability(
                p_uecc_ue_context,
                (void*)(&p_rrc_x2ap_ho_request->rrc_x2ap_ue_ctx_info.
                    ue_context_info.uESecurityCapabilities),
                (HandoverPreparationInformation_r8_IEs *)(&(p_rrc_x2ap_ho_request->
                        rrc_x2ap_ue_ctx_info.rRC_Container)));

        if (result != RRC_SUCCESS)
        {
            p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
            p_uecc_ue_context->ho_info.x2ap_cause.u.protocol =
                x2ap_encryption_and_or_integrity_protection_algorithms_not_supported;
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                "[%s] Security Algos not supported",__FUNCTION__);
            break;
        }

        asn1Copy_x2ap_EncryptionAlgorithms(&p_uecc_ue_context->s1ap_asn1_ctx,
                &p_rrc_x2ap_ho_request->rrc_x2ap_ue_ctx_info.ue_context_info.
                uESecurityCapabilities.encryptionAlgorithms,
                &p_uecc_ue_context->ue_security_capabilities.
                encryptionAlgorithms);

        asn1Copy_x2ap_IntegrityProtectionAlgorithms(&p_uecc_ue_context->
                s1ap_asn1_ctx,
                &p_rrc_x2ap_ho_request->rrc_x2ap_ue_ctx_info.ue_context_info.
                uESecurityCapabilities.integrityProtectionAlgorithms,
                &p_uecc_ue_context->ue_security_capabilities.
                integrityProtectionAlgorithms);

        /* Store NCC, sent in rrcConnectionReconfigurationComplete(
           Trg to src transparent container)*/
        p_uecc_ue_context->nh_ncc_pair.ncc = 
            p_rrc_x2ap_ho_request->rrc_x2ap_ue_ctx_info.
            ue_context_info.aS_SecurityInformation.
            nextHopChainingCount;

        /*SPR_17143_START*/
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
            p_uecc_ue_context->p_gb_context->facility_name,
            RRC_INFO,
            "[%s]:[UE=%u]: NCC Value received in X2 HO Req = %u",
            __FUNCTION__, p_uecc_ue_context->ue_index, p_uecc_ue_context->nh_ncc_pair.ncc);
        /*SPR_17143_END*/
        /* CSR 00077460 Fix Start */
        p_uecc_ue_context->ncc =  p_uecc_ue_context->nh_ncc_pair.ncc;
        /* CSR 00077460 Fix Stop */

        /* Mark the pair as USED */
        p_uecc_ue_context->nh_ncc_pair.used_flag = RRC_TRUE;

        /* Store the received NH value in UE context */
        asn1Copy_x2ap_Key_eNodeB_Star(&p_uecc_ue_context->s1ap_asn1_ctx,
                &p_rrc_x2ap_ho_request->rrc_x2ap_ue_ctx_info.ue_context_info.
                aS_SecurityInformation.key_eNodeB_star,
                (x2ap_Key_eNodeB_Star*)&p_uecc_ue_context->nh_ncc_pair.nh);

        p_uecc_ue_context->security_key.numbits = 256;
        l3_memcpy_wrapper(
                &p_uecc_ue_context->security_key.data[0],
                &p_rrc_x2ap_ho_request->rrc_x2ap_ue_ctx_info.ue_context_info.aS_SecurityInformation.key_eNodeB_star.data[0],
                32);
        p_rrc_container = &(p_rrc_x2ap_ho_request->rrc_x2ap_ue_ctx_info.
                rRC_Container);

        RRC_ASSERT(PNULL != p_uecc_ue_context->ho_info.p_ho_info);
        /* HD-FDD start */
        /*Fill ue_category in UE Context */
        if( (p_rrc_container->ue_RadioAccessCapabilityInfo.count < 1) || (p_rrc_container->ue_RadioAccessCapabilityInfo.
                    count > MAX_RAT_CAPABILITY))
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name,
                RRC_WARNING,
                "uecc_fsm_trg_ho_process_s1ap_handover_request- wrong list count [%u]",
                p_rrc_container->ue_RadioAccessCapabilityInfo.count);
            break;
        }

        /*crash fix*/
        p_node1 = p_rrc_container->ue_RadioAccessCapabilityInfo.head;

        for(counter = 0 ; counter < p_rrc_container->ue_RadioAccessCapabilityInfo.count ; counter++)
        {
            RRC_ASSERT(PNULL != p_node1);
            RRC_ASSERT(PNULL != p_node1->data);

            /* building rat capability */
            result =uecc_rrm_build_ue_rat_capability(p_uecc_ue_context,
                    p_radio_capability_info,p_node1->data);
            if(!result)
            {
                break;
            }
            p_node1 = p_node1->next;
        }
        /*SPR:5084 start */
        /* Save the EUTRA UE Category in UE Context */
        p_uecc_ue_context->ue_category = p_radio_capability_info->eutra_radio_capability_info.
            eutra_radio_capability_info_def.ue_category;
        /*SPR:5084 stop */
        /* Save the UE Mode in UE Context */
        p_uecc_ue_context->ue_mode = 0;/* Reset */

        if(RRC_SUCCESS != uecc_fsm_trg_x2_ho_process_store_radio_cap_info(
                    p_uecc_ue_context, p_radio_capability_info,
                    p_rrc_x2ap_ho_request, p_rrc_container))
        {
            break;
        }

        p_uecc_e_rab_failed_to_add_list =
            &p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
            s1_trg_ho_info.erab_failed_list;

        /*Target cell id */
        p_uecc_ue_context->ho_info.p_ho_info->x2ap_targetCell_ID =
            p_rrc_x2ap_ho_request->target_cell_id;

        p_uecc_ue_context->ue_aggregate_maximum_bitrate.
            uEaggregateMaximumBitRateDL =
            p_rrc_x2ap_ho_request->rrc_x2ap_ue_ctx_info.ue_context_info.
            uEaggregateMaximumBitRate.uEaggregateMaximumBitRateDownlink;
        p_uecc_ue_context->ue_aggregate_maximum_bitrate.
            uEaggregateMaximumBitRateUL =
            p_rrc_x2ap_ho_request->rrc_x2ap_ue_ctx_info.ue_context_info.
            uEaggregateMaximumBitRate.uEaggregateMaximumBitRateUplink;

        /* Store E-RAB configuration*/
        uecc_fsm_trg_x2_ho_process_store_erab_config(p_uecc_ue_context,
                p_rrc_x2ap_ho_request, p_uecc_e_rab_failed_to_add_list);

        /* Free the corresponding ERAB id memory in UECC UE 
           context for Duplicate ERAB IEs*/
        for (e_rab_index = 0 ; e_rab_index < 
                p_uecc_e_rab_failed_to_add_list->count; e_rab_index++)
        {
            if(PNULL == p_uecc_ue_context->p_e_rab_list[
                    p_uecc_e_rab_failed_to_add_list->
                    e_rab_failed_to_add_list[e_rab_index].e_rab_id])
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "[%s] ERAB id %d 's admission should have failed"
                    " due to GBR QOS check",
                    __FUNCTION__,p_uecc_e_rab_failed_to_add_list->
                    e_rab_failed_to_add_list[e_rab_index].e_rab_id);
                continue;
            }

            if (PNULL != p_uecc_ue_context->p_e_rab_list[
                    p_uecc_e_rab_failed_to_add_list->
                    e_rab_failed_to_add_list[e_rab_index].e_rab_id])
            {
                rrc_mem_free(p_uecc_ue_context->p_e_rab_list[
                        p_uecc_e_rab_failed_to_add_list->
                        e_rab_failed_to_add_list[e_rab_index].e_rab_id]);
                p_uecc_ue_context->p_e_rab_list[
                    p_uecc_e_rab_failed_to_add_list->
                    e_rab_failed_to_add_list[e_rab_index].e_rab_id] = PNULL;
            }

            if (!(--(p_uecc_ue_context->e_rab_list_count)))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                    "[%s] ERAB to be added has become zero !!",
                    __FUNCTION__);
                break;
            }
        }

        /* Redmine 650 Fix Start */
        if(p_rrc_x2ap_ho_request->rrc_x2ap_ue_ctx_info.ue_context_info.m.handoverRestrictionListPresent)
        {
            p_uecc_ue_context->m.ho_restriction_list_present=RRC_TRUE;

            asn1Copy_x2ap_HandoverRestrictionList(
                    &p_uecc_ue_context->s1ap_asn1_ctx,
                    &p_rrc_x2ap_ho_request->rrc_x2ap_ue_ctx_info.ue_context_info.handoverRestrictionList,
                    (x2ap_HandoverRestrictionList *)(&p_uecc_ue_context->ics_handover_restriction_list));
        }
        /* Redmine 650 Fix Stop */

        if (p_uecc_ue_context->e_rab_list_count)
        {
            if (RRC_FAILURE == (result =
                        uecc_fsm_trg_ho_is_atleast_one_non_gbr_bearer_present(
                            p_uecc_ue_context)))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                    "[%s] No Non GBR bearer present",
                    __FUNCTION__);
            }
        }
        else
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                "[%s] No Bearers left to set up!!!",
                __FUNCTION__);
            result = RRC_FAILURE;
            break;
        }
        /*SPR_19066_START*/
        if (p_rrc_container->m.rrm_ConfigPresent == 1)
        {
            p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                m.rrm_ConfigPresent = 1;
            if (p_rrc_container->rrm_Config.m.ue_InactiveTimePresent == 1)
            {
                p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                    rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                    rrm_Config.m.ue_InactiveTimePresent = 1;

                p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                    rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                    rrm_Config.ue_InactiveTime = p_rrc_container->rrm_Config.ue_InactiveTime;
            }
        }
        /*SPR_19066_END*/
    }while (0);

    if (PNULL != p_radio_capability_info)
    {
        rrc_mem_free (p_radio_capability_info);
        /* coverity fix 83408 start */
        p_radio_capability_info = PNULL;
        /* coverity fix 83408 end */
    }
    /* SPR 13502 Fix Stop */

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_s1_ho_free_duplicate_erab_id
 * Inputs         : p_gb_context - UECC global context
                    p_uecc_e_rab_failed_to_add_list - e-rab failed list
 * Outputs        : None
 * Returns        : RRC_SUCCESS - If S1AP HO Request is valid.
                    RRC_FAILURE - If S1AP HO Request is not valid.
 * Description    : This function is used to Free the corresponding ERAB id
 *                   memory in UECC UE context for Duplicate ERAB IEs
 ****************************************************************************/
static rrc_return_t uecc_fsm_trg_s1_ho_free_duplicate_erab_id(
        uecc_ue_context_t   *p_uecc_ue_context,      /* UECC UE context */
        uecc_e_rab_failed_to_add_list_t *p_uecc_e_rab_failed_to_add_list
)
{ 
    U16 e_rab_index = RRC_NULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    for (e_rab_index = 0 ; e_rab_index < 
            p_uecc_e_rab_failed_to_add_list->count; e_rab_index++)
    {
        if(PNULL == p_uecc_ue_context->p_e_rab_list[
                p_uecc_e_rab_failed_to_add_list->
                e_rab_failed_to_add_list[e_rab_index].e_rab_id])
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "[%s] ERAB id %d 's admission should have failed"
                    " due to GBR QOS check",
                    __FUNCTION__,p_uecc_e_rab_failed_to_add_list->
                    e_rab_failed_to_add_list[e_rab_index].e_rab_id);
            continue;
        }

        if (PNULL != p_uecc_ue_context->p_e_rab_list[
                p_uecc_e_rab_failed_to_add_list->
                e_rab_failed_to_add_list[e_rab_index].e_rab_id])
        {
            rrc_mem_free(p_uecc_ue_context->p_e_rab_list[
                    p_uecc_e_rab_failed_to_add_list->
                    e_rab_failed_to_add_list[e_rab_index].e_rab_id]);
            p_uecc_ue_context->p_e_rab_list[
                p_uecc_e_rab_failed_to_add_list->
                e_rab_failed_to_add_list[e_rab_index].e_rab_id] = PNULL;
        }

        if (!(--(p_uecc_ue_context->e_rab_list_count)))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                    "[%s] ERAB to be added has become zero !!",
                    __FUNCTION__);
            break;
        }
    }

    if (p_uecc_ue_context->e_rab_list_count)
    {
        if (RRC_FAILURE == uecc_fsm_trg_ho_is_atleast_one_non_gbr_bearer_present 
                 (p_uecc_ue_context))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                    "[%s] No Non GBR bearer present",
                    __FUNCTION__);
            return RRC_FAILURE;
        }
    }
    else
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                "[%s] No Bearers left to set up!!!",
                __FUNCTION__);
        return RRC_FAILURE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_s1_ho_store_erab_config
 * Inputs         : p_gb_context - UECC global context
                    p_rrc_s1ap_ho_request - Pointer to S1AP HO Request
 * Outputs        : None
 * Returns        : None
 * Description    : This validates the S1AP HO Request and stores the
                    necessary information in UECC UE context.
 ****************************************************************************/
static void uecc_fsm_trg_s1_ho_store_erab_config(
        uecc_ue_context_t   *p_uecc_ue_context,      /* UECC UE context */
        rrc_s1ap_handover_request_t  *p_rrc_s1ap_ho_request, /*RRC S1AP HO Req */ 
        uecc_e_rab_failed_to_add_list_t *p_uecc_e_rab_failed_to_add_list
)
{
    OSRTDListNode*      p_node = PNULL;
    U16 e_rab_list_count = RRC_NULL;
    U16 e_rab_index = RRC_NULL;
    U32 e_rab_id = RRC_NULL;
    U32 add_once_duplicate_e_rab_list[INVALID_ERAB_ID];
    s1ap_E_RABToBeSetupListHOReq_element *p_erab_elem = PNULL;
    s1ap_E_RABToBeSetupItemHOReq *p_e_rab_list[ASN1V_s1ap_maxnoofE_RABs] = {PNULL};
    /* CSR 00077460 Fix Start */
    U32 num_bytes = 0;
    U32 num_bits = 0;
    /* CSR 00077460 Fix Stop */

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    memset_wrapper(&add_once_duplicate_e_rab_list,0,
            sizeof(add_once_duplicate_e_rab_list));

    for (p_node = p_rrc_s1ap_ho_request->e_rab_to_be_setup_list.head;
            (e_rab_list_count< p_rrc_s1ap_ho_request->
             e_rab_to_be_setup_list.count);
            e_rab_list_count++)
    {
        RRC_ASSERT(PNULL != p_node); 
        RRC_ASSERT(PNULL != p_node->data); 

        p_erab_elem=(s1ap_E_RABToBeSetupListHOReq_element*)p_node->data;
        if (p_erab_elem->value.u._E_RABToBeSetupItemHOReqIEs_1 == PNULL)
        {
            continue;
        }
        p_e_rab_list[e_rab_list_count] = p_erab_elem->value.u._E_RABToBeSetupItemHOReqIEs_1;
	/*SPR 15896 Fix Start*/
        if (p_uecc_ue_context->m.traceActivated) 
        {
            P_RRC_TRACE (p_uecc_ue_context, set_minimum_medium_both_bitmask(), S1, "\n<ie name=\"E-RABID\">%d</ie>",p_e_rab_list[e_rab_list_count]->e_RAB_ID);
	/*SPR 15896 Fix Stop*/
        }

        p_node=p_node->next;
    }	
	/*SPR 15896 Fix Start*/
    if (p_uecc_ue_context->m.traceActivated) 
    {
        P_RRC_TRACE (p_uecc_ue_context, set_minimum_medium_both_bitmask(), S1, "</ieGroup> \n </msg>");
	/*SPR 15896 Fix Stop*/
    }

    for (e_rab_index=0;e_rab_index < e_rab_list_count;e_rab_index++)
    {
        e_rab_id = p_e_rab_list[e_rab_index]->e_RAB_ID;

        /* [TS 36.413, 8.4.2.4 Abnormal Conditions]
           If the eNB receives a HANDOVER REQUEST message containing 
           a E-RAB Level QoS Parameters IE which contains a QCI IE 
           indicating a GBR bearer (as defined in [13]), and which does 
           not contain the GBR QoS Information IE, the eNB shall not 
           admit the corresponding E-RAB*/
        /* Also presently,Only Standardized QCI values [TS 23.203,6.1.7.2]
           i.e for 1 to 9 are supported */

        if ((!RRC_IS_VALID_QCI_VALUE(p_e_rab_list[e_rab_index]->
                        e_RABlevelQosParameters.qCI)) || 
                (RRC_IS_GBR_BEARER(p_e_rab_list[e_rab_index]->
                                   e_RABlevelQosParameters.qCI) 
                 && !p_e_rab_list[e_rab_index]->
                 e_RABlevelQosParameters.m.gbrQosInformationPresent))
        {
            /* Add to GBR failed to add list */
            /*Erab id */
            p_uecc_e_rab_failed_to_add_list->
                e_rab_failed_to_add_list[
                p_uecc_e_rab_failed_to_add_list
                ->count].e_rab_id = e_rab_id;
            /*cause */
            p_uecc_e_rab_failed_to_add_list->
                e_rab_failed_to_add_list[
                p_uecc_e_rab_failed_to_add_list
                ->count].cause.t = T_s1ap_Cause_radioNetwork;
            p_uecc_e_rab_failed_to_add_list->
                e_rab_failed_to_add_list[
                p_uecc_e_rab_failed_to_add_list
                ->count].cause.u.radioNetwork = 
                s1ap_invalid_qos_combination;

            p_uecc_e_rab_failed_to_add_list->count++;
            /*SPR 13782 Fix Start*/
            if (p_uecc_e_rab_failed_to_add_list->count == e_rab_list_count)
		    {
			    p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
			    p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_invalid_qos_combination;
		    }
            /*SPR 13782 Fix Stop*/

            continue;
        }
        /*[TS 36.413, 8.4.2.4 Abnormal Conditions]
          If the eNB receives a HANDOVER REQUEST message containing 
          several E-RAB ID IEs (in the E-RABs To Be Setup List IE)
          set to the same value, the eNB shall not admit the 
          corresponding E-RABs.*/

/*SPR 13844 Fix Start*/
        if ((p_uecc_ue_context->p_e_rab_list[e_rab_id]) || (add_once_duplicate_e_rab_list[e_rab_id]))
/*SPR 13844 Fix Stop*/
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s]Duplicated ERAB ID IEs found %d !!",
                    __FUNCTION__,e_rab_id);
            /* Adding Duplicated ERABs only Once to failed to
               add list */
            if (!add_once_duplicate_e_rab_list[e_rab_id])
            {
                /*Erab id */
                p_uecc_e_rab_failed_to_add_list->
                    e_rab_failed_to_add_list[
                    p_uecc_e_rab_failed_to_add_list
                    ->count].e_rab_id = e_rab_id;
                /*cause */
                p_uecc_e_rab_failed_to_add_list->
                    e_rab_failed_to_add_list[
                    p_uecc_e_rab_failed_to_add_list
                    ->count].cause.t = T_s1ap_Cause_radioNetwork;
                p_uecc_e_rab_failed_to_add_list->
                    e_rab_failed_to_add_list[
                    p_uecc_e_rab_failed_to_add_list
                    ->count].cause.u.radioNetwork = 
                    s1ap_multiple_E_RAB_ID_instances;

                p_uecc_e_rab_failed_to_add_list->count++;
                add_once_duplicate_e_rab_list[e_rab_id] = 1;
/*SPR 13844 Fix Start*/
                rrc_mem_free(p_uecc_ue_context->p_e_rab_list[e_rab_id]);
                p_uecc_ue_context->p_e_rab_list[e_rab_id] = PNULL;
		p_uecc_ue_context->e_rab_list_count--;

		
/*SPR 13844 Fix Stop*/

            }
/*SPR 13844 Fix Start*/
	    if (e_rab_list_count == e_rab_index + 1 )
	    {
	    	if (!p_uecc_ue_context->e_rab_list_count )
	    	{
 			p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
			p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_multiple_E_RAB_ID_instances;		
    	    	}
	    } 
/*SPR 13844 Fix Stop*/
            continue;
        }
        else
        {
            p_uecc_ue_context->p_e_rab_list[e_rab_id] =
                (uecc_drb_ctx_t*)rrc_mem_get(sizeof(uecc_drb_ctx_t));

            if (PNULL == p_uecc_ue_context->p_e_rab_list[e_rab_id])
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                        "[%s]Memory allocation failed !!",__FUNCTION__);
                break;
            }
            p_uecc_ue_context->e_rab_list_count++;
        }

        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */

        p_uecc_ue_context->p_e_rab_list[e_rab_id]->e_RAB_ID = e_rab_id;

        asn1Copy_s1ap_E_RABLevelQoSParameters(&p_uecc_ue_context->
                s1ap_asn1_ctx,
                &p_e_rab_list[e_rab_index]->e_RABlevelQosParameters,
                &p_uecc_ue_context->p_e_rab_list[e_rab_id]->
                e_RABlevelQoSParameters);
        /*Bug_12446_fix_start*/
        if (RRC_SUCCESS != rrc_copy_transportlayer_address(&p_uecc_ue_context->
                p_e_rab_list[e_rab_id]->transportLayerAddress,
                &p_e_rab_list[e_rab_index]->transportLayerAddress,
                p_uecc_ue_context))
        {
            break;
        }

        /*asn1Copy_s1ap_TransportLayerAddress(&p_uecc_ue_context->
                s1ap_asn1_ctx,
                &p_e_rab_list[e_rab_index]->transportLayerAddress,
                &p_uecc_ue_context->p_e_rab_list[e_rab_id]->
                transportLayerAddress);*/
        /*Bug_12446_fix_stop*/

        /* CSR 00077460 Fix Start */
        num_bits = p_uecc_ue_context->p_e_rab_list[e_rab_id]->
                    transportLayerAddress.numbits;

        /* Copy the E-RAB Level QoS Parameters & Transport Layer Address for
         * the E-RAB finally accepted on Target eNB */
        if(num_bits%8)
        {
            num_bytes = (num_bits/8) + 1;
        }
        else
        {
            num_bytes = num_bits/8;
        }

        p_uecc_ue_context->p_e_rab_list[e_rab_id]->
            self_transport_address_length = (U8)num_bytes;

        l3_memcpy_wrapper(p_uecc_ue_context->p_e_rab_list[e_rab_id]->
                        self_transport_address, p_uecc_ue_context->p_e_rab_list[e_rab_id]->
                        transportLayerAddress.data, num_bytes);
        /* CSR 00077460 Fix Stop */

        p_uecc_ue_context->p_e_rab_list[e_rab_id]->gTP_TEID =
            p_e_rab_list[e_rab_index]->gTP_TEID;

        p_uecc_ue_context->p_e_rab_list[e_rab_id]->m.nAS_PDUPresent = 0;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_s1_ho_store_transparent_container
 * Inputs         : p_gb_context - UECC global context
                    p_rrc_s1ap_ho_request - Pointer to S1AP HO Request
 * Outputs        : None
 * Returns        : RRC_SUCCESS - If S1AP HO Request is valid.
                    RRC_FAILURE - If S1AP HO Request is not valid.
 * Description    : This validates the S1AP HO Request and stores the
                    necessary information in UECC UE context.
 ****************************************************************************/
static rrc_return_t uecc_fsm_trg_s1_ho_store_transparent_container(
    uecc_ue_context_t   *p_uecc_ue_context,      /* UECC UE context */
    rrc_s1ap_handover_request_t  *p_rrc_s1ap_ho_request, /*RRC S1AP HO Req */ 
    rrc_radio_capability_info_t   *p_radio_capability_info,
    uecc_e_rab_failed_to_add_list_t *p_uecc_e_rab_failed_to_add_list,
    rrc_return_t    *p_result
)
{ 
    HandoverPreparationInformation_r8_IEs * p_rrc_container = PNULL;
    OSRTDListNode*      p_node1 = PNULL;
    U8 counter;
    U8 count;

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

    if (p_rrc_s1ap_ho_request->source_to_target_transparent_container.
            bitmask &
            RRC_SOURCE_ENB_TO_TARGET_ENB_TRANSPARENT_CONTAINER_PRESENT)
    {
        p_rrc_container = &p_rrc_s1ap_ho_request->
            source_to_target_transparent_container. 
            source_enb_to_target_enb_transparent_container.rRC_Container;

        RRC_ASSERT(PNULL != p_uecc_ue_context->ho_info.p_ho_info);
        /* HD-FDD start */
        /*Fill ue_category in UE Context */
        if( (p_rrc_container->ue_RadioAccessCapabilityInfo.count < 1) || 
          (p_rrc_container->ue_RadioAccessCapabilityInfo.count > MAX_RAT_CAPABILITY))
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
                    p_uecc_ue_context->p_gb_context->facility_name,
                    RRC_WARNING,
                    "uecc_fsm_trg_ho_process_x2ap_handover_request- wrong list count [%u]",
                    p_rrc_container->ue_RadioAccessCapabilityInfo.count);
            return RRC_FAILURE;
        }
        
        p_node1 = p_rrc_container->ue_RadioAccessCapabilityInfo.head;

        for(counter = 0 ; counter < p_rrc_container->ue_RadioAccessCapabilityInfo.count ; counter++)
        {
            RRC_ASSERT(PNULL != p_node1);
            RRC_ASSERT(PNULL != p_node1->data);

            /* building rat capability */
            *p_result = uecc_rrm_build_ue_rat_capability(p_uecc_ue_context,
                        p_radio_capability_info,p_node1->data);
            if(!*p_result)
            {
                break;
            }
            p_node1 = p_node1->next;
        }
        /*SPR:5084 start */
        /* Save the EUTRA UE Category in UE Context */
        p_uecc_ue_context->ue_category = p_radio_capability_info->eutra_radio_capability_info.
            eutra_radio_capability_info_def.ue_category;
        /*SPR:5084 stop */
        /* 36321_CR0538 start */
        p_uecc_ue_context->access_stratum_release = p_radio_capability_info->eutra_radio_capability_info.
            eutra_radio_capability_info_def.access_stratum_release;
        /* 36321_CR0538 stop */
        /* Save the UE Mode in UE Context */
        p_uecc_ue_context->ue_mode = 0;/* Reset */

        /* SPR 11997 Fix Start */
        if (DUPLEXING_MODE_TDD != p_uecc_ue_context->p_gb_context->
              p_p_csc_context[p_uecc_ue_context->cell_index]->p_csc_init_setup_ind->duplexing_mode)
        {
        for(count = 0; count< p_radio_capability_info->eutra_radio_capability_info.
                eutra_radio_capability_info_def.rf_parameters.supported_band_list_eutra.count; count++)
        {
            if(p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                    p_csc_init_setup_ind->sib_type_1_Info.freq_band_indicator == 
                    p_radio_capability_info->eutra_radio_capability_info.
                    eutra_radio_capability_info_def.rf_parameters.supported_band_list_eutra.
                    supported_band_eutra[count].band_eutra)
            {
                if(p_radio_capability_info->eutra_radio_capability_info.
                        eutra_radio_capability_info_def.rf_parameters.supported_band_list_eutra.
                        supported_band_eutra[count].half_duplex == 1)
                {
                    p_uecc_ue_context->ue_mode = 1;
                    break;
                }
            } 

        }  
        }  
        /* SPR 11997 Fix Stop */
        /*HD-FDD stop */

        /* Fill Source Cell C-RNTI */
        if (p_rrc_container->m.as_ConfigPresent == 1)
        {
            p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                rrc_s1ap_ho_request.source_to_target_transparent_container.
                source_enb_to_target_enb_transparent_container.rRC_Container.
                as_Config.sourceUE_Identity  = p_rrc_container->as_Config.sourceUE_Identity;

            /* meas_config change start */
            /* fill meas_config in curr_meas_config */
            p_uecc_ue_context->curr_meas_config.is_meas_config_valid = 
                RRM_RRC_MC_VALIDATE_FAILURE;

            if (RRC_FAILURE == uecc_build_rrm_curr_meas_config(
                        p_uecc_ue_context,
                        &p_uecc_ue_context->curr_meas_config,
                        &p_rrc_container->as_Config.sourceMeasConfig))
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,RRC_WARNING,
                        "[%s] uecc_rrm_build_rrm_curr_meas_config "
                        "returned failure!\n",__FUNCTION__);
                return RRC_FAILURE;
            }
            else
            {
                p_uecc_ue_context->curr_meas_config.is_meas_config_valid =
                    RRM_RRC_MC_VALIDATE_SUCCESS;
            }
            if (p_uecc_ue_context->p_meas_config == PNULL)
            {
                p_uecc_ue_context->p_meas_config = (rrm_meas_config_t*) 
                    rrc_mem_get (sizeof (rrm_meas_config_t));

                if (PNULL == p_uecc_ue_context->p_meas_config)
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                            "[%s]Memory allocation failed !!",__FUNCTION__);
                    return RRC_FAILURE;
                }

                /* SES-508 Fix Start */
                /* Code Removed */
                /* SES-508 Fix End */
            }

            if ((p_rrc_container->as_Config.sourceMeasConfig.m.
                        measObjectToRemoveListPresent == 1) ||
                    (p_rrc_container->as_Config.sourceMeasConfig.m.
                     reportConfigToRemoveListPresent == 1) || 
                    (p_rrc_container->as_Config.sourceMeasConfig.m.
                     measIdToRemoveListPresent == 1))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                        "[%s] Remove Meas-config IE are present\n",
                        __FUNCTION__);

                if (RRC_FAILURE == uecc_build_rrm_meas_config_for_remove_list(
                            p_uecc_ue_context,
                            p_uecc_ue_context->p_meas_config,
                            &p_rrc_container->as_Config.sourceMeasConfig))
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name,RRC_WARNING,
                            "[%s]uecc_build_rrm_meas_config_for_remove_list "
                            "returned failure!\n",__FUNCTION__);
                    return RRC_FAILURE;
                }
            }
            if (p_uecc_ue_context->curr_meas_config.is_meas_config_valid ==
                    RRM_RRC_MC_VALIDATE_SUCCESS)
            {
                if (RRC_SUCCESS != uecc_rrc_update_CurrMeasConfigList(
                            p_uecc_ue_context->p_meas_config,
                            &p_uecc_ue_context->curr_meas_config,
                            p_uecc_ue_context->p_gb_context))
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                            "[%s] Failure to update CurrMeasConfigList.\n!!",
                            __FUNCTION__);
                }
            }

            /* meas_config change end */
        }
        if (p_rrc_container->m.as_ContextPresent == 1)
        {
            if (p_rrc_container->as_Context.m.reestablishmentInfoPresent == 1)
            {
                /* Fill Souce phyCellId */
                p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                    rrc_s1ap_ho_request.source_to_target_transparent_container.
                    source_enb_to_target_enb_transparent_container.rRC_Container.
                    as_Context.reestablishmentInfo.sourcePhysCellId  = 
                    p_rrc_container->as_Context.reestablishmentInfo.sourcePhysCellId;
                /* Fill short MAC-I */
                p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                    rrc_s1ap_ho_request.source_to_target_transparent_container.
                    source_enb_to_target_enb_transparent_container.rRC_Container.
                    as_Context.reestablishmentInfo.targetCellShortMAC_I  = 
                    p_rrc_container->as_Context.reestablishmentInfo.targetCellShortMAC_I;
            }
        }


        p_uecc_e_rab_failed_to_add_list = 
            &p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
            s1_trg_ho_info.erab_failed_list;


        p_uecc_ue_context->ho_info.s1_or_x2_handover = S1_HO;

        /*Target cell id */
        p_uecc_ue_context->ho_info.p_ho_info->targetCell_ID =
            p_rrc_s1ap_ho_request->
            source_to_target_transparent_container. 
            source_enb_to_target_enb_transparent_container.
            targetCell_ID;

        p_uecc_ue_context->mme_ue_id = p_rrc_s1ap_ho_request->
            mme_ue_s1ap_id;
        p_uecc_ue_context->ue_aggregate_maximum_bitrate.
            uEaggregateMaximumBitRateDL = 
            p_rrc_s1ap_ho_request->ue_aggregate_maximum_bitrate.
            uEaggregateMaximumBitRateDL;
        p_uecc_ue_context->ue_aggregate_maximum_bitrate.
            uEaggregateMaximumBitRateUL = 
            p_rrc_s1ap_ho_request->ue_aggregate_maximum_bitrate.
            uEaggregateMaximumBitRateUL;
        p_uecc_ue_context->ho_info.
            p_ho_info->ho_type = (U8)p_rrc_s1ap_ho_request->
            handover_type;

        /* Store E-RAB configuration*/
        uecc_fsm_trg_s1_ho_store_erab_config(p_uecc_ue_context,
                p_rrc_s1ap_ho_request, p_uecc_e_rab_failed_to_add_list);
    }
    else
    {
        return RRC_FAILURE;
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_s1_ho_gen_prot_evt_ciph_int_alg_mismatch
 * Inputs         : p_gb_context - UECC global context
                    p_rrc_s1ap_ho_request - Pointer to S1AP HO Request
 * Outputs        : None
 * Returns        : None
 * Description    : This function generated the Protocol Event for
 *                  LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH
 ****************************************************************************/
static void uecc_fsm_trg_s1_ho_gen_prot_evt_ciph_int_alg_mismatch(
        uecc_ue_context_t   *p_uecc_ue_context,
        LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH_STRUCT
                            *p_event_ciph_integrity_algo)
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    if (p_event_ciph_integrity_algo)
    {
        memset_wrapper(p_event_ciph_integrity_algo, RRC_NULL, 
                sizeof(LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH_STRUCT));
        p_event_ciph_integrity_algo->header.length = 
            sizeof(LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH_STRUCT);
        p_event_ciph_integrity_algo->header.event_id = 
            LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH;

        if ((PNULL != p_uecc_ue_context->p_gb_context) && (PNULL != 
                    p_uecc_ue_context->p_gb_context->p_p_csc_context) && (PNULL !=
                        p_uecc_ue_context->p_gb_context->
                        p_p_csc_context[p_uecc_ue_context->cell_index])
                && (PNULL != p_uecc_ue_context->p_gb_context->
                    p_p_csc_context[p_uecc_ue_context->cell_index]->
                    p_csc_init_setup_ind))
        {
            l3_memcpy_wrapper(&p_event_ciph_integrity_algo->header.EVENT_PARAM_CELL_ID, 
                    p_uecc_ue_context->p_gb_context->
                    p_p_csc_context[p_uecc_ue_context->cell_index]->
                    p_csc_init_setup_ind->sib_type_1_Info.
                    cell_access_related_info.cell_Id, sizeof(U32));
        }
        p_event_ciph_integrity_algo->header.EVENT_PARAM_EUTRANCELL_FROID = 
            p_event_ciph_integrity_algo->header.EVENT_PARAM_CELL_ID;

        if (p_uecc_ue_context->m.mme_ue_id_present)
        {
            p_event_ciph_integrity_algo->header.EVENT_PARAM_MMES1APID = 
                p_uecc_ue_context->mme_ue_id;
        }

        p_event_ciph_integrity_algo->header.EVENT_PARAM_RAC_UE_REF = 
            p_uecc_ue_context->s1ap_ue_id;

        rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_ciph_integrity_algo);
    }

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
            "[%s] Security Algos"
            " not supported",__FUNCTION__);
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_process_s1ap_handover_request
 * Inputs         : uecc_ue_context_t   *p_uecc_ue_context
                    rrc_s1ap_handover_request_t  *p_rrc_s1ap_ho_request
 * Outputs        : None
 * Returns        : RRC_SUCCESS - If S1AP HO Request is valid.
                    RRC_FAILURE - If S1AP HO Request is not valid.
 * Description    : This validates the S1AP HO Request and stores the
                    necessary information in UECC UE context.
 ****************************************************************************/
rrc_return_t uecc_fsm_trg_ho_process_s1ap_handover_request(
        uecc_ue_context_t   *p_uecc_ue_context,      /* UECC UE context */
        rrc_s1ap_handover_request_t  *p_rrc_s1ap_ho_request /*RRC S1AP HO Req */ 
)
{ 
    rrc_return_t    result = RRC_FAILURE;
    uecc_e_rab_failed_to_add_list_t  uecc_e_rab_failed_to_add_list;

    LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH_STRUCT
        *p_event_ciph_integrity_algo = PNULL;
    /* HD-FDD start */
    /* SPR 13502 Fix Start */
    rrc_radio_capability_info_t    *p_radio_capability_info = PNULL;
    /* HD-FDD stop */

    RRC_ASSERT(PNULL != p_uecc_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    p_radio_capability_info = (rrc_radio_capability_info_t *)rrc_mem_get
        (sizeof(rrc_radio_capability_info_t));
    if (PNULL == p_radio_capability_info)
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
              "[%s]Memory allocation failed !!",__FUNCTION__);
        return RRC_FAILURE;
    }
    /* SES-508 Fix Start */
    /* Code Removed */
    /* SES-508 Fix End */

    memset_wrapper(&uecc_e_rab_failed_to_add_list, 0, sizeof(uecc_e_rab_failed_to_add_list_t));

    if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(p_uecc_ue_context->p_gb_context,
                p_uecc_ue_context->cell_index))
    {
        /* SPR 14790 Fix Start */
        p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_uecc_ue_context->ho_info.cause.u.protocol = s1ap_cell_not_available;
        /* SPR 14790 Fix Stop */
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name,
                RRC_WARNING,"cell not active");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

        /*SPR_17484_START*/
        p_uecc_ue_context->ho_info.x2ap_cause.t = T_s1ap_Cause_radioNetwork;
        p_uecc_ue_context->ho_info.x2ap_cause.u.radioNetwork = s1ap_cell_not_available; 
        /*SPR_17484_END*/
        /* Coverity Fix 83407 Start */
        if (PNULL != p_radio_capability_info)
        {
            rrc_mem_free (p_radio_capability_info);
            p_radio_capability_info = PNULL;
        }
        /* Coverity Fix 83407 Stop */
        return RRC_FAILURE;
    }
    /* SPR 14790 Fix Start */
    if (RRM_RRC_CELL_BARRED == p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
            p_csc_init_setup_ind->sib_type_1_Info.cell_access_related_info.cell_barred )

    {
        p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_uecc_ue_context->ho_info.cause.u.protocol = s1ap_cell_not_available;
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name,
                RRC_WARNING,"Cell is Barred");
        /* Coverity Fix 83407 Start */
        if (PNULL != p_radio_capability_info)
        {
            rrc_mem_free (p_radio_capability_info);
            p_radio_capability_info = PNULL;
        }
        /* Coverity Fix 83407 Stop */
        RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
        return RRC_FAILURE;
    }
    /* SPR 14790 Fix Stop */

    /* (PNULL != p_uecc_ue_context) check done in calling function */
    /* (PNULL != p_rrc_s1ap_ho_request)check done in calling funtion*/
    /*SPR 15896 Fix Start*/
    if (p_uecc_ue_context->m.traceActivated) 
    {
        P_RRC_TRACE(p_uecc_ue_context, set_minimum_medium_both_bitmask(), S1,"\n <ieGroup name=\"E-RABs To Be Setup List \">");
        /*SPR 15896 Fix Stop*/
    }

    /* Set HO failure cause to default */
    p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
    p_uecc_ue_context->ho_info.cause.u.protocol =
        s1ap_ho_failure_in_target_EPC_eNB_or_target_system;

    p_uecc_ue_context->m.mme_ue_id_present = 1;
    p_uecc_ue_context->mme_ue_id = p_rrc_s1ap_ho_request->mme_ue_s1ap_id;

    do {


        /* SPR 2594 Start */
        /* Copy RRC Container Received */
        l3_memcpy_wrapper(&p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                rrc_s1ap_ho_request.source_to_target_transparent_container.
                source_enb_to_target_enb_transparent_container.rRC_Container,
                &p_rrc_s1ap_ho_request->source_to_target_transparent_container.
                source_enb_to_target_enb_transparent_container.rRC_Container,
                sizeof(HandoverPreparationInformation_r8_IEs));

        /*SPR_19066_START*/         
        if(RRC_SOURCE_ENB_TO_TARGET_ENB_TRANSPARENT_CONTAINER_PRESENT &
                p_rrc_s1ap_ho_request->source_to_target_transparent_container.bitmask)
        {
            p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                rrc_s1ap_ho_request.source_to_target_transparent_container.bitmask |=
                RRC_SOURCE_ENB_TO_TARGET_ENB_TRANSPARENT_CONTAINER_PRESENT;
        }
        /* SPR 2594 Stop */
        /*SPR_19066_END*/

        if ((s1ap_intralte == p_rrc_s1ap_ho_request->handover_type) ||
                (s1ap_utrantolte== p_rrc_s1ap_ho_request->handover_type) ||
                (s1ap_gerantolte == p_rrc_s1ap_ho_request->handover_type))
        {

            /*[TS 36.413, 8.4.2.4 Abnormal Conditions] 
              If the supported algorithms for encryption defined in the Encryption 
              Algorithms IE in the UE Security Capabilities IE, plus the mandated 
              support of EEA0 in all UEs [15], do not match any allowed algorithms 
              defined in the configured list of allowed encryption algorithms in 
              the eNB [15], the eNB shall reject the procedure using the HANDOVER 
              FAILURE message.
              If the supported algorithms for integrity defined in the Integrity 
              Protection Algorithms IE in the UE Security Capabilities IE, do not
              match any allowed algorithms defined in the configured list of 
              allowed integrity protection algorithms in the eNB [15] or if 
              all bits in Integrity Protection Algorithms IE are equal to 0, 
              the eNB shall reject the procedure using the HANDOVER FAILURE 
              message.*/

            /* Check security capabilities */
            result = uecc_fsm_icsp_check_and_process_security_capability(
                    p_uecc_ue_context,
                    (void*)(&p_rrc_s1ap_ho_request->ue_security_capabilities));

            if (result != RRC_SUCCESS)
            {
                p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                p_uecc_ue_context->ho_info.cause.u.protocol =
                    s1ap_encryption_and_or_integrity_protection_algorithms_not_supported;

                /* Generating Protocol Event
                 * LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH*/
                p_event_ciph_integrity_algo = 
                    rrc_mem_get(sizeof(LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH_STRUCT));
                uecc_fsm_trg_s1_ho_gen_prot_evt_ciph_int_alg_mismatch(
                        p_uecc_ue_context, p_event_ciph_integrity_algo);

                break;
            }

            asn1Copy_s1ap_EncryptionAlgorithms(&p_uecc_ue_context->s1ap_asn1_ctx,
                    &p_rrc_s1ap_ho_request->ue_security_capabilities.
                    encryptionAlgorithms,
                    &p_uecc_ue_context->ue_security_capabilities.encryptionAlgorithms);
            asn1Copy_s1ap_IntegrityProtectionAlgorithms(&p_uecc_ue_context->
                    s1ap_asn1_ctx,
                    &p_rrc_s1ap_ho_request->ue_security_capabilities.
                    integrityProtectionAlgorithms,
                    &p_uecc_ue_context->ue_security_capabilities.
                    integrityProtectionAlgorithms);

            /* Store the received NH value in UE context */
            p_uecc_ue_context->nh_ncc_pair.ncc = p_rrc_s1ap_ho_request->
                security_context.nextHopChainingCount;

            /*SPR_17143_START*/
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
                    p_uecc_ue_context->p_gb_context->facility_name,
                    RRC_INFO,
                    "[%s]:[UE=%u]: NCC Value received in S1 HO Req = %u",
                    __FUNCTION__, p_uecc_ue_context->ue_index, p_uecc_ue_context->nh_ncc_pair.ncc);
            /*SPR_17143_END*/
            /* CSR 00077460 Fix Start */
            p_uecc_ue_context->ncc =  p_uecc_ue_context->nh_ncc_pair.ncc;
            /* CSR 00077460 Fix Stop */

            /* Mark the pair as USED */
            /*p_uecc_ue_context->nh_ncc_pair.used_flag = RRC_TRUE;*/
            asn1Copy_s1ap_SecurityKey(&p_uecc_ue_context->s1ap_asn1_ctx,
                    &p_rrc_s1ap_ho_request->security_context.nextHopParameter,
                    (s1ap_SecurityKey*)&p_uecc_ue_context->nh_ncc_pair.nh);

            if ((s1ap_intralte == p_rrc_s1ap_ho_request->handover_type) ||
                    (s1ap_utrantolte== p_rrc_s1ap_ho_request->handover_type) ||
                    (s1ap_gerantolte == p_rrc_s1ap_ho_request->handover_type))
            {
                if ((PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context[
                            p_uecc_ue_context->cell_index]) &&
                        (PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context[
                         p_uecc_ue_context->cell_index]->p_csc_init_setup_ind))
                {
                    /* SPR 21610 Fix Start */
                    if(s1ap_intralte == p_rrc_s1ap_ho_request->handover_type)
                    {

                        rrc_generate_kenb_star(
                                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                cell_index]->p_csc_init_setup_ind->phys_cell_id,
                                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                cell_index]->p_csc_init_setup_ind->eutra_dl_carrier_freq,
                                &p_uecc_ue_context->security_key.data[0],
                                &p_rrc_s1ap_ho_request->security_context.
                                nextHopParameter.data[0]
                                );


                        /*SPR_17143_START*/
                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                                "KeNB* security_key - "
                                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]",
                                p_uecc_ue_context->security_key.data[0],
                                p_uecc_ue_context->security_key.data[1],
                                p_uecc_ue_context->security_key.data[2],
                                p_uecc_ue_context->security_key.data[3],
                                p_uecc_ue_context->security_key.data[4],
                                p_uecc_ue_context->security_key.data[5],
                                p_uecc_ue_context->security_key.data[6],
                                p_uecc_ue_context->security_key.data[7],
                                p_uecc_ue_context->security_key.data[8],
                                p_uecc_ue_context->security_key.data[9],
                                p_uecc_ue_context->security_key.data[10],
                                p_uecc_ue_context->security_key.data[11],
                                p_uecc_ue_context->security_key.data[12],
                                p_uecc_ue_context->security_key.data[13],
                                p_uecc_ue_context->security_key.data[14],
                                p_uecc_ue_context->security_key.data[15],
                                p_uecc_ue_context->security_key.data[16],
                                p_uecc_ue_context->security_key.data[17],
                                p_uecc_ue_context->security_key.data[18],
                                p_uecc_ue_context->security_key.data[19],
                                p_uecc_ue_context->security_key.data[20],
                                p_uecc_ue_context->security_key.data[21],
                                p_uecc_ue_context->security_key.data[22],
                                p_uecc_ue_context->security_key.data[23],
                                p_uecc_ue_context->security_key.data[24],
                                p_uecc_ue_context->security_key.data[25],
                                p_uecc_ue_context->security_key.data[26],
                                p_uecc_ue_context->security_key.data[27],
                                p_uecc_ue_context->security_key.data[28],
                                p_uecc_ue_context->security_key.data[29],
                                p_uecc_ue_context->security_key.data[30],
                                p_uecc_ue_context->security_key.data[31]);

                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                                "KeNB* Next Hope Parameter Data  - "
                                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]",
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[0],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[1],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[2],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[3],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[4],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[5],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[6],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[7],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[8],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[9],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[10],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[11],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[12],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[13],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[14],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[15],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[16],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[17],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[18],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[19],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[20],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[21],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[22],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[23],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[24],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[25],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[26],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[27],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[28],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[29],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[30],
                                p_rrc_s1ap_ho_request->security_context.nextHopParameter.data[31]);
                        /*SPR_17143_END*/
                    }
                    else
                    {
                        asn1Copy_s1ap_SecurityKey(&p_uecc_ue_context->s1ap_asn1_ctx,
                                &p_rrc_s1ap_ho_request->security_context.nextHopParameter,
                                &p_uecc_ue_context->security_key);
                    }
                    /* SPR 21610 Fix End */
                }
                else
                {
                    asn1Copy_s1ap_SecurityKey(&p_uecc_ue_context->s1ap_asn1_ctx,
                            &p_rrc_s1ap_ho_request->security_context.nextHopParameter,
                            &p_uecc_ue_context->security_key);
                }

                /* Transparent container */
                if(RRC_SUCCESS != uecc_fsm_trg_s1_ho_store_transparent_container(
                            p_uecc_ue_context, p_rrc_s1ap_ho_request, p_radio_capability_info, 
                            &uecc_e_rab_failed_to_add_list, &result))
                {
                    break;
                }

                /* Free the corresponding ERAB id memory in UECC UE 
                   context for Duplicate ERAB IEs*/
                result = uecc_fsm_trg_s1_ho_free_duplicate_erab_id(
                        p_uecc_ue_context, &uecc_e_rab_failed_to_add_list);

                /* for inter-RAT, NAS Security Parameters to Eutran is mandatory */

                if ((s1ap_utrantolte== p_rrc_s1ap_ho_request->handover_type) ||
                        (s1ap_gerantolte == p_rrc_s1ap_ho_request->handover_type))
                {
                    if(RRC_S1AP_HANDOVER_REQUEST_NAS_SECURITY_PARAMETERS_TO_E_UTRAN_PRESENT &
                            p_rrc_s1ap_ho_request->bitmask)
                    {
                        p_uecc_ue_context->nas_security_eutra_numocts = \
                                                                        p_rrc_s1ap_ho_request->\
                                                                        nas_security_parameters_to_e_utran.numocts;
                        l3_memcpy_wrapper(p_uecc_ue_context->nas_security_eutra,
                                p_rrc_s1ap_ho_request->\
                                nas_security_parameters_to_e_utran.data,
                                p_uecc_ue_context->nas_security_eutra_numocts);

                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                                "[%s] NAS Security Parameters to Eutran\
                                saved in UE Context in inter-RAT HO !!\n",
                                __FUNCTION__);
                    }
                    else
                    {
                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                                "[%s] NAS Security Parameters to Eutran\
                                should be present in inter-RAT HO !!\n",
                                __FUNCTION__);
                        result = RRC_FAILURE;
                        break;
                    }
                }
                /* cr_592 CSG check */

                if ( (PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context[
                            p_uecc_ue_context->cell_index]) &&
                        (PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context[
                         /* SPR 13973 Fix Start */
                         p_uecc_ue_context->cell_index]->p_csc_init_setup_ind) && 
                        ( CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG & 
                          p_uecc_ue_context->p_gb_context->p_p_csc_context[
                          p_uecc_ue_context->cell_index]->p_csc_init_setup_ind->
                          sib_type_1_Info.cell_access_related_info.presence_bitmask) )
                    /* SPR 13973 Fix Stop */
                {
                    /* CSG is of 27 bits only so masking 5 bits */
                    p_rrc_s1ap_ho_request->csg_id.data[3]  = p_rrc_s1ap_ho_request->csg_id.data[3] & CSG_RESET_VAL;
                    p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                        p_csc_init_setup_ind->sib_type_1_Info.cell_access_related_info.csg_identity[3]
                        = p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                        p_csc_init_setup_ind->sib_type_1_Info.cell_access_related_info.csg_identity[3]
                        &  CSG_RESET_VAL;

                    if((!(p_rrc_s1ap_ho_request->bitmask & RRC_S1AP_HANDOVER_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT))
                            && (p_rrc_s1ap_ho_request->bitmask & RRC_S1AP_HANDOVER_REQUEST_CSG_ID_PRESENT)
                            && (RRC_FALSE != memcmp_wrapper(p_rrc_s1ap_ho_request->csg_id.data,
                                    p_uecc_ue_context->p_gb_context->
                                    p_p_csc_context[p_uecc_ue_context->cell_index]->
                                    p_csc_init_setup_ind->sib_type_1_Info.
                                    cell_access_related_info.csg_identity,
                                    CSG_ID_OCTET_SIZE)))
                    {
                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,"Invalid CSG ID.\n");
                        result = RRC_FAILURE;
                        //filling cause for handover failure
                        p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                        p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_invalid_CSG_Id;
                        break;
                    }  
                }

                if((p_rrc_s1ap_ho_request->bitmask & RRC_S1AP_HANDOVER_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT)
                        /* SPR 12405 Fix Start */
                        && (s1ap_member == p_rrc_s1ap_ho_request->csg_membership_status)
                        /* SPR 12405 Fix Stop */
                        && (!(p_rrc_s1ap_ho_request->bitmask & RRC_S1AP_HANDOVER_REQUEST_CSG_ID_PRESENT)))
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,"CSG ID Not Present.\n");
                    result = RRC_FAILURE;
                    //filling cause for handover failure
                    p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_protocol;
                    p_uecc_ue_context->ho_info.cause.u.protocol = s1ap_semantic_error;
                    break;
                }
            }
        }
    }while (0);

    if (PNULL != p_radio_capability_info)
    {
        rrc_mem_free (p_radio_capability_info);
        /* coverity fix 83407 start */ 
        p_radio_capability_info = PNULL;
        /* coverity fix 83407 end */ 
    }
    /* SPR 13502 Fix Stop */

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_procedure_finish_fail
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Failure handler for Target HO procedure
 ****************************************************************************/
void uecc_fsm_ue_trg_ho_procedure_finish_fail(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
	/* coverity_fix_62464_start */
/*SPR 20636 Changes Start*/
	uecc_fsm_event_et     failure_ev = (uecc_fsm_event_et)((U32_64_Bit)p_api);

/*SPR 20636 Changes End*/
	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
			"Proc Failure handler");


	/* SPS Start */
    /* SPR 20747 Fix Start */  

    if ((p_uecc_ue_context->m.is_sps_recv_frm_rrm) && 
                   (p_uecc_ue_context->m.sps_config_updated))
	{
		if (p_uecc_ue_context->p_sps_config != PNULL)
		{
			rrc_mem_free(p_uecc_ue_context->p_sps_config);
			p_uecc_ue_context->p_sps_config = PNULL;
		}

		/* Send to MAC - RRC_MAC_RECONFIG_COMPLETE_IND */
		uecc_llim_build_and_send_mac_reconfig_complete_ind(
				p_uecc_ue_context->p_gb_context,
				p_uecc_ue_context,
				RRC_FAILURE);
	}
        /* SPR 20747 Fix End */
        /* SPS Stop */

    /* Bug#21403 start */
    if (p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.drb_tunnel_info_count > RRC_NULL)
    {
    RRC_UECC_TRG_HO_FSM_TRACE (RRC_WARNING, "Deleting forwarding tunnels [%d] ",
                        p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.drb_tunnel_info_count);
    /* Delete all the forwarding tunnels */
    if ( RRC_SUCCESS != uecc_llim_build_and_send_delete_trg_forwarding_tunnel_req(p_uecc_ue_context))
    {
        RRC_UECC_TRG_HO_FSM_TRACE (RRC_ERROR, "Either no forwarding tunnel exist or"
        " delivery of S1U_RECONFIGURE_REQ gets failed.");
        }
    }
    /* Bug#21403 end */


	/* Bug 799 Fix Start */
	rrc_delete_ho_ue_identity_node (p_uecc_ue_context);
        /* Bug 799 Fix Stop */
    /*SPR_17976_START*/
    /* Delete nodes stored in TeNB re-establishment queue */
    uecc_delete_tenb_cre_queue(p_uecc_ue_context,
            &(p_uecc_ue_context->p_tenb_cre_event_queue));
    /*SPR_17976_END*/

	/*BUG 604 changes start*/
	if (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
	{
		/*Enabling flag*/
		p_uecc_ue_context->share_data.uecc_rcr_data.
			s1u_release_not_req_flag = RRC_TRUE;
	}
	/*BUG 604 changes stop*/
	/* Stop timer */
	uecc_ue_timer_stop(p_uecc_ue_context->p_gb_context,
			UECC_UE_TRG_HO_GUARD_TIMER, 
			&p_uecc_ue_context->timers_data);

	/* Release all TRG HO related memory */
	uecc_fsm_free_trg_ho_context(p_uecc_ue_context);
/*valgrind_fix_start*/
	if (PNULL != p_uecc_ue_context->p_meas_config)
	{
		rrc_mem_free(p_uecc_ue_context->p_meas_config);
		p_uecc_ue_context->p_meas_config = PNULL;
	}
/*valgrind_fix_stop*/    
	p_uecc_ue_context->x2ap_reset_recvd_flag = RRC_FALSE;
        /* SPR 16750 Fix Start */
        p_uecc_ue_context->x2ap_context.m.error_indication_ongoing = 0;
        /* SPR 16750 Fix Stop */

	if(p_uecc_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present == 1)
	{
		/*Release new X2AP_UEID */
		uecc_x2ap_id_free(p_uecc_ue_context->p_gb_context,
				p_uecc_ue_context->x2ap_context.new_enb_ue_x2ap_id);
            /*SPR_16618_START*/
            p_uecc_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present = RRC_FALSE;
            /*SPR_16618_END*/
	}
	/* Send event to UE Main FSM */
	uecc_event_queue_push_event(
			failure_ev,
			p_uecc_ue_context,
			p_api);
	/*BUG 604 changes start*/
	//}
	/*BUG 604 changes stop*/

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

}
/* coverity_fix_62464_stop */

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_csc_cell_del_handler_type1
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handler for CSC delete all UE request.
                    Actions:
                    (a) Stop procedural timer
                    (b) Push UECC_EV_TRG_HO_FAILURE_INT to Main FSM
 ****************************************************************************/
void uecc_fsm_trg_ho_csc_cell_del_handler_type1(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    /*SPR 21499 Start*/
    /*Code Deleted*/
    /*SPR 21499 Stop*/
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_uecc_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received  CSC_DELETE_ALL_UE_REQ\n");

    /* Procedural timer stopped in finish_failure */

    /* X2AP HO_2 : Start*/
/*BUG 604 changes start*/
    if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
            (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
        /*BUG 604 changes stop*/

    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END: Sending "
                " X2AP HO PREPARATION FAILURE");
        /*SPR_17484_START*/
        p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_uecc_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_cell_not_available; 
        /*SPR_17484_END*/

        /* Send HO PREPARATION FAILURE to Source eNB */
        /* criticality diagnostics to be filled
           during error indication */
        uecc_x2ap_build_and_send_handover_prep_failure(
                p_uecc_ue_context,
                PNULL);
    } 
    /* X2AP HO_2 : End*/

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    if ((PNULL != p_uecc_ue_context->p_gb_context) &&
            (PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context) &&
            (PNULL != p_uecc_ue_context->p_gb_context->
             p_p_csc_context[p_uecc_ue_context->cell_index])
       )
    {
        /* SPR 21412 Fix End */
        /*SPR 21499 Start*/
        /*Code Deleted*/
        /*SPR 21499 Stop*/
        /* decrement the count_use_ue_del_cell_req parameter by 1 
         * This count will be used during UECC_CSC_DEL_ALL_UE_RESP */
        if(p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                del_cell_ongoing == 1)
        {
            if(p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                    count_use_ue_del_cell_req > 0)
                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                    count_use_ue_del_cell_req --;

            if(p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                    count_use_ue_del_cell_req == 0)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
                        "%s sending CSC DEL ALL UE ENTITY RESP",__FUNCTION__);
                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                    del_cell_ongoing = 0;
                uecc_csc_build_and_send_delete_all_ue_entity_resp(RRC_SUCCESS,
                        p_uecc_ue_context->p_gb_context,p_uecc_ue_context->cell_index);
                if (RRC_TRUE == p_uecc_ue_context->p_gb_context->
                        p_p_csc_context[p_uecc_ue_context->cell_index]->is_cell_stop_ongoing)
                {
                    p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                        is_cell_stop_ongoing = RRC_FALSE;
                }
                else
                {
                    /* Clean Cell Context */
                    if (p_uecc_ue_context->p_gb_context->p_p_csc_context != PNULL )
                    {
                        if (p_uecc_ue_context->p_gb_context->
                                p_p_csc_context[p_uecc_ue_context->cell_index] != PNULL )
                        {
                            if (p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_csc_init_setup_ind != PNULL)
                            {
                                rrc_mem_free (p_uecc_ue_context->p_gb_context->
                                        p_p_csc_context[p_uecc_ue_context->cell_index]->
                                        p_csc_init_setup_ind);
                                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_csc_init_setup_ind = PNULL;
                            }
                            if (p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_rrc_uecc_statistic != PNULL)
                            {
                                rrc_mem_free (p_uecc_ue_context->p_gb_context->
                                        p_p_csc_context[p_uecc_ue_context->cell_index]->p_rrc_uecc_statistic);
                                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_rrc_uecc_statistic = PNULL;
                            }
                            if (p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_cell_traffic_trace_info != PNULL)
                            {
                                rrc_mem_free (p_uecc_ue_context->p_gb_context->
                                        p_p_csc_context[p_uecc_ue_context->cell_index]->p_cell_traffic_trace_info);
                                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_cell_traffic_trace_info = PNULL;
                            }
                            rrc_mem_free(p_uecc_ue_context->p_gb_context->
                                    p_p_csc_context[p_uecc_ue_context->cell_index]);
                            p_uecc_ue_context->p_gb_context->
                                p_p_csc_context[p_uecc_ue_context->cell_index] = PNULL;
                        }
                    }
                }
            }
        }
        /*SPR 21499 Start*/
        /*Code Deleted*/
        /*SPR 21499 Stop*/
    }

    /* notify UE FSM about csc cell delete */
    /* Post event to UE FSM for future processing */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_csc_cell_del_handler_type2
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handler for CSC delete all UE request.
                    Actions:
                    (a) Stop procedural timer
                    (b) Set rrc_connection_release_not_required_flag in
                        UE context.
                    (c) Push UECC_EV_TRG_HO_CSC_DELETE_ALL_UE_REQ_INT 
                        to Main FSM
 ****************************************************************************/
void uecc_fsm_trg_ho_csc_cell_del_handler_type2(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received  CSC_DELETE_ALL_UE_REQ\n");

/*BUG 604 changes start*/
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "[%s] Toglling rrc_connection_release_not_required_flag",
            __FUNCTION__);
        /* Change Flag value */
        if(RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
                   p_trg_ho_info->is_rrc_conn_reconf_comp_recv)
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_FALSE;
        }
        else
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_TRUE;
        }
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

        /*RRC_RRM_HO_ADM_CNF to RRM*/
        uecc_rrm_build_and_send_ue_ho_adm_cnf(
                p_uecc_ue_context,
                RRC_FAILURE);

        /*Delete UE context at X2AP*/
        if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
        {
            uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
        }
/*BUG 604 changes stop*/

        /*spr_22003_changes_start*/
        /*As HO admission has been failed at target, so no need to increment the
         * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
        p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
        /*spr_22003_changes_end*/
    /* notify UE FSM about csc cell delete */
    /* Post event to UE FSM for future processing */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_CSC_DELETE_ALL_UE_REQ_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_csc_cell_del_handler_type3
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handler for CSC delete all UE request.
                    Actions:
                    (a) Stop procedural timer
                    (b) Send UECC_LLIM_HO_REL_RACH_RESOURCE_IND to LLIM
                    (c) Push UECC_EV_TRG_HO_FAILURE_INT to Main FSM
 ****************************************************************************/
void uecc_fsm_trg_ho_csc_cell_del_handler_type3(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    /*SPR 21499 Start*/
    /*Code Deleted*/
    /*SPR 21499 Stop*/

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_uecc_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received  CSC_DELETE_ALL_UE_REQ\n");

    /* Procedural timer stopped in finish_failure */

    /* X2AP HO_2 : Start*/
/*BUG 604 changes start*/
    if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
            (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
        /*BUG 604 changes start*/
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END: Sending "
                " X2AP HO PREPARATION FAILURE");

        /*SPR_17484_START*/
        p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_uecc_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_cell_not_available; 
        /*SPR_17484_END*/
        /* Send HO PREPARATION FAILURE to Source eNB */
        /* criticality diagnostics to be filled
           during error indication */
        uecc_x2ap_build_and_send_handover_prep_failure(
                p_uecc_ue_context,
                PNULL);
    } 
    /* X2AP HO_2 : End*/

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RACH REL RES IND(Fail)");

    /* Send UECC_LLIM_HO_REL_RACH_RESOURCE_IND to LLIM */
    uecc_llim_build_and_send_rrc_uecc_llim_ho_rach_rel_resource_ind (
            p_uecc_ue_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    if ((PNULL != p_uecc_ue_context->p_gb_context) &&
            (PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context) &&
            (PNULL != p_uecc_ue_context->p_gb_context->
             p_p_csc_context[p_uecc_ue_context->cell_index])
       )
    {

        /*SPR 21499 Start*/
        /*Code Deleted*/
        /*SPR 21499 Stop*/
        /* decrement the count_use_ue_del_cell_req parameter by 1 
         * This count will be used during UECC_CSC_DEL_ALL_UE_RESP */
        if(p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                del_cell_ongoing == 1)
        {
            if(p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                    count_use_ue_del_cell_req > 0)
                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                    count_use_ue_del_cell_req --;

            if(p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                    count_use_ue_del_cell_req == 0)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
                        "%s sending CSC DEL ALL UE ENTITY RESP",__FUNCTION__);
                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                    del_cell_ongoing = 0;
                uecc_csc_build_and_send_delete_all_ue_entity_resp(RRC_SUCCESS,
                        p_uecc_ue_context->p_gb_context,p_uecc_ue_context->cell_index);
                if (RRC_TRUE == p_uecc_ue_context->p_gb_context->
                        p_p_csc_context[p_uecc_ue_context->cell_index]->is_cell_stop_ongoing)
                {
                    p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                        is_cell_stop_ongoing = RRC_FALSE;
                }
                else
                {
                    if (p_uecc_ue_context->p_gb_context->p_p_csc_context != PNULL )
                    {
                        if (p_uecc_ue_context->p_gb_context->
                                p_p_csc_context[p_uecc_ue_context->cell_index] != PNULL )
                        {
                            if (p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_csc_init_setup_ind != PNULL)
                            {
                                rrc_mem_free (p_uecc_ue_context->p_gb_context->
                                        p_p_csc_context[p_uecc_ue_context->cell_index]->
                                        p_csc_init_setup_ind);
                                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_csc_init_setup_ind = PNULL;
                            }
                            if (p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_rrc_uecc_statistic != PNULL)
                            {
                                rrc_mem_free (p_uecc_ue_context->p_gb_context->
                                        p_p_csc_context[p_uecc_ue_context->cell_index]->p_rrc_uecc_statistic);
                                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_rrc_uecc_statistic = PNULL;
                            }
                            if (p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_cell_traffic_trace_info != PNULL)
                            {
                                rrc_mem_free (p_uecc_ue_context->p_gb_context->
                                        p_p_csc_context[p_uecc_ue_context->cell_index]->p_cell_traffic_trace_info);
                                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_cell_traffic_trace_info = PNULL;
                            }

                            rrc_mem_free(p_uecc_ue_context->p_gb_context->
                                    p_p_csc_context[p_uecc_ue_context->cell_index]);
                            p_uecc_ue_context->p_gb_context->
                                p_p_csc_context[p_uecc_ue_context->cell_index] = PNULL;
                        }
                    }
                }
            }
        }
        /*SPR 21499 Start*/
        /*Code Deleted*/
        /*SPR 21499 Stop*/
    }

    /* Post event to UE FSM for future processing */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/* X2AP HO_3 : Start*/
/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_csc_cell_del_handler_type4
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handler for CSC delete all UE request.
                    Actions:
                    (a) Stop procedural timer
                    (b) Set rrc_connection_release_not_required_flag in
                        UE context.
                    (c) Push UECC_EV_TRG_HO_CSC_DELETE_ALL_UE_REQ_INT 
                        to Main FSM
 ****************************************************************************/
void uecc_fsm_trg_ho_csc_cell_del_handler_type4(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received  CSC_DELETE_ALL_UE_REQ\n");

    /* Procedural timer stopped in finish_failure */
/*BUG 604 changes start*/
    if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes start*/
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END: Sending "
                " X2AP HO PREPARATION FAILURE");

        /*SPR_17484_START*/
        p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
        p_uecc_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_cell_not_available; 
        /*SPR_17484_END*/

        /* Send HO PREPARATION FAILURE to Source eNB */
        /* criticality diagnostics to be filled
           during error indication */
        uecc_x2ap_build_and_send_handover_prep_failure(
                p_uecc_ue_context,
                PNULL);
    } 


    p_uecc_ue_context->share_data.uecc_rcr_data.
        rrc_connection_release_not_required_flag = RRC_TRUE;


    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    /* notify UE FSM about csc cell delete */
    /* Post event to UE FSM for future processing */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_CSC_DELETE_ALL_UE_REQ_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/* X2AP HO_3 : End*/

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_reset_handler_type1
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handler for CSC delete all UE request.
                    Actions:
                    (a) Stop procedural timer
                    (b) Push UECC_EV_TRG_HO_FAILURE_INT to Main FSM
 ****************************************************************************/
void uecc_fsm_trg_ho_reset_handler_type1(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received  S1AP RESET IND\n");
    /* Procedural timer stopped in finish_failure */

    /* X2AP RW_2 : Start*/
/*BUG 604 changes start*/
    if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END: Sending "
                " X2AP HO PREPARATION FAILURE");

        /* Send HO PREPARATION FAILURE to Source eNB */
        /* criticality diagnostics to be filled
           during error indication */
        uecc_x2ap_build_and_send_handover_prep_failure(
                p_uecc_ue_context,
                PNULL);
    } 
    /* X2AP RW_2 : End*/

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");
    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);
    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    /* notify UE FSM about csc cell delete */
    /* Post event to UE FSM for future processing */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_reset_handler_type2
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handler for CSC delete all UE request.
                    Actions:
                    (a) Stop procedural timer
                    (b) Set rrc_connection_release_not_required_flag in UECC
                        UE context.
                    (c) Push UECC_EV_TRG_HO_RESET_REQ_INT 
                        to Main FSM
 ****************************************************************************/
void uecc_fsm_trg_ho_reset_handler_type2(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received  S1AP RESET IND\n");

    /* Procedural timer stopped in finish_failure */

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");
    
/*BUG 604 changes start*/
        /*RRC_RRM_HO_ADM_CNF to RRM*/
        uecc_rrm_build_and_send_ue_ho_adm_cnf(
                p_uecc_ue_context,
                RRC_FAILURE);

        /*Delete UE context at X2AP*/
        if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
        {
            uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
        }
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "[%s] Toggling rrc_connection_release_not_required_flag",
                __FUNCTION__);
        
        if(RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
                    p_trg_ho_info->is_rrc_conn_reconf_comp_recv)
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_FALSE;
        }
        else
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_TRUE;
        }
/*BUG 604 changes stop*/

        /*spr_22003_changes_start*/
        /*As HO admission has been failed at target, so no need to increment the
         * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
        p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
        /*spr_22003_changes_end*/

    /* notify UE FSM about csc cell delete */
    /* Post event to UE FSM for future processing */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RESET_REQ_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_reset_handler_type3
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handler for CSC delete all UE request.
                    Actions:
                    (a) Stop procedural timer
                    (b) Send UECC_LLIM_HO_REL_RACH_RESOURCE_IND to LLIM
                    (c) Push UECC_EV_TRG_HO_FAILURE_INT to Main FSM
 ****************************************************************************/
void uecc_fsm_trg_ho_reset_handler_type3(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received  S1AP RESET IND\n");

    /* Procedural timer stopped in finish_failure */
    

    /* X2AP HO_1 : Start*/
/*BUG 604 changes start*/
    if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END: Sending "
                " X2AP HO PREPARATION FAILURE");

        /* Send HO PREPARATION FAILURE to Source eNB */
        /* criticality diagnostics to be filled
           during error indication */
        uecc_x2ap_build_and_send_handover_prep_failure(
                p_uecc_ue_context,
                PNULL);
    } 
    /* X2AP HO_1 : End*/

    /* Send UECC_LLIM_HO_REL_RACH_RESOURCE_IND to LLIM */
    uecc_llim_build_and_send_rrc_uecc_llim_ho_rach_rel_resource_ind (
            p_uecc_ue_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    /* Post event to UE FSM for future processing */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/* X2AP RW_2 : Start*/
/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_reset_handler_type4
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handler for CSC delete all UE request.
                    Actions:
                    (a) Stop procedural timer
                    (b) Set rrc_connection_release_not_required_flag in UECC
                        UE context.
                    (c) Push UECC_EV_TRG_HO_RESET_REQ_INT 
                        to Main FSM
 ****************************************************************************/
void uecc_fsm_trg_ho_reset_handler_type4(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received  S1AP RESET IND\n");

    /* Procedural timer stopped in finish_failure */

    /* X2AP RW_2 : Start*/
/*BUG 604 changes start*/
    if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END: Sending "
                " X2AP HO PREPARATION FAILURE");

        /* Send HO PREPARATION FAILURE to Source eNB */
        /* criticality diagnostics to be filled
           during error indication */
        uecc_x2ap_build_and_send_handover_prep_failure(
                p_uecc_ue_context,
                PNULL);
    } 
    /* X2AP RW_2 : End*/

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "[%s] Setting rrc_connection_release_not_required_flag",
            __FUNCTION__);
    p_uecc_ue_context->share_data.uecc_rcr_data.
        rrc_connection_release_not_required_flag = 
        RRC_CONNECTION_RELEASE_NOT_REQUIRED;

    /* notify UE FSM about csc cell delete */
    /* Post event to UE FSM for future processing */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RESET_REQ_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/* X2AP RW_2 : End*/
/*SPR_17348_START*/
/****************************************************************************
 * Function Name  : uecc_trg_ho_rach_res_ongoing_rrm_ue_connection_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Handler of RRM_UE_RELEASE_IND during trg_ho_rach_res_ongoing.
                    Actions:
                    (a) Stop procedural timer
                    (b) Send UECC_LLIM_HO_REL_RACH_RESOURCE_IND to LLIM
                    (c) Push UECC_EV_TRG_HO_FAILURE_INT to Main FSM
 ****************************************************************************/
void uecc_trg_ho_rach_res_ongoing_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    /* Procedural timer stopped in finish_failure */

    if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END: Sending "
                " X2AP HO PREPARATION FAILURE");

        /* Send HO PREPARATION FAILURE to Source eNB */
        /* criticality diagnostics to be filled
           during error indication */
        uecc_x2ap_build_and_send_handover_prep_failure(
                p_uecc_ue_context,
                PNULL);
    }
    else if(S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        /* Send HO Prep failure to MME */
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END: Sending S1AP HO FAILURE");

        uecc_s1ap_build_and_send_handover_failure(
                p_uecc_ue_context->p_gb_context,
                p_uecc_ue_context,
                p_uecc_ue_context->s1ap_ue_id,
                p_uecc_ue_context->mme_ue_id,
                p_uecc_ue_context->mme_id,
                &p_uecc_ue_context->ho_info.cause,
                (rrc_s1ap_error_indication_t *)PNULL);

    }

    /* Send UECC_LLIM_HO_REL_RACH_RESOURCE_IND to LLIM */
    uecc_llim_build_and_send_rrc_uecc_llim_ho_rach_rel_resource_ind (
            p_uecc_ue_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    /* Post event to UE FSM for future processing */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_trg_ho_llim_ongoing_rrm_ue_connection_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Handler of RRM_UE_RELEASE_IND during trg_ho_llim_ongoing.
                    Actions:
                    (a) Stop procedural timer
                    (b) Send handover prepration falure
                    (c) Push UECC_EV_TRG_HO_RESET_REQ_INT to Main FSM
 ****************************************************************************/
void uecc_trg_ho_llim_ongoing_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    /* Procedural timer stopped in finish_failure */

    if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END: Sending "
                " X2AP HO PREPARATION FAILURE");

        /* Send HO PREPARATION FAILURE to Source eNB */
        /* criticality diagnostics to be filled
           during error indication */
        uecc_x2ap_build_and_send_handover_prep_failure(
                p_uecc_ue_context,
                PNULL);
    }
    else if(S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        /* Send HO Prep failure to MME */
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END: Sending S1AP HO FAILURE");

        uecc_s1ap_build_and_send_handover_failure(
                p_uecc_ue_context->p_gb_context,
                p_uecc_ue_context,
                p_uecc_ue_context->s1ap_ue_id,
                p_uecc_ue_context->mme_ue_id,
                p_uecc_ue_context->mme_id,
                &p_uecc_ue_context->ho_info.cause,
                (rrc_s1ap_error_indication_t *)PNULL);

    }

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "[%s] Setting rrc_connection_release_not_required_flag",
            __FUNCTION__);

    p_uecc_ue_context->share_data.uecc_rcr_data.
        rrc_connection_release_not_required_flag = 
        RRC_CONNECTION_RELEASE_NOT_REQUIRED;

    /* Post event to UE FSM for future processing */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RESET_REQ_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/*********************************************************************************
 * Function Name  : uecc_trg_ho_exec_ongoing_rrm_ue_connection_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Handler of RRM_UE_RELEASE_IND during trg_ho_llim_ongoing.
                    Actions:
                    (a) Stop procedural timer
                    (b) Set rrc_connection_release_not_required_flag in UECC
                        UE context.
                    (c) Push UECC_EV_TRG_HO_RESET_REQ_INT 
                        to Main FSM
 ****************************************************************************/
void uecc_trg_ho_exec_ongoing_rrm_ue_connection_release_ind_handler(
    uecc_ue_context_t*  p_uecc_ue_context
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    /* Procedural timer stopped in finish_failure */

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);

        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "[%s] Toggling rrc_connection_release_not_required_flag",
                __FUNCTION__);
        
        if(RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
                    p_trg_ho_info->is_rrc_conn_reconf_comp_recv)
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_FALSE;
        }
        else
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_TRUE;
        }


        /*Delete UE context at X2AP*/
        if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
        {
            uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);

            /* Post event to UE FSM for future processing */
            uecc_fsm_finish_activity_failure(
                    p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RESET_REQ_INT);
        }
        else if(S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
        {
            s1ap_Cause    cause;
            /* Fill Ue Release cause */
            cause.t= T_s1ap_Cause_radioNetwork;
            cause.u.radioNetwork = s1ap_unspecified_2;
            /*Send UE CONTEXT RELEASE REQUEST message to S1AP */
            if(RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                        p_uecc_ue_context,
                        &cause))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                        "[%s]Unable to build and send enb_ue_ctx_release_req ",__FUNCTION__);
            }
            /*Finish child FSM by posting
              UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT event to main FSM. */
            uecc_fsm_finish_activity_failure(
                    p_uecc_ue_context, (void*)UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT);
        }
        else
        {
            /* Post event to UE FSM for future processing */
            uecc_fsm_finish_activity_failure(
                    p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RESET_REQ_INT);
        }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}


/*SPR_17348_END*/

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_rrm_adm_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Timer expiry handler for UE in "UECC_TRG_HO_RRM_ADM_ONGOING" 
 *                  state.
 ****************************************************************************/
void uecc_fsm_trg_ho_rrm_adm_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    
    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_uecc_ue_context,
            uecc_fsm_ue_trg_ho_fail_type10_actions);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_mac_rach_res_get_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is intializer for State 
                    "UECC_TRG_HO_MAC_RACH_RES_GET_ONGOING"
 ****************************************************************************/
void uecc_fsm_trg_ho_mac_rach_res_get_ongoing_init_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_BRIEF,
            "Sending  UECC LLIM HO RACH RESOURCE REQ");
    /* Send UECC_LLIM_MAC_RACH_RES_REQ to MAC*/
/*BUG 604 changes start*/
    if (RRC_SUCCESS !=
    uecc_llim_build_and_send_rrc_uecc_llim_ho_rach_resource_req
    (p_uecc_ue_context))
/*BUG 604 changes stop*/
    {
        uecc_fsm_start_failure_processing(
                p_uecc_ue_context,
               uecc_fsm_ue_trg_ho_fail_type10_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_mac_rach_res_get_ongoing_mac_rach_resp_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of UECC_EV_LLIM_RACH_RES_RESP
 ****************************************************************************/
void uecc_fsm_trg_ho_mac_rach_res_get_ongoing_mac_rach_resp_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    uecc_trg_ho_info_t  *p_trg_ho_info = PNULL;
    rrc_uecc_llim_ho_rach_resource_resp_t * 
        p_rrc_uecc_llim_ho_rach_resource_resp
        = (rrc_uecc_llim_ho_rach_resource_resp_t*) p_api;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);


    if ((RRC_SUCCESS == p_rrc_uecc_llim_ho_rach_resource_resp->response_code)
            && (RRC_UECC_LLIM_CRNTI_PRESENT &
                p_rrc_uecc_llim_ho_rach_resource_resp->bitmask))
    {
        p_trg_ho_info = p_uecc_ue_context->ho_info.
            p_ho_info->p_trg_ho_info;
        /*save the RACH resource in UE context.*/
        if (RRC_UECC_LLIM_PREAMBLE_INFO_PRESENT & 
                p_rrc_uecc_llim_ho_rach_resource_resp->bitmask)
        {
            p_trg_ho_info->m.ra_preamble_index_present = 
                RA_PREAMBLE_INDEX_PRESENT;
            p_trg_ho_info->m.ra_prach_mask_index_present = 
                RA_PRACH_MASK_INDEX_PRESENT;

            p_trg_ho_info->ra_PRACH_mask_index = 
                p_rrc_uecc_llim_ho_rach_resource_resp->preamble_info.
                prach_mask_idx;
            p_trg_ho_info->ra_preamble_index = 
                p_rrc_uecc_llim_ho_rach_resource_resp->preamble_info.
                preamble_idx;
        }

        p_uecc_ue_context->crnti = 
            p_rrc_uecc_llim_ho_rach_resource_resp->crnti;

        /* Change state to UECC_TRG_HO_CREATE_UE_ENTITY_ONGOING */
        UECC_FSM_SET_CHILD_STATE(
                p_uecc_ue_context,
                &uecc_fsm_trg_ho_create_ue_entity_ongoing, PNULL);
    }
    else
    {
/*BUG 604 changes start*/
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "Received  UECC LLIM HO RACH RESOURCE RESP (failure)\n");
        /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_uecc_ue_context,
                    uecc_fsm_ue_trg_ho_fail_type10_actions);
/*BUG 604 changes stop*/

    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_mac_rach_res_get_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is HO procedural timer expiry handler for State 
                    "UECC_TRG_HO_MAC_RACH_RES_GET_ONGOING"
 ****************************************************************************/
void uecc_fsm_trg_ho_mac_rach_res_get_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_uecc_ue_context,
            uecc_fsm_ue_trg_ho_fail_type3_actions);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_fail_action_last
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of UECC_TRG_HO_FAIL_ACTION_LAST
 ****************************************************************************/
void uecc_fsm_ue_trg_ho_fail_action_last(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END");

    /* notify procedure about failure */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_fail_action_send_ho_failure
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : The funtion sends Handover failure to S1AP.
 ****************************************************************************/
void uecc_fsm_ue_trg_ho_fail_action_send_ho_failure(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

/*BUG 604 changes start*/
    if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END: Sending " 
                " X2AP HO PREPARATION FAILURE");

        /* Send HO PREPARATION FAILURE to Source eNB */
        /* criticality diagnostics to be filled
           during error indication */
        uecc_x2ap_build_and_send_handover_prep_failure(
                p_uecc_ue_context,
                (x2ap_CriticalityDiagnostics*)PNULL);
    }
    else
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END: Sending S1AP HO FAILURE");

        /*send HO FAILURE to MME*/
        /* SPR 16149 Fix Start*/
        uecc_s1ap_build_and_send_handover_failure(
                p_uecc_ue_context->p_gb_context,
                p_uecc_ue_context,
                p_uecc_ue_context->s1ap_ue_id,
                p_uecc_ue_context->mme_ue_id,
                p_uecc_ue_context->mme_id,
                &p_uecc_ue_context->ho_info.cause,
                (rrc_s1ap_error_indication_t *)PNULL);
        /* SPR 16149 Fix Stop*/ 
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_fail_action_send_rach_res_rel_ind
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : The funtion sends UECC_LLIM_RACH_RESOURCE_REL_IND to LLIM
 ****************************************************************************/
void uecc_fsm_ue_trg_ho_fail_action_send_rach_res_rel_ind(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END Sending RACH RES REL IND");

    /*send UECC_LLIM_RACH_RESOURCE_REL_IND to LLIM*/
    uecc_llim_build_and_send_rrc_uecc_llim_ho_rach_rel_resource_ind(p_uecc_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}



/****************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : The funtion sends Handover failure to S1AP.
 ****************************************************************************/
void uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/**********************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf_null_bitmask
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : The funtion sends Handover Adm Cnf failure to RRM.
 **********************************************************************************/
void uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf_failure(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
)
{
    
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf_failure(
                            p_uecc_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_create_ue_entity_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is intializer for State 
                    "UECC_TRG_HO_MAC_RACH_RES_GET_ONGOING"
 ****************************************************************************/
void uecc_fsm_trg_ho_create_ue_entity_ongoing_init_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

   RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILEDALL,"Unused variable in FSM:%p",
            p_api);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Sending UECC LLIM CREATE UE ENTITY REQUEST");
    /* UECC_LLIM_CREATE_UE_ENTIRY_REQ to create UE at lower layers */
    if (RRC_SUCCESS !=
            uecc_llim_build_and_send_create_ue_entity_req(p_uecc_ue_context))
    {
        uecc_fsm_start_failure_processing(
                p_uecc_ue_context,
               uecc_fsm_ue_trg_ho_fail_type3_actions);
    }


    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_process_radio_resource_cfg_params
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : Process data from rrm_ho_ue_admission_resp message and
 *                  update UE context for DRB_SUB_FWR_LIST,
 *                  RLF_TIMERS_AND_CONSTANTS
 ****************************************************************************/
static rrc_return_t uecc_fsm_trg_ho_process_radio_resource_cfg_params(
    uecc_ue_context_t*           p_uecc_ue_context,
    rrc_rrm_ue_ho_adm_resp_t* p_rrc_rrm_ho_adm_resp
)
{
    U8 return_failure = 0;
    U8 drb_count = RRC_NULL;
    uecc_ho_drb_forward_info_list_t          
         **pp_erab_trg_ho_forward_list = PNULL;
    rrm_drb_sub_to_fwd_list_t            * p_drb_sub_to_fwd_list = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    if ( RADIO_RESOURCE_CONFIG_DRB_SUB_FWR_LIST_PRESENT &
            p_rrc_rrm_ho_adm_resp->radio_resource_config.bitmask )
    {
        pp_erab_trg_ho_forward_list = &p_uecc_ue_context->ho_info.
            p_ho_info->p_trg_ho_info->s1_trg_ho_info.
            p_erab_trg_ho_forward_list[0];

        p_drb_sub_to_fwd_list = &p_rrc_rrm_ho_adm_resp->
            radio_resource_config.drb_sub_to_fwd_list;

        p_uecc_ue_context->ho_info.p_ho_info->
            p_trg_ho_info->s1_trg_ho_info.trg_ho_forward_list_count =
            p_drb_sub_to_fwd_list->count;

        for (drb_count = 0; drb_count < p_drb_sub_to_fwd_list->count;
                drb_count++)
        {
            pp_erab_trg_ho_forward_list[p_drb_sub_to_fwd_list->
                drb_sub_to_fwd[drb_count].erab_id] = 
                (uecc_ho_drb_forward_info_list_t*) 
                rrc_mem_get(sizeof(uecc_ho_drb_forward_info_list_t));

            if (!pp_erab_trg_ho_forward_list[p_drb_sub_to_fwd_list->
                    drb_sub_to_fwd[drb_count].erab_id])
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                        "[%s]Memory allocation failed !!",__FUNCTION__);
                return_failure = 1;
                break;
            }
            /* SES-508 Fix Start */
            /* Code Removed */
            /* SES-508 Fix End */
            pp_erab_trg_ho_forward_list[p_drb_sub_to_fwd_list->
                drb_sub_to_fwd[drb_count].erab_id]->forward_direction =
                (rrm_forwarding_direction_et)p_drb_sub_to_fwd_list->drb_sub_to_fwd[drb_count].
                forward_direction;
        }
        if (return_failure)
        {
            return RRC_FAILURE;
        }
    }

    /* cr_220: Store RLF Timers & Constants from p_rrc_rrm_ho_adm_resp data
       from RRM into UE Context */
    if (RADIO_RESOURCE_CONFIG_RLF_TIMERS_AND_CONSTANTS_PRESENT &
            p_rrc_rrm_ho_adm_resp->radio_resource_config.bitmask )
    {

        p_uecc_ue_context->m.rlf_timers_and_constants_r9_present = 1;

        if( RLF_TIMERS_AND_CONSTANTS_SETUP_PRESENT & 
            p_rrc_rrm_ho_adm_resp->radio_resource_config.rlf_timers_and_constants_r9.bitmask)
        {

            if(RRC_NULL == p_uecc_ue_context->p_rlf_timers_and_constants_r9)
            {
                p_uecc_ue_context->p_rlf_timers_and_constants_r9 =
                    (rlf_timers_and_constants_r9_t *)rrc_mem_get(sizeof(rlf_timers_and_constants_r9_t));
            }
            if (PNULL != p_uecc_ue_context->p_rlf_timers_and_constants_r9)
            {
                p_uecc_ue_context->p_rlf_timers_and_constants_r9->bitmask = RLF_TIMERS_AND_CONSTANTS_SETUP_PRESENT;

                p_uecc_ue_context->p_rlf_timers_and_constants_r9->setup.t301_r9 =
                    p_rrc_rrm_ho_adm_resp->radio_resource_config.rlf_timers_and_constants_r9.setup.t301_r9;

                p_uecc_ue_context->p_rlf_timers_and_constants_r9->setup.t310_r9 =
                    p_rrc_rrm_ho_adm_resp->radio_resource_config.rlf_timers_and_constants_r9.setup.t310_r9;

                p_uecc_ue_context->p_rlf_timers_and_constants_r9->setup.n310_r9 =
                    p_rrc_rrm_ho_adm_resp->radio_resource_config.rlf_timers_and_constants_r9.setup.n310_r9;

                p_uecc_ue_context->p_rlf_timers_and_constants_r9->setup.t311_r9 =
                    p_rrc_rrm_ho_adm_resp->radio_resource_config.rlf_timers_and_constants_r9.setup.t311_r9;

                p_uecc_ue_context->p_rlf_timers_and_constants_r9->setup.n311_r9 =
                    p_rrc_rrm_ho_adm_resp->radio_resource_config.rlf_timers_and_constants_r9.setup.n311_r9;
            }

        }
    }

    /* Copy Proximity Configuration */
    if (p_rrc_rrm_ho_adm_resp->bitmask &
            RRC_RRM_UE_HO_ADM_RESP_CONFIG_PROXIMITY_PRESENT)
    {
        p_uecc_ue_context->m.proximity_config_present = 1;
        l3_memcpy_wrapper(&(p_uecc_ue_context->proximity_config),
                &(p_rrc_rrm_ho_adm_resp->proximity_config),
                sizeof(rrm_report_proximity_config_r9_t));

    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_process_meas_config
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : Process data from rrm_ho_ue_admission_resp message and
 *                  update UE context for Meas Config
 ****************************************************************************/
static rrc_return_t uecc_fsm_trg_ho_process_meas_config(
    uecc_ue_context_t*           p_uecc_ue_context,
    rrc_rrm_ue_ho_adm_resp_t* p_rrc_rrm_ho_adm_resp
)
{
    U8 meas_object_to_add_mod_list_count = 0;
    U8 i = RRC_NULL;
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    if ( RADIO_RESOURCE_CONFIG_MEAS_CONFIG_PRESENT &
            p_rrc_rrm_ho_adm_resp->radio_resource_config.bitmask )
    {

        if(p_rrc_rrm_ho_adm_resp->
                radio_resource_config.meas_config.bitmask & 
                UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT)
        {
            meas_object_to_add_mod_list_count =
                p_rrc_rrm_ho_adm_resp->radio_resource_config.
                meas_config.meas_object_to_add_mod_list.count;

            for(i=0; i <meas_object_to_add_mod_list_count;i++)
            {
                if(p_rrc_rrm_ho_adm_resp->radio_resource_config.meas_config.
                        meas_object_to_add_mod_list.
                        meas_object_to_add_mod[i].
                        meas_object.bitmask & 
                        MEAS_OBJECT_TO_ADD_EUTRA_PRESENT)
                {
                    p_uecc_ue_context->m.measObjectEutraPresent = RRC_TRUE;
                    break;
                }
            }
        }
        p_uecc_ue_context->m.meas_config_present = 1;
        p_uecc_ue_context->m.meas_config_updated = 1;

        /*Memory Optimization */
        /*valgrind_fix_start*/
        if (PNULL == p_uecc_ue_context->p_meas_config)
        {
            p_uecc_ue_context->p_meas_config = (rrm_meas_config_t*) 
                rrc_mem_get (sizeof (rrm_meas_config_t));

            if (PNULL == p_uecc_ue_context->p_meas_config)
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                        "[%s]Memory allocation failed !!",__FUNCTION__);
                return RRC_FAILURE;
            }
            /* SES-508 Fix Start */
            /* Code Removed */
            /* SES-508 Fix End */
        }
        /*valgrind_fix_stop*/

        l3_memcpy_wrapper(p_uecc_ue_context->p_meas_config,
                &p_rrc_rrm_ho_adm_resp->radio_resource_config.meas_config,
                sizeof(rrm_meas_config_t));


        /* meas_config change start */
        if (p_uecc_ue_context->curr_meas_config.is_meas_config_valid ==
                RRM_RRC_MC_VALIDATE_SUCCESS)
        {
            if (RRC_SUCCESS != uecc_rrc_update_CurrMeasConfigList(
                        p_uecc_ue_context->p_meas_config,
                        &p_uecc_ue_context->curr_meas_config,
                        p_uecc_ue_context->p_gb_context))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s] Failure to update CurrMeasConfigList.\n!!",
                        __FUNCTION__);
                return RRC_FAILURE;
            }
        }
        /* meas_config change end */

        /* Memory Optimization: Meas Config are updated in the Current Meas
         * Config. It should not get freed here in case of Trg HO
         * as the Radio Resources are built during HO Req ACK
         if (p_uecc_ue_context->p_meas_config ==  PNULL)
         {
         rrc_mem_free(p_uecc_ue_context->p_meas_config);
         p_uecc_ue_context->p_meas_config = PNULL;
         }*/
    }
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_process_mac_configuration
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : Process data from rrm_ho_ue_admission_resp message and
 *                  update UE context for MAC Configuration
 ****************************************************************************/
static rrc_return_t uecc_fsm_trg_ho_process_mac_configuration(
    uecc_ue_context_t*           p_uecc_ue_context,
    rrc_rrm_ue_ho_adm_resp_t* p_rrc_rrm_ho_adm_resp
)
{
    U16      sub_frame_assignment;
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    if(DUPLEXING_MODE_FDD == p_uecc_ue_context->p_gb_context->p_p_csc_context[
            p_uecc_ue_context->cell_index]->p_csc_init_setup_ind->
            duplexing_mode)
    {
        if ((p_rrc_rrm_ho_adm_resp->radio_resource_config.mac_config.
                    mac_main_config_extensions.dl_num_harq_process <
                    RRC_RRM_FDD_DL_HARQ_PROCESS_MIN) ||
                (p_rrc_rrm_ho_adm_resp->radio_resource_config.mac_config.
                 mac_main_config_extensions.dl_num_harq_process > RRC_RRM_FDD_DL_HARQ_PROCESS_MAX))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_UE_ADMISSION_RESP]"
                    "Duplexing Mode = FDD "
                    "and dl_num_harq_process = %u",
                    p_rrc_rrm_ho_adm_resp->radio_resource_config.mac_config.
                    mac_main_config_extensions.dl_num_harq_process);
            return RRC_FAILURE;
        }
    }

    if(DUPLEXING_MODE_TDD == p_uecc_ue_context->p_gb_context->p_p_csc_context[
            p_uecc_ue_context->cell_index]->p_csc_init_setup_ind->
            duplexing_mode)
    {
        sub_frame_assignment = p_uecc_ue_context->p_gb_context->p_p_csc_context[
            p_uecc_ue_context->cell_index]->p_csc_init_setup_ind->
            sib_type_1_Info.tdd_config.sub_frame_assignment;

        if (RRC_TRUE == p_rrc_rrm_ho_adm_resp->radio_resource_config.
                mac_config.mac_main_config.ul_sch_config.tti_bundling)
        {
            /* TTI bundling can be enabled for FDD and for TDD only for
             * configurations 0, 1 and 6 */
            if (sa_0 != sub_frame_assignment &&
                    sa_1 != sub_frame_assignment &&
                    sa_6 != sub_frame_assignment)
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[RRC_RRM_UE_ADMISSION_RESP] :"\
                        "TTI bundling can be enabled for FDD"\
                        " and for TDD only for"\
                        " configurations 0, 1 and 6 ");
                return RRC_FAILURE;
            }
            if(RADIO_RESOURCE_CONFIG_SPS_CONFIG_PRESENT &
                    p_rrc_rrm_ho_adm_resp->radio_resource_config.
                    bitmask)
            {
                /*For TDD, E-UTRAN does not simultaneously enable TTI
                 * bundling and semi-persistent scheduling */
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "For TDD, E-UTRAN does not simultaneously enable TTI"\
                        " bundling and semi-persistent scheduling");
                return RRC_FAILURE;
            }

        }
    }
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_process_sps_cfg_pucch_cfg_dedicated
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Process data from rrm_ho_ue_admission_resp message and
 *                  update UE context for PUCCH Configuration Dedicated and
 *                  SPS Config
 ****************************************************************************/
static void uecc_fsm_trg_ho_process_sps_cfg_pucch_cfg_dedicated(
    uecc_ue_context_t*           p_uecc_ue_context,
    rrc_rrm_ue_ho_adm_resp_t* p_rrc_rrm_ho_adm_resp
)
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    if (p_rrc_rrm_ho_adm_resp->radio_resource_config.
            physical_config_dedicated.bitmask &
            RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT)
    {
        if (RRC_PHY_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT &
                p_rrc_rrm_ho_adm_resp->radio_resource_config.
                physical_config_dedicated.
                pucch_configuration_dedicated.bitmask )
        {
            if (DUPLEXING_MODE_FDD == p_uecc_ue_context->p_gb_context->
                    p_p_csc_context[p_uecc_ue_context->cell_index]->p_csc_init_setup_ind->
                    duplexing_mode)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,
                        RRC_WARNING,"TDD ack nack feedback mode " \
                        " must be absent during in UE admission resp from RRM" \
                        "in case of FDD Cell");

                p_uecc_ue_context->physical_config_dedicated.
                    pucch_configuration_dedicated.bitmask =
                    (rrc_bitmask_t)(p_uecc_ue_context->physical_config_dedicated.\
                            pucch_configuration_dedicated.bitmask &
                            (~RRC_PHY_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT));
            }
        }
    }

    if ( RADIO_RESOURCE_CONFIG_SPS_CONFIG_PRESENT &
            p_rrc_rrm_ho_adm_resp->radio_resource_config.bitmask )
    {
        /* SPS Start */
        /* sps_config */
        if (RRC_FAILURE == uecc_rrm_parse_and_update_sps_config(
                    p_uecc_ue_context,
                    &p_rrc_rrm_ho_adm_resp->radio_resource_config.rrc_sps_config))
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name,
                    RRC_WARNING,"[RRC_RRM_ERB_SETUP_RESP] SPS config updation failed");
        }
        /* SPS Stop */
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_process_cqi_report_config
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Process data from rrm_ho_ue_admission_resp message and
 *                  update UE context for CQI Report Config
 ****************************************************************************/
static void uecc_fsm_trg_ho_process_cqi_report_config(
    uecc_ue_context_t*           p_uecc_ue_context
)
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    if (!(p_uecc_ue_context->physical_config_dedicated.bitmask &
                RRC_PHY_CQI_REPORTING_PRESENT))
    {
	    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
			    p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
			    "CQI_REPORTING Config is not present");
	    /*Bug_11255_start*/
	    if (p_uecc_ue_context->physical_config_dedicated.bitmask &
			    RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT)
	    {
		    p_uecc_ue_context->physical_config_dedicated.bitmask ^=
			    RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT;
	    }
	    /*Bug_11255_stop*/
    }    
    else
    {    
	    /*Bug_11255_start*/
	    if (p_uecc_ue_context->physical_config_dedicated.bitmask &
			    RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT)
	    { 
		    /*Bug_11255_stop*/
		    /* Varifying the conditional presence of CQI_MASK_R9 */
		    if (p_uecc_ue_context->physical_config_dedicated.cqi_report_config_v920.bitmask &
				    RRC_PHY_CQI_MASK_R9_PRESENT)
		    {
			    if (p_uecc_ue_context->physical_config_dedicated.cqi_reporting.
					    cqi_reporting_periodic.bitmask &
					    RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT)
			    {
				    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						    p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
						    "CQI_MASK_R9 :: Field cqi-ReportPeriodic is present"
						    " and set to SETUP");
			    }
			    else
			    {
				    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						    p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
						    "[RRC_RRM_HO_UE_ADMISSION_RESP] In PHY Dedicated Info, "
						    " or the field cqi-ReportPeriodic is set to RELEASE");
				    p_uecc_ue_context->physical_config_dedicated.cqi_report_config_v920.
					    bitmask ^= RRC_PHY_CQI_MASK_R9_PRESENT;
				    if (!(p_uecc_ue_context->physical_config_dedicated.
							    cqi_report_config_v920.bitmask & RRC_PHY_PMI_RI_REPORT_R9_PRESENT))
				    {
					    p_uecc_ue_context->physical_config_dedicated.bitmask ^=
						    RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT;
				    }    
			    }
		    }
		    /*Bug_11255_start*/
	    }
	    /*Bug_11255_stop*/
	    /* Varifying the conditional presence of pmi-RI-Report-r9 */
        if (p_uecc_ue_context->physical_config_dedicated.cqi_report_config_v920.bitmask &
                RRC_PHY_PMI_RI_REPORT_R9_PRESENT)
        {
            if ((p_uecc_ue_context->physical_config_dedicated.cqi_reporting.
                        cqi_reporting_periodic.bitmask &
                        RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT) &&
                    (p_uecc_ue_context->physical_config_dedicated.cqi_reporting.bitmask &
                     RRC_PHY_CQI_REPORTING_MODE_APERIODIC_PRESENT))
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
                        "PMI_RI_REPORT_R9 :: Field cqi-ReportPeriodic is present and set to SETUP "
                        " and cqi-ReportModeAperiodic is also Present");
            }
            else
            {    
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
                        "[RRC_RRM_HO_UE_ADMISSION_RESP] In PHY Dedicated Info, "
                        "Field cqi-ReportPeriodic is set to RELEASE"
                        " or APERIODIC is not present");
                p_uecc_ue_context->physical_config_dedicated.cqi_report_config_v920.
                    bitmask ^= RRC_PHY_PMI_RI_REPORT_R9_PRESENT;
                if (!(p_uecc_ue_context->physical_config_dedicated.cqi_report_config_v920.
                            bitmask & RRC_PHY_CQI_MASK_R9_PRESENT))            
                {
                    p_uecc_ue_context->physical_config_dedicated.bitmask ^=
                        RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT;
                }
            }
        }
    } 

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_process_rrm_ho_ue_adm_resp_data
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  p_rrc_rrm_ho_adm_resp* - pointer on message data
 * Outputs        : None
 * Returns        : None
 * Description    : Process data from rrm_ho_ue_admission_resp message and
 *                  update UE context.
 ****************************************************************************/
rrc_return_t uecc_fsm_trg_ho_process_rrm_ho_ue_adm_resp_data(
    uecc_ue_context_t*           p_uecc_ue_context,
    rrc_rrm_ue_ho_adm_resp_t* p_rrc_rrm_ho_adm_resp
)
{
    U8 drb_count = RRC_NULL;
    U8 e_rab_count = RRC_NULL;
    rrc_return_t result = RRC_FAILURE;
   
    rrm_drb_to_add_info_list_t  *p_drb_to_add_list = PNULL;
    uecc_e_rab_failed_to_add_list_t 
        *p_uecc_e_rab_failed_to_add_list = PNULL;
    rrm_drb_failed_to_add_info_list_t 
        *p_rrm_drb_failed_to_add_info_list = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    /* Store UE parameters in UE context and apply default values for them
     * if its needed.*/

    RRC_ASSERT(PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context);

    if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(p_uecc_ue_context->p_gb_context,
                p_uecc_ue_context->cell_index))
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name,
                RRC_WARNING,"cell not active");
        /*SPR_17484_START*/
        if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
        {
            p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork;
            p_uecc_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_cell_not_available; 
        }
        /*SPR_17484_END*/
        RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
        return RRC_FAILURE;
    }
    do {
        if ( (PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context[
                p_uecc_ue_context->cell_index]) &&
             (PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context[
                 p_uecc_ue_context->cell_index]->p_csc_init_setup_ind))
        {

            /*SPR_19066_START*/
            p_uecc_ue_context->m.ue_inactive_time_updated = RRC_TRUE;
            p_uecc_ue_context->ue_inactive_time = 
                p_rrc_rrm_ho_adm_resp->ue_inactive_time_config;

            /*SPR_19066_END*/
            
        p_drb_to_add_list = &p_rrc_rrm_ho_adm_resp->radio_resource_config.
            drb_to_add_list;
        if (!p_drb_to_add_list->drb_count)
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,"Num DRB to add ==0\n");
            break;
        }

        /* cr_311 sr_ConfigIndex range check */
        if (p_uecc_ue_context->m.access_stratum_release)
        {
            if (((p_rrc_rrm_ho_adm_resp->radio_resource_config.physical_config_dedicated.
            scheduling_request_config.scheduling_request_config_param.
            sr_configuration_index == 156) ||
            (p_rrc_rrm_ho_adm_resp->radio_resource_config.physical_config_dedicated.
            scheduling_request_config.scheduling_request_config_param.
            sr_configuration_index == 157)) &&
            ((p_uecc_ue_context->access_stratum_release == rrm_rel8)))
            {
                RRC_UECC_TRACE (p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,
                        RRC_INFO, "Release 8 do not support"
                        " Values 156 or 157 for sr_ConfigIndex");
            }
        }

        /* physical_config_dedicated */
        p_uecc_ue_context->m.physical_config_dedicated_present = 1;
        p_uecc_ue_context->m.physical_config_dedicated_updated = 1;

        /* Store physical_config_dedicated if present */
        p_uecc_ue_context->physical_config_dedicated =
            p_rrc_rrm_ho_adm_resp->radio_resource_config.
            physical_config_dedicated;

        /* CBSR Changes Start */
        if(RRC_PHY_ANTENNA_INFORMATION_PRESENT &
                p_uecc_ue_context->physical_config_dedicated.bitmask)
        {
            if(!(RRC_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT &
                        p_uecc_ue_context->physical_config_dedicated
                        .antenna_information.bitmask))
            {   /* Default values shall be used */
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
                        "[RRC_RRM_HO_UE_ADMISSION_RESP] In PHY Dedicated Info, "
                        "Antenna info is not present.");
            }
            else
            {
                if (RRC_FAILURE ==
                        uecc_fsm_validate_antenna_codebook_subset_restriction(
                            &p_uecc_ue_context->physical_config_dedicated.
                            antenna_information.antenna_information_explicit_value,
                            p_uecc_ue_context))
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
                            "[RRC_RRM_HO_UE_ADMISSION_RESP] Code Book Subset Restriction"
                            " Validation Failed");
                    return RRC_FAILURE;
                }
            }
        }
        /* CBSR Changes Stop */
    
        /* cr_316: verifying antenna_info_v920 along with TM 8 presence */
        if (p_uecc_ue_context->physical_config_dedicated.antenna_info_v920.bitmask &
        RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT)
        {
            if((RRC_PHY_ANTENNA_INFORMATION_PRESENT &
            p_uecc_ue_context->physical_config_dedicated.bitmask) &&
            (p_uecc_ue_context->physical_config_dedicated.antenna_information.
            antenna_information_explicit_value.transmission_mode == 
            TRANSMISSION_MODE_TM8_V920))
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
                        "Transmission Mode is set to --> 8");
            }
            else
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
                        "[RRC_RRM_HO_UE_ADMISSION_RESP] In PHY Dedicated Info, "
                        "Antenna info is not present."
                        "or Transmission Mode != 8");
                p_uecc_ue_context->physical_config_dedicated.bitmask ^=
                    RRC_PHY_ANTENNA_INFO_V920_PRESENT;
            }
        }

        /*Bug_12240_fix_start*/
        if ((p_uecc_ue_context->physical_config_dedicated.bitmask &
                    RRC_PHY_ANTENNA_INFO_R10_PRESENT) &&
                (p_uecc_ue_context->physical_config_dedicated.antenna_info_r10.
                 bitmask & RRC_ANTENNA_INFO_R10_EXPLICIT_VALUE_PRESENT))
        {    
            if (RRC_FAILURE ==
                    uecc_fsm_validate_antenna_codebook_subset_restriction_r10(
                        &p_uecc_ue_context->physical_config_dedicated.
                        antenna_info_r10.explicit_value,p_uecc_ue_context))
            {    
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
                        "[RRC_RRM_ERB_SETUP_RESP] Code Book Subset"
                        " Restriction for r10 Validation Failed");
                return RRC_FAILURE;
            }    
        }    
        /*Bug_12240_fix_stop*/
        /* CR 247: cqi_report_config_v920 */
        uecc_fsm_trg_ho_process_cqi_report_config(p_uecc_ue_context);

        uecc_fsm_trg_ho_process_sps_cfg_pucch_cfg_dedicated(
                    p_uecc_ue_context, p_rrc_rrm_ho_adm_resp);

        /* MAC configuration */
        if(RRC_SUCCESS != uecc_fsm_trg_ho_process_mac_configuration(
                            p_uecc_ue_context, p_rrc_rrm_ho_adm_resp))
        {
            result = RRC_FAILURE;
            break;
        }

        /* Store rrm_mac_config_t if present */
        p_uecc_ue_context->m.mac_config_present = 1;
        p_uecc_ue_context->m.mac_config_updated = 1;
        p_uecc_ue_context->m.mac_main_config_present =1;
        
        p_uecc_ue_context->mac_config =
        p_rrc_rrm_ho_adm_resp->radio_resource_config.mac_config;
        /* Store rrm_meas_config_t if present */
        if(RRC_SUCCESS != uecc_fsm_trg_ho_process_meas_config(
                          p_uecc_ue_context, p_rrc_rrm_ho_adm_resp))
        {
            break;
        }

        /* SRB configuration */
        /* Store SRB1 configuration if present */
        p_uecc_ue_context->m.srb1_config_present = 1;
        p_uecc_ue_context->m.srb1_config_updated = 1;

        p_uecc_ue_context->srb1_config =
            p_rrc_rrm_ho_adm_resp->radio_resource_config.srb_info.srb1_config;

        if((RRM_SRB_CONFIG_AM_CONFIG_PRESENT & 
                    p_uecc_ue_context->srb1_config.bitmask) &&
                (!(RRM_SRB_AM_CONFIG_EXPLICIT_PRESENT 
                   & p_uecc_ue_context->srb1_config.srb_am_config.bitmask)))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "[RRC_RRM_HO_UE_ADMISSION_RESP] In SRB AM Config, "
                    "explicit AM config is not present."
                    " Applying default values for it.");
            p_uecc_ue_context->srb1_config.bitmask =
                (rrc_bitmask_t)(p_uecc_ue_context->srb1_config.bitmask &
                                ~RRM_SRB_CONFIG_AM_CONFIG_PRESENT);
        }

        if((RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT& 
                    p_uecc_ue_context->srb1_config.bitmask) &&
                (!(RRM_UL_SPECIFIC_PARAMETERS_EXPLICIT_PRESENT 
                   & p_uecc_ue_context->srb1_config.
                   logical_channel_config.bitmask)))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "[RRC_RRM_HO_UE_ADMISSION_RESP] In SRB UL Specific "
                    " parameters, explicit SRB UL specific parameters"
                    " are not present. Applying default values for it.");
            p_uecc_ue_context->srb1_config.bitmask =
                (rrc_bitmask_t)(p_uecc_ue_context->srb1_config.bitmask &
                                ~RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT);
        }

        /* SPR 14984 Fix Start */
        /* Apply default configuration */
        uecc_defaults_apply_srb_config(
                &p_uecc_ue_context->srb1_config, &srb1_config_default, 
                p_uecc_ue_context, RRC_SRB1_LC_ID);
        /* SPR 14984 Fix Stop */

        /* Store SRB2 configuration if present */
        p_uecc_ue_context->srb2_config =
            p_rrc_rrm_ho_adm_resp->radio_resource_config.srb_info.srb2_config;

        p_uecc_ue_context->m.srb2_config_present = 1;
        p_uecc_ue_context->m.srb2_config_updated = 1;

        /* Apply default configuration */
        if((RRM_SRB_CONFIG_AM_CONFIG_PRESENT & 
                    p_uecc_ue_context->srb2_config.bitmask) &&
                (!(RRM_SRB_AM_CONFIG_EXPLICIT_PRESENT 
                   & p_uecc_ue_context->srb2_config.srb_am_config.bitmask)))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_HO_UE_ADMISSION_RESP] In SRB AM Config, "
                    "explicit AM config is not present."
                    " Applying default values for it.");
            p_uecc_ue_context->srb2_config.bitmask =
                (rrc_bitmask_t)(p_uecc_ue_context->srb2_config.bitmask &
                                ~RRM_SRB_CONFIG_AM_CONFIG_PRESENT);
        }

        if((RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT & 
                    p_uecc_ue_context->srb2_config.bitmask) &&
                (!(RRM_UL_SPECIFIC_PARAMETERS_EXPLICIT_PRESENT 
                   & p_uecc_ue_context->srb2_config.logical_channel_config.
                   bitmask)))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_HO_UE_ADMISSION_RESP] In SRB UL Specific "
                    "parameters, explicit SRB UL specific parameters"
                    " are not present. Applying default values for it.");
            p_uecc_ue_context->srb2_config.bitmask =
                (rrc_bitmask_t)(p_uecc_ue_context->srb2_config.bitmask &
                                ~RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT);
        }

        /* SPR 14984 Fix Start */
        uecc_defaults_apply_srb_config(
                &p_uecc_ue_context->srb2_config, &srb2_config_default, 
                p_uecc_ue_context, RRC_SRB2_LC_ID);
        /* SPR 14984 Fix Stop */


        /*DRB to add list */
/*        p_uecc_ue_context->e_rab_list_count = 
            p_rrc_rrm_ho_adm_resp->radio_resource_config.drb_to_add_list.
            drb_count;
*/
        p_uecc_ue_context->e_rab_list_count = RRC_NULL; 
        
        for (drb_count = 0; drb_count < p_drb_to_add_list->drb_count;
                drb_count++)
        {
            if (!p_uecc_ue_context->p_e_rab_list
                [p_drb_to_add_list->drb_config[drb_count].
                erab_id])
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "e_rab context NULL at index %d\n",
                        p_drb_to_add_list->drb_config[drb_count].
                        erab_id);
                continue;
            }
            p_uecc_ue_context->e_rab_list_count++;
            p_uecc_ue_context->p_e_rab_list
                [p_drb_to_add_list->drb_config[drb_count].
                erab_id]->drb_config = 
                p_drb_to_add_list->drb_config[drb_count];
        }

       /*Update DRB failed to add list */
        if ( RADIO_RESOURCE_CONFIG_DRB_FAIL_TO_ADD_LIST_PRESENT &
                p_rrc_rrm_ho_adm_resp->radio_resource_config.bitmask )
        {
            p_uecc_e_rab_failed_to_add_list = 
                &p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
            s1_trg_ho_info.erab_failed_list;

            p_rrm_drb_failed_to_add_info_list = 
                &p_rrc_rrm_ho_adm_resp->radio_resource_config.
                drb_failed_to_add_list;

            for (e_rab_count = 0; e_rab_count < 
                    p_rrm_drb_failed_to_add_info_list->count; 
                    e_rab_count++)
            {
                /*Erab id */
                p_uecc_e_rab_failed_to_add_list->
                    e_rab_failed_to_add_list[p_uecc_e_rab_failed_to_add_list
                    ->count].e_rab_id = p_rrm_drb_failed_to_add_info_list->
                    drb_failed_to_add[e_rab_count].erab_id;
                /*cause */
        /*BUG 604 changes start*/
                if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) ||
                    (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
                    (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
        /*BUG 604 changes stop*/
                {
                    if( RRC_SUCCESS !=  fill_x2ap_cause(p_uecc_ue_context->p_gb_context,(&p_uecc_e_rab_failed_to_add_list->
                                e_rab_failed_to_add_list
                                [p_uecc_e_rab_failed_to_add_list->count].
                                x2ap_cause),
                            (&p_rrm_drb_failed_to_add_info_list->drb_failed_to_add
                             [e_rab_count].cause)))
                    {
                        /* SPR_7888_fix */
                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING, "[%s] Invalid Cause received from RRM!!",  
                                                  __FUNCTION__);
                        return RRC_FAILURE;
                        /* SPR_7888_fix */
                    }
                    
                }
                else
                {
                    /* SPR_7888_fix */
                    if(RRC_SUCCESS != convert_rrm_cause_to_s1ap_cause(&p_rrm_drb_failed_to_add_info_list->
                                                         drb_failed_to_add[e_rab_count].cause,
                                            &p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list[
                                                     p_uecc_e_rab_failed_to_add_list->count].cause))
                    {
                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING, "[%s] Invalid Cause received from RRM!!",
                                                __FUNCTION__);
                        return RRC_FAILURE;
                    }
                    /* SPR_7888_fix */
                }

                /* Free the corresponding UECC UE ERAB context*/
                if (PNULL == p_uecc_ue_context->p_e_rab_list[
                        p_rrm_drb_failed_to_add_info_list->
                        drb_failed_to_add[e_rab_count].erab_id])
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                            "[%s] p_e_rab_list null at idx %d\n",
                            __FUNCTION__,
                            p_rrm_drb_failed_to_add_info_list->
                            drb_failed_to_add[e_rab_count].erab_id);
                    continue;
                }

                if (PNULL != p_uecc_ue_context->p_e_rab_list[
                        p_rrm_drb_failed_to_add_info_list->
                        drb_failed_to_add[e_rab_count].erab_id])
                {
		            /*SPR 15086 Fix Start */
		            p_uecc_e_rab_failed_to_add_list->
		             e_rab_failed_to_add_list[p_uecc_e_rab_failed_to_add_list->count].drb_id =
		                 p_uecc_ue_context->p_e_rab_list[p_rrm_drb_failed_to_add_info_list->
                           drb_failed_to_add[e_rab_count].erab_id]->drb_config.drb_id;
		            /*SPR 15086 Fix Stop */
                    rrc_mem_free(p_uecc_ue_context->p_e_rab_list[
                            p_rrm_drb_failed_to_add_info_list->
                            drb_failed_to_add[e_rab_count].erab_id]);
                    p_uecc_ue_context->p_e_rab_list[
                        p_rrm_drb_failed_to_add_info_list->
                        drb_failed_to_add[e_rab_count].erab_id] = PNULL;
                }

                p_uecc_e_rab_failed_to_add_list->count++;
            }

            /* Update the last index of failed list as received from RRM 
             * This would be required during RRM HO UE ADM CNF */
            p_uecc_e_rab_failed_to_add_list->llim_index = 
                p_uecc_e_rab_failed_to_add_list->count;

        }

        if (RRC_FAILURE == 
                uecc_fsm_trg_ho_is_atleast_one_non_gbr_bearer_present(
                    p_uecc_ue_context))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s] No non GBR bearers left\n!!",
                    __FUNCTION__);
            break;
        }

        if (RRC_SUCCESS != uecc_fsm_trg_ho_process_radio_resource_cfg_params(
                               p_uecc_ue_context, p_rrc_rrm_ho_adm_resp))
        {
            break;
        }

        /* Generate integrity/security keys */
        rrc_generate_key(
                K_RRC_INT_ALG_P1,
                (U8)p_uecc_ue_context->
                configure_integrity_protection.algorithm_id,
                &p_uecc_ue_context->configure_integrity_protection.key[0],
                &p_uecc_ue_context->security_key.data[0]);

        rrc_generate_key(
                K_RRC_ENC_ALG_P1,
                (U8)p_uecc_ue_context->configure_ciphering.algorithm_id,
                &p_uecc_ue_context->configure_ciphering.key[0],
                &p_uecc_ue_context->security_key.data[0]);

        rrc_generate_key(
                K_UP_ENC_ALG_P1,
                (U8)p_uecc_ue_context->configure_ciphering_user_plane.
                algorithm_id,
                &p_uecc_ue_context->configure_ciphering_user_plane.key[0],
                &p_uecc_ue_context->security_key.data[0]);

/*SPR 13469 Fix Start*/
    	if (p_rrc_rrm_ho_adm_resp->bitmask &
	   	RRC_RRM_UE_HO_ADM_RESP_FULL_CONFIG_PRESENT)
    	{
		p_uecc_ue_context->m.full_config_present = 1;
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            		"Full Config bitmask set in UE Context ");
    	}
/*SPR 13469 Fix Stop*/
        result = RRC_SUCCESS;
            /*SPR_17143_START*/
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "SRB Key Generation Inputs at TeNB during HO: "
                    "Integrity Algo Id[%u] ciphering algo[%u] "
                    "security_key size[%u] key  - "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]",
                    p_uecc_ue_context->configure_integrity_protection.algorithm_id,
                    p_uecc_ue_context->configure_ciphering.algorithm_id,
                    p_uecc_ue_context->security_key.numbits,
                    p_uecc_ue_context->security_key.data[0],
                    p_uecc_ue_context->security_key.data[1],
                    p_uecc_ue_context->security_key.data[2],
                    p_uecc_ue_context->security_key.data[3],
                    p_uecc_ue_context->security_key.data[4],
                    p_uecc_ue_context->security_key.data[5],
                    p_uecc_ue_context->security_key.data[6],
                    p_uecc_ue_context->security_key.data[7],
                    p_uecc_ue_context->security_key.data[8],
                    p_uecc_ue_context->security_key.data[9],
                    p_uecc_ue_context->security_key.data[10],
                    p_uecc_ue_context->security_key.data[11],
                    p_uecc_ue_context->security_key.data[12],
                    p_uecc_ue_context->security_key.data[13],
                    p_uecc_ue_context->security_key.data[14],
                    p_uecc_ue_context->security_key.data[15],
                    p_uecc_ue_context->security_key.data[16],
                    p_uecc_ue_context->security_key.data[17],
                    p_uecc_ue_context->security_key.data[18],
                    p_uecc_ue_context->security_key.data[19],
                    p_uecc_ue_context->security_key.data[20],
                    p_uecc_ue_context->security_key.data[21],
                    p_uecc_ue_context->security_key.data[22],
                    p_uecc_ue_context->security_key.data[23],
                    p_uecc_ue_context->security_key.data[24],
                    p_uecc_ue_context->security_key.data[25],
                    p_uecc_ue_context->security_key.data[26],
                    p_uecc_ue_context->security_key.data[27],
                    p_uecc_ue_context->security_key.data[28],
                    p_uecc_ue_context->security_key.data[29],
                    p_uecc_ue_context->security_key.data[30],
                    p_uecc_ue_context->security_key.data[31]);

            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "SRB Integrity Key Data: "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]", 
                    p_uecc_ue_context->configure_integrity_protection.key[0],
                    p_uecc_ue_context->configure_integrity_protection.key[1],
                    p_uecc_ue_context->configure_integrity_protection.key[2],
                    p_uecc_ue_context->configure_integrity_protection.key[3],
                    p_uecc_ue_context->configure_integrity_protection.key[4],
                    p_uecc_ue_context->configure_integrity_protection.key[5],
                    p_uecc_ue_context->configure_integrity_protection.key[6],
                    p_uecc_ue_context->configure_integrity_protection.key[7],
                    p_uecc_ue_context->configure_integrity_protection.key[8],
                    p_uecc_ue_context->configure_integrity_protection.key[9],
                    p_uecc_ue_context->configure_integrity_protection.key[10],
                    p_uecc_ue_context->configure_integrity_protection.key[11],
                    p_uecc_ue_context->configure_integrity_protection.key[12],
                    p_uecc_ue_context->configure_integrity_protection.key[13],
                    p_uecc_ue_context->configure_integrity_protection.key[14],
                    p_uecc_ue_context->configure_integrity_protection.key[15]);

            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "SRB Ciphering Key Data: "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]", 
                    p_uecc_ue_context->configure_ciphering.key[0],
                    p_uecc_ue_context->configure_ciphering.key[1],
                    p_uecc_ue_context->configure_ciphering.key[2],
                    p_uecc_ue_context->configure_ciphering.key[3],
                    p_uecc_ue_context->configure_ciphering.key[4],
                    p_uecc_ue_context->configure_ciphering.key[5],
                    p_uecc_ue_context->configure_ciphering.key[6],
                    p_uecc_ue_context->configure_ciphering.key[7],
                    p_uecc_ue_context->configure_ciphering.key[8],
                    p_uecc_ue_context->configure_ciphering.key[9],
                    p_uecc_ue_context->configure_ciphering.key[10],
                    p_uecc_ue_context->configure_ciphering.key[11],
                    p_uecc_ue_context->configure_ciphering.key[12],
                    p_uecc_ue_context->configure_ciphering.key[13],
                    p_uecc_ue_context->configure_ciphering.key[14],
                    p_uecc_ue_context->configure_ciphering.key[15]);

            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "User Plane Ciphering Key Data: "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]", 
                    p_uecc_ue_context->configure_ciphering_user_plane.key[0],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[1],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[2],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[3],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[4],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[5],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[6],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[7],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[8],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[9],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[10],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[11],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[12],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[13],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[14],
                    p_uecc_ue_context->configure_ciphering_user_plane.key[15]);
            /*SPR_17143_END*/
        }
    }while (0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_create_ue_entity_ongoing_llim_create_ue_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is called upon receiving uecc_llim_create_ue_entity_res
                    in "UECC_TRG_HO_CREATE_UE_ENTITY_ONGOING" state.
 ****************************************************************************/
void uecc_fsm_trg_ho_create_ue_entity_ongoing_llim_create_ue_resp_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api
        )
{
    rrc_return_t                           result = RRC_FAILURE;
    rrc_uecc_llim_create_ue_entity_resp_t* p_llim_create_ue_entity_resp = PNULL;
    uecc_fsm_fail_handler_f const*         p_failure_actions_list =
        uecc_fsm_ue_trg_ho_fail_type2_actions; 
    /* Bug 817 Fix Start */
    U8 drb_count = RRC_NULL;
    /* Bug 817 Fix Stop */

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received [UECC_EV_LLIM_CREATE_UE_ENTITY_RESP] ");
    do {
        p_llim_create_ue_entity_resp =
            (rrc_uecc_llim_create_ue_entity_resp_t*)p_api;

        if ( RRC_LLIM_RESPONSE_CODE_LAST <=
                p_llim_create_ue_entity_resp->response_code )
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_EV_LLIM_CREATE_UE_ENTITY_RESP] Invalid Response=%i",
                    p_llim_create_ue_entity_resp->response_code);
            break;
        }

        /* Bug 817 Fix Start */
        memset_wrapper (&p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info, RRC_NULL, 
                       sizeof(rrc_uecc_fwd_tunnel_info_t));
        /* Bug 817 Fix Stop */

        /* AG_11 - Start*/
        /* Process uecc_llim_create_ue_entity_resp */
        if (RRC_FAILURE == 
                uecc_fsm_trg_ho_llim_process_create_ue_entity_response
                (p_uecc_ue_context,
                 p_llim_create_ue_entity_resp))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s] uecc_fsm_trg_ho_llim_process_"
                    "create_ue_entity_response faild ",
                    __FUNCTION__);
            p_failure_actions_list = uecc_fsm_ue_trg_ho_fail_type6_actions;

            break;
        }

        /*SPR_17121_START*/
        if(p_llim_create_ue_entity_resp->bitmask & 
                UECC_LLIM_CREATE_UE_ENTITY_RESP_MACI_PRESENT)
        {
            p_uecc_ue_context->mac_i = p_llim_create_ue_entity_resp->mac_i.mac_i; 
        }
        else
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s] uecc_fsm_trg_ho_llim_process_"
                    "MACI not recieved in create_ue_entity_resp",
                    __FUNCTION__);
            p_failure_actions_list = uecc_fsm_ue_trg_ho_fail_type6_actions;

            break;
        }
        /*SPR_17121_END*/

        
        /* AG_11 - End*/
        if ( RRC_LLIM_SUCCESS == p_llim_create_ue_entity_resp->response_code ||
               RRC_LLIM_UE_ALREADY_EXIST == p_llim_create_ue_entity_resp->response_code )
        {
            p_failure_actions_list = uecc_fsm_ue_trg_ho_fail_type6_actions;

            /* SPS Start */
            /* SPR 20747 Fix Start */  
            if ((p_uecc_ue_context->m.is_sps_recv_frm_rrm) &&
                  (RRC_NULL == p_uecc_ue_context->m.is_sps_setup_or_rel_recv))
            /* SPR 20747 Fix End */  
            {
                if (p_llim_create_ue_entity_resp->bitmask &
                        UECC_LLIM_CREATE_SRB_ENTITY_SPS_CRNTI_PRESENT)
                {
                    /* Copy SPS CRNTI into UE Context */
                    rrc_pack_U16(p_uecc_ue_context->p_sps_config->semi_presist_sched_c_rnti,
                            &p_llim_create_ue_entity_resp->sps_crnti);
                    p_uecc_ue_context->p_sps_config->bitmask |= RRC_SPS_CONFIG_C_RNTI_PRESENT;

                }
                else
                {
                    p_uecc_ue_context->m.sps_config_updated = RRC_NULL; /* Reset to 0 */
                    if (p_uecc_ue_context->p_sps_config != PNULL)
                    {
                        rrc_mem_free(p_uecc_ue_context->p_sps_config);
                        p_uecc_ue_context->p_sps_config = PNULL;
                        p_uecc_ue_context->m.sps_config_present = 0;
                    }
                }
            }

            /* SPS Stop */

            /* For X2AP or INTER_CELL Handover*/
        /*BUG 604 changes start*/
            if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
        /*BUG 604 changes stop*/
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                        "Sending X2AP HO REQUEST ACK ");
                /* Send HANDOVER REQUEST ACK to source eNB */
                if (RRC_FAILURE == 
                        uecc_x2ap_build_and_send_handover_request_ack(
                            p_uecc_ue_context,
                            PNULL))
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                            "[%s] uecc_x2ap_build_and_send"
                            "_handover_request_ack faild ",
                            __FUNCTION__);

                    break;
                }
            }
            /* For S1AP Handover*/
            else
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                        "Sending S1AP HO REQUEST ACK ");
                /*send HANDOVER REQUEST ACK to MME */

                if (RRC_FAILURE == 
                        uecc_s1ap_build_and_send_handover_request_ack(
                            p_uecc_ue_context,
                            PNULL))
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                            "[%s] uecc_s1ap_build_and_send"
                            "_handover_request_ack faild ",
                            __FUNCTION__);

                    break;
                }
            }
            /* Set flag Handover_Request_Ack Sent in UE Context */
            if ( PNULL != p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info )
            {
                p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                    is_ho_req_ack_sent = RRC_TRUE;
            } 
    
/*SPR_16942_START*/
        /* Update DRX and PHR config for RELEASE in MAC Main Config*/
        if(p_uecc_ue_context->m.mac_config_present == 1)
        {
            if((p_uecc_ue_context->mac_config.bitmask & RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT) &&
                    (p_uecc_ue_context->mac_config.mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT) &&
                    (p_uecc_ue_context->mac_config.mac_main_config.drx_config.drx_config_type == RRC_DRX_CONFIG_TYPE_REL))
            {
                p_uecc_ue_context->mac_config.mac_main_config.bitmask &= ~(RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT);
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "DRX Config Released");
            }
            if((p_uecc_ue_context->mac_config.bitmask & RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT) &&
                    (p_uecc_ue_context->mac_config.mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT) &&
                    (!(p_uecc_ue_context->mac_config.mac_main_config.phr_config.bitmask & RRC_PHR_CONFIG_PARAM_PRESENT)))
            {
                p_uecc_ue_context->mac_config.mac_main_config.bitmask &= ~(RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT);
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "PHR Config Released");
            }
        }
/*SPR_16942_END*/


            /* for inter-RAT, MME Status Transfer will not come from MME */
            if((HANDOVER_TYPE_UTRAN_TO_LTE == p_uecc_ue_context->ho_info.p_ho_info->ho_type)||
                    (HANDOVER_TYPE_GERAN_TO_LTE == p_uecc_ue_context->ho_info.p_ho_info->ho_type))
            {
                /* change the state to 
                   UECC_TRG_HO_W_FOR_RRC_CONN_RECONFIG_COMPLETE*/

                UECC_FSM_SET_CHILD_STATE(
                        p_uecc_ue_context,
                        &uecc_fsm_trg_ho_w_for_rrc_conn_reconfig_complete, 
                        PNULL);
                result = RRC_SUCCESS;
            }
/*BUG 604 changes start*/
            else if(HANDOVER_TYPE_INTRA_LTE_S1 == 
                    p_uecc_ue_context->ho_info.p_ho_info->ho_type ||
                    X2_HO == 
                    p_uecc_ue_context->ho_info.s1_or_x2_handover ||
                    INTER_CELL_HO == 
                    p_uecc_ue_context->ho_info.s1_or_x2_handover||
                    INTRA_CELL_HO ==
                    p_uecc_ue_context->ho_info.s1_or_x2_handover)
/*BUG 604 changes stop*/
            {
                /*If at least one RLC-AM mode DRB present, change the state to
                  UECC_TRG_HO_WAIT_FOR_MME_STATUS_TRANSFER */
                if ( RRC_SUCCESS == 
                        uecc_fsm_trg_ho_is_atleast_one_rlc_am_mode_drb_present(
                            p_uecc_ue_context))

                {
                    UECC_FSM_SET_CHILD_STATE(
                            p_uecc_ue_context,
                            &uecc_fsm_trg_ho_w_for_mme_status_transfer, PNULL);
                }
                else
                {
                    /*Else change the state to 
                      UECC_TRG_HO_W_FOR_RRC_CONN_RECONFIG_COMPLETE*/

                    UECC_FSM_SET_CHILD_STATE(
                            p_uecc_ue_context,
                            &uecc_fsm_trg_ho_w_for_rrc_conn_reconfig_complete, 
                            PNULL);
                }
                result = RRC_SUCCESS;
            }
            else
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,    \
                        "[UECC_EV_LLIM_CREATE_UE_ENTITY_RESP] " \
                        "invalid Ho_Type [%i]", \
                        p_uecc_ue_context->ho_info.p_ho_info->ho_type);
            }
        }
        else
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[UECC_EV_LLIM_CREATE_UE_ENTITY_RESP] "
                    "Create UE failed at LL [%i]",
                    p_llim_create_ue_entity_resp->response_code);

            if (RRC_LLIM_CREATE_UE_FAIL_PHY == 
                    p_llim_create_ue_entity_resp->response_code)
            {
                p_failure_actions_list = uecc_fsm_ue_trg_ho_fail_type3_actions;
            }

            if ( RRC_TRUE == p_llim_create_ue_entity_resp->scell_addition_failed )
            {
                p_uecc_ue_context->m.scell_add_fail_at_lower_layer = RRC_TRUE;
            }
        }
    }while(0);

#ifdef RRC_DEBUG
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,"[%s]",__FUNCTION__);
    trg_ho_debug_print_erab_info(p_uecc_ue_context);
#endif

    /* Process failure */
    if (RRC_SUCCESS != result)
    {
        /* Bug 817 Fix Start */
        /* Free Forwarding tunnel information */
        if (p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
                drb_tunnel_info_count > RRC_NULL)
        {
            for (drb_count = 0; drb_count < MAX_ERAB_COUNT; drb_count++)
            {
                if (PNULL != p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
                        p_uecc_drb_fwd_tnl_list[drb_count])
                {
                    rrc_mem_free (p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
                            p_uecc_drb_fwd_tnl_list[drb_count]);
                    p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
                        p_uecc_drb_fwd_tnl_list[drb_count] = PNULL;
                }
            }
            p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.drb_tunnel_info_count = 0;
        }
        /* Bug 817 Fix Stop */

        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_uecc_ue_context,
                p_failure_actions_list);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_create_ue_entity_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is HO procedural timer expiry handler for State 
                    "UECC_TRG_HO_MAC_RACH_RES_GET_ONGOING"
 ****************************************************************************/
void uecc_fsm_trg_ho_create_ue_entity_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);
/*BUG 604 changes start*/
    if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        /* criticality diagnostics to be filled
           during error indication */
        /*HO:error_ind:start*/
        uecc_x2ap_build_and_send_handover_prep_failure(
                p_uecc_ue_context,
                (x2ap_CriticalityDiagnostics*)PNULL);
    }
    else
    {
        /* SPR 16149 Fix Start*/ 
        uecc_s1ap_build_and_send_handover_failure(
                p_uecc_ue_context->p_gb_context,
                p_uecc_ue_context,
                p_uecc_ue_context->s1ap_ue_id,
                p_uecc_ue_context->mme_ue_id,
                p_uecc_ue_context->mme_id,
                &p_uecc_ue_context->ho_info.cause,
                (rrc_s1ap_error_indication_t *)PNULL);
        /* SPR 16149 Fix Stop*/ 
    }

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    UECC_FSM_SET_CHILD_STATE(
            p_uecc_ue_context,
            &uecc_fsm_trg_ho_delete_ue_entity_ongoing, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ue_rel_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_UE_RELEASE_REQ message.
 ****************************************************************************/
void uecc_fsm_trg_ue_rel_req_handler( 
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    rrc_s1ap_ue_context_release_command_t *p_ue_context_release_command =
        (rrc_s1ap_ue_context_release_command_t*)p_api;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
/*BUG 604 changes start*/
        /* Procedural timer stopped in finish_failure */

        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "[%s] Toggling rrc_connection_release_not_required_flag",
                __FUNCTION__);
        if(RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
                   p_trg_ho_info->is_rrc_conn_reconf_comp_recv)
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_FALSE;
        }
        else
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_TRUE;
        }
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

        /*RRC_RRM_HO_ADM_CNF to RRM*/
        uecc_rrm_build_and_send_ue_ho_adm_cnf(
                p_uecc_ue_context,
                RRC_FAILURE);

        /*spr_22003_changes_start*/
        /*As HO admission has been failed at target, so no need to increment the
         * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
        p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
        /*spr_22003_changes_end*/

        /*Delete UE context at X2AP*/
        if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
        {
            uecc_fsm_activity_state_t* p_current_state = PNULL;

            p_current_state=p_uecc_ue_context->uecc_fsm.proc_descriptor.p_current_state;

            if(!((p_current_state == &uecc_fsm_trg_ho_mac_rach_res_get_ongoing)||
                        (p_current_state == &uecc_fsm_trg_ho_create_ue_entity_ongoing)))
            {
                uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
            }
        }
/*BUG 604 changes stop*/

    p_uecc_ue_context->share_data.uecc_rcr_data.u.cause =
        p_ue_context_release_command->cause;

    /* notify UE FSM about ue ctx delete */
    /* Post event to UE FSM for future processing */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_CTX_REL_CMD_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
 
/* SPR 22500 Fix - Starts */
/****************************************************************************
 * Function Name  : uecc_fsm_trg_ue_re_establish_req_before_recofig_complete_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ message before RRC 
 *                  Connection Reconfiguration Complete message.
 ****************************************************************************/
void uecc_fsm_trg_ue_re_establish_req_before_recofig_complete_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRCConnectionReestablishmentRequest_r8_IEs*  
        p_rrcConnectionReestablishmentRequest_r8 = PNULL;
    s1ap_Cause    cause;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    p_rrcConnectionReestablishmentRequest_r8 =
        (RRCConnectionReestablishmentRequest_r8_IEs*)p_api;

    /* If RRC Connection Reestablishment Request received with otherFailure 
     * cause and RRC Connection Reconfiguration complete not received, then
     * Send RRC Connection Reestablishment Reject & start Failure handling */
    if (((PNULL != p_uecc_ue_context->ho_info.p_ho_info) &&
         (PNULL != p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info) &&
         (RRC_TRUE != p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                 is_rrc_conn_reconf_comp_recv)) &&
        (otherFailure ==
             p_rrcConnectionReestablishmentRequest_r8->reestablishmentCause))
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, 
            "[RRC Connection Reestablishment Req]:otherFailure is received,"
            " before RRC Connection Reconfiguration Complete");

        if (RRC_SUCCESS !=
            uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                p_uecc_ue_context->p_gb_context, p_uecc_ue_context->crnti,
                p_uecc_ue_context->reestablishmentCause,
                p_uecc_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                "Fail to send the RRCConnectionReestablishmentReject to UE");
        }

        /* Change rrc_connection_release_not_required_flag */
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "[%s] Set rrc_connection_release_not_required_flag", __FUNCTION__);
        p_uecc_ue_context->share_data.uecc_rcr_data.
            rrc_connection_release_not_required_flag = RRC_TRUE;

        /* Send RRC RRM HO ADM CNF Failure */
        uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf(p_uecc_ue_context,
            (rrc_bool_et*)RRC_TRUE);

        if (S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
        {
            /* Fill Ue Release cause */
            cause.t= T_s1ap_Cause_radioNetwork;
            cause.u.radioNetwork = s1ap_unspecified_2;

            /*Send UE CONTEXT RELEASE REQUEST message to S1AP */
            if(RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                        p_uecc_ue_context, &cause))
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[%s] Fail to Send UE CONTEXT RELEASE REQUEST "
                    "message to S1AP.", __FUNCTION__);
            }

            /* Finish child FSM by posting
             * UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT event to main FSM. */
            uecc_fsm_finish_activity_failure(p_uecc_ue_context,
                (void*)UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT);
        }
        else if (X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
        {
            SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
            /*Delete UE context at X2AP*/
            uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);

            /* Finish child FSM by posting
             * UECC_EV_TRG_HO_RELEASE_REQ_INT event to main FSM. */
            uecc_fsm_finish_activity_failure(p_uecc_ue_context,
                (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
        }
        else if ((INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
        {
            SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
            /* Finish child FSM by posting
             * UECC_EV_TRG_HO_RELEASE_REQ_INT event to main FSM. */
            uecc_fsm_finish_activity_failure(p_uecc_ue_context,
                (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
        }
    }
    else
    {
        /* Call the Regular routine function to handle Reestablishment with
         * Cause other than otherFailure */
        uecc_fsm_trg_ue_re_establish_req_handler(p_uecc_ue_context, p_api);
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/* SPR 22500 Fix - Ends */

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ue_re_establish_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ message.
 ****************************************************************************/
void uecc_fsm_trg_ue_re_establish_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRCConnectionReestablishmentRequest_r8_IEs*  
        p_rrcConnectionReestablishmentRequest_r8 = PNULL;
    U16     len = RRC_NULL;
    s1ap_Cause    cause;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    p_rrcConnectionReestablishmentRequest_r8 =
        (RRCConnectionReestablishmentRequest_r8_IEs*)p_api;

    /*BUG 604 changes start*/
    /* If Reestablishment Cause is Reconfiguration Failure then
     * take failure action */
    if (reconfigurationFailure == 
            p_rrcConnectionReestablishmentRequest_r8->reestablishmentCause)
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "[RRC Connection Reestablishment Req]"
                ":reconfiguration failure is received");

        if (RRC_SUCCESS !=
                uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                    p_uecc_ue_context->p_gb_context, p_uecc_ue_context->crnti,
                    p_uecc_ue_context->reestablishmentCause,
                    p_uecc_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                    "Fail to send the RRCConnectionReestablishmentReject to UE");
        }
        /* Change rrc_connection_release_not_required_flag */
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "[%s] Toggling rrc_connection_release_not_required_flag",
                __FUNCTION__);
        if(RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
                p_trg_ho_info->is_rrc_conn_reconf_comp_recv)
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_FALSE;
        }
        else
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_TRUE;
        }

        uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf(
                p_uecc_ue_context, (rrc_bool_et*)RRC_TRUE);

        if ( (S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) ||
                (RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
                 p_trg_ho_info->is_path_switch_req_ack_recv) )
        {
            /* Fill Ue Release cause */
            cause.t= T_s1ap_Cause_radioNetwork;
            cause.u.radioNetwork = s1ap_unspecified_2;

            /*Send UE CONTEXT RELEASE REQUEST message to S1AP */
            if(RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                        p_uecc_ue_context,
                        &cause))
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,
                        RRC_ERROR,
                        "[%s] Fail to Send UE CONTEXT RELEASE REQUEST message to S1AP." 
                        ,__FUNCTION__);
            }

            /*Finish child FSM by posting
             * UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT event to main FSM. */
            uecc_fsm_finish_activity_failure(
                    p_uecc_ue_context, (void*)UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT);
        }
        else if (X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
        {
            SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
            /*Delete UE context at X2AP*/
            uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);

            /*Finish child FSM by posting
              UECC_EV_TRG_HO_RELEASE_REQ_INT event to main FSM. */
            uecc_fsm_finish_activity_failure(
                    p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
        }
        /*BUG 604 changes start*/
        else if ((INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
        /*BUG 604 changes stop*/
        {
            SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
            /*Finish child FSM by posting
              UECC_EV_TRG_HO_RELEASE_REQ_INT event to main FSM. */
            uecc_fsm_finish_activity_failure(
                    p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
        }
    }
    else
    {
        /* Stop timer */
        uecc_ue_timer_stop(p_uecc_ue_context->p_gb_context,
                UECC_UE_TRG_HO_GUARD_TIMER, 
                &p_uecc_ue_context->timers_data);

        /*Save the ongoing UE FSM Context to restore it back after
         *high priority re-establishment procedure has completed 
         */
        uecc_fsm_store_ongoing_active_procedure(p_uecc_ue_context);

        /* Finish active procedure descriptor */
        uecc_fsm_finish_active_procedure(p_uecc_ue_context);

        /* Set flag */
        p_uecc_ue_context->trg_ho_fail_rrc_cre_flag = RRC_TRUE;

        /* Memory allocated to store RRCConnectionReestablishmentReq msg */
        p_uecc_ue_context->p_curr_proc_data = (curr_proc_data_t *) 
            rrc_mem_get(sizeof(curr_proc_data_t));

        if (PNULL != p_uecc_ue_context->p_curr_proc_data)
        {
            len = (U16)(sizeof(RRCConnectionReestablishmentRequest_r8_IEs));
            /* Buffered Received Re-establishment Msg */
            if (PNULL == (p_uecc_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
                        rrc_mem_get(len)))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                        "Memory Allocation Failed to store"
                        "Reestablishment Req msg");

                rrc_mem_free(p_uecc_ue_context->p_curr_proc_data);
                p_uecc_ue_context->p_curr_proc_data = PNULL;
                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                return;
            }
        }
        else
        {
            /* Memory Allocation Failed */
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL, 
                    "Memory Allocation Failure");     
            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
            return;
        }
        /* Storing re_establishment contents into msg_bfr_rcvd */
        l3_memcpy_wrapper(p_uecc_ue_context->p_curr_proc_data->msg_bfr_rcvd,
                p_api, len);

        /* Fill received message buffer type */
        p_uecc_ue_context->p_curr_proc_data->msg_bfr_type = 
            RRC_RE_ESTABLISH_MSG;

        uecc_event_queue_push_event(
                UECC_EV_RRC_RE_ESTABLISH_REQ_INT,
                p_uecc_ue_context,
                p_uecc_ue_context->p_curr_proc_data->msg_bfr_rcvd);
    }
    /*BUG 604 changes stop*/
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_error_indication_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_UE_RELEASE_REQ message.
 ****************************************************************************/
void uecc_fsm_trg_error_indication_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api
        )
{
    /*HO:error_ind:start*/
    rrc_s1ap_error_indication_t *p_error_indication =
        (rrc_s1ap_error_indication_t*)p_api;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    if((INTRA_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover)||
         (INTER_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover))
    { 
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        return;
    }
    
    /* Procedural timer stopped in finish_failure */

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
        "[%s] Toggling rrc_connection_release_not_required_flag",
        __FUNCTION__);
    if(RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
            p_trg_ho_info->is_rrc_conn_reconf_comp_recv)
    {
        p_uecc_ue_context->share_data.uecc_rcr_data.
            rrc_connection_release_not_required_flag = RRC_FALSE;
    }
    else
    {
        p_uecc_ue_context->share_data.uecc_rcr_data.
            rrc_connection_release_not_required_flag = RRC_TRUE;
    }
    if (RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT &
           p_error_indication->bitmask)
    {
        p_uecc_ue_context->share_data.uecc_rcr_data.u.cause =
            p_error_indication->cause;
    }
    else
    {
        p_uecc_ue_context->share_data.uecc_rcr_data.u.cause.t = T_s1ap_Cause_protocol;
        p_uecc_ue_context->share_data.uecc_rcr_data.u.cause.u.protocol = s1ap_unspecified_1;
    }

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    /* notify UE FSM about ue ctx delete
     * */
    /* Post event to UE FSM for future
     * processing */
    uecc_fsm_finish_activity_failure(
            p_uecc_ue_context, (void*)UECC_EV_TRG_HO_CTX_REL_CMD_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_x2ap_error_indication_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_UE_RELEASE_REQ message.
 ****************************************************************************/
void uecc_fsm_trg_x2ap_error_indication_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api
        )
{

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    if((INTRA_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover)||
       (INTER_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover))
    { 
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        return;
    }
    
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
        "[%s] Toggling rrc_connection_release_not_required_flag",
        __FUNCTION__);

    if(RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
            p_trg_ho_info->is_rrc_conn_reconf_comp_recv)
    {
        p_uecc_ue_context->share_data.uecc_rcr_data.
            rrc_connection_release_not_required_flag = RRC_FALSE;
    }
    else
    {
        p_uecc_ue_context->share_data.uecc_rcr_data.
            rrc_connection_release_not_required_flag = RRC_TRUE;
    }
    /* SPR 16750 Fix Start */
    /*Delete UE context at X2AP*/
    if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
    }
    /* SPR 16750 Fix Stop */
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    /* notify UE FSM about ue ctx delete
     * */
    /* Post event to UE FSM for future
     * processing */
    uecc_fsm_finish_activity_failure(
            p_uecc_ue_context, (void*)UECC_EV_TRG_HO_CTX_REL_CMD_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/* SPR 16750 Fix Start */
/****************************************************************************
 * Function Name  : uecc_fsm_trg_rrm_adm_error_indication_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  ERROR_INDICATION message.
 ****************************************************************************/
void uecc_fsm_trg_rrm_adm_error_indication_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api
        )
{

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    if((INTRA_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover)||
       (INTER_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover))
    { 
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        return;
    }
    
    if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
            uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
    }
    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    uecc_fsm_start_failure_processing(
            p_uecc_ue_context,
                uecc_fsm_ue_trg_ho_fail_type11_actions);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_mac_rach_res_error_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  ERROR_INDICATION message.
 ****************************************************************************/
void uecc_fsm_trg_ho_mac_rach_res_error_ind_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api
        )
{

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    if((INTRA_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover)||
       (INTER_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover))
    { 
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        return;
    }
    
    if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
            uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
    }

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    uecc_fsm_start_failure_processing(
            p_uecc_ue_context,
            uecc_fsm_ue_trg_ho_fail_type8_actions);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_create_ue_entity_error_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  ERROR_INDICATION message.
 ****************************************************************************/
void uecc_fsm_trg_ho_create_ue_entity_error_ind_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api
        )
{

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    if((INTRA_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover)||
       (INTER_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover))
    { 
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        return;
    }
    
    if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
    }

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf_failure(
            p_uecc_ue_context);

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    UECC_FSM_SET_CHILD_STATE(p_uecc_ue_context,
                &uecc_fsm_trg_ho_delete_ue_entity_ongoing, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/* SPR 16750 Fix Stop */

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_delete_ue_entity_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
                    UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_trg_ho_delete_ue_entity_ongoing_init_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* Stop timer */
    uecc_ue_timer_stop(p_uecc_ue_context->p_gb_context, UECC_UE_TRG_HO_GUARD_TIMER, 
            &p_uecc_ue_context->timers_data);

    if(RRC_SUCCESS != uecc_ue_timer_start(
            p_uecc_ue_context->p_gb_context, 
            UECC_UE_LL_GUARD_TIMER,
            &p_uecc_ue_context->p_gb_context->ue_timers_data,
            &p_uecc_ue_context->timers_data,
            p_uecc_ue_context->ue_index))
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                "uecc_ue_timer_start failed !!!\n");

        uecc_fsm_start_failure_processing(
                p_uecc_ue_context,
                uecc_fsm_ue_trg_ho_fail_type1_actions);
    }

    /* Send UE entity delete request to LLIM */
    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_delete_ue_entity_req( p_uecc_ue_context ))
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
            "UECC_LLIM_DELETE_UE_ENTITY_REQ delivery fail.");
        /* Switch to the next step
         *  - simulate receiving of UECC_RCR_EVENT_LLIM_DELETE_UE_RESP message
         * Post event to RCR FSM for future processing */
        uecc_event_queue_push_event(
            UECC_EV_LLIM_DELETE_UE_ENTITY_RESP,
            p_uecc_ue_context,
            PNULL);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_path_switch_ongoing_init_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_trg_ho_path_switch_ongoing_init_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api
        )
{
/* Bug 8198 Fix Start */
    U8 procedure_code = RRC_NULL;
/* Bug 8198 Fix End */
    
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILEDALL,"Unused variable in FSM:%p",
            p_api);

/*BUG 604 changes start*/
    if(INTRA_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        if ( (PNULL != p_uecc_ue_context->ho_info.p_ho_info) && 
              (PNULL != p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info) )
        {
            p_uecc_ue_context->ho_info.p_ho_info->\
               p_trg_ho_info->is_path_switch_req_ack_recv = RRC_TRUE;
        }

        if (RRC_SUCCESS != uecc_llim_build_and_send_s1u_path_switch_req(
                    p_uecc_ue_context))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "build_and_send_s1u_path_switch_req"
                    " failed !!");
            uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf(
                    p_uecc_ue_context, (rrc_bool_et *)RRC_TRUE);

            uecc_fsm_finish_activity_failure(
                    p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);

            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
            return;
        }
    }
    else
    {
	    /*BUG 604 changes stop*/
	    /* Bug 8198 Fix Start */
	    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
			    "Sending S1AP Allocate MME Request");
	    procedure_code = ASN1V_s1ap_id_PathSwitchRequest; 
	    if (RRC_SUCCESS != build_and_send_allocate_mme_req(
				    p_uecc_ue_context,procedure_code))
	    {
		    RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
				    "build_and_send_allocate_mme_req"
				    " failed !!");
		    uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf(
				    p_uecc_ue_context, (rrc_bool_et *)RRC_TRUE);

        /* SPR_17847_START*/
        /*Delete UE context at X2AP*/
        if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
        {
            uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
        }
        /* SPR_17847_END*/
	uecc_fsm_finish_activity_failure(
              p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);

		    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
		    return;
	    }/* Bug 8198 Fix End */
/*BUG 604 changes start*/
    }	   
/*BUG 604 changes stop*/

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}



/*SPR_17893_START*/
/*Code Removed*/
/*SPR_17893_END*/
/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_uplink_tunnel_reconfig_init_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void uecc_fsm_trg_ho_uplink_tunnel_reconfig_init_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Sending S1U Reconfig Request for ul switched list\n");

    /* send ul erab to be switched to llim(s1u) */ 
    if (RRC_SUCCESS != 
            uecc_llim_build_and_send_s1u_reconfig_req_for_ul_switched_list(
                p_uecc_ue_context))
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                "[%s]uecc_llim_build_and_send_s1u_reconfig_req_for_ul_switched_list"
                " failed !!",__FUNCTION__);
        uecc_fsm_start_failure_processing(
                p_uecc_ue_context,
                uecc_fsm_ue_trg_ho_fail_type5_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_delete_ue_entity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_trg_ho_delete_ue_entity_resp_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_delete_ue_entity_resp_t * 
        p_rrc_uecc_llim_delete_ue_entity_resp = PNULL;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    /* ARGUSED */
    p_rrc_uecc_llim_delete_ue_entity_resp = 
        (rrc_uecc_llim_delete_ue_entity_resp_t *)p_api;

    if (p_api && 
            p_rrc_uecc_llim_delete_ue_entity_resp->
            response_code == RRC_FAILURE)
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                "[%s]uecc_llim_delete_ue_entity_resp == FAILURE"
                ,__FUNCTION__);
    }
    /* Stop timer */
    uecc_ue_timer_stop(p_uecc_ue_context->p_gb_context, UECC_UE_LL_GUARD_TIMER, 
            &p_uecc_ue_context->timers_data);

    /* notify procedure about failure */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_delete_ue_entity_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_trg_ho_delete_ue_entity_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    /* notify procedure about failure */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_mme_status_tranfer_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_S1AP_MME_STATUS_TRANSFER message.
 ****************************************************************************/
void uecc_fsm_trg_ho_mme_status_tranfer_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);


    /* Send the UECC_LLIM_SN_HFN_STATUS_IND to PDCP
       through LLIM */

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Sending UECC LLIM SN HFN IND\n");

    if (RRC_SUCCESS != uecc_llim_build_and_send_sn_hfn_status_ind(
                p_uecc_ue_context,(rrc_s1ap_mme_status_transfer_t * )p_api))
    {
        /* Change rrc_connection_release_not_required_flag */
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "[%s] Toggling rrc_connection_release_not_required_flag",
            __FUNCTION__);

        if(RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
               p_trg_ho_info->is_rrc_conn_reconf_comp_recv)
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_FALSE;
        }
        else
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_TRUE;
        }
        uecc_fsm_start_failure_processing(
                p_uecc_ue_context,
                uecc_fsm_ue_trg_ho_fail_type5_actions);
    }
    if (RRC_TRUE == 
            p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->is_rrc_conn_reconf_comp_recv)
    {
            /*Send HO NOTIFY message to MME */
            if (RRC_SUCCESS != uecc_s1ap_build_and_send_handover_notify (
                        p_uecc_ue_context))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s]uecc_s1ap_build_and_send_handover_notify"
                        " failed !!",__FUNCTION__);
            }

            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "Sending UECC LLIM DATA BUFFER STOP IND\n");

            /*Send UECC_LLIM_STOP_BUFFER_IND to LLIM*/
            if (RRC_SUCCESS != uecc_llim_build_and_send_data_buffer_stop_ind(
                        p_uecc_ue_context))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s]uecc_llim_build_and_send_data_buffer_stop_ind "
                        "failed !!",__FUNCTION__);
            }

        /*SPR_17893_START*/
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "Sending UECC RRM UE HO ADM CNF");
        /* RRC_RRM_HO_ADM_CNF to RRM.*/
        if (RRC_FAILURE == uecc_rrm_build_and_send_ue_ho_adm_cnf(
                    p_uecc_ue_context,
                    RRC_SUCCESS))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s] uecc_rrm_build_and_send_ue_ho_adm_cnf failed ",
                    __FUNCTION__);
        }
        uecc_fsm_finish_activity_success(p_uecc_ue_context, PNULL);
        /*SPR_17893_END*/
    }
    else
    {
        p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->is_mme_status_transfer_ind_recv = 
            RRC_TRUE;
    }


    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_sn_status_tranfer_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_S1AP_MME_STATUS_TRANSFER message.
 ****************************************************************************/
void uecc_fsm_trg_ho_sn_status_tranfer_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);


    /* Send the UECC_LLIM_SN_HFN_STATUS_IND to PDCP through LLIM */

    if (RRC_SUCCESS != uecc_llim_x2ap_build_and_send_sn_hfn_status_ind(
                p_uecc_ue_context,(rrc_x2ap_sn_status_transfer_t *)p_api))
    {
        /* Change rrc_connection_release_not_required_flag */
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "[%s] Toggling rrc_connection_release_not_required_flag",
            __FUNCTION__);

        if(RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
               p_trg_ho_info->is_rrc_conn_reconf_comp_recv)
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_FALSE;
        }
        else
        {
            p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_TRUE;
        }

        uecc_fsm_start_failure_processing(
                p_uecc_ue_context,
                uecc_fsm_ue_trg_ho_fail_type7_actions);
    }
    if (RRC_TRUE == 
            p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->is_rrc_conn_reconf_comp_recv)
    {
/*SPR_17382_START*/
            /*Send UECC_LLIM_STOP_BUFFER_IND to LLIM*/
            if (RRC_SUCCESS != uecc_llim_build_and_send_data_buffer_stop_ind(
                        p_uecc_ue_context))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s]uecc_llim_build_and_send_data_buffer_stop_ind "
                        "failed !!",__FUNCTION__);
            }
/*SPR_17382_END*/
            /* Change its state to UECC_TRG_HO_PATH_SWITCH_ONGOING */
            UECC_FSM_SET_CHILD_STATE(
                    p_uecc_ue_context,
                    &uecc_fsm_trg_ho_path_switch_ongoing, PNULL);
    } 
    else
    {
        p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->is_sn_status_transfer_ind_recv = 
            RRC_TRUE;
        /*Change its state to UECC_TRG_HO_W_FOR_RRC_CON_RECONFIG_COMPLETE*/
        UECC_FSM_SET_CHILD_STATE(
                p_uecc_ue_context,
                &uecc_fsm_trg_ho_w_for_rrc_conn_reconfig_complete, PNULL);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_w_for_mme_status_transfer_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_trg_ho_w_for_mme_status_transfer_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Change rrc_connection_release_not_required_flag */
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
        "[%s] Toggling rrc_connection_release_not_required_flag",
        __FUNCTION__);

    if(RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
        p_trg_ho_info->is_rrc_conn_reconf_comp_recv)
    {
        p_uecc_ue_context->share_data.uecc_rcr_data.
            rrc_connection_release_not_required_flag = RRC_FALSE;
    }
    else
    {
        p_uecc_ue_context->share_data.uecc_rcr_data.
            rrc_connection_release_not_required_flag = RRC_TRUE;
    }

    /*SPR_17976_START*/
    /* Code Removed */
    /*SPR_17976_END*/

    if (S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;

        /*Send UE CONTEXT RELEASE REQUEST message to S1AP */
        if(RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                p_uecc_ue_context,
                &p_uecc_ue_context->ho_info.cause))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                    "[%s]Unable to build and send enb_ue_ctx_release_req ",__FUNCTION__);
        }
    }

   uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf(
            p_uecc_ue_context, (rrc_bool_et*)RRC_TRUE);

   /*spr_22003_changes_start*/
   /*As HO admission has been failed at target, so no need to increment the
    * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
   p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
   /*spr_22003_changes_end*/

   if (S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
   {
       /*Finish child FSM by posting
         UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT event to main FSM. */
       uecc_fsm_finish_activity_failure(
               p_uecc_ue_context, (void*)UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT);
   }
   else if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
   {
       SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
       /*Delete UE context at X2AP*/
       uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
       
       /*Finish child FSM by posting
         UECC_EV_TRG_HO_RELEASE_REQ_INT event to main FSM. */
       uecc_fsm_finish_activity_failure(
               p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
   }
/*BUG 604 changes start*/
   else if((INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
           (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
   {
       SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
       /*Finish child FSM by posting
         UECC_EV_TRG_HO_RELEASE_REQ_INT event to main FSM. */
       uecc_fsm_finish_activity_failure(
               p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
   }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_w_rrc_conn_reconfig_rrc_conn_reconf_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE message.
 ****************************************************************************/
void uecc_fsm_trg_ho_w_rrc_conn_reconfig_rrc_conn_reconf_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api
        )
{
    rrc_return_t result = RRC_FAILURE;
    uecc_fsm_fail_handler_f const* p_failure_actions_list =
        uecc_fsm_ue_trg_ho_fail_type5_actions;
    
    /* CR 105 changes start*/
    U8 qci_value = RRC_NULL;
    U8 counter = RRC_NULL;
    /* CR 105 changes stop*/
    /*SPR_17284_START*/
    s1ap_Cause s1apCause;
	memset_wrapper(&s1apCause, 0, sizeof(s1apCause));
    /*SPR_17284_END*/
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    
    do {
/*SPR_20529_START*/
        /* Update DRX and PHR config for RELEASE in MAC Main Config*/
        if(p_uecc_ue_context->m.mac_config_present == 1)
        {
            if((p_uecc_ue_context->mac_config.bitmask & RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT) &&
                    (p_uecc_ue_context->mac_config.mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT) &&
                    (p_uecc_ue_context->mac_config.mac_main_config.drx_config.drx_config_type == RRC_DRX_CONFIG_TYPE_REL))
            {
                p_uecc_ue_context->mac_config.mac_main_config.bitmask &= ~(RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT);
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "DRX Config Released");
            }
            if((p_uecc_ue_context->mac_config.bitmask & RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT) &&
                    (p_uecc_ue_context->mac_config.mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT) &&
                    (!(p_uecc_ue_context->mac_config.mac_main_config.phr_config.bitmask & RRC_PHR_CONFIG_PARAM_PRESENT)))
            {
                p_uecc_ue_context->mac_config.mac_main_config.bitmask &= ~(RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT);
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "PHR Config Released");
            }
        }
/*SPR_20529_END*/
/*BUG 604 changes start*/
        if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
            (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
        /*BUG 604 changes stop*/
        {
            /* CR 019 changes start*/
            if ((INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) && 
                    (PNULL != g_rrc_stats.p_rrc_stats))
            {
                /*SPR_17284_START*/
                map_x2ap_cause_to_s1ap_cause(
                        p_uecc_ue_context->ho_info.inter_cell_ho_cause,
                        &s1apCause);
                RRC_UPDATE_PERFORMANCE_COUNTER_X2AP_CAUSE(p_uecc_ue_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].rrc_handover_related_stats.
                        intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.
                        ho_IntraEnbOutSucc_Cause, p_uecc_ue_context->ho_info.inter_cell_ho_cause);
                /*SPR_17284_END*/

                RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].rrc_handover_related_stats.
                        intra_enb_ho_stats.ho_IntraEnbOutSucc_sum, 0, 
                        DUMMY_RANGE_CHECK_INDEX); 
                /* CR 019 changes stop*/

                /*CR 105 changes start*/
                for (counter = 0; counter < MAX_ERAB_COUNT; counter++)
                {
                    if (PNULL != p_uecc_ue_context->p_e_rab_list[counter])
                    {
                        /* update the performance counter */
                        qci_value = p_uecc_ue_context->p_e_rab_list[counter]->
                            e_RABlevelQoSParameters.qCI;
                        /* SPR 13441 Fix Start */
                        if ( (RRC_MIN_QCI_GBR_LIMIT <= qci_value) &&
                             (RRC_MAX_QCI_NON_GBR_LIMIT >= qci_value) )
                        {
                        RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context, 
                                g_rrc_stats.p_rrc_stats->
                                rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                                p_gb_context->offset_uecc_mod_id].
                                rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].rrc_erab_related_stats.
                                erab_release_stats.
                                erab_relAttNbr_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
                                RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

                        RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context, 
                                g_rrc_stats.p_rrc_stats->
                                rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                                p_gb_context->offset_uecc_mod_id].
                                rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].rrc_erab_related_stats.
                                erab_release_stats.erab_relAttNbr_sum, 
                                RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

                        /* SPR Fix 8377 */
                        RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context, 
                                g_rrc_stats.p_rrc_stats->
                                rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                                p_gb_context->offset_uecc_mod_id].
                                rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].
                                rrc_erab_related_stats.erab_release_stats.
                                erab_relSuccNbr_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
                                RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

                        RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                                g_rrc_stats.p_rrc_stats->
                                rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                                p_gb_context->offset_uecc_mod_id].
                                rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].
                                rrc_erab_related_stats.
                                erab_release_stats.erab_relSuccNbr_sum, 
                                RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);
                        /* SPR Fix 8377 */
                    }
                        /* SPR 13441 Fix End */
                    }
                }
                /*CR 105 changes stop*/
            }
            
            /*SPR_17284_START*/
            else if (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
            {
                if (PNULL != g_rrc_stats.p_rrc_stats)
                {
                    map_x2ap_cause_to_s1ap_cause(
                        p_uecc_ue_context->ho_info.intra_cell_ho_cause,
                        &s1apCause);

                    RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_uecc_ue_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].rrc_handover_related_stats.
                        intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.
                        ho_IntraEnbOutSucc_Cause, s1apCause);

                    RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].rrc_handover_related_stats.
                        intra_enb_ho_stats.ho_IntraEnbOutSucc_sum, 0, 
                        DUMMY_RANGE_CHECK_INDEX); 

                }
            }
            /*SPR_17284_END*/
            /* SPR 17391 Fix Start */
            else  
            {
                /*SPR_20720_START*/
                if ((PNULL != g_rrc_stats.p_rrc_stats) &&
                    (RRC_FALSE == p_uecc_ue_context->m.rrc_connection_count_updated))
                /*SPR_20720_END*/
                {
                    RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].
                        rrc_connection_related_stats.
                        rrcConnectionNumber, 0, DUMMY_RANGE_CHECK_INDEX);
	    	    
        		    p_uecc_ue_context->m.rrc_connection_count_updated = RRC_TRUE;
                }
            }
            /* SPR 17391 Fix Stop */
/*SPR_17382_START*/
            /*Send UECC_LLIM_STOP_BUFFER_IND to LLIM*/
            if (RRC_SUCCESS != uecc_llim_build_and_send_data_buffer_stop_ind(
                        p_uecc_ue_context))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s]uecc_llim_build_and_send_data_buffer_stop_ind "
                        "failed !!",__FUNCTION__);
            }
            /* Change its state to UECC_TRG_HO_PATH_SWITCH_ONGOING */
/*SPR_17382_END*/
            UECC_FSM_SET_CHILD_STATE(
                    p_uecc_ue_context,
                    &uecc_fsm_trg_ho_path_switch_ongoing, PNULL);
        }
        else if (S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
        {
                        /*SPR_20720_START*/
            if ((PNULL != g_rrc_stats.p_rrc_stats) &&
                (RRC_FALSE == p_uecc_ue_context->m.rrc_connection_count_updated))
            /*SPR_20720_END*/
            {
                RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].
                    rrc_connection_related_stats.
                    rrcConnectionNumber, 0, DUMMY_RANGE_CHECK_INDEX);
	    	    
        		p_uecc_ue_context->m.rrc_connection_count_updated = RRC_TRUE;
            }
            /* SPR 17391 Fix Stop */
            
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "Sending S1AP HO NOTIFY\n");

            /*Send HO NOTIFY message to MME */
            if (RRC_SUCCESS != uecc_s1ap_build_and_send_handover_notify (
                        p_uecc_ue_context))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s]uecc_s1ap_build_and_send_handover_notify"
                        " failed !!",__FUNCTION__);
                break;
            }
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "Sending UECC LLIM DATA BUFFER STOP IND\n");

            /*Send UECC_LLIM_STOP_BUFFER_IND to LLIM*/
            if (RRC_SUCCESS != uecc_llim_build_and_send_data_buffer_stop_ind(
                        p_uecc_ue_context))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s]uecc_llim_build_and_send_data_buffer_stop_ind "
                        "failed !!;Unused variable p_api=%p",__FUNCTION__,p_api);
                            /*SPR 17777 +-*/
                p_failure_actions_list = 
                    uecc_fsm_ue_trg_ho_fail_type5_actions;
                break;
            }
        /*SPR_17893_START*/
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "Sending UECC RRM UE HO ADM CNF");
        /* RRC_RRM_HO_ADM_CNF to RRM.*/
        if (RRC_FAILURE == uecc_rrm_build_and_send_ue_ho_adm_cnf(
                    p_uecc_ue_context,
                    RRC_SUCCESS))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s] uecc_rrm_build_and_send_ue_ho_adm_cnf failed ",
                    __FUNCTION__);
            break;
        }
        
        uecc_fsm_finish_activity_success(p_uecc_ue_context, PNULL);
        /*SPR_17893_END*/
        }
        /*SPR_17893_START*/
        /* Code Removed */
        /*SPR_17893_END*/

        if ( (PNULL != p_uecc_ue_context->ho_info.p_ho_info) &&
                (PNULL != p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info) )
        {
            p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->is_rrc_conn_reconf_comp_recv = 
                RRC_TRUE;
        }

        result = RRC_SUCCESS;
    }while(0);

    if (result != RRC_SUCCESS)
    {
            uecc_fsm_start_failure_processing(
                    p_uecc_ue_context,
                    p_failure_actions_list);
        }
    uecc_llim_check_and_send_rrc_ue_information(p_uecc_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/******************************************************************************
 *   FUNCTION NAME: uecc_fsm_validate_and_fill_erab_switched_list 
 *   INPUT        : uecc_ue_context_t* p_uecc_ue_context
 *                  s1ap_E_RABToBeSwitchedULList *p_erab_switched_list
 *   OUTPUT       : None               
 *   RETURNS      : RRC_SUCCESS/RRC_PARTIAL_SUCCESS/RRC_FAILURE
 *   DESCRIPTION  : This function validate and fill the erab_switched_list
 *******************************************************************************/
static
rrc_return_et uecc_fsm_validate_and_fill_erab_switched_list(
        uecc_ue_context_t* p_uecc_ue_context,
        s1ap_E_RABToBeSwitchedULList *p_erab_switched_list
        )
{
    OSRTDListNode* p_node = PNULL;
    s1ap_E_RABToBeSwitchedULList_element *p_elem = PNULL;
    uecc_erab_to_be_switched_list_t *p_uecc_erab_switched_list = PNULL;
    rrc_return_et ret = RRC_SUCCESS;
    U8      erab_idx = 0;
    U8      transport_addr_size = 0;
    RRC_ASSERT(PNULL!=p_uecc_ue_context);
    RRC_ASSERT(PNULL!=p_erab_switched_list);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    p_uecc_erab_switched_list = &(p_uecc_ue_context->ho_info.p_ho_info->
            p_trg_ho_info->s1_trg_ho_info.erab_switched_list);

    p_uecc_erab_switched_list->count = 0;

    for( p_node = p_erab_switched_list->head;
            PNULL!=p_node;
            p_node=p_node->next)
    {
        p_elem = (s1ap_E_RABToBeSwitchedULList_element *)p_node->data;
        if (PNULL == p_elem || p_elem->value.u._E_RABToBeSwitchedULItemIEs_1 == PNULL)
        {
            continue;
        }
        if (ASN1V_s1ap_id_E_RABToBeSwitchedULItem != p_elem->id)
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
                    "ASN1V_s1ap_id_E_RABToBeSwitchedULItem != p_elem"
                    "->id (%i)",
                    p_elem->id);
            p_uecc_ue_context->ho_info.cause.u.protocol = s1ap_transfer_syntax_error;
            ret = RRC_FAILURE;
            break;

        }
        erab_idx = (U8)p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->e_RAB_ID;
        if(INVALID_ERAB_ID <= erab_idx )
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                    "INVALID ERAB ID PRESENT [%d]",erab_idx);
            ret = RRC_FAILURE;
            continue;
        }

        if (PNULL == p_uecc_ue_context->p_e_rab_list[erab_idx]) 
        {
            RRC_TRACE(RRC_WARNING,
                    "E-RAB ID not exists in UE Context = %d, Ignoring it",
                    erab_idx);
            continue;
        }
        else
        {
            if ( RRC_NULL == (p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->
                                          transportLayerAddress.numbits % 8) )
            {
                transport_addr_size = (p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->
                                transportLayerAddress.numbits/8);
            }
            else
            {
                transport_addr_size = (p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->
                                transportLayerAddress.numbits/8) + 1;
            }

            if (!memcmp_wrapper(p_uecc_ue_context->p_e_rab_list[erab_idx]->
                        uecc_x2ap_drb_ctx.transportLayerAddress.data,
                        p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->transportLayerAddress.data,
                        transport_addr_size))
            {
                if (!memcmp_wrapper(p_uecc_ue_context->p_e_rab_list[erab_idx]->
                            uecc_x2ap_drb_ctx.gTP_TEID.data,
                            p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->gTP_TEID.data,
                            p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->gTP_TEID.numocts))
                {
                    continue;
                }
            }
        }
        
        /*SPR_17681_START*/
        /* filling tunnel info in UE context */
        if(p_uecc_ue_context->p_e_rab_list[erab_idx]->transportLayerAddress.numbits
                != p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->
                transportLayerAddress.numbits)
        {
            /*
             * Release the memory allocated on receiving HO Request
             * and allocate it again as per memory required to store 
             * the transport address received in Path Switch Req Ack
             */
            if(PNULL != p_uecc_ue_context->p_e_rab_list[erab_idx]->
                    transportLayerAddress.data)
            {
                rrc_mem_free((void *)p_uecc_ue_context->p_e_rab_list[erab_idx]->
                        transportLayerAddress.data);
            }
            p_uecc_ue_context->p_e_rab_list[erab_idx]->transportLayerAddress.numbits
                = p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->
                transportLayerAddress.numbits;
            p_uecc_ue_context->p_e_rab_list[erab_idx]->transportLayerAddress.data
                = (U8 *)rrc_mem_get(transport_addr_size);

            if (PNULL == p_uecc_ue_context->p_e_rab_list[erab_idx]->
                    transportLayerAddress.data)
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_FATAL,
                        "[%s]Memory allocation failed !!",__FUNCTION__);
                break;
            }
        }
        l3_memcpy_wrapper((void*)(p_uecc_ue_context->p_e_rab_list[erab_idx]->
                    transportLayerAddress.data),
                (const void *)(p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->
                    transportLayerAddress.data),
                transport_addr_size);

        /* filling gTP_TEID */
        p_uecc_ue_context->p_e_rab_list[erab_idx]->gTP_TEID
            = p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->gTP_TEID;
        /*SPR_17681_END*/

        p_uecc_erab_switched_list->erab_to_be_switched_item
            [p_uecc_erab_switched_list->count].e_RAB_ID 
            = p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->e_RAB_ID;

        p_uecc_erab_switched_list->erab_to_be_switched_item
            [p_uecc_erab_switched_list->count].gTP_TEID 
            = p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->gTP_TEID;

        asn1Copy_s1ap_TransportLayerAddress(&p_uecc_ue_context->
                s1ap_asn1_ctx,
                &p_elem->value.u._E_RABToBeSwitchedULItemIEs_1->transportLayerAddress,
                &p_uecc_erab_switched_list->
                erab_to_be_switched_item[p_uecc_erab_switched_list->count].
                transportLayerAddress);

        p_uecc_erab_switched_list->count++;
    }
    if(p_uecc_erab_switched_list->count)
    {
        p_uecc_ue_context->ho_info.p_ho_info->
            p_trg_ho_info->s1_trg_ho_info.bitmask |=
            ERAB_TO_BE_SWITCHED_UL_LIST_PRESENT;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return ret;
}

/****************************************************************************
 * Function Name  : uecc_fsm_check_erab_switched_ul_list 
 * Inputs         : uecc_ue_context_t* p_uecc_ue_context
 *                  rrc_s1ap_path_switch_request_ack_t *p_path_switch_req_ack
 * Outputs        : None
 * Returns        : rrc_bool_t
 * Description    : This is the function to check presence of erab switched 
 *                  ul list and fill in ue context.
 ****************************************************************************/
static
rrc_bool_t uecc_fsm_check_erab_switched_ul_list(
        uecc_ue_context_t* p_uecc_ue_context,
        rrc_s1ap_path_switch_request_ack_t *p_path_switch_req_ack
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_ASSERT(PNULL!=p_path_switch_req_ack);

    if(p_path_switch_req_ack->bitmask &
            RRC_S1AP_PATH_SWITCH_REQUEST_ACK_E_RAB_TO_BE_SWITCHED_UL_LIST_PRESENT)
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "[%s]E_RAB_TO_BE_SWITCHED_UL_LIST is present",__FUNCTION__);
        if ( RRC_SUCCESS != uecc_fsm_validate_and_fill_erab_switched_list(
                    p_uecc_ue_context,
                    &(p_path_switch_req_ack->e_rab_to_be_switched_ul_list)))
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name,RRC_ERROR,
                    "Invalid Path Switch Req Ack Received from S1AP");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
            return RRC_FALSE;
        }
        
        if(p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->s1_trg_ho_info.bitmask & ERAB_TO_BE_SWITCHED_UL_LIST_PRESENT)
        {
             UECC_FSM_SET_CHILD_STATE(
                p_uecc_ue_context,
                &uecc_fsm_trg_ho_uplink_tunnel_reconfig_ongoing, PNULL);
        }
        else
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name,
                    RRC_INFO,"Original Transport Layer Address(es) &"
                    "GTP-TEID(s) received in E-RAB TO BE SWITCHED UL LIST",__FUNCTION__);
        }
    }
    if(!(p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->s1_trg_ho_info.bitmask & ERAB_TO_BE_SWITCHED_UL_LIST_PRESENT))
    {
       RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "[%s]E_RAB_TO_BE_SWITCHED_UL_LIST is not present",__FUNCTION__);
       
/*SPR_17382_START*/
/* Code Removed */
/*SPR_17382_END*/
        /*SPR_17893_START*/
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "Sending UECC RRM UE HO ADM CNF");
        /* RRC_RRM_HO_ADM_CNF to RRM.*/
        if (RRC_FAILURE == uecc_rrm_build_and_send_ue_ho_adm_cnf(
                    p_uecc_ue_context,
                    RRC_SUCCESS))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s] uecc_rrm_build_and_send_ue_ho_adm_cnf failed ",
                    __FUNCTION__);
            p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
            uecc_fsm_start_failure_processing(
                    p_uecc_ue_context, uecc_fsm_ue_trg_ho_fail_type5_actions);
        }
        else
        {
            uecc_fsm_finish_activity_success(p_uecc_ue_context, PNULL);
        }
        /*SPR_17893_END*/
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_TRUE;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_fsm_fill_erab_to_be_released_list 
 *   INPUT        : uecc_ue_context_t* p_uecc_ue_context
 *                  s1ap_E_RABList *p_erab_fail_list
 *   OUTPUT       : None
 *   RETURNS      : RRC_SUCCESS/RRC_PARTIAL_SUCCESS/RRC_FAILURE
 *   DESCRIPTION  : This function fill the the erab_fail_list
 *
 *******************************************************************************/
static rrc_return_et uecc_fsm_fill_erab_to_be_released_list(
        uecc_ue_context_t* p_uecc_ue_context,
        s1ap_E_RABList *p_erab_fail_list
        )
{
    OSRTDListNode* p_node = PNULL;
    s1ap_E_RABList_element *p_elem = PNULL;
    rrc_return_et ret = RRC_SUCCESS;
    /* CR 105 changes start*/
    U8  qci_value = 0;
    /* CR 105 changes stop*/

    RRC_ASSERT(PNULL!=p_uecc_ue_context);
    RRC_ASSERT(PNULL!=p_erab_fail_list);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    for (p_node=p_erab_fail_list->head ; PNULL!=p_node; p_node=p_node->next)
    {
        p_elem = (s1ap_E_RABList_element*)p_node->data;
        RRC_ASSERT(PNULL!=p_elem);
        if (p_elem->value.u._E_RABItemIEs_1 == PNULL)
        {
            continue;
        }
        if (ASN1V_s1ap_id_E_RABItem != p_elem->id)
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "ASN1V_s1ap_id_E_RABToBeReleasedList != p_elem"
                    "->id (%i)",
                    p_elem->id);
            /* filling the cause in p_ue_context */
            p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
            ret = RRC_FAILURE;
            break;
        }
        if(PNULL != p_uecc_ue_context->p_e_rab_list[p_elem->value.u._E_RABItemIEs_1->e_RAB_ID])
        {
            /*SPR_17382_START*/
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "[%s] Updating uecc_x2ap_drb_ctx status as "
                    "RRC_ERAB_FAILURE_PATH_SWITCH_REQ_ACK for e_RAB_ID[%d]",
                    __FUNCTION__,p_elem->value.u._E_RABItemIEs_1->e_RAB_ID);
            fill_rrm_cause_from_s1ap_cause((&p_uecc_ue_context->p_e_rab_list
                            [p_elem->value.u._E_RABItemIEs_1->e_RAB_ID]->uecc_x2ap_drb_ctx.cause),
                            (&p_elem->value.u._E_RABItemIEs_1->cause),
                            p_uecc_ue_context);
            p_uecc_ue_context->p_e_rab_list[p_elem->value.u._E_RABItemIEs_1->e_RAB_ID]->
                uecc_x2ap_drb_ctx.status = RRC_ERAB_FAILURE_PATH_SWITCH_REQ_ACK;
            /*SPR_17382_END*/

            /* CR 105 changes start*/
            qci_value = p_uecc_ue_context->p_e_rab_list[p_elem->value.u._E_RABItemIEs_1->e_RAB_ID]->
                e_RABlevelQoSParameters.qCI;

            if (PNULL != g_rrc_stats.p_rrc_stats)
            {
                /* SPR 13441 Fix Start */
                if ( (RRC_MIN_QCI_GBR_LIMIT <= qci_value) &&
                        (RRC_MAX_QCI_NON_GBR_LIMIT >= qci_value) )
                {
                RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].
                        rrc_erab_related_stats.erab_release_stats.
                        erab_relAttNbr_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
                        RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

                RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].
                        rrc_erab_related_stats.erab_release_stats.
                        erab_relAttNbr_sum, RRC_UPDATED_QCI_INDEX(qci_value), 
                        RRC_MAX_QCI_INDEX);
            }
                /* SPR 13441 Fix End */
            }
            /* CR 105 changes start*/
        }
        else
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "[%s] UECC E_RAB context is not found for e_RAB_ID[%d] "
                    ,__FUNCTION__,p_elem->value.u._E_RABItemIEs_1->e_RAB_ID);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return ret;
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_s1ap_path_switch_req_ack_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_S1AP_PATH_SWITCH_REQ_ACK message.
 ****************************************************************************/
static
void uecc_fsm_trg_ho_s1ap_path_switch_req_ack_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api
        )
{
    rrc_return_t result = RRC_FAILURE;
    rrc_s1ap_path_switch_request_ack_t * 
        p_rrc_s1ap_path_switch_request_ack = PNULL;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received S1AP PATH SWITCH REQUEST ACK\n");

    p_rrc_s1ap_path_switch_request_ack = 
        (rrc_s1ap_path_switch_request_ack_t *)p_api;

    do {
         /* Send UE CONTEXT RELEASE to source eNB */ 
        if (RRC_SUCCESS != 
            uecc_x2ap_build_and_send_ue_context_release(p_uecc_ue_context
                ))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                "[%s]uecc_x2ap_build_and_send_ue_context_release"
                " failed !!",__FUNCTION__);

            /* filling the cause in p_ue_context */
            p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
            break;
        }

        if ( (PNULL != p_uecc_ue_context->ho_info.p_ho_info) && 
              (PNULL != p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info) )
        {
            p_uecc_ue_context->ho_info.p_ho_info->\
               p_trg_ho_info->is_path_switch_req_ack_recv = RRC_TRUE;
        }

        /* fill erab released ul list */
        if(p_rrc_s1ap_path_switch_request_ack->bitmask &
                RRC_S1AP_PATH_SWITCH_REQUEST_ACK_E_RAB_TO_BE_RELEASED_LIST_PRESENT)
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "[%s]E_RAB_TO_BE_RELEASED_LIST is present",__FUNCTION__);
            if (RRC_SUCCESS != uecc_fsm_fill_erab_to_be_released_list(
                        p_uecc_ue_context,
                        &p_rrc_s1ap_path_switch_request_ack->
                        e_rab_to_be_released_list))
            {
                /* filling the cause in p_ue_context */
                p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
                break;
            }
            /* x2ap_drop2_rw_start */
            else
            {
                /*Erabs populated in the list need to be released after ho is successful*/
                p_uecc_ue_context->ho_info.is_x2_ho_erab_rel_reqd = RRC_TRUE;
            }
            /*SPR_17382_START*/
            result = uecc_fsm_trg_ho_is_atleast_one_non_gbr_bearer_present(
                    p_uecc_ue_context);

            if(RRC_FAILURE == result)
            {
                /* filling the cause in p_ue_context */
                p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
                break;
            }
            /*SPR_17382_END*/
            /* x2ap_drop2_rw_end */
        }
        else
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "[%s]E_RAB_TO_BE_RELEASED_LIST is not present",__FUNCTION__);
        }
        /*SPR_16416_START*/
        if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
        {
            /* Process queued events*/
            uecc_process_ul_nas_transport_queue(p_uecc_ue_context, &(p_uecc_ue_context->p_ul_nas_transport_queue));
        }
        /*SPR_16416_END*/

        /* check presence of erab switched list, fill in ue context, 
           send llim lc modify req or ue context release accordingly */
        if ( RRC_TRUE != uecc_fsm_check_erab_switched_ul_list(
                    p_uecc_ue_context,
                    p_rrc_s1ap_path_switch_request_ack) )
        {
            /* filling the cause in p_ue_context */
            p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
            break;
        }

        /* fill security context in uecc context */
        p_uecc_ue_context->nh_ncc_pair.used_flag=RRC_FALSE;
        p_uecc_ue_context->nh_ncc_pair.ncc
            = p_rrc_s1ap_path_switch_request_ack->security_context.
            nextHopChainingCount;

        /*SPR_17143_START*/
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
                    p_uecc_ue_context->p_gb_context->facility_name,
                    RRC_INFO,
                    "[%s]:[UE=%u]: NCC Value received in Path Switch Ack = %u",
                    __FUNCTION__, p_uecc_ue_context->ue_index, p_uecc_ue_context->nh_ncc_pair.ncc);
        /*SPR_17143_END*/

        asn1Copy_s1ap_SecurityKey(&p_uecc_ue_context->s1ap_asn1_ctx,
                &p_rrc_s1ap_path_switch_request_ack->security_context.
                nextHopParameter,
                (s1ap_SecurityKey*)&p_uecc_ue_context->nh_ncc_pair.nh);
        result = RRC_SUCCESS;
    }while(0);

    if (result != RRC_SUCCESS)
    {
        uecc_fsm_start_failure_processing(
                p_uecc_ue_context,
                uecc_fsm_ue_trg_ho_fail_type5_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_s1ap_path_switch_req_fail_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_S1AP_PATH_SWITCH_REQ_FAIL message.
 ****************************************************************************/
static
void uecc_fsm_trg_ho_s1ap_path_switch_req_fail_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api
        )
{
    /* CR 105 changes start */
    U8  qci_value = RRC_NULL;
    U8  counter = RRC_NULL;
    /* CR 105 changes stop */
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received S1AP PATH SWITCH REQUEST FAILURE\n");

    /*SPR_16590_START*/
    /* Delete UE context from MME */
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
        "[%s] Setting rrc_connection_release_not_required_flag",
        __FUNCTION__);

    p_uecc_ue_context->share_data.uecc_rcr_data.
        rrc_connection_release_not_required_flag = RRC_FALSE;
    /*SPR_16590_END*/

    /* CR 105 changes start */
    if ( PNULL != g_rrc_stats.p_rrc_stats)
    {
        for (counter = 0; counter < MAX_ERAB_COUNT; counter++)
        {
            if (PNULL != p_uecc_ue_context->p_e_rab_list[counter])
            {
                qci_value = p_uecc_ue_context->p_e_rab_list[counter]->
                    e_RABlevelQoSParameters.qCI;
                /* SPR 13441 Fix Start */
                if ( (RRC_MIN_QCI_GBR_LIMIT <= qci_value) &&
                        (RRC_MAX_QCI_NON_GBR_LIMIT >= qci_value) )
                {
                RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].
                        rrc_erab_related_stats.erab_release_stats.
                        erab_relAttNbr_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
                        RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

                RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].rrc_erab_related_stats.
                        erab_release_stats.erab_relAttNbr_sum, 
                        RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);
                
                /* SPR Fix 8377 */
                RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].
                        rrc_erab_related_stats.erab_release_stats.
                        erab_relSuccNbr_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
                        RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

                RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].rrc_erab_related_stats.
                        erab_release_stats.erab_relSuccNbr_sum, 
                        RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);
                /* SPR Fix 8377 */
            }
                /* SPR 13441 Fix End */
            }
        }
    }
    /*SPR_16590_START*/
    /* Send UE CONTEXT RELEASE to source eNB */ 
    if (RRC_SUCCESS != 
            uecc_x2ap_build_and_send_ue_context_release(p_uecc_ue_context
                ))
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                "[%s]uecc_x2ap_build_and_send_ue_context_release"
                " failed !!;Unused variable p_api=%p",__FUNCTION__,p_api);/*SPR 17777 +-*/
        }

    /* Path Switch Failure will also be
     * treated as Path Switch Ack though in this case
     * L3 will wait for only UE Context Release Command
     * from MME and will process only UL/DL NAS Msg */
    p_uecc_ue_context->ho_info.p_ho_info->\
        p_trg_ho_info->is_path_switch_req_ack_recv = RRC_TRUE;

    if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        /* Process queued events*/
        uecc_process_ul_nas_transport_queue(p_uecc_ue_context, &(p_uecc_ue_context->p_ul_nas_transport_queue));
    }
    /*SPR_16590_END*/

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_llim_s1u_reconfig_for_switched_uplink_tunnel_cnf_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_S1U_RECONFIG_CNF message.
 ****************************************************************************/
static void 
uecc_fsm_trg_ho_llim_s1u_reconfig_for_switched_uplink_tunnel_cnf_handler
(
 uecc_ue_context_t*  p_uecc_ue_context,
 void*               p_api
 )
{

    rrc_uecc_llim_s1u_reconfigure_cnf_t*
        p_rrc_uecc_llim_s1u_reconfigure_cnf = PNULL;
    /*SPR_17382_START*/
    rrc_return_t                    result = RRC_SUCCESS;
    /*SPR_17382_END*/
    U8 drb_count = RRC_NULL;
    U32 erab_id = RRC_NULL;


    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    p_rrc_uecc_llim_s1u_reconfigure_cnf
        = (rrc_uecc_llim_s1u_reconfigure_cnf_t*)p_api;

    if (p_rrc_uecc_llim_s1u_reconfigure_cnf->bitmask &
            RRC_UECC_LLIM_S1U_SETUP_SAP_CNF_PRESENT)
    {
        for (; drb_count<(p_rrc_uecc_llim_s1u_reconfigure_cnf->
                setup_sap_cnf_list.num_setup_sap_cnf); drb_count++)
        {
            /* getting erab id */
            erab_id = get_erab_id_from_lc_id(
                    p_uecc_ue_context,
                    p_rrc_uecc_llim_s1u_reconfigure_cnf-> 
                    setup_sap_cnf_list.setup_sap_cnf[drb_count].lc_id);

            if (erab_id >= INVALID_ERAB_ID)
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s] "
                        "Invalid LC id %d \n",__FUNCTION__,
                        p_rrc_uecc_llim_s1u_reconfigure_cnf->
                        setup_sap_cnf_list.setup_sap_cnf[drb_count].lc_id);
                continue;
            }
            if(!(p_uecc_ue_context->p_e_rab_list[erab_id]))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "erab_context " 
                        "corresponding to erab id [%d] is missing ",erab_id);
                continue;
            }
            if(S1U_SUCCESS != p_rrc_uecc_llim_s1u_reconfigure_cnf->
                    setup_sap_cnf_list.setup_sap_cnf[drb_count].cause)
            { 
                p_uecc_ue_context->p_e_rab_list[erab_id]->
                uecc_x2ap_drb_ctx.cause.type
                    = T_x2ap_Cause_transport;

                p_uecc_ue_context->p_e_rab_list[erab_id]->
                uecc_x2ap_drb_ctx.cause.value
                    = x2ap_transport_resource_unavailable; 

                p_uecc_ue_context->p_e_rab_list[erab_id]->
                /*SPR_17382_START*/
                uecc_x2ap_drb_ctx.status
                    = RRC_ERAB_FAILURE_S1U_RECONFIGURE_CNF;
                /*Erabs populated in the list need to be released after ho is successful*/
                p_uecc_ue_context->ho_info.is_x2_ho_erab_rel_reqd = RRC_TRUE;
                /*SPR_17382_END*/
            }
        }
        /*SPR_17382_START*/
        result = uecc_fsm_trg_ho_is_atleast_one_non_gbr_bearer_present(
                p_uecc_ue_context);
        if(RRC_FAILURE == result)
        {
            p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_transport;
            p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_transport_resource_unavailable;
        }
        /*SPR_17382_END*/
    }   
    /*SPR_17382_START*/
    if(RRC_SUCCESS == result)
    {
        switch (p_rrc_uecc_llim_s1u_reconfigure_cnf->response_code)
        {
            case RRC_SUCCESS:
            case RRC_PARTIAL_SUCCESS:
                result = RRC_SUCCESS;
                /* Set S1U Reconfigured  */
                RRC_ASSERT(PNULL != p_uecc_ue_context->ho_info.p_ho_info);
                RRC_ASSERT(PNULL != p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info);

                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                        "[UECC_LLIM_S1U_RECONFIG_CNF] S1U Reconfigured "
                        "successfully");
                p_uecc_ue_context->x2ap_reset_recvd_flag = RRC_FALSE; 
                /*SPR_17893_START*/
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                        "Sending UECC RRM UE HO ADM CNF");
                /* RRC_RRM_HO_ADM_CNF to RRM.*/
                if (RRC_FAILURE == uecc_rrm_build_and_send_ue_ho_adm_cnf(
                            p_uecc_ue_context,
                            RRC_SUCCESS))
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                            "[%s] uecc_rrm_build_and_send_ue_ho_adm_cnf failed ",
                            __FUNCTION__);
                    result = RRC_FAILURE;
                    break;
                }
                uecc_fsm_finish_activity_success(p_uecc_ue_context, PNULL);
                /*SPR_17893_END*/
                break;
            case RRC_FAILURE:
                result = RRC_FAILURE;
                if (X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
                {
                    uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
                }
                break;
            default:
                result = RRC_FAILURE;
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[UECC_LLIM_S1U_RECONFIG_CNF] Invalid Response=%i",
                        p_rrc_uecc_llim_s1u_reconfigure_cnf->response_code);
                break;
        }
    }
    /*SPR_17382_END*/
    /* Process failure */
    if (RRC_FAILURE == result)
    {
        p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
        uecc_fsm_start_failure_processing(
                p_uecc_ue_context,
                uecc_fsm_ue_trg_ho_fail_type5_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_w_for_rrc_conn_reconfig_complete_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_trg_ho_w_for_rrc_conn_reconfig_complete_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

/*BUG 604 changes start*/
    /*if(INTRA_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover)
    { 
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        return;
    }*/
/*BUG 604 changes stop*/
    
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "[%s] Setting rrc_connection_release_not_required_flag",
            __FUNCTION__);
    p_uecc_ue_context->share_data.uecc_rcr_data.
        rrc_connection_release_not_required_flag = 1;

    if (S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;

        /*Send UE CONTEXT RELEASE REQUEST message to S1AP */
        if(RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                    p_uecc_ue_context,
                    &p_uecc_ue_context->ho_info.cause))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                    "[%s]Unable to build and send enb_ue_ctx_release_req ",__FUNCTION__);
        }
    }

    uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf(
            p_uecc_ue_context, (rrc_bool_et *)RRC_TRUE);

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    if (S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        /*Finish child FSM by posting
          UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT event to main FSM. */
        uecc_fsm_finish_activity_failure(
                p_uecc_ue_context, (void*)UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT);
    }
    else if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
        /*Delete UE context at X2AP*/
        uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
        
        /*Finish child FSM by posting
          UECC_EV_TRG_HO_RELEASE_REQ_INT event to main FSM. */
        uecc_fsm_finish_activity_failure(
                p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
    }
/*BUG 604 changes start*/
    else if((INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
    {
        SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
        /*Finish child FSM by posting
          UECC_EV_TRG_HO_RELEASE_REQ_INT event to main FSM. */
        uecc_fsm_finish_activity_failure(
                p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/*********************************************************************************
 * Function Name  : uecc_fsm_trg_ho_w_for_path_switch_req_ack_timer_expiry_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_TIMER_EXPIRY message in state 
 *                  uecc_fsm_trg_ho_path_switch_ongoing.
 ********************************************************************************/
void uecc_fsm_trg_ho_w_for_path_switch_req_ack_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    /*SPR_16590_START*/
    s1ap_Cause cause;
    /*SPR_16590_END*/

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
        "[%s] Toggling rrc_connection_release_not_required_flag",
        __FUNCTION__);

    p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_FALSE;
    /*SPR_16590_START*/
    /*Send UE CONTEXT RELEASE REQUEST message to S1AP */
    if(RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->
             p_trg_ho_info->is_path_switch_req_ack_recv)
    {
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_unspecified_2; 

        if(RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                    p_uecc_ue_context,
                    &cause))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                "[%s]Unable to build and send enb_ue_ctx_release_req ",__FUNCTION__);
        }
    /*SPR_17851_START*/
    }
    else
    {
        SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);

    }
    /*SPR_17851_END*/
    /*SPR_16590_END*/

     uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf(
            p_uecc_ue_context, (rrc_bool_et *)RRC_TRUE);

    /*Delete UE context at X2AP*/
    if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
    }        
    
    /*SPR_16590_START*/
    
    /*Finish child FSM by posting UECC_EV_TRG_HO_RELEASE_REQ_INT event to main FSM. */
    if(RRC_TRUE != p_uecc_ue_context->ho_info.p_ho_info->
             p_trg_ho_info->is_path_switch_req_ack_recv)
    {
        /*SPR_17851_START*/
        if(RRC_FAILURE == uecc_s1ap_build_and_send_ue_release_ind(p_uecc_ue_context))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s] Failed to send UE Release Indication to S1AP "
                    ,__FUNCTION__);
        }
        /*SPR_17851_END*/
        uecc_fsm_finish_activity_failure(
                p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
    }
    else
    {
        /*Finish child FSM by posting
          UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT event to main FSM. */
        uecc_fsm_finish_activity_failure(
                p_uecc_ue_context, (void*)UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT);
    }
    /*SPR_16590_END*/

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_w_for_rrc_s1u_reconfig_cnf_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_TIMER_EXPIRY message in state 
 *                  uecc_fsm_trg_ho_uplink_tunnel_reconfig_ongoing.
 ****************************************************************************/
void uecc_fsm_trg_ho_w_for_rrc_s1u_reconfig_cnf_timer_expiry_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    s1ap_Cause cause;
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "[%s] Setting rrc_connection_release_not_required_flag",
            __FUNCTION__);

    p_uecc_ue_context->share_data.uecc_rcr_data.
        rrc_connection_release_not_required_flag = RRC_FALSE;

    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_unspecified_2; 

    /*Send UE CONTEXT RELEASE REQUEST message to S1AP */
    if(RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
            p_uecc_ue_context,
            &cause))
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                "[%s]Unable to build and send enb_ue_ctx_release_req ",__FUNCTION__);
    }

    uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf(
            p_uecc_ue_context, (rrc_bool_et *)RRC_TRUE);

    /*Delete UE context at X2AP*/
    if (X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
    }
    /*Finish child FSM by posting
      UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT event to main FSM. */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/*This function is not used any where */

/****************************************************************************
 * Function Name  : uecc_fsm_free_trg_ho_context
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : This function free TRG HO related memory allocated in
                    UECC UE context
 ****************************************************************************/
void uecc_fsm_free_trg_ho_context(
    uecc_ue_context_t*  p_uecc_ue_context
)
{
    U32 count=RRC_NULL;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    if (p_uecc_ue_context->ho_info.p_ho_info)
    {
        /* SPR 19074 Fix Start */
        /* SPR 19338 Fix Start */
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                       p_uecc_ue_context->p_gb_context->facility_name,
                       RRC_INFO,
              "[%s]removing target ho context for UE Index [%u]!!",
              __FUNCTION__, p_uecc_ue_context->ue_index);
        /* SPR 19338 Fix Stop */
        /* SPR 19074 Fix End */
        if (p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info)
        {
            if (p_uecc_ue_context->ho_info.p_ho_info->
                    p_trg_ho_info->p_rrc_rrm_ho_adm_resp)
            {
                rrc_mem_free(p_uecc_ue_context->ho_info.p_ho_info->
                        p_trg_ho_info->p_rrc_rrm_ho_adm_resp);
                p_uecc_ue_context->ho_info.p_ho_info->
                    p_trg_ho_info->p_rrc_rrm_ho_adm_resp = PNULL;
            }
            for (count = 0; count < MAX_ERAB_COUNT; count++)
            {
                if((p_uecc_ue_context->ho_info.p_ho_info->
                            p_trg_ho_info->s1_trg_ho_info.
                            p_erab_trg_ho_forward_list[count]))
                {
                    rrc_mem_free(p_uecc_ue_context->ho_info.p_ho_info->
                            p_trg_ho_info->s1_trg_ho_info.
                            p_erab_trg_ho_forward_list[count]);
                    p_uecc_ue_context->ho_info.p_ho_info->
                        p_trg_ho_info->s1_trg_ho_info.
                        p_erab_trg_ho_forward_list[count] = PNULL;
                }
            }
            rrc_mem_free(p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info);
            p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info = PNULL;
        }
        rrc_mem_free(p_uecc_ue_context->ho_info.p_ho_info);
        p_uecc_ue_context->ho_info.p_ho_info = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_llim_process_create_ue_entity_response
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_uecc_llim_create_ue_entity_resp_t*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_LLIM_CREATE_UE_ENTITY_RESP from LLIM
 ****************************************************************************/
rrc_return_t uecc_fsm_trg_ho_llim_process_create_ue_entity_response(
    uecc_ue_context_t   *p_uecc_ue_context,
    rrc_uecc_llim_create_ue_entity_resp_t* p_llim_create_ue_entity_resp
)
{
    U32 erab_id = RRC_NULL;
    rrc_return_t                           result = RRC_FAILURE;
    U8 drb_count = RRC_NULL;
    uecc_e_rab_failed_to_add_list_t 
        *p_uecc_e_rab_failed_to_add_list = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);


    if (p_llim_create_ue_entity_resp->bitmask & 
            UECC_LLIM_CREATE_SRB_ENTITY_ERROR_INFO_PRESENT)
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                "[%s]SRB creation failed at Lower Layers!!",__FUNCTION__);
        return RRC_FAILURE;
    }

    p_uecc_e_rab_failed_to_add_list = 
        &p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
        s1_trg_ho_info.erab_failed_list;

    for (drb_count =0; drb_count < p_llim_create_ue_entity_resp->
            create_drb_status_counter; drb_count++)
    {
        /*Erab id */

        erab_id = get_erab_id_from_lc_id (
                p_uecc_ue_context,
                p_llim_create_ue_entity_resp->
                create_drb_status[drb_count].lc_id);

        if (erab_id >= INVALID_ERAB_ID)
        {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s] "
                        "Invalid LC id %d \n",__FUNCTION__,
                        p_llim_create_ue_entity_resp->
                        create_drb_status[drb_count].lc_id);
            continue;
        }

        if (RRC_SUCCESS != p_llim_create_ue_entity_resp->
                create_drb_status[drb_count].response)
        {
            p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list[
                p_uecc_e_rab_failed_to_add_list ->count].e_rab_id = 
                erab_id;
            /*cause */
        /*BUG 604 changes start*/
            if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
        /*BUG 604 changes start*/
            {
                p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list[
                    p_uecc_e_rab_failed_to_add_list->count].
                    x2ap_cause.t = T_x2ap_Cause_radioNetwork;
                p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list[
                    p_uecc_e_rab_failed_to_add_list->count].x2ap_cause.u.
                    radioNetwork = x2ap_unspecified_2;
            }
            else
            {
                p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list[
                    p_uecc_e_rab_failed_to_add_list->count].
                    cause.t = T_s1ap_Cause_radioNetwork;
                p_uecc_e_rab_failed_to_add_list->e_rab_failed_to_add_list[
                    p_uecc_e_rab_failed_to_add_list->count].cause.u.radioNetwork = 
                    s1ap_unspecified_2;
            }
            if (PNULL != p_uecc_ue_context->p_e_rab_list[erab_id])
            {
                /*SPR 15086 Fix Start */
                p_uecc_e_rab_failed_to_add_list->
                    e_rab_failed_to_add_list[p_uecc_e_rab_failed_to_add_list ->count].drb_id =
                     p_uecc_ue_context->p_e_rab_list[erab_id]->drb_config.drb_id;
                /*SPR 15086 Fix Stop */
            /* Free UECC UE ERAB context */
                rrc_mem_free(p_uecc_ue_context->p_e_rab_list[erab_id]);
                p_uecc_ue_context->p_e_rab_list[erab_id] = PNULL;
            }

            /* Free any forward list if present */
            if (p_uecc_ue_context->ho_info.p_ho_info->
                    p_trg_ho_info->s1_trg_ho_info.
                    p_erab_trg_ho_forward_list[erab_id])
            {
                rrc_mem_free (p_uecc_ue_context->ho_info.p_ho_info->
                        p_trg_ho_info->s1_trg_ho_info.
                        p_erab_trg_ho_forward_list[erab_id]);
                p_uecc_ue_context->ho_info.p_ho_info->
                    p_trg_ho_info->s1_trg_ho_info.
                    p_erab_trg_ho_forward_list[erab_id]= PNULL;
            }
            p_uecc_e_rab_failed_to_add_list->count++;

            if (!(--(p_uecc_ue_context->e_rab_list_count)))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                        "[%s] ERAB to be added has become zero !!\n",
                        __FUNCTION__);
                return RRC_FAILURE;
            }
        }
        else
        {
            if (!(p_llim_create_ue_entity_resp->create_drb_status
                        [drb_count].bitmask & 
                        UECC_LLIM_CREATE_LC_STATUS_S1U_RESPONSE_PRESENT))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "UECC_LLIM_CREATE_LC_STATUS_S1U_RESPONSE"
                        "is not present");
                continue;
            }
            if (RRC_FAILURE ==  
                    uecc_fsm_trg_ho_llim_process_create_ue_entity_response_success(
                        p_uecc_ue_context,
                        &p_llim_create_ue_entity_resp->create_drb_status[drb_count],
                        p_uecc_e_rab_failed_to_add_list,
                        erab_id
                        ))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "uecc_fsm_trg_ho_llim_process_create_ue_entity_response_success"
                        "failed!!");
                return RRC_FAILURE;
            }
        }
    }

    result = uecc_fsm_trg_ho_is_atleast_one_non_gbr_bearer_present(
            p_uecc_ue_context);

    if (result == RRC_FAILURE)
    {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s] No non GBR bearers left\n!!",
                    __FUNCTION__);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_is_atleast_one_non_gbr_bearer_present
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : This returns RRC_SUCCESS if atleast one non GBR bearer
                    is present.
 ****************************************************************************/
rrc_return_t uecc_fsm_trg_ho_is_atleast_one_non_gbr_bearer_present(
    uecc_ue_context_t   *p_uecc_ue_context
)
{
    rrc_return_t result = RRC_FAILURE;
    U8 erab_count = RRC_NULL;

    /* According to 
       [TS 23.203 Table 6.1.7: Standardized QCI characteristics] 
       A GBR Bearer has QCI values from 1 to 4 and
       a non GBR Bearer has QCI values from 5 to 9*/

    for (erab_count = 0; erab_count < MAX_ERAB_COUNT; erab_count++)
    {
        /*BUG 604 changes start*/
        if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
        /*BUG 604 changes stop*/
        {
            /*SPR_17382_START*/
            if ((p_uecc_ue_context->p_e_rab_list[erab_count]) &&
                    (!RRC_IS_GBR_BEARER(p_uecc_ue_context->p_e_rab_list
                        [erab_count]->uecc_x2ap_drb_ctx.
                        e_RABlevelQoSParameters.qCI)) &&
                    (RRC_ERAB_FAILURE_PATH_SWITCH_REQ_ACK != p_uecc_ue_context->p_e_rab_list
                     [erab_count]->uecc_x2ap_drb_ctx.status) &&
                    (RRC_ERAB_FAILURE_S1U_RECONFIGURE_CNF != p_uecc_ue_context->p_e_rab_list
                     [erab_count]->uecc_x2ap_drb_ctx.status))
            /*SPR_17382_END*/
            {
                result = RRC_SUCCESS;
                break;
            }
        }
        else
        {
            if (p_uecc_ue_context->p_e_rab_list[erab_count] &&
                    !RRC_IS_GBR_BEARER(p_uecc_ue_context->p_e_rab_list[erab_count]->
                        e_RABlevelQoSParameters.qCI))
            {
                result = RRC_SUCCESS;
                break;
            }
        }
    }

    return result;
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_is_atleast_one_rlc_am_mode_drb_present
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : This returns RRC_SUCCESS if atleast one non GBR bearer
                    is present.
 ****************************************************************************/
rrc_return_t uecc_fsm_trg_ho_is_atleast_one_rlc_am_mode_drb_present(
        uecc_ue_context_t   *p_uecc_ue_context
        )
{
    rrc_return_t result = RRC_FAILURE;
    U8 erab_count = RRC_NULL;
    U8 drb_count = RRC_NULL;
    rrm_drb_to_add_info_list_t *p_drb_info_list = PNULL;

    /* According to
       [TS 23.203 Table 6.1.7: Standardized QCI characteristics] 
       A GBR Bearer has QCI values from 1 to 4 and
       a non GBR Bearer has QCI values from 5 to 9*/
    if ( (PNULL == p_uecc_ue_context->ho_info.p_ho_info)||
            (PNULL == p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info))
        {
           return RRC_FAILURE;
        }
    

    p_drb_info_list = (rrm_drb_to_add_info_list_t *)(&p_uecc_ue_context->ho_info.p_ho_info->
                         p_trg_ho_info->drb_info_recv_from_src);

    for (erab_count = 0; erab_count < MAX_ERAB_COUNT; erab_count++)
    {
        if (RRC_SUCCESS == result)
        {
            break;
        }
        if (p_uecc_ue_context->p_e_rab_list[erab_count] &&
                (p_uecc_ue_context->p_e_rab_list[erab_count]->
                 drb_config.rlc_config_enb.rlc_config.bitmask &
                 RRC_RLC_CONFIG_AM_CONFIG_PRESENT))

        {
          for ( drb_count = 0; drb_count < p_drb_info_list->drb_count; drb_count++ )
          {
            if ( p_drb_info_list->drb_config[drb_count].erab_id ==
                     p_uecc_ue_context->p_e_rab_list[erab_count]->e_RAB_ID )
            {
              if ( p_drb_info_list->drb_config[drb_count].rlc_config_ue.
                         bitmask & RRC_RLC_CONFIG_AM_CONFIG_PRESENT )
              {
                  result = RRC_SUCCESS;
                  break;
              }
            }
          }
        }
    }

    return result;
}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_fail_action_last_wait_for_s1ap_ue_rel_cmd
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of UECC_TRG_HO_FAIL_ACTION_LAST
 ****************************************************************************/
void uecc_fsm_ue_trg_ho_fail_action_last_wait_for_s1ap_ue_rel_cmd(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    /*Send UE CONTEXT RELEASE REQUEST message to S1AP */
    if(RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                p_uecc_ue_context,
                &p_uecc_ue_context->ho_info.cause))
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
            p_uecc_ue_context->p_gb_context->facility_name,
            RRC_ERROR,
            "[%s] Fail to Send UE CONTEXT RELEASE REQUEST message to S1AP." 
            ,__FUNCTION__);
    }

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END");

    /*Finish child FSM by posting
      UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT event to main FSM. */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT);
    
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_llim_process_create_ue_entity_response_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_uecc_llim_create_ue_entity_resp_t*   - api data
 *                  uecc_e_rab_failed_to_add_list_t *p_uecc_e_rab_failed_to_add_list
 *                  U32 erab_id
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_LLIM_CREATE_UE_ENTITY_RESP from LLIM
                    in case if it is a success and contains s1u response.
 ****************************************************************************/
rrc_return_t uecc_fsm_trg_ho_llim_process_create_ue_entity_response_success(
    uecc_ue_context_t   *p_uecc_ue_context,
    rrc_uecc_llim_create_lc_status_t * p_create_drb_status,
    uecc_e_rab_failed_to_add_list_t *p_uecc_e_rab_failed_to_add_list,
    U32 erab_id
)
{
    U8 rb_direction = RRC_NULL;
    uecc_ho_drb_forward_info_list_t          
        **pp_erab_trg_ho_forward_list = PNULL;
    U8 tunnel_info_cnf_count = RRC_NULL;
    U8 is_not_fwd_tunnel = RRC_NULL;
    rrc_uecc_llim_s1u_response_t * p_rrc_uecc_llim_s1u_response = PNULL;
    U8 expect_tunnel_count = 1; // Default tunnel 
    /* Bug 817 Fix Start */
    U8 fwd_tunnel_count = RRC_NULL;
    /* Bug 817 Fix Stop */

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    p_rrc_uecc_llim_s1u_response = &p_create_drb_status->s1u_response;

    pp_erab_trg_ho_forward_list = &p_uecc_ue_context->ho_info.
        p_ho_info->p_trg_ho_info->s1_trg_ho_info.
        p_erab_trg_ho_forward_list[0];

    /* Find expected tunnel count number */
    if (PNULL == pp_erab_trg_ho_forward_list[erab_id])
    {
        expect_tunnel_count = 1;
    }
    else if ((UECC_FORWARDING_DIRECTION_UL == 
             pp_erab_trg_ho_forward_list[erab_id]->forward_direction) ||
             (UECC_FORWARDING_DIRECTION_DL ==
              pp_erab_trg_ho_forward_list[erab_id]->forward_direction))
    {
        expect_tunnel_count = 2;
    }
    else if (UECC_FORWARDING_DIRECTION_BOTH ==
             pp_erab_trg_ho_forward_list[erab_id]->forward_direction)
    {
        expect_tunnel_count = 3;
    }
    

    if (p_rrc_uecc_llim_s1u_response->tunnel_info_cnf_count!=
        expect_tunnel_count)
    {
	    RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
		    "INCORRECT TUNNEL INFO RECV IN S1U CNF FOR ERAB ID = %u: RECV TUNNEL INFO COUNT = %u ."
                    " EXPECTED TUNNEL COUNT = %u .",
                    erab_id,
                    p_rrc_uecc_llim_s1u_response->tunnel_info_cnf_count,
                    expect_tunnel_count);
	    return RRC_FAILURE;
    }


    for (tunnel_info_cnf_count = 0;
            tunnel_info_cnf_count < p_rrc_uecc_llim_s1u_response->
            tunnel_info_cnf_count; tunnel_info_cnf_count++)
    {
        is_not_fwd_tunnel = 0;

        if(UECC_FORWARDING_DIRECTION_BOTH ==
           p_rrc_uecc_llim_s1u_response->tunnel_info_cnf[tunnel_info_cnf_count].
           rb_direction)
           {
               is_not_fwd_tunnel = 1;
           }

        if (S1U_SUCCESS != p_rrc_uecc_llim_s1u_response->
                tunnel_info_cnf[tunnel_info_cnf_count].cause)
        {
            if (is_not_fwd_tunnel)
            {
                /* If the tunnel whose creation failed is not
                   one the forwarding tunnel then the creation of
                   DRB at S1U failed */
                p_uecc_e_rab_failed_to_add_list->
                    e_rab_failed_to_add_list[
                    p_uecc_e_rab_failed_to_add_list->count].e_rab_id = 
                    erab_id;
                /*cause */
                if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) ||
                        (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
                {
                    p_uecc_e_rab_failed_to_add_list->
                        e_rab_failed_to_add_list[
                        p_uecc_e_rab_failed_to_add_list->count].
                        x2ap_cause.t = T_x2ap_Cause_radioNetwork;
                    p_uecc_e_rab_failed_to_add_list->
                        e_rab_failed_to_add_list[
                        p_uecc_e_rab_failed_to_add_list->count].
                        x2ap_cause.u.radioNetwork=
                        x2ap_unspecified_2;
                }
                else
                {
                    p_uecc_e_rab_failed_to_add_list->
                        e_rab_failed_to_add_list[
                        p_uecc_e_rab_failed_to_add_list->count].
                        cause.t = T_s1ap_Cause_radioNetwork;
                    p_uecc_e_rab_failed_to_add_list->
                        e_rab_failed_to_add_list[
                        p_uecc_e_rab_failed_to_add_list->count].
                        cause.u.radioNetwork= 
                        s1ap_unspecified_2;
                }
                if (PNULL != p_uecc_ue_context->p_e_rab_list[erab_id])
                {
	                /*SPR 15086 Fix Start */
		            p_uecc_e_rab_failed_to_add_list->
		             e_rab_failed_to_add_list[p_uecc_e_rab_failed_to_add_list->count].drb_id =
		              p_uecc_ue_context->p_e_rab_list[erab_id]->drb_config.drb_id;
		            /*SPR 15086 Fix Stop */
                /* Free UECC UE ERAB context */
                    rrc_mem_free(p_uecc_ue_context->p_e_rab_list[erab_id]);
                    p_uecc_ue_context->p_e_rab_list[erab_id] = PNULL;
                }

                /* Free any forward list if present */
                if (p_uecc_ue_context->ho_info.p_ho_info->
                        p_trg_ho_info->s1_trg_ho_info.
                        p_erab_trg_ho_forward_list[erab_id])
                {
                    rrc_mem_free (p_uecc_ue_context->ho_info.p_ho_info->
                            p_trg_ho_info->s1_trg_ho_info.
                            p_erab_trg_ho_forward_list[erab_id]);
                    p_uecc_ue_context->ho_info.p_ho_info->
                        p_trg_ho_info->s1_trg_ho_info.
                        p_erab_trg_ho_forward_list[erab_id]= PNULL;
                }
                p_uecc_e_rab_failed_to_add_list->count++;

                if (!(--(p_uecc_ue_context->e_rab_list_count)))
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                            "[%s] ERAB to be added has become zero !!\n",
                            __FUNCTION__);
                    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }
            }
            else
            {
                /* If the tunnel whose creation failed is 
                   one the forwarding tunnel then update
                   the fwd tunnel info accordingly */
                rb_direction = 
                    (U8) (p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                    [tunnel_info_cnf_count].rb_direction);

                if (!pp_erab_trg_ho_forward_list[erab_id])
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                            "[%s] Forwarding not present for ERAB ID %d ignoring fwd info!!\n",
                            __FUNCTION__,erab_id);
                    return RRC_SUCCESS;
                }
                if (pp_erab_trg_ho_forward_list[erab_id]->
                        forward_direction ==
                        UECC_FORWARDING_DIRECTION_BOTH  )
                {
                    pp_erab_trg_ho_forward_list[erab_id]->
                        forward_direction = (rrm_forwarding_direction_et)(!rb_direction);
                }
                else
                {
                    pp_erab_trg_ho_forward_list[erab_id]->
                        forward_direction =
                        UECC_FORWARDING_DIRECTION_INVALID;
                }
            }
        }
        else
        {
            if (!is_not_fwd_tunnel)
            {
                /* If the tunnel whose creation succeded is not
                   one the forwarding tunnel then update
                   the forwarding tunnel information*/

                rb_direction = 
                    (U8) (p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                    [tunnel_info_cnf_count].rb_direction);

                RRC_ASSERT(rb_direction < MAX_FWD_TUNNELS_PER_ERAB);
                
                if (!pp_erab_trg_ho_forward_list[erab_id])
                {
                    RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                            "[%s] Forwarding not present for ERAB ID %d "
                            " returning failure!!\n",
                            __FUNCTION__,erab_id);
                    return RRC_FAILURE;
                }

                /* Match Tunnel Direction */
                if (pp_erab_trg_ho_forward_list[erab_id]->forward_direction != 
                        UECC_FORWARDING_DIRECTION_BOTH)
                {
                    if (pp_erab_trg_ho_forward_list[erab_id]->forward_direction
                            != 
                        p_rrc_uecc_llim_s1u_response->tunnel_info_cnf[tunnel_info_cnf_count].
                        rb_direction)
                    {
                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                            "Invalid Forwarding Direction Received for ERAB ID %d"
                            ", Recv Tunnel Direction = %u, Tunnel Direction In"
                            "Context = %u", 
                            erab_id,
                            p_rrc_uecc_llim_s1u_response->tunnel_info_cnf[tunnel_info_cnf_count].
                            rb_direction,
                            pp_erab_trg_ho_forward_list[erab_id]->forward_direction);
                        
                        return RRC_FAILURE;
                    }
                }
                /* Bug 817 Fix Start */
                if (PNULL == p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
				 p_uecc_drb_fwd_tnl_list[p_create_drb_status->lc_id])
                {
                    p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
                        p_uecc_drb_fwd_tnl_list[p_create_drb_status->lc_id] = 
                               rrc_mem_get (sizeof(rrc_uecc_drb_fwd_tunnel_info_list_t));
                    if (PNULL !=  p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
                            p_uecc_drb_fwd_tnl_list[p_create_drb_status->lc_id])
                    {
                        /* SES-508 Fix Start */
                        /* Code Removed */
                        /* SES-508 Fix End */
                        p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.drb_tunnel_info_count++;
                    }
                }

                if (PNULL !=  p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
                          p_uecc_drb_fwd_tnl_list[p_create_drb_status->lc_id])
                {

                    fwd_tunnel_count = p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
                        p_uecc_drb_fwd_tnl_list[p_create_drb_status->lc_id]->tunnel_info_count;

                    p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
                        p_uecc_drb_fwd_tnl_list[p_create_drb_status->lc_id]->
                        uecc_fwd_tnl_list[fwd_tunnel_count].rb_direction = p_rrc_uecc_llim_s1u_response->
                        tunnel_info_cnf[tunnel_info_cnf_count].rb_direction;

                    p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
                        p_uecc_drb_fwd_tnl_list[p_create_drb_status->lc_id]->
                        uecc_fwd_tnl_list[fwd_tunnel_count].teid_self = p_rrc_uecc_llim_s1u_response->
                        tunnel_info_cnf[tunnel_info_cnf_count].teid_self;

                   l3_memcpy_wrapper (&p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
                          p_uecc_drb_fwd_tnl_list[p_create_drb_status->lc_id]->
                           uecc_fwd_tnl_list[fwd_tunnel_count].transport_addr,
                          &p_rrc_uecc_llim_s1u_response->tunnel_info_cnf[tunnel_info_cnf_count].
                           transport_addr, sizeof(rrc_uecc_llim_s1u_gsn_addr_t));

                   p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.
                       p_uecc_drb_fwd_tnl_list[p_create_drb_status->lc_id]->tunnel_info_count++;
                }
                /* Bug 817 Fix Stop */
                
                if (((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) ||
                        (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
                        && (rb_direction < MAX_FWD_TUNNELS_PER_ERAB))
                {
                    pp_erab_trg_ho_forward_list[erab_id]->
                        x2ap_drb_forward_info[rb_direction].teid_self.numocts
                        = sizeof(rrc_gtp_teid_t);

                    /*l3_memcpy_wrapper(pp_erab_trg_ho_forward_list[erab_id]->
                            x2ap_drb_forward_info[rb_direction].teid_self.data,
                            &p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                            [tunnel_info_cnf_count].teid_self,
                            sizeof(rrc_gtp_teid_t));*/
#ifdef LTE_L3_BIG_ENDIAN
                    pp_erab_trg_ho_forward_list[erab_id]->x2ap_drb_forward_info[rb_direction].teid_self.data
                        = 
                        p_rrc_uecc_llim_s1u_response->tunnel_info_cnf[tunnel_info_cnf_count].teid_self;
#elif LTE_L3_LITTLE_ENDIAN
                    pp_erab_trg_ho_forward_list[erab_id]->x2ap_drb_forward_info[rb_direction].teid_self.data[0]
                        =
                        (U8)(p_rrc_uecc_llim_s1u_response->tunnel_info_cnf[tunnel_info_cnf_count].teid_self>>24
                             & 0xFF);
                    pp_erab_trg_ho_forward_list[erab_id]->x2ap_drb_forward_info[rb_direction].teid_self.data[1]
                        =
                        (U8)(p_rrc_uecc_llim_s1u_response->tunnel_info_cnf[tunnel_info_cnf_count].teid_self>>16
                             & 0xFF);
                    pp_erab_trg_ho_forward_list[erab_id]->x2ap_drb_forward_info[rb_direction].teid_self.data[2]
                        =
                        (U8)(p_rrc_uecc_llim_s1u_response->tunnel_info_cnf[tunnel_info_cnf_count].teid_self>>8
                             & 0xFF);
                    pp_erab_trg_ho_forward_list[erab_id]->x2ap_drb_forward_info[rb_direction].teid_self.data[3]
                        =
                        (U8)(p_rrc_uecc_llim_s1u_response->tunnel_info_cnf[tunnel_info_cnf_count].teid_self>>0
                             & 0xFF);
#endif

                    pp_erab_trg_ho_forward_list[erab_id]->
                        x2ap_drb_forward_info[rb_direction].
                        transportLayerAddress.numbits =
                        (OSUINT32)(p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                                   [tunnel_info_cnf_count].
                                   transport_addr.data_length *
                                   RRC_OCTET_SIZE);

                    /* transportLayerAddress */
                    pp_erab_trg_ho_forward_list[erab_id]->
                        x2ap_drb_forward_info[rb_direction].
                        transportLayerAddress.data =
                        (OSOCTET*)rtxMemAlloc(&p_uecc_ue_context->
                                              s1ap_asn1_ctx,
                                              p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                                              [tunnel_info_cnf_count].transport_addr
                                              .data_length);

                    if (!pp_erab_trg_ho_forward_list[erab_id]->
                            x2ap_drb_forward_info[rb_direction].
                            transportLayerAddress.data)
                    {
                        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                                p_uecc_ue_context->p_gb_context->facility_name,
                                RRC_ERROR, "ASN malloc failed.");
                        RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                        return RRC_FAILURE;
                    }
                    l3_memcpy_wrapper(((void*)pp_erab_trg_ho_forward_list[erab_id]->
                                x2ap_drb_forward_info[rb_direction].
                                transportLayerAddress.data),
                            (
                             p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                             [tunnel_info_cnf_count].transport_addr.data),
                            p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                            [tunnel_info_cnf_count].transport_addr
                            .data_length);
                }
                else if (rb_direction < MAX_FWD_TUNNELS_PER_ERAB)
                {
                    pp_erab_trg_ho_forward_list[erab_id]->
                        drb_forward_info[rb_direction].teid_self.numocts
                        = sizeof(rrc_gtp_teid_t);

                    /*l3_memcpy_wrapper(pp_erab_trg_ho_forward_list[erab_id]->
                            drb_forward_info[rb_direction].teid_self.data,
                            &p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                            [tunnel_info_cnf_count].teid_self,
                            sizeof(rrc_gtp_teid_t));*/
                    rrc_pack_U32(pp_erab_trg_ho_forward_list[erab_id]->
                            drb_forward_info[rb_direction].teid_self.data,
                            &p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                            [tunnel_info_cnf_count].teid_self);

                    pp_erab_trg_ho_forward_list[erab_id]->
                        drb_forward_info[rb_direction].
                        transportLayerAddress.numbits =
                        (OSUINT32)(p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                                   [tunnel_info_cnf_count].
                                   transport_addr.data_length * 
                                   RRC_OCTET_SIZE);

                    /* transportLayerAddress */
                    pp_erab_trg_ho_forward_list[erab_id]->
                        drb_forward_info[rb_direction].
                        transportLayerAddress.data = 
                        (OSOCTET*)rtxMemAlloc(&p_uecc_ue_context->
                                              s1ap_asn1_ctx,
                                              p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                                              [tunnel_info_cnf_count].transport_addr
                                              .data_length);

                    if (!pp_erab_trg_ho_forward_list[erab_id]->
                            drb_forward_info[rb_direction].
                            transportLayerAddress.data)
                    {
                        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                                p_uecc_ue_context->p_gb_context->facility_name,
                                RRC_ERROR, "ASN malloc failed.");
                        RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                        return RRC_FAILURE;
                    }

                    l3_memcpy_wrapper(((void*)pp_erab_trg_ho_forward_list[erab_id]->
                                drb_forward_info[rb_direction].
                                transportLayerAddress.data),
                            (
                             p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                             [tunnel_info_cnf_count].transport_addr.data),
                            p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                            [tunnel_info_cnf_count].transport_addr
                            .data_length);
                }
            }
            else
            {
                /* If the tunnel whose creation succeded is not
                   one the forwarding tunnel then update
                   the teid_self and transportLayerAddress_self*/

                p_uecc_ue_context->p_e_rab_list[erab_id]
                    ->teid_self = 
                    p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                    [tunnel_info_cnf_count].teid_self;

                p_uecc_ue_context->p_e_rab_list[erab_id]->transportLayerAddress_self 
                    .numbits =
                    (OSUINT32)(p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                    [tunnel_info_cnf_count].
                    transport_addr.data_length * 
                    RRC_OCTET_SIZE);
                
                p_uecc_ue_context->p_e_rab_list[erab_id]->transportLayerAddress_self 
                    .data = (OSOCTET*)rtxMemAlloc(&p_uecc_ue_context->
                                          s1ap_asn1_ctx,
                                          p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                                          [tunnel_info_cnf_count].transport_addr
                                          .data_length);

                if (!  p_uecc_ue_context->p_e_rab_list[erab_id]->transportLayerAddress_self 
                    .data)
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name,
                            RRC_ERROR, "ASN malloc failed.");
                    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }
                l3_memcpy_wrapper(((void*)p_uecc_ue_context->p_e_rab_list[erab_id]->
                            transportLayerAddress_self.data),
                        (
                         p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                         [tunnel_info_cnf_count].transport_addr.data),
                        p_rrc_uecc_llim_s1u_response->tunnel_info_cnf
                        [tunnel_info_cnf_count].transport_addr
                        .data_length);

            }
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_fail_action_set_state_delete_ue
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Changes state to delete ue entity ongoin 
 ****************************************************************************/
void uecc_fsm_ue_trg_ho_fail_action_set_state_delete_ue(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END");

    UECC_FSM_SET_CHILD_STATE(
            p_uecc_ue_context,
            &uecc_fsm_trg_ho_delete_ue_entity_ongoing, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_rrm_adm_ongoing_ue_rel_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_UE_RELEASE_REQ message.
 ****************************************************************************/
void uecc_fsm_trg_rrm_adm_ongoing_ue_rel_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Procedural timer stopped in finish_failure */

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);
    
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: Sending S1AP UE CONTEXT REL COMPLETE");
    if(RRC_FAILURE != uecc_s1ap_build_and_send_ue_ctx_release_comp_resp(
                p_uecc_ue_context))
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name,
                RRC_ERROR,
                "[%s] Fail to Send UE CONTEXT RELEASE COMPLETE RESPONSE.", __FUNCTION__);
    }

    /* notify UE FSM about ue ctx delete */
    /* Post event to UE FSM for future processing */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_mac_rach_res_get_ongoing_x2ap_ho_cancel_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handler for CSC delete all UE request.
                    Actions:
                    (a) Stop procedural timer
                    (b) Send UECC_LLIM_HO_REL_RACH_RESOURCE_IND to LLIM
                    (c) Push UECC_EV_TRG_HO_FAILURE_INT to Main FSM
 ****************************************************************************/
void uecc_fsm_trg_mac_rach_res_get_ongoing_x2ap_ho_cancel_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received HO CANCEL\n");

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    /* Procedural timer stopped in finish_failure */

    uecc_fsm_start_failure_processing(
        p_uecc_ue_context,
        uecc_fsm_ue_trg_ho_fail_type8_actions);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_rrm_adm_ongoing_x2ap_ho_cancel_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handler for CSC delete all UE request.
                    Actions:
                    (a) Stop procedural timer
                    (b) Push UECC_EV_TRG_HO_FAILURE_INT to Main FSM
 ****************************************************************************/
void uecc_fsm_trg_rrm_adm_ongoing_x2ap_ho_cancel_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received  HO CANCEL\n");

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    /* Procedural timer stopped in finish_failure */

    uecc_fsm_start_failure_processing(
        p_uecc_ue_context,
        uecc_fsm_ue_trg_ho_fail_type11_actions);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_mac_rach_res_get_ongoing_ue_rel_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_X2AP_HO_CANCEL message.
 ****************************************************************************/
void uecc_fsm_trg_mac_rach_res_get_ongoing_ue_rel_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* Procedural timer stopped in finish_failure */

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RACH REL RES IND(Fail)");

    /* Send UECC_LLIM_HO_REL_RACH_RESOURCE_IND to LLIM */
    uecc_llim_build_and_send_rrc_uecc_llim_ho_rach_rel_resource_ind (
            p_uecc_ue_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: END SENDING RRM HO ADM CNF (Fail)");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "FAILURE: Sending S1AP UE CONTEXT REL COMPLETE");
    if(RRC_FAILURE != uecc_s1ap_build_and_send_ue_ctx_release_comp_resp(
            p_uecc_ue_context))
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name,
                RRC_ERROR,
                "[%s] Fail to Send UE CONTEXT RELEASE COMPLETE RESPONSE.", __FUNCTION__);
    }

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    /* notify UE FSM about ue ctx delete */
    /* Post event to UE FSM for future processing */
    uecc_fsm_finish_activity_failure(
        p_uecc_ue_context, (void*)UECC_EV_TRG_HO_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  :  uecc_fsm_trg_x2ap_ho_cancel_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_X2AP_HO_CANCEL message.
 ****************************************************************************/
void uecc_fsm_trg_x2ap_ho_cancel_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*uecc_fsm_start_failure_processing(
        p_uecc_ue_context,
        uecc_fsm_ue_trg_ho_fail_type9_actions);*/

/*BUG 604 changes start*/
/*BUG 604 changes stop*/
    
    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf_failure(
            p_uecc_ue_context);

    /*Delete UE context at X2AP*/
    if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_fsm_activity_state_t* p_current_state = PNULL;
        p_current_state=
            p_uecc_ue_context->uecc_fsm.proc_descriptor.p_current_state;

        if(!((p_current_state == &uecc_fsm_trg_ho_mac_rach_res_get_ongoing)||
            (p_current_state == &uecc_fsm_trg_ho_create_ue_entity_ongoing)))
        {
            uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
        }
            /* Bug#21403 start */
        if (p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.drb_tunnel_info_count > RRC_NULL)
        {
        RRC_UECC_TRG_HO_FSM_TRACE (RRC_WARNING, "Deleting forwarding tunnels [%d] ",
                            p_uecc_ue_context->rrc_trg_ho_fwd_tnl_info.drb_tunnel_info_count);
        /* Delete all the forwarding tunnels */
        if ( RRC_SUCCESS != uecc_llim_build_and_send_delete_trg_forwarding_tunnel_req(p_uecc_ue_context))
        {
            RRC_UECC_TRG_HO_FSM_TRACE (RRC_ERROR, "Either no forwarding tunnel exist or"
            " delivery of S1U_RECONFIGURE_REQ gets failed.");
        }
        }
        /* Bug#21403 end */
    }

    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    if(RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
            p_trg_ho_info->is_rrc_conn_reconf_comp_recv)
    {
        SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
        
        p_uecc_ue_context->share_data.uecc_rcr_data.
            rrc_connection_release_not_required_flag = RRC_FALSE;

        /* notify UE FSM about csc cell delete */
        /* Post event to UE FSM for future processing */
        uecc_fsm_finish_activity_failure(
                p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
    }
    else
    {
        UECC_FSM_SET_CHILD_STATE(p_uecc_ue_context,
                &uecc_fsm_trg_ho_delete_ue_entity_ongoing, PNULL);
    }



    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_compl_ongoing_rrm_ue_connection_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND message.
 ****************************************************************************/
static void uecc_fsm_trg_ho_compl_ongoing_rrm_ue_connection_release_ind_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api)
{
    s1ap_Cause cause;
    rrc_rrm_ue_connection_release_ind_t *p_ue_connection_release_ind =
        (rrc_rrm_ue_connection_release_ind_t *)p_api;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    p_uecc_ue_context->share_data.uecc_rcr_data.
                rrc_connection_release_not_required_flag = RRC_FALSE;

    /*Delete UE context at X2AP*/
    if ( X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
    }
    /*SPR_17348_START*/
    if ((RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
                         p_trg_ho_info->is_path_switch_req_ack_recv) || 
            (S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
    {
        if (RRC_SUCCESS == uecc_s1ap_build_s1ap_cause(
                    &cause,&p_ue_connection_release_ind->release_cause,p_uecc_ue_context))
        {
            if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                        p_uecc_ue_context,
                        &cause))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_ERROR,
                        "[%s]Unable to build and send enb_ue_ctx_release_req ",__FUNCTION__);
            }
            /*Finish child FSM by posting
              UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT event to main FSM. */
            uecc_fsm_finish_activity_failure(
                    p_uecc_ue_context, (void*)UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT);
        }
    /*SPR_17348_END*/
        else
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[%s] Failed to fill s1ap cause. " ,__FUNCTION__);
        }
    }
    else
    {
        SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
        
        /*Finish child FSM by posting UECC_EV_TRG_HO_RELEASE_REQ_INT event to main FSM. */
        uecc_fsm_finish_activity_failure(
                p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_rach_res_ongoing_rrm_ue_connection_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND message.
 ****************************************************************************/
static void uecc_fsm_trg_ho_rach_res_ongoing_rrm_ue_connection_release_ind_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api)
{
    rrc_rrm_ue_connection_release_ind_t *p_ue_connection_release_ind =
        (rrc_rrm_ue_connection_release_ind_t *)p_api;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    /*SPR_17348_START*/
    if ((RRC_RRM_UE_CONNECTION_RELEASE_IND_REDIRECTED_CARRIER_INFO_PRESENT |
                RRC_RRM_UE_CONNECTION_RELEASE_IND_IDLE_MODE_MOBILITY_CONTROL_PRESENT)
            & p_ue_connection_release_ind->bitmask)
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
                "[%s] Ignoring Redirected Carrier Info",
                __FUNCTION__);
    }

    if (X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        /*SPR 16609 FIX START*/
        p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork; 
        p_uecc_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
        /*SPR 16609 FIX END*/
    }
    else if(S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork; 
        p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
    }
    /* No need to send UE release request to RRM */ 
    p_uecc_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag =
        RRC_TRUE;

    uecc_trg_ho_rach_res_ongoing_rrm_ue_connection_release_ind_handler(p_uecc_ue_context);

    /*SPR_17348_END*/

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_llim_ongoing_rrm_ue_connection_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND message.
 ****************************************************************************/
static void uecc_fsm_trg_ho_llim_ongoing_rrm_ue_connection_release_ind_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api)
{
    rrc_rrm_ue_connection_release_ind_t *p_ue_connection_release_ind =
        (rrc_rrm_ue_connection_release_ind_t *)p_api;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

/*SPR_17348_START*/    
    if ((RRC_RRM_UE_CONNECTION_RELEASE_IND_REDIRECTED_CARRIER_INFO_PRESENT |
                RRC_RRM_UE_CONNECTION_RELEASE_IND_IDLE_MODE_MOBILITY_CONTROL_PRESENT)
            & p_ue_connection_release_ind->bitmask)
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
                "[%s] Ingonring Redirected Carrier Info",
                __FUNCTION__);
    }
    if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        /*SPR 16609 FIX START*/
        p_uecc_ue_context->ho_info.x2ap_cause.t = T_x2ap_Cause_radioNetwork; 
        p_uecc_ue_context->ho_info.x2ap_cause.u.radioNetwork = x2ap_unspecified_2; 
        /*SPR 16609 FIX END*/
    }
    else if(S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork; 
        p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
    }

    /* No need to send UE release request to RRM */ 
    p_uecc_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag =
        RRC_TRUE;

    uecc_trg_ho_llim_ongoing_rrm_ue_connection_release_ind_handler(p_uecc_ue_context);
    /*spr_22003_changes_start*/
    /*As HO admission has been failed at target, so no need to increment the
     * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
    p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
    /*spr_22003_changes_end*/

    /*SPR_17348_END*/    

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_exec_ongoing_rrm_ue_connection_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND message.
 ****************************************************************************/
static void uecc_fsm_trg_ho_exec_ongoing_rrm_ue_connection_release_ind_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api)
{
    rrc_rrm_ue_connection_release_ind_t *p_ue_connection_release_ind =
        (rrc_rrm_ue_connection_release_ind_t *)p_api;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

/*SPR_17348_START*/    
    if ((RRC_RRM_UE_CONNECTION_RELEASE_IND_REDIRECTED_CARRIER_INFO_PRESENT |
                RRC_RRM_UE_CONNECTION_RELEASE_IND_IDLE_MODE_MOBILITY_CONTROL_PRESENT)
            & p_ue_connection_release_ind->bitmask)
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
                "[%s] Ignoring Redirected Carrier Info present since Handover"
                "Preparation phase is ongoing",
                __FUNCTION__);
    }

    uecc_trg_ho_exec_ongoing_rrm_ue_connection_release_ind_handler(p_uecc_ue_context);
/*SPR_17348_END*/    

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_check_and_process_x2ap_security_capability
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*                                   p_api
 *                  HandoverPreparationInformation_r8_IEs*  p_rrc_container
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This is the function processes UE security capability IE
 *                  and configures UE ciphering and integrity protection algs
 ****************************************************************************/
rrc_return_t uecc_fsm_trg_ho_check_and_process_x2ap_security_capability(
        uecc_ue_context_t*                      p_uecc_ue_context,
        void*                                   p_api,
        HandoverPreparationInformation_r8_IEs*  p_rrc_container
)
{
    rrc_return_t            result = RRC_FAILURE;
    U16     security_algorithms = UECC_CIPHERING_EEA0_BIT;
    U8      algo_index  = 0;
    U8      mme_integrity_algo = 0;
    U8      mme_ciphering_algo = 0;
    U8      integrity_algo_flag = 0;
    U8      ciphering_supported = 0;
    rrc_bool_t  ims_emergency_support = RRC_FALSE;
    x2ap_UESecurityCapabilities *p_ue_security_capabilities
            = (x2ap_UESecurityCapabilities *)p_api;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context);

    /*SPR_17484_START*/ 
    /* Code Removed */
    /*SPR_17484_END*/ 
    do
    {
        /*
         * Check and select ciphering alg
         * */

        if (0 !=
            rtxTestBit(p_ue_security_capabilities->encryptionAlgorithms.data,
                    p_ue_security_capabilities->encryptionAlgorithms.numbits,0))
        {
            /* 128-EEA1 */
            security_algorithms |= UECC_CIPHERING_EEA1_BIT;
        }

        if (0 !=
            rtxTestBit(p_ue_security_capabilities->encryptionAlgorithms.data,
                    p_ue_security_capabilities->encryptionAlgorithms.numbits,1))
        {
            /* 128-EEA2 */
            security_algorithms |= UECC_CIPHERING_EEA2_BIT;
        }
        
        for (algo_index = 0; algo_index < p_uecc_ue_context->p_gb_context->
                supported_security_algo_per_cell.
                supported_security_algorithms[p_uecc_ue_context->cell_index].
                ciphering_algorithms.num_algorithms; algo_index++)
        {
            mme_ciphering_algo = (U8)(1 << p_uecc_ue_context->p_gb_context->
                   supported_security_algo_per_cell.
                   supported_security_algorithms[p_uecc_ue_context->cell_index].
                   ciphering_algorithms.algorithms[algo_index]);
        
            /* Check its presence in MME side */
            if (mme_ciphering_algo & security_algorithms)
            {
                p_uecc_ue_context->configure_ciphering_user_plane.algorithm_id =
                    p_uecc_ue_context->p_gb_context->
                    supported_security_algo_per_cell.
                    supported_security_algorithms[p_uecc_ue_context->
                    cell_index].ciphering_algorithms.algorithms[algo_index];
                p_uecc_ue_context->configure_ciphering.algorithm_id =
                    p_uecc_ue_context->p_gb_context->
                    supported_security_algo_per_cell.
                    supported_security_algorithms[p_uecc_ue_context->cell_index].
                    ciphering_algorithms.algorithms[algo_index];

                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "Ciphering algorithms %d selected",
                    p_uecc_ue_context->configure_ciphering.algorithm_id);
                    /* Set ciphering supported flag */
                ciphering_supported = 1;
                break;    
            }
        }
        if(!ciphering_supported)
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, 
                "Ciphering algorithm not matched between oam and mme");
            break;
        }
        /*
         * Check and select integrity protection alg
         * */
        if ( (PNULL != p_rrc_container) && 
               (1 == p_rrc_container->m.as_ConfigPresent) && 
                (eia0_v920 == p_rrc_container->as_Config.
                  sourceSecurityAlgorithmConfig.integrityProtAlgorithm) )
        {
            security_algorithms = UECC_INTEGRITY_PROTECTION_EIA0_BIT;
        }
        else
        {
            security_algorithms = RRC_NULL;
            if (0 !=
                    rtxTestBit(
                        p_ue_security_capabilities->integrityProtectionAlgorithms.data,
                        p_ue_security_capabilities->integrityProtectionAlgorithms.numbits,
                        0))
            {
                /* 128-EIA1 */
                security_algorithms |= UECC_INTEGRITY_PROTECTION_EIA1_BIT;
            }

            if (0 !=
                    rtxTestBit(
                        p_ue_security_capabilities->integrityProtectionAlgorithms.data,
                        p_ue_security_capabilities->integrityProtectionAlgorithms.numbits,
                        1))
            {
                /* 128-EIA2 */
                security_algorithms |= UECC_INTEGRITY_PROTECTION_EIA2_BIT;
            }
        }

        if ( (0 == security_algorithms) &&
           (0 == p_uecc_ue_context->p_gb_context->supported_security_algo_per_cell.
            supported_security_algorithms[p_uecc_ue_context->cell_index].integrity_algorithms.num_algorithms))
        {
            /* MME don't provide valid integrity protection alg- Failure */
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "MME don't provide valid integrity protection algorithms");
            break;
        }

        for (algo_index = 0; algo_index < p_uecc_ue_context->p_gb_context->
                supported_security_algo_per_cell.
                supported_security_algorithms[p_uecc_ue_context->cell_index].
                integrity_algorithms.num_algorithms;algo_index++)
        {
            mme_integrity_algo = (U8)(1 << p_uecc_ue_context->p_gb_context->
                    supported_security_algo_per_cell.
                    supported_security_algorithms[p_uecc_ue_context->cell_index].
                    integrity_algorithms.algorithms[algo_index]);

            /* Check its presence in MME side */
            if(mme_integrity_algo & security_algorithms)
            {
                p_uecc_ue_context->configure_integrity_protection.algorithm_id =
                    p_uecc_ue_context->p_gb_context->supported_security_algo_per_cell.
                    supported_security_algorithms[p_uecc_ue_context->cell_index].
                    integrity_algorithms.algorithms[algo_index];
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                        "Integrity protection algorithms %d selected",
                        p_uecc_ue_context->configure_integrity_protection.algorithm_id);
                integrity_algo_flag = 1;
                break;
            }
        }

        if (RRC_PDCP_INT_ALG_EIA0 == p_uecc_ue_context->configure_integrity_protection.algorithm_id)
        {
            if (PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context[
                    p_uecc_ue_context->cell_index])
            {
                if (PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context[
                        p_uecc_ue_context->cell_index]->p_csc_init_setup_ind)
                {
                    if (SIB_TYPE_1_NON_CRITICAL_EXTENSION_PRESENT_FLAG & 
                            p_uecc_ue_context->p_gb_context->p_p_csc_context[
                            p_uecc_ue_context->cell_index]->p_csc_init_setup_ind->
                            sib_type_1_Info.presence_bitmask)
                    {
                        if (SIB_TYPE_1_V890_IES_NON_CROTICAL_EXTENSION_PRESENT_FLAG &
                                p_uecc_ue_context->p_gb_context->p_p_csc_context[
                                p_uecc_ue_context->cell_index]->p_csc_init_setup_ind->
                                sib_type_1_Info.non_critical_extension.presence_bitmask)
                        {
                            if (SIB_TYPE_1_V920_IES_IMS_EMERGENCY_SUPPORT_PRESENT_FLAG &
                                    p_uecc_ue_context->p_gb_context->p_p_csc_context[
                                    p_uecc_ue_context->cell_index]->p_csc_init_setup_ind->
                                    sib_type_1_Info.non_critical_extension.non_critical_extension.
                                    presence_bitmask)
                            {
                                ims_emergency_support = RRC_TRUE;
                            }
                        }
                    }

                    if ( (RRC_TRUE == ims_emergency_support) &&
                            (0 == integrity_algo_flag) )
                    {
                        /* eNodeB doesnot support EIA0 while provides IMS Emergency Support */
                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                                "eNodeB doesnot support EIA0 while provides IMS Emergency Support");
                        break;
                    }
                    if ( (RRC_FALSE == ims_emergency_support) &&
                            (1 == integrity_algo_flag) )
                    {
                        /* eNodeB doesnot IMS Emergency while MME supports EIA0 integrity algo */
                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                                "eNodeB doesnot provide IMS Emergency Support while supports EIA0 algo");
                        break;
                    }
                    if ( (1 == p_uecc_ue_context->m.rel_of_mme_present) &&
                            (RRC_MME_REL8 == p_uecc_ue_context->rel_of_mme) )
                    {
                        /* MME is REL8 so doesnot support EIA0 */
                        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                                "MME is of REL8 so doesnot support EIA0");
                        break;
                    }
                }
            }
        }

        if( 0 == integrity_algo_flag )
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "Integrity protection algorithms not matching by OAM");
            break;
        }

        result = RRC_SUCCESS;
    }while (0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}

/*SPR_17143_START*/
/*Code Removed*/
/*SPR_17143_END*/

#ifdef RRC_DEBUG
/**** FOR TESTING PURPOSE ONLY ***********/

/*****************************************************************************
 * Function Name  : trg_ho_debug_print_erab_info 
 * Inputs         : uecc_ue_context_t *  p_uecc_ue_context
 * Outputs        : None
 * Returns        : None
 * Description    : This function prints debug info for handover.
 ********************************************************************************/
void trg_ho_debug_print_erab_info(uecc_ue_context_t * p_uecc_ue_context)
{
    uecc_ho_drb_forward_info_list_t          
        **pp_erab_trg_ho_forward_list = PNULL;
    uecc_e_rab_failed_to_add_list_t 
        *p_uecc_e_rab_failed_to_add_list = PNULL;
    int i;

    p_uecc_e_rab_failed_to_add_list = 
        &p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                s1_trg_ho_info.erab_failed_list;

    pp_erab_trg_ho_forward_list = &p_uecc_ue_context->ho_info.
        p_ho_info->p_trg_ho_info->s1_trg_ho_info.
        p_erab_trg_ho_forward_list[0];

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "%s:ERAB INFO in UE CONTEXT count %d",__FUNCTION__,p_uecc_ue_context->e_rab_list_count);

    for (i =0; i < MAX_ERAB_COUNT; i++)
    {
        if (p_uecc_ue_context->p_e_rab_list[i])
        {

            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "%s:ERAB ID %d DRB ID %d LC ID %d qci %d :teid self 0x%x  tx_addr_self:numbits %d 0x%2x%2x%2x%2x teid_peer:numocts %d 0x%2x%2x%2x%2x tx_addr_peer:numbits %d 0x%2x%2x%2x%2x",
                    __FUNCTION__,
                    p_uecc_ue_context->p_e_rab_list[i]->e_RAB_ID,
                    p_uecc_ue_context->p_e_rab_list[i]->drb_config.drb_id,
                    p_uecc_ue_context->p_e_rab_list[i]->drb_config.logical_channel_identity,
                    p_uecc_ue_context->p_e_rab_list[i]->e_RABlevelQoSParameters.qCI,
                    p_uecc_ue_context->p_e_rab_list[i]->teid_self,
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress_self.numbits,
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress_self.numbits ?
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress_self.data[0]:0,
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress_self.numbits ?
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress_self.data[1]:0,
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress_self.numbits ?
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress_self.data[2]:0,
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress_self.numbits ?
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress_self.data[3]:0,
                    p_uecc_ue_context->p_e_rab_list[i]->gTP_TEID.numocts,
                    p_uecc_ue_context->p_e_rab_list[i]->gTP_TEID.data[0],
                    p_uecc_ue_context->p_e_rab_list[i]->gTP_TEID.data[1],
                    p_uecc_ue_context->p_e_rab_list[i]->gTP_TEID.data[2],
                    p_uecc_ue_context->p_e_rab_list[i]->gTP_TEID.data[3],
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress.numbits,
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress.data[0],
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress.data[1],
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress.data[2],
                    p_uecc_ue_context->p_e_rab_list[i]->transportLayerAddress.data[3]);
        }
    }

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "%s:ERAB FAILED TO ADD LIST::count %d",__FUNCTION__,
            p_uecc_e_rab_failed_to_add_list->count);

    for(i =0; i < p_uecc_e_rab_failed_to_add_list->count;i++)
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "%s:ERAB ID %d: cause t %d v %d",
                __FUNCTION__,
                p_uecc_e_rab_failed_to_add_list-> e_rab_failed_to_add_list[i].e_rab_id,
                p_uecc_e_rab_failed_to_add_list-> e_rab_failed_to_add_list[i].cause.t,
                p_uecc_e_rab_failed_to_add_list-> e_rab_failed_to_add_list[i].cause.u.radioNetwork);
    }


    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "%s:ERAB TRG HO FWD LIST::count %d",__FUNCTION__,
            p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->s1_trg_ho_info.trg_ho_forward_list_count);

    for (i =0; i < MAX_ERAB_COUNT; i++)
    {
        if (pp_erab_trg_ho_forward_list[i])
        {

            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "%s:ERAB ID %d fwd direction %d ",
                    __FUNCTION__,
                    i,
                    pp_erab_trg_ho_forward_list[i]->forward_direction);

            if (UECC_FORWARDING_DIRECTION_DL == pp_erab_trg_ho_forward_list[i]->forward_direction ||
                    UECC_FORWARDING_DIRECTION_BOTH == pp_erab_trg_ho_forward_list[i]->forward_direction)
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "drb tunnel 1 info teid_self:numocts %d 0x%2x%2x%2x%2x tx_addr_self:numbits %d 0x%2x%2x%2x%2x",
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].teid_self.numocts,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].teid_self.numocts ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].teid_self.data[0]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].teid_self.numocts ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].teid_self.data[1]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].teid_self.numocts ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].teid_self.data[2]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].teid_self.numocts ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].teid_self.data[3]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].transportLayerAddress.numbits,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].transportLayerAddress.numbits ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].transportLayerAddress.data[0]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].transportLayerAddress.numbits ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].transportLayerAddress.data[1]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].transportLayerAddress.numbits ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].transportLayerAddress.data[2]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].transportLayerAddress.numbits ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[0].transportLayerAddress.data[3]:0);
            }

            if (UECC_FORWARDING_DIRECTION_UL == pp_erab_trg_ho_forward_list[i]->forward_direction ||
                    UECC_FORWARDING_DIRECTION_BOTH == pp_erab_trg_ho_forward_list[i]->forward_direction)
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "drb tunnel 2 info teid_self:numocts %d 0x%2x%2x%2x%2x tx_addr_self:numbits %d 0x%2x%2x%2x%2x",
                         pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].teid_self.numocts,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].teid_self.numocts ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].teid_self.data[0]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].teid_self.numocts ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].teid_self.data[1]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].teid_self.numocts ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].teid_self.data[2]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].teid_self.numocts ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].teid_self.data[3]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].transportLayerAddress.numbits,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].transportLayerAddress.numbits ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].transportLayerAddress.data[0]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].transportLayerAddress.numbits ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].transportLayerAddress.data[1]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].transportLayerAddress.numbits ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].transportLayerAddress.data[2]:0,
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].transportLayerAddress.numbits ?
                        pp_erab_trg_ho_forward_list[i]->drb_forward_info[1].transportLayerAddress.data[3]:0);
           }

        }
    }
    return;
}
#endif

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_rrm_adm_ongoing_x2ap_reset_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handler for CSC delete all UE request.
                    Actions:
                    (a) Stop procedural timer
                    (b) Push UECC_EV_TRG_HO_FAILURE_INT to Main FSM
 ****************************************************************************/
static void uecc_fsm_trg_ho_rrm_adm_ongoing_x2ap_reset_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received  X2AP RESET\n");
    if (X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        /*spr_22003_changes_start*/
        /*As HO admission has been failed at target, so no need to increment the
         * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
        p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
        /*spr_22003_changes_end*/
        /* Procedural timer stopped in finish_failure */
        uecc_fsm_start_failure_processing(
            p_uecc_ue_context,
        /*SPR_17592_START*/
            uecc_fsm_ue_trg_ho_fail_type11_actions);
        /*SPR_17592_END*/
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_mac_rach_res_get_ongoing_x2ap_reset_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handler for CSC delete all UE request.
                    Actions:
                    (a) Stop procedural timer
                    (b) Send UECC_LLIM_HO_REL_RACH_RESOURCE_IND to LLIM
                    (c) Push UECC_EV_TRG_HO_FAILURE_INT to Main FSM
 ****************************************************************************/
static void uecc_fsm_trg_ho_mac_rach_res_get_ongoing_x2ap_reset_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received X2AP RESET \n");

    /* Procedural timer stopped in finish_failure */

    if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        /*spr_22003_changes_start*/
        /*As HO admission has been failed at target, so no need to increment the
         * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
        p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
        /*spr_22003_changes_end*/

        /* SPR 16149 Fix Start*/
        uecc_fsm_start_failure_processing(
            p_uecc_ue_context,
            /*SPR_17591_START*/
            uecc_fsm_ue_trg_ho_fail_type15_actions);
            /*SPR_17591_END*/
        /* SPR 16149 Fix Stop*/ 
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_x2ap_reset_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_X2AP_HO_CANCEL message.
 ****************************************************************************/
static void uecc_fsm_trg_ho_x2ap_reset_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*uecc_fsm_start_failure_processing(
        p_uecc_ue_context,
        uecc_fsm_ue_trg_ho_fail_type9_actions);*/

    if((INTER_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover)||
        (INTRA_CELL_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover))
    { 
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,"Invalid event received");
        return;
    }
    /*Delete UE context at X2AP*/
    if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        uecc_fsm_activity_state_t* p_current_state = PNULL;
        p_current_state=
            p_uecc_ue_context->uecc_fsm.proc_descriptor.p_current_state;

        /*SPR_17592_START*/
        /*RRC_RRM_HO_ADM_CNF to RRM*/
        uecc_rrm_build_and_send_ue_ho_adm_cnf_failure(
                p_uecc_ue_context);
        /*SPR_17592_END*/

        /*spr_22003_changes_start*/
        /*As HO admission has been failed at target, so no need to increment the
         * counter ERAB.RelSuccNbr.QCI as mentioned in 3GPP, 32.425*/
        p_uecc_ue_context->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_FALSE;
        /*spr_22003_changes_end*/

        if(!((p_current_state == &uecc_fsm_trg_ho_mac_rach_res_get_ongoing)||
            (p_current_state == &uecc_fsm_trg_ho_create_ue_entity_ongoing)))
        {
            uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
        }

        UECC_FSM_SET_CHILD_STATE(p_uecc_ue_context,
                &uecc_fsm_trg_ho_delete_ue_entity_ongoing, PNULL);
    }                           

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_path_switch_ongoing_uplink_tunnel_reconfig_x2ap_reset_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_X2AP_HO_CANCEL message.
 ****************************************************************************/
static void uecc_fsm_trg_ho_path_switch_ongoing_uplink_tunnel_reconfig_x2ap_reset_req_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    if(X2_HO ==  p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
           "Received X2AP RESET and now updating the x2ap" 
           "reset flag in the UECC ue context \n");
        p_uecc_ue_context->x2ap_reset_recvd_flag = RRC_TRUE;        
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

}
        /*BUG 604 changes start*/
/*BUG 604 changes stop*/

/*BUG 604 changes start*/
/**********************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_fail_action_send_enb_ue_ctx_release_req 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : The funtion sends UE CONTEXT RELEASE REQUEST to S1AP.
 **********************************************************************************/
void uecc_fsm_ue_trg_ho_fail_action_send_enb_ue_ctx_release_req(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
)
{
    s1ap_Cause   cause;

    RRC_ASSERT(PNULL != p_uecc_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "SENDING UE CONTEXT RELEASE REQUEST");

    /* Fill Ue Release cause */
    cause.t= T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_unspecified_2;

    /*Send UE CONTEXT RELEASE REQUEST message to S1AP */
    if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                p_uecc_ue_context,
                &cause))
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
            p_uecc_ue_context->p_gb_context->facility_name,
            RRC_ERROR,
            "[%s] Fail to Send UE CONTEXT RELEASE REQUEST message to S1AP." 
            ,__FUNCTION__);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/* UL NAS START */
/****************************************************************************
* Function Name  : uecc_fsm_trg_ho_ul_nas_transport_handler 
* Inputs         : uecc_ue_context_t* - pointer to UE context
*                  void*   - api data
* Outputs        : None
* Returns        : None
* Description    : Sends UL NAS TRANSPORT msg to MME
****************************************************************************/
void uecc_fsm_trg_ho_ul_nas_transport_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    /*SPR_16416_START*/
    ULInformationTransfer_r8_IEs_dedicatedInfoType *p_dedicatedInfoType = 
	    (ULInformationTransfer_r8_IEs_dedicatedInfoType*)p_api;
    /*SPR_16416_END*/

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);

    /* Pass NAS data to S1AP using RRC_S1AP_UL_NAS_TRANSPORT_REQ
     * message */
    if (RRC_SUCCESS != uecc_s1ap_build_and_send_ul_nas_transport_req(
    /*SPR_16416_START*/
                p_uecc_ue_context, p_dedicatedInfoType->u.dedicatedInfoNAS))
    /*SPR_16416_END*/
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                "[s1ap_uplinkNASTransport] delivery fail");
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

}

/* UL NAS STOP */

/*SPR_16416_START*/
/****************************************************************************
* Function Name  : uecc_fsm_trg_ho_ul_nas_transport_before_psa_handler
* Inputs         : uecc_ue_context_t* - pointer to UE context
*                  void*   - api data
* Outputs        : None
* Returns        : None
* Description    : For X2 HO, Queue the UL NAS TRANSPORT message received at TeNB before 
*                  Path Switch. For S1 HO, sends UL NAS TRANSPORT msg to MME
****************************************************************************/
void uecc_fsm_trg_ho_ul_nas_transport_before_psa_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    ULInformationTransfer_r8_IEs_dedicatedInfoType *p_dedicatedInfoType = 
	    (ULInformationTransfer_r8_IEs_dedicatedInfoType*)p_api;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);

    /* Pass NAS data to S1AP using RRC_S1AP_UL_NAS_TRANSPORT_REQ
     * message */
    /*SPR_16590_START*/
    /* Queue the UL NAS messages received before
     * Path Switch Ack or Fail */
    if((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) &&
            (RRC_TRUE != p_uecc_ue_context->ho_info.p_ho_info->
             p_trg_ho_info->is_path_switch_req_ack_recv))
    /*SPR_16590_END*/
    {
        uecc_event_queue_push_ul_nas_transport_event(
                UECC_EV_S1AP_UL_NAS_TRANSPORT_REQ, 
                p_uecc_ue_context, 
                p_dedicatedInfoType->u.dedicatedInfoNAS, 
                &(p_uecc_ue_context->p_ul_nas_transport_queue));
    }
    else
    {
        if (RRC_SUCCESS != uecc_s1ap_build_and_send_ul_nas_transport_req(
                    p_uecc_ue_context, p_dedicatedInfoType->u.dedicatedInfoNAS))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[s1ap_uplinkNASTransport] delivery fail");
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

}
/*SPR_16416_END*/
/*SPR 18758 Fix Start*/
/****************************************************************************
* Function Name  : uecc_fsm_trg_ho_dl_nas_transport_handler 
* Inputs         : uecc_ue_context_t* - pointer to UE context
*                  void*   - api data
* Outputs        : None
* Returns        : None
* Description    : Sends DL NAS TRANSPORT msg to MME
****************************************************************************/
void uecc_fsm_trg_ho_dl_nas_transport_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);

    rrc_s1ap_downlink_nas_transport_t *p_downlink_nas_transport = PNULL;
    p_downlink_nas_transport = (rrc_s1ap_downlink_nas_transport_t*)p_api;
    s1ap_Cause s1ap_cause;

    /* Pass NAS data to UE through MAC using
     * UECC_LLIM_SRB_DATA_REQ(DLInformationTransfer) message */
    if (RRC_SUCCESS != uecc_llim_build_and_send_rrc_dl_information_transfer(
                            p_uecc_ue_context,
                            &p_downlink_nas_transport->nas_pdu,
	T_DLInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoNAS) )
    {
        s1ap_cause.t = T_s1ap_Cause_radioNetwork;
        s1ap_cause.u.radioNetwork = s1ap_unspecified_2;

        if (RRC_SUCCESS != uecc_s1ap_build_and_send_nas_non_delivery_ind(
                                p_uecc_ue_context->p_gb_context,
                                p_downlink_nas_transport->mme_ue_s1ap_id,
                                p_downlink_nas_transport->enb_ue_s1ap_id,
                                &p_downlink_nas_transport->nas_pdu,
                                &s1ap_cause))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[s1ap_NASNonDeliveryIndication] delivery fail");
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
* Function Name  : uecc_fsm_trg_ho_nas_non_delievery_handler 
* Inputs         : uecc_ue_context_t* - pointer to UE context
*                  void*   - api data
* Outputs        : None
* Returns        : None
* Description    : Sends NAS NON DELIEVERY msg to MME
****************************************************************************/
void uecc_fsm_trg_ho_nas_non_delievery_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    
    rrc_uecc_llim_srb_data_status_ind_t *p_llim_srb_data_status_ind = PNULL;
    s1ap_Cause          s1ap_cause;
    s1ap_NAS_PDU        s1ap_nas_pdu;
    DedicatedInfoNAS*   p_nas_pdu = PNULL;
    OSCTXT              asn1_ctx;
    
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    p_llim_srb_data_status_ind = (rrc_uecc_llim_srb_data_status_ind_t*)p_api;

    if ( RRC_SUCCESS != p_llim_srb_data_status_ind->response_code )
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING, "NAS DL message delivery fail.");

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
                p_uecc_ue_context,
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
                p_uecc_ue_context->p_gb_context,
                uecc_ue_ctx_get_mme_ue_id(p_uecc_ue_context),
                p_uecc_ue_context->s1ap_ue_id,
                &s1ap_nas_pdu,
                &s1ap_cause) )
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                "[s1ap_NASNonDeliveryIndication] delivery fail");
        }

        if (PNULL != p_nas_pdu)
        {
            /* If NAS PDU was retrieved - Free ASN Context */
            rtFreeContext(&asn1_ctx);
        }
        else
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                "[s1ap_NASNonDeliveryIndication] NAS PDU has not been found");
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

}

/****************************************************************************
* Function Name  : uecc_fsm_trg_ho_dl_cdma2000_tunneling_handler 
* Inputs         : uecc_ue_context_t* - pointer to UE context
*                  void*   - api data
* Outputs        : None
* Returns        : None
* Description    : Sends DL CDMA2000 TUNNELING msg to MME
****************************************************************************/
void uecc_fsm_trg_ho_dl_cdma2000_tunneling_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{


    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    rrc_s1ap_downlink_s1_cdma2000_tunneling_t *p_downlink_s1_cdma2000_tunneling = PNULL;
    s1ap_Cause s1ap_cause;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    
    p_downlink_s1_cdma2000_tunneling = 
        (rrc_s1ap_downlink_s1_cdma2000_tunneling_t*)p_api;
    if(s1ap_onexRTT == p_downlink_s1_cdma2000_tunneling->cdma2000_rat_type)
    {
        if(RRC_SUCCESS != uecc_llim_build_and_send_rrc_dl_information_transfer(
                    p_uecc_ue_context,
                    &p_downlink_s1_cdma2000_tunneling->cdma2000_pdu,
                    T_DLInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_1XRTT))
        {
            s1ap_cause.t = T_s1ap_Cause_radioNetwork;
            s1ap_cause.u.radioNetwork = s1ap_unspecified_2;

            if (RRC_SUCCESS != uecc_s1ap_build_and_send_nas_non_delivery_ind(
                        p_uecc_ue_context->p_gb_context,
                        p_downlink_s1_cdma2000_tunneling->mme_ue_s1ap_id,
                        p_downlink_s1_cdma2000_tunneling->enb_ue_s1ap_id,
                        &p_downlink_s1_cdma2000_tunneling->cdma2000_pdu,
                        &s1ap_cause))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[s1ap_NASNonDeliveryIndication] delivery fail");
            }
        }
    }
    else if(s1ap_hRPD == p_downlink_s1_cdma2000_tunneling->cdma2000_rat_type)
    {
        if(RRC_SUCCESS != uecc_llim_build_and_send_rrc_dl_information_transfer(
                    p_uecc_ue_context,
                    &p_downlink_s1_cdma2000_tunneling->cdma2000_pdu,
                    T_DLInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoCDMA2000_HRPD))
        {
            s1ap_cause.t = T_s1ap_Cause_radioNetwork;
            s1ap_cause.u.radioNetwork = s1ap_unspecified_2;

            if (RRC_SUCCESS != uecc_s1ap_build_and_send_nas_non_delivery_ind(
                        p_uecc_ue_context->p_gb_context,
                        p_downlink_s1_cdma2000_tunneling->mme_ue_s1ap_id,
                        p_downlink_s1_cdma2000_tunneling->enb_ue_s1ap_id,
                        &p_downlink_s1_cdma2000_tunneling->cdma2000_pdu,
                        &s1ap_cause))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[s1ap_NASNonDeliveryIndication] delivery fail");
            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

}
/*SPR 18758 Fix Stop*/

/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_w_mme_status_and_rrc_conn_reconfig_comp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE message.
 ****************************************************************************/
void uecc_fsm_trg_ho_w_mme_status_and_rrc_conn_reconfig_comp_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    void*               p_api
)
{
    rrc_return_t result = RRC_FAILURE;
    uecc_fsm_fail_handler_f const*         p_failure_actions_list =
        uecc_fsm_ue_trg_ho_fail_type5_actions; 
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received RRC CONN RECONF COMPLETE\n");

    do {
        /*SPR_20529_START*/
        /* Update DRX and PHR config for RELEASE in MAC Main Config*/
        if(p_uecc_ue_context->m.mac_config_present == 1)
        {
            if((p_uecc_ue_context->mac_config.bitmask & RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT) &&
                    (p_uecc_ue_context->mac_config.mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT) &&
                    /* Coverity 113027 Fix Start */
                    (p_uecc_ue_context->mac_config.mac_main_config.drx_config.drx_config_type == RRC_DRX_CONFIG_TYPE_REL))
                    /* Coverity 113027 Fix Stop */
            {
                p_uecc_ue_context->mac_config.mac_main_config.bitmask &= ~(RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT);
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                        "DRX Config Released");
            }
            if((p_uecc_ue_context->mac_config.bitmask & RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT) &&
                    (p_uecc_ue_context->mac_config.mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT) &&
                    (!(p_uecc_ue_context->mac_config.mac_main_config.phr_config.bitmask & RRC_PHR_CONFIG_PARAM_PRESENT)))
            {
                p_uecc_ue_context->mac_config.mac_main_config.bitmask &= ~(RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT);
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                        "PHR Config Released");
            }
        }
        /*SPR_20529_END*/
        
/*BUG 604 changes start*/
        if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
            (INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
        {
          /*SPR_20720_START*/
          if ((X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) &&
              (PNULL != g_rrc_stats.p_rrc_stats) &&
	    	  (RRC_FALSE == p_uecc_ue_context->m.rrc_connection_count_updated))
          /*SPR_20720_END*/
          {
              RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].
                    rrc_connection_related_stats.
                    rrcConnectionNumber, 0, DUMMY_RANGE_CHECK_INDEX);
	    	    
	          p_uecc_ue_context->m.rrc_connection_count_updated = RRC_TRUE;
          }
          /* SPR 17391 Fix Stop */
            /*SPR_17893_START*/
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "Sending UECC LLIM DATA BUFFER STOP IND\n");

            /*Send UECC_LLIM_STOP_BUFFER_IND to LLIM*/
            if (RRC_SUCCESS != uecc_llim_build_and_send_data_buffer_stop_ind(
                        p_uecc_ue_context))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s]uecc_llim_build_and_send_data_buffer_stop_ind "
                        "failed !!",__FUNCTION__);
                p_failure_actions_list = 
                    uecc_fsm_ue_trg_ho_fail_type5_actions;
                break;
            }
            /*SPR_17893_END*/
        
          if (RRC_TRUE == 
                p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->is_sn_status_transfer_ind_recv)
          {
            /* Change its state to UECC_TRG_HO_PATH_SWITCH_ONGOING */
            UECC_FSM_SET_CHILD_STATE(
                    p_uecc_ue_context,
                    &uecc_fsm_trg_ho_path_switch_ongoing, PNULL);
          }
          p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->is_rrc_conn_reconf_comp_recv = 
                RRC_TRUE;
        }
        else if (S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
        {
            /* SPR 20549 Fix Start */
            /* Code Deleted */
            /* SPR 20549 Fix End */
          if (RRC_TRUE == 
                p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->is_mme_status_transfer_ind_recv)
          {

                        /*SPR_20720_START*/
            if ((PNULL != g_rrc_stats.p_rrc_stats) &&
                (RRC_FALSE == p_uecc_ue_context->m.rrc_connection_count_updated))
            /*SPR_20720_END*/
            {
                RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_uecc_ue_context->cell_index].
                    rrc_connection_related_stats.
                    rrcConnectionNumber, 0, DUMMY_RANGE_CHECK_INDEX);
	    	    
	        	p_uecc_ue_context->m.rrc_connection_count_updated = RRC_TRUE;
            }
            /* SPR 17391 Fix Stop */

            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "Sending S1AP HO NOTIFY\n");
        
            /*Send HO NOTIFY message to MME */
            if (RRC_SUCCESS != uecc_s1ap_build_and_send_handover_notify (
                        p_uecc_ue_context))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s]uecc_s1ap_build_and_send_handover_notify"
                        " failed !!",__FUNCTION__);
                break;
            }

            RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                    "Sending UECC LLIM DATA BUFFER STOP IND\n");

            /*SPR_18028_START*/
            /*Send UECC_LLIM_STOP_BUFFER_IND to LLIM*/
            if (RRC_SUCCESS != uecc_llim_build_and_send_data_buffer_stop_ind(
                        p_uecc_ue_context))
            {     
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s]uecc_llim_build_and_send_data_buffer_stop_ind "
                        "failed !!",__FUNCTION__);
                p_failure_actions_list = 
                    uecc_fsm_ue_trg_ho_fail_type5_actions;
                break;
            }     
            /*SPR_18028_END*/

            /*SPR_17893_START*/
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
                "Sending UECC RRM UE HO ADM CNF");
        /* RRC_RRM_HO_ADM_CNF to RRM.*/
        if (RRC_FAILURE == uecc_rrm_build_and_send_ue_ho_adm_cnf(
                    p_uecc_ue_context,
                    RRC_SUCCESS))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s] uecc_rrm_build_and_send_ue_ho_adm_cnf failed ",
                    __FUNCTION__);
            p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
            p_failure_actions_list = 
                uecc_fsm_ue_trg_ho_fail_type5_actions;
            break;
        }

        uecc_fsm_finish_activity_success(p_uecc_ue_context, PNULL);
        /*SPR_17893_END*/
        }
        if ( (PNULL != p_uecc_ue_context->ho_info.p_ho_info) &&
               (PNULL != p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info) )
        {
            p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->is_rrc_conn_reconf_comp_recv = 
                RRC_TRUE;
        }
      }  
        result = RRC_SUCCESS;
    }while(0);

    if (result != RRC_SUCCESS)
    {
        uecc_fsm_start_failure_processing(
                p_uecc_ue_context,
                p_failure_actions_list);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/*SPR:6462 start*/
/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_reestablish_ue_entity_complete_cnf_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_REESTAB_UE_ENTITY_COMP_CNF message.
 ****************************************************************************/
void uecc_fsm_trg_ho_reestablish_ue_entity_complete_cnf_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api
        )
{
	/* coverity_fix_64026_start */
	rrc_return_t result = RRC_FAILURE;
	uecc_fsm_fail_handler_f const* p_failure_actions_list =
		uecc_fsm_ue_trg_ho_fail_type5_actions; 
	rrc_uecc_llim_re_establish_ue_entity_complete_cnf_t 
		*p_rrc_uecc_llim_reestab_ue_entity_comp_cnf
		= (rrc_uecc_llim_re_establish_ue_entity_complete_cnf_t*)p_api;
	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
			"Received LLIM_REESTABLISH_UE_ENTITY_COMPLETE_CNF \n");
	do {

		if (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
		{
			/* SPR:8308 start */
			p_uecc_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag = RRC_TRUE;
			/* SPR:8308 stop */
			if(RRC_SUCCESS == p_rrc_uecc_llim_reestab_ue_entity_comp_cnf->response_code) 
			{
				/*Send UECC_LLIM_STOP_BUFFER_IND to LLIM*/
				if (RRC_SUCCESS != uecc_llim_build_and_send_data_buffer_stop_ind(
							p_uecc_ue_context))
				{
					RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
							"[%s]uecc_llim_build_and_send_data_buffer_stop_ind "
							"failed !!",__FUNCTION__);
					p_failure_actions_list = 
						uecc_fsm_ue_trg_ho_fail_type14_actions;
					break;
				}
				/*if  Intra cell HO is invoked by UCM security rekeying
				 * procedure then surpress RRC_RRM_HO_ADM_CNF */
				/*BUG 604 changes start*/
				/*securitykey*/
				RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
						"Sending UECC RRM UE HO ADM CNF\n");
				/* RRC_RRM_HO_ADM_CNF to RRM.*/
				if (RRC_FAILURE == uecc_rrm_build_and_send_ue_ho_adm_cnf(
							p_uecc_ue_context,
							RRC_SUCCESS))
				{
					RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
							"[%s] uecc_rrm_build_and_send_ue_ho_adm_cnf failed ",
							__FUNCTION__);
					p_failure_actions_list = 
						uecc_fsm_ue_trg_ho_fail_type12_actions;
					break;
				}
				/*BUG 604 changes stop*/
				/*securitykey*/
				uecc_fsm_finish_activity_success(p_uecc_ue_context, PNULL);
			}
			else
			{
				p_failure_actions_list = uecc_fsm_ue_trg_ho_fail_type14_actions;
				break;
			}
		}
		result = RRC_SUCCESS;
	}while(0);

	if (result != RRC_SUCCESS)
	{
		uecc_fsm_start_failure_processing(
				p_uecc_ue_context,
				p_failure_actions_list);
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/* coverity_fix_64026_stop */
/*SPR:6462 stop*/


/* Bug 8198 Fix Start */
/****************************************************************************
 * Function Name  : uecc_fsm_trg_ho_s1ap_allocate_mme_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_S1AP_ALLOCATE_MME_RESP
 ****************************************************************************/
void uecc_fsm_trg_ho_s1ap_allocate_mme_resp_handler(
        uecc_ue_context_t   *p_uecc_ue_context,
        void                *p_api_data )
{
    s1ap_allocate_mme_resp_t     *p_allocate_mme_resp = PNULL;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_uecc_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api_data);

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    p_allocate_mme_resp = (s1ap_allocate_mme_resp_t*)p_api_data;

    if(RRC_INVALID_MME_ID == p_allocate_mme_resp->mme_id)
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                "[%s]MME is not allocated during path switch procedure"
                " sending failure",__FUNCTION__);

        uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf(
                p_uecc_ue_context, (rrc_bool_et *)RRC_TRUE);

        /* SPR_17847_START*/
        /*Delete UE context at X2AP*/
        if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
        {
            uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
        }
        /* SPR_17847_END*/

        uecc_fsm_finish_activity_failure(
                p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
    }
    else
    {
        if (RRC_SUCCESS != uecc_s1ap_build_and_send_path_switch_request(
                    p_uecc_ue_context,p_allocate_mme_resp->mme_id))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s]uecc_s1ap_build_and_send_path_switch_request"
                    " failed !!",__FUNCTION__);

            uecc_fsm_ue_trg_ho_fail_action_send_rrm_ho_adm_cnf(
                    p_uecc_ue_context, (rrc_bool_et *)RRC_TRUE);
            /* SPR_17847_START*/
            /*Delete UE context at X2AP*/
            if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
            {
                uecc_x2ap_build_and_send_ue_release_ind(p_uecc_ue_context);
            }
            /* SPR_17847_END*/

            uecc_fsm_finish_activity_failure(
                    p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/* Bug 8198 Fix End */

/* Bug 799 Fix Start */
/****************************************************************************
 * Function Name  : rrc_delete_ho_ue_identity_node
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Function to delete HO Node containing re-establishment
 *                  information.
 ****************************************************************************/
void rrc_delete_ho_ue_identity_node(
    uecc_ue_context_t*  p_uecc_ue_context
)
{
    /*SPR_16680_START*/
    /* Code Removed */
    /*SPR_16680_END*/
    ho_ongoing_ue_identity_search_node_t *p_ue_identity_st_node = PNULL;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    if ((PNULL !=  p_uecc_ue_context->ho_info.p_ho_info) &&
            (PNULL !=  p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info))
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "Deleting HO Node");

            /*SPR_16680_START*/
            for(p_ue_identity_st_node = (ho_ongoing_ue_identity_search_node_t*)(st_get_first_node
                ((const search_tree_t *)&(p_uecc_ue_context->
                p_gb_context->ho_ongoing_ue_identity_search_tree))); 
                p_ue_identity_st_node; 
                p_ue_identity_st_node = (ho_ongoing_ue_identity_search_node_t*)(st_get_next_node
                ((const search_tree_node_t *) p_ue_identity_st_node)))
            {
                if(p_ue_identity_st_node->ue_index == p_uecc_ue_context->ue_index)
                {
                    /* Matching Node found. So, quit from 'for' loop */
                    break;
                }
            }
            /*SPR_16680_END*/

            if (PNULL != p_ue_identity_st_node)
            {
                /* Now Free entry from tree */
                st_delete_node(&p_uecc_ue_context->p_gb_context->
                        ho_ongoing_ue_identity_search_tree,
                        &p_ue_identity_st_node->nodeAnchor);

                if (PNULL != p_ue_identity_st_node)
                {
                    rrc_mem_free (p_ue_identity_st_node);
                    p_ue_identity_st_node = PNULL;
                }

            /*SPR_16680_START*/
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO, "HO Node - Deleted ");
            }
    /*SPR_16680_END*/
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/* Bug 799 Fix Stop */
/*BUG 604 changes start*/
/****************************************************************************
 * Function Name  : uecc_fsm_trg_s1u_local_path_switch_cnf_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_S1U_LOCAL_PATH_SWITCH_CNF message.
 ****************************************************************************/
static
void uecc_fsm_trg_s1u_local_path_switch_cnf_handler(
        uecc_ue_context_t*  p_uecc_ue_context,
        void*               p_api
        )
{
    rrc_return_t result = RRC_SUCCESS;
    rrc_uecc_llim_s1u_local_path_switch_cnf_t*
        p_rrc_uecc_llim_s1u_path_switch_cnf = PNULL;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRG_HO_FSM_TRACE(RRC_INFO,
            "Received S1U PATH SWITCH CNF \n");

    p_rrc_uecc_llim_s1u_path_switch_cnf
        = (rrc_uecc_llim_s1u_local_path_switch_cnf_t*)p_api;

    do {/*BUG 10605 start*/
        if(RRC_SUCCESS == p_rrc_uecc_llim_s1u_path_switch_cnf->response)
        {/*BUG 10605 stop*/
            /* Send UE CONTEXT RELEASE to source eNB */ 
            if (RRC_SUCCESS != 
                    uecc_x2ap_build_and_send_ue_context_release(p_uecc_ue_context
                        ))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s]uecc_x2ap_build_and_send_ue_context_release"
                        " failed !!",__FUNCTION__);

                /* filling the cause in p_ue_context */
                p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
                result = RRC_FAILURE;
                break;
            }
/*SPR_17382_START*/
/* Code Removed */
/*SPR_17382_END*/
            /* Send RRC_RRM_HO_ADM_CNF (SUCCESS) to RRM */
            if (RRC_FAILURE == uecc_rrm_build_and_send_ue_ho_adm_cnf(
                        p_uecc_ue_context,
                        RRC_SUCCESS))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s] uecc_rrm_build_and_send_ue_ho_adm_cnf failed ",
                        __FUNCTION__);
                /* filling the cause in p_ue_context */
                p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
                result = RRC_FAILURE;
                break;
            }
            /*Send intra_cell_ho success indication to S1AP*/
            if (RRC_FAILURE == uecc_s1ap_build_and_send_intra_cell_ho_success_ind(
                        p_uecc_ue_context))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s] uecc_s1ap_build_and_send_intra_cell_ho_success_ind failed ",
                        __FUNCTION__);
                /* filling the cause in p_ue_context */
                p_uecc_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                p_uecc_ue_context->ho_info.cause.u.radioNetwork = s1ap_unspecified_2;
                result = RRC_FAILURE;
                break;
            }
        }
        else
        {

            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s]S1U_LOCAL_PATH_SWITCH_CNF failure returned from S1U!!"
                    ,__FUNCTION__);
            result = RRC_FAILURE;
            break;
        }
    }while(0);

    if (result != RRC_SUCCESS)
    {
            uecc_fsm_start_failure_processing(
                    p_uecc_ue_context,
                    uecc_fsm_ue_trg_ho_fail_type5_actions);
    }
    else
    {
       uecc_fsm_finish_activity_success(p_uecc_ue_context, PNULL);
       /*uecc_fsm_finish_activity_success(p_uecc_ue_context, 
                     (void*)UECC_EV_TRG_HO_SUCCESS_INT);*/
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/*BUG 604 changes stop*/

/*SPR_16416_START*/
/********************************************************************
 *
 *  FUNCTION NAME : uecc_process_ul_nas_transport_queue
 *
 *  INPUT         : uecc_gb_context_t*      p_uecc_gb_context
 *                  uecc_event_queue_t*     p_uecc_event_queue
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : None
 *
 *  DESCRIPTION   : This function processes UECC UL NAS event queue
 *
 *******************************************************************/
void uecc_process_ul_nas_transport_queue
(
    uecc_ue_context_t*      p_uecc_ue_context,
    uecc_event_queue_t*     p_uecc_event_queue  /* Pointer to UECC queue */
)
{
    uecc_event_t*       p_event = PNULL;
    DedicatedInfoNAS    *p_dedicated_ul_nas_info = PNULL;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_uecc_event_queue);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    while ( PNULL!=(p_event = uecc_event_queue_pop_event(p_uecc_event_queue)) )
    {
        /* Send event to corresponding FSM */
        if (RRC_SUCCESS != uecc_s1ap_build_and_send_ul_nas_transport_req(
                    p_uecc_ue_context, p_event->p_msg_info))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[s1ap_uplinkNASTransport] delivery fail");
        }

        /* Release the memory allocated for UL NAS message
         * in function uecc_event_queue_push_ul_nas_transport_event */
        p_dedicated_ul_nas_info = p_event->p_msg_info;
        if(PNULL != p_dedicated_ul_nas_info)
        {
            if(PNULL != p_dedicated_ul_nas_info->data)
            {
                rrc_mem_free((void *)(p_dedicated_ul_nas_info->data));
                p_dedicated_ul_nas_info->data = PNULL;
            }
            rrc_mem_free(p_dedicated_ul_nas_info);
            p_dedicated_ul_nas_info = PNULL;
            p_event->p_msg_info = PNULL;
        }

        /* Free event */
        rrc_mem_free(p_event);
        p_event = PNULL;
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/*SPR_17976_START*/
/********************************************************************
 *
 *  FUNCTION NAME : uecc_trg_is_path_switch_ongoing
 *
 *  INPUT         : uecc_ue_context_t* - pointer to UE contexte
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : rrc_bool_et
 *
 *  DESCRIPTION   : This function returns RRC_TRUE if UE is in path
 *                  switch ongoing state otherwise it returns RRC_FALSE
 *
 *******************************************************************/
rrc_bool_et uecc_trg_is_path_switch_ongoing
(
    uecc_ue_context_t*      p_uecc_ue_context
)
{
    if((p_uecc_ue_context->prev_ongoing_uecc_fsm.proc_descriptor.p_current_state
                == &uecc_fsm_trg_ho_path_switch_ongoing) || 
            (p_uecc_ue_context->prev_ongoing_uecc_fsm.proc_descriptor.p_current_state
             == &uecc_fsm_resume_proc_trg_ho_path_switch_ongoing)) 
    {
        RRC_UECC_TRG_HO_FSM_TRACE(RRC_DETAILED,
            "UE:%u is in path switch ongoing state", p_uecc_ue_context->ue_index);
        return RRC_TRUE;
    }
    return RRC_FALSE;
}
/*SPR_17976_END*/
/*SPR_20135_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_path_switch_radio_link_failure_ind_handler
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_LLIM_RADIO_LINK_FAILURE_IND  message
 *                  in PATH_SWITCH_ONGOING state.
 ****************************************************************************/
void uecc_fsm_ue_trg_ho_path_switch_radio_link_failure_ind_handler (
 uecc_ue_context_t   *p_uecc_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    s1ap_Cause    cause;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    /*This flag indicates that RRC connection Release will not be
     Sent to UE */
    p_uecc_ue_context->
    share_data.uecc_rcr_data.rrc_connection_release_not_required_flag =
    RRC_TRUE ;

    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
            p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
            "Inside the Handler for Radio Link failure"
            " in UE TRG HO Ongoing state ");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);
    p_uecc_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag = RRC_TRUE;


    if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        if (RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
                p_trg_ho_info->is_path_switch_req_ack_recv)
        {
            /* Fill Ue Release cause */
            cause.t= T_s1ap_Cause_radioNetwork;
            cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost;

            /*Send UE CONTEXT RELEASE REQUEST message to S1AP */
            if(RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                        p_uecc_ue_context,
                        &cause))
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,
                        RRC_ERROR,
                        "[%s] Fail to Send UE CONTEXT RELEASE REQUEST message to S1AP." 
                        ,__FUNCTION__);
            }

            /*Finish child FSM by posting
             * UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT event to main FSM. */
            uecc_fsm_finish_activity_failure(
                    p_uecc_ue_context, (void*)UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT);
        }
        else
        {
            if (RRC_SUCCESS != uecc_s1ap_build_and_send_ue_release_msg_req(
                        p_uecc_ue_context->s1ap_ue_id,
                        RRC_TRUE,
                        p_uecc_ue_context))
            {    
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,RRC_WARNING,
                        "ue_release_msg delivery failure");
            }    

            SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
            p_uecc_ue_context->ue_ctx_del_status.is_rrc_resp_expected = RRC_TRUE;
            if (RRC_SUCCESS != 
                    uecc_x2ap_build_and_send_ue_context_release(p_uecc_ue_context
                        ))
            {
                RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                        "[%s]uecc_x2ap_build_and_send_ue_context_release"
                        " failed !!",__FUNCTION__);
            }
            uecc_fsm_finish_activity_failure(
                    p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
        }
    }
    else if ((INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
    {
        SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
        /*Finish child FSM by posting
          UECC_EV_TRG_HO_RELEASE_REQ_INT event to main FSM. */
        uecc_fsm_finish_activity_failure(
                p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_trg_ho_radio_link_failure_ind_handler
 * Input          : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_LLIM_RADIO_LINK_FAILURE_IND  message
 *                  in TRG_HO_ONGOING states.
 ****************************************************************************/
void uecc_fsm_ue_trg_ho_radio_link_failure_ind_handler (
 uecc_ue_context_t   *p_uecc_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    s1ap_Cause    cause;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    /*This flag indicates that RRC connection Release will not be
     Sent to UE */
    p_uecc_ue_context->
    share_data.uecc_rcr_data.rrc_connection_release_not_required_flag =
    RRC_TRUE ;

    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
            p_uecc_ue_context->p_gb_context->facility_name,RRC_INFO,
            "Inside the Handler for Radio Link failure"
            " in UE TRG HO Ongoing state ");

    /*RRC_RRM_HO_ADM_CNF to RRM*/
    uecc_rrm_build_and_send_ue_ho_adm_cnf(
            p_uecc_ue_context,
            RRC_FAILURE);
    p_uecc_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag = RRC_TRUE;


    if ((INTER_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover))
    {
        SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
        /*Finish child FSM by posting
          UECC_EV_TRG_HO_RELEASE_REQ_INT event to main FSM. */
        uecc_fsm_finish_activity_failure(
                p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
    }
    else if ( (S1_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover) ||
            (RRC_TRUE == p_uecc_ue_context->ho_info.p_ho_info->\
             p_trg_ho_info->is_path_switch_req_ack_recv) )
    {
        /* Fill Ue Release cause */
        cause.t= T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost;

        /*Send UE CONTEXT RELEASE REQUEST message to S1AP */
        if(RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                    p_uecc_ue_context,
                    &cause))
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name,
                    RRC_ERROR,
                    "[%s] Fail to Send UE CONTEXT RELEASE REQUEST message to S1AP." 
                    ,__FUNCTION__);
        }

        /*Finish child FSM by posting
         * UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT event to main FSM. */
        uecc_fsm_finish_activity_failure(
                p_uecc_ue_context, (void*)UECC_EV_TRG_HO_WAIT_FOR_RELEASE_REQ_INT);
    }
    else if(X2_HO == p_uecc_ue_context->ho_info.s1_or_x2_handover)
    {
        SET_UE_CONTEXT_RELEASE_COMPLETE_NOT_REQ_FLAG(p_uecc_ue_context);
        if (RRC_SUCCESS != 
                uecc_x2ap_build_and_send_ue_context_release(p_uecc_ue_context
                    ))
        {
            RRC_UECC_TRG_HO_FSM_TRACE(RRC_WARNING,
                    "[%s]uecc_x2ap_build_and_send_ue_context_release"
                    " failed !!",__FUNCTION__);
        }
        uecc_fsm_finish_activity_failure(
                p_uecc_ue_context, (void*)UECC_EV_TRG_HO_RELEASE_REQ_INT);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/*SPR_20135_END*/


