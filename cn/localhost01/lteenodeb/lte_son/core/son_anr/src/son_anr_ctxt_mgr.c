/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_anr_ctxt_mgr.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains the functions for managing ANR Module's
 *        Global context and cell specific context.
 *
 *******************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *  Feb, 2012     Komal/Shekhar                  Initial
 *  May, 2012     Komal/Shekhar                  Release 1.1 Changes
 *  Aug, 2012     Komal/Shekhar                  GERAN support added
 *  June,2013     Rashi/Swetank                  CDMA2000 support added
 *  Aug, 2014     Shilpi                         SPR 13251 Fix
 ******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_utils.h>
#include <son_anr_ctxt_mgr.h>
#include <son_anr_table.h>
#include <son_defines.h>
#include <son_anr_enb_ctxt_mgr.h>
#include <son_anr_table.h>

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern const son_8 *p_son_anr_facility_name;
extern son_global_config_params_t g_config_params;

/*****************************************************************************
 *  Name  : ANR Global Context List.
 ****************************************************************************/
anr_global_t g_anr_context;

/*****************************************************************************
 *  Name  : Global Pending Response.
 ****************************************************************************/

pending_res_t g_anr_pending_response;

/* SONANR FSM state names */
const son_s8 *SON_ANR_FSM_STATES_NAMES[] =
{
	(const son_s8 *)"SONANR_STATE_INIT",
	(const son_s8 *)"SONANR_STATE_ACTIVE",
	(const son_s8 *)"SONANR_STATE_ENABLING",
	(const son_s8 *)"SONANR_STATE_DISABLING",
	(const son_s8 *)"SONANR_STATE_SHUTDOWN",
	(const son_s8 *)"SONANR_STATE_UNDEFINED"
};

/* SONANR FSM state names */
const son_s8 *SON_ANR_CELL_FSM_STATES_NAMES[] =
{
	(const son_s8 *)"SONANR_CELL_STATE_ENABLING",
	(const son_s8 *)"SONANR_CELL_STATE_ENABLED",
	(const son_s8 *)"SONANR_CELL_STATE_DISABLING",
	(const son_s8 *)"SONANR_CELL_STATE_DISABLED",
	(const son_s8 *)"SONANR_CELL_STATE_UNDEFINED"
};

/* SONANR FSM Event names */
const son_s8 *SON_ANR_CELL_FSM_EVENT_NAMES[] =
{
	(const son_s8 *)"SONANR_ENABLE_REQ",
	(const son_s8 *)"SONANR_DISABLE_REQ",
	(const son_s8 *)"SONANR_NR_ADD_REQ",
	(const son_s8 *)"SONANR_NR_DELETE_REQ",
	(const son_s8 *)"SONANR_NR_UPDATE_REQ",
	(const son_s8 *)"SONANR_NR_RETRIEVE_REQ",
	(const son_s8 *)"RRM_SON_REGISTER_RESP",
	(const son_s8 *)"RRM_SON_DEREGISTER_RESP",
	(const son_s8 *)"RRM_SON_CELL_STATE_CHANGE_IND",
	(const son_s8 *)"SONANR_SON_MEAS_CONFIG_REQ",
	(const son_s8 *)"RRM_SON_MEAS_CONFIG_RESP",
	(const son_s8 *)"SONANR_MODIFY_UE_COUNT_REQ",
	(const son_s8 *)"SONANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ",
	(const son_s8 *)"RRM_SON_SET_ATTRIBUTE_VALUE_RESP",
	(const son_s8 *)"RRM_SON_MEAS_RESULTS_IND",
	(const son_s8 *)"SONANR_NR_DELETE_FROM_REMOVE_LIST_REQ",
	(const son_s8 *)"SONANR_MODIFY_REMOVAL_ATTRIBUTES_REQ",
	(const son_s8 *)"SONANR_MODIFY_PCI_CONFUSION_CFG_REQ",
	(const son_s8 *)"SONANR_ENABLE_DISABLE_LIMITED_MODE_REQ",
	(const son_s8 *)"RRM_SON_HO_REPORT",
	/*Coverity :41589:Starts*/
	(const son_s8 *)"RRM_SON_HO_ATTEMPT",
	(const son_s8 *)"RRM_SON_HO_FAILURE_REPORT",
	(const son_s8 *)"X2_HO_FAILURE_REPORT",
	(const son_s8 *)"SON_ANR_RRM_MEAS_CONFIG_OBJ_REMOVE_REQ",
	(const son_s8 *)"RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP",
    /* SPR 20653 Fix Start */
	(const son_s8 *)"SONANR_MODIFY_ATTRIBUTE_REQ",
    /* SPR 20653 Fix End */
	/*Coverity :41589:Stops*/
	(const son_s8 *)"NOT_AVAILABLE"
};

/*****************************************************************************
 * Function Name  : anr_get_ho_status_of_cdma_nr
 * Inputs         : p_inter_nri - Void pointer to xrtt/hrpd neighbour info
 * Outputs        : None
 * Returns        : son_ho_status_et
 * Description    : Returns the ho status of cdma NBR
 ****************************************************************************/

son_ho_status_et
anr_get_ho_status_of_cdma_nr
(
 son_void_t *p_inter_nr
 );

/*****************************************************************************
 * Function Name  : anr_print_pending_res
 * Inputs         : son_method_name - name of calling method
 * Outputs        : None
 * Returns        : son_void_t      - None
 * Description    : Print content of pending_response structure
 ****************************************************************************/
static son_void_t
anr_print_pending_res
(
 const son_8 *son_method_name
 )
{
	SON_LIST_NODE           *p_node             = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_DETAILED,
			"Pending Response Structure:"
			"API Id: %d "
			"Trans Id: %d "
			"Expected Response Count: %d"
            /*SPR 17777 +-*/ 
			"Error Received Flag: %d"
            "method name %s ",
			g_anr_pending_response.api_id,
			g_anr_pending_response.transaction_id,
			g_anr_pending_response.expected_res_count,
			g_anr_pending_response.is_error_received,
            son_method_name);
            /*SPR 17777 +-*/ 

	p_node = get_first_list_node(&g_anr_pending_response.cell_sts_list);

	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF( cell_status_list_data_t,
				cell_status_node,
				p_node);
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
				"Status for Cell [Cell Id: 0x%x] in Pending Response is error code = %d",
				son_convert_char_cell_id_to_int_cell_id(p_list_node->cell_status.cgi.cell_identity),
				p_list_node->cell_status.error_code);
		p_node = get_next_list_node(p_node);
	}

	SON_UT_TRACE_EXIT();
} /* anr_print_pending_res */

/*****************************************************************************
 * Function Name  : anr_print_global_context
 * Inputs         : son_method_name - name of calling method
 * Outputs        : None
 * Returns        : son_void_t      - None
 * Description    : This function prints the ANR global context
 ******************************************************************************/
static son_void_t
anr_print_global_context
(
 const son_8 *son_method_name
 )
{
	anr_cell_context_node_t *p_node = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_BRIEF,
			"\n===== ANR Global Context(%s):====\n"
			"Current FSM State: %s \n"
			"ANR Global Mode: %d \n"
			"ANR Log Enable: %d\n"
			"PCI Mode: %d\n"
			"No of Cell configured at ANR is %d\n"
			"ANR X2 Enable: %d\n"
			"ENB Type:%d\n"
			/* bug_13864_start */
			"CSG scheme:%d\n"
			"PICO scheme:%d\n"
            /*SPR 17777 +-*/ 
			"MACRO scheme:%d\n",
			/* bug_13864_end */
            son_method_name,
			SON_ANR_FSM_STATES_NAMES[g_anr_context.current_fsm_state],
			g_anr_context.anr_global_mode,
			g_anr_context.log_enable,
			g_anr_context.pci_mode,
			g_anr_context.cell_ctxt_list.count,
			g_anr_context.x2_enabled,
			/* Rel 3.0 Multi Sector code changes Start */
			g_anr_context.own_enb_id.enb_type,
			/* bug_13864_start */
			g_anr_context.csg_scheme,
			g_anr_context.pico_scheme,
			g_anr_context.macro_scheme);
	/* Rel 3.0 Multi Sector code changes Stop */

	p_node = anr_get_first_cell_ctxt();
	while (SON_PNULL != p_node)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
				"Cell [Cell Id: 0x%x] is in %s serving on pci %d and earfcn %d has %d "
				"Intra-Rat neighbors and %d Inter-Rat neighbors",
				son_convert_char_cell_id_to_int_cell_id(p_node->data.cell_id.cell_identity),
				SON_ANR_CELL_FSM_STATES_NAMES[p_node->data.current_cell_fsm_state],
				p_node->data.pci_value,
				anr_get_earfcn(&p_node->data),
				hash_count(&p_node->data.intra_rat_neighbor_table),
				hash_count(&p_node->data.inter_rat_neighbor_table));
		p_node = anr_get_next_node(p_node);
	}

	SON_UT_TRACE_EXIT();
} /* anr_print_global_context */

/*****************************************************************************
 * Function Name  : anr_print_eutran_nbr
 * Inputs         : p_eutran_rec - Pointer to eutran record
 * Outputs        : None
 * Returns        : son_void_t    - None
 * Description    : This function prints records in the utran record
 ******************************************************************************/
static son_void_t
anr_print_eutran_nbr
(
 anr_intra_rat_hash_rec_t *p_eutran_rec
 )
{
	son_intra_rat_neighbor_info_t *p_eutran_data = &p_eutran_rec->neighbor_info;


	SON_UT_TRACE_ENTER();
	SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
			SON_INFO,
			"\nNeighbor [Cell Id: 0x%x] has attributes:\n"
			"Bitmask: %d\n"
			"Pci: %d Earfcn: %u Rsrp: %d Rsrq: %d\n"
			"Cell specific offset: %d Tac: 0x%x%x csg_identity: 0x%x\n"
			"NR Status: %d HO Status: %d X2 Status: %d\n"
			"Suspected PCI:%d HOs Attempted:%d HOs Failed:%d HOs Successful:%d\n"
			"Last Activity TS: %ld No Neighbors TS: %ld HOs Failed (PCI Conflict) TS: %d\n"
			"num_too_early_ho: %u num_too_late_ho: %u num_ho_to_wrong_cell_for_failure_cell: %u\n"
			"num_ho_to_wrong_cell_for_reestablishment_cell: %u total_ho_attempted: %u\n",
			son_convert_char_cell_id_to_int_cell_id(p_eutran_data->nbr_cgi.cell_identity),
			p_eutran_data->bitmask,
			p_eutran_data->pci,
			p_eutran_data->cell_specific_offset,
			p_eutran_data->tac[SON_NULL],
			p_eutran_data->tac[SON_ONE],
			son_convert_char_cell_id_to_int_cell_id(p_eutran_data->csg_identity),
			p_eutran_data->earfcn,
			p_eutran_data->rsrp,
			p_eutran_data->rsrq,
			p_eutran_data->nr_status,
			p_eutran_data->ho_status,
			p_eutran_data->x2_status,
			p_eutran_data->suspected_pci,
			p_eutran_data->handover_stats.handovers_attempted,
			p_eutran_data->handover_stats.handovers_failed,
			p_eutran_data->handover_stats.handovers_succesful,
			((son_ul32)(p_eutran_rec->last_activity_ts)),
			((son_ul32)(p_eutran_rec->no_nr_neighbors_ts)),
			p_eutran_rec->ho_failed_for_pci_conflict,
			p_eutran_rec->mro_data.mro_ho_counter.num_too_early_ho,
			p_eutran_rec->mro_data.mro_ho_counter.num_too_late_ho,
			p_eutran_rec->mro_data.mro_ho_counter.
			num_ho_to_wrong_cell_for_failure_cell,
			p_eutran_rec->mro_data.mro_ho_counter.
			num_ho_to_wrong_cell_for_reestablishment_cell,
			p_eutran_rec->mro_data.mro_ho_counter.total_ho_attempted);

	SON_UT_TRACE_EXIT();
} /* anr_print_eutran_nbr */

/*****************************************************************************
 * Function Name  : anr_print_intra_rat_table_records
 * Inputs         : p_intra_hash_ctxt - Pointer to the hash context for a cell configured
 * Outputs        : None
 * Returns        : son_void_t        - None
 * Description    : This function prints records in the intra rat hash table
 ******************************************************************************/
static son_void_t
anr_print_intra_rat_table_records
(
 SON_HASH *p_intra_hash_ctxt
 )
{
	anr_intra_rat_hash_nr_tuple_t *p_node = SON_PNULL;


	SON_UT_TRACE_ENTER();
	p_node = anr_get_first_nr_from_intra_rat_hash_table(p_intra_hash_ctxt);

	while (SON_PNULL != p_node)
	{
		anr_print_eutran_nbr(&p_node->data);
		p_node = anr_get_next_nr_from_intra_rat_hash_table(p_intra_hash_ctxt,
				&p_node->anchor);
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_print_utran_nbr
 * Inputs         : p_utran_data - Pointer to utran record
 * Outputs        : None
 * Returns        : son_void_t   - None
 * Description    : This function prints records in the utran record
 ******************************************************************************/
static son_void_t
anr_print_utran_nbr
(
 son_utran_neighbor_info_t *p_utran_data
 )
{
	SON_UT_TRACE_ENTER();
	SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
			SON_INFO,
			"UTRAN Neighbor Cell [Cell Id: 0x%x] has attributes:\n"
			"Rat Type: %d Bitmask: %d Pci_Bitmask:%d Pci_fdd: %d Pci_tdd: %d\n"
			"Lac: %d Rac:%d csg_identity: %d Uarfcn: %d Rscp: %d Ecno: %d NR Status: %d HO Status: %d\n"
			"Cell Specific Offset: %d HOs Attempted:%d HOs Failed:%d HOs Successful:%d",
			son_convert_char_cell_id_to_int_cell_id(p_utran_data->nbr_cgi.cell_identity),
			p_utran_data->nbr_cgi.rat_type,
			p_utran_data->bitmask,
			p_utran_data->pci.bitmask,
			p_utran_data->pci.pci_fdd,
			p_utran_data->pci.pci_tdd,
			son_convert_char_cell_id_to_int_cell_id(p_utran_data->lac),
			p_utran_data->rac,
			son_convert_char_cell_id_to_int_cell_id(p_utran_data->csg_identity),
			p_utran_data->uarfcn,
			p_utran_data->rscp,
			p_utran_data->ecno,
			p_utran_data->nr_status,
			p_utran_data->ho_status,
			p_utran_data->cell_specific_offset,
			p_utran_data->handover_stats.handovers_attempted,
			p_utran_data->handover_stats.handovers_failed,
			p_utran_data->handover_stats.handovers_succesful);
	SON_UT_TRACE_EXIT();
} /* anr_print_utran_nbr */

/*****************************************************************************
 * Function Name  : anr_print_geran_nbr
 * Inputs         : p_geran_data - Pointer to geran record
 * Outputs        : None
 * Returns        : son_void_t   - None
 * Description    : This function prints records in the geran record
 ******************************************************************************/
static son_void_t
anr_print_geran_nbr
(
 son_geran_neighbor_info_t *p_geran_data
 )
{
	SON_UT_TRACE_ENTER();
	SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
			SON_BRIEF,
			"GERAN Neighbor Cell [Cell Id: 0x%x] has attributes:\n"
			"Rat Type: %d" "Bitmask: %d Pci_ncc: %d Pci_bscc: %d Lac: %d Rac:%d arfcn: %d\n"
			"band indicator: %d Rssi: %d NR Status: %d HO Status: %d DTM_Supported: %d\n"
			"Cell Specific Offset: %d HOs Attempted:%d HOs Failed:%d HOs Successful:%d",
			son_convert_char_cell_id_to_int_cell_id(p_geran_data->nbr_cgi.cell_identity),
			p_geran_data->nbr_cgi.rat_type,
			p_geran_data->bitmask,
			p_geran_data->pci.ncc,
			p_geran_data->pci.bscc,
			son_convert_char_cell_id_to_int_cell_id((p_geran_data->nbr_cgi.cell_identity) + 1), /*LAC value*/
			p_geran_data->rac,
			p_geran_data->arfcn,
			p_geran_data->band_ind,
			p_geran_data->rssi,
			p_geran_data->nr_status,
			p_geran_data->ho_status,
			p_geran_data->dtm_supported,
			p_geran_data->cell_specific_offset,
			p_geran_data->handover_stats.handovers_attempted,
			p_geran_data->handover_stats.handovers_failed,
			p_geran_data->handover_stats.handovers_succesful); /*SPR-11121-Start-End*/
	SON_UT_TRACE_EXIT();
} /* anr_print_geran_nbr */

/*****************************************************************************
 * Function Name  : anr_print_cdma_cell_id
 * Inputs         : p_nbr       - Pointer to XRTT/HRPD neighbour cell identity
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : Prints CDMA2000 NBR's ECGI
 ****************************************************************************/
son_void_t
anr_print_cdma_cell_id
(
 son_cdma_global_cell_id_t   *p_nbr
 )
{
	if (SON_PNULL == p_nbr)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"CDMA cell id cannot be printed, "
				"as p_nbr is NULL\n");

		SON_UT_TRACE_ENTER();
		/* Coverity Fix: 30723*/
		return;
	}

	if (p_nbr->bitmask & SON_CGI_1X_RTT_PRESENT)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_BRIEF,
				"CDMA XRTT cell id: 0x%x%x%x%x%x%x\n",
				p_nbr->cell_global_id_1X_RTT[0],
				p_nbr->cell_global_id_1X_RTT[1],
				p_nbr->cell_global_id_1X_RTT[2],
				p_nbr->cell_global_id_1X_RTT[3],
				p_nbr->cell_global_id_1X_RTT[4],
				p_nbr->cell_global_id_1X_RTT[5]);
	}
	else if (p_nbr->bitmask & SON_CGI_HRPD_PRESENT)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_BRIEF,
				"CDMA HRPD cell id: 0x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x\n",
				p_nbr->cell_global_id_HRPD[0],
				p_nbr->cell_global_id_HRPD[1],
				p_nbr->cell_global_id_HRPD[2],
				p_nbr->cell_global_id_HRPD[3],
				p_nbr->cell_global_id_HRPD[4],
				p_nbr->cell_global_id_HRPD[5],
				p_nbr->cell_global_id_HRPD[6],
				p_nbr->cell_global_id_HRPD[7],
				p_nbr->cell_global_id_HRPD[8],
				p_nbr->cell_global_id_HRPD[9],
				p_nbr->cell_global_id_HRPD[10],
				p_nbr->cell_global_id_HRPD[11],
				p_nbr->cell_global_id_HRPD[12],
				p_nbr->cell_global_id_HRPD[13],
				p_nbr->cell_global_id_HRPD[14],
				p_nbr->cell_global_id_HRPD[15]);
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Unknown CDMA type\n");
	}
} /* anr_print_cdma_cell_id */

/*****************************************************************************
 * Function Name  : anr_print_cdma_nbr
 * Inputs         : p_cdma_data - Pointer to cdma record
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : This function prints CDMA2000 NBR information
 ******************************************************************************/
static son_void_t
anr_print_cdma_nbr
(
 son_cdma_neighbor_info_t *p_cdma_data
 )
{
	SON_UT_TRACE_ENTER();
	anr_print_cdma_cell_id(&p_cdma_data->nbr_cgi);

	SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
			SON_DETAILED,
			"Bitmask: %d PCI: %d Band Class: %d arfcn: %d pilot pn phase: %d pilot pn strength: %d"
			"NR Status: %d HO Status: %d Broadcast Status: %d Cell Specific Offset: %d\n"
			"HOs Attempted:%d HOs Failed:%d HOs Successful:%d",
			p_cdma_data->bitmask,
			p_cdma_data->pci,
			p_cdma_data->band_class,
			p_cdma_data->arfcn,
			p_cdma_data->pilot_pn_phase,
			p_cdma_data->pilot_strength,
			p_cdma_data->nr_status,
			p_cdma_data->ho_status,
			p_cdma_data->broadcast_status,
			p_cdma_data->cell_specific_offset,
			p_cdma_data->handover_stats.handovers_attempted,
			p_cdma_data->handover_stats.handovers_failed,
			p_cdma_data->handover_stats.handovers_succesful);
	SON_UT_TRACE_EXIT();
} /* anr_print_cdma_nbr */

/*****************************************************************************
 * Function Name  : anr_print_inter_rat_record
 * Inputs         : p_inter_rec - Pointer to inter rat record
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : This function prints records for inter rat record
 ******************************************************************************/
static son_void_t
anr_print_inter_rat_record
(
 anr_inter_rat_hash_rec_t *p_inter_rec
 )
{
	SON_UT_TRACE_ENTER();
	if (ANR_UTRAN_NEIGHBOR ==  p_inter_rec->t)
	{
		anr_print_utran_nbr( &p_inter_rec->u.utran_neighbor_info );
	}
	else if (ANR_GERAN_NEIGHBOR ==  p_inter_rec->t)
	{
		anr_print_geran_nbr(&p_inter_rec->u.geran_neighbor_info);
	}
	else if (ANR_CDMA_NEIGHBOR == p_inter_rec->t)
	{
		anr_print_cdma_nbr(&p_inter_rec->u.cdma_neighbor_info);
	}
	else
	{
		/*Future use*/
	}

	SON_UT_TRACE_EXIT();
} /* anr_print_inter_rat_record */

/*****************************************************************************
 * Function Name  : anr_print_inter_rat_table_records
 * Inputs         : p_inter_hash_ctxt - Pointer to the hash context for a cell configured
 * Outputs        : None
 * Returns        : son_void_t        - None
 * Description    : This function prints records in the inter rat hash table
 ******************************************************************************/
static son_void_t
anr_print_inter_rat_table_records
(
 SON_HASH *p_inter_hash_ctxt
 )
{
	anr_inter_rat_hash_nr_tuple_t *p_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_node = anr_get_first_nr_from_inter_rat_hash_table( p_inter_hash_ctxt);
	while (SON_PNULL != p_node)
	{
		anr_print_inter_rat_record(&p_node->data);
		p_node = anr_get_next_nr_from_inter_rat_hash_table(p_inter_hash_ctxt, &p_node->anchor );
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_print_cell_context
 * Inputs         : p_ctxt      - Pointer to cell context
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : This function prints the specified cell context
 ******************************************************************************/
son_void_t
anr_print_cell_context
(
 anr_cell_context_t *p_ctxt
 )
{
	son_u8 idx = 0;
	son_u8 *p_tac = p_ctxt->tac;


	SON_UT_TRACE_ENTER();
	if (p_ctxt != SON_PNULL)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_BRIEF,
				"ANR Cell Context Structure for Cell- "
				"Cell Identity: 0x%x "
				"Current Cell FSM State: %s Previous Cell FSM State: %s "
				"Current Cell Operational State: %d"
				"PCI %d EARFCN %u Tac %d Number of PLMNs: %d Limited Mode: %d",
				son_convert_char_cell_id_to_int_cell_id(p_ctxt->cell_id.cell_identity),
				SON_ANR_CELL_FSM_STATES_NAMES[p_ctxt->current_cell_fsm_state],
				SON_ANR_CELL_FSM_STATES_NAMES[p_ctxt->previous_cell_fsm_state],
				p_ctxt->current_cell_state,
				p_ctxt->pci_value,
				anr_get_earfcn(p_ctxt),
				*p_tac,
				p_ctxt->num_plmn_id,
				p_ctxt->limited_mode);

		for (idx = 0; idx < p_ctxt->num_plmn_id; idx++)
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_DETAILED,
					"Idx [%d] Plmn Id  is -"
					"MCC[0] %d MCC[1] %d MCC[2] %d "
					"No of MNC digits is: %d "
					"MNC[0] %d MNC[1] %d MNC[2] %d ",
					idx + 1,
					p_ctxt->plmn_id[idx].mcc[0],
					p_ctxt->plmn_id[idx].mcc[1],
					p_ctxt->plmn_id[idx].mcc[2],
					p_ctxt->plmn_id[idx].num_mnc_digit,
					p_ctxt->plmn_id[idx].mnc[0],
					p_ctxt->plmn_id[idx].mnc[1],
					p_ctxt->plmn_id[idx].mnc[2]);
		}

		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_DETAILED,
				"\nRemoval Criteria- "
				"no_activity_interval: %d "
				"handover_failure_threshold: %d "
				"timeout_no_nr_neighbors: %d "
				"PCI Confusion configuration- "
				"suspect_pci_threshold: %d "
				"clear_suspect_pci_threshold: %d "
				"Number of cells in Remove List: %d "
				"Number of Intra-Rat Neighbors: %d "
				"Number of Inter-Rat Neighbors: %d "
				"Number of elements in Txn Id List: %d",
				p_ctxt->removal_criteria.no_activity_interval,
				p_ctxt->removal_criteria.handover_failure_threshold,
				p_ctxt->removal_criteria.timeout_no_nr_neighbors,
				p_ctxt->pci_confusion_cfg.suspect_pci_threshold,
				p_ctxt->pci_confusion_cfg.clear_suspect_pci_threshold,
				p_ctxt->nr_remove_list.count,
				hash_count(&p_ctxt->intra_rat_neighbor_table),
				hash_count(&p_ctxt->inter_rat_neighbor_table),
				p_ctxt->txn_id_list.count);
		anr_print_intra_rat_table_records(&p_ctxt->intra_rat_neighbor_table );
		anr_print_inter_rat_table_records(&p_ctxt->inter_rat_neighbor_table );
	}

	SON_UT_TRACE_EXIT();
} /* anr_print_cell_context */

/*****************************************************************************
 * Function Name  : anr_reset_pending_res
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : Resets pending_response structure
 ****************************************************************************/
son_void_t
anr_reset_pending_res
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */

)
{
	SON_UT_TRACE_ENTER();
	anr_print_pending_res("anr_reset_pending_res: before reset");
	g_anr_pending_response.api_id = SON_PROCEDURE_UNDEFINED;
	g_anr_pending_response.transaction_id = SON_UNDEFINED;
	g_anr_pending_response.expected_res_count = 0;
	g_anr_pending_response.is_error_received = SON_FALSE;
	SON_MEMSET(&g_anr_pending_response.ts, 0, sizeof(time_stamp_t));
	if (g_anr_pending_response.cell_sts_list.count)
	{
		anr_clear_pending_res_list();
	}

	list_init(&g_anr_pending_response.cell_sts_list);
	g_anr_pending_response.ts = 0;
	anr_print_pending_res("anr_reset_pending_res: after reset");
	SON_UT_TRACE_EXIT();
} /* anr_reset_pending_res */

/*****************************************************************************
 * Function Name  : anr_init_pending_res
 * Inputs         : tid     - Transaction Id
 *                : api     - Procedure Code
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : Initialize pending_response structure
 ****************************************************************************/
son_void_t
anr_init_pending_res
(
 son_u16 tid,
 son_procedure_code_et api
 )
{
	SON_UT_TRACE_ENTER();
	g_anr_pending_response.api_id = api;
	g_anr_pending_response.transaction_id = tid;
	g_anr_pending_response.expected_res_count = MIN_PENDING_RES_COUNT;
	g_anr_pending_response.is_error_received = SON_FALSE;
	list_init(&g_anr_pending_response.cell_sts_list);
	son_time(&g_anr_pending_response.ts);
	SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
			SON_DETAILED,
			"Pending Response Initialized");

	anr_print_pending_res("anr_init_pending_res");

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_get_pending_res_transaction_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u16 - Returns transaction id
 * Description    : Returns transaction id of present in pending response
 ****************************************************************************/
son_u16
anr_get_pending_res_transaction_id
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_anr_pending_response.transaction_id;
}

/*****************************************************************************
 * Function Name  : anr_get_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u8  - Response count
 * Description    : Returns Expected Response Count present in pending response
 ****************************************************************************/
son_u8
anr_get_pending_res_expected_count
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_anr_pending_response.expected_res_count;
}

/*****************************************************************************
 * Function Name  : anr_increment_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : Increment Expected Response Count present in pending response
 ****************************************************************************/
son_void_t
anr_increment_pending_res_expected_count
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	if (g_anr_pending_response.expected_res_count < MAX_PENDING_RES_COUNT)
	{
		g_anr_pending_response.expected_res_count++;
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Max value"
				" of Expected Response Count in Pending Response reached");
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_decrement_pending_res_expected_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : Decrement Expected Response Count present in pending response
 ****************************************************************************/
son_void_t
anr_decrement_pending_res_expected_count
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	if (g_anr_pending_response.expected_res_count > MIN_PENDING_RES_COUNT)
	{
		g_anr_pending_response.expected_res_count--;
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Min value of Expected"
				"Response Count in Pending Response "
				"reached");
	}

	SON_UT_TRACE_EXIT();
} /* anr_decrement_pending_res_expected_count */

/*****************************************************************************
 * Function Name  : anr_get_pending_res_api_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_procedure_code_et - returns api id
 * Description    : Get API ID present in pending response
 ****************************************************************************/
son_procedure_code_et
anr_get_pending_res_api_id
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_anr_pending_response.api_id;
}

/*****************************************************************************
 * Function Name  : anr_get_pending_res_cell_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_LIST* - Returns pointer to cell status list
 * Description    : Returns pointer to cell_status_list present in pending response
 ****************************************************************************/
SON_LIST *
anr_get_pending_res_cell_list
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return &g_anr_pending_response.cell_sts_list;
}

/*****************************************************************************
 * Function Name  : anr_get_pending_res_ts_diff_from_curr_ts
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u32 - Returns timestamp
 * Description    : Returns difference between current timestamp and timestamp
 *                : present in pending response
 ****************************************************************************/
son_u32
anr_get_pending_res_ts_diff_from_curr_ts
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	time_stamp_t curr_ts;


	son_time(&curr_ts);
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return ((son_u32)son_difftime(curr_ts, g_anr_pending_response.ts));
}

/*****************************************************************************
 * Function Name  : anr_set_pending_res_ts_with_current_timestamp
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : Set timestamp present in pending response with current time
 ****************************************************************************/
son_void_t
anr_set_pending_res_ts_with_current_timestamp
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	son_time(&g_anr_pending_response.ts);
	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_set_pending_res_error_rcvd
 * Inputs         : val         - SON_TRUE,SON_FALSE
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : Set is_eror_received flag present in pending response
 ****************************************************************************/
son_void_t
anr_set_pending_res_error_rcvd
(
 son_bool_et val
 )
{
	SON_UT_TRACE_ENTER();
	if (g_anr_pending_response.is_error_received != val)
	{
		g_anr_pending_response.is_error_received = val;
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_get_pending_res_error_rcvd
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_bool_et     - SON_TRUE if no error received in response
 *                                    otherwise SON_FALSE
 * Description    : Get is_eror_received flag present in pending response
 ****************************************************************************/
son_bool_et
anr_get_pending_res_error_rcvd
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_anr_pending_response.is_error_received;
}

/*****************************************************************************
 * Function Name  : anr_find_cell_in_pending_res
 * Input          : p_rcvd_cell_id      - Pointer to ECGI of the cell
 * Outputs        : None
 * Returns        : cell_status_list_data_t* - returns Pointer to cell status list
 * Description    : Find cell_status node in the cell_status_list
 ****************************************************************************/
cell_status_list_data_t *
anr_find_cell_in_pending_res
(
 son_intra_rat_global_cell_id_t  *p_rcvd_cell_id
 )
{
	SON_LIST_NODE            *p_node             = SON_PNULL;
	cell_status_list_data_t *p_list_node         = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_rcvd_cell_id)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Cell ID is NULL");

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	p_node = list_find_node(&g_anr_pending_response.cell_sts_list,
			(son_void_t *)p_rcvd_cell_id, son_key_of_anr,
			son_compare_cell_id);
	if (SON_PNULL != p_node)
	{
		p_list_node = YMEMBEROF(cell_status_list_data_t, cell_status_node, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* anr_find_cell_in_pending_res */

/*****************************************************************************
 * Function Name  : anr_delete_succesful_cells_from_pending_res
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : Delete cell_status nodes with result = SON_SUCCESS in
 *                  the cell_status_list
 ****************************************************************************/
son_void_t
anr_delete_succesful_cells_from_pending_res
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_LIST_NODE            *p_node         = SON_PNULL;
	SON_LIST_NODE           *p_next_node     = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_node = get_first_list_node(&g_anr_pending_response.cell_sts_list);

	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF(cell_status_list_data_t,
				cell_status_node, p_node);

		if (SON_NO_ERROR == p_list_node->cell_status.error_code)
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_BRIEF,
					"Cell [Cell Id: 0x%x] Status node  with error = SON_NO_ERROR "
					"is successfully deleted from ANR pending response",
					son_convert_char_cell_id_to_int_cell_id(p_list_node->cell_status.
						cgi.cell_identity));
			p_next_node = get_next_list_node(p_node);
			list_delete_node(&g_anr_pending_response.cell_sts_list, p_node);
			son_mem_free(p_list_node);
			p_node = p_next_node;
		}
		else
		{
			p_node = get_next_list_node(p_node);
		}
	}

	anr_print_pending_res("after anr_delete_succesful_cells_from_pending_res");

	SON_UT_TRACE_EXIT();
} /* anr_delete_succesful_cells_from_pending_res */

/*****************************************************************************
 * Function Name  : anr_add_cell_in_pending_res
 * Inputs         : p_sts_node  - Pointer to Cell Status Node
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE if cell is added into the pending
 *                                list, otherwise SON_FALSE
 * Description    : Add cell_status node in the cell_status_list
 ****************************************************************************/
son_bool_et
anr_add_cell_in_pending_res
(
 son_cell_status_t    *p_sts_node
 )
{
	cell_status_list_data_t *p_list_node = SON_PNULL;
	son_bool_et ret_val      = SON_FALSE;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_sts_node &&
			g_anr_pending_response.expected_res_count < MAX_PENDING_RES_COUNT)
	{
		p_list_node = son_mem_get(sizeof(cell_status_list_data_t));
		if (SON_PNULL != p_list_node)
		{
			SON_MEMCPY(&p_list_node->cell_status, p_sts_node,
					sizeof(son_cell_status_t));

			list_insert_node(&g_anr_pending_response.cell_sts_list,
					&p_list_node->cell_status_node);

			anr_increment_pending_res_expected_count();
			ret_val = SON_TRUE;

			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_BRIEF,
					"Added cell [Cell Id: 0x%x]"
					" in Pending Response", son_convert_char_cell_id_to_int_cell_id(p_sts_node->cgi.cell_identity));
			anr_print_pending_res("anr_add_cell_in_pending_res");
		}
		else
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_ERROR,
					"Memory allocation failure");
		}
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Either Invalid input parameters [p_sts_node] is NULL or pending "
				"response count reached max limit[%d]", MAX_PENDING_RES_COUNT);
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_add_cell_in_pending_res */

/*******************************************************************************
 * Function Name  : anr_delete_cell_from_pending_res
 * Inputs         : p_cell_id   - Pointer to Cell Id
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : This function deletes cell_status node from the cell_status_list
 *******************************************************************************/
son_void_t
anr_delete_cell_from_pending_res
(
 son_intra_rat_global_cell_id_t  *p_cell_id
 )
{
	cell_status_list_data_t        *p_node             = SON_PNULL;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL != p_cell_id)
	{
		p_node = anr_find_cell_in_pending_res(p_cell_id);

		if (SON_PNULL != p_node)
		{
			list_delete_node(&g_anr_pending_response.cell_sts_list,
					&p_node->cell_status_node);

			son_mem_free(p_node);

			anr_decrement_pending_res_expected_count();
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_BRIEF,
					"Deleted cell "
					"[Cell Id: 0x%x] from Pending Response",
					son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
			anr_print_pending_res("After deletion in "
					"anr_delete_cell_from_pending_res");
		}
		else
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_ERROR,
					"Node for cell [Cell Id: 0x%x]"
					" not found", son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
		}
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Cell ID is NULL");
	}

	SON_UT_TRACE_EXIT();
} /* anr_delete_cell_from_pending_res */

/*****************************************************************************
 * Function Name  : anr_clear_pending_res_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : Delete all cell_status node from the cell_status_list
 ****************************************************************************/
son_void_t
anr_clear_pending_res_list
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_LIST_NODE            *p_node         = SON_PNULL;
	cell_status_list_data_t *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_node = list_pop_node(&g_anr_pending_response.cell_sts_list);

	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF(cell_status_list_data_t,
				cell_status_node, p_node);
		son_mem_free(p_list_node);
		p_node = list_pop_node(&g_anr_pending_response.cell_sts_list);
	}

	SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
			SON_DETAILED,
			"Cleared pending response list");
	anr_print_pending_res("anr_clear_pending_res_list");

	SON_UT_TRACE_EXIT();
} /* anr_clear_pending_res_list */

