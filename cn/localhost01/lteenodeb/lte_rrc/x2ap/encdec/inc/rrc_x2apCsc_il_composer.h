/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2014 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: $
 ****************************************************************************
 *
 *  File Description : The file rrc_x2apCsc_il_composer.h contains the prototypes 
 *                     of RRC-X2APCSC interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_X2APCSC_IL_COMPOSER_H_
#define _RRC_X2APCSC_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_x2apCsc_intf.h"

rrc_length_t
rrc_il_get_x2ap_csc_cell_add_len
(
    x2ap_csc_cell_add_t *p_x2ap_csc_cell_add
);

rrc_return_et
rrc_il_compose_x2ap_csc_cell_add
(
    U8  **pp_buffer,
    x2ap_csc_cell_add_t *p_x2ap_csc_cell_add
);

rrc_length_t
rrc_il_get_x2ap_csc_cell_upd_len
(
    x2ap_csc_cell_upd_t *p_x2ap_csc_cell_upd
);

rrc_return_et
rrc_il_compose_x2ap_csc_cell_upd
(
    U8  **pp_buffer,
    x2ap_csc_cell_upd_t *p_x2ap_csc_cell_upd
);

rrc_length_t
rrc_il_get_x2ap_csc_cell_del_len
(
    x2ap_csc_cell_del_t *p_x2ap_csc_cell_del
);

rrc_return_et
rrc_il_compose_x2ap_csc_cell_del
(
    U8  **pp_buffer,
    x2ap_csc_cell_del_t *p_x2ap_csc_cell_del
);

rrc_length_t
rrc_il_get_x2ap_csc_cell_status_req_len
(
    x2ap_csc_cell_status_req_t *p_x2ap_csc_cell_status_req
);

rrc_return_et
rrc_il_compose_x2ap_csc_cell_status_req
(
    U8  **pp_buffer,
    x2ap_csc_cell_status_req_t *p_x2ap_csc_cell_status_req
);

rrc_length_t
rrc_il_get_x2ap_csc_cell_status_resp_len
(
    x2ap_csc_cell_status_resp_t *p_x2ap_csc_cell_status_resp
);

rrc_return_et
rrc_il_compose_x2ap_csc_cell_status_resp
(
    U8  **pp_buffer,
    x2ap_csc_cell_status_resp_t *p_x2ap_csc_cell_status_resp
);

rrc_return_et
rrc_x2apCsc_il_send_x2ap_csc_cell_add
(
    x2ap_csc_cell_add_t  *p_x2ap_csc_cell_add,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCsc_il_send_x2ap_csc_cell_upd
(
    x2ap_csc_cell_upd_t  *p_x2ap_csc_cell_upd,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCsc_il_send_x2ap_csc_cell_del
(
    x2ap_csc_cell_del_t  *p_x2ap_csc_cell_del,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCsc_il_send_x2ap_csc_cell_status_req
(
    x2ap_csc_cell_status_req_t  *p_x2ap_csc_cell_status_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apCsc_il_send_x2ap_csc_cell_status_resp
(
    x2ap_csc_cell_status_resp_t  *p_x2ap_csc_cell_status_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _RRC_X2APCSC_IL_COMPOSER_H_ */
