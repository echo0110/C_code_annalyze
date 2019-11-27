/*! \file  rrm_ue_fsm.h
 *  \brief This file contains the prototypes for UE FSM.
 *  \date January 30, 2012
 *  \author gur30239
*/
/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
*/

#ifndef _RRM_UE_FSM_H_
#define _RRM_UE_FSM_H_

/*! \headerfile <> rrm_ue_context.h 
*/
#include "rrm_ue_context.h"

/*! \fn pointer typedef declaration for event handler */
typedef rrm_return_et (*rrmh_fsm_ptr)(rrm_ue_global_context_t *);

/*! \fn  rrm_de_allocate_tpc_rnti(U16 tpc_rnti_pucch,U16 tpc_rnti_pusch,U8 tpc_rnti_index_pucch,U8 tpc_rnti_index_pusch ,rrm_cell_index_t cell_index)
 *  \brief      This function will deallocate TPC RNTI
 *  \param  tpc_rnti_pucch  TPC RNTI PUCCH
 *  \param  tpc_rnti_pusch  TPC RNTI PUSCH
 *  \param  tpc_rnti_index_pucch  tpc rnti index pucch
 *  \param  tpc_rnti_index_pusch  tpc rnti index pusch
 *  \param  cell_index      Index of Cell.
 */
void
rrm_de_allocate_tpc_rnti(
        U16 tpc_rnti_pucch,
        U16 tpc_rnti_pusch,
        U8 tpc_rnti_index_pucch,
        U8 tpc_rnti_index_pusch,
        rrm_cell_index_t
        cell_index
        );


/*! \fn	return_et rrm_ue_handle_ue_admission_cnf(rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE Admission conf 
 */
rrm_return_et
rrm_ue_handle_ue_admission_cnf (
								 rrm_ue_global_context_t *
						       );

/*! \fn rrm_return_et rrm_ue_does_erb_exist(
						const rrm_ue_erab_setup_item_list_t *p_erb_setup_item_list,
						const rrm_ue_context_t *p_ue_context,
						U8 *erb_exist)
 *  \brief              This function will determine that erb exist already or not
 *  \param p_erb_setup_item_list Pointer to rrm_ue_erab_setup_item_list_t
 *  \param p_ue_context Pointer to rrm_ue_context_t
 *  \param erb_exist Pointer to unsigned char
 */
rrm_return_et
rrm_ue_does_erb_exist(
                      const rrm_ue_erab_setup_item_list_t     *p_erb_setup_item_list,
                      const rrm_ue_context_t                  *p_ue_context,
                      U8                                      *erb_exist
                                        );


/*! \fn	rrm_return_et rrm_ue_handle_erb_setup_req(rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE erab setup request
 */
rrm_return_et
rrm_ue_handle_erb_setup_req (
							     rrm_ue_global_context_t *
						    );


/*LTE_RRM_KLOCWORK_WARN_9_JULY_START*/
/*! \fn  rrm_return_et rrm_ue_handle_cs_fallback (const rrm_ue_global_context_t *p_rrm_glb_ctxt,rrm_bool_et *is_ue_released)
 *  \brief This function will handle the UE cs fallback.
 *  \param p_rrm_glb_ctxt pointer to the global context
 *  \param is_ue_released Boolean type variable
 */
rrm_return_et
rrm_ue_handle_cs_fallback (
				const rrm_ue_global_context_t         *p_rrm_glb_ctxt,
                rrm_bool_et *is_ue_released
						    );
/*LTE_RRM_KLOCWORK_WARN_9_JULY_END*/
/*SPR 17370 START*/

rrm_return_et
rrm_check_for_common_cause_in_failed_list(
        rrm_ue_erab_failed_item_list_t    *p_failed_erbs
        );
/*SPR 17370 END*/

/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
rrm_return_et
rrm_check_is_configured_peer_gnb_pci_valid
(
    rrm_cell_index_t  cell_index
);

rrm_bool_t
rrm_is_qci_throughput_exceeds_threshold
(
  rrm_ue_throughput_reports_t *p_ue_throughput_reports,
  rrm_cell_index_t  cell_index,
  U8                qci ,
  U8                lc_id
);

U8
get_lcid_for_qci_of_pdcp_stats
(
    rrm_ue_context_t *p_ue_context,
    U8 qci ,
    U8 pos_in_stats
);

rrm_void_t
rrm_build_list_of_selected_erabs
(
    rrm_ue_context_t *p_ue_context,
    endc_eligible_erab_list_t *p_erb_change_list,
    U8               *p_erb_change_list_count
);

rrm_bool_t
rrm_is_ue_channel_quality_is_worse_than_threshold
(
  rrm_ue_channel_quality_reports_t *p_ue_channel_quality_reports,
  rrm_cell_index_t cell_index
);

rrm_return_et 
rrm_build_and_send_meas_config_req_for_endc
(
 rrm_ue_context_t*            p_ue_context, 
 rrmc_meas_report_event_et    meas_rep_event
);
/****************************************************************************
 * Function Name  : rrm_get_data_transfer_mode_configd
 * Inputs         : Cell context, qci
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 * Description    : This function get configured data transfer mode
 *
 ****************************************************************************/
U8
rrm_get_data_transfer_mode_configd(U8                qci,
                                   rrm_cell_index_t  cell_index);