/*****************************************************************************
 * Function Name  : anr_update_cell_in_pending_res
 * Inputs         : p_rcvd_cell_id - Pointer of ECGI of cell
 *                : err_code       - error_code
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE if pending response list successfully
 *                                updated, otherwise SON_FALSE
 * Description    : Update cell_status node in the cell_status_list
 ****************************************************************************/
son_bool_et
anr_update_cell_in_pending_res
(
 son_intra_rat_global_cell_id_t  *p_rcvd_cell_id,
 son_error_et err_code
 )
{
	cell_status_list_data_t *p_list_node = SON_PNULL;
	son_bool_et ret_val = SON_TRUE;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_rcvd_cell_id)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Cell ID is NULL");
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	p_list_node = anr_find_cell_in_pending_res(p_rcvd_cell_id);

	if (SON_PNULL != p_list_node)
	{
		p_list_node->cell_status.error_code = err_code;

		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_BRIEF,
				"Updated cell "
				"[Cell Id: 0x%x] in pending response with "
				" error = %d", son_convert_char_cell_id_to_int_cell_id(p_rcvd_cell_id->cell_identity), err_code);
		anr_print_pending_res("anr_update_cell_in_pending_res");
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Node for cell [Cell Id: 0x%x]"
				" to be updated is not found", son_convert_char_cell_id_to_int_cell_id(p_rcvd_cell_id->cell_identity));
		ret_val = SON_FALSE;
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_update_cell_in_pending_res */

/*****************************************************************************
 * Function Name  : anr_construct_cell_arr_from_pending_res_list
 * Inputs         : None
 * Output         : p_cell_sts_arr - pointer to cell status
 * Returns        : son_void_t     - None
 * Description    : Construct cell_status_arr from cell_status_list
 ****************************************************************************/
son_void_t
anr_construct_cell_arr_from_pending_res_list
(
 son_cell_status_t             *p_cell_sts_arr
 )
{
	SON_LIST_NODE            *p_node             = SON_PNULL;
	cell_status_list_data_t *p_list_node     = SON_PNULL;
	son_u8 idx = 0;
	son_u8 count = list_count(&g_anr_pending_response.cell_sts_list);


	SON_UT_TRACE_ENTER();

	if (count > MIN_PENDING_RES_COUNT)
	{
		p_node = get_first_list_node(&g_anr_pending_response.cell_sts_list);
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_DETAILED,
				"Constructing Cell Array from pending Response");

		anr_print_pending_res("anr_construct_cell_arr_from_pending_res_list");

		while (SON_PNULL != p_node)
		{
			p_list_node = YMEMBEROF(cell_status_list_data_t, cell_status_node,
					p_node);
			SON_MEMCPY(&p_cell_sts_arr[idx++], &p_list_node->cell_status,
					sizeof(son_cell_status_t));
			p_node = get_next_list_node(p_node);
		}
	}

	SON_UT_TRACE_EXIT();
} /* anr_construct_cell_arr_from_pending_res_list */

/*****************************************************************************
 * Function Name  : son_key_of_anr
 * Inputs         : p_node          - anchor field of the NR record
 * Outputs        : None
 * Returns        : son_void_t *    - pointer to cell id as void pointer
 * Description    : This function determines the key in ANR
 ******************************************************************************/
const son_void_t *
son_key_of_anr
(
 const SON_LIST_NODE *p_node
 )
{
	anr_cell_context_node_t *p_data = SON_PNULL;
	son_intra_rat_global_cell_id_t  *p_cell_id = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_cell_context_node_t, sNode, p_node);
	p_cell_id = &(p_data->data.cell_id);

	SON_UT_TRACE_EXIT();
	return (son_void_t *)p_cell_id;
}

/*SPR_19279_START*/
const son_void_t *
son_cell_index_key_of_anr
(
 const SON_LIST_NODE *p_node
 )
{
	anr_cell_context_node_t *p_data = SON_PNULL;
	son_u8  *p_cell_index = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_cell_context_node_t, sNode, p_node);
	p_cell_index = &(p_data->data.cell_index);

	SON_UT_TRACE_EXIT();
	return (son_void_t *)p_cell_index;
}
/*SPR_19279_END*/

/*****************************************************************************
 * Function Name  : anr_key_of_txn_id_list
 * Inputs         : p_node       - anchor field of the transaction Id list record
 * Outputs        : None
 * Returns        : son_void_t * - pointer to transaction Id as void pointer
 * Description    : This function determines the key in ANR transaction Id list
 ******************************************************************************/
const son_void_t *
anr_key_of_txn_id_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_txn_id_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_txn_id_node_t, txn_list_node, p_node);

	return (son_void_t *)(&p_data->txn_id);
}

/*SPR-9574-Start*/

/*SPR-11554-Start*/

/*Function for determining PCI range end value shifted to common area*/

/*SPR-11554-End*/

/*****************************************************************************
 * Function Name  : anr_insert_in_intra_freq_list
 * Inputs         : p_cell  - Pointer to cell inserted in global ANR context
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Function to insert the intra frequency and its corresponding
 *                  pci ranges in intra frquency list
 *****************************************************************************/
static son_void_t
anr_insert_in_intra_freq_list
(
 son_cell_t    *p_cell
 )
{

	anr_intra_freq_list_node_t  *p_freq_list_node = SON_PNULL;
	son_u32                      temp_earfcn = 0;

	SON_UT_TRACE_ENTER();

	/*Check if the cell is in fdd mode or tdd mode*/
	if(p_cell->choice_eutra_mode.bitmask & SON_EUTRA_MODE_FDD)
	{
		temp_earfcn = p_cell->choice_eutra_mode.fdd_info.dl_earfcn;
	}
	else if(p_cell->choice_eutra_mode.bitmask & SON_EUTRA_MODE_TDD)
	{
		temp_earfcn = p_cell->choice_eutra_mode.tdd_info.earfcn;
	}

	/*Finding the frequency in Intra frequency list*/
	p_freq_list_node = anr_find_freq_in_intra_freq_list(temp_earfcn);

	/*If earfcn is not already present in the intra freq list,then store it alongwith pci ranges*/
	if(p_freq_list_node == SON_PNULL)
	{
		p_freq_list_node = (anr_intra_freq_list_node_t *)son_mem_get
			(sizeof(anr_intra_freq_list_node_t));

		if(p_freq_list_node !=SON_PNULL)
		{
			SON_MEMSET(p_freq_list_node, 0x00, sizeof(anr_intra_freq_list_node_t));
			/*Inserting the earfcn value and corresponding pci ranges*/
			p_freq_list_node->intra_freq_value.earfcn = temp_earfcn;

			/*SPR-10426-Start*/
			if(p_cell->bitmask & SON_CELL_INTRA_FREQ_OPEN_PCI_RANGE_PRESENT)
			{
				p_freq_list_node->intra_freq_value.open_pci_range_present = SON_TRUE;
				SON_MEMCPY(&p_freq_list_node->intra_freq_value.intra_freq_open_pci_range, &p_cell->intra_freq_open_pci_range,
						sizeof(son_per_freq_pci_range_t));
			}

			if(p_cell->bitmask & SON_CELL_INTRA_FREQ_CLOSED_PCI_RANGE_PRESENT)
			{
				p_freq_list_node->intra_freq_value.closed_pci_range_present = SON_TRUE;
				SON_MEMCPY(&p_freq_list_node->intra_freq_value.intra_freq_closed_pci_range, &p_cell->intra_freq_closed_pci_range,
						sizeof(son_per_freq_pci_range_t));
			}

			if(p_cell->bitmask & SON_CELL_INTRA_FREQ_HYBRID_PCI_RANGE_PRESENT)
			{
				p_freq_list_node->intra_freq_value.hybrid_pci_range_present = SON_TRUE;
				SON_MEMCPY(&p_freq_list_node->intra_freq_value.intra_freq_hybrid_pci_range, &p_cell->intra_freq_hybrid_pci_range,
						sizeof(son_per_freq_pci_range_t));
			}
			/*SPR-10426-End*/

			list_insert_node(&g_anr_context.intra_freq_list, &p_freq_list_node->anchor);

			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_INFO,
					"Intra Freq EARFCN %d added in list",
					temp_earfcn);
		}
		else
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"Memory allocation failure for intra freq list");
		}
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_WARNING,
				"EARFCN %d already present in intra freq list",
				temp_earfcn);
	}

	SON_UT_TRACE_EXIT();
} /* anr_insert_in_intra_freq_list */


/*****************************************************************************
 * Function Name  : anr_find_freq_in_intra_freq_list
 * Inputs         : earfcn  - Frequency
 * Outputs        : None
 * Returns        : anr_intra_freq_list_node_t * - Pointer to matching frequency 
 *                  node
 * Description    : This function finds the node for the
 *                  given earfcn value
 *****************************************************************************/
anr_intra_freq_list_node_t *
anr_find_freq_in_intra_freq_list
(
 son_u32                       temp_earfcn
 )
{
	SON_LIST_NODE                     *p_freq_node       = SON_PNULL;
	anr_intra_freq_list_node_t        *p_freq_list_node  = SON_PNULL;
	SON_LIST                          *p_list            = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list = &g_anr_context.intra_freq_list;

	/* earfcn level search */
	p_freq_node = list_find_node(p_list, (son_void_t *)(&temp_earfcn),
			anr_key_of_intra_freq_list,
			anr_compare_intra_freq_list_node);

	if (p_freq_node != SON_PNULL)
	{
		p_freq_list_node = YMEMBEROF(anr_intra_freq_list_node_t,
				anchor, p_freq_node);
	}

	SON_UT_TRACE_EXIT();
	return p_freq_list_node;
} /* anr_find_freq_in_intra_freq_list */

/*****************************************************************************
 * Function Name  : anr_key_of_intra_freq_list
 * Inputs         : p_node -  Anchor to intra freq node
 * Outputs        : None
 * Returns        : son_void_t * - void type pointer to frequency node
 * Description    : This function generates the key
 ******************************************************************************/
const son_void_t *
anr_key_of_intra_freq_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_intra_freq_list_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_intra_freq_list_node_t,
			anchor, p_node);

	return (son_void_t *)(&p_data->intra_freq_value);
}

/*****************************************************************************
 * Function Name  : anr_compare_intra_freq_list_node
 * Inputs         : p_val_1  - Pointer to the key field(frequency)as void pointer
 *                : p_val_2  - Pointer to the key field(frequency)as void pointer
 * Outputs        : None
 * Returns        : son_s32  - 0 If matches
 *                             Non zero if not matches
 * Description    : This function compares the key field(frequency) of the
 *                  frequency node in the list
 ******************************************************************************/
son_s32
anr_compare_intra_freq_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	son_u16 *p_data_1 = SON_PNULL;
	son_u16 *p_data_2 = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointers passed are  NULL");
		SON_UT_TRACE_EXIT();
		return ret_value;
	}
	else
	{
		p_data_1 = (son_u16 *)p_val_1;
		p_data_2 = (son_u16 *)p_val_2;

		if ((*p_data_1 == *p_data_2))
		{
			ret_value = 0;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_intra_freq_list_node */


/*****************************************************************************
 * Function Name  : anr_insert_in_inter_freq_list
 * Inputs         : p_in_config_list    - Pointer to the EUTRAN neighbor cells
 *                                        info list received from OAM
 *                  list_size           - Size of EUTRAN neighbor cells info
 *                                        list received from OAM
 * Outputs        :
 * Returns        : son_void_t          - None
 * Description    : This function is used to populate the inter frequency list
 *                  in ANR global context
 ****************************************************************************/
son_void_t
anr_insert_in_inter_freq_list
(
 const son_meas_config_eutran_t      *p_in_config_list,
 const son_u8                        list_size
 )
{
	son_u8 index = 0;
	anr_inter_freq_list_node_t  *p_freq_list_node = SON_PNULL;
	son_u32                      temp_earfcn = 0;

	SON_UT_TRACE_ENTER();

	if (p_in_config_list == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Pointer to measurement configuration info for EUTRAN is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	for (; index < list_size; index++)
	{
		p_freq_list_node = SON_PNULL;
		temp_earfcn = 0;
		temp_earfcn =  p_in_config_list[index].earfcn;

		/*Finding the frequency in Inter frequency list*/
		p_freq_list_node = anr_find_freq_in_inter_freq_list(temp_earfcn);

		/*If earfcn is not already present in the inter freq list,then store it alongwith pci ranges*/
		if(p_freq_list_node == SON_PNULL)
		{
			p_freq_list_node = (anr_inter_freq_list_node_t *)son_mem_get
				(sizeof(anr_inter_freq_list_node_t));

			if(p_freq_list_node !=SON_PNULL)
			{
				SON_MEMSET(p_freq_list_node, 0x00, sizeof(anr_inter_freq_list_node_t));
				/*Inserting the earfcn value and corresponding pci ranges*/
				p_freq_list_node->inter_freq_value.earfcn = temp_earfcn;

				/*SPR-10426-Start*/
				if(p_in_config_list[index].bitmask & SON_MC_INTER_FREQ_OPEN_PCI_RANGE_PRESENT)
				{
					p_freq_list_node->inter_freq_value.open_pci_range_present = SON_TRUE;
					SON_MEMCPY(&p_freq_list_node->inter_freq_value.inter_freq_open_pci_range, &p_in_config_list[index].inter_freq_open_pci_range,
							sizeof(son_per_freq_pci_range_t));
				}

				if(p_in_config_list[index].bitmask & SON_MC_INTER_FREQ_CLOSED_PCI_RANGE_PRESENT)
				{
					p_freq_list_node->inter_freq_value.closed_pci_range_present = SON_TRUE;
					SON_MEMCPY(&p_freq_list_node->inter_freq_value.inter_freq_closed_pci_range, &p_in_config_list[index].inter_freq_closed_pci_range,
							sizeof(son_per_freq_pci_range_t));
				}

				if(p_in_config_list[index].bitmask & SON_MC_INTER_FREQ_HYBRID_PCI_RANGE_PRESENT)
				{
					p_freq_list_node->inter_freq_value.hybrid_pci_range_present = SON_TRUE;
					SON_MEMCPY(&p_freq_list_node->inter_freq_value.inter_freq_hybrid_pci_range, &p_in_config_list[index].inter_freq_hybrid_pci_range,
							sizeof(son_per_freq_pci_range_t));
				}
				/*SPR-10426-End*/

				list_insert_node(&g_anr_context.inter_freq_list, &p_freq_list_node->anchor);

				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_INFO,
						"EARFCN %d added to inter freq list",
						temp_earfcn);
			}
			else
			{
				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_ERROR,
						"Memory allocation failure for inter freq list");
			}
		}
		else
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_WARNING,
					"EARFCN %d already present in inter freq list",
					temp_earfcn);
		}

	}
}

/*****************************************************************************
 * Function Name  : anr_find_freq_in_inter_freq_list
 * Inputs         : earfcn  - Frequency
 * Outputs        : None
 * Returns        : anr_inter_freq_list_node_t * - Pointer to matching frequency 
 *                  node
 * Description    : This function finds the node for the
 *                  given earfcn value
 *****************************************************************************/
anr_inter_freq_list_node_t *
anr_find_freq_in_inter_freq_list
(
 son_u32                       temp_earfcn
 )
{
	SON_LIST_NODE                     *p_freq_node       = SON_PNULL;
	anr_inter_freq_list_node_t        *p_freq_list_node  = SON_PNULL;
	SON_LIST                          *p_list            = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list = &g_anr_context.inter_freq_list;

	/* earfcn level search */
	p_freq_node = list_find_node(p_list, (son_void_t *)(&temp_earfcn),
			anr_key_of_inter_freq_list,
			anr_compare_inter_freq_list_node);

	if (p_freq_node != SON_PNULL)
	{
		p_freq_list_node = YMEMBEROF(anr_inter_freq_list_node_t,
				anchor, p_freq_node);
	}

	SON_UT_TRACE_EXIT();
	return p_freq_list_node;
} /* anr_find_freq_in_inter_freq_list */

/*****************************************************************************
 * Function Name  : anr_key_of_inter_freq_list
 * Inputs         : p_node -  Anchor to inter freq node
 * Outputs        : None
 * Returns        : son_void_t * - void type pointer to frequency node
 * Description    : This function generates the key
 ******************************************************************************/
const son_void_t *
anr_key_of_inter_freq_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_inter_freq_list_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_inter_freq_list_node_t,
			anchor, p_node);

	return (son_void_t *)(&p_data->inter_freq_value);
}

/*****************************************************************************
 * Function Name  : anr_compare_inter_freq_list_node
 * Inputs         : p_val_1  - Pointer to the key field(frequency)as void pointer
 *                : p_val_2  - Pointer to the key field(frequency)as void pointer
 * Outputs        : None
 * Returns        : son_s32  - 0 If matches
 *                             Non zero if not matches
 * Description    : This function compares the key field(frequency) of the
 *                  frequency node in the list
 ******************************************************************************/
son_s32
anr_compare_inter_freq_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	son_u16 *p_data_1 = SON_PNULL;
	son_u16 *p_data_2 = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointers passed are  NULL");
		SON_UT_TRACE_EXIT();
		return ret_value;
	}
	else
	{
		p_data_1 = (son_u16 *)p_val_1;
		p_data_2 = (son_u16 *)p_val_2;

		if ((*p_data_1 == *p_data_2))
		{
			ret_value = 0;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_inter_freq_list_node */

/*SPR-9574-End*/

/*SPR-12596-Start*/

/*****************************************************************************
 * Function Name  : anr_insert_in_conflict_confusion_pci_list
 * Inputs         : p_cell_ctxt    -      Pointer to the cell context
 *                  cgi_1               - cgi of neighbor one
 *                  cgi_2               - cgi of neighbor two
 *                  pci                 - pci involved in conflict/confusion
 *                  cause               - cause conflict/confusion
 * Outputs        :
 * Returns        : son_void_t          - None
 * Description    : This function is used to insert pci in conflict confusion 
 *                  pci list                
 ****************************************************************************/
son_void_t
anr_insert_in_conflict_confusion_pci_list
(
 anr_cell_context_t                   *p_cell_ctxt,
 son_intra_rat_global_cell_id_t        cgi_1,
 son_intra_rat_global_cell_id_t        cgi_2,
 son_u16                               pci,
 son_pci_cause_et                      cause
 )
{
	conflict_confusion_pci_list_node_t  *p_pci_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	/*Finding the pci in conflict confusion PCI list*/
	p_pci_list_node = anr_find_pci_in_conflict_confusion_pci_list(p_cell_ctxt, pci);

	/*If pci is not already present in the list,then store it alongwith other data*/
	if(p_pci_list_node == SON_PNULL)
	{
		p_pci_list_node = (conflict_confusion_pci_list_node_t *)son_mem_get
			(sizeof(conflict_confusion_pci_list_node_t));

		if(p_pci_list_node !=SON_PNULL)
		{
			SON_MEMSET(p_pci_list_node, 0x00, sizeof(conflict_confusion_pci_list_node_t));
			/*Inserting the pci and other data*/
			SON_MEMCPY(&p_pci_list_node->pci_data.src_cgi, &p_cell_ctxt->cell_id,
					sizeof(son_intra_rat_global_cell_id_t));

			SON_MEMCPY(&p_pci_list_node->pci_data.cgi_1, &cgi_1,
					sizeof(son_intra_rat_global_cell_id_t));

			SON_MEMCPY(&p_pci_list_node->pci_data.cgi_2, &cgi_2,
					sizeof(son_intra_rat_global_cell_id_t));

			p_pci_list_node->pci_data.pci = pci;

			if(SON_PCI_CONFLICT == cause)
			{
				p_pci_list_node->pci_data.is_in_conflict = SON_TRUE;
			}
			else if(SON_PCI_CONFUSION == cause)
			{
				p_pci_list_node->pci_data.is_confused = SON_TRUE;
				p_pci_list_node->pci_data.confusion_counter++;
			}

			list_insert_node(&p_cell_ctxt->conflict_confusion_pci_list, &p_pci_list_node->anchor);

			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_INFO,
					"PCI %d added to conflict confusion list",
					pci);
		}
		else
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_ERROR,
					"Memory allocation failure");
		}
	}
	else
	{
		if(SON_PCI_CONFLICT == cause)
		{
			p_pci_list_node->pci_data.is_in_conflict = SON_TRUE;
		}
		else if(SON_PCI_CONFUSION == cause)
		{
			p_pci_list_node->pci_data.is_confused = SON_TRUE;
			p_pci_list_node->pci_data.confusion_counter++;
		}

		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_WARNING,
				"PCI %d already present in conflict confusion list is"
				" again detected for reason %d",pci,cause);
	}

	SON_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : anr_find_pci_in_conflict_confusion_pci_list
 * Inputs         : p_cell_ctxt  - Cell Context
 *                  pci 
 * Outputs        : None
 * Returns        : anr_inter_freq_list_node_t * - Pointer to matching pci 
 *                  node
 * Description    : This function finds the node for the
 *                  given pci value
 *****************************************************************************/
conflict_confusion_pci_list_node_t *
anr_find_pci_in_conflict_confusion_pci_list
(
 anr_cell_context_t            *p_cell_ctxt,
 son_u16                        pci
 )
{
	SON_LIST_NODE                        *p_pci_node       = SON_PNULL;
	conflict_confusion_pci_list_node_t   *p_pci_list_node  = SON_PNULL;
	SON_LIST                             *p_list            = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list = &p_cell_ctxt->conflict_confusion_pci_list;

	/* pci level search */
	p_pci_node = list_find_node(p_list, (son_void_t *)(&pci),
			anr_key_of_conflict_confusion_list,
			anr_compare_pci_list_node);

	if (p_pci_node != SON_PNULL)
	{
		p_pci_list_node = YMEMBEROF(conflict_confusion_pci_list_node_t,
				anchor, p_pci_node);
	}

	SON_UT_TRACE_EXIT();
	return p_pci_list_node;
} /* anr_find_pci_in_conflict_confusion_pci_list */

/*****************************************************************************
 * Function Name  : anr_key_of_conflict_confusion_list
 * Inputs         : p_node -  Anchor to pci node
 * Outputs        : None
 * Returns        : son_void_t * - void type pointer to pci node
 * Description    : This function generates the key
 ******************************************************************************/
const son_void_t *
anr_key_of_conflict_confusion_list
(
 const SON_LIST_NODE *p_node
 )
{
	conflict_confusion_pci_list_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(conflict_confusion_pci_list_node_t,
			anchor, p_node);

	return (son_void_t *)(&p_data->pci_data);
}

/*****************************************************************************
 * Function Name  : anr_compare_pci_list_node
 * Inputs         : p_val_1  - Pointer to the key field(pci)as void pointer
 *                : p_val_2  - Pointer to the key field(pci)as void pointer
 * Outputs        : None
 * Returns        : son_s32  - 0 If matches
 *                             Non zero if not matches
 * Description    : This function compares the key field(pci) of the
 *                  pci node in the list
 ******************************************************************************/
son_s32
anr_compare_pci_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	son_u16 *p_data_1 = SON_PNULL;
	son_u16 *p_data_2 = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointers passed are  NULL");
		SON_UT_TRACE_EXIT();
		return ret_value;
	}
	else
	{
		p_data_1 = (son_u16 *)p_val_1;
		p_data_2 = (son_u16 *)p_val_2;

		if ((*p_data_1 == *p_data_2))
		{
			ret_value = 0;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_pci_list_node */

/*SPR-12596-End*/

/*SPR-9759-Start*/

/*****************************************************************************
 * Function Name  : anr_find_record_in_eutran_cdma_pci_value_list
 * Inputs         : p_list  - Pointer to the list of pci values of eutran or
 *                            cdma cells
 *                  pci     - pci value
 * Outputs        : None
 * Returns        : anr_eutran_cdma_pci_value_node_t * - Pointer to pci value node
 * Description    : This function finds the node for the
 *                  given pci value
 *****************************************************************************/
anr_eutran_cdma_pci_value_node_t *
anr_find_record_in_eutran_cdma_pci_value_list
(
 SON_LIST                     *p_list,
 /*SPR 18360 Fix Start*/
 /*Lines Deleted*/
 anr_eutran_cdma_key_t        *p_key
 /*SPR 18360 Fix Stop*/
 )
{
	SON_LIST_NODE                     *p_pci_node             = SON_PNULL;
	anr_eutran_cdma_pci_value_node_t  *p_pci_list_node        = SON_PNULL;


	SON_UT_TRACE_ENTER();

	/* Pci level search */
    	/*SPR 18360 Fix Start*/
    	p_pci_node = list_find_node(p_list, (son_void_t *)(p_key),
        	anr_key_of_eutran_cdma_pci_value_list,
            	anr_compare_eutran_cdma_pci_value_list_node);
    	/*SPR 18360 Fix Stop*/
	if (p_pci_node != SON_PNULL)
	{
		p_pci_list_node = YMEMBEROF(anr_eutran_cdma_pci_value_node_t,
				anchor, p_pci_node);
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"PCI %d not found in the list", p_key->key.cdma_key.pci);
	}

	SON_UT_TRACE_EXIT();
	return p_pci_list_node;
} /* anr_find_record_in_eutran_cdma_pci_value_list */

/*****************************************************************************
 * Function Name  : anr_key_of_eutran_cdma_pci_value_list
 * Inputs         : p_node -  Anchor to eutran or cdma pci value node
 * Outputs        : None
 * Returns        : son_void_t * - void type pointer to pci value node
 * Description    : This function generates the key
 ******************************************************************************/
const son_void_t *
anr_key_of_eutran_cdma_pci_value_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_eutran_cdma_pci_value_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_eutran_cdma_pci_value_node_t,
			anchor, p_node);

    	/*SPR 18360 Fix Start*/
    	return (son_void_t *)(&p_data->eutran_cdma_pci_value.key);
    	/*SPR 18360 Fix Stop*/
}

/*****************************************************************************
 * Function Name  : anr_compare_eutran_cdma_pci_value_list_node
 * Inputs         : p_val_1  - Pointer to the key field(pci)as void pointer
 *                : p_val_2  - Pointer to the key field(pci)as void pointer
 * Outputs        : None
 * Returns        : son_s32  - 0 If matches
 *                             Non zero if not matches
 * Description    : This function compares the key field(pci) of the
 *                  pci value node in the list
 ******************************************************************************/
son_s32
anr_compare_eutran_cdma_pci_value_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	/*SPR 18360 Fix Start*/
	/* Lines deleted */
	/*SPR 18360 Fix Stop*/

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointers passed are  NULL");
		SON_UT_TRACE_EXIT();
		return ret_value;
	}
	else
	{
        	/*SPR 18360 Fix Start*/
		/* Lines deleted */
        	return SON_MEMCMP(p_val_1,p_val_2,sizeof(anr_eutran_cdma_key_t));
        	/*SPR 18360 Fix Stop*/
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_eutran_cdma_pci_value_list_node */

/*****************************************************************************
 * Function Name  : anr_find_record_in_ue_index_and_counter_list
 * Inputs         : p_list  - Pointer to the list of ue_index and counter values
 *                  ue_index     - UE Index value
 * Outputs        : None
 * Returns        : ue_index_and_counter_list_node_t * - Pointer to the ue_index
 *                                                        and counter node
 * Description    : This function finds the node for the
 *                  given ue_index value
 *****************************************************************************/
ue_index_and_counter_list_node_t *
anr_find_record_in_ue_index_and_counter_list
(
 SON_LIST                     *p_list,
 son_u16                        ue_index
 )
{
	SON_LIST_NODE                      *p_ue_index_counter_node             = SON_PNULL;
	ue_index_and_counter_list_node_t  *p_ue_index_counter_list_node        = SON_PNULL;


	SON_UT_TRACE_ENTER();

	/* ue_index search */
	p_ue_index_counter_node = list_find_node(p_list, (son_void_t *)(&ue_index),
			anr_key_of_ue_index_and_counter_list,
			anr_compare_ue_index_and_counter_list_node);

	if (p_ue_index_counter_node != SON_PNULL)
	{
		p_ue_index_counter_list_node = YMEMBEROF(ue_index_and_counter_list_node_t,
				anchor, p_ue_index_counter_node);
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"[UE:%u] not found in the list", ue_index);
	}

	SON_UT_TRACE_EXIT();
	return p_ue_index_counter_list_node;
} /* anr_find_record_in_ue_index_and_counter_list */

/*****************************************************************************
 * Function Name  : anr_key_of_ue_index_and_counter_list
 * Inputs         : p_node -  Anchor to ue_index and counter value node
 * Outputs        : None
 * Returns        : son_void_t * - void type pointer to ue_index and counter value node
 * Description    : This function generates the key
 ******************************************************************************/
const son_void_t *
anr_key_of_ue_index_and_counter_list
(
 const SON_LIST_NODE *p_node
 )
{
	ue_index_and_counter_list_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(ue_index_and_counter_list_node_t,
			anchor, p_node);

	return (son_void_t *)(&p_data->ue_index_and_counter_values);
}

/*****************************************************************************
 * Function Name  : anr_compare_ue_index_and_counter_list_node
 * Inputs         : p_val_1  - Pointer to the key field(ue_index)as void pointer
 *                : p_val_2  - Pointer to the key field(ue_index)as void pointer
 * Outputs        : None
 * Returns        : son_s32  - 0 If matches
 *                             Non zero if not matches
 * Description    : This function compares the key field(ue_index) of the
 *                  ue_index and counter value node in the list
 ******************************************************************************/
son_s32
anr_compare_ue_index_and_counter_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	son_u16 *p_data_1 = SON_PNULL;
	son_u16 *p_data_2 = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointers passed are  NULL");
		SON_UT_TRACE_EXIT();
		return ret_value;
	}
	else
	{
		p_data_1 = (son_u16 *)p_val_1;
		p_data_2 = (son_u16 *)p_val_2;

		if ((*p_data_1 == *p_data_2))
		{
			ret_value = 0;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_ue_index_and_counter_list_node */

/*****************************************************************************
 * Function Name  : anr_delete_ue_index_and_counter_list
 * Inputs         : p_list - ue_index_and_counter list
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : Delete the nodes of the ue_index_and_counter list
 ****************************************************************************/

son_void_t
anr_delete_ue_index_and_counter_list
(
 SON_LIST  *p_list
 )
{
	SON_LIST_NODE            *p_ue_index_counter_node         = SON_PNULL;
	SON_LIST_NODE           *p_next_ue_index_counter_node     = SON_PNULL;
	ue_index_and_counter_list_node_t *p_ue_index_counter_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_ue_index_counter_node = get_first_list_node(p_list);

	while (SON_PNULL != p_ue_index_counter_node)
	{
		p_ue_index_counter_list_node =  YMEMBEROF(ue_index_and_counter_list_node_t,
				anchor, p_ue_index_counter_node);
		p_next_ue_index_counter_node = get_next_list_node(p_ue_index_counter_node);
		list_delete_node(p_list, &p_ue_index_counter_list_node->anchor);
		son_mem_free(p_ue_index_counter_list_node);
		p_ue_index_counter_node = p_next_ue_index_counter_node;
	}
	SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
			SON_BRIEF,
			"ue_index_and_counter list count %d",
			list_count(p_list));

	SON_UT_TRACE_EXIT();
} /* anr_delete_ue_index_and_counter_list */

/*****************************************************************************
 * Function Name  : anr_delete_pci_from_eutran_cdma_pci_requested_cgi_list
 * Inputs         : p_list  - Pointer to the list of pci values of eutran or
 *                            cdma cells
 *                  pci     - pci value
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function deletes the pci node from 
 *                  eutran_pci_requested_cgi_list or cdma_pci_requested_cgi_list
 *****************************************************************************/
son_void_t
anr_delete_pci_from_eutran_cdma_pci_requested_cgi_list
(
 SON_LIST                     *p_list,
 /*SPR 18360 Fix Start*/
 /*Lines Deleted*/
 anr_eutran_cdma_key_t        *p_key
 /*SPR 18360 Fix Stop*/
 )
{
	anr_eutran_cdma_pci_value_node_t       *p_pci_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	/*Finding the PCI value in EUTRAN PCI value list*/
    	/*SPR 18360 Fix Start*/
    	p_pci_list_node = anr_find_record_in_eutran_cdma_pci_value_list(p_list,p_key);
    	/*SPR 18360 Fix Stop*/

	/*If PCI is present*/
	if(p_pci_list_node != SON_PNULL)
	{
		anr_delete_ue_index_and_counter_list(&p_pci_list_node->eutran_cdma_pci_value.ue_index_and_counter_list);
		list_delete_node(p_list, &p_pci_list_node->anchor);
		son_mem_free(p_pci_list_node);
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_find_record_in_utran_pci_value_list
 * Inputs         : p_list  - Pointer to the list of pci values of utran cells
 *                  pci     - pci value
 * Outputs        : None
 * Returns        : anr_utran_pci_value_node_t * - Pointer to pci value node
 * Description    : This function finds the node for the
 *                  given pci value
 *****************************************************************************/
anr_utran_pci_value_node_t *
anr_find_record_in_utran_pci_value_list
(
 SON_LIST                            *p_list,
 /*SPR 18360 Fix Start*/
 /*Lines Deleted*/
 anr_utran_key_t                     *p_key
 /*SPR 18360 Fix Stop*/
 )
{
	SON_LIST_NODE                *p_pci_node             = SON_PNULL;
	anr_utran_pci_value_node_t  *p_pci_list_node        = SON_PNULL;


	SON_UT_TRACE_ENTER();

	/* Pci level search */
    	/*SPR 18360 Fix Start*/
    	p_pci_node = list_find_node(p_list, (son_void_t *)(p_key),
            	anr_key_of_utran_pci_value_list,
            	anr_compare_utran_pci_value_list_node);
    	/*SPR 18360 Fix Start*/
	if (p_pci_node != SON_PNULL)
	{
		p_pci_list_node = YMEMBEROF(anr_utran_pci_value_node_t,
				anchor, p_pci_node);
	}
	else
	{
        if (RRM_PCI_FDD_PRESENT & p_key->pci.bitmask)
        {
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                    SON_WARNING,
                    "UTRAN FDD PCI %u EARFCN %u not found in the list", p_key->pci.pci_fdd, p_key->uarfcn);
        }
        else if (RRM_PCI_TDD_PRESENT & p_key->pci.bitmask)
        {
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                    SON_WARNING,
                    "UTRAN TDD PCI %d EARFCN %u not found in the list", p_key->pci.pci_tdd, p_key->uarfcn);
        }
        else
        {
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                    SON_WARNING,
                    "UTRAN EARFCN %u bitmask not set for PCI", p_key->uarfcn);
        }

	}

	SON_UT_TRACE_EXIT();
	return p_pci_list_node;
} /* anr_find_record_in_utran_pci_value_list */

/*****************************************************************************
 * Function Name  : anr_key_of_utran_pci_value_list
 * Inputs         : p_node -  Anchor to utran pci value node
 * Outputs        : None
 * Returns        : son_void_t * - void type pointer to pci value node
 * Description    : This function generates the key
 ******************************************************************************/
const son_void_t *
anr_key_of_utran_pci_value_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_utran_pci_value_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_utran_pci_value_node_t,
			anchor, p_node);

    	/*SPR 18360 Fix Start*/
    	return (son_void_t *)(&p_data->utran_pci_value.key);
   	/*SPR 18360 Fix Stop*/
}

/*****************************************************************************
 * Function Name  : anr_compare_utran_pci_value_list_node
 * Inputs         : p_val_1  - Pointer to the key field(pci)as void pointer
 *                : p_val_2  - Pointer to the key field(pci)as void pointer
 * Outputs        : None
 * Returns        : son_s32  - 0 If matches
 *                             Non zero if not matches
 * Description    : This function compares the key field(pci) of the
 *                  pci value node in the list
 ******************************************************************************/
