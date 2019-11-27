/******************************************************************************
*
*   FILE NAME:
*       csc_sfn_m.h
*
*   DESCRIPTION:
*       This is the header file of the CSC SFN manager.
*       SFN manager maintain SFN in RRC.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*   06 June 2009    VasylS      ---------       Cleanup support
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_SFN_M_H_
#define _CSC_SFN_M_H_

#include "csc_global_ctx.h"
#include "csc_cell_ctx.h"
#include "csc_timers.h"

void sfn_m_init_sfn
(
    rrc_sfn_t       sfn,
    csc_cell_ctx_t  *p_cell_ctx,
    csc_gl_ctx_t    *p_csc_gl_ctx
);

rrc_sfn_t sfn_m_get_sfn
(
    csc_cell_ctx_t  *p_cell_ctx
);

void sfn_m_llim_sfn_ind_process_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx_t
);

void sfn_m_llim_sfn_resp_process_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx_t
);

void sfn_m_stop_sfn
(
    csc_cell_ctx_t  *p_cell_ctx
);

void csc_sfn_m_process_timer_msg
(
    csc_timer_buf_t     *p_csc_timer_buf,
    csc_gl_ctx_t        *p_csc_gl_ctx_t
);

void csc_rrm_build_and_send_updated_pws_si_req(
    csc_gl_ctx_t        *p_csc_gl_ctx_t,
    csc_cell_ctx_t *p_cell_ctx,
    rrc_warning_info_list_t   *rrc_warning_info_list
);

#endif /* _CSC_SFN_M_H_ */

