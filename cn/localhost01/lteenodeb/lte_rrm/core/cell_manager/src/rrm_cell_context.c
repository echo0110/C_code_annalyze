
/******************************************************************************
  *
  * ARICENT -
  *
  * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
  *
  ******************************************************************************
  *
  * $$Id: rrm_cell_context.c $
  *
  *******************************************************************************
  *
  * File Description: This file contains the local function for CELL RRM  
  *      
  *
  *
  *******************************************************************************
  *Revision Details
  *----------------------
  *   DATE            AUTHOR      REFERENCE       REASON
  *   Feb,2012                    Initial
  *   16 May,2014     gur13083    SPR 10869       Modified ECN function to
  *                                               calculate UL/DL status separately
  *
  *   28 May,2014     gur29831    SPR 11368       Added fix for SPR 11368
  ******************************************************************************/
/****************************************************************************
 * Includes
 ****************************************************************************/
#include "rrm_cell_context.h"
#include "rrm_utils.h"
#include "rrm_cell_mgr.h"
#include "rrm_mem_utils.h"
/*** Function Pointer Variable Definition for Cell Load Action */
rrm_cell_load_actn_func_ptr rrm_cell_load_actn[RRM_ACTN_MAX] = 
{
    /* MEDIUM */
    /* RRM_ACTN_NONE */
    rrm_cell_load_action_none_hndl,
    /* RRM_ACTN_HANDOVER */
    rrm_cell_load_action_ho_hndl,
    /* RRM_ACTN_SSAC_BARRING */
    rrm_cell_load_action_ssac_bar_hndl,
    /* RRM_ACTN_STOP_ADM */
    rrm_cell_load_action_stop_adm_hndl,
    /* RRMCM_ACTN_REL_UE_CARR_REDIRECT */
    rrm_cell_load_action_rel_ue_carr_redir_hndl,
    /* RRM_ACTN_REL_UE */
    rrm_cell_load_action_rel_ue_hndl,
    /* RRM_ACTN_AQM */
    rrm_cell_load_action_aqm_hndl,
    /* RRM_ACTN_RRM_ECN_UL_DIR */
    rrm_cell_load_action_ecn_ul_dir_hndl,
    /* RRM_ACTN_RRM_ECN_DL_DIR */
    rrm_cell_load_action_ecn_dl_dir_hndl,
    /* RRM_ACTN_RRM_ECN_BI_DIR */
    rrm_cell_load_action_ecn_bi_dir_hndl,
    /* Carrier_Aggregation_Start */
    /* RRM_ACTN_RRM_SCELL_DEACTIVATE */
    rrm_cell_load_action_scell_deact_hndl,    
    /* Carrier_Aggregation_End */
    /* eICIC_PHASE_1_2_CHANGES_START */
    /* RRM_ACTN_RRM_SEND_ABS_TO_L2 */
    /*SPR 14182 start*/
    //code_delete
    /*SPR 14182 start*/
    /* eICIC_PHASE_1_2_CHANGES_END */
};

/* SPR 11681 Fix Start */
/* Code Deleted */
/* SPR 11681 Fix End */

/****************************************************************************
 * Function Name  : de_allocate_tcb_from_cell_ctxt
 * Inputs         :
 *                  -  p_cell_context
 *                  -  trans  Id
 * Outputs        :
 *
 * Returns        :  SUCCESS/FAILURE
 *
 * Variables      :
 * Description    : This function de allocated the TCB
 ****************************************************************************/
rrm_return_et de_allocate_tcb_from_cell_ctxt(
        rrm_cell_context_t *p_cell_context,
        U16                 rcvd_trans_id)
{
    cellm_tcb  *p_temp = RRM_PNULL;
    cellm_tcb  *p_current = RRM_PNULL;
    rrm_return_et      ret_val = RRM_FAILURE;

    RRM_UT_TRACE_ENTER();

    if ((RRM_PNULL == p_cell_context) || (RRM_PNULL == p_cell_context->p_tcb))
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR, "Cell context NULL or Incorrect Data TransId(%d) Rcvd", rcvd_trans_id);
        ret_val = RRM_FAILURE;
    }
    else
    {
        p_current = p_temp = p_cell_context->p_tcb;
        if (p_current->trans_id == rcvd_trans_id)
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED, "Matched Data With Rcvd TransId(%d) "
                    "& Stored Trans Id(%d)",
                    rcvd_trans_id, p_current->trans_id);
            p_cell_context->p_tcb = p_current->p_next;
            RRM_MEM_FREE(p_current);
            RRM_UT_TRACE_EXIT();
            return RRM_SUCCESS;
        }
        else
        {
            while(RRM_PNULL != p_temp)
            {
                if (p_temp->trans_id == rcvd_trans_id)
                {
                    ret_val = RRM_SUCCESS;
                    break;
                }
                p_current = p_temp;
                p_temp = p_temp->p_next;
            }
        }
        if (RRM_SUCCESS == ret_val)
        {
            p_current->p_next = p_temp->p_next;
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED, "Matched Data With Rcvd TransId(%d) "
                    "& Stored Trans Id(%d)",
                    rcvd_trans_id, p_temp->trans_id);
            RRM_MEM_FREE(p_temp);
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}


