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
 *  File Description : The file rrc_cp_common.c contains the definitions
 *                     of functions used for composing/parsing basic data 
 *                     types.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#include "rrc_cp_common.h"
#include "rrc_common_utils.h"

/****************************************************************************
 * FUNCTION       : rrc_cp_pack_U8
 * INPUT          : void    *p_dest,
 *                  void    *p_src,
 *                  char    *varname
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function packs 1 Byte of unsigned char type
                    from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_pack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    RRC_TRACE(RRC_COMP_PARSE, "%s = %d", varname, *((U8 *)p_src));

    l3_memcpy_wrapper(p_dest, p_src, sizeof(U8));

    return(RRC_SUCCESS);
}

/****************************************************************************
 * FUNCTION       : rrc_cp_pack_U16
 * INPUT          : void    *p_dest,
 *                  void    *p_src,
 *                  char    *varname
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function packs 2 Byte of unsigned char type
 *                  from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_pack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing U16 */

    ((U8*)p_dest)[0]    = (U8)((*(U16 *)p_src) >> 8);
    ((U8*)p_dest)[1]    = (U8)((*(U16 *)p_src));

    RRC_TRACE(RRC_COMP_PARSE, "%s = %d", varname, *((U16 *)p_src));

    return(RRC_SUCCESS);
}

/****************************************************************************
 * FUNCTION       : rrc_cp_pack_U64
 * INPUT          : void    *p_dest,
 *                  void    *p_src,
 *                  char    *varname
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function packs 8 Byte of unsigned char type
 *                  from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_pack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{

#ifdef LTE_L3_BIG_ENDIAN
    *(U64 *)p_dest = *(U64 *)p_src;
 
#elif LTE_L3_LITTLE_ENDIAN
    /* Packing U64 */

    ((U8*)p_dest)[0]    = (U8)((*(U64 *)p_src) >> 56);
    ((U8*)p_dest)[1]    = (U8)((*(U64 *)p_src) >> 48);
    ((U8*)p_dest)[2]    = (U8)((*(U64 *)p_src) >> 40);
    ((U8*)p_dest)[3]    = (U8)((*(U64 *)p_src) >> 32);
    ((U8*)p_dest)[4]    = (U8)((*(U64 *)p_src) >> 24);
    ((U8*)p_dest)[5]    = (U8)((*(U64 *)p_src) >> 16);
    ((U8*)p_dest)[6]    = (U8)((*(U64 *)p_src) >> 8);
    ((U8*)p_dest)[7]    = (U8)((*(U64 *)p_src));
#endif

    RRC_TRACE(RRC_COMP_PARSE, "%s = %llu", varname, *((U64 *)p_src));

    return(RRC_SUCCESS);
}

/****************************************************************************
 * FUNCTION       : rrc_cp_pack_U32
 * INPUT          : void    *p_dest,
 *                  void    *p_src,
 *                  char    *varname
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function packs 4 Byte of unsigned char type
 *                  from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_pack_U32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing U32 */

    ((U8*)p_dest)[0]    = (U8)((*(U32 *)p_src) >> 24);
    ((U8*)p_dest)[1]    = (U8)((*(U32 *)p_src) >> 16);
    ((U8*)p_dest)[2]    = (U8)((*(U32 *)p_src) >> 8);
    ((U8*)p_dest)[3]    = (U8)((*(U32 *)p_src));

    RRC_TRACE(RRC_COMP_PARSE, "%s = %u", varname, *((U32 *)p_src));

    return(RRC_SUCCESS);
}

/****************************************************************************
 * FUNCTION       : rrc_cp_pack_S8
 * INPUT          : void    *p_dest,
 *                  void    *p_src,
 *                  char    *varname
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function packs 1 Byte of signed char type
 *                  from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_pack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    RRC_TRACE(RRC_COMP_PARSE, "%s = %d", varname, *((S8 *)p_src));

    l3_memcpy_wrapper(p_dest, p_src, sizeof(S8));

    return(RRC_SUCCESS);
}

/****************************************************************************
 * FUNCTION       : rrc_cp_pack_S16
 * INPUT          : void    *p_dest,
 *                  void    *p_src,
 *                  char    *varname
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function packs 2 Byte of signed char type
 *                  from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_pack_S16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing S16 */

    ((U8*)p_dest)[0]    = (U8)((*(S16 *)p_src) >> 8);
    ((U8*)p_dest)[1]    = (U8)((*(S16 *)p_src));

    RRC_TRACE(RRC_COMP_PARSE, "%s = %d", varname, *((S16 *)p_src));

    return(RRC_SUCCESS);
}

/****************************************************************************
 * FUNCTION       : rrc_cp_pack_S32
 * INPUT          : void    *p_dest,
 *                  void    *p_src,
 *                  char    *varname
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function packs 4 Byte of signed char type
 *                  from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_pack_S32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing S32 */

    ((U8*)p_dest)[0]    = (U8)((*(S32 *)p_src) >> 24);
    ((U8*)p_dest)[1]    = (U8)((*(S32 *)p_src) >> 16);
    ((U8*)p_dest)[2]    = (U8)((*(S32 *)p_src) >> 8);
    ((U8*)p_dest)[3]    = (U8)((*(S32 *)p_src));

    RRC_TRACE(RRC_COMP_PARSE, "%s = %d", varname, *((S32 *)p_src));

    return(RRC_SUCCESS);
}

