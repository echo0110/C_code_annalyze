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
 *  File Description : The file oam_x2ap_il_parser.h contains the prototypes 
 *                     of RRC-X2APOAM interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _OAM_X2AP_IL_PARSER_H_
#define _OAM_X2AP_IL_PARSER_H_

#include "rrc_defines.h"
#include "rrc_x2apOam_intf.h"
#include "oam_defines.h"
#include "oam_proto.h"

oam_return_et
oam_rrc_il_parse_x2ap_oam_provision_req
(
    x2ap_oam_provision_req_t *p_x2ap_oam_provision_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_x2ap_oam_provision_resp
(
    x2ap_oam_provision_resp_t *p_x2ap_oam_provision_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_x2ap_oam_get_log_level_resp
(
    x2ap_oam_get_log_level_resp_t *p_x2ap_oam_get_log_level_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_x2ap_oam_set_log_level_req
(
    x2ap_oam_set_log_level_req_t *p_x2ap_oam_set_log_level_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_x2ap_oam_set_log_level_resp
(
    x2ap_oam_set_log_level_resp_t *p_x2ap_oam_set_log_level_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_x2ap_oam_log_enable_req
(
    x2ap_oam_log_enable_req_t *p_x2ap_oam_log_enable_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_x2ap_oam_log_enable_resp
(
    x2ap_oam_log_enable_resp_t *p_x2ap_oam_log_enable_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_x2ap_oam_x2_link_down
(
    x2ap_oam_x2_link_down_t *p_x2ap_oam_x2_link_down,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_x2ap_oam_x2_link_up
(
    x2ap_oam_x2_link_up_t *p_x2ap_oam_x2_link_up,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_x2ap_oam_x2_link_setup_req
(
    x2ap_oam_x2_link_setup_req_t *p_x2ap_oam_x2_link_setup_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_x2ap_oam_x2_link_setup_resp
(
    x2ap_oam_x2_link_setup_resp_t *p_x2ap_oam_x2_link_setup_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_x2ap_oam_cleanup_resp
(
    x2ap_oam_cleanup_resp_t *p_x2ap_oam_cleanup_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_x2ap_oam_get_sctp_status_req
(
    x2ap_oam_get_sctp_status_req_t *p_x2ap_oam_get_sctp_status_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_x2ap_oam_get_sctp_status_resp
(
    x2ap_oam_get_sctp_status_resp_t *p_x2ap_oam_get_sctp_status_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

#endif /* _OAM_X2AP_IL_PARSER_H_ */
