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
 *  File Description : The file rrc_rlc_il_parser.h contains the prototypes 
 *                     of RRC-RLC interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_RLC_IL_PARSER_H_
#define _RRC_RLC_IL_PARSER_H_

#include "rrc_defines.h"
#include "rrc_rlc_intf.h"

rrc_return_et
rrc_il_parse_rrc_rlc_create_ue_entity_req
(
    rrc_rlc_create_ue_entity_req_t *p_rrc_rlc_create_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_create_ue_entity_cnf
(
    rrc_rlc_create_ue_entity_cnf_t *p_rrc_rlc_create_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_reconfig_ue_entity_req
(
    rrc_rlc_reconfig_ue_entity_req_t *p_rrc_rlc_reconfig_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_reconfig_ue_entity_cnf
(
    rrc_rlc_reconfig_ue_entity_cnf_t *p_rrc_rlc_reconfig_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_delete_ue_entity_req
(
    rrc_rlc_delete_ue_entity_req_t *p_rrc_rlc_delete_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_delete_ue_entity_cnf
(
    rrc_rlc_delete_ue_entity_cnf_t *p_rrc_rlc_delete_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_re_establish_ue_entity_req
(
    rrc_rlc_re_establish_ue_entity_req_t *p_rrc_rlc_re_establish_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_re_establish_ue_entity_cnf
(
    rrc_rlc_re_establish_ue_entity_cnf_t *p_rrc_rlc_re_establish_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rlc_ue_entity_error_ind
(
    rlc_ue_entity_error_ind_t *p_rlc_ue_entity_error_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_change_crnti_req
(
    rrc_rlc_change_crnti_req_t *p_rrc_rlc_change_crnti_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_change_crnti_cnf
(
    rrc_rlc_change_crnti_cnf_t *p_rrc_rlc_change_crnti_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_re_establish_ue_entity_complete_ind
(
    rrc_rlc_re_establish_ue_entity_complete_ind_t *p_rrc_rlc_re_establish_ue_entity_complete_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_re_establish_ue_entity_complete_cnf
(
    rrc_rlc_re_establish_ue_entity_complete_cnf_t *p_rrc_rlc_re_establish_ue_entity_complete_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_create_mbms_area_entity_req
(
    rrc_rlc_create_mbms_area_entity_req_t *p_rrc_rlc_create_mbms_area_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_create_mbms_area_entity_cnf
(
    rrc_rlc_create_mbms_area_entity_cnf_t *p_rrc_rlc_create_mbms_area_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_delete_mbms_area_entity_req
(
    rrc_rlc_delete_mbms_area_entity_req_t *p_rrc_rlc_delete_mbms_area_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_delete_mbms_area_entity_cnf
(
    rrc_rlc_delete_mbms_area_entity_cnf_t *p_rrc_rlc_delete_mbms_area_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_config_cell_req
(
    rrc_rlc_config_cell_req_t *p_rrc_rlc_config_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_rlc_config_cell_cnf
(
    rrc_rlc_config_cell_cnf_t *p_rrc_rlc_config_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif /* _RRC_RLC_IL_PARSER_H_ */
