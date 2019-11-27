/******************************************************************************
*
*   FILE NAME:
*       csc_cell_m.h
*
*   DESCRIPTION:
*       This header file contains declaration of
*       in points of CSC cell manager submodule
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*   06 June 2009    VasylS      ---------       Cleanup support
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_CELL_M_H_
#define _CSC_CELL_M_H_

#include "csc_global_ctx.h"
#include "csc_timers.h"

void cell_m_rrm_cell_setup_req_process_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx
);
void cell_m_rrm_cell_delete_req_process_msg
(
   void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx
);

void cell_m_s1ap_paging_ind_process_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx
);

void cell_m_llim_config_cell_resp_process_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx
);

void cell_m_llim_reconfig_cell_resp_process_msg
(
    void            *p_api,         
    csc_gl_ctx_t    *p_csc_gl_ctx   
);
void cell_m_llim_delete_cell_resp_process_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx
);

void cell_m_oamh_cleanup_req_process_msg
(
    csc_gl_ctx_t    *p_csc_gl_ctx
);

void csc_cell_m_process_timer_msg
(
    rrc_timer_t         timer_id,
    csc_timer_buf_t     *p_csc_timer_buf,
    csc_gl_ctx_t        *p_csc_gl_ctx
);

void cell_m_rrm_cell_reconfig_req_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
);

void cell_m_uecc_del_cell_resp_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
);

void cell_m_rrm_updated_pws_si_list_resp_process_msg 
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
);
void cell_m_s1ap_pws_req
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
);
void cell_m_s1ap_pws_failure_req
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
);
void cell_m_llim_pws_resp_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
);
void cell_m_llim_updated_pws_si_resp_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
);
void cell_m_s1ap_pws_failure_ind
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
);
void cell_m_s1ap_kill_req
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
);
void cell_m_s1ap_kill_failure_ind
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
);
void cell_m_rrm_cell_start_req_process_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx
);
void cell_m_llim_cell_start_resp_process_msg
(
  void            *p_api,           /* pointer to the inpu api buffer */
  csc_gl_ctx_t    *p_csc_gl_ctx    /* pointer to teh CSC global context data */
);
void cell_m_rrm_cell_stop_req_process_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx
);
void cell_m_llim_cell_stop_resp_process_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx
);

void cell_m_x2ap_csc_cell_status_req_process_and_send_resp_msg
(
    void            *p_api,
    csc_gl_ctx_t    *p_csc_gl_ctx
);
#endif /* _CSC_CELL_M_H_ */