son_s32
anr_compare_utran_pci_value_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointers passed are  NULL");
		SON_UT_TRACE_EXIT();
		return ret_value;
	}
	else
	{
        /*SPR 18360 Fix Start*/
        ret_value = SON_MEMCMP(p_val_1, p_val_2, sizeof(anr_utran_key_t));
        /*SPR 18360 Fix Stop*/
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_utran_pci_value_list_node */

/*****************************************************************************
 * Function Name  : anr_delete_pci_from_utran_pci_requested_cgi_list
 * Inputs         : p_list  - Pointer to the list of pci values of utran cells
 *                  pci     - pci value
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function deletes the pci node from 
 *                  utran_pci_requested_cgi_list
 *****************************************************************************/
son_void_t
anr_delete_pci_from_utran_pci_requested_cgi_list
(
 SON_LIST                            *p_list,
 rrm_son_utran_physical_cell_id_t     pci,
 /*SPR 18360 Fix Start*/
 son_u16                              uarfcn
 /*SPR 18360 Fix Stop*/
 )
{
	anr_utran_pci_value_node_t       *p_pci_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

    	/*SPR 18360 Fix Start*/
    	anr_utran_key_t  key;

    	SON_UT_TRACE_ENTER();

    	SON_MEMSET(&key,0,sizeof(anr_utran_key_t));
    	key.pci = pci;
    	key.uarfcn = uarfcn;

    	/*Finding the PCI value in UTRAN PCI value list*/
    	p_pci_list_node = anr_find_record_in_utran_pci_value_list(p_list,&key);
    	/*SPR 18360 Fix Stop*/


	/*If PCI is present*/
	if(p_pci_list_node != SON_PNULL)
	{
		anr_delete_ue_index_and_counter_list(&p_pci_list_node->utran_pci_value.ue_index_and_counter_list);
		list_delete_node(p_list, &p_pci_list_node->anchor);
		son_mem_free(p_pci_list_node);
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_find_record_in_geran_pci_value_list
 * Inputs         : p_list  - Pointer to the list of pci values of geran cells
 *                  pci     - pci value
 * Outputs        : None
 * Returns        : anr_geran_pci_value_node_t * - Pointer to pci value node
 * Description    : This function finds the node for the
 *                  given pci value
 *****************************************************************************/
anr_geran_pci_value_node_t *
anr_find_record_in_geran_pci_value_list
(
 SON_LIST                            *p_list,
 /* Spr 18615 Changes Start */ 
 anr_geran_key_t                    *p_key
 /* Spr 18615 Changes End */
 )
{
	SON_LIST_NODE                *p_pci_node             = SON_PNULL;
	anr_geran_pci_value_node_t  *p_pci_list_node        = SON_PNULL;


	SON_UT_TRACE_ENTER();

	/* Pci level search */
    /* Spr 18615 Changes Start */ 
    p_pci_node = list_find_node(p_list, (son_void_t *)(p_key),
    /* Spr 18615 Changes End */
			anr_key_of_geran_pci_value_list,
			anr_compare_geran_pci_value_list_node);

	if (p_pci_node != SON_PNULL)
	{
		p_pci_list_node = YMEMBEROF(anr_geran_pci_value_node_t,
				anchor, p_pci_node);
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"GERAN PCI [NCC:%u BSCC:%u] EARFCN %u not found in the list", p_key->pci.ncc, p_key->pci.bscc, p_key->arfcn);
	}

	SON_UT_TRACE_EXIT();
	return p_pci_list_node;
} /* anr_find_record_in_geran_pci_value_list */

/*****************************************************************************
 * Function Name  : anr_key_of_geran_pci_value_list
 * Inputs         : p_node -  Anchor to geran pci value node
 * Outputs        : None
 * Returns        : son_void_t * - void type pointer to pci value node
 * Description    : This function generates the key
 ******************************************************************************/
const son_void_t *
anr_key_of_geran_pci_value_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_geran_pci_value_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_geran_pci_value_node_t,
			anchor, p_node);
    /* Spr 18615 Changes Start */
    return (son_void_t *)(&p_data->geran_pci_value.key);
    /* Spr 18615 Changes End */
}

/*****************************************************************************
 * Function Name  : anr_compare_geran_pci_value_list_node
 * Inputs         : p_val_1  - Pointer to the key field(pci)as void pointer
 *                : p_val_2  - Pointer to the key field(pci)as void pointer
 * Outputs        : None
 * Returns        : son_s32  - 0 If matches
 *                             Non zero if not matches
 * Description    : This function compares the key field(pci) of the
 *                  pci value node in the list
 ******************************************************************************/
son_s32
anr_compare_geran_pci_value_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointers passed are  NULL");
		SON_UT_TRACE_EXIT();
		return ret_value;
	}
	else
	{
        /* Spr 18615 Changes Start */
        ret_value = SON_MEMCMP(p_val_1, p_val_2, sizeof(anr_geran_key_t));
        /* Spr 18615 Changes End */
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_utran_pci_value_list_node */

/*****************************************************************************
 * Function Name  : anr_delete_pci_from_geran_pci_requested_cgi_list
 * Inputs         : p_list  - Pointer to the list of pci values of geran cells
 *                  pci     - pci value
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function deletes the pci node from 
 *                  geran_pci_requested_cgi_list
 *****************************************************************************/
son_void_t
anr_delete_pci_from_geran_pci_requested_cgi_list
(
 SON_LIST                            *p_list,
    /* Spr 18615 Changes Start */
    anr_geran_key_t                    *p_key
    /* Spr 18615 Changes End */
)
{
	anr_geran_pci_value_node_t       *p_pci_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

    /*Finding the PCI value in UTRAN PCI value list*/
    /* Spr 18615 Changes Start */
    p_pci_list_node = anr_find_record_in_geran_pci_value_list(p_list, p_key);
    /* Spr 18615 Changes End */

	/*If PCI is present*/
	if(p_pci_list_node != SON_PNULL)
	{
		anr_delete_ue_index_and_counter_list(&p_pci_list_node->geran_pci_value.ue_index_and_counter_list);
		list_delete_node(p_list, &p_pci_list_node->anchor);
		son_mem_free(p_pci_list_node);
	}

	SON_UT_TRACE_EXIT();
}

/*SPR-9759-End*/

/*****************************************************************************
 * Function Name  : anr_key_of_cdma_hrpd_band_stats_list
 * Inputs         : p_node -  Anchor to CDMA2000 HRPD band stats node
 * Outputs        : None
 * Returns        : son_void_t * - void type pointer to CDMA2000 band stats node
 * Description    : This function generates the key using band class for hash
 *                  record of CDMA2000 HRPD
 ******************************************************************************/
const son_void_t *
anr_key_of_cdma_hrpd_band_stats_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_cdma_hrpd_band_stats_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_cdma_hrpd_band_stats_node_t,
			anchor, p_node);

	return (son_void_t *)(&p_data->cdma_band_stats);
}

/*****************************************************************************
 * Function Name  : anr_key_of_cdma_xrtt_band_stats_list
 * Inputs         : p_node        - Anchor to CDMA2000 XRTT band stats node
 * Outputs        : None
 * Returns        : son_void_t *  - void type pointer to CDMA2000 band node found
 * Description    : This function generates the key using  band class for
 *                  hash record of CDMA XRTT
 ******************************************************************************/
const son_void_t *
anr_key_of_cdma_xrtt_band_stats_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_cdma_xrtt_band_stats_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_cdma_xrtt_band_stats_node_t,
			anchor, p_node);

	return (son_void_t *)(&p_data->cdma_band_stats);
}

/*****************************************************************************
 * Function Name  : anr_key_of_cdma_hrpd_freq_stats_list
 * Inputs         : p_node -  Anchor to CDMA2000 HRPD band stats node
 * Outputs        : None
 * Returns        : son_void_t *  - void type pointer to CDMA2000 band node found
 * Description    : This function generates the key using arfcn for
 *                  hash record of CDMA HRPD
 ******************************************************************************/
const son_void_t *
anr_key_of_cdma_hrpd_freq_stats_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_cdma_hrpd_freq_stats_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_cdma_hrpd_freq_stats_node_t,
			anchor, p_node);

	return (son_void_t *)(&p_data->arfcn_stats);
}

/*****************************************************************************
 * Function Name  : anr_key_of_cdma_xrtt_freq_stats_list
 * Inputs         : p_node       - Anchor to CDMA2000 HRPD band stats node
 * Outputs        : None
 * Returns        : son_void_t * - void type pointer to CDMA2000 band node found
 * Description    : This function generates the key using arfcn for
 *                  hash record of CDMA XRTT
 ******************************************************************************/
const son_void_t *
anr_key_of_cdma_xrtt_freq_stats_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_cdma_xrtt_freq_stats_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_cdma_xrtt_freq_stats_node_t,
			anchor, p_node);

	return (son_void_t *)(&p_data->arfcn_stats);
}

/*****************************************************************************
 * Function Name  : anr_compare_cdma_hrpd_band_stats_list_node
 * Inputs         : p_val_1  - Pointer to the key field(earfcn)as void pointer
 *                : p_val_2  - Pointer to the key field(earfcn)as void pointer
 * Outputs        : None
 * Returns        : son_s32  - 0 If matches
 *                             1 If not matches
 * Description    : This function compares the key field(earfcn) of the
 *                  broadcast statistics node in the list
 ******************************************************************************/
son_s32
anr_compare_cdma_hrpd_band_stats_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	son_u16 *p_data_1 = SON_PNULL;
	son_u16 *p_data_2 = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointers passed are  NULL");
		SON_UT_TRACE_EXIT();
		return ret_value;
	}
	else
	{
		p_data_1 = (son_u16 *)p_val_1;
		p_data_2 = (son_u16 *)p_val_2;

		if ((*p_data_1 == *p_data_2))
		{
			ret_value = 0;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_cdma_hrpd_band_stats_list_node */

/*****************************************************************************
 * Function Name  : anr_compare_cdma_xrtt_band_stats_list_node
 * Inputs         : p_val_1 - Pointer to the key field(earfcn)as void pointer
 *                : p_val_2 - Pointer to the key field(earfcn)as void pointer
 * Outputs        : None
 * Returns        : son_s32 - 0  If matches
 *                            Non Zero If not matches
 * Description    : This function compares the key field(earfcn) of the
 *                  broadcast statistics node in the list
 ******************************************************************************/
son_s32
anr_compare_cdma_xrtt_band_stats_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	son_u16 *p_data_1 = SON_PNULL;
	son_u16 *p_data_2 = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointers passed are  NULL");
		SON_UT_TRACE_EXIT();
		return ret_value;
	}
	else
	{
		p_data_1 = (son_u16 *)p_val_1;
		p_data_2 = (son_u16 *)p_val_2;

		if ((*p_data_1 == *p_data_2))
		{
			ret_value = 0;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_cdma_xrtt_band_stats_list_node */

/*****************************************************************************
 * Function Name  : anr_compare_cdma_hrpd_freq_stats_list_node
 * Inputs         : p_val_1 - Pointer to the key field(earfcn)as void pointer
 *                : p_val_2 - Pointer to the key field(earfcn)as void pointer
 * Outputs        : None
 * Returns        : son_s32 - 0 If matches
 *                            Non Zero If not matches
 * Description    : This function compares the key field(earfcn) of the
 *                  broadcast statistics node in the list
 ******************************************************************************/
son_s32
anr_compare_cdma_hrpd_freq_stats_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	son_u16 *p_data_1 = SON_PNULL;
	son_u16 *p_data_2 = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointers passed are  NULL");
		SON_UT_TRACE_EXIT();
		return ret_value;
	}
	else
	{
		p_data_1 = (son_u16 *)p_val_1;
		p_data_2 = (son_u16 *)p_val_2;

		if ((*p_data_1 == *p_data_2))
		{
			ret_value = 0;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_cdma_hrpd_freq_stats_list_node */

/*****************************************************************************
 * Function Name  : anr_compare_cdma_xrtt_freq_stats_list_node
 * Inputs         : p_val_1  - Pointer to the key field(earfcn)as void pointer
 *                : p_val_2  - Pointer to the key field(earfcn)as void pointer
 * Outputs        : None
 * Returns        : son_s32  - 0 If matches
 *                             Non Zero If not matches
 * Description    : This function compares the key field(earfcn) of the
 *                  broadcast statistics node in the list
 ******************************************************************************/
son_s32
anr_compare_cdma_xrtt_freq_stats_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	son_u16 *p_data_1 = SON_PNULL;
	son_u16 *p_data_2 = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointers passed are  NULL");
		SON_UT_TRACE_EXIT();
		return ret_value;
	}
	else
	{
		p_data_1 = (son_u16 *)p_val_1;
		p_data_2 = (son_u16 *)p_val_2;

		if ((*p_data_1 == *p_data_2))
		{
			ret_value = 0;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_cdma_xrtt_freq_stats_list_node */

/*****************************************************************************
 * Function Name  : anr_find_record_in_cdma_hrpd_freq_stats_list
 * Inputs         : p_list  - Pointer to frequency statistics List
 *                  arfcn   - ARFCN value
 * Outputs        : None
 * Returns        : anr_cdma_hrpd_freq_stats_node_t * - Pointer to cdma hrpd
 *                                                      freq stats node
 * Description    : This function finds the frequency statistcs node for the
 *                  given arfcn value
 *****************************************************************************/
anr_cdma_hrpd_freq_stats_node_t *
anr_find_record_in_cdma_hrpd_freq_stats_list
(
 SON_LIST                     *p_list,
 son_u16 arfcn
 )
{
	SON_LIST_NODE                *p_freq_node             = SON_PNULL;
	anr_cdma_hrpd_freq_stats_node_t  *p_freq_list_node         = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_freq_node = list_find_node(p_list,
			(son_void_t *)(&arfcn),
			anr_key_of_cdma_hrpd_freq_stats_list,
			anr_compare_cdma_hrpd_freq_stats_list_node);

	if (p_freq_node != SON_PNULL)
	{
		p_freq_list_node = YMEMBEROF(anr_cdma_hrpd_freq_stats_node_t,
				anchor, p_freq_node);
	}

	SON_UT_TRACE_EXIT();
	return p_freq_list_node;
} /* anr_find_record_in_cdma_hrpd_freq_stats_list */

/*****************************************************************************
 * Function Name  : anr_find_record_in_cdma_hrpd_band_stats_list
 * Inputs         : p_list  - Pointer to broadcast allowed neighbor
 *                            statistics List
 *                  band    - band class
 * Outputs        : None
 * Returns        : anr_cdma_hrpd_band_stats_node_t * - Pointer to cdma hrpd band
 *                                                      stats node
 * Description    : This function finds the broadcast statistcs node for the
 *                  given band class
 *****************************************************************************/
anr_cdma_hrpd_band_stats_node_t *
anr_find_record_in_cdma_hrpd_band_stats_list
(
 SON_LIST                     *p_list,
 son_cdma_band_class_et band
 )
{
	SON_LIST_NODE                *p_band_node             = SON_PNULL;
	anr_cdma_hrpd_band_stats_node_t *p_band_list_node        = SON_PNULL;


	SON_UT_TRACE_ENTER();

	/* Band level search */
	p_band_node = list_find_node(p_list, (son_void_t *)(&band),
			anr_key_of_cdma_hrpd_band_stats_list,
			anr_compare_cdma_hrpd_band_stats_list_node);

	if (p_band_node != SON_PNULL)
	{
		p_band_list_node = YMEMBEROF(anr_cdma_hrpd_band_stats_node_t,
				anchor, p_band_node);
	}

	SON_UT_TRACE_EXIT();
	return p_band_list_node;
} /* anr_find_record_in_cdma_hrpd_band_stats_list */

/*****************************************************************************
 * Function Name  : anr_insert_record_in_cdma_hrpd_band_stats_list
 * Inputs         : p_cell_ctxt - Pointer to the Cell context
 *                  p_nbr_info  - Pointer to CDMA2000 neighbor info
 * Outputs        : None
 * Returns        : anr_cdma_hrpd_band_stats_node_t * - Pointer to cdma hrpd band
 *                                                      node inserted in list
 * Description    : This function inserts a CDMA statistics record in the
 *                   List
 *****************************************************************************/
anr_cdma_hrpd_band_stats_node_t *
anr_insert_record_in_cdma_hrpd_band_stats_list
(
 anr_cell_context_t         *p_cell_ctxt,
 son_cdma_neighbor_info_t   *p_nbr_info
 )
{
	anr_cdma_hrpd_band_stats_node_t  *p_band_list_node      = SON_PNULL;
	anr_cdma_hrpd_freq_stats_node_t  *p_freq_list_node      = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_nbr_info == SON_PNULL)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure for freq list");
		SON_UT_TRACE_EXIT();
		return p_band_list_node;
	}

	/* Finding the band class */
	p_band_list_node = anr_find_record_in_cdma_hrpd_band_stats_list(
			&p_cell_ctxt->cdma_hrpd_band_stats_list,
			p_nbr_info->band_class);

	/* If band is present */
	if (p_band_list_node != SON_PNULL)
	{
		/* Getting the memory for frequency */
		p_freq_list_node = (anr_cdma_hrpd_freq_stats_node_t *)son_mem_get
			(sizeof(anr_cdma_hrpd_freq_stats_node_t));

		if (SON_PNULL != p_freq_list_node)
		{
			/* Coverity Fix: 30297 */
			SON_MEMSET(p_freq_list_node, 0x00, sizeof(anr_cdma_hrpd_freq_stats_node_t));

			/* Inserting the frequency */
			p_freq_list_node->arfcn_stats.arfcn = p_nbr_info->arfcn;
			list_insert_node(&p_band_list_node->cdma_band_stats.freq_list, &p_freq_list_node->anchor);

			/* Incrementing the count */
			increment_cdma_hrpd_nbr_count_for_rrm(
					p_nbr_info,
					p_cell_ctxt);
		}
		else
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_ERROR,
					"Memory allocation failure for freq list");
		}
	}
	/* If band class is not present */
	else
    {
        /* Getting the memory for band class*/
        p_band_list_node  = (anr_cdma_hrpd_band_stats_node_t *)son_mem_get
            (sizeof(anr_cdma_hrpd_band_stats_node_t));

        if (SON_PNULL != p_band_list_node)
        {
            /* Coverity Fix: 30297 */
            SON_MEMSET(p_band_list_node, 0x00, sizeof(anr_cdma_hrpd_band_stats_node_t));

            /* SPR 15935 Fix Start */
            /*Lines Deleted*/
            /* SPR 15935 Fix Stop */

            /* Getting the memory for frequency */
            p_freq_list_node = (anr_cdma_hrpd_freq_stats_node_t *)son_mem_get
                (sizeof(anr_cdma_hrpd_freq_stats_node_t));

            if (SON_PNULL != p_freq_list_node)
            {
                SON_MEMSET(p_freq_list_node, 0x00, sizeof(anr_cdma_hrpd_freq_stats_node_t));

                /* SPR 15935 Fix Start */
                /* Inserting the band class */
                p_band_list_node->cdma_band_stats.band_class = p_nbr_info->band_class;
                list_insert_node(&p_cell_ctxt->cdma_hrpd_band_stats_list, &p_band_list_node->anchor);
                /* SPR 15935 Fix Stop */

                /* Inserting the frequency */
                p_freq_list_node->arfcn_stats.arfcn = p_nbr_info->arfcn;
                list_insert_node(&p_band_list_node->cdma_band_stats.freq_list, &p_freq_list_node->anchor);

                /* Incrementing the count */
                increment_cdma_hrpd_nbr_count_for_rrm(
                        p_nbr_info,
                        p_cell_ctxt);
            }
            else
            {
                son_mem_free(p_band_list_node);
                /* Coverity ID 83379 Starts */
                p_band_list_node=SON_PNULL;
                /* Coverity ID 83379 Ends */

                SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                        SON_ERROR,
                        "Memory allocation failure for freq list");
                /*Coverity 83379 Fix Start*/
                return SON_PNULL;
                /*Coverity 83379 Fix End*/
            }
        }
        else
        {
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                    SON_ERROR,
                    "Memory allocation failure for band list");
        }
    }

	SON_UT_TRACE_EXIT();
	return p_band_list_node;
} /* anr_insert_record_in_cdma_hrpd_band_stats_list */

/*****************************************************************************
 * Function Name  : anr_insert_record_in_cdma_xrtt_band_stats_list
 * Inputs         : p_cell_ctxt - Pointer to the Cell context
 *                  p_nbr_info  - Pointer to CDMA neighbor info
 * Outputs        : None
 * Returns        : anr_cdma_xrtt_band_stats_node_t * - Pointer to cdma xrtt band
 *                                                      node inserted in list
 * Description    : This function inserts a CDMA statistics record in the
 *                  cdma band stats list
 ****************************************************************************/
anr_cdma_xrtt_band_stats_node_t *
anr_insert_record_in_cdma_xrtt_band_stats_list
(
 anr_cell_context_t             *p_cell_ctxt,
 son_cdma_neighbor_info_t       *p_nbr_info
 )
{
	anr_cdma_xrtt_band_stats_node_t  *p_band_list_node      = SON_PNULL;
	anr_cdma_xrtt_freq_stats_node_t  *p_freq_list_node      = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_nbr_info == SON_PNULL)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure for freq list");
		SON_UT_TRACE_EXIT();
		return p_band_list_node;
	}

	/* Finding the band class */
	p_band_list_node = anr_find_record_in_cdma_xrtt_band_stats_list(
			&p_cell_ctxt->cdma_xrtt_band_stats_list,
			p_nbr_info->band_class);

	/* If band is present */
	if (p_band_list_node != SON_PNULL)
	{
		/* Getting the memory for frequency */
		p_freq_list_node = (anr_cdma_xrtt_freq_stats_node_t *)son_mem_get
			(sizeof(anr_cdma_xrtt_freq_stats_node_t));

		if (SON_PNULL != p_freq_list_node)
		{
			/* Coverity Fix: 30298 */
			SON_MEMSET(p_freq_list_node, 0x00, sizeof(anr_cdma_xrtt_freq_stats_node_t));

			/* Inserting the frequency */
			p_freq_list_node->arfcn_stats.arfcn = p_nbr_info->arfcn;
			list_insert_node(&p_band_list_node->cdma_band_stats.freq_list, &p_freq_list_node->anchor);

			/* Incrementing the count */
			increment_cdma_xrtt_nbr_count_for_rrm(
					p_nbr_info,
					p_cell_ctxt);
		}
		else
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_ERROR,
					"Memory allocation failure for freq list");
		}
	}
	/* If band class is not present */
	else
    {
        /* Getting the memory for band class*/
        p_band_list_node  = (anr_cdma_xrtt_band_stats_node_t *)son_mem_get
            (sizeof(anr_cdma_xrtt_band_stats_node_t));

        if (SON_PNULL != p_band_list_node)
        {
            /* Coverity Fix: 30298 */
            SON_MEMSET(p_band_list_node, 0x00, sizeof(anr_cdma_xrtt_band_stats_node_t));
            /* SPR 15935 Fix Start */
            /*Lines Deleted*/
            /* SPR 15935 Fix Stop */
            /* Getting the memory for frequency */
            p_freq_list_node = (anr_cdma_xrtt_freq_stats_node_t *)son_mem_get
                (sizeof(anr_cdma_xrtt_freq_stats_node_t));

            if (SON_PNULL != p_freq_list_node)
            {
                SON_MEMSET(p_freq_list_node, 0x00, sizeof(anr_cdma_xrtt_freq_stats_node_t));

                /* SPR 15935 Fix Start */
                /* Inserting the band class */
                p_band_list_node->cdma_band_stats.band_class = p_nbr_info->band_class;
                list_insert_node(&p_cell_ctxt->cdma_xrtt_band_stats_list, &p_band_list_node->anchor);
                /* SPR 15935 Fix Stop */

                /* Inserting the frequency */
                p_freq_list_node->arfcn_stats.arfcn = p_nbr_info->arfcn;
                list_insert_node(&p_band_list_node->cdma_band_stats.freq_list, &p_freq_list_node->anchor);

                /* Incrementing the count */
                increment_cdma_xrtt_nbr_count_for_rrm(
                        p_nbr_info,
                        p_cell_ctxt);
            }
            else
            {
                son_mem_free(p_band_list_node);
                /* Coverity ID 83377 Starts */
                p_band_list_node=SON_PNULL;
                /* Coverity ID 83377 Starts */

                SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                        SON_ERROR,
                        "Memory allocation failure for freq list");
                /*Coverity 83377 Fix Start*/
                return SON_PNULL;
                /*Coverity 83377 Fix End*/
            }
        }
        else
        {
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                    SON_ERROR,
                    "Memory allocation failure for band list");
        }
    }

	SON_UT_TRACE_EXIT();
	return p_band_list_node;
} /* anr_insert_record_in_cdma_xrtt_band_stats_list */

/*****************************************************************************
 * Function Name  : anr_increment_nbr_count_hrpd
 * Inputs         : p_list           - Pointer to CDMA HRPD band stats list
 *                : band             - CDMA band class
 *                : arfcn            - Arfcn
 *                : broadcast_status - Broadcast status
 *                : ho_status        - Handover status
 * Outputs        : None
 * Returns        : son_void_t       - None
 * Description    : This function increments the nbr count of CDMA HRPD nbr
 *****************************************************************************/
son_bool_et
anr_increment_nbr_count_hrpd
(
 SON_LIST                     *p_list,
 son_cdma_band_class_et band,
 son_u16 arfcn,
 son_bool_et broadcast_status,
 son_ho_status_et ho_status
 )
{
	anr_cdma_hrpd_band_stats_node_t *p_band_list_node   = SON_PNULL;
	anr_cdma_hrpd_freq_stats_node_t *p_freq_list_node   = SON_PNULL;
	son_bool_et ret_val             = SON_FALSE;


	SON_UT_TRACE_ENTER();

	/* Finding the band in the band stats list */
	p_band_list_node = anr_find_record_in_cdma_hrpd_band_stats_list(
			p_list, band);

	if (SON_PNULL != p_band_list_node)
	{
		/* Finding the frequency in the frequency list */
		p_freq_list_node = anr_find_record_in_cdma_hrpd_freq_stats_list(
				&p_band_list_node->cdma_band_stats.freq_list,
				arfcn);

		if (SON_PNULL != p_freq_list_node)
		{
			switch (broadcast_status)
			{
				case SON_TRUE:
					{
						if ((ho_status == SON_HO_ALLOWED) &&
								(anr_get_no_of_hrpd_nbrs_per_frequency(p_freq_list_node) <
								 SON_MAX_NO_CDMA_HRPD_CELLS_PER_EARFCN))
						{
							p_freq_list_node->arfcn_stats.num_bs_true_ho_allwd++;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA hrpd NBR count incremented, "
									"for band %d, arfcn %d, num_bs_true_ho_allwd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_true_ho_allwd);
						}
						else if ((ho_status == SON_HO_UNDEFINED) &&
								(anr_get_no_of_hrpd_nbrs_per_frequency(p_freq_list_node) <
								 SON_MAX_NO_CDMA_HRPD_CELLS_PER_EARFCN))
						{
							p_freq_list_node->arfcn_stats.num_bs_true_ho_blklstd++;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA hrpd NBR count incremented, "
									"for band %d, arfcn %d, num_bs_true_ho_blklstd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_true_ho_blklstd);
						}
						else
						{
							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_WARNING,
									"MAX NBR count for RRM reached, "
									"for band %d, arfcn %d, broadcast_status: TRUE, ho_status: %d",
									band, arfcn, ho_status);
						}

						break;
					}

				case SON_FALSE:
					{
						if ((ho_status == SON_HO_ALLOWED) &&
								(anr_get_no_of_hrpd_nbrs_per_frequency(p_freq_list_node) <
								 SON_MAX_NO_CDMA_HRPD_CELLS_PER_EARFCN))
						{
							p_freq_list_node->arfcn_stats.num_bs_false_ho_allwd++;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA hrpd NBR count incremented "
									"for band %d arfcn %d num_bs_false_ho_allwd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_false_ho_allwd);
						}
						else if ((ho_status == SON_HO_UNDEFINED) &&
								(anr_get_no_of_hrpd_nbrs_per_frequency(p_freq_list_node) <
								 SON_MAX_NO_CDMA_HRPD_CELLS_PER_EARFCN))
						{
							p_freq_list_node->arfcn_stats.num_bs_false_ho_blklstd++;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA hrpd NBR count incremented "
									"for band %d, arfcn %d, num_bs_false_ho_blklstd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_false_ho_blklstd);
						}
						else
						{
							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_WARNING,
									"MAX NBR count for RRM reached "
									"for band %d, arfcn %d, broadcast_status: FALSE, ho_status: %d",
									band, arfcn, ho_status);
						}

						break;
					}

				default:
					{
						SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_ERROR,
								"Unknown broadcast status = %d",
								broadcast_status);
						break;
					}
			} /* switch */
		}
		else
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_ERROR,
					"No node found for band: %d arfcn: %d",
					band, arfcn);
		}
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"No node found for band: %d",
				band);
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_increment_nbr_count_hrpd */

/****************************************************************************
 * Function Name  : anr_increment_nbr_count_xrtt
 * Inputs         : p_list           - Pointer to XRTT nbr band stats list
 *                : band             - CDMA band class
 *                : arfcn            - Arfcn
 *                : broadcast_status - Broadcast status of CDMA nbr
 *                : ho_status        - Handover status of CDMA nbr
 * Outputs        : None
 * Returns        : son_bool_et      - SON_TRUE / SON_FALSE
 * Description    : This function increments the CDMA XRTT neighbor count
 ****************************************************************************/
son_bool_et
anr_increment_nbr_count_xrtt
(
 SON_LIST                     *p_list,
 son_cdma_band_class_et band,
 son_u16 arfcn,
 son_bool_et broadcast_status,
 son_ho_status_et ho_status
 )
{
	anr_cdma_xrtt_band_stats_node_t *p_band_list_node   = SON_PNULL;
	anr_cdma_xrtt_freq_stats_node_t *p_freq_list_node   = SON_PNULL;
	son_bool_et ret_val             = SON_FALSE;


	SON_UT_TRACE_ENTER();

	/* Finding the band in the band stats list */
	p_band_list_node = anr_find_record_in_cdma_xrtt_band_stats_list(
			p_list, band);

	if (SON_PNULL != p_band_list_node)
	{
		/* Finding the frequency in the frequency list */
		p_freq_list_node = anr_find_record_in_cdma_xrtt_freq_stats_list(
				&p_band_list_node->cdma_band_stats.freq_list,
				arfcn);

		if (SON_PNULL != p_freq_list_node)
		{
			switch (broadcast_status)
			{
				case SON_TRUE:
					{
						if ((ho_status == SON_HO_ALLOWED) &&
								(anr_get_no_of_xrtt_nbrs_per_frequency(p_freq_list_node) <
								 SON_MAX_NO_CDMA_XRTT_CELLS_PER_EARFCN))
						{
							p_freq_list_node->arfcn_stats.num_bs_true_ho_allwd++;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA XRTT NBR count incremented "
									"for band %d arfcn %d num_bs_true_ho_allwd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_true_ho_allwd);
						}
						else if ((ho_status == SON_HO_UNDEFINED) &&
								(anr_get_no_of_xrtt_nbrs_per_frequency(p_freq_list_node) <
								 SON_MAX_NO_CDMA_XRTT_CELLS_PER_EARFCN))
						{
							p_freq_list_node->arfcn_stats.num_bs_true_ho_blklstd++;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA XRTT NBR count successfully incremented, "
									"for band %d, arfcn %d, num_bs_true_ho_blklstd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_true_ho_blklstd);
						}
						else
						{
							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_WARNING,
									"MAX NBR count for RRM reached, "
									"for band %d, arfcn %d, broadcast_status: TRUE, ho_status: %d",
									band, arfcn, ho_status);
						}

						break;
					}

				case SON_FALSE:
					{
						if ((ho_status == SON_HO_ALLOWED) &&
								(anr_get_no_of_xrtt_nbrs_per_frequency(p_freq_list_node) <
								 SON_MAX_NO_CDMA_XRTT_CELLS_PER_EARFCN))
						{
							p_freq_list_node->arfcn_stats.num_bs_false_ho_allwd++;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA XRTT NBR count successfully incremented "
									"for band %d, arfcn %d, num_bs_false_ho_allwd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_false_ho_allwd);
						}
						else if ((ho_status == SON_HO_UNDEFINED) &&
								(anr_get_no_of_xrtt_nbrs_per_frequency(p_freq_list_node) <
								 SON_MAX_NO_CDMA_XRTT_CELLS_PER_EARFCN))
						{
							p_freq_list_node->arfcn_stats.num_bs_false_ho_blklstd++;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA XRTT NBR count incremented, "
									"for band %d arfcn %d num_bs_false_ho_blklstd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_false_ho_blklstd);
						}
						else
						{
							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_WARNING,
									"MAX NBR count for RRM reached "
									"for band %d arfcn %d broadcast_status: FALSE, ho_status: %d",
									band, arfcn, ho_status);
						}

						break;
					}

				default:
					{
						SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_ERROR,
								"Unknown broadcast status = %d",
								broadcast_status);
						break;
					}
			} /* switch */
		}
		else
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_ERROR,
					"No node found for band: %d, arfcn: %d",
					band, arfcn);
		}
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"No node found for band: %d",
				band);
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_increment_nbr_count_xrtt */

/*****************************************************************************
 * Function Name  : increment_cdma_hrpd_nbr_count_for_rrm
 * Inputs         : p_nbr_info  - Pointer to CDMA neighbour information
 *                  p_cell_ctxt - Pointer to serving cell context
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function increment the nbr count of CDMA HRPD nbr
 *****************************************************************************/
son_bool_et
increment_cdma_hrpd_nbr_count_for_rrm
(
 son_cdma_neighbor_info_t   *p_nbr_info,
 anr_cell_context_t         *p_cell_ctxt
 )
{
	son_bool_et ret_val         = SON_FALSE;
	son_ho_status_et ho_status = SON_HO_UNDEFINED;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Invalid data as "
				"p_cell_ctxt is SON_PNULL");

		return ret_val;
	}

	ho_status = anr_get_ho_status_of_cdma_nr(p_nbr_info);

	ret_val = anr_increment_nbr_count_hrpd(
			&p_cell_ctxt->cdma_hrpd_band_stats_list,
			p_nbr_info->band_class,
			p_nbr_info->arfcn,
			p_nbr_info->broadcast_status,
			ho_status);

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* increment_cdma_hrpd_nbr_count_for_rrm */

/****************************************************************************
 * Function Name  : anr_decrement_nbr_count_hrpd
 * Inputs         : p_list              - Pointer to SON LIST HASH Table
 *                : band                - CDMA2000 band class
 *                : arfcn               - Arfcn
 *                : broadcast_status    - CDMA broadcast status
 *                : ho_status           - Handover status
 * Outputs        : None
 * Returns        : son_bool_et         - SON_TRUE / SON_FALSE
 * Description    : This function decrements the CDMA HRPD neighbor count
 ***************************************************************************/
son_bool_et
anr_decrement_nbr_count_hrpd
(
 SON_LIST                     *p_list,
 son_cdma_band_class_et band,
 son_u16 arfcn,
 son_bool_et broadcast_status,
 son_ho_status_et ho_status
 )
{
	anr_cdma_hrpd_band_stats_node_t *p_band_list_node   = SON_PNULL;
	anr_cdma_hrpd_freq_stats_node_t *p_freq_list_node   = SON_PNULL;
	son_bool_et ret_val             = SON_FALSE;
	son_u16 total_nbrs          = 0;


	SON_UT_TRACE_ENTER();

	/* Finding the band in the band stats list */
	p_band_list_node = anr_find_record_in_cdma_hrpd_band_stats_list(
			p_list, band);

	if (SON_PNULL != p_band_list_node)
	{
		/* Finding the frequency in the frequency list */
		p_freq_list_node = anr_find_record_in_cdma_hrpd_freq_stats_list(
				&p_band_list_node->cdma_band_stats.freq_list,
				arfcn);

		if (SON_PNULL != p_freq_list_node)
		{
			switch (broadcast_status)
			{
				case SON_TRUE:
					{
						if ((ho_status == SON_HO_ALLOWED) &&
								(0 < p_freq_list_node->arfcn_stats.num_bs_true_ho_allwd))
						{
							p_freq_list_node->arfcn_stats.num_bs_true_ho_allwd--;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA HRPD NBR count decremented, "
									"for band %d, arfcn %d, num_bs_true_ho_allwd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_true_ho_allwd);
						}
						else if ((ho_status == SON_HO_UNDEFINED) &&
								(0 < p_freq_list_node->arfcn_stats.num_bs_true_ho_blklstd))
						{
							p_freq_list_node->arfcn_stats.num_bs_true_ho_blklstd--;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA HRPD NBR count decremented, "
									"for band %d, arfcn %d, num_bs_true_ho_blklstd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_true_ho_blklstd);
						}
						else
						{
							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"Unknown handover status %d OR "
									"There are no neighbors configured on "
									"band %d, arfcn %d, broadcast status SON_TRUE",
									ho_status, band, arfcn);
						}

						break;
					}

				case SON_FALSE:
					{
						if ((ho_status == SON_HO_ALLOWED) &&
								(0 < p_freq_list_node->arfcn_stats.num_bs_false_ho_allwd))
						{
							p_freq_list_node->arfcn_stats.num_bs_false_ho_allwd--;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA HRPD NBR count successfully decremented, "
									"for band %d, arfcn %d, num_bs_false_ho_allwd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_false_ho_allwd);
						}
						else if ((ho_status == SON_HO_UNDEFINED) &&
								(0 < p_freq_list_node->arfcn_stats.num_bs_false_ho_blklstd))
						{
							p_freq_list_node->arfcn_stats.num_bs_false_ho_blklstd--;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA HRPD NBR count decremented, "
									"for band %d, arfcn %d, num_bs_false_ho_blklstd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_false_ho_blklstd);
						}
						else
						{
							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_WARNING,
									"Unknown handover status %d OR "
									"There are no neighbors configured on "
									"band %d arfcn %d broadcast status SON_FALSE",
									ho_status, band, arfcn);
						}

						break;
					}

				default:
					{
						SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_ERROR,
								"Unknown broadcast status %d",
								broadcast_status);
						break;
					}
			} /* switch */

			/* Checking the total no. of nbrs left, if 0 then delete record */
			total_nbrs = anr_get_no_of_hrpd_nbrs_per_frequency(p_freq_list_node);

			if ((SON_TRUE == ret_val) && (0 == total_nbrs))
			{
				anr_delete_record_from_cdma_hrpd_stats_list(
						p_list,
						band, arfcn);
			}
		}
		else
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_ERROR,
					"No node found for "
					"band class: %d, arfcn: %d", band, arfcn);
		}
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"No node found for band: %d",
				band);
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_decrement_nbr_count_hrpd */

