/******************************************************************************
*
*   FILE NAME:
*       uecc_csc.c
*
*   DESCRIPTION:
*       This is the source file of the UECC CSC.
*       UECC CSC interacts with RRC CSC module.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   17 Jul 2009     GrygoriyS   ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"
#include "uecc_global_ctx.h"
#include "rrc_intrl_api.h"
#include "rrc_csc_uecc_intf.h"
#include "uecc_logger.h"
#include "uecc_event_queue.h"
#include "uecc_csc.h"
#include "uecc_s1ap.h"
#include "uecc_defaults.h"
#include "rrc_phy_intf.h"
#include "uecc_fsm_engine.h"
#include "rrc_oamh_intf.h"
#include "rrc_shared_stats.h"
#include "uecc_statistic.h"

/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
static void uecc_csc_prepare_plmn_identity_list
(
    rrc_csc_uecc_init_setup_ind_t       *p_csc_setup
);

static void uecc_csc_print_csc_configuration
(
    rrc_csc_uecc_init_setup_ind_t       *p_csc_setup_ind,
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
);

/****************************************************************************
 * Functions Implementation
 ****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: uecc_csc_process_init_setup_ind
*   INPUT        : void                *p_api
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : None               
*   RETURNS      : None
*   DESCRIPTION  : This function processes CSC CSC_UECC_INIT_SETUP_IND
*
******************************************************************************/
void uecc_csc_process_init_setup_ind
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
)
{
    rrc_csc_uecc_init_setup_ind_t       *p_init_setup_ind = PNULL;


    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_uecc_gb_context->p_p_csc_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    /* Precondition - p_api should point to right message */
    if ( rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE +
                                       sizeof(rrc_csc_uecc_init_setup_ind_t) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "[CSC] Wrong message size - ignore");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }

    p_init_setup_ind =
        (rrc_csc_uecc_init_setup_ind_t *)((U8*)p_api + RRC_API_HEADER_SIZE);
    
    /* Check message data */
    /*REL 1.2:HANDOVER PROCEDURE START*/
    if ( RRC_NULL == p_init_setup_ind->sib_type_1_Info.
                cell_access_related_info.plmn_Id_info_list.count )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "[CSC] PLMN identities list is empty");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }

    if ( !(PLMN_IDENTITY_MCC_PRESENCE_FLAG &
           p_init_setup_ind->sib_type_1_Info.cell_access_related_info.
                 plmn_Id_info_list.plmn_identity_info[0].
                       plmn_identity.presence_bitmask) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING,
                "[CSC] MCC is absent in first PLMN identity");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }
    

    if((PNULL != p_uecc_gb_context->p_p_csc_context)&&(PNULL != 
                p_uecc_gb_context->p_p_csc_context[p_init_setup_ind->cell_index]))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING,
                "[CSC] Cell Already Exists");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }
    else if (PNULL != p_uecc_gb_context->p_p_csc_context)
    {
        p_uecc_gb_context->p_p_csc_context[p_init_setup_ind->cell_index] = 
            (rrc_csc_context_t *)rrc_mem_get(sizeof(rrc_csc_context_t));

        if (p_uecc_gb_context->p_p_csc_context[p_init_setup_ind->cell_index] == PNULL )
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_WARNING,
                    "[CSC] Mem Allocation failed..");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            return;
        }

        memset_wrapper( p_uecc_gb_context->p_p_csc_context[p_init_setup_ind->cell_index],
                RRC_NULL, sizeof(rrc_csc_context_t));

        /* init global ue context statistic engine */
        if(RRC_SUCCESS != uecc_status_cell_engine_init(p_uecc_gb_context->
                    p_p_csc_context[p_init_setup_ind->cell_index],
                    p_uecc_gb_context))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_WARNING, "[CSC] unable to init statistic");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            return;
        }

        /* Store CSC setup data*/
        p_uecc_gb_context->p_p_csc_context[p_init_setup_ind->cell_index]->
            p_csc_init_setup_ind = (rrc_csc_uecc_init_setup_ind_t *)
            rrc_mem_get(sizeof(rrc_csc_uecc_init_setup_ind_t));

        if ( (p_uecc_gb_context->p_p_csc_context[p_init_setup_ind->cell_index]->
                    p_csc_init_setup_ind) == PNULL )
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_WARNING,
                    "[CSC] Mem Allocation failed..");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            return;
        }

        memset_wrapper(p_uecc_gb_context->p_p_csc_context[p_init_setup_ind->cell_index]->
                p_csc_init_setup_ind, RRC_NULL,
                sizeof(rrc_csc_uecc_init_setup_ind_t));

        l3_memcpy_wrapper(p_uecc_gb_context->p_p_csc_context[p_init_setup_ind->cell_index]->
                    p_csc_init_setup_ind, p_init_setup_ind, sizeof(rrc_csc_uecc_init_setup_ind_t));
         /*SPR_19262_START*/
        /* Code Removed */
        /*SPR_19262_END*/
        if (p_init_setup_ind->num_of_antennas != 1) 
        {
            physical_config_dedicated_default.antenna_information.
                antenna_information_explicit_value.transmission_mode = 
                TRANSMISSION_MODE_TM2;
        }

        uecc_csc_prepare_plmn_identity_list(p_uecc_gb_context->p_p_csc_context[
                p_init_setup_ind->cell_index]->p_csc_init_setup_ind);

        uecc_csc_print_csc_configuration(p_uecc_gb_context->p_p_csc_context[
                p_init_setup_ind->cell_index]->p_csc_init_setup_ind,
                p_uecc_gb_context);
        /*REL 1.2:HANDOVER PROCEDURE END*/

    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
