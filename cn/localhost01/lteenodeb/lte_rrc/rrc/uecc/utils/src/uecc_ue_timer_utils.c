/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ***************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_ue_timer_utils.c,v 1.20 2010/10/13 07:03:56 gur18569 Exp $
 ***************************************************************************
 *
 *  File Description : UECC UE Timers engine implementation
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * Jan 14, 2010   grag -   initial creation
 * $Log: uecc_ue_timer_utils.c,v $
 * Revision 1.20  2010/10/13 07:03:56  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.6.4.8  2010/10/07 11:26:09  gur04518
 * update for kclockwork warning
 *
 * Revision 1.6.4.7  2010/10/07 06:54:21  gur04518
 * corrected the boundry check for timer_index
 *
 * Revision 1.6.4.6  2010/09/22 14:53:45  gur25381
 * klocwork warning solved after merging
 *
 * Revision 1.6.4.5  2010/09/13 13:04:13  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.6.4.4  2010/08/10 10:04:11  gur18569
 * incorported review comments for ho
 *
 * Revision 1.6.4.3  2010/08/05 08:08:56  gur15697
 * merging round 2
 *
 * Revision 1.6.4.2  2010/07/21 08:41:39  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.6.4.1  2010/06/24 12:55:10  gur22047
 * internal source and s1ap code
 *
 * Revision 1.6  2010/04/20 05:45:37  gur21006
 * Klocwork warnings removed
 *
 * Revision 1.5  2010/04/07 15:32:59  gur21006
 * warnings removed
 *
 * Revision 1.4  2010/04/05 08:13:07  gur18569
 * added ue capability info to uecc_ue_timers_descriptors
 *
 * Revision 1.3  2010/03/31 18:27:33  gur22133
 * ssit_changes
 *
 * Revision 1.2  2010/03/24 10:42:40  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.1.2.3  2010/03/05 10:40:07  gur21006
 * int changed to S32 and char changed to S8
 *
 * Revision 1.1.2.2  2010/01/20 11:19:01  ukr15916
 * updated according to code review
 *
 * Revision 1.1.2.1  2010/01/15 10:09:04  ukr16032
 * UECC FSM - Multi timers functionality has been added
 *
 *
 ***************************************************************************/

/****************************************************************************
 * Inculdes
 ****************************************************************************/
#include "rrc_common_utils.h"
#include "uecc_common.h"
#include "uecc_logger.h"
#include "uecc_fsm_common.h"
#include "rrc_oam_intf.h"
#include "uecc_ue_timer_utils.h"
#include "uecc_global_ctx.h"

/****************************************************************************
 * Local definitions
 ****************************************************************************/

/****************************************************************************
 * Local Types definitions
 ****************************************************************************/

typedef const struct
{
    U16     oam_timer_id;
    U16     uecc_timer_id;
} uecc_ue_timer_oam_to_uecc_id_t;

/****************************************************************************
 * Local Variables
 ****************************************************************************/
