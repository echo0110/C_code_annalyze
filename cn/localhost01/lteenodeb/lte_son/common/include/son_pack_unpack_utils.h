/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_pack_unpack_utils.h $
 *
 *******************************************************************************
 *
 * File Description: File son_pack_unpack_utils.h added for SON module code
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _SON_PACK_UNPACK_UTILS_H_
#define _SON_PACK_UNPACK_UTILS_H_

#include "son_defines.h"

son_return_et
son_cp_pack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

son_return_et
son_cp_pack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

son_return_et
son_cp_pack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

son_return_et
son_cp_pack_U32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

son_return_et
son_cp_pack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

son_return_et
son_cp_pack_S16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

son_return_et
son_cp_pack_S32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

son_return_et
son_cp_unpack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

son_return_et
son_cp_unpack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

son_return_et
son_cp_unpack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

son_return_et
son_cp_unpack_U32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

son_return_et
son_cp_unpack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

son_return_et
son_cp_unpack_S16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

son_return_et
son_cp_unpack_S32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

#endif /* _SON_PACK_UNPACK_UTILS_H_ */