/*****************************************************************************
 * Function Name  : anr_decrement_nbr_count_xrtt
 * Inputs         : p_list              - Pointer to SON LIST HASH Table
 *                : band                - CDMA2000 band class
 *                  arfcn               - Arfcn
 *                  broadcast_status    - CDMA broadcast status
 *                  ho_status           - Handover status
 * Outputs        : None
 * Returns        : son_bool_et         - SON_TRUE / SON_FALSE
 * Description    : This function decrements the CDMA XRTT neighbor count
 *****************************************************************************/
son_bool_et
anr_decrement_nbr_count_xrtt
(
 SON_LIST                     *p_list,
 son_cdma_band_class_et band,
 son_u16 arfcn,
 son_bool_et broadcast_status,
 son_ho_status_et ho_status
 )
{
	anr_cdma_xrtt_band_stats_node_t *p_band_list_node   = SON_PNULL;
	anr_cdma_xrtt_freq_stats_node_t *p_freq_list_node   = SON_PNULL;
	son_bool_et ret_val             = SON_FALSE;
	son_u16 total_nbrs          = 0;


	SON_UT_TRACE_ENTER();

	/* Finding the band in the band stats list */
	p_band_list_node = anr_find_record_in_cdma_xrtt_band_stats_list(
			p_list, band);

	if (SON_PNULL != p_band_list_node)
	{
		/* Finding the frequency in the frequency list */
		p_freq_list_node = anr_find_record_in_cdma_xrtt_freq_stats_list(
				&p_band_list_node->cdma_band_stats.freq_list,
				arfcn);

		if (SON_PNULL != p_freq_list_node)
		{
			switch (broadcast_status)
			{
				case SON_TRUE:
					{
						if ((ho_status == SON_HO_ALLOWED) &&
								(0 < p_freq_list_node->arfcn_stats.num_bs_true_ho_allwd))
						{
							p_freq_list_node->arfcn_stats.num_bs_true_ho_allwd--;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA XRTT NBR count decremented, "
									"for band %d arfcn %d num_bs_true_ho_allwd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_true_ho_allwd);
						}
						else if ((ho_status == SON_HO_UNDEFINED) &&
								(0 < p_freq_list_node->arfcn_stats.num_bs_true_ho_blklstd))
						{
							p_freq_list_node->arfcn_stats.num_bs_true_ho_blklstd--;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA XRTT NBR count decremented, "
									"for band %d, arfcn %d num_bs_true_ho_blklstd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_true_ho_blklstd);
						}
						else
						{
							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_WARNING,
									"Unknown handover status %d OR "
									"There are no neighbors configured on "
									"band %d, arfcn %d broadcast status SON_TRUE",
									ho_status, band, arfcn);
						}

						break;
					}

				case SON_FALSE:
					{
						if ((ho_status == SON_HO_ALLOWED) &&
								(0 < p_freq_list_node->arfcn_stats.num_bs_false_ho_allwd))
						{
							p_freq_list_node->arfcn_stats.num_bs_false_ho_allwd--;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA XRTT NBR count decremented, "
									"for band %d arfcn %d num_bs_false_ho_allwd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_false_ho_allwd);
						}
						else if ((ho_status == SON_HO_UNDEFINED) &&
								(0 < p_freq_list_node->arfcn_stats.num_bs_false_ho_blklstd))
						{
							p_freq_list_node->arfcn_stats.num_bs_false_ho_blklstd--;
							ret_val = SON_TRUE;

							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
									"CDMA XRTT NBR count decremented, "
									"for band %d, arfcn %d num_bs_false_ho_blklstd %d",
									band, arfcn, p_freq_list_node->arfcn_stats.num_bs_false_ho_blklstd);
						}
						else
						{
							SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_WARNING,
									"Unknown handover status %d OR "
									"No neighbors configured on "
									"band %d, arfcn %d, broadcast status SON_FALSE",
									ho_status, band, arfcn);
						}

						break;
					}

				default:
					{
						SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_ERROR,
								"Unknown broadcast status %d",
								broadcast_status);
						break;
					}
			} /* switch */

			/* Checking the total no. of nbrs left, if 0 then delete record */
			total_nbrs = anr_get_no_of_xrtt_nbrs_per_frequency(p_freq_list_node);

			if ((SON_TRUE == ret_val) && (0 == total_nbrs))
			{
				anr_delete_record_from_cdma_xrtt_stats_list(
						p_list,
						band, arfcn);
			}
		}
		else
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_ERROR,
					"No node found for "
					"band class: %d, arfcn:%d",
					band, arfcn);
		}
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"No node found for band: %d",
				band);
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_decrement_nbr_count_xrtt */

/*****************************************************************************
 * Function Name  : decrement_cdma_hrpd_nbr_count_for_rrm
 * Inputs         : p_nbr_info       - CDMA neighbour information
 *                  p_cell_ctxt - Pointer to serving cell context
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function decrements the CDMA HRPD neighbor count
 *****************************************************************************/
son_bool_et
decrement_cdma_hrpd_nbr_count_for_rrm
(
 son_cdma_neighbor_info_t   *p_nbr_info,
 anr_cell_context_t         *p_cell_ctxt
 )
{
	son_ho_status_et ho_status = SON_HO_UNDEFINED;
	son_bool_et ret_val    = SON_FALSE;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Invalid data as"
				"p_cell_ctxt is SON_PNULL");

		SON_UT_TRACE_EXIT();
		return ret_val;
	}

	ho_status = anr_get_ho_status_of_cdma_nr(p_nbr_info);

	ret_val = anr_decrement_nbr_count_hrpd(
			&p_cell_ctxt->cdma_hrpd_band_stats_list,
			p_nbr_info->band_class,
			p_nbr_info->arfcn,
			p_nbr_info->broadcast_status,
			ho_status);

	return ret_val;

	SON_UT_TRACE_EXIT();
} /* decrement_cdma_hrpd_nbr_count_for_rrm */

/*****************************************************************************
 * Function Name  : decrement_cdma_xrtt_nbr_count_for_rrm
 * Inputs         : p_nbr_info       - CDMA neighbour information
 *                  p_cell_ctxt - Pointer to serving cell context
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function decrements the CDMA XRTT neighbor count
 *****************************************************************************/
son_bool_et
decrement_cdma_xrtt_nbr_count_for_rrm
(
 son_cdma_neighbor_info_t   *p_nbr_info,
 anr_cell_context_t         *p_cell_ctxt
 )
{
	son_ho_status_et ho_status = SON_HO_UNDEFINED;
	son_bool_et ret_val     = SON_FALSE;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Invalid data as"
				"p_cell_ctxt is SON_PNULL");

		SON_UT_TRACE_EXIT();
		return ret_val;
	}

	ho_status = anr_get_ho_status_of_cdma_nr(p_nbr_info);

	ret_val = anr_decrement_nbr_count_xrtt(
			&p_cell_ctxt->cdma_xrtt_band_stats_list,
			p_nbr_info->band_class,
			p_nbr_info->arfcn,
			p_nbr_info->broadcast_status,
			ho_status);

	return ret_val;

	SON_UT_TRACE_EXIT();
} /* decrement_cdma_xrtt_nbr_count_for_rrm */

/*****************************************************************************
 * Function Name  : increment_cdma_xrtt_nbr_count_for_rrm
 * Inputs         : p_nbr_info  - Pointer to CDMA neighbor information
 *                  p_cell_ctxt - Pointer to Serving Cell context
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function increments the CDMA XRTT neighbor count
 *****************************************************************************/
son_bool_et
increment_cdma_xrtt_nbr_count_for_rrm
(
 son_cdma_neighbor_info_t   *p_nbr_info,
 anr_cell_context_t         *p_cell_ctxt
 )
{
	son_bool_et ret_val         = SON_FALSE;
	son_ho_status_et ho_status = SON_HO_UNDEFINED;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Invalid data as "
				"p_cell_ctxt is SON_PNULL");

		return ret_val;
	}

	ho_status = anr_get_ho_status_of_cdma_nr(p_nbr_info);

	ret_val = anr_increment_nbr_count_xrtt(
			&p_cell_ctxt->cdma_xrtt_band_stats_list,
			p_nbr_info->band_class,
			p_nbr_info->arfcn,
			p_nbr_info->broadcast_status,
			ho_status);

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* increment_cdma_xrtt_nbr_count_for_rrm */

/*****************************************************************************
 * Function Name  : anr_find_record_in_cdma_xrtt_freq_stats_list
 * Inputs         : p_list  - Pointer to frequency statistics List
 *                  arfcn   - ARFCN value
 * Outputs        : None
 * Returns        : anr_cdma_xrtt_freq_stats_node_t* - Pointer to cdma xrtt freq
 *                                                     stats node
 * Description    : This function finds the frequency statistcs node for the
 *                   given arfcn value
 *****************************************************************************/
anr_cdma_xrtt_freq_stats_node_t *
anr_find_record_in_cdma_xrtt_freq_stats_list
(
 SON_LIST                     *p_list,
 son_u16 arfcn
 )
{
	SON_LIST_NODE                *p_freq_node             = SON_PNULL;
	anr_cdma_xrtt_freq_stats_node_t  *p_freq_list_node         = SON_PNULL;


	SON_UT_TRACE_ENTER();

	/* Frequency level search */
	p_freq_node = list_find_node(p_list,
			(son_void_t *)(&arfcn),
			anr_key_of_cdma_xrtt_freq_stats_list,
			anr_compare_cdma_xrtt_freq_stats_list_node);

	if (p_freq_node != SON_PNULL)
	{
		p_freq_list_node = YMEMBEROF(anr_cdma_xrtt_freq_stats_node_t,
				anchor, p_freq_node);
	}

	SON_UT_TRACE_EXIT();
	return p_freq_list_node;
} /* anr_find_record_in_cdma_xrtt_freq_stats_list */

/*****************************************************************************
 * Function Name  : anr_find_record_in_cdma_xrtt_band_stats_list
 * Inputs         : p_list  : Pointer to broadcast allowed neighbor
 *                            statistics List
 *                  band    : band class
 * Outputs        : None
 * Returns        : anr_cdma_xrtt_band_stats_node_t* - Pointer to cdma xrtt band
 *                                                     stats node
 * Description    : This function finds the broadcast statistcs node for the
 *                  given band class
 *****************************************************************************/
anr_cdma_xrtt_band_stats_node_t *
anr_find_record_in_cdma_xrtt_band_stats_list
(
 SON_LIST                     *p_list,
 son_cdma_band_class_et band
 )
{
	SON_LIST_NODE                *p_band_node             = SON_PNULL;
	anr_cdma_xrtt_band_stats_node_t *p_band_list_node        = SON_PNULL;


	SON_UT_TRACE_ENTER();

	/* Band level search */
	p_band_node = list_find_node(p_list, (son_void_t *)&(band),
			anr_key_of_cdma_xrtt_band_stats_list,
			anr_compare_cdma_xrtt_band_stats_list_node);

	if (p_band_node != SON_PNULL)
	{
		p_band_list_node = YMEMBEROF(anr_cdma_xrtt_band_stats_node_t,
				anchor, p_band_node);
	}

	SON_UT_TRACE_EXIT();
	return p_band_list_node;
} /* anr_find_record_in_cdma_xrtt_band_stats_list */

/*****************************************************************************
 * Function Name  : anr_compare_txn_id
 * Inputs         : p_val_1   - Pointer to the transaction Id as void pointer
 *                : p_val_2   - Pointer to the transaction Id as void pointer
 * Outputs        : None
 * Returns        : son_s32   - Zero If matches
 *                              Non Zero If not matches
 * Description    : This function compares the transaction Ids
 ******************************************************************************/
son_s32
anr_compare_txn_id
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointer to the transaction Id is NULL");
	}
	else
	{
		son_u16 txn_id_1 = *(son_u16 *)p_val_1;
		son_u16 txn_id_2 = *(son_u16 *)p_val_2;

		if (txn_id_1 < txn_id_2)
		{
			ret_value = -1;
		}
		else if (txn_id_1 > txn_id_2)
		{
			ret_value = 1;
		}
		else
		{
			ret_value = 0;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_txn_id */

/*****************************************************************************
 * Function Name  : anr_key_of_nr_remove_list
 * Inputs         : p_node - anchor field of the nr remove list record
 * Outputs        : None
 * Returns        : son_void_t  - pointer to global cell id as void pointer
 * Description    : This function determines the key in Cell specifc Remove list
 ******************************************************************************/
const son_void_t *
anr_key_of_nr_remove_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_nr_remove_list_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_nr_remove_list_node_t, nr_remove_list_node, p_node);

	return (son_void_t *)(&p_data->cell_id);
}

/*****************************************************************************
 * Function Name  : anr_compare_nr_remove_list_node
 * Inputs         : p_val_1   -  Pointer to the son_global_cell_id structure as
 *                            -  void pointer
 *                : p_val_2     Pointer to the son_global_cell_id structure as
 *                              void pointer
 * Outputs        : None
 * Returns        : son_s32   - Zero If matches
 *                              Non Zero If not matches
 * Description    : This function compares the son_global_cell_id structure
 ******************************************************************************/
son_s32
anr_compare_nr_remove_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointer to the cell Id is NULL");
	}
	else
	{
		ret_value = SON_MEMCMP(p_val_1, p_val_2, sizeof(son_global_cell_id_t));
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_nr_remove_list_node */

/*****************************************************************************
 * Function Name  : anr_init_global_context
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_return_et - SON_SUCCESS / SON_FAILURE or SON_PARTIAL_SUCCESS
 * Description    :This function initialzes the  ANR global context
 ******************************************************************************/
/* + SPR 17439 */
son_return_et
anr_init_global_context(void)
/* - SPR 17439 */
{
	son_return_et ret_val = SON_SUCCESS;


	SON_UT_TRACE_ENTER();
	g_anr_context.current_fsm_state = ANR_STATE_INIT;
	g_anr_context.anr_global_mode = SON_ENABLED;
	g_anr_context.pending_scan_res_count = 0;
	g_anr_context.pending_scan_res_error_code = SON_NO_ERROR;
	g_anr_context.x2_enabled = SON_TRUE;
	g_anr_context.log_enable = SON_OAM_LOG_ON;

	/* Rel 3.0 Multi Sector code changes Start */
	SON_MEMSET(&g_anr_context.own_enb_id, 0, sizeof(son_global_enb_id_t));
	g_anr_context.own_enb_id.enb_type = SON_ENB_TYPE_UNDEFINED;
	/* Rel 3.0 Multi Sector code changes Stop */

	g_anr_context.pci_mode   = SON_PCI_ENABLE;
	/* SPR-13251 Fix Starts */
	g_anr_context.log_level = SON_OAM_LOG_LEVEL_BRIEF;
	/* SPR-13251 Fix Ends */
	SET_MODULE_LOG_LEVEL(SON_OAM_LOG_LEVEL_BRIEF);

	list_init(&g_anr_context.cell_ctxt_list);
	list_init(&g_anr_context.x2_pending_res_list);
	/*SPR-9574-Start*/
	list_init(&g_anr_context.intra_freq_list);
	list_init(&g_anr_context.inter_freq_list);
	/*SPR-9574-End*/
	/* eICIC feature code starts */
	g_anr_context.csg_scheme = SON_NO_INTF_SCHEME;
	g_anr_context.pico_scheme = SON_NO_INTF_SCHEME;
	g_anr_context.macro_scheme = SON_NO_INTF_SCHEME;
	/* eICIC feature code ends */

	ret_val = anr_init_db();
	if (SON_SUCCESS == ret_val)
	{
		anr_print_global_context("anr_init_global_context");
		g_anr_context.anr_timer_id = son_start_timer(ANR_DEFAULT_TIMEOUT,
				SON_PNULL,
				0,
				SON_TRUE);

        /* SPR 15698 Start */
        /// Start CGI Gaurd timer. This timer will keep expired in 1 second.
        /// On Expiry of this timer, the CGI list will be traversed and current timestamp will be compared with the
        /// absolute timestamp configured against each node. A node will be deleted from the list if
        /// current_timestamp > cgi_node.absolute_gaurd_timestamp
        ///
        g_anr_context.cgi_gaurd_timer_id = son_start_timer(CGI_GAURD_TIMER_PERIODICITY_IN_MSECS,
                SON_PNULL,
                0,
                SON_TRUE);

        /* SPR 15698 End */
	}
	else
	{
		anr_clear_db();
	}

	SON_UT_TRACE_EXIT();

	return ret_val;
} /* anr_init_global_context */

/*****************************************************************************
 * Function Name  : anr_init_cell_context
 * Inputs         : p_ctxt - Pointer to cell context
 * Outputs        : None
 * Returns        : son_return_et - SON_SUCCESS / SON_FAILURE or SON_PARTIAL_SUCCESS
 * Description    : This function initializes the specified cell context
 ******************************************************************************/
son_return_et
anr_init_cell_context
(
 anr_cell_context_t *p_ctxt
 )
{
	son_return_et ret_val = SON_SUCCESS;


	SON_UT_TRACE_ENTER();
	if (p_ctxt != SON_PNULL)
	{
		SON_MEMSET(&p_ctxt->cell_id, 0, sizeof(son_intra_rat_global_cell_id_t));
		SON_MEMSET(p_ctxt->csg_identity, 0, sizeof(p_ctxt->csg_identity));
		SON_MEMSET(p_ctxt->tac, 0, sizeof(p_ctxt->tac));
		SON_MEMSET(p_ctxt->plmn_id, 0, sizeof(p_ctxt->plmn_id));
		SON_MEMSET(&p_ctxt->choice_eutra_mode, 0, sizeof(p_ctxt->choice_eutra_mode));
		SON_MEMSET(&p_ctxt->removal_criteria, 0, sizeof(p_ctxt->removal_criteria));
		SON_MEMSET(&p_ctxt->pci_confusion_cfg, 0, sizeof(p_ctxt->pci_confusion_cfg));
		p_ctxt->num_plmn_id = 0;

		p_ctxt->current_cell_fsm_state = ANR_CELL_STATE_DISABLED;
		p_ctxt->previous_cell_fsm_state = ANR_CELL_STATE_DISABLED;
		p_ctxt->current_cell_state = RRM_CELL_OPERATIONAL;

		list_init(&p_ctxt->nr_remove_list);
		list_init(&p_ctxt->txn_id_list);
		list_init(&p_ctxt->bs_allowed_stats_list);
		list_init(&p_ctxt->utran_freq_fdd_stats_list);
		list_init(&p_ctxt->utran_freq_tdd_stats_list);
		list_init(&p_ctxt->geran_freq_stats_list);
		list_init(&p_ctxt->pci_sus_conf_ctxt_list);
		list_init(&p_ctxt->cdma_xrtt_band_stats_list);
		list_init(&p_ctxt->cdma_hrpd_band_stats_list);
		/*SPR-9759-Start*/
		list_init(&p_ctxt->eutran_pci_requested_cgi_list);
		list_init(&p_ctxt->utran_pci_requested_cgi_list);
		list_init(&p_ctxt->geran_pci_requested_cgi_list);
		list_init(&p_ctxt->cdma_pci_requested_cgi_list);
		/*SPR-9759-End*/

		/*SPR-12596-Start*/
		list_init(&p_ctxt->conflict_confusion_pci_list);
		/*SPR-12596-End*/

		p_ctxt->limited_mode = SON_FALSE;
		SON_MEMSET(&p_ctxt->prach_configuration,0,sizeof(p_ctxt->prach_configuration));
		p_ctxt->is_prach_configuration_available = SON_FALSE;
		SON_MEMSET(&p_ctxt->rach_related_info.old_prach_configuration,0,
				sizeof(p_ctxt->rach_related_info.old_prach_configuration));
		p_ctxt->rach_related_info.is_old_prach_configuration_available = SON_FALSE;
		p_ctxt->rach_related_info.is_cell_info_ind_valid_as_per_anr = SON_FALSE;
		p_ctxt->is_csg_id_available = SON_FALSE;
		list_init(&p_ctxt->extended_rnc_id_list);
		list_init(&p_ctxt->non_extended_rnc_id_list);
        /* Spr 17753 Changes Start */ 
        p_ctxt->is_conflict_with_second_degree_enable = SON_FALSE;
        p_ctxt->conflict_detection_backoff_timer_val  = 0;
        p_ctxt->son_backoff_timer_id                  = SON_PNULL;
        p_ctxt->nr_add_wait_timer_val                 = 0;
        p_ctxt->son_nr_add_timer_id                   = SON_PNULL;
        list_init(&p_ctxt->second_degree_nbr_list);
        /* Spr 17753 Changes End */ 

		ret_val = anr_init_intra_rat_hash(&p_ctxt->intra_rat_neighbor_table);

		if (SON_SUCCESS == ret_val)
		{
			ret_val = anr_init_inter_rat_hash(&p_ctxt->inter_rat_neighbor_table);
		}

		if (SON_SUCCESS == ret_val)
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_BRIEF,
					"Cell Context Initialized for Cell"
					" [Cell Id: 0x%x]", son_convert_char_cell_id_to_int_cell_id(p_ctxt->cell_id.cell_identity));
#ifdef SON_DEBUG
			anr_print_cell_context(p_ctxt);
#endif
		}
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Input Cell Context Pointer is NULL");
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_init_cell_context */

/*****************************************************************************
 * Function Name  : anr_get_context_cell_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u32 - number of context cells configured
 * Description    : This function gets number of context cells configured
 ******************************************************************************/
son_u32
anr_get_context_cell_count
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return list_count(&g_anr_context.cell_ctxt_list);
}

/*****************************************************************************
 * Function Name  : anr_get_cell_ctxt_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_LIST * - Pointer to the cell context list
 * Description    : This function gives the pointer to list of cells configured.
 ******************************************************************************/
SON_LIST *
anr_get_cell_ctxt_list
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return &g_anr_context.cell_ctxt_list;
}

/*****************************************************************************
 * Function Name  : anr_get_cell_context_from_global_context
 * Inputs         : p_cell_id - Pointer to the cell Id
 * Outputs        : None
 * Returns        : anr_cell_context_t * - Pointer to the cell context
 * Description    : This function gets the cell context of the specified cell
 ******************************************************************************/
anr_cell_context_t *
anr_get_cell_context_from_global_context
(
 son_intra_rat_global_cell_id_t     *p_cell_id
 )
{
	anr_cell_context_node_t     *p_data         = SON_PNULL;
	SON_LIST_NODE                 *p_list_node     = SON_PNULL;
	anr_cell_context_t            *p_ret_val        = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_id)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Cell ID is NULL");

		SON_UT_TRACE_EXIT();
		return p_ret_val;
	}

	p_list_node = list_find_node(&g_anr_context.cell_ctxt_list,
			p_cell_id, son_key_of_anr, son_compare_cell_id);

	if (SON_PNULL != p_list_node)
	{
		p_data = YMEMBEROF(anr_cell_context_node_t, sNode, p_list_node);
		p_ret_val = &p_data->data;
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Cell 0x%x not found",
                son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return p_ret_val;
} /* anr_get_cell_context_from_global_context */

/*****************************************************************************
 * Function Name  : anr_get_context_state
 * Inputs         : None
 * Outputs        : None
 * Returns        : anr_fsm_state_et - ANR context state
 * Description    : This function gets the ANR context state
 ******************************************************************************/
anr_fsm_state_et
anr_get_context_state
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_anr_context.current_fsm_state;
}

/*****************************************************************************
 * Function Name  : anr_set_context_state
 * Inputs         : state - context state
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function sets the context state
 ******************************************************************************/
son_void_t
anr_set_context_state
(
 anr_fsm_state_et state
 )
{
	SON_UT_TRACE_ENTER();
	if (g_anr_context.current_fsm_state != state)
	{
		SONANR_FSM_SET_STATE(g_anr_context.current_fsm_state, state);
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_set_log_mode
 * Inputs         : log_mode
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function sets the log mode
 ******************************************************************************/
son_void_t
anr_set_log_mode
(
 son_oam_log_on_off_et log_mode
 )
{
	SON_UT_TRACE_ENTER();
	g_anr_context.log_enable = log_mode;
	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_set_x2_enabled
 * Inputs         : x2_enabled
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function sets the x2 enable/disable
 ******************************************************************************/
son_void_t
anr_set_x2_enabled
(
 son_bool_et x2_enabled
 )
{
	SON_UT_TRACE_ENTER();
	g_anr_context.x2_enabled = x2_enabled;
	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_validate_cell_data
 * Inputs         : p_cell - Pointer to the cell Information Structure
 * Outputs        : None
 * Returns        : SON_TRUE: If valid
 *                  SON_FALSE:If not valid
 * Description    : This function validates the data of cell received in cell
 *                  info indication
 ******************************************************************************/
static son_bool_et
anr_validate_cell_data
(
 son_cell_t  *p_cell
 )
{
	son_bool_et is_valid = SON_TRUE;
	/*SPR-10230-Start*/
	son_u16                      temp_open_pci_end = 0;
	son_u16                      temp_closed_pci_end = 0;
	son_u16                      temp_hybrid_pci_end = 0;
	/*SPR-10230-End*/
	/*SPR-10426-Start*/
	son_bool_et is_home_eNB = SON_FALSE;
	/*SPR-10426-End*/

	SON_UT_TRACE_ENTER();

	/*SPR-10230-Start*/

	/*SPR-10426-Start*/

	/*Check if the pci range received is valid or not.Also check whether eNB is to be configured as Home or Macro*/

	if(p_cell->bitmask & SON_CELL_INTRA_FREQ_OPEN_PCI_RANGE_PRESENT)
	{
		/*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
		temp_open_pci_end = son_determine_pci_range_end_value(p_cell->intra_freq_open_pci_range.pci_range);
		if((temp_open_pci_end == 0) || ((p_cell->intra_freq_open_pci_range.pci_start + temp_open_pci_end -1) > 503))
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_ERROR,
					"Invalid pci range received. open_pci_start[%d], open_pci_end[%d]",
                    p_cell->intra_freq_open_pci_range.pci_start,
                    temp_open_pci_end);

			is_valid = SON_FALSE;
			SON_UT_TRACE_EXIT();
			return is_valid;
		}

		if((p_cell->pci >=  p_cell->intra_freq_open_pci_range.pci_start) &&
				(p_cell->pci <= (p_cell->intra_freq_open_pci_range.pci_start + temp_open_pci_end -1)))
		{
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_INFO,
					"Open HeNB PCI[%d]", p_cell->pci);
			is_home_eNB = SON_TRUE;
		}
	}

	if(p_cell->bitmask & SON_CELL_INTRA_FREQ_CLOSED_PCI_RANGE_PRESENT)
	{
		/*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
		temp_closed_pci_end = son_determine_pci_range_end_value(p_cell->intra_freq_closed_pci_range.pci_range);
		if((temp_closed_pci_end == 0) || ((p_cell->intra_freq_closed_pci_range.pci_start + temp_closed_pci_end -1) > 503))
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_ERROR,
					"Invalid pci range received. closed_pci_start[%d], closed_pci_end[%d]",
                    p_cell->intra_freq_closed_pci_range.pci_start,
                    temp_closed_pci_end);

			is_valid = SON_FALSE;
			SON_UT_TRACE_EXIT();
			return is_valid;
		}

		if((p_cell->pci >=  p_cell->intra_freq_closed_pci_range.pci_start) &&
				(p_cell->pci <= (p_cell->intra_freq_closed_pci_range.pci_start + temp_closed_pci_end -1)))
		{
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_INFO,
					"Close HeNB PCI[%d]", p_cell->pci);
			is_home_eNB = SON_TRUE;
		}
	}

	if(p_cell->bitmask & SON_CELL_INTRA_FREQ_HYBRID_PCI_RANGE_PRESENT)
	{
		/*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
		temp_hybrid_pci_end = son_determine_pci_range_end_value(p_cell->intra_freq_hybrid_pci_range.pci_range);
		if((temp_hybrid_pci_end == 0) || ((p_cell->intra_freq_hybrid_pci_range.pci_start + temp_hybrid_pci_end -1) > 503))
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_ERROR,
					"Invalid pci range received. hybrid_pci_start[%d], hybrid_pci_end[%d]",
                    p_cell->intra_freq_hybrid_pci_range.pci_start,
                    temp_hybrid_pci_end);

			is_valid = SON_FALSE;
			SON_UT_TRACE_EXIT();
			return is_valid;
		}

		if((p_cell->pci >=  p_cell->intra_freq_hybrid_pci_range.pci_start) &&
				(p_cell->pci <= (p_cell->intra_freq_hybrid_pci_range.pci_start + temp_hybrid_pci_end -1)))
		{
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_INFO,
					"Hybrid HeNB PCI[%d]", p_cell->pci);
			is_home_eNB = SON_TRUE;
		}
	}

	/*SPR-10426-End*/

	/*SPR-10230-End*/

	/* Check if it is the first cell being configured at ANR */
	if (0 == g_anr_context.cell_ctxt_list.count)
	{
		/* Rel 3.0 Multi Sector code changes Start */
		SON_MEMCPY(&g_anr_context.own_enb_id.plmn_id,
				&p_cell->src_cgi.plmn_id,
				sizeof(son_cell_plmn_info_t));
		/*Check if the serving cell is home or macro*/
		if(is_home_eNB == SON_TRUE) /*SPR-10426-If condition modified*/
		{
			g_anr_context.own_enb_id.enb_type = SON_HOME_ENB;
			g_anr_context.own_enb_id.enb_id[0] = 
				p_cell->src_cgi.cell_identity[0];
			g_anr_context.own_enb_id.enb_id[1] = 
				p_cell->src_cgi.cell_identity[1];
			g_anr_context.own_enb_id.enb_id[2] = 
				p_cell->src_cgi.cell_identity[2];
			g_anr_context.own_enb_id.enb_id[3] = 
				p_cell->src_cgi.cell_identity[3] & 0xF0;
			SON_LOG(g_anr_context.log_enable,
					p_son_anr_facility_name,
					SON_INFO,
					"Serving is a HeNB");
		}
		else
		{
			g_anr_context.own_enb_id.enb_type = SON_MACRO_ENB;
			anr_determine_macro_enb_id(&p_cell->src_cgi, 
					&g_anr_context.own_enb_id);
			/* Rel 3.0 Multi Sector code changes Stop */
			SON_LOG(g_anr_context.log_enable,
					p_son_anr_facility_name,
					SON_INFO, "Serving is a Macro eNB");
		}

	}

	/* Check if 1 cell is already configured and if the eNB is
	 * home eNB do not configure any other cell as home eNB can
	 * have only one serving cell */
	/* Rel 3.0 Multi Sector code changes Start */
	else if (SON_HOME_ENB == g_anr_context.own_enb_id.enb_type)
		/* Rel 3.0 Multi Sector code changes Stop */
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_WARNING,
				"Cell [Cell Id: 0x%x]"
				" is not added to ANR Global Context - HeNB is"
				" already configured with 1 serving cell",
				son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));
		is_valid = SON_FALSE;
	}
	/* Rel 3.0 Multi Sector code changes Start */
	else if ((SON_MACRO_ENB ==  g_anr_context.own_enb_id.enb_type) &&
			/*SPR-11554-Start*/
			(SON_TRUE == is_home_eNB))
		/* Rel 3.0 Multi Sector code changes Stop */
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_WARNING,                
				"Cell [Cell Id: 0x%x]"
				" is not added to ANR Global Context - macro eNB "
				" as it is already configured as macro, cannot configure femto cell",
				son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));
		/*SPR-11554-End*/
		is_valid = SON_FALSE;
	}

	SON_UT_TRACE_EXIT();
	return is_valid;
} /* anr_validate_cell_data */



/*****************************************************************************
 * Function Name  : anr_add_new_cell_in_context
 * Inputs         : p_cell - Pointer to the cell Information Structure
 * Outputs        : None
 * Returns        : anr_cell_context_node_t * - Pointer to the new node added
 *                                              in the list, NULL if node
 *                                              already exists or creation of
 *                                              node fails
 * Description    : This function adds new cell in the cell context
 ******************************************************************************/
