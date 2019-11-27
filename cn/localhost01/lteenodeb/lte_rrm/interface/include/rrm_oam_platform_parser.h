/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_oam_platform_parser.h $
 *
 *******************************************************************************
 *
 * File Description: RRM OAM platform parser header file.
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _RRM_OAM_IL_PLAT_PARSER_H_
#define _RRM_OAM_IL_PLAT_PARSER_H_

#include "rrm_defines.h"
#include "rrm_oam_platform_intf.h"

rrm_return_et
rrm_parse_platform_rrm_load_ind
(
    platform_rrm_load_ind_t *p_platform_rrm_load_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

#endif /* _RRM_OAM_IL_PARSER_H_ */
