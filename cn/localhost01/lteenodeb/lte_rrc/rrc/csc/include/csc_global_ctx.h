/******************************************************************************
*
*   FILE NAME:
*       csc_global_ctx.h
*
*   DESCRIPTION:
*       This file contains RRC CSC global contexts definitions.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*   06 June 2009    VasylS      ---------       Cleanup support
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_GLOBAL_CTX_H_
#define _CSC_GLOBAL_CTX_H_

#include "rrc_defines.h"
struct _csc_cell_ctx_t;

typedef struct _csc_gl_ctx_t
{
    /* oamh_init_ind_timer duration */
    rrc_timer_duration_t    oamh_init_ind_duration;
    /* CSC_OAMH_PROV_REQ msg, CSC_CELL_SETUP_TIMER duration */
    rrc_timer_duration_t    cell_cfg_duration;
    /* CSC_OAMH_PROV_REQ msg, CSC_CELL_RECONFIG_TIMER duration */
    rrc_timer_duration_t    cell_recfg_duration;
    /* CSC_OAMH_PROV_REQ msg,  CSC_CELL_DELETE_TIMER duration */
    rrc_timer_duration_t    cell_del_duration;
    /* CSC_PWS_TIMER duration */
    rrc_timer_duration_t    pws_duration;
    /* CSC_KILL_TIMER duration */
    rrc_timer_duration_t    kill_duration;
    /* CSC_OAMH_PROV_REQ msg, CSC_CELL_START_TIMER duration */
    rrc_timer_duration_t    cell_start_duration;
    /* CSC_OAMH_PROV_REQ msg, CSC_CELL_STOP_TIMER duration */
    rrc_timer_duration_t    cell_stop_duration;
    /* csc_cell_ctx_t::sfn_timer period */
    rrc_timer_duration_t    sfn_period;
    /* CSC_OAMH_INIT_IND timer */
    rrc_timer_t             oamh_init_ind_timer;
    /* SFN_OFFSET required for NON -Broadcast info */
    rrc_sfn_t               sfn_offset;
    /*Maxium retry count for cell reconfig */
    U8                      max_cell_reconfig_retry_count;
     /* Maximum Number of suported Cells */
     U8                      max_num_supported_cells;
     struct _csc_cell_ctx_t  **p_p_cell_ctx;
     /* Number of cell contexts allocated currently */
     U8                      num_allocated_cell_ctx_curr;
     /*Null Integrity Support of cell for Emergency services */
     rrc_bool_t              null_integrity_support[MAX_NUM_CELLS];
     U16                     max_num_supported_ue_per_cell[MAX_NUM_CELLS];
     U8                      csc_cell_index_status;
     U8                      no_of_uecc_instances;
    rrc_timer_t             csc_health_monitor_timer;
  
} csc_gl_ctx_t;

void rrc_csc_init_gl_ctx
(
    csc_gl_ctx_t    *p_csc_gl_ctx
);


#endif /* _CSC_GLOBAL_CTX_H_ */