rrm_drb_type_et
rrm_get_endc_drb_type_on_qci
(
  U8 qci ,
  rrm_cell_index_t cell_index
);

rrm_bool_t
rrm_check_trigger_to_configure_nr_database
(
  rrm_ue_context_t           *p_ue_context,
  rrm_nr_meas_trigger_type_et trigger_type,
  U8                          qci
);

rrm_bool_t
rrm_is_endc_supported_on_qci
(
  U8 qci ,
  rrm_cell_index_t cell_index
);

rrm_return_et
rrm_ue_handle_meas_results_ind_for_nr
(
    rrm_ue_global_context_t* p_g_rrm_ue_ctx
);

/****************************************************************************
 * Function Name  : rrm_ue_get_new_drb_id_for_bearer_change
 * Inputs         : rrm_ue_context_t, rrm_ue_erab_setup_item_t
 * Outputs        : None
 * Returns        : Success/Failure
 * Description    : Assign new drb_id for existing erab
 ***************************************************************************/
rrm_return_et
rrm_ue_get_new_drb_id_for_bearer_change(
        rrm_ue_context_t           *p_ue_context,
        rrm_ue_erab_setup_item_t   *p_erab_item
);

rrm_return_et
	rrm_ue_handle_dc_bearer_change_cnf
(
 rrm_ue_global_context_t *p_g_rrm_ue_ctx
);
#endif
/* OPTION3X Changes End */
/*! \fn	rrm_return_et rrm_ue_handle_erb_setup_cnf (rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE erab setup conf. 
 */
rrm_return_et
rrm_ue_handle_erb_setup_cnf (
							   rrm_ue_global_context_t *
							);

/*! \fn	rrm_return_et rrm_ue_handle_erb_modify_req (rrm_ue_global_context_t *)
 *  \brief This function will handle the UE erab modify request
 */
rrm_return_et
rrm_ue_handle_erb_modify_req (
								rrm_ue_global_context_t *
							 );

/*! \fn	rrm_return_et rrm_ue_handle_erb_modify_cnf (rrm_ue_global_context_t *)
 *  \brief This function will handle the UE ERAB modify conf. 
 */
rrm_return_et
rrm_ue_handle_erb_modify_cnf (
								rrm_ue_global_context_t *
						     );

/*! \fn rrm_return_et rrm_ue_update_erb_failed_success_list(const rrm_ue_erab_to_be_released_item_list_t *p_erab_to_release,rrm_ue_context_t *p_ue_context)
 *  \brief      This function will update the erb release count.
 *  \param[in]  p_erab_to_release
 *  \param[out] p_ue_context
*/
rrm_return_et
rrm_ue_update_erb_failed_success_list(
				 const rrm_ue_erab_to_be_released_item_list_t
									*p_erab_to_release,
				 rrm_ue_context_t			*p_ue_context
				);

/*! \fn	rrm_return_et rrm_ue_handle_erb_release_req (rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE erab release request
 */
rrm_return_et
rrm_ue_handle_erb_release_req (
								rrm_ue_global_context_t *
							  );

/*! \fn	rrm_return_et rrm_ue_handle_erb_release_cnf (rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE ERAB release conf
 */
rrm_return_et
rrm_ue_handle_erb_release_cnf (
								rrm_ue_global_context_t *
							  );

/*! \fn	rrm_return_et rrm_ue_handle_ue_release_req (rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE admission release request
 */
rrm_return_et
rrm_ue_handle_ue_release_req (
								rrm_ue_global_context_t *
							);

/*! \fn	rrm_return_et rrm_ue_handle_meas_config_resp (rrm_ue_global_context_t *)
 *  \brief		This function will handle UE measurement config response
*/


rrm_return_et
rrm_ue_handle_meas_config_resp (
								 rrm_ue_global_context_t *
							   );

/*! \fn	rrm_return_et  rrm_ue_handle_meas_results_ind (rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE measurement results indication
 */
rrm_return_et
rrm_ue_handle_meas_results_ind (
								 rrm_ue_global_context_t *
								);

/*SPR 22151 Fix Start*/
/*SPR 18654 Code deleted(Declaration moved)*/
/*SPR 22151 Fix End*/


/*! \fn	rrm_return_et rrm_ue_handle_ue_capability_enquiry_resp (rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE capability enquiry response
 */
rrm_return_et
rrm_ue_handle_ue_capability_enquiry_resp (
											rrm_ue_global_context_t *
										 );

/*! \fn	rrm_return_et rrm_ue_handle_ue_ho_adm_req (rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE HO admission request
 */
rrm_return_et
rrm_ue_handle_ue_ho_adm_req (
								rrm_ue_global_context_t *
							);

/*! \fn 		rrm_return_et rrm_ue_handle_ue_ho_adm_cnf (rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE HO admission confirmation
 */
rrm_return_et
rrm_ue_handle_ue_ho_adm_cnf (
								rrm_ue_global_context_t *
						    );

/*! \fn 		rrm_return_et rrm_ue_handle_ue_ho_cmd_req (rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE HO command request
 */
rrm_return_et
rrm_ue_handle_ue_ho_cmd_req (
								rrm_ue_global_context_t *
						    );

