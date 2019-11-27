/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_oamh.c,v 1.22 2010/11/16 13:15:36 gur21239 Exp $
 ****************************************************************************
 *
 *   DESCRIPTION:
 *       This is the implementation of the UECC OAMH interface.
 *       UECC OAMH interacts with RRC OAMH module.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_oamh.c,v $
 * Revision 1.22  2010/11/16 13:15:36  gur21239
 * spr 644 related changes done
 *
 * Revision 1.21  2010/10/13 07:03:49  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.6.4.8  2010/10/05 12:39:41  gur22047
 * compilation warning removed
 *
 * Revision 1.6.4.7  2010/10/04 13:53:58  gur04518
 * Removed TABS
 *
 * Revision 1.6.4.6  2010/09/13 13:04:09  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.6.4.5  2010/08/12 13:45:50  gur18569
 * incoporated review comments for HO
 *
 * Revision 1.6.4.4  2010/08/09 13:54:57  gur22047
 * file modified during ut
 *
 * Revision 1.6.4.3  2010/08/05 08:08:56  gur15697
 * merging round 2
 *
 * Revision 1.6.4.2  2010/07/21 08:40:18  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.6.4.1  2010/06/24 12:53:45  gur22047
 * internal source and s1ap code
 *
 * Revision 1.6  2010/04/06 04:02:31  gur18569
 * updating  p_uecc_gb_context->p_p_csc_context.cell_index instead of p_uecc_gb_context->cell_index
 *
 * Revision 1.5  2010/03/24 09:50:42  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.4  2010/02/23 14:09:51  gur22133
 * after_lokesh_ue_cap_code_review_23feb
 *
 * Revision 1.3  2010/02/18 04:19:02  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.4.16  2009/12/15 11:54:43  ukr15916
 * srb0 is added during status (srb_num+1)
 *
 * Revision 1.1.4.15  2009/12/09 15:36:26  ukr16032
 * UECC - inspection log items PN_4, SG_1, SG_6, AK_1, AK_12, AK_25, AK_27 fixed
 *
 * Revision 1.1.4.14  2009/12/08 11:22:51  ukr16032
 * UECC - inspection log items LG_20, LG_23, LG_24, LG_26, LG_27, LG_28, LG_29,
 *  LG_34, LG_40, LG_41 fixed
 *
 * Revision 1.1.4.13  2009/11/26 18:33:49  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.4.11.2.26  2009/11/24 12:12:52  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.4.11.2.25  2009/11/24 11:32:35  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.4.11.2.24.2.3  2009/11/23 16:43:50  ukr15916
 * beautification
 *
 * Revision 1.1.4.11.2.24.2.2  2009/11/20 15:38:46  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.4.11.2.24.2.1  2009/11/19 15:34:29  ukr15916
 * RRC_NULL fixed
 *
 * Revision 1.1.4.11.2.24  2009/11/19 10:24:57  ukr15916
 * using memset_wrapper,l3_memcpy_wrapper,memcmp_wrapper
 *
 * Revision 1.1.4.11.2.23  2009/11/19 09:52:19  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.4.11.2.22  2009/11/18 17:06:14  ukr16032
 * UECC NAS processing updated
 * (except sending NAS PDU in non delivery msg).
 * Security capability processing fixed.
 * Transaction_id generetion updated.
 *
 * Revision 1.1.4.11.2.21  2009/11/17 09:42:25  ukr18880
 * uecc_ue_ctx_get_mme_ue_id(p_ue_context) function is now used instead of
 * directly using mme_ue_id from UE context, in all cases except it initializing
 *
 * Revision 1.1.4.11.2.20  2009/11/15 10:02:59  ukr15916
 * beautification
 *
 * Revision 1.1.4.11.2.19  2009/11/14 16:29:55  ukr16032
 * UECC UE Capability transfer implemented
 * UECC ICSP - security capability checking implemented
 *
 * Revision 1.1.4.11.2.18  2009/11/13 13:14:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.4.11.2.17  2009/11/13 12:36:55  ukr18880
 * *** empty log message ***
 *
 * Revision 1.1.4.11.2.16  2009/11/13 12:23:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.4.11.2.15  2009/11/12 12:31:21  ukr18880
 * *** empty log message ***
 *
 * Revision 1.1.4.11.2.14  2009/11/12 12:25:49  ukr18880
 * New parameter to global context was added from oamh provision request
 *
 * Revision 1.1.4.11.2.13  2009/11/11 20:42:34  ukr18880
 * OAMH fixed according to statistic
 *
 * Revision 1.1.4.11.2.12  2009/11/11 14:46:28  ukr18880
 * UECC statistic bug fix
 *
 * Revision 1.1.4.11.2.11  2009/11/10 20:31:18  ukr18880
 * UECC statistic was implemented
 *
 * Revision 1.1.4.11.2.10  2009/11/10 14:08:23  ukr18880
 * UECC statistic engine was updated
 *
 * Revision 1.1.4.11.2.9  2009/11/06 15:32:23  ukr18880
 * statistic engine was updated
 *
 * Revision 1.1.4.11.2.8  2009/11/06 11:17:21  ukr18880
 * Statistic engine was updated
 *
 * Revision 1.1.4.11.2.7  2009/10/30 15:21:08  ukr15916
 * Define RRC_UECC_N_UE is removed. Dynamic UE number supporting improved.
 *
 * Revision 1.1.4.11.2.6  2009/10/30 12:45:20  ukr15916
 * UECC-OAMH interface -- Get Cell Status and Get UE Status requests handling
 *     added
 *
 ****************************************************************************/

/****************************************************************************
 * Local includes
 ****************************************************************************/

#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"
#include "rrc_intrl_api.h"
#include "rrc_oamh_intf.h"
#include "uecc_common.h"
#include "uecc_oamh.h"
#include "uecc_utils.h"
#include "uecc_logger.h"
#include "uecc_ue_ctx.h"
#include "uecc_ed.h"
#include "uecc_s1ap.h"
#include "uecc_rrm.h"
#include <lteMisc.h>
extern uecc_fsm_activity_state_t uecc_fsm_ue_connection_release_ongoing;

/****************************************************************************
 * L O C A L    M A C R O s
 ****************************************************************************/
#define RRC_FLOATING_POINT_PRECISION 1000

/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/

static void uecc_oamh_print_current_configuration
(
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

static rrc_return_t uecc_oamh_prov_check_integrity_algorithms(
        uecc_gb_context_t                *p_uecc_gb_context,  /* UECC global context */
        rrc_oam_integrity_algorithms_t   *p_prov_integrity_algorithms,
        rrc_oam_integrity_algorithms_t   *p_integrity_algorithms 
);

static rrc_return_t uecc_oamh_prov_check_ciphering_algorithms(
        uecc_gb_context_t                *p_uecc_gb_context,
        rrc_oam_ciphering_algorithms_t   *p_prov_ciphering_algorithms,
        rrc_oam_ciphering_algorithms_t   *p_ciphering_algorithms
);

static rrc_return_t uecc_oamh_prov_check_and_process_prov_req_data
(
    rrc_uecc_oamh_prov_req_t    *p_prov_req,
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

static rrc_return_t uecc_oamh_fill_ue_status
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    rrc_ue_index_t      ue_index,
    rrc_cell_index_t     cell_index,
    rrc_oam_ue_status_t *p_ue_status
);
static void uecc_oamh_start_cell_traffic_trace
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    rrc_uecc_oamh_cell_traffic_trace_start_t *p_request
);
static void uecc_oamh_stop_cell_traffic_trace
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    rrc_cell_index_t    cell_index
);

void start_uecc_health_monitor_timer(
    uecc_gb_context_t*      p_uecc_gb_context,
    uecc_ue_timer_id_et     uecc_ue_timer_id,
    rrc_timer_duration_t    duration
);

/******************************************************************************
 * Function implementation
 *****************************************************************************/

/******************************************************************************
 * *   FUNCTION NAME: process_timer 
 * *
 * *   set timer with the value  as provided by OAMH.
 * *
 * *   RETURNS:
 * *       RRC_SUCCESS/RRC_FAILURE in actual argument sent in function call
 * *
 * ******************************************************************************/
