/******************************************************************************
*
*   FILE NAME:
*       rrc_llim_utils.h
*
*   DESCRIPTION:
*       This header file contains declaration LLIM specific utilities
*
*   DATE            AUTHOR      REFERENCE       REASON
*   14 May 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_LLIM_UTILS_H_
#define _RRC_LLIM_UTILS_H_

#include "rrc_defines.h"
#include "rrc_llim_global_ctx.h"
#include "rrc_csc_llim_intf.h"
#include "rrc_llim_ue_ctx.h"

struct _rrc_llim_gl_ctx_t;

#define LLIM_LAST_ID            0xFFFF

typedef void (*p_llim_intf_handler_t) (
    void*                       p_api,             /* Input API buffer */
    struct _rrc_llim_gl_ctx_t*  p_rrc_llim_gl_ctx  /* LLIM global context */
);

typedef struct
{
    U16     id;
} llim_id_t;

typedef struct
{
    llim_id_t               api_id;
    p_llim_intf_handler_t   p_llim_intf_handler;
} llim_intf_handlers_table_t;

U16 llim_find_index_by_id
(
    U16         id,
    llim_id_t*  p_table,
    U16         element_size
);

rrc_return_t rrc_llim_send_internal_msg
(
    rrc_module_id_t             dst_module_id,
    U16                         api_id,
    U16                         api_msg_size,
    void                        *p_msg
);

void* rrc_llim_alloc_internal_msg
(
    rrc_module_id_t             dst_module_id,
    U16                         api_id,
    U16                         api_msg_size
);

void* rrc_llim_alloc_external_msg
(
    U16                         transaction_id,
    rrc_module_id_t             int_dst_module_id,
    rrc_module_id_t             ext_dst_module_id,
    U16                         api_id,
    U16                         api_msg_size,
    U8                          cell_index
);

void rrc_llim_send_csc_llim_config_cell_resp_failure
(
    rrc_llim_csc_response_et    response,
    rrc_transaction_id_t        transaction_id,
    rrc_cell_index_t            cell_index
);

rrc_return_et rrc_llim_send_csc_llim_reconfig_cell_resp_failure
(
    rrc_llim_csc_response_et    response,
    rrc_transaction_id_t        transaction_id,
    rrc_cell_index_t            cell_index
);
rrc_return_et rrc_llim_send_csc_llim_cell_start_resp
(
    rrc_transaction_id_t        transaction_id,
    rrc_cell_index_t            cell_index,
    rrc_llim_csc_response_et    response,
    rrc_sfn_t                   sfn,
    /*cdma_fix bug 10186 start*/
    rrc_sf_t                    sf,
    rrc_timestamp_t             timestamp
    /*cdma_fix bug 10186 stop */
);
rrc_return_et rrc_llim_send_csc_llim_cell_stop_resp
(
    rrc_transaction_id_t        transaction_id,
    rrc_cell_index_t            cell_index,
    rrc_llim_csc_response_et    response
);

void rrc_llim_get_ue_context
(
  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx,
  rrc_ue_index_t      ue_id,
  rrc_llim_ue_ctx_t   **p_p_ue_ctx
);

void rrc_llim_set_ue_context
(
  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx,
  rrc_ue_index_t      ue_id,
  rrc_llim_ue_ctx_t   *p_p_ue_ctx
);

void rrc_llim_get_cell_index(
   rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx,  /* Pointer to the LLIM global context data */
   void                *p_api,              /* Ponter to Received Message */
   rrc_cell_index_t    *cell_index          /* Pointer to Cell Index */
);

/* SPR 21660 changes end */

#endif /* _RRC_LLIM_UTILS_H_ */

