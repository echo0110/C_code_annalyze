/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: oam_rrmNb_composer.h $
 *
 *******************************************************************************
 *
 * File Description: RRM OAM NB Composer file used to declare RRM OAM NB APIs 
 *                   function
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _RRM_OAMNB_IL_COMPOSER_H_
#define _RRM_OAMNB_IL_COMPOSER_H_

#include "oam_types.h"
#include "rrm_oam_intf.h"
#include "oam_types.h"
#include "rrm_oamNb_intf.h"
rrm_length_t
rrm_get_rrm_oam_nb_cell_config_req_len
(
    rrm_oam_nb_cell_config_req_t *p_rrm_oam_nb_cell_config_req
);

oam_return_et
oam_compose_rrm_oam_nb_cell_config_req
(
    U8  **pp_buffer,
    rrm_oam_nb_cell_config_req_t *p_rrm_oam_nb_cell_config_req
);

rrm_length_t
rrm_get_rrm_oam_nb_cell_config_resp_len
(
    rrm_oam_nb_cell_config_resp_t *p_rrm_oam_nb_cell_config_resp
);

oam_return_et
oam_compose_rrm_oam_nb_cell_config_resp
(
    U8  **pp_buffer,
    rrm_oam_nb_cell_config_resp_t *p_rrm_oam_nb_cell_config_resp
);

oam_return_et
oam_rrmNb_send_rrm_oam_nb_cell_config_req
(
    rrm_oam_nb_cell_config_req_t  *p_rrm_oam_nb_cell_config_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

oam_return_et
oam_rrmNb_send_rrm_oam_nb_cell_config_resp
(
    rrm_oam_nb_cell_config_resp_t  *p_rrm_oam_nb_cell_config_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

#endif /* _RRM_OAMNB_IL_COMPOSER_H_ */
