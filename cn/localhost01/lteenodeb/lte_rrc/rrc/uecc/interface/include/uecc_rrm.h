/******************************************************************************
*
*   FILE NAME:
*       uecc_rrm.h
*
*   DESCRIPTION:
*       This is the interface file of the UECC RRM.
*       UECC RRM interacts with RRC RRM module.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   23 Jul 2009     GrygoriyS   ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef UECC_RRM_H
#define UECC_RRM_H

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "uecc_global_ctx.h"
#include "rrc_s1ap_asn_intrl_3gpp.h"

/****************************************************************************
 * Public functions definitions
 ****************************************************************************/
void uecc_rrm_process_ue_admission_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

void uecc_rrm_process_ue_reconfig_request
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

void uecc_rrm_process_erb_setup_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

void uecc_rrm_process_ue_connection_release_ind
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);
void uecc_rrm_process_ue_capability_enquiry
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);
void uecc_rrm_process_ue_release_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);
/*REL 1.2:HANDOVER PROCEDURE START*/
void uecc_rrm_process_ho_required
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

void uecc_rrm_process_ue_ho_adm_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

void uecc_rrm_process_ho_cancel_req
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

void uecc_rrm_process_ue_ho_cmd_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

rrc_return_et process_fill_ho_restriction_list(
        uecc_ue_context_t* p_uecc_ue_context,
        ho_restriction_list_t * p_ho_restriction_list,
        s1ap_HandoverRestrictionList * p_s1ap_HandoverRestrictionList
);

rrc_return_et process_fill_x2ap_ho_restriction_list(
        uecc_ue_context_t*              p_uecc_ue_context,
        ho_restriction_list_t*          p_ho_restriction_list,
        x2ap_HandoverRestrictionList*   p_x2ap_HandoverRestrictionList
);


rrc_return_et uecc_rrm_build_and_send_counter_check_failure(
        uecc_ue_context_t               *p_ue_context,      
        U8                               fail_cause
);

void uecc_rrm_counter_check_req_handler(
       uecc_ue_context_t   *p_ue_context,
       void                *p_api
);

void uecc_rrm_counter_check_start_timer(
        uecc_ue_context_t   *p_ue_context
);

void uecc_rrm_counter_check_stop_timer(
        uecc_ue_context_t   *p_ue_context
);

rrc_return_et uecc_llim_build_and_send_drb_count_msb_req(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
);

rrc_return_et uecc_rrm_build_and_send_counter_check_success(
        uecc_ue_context_t   *p_ue_context,
        CounterCheckResponse_r8_IEs    *p_counterCheckResponse_r8
);


/*REL 1.2:HANDOVER PROCEDURE END*/
/* Measurment Re-config START */
/******************************************************************************
*   FUNCTION NAME: uecc_rrm_process_meas_config_req
*
*   DESCRIPTION:
*       This function processes RRM RRC_RRM_MEAS_CONFIG_REQ
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_rrm_process_meas_config_req
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/* Measurment Re-config END */

rrc_return_t uecc_rrm_build_and_send_ue_admission_req(
    uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
);


rrc_return_et uecc_rrm_build_and_send_ue_admission_cnf
(
    uecc_ue_context_t*  p_uecc_ue_context,          /* UECC UE context */
    rrc_return_et       rrm_response
);

rrc_return_et uecc_rrm_build_and_send_erb_setup_req(
    uecc_ue_context_t*  p_ue_context            /* UECC UE context */
);

rrc_return_et uecc_rrm_build_and_send_erb_setup_conf(
        uecc_ue_context_t*  p_ue_context);

rrc_return_et uecc_rrm_build_and_send_ue_release_req(
    uecc_ue_context_t*  p_ue_context            /* UECC UE context */
);
rrc_return_et uecc_rrm_build_and_send_ue_capability_info(
    uecc_ue_context_t               *p_ue_context,      /* UECC UE context */
    UECapabilityInformation *p_UECapabilityInformation
);

