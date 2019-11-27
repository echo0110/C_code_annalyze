/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2012 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: ueccmd_global_ctx.h $
 ****************************************************************************
 *
 *   DESCRIPTION:
 *       This is the UECC MD global context data definition file.
 *
 ****************************************************************************/

#ifndef UECCMD_GLOBAL_CTX_H
#define UECCMD_GLOBAL_CTX_H

/****************************************************************************
 * Local includes
 ****************************************************************************/

#include "rrc_defines.h"
#include "rrc_oamh_intf.h"
#include "search_tree_wrap.h"
 
/****************************************************************************
 * Public definitions
 ****************************************************************************/

#define MAX_UECC_INSTANCES     0x08

/****************************************************************************
* Public types definitions
****************************************************************************/
typedef enum
{
    UECCMD_RESET_CELL_STATS,
    UECCMD_GET_CELL_STATUS,
    UECCMD_DELETE_ALL_UE,
    UECCMD_TRAFFIC_TRACE_START,
    UECCMD_GET_UE_STATUS,
    UECCMD_PROVISION,
    UECCMD_CLEANUP,
    UECCMD_LAST_PROC
}ueccmd_current_proc_t;
#define MAX_CELL_PROC          5
/*CID 112464 Fix End */

#define UECCMD_RESET_CELL_STATS_PROC_RUNNING         1
#define UECCMD_GET_CELL_STATUS_PROC_RUNNING          2
#define UECCMD_DELETE_ALL_UE_PROC_RUNNING            4
#define UECCMD_TRAFFIC_TRACE_START_PROC_RUNNING      8
#define UECCMD_GET_UE_STATUS_PROC_RUNNING           16

typedef struct _ueccmd_uecc_resp_per_cell_t
{
    /* Response from UECC (SUCCESS/FAILURE) */
    U8                      uecc_resp[MAX_UECC_INSTANCES];
    /* Number of responses received from UECC instances */
    U8                      num_resp_recv;
    /* UECC Cell Status Response */
    rrc_uecc_oamh_get_cell_status_resp_t    cell_status_resp;
    rrc_uecc_oamh_get_ue_status_resp_t      ue_status_resp;
} ueccmd_uecc_resp_per_cell_t;

typedef struct _ueccmd_cell_ctx_t
{
    U8                      bitmask; /* Bitmask to store Procedure Running at UECCMD for cell */
    ueccmd_uecc_resp_per_cell_t    uecc_resp_per_cell[MAX_CELL_PROC];      

}ueccmd_cell_ctx_t;

typedef struct _ueccmd_ue_ctx_release_node_t
{
    /* Tree anchor */
    search_tree_node_t              anchor;
    U32                             mme_ue_id;
    /* Response from UECC (SUCCESS/FAILURE) */
    U8                              uecc_resp[MAX_UECC_INSTANCES];
    /* Number of responses received from UECC instances */
    U8                              num_resp_recv;
}ueccmd_ue_ctx_release_node_t;

typedef struct
{
    U8    cell_index; 
    U16   crnti;
    U16   pci;
    U16   short_mac_i;
}ueccmd_ue_reestab_info_t;

typedef struct _ueccmd_ue_conn_reestab_node_t
{
    /* Tree anchor */
    search_tree_node_t              anchor;
    rrc_rnti_t                      rnti;
    /* Response from UECC (SUCCESS/FAILURE) */
    U8                              uecc_resp[MAX_UECC_INSTANCES];
    /* Number of responses received from UECC instances */
    U8                              num_resp_recv;
    ueccmd_ue_reestab_info_t        ue_reestab_info;
}ueccmd_ue_conn_reestab_node_t;

typedef search_tree_t ueccmd_ue_ctx_search_tree_t;

typedef search_tree_t ueccmd_ue_conn_reestab_search_tree_t;

typedef struct _ueccmd_gl_ctx_t
{
    /* oamh_init_ind timer duration */
    rrc_timer_duration_t    oamh_init_ind_duration;
    /* UECCMD_OAMH_INIT_IND timer */
    rrc_timer_t             oamh_init_ind_timer;
    /* Number of UECC instances created */
    U8                      num_uecc_instances;
    /* Sum of UE's supported in all the cells */
    U16                     total_ue_supported;
    /* Offset of UECC Module Id which currently will be used for UE allocation */
    U8                      not_allocated_uecc_index_cur;
    
    /* Response from UECC (SUCCESS/FAILURE) for cleanup or provisioning*/
    U8                      uecc_resp[MAX_UECC_INSTANCES];
    /* Number of responses received from UECC instances for cleanup or provisioning*/
    U8                      num_resp_recv;
    /* Current procedure running at UECC MD (cleanup or provisioning)*/
    ueccmd_current_proc_t   current_proc;
    ueccmd_cell_ctx_t                       ueccmd_cell_ctx[MAX_NUM_CELLS];                      
    ueccmd_ue_ctx_search_tree_t             ueccmd_ue_ctx_search_tree;                      
    ueccmd_ue_conn_reestab_search_tree_t    ueccmd_ue_conn_reestab_search_tree;                      

} ueccmd_gl_ctx_t;

void rrc_ueccmd_init_gl_ctx
(
     ueccmd_gl_ctx_t *p_ueccmd_gl_ctx   /* Pointer to the UECCMD global context */
);


#endif /* UECCMD_GLOBAL_CTX_H */