*   FUNCTION NAME : uecc_csc_process_update_performance_counter_contd
*
*   DESCRIPTION   : This function updates the performance counters
*
*   RETURNS       : None
*
******************************************************************************/
static void uecc_csc_process_update_performance_counter_contd(
          uecc_ue_context_t*      p_uecc_context,
          U8       qci_value
)
{
    /* SPR 13441 Fix Start */
    if ( (RRC_MIN_QCI_GBR_LIMIT <= qci_value) &&
            (RRC_MAX_QCI_NON_GBR_LIMIT >= qci_value) )
    {
    RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_context->p_gb_context,
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_uecc_context->cell_index].rrc_erab_related_stats.
            erab_release_stats.erab_relAttNbr_sum,
            RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);
    /* SPR 16137 START */
    /* Code Removed */
    /* SPR 16137 END */

    /* SPR 11462 Fix Start*/
    if (p_uecc_context->m.srb1_created)
    {
        RRC_UPDATE_PERFORMANCE_COUNTER_WITH_DECREMENT(p_uecc_context->p_gb_context,
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_uecc_context->cell_index].rrc_erab_related_stats.
            erab_sim_num_stats.erabSimNumber_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
            RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

        RRC_UPDATE_PERFORMANCE_COUNTER_WITH_DECREMENT(p_uecc_context->p_gb_context,
            g_rrc_stats.p_rrc_stats->
            rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_uecc_context->cell_index].rrc_erab_related_stats.
            erab_sim_num_stats.erabSimNumber_sum,
            RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);
    }
    }
    /* SPR 13441 Fix End */
}

/******************************************************************************
*   FUNCTION NAME : uecc_csc_process_update_performance_counter 
*
*   DESCRIPTION   : This function updates the performance counters
*
*   RETURNS       : None
*
******************************************************************************/
static void uecc_csc_process_update_performance_counter(
          uecc_ue_context_t*      p_uecc_context,
          s1ap_Cause cause
)
{
    U8       qci_value = 0;
    U8       counter = 0;

    for (counter = 0; counter < MAX_ERAB_COUNT; counter++)
    {
        if (PNULL != p_uecc_context->p_e_rab_list[counter])
        {
            qci_value = p_uecc_context->p_e_rab_list[counter]->
                e_RABlevelQoSParameters.qCI;
            /* SPR 13441 Fix Start */
            if ( (RRC_MIN_QCI_GBR_LIMIT <= qci_value) &&
                    (RRC_MAX_QCI_NON_GBR_LIMIT >= qci_value) )
            {
                RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_uecc_context->cell_index].
                    rrc_erab_related_stats.erab_release_stats.
                    erab_relEnbNbr_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
                    RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

            RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_uecc_context->cell_index].
                    rrc_erab_related_stats.erab_release_stats.
                    erab_relEnbNbr_sum, RRC_UPDATED_QCI_INDEX(qci_value),
                    RRC_MAX_QCI_INDEX);

            RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_uecc_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_uecc_context->cell_index].
                    rrc_erab_related_stats.erab_release_stats.
                    erab_rel_enb_cause_stats.erab_relEnbNbr_cause,cause);

            RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_uecc_context->cell_index].
                    rrc_erab_related_stats.erab_release_stats.
                    erab_relAttNbr_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
                    RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);
            }
            /* SPR 13441 Fix End */

            uecc_csc_process_update_performance_counter_contd(
                                              p_uecc_context, qci_value);
        } 
    }
}