rrc_return_et uecc_rrm_build_ue_rat_capability(
    uecc_ue_context_t               *p_ue_context,      
    rrc_radio_capability_info_t     *p_rrc_radio_capability_info,
    void                            *p_api
);

rrc_return_et uecc_rrm_build_meas_results_eutra(
         meas_result_list_eutra_t*    p_meas_results_eutra,
         MeasResultListEUTRA*        p_asn_meas_results_eutra);

rrc_return_et uecc_rrm_build_and_send_failure_ue_capability_info_to_rrm(
    uecc_ue_context_t               *p_ue_context      /* UECC UE context */
);
rrc_return_t uecc_save_ue_rat_capability(
        uecc_ue_context_t  *p_ue_context,
        void                *p_api
);
rrc_return_et uecc_rrm_build_and_send_measurment_results_ind(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    void*               p_data);

rrc_return_et uecc_rrm_build_and_send_proximity_indication_ind(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    void*               p_data);
/*HANDOVER_CHANGES_REVANTH_01_begin*/
rrc_return_et uecc_rrm_build_and_send_ue_ho_adm_req(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_s1ap_handover_request_t *p_rrc_s1ap_ho_request
);
rrc_return_et uecc_rrm_build_and_send_ue_ho_adm_cnf(
    uecc_ue_context_t*  p_ue_context,
    rrc_return_et response
);
/*HANDOVER_CHANGES_REVANTH_01_end*/

rrc_return_et uecc_rrm_build_and_send_x2ap_ue_ho_adm_req(
    uecc_ue_context_t*              p_uecc_ue_context,
    rrc_x2ap_handover_request_t*    p_rrc_x2ap_ho_request
);

rrc_return_t uecc_rrm_build_and_send_handover_command(
        uecc_ue_context_t   *p_uecc_ue_context      
        );

rrc_return_t uecc_rrm_build_and_send_ho_failure(
        uecc_ue_context_t   *p_uecc_ue_context      
        );


rrc_return_t uecc_rrm_build_and_send_ho_cancel_resp(
        uecc_ue_context_t   *p_uecc_ue_context,     
        rrc_return_et       response
        );
void uecc_rrm_process_ue_context_modification_resp
(
 void                *p_api,             /* Input API buffer */
 uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);
/*Start E-RAB Modify*/
void uecc_rrm_process_erb_modify_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

rrc_return_et uecc_rrm_build_and_send_erb_modify_req(
    uecc_ue_context_t*  p_ue_context            /* UECC UE context */
);

rrc_return_et uecc_rrm_build_and_send_erb_modify_conf
(
    uecc_ue_context_t*  p_ue_context,
    U16                 response,
    U32                 error_code
);
/*End E-RAB Modify*/


/* ERB RELEASE COMMAND START */
rrc_return_et uecc_rrm_build_and_send_erb_release_req(
    uecc_ue_context_t*  p_ue_context          /* UECC UE context */
);
void uecc_rrm_process_erb_release_resp
(
    void*                p_api,             /* Input API buffer */
    uecc_gb_context_t*   p_uecc_gb_context  /* UECC global context */
);
void uecc_rrm_process_erb_release_ind
(
    void*                p_api,             /* Input API buffer */
    uecc_gb_context_t*   p_uecc_gb_context  /* UECC global context */
);
rrc_return_et uecc_rrm_build_and_send_erb_release_conf(
    uecc_ue_context_t*  p_ue_context,
    rrc_response_et       response
);
rrc_return_et uecc_rrm_build_rrm_cause(
    rrm_cause_t*       p_rrm_cause,
    s1ap_Cause*        p_cause,
    uecc_ue_context_t* p_uecc_ue_context
);