/*! \fn 		rrm_return_et rrm_ue_handle_ue_ho_failure (rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE HO failure request
 */
rrm_return_et
rrm_ue_handle_ue_ho_failure (
								rrm_ue_global_context_t *
							);

/*! \fn 		rrm_return_et rrm_ue_handle_ue_ho_cancel_resp (rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE HO cancle response
 */
rrm_return_et
rrm_ue_handle_ue_ho_cancel_resp (
									rrm_ue_global_context_t *
								);
/*! \fn 		rrm_return_et rrm_ue_handle_ue_capability_ind(rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE capability indication
 */
rrm_return_et
rrm_ue_handle_ue_capability_ind (
									rrm_ue_global_context_t *
								);

/*! \fn 		rrm_return_et  rrm_ue_handle_ue_erab_setup_req(rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE ERAB setup request
 */
rrm_return_et
rrm_ue_handle_ue_erab_setup_req (
									rrm_ue_global_context_t *
								);

/*! \fn 		rrm_return_et rrm_ue_handle_log_and_ignore_event (rrm_ue_global_context_t *)
 *  \brief		This function will handle error event handling
 */
rrm_return_et
rrm_ue_handle_log_and_ignore_event (
										rrm_ue_global_context_t *
									);

/*! \fn 		rrm_return_et rrm_ue_handle_snr_response(rrm_ue_global_context_t *p_rrm_glb_ctxt)
 *  \brief		This function will handle the UE SNR response
 *  \param 	        p_rrm_glb_ctxt
 */
rrm_return_et
rrm_ue_handle_snr_response(
                            rrm_ue_global_context_t *p_rrm_glb_ctxt
                          );

/*! \fn         rrm_return_et rrm_ue_handle_context_mod_req(rrm_ue_global_context_t *p_rrm_glb_ctxt)
 *  \brief      This function will handle the UE Context Mod Request
 *  \param      p_rrm_glb_ctxt  
             
*/
rrm_return_et
rrm_ue_handle_context_mod_req(
                               rrm_ue_global_context_t *p_rrm_glb_ctxt
                             );

/*SPR 18241 START*/
/*! \fn         rrm_return_et rrm_ue_queue_ue_ctxt_msg(rrm_ue_global_context_t *p_rrm_glb_ctxt)
 *  \brief      This function will handle the UE Context Mod Request
 *  \param      p_rrm_glb_ctxt  
             
*/
rrm_return_et
rrm_ue_queue_ue_ctxt_msg(
                               rrm_ue_global_context_t *p_rrm_glb_ctxt
                             );
/*SPR 18241 END*/

/*SPR 18241 14JAN START*/
/*! \fn         rrm_ue_handle_ue_context_mod_in_ho_ongoing_state(rrm_ue_global_context_t *p_rrm_glb_ctxt)
 *  \brief      This function will handle the UE Context Mod Request
 *  \param      p_rrm_glb_ctxt  
             
*/
rrm_return_et
rrm_ue_handle_ue_context_mod_in_ho_ongoing_state(
                rrm_ue_global_context_t *p_rrm_glb_ctxt
                        );
/*SPR 18241 14JAN END*/




/*! \fn         rrm_return_et rrm_ue_handle_context_mod_cnf(rrm_ue_global_context_t *p_rrm_glb_ctxt)
 *  \brief      This function will handle the UE Context Mod Request
 *  \param      p_rrm_glb_ctxt
 */
rrm_return_et
rrm_ue_handle_context_mod_cnf(
                               rrm_ue_global_context_t *p_rrm_glb_ctxt
                             );

/*! \fn 		rrm_return_et rrm_ue_wait_for_cap_ind_handle_erb_setup_req (rrm_ue_global_context_t *p_rrm_glb_ctxt)
 *  \brief		This function will handle  capability indication in wait for UE state.
 *  \param              p_rrm_glb_ctxt
*/ 	
rrm_return_et
rrm_ue_wait_for_cap_ind_handle_erb_setup_req (rrm_ue_global_context_t *p_rrm_glb_ctxt);

/*! \fn  rrm_return_et rrm_ue_wait_for_cap_ind_handle_ue_release_req(rrm_ue_global_context_t *p_rrm_glb_ctxt)
 *  \brief this function handles UE release request in wait for capability indication
 *  \param[in]	p_rrm_glb_ctxt pointer to the global context.
 */
rrm_return_et
rrm_ue_wait_for_cap_ind_handle_ue_release_req
(
    rrm_ue_global_context_t *p_rrm_glb_ctxt
);

/*! \fn         rrm_ue_map_drb_id( rrm_ue_context_t *p_ue_context, rrm_ue_erab_setup_item_list_t *p_setup_item_list)	
 *  \brief	this function will map the drb id.
 *  \param 	p_ue_context Pointer to UE context
 *  \param	p_setup_item_list Pointer to UE erab setup item list
 */
void
rrm_ue_map_drb_id(
					rrm_ue_context_t                    *p_ue_context,
					rrm_ue_erab_setup_item_list_t   	*p_setup_item_list
				);
/*! \fn         rrm_ue_map_lc_id( rrm_ue_context_t *p_ue_context, rrm_ue_erab_setup_item_list_t  *p_setup_item_list)	
 *  \brief      this function will map the lc id.
 *  \param 	p_ue_context
 *  \param	p_setup_item_list 
 */
