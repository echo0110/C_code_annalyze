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
 *  File Description : The file oam_s1apCommon_il_parser.h contains the prototypes 
 *                     of OAM-L3 interface message parsing functions.
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
#include "oam_defines.h"
#include "oam_proto.h"

oam_return_et
oam_parse_s1ap_add_mme_req
(
    s1ap_add_mme_req_t *p_s1ap_add_mme_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_s1ap_add_mme_res
(
    s1ap_add_mme_res_t *p_s1ap_add_mme_res,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

#endif /* _RRC_S1APCOMMON_IL_PARSER_H_ */