/* Code for MLB Start */
/****************************************************************************
* Function Name  : rrm_populate_nbr_rsu_cell 
* Inputs         :
*                  -  x2ap_rrm_rsu_info_t *
*                  -  
* Outputs        : rrm_nbr_rsu_info_t *
*
* Returns        :  None
*
* Variables      :
* Description    : This function populate neighbor load in NCL for intra/inter cell 
****************************************************************************/
rrm_void_t rrm_populate_nbr_rsu_cell
(
    x2ap_rrm_rsu_info_t *p_rsu_info,
    /* eICIC_PHASE_1_2_CHANGES_START */
    rrm_cell_context_t *p_cell_ctx ,
    rrm_eutran_cell_types_et cell_type,
    U16 nbr_cell_idx
)
{
    U8 bitmask = 0;
    rrm_rs_info_t *p_intra_inter_rsu_info = RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    RRM_ASSERT(RRM_PNULL != p_rsu_info);
    RRM_ASSERT(RRM_PNULL != p_cell_ctx);

    if (RRM_EUTRAN_INTRA_FREQ_CELL == cell_type) /* EUTRAN Intra cell neighbor */
    {
        p_intra_inter_rsu_info = &(p_cell_ctx->ran_info.ncl_params.lte_ncl.intra_freq_cells[nbr_cell_idx].resrc_report); 
    }
    else /* EUTRAN Inter Cell neighbor */
    {
        p_intra_inter_rsu_info = &(p_cell_ctx->ran_info.ncl_params.lte_ncl.inter_freq_cells[nbr_cell_idx].resrc_report); 
    }
    /* eICIC_PHASE_1_2_CHANGES_END */
    bitmask =  p_rsu_info->bitmask;
    if (bitmask & RRM_RR_LOAD_PERIODIC_REPORT_PRESENT)
    {
        p_intra_inter_rsu_info->bitmask |= RRM_RR_LOAD_PERIODIC_REPORT_PRESENT; 
        p_intra_inter_rsu_info->rr_load.dl_gbr_prb_usage = \
                p_rsu_info->radio_resource_status.dl_gbr_prb_usage;
        p_intra_inter_rsu_info->rr_load.ul_gbr_prb_usage = \
                 p_rsu_info->radio_resource_status.ul_gbr_prb_usage;
        p_intra_inter_rsu_info->rr_load.dl_non_gbr_prb_usage = \
                 p_rsu_info->radio_resource_status.dl_non_gbr_prb_usage;
        p_intra_inter_rsu_info->rr_load.ul_non_gbr_prb_usage = \
                 p_rsu_info->radio_resource_status.ul_non_gbr_prb_usage;
        p_intra_inter_rsu_info->rr_load.dl_total_prb_usage = \
                 p_rsu_info->radio_resource_status.dl_total_gbr_prb_usage;
        p_intra_inter_rsu_info->rr_load.ul_total_prb_usage = \
                 p_rsu_info->radio_resource_status.ul_total_gbr_prb_usage;
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_DETAILED, \
                 "PRB Load not present");
    }
    if (bitmask & RRM_TNL_LOAD_PERIODIC_REPORT_PRESENT)
    {
        p_intra_inter_rsu_info->bitmask |= RRM_TNL_LOAD_PERIODIC_REPORT_PRESENT; 
        p_intra_inter_rsu_info->s1_tnl_load.dl = \
                (rrm_x2ap_rsu_load_ind_et)p_rsu_info->s1_tnl_load_indicator.dl_s1_tnl_load_indicator;
        p_intra_inter_rsu_info->s1_tnl_load.ul = \
                (rrm_x2ap_rsu_load_ind_et)p_rsu_info->s1_tnl_load_indicator.ul_s1_tnl_load_indicator;
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_DETAILED, \
                         "S1 TNL Load not present");
    }
    if (bitmask & RRM_HW_LOAD_PERIODIC_REPORT_PRESENT)
    {
        p_intra_inter_rsu_info->bitmask |= RRM_HW_LOAD_PERIODIC_REPORT_PRESENT; 
        p_intra_inter_rsu_info->hw_load.dl = \
                (rrm_x2ap_rsu_load_ind_et)p_rsu_info->hardware_load_indicator.dl_hardware_load_indicator;
        p_intra_inter_rsu_info->hw_load.ul = \
                (rrm_x2ap_rsu_load_ind_et)p_rsu_info->hardware_load_indicator.ul_hardware_load_indicator;
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_DETAILED, \
                         "HW Load not present");
    }
    if (bitmask & RRM_COMP_AVL_CAP_PERIODIC_REPORT_PRESENT)
    {
        p_intra_inter_rsu_info->bitmask |= RRM_COMP_AVL_CAP_PERIODIC_REPORT_PRESENT; 
        if (p_rsu_info->composite_available_capacity_group.dl_composite_available_capacity.\
            bitmask & RRM_CELL_CAPACITY_CLASS_VALUE_PRESENT)
        {
            p_intra_inter_rsu_info->comp_avl_cap_grp.dl.bitmask |= RRM_CELL_CAPACITY_CLASS_VALUE_PRESENT;
            p_intra_inter_rsu_info->comp_avl_cap_grp.dl.cell_cap_class_val =
                p_rsu_info->composite_available_capacity_group.dl_composite_available_capacity.cell_capacity_class_value;
        }
        if (p_rsu_info->composite_available_capacity_group.ul_composite_available_capacity.\
            bitmask & RRM_CELL_CAPACITY_CLASS_VALUE_PRESENT)
        {
            p_intra_inter_rsu_info->comp_avl_cap_grp.ul.\
                        bitmask |= RRM_CELL_CAPACITY_CLASS_VALUE_PRESENT;
            p_intra_inter_rsu_info->comp_avl_cap_grp.ul.\
                        cell_cap_class_val = \
                        p_rsu_info->composite_available_capacity_group.ul_composite_available_capacity.\
                        cell_capacity_class_value;
        }
        p_intra_inter_rsu_info->comp_avl_cap_grp.dl.\
                         cell_cap_val = \
                         p_rsu_info->composite_available_capacity_group.dl_composite_available_capacity.\
                         cell_capacity_value;
        p_intra_inter_rsu_info->comp_avl_cap_grp.ul.\
                        cell_cap_val = \
                        p_rsu_info->composite_available_capacity_group.ul_composite_available_capacity.\
                        cell_capacity_value;
    }
    else
    {
        p_intra_inter_rsu_info->comp_avl_cap_grp.dl.bitmask = RRM_ZERO;
        p_intra_inter_rsu_info->comp_avl_cap_grp.ul.bitmask = RRM_ZERO;
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_DETAILED, \
                         "Composit Avl Cap grp not present");
    }
    
    /* eICIC_PHASE_1_2_CHANGES_START */
    if (bitmask & RRM_ABS_STATUS_INFO_PRESENT)
    {
        p_intra_inter_rsu_info->bitmask |= RRM_ABS_STATUS_INFO_PRESENT; 
          p_intra_inter_rsu_info->abs_status.dl_abs_status = p_rsu_info->abs_status.dl_abs_status;
          /*SPR 14867 start*/ 
#ifndef TDD_MODE_FLAG
          /*SPR 14867 end*/
          if(p_rsu_info->abs_status.x2ap_usable_abs_information.bitmask & 
                                    X2AP_USABLE_ABS_INFORMATION_FDD_PRESENT)
          {
            RRM_MEMCPY(p_intra_inter_rsu_info->abs_status.usable_abs_pattern,
                       p_rsu_info->abs_status.x2ap_usable_abs_information.
                          usable_abs_info_fdd.usable_abs_pattern_fdd_info,
                       X2AP_ABS_PATTERN_SIZE_FDD);
          }
#else
          if(p_rsu_info->abs_status.x2ap_usable_abs_information.bitmask & 
                                    X2AP_USABLE_ABS_INFORMATION_TDD_PRESENT)
          {
            RRM_MEMCPY(p_intra_inter_rsu_info->abs_status.usable_abs_pattern,
                       p_rsu_info->abs_status.x2ap_usable_abs_information.
                          usable_abs_info_tdd.usable_abs_pattern_tdd_info,
                       X2AP_ABS_PATTERN_SIZE_TDD);
          }
#endif

          if( p_intra_inter_rsu_info->abs_status.bitmask & RRM_NUM_OF_ABS_REPORT_PRESENT )
		  {
             p_intra_inter_rsu_info->abs_status.abs_usage_rep_status = ABS_USAGE_UPDATE;
             /*SPR 14523 start*/
             if(p_rsu_info->abs_status.dl_abs_status == RRM_ZERO)
			  {
				  if( RRM_DL_ABS_STATUS_ZERO == p_intra_inter_rsu_info->abs_status.abs_change_type)
				  {
					  p_intra_inter_rsu_info->abs_status.num_abs_report++; 
				  }
				  else
				  {
					  p_intra_inter_rsu_info->abs_status.num_abs_report = RRM_ONE; 
				  }
				  p_intra_inter_rsu_info->abs_status.abs_change_type = RRM_DL_ABS_STATUS_ZERO;
			  }
             /*SPR 14523 end*/
             else if(p_rsu_info->abs_status.dl_abs_status >
					  p_cell_ctx->operator_info.eicic_info.abs_info.abs_usage_high_threshold)
			  {
				  if( RRM_ABS_INCREASE == p_intra_inter_rsu_info->abs_status.abs_change_type)
				  {
					  p_intra_inter_rsu_info->abs_status.num_abs_report++; 
				  }
				  else
				  {
					  p_intra_inter_rsu_info->abs_status.num_abs_report = RRM_ONE; 
				  }
				  
				  p_intra_inter_rsu_info->abs_status.abs_change_type = RRM_ABS_INCREASE;
			  }
			  else if(p_rsu_info->abs_status.dl_abs_status <
					  p_cell_ctx->operator_info.eicic_info.abs_info.abs_usage_low_threshold)
			  {
				  if( RRM_ABS_DECREASE == p_intra_inter_rsu_info->abs_status.abs_change_type)
				  {
					  p_intra_inter_rsu_info->abs_status.num_abs_report++; 
				  }
				  else
				  {
					  p_intra_inter_rsu_info->abs_status.num_abs_report = RRM_ONE; 
				  }
				  p_intra_inter_rsu_info->abs_status.abs_change_type = RRM_ABS_DECREASE;
			  }
			  else
			  {
				  RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_DETAILED, \
						    "ABS Status is neither highr than the upper threshold nor lower than the"
							"Low threshold.");
			  }
		  }
	}
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_DETAILED, \
                  "ABS Status not present");
    }

    /* eICIC_PHASE_1_2_CHANGES_END */

    /* This is the latest status that needs to be reported */
    p_intra_inter_rsu_info->is_reported = RRM_FALSE;
    RRM_UT_TRACE_EXIT();
    return;
}

