/******************************************************************************
*
*   FILE NAME:
*       rrc_uemh.h
*
*   DESCRIPTION:
*       This header file contains declaration of in points of UEMH submodule
*
*   DATE            AUTHOR      REFERENCE       REASON
*   14 May 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_UEMH_H_
#define _RRC_UEMH_H_

#include "rrc_llim_global_ctx.h"
#include "rrc_llim_timers.h"

void rrc_uemh_uecc_llim_create_ue_entity_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_delete_ue_entity_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_configure_security_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_add_lc_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_srb_data_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_ccch_data_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_ue_con_rej_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_ue_con_reest_rej_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
/*REL 1.2:HANDOVER PROCEDURE START*/
void rrc_uemh_uecc_llim_sn_hfn_status_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_sn_hfn_status_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_mac_i_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_data_buffer_stop_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_s1u_reconfigure_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_ue_inactive_time_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_ho_rach_resource_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_ho_rel_rach_resource_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_re_establish_ue_entity_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_reestablish_ue_entity_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_uecc_llim_resume_ue_entity_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_uemh_uecc_llim_reconfig_security_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_uemh_uecc_llim_change_crnti_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
/*REL 1.2:HANDOVER PROCEDURE END*/

void rrc_uemh_phy_create_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_phy_delete_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_phy_reconfig_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_mac_create_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_mac_delete_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_mac_reconfigure_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_mac_ccch_msg_ind_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_rlc_create_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_rlc_reconfig_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_rlc_delete_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_pdcp_create_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_pdcp_delete_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_pdcp_reconfig_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_uemh_pdcp_srb_data_ind_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_pdcp_srb_data_status_ind_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_s1u_create_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_s1u_delete_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_s1u_reconfigure_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_oamh_cleanup_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_s1u_error_ind_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_s1u_path_failure_ind_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_s1u_path_success_ind_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_process_timer_msg
(
    rrc_timer_t             timer_id,
    rrc_llim_timer_buf_t    *p_rrc_llim_timer_buf,
    rrc_llim_gl_ctx_t       *p_rrc_llim_gl_ctx
);

/* JANEN_START */

void rrc_uemh_pdcp_mac_i_resp_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_pdcp_sn_hfn_status_resp_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
/*SPR_19066_Start*/
/*Code Deleted*/
/*SPR_19066_END*/
void rrc_uemh_s1u_end_marker_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);


/*JANEN_END*/
void rrc_uemh_pdcp_suspend_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx

);
void rrc_uemh_pdcp_re_establish_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx

);
void rrc_uemh_rlc_re_establish_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx

);
void rrc_uemh_pdcp_resume_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx

);
void rrc_uemh_phy_change_crnti_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx

);
void rrc_uemh_mac_change_crnti_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx

);
void rrc_uemh_rlc_change_crnti_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx

);
void rrc_uemh_pdcp_change_crnti_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx

);
void rrc_uemh_rlc_ue_entity_error_ind_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx

);


void rrc_uemh_mac_radio_link_failure_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_mac_ue_sync_status_ind_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_mac_reset_ue_entity_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
/*Pranab */
void  rrc_uemh_uecc_llim_suspend_ue_entity_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);


void rrc_uemh_uecc_llim_modify_lc_req_process_msg
(
void                *p_api,
rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
    
void rrc_uemh_uecc_llim_modify_roll_back_process_msg
(
void                *p_api,
rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
    
void rrc_uemh_uecc_llim_add_roll_back_process_msg
(
void                *p_api,
rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
    
/* ERB RELEASE COMMAND START */
void rrc_uemh_uecc_llim_delete_lc_req_process_msg
(
    void*                p_api,
    rrc_llim_gl_ctx_t*   p_rrc_llim_gl_ctx
);
/* ERB RELEASE COMMAND END */

void rrc_uemh_mac_ho_rach_resource_resp
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_uemh_uecc_llim_meas_config_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
/*SPR_19066_START*/
/*Code Deleted*/
/*SPR_19066_END*/
void rrc_uemh_uecc_llim_reset_ue_entity_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_pdcp_count_wraparound_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
/* SPS Start */
void rrc_uemh_uecc_llim_process_mac_reconfig_complete_ind_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
/* SPS Stop */

void rrc_uemh_uecc_llim_drb_count_msg_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_uemh_uecc_llim_reset_ue_entity_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);


/* SPR 4118 Start */
void rrc_uemh_pdcp_notify_integrity_failure
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_uemh_pdcp_drb_msb_resp
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

/* SPR 4118 Stop */
/*SPR:6462 start*/
void rrc_uemh_rlc_reestab_ue_entity_comp_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

/*SPR:6462 stop*/

/*BUG 604 Fixes start*/
void rrc_uemh_uecc_llim_s1u_intra_enb_data_fwd_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_uemh_uecc_llim_s1u_local_path_switch_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
/*SPR_19066_START*/
void rrc_uemh_uecc_llim_ho_prep_info_req_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 );
/*SPR_19066_END*/
void rrc_uemh_s1u_intra_enb_data_fwd_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_uemh_s1u_local_path_switch_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

/*BUG 604 Fixes stop*/
/*Bug 357 Fix Start*/
void rrc_uemh_uecc_llim_pdcp_re_establish_ue_entity_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
/*Bug 357 Fix Stop*/
/* Bug 839 Fix Start */
void uemh_send_mac_ho_rel_rach_res_ind
(
    void                *p_msg,             /* p_msg */
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
/* Bug 839 Fix Stop */

/* Bug 4450 Start */
void rrc_uemh_cleanup_ues
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
/* Bug 4450 End */

#endif /* _RRC_UEMH_H_ */
