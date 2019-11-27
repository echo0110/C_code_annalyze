/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: oam_rrmNb_parser.h $
 *
 *******************************************************************************
 *
 * File Description: RRM OAM NB API parser file to declare the RRM OAM NB APIs parser 
 *                   function
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _RRM_OAMNB_IL_PARSER_H_
#define _RRM_OAMNB_IL_PARSER_H_

#include "oam_types.h"
#include "rrm_oam_intf.h"
#include "rrm_oamNb_intf.h"
oam_return_et
oam_parse_rrm_oam_nb_cell_config_req
(
    rrm_oam_nb_cell_config_req_t *p_rrm_oam_nb_cell_config_req,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_return_et
oam_parse_rrm_oam_nb_cell_config_resp
(
    rrm_oam_nb_cell_config_resp_t *p_rrm_oam_nb_cell_config_resp,
    U8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

#endif /* _RRM_OAMNB_IL_PARSER_H_ */