/****************************************************************************
* Function Name  : rrm_cellm_populate_nbr_rsu_in_cell_context 
* Inputs         :
*                  -  p_x2ap_rrm_rsu_enb_update_ind
*                  -  report_cnt, cell_type, nbr_cell_idx
* Outputs        : 
*                  -p_lte_ncl
* Returns        :  RRM_SUCCESS/RRM_FAILURE
*
* Variables      :
* Description    : This function populate neighbor load in NCL
****************************************************************************/
rrm_return_et rrm_cellm_populate_nbr_rsu_in_cell_context
(
    x2ap_rrm_rsu_enb_update_ind_t *p_x2ap_rrm_rsu_enb_update_ind,
	/* eICIC_PHASE_1_2_CHANGES_START */
    rrm_cell_context_t *p_cell_ctx ,
    U16 report_cnt,
    rrm_eutran_cell_types_et cell_type,
    U16 nbr_cell_idx
)
{

	rrm_return_et ret_val = RRM_SUCCESS;
	RRM_UT_TRACE_ENTER();

	RRM_ASSERT(RRM_PNULL != p_x2ap_rrm_rsu_enb_update_ind);
	RRM_ASSERT(RRM_PNULL != p_cell_ctx);

	rrm_populate_nbr_rsu_cell(&( p_x2ap_rrm_rsu_enb_update_ind->rsu_report[report_cnt].rsu_info),
			p_cell_ctx, cell_type ,nbr_cell_idx);
	/* eICIC_PHASE_1_2_CHANGES_END */

	RRM_UT_TRACE_EXIT();
	return ret_val;
}

rrm_return_et
rrm_cell_load_action_none_hndl
(
    rrm_cell_context_t *p_cell_ctxt,
    rrm_cell_load_def_t *p_load_def,
    U8 ld_choice,
    U8 resrc_cnt
)
{
    rrm_return_et ret_val = RRM_SUCCESS;
    /*SPR 17777 +-*/
    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF,
                "No Actin Defined for Load;"
                "Unused variable p_cell_ctxt=%p p_load_def=%p ld_choice=%u resrc_cnt=%u"
                ,p_cell_ctxt,p_load_def,ld_choice,resrc_cnt);
    /*SPR 17777 +-*/
    return ret_val;
}

