/******************************************************************************
*
*   FILE NAME:
*       uecc_csc.h
*
*   DESCRIPTION:
*       This is the interface file of the UECC CSC.
*       UECC CSC interacts with RRC CSC module.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   17 Jul 2009     GrygoriyS   ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef UECC_CSC_H
#define UECC_CSC_H

#include "uecc_global_ctx.h"
#include "uecc_ed.h"


void uecc_csc_process_init_setup_ind
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
);

void uecc_csc_process_del_all_ue_req
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
);
void uecc_csc_process_cell_reconfig_ind
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
);
rrc_return_et uecc_csc_build_and_send_delete_all_ue_entity_resp
(
    rrc_return_et     response,
    uecc_gb_context_t  *p_uecc_gb_context,
    rrc_cell_index_t   cell_index
);
void uecc_csc_process_cell_start_ind
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t  *p_uecc_gb_context
);

void uecc_csc_process_cell_stop_ind
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t  *p_uecc_gb_context
);


#endif /* UECC_CSC_H */
