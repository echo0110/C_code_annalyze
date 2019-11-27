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
 *  File Description : The file rrc_pdcp_il_parser.h contains the prototypes 
 *                     of RRC-PDCP interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_PDCP_IL_PARSER_H_
#define _RRC_PDCP_IL_PARSER_H_

#include "rrc_defines.h"
#include "rrc_pdcp_intf.h"

rrc_return_et
rrc_il_parse_rrc_pdcp_cr_ue_entity_req
(
    rrc_pdcp_cr_ue_entity_req_t *p_rrc_pdcp_cr_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_cr_ue_entity_cnf
(
    rrc_pdcp_cr_ue_entity_cnf_t *p_rrc_pdcp_cr_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_del_ue_entity_req
(
    rrc_pdcp_del_ue_entity_req_t *p_rrc_pdcp_del_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_del_ue_entity_cnf
(
    rrc_pdcp_del_ue_entity_cnf_t *p_rrc_pdcp_del_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_update_lwa_traffic_ind
(
    rrc_pdcp_update_lwa_traffic_ind_t *p_rrc_pdcp_update_lwa_traffic_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_reconf_ue_entity_req
(
    rrc_pdcp_reconf_ue_entity_req_t *p_rrc_pdcp_reconf_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_reconfig_ue_entity_cnf
(
    rrc_pdcp_reconfig_ue_entity_cnf_t *p_rrc_pdcp_reconfig_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_srb_data_req
(
    rrc_pdcp_srb_data_req_t *p_rrc_pdcp_srb_data_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_srb_data_ind
(
    rrc_pdcp_srb_data_ind_t *p_rrc_pdcp_srb_data_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_srb_data_status_ind
(
    rrc_pdcp_srb_data_status_ind_t *p_rrc_pdcp_srb_data_status_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_suspend_ue_entity_req
(
    rrc_pdcp_suspend_ue_entity_req_t *p_rrc_pdcp_suspend_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_suspend_ue_entity_cnf
(
    rrc_pdcp_suspend_ue_entity_cnf_t *p_rrc_pdcp_suspend_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_sn_hfn_status_req
(
    rrc_pdcp_sn_hfn_status_req_t *p_rrc_pdcp_sn_hfn_status_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_sn_hfn_status_resp
(
    rrc_pdcp_sn_hfn_status_resp_t *p_rrc_pdcp_sn_hfn_status_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_sn_hfn_status_ind
(
    rrc_pdcp_sn_hfn_status_ind_t *p_rrc_pdcp_sn_hfn_status_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_data_buffer_stop_ind
(
    rrc_pdcp_data_buffer_stop_ind_t *p_rrc_pdcp_data_buffer_stop_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_mac_i_req
(
    rrc_pdcp_mac_i_req_t *p_rrc_pdcp_mac_i_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_mac_i_resp
(
    rrc_pdcp_mac_i_resp_t *p_rrc_pdcp_mac_i_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_re_establish_ue_entity_req
(
    rrc_pdcp_re_establish_ue_entity_req_t *p_rrc_pdcp_re_establish_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_re_establish_ue_entity_cnf
(
    rrc_pdcp_re_establish_ue_entity_cnf_t *p_rrc_pdcp_re_establish_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_resume_ue_entity_req
(
    rrc_pdcp_resume_ue_entity_req_t *p_rrc_pdcp_resume_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_resume_ue_entity_cnf
(
    rrc_pdcp_resume_ue_entity_cnf_t *p_rrc_pdcp_resume_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_change_crnti_req
(
    rrc_pdcp_change_crnti_req_t *p_rrc_pdcp_change_crnti_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_change_crnti_cnf
(
    rrc_pdcp_change_crnti_cnf_t *p_rrc_pdcp_change_crnti_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_count_wraparound_ind
(
    rrc_pdcp_count_wraparound_ind_t *p_rrc_pdcp_count_wraparound_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_notify_integrity_failure
(
    rrc_pdcp_notify_integrity_failure_t *p_rrc_pdcp_notify_integrity_failure,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_drb_count_msb_req
(
    rrc_pdcp_drb_count_msb_req_t *p_rrc_pdcp_drb_count_msb_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_drb_count_msb_resp
(
    rrc_pdcp_drb_count_msb_resp_t *p_rrc_pdcp_drb_count_msb_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_config_cell_req
(
    rrc_pdcp_config_cell_req_t *p_rrc_pdcp_config_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_config_cell_cnf
(
    rrc_pdcp_config_cell_cnf_t *p_rrc_pdcp_config_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_ho_prep_info_req
(
    rrc_pdcp_ho_prep_info_req_t *p_rrc_pdcp_ho_prep_info_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_ho_prep_info_resp
(
    rrc_pdcp_ho_prep_info_resp_t *p_rrc_pdcp_ho_prep_info_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_pdcp_inactive_ues_ind
(
    rrc_pdcp_inactive_ues_ind_t *p_rrc_pdcp_inactive_ues_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif /* _RRC_PDCP_IL_PARSER_H_ */
