/******************************************************************************
*
*   FILE NAME:
*       ueccmd_llim.h
*
*   DESCRIPTION:
*       This is the header file of the UECC MD LLIM.
*       UECC MD OAMH interract with RRC LLIM module.
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _UECCMD_LLIM_H_
#define _UECCMD_LLIM_H_

#include "ueccmd_global_ctx.h"
#include "ueccmd_timers.h"

void ueccmd_uecc_llim_ccch_data_ind_process_msg
(
    void            *p_api,
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx_t
);

rrc_return_et partial_decode_uecc_llim_ccch_data_ind
(
    U8                *msg_type,        /* Destination */
    void              *p_api            /* Pointer to input API buffer */
);

void ueccmd_insert_conn_reestab_node
( 
    ueccmd_gl_ctx_t    *p_ueccmd_gl_ctx,
    void               *p_api           /* Pointer to input API buffer */
);

#endif /* _UECCMD_LLIM_H_ */