anr_cell_context_node_t *
anr_add_new_cell_in_context
(
/*SPR_19279_START*/
 son_cell_t  *p_cell,
 son_u8 cell_index
/*SPR_19279_END*/
 )
{
	anr_cell_context_node_t *p_new_node     = SON_PNULL;
	SON_LIST_NODE             *p_list_node     = SON_PNULL;
	son_bool_et is_update       = SON_FALSE;
	son_bool_et is_pci_earfcn_updated  = SON_FALSE;
	son_x2_nrt_update_ind_t nrt_upd_ind     = {0};

	/*SPR-12596-Start*/
	son_anr_pci_conflict_confusion_ind_t msg;
	conflict_confusion_pci_list_node_t  *p_pci_list_node = SON_PNULL;
	/*SPR-12596-End*/


	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_cell)
	{
		p_list_node = list_find_node(&g_anr_context.cell_ctxt_list,
                /*SPR_19279_START*/
				&cell_index, son_cell_index_key_of_anr,
				son_compare_cell_index);
        /*SPR_19279_END*/

		if (SON_PNULL == p_list_node)
		{
			if (g_anr_context.cell_ctxt_list.count <
					SON_MAX_CELL_SUPPORTED_AT_ENB)
			{
				if (!(p_cell->bitmask & SON_CELL_TAC_PRESENT) ||
						!(p_cell->bitmask & SON_CELL_PLMN_PRESENT) ||
						!(p_cell->bitmask & SON_CELL_EUTRA_MODE_PRESENT))
				{
					SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
							SON_ERROR,
							"Cell [Cell Id: 0x%x]"
							" could not be added to ANR Global Context - "
							"Mandatory parameters TAC, PLMN or EUTRA Mode  is not "
							"present",
							son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));
					SON_UT_TRACE_EXIT();
					return p_new_node;
				}

				if (!(p_cell->choice_eutra_mode.bitmask &&
							((p_cell->choice_eutra_mode.bitmask ==  
							  SON_EUTRA_MODE_FDD) ||
							 (p_cell->choice_eutra_mode.bitmask ==
							  SON_EUTRA_MODE_TDD))))
				{
					SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
							SON_ERROR,
							"Cell [Cell Id: 0x%x]"
							" could not be added to ANR Global Context - "
							"Incorrect EUTRA Mode (FDD/TDD)",
							son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));
					SON_UT_TRACE_EXIT();
					return p_new_node;
				}

				if((p_cell->choice_eutra_mode.bitmask & SON_EUTRA_MODE_FDD) &&
						(p_cell->choice_eutra_mode.fdd_info.ul_earfcn ==
						 p_cell->choice_eutra_mode.fdd_info.dl_earfcn))
				{
					SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
							SON_ERROR,
							"Serving Cell [Cell Id: 0x%x]"
							" could not be added to ANR Global Context "
							" as EARFCN is in FDD mode but UL EARFCN %u is same as DL EARFCN",
							son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity),
                            p_cell->choice_eutra_mode.fdd_info.ul_earfcn);

					SON_UT_TRACE_EXIT();
					return p_new_node;
				}

				if (SON_FALSE == anr_validate_cell_data(p_cell))
				{
					SON_UT_TRACE_EXIT();
					return p_new_node;
				}


				/*Create a new node*/
				p_new_node = (anr_cell_context_node_t *)son_mem_get(
						sizeof(anr_cell_context_node_t));

				if (SON_PNULL != p_new_node)
				{
					/*Populate the node*/
					if (SON_SUCCESS == anr_init_cell_context(&p_new_node->data))
					{
                        /*SPR_19279_START*/
                        p_new_node->data.cell_index = cell_index;
                        /*SPR_19279_END*/
						SON_MEMCPY(&p_new_node->data.cell_id, &p_cell->src_cgi,
								sizeof(son_intra_rat_global_cell_id_t));

						p_new_node->data.pci_value   = p_cell->pci;
						SON_MEMCPY(p_new_node->data.tac,
								p_cell->tac,
								sizeof(p_new_node->data.tac));
						p_new_node->data.num_plmn_id = p_cell->num_plmn_id;
						SON_MEMCPY(p_new_node->data.plmn_id,
								p_cell->plmn_id,
								sizeof(p_new_node->data.plmn_id));

						if (SON_CELL_CSG_IDENTITY_PRESENT & p_cell->bitmask)
						{
							SON_MEMCPY(p_new_node->data.csg_identity,
									p_cell->csg_identity,
									SON_CSG_ID_OCTET_SIZE * sizeof(son_u8));
							p_new_node->data.is_csg_id_available = SON_TRUE;
						}

						SON_MEMCPY(&p_new_node->data.choice_eutra_mode,
								&p_cell->choice_eutra_mode,
								sizeof(p_new_node->data.choice_eutra_mode));

						p_new_node->data.rach_related_info.
							is_cell_info_ind_valid_as_per_anr  = SON_TRUE;

						/*Prach configuraiton*/
						if(p_cell->bitmask & SON_CELL_PRACH_CONFIG_PRESENT)
						{
							SON_MEMCPY(&p_new_node->data.prach_configuration,
									&p_cell->prach_config,
									sizeof(p_new_node->data.prach_configuration));
							p_new_node->data.is_prach_configuration_available = 
								SON_TRUE;
						}

                        /* Spr 17753 Changes Start */
                        if(p_cell->bitmask & SON_CELL_SECOND_DEGREE_CONFLICT_CONFIG_PRESENT)
                        {
                           p_new_node->data.is_conflict_with_second_degree_enable = 
                                p_cell->son_x2_second_degree_conflict_config.second_degree_conflict_enable;
                    
                           p_new_node->data.conflict_detection_backoff_timer_val = 
                               p_cell->son_x2_second_degree_conflict_config.conflict_detection_backoff_timer;

                           p_new_node->data.nr_add_wait_timer_val = 
                               p_cell->son_x2_second_degree_conflict_config.nr_add_wait_timer; 
                        }
                        /* Spr 17753 Changes End */
						/*Insert it in context list */
						list_insert_node(&g_anr_context.cell_ctxt_list,
								&p_new_node->sNode);

						/* Rel 3.0 Multi Sector code changes Start */
						if (SON_TRUE ==
								anr_get_auto_maintain_local_cell_as_nbr_status())
						{
							anr_add_serving_cell_as_nbr_autonomously(
									&p_cell->src_cgi);
						}
						/* Rel 3.0 Multi Sector code changes Stop */
						anr_print_global_context("anr_add_new_cell_in_context");

						/*SPR-9574-Start*/
						anr_insert_in_intra_freq_list(p_cell);
						/*SPR-9574-End*/
					}
					else
					{
						SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
								SON_ERROR,
								"Cell Context Initialization failed");
						son_mem_free(p_new_node);
						p_new_node = SON_PNULL;
					}
				}
				else
				{
					SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
							SON_ERROR,
							"Memory allocation failure");
				}
			}
			else
			{
				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_ERROR,
						"Cell [Cell Id: 0x%x]"
						" could not be added to ANR Global Context - "
						"Maximum(%d) supported cells limit reached",
						son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity),
						g_anr_context.cell_ctxt_list.count);
			}
		}
		else
		{
			p_new_node = YMEMBEROF(anr_cell_context_node_t, sNode, p_list_node);

			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_WARNING,
					"Cell [Cell Id: 0x%x] already"
					" exists in ANR Cell Context List",
					son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));

			if((p_cell->choice_eutra_mode.bitmask & SON_EUTRA_MODE_FDD) &&
					(p_cell->choice_eutra_mode.fdd_info.ul_earfcn ==
					 p_cell->choice_eutra_mode.fdd_info.dl_earfcn))
			{
				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_ERROR,
						"Serving Cell [Cell Id: 0x%x]"
						" could not be updated to ANR Global Context "
						" as EARFCN is in FDD mode but UL EARFCN %u is same as DL EARFCN ",
						son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity),
                        p_cell->choice_eutra_mode.fdd_info.ul_earfcn);

				p_new_node->data.rach_related_info.is_cell_info_ind_valid_as_per_anr
					= SON_FALSE;

				p_new_node = SON_PNULL;
				SON_UT_TRACE_EXIT();
				return p_new_node;
			}

			/*Determine if there is change in the EARFCNs and the change
			 * in the serving EARFCN will voilate the condition for RRM */
			if ((p_cell->bitmask & SON_CELL_EUTRA_MODE_PRESENT) &&
					((p_cell->choice_eutra_mode.bitmask == SON_EUTRA_MODE_FDD) ||
					 (p_cell->choice_eutra_mode.bitmask == SON_EUTRA_MODE_TDD)) &&
					SON_MEMCMP(&p_new_node->data.choice_eutra_mode,
						&p_cell->choice_eutra_mode, sizeof(p_new_node->data.choice_eutra_mode)))
			{
				anr_bs_allowed_node_t  *p_intra_sts_node = SON_PNULL;
				son_u32 earfcn = 0;
				if (p_cell->choice_eutra_mode.bitmask & SON_EUTRA_MODE_FDD)
				{
					earfcn = p_cell->choice_eutra_mode.fdd_info.dl_earfcn;
				}
				else
				{
					earfcn = p_cell->choice_eutra_mode.tdd_info.earfcn;
				}

				if (earfcn != anr_get_earfcn(&p_new_node->data))
				{
					p_intra_sts_node = anr_find_record_in_bs_allowed_stats_list(
							&p_new_node->data.bs_allowed_stats_list, earfcn);

					if (SON_PNULL == p_intra_sts_node && SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED <=
							list_count(&p_new_node->data.bs_allowed_stats_list))
					{
						SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
								SON_WARNING,
								"EARFCN can not be modified as we have already [%d] freqs "
								" [Cell Id: 0x%x]", list_count(&p_new_node->data.bs_allowed_stats_list),
								son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));

						p_new_node->data.rach_related_info.is_cell_info_ind_valid_as_per_anr
							= SON_FALSE;
						p_new_node = SON_PNULL;
						SON_UT_TRACE_EXIT();
						return p_new_node;
					}
				}
			}
            /*SPR_19279_START*/
            if (SON_MEMCMP(&p_new_node->data.cell_id,
                        &p_cell->src_cgi, sizeof(p_new_node->data.cell_id)))
            {
                SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                        SON_BRIEF,
                        "ECGI for cell [cell "
                        "id: 0x%x] updated",
                        son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));

                SON_MEMCPY(&p_new_node->data.cell_id,
                        &p_cell->src_cgi,
                        sizeof(p_new_node->data.cell_id));

                is_update = SON_TRUE;
            }
            /*SPR_19279_END*/


			/* Rel 3.0 Multi Sector code changes Start */
			if ((SON_CELL_CSG_IDENTITY_PRESENT & p_cell->bitmask)
					&& (SON_HOME_ENB == g_anr_context.own_enb_id.enb_type) &&
					(SON_MEMCMP(p_new_node->data.csg_identity,
						    p_cell->csg_identity,
						    SON_CSG_ID_OCTET_SIZE * sizeof(son_u8))))
				/* Rel 3.0 Multi Sector code changes Stop */
			{
				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_INFO,
						"CSG identity for [Cell Id: 0x%x] updated 0x%d -> 0x%x",
						son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity),
						son_convert_char_cell_id_to_int_cell_id(p_new_node->data.csg_identity),
						son_convert_char_cell_id_to_int_cell_id(p_cell->csg_identity));

				SON_MEMCPY(p_new_node->data.csg_identity,
						p_cell->csg_identity,
						SON_CSG_ID_OCTET_SIZE * sizeof(son_u8));
				p_new_node->data.is_csg_id_available = SON_TRUE;
				is_update = SON_TRUE;
			}

			/* Rel 3.0 Multi Sector code changes Start */
			if ((SON_CELL_CSG_IDENTITY_PRESENT & p_cell->bitmask) &&
					(SON_MACRO_ENB == g_anr_context.own_enb_id.enb_type))
				/* Rel 3.0 Multi Sector code changes Stop */
			{
				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_INFO,
						"CSG identity specified for a macro Cell"
						" [Cell Id: 0x%x]", son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));

				p_new_node->data.rach_related_info.is_cell_info_ind_valid_as_per_anr
					= SON_FALSE;

				p_new_node = SON_PNULL;
				SON_UT_TRACE_EXIT();
				return p_new_node;
			}

			if (p_new_node->data.pci_value != p_cell->pci)
			{
				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_INFO,
						"PCI for [Cell Id: 0x%x] updated %d -> %d",
						son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity),
						p_new_node->data.pci_value,
						p_cell->pci);

				/*SPR-12596-Start*/

				SON_MEMSET(&msg, 0, sizeof(son_anr_pci_conflict_confusion_ind_t));
				p_pci_list_node = anr_find_pci_in_conflict_confusion_pci_list(&p_new_node->data, p_new_node->data.pci_value);

				if((p_pci_list_node != SON_PNULL) && (SON_TRUE == p_pci_list_node->pci_data.is_in_conflict))
				{
					p_pci_list_node->pci_data.is_in_conflict = SON_FALSE;

					/*Send conflict clearance indication to OAM*/
					msg.src_cgi.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
					msg.cgi_1.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;

					SON_MEMCPY(&msg.src_cgi.intra_rat_global_cell_id,
							&p_new_node->data.cell_id,
							sizeof(son_intra_rat_global_cell_id_t));

					SON_MEMCPY(&msg.cgi_1.intra_rat_global_cell_id,
							&p_pci_list_node->pci_data.cgi_1,
							sizeof(son_intra_rat_global_cell_id_t));

					msg.pci = p_pci_list_node->pci_data.pci;
					msg.cause = SON_PCI_CONFLICT_CLEARED;

					son_create_send_buffer((son_void_t *)&msg, SON_ANR_MODULE_ID, SON_APPS_MODULE_ID,
							SONANR_PCI_CONFLICT_CONFUSION_IND, sizeof(son_anr_pci_conflict_confusion_ind_t));

					if(SON_FALSE == p_pci_list_node->pci_data.is_confused)
					{
						list_delete_node(&p_new_node->data.conflict_confusion_pci_list, &p_pci_list_node->anchor);
						son_mem_free(p_pci_list_node);
					}
				}

				/*SPR-12596-End*/

				p_new_node->data.pci_value = p_cell->pci;
				is_update = SON_TRUE;
				is_pci_earfcn_updated = SON_TRUE;
			}

			if ((p_cell->bitmask & SON_CELL_EUTRA_MODE_PRESENT) &&
					((p_cell->choice_eutra_mode.bitmask == SON_EUTRA_MODE_FDD) ||
					 (p_cell->choice_eutra_mode.bitmask == SON_EUTRA_MODE_TDD)) &&
					SON_MEMCMP(&p_new_node->data.choice_eutra_mode,
						&p_cell->choice_eutra_mode,
						sizeof(p_new_node->data.choice_eutra_mode)))
			{
				son_u32 prev_earfcn = anr_get_earfcn(&p_new_node->data);
				son_u32 new_earfcn   = 0;

				SON_MEMCPY(&p_new_node->data.choice_eutra_mode,
						&p_cell->choice_eutra_mode,
						sizeof(p_new_node->data.choice_eutra_mode));

				new_earfcn = anr_get_earfcn(&p_new_node->data);
				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_INFO,
						"EARFCN for [Cell Id: 0x%x] updated %u -> %u",
						son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity), prev_earfcn, new_earfcn);

				is_update = SON_TRUE;
				is_pci_earfcn_updated = SON_TRUE;
			}

			if ((p_cell->bitmask & SON_CELL_TAC_PRESENT) &&
					SON_MEMCMP(p_new_node->data.tac,
						p_cell->tac,
						sizeof(p_new_node->data.tac)))
			{
				son_u32 prev_tac = son_convert_char_cell_id_to_int_cell_id(p_new_node->data.tac);
				son_u32 new_tac  = son_convert_char_cell_id_to_int_cell_id(p_cell->tac);

				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_INFO,
						"TAC for [Cell Id: 0x%x] updated %d -> %d",
						son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity), prev_tac, new_tac);

				SON_MEMCPY(p_new_node->data.tac,
						p_cell->tac,
						sizeof(p_new_node->data.tac));
				is_update = SON_TRUE;
			}
            /*SPR_19279_START*/
			if ((p_cell->bitmask & SON_CELL_PLMN_PRESENT) &&
					(p_new_node->data.num_plmn_id != p_cell->num_plmn_id || 
                     SON_MEMCMP(&p_new_node->data.plmn_id,
                         &p_cell->plmn_id,
                         sizeof(p_new_node->data.plmn_id))))
            /*SPR_19279_END*/
			{
				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_INFO,
						"PLMN for Cell [Cell Id: 0x%x] updated",
						son_convert_char_cell_id_to_int_cell_id(p_cell->src_cgi.cell_identity));

				p_new_node->data.num_plmn_id = p_cell->num_plmn_id;
				SON_MEMCPY(p_new_node->data.plmn_id,
						p_cell->plmn_id,
						sizeof(p_new_node->data.plmn_id));
				is_update = SON_TRUE;
			}

			/*PRACH Configuration*/
			p_new_node->data.rach_related_info.is_old_prach_configuration_available
				= p_new_node->data.is_prach_configuration_available;

			SON_MEMCPY(&p_new_node->data.rach_related_info.old_prach_configuration,
					&p_new_node->data.prach_configuration,
					sizeof(p_new_node->data.rach_related_info.old_prach_configuration));
			p_new_node->data.rach_related_info.is_cell_info_ind_valid_as_per_anr
				= SON_TRUE;

			/*PRACH Configuration*/
			if(p_cell->bitmask & SON_CELL_PRACH_CONFIG_PRESENT)
			{
				if(SON_FALSE == p_new_node->data.is_prach_configuration_available)
				{
					p_new_node->data.is_prach_configuration_available = SON_TRUE;
					SON_MEMCPY(&p_new_node->data.prach_configuration,
							&p_cell->prach_config,
							sizeof(p_new_node->data.prach_configuration));
					is_update = SON_TRUE;
				}
				else
				{
					if((p_cell->prach_config.root_sequence_index != 
								p_new_node->data.prach_configuration.root_sequence_index)||
							(p_cell->prach_config.zero_correlation_zone_config != 
							 p_new_node->data.prach_configuration.zero_correlation_zone_config)||
							(p_cell->prach_config.high_speed_flag != 
							 p_new_node->data.prach_configuration.high_speed_flag)||
							(p_cell->prach_config.prach_frequency_offset != 
							 p_new_node->data.prach_configuration.prach_frequency_offset))
					{
						p_new_node->data.prach_configuration.root_sequence_index =
							p_cell->prach_config.root_sequence_index;
						p_new_node->data.prach_configuration.zero_correlation_zone_config =
							p_cell->prach_config.zero_correlation_zone_config;
						p_new_node->data.prach_configuration.high_speed_flag =
							p_cell->prach_config.high_speed_flag;
						p_new_node->data.prach_configuration.prach_frequency_offset =
							p_cell->prach_config.prach_frequency_offset;    
						is_update = SON_TRUE;
					}

					/* if bitmask value for PRACH configuration index is set and value is different */
					if((SON_PRACH_CONFIG_INDEX_PRESENT & p_cell->prach_config.bitmask) 
							&& (!(p_new_node->data.prach_configuration.bitmask &
									SON_PRACH_CONFIG_INDEX_PRESENT)||(p_cell->prach_config.
										prach_configuration_index != p_new_node->data.
										prach_configuration.prach_configuration_index)))
					{
						p_new_node->data.prach_configuration.prach_configuration_index = 
							p_cell->prach_config.prach_configuration_index;
						p_new_node->data.prach_configuration.bitmask |= SON_PRACH_CONFIG_INDEX_PRESENT;
						if(SON_EUTRA_MODE_TDD == p_new_node->data.choice_eutra_mode.bitmask)
						{
							is_update = SON_TRUE;
						}
					}
				}
			}


			/*either PCI or earfcn has changed and PCI mode is ON.
			 * Determine the PCI conflict and send the report to PCI Module*/
			if (SON_TRUE == is_pci_earfcn_updated &&
					SON_PCI_ENABLE ==  anr_get_pci_module_mode())
			{
                /* Spr 17753 Changes Start */
                anr_find_conflict_in_nrt_n_send_conflict_ind(&p_new_node->data);
                /* Spr 17753 Changes End */
			}

			if (SON_TRUE == is_update)
			{
				/* Rel 3.0 Multi Sector code changes Start */
				anr_update_serving_cell_as_nbr_autonomously(
						&p_cell->src_cgi);
				/* Rel 3.0 Multi Sector code changes Stop */
				anr_populate_send_x2_nrt_update_ind(&nrt_upd_ind,
						&p_cell->src_cgi, SON_NRT_OP_UPDATE);
			}

			p_new_node = SON_PNULL;
		}
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Cell ID is NULL");
	}

	SON_UT_TRACE_EXIT();
	return p_new_node;
} /* anr_add_new_cell_in_context */

/******************************************************************************
 * Function Name  : anr_clear_remove_list
 * Inputs         : p_list -   Pointer to the remove list
 * Returns        : son_void_t  - None
 * Description    : This function clears the entire Remove NR list for a
 *                  configured cell
 ******************************************************************************/
static son_void_t
anr_clear_remove_list
(
 SON_LIST *p_list
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = list_pop_node(p_list);

	while (p_list_node)
	{
		anr_nr_remove_list_node_t *p_data = YMEMBEROF(anr_nr_remove_list_node_t,
				nr_remove_list_node, p_list_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);

		p_list_node = list_pop_node(p_list);
	}

	SON_UT_TRACE_EXIT();
} /* anr_clear_remove_list */

/*SPR-9574-Start*/

/*****************************************************************************
 * Function Name  : anr_clear_intra_and_inter_freq_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : Delete all the frequencies from Inter and Intra freq lists
 ****************************************************************************/
son_void_t
anr_clear_intra_and_inter_freq_list
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_LIST_NODE            *p_intra_node        = SON_PNULL;
	SON_LIST_NODE            *p_inter_node        = SON_PNULL;
	anr_inter_freq_list_node_t *p_inter_list_node = SON_PNULL;
	anr_intra_freq_list_node_t *p_intra_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_intra_node = list_pop_node(&g_anr_context.intra_freq_list);

	while (SON_PNULL != p_intra_node)
	{
		p_intra_list_node =  YMEMBEROF(anr_intra_freq_list_node_t,
				anchor, p_intra_node);
		son_mem_free(p_intra_list_node);
		p_intra_node = list_pop_node(&g_anr_context.intra_freq_list);
	}

	p_inter_node = list_pop_node(&g_anr_context.inter_freq_list);

	while (SON_PNULL != p_inter_node)
	{
		p_inter_list_node =  YMEMBEROF(anr_inter_freq_list_node_t,
				anchor, p_inter_node);
		son_mem_free(p_inter_list_node);
		p_inter_node = list_pop_node(&g_anr_context.inter_freq_list);
	}

	SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
			SON_DETAILED,
			"Cleared Intra and Inter Freq lists");

	SON_UT_TRACE_EXIT();
} /* anr_clear_intra_and_inter_freq_list */

/*SPR-9574-End*/

/*****************************************************************************
 * Function Name  : anr_clear_context_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function deletes the cell contexts
 ******************************************************************************/
son_void_t
anr_clear_context_list
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_LIST_NODE             *p_node     = SON_PNULL;
	anr_cell_context_node_t *p_data     = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_node = list_pop_node(&g_anr_context.cell_ctxt_list);
	while (SON_PNULL != p_node)
	{
		p_data = YMEMBEROF(anr_cell_context_node_t, sNode, p_node);
		/* Clean up intra RAT hash table */
		anr_clear_nr_intra_rat_hash_table(&p_data->data.
				intra_rat_neighbor_table);
		/* Clean up inter RAT hash table */
		anr_clear_nr_inter_rat_hash_table(&p_data->data.
				inter_rat_neighbor_table);

		/* Clean up remove list list */
		anr_clear_remove_list(&p_data->data.nr_remove_list);

		/* Clean up transaction list */
		anr_clear_txn_id_list(&p_data->data.txn_id_list);

		/*Clean up broadcast statistics list*/
		anr_clear_bs_allowed_stats_list(
				&p_data->data.bs_allowed_stats_list);

		/*Clean up utran freq stats list*/
		anr_clear_utran_freq_stats_list(
				&p_data->data.utran_freq_fdd_stats_list);

		anr_clear_utran_freq_stats_list(
				&p_data->data.utran_freq_tdd_stats_list);

		/*Clean up geran freq stats list*/
		anr_clear_geran_freq_stats_list(
				&p_data->data.geran_freq_stats_list);

		/*Clean up cdma xrtt band/freq stats list*/
		anr_clear_cdma_xrtt_band_stats_list(
				&p_data->data.cdma_xrtt_band_stats_list);

		/*Clean up cdma xrtt band/freq stats list*/
		anr_clear_cdma_hrpd_band_stats_list(
				&p_data->data.cdma_hrpd_band_stats_list);

		/*SPR-9759-Start*/
		/*Clean up eutran_pci_requested_cgi_list*/
		anr_clear_eutran_cdma_pci_requested_cgi_list(
				&p_data->data.eutran_pci_requested_cgi_list);
		/*Clean up utran_pci_requested_cgi_list*/
		anr_clear_utran_pci_requested_cgi_list(
				&p_data->data.utran_pci_requested_cgi_list);
		/*Clean up geran_pci_requested_cgi_list*/
		anr_clear_geran_pci_requested_cgi_list(
				&p_data->data.geran_pci_requested_cgi_list);
		/*Clean up cdma_pci_requested_cgi_list*/
		anr_clear_eutran_cdma_pci_requested_cgi_list(
				&p_data->data.cdma_pci_requested_cgi_list);
		/*SPR-9759-End*/

		/*SPR-12596-Start*/
		anr_clear_conflict_confusion_pci_list(
				&p_data->data.conflict_confusion_pci_list);
		/*SPR-12596-End*/

		/*Clean up pci conflict confuison list*/
		anr_clear_pci_confl_conf_list(
				&p_data->data.pci_sus_conf_ctxt_list);

		/*Clean up non-extended RNC ID list*/
		anr_clear_rnc_id_list(
				&p_data->data.non_extended_rnc_id_list);

		/*Clean up extended RNC ID list*/
		anr_clear_rnc_id_list(
				&p_data->data.extended_rnc_id_list);

		son_mem_free(p_data);
		p_node = list_pop_node(&g_anr_context.cell_ctxt_list);
	}

	SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
			SON_BRIEF,
			"Cleared ANR Global Context Cell List");

	SON_UT_TRACE_EXIT();
} /* anr_clear_context_list */

/*****************************************************************************
 * Function Name  : anr_delete_cell_from_context
 * Inputs         : p_cell_id - Pointer to the cell Id
 * Outputs        : None
 * Returns        : SON_TRUE  :  if node deleted
 *                : SON_FALSE :  if node not deleted
 * Description    : This function deletes the specified cell context from the cell context list
 ******************************************************************************/
son_bool_et
anr_delete_cell_from_context
(
 son_intra_rat_global_cell_id_t     *p_cell_id
 )
{
	son_bool_et is_node_deleted = SON_FALSE;
	SON_LIST_NODE             *p_list_node     = SON_PNULL;
	anr_cell_context_node_t *p_data         = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_cell_id)
	{
		p_list_node = list_find_node(&g_anr_context.cell_ctxt_list,
				p_cell_id,
				son_key_of_anr, son_compare_cell_id);

		if (SON_PNULL != p_list_node)
		{
			p_data = (anr_cell_context_node_t *)
				YMEMBEROF(anr_cell_context_node_t, sNode, p_list_node);

			/* Clean up intra RAT hash table */
			anr_clear_nr_intra_rat_hash_table(&p_data->data.
					intra_rat_neighbor_table);
			/* Clean up inter RAT hash table */
			anr_clear_nr_inter_rat_hash_table(&p_data->data.
					inter_rat_neighbor_table);
			/* Clean up remove list */
			anr_clear_remove_list(&p_data->data.nr_remove_list);

			/* Clean up transaction list */
			anr_clear_txn_id_list(&p_data->data.txn_id_list);

			/*Clean up broadcast statistics list*/
			anr_clear_bs_allowed_stats_list(
					&p_data->data.bs_allowed_stats_list);

			/*Clean up utran freq stats list*/
			anr_clear_utran_freq_stats_list(
					&p_data->data.utran_freq_fdd_stats_list);

			anr_clear_utran_freq_stats_list(
					&p_data->data.utran_freq_tdd_stats_list);

			/*Clean up geran freq stats list*/
			anr_clear_geran_freq_stats_list(
					&p_data->data.geran_freq_stats_list);

			/*Clean up pci conflict confuison list*/
			anr_clear_pci_confl_conf_list(
					&p_data->data.pci_sus_conf_ctxt_list);

			/*Clean up non-extended RNC ID list*/
			anr_clear_rnc_id_list(
					&p_data->data.non_extended_rnc_id_list);

			/*Clean up extended RNC ID list*/
			anr_clear_rnc_id_list(
					&p_data->data.extended_rnc_id_list);

            /* Spr 17753 Changes Start */ 
            /* Clean Conflict data */ 
            anr_clean_second_degree_conflict_data
                    (&p_data->data);
            /* Spr 17753 Changes End */
			/* Remove entry from pci conflict detection table */

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_BRIEF,
					"The intra RAT, inter RAT "
					"hash table and the transcation Id list deleted for cell "
					"[Cell Id: 0x%x]",
					son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));

			list_delete_node(&g_anr_context.cell_ctxt_list,
					p_list_node);

			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_INFO,
					"Deleted cell [Cell Id: 0x%x] from ANR Global Context",
                    son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
			anr_print_global_context("anr_delete_cell_from_context");

			/* Free the memory occupied by the node */
			son_mem_free(p_data);
			is_node_deleted = SON_TRUE;
		}
		else
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_WARNING,
					"[Cell Id: 0x%x] could "
					"not be deleted as cell not found in ANR Cell Context ",
					son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
		}
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Cell ID is NULL");
	}

	SON_UT_TRACE_EXIT();
	return is_node_deleted;
} /* anr_delete_cell_from_context */

/*****************************************************************************
 * Function Name  : anr_get_global_mode
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_app_mode_et - current anr mode value
 * Description    :This function gets the anr mode
 ******************************************************************************/
son_app_mode_et
anr_get_global_mode
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_anr_context.anr_global_mode;
}

/*****************************************************************************
 * Function Name  : anr_set_global_mode
 * Inputs         : mode : anr mode value to be set
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function sets anr mode
 ******************************************************************************/
son_void_t
anr_set_global_mode
(
 son_app_mode_et mode
 )
{
	SON_UT_TRACE_ENTER();
	g_anr_context.anr_global_mode = mode;
	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_get_log_mode
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_oam_log_on_off_et - current log value
 * Description    : This function gets the  current log mode
 ******************************************************************************/
son_oam_log_on_off_et
anr_get_log_mode
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_anr_context.log_enable;
}

/*****************************************************************************
 * Function Name  : son_get_x2_enabled
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_bool_et - current x2 enabled value
 * Description    : This function gets the current x2 enabled value
 ******************************************************************************/
son_bool_et
son_get_x2_enabled
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_anr_context.x2_enabled;
}

/*****************************************************************************
 * Function Name  : anr_get_first_cell_ctxt
 * Inputs         : None
 * Outputs        : None
 * Returns        : anr_cell_context_node_t * - Pointer to cell context of first cell in list
 * Description    : This function gets the cell context of the first cell
 *                : in the context list
 ******************************************************************************/
anr_cell_context_node_t *
anr_get_first_cell_ctxt
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_LIST_NODE    *p_node    = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (list_count(&g_anr_context.cell_ctxt_list) > 0)
	{
		p_node = get_first_list_node(&g_anr_context.cell_ctxt_list);
		if (SON_PNULL != p_node)
		{
			SON_UT_TRACE_EXIT();
			return (anr_cell_context_node_t *)
				YMEMBEROF(anr_cell_context_node_t, sNode, p_node);
		}
	}

	SON_UT_TRACE_EXIT();
	return SON_PNULL;
} /* anr_get_first_cell_ctxt */

/*****************************************************************************
 * Function Name  : anr_get_next_node
 * Inputs         : p_node   Pointer to the current cell context
 * Outputs        : None
 * Returns        : anr_cell_context_node_t* -  cell context pointer of the next cell
 * Description    : This function gets the  next cell context in the list
 ******************************************************************************/
anr_cell_context_node_t *
anr_get_next_node
(
 anr_cell_context_node_t *p_node
 )
{
	SON_LIST_NODE    *p_list_node    = SON_PNULL;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL != p_node)
	{
		p_list_node = get_next_list_node(&p_node->sNode);
		if (SON_PNULL != p_list_node)
		{
			return (anr_cell_context_node_t *)
				YMEMBEROF(anr_cell_context_node_t, sNode, p_list_node);
		}
	}

	SON_UT_TRACE_EXIT();
	return SON_PNULL;
} /* anr_get_next_node */

/*******************************************************************************
 * Function Name  : anr_get_pending_res_cell_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_u32 - number of cells available in pending response
 * Description    : This function gets number of cells available in pending response
 ******************************************************************************/
son_u32
anr_get_pending_res_cell_count
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return list_count(&g_anr_pending_response.cell_sts_list);
}

/*******************************************************************************
 * Function Name  : anr_get_intra_rat_hash_rec_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : anr_hash_pool_t* - Pointer to the context pool
 * Description    : This function gets pointer to the context pool
 ******************************************************************************/
anr_hash_pool_t *
anr_get_intra_rat_hash_rec_pool
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return &g_anr_context.intra_rat_hash_rec_pool;
}

/******************************************************************************
 * Function Name  : anr_get_hash_node_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : anr_hash_pool_t* - Pointer to the table pool
 * Description    : This function gets pointer to the table pool
 ****************************************************************************/
anr_hash_pool_t *
anr_get_hash_node_pool
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return &g_anr_context.hash_node_pool;
}

/*******************************************************************************
 * Function Name  : anr_get_inter_rat_hash_rec_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : anr_hash_pool_t* - Pointer to the inter RAT context pool
 * Description    : This function gets pointer to the inter RAT context pool
 ******************************************************************************/
anr_hash_pool_t *
anr_get_inter_rat_hash_rec_pool
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return &g_anr_context.inter_rat_hash_rec_pool;
}

/*****************************************************************************
 * Function Name  : anr_get_pci_module_mode
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_pci_mode_et - son_pci_mode_et
 * Description    : This function returns the  pci mode
 ****************************************************************************/
son_pci_mode_et
anr_get_pci_module_mode
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_anr_context.pci_mode;
}

/*****************************************************************************
 * Function Name  : anr_set_pci_module_mode
 * Inputs         : pci_mode  Mode value to be set
 * Outputs        : None
 * Returns        : son_void_t  - None
 * Description    : This function sets the  pci mode
 ****************************************************************************/
son_void_t
anr_set_pci_module_mode
(
 son_pci_mode_et pci_mode
 )
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	g_anr_context.pci_mode = pci_mode;
}

/*****************************************************************************
 * Function Name  : anr_check_cell_state
 * Inputs         : cell_state  Expected RRM cell state
 * Outputs        :
 * Returns        : son_return_et - SON_SUCCESS / SON_FAILURE or SON_PARTIAL_SUCCESS
 * Description    : This function checks the fsm state of all cells in cell context
 ****************************************************************************/
son_return_et
anr_check_cell_state
(
 rrm_cell_operational_state_et cell_state
 )
{
	anr_cell_context_node_t   *p_ctxt_node = SON_PNULL;
	son_return_et ret  = SON_SUCCESS;
	son_oam_log_on_off_et log_mode = anr_get_log_mode();


	SON_UT_TRACE_ENTER();

	p_ctxt_node  = anr_get_first_cell_ctxt();

	if (SON_PNULL  == p_ctxt_node)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR, "ANR Cell context list is "
				"empty");
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	while (SON_PNULL != p_ctxt_node)
	{
		if (cell_state != p_ctxt_node->data.current_cell_state)
		{
			ret = SON_FAILURE;
			break;
		}

		p_ctxt_node = anr_get_next_node(p_ctxt_node);
	}

	if (SON_FAILURE  == ret)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR, "Some cells are not in [%d] "
				"state.",
				cell_state);
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* anr_check_cell_state */

/*****************************************************************************
 * Function Name  : anr_check_fsm_state
 * Inputs         : cell_state  Expected ANR cell state
 * Outputs        :
 * Returns        : son_return_et - SON_SUCCESS / SON_FAILURE or SON_PARTIAL_SUCCESS
 * Description    : This function checks the fsm state of all cells in cell context
 ****************************************************************************/
son_return_et
anr_check_fsm_state
(
 anr_cell_fsm_state_et cell_state
 )
{
	anr_cell_context_node_t   *p_ctxt_node = SON_PNULL;
	son_return_et ret  = SON_FAILURE;
	son_oam_log_on_off_et log_mode = anr_get_log_mode();


	SON_UT_TRACE_ENTER();

	p_ctxt_node  = anr_get_first_cell_ctxt();

	if (SON_PNULL  == p_ctxt_node)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR, "ANR Cell context list is "
				"empty");
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	while (SON_PNULL != p_ctxt_node)
	{
		if (cell_state == p_ctxt_node->data.current_cell_fsm_state)
		{
			ret = SON_SUCCESS;
			break;
		}

		p_ctxt_node = anr_get_next_node(p_ctxt_node);
	}

	if (SON_FAILURE  == ret)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR, "Some cells are not in [%s] "
				"state.",
				SON_ANR_CELL_FSM_STATES_NAMES[cell_state]);
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* anr_check_fsm_state */

/******************************************************************************
 * Function Name  : anr_clear_txn_id_list
 * Inputs         : p_list -   Pointer to the transaction Id list
 * Returns        : son_void_t - None
 * Description    : This function clears the entire transaction Id list for a
 *                  configured cell
 ******************************************************************************/
son_void_t
anr_clear_txn_id_list
(
 SON_LIST *p_list
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = list_pop_node(p_list);

	while (p_list_node)
	{
		anr_txn_id_node_t *p_data = YMEMBEROF(anr_txn_id_node_t,
				txn_list_node, p_list_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);

		p_list_node = list_pop_node(p_list);
	}

	SON_UT_TRACE_EXIT();
} /* anr_clear_txn_id_list */

/******************************************************************************
 * Function Name  : anr_clear_bs_allowed_stats_list
 * Inputs         : p_list -   Pointer to broadcast statistics  list
 * Output         : None
 * Returns        : son_void_t - None
 * Description    : This function clears the entire broadcast statistics
 *                   list for a  configured cell
 ******************************************************************************/
son_void_t
anr_clear_bs_allowed_stats_list
(
 SON_LIST *p_list
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = list_pop_node(p_list);

	while (p_list_node)
	{
		anr_bs_allowed_node_t *p_data = YMEMBEROF(anr_bs_allowed_node_t,
				bs_allowed_stats_list_node, p_list_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);

		p_list_node = list_pop_node(p_list);
	}

	SON_UT_TRACE_EXIT();
} /* anr_clear_bs_allowed_stats_list */

/******************************************************************************
 * Function Name  : anr_clear_utran_freq_stats_list
 * Inputs         : p_list -   Pointer to utran freq statistics list
 * Output         : None
 * Returns        : son_void_t - None
 * Description    : This function clears the entire utran freq statistics
 *                   list for a  configured cell
 ******************************************************************************/
son_void_t
anr_clear_utran_freq_stats_list
(
 SON_LIST *p_list
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = list_pop_node(p_list);

	while (p_list_node)
	{
		anr_utran_freq_stats_node_t *p_data = YMEMBEROF(anr_utran_freq_stats_node_t,
				anchor, p_list_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);

		p_list_node = list_pop_node(p_list);
	}

	SON_UT_TRACE_EXIT();
} /* anr_clear_utran_freq_stats_list */

/******************************************************************************
 * Function Name  : anr_clear_cdma_hrpd_band_stats_list
 * Inputs         : p_list -   Pointer to geran freq statistics list
 * Output         : None
 * Returns        : son_void_t - None
 * Description    : This function clears the entire cdma hrpd band statistics
 *                   list for a  configured cell
 ******************************************************************************/
