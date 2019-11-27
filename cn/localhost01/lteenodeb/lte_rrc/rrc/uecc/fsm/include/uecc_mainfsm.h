/******************************************************************************
**  Filename :  uecc_uefsm.h
**
**  DESCRIPTION   :  This file contains the function prototypes for UECC UE FSM
**
**
**  DATE        AUTHOR      REF             REASON
**  --------    -------     ---             ----------
**  18-May-09   Pradeep S    DDv0.2          Initial
**
**     Copyright (C) 2009 Aricent Inc . All Rights Reservfsm
******************************************************************************/

#ifndef INCLUDED_UECC_UE_FSM_H
#define INCLUDED_UECC_UE_FSM_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/
extern uecc_fsm_activity_state_t uecc_fsm_ue_idle;


#define RRC_T2_TIMER_DURATION   1000
#define RRC_RLF_TIMER_DURATION  3000
extern uecc_fsm_handler_record_t uecc_fsm_ue_radio_link_failure_handling_ongoing_mht[];
extern void uecc_fsm_ue_s1ap_ul_nas_transport_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

extern void uecc_fsm_ue_s1ap_nas_non_delievery_ind_handler (
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

extern void uecc_fsm_ue_s1ap_dl_nas_transport_ind_handler (
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

extern  void uecc_fsm_ue_rrc_measurement_report_ind_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api                  /* Input API buffer */
);

extern void uecc_fsm_ue_radio_link_failure_handling_finish_failure(
     uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
     void                *p_api              /* Input API buffer */
);

rrc_return_t uecc_fsm_validate_antenna_codebook_subset_restriction(
      rrc_phy_antenna_information_dedicated_t *p_antenna_info,
      uecc_ue_context_t   *p_ue_context      /* UECC UE context */
);

extern void s1ap_generate_plmn_identity_to_int(
        plmn_identity_t    *p_plmn_identity,
        OSOCTET                 *p_s1ap_plmn_identity);
/*Bug_12240_fix_start*/
rrc_return_t uecc_fsm_validate_antenna_codebook_subset_restriction_r10(
        rrc_antenna_info_dedicated_r10_t *p_antenna_info,
        uecc_ue_context_t   *p_ue_context      /* UECC UE context */
);
/*Bug_12240_fix_stop*/

/*SPR_18241_START*/
rrc_return_et  uecc_fsm_process_ue_ctx_mod_msg(
     uecc_ue_context_t*  p_ue_context,
     curr_proc_data_t*   p_curr_proc_data,
     void*               p_api
);
/*SPR_18241_END*/

/*SPR_18582_FIX_START*/
rrc_return_et check_if_erab_id_already_present_in_failed_list( erab_item_t *p_erab_item,
                                                      s1ap_E_RAB_ID erab_id, uecc_ue_context_t *p_ue_context );
/*SPR_18582_FIX_END*/

#ifdef ENDC_ENABLED
void  uecc_fsm_x2ap_ue_context_release_handler
(
    uecc_ue_context_t*  p_ue_context
);
#endif
#endif /* INCLUDED_UECC_UE_FSM_H */

