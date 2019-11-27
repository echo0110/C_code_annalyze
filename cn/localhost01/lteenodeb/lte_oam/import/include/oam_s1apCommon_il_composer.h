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
 *  File Description : The file oam_s1apCommon_il_composer.h contains the prototypes 
 *                     of OAM-L3 interface message composing functions.
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
#include "oam_defines.h"
#include "oam_proto.h"

rrc_length_t
rrc_get_s1ap_add_mme_req_len
(
    s1ap_add_mme_req_t *p_s1ap_add_mme_req
);

oam_return_et
oam_compose_s1ap_add_mme_req
(
    UInt8  **pp_buffer,
    s1ap_add_mme_req_t *p_s1ap_add_mme_req
);

rrc_length_t
rrc_get_s1ap_add_mme_res_len
(
    s1ap_add_mme_res_t *p_s1ap_add_mme_res
);

oam_return_et
oam_compose_s1ap_add_mme_res
(
    UInt8  **pp_buffer,
    s1ap_add_mme_res_t *p_s1ap_add_mme_res
);

oam_return_et
oam_s1apCommon_send_s1ap_add_mme_req
(
    s1ap_add_mme_req_t  *p_s1ap_add_mme_req,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

oam_return_et
oam_s1apCommon_send_s1ap_add_mme_res
(
    s1ap_add_mme_res_t  *p_s1ap_add_mme_res,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  cell_index
);

#endif /* _RRC_S1APCOMMON_IL_COMPOSER_H_ */
