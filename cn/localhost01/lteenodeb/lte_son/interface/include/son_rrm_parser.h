/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_rrm_parser.h $
 *
 *******************************************************************************
 *
 * File Description: File son_rrm_parser.h added for SON module code
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _SON_RRM_IL_PARSER_H_
#define _SON_RRM_IL_PARSER_H_

#include "son_defines.h"
#include "son_rrm_intf.h"

son_return_et
son_parse_son_anr_rrm_updated_nrt_info
(
    son_anr_rrm_updated_nrt_info_t *p_son_anr_rrm_updated_nrt_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_apps_rrm_carrier_freq_and_dl_power_params_req
(
    son_apps_rrm_carrier_freq_and_dl_power_params_req_t *p_son_apps_rrm_carrier_freq_and_dl_power_params_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_apps_rrm_carrier_freq_and_dl_power_params_resp
(
    son_apps_rrm_carrier_freq_and_dl_power_params_resp_t *p_son_apps_rrm_carrier_freq_and_dl_power_params_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_rrm_ttt_update_ind
(
    son_rrm_ttt_update_ind_t *p_son_rrm_ttt_update_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_rrm_stop_tnl_discovery_ind
(
    son_rrm_stop_tnl_discovery_ind_t *p_son_rrm_stop_tnl_discovery_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif /* _SON_RRM_IL_PARSER_H_ */