rrc_return_et uecc_rrm_build_rrm_x2ap_cause(
    rrm_cause_t*       p_rrm_cause,
    x2ap_Cause*        p_cause,
    uecc_ue_context_t* p_uecc_ue_context
);
rrc_return_et uecc_rrm_build_and_send_ue_ho_adm_cnf_failure(
    uecc_ue_context_t*  p_ue_context
);
/* ERB RELEASE COMMAND END */
/******************************************************************************
 *   FUNCTION NAME: uecc_rrm_build_and_send_ue_context_mod_req
 *
 *   DESCRIPTION:    This is the function for building and sending
 *                   RRC_RRM_UE_ADMISSION_REQ message to the RRM.
 *
 *   RETURNS:        None
 *
 ******************************************************************************/
rrc_return_et uecc_rrm_build_and_send_ue_context_mod_req(
    uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
);
/******************************************************************************
*   FUNCTION NAME: uecc_rrm_build_and_send_erb_release_ind_fail_resp
*
*   DESCRIPTION:
*       This function builds and sends RRC_RRM_ERB_RELEASE_IND_CNF message
*       with failure to RRM module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_rrm_build_and_send_erb_release_ind_fail_resp(
    void                *p_api,
    uecc_ue_context_t   *p_ue_context,
    rrc_response_et     response,
    U32                 error_code
);
/* Measurment Re-config START */
/******************************************************************************
*   FUNCTION NAME: uecc_rrm_build_and_send_meas_config_resp
*
*   DESCRIPTION:    This is the function for building and sending
*                   RRC_RRM_MEAS_CONFIG_RESP message to the RRM.
*
*   RETURNS:        None
*
******************************************************************************/
rrc_return_t uecc_rrm_build_and_send_meas_config_resp(
        uecc_ue_context_t   *p_uecc_ue_context,      /* UECC UE context */
        rrc_return_et        response,
        U32                  error_code
);
/* meas_config change start */
rrc_return_et uecc_build_rrm_curr_meas_config(
    uecc_ue_context_t*  p_uecc_ue_context,
    uecc_ue_curr_meas_config_t*   p_uecc_curr_meas_config,
    MeasConfig*         p_meas_config
);

rrc_return_et uecc_build_rrm_meas_config_for_remove_list(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrm_meas_config_t*  p_rrm_meas_config,
    MeasConfig*         p_meas_config
);

rrc_return_et uecc_rrm_build_rrm_meas_config(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrm_meas_config_t*   p_rrm_meas_config,
    MeasConfig*         p_meas_config
);
/* meas_config change end*/

/* Measurment Re-config END */