/******************************************************************************
*   FUNCTION NAME: uecc_csc_process_clean_cell_context
*
*   DESCRIPTION:
*       This function cleans the cell context
*
*   RETURNS:
*       None
*
******************************************************************************/
static void uecc_csc_process_clean_cell_context(
          uecc_gb_context_t                      *p_uecc_gb_context,
          uecc_ue_context_t                      *p_uecc_context,
          rrc_csc_uecc_delete_all_ue_req_t       *p_del_all_ue_req
)
{
    U16                     i = 0, count = 0;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    /* Check whether any UE is connected */
    for (i=0; i < p_uecc_gb_context->total_ue_supported; i++)
    {
        p_uecc_context = p_uecc_gb_context->ue_db.ue_contexts[i];
        if (PNULL == p_uecc_context)
        {
            count++;
        }
        else if ( (PNULL != p_uecc_context) && (p_uecc_context->
                    cell_index != p_del_all_ue_req->cell_index) )
        {
            count++;
        }
        else
        {
            break;
        }
    }
    if(count == (p_uecc_gb_context->total_ue_supported))
    {
        uecc_csc_build_and_send_delete_all_ue_entity_resp(RRC_SUCCESS,
                p_uecc_gb_context,p_del_all_ue_req->cell_index);

        if (RRC_TRUE == p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->cell_index]->
                is_cell_stop_ongoing)
        {
            p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->cell_index]->
                is_cell_stop_ongoing = RRC_FALSE;
            p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->cell_index]->
                del_cell_ongoing = 0;
        }
        else
        {
            /* Clean Cell Context */
            if (p_uecc_gb_context->
                    p_p_csc_context[p_del_all_ue_req->cell_index] != PNULL)
            {
                if (p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->
                        cell_index]->p_csc_init_setup_ind != PNULL)
                {
                    rrc_mem_free (p_uecc_gb_context->
                            p_p_csc_context[p_del_all_ue_req->cell_index]->
                            p_csc_init_setup_ind);

                    p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->
                        cell_index]->p_csc_init_setup_ind = PNULL;
                }
                if (p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->cell_index]->
                        p_rrc_uecc_statistic != PNULL)
                {
                    rrc_mem_free (p_uecc_gb_context->
                            p_p_csc_context[p_del_all_ue_req->cell_index]->
                            p_rrc_uecc_statistic);
                    p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->
                        cell_index]->
                        p_rrc_uecc_statistic = PNULL;
                }
                if (p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->cell_index]->
                        p_cell_traffic_trace_info != PNULL)
                {
                    rrc_mem_free (p_uecc_gb_context->
                            p_p_csc_context[p_del_all_ue_req->cell_index]->
                            p_cell_traffic_trace_info);
                    p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->
                        cell_index]->p_cell_traffic_trace_info = PNULL;
                }

                p_uecc_gb_context-> p_p_csc_context[p_del_all_ue_req->cell_index]->
                    del_cell_ongoing = 0;

                rrc_mem_free(p_uecc_gb_context->
                        p_p_csc_context[p_del_all_ue_req->cell_index]);

                p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->cell_index] = PNULL;

            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_csc_process_reset_all_UEs