void
rrm_ue_map_lc_id(
                    rrm_ue_context_t                                *p_ue_context,
                    rrm_ue_erab_setup_item_list_t   *p_setup_item_list
                );

/* UE MEAS CHANGES: STARTS */

/*! \fn         rrm_return_et rrm_ue_handle_ho_restriction_ind(rrm_ue_global_context_t *p_rrm_glb_ctxt)
 *  \brief      This function will handle the HO Restriction list
 *  \param      p_rrm_glb_ctxt
 */
rrm_return_et
rrm_ue_handle_ho_restriction_ind(
                               rrm_ue_global_context_t *p_rrm_glb_ctxt
                             );

/*! \fn         rrm_return_et rrm_ue_handle_ue_timer_expiry(rrm_ue_global_context_t *p_rrm_glb_ctxt)
 *  \brief      This function will handle the ue timer expiry 
 *  \param    p_rrm_glb_ctxt  
*/
rrm_return_et
rrm_ue_handle_ue_timer_expiry (rrm_ue_global_context_t *p_rrm_glb_ctxt);
rrm_return_et
rrm_uem_send_meas_config_resp(
        rrm_ue_context_t    *p_ue_context,
        rrm_return_et           response,
        U16                     error_code
        );

/* UE MEAS CHANGES: ENDS */

/*! \fn 	    void rrm_ue_print_drb_configured( rrm_ue_context_t *p_ue_context)	
 *  \brief		this function will print the drb configured params.
 *  \param 	 p_ue_context
 */
void
rrm_ue_print_drb_configured(
        rrm_ue_context_t *p_ue_context
        );

/*! \fn            rrm_return_et rrm_ue_check_config_gbr_drbs(rrm_ue_context_t *p_ue_context, U8 gbr_configd_count, U8 gbr_success_count)
 *  \brief         This function checks number of gbr drbs configured and
 *                  returns failure if already maximum number of gbr drbs are
 *                  configured 
 *  \param   p_ue_context   
 *  \param   gbr_configd_count
 *  \param   gbr_success_count
 */
rrm_return_et
rrm_ue_check_config_gbr_drbs(
	rrm_ue_context_t *p_ue_context,
	U8               gbr_configd_count,
	U8               gbr_success_count
	);

/*! \fn            rrm_return_et rrm_ue_check_config_ngbr_drbs(rrm_ue_context_t *p_ue_context, U8 ngbr_configd_count, U8  ngbr_success_count)
 *  \brief         This function checks number of ngbr drbs configured and
 *                  returns failure if already maximum number of ngbr drbs are
 *                  configured
 *  \param   p_ue_context
 *  \param   ngbr_configd_count
 *  \param   ngbr_success_count
 */
rrm_return_et
rrm_ue_check_config_ngbr_drbs(
        rrm_ue_context_t *p_ue_context,
        U8               ngbr_configd_count,
        U8               ngbr_success_count
        );

/* BLR Team HO status report to SON Changes Start */
rrm_void_t
rrm_ue_populate_rrmif_handover_report(
         rrm_ue_context_t              *p_rrm_ue_context,
         rrm_target_id_t               *p_target_id,
         /* MLB Changes start */
    /* SPR 17108 FIX START*/
         phy_cell_id_info_t            *p_phy_cell_id_info
    /* SPR 17108 FIX END*/
         /* MLB Changes end */
         );
/****************************************************************************
 * Function Name  : rrm_ue_populate_and_send_rrmif_meas_results_ind
 * Inputs         : p_ue_context - Pointer to the UE context for which meas
 *                  results are being populated
 * Outputs        : None
 *
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Populates the meas_results_ind message to be sent to RMIF
 ****************************************************************************/
/*! \fn rrm_void_t rrm_ue_populate_and_send_rrmif_meas_results_ind(rrm_ue_context_t *p_ue_context)
 *  \brief This function is used to populate and send rrmif measurement results indication
 *  \param p_ue_context Pointer to UE context
 */
/* SPR 16406 Start */
rrm_void_t
rrm_ue_populate_and_send_rrmif_meas_results_ind(
        rrm_ue_context_t                    *p_ue_context,
        rrm_son_meas_results_ind_t          *p_meas_result
        );
/* SPR 16406 End */
/* BLR Team HO status report to SON Changes End */

/*csg start*/
/*PROXIMITY IND START*/

/*! \fn         rrm_return_et rrm_ue_handle_proximity_ind_event(rrm_ue_global_context_t *p_rrm_glb_ctxt)
 *  \brief      This function will handle the proximity ind
 *  \param[in]  p_rrm_glb_ctxt Pointer to glb context
 */
rrm_return_et
rrm_ue_handle_proximity_ind_event(
        rrm_ue_global_context_t *p_rrm_glb_ctxt
        );
/*PROXIMITY IND END*/
/*csg end*/

/*LTE_RRM_TM_SWITCH_START*/
/*! \fn          rrm_return_et rrm_ue_handle_reconfig_resp_event(rrm_ue_global_context_t *)
 *  \brief       This function handle the ue reconfig resp from rrc
 *  \param[in]   p_rrm_glb_ctxt
 */
rrm_return_et
rrm_ue_handle_reconfig_resp_event(
        rrm_ue_global_context_t *p_rrm_glb_ctxt
        );