son_void_t
anr_clear_cdma_hrpd_band_stats_list
(
 SON_LIST *p_list
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = list_pop_node(p_list);

	while (p_list_node)
	{
		anr_cdma_hrpd_band_stats_node_t *p_data = YMEMBEROF(anr_cdma_hrpd_band_stats_node_t,
				anchor, p_list_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);

		p_list_node = list_pop_node(p_list);
	}

	SON_UT_TRACE_EXIT();
} /* anr_clear_cdma_hrpd_band_stats_list */

/******************************************************************************
 * Function Name  : anr_clear_cdma_xrtt_band_stats_list
 * Inputs         : p_list -   Pointer to geran freq statistics list
 * Output         : None
 * Returns        : son_void_t - None
 * Description    : This function clears the entire cdma xrtt band statistics
 *                   list for a  configured cell
 ******************************************************************************/
son_void_t
anr_clear_cdma_xrtt_band_stats_list
(
 SON_LIST *p_list
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = list_pop_node(p_list);

	while (p_list_node)
	{
		anr_cdma_xrtt_band_stats_node_t *p_data = YMEMBEROF(anr_cdma_xrtt_band_stats_node_t,
				anchor, p_list_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);

		p_list_node = list_pop_node(p_list);
	}

	SON_UT_TRACE_EXIT();
} /* anr_clear_cdma_xrtt_band_stats_list */

/*SPR-9759-Start*/

/******************************************************************************
 * Function Name  : anr_clear_eutran_cdma_pci_requested_cgi_list
 * Inputs         : p_list -   Pointer to eutran_pci_requested_cgi_list
 * Output         : None
 * Returns        : son_void_t - None
 * Description    : This function clears the entire list of pci's for which cgi
 *                  has been requested
 ******************************************************************************/
son_void_t
anr_clear_eutran_cdma_pci_requested_cgi_list
(
 SON_LIST *p_list
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = list_pop_node(p_list);

	while (p_list_node)
	{
		anr_eutran_cdma_pci_value_node_t *p_data = YMEMBEROF(anr_eutran_cdma_pci_value_node_t,
				anchor, p_list_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);

		p_list_node = list_pop_node(p_list);
	}

	SON_UT_TRACE_EXIT();
} /* anr_clear_eutran_cdma_pci_requested_cgi_list */

/******************************************************************************
 * Function Name  : anr_clear_utran_pci_requested_cgi_list
 * Inputs         : p_list -   Pointer to utran_pci_requested_cgi_list
 * Output         : None
 * Returns        : son_void_t - None
 * Description    : This function clears the entire list of pci's for which cgi
 *                  has been requested
 ******************************************************************************/
son_void_t
anr_clear_utran_pci_requested_cgi_list
(
 SON_LIST *p_list
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = list_pop_node(p_list);

	while (p_list_node)
	{
		anr_utran_pci_value_node_t *p_data = YMEMBEROF(anr_utran_pci_value_node_t,
				anchor, p_list_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);

		p_list_node = list_pop_node(p_list);
	}

	SON_UT_TRACE_EXIT();
} /* anr_clear_utran_pci_requested_cgi_list */

/******************************************************************************
 * Function Name  : anr_clear_geran_pci_requested_cgi_list
 * Inputs         : p_list -   Pointer to geran_pci_requested_cgi_list
 * Output         : None
 * Returns        : son_void_t - None
 * Description    : This function clears the entire list of pci's for which cgi
 *                  has been requested
 ******************************************************************************/
son_void_t
anr_clear_geran_pci_requested_cgi_list
(
 SON_LIST *p_list
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = list_pop_node(p_list);

	while (p_list_node)
	{
		anr_geran_pci_value_node_t *p_data = YMEMBEROF(anr_geran_pci_value_node_t,
				anchor, p_list_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);

		p_list_node = list_pop_node(p_list);
	}

	SON_UT_TRACE_EXIT();
} /* anr_clear_geran_pci_requested_cgi_list */

/*SPR-9759-End*/

/*SPR-12596-Start*/

/******************************************************************************
 * Function Name  : anr_clear_conflict_confusion_pci_list
 * Inputs         : p_list -   Pointer to conflict confusion pci list
 * Output         : None
 * Returns        : son_void_t - None
 * Description    : This function clears the entire list of pci's involved in
 *                  conflict confusion
 ******************************************************************************/
son_void_t
anr_clear_conflict_confusion_pci_list
(
 SON_LIST *p_list
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = list_pop_node(p_list);

	while (p_list_node)
	{
		conflict_confusion_pci_list_node_t *p_data = YMEMBEROF(conflict_confusion_pci_list_node_t,
				anchor, p_list_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);

		p_list_node = list_pop_node(p_list);
	}

	SON_UT_TRACE_EXIT();
} /* anr_clear_conflict_confusion_pci_list */

/*SPR-12596-End*/

/******************************************************************************
 * Function Name  : anr_clear_geran_freq_stats_list
 * Inputs         : p_list -   Pointer to geran freq statistics list
 * Output         : None
 * Returns        : son_void_t - None
 * Description    : This function clears the entire geran freq statistics
 *                   list for a  configured cell
 ******************************************************************************/
son_void_t
anr_clear_geran_freq_stats_list
(
 SON_LIST *p_list
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = list_pop_node(p_list);

	while (p_list_node)
	{
		anr_geran_freq_stats_node_t *p_data = YMEMBEROF(anr_geran_freq_stats_node_t,
				anchor, p_list_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);

		p_list_node = list_pop_node(p_list);
	}

	SON_UT_TRACE_EXIT();
} /* anr_clear_geran_freq_stats_list */

/******************************************************************************
 * Function Name  : anr_clear_pci_confl_conf_list
 * Inputs         : p_list -   Pointer to the remove list
 * Output         : None
 * Returns        : son_void_t - None
 * Description    : This function clears the pci conflcit confusion list
 ******************************************************************************/
son_void_t
anr_clear_pci_confl_conf_list
(
 SON_LIST *p_list
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = list_pop_node(p_list);

	while (p_list_node)
	{
		anr_pci_sus_conf_node_t *p_data = YMEMBEROF(anr_pci_sus_conf_node_t,
				anchor, p_list_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);

		p_list_node = list_pop_node(p_list);
	}

	SON_UT_TRACE_EXIT();
} /* anr_clear_pci_confl_conf_list */

/*****************************************************************************
 * Function Name  : anr_get_pending_scan_res_count
 * Inputs         : None
 * Output         : None
 * Returns        : son_u8 - pending scan res count
 * Description    : Returns NMM Scan Response Count present in anr_global_context
 ****************************************************************************/
son_u8
anr_get_pending_scan_res_count
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_anr_context.pending_scan_res_count;
}

/*****************************************************************************
 * Function Name  : anr_increment_pending_scan_res_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : Increment Pending NMM Scan Response Count present in anr_global_context
 ****************************************************************************/
son_void_t
anr_increment_pending_scan_res_count
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	if (g_anr_context.pending_scan_res_count < MAX_PENDING_RES_COUNT)
	{
		g_anr_context.pending_scan_res_count++;
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Max value"
				"of Pending NMM Scan Response Count reached");
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_decrement_pending_scan_res_count
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : Decrement NMM Scan Response Count present in anr_global_context
 ****************************************************************************/
son_void_t
anr_decrement_pending_scan_res_count
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	if (g_anr_context.pending_scan_res_count > MIN_PENDING_RES_COUNT)
	{
		g_anr_context.pending_scan_res_count--;
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Min value of Pending"
				"NMM Scan Response Count "
				"reached");
	}

	SON_UT_TRACE_EXIT();
} /* anr_decrement_pending_scan_res_count */

/*****************************************************************************
 * Function Name  : anr_get_pending_scan_res_error_code
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_error_et - An enum that specifies type of causes
 * Description    : Gets Pending NMM Scan Response Error Code in anr_global_context
 ****************************************************************************/
son_error_et
anr_get_pending_scan_res_error_code
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_anr_context.pending_scan_res_error_code;
}

/*****************************************************************************
 * Function Name  : anr_get_pending_scan_res_error_code
 * Inputs         : son_error_et
 * Outputs        : None
 * Returns        : son_void_et - None
 * Description    : Sets Pending NMM Scan Response Error Code in anr_global_context
 ****************************************************************************/
son_void_t
anr_set_pending_scan_res_error_code
(
 son_error_et err
 )
{
	SON_UT_TRACE_ENTER();
	g_anr_context.pending_scan_res_error_code = err;
	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_pending_res_timer_handler
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_et - None
 * Description    : This function cleans up Pending Response Structure and sends
 *                : pending response to OAM
 ****************************************************************************/
son_void_t
anr_pending_res_timer_handler
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;
	son_anr_feature_state_change_ind_t state_change_ind;


	SON_MEMSET(&state_change_ind, 0, sizeof(son_oam_feature_state_change_ind_t));

	SON_UT_TRACE_ENTER();
	if ((api = anr_get_pending_res_api_id()) != SMIF_CELL_INFO_IND)
	{
		son_anr_enable_res_t res;
		SON_MEMSET(&res, 0, sizeof(son_anr_enable_res_t));

		/* send response for request
		 * maintained in pending_response */
		res.transaction_id = anr_get_pending_res_transaction_id();

		if (anr_get_pending_res_cell_count() > MIN_PENDING_RES_COUNT)
		{
			res.enable_res.cell_status_list_size =
				anr_get_pending_res_cell_count();
			anr_construct_cell_arr_from_pending_res_list(
					res.enable_res.cell_status_list);
		}
		else
		{
			res.enable_res.result = SON_SUCCESS;
			res.enable_res.error_code = SON_NO_ERROR;
		}

		son_create_send_buffer((son_u8 *)&res,
				SON_ANR_MODULE_ID,
				SON_MIF_MODULE_ID,
				api,
				sizeof(res));
	}

	if (anr_get_pending_res_ts_diff_from_curr_ts() >
			ANR_TIMEOUT_FOR_PENDING_RES)
	{
		anr_cell_context_node_t *p_ctxt_node = SON_PNULL;
		anr_cell_context_t  *p_cell_ctxt = SON_PNULL;
        /* SPR 20653 Fix Start */
		anr_cell_fsm_state_et new_state = ANR_CELL_STATE_MAX;
        /* SPR 20653 Fix End */
		p_ctxt_node = anr_get_first_cell_ctxt();
		while (SON_PNULL != p_ctxt_node)
		{
			p_cell_ctxt = &p_ctxt_node->data;
			if (anr_find_cell_in_pending_res(&p_cell_ctxt->cell_id))
			{
				new_state = p_cell_ctxt->previous_cell_fsm_state;
				SONANR_CELL_FSM_SET_STATE(p_cell_ctxt,
						p_cell_ctxt->previous_cell_fsm_state,
						p_cell_ctxt->current_cell_fsm_state,
						new_state);

				if ((api = anr_get_pending_res_api_id()) == SMIF_CELL_INFO_IND)
				{
					SON_MEMCPY(&state_change_ind.srv_cgi,
							&p_cell_ctxt->cell_id,
							sizeof(son_intra_rat_global_cell_id_t));

					state_change_ind.module_id = SON_ANR_MODULE_ID;
					state_change_ind.state = SON_STATE_DISABLED;
					state_change_ind.cause = SON_RRM_REGISTRATION_UNSUCCESSFUL;

					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_INFO,
							"Sending cell state change indication for"
							"[Cell Id: %d]",
							son_convert_char_cell_id_to_int_cell_id(state_change_ind.srv_cgi.cell_identity));

					son_create_send_buffer((son_u8 *)&state_change_ind,
							SON_ANR_MODULE_ID,
							SON_MIF_MODULE_ID,
							SONANR_FEATURE_STATE_CHANGE_IND,
							sizeof(state_change_ind));
				}
			}

			p_ctxt_node = anr_get_next_node(p_ctxt_node);
		}
	}

	SON_UT_TRACE_EXIT();
} /* anr_pending_res_timer_handler */

/*****************************************************************************
 * Function Name  : anr_get_x2_pending_res_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_LIST* - Pointer to X2 Pending Response List
 * Description    : This function returns Pointer to X2 Pending Response List
 ****************************************************************************/
SON_LIST *
anr_get_x2_pending_res_list
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return &g_anr_context.x2_pending_res_list;
}

/*****************************************************************************
 * Function Name  : anr_delete_from_txn_id_list
 * Inputs         : p_list  - Cell specfic NR Remove List
 *                  tid     - Transaction Id
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function deletes a TID from Transaction ID list
 *****************************************************************************/
son_void_t
anr_delete_from_txn_id_list
(
 SON_LIST *p_list,
 son_u16 tid
 )
{
	anr_txn_id_node_t        *p_node             = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_node = anr_find_txn_id_list(p_list, tid);

	if (SON_PNULL != p_node)
	{
		list_delete_node(p_list, &p_node->txn_list_node);

		son_mem_free(p_node);

		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_DETAILED,
				"Deleted Trans Id "
				"[Trans Id: %d] from Transaction ID List",
				tid);
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Node for Trans Id [Trans Id: %d]"
				" not found", tid);
	}

	SON_UT_TRACE_EXIT();
} /* anr_delete_from_txn_id_list */

/*****************************************************************************
 * Function Name  : anr_find_txn_id_list
 * Inputs         : p_list  - Cell specfic NR Remove List
 *                  tid     - Transaction Id
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function finds a TID from Transaction ID list
 *****************************************************************************/
