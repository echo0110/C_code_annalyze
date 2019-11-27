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
 *  File Description : The file rrc_phy_il_parser.h contains the prototypes 
 *                     of RRC-PHY interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_PHY_IL_PARSER_H_
#define _RRC_PHY_IL_PARSER_H_

#include "rrc_defines.h"
#include "rrc_phy_intf.h"

rrc_return_et
rrc_il_parse_rrc_phy_config_cell_req
(
    rrc_phy_config_cell_req_t *p_rrc_phy_config_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_config_cell_cnf
(
    rrc_phy_config_cell_cnf_t *p_rrc_phy_config_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_reconfig_cell_req
(
    rrc_phy_reconfig_cell_req_t *p_rrc_phy_reconfig_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_reconfig_cell_cnf
(
    rrc_phy_reconfig_cell_cnf_t *p_rrc_phy_reconfig_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_delete_cell_req
(
    rrc_phy_delete_cell_req_t *p_rrc_phy_delete_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_delete_cell_cnf
(
    rrc_phy_delete_cell_cnf_t *p_rrc_phy_delete_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_create_ue_entity_req
(
    rrc_phy_create_ue_entity_req_t *p_rrc_phy_create_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_create_ue_entity_cnf
(
    rrc_phy_create_ue_entity_cnf_t *p_rrc_phy_create_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_delete_ue_entity_req
(
    rrc_phy_delete_ue_entity_req_t *p_rrc_phy_delete_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_delete_ue_entity_cnf
(
    rrc_phy_delete_ue_entity_cnf_t *p_rrc_phy_delete_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_reconfig_ue_entity_req
(
    rrc_phy_reconfig_ue_entity_req_t *p_rrc_phy_reconfig_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_reconfig_ue_entity_cnf
(
    rrc_phy_reconfig_ue_entity_cnf_t *p_rrc_phy_reconfig_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_change_crnti_req
(
    rrc_phy_change_crnti_req_t *p_rrc_phy_change_crnti_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_change_crnti_cnf
(
    rrc_phy_change_crnti_cnf_t *p_rrc_phy_change_crnti_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_cell_start_req
(
    rrc_phy_cell_start_req_t *p_rrc_phy_cell_start_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_cell_start_cnf
(
    rrc_phy_cell_start_cnf_t *p_rrc_phy_cell_start_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_cell_stop_req
(
    rrc_phy_cell_stop_req_t *p_rrc_phy_cell_stop_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_rrc_phy_cell_stop_cnf
(
    rrc_phy_cell_stop_cnf_t *p_rrc_phy_cell_stop_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif /* _RRC_PHY_IL_PARSER_H_ */