/*! \fn          rrm_return_et rrm_ue_handle_mac_periodic_report_event(rrm_ue_global_context_t *)
 *  \brief       This function handle the ue reconfig resp from rrc
 *  \param[in]   p_rrm_glb_ctxt
 */
rrm_return_et
rrm_ue_handle_mac_periodic_report_event(
                                 rrm_ue_global_context_t *p_rrm_glb_ctxt
                                 );

/*! \fn          rrm_return_et rrm_ue_handle_cdma2000_mob_info_req(rrm_ue_global_context_t *
 *  \brief       This function handle the cdma2000 mob req from rrc
 *  \param[in]   p_rrm_glb_ctxt
 */
rrm_return_et
rrm_ue_handle_cdma2000_mob_info_req(
        rrm_ue_global_context_t *p_rrm_glb_ctxt
        );
/*! \fn          rrm_return_et rrm_ue_handle_reconfig_resp_for_tm_switch_event(rrm_ue_context_t *p_ue_context)
 *  \brief       This function process the ue reconfig resp from tm mode switching from rrc
 *  \param       p_ue_context
 */
rrm_return_et
rrm_ue_handle_reconfig_resp_for_tm_switch_event(rrm_ue_context_t *p_ue_context);

/*LTE_RRM_TM_SWITCH_END*/

/*csg start*/
/*! \fn rrm_adjust_csg_nsg_counter(rrm_csg_membership_status_et old_membership_status,
rrm_ue_context_t  *p_ue_context)
 *  \brief This function adjusts csg nsg counter
 *  \param old_membership_status old status
 *  \param p_ue_context ue context
*/
void
rrm_adjust_csg_nsg_counter(
        rrm_csg_membership_status_et old_membership_status,
        rrm_ue_context_t   *p_ue_context
        );

void
rrm_decrease_csg_non_csg_counter(
        rrm_ue_context_t *p_ue_context
        );
/*csg end*/

/* COMP_WARN_1_FEB:compilation warning removal */
/*! \fn rrm_return_et rrm_handle_erb_setup_modify_procedure(rrm_ue_global_context_t *p_rrm_glb_ctxt, rrm_ue_erab_setup_item_list_t *p_erab_setup_item_success_list,rrm_ue_erab_failed_item_list_t *p_erab_setup_item_failed_list,rrm_erab_procedure_et rrm_erab_procedure)
 * \brief this function handles erb setup
 * \param p_rrm_glb_ctxt  rrm global context
 * \param p_erab_setup_item_success_list erab setup success list  
 * \param p_erab_setup_item_failed_list  setup failure list
 * \param rrm_erab_procedure erab procedure
*/
rrm_return_et
rrm_handle_erb_setup_modify_procedure
(
 rrm_ue_global_context_t *p_rrm_glb_ctxt,
 rrm_ue_erab_setup_item_list_t   *p_erab_setup_item_success_list,
 rrm_ue_erab_failed_item_list_t  *p_erab_setup_item_failed_list,
 rrm_erab_procedure_et            rrm_erab_procedure
 );



/* SPS related changes start */
/*! \fn  rrm_void_t rrm_find_sps_erab_highest_priority(rrm_ue_erab_setup_item_list_t  *p_erab_setup_item_list, rrm_ue_context_t *p_ue_context, rrm_cell_context_t *p_cell_context)
 *  \brief   This function finds the 'highest priority' for the RAB to be given SPS config
 *  \param   p_erab_setup_item_list
 *  \param   p_ue_context
 *  \param   p_cell_context
 */

rrm_void_t
rrm_find_sps_erab_highest_priority (
        rrm_ue_erab_setup_item_list_t  *p_erab_setup_item_list,
        rrm_ue_context_t               *p_ue_context,
        rrm_cell_context_t             *p_cell_context
        );
/* SPS related changes end */
/* SRS_FREQ_HOPPIN_START */
/*! \fn rrm_void_t rrm_rollback_ue_srs_configuration(rrm_ue_context_t *p_ue_context)
 *  \brief This function is used for rollback UE SRS configuration
 *  \param p_ue_context Pointer to UE Context
 */
rrm_void_t
rrm_rollback_ue_srs_configuration
(
    rrm_ue_context_t *p_ue_context
);
/*! \fn rrm_return_et rrm_ue_handle_reconfig_resp_for_srs_hopping_status_changed_event(rrm_ue_context_t *p_ue_context)
 *  \brief This function is used for UE handle reconfig Response for SRS hopping status
 *  \param p_ue_context Pointer to UE Context
 */
rrm_return_et
rrm_ue_handle_reconfig_resp_for_srs_hopping_status_changed_event
(
    rrm_ue_context_t *p_ue_context
);

/* SRS_FREQ_HOPPIN_END */

/* HO Oscillation start */
rrm_return_et rrm_compare_global_cell_id(  
   cell_global_id_eutra_t *p_global_cell_id,
   rrm_global_cell_id_t *p_rrm_global_cell_id
);

/* HO Oscillation end */
rrm_return_et 
rrm_compare_global_cell_id
(
cell_global_id_eutra_t *p_global_cell_id,
rrm_global_cell_id_t *p_rrm_global_cell_id
);
rrm_cell_type_t
rrm_get_hghst_prorty_inter_rat_basd_on_spid
(
    rrm_ue_context_t  *p_ue_ctxt
);