anr_txn_id_node_t *
anr_find_txn_id_list
(
 SON_LIST *p_list,
 son_u16 tid
 )
{
	SON_LIST_NODE            *p_node             = SON_PNULL;
	anr_txn_id_node_t         *p_list_node         = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_node = list_find_node(p_list,
			(son_void_t *)&tid, anr_key_of_txn_id_list,
			anr_compare_txn_id);
	if (SON_PNULL != p_node)
	{
		p_list_node = YMEMBEROF(anr_txn_id_node_t,
				txn_list_node, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* anr_find_txn_id_list */

/*****************************************************************************
 * Function Name  : anr_insert_from_txn_id_list
 * Inputs         : p_list  - Cell specfic NR Remove List
 *                  tid     - Transaction Id
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function inserts a TID in Transaction ID list
 *****************************************************************************/
son_bool_et
anr_insert_in_txn_id_list
(
 SON_LIST *p_list,
 son_u16 tid
 )
{
	anr_txn_id_node_t *p_list_node = SON_PNULL;
	son_bool_et ret_val      = SON_FALSE;


	SON_UT_TRACE_ENTER();

	p_list_node = son_mem_get(sizeof(anr_txn_id_node_t));
	if (SON_PNULL != p_list_node)
	{
		p_list_node->txn_id = tid;

		list_insert_node(p_list,
				&p_list_node->txn_list_node);

		ret_val = SON_TRUE;

		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_DETAILED,
				"Added Transaction ID [Trans Id: %d]"
				" in Transaction Id List", tid);
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_insert_in_txn_id_list */

/*****************************************************************************
 * Function Name  : anr_find_cell_in_remove_list
 * Inputs         : p_list  - Cell specfic NR Remove List
 *                  cell_id - SON Global cell id
 * Outputs        : None
 * Returns        : anr_nr_remove_list_node_t* - Pointer to ANR Cell specific
 *                                               Remove list Node
 * Description    : This function finds a cell in nr remove list
 *****************************************************************************/
anr_nr_remove_list_node_t *
anr_find_cell_in_remove_list
(
 SON_LIST                *p_list,
 son_global_cell_id_t    *p_cell_id
 )
{
	SON_LIST_NODE               *p_node             = SON_PNULL;
	anr_nr_remove_list_node_t   *p_list_node        = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_node = list_find_node(p_list, (son_void_t *)p_cell_id,
			anr_key_of_nr_remove_list,
			anr_compare_nr_remove_list_node);

	if (SON_PNULL != p_node)
	{
		p_list_node = YMEMBEROF(anr_nr_remove_list_node_t,
				nr_remove_list_node, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* anr_find_cell_in_remove_list */

/*****************************************************************************
 * Function Name  : anr_insert_cell_in_remove_list
 * Inputs         : p_list  : Cell specfic NR Remove List
 *                  cell_id : SON Global cell id
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function inserts a cell in nr remove list
 *****************************************************************************/
son_bool_et
anr_insert_cell_in_remove_list
(
 SON_LIST                *p_list,
 son_global_cell_id_t    *p_cell_id
 )
{
	anr_nr_remove_list_node_t   *p_list_node    = SON_PNULL;
	son_bool_et ret_val         = SON_FALSE;


	SON_UT_TRACE_ENTER();

	p_list_node = (anr_nr_remove_list_node_t *)son_mem_get
		(sizeof(anr_nr_remove_list_node_t));

	if (SON_PNULL != p_list_node)
	{
		SON_MEMSET(p_list_node, 0, sizeof(anr_nr_remove_list_node_t));
		SON_MEMCPY(&p_list_node->cell_id,
				p_cell_id, sizeof(son_global_cell_id_t));
		list_insert_node(p_list, &p_list_node->nr_remove_list_node);

		ret_val      = SON_TRUE;
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Failed to get memory for "
				" storing cell id in Remove list");
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_insert_cell_in_remove_list */

/*****************************************************************************
 * Function Name  : anr_delete_cell_from_remove_list
 * Inputs         : p_list  - Cell specfic NR Remove List
 *                  cell_id - SON Global cell id
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function inserts a cell in nr remove list
 *****************************************************************************/
son_bool_et
anr_delete_cell_from_remove_list
(
 SON_LIST                *p_list,
 son_global_cell_id_t    *p_cell_id
 )
{
	anr_nr_remove_list_node_t   *p_list_node    = SON_PNULL;
	son_bool_et ret_val         = SON_FALSE;


	SON_UT_TRACE_ENTER();

	p_list_node = anr_find_cell_in_remove_list(
			p_list, p_cell_id);

	if (SON_PNULL != p_list_node)
	{
		list_delete_node(p_list, &p_list_node->nr_remove_list_node);
		son_mem_free(p_list_node);
		ret_val      = SON_TRUE;
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_delete_cell_from_remove_list */

/*****************************************************************************
 * Function Name  : anr_key_of_bs_allowed_stats_list
 * Inputs         : p_node - anchor field of the broadcast statistics list node
 * Outputs        : None
 * Returns        : son_void_t* - pointer to key field(earfcn)  as void pointer
 * Description    : This function determines the key field(earfcn) of
 *                  the broadcast statistics node in the list.
 ******************************************************************************/
const son_void_t *
anr_key_of_bs_allowed_stats_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_bs_allowed_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_bs_allowed_node_t,
			bs_allowed_stats_list_node, p_node);

	return (son_void_t *)(&p_data->bs_allowed_per_earfcn_record.earfcn);
}

/*****************************************************************************
 * Function Name  : anr_compare_bs_allowed_stats_list_node
 * Inputs         : p_val_1  - Pointer to the key field(earfcn)as void pointer
 *                : p_val_2  - Pointer to the key field(earfcn)as void pointer
 * Outputs        : None
 * Returns        : son_s32  - Zero If matches
 *                             Non Zero If not matches
 * Description    : This function compares the key field(earfcn) of the
 *                  broadcast statistics node in the list
 ******************************************************************************/
son_s32
anr_compare_bs_allowed_stats_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	son_u16 *p_earfcn_1 = SON_PNULL;
	son_u16 *p_earfcn_2 = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"anr_compare_bs_allowed_stats_list_node: Pointer to the cell Id is NULL");
	}
	else
	{
		p_earfcn_1 = (son_u16 *)p_val_1;
		p_earfcn_2 = (son_u16 *)p_val_2;
		if ((*p_earfcn_1) == (*p_earfcn_2))
		{
			ret_value = 0;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_bs_allowed_stats_list_node */

/*****************************************************************************
 * Function Name  : anr_find_record_in_bs_allowed_stats_list
 * Inputs         : p_list  - Pointer to broadcast allowed neighbor
 *                   statistics List
 *                  earfcn  - EARFCN value
 * Outputs        : None
 * Returns        : anr_bs_allowed_node_t* - Pointer to ANR EARFCN specific
 *                                           broadcasted neighbor count Node
 * Description    : This function finds the broadcast statistcs node for the
 *                   given earfcn value
 *****************************************************************************/
anr_bs_allowed_node_t *
anr_find_record_in_bs_allowed_stats_list
(
 SON_LIST                *p_list,
 son_u32 earfcn
 )
{
	SON_LIST_NODE               *p_node             = SON_PNULL;
	anr_bs_allowed_node_t       *p_list_node        = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_node = list_find_node(p_list, (son_void_t *)&earfcn,
			anr_key_of_bs_allowed_stats_list,
			anr_compare_bs_allowed_stats_list_node);

	if (SON_PNULL != p_node)
	{
		p_list_node = YMEMBEROF(anr_bs_allowed_node_t,
				bs_allowed_stats_list_node, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* anr_find_record_in_bs_allowed_stats_list */

/*****************************************************************************
 * Function Name  : anr_insert_record_in_bs_allowed_stats_list
 * Inputs         : p_list - Pointer to the Broadcast statistics list
 *                  earfcn - EARFCN value
 *                  is_ho_allowd_nbr - Bool value. Is  HO allowed to be incremented.
 *                  is_ho_blklst_nbr - Bool value. Is  HO blacklisted to be incremented.
 *                  broadcast_status
 * Outputs        : None
 * Returns        : anr_bs_allowed_node_t* - Pointer to ANR EARFCN specific
 *                                           broadcasted neighbor count Node
 * Description    : This function inserts a Broadcast statistics record in the
 *                  List
 **********************************************************************/
anr_bs_allowed_node_t *
anr_insert_record_in_bs_allowed_stats_list
(
 SON_LIST                *p_list,
 son_u32 earfcn,
 son_bool_et broadcast_status,
 son_bool_et is_ho_allowd_nbr,
 son_bool_et is_ho_blklst_nbr
 )
{
	anr_bs_allowed_node_t   *p_list_node      = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = (anr_bs_allowed_node_t *)son_mem_get
		(sizeof(anr_bs_allowed_node_t));

	if (SON_PNULL != p_list_node)
	{
		SON_MEMSET(p_list_node, 0, sizeof(anr_bs_allowed_node_t));
		p_list_node->bs_allowed_per_earfcn_record.earfcn =
			earfcn;
		if (SON_TRUE ==  broadcast_status)
		{
			if (SON_TRUE == is_ho_allowd_nbr)
			{
				p_list_node->bs_allowed_per_earfcn_record.
					bs_allowed_ho_allowed_nbrs_count++;
			}

			if (SON_TRUE == is_ho_blklst_nbr)
			{
				p_list_node->bs_allowed_per_earfcn_record.
					bs_allowed_ho_blklisted_nbrs_count++;
			}
		}
		else
		{
			if (SON_TRUE == is_ho_allowd_nbr)
			{
				p_list_node->bs_allowed_per_earfcn_record.
					bs_not_allowed_ho_allowed_nbrs_count++;
			}

			if (SON_TRUE == is_ho_blklst_nbr)
			{
				p_list_node->bs_allowed_per_earfcn_record.
					bs_not_allowed_ho_blklisted_nbrs_count++;
			}
		}

		list_insert_node(p_list, &p_list_node->bs_allowed_stats_list_node);
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Failed to get memory for storing the broadcast statistics");
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* anr_insert_record_in_bs_allowed_stats_list */

/*****************************************************************************
 * Function Name  : anr_delete_record_from_bs_allowed_stats_list
 * Inputs         : p_list  - Cell specfic NR Remove List
 *                  cell_id - SON Global cell id
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function deletes the record from the broadcast
 *                   Statistics list
 *****************************************************************************/
son_bool_et
anr_delete_record_from_bs_allowed_stats_list
(
 SON_LIST                *p_list,
 son_u32 earfcn
 )
{
	anr_bs_allowed_node_t      *p_list_node    = SON_PNULL;
	son_bool_et ret_val         = SON_FALSE;


	SON_UT_TRACE_ENTER();

	p_list_node = anr_find_record_in_bs_allowed_stats_list(
			p_list, earfcn);

	if (SON_PNULL != p_list_node)
	{
		if (0 == (p_list_node->bs_allowed_per_earfcn_record.bs_allowed_ho_allowed_nbrs_count +
					p_list_node->bs_allowed_per_earfcn_record.bs_allowed_ho_blklisted_nbrs_count +
					p_list_node->bs_allowed_per_earfcn_record.bs_not_allowed_ho_allowed_nbrs_count +
					p_list_node->bs_allowed_per_earfcn_record.bs_not_allowed_ho_blklisted_nbrs_count))
		{
			list_delete_node(p_list, &p_list_node->bs_allowed_stats_list_node);
			son_mem_free(p_list_node);
			ret_val      = SON_TRUE;

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"Broadcast statistics node deleted");
		}
		else
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"Broadcast statistics node can not be deleted");
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_delete_record_from_bs_allowed_stats_list */

/*****************************************************************************
 * Function Name  : increment_bs_allowed_nbr_count
 * Inputs         : p_cell_ctxt - Pointer to serving cell context
 *                : p_list_node - Pinter to the broadcast statistics node
 *                : is_ho_allowed - Bool value. Is HO Allowed count
 *                                   to be incremented
 *                : is_ho_blklsted - Bool value.Is HO blacklisted count
 *                                   to be incremaented
 *                : broadcast_status
 * Outputs        : None
 * Returns        : son_bool_et - SON_FALSE/SON_TRUE
 * Description    : This function increments the count of neighbor in  broadcast
 *                   Statistics Record
 *****************************************************************************/
son_bool_et
increment_bs_allowed_nbr_count
(
 anr_cell_context_t    *p_cell_ctxt,
 anr_bs_allowed_node_t *p_list_node,
 son_bool_et broadcast_status,
 son_bool_et is_ho_allowed,
 son_bool_et is_ho_blklsted
 )
{
	son_bool_et ret_val         = SON_FALSE;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_list_node ||
			(1 != (is_ho_allowed + is_ho_blklsted)))
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Invalid data as"
				" either p_cell_ctxt,p_list_node is SON_PNULL"
				"or is_ho_allowed[%d]  and "
				"is_ho_blklsted[%d] are having wrong values",
				is_ho_allowed, is_ho_blklsted);
		return SON_FALSE;
	}

	do
	{
		if (SON_TRUE == is_ho_allowed)
		{
			if (SON_TRUE == broadcast_status)
			{
				if (SON_RRM_MAX_INTRA_RAT_BROADCAST_HO_ALLOWED_NBRS >
						p_list_node->bs_allowed_per_earfcn_record.
						bs_allowed_ho_allowed_nbrs_count)
				{
					p_list_node->bs_allowed_per_earfcn_record.bs_allowed_ho_allowed_nbrs_count++;
					ret_val = SON_TRUE;
					break;
				}
			}
			else
			{
				if (SON_RRM_MAX_INTRA_RAT_NON_BROADCAST_HO_ALLOWED_NBRS >
						p_list_node->bs_allowed_per_earfcn_record.
						bs_not_allowed_ho_allowed_nbrs_count)
				{
					p_list_node->bs_allowed_per_earfcn_record.bs_not_allowed_ho_allowed_nbrs_count++;
					ret_val = SON_TRUE;
					break;
				}
			}

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_DETAILED,
					"Candidate Neighbors broadcast status is[%d]."
					"Increament not possible as"
					" [%d]is number of HO allowed neighbor cells send to RRM"
					" with Broadcast status TRUE and [%d] is the number of HO"
					"allowed neigbor cell send to RRM with Broadcast status"
					"FALSE  under EARFCN[%d] for serving cell [Cell_id:0x%x]",
					broadcast_status,
					p_list_node->bs_allowed_per_earfcn_record.bs_allowed_ho_allowed_nbrs_count,
					p_list_node->bs_allowed_per_earfcn_record.bs_not_allowed_ho_allowed_nbrs_count,
					p_list_node->bs_allowed_per_earfcn_record.earfcn,
					son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
			break;
		}

		if (SON_TRUE == is_ho_blklsted)
		{
			if (SON_TRUE == broadcast_status)
			{
				if (SON_RRM_MAX_INTRA_RAT_BROADCAST_HO_BLKLSTED_NBRS >
						p_list_node->bs_allowed_per_earfcn_record.
						bs_allowed_ho_blklisted_nbrs_count)
				{
					p_list_node->bs_allowed_per_earfcn_record.bs_allowed_ho_blklisted_nbrs_count++;
					ret_val = SON_TRUE;
					break;
				}
			}
			else
			{
				if (SON_RRM_MAX_INTRA_RAT_NON_BROADCAST_HO_BLKLSTED_NBRS >
						p_list_node->bs_allowed_per_earfcn_record.
						bs_not_allowed_ho_blklisted_nbrs_count)
				{
					p_list_node->bs_allowed_per_earfcn_record.bs_not_allowed_ho_blklisted_nbrs_count++;
					ret_val = SON_TRUE;
					break;
				}
			}

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_DETAILED,
					"Candidate Neighbors broadcast status is[%d]."
					"Increament not possible as"
					" [%d]is number of HO Blacklisted neighbor cells send to RRM with "
					"Broadcast status TRUE and [%d] is the number of HO allowed neigbor "
					"cell send to RRM with Broadcast status FALSE   under EARFCN[%d] for "
					"serving cell [Cell_id:0x%x]",
					broadcast_status,
					p_list_node->bs_allowed_per_earfcn_record.bs_allowed_ho_blklisted_nbrs_count,
					p_list_node->bs_allowed_per_earfcn_record.bs_not_allowed_ho_blklisted_nbrs_count,
					p_list_node->bs_allowed_per_earfcn_record.earfcn,
					son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
			break;
		}
	} while (0);
	SON_UT_TRACE_EXIT();
	return ret_val;
} /* increment_bs_allowed_nbr_count */

/*****************************************************************************
 * Function Name  : decrement_bs_allowed_nbr_count
 * Inputs         : p_cell_ctxt - Pointer to serving cell context
 *                : p_list_node - Pointer to the broadcast statistics node
 *                : is_ho_allowed - Bool value.Is HO Allowed count
 *                                   to be incremented
 *                : is_ho_blklisted - Bool value.Is HO blacklisted count
 *                                     to be incremented
 *                : broadcast_status
 * Outputs        : None
 * Returns        : son_bool_et  -  SON_FALSE/SON_TRUE
 * Description    : This function decrements the count of neighbor in  broadcast
 *                   Statistics Record
 *****************************************************************************/
son_bool_et
decrement_bs_allowed_nbr_count
(
 anr_cell_context_t    *p_cell_ctxt,
 anr_bs_allowed_node_t *p_list_node,
 son_bool_et broadcast_status,
 son_bool_et is_ho_allowed,
 son_bool_et is_ho_blklisted
 )
{
	son_bool_et ret_val         = SON_FALSE;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_list_node ||
			(1 != (is_ho_allowed + is_ho_blklisted)))
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Invalid data as"
				" either p_cell_ctxt,p_list_node is SON_PNULL"
				"or is_ho_allowed[val:%d]  and "
				"is_ho_blklisted[val:%d] are having wrong values",
				is_ho_allowed, is_ho_blklisted);
		return SON_FALSE;
	}

	do
	{
		if (SON_TRUE == is_ho_allowed)
		{
			if (SON_TRUE == broadcast_status)
			{
				if (0 <  p_list_node->bs_allowed_per_earfcn_record.
						bs_allowed_ho_allowed_nbrs_count)
				{
					p_list_node->bs_allowed_per_earfcn_record.bs_allowed_ho_allowed_nbrs_count--;
					ret_val = SON_TRUE;
					break;
				}
			}
			else
			{
				if (0 <  p_list_node->bs_allowed_per_earfcn_record.
						bs_not_allowed_ho_allowed_nbrs_count)
				{
					p_list_node->bs_allowed_per_earfcn_record.bs_not_allowed_ho_allowed_nbrs_count--;
					ret_val = SON_TRUE;
					break;
				}
			}

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_DETAILED,
					"Candidate neighbors broadcast status is [%d]"
					"Decrement not possible as"
					"[%d]is number of HO allowed neighbor cells send to RRM"
					"with Broadcast status TRUE  and [%d]is number of HO allowed "
					"neighbor cells send to RRM with Broadcast status FALSE under EARFCN[%d] for "
					"serving cell [Cell_id:0x%x]",
					broadcast_status,
					p_list_node->bs_allowed_per_earfcn_record.bs_allowed_ho_allowed_nbrs_count,
					p_list_node->bs_allowed_per_earfcn_record.bs_not_allowed_ho_allowed_nbrs_count,
					p_list_node->bs_allowed_per_earfcn_record.earfcn,
					son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
			break;
		}

		if (SON_TRUE == is_ho_blklisted)
		{
			if (SON_TRUE == broadcast_status)
			{
				if (0 <  p_list_node->bs_allowed_per_earfcn_record.
						bs_allowed_ho_blklisted_nbrs_count)
				{
					p_list_node->bs_allowed_per_earfcn_record.bs_allowed_ho_blklisted_nbrs_count--;
					ret_val = SON_TRUE;
					break;
				}
			}
			else
			{
				if (0 <  p_list_node->bs_allowed_per_earfcn_record.
						bs_not_allowed_ho_blklisted_nbrs_count)
				{
					p_list_node->bs_allowed_per_earfcn_record.bs_not_allowed_ho_blklisted_nbrs_count--;
					ret_val = SON_TRUE;
					break;
				}
			}

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_DETAILED,
					"Candidate neighbors broadcast status is [%d]."
					"Decreament not possible as"
					"[%d]is number of HO Blacklisted neighbor cells send to RRM with "
					"Broadcast status TRUE and [%d]is number of HO Blacklisted neighbor cells"
					"send to RRM with  Broadcast status FALSE under EARFCN[%d] for "
					"serving cell [Cell_id:0x%x]",
					broadcast_status,
					p_list_node->bs_allowed_per_earfcn_record.bs_allowed_ho_blklisted_nbrs_count,
					p_list_node->bs_allowed_per_earfcn_record.bs_not_allowed_ho_blklisted_nbrs_count,
					p_list_node->bs_allowed_per_earfcn_record.earfcn,
					son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
			break;
		}
	} while (0);

	/*The check is redundant,but effective as log from the
	 * delete function will not appear over and over*/
	if (SON_TRUE == ret_val &&
			(0 == p_list_node->bs_allowed_per_earfcn_record.bs_allowed_ho_blklisted_nbrs_count +
			 p_list_node->bs_allowed_per_earfcn_record.bs_allowed_ho_allowed_nbrs_count        +
			 p_list_node->bs_allowed_per_earfcn_record.bs_not_allowed_ho_blklisted_nbrs_count  +
			 p_list_node->bs_allowed_per_earfcn_record.bs_not_allowed_ho_allowed_nbrs_count))
	{
		anr_delete_record_from_bs_allowed_stats_list(
				&p_cell_ctxt->bs_allowed_stats_list,
				p_list_node->bs_allowed_per_earfcn_record.earfcn);
	}

	/*Making the pointer NULL delibrately,
	 * as deletion might have happend*/
    p_list_node = SON_PNULL;

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* decrement_bs_allowed_nbr_count */

/*****************************************************************************
 * Function Name  : anr_stop_timer
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function stop ANR Timer
 *****************************************************************************/
son_void_t
anr_stop_timer
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	if (g_anr_context.anr_timer_id)
	{
		son_stop_timer(g_anr_context.anr_timer_id);
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_get_earfcn
 * Inputs         : p_cell_ctxt - Pointer to cell context
 * Outputs        : None
 * Returns        : son_u16 - Returns earfcn
 * Description    : This function returns earfcn of a cell
 *****************************************************************************/
son_u32
anr_get_earfcn
(
 anr_cell_context_t *p_cell_ctxt
 )
{
	son_u32 earfcn = 0;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL != p_cell_ctxt)
	{
		if (p_cell_ctxt->choice_eutra_mode.bitmask & SON_EUTRA_MODE_FDD)
		{
			earfcn = p_cell_ctxt->choice_eutra_mode.fdd_info.dl_earfcn;
		}

		if (p_cell_ctxt->choice_eutra_mode.bitmask & SON_EUTRA_MODE_TDD)
		{
			earfcn = p_cell_ctxt->choice_eutra_mode.tdd_info.earfcn;
		}
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Invalid Params: p_cell_ctxt is NULL");
	}

	SON_UT_TRACE_EXIT();
	return earfcn;
} /* anr_get_earfcn */

/*Code for RACH coptmizaton:starts*/

/*****************************************************************************
 * Function Name  : anr_get_ul_earfcn
 * Inputs         : p_cell_ctxt - Pointer to cell context
 * Outputs        : None
 * Returns        : son_u16 - Returns uplink earfcn
 * Description    : This function returns uplink earfcn of a cell
 *****************************************************************************/
son_u32
anr_get_ul_earfcn
(
 anr_cell_context_t *p_cell_ctxt
 )
{
	son_u32 earfcn = 0;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL != p_cell_ctxt)
	{
		if (p_cell_ctxt->choice_eutra_mode.bitmask & SON_EUTRA_MODE_FDD)
		{
			earfcn = p_cell_ctxt->choice_eutra_mode.fdd_info.ul_earfcn;
		}

		if (p_cell_ctxt->choice_eutra_mode.bitmask & SON_EUTRA_MODE_TDD)
		{
			earfcn = p_cell_ctxt->choice_eutra_mode.tdd_info.earfcn;
		}
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Invalid Params: p_cell_ctxt is NULL");
	}

	SON_UT_TRACE_EXIT();
	return earfcn;
} /* anr_get_ul_earfcn */

/*****************************************************************************
 * Function Name  : anr_get_duplex_mode_of_serving_cell
 * Inputs         : p_cell_ctxt - Pointer to cell context
 * Outputs        : None
 * Returns        : son_duplex_mode_et - Duplex mode of the serving cell
 * Description    : This function returns duplex mode of the serving cell
 *****************************************************************************/
son_duplex_mode_et
anr_get_duplex_mode_of_serving_cell
(
 anr_cell_context_t *p_cell_ctxt
 )
{
	son_duplex_mode_et  mode = SON_INVALID_MODE;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL != p_cell_ctxt)
	{
		if (p_cell_ctxt->choice_eutra_mode.bitmask & SON_EUTRA_MODE_FDD)
		{
			mode = SON_FDD_MODE;
		}

		if (p_cell_ctxt->choice_eutra_mode.bitmask & SON_EUTRA_MODE_TDD)
		{
			mode = SON_TDD_MODE;
		}
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Invalid Params:"
				"p_cell_ctxt is NULL");
	}

	SON_UT_TRACE_EXIT();
	return mode;
} /* anr_get_duplex_mode_of_serving_cell */
/*Code for RACH coptmizaton:starts*/
/*****************************************************************************
 * Function Name  : is_cgi_matches_serving_cell
 * Inputs         : p_cell
 * Outputs        : None
 * Returns        : son_bool_et
 * Description    : This function determines if the CGI of the cell is equal to the CGI of the serving cell 
 ******************************************************************************/
son_bool_et
is_cgi_matches_serving_cell
(
 son_intra_rat_global_cell_id_t *p_cell
 )
{
	anr_cell_context_node_t *p_node = SON_PNULL;

	son_bool_et ret_val = SON_FALSE;


	SON_UT_TRACE_ENTER();
	p_node = anr_get_first_cell_ctxt();
	while (SON_PNULL != p_node)
	{
		if (!SON_MEMCMP(&p_node->data.cell_id, p_cell, sizeof(son_intra_rat_global_cell_id_t)))
		{
			ret_val = SON_TRUE;
			break;
		}

		p_node = anr_get_next_node(p_node);
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* is_cgi_matches_serving_cell */

/******************************************************************************
 * Function Name  : anr_get_anr_mode_for_cell
 * Inputs         : p_cell_ctxt     Pointer to ANR cell context
 * Outputs        : None
 * Returns        : anr_cell_fsm_state_et - ANR mode
 * Description    : This function returns the ANR mode for the given cell.
 ****************************************************************************/
anr_cell_fsm_state_et
anr_get_anr_mode_for_cell
(
 anr_cell_context_t *p_cell_ctxt
 )
{
    /* SPR 20653 Fix Start */
	anr_cell_fsm_state_et anr_mode = ANR_CELL_STATE_MAX;
    /* SPR 20653 Fix End */


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"anr_get_cell_context_from_global_context: Pointer to cell"
				" context is NULL");
	}
	else
	{
		anr_mode = p_cell_ctxt->current_cell_fsm_state;
	}

	SON_UT_TRACE_EXIT();
	return anr_mode;
} /* anr_get_anr_mode_for_cell */

/*PCI CONFUSION_SUSPECTED CHANGES:START*/

/*****************************************************************************
 * Function Name  : anr_print_pci_ctxt_from_sus_pci_ctxt_list
 * Inputs         : p_list_node  : Cell context node
 * Outputs        : None
 * Returns        : son_bool_et - SON_FALSE / SON_TRUE
 * Description    : This function  prints the PCI context
 *****************************************************************************/
son_void_t
anr_print_pci_ctxt_from_sus_pci_ctxt_list
(
 anr_pci_sus_conf_node_t   *p_list_node
 )
{
	SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
			SON_BRIEF,
			"PCI context"
			"[PCI %d EARFCN %d Cell Count %d state %d ask_cgi_counter %d ask_cgi_thx_val %d]",
			p_list_node->pci_sus_conf_data.pci,
			p_list_node->pci_sus_conf_data.earfcn,
			p_list_node->pci_sus_conf_data.nbr_cell_count,
			p_list_node->pci_sus_conf_data.state,
			p_list_node->pci_sus_conf_data.ask_cgi_counter,
			p_list_node->pci_sus_conf_data.ask_cgi_thx_val);
}

/*****************************************************************************
 * Function Name  : anr_key_of_sus_pci_list
 * Inputs         : p_node - anchor field of the suspected pci context record
 * Outputs        : None
 * Returns        : son_void_t* -  pointer to pci context as void pointer
 * Description    : This function determines the PCI context
 ******************************************************************************/
const son_void_t *
anr_key_of_sus_pci_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_pci_sus_conf_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_pci_sus_conf_node_t, anchor, p_node);

	return (son_void_t *)(&p_data->pci_sus_conf_data);
}

/*****************************************************************************
 * Function Name  : anr_compare_sus_pci_ctxt_list_node
 * Inputs         : p_val_1     Pointer to the son_pci_ctxt_t structure as
 *                              void pointer
 *                : p_val_2     Pointer to the son_pci_ctxt_t structure as
 *                              void pointer
 * Outputs        : None
 * Returns        : son_s32    - Zero If matches
 *                               Non Zero If not matches
 * Description    : This function compares the pci and earfcn values
 ******************************************************************************/
son_s32
anr_compare_sus_pci_ctxt_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	anr_pci_sus_conf_data_t *p_ctxt_1  = (anr_pci_sus_conf_data_t *)p_val_1;
	anr_pci_sus_conf_data_t *p_ctxt_2  = (anr_pci_sus_conf_data_t *)p_val_2;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_ctxt_1 || SON_PNULL == p_ctxt_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointer to the pci context node is NULL");
	}
	else
	{
		if (p_ctxt_1->pci == p_ctxt_2->pci && p_ctxt_1->earfcn == p_ctxt_2->earfcn)
		{
			ret_value = 0;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_sus_pci_ctxt_list_node */

/*****************************************************************************
 * Function Name  : anr_insert_in_sus_pci_ctxt_list
 * Inputs         : p_cell_ctxt     - Cell context
 *                : pci             - PCI value
 *                : earfcn          - EARFCN
 *                : state           - Suspected/Confused
 *                : nbr_cell_count  - Nbr cell count
 * Outputs        : None
 * Returns        : anr_pci_sus_conf_node_t * -  Pointer to ANR Cell specific
 *                                               transaction Id list Node
 * Description    : This function inserts a PCI context in suspected PCI ctxt list
 *****************************************************************************/
anr_pci_sus_conf_node_t *
anr_insert_in_sus_pci_ctxt_list
(
 anr_cell_context_t *p_cell_ctxt,
 son_u16 pci,
 son_u32 earfcn,
 son_pci_state_et state,
 son_u32 nbr_cell_count
 )
{
	anr_pci_sus_conf_node_t   *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = son_mem_get(sizeof(anr_pci_sus_conf_node_t));
	if (SON_PNULL != p_list_node)
	{
		p_list_node->pci_sus_conf_data.pci             = pci;
		p_list_node->pci_sus_conf_data.earfcn          = earfcn;
		p_list_node->pci_sus_conf_data.state           = state;
		p_list_node->pci_sus_conf_data.nbr_cell_count  = nbr_cell_count;

		p_list_node->pci_sus_conf_data.ask_cgi_counter = p_cell_ctxt->pci_confusion_cfg.
			clear_suspect_pci_threshold;
		p_list_node->pci_sus_conf_data.ask_cgi_thx_val = p_cell_ctxt->pci_confusion_cfg.
			clear_suspect_pci_threshold;

		list_insert_node(&p_cell_ctxt->pci_sus_conf_ctxt_list,
				&p_list_node->anchor);

		anr_print_pci_ctxt_from_sus_pci_ctxt_list(p_list_node);
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_INFO,
				"Marked [PCI: %d EARFCN:%u] "
				" as %s for serving [CellId: 0x%x]", pci, earfcn,
				(0 == state) ? "confused" : "suspected", son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* anr_insert_in_sus_pci_ctxt_list */

/*****************************************************************************
 * Function Name  : anr_find_in_sus_pci_ctxt_list
 * Inputs         : p_cell_ctxt  - Pointer to the cell context
 *                 : pci          - PCI value
 *                 : earfcn       - EARFCN value
 * Outputs        : None
 * Returns        : anr_pci_sus_conf_node_t * - Pointer to ANR Cell specific
 *                                               transaction Id list Node
 * Description    : This function finds a pci context in the suspected PCI ctxt list
 *****************************************************************************/
anr_pci_sus_conf_node_t *
anr_find_in_sus_pci_ctxt_list
(
 anr_cell_context_t *p_cell_ctxt,
 son_u16 pci,
 son_u32 earfcn
 )
{
	SON_LIST_NODE                        *p_node             = SON_PNULL;
	anr_pci_sus_conf_node_t              *p_list_node        = SON_PNULL;
	anr_pci_sus_conf_data_t pci_ctxt;


	SON_UT_TRACE_ENTER();
	SON_MEMSET(&pci_ctxt, 0, sizeof(anr_pci_sus_conf_data_t));
	pci_ctxt.pci    = pci;
	pci_ctxt.earfcn = earfcn;

	p_node = list_find_node(&p_cell_ctxt->pci_sus_conf_ctxt_list, (son_void_t *)&pci_ctxt,
			anr_key_of_sus_pci_list,
			anr_compare_sus_pci_ctxt_list_node);

	if (SON_PNULL != p_node)
	{
		p_list_node = YMEMBEROF(anr_pci_sus_conf_node_t, anchor, p_node);
		anr_print_pci_ctxt_from_sus_pci_ctxt_list(p_list_node);
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* anr_find_in_sus_pci_ctxt_list */

/*****************************************************************************
 * Function Name  : anr_delete_pci_ctxt_from_sus_pci_ctxt_list
 * Inputs         : p_cell_ctxt  - Cell context node
 *                  pci          - pci value
 *                   earfcn       - EARFCN value
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function deletes a pci context from
 *                   suspected PCI context list
 *****************************************************************************/
son_bool_et
anr_delete_pci_ctxt_from_sus_pci_ctxt_list
(
 anr_cell_context_t *p_cell_ctxt,
 son_u16 pci,
 son_u32 earfcn
 )
{
	anr_pci_sus_conf_node_t   *p_list_node    = SON_PNULL;
	son_bool_et ret_val         = SON_FALSE;


	SON_UT_TRACE_ENTER();

	p_list_node = anr_find_in_sus_pci_ctxt_list(
			p_cell_ctxt, pci, earfcn);
	if (SON_PNULL == p_list_node)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
				"Suspected PCI context"
				"[PCI %d EARFCN %u is not present for [Cell id: 0x%x]]",
				pci, earfcn, son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
		return ret_val;
	}

	anr_print_pci_ctxt_from_sus_pci_ctxt_list(p_list_node);
	if (2 >= p_list_node->pci_sus_conf_data.nbr_cell_count)
	{
		list_delete_node(&p_cell_ctxt->pci_sus_conf_ctxt_list, &p_list_node->anchor);
		son_mem_free(p_list_node);
		ret_val      = SON_TRUE;

		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
				"PCI context"
				"[PCI %d EARFCN %u is Deleted for [Cell_id :%d]]",
				pci, earfcn, son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_INFO,
				"PCI context"
				"[PCI %d EARFCN %u is with the Neighbor "
				" count %d is greater than 2 for [Cell Id :0x%x]]",
				pci, earfcn, p_list_node->pci_sus_conf_data.nbr_cell_count,
				son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_delete_pci_ctxt_from_sus_pci_ctxt_list */

/*****************************************************************************
 * Function Name  : anr_reset_sus_pci_count
 * Inputs         : p_cell_ctxt  - Cell context node
 *                  p_sus_pci_ctxt_node
 * Outputs        : None
 * Returns        : son_bool_et  - SON_TRUE / SON_FALSE
 * Description    : This function deletes a pci context from
 *                   suspected PCI context list
 *****************************************************************************/
son_bool_et
anr_reset_sus_pci_count
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_pci_sus_conf_node_t   *p_sus_pci_ctxt_node
 )
{
	son_bool_et ret_val = SON_FALSE;
	son_u32 cell_count  = 0;
	anr_intra_rat_hash_nr_tuple_t  *p_hash_tpl = SON_PNULL;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_sus_pci_ctxt_node)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointer to Cell context or Suspected PCI node "
				"is SON_PNULL");
	}
	else
	{
		if (p_sus_pci_ctxt_node->pci_sus_conf_data.nbr_cell_count)
		{
			p_sus_pci_ctxt_node->pci_sus_conf_data.ask_cgi_counter =
				p_cell_ctxt->pci_confusion_cfg.clear_suspect_pci_threshold;

			p_sus_pci_ctxt_node->pci_sus_conf_data.ask_cgi_thx_val =
				p_cell_ctxt->pci_confusion_cfg.clear_suspect_pci_threshold;

			cell_count = p_sus_pci_ctxt_node->pci_sus_conf_data.nbr_cell_count;

			p_hash_tpl   = anr_get_first_nr_from_intra_rat_hash_table(
					&p_cell_ctxt->intra_rat_neighbor_table);

			while (cell_count && SON_PNULL != p_hash_tpl)
			{
				if (p_hash_tpl->data.neighbor_info.pci == p_sus_pci_ctxt_node->pci_sus_conf_data.pci  &&
						p_hash_tpl->data.neighbor_info.earfcn == p_sus_pci_ctxt_node->pci_sus_conf_data.earfcn)
				{
					p_hash_tpl->data.received_cgi_counter = p_cell_ctxt->pci_confusion_cfg.
						clear_suspect_pci_threshold;
					cell_count--;
				}

				p_hash_tpl = anr_get_next_nr_from_intra_rat_hash_table(&p_cell_ctxt->intra_rat_neighbor_table,
						&p_hash_tpl->anchor);
			}

			ret_val = SON_TRUE;

			SON_LOG(g_anr_context.log_enable,
					p_son_anr_facility_name,
					SON_BRIEF,
					"Ask CGI Counter %d Ask CGI Threshold %d "
					"in Suspect - confusion PCI node element for serving Cell [Cell id:0x%x]",
					p_sus_pci_ctxt_node->pci_sus_conf_data.ask_cgi_counter,
					p_sus_pci_ctxt_node->pci_sus_conf_data.ask_cgi_thx_val,
					son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
		}
		else
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_INFO,
					"No Neighbor of PCI %d EARFCN %u is present "
					"in NRT of [Cell id:0x%x]",
					p_sus_pci_ctxt_node->pci_sus_conf_data.pci, p_sus_pci_ctxt_node->pci_sus_conf_data.earfcn,
					son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_reset_sus_pci_count */

/*UTRAN-GERAN STATS MGMT STARTS*/

/*****************************************************************************
 * Function Name  : anr_key_of_utran_freq_stats_list
 * Inputs         : p_node - anchor field of the broadcast statistics list node
 * Outputs        : None
 * Returns        : son_void_t * - pointer to key field(earfcn)  as void pointer
 * Description    : This function determines the key field(earfcn) of
 *                  the broadcast statistics node in the list.
 ******************************************************************************/
const son_void_t *
anr_key_of_utran_freq_stats_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_utran_freq_stats_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_utran_freq_stats_node_t,
			anchor, p_node);

	return (son_void_t *)(&p_data->utran_stats.uarfcn);
}

/*****************************************************************************
 * Function Name  : anr_compare_utran_freq_stats_list_node
 * Inputs         : p_val_1  Pointer to the key field(earfcn)as void pointer
 *                : p_val_2  Pointer to the key field(earfcn)as void pointer
 * Outputs        : None
 * Returns        : son_s32 - Zero If matches
 *                            Non Zero If not matches
 * Description    : This function compares the key field(earfcn) of the
 *                  broadcast statistics node in the list
 ******************************************************************************/
son_s32
anr_compare_utran_freq_stats_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	son_u16 *p_uarfcn_1 = SON_PNULL;
	son_u16 *p_uarfcn_2 = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointer to the cell Id is NULL");
	}
	else
	{
		p_uarfcn_1 = (son_u16 *)p_val_1;
		p_uarfcn_2 = (son_u16 *)p_val_2;
		if ((*p_uarfcn_1) == (*p_uarfcn_2))
		{
			ret_value = 0;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_utran_freq_stats_list_node */

/*****************************************************************************
 * Function Name  : anr_find_record_in_bs_allowed_stats_list
 * Inputs         : p_list  - Pointer to broadcast allowed neighbor
 *                   statistics List
 *                  earfcn  - EARFCN value
 * Outputs        : None
 * Returns        : anr_bs_allowed_node_t* - Pointer to ANR EARFCN specific
 *                                           broadcasted neighbor count Node
 * Description    : This function finds the broadcast statistcs node for the
 *                   given earfcn value
 *****************************************************************************/
anr_utran_freq_stats_node_t *
anr_find_record_in_utran_freq_stats_list
(
 SON_LIST                *p_list,
 son_u16 uarfcn
 )
{
	SON_LIST_NODE                *p_node             = SON_PNULL;
	anr_utran_freq_stats_node_t  *p_list_node        = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_node = list_find_node(p_list, (son_void_t *)&uarfcn,
			anr_key_of_utran_freq_stats_list,
			anr_compare_utran_freq_stats_list_node);

	if (SON_PNULL != p_node)
	{
		p_list_node = YMEMBEROF(anr_utran_freq_stats_node_t,
				anchor, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* anr_find_record_in_utran_freq_stats_list */

/*****************************************************************************
 * Function Name  : anr_insert_record_in_utran_freq_stats_list
 * Inputs         : p_list : Pointer to the UTRAN freq stats list
 *                  uarfcn : UARFCN value
 * Outputs        : None
 * Returns        : anr_utran_freq_stats_node_t* - Utran freq stats list node
 * Description    : This function inserts a UTRAN statistics record in the
 *                  List
 **********************************************************************/
anr_utran_freq_stats_node_t *
anr_insert_record_in_utran_freq_stats_list
(
 SON_LIST                *p_list,
 son_u16 uarfcn
 )
{
	anr_utran_freq_stats_node_t   *p_list_node      = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = (anr_utran_freq_stats_node_t *)son_mem_get
		(sizeof(anr_utran_freq_stats_node_t));

	if (SON_PNULL != p_list_node)
	{
		SON_MEMSET(p_list_node, 0, sizeof(anr_utran_freq_stats_node_t));

		p_list_node->utran_stats.uarfcn = uarfcn;
		p_list_node->utran_stats.num_of_utran_nbrs_to_rrm++;

		list_insert_node(p_list, &p_list_node->anchor);
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Failed to get memory for storing the UTRAN frequencies statistics");
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* anr_insert_record_in_utran_freq_stats_list */

/*****************************************************************************
 * Function Name  : anr_delete_record_from_utran_freq_stats_list
 * Inputs         : p_list   -Utran frequency list pointer
 *                : uarfcn   - UARFCN value
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function deletes the record from the UTRAN freq
 *                   Statistics list
 *****************************************************************************/
son_bool_et
anr_delete_record_from_utran_freq_stats_list
(
 SON_LIST                *p_list,
 son_u16 uarfcn
 )
{
	anr_utran_freq_stats_node_t *p_list_node     = SON_PNULL;
	son_bool_et ret_val         = SON_FALSE;


	SON_UT_TRACE_ENTER();

	p_list_node = anr_find_record_in_utran_freq_stats_list(
			p_list, uarfcn);

	if (SON_PNULL != p_list_node)
	{
		if (0 == p_list_node->utran_stats.num_of_utran_nbrs_to_rrm)
		{
			list_delete_node(p_list, &p_list_node->anchor);
			son_mem_free(p_list_node);
			ret_val      = SON_TRUE;

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_DETAILED,
					"UTRAN frequency stats node deleted");
		}
		else
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_DETAILED,
					"UTRAN frequency stats node  can not be deleted");
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_delete_record_from_utran_freq_stats_list */

/*****************************************************************************
 * Function Name  : increment_utran_nbr_count_for_rrm
 * Inputs         : p_cell_ctxt - Pointer to serving cell context
 *                : p_list_node - Pinter to the broadcast statistics node
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function increments the NBR count for UTRAN RAT type
 *****************************************************************************/
son_bool_et
increment_utran_nbr_count_for_rrm
(
 anr_cell_context_t         *p_cell_ctxt,
 anr_utran_freq_stats_node_t *p_list_node
 )
{
	son_bool_et ret_val         = SON_FALSE;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_list_node)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Invalid data as"
				" either p_cell_ctxt,p_list_node is SON_PNULL");

		return SON_FALSE;
	}

	if (p_list_node->utran_stats.num_of_utran_nbrs_to_rrm <
			SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN)
	{
		p_list_node->utran_stats.num_of_utran_nbrs_to_rrm++;
		ret_val = SON_TRUE;
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_INFO,
				"Increment not possible as number of neighbrs send to rrm for UARFCN %u is %d",
				p_list_node->utran_stats.uarfcn, p_list_node->utran_stats.num_of_utran_nbrs_to_rrm);
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* increment_utran_nbr_count_for_rrm */

/*****************************************************************************
 * Function Name  : decrement_utran_nbr_count_for_rrm
 * Inputs         : p_cell_ctxt - Pointer to serving cell context
 *                : p_list_node - Pointer to the broadcast statistics node
 *                : p_list
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function decrements the count of neighbor in  broadcast
 *                   Statistics Record
 *****************************************************************************/
son_bool_et
decrement_utran_nbr_count_for_rrm
(
 anr_cell_context_t    *p_cell_ctxt,
 anr_utran_freq_stats_node_t *p_list_node,
 SON_LIST *p_list
 )
{
	son_bool_et ret_val         = SON_FALSE;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_list_node)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Invalid data as"
				" either p_cell_ctxt,p_list_node is SON_PNULL");
		return SON_FALSE;
	}

	if (0 < p_list_node->utran_stats.num_of_utran_nbrs_to_rrm)
	{
		p_list_node->utran_stats.num_of_utran_nbrs_to_rrm--;
		ret_val = SON_TRUE;
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_INFO,
				"Decrement not possible as number of neighbrs send to rrm for UARFCN %u is %d",
				p_list_node->utran_stats.uarfcn, p_list_node->utran_stats.num_of_utran_nbrs_to_rrm);
	}

	/*The check is redundant,but effective as log from the
	 * delete function will not appear over and over*/
	if (SON_TRUE == ret_val && 0 == p_list_node->utran_stats.num_of_utran_nbrs_to_rrm)
	{
		if (SON_PNULL != p_list)
		{
			anr_delete_record_from_utran_freq_stats_list(
					p_list, p_list_node->utran_stats.uarfcn);
		}
	}

	/*Making the pointer NULL delibrately,
	 * as deletion might have happend*/
    p_list_node = SON_PNULL;

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* decrement_utran_nbr_count_for_rrm */

/*GERAN*/

/*****************************************************************************
 * Function Name  : anr_key_of_geran_freq_stats_list
 * Inputs         : p_node - anchor field of the broadcast statistics list node
 * Outputs        : None
 * Returns        : son_void_t * - pointer to key field(earfcn)  as void pointer
 * Description    : This function determines the key field(earfcn) of
 *                  the broadcast statistics node in the list.
 ******************************************************************************/
const son_void_t *
anr_key_of_geran_freq_stats_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_geran_freq_stats_node_t *p_data = SON_PNULL;


	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_geran_freq_stats_node_t,
			anchor, p_node);

	return (son_void_t *)(&p_data->geran_stats);
}

/*****************************************************************************
 * Function Name  : anr_compare_geran_freq_stats_list_node
 * Inputs         : p_val_1  - Pointer to the key field(earfcn)as void pointer
 *                : p_val_2  - Pointer to the key field(earfcn)as void pointer
 * Outputs        : None
 * Returns        : son_s32  - Zero If matches
 *                             Non Zero If not matches
 * Description    : This function compares the key field(earfcn) of the
 *                  broadcast statistics node in the list
 ******************************************************************************/
son_s32
anr_compare_geran_freq_stats_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	anr_geran_freq_stats_data_t *p_data_1 = SON_PNULL;
	anr_geran_freq_stats_data_t *p_data_2 = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Pointers passed are  NULL");
	}
	else
	{
		p_data_1 = (anr_geran_freq_stats_data_t *)p_val_1;
		p_data_2 = (anr_geran_freq_stats_data_t *)p_val_2;

		if ((p_data_1->arfcn == p_data_2->arfcn) && (p_data_1->band_ind == p_data_2->band_ind))
		{
			ret_value = 0;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_geran_freq_stats_list_node */

/*****************************************************************************
 * Function Name  : anr_find_record_in_geran_freq_stats_list
 * Inputs         : p_list  : Pointer to broadcast allowed neighbor
 *                   statistics List
 *                  earfcn  : EARFCN value
 * Outputs        : None
 * Returns        : anr_bs_allowed_node_t* - Pointer to ANR EARFCN specific
 *                                           broadcasted neighbor count Node
 * Description    : This function finds the broadcast statistcs node for the
 *                   given earfcn value
 *****************************************************************************/
anr_geran_freq_stats_node_t *
anr_find_record_in_geran_freq_stats_list
(
 SON_LIST                     *p_list,
 anr_geran_freq_stats_data_t  *p_data
 )
{
	SON_LIST_NODE                *p_node             = SON_PNULL;
	anr_geran_freq_stats_node_t  *p_list_node        = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_node = list_find_node(p_list, (son_void_t *)p_data,
			anr_key_of_geran_freq_stats_list,
			anr_compare_geran_freq_stats_list_node);

	if (SON_PNULL != p_node)
	{
		p_list_node = YMEMBEROF(anr_geran_freq_stats_node_t,
				anchor, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* anr_find_record_in_geran_freq_stats_list */

/*****************************************************************************
 * Function Name  : anr_insert_record_in_geran_freq_stats_list
 * Inputs         : p_list : Pointer to the GERAN freq stats list
 *                  arfcn  : ARFCN value
 *                :
 * Outputs        : None
 * Returns        : anr_geran_freq_stats_node_t* - Pointer to Geran freq stats
 *                                                 list node
 * Description    : This function inserts a UTRAN statistics record in the
 *                  List
 **********************************************************************/
anr_geran_freq_stats_node_t *
anr_insert_record_in_geran_freq_stats_list
(
 SON_LIST                *p_list,
 son_u16 arfcn,
 son_geran_band_indicator_et band_ind
 )
{
	anr_geran_freq_stats_node_t   *p_list_node      = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_list_node = (anr_geran_freq_stats_node_t *)son_mem_get
		(sizeof(anr_geran_freq_stats_node_t));

	if (SON_PNULL != p_list_node)
	{
		SON_MEMSET(p_list_node, 0, sizeof(anr_geran_freq_stats_node_t));

		p_list_node->geran_stats.arfcn = arfcn;
		p_list_node->geran_stats.band_ind = band_ind;
		p_list_node->geran_stats.num_of_geran_nbrs_to_rrm++;

		list_insert_node(p_list, &p_list_node->anchor);
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Failed to get memory for storing the GERAN frequencies statistics");
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* anr_insert_record_in_geran_freq_stats_list */

/***************************************************************************
 * Function Name  : anr_delete_record_from_cdma_hrpd_stats_list
 * Inputs         : p_list      - Pointer to CDMA frequency list
 *                  band_class  - Band Class
 *                  arfcn       - ARFCN value
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function deletes the record from the CDMA freq/band
 *                  statistics list
 ***************************************************************************/
son_bool_et
anr_delete_record_from_cdma_hrpd_stats_list
(
 SON_LIST                *p_list,
 son_cdma_band_class_et band_class,
 son_u16 arfcn
 )
{
	anr_cdma_hrpd_band_stats_node_t  *p_band_list_node      = SON_PNULL;
	anr_cdma_hrpd_freq_stats_node_t  *p_freq_list_node      = SON_PNULL;
	son_bool_et ret_val                 = SON_FALSE;
	static son_u16 total_hrpd_nbr_per_freq = 0;


	SON_UT_TRACE_ENTER();

	/* Finding the band class */
	p_band_list_node = anr_find_record_in_cdma_hrpd_band_stats_list(
			p_list,
			band_class);

	if (p_band_list_node != SON_PNULL)
	{
		/* Finding the frequency */
		p_freq_list_node = anr_find_record_in_cdma_hrpd_freq_stats_list(
				&p_band_list_node->cdma_band_stats.freq_list,
				arfcn);

		if (p_freq_list_node != SON_PNULL)
		{
			/* Getting the count of neighbors on particular frequency */
			total_hrpd_nbr_per_freq = anr_get_no_of_hrpd_nbrs_per_frequency(p_freq_list_node);

			/* If there are no nbrs and frequency list has more than one freq then
			 * just delete frequency node */
			if ((total_hrpd_nbr_per_freq == 0) &&
					(list_count(&p_band_list_node->cdma_band_stats.freq_list) > 1))
			{
				/*Deleting frequence information */
				list_delete_node(&p_band_list_node->cdma_band_stats.freq_list, &p_freq_list_node->anchor);
				son_mem_free(p_freq_list_node);

				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_DETAILED,
						"CDMA HRPD frequency stats node deleted for band class %d,"
						"arfcn %d", band_class, arfcn);
				ret_val = SON_TRUE;
			}

			/* If there are no nbrs and frequency list has just one freq then
			 * delete frequency node as well as band node */
			else if ((total_hrpd_nbr_per_freq == 0) &&
					(list_count(&p_band_list_node->cdma_band_stats.freq_list) == 1))
			{
				/* Deleting frequence information */
				list_delete_node(&p_band_list_node->cdma_band_stats.freq_list,
						&p_freq_list_node->anchor);
				son_mem_free(p_freq_list_node);

				/* Deleting Band information*/
				list_delete_node(p_list, &p_band_list_node->anchor);
				son_mem_free(p_band_list_node);

				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_DETAILED,
						"CDMA HRPD frequency and band stats node deleted for band class %d,"
						"arfcn %d", band_class, arfcn);
				ret_val = SON_TRUE;
			}
			else
			{
				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_DETAILED,
						"CDMA HRPD frequency stats node can not be deleted "
						"for band class %d, arfcn %d",
						band_class, arfcn);
			}
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_delete_record_from_cdma_hrpd_stats_list */

/***************************************************************************
 * Function Name  : anr_delete_record_from_cdma_xrtt_stats_list
 * Inputs         : p_list      - Pointer to CDMA frequency list
 *                  band_class  - band class
 *                  arfcn       - ARFCN value
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function deletes the record from the CDMA freq/band
 *                  statistics list
 ***************************************************************************/
son_bool_et
anr_delete_record_from_cdma_xrtt_stats_list
(
 SON_LIST                *p_list,
 son_cdma_band_class_et band_class,
 son_u16 arfcn
 )
{
	anr_cdma_xrtt_band_stats_node_t  *p_band_list_node      = SON_PNULL;
	anr_cdma_xrtt_freq_stats_node_t  *p_freq_list_node      = SON_PNULL;
	son_bool_et ret_val                 = SON_FALSE;
	son_u16 total_xrtt_nbr_per_freq = 0;


	SON_UT_TRACE_ENTER();

	/* Finding the band class */
	p_band_list_node = anr_find_record_in_cdma_xrtt_band_stats_list(
			p_list,
			band_class);

	if (p_band_list_node != SON_PNULL)
	{
		/* Finding the frequency */
		p_freq_list_node = anr_find_record_in_cdma_xrtt_freq_stats_list(
				&p_band_list_node->cdma_band_stats.freq_list,
				arfcn);

		if (p_freq_list_node != SON_PNULL)
		{
			/* Getting the count of neighbors on particular frequency */
			total_xrtt_nbr_per_freq = anr_get_no_of_xrtt_nbrs_per_frequency(p_freq_list_node);

			/* If there are no nbrs and frequency list has more than one freq then
			 * just delete frequency node */
			if ((total_xrtt_nbr_per_freq == 0) &&
					(list_count(&p_band_list_node->cdma_band_stats.freq_list) > 1))
			{
				/* Deleting frequency information */
				list_delete_node(&p_band_list_node->cdma_band_stats.freq_list,
						&p_freq_list_node->anchor);
				son_mem_free(p_freq_list_node);

				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_DETAILED,
						"CDMA XRTT frequency stats node deleted for band class %d,"
						"arfcn %d", band_class, arfcn);
				ret_val = SON_TRUE;
			}

			/* If there are no nbrs and frequency list has just one freq then
			 * delete frequency node as well as band node */
			else if ((total_xrtt_nbr_per_freq == 0) &&
					(list_count(&p_band_list_node->cdma_band_stats.freq_list) == 1))
			{
				/* Deleting frequence information */
				list_delete_node(&p_band_list_node->cdma_band_stats.freq_list,
						&p_freq_list_node->anchor);
				son_mem_free(p_freq_list_node);

				/* Deleting Band information*/
				list_delete_node(p_list, &p_band_list_node->anchor);
				son_mem_free(p_band_list_node);

				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_DETAILED,
						"CDMA XRTT frequency and band stats node deleted for band class %d,"
						"arfcn %d", band_class, arfcn);
				ret_val = SON_TRUE;
			}
			else
			{
				SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
						SON_DETAILED,
						"CDMA XRTT frequency stats node can not be deleted "
						"for band class %d, arfcn %d",
						band_class, arfcn);
			}
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_delete_record_from_cdma_xrtt_stats_list */

/*****************************************************************************
 * Function Name  : anr_delete_record_from_geran_freq_stats_list
 * Inputs         : p_list   -Geran frequency list pointer
 *                : arfcn   - ARFCN value
 *                : band_ind - band Indicator
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function deletes the record from the UTRAN freq
 *                   Statistics list
 *****************************************************************************/
son_bool_et
anr_delete_record_from_geran_freq_stats_list
(
 SON_LIST                *p_list,
 son_u16 arfcn,
 son_geran_band_indicator_et band_ind
 )
{
	anr_geran_freq_stats_node_t *p_list_node     = SON_PNULL;
	son_bool_et ret_val         = SON_FALSE;
	anr_geran_freq_stats_data_t data;


	SON_UT_TRACE_ENTER();
	SON_MEMSET(&data, 0, sizeof(anr_geran_freq_stats_data_t));
	data.arfcn    =  arfcn;
	data.band_ind = band_ind;

	p_list_node = anr_find_record_in_geran_freq_stats_list(
			p_list, &data);

	if (SON_PNULL != p_list_node)
	{
		if (0 == p_list_node->geran_stats.num_of_geran_nbrs_to_rrm)
		{
			list_delete_node(p_list, &p_list_node->anchor);
			son_mem_free(p_list_node);
			ret_val      = SON_TRUE;

			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_DETAILED,
					"GERAN frequency stats node deleted");
		}
		else
		{
			SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
					SON_DETAILED,
					"GERAN frequency stats node  can not be deleted");
		}
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_delete_record_from_geran_freq_stats_list */

/*****************************************************************************
 * Function Name  : increment_geran_nbr_count_for_rrm
 * Inputs         : p_cell_ctxt - Pointer to serving cell context
 *                : p_list_node - Pointer to the GERAN freq stats node
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : -
 *****************************************************************************/
son_bool_et
increment_geran_nbr_count_for_rrm
(
 anr_cell_context_t         *p_cell_ctxt,
 anr_geran_freq_stats_node_t *p_list_node
 )
{
	son_bool_et ret_val         = SON_FALSE;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_list_node)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Invalid data as"
				" either p_cell_ctxt,p_list_node is SON_PNULL");

		return SON_FALSE;
	}

	if (p_list_node->geran_stats.num_of_geran_nbrs_to_rrm <
			SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN)
	{
		p_list_node->geran_stats.num_of_geran_nbrs_to_rrm++;
		ret_val = SON_TRUE;
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_INFO,
				"Increment not possible as number of neighbrs send to rrm for ARFCN %d band indicator %d is %d",
				p_list_node->geran_stats.arfcn,
				p_list_node->geran_stats.band_ind,
				p_list_node->geran_stats.num_of_geran_nbrs_to_rrm);
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* increment_geran_nbr_count_for_rrm */

/*****************************************************************************
 * Function Name  : decrement_geran_nbr_count_for_rrm
 * Inputs         : p_cell_ctxt - Pointer to serving cell context
 *                : p_list_node - Pointer to the broadcast statistics node
 * Outputs        : None
 * Returns        : son_bool_et - SON_TRUE / SON_FALSE
 * Description    : This function decrements the count of neighbor in  broadcast
 *                   Statistics Record
 *****************************************************************************/
son_bool_et
decrement_geran_nbr_count_for_rrm
(
 anr_cell_context_t    *p_cell_ctxt,
 anr_geran_freq_stats_node_t *p_list_node
 )
{
	son_bool_et ret_val         = SON_FALSE;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_list_node)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Invalid data as"
				" either p_cell_ctxt,p_list_node is SON_PNULL");
		return SON_FALSE;
	}

	if (0 < p_list_node->geran_stats.num_of_geran_nbrs_to_rrm)
	{
		p_list_node->geran_stats.num_of_geran_nbrs_to_rrm--;
		ret_val = SON_TRUE;
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_INFO,
				"Decrement not possible as number of neighbrs send to rrm for ARFCN %d band indicator %d is %d",
				p_list_node->geran_stats.arfcn,
				p_list_node->geran_stats.band_ind,
				p_list_node->geran_stats.num_of_geran_nbrs_to_rrm);
	}

	/*The check is redundant,but effective as log from the
	 * delete function will not appear over and over*/
	if (SON_TRUE == ret_val && 0 == p_list_node->geran_stats.num_of_geran_nbrs_to_rrm)
	{
		anr_delete_record_from_geran_freq_stats_list(&p_cell_ctxt->geran_freq_stats_list,
				p_list_node->geran_stats.arfcn,
				p_list_node->geran_stats.band_ind);
	}

	/*Making the pointer NULL delibrately,
	 * as deletion might have happend*/
    p_list_node = SON_PNULL;

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* decrement_geran_nbr_count_for_rrm */

/*UTRAN-GERAN STATS MGMT STOPS*/

/*****************************************************************************
 * Function Name  : anr_key_of_rnc_id_list
 * Inputs         : p_node       - anchor field of the RNC Id list record
 * Outputs        : None
 * Returns        : son_void_t * - pointer to transaction Id as void pointer
 * Description    : This function determines the key in ANR transaction Id list
 ******************************************************************************/
const son_void_t *
anr_key_of_rnc_id_list
(
 const SON_LIST_NODE *p_node
 )
{
	anr_rnc_id_node_t *p_data = SON_PNULL;

	SON_UT_TRACE_ENTER();
	SON_ASSERT(SON_PNULL != p_node);

	p_data = YMEMBEROF(anr_rnc_id_node_t, rnc_list_node, p_node);

	return (son_void_t *)(&p_data->rnc_id);
}

/*****************************************************************************
 * Function Name  : anr_compare_rnc_id
 * Inputs         : p_val_1   - Pointer to the transaction Id as void pointer
 *                : p_val_2   - Pointer to the transaction Id as void pointer
 * Outputs        : None
 * Returns        : son_s32   - Zero If matches
 *                              Non Zero If not matches
 * Description    : This function compares the transaction Ids
 ******************************************************************************/
son_s32
anr_compare_rnc_id
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;
	const son_u32 *rnc_id_1 = SON_PNULL;
	const son_u32 *rnc_id_2 = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"anr_compare_rnc_id: Pointer to the RNC Id is NULL");
	}
	else
	{
		rnc_id_1 = (son_u32 *)p_val_1;
		rnc_id_2 = (son_u32 *)p_val_2;

		ret_value = SON_MEMCMP(rnc_id_1, rnc_id_2, sizeof(son_u32));
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* anr_compare_rnc_id */

/******************************************************************************
 * Function Name  : anr_clear_rnc_id_list
 * Inputs         : p_list - Pointer to the RNC Id list
 * Returns        : son_void_t - None
 * Description    : This function clears the entire RNC Id list for a
 *                  configured cell
 ******************************************************************************/
son_void_t
anr_clear_rnc_id_list
(
 SON_LIST    *p_list
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_list_node = list_pop_node(p_list);

	while (p_list_node)
	{
		anr_rnc_id_node_t *p_data = YMEMBEROF(anr_rnc_id_node_t,
				rnc_list_node, p_list_node);

		/* Free the memory occupied by the node */
		son_mem_free(p_data);

		p_list_node = list_pop_node(p_list);
	}

	SON_UT_TRACE_EXIT();
} /* anr_clear_rnc_id_list */

/*****************************************************************************
 * Function Name  : anr_get_rnc_indicator_from_cell_id
 * Inputs         : p_cell_id    - Pointer to CELL Id
 * Outputs        : None
 * Returns        : son_u8 - RNC indicator
 * Description    : This function gets RNC Indicator from a CELL Id
 *****************************************************************************/
son_u8
anr_get_rnc_indicator_from_cell_id
(
 son_u8  *p_cell_id
 )
{
	son_u8  ret_val = -1;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_id)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Cell Id is NULL");

		SON_UT_TRACE_EXIT();
		return ret_val;
	}

	ret_val = p_cell_id[3] & SON_RIGHT_NIBBLE_MASK;

	SON_UT_TRACE_EXIT();
	return ret_val;
}

/*****************************************************************************
 * Function Name  : anr_get_extended_rnc_id_from_cell_id
 * Inputs         : p_cell_id    - Pointer to CELL Id
 * Outputs        : p_rnc_id     - Pointer to RND Id
 * Returns        : son_void_t - None
 * Description    : This function gets extended RNC Id from a CELL Id
 *****************************************************************************/
son_void_t
anr_get_extended_rnc_id_from_cell_id
(
 son_u8  *p_cell_id,
 son_u8  *p_rnc_id
 )
{
	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_id)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Cell Id is NULL");

		SON_UT_TRACE_EXIT();
		return;
	}

	p_rnc_id[0] = p_cell_id[0];
	p_rnc_id[1] = p_cell_id[1]; 

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_get_non_extended_rnc_id_from_cell_id
 * Inputs         : p_cell_id    - Pointer to CELL Id
 * Outputs        : p_rnc_id     - Pointer to RND Id
 * Returns        : son_void_t - None
 * Description    : This function gets non-extended RNC Id from a CELL Id
 *****************************************************************************/
son_void_t
anr_get_non_extended_rnc_id_from_cell_id
(
 son_u8  *p_cell_id,
 son_u8  *p_rnc_id 
 )
{
	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_id)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Cell Id is NULL");

		SON_UT_TRACE_EXIT();
		return;
	}

	p_rnc_id[0] = p_cell_id[0];
	p_rnc_id[1] = p_cell_id[1] & SON_LEFT_NIBBLE_MASK;

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_find_rnc_id_from_the_list
 * Inputs         : p_list   - Pointer to RNC Id list (Extended/Non-Extended)
 *                  p_rnc_id - Pointer to RND Id
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function finds a RNC Id from RNC Id list
 *****************************************************************************/
