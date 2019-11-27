/*! \file rrm_ue_ho_utils.h
 *  \brief This file contains the prototypes for UE FSM.
 *  \date January 30, 2012
 *  \author gur30239
 */
/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _RRM_UE_HO_UTILS_H_
#define _RRM_UE_HO_UTILS_H_
/* SPR 15336 Fix Start */
/*! \fn U16 rrm_validate_and_get_geran_daho_pci(rrm_ue_context_t *p_ue_ctxt)
 *  \brief This function is used to validate and get geran DAHO PCI information
 *  \param p_ue_ctxt UE CONTEXT
 */
/* SPR 15674 Start */
phy_cell_id_info_t
/* SPR 15674 End */
rrm_validate_and_get_geran_daho_pci(
  rrm_ue_context_t *p_ue_ctxt,
  U8               *dtm_ho_supported
    );
/* SPR 15336 Fix End */

/* SPR 15674 Start */
phy_cell_id_info_t
/* SPR 15674 End */
rrm_validate_and_get_utran_daho_pci(
  rrm_ue_context_t *p_ue_ctxt
  );

/* SPR 15674 Start */
phy_cell_id_info_t
/* SPR 15674 End */
rrm_validate_and_get_eutran_daho_pci(
  rrm_ue_context_t *p_ue_ctxt
  );

/* SPR 15674 Start */
phy_cell_id_info_t
/* SPR 15674 End */
rrm_get_valid_daho_cell_id(
        rrm_ue_context_t *p_ue_context
        );
/* ho restriction : code chanes start */
rrm_bool_et
rrm_is_inter_rat_cell_restricted (
/* SPR 17564 Fix Start */
    rrm_ue_context_t                *p_ue_context,
/* SPR 17564 Fix End */
        plmn_identity_t                 iner_rat_plmn_id,
        U8                              lac[],
        rrm_cell_type_t                 cell_type,
        rrm_ho_restriction_list_t       *p_ho_restriction_list
        );

rrm_bool_et
rrm_is_eutran_cell_restricted (
/* SPR 17564 Fix Start */
    rrm_ue_context_t                *p_ue_context,
/* SPR 17564 Fix End */
        rrm_oam_cell_plmn_info_t        euran_plmn_id,
        U8                              tac[],
        rrm_ho_restriction_list_t       *p_ho_restriction_list
        );

rrm_bool_et
rrm_is_cell_pres_in_ho_res_lac (
        U8                               plmn_lac[],
        rrm_plmn_identity_t              plmn_id,
        rrm_ho_restriction_list_t       *p_ho_restriction_list
        );

rrm_bool_et
rrm_is_cell_pres_in_ho_res_tac (
        U8                               plmn_tac[],
        rrm_plmn_identity_t              plmn_id,
        rrm_ho_restriction_list_t       *p_ho_restriction_list
        );

rrm_bool_et
rrm_is_cell_pres_in_serv_eqv_list (
     /*SPR 17777 +-*/
        rrm_ho_restriction_list_t        *p_ho_restriction_list
        );

rrm_bool_et
rrm_is_daho_cell_restricted(
        rrm_ue_context_t *p_ue_context
        );
/* ho restriction : code chanes end */

/* SPR_14564_fix: start */
/* CSR_97730_fix: start */
/* SPR 16406 Start */
rrm_return_et
rrm_config_ue_for_cgi_reporting_for_new_detected_pci(
         rrm_ue_context_t            *p_ue_context,
        rrm_ue_rat_et               rrm_cell_rat_type,
        rrm_cgi_trigger_cause_et    cgi_trigger_cause
 );
rrm_return_et 
rrm_uem_search_and_select_pci_for_ho 
(
    rrm_ue_context_t *p_ue_context,
    U16 transaction_id
);

cell_selection_priority_list_t* rrm_uem_get_active_cell_selection_list
(
 rrm_ue_context_t *p_ue_context
);

rrm_return_et
rrm_handle_pci_in_confused_cell_selection_list
(
  rrm_ue_context_t *p_ue_context,
  cell_selection_priority_list_t *p_confused_cell_list
);

rrm_return_et
rrm_handle_pci_in_unknown_cell_selection_list
(
  rrm_ue_context_t *p_ue_context
);
/* SPR 16406 End */
/* CSR_97730_fix: end */
/* SPR_14564_fix: end */
/* SPR 16406 8SEP start */
rrm_void_t
rrm_reset_eutran_ranking(
        rrm_ue_context_t *p_ue_context);

rrm_void_t
rrm_reset_inter_rat_ranking(
        rrm_ue_context_t *p_ue_context);
rrm_bool_et rrm_is_all_cgi_requied_cells_list_is_empty(
	rrm_ue_context_t *p_ue_context);
rrm_return_et
rrm_handle_cgi_measurement(
        rrm_ue_context_t *p_ue_context,
        rrm_ue_measurment_results_ind_t *p_rrm_parsed_meas_res_ind
        );
rrm_return_et
rrm_check_if_node_already_exits_for_ho(
        rrm_ue_context_t *p_ue_context,
        U16 carrier_earfcn,
        U16 pci);
/*SPR 17527 START*/
rrm_cgi_trigger_cause_et
rrm_check_cgi_of_utran_reported_cell(
        rrm_ue_context_t *p_ue_context,
        U16 ura_pci,
        U16 carrier_earfcn,
        phy_cell_id_info_t  *p_phy_cell_info,
        rrm_bool_et *p_cgi_present_in_unknwn_list_flag);
/*SPR 17527 END*/
rrm_return_et
rrm_remove_previous_cell_for_ho(rrm_ue_context_t *p_ue_context,
                        U16 pci,
                        U16 carrier_earfcn);
/* SPR 16406 8SEP end */

/* SPR_13721_fix : start */
rrm_return_et
rrm_uem_configure_geran_srvcc(
        rrm_ue_context_t            *p_ue_ctxt,
        rrm_srvcc_ue_criteria_et    ue_srvcc_criteria,
        rrm_bool_et                 dtm_ho_supported,
        rrm_bool_et                 dtm_supported,
        rrm_bool_et                 voip_capable
        );
/* SPR_13721_fix : end */


/*SPR_17893_START*/
rrm_bool_et rrm_check_if_utran_ho_is_possible(rrm_ue_context_t*   p_ue_context);
rrm_bool_et rrm_determine_if_utran_ho_is_forbidden_or_not_allowed(rrm_ue_context_t*   p_ue_context);
/*SPR_17893_END*/
#endif /*_RRM_UE_HO_UTILS_H_*/
