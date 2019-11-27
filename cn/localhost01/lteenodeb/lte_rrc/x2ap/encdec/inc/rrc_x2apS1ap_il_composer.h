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
 *  File Description : The file rrc_x2apS1ap_il_composer.h contains the prototypes 
 *                     of RRC-X2APS1AP interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_X2APS1AP_IL_COMPOSER_H_
#define _RRC_X2APS1AP_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_x2apS1ap_intf.h"

rrc_length_t
rrc_il_get_x2ap_s1ap_comm_info_len
(
    x2ap_s1ap_comm_info_t *p_x2ap_s1ap_comm_info
);

rrc_return_et
rrc_il_compose_x2ap_s1ap_comm_info
(
    U8  **pp_buffer,
    x2ap_s1ap_comm_info_t *p_x2ap_s1ap_comm_info
);

rrc_return_et
rrc_x2apS1ap_il_send_x2ap_s1ap_comm_info
(
    x2ap_s1ap_comm_info_t  *p_x2ap_s1ap_comm_info,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _RRC_X2APS1AP_IL_COMPOSER_H_ */