rrm_return_et
rrm_cell_load_action_ho_hndl
(
    rrm_cell_context_t  *p_cell_ctxt,
    rrm_cell_load_def_t *p_load_def,
    U8 ld_choice,
    U8 resrc_cnt
)
{
    RRM_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF,
                    "Triggered action [Handover]"
                    "Unused variable ld_choice=%u resrc_cnt=%u",ld_choice,resrc_cnt);
    /*SPR 17777 +-*/

    /* This flag is set to TRUE which will be used in HO procedure
     * to handle Admisnistartive HO procedure with different flow
     */
    p_cell_ctxt->administrative_procedure_ongoing = RRM_TRUE;
    rrm_sort_ue_for_ho_send_meas_config(p_cell_ctxt->cell_index, p_load_def);
    p_cell_ctxt->administrative_procedure_ongoing = RRM_FALSE;

    RRM_UT_TRACE_EXIT();
    return RRM_SUCCESS;
}


rrm_return_et
rrm_cell_load_action_stop_adm_hndl
(
    rrm_cell_context_t *p_cell_ctxt,
    rrm_cell_load_def_t *p_load_def,
    U8 ld_choice,
    U8 resrc_cnt
)
{
    rrm_return_et ret_val = RRM_SUCCESS;
    RRM_UT_TRACE_ENTER();
    RRM_ASSERT(RRM_PNULL != p_cell_ctxt);
    RRM_ASSERT(RRM_PNULL != p_load_def);
    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF,
                    "Triggered action [Stop Admission]");
    if (ld_choice == RRM_CELL_LOAD_COMPOSITE_PRESENT)
    {
        if (RRM_TRUE != p_cell_ctxt->stop_adm_flag && !(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_STOP_ADM))
        {
            p_cell_ctxt->stop_adm_flag = RRM_TRUE;
            p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status |= RRMCM_ACTN_STOP_ADM;
        }
        else
        {
            /*SPR 17777 +-*/
            RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_WARNING,
                    "CELLM Load Action Stop Adm <Stop Adm Flags> Already set by other"
                    "Unused variable p_load_def=%p ld_choice=%u",p_load_def,ld_choice);
            /*SPR 17777 +-*/
        }
    }
    else
    {
        if (RRM_TRUE != p_cell_ctxt->stop_adm_flag && !(p_cell_ctxt->cell_load_action.u.resrc_spec_info.
            resrc_spec_info[resrc_cnt].ld_actn.actn_status & RRMCM_ACTN_STOP_ADM))
        {
            p_cell_ctxt->stop_adm_flag = RRM_TRUE;
            p_cell_ctxt->cell_load_action.u.resrc_spec_info.
                        resrc_spec_info[resrc_cnt].ld_actn.actn_status |= RRMCM_ACTN_STOP_ADM;
        }
        else
        {
            RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_WARNING,
                    "CELLM Load Action Stop Adm <Stop Adm Flags> Already set by other");
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}


/****************************************************************************
 * Function Name  : rrm_cell_load_action_rel_ue_hndl 
 * Inputs 	      :	  rrm_cell_context_t *p_cell_ctxt
 *    			  :	  rrm_cell_load_def_t *p_load_def
 *  		      :	  U8 ld_choice
 *   			  :	  U8 resrc_cnt
 * Returns        : RRM_SUCCESS/RRM_FAILURE 
 * Description    : This function releases ue against the action of load  
 ****************************************************************************/
