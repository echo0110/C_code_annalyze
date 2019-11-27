/******************************************************************************
*
*   FILE NAME:
*       rrc_logging.c
*
*   DESCRIPTION:
*       This is the main file of the logging thread.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   27 Jun 2013     Simar       ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
/*Logging Changes Start*/
#define _GNU_SOURCE
#include <cspl.h>
#include <stacklayer.h>
#include "rrc_defines.h"
#include "rrc_handlers.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"
#include "rrc_logging.h"
#include "rrc_oam_intf.h"
#if (LOGGER_TYPE == RRC_ADV_LOGGER)
#include "advlogging.h"
#endif

extern thread_arg_t thd_arg[RRC_MAX_THREADS];
extern pthread_key_t  logger_key;

/******************************************************************************
*   FUNCTION NAME: rrc_logging_gl_ctx_init
*
*       This function initialises Logger global context with default values
*
*   RETURNS:
*       None
*
******************************************************************************/
void rrc_logging_gl_ctx_init(
    /* Pointer to the Logger global context data */
    rrc_logging_gl_ctx_t *p_rrc_log_gl_ctx,
    void                 *p_reader_config)
{
    /* Init timers */
    l3_memcpy_wrapper(&p_rrc_log_gl_ctx->reader_config, 
            (rrc_logging_config_t *)p_reader_config, 
            sizeof (rrc_logging_config_t));
}

/******************************************************************************
*   FUNCTION NAME: start_log_read_timer
*
*   DESCRIPTION:   This functions starts the Log Reader timer
*
*   RETURNS:  None
*
******************************************************************************/
void start_log_read_timer(rrc_logging_gl_ctx_t *p_rrc_log_gl_ctx)
{
    rrc_logging_timer_buf_t timer_buf;
    timer_buf.timer_type = RRC_LOGGING_READ_TIMER;
    timer_buf.auto_delete = RRC_FALSE;

    /* Start repeated timer for sending LLIM_OAMH_INIT_IND */
    p_rrc_log_gl_ctx->init_log_read_timer =
        rrc_start_timer(p_rrc_log_gl_ctx->reader_config.int_log_read_duration,
                &timer_buf,
                sizeof(timer_buf),
/* Bug 8676 Fix Start */
                RRC_TRUE);
/* Bug 8676 Fix End */
}

/******************************************************************************
*   FUNCTION NAME: l3_logger_init
*
*   DESCRIPTION:
*       This is the main entry point of LOGGING module - CSPL callback. It handles
*       initialization of Logging module.
*
*   RETURNS:
*
******************************************************************************/
void* l3_logger_init
(
    void *p_reader_config /* Pointer to RRC stack initialization data */
)
{
    rrc_logging_gl_ctx_t *p_rrc_log_gl_ctx = PNULL;

#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    /* Allocate Memory for Logging global context data structure */
    p_rrc_log_gl_ctx = (rrc_logging_gl_ctx_t *)rrc_mem_get(sizeof(rrc_logging_gl_ctx_t));

    if(PNULL == p_rrc_log_gl_ctx)
    {
        return PNULL;
    }
    else
    {
        memset_wrapper(p_rrc_log_gl_ctx, 0, sizeof(rrc_logging_gl_ctx_t));

        /* Init Logging global context */
        rrc_logging_gl_ctx_init(p_rrc_log_gl_ctx, p_reader_config);

        start_log_read_timer(p_rrc_log_gl_ctx);
    }
#endif
    return p_rrc_log_gl_ctx;
}

/*****************************************************************************
 * Function Name  : l3_logger_process_timer_msg
 * Inputs         : timer_id - The id of the timer expired.
 *                  p_timer_buf - Pointer to timer buffer which is allocated
 *                                when starting a timer.
 *                  p_gl_ctx - Pointer to Logger global context.
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *       It handles all the timer expiry events for Logger module.
 *       The timer buffer should always start with rrc_logging_timer_buf_t
 *****************************************************************************/
void    l3_logger_process_timer_msg(
        rrc_timer_t timer_id,   /* Identity of the expired timer */
        void        *p_timer_buf,/* Pointer to data associated with the timer */
        void        *p_gl_ctx)   /* Pointer to the Logger global context data */
{
    /*SPR 17777 +*/
    (void)timer_id;
    (void)p_gl_ctx;
    /*SPR 17777 -*/
#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    /*rrc_logging_gl_ctx_t       *p_logging_gb_ctx =  RRC_NULL;*/
    rrc_logging_timer_buf_t *p_logging_timer_buf = RRC_NULL;

    /* Precondition check */
    RRC_ASSERT(RRC_NULL!= p_timer_buf);
    RRC_ASSERT(RRC_NULL!= p_gl_ctx);

    /*p_logging_gb_ctx    = (rrc_logging_gl_ctx_t *)p_gl_ctx;*/
    p_logging_timer_buf = (rrc_logging_timer_buf_t*)p_timer_buf;
    

    switch (p_logging_timer_buf->timer_type)
    {
        case RRC_LOGGING_READ_TIMER:
        {
            lgr_process_log_segments();
            break;
        }
        default:
            break;
    }
#endif
}

S32 l3_logger_process_msg( void *p_api, void *p_gl_ctx)
{
    /*SPR 17777 +*/
    (void)p_api;
    (void)p_gl_ctx;
    /*SPR 17777 -*/
    return 1;
}