void uecc_s1ap_generate_plmn_identity_to_int(
    plmn_identity_t    *p_plmn_identity,
    const OSOCTET      *p_s1ap_plmn_identity
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_and_send_rrc_rrm_intra_enb_ho_ind
*
*   DESCRIPTION:    This is the function for building and sending
*                   RRC_RRM_INTRA_ENB_HO_IND message to the RRM.
*
*   RETURNS:        None
*
******************************************************************************/
rrc_return_t uecc_llim_build_and_send_rrc_rrm_intra_enb_ho_ind(
        uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
);




/******************************************************************************
* FUNCTION NAME:  uecc_rrm_build_and_send_csfb_parameters_req_cdma2000
*
* DESCRIPTION:    This is the function for building and sending
*                 RRC_RRM_CSFB_PARAMETERS_REQ_CDMA2000 message to the RRM.
*
* RETURNS:        None
*
******************************************************************************/
rrc_return_t uecc_rrm_build_and_send_csfb_parameters_req_cdma2000(
    uecc_ue_context_t   *p_uecc_ue_context
);

/******************************************************************************
* FUNCTION NAME:  uecc_rrm_process_csfb_parameters_resp_cdma2000
*
* DESCRIPTION:    This is the function for receives
*                 RRC_RRM_CSFB_PARAMETERS_RESP_CDMA2000 message from RRM
*                 decodes it and posts event to the mainfsm.
*
* RETURNS:        None
*
******************************************************************************/

void uecc_rrm_process_csfb_parameters_resp_cdma2000(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
* FUNCTION NAME :validate_ho_cause 
*
* DESCRIPTION   :This is the function for validating the handover cause when
*                handover type is INTER_CELL_HO.
*
* RETURNS       :RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_t validate_ho_cause
(   
    rrm_cause_t *p_rrm_cause,
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME:uecc_rrm_parse_and_update_sps_config 
*
*   DESCRIPTION:
*       This function update SPS config 
*
*   RETURNS:
*       None
*
******************************************************************************/
/* SPS Start */
rrc_return_et uecc_rrm_parse_and_update_sps_config(
        uecc_ue_context_t*  p_ue_context,
        rrc_sps_config_t *p_rrm_sps_config);
/* SPS Stop */

/******************************************************************************
*   FUNCTION NAME: uecc_rrm_process_counter_check_req
*
*   DESCRIPTION:
*       This function processes RRM RRC_RRM_COUNTER_CHECK_REQ
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_rrm_process_counter_check_req
(
   void*                p_api,            
   uecc_gb_context_t*   p_uecc_gb_context
);


void uecc_rrm_process_ue_information_req(
        void                *p_api, /* Input API Buffer*/
        uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

rrc_return_et uecc_rrm_build_and_send_ue_information_resp(
        uecc_ue_context_t               *p_ue_context,      /* UECC UE context */
        UEInformationResponse_r9    *p_UEInformationResp
);

void uecc_fsm_rrm_ue_information_req_start_timer(
    uecc_ue_context_t   *p_ue_context
);

void uecc_rrm_ue_information_req_stop_timer(
    uecc_ue_context_t   *p_ue_context
);

void uecc_rrm_ue_information_req_timer_expiry_handler(
    uecc_gb_context_t   *p_uecc_gb_context,
    uecc_timer_buffer_t*    p_uecc_timer_buffer    /* Timer buffer */
);

rrc_return_et uecc_rrm_build_and_send_rrc_rrm_ue_information_failure(
    uecc_ue_context_t               *p_ue_context,      /* UECC UE context */
    U8                               fail_cause
);

rrc_return_et uecc_rrm_build_and_send_failure_ue_information_info_to_rrm(
    uecc_ue_context_t               *p_ue_context      /* UECC UE context */
);

rrc_return_t uecc_rrm_build_and_send_ue_info_resp(
        uecc_ue_context_t   *p_uecc_ue_context,      /* UECC UE context */
        rrc_return_et        response,
        U32                  error_code
);

void uecc_rrm_ue_information_req_handler(
        uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
        void                *p_api              /* Input API buffer */
);

/*CR 895 changes start */
rrc_return_et uecc_rrm_build_ue_eutra_capability_v9a0(
        uecc_ue_context_t               *p_ue_context,      /* Added UE context */        
        ue_eutra_capability_v9a0_ies_t *p_ue_eutra_capability,
        UE_EUTRA_Capability_v9a0_IEs   *p_UE_EUTRA_Capability_v9a0
);
/*CR 895 changes stop */

/* CR 919 changes start*/
void fill_ue_eutra_capability_v1060_ies( 
    UE_EUTRA_Capability_v1060_IEs   *p_asn_UE_EUTRA_Capability_v1060_IEs,
    ue_eutra_capability_v1060_ies_t *p_ue_eutra_capability_v1060_ies
);


void fill_phylayer_params_v1060(
    PhyLayerParameters_v1020   *p_asn_PhyLayerParameters_v1060, 
    phylayer_parameters_v1020_ies_t *p_phylayer_parameter_v1060 
);


/* CR 919 changes stop*/
rrc_return_et uecc_rrm_build_and_send_inter_freq_meas_ind(
        uecc_ue_context_t   *p_uecc_ue_context,      /* UECC UE context */
        InterFreqRSTDMeasurementIndication_r10_IEs_rstd_InterFreqIndication_r10 *p_rstd_inter_freq_ind
);

/* lipa start */
rrc_return_et uecc_rrm_build_and_send_rrc_rrm_local_error_ind(
       uecc_gb_context_t  *p_uecc_gb_context,     
       U8                  cause,
       void               *p_api
);

rrc_return_et uecc_rrm_build_and_send_rrc_rrm_local_error_ind_for_s1u_error(
  uecc_ue_context_t      *p_ue_context,   
  rrc_uecc_llim_s1u_error_ind_t *p_data  
);
/* SPR 16053 Fix Start */
rrc_return_t uecc_rrm_build_and_send_ue_reestab_complete_ind(
        uecc_ue_context_t   *p_uecc_ue_context,      /* UECC UE context */
        rrc_return_et        response
);
/* SPR 16053 Fix Stop */

/*BUG 604 changes start*/
/******************************************************************************
 *   FUNCTION NAME: uecc_rrm_build_and_send_ue_context_mod_ind
 *
 *   DESCRIPTION:    This is the function for building and sending
 *                   RRC_RRM_UE_CONTEXT_MOD_IND message to the RRM.
 *
 *   RETURNS:        None
 *
 ******************************************************************************/
rrc_return_et uecc_rrm_build_and_send_ue_context_mod_ind(
    uecc_ue_context_t   *p_uecc_ue_context      /* UECC UE context */
);
/*BUG 604 changes stop*/

/* lipa end */

/* SPR 9343 Start */
rrc_return_et uecc_rrm_build_and_send_ue_reconfig_resp(  uecc_ue_context_t*  p_ue_context,
                                                         U8                  response,
                                                         U32                 fail_cause);
/* SPR 9343 Stop */

rrc_return_et rrc_rrm_validate_scell_config (
        rrm_scell_config_t* p_scell_config,
        uecc_ue_context_t*  p_uecc_ue_context);

void uecc_rrm_update_ue_ctx_scell_config (rrm_scell_config_t* p_scell_config,
        uecc_ue_context_t*  p_uecc_ue_context);

/******************************************************************************
*   FUNCTION NAME: uecc_rrm_build_scell_config_info
*
*   DESCRIPTION  : This function builds as_config_ext_t   
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_rrm_build_scell_config_info (
        uecc_ue_context_t   *p_ue_context,
        as_config_ext_t     *p_rrm_as_config_ext,
        SCellToAddModList_r10    *p_sourceSCellConfigList_r10 
);


/******************************************************************************
*   FUNCTION NAME: uecc_rrm_build_radio_res_config_common_scell
*
*   DESCRIPTION  : This function builds rrc_radio_res_config_common_scell_t
*
*   RETURNS      : None
*
******************************************************************************/
void uecc_rrm_build_radio_res_config_common_scell (
        uecc_ue_context_t   *p_ue_context,
        rrc_radio_res_config_common_scell_t  *p_config_common_scell,
        RadioResourceConfigCommonSCell_r10   *p_radioResourceConfigCommonSCell_r10
);



/******************************************************************************
*   FUNCTION NAME: uecc_rrm_build_radio_res_config_dedicted_scell
*
*   DESCRIPTION  : This function builds rrc_radio_res_config_dedicated_scell_t
*
*   RETURNS      : None
*
******************************************************************************/
void uecc_rrm_build_radio_res_config_dedicted_scell (
        uecc_ue_context_t   *p_ue_context,
        rrc_radio_res_config_dedicated_scell_t  *p_config_dedicated_scell,/*dest*/
        RadioResourceConfigDedicatedSCell_r10   *p_radio_Res_Config_Dedicated_SCell/*src*/
);


/******************************************************************************
*   FUNCTION NAME: uecc_rrm_build_phy_config_dedicated_non_ul_config
*
*   DESCRIPTION  : This function builds
*                  rrc_radio_res_dedicated_scell_non_ul_config_t
*
*   RETURNS      : None
*
******************************************************************************/
void uecc_rrm_build_phy_config_dedicated_non_ul_config (
        uecc_ue_context_t                       *p_ue_context,
        rrc_radio_res_dedicated_scell_non_ul_config_t *p_scell_non_ul_config,
        PhysicalConfigDedicatedSCell_r10_nonUL_Configuration_r10
                                                *p_nonUL_Configuration_r10
);

/******************************************************************************
*   FUNCTION NAME: uecc_rrm_build_phy_config_dedicated_ul_config
*
*   DESCRIPTION  : This function builds
*                  rrc_radio_res_dedicated_scell_ul_config_t
*
*   RETURNS      : None
*
******************************************************************************/
void uecc_rrm_build_phy_config_dedicated_ul_config (
        uecc_ue_context_t                *p_ue_context,
        rrc_radio_res_dedicated_scell_ul_config_t *p_scell_ul_config,/*dest*/
        PhysicalConfigDedicatedSCell_r10_ul_Configuration_r10
                                         *p_ul_Configuration_r10    /*src*/
);

/******************************************************************************
*   FUNCTION NAME: uecc_rrm_build_phy_cqi_report_periodic_r10_setup 
*
*   DESCRIPTION  : This function builds
*                  rrc_phy_cqi_report_periodic_r10_setup_t
*
*   RETURNS      : None
*
******************************************************************************/
void uecc_rrm_build_phy_cqi_report_periodic_r10_setup (
        uecc_ue_context_t                *p_ue_context,
        rrc_phy_cqi_report_periodic_r10_setup_t *p_cqi_report_periodic_setup,/*Dest*/
        CQI_ReportPeriodic_r10_setup *p_setup   /*src*/
);

/******************************************************************************
*   FUNCTION NAME: uecc_rrm_build_rrm_config_candidate_cell_info
*
*   DESCRIPTION  : This function builds
*                  rrc_phy_cqi_report_periodic_r10_setup_t
*
*   RETURNS      : None
*
******************************************************************************/
void uecc_rrm_build_rrm_config_candidate_cell_info (
        uecc_ue_context_t    *p_ue_context,
        candidate_cell_info_list_t  *p_candidate_cell_info_list, /*dest*/
        CandidateCellInfoList_r10   *p_candidateCellInfoList_r10 /*src*/
);

rrc_return_et fill_phyConfigDed_antennaInfo_r10 (
        uecc_ue_context_t                   *p_ue_context,
	    PhysicalConfigDedicated             *p_asn1_phy,    /*src*/
        rrc_phy_physical_config_dedicated_t *p_rrm_physical_config_dedicated    /*dest*/
);

rrc_return_et fill_phyConfigDed_CQI_ReportConfig_r10 (
        uecc_ue_context_t                   *p_ue_context,
	    PhysicalConfigDedicated             *p_asn1_phy,    /*src*/
        rrc_phy_physical_config_dedicated_t *p_rrm_physical_config_dedicated    /*dest*/
);

rrc_return_et fill_phy_cqi_report_periodic_r10 (
        uecc_ue_context_t                   *p_ue_context,
        CQI_ReportPeriodic_r10 cqi_ReportPeriodic_r10, /*src*/
        rrc_phy_cqi_report_periodic_r10_t   *p_rrm_cqi_report_periodic_r10 /*dest*/
);

rrc_return_et fill_phy_cqi_report_csi_subframePattern_r10 (
        uecc_ue_context_t    *p_ue_context,
        CQI_ReportConfig_r10_csi_SubframePatternConfig_r10
                             asn_csi_SubframePattern,   /*src*/
        rrc_phy_cqi_report_csi_subframePattern_r10_t
                             *p_rrm_csi_subframePattern /*dest*/
);


rrc_return_et fill_phy_meas_subframe_pattern_r10 (
        uecc_ue_context_t    *p_ue_context,
        MeasSubframePattern_r10  asn_MeasSubframeSet1_r10,  /*src*/
        rrc_phy_meas_subframe_pattern_r10_t *p_rrm_subframe_set1_r10 /*dest*/
);


rrc_return_et fill_phyConfigDed_additional_spectrum_emission_ca_r10 (
        uecc_ue_context_t                   *p_ue_context,
	    PhysicalConfigDedicated             *p_asn1_phy,    /*src*/
        rrc_phy_physical_config_dedicated_t *p_rrm_physical_config_dedicated    /*dest*/
);


rrc_return_et fill_phyConfigDed_pucch_channel_selection_config_v1020 (
        uecc_ue_context_t                   *p_ue_context,
	    PhysicalConfigDedicated             *p_asn1_phy,    /*src*/
        rrc_phy_physical_config_dedicated_t *p_rrm_physical_config_dedicated    /*dest*/
);


rrc_return_et uecc_rrm_process_scell_config
(
 uecc_ue_context_t    *p_ue_context,
 rrm_scell_config_t   *p_msg_scell_config   
);

void  rrc_rrm_update_ue_ctx_scell_config
(
 rrm_scell_config_t *p_scell_config,
 uecc_ue_context_t*  p_uecc_ue_context
);

void uecc_rrm_modify_scell_config
(
 rrc_scell_to_add_mod_t  *p_rrm_scell_to_add_mod, /*src*/
 rrc_scell_to_add_mod_t  *p_ue_ctx_scell_to_add_mod, /*trg*/
 uecc_ue_context_t       *p_uecc_ue_context
);

void uecc_rrm_update_dedicated_scell_non_ul_config
(
 rrc_radio_res_dedicated_scell_non_ul_config_t   *p_rrm_scell_non_ul_config,     /*src*/
 rrc_radio_res_dedicated_scell_non_ul_config_t   *p_ue_ctx_scell_non_ul_config,  /*dest*/
 uecc_ue_context_t                               *p_uecc_ue_context
);



void uecc_rrm_update_dedicated_scell_ul_config (
        rrc_radio_res_dedicated_scell_ul_config_t   *p_rrm_scell_ul_config,     /*src*/
        rrc_radio_res_dedicated_scell_ul_config_t   *p_ue_ctx_scell_ul_config,  /*dest*/
        uecc_ue_context_t                           *p_uecc_ue_context
 );


void uecc_rrm_update_cqi_format_ind_periodic (
        rrc_phy_cqi_format_ind_periodic_r10_t   *p_rrm_cqi_format_ind_periodic, /*src*/
        rrc_phy_cqi_format_ind_periodic_r10_t   *p_ue_ctx_cqi_format_ind_periodic, /*dest*/
        uecc_ue_context_t                       *p_uecc_ue_context);

/*CA_PHASE3_RE_ESTAB_START*/
rrc_return_et uecc_rrm_build_and_send_ue_reestab_config_cnf(
        uecc_ue_context_t*  p_ue_context, /*UECC UE Context*/
        U8                  response);
/*CA_PHASE3_RE_ESTAB_STOP*/
#ifdef ENDC_ENABLED
void uecc_rrm_process_dc_bearer_change_req
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

rrc_return_t uecc_rrm_build_and_send_dc_bearer_change_response
(
 uecc_ue_context_t               *p_ue_context
 );

/******************************************************************************
*   function name: uecc_rrm_build_and_send_dc_bearer_change_cnf
*   input        : uecc_ue_context_t    *p_ue_context
*   output       : none 
*   return       : none
*   description  : this function builds and sends rrc_rrm_dc_bearer_change_cnf
*                  message to rrm.
******************************************************************************/
rrc_return_t
uecc_rrm_build_and_send_dc_bearer_change_cnf
(
    uecc_ue_context_t               *p_ue_context,
    U8                              response 
);
#endif
#endif /* UECC_RRM_H */
