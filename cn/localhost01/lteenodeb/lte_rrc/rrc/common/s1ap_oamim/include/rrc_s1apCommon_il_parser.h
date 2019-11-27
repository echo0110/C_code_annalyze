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
 *  File Description : The file rrc_s1apCommon_il_parser.h contains the prototypes 
 *                     of RRC-S1APCOMMON interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#ifndef _RRC_S1APCOMMON_IL_PARSER_H_
#define _RRC_S1APCOMMON_IL_PARSER_H_

#include "rrc_defines.h"
#include "rrc_s1apCommon_intf.h"

rrc_return_et
rrc_parse_s1ap_add_mme_req
(
    s1ap_add_mme_req_t *p_s1ap_add_mme_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et
rrc_parse_s1ap_add_mme_res
(
    s1ap_add_mme_res_t *p_s1ap_add_mme_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif /* _RRC_S1APCOMMON_IL_PARSER_H_ */
