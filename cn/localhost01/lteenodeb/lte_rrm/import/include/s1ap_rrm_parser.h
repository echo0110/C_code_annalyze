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
 *  File Description : The file rrc_s1apRrm_il_parser.h contains the prototypes 
 *                     of RRC-S1APRRM interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_S1APRRM_IL_PARSER_H_
#define _RRC_S1APRRM_IL_PARSER_H_

#include "rrm_defines.h"
#include "rrc_s1apRrm_intf.h"

rrm_return_et
rrm_il_parse_s1ap_rrm_enb_direct_info_transfer
(
    s1ap_rrm_enb_direct_info_transfer_t *p_s1ap_rrm_enb_direct_info_transfer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_mme_direct_info_transfer
(
    s1ap_rrm_mme_direct_info_transfer_t *p_s1ap_rrm_mme_direct_info_transfer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_enb_config_transfer
(
    s1ap_rrm_enb_config_transfer_t *p_s1ap_rrm_enb_config_transfer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_mme_config_transfer
(
    s1ap_rrm_mme_config_transfer_t *p_s1ap_rrm_mme_config_transfer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_pws_request
(
    s1ap_rrm_pws_request_t *p_s1ap_rrm_pws_request,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_pws_response
(
    s1ap_rrm_pws_response_t *p_s1ap_rrm_pws_response,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_pws_cnf
(
    s1ap_rrm_pws_cnf_t *p_s1ap_rrm_pws_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_kill_request
(
    s1ap_rrm_kill_request_t *p_s1ap_rrm_kill_request,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_kill_response
(
    s1ap_rrm_kill_response_t *p_s1ap_rrm_kill_response,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_kill_cnf
(
    s1ap_rrm_kill_cnf_t *p_s1ap_rrm_kill_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_rim_information_req
(
    rim_information_req_t *p_rim_information_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_rim_information
(
    rim_information_t *p_rim_information,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_rim_information_ack
(
    rim_information_ack_t *p_rim_information_ack,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_rim_information_app_err
(
    rim_information_app_err_t *p_rim_information_app_err,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_rim_information_err
(
    rim_information_err_t *p_rim_information_err,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_mme_info
(
    s1ap_rrm_mme_info_t *p_s1ap_rrm_mme_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_lppa_ecid_meas_init_req
(
    s1ap_rrm_lppa_ecid_meas_init_req_t *p_s1ap_rrm_lppa_ecid_meas_init_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_lppa_ecid_meas_init_resp
(
    s1ap_rrm_lppa_ecid_meas_init_resp_t *p_s1ap_rrm_lppa_ecid_meas_init_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_lppa_ecid_meas_init_fail
(
    s1ap_rrm_lppa_ecid_meas_init_fail_t *p_s1ap_rrm_lppa_ecid_meas_init_fail,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_lppa_ecid_meas_fail_ind
(
    s1ap_rrm_lppa_ecid_meas_fail_ind_t *p_s1ap_rrm_lppa_ecid_meas_fail_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_lppa_ecid_meas_report
(
    s1ap_rrm_lppa_ecid_meas_report_t *p_s1ap_rrm_lppa_ecid_meas_report,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_lppa_ecid_meas_termination_cmd
(
    s1ap_rrm_lppa_ecid_meas_termination_cmd_t *p_s1ap_rrm_lppa_ecid_meas_termination_cmd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_lppa_otdoa_info_req
(
    s1ap_rrm_lppa_otdoa_info_req_t *p_s1ap_rrm_lppa_otdoa_info_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_lppa_otdoa_info_resp
(
    s1ap_rrm_lppa_otdoa_info_resp_t *p_s1ap_rrm_lppa_otdoa_info_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_lppa_otdoa_info_fail
(
    s1ap_rrm_lppa_otdoa_info_fail_t *p_s1ap_rrm_lppa_otdoa_info_fail,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_lppa_error_ind
(
    s1ap_rrm_lppa_error_ind_t *p_s1ap_rrm_lppa_error_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrm_return_et
rrm_il_parse_s1ap_rrm_lppa_error_ind_req
(
    s1ap_rrm_lppa_error_ind_req_t *p_s1ap_rrm_lppa_error_ind_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif /* _RRC_S1APRRM_IL_PARSER_H_ */
