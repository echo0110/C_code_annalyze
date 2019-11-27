/******************************************************************************
*
*   FILE NAME:
*       rrc_cemh.h
*
*   DESCRIPTION:
*       This header file contains declaration of in points of CEMH submodule
*
*   DATE            AUTHOR      REFERENCE       REASON
*   14 May 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_CEMH_H_
#define _RRC_CEMH_H_

#include "rrc_llim_global_ctx.h"
#include "rrc_llim_timers.h"

void rrc_cemh_csc_llim_config_cell_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_cemh_csc_llim_delete_cell_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_cemh_csc_llim_pcch_data_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);


void rrc_cemh_phy_config_cell_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_cemh_mac_config_cell_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_cemh_mac_sfn_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_cemh_mac_sfn_ind_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_cemh_oamh_cleanup_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_cemh_process_timer_msg
(
    rrc_timer_t             timer_id,
    rrc_llim_timer_buf_t    *p_rrc_llim_timer_buf,
    rrc_llim_gl_ctx_t       *p_rrc_llim_gl_ctx
);

void rrc_cemh_csc_llim_sfn_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_cemh_mac_reconfig_cell_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_cemh_csc_llim_reconfig_cell_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_cemh_phy_reconfig_cell_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_cemh_csc_llim_write_replace_warning_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_cemh_csc_llim_si_without_etws_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_cemh_mac_delete_cell_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_cemh_mac_cell_start_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_cemh_mac_cell_stop_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_cemh_phy_delete_cell_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_cemh_phy_cell_start_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_cemh_phy_cell_stop_cnf_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_cemh_csc_llim_cell_start_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_cemh_csc_llim_cell_stop_req_process_msg
(
    void                *p_api,
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
void rrc_cemh_rlc_create_mbms_area_cnf_process_msg
(
     /* Pointer to input API buffer */
     void                *p_api,
     /* Pointer to the LLIM global context data */
     rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);

void rrc_cemh_rlc_delete_mbms_area_cnf_process_msg
(
     /* Pointer to input API buffer */
     void                *p_api,
     /* Pointer to the LLIM global context data */
     rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
);
#endif /* _RRC_CEMH_H_ */