static void process_timer(uecc_gb_context_t *p_uecc_gb_context,
        rrc_oam_uecc_timer_info_t   *p_timer_info,
        rrc_return_t                *result
)
{
    U16                         index = RRC_NULL;
    rrc_timer_duration_t        rcs_fsm_duration = RRC_NULL;
    rrc_timer_duration_t        rcr_fsm_duration = RRC_NULL;
    rrc_timer_duration_t        rm_fsm_duration = RRC_NULL;
    rrc_timer_duration_t        rrm_ue_capability_enq_duration = RRC_NULL;
    rrc_timer_duration_t        mme_guard_duration = 0;
    rrc_timer_duration_t        contxt_mod_duration = RRC_NULL;
    rrc_timer_duration_t        erab_mod_duration = RRC_NULL;
    rrc_timer_duration_t        erab_setup_duration = RRC_NULL; 
    rrc_timer_duration_t        erab_rel_cmd_duration = RRC_NULL;
    rrc_timer_duration_t        erab_rel_ind_duration = RRC_NULL;
    rrc_timer_duration_t        rlf_w_reest_duration = RRC_NULL;
    rrc_timer_duration_t        rlf_suspend_ue_duration = RRC_NULL;

    /*REL 1.2:HANDOVER PROCEDURE START*/
    rrc_timer_duration_t        ho_prep_duration = RRC_NULL;
    rrc_timer_duration_t        ho_overall_duration = RRC_NULL;
    rrc_timer_duration_t        ll_guard_duration = RRC_NULL;
    rrc_timer_duration_t        offset_duration = RRC_NULL;
    rrc_timer_duration_t        meas_config_guard_duration = RRC_NULL;
    rrc_timer_duration_t        trg_ho_guard_duration = RRC_NULL;
    rrc_timer_duration_t        cre_guard_duration = RRC_NULL;
    /*REL 1.2:HANDOVER PROCEDURE END*/

    /*x2_ho_timer*/
    rrc_timer_duration_t        x2_ho_prep_duration = RRC_NULL;
    rrc_timer_duration_t        x2_ho_overall_duration = RRC_NULL; 

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    /* Process timers */
    for (index = 0;
            (RRC_SUCCESS == (*result)) && (index < p_timer_info->num_of_timers);
            index++)
    {
        /* Check whether timer value is correct */
        if (RRC_NULL == p_timer_info->timer_info[index].timer_val)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING, "[OAMH] Zero timer value [id = %u]",
                    p_timer_info->timer_info[index].timer_id);
            (*result) = RRC_FAILURE;
            return;
        }

        /* Fill local timer values */
        switch (p_timer_info->timer_info[index].timer_id)
        {
            case UECC_RRC_CONN_SETUP_TIMER:
                rcs_fsm_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_RRC_CONN_RECONFIG_TIMER:
                rm_fsm_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_RRC_CONN_RELEASE_TIMER:
                rcr_fsm_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_RRM_UE_CAPABILITY_ENQ_TIMER:
                rrm_ue_capability_enq_duration = p_timer_info->timer_info[index].
                    timer_val;
                break;

            case UECC_RRC_MME_GUARD_TIMER:
                mme_guard_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_UE_CONTXT_MODIFY_TIMER:
                contxt_mod_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_ERAB_SETUP_TIMER:
                erab_setup_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_ERAB_MODIFY_TIMER:
                erab_mod_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_ERAB_RELEASE_CMD_TIMER:
                erab_rel_cmd_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_ERAB_RELEASE_IND_TIMER:
                erab_rel_ind_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_RLF_WAIT_FOR_REEST_TIMER:
                rlf_w_reest_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_RLF_UE_SUSPEND_TIMER:
                rlf_suspend_ue_duration = p_timer_info->timer_info[index].timer_val;
                break;

                /*REL 1.2:HANDOVER PROCEDURE START*/

            case UECC_RRC_HO_PREP_TIMER:
                ho_prep_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_RRC_HO_OVERALL_TIMER:
                ho_overall_duration =
                    p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_LL_GUARD_TIMER:
                ll_guard_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_RRC_OFFSET_TIMER:
                offset_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_MEAS_CONFIG_GUARD_TIMER:
                meas_config_guard_duration =
                    p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_RRC_TRG_HO_GUARD_TIMER:
                trg_ho_guard_duration =
                    p_timer_info->timer_info[index].timer_val;
                break;
                /*REL 1.2:HANDOVER PROCEDURE END*/

            case UECC_CRE_GUARD_TIMER:
                cre_guard_duration =
                    p_timer_info->timer_info[index].timer_val;
                break;
                /*x2_ho_timer :start*/ 
            case UECC_RRC_X2_HO_PREP_TIMER:
                x2_ho_prep_duration = p_timer_info->timer_info[index].timer_val;
                break;

            case UECC_RRC_X2_HO_OVERALL_TIMER:
                x2_ho_overall_duration =
                    p_timer_info->timer_info[index].timer_val;
                break;
                /*x2_ho_timer :end*/ 

            default:
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING, "[OAMH] Unknown timer [id = %u]",
                        p_timer_info->timer_info[index].timer_id);
                (*result) = RRC_FAILURE;
                return;
        }
    }

    if (RRC_SUCCESS == (*result))
    {
        /* Store parameters in UECC global context */
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_CONN_SETUP_TIMER,
                &p_uecc_gb_context->ue_timers_data, rcs_fsm_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_CONN_RELEASE_TIMER,
                &p_uecc_gb_context->ue_timers_data, rcr_fsm_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_CONN_RECONFIG_TIMER,
                &p_uecc_gb_context->ue_timers_data, rm_fsm_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_MME_GUARD_TIMER,
                &p_uecc_gb_context->ue_timers_data, mme_guard_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRM_UE_CAPABILITY_ENQ_TIMER,
                &p_uecc_gb_context->ue_timers_data, rrm_ue_capability_enq_duration);

        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_UE_CONTXT_MODIFY_TIMER,
                &p_uecc_gb_context->ue_timers_data, contxt_mod_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_ERAB_SETUP_TIMER,
                &p_uecc_gb_context->ue_timers_data, erab_setup_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_ERAB_MODIFY_TIMER,
                &p_uecc_gb_context->ue_timers_data, erab_mod_duration);

        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_ERAB_RELEASE_CMD_TIMER,
                &p_uecc_gb_context->ue_timers_data, erab_rel_cmd_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_ERAB_RELEASE_IND_TIMER,
                &p_uecc_gb_context->ue_timers_data, erab_rel_ind_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RLF_WAIT_FOR_REEST_TIMER,
                &p_uecc_gb_context->ue_timers_data, rlf_w_reest_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RLF_UE_SUSPEND_TIMER,
                &p_uecc_gb_context->ue_timers_data, rlf_suspend_ue_duration);

        /*REL 1.2:HANDOVER PROCEDURE START*/
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_HO_PREP_TIMER,
                &p_uecc_gb_context->ue_timers_data, (U16)(ho_prep_duration + 
                    offset_duration));
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_HO_OVERALL_TIMER,
                &p_uecc_gb_context->ue_timers_data, (U16)(ho_overall_duration +
                    offset_duration));
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_LL_GUARD_TIMER,
                &p_uecc_gb_context->ue_timers_data, ll_guard_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_OFFSET_TIMER,
                &p_uecc_gb_context->ue_timers_data, offset_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_MEAS_CONFIG_GUARD_TIMER,
                &p_uecc_gb_context->ue_timers_data, meas_config_guard_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_TRG_HO_GUARD_TIMER,
                &p_uecc_gb_context->ue_timers_data,trg_ho_guard_duration);
        /*REL 1.2:HANDOVER PROCEDURE END*/
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_CRE_GUARD_TIMER,
                &p_uecc_gb_context->ue_timers_data,cre_guard_duration);

        /*x2_ho_timer :start*/
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_X2_HO_PREP_TIMER,
                &p_uecc_gb_context->ue_timers_data, (U16)(x2_ho_prep_duration + 
                    offset_duration));
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_X2_HO_OVERALL_TIMER,
                &p_uecc_gb_context->ue_timers_data, (U16)(x2_ho_overall_duration +
                    offset_duration));
        /*x2_ho_timer :end*/
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
 * *   FUNCTION NAME: fill_prov_timer_info_in_uecc_global_context 
 * *
 * *   fill timer info into ue cc global context as provided by OAMH.
 * *
 * *   RETURNS:
 * *       RRC_SUCCESS/RRC_FAILURE
 * *
 * ******************************************************************************/

static rrc_return_t fill_prov_timer_info_in_uecc_global_context(
        rrc_uecc_oamh_prov_req_t    *p_prov_req,
        uecc_gb_context_t           *p_uecc_gb_context
        )
{
    rrc_return_t            result = RRC_SUCCESS;
    rrc_timer_duration_t    intra_cell_ho_guard_duration = RRC_NULL;
    rrc_timer_duration_t    ue_reconfig_duration = RRC_NULL;
    rrc_timer_duration_t    cco_guard_timer_duration = RRC_NULL;
    rrc_timer_duration_t    counter_check_guard_duration = RRC_NULL;
    rrc_timer_duration_t    ue_information_guard_duration = RRC_NULL;
    rrc_timer_duration_t    store_ue_context_timer_duration = RRC_NULL;
    rrc_timer_duration_t        health_monitor_timer_duration = RRC_NULL;
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    rrc_timer_duration_t    dc_prep_timer_duration = RRC_NULL;
    rrc_timer_duration_t    dc_bearer_change_guard_timer_duration = RRC_NULL;
/*NR_DC Code Change Stop*/
#endif

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    if ( RRC_UECC_TIMERS_INFO & p_prov_req->bitmask )
    {
        if ( RRC_UECC_INTRA_CELL_HO_GUARD_TIMER_PRESENT &
                p_prov_req->uecc_timers_info.bitmask )
        {
            intra_cell_ho_guard_duration = 
                p_prov_req->uecc_timers_info.uecc_intra_cell_ho_guard_timer;
        }
        if ( RRC_UECC_UE_RECONFIG_TIMER_PRESENT &
                p_prov_req->uecc_timers_info.bitmask )
        {
            ue_reconfig_duration = 
                p_prov_req->uecc_timers_info.uecc_ue_reconfig_timer;
        }
        if ( RRC_UECC_CCO_GUARD_TIMER_PRESENT &
                p_prov_req->uecc_timers_info.bitmask )
        {
            cco_guard_timer_duration = 
                p_prov_req->uecc_timers_info.uecc_cco_guard_timer;
        }
        if ( RRC_UECC_COUNTER_CHECK_TIMER_PRESENT &
                p_prov_req->uecc_timers_info.bitmask )
        {
            counter_check_guard_duration = 
                p_prov_req->uecc_timers_info.uecc_counter_check_timer;
        }
        if ( RRC_UECC_UE_INFORMATION_TIMER_PRESENT &
                p_prov_req->uecc_timers_info.bitmask )
        {
            ue_information_guard_duration = 
                p_prov_req->uecc_timers_info.uecc_ue_information_timer;
        }
        if ( RRC_UECC_STORE_UE_CONTEXT_TIMER_PRESENT &
                p_prov_req->uecc_timers_info.bitmask )
        {
            store_ue_context_timer_duration = 
                p_prov_req->uecc_timers_info.uecc_store_ue_context_timer;
        }
        if (RRC_UECC_HEALTH_MONITORING_TIMER_PRESENT &
                    p_prov_req->uecc_timers_info.bitmask)
        {
            health_monitor_timer_duration = 
                    p_prov_req->uecc_timers_info.uecc_health_monitor_timer_value * 1000;
        }
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
        if (RRC_UECC_DC_PREP_TIMER_PRESENT &
                    p_prov_req->uecc_timers_info.bitmask)
        {
            dc_prep_timer_duration = 
                    p_prov_req->uecc_timers_info.uecc_dc_prep_timer;
        }
        if(RRC_UECC_DC_BEARER_CHANGE_GUARD_PRESENT &
                p_prov_req->uecc_timers_info.bitmask)
        {
            dc_bearer_change_guard_timer_duration = 
                    p_prov_req->uecc_timers_info.uecc_dc_bearer_change_guard_timer;
        }
/*NR_DC Code Change Stop*/
#endif
    }

    /*Initialize UE contexts DB*/
    result = uecc_ue_ctx_db_init(p_uecc_gb_context); 
    if ( RRC_FAILURE == result )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_WARNING, "[OAMH] Out of memory");
    }

    if(p_prov_req->len_vendor_name > RRC_VENDOR_NAME_LEN)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_WARNING, "[OAMH] Wrong Vendor Name length");
        result = RRC_FAILURE;
    }
    
    if (result == RRC_SUCCESS)
    {
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_INTRA_CELL_HO_GUARD_TIMER,
                &p_uecc_gb_context->ue_timers_data,intra_cell_ho_guard_duration);

        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRM_UE_INFORMATION_REQ_TIMER,
                &p_uecc_gb_context->ue_timers_data,ue_information_guard_duration);

        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_COUNTER_CHECK_TIMER,
                &p_uecc_gb_context->ue_timers_data,counter_check_guard_duration);

        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRM_UE_RECONFIG_TIMER,
                &p_uecc_gb_context->ue_timers_data, (U16)(ue_reconfig_duration));
        /* SPR 4118 Start */
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_INTEGRITY_FAILURE_TIMER,
                &p_uecc_gb_context->ue_timers_data, p_uecc_gb_context->uecc_integrity_threshold_timer_value);
        /* SPR 4118 Stop */

        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_CCO_GUARD_TIMER,
                &p_uecc_gb_context->ue_timers_data,cco_guard_timer_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_STORE_UE_CONTEXT_TIMER,
                &p_uecc_gb_context->ue_timers_data,store_ue_context_timer_duration);
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_OAM_HEALTH_MONITOR_TIMER,
                &p_uecc_gb_context->ue_timers_data,health_monitor_timer_duration);
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_DC_PREP_TIMER,
                &p_uecc_gb_context->ue_timers_data,dc_prep_timer_duration);
/*NR_DC Code Change Stop*/
/*OPTION3X Changes Start*/
        uecc_ue_timer_set_durations(p_uecc_gb_context, UECC_RRC_DC_BEARER_CHANGE_GUARD_TIMER,
                &p_uecc_gb_context->ue_timers_data, dc_bearer_change_guard_timer_duration);
/*OPTION3X Changes Stop*/
#endif
        /* Vendor Name */
        RRC_ASSERT(sizeof(p_uecc_gb_context->vendor_name) ==
                sizeof(p_prov_req->vendor_name));

        p_uecc_gb_context->len_vendor_name = p_prov_req->len_vendor_name;

        l3_memcpy_wrapper(p_uecc_gb_context->vendor_name,
                p_prov_req->vendor_name,
                sizeof(p_uecc_gb_context->vendor_name));
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}


