/******************************************************************************
*
*   FILE NAME:
*       ueccmd_init.h
*
*   DESCRIPTION:
*       This is the header file for the init functions of UECC MD.
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _UECCMD_INIT_H_
#define _UECCMD_INIT_H_

#include "ueccmd_global_ctx.h"
#include "ueccmd_timers.h"

void rrc_ueccmd_init_gl_ctx
(
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx   /* Pointer to the UECCMD global context */
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

const void * key_of_ue_conn_reestab_search_tree
(
    /* Pointer to Tree node */
     const YTNODE * p_node
);
#endif /* _UECCMD_INIT_H_ */

