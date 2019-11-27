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
 *  File Description : The file rrc_s1apCommon_il_composer.h contains the prototypes 
 *                     of RRC-S1APCOMMON interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_S1APCOMMON_IL_COMPOSER_H_
#define _RRC_S1APCOMMON_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_s1apCommon_intf.h"

rrc_length_t
rrc_get_s1ap_add_mme_req_len
(
    s1ap_add_mme_req_t *p_s1ap_add_mme_req
);

rrc_return_et
rrc_compose_s1ap_add_mme_req
(
    U8  **pp_buffer,
    s1ap_add_mme_req_t *p_s1ap_add_mme_req
);

rrc_length_t
rrc_get_s1ap_add_mme_res_len
(
    s1ap_add_mme_res_t *p_s1ap_add_mme_res
);

rrc_return_et
rrc_compose_s1ap_add_mme_res
(
    U8  **pp_buffer,
    s1ap_add_mme_res_t *p_s1ap_add_mme_res
);

rrc_return_et
rrc_s1apCommon_send_s1ap_add_mme_req
(
    s1ap_add_mme_req_t  *p_s1ap_add_mme_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_s1apCommon_send_s1ap_add_mme_res
(
    s1ap_add_mme_res_t  *p_s1ap_add_mme_res,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _RRC_S1APCOMMON_IL_COMPOSER_H_ */