#if (LOGGER_TYPE == RRC_ADV_LOGGER)
/******************************************************************************
 * *   FUNCTION NAME: fill_prov_logger_info_in_uecc_global_context
 * *
 * *   Description: fill advanced logging info into ue cc global context
 * *
 *      RETURNS:  RRC_SUCCESS/RRC_FAILURE
 * *
 * ******************************************************************************/
rrc_return_t  fill_prov_logger_info_in_uecc_global_context(
        rrc_uecc_oamh_prov_req_t    *p_prov_req,
        uecc_gb_context_t           *p_uecc_gb_context)
{
    /*local*/
    rrc_return_t result = RRC_SUCCESS;
    U16          index = RRC_NULL;
    const S8   *uecc_facility_name = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    if (p_prov_req->bitmask & RRC_UECC_LOG_FILE_NAME_PRESENT) 
    {
        result = RRC_FAILURE;
        for(index = 0; index < sizeof(p_prov_req->log_file_name); index++)
        {
            if ('\0' == p_prov_req->log_file_name[index])
            {
                result = RRC_SUCCESS;
                break;
            }
        }

        if (RRC_FAILURE != result)
        {
            /*Create Advanced Logger File */
            for (index = 0; p_prov_req->log_file_name[index]; index++)
            {
                p_prov_req->log_file_name[index] = 
                    (S8)tolower_wrapper((p_prov_req->log_file_name[index]));
            }

            l3_memcpy_wrapper(p_uecc_gb_context->log_file_name, 
                    p_prov_req->log_file_name, sizeof(p_prov_req->log_file_name));

            result = create_adv_logger_file(p_uecc_gb_context);
        }
    }
    else if (p_prov_req->bitmask & RRC_UECC_NETWORK_INDEX_PRESENT)
    {
        switch (p_uecc_gb_context->uecc_module_id)
        {
            case  RRC_UECC_1_MODULE_ID:
                uecc_facility_name = (const S8*)"eNodeB RRC UECC1";
                break;
            case  RRC_UECC_2_MODULE_ID:
                uecc_facility_name = (const S8*)"eNodeB RRC UECC2";
                break;
            case  RRC_UECC_3_MODULE_ID:
                uecc_facility_name = (const S8*)"eNodeB RRC UECC3";
                break;
            case  RRC_UECC_4_MODULE_ID:
                uecc_facility_name = (const S8*)"eNodeB RRC UECC4";
                break;
            case  RRC_UECC_5_MODULE_ID:
                uecc_facility_name = (const S8*)"eNodeB RRC UECC5";
                break;
            case  RRC_UECC_6_MODULE_ID:
                uecc_facility_name = (const S8*)"eNodeB RRC UECC6";
                break;
            case  RRC_UECC_7_MODULE_ID:
                uecc_facility_name = (const S8*)"eNodeB RRC UECC7";
                break;
            case  RRC_UECC_8_MODULE_ID:
                uecc_facility_name = (const S8*)"eNodeB RRC UECC8";
                break;
            default :
                RRC_TRACE( RRC_WARNING,"Wrong Module Id");
                result = RRC_FAILURE;
                break;
        }
        p_uecc_gb_context->facility_name = (const S8 *)uecc_facility_name;
        p_uecc_gb_context->context_index = p_prov_req->network_index;
    }
    else if(p_prov_req->bitmask & RRC_UECC_SHARED_MEM_INDEX_PRESENT)
    {
        p_uecc_gb_context->facility_name = (const S8 *)uecc_facility_name;
        p_uecc_gb_context->context_index = p_prov_req->shared_mem_index;
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}
#endif
/******************************************************************************
 * *   FUNCTION NAME: fill_totalSupportedUeInfo_in_ueCcGbContext 
 * *
 * *   Description: fill total supported UE info into UE CC Global context 
 * *
 * *   RETURNS:
 * *       RRC_SUCCESS/RRC_FAILURE
 * *
 * ******************************************************************************/
static rrc_return_t fill_totalSupportedUeInfo_in_ueCcGbContext(
        rrc_uecc_oamh_prov_req_t    *p_prov_req,
        uecc_gb_context_t           *p_uecc_gb_context,
        rrc_oam_uecc_timer_info_t   *p_timer_info)
{
    /*local*/
    rrc_return_t result = RRC_SUCCESS;
    U8           cell_index = RRC_NULL;
    U8           received_cell_index = RRC_NULL;
    U8           counter = RRC_NULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    /* Check total_ue_supported */

    /*SPR 5759 start*/
    if ( MAX_NUM_SUPPORTED_UE_PER_UECC < p_prov_req->total_ue_supported )
    {
        /*SPR 5759 end*/
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_WARNING,
                "[OAMH] Incorrect total_ue_supported [%u]",
                p_prov_req->total_ue_supported);
        return RRC_FAILURE;
    }


    /* Check timers count */
    if (p_timer_info->num_of_timers > MAX_UECC_TIMER_INFO)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_WARNING,
                "[OAMH] Incorrect number of timers [%u]",
                p_timer_info->num_of_timers);
        return RRC_FAILURE;
    }

    p_uecc_gb_context->total_ue_supported = 
        p_prov_req->total_ue_supported;

    p_uecc_gb_context->max_ue_context_list_size = 1;
    if(p_prov_req->bitmask & RRC_UECC_UE_CONTEXT_LIST_SIZE_PRESENT)
    {
        for (counter = 0; counter < p_prov_req->ue_context_list_size;
                counter ++)
        {
            p_uecc_gb_context->max_ue_context_list_size = 
                2*p_uecc_gb_context->max_ue_context_list_size;
        }
    }
    else
    {
        for (counter = 0; counter < UECC_UE_CONTEXT_LIST_SIZE_DEFAULT;
                counter ++)
        {
            p_uecc_gb_context->max_ue_context_list_size = 
                2*p_uecc_gb_context->max_ue_context_list_size;
        }
    }
    RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
            p_uecc_gb_context->facility_name, RRC_INFO,
            "max_ue_context_list_size:%d",
            p_uecc_gb_context->max_ue_context_list_size);

    /* Check integrity protection capabilities */
    p_uecc_gb_context->supported_security_algo_per_cell.num_cells = 
        p_prov_req->supported_security_algo_per_cell.num_cells;

    for ( cell_index = 0; cell_index < p_prov_req->
            supported_security_algo_per_cell.num_cells; cell_index++)
    {
        received_cell_index = p_prov_req->supported_security_algo_per_cell.
            supported_security_algorithms[cell_index].cell_index;

        if ( RRC_SUCCESS !=
                uecc_oamh_prov_check_integrity_algorithms(
                    p_uecc_gb_context,
                    &p_prov_req->supported_security_algo_per_cell.
                    supported_security_algorithms[cell_index]
                    .integrity_algorithms, &p_uecc_gb_context->
                    supported_security_algo_per_cell.supported_security_algorithms[
                    received_cell_index].integrity_algorithms))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING,
                    "[OAMH] Cell Index [%u]",
                    received_cell_index);
            memset_wrapper(&(p_uecc_gb_context->supported_security_algo_per_cell),RRC_NULL,
                    sizeof(rrc_oam_supported_security_algo_per_cell_t));
            result = RRC_FAILURE;
            break;
        }
        /* Check ciphering capabilities */
        if ( RRC_SUCCESS != 
                uecc_oamh_prov_check_ciphering_algorithms(
                    p_uecc_gb_context,
                    &p_prov_req->supported_security_algo_per_cell.
                    supported_security_algorithms[cell_index].ciphering_algorithms,
                    &p_uecc_gb_context->supported_security_algo_per_cell.
                    supported_security_algorithms[received_cell_index].ciphering_algorithms))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING,
                    "[OAMH] Cell Index [%u]",
                    received_cell_index);
            memset_wrapper(&(p_uecc_gb_context->supported_security_algo_per_cell),RRC_NULL,
                    sizeof(rrc_oam_supported_security_algo_per_cell_t));
            result = RRC_FAILURE;
            break;
        }
        p_uecc_gb_context->supported_security_algo_per_cell.
            supported_security_algorithms[received_cell_index].cell_index = p_prov_req->
            supported_security_algo_per_cell.supported_security_algorithms[cell_index].cell_index;
    }

    /* Allocating Memory for CSC Context in UECC Global Conetxt */
    if (PNULL == (p_uecc_gb_context->p_p_csc_context = (rrc_csc_context_t**)
                rrc_mem_get((MAX_NUM_CELLS) * sizeof(rrc_csc_context_t*))) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_WARNING,
                "[OAMH] Mem Allocation failed");

        result = RRC_FAILURE;
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_oamh_process_timer_msg
*   INPUT        : rrc_timer_t         timer_id
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : none
*   DESCRIPTION  : This function processes OAMH timer message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_oamh_process_timer_msg
(
    rrc_timer_t         timer_id, /* Identity of the expired timer */
    uecc_gb_context_t   *p_uecc_gb_context /* UECC global context */
)
{

    RRC_ASSERT(PNULL != timer_id);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);


    /* Currently we have only one timer for OAMH */
    if (timer_id == p_uecc_gb_context->oamh_init_ind_timer)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_BRIEF, "[OAMH] Send init indication...");

        /* Send wake up indication to OAMH */
        if ( RRC_FAILURE == uecc_rrc_send_internal_msg( RRC_OAMH_MODULE_ID,
                UECC_OAMH_INIT_IND, 0, PNULL) )
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR, "[OAMH] Fail sending init indication");
        }
    }
    else
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_ERROR, "[OAMH] Unknown timer id [%p] received",
            timer_id);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}


/******************************************************************************
*   FUNCTION NAME: uecc_oamh_print_current_configuration
*   INPUT        : uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : none
*   DeSCRIPTION  : This function prints out current UECC configuration
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_oamh_print_current_configuration
(
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    U8                  algo_index = RRC_NULL;
    U8                  gb_algo_integrity = RRC_NULL;
    U8                  gb_algo_ciphering = RRC_NULL;
    U8                  oam_ciphering_algo  = RRC_NULL;
    U8                  sector_index        = RRC_NULL;
    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_INFO, "[OAMH] Number of supported UEs [%u]",
        p_uecc_gb_context->total_ue_supported);


    /* timers */
    uecc_ue_timers_print_durations(p_uecc_gb_context,&p_uecc_gb_context->ue_timers_data);

    /* Store security capabilities */
    /* Integrity protection algorithms */
    for (sector_index=0; sector_index< p_uecc_gb_context->
            supported_security_algo_per_cell.num_cells; sector_index++)
    {
        for (algo_index = 0; algo_index < p_uecc_gb_context->
                supported_security_algo_per_cell.supported_security_algorithms[sector_index].
                integrity_algorithms.num_algorithms; algo_index++)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, "[OAMH] Cell index [%u]",
            p_uecc_gb_context->supported_security_algo_per_cell.
            supported_security_algorithms[sector_index].cell_index);
            
            gb_algo_integrity = p_uecc_gb_context->supported_security_algo_per_cell.
                supported_security_algorithms[sector_index].integrity_algorithms.
                algorithms[algo_index];
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
                    "[OAMH] Supported Integrity protection algorithms "
                    "[128-EIA0=%s] [128-EIA1=%s] [128-EIA2=%s]",
                    gb_algo_integrity & UECC_INTEGRITY_PROTECTION_EIA0_BIT?"YES":"NO",
                    gb_algo_integrity & UECC_INTEGRITY_PROTECTION_EIA1_BIT?"YES":"NO",
                    gb_algo_integrity & UECC_INTEGRITY_PROTECTION_EIA2_BIT?"YES":"NO");
        }            

        /* Ciphering algorithm */
        for (algo_index = 0; algo_index < p_uecc_gb_context->
                supported_security_algo_per_cell.supported_security_algorithms[sector_index].
                ciphering_algorithms.num_algorithms; algo_index++)
        {
            gb_algo_ciphering = p_uecc_gb_context->supported_security_algo_per_cell.
                supported_security_algorithms[sector_index].ciphering_algorithms.
                algorithms[algo_index];

            oam_ciphering_algo = (U8) (oam_ciphering_algo | 1 << gb_algo_ciphering);
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
                    "[OAMH] Supported Ciphering algorithms "
                    "[128-EEA0=%s] [128-EEA1=%s] [128-EEA2=%s]",

                    oam_ciphering_algo & UECC_CIPHERING_EEA0_BIT?"YES":"NO",
                    oam_ciphering_algo & UECC_CIPHERING_EEA1_BIT?"YES":"NO",
                    oam_ciphering_algo & UECC_CIPHERING_EEA2_BIT?"YES":"NO");
        }
    }
}

