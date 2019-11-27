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
 *  File Description : The file rrc_x2apS1ap_il_parser.h contains the prototypes 
 *                     of RRC-X2APS1AP interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_X2APS1AP_IL_PARSER_H_
#define _RRC_X2APS1AP_IL_PARSER_H_

#include "rrc_defines.h"
#include "rrc_x2apS1ap_intf.h"

rrc_return_et
rrc_il_parse_x2ap_s1ap_comm_info
(
    x2ap_s1ap_comm_info_t *p_x2ap_s1ap_comm_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif /* _RRC_X2APS1AP_IL_PARSER_H_ */
