/******************************************************************************
*
*   FILE NAME:
*       rrc_llmd.c
*
*   DESCRIPTION:
*       This is the main file of the LLMD module. This module provides
*       the main message handler and timer expiry handler functions of
*       RRC LLIM module.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   31 Mar 2009     VasylN      ---------       Initial
*   06 Nov 2009     VasylN      ---------       S1-U support added to LLIM
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "rrc_defines.h"
#include "rrc_llim_logger.h"
#include "rrc_common_utils.h"
#include "rrc_llim_timers.h"
#include "rrc_llim_global_ctx.h"
#include "rrc_msg_mgmt.h"

#include "rrc_ext_api.h"
#include "rrc_intrl_api.h"

#include "rrc_cemh.h"
#include "rrc_uemh.h"
#include "rrc_llim_oamh.h"
#include "rrc_handlers.h"
#include "rrc_llim_utils.h"

/*
*   Global variables
*/

/* Used in logger for identifying LLIM (just for avoid string duplication) */
const S8* rrc_llim_log_facility_name = (const S8*)"eNodeB RRC LLIM";

extern void rrc_uemh_uecc_llim_reest_ue_entity_comp_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
/*SPR_19066_START*/
void rrc_uemh_pdcp_ho_prep_info_resp_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_uemh_pdcp_inactive_ues_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
/*SPR_19066_END*/

/* UECC LLIM messages handlers */
static llim_intf_handlers_table_t p_llim_handlers_table[]=
{
    { { UECC_LLIM_CREATE_UE_ENTITY_REQ },
            rrc_uemh_uecc_llim_create_ue_entity_req_process_msg },
    { { UECC_LLIM_DELETE_UE_ENTITY_REQ },
            rrc_uemh_uecc_llim_delete_ue_entity_req_process_msg },
    { { UECC_LLIM_ADD_LC_REQ },
            rrc_uemh_uecc_llim_add_lc_req_process_msg },
    { { UECC_LLIM_SRB_DATA_REQ },
            rrc_uemh_uecc_llim_srb_data_req_process_msg },
    { { UECC_LLIM_CCCH_DATA_REQ },
            rrc_uemh_uecc_llim_ccch_data_req_process_msg },
    { { UECC_LLIM_UE_CON_REJ_REQ },
            rrc_uemh_uecc_llim_ue_con_rej_req_process_msg },
    { { UECC_LLIM_CONFIGURE_SECURITY_REQ },
            rrc_uemh_uecc_llim_configure_security_req_process_msg },
    { { UECC_LLIM_MODIFY_LC_REQ },
            rrc_uemh_uecc_llim_modify_lc_req_process_msg },
    { { UECC_LLIM_ADD_ROLLBACK_REQ },
            rrc_uemh_uecc_llim_add_roll_back_process_msg },
    { { UECC_LLIM_MODIFY_ROLLBACK_REQ },
            rrc_uemh_uecc_llim_modify_roll_back_process_msg },
    { { UECC_LLIM_DELETE_LC_REQ },
            rrc_uemh_uecc_llim_delete_lc_req_process_msg },
    { { UECC_LLIM_SUSPEND_UE_ENTITY_REQ },
            rrc_uemh_uecc_llim_suspend_ue_entity_req_process_msg },
    { { UECC_LLIM_SN_HFN_STATUS_REQ },
            rrc_uemh_uecc_llim_sn_hfn_status_req_process_msg },
    { { UECC_LLIM_DATA_BUFFER_STOP_IND },
            rrc_uemh_uecc_llim_data_buffer_stop_ind_process_msg },
    { { UECC_LLIM_SN_HFN_STATUS_IND },
            rrc_uemh_uecc_llim_sn_hfn_status_ind_process_msg },
    { { UECC_LLIM_MAC_I_REQ },
            rrc_uemh_uecc_llim_mac_i_req_process_msg },
    { { UECC_LLIM_S1U_RECONFIGURE_REQ },
            rrc_uemh_uecc_llim_s1u_reconfigure_req_process_msg },
    { { UECC_LLIM_HO_RACH_RESOURCE_REQ },
            rrc_uemh_uecc_llim_ho_rach_resource_req_process_msg },
    { { UECC_LLIM_HO_REL_RACH_RESOURCE_IND },
            rrc_uemh_uecc_llim_ho_rel_rach_resource_ind_process_msg },
    { { UECC_LLIM_UE_INACTIVE_TIME_REQ },
            rrc_uemh_uecc_llim_ue_inactive_time_req_process_msg },
    { { UECC_LLIM_RE_ESTABLISH_UE_ENTITY_REQ },
            rrc_uemh_uecc_llim_re_establish_ue_entity_req_process_msg },
    { { UECC_LLIM_RESUME_UE_ENTITY_REQ },
            rrc_uemh_uecc_llim_resume_ue_entity_req_process_msg },
    { { UECC_LLIM_RECONFIG_SECURITY_REQ },
            rrc_uemh_uecc_llim_reconfig_security_req_process_msg },
    { { UECC_LLIM_UE_CON_RE_ESTABLISH_REJ_REQ },
            rrc_uemh_uecc_llim_ue_con_reest_rej_req_process_msg },
    { { UECC_LLIM_REESTABLISH_UE_ENTITY_REQ },
            rrc_uemh_uecc_llim_reestablish_ue_entity_req_process_msg },
    { { UECC_LLIM_CHANGE_CRNTI_REQ },
            rrc_uemh_uecc_llim_change_crnti_req_process_msg },
    { { UECC_LLIM_MEAS_CONFIG_REQ },
            rrc_uemh_uecc_llim_meas_config_req_process_msg },
    { { UECC_LLIM_RESET_UE_ENTITY_REQ },
            rrc_uemh_uecc_llim_reset_ue_entity_req_process_msg },
    { { UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND },
            rrc_uemh_uecc_llim_reest_ue_entity_comp_ind_process_msg },
    { { UECC_LLIM_DRB_COUNT_MSB_REQ },
            rrc_uemh_uecc_llim_drb_count_msg_req_process_msg },
    { { UECC_LLIM_MAC_RECONFIG_COMPLETE_IND },
            rrc_uemh_uecc_llim_process_mac_reconfig_complete_ind_msg },
/*BUG 604 changes start*/
/*Bug 357 Fix Start*/
/*    { { UECC_LLIM_PDCP_RE_ESTABLISH_UE_ENTITY_REQ },
            rrc_uemh_uecc_llim_pdcp_re_establish_ue_entity_req_process_msg },*/
/*Bug 357 Fix Stop*/
    { { UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_REQ },
            rrc_uemh_uecc_llim_s1u_intra_enb_data_fwd_req_process_msg },
    { { UECC_LLIM_S1U_LOCAL_PATH_SWITCH_REQ },
            rrc_uemh_uecc_llim_s1u_local_path_switch_req_process_msg },
/*SPR_19066_START*/
    { { UECC_LLIM_HO_PREP_INFO_REQ },
            rrc_uemh_uecc_llim_ho_prep_info_req_process_msg },
/*SPR_19066_END*/
/*BUG 604 changes stop*/
    { { LLIM_LAST_ID }, PNULL }
};