/******************************************************************************
*   FUNCTION NAME: uecc_oamh_prov_check_integrity_algorithms
*   INPUT        : uecc_gb_context_t                *p_uecc_gb_context
*                  rrc_oam_integrity_algorithms_t   *p_prov_integrity_algorithms
*                  rrc_oam_integrity_algorithms_t    *p_integrity_algorithms
*   OUTPUT       : none
*   DESCRIPTION  : Checks integrity protection capabilities provided by OAMH.
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_t uecc_oamh_prov_check_integrity_algorithms(
        uecc_gb_context_t                *p_uecc_gb_context,
        rrc_oam_integrity_algorithms_t   *p_prov_integrity_algorithms,
        /* Received from Global context */
        rrc_oam_integrity_algorithms_t    *p_integrity_algorithms
)
{
    rrc_return_t result = RRC_SUCCESS;
    U16          index = RRC_NULL;

    /* Check integrity protection capabilities */
    index = 0;

    while (index < p_prov_integrity_algorithms->num_algorithms)
    {
        if (RRC_PDCP_INT_ALG_EIA2 <
                p_prov_integrity_algorithms->algorithms[index])
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING,
                    "[OAMH] Incorrect integrity algorithm [%u]",
                    p_prov_integrity_algorithms->algorithms[index]);
            result = RRC_FAILURE;
            break;
        }

    /* Storing the information in Global context */
        p_integrity_algorithms->num_algorithms = p_prov_integrity_algorithms->
        num_algorithms;
        p_integrity_algorithms->algorithms[index] = p_prov_integrity_algorithms->
        algorithms[index];

        index++;
    }

    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_oamh_prov_check_ciphering_algorithms
*   INPUT        : uecc_gb_context_t                *p_uecc_gb_context
*                  rrc_oam_ciphering_algorithms_t   *p_prov_ciphering_algorithms
*                  rrc_oam_ciphering_algorithms_t    *p_ciphering_algorithms
*   INPUT        : none
*   DESCRIPTION  : Checks ciphering capabilities provided by OAMH.
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_t uecc_oamh_prov_check_ciphering_algorithms(
        uecc_gb_context_t                *p_uecc_gb_context,
        rrc_oam_ciphering_algorithms_t   *p_prov_ciphering_algorithms,
        rrc_oam_ciphering_algorithms_t    *p_ciphering_algorithms
)
{
    rrc_return_t result = RRC_SUCCESS;
    U16          index = RRC_NULL;

    /* Check ciphering capabilities */
    index = 0;

    while (index < p_prov_ciphering_algorithms->num_algorithms)
    {
        if (RRC_PDCP_CIPH_ALG_EEA2 <
                p_prov_ciphering_algorithms->algorithms[index] )
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING,
                    "[OAMH] Incorrect ciphering algorithm [%u]",
                    p_prov_ciphering_algorithms->algorithms[index]);
            result = RRC_FAILURE;
            break;
        }

        /* Storing the information in Global context */
        p_ciphering_algorithms->num_algorithms = p_prov_ciphering_algorithms->
        num_algorithms;
        p_ciphering_algorithms->algorithms[index] = p_prov_ciphering_algorithms->
        algorithms[index];

        index++;
    }

    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_oamh_prov_req_processs_msg
*   INPUT        : rrc_uecc_oamh_prov_req_t    *p_prov_req,
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : none               
*   DESCRIPTION  : This function checks UECC_OAMH_PROV_REQ message data and,
*       if all parameters are valid, then it updates UECC global context.
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_t uecc_oamh_prov_check_and_process_prov_req_data
(
    rrc_uecc_oamh_prov_req_t    *p_prov_req,
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    rrc_oam_uecc_timer_info_t* p_timer_info = PNULL;
    rrc_return_t result = RRC_SUCCESS;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
/*
    U16                         integrity_algorithms  = 0;
    U16                         ciphering_algorithms  = 0;
*/
    p_timer_info = &p_prov_req->timer_info;

    do
    {
        result = fill_totalSupportedUeInfo_in_ueCcGbContext(
                p_prov_req,
                p_uecc_gb_context,
                p_timer_info);
        if(RRC_FAILURE == result) 
        {
            break;
        }

        memset_wrapper(p_uecc_gb_context->p_p_csc_context,
                RRC_NULL, MAX_NUM_CELLS*sizeof(rrc_csc_context_t*));

        /* Process timers */
        process_timer(p_uecc_gb_context,p_timer_info, &result);        
        if (RRC_FAILURE == result)
        {
            break;
        }

        result = fill_prov_timer_info_in_uecc_global_context(p_prov_req,
                        p_uecc_gb_context);
        
        if (RRC_FAILURE == result)
        {
            break;
        }

#if (LOGGER_TYPE == RRC_ADV_LOGGER)
        result = fill_prov_logger_info_in_uecc_global_context(
                        p_prov_req,
                        p_uecc_gb_context);
#endif
    } while (0);

        /* SPR 4118 Start */
        if (p_prov_req->bitmask &
                RRC_UECC_INTEGRITY_THRESHOLD_COUNTER_PRESENT)
        {
            p_uecc_gb_context->pdcp_integrity_threshold_counter =
                p_prov_req->integrity_threshold_counter;
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, "Integrity Threshold Counter = %u",
                    p_uecc_gb_context->pdcp_integrity_threshold_counter);
        }

        if (p_prov_req->bitmask &
                RRC_UECC_INTEGRITY_TIMER_VALUE_PRESENT)
        {
            p_uecc_gb_context->uecc_integrity_threshold_timer_value =
                p_prov_req->uecc_integrity_timer_value;
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, "Integrity Timer Duration = %u",
                    p_uecc_gb_context->uecc_integrity_threshold_timer_value);
        }
        /* SPR 4118 Stop */

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}
/******************************************************************************
*   FUNCTION NAME: uecc_oamh_prov_req_processs_msg
*   INPUT        : void                *p_api
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : none
*   DESCRIPTION  : This function processes UECC_OAMH_PROV_REQ message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_oamh_prov_req_processs_msg
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    rrc_uecc_oamh_prov_req_t    *p_prov_req = PNULL;
    rrc_uecc_oamh_prov_resp_t   *p_prov_resp = PNULL;
    U8                          *p_msg = PNULL;
    U16                         msg_api_length = RRC_NULL;

    rrc_return_t                result = RRC_FAILURE;


    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_BRIEF, "[OAMH] [EVENT:UECC_OAMH_PROV_REQ]");

    /* Allocate memory for response */
    p_msg = uecc_alloc_intrl_msg(RRC_UECCMD_MODULE_ID,
                                UECC_OAMH_PROV_RESP,
                                sizeof(rrc_uecc_oamh_prov_resp_t));
    if (PNULL == p_msg)
    {
        /*out of memory*/
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_WARNING,
                "[OAMH] Low memory for UECC_OAMH_PROV_RESP");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }

    /* Fill the source module id from the received message */

    msg_api_length = (U16)(sizeof(rrc_uecc_oamh_prov_resp_t) + RRC_API_HEADER_SIZE); 

    /* Fill RRC header */
    rrc_construct_api_header( p_msg, RRC_VERSION_ID, p_uecc_gb_context->uecc_module_id,
            RRC_UECCMD_MODULE_ID, UECC_OAMH_PROV_RESP, msg_api_length);

    p_prov_resp = (rrc_uecc_oamh_prov_resp_t *)(p_msg + RRC_API_HEADER_SIZE);

    do {
        /* Precondition - UECC_OAMH_INIT_CNF should be received */
        if (PNULL != p_uecc_gb_context->oamh_init_ind_timer)
        {
            /* Not ready for provision request processing*/
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING, "[OAMH] Not ready "
                    "for provision request processing - ignore");
            break;
        }

        /* Precondition - UECC_OAMH_PROV_REQ can be received just once */
        if (UECC_READY_OAMH_FLAG & p_uecc_gb_context->ready_condition)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING, "[OAMH] Received again - ignore");
            break;
        }

        /* Precondition - p_api should point to right message */
        if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE +
                                           sizeof(rrc_uecc_oamh_prov_req_t) )
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING, "[OAMH] Wrong message size - ignore");
            break;
        }

        /* Get message payload */
        p_prov_req = (rrc_uecc_oamh_prov_req_t*)((U8*)p_api
                     + RRC_API_HEADER_SIZE);

        /* Check and process parameters */
        if(p_prov_req->uecc_timers_info.bitmask &
                RRC_UECC_HEALTH_MONITORING_TIMER_PRESENT)
        {
            start_uecc_health_monitor_timer(p_uecc_gb_context,
                    UECC_HEALTH_MONITOR_TIMER,
                    ((p_prov_req->uecc_timers_info.uecc_health_monitor_timer_value * 1000)/2));
        }

        result = uecc_oamh_prov_check_and_process_prov_req_data(
                                p_prov_req, p_uecc_gb_context);


        if ( RRC_SUCCESS == result )
        {
            uecc_oamh_print_current_configuration(p_uecc_gb_context);

            /* Set OAMH ready condition */
            p_uecc_gb_context->ready_condition |= UECC_READY_OAMH_FLAG;

            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_DETAILED, "[OAMH] Ready");
        }
    } while (0);

    /* negative response by default*/
    p_prov_resp->result = result;

    /* Send response */
    rrc_send_message(p_msg, RRC_UECCMD_MODULE_ID);
    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_BRIEF, "PROVISIONING RESPONSE SENT TO OAMH");

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_oamh_clear_req_processs_msg
*
*   INPUT        : void                *p_api
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : none
* DESCRIPTION    : This function processes UECCMD_OAMH_CLEANUP_REQ message and
*       resets UECC state
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_oamh_clear_req_processs_msg
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    rrc_module_id_t             src_module_id = RRC_NULL;
    U8                          *p_intrl_msg = PNULL;
    U16                         msg_api_length = RRC_NULL;


    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_BRIEF, "[OAMH] [EVENT:UECCMD_OAMH_CLEANUP_REQ]");

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + 0)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_WARNING, "[OAMH] Wrong message size - ignore.");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }

    src_module_id = p_uecc_gb_context->uecc_module_id;

    /********************************
     * Reset UECC
     ********************************/

    rrc_uecc_clean_up(p_uecc_gb_context);

    p_uecc_gb_context->uecc_module_id = (U8)src_module_id;
    p_uecc_gb_context->offset_uecc_mod_id = (U8)(src_module_id - RRC_UECC_1_MODULE_ID);

    msg_api_length = (U16)(RRC_API_HEADER_SIZE + 0);
    
    /* Allocate buffer for CLEANUP_RESP*/
    p_intrl_msg = rrc_msg_mem_get(RRC_API_HEADER_SIZE + 0);

    if (PNULL == p_intrl_msg)
    {
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }

    memset_wrapper(p_intrl_msg, 0, msg_api_length);

    rrc_construct_api_header(   p_intrl_msg, RRC_VERSION_ID, src_module_id,
                                RRC_UECCMD_MODULE_ID, UECCMD_OAMH_CLEANUP_RESP,
                                msg_api_length);

    /* Send message to */
    rrc_send_message( p_intrl_msg, RRC_UECCMD_MODULE_ID);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_oamh_cell_traffic_trace_start_processs_msg
