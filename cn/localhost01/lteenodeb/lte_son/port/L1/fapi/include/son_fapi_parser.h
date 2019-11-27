/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_fapi_parser.h $
 *
 *******************************************************************************
 *
 * File Description: File son_fapi_parser.h added for SON module code
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _SON_FAPI_IL_PARSER_H_
#define _SON_FAPI_IL_PARSER_H_

#include "son_defines.h"
#include "son_fapi_intf.h"

son_return_et
son_parse_son_phy_rssi_meas_req
(
    son_phy_rssi_meas_req_t *p_son_phy_rssi_meas_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_rssi_meas_ind
(
    son_phy_rssi_meas_ind_t *p_son_phy_rssi_meas_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_rssi_meas_cnf
(
    son_phy_rssi_meas_cnf_t *p_son_phy_rssi_meas_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_stop_rssi_meas_req
(
    son_phy_stop_rssi_meas_req_t *p_son_phy_stop_rssi_meas_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_cell_search_req
(
    son_phy_cell_search_req_t *p_son_phy_cell_search_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_cell_search_ind
(
    son_phy_cell_search_ind_t *p_son_phy_cell_search_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_cell_search_cnf
(
    son_phy_cell_search_cnf_t *p_son_phy_cell_search_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_stop_cell_search_req
(
    son_phy_stop_cell_search_req_t *p_son_phy_stop_cell_search_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_pbch_config_req
(
    son_phy_pbch_config_req_t *p_son_phy_pbch_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_pbch_config_cnf
(
    son_phy_pbch_config_cnf_t *p_son_phy_pbch_config_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_pbch_data_ind
(
    son_phy_pbch_data_ind_t *p_son_phy_pbch_data_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_stop_pbch_req
(
    son_phy_stop_pbch_req_t *p_son_phy_stop_pbch_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_sib1_config_req
(
    son_phy_sib1_config_req_t *p_son_phy_sib1_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_sib1_config_cnf
(
    son_phy_sib1_config_cnf_t *p_son_phy_sib1_config_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_bcch_config_req
(
    son_phy_bcch_config_req_t *p_son_phy_bcch_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_bcch_config_cnf
(
    son_phy_bcch_config_cnf_t *p_son_phy_bcch_config_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_bcch_data_ind
(
    son_phy_bcch_data_ind_t *p_son_phy_bcch_data_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

son_return_et
son_parse_son_phy_stop_bcch_req
(
    son_phy_stop_bcch_req_t *p_son_phy_stop_bcch_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif /* _SON_FAPI_IL_PARSER_H_ */