rrm_ue_timer_type_et
rrm_get_ecid_meas_timer_type(
        U8                   esmlc_meas_id
        );
U32
rrm_get_meas_priodicity_in_ms(
        rrm_lppa_meas_peridicity_et          report_interval
        );

rrm_void_t
rrm_ue_process_meas_resp_for_ue_pos(
    rrm_ue_context_t         *p_rrm_ue_context,
    U8                        meas_id,
    rrm_meas_ecid_resp_et     ecid_meas_resp_action
    );

rrm_return_et
rrm_ue_store_ue_meas_results_in_ue_context(
        rrm_ue_measurment_results_ind_t  *p_ue_meas_results,
        rrm_ue_context_t                 *p_ue_context,
        U8                                ecid_meas_req_index
        );

rrm_return_et
rrm_build_and_send_mac_ecid_meas_report_req(
    rrm_ue_context_t            *p_ue_context,
    S32                         ue_started_meas_time_in_ms 
    );

rrm_return_et
rrm_send_mac_ecid_meas_report_req(
            RrmMacEcidMeasConfigReq  *p_rrm_mac_ecid_meas_report_req,
            U16                              src_module_id,
            U16                              transaction_id,
            U8                               cell_index
            );

/*DYNAMIC ICIC START*/
rrm_void_t
rrm_rollback_ue_dynamic_icic_data
( 
    rrm_ue_context_t *p_ue_context
 );

rrm_void_t
rrm_commit_ue_dynamic_icic_data
( 
    rrm_ue_context_t *p_rrm_ue_context
 );

/*DYNAMIC ICIC END*/

rrm_ue_response_et
rrm_map_ue_response_value
(
    rrm_return_et rrm_response
);

/* Start: Enh 317 */
rrm_void_t
rrm_ue_print_meas_config_struct(
    rrm_ue_meas_config_t meas_config);
/* End: Enh 317 */   

/* SPR 15674 Start */
rrm_return_et
is_x2_link_exist
    (
 phy_cell_id_info_t phy_cell_id_info
    );
/* SPR 15674 End */
    /*SPR-695 Fix Starts*/
    /*! \fn         rrm_return_et rrm_ue_handle_low_priority_ue_rel_event(rrm_ue_global_context_t *)
     *  \brief      This function will handle the UE capability indication
     */
    rrm_return_et
    rrm_ue_handle_low_priority_ue_rel_event(
            rrm_ue_global_context_t *
            );
    /*SPR-695 Fix Ends*/

/*SPR_7831_CHANGES_START*/
meas_type_et
rrm_check_meas_id_validity
(
 rrm_ue_context_t                  *p_ue_context,
 rrm_ue_measurment_results_ind_t     *p_proc_data
);
/*SPR_7831_CHANGES_END*/

/* CA: SCell Selection start */
rrm_return_et
rrm_check_for_ca_need_thru_erab_setup_or_ho_cnf(
                rrm_ue_context_t                 *p_ue_context
                );

rrm_bool_et
rrm_compare_ue_csg_with_cell_csg_status(
                rrm_ue_context_t             *p_ue_context,
                rrm_cell_context_t           *p_scell_ctxt,
                access_mgmt_params_t         *p_cell_access_mgmt_params
                );

rrm_void_t
rrm_check_ue_compatibilty_with_scell(
                rrm_ue_context_t             *p_ue_context,
                rrm_cell_context_t           *p_cell_context,
                rrm_cell_index_t             *scell_list,        /* SCell candidates received from CellM */
                rrm_cell_index_t             *scell_index_list   /* Final SCell list of SCells compatible with UE */
                );

U8
rrm_count_scells_in_final_list(
                rrm_cell_index_t             *final_scell_list
                );

rrm_void_t
rrm_store_final_scell_list_in_ue_context(
                rrm_ue_context_t             *p_ue_context,
                rrm_cell_index_t             *final_scell_list,
                U8                            count
                );

rrm_return_et
rrm_enable_ca_for_the_ue(
                rrm_ue_context_t             *p_ue_context,
                rrm_cell_context_t           *p_cell_context
                );

rrm_bool_et
rrm_check_if_ue_is_ca_capable(
                rrm_ue_context_t             *p_ue_context,
                rrm_cell_context_t           *p_cell_context
                );

rrm_return_et
rrm_check_for_ca_preconditions(
                rrm_ue_context_t                *p_ue_context,
                rrm_cell_context_t              *p_cell_context
                );

rrm_void_t
rrm_get_release_and_category_of_ue(
                rrm_ue_context_t    *p_ue_context,
                U8                  *ue_category,
                U8                  *access_stratum_release
                );


/* SPR 13383 changes start */

/*! \fn rrm_void_t rrm_function_check_band_combination_set_support(rrm_ue_context_t*, U8,U8 )
 *  \brief This function find the selected band entry in ue capability
 *  \param p_ue_context pointer to Ue context 
 *  \param index 
 *  \param bandwidthcombination_set
 */
S8
rrm_function_check_band_combination_set_support( rrm_ue_context_t    *p_ue_context , 
                                                 U8                   index , 
                                                 U32                   bandwidthcombination_set );