*
*   DESCRIPTION:
*       This function resets all UEs
*
*   RETURNS:
*       None
*
******************************************************************************/
static rrc_return_et uecc_csc_process_reset_all_UEs(
          uecc_gb_context_t   *p_uecc_gb_context,
          rrc_csc_uecc_delete_all_ue_req_t       *p_del_all_ue_req,
          mme_ue_s1ap_id_list_t   *p_mme_ue_s1ap_id_list,
          s1ap_Cause cause
)
{
    uecc_ue_context_t*      p_uecc_context = PNULL;
    rrc_return_et           result = RRC_SUCCESS;
    U16                     i = 0;

    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_uecc_gb_context->p_p_csc_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    /* reset all UE's */
    p_uecc_gb_context-> p_p_csc_context[p_del_all_ue_req->cell_index]->
        del_cell_ongoing = 1;
    p_uecc_gb_context-> p_p_csc_context[p_del_all_ue_req->cell_index]->
        is_cell_stop_ongoing = p_del_all_ue_req->is_cell_stopped;
    p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->cell_index]->
        p_csc_init_setup_ind->cell_state = RRC_CSC_CELL_STOPPED;

    /*SPR_17484_START*/
    /* Code Removed */
    /*SPR_17484_END*/
    for(i=0; i < p_uecc_gb_context->total_ue_supported; i++)
    {
        p_uecc_context = p_uecc_gb_context->ue_db.ue_contexts[i];

        if(PNULL != p_uecc_context)
        {
            /* SPR 21412 Fix Start */
            if(p_uecc_context->cell_index == p_del_all_ue_req->cell_index)
            {
            /* SPR 21412 Fix End */
                if ((PNULL != p_uecc_context->p_gb_context) && (PNULL != g_rrc_stats.p_rrc_stats))
                {
                    /* BUG_11901 start */
                    /* SPR 21070 Fix Start */
                    if((!p_uecc_gb_context->reset_ongoing) || 
                            (( PNULL != p_uecc_context->uecc_fsm.p_current_state->s_name) &&
                             strcmp_wrapper(p_uecc_context->uecc_fsm.p_current_state->s_name,(const S8 *)"UECC_UE_CONNECTION_RELEASE_ONGOING")))
                        /* SPR 21070 Fix End */
                    {                    
                        uecc_csc_process_update_performance_counter(
                                p_uecc_context, cause);
                    }    
                    /* BUG_11901 stop */
                }    

                p_uecc_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag
                    = RRC_TRUE;
                /* SPR 15794 Fix Start*/
                p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->cell_index]->
                    count_use_ue_del_cell_req ++;
		/* SPR 21458 Start */
                p_uecc_gb_context->l2_instance_reset_flag = p_del_all_ue_req->l2_instance_reset_flag;
		/* SPR 21458 End */
                /* SPR 15794 Fix Stop*/

                /* Send event to specified UE */
                if(PNULL != p_uecc_context->p_gb_context)
                {

                    uecc_fsm_process_event(
                            p_uecc_context,
                            UECC_EV_CSC_DELETE_ALL_UE_REQ,
                            PNULL);
                }

                if ( PNULL != p_uecc_context->ho_info.p_ho_info )
                {
                    if ( ( S1_HO == p_uecc_context->ho_info.s1_or_x2_handover ) &&
                            ( PNULL != p_uecc_context->ho_info.p_ho_info ) )
                    {
                        if ( PNULL != p_uecc_context->ho_info.p_ho_info->
                                p_trg_ho_info )
                        {
                            if ( RRC_FALSE == p_uecc_context->ho_info.p_ho_info->
                                    p_trg_ho_info->is_ho_req_ack_sent )
                            {
                                /* store MME S1AP ID in S1AP_RESET_MSG_IND message
                                   to delete the temprory list maintained at S1AP for 
                                   Handover */
                                p_mme_ue_s1ap_id_list->mme_ue_s1ap_id
                                    [p_mme_ue_s1ap_id_list->count++] =
                                    p_uecc_context->mme_ue_id;

                                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                        p_uecc_gb_context->facility_name,RRC_BRIEF,
                                        "[S1AP_FAILURE_MSG_REQ][UE_INDEX:%u] "
                                        "Added for resetting mme_ue_id[0x%08Xh]",
                                        p_uecc_context->ue_index,
                                        p_uecc_context->mme_ue_id);

                                /* At a time reset can be sent for 256 MME
                                   S1AP Ids */
                                if (MAX_RESET_UE_S1AP_ID_COUNT ==
                                        (p_mme_ue_s1ap_id_list->count - 1))
                                {
                                    result = uecc_s1ap_build_and_send_ho_ongoing_ue_release_ind 
                                        (p_mme_ue_s1ap_id_list,p_uecc_gb_context);
                                    if (RRC_FAILURE == result)
                                    {
                                        RRC_UECC_TRACE(p_uecc_gb_context->
                                                context_index,
                                                p_uecc_gb_context->facility_name,
                                                RRC_WARNING, "Unable to send"
                                                " HO Ongoing Ue Release Ind to S1AP");
                                        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                                        return result;
                                    }
                                    memset_wrapper(p_mme_ue_s1ap_id_list, RRC_NULL, 
                                            sizeof(mme_ue_s1ap_id_list_t));
                                }
                            }
                        }
                    }
                }
                /* SPR 15794 Fix Delete Start*/
                /* SPR 15794 Fix Delete Stop*/


            /* SPR 21412 Fix Start */
            }
            /* SPR 21412 Fix End */
        }
    }

    /* BUG 12722 Fix Start */
    /*SPR_17484_START*/
    uecc_csc_process_clean_cell_context(
            p_uecc_gb_context,p_uecc_context,p_del_all_ue_req);
    /*SPR_17484_END*/
    /* BUG_11901 start */
    /* BUG_11901 stop */
    /* BUG 12722 Fix Stop */

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_csc_process_del_all_ue_req
*   INPUT        : void                *p_api
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : None               
*   RETURNS      : None
*   DESCRIPTION  : This function processes CSC CSC_UECC_del_all_ue_req
******************************************************************************/
void uecc_csc_process_del_all_ue_req
(
 void                *p_api,             /* Pointer to input API buffer */
 uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
 )
{
    rrc_csc_uecc_delete_all_ue_req_t       *p_del_all_ue_req = PNULL;
    s1ap_Cause cause;
    mme_ue_s1ap_id_list_t   mme_ue_s1ap_id_list;
    rrc_return_et           result = RRC_FAILURE;


    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_uecc_gb_context->p_p_csc_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    memset_wrapper(&cause, RRC_NULL, sizeof(cause));
    memset_wrapper(&mme_ue_s1ap_id_list, RRC_NULL, sizeof(mme_ue_s1ap_id_list_t));
    cause.t = T_s1ap_Cause_misc;
    cause.u.misc = s1ap_om_intervention;

    RRC_UECC_TRACE(p_uecc_gb_context->context_index,p_uecc_gb_context->
                    facility_name,RRC_INFO, "Received CSC_UECC_DELETE_ALL_UE_REQ");

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE +
            sizeof(rrc_csc_uecc_delete_all_ue_req_t) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,p_uecc_gb_context->
                        facility_name,RRC_WARNING, 
                        "[CSC] Wrong message size - ignore");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }

    p_del_all_ue_req = (rrc_csc_uecc_delete_all_ue_req_t *)((U8*)p_api + CV_HDRSIZE);

    if((PNULL != p_uecc_gb_context->p_p_csc_context) && (PNULL == 
               p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->cell_index]))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,p_uecc_gb_context->
                        facility_name,RRC_WARNING, "Cell does not exist.");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }
    if ((PNULL != p_uecc_gb_context->p_p_csc_context) 
        && (p_del_all_ue_req->cell_index == 
            p_uecc_gb_context->p_p_csc_context[p_del_all_ue_req->cell_index]->
              p_csc_init_setup_ind->cell_index)
        )
    {
        /* reset all UE's */
        result = uecc_csc_process_reset_all_UEs(p_uecc_gb_context,
                                 p_del_all_ue_req, &mme_ue_s1ap_id_list, cause);
        if(RRC_SUCCESS != result)
        {
            return;
        }
    }

    if ( 0 <  mme_ue_s1ap_id_list.count )
    {
        result = uecc_s1ap_build_and_send_ho_ongoing_ue_release_ind 
            (&mme_ue_s1ap_id_list,p_uecc_gb_context);
        if (RRC_FAILURE == result)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_WARNING, "Unable to send"
                    " HO Ongoing Ue Release Ind to S1AP");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            return;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);

}
/******************************************************************************
*   FUNCTION NAME: uecc_csc_build_and_send_delete_all_ue_entity_resp
*   INPUT        : rrc_return_et     response
*                  uecc_gb_context_t  *p_uecc_gb_context
*                  rrc_cell_index_t   cell_index
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function build and send ALL UE DELETE RESP to CSC with event UECC_CSC_DELETE_ALL_UE_RESP
******************************************************************************/
rrc_return_et uecc_csc_build_and_send_delete_all_ue_entity_resp
(
    rrc_return_et     response ,
   uecc_gb_context_t  *p_uecc_gb_context,
   rrc_cell_index_t   cell_index
)
{
    rrc_csc_uecc_delete_all_ue_resp_t        uecc_csc_delete_ue_resp;


    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_uecc_gb_context->p_p_csc_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    if (PNULL != p_uecc_gb_context->p_p_csc_context[cell_index])
    {
        if (PNULL != p_uecc_gb_context->p_p_csc_context[cell_index]->p_csc_init_setup_ind)
        {
            /* Fill message */
            uecc_csc_delete_ue_resp.cell_index = p_uecc_gb_context->p_p_csc_context[
                cell_index]->p_csc_init_setup_ind->cell_index;
        }

    }
    /*SPR_19262_START*/
    /* Code Removed */
    /*SPR_19262_END*/
    uecc_csc_delete_ue_resp.result = RRC_SUCCESS;

    response = (rrc_return_et)uecc_rrc_send_internal_msg(RRC_UECCMD_MODULE_ID,
            UECC_CSC_DELETE_ALL_UE_RESP,
            sizeof(rrc_csc_uecc_delete_all_ue_resp_t),
            &uecc_csc_delete_ue_resp);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);

    return response;
}
/******************************************************************************
*   FUNCTION NAME: uecc_csc_prepare_plmn_identity_list
*   INPUT        : rrc_csc_uecc_init_setup_ind_t       *p_csc_setup
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : Prepares PLMN identity list for using.
*                  Resolve any absent MCC IEs.
******************************************************************************/
void uecc_csc_prepare_plmn_identity_list
(
    rrc_csc_uecc_init_setup_ind_t       *p_csc_setup
)
{
    U8  index = 0;
    U8  work_mcc[MCC_OCTET_SIZE];
    plmn_identity_info_t *p_plmn = PNULL;

    /* Initialize the Array */
    memset_wrapper(work_mcc, 0, MCC_OCTET_SIZE);

    /* Resolve any absent MCC IEs.*/
    /* For UECC working the MCC should be presented in each item of
     * PLMN identity list */
    /* According to TS 36.331:
     *   If the MCC field is absent, it takes the same value as the MCC of the
     *   immediately preceding IE PLMN-Identity.
     *   */
/*REL 1.2:HANDOVER PROCEDURE START*/
    for (index = 0; index < p_csc_setup->sib_type_1_Info.
             cell_access_related_info.plmn_Id_info_list.count; index++)
    {
        p_plmn = &p_csc_setup->sib_type_1_Info.cell_access_related_info.
                 plmn_Id_info_list.plmn_identity_info[index];

        if ( PLMN_IDENTITY_MCC_PRESENCE_FLAG &
                p_plmn->plmn_identity.presence_bitmask )
        {
            /* Store MCC */
            l3_memcpy_wrapper(work_mcc, p_plmn->plmn_identity.mcc, sizeof(work_mcc));
        }
/*REL 1.2:HANDOVER PROCEDURE END*/
        else
        {
            /* Use MCC preceding PLMN-Identity*/
            l3_memcpy_wrapper(p_plmn->plmn_identity.mcc, work_mcc, sizeof(work_mcc));
            p_plmn->plmn_identity.presence_bitmask |=
                PLMN_IDENTITY_MCC_PRESENCE_FLAG;
        }
    }
}

