/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_rrm_composer.h $
 *
 *******************************************************************************
 *
 * File Description: SON RRM API Composer file to compose SON RRM APIs.
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _SON_RRM_IL_COMPOSER_H_
#define _SON_RRM_IL_COMPOSER_H_

#include "rrm_defines.h"
#include "rrm_son_intf.h"

rrm_length_t
rrm_get_son_anr_rrm_updated_nrt_info_len
(
    son_anr_rrm_updated_nrt_info_t *p_son_anr_rrm_updated_nrt_info
);

rrm_return_et
rrm_compose_son_anr_rrm_updated_nrt_info
(
    U8  **pp_buffer,
    son_anr_rrm_updated_nrt_info_t *p_son_anr_rrm_updated_nrt_info
);

rrm_length_t
rrm_get_son_apps_rrm_carrier_freq_and_dl_power_params_req_len
(
    son_apps_rrm_carrier_freq_and_dl_power_params_req_t *p_son_apps_rrm_carrier_freq_and_dl_power_params_req
);

rrm_return_et
rrm_compose_son_apps_rrm_carrier_freq_and_dl_power_params_req
(
    U8  **pp_buffer,
    son_apps_rrm_carrier_freq_and_dl_power_params_req_t *p_son_apps_rrm_carrier_freq_and_dl_power_params_req
);

rrm_length_t
rrm_get_son_apps_rrm_carrier_freq_and_dl_power_params_resp_len
(
    son_apps_rrm_carrier_freq_and_dl_power_params_resp_t *p_son_apps_rrm_carrier_freq_and_dl_power_params_resp
);

rrm_return_et
rrm_compose_son_apps_rrm_carrier_freq_and_dl_power_params_resp
(
    U8  **pp_buffer,
    son_apps_rrm_carrier_freq_and_dl_power_params_resp_t *p_son_apps_rrm_carrier_freq_and_dl_power_params_resp
);

rrm_length_t
rrm_get_son_rrm_ttt_update_ind_len
(
    son_rrm_ttt_update_ind_t *p_son_rrm_ttt_update_ind
);

rrm_return_et
rrm_compose_son_rrm_ttt_update_ind
(
    U8  **pp_buffer,
    son_rrm_ttt_update_ind_t *p_son_rrm_ttt_update_ind
);

rrm_length_t
rrm_get_son_rrm_stop_tnl_discovery_ind_len
(
    son_rrm_stop_tnl_discovery_ind_t *p_son_rrm_stop_tnl_discovery_ind
);

rrm_return_et
rrm_compose_son_rrm_stop_tnl_discovery_ind
(
    U8  **pp_buffer,
    son_rrm_stop_tnl_discovery_ind_t *p_son_rrm_stop_tnl_discovery_ind
);

rrm_return_et
rrm_son_send_son_anr_rrm_updated_nrt_info
(
    son_anr_rrm_updated_nrt_info_t  *p_son_anr_rrm_updated_nrt_info,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index 
);

rrm_return_et
rrm_son_send_son_apps_rrm_carrier_freq_and_dl_power_params_req
(
    son_apps_rrm_carrier_freq_and_dl_power_params_req_t  *p_son_apps_rrm_carrier_freq_and_dl_power_params_req,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index 
);

rrm_return_et
rrm_son_send_son_apps_rrm_carrier_freq_and_dl_power_params_resp
(
    son_apps_rrm_carrier_freq_and_dl_power_params_resp_t  *p_son_apps_rrm_carrier_freq_and_dl_power_params_resp,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index 
);

rrm_return_et
rrm_son_send_son_rrm_ttt_update_ind
(
    son_rrm_ttt_update_ind_t  *p_son_rrm_ttt_update_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index 
);

rrm_return_et
rrm_son_send_son_rrm_stop_tnl_discovery_ind
(
    son_rrm_stop_tnl_discovery_ind_t  *p_son_rrm_stop_tnl_discovery_ind,
    U16                 src_module_id,
    U16                 dest_module_id,
    U16                 transaction_id,
    U8                  cell_index 
);

#endif /* _SON_RRM_IL_COMPOSER_H_ */