/*! \fn rrm_void_t rrm_function_match_band_combination_in_ue_capability(
 *                                                                      rrm_ue_context_t*, U8,U8,U8 )
 *  \brief This function find the selected band entry in ue capability
 *  \param p_ue_context pointer to Ue context 
 *  \param carrier_1_band
 *  \param carrier_2_band
 *  \param band_comb_set
 */
S8
rrm_function_match_band_combination_in_ue_capability( 
                                                      rrm_ue_context_t   *p_ue_context,
                                                      U8                  carrier_1_band,
                                                      U8                  carrier_2_band,
                                                      U8                  carrier_1_bw_class,
                                                      U8                  carrier_2_bw_class,
                                                      U32                 band_comb_set,
                                                      rrm_bool_et         contigious_bandwidth );
/* SPR 13383 changes end */
/* CA: SCell Selection end */

/* CA Changes : Start */
rrm_return_et
rrm_rollback_ue_scell_info_add_fail(rrm_ue_context_t *p_ue_context);

rrm_return_et
rrm_reset_ue_scell_info_del_fail(rrm_ue_context_t *p_ue_context);

rrm_return_et
rrm_scell_add_success(rrm_ue_context_t *p_ue_context);

rrm_return_et
rrm_scell_del_success(rrm_ue_context_t *p_ue_context);

void
reset_scell_add_bkp(rrm_ue_context_t *p_ue_context);
/* CA Changes : End */
/* SPR 9216:MRO enhancement start */
rrm_return_et
rrm_ue_handle_ue_reestab_config_cnf_in_ho(rrm_ue_global_context_t *p_g_rrm_ue_ctx);
rrm_return_et
rrm_ue_handle_ue_reestab_config_cnf(rrm_ue_global_context_t *p_g_rrm_ue_ctx);

/* SPR 9216:MRO enhancement end */
/*SPR 11535:start*/
rrm_return_et rrm_ue_handle_rrc_ho_ongoing_ue_inactivity(rrm_ue_global_context_t *p_g_rrm_ue_ctx);
/*SPR 11535:end*/
/* SPR 10450 Fix Start */
rrm_void_t
rrm_ue_rac_clear_drx_res
( 
    rrm_cell_index_t cell_index,
    U32              applied_drx_profile_index,
    U8               applied_drx_offset_val
);

rrm_void_t
rrm_ue_rac_apply_drx_res
( 
    rrm_cell_index_t cell_index,
    U32              applied_drx_profile_index,
    U8               applied_drx_offset_val
);
rrm_void_t
rrm_handle_ue_reconfig_response_for_drx
(
  rrm_ue_context_t *p_ue_context,
  U8                response 
);
/* SPR 10450 Fix End */

/* SPR 16373 FIX START */
void
rrm_check_and_dec_num_anr_strong_cell_req_count
(
 rrm_ue_context_t              *p_rrm_ue_ctx
 );
/* SPR 16373 FIX END */

/* SPR 13445 Fix Start */
rrm_return_et 
rrm_try_daho_on_csg_timer_expiry( 
   rrm_ue_context_t *p_ue_cntxt 
);

void
rrm_get_rat_priority_from_spid(
        rrm_ue_context_t *p_ue_ctxt,
        rrm_bool_et *is_eutran_spprted_by_spid,
        rrm_bool_et *is_utran_spprted_by_spid,
        rrm_bool_et *is_geran_supported_by_spid,
        rrm_bool_et *is_cdma2000_supported_by_spid,
        rrm_bool_et *spid_found
        );
/* SPR 13445 Fix End */
/* SPR 12512 Fix Start */
rrm_return_et
rrm_ue_handle_ue_sync_status_ind_event
(
   rrm_ue_global_context_t *p_rrm_glb_ctxt
   );

/* SPR_16053_Fix: Start */
rrm_return_et
rrm_ue_handle_ue_reestab_config_cnf(
        rrm_ue_global_context_t *p_rrm_glb_ctxt
        );
/* SPR_16053_Fix: End */

rrm_return_et
rrm_build_and_send_ue_reconfig_req_on_ue_insync
(
 rrm_ue_context_t *p_rrm_ue_context
 );
/* SPR 15232 start */
/* SPR 15674 Start */
rrm_return_et 
rrm_search_and_remove_phy(
                rrm_ue_context_t *p_ue_context,
                U16 phy_cell_id,
                U16 earfcn
                );
/* SPR 15674 End */
/* SPR 15232 end */
 /* eICIC_PHASE_1_2_CHANGES_START */
rrm_return_et
rrm_uem_send_remove_meas_config_for_victim
(
	rrm_ue_context_t *p_ue_context
);
 /* eICIC_PHASE_1_2_CHANGES_END */
    /* SPR 12512 Fix End */
    /*SPR 17061 Start*/
/* SPR 15397 Start */

/*! \fn rrm_void_t rrm_release_anr_meas_configuration (rrm_ue_context_t *p_ue_context)
 *  \brief This function release the ANR object configuration and Measurement Gap configuration for the concerned UE.
 *  \param p_ue_context Pointer to UE Context
 *  \retunr None
 */
rrm_void_t
rrm_release_anr_meas_configuration
(
  rrm_ue_context_t *p_ue_context,
  /*SPR 15724:Start */
  U8 meas_id
  /*SPR 15724:End*/

);