son_bool_et
anr_find_rnc_id_from_the_list
(
 SON_LIST    *p_list,
 son_u8      *p_rnc_id
 )
{
	SON_LIST_NODE       *p_node         = SON_PNULL;
	son_bool_et         ret_val         = SON_FALSE;

	SON_UT_TRACE_ENTER();

	p_node = list_find_node(p_list,
			(son_void_t *)p_rnc_id, anr_key_of_rnc_id_list,
			anr_compare_rnc_id);

	if (SON_PNULL != p_node)
	{
		ret_val = SON_TRUE;
		return ret_val;
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_find_rnc_id_from_the_list */

/*****************************************************************************
 * Function Name  : anr_insert_in_rnc_id_list
 * Inputs         : p_list   - Cell specfic RNC Id list
 *                  p_rnc_id - Pointer to RNC Id
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function inserts a RNC Id in RNC Id list
 *****************************************************************************/
son_bool_et
anr_insert_in_rnc_id_list
(
 SON_LIST *p_list,
 son_u8  *p_rnc_id
 )
{
	anr_rnc_id_node_t   *p_list_node = SON_PNULL;
	son_bool_et         ret_val      = SON_FALSE;


	SON_UT_TRACE_ENTER();

	p_list_node = son_mem_get(sizeof(anr_rnc_id_node_t));

	if (SON_PNULL != p_list_node)
	{
		SON_MEMCPY(&p_list_node->rnc_id, p_rnc_id,
				sizeof(p_list_node->rnc_id));

		list_insert_node(p_list,
				&p_list_node->rnc_list_node);

		ret_val = SON_TRUE;
        /*SPR 15935 Fix Start*/
        p_list_node = SON_PNULL;
        /*SPR 15935 Fix Stop*/
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_INFO,
				"Added [RNCID: 0x%lu]"
				" in RNC Id List", (son_ul32)p_list_node->rnc_id);
	}
	else
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_insert_in_rnc_id_list */

/* Rel 3.0 Multi Sector code changes Start */
/*****************************************************************************
 * Function Name  : anr_get_auto_maintain_local_cell_as_nbr_status
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_bool_et - Value of anr_auto_maintain_local_cell_as_nbr 
 *                                configuration parameter
 * Description    : This function gets the value of 
 *                  anr_auto_maintain_local_cell_as_nbr configuration parameter
 ******************************************************************************/
son_bool_et
anr_get_auto_maintain_local_cell_as_nbr_status
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_config_params.anr_config_params.
		anr_auto_maintain_local_cell_as_nbr;
}

/*****************************************************************************
 * Function Name  : anr_is_nbr_cell_on_serving_enb
 * Inputs         : p_nbr_enb_id -  Pointer to the eNB Id
 * Outputs        : None
 * Returns        : SON_TRUE - If the neighbor cell belongs to the serving eNB
 *                  SON_FALSE - Otherwise
 * Description    : This function finds if a neighbor cell is on the serving 
 *                  eNB or is on peer eNB
 ******************************************************************************/
son_bool_et
anr_is_nbr_cell_on_serving_enb
(
 son_global_enb_id_t *p_nbr_enb_id
 )
{
	son_bool_et is_nbr_cell_on_same_enb_id = SON_FALSE;
	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_nbr_enb_id)
	{
		SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
				SON_ERROR,
				"p_nbr_enb_id is NULL");

		SON_UT_TRACE_EXIT();
		return is_nbr_cell_on_same_enb_id;
	}

	if (0 == SON_MEMCMP(p_nbr_enb_id, &g_anr_context.own_enb_id, 
				sizeof(son_global_enb_id_t)))
	{
		is_nbr_cell_on_same_enb_id = SON_TRUE;
	}

	SON_UT_TRACE_EXIT();
	return is_nbr_cell_on_same_enb_id;
}

/*****************************************************************************
 * Function Name  : anr_get_own_enb_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_global_enb_id_t - Own Global eNB Identifier
 * Description    : This function gets the own Global eNB Identifier
 ******************************************************************************/
son_global_enb_id_t
anr_get_own_enb_id
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_anr_context.own_enb_id;
}

/*SPR-11554-Start*/

/*Function removed*/

/*SPR-11554-End*/

/* Rel 3.0 Multi Sector code changes Stop */

/* SPR-13251 Fix Starts */
/*****************************************************************************
 * Function Name  : anr_get_log_level
 * Inputs         : None
 * Outputs        : None
 * Returns        : Log Level of the ANR 
 * Description    : This function gets the log level
 ******************************************************************************/
son_u8
anr_get_log_level
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	SON_UT_TRACE_EXIT();
	return g_anr_context.log_level;
}

/*****************************************************************************
 * Function Name  : anr_set_log_level
 * Inputs         : log_level - Log Level to be set
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function sets the log level
 ******************************************************************************/
son_void_t
anr_set_log_level
(
 son_u8 log_level
 )
{
	SON_UT_TRACE_ENTER();
	g_anr_context.log_level = log_level;
	SON_UT_TRACE_EXIT();
}
/* SPR-13251 Fix Ends */

/* SPR 15698 Start */
/*****************************************************************************
 * Function Name  : get_first_pending_cgi_list_node_eutran_cdma
 * Inputs         : Pointer of eutran_pci_requested_cgi_list
 * Outputs        : None
 * Returns        : anr_eutran_cdma_pci_value_node_t * - Pointer of first node in list
 * Description    : This function gets the first node in eutran_pci_requested_cgi_list
 ******************************************************************************/
    anr_eutran_cdma_pci_value_node_t*
get_first_pending_cgi_list_node_eutran_cdma(SON_LIST* p_list)

{
    SON_LIST_NODE    *p_node    = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (list_count(p_list) > 0)
    {
        p_node = get_first_list_node(p_list);
        if (SON_PNULL != p_node)
        {
            SON_UT_TRACE_EXIT();
            return (anr_eutran_cdma_pci_value_node_t*) YMEMBEROF(anr_eutran_cdma_pci_value_node_t, anchor, p_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
}

/*****************************************************************************
 * Function Name  : get_next_pending_cgi_list_node_eutran_cdma
 * Inputs         : Pointer of anr_eutran_cdma_pci_value_node_t
 * Outputs        : None
 * Returns        : anr_eutran_cdma_pci_value_node_t* - Pointer of the relatively next
 *                  node to the node passed as function argument
 * Description    : This function gets the Pointer of the relatively next
 *                  node to the node passed as function argument
 ******************************************************************************/
    anr_eutran_cdma_pci_value_node_t*
get_next_pending_cgi_list_node_eutran_cdma(anr_eutran_cdma_pci_value_node_t* p_node)

{
    SON_LIST_NODE    *p_list_node    = SON_PNULL;

    SON_UT_TRACE_ENTER();
    if (SON_PNULL != p_node)
    {
        p_list_node = get_next_list_node(&p_node->anchor);
        if (SON_PNULL != p_list_node)
        {
            return (anr_eutran_cdma_pci_value_node_t*)
                YMEMBEROF(anr_eutran_cdma_pci_value_node_t, anchor, p_list_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;

}

/*****************************************************************************
 * Function Name  : get_first_pending_cgi_list_node_utran
 * Inputs         : Pointer of utran_pci_requested_cgi_list
 * Outputs        : None
 * Returns        : anr_utran_pci_value_node_t* - Pointer of first node in list
 * Description    : This function gets the first node in utran_pci_requested_cgi_list
 ******************************************************************************/
    anr_utran_pci_value_node_t*
get_first_pending_cgi_list_node_utran(SON_LIST* p_list)

{
    SON_LIST_NODE    *p_node    = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (list_count(p_list) > 0)
    {
        p_node = get_first_list_node(p_list);
        if (SON_PNULL != p_node)
        {
            SON_UT_TRACE_EXIT();
            return (anr_utran_pci_value_node_t*) YMEMBEROF(anr_utran_pci_value_node_t, anchor, p_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
}

/*****************************************************************************
 * Function Name  : get_next_pending_cgi_list_node_utran
 * Inputs         : Pointer of anr_utran_pci_value_node_t
 * Outputs        : None
 * Returns        : anr_utran_pci_value_node_t* - Pointer of the relatively next
 *                  node to the node passed as function argument
 * Description    : This function gets the Pointer of the relatively next
 *                  node to the node passed as function argument
 ******************************************************************************/
    anr_utran_pci_value_node_t*
get_next_pending_cgi_list_node_utran(anr_utran_pci_value_node_t* p_node)
{
    SON_LIST_NODE    *p_list_node    = SON_PNULL;

    SON_UT_TRACE_ENTER();
    if (SON_PNULL != p_node)
    {
        p_list_node = get_next_list_node(&p_node->anchor);
        if (SON_PNULL != p_list_node)
        {
            return (anr_utran_pci_value_node_t*)
                YMEMBEROF(anr_utran_pci_value_node_t, anchor, p_list_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
}

/*****************************************************************************
 * Function Name  : get_first_pending_cgi_list_node_geran
 * Inputs         : Pointer of geran_pci_requested_cgi_list
 * Outputs        : None
 * Returns        : anr_geran_pci_value_node_t* - Pointer of first node in list
 * Description    : This function gets the first node in geran_pci_requested_cgi_list
 ******************************************************************************/
    anr_geran_pci_value_node_t*
get_first_pending_cgi_list_node_geran(SON_LIST* p_list)

{
    SON_LIST_NODE    *p_node    = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (list_count(p_list) > 0)
    {
        p_node = get_first_list_node(p_list);
        if (SON_PNULL != p_node)
        {
            SON_UT_TRACE_EXIT();
            return (anr_geran_pci_value_node_t*) YMEMBEROF(anr_geran_pci_value_node_t, anchor, p_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
}

/*****************************************************************************
 * Function Name  : get_next_pending_cgi_list_node_geran
 * Inputs         : Pointer of anr_geran_pci_value_node_t
 * Outputs        : None
 * Returns        : anr_geran_pci_value_node_t* - Pointer of the relatively next
 *                  node to the node passed as function argument
 * Description    : This function gets the Pointer of the relatively next
 *                  node to the node passed as function argument
 ******************************************************************************/
    anr_geran_pci_value_node_t*
get_next_pending_cgi_list_node_geran(anr_geran_pci_value_node_t* p_node)

{
    SON_LIST_NODE    *p_list_node    = SON_PNULL;

    SON_UT_TRACE_ENTER();
    if (SON_PNULL != p_node)
    {
        p_list_node = get_next_list_node(&p_node->anchor);
        if (SON_PNULL != p_list_node)
        {
            return (anr_geran_pci_value_node_t*)
                YMEMBEROF(anr_geran_pci_value_node_t, anchor, p_list_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;

}

/*****************************************************************************
 * Function Name  : anr_cleanup_pending_cgi_list_eutran_cdma
 * Inputs         : Pointer of eutran_pci_requested_cgi_list
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function is used to implement an Audit kind of funtionality
 *                  which is used to clean up the pending eutran cgi nodes if no cgi measurement
 *                  received for some predefined time.
 ******************************************************************************/
    son_void_t
anr_cleanup_pending_cgi_list_eutran_cdma(SON_LIST* p_list)
{
    anr_eutran_cdma_pci_value_node_t  *p_pci_list_node = SON_PNULL;
    anr_eutran_cdma_pci_value_node_t  *p_temp_node = SON_PNULL;
    time_t                            current_timestamp;

    SON_UT_TRACE_ENTER();

    if (list_count(p_list) > 0)
    {
        p_pci_list_node = get_first_pending_cgi_list_node_eutran_cdma(p_list);

        /// We need to traverse this list until
        /// - Search is exhausted Or
        /// - currentTimestamp =< node.absolute_cgi_gaurd_time_in_secs
        son_time(&current_timestamp);

        while((SON_PNULL != p_pci_list_node) &&
                (p_pci_list_node->absolute_cgi_gaurd_time_in_secs < current_timestamp))
        {
            /* SPR 15831 Start */
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                    SON_BRIEF, "Stale Node Found current_timestamp = %lu, Node Absolute Expired Time = %lu",
                    (son_ul32)current_timestamp, ((son_ul32)(p_pci_list_node->absolute_cgi_gaurd_time_in_secs)));
            /* SPR 15831 End */

            /// Get Next Node
            p_temp_node = get_next_pending_cgi_list_node_eutran_cdma(p_pci_list_node);

            /// Delete Current Node
            anr_delete_ue_index_and_counter_list(&p_pci_list_node->eutran_cdma_pci_value.ue_index_and_counter_list);
            list_delete_node(p_list, &p_pci_list_node->anchor);
            son_mem_free(p_pci_list_node);

            /// Reset Pointer to the next Node
            p_pci_list_node = p_temp_node;
        }
    }

    SON_UT_TRACE_EXIT();
    return;
}

/*****************************************************************************
 * Function Name  : anr_cleanup_pending_cgi_list_utran
 * Inputs         : Pointer of utran_pci_requested_cgi_list
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function is used to implement an Audit kind of funtionality
 *                  which is used to clean up the pending utran cgi nodes if no cgi measurement
 *                  received for some predefined time.
 ******************************************************************************/
    son_void_t
anr_cleanup_pending_cgi_list_utran(SON_LIST* p_list)
{
    anr_utran_pci_value_node_t   *p_pci_list_node = SON_PNULL;
    anr_utran_pci_value_node_t   *p_temp_node = SON_PNULL;
    time_t  current_timestamp;

    SON_UT_TRACE_ENTER();

    if (list_count(p_list) > 0)
    {
        p_pci_list_node = get_first_pending_cgi_list_node_utran(p_list);

        /// We need to traverse this list until
        /// - Search is exhausted Or
        /// - currentTimestamp =< node.absolute_cgi_gaurd_time_in_secs
        son_time(&current_timestamp);

        while((SON_PNULL != p_pci_list_node) &&
                (p_pci_list_node->absolute_cgi_gaurd_time_in_secs < current_timestamp))
        {
            /* SPR 15831 Start */
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                    SON_BRIEF, "Stale Node Found current_timestamp = %lu, Node Absolute Expired Time = %lu",
                    (son_ul32) current_timestamp,((son_ul32)( p_pci_list_node->absolute_cgi_gaurd_time_in_secs)));

            /* SPR 15831 End */

            /// Get Next Node
            p_temp_node = get_next_pending_cgi_list_node_utran(p_pci_list_node);

            /// Delete Current Node
            anr_delete_ue_index_and_counter_list(&p_pci_list_node->utran_pci_value.ue_index_and_counter_list);
            list_delete_node(p_list, &p_pci_list_node->anchor);
            son_mem_free(p_pci_list_node);

            /// Reset Pointer to the next Node
            p_pci_list_node = p_temp_node;

        }
    }

    SON_UT_TRACE_EXIT();
    return;
}

/*****************************************************************************
 * Function Name  : anr_cleanup_pending_cgi_list_geran
 * Inputs         : Pointer of geran_pci_requested_cgi_list
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function is used to implement an Audit kind of funtionality
 *                  which is used to clean up the pending geran cgi nodes if no cgi measurement
 *                  received for some predefined time.
 ******************************************************************************/
    son_void_t
anr_cleanup_pending_cgi_list_geran(SON_LIST* p_list)
{
    anr_geran_pci_value_node_t   *p_pci_list_node   = SON_PNULL;
    anr_geran_pci_value_node_t   *p_temp_node       = SON_PNULL;
    time_t                        current_timestamp;

    SON_UT_TRACE_ENTER();

    if (list_count(p_list) > 0)
    {
        p_pci_list_node = get_first_pending_cgi_list_node_geran(p_list);

        /// We need to traverse this list until
        /// - Search is exhausted Or
        /// - currentTimestamp =< node.absolute_cgi_gaurd_time_in_secs
        son_time(&current_timestamp);

        while((SON_PNULL != p_pci_list_node) &&
                (p_pci_list_node->absolute_cgi_gaurd_time_in_secs < current_timestamp))
        {
            /* SPR 15831 Start */
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                    SON_BRIEF, "Stale Node Found current_timestamp = %lu, Node Absolute Expired Time = %lu",
                    (son_ul32) current_timestamp, ((son_ul32)(p_pci_list_node->absolute_cgi_gaurd_time_in_secs)));
            /* SPR 15831 End */

            /// Get Next Node
            p_temp_node = get_next_pending_cgi_list_node_geran(p_pci_list_node);

            /// Delete Current Node
            anr_delete_ue_index_and_counter_list(&p_pci_list_node->geran_pci_value.ue_index_and_counter_list);
            list_delete_node(p_list, &p_pci_list_node->anchor);
            son_mem_free(p_pci_list_node);

            /// Reset Pointer to the next Node
            p_pci_list_node = p_temp_node;

        }
    }

    SON_UT_TRACE_EXIT();
    return;
}

/*****************************************************************************
 * Function Name  : anr_cleanup_pending_cgi_request
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function is used to implementa an Audit kind of funtionality
 *                  which is used to clean up the pending cgi nodes if no cgi measurement
 *                  received for some predefined time.
 ******************************************************************************/
    son_void_t
anr_cleanup_pending_cgi_request(
 /* + SPR 17439 */
 void
 )
 /* - SPR 17439 */
{
    anr_cell_context_node_t *p_cell_ctxt_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_cell_ctxt_node = anr_get_first_cell_ctxt();

    while (SON_PNULL != p_cell_ctxt_node)
    {
        /// Cleanup Eutran Node List
        anr_cleanup_pending_cgi_list_eutran_cdma(&p_cell_ctxt_node->data.eutran_pci_requested_cgi_list);

        /// Cleanup Utran Node List
        anr_cleanup_pending_cgi_list_utran(&p_cell_ctxt_node->data.utran_pci_requested_cgi_list);

        /// Cleanup Geran Node List
        anr_cleanup_pending_cgi_list_geran(&p_cell_ctxt_node->data.geran_pci_requested_cgi_list);

        /// Cleanup CDMA List
        anr_cleanup_pending_cgi_list_eutran_cdma(&p_cell_ctxt_node->data.cdma_pci_requested_cgi_list);

        /// Get Next Node
        p_cell_ctxt_node = anr_get_next_node(p_cell_ctxt_node);
    }

    SON_UT_TRACE_EXIT();
    return;
}

/* SPR 15698 End */
/* eICIC feature code starts */
/*****************************************************************************
 * Function Name  : anr_set_intf_csg_scheme
 * Inputs         : scheme  - ICIC / eICIC / NO scheme to be set
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function sets the ICIC / eICIC / NO scheme.
 ******************************************************************************/
son_void_t
anr_set_intf_csg_scheme
(
 son_u32 scheme
 )
{
	SON_UT_TRACE_ENTER();
	g_anr_context.csg_scheme = scheme;
	SON_UT_TRACE_EXIT();
}
/*****************************************************************************
 * Function Name  : anr_set_intf_pico_scheme
 * Inputs         : scheme  - ICIC / eICIC / NO scheme to be set
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function sets the ICIC / eICIC / NO scheme.
 ******************************************************************************/
son_void_t
anr_set_intf_pico_scheme
(
 son_u32 scheme
 )
{
	SON_UT_TRACE_ENTER();
	g_anr_context.pico_scheme = scheme;
	SON_UT_TRACE_EXIT();
}
/*****************************************************************************
 * Function Name  : anr_set_intf_macro_scheme
 * Inputs         : scheme  - ICIC / eICIC / NO scheme to be set
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function sets the ICIC / eICIC / NO scheme.
 ******************************************************************************/
son_void_t
anr_set_intf_macro_scheme
(
 son_u32 scheme
 )
{
	SON_UT_TRACE_ENTER();
	g_anr_context.macro_scheme = scheme;
	SON_UT_TRACE_EXIT();
}
/*****************************************************************************
 * Function Name  : anr_get_intf_scheme
 * Inputs         : scheme  - ICIC / eICIC / NO scheme to be set
 * Outputs        : None
 * Returns        : son_u32 return eICIC, ICIC or No scheme
 * Description    : This function gets the ICIC / eICIC / NO scheme.
 ******************************************************************************/
son_u32
anr_get_intf_scheme
(
 son_u32     access_mode
 )
{
	son_u32     eicic_scheme;
	SON_UT_TRACE_ENTER();
	switch(access_mode)
	{
		case SON_CLOSED :
			{
				eicic_scheme = g_anr_context.csg_scheme;
				break;
			}
		case SON_HYBRID :
			{
				eicic_scheme = g_anr_context.pico_scheme;
				break;
			}
		case SON_OPEN :
			{
				eicic_scheme = g_anr_context.macro_scheme;
				break;
			}
		case SON_OPEN_FEMTO :
			{
				/** UT Fix */
				eicic_scheme = g_anr_context.pico_scheme;
				/** UT Fix */
				break;
			}
		default:
			eicic_scheme = SON_NO_INTF_SCHEME;
	}
	SON_UT_TRACE_EXIT();
	return eicic_scheme;
}
/* eICIC feature code ends */
/* Spr 17753 Changes Start */
/*****************************************************************************
 * Function Name  : anr_handle_nr_add_timer_expiry 
 * Inputs         : p_cell_context 
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function is used to  handle nr add timer expiry on, 
 *                  this timer expiry it clean up second degree neighbour list 
 ******************************************************************************/
son_void_t
anr_handle_nr_add_timer_expiry
(
 anr_cell_context_t * p_cell_context
 )
{
    anr_second_degree_nr_list_node_t *p_list_node  = SON_PNULL;
    SON_LIST_NODE                   *p_node         = SON_PNULL;
    SON_LIST_NODE                   *p_tmpnode      = SON_PNULL;
    time_stamp_t curr_ts;
    son_u32 retVal = 0;
    SON_UT_TRACE_ENTER();
     
    p_cell_context->son_nr_add_timer_id = SON_PNULL;
    son_time(&curr_ts);

    /* Delele the second degree list node */
    p_node = get_first_list_node(&(p_cell_context->second_degree_nbr_list));
    while (SON_PNULL != p_node) 
    {
        p_list_node = YMEMBEROF(anr_second_degree_nr_list_node_t, nr_nbr_anchor, p_node);

        retVal = (son_u32)son_difftime(curr_ts, p_list_node->ts); 
        if(retVal >= p_cell_context->nr_add_wait_timer_val)
        {
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                    SON_BRIEF,
                    "anr_handle_nr_add_timer_expiry:"
                    "Deleted node for nbr cell id [0x%x]",
                    son_convert_char_cell_id_to_int_cell_id
                    (p_list_node->nr_nbr_data.first_degree_nbr_cgi.cell_identity));

            p_tmpnode = get_next_list_node(&p_list_node->nr_nbr_anchor);

            list_delete_node(&(p_cell_context->second_degree_nbr_list), 
                    &p_list_node->nr_nbr_anchor);

            anr_clear_nr_nbrs_list(&p_list_node->nr_nbr_data.anr_nr_nbr_list);

            son_mem_free(p_list_node);
            p_node = p_tmpnode;
        }
        else
        {    
            p_node = get_next_list_node(&p_list_node->nr_nbr_anchor);
        }
    }
    if(0 < list_count(&p_cell_context->second_degree_nbr_list))
    {
        /* Start The timer again as list is not empty */ 
        p_cell_context->son_nr_add_timer_id = 
            son_start_timer(p_cell_context->nr_add_wait_timer_val *
                    SON_MILLISECONDS_IN_A_SECOND,
                    &p_cell_context->cell_id,
                    sizeof(p_cell_context->cell_id),
                    SON_FALSE);
    }
    SON_UT_TRACE_EXIT();
}/* anr_handle_nr_add_timer_expiry */

/*****************************************************************************
 * Function Name  : anr_check_if_nr_add_timer_or_conflict_back_off_timer_expired 
 * Inputs         : timer_id,p_cell_context,p_expiry_flag
 * Outputs        : p_cell_context 
 * Returns        : son_bool_et - SON_TRUE/SON_FALSE 
 * Description    : This function is used to check if backoff timer is expired  
 ******************************************************************************/
son_bool_et
anr_check_if_nr_add_timer_or_conflict_back_off_timer_expired
(
 son_timer_t timer_id,     /* Identity of the expired timer */
 anr_cell_context_t **p_cell_context, /* Pointer to cell context */
 anr_timer_expiry_flag_et * p_expiry_flag /* Pointer to flag for timer type */
 )
{
    son_bool_et   retVal = SON_FALSE;
    anr_cell_context_node_t   *p_cell_ctxt_tpl              = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_cell_ctxt_tpl =  anr_get_first_cell_ctxt();

    while (SON_PNULL != p_cell_ctxt_tpl)
    {    
        *p_cell_context = &p_cell_ctxt_tpl->data;

        if((*p_cell_context)->son_nr_add_timer_id == timer_id)
        {
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                    SON_BRIEF,
                    "anr_check_if_nr_add_timer_or_conflict_back_off_timer_expired: NR add timer expire"
                    " for  Cell [Cell Id: 0x%x] ",
                    son_convert_char_cell_id_to_int_cell_id((*p_cell_context)->cell_id.cell_identity));
            *p_expiry_flag = NR_ADD_TIMER_EXPIRED; 
            retVal = SON_TRUE;
            break;
        }
        else if((*p_cell_context)->son_backoff_timer_id == timer_id)
        {    
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name,
                    SON_BRIEF,
                    "anr_check_if_nr_add_timer_or_conflict_back_off_timer_expired: backoff timer expired"
                    " for  Cell [Cell Id: 0x%x] ",
                    son_convert_char_cell_id_to_int_cell_id((*p_cell_context)->cell_id.cell_identity));
            *p_expiry_flag = BACK_OFF_TIMER_EXPIRED; 
            retVal = SON_TRUE;
            break;
        }    

        p_cell_ctxt_tpl = anr_get_next_node(p_cell_ctxt_tpl);
    }    

    SON_UT_TRACE_EXIT();
    return retVal;
} /* anr_check_if_nr_add_timer_or_conflict_back_off_timer_expired */

/*****************************************************************************
 * Function Name  : anr_handle_conflict_back_off_timer_expiry 
 * Inputs         : p_cell_context, p_timer_buf 
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function is used handle conflict back off timer, 
 *                  this timer expiry it sends conflict ind to OAM 
 ******************************************************************************/
son_void_t
anr_handle_conflict_back_off_timer_expiry
(
 anr_cell_context_t * p_cell_context
 )
{
    son_anr_pci_conflict_confusion_ind_t * p_msg    = SON_PNULL;
    SON_HASH * p_hash                               = SON_PNULL;
    anr_intra_rat_hash_nr_tuple_t * p_tpl           = SON_PNULL;
    son_bool_et is_conflict_found                   = SON_FALSE;
    anr_intra_rat_hash_rec_t   *p_hash_rec          = SON_PNULL
    
    SON_UT_TRACE_ENTER();

    p_msg = (son_anr_pci_conflict_confusion_ind_t *)
        son_mem_get(sizeof(son_anr_pci_conflict_confusion_ind_t));

    if(SON_PNULL == p_msg)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                " anr_handle_conflict_back_off_timer_expiry: Memory allocation failure");

        SON_UT_TRACE_EXIT();
        return ;
    }
    SON_MEMSET(p_msg,0,sizeof(son_anr_pci_conflict_confusion_ind_t));
     
    p_hash = &p_cell_context->intra_rat_neighbor_table;
    p_tpl = anr_get_first_nr_from_intra_rat_hash_table(p_hash);
    while (SON_PNULL != p_tpl)
    {
        p_hash_rec = &p_tpl->data;

        if(SON_TRUE == anr_is_serving_conflicts_with_inserted_neighbour(p_cell_context,p_hash_rec,p_msg))
        {
            is_conflict_found = SON_TRUE;
            break;
        }
        p_tpl = anr_get_next_nr_from_intra_rat_hash_table(p_hash,
                &p_tpl->anchor);
    }
    if (SON_TRUE == is_conflict_found)
    {
        if (SON_PCI_ENABLE ==  anr_get_pci_module_mode())
        {
            /* Send the PCI Conflict IND from here */
            SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_DETAILED,
                    "anr_handle_conflict_back_off_timer_expiry:"
                    " Sending PCI CONFLICT Indication"
                    " for served Cell ID: [0x%x]"
                    " and pci : %d\n",
                    son_convert_char_cell_id_to_int_cell_id(p_cell_context->cell_id.cell_identity),
                    p_msg->pci);

            son_create_send_buffer((son_void_t *)p_msg, SON_ANR_MODULE_ID, SON_APPS_MODULE_ID,
                    SONANR_PCI_CONFLICT_CONFUSION_IND, sizeof(son_anr_pci_conflict_confusion_ind_t));
        }
        else
        {
              SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_DETAILED,
                    "anr_handle_conflict_back_off_timer_expiry:"
                    " PCI CONFLICT Detcted"
                    " for served Cell ID: [0x%x]"
                    " and pci : %d\n"
                    " But not sending cnflict to OAM"
                    " as pci_module_mode is disabled",
                    son_convert_char_cell_id_to_int_cell_id(p_cell_context->cell_id.cell_identity),
                    p_msg->pci);
        }
    }
    else
    {

        SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, SON_DETAILED,
                "anr_handle_conflict_back_off_timer_expiry:"
                " conflict Back off Indication timer [Id : %p] expired"
                " for served Cell ID: [0x%x]"
                " not sending the conflict IND to OAM as conflict is cleared",
                p_cell_context->son_backoff_timer_id,
                son_convert_char_cell_id_to_int_cell_id(p_cell_context->cell_id.cell_identity));
            
    } 
    /* Free the Timer buff */
    if(p_msg != SON_PNULL)
    {
        son_mem_free(p_msg);
        p_msg = SON_PNULL;
    }
    p_cell_context->son_backoff_timer_id = SON_PNULL;

    SON_UT_TRACE_EXIT();

}/* anr_handle_conflict_back_off_timer_expiry */ 
/* Spr 17753 Changes End */
