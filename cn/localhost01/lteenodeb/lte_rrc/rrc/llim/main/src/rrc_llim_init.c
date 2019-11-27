/******************************************************************************
*
*   FILE NAME:
*       rrc_llim_init.c
*
*   DESCRIPTION:
*       This is the main file of the LLIM Initialization module. This module
*       provides initialization handler for LLIM CSPL entity.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   31 Mar 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "rrc_llim_logger.h"
#include "rrc_common_utils.h"
#include "rrc_llim_global_ctx.h"
#include "rrc_llim_timers.h"
#include "rrc_handlers.h"
/* Default duration of LLIM timers in ms */
#define RRC_LLIM_OAMH_INIT_IND_TIMER_DEFAULT_DURATION       1000
#define RRC_LLIM_CELL_CFG_TIMER_DEFAULT_DURATION            3000
#define RRC_LLIM_CELL_RECFG_TIMER_DEFAULT_DURATION          2000
#define RRC_LLIM_CREATE_UE_TIMER_DEFAULT_DURATION           3000
#define RRC_LLIM_DELETE_UE_TIMER_DEFAULT_DURATION           3000
#define RRC_LLIM_RECONFIG_UE_TIMER_DEFAULT_DURATION         3000
#define RRC_LLIM_CONFIG_UE_SECURITY_TIMER_DEFAULT_DURATION  3000
#define RRC_LLIM_RE_ESTABLISH_UE_TIMER_DEFAULT_DURATION     2000
#define RRC_LLIM_RECFG_SECURITY_UE_TIMER_DEFAULT_DURATION   2000
#define RRC_LLIM_RESUME_UE_TIMER_DEFAULT_DURATION           2000
#define RRC_LLIM_SUSPEND_UE_TIMER_DEFAULT_DURATION          2000
#define RRC_LLIM_CHANGE_CRNTI_TIMER_DEFAULT_DURATION        2000
#define RRC_LLIM_CELL_START_TIMER_DEFAULT_DURATION          3000
#define RRC_LLIM_CELL_STOP_TIMER_DEFAULT_DURATION           3000
#define RRC_LLIM_CELL_DELETE_TIMER_DEFAULT_DURATION         3000


#ifdef RRC_UNIT_TEST_FRAMEWORK
rrc_llim_gl_ctx_t *p_rrc_llim_gl_ctx = PNULL;
#endif

/******************************************************************************
*   FUNCTION NAME: rrc_llim_init_gl_ctx
*   INPUT        : rrc_llim_gl_ctx_t *p_rrc_llim_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function initialises LLIM global context with default values
*
******************************************************************************/
void rrc_llim_init_gl_ctx
(
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t *p_rrc_llim_gl_ctx
)
{
    RRC_LLIM_UT_TRACE_ENTER();

    memset_wrapper(p_rrc_llim_gl_ctx, 0, sizeof(rrc_llim_gl_ctx_t));

    /* Init timers */
    p_rrc_llim_gl_ctx->oamh_init_ind_duration =
        RRC_LLIM_OAMH_INIT_IND_TIMER_DEFAULT_DURATION;

    p_rrc_llim_gl_ctx->cell_cfg_duration =
        RRC_LLIM_CELL_CFG_TIMER_DEFAULT_DURATION;
    p_rrc_llim_gl_ctx->create_ue_duration =
        RRC_LLIM_CREATE_UE_TIMER_DEFAULT_DURATION;
    p_rrc_llim_gl_ctx->delete_ue_duration =
        RRC_LLIM_DELETE_UE_TIMER_DEFAULT_DURATION;
    p_rrc_llim_gl_ctx->reconfig_ue_duration =
        RRC_LLIM_RECONFIG_UE_TIMER_DEFAULT_DURATION;
    p_rrc_llim_gl_ctx->del_rcfg_ue_duration =
        RRC_LLIM_RECONFIG_UE_TIMER_DEFAULT_DURATION;
    p_rrc_llim_gl_ctx->config_ue_security_duration =
        RRC_LLIM_CONFIG_UE_SECURITY_TIMER_DEFAULT_DURATION;
    p_rrc_llim_gl_ctx->cell_recfg_duration = 
        RRC_LLIM_CELL_RECFG_TIMER_DEFAULT_DURATION;
    p_rrc_llim_gl_ctx->suspend_ue_duration = 
        RRC_LLIM_SUSPEND_UE_TIMER_DEFAULT_DURATION;        
    p_rrc_llim_gl_ctx->reestablish_ue_duration = 
        RRC_LLIM_RE_ESTABLISH_UE_TIMER_DEFAULT_DURATION;
    p_rrc_llim_gl_ctx->reconfig_security_ue_duration = 
        RRC_LLIM_RECFG_SECURITY_UE_TIMER_DEFAULT_DURATION;
    p_rrc_llim_gl_ctx->resume_ue_duration = 
        RRC_LLIM_RESUME_UE_TIMER_DEFAULT_DURATION;
    p_rrc_llim_gl_ctx->change_crnti_duration =
        RRC_LLIM_CHANGE_CRNTI_TIMER_DEFAULT_DURATION;

    /* Cell Reconfiguration Phase-2 related timerS */
    p_rrc_llim_gl_ctx->cell_start_duration =
        RRC_LLIM_CELL_START_TIMER_DEFAULT_DURATION;
    p_rrc_llim_gl_ctx->cell_stop_duration =
        RRC_LLIM_CELL_STOP_TIMER_DEFAULT_DURATION;
    p_rrc_llim_gl_ctx->cell_delete_duration =
        RRC_LLIM_CELL_DELETE_TIMER_DEFAULT_DURATION;

    /* Init cell DB */
    /* Already done in memset_wrapper */

    /* Init UE DB */
    memset_wrapper(&(p_rrc_llim_gl_ctx->max_num_supported_ue),
            RRC_NULL, sizeof(U16)*MAX_NUM_CELLS);
    
    p_rrc_llim_gl_ctx->p_p_uecc_ctx = PNULL;
    p_rrc_llim_gl_ctx->p_p_cell_ctx = PNULL;
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

    RRC_LLIM_UT_TRACE_EXIT();
}