int compare_by_priority(const void *a,const void *b);
rrm_bool_et
rrm_uem_check_req_status_of_anr_meas_objects(
    rrm_ue_context_t                  *p_ue_context
    );

/* SPR 15397 End */
    /*SPR 17061 End*/
/* SPR 15860 Start */
/* SPR 15861 Start */
/* SPR 15862 Start */
rrm_bool_et 
rrm_ue_is_cgi_report_pending(rrm_ue_context_t* p_ue_context);
/* SPR 15860 End */
/* SPR 15861 End */
/* SPR 15862 End */
/* SPR 16406 3Aug Start */
rrm_void_t
    rrm_uem_send_cdrx_removal
(
 rrm_ue_context_t *p_ue_context
);
    /* SPR 16406 3Aug End */

/* SPR 16331 Fix start */
rrm_void_t
rrm_ue_clean_prb_usage_proc_data_at_cnf_failure
(
 rrm_ue_context_t                *p_ue_context,
 rrm_ue_proc_erab_cnf_data_t     *p_rrm_ue_proc_erab_cnf_data,
 rrm_ue_erab_setup_item_list_t   *p_rrm_ue_erab_success_list,
/*SPR 19819 START*/
 rrm_erab_procedure_et            rrm_erab_procedure
/*SPR 19819 STOP*/
 );
/* SPR 16331 Fix end */

/* SPR 16053 start */
rrm_bool_et rrm_is_anr_configured_for_inter_object(
        rrm_ue_context_t *p_ue_context);
/* SPR 16053 end */
    /* SPR 16406 18sep Start */
rrm_void_t
    rrm_reset_cell_selection_and_cgi_pending_list
(
 rrm_ue_context_t *p_ue_context
 );
    /* SPR 16406 18sep End */
/*spr 17437 fix start*/
rrm_void_t
rrm_reset_rab_removal_container
(
  rrm_rab_removal_container_t  *p_rab_remove_ctnr
);

rrm_void_t
rrm_reset_rab_removal_ctxt
(
  rrm_rab_removal_ctxt_t          *p_local_rab_removal_ctxt
);
rrm_void_t
rrm_fill_rab_removal_info_in_ue_ctxt
(
  rrm_ue_context_t                *p_ue_context,
  rrm_rab_removal_ctxt_t          *p_local_rab_removal_ctxt
);
rrm_void_t
rrm_process_rab_removal_info_in_ue_ctxt
(
   rrm_ue_context_t                *p_ue_context
);
/*spr 17437 fix stop*/



/* SPR 17837 FIX START */
meas_report_info_node_t   
*rrm_get_meas_node_for_meas_id
(
    U8                        meas_id,
    rrm_ue_context_t          *p_ue_context
);
/* SPR 17837 FIX END */

/* SPR 17850 Fix Start */
rrm_return_et
rrm_ue_handle_ue_capability_ind_ho_ongoing
(
    rrm_ue_global_context_t *p_rrm_glb_ctxt
);

rrm_return_et
rrm_ue_setting_capability_params
(
    rrm_ue_context_t    *p_ue_context
);
/* SPR 17850 Fix End */
/* SPR 17784 Start */
rrm_return_et rrm_ue_handle_ue_cgi_timer_expiry
(
rrm_ue_global_context_t *p_rrm_glb_ctxt
);
/* SPR 17784 End */
/*SPR 19968 Start*/
rrm_void_t 
rrm_ue_process_erb_release_list_in_erb_setup_cnf(rrm_ue_context_t *p_ue_context,rrm_ue_proc_erab_cnf_data_t *p_rrm_ue_proc_erab_cnf_data);
/*SPR 19968 End*/


/*SPR_17893_START*/
rrm_void_t
rrm_set_utran_capability_enq_info(
rrm_ue_context_t *p_ue_context,
rrm_bool_et is_req_sent, rrm_bool_et is_resp_rcvd);
rrm_bool_et
rrm_check_utran_capability_enq_info(
rrm_ue_context_t *p_ue_context,
rrm_bool_et is_req_sent, rrm_bool_et is_resp_rcvd);
rrm_void_t 
rrm_check_utran_cell_selection_lists(rrm_ue_context_t *p_ue_context);
rrm_return_et
rrm_process_ue_for_utran_cell_selection_list(rrm_ue_context_t *p_ue_context);
/*! \fn	rrm_return_et rrm_ue_handle_ue_capability_enquiry_resp_in_ho_ongoing(rrm_ue_global_context_t *)
 *  \brief		This function will handle the UE capability enquiry response
 *              in ho onging state.
 */
rrm_return_et
rrm_ue_handle_ue_capability_enquiry_resp_in_ho_ongoing (rrm_ue_global_context_t *
										 );
/*SPR 20523 START*/
S8
rrm_ue_convert_actual_qoffset_value
(
    rrm_rat_type_et         rat_type,
    U8                      offset_freq
);
/*SPR 20523 END*/
/*SPR_17893_END*/
/*SPR 20523 START*/
S8
rrm_ue_convert_actual_qoffset_value
(
    rrm_rat_type_et         rat_type,
    U8                      offset_freq
);
/*SPR 20523 END*/

#endif