*   INPUT        : void                *p_api
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : none
*
*   DESCRIPTION  : This function processes UECC_OAMH_CELL_TRAFFIC_TRACE_START message and
*       start trace of each Ue in cell.
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_oamh_cell_traffic_trace_start_processs_msg
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    rrc_uecc_oamh_cell_traffic_trace_start_t *p_request = PNULL;
    rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t  *p_resp = PNULL;
    rrc_uecc_proc_status_ind_t                          *p_status_ind = PNULL;
    U8                                      *p_msg = PNULL;
    U8                                      response = RRC_FAILURE;
    U16                                    msg_api_length = RRC_NULL;
    U8      cause = RRC_INVALID_CAUSE; /* rrc_trace_start_error_cause_et */


    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_BRIEF, "[OAMH] [EVENT:UECC_OAMH_CELL_TRAFFIC_TRACE_START]");

    do {
        if (PNULL != p_uecc_gb_context->p_p_csc_context)
        {
            /* Precondition - p_api should point to right message */
            if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE +
                    sizeof(rrc_uecc_oamh_cell_traffic_trace_start_t))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING, "[OAMH] Wrong message size - ignore.");
                break;
            }

            /* Get message payload */
            p_request = (rrc_uecc_oamh_cell_traffic_trace_start_t*)((U8*)p_api
                    + RRC_API_HEADER_SIZE);

            if (PNULL == p_uecc_gb_context->p_p_csc_context[p_request->cell_index])
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING, "[OAMH] Incorrect Cell Index ");
                cause = RRC_WRONG_CELL_INDEX; 
                break;
            }
            else
            {
                if (PNULL != p_uecc_gb_context->p_p_csc_context[p_request->
                        cell_index]->p_csc_init_setup_ind)
                {
                    if (p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->
                            del_cell_ongoing)
                    {
                        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                                facility_name, RRC_WARNING, "[OAMH]-Cell deletion is ongoing");
                        cause = RRC_CELL_DELETION_ONGOING; 
                        break;
                    }

                    else if (RRC_CSC_CELL_STOPPED == p_uecc_gb_context->
                            p_p_csc_context[p_request->cell_index]->p_csc_init_setup_ind->cell_state )
                    { 
                        /* SPR 22584 Changes - Starts */
                        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                                facility_name, RRC_WARNING, "[OAMH]-Cell := %u not active",
                                p_request->cell_index);
                        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                        cause = RRC_CELL_INACTIVE;
                        break;
                        /* SPR 22584 Changes - Ends */
                    }
                    else if (p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->
                            cell_trace_ongoing)
                    {
                        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                                facility_name, RRC_WARNING, "[OAMH]-Cell trace is ongoing");
                        cause = RRC_DUPLICATE_REQ; 
                        break;
                    }
                }

                /* Allocate memory to store trace activation control and
                 * configuration parameter */
                if (PNULL == (p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->p_cell_traffic_trace_info =
                            (rrc_uecc_cell_traffic_trace_info_t*)
                            rrc_mem_get(sizeof(rrc_uecc_cell_traffic_trace_info_t))))
                {
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                            facility_name, RRC_FATAL,
                            "[CELL TRAFFIC TRACE]Memory Allocation Failed ");
                    return;
                }
                else
                {
                    memset_wrapper(p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->p_cell_traffic_trace_info, RRC_NULL,
                            sizeof(rrc_uecc_cell_traffic_trace_info_t));

                    /* interfaces to trace */
                    p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->p_cell_traffic_trace_info->interface_to_trace =
                        p_request->interface_to_trace;

                    /* trace depth */
                    p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->p_cell_traffic_trace_info->trace_depth =
                        p_request->trace_depth;

                    /* trace collection entity info */
                    l3_memcpy_wrapper(&p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->p_cell_traffic_trace_info->
                            trace_collection_entity,
                            &p_request->trace_collection_entity ,
                            sizeof(p_request->trace_collection_entity));

                    /* Set cell_trace_ongoing flag in UECC global context so that
                     * trace activation can be applicable for each existing UE and 
                     * for new connection in Cell */
                    p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->
                        cell_trace_ongoing = RRC_TRUE;
                    // Trace Change Start
                    /* Set traceId and papulate Trace Reference 
                     * Assign trace id at lower three bytes */
                    l3_memcpy_wrapper(&p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->traceRef,
                               &p_request->traceRef,
                               (sizeof(p_request->traceRef)));
                    // Trace Change End
                }
            }

            /* Call the function to activate trace for existing UE's in cell */
            uecc_oamh_start_cell_traffic_trace(p_uecc_gb_context, p_request);

            response = RRC_SUCCESS;
            msg_api_length = (U16)(sizeof(rrc_uecc_proc_status_ind_t) + RRC_API_HEADER_SIZE);
            /* Prepare success ind message for UECCMD */
            p_msg = uecc_alloc_intrl_msg(RRC_UECCMD_MODULE_ID,
                    UECCMD_UECC_PROC_STATUS_IND,
                    msg_api_length);

            if (PNULL == p_msg)
            {
                /*out of memory*/
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_FATAL,
                        "[OAMH] Memory allocation failed"
                        "UECCMD_UECC_PROC_STATUS_IND");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                return;
            }
            /* Fill RRC header */
            rrc_construct_api_header( p_msg, RRC_VERSION_ID, p_uecc_gb_context->uecc_module_id,
                    RRC_UECCMD_MODULE_ID, UECCMD_UECC_PROC_STATUS_IND, msg_api_length);

            p_status_ind = (rrc_uecc_proc_status_ind_t*)(p_msg
                    + RRC_API_HEADER_SIZE);

            p_status_ind->result = RRC_SUCCESS;
            p_status_ind->cell_index = p_request->cell_index;
            rrc_send_message(p_msg, RRC_UECCMD_MODULE_ID);

        }
    }
    while (0);

    if (response != RRC_SUCCESS && cause != RRC_INVALID_CAUSE)
    {
        /* Prepare response */
        p_msg = uecc_alloc_intrl_msg(RRC_UECCMD_MODULE_ID,
                UECC_OAMH_CELL_TRAFFIC_TRACE_START_ERROR_IND,
                sizeof(rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t));

        if (PNULL == p_msg)
        {
            /*out of memory*/
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_FATAL,
                    "[OAMH] Memory allocation failed"
                    "UECC_OAMH_CELL_TRAFFIC_TRACE_START_ERROR_IND");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            return;
        }

        /* Fill RRC header */
        msg_api_length = (U16)(sizeof(rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t) + RRC_API_HEADER_SIZE);
        rrc_construct_api_header( p_msg, RRC_VERSION_ID, RRC_UECC_MODULE_ID,
                RRC_UECCMD_MODULE_ID, UECC_OAMH_CELL_TRAFFIC_TRACE_START_ERROR_IND, msg_api_length);

        p_resp = (rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t*)(p_msg
                + RRC_API_HEADER_SIZE);

        p_resp->transaction_id = p_request->transaction_id;
        p_resp->cell_index = p_request->cell_index;
        p_resp->cause = cause;
        /* Send CELL TRAFFIC TRACE START ERROR IND */
        rrc_send_message(p_msg, RRC_UECCMD_MODULE_ID);
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_oamh_cell_traffic_trace_stop_processs_msg
*   INPUT        : void                *p_api
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : none
*
*   DESCRIPTION  : This function processes UECC_OAMH_CELL_TRAFFIC_TRACE_START message and
*       start trace of each Ue in cell.
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_oamh_cell_traffic_trace_stop_processs_msg
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    rrc_uecc_oamh_cell_traffic_trace_stop_t *p_request = PNULL;


    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_BRIEF, "[OAMH] [EVENT:UECC_OAMH_CELL_TRAFFIC_TRACE_STOP]");

    do {
        if (PNULL != p_uecc_gb_context->p_p_csc_context)
        {
            /* Precondition - p_api should point to right message */
            if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE +
                    sizeof(rrc_uecc_oamh_cell_traffic_trace_stop_t))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING, "[OAMH] Wrong message size - ignore.");
                break;
            }

            /* Get message payload */
            p_request = (rrc_uecc_oamh_cell_traffic_trace_stop_t*)((U8*)p_api
                    + RRC_API_HEADER_SIZE);

            if (PNULL == p_uecc_gb_context->
                        p_p_csc_context[p_request->cell_index])
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING, "[OAMH] Incorrect Cell Index-Ignore Message");
                break;
            }
            else if (RRC_CSC_CELL_STOPPED == p_uecc_gb_context->
                    p_p_csc_context[p_request->cell_index]->p_csc_init_setup_ind->cell_state )
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING,"cell not active");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                return;
            }

            /* Reset cell_trace_ongoing flag in UECC global context 
             * to stop cell level tracing */
            p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->
                cell_trace_ongoing = RRC_FALSE;

            /* Function activate trace for UE's in cell */
            uecc_oamh_stop_cell_traffic_trace(p_uecc_gb_context, p_request->cell_index);

            if (PNULL != p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->p_cell_traffic_trace_info)
            {
                /* Deallocate memory previously used for cell traffic trace */
                rrc_mem_free(p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->p_cell_traffic_trace_info);
                p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->p_cell_traffic_trace_info = PNULL;
            }
        }
    }
    while (0);
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_oamh_trace_file_transfer_complete_ind_processs_msg
*   INPUT        : void                *p_api
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : none
*
*   DESCRIPTION  : This function processes UECC_OAMH_TRACE_FILE_TRANSFER_COMPLETE_IND 
*       message and delete_trace_file.
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_oamh_trace_file_transfer_complete_ind_processs_msg
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    rrc_uecc_oamh_trace_file_transfer_complete_ind_t *p_request = PNULL;


    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_BRIEF, "[OAMH] "
        "[EVENT:UECC_OAMH_TRACE_FILE_TRANSFER_COMPLETE_IND]");

    do {
        /* Precondition - p_api should point to right message */
        if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE +
               sizeof(rrc_uecc_oamh_trace_file_transfer_complete_ind_t))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING, "[OAMH] Wrong message size - ignore.");
            break;
        }

        /* Get message payload */
        p_request = (rrc_uecc_oamh_trace_file_transfer_complete_ind_t*)((U8*)
                        p_api + RRC_API_HEADER_SIZE);

        if(PNULL != p_uecc_gb_context->p_p_csc_context)
        {
            if (PNULL == p_uecc_gb_context->p_p_csc_context[p_request->cell_index])
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING, "[OAMH] Incorrect Cell Index-Ignore Message");
                break;
            }
            else if (RRC_CSC_CELL_STOPPED == p_uecc_gb_context->
                    p_p_csc_context[p_request->cell_index]->p_csc_init_setup_ind->cell_state )
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING,"cell not active");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                return;
            }

        }
    }
    while (0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_oamh_get_cell_status_req_processs_msg
*   INPUT        : void                *p_api
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : none
*
*   DESCRIPTION  : This function processes UECC_OAMH_GET_CELL_STATUS_REQ message and
*                  sends UECC cell status data to OAMH module.
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_oamh_get_cell_status_req_processs_msg
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    rrc_uecc_oamh_get_cell_status_req_t   *p_request = PNULL;
    rrc_uecc_oamh_get_cell_status_resp_t  *p_resp = PNULL;
    U8                                    *p_msg = PNULL;
    U8                                    response = RRC_FAILURE;
    U16                                    msg_api_length = RRC_NULL;
    rrc_cell_status_t                     *p_cell_status = PNULL;


    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_BRIEF, "[OAMH] [EVENT:UECC_OAMH_GET_CELL_STATUS_REQ]");

    /* Get message payload */
    p_request = (rrc_uecc_oamh_get_cell_status_req_t*)((U8*)p_api
            + RRC_API_HEADER_SIZE);
    
    /* Prepare response */
    p_msg = uecc_alloc_intrl_msg(RRC_UECCMD_MODULE_ID,
                                 UECC_OAMH_GET_CELL_STATUS_RESP,
                                 sizeof(rrc_uecc_oamh_get_cell_status_resp_t));
    if (PNULL == p_msg)
    {
        /*out of memory*/
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_WARNING,
                "[OAMH] Low memory for UECC_OAMH_GET_CELL_STATUS_REQ");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }
    /* Fill RRC header */
    msg_api_length = (U16)(sizeof(rrc_uecc_oamh_get_cell_status_resp_t) + RRC_API_HEADER_SIZE);

    rrc_construct_api_header( p_msg, RRC_VERSION_ID, p_uecc_gb_context->uecc_module_id,
            RRC_UECCMD_MODULE_ID, UECC_OAMH_GET_CELL_STATUS_RESP, msg_api_length);
    p_resp = (rrc_uecc_oamh_get_cell_status_resp_t*)(p_msg
                  + RRC_API_HEADER_SIZE);

    do {
        p_resp->bitmask = 0;

        if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(
                    p_uecc_gb_context,p_request->cell_index))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING,"cell not active");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            break;
        }
        /* Precondition - p_api should point to right message */
        if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE +
                                   sizeof(rrc_uecc_oamh_get_cell_status_req_t))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING, "[OAMH] Wrong message size - ignore.");
            break;
        }

        /* Precondition - UECC_OAMH_PROV_REQ should be received */
        if ( !(UECC_READY_OAMH_FLAG & p_uecc_gb_context->ready_condition) )
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING, "[OAMH] Not ready - reject");
            break;
        }

        /********************************
         * Get Cell status
         ********************************/
        p_resp->bitmask |= UECC_OAMH_CELL_STATUS_PRESENT;
        
        if (PNULL != p_uecc_gb_context->p_p_csc_context)
        {
            if (PNULL != p_uecc_gb_context->p_p_csc_context[p_request->cell_index])
            {
                if (PNULL != p_uecc_gb_context->p_p_csc_context
                        [p_request->cell_index]->p_rrc_uecc_statistic)
                {
                    p_cell_status =
                        &p_uecc_gb_context->p_p_csc_context[p_request->cell_index]->
                        p_rrc_uecc_statistic->rrc_cell_status;


                    p_resp->cell_status.num_ue = p_cell_status->rrc_num_of_ue_per_cell;
                    p_resp->cell_status.num_srb1 = p_cell_status->rrc_num_of_srb1_per_cell;
                    p_resp->cell_status.num_srb2 = p_cell_status->rrc_num_of_srb2_per_cell;
                    p_resp->cell_status.num_drb = p_cell_status->rrc_num_of_drb_per_cell;

                    response = RRC_SUCCESS;
                }
            }
        }

    }
    while (0);

    p_resp->transaction_id = p_request->transaction_id;
    p_resp->cell_index = p_request->cell_index;
    p_resp->response = response;

    /* Send response */
    rrc_send_message(p_msg,RRC_UECCMD_MODULE_ID);
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}


