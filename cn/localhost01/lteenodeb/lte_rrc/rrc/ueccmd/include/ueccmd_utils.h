/******************************************************************************
*
*   FILE NAME:
*       ueccmd_utils.h
*
*   DESCRIPTION:
*       This header file contains declaration UECCMD specific utilities
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _UECCMD_UTILS_H_
#define _UECCMD_UTILS_H_

#include "rrc_defines.h"
#include "ueccmd_global_ctx.h"

void ueccmd_broadcast_msg
(
    void            *p_api,           /* Pointer to input API buffer */
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECCMD global context */
);

void ueccmd_round_robin_ue_allocation_to_uecc
(
 void            *p_api,           /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx  /* Pointer to the UECCMD global context */
);

void* ueccmd_alloc_intrl_msg
(
  rrc_module_id_t             src_module_id,
  rrc_module_id_t             dst_module_id, /* Destination */
  U16                         api_id,         /* API Id */
  rrc_size_t                  msg_size        /* Size of message (payload) */
);

void ueccmd_check_cell_state_and_broadcast_msg
(
    ueccmd_gl_ctx_t           *p_ueccmd_gl_ctx,  /* Pointer to the UECCMD global context */
    void                      *p_api,           /* Pointer to input API buffer */
    U8                         current_proc     /* Bitmask value for Procedure */
);

S32 compare_ue_ctx_search_tree
(
    /* Pointer to first element being compared */
    const void * p_elem1 ,
    /* Pointer to second element being compared */
    const void * p_elem2
);

const void * key_of_ue_ctx_search_tree
(
     /* Pointer to Tree node */
     const YTNODE * p_node
);

rrc_cell_index_t ueccmd_get_cell_index
(
    void                      *p_api,           /* Pointer to input API buffer */
    U8                         current_proc     /* Bitmask value for Procedure */
);

const void * key_of_ue_conn_reestab_search_tree
(
     /* Pointer to Tree node */
     const YTNODE * p_node
);

#endif /* _UECCMD_UTILS_H_ */

