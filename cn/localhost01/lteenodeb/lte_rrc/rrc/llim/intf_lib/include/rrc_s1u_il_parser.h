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
 *  File Description : The file rrc_s1u_il_parser.h contains the prototypes 
 *                     of RRC-S1U interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_S1U_IL_PARSER_H_
#define _RRC_S1U_IL_PARSER_H_

#include "rrc_defines.h"
#include "rrc_s1u_intf.h"

rrc_return_et
rrc_il_parse_rrc_s1u_delete_ue_entity_req
(
    rrc_s1u_delete_ue_entity_req_t *p_rrc_s1u_delete_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_delete_ue_entity_cnf
(
    rrc_s1u_delete_ue_entity_cnf_t *p_rrc_s1u_delete_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_reconfigure_ue_entity_req
(
    rrc_s1u_reconfigure_ue_entity_req_t *p_rrc_s1u_reconfigure_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_reconfigure_ue_entity_cnf
(
    rrc_s1u_reconfigure_ue_entity_cnf_t *p_rrc_s1u_reconfigure_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_create_ue_entity_req
(
    rrc_s1u_create_ue_entity_req_t *p_rrc_s1u_create_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_create_ue_entity_cnf
(
    rrc_s1u_create_ue_entity_cnf_t *p_rrc_s1u_create_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_error_ind
(
    rrc_s1u_error_ind_t *p_rrc_s1u_error_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_path_failure_ind
(
    rrc_s1u_path_failure_ind_t *p_rrc_s1u_path_failure_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_end_marker_ind
(
    rrc_s1u_end_marker_ind_t *p_rrc_s1u_end_marker_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_path_success_ind
(
    rrc_s1u_path_success_ind_t *p_rrc_s1u_path_success_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_intra_enb_data_fwd_req
(
    rrc_s1u_intra_enb_data_fwd_req_t *p_rrc_s1u_intra_enb_data_fwd_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_intra_enb_data_fwd_cnf
(
    rrc_s1u_intra_enb_data_fwd_cnf_t *p_rrc_s1u_intra_enb_data_fwd_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_local_path_switch_req
(
    rrc_s1u_local_path_switch_req_t *p_rrc_s1u_local_path_switch_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_local_path_switch_cnf
(
    rrc_s1u_local_path_switch_cnf_t *p_rrc_s1u_local_path_switch_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_config_cell_req
(
    rrc_s1u_config_cell_req_t *p_rrc_s1u_config_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_s1u_config_cell_cnf
(
    rrc_s1u_config_cell_cnf_t *p_rrc_s1u_config_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif /* _RRC_S1U_IL_PARSER_H_ */
