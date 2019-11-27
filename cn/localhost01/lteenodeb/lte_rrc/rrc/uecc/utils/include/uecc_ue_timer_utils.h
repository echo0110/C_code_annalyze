/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ***************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_ue_timer_utils.h,v 1.16 2010/10/13 07:03:53 gur18569 Exp $
 ***************************************************************************
 *
 *  File Description : UECC UE Timers engine definitions
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * Jan 14, 2010   grag -   initial creation
 * $Log: uecc_ue_timer_utils.h,v $
 * Revision 1.16  2010/10/13 07:03:53  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.3.4.6  2010/09/13 13:04:12  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.3.4.5  2010/08/10 10:03:59  gur18569
 * incorported review comments for ho
 *
 * Revision 1.3.4.4  2010/08/05 08:08:56  gur15697
 * merging round 2
 *
 * Revision 1.3.4.3  2010/07/28 06:52:33  gur15697
 * warnings removed
 *
 * Revision 1.3.4.2  2010/07/21 08:41:14  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.3.4.1  2010/06/24 12:54:46  gur22047
 * internal source and s1ap code
 *
 * Revision 1.3  2010/04/05 07:28:04  gur18569
 * moved ue capability timer below mme guard timer
 *
 * Revision 1.2  2010/03/24 10:42:40  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.1.2.1  2010/01/15 10:09:04  ukr16032
 * UECC FSM - Multi timers functionality has been added
 *
 *
 ***************************************************************************/

#ifndef UECC_UE_TIMER_UTILS_H_
#define UECC_UE_TIMER_UTILS_H_

/****************************************************************************
 * Inculdes
 ****************************************************************************/
#include "rrc_defines.h"
#include "uecc_fsm_common.h"

struct _uecc_gb_context_t;
/******************************************************************************
 * Global definitions
 *****************************************************************************/
typedef enum {
    UECC_TIMER_TYPE_CONTROL,
    UECC_TIMER_TYPE_RRC_PROCEDURAL,
} uecc_timer_type_et;

typedef enum
{
    UECC_UE_RCS_TIMER,
    UECC_UE_ICS_TIMER,
    UECC_UE_RCR_TIMER,
    UECC_UE_MME_GUARD_TIMER,
    UECC_UE_CAPABILITY_ENQ_TIMER,
    UECC_UE_CONTXT_MOD_TIMER,

    /* This will be changed to ERAB_GURAD TIMER */
    UECC_ERAB_SETUP_GUARD_TIMER,/* 6 */
    UECC_ERAB_MODIFY_GUARD_TIMER,/* 7 */
    UECC_ERAB_RELEASE_CMD_GUARD_TIMER,/* 8 */
    UECC_ERAB_RELEASE_IND_GUARD_TIMER,/* 9 */
    UECC_RLF_W_FOR_REEST_TIMER,/* 10 */
    UECC_RLF_UE_SUSPEND_GUARD_TIMER,/* 11 */

/*REL 1.2:HANDOVER PROCEDURE START*/
    UECC_UE_HO_PREP_TIMER,
    UECC_UE_HO_OVERALL_TIMER,
    UECC_UE_LL_GUARD_TIMER,
    UECC_UE_OFFSET_TIMER,
    UECC_UE_MEAS_CONFIG_GUARD_TIMER,
    UECC_UE_TRG_HO_GUARD_TIMER,
/*REL 1.2:HANDOVER PROCEDURE END*/
    UECC_UE_CRE_TIMER,

    /*x2_ho_timer*/
    UECC_UE_X2_HO_PREP_TIMER,
    UECC_UE_X2_HO_OVERALL_TIMER,
    UECC_UE_INTRA_CELL_HO_GUARD_TIMER,
    UECC_RRM_UE_RECONFIG_GUARD_TIMER,
    /* SPR 4118 Start */
    UECC_UE_INTEGRITY_FAILURE_TIMER,
    /* SPR 4118 Stop */
    UECC_UE_CCO_GUARD_TIMER,

    UECC_UE_COUNTER_CHECK_GUARD_TIMER,
    UECC_UE_INFORMATION_GUARD_TIMER,
    UECC_STORE_UE_CONTEXT_TIMER,
    UECC_HEALTH_MONITOR_TIMER,


#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    UECC_DC_PREP_TIMER,
    UECC_DC_BEARER_CHANGE_GUARD_TIMER,
    UECC_ERAB_RELEASE_ENDC_GUARD_TIMER,
/*NR_DC Code Change Stop*/
#endif
    UECC_UE_TIMER_LAST
} uecc_ue_timer_id_et;

/****************************************************************************
 * Global Types definitions
 ****************************************************************************/

typedef struct {
    uecc_timer_type_et     type;
    uecc_ue_timer_id_et    id;
    rrc_bool_t             auto_delete;
    rrc_ue_index_t         ue_index;
    uecc_fsm_event_et      fsm_event_id;
}uecc_timer_buffer_t;

