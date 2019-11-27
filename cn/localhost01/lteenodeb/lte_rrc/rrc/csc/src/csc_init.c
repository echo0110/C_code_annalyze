/******************************************************************************
*
*   FILE NAME:
*       csc_init.c
*
*   DESCRIPTION:
*       This is the main file of the CSC Initialization module. This module
*       provides initialization handler for CSC entity.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*   06 June 2009    VasylS      ---------       Cleanup support
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "rrc_common_utils.h"
#include "csc_logger.h"
#include "csc_global_ctx.h"
#include "csc_timers.h"
#include "rrc_handlers.h"
#include "csc_cell_ctx.h"

/* Default duration of CSC timers in ms */
#define RRC_CSC_OAMH_INIT_IND_DURATION  1000
#define RRC_CSC_CELL_CFG_DURATION       3000
#define RRC_CSC_CELL_RECFG_DURATION     3000
#define RRC_CSC_CELL_DEL_DURATION       3000
#define RRC_CSC_PWS_DURATION            3000
#define RRC_CSC_KILL_DURATION           3000
#define RRC_CSC_CELL_START_DURATION     3000
#define RRC_CSC_CELL_STOP_DURATION      3000
#ifdef LINUX_PC_TEST//added by zengyan for pc test
#define RRC_CSC_SFN_PERIOD              10*3000
#else
#define RRC_CSC_SFN_PERIOD              10
#endif


/******************************************************************************
*   FUNCTION NAME: rrc_csc_init_gl_ctx
*   INPUT        : csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : This function init CSC global context with default values
******************************************************************************/
void rrc_csc_init_gl_ctx
(
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    RRC_CSC_UT_TRACE_ENTER();

    memset_wrapper(p_csc_gl_ctx, 0, sizeof(csc_gl_ctx_t));

    /* Init timers */
    p_csc_gl_ctx->oamh_init_ind_duration    = RRC_CSC_OAMH_INIT_IND_DURATION;
    p_csc_gl_ctx->cell_cfg_duration         = RRC_CSC_CELL_CFG_DURATION;

    p_csc_gl_ctx->cell_recfg_duration       = RRC_CSC_CELL_RECFG_DURATION;

    p_csc_gl_ctx->cell_del_duration         = RRC_CSC_CELL_DEL_DURATION;
    p_csc_gl_ctx->pws_duration              = RRC_CSC_PWS_DURATION;
    p_csc_gl_ctx->kill_duration             = RRC_CSC_KILL_DURATION;
    p_csc_gl_ctx->cell_start_duration       = RRC_CSC_CELL_START_DURATION;
    p_csc_gl_ctx->cell_stop_duration        = RRC_CSC_CELL_STOP_DURATION;
    p_csc_gl_ctx->sfn_period                = RRC_CSC_SFN_PERIOD;
    p_csc_gl_ctx->num_allocated_cell_ctx_curr = 0;

    /* Init oamh_init_ind_timer */
    /* Already done in memset_wrapper */

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_csc_init
*   INPUT        : void *p_rrc_csc_init_data
*   OUTPUT       : None
*   RETURNS      : Buffer that will be passed in rrc_csc_process_msg
*                  and in rrc_csc_process_timer_msg callbacks
*   DESCRIPTION  : This function is CSPL callback. It initialize CSC entity.
*
******************************************************************************/
void* rrc_csc_init
(
    /* Pointer to CSC stack initialization data. Unused */
    void *p_rrc_csc_init_data
)
{
    csc_gl_ctx_t *p_csc_gl_ctx = PNULL;

    RRC_CSC_UT_TRACE_ENTER();
    RRC_CSC_TRACE(RRC_DETAILED,
            "Unused variable in fsm csc_init_data=%p ", p_rrc_csc_init_data);

    RRC_CSC_TRACE(RRC_INFO, "Init.");

    /* Allocate Memory for CSC global context data structure */
    /* p_csc_gl_ctx = (csc_gl_ctx_t *)rrc_sys_mem_get(sizeof(csc_gl_ctx_t)); */
    /* CSC review comments YR_1 */
    p_csc_gl_ctx = (csc_gl_ctx_t *)rrc_mem_get(sizeof(csc_gl_ctx_t));

    if(PNULL == p_csc_gl_ctx)
    {
        RRC_CSC_TRACE(RRC_FATAL, "rrc_csc_init: unable to Allocate Memory for "
            "CSC global context!");
        return PNULL;
    }
    else
    {
        csc_timer_buf_t repeated_timer_buf;
        repeated_timer_buf.auto_delete = RRC_FALSE;
        repeated_timer_buf.target_submodule = CSC_SUBMODULE_OAMH;

        rrc_csc_init_gl_ctx(p_csc_gl_ctx);

        RRC_CSC_TRACE(RRC_BRIEF,
            "Start timer [CSC_OAMH_INIT_IND] %u ms.",
            p_csc_gl_ctx->oamh_init_ind_duration);

        /* Start repeated timer for sending CSC_OAMH_INIT_IND */
        p_csc_gl_ctx->oamh_init_ind_timer =
                    rrc_start_timer(p_csc_gl_ctx->oamh_init_ind_duration,
                                    &repeated_timer_buf,
                                    sizeof(repeated_timer_buf),
                                    RRC_TRUE);
    }

    RRC_CSC_UT_TRACE_EXIT();

    return p_csc_gl_ctx;
}

