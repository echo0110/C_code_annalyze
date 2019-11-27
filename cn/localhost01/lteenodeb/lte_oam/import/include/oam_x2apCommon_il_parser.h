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
 *  File Description : The file oam_x2apCommon_il_parser.h contains the prototypes 
 *                     of OAM-L3 interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#ifndef _RRC_X2APCOMMON_IL_PARSER_H_
#define _RRC_X2APCOMMON_IL_PARSER_H_

#include "rrc_defines.h"
#include "rrc_x2apCommon_intf.h"
#include "oam_defines.h"
#include "oam_proto.h"

oam_return_et
oam_parse_x2ap_enb_config_update_req
(
    x2ap_enb_config_update_req_t *p_x2ap_enb_config_update_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_enb_config_update_resp
(
    x2ap_enb_config_update_resp_t *p_x2ap_enb_config_update_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_enb_config_update_ind
(
    x2ap_enb_config_update_ind_t *p_x2ap_enb_config_update_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_reset_req
(
    x2ap_reset_req_t *p_x2ap_reset_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_reset_resp
(
    x2ap_reset_resp_t *p_x2ap_reset_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_reset_ind
(
    x2ap_reset_ind_t *p_x2ap_reset_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_add_enb_req
(
    x2ap_add_enb_req_t *p_x2ap_add_enb_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_add_enb_res
(
    x2ap_add_enb_res_t *p_x2ap_add_enb_res,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_delete_enb_req
(
    x2ap_delete_enb_req_t *p_x2ap_delete_enb_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_delete_enb_res
(
    x2ap_delete_enb_res_t *p_x2ap_delete_enb_res,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_enb_link_up_ind
(
    x2ap_enb_link_up_ind_t *p_x2ap_enb_link_up_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_enb_link_down_ind
(
    x2ap_enb_link_down_ind_t *p_x2ap_enb_link_down_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_enb_config_update_wait_ind
(
    x2ap_enb_config_update_wait_ind_t *p_x2ap_enb_config_update_wait_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_ho_report_ind
(
    x2ap_ho_report_ind_t *p_x2ap_ho_report_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_peer_cell_activation_req
(
    x2ap_peer_cell_activation_req_t *p_x2ap_peer_cell_activation_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_peer_cell_activation_resp
(
    x2ap_peer_cell_activation_resp_t *p_x2ap_peer_cell_activation_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_cell_activation_req
(
    x2ap_cell_activation_req_t *p_x2ap_cell_activation_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_cell_activation_response
(
    x2ap_cell_activation_response_t *p_x2ap_cell_activation_response,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_mobility_change_req
(
    x2ap_mobility_change_req_t *p_x2ap_mobility_change_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_mobility_change_resp
(
    x2ap_mobility_change_resp_t *p_x2ap_mobility_change_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_mobility_setting_change_req
(
    x2ap_mobility_setting_change_req_t *p_x2ap_mobility_setting_change_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_x2ap_mobility_setting_change_resp
(
    x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_setting_change_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

#endif /* _RRC_X2APCOMMON_IL_PARSER_H_ */