/******************************************************************************
*   FUNCTION NAME: uecc_csc_print_csc_configuration
*   INPUT        : rrc_csc_uecc_init_setup_ind_t       *p_csc_setup
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : Prints current CSC configuration
******************************************************************************/
void uecc_csc_print_csc_configuration
(
    rrc_csc_uecc_init_setup_ind_t       *p_csc_setup_ind,
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
)
{
	/* coverity_fix_54731_start */
	U8  index = 0;
	plmn_identity_info_t *p_plmn = PNULL;

	RRC_UECC_TRACE(p_uecc_gb_context->context_index,
			p_uecc_gb_context->facility_name,
			RRC_INFO, "[CSC] Configuration:");

	RRC_UECC_TRACE(p_uecc_gb_context->context_index,
			p_uecc_gb_context->facility_name,
			RRC_INFO, "[CSC] Cell index [%u]",
			p_csc_setup_ind->cell_index);

	/*REL 1.2:HANDOVER PROCEDURE START*/

	RRC_UECC_TRACE(p_uecc_gb_context->context_index,
			p_uecc_gb_context->facility_name,
			RRC_INFO,
			"[CSC] Cell identity [0x%02X 0x%02X 0x%02X 0x%02X]",
			p_csc_setup_ind->sib_type_1_Info.cell_access_related_info.cell_Id[0],
			p_csc_setup_ind->sib_type_1_Info.cell_access_related_info.cell_Id[1],
			p_csc_setup_ind->sib_type_1_Info.cell_access_related_info.cell_Id[2],
			p_csc_setup_ind->sib_type_1_Info.cell_access_related_info.cell_Id[3]);
	/* SPR:6801 start */
	RRC_UECC_TRACE(p_uecc_gb_context->context_index,
			p_uecc_gb_context->facility_name,
			RRC_INFO, "[CSC] TAC [0x%02X 0x%02X ]",
			p_csc_setup_ind->sib_type_1_Info.cell_access_related_info.tac[0], 
			p_csc_setup_ind->sib_type_1_Info.cell_access_related_info.tac[1]);
	/* SPR:6801 stop */

	if ( CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG & p_csc_setup_ind->
			sib_type_1_Info.cell_access_related_info.presence_bitmask )
	{
		RRC_UECC_TRACE(p_uecc_gb_context->context_index,
				p_uecc_gb_context->facility_name,
				RRC_INFO,
				"[CSC] CSG identity [0x%02X 0x%02X 0x%02X 0x%02X]",
				p_csc_setup_ind->sib_type_1_Info.cell_access_related_info.
				csg_identity[0], 
				p_csc_setup_ind->sib_type_1_Info.cell_access_related_info.
				csg_identity[1], 
				p_csc_setup_ind->sib_type_1_Info.cell_access_related_info.
				csg_identity[2], 
				p_csc_setup_ind->sib_type_1_Info.cell_access_related_info.
				csg_identity[3]);
	}

	for (index = 0; index < p_csc_setup_ind->sib_type_1_Info.
			cell_access_related_info.plmn_Id_info_list.count; index++)
	{
		p_plmn = &p_csc_setup_ind->sib_type_1_Info.cell_access_related_info.
			plmn_Id_info_list.plmn_identity_info[index];

		/*REL 1.2:HANDOVER PROCEDURE END*/

		RRC_UECC_TRACE(p_uecc_gb_context->context_index,
				p_uecc_gb_context->facility_name,
				RRC_INFO,
				"[CSC][PLMN%u] cellReservedForOperatorUse [%u]",
				index,
				p_plmn->cell_res_for_operator_use);

		if ( PLMN_IDENTITY_MCC_PRESENCE_FLAG &
				p_plmn->plmn_identity.presence_bitmask )
		{
			RRC_UECC_TRACE(p_uecc_gb_context->context_index,
					p_uecc_gb_context->facility_name,
					RRC_INFO, "[CSC][PLMN%u] MCC [0x%02X 0x%02X 0x%02X]",
					index,
					p_plmn->plmn_identity.mcc[0],
					p_plmn->plmn_identity.mcc[1],
					p_plmn->plmn_identity.mcc[2]);
		}

		RRC_UECC_TRACE(p_uecc_gb_context->context_index,
				p_uecc_gb_context->facility_name,
				RRC_INFO,
				"[CSC][PLMN%u] MNC [0x%02X 0x%02X 0x%02X]",
				index,
				p_plmn->plmn_identity.mnc.mnc[0],
				p_plmn->plmn_identity.mnc.mnc[1],
				p_plmn->plmn_identity.mnc.count == 3?
				p_plmn->plmn_identity.mnc.mnc[2]:0x0F);
	}
}
/* coverity_fix_54731_stop */

