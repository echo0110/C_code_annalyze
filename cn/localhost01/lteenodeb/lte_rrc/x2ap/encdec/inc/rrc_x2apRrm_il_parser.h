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
 *  File Description : The file rrc_x2apRrm_il_parser.h contains the prototypes 
 *                     of RRC-X2APRRM interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_X2APRRM_IL_PARSER_H_
#define _RRC_X2APRRM_IL_PARSER_H_

#include "rrc_defines.h"
#include "rrc_x2apRrm_intf.h"

rrc_return_et
rrc_il_parse_rrm_updated_mbms_sf_info
(
    rrm_updated_mbms_sf_info_t *p_rrm_updated_mbms_sf_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_x2_link_down_ind
(
    x2ap_rrm_x2_link_down_ind_t *p_x2ap_rrm_x2_link_down_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_x2_link_up_ind_st
(
    x2ap_rrm_x2_link_up_ind_st *p_x2ap_rrm_x2_link_up_ind_st,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_rrm_start_req
(
    x2ap_rrm_rsu_rrm_start_req_t *p_x2ap_rrm_rsu_rrm_start_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_enb_start_res
(
    x2ap_rrm_rsu_enb_start_res_t *p_x2ap_rrm_rsu_enb_start_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_enb_stop_res
(
    x2ap_rrm_rsu_enb_stop_res_t *p_x2ap_rrm_rsu_enb_stop_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_rrm_start_res
(
    x2ap_rrm_rsu_rrm_start_res_t *p_x2ap_rrm_rsu_rrm_start_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_rrm_stop_res
(
    x2ap_rrm_rsu_rrm_stop_res_t *p_x2ap_rrm_rsu_rrm_stop_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_enb_start_req
(
    x2ap_rrm_rsu_enb_start_req_t *p_x2ap_rrm_rsu_enb_start_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_rrm_update
(
    x2ap_rrm_rsu_rrm_update_t *p_x2ap_rrm_rsu_rrm_update,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_rrm_stop_req
(
    x2ap_rrm_rsu_rrm_stop_req_t *p_x2ap_rrm_rsu_rrm_stop_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_enb_stop_req
(
    x2ap_rrm_rsu_enb_stop_req_t *p_x2ap_rrm_rsu_enb_stop_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_rsu_enb_update_ind
(
    x2ap_rrm_rsu_enb_update_ind_t *p_x2ap_rrm_rsu_enb_update_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_load_information_req
(
    x2ap_rrm_load_information_req_t *p_x2ap_rrm_load_information_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_load_information_resp
(
    x2ap_rrm_load_information_resp_t *p_x2ap_rrm_load_information_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rrm_load_information_ind
(
    x2ap_rrm_load_information_ind_t *p_x2ap_rrm_load_information_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_rlf_ind
(
    x2ap_rlf_ind_t *p_x2ap_rlf_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


#ifdef ENDC_ENABLED

rrc_return_et
rrc_il_parse_x2ap_endc_config_update_en_gnb_ind
(
    x2ap_endc_config_update_en_gnb_ind_t *p_x2_endc_config_update_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_endc_config_update_req
(
    x2ap_endc_config_update_req_t *p_x2ap_endc_config_update_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_endc_config_update_resp
(
    x2ap_endc_config_update_resp_t *p_x2ap_endc_config_update_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_endc_config_update_wait_ind
(
    x2ap_endc_config_update_wait_ind_t *p_x2ap_endc_config_update_wait_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif

#endif /* _RRC_X2APRRM_IL_PARSER_H_ */