/******************************************************************************
 *   FUNCTION NAME: uecc_oamh_fill_ue_status
 *                  rrc_cell_index_t     cell_index,
 *                  rrc_oam_ue_status_t *p_ue_status
 *   OUTPUT       : none
 *   DESCRIPTION  : This function gets and fills UE status UECC UE status data
 *                  for OAMH module.
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
    rrc_return_t uecc_oamh_fill_ue_status
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 rrc_ue_index_t       ue_index,
 rrc_cell_index_t     cell_index,
 rrc_oam_ue_status_t *p_ue_status
 )
{
    rrc_return_t        result = RRC_FAILURE;
    uecc_ue_context_t   *p_ue_context = PNULL;

    /* get status for given UE */
    p_ue_context = uecc_ue_ctx_get(p_uecc_gb_context, ue_index);
    if ( PNULL != p_ue_context )
    {
        if (p_ue_context->cell_index == cell_index) 
        {
            p_ue_status->ue_index = p_ue_context->ue_index;
            p_ue_status->crnti = p_ue_context->crnti;
            p_ue_status->ue_state = p_ue_context->ue_status.ue_state;
            p_ue_status->ue_category = p_ue_context->ue_category;

            /* UECC_STATUS_INITIAL_SRB_NUM == 1 (we always have SRB0) */
            p_ue_status->num_srb =
                (U8)(p_ue_context->ue_status.ue_num_of_srb1 +
                        p_ue_context->ue_status.ue_num_of_srb2 + 
                        UECC_STATUS_INITIAL_SRB_NUM);

            p_ue_status->num_drb =
                p_ue_context->ue_status.ue_num_of_drb + UECC_STATUS_INITIAL_DRB_NUM;

            p_ue_status->enb_ue_s1ap_id = p_ue_context->s1ap_ue_id;

            if( 0 != p_ue_context->m.mme_ue_id_present)
            {
                p_ue_status->bitmask |= RRC_OAM_UE_STATUS_MME_UE_S1AP_ID_PRESENT;
                p_ue_status->mme_ue_s1ap_id = uecc_ue_ctx_get_mme_ue_id(p_ue_context);
            }

            result = RRC_SUCCESS;
        }   
    }

    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_oamh_get_ue_status_req_processs_msg
