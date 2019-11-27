/******************************************************************************
 *
 * ARICENT 
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_anr_table.c $
 *
 *******************************************************************************
 *
 * File Description:This file contains the functions for handling the
 *                  neighbor relation table
 *
 *******************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *  Feb, 2012     Komal/Shekhar                  Initial
 *  May, 2012     Shilpi                         Changes to support Inter RAT 
 *                                               NRT (UTRAN neighbors only)
 *  Aug, 2012     Komal/Shekhar                  GERAN support added
 *  June,2013     Rashi/Swetank                  CDMA2000 support addition
 *  May, 2014     Shilpi                         Coverity Fixes  
 *  June,2014     Shilpi                         SPR-11877 Fix
 ******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_defines.h>
#include <son_anr_table.h>
#include <son_utils.h>
#include <son_anr_utils.h>
#include <son_anr_enb_ctxt_mgr.h>
#include <son_mlb_ctxt_mgr.h>
#include <son_anr_ctxt_mgr.h>  /*SPR-9574*/

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern const son_8 *p_son_anr_facility_name;
extern const son_s8 *CIO_NAMES[];
extern anr_global_t g_anr_context;

/*RELEASE1_2_4 Changes*/

/****************************************************************************
 * Function Name  : anr_determine_utran_freq_stats_list
 * Inputs         : anr_cell_context_t - 
 *                  p_nbr_info         - 
 * Outputs        : None
 * Returns        : SON_LIST - 
 * Description    : 
 ****************************************************************************/
SON_LIST *
anr_determine_utran_freq_stats_list
(
 anr_cell_context_t *p_cell_ctxt,
 son_utran_physical_cell_id_t pci
 )
{
	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt or pci is NULL");

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if (SON_PCI_FDD_PRESENT == pci.bitmask)
	{
		return &p_cell_ctxt->utran_freq_fdd_stats_list;
	}
	else if (SON_PCI_TDD_PRESENT == pci.bitmask)
	{
		return &p_cell_ctxt->utran_freq_tdd_stats_list;
	}

	SON_UT_TRACE_EXIT();
	return SON_PNULL;
}

/****************************************************************************
 * Function Name  : anr_get_no_of_xrtt_nbrs_per_frequency
 * Inputs         : p_cdma_xrtt_freq_stats_node  - Pointer to CDMA2000 XRTT freq
 *                  stats node
 * Outputs        : None
 * Returns        : son_u16 -  total numbers of neighbours
 * Description    : This function returns the CDMA XRTT neighbor count per
 *                  ARFCN
 ****************************************************************************/
son_u16
anr_get_no_of_xrtt_nbrs_per_frequency
(
 anr_cdma_xrtt_freq_stats_node_t *p_cdma_xrtt_freq_stats_node
 )
{
	SON_UT_TRACE_ENTER();
	son_u16 total_no_of_nbrs = 0; 


	total_no_of_nbrs = p_cdma_xrtt_freq_stats_node->arfcn_stats.num_bs_true_ho_allwd +
		p_cdma_xrtt_freq_stats_node->arfcn_stats.num_bs_true_ho_blklstd +
		p_cdma_xrtt_freq_stats_node->arfcn_stats.num_bs_false_ho_allwd +
		p_cdma_xrtt_freq_stats_node->arfcn_stats.num_bs_false_ho_blklstd;

	SON_UT_TRACE_EXIT();
	return total_no_of_nbrs;
}

/****************************************************************************
 * Function Name  : anr_get_no_of_hrpd_nbrs_per_frequency
 * Inputs         : p_cdma_hrpd_freq_stats_node  : Pointer to CDMA2000 HRPD freq
 *                  stats node
 * Outputs        : None
 * Returns        : son_u16 -   total numbers of neighbours
 * Description    : This function returns the CDMA2000 HRPD neighbor count per
 *                  ARFCN
 ****************************************************************************/
son_u16
anr_get_no_of_hrpd_nbrs_per_frequency
(
 anr_cdma_hrpd_freq_stats_node_t *p_cdma_hrpd_freq_stats_node
 )
{
	SON_UT_TRACE_ENTER();
	son_u16 total_no_of_nbrs = 0; 

	total_no_of_nbrs = p_cdma_hrpd_freq_stats_node->arfcn_stats.num_bs_true_ho_allwd +
		p_cdma_hrpd_freq_stats_node->arfcn_stats.num_bs_true_ho_blklstd +
		p_cdma_hrpd_freq_stats_node->arfcn_stats.num_bs_false_ho_allwd +
		p_cdma_hrpd_freq_stats_node->arfcn_stats.num_bs_false_ho_blklstd;

	SON_UT_TRACE_EXIT();
	return total_no_of_nbrs;
}

/****************************************************************************
 * Function Name  : anr_get_ho_status_of_eutran_nr
 * Inputs         : p_intra_nr  - Pointer to intra rat neighbour info data
 * Outputs        : None
 * Returns        : son_ho_status_et - SON handover status code for eutran nbr
 * Description    : This function returns the handover status of eutran son NBR
 ****************************************************************************/
son_ho_status_et
anr_get_ho_status_of_eutran_nr
(
 son_void_t *p_intra_nr
 )
{
	son_ho_status_et ret = SON_HO_PROHIBITED;


	SON_UT_TRACE_ENTER();
	if (p_intra_nr == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_intra_nr is NULL");
		SON_UT_TRACE_EXIT();
		return ret;
	}

	/*If HO Status is allowed*/
	if (SON_HO_ALLOWED == ((son_intra_rat_neighbor_info_t *)p_intra_nr)->ho_status)
	{
		ret = SON_HO_ALLOWED;
	} /*If HO status is blacklisted*/
	else if (SON_NR_LOCKED == ((son_intra_rat_neighbor_info_t *)p_intra_nr)->nr_status)
	{
		ret = SON_HO_UNDEFINED;
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* anr_get_ho_status_of_eutran_nr */

/****************************************************************************
 * Function Name  : is_intra_rat_nbr_notifiable_to_rrm
 * Inputs         : p_intra_nr  - Pointer to intra rat neighbour info structure
 * Outputs        : None
 * Returns        : son_bool_et - SON_FALSE / SON_TRUE
 * Description    : This function returns the handover status of eutran son NBR
 ****************************************************************************/
son_bool_et
is_intra_rat_nbr_notifiable_to_rrm
(
 son_intra_rat_neighbor_info_t *p_nbr_info
 )
{
	SON_UT_TRACE_ENTER();
	son_bool_et is_nbr_notf_to_rrm = SON_FALSE;
	son_u32 bitmask = 0;
	son_ho_status_et ho_status = SON_HO_UNDEFINED;

	if (SON_PNULL == p_nbr_info)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
				"is_intra_rat_nbr_notifiable_to_rrm:Validation failure "
				"as p_nbr_info is SON_PNULL");
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	if (!(((SON_ACCESS_MODE_PRESENT & p_nbr_info->bitmask) &&     /*Condition modified for SPR-10230*/
					((SON_OPEN == p_nbr_info->access_mode)||(SON_OPEN_FEMTO  ==  p_nbr_info->access_mode))) && 
				(SON_INTRA_CSG_ID_PRESENT & p_nbr_info->bitmask)))
	{
		bitmask = SON_CM_CELL_OFFSET_PRESENT | SON_CELL_OFFSET_PRESENT | SON_TAC_PRESENT;
		ho_status = anr_get_ho_status_of_eutran_nr(p_nbr_info);
		if (((p_nbr_info->bitmask & bitmask) == bitmask)  && SON_HO_PROHIBITED != ho_status)
		{
			is_nbr_notf_to_rrm = SON_TRUE;    
		}
#ifdef LTE_EMBMS_SUPPORTED
		if (p_nbr_info->bitmask & SON_EMBMS_MBMSFN_INFO_PRESENT )
		{
			is_nbr_notf_to_rrm = SON_TRUE;    
		}
#endif
	}

	SON_UT_TRACE_EXIT();
	return is_nbr_notf_to_rrm;
} /* is_intra_rat_nbr_notifiable_to_rrm */

/****************************************************************************
 * Function Name  : anr_vaildate_insertion_of_nr
 * Inputs         : p_cell_ctxt  - Pointer to cell context
 *                  p_nbr_info   - Pointer to intra rat neighbour info
 *                  p_error_code - Pointer to son error code variable
 * Outputs        : None
 * Returns        : son_bool_et - SON_FALSE / SON_TRUE
 * Description    : This function validates the whether the new neighbour should
 *                  be send to RRM.
 ****************************************************************************/
son_bool_et
anr_vaildate_insertion_of_nr
(
 anr_cell_context_t *p_cell_ctxt,
 son_intra_rat_neighbor_info_t *p_nbr_info,
 son_error_et *p_error_code
 )
{
	son_bool_et  is_nr_send_to_rrm = SON_FALSE;
	anr_bs_allowed_node_t *p_bs_allowed_node = SON_PNULL;
	son_ho_status_et  ho_status = SON_HO_UNDEFINED;
	son_u32 inter_earfcn_list_size  = 0;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_nbr_info || SON_PNULL == p_error_code)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
				"Validation failure as p_cell_ctxt"
				"p_nbr_info or p_error_code is SON_PNULL");
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	do
	{
		/*Step1: Check if the Cell Specific offset, CM cell specific offset,
		 * access mode, and CSG identity are  present. Check the nr status.
		 * If it is PROHIBITED then it will not be send to RRM  */
		if (SON_FALSE == is_intra_rat_nbr_notifiable_to_rrm(p_nbr_info))
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
					"Neighbor [0x%x] can be inserted but it will not be send to RRM due to restriction",
					son_convert_char_cell_id_to_int_cell_id(p_nbr_info->nbr_cgi.cell_identity));
			*p_error_code = SON_NO_ERROR;
			is_nr_send_to_rrm = SON_FALSE;
			break;
		}

		ho_status =  anr_get_ho_status_of_eutran_nr(p_nbr_info);
		p_bs_allowed_node =  anr_find_record_in_bs_allowed_stats_list(
				&p_cell_ctxt->bs_allowed_stats_list,
				p_nbr_info->earfcn);

		/*Get the count of the inter EARFCNs configured*/
		inter_earfcn_list_size =  list_count(&p_cell_ctxt->bs_allowed_stats_list);
		if (SON_PNULL != anr_find_record_in_bs_allowed_stats_list(
					&p_cell_ctxt->bs_allowed_stats_list,
					anr_get_earfcn(p_cell_ctxt)))
		{
			inter_earfcn_list_size--;
		}

		/*Step2:If there are already 8 inter EARFCNs send to RRM and 
		 *  a given Neighbor doesn't belong to any of these 8 EARFCNs */
		if (p_nbr_info->earfcn != anr_get_earfcn(p_cell_ctxt))
		{
			if (SON_PNULL == p_bs_allowed_node &&
					((MAX_NUM_OF_ALLOWED_EARFCNS_TO_RRM - 1)  <= inter_earfcn_list_size)) 
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
						"Neighbor [0x%x] can not be inserted because number of"
						" Inter EARFCNs send are [%d] which is the max allowed value",
						son_convert_char_cell_id_to_int_cell_id(p_nbr_info->nbr_cgi.cell_identity),
						list_count(&p_cell_ctxt->bs_allowed_stats_list));
				*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;
				is_nr_send_to_rrm = SON_TRUE;
				break;
			}
		}

		/*Step3:Make the is_nr_send_to_rrm to SON_TRUE*/
		is_nr_send_to_rrm = SON_TRUE;
		*p_error_code = SON_NO_ERROR;

		/*Step4:Get the Broadcast statistics node for the given EARFCN value*/
		if (SON_PNULL != p_bs_allowed_node)
		{
			if (SON_TRUE == p_nbr_info->broadcast_status)
			{
				if (SON_HO_ALLOWED == ho_status)
				{
					if (SON_RRM_MAX_INTRA_RAT_BROADCAST_HO_ALLOWED_NBRS <=
							p_bs_allowed_node->bs_allowed_per_earfcn_record.
							bs_allowed_ho_allowed_nbrs_count)
					{
						SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
								"Neighbor [0x%x] can "
								"not be inserted in the NRT table for serving [0x%x] "
								"as there are already [%d] neighbors in EARFCN [%d] having HO Status as HO ALLOWED "
								"and BROADCAST STATUS as Allowed",
								son_convert_char_cell_id_to_int_cell_id(p_nbr_info->nbr_cgi.cell_identity),
								son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
								p_bs_allowed_node->bs_allowed_per_earfcn_record.
								bs_allowed_ho_allowed_nbrs_count,
								p_nbr_info->earfcn);
						*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;
						break;
					}
				}
				else if (SON_RRM_MAX_INTRA_RAT_BROADCAST_HO_BLKLSTED_NBRS <=
						p_bs_allowed_node->bs_allowed_per_earfcn_record.
						bs_allowed_ho_blklisted_nbrs_count)
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
							"Neighbor [0x%x] can "
							" not be inserted in the NRT table for serving cell [0x%x]"
							" as there are already [%d] neighbors in EARFCN [%d] having HO Status as HO BLACKLISTED"
							" and BROADCAST STATUS as not allowed",
							son_convert_char_cell_id_to_int_cell_id(p_nbr_info->nbr_cgi.cell_identity),
							son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
							p_bs_allowed_node->bs_allowed_per_earfcn_record.
							bs_allowed_ho_blklisted_nbrs_count,
							p_nbr_info->earfcn);
					*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;
					break;
				}
			}
			else
			{
				if (SON_HO_ALLOWED == ho_status)
				{   
					if (SON_RRM_MAX_INTRA_RAT_NON_BROADCAST_HO_ALLOWED_NBRS <=
							p_bs_allowed_node->bs_allowed_per_earfcn_record.
							bs_not_allowed_ho_allowed_nbrs_count)
					{
						SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
								"Neighbor [0x%x] can"
								" not be inserted in the NRT table for serving cell [0x%x]"
								" as there are already [%d] neighbors in EARFCN [%d] having HO Status as HO ALLOWED "
								" and BROADCAST STATUS as Not  Allowed",
								son_convert_char_cell_id_to_int_cell_id(p_nbr_info->nbr_cgi.cell_identity),
								son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
								p_bs_allowed_node->bs_allowed_per_earfcn_record.
								bs_not_allowed_ho_allowed_nbrs_count,
								p_nbr_info->earfcn);
						*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;
						break;
					}
				}
				else if (SON_RRM_MAX_INTRA_RAT_NON_BROADCAST_HO_BLKLSTED_NBRS <=
						p_bs_allowed_node->bs_allowed_per_earfcn_record.
						bs_not_allowed_ho_blklisted_nbrs_count)
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
							"Neighbor [0x%x] can"
							"not be inserted in the NRT table for serving cell [0x%x]"
							"as there are already [%d] neighbors in EARFCN [%d] having HO Status as HO BLACKLISTED"
							"and BROADCAST STATUS as Not not allowed",
							son_convert_char_cell_id_to_int_cell_id(p_nbr_info->nbr_cgi.cell_identity),
							son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
							p_bs_allowed_node->bs_allowed_per_earfcn_record.
							bs_not_allowed_ho_blklisted_nbrs_count,
							p_nbr_info->earfcn);
					*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;
					break;
				}
			}
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
				"Neighbor [0x%x] can"
				" be inserted in the NRT table for serving cell [0x%x]"
				" send to RRM",
				son_convert_char_cell_id_to_int_cell_id(p_nbr_info->nbr_cgi.cell_identity),
				son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
	} while (0);

	SON_UT_TRACE_EXIT(); 
	return is_nr_send_to_rrm;
} /* anr_vaildate_insertion_of_nr */

/****************************************************************************
 * Function Name  : anr_chk_old_nr_was_not_send_to_rrm
 * Inputs         : p_cell_ctxt  - Pointer to cell context
 *                  p_old_hash   - Pointer to intra rat hash rec containing old
 *                                 hash data that needs to be validated
 *                  p_new_hash   - Pointer to new intra rat hash rec containing new
 *                                 hash data that needs to be modified
 *                  p_is_old_nr_send - flag that checks whether the old NR was sendable
 *                                     to RRM
 *                  p_is_rrm_notif_reqd_for_new_nr - flag to check if rrm should be notified
 *                                                   for new neighbour insertion
 *                  p_error_code - Pointer to son error code variable
 * Outputs        : None
 * Returns        : son_bool_et - SON_FALSE / SON_TRUE
 * Description    : This function checks if old neighbour was send to rrm or not
 ****************************************************************************/
son_bool_et
anr_chk_old_nr_was_not_send_to_rrm
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_intra_rat_hash_rec_t  *p_old_hash,
 anr_intra_rat_hash_rec_t  *p_new_hash,
 son_bool_et               *p_is_old_nr_send,
 son_bool_et               *p_is_rrm_notif_reqd_for_new_nr,
 son_error_et              *p_error_code
 )
{
	son_bool_et ret = SON_TRUE;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_old_hash || SON_PNULL == p_new_hash)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"p_old_hash or p_new_hash is NULL");
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	/*If old NR was not send and new NR is also not sendable to RRM */
	if (SON_FALSE == *p_is_rrm_notif_reqd_for_new_nr)
	{
		/*Modification done is allowed*/
		*p_error_code = SON_NO_ERROR; 

		/*RRM notifcation for new NR is not allowed */
		*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

		/*RRM notifcation for old NR is not allowed*/
		*p_is_old_nr_send = SON_FALSE;
	}
	else /*If old NR was not send but new NR is sendable to RRM*/
	{
		anr_bs_allowed_node_t *p_list_node = SON_PNULL;  
		son_bool_et  is_ho_allowed_stat    = SON_FALSE;
		son_bool_et  is_ho_blk_stat        = SON_FALSE;
		son_u32     inter_earfcn_list_size = 0;

		/*Get the BS allowed record node for the new EARFCN*/         
		p_list_node =  anr_find_record_in_bs_allowed_stats_list(
				&p_cell_ctxt->bs_allowed_stats_list,
				p_new_hash->neighbor_info.earfcn);

		/*Get the inter EARFCNs count*/
		inter_earfcn_list_size = list_count(&p_cell_ctxt->bs_allowed_stats_list);
		if (SON_PNULL != anr_find_record_in_bs_allowed_stats_list(
					&p_cell_ctxt->bs_allowed_stats_list,
					anr_get_earfcn(p_cell_ctxt)))
		{
			inter_earfcn_list_size--;
		} 

		/*If the EARFCN for the new is not send to RRM and 
		 * the count is already exceeded*/
		if (p_new_hash->neighbor_info.earfcn != anr_get_earfcn(p_cell_ctxt) &&
				SON_PNULL == p_list_node && 
				(MAX_NUM_OF_ALLOWED_EARFCNS_TO_RRM - 1)  <= inter_earfcn_list_size)
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
					"anr_chk_old_nr_was_not_send_to_rrm:The Modification for neighbor [Cell_id:0x%x] can not"
					"be done as ANR has already configure  [%d]EARFCNs at RRM and this is a new EARCN",
					(son_convert_char_cell_id_to_int_cell_id(
										 p_new_hash->neighbor_info.nbr_cgi.cell_identity)), inter_earfcn_list_size);

			/*Modification done is not allowed*/
			*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;

			/*RRM notifcation for new NR is not allowed */
			*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

			/*RRM notifcation for old NR is not allowed*/
			*p_is_old_nr_send = SON_FALSE;

			/*return from here*/
			SON_UT_TRACE_EXIT();
			return SON_FALSE;
		}

		/*Get the ho_status of the new NR*/ 
		if (SON_HO_ALLOWED == anr_get_ho_status_of_eutran_nr(
					&p_new_hash->neighbor_info))
		{
			is_ho_allowed_stat = SON_TRUE;                  
		}
		else
		{
			is_ho_blk_stat = SON_TRUE;    
		}

		if (SON_PNULL != p_list_node)
		{
			if (SON_TRUE == increment_bs_allowed_nbr_count(
						p_cell_ctxt,
						p_list_node,
						p_new_hash->neighbor_info.broadcast_status,
						is_ho_allowed_stat,
						is_ho_blk_stat ))
			{
				/*Modification done is allowed*/
				*p_error_code = SON_NO_ERROR; 

				/*RRM notifcation for new NR is  allowed */
				*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

				/*RRM notification for old NR is not allowed*/
				*p_is_old_nr_send = SON_FALSE;
			}
			else
			{
				/*Roll back the modification done */
				*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;

				/*RRM notifcation for new NR is not allowed */
				*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

				/*RRM notification for old NR is not allowed*/
				*p_is_old_nr_send = SON_FALSE;
			}
		}
		else
		{
			/*Create the bs allowed status record for new NR's EARFCN*/
			p_list_node = anr_insert_record_in_bs_allowed_stats_list(
					&p_cell_ctxt->bs_allowed_stats_list,
					p_new_hash->neighbor_info.earfcn,
					p_new_hash->neighbor_info.broadcast_status,
					is_ho_allowed_stat,
					is_ho_blk_stat);

			if (SON_PNULL == p_list_node)
			{
				/*Roll back the modification done */
				*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;

				/*RRM notifcation for new NR is not allowed */
				*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

				/*RRM notification for old NR is not allowed*/
				*p_is_old_nr_send = SON_FALSE;
			}
			else
			{
				/* Modification done is allowed */
				*p_error_code = SON_NO_ERROR;

				/*RRM notifcation for new NR is allowed */
				*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

				/*RRM notification for old NR is not allowed*/
				*p_is_old_nr_send = SON_FALSE;
			}
		}
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* anr_chk_old_nr_was_not_send_to_rrm */

/****************************************************************************
 * Function Name  : anr_chk_old_nr_was_send_to_rrm_with_bs
 * Inputs         : p_cell_ctxt  - Pointer to cell context
 *                  p_old_hash   - Pointer to intra rat hash rec containing old
 *                                 hash data that needs to be validated
 *                  p_new_hash   - Pointer to new intra rat hash rec containing new
 *                                 hash data that needs to be modified
 *                  p_is_old_nr_send - flag that checks whether the old NR was sendable
 *                                     to RRM
 *                  p_is_rrm_notif_reqd_for_new_nr - flag to check if rrm should be notified
 *                                                   for new neighbour insertion
 *                  p_error_code - Pointer to son error code variable
 * Outputs        : None
 * Returns        : son_bool_et - SON_FALSE / SON_TRUE
 * Description    : This function checks if old neighbour was send to rrm or not
 *                  with broadcast status
 ****************************************************************************/
static son_void_t
anr_chk_old_nr_was_send_to_rrm_with_bs
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_intra_rat_hash_rec_t  *p_old_hash,
 anr_intra_rat_hash_rec_t  *p_new_hash,
 son_bool_et               *p_is_old_nr_send,
 son_bool_et               *p_is_rrm_notif_reqd_for_new_nr,
 son_error_et              *p_error_code
 )
{
	anr_bs_allowed_node_t *p_list_node_old_earfcn = SON_PNULL;  

	son_bool_et      is_ho_allowed_stat_old    = SON_FALSE;
	son_bool_et      is_ho_blk_stat_old        = SON_FALSE;
	son_ho_status_et ho_sts_old                = SON_HO_UNDEFINED;

	son_bool_et      is_ho_allowed_stat_new    = SON_FALSE;
	son_bool_et      is_ho_blk_stat_new        = SON_FALSE;
	son_ho_status_et ho_sts_new                = SON_HO_UNDEFINED;
	anr_bs_allowed_node_t *p_list_node_new_earfcn = SON_PNULL;


	SON_UT_TRACE_ENTER();

	/*Get the BS allowed record node for the old EARFCN*/         
	p_list_node_old_earfcn =  anr_find_record_in_bs_allowed_stats_list(
			&p_cell_ctxt->bs_allowed_stats_list,
			p_old_hash->neighbor_info.earfcn);

	/*If p_list_node_old_earfcn is getting as NULL pointer ,roll back the changes.
	 * basically this is the wrong handling , we should be
	 * sending the old NR in this case,but this scenario should never happen*/
	if (SON_PNULL == p_list_node_old_earfcn)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_BRIEF,
				"Falied to get the BS ALLOWED record for"
				"EARFCN[%d] in a serving cell [Cell_id:0x%p] hence"
				"modification rolled back",
				p_old_hash->neighbor_info.earfcn,
				((son_u32 *)p_cell_ctxt->cell_id.cell_identity));

		*p_error_code = SON_ERR_ANR_MODIFICATION_NOT_ALLOWED;    
		*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;
		*p_is_old_nr_send = SON_FALSE;
		SON_UT_TRACE_EXIT();
		return;
	}

	/*Get the ho_status of the old  NR*/
	ho_sts_old =  anr_get_ho_status_of_eutran_nr(&p_old_hash->neighbor_info);

	if (SON_HO_ALLOWED == ho_sts_old)
	{
		is_ho_allowed_stat_old = SON_TRUE;                  
	}
	else
	{
		is_ho_blk_stat_old    = SON_TRUE;    
	}

	/*Get the HO status*/
	ho_sts_new = anr_get_ho_status_of_eutran_nr(&p_new_hash->neighbor_info );
	if (SON_HO_ALLOWED == ho_sts_new)
	{
		is_ho_allowed_stat_new = SON_TRUE;                  
	}
	else
	{
		is_ho_blk_stat_new  = SON_TRUE;    
	}

	/*If the p_list_node_old_earfcn is getting zero nbrs 
	 * when ho status is ho Allowed  and broadcast status is true/false of old nR or
	 * getting zero nbrs when ho allowed is ho blklisted for and broadcast status is true/false of old NR*/
	if (((SON_TRUE ==  is_ho_allowed_stat_old) && (SON_TRUE  == p_old_hash->neighbor_info.broadcast_status)
				&& (0 == p_list_node_old_earfcn->bs_allowed_per_earfcn_record.bs_allowed_ho_allowed_nbrs_count))    ||
			((SON_TRUE ==  is_ho_allowed_stat_old) && (SON_FALSE == p_old_hash->neighbor_info.broadcast_status)
			 && (0 == p_list_node_old_earfcn->bs_allowed_per_earfcn_record.bs_not_allowed_ho_allowed_nbrs_count)) ||
			((SON_TRUE == is_ho_blk_stat_old) && (SON_TRUE == p_old_hash->neighbor_info.broadcast_status)
			 && (0 == p_list_node_old_earfcn->bs_allowed_per_earfcn_record.bs_allowed_ho_blklisted_nbrs_count))  ||
			((SON_TRUE == is_ho_blk_stat_old) && (SON_FALSE == p_old_hash->neighbor_info.broadcast_status)
			 && (0 == p_list_node_old_earfcn->bs_allowed_per_earfcn_record.bs_not_allowed_ho_blklisted_nbrs_count)))
	{
        /* SPR 22304 Fix Start */
		SON_LOG(
				anr_get_log_mode(), p_son_anr_facility_name,
				SON_INFO,
				"Falied to decrement the count value as "
				"num of ho allowed nbrs with broadcast allowed is [%d]"
				"num of ho allowed nbrs with broadcast not allowed is [%d]"
				"num of ho blacklisted nbrs with broadcasr allowed is [%d] and "
				"num of ho blacklisted nbrs with broadcast not allowed is [%d] when the HO "
				"status for the Old NR is [%d] with broadcast status as [%d] for EARFCN [%d] for serving cell[cell_id:0x%x]",
				p_list_node_old_earfcn->bs_allowed_per_earfcn_record.
				bs_allowed_ho_allowed_nbrs_count,
				p_list_node_old_earfcn->bs_allowed_per_earfcn_record.
				bs_not_allowed_ho_allowed_nbrs_count,
				p_list_node_old_earfcn->bs_allowed_per_earfcn_record.
				bs_allowed_ho_blklisted_nbrs_count,
				p_list_node_old_earfcn->bs_allowed_per_earfcn_record.
				bs_not_allowed_ho_blklisted_nbrs_count,
				ho_sts_old, p_old_hash->neighbor_info.broadcast_status,
				p_old_hash->neighbor_info.earfcn,
				son_convert_const_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
        /* SPR 22304 Fix End */

		*p_error_code = SON_ERR_ANR_MODIFICATION_NOT_ALLOWED;    
		*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;
		*p_is_old_nr_send = SON_FALSE;
		SON_UT_TRACE_EXIT();
		return;
	}

	if (SON_FALSE == *p_is_rrm_notif_reqd_for_new_nr)
	{
		/*Decrement the count*/
		if (SON_TRUE == decrement_bs_allowed_nbr_count(
					p_cell_ctxt,
					p_list_node_old_earfcn,
					p_old_hash->neighbor_info.broadcast_status,
					is_ho_allowed_stat_old,
					is_ho_blk_stat_old))
		{
			/*NR modficaition is allowed*/
			*p_error_code = SON_NO_ERROR; 

			/*RRM notifcation for new NR is not allowed*/
			*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

			/*RRM notification for old NR is allowed*/
			*p_is_old_nr_send = SON_TRUE;
		}
	}
	else
	{
		if (p_old_hash->neighbor_info.earfcn ==
				p_new_hash->neighbor_info.earfcn)
		{
			if ((p_old_hash->neighbor_info.broadcast_status !=
						p_new_hash->neighbor_info.broadcast_status) ||
					(ho_sts_old != ho_sts_new))
			{
				if (SON_TRUE == decrement_bs_allowed_nbr_count(
							p_cell_ctxt, p_list_node_old_earfcn,
							p_old_hash->neighbor_info.broadcast_status,
							is_ho_allowed_stat_old, is_ho_blk_stat_old))
				{
					p_list_node_old_earfcn =  anr_find_record_in_bs_allowed_stats_list(
							&p_cell_ctxt->bs_allowed_stats_list,
							p_old_hash->neighbor_info.earfcn);

					if (p_list_node_old_earfcn != SON_PNULL)
					{
						if (SON_TRUE == increment_bs_allowed_nbr_count(p_cell_ctxt,
									p_list_node_old_earfcn,
									p_new_hash->neighbor_info.broadcast_status,
									is_ho_allowed_stat_new,
									is_ho_blk_stat_new))
						{
							/*NR modification is allowed*/
							*p_error_code =  SON_NO_ERROR;

							/*RRM notification for the new NR is allowed*/
							*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

							/*check if the ho_stats is chnaged*/
							if (ho_sts_new == ho_sts_old)
							{
								/*RRM notification for the old NR is not allowed*/
								*p_is_old_nr_send = SON_FALSE;
							}
							else
							{
								/*RRM notification for the old NR is  allowed*/
								*p_is_old_nr_send = SON_TRUE;
							}
						}
						else
						{
							/*NR modification is rolled back */
							*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;

							/*RRM notifcation for the new NR is not allowed*/
							*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

							/*RRM notifcation for the old NR is not allowed*/
							*p_is_old_nr_send = SON_FALSE;

							/*Increment for the old one
							 * Coverity Fix: 25704 */
							if (SON_FALSE == increment_bs_allowed_nbr_count(p_cell_ctxt,
										p_list_node_old_earfcn,
										p_old_hash->neighbor_info.broadcast_status,
										is_ho_allowed_stat_old, is_ho_blk_stat_old ))
							{
								SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
										"Increment of bs allowed nbr count is "
										"failed for Cell Identity [Cell id:0x%x]",
										son_convert_char_cell_id_to_int_cell_id(p_old_hash->neighbor_info.nbr_cgi.
											cell_identity));
							}
						}
					}
					else
					{
                        p_list_node_old_earfcn = SON_PNULL;
						p_list_node_new_earfcn = anr_insert_record_in_bs_allowed_stats_list(
								&p_cell_ctxt->bs_allowed_stats_list,
								p_new_hash->neighbor_info.earfcn,
								p_new_hash->neighbor_info.broadcast_status,
								is_ho_allowed_stat_new,
								is_ho_blk_stat_new);

						if (SON_PNULL != p_list_node_new_earfcn)
						{
							/*NR modification is allowed*/ 
							*p_error_code =  SON_NO_ERROR; 

							/*RRM notification for the new NR is allowed*/ 
							*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

							/*check if the ho_stats is chnaged*/
							if (ho_sts_new == ho_sts_old)
							{
								/*RRM notification for the old NR is not allowed*/
								*p_is_old_nr_send = SON_FALSE;
							}
							else
							{
								/*RRM notification for the old NR is  allowed*/
								*p_is_old_nr_send = SON_TRUE;
							}
						}
					}
				}
				else
				{
					/*NR modification is rolled back */
					*p_error_code = SON_ERR_ANR_MODIFICATION_NOT_ALLOWED;

					/*RRM notifcation for the new NR is not allowed*/
					*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

					/*RRM notifcation for the old NR is not allowed*/
					*p_is_old_nr_send = SON_FALSE;
				}
			}
			else
			{
				/*NR modification is allowed*/ 
				*p_error_code =  SON_NO_ERROR; 

				/*RRM notification for the new NR is allowed*/ 
				*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

				/*check if the ho_stats is chnaged*/
				if (ho_sts_new == ho_sts_old)
				{
					/*RRM notification for the old NR is not allowed*/
					*p_is_old_nr_send = SON_FALSE;
				}
				else
				{
					/*RRM notification for the old NR is  allowed*/
					*p_is_old_nr_send = SON_TRUE;
				}
			}
		}
		else
		{
			/*Find the BS allowed contxt for the new EARFCN*/
			p_list_node_new_earfcn = anr_find_record_in_bs_allowed_stats_list(
					&p_cell_ctxt->bs_allowed_stats_list,
					p_new_hash->neighbor_info.earfcn);

			/*If node is fouund*/
			if (SON_PNULL !=  p_list_node_new_earfcn)
			{
				if (SON_TRUE == increment_bs_allowed_nbr_count(p_cell_ctxt,
							p_list_node_new_earfcn,
							p_new_hash->neighbor_info.broadcast_status,
							is_ho_allowed_stat_new,
							is_ho_blk_stat_new))
				{
					/*Decrement the count for the OLD EARFCN*/
					decrement_bs_allowed_nbr_count(p_cell_ctxt,
							p_list_node_old_earfcn,
							p_old_hash->neighbor_info.broadcast_status,
							is_ho_allowed_stat_old,
							is_ho_blk_stat_old);

					/*RRM modification is allowed */
					*p_error_code = SON_NO_ERROR;

					/*RRM notifcation for the new NR is allowed*/
					*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

					/*RRM notifcation for the old NR is allowed*/
					*p_is_old_nr_send = SON_TRUE;
				}
				else
				{
					/*NR modification is rolled back */     
					*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;

					/*RRM notifcation for the new NR is not allowed*/
					*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

					/*RRM notifcation for the old NR is not allowed*/
					*p_is_old_nr_send = SON_FALSE;
				}
			}
			else
			{
				/*Inter EARFCNs*/
				son_u32  inter_earfcn_list_size =  list_count(&p_cell_ctxt->bs_allowed_stats_list);
				if (SON_PNULL != anr_find_record_in_bs_allowed_stats_list(
							&p_cell_ctxt->bs_allowed_stats_list,
							anr_get_earfcn(p_cell_ctxt)))
				{
					inter_earfcn_list_size--;
				}

				/*If the EARFCN is inter and we have alreay max number of inter avaliable*/ 
				if (p_new_hash->neighbor_info.earfcn != anr_get_earfcn(p_cell_ctxt))
				{   
					if ((MAX_NUM_OF_ALLOWED_EARFCNS_TO_RRM - 1)  == inter_earfcn_list_size)
					{
						/*If the neighbor count that is already sent to RRM is 1 */
						if ((1 ==
									p_list_node_old_earfcn->bs_allowed_per_earfcn_record.bs_allowed_ho_allowed_nbrs_count +
									p_list_node_old_earfcn->bs_allowed_per_earfcn_record.bs_allowed_ho_blklisted_nbrs_count +
									p_list_node_old_earfcn->bs_allowed_per_earfcn_record.bs_not_allowed_ho_allowed_nbrs_count +
									p_list_node_old_earfcn->bs_allowed_per_earfcn_record.bs_not_allowed_ho_blklisted_nbrs_count)
								&&
								p_list_node_old_earfcn->bs_allowed_per_earfcn_record.earfcn !=  anr_get_earfcn(p_cell_ctxt))
						{
							/*Decrement the count for the OLD EARFCN*/
							if (SON_TRUE == decrement_bs_allowed_nbr_count(p_cell_ctxt,
										p_list_node_old_earfcn,
										p_old_hash->neighbor_info.broadcast_status,
										is_ho_allowed_stat_old,
										is_ho_blk_stat_old))
							{
								p_list_node_new_earfcn = anr_insert_record_in_bs_allowed_stats_list(
										&p_cell_ctxt->bs_allowed_stats_list,
										p_new_hash->neighbor_info.earfcn,
										p_new_hash->neighbor_info.broadcast_status,
										is_ho_allowed_stat_new,
										is_ho_blk_stat_new);

								if (SON_PNULL != p_list_node_new_earfcn)
								{
									/*RRM modification is allowed */
									*p_error_code = SON_NO_ERROR;

									/*RRM notifcation for the new NR is allowed*/
									*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

									/*RRM notifcation for the old NR is allowed*/
									*p_is_old_nr_send = SON_TRUE;
								}
								else
								{
									/*NR modification is rolled back */     
									*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;

									/*RRM notifcation for the new NR is not allowed*/
									*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

									/*RRM notifcation for the old NR is not allowed*/
									*p_is_old_nr_send = SON_FALSE;
								}

								SON_UT_TRACE_EXIT();
								return;
							}
						}
					}

					if ((MAX_NUM_OF_ALLOWED_EARFCNS_TO_RRM - 1) <= inter_earfcn_list_size)
					{
						SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
								"Cell Identity [Cell id:0x%x] is"
								"can not be modified because the number of"
								"Inter EARFCNs send are [%d] which is the max allowed value",
								son_convert_char_cell_id_to_int_cell_id(
									p_new_hash->neighbor_info.nbr_cgi.cell_identity), inter_earfcn_list_size);

						*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;    
						SON_UT_TRACE_EXIT();
						return;
					}
				}

				p_list_node_new_earfcn = anr_insert_record_in_bs_allowed_stats_list(
						&p_cell_ctxt->bs_allowed_stats_list,
						p_new_hash->neighbor_info.earfcn,
						p_new_hash->neighbor_info.broadcast_status,
						is_ho_allowed_stat_new,
						is_ho_blk_stat_new);

				if (SON_PNULL != p_list_node_new_earfcn)
				{
					/*Decrement the count for the OLD EARFCN*/
					decrement_bs_allowed_nbr_count(p_cell_ctxt,
							p_list_node_old_earfcn,
							p_old_hash->neighbor_info.broadcast_status,
							is_ho_allowed_stat_old,
							is_ho_blk_stat_old);

					/*RRM modification is allowed */
					*p_error_code = SON_NO_ERROR;

					/*RRM notifcation for the new NR is allowed*/
					*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

					/*RRM notifcation for the old NR is allowed*/
					*p_is_old_nr_send = SON_TRUE;
				}
				else
				{
					/*NR modification is rolled back */     
					*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;

					/*RRM notifcation for the new NR is not allowed*/
					*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

					/*RRM notifcation for the old NR is not allowed*/
					*p_is_old_nr_send = SON_FALSE;
				}
			}
		}
	}

	SON_UT_TRACE_EXIT();
} /* anr_chk_old_nr_was_send_to_rrm_with_bs */

/****************************************************************************
 * Function Name  : anr_determine_the_changes_in_nrt
 * Inputs         : p_cell_ctxt  - Pointer to cell context
 *                  p_old_hash   - Pointer to intra rat hash rec containing old
 *                                 hash data that needs to be validated
 *                  p_new_hash   - Pointer to new intra rat hash rec containing new
 *                                 hash data that needs to be modified
 *                  p_is_old_nr_send - flag that checks whether the old NR was sendable
 *                                     to RRM
 *                  p_is_rrm_notif_reqd_for_new_nr - flag to check if rrm should be notified
 *                                                   for new neighbour insertion
 *                  p_error_code - Pointer to son error code variable
 * Outputs        : None
 * Returns        : son_bool_et - SON_FALSE / SON_TRUE
 * Description    : This function checks the changes if any in NRT due when modifying any
 *                  neighbour in NRT
 ****************************************************************************/
son_bool_et
anr_determine_the_changes_in_nrt
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_intra_rat_hash_rec_t  *p_old_hash,
 anr_intra_rat_hash_rec_t  *p_new_hash,
 son_bool_et               *p_is_old_nr_send,
 son_bool_et               *p_is_rrm_notif_reqd_for_new_nr,
 son_error_et              *p_error_code
 )
{
	SON_UT_TRACE_ENTER();
	son_bool_et ret = SON_TRUE;
	if (SON_PNULL ==  p_cell_ctxt || SON_PNULL == p_old_hash ||
			SON_PNULL == p_new_hash || SON_PNULL == p_is_old_nr_send ||
			SON_PNULL == p_is_rrm_notif_reqd_for_new_nr ||
			SON_PNULL == p_error_code)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_DETAILED,
				"anr_determine_the_changes_in_nrt:validation can not be done as"
				"p_cell_ctxt,p_old_hash,p_new_hash,p_is_old_nr_send,"
				"p_is_rrm_notif_reqd_for_new_nr or p_error_code is SON_PNULL");
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}
	/*If old NR was not send*/
	if (SON_FALSE ==  *p_is_old_nr_send)
	{
		anr_chk_old_nr_was_not_send_to_rrm(p_cell_ctxt,
				p_old_hash,
				p_new_hash,
				p_is_old_nr_send,
				p_is_rrm_notif_reqd_for_new_nr,
				p_error_code); 
	}
	else
	{
		anr_chk_old_nr_was_send_to_rrm_with_bs(p_cell_ctxt,
				p_old_hash,
				p_new_hash,
				p_is_old_nr_send,
				p_is_rrm_notif_reqd_for_new_nr,
				p_error_code);
	}

	SON_UT_TRACE_EXIT(); 
	return ret;
} /* anr_determine_the_changes_in_nrt */

/*RELEASE1_2_4 Changes
 *RELEASE1_2_4 Changes for the Inter RAT*/

/****************************************************************************
 * Function Name  : is_utran_nbr_notifiable_to_rrm
 * Inputs         : p_nbr_info  - Pointer to utran cell neighbiour info
 * Outputs        : None
 * Returns        : son_bool_et - SON_FALSE / SON_TRUE
 * Description    : This function checks if new utran neighbour addition / modification
 *                  should be notified to RRM
 ****************************************************************************/
son_bool_et
is_utran_nbr_notifiable_to_rrm
(
 son_utran_neighbor_info_t *p_nbr_info
 )
{
	son_bool_et is_nbr_notf_to_rrm = SON_FALSE;


	SON_UT_TRACE_ENTER();
	son_u32 bitmask = 0;
	son_ho_status_et ho_status = SON_HO_UNDEFINED;

	if (SON_PNULL == p_nbr_info)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"Validation failure "
				"as p_nbr_info is SON_PNULL");
		return SON_FALSE;

		SON_UT_TRACE_EXIT();
	}

	if (!(((SON_UTRAN_ACCESS_MODE_PRESENT & p_nbr_info->bitmask) && 
					(SON_OPEN == p_nbr_info->access_mode)) && 
				(SON_UTRAN_CSG_ID_PRESENT & p_nbr_info->bitmask)))
	{
		bitmask = SON_UTRAN_CSO_PRESENT | SON_UTRAN_LAC_PRESENT | SON_UTRAN_RAC_PRESENT;
		ho_status = anr_get_ho_status_of_utran_nr(p_nbr_info);

		if ((bitmask == (p_nbr_info->bitmask & bitmask)) && SON_HO_PROHIBITED != ho_status)
		{
			is_nbr_notf_to_rrm = SON_TRUE;    
		}
	}

	SON_UT_TRACE_EXIT();
	return is_nbr_notf_to_rrm;
} /* is_utran_nbr_notifiable_to_rrm */

/****************************************************************************
 * Function Name  : is_geran_nbr_notifiable_to_rrm
 * Inputs         : p_nbr_info  - Pointer to geran cell neighbiour info
 * Outputs        : None
 * Returns        : son_bool_et - SON_FALSE / SON_TRUE
 * Description    : This function checks if new geran neighbour addition / modification
 *                  should be notified to RRM
 ****************************************************************************/
son_bool_et
is_geran_nbr_notifiable_to_rrm
(
 son_geran_neighbor_info_t *p_nbr_info
 )
{
	son_bool_et is_nbr_notf_to_rrm = SON_TRUE;


	SON_UT_TRACE_ENTER();
	son_u32 bitmask = 0;
	son_ho_status_et ho_status = SON_HO_UNDEFINED;

	if (SON_PNULL == p_nbr_info)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"Validation failure "
				"as p_nbr_info is SON_PNULL");
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	bitmask = SON_GERAN_CSO_PRESENT;

	ho_status = anr_get_ho_status_of_geran_nr(p_nbr_info);

	if (((p_nbr_info->bitmask & bitmask) != bitmask)  || SON_HO_PROHIBITED == ho_status)
	{
		is_nbr_notf_to_rrm = SON_FALSE;
	}

	SON_UT_TRACE_EXIT();
	return is_nbr_notf_to_rrm;
} /* is_geran_nbr_notifiable_to_rrm */

/******************************************************************************
 * Function Name  : is_inter_rat_nr_sendable_to_rrm
 * Inputs         : p_hash_rec - Pointer to inter rat NR record
 * Outputs        : None.
 * Returns        : son_bool_et - status flag for notifiability to rrm
 * Description    : This function is used to check if the NBR is sendable to RRM
 ******************************************************************************/
son_bool_et
is_inter_rat_nr_sendable_to_rrm
(
 anr_inter_rat_hash_rec_t *p_hash
 )
{
	son_bool_et ret = SON_FALSE;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_hash)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"p_hash is NULL");
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	if (ANR_UTRAN_NEIGHBOR == p_hash->t)
	{
		ret = is_utran_nbr_notifiable_to_rrm(&p_hash->u.utran_neighbor_info); 
	}
	else if (ANR_GERAN_NEIGHBOR == p_hash->t)
	{
		ret = is_geran_nbr_notifiable_to_rrm(&p_hash->u.geran_neighbor_info); 
	}
	else if (ANR_CDMA_NEIGHBOR == p_hash->t)
	{
		ret = is_cdma_nbr_notifiable_to_rrm(&p_hash->u.cdma_neighbor_info);
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"Wrong RAT TYPE %u mentioned", p_hash->t);
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* is_inter_rat_nr_sendable_to_rrm */

/******************************************************************************
 * Function Name  : get_ho_status_for_inter_rat_nbrs
 * Inputs         : p_hash_rec - Pointer to a the NR record
 * Outputs        : None.
 * Returns        : ho status flag - SON handover status code
 * Description    : This function is used to get the ho status of the inter rat 
 *                  NBR
 ******************************************************************************/
son_ho_status_et
get_ho_status_for_inter_rat_nbrs
(
 anr_inter_rat_hash_rec_t *p_hash
 )
{
	son_ho_status_et ho_sts = SON_HO_UNDEFINED;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_hash)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"p_hash is NULL");
		SON_UT_TRACE_EXIT();
		return ho_sts;
	}

	if (ANR_UTRAN_NEIGHBOR == p_hash->t)
	{
		ho_sts = anr_get_ho_status_of_utran_nr(&p_hash->u.utran_neighbor_info);    
	}
	else if (ANR_GERAN_NEIGHBOR == p_hash->t)
	{
		ho_sts = anr_get_ho_status_of_geran_nr(&p_hash->u.geran_neighbor_info);    
	}

	if (p_hash->t == ANR_CDMA_NEIGHBOR)
	{
		ho_sts = anr_get_ho_status_of_cdma_nr(&p_hash->u.cdma_neighbor_info);
	}

	SON_UT_TRACE_EXIT();
	return ho_sts;
} /* get_ho_status_for_inter_rat_nbrs */

/******************************************************************************
 * Function Name  : anr_validate_inter_rat_record_insertion
 * Inputs         : p_cell_ctxt - Ppointer to cell context
 *                  p_hash      - Pointer to NBR hash record
 *                  p_error_code- Pointer to error code
 * Outputs        : None.
 * Returns        : son_bool_et - validity status flag
 * Description    : This function is used to verify if the inter rat NBR is 
 *                  valid for adding in NRT and sendable to rrm
 *                  
 ******************************************************************************/
son_bool_et
anr_validate_inter_rat_record_insertion
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_inter_rat_hash_rec_t  *p_hash,
 son_error_et              *p_error_code
 )
{
	son_bool_et is_data_sendable_to_rrm = SON_FALSE;
	SON_LIST *p_list = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_hash)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
				"p_hash is NULL");
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	*p_error_code = SON_NO_ERROR;

	if (ANR_UTRAN_NEIGHBOR == p_hash->t)
	{
		anr_utran_freq_stats_node_t *p_node = SON_PNULL;

		p_list = anr_determine_utran_freq_stats_list(
				p_cell_ctxt, p_hash->u.utran_neighbor_info.pci);

		if (SON_PNULL != p_list)
		{
			p_node = anr_find_record_in_utran_freq_stats_list(
					p_list,
					p_hash->u.utran_neighbor_info.uarfcn);
		}

		is_data_sendable_to_rrm = is_utran_nbr_notifiable_to_rrm(
				&p_hash->u.utran_neighbor_info); 

		if (SON_TRUE == is_data_sendable_to_rrm && p_node == SON_PNULL &&
				((SON_PCI_FDD_PRESENT == p_hash->u.utran_neighbor_info.pci.bitmask && 
				  SON_RRM_MAX_UTRAN_FDD_FREQ_ALLOWED <= list_count(&p_cell_ctxt->utran_freq_fdd_stats_list)) ||
				 (SON_PCI_TDD_PRESENT == p_hash->u.utran_neighbor_info.pci.bitmask && 
				  SON_RRM_MAX_UTRAN_TDD_FREQ_ALLOWED <= list_count(&p_cell_ctxt->utran_freq_tdd_stats_list))))
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_INFO,
					"UTRAN Nbr Insertion is not possible as the"
					" maximum number of frequencies send to RRM is already reached");
			*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;       
		}
	}
	else if (ANR_GERAN_NEIGHBOR == p_hash->t)
	{
		anr_geran_freq_stats_node_t *p_node = SON_PNULL;
		anr_geran_freq_stats_data_t data;

		SON_MEMSET(&data, 0, sizeof(anr_geran_freq_stats_data_t));
		data.arfcn = p_hash->u.geran_neighbor_info.arfcn;
		data.band_ind = p_hash->u.geran_neighbor_info.band_ind;

		p_node = anr_find_record_in_geran_freq_stats_list(
				&p_cell_ctxt->geran_freq_stats_list, &data);

		is_data_sendable_to_rrm =  is_geran_nbr_notifiable_to_rrm(
				&p_hash->u.geran_neighbor_info);

		if (SON_TRUE == is_data_sendable_to_rrm && p_node == SON_NULL &&
				SON_RRM_MAX_GERAN_FREQ_ALLOWED <= list_count(&p_cell_ctxt->geran_freq_stats_list))
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_INFO,
					"GERAN Insertion is not possible as the   "
					" as Max limit %u reached", list_count(&p_cell_ctxt->geran_freq_stats_list));
			*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;
		}
	}

	else if (ANR_CDMA_NEIGHBOR == p_hash->t)
	{
		if (SON_CGI_1X_RTT_PRESENT & p_hash->u.cdma_neighbor_info.nbr_cgi.bitmask)
		{
			anr_cdma_xrtt_band_stats_node_t *p_band_node = SON_PNULL;
			anr_cdma_xrtt_freq_stats_node_t *p_freq_node = SON_PNULL;

			/* Finding the band in cell context*/
			p_band_node = anr_find_record_in_cdma_xrtt_band_stats_list(
					&p_cell_ctxt->cdma_xrtt_band_stats_list,
					p_hash->u.cdma_neighbor_info.band_class);

			if (p_band_node != SON_PNULL)
			{
				/* Finding the freq in band stats list */
				p_freq_node = anr_find_record_in_cdma_xrtt_freq_stats_list(
						&p_band_node->cdma_band_stats.freq_list,
						p_hash->u.cdma_neighbor_info.arfcn);
			}

			is_data_sendable_to_rrm =  is_cdma_nbr_notifiable_to_rrm(
					&p_hash->u.cdma_neighbor_info);

			if ((SON_TRUE == is_data_sendable_to_rrm) &&
					(p_band_node == SON_PNULL) &&
					(SON_RRM_MAX_CDMA_BAND_ALLOWED <=
					 (list_count(&p_cell_ctxt->cdma_xrtt_band_stats_list))))
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_INFO,
						"CDMA XRTT NR cannot be inserted into NRT as "
						"the maximum limit %u for band class sendable to RRM "
						"is already reached", list_count(&p_cell_ctxt->cdma_xrtt_band_stats_list));

				*p_error_code = SON_ERR_ANR_MAX_ALLOWED_BANDS_NBRS_TO_RRM_REACHED;
			}
			else if ((SON_TRUE == is_data_sendable_to_rrm) &&
					((p_band_node != SON_PNULL) && (p_freq_node == SON_PNULL)) &&
					(SON_RRM_MAX_CDMA_FREQ_ALLOWED <=
					 (list_count(&p_band_node->cdma_band_stats.freq_list))))
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_INFO,
						"CDMA XRTT NR cannot be inserted into NRT as "
						"the maximum limit %u for frequency sendable to RRM "
						"is already reached", list_count(&p_band_node->cdma_band_stats.freq_list));

				*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;
			}
			else if ((SON_TRUE == is_data_sendable_to_rrm) &&
					((p_band_node != SON_PNULL) && (p_freq_node != SON_PNULL)) &&
					(SON_MAX_NO_CDMA_XRTT_CELLS_PER_EARFCN <=
					 (anr_get_no_of_xrtt_nbrs_per_frequency(p_freq_node))))
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_INFO,
						"CDMA XRTT NR cannot be inserted into NRT as "
						"the maximum limit %u for neighbors per frequency "
						"sendable to RRM is already reached",
                        anr_get_no_of_xrtt_nbrs_per_frequency(p_freq_node));

				*p_error_code = SON_ERR_ANR_MAX_ALLOWED_XRTT_NBRS_PER_ARFCN_TO_RRM_REACHED;
			}
			else if (SON_TRUE == is_data_sendable_to_rrm)
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_DETAILED,
						"CDMA XRTT NR is sendable to RRM and can be "
						"inserted into NRT"); 
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_INFO,
						"CDMA XRTT NR is NOT sendable to RRM, but can be "
						"inserted into NRT");
			}
		}
		else if (p_hash->u.cdma_neighbor_info.nbr_cgi.bitmask & SON_CGI_HRPD_PRESENT)
		{
			anr_cdma_hrpd_band_stats_node_t *p_band_node = SON_PNULL;
			anr_cdma_hrpd_freq_stats_node_t *p_freq_node = SON_PNULL;

			/* Finding the band in cell context*/
			p_band_node = anr_find_record_in_cdma_hrpd_band_stats_list(
					&p_cell_ctxt->cdma_hrpd_band_stats_list,
					p_hash->u.cdma_neighbor_info.band_class);

			if (p_band_node != SON_PNULL) 
			{
				/* Finding the freq in band stats list */
				p_freq_node = anr_find_record_in_cdma_hrpd_freq_stats_list(
						&p_band_node->cdma_band_stats.freq_list,
						p_hash->u.cdma_neighbor_info.arfcn);
			}

			is_data_sendable_to_rrm =  is_cdma_nbr_notifiable_to_rrm(
					&p_hash->u.cdma_neighbor_info);

			if ((SON_TRUE == is_data_sendable_to_rrm) &&
					(p_band_node == SON_PNULL) &&
					(SON_RRM_MAX_CDMA_BAND_ALLOWED <=
					 (list_count(&p_cell_ctxt->cdma_hrpd_band_stats_list))))
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_INFO,
						"CDMA HRPD NR cannot be inserted into NRT as "
						"as maximum limit %u for band class sendable to RRM "
						"is already reached", list_count(&p_cell_ctxt->cdma_hrpd_band_stats_list));

				*p_error_code = SON_ERR_ANR_MAX_ALLOWED_BANDS_NBRS_TO_RRM_REACHED;
			}
			else if ((SON_TRUE == is_data_sendable_to_rrm) &&
					((p_band_node != SON_PNULL) && (p_freq_node == SON_PNULL)) &&
					(SON_RRM_MAX_CDMA_FREQ_ALLOWED <=
					 (list_count(&p_band_node->cdma_band_stats.freq_list))))
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_INFO,
						"CDMA HRPD NR cannot be inserted into NRT as "
						"the maximum limit %u for frequency sendable to RRM "
						"is already reached",
                        list_count(&p_band_node->cdma_band_stats.freq_list));

				*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;
			}
			else if ((SON_TRUE == is_data_sendable_to_rrm) &&
					((p_band_node != SON_PNULL) && (p_freq_node != SON_PNULL)) &&
					(SON_MAX_NO_CDMA_HRPD_CELLS_PER_EARFCN <=
					 (anr_get_no_of_hrpd_nbrs_per_frequency(p_freq_node))))
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_INFO,
						"CDMA HRPD NR cannot be inserted into NRT as "
						"the maximum limit %u for neighbors per frequency "
                        "sendable to RRM is already reached",
                        anr_get_no_of_hrpd_nbrs_per_frequency(p_freq_node));

				*p_error_code = SON_ERR_ANR_MAX_ALLOWED_HRPD_NBRS_PER_ARFCN_TO_RRM_REACHED;
			}
			else if (SON_TRUE == is_data_sendable_to_rrm)
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_DETAILED,
						"CDMA HRPD NR is sendable to RRM and can be "
						"inserted into NRT");
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_INFO,
						"CDMA HRPD NR is NOT sendable to RRM, but can be "
						"inserted into NRT");
			}
		}
	}

	SON_UT_TRACE_EXIT();
	return is_data_sendable_to_rrm;
} /* anr_validate_inter_rat_record_insertion */

/******************************************************************************
 * Function Name  : anr_determine_utran_changes_in_nrt_when_old_nr_not_send
 * Inputs         : p_cell_ctxt - Pointer to cell context
 *                  p_old_hash      - Pointer to old NBR hash record
 *                  p_new_hash      - Pointer to new NBR hash record
 *                  old_ho_status   - HO status for old NBR
 *                  new_ho_status   - HO status for new NBR
 *                  p_is_old_nr_send- flag pointer to check if old NBR is sendable
 *                                    to RRM
 *                  p_is_rrm_notif_reqd_for_new_nr- Status flag to check if new
 *                                    NBR if to be notified to RRM
 *                  p_error_code- Pointer to error code
 * Outputs        : None.
 *
 * Returns        : validity status flag
 * Description    : This function is used to verify the new NBRs status to be notified
 *                  to RRM when old NBR was not send to RRM
 ******************************************************************************/
son_void_t
anr_determine_utran_changes_in_nrt_when_old_nr_not_send
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_inter_rat_hash_rec_t  *p_old_hash,
 anr_inter_rat_hash_rec_t  *p_new_hash,
        /*SPR 17777 +-*/
 son_bool_et               *p_is_old_nr_send,
 son_bool_et               *p_is_rrm_notif_reqd_for_new_nr,
 son_error_et              *p_error_code
 )
{
	SON_UT_TRACE_ENTER();

	if (p_old_hash == SON_PNULL || p_new_hash == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"p_old_hash or p_new_hash is NULL");
	}
	else
	{
		son_bool_et is_roll_back_allowed = SON_FALSE;
		if (SON_FALSE == *p_is_rrm_notif_reqd_for_new_nr)
		{
			*p_error_code = SON_NO_ERROR;

			*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

			*p_is_old_nr_send = SON_FALSE;
		}
		else
		{
			anr_utran_freq_stats_node_t *p_list_node = SON_PNULL;
			SON_LIST *p_list = SON_PNULL;

			p_list = anr_determine_utran_freq_stats_list(
					p_cell_ctxt, p_new_hash->u.utran_neighbor_info.pci);

			if (SON_PNULL != p_list)
			{
				p_list_node = anr_find_record_in_utran_freq_stats_list(
						p_list,
						p_new_hash->u.utran_neighbor_info.uarfcn);
			}

			if (SON_PNULL == p_list_node &&
					((SON_PCI_FDD_PRESENT == p_new_hash->u.utran_neighbor_info.pci.bitmask &&
					  SON_RRM_MAX_UTRAN_FDD_FREQ_ALLOWED <= list_count(&p_cell_ctxt->utran_freq_fdd_stats_list)) ||
					 (SON_PCI_TDD_PRESENT == p_new_hash->u.utran_neighbor_info.pci.bitmask &&
					  SON_RRM_MAX_UTRAN_TDD_FREQ_ALLOWED <= list_count(&p_cell_ctxt->utran_freq_tdd_stats_list))))
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_DETAILED,
						"New UARFCN can not be "
						"configured as there are already maximum configured UARFCNs");

				*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;

				*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

				*p_is_old_nr_send = SON_FALSE;

				SON_UT_TRACE_EXIT();
				return;
			}

			if (SON_PNULL == p_list_node && SON_PNULL != p_list)
			{
				p_list_node =  anr_insert_record_in_utran_freq_stats_list(
						p_list,
						p_new_hash->u.utran_neighbor_info.uarfcn);

				if (SON_PNULL == p_list_node)
				{
					is_roll_back_allowed = SON_TRUE;
					*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;
				}
			}
			else if (SON_FALSE == increment_utran_nbr_count_for_rrm(p_cell_ctxt, p_list_node))
			{
				is_roll_back_allowed = SON_TRUE;
				*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;
			}

			if (SON_FALSE == is_roll_back_allowed)
			{
				*p_error_code = SON_NO_ERROR;

				*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

				*p_is_old_nr_send = SON_FALSE;
			}
			else
			{
				*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

				*p_is_old_nr_send = SON_FALSE;
			}
		}
	}

	SON_UT_TRACE_EXIT();
	return;
} /* anr_determine_utran_changes_in_nrt_when_old_nr_not_send */

/******************************************************************************
 * Function Name  : anr_determine_utran_changes_in_nrt_when_old_nr_send
 * Inputs         : p_cell_ctxt - Pointer to cell context
 *                  p_old_hash      - Pointer to old NBR hash record
 *                  p_new_hash      - Pointer to new NBR hash record
 *                  old_ho_status   - HO status for old NBR
 *                  new_ho_status   - HO status for new NBR
 *                  p_is_old_nr_send- flag pointer to check if old NBR is sendable
 *                                    to RRM
 *                  p_is_rrm_notif_reqd_for_new_nr- Status flag to check if new
 *                                    NBR if to be notified to RRM
 *                  p_error_code- Pointer to error code
 * Outputs        : None.
 *
 * Returns        : validity status flag
 * Description    : This function is used to verify the new NBRs status to be notified
 *                  to RRM when old NBR was send to RRM
 ******************************************************************************/
son_void_t
anr_determine_utran_changes_in_nrt_when_old_nr_send
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_inter_rat_hash_rec_t  *p_old_hash,
 anr_inter_rat_hash_rec_t  *p_new_hash,
 son_ho_status_et          old_ho_status,
 son_ho_status_et          new_ho_status,
 son_bool_et               *p_is_old_nr_send,
 son_bool_et               *p_is_rrm_notif_reqd_for_new_nr,
 son_error_et              *p_error_code
 )
{
	anr_utran_freq_stats_node_t *p_list_node_old = SON_PNULL;
	anr_utran_freq_stats_node_t *p_list_node_new = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (p_old_hash == SON_PNULL || p_new_hash == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
				"p_old_hash or p_new_hash is NULL");
	}
	else
	{
		SON_LIST *p_list = SON_PNULL;

		p_list = anr_determine_utran_freq_stats_list(
				p_cell_ctxt, p_old_hash->u.utran_neighbor_info.pci);

		if (SON_PNULL != p_list)
		{
			p_list_node_old = anr_find_record_in_utran_freq_stats_list(
					p_list,
					p_old_hash->u.utran_neighbor_info.uarfcn);
		}

		if (SON_PNULL ==  p_list_node_old || 0 == p_list_node_old->utran_stats.num_of_utran_nbrs_to_rrm)
		{
			SON_LOG(
					anr_get_log_mode(), p_son_anr_facility_name,
					SON_DETAILED,
					"Either node for old nbrs uarfcn is not found or the"
					"neighbors count for RRM is 0");

			*p_error_code = SON_ERR_ANR_MODIFICATION_NOT_ALLOWED;        

			*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

			*p_is_old_nr_send   = SON_FALSE;

			SON_UT_TRACE_EXIT();
			return;
		}

		if (SON_FALSE == *p_is_rrm_notif_reqd_for_new_nr)
		{
			*p_error_code = SON_NO_ERROR; 

			*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

			*p_is_old_nr_send = SON_TRUE;
		} 
		else
		{
			if (p_old_hash->u.utran_neighbor_info.uarfcn == p_new_hash->u.utran_neighbor_info.uarfcn)
			{
				*p_error_code = SON_NO_ERROR;

				*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

				if (old_ho_status != new_ho_status)
				{
					*p_is_old_nr_send = SON_TRUE;
				}
				else
				{
					*p_is_old_nr_send = SON_FALSE;
				}
			}
			else
			{
				p_list = anr_determine_utran_freq_stats_list(
						p_cell_ctxt, p_new_hash->u.utran_neighbor_info.pci);

				if (SON_PNULL != p_list)
				{
					p_list_node_new = anr_find_record_in_utran_freq_stats_list(
							p_list,
							p_new_hash->u.utran_neighbor_info.uarfcn);
				}

				if (SON_PNULL != p_list_node_new)
				{
					if (SON_TRUE == increment_utran_nbr_count_for_rrm(p_cell_ctxt, p_list_node_new))
					{
						decrement_utran_nbr_count_for_rrm(p_cell_ctxt, p_list_node_old, p_list);

						*p_error_code = SON_NO_ERROR;

						*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

						*p_is_old_nr_send = SON_TRUE;
					}
					else
					{
						*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;

						*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

						*p_is_old_nr_send = SON_FALSE;
					}
				}
				else
				{
					/*Added a case where we can have a 16 freq out of which 1 freq has only 1 nbr in it.
					 * If the freq of that neighbor is changes , it should be told to RRM */

					son_u32 count = 0;

					p_list = anr_determine_utran_freq_stats_list(
							p_cell_ctxt, p_old_hash->u.utran_neighbor_info.pci);

                    /* SPR 20007:CID 110679 Start*/
                    if(p_list!=SON_PNULL)
                    {     
					count  = list_count(p_list);
                    }     
                    else  
                    {     
                        return ; 
                    }     
                    /*SPR 20007 : CID 110679 End*/

					if ((SON_PCI_FDD_PRESENT == p_old_hash->u.utran_neighbor_info.pci.bitmask && 
								SON_RRM_MAX_UTRAN_FDD_FREQ_ALLOWED == count) ||
							(SON_PCI_TDD_PRESENT == p_old_hash->u.utran_neighbor_info.pci.bitmask &&
							 SON_RRM_MAX_UTRAN_TDD_FREQ_ALLOWED == count))
					{
						if (1  ==  p_list_node_old->utran_stats.num_of_utran_nbrs_to_rrm)
						{
							/*decreament the old*/ 
							decrement_utran_nbr_count_for_rrm(p_cell_ctxt, p_list_node_old, p_list);

							/*Do the insertion for the new.Assuming that the insetion 
							 * will take place and memory will be available */
							/* Coverity Fix: 23478 */
							p_list_node_new = anr_insert_record_in_utran_freq_stats_list(
									p_list,
									p_new_hash->u.utran_neighbor_info.uarfcn);

							if (SON_PNULL != p_list_node_new)
							{
								*p_error_code = SON_NO_ERROR;

								*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

								*p_is_old_nr_send = SON_TRUE;
							}
						}
						else
						{
							SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
									SON_INFO,
									"New UARFCN %u can not be "
									"configured as there are already maximum configured UARFCNs",
                                    p_list_node_old->utran_stats.uarfcn);

							*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;

							*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

							*p_is_old_nr_send = SON_FALSE;
						}
					}
					else if ((SON_PCI_FDD_PRESENT == p_old_hash->u.utran_neighbor_info.pci.bitmask && 
								SON_RRM_MAX_UTRAN_FDD_FREQ_ALLOWED > count) ||
							(SON_PCI_TDD_PRESENT == p_old_hash->u.utran_neighbor_info.pci.bitmask &&
							 SON_RRM_MAX_UTRAN_TDD_FREQ_ALLOWED > count))
					{
						p_list_node_new = anr_insert_record_in_utran_freq_stats_list(
								p_list,
								p_new_hash->u.utran_neighbor_info.uarfcn);

						if (SON_PNULL != p_list_node_new)
						{
							decrement_utran_nbr_count_for_rrm(p_cell_ctxt, p_list_node_old, p_list);

							*p_error_code = SON_NO_ERROR;

							*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

							*p_is_old_nr_send = SON_TRUE;
						}
						else
						{
							*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;

							*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

							*p_is_old_nr_send = SON_FALSE;
						}
					}
					else
					{
						SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
								SON_INFO,
								"New UARFCN can not be "
								"configured as there are already maximum %u configured UARFCNs", count);

						*p_error_code = SON_ERR_ANR_MODIFICATION_NOT_ALLOWED;

						*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

						*p_is_old_nr_send = SON_FALSE;
					} 
				}
			}
		}
	}

	SON_UT_TRACE_EXIT();
} /* anr_determine_utran_changes_in_nrt_when_old_nr_send */

/******************************************************************************
 * Function Name  : anr_determine_geran_changes_in_nrt_when_old_nr_not_send
 * Inputs         : p_cell_ctxt - Pointer to cell context
 *                  p_old_hash      - Pointer to old NBR hash record
 *                  p_new_hash      - Pointer to new NBR hash record
 *                  old_ho_status   - HO status for old NBR
 *                  new_ho_status   - HO status for new NBR
 *                  p_is_old_nr_send- flag pointer to check if old NBR is sendable
 *                                    to RRM
 *                  p_is_rrm_notif_reqd_for_new_nr- Status flag to check if new
 *                                    NBR if to be notified to RRM
 *                  p_error_code- Pointer to error code
 * Outputs        : None.
 *
 * Returns        : validity status flag
 * Description    : This function is used to verify the new NBRs status to be notified
 *                  to RRM when old NBR was not send to RRM
 ******************************************************************************/
son_void_t
anr_determine_geran_changes_in_nrt_when_old_nr_not_send
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_inter_rat_hash_rec_t  *p_old_hash,
 anr_inter_rat_hash_rec_t  *p_new_hash,
        /*SPR 17777 +-*/
 son_bool_et               *p_is_old_nr_send,
 son_bool_et               *p_is_rrm_notif_reqd_for_new_nr,
 son_error_et              *p_error_code
 )
{
	SON_UT_TRACE_ENTER();

	if (p_old_hash == SON_PNULL || p_new_hash == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
				"p_old_hash or p_new_hash is NULL");
	}
	else
	{
		son_bool_et is_roll_back_allowed = SON_FALSE;
		anr_geran_freq_stats_node_t *p_list_node = SON_PNULL;
		anr_geran_freq_stats_data_t data;

		if (SON_FALSE == *p_is_rrm_notif_reqd_for_new_nr)
		{
			*p_error_code = SON_NO_ERROR;

			*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

			*p_is_old_nr_send = SON_FALSE;
		}
		else
		{
			SON_MEMSET(&data, 0, sizeof(anr_geran_freq_stats_data_t));
			data.arfcn = p_new_hash->u.geran_neighbor_info.arfcn;
			data.band_ind = p_new_hash->u.geran_neighbor_info.band_ind; 

			p_list_node = anr_find_record_in_geran_freq_stats_list(
					&p_cell_ctxt->geran_freq_stats_list, &data);

			if (SON_PNULL == p_list_node &&
					SON_RRM_MAX_GERAN_FREQ_ALLOWED <= list_count(&p_cell_ctxt->geran_freq_stats_list))
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_INFO,
						"New ARFCN %u Band %u can not be "
						"configured as there are %d already configured ARFCNs",
                        data.arfcn, data.band_ind,
						list_count(&p_cell_ctxt->geran_freq_stats_list));

				*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;

				*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

				*p_is_old_nr_send = SON_FALSE;

				SON_UT_TRACE_EXIT();
				return;
			}

			if (SON_PNULL == p_list_node)
			{
				p_list_node =  anr_insert_record_in_geran_freq_stats_list(
						&p_cell_ctxt->geran_freq_stats_list,
						p_new_hash->u.geran_neighbor_info.arfcn,
						p_new_hash->u.geran_neighbor_info.band_ind);

				if (SON_PNULL == p_list_node)
				{
					is_roll_back_allowed = SON_TRUE;
					*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;
				}
			}
			else if (SON_FALSE == increment_geran_nbr_count_for_rrm(p_cell_ctxt, p_list_node))
			{
				is_roll_back_allowed = SON_TRUE;
				*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;
			}

			if (SON_FALSE == is_roll_back_allowed)
			{
				*p_error_code = SON_NO_ERROR;

				*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

				*p_is_old_nr_send = SON_FALSE;
			}
			else
			{
				*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

				*p_is_old_nr_send = SON_FALSE;
			}
		}
	}

	SON_UT_TRACE_EXIT();
	return;
} /* anr_determine_geran_changes_in_nrt_when_old_nr_not_send */

/******************************************************************************
 * Function Name  : anr_determine_cdma_changes_in_nrt_when_old_nr_send
 * Inputs         : p_cell_ctxt     - Pointer to cell context
 *                  p_old_hash      - Pointer to old NBR hash record
 *                  p_new_hash      - Pointer to new NBR hash record
 *                  old_ho_status   - HO status for old NBR
 *                  new_ho_status   - HO status for new NBR
 *                  p_is_old_nr_send- Flag pointer to check if old NBR is
 *                                    sendable to RRM
 *                  p_is_rrm_notif_reqd_for_new_nr- Status flag to check if new
 *                                    NBR is notifiable to RRM
 *                  p_error_code    - Pointer to error code
 * Outputs        : None
 * Returns        : void
 * Description    : This function is used to verify the new CDMA NBRs status 
 *                  to be notified to RRM when old NBR was send to RRM
 ******************************************************************************/
son_void_t
anr_determine_cdma_changes_in_nrt_when_old_nr_send
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_inter_rat_hash_rec_t  *p_old_hash,
 anr_inter_rat_hash_rec_t  *p_new_hash,
 son_ho_status_et          old_ho_status,
 son_ho_status_et          new_ho_status,
 son_bool_et               *p_is_old_nr_send,
 son_bool_et               *p_is_rrm_notif_reqd_for_new_nr,
 son_error_et              *p_error_code
 )
{
	/* Coverity CID-55068 Fix Starts */
	anr_cdma_xrtt_band_stats_node_t *p_band_list_xrtt_node_old = SON_PNULL;
	anr_cdma_xrtt_freq_stats_node_t *p_freq_list_xrtt_node_old = SON_PNULL;
	anr_cdma_xrtt_band_stats_node_t *p_band_list_xrtt_node_new = SON_PNULL;
	anr_cdma_xrtt_freq_stats_node_t *p_freq_list_xrtt_node_new = SON_PNULL;

	anr_cdma_hrpd_band_stats_node_t *p_band_list_hrpd_node_old = SON_PNULL;
	anr_cdma_hrpd_freq_stats_node_t *p_freq_list_hrpd_node_old = SON_PNULL;
	anr_cdma_hrpd_band_stats_node_t *p_band_list_hrpd_node_new = SON_PNULL;
	anr_cdma_hrpd_freq_stats_node_t *p_freq_list_hrpd_node_new = SON_PNULL;

	SON_LIST_NODE   *p_node     = SON_PNULL;
	son_u32         band_count  = 0;
	son_u32         freq_count  = 0;
	son_u16         total_nbrs  = 0;
	son_u16         idx         = 0;


	SON_UT_TRACE_ENTER();

	if (p_old_hash == SON_PNULL || p_new_hash == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
				"p_old_hash or p_new_hash is NULL");
	}
	else
	{
		if (SON_CGI_1X_RTT_PRESENT & p_new_hash->u.cdma_neighbor_info.nbr_cgi.bitmask)
		{
			/* Band level search for old nbr */
			p_band_list_xrtt_node_old = anr_find_record_in_cdma_xrtt_band_stats_list(
					&p_cell_ctxt->cdma_xrtt_band_stats_list,
					p_old_hash->u.cdma_neighbor_info.band_class);

			if (p_band_list_xrtt_node_old != SON_PNULL)
			{
				/* Freq level search for old nbr */
				p_freq_list_xrtt_node_old = anr_find_record_in_cdma_xrtt_freq_stats_list(
						&p_band_list_xrtt_node_old->cdma_band_stats.freq_list,
						p_old_hash->u.cdma_neighbor_info.arfcn);
			}

			if ((SON_PNULL == p_band_list_xrtt_node_old) ||
					((SON_PNULL != p_band_list_xrtt_node_old) && 
					 (SON_PNULL == p_freq_list_xrtt_node_old)))
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_INFO,
                        "Node for old nbr arfcn %u band_class %u is not found",
                        p_old_hash->u.cdma_neighbor_info.arfcn,
                        p_old_hash->u.cdma_neighbor_info.band_class);

				*p_error_code = SON_ERR_ANR_MODIFICATION_NOT_ALLOWED;        
				*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;
				*p_is_old_nr_send   = SON_FALSE;

				SON_UT_TRACE_EXIT();
				return;
			}

			if (SON_FALSE == *p_is_rrm_notif_reqd_for_new_nr)
			{
				*p_error_code = SON_NO_ERROR; 

				*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

				*p_is_old_nr_send = SON_TRUE;
			}
			else
			{
				if ((p_old_hash->u.cdma_neighbor_info.arfcn ==
							p_new_hash->u.cdma_neighbor_info.arfcn) &&
						(p_old_hash->u.cdma_neighbor_info.band_class ==
						 p_new_hash->u.cdma_neighbor_info.band_class))
				{
					*p_error_code = SON_NO_ERROR;

					*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

					if (old_ho_status != new_ho_status)
					{
						*p_is_old_nr_send = SON_TRUE;
					}
					else
					{
						*p_is_old_nr_send = SON_FALSE;
					}
				}
				else
				{
					/* Finding if band class already exists */
					p_band_list_xrtt_node_new = anr_find_record_in_cdma_xrtt_band_stats_list(
							&p_cell_ctxt->cdma_xrtt_band_stats_list,
							p_new_hash->u.cdma_neighbor_info.band_class);

					if (SON_PNULL != p_band_list_xrtt_node_new)
					{
						/* Finding if frequency already exists */
						p_freq_list_xrtt_node_new = anr_find_record_in_cdma_xrtt_freq_stats_list(
								&p_band_list_xrtt_node_new->cdma_band_stats.freq_list,
								p_new_hash->u.cdma_neighbor_info.arfcn);

						if (SON_PNULL != p_freq_list_xrtt_node_new)
						{
							/* Incrementing new nbr count */
							if (SON_TRUE == increment_cdma_xrtt_nbr_count_for_rrm(
										&p_new_hash->u.cdma_neighbor_info,
										p_cell_ctxt))
							{
								/* Decrementing old nbr count */
								decrement_cdma_xrtt_nbr_count_for_rrm(
										&p_old_hash->u.cdma_neighbor_info,
										p_cell_ctxt);

								*p_error_code = SON_NO_ERROR;

								*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

								*p_is_old_nr_send = SON_TRUE;
							}
							else 
							{
								*p_error_code = SON_ERR_ANR_MAX_ALLOWED_XRTT_NBRS_PER_ARFCN_TO_RRM_REACHED;

								*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

								*p_is_old_nr_send = SON_FALSE;
							}
						}
						else
						{
							/*Added a case where we can have a 16 freq out of which 1 freq has only 1 nbr in it.
							 * If the freq of that neighbor is changes , it should be told to RRM */

							freq_count = list_count(&p_band_list_xrtt_node_new->cdma_band_stats.freq_list);

							if (SON_RRM_MAX_CDMA_FREQ_ALLOWED == freq_count)
							{
								total_nbrs = 
									anr_get_no_of_xrtt_nbrs_per_frequency(p_freq_list_xrtt_node_old);

								if (1 == total_nbrs)
								{
									/* Decrement the old nbr */ 
									decrement_cdma_xrtt_nbr_count_for_rrm(
											&p_old_hash->u.cdma_neighbor_info,
											p_cell_ctxt);

									/* Do the insertion for the new. Assuming that the insertion 
									 * will take place and memory will be available */
									anr_insert_record_in_cdma_xrtt_band_stats_list(
											p_cell_ctxt,
											&p_new_hash->u.cdma_neighbor_info);

									*p_error_code = SON_NO_ERROR;

									*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

									*p_is_old_nr_send = SON_TRUE;
								}
								else
								{
									/* Bug Fix: 788 */
									SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
											SON_WARNING,
											"New CDMA 1XRTT ARFCN %u can not be "
											"configured as there are [%d] already configured ARFCNs",
                                            p_new_hash->u.cdma_neighbor_info.arfcn, freq_count);

									*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;

									*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

									*p_is_old_nr_send = SON_FALSE;
								}
							}
							else if (SON_RRM_MAX_CDMA_FREQ_ALLOWED > freq_count)
							{
								/* Insert the new nbr */
								p_band_list_xrtt_node_new =
									anr_insert_record_in_cdma_xrtt_band_stats_list(
											p_cell_ctxt,
											&p_new_hash->u.cdma_neighbor_info);

								if (SON_PNULL != p_band_list_xrtt_node_new)
								{
									/* Decrement the old nbr */
									decrement_cdma_xrtt_nbr_count_for_rrm(
											&p_old_hash->u.cdma_neighbor_info,
											p_cell_ctxt);

									*p_error_code = SON_NO_ERROR;

									*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

									*p_is_old_nr_send = SON_TRUE;
								}
								else
								{
									*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;

									*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

									*p_is_old_nr_send = SON_FALSE;
								}
							}
							else
							{
								/* Bug Fix: 788 */
								SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
										SON_INFO,
										"New CDMA 1XRTT ARFCN %u can not be "
										"configured as there are [%d] already configured ARFCNs",
                                        p_new_hash->u.cdma_neighbor_info.arfcn,
										freq_count);

								*p_error_code = SON_ERR_ANR_MODIFICATION_NOT_ALLOWED;

								*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

								*p_is_old_nr_send = SON_FALSE;
							} 
						}
					}
					else
					{
						/* Getting the count of number of bands already configured on RRM */
						band_count = list_count(&p_cell_ctxt->cdma_xrtt_band_stats_list);

						if (SON_RRM_MAX_CDMA_BAND_ALLOWED == band_count)
						{
							freq_count = list_count(&p_band_list_xrtt_node_old->cdma_band_stats.freq_list);

							p_node = get_first_list_node(
									&p_band_list_xrtt_node_old->cdma_band_stats.freq_list);

							for (idx = 0; idx < freq_count; idx++)
							{
								if (SON_PNULL != p_node)
								{
									p_freq_list_xrtt_node_new = YMEMBEROF(anr_cdma_xrtt_freq_stats_node_t,
											anchor, p_node);

									total_nbrs += anr_get_no_of_xrtt_nbrs_per_frequency(
											p_freq_list_xrtt_node_new);

									p_node = get_next_list_node(p_node); 
								}
							}

							if (1 == total_nbrs)
							{
								/* Decrement the old nbr */
								decrement_cdma_xrtt_nbr_count_for_rrm(
										&p_old_hash->u.cdma_neighbor_info,
										p_cell_ctxt);

								/* Do the insertion for the new. Assuming that the insertion
								 * will take place and memory will be available */
								anr_insert_record_in_cdma_xrtt_band_stats_list(
										p_cell_ctxt,
										&p_new_hash->u.cdma_neighbor_info);

								*p_error_code = SON_NO_ERROR;

								*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

								*p_is_old_nr_send = SON_TRUE;
							}
							else
							{
								/* Bug Fix: 788 */
								SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
										SON_ERROR,
										"New BAND CLASS can not be "
										"configured as there are [%d] already configured BAND CLASS",
										band_count);

								*p_error_code = SON_ERR_ANR_MAX_ALLOWED_BANDS_NBRS_TO_RRM_REACHED;

								*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

								*p_is_old_nr_send = SON_FALSE;
							}
						}
						else if (SON_RRM_MAX_CDMA_BAND_ALLOWED > band_count)
						{
							/* Insert the new nbr band class */
							p_band_list_xrtt_node_new =
								anr_insert_record_in_cdma_xrtt_band_stats_list(
										p_cell_ctxt,
										&p_new_hash->u.cdma_neighbor_info);

							if (SON_PNULL != p_band_list_xrtt_node_new)
							{
								/* Decrement the old nbr */
								decrement_cdma_xrtt_nbr_count_for_rrm(
										&p_old_hash->u.cdma_neighbor_info,
										p_cell_ctxt);

								*p_error_code = SON_NO_ERROR;

								*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

								*p_is_old_nr_send = SON_TRUE;
							}
							else
							{
								*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;

								*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

								*p_is_old_nr_send = SON_FALSE;
							}
						}
						else
						{
							/* Bug Fix: 788 */
							SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
									SON_ERROR,
									"anr_determine_cdma_changes_in_nrt_when_old_nr_send: "
									"New BAND CLASS can not be "
									"configured as there are [%d] already configured BANDs",
									band_count);

							*p_error_code = SON_ERR_ANR_MODIFICATION_NOT_ALLOWED;

							*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

							*p_is_old_nr_send = SON_FALSE;
						}
					}
				}
			}
		}

		else if (SON_CGI_HRPD_PRESENT & p_new_hash->u.cdma_neighbor_info.nbr_cgi.bitmask)
		{
			/* Band level search for old nbr */
			p_band_list_hrpd_node_old = anr_find_record_in_cdma_hrpd_band_stats_list(
					&p_cell_ctxt->cdma_hrpd_band_stats_list,
					p_old_hash->u.cdma_neighbor_info.band_class);

			if (p_band_list_hrpd_node_old != SON_PNULL)
			{
				/* Freq level search for old nbr */
				p_freq_list_hrpd_node_old = anr_find_record_in_cdma_hrpd_freq_stats_list(
						&p_band_list_hrpd_node_old->cdma_band_stats.freq_list,
						p_old_hash->u.cdma_neighbor_info.arfcn);
			}

			if ((SON_PNULL == p_band_list_hrpd_node_old) ||
					((SON_PNULL != p_band_list_hrpd_node_old) &&
					 (SON_PNULL == p_freq_list_hrpd_node_old)))
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_INFO,
						"Node for old HRPD nbrs arfcn %u band_class %u is not found",
                        p_old_hash->u.cdma_neighbor_info.arfcn,
                        p_old_hash->u.cdma_neighbor_info.band_class);

				*p_error_code = SON_ERR_ANR_MODIFICATION_NOT_ALLOWED;        
				*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;
				*p_is_old_nr_send   = SON_FALSE;

				SON_UT_TRACE_EXIT();
				return;
			}

			if (SON_FALSE == *p_is_rrm_notif_reqd_for_new_nr)
			{
				*p_error_code = SON_NO_ERROR; 

				*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

				*p_is_old_nr_send = SON_TRUE;
			}
			else
			{
				if ((p_old_hash->u.cdma_neighbor_info.arfcn ==
							p_new_hash->u.cdma_neighbor_info.arfcn) &&
						(p_old_hash->u.cdma_neighbor_info.band_class ==
						 p_new_hash->u.cdma_neighbor_info.band_class))
				{
					*p_error_code = SON_NO_ERROR;

					*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

					if (old_ho_status != new_ho_status)
					{
						*p_is_old_nr_send = SON_TRUE;
					}
					else
					{
						*p_is_old_nr_send = SON_FALSE;
					}
				}
				else
				{
					/* Finding if band class already exists */
					p_band_list_hrpd_node_new = anr_find_record_in_cdma_hrpd_band_stats_list(
							&p_cell_ctxt->cdma_hrpd_band_stats_list,
							p_new_hash->u.cdma_neighbor_info.band_class);

					if (SON_PNULL != p_band_list_hrpd_node_new)
					{
						/* Finding if frequency already exists */
						p_freq_list_hrpd_node_new = anr_find_record_in_cdma_hrpd_freq_stats_list(
								&p_band_list_hrpd_node_new->cdma_band_stats.freq_list,
								p_new_hash->u.cdma_neighbor_info.arfcn);

						if (SON_PNULL != p_freq_list_hrpd_node_new)
						{
							/* Incrementing new nbr count*/
							if (SON_TRUE == increment_cdma_hrpd_nbr_count_for_rrm(
										&p_new_hash->u.cdma_neighbor_info,
										p_cell_ctxt))
							{
								/* Decrementing old nbr count*/
								decrement_cdma_hrpd_nbr_count_for_rrm(
										&p_old_hash->u.cdma_neighbor_info,
										p_cell_ctxt);

								*p_error_code = SON_NO_ERROR;

								*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

								*p_is_old_nr_send = SON_TRUE;
							}
							else 
							{
								*p_error_code = SON_ERR_ANR_MAX_ALLOWED_HRPD_NBRS_PER_ARFCN_TO_RRM_REACHED;

								*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

								*p_is_old_nr_send = SON_FALSE;
							}
						}
						else
						{
							/*Added a case where we can have a 16 freq out of which 1 freq has only 1 nbr in it.
							 * If the freq of that neighbor is changes , it should be told to RRM */

							freq_count  = list_count(&p_band_list_hrpd_node_new->cdma_band_stats.freq_list);

							if (SON_RRM_MAX_CDMA_FREQ_ALLOWED == freq_count)
							{
								total_nbrs = 
									anr_get_no_of_hrpd_nbrs_per_frequency(p_freq_list_hrpd_node_old);

								if (1 == total_nbrs)
								{
									/* Decrement the old nbr */ 
									decrement_cdma_hrpd_nbr_count_for_rrm(
											&p_old_hash->u.cdma_neighbor_info,
											p_cell_ctxt);

									/* Do the insertion for the new. Assuming that the insertion 
									 * will take place and memory will be available */
									anr_insert_record_in_cdma_hrpd_band_stats_list(
											p_cell_ctxt,
											&p_new_hash->u.cdma_neighbor_info);

									*p_error_code = SON_NO_ERROR;

									*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

									*p_is_old_nr_send = SON_TRUE;
								}
								else
								{
									/* Bug Fix: 788 */
									SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
											SON_ERROR,
											"anr_determine_cdma_changes_in_nrt_when_old_nr_send: "
											"New ARFCN can not be "
											"configured as there are [%d] already configured ARFCNs\n\n",
											freq_count);

									*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;

									*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

									*p_is_old_nr_send = SON_FALSE;
								}
							}
							else if (SON_RRM_MAX_CDMA_FREQ_ALLOWED > freq_count)
							{
								/* Insert the new nbr */
								p_band_list_hrpd_node_new =
									anr_insert_record_in_cdma_hrpd_band_stats_list(
											p_cell_ctxt,
											&p_new_hash->u.cdma_neighbor_info);

								if (SON_PNULL != p_band_list_hrpd_node_new)
								{
									/* Decrement the old nbr */
									decrement_cdma_hrpd_nbr_count_for_rrm(
											&p_old_hash->u.cdma_neighbor_info,
											p_cell_ctxt);

									*p_error_code = SON_NO_ERROR;

									*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

									*p_is_old_nr_send = SON_TRUE;
								}
								else
								{
									*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;

									*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

									*p_is_old_nr_send = SON_FALSE;
								}
							}
							else
							{
								/* Bug Fix: 788 */
								SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
										SON_ERROR,
										"anr_determine_cdma_changes_in_nrt_when_old_nr_send: "
										"New ARFCN can not be "
										"configured as there are [%d] already configured ARFCNs\n\n",
										total_nbrs);

								*p_error_code = SON_ERR_ANR_MODIFICATION_NOT_ALLOWED;

								*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

								*p_is_old_nr_send = SON_FALSE;
							} 
						}
					}
					else
					{
						/* Getting the count of number of bands already configured on RRM */
						band_count  = list_count(&p_cell_ctxt->cdma_hrpd_band_stats_list);

						if (SON_RRM_MAX_CDMA_BAND_ALLOWED == band_count)
						{
							freq_count  = list_count(&p_band_list_hrpd_node_old->cdma_band_stats.freq_list);

							p_node = get_first_list_node(
									&p_band_list_hrpd_node_old->cdma_band_stats.freq_list);

							for (idx = 0; idx < freq_count; idx++)
							{
								if (SON_PNULL != p_node)
								{
									p_freq_list_hrpd_node_new = YMEMBEROF(anr_cdma_hrpd_freq_stats_node_t,
											anchor, p_node);

									total_nbrs += anr_get_no_of_hrpd_nbrs_per_frequency(
											p_freq_list_hrpd_node_new);

									p_node = get_next_list_node(p_node);
								}
							}

							if (1 == total_nbrs)
							{
								/* Decrement the old nbr */
								decrement_cdma_hrpd_nbr_count_for_rrm(
										&p_old_hash->u.cdma_neighbor_info,
										p_cell_ctxt);

								/* Do the insertion for the new. Assuming that the insertion
								 * will take place and memory will be available */
								anr_insert_record_in_cdma_hrpd_band_stats_list(
										p_cell_ctxt,
										&p_new_hash->u.cdma_neighbor_info);

								*p_error_code = SON_NO_ERROR;

								*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

								*p_is_old_nr_send = SON_TRUE;
							}
							else
							{
								/* Bug Fix: 788 */
								SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
										SON_ERROR,
										"anr_determine_cdma_changes_in_nrt_when_old_nr_send: "
										"New BAND CLASS can not be "
										"configured as there are [%d] already configured BAND CLASS\n\n",
										band_count);

								*p_error_code = SON_ERR_ANR_MAX_ALLOWED_BANDS_NBRS_TO_RRM_REACHED;

								*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

								*p_is_old_nr_send = SON_FALSE;
							}
						}
						else if (SON_RRM_MAX_CDMA_BAND_ALLOWED > band_count)
						{
							/* Insert the new nbr band class */
							p_band_list_hrpd_node_new =
								anr_insert_record_in_cdma_hrpd_band_stats_list(
										p_cell_ctxt,
										&p_new_hash->u.cdma_neighbor_info);

							if (SON_PNULL != p_band_list_hrpd_node_new)
							{
								/* Decrement the old nbr */
								decrement_cdma_hrpd_nbr_count_for_rrm(
										&p_old_hash->u.cdma_neighbor_info,
										p_cell_ctxt);

								*p_error_code = SON_NO_ERROR;

								*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

								*p_is_old_nr_send = SON_TRUE;
							}
							else
							{
								*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;

								*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

								*p_is_old_nr_send = SON_FALSE;
							}
						}
						else
						{
							/* Bug Fix: 788 */
							SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
									SON_ERROR,
									"anr_determine_cdma_changes_in_nrt_when_old_nr_send: "
									"New BAND CLASS can not be "
									"configured as there are [%d] already configured BANDs\n\n",
									band_count);

							*p_error_code = SON_ERR_ANR_MODIFICATION_NOT_ALLOWED;

							*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

							*p_is_old_nr_send = SON_FALSE;
						}
					}
				}
			}
		}
	}

	SON_UT_TRACE_EXIT();
	return;
	/* Coverity CID-55068 Fix Ends */
} /* anr_determine_cdma_changes_in_nrt_when_old_nr_send */

/******************************************************************************
 * Function Name  : anr_determine_geran_changes_in_nrt_when_old_nr_send
 * Inputs         : p_cell_ctxt - Pointer to cell context
 *                  p_old_hash      - Pointer to old NBR hash record
 *                  p_new_hash      - Pointer to new NBR hash record
 *                  old_ho_status   - HO status for old NBR
 *                  new_ho_status   - HO status for new NBR
 *                  p_is_old_nr_send- flag pointer to check if old NBR is sendable
 *                                    to RRM
 *                  p_is_rrm_notif_reqd_for_new_nr- Status flag to check if new
 *                                    NBR if to be notified to RRM
 *                  p_error_code- Pointer to error code
 * Outputs        : None.
 *
 * Returns        : validity status flag
 * Description    : This function is used to verify the new NBRs status to be notified
 *                  to RRM when old NBR was send to RRM
 ******************************************************************************/
son_void_t
anr_determine_geran_changes_in_nrt_when_old_nr_send
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_inter_rat_hash_rec_t  *p_old_hash,
 anr_inter_rat_hash_rec_t  *p_new_hash,
 son_ho_status_et          old_ho_status,
 son_ho_status_et          new_ho_status,
 son_bool_et               *p_is_old_nr_send,
 son_bool_et               *p_is_rrm_notif_reqd_for_new_nr,
 son_error_et              *p_error_code
 )
{
	anr_geran_freq_stats_node_t *p_list_node_old = SON_PNULL;
	anr_geran_freq_stats_node_t *p_list_node_new = SON_PNULL;
	anr_geran_freq_stats_data_t  old_data;
	anr_geran_freq_stats_data_t  new_data;


	SON_UT_TRACE_ENTER();

	if (p_old_hash == SON_PNULL || p_new_hash == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
				"p_old_hash or p_new_hash is NULL");
	}
	else
	{
		SON_MEMSET(&old_data, 0, sizeof(anr_geran_freq_stats_data_t));
		old_data.arfcn = p_old_hash->u.geran_neighbor_info.arfcn;
		old_data.band_ind = p_old_hash->u.geran_neighbor_info.band_ind;

		SON_MEMSET(&new_data, 0, sizeof(anr_geran_freq_stats_data_t));
		new_data.arfcn = p_new_hash->u.geran_neighbor_info.arfcn;
		new_data.band_ind = p_new_hash->u.geran_neighbor_info.band_ind;

		p_list_node_old = anr_find_record_in_geran_freq_stats_list(
				&p_cell_ctxt->geran_freq_stats_list, &old_data);

		if (SON_PNULL ==  p_list_node_old || 0 == p_list_node_old->geran_stats.num_of_geran_nbrs_to_rrm)
		{
			SON_LOG(
					anr_get_log_mode(), p_son_anr_facility_name,
					SON_DETAILED,
					"Either node for old nbrs arfcn/band_ind is not found or the"
					"neighbors count for RRM is 0\n\n");

			*p_error_code = SON_ERR_ANR_MODIFICATION_NOT_ALLOWED;        

			*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

			*p_is_old_nr_send   = SON_FALSE;

			SON_UT_TRACE_EXIT();
			return;
		}

		if (SON_FALSE == *p_is_rrm_notif_reqd_for_new_nr)
		{
			*p_error_code = SON_NO_ERROR; 

			*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

			*p_is_old_nr_send = SON_TRUE;
		} 
		else
		{
			if ((p_old_hash->u.geran_neighbor_info.arfcn == p_new_hash->u.geran_neighbor_info.arfcn) &&
					(p_old_hash->u.geran_neighbor_info.band_ind == p_new_hash->u.geran_neighbor_info.band_ind))
			{
				*p_error_code = SON_NO_ERROR;

				*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

				if (old_ho_status != new_ho_status)
				{
					*p_is_old_nr_send = SON_TRUE;
				}
				else
				{
					*p_is_old_nr_send = SON_FALSE;
				}
			}
			else
			{
				p_list_node_new = anr_find_record_in_geran_freq_stats_list(
						&p_cell_ctxt->geran_freq_stats_list, &new_data);

				if (SON_PNULL != p_list_node_new)
				{
					if (SON_TRUE == increment_geran_nbr_count_for_rrm(p_cell_ctxt, p_list_node_new))
					{
						decrement_geran_nbr_count_for_rrm(p_cell_ctxt, p_list_node_old );

						*p_error_code = SON_NO_ERROR;

						*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

						*p_is_old_nr_send = SON_TRUE;
					}
					else
					{
						*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;

						*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

						*p_is_old_nr_send = SON_FALSE;
					}
				}
				else
				{ /*IMP*/
					/*Added a case where we can have a 16 freq out of which 1 freq has only 1 nbr in it.
					 * If the freq of that neighbor is changes , it should be told to RRM */

					son_u32 count = 0;
					count  = list_count(&p_cell_ctxt->geran_freq_stats_list);

					if (SON_RRM_MAX_GERAN_FREQ_ALLOWED == count)
					{
						if (1  ==  p_list_node_old->geran_stats.num_of_geran_nbrs_to_rrm)
						{
							/*decreament the old*/ 
							decrement_geran_nbr_count_for_rrm(p_cell_ctxt, p_list_node_old );

							/*Do the insertion for the new.Assuming that the insetion 
							 * will take place and memory will be available */
							/* Coverity Fix: 23477 */
							p_list_node_new = anr_insert_record_in_geran_freq_stats_list(
									&p_cell_ctxt->geran_freq_stats_list,
									p_new_hash->u.geran_neighbor_info.arfcn,
									p_new_hash->u.geran_neighbor_info.band_ind);

							if (SON_PNULL == p_list_node_new)
							{
								*p_error_code = SON_NO_ERROR;

								*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

								*p_is_old_nr_send = SON_TRUE;
							}
						}
						else
						{
							SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
									SON_DETAILED,
									"New ARFCN can not be "
									"configured as there are [%d] already configured ARFCNs\n\n",
									list_count(&p_cell_ctxt->geran_freq_stats_list));

							*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;

							*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

							*p_is_old_nr_send = SON_FALSE;
						}
					}
					else if (SON_RRM_MAX_GERAN_FREQ_ALLOWED  > count)
					{
						p_list_node_new = anr_insert_record_in_geran_freq_stats_list(                
								&p_cell_ctxt->geran_freq_stats_list,
								p_new_hash->u.geran_neighbor_info.arfcn,
								p_new_hash->u.geran_neighbor_info.band_ind);

						if (SON_PNULL != p_list_node_new)
						{
							decrement_geran_nbr_count_for_rrm(p_cell_ctxt, p_list_node_old);

							*p_error_code = SON_NO_ERROR;

							*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

							*p_is_old_nr_send = SON_TRUE;
						}
						else
						{
							*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;

							*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

							*p_is_old_nr_send = SON_FALSE;
						}
					}
					else
					{
						SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
								SON_DETAILED,
								"New ARFCN can not be "
								"configured as there are [%d] already configured ARFCNs\n\n",
								list_count(&p_cell_ctxt->geran_freq_stats_list));

						*p_error_code = SON_ERR_ANR_MODIFICATION_NOT_ALLOWED;

						*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

						*p_is_old_nr_send = SON_FALSE;
					} 

					/*IMP*/
				}
			}
		}

	}
	SON_UT_TRACE_EXIT();
	return;
} /* anr_determine_geran_changes_in_nrt_when_old_nr_send */



/******************************************************************************
 * Function Name  : anr_determine_the_changes_in_inter_nrt
 * Inputs         : p_cell_ctxt - Pointer to cell context
 *                  p_old_hash      - Pointer to old NBR hash record
 *                  p_new_hash      - Pointer to new NBR hash record
 *                  old_ho_status   - HO status for old NBR
 *                  new_ho_status   - HO status for new NBR
 *                  p_is_old_nr_send- flag pointer to check if old NBR is sendable
 *                                    to RRM
 *                  p_is_rrm_notif_reqd_for_new_nr- Status flag to check if new
 *                                    NBR if to be notified to RRM
 *                  p_error_code- Pointer to error code
 * Outputs        : None.
 *
 * Returns        : son_void_t      - None
 * Description    : This function is used to determine the changes due to inter RAT
 *                  NRT modification / insertion
 ******************************************************************************/
son_void_t
anr_determine_the_changes_in_inter_nrt
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_inter_rat_hash_rec_t  *p_old_hash,
 anr_inter_rat_hash_rec_t  *p_new_hash,
 son_ho_status_et          old_ho_status,
 son_ho_status_et          new_ho_status,
 son_bool_et               *p_is_old_nr_send,
 son_bool_et               *p_is_rrm_notif_reqd_for_new_nr,
 son_error_et              *p_error_code
 )
{
	SON_UT_TRACE_ENTER();

	if (p_old_hash == SON_PNULL || p_new_hash == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"p_old_hash or p_new_hash is NULL");
	}
	else
	{
		if (ANR_UTRAN_NEIGHBOR == p_new_hash->t)
		{
			if (SON_FALSE == *p_is_old_nr_send)
			{
				anr_determine_utran_changes_in_nrt_when_old_nr_not_send(p_cell_ctxt, p_old_hash, p_new_hash,
                        /*SPR 17777 +-*/
						p_is_old_nr_send,
						p_is_rrm_notif_reqd_for_new_nr, p_error_code );
			}
			else
			{
				anr_determine_utran_changes_in_nrt_when_old_nr_send(p_cell_ctxt, p_old_hash, p_new_hash,
						old_ho_status, new_ho_status, p_is_old_nr_send,
						p_is_rrm_notif_reqd_for_new_nr, p_error_code );
			}
		}
		else if (ANR_GERAN_NEIGHBOR == p_new_hash->t)
		{
			if (SON_FALSE == *p_is_old_nr_send)
			{
                        /*SPR 17777 +-*/
				anr_determine_geran_changes_in_nrt_when_old_nr_not_send(p_cell_ctxt, p_old_hash, p_new_hash,
						p_is_old_nr_send,
						p_is_rrm_notif_reqd_for_new_nr, p_error_code );
			}
			else
			{
				anr_determine_geran_changes_in_nrt_when_old_nr_send(p_cell_ctxt, p_old_hash, p_new_hash,
						old_ho_status, new_ho_status, p_is_old_nr_send,
						p_is_rrm_notif_reqd_for_new_nr, p_error_code );
			}
		}
		else if (ANR_CDMA_NEIGHBOR == p_new_hash->t)
		{
			if (SON_FALSE == *p_is_old_nr_send)
			{
                        /*SPR 17777 +-*/
				anr_determine_cdma_changes_in_nrt_when_old_nr_not_send(p_cell_ctxt, p_old_hash, p_new_hash,
						p_is_old_nr_send,
						p_is_rrm_notif_reqd_for_new_nr, p_error_code );
			}
			else
			{
				anr_determine_cdma_changes_in_nrt_when_old_nr_send(p_cell_ctxt, p_old_hash, p_new_hash,
						old_ho_status, new_ho_status, p_is_old_nr_send,
						p_is_rrm_notif_reqd_for_new_nr, p_error_code );
			}
		}
	}

	SON_UT_TRACE_EXIT();
} /* anr_determine_the_changes_in_inter_nrt */

/******************************************************************************
 * Function Name  : anr_validate_csg_id_access_mode_for_eutran_neighbor
 * Inputs         : p_hash          - Pointer to intra rat hash record
 * Outputs        : None.
 * Returns        : son_bool_et      - SON_TRUE / SON_FALSE
 * Description    : This function validates the csg id and access mode for the
 *                  eutran neighbours for inserting/ deleting/ modifying any
 *                  neighbour in NRT
 ******************************************************************************/
son_bool_et
anr_validate_csg_id_access_mode_for_eutran_neighbor
(
 anr_intra_rat_hash_rec_t *p_hash
 )
{
	son_bool_et ret = SON_TRUE;


	SON_UT_TRACE_ENTER();

	if (p_hash == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"p_hash is NULL");
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	if (SON_INTRA_CSG_ID_PRESENT & p_hash->neighbor_info.bitmask)
	{
		if ((SON_ACCESS_MODE_PRESENT & p_hash->neighbor_info.bitmask) &&  /*Condition modified for SPR-10230*/
				((p_hash->neighbor_info.access_mode == SON_OPEN) || (p_hash->neighbor_info.access_mode == SON_OPEN_FEMTO)))
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR, "anr_validate_csg_id_access_mode_for_eutran_neighbor:"
					"Validation Fails for NR[Cell Id :0x%x] as As Acess mode"
					"is OPEN or OPEN_FEMTO and CSG Identity is also present\n",
					son_convert_char_cell_id_to_int_cell_id(p_hash->neighbor_info.nbr_cgi.cell_identity));
			ret = SON_FALSE;
		}
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* anr_validate_csg_id_access_mode_for_eutran_neighbor */

/******************************************************************************
 * Function Name  : anr_validate_csg_id_access_mode_for_utran_neighbor
 * Inputs         : p_hash          - Pointer to inter rat hash record
 * Outputs        : None.
 * Returns        : son_bool_et      - SON_TRUE / SON_FALSE
 * Description    : This function validates the csg id and access mode for the
 *                  utran neighbours for inserting/ deleting/ modifying any
 *                  neighbour in NRT
 ******************************************************************************/
son_bool_et
anr_validate_csg_id_access_mode_for_utran_neighbor
(
 const anr_inter_rat_hash_rec_t *p_hash
 )
{
	son_bool_et ret = SON_TRUE;


	SON_UT_TRACE_ENTER();

	if (p_hash == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"p_hash is NULL");
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	if (ANR_UTRAN_NEIGHBOR == p_hash->t)
	{
		if (SON_UTRAN_CSG_ID_PRESENT & p_hash->u.utran_neighbor_info.bitmask)
		{
			if ((SON_UTRAN_ACCESS_MODE_PRESENT &  p_hash->u.utran_neighbor_info.bitmask) &&
					p_hash->u.utran_neighbor_info.access_mode  == SON_OPEN)
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR, "anr_validate_csg_id_access_mode_for_utran_neighbor:"
						"Validation Fails for NR as As Acess mode"
						"is OPEN and CSG Identity is also present\n");
				ret = SON_FALSE;
			}
		}
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* anr_validate_csg_id_access_mode_for_utran_neighbor */
/******************************************************************************
 * Function Name  : anr_validate_csg_id_access_mode_for_inter_rat_neighbor
 * Inputs         : p_hash          - Pointer to inter rat hash record
 * Outputs        : None.
 * Returns        : son_bool_et      - SON_TRUE / SON_FALSE
 * Description    : This function validates the csg id and access mode for the
 *                  inter RAT neighbours for inserting/ deleting/ modifying any neighbour
 *                  in NRT
 ******************************************************************************/
son_bool_et
anr_validate_csg_id_access_mode_for_inter_rat_neighbor
(
 anr_inter_rat_hash_rec_t *p_hash_rec
 )
{
	son_bool_et is_data_valid = SON_TRUE;


	SON_UT_TRACE_ENTER();

	if (p_hash_rec == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"p_hash_rec is NULL");
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	if (ANR_UTRAN_NEIGHBOR == p_hash_rec->t)
	{
		if (SON_UTRAN_CSG_ID_PRESENT & p_hash_rec->u.utran_neighbor_info.bitmask)
		{
			if ((SON_UTRAN_ACCESS_MODE_PRESENT & p_hash_rec->u.utran_neighbor_info.bitmask) &&
					p_hash_rec->u.utran_neighbor_info.access_mode == SON_OPEN)
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR,
						"anr_validate_csg_id_access_mode_for_inter_rat_neighbor:Validation fails "
						"as Access mode is OPEN and CSG Identity is present\n");
				is_data_valid = SON_FALSE;
			}
		}
	}

	SON_UT_TRACE_EXIT();
	return is_data_valid;
} /* anr_validate_csg_id_access_mode_for_inter_rat_neighbor */

/******************************************************************************
 * Function Name  : get_enb_type_for_the_given_cell
 * Inputs         : p_hash            - hash record
 * Outputs        : None
 * Returns        : anr_enbs_et - Enum for determining eNB type
 * Description    : This function determines enb type of the cell.The type
 *                   value returned by the function is taken from the internal enum
 ******************************************************************************/
static anr_enbs_et
get_enb_type_for_the_given_cell
(
 anr_intra_rat_hash_rec_t *p_hash
 )
{
	SON_UT_TRACE_ENTER();
	anr_enbs_et  enb_type  = INVALID_ENB_TYPE; 
	if (p_hash == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"p_hash is NULL");
		SON_UT_TRACE_EXIT();
		return enb_type;
	}

	/* SPR 10103 FIX START */
	if(p_hash->enb_id.enb_type == SON_HOME_ENB)
	{
		enb_type = FEMTO_ENB;
	}
	else if(p_hash->enb_id.enb_type == SON_MACRO_ENB)
	{
		if(p_hash->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT)
		{
			if(p_hash->neighbor_info.access_mode == SON_OPEN)
			{
				enb_type = MACRO_ENB_TYPE_A;
			}
		}
		else
		{
			enb_type = MACRO_ENB_TYPE_B;
		}
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"get_enb_type_for_the_given_cell: "
				"Invalid enb_type[%d]\n", enb_type);
		SON_UT_TRACE_EXIT();
		return enb_type;
	}
	/* SPR 10103 FIX END */

	SON_UT_TRACE_EXIT();
	return enb_type;
} /* get_enb_type_for_the_given_cell */

/******************************************************************************
 * Function Name  : get_the_x2_status_value
 * Inputs         : p_existing_hash   - Existing hash record from table
 *                : p_new_hash        - New hash tuple
 * Outputs        : p_err_data        - Error code
 *                : p_nr_x2_status    - X2 status
 * Returns        : son_void_t        - None
 * Description    : This function determines the X2 status value.
 ******************************************************************************/
static son_void_t
get_the_x2_status_value
(
 anr_intra_rat_hash_rec_t *p_existing_hash,
 anr_intra_rat_hash_rec_t *p_new_hash,
 son_error_et       *p_err_data,
 son_x2_status_et  *p_nr_x2_status
 )
{
	son_nbr_enb_hash_tuple_t     *p_existing_enb_id      = SON_PNULL;
	son_nbr_enb_hash_tuple_t     *p_new_enb_id           = SON_PNULL;
	son_error_et            error_code              = SON_NO_ERROR;
	anr_enbs_et                  existing_cells_enb_type = INVALID_ENB_TYPE;
	anr_enbs_et                  new_cells_enb_type      = INVALID_ENB_TYPE;


	SON_UT_TRACE_ENTER();

	/*SPR 10103 Start*/
	p_existing_enb_id  =  x2_find_nbr_enb(x2_get_enb_nrt(),
			&p_existing_hash->enb_id, &error_code);

	p_new_enb_id = x2_find_nbr_enb(x2_get_enb_nrt(),
			&p_new_hash->enb_id, &error_code);
	/*SPR 10103 End*/

	existing_cells_enb_type  = get_enb_type_for_the_given_cell(p_existing_hash);
	new_cells_enb_type       = get_enb_type_for_the_given_cell(p_new_hash);

	if (existing_cells_enb_type == INVALID_ENB_TYPE ||
			new_cells_enb_type      == INVALID_ENB_TYPE)
	{
		*p_err_data = SON_ERR_INVALID_PARAMS;
	}
	else if (new_cells_enb_type  == MACRO_ENB_TYPE_A)
	{
		if (FEMTO_ENB == existing_cells_enb_type)
		{
			*p_err_data = SON_ERR_INVALID_PARAMS;
		}
		else
		{ 
			*p_nr_x2_status  = SON_X2_ALLOWED_DISCONNECTED; 
			if (SON_PNULL != p_existing_enb_id)
			{
				*p_nr_x2_status  = get_x2_sts_value_for_nbr_from_enb_rec(&p_existing_enb_id->data);
			}

			*p_err_data = SON_NO_ERROR;
		} 
	}
	else if (new_cells_enb_type == MACRO_ENB_TYPE_B)
	{
		if (p_new_enb_id == SON_PNULL)
		{
			*p_nr_x2_status  = SON_X2_ALLOWED_DISCONNECTED;
			if (SON_PNULL != p_existing_enb_id)
			{
				*p_nr_x2_status  = get_x2_sts_value_for_nbr_from_enb_rec(&p_existing_enb_id->data);    
			}

			*p_err_data = SON_NO_ERROR;
		}
		else
		{
			if (FEMTO_ENB == existing_cells_enb_type)
			{
				*p_err_data = SON_ERR_INVALID_PARAMS;
			}
			else 
			{
				if (p_existing_enb_id != SON_PNULL &&
						SON_MEMCMP(&p_existing_enb_id->data, &p_new_enb_id->data, sizeof(son_global_enb_id_t)))
				{
					*p_err_data = SON_ERR_INVALID_PARAMS;
				}
				else
				{
					*p_nr_x2_status  = get_x2_sts_value_for_nbr_from_enb_rec(&p_new_enb_id->data);
					*p_err_data = SON_NO_ERROR;
				}
			}
		}
	}
	else
	{
		if (existing_cells_enb_type == MACRO_ENB_TYPE_A)
		{
			*p_err_data = SON_ERR_INVALID_PARAMS;
		}
		else if (existing_cells_enb_type == MACRO_ENB_TYPE_B)
		{
			if (p_existing_enb_id != SON_PNULL && p_new_enb_id != SON_PNULL)
			{
				*p_err_data = SON_ERR_INVALID_PARAMS;
			}

			if (p_existing_enb_id == SON_PNULL)
			{
				*p_nr_x2_status  = SON_X2_ALLOWED_DISCONNECTED;
				if (SON_PNULL != p_new_enb_id)
				{
					*p_nr_x2_status  = get_x2_sts_value_for_nbr_from_enb_rec(&p_new_enb_id->data);
				}

				*p_err_data = SON_NO_ERROR;
			}
		}
		else
		{
			*p_nr_x2_status  = SON_X2_ALLOWED_DISCONNECTED;
			if (SON_PNULL != p_new_enb_id)
			{
				*p_nr_x2_status  = get_x2_sts_value_for_nbr_from_enb_rec(&p_new_enb_id->data);
			}

			*p_err_data = SON_NO_ERROR;
		}
	}

	SON_UT_TRACE_EXIT();
} /* get_the_x2_status_value */

/*RELEASE1_2_4 Changes for the Inter RAT*/

/******************************************************************************
 * Function Name  : anr_init_intra_rat_hash_rec_table
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_FAILURE,SON_SUCCESS
 * Description    : This function created hash table context pool
 ******************************************************************************/
static son_return_et
anr_init_intra_rat_hash_rec_table
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	if (!cvContextPoolSetup(anr_get_intra_rat_hash_rec_pool(),
				sizeof(anr_intra_rat_hash_nr_tuple_t),
				ANR_INIT_HASH_CTXT_POOL,
				ANR_MAX_HASH_CTXT_POOL,
				ANR_INIT_HASH_CTXT_POOL))
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_init_intra_rat_hash_rec_table: Hash Context Pool "
				"Setup could not be completed\n");
		return SON_FAILURE;
	}

	SON_UT_TRACE_EXIT();
	return SON_SUCCESS;
} /* anr_init_intra_rat_hash_rec_table */

/******************************************************************************
 * Function Name  : anr_init_inter_rat_hash_rec_table
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_FAILURE,SON_SUCCESS
 * Description    : This function is used to create inter RAT hash table 
 *                  context pool
 ******************************************************************************/
static son_return_et
anr_init_inter_rat_hash_rec_table
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	if (!cvContextPoolSetup(anr_get_inter_rat_hash_rec_pool(),
				sizeof(anr_inter_rat_hash_nr_tuple_t),
				ANR_INIT_HASH_CTXT_POOL,
				ANR_MAX_HASH_CTXT_POOL,
				ANR_INIT_HASH_CTXT_POOL))
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_init_inter_rat_hash_rec_table: Hash Context Pool "
				"Setup could not be completed\n");
		return SON_FAILURE;
	}

	SON_UT_TRACE_EXIT();
	return SON_SUCCESS;
} /* anr_init_inter_rat_hash_rec_table */

/******************************************************************************
 * Function Name  : anr_destroy_intra_rat_hash_rec_table
 * Inputs         : None
 * Outputs        : None
 * Returns        : void
 * Description    : This function destroys hash table context pool
 *****************************************************************************/
static son_void_t
anr_destroy_intra_rat_hash_rec_table
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	qvPoolDelete((anr_get_intra_rat_hash_rec_pool())->pool);
}

/******************************************************************************
 * Function Name  : anr_destroy_inter_rat_hash_rec_table
 * Inputs         : None
 * Outputs        : None
 * Returns        : void
 * Description    : This function destroys inter RAT hash table context pool
 *****************************************************************************/
static son_void_t
anr_destroy_inter_rat_hash_rec_table
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	qvPoolDelete((anr_get_inter_rat_hash_rec_pool())->pool);
}

/*******************************************************************************
 * Function Name  : anr_init_hash_node_table
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_SUCCESS,SON_FAILURE
 * Description    : This function created hash table pool
 ******************************************************************************/
static son_return_et
anr_init_hash_node_table
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	if (!cvContextPoolSetup(anr_get_hash_node_pool(),
				sizeof(SON_HASH_NODE *) * 
				ANR_MAX_INITIAL_NR_TBL_BKT,
				ANR_INIT_HASH_CTXT_POOL,
				ANR_MAX_HASH_CTXT_POOL,
				ANR_INIT_HASH_CTXT_POOL))
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_init_hash_node_table: Hash Table Pool Setup could not be "
				"completed\n");
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	SON_UT_TRACE_EXIT();
	return SON_SUCCESS;
} /* anr_init_hash_node_table */

/******************************************************************************
 * Function Name  : anr_init_intra_rat_hash
 * Inputs         : H  Pointer to SON_HASH
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : This function created hash table
 ******************************************************************************/
son_return_et
anr_init_intra_rat_hash
(
 SON_HASH *H
 )
{
	SON_HASH_NODE * *table;


	ANR_GET_TABLE(anr_get_hash_node_pool(), table);
	hash_init(H, ANR_MAX_INITIAL_NR_TBL_BKT, anr_intra_rat_hash_nr_func,
			anr_intra_rat_cmp_nr_func,
			anr_intra_rat_key_off_nr_func, table);
	return SON_SUCCESS;
}

/******************************************************************************
 * Function Name  : anr_init_inter_rat_hash
 * Inputs         : H  Pointer to SON_HASH
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : This function is used to create inter RAT hash table
 ******************************************************************************/
son_return_et
anr_init_inter_rat_hash
(
 SON_HASH *H
 )
{
	SON_HASH_NODE * *table;


	ANR_GET_TABLE(anr_get_hash_node_pool(), table);
	hash_init(H, ANR_MAX_INITIAL_NR_TBL_BKT, anr_inter_rat_hash_nr_func,
			anr_inter_rat_cmp_nr_func,
			anr_inter_rat_key_off_nr_func, table);
	return SON_SUCCESS;
}

/*******************************************************************************
 * Function Name  : anr_destroy_hash_node_table
 * Inputs         : None
 * Outputs        : None
 * Returns        : void
 * Description    : Deletes hash table
 ******************************************************************************/
static son_void_t
anr_destroy_hash_node_table
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	cvContextPoolDelete(anr_get_hash_node_pool());
}

/******************************************************************************
 * Function Name  : hasher
 * Inputs         : p_first - Pointer to string
 * Returns        : son_ul32
 * Description    : This function generates the hash key for a string
 *                : This algorithm has been taken from std::Collate
 ******************************************************************************/
son_ul32
hasher
(
 son_u8 *p_first,
 son_u8 *p_last
 )
{
	son_ul32 val = 0;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL != p_first && SON_PNULL != p_last)
	{
		for (; p_first != p_last; ++p_first)
		{
			val = (val << 8 | val >> 24) + *p_first;
		}
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"hasher: Invalid input parameters. NULL pointers passed.\n");
	}

	SON_UT_TRACE_EXIT();
	return val;
} /* hasher */

/******************************************************************************
 * Function Name  : anr_get_hash_of_intra_rat_cell_id
 * Inputs         : p_cell_id   - Pointer to cell id
 * Outputs        : None
 * Returns        : The hash value for the given cell Id
 * Description    : This is a function for generating the hash value for
 *                                :	the given cell Id.
 ****************************************************************************/
son_ul32
anr_get_hash_of_intra_rat_cell_id
(
 son_void_t *p_cell_id
 )
{
	son_ul32 key = 0;
	son_u8  key_len = 0;
	son_u8 temp[SON_MCC_OCTET_SIZE +
		SON_CELL_ID_OCTET_SIZE +
		SON_MAX_MNC_OCTET_SIZE] = {0};
	son_oam_log_on_off_et log_mode = anr_get_log_mode();
	son_intra_rat_global_cell_id_t *p_nr_cell_id = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_id)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_get_hash_of_intra_rat_cell_id: Could not found hash value, "
				"p_cell_id is NULL\n");

		SON_UT_TRACE_EXIT();
		return key;
	}

	p_nr_cell_id = (son_intra_rat_global_cell_id_t *)p_cell_id;
	/* Convert MCC + MNC + Cell ID */
	SON_MEMCPY(&temp[0], p_nr_cell_id->plmn_id.mcc, SON_MCC_OCTET_SIZE);
	key_len = SON_MCC_OCTET_SIZE;

	SON_MEMCPY(&temp[key_len],
			p_nr_cell_id->plmn_id.mnc,
			p_nr_cell_id->plmn_id.num_mnc_digit);
	key_len += p_nr_cell_id->plmn_id.num_mnc_digit;

	SON_MEMCPY(&temp[SON_MCC_OCTET_SIZE + p_nr_cell_id->plmn_id.
			num_mnc_digit],
			p_nr_cell_id->cell_identity,
			SON_CELL_ID_OCTET_SIZE);
	key_len += SON_CELL_ID_OCTET_SIZE;

	key = hasher(&temp[0],
			&temp[key_len - 1]);

	SON_UT_TRACE_EXIT();
	return key;
} /* anr_get_hash_of_intra_rat_cell_id */

/******************************************************************************
 * Function Name  : anr_get_hash_of_cell_id
 * Inputs         : p_cell_id   - Pointer to cell id 
 * Outputs        : None
 * Returns        : son_ul32 - The hash value for the given cell Id
 * Description    : This is a function for generating the hash value for
 *                                :	the given cell Id.
 ****************************************************************************/
static son_ul32
anr_get_hash_of_cell_id
(
 son_void_t *p_cell_id
 )
{
	son_ul32 key = 0;
	son_u8  key_len = 0;
	son_u8 temp[SON_MCC_OCTET_SIZE +
		SON_CELL_ID_OCTET_SIZE +
		SON_MAX_MNC_OCTET_SIZE] = {0};
	son_oam_log_on_off_et log_mode = anr_get_log_mode();
	son_utran_geran_global_cell_id_t *p_nr_cell_id = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_id)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_get_hash_of_cell_id: Could not found hash value, "
				"p_cell_id is NULL\n");

		SON_UT_TRACE_EXIT();
		return key;
	}

	p_nr_cell_id = (son_utran_geran_global_cell_id_t *)p_cell_id;
	/* Convert MCC + MNC + Cell ID */
	SON_MEMCPY(&temp[0], p_nr_cell_id->plmn_id.mcc, SON_MCC_OCTET_SIZE);
	key_len = SON_MCC_OCTET_SIZE;

	SON_MEMCPY(&temp[key_len],
			p_nr_cell_id->plmn_id.mnc,
			p_nr_cell_id->plmn_id.num_mnc_digit);
	key_len += p_nr_cell_id->plmn_id.num_mnc_digit;

	SON_MEMCPY(&temp[SON_MCC_OCTET_SIZE + p_nr_cell_id->plmn_id.
			num_mnc_digit],
			p_nr_cell_id->cell_identity,
			SON_CELL_ID_OCTET_SIZE);
	key_len += SON_CELL_ID_OCTET_SIZE;

	key = hasher(&temp[0],
			&temp[key_len - 1]);

	SON_UT_TRACE_EXIT();
	return key;
} /* anr_get_hash_of_cell_id */

/******************************************************************************
 * Function Name  : anr_get_hash_of_cdma_cell_id
 * Inputs         : p_cell_id   - Pointer to cell id 
 * Outputs        : None
 * Returns        : son_ul32 - The hash value for the given cell Id
 * Description    : This is a function for generating the hash value for
 *                                :	the given cell Id.
 ****************************************************************************/
static son_ul32
anr_get_hash_of_cdma_cell_id
(
 son_void_t *p_cell_id
 )
{
	son_ul32 key = 0;
	son_u8  key_len = 0;
	son_u8 temp[SON_CELL_GLOBAL_ID_HRPD_SIZE] = {0};
	son_oam_log_on_off_et log_mode = anr_get_log_mode();
	son_cdma_global_cell_id_t *p_nr_cell_id = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_id)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_get_hash_of_cdma_cell_id: Could not found hash value, "
				"p_cell_id is NULL\n");

		SON_UT_TRACE_EXIT();
		return key;
	}

	p_nr_cell_id = (son_cdma_global_cell_id_t *)p_cell_id;

	if (p_nr_cell_id->bitmask & SON_CGI_1X_RTT_PRESENT)
	{
		SON_MEMCPY(&temp[0], p_nr_cell_id->cell_global_id_1X_RTT, SON_CELL_GLOBAL_ID_1X_RTT_SIZE);
		key_len += SON_CELL_GLOBAL_ID_1X_RTT_SIZE;
	}
	else if (p_nr_cell_id->bitmask & SON_CGI_HRPD_PRESENT)
	{
		SON_MEMCPY(&temp[0], p_nr_cell_id->cell_global_id_HRPD, SON_CELL_GLOBAL_ID_HRPD_SIZE);
		key_len += SON_CELL_GLOBAL_ID_HRPD_SIZE;
	}
	else
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_get_hash_of_cdma_cell_id: wrong CDMA type");
		SON_UT_TRACE_EXIT();
		return key;
	}

	key = hasher(&temp[0],
			&temp[key_len - 1]);

	SON_UT_TRACE_EXIT();
	return key;
} /* anr_get_hash_of_cdma_cell_id */

/******************************************************************************
 * Function Name  : anr_intra_rat_hash_nr_func
 * Inputs         : p_key - Pointer to the structure of type 
 *                          anr_intra_rat_hash_rec_t
 * Outputs        : None
 * Returns        : son_ul32 - The hash value for the intra RAT hash table
 * Description    : This is a hash function for generating the hash value for
 *                                :	the intra RAT hash table.
 ****************************************************************************/
son_ul32
anr_intra_rat_hash_nr_func
(
 const son_void_t *p_key
 )
{
	son_ul32 key = 0;
	son_intra_rat_global_cell_id_t  *p_nr_cell_id; 


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_key)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_BRIEF,
				"anr_intra_rat_hash_nr_func: Hash Key Generation for "
				"NR Hash Table failed, p_key is NULL");
	}
	else 
	{
		/* Get the cell Id */
		p_nr_cell_id = &(((anr_intra_rat_hash_rec_t *)p_key)->
				neighbor_info.nbr_cgi); 
		/* Get the hash value */
		key = anr_get_hash_of_intra_rat_cell_id((son_void_t *)p_nr_cell_id);

#ifdef SON_DEBUG        
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_DETAILED,
				"Hash Key generated for "
				"NR Hash Table is [%u]\n", key);
#endif                
	}

	SON_UT_TRACE_EXIT();
	return key;
} /* anr_intra_rat_hash_nr_func */

/******************************************************************************
 * Function Name  : anr_inter_rat_hash_nr_func
 * Inputs         : p_key - Pointer to the structure of type 
 *                          anr_inter_rat_hash_rec_t
 * Outputs        : None
 * Returns        : son_ul32 - The hash value for the inter RAT hash table
 * Description    : This is a hash function for generating the hash value for
 *                                :	the inter RAT hash table.
 ****************************************************************************/
son_ul32
anr_inter_rat_hash_nr_func
(
 const son_void_t *p_key
 )
{
	son_ul32 key = 0;
	anr_inter_rat_hash_rec_t *p_nr_rec = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_key)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_BRIEF,
				"anr_inter_rat_hash_nr_func: Hash Key Generation for "
				"NR Hash Table failed, p_key is NULL");
	}
	else
	{
		p_nr_rec = (anr_inter_rat_hash_rec_t *)p_key;

		if (ANR_UTRAN_NEIGHBOR == p_nr_rec->t)
		{
			son_utran_geran_global_cell_id_t *p_nr_cell_id = SON_PNULL;
			/* Get the cell Id */
			p_nr_cell_id = &(p_nr_rec->u.utran_neighbor_info.nbr_cgi);  
			/* Get the hash value */
			key = anr_get_hash_of_cell_id((son_void_t *)p_nr_cell_id);
		}
		else if (ANR_GERAN_NEIGHBOR == p_nr_rec->t)
		{
			son_utran_geran_global_cell_id_t *p_nr_cell_id = SON_PNULL; 
			/* Get the cell Id */
			p_nr_cell_id = &(p_nr_rec->u.geran_neighbor_info.nbr_cgi);  
			/* Get the hash value */
			key = anr_get_hash_of_cell_id((son_void_t *)p_nr_cell_id);          
		}
		else if (ANR_CDMA_NEIGHBOR == p_nr_rec->t)
		{
			son_cdma_global_cell_id_t *p_nr_cell_id = SON_PNULL;
			/* Get the cell Id */
			p_nr_cell_id = &(p_nr_rec->u.cdma_neighbor_info.nbr_cgi);
			/* Get the hash value */
			key = anr_get_hash_of_cdma_cell_id((son_void_t *)p_nr_cell_id);
		}
		else
		{
			/*For future use*/    
		}

#ifdef SON_DEBUG       
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_DETAILED,
				"Hash Key Generated for "
				"inter RAT NR Hash Table is [%lu]\n", key);
#endif                
	}

	SON_UT_TRACE_EXIT();
	return key;
} /* anr_inter_rat_hash_nr_func */

/*******************************************************************************
 * Function Name  : anr_intra_rat_cmp_nr_func
 * Inputs         : p_val_1 - Pointer to the structure of type 
 *                            anr_intra_rat_hash_rec_t
 *                : p_val_2 - Pointer to the structure of type
 *                             anr_intra_rat_hash_rec_t
 * Outputs        : None
 * Returns        : son_s32 - SON_TRUE In case comparision is successful
 *                :           SON_FALSE In case comparision failed
 * Description    : This is a hash function used for comparing the key values
 *                                :	of NR Hash table
 ******************************************************************************/
son_s32
anr_intra_rat_cmp_nr_func
(
 const son_void_t *val_1,
 const son_void_t *val_2
 )
{
	const anr_intra_rat_hash_rec_t    *p_rec_1    = SON_PNULL;
	const anr_intra_rat_hash_rec_t    *p_rec_2    = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_rec_1 = (anr_intra_rat_hash_rec_t *)val_1;
	p_rec_2 = (anr_intra_rat_hash_rec_t *)val_2;

	SON_UT_TRACE_EXIT();
	return SON_MEMCMP(&p_rec_1->neighbor_info.nbr_cgi, 
			&p_rec_2->neighbor_info.nbr_cgi, 
			sizeof(son_intra_rat_global_cell_id_t));
}

/*******************************************************************************
 * Function Name  : anr_inter_rat_cmp_nr_func
 * Inputs         : p_val_1 - Pointer to the structure of type 
 *                            anr_inter_rat_hash_rec_t
 *                : p_val_2 - Pointer to the structure of type
 *                             anr_inter_rat_hash_rec_t
 * Outputs        : None
 * Returns        : son_s32 - Zero In case comparision is successful
 *                :           Non-Zero In case comparision failed
 * Description    : This is a hash function used for comparing the key values
 *                                :	of inter RAT NR Hash table
 ******************************************************************************/
son_s32
anr_inter_rat_cmp_nr_func
(
 const son_void_t *val_1,
 const son_void_t *val_2
 )
{
	const anr_inter_rat_hash_rec_t    *p_rec_1    = SON_PNULL;
	const anr_inter_rat_hash_rec_t    *p_rec_2    = SON_PNULL;
	son_s32                            cmp_result = 0;


	SON_UT_TRACE_ENTER();

	p_rec_1 = (anr_inter_rat_hash_rec_t *)val_1;
	p_rec_2 = (anr_inter_rat_hash_rec_t *)val_2;

	if (ANR_UTRAN_NEIGHBOR == p_rec_1->t)
	{
		cmp_result = SON_MEMCMP(&p_rec_1->u.utran_neighbor_info.nbr_cgi,
				&p_rec_2->u.utran_neighbor_info.nbr_cgi,
				sizeof(son_utran_geran_global_cell_id_t));
	}
	else if (ANR_GERAN_NEIGHBOR == p_rec_1->t)
	{
		cmp_result = SON_MEMCMP(&p_rec_1->u.geran_neighbor_info.nbr_cgi,
				&p_rec_2->u.geran_neighbor_info.nbr_cgi,
				sizeof(son_utran_geran_global_cell_id_t));
	}
	else if (ANR_CDMA_NEIGHBOR == p_rec_1->t)
	{
		cmp_result = SON_MEMCMP(&p_rec_1->u.cdma_neighbor_info.nbr_cgi,
				&p_rec_2->u.cdma_neighbor_info.nbr_cgi,
				sizeof(son_cdma_global_cell_id_t)); 
	}
	else
	{
		/*For future use*/
	}

	SON_UT_TRACE_EXIT();
	return cmp_result;
} /* anr_inter_rat_cmp_nr_func */

/******************************************************************************
 * Function Name  : anr_intra_rat_key_off_nr_func
 * Inputs         : p_node -  Pointer to the anchor field of 
 *                            NR record node in NR table.                  
 * Outputs        : None
 * Returns        : son_void_t * - Pointer to the intra rat hash data 
 * Description    : This is a hash function used for find the 
 *                  data corresponding to the key value
 ******************************************************************************/
const son_void_t *
anr_intra_rat_key_off_nr_func
(
 const SON_HASH_NODE *p_node
 )
{
	const son_void_t           			*p_data     = SON_PNULL;
	const anr_intra_rat_hash_nr_tuple_t  	*p_tpl      = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_node)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_BRIEF,
				"anr_intra_rat_key_off_nr_func: Failed to find the data for"
				" the given node, p_node is NULL");
	}
	else
	{
		p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_intra_rat_hash_nr_tuple_t, anchor, p_node);
		p_data = (son_void_t *)&p_tpl->data;
	}

	SON_UT_TRACE_EXIT();
	return p_data;
} /* anr_intra_rat_key_off_nr_func */

/******************************************************************************
 * Function Name  : anr_inter_rat_key_off_nr_func
 * Inputs         : p_node -  Pointer to the anchor field of 
 *                            NR record node in inter RAT NR table.                  
 * Outputs        : None
 * Returns        : son_void_t* - Pointer to inter rat hash data
 * Description    : This is a hash function used for finding the 
 *                  data corresponding to the key value from inter RAT NR hash 
 *                  table
 ******************************************************************************/
const son_void_t *
anr_inter_rat_key_off_nr_func
(
 const SON_HASH_NODE *p_node
 )
{
	const son_void_t           			*p_data     = SON_PNULL;
	const anr_inter_rat_hash_nr_tuple_t  	*p_tpl      = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_node)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_BRIEF,
				"anr_inter_rat_key_off_nr_func: Failed to find the data for"
				" the given node, p_node is NULL");
	}
	else
	{
		p_tpl = (anr_inter_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_inter_rat_hash_nr_tuple_t, anchor, p_node);
		p_data = (son_void_t *)&p_tpl->data;
	}

	SON_UT_TRACE_EXIT();
	return p_data;
} /* anr_inter_rat_key_off_nr_func */

/*SPR-9574-Start*/

/*****************************************************************************
 * Function Name  : anr_set_correct_access_mode_of_intra_rat_nbr
 * Inputs         : p_nr   Intra rat neighbor info
 * Outputs        : None
 * Returns        : son_bool_et  - SON_TRUE if correct access mode set
 *                                 and nbr can be inserted  
 *                                 SON_FALSE if access_mode not set and
 *                                 nbr cannot be inserted
 * Description    : This function sets the correct access mode value of the Intra
 *                  rat nbr based on pci ranges configured
 ****************************************************************************/
static son_bool_et
anr_set_correct_access_mode_of_intra_rat_nbr
(
 anr_intra_rat_hash_rec_t *p_nr
 )
{
	anr_intra_freq_list_node_t  *p_intra_freq_list_node = SON_PNULL;
	anr_inter_freq_list_node_t  *p_inter_freq_list_node = SON_PNULL;
	son_u32                      temp_earfcn = 0;
	son_u16                      temp_open_pci_end = 0;
	son_u16                      temp_closed_pci_end = 0;
	son_u16                      temp_hybrid_pci_end = 0;
	/*SPR-10426-Start*/
	son_bool_et                  ret_val = SON_TRUE;
	son_bool_et                  is_macro_nbr = SON_TRUE;
	/*SPR-10426-End*/

	SON_UT_TRACE_ENTER();

	temp_earfcn = p_nr->neighbor_info.earfcn;

	/*Check if the neighbor belongs to Inter Freq or Intra Freq category*/
	p_intra_freq_list_node = anr_find_freq_in_intra_freq_list(temp_earfcn);
	p_inter_freq_list_node = anr_find_freq_in_inter_freq_list(temp_earfcn);

	/*SPR-11889-Start*/
	/*Condition removed*/
	/*SPR-11889-End*/

	if(p_intra_freq_list_node != SON_PNULL )
	{
		/*SPR-10426-Start*/
		if(p_intra_freq_list_node->intra_freq_value.open_pci_range_present == SON_TRUE)
		{
			/*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
			temp_open_pci_end = son_determine_pci_range_end_value(p_intra_freq_list_node->intra_freq_value.intra_freq_open_pci_range.pci_range);
			if((p_nr->neighbor_info.pci >=  p_intra_freq_list_node->intra_freq_value.intra_freq_open_pci_range.pci_start) &&
					(p_nr->neighbor_info.pci <= (p_intra_freq_list_node->intra_freq_value.intra_freq_open_pci_range.pci_start + temp_open_pci_end -1)))
			{
				p_nr->neighbor_info.access_mode = SON_OPEN_FEMTO;
				is_macro_nbr = SON_FALSE;
			}
		}

		if(p_intra_freq_list_node->intra_freq_value.closed_pci_range_present == SON_TRUE)
		{
			/*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
			temp_closed_pci_end = son_determine_pci_range_end_value(p_intra_freq_list_node->intra_freq_value.intra_freq_closed_pci_range.pci_range);
			if((p_nr->neighbor_info.pci >=  p_intra_freq_list_node->intra_freq_value.intra_freq_closed_pci_range.pci_start) &&
					(p_nr->neighbor_info.pci <= (p_intra_freq_list_node->intra_freq_value.intra_freq_closed_pci_range.pci_start + temp_closed_pci_end -1)))
			{
				p_nr->neighbor_info.access_mode = SON_CLOSED;
				is_macro_nbr = SON_FALSE;
			}
		}

		if(p_intra_freq_list_node->intra_freq_value.hybrid_pci_range_present == SON_TRUE)
		{
			/*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
			temp_hybrid_pci_end = son_determine_pci_range_end_value(p_intra_freq_list_node->intra_freq_value.intra_freq_hybrid_pci_range.pci_range);
			if((p_nr->neighbor_info.pci >=  p_intra_freq_list_node->intra_freq_value.intra_freq_hybrid_pci_range.pci_start) &&
					(p_nr->neighbor_info.pci <= (p_intra_freq_list_node->intra_freq_value.intra_freq_hybrid_pci_range.pci_start + temp_hybrid_pci_end -1)))
			{
				p_nr->neighbor_info.access_mode = SON_HYBRID;
				is_macro_nbr = SON_FALSE;
			}
		}

		if(is_macro_nbr == SON_TRUE)
		{
			p_nr->neighbor_info.access_mode = SON_OPEN;
		}
		/*SPR-10426-End*/

		p_nr->neighbor_info.bitmask |= SON_ACCESS_MODE_PRESENT;

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_set_correct_access_mode_of_intra_rat_nbr:"
				"access mode of Intra freq nbr set to %d\n",
				p_nr->neighbor_info.access_mode);
	}
	else if(p_inter_freq_list_node != SON_PNULL )
	{
		/*SPR-10426-Start*/
		if(p_inter_freq_list_node->inter_freq_value.open_pci_range_present == SON_TRUE)
		{
			/*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
			temp_open_pci_end = son_determine_pci_range_end_value(p_inter_freq_list_node->inter_freq_value.inter_freq_open_pci_range.pci_range);
			if((p_nr->neighbor_info.pci >=  p_inter_freq_list_node->inter_freq_value.inter_freq_open_pci_range.pci_start) &&
					(p_nr->neighbor_info.pci <= (p_inter_freq_list_node->inter_freq_value.inter_freq_open_pci_range.pci_start + temp_open_pci_end -1)))
			{
				p_nr->neighbor_info.access_mode = SON_OPEN_FEMTO;
				is_macro_nbr = SON_FALSE;
			}
		}

		if(p_inter_freq_list_node->inter_freq_value.closed_pci_range_present == SON_TRUE)
		{
			/*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
			temp_closed_pci_end = son_determine_pci_range_end_value(p_inter_freq_list_node->inter_freq_value.inter_freq_closed_pci_range.pci_range);
			if((p_nr->neighbor_info.pci >=  p_inter_freq_list_node->inter_freq_value.inter_freq_closed_pci_range.pci_start) &&
					(p_nr->neighbor_info.pci <= (p_inter_freq_list_node->inter_freq_value.inter_freq_closed_pci_range.pci_start + temp_closed_pci_end -1)))
			{
				p_nr->neighbor_info.access_mode = SON_CLOSED;
				is_macro_nbr = SON_FALSE;
			}
		}

		if(p_inter_freq_list_node->inter_freq_value.hybrid_pci_range_present == SON_TRUE)
		{
			/*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
			temp_hybrid_pci_end = son_determine_pci_range_end_value(p_inter_freq_list_node->inter_freq_value.inter_freq_hybrid_pci_range.pci_range);
			if((p_nr->neighbor_info.pci >=  p_inter_freq_list_node->inter_freq_value.inter_freq_hybrid_pci_range.pci_start) &&
					(p_nr->neighbor_info.pci <= (p_inter_freq_list_node->inter_freq_value.inter_freq_hybrid_pci_range.pci_start + temp_hybrid_pci_end -1)))
			{
				p_nr->neighbor_info.access_mode = SON_HYBRID;
				is_macro_nbr = SON_FALSE;
			}
		}

		if(is_macro_nbr == SON_TRUE)
		{
			p_nr->neighbor_info.access_mode = SON_OPEN;
		}
		/*SPR-10426-End*/

		p_nr->neighbor_info.bitmask |= SON_ACCESS_MODE_PRESENT;

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_set_correct_access_mode_of_intra_rat_nbr:"
				"access mode of Inter freq nbr set to %d\n",
				p_nr->neighbor_info.access_mode);
	}
	else /*SPR-10426-Start*/
	{
        /*SPR 15799 Fix Start*/
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_DETAILED,
				"anr_set_correct_access_mode_of_intra_rat_nbr:"
                "Nbr's ERAFCN is not avilable in Provided Intra nor Inter Frequency PCI list to determine Accessibility\n");
        /*Lines Deleted*/
        /*SPR 15799 Fix Start*/
	} /*SPR-10426-End*/

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_set_correct_access_mode_of_intra_rat_nbr */

/*SPR-9574-End*/

/******************************************************************************
 * Function Name  : anr_intra_rat_insert_nr
 * Inputs         : p_cell_ctxt        - Pointer to cell context
 *                  p_nr               - Pointer to a structure containing the NR 
 *                  check_remove_list  - Flag to check the remove list
 *                  p_error_code       - Error code
 *                  p_oam_notification - Pointer to notification msg to oam
 *                  p_rrm_notification - Pointer to notification msg to rrm
 * Outputs        : p_error_code -  Pointer to a variable for storing error code
 *
 * Returns        : anr_intra_rat_hash_rec_t* - Pointer to the data field of the                    
 *                                              new NR record inserted.
 * Description    : This function is used to insert an 
 *                  entry in the intra RAT Neighbor Relation Table to store 
 *                  the neighbors of a cell
 ******************************************************************************/
anr_intra_rat_hash_rec_t *
anr_intra_rat_insert_nr
(
 anr_cell_context_t *p_cell_ctxt,
 anr_intra_rat_hash_rec_t *p_nr,
 son_bool_et check_remove_list,
 son_error_et *p_error_code,
 son_anr_updated_nrt_info_t *p_oam_notification_add,
 son_anr_updated_nrt_info_t *p_oam_notifiation_update,
 son_anr_rrm_updated_nrt_info_t *p_rrm_notification,
 son_bool_et         is_conflict_confusion_trigger,
 son_bool_et         is_oam_triggered_insertion
 )
{
	anr_intra_rat_hash_nr_tuple_t 	*p_hash_tpl   = SON_PNULL;
	son_oam_log_on_off_et        	log_mode      = anr_get_log_mode();
	SON_HASH 						*p_hash 	  = SON_PNULL;
	son_global_cell_id_t            cell_id;
	son_error_et    				error_code 	  = SON_NO_ERROR;
	son_x2_status_et                nr_x2_status  = SON_X2_ALLOWED_DISCONNECTED;
	son_global_enb_id_t 			enb_id;
	son_nbr_enb_hash_tuple_t 		*p_enb_ctxt  = SON_PNULL;
	son_enb_type_et                 enb_type     = SON_MACRO_ENB;
	son_bool_et is_rrm_notif_reqd = SON_FALSE;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_nr || SON_PNULL == p_cell_ctxt || SON_PNULL == p_error_code)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_intra_rat_insert_nr: NR could not be added, "
				"p_cell_ctxt or p_nr is NULL\n");

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	SON_MEMSET(&cell_id, 0, sizeof(cell_id));
	anr_convert_intra_rat_nr_to_global_cell_id_struct(&p_nr->neighbor_info, &cell_id);

	if (!SON_MEMCMP(&p_cell_ctxt->cell_id,
				&p_nr->neighbor_info.nbr_cgi,
				sizeof(son_intra_rat_global_cell_id_t)))
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_intra_rat_insert_nr:" 
				"Cell [Cell Id: 0x%x] matches the serving cell\n",
				son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));
		*p_error_code = SON_ERR_INVALID_PARAMS;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if (SON_TRUE == check_remove_list)
	{
		if (SON_PNULL != anr_find_cell_in_remove_list(
					&p_cell_ctxt->nr_remove_list,
					&cell_id))
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_intra_rat_insert_nr:"
					"Cell [Cell Id: 0x%x]is present in remove list \n",
					son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));
			*p_error_code = SON_ERR_ANR_NR_PRESENT_IN_REMOVE_LIST;
			SON_UT_TRACE_EXIT();
			return SON_PNULL;
		}
	}

	if (!(p_nr->neighbor_info.bitmask & SON_PCI_PRESENT) ||
			!(p_nr->neighbor_info.bitmask & SON_EARFCN_PRESENT) ||
			!(p_nr->neighbor_info.bitmask & SON_ENABLE_HO_STATUS_PRESENT) ||
			!(p_nr->neighbor_info.bitmask & SON_ENABLE_NR_STATUS_PRESENT) || 
			(p_nr->neighbor_info.bitmask & SON_SUSPECT_PCI_PRESENT) || 
			(p_nr->neighbor_info.bitmask & SON_EUTRAN_NR_HO_STATS_PRESENT) ||
            (p_nr->neighbor_info.bitmask & SON_ENABLE_X2_STATUS_PRESENT)||
            /*SPR 15718 Fix Start*/
            !(p_nr->neighbor_info.bitmask & SON_TAC_PRESENT))
            /*SPR 15718 Fix Stop*/
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_intra_rat_insert_nr: NR [Cell Id: 0x%x] could not be added, "
				"as mandatory fields(pci or earfcn or ho_status or nr_status) "
                "are not present or read only parameters are present(suspect_pci, ho_stats,x2_status)"
            /*SPR 15718 Fix Start*/
                " or tac is absent \n"
				"are not present or read only parameters are present (suspect_pci, ho_stats,x2_status)\n",
				son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));
		*p_error_code = SON_ERR_INVALID_PARAMS;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	/*SPR-10230-Start*/

	if((is_oam_triggered_insertion == SON_TRUE) && !(p_nr->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT))
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_intra_rat_insert_nr: NR [Cell Id: 0x%x] could not be added, "
				"as insertion of this nbr has been triggered from OAM"
				"but access_mode value has not been provided\n",
				son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));
		*p_error_code = SON_ERR_INVALID_PARAMS;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	/*SPR-10230-End*/


	p_hash = &p_cell_ctxt->intra_rat_neighbor_table;

	/*Coverity fix*/
	if (SON_PNULL != hash_find(p_hash, (son_void_t *)p_nr))
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_intra_rat_insert_nr: NR [Cell Id: 0x%x] Already Present."
				" Duplicate Entry\n", son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.
					cell_identity));

		*p_error_code = SON_ERR_ANR_DUPLICATE_NR;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	/*If max size is already crossed*/ 
	if (SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS <=  hash_count(p_hash))
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_intra_rat_insert_nr: NR [Cell Id: 0x%x] can not"
				"be inserted as number of Intra rat neighbors currently present in NRT is [%d]\n\n",
				son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity), hash_count(p_hash));

		*p_error_code = SON_ERR_ANR_MAX_NRT_SIZE_REACHED;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	/*SPR-10230-Start*/

	if((is_oam_triggered_insertion == SON_TRUE)||
			((p_nr->enb_id.enb_type == SON_MACRO_ENB)&&(p_nr->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT)&&
			 (p_nr->neighbor_info.access_mode == SON_OPEN)))
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_intra_rat_insert_nr:For NR [Cell Id: 0x%x]"
				"access mode is not going to be calculated on pci range basis"
				"as either it is being inserted by OAM or has come in link up indication"
				"and belongs to a macro eNB\n",
				son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));
	}
	else  /*SPR-10230-End*/
	{
		/*SPR-9574-Start*/
		if(SON_FALSE ==  anr_set_correct_access_mode_of_intra_rat_nbr(p_nr)) /*SPR-10426-Start-If clause added*/
		{
			SON_LOG(log_mode, p_son_anr_facility_name,
					SON_ERROR,
					"NR [Cell Id: 0x%x]"
					"cannot be inserted\n",
					son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));

			*p_error_code = SON_ERR_INVALID_PARAMS;
			SON_UT_TRACE_EXIT();
			return SON_PNULL;
		} /*SPR-10426-End*/
		/*SPR-9574-End*/
	}

	SON_MEMSET(&enb_id, 0, sizeof(enb_id));
	anr_determine_enb_id(p_nr,
			&enb_id);

	/* SPR 10103 FIX START */
	SON_MEMCPY(&p_nr->enb_id, &enb_id, sizeof(son_global_enb_id_t));
	/* SPR 10103 FIX END */

	/* Rel 3.0 Multi Sector code changes Start */
     /*SPR 17773 Start*/
    if(SON_PNULL 
        != list_find_node(&g_anr_context.cell_ctxt_list,
                                      &p_nr->neighbor_info.nbr_cgi,
                                      son_key_of_anr,son_compare_cell_id))
     /*SPR 17773 End*/
	{
		nr_x2_status = SON_LOCALLY_CONNECTED_CELL;
	}
	else
	{
		/* Rel 3.0 Multi Sector code changes Stop */
		p_enb_ctxt   =  x2_find_nbr_enb(x2_get_enb_nrt(),
				&enb_id,
				&error_code);

		if (SON_PNULL != p_enb_ctxt)
		{
			nr_x2_status = get_x2_sts_value_for_nbr_from_enb_rec(&p_enb_ctxt->data);

			enb_type     = p_enb_ctxt->data.enb_id.enb_type;

			if ((SON_HOME_ENB == enb_type &&
						(p_nr->neighbor_info.access_mode == SON_OPEN))) /*Condition modified for SPR-10230*/
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_ERROR, "anr_intra_rat_insert_nr: NR [Cell Id: 0x%x]"
						" could not be added, As the eNB is HOME eNB and cell is of MACRO eNB\n",
						son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));   
				*p_error_code = SON_ERR_INVALID_PARAMS;
				SON_UT_TRACE_EXIT();
				return SON_PNULL;
			}

			if ((SON_MACRO_ENB == enb_type &&
						(p_nr->neighbor_info.access_mode != SON_OPEN))) /*Condition modified for SPR-10230*/
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_ERROR, "anr_intra_rat_insert_nr: NR [Cell Id: 0x%x]"
						" could not be added, As the eNB is MACRO eNB and cell is of HOME eNB\n",
						son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));   
				*p_error_code = SON_ERR_INVALID_PARAMS;
				SON_UT_TRACE_EXIT();
				return SON_PNULL;
			}

			/* Bug 4214 start */

			if(p_nr->neighbor_info.bitmask & SON_TAC_PRESENT &&
					SON_MEMCMP(p_nr->neighbor_info.tac,&p_enb_ctxt->data.tac_list[0], sizeof(p_nr->neighbor_info.tac)))
			{
				SON_MEMCPY(&p_enb_ctxt->data.tac_list[0], p_nr->neighbor_info.tac, sizeof(p_nr->neighbor_info.tac));
				SON_LOG(log_mode, p_son_anr_facility_name,SON_DETAILED, "TAC value updated in Neighbor enb context \n");
			}
			else
			{
				SON_LOG(log_mode, p_son_anr_facility_name,SON_WARNING, 
						"TAC value is not updated. Either SON_TAC_PRESENT is not present in nr_info or TAC in enb and nr_cell record are same \n");
			}
		}
		/* Bug 4214 end */
		/* Rel 3.0 Multi Sector code changes Start */
	}
	/* Rel 3.0 Multi Sector code changes Stop */

	/*Inserting the x2 status*/
	p_nr->neighbor_info.bitmask |= SON_ENABLE_X2_STATUS_PRESENT; 
	p_nr->neighbor_info.x2_status = nr_x2_status;

	if (SON_FALSE ==  anr_validate_csg_id_access_mode_for_eutran_neighbor(p_nr))
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR, "anr_intra_rat_insert_nr: NR [Cell Id: 0x%x]"
				" could not be added\n",
				son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));

		*p_error_code = SON_ERR_INVALID_PARAMS;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

    /*SPR 16372 fix start*/
    if((SON_ACCESS_MODE_PRESENT & p_nr->neighbor_info.bitmask) &&
            ((SON_HYBRID == p_nr->neighbor_info.access_mode) || (SON_CLOSED == p_nr->neighbor_info.access_mode))  &&
            (!(SON_INTRA_CSG_ID_PRESENT & p_nr->neighbor_info.bitmask))
      )
    {
        SON_LOG(log_mode, p_son_anr_facility_name,
                SON_ERROR, "anr_intra_rat_insert_nr: NR [Cell Id: 0x%x]"
                " could not be added as access mode is HYBRID/CLOSED and CSGID is not present\n",
                son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));

        *p_error_code = SON_ERR_INVALID_PARAMS;
        SON_UT_TRACE_EXIT();
        return SON_PNULL;
    }
    /*SPR 16372 fix stop*/

	if (!(SON_BROADCAST_STATUS_PRESENT  & p_nr->neighbor_info.bitmask))
	{
		p_nr->neighbor_info.bitmask |= SON_BROADCAST_STATUS_PRESENT;
		p_nr->neighbor_info.broadcast_status = SON_TRUE;
	}

	if (!(p_nr->neighbor_info.bitmask & SON_CELL_OFFSET_PRESENT))
	{
		p_nr->neighbor_info.bitmask |= SON_CELL_OFFSET_PRESENT;
		p_nr->neighbor_info.cell_specific_offset = SON_Q_OFFSET_RANGE_DB0;
	}

	if (!(p_nr->neighbor_info.bitmask & SON_CM_CELL_OFFSET_PRESENT))
	{
		p_nr->neighbor_info.bitmask |= SON_CM_CELL_OFFSET_PRESENT;
		p_nr->neighbor_info.cm_cell_specific_offset = SON_Q_OFFSET_RANGE_DB0;
	}



	/*This funcition determines if the neighbor can be inserted or not and
	 * if yes, then will it be send to rrm of not*/

	is_rrm_notif_reqd = anr_vaildate_insertion_of_nr(p_cell_ctxt,
			&p_nr->neighbor_info, p_error_code);

	if (SON_NO_ERROR != *p_error_code)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR, "anr_intra_rat_insert_nr: NR [Cell Id: 0x%x]"
				" could not be inserted\n",
				son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));   

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	ANR_GET_HASH_REC(anr_get_intra_rat_hash_rec_pool(), p_hash_tpl);

	if (SON_PNULL == p_hash_tpl)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"NR [Cell Id: 0x%x]" 
				" could not be added, memory allocation failed\n",
				son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));

		SON_UT_TRACE_EXIT();
		*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;
		return SON_PNULL;
	}

	if (!(p_nr->neighbor_info.bitmask & SON_ENABLE_X2_STATUS_PRESENT))
	{
		p_nr->neighbor_info.bitmask |= SON_ENABLE_X2_STATUS_PRESENT; 
		p_nr->neighbor_info.x2_status = nr_x2_status;
	}

	SON_MEMCPY(&p_hash_tpl->data, p_nr, sizeof(anr_intra_rat_hash_rec_t));
	list_init(&p_hash_tpl->data.anr_nr_nbr_list);

	SON_LOG(log_mode, p_son_anr_facility_name,
			SON_BRIEF,
			"NR [Cell id :0x%x]"
			" received is added to the NR hash table\n",
			son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));

	*p_error_code = SON_NO_ERROR;
	hash_insert(p_hash, &p_hash_tpl->anchor);

	/*Setting the HO stats*/
	p_hash_tpl->data.neighbor_info.bitmask |= SON_EUTRAN_NR_HO_STATS_PRESENT; 
	SON_MEMSET(&p_hash_tpl->data.neighbor_info.handover_stats, 0,
			sizeof(son_neighbor_handover_stats_t));

	/*Setting up suspected PCIs*/
	p_hash_tpl->data.neighbor_info.bitmask |= SON_SUSPECT_PCI_PRESENT;
	p_hash_tpl->data.neighbor_info.suspected_pci = SON_FALSE;

	/* Init MRO parameters */
	p_hash_tpl->data.mro_data.is_cio_algo_running = SON_FALSE;
	p_hash_tpl->data.mro_data.retry_count = 0;
	p_hash_tpl->data.mro_data.prev_cio = SON_Q_OFFSET_RANGE_DB0;
	p_hash_tpl->data.mro_data.prev_ho_fail_rate = 0.0;
	p_hash_tpl->data.mro_data.optimizaton_for = SON_NO_OPTIMIZATION_DIR;
	p_hash_tpl->data.mro_data.in_use_by_mlb = SON_FALSE;
	p_hash_tpl->data.mro_data.mro_ho_counter.num_too_early_ho = 0;
	p_hash_tpl->data.mro_data.mro_ho_counter.num_too_late_ho = 0;
	p_hash_tpl->data.mro_data.mro_ho_counter.
		num_ho_to_wrong_cell_for_failure_cell = 0;
	p_hash_tpl->data.mro_data.mro_ho_counter.
		num_ho_to_wrong_cell_for_reestablishment_cell = 0;
	p_hash_tpl->data.mro_data.mro_ho_counter.total_ho_attempted = 0;

	if (SON_FALSE == check_remove_list)
	{
		/* If the neighbor is present in Remove List, remove neighbor from REMOVE LIST */
		if (SON_TRUE == anr_delete_cell_from_remove_list(
					&p_cell_ctxt->nr_remove_list, &cell_id))
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"NR [Cell Id: 0x%x] Removed"
					" from Remove List.",
					son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));
		}
	}

	son_time(&p_hash_tpl->data.last_activity_ts);

	if (p_rrm_notification && SON_TRUE == is_rrm_notif_reqd)
	{
		anr_bs_allowed_node_t *p_bs_allowed_node = SON_PNULL;
		son_bool_et is_ho_allowed_reqd = SON_FALSE;
		son_bool_et is_ho_blk_reqd     = SON_FALSE;

		p_bs_allowed_node = anr_find_record_in_bs_allowed_stats_list(
				&p_cell_ctxt->bs_allowed_stats_list,
				p_hash_tpl->data.neighbor_info.earfcn);

		son_ho_status_et ho_stat  = 
			anr_get_ho_status_of_eutran_nr(&p_hash_tpl->data.neighbor_info);
		if (ho_stat == SON_HO_ALLOWED)
		{
			is_ho_allowed_reqd = SON_TRUE;
		}
		else
		{
			is_ho_blk_reqd = SON_TRUE;
		}

		if (p_bs_allowed_node == SON_PNULL)
		{
			/*Assuming that the insertion will never fail due to
			 * memory failure.If it does we need to delete the inserted NR from
			 * the NRT.*/
			anr_insert_record_in_bs_allowed_stats_list(
					&p_cell_ctxt->bs_allowed_stats_list,
					p_hash_tpl->data.neighbor_info.earfcn,
					p_hash_tpl->data.neighbor_info.broadcast_status,
					is_ho_allowed_reqd, is_ho_blk_reqd);
		}
		else
		{
			/* Coverity Fix: 23481 */
			if (SON_FALSE == increment_bs_allowed_nbr_count(p_cell_ctxt,
						p_bs_allowed_node,
						p_hash_tpl->data.neighbor_info.broadcast_status,
						is_ho_allowed_reqd, is_ho_blk_reqd))
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
						"Increment of bs allowed nbr count is "
						"failed for Cell Identity [Cell id:0x%x] \n\n",
						son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
			}
		}

		anr_construct_rrm_intra_rat_ncl(&p_rrm_notification->intra_rat_ncl,
				&p_hash_tpl->data,
				SON_FALSE);
	}

	/*SPR_5193*/
	if (p_oam_notification_add)
	{
		p_oam_notification_add->nrt_op = SON_NRT_OP_ADD;
		anr_construct_oam_updated_ncl(&p_oam_notification_add->nr_list,
				&p_hash_tpl->data.neighbor_info,
				SON_FALSE);
	}

	if (SON_TRUE == is_conflict_confusion_trigger)
	{
		insert_pci_suspect_conflict_confusion_handler(p_cell_ctxt, &p_hash_tpl->data,
				p_oam_notifiation_update);
	}

	SON_UT_TRACE_EXIT();
	return &p_hash_tpl->data;
} /* anr_intra_rat_insert_nr */

/******************************************************************************
 * Function Name  : anr_convert_intra_rat_nr_to_intra_rat_hash_rec
 * Inputs         : p_nr        -  Pointer to a structure containing the NR
 *                                 information
 *                  p_hash_rec  - Pointer to hash record
 * Outputs        : anr_intar_rat_hash_rec_t* - Pointer to the data field of
 *                  the new NR record to be inserted.
 *
 * Returns        : son_void_t  - None.
 * Description    : This function is used to populate node data of inter RAT NR
 *                  hash table using the given neighbor info.
 ******************************************************************************/
son_void_t
anr_convert_intra_rat_nr_to_intra_rat_hash_rec
(
 const son_intra_rat_neighbor_info_t *p_nr,
 anr_intra_rat_hash_rec_t *p_hash_rec
 )
{
	son_oam_log_on_off_et         log_mode     = anr_get_log_mode();


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_nr || SON_PNULL == p_hash_rec)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_convert_intra_rat_nr_to_intra_rat_hash_rec: Could not "
				"populate the intra RAT record, p_nr or p_hash_rec is NULL\n");
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMSET(p_hash_rec, 0, sizeof(anr_intra_rat_hash_rec_t));

	SON_MEMCPY(&p_hash_rec->neighbor_info,
			p_nr,
			sizeof(son_intra_rat_neighbor_info_t));

	SON_UT_TRACE_EXIT();
} /* anr_convert_intra_rat_nr_to_intra_rat_hash_rec */

/*****************************************************************************
 * Function Name  : anr_convert_intra_rat_hash_rec_to_intra_rat_nr
 * Inputs         : p_intra_rat_hash_rec - Constant Pointer to inter_rat_hash_rec structure
 * Outputs        : p_intra_rat_nr       - Pointer to inter rat nr structure
 * Returns        : son_void_t           - None
 * Description    : This function converts Inter Rat Hash Record to Intra Rat Neighbor
 ******************************************************************************/
son_void_t
anr_convert_intra_rat_hash_rec_to_intra_rat_nr
(
 const anr_intra_rat_hash_rec_t  *p_intra_rat_hash_rec,
 son_intra_rat_neighbor_info_t   *p_intra_rat_nr
 )
{
	son_oam_log_on_off_et         log_mode     = anr_get_log_mode();


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_intra_rat_hash_rec || SON_PNULL == p_intra_rat_nr)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_convert_intra_rat_hash_rec_to_intra_rat_nr: Could not "
				"populate the intra RAT NR, p_intra_rat_hash_rec or "
				"p_intra_rat_nr is NULL\n");
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMCPY(p_intra_rat_nr,
			&p_intra_rat_hash_rec->neighbor_info,
			sizeof(son_intra_rat_neighbor_info_t));
	SON_UT_TRACE_EXIT();
} /* anr_convert_intra_rat_hash_rec_to_intra_rat_nr */

/******************************************************************************
 * Function Name  : anr_convert_inter_rat_nr_to_inter_rat_hash_rec
 * Inputs         : p_nr   -  Pointer to a structure containing the NR 
 *                            information
 *                  p_hash_rec  - Pointer to hash record
 * Outputs        : anr_inter_rat_hash_rec_t* - Pointer to the data field of
 *                  the new NR record to be inserted.
 *
 * Returns        : son_void_t - None.
 * Description    : This function is used to populate node data of inter RAT NR
 *                  hash table using the given neighbor info.
 ******************************************************************************/
son_void_t
anr_convert_inter_rat_nr_to_inter_rat_hash_rec
(
 const son_inter_rat_neighbor_info_t *p_nr, 
 anr_inter_rat_hash_rec_t *p_hash_rec
 )
{
	son_oam_log_on_off_et         log_mode     = anr_get_log_mode();


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_nr || SON_PNULL == p_hash_rec)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_convert_inter_rat_nr_to_inter_rat_hash_rec: Could not "
				"populate the inter RAT record, p_nr or p_hash_rec is NULL\n");
		SON_UT_TRACE_EXIT();
		return;
	}

	if (SON_INTER_UTRAN_NEIGHBOR & p_nr->bitmask)
	{
		p_hash_rec->t = ANR_UTRAN_NEIGHBOR;
		SON_MEMCPY(&p_hash_rec->u.utran_neighbor_info,
				&p_nr->utran_neighbor,
				sizeof(son_utran_neighbor_info_t));
	}
	else if (SON_INTER_GERAN_NEIGHBOR & p_nr->bitmask)
	{
		p_hash_rec->t = ANR_GERAN_NEIGHBOR;
		SON_MEMCPY(&p_hash_rec->u.geran_neighbor_info,
				&p_nr->geran_neighbor, sizeof(son_geran_neighbor_info_t));
	}
	else if (SON_INTER_CDMA_NEIGHBOR & p_nr->bitmask)
	{
		p_hash_rec->t = ANR_CDMA_NEIGHBOR;
		SON_MEMCPY(&p_hash_rec->u.cdma_neighbor_info,
				&p_nr->cdma_neighbor, sizeof(son_cdma_neighbor_info_t));
	}
	else 
	{
		/* Poplate the hash rec as per other RAT type in later releases */
	}

	SON_UT_TRACE_EXIT();
} /* anr_convert_inter_rat_nr_to_inter_rat_hash_rec */

/*****************************************************************************
 * Function Name  : anr_convert_inter_rat_hash_rec_to_inter_rat_nr
 * Inputs         : p_inter_rat_hash_rec - Constant Pointer to inter_rat_hash_rec structure
 * Outputs        : p_inter_rat_nr       - Pointer to inter rat nr structure
 * Returns        : son_void_t           -
 * Description    : This function converts Inter Rat Hash Record to Inter Rat Neighbor
 ******************************************************************************/
son_void_t
anr_convert_inter_rat_hash_rec_to_inter_rat_nr
(
 const anr_inter_rat_hash_rec_t  *p_inter_rat_hash_rec,
 son_inter_rat_neighbor_info_t   *p_inter_rat_nr
 )
{
	son_void_t *p_nr_from          = SON_PNULL;
	son_void_t *p_nr_to            = SON_PNULL;
	son_u16    length              = 0;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_inter_rat_hash_rec)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_inter_rat_hash_rec is NULL");
		SON_UT_TRACE_EXIT();
		return;
	}

	switch (p_inter_rat_hash_rec->t)
	{
		case ANR_UTRAN_NEIGHBOR:
			{
				p_inter_rat_nr->bitmask = SON_INTER_UTRAN_NEIGHBOR;
				length = sizeof(son_utran_neighbor_info_t);
				p_nr_from  = (son_void_t *)&(p_inter_rat_hash_rec->u.
						utran_neighbor_info);
				p_nr_to    = (son_void_t *)&(p_inter_rat_nr->utran_neighbor);
				break;
			}

		case ANR_GERAN_NEIGHBOR:
			{
				p_inter_rat_nr->bitmask = SON_INTER_GERAN_NEIGHBOR;
				length = sizeof(son_geran_neighbor_info_t);
				p_nr_from  = (son_void_t *)&(p_inter_rat_hash_rec->u.
						geran_neighbor_info);
				p_nr_to    = (son_void_t *)&(p_inter_rat_nr->geran_neighbor);
				break;
			}

		case ANR_CDMA_NEIGHBOR:
			{
				p_inter_rat_nr->bitmask = SON_INTER_CDMA_NEIGHBOR;
				length = sizeof(son_cdma_neighbor_info_t);
				p_nr_from  = (son_void_t *)&(p_inter_rat_hash_rec->u.
						cdma_neighbor_info);
				p_nr_to    = (son_void_t *)&(p_inter_rat_nr->cdma_neighbor);
				break;
			}

		default:
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR,
						"anr_convert_inter_rat_hash_rec_to_inter_rat_nr: Invalid "
						"Rat Type: %d\n",
						p_inter_rat_hash_rec->t);
				break;
			}
	} /* switch */

	/*Coverity Fixes, adding of If condition*/
	if (SON_PNULL != p_nr_to && SON_PNULL != p_nr_from)
	{
		SON_MEMCPY(p_nr_to, p_nr_from, length);
	}

	SON_UT_TRACE_EXIT();
} /* anr_convert_inter_rat_hash_rec_to_inter_rat_nr */

/*****************************************************************************
 * Function Name  : anr_extract_global_cell_id_from_inter_rat_hash_rec
 * Inputs         : p_inter_rat_hash_rec - Pointer to inter_rat hash record
 * Outputs        : p_global_cell_id     - Pointet to global cell id
 * Returns        : son_void_t           - None
 * Description    : This function populates the global cell id from inter_rat 
 *                  hash record
 ******************************************************************************/
son_void_t
anr_extract_global_cell_id_from_inter_rat_hash_rec
(
 const  anr_inter_rat_hash_rec_t *p_inter_rat_hash_rec,
 son_global_cell_id_t *p_global_cell_id
 )
{
	son_void_t *p_from          = SON_PNULL;
	son_void_t *p_to            = SON_PNULL;
	son_u16    length              = 0;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_inter_rat_hash_rec)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_inter_rat_hash_rec is NULL");
		SON_UT_TRACE_EXIT();
	}
	else
	{
		switch (p_inter_rat_hash_rec->t)
		{
			case ANR_UTRAN_NEIGHBOR:
				{
					p_global_cell_id->bitmask = SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT;
					p_global_cell_id->inter_rat_global_cell_id.bitmask =
						SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT;

					length = sizeof(son_utran_geran_global_cell_id_t);
					p_from  = (son_void_t *)&(p_inter_rat_hash_rec->u.
							utran_neighbor_info.nbr_cgi);
					p_to    = (son_void_t *)&(p_global_cell_id->
							inter_rat_global_cell_id.utran_geran_global_cell_id);
					break;
				}

			case ANR_GERAN_NEIGHBOR:
				{
					p_global_cell_id->bitmask = SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT;

					p_global_cell_id->inter_rat_global_cell_id.bitmask =
						SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT;

					length = sizeof(son_utran_geran_global_cell_id_t);
					p_from  = (son_void_t *)&(p_inter_rat_hash_rec->u.
							geran_neighbor_info.nbr_cgi);

					p_to = (son_void_t *)&(p_global_cell_id->
							inter_rat_global_cell_id.utran_geran_global_cell_id);
					break;
				}

			case ANR_CDMA_NEIGHBOR:
				{
					p_global_cell_id->bitmask = SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT;
					p_global_cell_id->inter_rat_global_cell_id.bitmask = 
						SON_CDMA_GLOBAL_CELL_ID_PRESENT;

					length = sizeof(son_cdma_global_cell_id_t);
					p_from  = (son_void_t *)&(p_inter_rat_hash_rec->u.
							cdma_neighbor_info.nbr_cgi);
					p_to = (son_void_t *)&(p_global_cell_id->
							inter_rat_global_cell_id.cdma_global_cell_id); 
					break;
				}

			default:
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_ERROR,
							"anr_extract_global_cell_id_from_inter_rat_hash_rec: "
							"Invalid Rat Type: %d\n",
							p_inter_rat_hash_rec->t);
					break;
				}
		} /* switch */
	}

	/*Coverity fix, adding the if check */
	if (SON_PNULL != p_to && SON_PNULL != p_from)
	{
		SON_MEMCPY(p_to, p_from, length);
	}

	SON_UT_TRACE_EXIT();
} /* anr_extract_global_cell_id_from_inter_rat_hash_rec */

/******************************************************************************
 * Function Name  : anr_validate_rec_data
 * Inputs         : p_hash_rec - Pointer to a the NR record to be validated
 * Outputs        : None.
 * Returns        : son_bool_et - SON_TRUE If record is valid
 *                                SON_FALSE If record is invalid
 * Description    : This function is used to validate the inter RAT record 
 *                  against the mendatory fields values such as NR_STATUS, 
 *                  HO_STATUS etc. to be inserted in inter RAT NR hash table.
 ******************************************************************************/
static son_bool_et
anr_validate_rec_data
(
 const anr_inter_rat_hash_rec_t      *p_hash_rec
 )
{
	son_bool_et is_valid = SON_TRUE;
	son_oam_log_on_off_et log_mode = anr_get_log_mode();
	son_u32  cell_identity  = 0;
	son_u32  bitmask  = 0;
	son_bool_et is_cdma_xrtt_present = SON_FALSE;
	son_bool_et is_cdma_hrpd_present = SON_FALSE;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_hash_rec)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_validate_rec_data: NR could not be added, "
				"p_hash_rec is NULL\n");
		is_valid = SON_FALSE;
	}

	else if (ANR_UTRAN_NEIGHBOR == p_hash_rec->t)
	{
		son_u32 is_data_valid = SON_TRUE;
		bitmask = SON_UTRAN_PCI_PRESENT | SON_UTRAN_UARFCN_PRESENT |
			SON_UTRAN_ENABLE_NR_STATUS_PRESENT | SON_UTRAN_ENABLE_HO_STATUS_PRESENT;

		if (((p_hash_rec->u.utran_neighbor_info.bitmask & bitmask) != bitmask) ||
				(p_hash_rec->u.utran_neighbor_info.bitmask & SON_UTRAN_NR_HO_STATS_PRESENT) ||
				!(p_hash_rec->u.utran_neighbor_info.pci.bitmask == SON_PCI_FDD_PRESENT ||
					p_hash_rec->u.utran_neighbor_info.pci.bitmask == SON_PCI_TDD_PRESENT))
		{
			is_data_valid = SON_FALSE;
		}
		else if (SON_FALSE == anr_validate_csg_id_access_mode_for_utran_neighbor(p_hash_rec))
		{
			is_data_valid = SON_FALSE;   
		}

		if (SON_FALSE == is_data_valid)
		{
			cell_identity = son_convert_const_char_cell_id_to_int_cell_id(
					p_hash_rec->u.utran_neighbor_info.nbr_cgi.cell_identity);

			is_valid = SON_FALSE;
		}
	}
	else if (ANR_GERAN_NEIGHBOR == p_hash_rec->t)
	{
		bitmask = SON_GERAN_PCI_PRESENT | SON_GERAN_ARFCN_BAND_IND_PRESENT |
			SON_GERAN_ENABLE_NR_STATUS_PRESENT | SON_GERAN_ENABLE_HO_STATUS_PRESENT;

		if (((p_hash_rec->u.geran_neighbor_info.bitmask & bitmask) != bitmask) ||
				(p_hash_rec->u.geran_neighbor_info.bitmask & SON_GERAN_NR_HO_STATS_PRESENT))
		{
			cell_identity = son_convert_const_char_cell_id_to_int_cell_id(
					p_hash_rec->u.geran_neighbor_info.nbr_cgi.cell_identity);
			is_valid = SON_FALSE;
		}
	}
	else if (ANR_CDMA_NEIGHBOR == p_hash_rec->t)
	{
		bitmask = SON_CDMA_PCI_PRESENT | SON_CDMA_BAND_CLASS_ARFCN_PRESENT |
			SON_CDMA_ENABLE_NR_STATUS_PRESENT | SON_CDMA_ENABLE_HO_STATUS_PRESENT |
			SON_CDMA_BAND_CLASS_PRESENT;

		if (((p_hash_rec->u.cdma_neighbor_info.bitmask & bitmask) != bitmask) ||
				(p_hash_rec->u.cdma_neighbor_info.bitmask & SON_CDMA_NR_HO_STATS_PRESENT))
		{
			is_valid = SON_FALSE;
		}

		/* Validating the cell identity */
		if (p_hash_rec->u.cdma_neighbor_info.nbr_cgi.bitmask &
				SON_CGI_1X_RTT_PRESENT)
		{
			is_cdma_xrtt_present = SON_TRUE;
		}

		if (p_hash_rec->u.cdma_neighbor_info.nbr_cgi.bitmask &
				SON_CGI_HRPD_PRESENT)
		{
			is_cdma_hrpd_present = SON_TRUE;
		}

		if ((is_cdma_xrtt_present && is_cdma_hrpd_present) ||
				(SON_FALSE == is_cdma_xrtt_present && 
				 SON_FALSE == is_cdma_hrpd_present))
		{
			is_valid = SON_FALSE;
		}
	}
	else
	{
		/* Populate the hash_data as per other RAT type in next release */
	}

	/* Coverity Fix: 30287 */
	if ((SON_PNULL != p_hash_rec) && (SON_FALSE == is_valid))
	{ 
		if (ANR_CDMA_NEIGHBOR != p_hash_rec->t) 
		{
			SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
					"anr_validate_rec: NR [Cell Id: 0x%x] could not "
					"be added, as mandatory fields(PCI or ARFCN or HO STATUS or NR STATUS) "
					"are not present or both UTRAN FDD and UTRAN TDD PCI are specified\n "
					"or there is CSG id ,access mode mismatch\n", cell_identity);
		}
		else 
		{
			SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
					"anr_validate_rec: CDMA NR could not be added, "
					"as mandatory fields(PCI or ARFCN or BAND CLASS or HO STATUS or NR STATUS) "
					"are not present OR Read only params HANDOVER STATS are present OR "
					"Both CDMA type are present\n");
		}
	}

	SON_UT_TRACE_EXIT();
	return is_valid;
} /* anr_validate_rec_data */

/******************************************************************************
 * Function Name  : anr_determine_cdma_changes_in_nrt_when_old_nr_not_send
 * Inputs         : p_cell_ctxt - Pointer to cell context
 *                  p_old_hash      - Pointer to old NBR hash record
 *                  p_new_hash      - Pointer to new NBR hash record
 *                  old_ho_status   - HO status for old NBR
 *                  new_ho_status   - HO status for new NBR
 *                  p_is_old_nr_send- flag pointer to check if old NBR is sendable
 *                                    to RRM
 *                  p_is_rrm_notif_reqd_for_new_nr- Status flag to check if new
 *                                    NBR if to be notified to RRM
 *                  p_error_code- Pointer to error code
 * Outputs        : None.
 *
 * Returns        : son_void_t  - NONE
 * Description    : This function is used to verify the new CDMA NBRs status to be notified
 *                  to RRM when old NBR was not send to RRM
 ******************************************************************************/
son_void_t
anr_determine_cdma_changes_in_nrt_when_old_nr_not_send
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_inter_rat_hash_rec_t  *p_old_hash,
 anr_inter_rat_hash_rec_t  *p_new_hash,
                        /*SPR 17777 +-*/
 son_bool_et               *p_is_old_nr_send,
 son_bool_et               *p_is_rrm_notif_reqd_for_new_nr,
 son_error_et              *p_error_code
 )
{
	/* Coverity CID-55067 Fix Starts */
	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_old_hash || SON_PNULL == p_new_hash)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_old_hash or p_new_hash or both is NULL");
		SON_UT_TRACE_EXIT();
	}
	else
	{
		son_bool_et is_roll_back_allowed = SON_FALSE;

		if (SON_FALSE == *p_is_rrm_notif_reqd_for_new_nr)
		{
			*p_error_code = SON_NO_ERROR;

			*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

			*p_is_old_nr_send = SON_FALSE;
		}
		else
		{
			if (SON_CGI_1X_RTT_PRESENT &
					p_new_hash->u.cdma_neighbor_info.nbr_cgi.bitmask) 
			{
				anr_cdma_xrtt_band_stats_node_t *p_band_list_node = SON_PNULL;
				anr_cdma_xrtt_freq_stats_node_t *p_freq_list_node = SON_PNULL;
				anr_cdma_xrtt_band_stats_node_t *p_list_node_tmp = SON_PNULL;

				/* Performing band level search */
				p_band_list_node = anr_find_record_in_cdma_xrtt_band_stats_list(
						&p_cell_ctxt->cdma_xrtt_band_stats_list,
						p_new_hash->u.cdma_neighbor_info.band_class);

				/* Performing freq level search */
				if (p_band_list_node != SON_PNULL)
				{
					p_freq_list_node = anr_find_record_in_cdma_xrtt_freq_stats_list(
							&p_band_list_node->cdma_band_stats.freq_list,
							p_new_hash->u.cdma_neighbor_info.arfcn);
				}

				if ((SON_PNULL == p_band_list_node) &&
						((SON_RRM_MAX_CDMA_BAND_ALLOWED) <=
						 (list_count(&p_cell_ctxt->cdma_xrtt_band_stats_list))))
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_ERROR,
							"anr_determine_cdma_changes_in_nrt_when_old_nr_not_send: "
							"CDMA NR cannot be updated as maximum band class sendable "
							"to RRM is already reached\n");

					*p_error_code = SON_ERR_ANR_MAX_ALLOWED_BANDS_NBRS_TO_RRM_REACHED;
					*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;
					*p_is_old_nr_send = SON_FALSE;

					SON_UT_TRACE_EXIT();
					return;
				}
				else if (((SON_PNULL != p_band_list_node) &&
							(SON_PNULL == p_freq_list_node)) &&
						((SON_RRM_MAX_CDMA_FREQ_ALLOWED) <=
						 (list_count(&p_band_list_node->cdma_band_stats.freq_list))))
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_ERROR,
							"anr_determine_cdma_changes_in_nrt_when_old_nr_not_send: "
							"CDMA NR cannot be updated as maximum frequency sendable "
							"to RRM is already reached\n");

					*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;
					*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;
					*p_is_old_nr_send = SON_FALSE;

					SON_UT_TRACE_EXIT();
					return;
				}
				else
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_DETAILED,
							"CDMA NR can be updated and is sendable to RRM\n"); 
				}

				/* If the band_class/frequency is modified and it is not in the list */
				if ((SON_PNULL == p_band_list_node) ||
						((SON_PNULL != p_band_list_node) && 
						 (SON_PNULL == p_freq_list_node)))
				{
					/* Inserting new band_class/frequency in the list */
					p_list_node_tmp =  anr_insert_record_in_cdma_xrtt_band_stats_list( 
							p_cell_ctxt,
							&p_new_hash->u.cdma_neighbor_info);

					if (SON_PNULL == p_list_node_tmp)
					{
						SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
								SON_ERROR,
								"anr_determine_cdma_changes_in_nrt_when_old_nr_not_send: "
								"CDMA NR cannot be updated because of memory allocation failure\n");

						is_roll_back_allowed = SON_TRUE;
						*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;
					}
				}

				/* If the band class & frequency exists then just increment the count */
				else if (SON_FALSE == increment_cdma_xrtt_nbr_count_for_rrm(
							&p_new_hash->u.cdma_neighbor_info,
							p_cell_ctxt))
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_ERROR,
							"anr_determine_cdma_changes_in_nrt_when_old_nr_not_send: "
							"CDMA NR cannot be updated as maximum no. of neighbors "
							"which can be send to RRM per ARFCN is already reached\n");

					is_roll_back_allowed = SON_TRUE;
					*p_error_code = SON_ERR_ANR_MAX_ALLOWED_XRTT_NBRS_PER_ARFCN_TO_RRM_REACHED;
				}

				if (SON_FALSE == is_roll_back_allowed)
				{
					*p_error_code = SON_NO_ERROR;

					*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

					*p_is_old_nr_send = SON_FALSE;
				}
				else
				{
					*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

					*p_is_old_nr_send = SON_FALSE;
				}
			}
			else if (SON_CGI_HRPD_PRESENT &
					p_new_hash->u.cdma_neighbor_info.nbr_cgi.bitmask) 
			{
				anr_cdma_hrpd_band_stats_node_t *p_band_list_node = SON_PNULL;
				anr_cdma_hrpd_freq_stats_node_t *p_freq_list_node = SON_PNULL;
				anr_cdma_hrpd_band_stats_node_t *p_list_node_tmp = SON_PNULL;

				/* Performing band level search */
				p_band_list_node = anr_find_record_in_cdma_hrpd_band_stats_list(
						&p_cell_ctxt->cdma_hrpd_band_stats_list,
						p_new_hash->u.cdma_neighbor_info.band_class);

				/* Performing freq level search */
				if (p_band_list_node != SON_PNULL)
				{
					p_freq_list_node = anr_find_record_in_cdma_hrpd_freq_stats_list(
							&p_band_list_node->cdma_band_stats.freq_list,
							p_new_hash->u.cdma_neighbor_info.arfcn);
				}

				if ((SON_PNULL == p_band_list_node) &&
						((SON_RRM_MAX_CDMA_BAND_ALLOWED) <=
						 (list_count(&p_cell_ctxt->cdma_hrpd_band_stats_list))))
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_ERROR,
							"anr_determine_cdma_changes_in_nrt_when_old_nr_not_send: "
							"CDMA NR cannot be updated as maximum band class sendable "
							"to RRM is already reached\n");

					*p_error_code = SON_ERR_ANR_MAX_ALLOWED_BANDS_NBRS_TO_RRM_REACHED;
					*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;
					*p_is_old_nr_send = SON_FALSE;

					SON_UT_TRACE_EXIT();
					return;
				}
				else if (((SON_PNULL != p_band_list_node) &&
							(SON_PNULL == p_freq_list_node)) &&
						((SON_RRM_MAX_CDMA_FREQ_ALLOWED) <=
						 (list_count(&p_band_list_node->cdma_band_stats.freq_list))))
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_ERROR,
							"anr_determine_cdma_changes_in_nrt_when_old_nr_not_send: "
							"CDMA NR cannot be updated as maximum frequency sendable "
							"to RRM is already reached\n");

					*p_error_code = SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED;
					*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;
					*p_is_old_nr_send = SON_FALSE;

					SON_UT_TRACE_EXIT();
					return;
				}
				else
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_DETAILED,
							"CDMA NR can be updated and is sendable to RRM\n");
				}

				/* If the band_class/frequency is modified and it is not in the list */
				if ((SON_PNULL == p_band_list_node) ||
						((SON_PNULL != p_band_list_node) &&
						 (SON_PNULL == p_freq_list_node)))
				{
					/* Inserting new band_class/frequency in the list */
					p_list_node_tmp =  anr_insert_record_in_cdma_hrpd_band_stats_list( 
							p_cell_ctxt,
							&p_new_hash->u.cdma_neighbor_info);

					if (SON_PNULL == p_list_node_tmp)
					{
						SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
								SON_ERROR,
								"anr_determine_cdma_changes_in_nrt_when_old_nr_not_send: "
								"CDMA NR cannot be updated because of memory allocation failure\n");

						is_roll_back_allowed = SON_TRUE;
						*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;
					}
				}

				/* If the frequency exists then just increment the count */
				else if (SON_FALSE == increment_cdma_hrpd_nbr_count_for_rrm(
							&p_new_hash->u.cdma_neighbor_info,
							p_cell_ctxt))
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_ERROR,
							"anr_determine_cdma_changes_in_nrt_when_old_nr_not_send: "
							"CDMA NR cannot be updated as maximum no. of neighbors "
							"which can be send to RRM per ARFCN is already reached\n");

					is_roll_back_allowed = SON_TRUE;
					*p_error_code = SON_ERR_ANR_MAX_ALLOWED_HRPD_NBRS_PER_ARFCN_TO_RRM_REACHED;
				}

				if (SON_FALSE == is_roll_back_allowed)
				{
					*p_error_code = SON_NO_ERROR;

					*p_is_rrm_notif_reqd_for_new_nr = SON_TRUE;

					*p_is_old_nr_send = SON_FALSE;
				}
				else
				{
					*p_is_rrm_notif_reqd_for_new_nr = SON_FALSE;

					*p_is_old_nr_send = SON_FALSE;
				}
			}
		}
	}

	SON_UT_TRACE_EXIT();
	return;
	/* Coverity CID-55067 Fix Ends */
} /* anr_determine_cdma_changes_in_nrt_when_old_nr_not_send */

/******************************************************************************
 * Function Name  : anr_get_cell_identity_from_inter_nr_rec
 * Inputs         : p_hash_rec - Pointer to a the NR record
 * Outputs        : None.
 * Returns        : son_u32    - Cell Identity value as unsigned int.
 * Description    : This function is used to find the cell Identity value from 
 *                  given inter RAT record.
 ******************************************************************************/
son_u32 
anr_get_cell_identity_from_inter_nr_rec
(
 const anr_inter_rat_hash_rec_t      *p_hash_rec
 )
{
	son_u32 cell_identity = 0;
	son_oam_log_on_off_et log_mode = anr_get_log_mode();


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_hash_rec)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_get_cell_identity_from_inter_nr_rec: Could not read the" 
				" cell identity, p_hash_rec is NULL\n");
	}

	else if (ANR_UTRAN_NEIGHBOR == p_hash_rec->t)
	{
		cell_identity = son_convert_const_char_cell_id_to_int_cell_id(p_hash_rec->u.utran_neighbor_info.
				nbr_cgi.cell_identity);
	}
	else if (ANR_GERAN_NEIGHBOR == p_hash_rec->t)
	{
		cell_identity = son_convert_const_char_cell_id_to_int_cell_id(p_hash_rec->u.geran_neighbor_info.
				nbr_cgi.cell_identity);
	}
	else
	{
		/* Get the cell Id for other RAT type in next release */
	}

	SON_UT_TRACE_EXIT();
	return cell_identity;
} /* anr_get_cell_identity_from_inter_nr_rec */

/******************************************************************************
 * Function Name  : anr_inter_rat_insert_nr
 * Inputs         : p_cell_ctxt         - Pointer to cell context
 *                  p_hash_rec          - Pointer to hash record
 *                  check_remove_list   - Flag to check remove list
 *                  p_error_code        - Error code
 *                  p_oam_notification  - Pointer to oam notification msg
 *                  p_rrm_notification  - Pointer to rrm notification msg
 * Outputs        : p_error_code -  Pointer to a variable for storing error code
 *
 * Returns        : anr_inter_rat_hash_rec_t* - Pointer to the data field of the
 *                  new NR record inserted.
 * Description    : This function is used to insert an entry in the inter RAT  
 *                  Neighbor Relation Table to store the neighbors of a cell
 ******************************************************************************/
anr_inter_rat_hash_rec_t *
anr_inter_rat_insert_nr
(
 anr_cell_context_t *p_cell_ctxt,
 anr_inter_rat_hash_rec_t      *p_hash_rec,
 son_bool_et check_remove_list,
 son_error_et *p_error_code,
 son_anr_updated_nrt_info_t *p_oam_notification,
 son_anr_rrm_updated_nrt_info_t *p_rrm_notification
 )
{
	/* Coverity CID-55066 Fix Starts */
	anr_inter_rat_hash_nr_tuple_t 	*p_hash_tpl  = SON_PNULL;
	son_oam_log_on_off_et         	log_mode     = anr_get_log_mode();
	son_u32                       	cell_identity = 0;
	son_inter_rat_neighbor_info_t   inter_rat_nr    = {0};
	son_global_cell_id_t            cell_id;
	SON_HASH 						*p_hash = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_hash_rec || SON_PNULL == p_cell_ctxt ||
			SON_PNULL == p_error_code)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_inter_rat_insert_nr: NR could not be added, "
				"p_cell_ctxt or p_hash_rec or p_error_code is NULL\n");

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	/* Get the value of cell identity to print in logs */
	cell_identity = anr_get_cell_identity_from_inter_nr_rec(p_hash_rec);

	SON_MEMSET(&cell_id, 0, sizeof(cell_id));
	anr_extract_global_cell_id_from_inter_rat_hash_rec(
			p_hash_rec,
			&cell_id);

	if (SON_TRUE == check_remove_list)
	{
		if (SON_PNULL != anr_find_cell_in_remove_list(
					&p_cell_ctxt->nr_remove_list,
					&cell_id))
		{
			if (ANR_CDMA_NEIGHBOR != p_hash_rec->t)
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_ERROR,
						"anr_inter_rat_insert_nr:"
						"Cell [Cell Id: 0x%x]is present in remove list \n",
						cell_identity);
			}
			else
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_ERROR,
						"anr_inter_rat_insert_nr:"
						"CDMA Nbr is present in remove list\n");
				anr_print_cdma_cell_id(&p_hash_rec->u.cdma_neighbor_info.nbr_cgi);
			}

			*p_error_code = SON_ERR_ANR_NR_PRESENT_IN_REMOVE_LIST;
			SON_UT_TRACE_EXIT();
			return SON_PNULL;
		}
	}

	/* Validate the data to be inserted */
	if (SON_FALSE == anr_validate_rec_data(p_hash_rec))
	{
		*p_error_code = SON_ERR_INVALID_PARAMS;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	p_hash = &p_cell_ctxt->inter_rat_neighbor_table;

	if (SON_PNULL != hash_find(p_hash, 
				(son_void_t *)p_hash_rec))
	{
		if (ANR_CDMA_NEIGHBOR != p_hash_rec->t)
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_inter_rat_insert_nr: NR [Cell Id: 0x%x] Already Present."
					"Duplicate Entry\n",
					cell_identity);
		}
		else 
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_inter_rat_insert_nr: CDMA NR Already Present."
					"Duplicate Entry\n");
			anr_print_cdma_cell_id(&p_hash_rec->u.cdma_neighbor_info.nbr_cgi);
		}

		*p_error_code = SON_ERR_ANR_DUPLICATE_NR;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if (SON_MAX_NO_INTER_RAT_NEIGHBOUR_CELLS <= hash_count(p_hash))
	{
		if (ANR_CDMA_NEIGHBOR != p_hash_rec->t)
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_inter_rat_insert_nr: NR [Cell Id: 0x%x] can not be inserted as "
					"number of inter rat neighbor cells currently present in the NRT is [%d]\n\n",                
					cell_identity,
					hash_count(p_hash));
		}
		else 
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_inter_rat_insert_nr: CDMA NR can not be inserted as "
					"number of inter rat neighbor cells currently present in the NRT is [%d]\n\n",
					hash_count(p_hash));
			anr_print_cdma_cell_id(&p_hash_rec->u.cdma_neighbor_info.nbr_cgi);
		}

		*p_error_code = SON_ERR_ANR_MAX_NRT_SIZE_REACHED;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	/*Set the dafault Cell specific offset */
	if (ANR_UTRAN_NEIGHBOR ==  p_hash_rec->t)
	{
		if (!(p_hash_rec->u.utran_neighbor_info.bitmask & SON_UTRAN_CSO_PRESENT))
		{
			p_hash_rec->u.utran_neighbor_info.bitmask |= SON_UTRAN_CSO_PRESENT;
			p_hash_rec->u.utran_neighbor_info.cell_specific_offset = 0;
		}
	}
	else if (ANR_GERAN_NEIGHBOR ==  p_hash_rec->t)
	{
		if (!(p_hash_rec->u.geran_neighbor_info.bitmask & SON_GERAN_CSO_PRESENT))
		{
			p_hash_rec->u.geran_neighbor_info.bitmask |= SON_GERAN_CSO_PRESENT;
			p_hash_rec->u.geran_neighbor_info.cell_specific_offset = 0;
		}
		/*SPR-11121-Start-Setting the default dtm_supported value if not present*/
		if(!(p_hash_rec->u.geran_neighbor_info.bitmask & SON_GERAN_DTM_SUPPORT_PRESENT))
		{
			p_hash_rec->u.geran_neighbor_info.bitmask |= SON_GERAN_DTM_SUPPORT_PRESENT;
			p_hash_rec->u.geran_neighbor_info.dtm_supported = 0;
		}
		/*SPR-11121-End*/
	}
	else if (ANR_CDMA_NEIGHBOR ==  p_hash_rec->t)
	{
		if (!(p_hash_rec->u.cdma_neighbor_info.bitmask & SON_CDMA_CSO_PRESENT))
		{
			p_hash_rec->u.cdma_neighbor_info.bitmask |= SON_CDMA_CSO_PRESENT;
			p_hash_rec->u.cdma_neighbor_info.cell_specific_offset = 0;
		}
	}

	/*validate inter rat hash record just to determine if the freq is already configured or not*/
	son_error_et err_code = SON_NO_ERROR;
	son_bool_et is_nr_sendable_to_rrm = SON_FALSE;

	is_nr_sendable_to_rrm
		= anr_validate_inter_rat_record_insertion(p_cell_ctxt, p_hash_rec, &err_code);

	if (SON_NO_ERROR != err_code)
	{
		if (ANR_CDMA_NEIGHBOR != p_hash_rec->t)
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_inter_rat_insert_nr: NR [Cell Id: 0x%x] can not be inserted\n",
					cell_identity);
		}
		else 
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_inter_rat_insert_nr: CDMA NR can not be inserted\n");
			anr_print_cdma_cell_id(&p_hash_rec->u.cdma_neighbor_info.nbr_cgi);
		}

		*p_error_code = err_code;   
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	/* Validating and adding RNC Id for UTRAN Neighbors */
	if (ANR_UTRAN_NEIGHBOR == p_hash_rec->t)
	{
		if (SON_PNULL == p_oam_notification)
		{
			son_u8      rnc_indicator;
			son_u8      rnc_id[SON_RNC_ID_SIZE];
			son_bool_et ret_val = SON_FALSE;

			SON_MEMSET(rnc_id, 0, sizeof(rnc_id));

			rnc_indicator = anr_get_rnc_indicator_from_cell_id(
					p_hash_rec->u.utran_neighbor_info.nbr_cgi.cell_identity);

			if (0 == rnc_indicator)
			{
				/* Get the non-extended RNC Id from Utran Cell Id*/
				anr_get_non_extended_rnc_id_from_cell_id(
						p_hash_rec->u.utran_neighbor_info.nbr_cgi.cell_identity,
						rnc_id);

				/* Find the RNC Id if already present in the list */
				ret_val = anr_find_rnc_id_from_the_list(
						&p_cell_ctxt->non_extended_rnc_id_list,
						rnc_id);

				if (SON_FALSE == ret_val)
				{
					son_bool_et result = SON_FALSE;

					/* Insert new Non-Extended RNC Id in the list */
					result = anr_insert_in_rnc_id_list(
							&p_cell_ctxt->non_extended_rnc_id_list,
							rnc_id);

					if (SON_FALSE == result)
					{
						SON_LOG(log_mode, p_son_anr_facility_name,
								SON_ERROR,
								"anr_inter_rat_insert_nr: Non-Extended RNC Id "
								"[RNCID: 0x%p] could not be inserted "
								"in the list\n", (son_u32 *)rnc_id);
					}
				}
			}
			else if (1 == rnc_indicator)
			{
				/* Get the extended RNC Id from Utran Cell Id*/
				anr_get_extended_rnc_id_from_cell_id(
						p_hash_rec->u.utran_neighbor_info.nbr_cgi.cell_identity,
						rnc_id);

				/* Find the RNC Id if already present in the list */
				ret_val = anr_find_rnc_id_from_the_list(
						&p_cell_ctxt->extended_rnc_id_list,
						rnc_id);

				if (SON_FALSE == ret_val)
				{
					son_bool_et result = SON_FALSE;

					/* Insert new Extended RNC Id in the list */
					result = anr_insert_in_rnc_id_list(
							&p_cell_ctxt->extended_rnc_id_list,
							rnc_id);

					if (SON_FALSE == result)
					{
						SON_LOG(log_mode, p_son_anr_facility_name,
								SON_ERROR,
								"anr_inter_rat_insert_nr: Extended RNC Id "
								"[RNCID: 0x%p] could not be inserted "
								"in the list\n", (son_u32 *)rnc_id);
					}
				}
			}
			else
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_ERROR,
						"anr_inter_rat_insert_nr: UTRAN NR [Cell id :0x%x] "
						"received cannot added to the NR hash table "
						"as RNC Id indicator is INVALID\n",
						cell_identity);

				*p_error_code = SON_ERR_INVALID_PARAMS;
				SON_UT_TRACE_EXIT();
				return SON_PNULL;
			}
		}
		else
		{
			son_u8      rnc_id[SON_RNC_ID_SIZE];
			son_bool_et ret_val = SON_FALSE;
			SON_MEMSET(rnc_id, 0, sizeof(rnc_id));

			/* Get the non-extended RNC Id from Utran Cell Id */
			anr_get_non_extended_rnc_id_from_cell_id(
					p_hash_rec->u.utran_neighbor_info.nbr_cgi.cell_identity,
					rnc_id);

			/* Find the RNC Id if present in the Non-Extended RNC Id list */
			ret_val = anr_find_rnc_id_from_the_list(
					&p_cell_ctxt->non_extended_rnc_id_list,
					rnc_id);

			if (ret_val == SON_FALSE)
			{
				/* Get the extended RNC Id from Utran Cell Id */
				anr_get_extended_rnc_id_from_cell_id(
						p_hash_rec->u.utran_neighbor_info.nbr_cgi.cell_identity,
						rnc_id);

				/* Find the RNC Id if present in the Extended RNC Id list */
				ret_val = anr_find_rnc_id_from_the_list(
						&p_cell_ctxt->extended_rnc_id_list,
						rnc_id);

				if (SON_FALSE == ret_val)
				{
					SON_LOG(log_mode, p_son_anr_facility_name,
							SON_ERROR,
							"anr_inter_rat_insert_nr: UTRAN NR [Cell id :0x%x] "
							"received cannot added to the NR hash table "
							"as RNC Id is not matching\n",
							cell_identity);

					*p_error_code = SON_ERR_ANR_RNC_ID_NOT_FOUND;
					SON_UT_TRACE_EXIT();
					return SON_PNULL;
				}
				else
				{
					p_hash_rec->u.utran_neighbor_info.nbr_cgi.cell_identity[3] 
						&= 0xF1;

					SON_LOG(log_mode, p_son_anr_facility_name,
							SON_BRIEF,
							"anr_inter_rat_insert_nr: Nbr "
							"[Cell Id: 0x%x] can be inserted "
							"in the NRT as RNC Id is matching\n",
							cell_identity);               
				}
			}
			else
			{
				p_hash_rec->u.utran_neighbor_info.nbr_cgi.cell_identity[3] 
					&= 0xF0;

				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_BRIEF,
						"anr_inter_rat_insert_nr: Nbr "
						"[Cell Id: 0x%x] can be inserted "
						"in the NRT as RNC Id is matching\n",
						cell_identity);
			}
		}
	}


	ANR_GET_HASH_REC(anr_get_inter_rat_hash_rec_pool(), p_hash_tpl);
	if (SON_PNULL == p_hash_tpl)
	{
		if (ANR_CDMA_NEIGHBOR != p_hash_rec->t)
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_inter_rat_insert_nr: NR [Cell Id: 0x%x]" 
					" could not be added, memory allocation failed\n",
					cell_identity);
		}
		else 
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_inter_rat_insert_nr: CDMA NR "
					"could not be added, memory allocation failed\n");
			anr_print_cdma_cell_id(&p_hash_rec->u.cdma_neighbor_info.nbr_cgi);
		}

		*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	SON_MEMSET(p_hash_tpl, 0, sizeof(anr_inter_rat_hash_nr_tuple_t));
	SON_MEMCPY(&p_hash_tpl->data, p_hash_rec, sizeof(anr_inter_rat_hash_rec_t));

	/*Set the HO Stats*/
	if (ANR_UTRAN_NEIGHBOR ==  p_hash_rec->t)
	{
		p_hash_tpl->data.u.utran_neighbor_info.bitmask |= SON_UTRAN_NR_HO_STATS_PRESENT;
		SON_MEMSET(&p_hash_tpl->data.u.utran_neighbor_info.handover_stats, 0,
				sizeof(son_neighbor_handover_stats_t));
	}
	else if (ANR_GERAN_NEIGHBOR ==  p_hash_rec->t)
	{
		p_hash_tpl->data.u.geran_neighbor_info.bitmask |= SON_GERAN_NR_HO_STATS_PRESENT;
		SON_MEMSET(&p_hash_tpl->data.u.geran_neighbor_info.handover_stats, 0,
				sizeof(son_neighbor_handover_stats_t));
	}
	else if (ANR_CDMA_NEIGHBOR == p_hash_rec->t)
	{
		p_hash_tpl->data.u.cdma_neighbor_info.bitmask |= SON_CDMA_NR_HO_STATS_PRESENT;
		SON_MEMSET(&p_hash_tpl->data.u.cdma_neighbor_info.handover_stats, 0,
				sizeof(son_neighbor_handover_stats_t));
	}

	/* Setting the broadcast status to default value, if not present*/
	if (!(p_hash_rec->u.cdma_neighbor_info.bitmask & SON_CDMA_BROADCAST_MODE_PRESENT))
	{
		p_hash_rec->u.cdma_neighbor_info.bitmask |= SON_CDMA_BROADCAST_MODE_PRESENT;
		p_hash_rec->u.cdma_neighbor_info.broadcast_status = SON_FALSE;
	}

	if (ANR_CDMA_NEIGHBOR != p_hash_rec->t)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_BRIEF,
				"anr_inter_rat_insert_nr: NR [Cell id :0x%x] "
				"received is added to the NR hash table\n",
				cell_identity);
	}
	else 
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_BRIEF,
				"anr_inter_rat_insert_nr: CDMA NR "
				"received is added to the NR hash table\n");
		anr_print_cdma_cell_id(&p_hash_rec->u.cdma_neighbor_info.nbr_cgi);
	}

	*p_error_code = SON_NO_ERROR;
	hash_insert(p_hash, &p_hash_tpl->anchor);


	if (SON_FALSE == check_remove_list)
	{
		/* If the neighbor is present in Remove List, remove neighbor from REMOVE LIST */
		if (SON_TRUE == anr_delete_cell_from_remove_list(
					&p_cell_ctxt->nr_remove_list, &cell_id))
		{
			if (ANR_CDMA_NEIGHBOR != p_hash_rec->t)
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_ERROR,
						"anr_inter_rat_insert_nr: NR [Cell Id: 0x%x] Removed"
						" from Remove List.",
						cell_identity);
			}
			else
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_ERROR,
						"anr_inter_rat_insert_nr: CDMA NR Removed "
						"from Remove List\n");
				anr_print_cdma_cell_id(&p_hash_rec->u.cdma_neighbor_info.nbr_cgi);
			}
		}
	}

	son_time(&p_hash_tpl->data.last_activity_ts);

	if (p_oam_notification)
	{
		p_oam_notification->nrt_op = SON_NRT_OP_ADD;
		anr_convert_inter_rat_hash_rec_to_inter_rat_nr(
				&p_hash_tpl->data, &inter_rat_nr);

		anr_construct_oam_updated_ncl(&p_oam_notification->nr_list,
				&inter_rat_nr,
				SON_TRUE);
	}

	if (p_rrm_notification  && SON_TRUE == is_nr_sendable_to_rrm)
	{
		if (ANR_UTRAN_NEIGHBOR == p_hash_tpl->data.t)
		{
			anr_utran_freq_stats_node_t *p_list_node = SON_PNULL;
			SON_LIST *p_list = SON_PNULL;

			p_list = anr_determine_utran_freq_stats_list(
					p_cell_ctxt, p_hash_tpl->data.u.utran_neighbor_info.pci);

			if (SON_PNULL != p_list)
			{ 
				p_list_node = anr_find_record_in_utran_freq_stats_list(
						p_list,
						p_hash_tpl->data.u.utran_neighbor_info.uarfcn);
			}

			/*Assuming memory will be available*/
			if (SON_PNULL == p_list_node && SON_PNULL != p_list)
			{
				anr_insert_record_in_utran_freq_stats_list(
						p_list,
						p_hash_tpl->data.u.utran_neighbor_info.uarfcn);
			}
			else
			{
				if (SON_FALSE == increment_utran_nbr_count_for_rrm(
							p_cell_ctxt, p_list_node))
				{        
					SON_LOG(log_mode, p_son_anr_facility_name,
							SON_ERROR,
							"anr_inter_rat_insert_nr:" 
							"increment_utran_nbr_count_for_rrm Failed \n");
				}
			}

			anr_construct_rrm_inter_rat_ncl_for_utran_nbr(
					&p_rrm_notification->inter_rat_ncl.utran_neighbor_list,
					&p_hash_tpl->data.u.utran_neighbor_info, SON_FALSE);

			if (SON_TRUE == is_utran_nbrs_available_in_rrm_notification(
						&p_rrm_notification->inter_rat_ncl.utran_neighbor_list))
			{
				p_rrm_notification->inter_rat_ncl.bitmask |= SON_RRM_UTRAN_NR_LIST_PRESENT;
			}
		}

		if (ANR_GERAN_NEIGHBOR == p_hash_tpl->data.t)
		{
			anr_geran_freq_stats_node_t *p_list_node = SON_PNULL; 

			anr_geran_freq_stats_data_t data;
			SON_MEMSET(&data, 0, sizeof(anr_geran_freq_stats_data_t));
			data.arfcn = p_hash_tpl->data.u.geran_neighbor_info.arfcn;
			data.band_ind = p_hash_tpl->data.u.geran_neighbor_info.band_ind;

			p_list_node = anr_find_record_in_geran_freq_stats_list(
					&p_cell_ctxt->geran_freq_stats_list, &data);

			/*Assuming memory will be available*/
			if (SON_PNULL == p_list_node)
			{
				anr_insert_record_in_geran_freq_stats_list(
						&p_cell_ctxt->geran_freq_stats_list,
						p_hash_tpl->data.u.geran_neighbor_info.arfcn,
						p_hash_tpl->data.u.geran_neighbor_info.band_ind);
			}
			else
			{
				if (SON_FALSE == increment_geran_nbr_count_for_rrm(
							p_cell_ctxt, p_list_node))
				{
					SON_LOG(log_mode, p_son_anr_facility_name,
							SON_ERROR,
							"anr_inter_rat_insert_nr:" 
							" increment_geran_nbr_count_for_rrm Failed \n");
				}
			}

			anr_construct_rrm_inter_rat_ncl_for_geran_nbr(
					&p_rrm_notification->inter_rat_ncl.geran_neighbor_list,
					&p_hash_tpl->data.u.geran_neighbor_info, SON_FALSE);

			if (SON_TRUE == is_geran_nbrs_available_in_rrm_notification(
						&p_rrm_notification->inter_rat_ncl.geran_neighbor_list))
			{
				p_rrm_notification->inter_rat_ncl.bitmask |= SON_RRM_GERAN_NR_LIST_PRESENT;
			}
		}

		if (ANR_CDMA_NEIGHBOR == p_hash_tpl->data.t)
		{
			if (p_hash_rec->u.cdma_neighbor_info.nbr_cgi.bitmask & SON_CGI_1X_RTT_PRESENT)
			{
				anr_cdma_xrtt_band_stats_node_t *p_list_xrtt_band_node = SON_PNULL;
				anr_cdma_xrtt_freq_stats_node_t *p_list_xrtt_freq_node = SON_PNULL;

				/*Searching for band in cell context*/
				p_list_xrtt_band_node = anr_find_record_in_cdma_xrtt_band_stats_list(
						&p_cell_ctxt->cdma_xrtt_band_stats_list,
						p_hash_rec->u.cdma_neighbor_info.band_class);

				/*Searching for freq in band stats list*/
				if (p_list_xrtt_band_node != SON_PNULL)
				{
					p_list_xrtt_freq_node = anr_find_record_in_cdma_xrtt_freq_stats_list(
							&p_list_xrtt_band_node->cdma_band_stats.freq_list,
							p_hash_rec->u.cdma_neighbor_info.arfcn);
				}

				if ((SON_PNULL == p_list_xrtt_band_node) ||
						((SON_PNULL != p_list_xrtt_band_node) && 
						 (SON_PNULL == p_list_xrtt_freq_node)))
				{
					/*Assuming memory will be available*/
					anr_insert_record_in_cdma_xrtt_band_stats_list(
							p_cell_ctxt,
							&p_hash_tpl->data.u.cdma_neighbor_info);
				}
				else
				{
					increment_cdma_xrtt_nbr_count_for_rrm(
							&p_hash_rec->u.cdma_neighbor_info,
							p_cell_ctxt);
				}

				anr_construct_rrm_inter_rat_ncl_for_cdma_xrtt_nbr(
						&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_xrtt,
						&p_hash_tpl->data.u.cdma_neighbor_info, SON_FALSE);

				if (SON_TRUE == is_cdma_xrtt_nbrs_available_in_rrm_notification(
							&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_xrtt))
				{
					p_rrm_notification->inter_rat_ncl.bitmask |= SON_RRM_CDMA_NR_LIST_XRTT_PRESENT;
				}
			}
			else if (SON_CGI_HRPD_PRESENT & p_hash_rec->u.cdma_neighbor_info.nbr_cgi.bitmask)
			{
				anr_cdma_hrpd_band_stats_node_t *p_list_hrpd_band_node = SON_PNULL;
				anr_cdma_hrpd_freq_stats_node_t *p_list_hrpd_freq_node = SON_PNULL;

				/*Searching for band in cell context*/
				p_list_hrpd_band_node = anr_find_record_in_cdma_hrpd_band_stats_list(
						&p_cell_ctxt->cdma_hrpd_band_stats_list,
						p_hash_rec->u.cdma_neighbor_info.band_class);

				/*Searching for freq in band stats list*/
				if (p_list_hrpd_band_node != SON_PNULL)
				{
					p_list_hrpd_freq_node = anr_find_record_in_cdma_hrpd_freq_stats_list(
							&p_list_hrpd_band_node->cdma_band_stats.freq_list,
							p_hash_rec->u.cdma_neighbor_info.arfcn);
				}

				if ((SON_PNULL == p_list_hrpd_band_node) ||
						((SON_PNULL != p_list_hrpd_band_node) && 
						 (SON_PNULL == p_list_hrpd_freq_node)))
				{
					anr_insert_record_in_cdma_hrpd_band_stats_list(
							p_cell_ctxt,
							&p_hash_tpl->data.u.cdma_neighbor_info);
				}
				else
				{
					increment_cdma_hrpd_nbr_count_for_rrm(
							&p_hash_rec->u.cdma_neighbor_info,
							p_cell_ctxt);
				}

				anr_construct_rrm_inter_rat_ncl_for_cdma_hrpd_nbr(
						&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_hrpd,
						&p_hash_tpl->data.u.cdma_neighbor_info, SON_FALSE);

				if (SON_TRUE == is_cdma_hrpd_nbrs_available_in_rrm_notification(
							&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_hrpd))
				{
					p_rrm_notification->inter_rat_ncl.bitmask |= SON_RRM_CDMA_NR_LIST_HRPD_PRESENT;
				}
			}
		} 
	}

	SON_UT_TRACE_EXIT();
	return &p_hash_tpl->data;
	/* Coverity CID-55066 Fix Ends */
} /* anr_inter_rat_insert_nr */

/******************************************************************************
 * Function Name  : is_cdma_nbr_notifiable_to_rrm
 * Inputs         : p_nbr_info - Pointer to CDMA NBR info
 * Outputs        : Status whether CDMA NBR is notifiable to RRM 
 * Returns        : son_bool_et - validity status flag
 * Description    : This function is used to verify if the CDMA NBRs are to be
 *                  notified to RRM.
 ******************************************************************************/
son_bool_et
is_cdma_nbr_notifiable_to_rrm
(
 son_cdma_neighbor_info_t *p_nbr_info
 )
{
	son_bool_et is_nbr_notf_to_rrm = SON_TRUE;
	son_ho_status_et ho_status = SON_HO_UNDEFINED;
	son_u32 bitmask = 0;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_nbr_info)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"Validation failure "
				"as p_nbr_info is SON_PNULL");
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	ho_status = anr_get_ho_status_of_cdma_nr(p_nbr_info);
	bitmask = SON_CDMA_CSO_PRESENT;

	if (((p_nbr_info->bitmask & bitmask) != bitmask)  || SON_HO_PROHIBITED == ho_status)
	{
		is_nbr_notf_to_rrm = SON_FALSE;
	}

	SON_UT_TRACE_EXIT();
	return is_nbr_notf_to_rrm;
} /* is_cdma_nbr_notifiable_to_rrm */

/******************************************************************************
 * Function Name  : anr_del_nr
 * Inputs         : p_cell_ctxt -   Pointer to the context for a cell
 *                  configured.
 *                : p_cell_id  - Pointer to the neighbor cell Id
 *                : p_oam_notification - Pointer to oam notification structure
 *                : p_rrm_notification - Pointer to rrm notification structure
 * Outputs        : p_error_code -  Pointer to a variable for storing error code
 *                : p_hash_rec - Pointer to intra rat hash record structure
 * Returns        : son_return_et      - SON_SUCCESS The message is handled successfully
 *                                       SON_FAILURE The message is not handled successfully
 * Description    : This function is used for removing selected NR
 *                  records for a cell configured
 ******************************************************************************/
son_return_et
anr_del_nr
(
 anr_cell_context_t      *p_cell_ctxt,
 son_global_cell_id_t    *p_cell_id,
 son_bool_et check_remove_list,
 son_error_et            *p_error_code,
 son_void_t              *p_hash_rec,
 son_anr_updated_nrt_info_t *p_oam_notification,
 son_anr_rrm_updated_nrt_info_t *p_rrm_notification
 )
{
	son_return_et del_status = SON_FAILURE;
	son_oam_log_on_off_et log_mode = anr_get_log_mode();


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_cell_id
			|| SON_PNULL == p_hash_rec)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_del_nr: NR could not be deleted, "
				"p_cell_ctxt or p_cell_id or p_hash_rec is NULL\n");
		*p_error_code = SON_ERR_INVALID_PARAMS;
		SON_UT_TRACE_EXIT();
		return del_status;
	}

	if (SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT & p_cell_id->bitmask)
	{
		del_status = anr_intra_rat_del_nr(
				p_cell_ctxt,
				&p_cell_id->intra_rat_global_cell_id,
				check_remove_list,
				p_error_code, p_hash_rec, p_oam_notification,
				p_rrm_notification, SON_TRUE);
	}
	else if (SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT & p_cell_id->bitmask)
	{
		del_status = anr_inter_rat_del_nr(
				p_cell_ctxt,
				&p_cell_id->inter_rat_global_cell_id,
				check_remove_list,
				p_error_code, p_hash_rec, p_oam_notification, p_rrm_notification);
	}
	else
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_del_nr: NR could not be deleted,"
				" cell Id received with invalid bitmask value = %u.\n",
				p_cell_id->bitmask);
		*p_error_code = SON_ERR_INVALID_PARAMS;
	}

	SON_UT_TRACE_EXIT();    
	return del_status;
} /* anr_del_nr */

/******************************************************************************
 * Function Name  : anr_intra_rat_del_nr
 * Inputs         : p_hash_rec -	Pointer to the hash context for a cell 
 *                  configured.
 *                  p_nr_cell_id  - Pointer to the list of neighbor cellids
 *                  check_remove_list - Flag to check the remove list
 *                  p_oam_notification  - Pointer to oam notification msg
 *                  p_rrm_notification  - Pointer to rrm notification msg
 *                  is_conflict_confusion_trigger - Flag to determine whether conflict
 *  8                              \ confusion indication needs to be triggered
 * Outputs        : p_error_code -  Pointer to a variable for storing error code
 *                : p_hash_rec - Pointer to a intra rat hash record variable 
 *
 * Returns        : son_return_et  - SON_SUCCESS The message is handled successfully
 *                                   SON_FAILURE The message is not handled successfully
 * Description    : This function is used for removing selected NR 
 *                  records for a cell configured from intra RAT hash table
 ******************************************************************************/
son_return_et
anr_intra_rat_del_nr
(
 anr_cell_context_t      *p_cell_ctxt,
 son_intra_rat_global_cell_id_t  *p_nr_cell_id,
 son_bool_et check_remove_list,
 son_error_et *p_error_code,
 son_void_t *p_hash_rec,
 son_anr_updated_nrt_info_t *p_oam_notification,
 son_anr_rrm_updated_nrt_info_t *p_rrm_notification,
 son_bool_et         is_conflict_confusion_trigger
 )
{
	anr_intra_rat_hash_rec_t       	hash_data;
	anr_intra_rat_hash_nr_tuple_t  	*p_tpl      = SON_PNULL;
	SON_HASH_NODE						*p_hash_node = SON_PNULL;
	son_oam_log_on_off_et        log_mode        = anr_get_log_mode();
	SON_HASH *p_hash = SON_PNULL;
	son_global_cell_id_t            cell_id;
	/*SPR-12596-Start*/
	son_anr_pci_conflict_confusion_ind_t msg; 
	conflict_confusion_pci_list_node_t  *p_pci_list_node = SON_PNULL;
	son_u16     pci = SON_NULL;
	/*SPR-12596-End*/


	SON_UT_TRACE_ENTER();

	SON_MEMSET(&hash_data, 0, sizeof(anr_intra_rat_hash_rec_t));
	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_nr_cell_id ||
			SON_PNULL == p_error_code || SON_PNULL == p_hash_rec)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"NR could not be deleted, "
				"p_cell_ctxt or p_nr_cell_id or p_error_code or p_hash_rec is NULL\n");
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	p_hash = &p_cell_ctxt->intra_rat_neighbor_table;

	SON_MEMSET(&hash_data, 0, sizeof(anr_intra_rat_hash_rec_t));

	SON_MEMCPY(&hash_data.neighbor_info.nbr_cgi, p_nr_cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	if (SON_PNULL != (p_hash_node = hash_find(p_hash, (son_void_t *)&hash_data)))
	{
		p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_intra_rat_hash_nr_tuple_t, anchor, p_hash_node);

		/* Populating the output intra rat hash rec */
		SON_MEMCPY((anr_intra_rat_hash_rec_t *)p_hash_rec,
				&p_tpl->data,
				sizeof(anr_intra_rat_hash_rec_t));
		list_init(&((anr_intra_rat_hash_rec_t *)p_hash_rec)->anr_nr_nbr_list);

		/*SPR-12596-Start*/
		pci = p_tpl->data.neighbor_info.pci;
		/*SPR-12596-End*/

		if (p_rrm_notification)
		{
			if (SON_TRUE == is_intra_rat_nbr_notifiable_to_rrm(&p_tpl->data.neighbor_info))
			{
				anr_bs_allowed_node_t *p_bs_allowed_record = SON_PNULL;
				son_bool_et      is_ho_allowed_reqd      = SON_FALSE;
				son_bool_et      is_ho_blk_reqd          = SON_FALSE;
				son_ho_status_et ho_status = anr_get_ho_status_of_eutran_nr(
						&p_tpl->data.neighbor_info);

				p_bs_allowed_record   =  anr_find_record_in_bs_allowed_stats_list(
						&p_cell_ctxt->bs_allowed_stats_list,
						p_tpl->data.neighbor_info.earfcn);

				if (SON_HO_ALLOWED == ho_status)
				{
					is_ho_allowed_reqd = SON_TRUE;
				}
				else if (SON_HO_UNDEFINED == ho_status)
				{
					is_ho_blk_reqd   = SON_TRUE;
				}

				if (SON_PNULL != p_bs_allowed_record)
				{
					decrement_bs_allowed_nbr_count(p_cell_ctxt,
							p_bs_allowed_record,
							p_tpl->data.neighbor_info.broadcast_status,
							is_ho_allowed_reqd, is_ho_blk_reqd);
				}
				else
				{
                    /* SPR 22304 Fix Start */
					SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
							"Wrong Behaviour as Broadcast statistics node should be "
							"available till the neighbor cell [Cell id:0x%x] of the serving cell [Cell is:0x%x]"
							"for the EARFCN [%d]",
							son_convert_const_char_cell_id_to_int_cell_id(p_tpl->data.neighbor_info.nbr_cgi.cell_identity),
							son_convert_const_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
							p_tpl->data.neighbor_info.earfcn);
                    /* SPR 22304 Fix End */
				}

				anr_construct_rrm_intra_rat_ncl(&p_rrm_notification->intra_rat_ncl,
						&p_tpl->data, SON_TRUE);
			}
		}

		/*SPR_5193*/

		if (p_oam_notification)
		{
			p_oam_notification->nrt_op = SON_NRT_OP_DELETE;

			anr_construct_oam_updated_ncl(&p_oam_notification->nr_list,
					&p_tpl->data,
					SON_FALSE);
		}

		if (SON_FALSE == check_remove_list)
		{
			SON_MEMSET(&cell_id, 0, sizeof(cell_id));
			anr_convert_intra_rat_nr_to_global_cell_id_struct(&p_tpl->data.neighbor_info, &cell_id);

			if (SON_TRUE == anr_insert_cell_in_remove_list(
						&p_cell_ctxt->nr_remove_list, &cell_id))
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_INFO,
						"NR [Cell Id: 0x%x] Added in Remove List.",
						son_convert_const_char_cell_id_to_int_cell_id(p_tpl->data.neighbor_info.nbr_cgi.cell_identity));
			}
		}

		anr_clear_nr_nbrs_list(&p_tpl->data.anr_nr_nbr_list);
		list_init(&p_tpl->data.anr_nr_nbr_list);
		ANR_FREE_HASH_REC(anr_get_intra_rat_hash_rec_pool(), p_hash, p_tpl,
				p_hash_node);

        p_tpl = SON_PNULL;

		if (SON_TRUE == is_conflict_confusion_trigger)
		{
			delete_pci_suspect_conflict_confusion_handler(p_cell_ctxt, p_hash_rec);
		}

		/*SPR-12596-Start*/

		SON_MEMSET(&msg, 0, sizeof(son_anr_pci_conflict_confusion_ind_t));
		p_pci_list_node = anr_find_pci_in_conflict_confusion_pci_list(p_cell_ctxt, pci);

		if((p_pci_list_node != SON_PNULL) && (SON_TRUE == p_pci_list_node->pci_data.is_confused))
		{
			p_pci_list_node->pci_data.confusion_counter--;

			if(SON_NULL == p_pci_list_node->pci_data.confusion_counter)
			{

				p_pci_list_node->pci_data.is_confused = SON_FALSE;

				/*Send confusion clearance indication to OAM*/
				msg.src_cgi.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
				msg.cgi_1.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
				msg.cgi_2.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;

				SON_MEMCPY(&msg.src_cgi.intra_rat_global_cell_id,
						&p_cell_ctxt->cell_id,
						sizeof(son_intra_rat_global_cell_id_t));

				SON_MEMCPY(&msg.cgi_1.intra_rat_global_cell_id,
						&p_pci_list_node->pci_data.cgi_1,
						sizeof(son_intra_rat_global_cell_id_t));

				SON_MEMCPY(&msg.cgi_2.intra_rat_global_cell_id,
						&p_pci_list_node->pci_data.cgi_2,
						sizeof(son_intra_rat_global_cell_id_t));

				msg.pci = p_pci_list_node->pci_data.pci;
				msg.cause = SON_PCI_CONFUSION_CLEARED; 

				son_create_send_buffer((son_void_t *)&msg, SON_ANR_MODULE_ID, SON_APPS_MODULE_ID,
						SONANR_PCI_CONFLICT_CONFUSION_IND, sizeof(son_anr_pci_conflict_confusion_ind_t));

				if(SON_FALSE == p_pci_list_node->pci_data.is_in_conflict)
				{
					list_delete_node(&p_cell_ctxt->conflict_confusion_pci_list, &p_pci_list_node->anchor);
					son_mem_free(p_pci_list_node);
				}
			}
		}

		/*SPR-12596-End*/

		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_BRIEF,
				"anr_intra_rat_del_nr: NR [Cell id: 0x%x]"
				" is successfully deleted from the table\n",
				son_convert_char_cell_id_to_int_cell_id(p_nr_cell_id->cell_identity));

		SON_UT_TRACE_EXIT();
		return SON_SUCCESS;
	}

	SON_LOG(log_mode, p_son_anr_facility_name,
			SON_ERROR,
			"anr_intra_rat_del_nr: NR [Cell Id: 0x%x]"
			" not found in the table\n",
			son_convert_char_cell_id_to_int_cell_id(p_nr_cell_id->cell_identity));

	SON_UT_TRACE_EXIT();
	*p_error_code = SON_ERR_ANR_NR_NOT_FOUND;
	return SON_FAILURE;
} /* anr_intra_rat_del_nr */

/******************************************************************************
 * Function Name  : anr_inter_rat_del_nr
 * Inputs         : p_cell_ctxt  - Pointer to cell context
 *                  p_nr_cell_id - Pointer to nbr cell id
 *                  check_remove_list - Flag to check the remove list
 *                  p_oam_notification - Pointer to oam notification msg
 *                  p_rrm_notification - Pointer to rrm notification msg
 * Outputs        : p_error_code -  Pointer to a variable for storing error code
 *                : p_hash_rec - Pointer to a inter rat hash record variable 
 *
 * Returns        : son_return_et  - SON_SUCCESS The message is handled successfully
 *                                   SON_FAILURE The message is not handled successfully
 * Description    : This function is used for removing selected NR 
 *                  records for a cell configured from inter RAT hash table	  
 ******************************************************************************/
son_return_et
anr_inter_rat_del_nr
(
 anr_cell_context_t      *p_cell_ctxt,
 son_inter_rat_global_cell_id_t  *p_nr_cell_id,
 son_bool_et check_remove_list,
 son_error_et *p_error_code,
 son_void_t *p_hash_rec,
 son_anr_updated_nrt_info_t *p_oam_notification,
 son_anr_rrm_updated_nrt_info_t *p_rrm_notification
 )
{
	anr_inter_rat_hash_rec_t        hash_data    = {0};
	anr_inter_rat_hash_nr_tuple_t   *p_tpl       = SON_PNULL;
	SON_HASH_NODE                   *p_hash_node = SON_PNULL;
	son_oam_log_on_off_et           log_mode     = anr_get_log_mode();
	son_u32                         cell_identity = 0;
	son_inter_rat_neighbor_info_t   inter_rat_nr    = {0};
	son_global_cell_id_t            cell_id;
	SON_HASH *p_hash = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_nr_cell_id ||
			SON_PNULL == p_error_code || SON_PNULL == p_hash_rec)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_inter_rat_del_nr: NR could not be deleted,"
				"p_cell_ctxt or p_nr_cell_id is NULL\n");
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	p_hash = &p_cell_ctxt->inter_rat_neighbor_table;

	if (SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT & p_nr_cell_id->bitmask)
	{
		if (SON_UTRAN_CELL == p_nr_cell_id->utran_geran_global_cell_id.rat_type)
		{
			hash_data.t = ANR_UTRAN_NEIGHBOR;

			SON_MEMCPY(&hash_data.u.utran_neighbor_info.nbr_cgi,
					&p_nr_cell_id->utran_geran_global_cell_id, sizeof(son_utran_geran_global_cell_id_t));

			cell_identity = son_convert_char_cell_id_to_int_cell_id(p_nr_cell_id->utran_geran_global_cell_id.
					cell_identity);
		}
		else if (SON_GERAN_CELL == p_nr_cell_id->utran_geran_global_cell_id.rat_type)
		{
			hash_data.t = ANR_GERAN_NEIGHBOR;
			SON_MEMCPY(&hash_data.u.geran_neighbor_info.nbr_cgi,
					&p_nr_cell_id->utran_geran_global_cell_id, sizeof(son_utran_geran_global_cell_id_t));

			cell_identity = son_convert_char_cell_id_to_int_cell_id(
					p_nr_cell_id->utran_geran_global_cell_id.cell_identity);
		}
	}
	else if (SON_CDMA_GLOBAL_CELL_ID_PRESENT & p_nr_cell_id->bitmask)
	{
		hash_data.t = ANR_CDMA_NEIGHBOR;

		SON_MEMCPY(&hash_data.u.cdma_neighbor_info.nbr_cgi,
				&p_nr_cell_id->cdma_global_cell_id, sizeof(son_cdma_global_cell_id_t));
	}
	else
	{
		/* Populate the hash_data as per other RAT type in next release */
	}

	if (SON_PNULL != (p_hash_node =
				hash_find(p_hash, (son_void_t *)&hash_data)))
	{
		p_tpl = (anr_inter_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_inter_rat_hash_nr_tuple_t, anchor, p_hash_node);

		SON_MEMCPY((anr_inter_rat_hash_rec_t *)p_hash_rec,
				&p_tpl->data,
				sizeof(anr_inter_rat_hash_rec_t));

		if (p_oam_notification)
		{
			p_oam_notification->nrt_op = SON_NRT_OP_DELETE;
			anr_convert_inter_rat_hash_rec_to_inter_rat_nr(
					&p_tpl->data, &inter_rat_nr);

			anr_construct_oam_updated_ncl(&p_oam_notification->nr_list,
					&inter_rat_nr,
					SON_TRUE);
		}

		if (p_rrm_notification)
		{
			if (SON_TRUE == is_inter_rat_nr_sendable_to_rrm(&p_tpl->data))
			{
				if (ANR_UTRAN_NEIGHBOR == p_tpl->data.t)
				{
					anr_utran_freq_stats_node_t *p_list_node = SON_PNULL;
					SON_LIST *p_list = SON_PNULL;

					p_list = anr_determine_utran_freq_stats_list(
							p_cell_ctxt, p_tpl->data.u.utran_neighbor_info.pci);

					if (SON_PNULL != p_list)
					{
						p_list_node = anr_find_record_in_utran_freq_stats_list(
								p_list,
								p_tpl->data.u.utran_neighbor_info.uarfcn);
					}

					if (SON_PNULL != p_list_node)
					{
						decrement_utran_nbr_count_for_rrm(p_cell_ctxt, p_list_node, p_list);
					}
					else
					{
						/* SPR-612 Coverity Fix Starts */
						SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
								SON_ERROR,
								"anr_inter_rat_del_nr: Failed to find the broadcast "
								"statistic node in utran_freq_stats_list for given "
								"uarfcn value %u",
								p_tpl->data.u.utran_neighbor_info.uarfcn);
						/* SPR-612 Coverity Fix Ends */
					}

					anr_construct_rrm_inter_rat_ncl_for_utran_nbr(
							&p_rrm_notification->inter_rat_ncl.utran_neighbor_list,
							&p_tpl->data.u.utran_neighbor_info, SON_TRUE);

					if (SON_TRUE == is_utran_nbrs_available_in_rrm_notification(
								&p_rrm_notification->inter_rat_ncl.utran_neighbor_list))
					{
						p_rrm_notification->inter_rat_ncl.bitmask |= SON_RRM_UTRAN_NR_LIST_PRESENT;
					}
				}

				if (ANR_GERAN_NEIGHBOR == p_tpl->data.t)
				{
					anr_geran_freq_stats_node_t *p_list_node = SON_PNULL;
					anr_geran_freq_stats_data_t data;

					SON_MEMSET(&data, 0, sizeof(anr_geran_freq_stats_data_t));
					data.arfcn = p_tpl->data.u.geran_neighbor_info.arfcn;
					data.band_ind = p_tpl->data.u.geran_neighbor_info.band_ind;

					p_list_node = anr_find_record_in_geran_freq_stats_list(
							&p_cell_ctxt->geran_freq_stats_list, &data);

					if (SON_PNULL != p_list_node)
					{
						decrement_geran_nbr_count_for_rrm(p_cell_ctxt, p_list_node);
					}
					else
					{
						/* SPR-612 Coverity Fix Starts */
						SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
								SON_ERROR,
								"anr_inter_rat_del_nr: Failed to find the broadcast "
								"statistic node in geran_freq_stats_list for given frequency");
						/* SPR-612 Coverity Fix Ends */
					}

					anr_construct_rrm_inter_rat_ncl_for_geran_nbr(
							&p_rrm_notification->inter_rat_ncl.geran_neighbor_list,
							&p_tpl->data.u.geran_neighbor_info,
							SON_TRUE);

					if (SON_TRUE == is_geran_nbrs_available_in_rrm_notification(
								&p_rrm_notification->inter_rat_ncl.geran_neighbor_list))
					{
						p_rrm_notification->inter_rat_ncl.bitmask |= SON_RRM_GERAN_NR_LIST_PRESENT;
					}
				}

				if (p_tpl->data.t == ANR_CDMA_NEIGHBOR)
				{
					if (SON_CGI_1X_RTT_PRESENT &
							p_tpl->data.u.cdma_neighbor_info.nbr_cgi.bitmask) 
					{
						anr_cdma_xrtt_band_stats_node_t *p_list_xrtt_band_node = SON_PNULL;
						anr_cdma_xrtt_freq_stats_node_t *p_list_xrtt_freq_node = SON_PNULL;

						/*Searching for band in cell context*/
						p_list_xrtt_band_node = anr_find_record_in_cdma_xrtt_band_stats_list(
								&p_cell_ctxt->cdma_xrtt_band_stats_list,
								p_tpl->data.u.cdma_neighbor_info.band_class);

						/*Searching for freq in band stats list*/
						if (SON_PNULL != p_list_xrtt_band_node)
						{
							p_list_xrtt_freq_node = anr_find_record_in_cdma_xrtt_freq_stats_list(
									&p_list_xrtt_band_node->cdma_band_stats.freq_list,
									p_tpl->data.u.cdma_neighbor_info.arfcn);

							if (SON_PNULL != p_list_xrtt_freq_node)
							{
								decrement_cdma_xrtt_nbr_count_for_rrm(
										&p_tpl->data.u.cdma_neighbor_info,
										p_cell_ctxt);
							}
							else
							{
								/* SPR-612 Coverity Fix Starts */
								SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
										SON_ERROR,
										"anr_inter_rat_del_nr: Failed to find the broadcast "
										"statistic node in cdma_xrtt_freq_stats_list for "
										"given arfcn value %u",
										p_tpl->data.u.cdma_neighbor_info.arfcn);
								/* SPR-612 Coverity Fix Ends */
							}
						}
						else
						{
							/* SPR-612 Coverity Fix Starts */
							SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
									SON_ERROR,
									"anr_inter_rat_del_nr: Failed to find the broadcast "
									"statistic node in cdma_xrtt_band_stats_list for "
									"given band class %u",
									p_tpl->data.u.cdma_neighbor_info.band_class);
							/* SPR-612 Coverity Fix Ends */
						}

						anr_construct_rrm_inter_rat_ncl_for_cdma_xrtt_nbr(
								&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_xrtt,
								&p_tpl->data.u.cdma_neighbor_info, SON_TRUE);

						if (SON_TRUE == is_cdma_xrtt_nbrs_available_in_rrm_notification(
									&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_xrtt))
						{
							p_rrm_notification->inter_rat_ncl.bitmask |= SON_RRM_CDMA_NR_LIST_XRTT_PRESENT;
						}
					}
					else if (p_tpl->data.u.cdma_neighbor_info.nbr_cgi.bitmask
							& SON_CGI_HRPD_PRESENT) 
					{
						anr_cdma_hrpd_band_stats_node_t *p_list_hrpd_band_node = SON_PNULL;
						anr_cdma_hrpd_freq_stats_node_t *p_list_hrpd_freq_node = SON_PNULL;

						/*Searching for band in cell context*/
						p_list_hrpd_band_node = anr_find_record_in_cdma_hrpd_band_stats_list(
								&p_cell_ctxt->cdma_hrpd_band_stats_list,
								p_tpl->data.u.cdma_neighbor_info.band_class);

						/*Searching for freq in band stats list*/
						if (SON_PNULL != p_list_hrpd_band_node)
						{
							p_list_hrpd_freq_node = anr_find_record_in_cdma_hrpd_freq_stats_list(
									&p_list_hrpd_band_node->cdma_band_stats.freq_list,
									p_tpl->data.u.cdma_neighbor_info.arfcn);

							if (SON_PNULL != p_list_hrpd_freq_node)
							{
								decrement_cdma_hrpd_nbr_count_for_rrm(
										&p_tpl->data.u.cdma_neighbor_info,
										p_cell_ctxt);
							}
							else
							{
								/* SPR-612 Coverity Fix Starts */
								SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
										SON_ERROR,
										"anr_inter_rat_del_nr: Failed to find the broadcast "
										"statistic node in cdma_hrpd_freq_stats_list for "
										"given arfcn value %u",
										p_tpl->data.u.cdma_neighbor_info.arfcn);
								/* SPR-612 Coverity Fix Ends */
							}
						}
						else
						{
							/* SPR-612 Coverity Fix Starts */
							SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
									SON_ERROR,
									"anr_inter_rat_del_nr: Failed to find the broadcast "
									"statistic node in cdma_hrpd_band_stats_list for "
									"given band class %u",
									p_tpl->data.u.cdma_neighbor_info.band_class);
							/* SPR-612 Coverity Fix Ends */
						}

						anr_construct_rrm_inter_rat_ncl_for_cdma_hrpd_nbr(
								&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_hrpd,
								&p_tpl->data.u.cdma_neighbor_info, SON_TRUE);

						if (SON_TRUE == is_cdma_hrpd_nbrs_available_in_rrm_notification(
									&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_hrpd))
						{
							p_rrm_notification->inter_rat_ncl.bitmask |= SON_RRM_CDMA_NR_LIST_HRPD_PRESENT;
						}
					}
				}
			}
		}

		if (SON_FALSE == check_remove_list)
		{
			SON_MEMSET(&cell_id, 0, sizeof(cell_id));
			anr_extract_global_cell_id_from_inter_rat_hash_rec(&p_tpl->data, &cell_id);

			/* If the neighbor is present in Remove List, remove neighbor from REMOVE LIST */
			if (SON_TRUE == anr_insert_cell_in_remove_list(
						&p_cell_ctxt->nr_remove_list, &cell_id))
			{
				if (ANR_CDMA_NEIGHBOR != p_tpl->data.t)
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_ERROR,
							"anr_inter_rat_del_nr: NR [Cell Id: 0x%x] Added"
							" in Remove List.",
							cell_identity);
				}
				else
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_ERROR,
							"anr_inter_rat_del_nr: CDMA NR Added "
							"in Remove List.");
					anr_print_cdma_cell_id(&p_nr_cell_id->cdma_global_cell_id);
				}
			}
		}

		ANR_FREE_HASH_REC(anr_get_inter_rat_hash_rec_pool(),
				p_hash,
				p_tpl,
				p_hash_node);
        p_tpl = SON_PNULL;

		if ((p_nr_cell_id->bitmask) & (SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT))
		{
			SON_LOG(log_mode, p_son_anr_facility_name,
					SON_BRIEF,
					"anr_inter_rat_del_nr: NR [Cell id: 0x%x]"
					" is successfully deleted from the table\n",
					cell_identity);
		}
		else
		{
			SON_LOG(log_mode, p_son_anr_facility_name,
					SON_BRIEF,
					"anr_inter_rat_del_nr: CDMA NR is successfully "
					"deleted from the table\n");
			anr_print_cdma_cell_id(&p_nr_cell_id->cdma_global_cell_id);
		}

		SON_UT_TRACE_EXIT();
		return SON_SUCCESS;
	}

	if ((p_nr_cell_id->bitmask) & (SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT))
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_inter_rat_del_nr: NR [Cell Id: 0x%x]"
				" not found in the table\n",
				cell_identity);
	}
	else
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_inter_rat_del_nr: CDMA NR "
				"not found in the table\n");
		anr_print_cdma_cell_id(&p_nr_cell_id->cdma_global_cell_id);
	}

	SON_UT_TRACE_EXIT();
	*p_error_code = SON_ERR_ANR_NR_NOT_FOUND;
	return SON_FAILURE;
} /* anr_inter_rat_del_nr */

/******************************************************************************
 * Function Name  : anr_intra_rat_modify_nr
 * Inputs         : p_cell_ctxt - Pointer to cell context
 *                  p_nr_mdfd - Pointer to intra rat has record to be modified.
 *                  check_remove_list - Flag to check the remove list
 *                : p_nr   - Pointer to a structure 
 *                           containing the neighbors's cell id
 *                           and new values for it.This parameter also acts as
 *                           an ouput parameter also. It returns the value of the
 *                           NR before modification if modification was successful.
 *                  p_oam_notification_add - Pointer to oam notification if needs
 *                           to be send to to oam
 *                  p_rrm_notification_upd - Pointer to rrm notification msg due to
 *                           update operation on NRT
 *                  p_rrm_notification     - Pointer to rrm notification msg
 *                  is_conflict_confusion_trigger - Flag informing conflict\confusion
 *                  is_oam_triggered_modificationi - Flag informing whether oam notification
 *                           will be triggered
 * Outputs        : p_error_code -  Pointer to a variable for storing error code
 *
 * Returns        : anr_intra_rat_hash_rec_t* - Pointer to intra rat record modified
 * Description    : This function is used for updating selected NR
 *                  records for a cell configured
 ******************************************************************************/
anr_intra_rat_hash_rec_t *
anr_intra_rat_modify_nr
(
 anr_cell_context_t      *p_cell_ctxt,
 anr_intra_rat_hash_rec_t *p_nr_mdfd,
 son_bool_et check_remove_list,
 son_error_et *p_error_code,
 son_anr_updated_nrt_info_t *p_oam_notification_add,
 son_anr_updated_nrt_info_t *p_oam_notification_upd,
 son_anr_rrm_updated_nrt_info_t *p_rrm_notification,
 son_bool_et         is_conflict_confusion_trigger,
 son_bool_et         is_oam_triggered_modification
 )
{
	anr_intra_rat_hash_rec_t          hash_data;
	anr_intra_rat_hash_rec_t          temp_hash_data;
	anr_intra_rat_hash_rec_t          *p_nr;
	anr_intra_rat_hash_nr_tuple_t     *p_tpl                     = SON_PNULL;
	anr_intra_rat_hash_rec_t 			*p_ret_val          	 = SON_PNULL;
	SON_HASH_NODE						*p_node	                 = SON_PNULL;
	son_bool_et						is_update_required           = SON_FALSE;
	son_oam_log_on_off_et        		log_mode        = anr_get_log_mode();
	son_u32                        	bitmask                      = 0;
	son_bool_et                       is_pci_changed             = SON_FALSE;
	SON_HASH *p_hash                                             = SON_PNULL;
	son_bool_et      is_rrm_params_changed                       = SON_FALSE;
	son_error_et     err_code                                    = SON_NO_ERROR;
	son_bool_et  is_old_nr_send                                  = SON_FALSE;
	son_bool_et  is_new_nr_sendable                              = SON_FALSE;
	son_bool_et is_rrm_notif_reqd_for_new_nr                     = SON_FALSE;
	son_ho_status_et old_nr_ho_status                            = SON_HO_UNDEFINED;
	son_ho_status_et new_nr_ho_status                            = SON_HO_UNDEFINED;
	son_x2_status_et nr_x2_status                                = SON_X2_ALLOWED_DISCONNECTED;
	son_bool_et      is_sus_pci_changed                          = SON_FALSE;
	son_bool_et      oam_notif_reqd                              = SON_FALSE;
	son_bool_et      is_new_nr_changed_from_old_nr_for_notif     = SON_TRUE;
	SON_LIST anr_nr_nbr_list;



	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_nr_mdfd || SON_PNULL == p_error_code)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_intra_rat_modify_nr: NR could not be modified,"
				" p_cell_ctxt or p_nr_cell_id is NULL; check_remove_list=%u p_oam_notification_add=%p\n",
                check_remove_list,p_oam_notification_add);
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if ((p_nr_mdfd->neighbor_info.bitmask & SON_SUSPECT_PCI_PRESENT) ||
			(p_nr_mdfd->neighbor_info.bitmask & SON_EUTRAN_NR_HO_STATS_PRESENT) ||
			(p_nr_mdfd->neighbor_info.bitmask & SON_ENABLE_X2_STATUS_PRESENT))
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_intra_rat_modify_nr: NR [Cell Id: 0x%x] could not be modified, "
				"as read only fields are present (suspect_pci, ho_stats,x2_status)\n",
				son_convert_char_cell_id_to_int_cell_id(p_nr_mdfd->neighbor_info.nbr_cgi.cell_identity));
		*p_error_code = SON_ERR_INVALID_PARAMS;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}


	p_hash = &p_cell_ctxt->intra_rat_neighbor_table;

	SON_MEMSET(&hash_data, 0, sizeof(anr_intra_rat_hash_rec_t));
	SON_MEMSET(&temp_hash_data, 0, sizeof(anr_intra_rat_hash_rec_t));

	list_init(&anr_nr_nbr_list);

	p_nr = &temp_hash_data;

	SON_MEMCPY(&hash_data.neighbor_info.nbr_cgi, &p_nr_mdfd->neighbor_info.nbr_cgi, 
			sizeof(son_intra_rat_global_cell_id_t));

	if (SON_PNULL != (p_node = hash_find(p_hash, (son_void_t *)&hash_data)))
	{
		p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_intra_rat_hash_nr_tuple_t, anchor, p_node);

		/*SPR-10602-Start*/
		if(!(p_nr_mdfd->neighbor_info.bitmask & SON_EARFCN_PRESENT))
		{
			p_nr_mdfd->neighbor_info.bitmask |= SON_EARFCN_PRESENT;
			p_nr_mdfd->neighbor_info.earfcn = p_tpl->data.neighbor_info.earfcn;
		}

		if(!(p_nr_mdfd->neighbor_info.bitmask & SON_PCI_PRESENT))
		{
			p_nr_mdfd->neighbor_info.bitmask |= SON_PCI_PRESENT;
			p_nr_mdfd->neighbor_info.pci = p_tpl->data.neighbor_info.pci;
		}

		/*SPR-10230-Start*/

		if(((is_oam_triggered_modification == SON_TRUE)&&(p_nr_mdfd->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT))||
				((p_nr_mdfd->enb_id.enb_type == SON_MACRO_ENB)&&(p_nr_mdfd->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT)&&
				 (p_nr_mdfd->neighbor_info.access_mode == SON_OPEN)))
		{
			SON_LOG(log_mode, p_son_anr_facility_name,
					SON_ERROR,
					"anr_intra_rat_modify_nr:For NR [Cell Id: 0x%x]"
					"access mode is not going to be calculated on pci range basis"
					"as either it is being modified by OAM or has come in link up indication"
					"or eNB config update and belongs to a macro eNB\n",
					son_convert_char_cell_id_to_int_cell_id(p_nr_mdfd->neighbor_info.nbr_cgi.cell_identity));
		}
		else  
		{
			if(SON_FALSE == anr_set_correct_access_mode_of_intra_rat_nbr(p_nr_mdfd)) /*SPR-10426-Start-If clause added*/
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_ERROR,
						"anr_intra_rat_modify_nr:NR [Cell Id: 0x%x]"
						"cannot be modified\n",
						son_convert_char_cell_id_to_int_cell_id(p_nr_mdfd->neighbor_info.nbr_cgi.cell_identity));

				*p_error_code = SON_ERR_INVALID_PARAMS;
				SON_UT_TRACE_EXIT();
				return SON_PNULL;
			} /*SPR-10426-End*/
            /*SPR 15799 Fix Start*/
            if(!(p_nr_mdfd->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT))
            {
                SON_LOG(log_mode, p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_intra_rat_modify_nr:NR [Cell Id: 0x%x]"
                        "cannot be modified as access mode is not available\n",
                        son_convert_char_cell_id_to_int_cell_id(p_nr_mdfd->neighbor_info.nbr_cgi.cell_identity));

                *p_error_code = SON_ERR_INVALID_PARAMS;
                SON_UT_TRACE_EXIT();
                return SON_PNULL;
            }
            /*SPR 15799 Fix Stop*/    
		}

		/*SPR-10230-End*/

		if (SON_FALSE == anr_validate_csg_id_access_mode_for_eutran_neighbor(p_nr_mdfd))
		{
			SON_LOG(log_mode, p_son_anr_facility_name,
					SON_ERROR,
					"anr_intra_rat_modify_nr: NR [Cell Id: 0x%x] could not be modified, "
					"As input data is at invalid paramas\n",
					son_convert_char_cell_id_to_int_cell_id(p_nr_mdfd->neighbor_info.nbr_cgi.cell_identity));
			*p_error_code = SON_ERR_INVALID_PARAMS;
			SON_UT_TRACE_EXIT();
			return SON_PNULL;
		}

		/*SPR-10602-End*/

		/*SPR-10230-Start*/
		if((p_nr_mdfd->enb_id.enb_type != SON_HOME_ENB)&&(p_nr_mdfd->enb_id.enb_type != SON_MACRO_ENB))
		{
			SON_MEMSET(&p_nr_mdfd->enb_id, 0, sizeof(son_global_enb_id_t));
			anr_determine_enb_id(p_nr_mdfd,
					&p_nr_mdfd->enb_id);

			SON_LOG(log_mode, p_son_anr_facility_name,
					SON_ERROR, "anr_intra_rat_modify_nr: For NR [Cell Id: 0x%x]" 
					"eNB id determined successfully\n",
					son_convert_char_cell_id_to_int_cell_id(p_nr_mdfd->neighbor_info.nbr_cgi.cell_identity));
		}
		/*SPR-10230-End*/

		/* Rel 3.0 Multi Sector code changes Start */
         /*SPR 17773 Start*/
		if ((SON_LOCALLY_CONNECTED_CELL != p_tpl->data.neighbor_info.x2_status) &&
               (SON_PNULL ==  list_find_node(&g_anr_context.cell_ctxt_list,
               &p_tpl->data.neighbor_info.nbr_cgi,son_key_of_anr,son_compare_cell_id)))
         /*SPR 17773 End*/
		{
			get_the_x2_status_value(&p_tpl->data, p_nr_mdfd, p_error_code, &nr_x2_status);
		}
		else
		{
			nr_x2_status = SON_LOCALLY_CONNECTED_CELL;
		}
		/* Rel 3.0 Multi Sector code changes Stop */

		if (*p_error_code != SON_NO_ERROR)
		{
			SON_LOG(log_mode, p_son_anr_facility_name,
					SON_ERROR, "anr_intra_rat_modify_nr: NR [Cell Id: 0x%x]"
					" could not be modified\n\n",
					son_convert_char_cell_id_to_int_cell_id(p_nr_mdfd->neighbor_info.nbr_cgi.cell_identity));
			SON_UT_TRACE_EXIT();
			return SON_PNULL;
		}

		/*Placing new data to the p_nr structure.This is a shallow copy*/
		SON_MEMCPY(p_nr, p_nr_mdfd, sizeof(anr_intra_rat_hash_rec_t));

		/*Placing previous data to the p_nr_mdfd.This is a shallow copy*/
		SON_MEMCPY(p_nr_mdfd, &p_tpl->data, sizeof(anr_intra_rat_hash_rec_t));
		/*Resetting the link list header in the p_nr_mdfd*/
		list_init(&p_nr_mdfd->anr_nr_nbr_list);

		/*Placing the link list header in the local structure*/
		SON_MEMCPY(&anr_nr_nbr_list, &p_tpl->data.anr_nr_nbr_list, sizeof(anr_nr_nbr_list));

		/*get the X2 status and then assigning the value to it.It will be 
		 * utilized in the modification for the x2 status when the linkup
		 * indication and the link down indication comes*/
		p_nr->neighbor_info.bitmask |= SON_ENABLE_X2_STATUS_PRESENT; 
		p_nr->neighbor_info.x2_status = nr_x2_status;


		/*Do the modification of the existing tuple*/
		bitmask = p_nr->neighbor_info.bitmask & SON_PCI_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					(p_tpl->data.neighbor_info.pci != p_nr->neighbor_info.pci)))
		{
			p_tpl->data.neighbor_info.pci = p_nr->neighbor_info.pci;
			if ((p_tpl->data.neighbor_info.bitmask & SON_SUSPECT_PCI_PRESENT) &&
					SON_TRUE == p_tpl->data.neighbor_info.suspected_pci)
			{
				p_tpl->data.neighbor_info.suspected_pci = SON_FALSE;
				is_sus_pci_changed  = SON_TRUE;         
				p_tpl->data.ho_failed_for_pci_conflict = 0;
			}

			is_update_required = SON_TRUE;
			is_pci_changed = SON_TRUE;
			is_rrm_params_changed = SON_TRUE;
		}

		bitmask = p_nr->neighbor_info.bitmask & SON_CELL_OFFSET_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					(p_tpl->data.neighbor_info.cell_specific_offset != 
					 p_nr->neighbor_info.cell_specific_offset)))
		{
			p_tpl->data.neighbor_info.cell_specific_offset
				= p_nr->neighbor_info.cell_specific_offset;
			is_update_required = SON_TRUE;
			is_rrm_params_changed = SON_TRUE;
		}

		bitmask = p_nr->neighbor_info.bitmask & SON_TAC_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					SON_MEMCMP(p_nr->neighbor_info.tac, p_tpl->data.neighbor_info.tac,
						sizeof(p_nr->neighbor_info.tac))))
		{
			SON_MEMCPY(p_tpl->data.neighbor_info.tac, p_nr->neighbor_info.tac,
					sizeof(p_tpl->data.neighbor_info.tac));
			is_update_required = SON_TRUE;
			is_rrm_params_changed = SON_TRUE;
		}

		bitmask = p_nr->neighbor_info.bitmask & SON_PLMN_ID_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					SON_MEMCMP(p_nr->neighbor_info.plmn_id, p_tpl->data.neighbor_info.plmn_id,
						sizeof(p_nr->neighbor_info.plmn_id))))
		{
			p_tpl->data.neighbor_info.num_plmn_id = p_nr->neighbor_info.num_plmn_id;
			SON_MEMCPY(p_tpl->data.neighbor_info.plmn_id, p_nr->neighbor_info.plmn_id,
					sizeof(p_tpl->data.neighbor_info.plmn_id));
			is_update_required = SON_TRUE;
		}

		bitmask = p_nr->neighbor_info.bitmask &  SON_INTRA_CSG_ID_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					SON_MEMCMP(p_nr->neighbor_info.csg_identity, p_tpl->data.neighbor_info.csg_identity,
						sizeof(p_nr->neighbor_info.csg_identity))))
		{
			SON_MEMCPY(p_tpl->data.neighbor_info.csg_identity, p_nr->neighbor_info.csg_identity,
					sizeof(p_tpl->data.neighbor_info.csg_identity));
			is_update_required = SON_TRUE;
			is_rrm_params_changed = SON_TRUE;
		}

		bitmask = p_nr->neighbor_info.bitmask & SON_EARFCN_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					(p_nr->neighbor_info.earfcn != p_tpl->data.neighbor_info.earfcn)))
		{
			p_tpl->data.neighbor_info.earfcn = p_nr->neighbor_info.earfcn;
			is_update_required = SON_TRUE;
			SON_MEMSET(p_tpl->data.intra_freq_neighbor_list, 0, sizeof(p_tpl->data.intra_freq_neighbor_list));
			p_tpl->data.num_intra_freq_neighbor = 0;
			is_pci_changed = SON_TRUE;
			if ((p_tpl->data.neighbor_info.bitmask & SON_SUSPECT_PCI_PRESENT) &&
					SON_TRUE == p_tpl->data.neighbor_info.suspected_pci)
			{
				p_tpl->data.neighbor_info.suspected_pci = SON_FALSE;
				is_sus_pci_changed  = SON_TRUE;
				p_tpl->data.ho_failed_for_pci_conflict = 0;
			}

			is_rrm_params_changed = SON_TRUE;
		}

		bitmask = p_nr->neighbor_info.bitmask & SON_RSRP_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					(p_nr->neighbor_info.rsrp != p_tpl->data.neighbor_info.rsrp)))
		{
			p_tpl->data.neighbor_info.rsrp = p_nr->neighbor_info.rsrp;
			is_update_required = SON_TRUE;
		}

		bitmask = p_nr->neighbor_info.bitmask & SON_RSRQ_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					(p_nr->neighbor_info.rsrq != p_tpl->data.neighbor_info.rsrq)))
		{
			p_tpl->data.neighbor_info.rsrq = p_nr->neighbor_info.rsrq;
			is_update_required = SON_TRUE;
		}

		bitmask = p_nr->neighbor_info.bitmask & SON_ENABLE_NR_STATUS_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask)
					|| (p_nr->neighbor_info.nr_status != p_tpl->data.neighbor_info.nr_status)))
		{
			p_tpl->data.neighbor_info.nr_status = p_nr->neighbor_info.nr_status;
			is_update_required = SON_TRUE;
		}

		bitmask = p_nr->neighbor_info.bitmask & SON_ENABLE_HO_STATUS_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					(p_nr->neighbor_info.ho_status != p_tpl->data.neighbor_info.ho_status)))
		{
			p_tpl->data.neighbor_info.ho_status = p_nr->neighbor_info.ho_status;
			is_update_required = SON_TRUE;
		}

		bitmask = p_nr->neighbor_info.bitmask & SON_ENABLE_X2_STATUS_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					(p_nr->neighbor_info.x2_status != p_tpl->data.neighbor_info.x2_status)))
		{
			p_tpl->data.neighbor_info.x2_status = p_nr->neighbor_info.x2_status;
			is_update_required = SON_TRUE;
			is_rrm_params_changed = SON_TRUE;
		}

		if (p_nr->num_intra_freq_neighbor &&
				SON_MEMCMP(p_nr->intra_freq_neighbor_list,
					p_tpl->data.intra_freq_neighbor_list,
					sizeof(p_nr->intra_freq_neighbor_list)))
		{
			p_tpl->data.num_intra_freq_neighbor = p_nr->num_intra_freq_neighbor;
			SON_MEMCPY(p_tpl->data.intra_freq_neighbor_list,
					p_nr->intra_freq_neighbor_list,
					sizeof(p_nr->intra_freq_neighbor_list));
		}

		bitmask = p_nr->neighbor_info.bitmask & SON_CM_CELL_OFFSET_PRESENT; 
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					(p_nr->neighbor_info.cm_cell_specific_offset !=
					 p_tpl->data.neighbor_info.cm_cell_specific_offset)))
		{
			p_tpl->data.neighbor_info.cm_cell_specific_offset = p_nr->neighbor_info.cm_cell_specific_offset;
			is_update_required = SON_TRUE;
			is_rrm_params_changed = SON_TRUE;
		}

		bitmask = p_nr->neighbor_info.bitmask & SON_BROADCAST_STATUS_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					(p_nr->neighbor_info.broadcast_status != p_tpl->data.neighbor_info.broadcast_status)))
		{
			p_tpl->data.neighbor_info.broadcast_status = p_nr->neighbor_info.broadcast_status;
			is_update_required = SON_TRUE;
			is_rrm_params_changed = SON_TRUE;
		}

		bitmask = p_nr->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					(p_nr->neighbor_info.access_mode != p_tpl->data.neighbor_info.access_mode)))
		{
			p_tpl->data.neighbor_info.access_mode = p_nr->neighbor_info.access_mode;
			is_update_required = SON_TRUE;
			is_rrm_params_changed = SON_TRUE;
		}

		/*handling for the PRACH configuraiton :starts*/
		bitmask = p_nr->neighbor_info.bitmask & SON_UL_EARFCN_PRESENT;
		if(bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					( p_nr->neighbor_info.ul_earfcn != p_tpl->data.neighbor_info.ul_earfcn)) )
		{
			p_tpl->data.neighbor_info.ul_earfcn = p_nr->neighbor_info.ul_earfcn;
			is_update_required = SON_TRUE;
		}

		bitmask = p_nr->neighbor_info.bitmask & SON_PRACH_CONFIGURATION_PRESENT;
		if(bitmask)
		{
			do{
				if(!(p_tpl->data.neighbor_info.bitmask & bitmask))
				{
					SON_MEMCPY(&p_tpl->data.neighbor_info.prach_config,
							&p_nr->neighbor_info.prach_config,
							sizeof(p_tpl->data.neighbor_info.prach_config));
					is_update_required = SON_TRUE;
					break;
				}
				if(p_tpl->data.neighbor_info.prach_config.root_sequence_index !=
						p_nr->neighbor_info.prach_config.root_sequence_index)
				{
					p_tpl->data.neighbor_info.prach_config.root_sequence_index =
						p_nr->neighbor_info.prach_config.root_sequence_index;
					is_update_required = SON_TRUE;
				}
				if(p_tpl->data.neighbor_info.prach_config.zero_correlation_zone_config !=
						p_nr->neighbor_info.prach_config.zero_correlation_zone_config)
				{
					p_tpl->data.neighbor_info.prach_config.zero_correlation_zone_config =
						p_nr->neighbor_info.prach_config.zero_correlation_zone_config;
					is_update_required = SON_TRUE;
				}
				if((p_tpl->data.neighbor_info.prach_config.high_speed_flag !=
							p_nr->neighbor_info.prach_config.high_speed_flag))
				{
					p_tpl->data.neighbor_info.prach_config.high_speed_flag =
						p_nr->neighbor_info.prach_config.high_speed_flag;
					is_update_required = SON_TRUE;
				}
				if((p_tpl->data.neighbor_info.prach_config.prach_frequency_offset !=
							p_nr->neighbor_info.prach_config.prach_frequency_offset))
				{
					p_tpl->data.neighbor_info.prach_config.prach_frequency_offset =
						p_nr->neighbor_info.prach_config.prach_frequency_offset;
					is_update_required = SON_TRUE;
				}

				bitmask = p_nr->neighbor_info.prach_config.bitmask & SON_PRACH_CONFIG_INDEX_PRESENT;
				if(bitmask && (  !(p_tpl->data.neighbor_info.prach_config.bitmask) ||
							(p_tpl->data.neighbor_info.prach_config.prach_configuration_index !=
							 p_nr->neighbor_info.prach_config.prach_configuration_index) ) )
				{
					p_tpl->data.neighbor_info.prach_config.prach_configuration_index =
						p_nr->neighbor_info.prach_config.prach_configuration_index ;
					p_tpl->data.neighbor_info.prach_config.bitmask = SON_PRACH_CONFIG_INDEX_PRESENT;
					is_update_required = SON_TRUE;
				}

			}while(0);
		}
		/*handling for the PRACH configuraiton :stops*/
		/* SPR 14369 Fix start */
		bitmask = p_nr->neighbor_info.bitmask & SON_EICIC_INFO_PRESENT;
		if(bitmask)
		{

			do{
				if(!(p_tpl->data.neighbor_info.bitmask & bitmask))
				{
					SON_MEMCPY(&p_tpl->data.neighbor_info.eicic_info,
							&p_nr->neighbor_info.eicic_info,
							sizeof(p_tpl->data.neighbor_info.eicic_info));
					is_update_required = SON_TRUE;
					break;
				}
				bitmask = p_nr->neighbor_info.eicic_info.bitmask & SON_CELL_INTF_SCHEME_TYPE_PRESENT;
				if(bitmask && (  !(p_tpl->data.neighbor_info.eicic_info.bitmask) ||
							(p_tpl->data.neighbor_info.eicic_info.interference_scheme !=
							 p_nr->neighbor_info.eicic_info.interference_scheme) ) )
				{
					p_tpl->data.neighbor_info.eicic_info.interference_scheme =
						p_nr->neighbor_info.eicic_info.interference_scheme ;
					p_tpl->data.neighbor_info.eicic_info.bitmask = SON_CELL_INTF_SCHEME_TYPE_PRESENT;
					is_update_required = SON_TRUE;
					/*SPR 14401 fix start */
					is_rrm_params_changed = SON_TRUE;
					/*SPR 14401 fix end */
				}
				bitmask = p_nr->neighbor_info.eicic_info.bitmask & SON_CELL_ABS_USAGE_THRESHOLD_PRESENT;
				if(bitmask && (!(p_tpl->data.neighbor_info.eicic_info.bitmask)))
				{
					bitmask = p_nr->neighbor_info.eicic_info.abs_usage_threshold.bitmask & SON_CELL_MIN_ABS_USAGE_THRESHOLD_PRESENT;
					if(bitmask && (!(p_tpl->data.neighbor_info.eicic_info.abs_usage_threshold.bitmask) ||
								(p_tpl->data.neighbor_info.eicic_info.abs_usage_threshold.min_abs_usage_threshold !=
								 p_nr->neighbor_info.eicic_info.abs_usage_threshold.min_abs_usage_threshold)))
					{
						p_tpl->data.neighbor_info.eicic_info.abs_usage_threshold.min_abs_usage_threshold =
							p_nr->neighbor_info.eicic_info.abs_usage_threshold.min_abs_usage_threshold;
						p_tpl->data.neighbor_info.eicic_info.bitmask = SON_CELL_ABS_USAGE_THRESHOLD_PRESENT;
						p_tpl->data.neighbor_info.eicic_info.abs_usage_threshold.bitmask = SON_CELL_MIN_ABS_USAGE_THRESHOLD_PRESENT;
						is_update_required = SON_TRUE;
						/*SPR 14401 fix start */
						is_rrm_params_changed = SON_TRUE;
						/*SPR 14401 fix end */
					}
					bitmask = p_nr->neighbor_info.eicic_info.abs_usage_threshold.bitmask & SON_CELL_MAX_ABS_USAGE_THRESHOLD_PRESENT;
					if(bitmask && (!(p_tpl->data.neighbor_info.eicic_info.abs_usage_threshold.bitmask) ||
								(p_tpl->data.neighbor_info.eicic_info.abs_usage_threshold.max_abs_usage_threshold !=
								 p_nr->neighbor_info.eicic_info.abs_usage_threshold.max_abs_usage_threshold)))
					{
						p_tpl->data.neighbor_info.eicic_info.abs_usage_threshold.max_abs_usage_threshold =
							p_nr->neighbor_info.eicic_info.abs_usage_threshold.max_abs_usage_threshold;
						p_tpl->data.neighbor_info.eicic_info.bitmask = SON_CELL_ABS_USAGE_THRESHOLD_PRESENT;
						p_tpl->data.neighbor_info.eicic_info.abs_usage_threshold.bitmask = SON_CELL_MAX_ABS_USAGE_THRESHOLD_PRESENT;
						is_update_required = SON_TRUE;
						/*SPR 14401 fix start */
						is_rrm_params_changed = SON_TRUE;
						/*SPR 14401 fix end */
					}
				}
			}while(0);
		}
		/* SPR 14369 Fix end */
 #ifdef LTE_EMBMS_SUPPORTED
		bitmask = p_nr->neighbor_info.bitmask &  SON_EMBMS_MBMSFN_INFO_PRESENT;
		if (bitmask && (!(p_tpl->data.neighbor_info.bitmask & bitmask) ||
					SON_MEMCMP(&(p_nr->neighbor_info.mbms_config),&(p_tpl->data.neighbor_info.mbms_config),
						sizeof(son_mbms_configuration_t))))
		{
			SON_MEMCPY(&(p_tpl->data.neighbor_info.mbms_config), &(p_nr->neighbor_info.mbms_config),
					sizeof(son_mbms_configuration_t));


            /* SPR 18605:start */
            p_tpl->data.neighbor_info.bitmask |= SON_EMBMS_MBMSFN_INFO_PRESENT;
            /* SPR 18605:end */
			is_update_required = SON_TRUE;
			is_rrm_params_changed = SON_TRUE;
		}
#endif
		p_tpl->data.neighbor_info.bitmask |= p_nr->neighbor_info.bitmask;

        /*SPR 16372 fix start*/
        if((SON_OPEN_FEMTO == p_tpl->data.neighbor_info.access_mode)
                &&(p_tpl->data.neighbor_info.bitmask & SON_INTRA_CSG_ID_PRESENT))
        {
            p_tpl->data.neighbor_info.bitmask &= ~(SON_INTRA_CSG_ID_PRESENT);
        }
        /*SPR 16372 fix stop*/

		/*Mark the last actitiy timer*/
		son_time(&p_tpl->data.last_activity_ts);

		/*If no updation is required*/
		if (SON_FALSE == is_update_required)
		{
			SON_LOG(log_mode, p_son_anr_facility_name, SON_BRIEF,
					"anr_intra_rat_modify_nr: NR [Cell id: 0x%x]"
					" does not require modification\n",
					son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));

			*p_error_code = SON_NO_ERROR;
			SON_UT_TRACE_EXIT();
			return p_ret_val;
		}


		/*Determine if the changes done on the CSG identitiy
		 * and the Access Mode does not bring any
		 * improper condition for the ETRAN neighbor*/
		if ((SON_FALSE == anr_validate_csg_id_access_mode_for_eutran_neighbor(&p_tpl->data)))
		{
			err_code = SON_ERR_INVALID_PARAMS;
		}    
		else
		{
			/*Check if the OLd Nr was send to RRM*/
			is_old_nr_send = is_intra_rat_nbr_notifiable_to_rrm(&p_nr_mdfd->neighbor_info); 
			old_nr_ho_status = anr_get_ho_status_of_eutran_nr(&p_nr_mdfd->neighbor_info); 

			/*Check if the New NR is sendable to RRM */
			is_new_nr_sendable = is_intra_rat_nbr_notifiable_to_rrm(&p_tpl->data.neighbor_info);  
			new_nr_ho_status  = anr_get_ho_status_of_eutran_nr(&p_tpl->data.neighbor_info);

			/*If the new NR is having all the params for RRM present and atleast 1 of it has changed*/
			if (SON_FALSE == is_rrm_params_changed && old_nr_ho_status == new_nr_ho_status)
			{
				is_new_nr_changed_from_old_nr_for_notif = SON_FALSE;
			}
			else if (SON_TRUE == is_new_nr_sendable)
			{
				is_rrm_notif_reqd_for_new_nr  = SON_TRUE;    
			}

			/*Determining if we need to roll back the changes or keep it.
			 * Also if the notifcation to RRM regarding the OLD Neighbor
			 * or the new neighbor to be send */
			if (SON_TRUE ==  is_new_nr_changed_from_old_nr_for_notif)
			{
				anr_determine_the_changes_in_nrt(p_cell_ctxt,
						p_nr_mdfd,
						&p_tpl->data,
						&is_old_nr_send,
						&is_rrm_notif_reqd_for_new_nr,
						&err_code);
			}
		}

		/*If the err_code is to roll back the changes*/
		if (SON_NO_ERROR !=  err_code)
		{
			/*Get the changes back*/
			SON_MEMCPY(&p_tpl->data, p_nr_mdfd,
					sizeof(anr_intra_rat_hash_rec_t));
			/*Get the Link list pointer back*/
			SON_MEMCPY(&p_tpl->data.anr_nr_nbr_list,
					&anr_nr_nbr_list, sizeof(p_tpl->data.anr_nr_nbr_list));

			/*Get the time stamping done*/
			son_time(&p_tpl->data.last_activity_ts);

			SON_LOG(log_mode, p_son_anr_facility_name, SON_BRIEF,
					"anr_intra_rat_modify_nr: NR [Cell Id: 0x%x] can not be modified\n",
					son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));

			*p_error_code = err_code;
			SON_UT_TRACE_EXIT();
			return p_ret_val;
		}

		*p_error_code = SON_NO_ERROR;
		if (is_update_required == SON_TRUE)
		{
			p_ret_val = &p_tpl->data;
			SON_LOG(log_mode, p_son_anr_facility_name, SON_BRIEF,
					"anr_intra_rat_modify_nr: NR [Cell Id: 0x%x] modified successfully\n",
					son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));
		}

		/*Do the OAM notifcation if update has happened*/
		if (p_oam_notification_upd)
		{
			if ((SON_TRUE == is_oam_triggered_modification && SON_TRUE == is_sus_pci_changed) ||
					(SON_FALSE == is_oam_triggered_modification && SON_TRUE == is_update_required))
			{
				oam_notif_reqd = SON_TRUE;
			}

			if (SON_TRUE == oam_notif_reqd)
			{
				p_oam_notification_upd->nrt_op = SON_NRT_OP_UPDATE;

				anr_construct_oam_updated_ncl(&p_oam_notification_upd->nr_list,
						&p_tpl->data,
						SON_FALSE);
			}
		}

		if (SON_TRUE == is_new_nr_changed_from_old_nr_for_notif)
		{
			/*Include the new nr in the rrm notifcation if required,in add mod list*/
			if (p_rrm_notification && SON_TRUE == is_rrm_notif_reqd_for_new_nr)
			{
				anr_construct_rrm_intra_rat_ncl(&p_rrm_notification->intra_rat_ncl, &p_tpl->data, SON_FALSE);
			}

			/*Include the old  nr in the rrm notifcation if required*/
			if (p_rrm_notification && SON_TRUE == is_old_nr_send)
			{
				anr_construct_rrm_intra_rat_ncl(&p_rrm_notification->intra_rat_ncl, p_nr_mdfd, SON_TRUE);
			}
		}

		if (SON_TRUE == is_pci_changed && SON_TRUE == is_conflict_confusion_trigger)
		{
			/*handling for old hash record*/
			delete_pci_suspect_conflict_confusion_handler(p_cell_ctxt, p_nr_mdfd);
			/*handling for the new hash record*/
			insert_pci_suspect_conflict_confusion_handler(p_cell_ctxt, &p_tpl->data,
					p_oam_notification_upd);
		}

		SON_UT_TRACE_EXIT();
		return p_ret_val;
	}

	SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
			"anr_intra_rat_modify_nr: NR [Cell Id: 0x%x]"
			" not present in the table\n",
			son_convert_char_cell_id_to_int_cell_id(p_nr_mdfd->neighbor_info.nbr_cgi.cell_identity));

	*p_error_code = SON_ERR_ANR_NR_NOT_FOUND;

	SON_UT_TRACE_EXIT();
	return p_ret_val;
} /* anr_intra_rat_modify_nr */

/******************************************************************************
 * Function Name  : check_anr_utran_update_status
 * Inputs         : p_new_nr_info - Pointer to the new UTRAN neighbor info
 *                  received in update request message.
 *                  p_is_rrm_notif_reqd - Flag that informs whether notification
 *                                        to rrm will be send
 * Outputs        : p_tbl_nr_info -  Updated UTRAN neighbor info structure
 *
 * Returns        : son_bool_et   - SON_TRUE If updation is required
 *                  SON_FALSE - If updation is not required
 * Description    : This function is used to check if UTRAN neighbor info is to
 *                  be updated or not in the inter RAT NR hash table
 ******************************************************************************/
static son_bool_et
check_anr_utran_update_status
(
 son_utran_neighbor_info_t *p_tbl_nr_info,
 const son_utran_neighbor_info_t *p_new_nr_info,
 son_bool_et         *p_is_rrm_notif_reqd
 )
{
	son_bool_et is_update_required = SON_FALSE;
	son_oam_log_on_off_et log_mode = anr_get_log_mode();
	son_u32 bitmask = 0;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_tbl_nr_info || SON_PNULL == p_new_nr_info || SON_PNULL == p_is_rrm_notif_reqd)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"check_anr_utran_update_status: NR could not be modified, "
				"p_tbl_nr_info or p_new_nr_info is NULL\n");
		SON_UT_TRACE_EXIT();
		return is_update_required;
	}

	*p_is_rrm_notif_reqd = SON_FALSE; 

	bitmask = SON_UTRAN_PCI_PRESENT & p_new_nr_info->bitmask;
	if (bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				SON_MEMCMP(&p_tbl_nr_info->pci, &p_new_nr_info->pci, 
					sizeof(p_tbl_nr_info->pci))))
	{
		SON_MEMCPY(&p_tbl_nr_info->pci, &p_new_nr_info->pci,
				sizeof(p_tbl_nr_info->pci));
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	bitmask = SON_UTRAN_CSO_PRESENT & p_new_nr_info->bitmask;
	if (bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				(p_tbl_nr_info->cell_specific_offset != 
				 p_new_nr_info->cell_specific_offset)))
	{
		p_tbl_nr_info->cell_specific_offset 
			= p_new_nr_info->cell_specific_offset;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	bitmask = SON_UTRAN_LAC_PRESENT & p_new_nr_info->bitmask;
	if (bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				SON_MEMCMP(p_tbl_nr_info->lac, p_new_nr_info->lac,
					sizeof(p_tbl_nr_info->lac))))
	{
		SON_MEMCPY(p_tbl_nr_info->lac, p_new_nr_info->lac,
				sizeof(p_tbl_nr_info->lac));
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	bitmask = SON_UTRAN_RAC_PRESENT & p_new_nr_info->bitmask;
	if (bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				(p_tbl_nr_info->rac != p_new_nr_info->rac)))
	{
		p_tbl_nr_info->rac = p_new_nr_info->rac;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	bitmask = SON_UTRAN_PLMN_ID_PRESENT & p_new_nr_info->bitmask;
	if (bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				SON_MEMCMP(p_tbl_nr_info->plmn_id, p_new_nr_info->plmn_id,
					sizeof(p_tbl_nr_info->plmn_id))))
	{
		p_tbl_nr_info->num_plmn_id = p_new_nr_info->num_plmn_id;
		SON_MEMCPY(p_tbl_nr_info->plmn_id, p_new_nr_info->plmn_id,
				sizeof(p_tbl_nr_info->plmn_id));
		is_update_required = SON_TRUE;
	}

	bitmask = SON_UTRAN_CSG_ID_PRESENT & p_new_nr_info->bitmask;
	if (bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				SON_MEMCMP(p_tbl_nr_info->csg_identity, p_new_nr_info->csg_identity,
					sizeof(p_tbl_nr_info->csg_identity))))
	{
		SON_MEMCPY(p_tbl_nr_info->csg_identity, p_new_nr_info->csg_identity,
				sizeof(p_tbl_nr_info->csg_identity));
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	bitmask = SON_UTRAN_UARFCN_PRESENT & p_new_nr_info->bitmask;
	if (bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				(p_tbl_nr_info->uarfcn != p_new_nr_info->uarfcn)))
	{
		p_tbl_nr_info->uarfcn = p_new_nr_info->uarfcn;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	bitmask = SON_UTRAN_RSCP_PRESENT & p_new_nr_info->bitmask;
	if (bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				(p_tbl_nr_info->rscp != p_new_nr_info->rscp)))
	{
		p_tbl_nr_info->rscp = p_new_nr_info->rscp;
		is_update_required = SON_TRUE;
	}

	bitmask = SON_UTRAN_ECNO_PRESENT & p_new_nr_info->bitmask;
	if (bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				(p_tbl_nr_info->ecno != p_new_nr_info->ecno)))
	{
		p_tbl_nr_info->ecno = p_new_nr_info->ecno;
		is_update_required = SON_TRUE;
	}

	bitmask = SON_UTRAN_ENABLE_NR_STATUS_PRESENT & p_new_nr_info->bitmask;   
	if (bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				(p_tbl_nr_info->nr_status != p_new_nr_info->nr_status)))
	{
		p_tbl_nr_info->nr_status = p_new_nr_info->nr_status;
		is_update_required = SON_TRUE;
	}

	bitmask = SON_UTRAN_ENABLE_HO_STATUS_PRESENT & p_new_nr_info->bitmask;
	if (bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				(p_tbl_nr_info->ho_status != p_new_nr_info->ho_status)))
	{
		p_tbl_nr_info->ho_status = p_new_nr_info->ho_status;
		is_update_required = SON_TRUE;
	}

	bitmask = SON_UTRAN_ACCESS_MODE_PRESENT & p_new_nr_info->bitmask;
	if(bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				(p_tbl_nr_info->access_mode !=  p_new_nr_info->access_mode)))
	{
		p_tbl_nr_info->access_mode =  p_new_nr_info->access_mode;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	/*SRVCC-Start*/
	bitmask = SON_UTRAN_PS_HO_SUPPORT_PRESENT & p_new_nr_info->bitmask;
	if(bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				(p_tbl_nr_info->ps_ho_supported !=  p_new_nr_info->ps_ho_supported)))
	{
		p_tbl_nr_info->ps_ho_supported =  p_new_nr_info->ps_ho_supported;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	bitmask = SON_UTRAN_VOIP_CAPABLE_PRESENT & p_new_nr_info->bitmask;
	if(bitmask && (!(bitmask & p_tbl_nr_info->bitmask) ||
				(p_tbl_nr_info->voip_capable !=  p_new_nr_info->voip_capable)))
	{
		p_tbl_nr_info->voip_capable =  p_new_nr_info->voip_capable;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}
	/*SRVCC-End*/

	if (SON_TRUE == is_update_required)
	{
		p_tbl_nr_info->bitmask |= p_new_nr_info->bitmask;
	}

	SON_UT_TRACE_EXIT();
	return is_update_required;
} /* check_anr_utran_update_status */

/******************************************************************************
 * Function Name  : check_anr_cdma_cell_specific_params_update_status
 * Inputs         : p_new_nr_info       - Pointer to the CDMA neighbor cell
 *                                        specific params received in update 
 *                                        request message
 *                  p_tbl_nr_info       - Pointer to existing CDMA neighbor info
 *                  p_is_rrm_notif_reqd - Pointer to boolean value to indicate 
 *                                        if RRM notification is required
 * Outputs        : None
 * Returns        : son_bool_et         - SON_TRUE If updation is required
 *                  SON_FALSE - If updation is not required
 * Description    : This function is used to check if CDMA neighbor info is to
 *                  be updated or not in the Inter RAT NR hash table
 ******************************************************************************/
static son_bool_et 
check_anr_cdma_cell_specific_params_update_status
(
 son_cdma_cell_specific_params_t         *p_tbl_nr_info,
 const son_cdma_cell_specific_params_t   *p_new_nr_info,
 son_bool_et                             *p_is_rrm_notif_reqd
 )
{
	son_bool_et is_update_required = SON_FALSE;
	son_bool_et is_hyst_params_update_required = SON_FALSE;
	son_bool_et is_fpc_fch_update_required = SON_FALSE;
	son_oam_log_on_off_et log_mode = anr_get_log_mode();
	son_u32 new_bitmask = 0;


	SON_UT_TRACE_ENTER();

	if ((SON_PNULL == p_tbl_nr_info) ||
			(SON_PNULL == p_new_nr_info) ||
			(SON_PNULL == p_is_rrm_notif_reqd))
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"check_anr_cdma_cell_specific_params_update_status: "
				"NR could not be modified, "
				"p_tbl_nr_info or p_new_nr_info is NULL\n");

		SON_UT_TRACE_EXIT();
		return is_update_required;
	}

	if (SON_CDMA_CSP_PZ_HYST_PARAMS_INCLUDED_PRESENT & p_new_nr_info->bitmask)
	{
		new_bitmask = p_new_nr_info->pz_hyst_params_included.bitmask &
			SON_OAM_PZ_HYST_ENABLED_PRESENT;

		if (new_bitmask && (!(p_tbl_nr_info->pz_hyst_params_included.bitmask & new_bitmask) ||
					(p_tbl_nr_info->pz_hyst_params_included.pz_hyst_enabled !=
					 p_new_nr_info->pz_hyst_params_included.pz_hyst_enabled)))
		{
			p_tbl_nr_info->pz_hyst_params_included.pz_hyst_enabled
				= p_new_nr_info->pz_hyst_params_included.pz_hyst_enabled;
			is_hyst_params_update_required = SON_TRUE;
			*p_is_rrm_notif_reqd = SON_TRUE;
		}

		new_bitmask = p_new_nr_info->pz_hyst_params_included.bitmask &
			SON_OAM_PZ_HYST_INFO_INCL_PRESENT;

		if (new_bitmask && (!(p_tbl_nr_info->pz_hyst_params_included.bitmask & new_bitmask) ||
					(p_tbl_nr_info->pz_hyst_params_included.pz_hyst_info_incl !=
					 p_new_nr_info->pz_hyst_params_included.pz_hyst_info_incl)))
		{
			p_tbl_nr_info->pz_hyst_params_included.pz_hyst_info_incl
				= p_new_nr_info->pz_hyst_params_included.pz_hyst_info_incl;
			is_hyst_params_update_required = SON_TRUE;
			*p_is_rrm_notif_reqd = SON_TRUE;
		}

		new_bitmask = p_new_nr_info->pz_hyst_params_included.bitmask &
			SON_OAM_PZ_HYST_TIMER_PRESENT;

		if (new_bitmask && (!(p_tbl_nr_info->pz_hyst_params_included.bitmask & new_bitmask) ||
					(p_tbl_nr_info->pz_hyst_params_included.pz_hyst_act_timer !=
					 p_new_nr_info->pz_hyst_params_included.pz_hyst_act_timer)))
		{
			p_tbl_nr_info->pz_hyst_params_included.pz_hyst_act_timer
				= p_new_nr_info->pz_hyst_params_included.pz_hyst_act_timer;
			is_hyst_params_update_required = SON_TRUE;
			*p_is_rrm_notif_reqd = SON_TRUE;
		}

		new_bitmask = p_new_nr_info->pz_hyst_params_included.bitmask &
			SON_OAM_PZ_HYST_TIMER_MUL_PRESENT;

		if (new_bitmask && (!(p_tbl_nr_info->pz_hyst_params_included.bitmask & new_bitmask) ||
					(p_tbl_nr_info->pz_hyst_params_included.pz_hyst_timer_mul !=
					 p_new_nr_info->pz_hyst_params_included.pz_hyst_timer_mul)))
		{
			p_tbl_nr_info->pz_hyst_params_included.pz_hyst_timer_mul
				= p_new_nr_info->pz_hyst_params_included.pz_hyst_timer_mul;
			is_hyst_params_update_required = SON_TRUE;
			*p_is_rrm_notif_reqd = SON_TRUE;
		}

		new_bitmask = p_new_nr_info->pz_hyst_params_included.bitmask &
			SON_OAM_PZ_HYST_TIMER_EXP_PRESENT;

		if (new_bitmask && (!(p_tbl_nr_info->pz_hyst_params_included.bitmask & new_bitmask) ||
					(p_tbl_nr_info->pz_hyst_params_included.pz_hyst_timer_exp !=
					 p_new_nr_info->pz_hyst_params_included.pz_hyst_timer_exp)))
		{
			p_tbl_nr_info->pz_hyst_params_included.pz_hyst_timer_exp
				= p_new_nr_info->pz_hyst_params_included.pz_hyst_timer_exp;
			is_hyst_params_update_required = SON_TRUE;
			*p_is_rrm_notif_reqd = SON_TRUE;
		}

		new_bitmask = p_new_nr_info->pz_hyst_params_included.bitmask &
			SON_OAM_PZ_HYST_LIST_LEN_PRESENT;

		if (new_bitmask && (!(p_tbl_nr_info->pz_hyst_params_included.bitmask & new_bitmask) ||
					(p_tbl_nr_info->pz_hyst_params_included.pz_hyst_list_len !=
					 p_new_nr_info->pz_hyst_params_included.pz_hyst_list_len)))
		{
			p_tbl_nr_info->pz_hyst_params_included.pz_hyst_list_len
				= p_new_nr_info->pz_hyst_params_included.pz_hyst_list_len;
			is_hyst_params_update_required = SON_TRUE;
			*p_is_rrm_notif_reqd = SON_TRUE;
		}

		if (SON_TRUE == is_hyst_params_update_required)
		{
			p_tbl_nr_info->pz_hyst_params_included.bitmask |=
				p_new_nr_info->pz_hyst_params_included.bitmask;
		}
	}

	new_bitmask = p_new_nr_info->bitmask & SON_CDMA_CSP_P_REV_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->p_rev !=
				 p_new_nr_info->p_rev)))
	{
		p_tbl_nr_info->p_rev = p_new_nr_info->p_rev;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_CDMA_CSP_MIN_P_REV_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->min_p_rev != p_new_nr_info->min_p_rev)))
	{
		p_tbl_nr_info->min_p_rev = p_new_nr_info->min_p_rev;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask &
		SON_CDMA_CSP_NEG_SLOT_CYCLE_INDEX_PRESENT;

	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->neg_slot_cycle_index_sup !=
				 p_new_nr_info->neg_slot_cycle_index_sup)))
	{
		p_tbl_nr_info->neg_slot_cycle_index_sup =
			p_new_nr_info->neg_slot_cycle_index_sup;

		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask &
		SON_CDMA_CSP_ENCRYPT_MODE_PRESENT;

	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->encrypt_mode !=
				 p_new_nr_info->encrypt_mode)))
	{
		p_tbl_nr_info->encrypt_mode =
			p_new_nr_info->encrypt_mode;

		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask &
		SON_CDMA_CSP_ENCRYPT_SUPPORTED_PRESENT;

	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->encrypt_suported !=
				 p_new_nr_info->encrypt_suported)))
	{
		p_tbl_nr_info->encrypt_suported =
			p_new_nr_info->encrypt_suported;

		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask &
		SON_CDMA_CSP_SIG_ENCRYPT_SUPPORTED_PRESENT;

	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->sig_encrypt_suported !=
				 p_new_nr_info->sig_encrypt_suported)))
	{
		p_tbl_nr_info->sig_encrypt_suported =
			p_new_nr_info->sig_encrypt_suported;

		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask &
		SON_CDMA_CSP_MSG_INT_SUPPORTED_PRESENT;

	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->msg_integrity_sup !=
				 p_new_nr_info->msg_integrity_sup)))
	{
		p_tbl_nr_info->msg_integrity_sup =
			p_new_nr_info->msg_integrity_sup;

		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask &
		SON_CDMA_CSP_SIG_INT_SUPPORTED_INCL_PRESENT;

	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->sig_integrity_sup_incl !=
				 p_new_nr_info->sig_integrity_sup_incl)))
	{
		p_tbl_nr_info->sig_integrity_sup_incl =
			p_new_nr_info->sig_integrity_sup_incl;

		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask &
		SON_CDMA_CSP_SIG_INT_SUPPORTED_PRESENT;

	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->sig_integrity_sup !=
				 p_new_nr_info->sig_integrity_sup)))
	{
		p_tbl_nr_info->sig_integrity_sup =
			p_new_nr_info->sig_integrity_sup;

		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask &
		SON_CDMA_CSP_MS_INIT_POS_LOC_SUP_IND_PRESENT;

	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->ms_init_pos_loc_sup_ind !=
				 p_new_nr_info->ms_init_pos_loc_sup_ind)))
	{
		p_tbl_nr_info->ms_init_pos_loc_sup_ind =
			p_new_nr_info->ms_init_pos_loc_sup_ind;

		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask &
		SON_CDMA_CSP_BAND_CLASS_INFO_REQ_PRESENT;

	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->band_class_info_req !=
				 p_new_nr_info->band_class_info_req)))
	{
		p_tbl_nr_info->band_class_info_req =
			p_new_nr_info->band_class_info_req;

		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask &
		SON_CDMA_CSP_BAND_CLASS_PRESENT;

	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->band_class !=
				 p_new_nr_info->band_class)))
	{
		p_tbl_nr_info->band_class =
			p_new_nr_info->band_class;

		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask &
		SON_CDMA_CSP_ALT_BAND_CLASS_PRESENT;

	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->alt_band_class !=
				 p_new_nr_info->alt_band_class)))
	{
		p_tbl_nr_info->alt_band_class =
			p_new_nr_info->alt_band_class;

		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask &
		SON_CDMA_CSP_TKZ_MODE_SUP_PRESENT;

	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->tkz_mode_supported !=
				 p_new_nr_info->tkz_mode_supported)))
	{
		p_tbl_nr_info->tkz_mode_supported =
			p_new_nr_info->tkz_mode_supported;

		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask &
		SON_CDMA_CSP_TKZ_ID_PRESENT;

	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->tkz_id !=
				 p_new_nr_info->tkz_id)))
	{
		p_tbl_nr_info->tkz_id =
			p_new_nr_info->tkz_id;

		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	if (SON_CDMA_CSP_FPC_FCH_INCLUDED & p_new_nr_info->bitmask)
	{
		new_bitmask = p_new_nr_info->fpc_fch_included.bitmask &
			SON_OAM_FPC_FCH_INCL_RC3_PRESENT;

		if (new_bitmask && (!(p_tbl_nr_info->fpc_fch_included.bitmask & new_bitmask) ||
					(p_tbl_nr_info->fpc_fch_included.fpc_fch_init_setpt_rc3 !=
					 p_new_nr_info->fpc_fch_included.fpc_fch_init_setpt_rc3)))
		{
			p_tbl_nr_info->fpc_fch_included.fpc_fch_init_setpt_rc3
				= p_new_nr_info->fpc_fch_included.fpc_fch_init_setpt_rc3;

			is_fpc_fch_update_required = SON_TRUE;
			*p_is_rrm_notif_reqd = SON_TRUE;
		}

		new_bitmask = p_new_nr_info->fpc_fch_included.bitmask &
			SON_OAM_FPC_FCH_INCL_RC4_PRESENT;

		if (new_bitmask && (!(p_tbl_nr_info->fpc_fch_included.bitmask & new_bitmask) ||
					(p_tbl_nr_info->fpc_fch_included.fpc_fch_init_setpt_rc4 !=
					 p_new_nr_info->fpc_fch_included.fpc_fch_init_setpt_rc4)))
		{
			p_tbl_nr_info->fpc_fch_included.fpc_fch_init_setpt_rc4
				= p_new_nr_info->fpc_fch_included.fpc_fch_init_setpt_rc4;

			is_fpc_fch_update_required = SON_TRUE;
			*p_is_rrm_notif_reqd = SON_TRUE;
		}

		new_bitmask = p_new_nr_info->fpc_fch_included.bitmask &
			SON_OAM_FPC_FCH_INCL_RC5_PRESENT;

		if (new_bitmask && (!(p_tbl_nr_info->fpc_fch_included.bitmask & new_bitmask) ||
					(p_tbl_nr_info->fpc_fch_included.fpc_fch_init_setpt_rc5 !=
					 p_new_nr_info->fpc_fch_included.fpc_fch_init_setpt_rc5)))
		{
			p_tbl_nr_info->fpc_fch_included.fpc_fch_init_setpt_rc5
				= p_new_nr_info->fpc_fch_included.fpc_fch_init_setpt_rc5;

			is_fpc_fch_update_required = SON_TRUE;
			*p_is_rrm_notif_reqd = SON_TRUE;
		}

		if (SON_TRUE == is_fpc_fch_update_required)
		{
			p_tbl_nr_info->fpc_fch_included.bitmask |=
				p_new_nr_info->fpc_fch_included.bitmask;
		}
	}

	if (SON_TRUE == is_update_required)
	{
		p_tbl_nr_info->bitmask |= p_new_nr_info->bitmask;
	}

	if ((SON_TRUE == is_fpc_fch_update_required) ||
			(SON_TRUE == is_hyst_params_update_required))
	{
		is_update_required = SON_TRUE;
	}

	SON_UT_TRACE_EXIT();
	return is_update_required;
} /* check_anr_cdma_cell_specific_params_update_status */

/******************************************************************************
 * Function Name  : check_anr_cdma_update_status
 * Inputs         : p_new_nr_info - Pointer to the new CDMA neighbor info
 *                                        received in update request message
 *                  p_tbl_nr_info       - Pointer to existing CDMA neighbor info
 *                  p_is_rrm_notif_reqd - Poniter to boolean value to indicate 
 *                                        if RRM notification is required
 * Outputs        : None
 * Returns        : son_bool_et         - SON_TRUE If updation is required
 *                  SON_FALSE - If updation is not required
 * Description    : This function is used to check if CDMA neighbor info is to
 *                  be updated or not in the Inter RAT NR hash table
 ******************************************************************************/
static son_bool_et 
check_anr_cdma_update_status
(
 son_cdma_neighbor_info_t        *p_tbl_nr_info,
 const son_cdma_neighbor_info_t  *p_new_nr_info,
 son_bool_et                     *p_is_rrm_notif_reqd
 )
{
	son_bool_et is_update_required = SON_FALSE;
	son_oam_log_on_off_et log_mode = anr_get_log_mode();
	son_u32 new_bitmask = 0;


	SON_UT_TRACE_ENTER();

	if ((SON_PNULL == p_tbl_nr_info) ||
			(SON_PNULL == p_new_nr_info) ||
			(SON_PNULL == p_is_rrm_notif_reqd))
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"check_anr_cdma_update_status: NR could not be modified, "
				"p_tbl_nr_info or p_new_nr_info is NULL\n");

		SON_UT_TRACE_EXIT();
		return is_update_required;
	}

	*p_is_rrm_notif_reqd = SON_FALSE;

	new_bitmask = p_new_nr_info->bitmask & SON_CDMA_PCI_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->pci != p_new_nr_info->pci)))
	{
		p_tbl_nr_info->pci = p_new_nr_info->pci;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_CDMA_BAND_CLASS_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->band_class != p_new_nr_info->band_class)))
	{
		p_tbl_nr_info->band_class = p_new_nr_info->band_class;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_CDMA_BAND_CLASS_ARFCN_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->arfcn != p_new_nr_info->arfcn)))
	{
		p_tbl_nr_info->arfcn = p_new_nr_info->arfcn;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_CDMA_CSO_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->cell_specific_offset != 
				 p_new_nr_info->cell_specific_offset)))
	{
		p_tbl_nr_info->cell_specific_offset 
			= p_new_nr_info->cell_specific_offset;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_CDMA_PILOT_PN_PHASE_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->pilot_pn_phase !=
				 p_new_nr_info->pilot_pn_phase)))
	{
		p_tbl_nr_info->pilot_pn_phase
			= p_new_nr_info->pilot_pn_phase;
		is_update_required = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_CDMA_PILOT_STRENGTH_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->pilot_strength !=
				 p_new_nr_info->pilot_strength)))
	{
		p_tbl_nr_info->pilot_strength
			= p_new_nr_info->pilot_strength;
		is_update_required = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_CDMA_ENABLE_NR_STATUS_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->nr_status != p_new_nr_info->nr_status)))
	{
		p_tbl_nr_info->nr_status = p_new_nr_info->nr_status;
		is_update_required = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_CDMA_ENABLE_HO_STATUS_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->ho_status != p_new_nr_info->ho_status)))
	{
		p_tbl_nr_info->ho_status = p_new_nr_info->ho_status;
		is_update_required = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_CDMA_CELL_SPECIFIC_PARAMS_PRESENT;
	if (new_bitmask)
	{
		if (SON_TRUE == check_anr_cdma_cell_specific_params_update_status(
					&p_tbl_nr_info->cell_specific_params,
					&p_new_nr_info->cell_specific_params,
					p_is_rrm_notif_reqd))
		{
			is_update_required = SON_TRUE;
		}
	}

	if (SON_TRUE == is_update_required)
	{
		p_tbl_nr_info->bitmask |= p_new_nr_info->bitmask;
	}

	SON_UT_TRACE_EXIT();
	return is_update_required;
} /* check_anr_cdma_update_status */

/******************************************************************************
 * Function Name  : check_anr_geran_update_status
 * Inputs         : p_new_nr_info - Pointer to the new GERAN neighbor info
 *                  received in update request message.
 *                  p_is_rrm_notif_reqd - flag that informs whether notification
 *                                        to rrm will be send
 * Outputs        : p_tbl_nr_info -  Pointer to existing GERAN 
 *                                    neighbor info structure
 * Returns        : son_bool_et   - SON_TRUE If updation is required
 *                                  SON_FALSE If updation is not required
 * Description    : This function is used to check if GERAN neighbor info is to
 *                  be updated or not in the inter RAT NR hash table
 ******************************************************************************/
static son_bool_et
check_anr_geran_update_status
(
 son_geran_neighbor_info_t *p_tbl_nr_info,
 const son_geran_neighbor_info_t *p_new_nr_info,
 son_bool_et  *p_is_rrm_notif_reqd
 )
{
	son_bool_et is_update_required = SON_FALSE;
	son_oam_log_on_off_et log_mode = anr_get_log_mode();
	son_u32 new_bitmask = 0;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_tbl_nr_info || SON_PNULL == p_new_nr_info || SON_PNULL == p_is_rrm_notif_reqd)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"check_anr_geran_update_status: NR could not be modified, "
				"p_tbl_nr_info or p_new_nr_info is NULL\n");
		SON_UT_TRACE_EXIT();
		return is_update_required;
	}

	*p_is_rrm_notif_reqd = SON_FALSE;
	new_bitmask = p_new_nr_info->bitmask & SON_GERAN_PCI_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				SON_MEMCMP(&p_new_nr_info->pci, &p_tbl_nr_info->pci,
					sizeof(son_geran_physical_cell_id_t))))
	{
		p_tbl_nr_info->pci = p_new_nr_info->pci;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_GERAN_CSO_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->cell_specific_offset !=
				 p_new_nr_info->cell_specific_offset)))
	{
		p_tbl_nr_info->cell_specific_offset 
			= p_new_nr_info->cell_specific_offset;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_GERAN_RAC_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->rac != p_new_nr_info->rac)))
	{
		p_tbl_nr_info->rac = p_new_nr_info->rac;
		is_update_required = SON_TRUE;
        /*SPR 15393 Fix Start*/
        *p_is_rrm_notif_reqd = SON_TRUE;
        /*SPR 15393 Fix Stop*/
	}

	new_bitmask = p_new_nr_info->bitmask & SON_GERAN_ARFCN_BAND_IND_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->arfcn != p_new_nr_info->arfcn) ||
				(p_tbl_nr_info->band_ind != p_new_nr_info->band_ind)))
	{
		p_tbl_nr_info->arfcn = p_new_nr_info->arfcn;
		p_tbl_nr_info->band_ind  = p_new_nr_info->band_ind;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_GERAN_RSSI_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->rssi != p_new_nr_info->rssi)))
	{
		p_tbl_nr_info->rssi = p_new_nr_info->rssi;
		is_update_required = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_GERAN_ENABLE_NR_STATUS_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->nr_status != p_new_nr_info->nr_status)))
	{
		p_tbl_nr_info->nr_status = p_new_nr_info->nr_status;
		is_update_required = SON_TRUE;
	}

	new_bitmask = p_new_nr_info->bitmask & SON_GERAN_ENABLE_HO_STATUS_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->ho_status != p_new_nr_info->ho_status)))
	{
		p_tbl_nr_info->ho_status = p_new_nr_info->ho_status;
		is_update_required = SON_TRUE;
	}
	/*SPR-11121-Start*/
	new_bitmask = p_new_nr_info->bitmask & SON_GERAN_DTM_SUPPORT_PRESENT;
	if (new_bitmask && (!(p_tbl_nr_info->bitmask & new_bitmask) ||
				(p_tbl_nr_info->dtm_supported != p_new_nr_info->dtm_supported)))
	{
		p_tbl_nr_info->dtm_supported = p_new_nr_info->dtm_supported;
		is_update_required = SON_TRUE;
		*p_is_rrm_notif_reqd = SON_TRUE;
	}
	/*SPR-11121-End*/

	if (SON_TRUE == is_update_required)
	{
		p_tbl_nr_info->bitmask |= p_new_nr_info->bitmask;
	}

	SON_UT_TRACE_EXIT();
	return is_update_required;
} /* check_anr_geran_update_status */

/******************************************************************************
 * Function Name  : anr_check_update_status
 * Inputs         : p_new_nr_info - Pointer to the new inter RAT neighbor info
 *                  received in update request message.
 *                  p_is_rrm_notif_reqd - flag that informs whether notification
 *                                        to rrm will be send
 * Outputs        : p_tbl_nr_info -  Updated inter RAT neighbor info structure
 *
 * Returns        : son_bool_et   - SON_TRUE If updation is required
 *                                  SON_FALSE If updation is not required
 * Description    : This function is used to check if inter RAT neighbor info 
 *                  is to be updated or not in the inter RAT NR hash table
 ******************************************************************************/
static son_bool_et
anr_check_update_status
(
 anr_inter_rat_hash_rec_t        *p_tbl_nr_info,
 const anr_inter_rat_hash_rec_t  *p_new_nr_info,
 son_bool_et                     *p_is_rrm_notif_reqd
 )
{
	son_bool_et is_update_required = SON_FALSE;
	son_oam_log_on_off_et log_mode = anr_get_log_mode();


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_tbl_nr_info || SON_PNULL == p_new_nr_info)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_check_update_status: NR could not be modified,"
				"p_tbl_nr_info or p_new_nr_info is NULL\n");
		SON_UT_TRACE_EXIT();
		return is_update_required;
	}

	if (ANR_UTRAN_NEIGHBOR == p_new_nr_info->t && 
			ANR_UTRAN_NEIGHBOR == p_tbl_nr_info->t)
	{
		is_update_required = check_anr_utran_update_status(&p_tbl_nr_info->u.
				utran_neighbor_info, &p_new_nr_info->u.utran_neighbor_info,
				p_is_rrm_notif_reqd);
	}
	else if (ANR_GERAN_NEIGHBOR == p_new_nr_info->t &&
			ANR_GERAN_NEIGHBOR == p_tbl_nr_info->t)
	{
		is_update_required = check_anr_geran_update_status(&p_tbl_nr_info->u.
				geran_neighbor_info, &p_new_nr_info->u.geran_neighbor_info,
				p_is_rrm_notif_reqd);
	}
	else if (ANR_CDMA_NEIGHBOR == p_new_nr_info->t &&
			ANR_CDMA_NEIGHBOR == p_tbl_nr_info->t)
	{
		is_update_required = check_anr_cdma_update_status(
				&p_tbl_nr_info->u.cdma_neighbor_info,
				&p_new_nr_info->u.cdma_neighbor_info,
				p_is_rrm_notif_reqd);
	}
	else
	{
		/* Check for other RAT types in next release */
	}

	SON_UT_TRACE_EXIT();
	return is_update_required;
} /* anr_check_update_status */

/******************************************************************************
 * Function Name  : anr_inter_rat_modify_nr
 * Inputs         : p_cell_ctxt - Pointer to cell context
 *                  p_nr_mdfd   - Pointer to modified nbr in inter rat hash record
 *                  check_remove_list - Flag to check the reomve list
 *                  p_oam_notification - Pointer to oam notification msg
 *                  p_rrm_notification - Pointer to rrm notification msg
 * Outputs        : p_error_code -  Pointer to a variable for storing error code
 *
 * Returns        : anr_inter_rat_hash_rec_t* - Pointer to the data
 *                                            field of the modified 
 *                                               NR record.
 * Description    : This function is used for updating selected NR 
 *                  records for a cell configured from inter RAT NR hash table. 
 ******************************************************************************/
anr_inter_rat_hash_rec_t *
anr_inter_rat_modify_nr
(
 anr_cell_context_t      *p_cell_ctxt,
 anr_inter_rat_hash_rec_t *p_nr_mdfd,
 son_bool_et check_remove_list,
 son_error_et *p_error_code,
 son_anr_updated_nrt_info_t *p_oam_notification,
 son_anr_rrm_updated_nrt_info_t *p_rrm_notification
 )
{
	anr_inter_rat_hash_nr_tuple_t   *p_tpl             = SON_PNULL;
	anr_inter_rat_hash_rec_t 		*p_ret_val 	       = SON_PNULL;
	SON_HASH_NODE					*p_node	           = SON_PNULL;
	son_bool_et					    is_update_required = SON_FALSE;
	son_oam_log_on_off_et           log_mode           = anr_get_log_mode();
	son_u32                         cell_identity      = 0;
	anr_inter_rat_hash_rec_t        temp_hash_data     = {0};
	anr_inter_rat_hash_rec_t        *p_hash_rec        = SON_PNULL; 
	son_inter_rat_neighbor_info_t   inter_rat_nr    = {0};
	SON_HASH *p_hash = SON_PNULL;
	son_bool_et is_rrm_sendable_params_changed = SON_FALSE;
	son_bool_et is_rrm_notif_reqd_to_rrm       = SON_FALSE;
	son_bool_et is_old_nbr_send                = SON_FALSE;
	son_bool_et is_new_nr_sendable             = SON_FALSE;
	son_ho_status_et old_ho_status             = SON_HO_UNDEFINED; 
	son_ho_status_et new_ho_status             = SON_HO_UNDEFINED;
	son_error_et     err_code_sts              = SON_NO_ERROR;
	son_bool_et     is_new_nr_changed_from_old_nr_for_notif   = SON_TRUE;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_nr_mdfd ||
			SON_PNULL == p_error_code)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_inter_rat_modify_nr: NR could not be modified,"
				" p_cell_ctxt or p_nr_mdfd or p_error_code is NULL check_remove_list=%u\n",
                check_remove_list);
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if ((p_nr_mdfd->t == ANR_UTRAN_NEIGHBOR &&
				(SON_UTRAN_NR_HO_STATS_PRESENT & p_nr_mdfd->u.utran_neighbor_info.bitmask)) ||
			(p_nr_mdfd->t == ANR_GERAN_NEIGHBOR &&
			 (SON_GERAN_NR_HO_STATS_PRESENT & p_nr_mdfd->u.geran_neighbor_info.bitmask)) ||
			((p_nr_mdfd->t == ANR_CDMA_NEIGHBOR &&
			  SON_CDMA_NR_HO_STATS_PRESENT & p_nr_mdfd->u.cdma_neighbor_info.bitmask)))
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_inter_rat_modify_nr: NR could not be modified,"
				" as read-only parameter ho_stats is present\n");
		SON_UT_TRACE_EXIT();
		*p_error_code =  SON_ERR_INVALID_PARAMS;
		return SON_PNULL;
	}

	if (SON_FALSE == anr_validate_csg_id_access_mode_for_inter_rat_neighbor(p_nr_mdfd))
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_inter_rat_modify_nr: NR could not be modified,"
				" as Input data is invalid\n");
		SON_UT_TRACE_EXIT();
		*p_error_code =  SON_ERR_INVALID_PARAMS;
		return SON_PNULL;
	}

	p_hash = &p_cell_ctxt->inter_rat_neighbor_table;
	p_hash_rec  = &temp_hash_data;

	if (SON_PNULL != (p_node = hash_find(p_hash, (son_void_t *)p_nr_mdfd)))
	{
		p_tpl = (anr_inter_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_inter_rat_hash_nr_tuple_t, anchor, p_node);

		/*Placing new data to the p_hash_rec structre*/
		SON_MEMCPY(p_hash_rec, p_nr_mdfd, sizeof(anr_inter_rat_hash_rec_t));

		/*Placing previous data to the p_nr_mdfd*/
		SON_MEMCPY(p_nr_mdfd, &p_tpl->data, sizeof(anr_inter_rat_hash_rec_t));

		/*This function determines if the updation has happened and
		 * if the updation has happened is it notifiable to RRM.This function
		 * also updates the inter rat neihgbor */

		is_update_required = anr_check_update_status(
				&p_tpl->data, p_hash_rec, &is_rrm_sendable_params_changed);

		/* Get the value of cell identity to print in logs */
		cell_identity = anr_get_cell_identity_from_inter_nr_rec(p_hash_rec);

		/*Set the time stamp*/
		son_time(&p_tpl->data.last_activity_ts);

		/*If no Updation is required */
		if (SON_FALSE == is_update_required)
		{
			if (ANR_CDMA_NEIGHBOR != p_hash_rec->t)
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_BRIEF,
						"anr_inter_rat_modify_nr: NR [Cell id: 0x%x]"
						" does not require modification\n",
						cell_identity);
			}
			else
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_BRIEF,
						"anr_inter_rat_modify_nr: CDMA NR "
						"does not require modification\n");
				anr_print_cdma_cell_id(&p_hash_rec->u.cdma_neighbor_info.nbr_cgi);
			}

			*p_error_code = SON_NO_ERROR;
			SON_UT_TRACE_EXIT();
			return p_ret_val;
		}

		if (SON_FALSE == anr_validate_csg_id_access_mode_for_inter_rat_neighbor(&p_tpl->data))
		{
			err_code_sts  = SON_ERR_INVALID_PARAMS;  
		}
		else
		{
			/*Determine if the old nr was send to RRM*/
			is_old_nbr_send = is_inter_rat_nr_sendable_to_rrm(p_nr_mdfd);

			/*Determine if the new nr is sendable to RRM*/
			is_new_nr_sendable = is_inter_rat_nr_sendable_to_rrm(&p_tpl->data);

			/*Determine the ho status for old inter rat nr*/
			old_ho_status = get_ho_status_for_inter_rat_nbrs(p_nr_mdfd); 

			/*Determine the ho status for the new inter rat nr*/  
			new_ho_status = get_ho_status_for_inter_rat_nbrs(&p_tpl->data);

			if (SON_FALSE == is_rrm_sendable_params_changed && old_ho_status == new_ho_status)
			{
				is_new_nr_changed_from_old_nr_for_notif = SON_FALSE;
			}
			else if (SON_TRUE == is_new_nr_sendable)
			{
				is_rrm_notif_reqd_to_rrm = SON_TRUE;
			}

			/*This function determine the changes in the 
			 *  NRT are valid or a Roll back should happen*/
			if (SON_TRUE == is_new_nr_changed_from_old_nr_for_notif)
			{
				anr_determine_the_changes_in_inter_nrt(p_cell_ctxt,
						p_nr_mdfd,
						&p_tpl->data, old_ho_status,
						new_ho_status,
						&is_old_nbr_send,
						&is_rrm_notif_reqd_to_rrm,
						&err_code_sts);
			}
		}

		/*Roll back if the error occurs*/
		if (SON_NO_ERROR  != err_code_sts)
		{
			/*Roll back to the old parameters*/  
			SON_MEMCPY(&p_tpl->data, p_nr_mdfd, sizeof(anr_inter_rat_hash_rec_t));

			*p_error_code = err_code_sts;
			if (ANR_CDMA_NEIGHBOR != p_hash_rec->t)
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_BRIEF,
						"anr_inter_rat_modify_nr: NR [Cell id: 0x%x]"
						" can not be  modified\n",
						cell_identity);
			}
			else
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_BRIEF,
						"anr_inter_rat_modify_nr: CDMA NR "
						"can not be modified\n");
				anr_print_cdma_cell_id(&p_hash_rec->u.cdma_neighbor_info.nbr_cgi);
			}

			SON_UT_TRACE_EXIT();
			return p_ret_val;
		}

		if (SON_TRUE == is_update_required)
		{
			/* Need to verify if just updating the object works or
			 * deletion and addition is required */
			if (ANR_CDMA_NEIGHBOR != p_hash_rec->t)
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_BRIEF,
						"anr_inter_rat_modify_nr: NR [Cell id: 0x%x]"
						"modified successfully\n",
						cell_identity);
			}
			else
			{
				SON_LOG(log_mode, p_son_anr_facility_name,
						SON_BRIEF,
						"anr_inter_rat_modify_nr: CDMA NR "
						"modified successfully\n");
				anr_print_cdma_cell_id(&p_hash_rec->u.cdma_neighbor_info.nbr_cgi);
			}

			p_ret_val = &p_tpl->data;
			*p_error_code = SON_NO_ERROR;
		}

		if (p_oam_notification && (SON_TRUE == is_update_required))
		{
			p_oam_notification->nrt_op = SON_NRT_OP_UPDATE;
			anr_convert_inter_rat_hash_rec_to_inter_rat_nr(
					&p_tpl->data, &inter_rat_nr);

			anr_construct_oam_updated_ncl(&p_oam_notification->nr_list,
					&inter_rat_nr,
					SON_TRUE);
		}

		if (p_rrm_notification && SON_TRUE == is_new_nr_changed_from_old_nr_for_notif)
		{
			if (ANR_UTRAN_NEIGHBOR == p_tpl->data.t)
			{
				if (SON_TRUE == is_rrm_notif_reqd_to_rrm)
				{
					anr_construct_rrm_inter_rat_ncl_for_utran_nbr(
							&p_rrm_notification->inter_rat_ncl.utran_neighbor_list,
							&p_tpl->data.u.utran_neighbor_info, SON_FALSE);
				}

				if (SON_TRUE == is_old_nbr_send)
				{
					anr_construct_rrm_inter_rat_ncl_for_utran_nbr(
							&p_rrm_notification->inter_rat_ncl.utran_neighbor_list,
							&p_nr_mdfd->u.utran_neighbor_info, SON_TRUE);
				}

				if (SON_TRUE == is_utran_nbrs_available_in_rrm_notification(
							&p_rrm_notification->inter_rat_ncl.utran_neighbor_list))
				{
					p_rrm_notification->inter_rat_ncl.bitmask |= SON_RRM_UTRAN_NR_LIST_PRESENT;
				}
			}

			else if (ANR_GERAN_NEIGHBOR == p_tpl->data.t)
			{
				if (SON_TRUE == is_rrm_notif_reqd_to_rrm)
				{
					anr_construct_rrm_inter_rat_ncl_for_geran_nbr(
							&p_rrm_notification->inter_rat_ncl.geran_neighbor_list,
							&p_tpl->data.u.geran_neighbor_info, SON_FALSE);
				}

				if (SON_TRUE == is_old_nbr_send)
				{
					anr_construct_rrm_inter_rat_ncl_for_geran_nbr(
							&p_rrm_notification->inter_rat_ncl.geran_neighbor_list,
							&p_nr_mdfd->u.geran_neighbor_info, SON_TRUE);
				}

				if (SON_TRUE == is_geran_nbrs_available_in_rrm_notification(
							&p_rrm_notification->inter_rat_ncl.geran_neighbor_list))
				{
					p_rrm_notification->inter_rat_ncl.bitmask |= SON_RRM_GERAN_NR_LIST_PRESENT;
				}
			}

			else if (ANR_CDMA_NEIGHBOR == p_tpl->data.t)
			{
				if (SON_CGI_1X_RTT_PRESENT & p_tpl->data.u.cdma_neighbor_info.nbr_cgi.bitmask)
				{
					if (SON_TRUE == is_rrm_notif_reqd_to_rrm)
					{
						anr_construct_rrm_inter_rat_ncl_for_cdma_xrtt_nbr(
								&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_xrtt,
								&p_tpl->data.u.cdma_neighbor_info, SON_FALSE);
					}

					if (SON_TRUE == is_old_nbr_send)
					{
						anr_construct_rrm_inter_rat_ncl_for_cdma_xrtt_nbr(
								&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_xrtt,
								&p_nr_mdfd->u.cdma_neighbor_info, SON_TRUE);
					}

					if (SON_TRUE == is_cdma_xrtt_nbrs_available_in_rrm_notification(
								&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_xrtt))
					{
						p_rrm_notification->inter_rat_ncl.bitmask |= SON_RRM_CDMA_NR_LIST_XRTT_PRESENT;
					}
				}
				else if (SON_CGI_HRPD_PRESENT & p_tpl->data.u.cdma_neighbor_info.nbr_cgi.bitmask)
				{
					if (SON_TRUE == is_rrm_notif_reqd_to_rrm)
					{
						anr_construct_rrm_inter_rat_ncl_for_cdma_hrpd_nbr(
								&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_hrpd,
								&p_tpl->data.u.cdma_neighbor_info, SON_FALSE);
					}

					if (SON_TRUE == is_old_nbr_send)
					{
						anr_construct_rrm_inter_rat_ncl_for_cdma_hrpd_nbr(
								&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_hrpd,
								&p_nr_mdfd->u.cdma_neighbor_info, SON_TRUE);
					}

					if (SON_TRUE == is_cdma_hrpd_nbrs_available_in_rrm_notification(
								&p_rrm_notification->inter_rat_ncl.cdma_neighbor_list_hrpd))
					{
						p_rrm_notification->inter_rat_ncl.bitmask |= SON_RRM_CDMA_NR_LIST_HRPD_PRESENT;
					}
				}
			}
		}

		SON_UT_TRACE_EXIT();
		return p_ret_val;
	}

	if (ANR_CDMA_NEIGHBOR != p_nr_mdfd->t)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_inter_rat_modify_nr: NR [Cell Id: 0x%x]"
				" not present in the table\n",
				cell_identity);
	}
	else
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_inter_rat_modify_nr: CDMA NR "
				"not present in the table\n");
		anr_print_cdma_cell_id(&p_nr_mdfd->u.cdma_neighbor_info.nbr_cgi);
	}

	*p_error_code = SON_ERR_ANR_NR_NOT_FOUND;

	SON_UT_TRACE_EXIT();
	return p_ret_val;
} /* anr_inter_rat_modify_nr */

/*****************************************************************************
 * Function Name  : anr_inter_rat_find_nr
 * Inputs         : p_hash -   Pointer to the hash context for a 
 *                             cell configured.
 *                : p_nr_cell_id   - Pointer to the cell id of the neighbor 
 * Outputs        : p_error_code -  Pointer to a variable for storing error code
 *                  p_rat_type - Pointer to a variable for storing RAT type
 *
 * Returns        : anr_inter_rat_hash_nr_tuple_t* - Pointer to the inter RAT 
 *                  NR record of a neighbor                    
 * Description     : This function returns the NR record of a specified 
 *                   neighbor from inter RAT NR hash table
 ******************************************************************************/
anr_inter_rat_hash_nr_tuple_t *
anr_inter_rat_find_nr
(
 SON_HASH *p_hash,
 son_inter_rat_global_cell_id_t  *p_nr_cell_id,
 son_error_et *p_error_code,
 son_rat_type_et         *p_rat_type
 )
{
	/* Coverity CID-55069 Fix Starts */
	anr_inter_rat_hash_rec_t       hash_data	 = {0};
	SON_HASH_NODE		             *p_node     = SON_PNULL;
	anr_inter_rat_hash_nr_tuple_t  *p_tpl      = SON_PNULL;
	son_u32                        cell_identity = 0;
	son_oam_log_on_off_et          log_mode      = anr_get_log_mode();


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_hash || SON_PNULL == p_nr_cell_id || 
			SON_PNULL == p_error_code || SON_PNULL == p_rat_type)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_inter_rat_find_nr: NR could not be found, "
				"p_hash or p_nr_cell_id or p_error_code or p_rat_type is NULL\n");
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if (SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT & p_nr_cell_id->bitmask)
	{
		if (SON_UTRAN_CELL == p_nr_cell_id->utran_geran_global_cell_id.rat_type)
		{
			hash_data.t = ANR_UTRAN_NEIGHBOR;

			SON_MEMCPY(&hash_data.u.utran_neighbor_info.nbr_cgi,
					&p_nr_cell_id->utran_geran_global_cell_id, sizeof(son_utran_geran_global_cell_id_t));

			cell_identity = son_convert_char_cell_id_to_int_cell_id(
					p_nr_cell_id->utran_geran_global_cell_id.cell_identity);
			*p_rat_type = SON_UTRAN_CELL;
		}
		else if (SON_GERAN_CELL == p_nr_cell_id->utran_geran_global_cell_id.rat_type)
		{
			hash_data.t = ANR_GERAN_NEIGHBOR;

			SON_MEMCPY(&hash_data.u.geran_neighbor_info.nbr_cgi,
					&p_nr_cell_id->utran_geran_global_cell_id, sizeof(son_utran_geran_global_cell_id_t));

			cell_identity = son_convert_char_cell_id_to_int_cell_id(
					p_nr_cell_id->utran_geran_global_cell_id.cell_identity);
			*p_rat_type = SON_GERAN_CELL;
		}
	}
	else if (SON_CDMA_GLOBAL_CELL_ID_PRESENT & p_nr_cell_id->bitmask)
	{
		hash_data.t = ANR_CDMA_NEIGHBOR;

		SON_MEMCPY(&hash_data.u.cdma_neighbor_info.nbr_cgi,
				&p_nr_cell_id->cdma_global_cell_id, sizeof(son_cdma_global_cell_id_t));

		if (p_nr_cell_id->cdma_global_cell_id.bitmask & SON_CGI_1X_RTT_PRESENT)
		{
			cell_identity = son_convert_char_cell_id_to_int_cell_id(
					p_nr_cell_id->cdma_global_cell_id.cell_global_id_1X_RTT);
		}
		else if (p_nr_cell_id->cdma_global_cell_id.bitmask & SON_CGI_HRPD_PRESENT)
		{
			cell_identity = son_convert_char_cell_id_to_int_cell_id(
					p_nr_cell_id->cdma_global_cell_id.cell_global_id_HRPD);
		}

		*p_rat_type = SON_CDMA_CELL;
	}
	else
	{
		/* Poplate the hash_data as per other RAT type in next release */
	}

	if (SON_PNULL != (p_node = hash_find(p_hash, (son_void_t *)&hash_data)))
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_BRIEF,
				"anr_inter_rat_find_nr: NR [Cell Id: 0x%x]"
				" is found in the table\n",
				cell_identity);
		p_tpl = (anr_inter_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_inter_rat_hash_nr_tuple_t, anchor, p_node);
		*p_error_code = SON_NO_ERROR;           
		SON_UT_TRACE_EXIT();
		return p_tpl;
	}

	SON_LOG(log_mode, p_son_anr_facility_name,
			SON_ERROR,
			"anr_inter_rat_find_nr: NR [Cell id: 0x%x]"
			" not found in the table\n",
			cell_identity);

	*p_error_code = SON_ERR_ANR_NR_NOT_FOUND;

	SON_UT_TRACE_EXIT();

	return SON_PNULL;
	/* Coverity CID-55069 Fix Ends */
} /* anr_inter_rat_find_nr */

/******************************************************************************
 * Function Name  : anr_find_nr
 * Inputs         : p_cell_ctxt -   Pointer to the context for a cell
 *                : p_tgt_cell_id  - Pointer to the neighbor cell Id
 *
 * Outputs        : p_error_code -  Pointer to a variable for storing error code
 *                  p_rat_type - Pointer to a variable for storing RAT type
 * Returns        : son_void_t*  - Pointer to the inter RAT tuple: The message is handled
 *                  successfully
 *                                 NULL - The message is not handled successfully
 * Description    : This function is used for removing selected NR
 *                  records for a cell configured
 ******************************************************************************/
son_void_t *
anr_find_nr
(
 anr_cell_context_t      *p_cell_ctxt,
 son_global_cell_id_t    *p_tgt_cell_id,
 son_error_et            *p_error_code,
 son_rat_type_et         *p_rat_type
 )
{
	son_void_t                   *p_tpl     = SON_PNULL;
	son_oam_log_on_off_et        log_mode   = anr_get_log_mode();


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_tgt_cell_id ||
			SON_PNULL == p_error_code || SON_PNULL == p_rat_type)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_find_nr: NR could not be found, "
				"p_cell_ctxt or p_tgt_cell_id or p_error_code or p_rat_type"
				" is NULL\n");
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if (SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT == p_tgt_cell_id->bitmask)
	{
		p_tpl = (anr_intra_rat_hash_nr_tuple_t *)anr_intra_rat_find_nr(
				&p_cell_ctxt->intra_rat_neighbor_table,
				&p_tgt_cell_id->intra_rat_global_cell_id,
				p_error_code);
		*p_rat_type = SON_EUTRAN_CELL;
	}
	else if (SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT == p_tgt_cell_id->bitmask)
	{
		p_tpl = (anr_inter_rat_hash_nr_tuple_t *)anr_inter_rat_find_nr(
				&p_cell_ctxt->inter_rat_neighbor_table,
				&p_tgt_cell_id->inter_rat_global_cell_id,
				p_error_code,
				p_rat_type);
	}
	else
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_find_nr: NR could not be found,"
				"cell Id received with invalid bitmask value = %u.\n",
				p_tgt_cell_id->bitmask);
		*p_error_code = SON_ERR_INVALID_PARAMS;
	}

	SON_UT_TRACE_EXIT();    
	return p_tpl;
} /* anr_find_nr */

/*****************************************************************************
 * Function Name  : anr_intra_rat_find_nr
 * Inputs         : p_hash -   Pointer to the hash context for a 
 *                             cell configured.
 *                : p_nr_cell_id   - Pointer to the cell id of the neighbor 
 * Outputs        : p_error_code -  Pointer to a variable for storing error code
 *
 * Returns        : anr_intra_rat_hash_nr_tuple_t* - Pointer to the NR record 
 *                  of a neighbor
 * Description     : This function returns the NR record of a specified neighbor
 ******************************************************************************/
anr_intra_rat_hash_nr_tuple_t *
anr_intra_rat_find_nr
(
 SON_HASH *p_hash,
 son_intra_rat_global_cell_id_t  *p_nr_cell_id,
 son_error_et *p_error_code
 )
{
	anr_intra_rat_hash_rec_t hash_data;
	SON_HASH_NODE                      *p_node     = SON_PNULL;
	anr_intra_rat_hash_nr_tuple_t  *p_tpl      = SON_PNULL;
	son_oam_log_on_off_et log_mode        = anr_get_log_mode();


	SON_UT_TRACE_ENTER();
	SON_MEMSET(&hash_data, 0, sizeof(anr_intra_rat_hash_rec_t));

	if (SON_PNULL == p_hash || SON_PNULL == p_nr_cell_id ||
			SON_PNULL == p_error_code)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_intra_rat_find_nr: NR could not be found, "
				"p_hash or p_nr_cell_id or p_error_code is NULL\n");
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	SON_MEMSET(&hash_data, 0, sizeof(anr_intra_rat_hash_rec_t));

	SON_MEMCPY(&hash_data.neighbor_info.nbr_cgi, p_nr_cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	if (SON_PNULL != (p_node = hash_find(p_hash, (son_void_t *)&hash_data)))
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_BRIEF,
				"anr_intra_rat_find_nr: NR [Cell Id: 0x%x]"
				" is found in the table\n",
				son_convert_char_cell_id_to_int_cell_id(p_nr_cell_id->cell_identity));
		p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_intra_rat_hash_nr_tuple_t, anchor, p_node);

		*p_error_code = SON_NO_ERROR;            
		SON_UT_TRACE_EXIT();
		return p_tpl;
	}

	SON_LOG(log_mode, p_son_anr_facility_name,
			SON_ERROR,
			"anr_intra_rat_find_nr: NR [Cell id: 0x%x]"
			" not found in the table\n",
			son_convert_char_cell_id_to_int_cell_id(p_nr_cell_id->cell_identity));

	*p_error_code = SON_ERR_ANR_NR_NOT_FOUND;

	SON_UT_TRACE_EXIT();

	return SON_PNULL;
} /* anr_intra_rat_find_nr */

/*****************************************************************************
 * Function Name  : anr_get_first_nr_from_intra_rat_hash_table
 * Inputs         : p_hash -   Pointer to the hash context for a cell configured
 * Outputs        : None
 * Returns        : anr_intra_rat_hash_nr_tuple_t*- Pointer to the first 
 *                  NR record node                     
 * Description    : Function to return the first NR record form hash table
 ******************************************************************************/
anr_intra_rat_hash_nr_tuple_t *
anr_get_first_nr_from_intra_rat_hash_table
(
 SON_HASH *p_hash
 )
{
	anr_intra_rat_hash_nr_tuple_t *p_tpl = SON_PNULL;
	SON_HASH_NODE *p_node = SON_PNULL;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_hash)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_get_first_nr_from_intra_rat_hash_table: "
				"NR could not be fetched, p_hash is NULL\n");
		SON_UT_TRACE_EXIT();
		return p_tpl;
	}

	p_node = hash_get_first(p_hash);
	if (SON_PNULL != p_node)
	{
		p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_intra_rat_hash_nr_tuple_t, anchor, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_tpl;
} /* anr_get_first_nr_from_intra_rat_hash_table */

/*****************************************************************************
 * Function Name  : anr_get_first_nr_from_inter_rat_hash_table
 * Inputs         : p_hash -   Pointer to the hash context for a cell configured
 * Output         : None
 * Returns        : anr_inter_rat_hash_nr_tuple_t*- Pointer to the first 
 *                  inter RAT NR record node                     
 * Description    : Function to return the first inter RAT NR record form 
 *                  hash table
 ******************************************************************************/
anr_inter_rat_hash_nr_tuple_t *
anr_get_first_nr_from_inter_rat_hash_table
(
 SON_HASH *p_hash
 )
{
	anr_inter_rat_hash_nr_tuple_t *p_tpl = SON_PNULL;
	SON_HASH_NODE *p_node = SON_PNULL;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_hash)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_get_first_nr_from_inter_rat_hash_table: "
				"NR could not be fetched, p_hash is NULL\n");
		SON_UT_TRACE_EXIT();
		return p_tpl;
	}

	p_node = hash_get_first(p_hash);
	if (SON_PNULL != p_node)
	{
		p_tpl = (anr_inter_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_inter_rat_hash_nr_tuple_t, anchor, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_tpl;
} /* anr_get_first_nr_from_inter_rat_hash_table */

/******************************************************************************
 * Function Name  : anr_get_next_nr_from_intra_rat_hash_table
 * Inputs         : p_hash -   Pointer to the hash context for a cell configured
 *                : p_node -   Pointer to the current hash node
 * Output         : None
 * Returns        : anr_intra_rat_hash_nr_tuple_t * - Pointer to the next 
 *                  NR record node                   
 * Description    : Function to return the next NR record form hash table
 ******************************************************************************/
anr_intra_rat_hash_nr_tuple_t *
anr_get_next_nr_from_intra_rat_hash_table
(
 SON_HASH        *p_hash,
 SON_HASH_NODE   *p_node
 )
{
	anr_intra_rat_hash_nr_tuple_t *p_tpl = SON_PNULL;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_hash || SON_PNULL == p_node)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_get_next_nr_from_intra_rat_hash_table: "
				"NR could not be fetched, p_hash is NULL\n");
		SON_UT_TRACE_EXIT();
		return p_tpl;
	}

	p_node = hash_get_next(p_hash, p_node);
	if (SON_PNULL != p_node)
	{
		p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_intra_rat_hash_nr_tuple_t, anchor, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_tpl;
} /* anr_get_next_nr_from_intra_rat_hash_table */

/******************************************************************************
 * Function Name  : anr_get_next_nr_from_inter_rat_hash_table
 * Inputs         : p_hash -   Pointer to the hash context for a cell configured
 *                : p_node -   Pointer to the current hash node
 * Output         : None
 * Returns        : anr_inter_rat_hash_nr_tuple_t * - Pointer to the next inter
 *                  RAT NR record node                   
 * Description    : Function to return the next NR record form inter RAT NR 
 *                  hash table
 ******************************************************************************/
anr_inter_rat_hash_nr_tuple_t *
anr_get_next_nr_from_inter_rat_hash_table
(
 SON_HASH        *p_hash,
 SON_HASH_NODE   *p_node
 )
{
	anr_inter_rat_hash_nr_tuple_t *p_tpl = SON_PNULL;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_hash || SON_PNULL == p_node)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_get_next_nr_from_inter_rat_hash_table: "
				"NR could not be fetched, p_hash is NULL\n");
		SON_UT_TRACE_EXIT();
		return p_tpl;
	}

	p_node = hash_get_next(p_hash, p_node);
	if (SON_PNULL != p_node)
	{
		p_tpl = (anr_inter_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_inter_rat_hash_nr_tuple_t, anchor, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_tpl;
} /* anr_get_next_nr_from_inter_rat_hash_table */

/******************************************************************************
 * Function Name  : anr_clear_nr_intra_rat_hash_table
 * Inputs         : p_hash -   Pointer to the hash context for a cell configured
 * Output         : None
 * Returns        : son_void_t - None
 * Description    : This function clears entire hash context along with 
 *                  the table  for a configured cell
 ******************************************************************************/
son_void_t
anr_clear_nr_intra_rat_hash_table
(
 SON_HASH *p_hash
 )
{
	SON_HASH_NODE *p_node        = SON_PNULL;
	SON_HASH_NODE *p_node_tmp    = SON_PNULL;
	anr_intra_rat_hash_nr_tuple_t *p_tpl = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_node =  hash_get_first(p_hash);

	while (p_node)
	{
		p_node_tmp = p_node;
		p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_intra_rat_hash_nr_tuple_t,
					anchor,
					p_node);

		anr_clear_nr_nbrs_list(&p_tpl->data.anr_nr_nbr_list);

		p_node = hash_get_next(p_hash, p_node);
		ANR_FREE_HASH_REC(anr_get_intra_rat_hash_rec_pool(), p_hash, p_tpl, p_node_tmp);
	}

	/* Coverity CID-48276 Fix Starts */
	ANR_FREE_TABLE(p_hash->table);
	/* Coverity CID-48276 Fix Ends */

	SON_UT_TRACE_EXIT();
} /* anr_clear_nr_intra_rat_hash_table */

/******************************************************************************
 * Function Name  : anr_clear_nr_inter_rat_hash_table
 * Inputs         : p_hash -   Pointer to the hash context for a cell configured
 * None           : None
 * Returns        : son_void_t - None
 * Description    : This function clears entire inter RAT NR hash context 
 *                  along with the table  for a configured cell
 ******************************************************************************/
son_void_t
anr_clear_nr_inter_rat_hash_table
(
 SON_HASH *p_hash
 )
{
	SON_HASH_NODE *p_node        = SON_PNULL;
	SON_HASH_NODE *p_node_tmp    = SON_PNULL;
	anr_inter_rat_hash_nr_tuple_t *p_tpl = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_node =  hash_get_first(p_hash);

	while (p_node)
	{
		p_node_tmp = p_node;
		p_tpl = (anr_inter_rat_hash_nr_tuple_t *)
			YMEMBEROF(anr_inter_rat_hash_nr_tuple_t,
					anchor,
					p_node);
		p_node = hash_get_next(p_hash, p_node);
		ANR_FREE_HASH_REC(anr_get_inter_rat_hash_rec_pool(),
				p_hash,
				p_tpl,
				p_node_tmp);
	}

	/* Coverity CID-48275 Fix Starts */
	ANR_FREE_TABLE(p_hash->table);
	/* Coverity CID-48275 Fix Ends */

	SON_UT_TRACE_EXIT();
} /* anr_clear_nr_inter_rat_hash_table */

/*SPR 13795 Changes - Start - Function optimized*/

/******************************************************************************
 * Function Name  : anr_automatic_neighbor_removal
 * Inputs         : None
 * Output         : None
 * Returns        : son_void_t
 * Description    : This function removes neighbor on the basis of timer expiry
 ******************************************************************************/
son_void_t
anr_automatic_neighbor_removal
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    anr_cell_context_node_t *p_cell_ctxt = SON_PNULL;
    anr_intra_rat_hash_nr_tuple_t *p_eutran_tpl = SON_PNULL;
    anr_inter_rat_hash_nr_tuple_t *p_other_rat_tpl = SON_PNULL;
    time_stamp_t curr_ts;
    son_bool_et delete_nbr = SON_FALSE;
    son_error_et    error_code = SON_NO_ERROR;
    anr_intra_rat_hash_rec_t eutran_nbr;
    anr_inter_rat_hash_rec_t other_rat_nbr;
    son_global_cell_id_t glbl_cell_id;
    son_anr_updated_nrt_info_t     *p_oam_notif =SON_PNULL;
    son_anr_rrm_updated_nrt_info_t *p_notification_msg = SON_PNULL;
    son_x2_nrt_update_ind_t nrt_upd_ind      = {0};



    SON_UT_TRACE_ENTER();

    p_cell_ctxt = anr_get_first_cell_ctxt();
    p_oam_notif = (son_anr_updated_nrt_info_t *)
        son_mem_get(sizeof(son_anr_updated_nrt_info_t));
    if (SON_PNULL == p_oam_notif)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                " Memory allocation failure. Auto NR removal stops");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_notification_msg = (son_anr_rrm_updated_nrt_info_t *)
        son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));
    if (SON_PNULL == p_notification_msg)
    {
        if (SON_PNULL != p_oam_notif)
        {
            son_mem_free(p_oam_notif);    
        }

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                " Memory allocation failure. Auto NR removal stops");
        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMSET(&eutran_nbr, 0, sizeof(eutran_nbr));
    SON_MEMSET(&other_rat_nbr, 0, sizeof(other_rat_nbr));
    SON_MEMSET(&glbl_cell_id, 0, sizeof(glbl_cell_id));

    while (SON_PNULL != p_cell_ctxt)
    {
        if (ANR_CELL_STATE_ENABLED == p_cell_ctxt->data.current_cell_fsm_state)
        {
            if (p_cell_ctxt->data.removal_criteria.no_activity_interval ||
                    p_cell_ctxt->data.removal_criteria.timeout_no_nr_neighbors)
                SON_MEMSET(p_notification_msg, 0, sizeof(son_anr_rrm_updated_nrt_info_t));
            SON_MEMSET(p_oam_notif, 0, sizeof(son_anr_updated_nrt_info_t));

            SON_MEMCPY(&p_notification_msg->src_cgi,
                    &p_cell_ctxt->data.cell_id,
                    sizeof(son_intra_rat_global_cell_id_t));

            SON_MEMCPY(&p_oam_notif->src_cgi,
                    &p_cell_ctxt->data.cell_id,
                    sizeof(son_intra_rat_global_cell_id_t));

            if (p_cell_ctxt->data.removal_criteria.no_activity_interval ||
                    p_cell_ctxt->data.removal_criteria.timeout_no_nr_neighbors)
            {
                p_eutran_tpl = SON_PNULL;
                p_eutran_tpl = anr_get_first_nr_from_intra_rat_hash_table(&p_cell_ctxt->data.intra_rat_neighbor_table);
                while (SON_PNULL != p_eutran_tpl)
                {
                    delete_nbr = SON_FALSE;
                    son_time(&curr_ts);
                    error_code = SON_NO_ERROR;

                    if (p_cell_ctxt->data.removal_criteria.no_activity_interval &&
                            p_eutran_tpl->data.last_activity_ts &&
                            (((son_u32)son_difftime(curr_ts, p_eutran_tpl->data.last_activity_ts)) >=
                             (p_cell_ctxt->data.removal_criteria.no_activity_interval * 60)))
                    {
                        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                                SON_BRIEF,
                                "No Activity timer expired for NR [Cell Id: 0x%x]",
                                son_convert_char_cell_id_to_int_cell_id(p_eutran_tpl->data.neighbor_info.nbr_cgi.
                                    cell_identity));

                        delete_nbr = SON_TRUE;
                    }

                    if (p_cell_ctxt->data.removal_criteria.timeout_no_nr_neighbors &&
                            p_eutran_tpl->data.no_nr_neighbors_ts &&
                            (((son_u32)son_difftime(curr_ts, p_eutran_tpl->data.no_nr_neighbors_ts)) >=
                             (p_cell_ctxt->data.removal_criteria.timeout_no_nr_neighbors * 60)))
                    {
                        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                                SON_BRIEF,
                                "No NR neighbors timer expired for  NR [Cell Id: 0x%x]",
                                son_convert_char_cell_id_to_int_cell_id(p_eutran_tpl->data.neighbor_info.nbr_cgi.
                                    cell_identity));

                        delete_nbr = SON_TRUE;
                    }

                    if (delete_nbr &&
                            SON_NR_UNLOCKED == p_eutran_tpl->data.neighbor_info.nr_status)
                    {
                        anr_intra_rat_del_nr(&p_cell_ctxt->data,
                                &p_eutran_tpl->data.neighbor_info.nbr_cgi,
                                SON_TRUE,
                                &error_code,
                                &eutran_nbr,
                                p_oam_notif,
                                p_notification_msg,
                                SON_TRUE);
                        p_eutran_tpl = anr_get_first_nr_from_intra_rat_hash_table(
                                &p_cell_ctxt->data.intra_rat_neighbor_table);
                    }
                    else
                    {
                        if (SON_TRUE == delete_nbr)
                        {
                            SON_LOG(
                                    anr_get_log_mode(), p_son_anr_facility_name,
                                    SON_BRIEF,
                                    "anr_automatic_neighbor_removal: Intra-RAT NR [Cell Id: 0x%x] not deleted as NR_STATUS is LOCKED\n",
                                    son_convert_char_cell_id_to_int_cell_id(p_eutran_tpl->data.neighbor_info.nbr_cgi.
                                        cell_identity));
                        }

                        p_eutran_tpl = anr_get_next_nr_from_intra_rat_hash_table(
                                &p_cell_ctxt->data.intra_rat_neighbor_table, &p_eutran_tpl->anchor);
                    }
                }

                p_other_rat_tpl = SON_PNULL;
                p_other_rat_tpl =
                    anr_get_first_nr_from_inter_rat_hash_table(&p_cell_ctxt->data.inter_rat_neighbor_table);
                while (SON_PNULL != p_other_rat_tpl)
                {
                    delete_nbr = SON_FALSE;
                    son_time(&curr_ts);
                    error_code = SON_NO_ERROR;

                    if (p_cell_ctxt->data.removal_criteria.no_activity_interval &&
                            p_other_rat_tpl->data.last_activity_ts &&
                            (((son_u32)son_difftime(curr_ts, p_other_rat_tpl->data.last_activity_ts)) >=
                             (p_cell_ctxt->data.removal_criteria.no_activity_interval * 60)))
                    {
                        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                                SON_BRIEF,
                                "anr_automatic_neighbor_removal: No Activity timer expired for "
                                " NR [Cell Id: 0x%x]\n",
                                anr_get_cell_identity_from_inter_nr_rec(&p_other_rat_tpl->data));

                        delete_nbr = SON_TRUE;
                    }

                    if (p_cell_ctxt->data.removal_criteria.timeout_no_nr_neighbors &&
                            p_other_rat_tpl->data.no_nr_neighbors_ts &&
                            (((son_u32)son_difftime(curr_ts, p_other_rat_tpl->data.no_nr_neighbors_ts)) >=
                             (p_cell_ctxt->data.removal_criteria.timeout_no_nr_neighbors * 60)))
                    {
                        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                                SON_BRIEF,
                                "anr_automatic_neighbor_removal: No NR neighbors timer expired for "
                                " NR [Cell Id: 0x%x]\n",
                                anr_get_cell_identity_from_inter_nr_rec(&p_other_rat_tpl->data));

                        delete_nbr = SON_TRUE;
                    }

                    if ((ANR_UTRAN_NEIGHBOR == p_other_rat_tpl->data.t &&
                                SON_NR_LOCKED == p_other_rat_tpl->data.u.utran_neighbor_info.nr_status) ||
                            (ANR_GERAN_NEIGHBOR == p_other_rat_tpl->data.t &&
                             SON_NR_LOCKED == p_other_rat_tpl->data.u.geran_neighbor_info.nr_status))
                    {
                        SON_LOG(
                                anr_get_log_mode(), p_son_anr_facility_name,
                                SON_BRIEF,
                                "anr_automatic_neighbor_removal: Inter-RAT NR [Cell Id: 0x%x] not deleted as NR_STATUS is LOCKED\n",
                                anr_get_cell_identity_from_inter_nr_rec(&p_other_rat_tpl->data));

                        delete_nbr = SON_FALSE;
                    }

                    if (delete_nbr)
                    {
                        anr_extract_global_cell_id_from_inter_rat_hash_rec(&p_other_rat_tpl->data, &glbl_cell_id);

                        anr_inter_rat_del_nr(&p_cell_ctxt->data,
                                &glbl_cell_id.inter_rat_global_cell_id,
                                SON_TRUE,
                                &error_code,
                                &other_rat_nbr,
                                p_oam_notif,
                                p_notification_msg);

                        p_other_rat_tpl = anr_get_first_nr_from_inter_rat_hash_table(
                                &p_cell_ctxt->data.inter_rat_neighbor_table);
                    }
                    else
                    {
                        p_other_rat_tpl = anr_get_next_nr_from_inter_rat_hash_table(
                                &p_cell_ctxt->data.inter_rat_neighbor_table,
                                &p_other_rat_tpl->anchor);    
                    }

                }

                /*Send NRT update notification to OAM */
                anr_send_updated_ncl_to_oam(p_oam_notif);
                    /* Spr 17753 Changes Start */
                    if(0 <  p_oam_notif->nr_list.intra_rat_nr_list_size)
                    {
                        anr_find_conflict_in_nrt_n_send_conflict_ind(&p_cell_ctxt->data);
                    }
                    /* Spr 17753 Changes End*/
                if (p_oam_notif->nr_list.intra_rat_nr_list_size && SON_TRUE ==  is_x2_connection_available())
                {
                    nrt_upd_ind.is_local_update = SON_FALSE;
                    anr_populate_x2_nrt_update_ind(&nrt_upd_ind, &p_cell_ctxt->data.cell_id, SON_NRT_OP_UPDATE);
                }

                /*Send NRT update notification to MLB */
                if (MLB_CELL_STATE_ENABLED == mlb_get_cell_mode(&p_cell_ctxt->data.cell_id))
                {
                    anr_send_updated_ncl_to_mlb(p_oam_notif);
                }

                /*Send NRT update notification to RRM */
                anr_send_updated_ncl_to_rrm(p_notification_msg,
                        son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->data.cell_id.cell_identity));
            }
        }

        p_cell_ctxt = anr_get_next_node(p_cell_ctxt);
    }

	if (nrt_upd_ind.num_srv_cells_list_size)
	{
		son_create_send_buffer((son_u8 *)&nrt_upd_ind, SON_ANR_MODULE_ID,
				SON_X2_MODULE_ID, SONX2_NRT_UPDATE_IND,
				sizeof(son_x2_nrt_update_ind_t));
	}

	if (SON_PNULL != p_oam_notif)
	{
		son_mem_free(p_oam_notif);
	}

	if (SON_PNULL != p_notification_msg)
	{
		son_mem_free(p_notification_msg);
	}

	SON_UT_TRACE_EXIT();
} /* anr_automatic_neighbor_removal */

/*SPR 13795 Changes - End*/

/*****************************************************************************
 * Function Name  : anr_get_intra_rat_ho_status
 * Inputs         : p_nr    - Pointer to Intra Rat Hash Rec
 * Outputs        : None
 * Returns        : son_ho_status_et
 * Description    : This function returns HO Status of Intra-RAT neighbors
 ******************************************************************************/
son_ho_status_et
anr_get_intra_rat_ho_status
(
 anr_intra_rat_hash_rec_t *p_nr
 )
{
	son_ho_status_et  ret = SON_HO_PROHIBITED;


	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_nr)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_nr is NULL");
	}
	else if (SON_HO_ALLOWED ==  p_nr->neighbor_info.ho_status)
	{
		ret = SON_HO_ALLOWED;
	}
	else if (SON_NR_LOCKED  ==  p_nr->neighbor_info.nr_status)
	{
		ret = SON_HO_UNDEFINED;
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* anr_get_intra_rat_ho_status */

/******************************************************************************
 * Function Name  : anr_convert_intra_rat_nr_to_global_cell_id_struct
 * Inputs         : p_nr      -  Pointer to a structure containing the NR
 *                            information
 *                  p_cell_id - Pointer to cell id
 * Outputs        : son_global_cell_id_t* - Global cell id structure.
 * Returns        : None.
 * Description    : This function is used to populate Global cell id using the
 *                  intra rat neighbor info.
 ******************************************************************************/
son_void_t
anr_convert_intra_rat_nr_to_global_cell_id_struct
(
 const son_intra_rat_neighbor_info_t *p_nr,
 son_global_cell_id_t *p_cell_id
 )
{
	son_oam_log_on_off_et         log_mode     = anr_get_log_mode();


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_nr || SON_PNULL == p_cell_id)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_convert_intra_rat_nr_to_global_cell_id_struct: Could not "
				"populate the global cell id structure, p_nr or p_cell_id is NULL\n");
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMSET(p_cell_id, 0, sizeof(son_global_cell_id_t));

	SON_MEMCPY(&p_cell_id->intra_rat_global_cell_id,
			&p_nr->nbr_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	p_cell_id->bitmask = SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;

	SON_UT_TRACE_EXIT();
} /* anr_convert_intra_rat_nr_to_global_cell_id_struct */

/******************************************************************************
 * Function Name  : anr_convert_inter_rat_nr_to_global_cell_id_struct
 * Inputs         : p_nr        -  Pointer to a structure containing the NR
 *                            information
 *                  p_cell_id   - Pointer to cell id
 * Outputs        : son_global_cell_id_t* - Global cell id structure.
 * Returns        : None.
 * Description    : This function is used to populate Global cell id using the
 *                  inter rat neighbor info.
 ******************************************************************************/
son_void_t
anr_convert_inter_rat_nr_to_global_cell_id_struct
(
 const son_inter_rat_neighbor_info_t *p_nr,
 son_global_cell_id_t *p_cell_id
 )
{
	son_oam_log_on_off_et         log_mode     = anr_get_log_mode();


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_nr || SON_PNULL == p_cell_id)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_convert_inter_rat_nr_to_global_cell_id_struct: Could not "
				"populate the global cell id structure, p_nr or p_cell_id is NULL\n");
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMSET(p_cell_id, 0, sizeof(son_global_cell_id_t));

	/*Klockwork fix*/
	if (SON_INTER_UTRAN_NEIGHBOR == p_nr->bitmask)
	{
		SON_MEMCPY(&p_cell_id->inter_rat_global_cell_id.utran_geran_global_cell_id,
				&p_nr->utran_neighbor.nbr_cgi, sizeof(son_utran_geran_global_cell_id_t));
	}
	else if (SON_INTER_GERAN_NEIGHBOR == p_nr->bitmask)
	{
		SON_MEMCPY(&p_cell_id->inter_rat_global_cell_id.utran_geran_global_cell_id,
				&p_nr->geran_neighbor.nbr_cgi, sizeof(son_utran_geran_global_cell_id_t));
	}
	else
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"anr_convert_inter_rat_nr_to_global_cell_id_struct: Invalid bitmask[0x%x]\n",
                p_nr->bitmask);
		SON_UT_TRACE_EXIT();
		return;
	}

	p_cell_id->inter_rat_global_cell_id.bitmask |= SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT;     
	p_cell_id->bitmask = SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT;

	SON_UT_TRACE_EXIT();
} /* anr_convert_inter_rat_nr_to_global_cell_id_struct */

/******************************************************************************
 * Function Name  : anr_init_db
 * Inputs         : None
 * Output         : None
 * Returns        : son_return_et - return value for SON APIs
 * Description    : This function initialize db for anr
 ******************************************************************************/
son_return_et
anr_init_db
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	son_return_et ret_val = SON_SUCCESS;


	SON_UT_TRACE_ENTER();

	do
	{
		if (SON_SUCCESS != (ret_val = anr_init_hash_node_table()))
		{
			break;
		}

		if (SON_SUCCESS != (ret_val = anr_init_intra_rat_hash_rec_table()))
		{
			anr_destroy_hash_node_table();
			break;
		}

		if (SON_SUCCESS != (ret_val = anr_init_inter_rat_hash_rec_table()))
		{
			anr_destroy_hash_node_table();
			anr_destroy_intra_rat_hash_rec_table();
			break;
		}
	} while (0);

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_init_db */

/******************************************************************************
 * Function Name  : anr_clear_db
 * Inputs         : None
 * Output         : None
 * Returns        : son_void_t - None
 * Description    : This function clears db for anr
 ******************************************************************************/
son_void_t
anr_clear_db
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	anr_destroy_intra_rat_hash_rec_table();
	anr_destroy_inter_rat_hash_rec_table();
	anr_destroy_hash_node_table();
	SON_UT_TRACE_EXIT();
}

/*PCI_CONFLICT_CONFUSION_CHANGES_START*/


/***************************************************************************************
 * Function Name  : anr_construct_sent_pci_conflict_confusion_ind
 * Inputs         : p_cell_id         - Pointer to intra rat global cell id
 *                  p_cell_ctxt       - Pointer to cell context
 *                  pci               - PCI value
 *                  earfcn            - EARRCN value
 *                  pci_cause         - Conflict/ Confusion
 *                  count             - Number of NBRs in conflict confusion list
 * Returns        : son_void_t        - None
 * Description    : This function composes the pci conflict / confusion indication message
 ****************************************************************************************/
son_void_t
anr_construct_sent_pci_conflict_confusion_ind
(
 son_intra_rat_global_cell_id_t *p_cell_id,
 anr_cell_context_t           *p_cell_ctxt,
 son_u16 pci,
 son_u32 earfcn,
 son_pci_cause_et pci_cause,
 son_u32 count
 )
{
	son_anr_pci_conflict_confusion_ind_t msg;
	anr_intra_rat_hash_nr_tuple_t *p_hash_rec = SON_PNULL;


	SON_UT_TRACE_ENTER();
	if (0 ==  count || SON_PNULL == p_cell_id || 1 < pci_cause)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
				"anr_construct_sent_pci_conflict_confusion_ind:"
				"Invalid params : either the nbr_cell_count value is 0 or "
				"the p_cell_id is NULL or the value of pci_case is incorrect ");

		/* Coverity Fix: 23469 */
		return;
	}

	SON_MEMSET(&msg, 0, sizeof(son_anr_pci_conflict_confusion_ind_t));
	msg.src_cgi.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT; /*SPR-12596-Start-End*/
	msg.cgi_1.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
	msg.cgi_2.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;

	msg.pci            = pci;
	msg.cause          = pci_cause;

	/*SPR-12596-Start*/

	SON_MEMCPY(&msg.src_cgi.intra_rat_global_cell_id,
			&p_cell_ctxt->cell_id, sizeof(son_intra_rat_global_cell_id_t));

	/*SPR-12596-End*/

	SON_MEMCPY(&msg.cgi_1.intra_rat_global_cell_id,
			p_cell_id, sizeof(son_intra_rat_global_cell_id_t));

	p_hash_rec = anr_get_first_nr_from_intra_rat_hash_table(
			&p_cell_ctxt->intra_rat_neighbor_table);

	while (SON_PNULL != p_hash_rec && 0 != count)
	{
		if (p_hash_rec->data.neighbor_info.pci == pci &&
				p_hash_rec->data.neighbor_info.earfcn == earfcn &&
				SON_MEMCMP(p_cell_id, &p_hash_rec->data.neighbor_info.nbr_cgi,
					sizeof(son_intra_rat_global_cell_id_t)))
		{
			count--;
			SON_MEMCPY(&msg.cgi_2.intra_rat_global_cell_id,
					&p_hash_rec->data.neighbor_info.nbr_cgi,
					sizeof(son_intra_rat_global_cell_id_t));

			/*SPR-12596-Start*/
			anr_insert_in_conflict_confusion_pci_list(p_cell_ctxt, msg.cgi_1.intra_rat_global_cell_id, 
					msg.cgi_2.intra_rat_global_cell_id, msg.pci, msg.cause);
			/*SPR-12596-End*/

			son_create_send_buffer((son_void_t *)&msg, SON_ANR_MODULE_ID, SON_APPS_MODULE_ID,
					SONANR_PCI_CONFLICT_CONFUSION_IND, sizeof(son_anr_pci_conflict_confusion_ind_t));

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
					"anr_construct_sent_pci_conflict_confusion_ind:"
					"Cell id [Cell id: 0x%x] and cell id [Cell id:0x%x] are in [%d][ 0 - CONFLICT/1-CONFUSION ] "
					"with each other for PCI[%d] and EARFCN [%d]",
					son_convert_char_cell_id_to_int_cell_id(p_hash_rec->data.neighbor_info.nbr_cgi.cell_identity),
					son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity),
					pci_cause, pci, earfcn);
		}

		p_hash_rec = anr_get_next_nr_from_intra_rat_hash_table(&p_cell_ctxt->intra_rat_neighbor_table,
				&p_hash_rec->anchor );
	}

	SON_UT_TRACE_EXIT();
} /* anr_construct_sent_pci_conflict_confusion_ind */

/******************************************************************************
 * Function Name  : get_other_intra_rat_nr_with_given_pci_earfcn
 * Inputs         : p_hash   - Pointer to head of SON_LIST hash list
 *                : p_hash_rec - Pointer to the new hash rec inserted
 * Returns        : son_void_t          - None
 * Description    : This function retrieves other intra rat neighbours with know
 *                  earfcn and pci as input
 ******************************************************************************/
anr_intra_rat_hash_rec_t *
get_other_intra_rat_nr_with_given_pci_earfcn
(
 SON_HASH *p_hash,
 anr_intra_rat_hash_rec_t *p_hash_rec
 )
{
	SON_UT_TRACE_ENTER();
	anr_intra_rat_hash_nr_tuple_t *p_hash_tpl   = SON_PNULL;
	anr_intra_rat_hash_rec_t      *p_hash_data  = SON_PNULL;
	p_hash_tpl =  anr_get_first_nr_from_intra_rat_hash_table(p_hash);
	while (SON_PNULL != p_hash_tpl)
	{
		if (p_hash_rec->neighbor_info.pci == p_hash_tpl->data.neighbor_info.pci &&
				p_hash_rec->neighbor_info.earfcn == p_hash_tpl->data.neighbor_info.earfcn &&
				SON_MEMCMP(&p_hash_rec->neighbor_info.nbr_cgi,
					&p_hash_tpl->data.neighbor_info.nbr_cgi, sizeof(son_intra_rat_global_cell_id_t)))
		{
			p_hash_data = &p_hash_tpl->data;
			break;
		}

		p_hash_tpl = anr_get_next_nr_from_intra_rat_hash_table(p_hash, &p_hash_tpl->anchor);
	}

	SON_UT_TRACE_EXIT();
	return p_hash_data;
} /* get_other_intra_rat_nr_with_given_pci_earfcn */

/******************************************************************************
 * Function Name  : insert_pci_suspect_conflict_confusion_handler
 * Inputs         : p_cell_ctxt         - Pointer to Serving cell context
 *                  p_inserted_hash_rec - Pointer to the new hash rec inserted
 *                  p_oam_notifiation_update - Pointer to oam updated nrt info structure
 * Returns        : son_void_t          - None
 * Description    : This function inserts / add the record in the suspect list.
 ******************************************************************************/
son_void_t
insert_pci_suspect_conflict_confusion_handler
(
 anr_cell_context_t           *p_cell_ctxt,
 anr_intra_rat_hash_rec_t   *p_inserted_hash_rec,
 son_anr_updated_nrt_info_t *p_oam_notifiation_update
 )
{
	anr_intra_rat_hash_rec_t      *p_other_intra_rat_hash_rec    = SON_PNULL;
	anr_pci_sus_conf_node_t       *p_pci_sus_conf_node           = SON_PNULL;
	anr_pci_sus_conf_node_t       *p_inserted_pci_sus_conf_node  = SON_PNULL;
	son_u8                        clear_sus_pci_thresh           = 0;
	/* Coverity Fix: 23475 */
	son_anr_pci_conflict_confusion_ind_t  msg;


	SON_UT_TRACE_ENTER();

	/* Coverity Fix: 23475 */
	SON_MEMSET(&msg, 0, sizeof(son_anr_pci_conflict_confusion_ind_t));
	/*SPR-12596-Start*/
	msg.src_cgi.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
	msg.cgi_1.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
	/*SPR-12596-End*/
  /* Spr 17753 Changes Start */
    /* Code Removed */
    /* Spr 17753 Changes End */


	/*2. CONFUSION HANDLING*/
	p_other_intra_rat_hash_rec  = get_other_intra_rat_nr_with_given_pci_earfcn(
			&p_cell_ctxt->intra_rat_neighbor_table,
			p_inserted_hash_rec);

	p_pci_sus_conf_node         = anr_find_in_sus_pci_ctxt_list(p_cell_ctxt,
			p_inserted_hash_rec->neighbor_info.pci,
			p_inserted_hash_rec->neighbor_info.earfcn);

	if (SON_PNULL == p_other_intra_rat_hash_rec && SON_PNULL == p_pci_sus_conf_node)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
				"Neighbor [0x%x] is the only intra rat neighbor "
				"with PCI [%d] and EARFCN [%u] in intra rat NRT of cell [0x%x]",
				son_convert_char_cell_id_to_int_cell_id(p_inserted_hash_rec->neighbor_info.nbr_cgi.cell_identity),
				p_inserted_hash_rec->neighbor_info.pci,
				p_inserted_hash_rec->neighbor_info.earfcn,
				son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
		return;
	}
	else if (SON_PNULL  != p_other_intra_rat_hash_rec && SON_PNULL == p_pci_sus_conf_node)
	{
		p_inserted_pci_sus_conf_node   = anr_insert_in_sus_pci_ctxt_list(p_cell_ctxt,
				p_inserted_hash_rec->neighbor_info.pci,
				p_inserted_hash_rec->neighbor_info.earfcn,
				SON_PCI_CONFUSED, 2);
		if (SON_PNULL !=  p_inserted_pci_sus_conf_node)
		{
			/*1. Set the ho_failed_due_to_pci_conflict to 0 for newly identifed nr*/
			p_other_intra_rat_hash_rec->ho_failed_for_pci_conflict = 0;

			/*2. Set the received_cgi_counter of the newly identifed nr */
			if (SON_CLEAR_SUSPECT_PCI_THRESHOLD_PRESENT & p_cell_ctxt->pci_confusion_cfg.bitmask)
			{
				clear_sus_pci_thresh = p_cell_ctxt->pci_confusion_cfg.clear_suspect_pci_threshold;
			}

			p_other_intra_rat_hash_rec->received_cgi_counter = clear_sus_pci_thresh;

			/*3. Set the received_cgi_counter of the inserted nr */
			p_inserted_hash_rec->received_cgi_counter = clear_sus_pci_thresh;

			/*4. Trigger confusion indication*/
			if (SON_PCI_ENABLE ==  anr_get_pci_module_mode())
			{
				anr_construct_sent_pci_conflict_confusion_ind(&p_inserted_hash_rec->neighbor_info.nbr_cgi,
						p_cell_ctxt, p_inserted_hash_rec->neighbor_info.pci,
						p_inserted_hash_rec->neighbor_info.earfcn, SON_PCI_CONFUSION,
						p_inserted_pci_sus_conf_node->pci_sus_conf_data.nbr_cell_count - 1);
			}
		}
	}
	else if (SON_PNULL  != p_other_intra_rat_hash_rec && SON_PNULL != p_pci_sus_conf_node)
	{
		if (SON_PCI_SUSPECTED == p_pci_sus_conf_node->pci_sus_conf_data.state 
				&& SON_PNULL != p_oam_notifiation_update)
		{
			/*1. Make state CONFUSED*/
			p_pci_sus_conf_node->pci_sus_conf_data.state = SON_PCI_CONFUSED;

			/*2. Make the suspected pci field to FALSE of the newly identifed nr
			 * and send the change to the OAM via p_oam_notification msg */
			p_other_intra_rat_hash_rec->neighbor_info.suspected_pci = SON_FALSE;

			p_oam_notifiation_update->nrt_op = SON_NRT_OP_UPDATE;
			anr_construct_oam_updated_ncl(&p_oam_notifiation_update->nr_list,
					&p_other_intra_rat_hash_rec->neighbor_info, SON_FALSE);

			/*3. Set the activity timer in the newly identifed nr */
			son_time(&p_other_intra_rat_hash_rec->last_activity_ts);

			/*4. Increment the nbr count*/
			p_pci_sus_conf_node->pci_sus_conf_data.nbr_cell_count++;

			/*5. Set the received_cgi_counter of the inserted nr
			 * with the threshhold count*/
			p_inserted_hash_rec->received_cgi_counter =
				p_pci_sus_conf_node->pci_sus_conf_data.ask_cgi_thx_val;

			/*6. PCI confusion indication*/
			if (SON_PCI_ENABLE ==  anr_get_pci_module_mode())
			{
				anr_construct_sent_pci_conflict_confusion_ind(&p_inserted_hash_rec->neighbor_info.nbr_cgi,
						p_cell_ctxt, p_inserted_hash_rec->neighbor_info.pci,
						p_inserted_hash_rec->neighbor_info.earfcn, SON_PCI_CONFUSION,
						p_pci_sus_conf_node->pci_sus_conf_data.nbr_cell_count - 1);
			}
		}
		else if (SON_PCI_CONFUSED == p_pci_sus_conf_node->pci_sus_conf_data.state)
		{
			/*1. Increment the nbr count*/
			p_pci_sus_conf_node->pci_sus_conf_data.nbr_cell_count++;

			/*2. Set the receieved_cgi_counter of the inserted nr
			 * with the threshhold count*/
			p_inserted_hash_rec->received_cgi_counter =
				p_pci_sus_conf_node->pci_sus_conf_data.ask_cgi_thx_val;

			/*3. Trigger the pci confusion indication*/
			if (SON_PCI_ENABLE ==  anr_get_pci_module_mode())
			{
				anr_construct_sent_pci_conflict_confusion_ind(&p_inserted_hash_rec->neighbor_info.nbr_cgi,
						p_cell_ctxt, p_inserted_hash_rec->neighbor_info.pci,
						p_inserted_hash_rec->neighbor_info.earfcn, SON_PCI_CONFUSION,
						p_pci_sus_conf_node->pci_sus_conf_data.nbr_cell_count - 1);
			}
		}
	}

	SON_UT_TRACE_EXIT();
} /* insert_pci_suspect_conflict_confusion_handler */

/******************************************************************************
 * Function Name  : delete_pci_suspect_conflict_confusion_handler
 * Inputs         : p_cell_ctxt         - Pointer to Serving cell context
 *                : p_deleted_hash_rec  - Pointer to the new hash rec inserted
 * Output         : None
 * Returns        : son_void_t - None
 * Description    : This function inserts deletes the record in the suspect list.
 ******************************************************************************/
son_void_t
delete_pci_suspect_conflict_confusion_handler
(
 anr_cell_context_t         *p_cell_ctxt,
 anr_intra_rat_hash_rec_t   *p_deleted_hash_rec
 )
{
	anr_intra_rat_hash_rec_t      *p_other_intra_rat_hash_rec    = SON_PNULL;
	anr_pci_sus_conf_node_t       *p_pci_sus_conf_node           = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_deleted_hash_rec)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"p_deleted_hash_rec is NULL\n");
	}
	else
	{
		p_other_intra_rat_hash_rec  = get_other_intra_rat_nr_with_given_pci_earfcn(
				&p_cell_ctxt->intra_rat_neighbor_table,
				p_deleted_hash_rec);

		p_pci_sus_conf_node     = anr_find_in_sus_pci_ctxt_list(p_cell_ctxt,
				p_deleted_hash_rec->neighbor_info.pci,
				p_deleted_hash_rec->neighbor_info.earfcn);

		if (SON_PNULL == p_other_intra_rat_hash_rec && SON_PNULL == p_pci_sus_conf_node)
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
					"delete_pci_suspect_conflict_confusion_handler:No conflict, Confusion and Suspecison exits for "
					"PCI [%d] and EARFCN[%d] for the cell id[Cellid :0x%x]",
					p_deleted_hash_rec->neighbor_info.pci, p_deleted_hash_rec->neighbor_info.earfcn,
					son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
			return;
		}
		else if (SON_PNULL == p_other_intra_rat_hash_rec && SON_PNULL != p_pci_sus_conf_node)
		{
			anr_delete_pci_ctxt_from_sus_pci_ctxt_list(p_cell_ctxt,
					p_deleted_hash_rec->neighbor_info.pci,
					p_deleted_hash_rec->neighbor_info.earfcn);

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
					"delete_pci_suspect_conflict_confusion_handler: No conflict, Confusion and Suspecison exits for "
					"PCI [%d] and EARFCN[%d] for the cell id[Cellid :0x%x]",
					p_deleted_hash_rec->neighbor_info.pci, p_deleted_hash_rec->neighbor_info.earfcn,
					son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
		}
		else if (SON_PNULL != p_other_intra_rat_hash_rec && SON_PNULL != p_pci_sus_conf_node)
		{
			if (2 >= p_pci_sus_conf_node->pci_sus_conf_data.nbr_cell_count)
			{
				anr_delete_pci_ctxt_from_sus_pci_ctxt_list(p_cell_ctxt,
						p_deleted_hash_rec->neighbor_info.pci,
						p_deleted_hash_rec->neighbor_info.earfcn);

				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
						"delete_pci_suspect_conflict_confusion_handler: No conflict, Confusion and Suspecison exits for "
						"PCI [%d] and EARFCN[%d] for the cell id[Cellid :0x%x]",
						p_deleted_hash_rec->neighbor_info.pci, p_deleted_hash_rec->neighbor_info.earfcn,
						son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
			}
			else
			{
				p_pci_sus_conf_node->pci_sus_conf_data.nbr_cell_count--;
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
						"delete_pci_suspect_conflict_confusion_handler:Neighbor Cell count decremented ");
				anr_print_pci_ctxt_from_sus_pci_ctxt_list(p_pci_sus_conf_node);
			}
		}
	}
	SON_UT_TRACE_EXIT();
} /* delete_pci_suspect_conflict_confusion_handler */

/******************************************************************************
 * Function Name  : anr_print_eutran_nbr_mro_info
 * Inputs         : p_eutran_rec    - Pointer to INTRA RAT NRT hash record
 * Output         : None
 * Returns        : son_void_t      - None
 * Description    : This funciton prints the MRO specific parameters for a 
 *                  particular record of INTRA RAT NRT.
 ******************************************************************************/
son_void_t
anr_print_eutran_nbr_mro_info
(
 anr_intra_rat_hash_rec_t *p_eutran_rec
 )
{
	SON_UT_TRACE_ENTER();

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_BRIEF,
			"Neighbor Cell [Cell Id: 0x%x] has following MRO attributes:\n"
			"num_too_early_ho: %u\n"
			"num_too_late_ho: %u\n"
			"num_ho_to_wrong_cell_for_failure_cell: %u\n"
			"num_ho_to_wrong_cell_for_reestablishment_cell: %u\n"
			"total_ho_attempted: %u\n"
			"is_cio_algo_running: %u\n"
			"retry_count: %u\n"
			"previous CIO: %s\n"
			"Previous HO failure rate: %f\n"
			"Optmization for: %u\n"
			"Is in use by MLB: %u\n",
			son_convert_char_cell_id_to_int_cell_id(p_eutran_rec->neighbor_info.nbr_cgi.cell_identity),
			p_eutran_rec->mro_data.mro_ho_counter.num_too_early_ho,
			p_eutran_rec->mro_data.mro_ho_counter.num_too_late_ho,
			p_eutran_rec->mro_data.mro_ho_counter.
			num_ho_to_wrong_cell_for_failure_cell,
			p_eutran_rec->mro_data.mro_ho_counter.
			num_ho_to_wrong_cell_for_reestablishment_cell,
			p_eutran_rec->mro_data.mro_ho_counter.total_ho_attempted,
			p_eutran_rec->mro_data.is_cio_algo_running,
			p_eutran_rec->mro_data.retry_count,
			CIO_NAMES[p_eutran_rec->mro_data.prev_cio],
			p_eutran_rec->mro_data.prev_ho_fail_rate,
			p_eutran_rec->mro_data.optimizaton_for,
			p_eutran_rec->mro_data.in_use_by_mlb);

	SON_UT_TRACE_EXIT();
} /* anr_print_eutran_nbr_mro_info */

/******************************************************************************
 * Function Name  : anr_validate_eutran_neighbor_for_l3
 * Inputs         : p_hash          - Pointer to ANR intra rat hash record
 * Outputs        : None.
 * Returns        : son_bool_et     - SON_TRUE / SON_FALSE
 * Description    : This function validates the eutran neighbours for L3
 ******************************************************************************/
son_bool_et
anr_validate_eutran_neighbor_for_l3
(
 anr_intra_rat_hash_rec_t *p_hash
 )
{
	son_bool_et  ret = SON_FALSE;
	SON_UT_TRACE_ENTER();

	if (p_hash == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,SON_BRIEF,
				"p_hash is NULL")
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	if (SON_INTRA_CSG_ID_PRESENT & p_hash->neighbor_info.bitmask)
	{
		if((SON_ACCESS_MODE_PRESENT & p_hash->neighbor_info.bitmask) &&
				(p_hash->neighbor_info.access_mode == SON_CLOSED||
				 p_hash->neighbor_info.access_mode == SON_HYBRID))
		{
			ret = SON_TRUE;
		}
	}
	else
	{
        /*SPR_19279_START*/
		if((SON_ACCESS_MODE_PRESENT & p_hash->neighbor_info.bitmask) &&
				((p_hash->neighbor_info.access_mode == SON_OPEN) ||
                 (p_hash->neighbor_info.access_mode == SON_OPEN_FEMTO)))
        /*SPR_19279_END*/
		{
			ret = SON_TRUE;
		}
	}
	SON_UT_TRACE_EXIT();
	return ret;
}
/*SPR 15799 Fix Start*/
/*****************************************************************************
 * Function Name  : prepare_x2_status_for_nbr_cell_from_enb_ctxt 
 * Inputs         : p_cell_ctxt        - Pointer to cell context
 *                  p_nr               -  Pointer to incoming message
 * Outputs        : None
 * Returns        : void 
 * Description    : This function x2status of the a new neighbor provided by network 
 ****************************************************************************/
son_x2_status_et  prepare_x2_status_for_nbr_cell_from_enb_ctxt(
                    son_global_enb_id_t      *p_enb_id,
                    anr_intra_rat_hash_rec_t *p_nr,
                    son_bool_et              *p_is_match_found)
{
    son_x2_status_et  nr_x2_status                          = SON_X2_ALLOWED_DISCONNECTED;     
    son_nbr_enb_hash_tuple_t        *p_enb_ctxt             = SON_PNULL;
    son_enb_type_et                 enb_type                = SON_MACRO_ENB;
    son_error_et                    error_code              = SON_NO_ERROR;
    son_nbr_enb_hash_rec_t          *p_enb_hash_rec         = SON_PNULL;


    SON_UT_TRACE_ENTER();
    p_enb_ctxt   =  x2_find_nbr_enb(x2_get_enb_nrt(),
            p_enb_id,&error_code);

    *p_is_match_found = SON_FALSE; 
    if (SON_PNULL != p_enb_ctxt)
    {
        *p_is_match_found = SON_TRUE; 
        p_enb_hash_rec = &p_enb_ctxt->data;   
        if((SON_NR_LOCKED == p_enb_hash_rec->nr_status) &&
                (X2_ENB_STATE_DISCONNECTED == p_enb_hash_rec->enb_curr_fsm_state))
        {
            nr_x2_status = SON_X2_BLACKLISTED;
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_BRIEF, "prepare_x2_status_for_nbr_cell_from_enb_ctxt:For NR [Cell Id: 0x%x] "
                    "marking X2 status[%d]\n",
                    son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity),
                    nr_x2_status);
        }
    }

    /*if the value calculated is X2_BLACKLISTED, it means that
      the eNB was X2 Blacklisted.*/    
    if((SON_X2_BLACKLISTED != nr_x2_status) && (SON_PNULL !=  p_enb_hash_rec))
    {
        nr_x2_status = get_x2_sts_value_for_nbr_from_enb_rec(&p_enb_ctxt->data);

    /* Coverity 86422 + */
	if(p_nr->enb_id.enb_type == SON_MACRO_ENB)   {
        enb_type     = p_enb_ctxt->data.enb_id.enb_type;  }
    /* Coverity 86422 - */

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_BRIEF, "prepare_x2_status_for_nbr_cell_from_enb_ctxt:For NR [Cell Id: 0x%x] "
                "Computed X2 status[%d]\n",
                son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity),
                nr_x2_status);

        if ((SON_HOME_ENB == enb_type) &&
                (p_nr->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT) &&
                (p_nr->neighbor_info.access_mode == SON_OPEN))
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR, "prepare_x2_status_for_nbr_cell_from_enb_ctxt:For NR [Cell Id: 0x%x] "
                    "mark X2 status:SON_X2_ALLOWED_DISCONNECTED ,eNB is HOME eNB and cell is of MACRO eNB\n",
                    son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));
            *p_is_match_found = SON_FALSE; 
            SON_UT_TRACE_EXIT();
            return SON_X2_ALLOWED_DISCONNECTED;
        }

        if ((SON_MACRO_ENB == enb_type) &&
                ( 
                 ((p_nr->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT)&& (p_nr->neighbor_info.access_mode != SON_OPEN)) ||
                 (p_nr->neighbor_info.bitmask & SON_INTRA_CSG_ID_PRESENT)
                )
           ) 
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR, "prepare_x2_status_for_nbr_cell_from_enb_ctxt:For NR [Cell Id: 0x%x] "
                    "mark X2 status:SON_X2_ALLOWED_DISCONNECTED ,eNB is MACRO eNB and cell is not of  MACRO eNB\n",
                    son_convert_char_cell_id_to_int_cell_id(p_nr->neighbor_info.nbr_cgi.cell_identity));
            *p_is_match_found = SON_FALSE; 
            SON_UT_TRACE_EXIT();
            return SON_X2_ALLOWED_DISCONNECTED;
        }
    }

    SON_UT_TRACE_EXIT();
    return nr_x2_status;
}

/*****************************************************************************
 * Function Name  : son_anr_set_x2_status_for_new_neighbor 
 * Inputs         : p_cell_ctxt        - Pointer to cell context
 *                  p_nr               -  Pointer to incoming message
 * Outputs        : None
 * Returns        : void 
 * Description    : This function shall compute the access mode and the 
 *                  x2status of the a new neighbor provided by network 
 ****************************************************************************/
son_void_t  son_anr_set_x2_status_for_new_neighbor(anr_cell_context_t *p_cell_ctxt,
                               anr_intra_rat_hash_rec_t *p_nr,
                               son_global_enb_id_t      *p_enb_id
                               )
{
    son_x2_status_et                nr_x2_status            = SON_X2_ALLOWED_DISCONNECTED;     
    son_bool_et                     search_for_enb          = SON_FALSE;
    son_bool_et                     is_enb_match_found      = SON_FALSE;
    son_bool_et                     is_access_mode_stored   = SON_FALSE;
    son_global_enb_id_t             enb_id;
    son_access_mode_et              access_mode             = SON_CLOSED;
    
    SON_UT_TRACE_ENTER();

    if((SON_PNULL == p_cell_ctxt) || (SON_PNULL == p_nr))
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
                "son_anr_set_x2_status_for_new_neighbor: "
                "NULL pointer is recived \n");
    }
    else 
    {
        SON_MEMSET(&enb_id, 0, sizeof(enb_id));

        /*If access mode is computed for a neighbor,store it locally*/
        if((SON_ACCESS_MODE_PRESENT & p_nr->neighbor_info.bitmask))
        {
            access_mode = p_nr->neighbor_info.access_mode;
            p_nr->neighbor_info.bitmask &= ~(SON_ACCESS_MODE_PRESENT);
            is_access_mode_stored  = SON_TRUE;
        }
        
        /*Determine the access mode as per the pci list available */
        if(SON_FALSE == anr_set_correct_access_mode_of_intra_rat_nbr(p_nr))
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
                    "son_anr_set_x2_status_for_new_neighbor: "
                    "Error in Access Mode calcuation \n");
        }
        
        /*Reset the access mode from the local storage */
        if(!(SON_ACCESS_MODE_PRESENT & p_nr->neighbor_info.bitmask) && (SON_TRUE == is_access_mode_stored))
        {
            p_nr->neighbor_info.access_mode = access_mode;
            p_nr->neighbor_info.bitmask |= SON_ACCESS_MODE_PRESENT;
        }

        if((SON_PNULL !=  p_enb_id))
        {
            SON_MEMCPY(&enb_id,p_enb_id,sizeof(son_global_enb_id_t));
        }
        else if((p_nr->enb_id.enb_type  == SON_HOME_ENB) || (p_nr->enb_id.enb_type == SON_MACRO_ENB))
        {
                SON_MEMCPY(&enb_id,&p_nr->enb_id, sizeof(enb_id));
        }
        else
        {
            search_for_enb = SON_TRUE; 
        }

        /* Coverity 86421 + */
        if( (SON_FALSE == search_for_enb) &&
            (SON_X2_ALLOWED_DISCONNECTED == nr_x2_status) )
        /* Coverity 86421 - */
        {
            nr_x2_status  = prepare_x2_status_for_nbr_cell_from_enb_ctxt(&enb_id,p_nr,&is_enb_match_found);
            p_nr->neighbor_info.bitmask |= SON_ENABLE_X2_STATUS_PRESENT;
            p_nr->neighbor_info.x2_status = nr_x2_status;
        }
        else
        {
            /*Search the enb table for the FEMTO eNB*/
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_DETAILED,
                    "son_anr_set_x2_status_for_new_neighbor: "
                    "Searching for eNB context,assuming the neighbor belongs to the HOME eNB \n");
            enb_id.enb_type = SON_HOME_ENB;
            /*SPR 15899 Fix Start*/
            SON_MEMCPY(&enb_id.plmn_id,
                    &p_nr->neighbor_info.nbr_cgi.plmn_id,
                    sizeof(son_cell_plmn_info_t));
            /*SPR 15899 Fix Stop*/

            enb_id.enb_id[0] = p_nr->neighbor_info.nbr_cgi.cell_identity[0];
            enb_id.enb_id[1] = p_nr->neighbor_info.nbr_cgi.cell_identity[1];
            enb_id.enb_id[2] = p_nr->neighbor_info.nbr_cgi.cell_identity[2];
            enb_id.enb_id[3] = p_nr->neighbor_info.nbr_cgi.cell_identity[3] & 0xF0;

            nr_x2_status = prepare_x2_status_for_nbr_cell_from_enb_ctxt(&enb_id,p_nr,&is_enb_match_found);
            if(SON_TRUE == is_enb_match_found)
            {
                p_nr->neighbor_info.bitmask |= SON_ENABLE_X2_STATUS_PRESENT;
                p_nr->neighbor_info.x2_status = nr_x2_status;
                if(!(p_nr->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT)&&!(p_nr->neighbor_info.bitmask & SON_INTRA_CSG_ID_PRESENT))
                {
                   p_nr->neighbor_info.bitmask |= SON_ACCESS_MODE_PRESENT;
                   /* SPR 20007 : CID 86934 fix start */
                   p_nr->neighbor_info.access_mode = SON_OPEN_FEMTO;  
                   /* SPR 20007 : CID 86934  fix end */
                
                }
                SON_UT_TRACE_EXIT();
                return;
            }

            /*Search the enb table for the MACRO eNB*/
            SON_MEMSET(&enb_id, 0, sizeof(enb_id));
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_DETAILED,
                    "son_anr_set_x2_status_for_new_neighbor: "
                    "Searching for eNB Context, assuming the neighbor belongs to the MACRO eNB \n");
            enb_id.enb_type = SON_MACRO_ENB;
            /*SPR 15899 Fix Start*/
            SON_MEMCPY(&enb_id.plmn_id,
                    &p_nr->neighbor_info.nbr_cgi.plmn_id,
                    sizeof(son_cell_plmn_info_t));
            /*SPR 15899 Fix Stop*/
            anr_determine_macro_enb_id(&p_nr->neighbor_info.nbr_cgi,&enb_id);
            nr_x2_status = prepare_x2_status_for_nbr_cell_from_enb_ctxt(&enb_id,p_nr,&is_enb_match_found);
            if(SON_TRUE == is_enb_match_found)
            {
                p_nr->neighbor_info.bitmask |= SON_ENABLE_X2_STATUS_PRESENT;
                p_nr->neighbor_info.x2_status = nr_x2_status;
                if(!(p_nr->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT)&&!(p_nr->neighbor_info.bitmask & SON_INTRA_CSG_ID_PRESENT))
                {
                   p_nr->neighbor_info.bitmask |= SON_ACCESS_MODE_PRESENT;
                   /* SPR 20007 : CID 86934 fix start */
                   p_nr->neighbor_info.access_mode = SON_OPEN;  
                   /* SPR 20007 : CID 86934 fix end */
                }
                SON_UT_TRACE_EXIT();
                return;
            }
            /*Fill the deafult value*/
            p_nr->neighbor_info.bitmask |= SON_ENABLE_X2_STATUS_PRESENT;
            p_nr->neighbor_info.x2_status = nr_x2_status;
        }
    }
    SON_UT_TRACE_EXIT();
    return;
}
/*SPR 15799 Fix Stop*/
/* Rel 3.0 Multi Sector code changes Start */
/*****************************************************************************
 * Function Name  : populate_intra_rat_hash_rec_for_autonomous_add_mod_op
 * Inputs         : p_srv_cell_id - Pointer to the serving cell Id 
 * Outputs        : p_intra_rat_hash_rec - Pointer to the intra RAT hash record
 * Returns        : SON_SUCCESS - If data is populated successfully
 *                  SON_FAILURE - Otherwise
 * Description    : This is the function which populates the neighbor data for 
 *                  the given serving cell so that it can be added in the 
 *                  NRT of all other serving cells.
 *****************************************************************************/
static son_return_et
populate_intra_rat_hash_rec_for_autonomous_add_mod_op
(
 son_intra_rat_global_cell_id_t  *p_srv_cell_id,
 anr_intra_rat_hash_rec_t        *p_intra_rat_hash_rec
 )
{
	anr_cell_context_t *p_cell_ctxt_added_as_nbr = SON_PNULL;

	/* Get the cell context of the serving cell which is to be 
	   added/updated in the NRT of other serving cells */
	p_cell_ctxt_added_as_nbr = 
		anr_get_cell_context_from_global_context(p_srv_cell_id);

	if (SON_PNULL == p_cell_ctxt_added_as_nbr)
	{
		SON_UT_TRACE_EXIT();
		return SON_FAILURE; 
	}

	SON_MEMCPY(&p_intra_rat_hash_rec->neighbor_info.nbr_cgi,
			&p_cell_ctxt_added_as_nbr->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_PCI_PRESENT;
	p_intra_rat_hash_rec->neighbor_info.pci = 
		p_cell_ctxt_added_as_nbr->pci_value;

	p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_TAC_PRESENT;
	SON_MEMCPY(p_intra_rat_hash_rec->neighbor_info.tac, 
			p_cell_ctxt_added_as_nbr->tac,  SON_TAC_OCTET_SIZE);

	p_intra_rat_hash_rec->neighbor_info.bitmask |= 
		SON_PLMN_ID_PRESENT;
	p_intra_rat_hash_rec->neighbor_info.num_plmn_id = 
		p_cell_ctxt_added_as_nbr->num_plmn_id;
	SON_MEMCPY(p_intra_rat_hash_rec->neighbor_info.plmn_id, 
			p_cell_ctxt_added_as_nbr->plmn_id,
			SON_NR_MAX_NUM_PLMN * sizeof(son_cell_plmn_info_t));

	if (SON_EUTRA_MODE_FDD & 
			p_cell_ctxt_added_as_nbr->choice_eutra_mode.bitmask)
	{
		p_intra_rat_hash_rec->neighbor_info.bitmask |= 
			(SON_EARFCN_PRESENT | SON_UL_EARFCN_PRESENT);
		p_intra_rat_hash_rec->neighbor_info.earfcn = 
			p_cell_ctxt_added_as_nbr->choice_eutra_mode.
			fdd_info.dl_earfcn;
		p_intra_rat_hash_rec->neighbor_info.ul_earfcn = 
			p_cell_ctxt_added_as_nbr->choice_eutra_mode.
			fdd_info.ul_earfcn;
	}
	else if (SON_EUTRA_MODE_TDD &
			p_cell_ctxt_added_as_nbr->choice_eutra_mode.bitmask)
	{
		p_intra_rat_hash_rec->neighbor_info.bitmask |= 
			(SON_EARFCN_PRESENT | SON_UL_EARFCN_PRESENT);
		p_intra_rat_hash_rec->neighbor_info.earfcn =
			p_cell_ctxt_added_as_nbr->choice_eutra_mode.
			tdd_info.earfcn;
		p_intra_rat_hash_rec->neighbor_info.ul_earfcn = 
			p_cell_ctxt_added_as_nbr->choice_eutra_mode.
			tdd_info.earfcn;
	}

	if (SON_TRUE == p_cell_ctxt_added_as_nbr->
			is_csg_id_available)
	{
		p_intra_rat_hash_rec->neighbor_info.bitmask |=  
			SON_INTRA_CSG_ID_PRESENT;
		SON_MEMCPY(p_intra_rat_hash_rec->neighbor_info.
				csg_identity,
				p_cell_ctxt_added_as_nbr->csg_identity,
				SON_CSG_ID_OCTET_SIZE);
	}

	if (SON_TRUE == p_cell_ctxt_added_as_nbr->
			is_prach_configuration_available)
	{
		p_intra_rat_hash_rec->neighbor_info.bitmask |= 
			SON_PRACH_CONFIGURATION_PRESENT; 
		SON_MEMCPY(&p_intra_rat_hash_rec->neighbor_info.
				prach_config, 
				&p_cell_ctxt_added_as_nbr->prach_configuration,
				sizeof(son_prach_configuration_t));
	}

	p_intra_rat_hash_rec->neighbor_info.bitmask |= 
		SON_ENABLE_NR_STATUS_PRESENT;
	p_intra_rat_hash_rec->neighbor_info.nr_status = 
		SON_NR_UNLOCKED;

	p_intra_rat_hash_rec->neighbor_info.bitmask |= 
		SON_ENABLE_HO_STATUS_PRESENT;
	p_intra_rat_hash_rec->neighbor_info.ho_status = 
		SON_HO_ALLOWED;

	SON_UT_TRACE_EXIT();
	return SON_SUCCESS; 
}

/*****************************************************************************
 * Function Name  : anr_update_serving_cell_as_nbr_autonomously
 * Inputs         : p_srv_cell_id - Pointer to the serving cell Id which is to
 *                                  be updated in NRT of all other 
 *                                  serving cells 
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the function which updates the given serving cell 
 *                  in the NRT of all other serving cells.
 *****************************************************************************/
son_void_t
anr_update_serving_cell_as_nbr_autonomously
(
 son_intra_rat_global_cell_id_t *p_srv_cell_id
 )
{
	anr_intra_rat_hash_rec_t intra_rat_hash_rec;
	anr_intra_rat_hash_rec_t intra_rat_hash_rec_temp;
	son_anr_updated_nrt_info_t *p_notif_to_oam_modify = SON_PNULL;
	son_anr_rrm_updated_nrt_info_t  *p_notif_to_rrm = SON_PNULL;
	anr_cell_context_node_t *p_cell_ctxt_node = SON_PNULL;
	anr_cell_context_t *p_cell_ctxt = SON_PNULL;
	son_error_et error_code = SON_NO_ERROR;
	son_x2_nrt_update_ind_t nrt_upd_ind;
	son_bool_et is_x2_available = is_x2_connection_available();
	SON_UT_TRACE_ENTER();

	SON_MEMSET(&nrt_upd_ind, 0, sizeof(son_x2_nrt_update_ind_t));

	p_notif_to_rrm = (son_anr_rrm_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));

	if (SON_PNULL == p_notif_to_rrm)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_update_serving_cell_as_nbr_autonomously: "
				"Memory allocation failure for notification to RRM");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_notif_to_oam_modify = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_notif_to_oam_modify)
	{
		if (SON_PNULL != p_notif_to_rrm)
		{
			son_mem_free(p_notif_to_rrm);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_update_serving_cell_as_nbr_autonomously: "
				"Memory allocation failure for update notification to OAM");
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMSET(&intra_rat_hash_rec, 0, 
			sizeof(anr_intra_rat_hash_rec_t));
	if (SON_SUCCESS == populate_intra_rat_hash_rec_for_autonomous_add_mod_op(
				p_srv_cell_id, &intra_rat_hash_rec))
	{
		/* Update the given serving cell in the NRT of all other serving cells
		   for which ANR is enabled. */
		/* Pointer to the first context node */
		p_cell_ctxt_node = anr_get_first_cell_ctxt();
		while (SON_PNULL != p_cell_ctxt_node)
		{
			/* Get the data part of the node */
			p_cell_ctxt = &p_cell_ctxt_node->data;
			if (SON_MEMCMP(&p_cell_ctxt->cell_id, p_srv_cell_id, 
						sizeof(son_intra_rat_global_cell_id_t)) &&
					(ANR_CELL_STATE_ENABLED == 
					 p_cell_ctxt->current_cell_fsm_state))
			{
				SON_MEMSET(p_notif_to_oam_modify, 0, 
						sizeof(son_anr_updated_nrt_info_t));
				SON_MEMSET(p_notif_to_rrm, 0, 
						sizeof(son_anr_rrm_updated_nrt_info_t));

				SON_MEMCPY(&p_notif_to_oam_modify->src_cgi, 
						&p_cell_ctxt->cell_id, 
						sizeof(son_intra_rat_global_cell_id_t));
				SON_MEMCPY(&p_notif_to_rrm->src_cgi, 
						&p_cell_ctxt->cell_id, 
						sizeof(son_intra_rat_global_cell_id_t));
				SON_MEMSET(&intra_rat_hash_rec_temp, 0, 
						sizeof(anr_intra_rat_hash_rec_t));
				SON_MEMCPY(&intra_rat_hash_rec_temp, &intra_rat_hash_rec,
						sizeof(anr_intra_rat_hash_rec_t));

				if (SON_PNULL != anr_intra_rat_modify_nr(p_cell_ctxt,
							&intra_rat_hash_rec_temp,
							SON_FALSE, 
							&error_code,
							SON_PNULL,
							p_notif_to_oam_modify,
							p_notif_to_rrm,
							SON_TRUE, 
							SON_FALSE))
				{
					if (SON_TRUE == is_x2_available)
					{
						/* Since the NRT is updated it should be indicated to 
						   X2 module so as to trigger eNB config update for NRT 
						   updation */
						SON_MEMCPY(
								&nrt_upd_ind.srv_cell_updated_info_list[
								nrt_upd_ind.num_srv_cells_list_size].src_cgi, 
								/*CID 82960 START*/
								&p_cell_ctxt->cell_id, 
								sizeof(son_intra_rat_global_cell_id_t));
						/*CID 82960 START*/
						nrt_upd_ind.srv_cell_updated_info_list[
							nrt_upd_ind.num_srv_cells_list_size].nrt_op = 
							SON_NRT_OP_UPDATE;
						nrt_upd_ind.num_srv_cells_list_size++;
					}

					/* Send SONANR_UPDATED_NRT_INFO to RRM */
					anr_send_updated_ncl_to_rrm(p_notif_to_rrm,
							son_convert_char_cell_id_to_int_cell_id(
								p_cell_ctxt->cell_id.cell_identity));

					/* Send SONANR_UPDATED_NRT_INFO to SONMgmtIfH */
					anr_send_updated_ncl_to_oam(p_notif_to_oam_modify);
				}
			}

			/* Pointer to the next context node */
			p_cell_ctxt_node = anr_get_next_node(p_cell_ctxt_node);
		}
	}

	/*Coverity CID 64968 Fix Start*/
	if ((0 != nrt_upd_ind.num_srv_cells_list_size) && (SON_PNULL != p_cell_ctxt))
		/*Coverity CID 64968 Fix End*/    
	{
		nrt_upd_ind.is_local_update = SON_FALSE;

		/* Send the msg to X2 to indicate updation of NRT */
		anr_populate_x2_nrt_update_ind(&nrt_upd_ind, 
				&p_cell_ctxt->cell_id, SON_NRT_OP_UPDATE);
		son_create_send_buffer((son_u8 *)&nrt_upd_ind, 
				SON_ANR_MODULE_ID, SON_X2_MODULE_ID, 
				SONX2_NRT_UPDATE_IND,
				sizeof(son_x2_nrt_update_ind_t));
	}

	/* Free the memory */
	if (SON_PNULL != p_notif_to_rrm)
	{
		son_mem_free(p_notif_to_rrm);
	}

	if (SON_PNULL != p_notif_to_oam_modify)
	{
		son_mem_free(p_notif_to_oam_modify);
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_add_serving_cell_as_nbr_autonomously
 * Inputs         : p_srv_cell_id - Pointer to the serving cell Id which is to
 *                                  be added in NRT of all other serving cells 
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the function which adds the given serving cell in 
 *                  the NRT of all other serving cells.
 *****************************************************************************/
son_void_t
anr_add_serving_cell_as_nbr_autonomously
(
 son_intra_rat_global_cell_id_t *p_srv_cell_id
 )
{
	anr_intra_rat_hash_rec_t intra_rat_hash_rec;
	anr_intra_rat_hash_rec_t intra_rat_hash_rec_temp;
	son_anr_updated_nrt_info_t *p_notif_to_oam = SON_PNULL;
	son_anr_updated_nrt_info_t *p_notif_to_oam_modify = SON_PNULL;
	son_anr_rrm_updated_nrt_info_t  *p_notif_to_rrm = SON_PNULL;
	anr_cell_context_node_t *p_cell_ctxt_node = SON_PNULL;
	anr_cell_context_t *p_cell_ctxt = SON_PNULL;
	son_error_et error_code = SON_NO_ERROR;
	son_bool_et is_x2_available = is_x2_connection_available();
	son_x2_nrt_update_ind_t nrt_upd_ind;
    /* SPR 18930  FIX START */
    /* Code Removed */
    /* SPR 18930  FIX END */
	SON_UT_TRACE_ENTER();

	SON_MEMSET(&nrt_upd_ind, 0, sizeof(son_x2_nrt_update_ind_t));

	p_notif_to_rrm = (son_anr_rrm_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));

	if (SON_PNULL == p_notif_to_rrm)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_add_serving_cell_as_nbr_autonomously: "
				"Memory allocation failure for notification to RRM");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_notif_to_oam = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_notif_to_oam)
	{
		if (SON_PNULL != p_notif_to_rrm)
		{
			son_mem_free(p_notif_to_rrm);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_add_serving_cell_as_nbr_autonomously: "
				"Memory allocation failure for add notification to OAM");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_notif_to_oam_modify = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_notif_to_oam_modify)
	{
		if (SON_PNULL != p_notif_to_rrm)
		{
			son_mem_free(p_notif_to_rrm);
		}

		if (SON_PNULL != p_notif_to_oam)
		{
			son_mem_free(p_notif_to_oam);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_add_serving_cell_as_nbr_autonomously: "
				"Memory allocation failure for update notification to OAM");
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMSET(&intra_rat_hash_rec, 0, 
			sizeof(anr_intra_rat_hash_rec_t));
	if (SON_SUCCESS == populate_intra_rat_hash_rec_for_autonomous_add_mod_op(
				p_srv_cell_id, &intra_rat_hash_rec))
	{
		/* Add the given serving cell in the NRT of all other serving cells
		   for which ANR is enabled. */

		/* Pointer to the first context node */
		p_cell_ctxt_node = anr_get_first_cell_ctxt();
		while (SON_PNULL != p_cell_ctxt_node)
		{
			/*Get the data part of the node*/
			p_cell_ctxt = &p_cell_ctxt_node->data;
			if (SON_MEMCMP(&p_cell_ctxt->cell_id, p_srv_cell_id, 
						sizeof(son_intra_rat_global_cell_id_t)) &&
					(ANR_CELL_STATE_ENABLED == 
					 p_cell_ctxt->current_cell_fsm_state))
			{
				SON_MEMSET(p_notif_to_oam, 0, 
						sizeof(son_anr_updated_nrt_info_t));
				SON_MEMSET(p_notif_to_oam_modify, 0, 
						sizeof(son_anr_updated_nrt_info_t));
				SON_MEMSET(p_notif_to_rrm, 0, 
						sizeof(son_anr_rrm_updated_nrt_info_t));

				SON_MEMCPY(&p_notif_to_oam->src_cgi, 
						&p_cell_ctxt->cell_id, 
						sizeof(son_intra_rat_global_cell_id_t));
				SON_MEMCPY(&p_notif_to_oam_modify->src_cgi, 
						&p_cell_ctxt->cell_id, 
						sizeof(son_intra_rat_global_cell_id_t));
				SON_MEMCPY(&p_notif_to_rrm->src_cgi, 
						&p_cell_ctxt->cell_id, 
						sizeof(son_intra_rat_global_cell_id_t));

				SON_MEMSET(&intra_rat_hash_rec_temp, 0, 
						sizeof(anr_intra_rat_hash_rec_t));
				SON_MEMCPY(&intra_rat_hash_rec_temp, &intra_rat_hash_rec,
						sizeof(anr_intra_rat_hash_rec_t));

                /* SPR 18930  FIX START */
                /* Code Removed */
                /* SPR 18930  FIX END */

					SON_MEMSET(p_notif_to_oam_modify, 0, 
							sizeof(son_anr_updated_nrt_info_t));
					SON_MEMSET(p_notif_to_rrm, 0, 
							sizeof(son_anr_rrm_updated_nrt_info_t));

					SON_MEMCPY(&p_notif_to_oam_modify->src_cgi, 
							&p_cell_ctxt->cell_id, 
							sizeof(son_intra_rat_global_cell_id_t));
					SON_MEMCPY(&p_notif_to_rrm->src_cgi, 
							&p_cell_ctxt->cell_id, 
							sizeof(son_intra_rat_global_cell_id_t));

					if (SON_PNULL != anr_intra_rat_modify_nr(p_cell_ctxt,
								&intra_rat_hash_rec,
								SON_FALSE, 
								&error_code,
								SON_PNULL,
								p_notif_to_oam_modify,
								p_notif_to_rrm,
								SON_TRUE, 
								SON_FALSE))
					{
						if (SON_TRUE == is_x2_available)
						{
							/* Since the NRT is updated it should be indicated 
							   to X2 module so as to trigger eNB config 
							   update for NRT updation */
							SON_MEMCPY(
									&nrt_upd_ind.srv_cell_updated_info_list[
									nrt_upd_ind.num_srv_cells_list_size].
									src_cgi, 
									/* Coverity ID 82973 FIX START */
									&p_cell_ctxt->cell_id,
									/* Coverity ID 82973 FIX END */
									sizeof(son_intra_rat_global_cell_id_t));
							nrt_upd_ind.srv_cell_updated_info_list[
								nrt_upd_ind.num_srv_cells_list_size].nrt_op = 
								SON_NRT_OP_UPDATE;
							nrt_upd_ind.num_srv_cells_list_size++;
						}

						/* Send SONANR_UPDATED_NRT_INFO to RRM */
						anr_send_updated_ncl_to_rrm(p_notif_to_rrm,
								son_convert_char_cell_id_to_int_cell_id(
									p_cell_ctxt->cell_id.cell_identity));

						/* Send SONANR_UPDATED_NRT_INFO to SONMgmtIfH */
						anr_send_updated_ncl_to_oam(p_notif_to_oam_modify);
					}
                    /* SPR 18930  FIX START */
                    else if (SON_ERR_ANR_NR_NOT_FOUND == error_code) 
                    {
                       
                        p_notif_to_oam->nrt_op = SON_NRT_OP_ADD;
                        intra_rat_hash_rec.neighbor_info.bitmask |=
                                                    SON_ENABLE_X2_STATUS_PRESENT; 
                        intra_rat_hash_rec.neighbor_info.x2_status = 
                                                     SON_LOCALLY_CONNECTED_CELL;

                        intra_rat_hash_rec.neighbor_info.bitmask |=
                                                    SON_ENABLE_HO_STATUS_PRESENT; 
                        intra_rat_hash_rec.neighbor_info.ho_status = 
                                                     SON_HO_ALLOWED; 

                        anr_construct_oam_updated_ncl(&p_notif_to_oam->nr_list,
                                &intra_rat_hash_rec.neighbor_info, 
                                SON_FALSE);
                        
                        if (0 != p_notif_to_oam->nr_list.
                                intra_rat_nr_list_size)
                        {
                            SON_MEMCPY(&p_notif_to_oam->src_cgi, 
                                    &p_cell_ctxt->cell_id, 
                                    sizeof(son_intra_rat_global_cell_id_t));
                            anr_send_updated_ncl_to_oam(p_notif_to_oam);
                        }
				}
                    /* SPR 18930  FIX  END */
			}
            else
            {
                
            }
			/* Pointer to the next context node */
			p_cell_ctxt_node = anr_get_next_node(p_cell_ctxt_node);
		}
	}
	/*Coverity CID 64967 Fix Start*/
	if ((0 != nrt_upd_ind.num_srv_cells_list_size) && (SON_PNULL != p_cell_ctxt))
		/*Coverity CID 64967 Fix End*/    
	{
		nrt_upd_ind.is_local_update = SON_FALSE;

		/* Send the msg to X2 to indicate updation of NRT */
		anr_populate_x2_nrt_update_ind(&nrt_upd_ind, 
				&p_cell_ctxt->cell_id, SON_NRT_OP_UPDATE);
		son_create_send_buffer((son_u8 *)&nrt_upd_ind, 
				SON_ANR_MODULE_ID, SON_X2_MODULE_ID, 
				SONX2_NRT_UPDATE_IND,
				sizeof(son_x2_nrt_update_ind_t));
	}
	/* Free the memory */
	if (SON_PNULL != p_notif_to_oam)
	{
		son_mem_free(p_notif_to_oam);
	}

	if (SON_PNULL != p_notif_to_rrm)
	{
		son_mem_free(p_notif_to_rrm);
	}

	if (SON_PNULL != p_notif_to_oam_modify)
	{
		son_mem_free(p_notif_to_oam_modify);
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_delete_serving_cell_as_nbr_autonomously
 * Inputs         : p_srv_cell_id - Pointer to the serving cell Id which is 
 *                                  to be deleted from NRT of all other 
 *                                  serving cells 
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the function which finds the given serving cell in 
 *                  the NRT of all other serving cells and deletes it.
 *****************************************************************************/
son_void_t
anr_delete_serving_cell_as_nbr_autonomously
(
 son_intra_rat_global_cell_id_t *p_srv_cell_id
 )
{
	anr_intra_rat_hash_rec_t intra_rat_hash_rec;
	son_anr_updated_nrt_info_t *p_notif_to_oam = SON_PNULL;
	son_anr_rrm_updated_nrt_info_t  *p_notif_to_rrm = SON_PNULL;
	anr_cell_context_node_t *p_cell_ctxt_node = SON_PNULL;
	anr_cell_context_t *p_cell_ctxt = SON_PNULL;
	son_error_et error_code = SON_NO_ERROR;
	son_return_et ret = SON_SUCCESS;
	son_x2_nrt_update_ind_t nrt_upd_ind;
	son_bool_et is_x2_available = is_x2_connection_available();
	SON_UT_TRACE_ENTER();

	SON_MEMSET(&nrt_upd_ind, 0, sizeof(son_x2_nrt_update_ind_t));

	p_notif_to_rrm = (son_anr_rrm_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));

	if (SON_PNULL == p_notif_to_rrm)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_delete_serving_cell_as_nbr_autonomously: "
				"Memory allocation failure for notification to RRM");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_notif_to_oam = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_notif_to_oam)
	{
		if (SON_PNULL != p_notif_to_rrm)
		{
			son_mem_free(p_notif_to_rrm);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_delete_serving_cell_as_nbr_autonomously: "
				"Memory allocation failure for notification to OAM");
		SON_UT_TRACE_EXIT();
		return;
	}

	/* Delete the given serving cell in the NRT of all other serving cells
	   for which ANR is enabled. */

	/*Pointer to the first context node*/
	p_cell_ctxt_node = anr_get_first_cell_ctxt();

	while (SON_PNULL != p_cell_ctxt_node)
	{
		/*Get the data part of the node*/
		p_cell_ctxt = &p_cell_ctxt_node->data;

		if (SON_MEMCMP(&p_cell_ctxt->cell_id, p_srv_cell_id, 
					sizeof(son_intra_rat_global_cell_id_t)))
		{
			SON_MEMSET(&intra_rat_hash_rec, 0, 
					sizeof(anr_intra_rat_hash_rec_t));
			SON_MEMSET(p_notif_to_rrm, 0, 
					sizeof(son_anr_rrm_updated_nrt_info_t));
			SON_MEMSET(p_notif_to_oam, 0,
					sizeof(son_anr_updated_nrt_info_t));

			SON_MEMCPY(&p_notif_to_rrm->src_cgi,
					&p_cell_ctxt->cell_id,
					sizeof(son_intra_rat_global_cell_id_t));
			SON_MEMCPY(&p_notif_to_oam->src_cgi,
					&p_cell_ctxt->cell_id,
					sizeof(son_intra_rat_global_cell_id_t));

			ret = anr_intra_rat_del_nr(p_cell_ctxt,
					p_srv_cell_id,
					SON_TRUE,
					&error_code,
					&intra_rat_hash_rec,
					p_notif_to_oam,
					p_notif_to_rrm,
					SON_TRUE);

			if (SON_FAILURE == ret)
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_WARNING,
						"anr_delete_serving_cell_as_nbr_autonomously: "
						"Seving cell [Cell Id: 0x%x] switched OFF due to "
						"energy saving is not deleted from NRT of other "
						"seving cell [Cell Id: 0x%x].",
						son_convert_char_cell_id_to_int_cell_id(
							p_srv_cell_id->cell_identity),
						son_convert_char_cell_id_to_int_cell_id(
							p_cell_ctxt->cell_id.cell_identity));
			}
			else 
			{
				if (SON_TRUE == is_x2_available)
				{
					/* Since the NRT is updated it should be indicated 
					   to X2 module so as to trigger eNB config 
					   update for NRT updation */
					/* SPR-11877 Fix Starts */
					anr_populate_x2_nrt_update_ind(&nrt_upd_ind, 
							&p_cell_ctxt->cell_id, SON_NRT_OP_UPDATE);
					/* SPR-11877 Fix Ends */
				}

				/* Send SONANR_UPDATED_NRT_INFO to RRM */
				anr_send_updated_ncl_to_rrm(p_notif_to_rrm,
						son_convert_char_cell_id_to_int_cell_id(
							p_cell_ctxt->cell_id.cell_identity));
				/* Send SONANR_UPDATED_NRT_INFO to SONMgmtIfH */
				anr_send_updated_ncl_to_oam(p_notif_to_oam);

				/* Send updated NRT info to MLB for neighbor deletion */
				if (MLB_CELL_STATE_ENABLED == mlb_get_cell_mode(
							&p_cell_ctxt->cell_id))
				{
					anr_send_updated_ncl_to_mlb(p_notif_to_oam);
				}

				anr_print_cell_context(p_cell_ctxt);
			}
		}
		/*Pointer to the next context node*/
		p_cell_ctxt_node = anr_get_next_node(p_cell_ctxt_node);
	}

	if (0 != nrt_upd_ind.num_srv_cells_list_size)
	{
		nrt_upd_ind.is_local_update = SON_FALSE;

		/* Send the msg to X2 to indicate updation of NRT */
		/* SPR-11877 Fix Starts */
		/* Lines deleted */
		/* SPR-11877 Fix Ends */
		son_create_send_buffer((son_u8 *)&nrt_upd_ind, 
				SON_ANR_MODULE_ID, SON_X2_MODULE_ID, 
				SONX2_NRT_UPDATE_IND,
				sizeof(son_x2_nrt_update_ind_t));
	}

	/* Free the memory */
	if (SON_PNULL != p_notif_to_oam)
	{
		son_mem_free(p_notif_to_oam);
	}

	if (SON_PNULL != p_notif_to_rrm)
	{
		son_mem_free(p_notif_to_rrm);
	}

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_add_other_serving_cells_as_nbr_autonomously
 * Inputs         : p_srv_cell_ctxt - Pointer to the serving cell context 
 *                                    whose NRT is to be updated.
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the function which finds all the serving cells
 *                  which are configured at ANR and them them as the neighbor
 *                  in the NRT of given serving cell (The serving cell itself
 *                  is not added in its own NRT). 
 *****************************************************************************/
son_void_t
anr_add_other_serving_cells_as_nbr_autonomously
(
 anr_cell_context_t *p_srv_cell_ctxt
 )
{
	anr_cell_context_node_t *p_cell_ctxt_node = SON_PNULL;
	anr_cell_context_t *p_cell_ctxt = SON_PNULL;
	anr_intra_rat_hash_rec_t intra_rat_hash_rec;
	son_anr_updated_nrt_info_t *p_notif_to_oam = SON_PNULL;
	son_anr_updated_nrt_info_t *p_notif_to_oam_modify = SON_PNULL;
	son_anr_rrm_updated_nrt_info_t  *p_notif_to_rrm = SON_PNULL;
    /* SPR 18930  FIX START */
    /* Code Removed */
    /* SPR 18930  FIX END */
	SON_UT_TRACE_ENTER();

	p_notif_to_rrm = (son_anr_rrm_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));

	if (SON_PNULL == p_notif_to_rrm)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_add_other_serving_cells_as_nbr_autonomously: "
				"Memory allocation failure for notification to RRM");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_notif_to_oam = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_notif_to_oam)
	{
		if (SON_PNULL != p_notif_to_rrm)
		{
			son_mem_free(p_notif_to_rrm);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_add_other_serving_cells_as_nbr_autonomously: "
				"Memory allocation failure for add notification to OAM");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_notif_to_oam_modify = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_notif_to_oam_modify)
	{
		if (SON_PNULL != p_notif_to_rrm)
		{
			son_mem_free(p_notif_to_rrm);
		}

		if (SON_PNULL != p_notif_to_oam)
		{
			son_mem_free(p_notif_to_oam);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_add_other_serving_cells_as_nbr_autonomously: "
				"Memory allocation failure for update notification to OAM");
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMSET(p_notif_to_oam, 0, 
			sizeof(son_anr_updated_nrt_info_t));
	SON_MEMSET(p_notif_to_oam_modify, 0, 
			sizeof(son_anr_updated_nrt_info_t));
	SON_MEMSET(p_notif_to_rrm, 0, 
			sizeof(son_anr_rrm_updated_nrt_info_t));

	SON_MEMCPY(&p_notif_to_oam->src_cgi,
			&p_srv_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
	SON_MEMCPY(&p_notif_to_oam_modify->src_cgi, 
			&p_srv_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
	SON_MEMCPY(&p_notif_to_rrm->src_cgi, 
			&p_srv_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	/* Pointer to the first context node */
	p_cell_ctxt_node = anr_get_first_cell_ctxt();

	while (SON_PNULL != p_cell_ctxt_node)
	{
		/*Get the data part of the node*/
		p_cell_ctxt = &p_cell_ctxt_node->data;

		/* Add this serving cell in the NRT of given serving cell if the two 
		   cells are different */
		if (SON_MEMCMP(&p_cell_ctxt->cell_id, &p_srv_cell_ctxt->cell_id, 
					sizeof(son_intra_rat_global_cell_id_t)))
		{
			SON_MEMSET(&intra_rat_hash_rec, 0, 
					sizeof(anr_intra_rat_hash_rec_t));
			if (SON_SUCCESS == 
					populate_intra_rat_hash_rec_for_autonomous_add_mod_op(
						&p_cell_ctxt->cell_id, &intra_rat_hash_rec))
			{
                /* SPR 18930  FIX START */
                /* Code Removed */
                /* SPR 18930  FIX END */
                p_notif_to_oam->nrt_op = SON_NRT_OP_ADD;
                intra_rat_hash_rec.neighbor_info.bitmask |=
                    SON_ENABLE_X2_STATUS_PRESENT; 
                intra_rat_hash_rec.neighbor_info.x2_status = 
                    SON_LOCALLY_CONNECTED_CELL;

                intra_rat_hash_rec.neighbor_info.bitmask |=
                    SON_ENABLE_HO_STATUS_PRESENT; 
                intra_rat_hash_rec.neighbor_info.ho_status = 
                    SON_HO_ALLOWED; 

                anr_construct_oam_updated_ncl(&p_notif_to_oam->nr_list,
                        &intra_rat_hash_rec.neighbor_info, 
						SON_FALSE);


			}
		}
		/* Pointer to the next context node */
		p_cell_ctxt_node = anr_get_next_node(p_cell_ctxt_node);
	}

	/* Send SONANR_UPDATED_NRT_INFO to RRM */
    /* SPR 18930  FIX START */
    /* Code Removed */
    /* SPR 18930  FIX END */

	/* Send SONANR_UPDATED_NRT_INFO to SONMgmtIfH for NR add */
	anr_send_updated_ncl_to_oam(p_notif_to_oam);                

	if (0 != p_notif_to_oam_modify->nr_list.
			intra_rat_nr_list_size)
	{
		/* Send SONANR_UPDATED_NRT_INFO to SONMgmtIfH for 
		   NR modify */
		SON_MEMCPY(&p_notif_to_oam_modify->src_cgi, 
				&p_cell_ctxt->cell_id, 
				sizeof(son_intra_rat_global_cell_id_t));
		anr_send_updated_ncl_to_oam(p_notif_to_oam_modify);
	}

	/* Free the memory */
	if (SON_PNULL != p_notif_to_oam)
	{
		son_mem_free(p_notif_to_oam);
	}

	if (SON_PNULL != p_notif_to_rrm)
	{
		son_mem_free(p_notif_to_rrm);
	}

	if (SON_PNULL != p_notif_to_oam_modify)
	{
		son_mem_free(p_notif_to_oam_modify);
	}

	SON_UT_TRACE_EXIT();
}

/* Rel 3.0 Multi Sector code changes Stop */
/* Spr 17753 Changes Start */
/*****************************************************************************
 * Function Name  : anr_is_serving_conflicts_with_inserted_neighbour 
 * Inputs         : p_cell_ctxt        - Pointer to cell context
 *                  p_hash_rec         - Pointer nr hash records
 *                  p_msg              - Pointer to conflict ind
 * Outputs        : None
 * Returns        : son_bool_et SON_TRUE/SON_FALSE 
 * Description    : This function find the conflict from inserted neighbour 
 *                  and serving cell 
 ****************************************************************************/
son_bool_et
anr_is_serving_conflicts_with_inserted_neighbour
(
 anr_cell_context_t           *p_cell_ctxt,
 anr_intra_rat_hash_rec_t   *p_hash_rec,
 son_anr_pci_conflict_confusion_ind_t *p_msg
 )
{
    son_bool_et retVal = SON_FALSE;
    son_u16     count  = 0;

    SON_UT_TRACE_ENTER();

    if((p_hash_rec->neighbor_info.earfcn == anr_get_earfcn(p_cell_ctxt)) &&
            (p_hash_rec->neighbor_info.pci == p_cell_ctxt->pci_value))
    {
        p_msg->cause = SON_PCI_CONFLICT;
        p_msg->pci = p_cell_ctxt->pci_value;
        p_msg->cgi_1.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
        p_msg->cgi_2.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
                 
        SON_MEMCPY(&p_msg->cgi_1.intra_rat_global_cell_id,
                &p_cell_ctxt->cell_id,
                sizeof(son_intra_rat_global_cell_id_t));

        SON_MEMCPY(&p_msg->cgi_2.intra_rat_global_cell_id,
                &p_hash_rec->neighbor_info.nbr_cgi,
                sizeof(son_intra_rat_global_cell_id_t));

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_WARNING,
                "anr_is_serving_conflicts_with_inserted_neighbour:"
                "Neighbor Cell id [Cell id: 0x%x] and Serving cell id [Cell id:0x%x] are in conflict"
                "with each other for PCI[%d] and EARFCN [%d]",
                son_convert_char_cell_id_to_int_cell_id(p_hash_rec->neighbor_info.nbr_cgi.cell_identity),
                son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                p_cell_ctxt->pci_value, anr_get_earfcn(p_cell_ctxt));

        retVal = SON_TRUE;

    }
    else if(p_cell_ctxt->is_conflict_with_second_degree_enable)
    {
        /* Search Second degree neighbor for conflict if present */
        count = list_count(&p_hash_rec->anr_nr_nbr_list);

        if(count)
        {
            SON_LIST_NODE           *p_node             = SON_PNULL;
            anr_nr_nbr_list_node_t *p_list_node = SON_PNULL;

            p_node = get_first_list_node(&p_hash_rec->anr_nr_nbr_list);

            while (SON_PNULL != p_node)
            {    
                p_list_node =  YMEMBEROF(anr_nr_nbr_list_node_t,
                        nr_nbr_anchor,
                        p_node);

                if((p_list_node->nr_nbr_data.earfcn == anr_get_earfcn(p_cell_ctxt)) &&
                        (p_list_node->nr_nbr_data.pci    == p_cell_ctxt->pci_value) &&
                        (0 != SON_MEMCMP(&p_list_node->nr_nbr_data.nr_nbr_cgi,&p_cell_ctxt->cell_id,
                                         sizeof(son_intra_rat_global_cell_id_t))))
                {
                    retVal = SON_TRUE;
                    break;
                }
                p_node = get_next_list_node(p_node);
            }  
            if(retVal == SON_TRUE)
            {
                p_msg->cause = SON_PCI_CONFLICT;
                p_msg->pci = p_cell_ctxt->pci_value;
                p_msg->cgi_1.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
                p_msg->cgi_2.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;

                SON_MEMCPY(&p_msg->cgi_1.intra_rat_global_cell_id,
                        &p_cell_ctxt->cell_id,
                        sizeof(son_intra_rat_global_cell_id_t));

                SON_MEMCPY(&p_msg->cgi_2.intra_rat_global_cell_id,
                        &p_list_node->nr_nbr_data.nr_nbr_cgi,
                        sizeof(son_intra_rat_global_cell_id_t));

                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_DETAILED,
                        "anr_is_serving_conflicts_with_inserted_neighbour:"
                        " Second Degree Neighbor Cell id [Cell id: 0x%x] of inserted neighbor Cell id [Cell id: 0x%x]"
                        " and Serving cell id [Cell id:0x%x] are in conflict"
                        " with each other for PCI[%d] and EARFCN [%d]",
                        son_convert_char_cell_id_to_int_cell_id(p_list_node->nr_nbr_data.nr_nbr_cgi.cell_identity),
                        son_convert_char_cell_id_to_int_cell_id(p_hash_rec->neighbor_info.nbr_cgi.cell_identity),
                        son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                        p_cell_ctxt->pci_value, anr_get_earfcn(p_cell_ctxt));


            }
        }
    }
    if(!retVal)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
                "anr_is_serving_conflicts_with_inserted_neighbour:"
                " No Conflct Found for Serving Cell [cellid : 0x%x]",
                son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
    } 

    SON_UT_TRACE_EXIT();
    return retVal;
} /* anr_is_serving_conflicts_with_inserted_neighbour */

/*****************************************************************************
 * Function Name  : anr_clean_second_degree_conflict_data 
 * Inputs         : p_cell_ctxt        - Pointer to cell context
 * Outputs        : None
 * Returns        : son_void_t NONE 
 * Description    : This function cleans the conflit data from cell context 
 ****************************************************************************/
son_void_t
anr_clean_second_degree_conflict_data(anr_cell_context_t * p_cell_ctxt)
{
    anr_second_degree_nr_list_node_t *p_list_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    /* Stop NR add Timer if running */   
    if(p_cell_ctxt->son_nr_add_timer_id)
    {
        son_stop_timer(p_cell_ctxt->son_nr_add_timer_id);
        p_cell_ctxt->son_nr_add_timer_id = SON_PNULL;
    }

    /* Stop backoff Timer if running */   
    if(p_cell_ctxt->son_backoff_timer_id)
    {
        son_stop_timer(p_cell_ctxt->son_backoff_timer_id);
        p_cell_ctxt->son_backoff_timer_id = SON_PNULL;
    }

    /* Delele the second degree list node */
    while (SON_PNULL != (p_list_node = (anr_second_degree_nr_list_node_t *)list_pop_node
                (&(p_cell_ctxt->second_degree_nbr_list))))
    {
        anr_clear_nr_nbrs_list(&p_list_node->nr_nbr_data.anr_nr_nbr_list);
        son_mem_free(p_list_node);
        p_list_node = SON_PNULL;
    }

    SON_UT_TRACE_EXIT();
}/* anr_clean_second_degree_conflict_data */
/* Spr 17753 Changes End */