typedef struct
{
    rrc_timer_duration_t    durations[UECC_UE_TIMER_LAST];
} uecc_ue_global_timer_data_t;

typedef struct
{
    rrc_timer_t    timers_ids[UECC_UE_TIMER_LAST];
} uecc_ue_timer_data_t;

typedef const struct
{
    rrc_timer_duration_t    default_duration;
    U16                     fsm_event_id;
    rrc_bool_t              auto_delete;
    rrc_bool_t              is_repeated;
    S8*                   str_id;
} uecc_ue_timer_descriptor_t;

extern uecc_ue_timer_descriptor_t uecc_ue_timers_descriptors[UECC_UE_TIMER_LAST];


/****************************************************************************
 * Global Functions Prototypes
 ****************************************************************************/

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timers_load_default_durations
 *
 *   DESCRIPTION:
 *       Loads default durations for all UECC UE timers
 *
 *   RETURNS:
 *       None
 ****************************************************************************/
void uecc_ue_timers_load_default_durations(
   struct _uecc_gb_context_t       *p_uecc_gb_context, 
       uecc_ue_global_timer_data_t *p_ue_gl_timer_data
);

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timer_set_durations
 *
 *   DESCRIPTION:
 *       Sets new duration for specified UECC UE timer
 *
 *   RETURNS:
 *       None
 ****************************************************************************/
void uecc_ue_timer_set_durations(
    struct _uecc_gb_context_t       *p_uecc_gb_context,
    U16                         oam_timer_id,
    uecc_ue_global_timer_data_t *p_ue_gl_timer_data,
    rrc_timer_duration_t        new_duration
);

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timer_start_with_duration
 *
 *   DESCRIPTION:
 *       Starts UECC UE timer with duration specified by caller
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 ****************************************************************************/
rrc_return_t uecc_ue_timer_start_with_duration(
    struct _uecc_gb_context_t       *p_uecc_gb_context,
    uecc_ue_timer_id_et     uecc_ue_timer_id,
    rrc_timer_duration_t    duration,
    uecc_ue_timer_data_t    *p_ue_timer_data,
    rrc_ue_index_t          ue_index
);

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timer_start
 *
 *   DESCRIPTION:
 *       Starts UECC UE timer with duration specified by OAMH
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 ****************************************************************************/
rrc_return_t uecc_ue_timer_start(
    struct _uecc_gb_context_t           *p_uecc_gb_context, 
    uecc_ue_timer_id_et         uecc_ue_timer_id,
    uecc_ue_global_timer_data_t *p_ue_gl_timer_data,
    uecc_ue_timer_data_t        *p_ue_timer_data,
    rrc_ue_index_t              ue_index
);

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timer_stop
 *
 *   DESCRIPTION:
 *       Stops UECC UE timer
 *
 *   RETURNS:
 *       None
 ****************************************************************************/
void uecc_ue_timer_stop(
    struct _uecc_gb_context_t           *p_uecc_gb_context,
    uecc_ue_timer_id_et         uecc_ue_timer_id,
    uecc_ue_timer_data_t        *p_ue_timer_data
);

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timer_mark_as_expired
 *
 *   DESCRIPTION:
 *       Marks UECC UE timer as expired
 *
 *   RETURNS:
 *       None
 ****************************************************************************/
void uecc_ue_timer_mark_as_expired(
    struct _uecc_gb_context_t           *p_uecc_gb_context,
    uecc_ue_timer_id_et         uecc_ue_timer_id,
    uecc_ue_timer_data_t        *p_ue_timer_data
);

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timers_stop_all
 *
 *   DESCRIPTION:
 *       Stops all UECC UE timer
 *
 *   RETURNS:
 *       None
 ****************************************************************************/
void uecc_ue_timers_stop_all(
    struct _uecc_gb_context_t       *p_uecc_gb_context,
    uecc_ue_timer_data_t        *p_ue_timer_data
);

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timers_data_init
 *
 *   DESCRIPTION:
 *       Initializes UECC UE timers data structure
 *
 *   RETURNS:
 *       None
 ****************************************************************************/
void uecc_ue_timers_data_init(
        struct _uecc_gb_context_t       *p_uecc_gb_context,
        uecc_ue_timer_data_t    *p_ue_timer_data
);

/****************************************************************************
 *   FUNCTION NAME: uecc_ue_timers_print_durations
 *
 *   DESCRIPTION:
 *       Prints current durations for all UECC UE timers
 *
 *   RETURNS:
 *       None
 ****************************************************************************/
void uecc_ue_timers_print_durations(
    struct  _uecc_gb_context_t *p_uecc_gb_context,
    uecc_ue_global_timer_data_t *p_ue_gl_timer_data
);


#endif /* UECC_UE_TIMER_UTILS_H_ */