/****************************************************************************
 * FUNCTION       : rrc_cp_unpack_U8
 * INPUT          : void    *p_dest,
 *                  void    *p_src,
 *                  char    *varname
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function unpacks 1 Byte of unsigned char type
 *                  from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_unpack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    l3_memcpy_wrapper(p_dest, p_src, sizeof(U8));

    RRC_TRACE(RRC_COMP_PARSE, "%s = %d", varname, *((U8 *)p_dest));

    return(RRC_SUCCESS);
}

/****************************************************************************
 * FUNCTION       : rrc_cp_unpack_U16
 * INPUT          : void    *p_dest,
 *                  void    *p_src,
 *                  char    *varname
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function unpacks 2 Byte of unsigned char type
 *                  from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_unpack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking U16 */

    *(U16 *)p_dest  = 0;

    *(U16 *)p_dest  = (U16)(*(U16 *)p_dest | ((U8*)p_src)[0] << 8);
    *(U16 *)p_dest  = (U16)(*(U16 *)p_dest | ((U8*)p_src)[1]);

    RRC_TRACE(RRC_COMP_PARSE, "%s = %d", varname, *((U16 *)p_dest));

    return(RRC_SUCCESS);
}

/****************************************************************************
 * FUNCTION       : rrc_cp_unpack_U64
 * INPUT          : void    *p_dest,
                    void    *p_src,
                    char    *varname
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function unpacks 8 Byte of unsigned char type
 *                  from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_unpack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{

#ifdef LTE_L3_BIG_ENDIAN
    *(U64 *)p_dest = *(U64 *)p_src;

#elif LTE_L3_LITTLE_ENDIAN
    /* Unpacking U64 */

    *(U64 *)p_dest  = 0;

    *(U64 *)p_dest  |= (U64)((U8*)p_src)[0] << 56;
    *(U64 *)p_dest  |= (U64)((U8*)p_src)[1] << 48;
    *(U64 *)p_dest  |= (U64)((U8*)p_src)[2] << 40;
    *(U64 *)p_dest  |= (U64)((U8*)p_src)[3] << 32;
    *(U64 *)p_dest  |= (U64)((U8*)p_src)[4] << 24;
    *(U64 *)p_dest  |= (U64)((U8*)p_src)[5] << 16;
    *(U64 *)p_dest  |= (U64)((U8*)p_src)[6] << 8;
    *(U64 *)p_dest  |= (U64)((U8*)p_src)[7];
#endif

    RRC_TRACE(RRC_COMP_PARSE, "%s = %llu", varname, *((U64 *)p_dest));

    return(RRC_SUCCESS);
}

/****************************************************************************
 * FUNCTION       : rrc_cp_unpack_U32
 * INPUT          : void    *p_dest,
 *                  void    *p_src,
 *                  char    *varname
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function unpacks 4 Byte of unsigned char type
 *                  from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_unpack_U32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking U32 */

    *(U32 *)p_dest  = 0;

    *(U32 *)p_dest  |= (U32)((U8*)p_src)[0] << 24;
    *(U32 *)p_dest  |= (U32)((U8*)p_src)[1] << 16;
    *(U32 *)p_dest  |= (U32)((U8*)p_src)[2] << 8;
    *(U32 *)p_dest  |= (U32)((U8*)p_src)[3];

    RRC_TRACE(RRC_COMP_PARSE, "%s = %u", varname, *((U32 *)p_dest));

    return(RRC_SUCCESS);
}

/****************************************************************************
 * FUNCTION       : rrc_cp_unpack_S8
 * INPUT          : void    *p_dest,
 *                  void    *p_src,
 *                  char    *varname
 *
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function unpacks 1 Byte of signed char type
 *                  from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_unpack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    l3_memcpy_wrapper(p_dest, p_src, sizeof(S8));

    RRC_TRACE(RRC_COMP_PARSE, "%s = %d", varname, *((S8 *)p_dest));

    return(RRC_SUCCESS);
}

/****************************************************************************
 * FUNCTION       : rrc_cp_unpack_S16
 * INPUT          : void    *p_dest,
 *                  void    *p_src,
 *                  char    *varname
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function unpacks 2 Byte of signed char type
 *                  from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_unpack_S16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking S16 */

    *(S16 *)p_dest  = 0;

    *(S16 *)p_dest  = (S16)(*(S16 *)p_dest | ((U8*)p_src)[0] << 8);
    *(S16 *)p_dest  = (S16)(*(S16 *)p_dest |((U8*)p_src)[1]);

    RRC_TRACE(RRC_COMP_PARSE, "%s = %d", varname, *((S16 *)p_dest));

    return(RRC_SUCCESS);
}

/****************************************************************************
 * FUNCTION       : rrc_cp_unpack_S32
 * INPUT          : void    *p_dest,
 *                  void    *p_src,
 *                  char    *varname
 * OUTPUT         : None
 * RETURN         : RRC_SUCCESS
 * DESCRIPTION    : This Function unpacks 4 Byte of signed char type
 *                  from p_src to p_dest.
 ****************************************************************************/
rrc_return_et
rrc_cp_unpack_S32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking S32 */

    *(S32 *)p_dest  = 0;

    *(S32 *)p_dest  |= (S32)((U8*)p_src)[0] << 24;
    *(S32 *)p_dest  |= (S32)((U8*)p_src)[1] << 16;
    *(S32 *)p_dest  |= (S32)((U8*)p_src)[2] << 8;
    *(S32 *)p_dest  |= (S32)((U8*)p_src)[3];

    RRC_TRACE(RRC_COMP_PARSE, "%s = %d", varname, *((S32 *)p_dest));

    return(RRC_SUCCESS);
}