rrm_return_et
rrm_cell_load_action_rel_ue_hndl
(
    rrm_cell_context_t *p_cell_ctxt,
    rrm_cell_load_def_t *p_load_def,
    U8 ld_choice,
    U8 resrc_cnt
)
{
    rrm_return_et ret_val = RRM_SUCCESS;

    /*SPR-11368 Fix Starts*/
    U8 num_ue_to_release = RRM_ZERO;

    /*SPR-695 Fix Starts*/
    U8 num_of_ue = RRM_ZERO;
    /*SPR-695 Fix Ends*/

    /*SPR-11368 Fix End*/

    RRM_UT_TRACE_ENTER();
    RRM_ASSERT(RRM_PNULL != p_cell_ctxt);
    RRM_ASSERT(RRM_PNULL != p_load_def);

    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF,
            "Triggered action [UE Release] for cell index:%d ", p_cell_ctxt->cell_index);

    /*SPR-11368 Fix Starts*/
    /* Number of ue to be released calculated only if active ue present */ 
    if (p_cell_ctxt->cell_resource_info.num_active_ue > RRM_ZERO)
    {
/*spr_22158_changes_start*/
	num_of_ue = rrm_cellm_get_num_active_ue(p_cell_ctxt->cell_index);
/*spr_22158_changes_end*/
        if(num_of_ue >= p_load_def->num_of_usr)
        {
            num_ue_to_release = p_load_def->num_of_usr;
        }
        else
        {
            num_ue_to_release = num_of_ue;
        }
        /*Cov_fix_start_64542*/
        if (RRM_SUCCESS != (ret_val = rrm_cellm_search_and_release_lowest_priority_arp(p_cell_ctxt->cell_index, num_ue_to_release)))
        {
            /* Error already captured inside the function
             * Breaking from here as no more lowest priority UE exist in
             * system */
        }
        /*Cov_fix_end_64542*/
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF,
                "No UE available for release  for cell index:%d",p_cell_ctxt->cell_index);
    }
    /*SPR-11368 Fix END*/

    if (RRM_CELL_LOAD_COMPOSITE_PRESENT == ld_choice)
    {
        if (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_REL_UE))
        {
            p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status |= RRMCM_ACTN_REL_UE;
        }
    }
    else
    {
        if(!(p_cell_ctxt->cell_load_action.u.resrc_spec_info.
                    resrc_spec_info[resrc_cnt].ld_actn.actn_status & RRMCM_ACTN_REL_UE))
        {
            p_cell_ctxt->cell_load_action.u.resrc_spec_info.
                resrc_spec_info[resrc_cnt].ld_actn.actn_status |= RRMCM_ACTN_REL_UE;
        }

    }

    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF,
            "UE Release impacted [%u] UE(s)", num_ue_to_release);

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* Changes start for MLB Ph2 SSAC */
rrm_return_et
rrm_cell_load_action_ssac_bar_hndl
(
    rrm_cell_context_t *p_cell_ctxt,
    rrm_cell_load_def_t *p_load_def,
    U8 ld_choice,
    U8 resrc_cnt
)
{
    rrm_return_et ret_val = RRM_SUCCESS;
    RRM_UT_TRACE_ENTER();
    RRM_ASSERT(RRM_PNULL != p_cell_ctxt);
    RRM_ASSERT(RRM_PNULL != p_load_def);
    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF,
                    "Triggered action [SSAC Barring]");
    if (RRM_PNULL == p_cell_ctxt->p_cell_ld_reconfig_data &&
        !(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_AC_BARRING))
    {
        p_cell_ctxt->p_cell_ld_reconfig_data = rrm_mem_get (sizeof(rrm_cellm_load_ac_barring_t));
	/* Coverity 88362 Fix Start */
        RRM_ASSERT (RRM_PNULL != p_cell_ctxt->p_cell_ld_reconfig_data);
	if (RRM_PNULL == p_cell_ctxt->p_cell_ld_reconfig_data)
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR, "Memory allocation failed for p_cell_ld_reconfig_data");
            ret_val = RRM_FAILURE;
        }
        else
        {
        	RRM_MEMCPY(p_cell_ctxt->p_cell_ld_reconfig_data, &p_load_def->ld_ac_bar, sizeof(rrm_cellm_load_ac_barring_t));
        	ret_val = cellm_fsm_process_event(RRC_RRM_MLB_CELL_RECONFIG_REQ_EVENT, p_cell_ctxt);    
	}
	/* Coverity 88362 Fix End */
    }
    /* BUG_11576_FIX_START */
    /* Coverity 88362 Fix Start */
    if (RRM_PNULL != p_cell_ctxt->p_cell_ld_reconfig_data)
    {
        RRM_MEM_FREE(p_cell_ctxt->p_cell_ld_reconfig_data);
    }
    /* Coverity 88362 Fix End */
    /* BUG_11576_FIX_END */
    if (RRM_CELL_LOAD_COMPOSITE_PRESENT == ld_choice)
    {
        if (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_AC_BARRING))
        {
            p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status |= RRMCM_ACTN_AC_BARRING;
        }
    }
    else
    {
        if(!(p_cell_ctxt->cell_load_action.u.resrc_spec_info.
            resrc_spec_info[resrc_cnt].ld_actn.actn_status & RRMCM_ACTN_AC_BARRING))
        {
            p_cell_ctxt->cell_load_action.u.resrc_spec_info.
                        resrc_spec_info[resrc_cnt].ld_actn.actn_status |= RRMCM_ACTN_AC_BARRING;
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* Changes End for MLB Ph2 SSAC */
/* Code for MLB End */
/*ECN load start*/
rrm_return_et
rrm_cell_load_action_ecn_bi_dir_hndl
(
    rrm_cell_context_t *p_cell_ctxt,
    rrm_cell_load_def_t *p_load_def,
    U8 ld_choice,
    U8 resrc_cnt
)
{
    RRM_UT_TRACE_ENTER();
    RrmPdcpEcnConfigParamReq                pdcp_ecn_config_param_req = {RRM_ZERO};
    rrm_return_et ret_val = RRM_SUCCESS;

    /*Send config request to PDCP for both direction 
    also check that whether it is already sent in ECN capacity req or not*/
    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF,
                    "Triggered action [BI-Direction]");
    if(!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_RRM_ECN_BI_DIR))
    {
        if ((p_cell_ctxt->p_cell_ecn_config == RRM_PNULL)&&
                (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_RRM_ECN_UL_DIR)) &&
                (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status &  RRMCM_ACTN_RRM_ECN_DL_DIR)) &&
                (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status &  RRMCM_ACTN_AQM)))
            /*Need to chk the bitmask is set for both direction or not*/
        {
            pdcp_ecn_config_param_req.ecnFeatureStatus = ECN_APPLIED_IN_BOTH_UL_DL; 
            pdcp_ecn_config_param_req.ecnTrigger=ECN_T2_BASED_ON_RRM_DETECTION; 
        }
        /* Coverity 73873 Fix Start */
        else if ((p_cell_ctxt->p_cell_ecn_config == RRM_PNULL)&&
                (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_RRM_ECN_UL_DIR)) &&
                (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status &
                   RRMCM_ACTN_RRM_ECN_BI_DIR)) &&
                (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status &  RRMCM_ACTN_AQM)))
        {
            pdcp_ecn_config_param_req.ecnFeatureStatus = ECN_APPLIED_IN_DL; 
            pdcp_ecn_config_param_req.ecnTrigger = ECN_T2_BASED_ON_RRM_DETECTION; 
        }
        else
        {
            pdcp_ecn_config_param_req.ecnFeatureStatus = ECN_APPLIED_IN_UL; 
            pdcp_ecn_config_param_req.ecnTrigger = ECN_T2_BASED_ON_RRM_DETECTION; 
        }
        /* Coverity 73873 Fix End */
        pdcp_ecn_config_param_req.lowWaterMark=DEFAULT_LOW_WATER_MARK; 
        pdcp_ecn_config_param_req.highWaterMark= DEFAULT_HIGH_WATER_MARK;
        ret_val = rrm_send_pdcp_config_param_req(&pdcp_ecn_config_param_req, RRM_MODULE_ID, RRM_ZERO, p_cell_ctxt->cell_index);
        /*Start checking the SINR and thp */
        /* SPR 10869 start */
        rrm_cell_cal_need_for_ecn(p_cell_ctxt, p_load_def, ECN_APPLIED_IN_BOTH_UL_DL);
        /* SPR 10869 end */
        /*Set the action as AQM is set*/
    }
    else
    {
        /*SPR 17777 +-*/
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_WARNING,
                    "CELLM Load Action ECN UL DL <ECN UL DL Flags> Already set by other"
                    "Unused variable ld_choice=%u resrc_cnt=%u",ld_choice,resrc_cnt);
        /*SPR 17777 +-*/
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
rrm_return_et
rrm_cell_load_action_ecn_dl_dir_hndl
(
    rrm_cell_context_t *p_cell_ctxt,
    rrm_cell_load_def_t *p_load_def,
    U8 ld_choice,
    U8 resrc_cnt
)
{
    RRM_UT_TRACE_ENTER();
    RrmPdcpEcnConfigParamReq                pdcp_ecn_config_param_req = {RRM_ZERO};
    rrm_return_et ret_val = RRM_SUCCESS;

    /*Send config request to PDCP for both direction 
    also check that whether it is already sent in ECN capacity req or not*/
    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF,
                    "Triggered action [DL Direction]");
    if(!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_RRM_ECN_DL_DIR))
    {
        /*Need to chk the bitmask is set for both direction or not*/
        if ((p_cell_ctxt->p_cell_ecn_config == RRM_PNULL)&&
	    (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_RRM_ECN_UL_DIR)) &&
	    (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status &  RRMCM_ACTN_RRM_ECN_BI_DIR)) &&
	    (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status &  RRMCM_ACTN_AQM)))
        {
            pdcp_ecn_config_param_req.ecnFeatureStatus = ECN_APPLIED_IN_DL; 
        }
        else
        {
            pdcp_ecn_config_param_req.ecnFeatureStatus = ECN_APPLIED_IN_BOTH_UL_DL; 
        }
        pdcp_ecn_config_param_req.lowWaterMark=DEFAULT_LOW_WATER_MARK; 
        pdcp_ecn_config_param_req.highWaterMark= DEFAULT_HIGH_WATER_MARK;
        pdcp_ecn_config_param_req.ecnTrigger=ECN_T2_BASED_ON_RRM_DETECTION; 
        ret_val = rrm_send_pdcp_config_param_req(&pdcp_ecn_config_param_req, RRM_MODULE_ID, RRM_ZERO, p_cell_ctxt->cell_index);
        /*Start checking the SINR and thp */
        /* SPR 10869 start */
        rrm_cell_cal_need_for_ecn(p_cell_ctxt, p_load_def, ECN_APPLIED_IN_DL);
        /* SPR 10869 end */
        /*Set the action as AQM is set*/
    }
    else
    {
        /*SPR 17777 +-*/
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_WARNING,
                    "CELLM Load Action ECN DL <ECN DL Flags> Already set by other"
                    "Unused variable ld_choice=%u resrc_cnt=%u",ld_choice,resrc_cnt);
        /*SPR 17777 +-*/
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;

}
rrm_return_et
rrm_cell_load_action_ecn_ul_dir_hndl
(
    rrm_cell_context_t *p_cell_ctxt,
    rrm_cell_load_def_t *p_load_def,
    U8 ld_choice,
    U8 resrc_cnt
)
{
    RRM_UT_TRACE_ENTER();
    RrmPdcpEcnConfigParamReq                pdcp_ecn_config_param_req = {RRM_ZERO};
    rrm_return_et ret_val = RRM_SUCCESS;

    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF,
                    "Triggered action [UL Direction]");
    /*Send config request to PDCP for both direction 
    also check that whether it is already sent in ECN capacity req or not*/
    if(!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_RRM_ECN_UL_DIR))
    {
        /*Need to chk the bitmask is set for both direction or not*/
        if ((p_cell_ctxt->p_cell_ecn_config == RRM_PNULL) && 
	    (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_RRM_ECN_DL_DIR)) &&
	    (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status &  RRMCM_ACTN_RRM_ECN_BI_DIR)) &&
	    (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status &  RRMCM_ACTN_AQM)))
        {
            pdcp_ecn_config_param_req.ecnFeatureStatus = ECN_APPLIED_IN_UL; 
        }
        else
        {
            pdcp_ecn_config_param_req.ecnFeatureStatus = ECN_APPLIED_IN_BOTH_UL_DL; 
        }
        pdcp_ecn_config_param_req.ecnTrigger=ECN_T2_BASED_ON_RRM_DETECTION; 
        pdcp_ecn_config_param_req.lowWaterMark=DEFAULT_LOW_WATER_MARK; 
        pdcp_ecn_config_param_req.highWaterMark= DEFAULT_HIGH_WATER_MARK;
        ret_val = rrm_send_pdcp_config_param_req(&pdcp_ecn_config_param_req, RRM_MODULE_ID, RRM_ZERO, p_cell_ctxt->cell_index);
        /*Start checking the SINR and thp */
        /* SPR 10869 start */
        rrm_cell_cal_need_for_ecn(p_cell_ctxt, p_load_def, ECN_APPLIED_IN_UL);
        /* SPR 10869 end */
        /*Set the action as AQM is set*/
    }
    else
    {
        /*SPR 17777 +-*/
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_WARNING,
                    "CELLM Load Action ECN UL <ECN UL Flags> Already set by other"
                    "Unused variable ld_choice=%u resrc_cnt=%u",ld_choice,resrc_cnt);
        /*SPR 17777 +-*/
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