/******************************************************************************
*   FUNCTION NAME: rrc_llim_init
*   INPUT        : void *p_rrc_llim_init_data
*   OUTPUT       : None
*   RETURNS      : Buffer that will be passed in rrc_llmd_process_msg
*                   and in rrc_llmd_process_timer_msg callbacks
*   DESCRIPTION  : This is the main entry point of LLIM module - CSPL callback. It handles
*                  initialization of LLIM module.
*
******************************************************************************/
void* rrc_llim_init
(
    void *p_rrc_llim_init_data /* Pointer to RRC stack initialization data */
)
{
#ifndef RRC_UNIT_TEST_FRAMEWORK
    rrc_llim_gl_ctx_t *p_rrc_llim_gl_ctx = PNULL;
#endif

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_LLIM_TRACE(RRC_DETAILED, 
            "Unused variable in fsm llim_init_data=%p", p_rrc_llim_init_data);

    RRC_LLIM_TRACE(RRC_DETAILED, "Init.");

    /* CSC review comments YR_1 */
    p_rrc_llim_gl_ctx = (rrc_llim_gl_ctx_t *)rrc_mem_get(sizeof
        (rrc_llim_gl_ctx_t));

    if(PNULL == p_rrc_llim_gl_ctx)
    {
        RRC_LLIM_TRACE(RRC_FATAL, "rrc_llim_init: unable to Allocate Memory "
            "for LLIM global context!");
        return PNULL;
    }
    else
    {
        rrc_llim_timer_buf_t repeated_timer_buf;
        repeated_timer_buf.auto_delete = RRC_FALSE;
        repeated_timer_buf.target_submodule = LLIM_SUBMODULE_OAMH;

        /* Init LLIM global context */
        rrc_llim_init_gl_ctx(p_rrc_llim_gl_ctx);

        RRC_LLIM_TRACE(RRC_BRIEF,
            "Start timer [LLIM_OAMH_INIT_IND] %u ms.",
            p_rrc_llim_gl_ctx->oamh_init_ind_duration);

        /* Start repeated timer for sending LLIM_OAMH_INIT_IND */
        p_rrc_llim_gl_ctx->oamh_init_ind_timer =
                    rrc_start_timer(p_rrc_llim_gl_ctx->oamh_init_ind_duration,
                                    &repeated_timer_buf,
                                    sizeof(repeated_timer_buf),
                                    RRC_TRUE);
    }

    RRC_LLIM_UT_TRACE_EXIT();

    return p_rrc_llim_gl_ctx;
}
