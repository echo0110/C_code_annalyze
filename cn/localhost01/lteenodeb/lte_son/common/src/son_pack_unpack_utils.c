/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_pack_unpack_utils.c $
 *
 *******************************************************************************
 *
 * File Description: File son_pack_unpack_utils.c added for SON module code
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#include "son_pack_unpack_utils.h"
#include "son_utils.h"
extern const son_8 *son_utils_facility_name;

son_return_et
son_cp_pack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %d", varname, *((U8 *)p_src));

    SON_MEMCPY(p_dest, p_src, sizeof(U8));

    return (SON_SUCCESS);
}

son_return_et
son_cp_pack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing U16 */

    ((U8 *)p_dest)[0]    = (U8)((*(U16 *)p_src) >> 8);
    ((U8 *)p_dest)[1]    = (U8)((*(U16 *)p_src));

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %d", varname, *((U16 *)p_src));

    return (SON_SUCCESS);
}

son_return_et
son_cp_pack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing U64 */

    ((U8 *)p_dest)[0]    = (U8)((*(U64 *)p_src) >> 56);
    ((U8 *)p_dest)[1]    = (U8)((*(U64 *)p_src) >> 48);
    ((U8 *)p_dest)[2]    = (U8)((*(U64 *)p_src) >> 40);
    ((U8 *)p_dest)[3]    = (U8)((*(U64 *)p_src) >> 32);
    ((U8 *)p_dest)[4]    = (U8)((*(U64 *)p_src) >> 24);
    ((U8 *)p_dest)[5]    = (U8)((*(U64 *)p_src) >> 16);
    ((U8 *)p_dest)[6]    = (U8)((*(U64 *)p_src) >> 8);
    ((U8 *)p_dest)[7]    = (U8)((*(U64 *)p_src));

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %llu", varname, *((U64 *)p_src));

    return (SON_SUCCESS);
}

son_return_et
son_cp_pack_U32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing U32 */

    ((U8 *)p_dest)[0]    = (U8)((*(U32 *)p_src) >> 24);
    ((U8 *)p_dest)[1]    = (U8)((*(U32 *)p_src) >> 16);
    ((U8 *)p_dest)[2]    = (U8)((*(U32 *)p_src) >> 8);
    ((U8 *)p_dest)[3]    = (U8)((*(U32 *)p_src));

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %d", varname, *((U32 *)p_src));

    return (SON_SUCCESS);
}

son_return_et
son_cp_pack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %d", varname, *((S8 *)p_src));

    SON_MEMCPY(p_dest, p_src, sizeof(S8));

    return (SON_SUCCESS);
}

son_return_et
son_cp_pack_S16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing S16 */

    ((U8 *)p_dest)[0]    = (U8)((*(S16 *)p_src) >> 8);
    ((U8 *)p_dest)[1]    = (U8)((*(S16 *)p_src));

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %d", varname, *((S16 *)p_src));

    return (SON_SUCCESS);
}

son_return_et
son_cp_pack_S32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing S32 */

    ((U8 *)p_dest)[0]    = (U8)((*(S32 *)p_src) >> 24);
    ((U8 *)p_dest)[1]    = (U8)((*(S32 *)p_src) >> 16);
    ((U8 *)p_dest)[2]    = (U8)((*(S32 *)p_src) >> 8);
    ((U8 *)p_dest)[3]    = (U8)((*(S32 *)p_src));

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %d", varname, *((S32 *)p_src));

    return (SON_SUCCESS);
}

son_return_et
son_cp_unpack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    SON_MEMCPY(p_dest, p_src, sizeof(U8));

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %d", varname, *((U8 *)p_dest));

    return (SON_SUCCESS);
}

son_return_et
son_cp_unpack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking U16 */

    *(U16 *)p_dest  = 0;

    *(U16 *)p_dest  |= (U16)((U8 *)p_src)[0] << 8;
    *(U16 *)p_dest  |= (U16)((U8 *)p_src)[1];

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %d", varname, *((U16 *)p_dest));

    return (SON_SUCCESS);
}

son_return_et
son_cp_unpack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking U64 */

    *(U64 *)p_dest  = 0;

    *(U64 *)p_dest  |= (U64)((U8 *)p_src)[0] << 56;
    *(U64 *)p_dest  |= (U64)((U8 *)p_src)[1] << 48;
    *(U64 *)p_dest  |= (U64)((U8 *)p_src)[2] << 40;
    *(U64 *)p_dest  |= (U64)((U8 *)p_src)[3] << 32;
    *(U64 *)p_dest  |= (U64)((U8 *)p_src)[4] << 24;
    *(U64 *)p_dest  |= (U64)((U8 *)p_src)[5] << 16;
    *(U64 *)p_dest  |= (U64)((U8 *)p_src)[6] << 8;
    *(U64 *)p_dest  |= (U64)((U8 *)p_src)[7];

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %llu", varname, *((U64 *)p_dest));

    return (SON_SUCCESS);
} /* son_cp_unpack_U64 */

son_return_et
son_cp_unpack_U32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking U32 */

    *(U32 *)p_dest  = 0;

    *(U32 *)p_dest  |= (U32)((U8 *)p_src)[0] << 24;
    *(U32 *)p_dest  |= (U32)((U8 *)p_src)[1] << 16;
    *(U32 *)p_dest  |= (U32)((U8 *)p_src)[2] << 8;
    *(U32 *)p_dest  |= (U32)((U8 *)p_src)[3];

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %d", varname, *((U32 *)p_dest));

    return (SON_SUCCESS);
}

son_return_et
son_cp_unpack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    SON_MEMCPY(p_dest, p_src, sizeof(S8));

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %d", varname, *((S8 *)p_dest));

    return (SON_SUCCESS);
}

son_return_et
son_cp_unpack_S16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking S16 */

    *(S16 *)p_dest  = 0;

    *(S16 *)p_dest  |= (S16)((U8 *)p_src)[0] << 8;
    *(S16 *)p_dest  |= (S16)((U8 *)p_src)[1];

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %d", varname, *((S16 *)p_dest));

    return (SON_SUCCESS);
}

son_return_et
son_cp_unpack_S32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking S32 */

    *(S32 *)p_dest  = 0;

    *(S32 *)p_dest  |= (S32)((U8 *)p_src)[0] << 24;
    *(S32 *)p_dest  |= (S32)((U8 *)p_src)[1] << 16;
    *(S32 *)p_dest  |= (S32)((U8 *)p_src)[2] << 8;
    *(S32 *)p_dest  |= (S32)((U8 *)p_src)[3];

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_COMP_PARSE, "%s = %d", varname, *((S32 *)p_dest));

    return (SON_SUCCESS);
}