rrm_return_et
rrm_cell_load_action_aqm_hndl
(
    rrm_cell_context_t *p_cell_ctxt,
    rrm_cell_load_def_t *p_load_def,
    U8 ld_choice,
    U8 resrc_cnt
)
{
    RRM_UT_TRACE_ENTER();
    RrmPdcpEcnConfigParamReq  pdcp_ecn_config_param_req = {RRM_ZERO};
    rrm_return_et ret_val = RRM_SUCCESS;

    /*Send config request to PDCP for DL direction 
    also check that whether it is already sent in ECN capacity req or not*/
    if(!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_AQM))
    {
        if ((p_cell_ctxt->p_cell_ecn_config == RRM_PNULL) &&
	    (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_RRM_ECN_DL_DIR)) &&
	    (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status &  RRMCM_ACTN_RRM_ECN_BI_DIR))&&
	    (!(p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status &  RRMCM_ACTN_RRM_ECN_UL_DIR)))
        {
            pdcp_ecn_config_param_req.ecnFeatureStatus = ECN_APPLIED_IN_DL; 
            pdcp_ecn_config_param_req.ecnTrigger=ECN_T1_BASED_ON_WATERMARKS; 
        }
        else
        {
            pdcp_ecn_config_param_req.ecnFeatureStatus = ECN_APPLIED_IN_BOTH_UL_DL; 
            pdcp_ecn_config_param_req.ecnTrigger=ECN_BOTH_TRIGGERS_APPLIED; 
        }
        pdcp_ecn_config_param_req.lowWaterMark=p_load_def->q_watermark.low_watermark; 
        pdcp_ecn_config_param_req.highWaterMark= p_load_def->q_watermark.high_watermark;
        ret_val = rrm_send_pdcp_config_param_req(&pdcp_ecn_config_param_req, RRM_MODULE_ID, RRM_ZERO, p_cell_ctxt->cell_index);
        /*Set the action as AQM is set*/
        p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status |= RRMCM_ACTN_AQM;
    }
    else
    {
        /*SPR 17777 +-*/
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_WARNING,
                    "CELLM Load Action AQM is already set"
                    "Unused variable ld_choice=%u resrc_cnt=%u",ld_choice,resrc_cnt);
        /*SPR 17777 +-*/
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/*ECN load end*/
rrm_return_et
rrm_cell_load_action_rel_ue_carr_redir_hndl
(
    rrm_cell_context_t *p_cell_ctxt,
    rrm_cell_load_def_t *p_load_def,
    U8 ld_choice,
    U8 resrc_cnt
)
{
    rrm_return_et ret_val = RRM_SUCCESS;
    rrmuem_cm_redirect_rel_ind_t   redirect_ue_rel = {0}; 
    rrm_ue_index_t                 ue_index = RRM_ZERO;
    U8                             num_ue_to_release = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
/*spr_22158_changes_start*/
RRM_ASSERT(RRM_PNULL != p_cell_ctxt);
/*spr_22158_changes_end*/

    for (num_ue_to_release = RRM_ZERO; num_ue_to_release <
            p_load_def->num_of_usr;
            num_ue_to_release++)
    {
        /*Bug_721_changes_start*/
/*spr_22158_changes_start*/
        if (RRM_SUCCESS == (ret_val = rrm_ue_get_low_priority_ue(p_cell_ctxt->cell_index, &ue_index, RRM_ZERO, RRM_ZERO, RRM_FALSE)))
/*spr_22158_changes_end*/
            /*Bug_721_changes_end*/
        {
            /*Set the action of release with redirect ue*/
            rrm_update_ue_actn(ue_index, p_cell_ctxt->cell_index);
            redirect_ue_rel.ue_index = ue_index;
            redirect_ue_rel.cell_index = p_cell_ctxt->cell_index;
            /* Bug_944_start */
            if (RRM_CELLM_MARD_INFO_PRESENT & p_load_def->bitmask)
            {
                redirect_ue_rel.bitmask |= RRMUEM_CM_MARD_INFO_PRESENT;
                if (RRM_CELLM_MARD_PRESENT & p_load_def->mard_info.bitmask)
                {
                    redirect_ue_rel.mard_info.bitmask |= RRMUEM_CM_MARD_PRESENT;
                    redirect_ue_rel.mard_info.mard_present = p_load_def->mard_info.mard_present;
                }
                if (RRM_CELLM_MEAS_BASED_RWR_TIMER_PRESENT & p_load_def->mard_info.bitmask)
                {
                    redirect_ue_rel.mard_info.bitmask |= RRMUEM_CM_MEAS_BASED_RWR_TIMER_PRESENT;
                    redirect_ue_rel.mard_info.rwr_timer = p_load_def->mard_info.rwr_timer;
                }
            }
            /* Bug_944_end */

            ret_val = rrm_send_internal_msg((U16)RRM_UEM_MODULE_ID,
                    RRMUEM_CM_REDIRECT_REL_IND,
                    sizeof(rrmuem_cm_redirect_rel_ind_t),
                    (void *)&(redirect_ue_rel));
        }
        else
        {
            /*SPR 17777 +-*/
            RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_WARNING,
                    "No low priority found"
                    "Unused variable ld_choice=%u resrc_cnt=%u",ld_choice,resrc_cnt);
            /*SPR 17777 +-*/
            break;
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* Carrier_Aggregation_Start */
/****************************************************************************
* Function Name  : rrm_build_and_send_scell_deact_ind 
* Inputs         : num_of_usr,
*                : cell_id,
*                : p_cell_ctxt
* Outputs        : None
* Returns        : RRM_SUCCESS/RRM_FAILURE
* Description    : This function is use to build and send 
*                : scell deactivation indication to peer cells on same enb.
****************************************************************************/
rrm_return_et
rrm_build_and_send_scell_deact_ind 
(
    U8 num_of_usr,
    rrm_oam_eutran_global_cell_id_t cell_id,
    rrm_cell_context_t *p_cell_ctxt
)
{
    rrm_return_et ret_val                               = RRM_SUCCESS;
    rrm_load_actn_scell_deact_ind_t scell_deact_ind     = {RRM_ZERO};

    RRM_UT_TRACE_ENTER();

    scell_deact_ind.cell_index_of_deact_scell = p_cell_ctxt->cell_index;
    scell_deact_ind.num_of_user = num_of_usr;
    /* copying global cell id */ 
    RRM_MEMCPY(&scell_deact_ind.global_cell_id, &cell_id,
                        sizeof (rrm_oam_eutran_global_cell_id_t));
    /* Sending scell deactivation indication to peer cell on same enb */
    if (RRM_FAILURE == rrm_send_internal_msg ((U16)RRM_CM_MODULE_ID,
                RRMCM_CELL_LOAD_ACTN_SCELL_DEACT_IND,
                sizeof(rrm_load_actn_scell_deact_ind_t),
                (void *)&scell_deact_ind))

    {
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_ERROR,
                "Fail to send api_id :%d cell index:%d",
                RRMCM_CELL_LOAD_ACTN_SCELL_DEACT_IND,
                p_cell_ctxt->cell_index);
        ret_val = RRM_FAILURE;
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF,
                "Successfully send api_id :%d cell index:%d",
                RRMCM_CELL_LOAD_ACTN_SCELL_DEACT_IND,
                p_cell_ctxt->cell_index);
        ret_val = RRM_SUCCESS;
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;

}
/* eICIC_PHASE_1_2_CHANGES_START */
/****************************************************************************
* Function Name  : rrm_cell_load_abs_pattern_send_to_l2
* Inputs         : p_cell_ctxt, p_load_def, ld_choice, resrc_cnt
* Outputs        : None
* Returns        : RRM_SUCCESS/RRM_FAILURE
* Description    : This function is use to send ABS pattern to L2 if the load 
*                  has changed.
****************************************************************************/
rrm_return_et
rrm_cell_load_abs_pattern_send_to_l2
 ( 
    rrm_cell_context_t *p_cell_ctx,
    rrm_cell_load_def_t *p_load_def
    /*SPR 14182 start*/
    //code_delete 
    /*SPR 14182 start*/
    )
{
    rrm_return_et ret_val  = RRM_SUCCESS;
    /*SPR 14182 start*/
    if(RRM_FALSE == rrm_check_abs_pattern_is_null( p_load_def->eicic_load_config.abs_pattern))
    /*SPR 14182 end*/
    {    
        if(RRM_ZERO != RRM_MEMCMP( p_cell_ctx->rrm_eicic_cell_info.serving_abs_pattern_applied,
                    p_load_def->eicic_load_config.abs_pattern,
                    RRM_MAX_PATTERN_BYTE))
        {

            RRM_MEMCPY( p_cell_ctx->rrm_eicic_cell_info.temp_srv_abs_pattern,
                    p_load_def->eicic_load_config.abs_pattern,
                    RRM_MAX_PATTERN_BYTE);
            ret_val = build_and_send_l2_cell_config_req( RRM_PNULL,
                    RRM_PNULL,
                    p_cell_ctx->rrm_eicic_cell_info.temp_srv_abs_pattern,
                    RRM_PNULL,
                    p_cell_ctx->cell_index,
                    p_cell_ctx->ongoing_trans_id);
            /*ut_bug_13978_fix_start*/
            if(ret_val == RRM_SUCCESS)
            {
                CELL_M_FSM_SET_STATE(p_cell_ctx,CELL_STATE_UEM_W_FOR_ICIC_ACTION_FRM_UEM_OR_L2);
                p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask |= RRM_CTXT_SRV_ABS_RECONFIG_INFO_PRESENT;
                p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask |= RRM_CTXT_ABS_PATTERN_FLAG_PRESENT;
            }
            else 
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF,
                        "Failure returned during the processing of l2_cell_config_req");

            }
            /*ut_bug_13978_fix_end*/
        }
        else
        {
            RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_DETAILED,
                    "Load level is same as the one which was previously configured,so no new request to L2 is sent");
        }
    }
    /*SPR 14182 start*/
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_ERROR,
                "ABS pattern is null for the current load");
        ret_val = RRM_FAILURE; 
     }    
     /*SPR 14182 end*/
    return ret_val;
}
/* eICIC_PHASE_1_2_CHANGES_END */
/****************************************************************************
* Function Name  : rrm_cell_load_action_scell_deact_hndl 
* Inputs         : p_cell_ctxt, p_load_def, ld_choice, resrc_cnt
* Outputs        : None
* Returns        : RRM_SUCCESS/RRM_FAILURE
* Description    : This function is use to handle scell deactivation action.
****************************************************************************/
rrm_return_et
rrm_cell_load_action_scell_deact_hndl
(
    rrm_cell_context_t *p_cell_ctxt,
    rrm_cell_load_def_t *p_load_def,
    U8 ld_choice,
    U8 resrc_cnt
)
{
    rrm_return_et ret_val                               = RRM_SUCCESS;
    lte_ncl_t *lte_ncl                                  = RRM_PNULL;
    U8 ncl_cnt                                          = RRM_ZERO;
    U8 scell_deact_ind_send_cnt                         = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_DETAILED,
                    "Triggered action [Scell deactivation]");

    if (ld_choice == RRM_CELL_LOAD_COMPOSITE_PRESENT)
    {
        if (p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status & RRMCM_ACTN_RRM_SCELL_DEACTIVATE)
        {
            RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_WARNING,
                    "CELLM Load Action Scell deactivation <Scell deactivation Flags> Already set by other");
        }
        else
        {
            p_cell_ctxt->cell_load_action.u.comp_load_info.actn_status |= RRMCM_ACTN_RRM_SCELL_DEACTIVATE;
        }
    }
    else
    {
        if (p_cell_ctxt->cell_load_action.u.resrc_spec_info.
                    resrc_spec_info[resrc_cnt].ld_actn.actn_status & RRMCM_ACTN_RRM_SCELL_DEACTIVATE)
        {
            RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_WARNING,
                    "CELLM Load Action Scell deactivation <Scell deactivation Flags> Already set by other");
        }
        else
        {
            p_cell_ctxt->cell_load_action.u.resrc_spec_info.
                resrc_spec_info[resrc_cnt].ld_actn.actn_status |= RRMCM_ACTN_RRM_SCELL_DEACTIVATE;
        }
    }

    lte_ncl = rrm_cellm_get_lte_ncl_info (p_cell_ctxt->cell_index);
    if(RRM_PNULL != lte_ncl)
    {
        for (ncl_cnt = RRM_ZERO; ((ncl_cnt < lte_ncl->num_valid_inter_freq_cell) &&
            (ncl_cnt < MAX_INTER_FREQ_CELLS)); ncl_cnt++)
        {
            /* Checking: Is cell is locally connected to peer cells on same enb */
            if (X2_LOCALLY_CONNECTED_CELL == 
                    lte_ncl->inter_freq_cells[ncl_cnt].x2_status)
            {
                ret_val = rrm_build_and_send_scell_deact_ind (p_load_def->num_of_usr,
                        lte_ncl->inter_freq_cells[ncl_cnt].cell_id, p_cell_ctxt);
                if (RRM_SUCCESS == ret_val)
                {
                    scell_deact_ind_send_cnt++;
                }
            }
            else
            {
                RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF,
                    "cell is not connected to local peer cell on same enb x2_status is %d",
                    lte_ncl->inter_freq_cells[ncl_cnt].x2_status);
            }
        }
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_DETAILED,
                "Send scell deactivation indication to %d no. of peer cell",
                scell_deact_ind_send_cnt);
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_WARNING,
                "NCL list is null on cell_index[%d]",p_cell_ctxt->cell_index);
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* Carrier_Aggregation_End */

/* Carrier Aggregation start */

