/******************************************************************************
*
*   FILE NAME:
*       ueccmd_oamh.h
*
*   DESCRIPTION:
*       This is the header file of the UECC MD OAMH.
*       UECC MD OAMH interact with RRC OAMH module.
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _UECCMD_OAMH_H_
#define _UECCMD_OAMH_H_

#include "ueccmd_global_ctx.h"
#include "ueccmd_timers.h"

void ueccmd_oamh_init_cnf_process_msg
(
    void            *p_api,
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx_t
);

void ueccmd_oamh_prov_req_process_msg
(
    void            *p_api,
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx_t
);


rrc_return_et ueccmd_create_uecc_instances
(
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx, /* Pointer to the UECC MD global context */
    U8              num_of_uecc_instances /* Number of UECC instances to be created */
);

void ueccmd_oamh_cleanup_req_process_msg
(
     void            *p_api,
     ueccmd_gl_ctx_t *p_ueccmd_gl_ctx_t
);

void ueccmd_process_traffic_trace_start_msg
(
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx, /* Pointer to the UECC MD global context */
    void            *p_api          /* Pointer to input API buffer */
);

void ueccmd_process_add_del_lgw_ind_msg
(
    ueccmd_gl_ctx_t           *p_ueccmd_gl_ctx,  /* Pointer to the UECCMD global context */
    void                      *p_api           /* Pointer to input API buffer */
);
#endif /* _UECCMD_OAMH_H_ */

