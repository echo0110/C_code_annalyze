/******************************************************************************
*
*   FILE NAME:
*       rrc_llim_oamh.h
*
*   DESCRIPTION:
*       This is the header file of the LLIM OAMH.
*       LLIM OAMH interract with RRC OAMH module.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   01 Apr 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _RRC_LLIM_OAMH_H_
#define _RRC_LLIM_OAMH_H_

#include "rrc_llim_global_ctx.h"
#include "rrc_llim_timers.h"

void rrc_llim_oamh_init_cnf_processs_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_llim_oamh_prov_req_processs_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_llim_oamh_cleanup_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_llim_oamh_process_timer_msg
(
    rrc_timer_t             timer_id,
    rrc_llim_timer_buf_t    *p_rrc_llim_timer_buf,
    rrc_llim_gl_ctx_t       *p_rrc_llim_gl_ctx
);

void start_llim_health_monitor_timer
(
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx,
    rrc_timer_duration_t    duration
);

#endif /* _RRC_LLIM_OAMH_H_ */
