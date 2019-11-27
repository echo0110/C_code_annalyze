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
 *  File Description : The file oam_s1ap_il_parser.h contains the prototypes 
 *                     of RRC-S1APOAM interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _OAM_S1AP_IL_PARSER_H_
#define _OAM_S1AP_IL_PARSER_H_

#include "rrc_defines.h"
#include "rrc_s1apOam_intf.h"
#include "oam_defines.h"
#include "oam_proto.h"

oam_return_et
oam_rrc_il_parse_s1ap_oam_enb_config_update
(
    s1ap_oam_enb_config_update_t *p_s1ap_oam_enb_config_update,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_mme_conn_status_response
(
    s1ap_oam_mme_conn_status_response_t *p_s1ap_oam_mme_conn_status_response,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_enb_config_update_response
(
    s1ap_oam_enb_config_update_response_t *p_s1ap_oam_enb_config_update_response,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_close_mme_conn
(
    s1ap_oam_close_mme_conn_t *p_s1ap_oam_close_mme_conn,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_reestab_mme_conn
(
    s1ap_oam_reestab_mme_conn_t *p_s1ap_oam_reestab_mme_conn,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_provision_req
(
    s1ap_oam_provision_req_t *p_s1ap_oam_provision_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_provision_resp
(
    s1ap_oam_provision_resp_t *p_s1ap_oam_provision_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_get_log_level_resp
(
    s1ap_oam_get_log_level_resp_t *p_s1ap_oam_get_log_level_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_set_log_level_req
(
    s1ap_oam_set_log_level_req_t *p_s1ap_oam_set_log_level_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_set_log_level_resp
(
    s1ap_oam_set_log_level_resp_t *p_s1ap_oam_set_log_level_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_log_enable_req
(
    s1ap_oam_log_enable_req_t *p_s1ap_oam_log_enable_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_log_enable_resp
(
    s1ap_oam_log_enable_resp_t *p_s1ap_oam_log_enable_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_reset_resp
(
    s1ap_oam_reset_resp_t *p_s1ap_oam_reset_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_s1ap_link_status_ind
(
    s1ap_oam_s1ap_link_status_ind_t *p_s1ap_oam_s1ap_link_status_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_status_resp
(
    s1ap_oam_status_resp_t *p_s1ap_oam_status_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_mme_info
(
    s1ap_oam_mme_info_t *p_s1ap_oam_mme_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_get_sctp_status_req
(
    s1ap_oam_get_sctp_status_req_t *p_s1ap_oam_get_sctp_status_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_rrc_il_parse_s1ap_oam_get_sctp_status_resp
(
    s1ap_oam_get_sctp_status_resp_t *p_s1ap_oam_get_sctp_status_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

#endif /* _OAM_S1AP_IL_PARSER_H_ */
