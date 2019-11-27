/******************************************************************************
*
*   FILE NAME:
*       ueccmd_init.c
*
*   DESCRIPTION:
*       This is the main file of the UECC MD Initialization module. This module
*       provides initialization handler for UECC MD CSPL entity.
*
*       In addition the UECC MD clean up function is placed here.
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/

/****************************************************************************
 * Local includes
 ****************************************************************************/
#include "rrc_common_utils.h"
#include "ueccmd_global_ctx.h"
#include "ueccmd_logger.h"
#include "ueccmd_timers.h"
#include "ueccmd_init.h"
#include "rrc_handlers.h" 

/* Default duration of UECC MD timers in ms */
#define RRC_UECCMD_OAMH_INIT_IND_DURATION  1000

/******************************************************************************
 * Private functions definitions
 *****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: rrc_ueccmd_init_gl_ctx
*   INPUT        : ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : none
*   DESCRIPTION  : This function init UECC MD global context with default values
******************************************************************************/
void rrc_ueccmd_init_gl_ctx
(
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx   /* Pointer to the UECCMD global context */
)
{
    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT (PNULL != p_ueccmd_gl_ctx);

    memset_wrapper(p_ueccmd_gl_ctx, 0, sizeof(ueccmd_gl_ctx_t));

    /* Init timers */
    p_ueccmd_gl_ctx->oamh_init_ind_duration = RRC_UECCMD_OAMH_INIT_IND_DURATION;

    p_ueccmd_gl_ctx->current_proc = UECCMD_LAST_PROC;

    /*Initialize UE Context release Search Tree */
    st_init(&(p_ueccmd_gl_ctx->ueccmd_ue_ctx_search_tree),
            &compare_ue_ctx_search_tree,
            key_of_ue_ctx_search_tree);

    /*Initialize UE Connection reestablishment Search Tree */
    st_init(&(p_ueccmd_gl_ctx->ueccmd_ue_conn_reestab_search_tree),
            &compare_ue_ctx_search_tree,
            key_of_ue_conn_reestab_search_tree);

    /* Rest of the IEs are initialized through memset_wrapper */

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_ueccmd_init
*   INPUT        : void *p_rrc_ueccmd_init_data
*   OUTPUT       : none
*   RETURNS      : Buffer that will be passed in rrc_ueccmd_process_msg
*                  and in rrc_ueccmd_process_timer_msg callbacks
*   DESCRIPTION  : This is the main entry point of UECC module - CSPL callback. It handles
*                  initialization of UECC MD module.
******************************************************************************/
void* rrc_ueccmd_init
(
    void *p_rrc_ueccmd_init_data /* Pointer to RRC stack initialization data */
)
{
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_UECCMD_TRACE(RRC_DETAILEDALL, "Unused variable in ueccmd_init:%p",p_rrc_ueccmd_init_data);     

    RRC_UECCMD_TRACE(RRC_DETAILED, "Init.");

    /* Allocate Memory for UECC MD global context data structure */
    p_ueccmd_gl_ctx =
        (ueccmd_gl_ctx_t *)rrc_mem_get(sizeof(ueccmd_gl_ctx_t));

    if(PNULL == p_ueccmd_gl_ctx)
    {
        RRC_UECCMD_TRACE(RRC_FATAL, "rrc_ueccmd_init: unable to Allocate Memory "
                "for UECCMD global context!");
        return PNULL;
    }
    else
    {
        ueccmd_timer_buf_t repeated_timer_buf;
        repeated_timer_buf.auto_delete = RRC_FALSE;
        
        /* Initialize the Global Context */ 
        rrc_ueccmd_init_gl_ctx(p_ueccmd_gl_ctx);

        RRC_UECCMD_TRACE(RRC_BRIEF,
                "Start timer [UECCMD_OAMH_INIT_IND] %u ms.",
                p_ueccmd_gl_ctx->oamh_init_ind_duration);

        /* Start repeated timer for sending UECCMD_OAMH_INIT_IND */
        p_ueccmd_gl_ctx->oamh_init_ind_timer =
            rrc_start_timer(p_ueccmd_gl_ctx->oamh_init_ind_duration,
                    &repeated_timer_buf,
                    sizeof(repeated_timer_buf),
                    RRC_TRUE);
    }
    RRC_UECCMD_UT_TRACE_EXIT();

    return p_ueccmd_gl_ctx;
}
