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
 *  File Description : The file rrc_s1apOam_il_parser.h contains the prototypes 
 *                     of RRC-S1APOAM interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_S1APOAM_IL_PARSER_H_
#define _RRC_S1APOAM_IL_PARSER_H_

#include "rrc_defines.h"
#include "rrc_s1apOam_intf.h"

rrc_return_et
rrc_il_parse_s1ap_oam_enb_config_update
(
    s1ap_oam_enb_config_update_t *p_s1ap_oam_enb_config_update,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_mme_conn_status_response
(
    s1ap_oam_mme_conn_status_response_t *p_s1ap_oam_mme_conn_status_response,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_enb_config_update_response
(
    s1ap_oam_enb_config_update_response_t *p_s1ap_oam_enb_config_update_response,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_close_mme_conn
(
    s1ap_oam_close_mme_conn_t *p_s1ap_oam_close_mme_conn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_reestab_mme_conn
(
    s1ap_oam_reestab_mme_conn_t *p_s1ap_oam_reestab_mme_conn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_provision_req
(
    s1ap_oam_provision_req_t *p_s1ap_oam_provision_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_provision_resp
(
    s1ap_oam_provision_resp_t *p_s1ap_oam_provision_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_get_log_level_resp
(
    s1ap_oam_get_log_level_resp_t *p_s1ap_oam_get_log_level_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_set_log_level_req
(
    s1ap_oam_set_log_level_req_t *p_s1ap_oam_set_log_level_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_set_log_level_resp
(
    s1ap_oam_set_log_level_resp_t *p_s1ap_oam_set_log_level_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_log_enable_req
(
    s1ap_oam_log_enable_req_t *p_s1ap_oam_log_enable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_log_enable_resp
(
    s1ap_oam_log_enable_resp_t *p_s1ap_oam_log_enable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_reset_resp
(
    s1ap_oam_reset_resp_t *p_s1ap_oam_reset_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_s1ap_link_status_ind
(
    s1ap_oam_s1ap_link_status_ind_t *p_s1ap_oam_s1ap_link_status_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_status_resp
(
    s1ap_oam_status_resp_t *p_s1ap_oam_status_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_mme_info
(
    s1ap_oam_mme_info_t *p_s1ap_oam_mme_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_get_sctp_status_req
(
    s1ap_oam_get_sctp_status_req_t *p_s1ap_oam_get_sctp_status_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_s1ap_oam_get_sctp_status_resp
(
    s1ap_oam_get_sctp_status_resp_t *p_s1ap_oam_get_sctp_status_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);
#ifdef LINUX_PC_TEST
rrc_return_et
rrc_il_parse_test_sctp_message
(
	test_sctp_message_t* p_test_sctp_message,
	U8* p_src,
	S32 length_left,
	S32* p_length_read
);

rrc_return_et
rrc_il_parse_test_x2ap_lksctp_message
(
    test_x2ap_lksctp_message_t *p_test_x2ap_lksctp_message,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_test_x2ap_close_sctp
(
	test_x2ap_close_sctp_t* p_test_x2ap_close_sctp,
	U8* p_src,
	S32 length_left,
	S32* p_length_read
);
rrc_return_et
rrc_il_parse_s1ap_gb_status_resp
(
	s1ap_gb_status_resp_t* p_test_s1ap_gb_status_resp,
	U8* p_src,
	S32 length_left,
	S32* p_length_read
);
#endif
#endif /* _RRC_S1APOAM_IL_PARSER_H_ */
