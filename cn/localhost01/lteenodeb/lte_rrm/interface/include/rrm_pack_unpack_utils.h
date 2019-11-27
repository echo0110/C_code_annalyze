/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_pack_unpack_utils.h $
 *
 *******************************************************************************
 *
 * File Description: Pack and Unpack function declaration header file.
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _RRM_PACK_UNPACK_UTILS_H_
#define _RRM_PACK_UNPACK_UTILS_H_

#include "rrm_defines.h"

rrm_return_et
rrm_cp_pack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_pack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_pack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_pack_U32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_pack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_pack_S16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_pack_S32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_unpack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_unpack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_unpack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_unpack_U32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_unpack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_unpack_S16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_unpack_S32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

#endif /* _RRM_PACK_UNPACK_UTILS_H_ */