/******************************************************************************
*   FUNCTION NAME: rrc_llmd_pdcp_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : None               
*   RETURNS      : None
*   DESCRIPTION  : This function passes PDCP messages to UEMH submodule
*                  based on incoming API id.
******************************************************************************/
static void rrc_llmd_pdcp_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(p_api != PNULL);
    RRC_ASSERT(p_rrc_llim_gl_ctx != PNULL);

    switch (rrc_get_api_id(p_api))
    {
    case RRC_PDCP_CREATE_UE_ENTITY_CNF:
        rrc_uemh_pdcp_create_ue_entity_cnf_process_msg(p_api,
            p_rrc_llim_gl_ctx);
        break;

    case RRC_PDCP_DELETE_UE_ENTITY_CNF:
        rrc_uemh_pdcp_delete_ue_entity_cnf_process_msg(p_api,
            p_rrc_llim_gl_ctx);
        break;

    case RRC_PDCP_RECONFIG_UE_ENTITY_CNF:
        rrc_uemh_pdcp_reconfig_ue_entity_cnf_process_msg(p_api,
            p_rrc_llim_gl_ctx);
        break;

    case RRC_PDCP_SRB_DATA_IND:
        rrc_uemh_pdcp_srb_data_ind_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_PDCP_SRB_DATA_STATUS_IND:
        rrc_uemh_pdcp_srb_data_status_ind_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;
     /* HANDOVER_JANEN_STRAT */
    case RRC_PDCP_MAC_I_RESP:
        rrc_uemh_pdcp_mac_i_resp_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_PDCP_SN_HFN_STATUS_RESP:
        rrc_uemh_pdcp_sn_hfn_status_resp_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;
     /* HANDOVER_JANEN_END */

    case RRC_PDCP_SUSPEND_UE_ENTITY_CNF:
        rrc_uemh_pdcp_suspend_ue_entity_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
        break ;

    case RRC_PDCP_RE_ESTABLISH_UE_ENTITY_CNF:
        rrc_uemh_pdcp_re_establish_ue_entity_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
        break ;
    case RRC_PDCP_RESUME_UE_ENTITY_CNF:
        rrc_uemh_pdcp_resume_ue_entity_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
        break ;
    case RRC_PDCP_CHANGE_CRNTI_CNF:
        rrc_uemh_pdcp_change_crnti_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
        break ;
    case RRC_PDCP_COUNT_WRAPAROUND_IND:
        rrc_uemh_pdcp_count_wraparound_ind_process_msg(p_api, p_rrc_llim_gl_ctx);
        break ;
    /* SPR 4118 Start */
    case RRC_PDCP_NOTIFY_INTEGRITY_FAILURE:
        rrc_uemh_pdcp_notify_integrity_failure(p_api, p_rrc_llim_gl_ctx);
        break;
    /* SPR 4118 End */
    case RRC_PDCP_DRB_COUNT_MSB_RESP:
        rrc_uemh_pdcp_drb_msb_resp(p_api, p_rrc_llim_gl_ctx);
        break ;


/*SPR_19066_START*/
    case RRC_PDCP_HO_PREP_INFO_RESP:
        rrc_uemh_pdcp_ho_prep_info_resp_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;
    case RRC_PDCP_INACTIVE_UES_IND:
        rrc_uemh_pdcp_inactive_ues_ind_process_msg(p_api,
                p_rrc_llim_gl_ctx);
        break;
/*SPR_19066_END*/
    default:
        RRC_LLIM_TRACE(RRC_WARNING, "rrc_llmd_pdcp_process_msg: unexpected API"
            " from PDCP module received");
        break;
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_llmd_rlc_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : None               
*   RETURNS      : None
*   DESCRIPTION  : This function passes RLC messages to UEMH submodule
*                  based on incoming API id.
******************************************************************************/
static void rrc_llmd_rlc_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(p_api != PNULL);
    RRC_ASSERT(p_rrc_llim_gl_ctx != PNULL);

    switch (rrc_get_api_id(p_api))
    {
    case RRC_RLC_CREATE_UE_ENTITY_CNF:
        rrc_uemh_rlc_create_ue_entity_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_RLC_RECONFIG_UE_ENTITY_CNF:
        rrc_uemh_rlc_reconfig_ue_entity_cnf_process_msg(p_api,
            p_rrc_llim_gl_ctx);
        break;

    case RRC_RLC_DELETE_UE_ENTITY_CNF:
        rrc_uemh_rlc_delete_ue_entity_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_RLC_UE_ENTITY_ERROR_IND:
         rrc_uemh_rlc_ue_entity_error_ind_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_RLC_RE_ESTABLISH_UE_ENTITY_CNF:
         rrc_uemh_rlc_re_establish_ue_entity_cnf_process_msg(p_api, 
             p_rrc_llim_gl_ctx);
        break;

    case RRC_RLC_CHANGE_CRNTI_CNF:
         rrc_uemh_rlc_change_crnti_cnf_process_msg(p_api, 
             p_rrc_llim_gl_ctx);
        break;
    /*SPR6462: start */
    case RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF: 
         rrc_uemh_rlc_reestab_ue_entity_comp_cnf_process_msg(p_api, 
             p_rrc_llim_gl_ctx);
        break;
    /*SPR6462: stop */
#ifdef LTE_EMBMS_SUPPORTED
    case RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_CNF: 
         rrc_cemh_rlc_create_mbms_area_cnf_process_msg(p_api, p_rrc_llim_gl_ctx); 
        break;
    case RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_CNF: 
         rrc_cemh_rlc_delete_mbms_area_cnf_process_msg(p_api, p_rrc_llim_gl_ctx); 
        break;
#endif
    default:
        RRC_LLIM_TRACE(RRC_WARNING, "rrc_llmd_rlc_process_msg: unexpected API "
            "from RLC module received");
        break;
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_llmd_mac_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : None               
*   RETURNS      : None
*   DESCRIPTION  : This function passes MAC messages to CEMH or UEMH submodule
*                  based on incoming API id.
******************************************************************************/
static void rrc_llmd_mac_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(p_api != PNULL);
    RRC_ASSERT(p_rrc_llim_gl_ctx != PNULL);

    switch (rrc_get_api_id(p_api))
    {
        case RRC_MAC_CONFIG_CELL_CNF:
            rrc_cemh_mac_config_cell_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;
        case RRC_MAC_SFN_CNF:
            rrc_cemh_mac_sfn_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_MAC_SFN_IND:
            rrc_cemh_mac_sfn_ind_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_MAC_DELETE_CELL_CNF:
            rrc_cemh_mac_delete_cell_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_MAC_CREATE_UE_ENTITY_CNF:
            rrc_uemh_mac_create_ue_entity_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_MAC_DELETE_UE_ENTITY_CNF:
            rrc_uemh_mac_delete_ue_entity_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_MAC_RECONFIGURE_UE_ENTITY_CNF:
            rrc_uemh_mac_reconfigure_ue_entity_cnf_process_msg(p_api,
                    p_rrc_llim_gl_ctx);
            break;

        case RRC_MAC_CCCH_MSG_IND:
            rrc_uemh_mac_ccch_msg_ind_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_MAC_RECONFIG_CELL_CNF:
            rrc_cemh_mac_reconfig_cell_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;
        case RRC_MAC_RADIO_LINK_FAILURE_IND :
            rrc_uemh_mac_radio_link_failure_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_MAC_RESET_UE_ENTITY_CNF :
            rrc_uemh_mac_reset_ue_entity_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

            /*SPR_19066_START*/
            /* Code Removed */
            /*SPR_19066_END*/
        case RRC_MAC_HO_RACH_RESOURCE_RESP:
            rrc_uemh_mac_ho_rach_resource_resp(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_MAC_CHANGE_CRNTI_CNF:
            rrc_uemh_mac_change_crnti_cnf_process_msg(p_api,
                    p_rrc_llim_gl_ctx);
            break;
            /*SPR_19066_START*/
            /* Code Removed */
            /*SPR_19066_END*/
        case RRC_MAC_CELL_START_CNF:
            rrc_cemh_mac_cell_start_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_MAC_CELL_STOP_CNF:
            rrc_cemh_mac_cell_stop_cnf_process_msg(p_api,
                    p_rrc_llim_gl_ctx);
            break;

        case RRC_MAC_UE_SYNC_STATUS_IND:
            rrc_uemh_mac_ue_sync_status_ind_process_msg(p_api,
                    p_rrc_llim_gl_ctx);
            break;
#ifdef LTE_EMBMS_SUPPORTED
         case RRC_M2AP_MAC_COUNTING_MSG_CNF:
            
         case RRC_M2AP_MAC_MCCH_IND:

         case RRC_M2AP_MAC_RESET_SESSION_CNF:   
            /* Change dest module id to M2AP and send this message to M2AP */
            rrc_set_api_dest_id(p_api,RRC_M2AP_MODULE_ID);
            rrc_send_message(p_api, RRC_M2AP_MODULE_ID);
            break;
#endif
        default:
            RRC_LLIM_TRACE(RRC_WARNING, "rrc_llmd_mac_process_msg: unexpected API "
                    "from MAC module received");
            break;
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 * *   FUNCTION NAME: rrc_llmd_phy_process_msg
 * *   INPUT        : void                *p_api
 * *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 * *   OUTPUT       : None               
 * *   RETURNS      : None
 * *   DESCRIPTION  : This function passes PHY messages to CEMH or UEMH submodule
 * *                  based on incoming API id.
 * ******************************************************************************/
static void rrc_llmd_phy_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(p_api != PNULL);
    RRC_ASSERT(p_rrc_llim_gl_ctx != PNULL);

    switch (rrc_get_api_id(p_api))
    {
        case RRC_PHY_CONFIG_CELL_CNF:
            rrc_cemh_phy_config_cell_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_PHY_DELETE_CELL_CNF:
            rrc_cemh_phy_delete_cell_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_PHY_CREATE_UE_ENTITY_CNF:
            rrc_uemh_phy_create_ue_entity_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_PHY_DELETE_UE_ENTITY_CNF:
            rrc_uemh_phy_delete_ue_entity_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_PHY_RECONFIG_UE_ENTITY_CNF:
            rrc_uemh_phy_reconfig_ue_entity_cnf_process_msg(p_api,
                    p_rrc_llim_gl_ctx);
            break;
        case RRC_PHY_RECONFIG_CELL_CNF:
            rrc_cemh_phy_reconfig_cell_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_PHY_CHANGE_CRNTI_CNF:
            rrc_uemh_phy_change_crnti_cnf_process_msg(p_api,
                    p_rrc_llim_gl_ctx);
            break;
        
        case RRC_PHY_CELL_START_CNF:
            rrc_cemh_phy_cell_start_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case RRC_PHY_CELL_STOP_CNF:
            rrc_cemh_phy_cell_stop_cnf_process_msg(p_api,
                    p_rrc_llim_gl_ctx);
            break;
        default:
            RRC_LLIM_TRACE(RRC_WARNING, "rrc_llmd_phy_process_msg: unexpected API "
                    "from PHY module received");
            break;
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 * *   FUNCTION NAME: rrc_llmd_s1u_process_msg
 * *   INPUT        : void                *p_api
 * *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 * *   OUTPUT       : None               
 * *   RETURNS      : None
 * *   DESCRIPTION  : This function passes S1-U messages to UEMH submodule
 * *                  and check incoming API id.
 * ******************************************************************************/
static void rrc_llmd_s1u_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(p_api != PNULL);
    RRC_ASSERT(p_rrc_llim_gl_ctx != PNULL);

    switch (rrc_get_api_id(p_api))
    {
    case RRC_S1U_CREATE_UE_ENTITY_CNF:
        rrc_uemh_s1u_create_ue_entity_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_S1U_DELETE_UE_ENTITY_CNF:
        rrc_uemh_s1u_delete_ue_entity_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_S1U_RECONFIGURE_UE_ENTITY_CNF:
        rrc_uemh_s1u_reconfigure_ue_entity_cnf_process_msg(p_api,
            p_rrc_llim_gl_ctx);
        break;

    case RRC_S1U_ERROR_IND:
        rrc_uemh_s1u_error_ind_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_S1U_PATH_FAILURE_IND:
        rrc_uemh_s1u_path_failure_ind_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

/*JANEN_START*/
    case RRC_S1U_END_MARKER_IND:
        rrc_uemh_s1u_end_marker_ind_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;
/*JANEN_END*/

    case RRC_S1U_PATH_SUCCESS_IND: 
        rrc_uemh_s1u_path_success_ind_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;
        /*BUG 604 changes start*/
    case RRC_S1U_INTRA_ENB_DATA_FWD_CNF: 
        rrc_uemh_s1u_intra_enb_data_fwd_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;
    case RRC_S1U_LOCAL_PATH_SWITCH_CNF: 
        rrc_uemh_s1u_local_path_switch_cnf_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;
        /*BUG 604 changes stop*/


    default:
        RRC_LLIM_TRACE(RRC_WARNING,
            "[S1U] unexpected API received [%u]", rrc_get_api_id(p_api));
        break;
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_llmd_oamh_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : None               
*   RETURNS      : None
*   DESCRIPTION  : This function passes OAMH messages to LLIM OAMH submodule
*                  and check incoming API id.
******************************************************************************/
static void rrc_llmd_oamh_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(p_api != PNULL);
    RRC_ASSERT(p_rrc_llim_gl_ctx != PNULL);

    switch (rrc_get_api_id(p_api))
    {
    case LLIM_OAMH_INIT_CNF:
        rrc_llim_oamh_init_cnf_processs_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case LLIM_OAMH_PROV_REQ:
        rrc_llim_oamh_prov_req_processs_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case LLIM_OAMH_CLEANUP_REQ:
        rrc_llim_oamh_cleanup_req_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    default:
        RRC_LLIM_TRACE(RRC_WARNING, "rrc_llim_oamh_process_msg: unexpected API"
            " from OAMH module received");
        break;
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_llmd_uecc_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : None               
*   RETURNS      : None
*   DESCRIPTION  : This function passes UECC messages to UEMH submodule
*                  and check incoming API id.
******************************************************************************/
static void rrc_llmd_uecc_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    U16    message_index = LLIM_LAST_ID;
    p_llim_intf_handler_t   message_handler = PNULL;

    /* SPR 22964 Fix Start */
    SET_UE_INDEX(0xFFFF);
    /* SPR 22964 Fix End */

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(p_api != PNULL);
    RRC_ASSERT(p_rrc_llim_gl_ctx != PNULL);

    /* find API id */
    message_index = llim_find_index_by_id(
                    rrc_get_api_id(p_api),
                    (llim_id_t*)p_llim_handlers_table,
                    sizeof(llim_intf_handlers_table_t));

    /* klockwork fix */
    if( message_index < (sizeof(p_llim_handlers_table)/sizeof(llim_intf_handlers_table_t)) )
    {  
       message_handler = p_llim_handlers_table[message_index].p_llim_intf_handler;
    }

    if (PNULL == message_handler)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "rrc_llmd_uecc_process_msg: "
                "No event handler for received API [0x%04X]",
                rrc_get_api_id(p_api));
    }
    else
    {
        /* run message handler */
        message_handler(p_api, p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_llmd_csc_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : None               
*   RETURNS      : None
*   DESCRIPTION  : This function passes CSC messages to CEMH submodule
*                  and check incoming API id.
******************************************************************************/
static void rrc_llmd_csc_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(p_api != PNULL);
    RRC_ASSERT(p_rrc_llim_gl_ctx != PNULL);

    switch (rrc_get_api_id(p_api))
    {
        case CSC_LLIM_CONFIG_CELL_REQ:
            rrc_cemh_csc_llim_config_cell_req_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case CSC_LLIM_DELETE_CELL_REQ:
            rrc_cemh_csc_llim_delete_cell_req_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case CSC_LLIM_PCCH_DATA_REQ:
             rrc_cemh_csc_llim_pcch_data_req_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case CSC_LLIM_SFN_REQ:
            rrc_cemh_csc_llim_sfn_req_process_msg(p_api, p_rrc_llim_gl_ctx);
            break; 

        case CSC_LLIM_RECONFIG_CELL_REQ:
            rrc_cemh_csc_llim_reconfig_cell_req_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

      

        case CSC_LLIM_CELL_START_REQ:
            rrc_cemh_csc_llim_cell_start_req_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        case CSC_LLIM_CELL_STOP_REQ:
            rrc_cemh_csc_llim_cell_stop_req_process_msg(p_api, p_rrc_llim_gl_ctx);
            break;

        
    case CSC_LLIM_PWS_REQ:
        rrc_cemh_csc_llim_write_replace_warning_req_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;
    
    case CSC_LLIM_UPDATED_PWS_SI_REQ:
        rrc_cemh_csc_llim_si_without_etws_req_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;
    default:
        RRC_LLIM_TRACE(RRC_WARNING, "rrc_llmd_csc_process_msg: unexpected API "
            "from CSC module received");
        break;
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_llmd_process_msg
*   INPUT        : void *p_api
*                  void *p_gl_ctx
*   OUTPUT       : None               
*   RETURNS    : RRC_BUFFER_SHOULD_BE_RELEASED - Encapsulating shell should release
*                the buffer
*   DESCRIPTION: This is the main entry point of RRC LLIM module - CSPL callback. It
*                passes all the external APIs received from external entities PDCP, RLC,
*                MAC, PHY, RF and internal RRC entities CSC, UECC, OAMH to appropriate
*                internal submodule CEMH, UEMH or LLIM OAMH based on incoming module id
*                and API id.
*                This function always returns nonzero value, i.e. the buffer is always
*                released by the encapsulating shell.
******************************************************************************/
S32 rrc_llmd_process_msg
(
    void *p_api,        /* Pointer to input API buffer */
    void *p_gl_ctx      /* Pointer to the LLIM global context data */
)
{
    rrc_llim_gl_ctx_t *p_rrc_llim_gl_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    /* RRC_LLIM_TRACE(RRC_INFO, RRC_MSG_RECVD); */

    /* Precondition check */
    /* Check general message integrity too */
    if ( (PNULL == p_api) || (PNULL == p_gl_ctx) )
    {
        RRC_LLIM_TRACE(RRC_ERROR, "rrc_llmd_process_msg: somebody call us in "
            "wrong context");
        RRC_LLIM_UT_TRACE_EXIT();
        /* Encapsulating shell should release the buffer */
        return RRC_BUFFER_SHOULD_BE_RELEASED;
    }
    else if (RRC_SUCCESS != rrc_check_cspl_header(p_api))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "rrc_llmd_process_msg: bad message header "
            "- ignore msg");
        RRC_LLIM_UT_TRACE_EXIT();
        /* Encapsulating shell should release the buffer */
        return RRC_BUFFER_SHOULD_BE_RELEASED;
    }

    p_rrc_llim_gl_ctx = (rrc_llim_gl_ctx_t*)p_gl_ctx;

    switch (rrc_get_src_module_id(p_api))
    {
    case RRC_PDCP_MODULE_ID:
        rrc_llmd_pdcp_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_RLC_MODULE_ID:
        rrc_llmd_rlc_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_MAC_MODULE_ID:
        rrc_llmd_mac_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_PHY_MODULE_ID:
        rrc_llmd_phy_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_S1U_MODULE_ID:
        rrc_llmd_s1u_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_OAMH_MODULE_ID:
        rrc_llmd_oamh_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_UECC_MODULE_ID:
        rrc_llmd_uecc_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    case RRC_CSC_MODULE_ID:
        rrc_llmd_csc_process_msg(p_api, p_rrc_llim_gl_ctx);
        break;

    default:
        RRC_LLIM_TRACE(RRC_WARNING, "rrc_llmd_process_msg: API from unexpected"
            " module received");
        break;
    }

    RRC_LLIM_UT_TRACE_EXIT();

    /* Encapsulating shell should release the buffer */
    return RRC_BUFFER_SHOULD_BE_RELEASED;
}

/******************************************************************************
*   FUNCTION NAME: rrc_llmd_process_timer_msg
*   INPUT        : void *p_api
*                  void *p_gl_ctx
*   OUTPUT       : None               
*   RETURNS      : None
*   DESCRIPTION  : This is the main entry point of RRC LLIM module - CSPL callback.
*                  It handles all the timer expiry events.
*                  The timer buffer is always allocated in the rrc_start_timer() function.
*                  This buffer should always start with rrc_llim_timer_buf_t
*                  Timer buffer can be freed depending on information in
*                  rrc_llim_timer_buf_t.
******************************************************************************/
void rrc_llmd_process_timer_msg
(
    rrc_timer_t timer_id,       /* Identity of the expired timer */
    void        *p_timer_buf,   /* Pointer to data associated with the timer */
    void        *p_gl_ctx       /* Pointer to the LLIM global context data */
)
{
    rrc_llim_gl_ctx_t *p_rrc_llim_gl_ctx        = PNULL;
    rrc_llim_timer_buf_t *p_rrc_llim_timer_buf  = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_LLIM_TRACE(RRC_BRIEF, "Timer [%p] expired", timer_id);

    /* Precondition check */
    if ( (PNULL == p_timer_buf) || (PNULL == p_gl_ctx) )
    {
        RRC_LLIM_TRACE(RRC_ERROR, "rrc_llmd_process_timer_msg: somebody call "
            "us in wrong context");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_rrc_llim_timer_buf = (rrc_llim_timer_buf_t*)p_timer_buf;
    p_rrc_llim_gl_ctx = (rrc_llim_gl_ctx_t*)p_gl_ctx;

    
    switch (p_rrc_llim_timer_buf->target_submodule)
    {
    case LLIM_SUBMODULE_CEMH:
        rrc_cemh_process_timer_msg(timer_id, p_rrc_llim_timer_buf,
            p_rrc_llim_gl_ctx);
        break;

    case LLIM_SUBMODULE_UEMH:
        rrc_uemh_process_timer_msg(timer_id, p_rrc_llim_timer_buf,
            p_rrc_llim_gl_ctx);
        break;

    case LLIM_SUBMODULE_OAMH:
        rrc_llim_oamh_process_timer_msg(timer_id, p_rrc_llim_timer_buf,
            p_rrc_llim_gl_ctx);
        break;

    default:
        RRC_ASSERT(0);
        RRC_LLIM_TRACE(RRC_ERROR, "rrc_llmd_process_timer_msg: timer event for"
            " unexpected submodule received");
        break;
    }

    if (RRC_TRUE == p_rrc_llim_timer_buf->auto_delete)
    {
        if (PNULL != p_timer_buf)
        {
            rrc_mem_free(p_timer_buf);
	    /* + Coverity 81288 */
            //p_timer_buf = PNULL;
	    /* - Coverity 81288 */
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

