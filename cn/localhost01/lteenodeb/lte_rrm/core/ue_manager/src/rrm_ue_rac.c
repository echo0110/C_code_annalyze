/* vim: ts=4:et:ai:ci:sw=4
*/
/******************************************************************************
 *
 * ARICENT  -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_ue_rac.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains the radio admission control functions.
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   3 Apr,2012      Harish       Initial
 *   2 MAY,2012      gur30487                    Fix regarding the bearer
 *                                               admission control according
 *                                               to the max no of bearer
 *                                               configured by OAM
 *   5 JUN,2012      gur30784                    emergency feature changes
 *   25 JUN,2012     gur30784                    changes for TNL feature 
 *   27JUN,2012      gur30784                    QOS modification feature
 *   26JUL,2012      gur27278                    Removed Compilation warnings
 *   28Nov,2012      gur21481                    BUG:220 fixed
 *   06Dec,2012	     gur31824                    BUG:268 fixed
 *   29Jan,2013      gur27278                    Added code for SPS feature
 *   16Jul,2013      gur11056                    Added code for SPR 490
 *   28 May,2014     gur29990      11386         RRM trigger HO of Non member UE to Closed cell
 *    2 July,2014     gur25767   10498           Inter-freq Daho Support
 *   02July,2014     gur26515      10450         Implemented DRX Staggering.
 *
 *   25 Apr 2014     gur31824     SPR 10729      RRM is not releasing the UE
 *                                               when RRC sends
 *                                               RRC_RRM_LOCAL_ERROR_IND to
 *                                               RRM for UE with single rabs
 *   16Apr,2014      gur32417                    Carrier Aggregation Stage 3 code
 *                                               added
 *    2 July,2014     gur25767   10498           Inter-freq Daho Support
 *   02July,2014     gur26515      10450         Implemented DRX Staggering.
 *   19 July,2014    gur31824    SPR 12473       RRM is admitting GBR rab 
 *                                               even resource are not available.
 *   11 Aug,2014     gur27356    SPR-12963       After PCI Confusion Reported, HO failure for neighbor 2.
 ******************************************************************************/
#include "rrm_stats.h"
#include "rrm_ue_context.h"
#include "rrm_cell_mgr.h"
#include "rrm_defines.h"
#include "rrm_utils.h"
#include "rrm_rrc_ue_intf.h"
#include "rrm_ue_mgr.h"
#include "rrm_ue_ho_utils.h"
/* Event handler header file*/
#include "rrm_events.h"
/*spr 17437 fix start*/
#include "rrm_ue_fsm.h"
/*spr 17437 fix stop*/

/*SPR 15713 Fix Start*/
/* Moved to rrm_ue_context.h */
/*SPR 15713 Fix End*/
/* warnings removal: macro INVALID_RI_INDEX is 
   defined to initialize 'ri_index' and INVALID_RES_INDEX 
   is changed from 0xFFFF to 0xFF, 10/08/2012 */
/*SPR 15713 Fix Start*/
/* Moved to rrm_ue_context.h */
/*SPR 15713 Fix End*/
/* Used in logger for identifying RAC (just for avoid string duplication) */
const S8* p_g_rrm_uem_rac_facility_name = (const S8*)"eNodeB RRM RAC";

#define RRM_UEM_RAC_TRACE(is_log_enable, log_level, format, ...)\
    RRM_LOG(is_log_enable, p_g_rrm_uem_rac_facility_name,log_level,format, ##__VA_ARGS__)

/*spr 19968 fix start*/
/****************************************************************************
 * Function Name  : rrm_is_self_rab_selected_for_deletion
 * Inputs         : p_rrm_ue_ctxt
 * 	          : p_rab_removal_ctxt
 * Outputs        : 
 * Returns        : RRM_TURE/RRM_FALSE
 * Description    : It check if the self rab is selcted for deletion or not.
 * 		    There could a situation where the number of Erabs were maximum
 * 		    but one rab is already selected for preeption in previous itertion.
 * 		    In that case max limit check is not required.
 ****************************************************************************/
    static rrm_bool_et 
rrm_is_self_rab_selected_for_deletion(rrm_ue_context_t *p_rrm_ue_ctxt,
        rrm_rab_removal_ctxt_t          *p_rab_removal_ctxt)
{
    rrm_bool_et ret_val = RRM_FALSE;
    RRM_UT_TRACE_ENTER();
    if((RRM_ZERO < p_rrm_ue_ctxt->erab_to_be_released_item_list.num_of_list) ||
            (RRM_ZERO < ylCount(&p_rab_removal_ctxt->rabs_to_rem_list)))
    {
        ret_val = RRM_TRUE;
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* FGI Feature Start */ 
/****************************************************************************
 * Function Name  : rrm_find_num_um_am_drb_configd
 * Inputs         : UE context, num_um_mode_drb_configd, num_am_mode_drb_configd
 * Outputs        : None
 * Returns        : void
 * Description    : This function returns the count of UM/AM mode 
 *                  drb configured
 ****************************************************************************/
    void
rrm_find_num_um_am_drb_configd(U32     *p_num_um_mode_drb_configd,
        U32     *p_num_am_mode_drb_configd,
        rrm_ue_context_t   *p_rrm_ue_context)
{
    U32   drb_index = RRM_ZERO;
    U32   num_um_mode = RRM_ZERO;
    U32   num_am_mode = RRM_ZERO;

    for (drb_index = RRM_ZERO;
            drb_index < p_rrm_ue_context->drb_configured.num_of_list;
            drb_index++)
    {
        if (p_rrm_ue_context->drb_configured.
                erab_item[drb_index].data_transfer_mode == AM_MODE)
        {
            num_am_mode++;
        }

        else if (p_rrm_ue_context->drb_configured.
                erab_item[drb_index].data_transfer_mode == UM_MODE)
        {
            num_um_mode++;
        }
    }

    *p_num_um_mode_drb_configd = num_um_mode;
    *p_num_am_mode_drb_configd = num_am_mode;
    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,   
            "AM Mode Count[%d]   UM Mode Count [%d]", 
            *p_num_am_mode_drb_configd, *p_num_um_mode_drb_configd);
}

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
                                   rrm_cell_index_t  cell_index)
{
  epc_params_t               *p_epc_params = RRM_PNULL;
  U16			      qos_count = RRM_ZERO;
  U8		              data_transfer_mode = RRM_ZERO;

  RRM_UT_TRACE_ENTER();

  p_epc_params = rrm_cellm_get_erb_service_profile(cell_index); 

  for (qos_count = RRM_ZERO;
       qos_count < p_epc_params->num_valid_qos_profiles; 
       qos_count++)
  {
    if (qci == p_epc_params->qos_config_params[qos_count].qci)
    {
      data_transfer_mode = p_epc_params->qos_config_params[qos_count].
	                           erb_service_profile.data_transfer_mode;
      break;
    }
  }

  RRM_UT_TRACE_EXIT();
  return data_transfer_mode;
}
/* FGI Feature End */

/****************************************************************************
 * Function Name  : rrm_ue_check_premption_capability
 * Inputs         : UE context pointer
 *                  preemption capability
 * Outputs        : None
 * Returns        : RRM_TRUE/RRM_FALSE
 * Description    : Checks if ue is preemption capable or not and if its 
 * 		    emergency UE or not
 ****************************************************************************/
rrm_bool_et
    rrm_ue_check_premption_capability
(
 rrm_ue_context_t *p_rrm_ue_ctxt,
 U8 preemption_capability
 )
{
    RRM_UT_TRACE_ENTER();
    rrm_bool_et ret_val = RRM_FALSE;
    /* SPR 17564 Fix Start */
    if((RRM_TRUE == rrm_is_ue_special_previledge(p_rrm_ue_ctxt)) ||
            /* SPR 17564 Fix End */
            (RRM_TRUE == preemption_capability))
    {
        ret_val = RRM_TRUE;
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/****************************************************************************
 * Function Name  : rrm_ue_max_rab_preemption
 * Inputs         : p_rrm_ue_ctxt
 * 	          : p_rab_removal_ctxt
 * Outputs        : 
 * Returns        : RRM_TURE/RRM_FALSE
 * Description    : It check if the self rab is selcted for deletion or not.
 * 		    There could a situation where the number of Erabs were maximum
 * 		    but one rab is already selected for preeption in previous itertion.
 * 		    In that case max limit check is not required.
 ****************************************************************************/
rrm_return_et 
	rrm_ue_max_rab_preemption
	(
		rrm_ue_context_t *p_rrm_ue_ctxt,
		rrm_ue_erab_setup_item_t *p_erab_to_be_setup,
		rrm_rab_removal_ctxt_t *p_rab_removal_ctxt, 
		U8 type
	)
{
    rrm_return_et ret_val = RRM_SUCCESS;
    U8 preemption_capability = RRM_ZERO;
    rrm_ue_erb_t erb_data;
    rrm_ue_erb_list_t *erb_release_list = RRM_PNULL;
    rrm_bool_et is_low_priority_erb_exist = RRM_TRUE;
    rrm_ue_erb_release_queue_t erb_release_queue;
    rrm_ue_index_t ue_index = RRM_ZERO;
    U8 erb_index = RRM_ZERO;
    U8      current_ue_priority = RRM_UE_ERB_LOWEST_ARP;
    U8   list_index = RRM_ZERO;
    rrm_ue_proc_erab_setup_data_t   *p_rrm_ue_proc_erab_setup_data = RRM_PNULL;
    rrm_ue_erab_setup_item_list_t   *p_erab_setup_item_list = RRM_PNULL;
    rrm_ue_context_t      *p_ue_context     = RRM_PNULL;
    rrm_ue_remove_node_t  *p_ue_rem_node    = RRM_PNULL;
    U8 erb_to_setup_rlc_mode = RRM_ZERO;
    U8 count= RRM_ZERO;
    U8 erb_to_rel_rlc_mode = RRM_ZERO;
    RRM_UT_TRACE_ENTER();

    p_rrm_ue_proc_erab_setup_data = (rrm_ue_proc_erab_setup_data_t *)
        p_rrm_ue_ctxt->current_procedure.p_proc_data;
    RRM_ASSERT(RRM_PNULL != p_rrm_ue_proc_erab_setup_data);
    p_erab_setup_item_list =  &p_rrm_ue_proc_erab_setup_data->erab_setup_list;
    erb_to_setup_rlc_mode = rrm_get_data_transfer_mode_configd(p_erab_to_be_setup->erab_level_qos_params.qci, p_rrm_ue_ctxt->cell_index);
    RRM_MEMSET(&erb_data, RRM_ZERO, sizeof(rrm_ue_erb_t));
    preemption_capability = p_erab_to_be_setup->erab_level_qos_params.
        alloc_and_reten_prior.preemption_capability;

    if( RRM_TRUE == rrm_ue_check_premption_capability(p_rrm_ue_ctxt,
                preemption_capability))
    {

        ylInit(&erb_release_queue);
        /*getting low priority RABs that need to be released to fulfill*/
        /*requirement*/
        if(RRM_SUCCESS == rrm_ue_get_low_priority_gbr_erb(&erb_data, p_rrm_ue_ctxt,
                    p_erab_to_be_setup, type))
        {
            /*Bug_721_changes_start*/
	    /* SPR 19968 Start */
            erb_release_list = (rrm_ue_erb_list_t *)rrm_mem_get(sizeof(rrm_ue_erb_list_t));
	    /* SPR 18868 End */
            if(RRM_PNULL == erb_release_list)
            {
                RRM_TRACE(g_cellm_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,
                        "rrm_ue_apply_arp: Mem allocation failed");
		RRM_UT_TRACE_EXIT();
                return RRM_FAILURE;
            }
            /*Bug_721_changes_end*/

            /* BUG_FIX_837_START */
            RRM_MEMSET(erb_release_list,RRM_ZERO,sizeof(rrm_ue_erb_list_t));
            /* BUG_FIX_837_END */
            for(erb_index = RRM_ZERO; erb_index < 
                    erb_data.p_rrm_ue_context->drb_configured.num_of_list; erb_index++)
            {
                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF,
                        "1. Erab_id [%d], UE Rab Id[%d]", 
                        erb_data.p_rrm_ue_context->drb_configured.erab_item[erb_index].erab_id, 
                        erb_data.erab_id);	
                if(erb_data.p_rrm_ue_context->drb_configured.erab_item[erb_index].erab_id == erb_data.erab_id) 	
                {
                    erb_to_rel_rlc_mode = erb_data.p_rrm_ue_context->drb_configured.erab_item[erb_index].data_transfer_mode;
                    break;
                }
            }

            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                    "Low Priority RAB[%d] found on index[%d]", erb_data.erab_id, erb_index);

            if(erb_to_rel_rlc_mode != erb_to_setup_rlc_mode)
            {
                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_ERROR,
                        "ERAB ID = %d cannot be release",erb_data.erab_id);
		RRM_MEM_FREE(erb_release_list);
		RRM_UT_TRACE_EXIT();
		return ret_val;
            }
            else
            {

		if(RRM_FAILURE == rrm_ue_delete_erb_entity(&erb_data))
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                            "UE_INDEX[%d]: Failure received while deletion from erab priority list",
			    erb_data.p_rrm_ue_context->ue_index);
		    RRM_MEM_FREE(erb_release_list);
		    RRM_UT_TRACE_EXIT();
		    return RRM_FAILURE;
                }
		else
		{
			RRM_MEMCPY(&(erb_release_list->ue_erb), &erb_data, sizeof(rrm_ue_erb_t));
			ylPushTail(&erb_release_queue, &(erb_release_list)->pNode);
			p_g_rrm_cell_ctx->p_rrm_cell_ctx[p_rrm_ue_ctxt->cell_index]->cell_resource_info.used_gbr_limit.used_dl_gbr_limit
				= p_g_rrm_cell_ctx->p_rrm_cell_ctx[p_rrm_ue_ctxt->cell_index]->
				cell_resource_info.used_gbr_limit.used_dl_gbr_limit - 
				p_rrm_ue_ctxt->drb_configured.erab_item[erb_index].erab_level_qos_params.
				gbr_qos_info.erab_guar_bit_rate_dl;
			p_g_rrm_cell_ctx->p_rrm_cell_ctx[p_rrm_ue_ctxt->cell_index]->cell_resource_info.used_gbr_limit.used_ul_gbr_limit
				= p_g_rrm_cell_ctx->p_rrm_cell_ctx[p_rrm_ue_ctxt->cell_index]->
				cell_resource_info.used_gbr_limit.used_ul_gbr_limit - 
				p_rrm_ue_ctxt->drb_configured.erab_item[erb_index].erab_level_qos_params.
				gbr_qos_info.erab_guar_bit_rate_ul;
		}

                if(RRM_TRUE != rrm_is_ue_special_previledge(p_rrm_ue_ctxt))
                {

                    if (RRM_FAILURE == rrm_ue_update_ue_priority_list(p_rrm_ue_ctxt))
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                "UE_INDEX[%d]: Failure received while updating the  \
                                ue_priority", erb_data.p_rrm_ue_context->ue_index);
                    }
                }

                for(count = erb_index; count < p_rrm_ue_ctxt->drb_configured.num_of_list; count++)
                {
                    RRM_MEMCPY(&(p_rrm_ue_ctxt->drb_configured.erab_item[count]),
                            &(p_rrm_ue_ctxt->drb_configured.erab_item[count+1]),
                            sizeof(rrm_ue_erab_setup_item_t));
                }
                p_rrm_ue_ctxt->drb_configured.num_of_list--;
            }
        }
        else
        {
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                    "No low priority GBR ERB exists.");
            is_low_priority_erb_exist = RRM_FALSE;
        }


        if(RRM_TRUE == is_low_priority_erb_exist)
        {
            /*Bug_721_changes_start*/
            while(RRM_PNULL != (erb_release_list = (rrm_ue_erb_list_t *)ylPopTail(&erb_release_queue)))
                /*Bug_721_changes_end*/
            {

                /*spr 7466 start*/
                if(p_rrm_ue_ctxt->ue_index == erb_release_list->ue_erb.p_rrm_ue_context->ue_index)
                {
                    rrm_rab_remove_node_t *p_rab_remove_list = rrm_mem_get(sizeof(rrm_rab_remove_node_t));
                    /* Coverity_Fix 16295 ++ */
                    if ( PNULL != p_rab_remove_list )
                    {
                        RRM_MEMSET(p_rab_remove_list,RRM_ZERO,sizeof(rrm_rab_remove_node_t)); 
                        p_rab_remove_list->erab_id  = erb_release_list->ue_erb.erab_id; 	
                        ylPushTail(&p_rab_removal_ctxt->rabs_to_rem_list,&p_rab_remove_list->pNode);
                        ret_val = RRM_SUCCESS;
                    }
                    else
                    {
                        ret_val = RRM_FAILURE;
                    }
                    /* Coverity_Fix 16295 -- */
                }
                else
                {
                    rrm_ue_rab_remove_record_t *p_ue_rab_release_node = rrm_mem_get(sizeof(rrm_ue_rab_remove_record_t));
                    /* Coverity_Fix 16295 ++ */
                    if ( PNULL != p_ue_rab_release_node)
                    {
                        RRM_MEMSET(p_ue_rab_release_node,RRM_ZERO,sizeof(rrm_ue_rab_remove_record_t));
                        p_ue_rab_release_node->ue_rab_info.ue_idx = erb_release_list->ue_erb.p_rrm_ue_context->ue_index;
                        p_ue_rab_release_node->ue_rab_info.erabs_to_be_released_count = RRM_ONE; 
                        p_ue_rab_release_node->ue_rab_info.erabs_to_be_released[RRM_ZERO] = erb_release_list->ue_erb.erab_id;
                        ylPushTail(&p_rab_removal_ctxt->rab_rem_container.ue_rab_list,&p_ue_rab_release_node->pNode);
                        ret_val = RRM_SUCCESS;
                    }
                    else
                    {
                        ret_val = RRM_FAILURE;
                    }
                    /* Coverity_Fix 16295 -- */
                }
                /*spr 7466 end*/
                /*Bug_721_changes_start*/
                RRM_MEM_FREE(erb_release_list);
                /*Bug_721_changes_end*/
            }
        }
        else
        {
            /*Bug_721_changes_start*/
            /*If erb_release_ind was not sent for any of the RAB and there are any RABs present in erb_release_queue then re-setting is_erb_selected_for_rel to
             * FALSE if any RAB was selected for release*/
            if(ylCount(&erb_release_queue) > RRM_ZERO)
            {
                erb_release_list = (rrm_ue_erb_list_t *)(erb_release_queue.node.next);
                while(RRM_PNULL != erb_release_list)
                {
                    rrm_ue_update_erb_priority_list_flag(erb_release_list);
                    erb_release_list = (rrm_ue_erb_list_t *)erb_release_list->pNode.next;
                }
            }
            /*Bug_721_changes_end*/
            /*If requested RAB is NGBR and no low priority GBR RAB is found*/
            /*then release low priority UE to fulfill the bitrate requirement*/
            if((p_erab_to_be_setup->erab_level_qos_params.qci > RRM_GBR_QCI_4) &&
                    (p_erab_to_be_setup->erab_level_qos_params.qci < RRM_MAX_QCI_LIMIT))
            {
                /*Bug_721_changes_start*/
                /*Computing priority of UE for which RAB admission has come based*/
                /*on already admitted RABs and RABs coming for admission, so that*/
                /*UE released during ARP is of lower priority than current one and*/
                /*released from vulnerable list only*/
                for(list_index = RRM_ZERO; list_index < p_rrm_ue_ctxt->drb_configured.num_of_list;
                        list_index++)
                {
                    if(current_ue_priority > p_rrm_ue_ctxt->drb_configured.erab_item[list_index].
                            erab_level_qos_params.alloc_and_reten_prior.priority_level)
                    {
                        current_ue_priority = p_rrm_ue_ctxt->drb_configured.erab_item[list_index].
                            erab_level_qos_params.alloc_and_reten_prior.priority_level;
                    }
                }

                for(list_index = RRM_ZERO; list_index < p_erab_setup_item_list->num_of_list;
                        list_index++)
                {
                    if(current_ue_priority > p_erab_setup_item_list->erab_item[list_index].
                            erab_level_qos_params.alloc_and_reten_prior.priority_level)
                    {
                        current_ue_priority = p_erab_setup_item_list->erab_item[list_index].
                            erab_level_qos_params.alloc_and_reten_prior.priority_level;
                    }
                }
                /* SPR 17564 Fix Start */
                if(RRM_TRUE == rrm_is_ue_special_previledge(p_rrm_ue_ctxt))
                    /* SPR 17564 Fix End */
                {
/*spr_22158_changes_start*/
                    ret_val = rrm_ue_get_low_priority_ue(p_rrm_ue_ctxt->cell_index, &ue_index,
/*spr_22158_changes_end*/
                            p_rrm_ue_ctxt->ue_index,
                            RRM_ZERO,
                            RRM_TRUE);
                }
                else
                {
/*spr_22158_changes_start*/
                    ret_val = rrm_ue_get_low_priority_ue(p_rrm_ue_ctxt->cell_index, &ue_index,
/*spr_22158_changes_end*/
                            p_rrm_ue_ctxt->ue_index,
                            current_ue_priority,
                            RRM_TRUE);
                }

                /*Bug_721_changes_end*/
                /*Release lowest priority UE */
                if(ret_val == RRM_SUCCESS)
                {
                    p_ue_context = rrm_ue_find_context(ue_index,p_rrm_ue_ctxt->cell_index);
                    if(p_ue_context == RRM_PNULL)
		    {	
			    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
					    "[UE:%d] failed to find ue context",ue_index);
			    RRM_UT_TRACE_EXIT();
			    return RRM_FAILURE;
		    }
		    p_ue_rem_node = rrm_mem_get(sizeof(rrm_ue_remove_node_t));
                    if(RRM_PNULL != p_ue_rem_node)
                    {
                        RRM_MEMSET(p_ue_rem_node,RRM_ZERO,sizeof(rrm_ue_remove_node_t));
                        p_ue_rem_node->ue_idx = ue_index;
                        p_ue_context->adm_ctrl_rec.is_ue_already_selected = RRM_TRUE;
                        ylPushTail(&p_rab_removal_ctxt->rab_rem_container.ue_to_remove_list,&p_ue_rem_node->pNode);
                    }
                    /*Code Deleted*/
                }
            }
            /*Bug_721_changes_start*/
            else
            {
                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,"ARP failed for requested RAB");
                ret_val = RRM_FAILURE;
            }
            /*Bug_721_changes_end*/
        }
        /*Bug_10333_changes_start*/
        while(RRM_PNULL != (erb_release_list = (rrm_ue_erb_list_t *)ylPopTail(&erb_release_queue)))
        {
            RRM_MEM_FREE(erb_release_list);
        }
        /*Bug_10333_changes_end*/

    }
    else
    {
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_WARNING, "RAC failed and requested ERB is not preemption capable.");
        ret_val = RRM_FAILURE;
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;

}




/****************************************************************************
 * Function Name  : rrm_uem_decide_am_um_mode
 * Inputs         : UE context 
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 * Description    : This function allocates the UM/AM mode on the basis of 
 *                  feature group indicator bits
 ****************************************************************************/
rrm_return_et
rrm_uem_decide_am_um_mode(rrm_ue_context_t  *p_rrm_ue_context,
        U8                data_transfer_mode_cnfgd,
        /* warnings removal: 'p_data_transfer_mode' should be
           type U32*, 10/05/2012 */
        U32               *p_data_transfer_mode,
        U32               *p_num_temp_um_mode,
        U32               *p_num_temp_am_mode,
        rrm_ue_erab_setup_item_t  erab_to_be_setup)
{
    rrm_return_et   ret_val = RRM_SUCCESS;
    U32             num_um_mode_drb_configd = RRM_ZERO;
    U32             num_am_mode_drb_configd = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    rrm_find_num_um_am_drb_configd(&num_um_mode_drb_configd,
            &num_am_mode_drb_configd,
            p_rrm_ue_context);

        /* bit20 & bit7, none set to 1 */
        /*Start:Bug 762*/
        if ((rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY) == RRM_FALSE)&&
                (rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_SEVEN) == RRM_FALSE))
            /*End:Bug 762*/
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "1. Already configured am drb= %d", num_am_mode_drb_configd + (*p_num_temp_am_mode));

            if ((num_am_mode_drb_configd + (*p_num_temp_am_mode)) < RRM_FOUR)
            {
                *p_data_transfer_mode = AM_MODE;
                (*p_num_temp_am_mode)++;
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,"ERAB Rejected due to maximum number of AM count reached for bit 20 = FALSE and BIT 7 = FALSE");
                ret_val = RRM_FAILURE;
            }
        }
        /* only bit7 set to 1 */
        /*Start:Bug 762*/
        else if ((rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY) == RRM_FALSE) &&
                (rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_SEVEN) == RRM_TRUE))
            /*End:Bug 762*/
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "1. already configured   am_drb= %d  um_drb = %d",
                    num_am_mode_drb_configd + (*p_num_temp_am_mode),
                    num_um_mode_drb_configd + (*p_num_temp_um_mode));

            if (((num_um_mode_drb_configd + (*p_num_temp_um_mode))+
                        (num_am_mode_drb_configd + (*p_num_temp_am_mode))) < RRM_FIVE)
            {
                if (UM_MODE == data_transfer_mode_cnfgd)
                {
                    if ((num_um_mode_drb_configd + (*p_num_temp_um_mode)) < RRM_ONE)
                    {
                        *p_data_transfer_mode = UM_MODE;
                        (*p_num_temp_um_mode)++;
                    }
                    else if (p_rrm_ue_context->ue_sps_data.sps_erab_id == 
                            erab_to_be_setup.erab_id)
                    {
                        ret_val = RRM_FAILURE;
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,"ERAB Rejected due to maximum number of UM count reached for bit 20 = FALSE and BIT 7 = TRUE ");
                        ret_val = RRM_FAILURE;

                    }
                }
                else/*AM mode*/
                {
                    if((num_am_mode_drb_configd + (*p_num_temp_am_mode)) < RRM_FOUR)    
                    {
                        *p_data_transfer_mode = AM_MODE;
                        (*p_num_temp_am_mode)++;
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,"ERAB Rejected due to maximum number of AM count reached for bit 20 = FALSE and BIT 7 = TRUE ");
                        ret_val = RRM_FAILURE;
                    }
                }
            }
            else
            {
                ret_val = RRM_FAILURE;
            }
        }
        /* Only bit20 set to 1 */
        /*Start:Bug 762*/
        else if ((rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY) == RRM_TRUE) &&
                (rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_SEVEN) == RRM_FALSE))
            /*End:Bug 762*/
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "2. already configured am_drb  = %d",num_am_mode_drb_configd+(*p_num_temp_am_mode));

            if ((num_am_mode_drb_configd + (*p_num_temp_am_mode)) < RRM_EIGHT)
            {
                *p_data_transfer_mode = AM_MODE;
                (*p_num_temp_am_mode)++;
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,"ERAB Rejected due to maximum number of AM count reached for bit 20 = TRUE and BIT 7 = FALSE ");
                ret_val = RRM_FAILURE;
            }
        }
        /* bit 20 & bit 7 both set to 1*/
        /*Start:Bug 762*/
        else if ((rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY) == RRM_TRUE) &&
                (rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_SEVEN) == RRM_TRUE))
            /*End:Bug 762*/
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "3. already configured am_drb= %d,   um_drb= %d",
                    num_am_mode_drb_configd + (*p_num_temp_am_mode),
                    num_um_mode_drb_configd + (*p_num_temp_um_mode));


            if ((num_um_mode_drb_configd+(*p_num_temp_um_mode)) +
                    (num_am_mode_drb_configd+(*p_num_temp_am_mode)) < RRM_EIGHT)
            {
                if (UM_MODE == data_transfer_mode_cnfgd)
                {
                    /* SPR 19968 Start */
                    if (((num_um_mode_drb_configd + (*p_num_temp_um_mode)) < RRM_THREE) && ((num_am_mode_drb_configd + *p_num_temp_am_mode) <= RRM_FIVE))
                    /* SPR 19968 End */
                    {
                        *p_data_transfer_mode = UM_MODE;
                        (*p_num_temp_um_mode)++;
                    }
                    else if (p_rrm_ue_context->ue_sps_data.sps_erab_id ==
                            erab_to_be_setup.erab_id)
                    {
                        ret_val = RRM_FAILURE;
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,"ERAB Rejected due to maximum number of UM count reached for bit 20 = TRUE and BIT 7 = TRUE ");
                        ret_val = RRM_FAILURE;
                    }
                }
                else/*AM mode*/
                {
                    if((num_am_mode_drb_configd + (*p_num_temp_am_mode)) < RRM_FIVE)    
                    {
                        *p_data_transfer_mode = AM_MODE;
                        (*p_num_temp_am_mode)++;
                    }
                    /* SPR 19968 Start */
                    else if (((num_am_mode_drb_configd + (*p_num_temp_am_mode)) >= RRM_FIVE) && (num_um_mode_drb_configd + *p_num_temp_um_mode)==0 )
                    /* SPR 19968 End */
                    {
                        *p_data_transfer_mode = AM_MODE;
                        (*p_num_temp_am_mode)++;
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,"ERAB Rejected due to maximum number of AM count reached for bit 20 = TRUE and BIT 7 = TRUE ");
                        ret_val = RRM_FAILURE;
                    }
                }
            }
            else
            {
                ret_val = RRM_FAILURE;
            }
        }
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
            "1. function exit  configured am drb= %d retval: %d", num_am_mode_drb_configd + (*p_num_temp_am_mode), ret_val);

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/*SPR 19968 End*/

/****************************************************************************
 * Function Name  : rrm_ue_rac_release_sr_res_index
 * Inputs         : UE context pointer
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Releases the reserved SR resources during UE admission
 ****************************************************************************/
rrm_return_et
rrm_ue_rac_release_sr_res_index
(
 rrm_ue_context_t *p_rrm_ue_ctxt
 )
{
    RRM_UT_TRACE_ENTER();
    rrm_return_et ret_val = RRM_SUCCESS;

    RRM_ASSERT(RRM_PNULL != p_rrm_ue_ctxt);

    U8  row_num = p_rrm_ue_ctxt->sr_params.sr_res_row;
    U16 col_num  = p_rrm_ue_ctxt->sr_params.sr_res_col;
    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;
    /*SPR 15713 Fix Start*/
    U8 n_one_pucch_an = RRM_ZERO;
    U8 n1_idx = RRM_ZERO;
    U8 sr_idx = RRM_ZERO;
    rrm_cell_context_t* p_rrm_cell_ctx = RRM_PNULL;
    p_rrm_cell_ctx =  rrm_cellm_get_cell_context(cell_index);
    /*SPR 15713 Fix End*/
    /* Coverity ID 87123 Starts */
    if(RRM_PNULL == p_rrm_cell_ctx)
    {
        RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                "RRM cell context not found.");
        return RRM_FAILURE;
    }
    /* Coverity ID 87123 Ends */

    /* DEBUG_FIX_821_START */
    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_INFO,
             "Releasing SR Resources For [UE:%d], SR Config Index[%d] SR resource Index[%d], Row(%d), Column(%d)",
             p_rrm_ue_ctxt->ue_index,
             p_rrm_ue_ctxt->sr_params.sr_configuration_index,
             p_rrm_ue_ctxt->sr_params.sr_pucch_res_index,
             row_num, col_num);
    /* DEBUG_FIX_821_END */
    if(INVALID_SLOT == row_num || INVALID_FRAME == col_num)
    {
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_WARNING,"Invalid SR res index or slot[%d] or frame[%d].",
        row_num, col_num);
#ifdef RRM_UT_FLAG
        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.delete_all_ue_resp_F);
        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.delete_all_ue_resp_F_inv_sr_res_idx);
#endif        
        ret_val = RRM_FAILURE;
        /*SPR 18531+-*/
        return ret_val;
        /*SPR 18531+-*/
    }
    else
    { 
        /* Coverity 19oct Start : 54929 */
        rrm_cellm_mark_sr_index_free(cell_index, row_num, col_num);
        /* Coverity 19oct End : 54929 */
    }
    /*SPR 15713 Fix Start*/
    n_one_pucch_an = p_rrm_cell_ctx->ran_info.physical_layer_params.physical_layer_param_pucch.sr_n1_pucch_an_last_indx + 1;
    for(n1_idx = RRM_ZERO;n1_idx < n_one_pucch_an;n1_idx++)
    {
        for(sr_idx = RRM_ZERO; sr_idx < p_rrm_cell_ctx->sr_data.allocated_sr_perodicity_max_val;sr_idx++)
        {
            /* SPR 17967 Fix Start */
            if ( RRM_PNULL != p_rrm_cell_ctx->sr_res) 
            { 
                /* SPR 17967 Fix Stop */
                rrm_sr_res_list_t *res_node = &(p_rrm_cell_ctx->sr_res[n1_idx][sr_idx]);
                if(res_node->sr_conflict_resolve == RRM_TRUE)
                {
                    res_node->sr_marked_status = RRM_RES_FREE;
                    res_node->sr_conflict_resolve = RRM_FALSE;
                    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF, 
                            "Releasing SR Resources For [UE:%d], Row(%d), Column(%d)",
                            p_rrm_ue_ctxt->ue_index, n1_idx, sr_idx );
                }/* SPR 17967 Fix Start */
            }/* SPR 17967 Fix Stop */
        }
    }
    /*SPR 15713 Fix End*/

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/****************************************************************************
 * Function Name  : rrm_ue_rac_srb_for_existing_users 
 * Inputs         : UE context
 * Outputs        : None
 * Returns        : RRM_SUCCESS or RRM_FAILURE
 * Description    : Checks if the cell resources are available for new UE 
 *                  admission. This function performs 4 checks. If any of the
 *                  check fails, the resources reserved in the previous check
 *                  are released/roll-backed.
 ****************************************************************************/
rrm_return_et
rrm_ue_rac_srb_for_existing_users
(
 rrm_ue_context_t *p_rrm_ue_ctxt
 )
{
    RRM_UT_TRACE_ENTER();
    rrm_return_et ret_val = RRM_SUCCESS;

    RRM_ASSERT(RRM_PNULL != p_rrm_ue_ctxt);

    rrm_cell_index_t cell_index        = p_rrm_ue_ctxt->cell_index;
    /* SPR 21527 Start */
    U16 num_active_ue     = rrm_cellm_get_num_active_ue(cell_index);
    /* SPR 21527 End */

    /* Next, check the number of active UEs in the cell */
    /*SPR-604 Fix Starts*/
    if((HANDOVER_TYPE_INTRA_CELL != p_rrm_ue_ctxt->ho_type) &&
            (rrm_cellm_get_max_num_ue_per_cell(cell_index) <= num_active_ue))
    /*SPR-604 Fix Ends*/
    {
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED, "Maximum supported number of UEs are \
                already active.");
        ret_val = RRM_FAILURE;
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_get_max_allowed_user 
 * Inputs         : cell index
 * Outputs        : None
 * Returns        : max_num_ue_allowed 
 * Description    : returns max allowed user in a cell 
 ****************************************************************************/
U8
rrm_get_max_allowed_user(
    rrm_cell_index_t cell_index
    )
{
    rrm_cell_context_t*          p_cell_ctxt  = RRM_PNULL;
    /* +- SPR 18268 */
    U16 max_num_ue_allowed = RRM_ZERO;
    /* +- SPR 18268 */
    RRM_UT_TRACE_ENTER();
    p_cell_ctxt =  rrm_cellm_get_cell_context(cell_index);
    RRM_ASSERT(RRM_PNULL != p_cell_ctxt);
    /* Coverity 93942 Fix Start */
    if (RRM_PNULL == p_cell_ctxt)
    {
    	RRM_UT_TRACE_EXIT();
        return max_num_ue_allowed;
    }
    /* Coverity 93942 Fix End */
    if(p_cell_ctxt->operator_info.admission_control_info.bitmask & RRMCM_RMIF_RESOURCE_RESERVED_PRESENT)
    {
        /*considering reserved resource percentage*/
        max_num_ue_allowed = (RRM_CEIL)((float)(rrm_cellm_get_max_num_ue_per_cell(cell_index))
                * ((float)(RRM_PERCENTAGE - rrm_cellm_get_reserved_resource_factor(cell_index))/(float)RRM_PERCENTAGE));
    }
    else
    {
        max_num_ue_allowed = rrm_cellm_get_max_num_ue_per_cell(cell_index);
    }
    RRM_UT_TRACE_EXIT();
    return max_num_ue_allowed;
}
/****************************************************************************
 * Function Name  : rrm_ue_rac_srb_for_new_users
 * Inputs         : UE context
 * Outputs        : None
 * Returns        : RRM_SUCCESS or RRM_FAILURE
 * Description    : Checks if the cell resources are available for new UE
 *                  admission with reserved resources factor
 ****************************************************************************/
rrm_return_et
rrm_ue_rac_srb_for_new_users
(
 rrm_ue_context_t *p_rrm_ue_ctxt
 )
{
    RRM_UT_TRACE_ENTER();
    rrm_return_et ret_val = RRM_SUCCESS;
    rrm_cell_context_t*          p_cell_ctxt  = RRM_PNULL;
    /* +- SPR 18268 */
    U16 max_num_ue_allowed_for_new_users = RRM_ZERO;
    /* +- SPR 18268 */

    RRM_ASSERT(RRM_PNULL != p_rrm_ue_ctxt);

    rrm_cell_index_t cell_index        = p_rrm_ue_ctxt->cell_index;
    p_cell_ctxt =  rrm_cellm_get_cell_context(cell_index);
    /*Coverity 93955 Fix Start*/
    if(RRM_PNULL == p_cell_ctxt) 
    {
        RRM_TRACE( g_uem_log_on_off, p_g_rrm_uem_facility_name,
                RRM_DETAILED,
                " cell index not found ");
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    /*Coverity 93955 Fix End*/
    /* +- SPR 18268 */
    U16 num_active_ue     = rrm_cellm_get_num_active_ue(cell_index);
    /* +- SPR 18268 */

    RRM_ASSERT(RRM_PNULL != p_cell_ctxt);

    if(p_cell_ctxt->operator_info.admission_control_info.bitmask & RRMCM_RMIF_RESOURCE_RESERVED_PRESENT)
    {
        /*considering reserved resource percentage*/
        max_num_ue_allowed_for_new_users = (RRM_FLOOR)((float)(rrm_cellm_get_max_num_ue_per_cell(cell_index))
                * ((float)(RRM_PERCENTAGE - rrm_cellm_get_reserved_resource_factor(cell_index))/(float)RRM_PERCENTAGE));
    }
    else
    {
        max_num_ue_allowed_for_new_users = rrm_cellm_get_max_num_ue_per_cell(cell_index);
    }

    /* Next, check the number of active UEs in the cell */
    if(max_num_ue_allowed_for_new_users <= num_active_ue)
    {
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_INFO,
        "Maximum supported number [%u] of UEs are already active.", max_num_ue_allowed_for_new_users);
        ret_val = RRM_FAILURE;
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_ue_assign_srb_params
 * Inputs         : UE context
 * Outputs        : None
 * Returns        : RRM_SUCCESS or RRM_FAILURE
 * Description    : This function assigns the SR and CQI configuration
 *                  parameters for the UE Admission.
 ****************************************************************************/
rrm_return_et
rrm_ue_assign_srb_params
(
 rrm_ue_context_t *p_rrm_ue_ctxt
 )
{
    RRM_UT_TRACE_ENTER();


    U8            sr_res_index      = INVALID_SR_CFG_INDEX;
    U8            sr_config_index   = INVALID_SR_CFG_INDEX;
    U8            row_num           = INVALID_FRAME;
    U16           col_num           = INVALID_SLOT;

    U16           ri_index          = INVALID_RI_INDEX;
    U8            cqi_res_index     = INVALID_RES_INDEX;
    U16           cqi_config_index  = INVALID_CQI_CFG_INDEX;
    /*SPR 15663 start*/
    rrm_abs_type_et    abs_type = RRM_EICIC_ABS_NULL;
    /*SPR 15663 end*/
    rrm_cqi_reporting_mode_et rrm_enable_cqi_reporting = RRM_DISABLE_CQI_REPORTING;
    rrc_rrm_ue_service_profile_t    *p_ue_service_profile   = RRM_PNULL;
    /*SPR 21090 START*/
    rrm_icic_scheme_type_et         icic_scheme = ICIC_SCHEME_NONE; 
    /*SPR 21090 END*/

#ifdef TDD_MODE_FLAG
    U8                      n_rb_cqi             = RRM_ZERO;
    U8                      cqi_res_last_row_idx = RRM_ZERO;
    U16                     cqi_res_last_col_idx = RRM_ZERO;
#endif 
    p_ue_service_profile = rrm_cellm_get_ue_service_profile(p_rrm_ue_ctxt->cell_index);
    rrm_enable_cqi_reporting = p_ue_service_profile->radio_res_config.
                               physical_config_dedicated.cqi_reporting.
                                                          rrm_cqi_reporting_mode_configured;
    rrm_frequency_selective_sched_et freq_selective_sched = RRM_FREQ_SELECTIVE_SCHEDULING_DISABLE; 
    rrm_cell_context_t *p_cell_ctxt = RRM_PNULL;
    rrm_return_et ret_val = RRM_SUCCESS;

    RRM_ASSERT(RRM_PNULL != p_rrm_ue_ctxt);

    rrm_cell_index_t cell_index        = p_rrm_ue_ctxt->cell_index;

    /* increase the active UE count here. We can decrease the count if 
       further checks fail*/
    rrm_cellm_increment_num_active_ue(cell_index);
    p_cell_ctxt =  rrm_cellm_get_cell_context(p_rrm_ue_ctxt->cell_index);
    /*CID 65846:start*/
    if (RRM_PNULL != p_cell_ctxt )
    {
        freq_selective_sched = p_cell_ctxt->operator_info.rrm_mac_config.
            enable_frequency_selective_scheduling.ul_freq_selective_enable;

        /* Next, check the availability of SR resources */
        /*KPI: pucch resource start*/
        p_cell_ctxt->kpi_stat.kpi.num_pucch_res_alloc_attempts++;
        /*KPI: pucch resource end*/

    /*SPR 21090 START*/
    if((RRMCM_RMIF_DYNAMIC_ICIC_INFO_PRESENT & p_cell_ctxt->operator_info.bitmask) && 
       (RRMCM_RMIF_ICIC_SCHEME_TYPE_PRESENT & p_cell_ctxt->operator_info.dynamic_icic_info.bitmask))
    {
        icic_scheme = p_cell_ctxt->operator_info.dynamic_icic_info.icic_scheme_type;
    }
    /*SPR 21090 END*/

        if( RRM_SUCCESS == rrm_cellm_get_sr_resources(cell_index, 
                    &sr_res_index, &sr_config_index, 
                    /*SPR 15713 Fix Start*/
                    &row_num, &col_num,RRM_FALSE))
                    /*SPR 15713 Fix End*/
        {
            /* Store sr_res_index and sr_config_index in UE context */
            p_rrm_ue_ctxt->sr_params.sr_pucch_res_index     = sr_res_index;
            p_rrm_ue_ctxt->sr_params.sr_configuration_index = sr_config_index;
            p_rrm_ue_ctxt->sr_params.sr_res_row             = row_num;
            p_rrm_ue_ctxt->sr_params.sr_res_col             = col_num;
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_INFO,
                    "sr_res_index(%d), sr_config_index(%d), row_num(%d), col_num(%d)", 
                    sr_res_index,sr_config_index, row_num, col_num);
        }
        else
        {
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_WARNING,
                "SR resources could not be allocated.");

            /* decrease the active UE count */
            rrm_cellm_decrement_num_active_ue(cell_index);

#ifdef RRM_UT_FLAG
            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmRrcDebugStats.ue_admission_resp_F);
            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmRrcDebugStats.ue_admission_resp_F_no_sr_resource);
#endif        
            ret_val = RRM_FAILURE;
           // p_rrm_ue_ctxt->ue_release_cause = RRM_UE_REL_CAUSE_RESOURCE_ALLOCATION_FAILED;
            /*SPR_15323 start*/
            /*Code Deleted*/
            /*SPR_15323 end*/

            /*KPI: sr resource fail*/
            p_cell_ctxt->kpi_stat.kpi.num_of_sr_res_alloc_fail++;
            /*KPI: sr resource fail*/
        }

        if(( RRM_SUCCESS == ret_val) &&
                (RRM_DISABLE_CQI_REPORTING != rrm_enable_cqi_reporting))
        {    

            /* Finally, check the availability of CQI resources */
            row_num             = INVALID_FRAME;
            col_num             = INVALID_SLOT;

            /*SPR 15663 start*/
            if(RRMCM_RMIF_EICIC_INFO_PRESENT & p_cell_ctxt->operator_info.bitmask)
            {
                abs_type = RRM_EICIC_ABS_NOT_PRESENT; 
            }
            if( RRM_SUCCESS == rrm_cellm_get_cqi_resources( cell_index, 
                        &cqi_res_index, &cqi_config_index,
                        &ri_index, sr_config_index, 
     /* SPR14123start*/
/* eICIC_PHASE_1_2_CHANGES_START */
                        &row_num, &col_num, abs_type))
/* eICIC_PHASE_1_2_CHANGES_END*/
     /* SPR14123end*/
                /*SPR 15663 end*/
            {
                /* Store cqi_res_index and cqi_config_index in UE context */
                p_rrm_ue_ctxt->cqi_params.cqi_pucch_res_index  = cqi_res_index;
                p_rrm_ue_ctxt->cqi_params.cqi_pmi_config_index = cqi_config_index;
                p_rrm_ue_ctxt->cqi_params.cqi_res_row          = row_num;
                p_rrm_ue_ctxt->cqi_params.cqi_res_col          = col_num;
                p_rrm_ue_ctxt->cqi_params.ri_index = ri_index;
                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_INFO, 
                        "cqi_pucch_res_index(%d), cqi_config_index(%d),ri_index(%d),row_num(%d), col_num(%d)", 
                        cqi_res_index, cqi_config_index, ri_index, row_num, col_num);
            }
            else
            {
                /*TB_FIX*/
#ifndef TDD_MODE_FLAG
                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_WARNING,"CQI resources could not be allocated.");

                /* cqi resources could not be allocated, so rollback the 
                 * allocation of SR resource
                 * if the following function returns RRM_FAILURE, it indicates 
                 * there is a potential
                 * problem in the system. But we cannot do much at this stage 
                 * because we are already
                 * releasing the resources since we cannot admit the UE. The end 
                 * result is anyway
                 * going to be same - "failed to admit the UE".
                 */
                /*CID 83028 Start*/
                //code removed
                /*CID 83028 End*/
                /* decrement the active UE count */
                rrm_cellm_decrement_num_active_ue(cell_index);
#ifdef RRM_UT_FLAG
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmRrcDebugStats.ue_admission_resp_F);
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmRrcDebugStats.ue_admission_resp_F_no_cqi_resource);
#endif        
                ret_val = RRM_FAILURE;
        //        p_rrm_ue_ctxt->ue_release_cause = RRM_UE_REL_CAUSE_RESOURCE_ALLOCATION_FAILED;
                /*SPR_15323 start*/
                /*Code Deleted*/
                /*SPR_15323 end*/

                /*KPI: sr resource fail*/
                p_cell_ctxt->kpi_stat.kpi.num_of_sr_cqi_alloc_fail++;
                /*KPI: sr resource fail*/
#else
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF, 
                    "Collision Free CQI resource allocation can't be done.Still allocate "
                    "last avaialable CQI resource for successful UE admission. "
                    "Here CQI will be dropped , due to collision with SR");
            n_rb_cqi = p_cell_ctxt->ran_info.physical_layer_params.
                physical_layer_param_pucch.n_rb_cqi;
            if(RRM_ZERO == n_rb_cqi)
            {
                n_rb_cqi = RRM_TWO;
            }
            cqi_res_last_row_idx = (RRM_TWELVE * n_rb_cqi)/
                (p_cell_ctxt->ran_info.physical_layer_params.
                 physical_layer_param_pucch.delta_pucch_shift + RRM_ONE);
            cqi_res_last_col_idx = p_cell_ctxt->allocated_cqi_perodicity_max_val;
            rrm_cqi_ri_res_list_t *res_node = &(p_cell_ctxt->cqi_ri_res[cqi_res_last_row_idx - RRM_ONE][cqi_res_last_col_idx - RRM_ONE]);

            p_rrm_ue_ctxt->cqi_params.cqi_pucch_res_index  = res_node->cqi_pucch_res_index; 
            p_rrm_ue_ctxt->cqi_params.cqi_pmi_config_index = res_node->cqi_config_index;
			/* +SPR 16889 */
            p_rrm_ue_ctxt->cqi_params.cqi_res_row          = cqi_res_last_row_idx - RRM_ONE;
            p_rrm_ue_ctxt->cqi_params.cqi_res_col          = cqi_res_last_col_idx - RRM_ONE;
			/* -SPR 16889 */
            p_rrm_ue_ctxt->cqi_params.ri_index             = res_node->ri_config_index;
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF, 
                    "cqi_pucch_res_index(%d), cqi_config_index(%d),ri_index(%d),row_num(%d), col_num(%d)", 
                    res_node->cqi_pucch_res_index, res_node->cqi_config_index, res_node->ri_config_index,
			/* +SPR 16889 */
                    (cqi_res_last_row_idx - RRM_ONE),(cqi_res_last_col_idx - RRM_ONE));
			/* -SPR 16889 */

#endif    
            }
        }
        /* SRS Start */
        /* Get the SRS resources */
        if((RRM_SUCCESS == ret_val) &&
                (RRM_TRUE == rrm_cellm_get_srs_status(cell_index)))
        {
            /* SRS_FREQ_HOPPING_START */
            if( RRM_FREQ_SELECTIVE_SCHEDULING_ENABLE == freq_selective_sched) 
            {
                /* SPR_8391_CHANGES_START*/
            /*SPR 21090 START*/
                if((RRMCM_RMIF_DYNAMIC_ICIC_INFO_PRESENT & p_cell_ctxt->operator_info.bitmask) && 
                        (RRMCM_RMIF_ICIC_SCHEME_TYPE_PRESENT & p_cell_ctxt->operator_info.dynamic_icic_info.bitmask) &&
                    ((ICIC_SCHEME_FFR == icic_scheme)||(ICIC_SCHEME_SFR == icic_scheme))&&
                        (p_cell_ctxt->operator_info.dynamic_icic_info.bitmask & RRMCM_RMIF_UL_RESOURCE_PARTITION_INFO_PRESENT) &&
                        (p_cell_ctxt->operator_info.dynamic_icic_info.ul_resource_partition_info.num_of_cell_edge_region != RRM_ZERO)
                  )
            /*SPR 21090 END*/
                {
                    if( RRM_UE_POSITION_AT_CELL_EDGE == p_rrm_ue_ctxt->dynamic_icic_info.rrm_ue_dynamic_icic_present_data.ue_present_position)
                    {
                        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_INFO,
                                "Providing sub-band resources, UE is in Cell Edge");
                        /* SPR_8391_CHANGES_END*/
                        if ( RRM_SUCCESS == rrm_cellm_get_subband_srs_resources(cell_index,
                                    &(p_rrm_ue_ctxt->srs_params)))
                        {
                            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                                    "Sub band SRS resources are allocted successfully");
                            RRM_MEMCPY(&(p_rrm_ue_ctxt->srs_params_last_used), &(p_rrm_ue_ctxt->srs_params),
                                    sizeof(rrm_ue_srs_params_t));
                            ret_val = RRM_SUCCESS;
                            /* SPR_8391_CHANGES_START*/
                        }
                        else
                        {
                            ret_val = RRM_FAILURE;
                        }
                    }
                    else
                    {
                        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_INFO,
                                "Providing wide-band resources,UE is in Cell Center");
                        if ( RRM_SUCCESS == rrm_cellm_get_wideband_srs_resources(cell_index,
                                    &(p_rrm_ue_ctxt->srs_params)))
                        {
                            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                                    "Wide band SRS resources are allocted successfully");
                            RRM_MEMCPY(&(p_rrm_ue_ctxt->srs_params_last_used), &(p_rrm_ue_ctxt->srs_params),
                                    sizeof(rrm_ue_srs_params_t));
                            ret_val = RRM_SUCCESS;

                        }
                        else
                        {
                            ret_val = RRM_FAILURE;
                        }
                    }
                }
                else
                {
                    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                            "Providing wide-band resources");
                    if ( RRM_SUCCESS == rrm_cellm_get_wideband_srs_resources(cell_index,
                                &(p_rrm_ue_ctxt->srs_params)))
                    {
                        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                                "Wide band SRS resources are allocted successfully");
                        RRM_MEMCPY(&(p_rrm_ue_ctxt->srs_params_last_used), &(p_rrm_ue_ctxt->srs_params),
                                sizeof(rrm_ue_srs_params_t));
                        ret_val = RRM_SUCCESS;

                    }
                    else
                    {
                        ret_val = RRM_FAILURE;
                    }
                    /* SPR_8391_CHANGES_END*/
                }
            }
            else 
            {
                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                        "Providing wide-band resources");
                if ( RRM_SUCCESS == rrm_cellm_get_wideband_srs_resources(cell_index,
                            &(p_rrm_ue_ctxt->srs_params)))
                {
                    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                            "Wide band SRS resources are allocted successfully");
                    RRM_MEMCPY(&(p_rrm_ue_ctxt->srs_params_last_used), &(p_rrm_ue_ctxt->srs_params),
                            sizeof(rrm_ue_srs_params_t));
                    ret_val = RRM_SUCCESS;
                }
                else
                {
                    ret_val = RRM_FAILURE;
                }
            }
            /*ul dl fss end*/
            if( RRM_FAILURE == ret_val )
            {
                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_WARNING,
                        "SRS resources could not be allocated");

                /* srs resources could not be allocated, so rollback the
                 * allocation of SR/CQI resources
                 * if the following function returns RRM_FAILURE, it indicates
                 * there is a potential
                 * problem in the system. But we cannot do much at this stage
                 * because we are already
                 * releasing the resources since we cannot admit the UE. The end
                 * result is anyway
                 * going to be same - "failed to admit the UE".
                 */                                                                                                                    
                ret_val = rrm_ue_rac_release_sr_res_index(p_rrm_ue_ctxt);
                /* Coverity Fix 83028 Start */
                if(RRM_FAILURE == ret_val)
                {
                    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                            "Failure returned by function rrm_ue_rac_release_sr_res_index");
                }
                /* Coverity Fix 83028 End */
                if(RRM_DISABLE_CQI_REPORTING != rrm_enable_cqi_reporting)
                {
                    ret_val = rrm_ue_rac_release_cqi_res_index(p_rrm_ue_ctxt);
                    /* Coverity ID 75878 Fix Start*/
                    if(RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,"Failed to release CQI resources");
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,"CQI resources released.");
                    }
                    /* Coverity ID 75878 Fix End*/
                }
                /* decrement the active UE count */
                rrm_cellm_decrement_num_active_ue(cell_index);
                ret_val = RRM_FAILURE;
         //       p_rrm_ue_ctxt->ue_release_cause = RRM_UE_REL_CAUSE_RESOURCE_ALLOCATION_FAILED;
                /*SPR_15323 start*/
                /*Code Deleted*/
                /*SPR_15323 end*/
            }
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"p_cell_ctxt is null");
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    /*SPR_15323 start*/
    if(RRM_SUCCESS == ret_val)
    {
        p_rrm_ue_ctxt->srb_allocated = RRM_TRUE;
    }
    /*SPR_15323 end*/ 
    /*CID 65846:end*/
    /* SRS_FREQ_HOPPING_END */

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SRS Start */
/****************************************************************************
 * Function Name  : rrm_ue_rac_release_srs_res_index
 * Inputs         : UE context pointer
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Find the BUSY resources and FREE the resources.
 ****************************************************************************/
rrm_return_et
rrm_ue_rac_release_srs_res_index
(
    rrm_ue_context_t *p_rrm_ue_ctxt
)
{
    U16   srs_anchor = RRM_ZERO;
    rrm_return_et ret_val = RRM_SUCCESS;
    rrm_cell_index_t cell_index = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
    
    RRM_ASSERT(RRM_PNULL != p_rrm_ue_ctxt);
    srs_anchor = p_rrm_ue_ctxt->srs_params.srs_anchor;
    cell_index = p_rrm_ue_ctxt->cell_index;

    /* Coverity 19oct Start : 54929 */
    ret_val = rrm_cellm_mark_srs_index_free(cell_index,srs_anchor);
    /*CID 55370 */
    if(RRM_FAILURE == ret_val)
    {
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_ERROR, 
                "Unable to Release SR Resources For [UE:%d], srs_anchor (%d)",
                p_rrm_ue_ctxt->ue_index, srs_anchor);
    }
    /* Coverity 19oct End : 54929 */

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SRS End */

/****************************************************************************
 * Function Name  : rrm_ue_rac_release_cqi_res_index
 * Inputs         : UE context pointer
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Releases the reserved CQI resources during UE admission
 ****************************************************************************/
rrm_return_et
rrm_ue_rac_release_cqi_res_index
(
 rrm_ue_context_t *p_rrm_ue_ctxt
 )
{
    RRM_UT_TRACE_ENTER();
    rrm_return_et ret_val = RRM_SUCCESS;
    U8 index = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_rrm_ue_ctxt);

    U8  row_num = p_rrm_ue_ctxt->cqi_params.cqi_res_row;
    U16 col_num  = p_rrm_ue_ctxt->cqi_params.cqi_res_col;
    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;

    /* DEBUG_FIX_821_START */
    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_INFO,
             "Releasing CQI Resources For [UE:%d], CQI Config Index[%d], RI Config Index[%d], Resouce Index[%d] Row(%d), Column(%d)",
             p_rrm_ue_ctxt->ue_index,
             p_rrm_ue_ctxt->cqi_params.cqi_pmi_config_index,
             p_rrm_ue_ctxt->cqi_params.ri_index,
             p_rrm_ue_ctxt->cqi_params.cqi_pucch_res_index,
             row_num, col_num);
    /* DEBUG_FIX_821_END */
    if(INVALID_SLOT == col_num  || INVALID_FRAME == row_num)
    {
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_WARNING,"Invalid slot[%d] or frame[%d].", col_num, row_num);
#ifdef RRM_UT_FLAG
        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.delete_all_ue_resp_F);
        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.delete_all_ue_resp_F_inv_cqi_res_idx);
#endif        
        ret_val = RRM_FAILURE;
    }
    else
    {
        /* Coverity 19oct Start : 54928 */
        rrm_cellm_mark_cqi_index_free(cell_index, row_num, col_num);
        /* Coverity 19oct End : 54928 */
    }
    /*CA stage2: start*/
    for (index = RRM_ZERO; index <p_rrm_ue_ctxt->ue_scell_add_params.count;index++)
    {
        if ((INVALID_FRAME != p_rrm_ue_ctxt->ue_scell_add_params.rrm_scell_info_for_ue[index].cqi_params.cqi_res_row )&&
                (INVALID_SLOT != p_rrm_ue_ctxt->ue_scell_add_params.rrm_scell_info_for_ue[index].cqi_params.cqi_res_col))
        {
            rrm_cellm_mark_cqi_index_free(p_rrm_ue_ctxt->cell_index, 
            p_rrm_ue_ctxt->ue_scell_add_params.rrm_scell_info_for_ue[index].cqi_params.cqi_res_row,
            p_rrm_ue_ctxt->ue_scell_add_params.rrm_scell_info_for_ue[index].cqi_params.cqi_res_col);
            
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF, 
             "CA:Releasing CQI Resources For [UE:%d], Row(%d), Column(%d)",
             p_rrm_ue_ctxt->ue_index, 
            p_rrm_ue_ctxt->ue_scell_add_params.rrm_scell_info_for_ue[index].cqi_params.cqi_res_row, 
            p_rrm_ue_ctxt->ue_scell_add_params.rrm_scell_info_for_ue[index].cqi_params.cqi_res_col);
        }
    }
    /*CA stage2: end*/

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/*spr 7466 start*/
/****************************************************************************
 * Function Name  : rrm_ue_rac_clean_cell_prb_resources
 * Inputs         : UE context pointer
 * Outputs        : None
 * Returns        : None
 * Description    : Releases all the prb resources reserved during bearer admission.
 ****************************************************************************/
void
rrm_ue_rac_clean_cell_prb_resources
(
 rrm_ue_context_t *p_rrm_ue_ctxt
 )
{
    RRM_UT_TRACE_ENTER();
    U32 index=RRM_ZERO;
    U8 qci=RRM_ZERO;
    /*SPR 18654 START*/
/*SPR 22151 Fix Start*/
    rrm_prb_t dl_prbs_to_release=RRM_ZERO;
    rrm_prb_t ul_prbs_to_release=RRM_ZERO;
/*SPR 22151 Fix End*/
    /*SPR 18654 END*/
    U64 dl_bitrate=RRM_ZERO;
    U64 ul_bitrate=RRM_ZERO;
    /*SPR 18654 START*/
    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;
    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context (cell_index);
    /* CID 109680 */
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                RRM_ERROR,"Cell Context not found");
        RRM_UT_TRACE_EXIT();
        /* CID 109704 */
        return;
    }
    /*SPR 18654 END*/


/*SPR 22151 Fix Start*/
/*Code deleted*/
/*SPR 22151 Fix End*/
    for(index = RRM_ZERO;index<p_rrm_ue_ctxt->drb_configured.num_of_list;
            index++)
    {
        qci = p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_level_qos_params.qci;
        if(qci >=RRM_GBR_QCI_1 && qci <=RRM_GBR_QCI_4)
        {
            dl_bitrate=p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_dl;
            ul_bitrate=p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_ul;
	    /*SPR 19819 30JUN START*/
	    /* Code deleted */
	    /*SPR 19819 30JUN END*/
            rrm_cellm_set_used_dl_bitrate_during_erb_release(p_rrm_ue_ctxt->cell_index, dl_bitrate);
            rrm_cellm_set_used_ul_bitrate_during_erb_release(p_rrm_ue_ctxt->cell_index, ul_bitrate);
            /*SPR 19968 START*/
            /*Code deleted*/
            /*SPR 22151 Fix Start*/
            /* SPR_12473_FIX_start */
            dl_prbs_to_release = rrm_convert_bitrate_to_prb_dl(p_rrm_ue_ctxt->cell_index, dl_bitrate);
            ul_prbs_to_release = rrm_convert_bitrate_to_prb_ul(p_rrm_ue_ctxt->cell_index, ul_bitrate);
            /*SPR 22151 Fix End*/
                
            /* SPR_12473_FIX_end */
            /*SPR 19968 END*/
            if(RRM_ZERO == dl_prbs_to_release)
            {
                dl_prbs_to_release = RRM_ONE;
            }
            
            if(RRM_ZERO == ul_prbs_to_release)
            {
                ul_prbs_to_release = RRM_ONE;
            }
            /*SPR 19819 30JUN START*/
            /* Code deleted */
            /*SPR 19819 30JUN END*/
            rrm_cellm_reset_dl_allocated_gbr_prb(p_rrm_ue_ctxt->cell_index,dl_prbs_to_release);
            rrm_cellm_reset_ul_allocated_gbr_prb(p_rrm_ue_ctxt->cell_index,ul_prbs_to_release);
        }
    }
    RRM_UT_TRACE_EXIT();
}

/* SPR 10450 Fix Start */
rrm_void_t
rrm_ue_rac_clear_drx_res
( 
    rrm_cell_index_t cell_index,
    U32              applied_drx_profile_index,
    U8               applied_drx_offset_val
)
{
   rrm_cell_context_t*          p_cell_ctxt  = RRM_PNULL;
   U8                           num_of_drx_offset  = RRM_ZERO;         
   U8                           count              = RRM_ZERO;
   rrm_bool_et                  drx_offset_val_found = RRM_FALSE;
   RRM_UT_TRACE_ENTER();

   
   p_cell_ctxt =  rrm_cellm_get_cell_context(cell_index);
   /*CID 65776:start*/
   if (RRM_PNULL != p_cell_ctxt)
   {
       num_of_drx_offset = p_cell_ctxt->rrm_cell_drx_config_resources.drx_res_arr[applied_drx_profile_index].total_drx_offset;

       for(count = RRM_ZERO; count < num_of_drx_offset; count++)
       {
           if(applied_drx_offset_val == p_cell_ctxt->rrm_cell_drx_config_resources.
                   drx_res_arr[applied_drx_profile_index].drx_res_tree[count].drx_start_offset)
           {
               if (RRM_RES_BUSY == p_cell_ctxt->rrm_cell_drx_config_resources.
                       drx_res_arr[applied_drx_profile_index].drx_res_tree[count].res_status)
               {
                   RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                           "DRX_PROFILE_IDX[%d] , DRX Offset Val [%d] found Busy. Mark it free",
                           applied_drx_profile_index,applied_drx_offset_val);
                   p_cell_ctxt->rrm_cell_drx_config_resources.
                       drx_res_arr[applied_drx_profile_index].drx_res_tree[count].res_status = RRM_RES_FREE;
                   drx_offset_val_found = RRM_TRUE; 
                   break;
               }
               else
               {
                   RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                           "DRX_PROFILE_IDX[%d] , DRX Offset Val [%d] is already free",
                           applied_drx_profile_index,applied_drx_offset_val);
               }

           }
       }

       if( RRM_FALSE == drx_offset_val_found)
       {
           RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                   "DRX_PROFILE_IDX[%d] , DRX Offset Val [%d] is NOT present in Drx Res Pool",
                   applied_drx_profile_index,applied_drx_offset_val);
       }
   }
   else
   {
       RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"p_cell_ctxt is null");
   }
   /*CID 65776:send*/

   RRM_UT_TRACE_EXIT();
}
rrm_void_t
rrm_ue_rac_apply_drx_res
( 
    rrm_cell_index_t cell_index,
    U32              applied_drx_profile_index,
    U8               applied_drx_offset_val
)
{
   rrm_cell_context_t*          p_cell_ctxt  = RRM_PNULL;
   U8                           num_of_drx_offset  = RRM_ZERO;         
   U8                           count              = RRM_ZERO;
   rrm_bool_et                  drx_offset_val_found = RRM_FALSE;
   RRM_UT_TRACE_ENTER();

   
   p_cell_ctxt =  rrm_cellm_get_cell_context(cell_index);
   /*CID 65773:start*/
   if (RRM_PNULL != p_cell_ctxt)
   {
       num_of_drx_offset = p_cell_ctxt->rrm_cell_drx_config_resources.drx_res_arr[applied_drx_profile_index].total_drx_offset;

       for(count = RRM_ZERO; count < num_of_drx_offset; count++)
       {
           if(applied_drx_offset_val == p_cell_ctxt->rrm_cell_drx_config_resources.
                   drx_res_arr[applied_drx_profile_index].drx_res_tree[count].drx_start_offset)
           {
               if (RRM_RES_FREE == p_cell_ctxt->rrm_cell_drx_config_resources.
                       drx_res_arr[applied_drx_profile_index].drx_res_tree[count].res_status)
               {
                   RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                           "DRX_PROFILE_IDX[%d] , DRX Offset Val [%d] found Free. Mark it Busy",
                           applied_drx_profile_index,applied_drx_offset_val);
                   p_cell_ctxt->rrm_cell_drx_config_resources.
                       drx_res_arr[applied_drx_profile_index].drx_res_tree[count].res_status = RRM_RES_BUSY;
                   drx_offset_val_found = RRM_TRUE; 
                   break;
               }
               else
               {
                   RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                           "DRX_PROFILE_IDX[%d] , DRX Offset Val [%d] is already BUSY",
                           applied_drx_profile_index,applied_drx_offset_val);
               }

           }
       }

       if( RRM_FALSE == drx_offset_val_found)
       {
           RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                   "DRX_PROFILE_IDX[%d] , DRX Offset Val [%d] is NOT present in Drx Res Pool",
                   applied_drx_profile_index,applied_drx_offset_val);
       }
   }
   else
   {
       RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"p_cell_ctxt is null");
   }

   /*CID 65773:end*/
   RRM_UT_TRACE_EXIT();
}
/* SPR 10450 Fix End */
/*spr 7466 end*/
/****************************************************************************
 * Function Name  : rrm_ue_rac_release_cell_resources
 * Inputs         : UE context pointer
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Releases all the resources reserved during UE admission.
 *                  This function should be called if the UE admission resp or
 *                  UE admission cnf failure is detected. Additionally it could
 *                  be called upon UE release procedure.
 ****************************************************************************/
rrm_return_et
rrm_ue_rac_release_cell_resources
(
 rrm_ue_context_t *p_rrm_ue_ctxt
 )
{
    RRM_UT_TRACE_ENTER();
    U8    index = RRM_ZERO;
    rrm_return_et ret_val = RRM_SUCCESS;
    rrm_cqi_reporting_mode_et rrm_enable_cqi_reporting = RRM_DISABLE_CQI_REPORTING;
    rrc_rrm_ue_service_profile_t    *p_ue_service_profile   = RRM_PNULL;
    rrm_cell_context_t *p_cell_ctx = RRM_PNULL;

    p_ue_service_profile = rrm_cellm_get_ue_service_profile(p_rrm_ue_ctxt->cell_index);
    rrm_enable_cqi_reporting = p_ue_service_profile->radio_res_config.
                               physical_config_dedicated.cqi_reporting.
                                                          rrm_cqi_reporting_mode_configured;
    p_cell_ctx = rrm_cellm_get_cell_context(p_rrm_ue_ctxt->cell_index); 
    RRM_ASSERT(RRM_PNULL != p_rrm_ue_ctxt);

    ret_val = rrm_ue_rac_release_sr_res_index(p_rrm_ue_ctxt);
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,"Failed to release SR resources.");
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,"SR resources released.");
    }

    if(RRM_DISABLE_CQI_REPORTING != rrm_enable_cqi_reporting)
    {
        ret_val = rrm_ue_rac_release_cqi_res_index(p_rrm_ue_ctxt);
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,"Failed to release CQI resources");
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,"CQI resources released.");
        }
    }
    /* SRS Start */
    if(RRM_TRUE == rrm_cellm_get_srs_status(p_rrm_ue_ctxt->cell_index))
    {
        /* Coverity 19oct Start : 54929 */
        rrm_ue_rac_release_srs_res_index(p_rrm_ue_ctxt);
        /* Coverity 19oct End : 54929 */
    }
    /* SRS End */
    /*spr 7466 start*/
    rrm_ue_rac_clean_cell_prb_resources(p_rrm_ue_ctxt);
    /*spr 7466 end*/
    /*  SPS related changes start*/
    if(p_rrm_ue_ctxt->ue_sps_data.is_sps_erab_established == RRM_TRUE)
    {
        rrm_cellm_decrement_num_sps_active_ue(p_rrm_ue_ctxt->cell_index, p_rrm_ue_ctxt->ue_sps_data.pucch_an_list_resource_index);
    }
    /*  SPS related changes end*/
/*delete format 1b*/
    /*CA stage2: start*/
    for (index = RRM_ZERO; index <p_rrm_ue_ctxt->ue_scell_add_params.count;index++)
    {
        if (RRM_PNULL != p_cell_ctx)
        {
            p_cell_ctx->sps_n1_pucch_an_marked_st[p_rrm_ue_ctxt->ue_scell_add_params.rrm_scell_info_for_ue[index].
                ue_format1b_resc.n1_pucch_an_cs_first_index]=RRM_ZERO;
            /*p_cell_ctx->sps_n1_pucch_an_marked_st[p_rrm_ue_ctxt->ue_scell_add_params.rrm_scell_info_for_ue[index].
                ue_format1b_resc.n1_pucch_an_cs_second_index]=RRM_ZERO;*/
            /* CA TDD changes start */
            /* We have to release resources for second PUCCH resource only if
             * resource for n1_pucch_an_cs_second_index was reserved. Earlier no
             * such check was required because we always use to reserve two
             * resources but now in some cases(1 TB case) we will be reserving only one
             * resource.*/
            /* Coverity 69717, 69721 Fix Start */
            if (RRM_OUT_OF_RANGE_TWO_BYTE != 
                    p_rrm_ue_ctxt->ue_scell_add_params.rrm_scell_info_for_ue[index].
                    ue_format1b_resc.n1_pucch_an_cs_second_index)
            /* Coverity 69717, 69721 Fix Start */
            {
                p_cell_ctx->sps_n1_pucch_an_marked_st[p_rrm_ue_ctxt->ue_scell_add_params.
                rrm_scell_info_for_ue[index].ue_format1b_resc.n1_pucch_an_cs_second_index] = RRM_ZERO;
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "Format1b PUCCH resources[%d] and [%d] are free",
                    p_rrm_ue_ctxt->ue_scell_add_params.rrm_scell_info_for_ue[index].ue_format1b_resc.n1_pucch_an_cs_first_index,
                    p_rrm_ue_ctxt->ue_scell_add_params.rrm_scell_info_for_ue[index].ue_format1b_resc.n1_pucch_an_cs_second_index);

            }
            else
            {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "Format1b PUCCH resource[%d] is freed",
                    p_rrm_ue_ctxt->ue_scell_add_params.rrm_scell_info_for_ue[index].ue_format1b_resc.n1_pucch_an_cs_first_index);
            }
            /* CA TDD changes end */
        }
    }
    /*CA stage2: end*/

    rrm_cellm_decrement_num_active_ue(p_rrm_ue_ctxt->cell_index);
    
    /* SPR 10450 Fix Start */
    if(NO_DRX_PROFILE != p_rrm_ue_ctxt->applied_drx_config.curr_applied_drx_config.applied_drx_profile_index) 
    {
        rrm_ue_rac_clear_drx_res(p_rrm_ue_ctxt->cell_index,
                p_rrm_ue_ctxt->applied_drx_config.curr_applied_drx_config.applied_drx_profile_index,
                p_rrm_ue_ctxt->applied_drx_config.curr_applied_drx_config.applied_drx_offset_val
                );
    }
    /* SPR 10450 Fix End */

    /* SPR 10450 Fix Start */
    if(NO_DRX_PROFILE != p_rrm_ue_ctxt->applied_drx_config.curr_applied_drx_config.applied_drx_profile_index) 
    {
        rrm_ue_rac_clear_drx_res(p_rrm_ue_ctxt->cell_index,
                p_rrm_ue_ctxt->applied_drx_config.curr_applied_drx_config.applied_drx_profile_index,
                p_rrm_ue_ctxt->applied_drx_config.curr_applied_drx_config.applied_drx_offset_val
                );
    }
    /* SPR 10450 Fix End */


    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_ue_apply_arp
 * Inputs         : UE context pointer
 *                  required uplink prbs
 *                  required downlink prbs
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Checks if the ERB release indication is sent or not
 ****************************************************************************/

rrm_return_et
rrm_ue_apply_arp
/*SPR 16060 Fix2 Start*/
/*SPR 18654 START*/
(
 rrm_ue_context_t *p_rrm_ue_ctxt,
 U32 ul_required_num_prbs,
 U32 dl_required_num_prbs,
 rrm_ue_erab_setup_item_t erab_to_be_setup,
 U64 ul_allocated_prbs_in_use,
 U64 dl_allocated_prbs_in_use,
 U32 ul_prb_budget,
 U32 dl_prb_budget,
 /*spr 17437 fix start*/
 U8     type,
 rrm_rab_removal_ctxt_t          *p_rab_removal_ctxt
 /*spr 17437 fix stop*/
 )
/*SPR 18654 END*/
/*SPR 16060 Fix2 End*/
{
    rrm_return_et ret_val = RRM_FAILURE;
    U8 preemption_capability = RRM_ZERO;
    U64 deallocated_dl_prbs = RRM_ZERO;
    U64 deallocated_ul_prbs = RRM_ZERO;
    /*SPR 22151 Fix Start*/
    /*SPR 18654 START*/
    rrm_prb_t released_dl_prbs = RRM_ZERO;
    rrm_prb_t released_ul_prbs = RRM_ZERO;
    /*SPR 22151 Fix End*/
    /*SPR 18654 END*/
    rrm_ue_erb_t erb_data;
    /*spr 17437 fix start*/
      /*Code Deleted*/
    /*spr 17437 fix stop*/
    rrm_ue_erb_list_t *erb_release_list = RRM_PNULL;
    rrm_bool_et is_low_priority_erb_exist = RRM_TRUE;
    rrm_ue_erb_release_queue_t erb_release_queue;
    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;
    /*spr 17437 fix start*/
      /*Code Deleted*/
    /*spr 17437 fix stop*/
    rrm_ue_index_t ue_index = RRM_ZERO;
    /**bug id 268 start **/
    /*spr 17437 fix start*/
      /*Code Deleted*/
    /*spr 17437 fix stop*/
    /**bug id 268 end **/
    /*Bug_721_changes_start*/
    U8      current_ue_priority = RRM_UE_ERB_LOWEST_ARP;
    U8   list_index = RRM_ZERO;
    rrm_ue_proc_erab_setup_data_t   *p_rrm_ue_proc_erab_setup_data = RRM_PNULL;
    rrm_ue_erab_setup_item_list_t   *p_erab_setup_item_list = RRM_PNULL;
    /*Bug_721_changes_end*/
    /*spr 7466 start*/
    /*spr 17437 fix start*/
    /* SPR-18198 START */
    /*code deleted*/
    /* SPR-18198 END */
    rrm_ue_remove_node_t  *p_ue_rem_node    = RRM_PNULL;
    /*spr 17437 fix stop*/
    /*spr 7466 end*/

    /*SPR 16060 Fix Start*/
/* SPR-18290 START */
/* code deleted */
/* SPR-18290 END */
    rrm_bool_et ues_to_be_rlsd = RRM_FALSE;
    rrm_bool_et is_emergency_ue = RRM_FALSE;
    U8 index=RRM_ZERO;
    /* SPR 20652 Fix Start */
    /* Code Removed */
    rrm_ue_context_t       *p_ue_context = RRM_PNULL;
    /* SPR 20652 Fix End */
    /*SPR 16060 Fix End*/
    /*SPR 16060 Fix2 Start*/
    /*SPR 18654 START*/
    U64 dl_current_allocated_prbs = dl_allocated_prbs_in_use;
    U64 ul_current_allocated_prbs = ul_allocated_prbs_in_use ;
    /*SPR 18654 END*/
    /*SPR 16060 Fix2 End*/

    /* SPR-18198 START */
    /*SPR 18654 START*/
    U32 ul_prb_remaining = RRM_ZERO;
    U32 dl_prb_remaining = RRM_ZERO;
    /*SPR 18654 END*/
    /* SPR-18198 END */

    RRM_UT_TRACE_ENTER();
    /*SPR 18654 START*/
    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context (cell_index);
    /* 109675 */
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE( g_uem_log_on_off, p_g_rrm_uem_facility_name,
                RRM_DETAILED,
                " cell index not found ");
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    U16   rb_factor_for_bitrate = p_cell_context->operator_info.admission_control_info.rb_factor_for_bitrate;
    /*SPR 18654 END*/
    /*Bug_721_changes_start*/
    p_rrm_ue_proc_erab_setup_data = (rrm_ue_proc_erab_setup_data_t *)
        p_rrm_ue_ctxt->current_procedure.p_proc_data;
    RRM_ASSERT(RRM_PNULL != p_rrm_ue_proc_erab_setup_data);
    p_erab_setup_item_list =  &p_rrm_ue_proc_erab_setup_data->erab_setup_list;
    RRM_ASSERT(RRM_PNULL != p_erab_setup_item_list);
    /*Bug_721_changes_end*/

    RRM_MEMSET(&erb_data, RRM_ZERO, sizeof(rrm_ue_erb_t));

    preemption_capability = erab_to_be_setup.erab_level_qos_params.
        alloc_and_reten_prior.preemption_capability;

    /*check if requested RAB is preemption capable or not*/
    if( RRM_TRUE == rrm_ue_check_premption_capability(p_rrm_ue_ctxt,
                preemption_capability))
    {
        /* SPR-18198 START */
        if(ul_prb_budget >= ul_allocated_prbs_in_use)
        {
            ul_prb_remaining = ul_prb_budget - ul_allocated_prbs_in_use;
        }
        if(dl_prb_budget >= dl_allocated_prbs_in_use)
        {
            dl_prb_remaining = dl_prb_budget - dl_allocated_prbs_in_use;
        }
        /* SPR-18198 END */
        ylInit(&erb_release_queue);
        do {
            /*getting low priority GBR RABs that need to be released to fulfill*/
            /*requirement*/
            if(RRM_SUCCESS == rrm_ue_get_low_priority_gbr_erb(&erb_data, p_rrm_ue_ctxt,
            /*spr 17437 fix start*/
                        &erab_to_be_setup,type))
            /*spr 17437 fix stop*/
            {
                /*Bug_721_changes_start*/
                erb_release_list = rrm_mem_get(sizeof(rrm_ue_erb_list_t));

                if(RRM_PNULL == erb_release_list)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                            "rrm_ue_apply_arp: Mem allocation failed");
                    /* SPR-18198 START */
                    RRM_UT_TRACE_EXIT();
                    /* SPR-18198 END */
                    return RRM_FAILURE;
                }
                /*Bug_721_changes_end*/
                /*LTE_RRM_KLOCWORK_WARN_6_JULY_12_START*/
                RRM_MEMCPY(&(erb_release_list->ue_erb), &erb_data, sizeof(rrm_ue_erb_t));
                /*LTE_RRM_KLOCWORK_WARN_6_JULY_12_END*/
                ylPushTail(&erb_release_queue, &(erb_release_list)->pNode);

                /*Bug_721_changes_start*/
                U8  erb_index = rrm_ue_find_erb_index(&erb_data);
                /*SPR 18654 START*/
                /* SPR_12473_FIX_start */
                /*SPR 22151 Fix Start*/
                /*SPR 20782 Fix Start*/
		/*SPR 22754 Fix Start*/
		/* CID 121170 fix Start*/
                released_dl_prbs = rrm_convert_bitrate_to_prb_dl(cell_index,(erb_data.p_rrm_ue_context->drb_configured.erab_item[erb_index].erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_dl));
		/* CID 121170 fix End*/
                /*SPR 20782 Fix Stop*/

                deallocated_dl_prbs += released_dl_prbs;

                /*SPR 20782 Fix Start*/
		/* CID 121170 fix Start*/
                released_ul_prbs = rrm_convert_bitrate_to_prb_ul(cell_index,(erb_data.p_rrm_ue_context->drb_configured.erab_item[erb_index].erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_ul));
		/* CID 121170 fix End*/
		/*SPR 22754 Fix End*/
                 /*SPR 20782 Fix Stop*/
                /*SPR 22151 Fix End*/
                /* SPR 19968 Start */
                deallocated_ul_prbs += released_ul_prbs;    
                /* SPR 19968 End */
                /* SPR_12473_FIX_end */
                /*Bug_721_changes_end*/
                /*SPR 18654 END*/
            }
            else
            {
                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                        "No low priority ERB exists.");
                is_low_priority_erb_exist = RRM_FALSE;
                break;
            }
        /* SPR-18198 START */
        }while(((ul_prb_remaining + deallocated_ul_prbs) < ul_required_num_prbs) &&
                ((dl_prb_remaining + deallocated_dl_prbs) < dl_required_num_prbs));
        /* SPR-18198 END */

        if(RRM_TRUE == is_low_priority_erb_exist)
        {
            /*Bug_721_changes_start*/
            while(RRM_PNULL != (erb_release_list = (rrm_ue_erb_list_t *)ylPopTail(&erb_release_queue)))
                /*Bug_721_changes_end*/
            {
        /*spr 17437 fix start*/
        /*Code Deleted*/
        /*spr 17437 fix stop*/

                /*spr 7466 start*/
        /*spr 17437 fix start*/
                if(p_rrm_ue_ctxt->ue_index == erb_release_list->ue_erb.p_rrm_ue_context->ue_index)
                {
                    rrm_rab_remove_node_t *p_rab_remove_list = rrm_mem_get(sizeof(rrm_rab_remove_node_t));
                    /* Coverity_Fix 16294 ++ */
                    if ( PNULL != p_rab_remove_list )
                    {
                        RRM_MEMSET(p_rab_remove_list,RRM_ZERO,sizeof(rrm_rab_remove_node_t));
                        p_rab_remove_list->erab_id  = erb_release_list->ue_erb.erab_id;
                        /* CID 96500 */
                        ylPushTail(&p_rab_removal_ctxt->rabs_to_rem_list,(YLNODE *)p_rab_remove_list);
                        /* CID 96497 */
                        ret_val = RRM_SUCCESS;
                    }
                    else
                    {
                        ret_val = RRM_FAILURE;
                    }
                    /* Coverity_Fix 16294 -- */

                    /*spr 17437 fix stop*/
                }
                else
                {
                    /*spr 17437 fix start*/
                    rrm_ue_rab_remove_record_t *p_ue_rab_release_node = rrm_mem_get(sizeof(rrm_ue_rab_remove_record_t));
                    /* Coverity_Fix 16294 ++ */
                    if ( PNULL != p_ue_rab_release_node )
                    {
                        RRM_MEMSET(p_ue_rab_release_node,RRM_ZERO,sizeof(rrm_ue_rab_remove_record_t));
                        p_ue_rab_release_node->ue_rab_info.ue_idx = erb_release_list->ue_erb.p_rrm_ue_context->ue_index;
                        p_ue_rab_release_node->ue_rab_info.erabs_to_be_released_count = RRM_ONE;
                        p_ue_rab_release_node->ue_rab_info.erabs_to_be_released[RRM_ZERO] = erb_release_list->ue_erb.erab_id;
                        /* CID 96500 */
                        ylPushTail(&p_rab_removal_ctxt->rab_rem_container.ue_rab_list,(YLNODE *)p_ue_rab_release_node);
                        /* CID 96497 */
                        /* SPR-18301 START */
                        ret_val = RRM_SUCCESS;
                        /* SPR-18301 END */
                    }
                    else
                    {
                        ret_val = RRM_FAILURE;
                    }
                    /* Coverity_Fix 16294 -- */
                    /*spr 17437 fix stop*/
                }
                /*spr 17437 fix start*/
                /*Code Deleted*/
                /*spr 17437 fix stop*/
                /*spr 7466 end*/
                /*Bug_721_changes_start*/
                RRM_MEM_FREE(erb_release_list);
                /*Bug_721_changes_end*/
            }
        }
        else
        {
            /*Bug_721_changes_start*/
            /*If erb_release_ind was not sent for any of the RAB and there are any RABs present in erb_release_queue then re-setting        is_erb_selected_for_rel to
             * FALSE if any RAB was selected for release*/
            if(ylCount(&erb_release_queue) > RRM_ZERO)
            {
                /* SPR-18198 START */
                erb_release_list = (rrm_ue_erb_list_t *)ylFirst(&erb_release_queue);
                while(RRM_PNULL != erb_release_list)
                {
                    rrm_ue_update_erb_priority_list_flag(erb_release_list);
                    erb_release_list = (rrm_ue_erb_list_t *)ylNext(&erb_release_list->pNode);
                }
                /* SPR-18198 END */
            }

            /*Bug_721_changes_end*/
            /*If requested RAB is NGBR and no low priority GBR RAB is found*/
            /*then release low priority UE to fulfill the bitrate requirement*/
            if((erab_to_be_setup.erab_level_qos_params.qci > RRM_GBR_QCI_4) &&
                    (erab_to_be_setup.erab_level_qos_params.qci < RRM_MAX_QCI_LIMIT))
            {
                /*Bug_721_changes_start*/
                /*Computing priority of UE for which RAB admission has come based */
                /*on already admitted RABs and RABs coming for admission, so that UE*/
                /*released during ARP is of lower priority than current one and released from vulnerable list only*/
                for(list_index = RRM_ZERO; list_index < p_rrm_ue_ctxt->drb_configured.num_of_list;
                        list_index++)
                {
                    if(current_ue_priority >
                            p_rrm_ue_ctxt->drb_configured.erab_item[list_index].erab_level_qos_params.
                            alloc_and_reten_prior.priority_level)
                    {
                        current_ue_priority = p_rrm_ue_ctxt->drb_configured.
                            erab_item[list_index].erab_level_qos_params.
                            alloc_and_reten_prior.priority_level;
                    }
                }

                for(list_index = RRM_ZERO; list_index < p_erab_setup_item_list->num_of_list;
                        list_index++)
                {
                    if(current_ue_priority > p_erab_setup_item_list->erab_item[list_index].
                            erab_level_qos_params.alloc_and_reten_prior.priority_level)
                    {
                        current_ue_priority = p_erab_setup_item_list->erab_item[list_index].
                            erab_level_qos_params.alloc_and_reten_prior.priority_level;
                    }
                }

                /*SPR 16060 Fix Start*/
                deallocated_dl_prbs = RRM_ZERO;
                deallocated_ul_prbs = RRM_ZERO;
                index = RRM_ZERO;
/* SPR-18290 START */
/* code deleted */
/* SPR-18290 END */

        /* SPR 17564 Fix Start */
        if(RRM_TRUE == rrm_is_ue_special_previledge(p_rrm_ue_ctxt))
            /* SPR 17564 Fix End */
        {
            is_emergency_ue = RRM_TRUE;
        }

        do{
            if(RRM_TRUE == is_emergency_ue)
                {
/* SPR-18290 START */
/*spr_22158_changes_start*/
                    ret_val = rrm_ue_get_low_priority_ue(p_rrm_ue_ctxt->cell_index, &ue_index,
/*spr_22158_changes_end*/
                        p_rrm_ue_ctxt->ue_index,
                            RRM_ZERO,
                            RRM_TRUE);
                }
                else
                {
/*spr_22158_changes_start*/
                    ret_val = rrm_ue_get_low_priority_ue(p_rrm_ue_ctxt->cell_index, &ue_index,
/*spr_22158_changes_end*/
                        p_rrm_ue_ctxt->ue_index,
                            current_ue_priority,
                            RRM_TRUE);
                }
/* SPR-18290 END */

            if(RRM_SUCCESS == ret_val)
            {
                        /** Find the UE context **/
                /* SPR 20652 Fix Start */
                p_ue_context = rrm_ue_find_context(ue_index,
                        cell_index);
                /* SPR-18290 START */
                if (RRM_PNULL == p_ue_context)
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                            "[UE:%d]CELL_INDEX[%d] failed to find ue context",ue_index,cell_index);
                    ret_val = RRM_FAILURE;
                    break;
                }
                if(p_rrm_ue_ctxt->ue_index ==  ue_index)
                {
                    continue;
                }
                deallocated_dl_prbs = RRM_ZERO;
                deallocated_ul_prbs = RRM_ZERO;
                /* SPR-18290 END */
                rrm_ue_calc_total_prb_to_be_rlsd(p_ue_context,
                        &deallocated_dl_prbs,
                        &deallocated_ul_prbs);
                /*SPR 18654 START*/
                deallocated_dl_prbs = (deallocated_dl_prbs * rb_factor_for_bitrate);
                deallocated_ul_prbs = (deallocated_ul_prbs * rb_factor_for_bitrate);
                /*SPR 18654 END*/
                /* SPR-18290 START */
                /* code deleted */
                /* SPR-18290 END */
                /* SPR-18198 START */
                if(p_rrm_ue_ctxt->ue_index !=  ue_index)
                {
                    index++;
                    p_ue_rem_node = rrm_mem_get(sizeof(rrm_ue_remove_node_t));
                    if(RRM_PNULL != p_ue_rem_node)
                    {
                        RRM_MEMSET(p_ue_rem_node,RRM_ZERO,sizeof(rrm_ue_remove_node_t));
                        p_ue_rem_node->ue_idx = ue_index;
                        p_ue_context->adm_ctrl_rec.is_ue_already_selected = RRM_TRUE;
                        /* SPR 20652 Fix End */
                        /* CID 96500 */
                        ylPushTail(&p_rab_removal_ctxt->rab_rem_container.ue_to_remove_list,(YLNODE *)p_ue_rem_node);
                        /*Coverity 96501 start*/
                        /*Code Deleted*/
                        /*Coverity 96501 stop*/
                    }

                }
                /* SPR-18198 END */

                /*SPR 16060 Fix2 Start*/
                /* Bug-3728 Fix START */
                /* SPR-18198 START */

                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                " dl_prb_budget =%d\t dl_current_allocated_prbs =%d\t deallocated_dl_prbs =%llu\t dl_required_num_prbs      =%d\n"
                                " ul_prb_budget =%d\t ul_current_allocated_prbs =%d\t deallocated_ul_prbs =%llu\t ul_required_num_prbs      =%d\n",
                                dl_prb_budget, dl_current_allocated_prbs, deallocated_dl_prbs, dl_required_num_prbs,
                                ul_prb_budget, ul_current_allocated_prbs, deallocated_ul_prbs, ul_required_num_prbs);

                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                " ul_prb_budget =%d\t ul_current_allocated_prbs =%d\t deallocated_ul_prbs =%llu\t ul_required_num_prbs      =%d\n", ul_prb_budget,
                                ul_current_allocated_prbs, deallocated_ul_prbs, ul_required_num_prbs);
                        /* SPR-18198 END */
                        /*SPR 18654 START*/
                        if(((dl_prb_remaining + deallocated_dl_prbs) >= dl_required_num_prbs)
                                && ((ul_prb_remaining + deallocated_ul_prbs) >= ul_required_num_prbs))
                        {
                            ues_to_be_rlsd = RRM_TRUE;
                        }
                        /*SPR 18654 END*/
                        /* code removed */
                        /* SPR-18198 START */
                        if(dl_current_allocated_prbs >= deallocated_dl_prbs)
                        {
                            dl_current_allocated_prbs = dl_current_allocated_prbs - deallocated_dl_prbs;
                        }
                        else
                        {
                            dl_current_allocated_prbs = RRM_ZERO;
                        }
                        if(ul_current_allocated_prbs >= deallocated_ul_prbs)
                        {
                            ul_current_allocated_prbs = ul_current_allocated_prbs - deallocated_ul_prbs;
                        }
                        else
                        {
                            ul_current_allocated_prbs = RRM_ZERO;
                        }
                        /* SPR-18198 END */

            }
        }while((RRM_SUCCESS == ret_val) && (ues_to_be_rlsd != RRM_TRUE));
        /*SPR 16060 Fix2 End*/
        /*SPR 16060 Fix End*/
            }
            /*Bug_721_changes_start*/
            else
            {

                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,"ARP failed for requested RAB");
            }
            /*Bug_721_changes_end*/
        }
        /*Bug_10333_changes_start*/
        while(RRM_PNULL != (erb_release_list = (rrm_ue_erb_list_t *)ylPopTail(&erb_release_queue)))
        {

            rrm_mem_free(erb_release_list);
        }
        /*Bug_10333_changes_end*/

    }
    else
    {
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_WARNING, "RAC failed and requested ERB is not preemption capable");
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;

}

/****************************************************************************
 * Function Name  : rac_admit_gbr_bearer
 * Inputs         : UE context pointer
 *                  required uplink bitrate
 *                  required downlink bitrate
                    erab_to_be_setup
                    ngbr_dl_prbs_released
                    ngbr_ul_prbs_released
                    gbr_dl_prbs_released
                    gbr_ul_prbs_released
                    type
                    gbr_configd_count
                    gbr_success_count
                    p_rab_removal_ctxt
*
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Checks if the GBR bearer could be admitted or not. It uses
 *                  the input bitrates and the current bandwidth utilization
 *                  for checking.
 ****************************************************************************/
/* SPR-15991_2 START */
rrm_return_et       /*13581 rel1.3*/
    rac_admit_gbr_bearer
(
 rrm_ue_context_t *p_rrm_ue_ctxt,
 U64              ul_bitrate,
 U64              dl_bitrate,
 rrm_ue_erab_setup_item_t erab_to_be_setup,
/*SPR 18654 START*/
 U64       ngbr_dl_prbs_released,
 U64 ngbr_ul_prbs_released,
 U64 gbr_dl_prbs_released,
 U64 gbr_ul_prbs_released,
/*SPR 18654 START*/
/*spr 17437 fix start*/
 U8               type,
 U8               gbr_configd_count,
 U8               gbr_success_count,
 rrm_rab_removal_ctxt_t          *p_rab_removal_ctxt
/*spr 17437 fix stop*/
#ifdef ENDC_ENABLED
 ,rrm_bool_et is_endc_ue_x2_up
#endif
 )
{
    rrm_return_et ret_val = RRM_SUCCESS;
    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;
    /*SPR 22151 Fix Start*/
    /*SPR 18654 START*/
    rrm_prb_t dl_required_num_prbs = RRM_ZERO;
    rrm_prb_t ul_required_num_prbs = RRM_ZERO;
    /*SPR 18654 END*/
    /*SPR 22151 Fix End*/
    /* SPR 21367 Fix Start */
    U64 ul_gbr_bitrate = RRM_ZERO;
    U64 dl_gbr_bitrate = RRM_ZERO;
    /* SPR 21367 Fix End */

    /*SPR_15022_Start*/
    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context (cell_index);
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE( g_uem_log_on_off, p_g_rrm_uem_facility_name,
                RRM_DETAILED,
                " cell index not found ");
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    
    /*SPR 22151 Fix Start*/
    /*SPR 18654 START*/
    rrm_prb_t dl_gbr_prbs_in_use_count = RRM_ZERO;
    rrm_prb_t ul_gbr_prbs_in_use_count = RRM_ZERO;
    rrm_prb_t dl_ngbr_prbs_in_use_count = RRM_ZERO;
    rrm_prb_t ul_ngbr_prbs_in_use_count = RRM_ZERO;
    /*SPR 18654 END*/
    /*SPR 22151 Fix End*/
    /*SPR_15022_End*/

    U64 curr_gbr_dl_bitrate = RRM_ZERO;
    U64 curr_gbr_ul_bitrate = RRM_ZERO;
    U64 prev_gbr_dl_bitrate = RRM_ZERO;
    U64 prev_gbr_ul_bitrate = RRM_ZERO;
    U8 old_qci = RRM_ZERO;
    /* SPR-15991_2 END */

    /* SPR-15991_3_5NOV Start */
    /*SPR 18654 START*/
    U64 total_prb_allocated_ul = RRM_ZERO;
    U64 total_prb_allocated_dl = RRM_ZERO;
    U64 total_prb_released_ul = RRM_ZERO;
    U64 total_prb_released_dl = RRM_ZERO;
    U64 dl_allocated_gbr_ngbr_prbs_temp =  RRM_ZERO;
    U64 ul_allocated_gbr_ngbr_prbs_temp =  RRM_ZERO;
    /*SPR 18654 END*/
    /* SPR-15991_3 End */
    /*SPR 18602 START*/
    U64             total_backhaul_capacity = RRM_NULL;
    U8              capacity_threshold  = RRM_NULL;
    U64             admitted_bit_rate   = RRM_NULL;
    U64             bitrate_required    = RRM_NULL;
    U64             add_bitrate = RRM_ZERO;
    U64             sub_bitrate = RRM_ZERO;
    U64             max_prev =  RRM_ZERO;
    U64             max_curr =  RRM_ZERO;
    /*SPR 18602 END*/
    /*SPR 18654 START*/
    U16   rb_factor_for_bitrate = p_cell_context->operator_info.admission_control_info.rb_factor_for_bitrate;
    /** Get the required downlink measurements from CellM */
    /* SPR 14509 start */
    U32 dl_gbr_prbs_in_use                  = RRM_ZERO;
    U32 dl_ngbr_prbs_in_use                 = RRM_ZERO;
    /* SPR 14509 end */
    U64 dl_allocated_gbr_prbs               =
        rrm_cellm_get_dl_allocated_gbr_prb(cell_index);
    U64 dl_allocated_ngbr_prbs              =
        rrm_cellm_get_dl_allocated_ngbr_prb(cell_index);
    U32 dl_prb_budget_for_gbr_ngbr_combined =
        (rrm_cellm_get_dl_prb_budget_combined(cell_index)*rb_factor_for_bitrate);
    U32 dl_prb_budget_gbr                   =
        (rrm_cellm_get_dl_prb_budget_gbr(cell_index)*rb_factor_for_bitrate);
    U32 dl_usable_bw_prb_for_bearers        =
        (rrm_cellm_get_dl_usable_bw_prb_for_bearers(cell_index)*rb_factor_for_bitrate);

    /** Get the required uplink measurements from CellM */
    U32 ul_gbr_prbs_in_use                  =
        (rrm_cellm_get_ul_gbr_prb_in_use(cell_index)*rb_factor_for_bitrate);
    U32 ul_ngbr_prbs_in_use                 =
        (rrm_cellm_get_ul_ngbr_prb_in_use(cell_index)*rb_factor_for_bitrate);
    U64 ul_allocated_gbr_prbs               =
        rrm_cellm_get_ul_allocated_gbr_prb(cell_index);
    U64 ul_allocated_ngbr_prbs              =
        rrm_cellm_get_ul_allocated_ngbr_prb(cell_index);
    U32 ul_prb_budget_for_gbr_ngbr_combined =
        (rrm_cellm_get_ul_prb_budget_combined(cell_index)*rb_factor_for_bitrate);
    U32 ul_prb_budget_gbr                   =
        (rrm_cellm_get_ul_prb_budget_gbr(cell_index)*rb_factor_for_bitrate);
    U32 ul_usable_bw_prb_for_bearers        =
        (rrm_cellm_get_ul_usable_bw_prb_for_bearers(cell_index)*rb_factor_for_bitrate);
    /* SPR 14509 start */
    if((RRM_OAM_ACT_AS_VICTIM == p_cell_context->operator_info.eicic_info.eicic_provisioned_type)
            && (RRM_UE_VICTIM_STATE == p_rrm_ue_ctxt->rrm_ue_eicic_info.victim_ue))
    {
    /** Get the required downlink measurements from CellM */
        /*SPR 18654 START*/
        dl_gbr_prbs_in_use =
            (rrm_cellm_get_dl_gbr_abs_in_use(cell_index) * rb_factor_for_bitrate);
        dl_ngbr_prbs_in_use =
            (rrm_cellm_get_dl_ngbr_abs_in_use(cell_index) * rb_factor_for_bitrate);
        /*SPR 18654 END*/
    }
    else
    {
        dl_gbr_prbs_in_use = (rrm_cellm_get_dl_gbr_prb_in_use(cell_index)* rb_factor_for_bitrate);
        dl_ngbr_prbs_in_use = (rrm_cellm_get_dl_ngbr_prb_in_use(cell_index)*rb_factor_for_bitrate);
    }
    /* SPR 14509 end */
    /*SPR 18654 END*/
    /* SPR-15991_2 START */
    /* SPR-15991_3_5NOV Start */
    total_prb_allocated_ul = ul_allocated_ngbr_prbs + ul_allocated_gbr_prbs;
    total_prb_allocated_dl = dl_allocated_ngbr_prbs + dl_allocated_gbr_prbs;
    total_prb_released_ul = ngbr_ul_prbs_released + gbr_ul_prbs_released;
    total_prb_released_dl = ngbr_dl_prbs_released + gbr_dl_prbs_released;
    /*SPR 18654 START*/
    U8 erab_id = erab_to_be_setup.erab_id;
    U64  total_prb_released_ul_factored = (total_prb_released_ul);
    U64  total_prb_released_dl_factored = (total_prb_released_dl);
    if (total_prb_allocated_ul >= total_prb_released_ul_factored)
    {
        ul_allocated_gbr_ngbr_prbs_temp = total_prb_allocated_ul - total_prb_released_ul_factored;
    }
    else
    {
        ul_allocated_gbr_ngbr_prbs_temp = RRM_ZERO;
    }

    if (total_prb_allocated_dl >= total_prb_released_dl_factored)
    {
        dl_allocated_gbr_ngbr_prbs_temp = total_prb_allocated_dl - total_prb_released_dl_factored;
    }
    else
    {
        dl_allocated_gbr_ngbr_prbs_temp = RRM_ZERO;
    }
    /*SPR 18654 END*/
    /* SPR-15991_3_5NOV End */

    curr_gbr_ul_bitrate = erab_to_be_setup.erab_level_qos_params.gbr_qos_info.
        erab_guar_bit_rate_ul;
    curr_gbr_dl_bitrate = erab_to_be_setup.erab_level_qos_params.
        gbr_qos_info.erab_guar_bit_rate_dl;
    get_earlier_gbr_bitrate(&(p_rrm_ue_ctxt->drb_configured), erab_id,
            &prev_gbr_ul_bitrate, &prev_gbr_dl_bitrate, &old_qci);
    if( curr_gbr_dl_bitrate > prev_gbr_dl_bitrate)
    {
        dl_bitrate = curr_gbr_dl_bitrate - prev_gbr_dl_bitrate;
    }
    if( curr_gbr_ul_bitrate > prev_gbr_ul_bitrate)
    {
        ul_bitrate = curr_gbr_ul_bitrate - prev_gbr_ul_bitrate;
    }
    /* SPR-15991_2 END */

    /*SPR 18602 START*/
    if(type == RRM_ERB_REQ_TYPE_MODIFY)
    {
        if(prev_gbr_ul_bitrate > prev_gbr_dl_bitrate)
        {
            max_prev = prev_gbr_ul_bitrate;
        }
        else
        {
            max_prev = prev_gbr_dl_bitrate;
        }
        if(curr_gbr_ul_bitrate > curr_gbr_dl_bitrate)
        {
            max_curr = curr_gbr_ul_bitrate;
        }
        else
        {
            max_curr = curr_gbr_dl_bitrate;
        }

        if(max_curr > max_prev)
        {
            add_bitrate = (max_curr - max_prev);
        }
        else
        {
            sub_bitrate = (max_prev - max_curr);
        }
        /* SPR 21367 Fix Start */
        rrm_get_admitted_bit_rate(p_cell_context, &ul_gbr_bitrate, &dl_gbr_bitrate);
        admitted_bit_rate = (ul_gbr_bitrate + dl_gbr_bitrate) + (add_bitrate - sub_bitrate);
        /* SPR 21367 Fix End */
    }
    else if(type == RRM_ERB_REQ_TYPE_SETUP)
    {
        /* SPR 21367 Fix Start */
        rrm_get_admitted_bit_rate(p_cell_context, &ul_gbr_bitrate, &dl_gbr_bitrate);
        admitted_bit_rate = ul_gbr_bitrate + dl_gbr_bitrate + ul_bitrate + dl_bitrate;
        /* SPR 21367 Fix End */
    }

    /*Start TNL validation block*/
    total_backhaul_capacity = rrm_cellm_get_total_backhaul_capacity(cell_index);
    total_backhaul_capacity = total_backhaul_capacity * RRM_MEGA;
    capacity_threshold = rrm_cellm_get_capacity_threshold(cell_index);
    total_backhaul_capacity = (total_backhaul_capacity * capacity_threshold)/RRM_PERCENTAGE;
    if(total_backhaul_capacity >= admitted_bit_rate)
    {
        RRM_UEM_RAC_TRACE(g_uem_log_on_off,RRM_INFO,
                "Backhaul capcity[%llu] Required bit rate[%llu]",
                total_backhaul_capacity, admitted_bit_rate);
        ret_val = RRM_SUCCESS;
    }
    else
    {
        if(type == RRM_ERB_REQ_TYPE_SETUP)
        {
#ifdef ENDC_ENABLED
			if(!is_endc_ue_x2_up)
#endif
            /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT START */
            rrm_raise_event_erab_adm_rej(erab_to_be_setup.erab_id, RRM_EVENT_TNL_REJECT);
            /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT END*/
        }
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_ERROR, "Required bit rate more than TNL"
                "backhaul capacity Backhaul capcity[%llu] Required bit rate[%llu]",
                total_backhaul_capacity, admitted_bit_rate);
        ret_val = RRM_FAILURE;
        return ret_val;
    }
    /*SPR 18602 END*/
    /*SPR 19819 30JUN START*/
    RRM_UEM_RAC_TRACE(g_uem_log_on_off,RRM_INFO,
		    "dl_bitrate [%llu] = curr_gbr_dl_bitrate [%llu]- prev_gbr_dl_bitrate[%llu]\n"
		    "ul_bitrate [%llu] = curr_gbr_ul_bitrate [%llu] - prev_gbr_ul_bitrate [%llu]",
		    dl_bitrate, curr_gbr_dl_bitrate,prev_gbr_dl_bitrate,ul_bitrate,curr_gbr_ul_bitrate,prev_gbr_ul_bitrate);
    /*SPR 19819 30JUN END*/

    /*spr 7466 start*/
    U64 dl_bitrate_bkp=dl_bitrate;
    U64 ul_bitrate_bkp=ul_bitrate;
    /*spr 7466 end*/

    /*SPR 22151 Fix Start*/
    /*SPR 20782 Fix Start*/
    dl_required_num_prbs = rrm_convert_bitrate_to_prb_dl(cell_index,dl_bitrate);
    ul_required_num_prbs = rrm_convert_bitrate_to_prb_ul(cell_index,ul_bitrate);
    /*SPR 20782 Fix Stop*/
    /* SPR_12473_FIX_end */
    /*code deleted*/
    /* SPR-15991_2 END */
    /*SPR_15022_Start*/
    /*CID 97142 Start*/	
    /*SPR 19968 START*/
    dl_gbr_prbs_in_use_count = (rrm_prb_t)((RRM_CEIL)(
                (get_dl_res_block_from_dl_bandwidth(p_cell_context))*(dl_gbr_prbs_in_use/(float)RRM_PERCENTAGE)));
    ul_gbr_prbs_in_use_count = (rrm_prb_t)((RRM_CEIL)(
                (get_ul_res_block_from_ul_bandwidth(p_cell_context))*(ul_gbr_prbs_in_use/(float)RRM_PERCENTAGE)));
    dl_ngbr_prbs_in_use_count = (rrm_prb_t)((RRM_CEIL)(
                (get_dl_res_block_from_dl_bandwidth(p_cell_context))*(dl_ngbr_prbs_in_use/(float)RRM_PERCENTAGE)));
    ul_ngbr_prbs_in_use_count = (rrm_prb_t)((RRM_CEIL)(
                (get_ul_res_block_from_ul_bandwidth(p_cell_context))*(ul_ngbr_prbs_in_use/(float)RRM_PERCENTAGE)));
    /*SPR 19968 END*/
    /*SPR 22151 Fix End*/
    /*CID 97142 End*/	
    /*SPR_15022_End*/

    /*SPR 18654 START*/
    /*SPR 19819 30JUN START*/
    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_INFO,
    /*SPR 19819 30JUN END*/
            "\n[GBR admission](Unit is shown after multipled with factor[%u])\n"
            "Required GBR PRBs [DL:%u][UL:%u] GBR PRB budget [DL:%u][UL:%u]\n Combined GBR PRB [DL:%u][UL:%u]\n"
            "Allocated GBR PRBs [DL:%llu][UL:%llu]\n GBR PRB in use [DL:%u][UL:%lu]",
            rb_factor_for_bitrate, dl_required_num_prbs, ul_required_num_prbs,
            dl_prb_budget_gbr, ul_prb_budget_gbr,
            dl_prb_budget_for_gbr_ngbr_combined, ul_prb_budget_for_gbr_ngbr_combined,
            dl_allocated_gbr_prbs, ul_allocated_gbr_prbs,
            dl_gbr_prbs_in_use, ul_gbr_prbs_in_use);
    /*SPR_15022_End*/
    /*SPR 18654 END*/

    /* SPR-18198 START */
    if(dl_required_num_prbs > dl_prb_budget_for_gbr_ngbr_combined + dl_prb_budget_gbr)
    {
        /*SPR 18654 START*/
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_WARNING, "factored dl_required_num_prbs[%d] > factored"
                "dl_prb_budget_for_gbr_ngbr_combined[%u]+dl_prb_budget_gbr[%u]",
                dl_required_num_prbs,dl_prb_budget_for_gbr_ngbr_combined,dl_prb_budget_gbr );
        /*SPR 18654 END*/
        /*SPR 18602 START*/
        ret_val =  RRM_FAILURE;
        /*SPR 18602 END*/
    }
    /*SPR 18654 START*/
    else if(ul_required_num_prbs > ul_prb_budget_for_gbr_ngbr_combined + ul_prb_budget_gbr)
    {
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_WARNING, " factored ul_required_num_prbs[%u] > factored"   
                "ul_prb_budget_for_gbr_ngbr_combined[%u]+factored ul_prb_budget_gbr[%u]", 
                ul_required_num_prbs,ul_prb_budget_for_gbr_ngbr_combined, ul_prb_budget_gbr );
        /*SPR 18654 END*/
        /*SPR 18602 START*/
        ret_val =  RRM_FAILURE;
        /*SPR 18602 END*/
    }
    /* SPR-18198 END */
    /*SPR 18602 START*/
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "[UE:%d], Deallocating dl/ul_bitrate(%llu/%llu) from gbr limit during prb validation failure",
                p_rrm_ue_ctxt->ue_index,dl_bitrate_bkp,ul_bitrate_bkp);

        rrm_cellm_set_used_dl_bitrate_during_erb_release(p_rrm_ue_ctxt->cell_index, dl_bitrate_bkp);
        rrm_cellm_set_used_ul_bitrate_during_erb_release(p_rrm_ue_ctxt->cell_index, ul_bitrate_bkp);

        if(type == RRM_ERB_REQ_TYPE_SETUP)
        {
#ifdef ENDC_ENABLED
			if(!is_endc_ue_x2_up)
#endif
            rrm_raise_event_erab_adm_rej(erab_to_be_setup.erab_id, RRM_EVENT_LACK_OF_RESOURCES);
        }
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    /*SPR 18602 END*/
    /*SPR_15022_Start*/
    if((dl_required_num_prbs + dl_gbr_prbs_in_use_count + dl_allocated_gbr_prbs) >
            /*SPR_15022_End*/
            (dl_prb_budget_for_gbr_ngbr_combined + dl_prb_budget_gbr))
    {
        /*SPR_15022_Start*/
    /*SPR 18654 START*/
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                "[GBR admission] Estimated total DL GBR PRBs more than DL GBR "
                "factored budget dl_required_num_prbs[%u]+factored dl_gbr_prbs_in_use_count[%u]+"
                "factored dl_allocated_gbr_prbs[%llu] >factored dl_prb_budget_for_gbr_ngbr_combined[%u]+factored dl_prb_budget_gbr[%u]",
                dl_required_num_prbs,dl_gbr_prbs_in_use_count,dl_allocated_gbr_prbs,dl_prb_budget_for_gbr_ngbr_combined,dl_prb_budget_gbr );
    /*SPR 18654 END*/
        /*SPR_15022_End*/
        ret_val = RRM_FAILURE;

        /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT START*/
#ifdef ENDC_ENABLED
			if(!is_endc_ue_x2_up)
#endif
        rrm_raise_event_erab_adm_rej(erab_to_be_setup.erab_id, RRM_EVENT_GBR_DL_CAPACITY_REJECT);
        /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT END*/
    }
        /* SPR-15991_2 START */
    /*SPR_15022_Start*/
    else if((dl_required_num_prbs + dl_gbr_prbs_in_use_count + dl_ngbr_prbs_in_use_count +
                /*SPR_15022_End*/
                    /* SPR-15991_3_5NOV Start */
                    dl_allocated_gbr_ngbr_prbs_temp) >
                dl_usable_bw_prb_for_bearers)
            /* SPR-15991_3_5NOV End */
    {
        /*SPR_15022_Start*/
        /* SPR-18198 START */
        /*SPR 18654 START*/
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                "[GBR admission] Estimated total DL PRBs more than total available DL PRBs."
                "\n factored dl_required_num_prbs[%u]+factored dl_gbr_prbs_in_use_count[%u]+factored dl_ngbr_prbs_in_use_count[%u]"
                "+ factored dl_allocated_gbr_ngbr_prbs_temp[%u] > factored dl_usable_bw_prb_for_bearers[%u]",
                dl_required_num_prbs,dl_gbr_prbs_in_use_count,
                dl_ngbr_prbs_in_use_count,dl_allocated_gbr_ngbr_prbs_temp,dl_usable_bw_prb_for_bearers);
        /*SPR 18654 END*/
        /* SPR-18198 END */
        /*SPR_15022_End*/
        /* SPR-15991_2 END */
        ret_val = RRM_FAILURE;

#ifdef ENDC_ENABLED
			if(!is_endc_ue_x2_up)
#endif
        /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT START */
        rrm_raise_event_erab_adm_rej(erab_to_be_setup.erab_id, RRM_EVENT_GBR_DL_CAPACITY_REJECT);
        /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT END*/
    }
    /*SPR_15022_Start*/
    else if((ul_required_num_prbs + ul_gbr_prbs_in_use_count+
                /*SPR_15022_End*/
                ul_allocated_gbr_prbs) >
            (ul_prb_budget_for_gbr_ngbr_combined + ul_prb_budget_gbr))
    {
        /*SPR_15022_Start*/
    /*SPR 18654 START*/
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                "[GBR admission] Estimated total UL GBR PRBs more than UL GBR budget "
                " factored ul_required_num_prbs[%u]+factored ul_gbr_prbs_in_use_count[%u]+factored ul_allocated_gbr_prbs[%llu]"
                " > factored ul_prb_budget_for_gbr_ngbr_combined[%u]+factored ul_prb_budget_gbr[%u]",
                ul_required_num_prbs,ul_gbr_prbs_in_use_count,ul_allocated_gbr_prbs,
                ul_prb_budget_for_gbr_ngbr_combined,ul_prb_budget_gbr);
    /*SPR 18654 END*/
        /*SPR_15022_End*/
        ret_val = RRM_FAILURE;

#ifdef ENDC_ENABLED
			if(!is_endc_ue_x2_up)
#endif
        /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT START */
        rrm_raise_event_erab_adm_rej(erab_to_be_setup.erab_id, RRM_EVENT_GBR_UL_CAPACITY_REJECT);
        /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT END*/
    }
        /* SPR-15991_2 START */
    /*SPR_15022_Start*/
    else if((ul_required_num_prbs + ul_gbr_prbs_in_use_count + ul_ngbr_prbs_in_use_count +
                /*SPR_15022_End*/
                    /* SPR-15991_3_5NOV Start */
                    ul_allocated_gbr_ngbr_prbs_temp) >
                ul_usable_bw_prb_for_bearers)
            /* SPR-15991_3_5NOV End */
        {
            /*SPR_15022_Start*/
            /* SPR-15991_3_5NOV Start */
        /* SPR-18198 START */
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                "[GBR admission] Estimated total UL PRBs more than total available UL PRBs.");
    /*SPR 18654 START*/
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                " factored ul_required_num_prbs[%u]+factored ul_gbr_prbs_in_use_count[%u]+factored ul_ngbr_prbs_in_use_count[%u]"
                "+factored ul_allocated_gbr_ngbr_prbs_temp[%u] > factored ul_usable_bw_prb_for_bearers[%u]",
                ul_required_num_prbs, ul_gbr_prbs_in_use_count, ul_ngbr_prbs_in_use_count,
                ul_allocated_gbr_ngbr_prbs_temp, ul_usable_bw_prb_for_bearers);
    /*SPR 18654 END*/
        /* SPR-18198 END */
        /* SPR-15991_3_5NOV End */
        /*SPR_15022_End*/
            /* SPR-15991_2 END */
        ret_val = RRM_FAILURE;

#ifdef ENDC_ENABLED
			if(!is_endc_ue_x2_up)
#endif
        /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT START */
        rrm_raise_event_erab_adm_rej(erab_to_be_setup.erab_id, RRM_EVENT_GBR_UL_CAPACITY_REJECT);
        /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT END*/
    }

    if(ret_val == RRM_SUCCESS)
    {
    /*spr 17437 fix start*/
        if((type == RRM_ERB_REQ_TYPE_SETUP) &&
       (RRM_FAILURE == rrm_ue_check_config_gbr_drbs(p_rrm_ue_ctxt,gbr_configd_count,gbr_success_count)) &&
       (RRM_FALSE == rrm_is_self_rab_selected_for_deletion(p_rrm_ue_ctxt,p_rab_removal_ctxt)) )
    {
         ret_val = RRM_FAILURE;

            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_ERROR,
                    "[GBR admission] [GBR-Cfgd:%u][GBR-Success:%u] RAB admission will cross total GBR Rabs Allowed per ue",
                    gbr_configd_count, gbr_success_count);

#ifdef ENDC_ENABLED
			if(!is_endc_ue_x2_up)
#endif
         rrm_raise_event_erab_adm_rej(erab_to_be_setup.erab_id, RRM_EVENT_LACK_OF_RESOURCES);
    }
    else
    {
    /*spr 17437 fix stop*/
        rrm_cellm_set_dl_allocated_gbr_prb(cell_index, dl_allocated_gbr_prbs +
                dl_required_num_prbs);
        rrm_cellm_set_ul_allocated_gbr_prb(cell_index, ul_allocated_gbr_prbs +
                ul_required_num_prbs);
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED, "[GBR admission] Required UL & DL PRBs allocated Successfully");
    /*spr 17437 fix start*/
    }
    /*spr 17437 fix stop*/
    }
    /*if RAC fails then ARP needs to be applied*/
    else
    {
        /*SPR 16060 Fix2 Start*/
        /* SPR-18198 START */
        ret_val = rrm_ue_apply_arp(p_rrm_ue_ctxt,
                  ul_required_num_prbs, dl_required_num_prbs, erab_to_be_setup,(ul_gbr_prbs_in_use_count+ul_allocated_gbr_prbs),
          (dl_gbr_prbs_in_use_count+dl_allocated_gbr_prbs),(ul_prb_budget_gbr + ul_prb_budget_for_gbr_ngbr_combined),
          (dl_prb_budget_gbr + dl_prb_budget_for_gbr_ngbr_combined),
        /* SPR-18198 END */
    /*SPR 16060 Fix2 End*/
            /*spr 17437 fix start*/
                type,p_rab_removal_ctxt);
            /*spr 17437 fix stop*/
        /*Bug_756_changes_start*/
        if(RRM_SUCCESS == ret_val)
        {
            rrm_cellm_set_dl_allocated_gbr_prb(cell_index, dl_allocated_gbr_prbs +
                    dl_required_num_prbs);
            rrm_cellm_set_ul_allocated_gbr_prb(cell_index, ul_allocated_gbr_prbs +
                    ul_required_num_prbs);

            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                    "As preemption is successful so updated UL & DL allocated GBR PRBs");
        }
        /*Bug_756_changes_end*/
    }
    /*spr 7466 start*/
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "[UE:%d], Deallocating dl/ul_bitrate(%llu/%llu) from gbr limit during prb validation failure",
                p_rrm_ue_ctxt->ue_index,dl_bitrate_bkp,ul_bitrate_bkp);

        rrm_cellm_set_used_dl_bitrate_during_erb_release(p_rrm_ue_ctxt->cell_index, dl_bitrate_bkp);
        rrm_cellm_set_used_ul_bitrate_during_erb_release(p_rrm_ue_ctxt->cell_index, ul_bitrate_bkp);
    }
    /*spr 7466 end*/
    /*SPR 18602 START*/
    if(ret_val == RRM_SUCCESS)
    {
        if(type == RRM_ERB_REQ_TYPE_SETUP)
        {
            p_rrm_ue_ctxt->rrm_additional_bitrate_setup_mod = p_rrm_ue_ctxt->rrm_additional_bitrate_setup_mod + bitrate_required;
        }
        if(type == RRM_ERB_REQ_TYPE_MODIFY)
        {
            p_rrm_ue_ctxt->rrm_additional_bitrate_setup_mod = p_rrm_ue_ctxt->rrm_additional_bitrate_setup_mod + add_bitrate;
            p_rrm_ue_ctxt->rrm_reduced_bitrate_setup_mod = p_rrm_ue_ctxt->rrm_reduced_bitrate_setup_mod + sub_bitrate;
        }
    }
    /*SPR 18602 END*/
    return ret_val;

}

/****************************************************************************
 * Function Name  : rac_admit_ngbr_bearer
 * Inputs         : UE context pointer
 *                  required uplink bitrate
 *                  required downlink bitrate
                    erab_to_be_setup,
                    is_default_bearer,
                    gbr_dl_prbs_released,
                    gbr_ul_prbs_released,
                    ngbr_dl_prbs_released,
                    ngbr_ul_prbs_released,
                    p_out_dl_allocated_ngbr_prbs,
                    p_out_ul_allocated_ngbr_prbs,
                    p_out_dl_required_num_prbs,
                    p_out_ul_required_num_prbs,
                    type,
                    ngbr_configd_count,
                    ngbr_success_count,
                    p_rab_removal_ctxt

 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Checks if the non GBR bearer could be admitted or not. It
 *                  uses the input bitrates and the current bandwidth utilization
 *                  for checking.
 ****************************************************************************/
/*SPR_15022_Start*/
rrm_return_et
    rac_admit_ngbr_bearer
(
 rrm_ue_context_t *p_rrm_ue_ctxt,
 U64 ul_bitrate,
 U64 dl_bitrate,
 rrm_ue_erab_setup_item_t erab_to_be_setup,
 rrm_bool_et is_default_bearer,
        /*SPR 18654 START*/
 U64 gbr_dl_prbs_released,
 U64 gbr_ul_prbs_released,
 U64 ngbr_dl_prbs_released,
 U64 ngbr_ul_prbs_released,
 U64               *p_out_dl_allocated_ngbr_prbs,
 U64               *p_out_ul_allocated_ngbr_prbs,
 U32               *p_out_dl_required_num_prbs,
 U32              *p_out_ul_required_num_prbs,
 /*SPR 18654 END*/
 /*spr 17437 fix start*/
 U8               type,
 U8               ngbr_configd_count,
 U8               ngbr_success_count,
 rrm_rab_removal_ctxt_t          *p_rab_removal_ctxt
/*spr 17437 fix stop*/
#ifdef ENDC_ENABLED
,rrm_bool_et is_endc_ue_x2_up
#endif
 )
{
    rrm_return_et ret_val = RRM_SUCCESS;
    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;
    /*SPR 18654 START*/
    /*SPR 22151 Fix Start*/
    rrm_prb_t dl_required_num_prbs = RRM_ZERO;
    rrm_prb_t ul_required_num_prbs = RRM_ZERO;
    /*SPR 22151 Fix End*/

    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context (cell_index);
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE( g_uem_log_on_off, p_g_rrm_uem_facility_name,
                RRM_DETAILED,
                " cell index not found ");
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    /*SPR 22151 Fix Start*/
    rrm_prb_t dl_gbr_prbs_in_use_count = RRM_ZERO;
    rrm_prb_t ul_gbr_prbs_in_use_count = RRM_ZERO;
    rrm_prb_t dl_ngbr_prbs_in_use_count = RRM_ZERO;
    rrm_prb_t ul_ngbr_prbs_in_use_count = RRM_ZERO;
    /*SPR 22151 Fix End*/
    /*SPR_15022_End*/

    /* SPR-15991_3_5NOV Start */
    U64 total_prb_allocated_ul = RRM_ZERO;
    U64 total_prb_allocated_dl = RRM_ZERO;
    U64 total_prb_released_ul = RRM_ZERO;
    U64 total_prb_released_dl = RRM_ZERO;
    U64 dl_allocated_gbr_ngbr_prbs_temp =  RRM_ZERO;
    U64 ul_allocated_gbr_ngbr_prbs_temp =  RRM_ZERO;
    /* SPR-15991_3_5NOV End */
    U16   rb_factor_for_bitrate = p_cell_context->operator_info.admission_control_info.rb_factor_for_bitrate;
    /** Get the required downlink measurements from CellM */
    U32 dl_gbr_prbs_in_use                  = RRM_ZERO;
    U32 dl_ngbr_prbs_in_use                 = RRM_ZERO;
    U64 dl_allocated_gbr_prbs               =
        rrm_cellm_get_dl_allocated_gbr_prb(cell_index);
    U64 dl_allocated_ngbr_prbs              =
        rrm_cellm_get_dl_allocated_ngbr_prb(cell_index);
    U32 dl_prb_budget_for_gbr_ngbr_combined =
        (rrm_cellm_get_dl_prb_budget_combined(cell_index)*rb_factor_for_bitrate);
    U32 dl_prb_budget_ngbr                  =
        (rrm_cellm_get_dl_prb_budget_ngbr(cell_index)*rb_factor_for_bitrate);
    U32 dl_usable_bw_prb_for_bearers        =
        (rrm_cellm_get_dl_usable_bw_prb_for_bearers(cell_index)*rb_factor_for_bitrate);

    /** Get the required uplink measurements from CellM */
    U32 ul_gbr_prbs_in_use                  =
        (rrm_cellm_get_ul_gbr_prb_in_use(cell_index)*rb_factor_for_bitrate);
    U32 ul_ngbr_prbs_in_use                 = 
        (rrm_cellm_get_ul_ngbr_prb_in_use(cell_index)*rb_factor_for_bitrate);
    U64 ul_allocated_gbr_prbs               =
        rrm_cellm_get_ul_allocated_gbr_prb(cell_index);
    U64 ul_allocated_ngbr_prbs              =
        rrm_cellm_get_ul_allocated_ngbr_prb(cell_index);
    U32 ul_prb_budget_for_gbr_ngbr_combined =
        (rrm_cellm_get_ul_prb_budget_combined(cell_index)*rb_factor_for_bitrate);
    U32 ul_prb_budget_ngbr                  =
        (rrm_cellm_get_ul_prb_budget_ngbr(cell_index)*rb_factor_for_bitrate);
    U32 ul_usable_bw_prb_for_bearers        =
        (rrm_cellm_get_ul_usable_bw_prb_for_bearers(cell_index)*rb_factor_for_bitrate);
    /*SPR 18654 END*/
    U8 ngbr_threshold = rrm_cellm_get_ngbr_threshold(cell_index);
    U8 non_default_bearer_multiplier =
        rrm_cellm_get_non_default_bearer_multiplier(cell_index);
/* SPR-15991_2 START */

    /* SPR 14509 start */
    if((RRM_OAM_ACT_AS_VICTIM == p_cell_context->operator_info.eicic_info.eicic_provisioned_type)
            && (RRM_UE_VICTIM_STATE == p_rrm_ue_ctxt->rrm_ue_eicic_info.victim_ue))
    {
    /** Get the required downlink measurements from CellM */
        /*SPR 18654 START*/
        dl_gbr_prbs_in_use =
            (rrm_cellm_get_dl_gbr_abs_in_use(cell_index)*rb_factor_for_bitrate);
        dl_ngbr_prbs_in_use =
            (rrm_cellm_get_dl_ngbr_abs_in_use(cell_index)*rb_factor_for_bitrate);
        /*SPR 18654 END*/
    }
    else
    {
        /** Get the required downlink measurements from CellM */
        dl_gbr_prbs_in_use                  =
        (rrm_cellm_get_dl_gbr_prb_in_use(cell_index)*rb_factor_for_bitrate);
        dl_ngbr_prbs_in_use                 =
        (rrm_cellm_get_dl_ngbr_prb_in_use(cell_index)*rb_factor_for_bitrate);
    }
    /* SPR 14509 end */

    /* SPR-15991_3_5NOV Start */
    total_prb_allocated_ul = ul_allocated_ngbr_prbs + ul_allocated_gbr_prbs;
    total_prb_allocated_dl = dl_allocated_ngbr_prbs + dl_allocated_gbr_prbs;
    total_prb_released_ul = ngbr_ul_prbs_released + gbr_ul_prbs_released;
    total_prb_released_dl = ngbr_dl_prbs_released + gbr_dl_prbs_released;
    /*SPR 18654 START*/
    U64  total_prb_released_ul_factored = (total_prb_released_ul);
    U64  total_prb_released_dl_factored = (total_prb_released_dl);
    if (total_prb_allocated_ul >= total_prb_released_ul_factored)
    {
        ul_allocated_gbr_ngbr_prbs_temp = total_prb_allocated_ul - total_prb_released_ul_factored;
    }
    else
    {
        ul_allocated_gbr_ngbr_prbs_temp = RRM_ZERO;
    }

    if (total_prb_allocated_dl >= total_prb_released_dl_factored)
    {
        dl_allocated_gbr_ngbr_prbs_temp = total_prb_allocated_dl - total_prb_released_dl_factored;
    }
    else
    {
        dl_allocated_gbr_ngbr_prbs_temp = RRM_ZERO;
    }
    /*SPR 18654 END*/
    /* SPR-15991_3_5NOV End */
    U64 prev_ambr_dl_bitrate = p_rrm_ue_ctxt->ue_agg_max_bit_rate.
        ue_agg_max_bit_rate_dl;
    U64 prev_ambr_ul_bitrate = p_rrm_ue_ctxt->ue_agg_max_bit_rate.
        ue_agg_max_bit_rate_ul;
    /* SPR-15991_3_5NOV Start */
    if(dl_bitrate > prev_ambr_dl_bitrate)
    {
        U64 ngbr_dl_bitrate_diff = dl_bitrate - prev_ambr_dl_bitrate;
        /*SPR 20782 Fix Start*/
        dl_bitrate = ngbr_dl_bitrate_diff;
        /*SPR 20782 Fix Stop*/
        /* Check if this is the default bearer */
        if(RRM_FALSE == is_default_bearer)
        {
            dl_bitrate = (U64)(dl_bitrate * non_default_bearer_multiplier);
        }
        else
        {
            dl_bitrate = (U64)(dl_bitrate * ngbr_threshold);
        }
    }
    else
    {
        dl_bitrate = RRM_ZERO;
    }
    if(ul_bitrate > prev_ambr_ul_bitrate)
    {
        U64 ngbr_ul_bitrate_diff = ul_bitrate - prev_ambr_ul_bitrate;
        /*SPR 18654 START*/
        /*SPR 20782 Fix Start*/
        ul_bitrate = ngbr_ul_bitrate_diff;
        /*SPR 20782 Fix Stop*/
        /*SPR 18654 END*/
        /* Check if this is the default bearer */
        if(RRM_FALSE == is_default_bearer)
        {
            ul_bitrate = (U64)(ul_bitrate * non_default_bearer_multiplier);
        }
        else
        {
            ul_bitrate = (U64)(ul_bitrate * ngbr_threshold);
        }
    }
    else
    {
        ul_bitrate = RRM_ZERO;
    }
    /* SPR-15991_3_5NOV End */

    /* SPR-18198 START */
    /*SPR 18654 START*/
    /* SPR_12473_FIX_start */
    /*SPR 20782 Fix Start*/
    /*SPR 22151 Fix Start*/
    dl_required_num_prbs = rrm_convert_bitrate_to_prb_dl(cell_index,dl_bitrate);
    ul_required_num_prbs = rrm_convert_bitrate_to_prb_ul(cell_index,ul_bitrate);
    /*SPR 22151 Fix End*/
    /*SPR 20782 Fix Stop*/
    /* SPR_12473_FIX_start */
    /* SPR-18198 END */

/* SPR-15991_2 END */

    /*CID 97144 Start*/	
    /*SPR 22151 Fix Start*/
    /*SPR 19968 START*/
    dl_gbr_prbs_in_use_count = ((RRM_CEIL)
            (get_dl_res_block_from_dl_bandwidth(p_cell_context))*(dl_gbr_prbs_in_use/(float)RRM_PERCENTAGE));
    ul_gbr_prbs_in_use_count = ((RRM_CEIL)
            (get_ul_res_block_from_ul_bandwidth(p_cell_context))*(ul_gbr_prbs_in_use/(float)RRM_PERCENTAGE));
    dl_ngbr_prbs_in_use_count = ((RRM_CEIL)
            (get_dl_res_block_from_dl_bandwidth(p_cell_context))*(dl_ngbr_prbs_in_use/(float)RRM_PERCENTAGE));
    ul_ngbr_prbs_in_use_count = ((RRM_CEIL)
            (get_ul_res_block_from_ul_bandwidth(p_cell_context))*(ul_ngbr_prbs_in_use/(float)RRM_PERCENTAGE));
    /*SPR 19968 END*/
    /*SPR 22151 Fix End*/
    /*CID 97144 End*/
    /* SPR 19968 START */
    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
            "\n[NGBR admission] (Unit is shown after multipled with factor[%u])\n"
            "Required PRBs [DL:%u][UL:%u] Usable PRB [DL:%u][UL:%u]\n"
            "PRB budget (NGBR->[DL:%u][UL:%u] Combined GBR/NGBR ->[DL:%u][UL:%u])\n"
            "Allocated PRB (NGBR->[DL:%llu][UL:%llu] GBR->[DL:%llu][UL:%llu])\n"
            "PRB in use (NGBR->[DL:%u][UL:%u] GBR->[DL:%d][UL:%d])",
            rb_factor_for_bitrate, dl_required_num_prbs, ul_required_num_prbs, dl_usable_bw_prb_for_bearers, ul_usable_bw_prb_for_bearers,
            dl_prb_budget_ngbr, ul_prb_budget_ngbr, dl_prb_budget_for_gbr_ngbr_combined, ul_prb_budget_for_gbr_ngbr_combined,
            dl_allocated_ngbr_prbs, ul_allocated_ngbr_prbs, dl_allocated_gbr_prbs, ul_allocated_gbr_prbs,
            dl_ngbr_prbs_in_use, ul_ngbr_prbs_in_use, dl_gbr_prbs_in_use, ul_gbr_prbs_in_use);
    /*SPR_15022_End*/
    /* SPR-18198 START */
    if(dl_required_num_prbs > dl_prb_budget_for_gbr_ngbr_combined + dl_prb_budget_ngbr)
    {

        /*SPR 18654 START*/
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_ERROR, "factored dl_required_num_prbs[%u] > factored                                        dl_prb_budget_for_gbr_ngbr_combined[%u]+factored dl_prb_budget_gbr[%u]", dl_required_num_prbs,dl_prb_budget_for_gbr_ngbr_combined,          dl_prb_budget_ngbr);
        /*SPR 18654 END*/
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    if(ul_required_num_prbs > ul_prb_budget_for_gbr_ngbr_combined + ul_prb_budget_ngbr)
    {
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_ERROR, "ul_required_num_prbs[%d] > ul_prb_budget_for_gbr_ngbr_combined[%d]+                 ul_prb_budget_gbr[%d]",
                ul_required_num_prbs,ul_prb_budget_for_gbr_ngbr_combined,ul_prb_budget_ngbr );
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    /* SPR-18198 END */
    /* SPR 19968 END */

    /*SPR_15022_Start*/
    if((dl_required_num_prbs + dl_ngbr_prbs_in_use_count +
                /*SPR_15022_End*/
                dl_allocated_ngbr_prbs) >
            (dl_prb_budget_for_gbr_ngbr_combined + dl_prb_budget_ngbr))
    {
        /*SPR_15022_Start*/
        /*SPR 18654 START*/
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                "[NGBR admission] factored Estimated total DL NGBR PRBs more than DL NGBR budget "
                "factored dl_required_num_prbs[%u]+factored dl_ngbr_prbs_in_use_count[%u]+factored dl_allocated_ngbr_prbs[%llu] "
                "> factored dl_prb_budget_for_gbr_ngbr_combined[%u]+factored dl_prb_budget_ngbr[%u]",
                dl_required_num_prbs,dl_ngbr_prbs_in_use_count,dl_allocated_ngbr_prbs,
                dl_prb_budget_for_gbr_ngbr_combined,dl_prb_budget_ngbr);
        /*SPR 18654 END*/
        /*SPR_15022_End*/
        ret_val = RRM_FAILURE;
    }
    /* SPR-15991_2 START */
    /*SPR_15022_Start*/
    else if((dl_required_num_prbs + dl_gbr_prbs_in_use_count + dl_ngbr_prbs_in_use_count +
                /*SPR_15022_End*/
                /* SPR-15991_3_5NOV Start */
                dl_allocated_gbr_ngbr_prbs_temp ) >
            dl_usable_bw_prb_for_bearers)
    {
        /*SPR 18654 START*/
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                "[NGBR admission] factored Estimated total DL NPRBs more than total available DL PRBs "
                "factored dl_required_num_prbs[%u]+factored dl_gbr_prbs_in_use_count[%u]+factored dl_ngbr_prbs_in_use_count[%u]"
                "+factored dl_allocated_gbr_prbs[%llu]+factored dl_allocated_gbr_ngbr_prbs_temp[%llu] > factored                            dl_usable_bw_prb_for_bearers[%u]",
                dl_required_num_prbs,dl_gbr_prbs_in_use_count,dl_ngbr_prbs_in_use_count,dl_allocated_gbr_prbs,
                dl_allocated_gbr_ngbr_prbs_temp,dl_usable_bw_prb_for_bearers);
        /*SPR 18654 START*/
        /* SPR-15991_3_5NOV End */
        /* SPR-15991_2 END */
        ret_val = RRM_FAILURE;
    }
    /*SPR_15022_Start*/
    else if((ul_required_num_prbs + ul_ngbr_prbs_in_use_count +
                /*SPR_15022_End*/
                ul_allocated_ngbr_prbs) >
            (ul_prb_budget_for_gbr_ngbr_combined + ul_prb_budget_ngbr))
    {
        /*SPR 18654 START*/
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                "[NGBR admission] factored Estimated total UL NGBR PRBs more than UL NGBR budget "
                "factored ul_required_num_prbs[%u]+factored ul_ngbr_prbs_in_use_count[%u]+factored ul_allocated_ngbr_prbs[%llu] "
                "> factored ul_prb_budget_for_gbr_ngbr_combined[%u]+factored ul_prb_budget_ngbr[%u]",
                ul_required_num_prbs,ul_ngbr_prbs_in_use_count,ul_allocated_ngbr_prbs,
                ul_prb_budget_for_gbr_ngbr_combined,ul_prb_budget_ngbr);
        /*SPR 18654 END*/
        ret_val = RRM_FAILURE;
    }
    /* SPR-15991_2 START */
    /*SPR_15022_Start*/
    else if((ul_required_num_prbs + ul_gbr_prbs_in_use_count + ul_ngbr_prbs_in_use_count +
                /*SPR_15022_End*/
                /* SPR-15991_3_5NOV Start */
                ul_allocated_gbr_ngbr_prbs_temp ) >
            ul_usable_bw_prb_for_bearers)
        /* SPR-15991_3_5NOV End */
    {
        /*SPR 18654 START*/
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                "[NGBR admission] factored Estimated total UL NPRBs more than total available UL PRBs "
                "factored ul_required_num_prbs[%u]+factored ul_gbr_prbs_in_use_count[%u]+factored ul_ngbr_prbs_in_use_count"
                "+factored ul_allocated_gbr_prbs[%llu]+factored ul_allocated_gbr_ngbr_prbs_temp[%llu] > factored                            ul_usable_bw_prb_for_bearers[%llu]",
                ul_required_num_prbs,ul_gbr_prbs_in_use_count,ul_ngbr_prbs_in_use_count,
                ul_allocated_gbr_prbs,ul_allocated_gbr_ngbr_prbs_temp,ul_usable_bw_prb_for_bearers);
        /*SPR 18654 END*/
        /* SPR-15991_2 END */
        ret_val = RRM_FAILURE;
    }

    *p_out_dl_allocated_ngbr_prbs = dl_allocated_ngbr_prbs;
    *p_out_ul_allocated_ngbr_prbs = ul_allocated_ngbr_prbs;

    if(ret_val == RRM_SUCCESS)
    {
        /*SPR 4820 START*/
        /*Allocated N-GBR PRBs shall not be considered when AMBR is used
          for all non-GBR ERBS. It shall be considered when admission would be
          done on the basis of minimum bitrate for each non-GBR ERB */

        /* Allocated prbs for non gbr bearers of a ue will be 
           stored later on. As all non-gbr bearers shares the ue ambr,
           we should only store the consolidated prb for ue ambr
           in allocated_ngbr_prb */

        /* *p_out_dl_allocated_ngbr_prbs = dl_allocated_ngbr_prbs;*/
        /* *p_out_ul_allocated_ngbr_prbs = ul_allocated_ngbr_prbs;*/
    /*spr 17437 fix start*/
    if((type == RRM_ERB_REQ_TYPE_SETUP) &&
       (RRM_FAILURE == rrm_ue_check_config_ngbr_drbs(p_rrm_ue_ctxt,ngbr_configd_count,ngbr_success_count))
        && (RRM_FALSE == rrm_is_self_rab_selected_for_deletion(p_rrm_ue_ctxt,p_rab_removal_ctxt)))
    {
         ret_val = RRM_FAILURE;

             RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_ERROR,
                    "[NGBR admission] [NGBR-Cfgd:%u][NGBR-Success:%u] "
                    "RAB admission will cross total NGBR Rabs Allowed per ue",
                    ngbr_configd_count,ngbr_success_count);
    }
    else
    {
    /*spr 17437 fix stop*/
        *p_out_dl_required_num_prbs = dl_required_num_prbs;
        *p_out_ul_required_num_prbs = ul_required_num_prbs;
    /*spr 17437 fix start*/
    }
    /*spr 17437 fix stop*/
        /*SPR 4820 END*/
    }
    /*if RAC fails then ARP needs to be applied*/
    else
    {
#ifdef ENDC_ENABLED
        if(is_endc_ue_x2_up)
        {
        	return ret_val;
        }
#endif
        /*SPR 16060 Fix2 Start*/
        /* SPR-18198 START */
        ret_val = rrm_ue_apply_arp(p_rrm_ue_ctxt,
                  ul_required_num_prbs, dl_required_num_prbs, erab_to_be_setup,(ul_ngbr_prbs_in_use_count+ul_allocated_ngbr_prbs),
          (dl_ngbr_prbs_in_use_count+dl_allocated_ngbr_prbs),(ul_prb_budget_ngbr + ul_prb_budget_for_gbr_ngbr_combined),
          (dl_prb_budget_ngbr + dl_prb_budget_for_gbr_ngbr_combined)
        /* SPR-18198 END */
    /*SPR 16060 Fix2 End*/
        /*spr 17437 fix start*/
        ,type,p_rab_removal_ctxt);
        /*spr 17437 fix stop*/

        /*Bug_756_changes_start*/
        if(RRM_SUCCESS == ret_val)
        {
            *p_out_dl_required_num_prbs = dl_required_num_prbs;
            *p_out_ul_required_num_prbs = ul_required_num_prbs;

            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                    "As preemption is successful so considering required_num_prbs.");
        }
        /*Bug_756_changes_end*/
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/* SPR-15991_2 START */
/****************************************************************************
 * Function Name  : rac_release_gbr_bearer
 * Inputs         : UE context pointer
                    erab_to_modify,
                    gbr_dl_prbs_released,
                    gbr_ul_prbs_released
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Checks if the non GBR bearer could be admitted or not. It
 *                  uses the input bitrates and the current bandwidth utilization
 *                  for checking.
****************************************************************************/
rrm_return_et
    rac_release_gbr_bearer
(
 rrm_ue_context_t   *p_rrm_ue_ctxt,
 rrm_ue_erab_setup_item_t erab_to_modify,
 U8                 gbr_dl_prbs_released,
 U8                 gbr_ul_prbs_released
 )
{
    RRM_UT_TRACE_ENTER();
    rrm_return_et ret_val = RRM_SUCCESS;
    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;
    U64 diff_gbr_ul_bitrate = RRM_ZERO;
    U64 diff_gbr_dl_bitrate = RRM_ZERO;
    U64 curr_gbr_ul_bitrate = RRM_ZERO;
    U64 curr_gbr_dl_bitrate = RRM_ZERO;
    U64 prev_gbr_ul_bitrate = RRM_ZERO;
    U64 prev_gbr_dl_bitrate = RRM_ZERO;
    U8 old_qci = RRM_ZERO;
    U8 erab_id = erab_to_modify.erab_id;

    /*SPR 18602 START*/
    U64 max_prev =  RRM_ZERO;
    U64 max_curr =  RRM_ZERO;
    /*SPR 18602 END*/
    /*SPR 18654 START*/
    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context (cell_index);
    /* CID 109677 */
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE( g_uem_log_on_off, p_g_rrm_uem_facility_name,
                RRM_DETAILED,
                " cell index not found ");
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    U16   rb_factor_for_bitrate = p_cell_context->operator_info.admission_control_info.rb_factor_for_bitrate;
    U64  factored_gbr_dl_prbs_released = (rb_factor_for_bitrate * gbr_dl_prbs_released);
    U64  factored_gbr_ul_prbs_released = (rb_factor_for_bitrate * gbr_ul_prbs_released);
    /*SPR 18654 END*/

    curr_gbr_dl_bitrate = erab_to_modify.erab_level_qos_params.
        gbr_qos_info.erab_guar_bit_rate_dl;
    curr_gbr_ul_bitrate = erab_to_modify.erab_level_qos_params.gbr_qos_info.
        erab_guar_bit_rate_ul;

    get_earlier_gbr_bitrate(&(p_rrm_ue_ctxt->drb_configured), erab_id,
            &prev_gbr_ul_bitrate, &prev_gbr_dl_bitrate, &old_qci);

    /*SPR 18602 START*/
    if((prev_gbr_dl_bitrate >= curr_gbr_dl_bitrate)&&(prev_gbr_ul_bitrate >= curr_gbr_ul_bitrate))
    {
        if(prev_gbr_ul_bitrate > prev_gbr_dl_bitrate)
        {
            max_prev = prev_gbr_ul_bitrate;
        }
        else
        {
            max_prev = prev_gbr_dl_bitrate;
        }
        if(curr_gbr_ul_bitrate > curr_gbr_dl_bitrate)
        {
            max_curr = curr_gbr_ul_bitrate;
        }
        else
        {
            max_curr = curr_gbr_dl_bitrate;
        }
        p_rrm_ue_ctxt->rrm_reduced_bitrate_setup_mod = p_rrm_ue_ctxt->rrm_reduced_bitrate_setup_mod + (max_prev - max_curr);
    }
    /*SPR 18602 END*/
    /* SPR-18198 START */
    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "prev_gbr_dl_bitrate[%llu],curr_gbr_dl_bitrate[%llu],prev_gbr_ul_bitrate[%llu],curr_gbr_ul_bitrate[%llu]"
            ,prev_gbr_dl_bitrate,curr_gbr_dl_bitrate,prev_gbr_ul_bitrate,curr_gbr_ul_bitrate);
    if(prev_gbr_dl_bitrate >= curr_gbr_dl_bitrate)
    {
        diff_gbr_dl_bitrate = prev_gbr_dl_bitrate - curr_gbr_dl_bitrate;
    }
    else
    {
        diff_gbr_dl_bitrate = RRM_ZERO;
    }
    if(prev_gbr_ul_bitrate >= curr_gbr_ul_bitrate)
    {
        diff_gbr_ul_bitrate = prev_gbr_ul_bitrate - curr_gbr_ul_bitrate;
    }
    else
    {
        diff_gbr_ul_bitrate = RRM_ZERO;
    }
    /* SPR-18198 END */
    rrm_cellm_set_used_dl_bitrate_during_erb_release(cell_index, diff_gbr_dl_bitrate);
    rrm_cellm_set_used_ul_bitrate_during_erb_release(cell_index, diff_gbr_ul_bitrate);

    /*SPR 18654 START*/
    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "[UE:%d],Deallocating dl_gbr_prbs(%llu)"
            ,p_rrm_ue_ctxt->ue_index,factored_gbr_dl_prbs_released);
    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "[UE:%d],Deallocating ul_gbr_prbs(%llu)"
            ,p_rrm_ue_ctxt->ue_index,factored_gbr_ul_prbs_released);

    rrm_cellm_reset_dl_allocated_gbr_prb(cell_index,factored_gbr_dl_prbs_released);
    rrm_cellm_reset_ul_allocated_gbr_prb(cell_index,factored_gbr_ul_prbs_released);
    /*SPR 18654 END*/

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR-15991_2 END */

/* SPR-15991_2 START */
/****************************************************************************
 * Function Name  : rac_release_ngbr_bearer
 * Inputs         : UE context pointer
                    ngbr_dl_prbs_released,
                    ngbr_ul_prbs_released,
                    dl_allocated_ngbr_prbs,
                    ul_allocated_ngbr_prbs
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Checks if the non GBR bearer could be admitted or not. It
 *                  uses the input bitrates and the current bandwidth utilization
 *                  for checking.
****************************************************************************/

/*SPR 18654 START*/
/* CID 96511 */
    rrm_return_et
rac_release_ngbr_bearer(
        rrm_ue_context_t    *p_rrm_ue_ctxt,
        U8                  ngbr_dl_prbs_released,
        U8                  ngbr_ul_prbs_released,
        U64                  dl_allocated_ngbr_prbs,
        U64                 ul_allocated_ngbr_prbs
        )
{
    rrm_return_et ret_val = RRM_SUCCESS;
    RRM_UT_TRACE_ENTER();
    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;
    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context (cell_index);
    /* CID 109674 */
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                RRM_ERROR,"Cell Context not found");
        RRM_UT_TRACE_EXIT();
        return ret_val;
    }
    U16   rb_factor_for_bitrate = p_cell_context->operator_info.admission_control_info.rb_factor_for_bitrate;
    U64  factored_ngbr_dl_prbs_released = (rb_factor_for_bitrate * ngbr_dl_prbs_released);
    U64  factored_ngbr_ul_prbs_released = (rb_factor_for_bitrate * ngbr_ul_prbs_released);
    /*Store allocated prbs for non-gbr bearers(required prb for ue-ambr)*/
    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
            "Number of DL ngbr PRBs to be released and PRBs allocated,"
            "%llu , %llu, deallocating the allocated PRBs only",
            factored_ngbr_dl_prbs_released, dl_allocated_ngbr_prbs);
    /* SPR-15991_3_5NOV Start */
    if(dl_allocated_ngbr_prbs < factored_ngbr_dl_prbs_released)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Number of DL ngbr PRBs to be released is greater than PRBs allocated,"
                "%u > %u, deallocating the allocated PRBs only",
                factored_ngbr_dl_prbs_released, dl_allocated_ngbr_prbs);
        factored_ngbr_dl_prbs_released = dl_allocated_ngbr_prbs;
    }
    rrm_cellm_set_dl_allocated_ngbr_prb(p_rrm_ue_ctxt->cell_index, dl_allocated_ngbr_prbs
            - factored_ngbr_dl_prbs_released);
    if(ul_allocated_ngbr_prbs < factored_ngbr_ul_prbs_released)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Number of UL ngbr PRBs to be released is greater than PRBs allocated,"
                "%llu > %llu, deallocating the allocated PRBs only",
                factored_ngbr_ul_prbs_released, ul_allocated_ngbr_prbs);
        factored_ngbr_ul_prbs_released = ul_allocated_ngbr_prbs;
    }
    rrm_cellm_set_ul_allocated_ngbr_prb(p_rrm_ue_ctxt->cell_index, ul_allocated_ngbr_prbs
            - factored_ngbr_ul_prbs_released);
    /* SPR-15991_3_5NOV End */

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/*SPR 18654 END*/
/* SPR-15991_2 END */


/****************************************************************************
 * Function Name  : rrm_ue_check_config_gbr_drbs
 * Inputs         : UE context
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function checks number of gbr drbs configured and 
 *                  returns failure if already maximum number of gbr drbs are
 *                  configured
 ****************************************************************************/
rrm_return_et
rrm_ue_check_config_gbr_drbs
(
    rrm_ue_context_t *p_rrm_ue_context, 
    U8 gbr_configd_count,
    U8 gbr_success_count
)
{
    rrm_cell_index_t          cell_index;
    admission_control_info_t* p_admission_control_info = RRM_NULL;
    cell_index = p_rrm_ue_context->cell_index;
 
    p_admission_control_info =
        rrm_cellm_get_admission_control_info(cell_index);
    /* SPR 22316 erab fix start */ 
    if((gbr_configd_count+gbr_success_count) <
            p_admission_control_info->max_num_gbr_drbs_per_ue)
    /* SPR 22316 erab fix end */
    {
       return RRM_SUCCESS;   
    }
    else
    {
       return RRM_FAILURE;
    }
}

/****************************************************************************
 * Function Name  : rrm_ue_check_config_ngbr_drbs
 * Inputs         : UE context
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function checks number of ngbr drbs configured and
 *                  returns failure if already maximum number of ngbr drbs are
 *                  configured
 ****************************************************************************/
rrm_return_et
rrm_ue_check_config_ngbr_drbs
(
    rrm_ue_context_t *p_rrm_ue_context,
    U8 ngbr_configd_count,
    U8 ngbr_success_count
 )
{
    rrm_cell_index_t          cell_index;
    cell_index = p_rrm_ue_context->cell_index;
    admission_control_info_t* p_admission_control_info = RRM_NULL;

    p_admission_control_info = 
        rrm_cellm_get_admission_control_info(cell_index);;

    if((ngbr_configd_count+ngbr_success_count) <
            p_admission_control_info->max_num_ngbr_drbs_per_ue)
    {
        return RRM_SUCCESS;
    }
    else
    {
       return RRM_FAILURE;
    }
}

/*spr 7466 start*/
/****************************************************************************
 * Function Name  : clean_resources_for_the_bearer
 * Inputs         : rrm_ue_erb_t
 * Outputs        : None
 * Returns        : void
 * Description    : This function deallocates prb isage and gbr limit from
 cell context
 ****************************************************************************/
void
clean_resources_for_the_bearer(
        rrm_ue_erb_t erb_data
        )
{
    RRM_UT_TRACE_ENTER();
    rrm_ue_context_t *p_rrm_ue_ctxt=erb_data.p_rrm_ue_context;
    /*SPR 18654 START*/
    U64 dl_prbs_to_release=RRM_ZERO;
    U64 ul_prbs_to_release=RRM_ZERO;
    /*SPR 18654 END*/
    U64 dl_bitrate=RRM_ZERO;
    U64 ul_bitrate=RRM_ZERO;
    U8 index=RRM_ZERO;
    /*SPR 18654 START*/
    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;
    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context (cell_index);
    /* CID 109681 */
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                RRM_ERROR,"Cell Context not found");
        RRM_UT_TRACE_EXIT();
        /* CID 109705 */
        return;
    }    
    U16   rb_factor_for_bitrate = p_cell_context->operator_info.admission_control_info.rb_factor_for_bitrate;
    /*SPR 18654 END*/

    for(index=RRM_ZERO;index<p_rrm_ue_ctxt->drb_configured.num_of_list;index++)
    {
        if(erb_data.erab_id == p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_id)
        {
            dl_bitrate=p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_dl;
            ul_bitrate=p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_ul;
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "[UE:%d]:erb_id[%d] Deallocating dl_bitrate(%llu) from gbr limit"
                    ,p_rrm_ue_ctxt->ue_index, p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_id,dl_bitrate);

            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "[UE:%d]:erb_id[%d] Deallocating ul_bitrate(%llu) from gbr limit"
                    ,p_rrm_ue_ctxt->ue_index, p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_id,ul_bitrate);

            rrm_cellm_set_used_dl_bitrate_during_erb_release(p_rrm_ue_ctxt->cell_index, dl_bitrate);
            rrm_cellm_set_used_ul_bitrate_during_erb_release(p_rrm_ue_ctxt->cell_index, ul_bitrate);
            /*SPR 18654 START*/
            /*code deleted*/
            /* SPR_12473_FIX_start */
            dl_prbs_to_release = (U64)RRM_CEIL((float)((rb_factor_for_bitrate * dl_bitrate * RRM_HUNDRED) /
                        (float)(rrm_cellm_get_num_prb_based_on_snr
                            (p_rrm_ue_ctxt->cell_index, RRM_FALSE) * RRM_MEGA)));
            ul_prbs_to_release = (U64)RRM_CEIL((float)((rb_factor_for_bitrate * ul_bitrate * RRM_HUNDRED) / 
                        (float)(rrm_cellm_get_num_prb_based_on_snr
                            (p_rrm_ue_ctxt->cell_index, RRM_TRUE) * RRM_MEGA)));
            /* SPR_12473_FIX_start */
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "[UE:%d]:erb_id[%d] Deallocating dl/ul_gbr_prbs(%d/%d)",
                    p_rrm_ue_ctxt->ue_index,p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_id,
                    dl_prbs_to_release,ul_prbs_to_release);
            /*SPR 18654 END*/

            rrm_cellm_reset_dl_allocated_gbr_prb(p_rrm_ue_ctxt->cell_index,dl_prbs_to_release);
            rrm_cellm_reset_ul_allocated_gbr_prb(p_rrm_ue_ctxt->cell_index,ul_prbs_to_release);
            break;
        }
    }
    RRM_UT_TRACE_EXIT();
}
/*spr 7466 end*/

/*GBR LIMIT START */
/****************************************************************************
 * Function Name  : rrm_ue_apply_arp_for_gbr_limit
 * Inputs         : UE context pointer
 *		    used DL bitrate
 *		    used UL bitrate
 *                  required uplink bitrate
 *                  required downlink bitrate
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Checks if the ERB release indication is sent or not
 ****************************************************************************/
/*Kshitij rel1.3*/
rrm_return_et
rrm_ue_apply_arp_for_gbr_limit
(
 rrm_ue_context_t *p_rrm_ue_ctxt,
 U64 used_dl_bitrate,
 U64 used_ul_bitrate,
 U64 dl_required_gbr,
 U64 ul_required_gbr,
    /*SPR 19968 START*/
 rrm_ue_erab_setup_item_t *p_erab_to_be_setup,
    /*SPR 19968 END*/
/*spr 17437 fix start*/
 U8  type,
rrm_rab_removal_ctxt_t          *p_rab_removal_ctxt,
    /*SPR 19968 START*/
 U32 *p_data_transfer_mode
    /*SPR 19968 END*/
/*spr 17437 fix stop*/
 )
{

    /*SPR 19968 Start*/
    rrm_ue_context_t *p_rrm_rel_erb_ue_ctxt = RRM_PNULL;
    rrm_return_et ret_val = RRM_SUCCESS;
    U8 count = RRM_ZERO;
    /* SPR 19968 End */
    U8 preemption_capability = RRM_ZERO;
    U64 deallocated_dl_gbr = RRM_ZERO;
    U64 deallocated_ul_gbr = RRM_ZERO;
    U64 released_dl_gbr = RRM_ZERO;
    U64 released_ul_gbr = RRM_ZERO;
    rrm_ue_erb_t erb_data;
    /*spr 17437 fix start*/
    /*Code Deleted*/
    /*spr 17437 fix stop*/
    rrm_ue_erb_list_t *erb_release_list = RRM_PNULL;
    rrm_bool_et is_low_priority_erb_exist = RRM_TRUE;
    rrm_ue_erb_release_queue_t erb_release_queue;
    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;
    /*spr 17437 fix start*/
    /*Code Deleted*/
    /*spr 17437 fix stop*/
    rrm_ue_index_t ue_index = RRM_ZERO;
    U8 erb_index = RRM_ZERO;
    /*spr 17437 fix start*/
    /*Code Deleted*/
    /*spr 17437 fix stop*/
    /*Bug_721_changes_start*/
    U8      current_ue_priority = RRM_UE_ERB_LOWEST_ARP;
    U8   list_index = RRM_ZERO;
    rrm_ue_proc_erab_setup_data_t   *p_rrm_ue_proc_erab_setup_data = RRM_PNULL;
    rrm_ue_erab_setup_item_list_t   *p_erab_setup_item_list = RRM_PNULL;
    /*spr 17437 fix start*/
    /* SPR 20652 Fix Start */
    /* Code Removed */
    rrm_ue_context_t      *p_ue_context     = RRM_PNULL;
    /* SPR 20652 Fix End */
    rrm_ue_remove_node_t  *p_ue_rem_node    = RRM_PNULL;
    /*spr 17437 fix stop*/
    /*spr 19968 fix start*/
    U32 num_temp_um_mode = RRM_ZERO;
    U32 num_temp_am_mode = RRM_ZERO;
    U8 erb_to_setup_rlc_mode = RRM_ZERO;
    U8 erb_to_rel_rlc_mode = RRM_ZERO;
    /*spr 19968 fix end*/
    /*SPR 19968 End*/
    /*Bug_721_changes_end*/
    RRM_UT_TRACE_ENTER();

    /*Bug_721_changes_start*/
    p_rrm_ue_proc_erab_setup_data = (rrm_ue_proc_erab_setup_data_t *)
        p_rrm_ue_ctxt->current_procedure.p_proc_data;
    RRM_ASSERT(RRM_PNULL != p_rrm_ue_proc_erab_setup_data);
    p_erab_setup_item_list =  &p_rrm_ue_proc_erab_setup_data->erab_setup_list;
    RRM_ASSERT(RRM_PNULL != p_erab_setup_item_list);
    /*Bug_721_changes_end*/
    /*SPR 19968 START*/
    erb_to_setup_rlc_mode = rrm_get_data_transfer_mode_configd(p_erab_to_be_setup->erab_level_qos_params.qci, p_rrm_ue_ctxt->cell_index);
    /*SPR 19968 End*/
    U16 dl_gbr_limit = rrm_cellm_get_dl_gbr_limit(cell_index);
    U16 ul_gbr_limit = rrm_cellm_get_ul_gbr_limit(cell_index);
    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
            "dl_gbr_limit[%d], ul_gbr_limit[%d]", dl_gbr_limit, ul_gbr_limit);
    /* Coverity ID 73729, 73894 Fix Start */
    U64 diff_dl_bitrate = (((U64)dl_gbr_limit)*RRM_MEGA) - used_dl_bitrate;
    U64 diff_ul_bitrate = (((U64)ul_gbr_limit)*RRM_MEGA) - used_ul_bitrate;
    /* Coverity ID 73729, 73894 Fix End */
    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
            "diff_dl_bitrate[%llu], diff_ul_bitrate[%llu] used_dl_bitrate[%llu], used_ul_bitrate[%llu]", 
            diff_dl_bitrate, diff_ul_bitrate, used_dl_bitrate, used_ul_bitrate);

    RRM_MEMSET(&erb_data, RRM_ZERO, sizeof(rrm_ue_erb_t));


    /*SPR 19968 START*/
    preemption_capability = p_erab_to_be_setup->erab_level_qos_params.
        alloc_and_reten_prior.preemption_capability;
    /* SPR 19968 End */
    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
            "preemption_capability[%d]", preemption_capability);

    /*check if requested RAB is preemption capable or not*/
    if( RRM_TRUE == rrm_ue_check_premption_capability(p_rrm_ue_ctxt,
                preemption_capability))
    {

        ylInit(&erb_release_queue);
        do {
            /*getting low priority RABs that need to be released to fulfill*/
            /*requirement*/
            if(RRM_SUCCESS == rrm_ue_get_low_priority_gbr_erb(&erb_data, p_rrm_ue_ctxt,
                        /* SPR 19968 Start */
                        p_erab_to_be_setup, type))
                /* SPR 19968 End */
            {
                /*SPR 21773 Start*/
                if(RRM_PNULL != erb_data.p_rrm_ue_context)
                {
                    /*SPR 21773 End*/
                    /*Bug_721_changes_start*/
                    erb_release_list = rrm_mem_get(sizeof(rrm_ue_erb_list_t));

                    if(RRM_PNULL == erb_release_list)
                    {
                        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,
                                "rrm_ue_apply_arp: Mem allocation failed");
                        return RRM_FAILURE;
                    }
                    /*Bug_721_changes_end*/

                    /* BUG_FIX_837_START */
                    RRM_MEMSET(erb_release_list,RRM_ZERO,sizeof(rrm_ue_erb_list_t));
                    /* BUG_FIX_837_END */

                    for(erb_index = RRM_ZERO; erb_index < 
                            erb_data.p_rrm_ue_context->drb_configured.num_of_list; erb_index++)
                    {
                        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF,
                                "1. Erab_id [%d], UE Rab Id[%d]", 
                                erb_data.p_rrm_ue_context->drb_configured.erab_item[erb_index].erab_id, 
                                erb_data.erab_id);	
                        if(erb_data.p_rrm_ue_context->drb_configured.erab_item[erb_index].erab_id == erb_data.erab_id) 	
                        {
                            /*SPR 19968 Start*/
                            erb_to_rel_rlc_mode = erb_data.p_rrm_ue_context->drb_configured.erab_item[erb_index].data_transfer_mode;
                            /*SPR 19968 End*/
                            break;
                        }
                    }
                    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                            "Low Priority RAB[%d] found on index[%d]", erb_data.erab_id, erb_index);

                    /*SPR 19968 Start*/
                    if(erb_to_rel_rlc_mode != erb_to_setup_rlc_mode)
                    {
                        ret_val = rrm_uem_decide_am_um_mode(p_rrm_ue_ctxt,
                                erb_to_setup_rlc_mode,
                                p_data_transfer_mode,
                                &num_temp_um_mode,
                                &num_temp_am_mode,
                                *p_erab_to_be_setup);


                    }
                    /* SPR 21994 Start */
                    else
                    {
                        *p_data_transfer_mode= erb_to_setup_rlc_mode;
                    }
                    /* SPR 21994 End */
		    if(ret_val == RRM_FAILURE)
                    {

                        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_ERROR,
                                "ERAB ID = %d cannot be release due to failure returned from function rrm_uem_decide_am_um_mode ",erb_data.erab_id);
                    }
                    else
                    {
                        RRM_MEMCPY(&(erb_release_list->ue_erb), &erb_data, sizeof(rrm_ue_erb_t));
                        ylPushTail(&erb_release_queue, &(erb_release_list)->pNode);

                        released_dl_gbr = (U64)(erb_data.p_rrm_ue_context->drb_configured.
                                erab_item[erb_index].erab_level_qos_params.
                                gbr_qos_info.erab_guar_bit_rate_dl);
                        deallocated_dl_gbr += released_dl_gbr;
                        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                                "released_dl_gbr[%llu], deallocated_dl_gbr[%llu]", released_dl_gbr, deallocated_dl_gbr);

                        released_ul_gbr = (U64)(erb_data.p_rrm_ue_context->drb_configured.
                                erab_item[erb_index].erab_level_qos_params.
                                gbr_qos_info.erab_guar_bit_rate_ul);
                        deallocated_ul_gbr += released_ul_gbr;
                        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                                "released_ul_gbr[%llu], deallocated_ul_gbr[%llu]", released_ul_gbr, deallocated_ul_gbr);
                        if(RRM_FAILURE == rrm_ue_delete_erb_entity(&erb_data))
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                    "UE_INDEX[%d]: Failure received while deletion from erab priority  \
                                    list", erb_data.p_rrm_ue_context->ue_index);
                        }
                        /* SPR 19968 Start */
                        if(RRM_TRUE != rrm_is_ue_special_previledge(p_rrm_ue_ctxt))
                            /* SPR 19968 End */
                        {

                            if (RRM_FAILURE == rrm_ue_update_ue_priority_list(p_rrm_ue_ctxt))
                            {
                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                        "UE_INDEX[%d]: Failure received while updating the  \
                                        ue_priority", erb_data.p_rrm_ue_context->ue_index);
                            }
                        }
                        /*SPR 21773 +-*/
                        p_rrm_rel_erb_ue_ctxt = rrm_ue_find_context(erb_data.p_rrm_ue_context->ue_index, p_rrm_ue_ctxt->cell_index);
                        if(p_rrm_rel_erb_ue_ctxt != RRM_PNULL)
                        {
                            for(count = erb_index; count < p_rrm_rel_erb_ue_ctxt->drb_configured.num_of_list; count++)
                            {
                                RRM_MEMCPY(&(p_rrm_rel_erb_ue_ctxt->drb_configured.erab_item[count]),
                                        &(p_rrm_rel_erb_ue_ctxt->drb_configured.erab_item[count+1]),
                                        sizeof(rrm_ue_erab_setup_item_t));
                            }
                            if (RRM_ZERO != p_rrm_rel_erb_ue_ctxt->drb_configured.num_of_list)
                            {
                                p_rrm_rel_erb_ue_ctxt->drb_configured.num_of_list--;
                            }
                        }
                    }


                }
                /*SPR 21773 Start*/
                else
                {
                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                        "UE CONTEXT NULL for low priority ERB   \
                                        for UE [%d] ", p_rrm_ue_ctxt->ue_index);

                }
                /*SPR 21773 End*/
                /*SPR 19968 End*/
            }
            else
            {
                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                        "No low priority GBR ERB exists.");
                is_low_priority_erb_exist = RRM_FALSE;
                break;
            }
            /* SPR 19968 Start */
        }while((dl_required_gbr > (deallocated_dl_gbr + diff_dl_bitrate)) &&
                /* SPR 19968 End */
                (ul_required_gbr > (deallocated_ul_gbr + diff_ul_bitrate)));

        /* SPR 19968 Start */
        p_g_rrm_cell_ctx->p_rrm_cell_ctx[p_rrm_ue_ctxt->cell_index]->cell_resource_info.used_gbr_limit.used_dl_gbr_limit
            = p_g_rrm_cell_ctx->p_rrm_cell_ctx[p_rrm_ue_ctxt->cell_index]->
            cell_resource_info.used_gbr_limit.used_dl_gbr_limit - deallocated_dl_gbr;
        p_g_rrm_cell_ctx->p_rrm_cell_ctx[p_rrm_ue_ctxt->cell_index]->cell_resource_info.used_gbr_limit.used_ul_gbr_limit
            = p_g_rrm_cell_ctx->p_rrm_cell_ctx[p_rrm_ue_ctxt->cell_index]->
            cell_resource_info.used_gbr_limit.used_ul_gbr_limit - deallocated_ul_gbr;
        /* SPR 19968 End */
        if(RRM_TRUE == is_low_priority_erb_exist)
        {
            /*Bug_721_changes_start*/
            while(RRM_PNULL != (erb_release_list = (rrm_ue_erb_list_t *)ylPopTail(&erb_release_queue)))
                /*Bug_721_changes_end*/
            {
                /*spr 17437 fix start*/
                /*Code Deleted*/
                /*spr 17437 fix stop*/

                //            RRM_MEMCPY(&erb_data, &(erb_release_list->ue_erb), sizeof(rrm_ue_erb_t));

                /*spr 7466 start*/
                /*spr 17437 fix start*/
                if(p_rrm_ue_ctxt->ue_index == erb_release_list->ue_erb.p_rrm_ue_context->ue_index)
                {
                    rrm_rab_remove_node_t *p_rab_remove_list = rrm_mem_get(sizeof(rrm_rab_remove_node_t));
                    /* Coverity_Fix 16295 ++ */
                    if ( PNULL != p_rab_remove_list )
                    {
                        RRM_MEMSET(p_rab_remove_list,RRM_ZERO,sizeof(rrm_rab_remove_node_t));
                        p_rab_remove_list->erab_id  = erb_release_list->ue_erb.erab_id;
                        /* CID 96500 */
                        ylPushTail(&p_rab_removal_ctxt->rabs_to_rem_list,(YLNODE *)p_rab_remove_list);
                        /* CID 96507 */
                        ret_val = RRM_SUCCESS;
                    }
                    else
                    {
                        ret_val = RRM_FAILURE;
                    }
                    /* Coverity_Fix 16295 -- */
                    /*spr 17437 fix stop*/
                }
                else
                {
                    /*spr 17437 fix start*/
                    rrm_ue_rab_remove_record_t *p_ue_rab_release_node = rrm_mem_get(sizeof(rrm_ue_rab_remove_record_t));
                    /* Coverity_Fix 16295 ++ */
                    if ( PNULL != p_ue_rab_release_node)
                    {
                        RRM_MEMSET(p_ue_rab_release_node,RRM_ZERO,sizeof(rrm_ue_rab_remove_record_t));
                        p_ue_rab_release_node->ue_rab_info.ue_idx = erb_release_list->ue_erb.p_rrm_ue_context->ue_index;
                        p_ue_rab_release_node->ue_rab_info.erabs_to_be_released_count = RRM_ONE;
                        p_ue_rab_release_node->ue_rab_info.erabs_to_be_released[RRM_ZERO] = erb_release_list->ue_erb.erab_id;
                        /* CID 96500 */
                        ylPushTail(&p_rab_removal_ctxt->rab_rem_container.ue_rab_list,(YLNODE *)p_ue_rab_release_node);
                        /* CID 96507 */
                        /* SPR-18198 START */
                        ret_val = RRM_SUCCESS;
                        /* SPR-18198 END */
                    }
                    else
                    {
                        ret_val = RRM_FAILURE;
                    }
                    /* Coverity_Fix 16295 -- */
                    /*spr 17437 fix stop*/
                }
                /*spr 17437 fix start*/
                /*Code Deleted*/
                /*spr 17437 fix stop*/

                /*spr 7466 end*/
                /*Bug_721_changes_start*/
                RRM_MEM_FREE(erb_release_list);
                /*Bug_721_changes_end*/
            }
        }
        else
        {
            /*Bug_721_changes_start*/
            /*If erb_release_ind was not sent for any of the RAB and there are any RABs present in erb_release_queue then re-setting is_erb_selected_for_rel to
             * FALSE if any RAB was selected for release*/
            if(ylCount(&erb_release_queue) > RRM_ZERO)
            {
                erb_release_list = (rrm_ue_erb_list_t *)(erb_release_queue.node.next);
                while(RRM_PNULL != erb_release_list)
                {
                    rrm_ue_update_erb_priority_list_flag(erb_release_list);
                    erb_release_list = (rrm_ue_erb_list_t *)erb_release_list->pNode.next;
                }
            }
            /*Bug_721_changes_end*/
            /*If requested RAB is NGBR and no low priority GBR RAB is found*/
            /*then release low priority UE to fulfill the bitrate requirement*/
            /* SPR 19968 Start */
            if((p_erab_to_be_setup->erab_level_qos_params.qci > RRM_GBR_QCI_4) &&
                    (p_erab_to_be_setup->erab_level_qos_params.qci < RRM_MAX_QCI_LIMIT))
                /* SPR 19968 Start */
            {
                /*Bug_721_changes_start*/
                /*Computing priority of UE for which RAB admission has come based*/
                /*on already admitted RABs and RABs coming for admission, so that*/
                /*UE released during ARP is of lower priority than current one and*/
                /*released from vulnerable list only*/
                for(list_index = RRM_ZERO; list_index < p_rrm_ue_ctxt->drb_configured.num_of_list;
                        list_index++)
                {
                    if(current_ue_priority > p_rrm_ue_ctxt->drb_configured.erab_item[list_index].
                            erab_level_qos_params.alloc_and_reten_prior.priority_level)
                    {
                        current_ue_priority = p_rrm_ue_ctxt->drb_configured.erab_item[list_index].
                            erab_level_qos_params.alloc_and_reten_prior.priority_level;
                    }
                }

                for(list_index = RRM_ZERO; list_index < p_erab_setup_item_list->num_of_list;
                        list_index++)
                {
                    if(current_ue_priority > p_erab_setup_item_list->erab_item[list_index].
                            erab_level_qos_params.alloc_and_reten_prior.priority_level)
                    {
                        current_ue_priority = p_erab_setup_item_list->erab_item[list_index].
                            erab_level_qos_params.alloc_and_reten_prior.priority_level;
                    }
                }
                /* SPR 17564 Fix Start */
                if(RRM_TRUE == rrm_is_ue_special_previledge(p_rrm_ue_ctxt))
                    /* SPR 17564 Fix End */
                {
/*spr_22158_changes_start*/
                    ret_val = rrm_ue_get_low_priority_ue(p_rrm_ue_ctxt->cell_index, &ue_index,
/*spr_22158_changes_end*/
                            p_rrm_ue_ctxt->ue_index,
                            RRM_ZERO,
                            RRM_TRUE);
                }
                else
                {
/*spr_22158_changes_start*/
                    ret_val = rrm_ue_get_low_priority_ue(p_rrm_ue_ctxt->cell_index, &ue_index,
/*spr_22158_changes_end*/
                            p_rrm_ue_ctxt->ue_index,
                            current_ue_priority,
                            RRM_TRUE);
                }

                /*Bug_721_changes_end*/
                /*Release lowest priority UE */
                if(ret_val == RRM_SUCCESS)
                {
                    /*spr 17437 fix start*/
                    p_ue_rem_node = rrm_mem_get(sizeof(rrm_ue_remove_node_t));
                    /* SPR 20652 Fix Start */
                    if (RRM_PNULL != p_ue_rem_node)
                    {
                        p_ue_context = rrm_ue_find_context(ue_index,p_rrm_ue_ctxt->cell_index);
                        if(RRM_PNULL != p_ue_context)
                        {
                            RRM_MEMSET(p_ue_rem_node,RRM_ZERO,sizeof(rrm_ue_remove_node_t));
                            p_ue_rem_node->ue_idx = ue_index;
                            p_ue_context->adm_ctrl_rec.is_ue_already_selected = RRM_TRUE;
                            /* SPR 20652 Fix End */
                            /* CID 96500 */
                            ylPushTail(&p_rab_removal_ctxt->rab_rem_container.ue_to_remove_list,(YLNODE *)p_ue_rem_node);
                            /* CID 96507 */
                        }
                        else
                        {
                            RRM_TRACE(g_cellm_log_on_off, p_rrm_uem_facility_name, RRM_ERROR, 
                                    "[UE:%d] context not found", ue_index);
                        }
                    }
                    else
                    {
                        RRM_TRACE(g_cellm_log_on_off, p_rrm_uem_facility_name, RRM_ERROR, 
                                "Failed to allocate memory for ue_rem_node");
                    }
                    /*Code Deleted*/
                    /*spr 17437 fix stop*/
                }

            }
            /*Bug_721_changes_start*/
            else
            {
                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,"ARP failed for requested RAB");
                /* SPR 19968 Start */
                ret_val = RRM_FAILURE;
                /* SPR 19968 End */
            }
            /*Bug_721_changes_end*/

            /* BUG_FIX_837_START */
            while(RRM_PNULL != (erb_release_list = (rrm_ue_erb_list_t *)ylPopTail(&erb_release_queue)))
            {
                RRM_MEM_FREE(erb_release_list);
            }
            /* BUG_FIX_837_END */
        }
        /*Bug_10333_changes_start*/
        while(RRM_PNULL != (erb_release_list = (rrm_ue_erb_list_t *)ylPopTail(&erb_release_queue)))
        {
            RRM_MEM_FREE(erb_release_list);
        }
        /*Bug_10333_changes_end*/

    }
    else
    {
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_WARNING, "RAC failed and \
                requested ERB is not preemption capable.");
        /* SPR 19968 Start */
        ret_val = RRM_FAILURE;
        /* SPR 19968 End */
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;

}

/****************************************************************************
 * Function Name  : rrm_ue_is_gbr_limit_exceeded
 * Inputs         : UE context
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function checks if GBR limit configured on OAM meets
 *                  the total requirement.
 ****************************************************************************/
rrm_return_et
rrm_ue_is_gbr_limit_exceeded
(
 rrm_ue_context_t *p_rrm_ue_context,
 U64              dl_bitrate,
 U64              ul_bitrate,
 /* SPR 19968 Start */
 rrm_ue_erab_setup_item_t *p_erab_to_be_setup,
 U8               type,
 U8               gbr_configd_count,
 U8       gbr_success_count,
 rrm_rab_removal_ctxt_t          *p_rab_removal_ctxt,
 U32 *p_data_transfer_mode,
 U32 *p_num_temp_um_mode,
 U32 *p_num_temp_am_mode
#ifdef ENDC_ENABLED
 ,rrm_bool_et is_endc_ue_x2_up
#endif
 /*SPR 19968 End*/
 )
{
    RRM_UT_TRACE_ENTER();
    rrm_cell_index_t          cell_index;
    rrm_return_et             ret_val = RRM_SUCCESS;
    U8                        erab_id = RRM_ZERO;
    U64                       old_dl_bitrate = RRM_ZERO;
    U64                       old_ul_bitrate = RRM_ZERO;
    U8                        index = RRM_ZERO;
    /* SPR 19968 Start */
    U8 qci = RRM_ZERO ;
    U8 gbr_dl_prbs_released = RRM_ZERO;
    U8 gbr_ul_prbs_released = RRM_ZERO;
    U8 ngbr_dl_prbs_released = RRM_ZERO;
    U8 ngbr_ul_prbs_released = RRM_ZERO;
    /*SPR 19968 End*/

    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context (p_rrm_ue_context->cell_index);
    cell_index = p_rrm_ue_context->cell_index;
    U8 data_transfer_mode_cnfgd = RRM_ZERO;

    U16 dl_gbr_limit = rrm_cellm_get_dl_gbr_limit(cell_index);
    U16 ul_gbr_limit = rrm_cellm_get_ul_gbr_limit(cell_index);

    U64 used_dl_bitrate = rrm_cellm_get_used_dl_bitrate(cell_index);
    U64 used_ul_bitrate = rrm_cellm_get_used_ul_bitrate(cell_index);
    /* Coverity 73979,74004 Fix Start */
    U64 diff_dl_bitrate = (((U64)dl_gbr_limit) * RRM_MEGA) - used_dl_bitrate;
    U64 diff_ul_bitrate = (((U64)ul_gbr_limit) * RRM_MEGA) - used_ul_bitrate;
    /* Coverity 73979,74004 Fix End */

    /* SPR 19968 Start */
    erab_id = p_erab_to_be_setup->erab_id;
    qci = p_erab_to_be_setup->erab_level_qos_params.qci;
    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
            "RabId[%d] gbr_limit [DL:%u][UL:%u] used_bitrate [DL:%llu][UL:%llu] diff_bitrate [DL:%llu][UL:%llu]",
            erab_id, dl_gbr_limit, ul_gbr_limit,
            used_dl_bitrate, used_ul_bitrate,
            diff_dl_bitrate, diff_ul_bitrate);
    data_transfer_mode_cnfgd = rrm_get_data_transfer_mode_configd(qci,
            p_rrm_ue_context->cell_index);
    /*SPR 19968 End*/

    if(type == RRM_ERB_REQ_TYPE_SETUP)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, "Request is for ERB setup");
        if((diff_dl_bitrate >= dl_bitrate) && (diff_ul_bitrate >= ul_bitrate))
        {
            rrm_cellm_set_used_dl_bitrate_during_erb_setup_modify(cell_index, dl_bitrate);
            rrm_cellm_set_used_ul_bitrate_during_erb_setup_modify(cell_index, ul_bitrate);
            /*SPR 19968 Start*/
            ret_val = rac_admit_gbr_bearer(p_rrm_ue_context, ul_bitrate, dl_bitrate, 
                    *p_erab_to_be_setup, ngbr_dl_prbs_released, ngbr_ul_prbs_released,
                    gbr_dl_prbs_released, gbr_ul_prbs_released,
                    /* SPR 22316 erab fix start */
                    RRM_ERB_REQ_TYPE_SETUP, gbr_configd_count, gbr_success_count, p_rab_removal_ctxt
#ifdef ENDC_ENABLED
					,is_endc_ue_x2_up
#endif
					);
                    /* SPR 22316 erab fix end */
            /*spr 19968 fix stop*/	
            if(RRM_FAILURE==ret_val)
            {
				RRM_UT_TRACE_EXIT();
                return ret_val;
            }
            else
            {

                ret_val = rrm_ue_check_config_gbr_drbs(p_rrm_ue_context,
                        gbr_configd_count,
                        gbr_success_count);
                if(RRM_FAILURE==ret_val)
                {
                    /* SPR 19968 Start */
                    rrm_raise_event_erab_adm_rej(p_erab_to_be_setup->erab_id, RRM_EVENT_LACK_OF_RESOURCES);
                    /* SPR 19968 End */
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR, "\nMaximum Number of GBR count Reached");
                    RRM_UT_TRACE_EXIT();
                    return ret_val;
                }
                else
                {
                    ret_val = rrm_uem_decide_am_um_mode(p_rrm_ue_context,
                            data_transfer_mode_cnfgd,
                            p_data_transfer_mode,
                            p_num_temp_um_mode,
                            p_num_temp_am_mode,
                            /* SPR 19968 Start */
                            *p_erab_to_be_setup);
                    /* SPR 19968 End */
                    if (RRM_FAILURE == ret_val)
                    {

                        ret_val = rrm_ue_max_rab_preemption(p_rrm_ue_context,p_erab_to_be_setup,p_rab_removal_ctxt,type); 
                        if(RRM_FAILURE== ret_val)
                        {
                            /* Raise Event RRM_EVENT_LOCAL_EVENT_UE_CAPABILITY_FGI_REJECT Start*/
                            rrm_raise_event_ue_cap_fgi_reject();/*SPR 17777 +-*/
                            /* Raise Event RRM_EVENT_LOCAL_EVENT_UE_CAPABILITY_FGI_REJECT End*/

                            /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT Start*/
                            RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name, RRM_DETAILED, "Failure returned from rrm_uem_decide_am_um_mode");
                            /* SPR 19968 Start */
                            rrm_raise_event_erab_adm_rej(p_erab_to_be_setup->erab_id, RRM_EVENT_UE_CAPABILITY_FGI_REJECT);
                            /* SPR 19968 End */
                            /*SPR 17777 +-*/
                            /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT End*/
                            if (RRM_TRUE == (rrm_bool_et)p_cell_context->sps_on)
                            {
                                /* SPR 19968 Start */
                                if (p_erab_to_be_setup->erab_id == p_rrm_ue_context->ue_sps_data.sps_erab_id)
                                    /* SPR 19968 End */
                                {
                                    /* Parameters need to be reset in case if
                                     ** this bearer becomes SPS bearer but RBC
                                     ** fails due to some other reason after that*/
                                    p_rrm_ue_context->ue_sps_data.sps_erab_id = RRM_OUT_OF_RANGE;
                                    p_rrm_ue_context->ue_sps_data.is_sps_erab_established = RRM_FALSE;
                                    p_cell_context->sps_ues_count--;
                                }
                            }
                            /*Coverity_fix_end_54963*/
                            RRM_UT_TRACE_EXIT();
                            return ret_val;
                        }

                        else
                        {
                            /* SPR 19968 Start */
                            if(AM_MODE == data_transfer_mode_cnfgd)
                            {
                                (*p_num_temp_am_mode)++;
                            }
                            else
                            {
                                (*p_num_temp_um_mode)++;
                            }
                            /* SPR 19968 End */

                        }
                        /*spr 17437 fix stop*/

                    }
                    /*End FGI validation block*/
                }
            }
            /*SPR 19968 End*/
        }
        else
        {
            /* SPR-18198 START */
            U64 dl_gbr_limit_bitrate = (U64)dl_gbr_limit * (U64)RRM_MEGA;
            U64 ul_gbr_limit_bitrate = (U64)ul_gbr_limit * (U64)RRM_MEGA;
            if(dl_gbr_limit_bitrate < dl_bitrate)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR, "Required dl_bitrate exceeds the limit");
                return RRM_FAILURE;
            }
            if(ul_gbr_limit_bitrate < ul_bitrate)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR, "Required ul_bitrate exceeds the limit");
                return RRM_FAILURE;
            }
#ifdef ENDC_ENABLED
			if(is_endc_ue_x2_up)
			{
				return RRM_FAILURE;
			}
#endif
            /* SPR-18198 END */
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED, "Applying ARP due to GBR limit");
            /*spr 17437 fix start*/
            /*SPR 19968 Start*/
            ret_val = rrm_ue_apply_arp_for_gbr_limit(p_rrm_ue_context, used_dl_bitrate,
                    used_ul_bitrate, dl_bitrate, ul_bitrate, p_erab_to_be_setup,type,p_rab_removal_ctxt,p_data_transfer_mode);
            /*SPR 19968 End*/

            /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT START */
            if(RRM_FAILURE == ret_val)
            {
                if (diff_dl_bitrate < dl_bitrate)
                {
                    /* SPR 19968 Start */
                    rrm_raise_event_erab_adm_rej(p_erab_to_be_setup->erab_id, RRM_EVENT_GBR_DL_CAPACITY_REJECT);
                    /* SPR 19968 End */
                    /*SPR 17777 +-*/
                }
                if (diff_ul_bitrate < ul_bitrate)
                {
                    /* SPR 19968 Start */
                    rrm_raise_event_erab_adm_rej(p_erab_to_be_setup->erab_id, RRM_EVENT_GBR_UL_CAPACITY_REJECT);
                    /* SPR 19968 End */
                    /*SPR 17777 +-*/
                }
            }
            /* SPR 19968 Start */
            else
            {

                rrm_cellm_set_used_dl_bitrate_during_erb_setup_modify(cell_index, dl_bitrate);
                rrm_cellm_set_used_ul_bitrate_during_erb_setup_modify(cell_index, ul_bitrate);
            }
            /* SPR 19968 End */
            /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT END*/

        }
    }
    /*Coverity_fix_start_54963*/
    else if(type == RRM_ERB_REQ_TYPE_MODIFY)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, "Request is for ERB modify");
        for(index = RRM_ZERO; index < p_rrm_ue_context->drb_configured.num_of_list; index++)
        {
            if(erab_id == p_rrm_ue_context->drb_configured.erab_item[index].erab_id)
            {
                old_dl_bitrate = p_rrm_ue_context->drb_configured.erab_item[index].
                    erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_dl;
                old_ul_bitrate = p_rrm_ue_context->drb_configured.erab_item[index].
                    erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_ul;
            }
        }
        if(((diff_dl_bitrate + old_dl_bitrate) >= dl_bitrate) &&
                ((diff_ul_bitrate + old_ul_bitrate) >= ul_bitrate))
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF, 
                    "[ERB modify] Available GBR bearer bitrate [DL:%llu][UL:%llu].",
                    diff_dl_bitrate + old_dl_bitrate, diff_ul_bitrate + old_ul_bitrate);
            /* SPR-18198 START */
            if(dl_bitrate > old_dl_bitrate)
            {
            U64 modified_dl_bitrate = dl_bitrate - old_dl_bitrate;
            rrm_cellm_set_used_dl_bitrate_during_erb_setup_modify(cell_index, modified_dl_bitrate);
            }
            if(ul_bitrate > old_ul_bitrate)
            {
                U64 modified_ul_bitrate = ul_bitrate - old_ul_bitrate;
                rrm_cellm_set_used_ul_bitrate_during_erb_setup_modify(cell_index, modified_ul_bitrate);
            }
            /* SPR-18198 END */
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, "Applying ARP due to GBR limit");
            /*SPR 19968 start*/
            ret_val = rrm_ue_apply_arp_for_gbr_limit(p_rrm_ue_context, used_dl_bitrate - old_dl_bitrate,
                    used_ul_bitrate - old_ul_bitrate, dl_bitrate, ul_bitrate, p_erab_to_be_setup,type,p_rab_removal_ctxt, p_data_transfer_mode); /*13584 rel1.3*/
            /*SPR 19968 end*/
        }
    }
    /*Coverity_fix_end_54963*/
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/*GBR LIMIT END */



/*csg start*/
/****************************************************************************
 * Function Name  : rrm_get_low_priority_non_csg_ue
 * Inputs         : UE context
 * Outputs        : ue_index
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function finds the lowest priority
                    non csg ue
 ****************************************************************************/
rrm_return_et
rrm_get_low_priority_non_csg_ue(
        rrm_ue_index_t *p_ue_index, /*outgoing ue_index*/
        rrm_ue_index_t ue_index_current /*ue_index of current ue*/

        )   
{
    rrm_return_et       return_value = RRM_SUCCESS;
    U8          list_index = RRM_NULL;
    U8          check_list = RRM_MAX_UE_PRIORITY;  /* =30 (Max capacity of priority List) */
    rrm_ue_priority_node_t  *p_temp = RRM_PNULL;
    rrm_bool_et         valid_ue_entry_found = RRM_FALSE;

    RRM_UT_TRACE_ENTER();
    if(RRM_PNULL == p_ue_index)
    {
        return_value = RRM_FAILURE;
    }
    else
    {
        for(; list_index < check_list ; list_index++)
        {
            p_temp = (rrm_ue_priority_node_t *)ue_priority_list[list_index].node.next;
            while(p_temp != RRM_NULL)
            {
                if(p_temp->p_rrm_ue_context->ue_index != ue_index_current)
                {
                    if(p_temp->p_rrm_ue_context->rrm_csg_membership.rrm_csg_membership_status
                            == RRM_NOT_MEMBER)
                    {
                        *p_ue_index = p_temp->p_rrm_ue_context->ue_index;
                        valid_ue_entry_found = RRM_TRUE;
                        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED,
                                "lowest priority non csg ue found : [UE:%d]", p_temp->p_rrm_ue_context->ue_index);
                        break;
                    }
                }
                p_temp = (rrm_ue_priority_node_t *)(p_temp->pNode.next);
            }
        }
    }
    if(!valid_ue_entry_found)
    {
        return_value = RRM_FAILURE;
    }
    RRM_UT_TRACE_EXIT();
    return(return_value);
}

/****************************************************************************
 * Function Name  : rrm_get_low_priority_csg_ue
 * Inputs         : UE context
 * Outputs        : ue_index
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function finds the lowest priority 
                    csg ue
 ****************************************************************************/
rrm_return_et
rrm_get_low_priority_csg_ue(
        rrm_ue_index_t *p_ue_index,  /*outgoing ue index*/
        rrm_ue_index_t ue_index_current  /*ue index of curent ue */
        )   
{
    rrm_return_et       return_value = RRM_SUCCESS;
    U8          list_index = RRM_NULL;
    U8          check_list = RRM_MAX_UE_PRIORITY; /* =30 (Max capacity of priority List) */
    rrm_ue_priority_node_t  *p_temp = RRM_PNULL;
    rrm_bool_et         valid_ue_entry_found = RRM_FALSE;

    RRM_UT_TRACE_ENTER();
    if(RRM_PNULL == p_ue_index)
    {
        return_value = RRM_FAILURE;
    }
    else
    {
        for(; list_index < check_list ; list_index++)
        {
            p_temp = (rrm_ue_priority_node_t *)ue_priority_list[list_index].node.next;
            while(p_temp != RRM_NULL)
            {
                /*current ue inserted in priority list should be skipped */
                if(p_temp->p_rrm_ue_context->ue_index != ue_index_current)
                {
                    if(p_temp->p_rrm_ue_context->rrm_csg_membership.rrm_csg_membership_status
                            == RRM_MEMBER)
                    {
                        *p_ue_index = p_temp->p_rrm_ue_context->ue_index;
                        valid_ue_entry_found = RRM_TRUE;
                        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED,
                                "lowest priority csg ue found : [UE:%d]", p_temp->p_rrm_ue_context->ue_index);
                        break;
                    }
                }
                p_temp = (rrm_ue_priority_node_t *)(p_temp->pNode.next);
            }
        }
    }
    if(!valid_ue_entry_found)
    {
        return_value = RRM_FAILURE;
    }
    RRM_UT_TRACE_EXIT();
    return(return_value);
}

/****************************************************************************
 * Function Name  : rrm_check_target_cell_access_mode_utran
 * Inputs         : phy_cell_id, access mode
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function checks the access mode of the cell
 ****************************************************************************/
rrm_return_et
rrm_check_target_cell_access_mode_utran(
        U16  phy_cell_id_target_cell,
        rrm_oam_access_mode_et prefered_cell_access_mode,
        rrm_cell_index_t cell_index,
        rrm_csg_membership_status_et ue_mbrshp_status_trget_cell
        )
{
   U8 index = RRM_ZERO;
   rrm_return_et ret_val = RRM_FAILURE;
   RRM_UT_TRACE_ENTER();
   inter_rat_ncl_t *p_ncl_params = 
           rrm_cellm_get_inter_rat_ncl_info(cell_index);
   for(index = RRM_ZERO; index < p_ncl_params->num_valid_utran_freq_cell;
              index++)
   {
       if(phy_cell_id_target_cell == p_ncl_params->utran_freq_cells[index].pci)
       {
           if(p_ncl_params->utran_freq_cells[index].bitmask & 
                RRM_OAM_UTRAN_FREQ_CELL_ACCESS_MODE_PRESENT)
           {
              /* Fix for CSR 58972 start*/
              if ((prefered_cell_access_mode == RRM_OAM_ACCESS_MODE_OPEN) || (prefered_cell_access_mode == RRM_OAM_ACCESS_MODE_OPEN_FEMTO))
              /* Fix for CSR 58972 end*/
              {
                    if(((S32)prefered_cell_access_mode == p_ncl_params->utran_freq_cells[index].cell_access_mode) ||
                          (RRM_OAM_ACCESS_MODE_HYBRID == p_ncl_params->utran_freq_cells[index].cell_access_mode))
                  {
                      ret_val = RRM_SUCCESS;
                  }
              }
              else if(prefered_cell_access_mode == RRM_OAM_ACCESS_MODE_CLOSE)
              {
                  /*For closed cell ho the ue only if the ue is member of it*/
                    if(((S32)prefered_cell_access_mode == p_ncl_params->utran_freq_cells[index].cell_access_mode) &&
                      (ue_mbrshp_status_trget_cell == RRM_MEMBER))         
                  {
                      ret_val = RRM_SUCCESS;
                  }
                  else if(RRM_OAM_ACCESS_MODE_HYBRID == p_ncl_params->utran_freq_cells[index].cell_access_mode)
                  {
                      ret_val = RRM_SUCCESS;
                  }
              }
           }
           break;
       }
   }
   RRM_UT_TRACE_EXIT();
   return ret_val;
}
/****************************************************************************
 * Function Name  : rrm_check_target_cell_access_mode_eutran
 * Inputs         : phy_cell_id, access mode
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function checks the access mode of the cell
 ****************************************************************************/
rrm_return_et
rrm_check_target_cell_access_mode_eutran(
        U16  phy_cell_id_target_cell,
        rrm_oam_access_mode_et prefered_cell_access_mode,
        rrm_cell_index_t  cell_index,
        rrm_csg_membership_status_et ue_mbrshp_status_trget_cell
        )
{
   U8 index = RRM_ZERO;
   rrm_return_et ret_val = RRM_FAILURE;
   RRM_UT_TRACE_ENTER();
   lte_ncl_t *p_lte_ncl_params = 
           rrm_cellm_get_lte_ncl_info(cell_index);
    /*coverity 29574, rel2.0.1, aditya*/
   if(RRM_PNULL != p_lte_ncl_params)
   {
   for(index = RRM_ZERO; index < p_lte_ncl_params->num_valid_inter_freq_cell;
              index++)
   {
       if(phy_cell_id_target_cell == p_lte_ncl_params->inter_freq_cells[index].phy_cell_id)
       {
           if(p_lte_ncl_params->inter_freq_cells[index].bitmask & 
                RRMCM_RMIF_CELL_ACCESS_MODE_PRESENT)
           {
              /* Fix for CSR 58972 start*/
              if((prefered_cell_access_mode == RRM_OAM_ACCESS_MODE_OPEN) || (prefered_cell_access_mode == RRM_OAM_ACCESS_MODE_OPEN_FEMTO))
              /* Fix for CSR 58972 end*/
              {
                        if(((S32)prefered_cell_access_mode == p_lte_ncl_params->inter_freq_cells[index].cell_access_mode) ||
                          (RRM_OAM_ACCESS_MODE_HYBRID == p_lte_ncl_params->inter_freq_cells[index].cell_access_mode))
                  {
                      ret_val = RRM_SUCCESS;
                  }
              }
              else if(prefered_cell_access_mode == RRM_OAM_ACCESS_MODE_CLOSE)
              {
                  /*In case of closed cell ho only if ue is member of it*/
                        if(((S32)prefered_cell_access_mode == p_lte_ncl_params->inter_freq_cells[index].cell_access_mode) &&
                      (ue_mbrshp_status_trget_cell == RRM_MEMBER)) 
                  {
                      ret_val = RRM_SUCCESS;
                  }
                  else if(RRM_OAM_ACCESS_MODE_HYBRID == p_lte_ncl_params->inter_freq_cells[index].cell_access_mode)
                  {
                      ret_val = RRM_SUCCESS;
                  }
              }
           }
           break;
       }
   }
   }
   else
   {
         RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
	                       "Memory allocation to p_lte_ncl_params failed");
	 ret_val = RRM_FAILURE;
   }
   RRM_UT_TRACE_EXIT();
   return ret_val;
}


/*spr 7996 start*/
/****************************************************************************
 * Function Name  : rrm_release_ue
 * Inputs         : p_ue_context
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function releases a UE
 ****************************************************************************/
rrm_return_et
rrm_release_ue(
 rrm_ue_context_t   *p_ue_context
)
{
    RRM_UT_TRACE_ENTER();
    rrm_return_et ret_val;
    ret_val= rrm_build_and_send_ue_connection_release_ind(
            p_ue_context->ue_index,
            /* SPR_8345_fix_start */
            RRM_NAS_CAUSE_TYPE,
            RRM_NAS_CSG_SUBSCRIPTION_EXPIRY,
            /* SPR_8345_fix_end */
            RRM_ZERO,
            RRM_PNULL,
            /* SPR_10729_fix */
            p_ue_context->cell_index);

    if (ret_val == RRM_FAILURE)
    {
        RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_ERROR,
                "Error in sending UE release indication");
    }
    else
    {
        if(RRM_FAILURE == rrm_ue_resource_clean_up(p_ue_context))
        {
            RRM_TRACE(g_uem_log_on_off,
                    p_g_rrm_uem_facility_name,
                    RRM_DETAILED,
                    "ue_connection_release_ind sent, \
                    Failed to clean up ue resources");
            ret_val = RRM_FAILURE;

        }
        else
        {
            /* SPR_8345_fix_start */
            RRM_TRACE(g_uem_log_on_off,
                    p_g_rrm_uem_facility_name,
                    RRM_DETAILED,
                    "ue_connection_release_ind sent for [UE:%d] due to CSG subscription expiry",
                    p_ue_context->ue_index);
            /* SPR_8345_fix_end */
            ret_val = RRM_SUCCESS;
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/*spr 7996 end*/

/****************************************************************************
 * Function Name  : rrm_handover_ue_to_best_cell
 * Inputs         : p_ue_context_to_ho,target_cell_access_mode
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function hands over a ue to a closed/open cell
 ****************************************************************************/
rrm_return_et
rrm_handover_ue_to_best_cell(
        rrm_ue_context_t   *p_ue_context_to_ho
        /*SPR 17777 +-*/
        )
{
    rrm_return_et  ret_val = RRM_SUCCESS;
    /*spr 7996 start*/
    rrm_ue_timer_buf_t timer_buff;
        /*SPR 17777 +-*/
    U32 timer_duration = RRM_UE_RELASE_GUARD_TIMER;
    /*spr 7996 end*/

    RRM_UT_TRACE_ENTER();
    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
            "Sending meas config req for [UE:%d]",
            p_ue_context_to_ho->ue_index);

    ret_val = rrm_build_and_send_meas_config_req(
            p_ue_context_to_ho,MEAS_TYPE_HO);
    RRM_TRACE(g_uem_log_on_off,
            p_g_rrm_uem_facility_name,
            RRM_BRIEF,
            "rrm_build_and_send_meas_config_req:%s",
            ret_val?"SUCCESS":"FAILED");
    /*spr 7996 start*/
    /* Coverity_ID : 54437 */
    timer_buff.timer_type = RRM_UE_CSG_GUARD_TIMER;
    timer_buff.ue_index = p_ue_context_to_ho->ue_index;
    timer_buff.cell_index = p_ue_context_to_ho->cell_index;
    /* Review_comment_for_SPR_7996_nd_8345_start stored csg subscription
       expiry timer in ue context */
    /* CSR-58333-fix start*/
    if(RRM_NULL == (p_ue_context_to_ho->csg_subscription_expiry_timer = rrm_ue_start_timer (
                    (timer_duration*1000),
                    &timer_buff,
                    sizeof(rrm_ue_timer_buf_t),
                    RRM_FALSE)))
        /* CSR-58333-fix end*/
    {
        RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_ERROR,
                "Failed to start CSG subscription expiry timer for [UE:%d]",
                p_ue_context_to_ho->ue_index);
        ret_val = rrm_release_ue(p_ue_context_to_ho);
    }
    else
    {
        /* SPR_8345_fix_start */
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                "Timer started[%p] for CSG subscription expiry for [UE:%d]",
                p_ue_context_to_ho->csg_subscription_expiry_timer, p_ue_context_to_ho->ue_index);
        /* SPR_8345_fix_end */
    }
    /*spr 7996 end*/
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/*CA HARDENING CHANGES REPORT EVENT*/
/*start tgt_cell_ranking*/
/*SPR 18178 Start */
/*Code Removed */
/*SPR 18178 End */
/****************************************************************************
 * Function Name  : rrm_get_required_ue_capacity
 * Inputs         : ue context
 * Outputs        : percentage of resource required by ue
 * Returns        : percentage of resource required by ue
 * Description    : This fucntion returns percentage of resource required by ue
 ****************************************************************************/
U8
rrm_get_required_ue_capacity(
      rrm_ue_context_t *p_rrm_ue_ctxt
      )
{
    RRM_UT_TRACE_ENTER();
    U8 req_ue_prb = RRM_ZERO;
    U8 dl_usable_bw_prb = RRM_ZERO;
    U8 ul_usable_bw_prb = RRM_ZERO;
    U64 ue_thp_ul = RRM_ZERO;
    /*SPR 22683 Fix Start*/ 
    U64 ue_thp_dl = RRM_ZERO;
    /*SPR 22683 Fix Stop*/ 

    U64 cell_thp_ul = RRM_ZERO;
    U64 cell_thp_dl = RRM_ZERO;
    U8 ul_prb_usage_by_ue = RRM_ZERO;
    U8 dl_prb_usage_by_ue = RRM_ZERO;

    ue_thp_ul = p_rrm_ue_ctxt->ue_ecn_list.max_ul_thp;
    /*SPR 22683 Fix Start*/ 
    ue_thp_dl = p_rrm_ue_ctxt->ue_ecn_list.max_dl_thp;
    /*SPR 22683 Fix Stop*/ 


    cell_thp_dl = rrm_cellm_get_cell_thp_dl(p_rrm_ue_ctxt->cell_index);
    cell_thp_ul = rrm_cellm_get_cell_thp_ul(p_rrm_ue_ctxt->cell_index);

    dl_usable_bw_prb = rrm_cellm_get_dl_gbr_prb_in_use(p_rrm_ue_ctxt->cell_index) +
        rrm_cellm_get_dl_ngbr_prb_in_use(p_rrm_ue_ctxt->cell_index) +
        rrm_cellm_get_dl_allocated_gbr_prb(p_rrm_ue_ctxt->cell_index) +
        rrm_cellm_get_dl_allocated_ngbr_prb(p_rrm_ue_ctxt->cell_index);
    ul_usable_bw_prb = rrm_cellm_get_ul_gbr_prb_in_use(p_rrm_ue_ctxt->cell_index) +
        rrm_cellm_get_ul_ngbr_prb_in_use(p_rrm_ue_ctxt->cell_index) +
        rrm_cellm_get_ul_allocated_gbr_prb(p_rrm_ue_ctxt->cell_index) +
        rrm_cellm_get_ul_allocated_ngbr_prb(p_rrm_ue_ctxt->cell_index);

    if(RRM_ZERO != cell_thp_dl)
    {
        /*SPR 22683 Fix Start*/
        dl_prb_usage_by_ue = (ue_thp_dl*dl_usable_bw_prb)/cell_thp_dl;
        /*SPR 22683 Fix Stop*/

    }

    if(RRM_ZERO != cell_thp_ul)
    {
        ul_prb_usage_by_ue = (ue_thp_ul*ul_usable_bw_prb)/cell_thp_ul;
    }

    req_ue_prb = (dl_prb_usage_by_ue > ul_prb_usage_by_ue)? (dl_prb_usage_by_ue):(ul_prb_usage_by_ue);

    RRM_UT_TRACE_EXIT();
    return req_ue_prb;
}
/* SPR 17529 Fix Start */
/****************************************************************************
 * Function Name  : rrm_ue_convert_actual_qoffset_value
 * Inputs         : rat_type
 *                  offset_freq
 * Outputs        : NONE
 * Returns        : actual_offset_freq
 * Description    : This function shall return actual offset frequency
 ****************************************************************************/
S8
rrm_ue_convert_actual_qoffset_value
(
    rrm_rat_type_et         rat_type,
    U8                      offset_freq
)
{
    S8        actual_offset_freq = RRM_ZERO;

    RRM_UT_TRACE_ENTER();
    
    if (RRM_EUTRAN_CELL == rat_type)
    {
        switch(offset_freq)
        {
            case RRM_OAM_Q_OFFESET_RANGE_DB_24:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_24_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_22:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_22_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_20:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_20_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_18:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_18_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_16:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_16_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_14:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_14_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_12:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_12_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_10:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_10_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_8:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_8_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_6:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_6_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_5:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_5_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_4:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_4_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_3:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_3_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_2:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_2_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB_1:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_MINUS_1_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB0:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_0_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB1:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_1_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB2:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_2_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB3:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_3_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB4:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_4_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB5:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_5_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB6:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_6_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB8:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_8_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB10:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_10_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB12:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_12_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB14:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_14_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB16:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_16_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB18:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_18_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB20:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_20_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB22:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_22_DB;
                break;
            case RRM_OAM_Q_OFFESET_RANGE_DB24:
                actual_offset_freq = RRM_EUTRAN_FREQ_OFFSET_PLUS_24_DB;
                break;
        }
    }
    RRM_UT_TRACE_EXIT();
    return actual_offset_freq;
}
/****************************************************************************
 * Function Name  : rrm_ue_rank_calc_for_neighbor_cells 
 * Inputs         : rsrx_value
 *                  freq_specific_offset
 *                  cell_specific_offset
 *                  hysteresis
 * Outputs        : Cell rank
 * Returns        : Cell rank
 * Description    : This function shall calculate the cell rank using below
 *                  formula.
 *                  RANK = Mn + Ofn + Ocn –Hys (EUTRAN cells)
 *                  Mn: RSRP/RSRQ
 *                  Ofn: offsetFreq as defined within measObjectEUTRA 
 *                  Ocn: cellIndividualOffset as defined within measObjectEUTRA
 *                  Hys: hysteresis as defined within reportConfigEUTRA
 ****************************************************************************/
S8
rrm_ue_rank_calc_for_neighbor_cells
(
    U8                  rsrx_value,
    S8                  offset_freq,
    U8                  hysteresis,
    rrm_rat_type_et     rat_type
)
{
    S16          final_rank_for_neighbor_cell = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
    if(RRM_EUTRAN_CELL == rat_type)
    {
        final_rank_for_neighbor_cell = rsrx_value + offset_freq - hysteresis;
    }
    else
    {
        final_rank_for_neighbor_cell = rsrx_value + offset_freq;
    }
    RRM_UT_TRACE_EXIT();
    return final_rank_for_neighbor_cell;
}
/****************************************************************************
 * Function Name  : rrm_ue_get_neighbor_cell_total_ofn_ocn_values
 * Inputs         : p_rrm_ue_context
 *                  target_pci
 * Outputs        : p_freq_specific_offset
 *                  p_cell_specific_offset
 * Returns        : RRM_FAILURE/RRM_SUCCESS
 * Description    : This function fetch q_offset and CIO offset for neighbor
 *                  cells required for algorithm to get the rank for a Cell
 ****************************************************************************/
rrm_return_et
rrm_ue_get_neighbor_cell_total_ofn_ocn_values
(
    rrm_ue_context_t    *p_rrm_ue_context,
    U16                 target_pci,
    U16                 target_earfcn,
    rrm_rat_type_et     rat_type,
/*SPR 18178 Start */
    S8                  *p_total_of_ofn_and_ocn,
    U8                  *p_tgt_cell_capacity
/*SPR 18178 End */
)
{
    U8                          intra_freq_index = RRM_ZERO;
    U8                          inter_freq_index = RRM_ZERO;
    U8                          inter_rat_utran_index = RRM_ZERO;
    U8                          inter_rat_geran_index = RRM_ZERO;
    ncl_params_t                *p_ncl_params   = RRM_PNULL;
    rrm_cell_context_t          *p_rrm_cell_context = RRM_PNULL;
    rrm_return_et                   ret_val = RRM_FAILURE;

    /*SPR 18178 Start */
    U8 tgt_cell_capacity_ul = RRM_MAX_AVLB_CAPCTY;
    U8 tgt_cell_capacity_dl = RRM_MAX_AVLB_CAPCTY;
    *p_total_of_ofn_and_ocn = RRM_ZERO;
    *p_tgt_cell_capacity = RRM_MAX_AVLB_CAPCTY;
    /*SPR 18178 End*/
    RRM_UT_TRACE_ENTER();
     
    p_ncl_params = rrm_cellm_get_ncl_info(p_rrm_ue_context->cell_index);
    p_rrm_cell_context = rrm_cellm_get_cell_context(p_rrm_ue_context->cell_index);

    /*Coverity 99668 Fix Start */
    if(RRM_PNULL == p_rrm_cell_context)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "Cell not Found");
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    /*Coverity 99668 Fix End */

    switch(rat_type)
    {
        case RRM_GERAN_CELL:
            {
            if(RRMCM_RMIF_INTER_RAT_NCL_PRESENT & p_ncl_params->bitmask)
            {
                for(inter_rat_geran_index = RRM_ZERO;
                    inter_rat_geran_index < p_ncl_params->inter_rat_ncl.num_valid_geran_cell;
                    inter_rat_geran_index++)
                {
                    if((target_pci == p_ncl_params->inter_rat_ncl.geran_freq_cells[inter_rat_geran_index].pci) &&
                       (target_earfcn == p_ncl_params->inter_rat_ncl.geran_freq_cells[inter_rat_geran_index].bcch_arfcn))
                    {
                        *p_total_of_ofn_and_ocn = p_ncl_params->inter_rat_ncl.geran_freq_cells[inter_rat_geran_index].offset_freq;
                        ret_val = RRM_SUCCESS;
                            /*SPR 18178 Start */
                            break;
                            /*SPR 18178 End */
                        }
                        /*SPR 18178 Start */
                        /* Code removed */
                        /*SPR 18178 End */
                    }
                }
            }
            break;

        case RRM_UTRAN_CELL:
            {
            if(RRMCM_RMIF_INTER_RAT_NCL_PRESENT & p_ncl_params->bitmask)
            {
                for(inter_rat_utran_index = RRM_ZERO;
                    inter_rat_utran_index < p_ncl_params->inter_rat_ncl.num_valid_utran_freq_cell;
                    inter_rat_utran_index++)
                {
                    if((target_pci == p_ncl_params->inter_rat_ncl.utran_freq_cells[inter_rat_utran_index].pci) &&
                       (target_earfcn == p_ncl_params->inter_rat_ncl.utran_freq_cells[inter_rat_utran_index].uarfcndl))
                    {
                        *p_total_of_ofn_and_ocn = p_ncl_params->inter_rat_ncl.utran_freq_cells[inter_rat_utran_index].offset_freq;
                        ret_val = RRM_SUCCESS;
                            /*SPR 18178 Start */
                            break;
                            /*SPR 18178 End */
                        }
                        /*SPR 18178 Start */
                        /* Code removed */
                        /*SPR 18178 End */
                    }
                }
            }
            break;

        case RRM_EUTRAN_CELL:
            {
                /* Intra freq cells ofn and cio store for algo */
                if(target_earfcn == p_rrm_cell_context->ran_info.rf_params.rf_configurations.dl_earfcn)
                {
                    for(intra_freq_index = RRM_ZERO;
                        intra_freq_index < p_ncl_params->lte_ncl.num_valid_intra_freq_cell;
                        intra_freq_index++)
                    {
                        if(target_pci == p_ncl_params->lte_ncl.intra_freq_cells[intra_freq_index].phy_cell_id)
                        {
                            *p_total_of_ofn_and_ocn = (rrm_ue_convert_actual_qoffset_value
                                                           (RRM_EUTRAN_CELL,
                                                           p_ncl_params->lte_ncl.intra_freq_cells[intra_freq_index].q_offset) +
                                                       rrm_ue_convert_actual_qoffset_value
                                                           (RRM_EUTRAN_CELL,
                                                           p_ncl_params->lte_ncl.intra_freq_cells[intra_freq_index].cio));

                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                      "Intra freq Cell PCI [ %d ] Total of CIO and QOFFSET [ %d ]",
                                       p_ncl_params->lte_ncl.intra_freq_cells[intra_freq_index].phy_cell_id,
                                       *p_total_of_ofn_and_ocn);
                            ret_val = RRM_SUCCESS;
                            /*SPR 18178 Start */
                            if(p_ncl_params->lte_ncl.intra_freq_cells[intra_freq_index].resrc_report.bitmask 
                                    & RRM_COMP_AVL_CAP_PERIODIC_REPORT_PRESENT)
                            {
                                tgt_cell_capacity_ul =  p_ncl_params->lte_ncl.intra_freq_cells[intra_freq_index].
                                    resrc_report.comp_avl_cap_grp.ul.cell_cap_val;

                                tgt_cell_capacity_dl =  p_ncl_params->lte_ncl.intra_freq_cells[intra_freq_index].
                                    resrc_report.comp_avl_cap_grp.dl.cell_cap_val;
                            }
                            *p_tgt_cell_capacity = RRM_GET_MIN(tgt_cell_capacity_dl,tgt_cell_capacity_ul);
                            break;
                            /*SPR 18178 End */
                        }
                    }
                }
                /* Inter freq cells ofn and cio store for algo */
                else
                {
                    for(inter_freq_index = RRM_ZERO;
                        inter_freq_index < p_ncl_params->lte_ncl.num_valid_inter_freq_cell;
                        inter_freq_index++)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                    "target cell is inter freq cell list PCI [%d] EARFCN [%d]",
                                    p_ncl_params->lte_ncl.inter_freq_cells[inter_freq_index].phy_cell_id,
                                    p_ncl_params->lte_ncl.inter_freq_cells[inter_freq_index].eutra_carrier_arfcn);

                        if ((target_pci == p_ncl_params->lte_ncl.inter_freq_cells[inter_freq_index].phy_cell_id) &&
                            (target_earfcn ==  p_ncl_params->lte_ncl.inter_freq_cells[inter_freq_index].eutra_carrier_arfcn))
                        {
                            
                            *p_total_of_ofn_and_ocn = (rrm_ue_convert_actual_qoffset_value
                                                           (RRM_EUTRAN_CELL,
                                                           p_ncl_params->lte_ncl.inter_freq_cells[inter_freq_index].q_offset) +
                                                       rrm_ue_convert_actual_qoffset_value
                                                           (RRM_EUTRAN_CELL,
                                                           p_ncl_params->lte_ncl.inter_freq_cells[inter_freq_index].cio));

                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                      "Inter freq Cell PCI [ %d ] Total of CIO and QOFFSET [ %d ]",
                                       p_ncl_params->lte_ncl.inter_freq_cells[inter_freq_index].phy_cell_id,
                                       *p_total_of_ofn_and_ocn);
                            ret_val = RRM_SUCCESS;
                            /*SPR 18178 Start */
                            if(p_ncl_params->lte_ncl.inter_freq_cells[inter_freq_index].resrc_report.bitmask 
                                    & RRM_COMP_AVL_CAP_PERIODIC_REPORT_PRESENT)
                            {
                                tgt_cell_capacity_ul =  p_ncl_params->lte_ncl.inter_freq_cells[inter_freq_index].
                                    resrc_report.comp_avl_cap_grp.ul.cell_cap_val;

                                tgt_cell_capacity_dl =  p_ncl_params->lte_ncl.inter_freq_cells[inter_freq_index].
                                    resrc_report.comp_avl_cap_grp.dl.cell_cap_val;
                            }
                            *p_tgt_cell_capacity = RRM_GET_MIN(tgt_cell_capacity_dl,tgt_cell_capacity_ul);
                            break;
                            /*SPR 18178 End */
                        }
                    }
                }
            }
            break;
        default:
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "Invalid rat type [%u] is received", rat_type);
            break;
    }
    /*SPR 18178 Start */
	RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
			"target cell PCI [%d] Earfcn [%d] rat type [%d] Avl Capacity[%d], Offset[%d]",
			target_pci,target_earfcn,rat_type, *p_tgt_cell_capacity, *p_total_of_ofn_and_ocn);
    /*SPR 18178 End */

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR 17529 Fix End */

/****************************************************************************
 * Function Name  : rrm_uem_is_cell_capable 
 * Inputs         : ue context, inter_freq_cells_t
 * Outputs        : none
 * Returns        : RRM_TRUE/RRM_FALSE
 * Description    : This fucntion check that CELL is capable for providing 
 *                  bitrate to the UE
 ****************************************************************************/
rrm_bool_et
rrm_uem_is_cell_capable(rrm_ue_context_t *p_rrm_ue_ctxt, inter_freq_cells_t *p_int_freq_cell)
{
    rrm_bool_et cell_is_capable = RRM_FALSE;
    U8 tgt_cell_capacity = RRM_ZERO;
    U8 tgt_cell_capacity_ul = RRM_ZERO;
    U8 tgt_cell_capacity_dl = RRM_ZERO;
    U8 req_ue_prb = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
 
    req_ue_prb = rrm_get_required_ue_capacity(p_rrm_ue_ctxt);

    if(p_int_freq_cell->resrc_report.bitmask & RRM_COMP_AVL_CAP_PERIODIC_REPORT_PRESENT)
    {
        tgt_cell_capacity_ul =  p_int_freq_cell->resrc_report.comp_avl_cap_grp.ul.cell_cap_val;
        tgt_cell_capacity_dl =  p_int_freq_cell-> resrc_report.comp_avl_cap_grp.dl.cell_cap_val;
    }
    else
    {
        tgt_cell_capacity_ul = RRM_MAX_AVLB_CAPCTY;
        tgt_cell_capacity_dl = RRM_MAX_AVLB_CAPCTY;
    }

    tgt_cell_capacity = RRM_GET_MIN(tgt_cell_capacity_dl,tgt_cell_capacity_ul);

    if(req_ue_prb <= tgt_cell_capacity)
    {
        cell_is_capable = RRM_TRUE;
    }
    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                 "Cell is %s. target cell capacity[%d] PCI[%u],[UE:%u] required PRB[%u] earfcn:[%u] ",
                 (RRM_TRUE== cell_is_capable ? "capable":"not capable"), tgt_cell_capacity, p_int_freq_cell->phy_cell_id,
                 p_rrm_ue_ctxt->ue_index,req_ue_prb,
                 p_int_freq_cell->eutra_carrier_arfcn);

    RRM_UT_TRACE_EXIT();
    return cell_is_capable;
}
/* SPR 16406 Start */
/****************************************************************************
 * Function Name  : rrm_rank_meas_reprted_cells
 * Inputs         : ue context
 * Outputs        : none
 * Returns        : none 
 * Description    : This fucntion assigns rank to meas reported cell
 *                  based on required ue resource and available target
 *                  cell capacity 
 ****************************************************************************/
rrm_void_t
rrm_rank_meas_reprted_cells(
        rrm_ue_context_t *p_rrm_ue_ctxt,
        cell_selection_priority_list_t *p_cell_selection_list
      )
{
    U8 index = RRM_ZERO;
    U8 tgt_cell_capacity = RRM_ZERO;
    U8 req_ue_prb = RRM_ZERO;
    /*SPR 22683 Fix Start*/
    /*Code Removed*/
    /*SPR 22683 Fix Stop*/

    /*SPR 17529 Fix Start */
    U8                              hysteresis           = RRM_ZERO;
    rrm_common_params_for_eutra_t   *p_common_params_for_eutra = RRM_PNULL;
    S8                              offset_freq = RRM_ZERO;
    /*SPR 17529 Fix End */

    RRM_UT_TRACE_ENTER();
    /*SPR 17529 Fix Start */
    p_common_params_for_eutra = rrm_cellm_get_connected_mode_common_params(p_rrm_ue_ctxt->cell_index);
    hysteresis = p_common_params_for_eutra->hysteresis;
    /*SPR 17529 Fix End */
    req_ue_prb = rrm_get_required_ue_capacity(p_rrm_ue_ctxt);


    RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
            "req_ue_prb=%d p_cell_selection_list->count =%d",
            req_ue_prb,
            p_cell_selection_list->count);

    /*rank p_cell_selection_list*/
    for(index = RRM_ZERO;
            index<p_cell_selection_list->count;
            index++)
    {
        /*SPR 17529 Fix Start */
        /*SPR 18178 Start */
        rrm_ue_get_neighbor_cell_total_ofn_ocn_values(p_rrm_ue_ctxt,
                p_cell_selection_list->\
                reported_neighbor_cell_info[index].phy_cell_id,
                p_cell_selection_list->\
                reported_neighbor_cell_info[index].target_earfcn,
                p_cell_selection_list->rat_type,
                &offset_freq,
                &tgt_cell_capacity);
        /*SPR 18178 End */

        /*SPR 18178 Start */
        /*Code Removed*/
        /*SPR 18178 End */
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "offset_freq: %d target cell capacity : %d" ,offset_freq,tgt_cell_capacity);
        if(RRM_ZERO == tgt_cell_capacity)
        {
            p_cell_selection_list->
                /* SPR 15232 start */
                reported_neighbor_cell_info[index].cell_rank = RRM_LOWEST_RANK;
            /* SPR 15232 end */
        }
        else if(tgt_cell_capacity>=req_ue_prb)
        {
            p_cell_selection_list->
                /* SPR 15232 start */
                reported_neighbor_cell_info[index].cell_rank =
                /* SPR 17529 Fix Start */
                /* Code deleted */
                rrm_ue_rank_calc_for_neighbor_cells(p_cell_selection_list->\
                        reported_neighbor_cell_info[index].rsrx_value,
                        offset_freq,
                        hysteresis,
                        p_cell_selection_list->rat_type);
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Final cell Rank = [%d] rsrx value [ %d] freq_offset[%d] hysteresis[%d]", 
                    p_cell_selection_list->reported_neighbor_cell_info[index].cell_rank,
                    p_cell_selection_list->reported_neighbor_cell_info[index].rsrx_value,
                    offset_freq,
                    hysteresis);
            /* SPR 17529 Fix End */
            /* SPR 15232 end */
        }
        else if(tgt_cell_capacity<req_ue_prb)
        {
            /*SPR 22683 Fix Start*/
            p_cell_selection_list->
                /* SPR 15232 start */
                reported_neighbor_cell_info[index].cell_rank =
                /* SPR 17529 Fix Start */
                /* Code deleted */
                (S8)((float)(rrm_ue_rank_calc_for_neighbor_cells(p_cell_selection_list->\
                                reported_neighbor_cell_info[index].rsrx_value,
                                offset_freq,
                                hysteresis,
                                p_cell_selection_list->rat_type) * (float)(RRM_ONE-(float)((float)(req_ue_prb - tgt_cell_capacity)/req_ue_prb))));
            /* SPR 17529 Fix End */
            /* SPR 15232 end */
            /*SPR 22683 Fix Stop*/ 
        }
        /*SPR 18178 Start */
        /*Code Removed */
        /*SPR 18178 End */
        /*SPR 17529 Fix End */
    }
} 


/****************************************************************************
 * Function Name  : rrm_sort_decesnding_based_on_rank
 * Inputs         : cell_selection_priority_list_t
 * Outputs        : none
 * Returns        : none
 * Description    : This function sorts meas reported cell based on
                    assigned rank
 ****************************************************************************/
    rrm_void_t
rrm_sort_decesnding_based_on_rank(
        cell_selection_priority_list_t  *p_cell_list
        )
{
    /* SPR 15232 start */
    rrm_rpt_cell_info_t temp_rpt_cell_info = {RRM_ZERO};
    /* SPR 15232 end */
    U16 index1 = RRM_ZERO;
    U16 index2 = RRM_ZERO;
    /* SPR 15232 start */
    /* Deleted code */
    /* SPR 15232 end */

    RRM_UT_TRACE_ENTER ();

    /* SPR 15232 start */
    p_cell_list->next_target_cell_idx = RRM_ZERO;
    /* SPR 15232 end */
    if (p_cell_list->count) 
    {
        for (index1 =RRM_ZERO; index1<(p_cell_list->count-1);index1++) 
        {
            for (index2=index1+1;index2<p_cell_list->count;index2++) 
            {
                    /* SPR 16406 18sep Start */
		    RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
				    RRM_BRIEF,
				    "Comapring ranks of PCI[%u], EARFCN[%u] at index[%d] = %d and PCI[%u], EARFCN[%u] at index[%d] = %d",
				    p_cell_list->reported_neighbor_cell_info[index1].phy_cell_id,
				    p_cell_list->reported_neighbor_cell_info[index1].target_earfcn,
				    index1, p_cell_list->reported_neighbor_cell_info[index1].cell_rank,
				    p_cell_list->reported_neighbor_cell_info[index2].phy_cell_id,
				    p_cell_list->reported_neighbor_cell_info[index2].target_earfcn,
				    index2, p_cell_list->reported_neighbor_cell_info[index2].cell_rank );
                    /* SPR 16406 18sep End */
                /* SPR 15232 start */
		    if (p_cell_list->reported_neighbor_cell_info[index1].cell_rank < 
				    p_cell_list->reported_neighbor_cell_info[index2].cell_rank) 
                    /* SPR 15232 end */
                {
                    /* SPR 15232 start */
			    temp_rpt_cell_info = p_cell_list->reported_neighbor_cell_info[index1];
			    p_cell_list->reported_neighbor_cell_info[index1] = p_cell_list->reported_neighbor_cell_info[index2];
			    p_cell_list->reported_neighbor_cell_info[index2] = temp_rpt_cell_info;
                    /* SPR 15232 end */
                }
            }
        }
    }
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_sort_and_rank_meas_reported_cell
 * Inputs         : p_ue_context
 * Outputs        : none
 * Returns        : none
 * Description    : This function ranks and sorts the reported cells
 ****************************************************************************/
rrm_void_t
rrm_sort_and_rank_meas_reported_cell(
    rrm_ue_context_t *p_rrm_ue_ctxt
    )
{
    cell_selection_priority_list_t *p_cell_selection_list = RRM_PNULL;
   RRM_UT_TRACE_ENTER();

    p_cell_selection_list = rrm_uem_get_active_cell_selection_list(p_rrm_ue_ctxt);
    if(RRM_PNULL == p_cell_selection_list)
    {
        RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_WARNING,
                "No cell selection list is present");
        RRM_UT_TRACE_EXIT();
        return;
	}
	/* ranking and sorting cell_selection_list */
    /* SPR 16406 8SEP start */
    if((RRM_FALSE == p_cell_selection_list->ranking_done) && (RRM_ZERO < p_cell_selection_list->count))
	{
		rrm_rank_meas_reprted_cells(p_rrm_ue_ctxt, 
				p_cell_selection_list);
		rrm_sort_decesnding_based_on_rank(p_cell_selection_list);
        p_cell_selection_list->ranking_done = RRM_TRUE;
    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_BRIEF,
                "Sorting not done as Ranking is %s and count is [%u]",
                (RRM_FALSE == p_cell_selection_list->ranking_done)? "required" : "already done",
                p_cell_selection_list->count);
	}
    /* SPR 16406 8SEP end */

	RRM_UT_TRACE_EXIT();
}
/*end tgt_cell_ranking*/
/* SPR 16406 End */

/* SPR 16406 Start */
/* Code Removed */
/* SPR 16406 End */

/* SPR 16406 Start */
/* SPR 15674 Start */
/* Fix for CSR 58972 end*/
/****************************************************************************
 * Function Name  : rrm_get_best_suitable_cell_for_ho
 * Inputs         : p_ue_context_to_ho,preferred_cell_access_mode
 * Outputs        : Phy cell id
 * Returns        : Phy cell id
 * Description    : This function find the best suitable cell
 considering csg info
 ****************************************************************************/
phy_cell_id_info_t
rrm_get_best_suitable_cell_for_ho(
        rrm_ue_context_t   *p_ue_context_to_ho,
        rrm_oam_access_mode_et prefered_cell_access_mode
        )
{
    /* SPR 16406 8SEP start */
        /* SPR 16406 18sep Start */
    phy_cell_id_info_t phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
        /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    cell_selection_priority_list_t *p_cell_selection_list = RRM_PNULL;
    /* coverity_fix 88200 +- : unused p_ho_params removed */
    rrm_daho_cell_config_info_t  *p_daho_cfg = RRM_NULL;
    /* coverity : CID 29705*/
    rrm_bool_et csg_membership_status_presence = RRM_FALSE;
    /* coverity : CID 29704*/
    rrm_bool_et csg_id_presence = RRM_FALSE;
    rrm_bool_et phy_cell_id_found = RRM_FALSE;

    RRM_UT_TRACE_ENTER();

    RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
            RRM_DETAILED,
     /* SPR_15636_Fix: Start */
             "Prefered cell access mode for HO = %d",
     /* SPR_15636_Fix: End */
            prefered_cell_access_mode);

    /*start sort meas reported cell based on cell load*/
    /* Start SPR:9843 */
    /* SPR 16406 8SEP start */
        rrm_sort_and_rank_meas_reported_cell(p_ue_context_to_ho);
    /* SPR 16406 8SEP end */
    /* End SPR:9843 */
    /*end sort meas reported cell based on cell load*/
    p_daho_cfg = rrm_cellm_get_daho_cell_config_info_params(p_ue_context_to_ho->cell_index);
    /* coverity_fix 88200 +- : unused p_ho_params removed */
    p_cell_selection_list = rrm_uem_get_active_cell_selection_list(p_ue_context_to_ho);
    /*search total cell list reported by UE*/
    /* coverity_fix 88200 ++ */
    if(RRM_PNULL != p_cell_selection_list)
    {
    while(RRM_TRUE)
    {

        phy_cell_id_info = rrm_ue_get_strongest_phy_cell_id_for_ho (
                p_ue_context_to_ho,
                &csg_membership_status_presence,
                &csg_id_presence);


        if(INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id)
        {
            /* SPR_15636_Fix_2: Start */
            /* Code Removed */
            RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                    RRM_DETAILED,
                    "Handing Over UE phy_cell_id = %d , Earfcn = %d",
                    phy_cell_id_info.phy_cell_id, phy_cell_id_info.carr_earfcn);
            phy_cell_id_found = RRM_TRUE;
            break;
            /* SPR_15636_Fix_2: End */
        }
        else
        {
            RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                    RRM_DETAILED,
                    "INVALID PHY CELL ID returned, can not hand out UE");
            break;
        }
        /*+-Coverity 88576*/
    }

    if((phy_cell_id_found == RRM_FALSE) && (INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id))
    {
        /*If no suitable cell is found and already a daho cell selected hand over
          on the DAHO cell*/
        if(p_cell_selection_list->bitmask & RRM_UE_BLIND_CELL_HO_TRIGGERED)
        {
            if((p_ue_context_to_ho->ho_params.utra_known_cell_selection_list.bitmask & RRM_UE_BLIND_CELL_HO_TRIGGERED) &&
                    (UTRA_CELL == p_ue_context_to_ho->target_cell_type) &&
                    /*daho clean up(condition added for presence of daho cell)*/
                    (RRM_PNULL != p_daho_cfg) && (p_daho_cfg->bitmask & UTRAN_DAHO_CELL_PRESENT))
            {
                phy_cell_id_info.phy_cell_id = p_daho_cfg->utran_daho_cell_config_info.pci;
                RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                        RRM_DETAILED,
                        "Triggering HO to utra DAHO cell, phy_cell_id = %d, Earfcn = %d",
                        phy_cell_id_info.phy_cell_id, phy_cell_id_info.carr_earfcn);
            }
            else if((p_ue_context_to_ho->ho_params.eutra_known_cell_selection_list.bitmask & RRM_UE_BLIND_CELL_HO_TRIGGERED) &&
                    (EUTRA_CELL == p_ue_context_to_ho->target_cell_type) &&
                    /*daho clean up(condition added for presence of daho cell)*/
                    (RRM_PNULL != p_daho_cfg) && (p_daho_cfg->bitmask & EUTRA_DAHO_CELL_PRESENT))
            {
                phy_cell_id_info.phy_cell_id = p_daho_cfg->eutran_daho_cell_config_info.phy_cell_id;
                RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                        RRM_DETAILED,
                        "Triggering HO to eutra DAHO cell, phy_cell_id = %d, Earfcn = %d",
                        phy_cell_id_info.phy_cell_id, phy_cell_id_info.carr_earfcn);
            }
            else if((p_ue_context_to_ho->ho_params.geran_known_cell_selection_list.bitmask & RRM_UE_BLIND_CELL_HO_TRIGGERED) &&
                    (GERAN_CELL == p_ue_context_to_ho->target_cell_type) &&
                    /*daho clean up(condition added for presence of daho cell)*/
                    (RRM_PNULL != p_daho_cfg) && (p_daho_cfg->bitmask & GERAN_DAHO_CELL_PRESENT))
            {
                phy_cell_id_info.phy_cell_id = p_daho_cfg->geran_daho_cell_config_info.pci;
                RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                        RRM_DETAILED,
                        "Triggering HO to geran DAHO cell, phy_cell_id = %d, Earfcn = %d",
                        phy_cell_id_info.phy_cell_id, phy_cell_id_info.carr_earfcn);
            }
            /* Coverity_fix_88196_start */
            /* coverity_fix 88200 ++ */
            /* Validate that is DAHO cell restricted as per restriction list */
            if ( RRM_TRUE == rrm_is_daho_cell_restricted (p_ue_context_to_ho))
            {
                phy_cell_id_info.phy_cell_id= INVALID_PHYSICAL_CELL_ID;
            }
            /* coverity_fix 88200 -- */
            /* Coverity_fix_88196_end */
        }
        else
        {

            phy_cell_id_info = rrm_ue_get_strongest_phy_cell_id_for_ho (
                    p_ue_context_to_ho,
                    &csg_membership_status_presence,
                    &csg_id_presence);
            if(INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id)
            {
                if((p_ue_context_to_ho->ho_params.utra_known_cell_selection_list.bitmask & RRM_UE_BLIND_CELL_HO_TRIGGERED) &&
                        (UTRA_CELL == p_ue_context_to_ho->target_cell_type) &&
                        /*daho clean up(condition added for presence of daho cell)*/
                        (RRM_PNULL != p_daho_cfg) && (p_daho_cfg->bitmask & UTRAN_DAHO_CELL_PRESENT))
                {
                    phy_cell_id_info.phy_cell_id = p_daho_cfg->utran_daho_cell_config_info.pci;
                    RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                            RRM_DETAILED,
                            "Triggering HO to utra DAHO cell, phy_cell_id = %d, Earfcn = %d",
                            phy_cell_id_info.phy_cell_id, phy_cell_id_info.carr_earfcn);
                }
                else if((p_ue_context_to_ho->ho_params.eutra_known_cell_selection_list.bitmask & RRM_UE_BLIND_CELL_HO_TRIGGERED) &&
                        (EUTRA_CELL == p_ue_context_to_ho->target_cell_type) &&
                        /*daho clean up(condition added for presence of daho cell)*/
                        (RRM_PNULL != p_daho_cfg) && (p_daho_cfg->bitmask & EUTRA_DAHO_CELL_PRESENT))
                {
                    phy_cell_id_info.phy_cell_id = p_daho_cfg->eutran_daho_cell_config_info.phy_cell_id;
                    RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                            RRM_DETAILED,
                            "Triggering HO to eutra DAHO cell, phy_cell_id = %d, Earfcn = %d",
                            phy_cell_id_info.phy_cell_id, phy_cell_id_info.carr_earfcn);
                }
                else if((p_ue_context_to_ho->ho_params.geran_known_cell_selection_list.bitmask & RRM_UE_BLIND_CELL_HO_TRIGGERED) &&
                        (GERAN_CELL == p_ue_context_to_ho->target_cell_type) &&
                        /*daho clean up(condition added for presence of daho cell)*/
                        (RRM_PNULL != p_daho_cfg) && (p_daho_cfg->bitmask & GERAN_DAHO_CELL_PRESENT))
                {
                    phy_cell_id_info.phy_cell_id = p_daho_cfg->geran_daho_cell_config_info.pci;
                    RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                            RRM_DETAILED,
                            "Triggering HO to utra geran cell, phy_cell_id = %d, Earfcn = %d",
                            phy_cell_id_info.phy_cell_id, phy_cell_id_info.carr_earfcn);
                }
                else
                {
                    phy_cell_id_info.phy_cell_id = INVALID_PHYSICAL_CELL_ID;
                }
            }
            else
            {
                RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                        RRM_DETAILED,
                        "INVALID PHY CELL ID returned, can not hand out UE");
            }
        }
    }
        /* coverity_fix 88200 +- : Code moved*/
    }
    else
    {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                RRM_WARNING,"p_cell_selection_list Null detected");
        RRM_UT_TRACE_EXIT();
        return phy_cell_id_info;
    }
    RRM_UT_TRACE_EXIT();
    return phy_cell_id_info;
    /* coverity_fix 88200 -- */
}
/* SPR 15674 End */
/* SPR 16406 End */

/* SPR 16406 Start */
/* SPR 15674 Start */
/* Fix for CSR 58972 start*/
/* SPR-13322-fix start */
/****************************************************************************
 * Function Name  : rrm_get_best_pci_for_ho
 * Inputs         : p_ue_context_to_ho,preferred_cell_access_mode
 * Outputs        : NULL
 * Returns        : Phy cell id
 * Description    : This function find the best suitable cell
 considering csg info and handles PCI confusion. 
 ****************************************************************************/
phy_cell_id_info_t
    rrm_get_best_pci_for_ho 
(
 rrm_ue_context_t   *p_ue_context_to_ho,
 rrm_oam_access_mode_et prefered_cell_access_mode
 )
{
    /* SPR 16406 8SEP start */
        /* SPR 16406 18sep Start */
    phy_cell_id_info_t              phy_cell_id_info  = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
        /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    rrm_ho_params_t*                p_ho_params              = RRM_PNULL;
    rrm_cell_context_t*             p_cell_ctxt              = RRM_PNULL; 
    RRM_UT_TRACE_ENTER();

    if (RRM_PNULL != p_ue_context_to_ho)
    {

        p_cell_ctxt =  rrm_cellm_get_cell_context(p_ue_context_to_ho->cell_index);
        p_ho_params = (rrm_ho_params_t*)&p_ue_context_to_ho->ho_params;


        if ((RRM_PNULL == p_cell_ctxt) || (RRM_PNULL == p_ho_params))
        {
            RRM_TRACE (g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "Cell context or HO parameter context is NULL");
            RRM_UT_TRACE_EXIT();
            return phy_cell_id_info;
        }

    }
    else
    {

        RRM_TRACE (g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "UE context is NULL");
        RRM_UT_TRACE_EXIT();
        return phy_cell_id_info;
    }

    phy_cell_id_info = rrm_get_best_suitable_cell_for_ho(p_ue_context_to_ho,prefered_cell_access_mode);
    if (INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id) 
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                "valid PCI [%u], EARFCN [%d] found", phy_cell_id_info.phy_cell_id, phy_cell_id_info.carr_earfcn);
    } 
    /* SPR 15232 start */
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "No valid phy cell Id found!!!");

    }

    /* SPR 15232 end */
    /* SPR 15232 start */
    /* Removed code */
    /* SPR 15232 end */

    RRM_UT_TRACE_EXIT();
    return phy_cell_id_info;
}
/* SPR 16406 End */
/* SPR 15674 End */
/* SPR-13322-fix end */
/* Fix for CSR 58972 end*/

/****************************************************************************
 * Function Name  : rrm_handle_csg_membership_hybrid_for_ho
 * Inputs         : UE context
 *                  acces management parameters
 *                  stored in cell comtext
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function verifies the csg membership status
                    for hybird cell in case of incoming ho
 ****************************************************************************/
rrm_return_et
rrm_handle_csg_membership_hybrid_for_ho(
        rrm_ue_context_t *p_rrm_ue_ctxt,
        S16                      max_csg_members,
        S16                      max_non_csg_members
        )
{
    rrm_return_et ret_val = RRM_SUCCESS;
    U8 num_connected_csg_ue = RRM_ZERO;
    U8 num_connected_non_csg_ue = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
    rrm_get_csg_non_csg_ues(&num_connected_csg_ue,&num_connected_non_csg_ue,p_rrm_ue_ctxt->cell_index);

    if(p_rrm_ue_ctxt->rrm_csg_membership.rrm_csg_membership_status == RRM_MEMBER )
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,
                "[UE:%d], csg membership status of the ue is MEMBER",
                p_rrm_ue_ctxt->ue_index);

        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                "connected csg ue = %d",
                (num_connected_csg_ue - RRM_ONE));

        /*All csg member ue are already admitted*/
        if((num_connected_csg_ue - RRM_ONE) >= max_csg_members)
        {
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                    "[UE:%d], configured number of csg ues are already"
                    "admitted, sending ho_adm_resp failure",
                    p_rrm_ue_ctxt->ue_index);
            ret_val = RRM_FAILURE;
        }
    }
    else/*not member*/
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,
                "[UE:%d], csg membership status of the ue is NOT MEMBER",
                p_rrm_ue_ctxt->ue_index);

        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                "connected non csg ue = %d",
                (num_connected_non_csg_ue - RRM_ONE));

        if((num_connected_non_csg_ue - RRM_ONE) >= max_non_csg_members)
        {
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                    "[UE:%d], configured number of non csg ues are already"
                    "admitted, sending ho_adm_resp failure",
                    p_rrm_ue_ctxt->ue_index);
            ret_val = RRM_FAILURE;
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* Review_comment_for_SPR_7996_nd_8345_start */
/******************************************************************************
 *   FUNCTION NAME: rrm_decrement_num_of_enb_init_ho_csg_and_non_csg_usr
 *   INPUT      : p_ue_context
 *   OUTPUT     : none
 *   DESCRIPTION:
 *       decrement number of enb initiated ho of csg and non csg user
 *
 *   RETURNS: rrm_void_t     
 ******************************************************************************/
rrm_void_t
rrm_decrement_num_of_enb_init_ho_csg_and_non_csg_usr(
        rrm_ue_context_t    *p_ue_ctxt
)
{

    rrm_cell_context_t  *p_cell_context     = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    p_cell_context =  rrm_cellm_get_cell_context(p_ue_ctxt->cell_index);
    RRM_ASSERT(RRM_PNULL != p_cell_context);
    /* Coverity 93975 Fix Start */
    if (RRM_PNULL == p_cell_context)
    {
        RRM_UT_TRACE_EXIT();
    }
    else
    {
    /* Coverity 93975 Fix End */

    if(p_ue_ctxt->rrm_csg_membership.rrm_csg_membership_status
                            == RRM_MEMBER )
    {
        p_cell_context->kpi_stat.kpi.num_of_enb_init_ho_csg_usr--;
    }
    else if(p_ue_ctxt->rrm_csg_membership.rrm_csg_membership_status
                            == RRM_NOT_MEMBER) 
    {
        p_cell_context->kpi_stat.kpi.num_of_enb_init_ho_non_csg_usr--;    
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,\
                    "[UE:%d] is neither CSG member nor non CSG member"
                    " So no decrement done",p_ue_ctxt->ue_index);
 
    }
    /* Coverity 93975 Fix Start */
    }
    /* Coverity 93975 Fix End */
    RRM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrm_increment_num_of_enb_init_ho_csg_and_non_csg_usr
 *   INPUT      : p_cell_context, p_ue_ctxt
 *   OUTPUT     : none
 *   DESCRIPTION:
 *       Increment number of enb initiated ho of csg and non csg user
 *
 *   RETURNS: rrm_void_t     
 ******************************************************************************/
rrm_void_t
rrm_increment_num_of_enb_init_ho_csg_and_non_csg_usr(
    rrm_ue_context_t    *p_ue_ctxt
)
{
    rrm_cell_context_t  *p_cell_context = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_cell_context =  rrm_cellm_get_cell_context(p_ue_ctxt->cell_index);
    RRM_ASSERT(RRM_PNULL != p_cell_context);
    /* Coverity 93938 Fix Start */
    if (RRM_PNULL == p_cell_context)
    {
	RRM_UT_TRACE_EXIT();
        return;
    }
    /* Coverity 93938 Fix End */
    if(p_ue_ctxt->rrm_csg_membership.rrm_csg_membership_status
                            == RRM_MEMBER )
    {
        p_cell_context->kpi_stat.kpi.num_of_enb_init_ho_csg_usr++;
    }
    else if(p_ue_ctxt->rrm_csg_membership.rrm_csg_membership_status
                            == RRM_NOT_MEMBER) 
    {
        p_cell_context->kpi_stat.kpi.num_of_enb_init_ho_non_csg_usr++;    
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,\
                    "[UE:%d] is neither CSG member nor non CSG member"
                    " So no increment done",p_ue_ctxt->ue_index);
 
    }
    RRM_UT_TRACE_EXIT();
}
/* Review_comment_for_SPR_7996_nd_8345_end */
/****************************************************************************
 * Function Name  : rrm_handle_csg_membership_hybrid
 * Inputs         : UE context
 *                  acces management parameters 
                    stored in cell comtext
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function verifies the csg membership status
                    for hybird cell
 ****************************************************************************/
rrm_return_et
rrm_handle_csg_membership_hybrid(
        rrm_ue_context_t *p_rrm_ue_ctxt,
        S16                      max_csg_members,
        S16                      max_non_csg_members
        )
{
   rrm_return_et ret_val = RRM_SUCCESS; 
   U8 num_connected_csg_ue = RRM_ZERO;
   U8 num_connected_non_csg_ue = RRM_ZERO;
   rrm_ue_index_t ue_index = RRM_ZERO;
   rrm_cell_context_t           *p_cell_context = RRM_PNULL;
   /* SPR 20652 Fix Start */
   /* Code Removed */
   rrm_ue_context_t             *p_ue_context = RRM_PNULL;
   /* SPR 20652 Fix End */
   RRM_UT_TRACE_ENTER();
   rrm_get_csg_non_csg_ues(&num_connected_csg_ue,&num_connected_non_csg_ue,p_rrm_ue_ctxt->cell_index);
   p_cell_context =  rrm_cellm_get_cell_context(p_rrm_ue_ctxt->cell_index);
    /*CID 65889:start*/
    if (RRM_PNULL != p_cell_context)
    {
        if(RRM_TRUE == p_rrm_ue_ctxt->rrm_ue_in_hold)
        {
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                    "[UE:%d] UE is in HOLD",
                    p_rrm_ue_ctxt->ue_index);
        }
        else
        {
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                    "[UE:%d] UE is NOT in HOLD",
                    p_rrm_ue_ctxt->ue_index);
        }

        if(p_rrm_ue_ctxt->rrm_csg_membership.rrm_csg_membership_status
                == RRM_MEMBER ) 
        {
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_INFO,
                    "[UE:%d] csg membership status of the UE is MEMBER"
                    "connected csg ue = %d",
                    p_rrm_ue_ctxt->ue_index,
                    (num_connected_csg_ue - RRM_ONE));

            /*All csg member ue are already admitted*/
            if((num_connected_csg_ue - RRM_ONE) >= max_csg_members) 
            {
                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                        "[UE:%d], configured number of csg ues are already"
                        "admitted, handing over lowest priority csg ue",
                        p_rrm_ue_ctxt->ue_index);

                /*Get lowest priority csg ue to handover*/ 
                ret_val = rrm_get_low_priority_csg_ue(&ue_index,p_rrm_ue_ctxt->ue_index);

                if(ret_val == RRM_SUCCESS)
                {
                    /** Find the UE context of the lowest priority ue*/
                    /* SPR 20652 Fix Start */
                    p_ue_context = rrm_ue_find_context(ue_index,
                            p_rrm_ue_ctxt->cell_index);

                    if (RRM_PNULL == p_ue_context)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR, "[UE:%d], failed to find ue context",
                                ue_index);
                        return RRM_FAILURE;
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, "[UE:%d],ue context found for"
                                "handover",
                                ue_index);
                    }

                    /*Hand over the ue to a closed cell*/
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, "csg membership status"
                            "is member for the UE, sending meas config req for the lowest priority [UE:%d]",
                            p_rrm_ue_ctxt->ue_index);
                    /*SPR 17777 +-*/
                    ret_val = rrm_handover_ue_to_best_cell(p_ue_context);
                    /* SPR 20652 Fix End */
                    /*KPI: start enb_init csg ho*/
                    p_cell_context->kpi_stat.kpi.num_of_enb_init_ho_csg_usr++;
                    /*KPI: end enb_init csg ho*/

                }
                else
                {
                    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_INFO,
                            "[UE:%d], low priority csg ue not found, cant handover any ue",
                            p_rrm_ue_ctxt->ue_index);

                }
            }
        }
        else /*not a csg member*/
        {
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_INFO,
                    "[UE:%d] csg membership status of the UE is NOT MEMBER",
                    p_rrm_ue_ctxt->ue_index);

            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                    "connected non csg ue = %d",
                    (num_connected_non_csg_ue - RRM_ONE));

            if((num_connected_non_csg_ue - RRM_ONE) >= max_non_csg_members) 
            {
                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF,
                        "[UE:%d], configured number of non csg ues are already"
                        "admitted, handing over lowest priority non csg ue",
                        p_rrm_ue_ctxt->ue_index);

                ret_val = rrm_get_low_priority_non_csg_ue(&ue_index,p_rrm_ue_ctxt->ue_index);
                if(ret_val == RRM_SUCCESS)
                {

                    /** Find the UE context of the lowest priority ue*/
                    /* SPR 20652 Fix Start */
                    p_ue_context = rrm_ue_find_context(ue_index,
                            p_rrm_ue_ctxt->cell_index);

                    if (RRM_PNULL == p_ue_context)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR, "[UE:%d], failed to find ue context",
                                ue_index);
                        return RRM_FAILURE;
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, "[UE:%d],ue context found for"
                                "handover",
                                ue_index);
                    }

                    /*Hand over the ue to a open cell*/
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, "csg membership status"
                            "is non_member,sending meas config req for the lowest priority [UE:%d]",
                            p_rrm_ue_ctxt->ue_index);
                    /*SPR 17777 +-*/
                    ret_val = rrm_handover_ue_to_best_cell(p_ue_context);
                    /* SPR 20652 Fix End */
                    /*KPI: start enb_init csg ho*/
                    p_cell_context->kpi_stat.kpi.num_of_enb_init_ho_non_csg_usr++;
                    /*KPI: end enb_init csg ho*/
                }
                else
                {
                    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_INFO,
                            "[UE:%d], low priority non csg ue not found, cant handover any ue",
                            p_rrm_ue_ctxt->ue_index);

                }
            }
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"p_cell_context is null");
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }

    /*CID 65889:end*/
   RRM_UT_TRACE_EXIT();
   return  ret_val;
}

/****************************************************************************
 * Function Name  : rrm_handle_csg_membership_closed
 * Inputs         : UE context
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : this function handles csg membership 
                    recvd in ue_context_mod in case of closed cell
 ****************************************************************************/
rrm_return_et
rrm_handle_csg_membership_closed(
        rrm_ue_context_t *p_rrm_ue_ctxt
        )
{
    rrm_return_et ret_val = RRM_SUCCESS;
    /*check csg membership status of the UE*/
    /*Handover the ue to a open cell if membership expires in case
      of closed cell*/
    if(p_rrm_ue_ctxt->rrm_csg_membership.rrm_csg_membership_status
                       == RRM_NOT_MEMBER)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO, "[UE:%d], csg membership status"
                "has been changed to non_member for the closed cell, trying HO to a open cell",
                p_rrm_ue_ctxt->ue_index);
        /*SPR 17777 +-*/
        ret_val = rrm_handover_ue_to_best_cell(p_rrm_ue_ctxt);
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, "[UE:%d], csg membership status"
                "of the UE is member for the closed cell",
                p_rrm_ue_ctxt->ue_index);
    }
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_handle_csg_membership_open
 * Inputs         : UE context
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : this function handles csg membership
                    recvd in ue_context_mod in case of open cell
 ****************************************************************************/
rrm_return_et
rrm_handle_csg_membership_open(
  rrm_ue_context_t *p_rrm_ue_ctxt
  )
{
    rrm_return_et ret_val = RRM_SUCCESS;

    /*Handover the ue to a closed cell if ue becomes member  
      in case of a open cell*/
    if(p_rrm_ue_ctxt->rrm_csg_membership.rrm_csg_membership_status == RRM_MEMBER)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO, "[UE:%d], csg membership status"
                "has been changed to member for the open cell, trying HO to a closed cell",
                p_rrm_ue_ctxt->ue_index);
     /*SPR 17777 +-*/
      ret_val = rrm_handover_ue_to_best_cell(p_rrm_ue_ctxt);
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, "[UE:%d], csg membership status"
                "of the UE is not-member for the open cell",
                p_rrm_ue_ctxt->ue_index);
    }
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_handle_max_csg_mem_minus_one_hybrid_for_ho
 * Inputs         : UE context,access_mgmt_params
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : this function handles csg membership for ho when num_csg_member
                    is minus one
 ****************************************************************************/
rrm_return_et
rrm_handle_max_csg_mem_minus_one_hybrid_for_ho(
                rrm_ue_context_t *p_rrm_ue_context,
                access_mgmt_params_t *p_access_mgmt_params
                )
{
    rrm_return_et      ret_val = RRM_SUCCESS;
    rrm_ue_index_t     ue_index = RRM_ZERO;
    /* SPR 20652 Fix Start */
    /* Code Removed */
    rrm_ue_context_t    *p_ue_context = RRM_PNULL;
    /* SPR 20652 Fix End */
    RRM_UT_TRACE_ENTER();
    if(p_rrm_ue_context->rrm_ue_in_hold == RRM_FALSE)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                "[UE:%d], ue is not in hold",
                p_rrm_ue_context->ue_index);
        rrm_handle_csg_membership_hybrid_for_ho(p_rrm_ue_context,rrm_cellm_get_max_num_ue_per_cell(p_rrm_ue_context->cell_index),
                p_access_mgmt_params->max_non_csg_members);
    }
    else if(p_rrm_ue_context->rrm_ue_in_hold == RRM_TRUE)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                "[UE:%d], ue is in hold",
                p_rrm_ue_context->ue_index);
        /* SPR 17564 Fix Start */
        if ((RRM_TRUE == rrm_is_ue_special_previledge(p_rrm_ue_context)) ||
        /* SPR 17564 Fix End */
                (RRM_MEMBER == p_rrm_ue_context->rrm_csg_membership.rrm_csg_membership_status))
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,
                    "[UE:%d],csg membership status of the ue is member",
                    p_rrm_ue_context->ue_index);
            /*handover a lowest priority non csg ue*/
            ret_val = rrm_get_low_priority_non_csg_ue(&ue_index,p_rrm_ue_context->ue_index);
            if(ret_val == RRM_SUCCESS)
            {

                /** Find the UE context of the lowest priority ue*/
                /* SPR 20652 Fix Start */
                p_ue_context = rrm_ue_find_context(ue_index,
                        p_rrm_ue_context->cell_index);

                if (RRM_PNULL == p_ue_context)
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR, "[UE:%d], failed to find ue context",
                            ue_index);
                    return RRM_FAILURE;
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, "[UE:%d],ue context found for"
                            "handover",
                            ue_index);
                }

                /*Hand over the ue to a open cell*/
                /*SPR 17777 +-*/
                ret_val = rrm_handover_ue_to_best_cell(p_ue_context);
                /* SPR 20652 Fix End */
            }
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "[UE:%d],csg membership status of the ue is not member",
                            p_rrm_ue_context->ue_index);
            ret_val = RRM_FAILURE;
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/****************************************************************************
 * Function Name  : rrm_handle_max_csg_mem_minus_one_hybrid
 * Inputs         : UE context,access_mgmt_params
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : this function handles csg membership when num_csg_member 
                    is minus one               
 ****************************************************************************/
rrm_return_et
rrm_handle_max_csg_mem_minus_one_hybrid(
       rrm_ue_context_t *p_rrm_ue_context,
       access_mgmt_params_t *p_access_mgmt_params
       )
{
   rrm_return_et      ret_val = RRM_SUCCESS;
   rrm_ue_index_t     ue_index = RRM_ZERO;
   /* SPR 20652 Fix Start */
   /* Code Removed */
   rrm_ue_context_t   *p_ue_context = RRM_PNULL;
   /* SPR 20652 Fix End */
   RRM_UT_TRACE_ENTER();

   if(p_rrm_ue_context->rrm_ue_in_hold == RRM_FALSE)
   {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                "[UE:%d], ue is not in hold",
                        p_rrm_ue_context->ue_index);
        ret_val = rrm_handle_csg_membership_hybrid(p_rrm_ue_context,rrm_get_max_allowed_user(p_rrm_ue_context->cell_index),
               p_access_mgmt_params->max_non_csg_members);
   }
   else if(p_rrm_ue_context->rrm_ue_in_hold == RRM_TRUE)
   {
       RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
               "[UE:%d], ue is in hold",
               p_rrm_ue_context->ue_index);

      if((RRM_UE_ESTAB_CAUSE_EMERGENCY == p_rrm_ue_context->establishment_cause) || 
         (RRM_UE_ESTAB_CAUSE_HIGHPRIORITY_ACCESS == p_rrm_ue_context->establishment_cause) ||
         (RRM_MEMBER == p_rrm_ue_context->rrm_csg_membership.rrm_csg_membership_status))
      {
          RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                  "[UE:%d], csg membership status of the ue is member",
                  p_rrm_ue_context->ue_index);
          /*handover a lowest priority non csg ue*/     
          ret_val = rrm_get_low_priority_non_csg_ue(&ue_index,p_rrm_ue_context->ue_index);
          if(ret_val == RRM_SUCCESS)
          {

              /** Find the UE context of the lowest priority ue*/
              /* SPR 20652 Fix Start */
              p_ue_context = rrm_ue_find_context(ue_index,
                      p_rrm_ue_context->cell_index);

              if (RRM_PNULL == p_ue_context)
              {
                  RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR, "[UE:%d], failed to find ue context",
                          ue_index);
                  return RRM_FAILURE;
              }
              else
              {
                  RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, "[UE:%d],ue context found for "
                          "handover",
                          ue_index);
              }

              /*Hand over the ue to a open cell*/
              /*SPR 17777 +-*/
              ret_val = rrm_handover_ue_to_best_cell(p_ue_context);
              /* SPR 20652 Fix End */
          }
          else
          {
              RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                        "lowest priority non_csg UE not found,"
                      "cant handover any UE");
          }
           
      }
      else
      {
          RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                  "[UE:%d], csg membership status of the ue is NOT member, "
                  "or UE is NOT high priority or emergency",
                  p_rrm_ue_context->ue_index);
          ret_val = RRM_FAILURE;
      }
   }
   RRM_UT_TRACE_EXIT();
   return ret_val;
}
/****************************************************************************
 * Function Name  : rrm_handle_csg_membership
 * Inputs         : UE context
 *                  ERAB to be setup
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function verifies the csg membership status
                    for closed and hybird cell
 ****************************************************************************/
rrm_return_et
rrm_handle_csg_membership(
        rrm_ue_context_t *p_rrm_ue_ctxt
        )
{

    rrm_return_et ret_val = RRM_SUCCESS;
    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;
    access_mgmt_params_t *p_access_mgmt_params = RRM_NULL;
    RRM_UT_TRACE_ENTER();
    /*get serving cell access management params*/
    p_access_mgmt_params = rrm_cellm_get_access_mgmt_params(cell_index);
    switch(p_access_mgmt_params->access_mode)
    {
        /* Fix for CSR 58972 start*/
        case RRM_OAM_ACCESS_MODE_OPEN:
        case RRM_OAM_ACCESS_MODE_OPEN_FEMTO:
            /* Fix for CSR 58972 end*/
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                    "[UE:%d],CELL_INDEX[%d] cell is open_cell, checking for"
                    " csg membership of the ue",
                    p_rrm_ue_ctxt->ue_index, p_rrm_ue_ctxt->cell_index);
            rrm_handle_csg_membership_open(p_rrm_ue_ctxt);
            /*We shall keep the  UE even if no PCI is found for HO,
              when ue became member in open cell*/
            ret_val = RRM_SUCCESS;
            break;
        case RRM_OAM_ACCESS_MODE_CLOSE:
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                    "[UE:%d],CELL_INDEX[%d] cell is closed_cell, checking for csg membership of the ue",
                    p_rrm_ue_ctxt->ue_index, p_rrm_ue_ctxt->cell_index);
            ret_val = rrm_handle_csg_membership_closed(p_rrm_ue_ctxt);
            break;
        case RRM_OAM_ACCESS_MODE_HYBRID:
            if(p_access_mgmt_params->max_csg_members != RRM_INDEX_ERR) 
            {
               ret_val = rrm_handle_csg_membership_hybrid(p_rrm_ue_ctxt,p_access_mgmt_params->max_csg_members,
                                                       p_access_mgmt_params->max_non_csg_members);

               /*due to change in csg membership status we shall try HO of the UE if 
                 csg validation fails*/
               if(RRM_FAILURE == ret_val)
               {
                   RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                           "[UE:%d],CELL_INDEX[%d] csg validation failed handing over"
                           " this UE",
                           p_rrm_ue_ctxt->ue_index, p_rrm_ue_ctxt->cell_index);
                   if(p_rrm_ue_ctxt->rrm_csg_membership.rrm_csg_membership_status
                           == RRM_MEMBER)
                   {
                       /*As ue is csg member try to ho it to a closed cell*/
                       /*SPR 17777 +-*/
                       ret_val = rrm_handover_ue_to_best_cell(p_rrm_ue_ctxt);
                   }
                   else if(p_rrm_ue_ctxt->rrm_csg_membership.rrm_csg_membership_status
                           == RRM_NOT_MEMBER)
                   {
                       /*As ue is csg member try to ho it to a open cell*/
                        /*SPR 17777 +-*/
                       ret_val = rrm_handover_ue_to_best_cell(p_rrm_ue_ctxt);
                   }
               }
            }
            else
            {
                /*as max_csg_members is set as -1 max_csg_members is max_allowed user*/
                ret_val = rrm_handle_csg_membership_hybrid(p_rrm_ue_ctxt,rrm_get_max_allowed_user(p_rrm_ue_ctxt->cell_index),
                        p_access_mgmt_params->max_non_csg_members);
                /*due to change in csg membership status we shall try HO of the UE if
                  csg validation fails*/
                if(RRM_FAILURE == ret_val)
                {
                    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                            "[UE:%d],CELL_INDEX[%d] csg validation failed handing over"
                            " this UE",
                            p_rrm_ue_ctxt->ue_index, p_rrm_ue_ctxt->cell_index);
                    if(p_rrm_ue_ctxt->rrm_csg_membership.rrm_csg_membership_status
                            == RRM_MEMBER)
                    {
                        /*As ue is csg member try to ho it to a closed cell*/
                        ret_val = rrm_handover_ue_to_best_cell(p_rrm_ue_ctxt);
                        /*SPR 17777 +-*/
                    }
                    else if(p_rrm_ue_ctxt->rrm_csg_membership.rrm_csg_membership_status
                            == RRM_NOT_MEMBER)
                    {
                        /*As ue is csg member try to ho it to a closed cell*/
                        ret_val = rrm_handover_ue_to_best_cell(p_rrm_ue_ctxt);
                        /*SPR 17777 +-*/
                    }
                }
            }
            break;
        default:
            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED,
                    "Not of any valid cell access mode");
            break;

    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/*csg end*/


/****************************************************************************
 * Function Name  : rrm_ue_rbc_erab_setup
 * Inputs         : UE context
 *                  ERAB to be setup
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function applies the RBC algorithms over the requested
 *                  ERAB to check if there are sufficient resources available
 *                  for setting up this ERAB. This function handles both GBR
 *                  and Non GBR ERAB setup requests.
 ****************************************************************************/
rrm_return_et
rrm_ue_rbc_erab_setup
(
 rrm_ue_context_t *p_rrm_ue_ctxt,
 /* SPR 19968 Start */
 rrm_ue_erab_setup_item_t *p_erab_to_be_setup,
 rrm_ue_ambr_t ue_agg_max_bit_rate,
 /* SPR 19968 END */
 U8 gbr_configd_count,
 U8 ngbr_configd_count,
 U8 gbr_success_count,
 U8 ngbr_success_count,
 /*SPR 18602 START*/
 /*code deleted*/
 /*SPR 18602 END*/
 /* FGI Feature Start */
 U32 *p_num_temp_um_mode,
 U32 *p_num_temp_am_mode,
 U32 *p_data_transfer_mode, /* FGI Feature End */
 /*SPR 18654 START*/
 U64               *p_out_dl_allocated_ngbr_prbs,
 U64              *p_out_ul_allocated_ngbr_prbs,
 U32               *p_out_dl_required_num_prbs,
 /* SPR 16853: start */
 U32               *p_out_ul_required_num_prbs,
 /*SPR 18654 END*/
 rrm_bool_et      is_default_bearer,
 /* SPR 16853: end */
/*spr 17437 fix start*/
 rrm_rab_removal_ctxt_t          *p_rab_removal_ctxt
/*spr 17437 fix stop*/
#ifdef ENDC_ENABLED
	,rrm_bool_et is_endc_ue_x2_up
	,rrm_bool_et *is_resource_available
#endif
 )
{
    RRM_UT_TRACE_ENTER();

    rrm_return_et ret_val = RRM_SUCCESS;
    rrm_bool_et is_gbr    = RRM_FALSE;
    /* SPR 16853: start */
    /* Code removed */
    /* SPR 16853: end */
    U8 ngbr_dl_prbs_released = RRM_ZERO;
    U8 ngbr_ul_prbs_released = RRM_ZERO;
    U8 gbr_dl_prbs_released = RRM_ZERO;
    U8 gbr_ul_prbs_released = RRM_ZERO;
    /*SPR 18602 START*/
    /*code deleted*/
    /*SPR 18602 END*/
    /* FGI Feature Start */
    U8		    data_transfer_mode_cnfgd = RRM_ZERO;
    rrm_cell_context_t     *p_cell_context = RRM_PNULL;
    /* FGI Feature End */
    
    RRM_ASSERT(RRM_PNULL != p_rrm_ue_ctxt);

    /*GBR LIMIT START */    
    admission_control_info_t* p_admission_control_info = RRM_NULL;
    p_admission_control_info =rrm_cellm_get_admission_control_info(
            p_rrm_ue_ctxt->cell_index);
    /*GBR LIMIT END */
    /* SPR 19968 Start */
    U8 qci = p_erab_to_be_setup->erab_level_qos_params.qci;
    /* SPR 19968 End */
    if(qci >= RRM_GBR_QCI_1 && qci <= RRM_GBR_QCI_4)
    {
        is_gbr = RRM_TRUE;
    }
    /* Start fix for Coverity_ID : 74983 */
    else if(RRM_ZERO == qci)
    {
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_ERROR, "QCI value is 0");
        ret_val = RRM_FAILURE;
        return ret_val;
    }
    else
    {
        /* Do Nothing */
    }
    /* End fix for Coverity_ID : 74983 */

    /*SPR 18602 START*/
    /*code deleted*/
    /*SPR 18602 END*/

    /*Start FGI validation block*/
    data_transfer_mode_cnfgd = rrm_get_data_transfer_mode_configd(qci,
                                                      p_rrm_ue_ctxt->cell_index);
    /* SPS related changes start */
    p_cell_context = rrm_cellm_get_cell_context(p_rrm_ue_ctxt->cell_index);
    /*cov_fix_65772_start*/
    if(RRM_PNULL == p_cell_context)
    { 
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_ERROR, "p_cell_context returned from rrm_cellm_get_cell_context is null");
        ret_val = RRM_FAILURE;
        RRM_UT_TRACE_EXIT();
        return ret_val;
    }
    /*cov_fix_65772_end*/
    if (RRM_TRUE == p_cell_context->sps_on)
    {
        ret_val = rrm_ue_sps_rab_to_be_established(p_rrm_ue_ctxt,
                /* SPR 19968 Start */
                *p_erab_to_be_setup, qci, &data_transfer_mode_cnfgd);
        /* SPR 19968 End */
    }
    
    if (RRM_FAILURE == ret_val)
    {
        RRM_UT_TRACE_EXIT();
        return ret_val;
    }
    /* SPS related changes end */
    /*SPR 19968 Start*/

    U64 dl_bitrate = RRM_ZERO;
    U64 ul_bitrate = RRM_ZERO;
    if(is_gbr)
    {
        dl_bitrate = p_erab_to_be_setup->erab_level_qos_params.
            gbr_qos_info.erab_guar_bit_rate_dl;
        ul_bitrate = p_erab_to_be_setup->erab_level_qos_params.
            gbr_qos_info.erab_guar_bit_rate_ul;
        /*SPR 19819 30JUN START*/
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_BRIEF, 
                "[ERB setup] GBR bearer bitrate [DL:%llu][UL:%llu].", 
                dl_bitrate, ul_bitrate);
        /*SPR 19819 30JUN END*/
        /*spr 17437 fix start*/
        /*Code Deleted*/
        /*spr 17437 fix stop*/
            /*Start gbr limit validation block*/
            /* SPR 21429 Fix Start */
            if(p_admission_control_info->bitmask & RRMCM_AVAILABLE_GBR_LIMIT_PRESENT)
            /* SPR 21429 Fix End */
            {
            /*spr 17437 fix start*/
            ret_val = rrm_ue_is_gbr_limit_exceeded(p_rrm_ue_ctxt, dl_bitrate,
                    ul_bitrate, p_erab_to_be_setup, RRM_ERB_REQ_TYPE_SETUP,
                    gbr_configd_count,
                    gbr_success_count,
                    p_rab_removal_ctxt,p_data_transfer_mode,p_num_temp_um_mode,p_num_temp_am_mode
#ifdef ENDC_ENABLED
                    ,is_endc_ue_x2_up
#endif
                    );
#ifdef ENDC_ENABLED
			if(is_endc_ue_x2_up && ret_val == RRM_FAILURE)
			{
				*is_resource_available = RRM_FALSE;
			}
#endif
            }

            /*End gbr limit validation block*/
        /*SPR 19968 +-*/        
    }
    else /* non-GBR */
    {
        ret_val = rrm_ue_check_config_ngbr_drbs(p_rrm_ue_ctxt,
                ngbr_configd_count,
                ngbr_success_count);
            
            if(RRM_SUCCESS == ret_val)
            {
            /* SPR_13815_fix: start */
            /* Lines deleted */
            /* SPR_13815_fix: end */
        /* SPR-15991_3 START */
        ul_bitrate = ue_agg_max_bit_rate.ue_agg_max_bit_rate_ul;
        dl_bitrate = ue_agg_max_bit_rate.ue_agg_max_bit_rate_dl;
        /* SPR-15991_3 END */

            RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED, "[ERB setup] NGBR bearer AMBR [DL:%llu][UL:%llu].", 
            dl_bitrate, ul_bitrate);
            /* SPR 16853: start */
            /* Code removed */
            /* SPR 16853: end */
            /*Start prb calulation block*/
        /* SPR-15991_2 START */
        ret_val = rac_admit_ngbr_bearer(p_rrm_ue_ctxt, ul_bitrate, dl_bitrate,
                    *p_erab_to_be_setup, is_default_bearer, gbr_dl_prbs_released, gbr_ul_prbs_released,
                ngbr_dl_prbs_released, ngbr_ul_prbs_released,
                p_out_dl_allocated_ngbr_prbs,
                p_out_ul_allocated_ngbr_prbs,
                p_out_dl_required_num_prbs,
                p_out_ul_required_num_prbs,
                /*spr 17437 fix start*/
                RRM_ERB_REQ_TYPE_SETUP,
                ngbr_configd_count,
                ngbr_success_count,
                p_rab_removal_ctxt
#ifdef ENDC_ENABLED
                    ,is_endc_ue_x2_up
#endif
					);
            if(RRM_FAILURE==ret_val)
            {
                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED, "Failure Returned from function rac_admit_ngbr_bearer");
#ifdef ENDC_ENABLED
                if(is_endc_ue_x2_up)
                {
                	*is_resource_available = RRM_FALSE;
                }
#endif
            }
            else
            {
                ret_val=rrm_uem_decide_am_um_mode(p_rrm_ue_ctxt,
                        data_transfer_mode_cnfgd,
                        p_data_transfer_mode,
                        p_num_temp_um_mode,
                        p_num_temp_am_mode,
                        *p_erab_to_be_setup);
                /* SPR 19968 End */
            }   
            /*End prb calulation block*/
            
            /* SPR_13815_fix: start */
            /* Lines deleted */
            /* SPR_13815_fix: end */
            /* End changes for SPR 490 */
            if(RRM_FAILURE == ret_val)
            {
#ifdef ENDC_ENABLED
                if(!is_endc_ue_x2_up)
#endif
                /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT START */
                /* SPR 19968 Start */
                rrm_raise_event_erab_adm_rej(p_erab_to_be_setup->erab_id, RRM_EVENT_LACK_OF_RESOURCES);
                /* SPR 19968 End */
                /*SPR 17777 +-*/
                /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT END*/
            }
        }
        else
        {

#ifdef ENDC_ENABLED
                if(is_endc_ue_x2_up)
                {
                	*is_resource_available = RRM_FALSE;
                }
                else
#endif
            /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT START */
            /* SPR 19968 Start */
            rrm_raise_event_erab_adm_rej(p_erab_to_be_setup->erab_id, RRM_EVENT_LACK_OF_RESOURCES);
            /* SPR 19968 End */
            /*SPR 17777 +-*/
            /* Raise Event RRM_EVENT_LOCAL_ERAB_ADMISSION_REJECT END*/
        }
    }
       if (RRM_TRUE == p_cell_context->sps_on)
       {
            if ( RRM_FAILURE == ret_val)
            {
            /* SPR 19968 Start */
            if (p_erab_to_be_setup->erab_id == p_rrm_ue_ctxt->ue_sps_data.sps_erab_id)
                /* SPR 19968 Start */
                    {
                        /* Parameters need to be reset in case if
                        ** this bearer becomes SPS bearer but RBC
                        ** fails due to some other reason after that*/
                        p_rrm_ue_ctxt->ue_sps_data.sps_erab_id = RRM_OUT_OF_RANGE;
                        p_rrm_ue_ctxt->ue_sps_data.is_sps_erab_established = RRM_FALSE;
                        p_cell_context->sps_ues_count--;
                    }
            }
       }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/* SPS related changes start */
/****************************************************************************
 * Function Name  : rrm_ue_sps_rab_to_be_established
 * Inputs         : p_rrm_ue_ctxt    - UE context
 *                  qci_val          - qci value corresponding to the requested RAB
 *                  erab_to_be_setup - ERAB requested
 * Outputs        : RRM_SUCCESS or RRM_FAILURE
 * Returns        : None
 * Description    : This function evaluates whether the requested RAB can be
 *                  established for SPS or not.
 ****************************************************************************/
rrm_return_et
rrm_ue_sps_rab_to_be_established(
    rrm_ue_context_t          *p_rrm_ue_ctxt,
    rrm_ue_erab_setup_item_t   erab_to_be_setup,
    U8                         qci_val,
    U8                        *data_transfer_mode_cnfgd
    )
{
    rrm_cell_context_t     *p_cell_context = RRM_PNULL;
    rrm_return_et           ret_val = RRM_SUCCESS;
    U16                     total_n1_pucch_an_for_sps = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    p_cell_context = rrm_cellm_get_cell_context(p_rrm_ue_ctxt->cell_index);
    /*CID 65775:start*/
    if (RRM_PNULL != p_cell_context)
    {
        /* SPR 8053 Fix start */
        if (RRM_ZERO == p_cell_context->sps_n1_pucch_an_start_indx)
        {
            total_n1_pucch_an_for_sps = RRM_ZERO;

            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "Total Pucch resources for SPS are %d", total_n1_pucch_an_for_sps);
        }
        else
        {
            /* SPR 11003 Fix Start */
            /* n1_pucch_an is the last index so 1 is added to get the totatl number i.e 0-> n1_pucch_an
             * sps_n1_pucch_an_start_indx is the absolute index so when index is subtracted from total
             * e.g n1_pucch_an 26, total 27, sps idx = 23, so 4 indexes are available 23,24,25,26
             */
            total_n1_pucch_an_for_sps = 
                ( (p_cell_context->ran_info.physical_layer_params.
                   physical_layer_param_pucch.n1_pucch_an + RRM_ONE) -
                  p_cell_context->sps_n1_pucch_an_start_indx);

            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Total Pucch resources for SPS are %d", total_n1_pucch_an_for_sps);
            /* SPR 11003 Fix End */
        }
        /* SPR 8053 Fix end */

        /* SPS configuration will be sent if the 29th bit of FGI is set to 1 */


        if(p_rrm_ue_ctxt->ue_capability_params.
                rrm_eutra_radio_capability_info.
                eutra_radio_capability_info_def.bitmask &
                RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT)
        {

            if ((RRM_FALSE != p_cell_context->sps_on) &&
                    /*Start:Bug 762*/
                    (RRM_TRUE  == rrm_is_set_fgi_bit(p_rrm_ue_ctxt,RRM_TWENTY_NINE)) && 
                    (RRM_TRUE  == rrm_is_set_fgi_bit(p_rrm_ue_ctxt,RRM_SEVEN)))
                /*End:Bug 762*/
            {
                if ((p_cell_context->epc_info.epc_params.qos_config_params[(qci_val-RRM_ONE)].bitmask & RRMCM_RMIF_SPS_CONFIG_DATA_PRESENT) &&
                        (p_cell_context->epc_info.epc_params.qos_config_params[(qci_val-RRM_ONE)].sps_config_enabled == RRM_TRUE))
                    /* Checking whether QCI is SPS enabled */
                {
                    if (RRM_FALSE == p_rrm_ue_ctxt->ue_sps_data.is_sps_erab_established)
                    {
                        if (((p_cell_context->sps_ues_count+RRM_ONE) <= total_n1_pucch_an_for_sps) &&
                                ((p_cell_context->sps_ues_count+RRM_ONE) <= p_cell_context->operator_info.admission_control_info.max_sps_ues))
                        {
                            if ( (p_rrm_ue_ctxt->ue_sps_data.sps_erab_highest_arp_priority == 
                                        erab_to_be_setup.erab_level_qos_params.alloc_and_reten_prior.priority_level) && 
                                    (p_rrm_ue_ctxt->ue_sps_data.sps_erab_highest_qci_priority == 
                                     p_cell_context->epc_info.epc_params.qos_config_params[(qci_val-RRM_ONE)].priority) )
                            {
                                /* First Come First Serve, if the SPS competitor RABs have same ARP priority and same QCI priority */
                                p_rrm_ue_ctxt->ue_sps_data.sps_erab_id = 
                                    erab_to_be_setup.erab_id;
                            /* SPR 18418 Fix Start */
                            /* Code Removed */
                            /* SPR 18418 Fix Stop */
                                p_cell_context->sps_ues_count++;
                                *data_transfer_mode_cnfgd = UM_MODE;
                            }
                            else
                            {
                                /* Reject lower ARP/QCI priority ERAB request */ 
                                RRM_UT_TRACE_EXIT();
                                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                                        "[UE:%d],ERAB_ID[%d] rejected because of lower ARP/QCI priorities",
                                        p_rrm_ue_ctxt->ue_index,erab_to_be_setup.erab_id);
                                return RRM_FAILURE;
                            }
                        }
                        else
                        {
                            /* SPR 8053 Fix start */

                            /* Reject ERAB request if max. SPS UE are already attached with SPS ERAB 
                               or PUCCH resources are lesser than what is required */ 

                            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,
                                    "[UE:%d],ERAB_ID[%d] rejected because MAX SPS UE are already attached" 
                                    " or PUCCH resources are lesser than what is required"
                                    " SPS UEs count is [%d] and Total N1 PUCCH resources for SPS are [%d]"
                                    " and max SPS UEs configured are [%d]",
                                    p_rrm_ue_ctxt->ue_index,erab_to_be_setup.erab_id,
                                    p_cell_context->sps_ues_count,
                                    total_n1_pucch_an_for_sps,
                                    p_cell_context->operator_info.admission_control_info.max_sps_ues);

                            /* SPR 8053 Fix end */
                            RRM_UT_TRACE_EXIT();
                            return RRM_FAILURE;
                        }
                    }
                    else
                    {
                        /* Reject ERAB request if this UE has already SPS ERAB */ 
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,
                                "[UE:%d],ERAB_ID[%d] rejected because UE has already SPS ERAB",
                                p_rrm_ue_ctxt->ue_index,erab_to_be_setup.erab_id);
                        RRM_UT_TRACE_EXIT();
                        return RRM_FAILURE;
                    }
                }
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,
                        "[UE:%d],FGI BIT 29 for SPS is OFF or SPS is not acivated",
                        p_rrm_ue_ctxt->ue_index);
            }
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"p_cell_context is null");
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    /*CID 65775:end*/

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPS related changes end */
/****************************************************************************
 * Function Name  : get_earlier_gbr_bitrate
 * Inputs         : drb_configured - pointer to the configured DRBs
 *                  erab_id        - id of the ERAB for which the bitrate is
 *                                   to be found
 * Outputs        : ul_bitrate     - earlier uplink bitrate of DRB
 *                  dl_bitrate     - earlier downlink bitrate of DRB
 * Returns        : None
 * Description    : This function finds the uplink and downlink bitrate of an
 *                  already configured ERAB for which the ERB modify request
 *                  is received. The earlier bitrates would be required to
 *                  calculate the no. of additional PRBs to be allocated.
 ****************************************************************************/
rrm_void_t
get_earlier_gbr_bitrate
(
 rrm_ue_erab_setup_item_list_t *p_drb_configured,
 U8 erab_id,
 U64 *p_ul_bitrate,
 U64 *p_dl_bitrate,
 U8  *p_old_qci
 )
{
    RRM_UT_TRACE_ENTER();
    U8 erab_index = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_drb_configured);

    for(;erab_index<p_drb_configured->num_of_list;erab_index++)
    {
        if(erab_id == p_drb_configured->erab_item[erab_index].erab_id)
        {
            *p_ul_bitrate = p_drb_configured->erab_item[erab_index].
                erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_ul;
            *p_dl_bitrate = p_drb_configured->erab_item[erab_index].
                erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_dl;
            *p_old_qci = p_drb_configured->erab_item[erab_index].
                             erab_level_qos_params.qci;
            break;
        }
    }
    RRM_UT_TRACE_EXIT();
}

/*qos modification starts*/
/****************************************************************************
 * Function Name  : rrm_ue_get_dl_released_prbs
 * Inputs         : UE context
 *                  Requested UE-AMBR
 *                  DL AMBR released
 *                  Ngbr DL PRBs released
 *                  DL gbr released
 *                  gbr DL PRBs released
 *                  ERAB to be modified
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function calculates the gbr or ngbr prbs to be 
 *                  released if any
 ****************************************************************************/
rrm_return_et
rrm_ue_get_dl_released_prbs
(
 rrm_ue_context_t *p_rrm_ue_ctxt,
 rrm_ue_ambr_t ue_agg_max_bit_rate,
 S8 *dl_ambr_released,
 /*SPR 18654 START*/
 U64 *ngbr_dl_prbs_released,
 S8 *dl_gbr_released,
 U64 *gbr_dl_prbs_released,
 /*SPR 18654 END*/ 
 rrm_ue_erab_setup_item_t erab_to_modify
 )
{
    RRM_UT_TRACE_ENTER();

    rrm_return_et ret_val = RRM_SUCCESS;
    /*SPR 22151 Fix Start*/
    rrm_prb_t ambr_diff_dl_bitrate = RRM_ZERO;
    /*SPR 22151 Fix End*/
    rrm_bool_et is_new_rab_gbr = RRM_FALSE;
    rrm_bool_et is_old_rab_gbr = RRM_FALSE;
    U8 old_qci = RRM_ZERO;
    U64 curr_gbr_dl_bitrate = RRM_ZERO;
    U64 prev_gbr_ul_bitrate = RRM_ZERO;
    U64 prev_gbr_dl_bitrate = RRM_ZERO;
    rrm_bool_et congestion_flag = RRM_FALSE;
    RRM_ASSERT(RRM_PNULL != p_rrm_ue_ctxt);

    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;
    U64 prev_ambr_dl_bitrate = p_rrm_ue_ctxt->ue_agg_max_bit_rate.ue_agg_max_bit_rate_dl;
    U64 current_ambr_dl_bitrate = ue_agg_max_bit_rate.ue_agg_max_bit_rate_dl;
    /*SPR 22151 Fix Start*/
    /*SPR 18654 START*/
    U16   rb_factor_for_bitrate = RRM_ZERO;
    ret_val = rrm_get_rb_factor_for_bitrate(p_rrm_ue_ctxt->cell_index, &rb_factor_for_bitrate);
    if(ret_val == RRM_FAILURE)
    {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "rb_factor_for_bitrate not found ");
    }
    /*SPR 18654 END*/
    /*SPR 22151 Fix End*/

    /*Radio Congestion Control Feature*/
    switch (rrm_cellm_erb_setup_and_modification_allowed(p_rrm_ue_ctxt->cell_index))
    {
        case RRM_ALL_CALLS:
            break;

        case RRM_PRIVILEGE_CALLS:
        case RRM_EMRGCY_CALLS:
            /* Fall through */
        default:
            congestion_flag = RRM_TRUE;
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                    "Congestion Occured or Building Up, so ERB Modification Not Allowed in case of upgrade");
            break;
    }

    /*Checking if UE-AMBR has decreased, calculating prbs to be released*/
    if(prev_ambr_dl_bitrate > current_ambr_dl_bitrate)
    {
        *dl_ambr_released = RRM_ONE;
        ambr_diff_dl_bitrate = prev_ambr_dl_bitrate - current_ambr_dl_bitrate;

        U8 ngbr_threshold = rrm_cellm_get_ngbr_threshold(cell_index);
        U8 non_default_bearer_multiplier =
            rrm_cellm_get_non_default_bearer_multiplier(cell_index);


        /*SPR 22151 Fix Start*/
        ambr_diff_dl_bitrate = ambr_diff_dl_bitrate * non_default_bearer_multiplier;

        ambr_diff_dl_bitrate  = ambr_diff_dl_bitrate * ngbr_threshold;
        /*SPR 19968 code deleted*/
        *ngbr_dl_prbs_released = (U64)rrm_convert_bitrate_to_prb_dl(cell_index,ambr_diff_dl_bitrate);
        /*SPR 22151 Fix End*/

        /*SPR 18654 START*/
        /*code deleted*/
        /* SPR_12473_FIX_start */
        /* SPR_12473_FIX_end */
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "ambr_diff_dl_bitrate[%llu],ngbr_dl_prbs_released[%llu]",
        ambr_diff_dl_bitrate,*ngbr_dl_prbs_released);


        /*SPR 18654 END*/
    }
    /*if UE-AMBR has not changed, no need to release any prbs*/
    /* SPR-18198 START */
    else if (prev_ambr_dl_bitrate < current_ambr_dl_bitrate)
    /* SPR-18198 END */
    {
        if(RRM_TRUE == congestion_flag)
        {
            return RRM_FAILURE;
        }
        *dl_ambr_released = RRM_MINUS_ONE;
        *ngbr_dl_prbs_released = RRM_ZERO;
    }

    U8 new_qci = erab_to_modify.erab_level_qos_params.qci;
    U8 erab_id = erab_to_modify.erab_id;

    curr_gbr_dl_bitrate = erab_to_modify.erab_level_qos_params.
                              gbr_qos_info.erab_guar_bit_rate_dl;
    get_earlier_gbr_bitrate(&(p_rrm_ue_ctxt->drb_configured), erab_id, 
                            &prev_gbr_ul_bitrate, &prev_gbr_dl_bitrate, &old_qci);
    /* SPR-18198 START */
    /* code deleted */
    /* SPR-18198 END */

    if(new_qci >= RRM_GBR_QCI_1 && new_qci <= RRM_GBR_QCI_4)
    {
        is_new_rab_gbr = RRM_TRUE;
    }
    if(old_qci >= RRM_GBR_QCI_1 && old_qci <= RRM_GBR_QCI_4)
    {
        is_old_rab_gbr = RRM_TRUE;
    }

    /*Modification for GBR to GBR*/
    if(is_new_rab_gbr && is_old_rab_gbr)
    {

        /* SPR-15991_2 START */
        if(prev_gbr_dl_bitrate > curr_gbr_dl_bitrate)
        {
            /* SPR-18198 START */
            U64 diff_gbr_dl_bitrate = prev_gbr_dl_bitrate - curr_gbr_dl_bitrate;
            /* SPR-18198 END */
            *dl_gbr_released = RRM_ONE;
            /*SPR 18654 START*/
            /*code deleted*/
            *gbr_dl_prbs_released = (U64)RRM_CEIL((float)((rb_factor_for_bitrate *diff_gbr_dl_bitrate * (float)rrm_cellm_get_num_prb_based_on_snr
                            (cell_index, RRM_FALSE))/(float)(RRM_THREE*RRM_MEGA)));
            /* SPR 19968 END */

        }
        /* SPR-18198 START */
        else if (prev_gbr_dl_bitrate < curr_gbr_dl_bitrate)
        /* SPR-18198 END */
        {
            if(RRM_TRUE == congestion_flag)
            {
                return RRM_FAILURE;
            }
            *dl_gbr_released = RRM_MINUS_ONE;
            *gbr_dl_prbs_released = RRM_ZERO;
        }

    }
    /*Modification for GBR to NGBR*/
    else if((!is_new_rab_gbr) && is_old_rab_gbr)
    {
        /* SPR-15991_2 START */
        *dl_gbr_released = RRM_ONE;
        /*SPR 18654 START*/
        /*code deleted*/
        *gbr_dl_prbs_released = (U64)RRM_CEIL((float)((rb_factor_for_bitrate * prev_gbr_dl_bitrate * (float)rrm_cellm_get_num_prb_based_on_snr
                        (cell_index, RRM_FALSE))/(float)(RRM_THREE*RRM_MEGA)));
    }
    /*Modification for NGBR to GBR*/
    else if((is_new_rab_gbr) && (!is_old_rab_gbr))
    {
        *dl_gbr_released = RRM_MINUS_ONE;
        *gbr_dl_prbs_released = RRM_ZERO;
    }


    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_ue_get_ul_released_prbs
 * Inputs         : UE context
 *                  Requested UE-AMBR
 *                  UL AMBR released
 *                  Ngbr UL PRBs released
 *                  UL gbr released
 *                  gbr UL PRBs released
 *                  ERAB to be modified
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function calculates the gbr or ngbr prbs to be 
 *                  released if any
 ****************************************************************************/
rrm_return_et
rrm_ue_get_ul_released_prbs
(
 rrm_ue_context_t *p_rrm_ue_ctxt,
 rrm_ue_ambr_t ue_agg_max_bit_rate,
 S8 *ul_ambr_released,
 /*SPR 18654 START*/
 U64 *ngbr_ul_prbs_released,
 S8 *ul_gbr_released,
 U64 *gbr_ul_prbs_released,
 /*SPR 18654 END*/
 rrm_ue_erab_setup_item_t erab_to_modify
 )
{
    RRM_UT_TRACE_ENTER();
    rrm_return_et ret_val = RRM_SUCCESS;
    /*SPR 22151 Fix Start*/
    rrm_prb_t ambr_diff_ul_bitrate = RRM_ZERO;
    /*SPR 22151 Fix End*/
    rrm_bool_et is_new_rab_gbr = RRM_FALSE;
    rrm_bool_et is_old_rab_gbr = RRM_FALSE;
    U8 old_qci = RRM_ZERO;
    U64 curr_gbr_ul_bitrate = RRM_ZERO;
    U64 prev_gbr_ul_bitrate = RRM_ZERO;
    U64 prev_gbr_dl_bitrate = RRM_ZERO;
    rrm_bool_et congestion_flag = RRM_FALSE;
    RRM_ASSERT(RRM_PNULL != p_rrm_ue_ctxt);

    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;
    U64 prev_ambr_ul_bitrate = p_rrm_ue_ctxt->ue_agg_max_bit_rate.
        ue_agg_max_bit_rate_ul;
    U64 current_ambr_ul_bitrate = ue_agg_max_bit_rate.ue_agg_max_bit_rate_ul;
    /*SPR 22151 Fix Start*/
    /*SPR 18654 START*/
    U16   rb_factor_for_bitrate = RRM_ZERO;
    ret_val = rrm_get_rb_factor_for_bitrate(p_rrm_ue_ctxt->cell_index, &rb_factor_for_bitrate);
    if(ret_val == RRM_FAILURE)
    {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "rb_factor_for_bitrate not found ");
    }
    /*SPR 18654 END*/
    /*SPR 22151 Fix End*/

    /*Radio Congestion Control Feature*/
    switch (rrm_cellm_erb_setup_and_modification_allowed(p_rrm_ue_ctxt->cell_index))
    {
        case RRM_ALL_CALLS:
            break;

        case RRM_PRIVILEGE_CALLS:
        case RRM_EMRGCY_CALLS:
            /* Fall through */
        default:
            congestion_flag = RRM_TRUE;
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                    "Congestion Occured or Building Up, so ERB Modification Not Allowed in case of upgrade");
            break;
    }

    /*Checking if UE-AMBR has decreased, calculating UL prbs to be released*/
    if(prev_ambr_ul_bitrate > current_ambr_ul_bitrate)
    {
        *ul_ambr_released = RRM_ONE;
        ambr_diff_ul_bitrate = prev_ambr_ul_bitrate - current_ambr_ul_bitrate;

        U8 ngbr_threshold = rrm_cellm_get_ngbr_threshold(cell_index);
        U8 non_default_bearer_multiplier =
            rrm_cellm_get_non_default_bearer_multiplier(cell_index);

        /*SPR 22151 Fix Start*/
        ambr_diff_ul_bitrate = ambr_diff_ul_bitrate * non_default_bearer_multiplier;

        ambr_diff_ul_bitrate = ambr_diff_ul_bitrate * ngbr_threshold;
        /*SPR 18654 FIX code deleted*/
        *ngbr_ul_prbs_released = (U64)rrm_convert_bitrate_to_prb_ul(cell_index,ambr_diff_ul_bitrate); 
        /*SPR 22151 Fix End*/
   }
    /* SPR-18198 START */
    else if(prev_ambr_ul_bitrate < current_ambr_ul_bitrate)
        /* SPR-18198 END */
    {
        if(RRM_TRUE == congestion_flag)
        {
            return RRM_FAILURE;
        }
        *ul_ambr_released = RRM_MINUS_ONE;
        *ngbr_ul_prbs_released = RRM_ZERO;
    }

    U8 new_qci = erab_to_modify.erab_level_qos_params.qci;
    U8 erab_id = erab_to_modify.erab_id;

    curr_gbr_ul_bitrate = erab_to_modify.erab_level_qos_params.gbr_qos_info.
        erab_guar_bit_rate_ul;
    get_earlier_gbr_bitrate(&(p_rrm_ue_ctxt->drb_configured), erab_id,
            &prev_gbr_ul_bitrate, &prev_gbr_dl_bitrate, &old_qci);
    /* SPR-18198 START */
    /* code deleted */
    /* SPR-18198 END */

    if(new_qci >= RRM_GBR_QCI_1 && new_qci <= RRM_GBR_QCI_4)
    {
        is_new_rab_gbr = RRM_TRUE;
    }
    if(old_qci >= RRM_GBR_QCI_1 && old_qci <= RRM_GBR_QCI_4)
    {
        is_old_rab_gbr = RRM_TRUE;
    }

    if(is_new_rab_gbr && is_old_rab_gbr)
    {
        if(prev_gbr_ul_bitrate > curr_gbr_ul_bitrate)
        {
            /* SPR-18198 START */
            U64 diff_gbr_ul_bitrate = prev_gbr_ul_bitrate - curr_gbr_ul_bitrate;
            /* SPR-18198 END */
        /* SPR-15991_2 START */
            *ul_gbr_released = RRM_ONE;
            /*SPR 18654 START*/
            /*code deleted*/
            *gbr_ul_prbs_released = (U64)RRM_CEIL((float)((rb_factor_for_bitrate *diff_gbr_ul_bitrate * (float)rrm_cellm_get_num_prb_based_on_snr
                            (cell_index, RRM_FALSE))/(float)(RRM_THREE*RRM_MEGA)));

        /* SPR-15991_2 END */

        }
        /* SPR-18198 START */
        else if(prev_gbr_ul_bitrate < curr_gbr_ul_bitrate)
            /* SPR-18198 END */
        {
            if(RRM_TRUE == congestion_flag)
            {
                return RRM_FAILURE;
            }
            *ul_gbr_released = RRM_MINUS_ONE;
            *gbr_ul_prbs_released = RRM_ZERO;
        }
    }
    /*Modification for GBR to NGBR*/
    else if((!is_new_rab_gbr) && is_old_rab_gbr)
    {
        /* SPR-15991_2 START */
       *ul_gbr_released = RRM_ONE;
       /* SPR-15991_2 END */
        /*SPR 18654 START*/
        /*code deleted*/
        *gbr_ul_prbs_released = (U64)RRM_CEIL((float)((rb_factor_for_bitrate * prev_gbr_ul_bitrate * (float)rrm_cellm_get_num_prb_based_on_snr
                        (cell_index, RRM_FALSE))/(float)(RRM_THREE*RRM_MEGA)));
    }
    /*Modification for NGBR to GBR*/
    else if((is_new_rab_gbr) && (!is_old_rab_gbr))
    {
        *ul_gbr_released = RRM_MINUS_ONE;
        *gbr_ul_prbs_released = RRM_ZERO;
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}


/****************************************************************************
 * Function Name  : rrm_ue_rbc_erab_modify
 * Inputs         : UE context
 *                  ERAB to be modified
 *                  Requested UE-AMBR
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function applies the RBC algorithms over the requested
 *                  ERAB to check if there are sufficient resources available
 *                  for modifying this ERAB. This function handles both GBR
 *                  and Non GBR ERAB setup requests.
 ****************************************************************************/


rrm_return_et
rrm_ue_rbc_erab_modify
(
 rrm_ue_context_t *p_rrm_ue_ctxt,
 rrm_ue_erab_setup_item_t erab_to_modify,
 rrm_ue_ambr_t ue_agg_max_bit_rate,
/*SPR 18654 START*/
 U64               *p_out_dl_allocated_ngbr_prbs,
 U64              *p_out_ul_allocated_ngbr_prbs,
 U32               *p_out_dl_required_num_prbs,
 U32               *p_out_ul_required_num_prbs,
/*SPR 18654 END*/
 /* SPR 15834 Fix Start */
rrm_ue_cause_t   *p_erb_setup_modify_fail_cause,
 /* SPR 15834 Fix End */
/*spr 17437 fix start*/
rrm_rab_removal_ctxt_t          *p_rab_removal_ctxt
/*spr 17437 fix stop*/
 )
{
    RRM_UT_TRACE_ENTER();
    rrm_return_et ret_val = RRM_SUCCESS;
    rrm_bool_et is_new_rab_gbr    = RRM_FALSE;
    S8 dl_ambr_released = RRM_ZERO;
    S8 ul_ambr_released = RRM_ZERO;
/*SPR 18654 START*/
    U64 ngbr_dl_prbs_released = RRM_ZERO;
    U64 ngbr_ul_prbs_released = RRM_ZERO;
    S8 dl_gbr_released = RRM_ZERO;
    S8 ul_gbr_released = RRM_ZERO;
    U64 gbr_ul_prbs_released = RRM_ZERO;
    U64 gbr_dl_prbs_released = RRM_ZERO;
/*SPR 18654 END*/
    /*SPR 19968 START*/
    /* CID Fix 115491 +*/
    U32 p_data_transfer_mode = RRM_ZERO;
    /* CID Fix 115491 -*/
    /*SPR 19968 END*/
    /* SPR-15991_2 START */
    rrm_bool_et is_default_bearer = RRM_FALSE;
    /* SPR-15991_2 END */
    /* SPR-15991_3_5NOV Start */
    /*SPR 18654 START*/
    U64 dl_allocated_ngbr_prbs = RRM_ZERO;
    U64 ul_allocated_ngbr_prbs = RRM_ZERO;
    /*SPR 18654 END*/
    /* SPR-15991_3_5NOV End */
    U8 index = RRM_ZERO;

    RRM_ASSERT(RRM_PNULL != p_rrm_ue_ctxt);


    U8 new_qci = erab_to_modify.erab_level_qos_params.qci;
    /*If qci is modified for the bearer fail the bearer
      for qci modificatio*/
    for(index = RRM_ZERO;index < p_rrm_ue_ctxt->drb_configured.num_of_list;
            index ++)
    {
        if(p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_id ==
                erab_to_modify.erab_id)
        {
            if(p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_level_qos_params.qci !=
                    erab_to_modify.erab_level_qos_params.qci)
            {
                RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED, "QCI modification is not allowed "
                        "failing the bearer, Rab Id = %d", erab_to_modify.erab_id);
                /* SPR 15834 Fix Start */
                p_erb_setup_modify_fail_cause->type  = RADIO_NETWORK_LAYER_TYPE; 
                p_erb_setup_modify_fail_cause->value = UNSPECIFIED; 
                /* SPR 15834 Fix End */
                return RRM_FAILURE;
            }
        }
    }

    if(new_qci >= RRM_GBR_QCI_1 && new_qci <= RRM_GBR_QCI_4)
    {
        is_new_rab_gbr = RRM_TRUE;
    }
    U64 ul_bitrate = RRM_ZERO;
    U64 dl_bitrate = RRM_ZERO;

    U64 ngbr_ul_bitrate = ue_agg_max_bit_rate.ue_agg_max_bit_rate_ul;
    U64 ngbr_dl_bitrate = ue_agg_max_bit_rate.ue_agg_max_bit_rate_dl;
    /* SPR-15991_2 START */
    RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED, "[ERB modify] NGBR bearer AMBR [DL:%llu][UL:%llu]",
                 ngbr_dl_bitrate, ngbr_ul_bitrate);

    if(is_new_rab_gbr)
    {
        dl_bitrate = erab_to_modify.erab_level_qos_params.gbr_qos_info.
            erab_guar_bit_rate_dl;
        ul_bitrate = erab_to_modify.erab_level_qos_params.gbr_qos_info.
            erab_guar_bit_rate_ul;
        RRM_UEM_RAC_TRACE(g_uem_log_on_off, RRM_DETAILED, "[ERB modify] GBR bearer bitrate [DL:%llu][UL:%llu]",
            dl_bitrate, ul_bitrate);
    }
    /* SPR-15991_2 END */
/*GBR LIMIT START */
    admission_control_info_t* p_admission_control_info = RRM_NULL;
    p_admission_control_info = rrm_cellm_get_admission_control_info(p_rrm_ue_ctxt->cell_index);


        /* SPR-15991_2 START */
        switch (rrm_cellm_erb_setup_and_modification_allowed(p_rrm_ue_ctxt->cell_index))
        {
            case RRM_ALL_CALLS:
            case RRM_PRIVILEGE_CALLS:
            case RRM_EMRGCY_CALLS:
                /* Fall through */
                /* BUG:220 Start */
                ret_val = rrm_ue_get_dl_released_prbs(p_rrm_ue_ctxt, ue_agg_max_bit_rate, &dl_ambr_released,
                        &ngbr_dl_prbs_released, &dl_gbr_released,
                        &gbr_dl_prbs_released, erab_to_modify);
                if(RRM_SUCCESS == ret_val)
                {
                    ret_val = rrm_ue_get_ul_released_prbs(p_rrm_ue_ctxt, ue_agg_max_bit_rate, &ul_ambr_released,
                            &ngbr_ul_prbs_released, &ul_gbr_released,
                            &gbr_ul_prbs_released, erab_to_modify);
                }

                /* BUG:220 End*/
                break;

            default:
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                        "Congestion Occured or Building Up, so ERB Modification Not Allowed in case of upgrade");
                break;
        }

        /* SPR 21429 Fix Start */
        if((p_admission_control_info->bitmask & RRMCM_AVAILABLE_GBR_LIMIT_PRESENT))
            /* SPR 21429 Fix End */
        {
            if((RRM_SUCCESS == ret_val) && (is_new_rab_gbr))
            {
                ret_val = rrm_ue_is_gbr_limit_exceeded(p_rrm_ue_ctxt, dl_bitrate,
                        /*spr 19968 fix start*/ 
                        ul_bitrate, &erab_to_modify, RRM_ERB_REQ_TYPE_MODIFY,RRM_ZERO,RRM_ZERO,p_rab_removal_ctxt,&p_data_transfer_mode,RRM_NULL,RRM_NULL
#ifdef ENDC_ENABLED
						,RRM_FALSE
#endif
                        );
                        /*spr 19968 fix stop*/ 
            }
    }
        if((RRM_SUCCESS == ret_val) && ((RRM_MINUS_ONE == dl_gbr_released) || (RRM_MINUS_ONE == ul_gbr_released)))
        {
            ret_val = rac_admit_gbr_bearer(p_rrm_ue_ctxt, ul_bitrate,
                    dl_bitrate, erab_to_modify, ngbr_dl_prbs_released, ngbr_ul_prbs_released,
                    gbr_dl_prbs_released, gbr_ul_prbs_released
            /*spr 17437 fix start*/
            ,RRM_ERB_REQ_TYPE_MODIFY,RRM_ZERO,RRM_ZERO,p_rab_removal_ctxt /*Kshitij 13589 rel1.3*/
#ifdef ENDC_ENABLED
						,RRM_FALSE
#endif
						);
            /*spr 17437 fix stop*/
        }

        if((RRM_SUCCESS == ret_val) && ((RRM_MINUS_ONE == dl_ambr_released) || (RRM_MINUS_ONE == ul_ambr_released)))
        {
            ret_val = rac_admit_ngbr_bearer(p_rrm_ue_ctxt, ngbr_ul_bitrate,
                    ngbr_dl_bitrate, erab_to_modify, is_default_bearer,
                    gbr_dl_prbs_released, gbr_ul_prbs_released,
                    ngbr_dl_prbs_released, ngbr_ul_prbs_released,
                    p_out_dl_allocated_ngbr_prbs,
                    p_out_ul_allocated_ngbr_prbs,
                    p_out_dl_required_num_prbs,
                    p_out_ul_required_num_prbs,
            /*spr 17437 fix start*/
            RRM_ERB_REQ_TYPE_MODIFY,RRM_ZERO,RRM_ZERO,p_rab_removal_ctxt
#ifdef ENDC_ENABLED
			,RRM_FALSE
#endif
            ); /*13588 rel1.3*/
                    /*spr 17437 fix stop*/
        }

        if((RRM_SUCCESS == ret_val) && ((RRM_ONE == dl_gbr_released) || (RRM_ONE == ul_gbr_released)))
        {
            ret_val = rac_release_gbr_bearer(p_rrm_ue_ctxt, erab_to_modify,
                                            gbr_dl_prbs_released, gbr_ul_prbs_released);
        }
        if((RRM_SUCCESS == ret_val) && ((RRM_ONE == dl_ambr_released) || (RRM_ONE == ul_ambr_released)))
        {
            /* SPR-15991_3_5NOV Start */
            dl_allocated_ngbr_prbs = rrm_cellm_get_dl_allocated_ngbr_prb(p_rrm_ue_ctxt->cell_index);
            ul_allocated_ngbr_prbs = rrm_cellm_get_ul_allocated_ngbr_prb(p_rrm_ue_ctxt->cell_index);
            ret_val = rac_release_ngbr_bearer(p_rrm_ue_ctxt, ngbr_dl_prbs_released, ngbr_ul_prbs_released,
                                            dl_allocated_ngbr_prbs, ul_allocated_ngbr_prbs);
            /* SPR-15991_3_5NOV End */
        }
        /* SPR-15991_2 END */

    /*GBR LIMIT END */
    RRM_UT_TRACE_EXIT();
    return ret_val;

}

/*LTE_RRM_TM_SWITCH_START*/
/****************************************************************************
 * Function Name  : rrm_ue_set_phy_antenna_info
 * Inputs         : UE Index
 *       	    Antenna info need to be set	
 * Outputs        : Antenna Info
 * Returns        : void 
 * Description    : This is Setter Method to set the Ue's Antenna information
 ****************************************************************************/
rrm_return_et
rrm_ue_set_phy_antenna_info(rrm_ue_index_t 	ue_index,
		     	    rrm_cell_index_t 	cell_index,
			    const rrm_ue_phy_antenna_information_t *p_ue_antenna_info)
{

    rrm_ue_context_t	*p_ue_context = RRM_PNULL;
    /* Fix for Coverity, LTE_RRM_MALL:CID:11136 [Prasant] */
    /* SPR 20652 Fix Start */
    /* Code Removed */
    rrm_return_et 	 ret_val = RRM_SUCCESS;

    RRM_UT_TRACE_ENTER();
    /* Fix for Coverity, LTE_RRM_MALL:CID:11136 [Prasant] */
    if (RRM_PNULL != (p_ue_context = (rrm_ue_find_context(ue_index,cell_index))))
    {    
        /* SPR 20652 Fix End */
       
        if( (RRM_PNULL != p_ue_antenna_info) && (RRM_PNULL != p_ue_context) )
        {
            p_ue_context->ue_phy_config_information.ue_antenna_information = *p_ue_antenna_info;
        }
        else
        {
	        RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_ERROR, 
                          "Setter method could not set the Antenna Information for" 
                           " [UE:%d],cell_index[%d]",
                           ue_index,cell_index);
	        ret_val = RRM_FAILURE;
        }
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_uem_facility_name, RRM_ERROR, 
                "UE context not found");
    }
    /* SPR 20652 Fix Start */
    /* Code removed */
    /* SPR 20652 Fix End */
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_ue_set_phy_antenna_info_r10
 * Inputs         : UE Index
 *                  Antenna info need to be set
 * Outputs        : Antenna Info
 * Returns        : void
 * Description    : This is Setter Method to set the Ue's Antenna information
 ****************************************************************************/
rrm_return_et
rrm_ue_set_phy_antenna_info_r10(rrm_ue_index_t      ue_index,
                                rrm_cell_index_t    cell_index,
                                const rrm_ue_phy_antenna_info_r10_t *p_ue_antenna_info)
{

    rrm_ue_context_t    *p_ue_context = RRM_PNULL;
    /* SPR 20652 Fix Start */
    /* Code Removed */
    rrm_return_et        ret_val = RRM_SUCCESS;

    RRM_UT_TRACE_ENTER();
    if (RRM_PNULL != (p_ue_context = (rrm_ue_find_context(ue_index,cell_index))))
    {
        /* SPR 20652 Fix End */

        if( (RRM_PNULL != p_ue_antenna_info) && (RRM_PNULL != p_ue_context) )
        {
            p_ue_context->ue_phy_config_information.ue_antenna_info_r10 = *p_ue_antenna_info;
        }
        else
        {
                RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_ERROR,
                          "Setter method could not set the Antenna Information for"
                           " [UE:%d],cell_index[%d]",
                           ue_index,cell_index);
                ret_val = RRM_FAILURE;
        }
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_uem_facility_name, RRM_ERROR, 
                "UE context not found");
    }
    /* SPR 20652 Fix Start */
    /* Code Removed */
    /* SPR 20652 Fix End */
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_set_phy_antenna_info_v920
 * Inputs         : UE Index
 *                  Antenna info need to be set
 * Outputs        : Antenna Info
 * Returns        : void
 * Description    : This is Setter Method to set the Ue's Antenna information
 ****************************************************************************/
rrm_return_et
rrm_set_phy_antenna_info_v920( rrm_ue_context_t  *p_ue_context,
                               const rrm_ue_phy_antenna_info_v920_t
                                                 *p_ue_antenna_info_v920 )
{

    rrm_return_et 	 ret_val = RRM_SUCCESS;

    RRM_UT_TRACE_ENTER();

    if( ( RRM_NULL != p_ue_context ) && ( RRM_NULL != p_ue_antenna_info_v920 ) )
    {
        p_ue_context->ue_phy_config_information.ue_antenna_info_v920 = *p_ue_antenna_info_v920;
    }
    else
    {
         RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_ERROR,
                   "NULL Pointer for UE context/antenna_info_v920");
         ret_val = RRM_FAILURE;
    }

    return ret_val;
}


/****************************************************************************
 * Function Name  : rrm_ue_get_phy_antenna_info
 * Inputs         : UE Index
		  : CELL Index
 * Outputs        : Antenna Info
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Getter Method to return the ue's antenna information
		    user are required to check for NULL pointer for received
		    antenna info pointer.
 ****************************************************************************/
const rrm_ue_phy_antenna_information_t*
rrm_ue_get_phy_antenna_info( rrm_ue_index_t	 ue_index,
			     rrm_cell_index_t	 cell_index )
{
    rrm_ue_context_t   *p_ue_context = RRM_PNULL;
    /* Fix for Coverity, LTE_RRM_MALL:CID:11136 [Prasant] */
    /* SPR 20652 Fix Start */
    /* Code Removed */
    const rrm_ue_phy_antenna_information_t *p_ue_antenna_information = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    /* Fix for Coverity, LTE_RRM_MALL:CID:11136 [Prasant] */
    if (RRM_PNULL != (p_ue_context = (rrm_ue_find_context(ue_index,cell_index))))
    {    
        /* SPR 20652 Fix End */
    
        if( p_ue_context != RRM_PNULL )
        {
     	    p_ue_antenna_information =  &(p_ue_context->ue_phy_config_information.ue_antenna_information);
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_ERROR, 
                      "could not get the Antenna Information for [UE:%d],cell_index[%d]",
                      ue_index,cell_index);
        }
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_uem_facility_name, RRM_ERROR, 
                "UE context not found");
    }
    /* SPR 20652 Fix Start */
    /* Code Removed */
    /* SPR 20652 Fix End */
    RRM_UT_TRACE_EXIT();

    return p_ue_antenna_information;
}



/****************************************************************************
 * Function Name  : rrm_ue_get_antenna_info_v920
 * Inputs         : UE Index
                  : CELL Index
 * Outputs        : Antenna Info
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Getter Method to return the ue's antenna information
                    user are required to check for NULL pointer for received
                    antenna info pointer.
 ****************************************************************************/
const rrm_ue_phy_antenna_info_v920_t*
rrm_ue_get_antenna_info_v920( const rrm_ue_context_t   *p_ue_context )
{
    const rrm_ue_phy_antenna_info_v920_t    *p_ue_antenna_info_v920 = RRM_NULL;

    RRM_UT_TRACE_ENTER();

    if( RRM_NULL != p_ue_context )
    {
        p_ue_antenna_info_v920 = &( p_ue_context->ue_phy_config_information.ue_antenna_info_v920); 
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_ERROR, 
                  "NULL Pointer for UE context ");
    }

    RRM_UT_TRACE_EXIT();

    return p_ue_antenna_info_v920;
}



/****************************************************************************
 * Function Name  : rrm_compare_codebook_subset_restriction
 * Inputs         : codebook_subset
                  : codebook_subset_stored
		  : size
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Compare the Ue's  codebook subset restriction value
 ****************************************************************************/
rrm_bool_et
rrm_compare_codebook_subset_restriction(const U8 *codebook_subset,
					const U8 *codebook_subset_stored,
					U16 size)
{
    rrm_bool_et		ret_val = RRM_TRUE;
    U16			index   = RRM_ZERO;
   
    RRM_UT_TRACE_ENTER();
   
    if(codebook_subset != RRM_PNULL && codebook_subset_stored != RRM_PNULL)
    { 
        for( index = RRM_ZERO ; index < size ; index++ )
        {
            if(codebook_subset[index] != codebook_subset_stored[index])
	    {
	        ret_val = RRM_FALSE;
	        break;
	    }
        }
    } 
    else
    {
        ret_val = RRM_FALSE;
    }	
  	

    RRM_UT_TRACE_EXIT();

    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_compare_ue_antenna_info
 * Inputs         : UE Index
                  : CELL Index
		  : Antenna Info
 * Outputs        : Antenna Info
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Compare the Ue's anetnna info with provided antenna info 
		  : in function
 ****************************************************************************/
rrm_return_et
rrm_compare_ue_antenna_info(rrm_ue_index_t       ue_index,
			    rrm_cell_index_t     cell_index,
			    const rrm_ue_phy_antenna_information_t *p_antenna_info,
			    rrm_bool_et 	  *p_antenna_info_match)
{
    rrm_return_et				ret_val = RRM_SUCCESS;
    const rrm_ue_phy_antenna_information_t     *p_ue_antenna_info_stored = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    p_ue_antenna_info_stored = rrm_ue_get_phy_antenna_info( ue_index,cell_index );

    if (( p_antenna_info != RRM_PNULL) && (p_ue_antenna_info_stored != RRM_PNULL ))
    {
        if( p_ue_antenna_info_stored->transmission_mode == p_antenna_info->transmission_mode )
	{
	    if( RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT ==  \
		(p_ue_antenna_info_stored->bitmask & p_antenna_info->bitmask) )
	    {
	         if( p_ue_antenna_info_stored->codebook_subset_restriction.type == \
		     p_antenna_info->codebook_subset_restriction.type )
		 {
		     if( RRM_TRUE == rrm_compare_codebook_subset_restriction(
					     p_antenna_info->codebook_subset_restriction.value,
					     p_ue_antenna_info_stored->codebook_subset_restriction.value,
   				             sizeof(p_antenna_info->codebook_subset_restriction.value)))
    		      {
		          if( RRC_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT == (p_ue_antenna_info_stored-> \
			     ue_transmit_antenna_selection.bitmask & p_antenna_info->ue_transmit_antenna_selection.bitmask))
			  {
			      if(p_ue_antenna_info_stored->ue_transmit_antenna_selection.ue_transmit_antenna_selection_type \
			 	    != p_antenna_info->ue_transmit_antenna_selection.ue_transmit_antenna_selection_type)
			      {
			          *p_antenna_info_match = RRM_FALSE;
			      }
			  }
		      }
		      else
		      {
		          *p_antenna_info_match = RRM_FALSE;
		      }
		 }
		 else
		 {
		     *p_antenna_info_match = RRM_FALSE;
		 }
	    }
       }
       else
       {
           *p_antenna_info_match = RRM_FALSE;
       }
    }
    else
    {
        *p_antenna_info_match = RRM_FALSE;

	RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_ERROR, "Null pointer Exception for"
     	" matching the UE's Antenna Information for [UE:%d],cell_index[%d]",ue_index,cell_index);
	ret_val = RRM_FAILURE;
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}


/****************************************************************************
 * Function Name  : rrm_compare_ue_antenna_info_v920 
 * Inputs         : UE Index
                  : CELL Index
                  : Antenna Info
 * Outputs        : Antenna Info
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Compare the Ue's anetnna info with provided antenna info
                  : in function
 ****************************************************************************/
rrm_return_et
rrm_compare_ue_antenna_info_v920(const rrm_ue_context_t     *p_ue_context,
                            const rrm_ue_phy_antenna_info_v920_t  *p_antenna_info_v920,
                            rrm_bool_et           *p_antenna_info_match)
{

    rrm_return_et				ret_val = RRM_SUCCESS;

    const rrm_ue_phy_antenna_info_v920_t   *p_ue_phy_antenna_v920_stored = RRM_NULL;

    if( ( RRM_NULL != p_ue_context ) && 
        ( RRM_NULL != p_antenna_info_v920 ) && 
        ( RRM_NULL != p_antenna_info_match ) )
    {
        p_ue_phy_antenna_v920_stored = &(p_ue_context->ue_phy_config_information.ue_antenna_info_v920);

        if( RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT == 
           ( p_ue_phy_antenna_v920_stored->bitmask & p_antenna_info_v920->bitmask ))
        {
            if( p_ue_phy_antenna_v920_stored->codebook_subset_restriction_v920.type ==
                p_antenna_info_v920->codebook_subset_restriction_v920.type )
            {
                if( RRM_FALSE == rrm_compare_codebook_subset_restriction(
                         p_antenna_info_v920->codebook_subset_restriction_v920.value,
                         p_ue_phy_antenna_v920_stored->codebook_subset_restriction_v920.value,
                         sizeof(p_antenna_info_v920->codebook_subset_restriction_v920.value)))
                {
                     /* CID 54925*/
                    (*p_antenna_info_match) = RRM_FALSE;
                }
                
            }
            else
            {
                 /* CID 54925*/
                (*p_antenna_info_match) = RRM_FALSE;
            }
        } 
    }
    else
    {
        if (RRM_NULL != p_antenna_info_match)
        {
            /* CID 54925*/
            (*p_antenna_info_match) = RRM_FALSE;
        }
	ret_val = RRM_FAILURE;
    }

     return ret_val;
}


/****************************************************************************
 * Function Name  : rrm_backup_ue_antenna_info
 * Inputs         : p_ue_context
                  : Antenna Info
 * Outputs        : NONE 
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This funcion back up the UE's last antenna info
 ****************************************************************************/
rrm_return_et
rrm_backup_ue_antenna_info( rrm_ue_context_t   *p_ue_context )
{

    rrm_return_et	ret_val = RRM_SUCCESS;

    RRM_UT_TRACE_ENTER();

    if( RRM_PNULL != p_ue_context )
    {
        /* R-10 Changes : Start */
        /*SPR 22464 Fix Start*/
        if(RRM_REL10 <= p_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
                eutra_radio_capability_info_def.access_stratum_release)
            /*SPR 22464 Fix Stop*/
        {
            RRM_MEMSET(&(p_ue_context->ue_phy_config_information. \
                        ue_backup_antenna_info_r10), RRM_ZERO, 
                    sizeof(rrm_ue_phy_antenna_info_r10_t));

            p_ue_context->ue_phy_config_information.ue_backup_antenna_info_r10 = \
                                                                                 p_ue_context->ue_phy_config_information.ue_antenna_info_r10;
        }
        else
        {
            RRM_MEMSET(&(p_ue_context->ue_phy_config_information. \
                        ue_backup_antenna_information), RRM_ZERO, 
                    sizeof(rrm_ue_phy_antenna_information_t));

            p_ue_context->ue_phy_config_information.ue_backup_antenna_information = \
                                                                                    p_ue_context->ue_phy_config_information.ue_antenna_information;
        }
        /* R-10 Changes : End */
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}



/****************************************************************************
 * Function Name  : rrm_backup_ue_antenna_info_v920
 * Inputs         : p_ue_context
                  : Antenna Info
 * Outputs        : NONE
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This funcion back up the UE's last antenna info
 ****************************************************************************/
rrm_return_et
rrm_backup_ue_antenna_info_v920( rrm_ue_context_t   *p_ue_context )
{

    rrm_return_et	ret_val = RRM_SUCCESS;

    RRM_UT_TRACE_ENTER();

    if( RRM_PNULL != p_ue_context )
    {
        RRM_MEMSET(&(p_ue_context->ue_phy_config_information. \
	ue_backup_antenna_info_v920 ), RRM_ZERO, 
	sizeof(rrm_ue_phy_antenna_info_v920_t ));

        p_ue_context->ue_phy_config_information.ue_backup_antenna_info_v920 = \
	p_ue_context->ue_phy_config_information.ue_antenna_info_v920 ;
    }
    else
    {
        ret_val = RRM_FAILURE;
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_rollback_ue_antenna_info
 * Inputs         : p_ue_context
                  : Antenna Info
 * Outputs        : NONE
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This funcion reset the UE's last antenna info
 ****************************************************************************/
rrm_return_et
rrm_rollback_ue_antenna_info( rrm_ue_context_t   *p_ue_context )
{

    rrm_return_et      ret_val = RRM_SUCCESS;

    RRM_UT_TRACE_ENTER();

    if( RRM_PNULL != p_ue_context )
    {
         p_ue_context->ue_phy_config_information.ue_antenna_information = \
	 p_ue_context->ue_phy_config_information.ue_backup_antenna_information;
	
        RRM_MEMSET( &(p_ue_context->ue_phy_config_information. \
        ue_backup_antenna_information), RRM_ZERO, sizeof(rrm_ue_phy_antenna_information_t));
    }
    else
    {
        ret_val = RRM_FAILURE;
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_rollback_ue_antenna_info_r10
 * Inputs         : p_ue_context
                  : Antenna Info
 * Outputs        : NONE
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function reset the UE's last antenna info r10
 ****************************************************************************/
rrm_return_et
rrm_rollback_ue_antenna_info_r10( rrm_ue_context_t   *p_ue_context )
{
    rrm_return_et      ret_val = RRM_SUCCESS;

    RRM_UT_TRACE_ENTER();

    if( RRM_PNULL != p_ue_context )
    {
         p_ue_context->ue_phy_config_information.ue_antenna_info_r10 = \
         p_ue_context->ue_phy_config_information.ue_backup_antenna_info_r10;

        RRM_MEMSET( &(p_ue_context->ue_phy_config_information. \
        ue_backup_antenna_info_r10), RRM_ZERO, sizeof(rrm_ue_phy_antenna_info_r10_t));
    }
    else
    {
        ret_val = RRM_FAILURE;
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}


/****************************************************************************
 * Function Name  : rrm_rollback_ue_antenna_info_v920
 * Inputs         : p_ue_context
                  : Antenna Info
 * Outputs        : NONE
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This funcion reset the UE's last antenna info
 ****************************************************************************/
rrm_return_et
rrm_rollback_ue_antenna_info_v920( rrm_ue_context_t   *p_ue_context )
{
    rrm_return_et      ret_val = RRM_SUCCESS;

    RRM_UT_TRACE_ENTER();

    if( RRM_PNULL != p_ue_context )
    {
         p_ue_context->ue_phy_config_information.ue_antenna_info_v920 = \
         p_ue_context->ue_phy_config_information.ue_backup_antenna_info_v920;

        RRM_MEMSET( &(p_ue_context->ue_phy_config_information. \
        ue_backup_antenna_information), RRM_ZERO, sizeof(rrm_ue_phy_antenna_information_t));
	
        p_ue_context->ue_phy_config_information.ue_reconfig_rollback_bitmask = ~(RRM_UE_RECONFIG_PHY_ANTENNA_INFO_R10_ROLLBACK_PRESENT) &
                                  		(p_ue_context->ue_phy_config_information.ue_reconfig_rollback_bitmask);
    }
    else
    {
        ret_val = RRM_FAILURE;
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}


/*LTE_RRM_TM_SWITCH_END*/

/*SPR 16060 Fix Start*/

/****************************************************************************
 * Function Name  : rrm_ue_calc_total_prb_to_be_rlsd
 * Inputs         : UE context pointer
 *                : total_dl_prbs
 *                : total_ul_prbs
 * Outputs        : None
 * Returns        : None
 * Description    : Calculates the total prbs that can be released.
 ****************************************************************************/
void
rrm_ue_calc_total_prb_to_be_rlsd
(
 rrm_ue_context_t *p_rrm_ue_ctxt,
 U64          *p_total_deallocated_dl_prbs,
 U64              *p_total_deallocated_ul_prbs
 )
{
    RRM_UT_TRACE_ENTER();
    U32 index=RRM_ZERO;
    U8 qci=RRM_ZERO;
    /* SPR_12473_FIX_start */
    U64 dl_prbs_to_release=RRM_ZERO;
    U64 ul_prbs_to_release=RRM_ZERO;
    /* SPR_12473_FIX_end */
    U64 dl_bitrate=RRM_ZERO;
    U64 ul_bitrate=RRM_ZERO;
    rrm_bool_et is_default_bearer = RRM_FALSE;
    rrm_bool_et ngbr_prb_to_be_rlsd = RRM_TRUE;

    /* SPR_12473_FIX_start */
    rrm_cell_index_t cell_index = p_rrm_ue_ctxt->cell_index;
    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context (cell_index);
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                RRM_ERROR,"Cell Context not found");
        RRM_UT_TRACE_EXIT();
        return;
    }
    U16   rb_factor_for_bitrate = p_cell_context->operator_info.admission_control_info.rb_factor_for_bitrate;
    /* SPR_12473_FIX_end */
    for(index = RRM_ZERO;index<p_rrm_ue_ctxt->drb_configured.num_of_list;
            index++)
    {
        qci = p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_level_qos_params.qci;
        if(qci >=RRM_GBR_QCI_1 && qci <=RRM_GBR_QCI_4)
        {
            dl_bitrate=p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_dl;
            ul_bitrate=p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_level_qos_params.gbr_qos_info.erab_guar_bit_rate_ul;
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "[UE:%d]:erb_id[%d] Deallocating dl_bitrate(%llu) from gbr limit"
                    ,p_rrm_ue_ctxt->ue_index,p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_id,dl_bitrate);
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "[UE:%d]:erb_id[%d] Deallocating ul_bitrate(%llu) from gbr limit"
                    ,p_rrm_ue_ctxt->ue_index,p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_id,ul_bitrate);
            /*SPR 18654 START*/
            dl_bitrate =(U64) RRM_CEIL((float)dl_bitrate / RRM_MEGA);
            ul_bitrate = (U64)RRM_CEIL((float)ul_bitrate / RRM_MEGA);
            /*SPR 18654 END*/
            /* SPR_12473_FIX_start */
            dl_prbs_to_release = (U64)((float)(rb_factor_for_bitrate * dl_bitrate * RRM_HUNDRED) /
                    (float)rrm_cellm_get_num_prb_based_on_snr(cell_index, RRM_FALSE));
            ul_prbs_to_release = (U64)((float)(rb_factor_for_bitrate * ul_bitrate * RRM_HUNDRED) /
                    (float)rrm_cellm_get_num_prb_based_on_snr(cell_index, RRM_TRUE));

            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "[UE:%d]:erb_id[%d] Deallocating dl_gbr_prbs(%llu)"
                    ,p_rrm_ue_ctxt->ue_index,p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_id,dl_prbs_to_release);

            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "[UE:%d]:erb_id[%d] Deallocating ul_gbr_prbs(%llu)"
                    ,p_rrm_ue_ctxt->ue_index,p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_id,ul_prbs_to_release);
            /* SPR_12473_FIX_end */
            *p_total_deallocated_dl_prbs += dl_prbs_to_release;
            *p_total_deallocated_ul_prbs += ul_prbs_to_release;
        }
        else
        {
            if(RRM_TRUE == ngbr_prb_to_be_rlsd)
            {
                if( RRM_ZERO == index)
                {
                    is_default_bearer = RRM_TRUE;
                }
                U8 ngbr_threshold = rrm_cellm_get_ngbr_threshold(p_rrm_ue_ctxt->cell_index);
                U8 non_default_bearer_multiplier =
                    rrm_cellm_get_non_default_bearer_multiplier(p_rrm_ue_ctxt->cell_index);
                dl_bitrate = p_rrm_ue_ctxt->ue_agg_max_bit_rate.ue_agg_max_bit_rate_dl;
                ul_bitrate = p_rrm_ue_ctxt->ue_agg_max_bit_rate.ue_agg_max_bit_rate_ul;
                /*SPR 18654 START*/
                dl_bitrate = RRM_CEIL((float)dl_bitrate / RRM_MEGA);
                ul_bitrate = RRM_CEIL((float)ul_bitrate / RRM_MEGA);
                /*SPR 18654 END*/
                if(RRM_FALSE == is_default_bearer)
                {
                    dl_bitrate = (U64)(dl_bitrate * non_default_bearer_multiplier);
                    ul_bitrate = (U64)(ul_bitrate * non_default_bearer_multiplier);
                }

                dl_bitrate = (U64)(dl_bitrate * ngbr_threshold);
                ul_bitrate = (U64)(ul_bitrate * ngbr_threshold);

                /* SPR_12473_FIX_start */
                dl_prbs_to_release = (U64)((float)(rb_factor_for_bitrate * dl_bitrate * RRM_HUNDRED) /
                        (float)rrm_cellm_get_num_prb_based_on_snr(cell_index, RRM_FALSE));
                ul_prbs_to_release = (U64)((float)(rb_factor_for_bitrate * ul_bitrate * RRM_HUNDRED) /
                        (float)rrm_cellm_get_num_prb_based_on_snr(cell_index, RRM_TRUE));

                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "[UE:%d]:erb_id[%d] Deallocating dl_ngbr_prbs(%llu)"
                        ,p_rrm_ue_ctxt->ue_index,p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_id,dl_prbs_to_release);

                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "[UE:%d]:erb_id[%d] Deallocating ul_ngbr_prbs(%llu)"
                        ,p_rrm_ue_ctxt->ue_index,p_rrm_ue_ctxt->drb_configured.erab_item[index].erab_id,ul_prbs_to_release);
                /* SPR_12473_FIX_end */
                *p_total_deallocated_dl_prbs += dl_prbs_to_release;
                *p_total_deallocated_ul_prbs += ul_prbs_to_release;
                ngbr_prb_to_be_rlsd = RRM_FALSE;
            }
        }
    }
    RRM_UT_TRACE_EXIT();
}

/*SPR 16060 Fix End*/

/****************************************************************************
 * Function Name  : rrm_ue_get_phy_antenna_info_r10
 * Inputs         : UE Index
                  : CELL Index
 * Outputs        : Antenna Info
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Getter Method to return the ue's antenna information
                    user are required to check for NULL pointer for received
                    antenna info pointer.
 ****************************************************************************/
const rrm_ue_phy_antenna_info_r10_t*
rrm_ue_get_phy_antenna_info_r10( rrm_ue_index_t      ue_index,
                                 rrm_cell_index_t    cell_index )
{
    rrm_ue_context_t   *p_ue_context = RRM_PNULL;
    /* Fix for Coverity, LTE_RRM_MALL:CID:11136 [Prasant] */
    /* SPR 20652 Fix Start */
    /* Code Removed */
    const rrm_ue_phy_antenna_info_r10_t *p_ue_antenna_information = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    /* Fix for Coverity, LTE_RRM_MALL:CID:11136 [Prasant] */
    if (RRM_PNULL != (p_ue_context = (rrm_ue_find_context(ue_index,cell_index))))
    {
        /* SPR 20652 Fix End */

        if( p_ue_context != RRM_PNULL )
        {
            p_ue_antenna_information = &(p_ue_context->ue_phy_config_information.ue_antenna_info_r10);
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_ERROR,
                      "could not get the Antenna Information for [UE:%d],cell_index[%d]",
                      ue_index,cell_index);
        }
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_uem_facility_name, RRM_ERROR, 
                "UE context not found");
    }
    /* SPR 20652 Fix Start */
    /* Code Removed */
    /* SPR 20652 Fix End */

    RRM_UT_TRACE_EXIT();

    return p_ue_antenna_information;
}

/****************************************************************************
 * Function Name  : rrm_compare_ue_antenna_info_r10
 * Inputs         : UE Index
                  : CELL Index
                  : Antenna Info
 * Outputs        : Antenna Info
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Compare the Ue's anetnna info with provided antenna info
                  : in function
 ****************************************************************************/
/* R10 Changes : Start */
rrm_return_et
rrm_compare_ue_antenna_info_r10(rrm_ue_index_t       ue_index,
                                rrm_cell_index_t     cell_index,
                                const rrc_antenna_info_dedicated_r10_t *p_antenna_info,
                                rrm_bool_et           *p_antenna_info_match)
{
    rrm_return_et                               ret_val = RRM_SUCCESS;
    const rrm_ue_phy_antenna_info_r10_t     *p_ue_antenna_info_stored = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    p_ue_antenna_info_stored = rrm_ue_get_phy_antenna_info_r10( ue_index,cell_index );

    if (( p_antenna_info != RRM_PNULL) && (p_ue_antenna_info_stored != RRM_PNULL ))
    {
        if( p_ue_antenna_info_stored->transmission_mode == p_antenna_info->transmission_mode )
        {
            if( RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT ==  \
                (p_ue_antenna_info_stored->bitmask & p_antenna_info->bitmask) )
            {
                 if( p_ue_antenna_info_stored->codebook_subset_restriction_r10.num_bits == \
                     p_antenna_info->codebook_subset_restriction_r10.num_bits )
                 {
                     if( RRM_TRUE == rrm_compare_codebook_subset_restriction(
                                             p_antenna_info->codebook_subset_restriction_r10.value,
                                             p_ue_antenna_info_stored->codebook_subset_restriction_r10.value,
                                             sizeof(p_antenna_info->codebook_subset_restriction_r10.value)))
                      {
                          if( RRC_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT == (p_ue_antenna_info_stored-> \
                             ue_transmit_antenna_selection.bitmask & p_antenna_info->ue_transmit_antenna_selection.bitmask))
                          {
                              if(p_ue_antenna_info_stored->ue_transmit_antenna_selection.ue_transmit_antenna_selection_type \
                                    != p_antenna_info->ue_transmit_antenna_selection.ue_transmit_antenna_selection_type)
                              {
                                  *p_antenna_info_match = RRM_FALSE;
                              }
                          }
                      }
                      else
                      {
                          *p_antenna_info_match = RRM_FALSE;
                      }
                 }
                 else
                 {
                     *p_antenna_info_match = RRM_FALSE;
                 }
            }
       }
       else
       {
           *p_antenna_info_match = RRM_FALSE;
       }
    }
    else
    {
        *p_antenna_info_match = RRM_FALSE;

        RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_ERROR, "Null pointer Exception for"
        " matching the UE's Antenna Information for [UE:%d],cell_index[%d]",ue_index,cell_index);
        ret_val = RRM_FAILURE;
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/* CA_Stage_3_Scell_TM_change: Start */
/******************************************************************************
 *   FUNCTION NAME: rrm_get_scell_index_for_scell_cell_index
 *   INPUT      : p_rrm_ue_context, scell_index, index, report_count
 *   OUTPUT     : none
 *   DESCRIPTION: get scell index corresponding to scell's cell index.
 *   RETURNS    :
 *               RRM_SUCCESS on Success
 *               RRM_FAILURE on Failure
*******************************************************************************/
rrm_return_et
rrm_get_scell_index_for_scell_cell_index( rrm_ue_context_t *p_rrm_ue_context,
                                          rrm_cell_index_t  scell_index,
                                          U8               *p_index,
                                          U8               *p_report_count)
{
    U8 count = RRM_ZERO;
    rrm_return_et ret_val = RRM_FAILURE;

    RRM_UT_TRACE_ENTER();
    if( p_rrm_ue_context != RRM_NULL)
    {
        if(p_rrm_ue_context->ue_scell_add_params.count < RRM_MAX_SCELL)
        {
            for(count = RRM_ZERO; count < p_rrm_ue_context->ue_scell_add_params.count; count++)
            {
                if(scell_index == p_rrm_ue_context->ue_scell_add_params.rrm_scell_info_for_ue[count].scell_cell_index)
                {
                    *p_index = p_rrm_ue_context->ue_scell_add_params.rrm_scell_info_for_ue[count].scell_index;
                    *p_report_count = p_rrm_ue_context->ue_scell_add_params.rrm_scell_info_for_ue[count].scell_periodic_stats.scell_report_count;
                    ret_val = RRM_SUCCESS;
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED, \
                            "Scell Cell index found & corresponding scell index is [%d]", 
                            p_rrm_ue_context->ue_scell_add_params.rrm_scell_info_for_ue[count].scell_index);
                }
            }
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR, \
                "UE context Null Pointer Exception.");
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/******************************************************************************
 *   FUNCTION NAME: rrm_backup_tm_scell_info
 *   INPUT      : p_rrm_ue_context, scell_index
 *   OUTPUT     : none
 *   DESCRIPTION: Takes the backup of previous configuration of scell's TX mode.
 *   RETURNS    :
 *               RRM_SUCCESS on Success
 *               RRM_FAILURE on Failure
*******************************************************************************/
rrm_return_et
rrm_backup_tm_scell_info(rrm_ue_context_t *p_rrm_ue_context, 
                         rrm_cell_index_t  scell_index)
{
    rrm_return_et       ret_val = RRM_SUCCESS;
    U8                  scell_arr_idx = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    if((rrm_is_scell_valid_for_ue(p_rrm_ue_context, scell_index, &scell_arr_idx)) != RRM_FAILURE )
    {
                    RRM_MEMSET(&(p_rrm_ue_context->ue_scell_add_params.rrm_scell_info_for_ue[scell_arr_idx].\
                               scell_antenna_info_dedic_r10_backup), RRM_ZERO,
                               sizeof(rrm_scell_antenna_info_dedicated_r10_t));

                    p_rrm_ue_context->ue_scell_add_params.rrm_scell_info_for_ue[scell_arr_idx].scell_antenna_info_dedic_r10_backup = 
                       p_rrm_ue_context->ue_scell_add_params.rrm_scell_info_for_ue[scell_arr_idx].scell_antenna_info_dedicated_r10;
    }
    else /* else of RRM_PNULL != p_ue_context */
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                  "NULL POINTER EXCEPTION for ue context!!");
        ret_val = RRM_FAILURE;
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/******************************************************************************
 *   FUNCTION NAME: rrm_ue_get_phy_antenna_info_r10
 *   INPUT      : p_rrm_ue_context, scell_index
 *   OUTPUT     : none
 *   DESCRIPTION: Fetches & returns the current antenna configuration for the
 *                scell index for the UE. 
 *   RETURNS    :
 *               RRM_SUCCESS on Success
 *               RRM_FAILURE on Failure
*******************************************************************************/
const rrm_scell_antenna_info_dedicated_r10_t* 
rrm_ue_get_scell_phy_antenna_info_r10(rrm_ue_context_t *p_rrm_ue_context,
                                rrm_cell_index_t  scell_index)
{
    U8                  scell_arr_idx = RRM_ZERO;
    const rrm_scell_antenna_info_dedicated_r10_t *p_scell_antenna_info_dedic_r10 = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    if((rrm_is_scell_valid_for_ue(p_rrm_ue_context, scell_index, &scell_arr_idx)) != RRM_FAILURE)
    {
        p_scell_antenna_info_dedic_r10 = 
                &(p_rrm_ue_context->ue_scell_add_params.rrm_scell_info_for_ue[scell_arr_idx].scell_antenna_info_dedicated_r10);
    }
    else /* else of RRM_PNULL != p_ue_context */
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                  "NULL POINTER EXCEPTION for ue context!!");
    }

    RRM_UT_TRACE_EXIT();
    return p_scell_antenna_info_dedic_r10;
}

/******************************************************************************
 *   FUNCTION NAME: rrm_compare_scell_ue_antenna_info_r10
 *   INPUT      : p_rrm_ue_context, p_antenna_info_dedicated_r10, scell_index,
 *                p_antenna_info_match
 *   OUTPUT     : none
 *   DESCRIPTION: Compare the current configuratio to the new configuration for
 *                scell's TX mode.
 *   RETURNS    :
 *               RRM_SUCCESS on Success
 *               RRM_FAILURE on Failure
*******************************************************************************/
rrm_return_et
rrm_compare_scell_ue_antenna_info_r10(
      rrm_ue_context_t                       *p_rrm_ue_context,
      const rrc_antenna_info_dedicated_r10_t *p_antenna_info_dedicated_r10,
      rrm_cell_index_t                        scell_index,
      rrm_bool_et                            *p_antenna_info_match)
{
    rrm_return_et                             ret_val = RRM_SUCCESS;
    const rrm_scell_antenna_info_dedicated_r10_t  *p_scell_antenna_info_stored_r10 = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_scell_antenna_info_stored_r10 = rrm_ue_get_scell_phy_antenna_info_r10( p_rrm_ue_context,
                                                                             scell_index);

    if((p_antenna_info_dedicated_r10 != RRM_PNULL) && (p_scell_antenna_info_stored_r10 != RRM_PNULL))
    {
        if(p_antenna_info_dedicated_r10->transmission_mode == p_scell_antenna_info_stored_r10->transmission_mode)
        {
            if( RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT == 
                (p_antenna_info_dedicated_r10->bitmask & p_scell_antenna_info_stored_r10->bitmask))
            {
                if(p_antenna_info_dedicated_r10->codebook_subset_restriction_r10.num_bits == \
                   p_scell_antenna_info_stored_r10->codebook_subset_restriction_r10.num_bits)
                {
                    if( RRM_TRUE == rrm_compare_codebook_subset_restriction(
                                               p_antenna_info_dedicated_r10->codebook_subset_restriction_r10.value,
                                               p_scell_antenna_info_stored_r10->codebook_subset_restriction_r10.value,
                                               sizeof(p_antenna_info_dedicated_r10->codebook_subset_restriction_r10.value)))
                    {
                        if( RRC_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT == (p_antenna_info_dedicated_r10-> \
                             ue_transmit_antenna_selection.bitmask & p_scell_antenna_info_stored_r10->ue_transmit_antenna_selection.bitmask))
                        {
                            if(p_antenna_info_dedicated_r10->ue_transmit_antenna_selection.ue_transmit_antenna_selection_type \
                                    != p_scell_antenna_info_stored_r10->ue_transmit_antenna_selection.ue_transmit_antenna_selection_type)
                              {
                                  *p_antenna_info_match = RRM_FALSE;
                              }
                        }
                    }
		    else
		    {
			*p_antenna_info_match = RRM_FALSE;
		    }
		}
		else
		{
		    *p_antenna_info_match = RRM_FALSE;
		}
	    }
            else
            {
                *p_antenna_info_match = RRM_FALSE;
            }
	}
	else
	{
	    *p_antenna_info_match = RRM_FALSE;
	}
    }
    else
    {
        *p_antenna_info_match = RRM_FALSE;

        RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,RRM_ERROR, "Null pointer Exception for"
        " matching the UE's Antenna Information for [UE:%d],cell_index[%d]",
        p_rrm_ue_context->ue_index,p_rrm_ue_context->cell_index);
        ret_val = RRM_FAILURE;
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}


/******************************************************************************
 *   FUNCTION NAME: rrm_set_scell_tm_info_r10
 *   INPUT      : p_rrm_ue_context, scell_index, p_antenna_info_dedicated_r10
 *   OUTPUT     : none
 *   DESCRIPTION: Save the new config. in UE context.
 *   RETURNS    :
 *               RRM_SUCCESS on Success
 *               RRM_FAILURE on Failure
*******************************************************************************/
rrm_return_et
rrm_set_scell_tm_info_r10( rrm_ue_context_t            *p_rrm_ue_context,
                           rrm_cell_index_t             scell_index,
                           const rrc_antenna_info_dedicated_r10_t 
                                                       *p_antenna_info_dedicated_r10)
{
    U8 scell_arr_idx = RRM_ZERO;
    rrm_return_et ret_val = RRM_SUCCESS;

    RRM_UT_TRACE_ENTER();
    if((p_antenna_info_dedicated_r10 != RRM_PNULL) && 
           (rrm_is_scell_valid_for_ue(p_rrm_ue_context, scell_index, &scell_arr_idx)) != RRM_FAILURE)
    {
		    p_rrm_ue_context->ue_scell_add_params.rrm_scell_info_for_ue[scell_arr_idx].
                                                      scell_antenna_info_dedicated_r10 = *p_antenna_info_dedicated_r10;
    }
    else /* else of RRM_PNULL != p_ue_context */
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                  "NULL POINTER EXCEPTION for ue context!!");
        ret_val = RRM_FAILURE;
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* CA_Stage_3_Scell_TM_change: End */
