/******************************************************************************
*
*   FILE NAME:
*       uecc_llim.h
*
*   DESCRIPTION:
*       This is the interface file of the UECC LLIM.
*       UECC LLIM interacts with RRC LLIM module.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   20 Jul 2009     GrygoriyS   ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef UECC_LLIM_H
#define UECC_LLIM_H

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "uecc_global_ctx.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "rrc_protocol_events.h"

/****************************************************************************
 * Public functions definitions
 ****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_ccch_data_ind
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_CCCH_DATA_IND message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_ccch_data_ind
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_create_ue_entity_resp
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_CREATE_UE_ENTITY_RESP message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_create_ue_entity_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_srb_data_ind
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_SRB_DATA_IND message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_srb_data_ind
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_add_lc_resp
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_ADD_LC_RESP message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_add_lc_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_delete_ue_entity_resp
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_DELETE_UE_ENTITY_RESP message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_delete_ue_entity_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_srb_data_status_ind
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_SRB_DATA_STATUS_IND message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_srb_data_status_ind
(
    void                *p_api,              /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context   /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_configure_security_resp
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_CONFIGURE_SECURITY_RESP message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_configure_security_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_s1u_error_ind
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_S1U_ERROR_IND message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_s1u_error_ind
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_s1u_path_failure_ind
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_S1U_PATH_FAILURE_IND message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_s1u_path_failure_ind
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_s1u_path_success_ind
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_S1U_PATH_SUCCESS_IND message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_s1u_path_success_ind
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/*REL 1.2:HANDOVER PROCEDURE START*/
/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_mac_i_resp
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_MAC_I_RESP message
*
*   RETURNS:  None
*
******************************************************************************/
void uecc_llim_process_mac_i_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_sn_hfn_status_resp
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_SN_HFN_STATUS_RESP message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_sn_hfn_status_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_ue_inactive_time_resp
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_UE_INACTIVE_TIME_RESP message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_ue_inactive_time_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_ho_rach_resource_resp
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_HO_RACH_RESOURCE_RESP message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_ho_rach_resource_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_s1u_reconfigure_cnf
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_S1U_RECONFIGURE_CNF message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_s1u_reconfigure_cnf
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_s1u_end_marker_ind
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_S1U_END_MARKER_IND message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_s1u_end_marker_ind
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);
/*REL 1.2:HANDOVER PROCEDURE END*/

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_re_establish_ue_entity_resp
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_RE_ESTABLISH_UE_ENTITY_RESP 
*       message
*
*   RETURNS:
*       None
*
 ******************************************************************************/
void uecc_llim_process_re_establish_ue_entity_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_reconfig_security_resp
*
*   DESCRIPTION:
*       This function processes UECC_LLIM_RECONFIG_SECURITY_RESP
*       message
*
*   RETURNS:
*       None
*
 ******************************************************************************/
void uecc_llim_process_reconfig_security_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_resume_ue_entity_resp
*
*   DESCRIPTION:
*       This function processes UECC_LLIM_RESUME_UE_ENTITY_RESP 
*       message
*
*   RETURNS:
*       None
*
 ******************************************************************************/
void uecc_llim_process_resume_ue_entity_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_change_crnti_resp
*
*   DESCRIPTION:
*       This function processes UECC_LLIM_CHANGE_CRNTI_RESP 
*       message
*
*   RETURNS:
*       None
*
 ******************************************************************************/
void uecc_llim_process_change_crnti_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);
/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_meas_config_resp
*
*   DESCRIPTION:
*       This function processes UECC_LLIM_MEAS_CONFIG_RESP 
*       message
*
*   RETURNS:
*       None
*
 ******************************************************************************/
void uecc_llim_process_meas_config_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_inactive_ues_ind
*
*   DESCRIPTION:
*       This function processes UECC_LLIM_INACTIVE_UES_IND 
*       message
*
*   RETURNS:
*       None
*
 ******************************************************************************/
