/******************************************************************************
*
*   FILE NAME:
*       rrc_llim_cemh_fsm.h
*
*   DESCRIPTION:
*       This header file contains declaration for CEMH FSM
*
*   DATE            AUTHOR      REFERENCE       REASON
*   14 May 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_LLIM_CEMH_FSM_H_
#define _RRC_LLIM_CEMH_FSM_H_

#include "rrc_llim_global_ctx.h"
#include "rrc_llim_timers.h"
#include "rrc_llim_cell_ctx.h"
#include "rrc_cemh_fsm_states.h"
#include "rrc_llim_logger.h"

extern const S8 * CEMH_FSM_STATES_NAMES[];
extern const S8 * CEMH_FSM_STATES_EVENTS[];

#define RRC_CEMH_FSM_NAME "CEMH_FSM"

#define CEMH_FSM_INIT(p_cell_ctx) \
    do \
    { \
        p_cell_ctx->current_state = CEMH_STATE_IDLE; \
        RRC_LLIM_FSM_INIT_TRACE(RRC_CEMH_FSM_NAME, \
            CEMH_FSM_STATES_NAMES[CEMH_STATE_IDLE]); \
    } \
    while (0)

#define CEMH_FSM_GET_CURRENT_STATE(p_cell_ctx)      (p_cell_ctx->current_state)

#define CEMH_FSM_SET_STATE(p_cell_ctx, new_state) \
    do \
    { \
        p_cell_ctx->current_state = new_state; \
        RRC_LLIM_FSM_SET_STATE_TRACE(RRC_CEMH_FSM_NAME, \
            CEMH_FSM_STATES_NAMES[new_state]); \
    } \
    while (0)

void cemh_fsm_process_event
(
    cemh_fsm_event_et   event,
    void                *param1,
    void                *param2,
    rrc_llim_cell_ctx_t *p_cell_ctx,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

#ifdef LTE_EMBMS_SUPPORTED
m2ap_mcch_related_bcch_info_t* get_m2ap_mcch_related_bcch_info(
    sib_type_13_info_t *sib_type_13_info_p,
     U8     area_id);
/* SPR 18672 Fix Start */
m2ap_reserved_mcch_related_bcch_info_t* get_m2ap_reserved_mcch_related_bcch_info(
        m2ap_reserved_mcch_related_bcch_info_list_t *m2ap_reserved_mcch_related_bcch_info_list_p,
        U8     area_id);
/* SPR 18672 Fix End */
#endif

#endif /* _RRC_LLIM_CEMH_FSM_H_ */