/******************************************************************************
*   FUNCTION NAME: uecc_csc_process_cell_reconfig_ind
*   INPUT        : void                *p_api
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function processes CSC CSC_UECC_CELL_RECONFIG_IND
******************************************************************************/
void uecc_csc_process_cell_reconfig_ind
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
)
{
    rrc_csc_uecc_cell_reconfig_ind_t       *p_cell_reconfig_ind = PNULL;
    

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_uecc_gb_context->p_p_csc_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    p_cell_reconfig_ind = 
        (rrc_csc_uecc_cell_reconfig_ind_t *)((U8*)p_api + CV_HDRSIZE);
    
    if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(p_uecc_gb_context,
                p_cell_reconfig_ind->cell_index))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "cell not active");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }
    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE +
            sizeof(rrc_csc_uecc_cell_reconfig_ind_t) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "[CSC] Wrong message size - ignore");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }


    if (p_cell_reconfig_ind->num_of_antennas != 1) 
    {
        physical_config_dedicated_default.antenna_information.
            antenna_information_explicit_value.transmission_mode = 
            TRANSMISSION_MODE_TM2;
    }
    else
    {
        physical_config_dedicated_default.antenna_information.
            antenna_information_explicit_value.transmission_mode = 
            TRANSMISSION_MODE_TM1;
    }
    if (PNULL != p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index])
    {
        if (PNULL != p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->
                cell_index]->p_csc_init_setup_ind)
        {

            /* Store CSC Reconfig Data */
            if (p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                    p_csc_init_setup_ind->cell_index == p_cell_reconfig_ind->cell_index)
            {
                /* Fill message */
                p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                    p_csc_init_setup_ind->num_of_antennas = p_cell_reconfig_ind->num_of_antennas;

                p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                    p_csc_init_setup_ind->mib_info = p_cell_reconfig_ind->mib_info;


                p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                    p_csc_init_setup_ind->sib_type_1_Info = p_cell_reconfig_ind->sib_type_1_Info;


                p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                    p_csc_init_setup_ind->sib_type_2_Info = p_cell_reconfig_ind->sib_type_2_Info;


                p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                    p_csc_init_setup_ind->eutra_dl_carrier_freq = p_cell_reconfig_ind->eutra_dl_carrier_freq;


                p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                    p_csc_init_setup_ind->phys_cell_id = p_cell_reconfig_ind->phys_cell_id;
               
                if (p_cell_reconfig_ind->bitmask & RRC_CSC_UECC_CDMA2000_RAND_PRESENT)
                {
                    p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                        p_csc_init_setup_ind->bitmask |= RRC_CSC_UECC_CDMA2000_RAND_PRESENT;

                    p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                      p_csc_init_setup_ind->cdma2000_rand = p_cell_reconfig_ind->cdma2000_rand;
                }    

                if (p_cell_reconfig_ind->bitmask & RRC_CSC_UECC_CDMA2000_1XRTT_CELL_ID_PRESENT)
                {
                      p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                        p_csc_init_setup_ind->bitmask |= RRC_CSC_UECC_CDMA2000_1XRTT_CELL_ID_PRESENT;

                     p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                       p_csc_init_setup_ind->cell_id_1xrtt = p_cell_reconfig_ind->cell_id_1xrtt;
                }
                if (p_cell_reconfig_ind->bitmask & RRC_CSC_UECC_CDMA2000_HRPD_SECTOR_ID_PRESENT)
                {
                    p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                        p_csc_init_setup_ind->bitmask |= RRC_CSC_UECC_CDMA2000_HRPD_SECTOR_ID_PRESENT;
                     p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                       p_csc_init_setup_ind->sector_id_hrpd = p_cell_reconfig_ind->sector_id_hrpd;
                }
                /* CSR 00070260 Changes Start */
                if (p_cell_reconfig_ind->bitmask & RRC_CSC_UECC_CDMA2000_1XRTT_REF_CELL_ID_PRESENCE_FLAG)
                {
                    p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                        p_csc_init_setup_ind->bitmask |= RRC_CSC_UECC_CDMA2000_1XRTT_REF_CELL_ID_PRESENCE_FLAG;
                     p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                       p_csc_init_setup_ind->ref_cell_id_1xrtt = p_cell_reconfig_ind->ref_cell_id_1xrtt;
                }
                if (p_cell_reconfig_ind->bitmask & RRC_CSC_UECC_CDMA2000_HRPD_REF_CELL_ID_PRESENCE_FLAG)
                {
                    p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                        p_csc_init_setup_ind->bitmask |= RRC_CSC_UECC_CDMA2000_HRPD_REF_CELL_ID_PRESENCE_FLAG;
                     p_uecc_gb_context->p_p_csc_context[p_cell_reconfig_ind->cell_index]->
                       p_csc_init_setup_ind->ref_sector_id_hrpd = p_cell_reconfig_ind->ref_sector_id_hrpd;
                }
                /* CSR 00070260 Changes Stop */
           }
        }
   }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/******************************************************************************
*   FUNCTION NAME: uecc_csc_process_cell_start_ind
*   INPUT        : void                *p_api
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function processes CSC CSC_UECC_CELL_START_IND
******************************************************************************/
void uecc_csc_process_cell_start_ind
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
)
{
    rrc_csc_uecc_cell_start_ind_t       *p_cell_start_ind = PNULL;
    

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_uecc_gb_context->p_p_csc_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE +
                                sizeof(rrc_csc_uecc_cell_start_ind_t) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "[CSC] Wrong message size - ignore");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }

    p_cell_start_ind = 
        (rrc_csc_uecc_cell_start_ind_t *)((U8*)p_api + CV_HDRSIZE);

    if (PNULL != p_uecc_gb_context->p_p_csc_context[p_cell_start_ind->cell_index])
    {
        if (PNULL != p_uecc_gb_context->p_p_csc_context
                [p_cell_start_ind->cell_index]->p_csc_init_setup_ind)
        {
            p_uecc_gb_context->p_p_csc_context[p_cell_start_ind->cell_index]->
                p_csc_init_setup_ind->cell_state = RRC_CSC_CELL_ACTIVE ; 
        }
        /*Bug 9328 Fix start*/
            /* Marking Cell as configured in shared memory */
        if (PNULL != g_rrc_stats.p_rrc_stats)
        {
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_uecc_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_cell_start_ind->cell_index].is_cell_configured = RRC_TRUE;
        }
        /*Bug 9328 Fix stop*/
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/******************************************************************************
*   FUNCTION NAME: uecc_csc_process_cell_stop_ind
*   INPUT        : void                *p_api
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function processes CSC CSC_UECC_CELL_STOP_IND
******************************************************************************/
void uecc_csc_process_cell_stop_ind
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
)
{
    rrc_csc_uecc_cell_stop_ind_t   *p_cell_stop_ind = PNULL;
    

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_uecc_gb_context->p_p_csc_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE +
                                sizeof(rrc_csc_uecc_cell_stop_ind_t) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING, "[CSC] Wrong message size - ignore");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }

    p_cell_stop_ind = 
        (rrc_csc_uecc_cell_stop_ind_t *)((U8*)p_api + CV_HDRSIZE);

    if (PNULL != p_uecc_gb_context->p_p_csc_context[p_cell_stop_ind->cell_index])
    {
        if (PNULL != p_uecc_gb_context->p_p_csc_context
                [p_cell_stop_ind->cell_index]->p_csc_init_setup_ind)
        {
            p_uecc_gb_context->p_p_csc_context[p_cell_stop_ind->cell_index]->
                p_csc_init_setup_ind->cell_state = RRC_CSC_CELL_STOPPED;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

