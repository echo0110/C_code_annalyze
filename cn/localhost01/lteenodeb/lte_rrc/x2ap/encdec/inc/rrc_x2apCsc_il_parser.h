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
 *  File Description : The file rrc_x2apCsc_il_parser.h contains the prototypes 
 *                     of RRC-X2APCSC interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_X2APCSC_IL_PARSER_H_
#define _RRC_X2APCSC_IL_PARSER_H_

#include "rrc_defines.h"
#include "rrc_x2apCsc_intf.h"

rrc_return_et
rrc_il_parse_x2ap_csc_cell_add
(
    x2ap_csc_cell_add_t *p_x2ap_csc_cell_add,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_csc_cell_upd
(
    x2ap_csc_cell_upd_t *p_x2ap_csc_cell_upd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_csc_cell_del
(
    x2ap_csc_cell_del_t *p_x2ap_csc_cell_del,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_csc_cell_status_req
(
    x2ap_csc_cell_status_req_t *p_x2ap_csc_cell_status_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_il_parse_x2ap_csc_cell_status_resp
(
    x2ap_csc_cell_status_resp_t *p_x2ap_csc_cell_status_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif /* _RRC_X2APCSC_IL_PARSER_H_ */
