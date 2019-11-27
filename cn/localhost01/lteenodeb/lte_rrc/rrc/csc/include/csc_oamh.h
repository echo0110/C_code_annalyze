/******************************************************************************
*
*   FILE NAME:
*       csc_oamh.h
*
*   DESCRIPTION:
*       This is the header file of the CSC OAMH.
*       CSC OAMH interract with RRC OAMH module.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*   06 June 2009    VasylS      ---------       Cleanup support
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_OAMH_H_
#define _CSC_OAMH_H_

#include "csc_global_ctx.h"
#include "csc_timers.h"

void csc_oamh_init_cnf_process_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx_t
);

void csc_oamh_prov_req_process_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx_t
);

void csc_oamh_cleanup_req_process_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx_t
);
/* SPR 20908 Start */
void csc_oamh_communication_info_resp_process_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx_t
);
/* SPR 20908 End */
void csc_oamh_process_timer_msg
(
    rrc_timer_t         timer_id,
    csc_timer_buf_t     *p_csc_timer_buf,
    csc_gl_ctx_t        *p_csc_gl_ctx_t
);

void start_csc_health_monitor_timer
(
    csc_gl_ctx_t *p_csc_gl_ctx,
    rrc_timer_duration_t    duration
);

#endif /* _CSC_OAMH_H_ */