void uecc_llim_process_inactive_ues_ind
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_rrc_connection_reject
*
*   DESCRIPTION:
*       This function builds and sends RRC connection reject message
*       to LLIM module. (UECC_LLIM_CCCH_DATA_REQ)
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_rrc_connection_reject
(
    uecc_gb_context_t*  p_uecc_gb_context,
    rrc_rnti_t          rnti,        /* UE rnti */
    U8                  wait_time,    /* RRCConnectionReject waitTime value */
    U32                 establishment_cause,
    rrc_cell_index_t    cell_index,
    rrc_bool_t          rcs_reject_cause,
    U16                 extended_wait_time,/*CR 026 changes start*/
    uecc_ue_context_t   *p_ue_context/*CR 026 changes stop*/
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_delete_ue_entity_req
*
*   DESCRIPTION:
*       This function builds and sends delete UE entity  message
*       to LLIM module. (UECC_LLIM_DELETE_UE_ENTITY_REQ)
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_delete_ue_entity_req
(
    uecc_ue_context_t*  p_uecc_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_create_ue_entity_req
*
*   DESCRIPTION:
*                This is the function for building and sending messages from
*                the UECC to LLIM to create UE entity at lower layers
*                (UECC_LLIM_CREATE_UE_ENTITY_REQ)
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_create_ue_entity_req(
        uecc_ue_context_t*  p_uecc_ue_context
);

/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_rrc_connnection_setup
 * Description    : This is the function for building and sending messages from
 *                : the UECC to LLIM
 * Returns        : rrc_return_et
****************************************************************************/
rrc_return_et uecc_llim_build_and_send_rrc_connection_setup(
        uecc_ue_context_t*  p_uecc_ue_context
);

/* ERAB SETUP START */

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_add_rollback_req
*
*   DESCRIPTION:
*       Send internal message UECC_LLIM_ADD_ROLLBACK_REQ to LLIM
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_add_rollback_req(
        uecc_ue_context_t *);

/* ERAB SETUP STOP */

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_add_lc_req
*
*   DESCRIPTION:
*       Send internal message UECC_LLIM_ADD_LC_REQ to LLIM
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_add_lc_req(
        uecc_ue_context_t *);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_rrc_connection_reconfiguration
*
*   DESCRIPTION:
*       This function builds and sends RRCConnectionReconfiguration
*       message to LLIM
*
*   RETURNS:
*       RRC_FAILURE/RRC_SUCCESS
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_rrc_connection_reconfiguration(
        uecc_ue_context_t*  p_uecc_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_rrc_connection_release
*
*   DESCRIPTION:
*       This function builds and sends RRCConnectionRelease message
*       to LLIM module. (UECC_LLIM_SRB_DATA_REQ)
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_rrc_connection_release
(
    uecc_ue_context_t*  p_uecc_ue_context                    /* UE context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_rrc_dl_information_transfer
*
*   DESCRIPTION:
*       This function builds and sends rrcDLInformationTransfer message
*       to LLIM module. (UECC_LLIM_SRB_DATA_REQ)
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_rrc_dl_information_transfer
(
    uecc_ue_context_t   *p_uecc_ue_context,                    /* UE context */
    s1ap_NAS_PDU        *p_nas_pdu,
    U8 		dedicatedInfoType
);
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_rrc_ue_capability_enquiry
*
*   DESCRIPTION:
*       This function builds and sends ueCapabilityEnquiry message
*       to LLIM module. (UECC_LLIM_SRB_DATA_REQ)
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_rrc_ue_capability_enquiry
(
    uecc_ue_context_t   *p_uecc_ue_context        /* UE context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_configure_security_req
*
*   DESCRIPTION:
*       This function builds and sends configure security message
*       to LLIM module. (UECC_LLIM_CONFIGURE_SECURITY_REQ)
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_configure_security_req
(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_t          configure_integrity,
    rrc_bool_t          configure_ciphering,
    rrc_rb_direction_et direction,
    rrc_bool_t          switch_off
);

/* CDMA2000_CSFB_HO start */

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_and_send_csfb_parameters_response_CDMA2000
 * 
 *   DESCRIPTION:
 *        This function builds and sends csfb_parameters_response
 *        to LLIM module. (UECC_LLIM_CONFIGURE_SECURITY_REQ)
 * 
 *   RETURNS:
 *        RRC_SUCCESS/RRC_FAILURE
 * 
 * ******************************************************************************/
rrc_return_et uecc_llim_build_and_send_csfb_parameters_response_CDMA2000
(
    uecc_ue_context_t                *p_uecc_ue_context,  /* UECC  context */
    rrc_rrm_csfb_parameters_resp_cdma2000_t *p_rrc_rrm_csfb_parameters_resp_cdma2000

);

/******************************************************************************
 *   FUNCTION NAME:  uecc_llim_build_and_send_ho_from_eutra_req
 * 
 *   DESCRIPTION:
 *        This function builds and sends ho from eutra request
 *        to LLIM module.
 * 
 *   RETURNS:
 *        RRC_SUCCESS/RRC_FAILURE
 *****************************************************************************/ 
rrc_return_et uecc_llim_build_and_send_ho_from_eutra_req
(
    uecc_ue_context_t*  p_uecc_ue_context        /* UE context );*/
);

/* CDMA2000_CSFB_HO stop */

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_rrc_security_mode_command
*
*   DESCRIPTION:
*       This function builds and sends rrcSecurityModeCommand message
*       to LLIM module. (UECC_LLIM_SRB_DATA_REQ)
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_rrc_security_mode_command
(
    uecc_ue_context_t   *p_ue_context        /* UE context */
);
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_delete_lc_req
*
*   DESCRIPTION:
*       This function builds and sends delete_lc_req message
*       to LLIM module. 
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_delete_lc_req
(
    uecc_ue_context_t*  p_ue_context        /* UE context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_rrc_uecc_llim_ho_rach_rel_resource_ind
*
*   DESCRIPTION:
*       This function builds and sends rrc_uecc_llim_ho_rach_rel_resource_ind message
*       to LLIM module. 
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_rrc_uecc_llim_ho_rach_rel_resource_ind
(
    uecc_ue_context_t*  p_uecc_ue_context
);

rrc_return_et uecc_delete_fwd_tunnels_llim_build_and_send_s1u_reconfigure_req
(
    rrc_uecc_llim_s1u_end_marker_ind_t * p_rrc_uecc_llim_s1u_end_marker_ind,
    uecc_ue_context_t                  *p_uecc_ue_context
);


/******************************************************************************
*   FUNCTION NAME: uecc_rrm_build_and_send_ue_ho_adm_cnf
*
*   DESCRIPTION:
*       This function builds and sends RRC_RRM_UE_HO_ADM_CNF message to RRM.
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_rrm_build_and_send_ue_ho_adm_cnf(
    uecc_ue_context_t*  p_ue_context,
    rrc_return_et response
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_sn_hfn_status_ind
*
*   DESCRIPTION:
*       This function builds and sends rrc_uecc_llim_sn_hfn_status_ind message
*       to LLIM module. 
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_sn_hfn_status_ind
(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_s1ap_mme_status_transfer_t * p_rrc_s1ap_mme_status_transfer
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_x2ap_build_and_send_sn_hfn_status_ind 
*
*   DESCRIPTION:
*       This function builds and sends rrc_uecc_llim_sn_hfn_status_ind message
*       to LLIM module. 
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_x2ap_build_and_send_sn_hfn_status_ind
(
    uecc_ue_context_t*              p_uecc_ue_context,
    rrc_x2ap_sn_status_transfer_t*  p_rrc_x2ap_mme_status_transfer
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_data_buffer_stop_ind
*
*   DESCRIPTION:
*       This function builds and sends rrc_uecc_llim_data_buffer_stop_ind message
*       to LLIM module. 
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_data_buffer_stop_ind
(
    uecc_ue_context_t*  p_uecc_ue_context
);

/* HANDOVER_CHANGES_KB_01_START */
/*SPR_19066_START*/
/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_ho_prep_info_req
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t 
 * Description:    This is the function for building and sending
 *                   UECC_LLIM_HO_PREP_INFO_REQ message to the LLIM.
 ****************************************************************************/
rrc_return_t uecc_llim_build_and_send_ho_prep_info_req(
/*SPR_19066_END*/
        uecc_ue_context_t   *p_ue_context      /* UECC UE context */
        );

/******************************************************************************
 * Function Name  : uecc_llim_build_and_send_ue_inactive_time_req 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t 
 * Description:    This is the function for building and sending
 *                   UECC_LLIM_UE_INACTIVE_TIME_REQ message to the LLIM.
 ****************************************************************************/
rrc_return_t uecc_llim_build_and_send_ue_inactive_time_req(
        uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
        );

/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_sn_hfn_status_transfer_req 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t 
 * Description:    This is the function for building and sending
 *                   UECC_LLIM_SN_HFN_STATUS_REQ message to the LLIM.
 ****************************************************************************/
rrc_return_t uecc_llim_build_and_send_sn_hfn_status_transfer_req(
        uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
        );

/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_re_est_req 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t 
 * Description:    This is the function for building and sending
 *                   UECC_LLIM_RE_EST_REQ message to the LLIM.
 ****************************************************************************/

rrc_return_t uecc_llim_build_and_send_re_est_req(
        uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
        );


/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_s1u_reconfig_req 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t 
 * Description:    This is the function for building and sending
 *                   UECC_LLIM_S1U_RECONFIG_REQ message to the LLIM.
 ****************************************************************************/

rrc_return_t uecc_llim_build_and_send_s1u_reconfig_req(
        uecc_ue_context_t   *p_ue_context      /* UECC UE context */
        );

/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_s1u_delete_forwarding_tunnel_req 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t 
 * Description:    This is the function for building and sending
 *                   UECC_LLIM_S1U_RECONFIG_REQ to delete forwarding tunnels 
 *                   message to the LLIM.
 ****************************************************************************/
rrc_return_t uecc_llim_build_and_send_s1u_delete_forwarding_tunnel_req(
        uecc_ue_context_t   *p_ue_context      /* UECC UE context */
        );

/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_data_buffer_stop_ind 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t 
 * Description:    This is the function for building and sending
 *                   UECC_LLIM_PDCP_DATA_BUFFER_STOP_IND message to the LLIM.
 ****************************************************************************/
rrc_return_t uecc_llim_build_and_send_stop_data_buffer_ind(
        uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
        );

/******************************************************************************
 *   FUNCTION NAME: uecc_s1ap_build_VarShortMAC_Input  

 *
 *   DESCRIPTION:
 *       This function builds VarShortMAC_Input  
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************/
rrc_return_et uecc_s1ap_build_VarShortMAC_Input(
        uecc_ue_context_t   *p_ue_context,
        U32                 *p_cell_identity,
        U8                  *p_encoded_mac_i,
        U16                 *p_encoded_msg_length );
/* HANDOVER_CHANGES_KB_01_END */

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_delete_lc_req
*
*   DESCRIPTION:
*       Bulid internal message UECC_LLIM_DELETE_LC_REQ 
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
void uecc_llim_build_delete_lc_req
(
    uecc_ue_context_t*  p_ue_context , /* UE context */
    rrc_uecc_llim_delete_lc_req_t* p_rrc_uecc_llim_delete_lc_req  
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_delete_lc_resp
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_DELETE_LC_RESP message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_delete_lc_resp
(
    void*                p_api,             /* Input API buffer */
    uecc_gb_context_t*   p_uecc_gb_context  /* UECC global context */
);

/* ERB RELEASE COMMAND END */

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_modify_lc_req
*
*   DESCRIPTION:
*       Send internal message UECC_LLIM_MODIFY_LC_REQ to LLIM
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_modify_lc_req(
        uecc_ue_context_t *);

/******************************************************************************
*  FUNCTION NAME: uecc_llim_build_and_send_s1u_reconfig_req_for_ul_switched_list 
*
*  DESCRIPTION:
*       Send internal message UECC_LLIM_S1U_RECONFIG_REQ to LLIM
*
*  RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et 
uecc_llim_build_and_send_s1u_reconfig_req_for_ul_switched_list(
        uecc_ue_context_t *);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_modify_lc_resp
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_MODIFY_LC_RESP message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_modify_lc_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);


/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_modify_lc_req
*
*   DESCRIPTION:
*       Send internal message UECC_LLIM_MODIFY_LC_REQ to LLIM
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_modify_lc_req
(
    uecc_ue_context_t *p_uecc_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_erb_modify_rrc_connection_reconfiguration
*
*   DESCRIPTION:
*       This function builds and sends RRCConnectionReconfiguration
*       message to LLIM
*
*   RETURNS:
*       RRC_FAILURE/RRC_SUCCESS
*
******************************************************************************/
rrc_return_et 
    uecc_llim_build_and_send_erb_modify_rrc_connection_reconfiguration(
        uecc_ue_context_t*  p_uecc_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_modify_rollback_req
*
*   DESCRIPTION:
*       This function builds and sends  LLIM UECC_LLIM_MODIFY_ROLLBACK_REQ message
*
*   RETURNS:
*       rrc_return_et
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_modify_rollback_req
(
    uecc_ue_context_t *p_uecc_ue_context
);




/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_add_rollback_resp
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_ADD_ROLLBACK_RESP message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_add_rollback_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/* ERAB SETUP START */

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_reestablish_ue_entity_req
*
*   DESCRIPTION:
*       This function processes build and send reestablish_ue_entity_req
*
*   RETURNS:
*       None
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_reestablish_ue_entity_req(
    uecc_ue_context_t *p_uecc_ue_context,
    rrc_lc_id_t        lc_id,
/* SPR 18926 Fix Start */
    rrc_bool_et        rlc_reestablish_required
/* SPR 18926 Fix Stop */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_erb_setup_rrc_connection_reconfiguration
*
*   DESCRIPTION:
*       This function builds and sends RRCConnectionReconfiguration
*       message to LLIM
*
*   RETURNS:
*       RRC_FAILURE/RRC_SUCCESS
*
******************************************************************************/
rrc_return_et 
    uecc_llim_build_and_send_erb_setup_rrc_connection_reconfiguration(
        uecc_ue_context_t*  p_uecc_ue_context
);


/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_s1ap_cause
*
*   DESCRIPTION:
*       This function builds s1ap_Cause
*
*   RETURNS:
*       None
*
*******************************************************************************/
rrc_return_et uecc_llim_build_s1ap_cause(
        uecc_gb_context_t   *p_uecc_gb_context, 
    s1ap_Cause*     p_cause,
    rrc_response_t             response_code
);

/* ERAB SETUP STOP */

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_radio_link_failure_ind
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_RADIO_LINK_FAILURE_IND  message
*
*   RETURNS:
*       None
*
******************************************************************************/
/* SPR 15644 fix start */
void uecc_llim_process_sync_status_ind
/* SPR 15644 fix end */
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_suspend_ue_entity_cnf
*
*   DESCRIPTION:
*       This function processes UECC_LLIM_SUSPEND_UE_ENTITY_CNF message
*
*   RETURNS:
*       None
*
******************************************************************************/

void uecc_llim_suspend_ue_entity_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

void uecc_llim_build_erb_setup_dedicated_nas_info(
RRCConnectionReconfiguration_r8_IEs*    p_r8,
uecc_ue_context_t*    p_uecc_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_modify_rollback_resp
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_MODIFY_ROLLBACK_RESP message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_process_modify_rollback_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

rrc_return_et 
    uecc_llim_build_ho_rrc_connection_reconfiguration(
        uecc_ue_context_t*  p_uecc_ue_context,
        DL_DCCH_Message                         * p_dl_dcch_msg,
    OSCTXT                                  * p_asn1_ctx
);

/******************************************************************************
*   FUNCTION NAME:
*       uecc_llim_build_and_send_erb_setup_rrc_connection_reconfiguration
*
*   DESCRIPTION:
*       This function builds and sends UECC_LLIM_SRB_DATA_REQ message
*       with RRCConnectionReconfiguration to LLIM
*
*   RETURNS:
*       RRC_FAILURE/RRC_SUCCESS
*
********************************************************************************/
rrc_return_et uecc_llim_build_and_send_ics_rrc_connection_reconfiguration
(
    uecc_ue_context_t*  p_uecc_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_rrc_uecc_llim_ho_rach_resource_req
*
*   DESCRIPTION:
*       This function builds and sends rrc_uecc_llim_ho_rach_resource_req message
*       to LLIM module. 
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_rrc_uecc_llim_ho_rach_resource_req
(
   /*BUG 604 changes start*/
   uecc_ue_context_t*  p_uecc_ue_context
    //rrc_bool_et         enable_intra_enb_ho
    /*BUG 604 changes stop*/
);

/******************************************************************************
*   FUNCTION NAME: 
*            uecc_llim_build_and_send_rrc_connection_reestablishment_reject
*
*   DESCRIPTION:
*       This function builds and sends RRC connection reestablishment reject
*       message to LLIM module. (UECC_LLIM_CCCH_DATA_REQ)
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_rrc_connection_reestablishment_reject 
(
    uecc_gb_context_t*  p_uecc_gb_context,
    rrc_rnti_t          rnti,        /* UE rnti */
    U32                 cause,
    rrc_cell_index_t    cell_index,
    rrc_module_id_t     dst_module_id
);
/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_rrc_connection_reestablishment
 * Description    : 
 *       This function builds and sends RRC connection reestablishment
 *       message to LLIM module. (UECC_LLIM_CCCH_DATA_REQ)
 *        
 * Returns        : rrc_return_et
****************************************************************************/
rrc_return_et uecc_llim_build_and_send_rrc_connection_reestablishment(
        uecc_ue_context_t*  p_uecc_ue_context
);

/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_reconfig_security_req
 * Description    :
 *       This function builds UECC_LLIM_RECONFIG_SECURITY_REQ
 *       and send to LLIM module.
 *
 * Returns        : RRC_SUCCESS/RRC_FAILURE
****************************************************************************/
rrc_return_et uecc_llim_build_and_send_reconfig_security_req(
        uecc_ue_context_t   *p_ue_context
);
/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_suspend_ue_entity_req
 * Description    :
 *       This function builds uecc_llim_suspend_ue_entity_req
 *       and send to LLIM module.
 *
 * Returns        : RRC_SUCCESS/RRC_FAILURE
****************************************************************************/
rrc_return_et uecc_llim_build_and_send_suspend_ue_entity_req(
        uecc_ue_context_t   *p_ue_context
);
        
/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_resume_ue_entity_req
 * Description    :
 *       This function builds UECC_LLIM_RESUME_UE_ENTITY_REQ
 *       and send to LLIM module.
 *
 * Returns        : RRC_SUCCESS/RRC_FAILURE
****************************************************************************/
rrc_return_et uecc_llim_build_and_send_resume_ue_entity_req(
        uecc_ue_context_t   *p_ue_context,
        rrc_lc_id_t          lc_id,
        rrc_rb_direction_t   resume_dir
);


/* Measurment Re-config START */
/******************************************************************************
*   FUNCTION NAME: 
*       uecc_llim_build_and_send_meas_reconfig_rrc_connection_reconfiguration
*
*   DESCRIPTION:
*       This function builds and sends UECC_LLIM_SRB_DATA_IND message
*       with RRCConnectionReconfiguration to LLIM
*
*   RETURNS:
*       RRC_FAILURE/RRC_SUCCESS
*
******************************************************************************/
rrc_return_et 
    uecc_llim_build_and_send_meas_reconfig_rrc_connection_reconfiguration(
        uecc_ue_context_t*  p_uecc_ue_context
);

/******************************************************************************
 *   FUNCTION NAME: uecc_rrc_update_MeasObjectToRemoveList_to_CurrMeasConfigList
 *
 *   DESCRIPTION:
 *       This function builds MeasObjectToRemoveList in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_rrc_update_MeasObjectToRemoveList_to_CurrMeasConfigList(
        rrm_meas_config_t* p_rrm_meas_config,
        uecc_ue_curr_meas_config_t* p_curr_rrm_meas_config,
        uecc_gb_context_t*  p_uecc_gb_context
        );

/******************************************************************************
 * *   FUNCTION NAME: uecc_rrc_update_MeasObjectToAddModList_to_CurrMeasConfigList
 * *
 * *   DESCRIPTION:
 * *       This function builds MeasObjectToAddModList in CurrMeasConfig structure
 * *
 * *   RETURNS:
 * *       RRC_SUCCESS/RRC_FAILURE
 * *
 * ******************************************************************************/
rrc_return_et uecc_rrc_update_MeasObjectToAddModList_to_CurrMeasConfigList(
        rrm_meas_config_t* p_rrm_meas_config,
        uecc_ue_curr_meas_config_t* p_curr_rrm_meas_config,
        uecc_gb_context_t*  p_uecc_gb_context
        );

/******************************************************************************
 * *   FUNCTION NAME: uecc_rrc_update_MeasIdToRemoveList_to_CurrMeasConfigList
 * *
 * *   DESCRIPTION:
 * *       This function update MeasIdToRemoveList in
 * *       CurrMeasConfig structure
 * *
 * *   RETURNS:
 * *       RRC_SUCCESS/RRC_FAILURE
 * *
 * ******************************************************************************/
rrc_return_et uecc_rrc_update_MeasIdToRemoveList_to_CurrMeasConfigList(
        rrm_meas_config_t* p_rrm_meas_config,
        uecc_ue_curr_meas_config_t* p_curr_rrm_meas_config,
        uecc_gb_context_t*  p_uecc_gb_context
        );

/******************************************************************************
 * *   FUNCTION NAME: uecc_rrc_update_ReportConfigToRemoveList_to_CurrMeasConfigLi
 * *
 * *   DESCRIPTION:
 * *       This function update MeasIdToRemoveList in
 * *       CurrMeasConfig structure
 * *
 * *   RETURNS:
 * *       RRC_SUCCESS/RRC_FAILURE
 * *
 * ******************************************************************************/
rrc_return_et uecc_rrc_update_ReportConfigToRemoveList_to_CurrMeasConfigList(
        rrm_meas_config_t* p_rrm_meas_config,
        uecc_ue_curr_meas_config_t* p_curr_rrm_meas_config,
        uecc_gb_context_t*  p_uecc_gb_context
        );

/******************************************************************************
 * *   FUNCTION NAME: uecc_rrc_update_ReportConfigToAddModList_to_CurrMeasConfigList
 * *
 * *   DESCRIPTION:
 * *       This function update MeasIdToRemoveList in
 * *       CurrMeasConfig structure
 * *
 * *   RETURNS:
 * *       RRC_SUCCESS/RRC_FAILURE
 * *
 * ******************************************************************************/
rrc_return_et uecc_rrc_update_ReportConfigToAddModList_to_CurrMeasConfigList(
        rrm_meas_config_t* p_rrm_meas_config,
        uecc_ue_curr_meas_config_t* p_curr_rrm_meas_config,
        uecc_gb_context_t*  p_uecc_gb_context
        );

/******************************************************************************
 * *   FUNCTION NAME: uecc_rrc_update_MeasIdToAddModList_to_CurrMeasConfigList
 * *
 * *   DESCRIPTION:
 * *       This function update MeasIdToRemoveList in
 * *       CurrMeasConfig structure
 * *
 * *   RETURNS:
 * *       RRC_SUCCESS/RRC_FAILURE
 * *
 * ******************************************************************************/
rrc_return_et uecc_rrc_update_MeasIdToAddModList_to_CurrMeasConfigList(
        rrm_meas_config_t* p_rrm_meas_config,
        uecc_ue_curr_meas_config_t* p_curr_rrm_meas_config,
        uecc_gb_context_t*  p_uecc_gb_context
        );

/******************************************************************************
 * *   FUNCTION NAME: uecc_rrc_update_CurrMeasConfig
 * *
 * *   DESCRIPTION:
 * *       This function builds CurrMeasConfig field from MeasConfig and update
 * *       the UE context.
 * *
 * *   RETURNS:
 * *       RRC_SUCCESS/RRC_FAILURE
 * *
 * ******************************************************************************/
rrc_return_et uecc_rrc_update_CurrMeasConfigList(
        rrm_meas_config_t*  p_rrm_meas_config,
        uecc_ue_curr_meas_config_t*  p_ue_curr_meas_config,
        uecc_gb_context_t*  p_uecc_gb_context
        );



/******************************************************************************
*   FUNCTION NAME: uecc_update_MeasConfig_in_ics_complete
*
*   DESCRIPTION:
*       This function builds MeasConfig field in RRCConnectionReconfiguration
*       message
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_update_MeasConfig_in_ics_complete(
    rrm_meas_config_t*           p_rrm_meas_config,
    uecc_ue_curr_meas_config_t*  p_rrm_curr_meas_config,
    uecc_gb_context_t*           p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_rrc_update_curr_MeasObjectCDMA2000
*
*   DESCRIPTION:
*       This function builds MeasObjectCDMA2000 in MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_rrc_update_curr_MeasObjectCDMA2000(
    meas_object_cdma2000_t*     p_rrm_curr_meas_object_cdma2000,
    meas_object_cdma2000_t*     p_rrm_meas_object_cdma2000,
    uecc_gb_context_t*           p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_rrc_update_curr_MeasObjectEUTRA
*
*   DESCRIPTION:
*       This function builds MeasObjectEUTRA in currMeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_rrc_update_curr_MeasObjectEUTRA(
    meas_object_eutra_t*     p_rrm_curr_meas_object_eutra,
    meas_object_eutra_t*     p_rrm_meas_object_eutra,
    uecc_gb_context_t*           p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_rrc_update_curr_CellsToAddModListCDMA2000
*
*   DESCRIPTION:
*       This function builds CellsToAddModCDMA2000 in curr MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_rrc_update_curr_CellsToAddModListCDMA2000(
        cells_to_add_mod_list_cdma2000_list_t*   p_rrm_curr_cells_to_add_mod_list_cdma2000_list,
        cells_to_add_mod_list_cdma2000_list_t*   p_rrm_cells_to_add_mod_list_cdma2000_list,
        uecc_gb_context_t*           p_uecc_gb_context  /* UECC global context */
        );

/******************************************************************************
*   FUNCTION NAME: uecc_rrc_update_curr_CellsToAddModListEUTRA
*
*   DESCRIPTION:
*       This function builds CellsToAddModEUTRA in curr MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_rrc_update_curr_CellsToAddModListEUTRA(
        cells_to_add_mod_list_t*   p_rrm_curr_cells_to_add_mod_list_eutra_list,
        cells_to_add_mod_list_t*   p_rrm_cells_to_add_mod_list_eutra_list,
        uecc_gb_context_t*           p_uecc_gb_context  /* UECC global context */
        );

/*****************************************************************************
*   FUNCTION NAME: uecc_rrc_update_curr_BlackListedCellsToAddModListEUTRA
*
*   DESCRIPTION:
*       This function builds CellsToAddModEUTRA in currMeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_rrc_update_curr_BlackListedCellsToAddModListEUTRA(
    black_cells_to_add_mod_list_t* p_rrm_curr_black_cells_to_add_mod_list_eutra_list,
    black_cells_to_add_mod_list_t* p_rrm_black_cells_to_add_mod_list_eutra_list,
    uecc_gb_context_t*           p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_rrc_update_curr_MeasObjectUTRA
*
*   DESCRIPTION:
*       This function builds MeasObjectUTRA in MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_rrc_update_curr_MeasObjectUTRA(
    meas_object_utra_t*         p_rrm_curr_meas_object_utra,
    meas_object_utra_t*         p_rrm_meas_object_utra,
    uecc_gb_context_t*           p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_rrc_update_curr_CellsToAddModListUTRA_FDD
*
*   DESCRIPTION:
*       This function builds CellsToAddModUTRA-FDD in MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_rrc_update_curr_CellsToAddModListUTRA_FDD(
    cells_to_add_mod_list_utra_fdd_t* p_rrm_curr_cells_to_add_mod_list_utra_fdd,
    cells_to_add_mod_list_utra_fdd_t* p_rrm_cells_to_add_mod_list_utra_fdd,
    uecc_gb_context_t*           p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_rrc_update_curr_CellsToAddModListUTRA_TDD
*
*   DESCRIPTION:
*       This function builds CellsToAddModUTRA-TDD in MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_rrc_update_curr_CellsToAddModListUTRA_TDD(
    cells_to_add_mod_list_utra_tdd_t* p_rrm_curr_cells_to_add_mod_list_utra_tdd,
    cells_to_add_mod_list_utra_tdd_t* p_rrm_cells_to_add_mod_list_utra_tdd,
    uecc_gb_context_t*           p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_rrc_update_curr_MeasobjectToAddMod
*
*   DESCRIPTION:
*       This function builds MeasObjectToAddMod in MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_rrc_update_curr_MeasobjectToAddMod(
    uecc_meas_object_to_add_mod_t*   p_ue_curr_meas_object_to_add_mod,
    meas_object_to_add_mod_t*   p_rrm_meas_object_to_add_mod,
    uecc_gb_context_t*           p_uecc_gb_context  /* UECC global context */
);

/* Measurment Re-config END */

/******************************************************************************
*   FUNCTION NAME: uecc_rat_type_keyof
*
*   DESCRIPTION:
*       This function is the keyof function for the UE capability
*       linked list.
*
*   RETURNS:
*
******************************************************************************/

const void * uecc_rat_type_keyof ( const YLNODE *p_ylnode);

/******************************************************************************
*   FUNCTION NAME: uecc_rat_type_compare
*
*   DESCRIPTION:
*       This function compares the key passed to retrieve correct node
*       from linked list.
*
*   RETURNS:
*
******************************************************************************/

int uecc_rat_type_compare (const void *p_key1, const void *p_key2);
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_meas_config_req
*
*   DESCRIPTION:
*       Build internal message UECC_LLIM_MEAS_CONFIG_REQ
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_build_meas_config_req(
    uecc_ue_context_t*             p_uecc_ue_context,
    rrc_uecc_llim_meas_config_req_t* p_rrc_uecc_llim_meas_config_req
);


/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_meas_config_req
*
*   DESCRIPTION:
*       Send internal message UECC_LLIM_MEAS_CONFIG_REQ to LLIM
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_meas_config_req
(
    uecc_ue_context_t* p_uecc_ue_context
);


/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_meas_config_req
*
*   DESCRIPTION:
*       Send internal message UECC_LLIM_MEAS_CONFIG_REQ to LLIM
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_meas_config_req
(
    uecc_ue_context_t* p_uecc_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_intra_cell_ho_rrc_conn_reconfig
*
*   DESCRIPTION:
*       This function builds and sends RRCConnectionReconfiguration
*       message to LLIM
*
*   RETURNS:
*       RRC_FAILURE/RRC_SUCCESS
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_intra_cell_ho_rrc_conn_reconfig(
        uecc_ue_context_t*  p_uecc_ue_context,
        OSCTXT   *                     p_asn1_ctx,
        rrc_uecc_llim_srb_data_req_t*  p_srb_data_req
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_count_wraparound_ind
*
*   DESCRIPTION:
*       This function processes UECC_LLIM_COUNT_WRAPAROUND_IND 
*       message
*
*   RETURNS:
*       None
*
 ******************************************************************************/
void uecc_llim_process_count_wraparound_ind
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_reset_ue_entity_cnf 
*
*   DESCRIPTION:
*   This function processes UECC_LLIM_RESET_UE_ENTITY_CNF
*
*   RETURNS:
*       None
*
 ******************************************************************************/
void uecc_llim_process_reset_ue_entity_cnf
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_reset_ue_entity_req 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t 
 * Description:     This is the function for building and sending
 *                  UECC_LLIM_RESET_UE_ENTITY_REQ message to the LLIM.
 ****************************************************************************/
rrc_return_t uecc_llim_build_and_send_reset_ue_entity_req(
        uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
);

/****************************************************************************
 * Function Name  : uecc_llim_rlf_ind_build_and_send_mac_i_req
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t
 * Description:   : This is the function for building and sending
 *                  UECC_LLIM_MAC_I_REQ message to the LLIM.
 ****************************************************************************/
rrc_return_t uecc_llim_rlf_ind_build_and_send_mac_i_req(
                            uecc_ue_context_t   *p_ue_context);

/* SPR 4118 Start */
void uecc_llim_process_notify_integrity_failure
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);
/* SPR 4118 Stop */

/****************************************************************************
 * Function Name  : rrc_uecc_fill_protocol_event_params 
 *
 * Description    : This  function fills the parameters for the specific 
 * 		    Protocol Event
  
 ****************************************************************************/
void rrc_uecc_fill_protocol_event_params
(
    uecc_ue_context_t *p_uecc_ue_context,
    EVENT_EXTERNAL_HEADER *p_event_header,
    U32 total_length,
    U16 event_id,
    U32 message_direction
);

/* CDMA2000_CSFB_HO start*/
/******************************************************************************
*   FUNCTION NAME: rrc_form_mob_params_to_octet_string
*
*   DESCRIPTION:
*       This function forms an octet string of mobility parameters received 
*       from parameter transfer response from RRM.
*
*   RETURNS:
*      void
*
******************************************************************************/

void  rrc_form_mob_params_to_octet_string(
        uecc_ue_context_t*  p_uecc_ue_context,
	OSDynOctStr *p_octet_data,
        mobility_parameters_t* p_mobility_parameters_fill,
        additional_mobility_parameters_t* p_add_mobility_parameters_fill
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_ho_from_eutra_prep_req 
*
*   DESCRIPTION:
*       This function builds and sends HO From EUTRA Req message
*       to LLIM module. (UECC_LLIM_SRB_DATA_REQ)
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_ho_from_eutra_prep_req
(
    uecc_ue_context_t*  p_uecc_ue_context        /* UE context */
);

/* CDMA2000_CSFB_HO stop*/
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind
*
*   DESCRIPTION:
*       This function builds and sends rrc_uecc_llim_reest_ue_entity_comp_ind message
*       to LLIM module. 
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind
(
    uecc_ue_context_t*  p_uecc_ue_context
);


/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_drb_count_msb_resp
*
*   DESCRIPTION:
*   This function processes UECC_LLIM_DRB_COUNT_MSB_RESP message
*
*   RETURNS:
*       None
*
******************************************************************************/

void uecc_llim_process_drb_count_msb_resp
(
   void                *p_api,           
   uecc_gb_context_t   *p_uecc_gb_context 
);

/******************************************************************************
*   FUNCTION NAME: 
*       uecc_llim_build_and_send_counter_check_req
*
*   DESCRIPTION:
*       This function builds and sends UECC_LLIM_SRB_DATA_REQ message
*       with CounterCheckReq to LLIM
*
*   RETURNS:
*       RRC_FAILURE/RRC_SUCCESS
*
******************************************************************************/
rrc_return_et uecc_llim_build_and_send_counter_check_req(
        uecc_ue_context_t*  p_uecc_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_dl_dcch_counter_check_msg
*
*   DESCRIPTION:
*       This function builds the DL-DCCH message for
*       Counter Check message sent to LLIM
*
*   RETURNS:
*       RRC_FAILURE/RRC_SUCCESS
*
******************************************************************************/
rrc_return_et uecc_llim_build_dl_dcch_counter_check_msg
(
    DL_DCCH_Message*              p_dl_dcch_msg,
    OSCTXT*                       p_asn1_ctx,
    uecc_ue_context_t*            p_uecc_ue_context
);

rrc_return_et uecc_llim_build_and_send_drb_count_msb_req(
        uecc_ue_context_t   *p_uecc_ue_context,
        void                *p_api
);


/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_rrc_ue_information 
 
 
 ****************************************************************************/
rrc_return_et uecc_llim_build_and_send_rrc_ue_information
(
 uecc_ue_context_t*  p_uecc_ue_context,
 void                *p_api
);

/****************************************************************************
 * Function Name  : uecc_llim_ue_information_resp_handler 
 
 * Description:     This  function process UECC_LLIM_UE_INFORMATION_RESP_HANDLER
 ****************************************************************************/
void uecc_llim_ue_information_resp_handler
(
 uecc_ue_context_t*  p_uecc_ue_context,
 void                *p_api
);

/****************************************************************************
 * Function Name  : rrc_uecc_fill_protocol_event_params 
 *
 * Description    : This  function fills the parameters for the specific 
 * 		    Protocol Event
  
 ****************************************************************************/
void rrc_uecc_fill_protocol_event_params
(
    uecc_ue_context_t *p_uecc_ue_context,
    EVENT_EXTERNAL_HEADER *p_event_header,
    U32 total_length,
    U16 event_id,
    U32 message_direction
);

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_and_send_mac_reconfig_complete_ind
 *
 *   DESCRIPTION:
 *       This function sends to LLIM UECC_LLIM_MAC_RECONFIG_COMPLETE_IND message
 *
 ******************************************************************************/
void uecc_llim_build_and_send_mac_reconfig_complete_ind
(
    uecc_gb_context_t   *p_uecc_gb_context,
    uecc_ue_context_t*  p_uecc_ue_context,
    U8    response
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_check_and_send_rrc_ue_information
*
*   DESCRIPTION:
*       This function is used to check for the condition to send
*       ue information request
*
*   RETURNS:
*       None
*
******************************************************************************/

void uecc_llim_check_and_send_rrc_ue_information
(
    uecc_ue_context_t*  p_ue_context
);

/*SPR:6462 start*/
/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_reestab_comp_cnf
*
*   DESCRIPTION:
*   This function processes UECC_LLIM_REESTAB_UE_ENTITY_COMPLETE_CNF message
*
*   RETURNS:
*       None
*
******************************************************************************/

void uecc_llim_process_reestab_comp_cnf
(
   void                *p_api,           
   uecc_gb_context_t   *p_uecc_gb_context 
);

/*SPR:6462 stop*/








/*BUG 604 changes start*/
/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_intra_enb_data_fwd_cnf
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_CNF 
*       message
*
*   RETURNS:
*       None
*
 ******************************************************************************/
void uecc_llim_process_intra_enb_data_fwd_cnf
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_process_local_path_switch_cnf
*
*   DESCRIPTION:
*       This function processes LLIM UECC_LLIM_S1U_LOCAL_PATH_SWITCH_CNF 
*       message
*
*   RETURNS:
*       None
*
 ******************************************************************************/
void uecc_llim_process_local_path_switch_cnf
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_s1u_intra_enb_data_fwd_req
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t 
 * Description:    This is the function for building and sending
 *                   UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_REQ message to the LLIM.
 ****************************************************************************/

rrc_return_t uecc_llim_build_and_send_s1u_intra_enb_data_fwd_req(
        uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
        );

/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_s1u_path_switch_req
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t 
 * Description:    This is the function for building and sending
 *                   UECC_LLIM_S1U_LOCAL_PATH_SWITCH_REQ message to the LLIM.
 ****************************************************************************/

rrc_return_t uecc_llim_build_and_send_s1u_path_switch_req(
        uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
        );

/*BUG 604 changes stop*/
/*Bug 357 Fix Start*/
/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_re_est_req_for_srbs 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t 
 * Description:    This is the function for building and sending
 *                   UECC_LLIM_RE_EST_REQ message to the LLIM.
 ****************************************************************************/

rrc_return_t uecc_llim_build_and_send_re_est_req_for_srbs(
        uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
        );

/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_pdcp_re_est_req
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t 
 * Description:    This is the function for building and sending
 *                   UECC_LLIM_PDCP_RE_EST_REQ message to the LLIM.
 ****************************************************************************/

rrc_return_t uecc_llim_build_and_send_pdcp_re_est_req(
        uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
        );

/*Bug 357 Fix Stop*/

/* TRACE_changes_start */
void uecc_rrc_ue_assoc_msg_max_trace_handler( 
              uecc_ue_context_t  *p_ue_context, 
              U8                 *buffer, 
              U32                buffer_len,
              /* SPR 16113 Start */
              char               *msg_name,
              char               *protocol_name);
              /* SPR 16113 Stop */
/* TRACE_changes_start */

/* Bug 817 Fix Start */
/****************************************************************************
 * Function Name  : uecc_llim_build_and_send_delete_trg_forwarding_tunnel_req 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : rrc_return_t 
 * Description:    This is the function for building and sending
 *                   UECC_LLIM_S1U_RECONFIG_REQ to delete forwarding tunnels 
 *                   message to the LLIM.
 ****************************************************************************/

rrc_return_t uecc_llim_build_and_send_delete_trg_forwarding_tunnel_req(
        uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
        );
/* Bug 817 Fix Stop */

/* eICIC changes start */
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_meas_subframe_pattern_pcell_r10
*   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
*                  rrc_meas_subframe_pattern_pcell_r10_t           *p_meas_subframe
*                  MeasSubframePatternPCell_r10                    *p_asn1_meas_subframe
*                  OSCTXT                                          *p_asn1_ctx
*   OUTPUT       : none
*   DESCRIPTION:   Builds ASN1 rrc_meas_subframe_pattern_pcell_r10 IE based on
*                  configuration provided by RRM.
*
*   RETURNS:       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_meas_subframe_pattern_pcell_r10(
    uecc_ue_context_t                               *p_uecc_ue_context,
    rrc_meas_subframe_pattern_pcell_r10_t           *p_meas_subframe,
    MeasSubframePatternPCell_r10                    *p_asn1_meas_subframe,
    OSCTXT                                          *p_asn1_ctx
);
/* eICIC changes stop */


rrc_return_et 
uecc_llim_init_asn_context
(
 OSCTXT             *p_asn1_ctx,
 uecc_ue_context_t  *p_uecc_ue_context
 );
rrc_return_et uecc_llim_build_dl_dcch_msg
(
 DL_DCCH_Message           *p_dl_dcch_msg,
 OSCTXT                    *p_asn1_ctx,
 uecc_ue_context_t         *p_uecc_ue_context,
 RRCConnectionReconfiguration** p_rrc_conn_recfg
 );
rrc_return_et uecc_llim_fill_erb_modify_srb_info
(
 RRCConnectionReconfiguration_r8_IEs    *p_r8,
 OSCTXT                                 *p_asn1_ctx,
 uecc_ue_context_t                      *p_uecc_ue_context
 );
rrc_return_et uecc_llim_send_rrc_connection_reconfiguration(
        uecc_ue_context_t*                       p_uecc_ue_context,
        OSCTXT*                                  p_asn1_ctx,
        DL_DCCH_Message*                  p_dl_dcch_msg
        );
rrc_return_et uecc_llim_build_asn1_drb_to_add_mod_new(
    uecc_ue_context_t*      p_uecc_ue_context,
    rrm_drb_config_t const  *p_drb,
    DRB_ToAddMod            *p_asn1_drb_elem,
    OSCTXT                  *p_asn1_ctx
);


#ifdef ENDC_ENABLED
rrc_return_t
uecc_llim_build_and_send_delete_new_lc_s1u_reconfig_req(
        uecc_ue_context_t*    p_uecc_ue_context
        );

/*NR_DC Code Change Start*/
typedef enum{
    ICS_SETUP = 1,
    ERAB_SETUP = 2,
    ERAB_RELEASE = 3
}uecc_currcent_proc_et;
void uecc_llim_build_nr_rrc_connection_reconfig_r15_ies(
        OSCTXT*                                 p_asn1_ctx,
        RRCConnectionReconfiguration_r8_IEs*    p_r8,     
        uecc_ue_context_t*    p_uecc_ue_context,
        uecc_currcent_proc_et current_proc
        );
/*NR_DC Code Change Stop*/
#endif
#endif /* UECC_LLIM_H */