/*static uecc_ue_timer_descriptor_t * uecc_ue_timers_descriptors[UECC_UE_TIMER_LAST] =*/
uecc_ue_timer_descriptor_t uecc_ue_timers_descriptors[UECC_UE_TIMER_LAST] =
{
    {UECC_TIMER_RCS_DEFAULT_DURATION,       UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_RCS_TIMER"},
    {UECC_TIMER_RM_DEFAULT_DURATION,        UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_ICS_TIMER"},
    {UECC_TIMER_RCR_DEFAULT_DURATION,       UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_RCR_TIMER"},
    {UECC_TIMER_MME_GUARD_DEFAULT_DURATION, UECC_EV_MME_GUARD_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_MME_GUARD_TIMER"},
    {UECC_TIMER_RRM_UE_CAPABILITY_ENQ_DURATION, UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_CAPABILITY_ENQ_TIMER"},        

    {UECC_TIMER_CONTXT_MOD_DEFAULT_DURATION, UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_CONTXT_MOD_TIMER"},
    {UECC_TIMER_ERAB_SETUP_DEFAULT_DURATION, UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_ERAB_SETUP_GUARD_TIMER"},
    {UECC_TIMER_ERAB_MODIFY_DEFAULT_DURATION, UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_ERAB_MODIFY_GUARD_TIMER"},
    {UECC_TIMER_ERAB_REL_CMD_DEFAULT_DURATION, UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_ERAB_RELEASE_CMD_GUARD_TIMER"},        
    {UECC_TIMER_ERAB_REL_IND_DEFAULT_DURATION, UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_ERAB_RELEASE_IND_GUARD_TIMER"},        
    {UECC_TIMER_RLF_W_FOR_REEST_TIMER_DEFAULT_DURATION,UECC_EV_W_FOR_REEST_TIMER_EXPIRY,
          RRC_TRUE, RRC_FALSE, (S8*)"UECC_RLF_W_FOR_REEST_TIMER"},
    {UECC_TIMER_RLF_UE_SUSPEND_TIMER_DEFAULT_DURATION,UECC_EV_TIMER_EXPIRY,
          RRC_TRUE, RRC_FALSE, (S8*)"UECC_RLF_UE_SUSPEND_GUARD_TIMER"},

/*REL 1.2:HANDOVER PROCEDURE START*/
    {UECC_TIMER_HO_PREP_DEFAULT_DURATION,       UECC_EV_HO_PREP_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_HO_PREP_TIMER"},
    {UECC_TIMER_HO_OVERALL_DEFAULT_DURATION,    UECC_EV_HO_OVERALL_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_HO_OVERALL_TIMER"},
    {UECC_TIMER_LL_GUARD_DEFAULT_DURATION,       UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_LL_GUARD_TIMER"},
    {UECC_TIMER_OFFSET_DEFAULT_DURATION, UECC_EV_INVALID_TIMER,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_OFFSET_TIMER"},
    {UECC_TIMER_MC_GUARD_DURATION, UECC_EV_TIMER_EXPIRY, 
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_MEAS_CONFIG_GUARD_TIMER"},
    {UECC_TIMER_TRG_HO_GUARD_DURATION, UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_TRG_HO_GUARD_TIMER"},
/*REL 1.2:HANDOVER PROCEDURE END*/
    {UECC_TIMER_CRE_GUARD_DURATION, UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_CRE_TIMER"},
    {UECC_TIMER_HO_PREP_DEFAULT_DURATION,       UECC_EV_HO_PREP_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_X2_HO_PREP_TIMER"},
    {UECC_TIMER_HO_OVERALL_DEFAULT_DURATION,    UECC_EV_HO_OVERALL_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_X2_HO_OVERALL_TIMER"},
    /* intra_cell_ho_timer */
    /* Bug 817 Fix Start */
    {UECC_TIMER_INTRA_CELL_HO_GUARD_DURATION, UECC_EV_S1U_END_MARKER_IND_TIMER_EXPIRY,
    /* Bug 817 Fix Stop */
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_INTRA_CELL_HO_GUARD_TIMER"},
    {UECC_TIMER_RRM_UE_RECONFIG_DEFAULT_DURATION,    UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_RRM_UE_RECONFIG_GUARD_TIMER"},
    /* SPR 4118 Start */
    {UECC_TIMER_INTEGRITY_FAILURE_DEFAULT_DURATION,    UECC_UE_INTEGRITY_FAILURE_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_INTEGRITY_FAILURE_TIMER"},
    {UECC_TIMER_CCO_GUARD_DEFAULT_DURATION, UECC_EV_CCO_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_CCO_GUARD_TIMER"},
    /* SPR 4118 Stop */
    {UECC_TIMER_RRM_COUNTER_CHECK_DEFAULT_DURATION,    UECC_EV_COUNTER_CHECK_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_RRM_COUNTER_CHECK_GUARD_TIMER"},          
    {UECC_TIMER_UE_INFO_DURATION, UECC_EV_UE_INFO_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_UE_INFO_TIMER"},
    {UECC_TIMER_STORE_UE_CONTEXT_DEFAULT_DURATION, UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_STORE_UE_CONTEXT_TIMER"},
    {UECC_TIMER_HEALTH_MONITOR_DEFAULT_DURATION, UECC_EV_TIMER_EXPIRY,
            RRC_FALSE, RRC_TRUE, (S8*)"UECC_TIMER_HEALTH_MONITOR_DEFAULT_DURATION"}
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    ,{UECC_TIMER_DC_PREP_DEFAULT_DURATION, UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_TIMER_DC_PREP_TIMER"}
    ,{UECC_TIMER_DC_BEARER_CHANGE_GUARD_DEFAULT_DURATION, UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_TIMER_DC_BEARER_CHANGE_GUARD_TIMER"}
    ,{UECC_TIMER_ERAB_RELEASE_ENDC_GUARD_TIMER_DEFAULT_DURATION, UECC_EV_TIMER_EXPIRY,
            RRC_TRUE, RRC_FALSE, (S8*)"UECC_ERAB_RELEASE_ENDC_GUARD_TIMER"}
/*NR_DC Code Change Stop*/
#endif

};

static uecc_ue_timer_oam_to_uecc_id_t uecc_ue_oam_to_uecc_id_table[] =
{
    {UECC_RRC_CONN_SETUP_TIMER,     UECC_UE_RCS_TIMER},
    {UECC_RRC_CONN_RECONFIG_TIMER,  UECC_UE_ICS_TIMER},
    {UECC_RRC_CONN_RELEASE_TIMER,   UECC_UE_RCR_TIMER},
    {UECC_RRC_MME_GUARD_TIMER,      UECC_UE_MME_GUARD_TIMER},
    {UECC_RRM_UE_CAPABILITY_ENQ_TIMER,      UECC_UE_CAPABILITY_ENQ_TIMER},

    {UECC_UE_CONTXT_MODIFY_TIMER,   UECC_UE_CONTXT_MOD_TIMER},
    {UECC_ERAB_SETUP_TIMER,         UECC_ERAB_SETUP_GUARD_TIMER},
    {UECC_ERAB_MODIFY_TIMER,        UECC_ERAB_MODIFY_GUARD_TIMER},
    {UECC_ERAB_RELEASE_CMD_TIMER,   UECC_ERAB_RELEASE_CMD_GUARD_TIMER },
    {UECC_ERAB_RELEASE_IND_TIMER,   UECC_ERAB_RELEASE_IND_GUARD_TIMER },
    {UECC_RLF_WAIT_FOR_REEST_TIMER, UECC_RLF_W_FOR_REEST_TIMER},
    {UECC_RLF_UE_SUSPEND_TIMER, UECC_RLF_UE_SUSPEND_GUARD_TIMER},

  /*handover*/
    {UECC_RRC_HO_PREP_TIMER,        UECC_UE_HO_PREP_TIMER},
    {UECC_RRC_HO_OVERALL_TIMER,     UECC_UE_HO_OVERALL_TIMER},
    {UECC_LL_GUARD_TIMER,           UECC_UE_LL_GUARD_TIMER},
    {UECC_RRC_OFFSET_TIMER,         UECC_UE_OFFSET_TIMER},
    {UECC_MEAS_CONFIG_GUARD_TIMER,  UECC_UE_MEAS_CONFIG_GUARD_TIMER},
    {UECC_RRC_TRG_HO_GUARD_TIMER,   UECC_UE_TRG_HO_GUARD_TIMER},
    {UECC_CRE_GUARD_TIMER,             UECC_UE_CRE_TIMER},
    /*x2_ho_timer*/
    {UECC_RRC_X2_HO_PREP_TIMER,           UECC_UE_X2_HO_PREP_TIMER},
    {UECC_RRC_X2_HO_OVERALL_TIMER,        UECC_UE_X2_HO_OVERALL_TIMER},
    {UECC_RRC_INTRA_CELL_HO_GUARD_TIMER,  UECC_UE_INTRA_CELL_HO_GUARD_TIMER},
    {UECC_RRM_UE_RECONFIG_TIMER,          UECC_RRM_UE_RECONFIG_GUARD_TIMER},
    /* SPR 4118 Start */
    {UECC_RRC_INTEGRITY_FAILURE_TIMER,     UECC_UE_INTEGRITY_FAILURE_TIMER},
    /* SPR 4118 Stop */
    {UECC_RRC_CCO_GUARD_TIMER,  UECC_UE_CCO_GUARD_TIMER},
    {UECC_RRC_COUNTER_CHECK_TIMER,        UECC_UE_COUNTER_CHECK_GUARD_TIMER},

    {UECC_RRM_UE_INFORMATION_REQ_TIMER,  UECC_UE_INFORMATION_GUARD_TIMER},
    {UECC_RRC_STORE_UE_CONTEXT_TIMER,  UECC_STORE_UE_CONTEXT_TIMER},
    {UECC_OAM_HEALTH_MONITOR_TIMER, UECC_HEALTH_MONITOR_TIMER},
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    {UECC_RRC_DC_PREP_TIMER,        UECC_DC_PREP_TIMER},
    /*OPTION3X Change Start*/
    {UECC_RRC_DC_BEARER_CHANGE_GUARD_TIMER,        UECC_DC_BEARER_CHANGE_GUARD_TIMER},
    /*OPTION3X Change Stop*/
    {UECC_RRC_ERAB_RELEASE_ENDC_GUARD_TIMER,        UECC_ERAB_RELEASE_ENDC_GUARD_TIMER},
/*NR_DC Code Change Stop*/
#endif
    {UECC_LAST_ID,                        UECC_LAST_ID}
};

/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/

/****************************************************************************
 * Function implementation
 ****************************************************************************/

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timers_load_default_durations
 *
 *   INPUT:         uecc_gb_context_t*      p_uecc_gb_context,
 *                  uecc_ue_global_timer_data_t *p_ue_gl_timer_data
 *
 *   OUTPUT:        None
 *
 *   RETURNS:       None
 *
 *   DESCRIPTION:   Loads default durations for all UECC UE timers
 *
 ****************************************************************************/
void uecc_ue_timers_load_default_durations(
        uecc_gb_context_t*      p_uecc_gb_context,
        uecc_ue_global_timer_data_t *p_ue_gl_timer_data
)
{
    U16 timer_index = RRC_NULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_ue_gl_timer_data);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_DETAILEDALL, "Unused variable p_uecc_gb_context=%p",
                p_uecc_gb_context);/*SPR 17777 +-*/
    for (timer_index = 0; timer_index < UECC_UE_TIMER_LAST; timer_index++)
    {
        p_ue_gl_timer_data->durations[timer_index] =
            uecc_ue_timers_descriptors[timer_index].default_duration;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timer_set_durations
 *
 *   INPUT:         uecc_gb_context_t*          p_uecc_gb_context,
 *                  U16                         oam_timer_id,
 *                  uecc_ue_global_timer_data_t *p_ue_gl_timer_data,
 *                  rrc_timer_duration_t        new_duration
 *
 *   OUTPUT:        None
 *
 *   RETURNS:       None
 *
 *   DESCRIPTION:   Sets new duration for specified UECC UE timer
 *
 ****************************************************************************/
void uecc_ue_timer_set_durations(
    uecc_gb_context_t*          p_uecc_gb_context,
    U16                         oam_timer_id,
    uecc_ue_global_timer_data_t *p_ue_gl_timer_data,
    rrc_timer_duration_t        new_duration
)
{
    U16 timer_index = RRC_NULL;
    U8 maxIndexVal = 0;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_ue_gl_timer_data);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_DETAILEDALL, "Unused variable p_uecc_gb_context=%p",
                p_uecc_gb_context);/*SPR 17777 +-*/
    if (0 == new_duration)
    {
        return;
    }

    timer_index = 0;
    /* Look for UECC timer id*/
    maxIndexVal = (U8)(sizeof(uecc_ue_oam_to_uecc_id_table)/
            sizeof(uecc_ue_timer_oam_to_uecc_id_t));
    while ((timer_index < maxIndexVal) &&
            (UECC_LAST_ID !=
           uecc_ue_oam_to_uecc_id_table[timer_index].oam_timer_id))
    {
        if (uecc_ue_oam_to_uecc_id_table[timer_index].oam_timer_id
                == oam_timer_id)
        {
            /* Store duration */
            p_ue_gl_timer_data->durations
            [
                uecc_ue_oam_to_uecc_id_table[timer_index].uecc_timer_id
            ] = new_duration;
            break;
        }
        timer_index++;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timer_start_with_duration
 *
 *   INPUT:         uecc_gb_context_t*      p_uecc_gb_context,
 *                  uecc_ue_timer_id_et     uecc_ue_timer_id,
 *                  rrc_timer_duration_t    duration,
 *                  uecc_ue_timer_data_t    *p_ue_timer_data,
 *                  rrc_ue_index_t          ue_index
 *
 *
 *   OUTPUT:        None
 *
 *   RETURNS:       RRC_SUCCESS/RRC_FAILURE
 *
 *   DESCRIPTION:   Starts UECC UE timer with duration specified by caller
 *
 ****************************************************************************/
rrc_return_t uecc_ue_timer_start_with_duration(
    uecc_gb_context_t*      p_uecc_gb_context,
    uecc_ue_timer_id_et     uecc_ue_timer_id,
    rrc_timer_duration_t    duration,
    uecc_ue_timer_data_t    *p_ue_timer_data,
    rrc_ue_index_t          ue_index
)
{
    rrc_return_t result = RRC_FAILURE;
    uecc_timer_buffer_t repeated_timer_buf;

    RRC_ASSERT(PNULL != p_ue_timer_data);
    RRC_ASSERT(UECC_UE_TIMER_LAST > uecc_ue_timer_id);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);


    if(uecc_ue_timer_id < UECC_UE_TIMER_LAST)
    {
        if ( PNULL != p_ue_timer_data->timers_ids[uecc_ue_timer_id] )
        {
            /* Already started */
            return result;
        }

        RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                p_uecc_gb_context->facility_name, RRC_BRIEF,
                "[%s] Start timer for %u ms.",
                uecc_ue_timers_descriptors[uecc_ue_timer_id].str_id,
                duration);

        /* Start the Procedural timer*/
        repeated_timer_buf.type = UECC_TIMER_TYPE_RRC_PROCEDURAL;
        repeated_timer_buf.id = uecc_ue_timer_id;
        repeated_timer_buf.auto_delete =
            uecc_ue_timers_descriptors[uecc_ue_timer_id].auto_delete;
        repeated_timer_buf.ue_index = ue_index;
        repeated_timer_buf.fsm_event_id =
           (uecc_fsm_event_et)uecc_ue_timers_descriptors[uecc_ue_timer_id].fsm_event_id;

        p_ue_timer_data->timers_ids[uecc_ue_timer_id] =
            rrc_start_timer(
                    duration,
                    &repeated_timer_buf,
                    sizeof(uecc_timer_buffer_t),
                    uecc_ue_timers_descriptors[uecc_ue_timer_id].is_repeated);

        if ( PNULL == p_ue_timer_data->timers_ids[uecc_ue_timer_id] )
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name, RRC_FATAL, "[%s] Starting error",
                    uecc_ue_timers_descriptors[uecc_ue_timer_id].str_id);
        }
        else
        {
            result = RRC_SUCCESS;
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);

    return result;
}

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timer_start
 *
 *   INPUT:         uecc_gb_context_t*          p_uecc_gb_context,
 *                  uecc_ue_timer_id_et         uecc_ue_timer_id,
 *                  uecc_ue_global_timer_data_t *p_ue_gl_timer_data,
 *                  uecc_ue_timer_data_t        *p_ue_timer_data,
 *                  rrc_ue_index_t              ue_index
 *
 *   OUTPUT:        None
 *
 *   RETURNS:       RRC_SUCCESS/RRC_FAILURE
 *
 *   DESCRIPTION:   Starts UECC UE timer with duration specified by OAMH
 *
 ****************************************************************************/
rrc_return_t uecc_ue_timer_start(
    uecc_gb_context_t*          p_uecc_gb_context,
    uecc_ue_timer_id_et         uecc_ue_timer_id,
    uecc_ue_global_timer_data_t *p_ue_gl_timer_data,
    uecc_ue_timer_data_t        *p_ue_timer_data,
    rrc_ue_index_t              ue_index
)
{
    rrc_return_t result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_ue_gl_timer_data);
    RRC_ASSERT(UECC_UE_TIMER_LAST > uecc_ue_timer_id);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
 
    if (uecc_ue_timer_id < UECC_UE_TIMER_LAST)
    {
        /* NBIOT TIMER FIX */
        result = uecc_ue_timer_start_with_duration(
                    p_uecc_gb_context, 
                    uecc_ue_timer_id,
                    p_ue_gl_timer_data->durations[uecc_ue_timer_id],
                    p_ue_timer_data,
                    ue_index);

    }
    /* NBIOT TIMER FIX */

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);

    return result;
}

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timer_stop
 *
 *   INPUT:         uecc_gb_context_t*          p_uecc_gb_context,
 *                  uecc_ue_timer_id_et         uecc_ue_timer_id,
 *                  uecc_ue_timer_data_t        *p_ue_timer_data
 *
 *
 *   OUTPUT:        None
 *
 *   RETURNS:       None
 *
 *   DESCRIPTION:   Stop UECC UE timer
 *
 ****************************************************************************/
void uecc_ue_timer_stop(
    uecc_gb_context_t*          p_uecc_gb_context,
    uecc_ue_timer_id_et         uecc_ue_timer_id,
    uecc_ue_timer_data_t        *p_ue_timer_data
)
{
    RRC_ASSERT(PNULL != p_ue_timer_data);
    RRC_ASSERT(UECC_UE_TIMER_LAST > uecc_ue_timer_id);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    /* Stop procedural timer */
    if (uecc_ue_timer_id < UECC_UE_TIMER_LAST)
    {
        if (PNULL != p_ue_timer_data->timers_ids[uecc_ue_timer_id])
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                p_uecc_gb_context->facility_name, RRC_BRIEF, "[%s] Stop timer",
                uecc_ue_timers_descriptors[uecc_ue_timer_id].str_id);
            rrc_stop_timer(p_ue_timer_data->timers_ids[uecc_ue_timer_id]);
            p_ue_timer_data->timers_ids[uecc_ue_timer_id] = PNULL;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timer_mark_as_expired
 *
 *   INPUT:         uecc_gb_context_t*          p_uecc_gb_context,
 *                  uecc_ue_timer_id_et         uecc_ue_timer_id,
 *                  uecc_ue_timer_data_t        *p_ue_timer_data
 *
 *
 *   OUTPUT:        None
 *
 *   RETURNS:       None
 *
 *   DESCRIPTION:   Marks UECC UE timer as expired
 *
 ****************************************************************************/
void uecc_ue_timer_mark_as_expired(
    uecc_gb_context_t*          p_uecc_gb_context,
    uecc_ue_timer_id_et         uecc_ue_timer_id,
    uecc_ue_timer_data_t        *p_ue_timer_data
)
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_ue_timer_data);
    RRC_ASSERT(UECC_UE_TIMER_LAST > uecc_ue_timer_id);
  
    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_DETAILEDALL, "Unused variable p_uecc_gb_context=%p",
                p_uecc_gb_context);/*SPR 17777 +-*/
    if(uecc_ue_timer_id < UECC_UE_TIMER_LAST)
    {
        /* Mark procedural timer as expired*/
        p_ue_timer_data->timers_ids[uecc_ue_timer_id] = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timers_stop_all
 *
 *   INPUT:         uecc_gb_context_t*          p_uecc_gb_context,
 *                  uecc_ue_timer_data_t        *p_ue_timer_data
 *
 *   OUTPUT:        None
 *
 *   RETURNS:       None
 *
 *   DESCRIPTION:   Stops all UECC UE timer
 *
 ****************************************************************************/
void uecc_ue_timers_stop_all(
        uecc_gb_context_t*          p_uecc_gb_context, 
        uecc_ue_timer_data_t        *p_ue_timer_data
)
{
    U16 timer_index = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_timer_data);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    for (timer_index = 0; timer_index < UECC_UE_TIMER_LAST; timer_index++)
    {
        uecc_ue_timer_stop(p_uecc_gb_context, (uecc_ue_timer_id_et)timer_index, p_ue_timer_data);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timers_data_init
 *
 *   INPUT:         uecc_gb_context_t*          p_uecc_gb_context,
 *                  uecc_ue_timer_data_t    *p_ue_timer_data
 *
 *   OUTPUT:        None
 *
 *   RETURNS:       None
 *
 *   DESCRIPTION:   Initializes UECC UE timers data structure
 *
 ****************************************************************************/
void uecc_ue_timers_data_init(
        uecc_gb_context_t*          p_uecc_gb_context,
        uecc_ue_timer_data_t    *p_ue_timer_data
)
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_ue_timer_data);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_DETAILEDALL, "Unused variable p_uecc_gb_context=%p",
                p_uecc_gb_context);/*SPR 17777 +-*/
    memset_wrapper(p_ue_timer_data, 0, sizeof(uecc_ue_timer_data_t));

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timers_print_durations
 *
 *   INPUT:         struct _uecc_gb_context_t *p_uecc_gb_context,
 *                  uecc_ue_global_timer_data_t *p_ue_gl_timer_data
 *
 *   OUTPUT:        None
 *
 *   RETURNS:       None
 *
 *   DESCRIPTION:   Prints current durations for all UECC UE timers
 *
 ****************************************************************************/
void uecc_ue_timers_print_durations(
     struct _uecc_gb_context_t *p_uecc_gb_context,
     uecc_ue_global_timer_data_t *p_ue_gl_timer_data
)
{
    U16 timer_index = RRC_NULL;

    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_ue_gl_timer_data);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    for (timer_index = 0; timer_index < UECC_UE_TIMER_LAST; timer_index++)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_INFO, "[OAMH] [%s] duration [%u]",
                uecc_ue_timers_descriptors[timer_index].str_id,
                p_ue_gl_timer_data->durations[timer_index]);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/****************************************************************************
 *   FUNCTION NAME: start_uecc_health_monitor_timer
 *
 *   DESCRIPTION:
 *       Starts UECC health monitor timer if duration specified by caller
 *       greater than zero
 *
 *   RETURNS: None
 ****************************************************************************/
void start_uecc_health_monitor_timer(
    uecc_gb_context_t*      p_uecc_gb_context,
    uecc_ue_timer_id_et     uecc_ue_timer_id,
    rrc_timer_duration_t    duration
)
{
    uecc_timer_buffer_t repeated_timer_buf;
    rrc_timer_t timer = PNULL;

    RRC_ASSERT(UECC_UE_TIMER_LAST > uecc_ue_timer_id);

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    if(duration == 0)
    {
        return;
    }
    
    if(uecc_ue_timer_id < UECC_UE_TIMER_LAST)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                p_uecc_gb_context->facility_name, RRC_BRIEF,
                "[%s] Start timer for %u ms.",
                uecc_ue_timers_descriptors[uecc_ue_timer_id].str_id,
                duration);

        /* Start the Procedural timer*/
        repeated_timer_buf.type = UECC_TIMER_TYPE_RRC_PROCEDURAL;
        repeated_timer_buf.id = uecc_ue_timer_id;
        repeated_timer_buf.auto_delete = RRC_FALSE;
        repeated_timer_buf.fsm_event_id =
            (uecc_fsm_event_et)uecc_ue_timers_descriptors[uecc_ue_timer_id].fsm_event_id; 
    }

    timer =
        rrc_start_timer(
                duration,
                &repeated_timer_buf,
                sizeof(uecc_timer_buffer_t),
                RRC_TRUE);
    if ( PNULL == timer )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name, RRC_FATAL, "[%s] Starting error");
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);

    return;
}