*   INPUT        : void                *p_api
*                  uecc_gb_context_t   *p_uecc_gb_context
*   OUTPUT       : none
*
*   DESCRIPTION  : This function processes UECC_OAMH_GET_UE_STATUS_REQ message and
*                  `sends UECC UE status data to OAMH module.
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_oamh_get_ue_status_req_processs_msg
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    rrc_uecc_oamh_get_ue_status_req_t   *p_request = PNULL;
    rrc_uecc_oamh_get_ue_status_resp_t  *p_resp = PNULL;
    U8      *p_msg = PNULL;
    U8      response = RRC_FAILURE;
    U16     index = RRC_NULL;
    U16     msg_api_length = RRC_NULL;


    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_BRIEF, "[OAMH] [EVENT:UECC_OAMH_GET_UE_STATUS_REQ]");

    /* Get message payload */
    p_request = (rrc_uecc_oamh_get_ue_status_req_t*)((U8*)p_api
            + RRC_API_HEADER_SIZE);
    
    if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(
                p_uecc_gb_context,p_request->cell_index))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_WARNING,"cell not active");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }
    /* Prepare response */
    if ( UECC_OAMH_UE_INDEX_PRESENT & p_request->bitmask)
    {
        p_msg = uecc_alloc_intrl_msg(RRC_OAMH_MODULE_ID,
                UECC_OAMH_GET_UE_STATUS_RESP,
                sizeof(rrc_uecc_oamh_get_ue_status_resp_t));
        if (PNULL == p_msg)
        {
            /*out of memory*/
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING,
                    "[OAMH] Low memory for UECC_OAMH_GET_UE_STATUS_RESP");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            return;
        }
    }
    else
    {
        p_msg = uecc_alloc_intrl_msg(RRC_UECCMD_MODULE_ID,
                UECC_OAMH_GET_UE_STATUS_RESP,
                sizeof(rrc_uecc_oamh_get_ue_status_resp_t));
        if (PNULL == p_msg)
        {
            /*out of memory*/
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING,
                    "[OAMH] Low memory for UECC_OAMH_GET_UE_STATUS_RESP");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            return;
        }
        /* Fill RRC header */
        msg_api_length = (U16)(sizeof(rrc_uecc_oamh_get_ue_status_resp_t) + RRC_API_HEADER_SIZE);

        rrc_construct_api_header( p_msg, RRC_VERSION_ID, p_uecc_gb_context->uecc_module_id,
                RRC_UECCMD_MODULE_ID, UECC_OAMH_GET_UE_STATUS_RESP, msg_api_length);
    }
    p_resp = (rrc_uecc_oamh_get_ue_status_resp_t*)(p_msg
                  + RRC_API_HEADER_SIZE);

    do {
        /* Precondition - p_api should point to right message */
        if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE +
                                   sizeof(rrc_uecc_oamh_get_ue_status_req_t))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING, "[OAMH] Wrong message size - ignore.");
            break;
        }

        /* Precondition - UECC_OAMH_PROV_REQ should be received */
        if ( !(UECC_READY_OAMH_FLAG & p_uecc_gb_context->ready_condition) )
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING, "[OAMH] Not ready - reject");
            break;
        }

        /********************************
         * Get UE Status
         ********************************/

        p_resp->bitmask = 0;

        if ( UECC_OAMH_UE_INDEX_PRESENT & p_request->bitmask)
        {
            /* get status for given UE */
            if (RRC_SUCCESS ==
                uecc_oamh_fill_ue_status(
                        p_uecc_gb_context,
                        p_request->ue_index,
                        p_request->cell_index, 
                        &p_resp->ue_status_list.ue_status[0]) )
            {
                p_resp->bitmask |= UECC_OAMH_UE_STATUS_LIST_PRESENT;
                p_resp->ue_status_list.num_ue_status = 1;
            }
        }
        else
        {
            /* get status for all UEs */
            p_resp->ue_status_list.num_ue_status = 0;
            for (index = 0;
                    index < p_uecc_gb_context->total_ue_supported;
                    index++)
            {
                /* get status for selected UE */
                if (RRC_SUCCESS ==
                    uecc_oamh_fill_ue_status(
                            p_uecc_gb_context,
                            index,
                            p_request->cell_index,
                            &p_resp->ue_status_list.ue_status[
                                      p_resp->ue_status_list.num_ue_status]) )
                {
                    p_resp->ue_status_list.num_ue_status++;
                }

                if ( (RRC_MAX_NUM_UE_STATUS ==
                     p_resp->ue_status_list.num_ue_status)
                     /* don't send if there is last index value*/
                     && (index <
                        ((p_uecc_gb_context->total_ue_supported) - 1)))
                {
                    /* Send part of UE status data to OAMH */
                    p_resp->bitmask |= UECC_OAMH_UE_STATUS_LIST_PRESENT;
                    p_resp->transaction_id = p_request->transaction_id;
                    p_resp->cell_index = p_request->cell_index;
                    p_resp->response = RRC_SUCCESS;

                    /* Send response */
                    rrc_send_message(p_msg, RRC_UECCMD_MODULE_ID);

                    /* allocate new message buffer */
                    /* Prepare response */
                    p_msg = uecc_alloc_intrl_msg(
                                RRC_UECCMD_MODULE_ID,
                                UECC_OAMH_GET_UE_STATUS_RESP,
                                sizeof(rrc_uecc_oamh_get_ue_status_resp_t));
                    if (PNULL == p_msg)
                    {
                        /*out of memory*/
                        RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                                p_uecc_gb_context->facility_name, RRC_WARNING,
                                "[OAMH] Low memory for UECC_OAMH_GET_UE_STATUS_RESP");
                        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                        return;
                    }
                    msg_api_length = RRC_NULL;

                    /* Fill RRC header */
                    msg_api_length = (U16)(sizeof(rrc_uecc_oamh_get_ue_status_resp_t) + RRC_API_HEADER_SIZE);

                    rrc_construct_api_header( p_msg, RRC_VERSION_ID, p_uecc_gb_context->uecc_module_id,
                            RRC_UECCMD_MODULE_ID, UECC_OAMH_GET_UE_STATUS_RESP, msg_api_length);
                    p_resp = (rrc_uecc_oamh_get_ue_status_resp_t*)(p_msg
                            + RRC_API_HEADER_SIZE);
                    p_resp = (rrc_uecc_oamh_get_ue_status_resp_t*)(p_msg
                            + RRC_API_HEADER_SIZE);

                    p_resp->bitmask = 0;
                    p_resp->ue_status_list.num_ue_status = 0;
                }
            }

            if (0 < p_resp->ue_status_list.num_ue_status)
            {
                p_resp->bitmask |= UECC_OAMH_UE_STATUS_LIST_PRESENT;
            }
        }

        response = RRC_SUCCESS;
    }
    while (0);

    p_resp->transaction_id = p_request->transaction_id;
    p_resp->cell_index = p_request->cell_index;
    p_resp->response = response;

    /* Send response */
    if ( UECC_OAMH_UE_INDEX_PRESENT & p_request->bitmask)
    {
        rrc_send_message(p_msg, RRC_OAMH_MODULE_ID);
    }
    else
    {
        rrc_send_message(p_msg, RRC_UECCMD_MODULE_ID);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/******************************************************************************
*   FUNCTION NAME: uecc_oamh_start_cell_traffic_trace
*   INPUT        : uecc_gb_context_t   *p_uecc_gb_context,
*                  rrc_uecc_oamh_cell_traffic_trace_start_t *p_request
*   OUTPUT       : none
*   DESCRIPTION  : This function start trace for each UE in cell.
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_oamh_start_cell_traffic_trace
(
    uecc_gb_context_t   *p_uecc_gb_context,
    rrc_uecc_oamh_cell_traffic_trace_start_t *p_request
)
{
    U16                 ue_index = RRC_NULL;
    U16                 ctx_index = RRC_NULL;
    uecc_ue_context_t   *p_ue_context = PNULL;
    rrc_uecc_oamh_trace_start_error_ind_t *p_resp = PNULL; 
    U8*                  p_msg = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    /* allocate memory for UECC_OAMH_TRACE_START_ERROR_IND msg */
    p_msg = uecc_alloc_intrl_msg(RRC_OAMH_MODULE_ID,
                                 UECC_OAMH_TRACE_START_ERROR_IND,
                                 sizeof(rrc_uecc_oamh_trace_start_error_ind_t));
    if (PNULL == p_msg)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_WARNING,
                "[OAMH] Memory allocation failed"
                "for UECC_OAMH_TRACE_START_ERROR_IND");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }

    p_resp = (rrc_uecc_oamh_trace_start_error_ind_t*)(p_msg
                  + RRC_API_HEADER_SIZE);

   for (ctx_index = 0; 
        ctx_index < p_uecc_gb_context->total_ue_supported;
        ctx_index++)
   {
       p_ue_context = p_uecc_gb_context->ue_db.ue_contexts[ctx_index];
       if ((p_ue_context != PNULL) &&
               (p_ue_context->cell_index == p_request->cell_index))
       {
           /* TRACE_fix */
	   /*SPR 15896 Fix Start*/
           if ((PNULL != p_ue_context) && (!(p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE))
             && (!(p_ue_context->uecc_fsm.p_current_state == &uecc_fsm_ue_connection_release_ongoing)))
	   /*SPR 15896 Fix Stop*/
           /* TRACE_fix */
           {
               /* Allocate memory to store trace activation control and
                * configuration parameter */
	       /*SPR 15896 Fix Start*/
               if (PNULL == (p_ue_context->p_ue_cell_trace_activation_info =
                           (ue_trace_activation_info_t*)
                           rrc_mem_get(sizeof(ue_trace_activation_info_t))))
	   	/*SPR 15896 Fix Stop*/
               {
                   RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                           facility_name, RRC_FATAL,
                           "[CELL TRAFFIC TRACE]"
                           "Memory Allocation Failed For %d UE_Index", ue_index);

		   /* Coverity Fix 48259 Start */
		   if ( PNULL != p_msg )
		   {
		       /* SPR 15887 Start */
		       rrc_msg_mem_free(p_msg);
		       /* SPR 15887 Stop */
		       p_msg = PNULL;
		   }
		   /* Coverity Fix 48259 Stop */
		   RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
		   return;
	       }
               else
               {
	       	   /*SPR 15896 Fix Start*/
                   memset_wrapper(p_ue_context->p_ue_cell_trace_activation_info, RRC_NULL,
                           sizeof(ue_trace_activation_info_t));
	       	 
                   /* rrc_trace_activation_flag_et*/
                   p_ue_context->m.traceActivated |= RRC_TRACE_ACTIVATION_CELL_INITIATE;
		   /*SPR 15896 Fix Stop*/	

                   /* trace reference and trace recording 
                    * session reference */
                   uecc_generate_eutran_traceid(p_ue_context);
                   

                   /* store interfaces to trace */
	       	   /*SPR 15896 Fix Start*/
                   p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace = 
                       p_request->interface_to_trace;  

                   /* store trace depth */
	       	   
                   p_ue_context->p_ue_cell_trace_activation_info->trace_depth =
                       (trace_depth_et)p_request->trace_depth;

                   /* Copy trace collection entity info into Ue Context */
	       	   
                   l3_memcpy_wrapper(&p_ue_context->p_ue_cell_trace_activation_info->
                           trace_collection_entity_info,
                           &p_request->trace_collection_entity ,
                           sizeof(p_request->trace_collection_entity));
	       	   /*SPR 15896 Fix Stop*/

                   /* function to create file name and open file for UE*/
                   uecc_generate_trace_file_name(p_ue_context,TRACE_OAM_INITIATED);
		   /*SPR 15896 Fix Start*/	
		   /*Code Removed*/
		   /*SPR 15896 Fix Stop*/	

                   /* Send only when mme_ue_s1ap_id is present */
                   if (p_ue_context->m.mme_ue_id_present) 
                   {
                       /* Build and send CELL_TRAFFIC_TRACE to S1AP */
                       if (RRC_SUCCESS != 
                               uecc_s1ap_build_and_send_cell_traffic_trace(p_ue_context))
                       {
                           RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                                   p_uecc_gb_context->facility_name, RRC_WARNING,
                                   "Send to fail [CELL TAFFIC TRACE] for %d ue_index", ue_index);
                       }
                   }
               }
           }
		   /*SPR 15896 Fix Start*/	
           else if ((PNULL != p_ue_context) && (p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE)
             && (!(p_ue_context->uecc_fsm.p_current_state == &uecc_fsm_ue_connection_release_ongoing)))
           {
               p_resp->trace_failure_list.item
                   [p_resp->trace_failure_list.counter].ue_index = ue_index;

               p_resp->trace_failure_list.item
                   [(p_resp->trace_failure_list.counter)++].cause = 
                   RRC_OAM_INITIATED_TRACE_ALREADY_ONGOING;
           }
		   /*SPR 15896 Fix Stop*/	
           /* TRACE_fix */
           else if ( (PNULL != p_ue_context) && (PNULL != p_ue_context->uecc_fsm.p_current_state)
                 && (p_ue_context->uecc_fsm.p_current_state == &uecc_fsm_ue_connection_release_ongoing))
           {
               RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                       p_uecc_gb_context->facility_name, RRC_WARNING,
                       "Ignoring start_cell_traffic_trace as UE_RELEASE is ongoing.");
           }
           /* TRACE_fix */
       }
   }
   if (RRC_NULL < p_resp->trace_failure_list.counter)
   {
       /* transaction id */
       p_resp->transaction_id = p_request->transaction_id;
       /* cell_index */
       p_resp->cell_index = p_request->cell_index;
       /* Send response */
       rrc_send_message(p_msg, RRC_OAMH_MODULE_ID);
   }
   else 
   {
       /* Coverity Fix 48259 Start */
       if ( PNULL != p_msg )
       { 
	   /* SPR 15887 Start */
	   rrc_msg_mem_free(p_msg);
	   /* SPR 15887 Stop */
	   p_msg = PNULL;
       }
       /* Coverity Fix 48259 Stop */
   }
   RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/*SPR 15896 Fix Start*/
/******************************************************************************
 *   FUNCTION NAME: uecc_oamh_stop_cell_traffic_trace
 *   INPUT        : uecc_gb_context_t   *p_uecc_gb_context,
 *                  rrc_cell_index_t    cell_index
 *   OUTPUT       : none
 *   DESCRIPTION  : This function stop trace for each UE in cell.
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void uecc_oamh_stop_cell_traffic_trace
(
 uecc_gb_context_t   *p_uecc_gb_context,
 rrc_cell_index_t    cell_index
 )
{
    rrc_uecc_oamh_trace_file_transfer_start_ind_t *p_out = PNULL;
    U8                  *p_msg = PNULL;
    uecc_ue_context_t   *p_ue_context = PNULL;
    U16                 ctx_index = RRC_NULL;
    /*SPR 15875 Fix Start*/
    trace_id_info_t *p_trace_info = PNULL;
    U8 mcc[4] = {RRC_NULL};
    U8 mnc[4] = {RRC_NULL};
    U8 trace_id[7] = {RRC_NULL};
    /*SPR 15875 Fix Stop*/	
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    for (ctx_index = 0; 
            ctx_index < p_uecc_gb_context->total_ue_supported;
            ctx_index++)
    {
        p_ue_context = p_uecc_gb_context->ue_db.ue_contexts[ctx_index];
        if ((PNULL != p_ue_context) && (p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE)
        /* SPR 18088 Fix Start */        
                                                                && (p_ue_context->cell_index == cell_index))
        /* SPR 18088 Fix End */
        {
            /* Reset flag to stop tracing for UE */
	   /*SPR 15875 Fix Start*/
    	   p_trace_info = &(p_ue_context->p_ue_cell_trace_activation_info->trace_id_info);
	   /*Extracting TTRACE ID*/
		      snprintf_wrapper((Char8*)trace_id,7,(const Char8*)"%02X%02X%02X",p_trace_info->cell_eutran_trace_id.trace_reference[3],
                  				p_trace_info->cell_eutran_trace_id.trace_reference[4],
						p_trace_info->cell_eutran_trace_id.trace_reference[5]);
                  /*Extracting MCC*/
		      snprintf_wrapper((Char8*)mcc,4,(const Char8*)"%d%d%d",(p_trace_info->cell_eutran_trace_id.trace_reference[0] & 0x0F),
                 				((p_trace_info->cell_eutran_trace_id.trace_reference[0] & 0xF0)>>4),
						(p_trace_info->cell_eutran_trace_id.trace_reference[1] & 0x0F));

                  /*Extracting MNC*/
		  /*Check if the higher nibble contains filler digit*/
		  /*If present , discard the filler digit('F')*/
                  if(0xF0 == (p_trace_info->cell_eutran_trace_id.trace_reference[1] & 0xF0))
                  {
		      snprintf_wrapper((Char8*)mnc,4,(const Char8*)"%d%d",(p_trace_info->cell_eutran_trace_id.trace_reference[2] & 0x0F),
                   				((p_trace_info->cell_eutran_trace_id.trace_reference[2] & 0xF0)>>4));
                  }
                  else
                  {
		      snprintf_wrapper((Char8*)mnc,4,(const Char8*)"%d%d%d",((p_trace_info->cell_eutran_trace_id.trace_reference[1] & 0xF0)>>4),
                   				(p_trace_info->cell_eutran_trace_id.trace_reference[2] & 0x0F),
						((p_trace_info->cell_eutran_trace_id.trace_reference[2] & 0xF0)>>4));
                  }
	   P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,"\n<traceSessionRef>");
	   P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,"\n<MCC>");
	   P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,mcc);
	   P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,"</MCC>");
	   P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,"\n<MNC>");
	   P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,mnc);
	   P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,"</MNC>");
	   P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,"\n<TRACE_ID>");
	   P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,trace_id);
	   P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,"</TRACE_ID>");
	   P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,"\n</traceSessionRef>");
	   P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,"\n</traceRecSession> \n </traceCollecFile>"); 	
	    /*SPR 15875 Fix Stop*/	

           /* Reset flag to stop tracing for UE */
            p_ue_context->m.traceActivated &= (0xFF^RRC_TRACE_ACTIVATION_CELL_INITIATE);
	    /*SPR 15875 Fix Start*/	
	    /*Code removed*/	
	    /*SPR 15875 Fix Stop*/	

            /* Close Trace file */
            /* Coverity fix 87056 start */
            if ((PNULL != p_ue_context->p_ue_cell_trace_activation_info) && 
                    (PNULL != p_ue_context->p_ue_cell_trace_activation_info->fp))

                /* Coverity fix 87056 end */
            {
                rrc_fclose(p_ue_context->p_ue_cell_trace_activation_info->fp);
                p_ue_context->p_ue_cell_trace_activation_info->fp = PNULL;
            }

            /* allocate memory to send trace_file_transfer_start_ind */
            p_msg = uecc_alloc_intrl_msg(RRC_OAMH_MODULE_ID,
                    UECC_OAMH_TRACE_FILE_TRANSFER_START_IND,
                    sizeof(rrc_uecc_oamh_trace_file_transfer_start_ind_t));

            if (PNULL == p_msg)
            { 
                /*out of memory*/
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_FATAL,
                        "[OAMH] Low memory for "
                        "UECC_OAMH_TRACE_FILE_TRANSFER_START_IND ");
                return;
            }
            /* Point to message */ 
            p_out = (rrc_uecc_oamh_trace_file_transfer_start_ind_t*)(p_msg + 
                    RRC_API_HEADER_SIZE); 

            /* cell_index */
            p_out->cell_index = cell_index;

            /* ue_index */
            p_out->ue_index = p_ue_context->ue_index;

            if (PNULL != p_ue_context->p_ue_cell_trace_activation_info)
            {
                /* e_utran_trace_id: trace reference */
                l3_memcpy_wrapper(p_out->e_utran_trace_id.trace_reference, 
                        p_ue_context->p_ue_cell_trace_activation_info->trace_id_info.
                        cell_eutran_trace_id.trace_reference,
                        sizeof(p_out->e_utran_trace_id.trace_reference)); 

                /* e_utran_trace_id: trace recording session reference */
                l3_memcpy_wrapper(&(p_out->e_utran_trace_id.trace_recording_session_reference), 
                        p_ue_context->p_ue_cell_trace_activation_info->trace_id_info.
                        cell_eutran_trace_id.trace_recording_session_reference,
                        sizeof(p_out->e_utran_trace_id.
                            trace_recording_session_reference)); 

                /* length of file */
                p_out->trace_file.len_file_name = 
                    (U16)(strlen_wrapper((const Char8 *)(p_ue_context->p_ue_cell_trace_activation_info->trace_file)));

                /* trace file name */
                l3_memcpy_wrapper(p_out->trace_file.file_name, 
                        p_ue_context->p_ue_cell_trace_activation_info->trace_file, 
                        (U16)(p_out->trace_file.len_file_name + 1)); /* +1 for Null Character */

                /* Trace Collection IP Addr */
                l3_memcpy_wrapper(&(p_out->trace_collection_entity), 
                        &(p_ue_context->p_ue_cell_trace_activation_info->
                            trace_collection_entity_info), 
                        (U16)(sizeof(p_out->trace_collection_entity))); 

                /* Send Message */
                rrc_send_message(p_msg, RRC_OAMH_MODULE_ID);

                /* Deallocate memory whis was used to store trace control and 
                 * configuration parameters in Ue Context */
                rrc_mem_free(p_ue_context->p_ue_cell_trace_activation_info);
                p_ue_context->p_ue_cell_trace_activation_info = PNULL;
            }
            else
            {
		RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
			p_uecc_gb_context->facility_name, RRC_FATAL, 
			"p_ue_trace_activation_info pointer is null.");
		/* Coverity Fix 63283 Start */
		if ( PNULL != p_msg )
		{
		    /* SPR 15887 Start */
		    rrc_msg_mem_free(p_msg);
		    /* SPR 15887 Stop */
		    p_msg = PNULL;
		}
		/* Coverity Fix 63283 Stop */
	    }
	}
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/*SPR 15896 Fix Stop*/


#if (LOGGER_TYPE == RRC_ADV_LOGGER)
/******************************************************************************
 *   FUNCTION NAME: create_adv_logger_file
 *   INPUT        : uecc_gb_context_t   *p_uecc_gb_context
 *   OUTPUT       : none               
 *   DESCRIPTION:
 *       This function creates Logger file
 *
 *   RETURNS:
 *       rrc_return_t
 *
 ******************************************************************************/
rrc_return_t create_adv_logger_file( uecc_gb_context_t   *p_uecc_gb_context)
{

    const S8   *uecc_facility_name = PNULL;
    const S8   *adv_logger_file_name = PNULL;
    U16        index;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    switch (p_uecc_gb_context->uecc_module_id)
    {
        case  RRC_UECC_1_MODULE_ID:
            uecc_facility_name = (const S8*)"eNodeB RRC UECC1";
            adv_logger_file_name = (const S8*)"_uecc1";
            break;
        case  RRC_UECC_2_MODULE_ID:
            uecc_facility_name = (const S8*)"eNodeB RRC UECC2";
            adv_logger_file_name = (const S8*)"_uecc2";
            break;
        case  RRC_UECC_3_MODULE_ID:
            uecc_facility_name = (const S8*)"eNodeB RRC UECC3";
            adv_logger_file_name = (const S8*)"_uecc3";
            break;
        case  RRC_UECC_4_MODULE_ID:
            uecc_facility_name = (const S8*)"eNodeB RRC UECC4";
            adv_logger_file_name = (const S8*)"_uecc4";
            break;
        case  RRC_UECC_5_MODULE_ID:
            uecc_facility_name = (const S8*)"eNodeB RRC UECC5";
            adv_logger_file_name = (const S8*)"_uecc5";
            break;
        case  RRC_UECC_6_MODULE_ID:
            uecc_facility_name = (const S8*)"eNodeB RRC UECC6";
            adv_logger_file_name =(const S8*) "_uecc6";
            break;
        case  RRC_UECC_7_MODULE_ID:
            uecc_facility_name = (const S8*)"eNodeB RRC UECC7";
            adv_logger_file_name = (const S8*)"_uecc7";
            break;
        case  RRC_UECC_8_MODULE_ID:
            uecc_facility_name = (const S8*)"eNodeB RRC UECC8";
            adv_logger_file_name = (const S8*)"_uecc8";
            break;
        default :
            RRC_TRACE( RRC_WARNING,"Wrong Module Id");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            return RRC_FAILURE;

    }
    p_uecc_gb_context->facility_name = (const S8 *)uecc_facility_name;

    /* COVERITY_31274 */
    l3_strncat_wrapper(p_uecc_gb_context->log_file_name, adv_logger_file_name, 
                (MAX_UECC_FILE_NAME_SIZE - MAX_FILE_NAME_SIZE));
    /* COVERITY_31274 */

    if (LogSuccess == init_log(LogTarget_File,
                (const char *)p_uecc_gb_context->log_file_name, &index))
    {
        p_uecc_gb_context->context_index = index;
    }
    else
    {
        RRC_TRACE(RRC_WARNING, "Couldn't init file logging for UECC Module = %d!",
                p_uecc_gb_context->uecc_module_id);
        return RRC_FAILURE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);

    return RRC_SUCCESS;
}
#endif
/* lipa start */
/******************************************************************************
*   FUNCTION NAME: uecc_oamh_add_del_lgw_ind_processs_msg
*
*       This function processes UECC_OAMH_LGW_INFO_IND message and
*       stores the LGW configuration
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_oamh_add_del_lgw_ind_processs_msg
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    uecc_oamh_add_del_lgw_ind_t *p_add_del_lgw_ind = PNULL;

    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_BRIEF, "[OAMH] [EVENT:UECC_OAMH_LGW_INFO_IND]");

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof(uecc_oamh_add_del_lgw_ind_t))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_WARNING, "[OAMH] Wrong message size - ignore.");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }

   /* Get message payload */
    p_add_del_lgw_ind = (uecc_oamh_add_del_lgw_ind_t*)((U8*)p_api
                     + RRC_API_HEADER_SIZE);

    /* If bitmask = 1 , then store the lgw address in uecc_global_context */
    if (p_add_del_lgw_ind->bitmask)
    {
     	p_uecc_gb_context->lgw_configured_status = RRC_TRUE;
        p_uecc_gb_context->lgw_data_length = p_add_del_lgw_ind->data_length;   
        l3_memcpy_wrapper((void *)&p_uecc_gb_context->lgw_ip_addr, 
                   (void *)&p_add_del_lgw_ind->lgw_addr,
                   p_add_del_lgw_ind->data_length);
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_INFO, "LGW configured at UECC module");
    }

    /* if bitmask = 0, then an indication is received that
     * LGW will be deleted and LOCAL ERROR IND is to be sent to RRM so that RRM can
     * initiate the release procedure for ongoing LIPA RABs */
    else
    {
     	p_uecc_gb_context->lgw_configured_status = RRC_FALSE;
        uecc_rrm_build_and_send_rrc_rrm_local_error_ind(p_uecc_gb_context, OAM_INITIATED_LGW_DEL, p_api);   
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_INFO, "LGW configuration deleted at UECC module");
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/* lipa end */
