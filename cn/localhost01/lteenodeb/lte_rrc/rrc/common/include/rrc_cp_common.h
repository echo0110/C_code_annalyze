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
 *  File Description : The file rrc_cp_common.h contains the prototypes 
 *                     for composing/parsing basic data types.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_CP_COMMON_H_
#define _RRC_CP_COMMON_H_

#include "rrc_defines.h"

rrc_return_et
rrc_cp_pack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrc_return_et
rrc_cp_pack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrc_return_et
rrc_cp_pack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrc_return_et
rrc_cp_pack_U32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrc_return_et
rrc_cp_pack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrc_return_et
rrc_cp_pack_S16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrc_return_et
rrc_cp_pack_S32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrc_return_et
rrc_cp_unpack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrc_return_et
rrc_cp_unpack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrc_return_et
rrc_cp_unpack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrc_return_et
rrc_cp_unpack_U32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrc_return_et
rrc_cp_unpack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrc_return_et
rrc_cp_unpack_S16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrc_return_et
rrc_cp_unpack_S32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

#endif /* _RRC_CP_COMMON_H_ */

