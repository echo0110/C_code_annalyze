/******************************************************************************
*
*   FILE NAME:
*       csc_cell_m_fsm.h
*
*   DESCRIPTION:
*       This header file contains declaration for Cell manager FSM
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*   06 June 2009    VasylS      ---------       Cleanup support
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_CELL_M_FSM_H_
#define _CSC_CELL_M_FSM_H_

#include "csc_global_ctx.h"
#include "csc_cell_ctx.h"
#include "csc_cell_m_fsm_states.h"
#include "csc_logger.h"
#include "rrc_rrm_intf.h"

/* keep in sync with cell_m_fsm_tbl! */
typedef enum
{
    CELL_M_EVENT_RRM_CONFIG_CELL_REQ,
    CELL_M_EVENT_LLIM_CONFIG_CELL_RESP,
    CELL_M_EVENT_CONFIG_CELL_TIMER_EXPIRED,
    CELL_M_EVENT_LLIM_DELETE_CELL_RESP,
    CELL_M_EVENT_DELETE_CELL_TIMER_EXPIRED,
    CELL_M_EVENT_OAMH_CLEANUP_REQ,
    CELL_M_EVENT_SFN_SYNC_TIMER_EXPIRED,
    CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP,
    CELL_M_EVENT_RRM_DELETE_CELL_REQ,
    CELL_M_EVENT_RRM_RECONFIG_CELL_REQ, 
    CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP,   
    CELL_M_EVENT_RECONFIG_CELL_TIMER_EXPIRED,
    CELL_M_EVENT_ENB_MGR_PWS_REQ,
    CELL_M_EVENT_LLIM_PWS_RESP,
    CELL_M_EVENT_PWS_TIMER_EXPIRED,
    CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ,
    CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP,
    CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP,
    CELL_M_EVENT_UPDATED_PWS_TIMER_EXPIRED,
    CELL_M_EVENT_ENB_MGR_KILL_REQ,
    CELL_M_EVENT_KILL_TIMER_EXPIRED,
    CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND, 
    CELL_M_EVENT_RRM_CELL_START_REQ,
    CELL_M_EVENT_RRM_CELL_STOP_REQ,
    CELL_M_EVENT_LLIM_CELL_START_RESP,
    CELL_M_EVENT_LLIM_CELL_STOP_RESP,
    CELL_M_EVENT_CELL_START_TIMER_EXPIRED,
    CELL_M_EVENT_CELL_STOP_TIMER_EXPIRED,
    /* CDMA2000_CSFB_HO start */ 
    CELL_M_EVENT_RAND_REGENERATE_TIMER_EXPIRED,
    /* CDMA2000_CSFB_HO stop */ 

    N_CELL_M_EVENTS
} cell_m_fsm_event_et;

extern const S8 * CELL_M_FSM_STATES_NAMES[];
extern const S8 * CELL_M_FSM_EVENTS_NAMES[];

#define CELL_RECONFIG_OFFSET    50
#define PWS_OFFSET              5
#define CSC_SFN_TICK            10
#define RRC_CELL_M_FSM_NAME "Cell Manager FSM"
#define CSC_PWS_SFN_OFFSET      20
#define RRC_MAX_SFN_VALUE           1023

#define CELL_M_FSM_INIT(p_cell_ctx) \
    do \
    { \
        p_cell_ctx->current_state = CELL_M_STATE_IDLE; \
        RRC_CSC_FSM_INIT_TRACE(RRC_CELL_M_FSM_NAME, \
            CELL_M_FSM_STATES_NAMES[CELL_M_STATE_IDLE]); \
    } \
    while (0)

#define CELL_M_FSM_GET_CURRENT_STATE(p_cell_ctx)      (p_cell_ctx->current_state)

#define CELL_M_FSM_SET_STATE(p_cell_ctx, new_state) \
    do \
    { \
        p_cell_ctx->current_state = new_state; \
        RRC_CSC_FSM_SET_STATE_TRACE(RRC_CELL_M_FSM_NAME, \
            CELL_M_FSM_STATES_NAMES[new_state]); \
    } \
    while (0)

void cell_m_fsm_process_event
(
    cell_m_fsm_event_et event,
    void                *param1,
    void                *param2,
    csc_cell_ctx_t      *p_cell_ctx,
    csc_gl_ctx_t        *p_csc_gl_ctx
);
void csc_send_cell_reconfig_msg
(
  csc_gl_ctx_t    *p_csc_gl_ctx,   /* Pointer to the CSC global context data */
  csc_cell_ctx_t  *p_cell_ctx    /* cell context */
);

U32 csc_get_mod_period
(
sib_type_2_Info_t *p_sib_type_2_Info
);



rrc_return_et resume_high_priority_thread
(
 csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
 rrc_sfn_t          sfn,
 rrc_sf_t           sf,
 rrc_timestamp_t    timestamp
 );

rrc_return_et pack_and_send_new_sib8_segments
(
 csc_cell_ctx_t  *p_cell_ctx,    /* cell context */
 csc_gl_ctx_t    *p_csc_gl_ctx
 );





#endif /* _CSC_CELL_M_FSM_H_ */

