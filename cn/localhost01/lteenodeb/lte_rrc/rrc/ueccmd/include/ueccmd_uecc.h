/******************************************************************************
*
*   FILE NAME:
*       ueccmd_uecc.h
*
*   DESCRIPTION:
*       This is the header file of the UECC MD LLIM.
*       UECC MD OAMH interract with RRC LLIM module.
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _UECCMD_UECC_H_
#define _UECCMD_UECC_H_

#include "ueccmd_global_ctx.h"
#include "ueccmd_timers.h"
void ueccmd_uecc_oamh_prov_resp_process_msg
(
     void            *p_api,
     ueccmd_gl_ctx_t *p_ueccmd_gl_ctx_t
);

void ueccmd_uecc_oamh_get_cell_status_resp_process_msg
(
     void            *p_api,
     ueccmd_gl_ctx_t *p_ueccmd_gl_ctx_t
);

void ueccmd_oamh_cleanup_resp_process_msg
(
     void            *p_api,
     ueccmd_gl_ctx_t *p_ueccmd_gl_ctx_t
);

/*SPR_17727_START*/
void ueccmd_uecc_ue_con_re_establish_process_msg
(
     void            *p_api,
     ueccmd_gl_ctx_t *p_ueccmd_gl_ctx_t
);
/*SPR_17727_END*/

void ueccmd_uecc_llim_ccch_data_req_process_msg
(
     void            *p_api,
     ueccmd_gl_ctx_t *p_ueccmd_gl_ctx_t
);

void ueccmd_oamh_cell_traffic_trace_start_error_ind_process_msg
(
     void            *p_api,
     ueccmd_gl_ctx_t *p_ueccmd_gl_ctx_t
);

void ueccmd_uecc_csc_delete_all_ue_resp_process_msg
(
    void            *p_api,
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx_t
);

void ueccmd_oamh_get_ue_status_resp_process_msg
(
     void            *p_api,
     ueccmd_gl_ctx_t *p_ueccmd_gl_ctx_t
);

void ueccmd_s1ap_ue_associated_sig_msg_req_process_msg
(
     void            *p_api,
     ueccmd_gl_ctx_t *p_ueccmd_gl_ctx_t
);

void ueccmd_uecc_llim_ue_con_re_establish_rej_req_process_msg
(
    void            *p_api,
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
);


void ueccmd_ue_context_release_ind_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
);

void ueccmd_uecc_proc_status_ind_process_msg
(
 void            *p_api,
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
);
#endif /* _UECCMD_UECC_H_ */
