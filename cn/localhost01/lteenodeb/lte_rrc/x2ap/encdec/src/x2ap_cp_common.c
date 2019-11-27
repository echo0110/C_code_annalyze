/*******************************************************************************
 *
 *  FILE NAME   : x2ap_cp_common.c
 *
 *  DESCRIPTION : 
 *
 *  Copyright 2009, Aricent Inc.
 *
 *******************************************************************************/
#include "x2ap_types.h"
#include "commoninc.h"
#include "rrc_x2apOam_intf.h"
#include "x2ap_cp_common.h"
#include "x2ap_utils.h"


/*****************************************************************************
 * Function Name  : x2ap_cp_pack_U8 
 * Inputs         : p_dest - pointer to destination
 *                  p_src - source
 *                  varname - variable name 
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE 
 * Description    : this function  copies and packs a unsigned char from
 *                  p_src to p_dest 
 ********************************************************************************/
x2ap_return_et
x2ap_cp_pack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    X2AP_TRACE(X2AP_DETAILED, "Value of U8 %s = %d", varname, *((U8 *)p_src));

    X2AP_MEMCPY(p_dest, p_src, sizeof(U8));

    return(X2AP_SUCCESS);
}

/*****************************************************************************
 * Function Name  : x2ap_cp_pack_U16 
 * Inputs         : p_dest - pointer to destination
 *                  p_src - source
 *                  varname - variable
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function  copies and packs a unsigned short int from
 *                   p_src to p_dest
 ********************************************************************************/
x2ap_return_et
x2ap_cp_pack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing U16 */

    ((U8*)p_dest)[0]    = (U8)((*(U16 *)p_src) >> 8);
    ((U8*)p_dest)[1]    = (U8)((*(U16 *)p_src));

    X2AP_TRACE(X2AP_DETAILED, "Value of U16 %s = %d", varname, *((U16 *)p_src));

    return(X2AP_SUCCESS);
}

/*****************************************************************************
 * Function Name  : x2ap_cp_pack_U64 
 * Inputs         : p_dest - pointer to destination
 *                  p_src  - source
 *                  varnam - variable
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function copies and packs a unsigned long long from 
 *                  p_src to p_dest 
 ********************************************************************************/
x2ap_return_et
x2ap_cp_pack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing U64 */

    ((U8*)p_dest)[0]    = (U8)((*(U64 *)p_src) >> 56);
    ((U8*)p_dest)[1]    = (U8)((*(U64 *)p_src) >> 48);
    ((U8*)p_dest)[2]    = (U8)((*(U64 *)p_src) >> 40);
    ((U8*)p_dest)[3]    = (U8)((*(U64 *)p_src) >> 32);
    ((U8*)p_dest)[4]    = (U8)((*(U64 *)p_src) >> 24);
    ((U8*)p_dest)[5]    = (U8)((*(U64 *)p_src) >> 16);
    ((U8*)p_dest)[6]    = (U8)((*(U64 *)p_src) >> 8);
    ((U8*)p_dest)[7]    = (U8)((*(U64 *)p_src));

    X2AP_TRACE(X2AP_DETAILED, "Value of U64 %s = %llu", varname, *((U64 *)p_src));

    return(X2AP_SUCCESS);
}

/*****************************************************************************
 * Function Name  : x2ap_cp_pack_U32 
 * Inputs         : p_dest - destination
 *                  p_src- source
 *                  varname - variable
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function copies and packs a unsigned int from p_src
 *                  to p_dest
 ********************************************************************************/
x2ap_return_et
x2ap_cp_pack_U32
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

    X2AP_TRACE(X2AP_DETAILED, "Value of U32 %s = %d", varname, *((U32 *)p_src));

    return(X2AP_SUCCESS);
}

/*****************************************************************************
 * Function Name  : x2ap_cp_pack_S8
 * Inputs         : p_dest - destination
 *                  p_src - source
 *                  varname - variable name 
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function copies and packs a signed char from 
 *                  p_src  to p_dest
 ********************************************************************************/
x2ap_return_et
x2ap_cp_pack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    X2AP_TRACE(X2AP_DETAILED, "Value of S8 %s = %d", varname, *((S8 *)p_src));

    X2AP_MEMCPY(p_dest, p_src, sizeof(S8));

    return(X2AP_SUCCESS);
}

/*****************************************************************************
 * Function Name  : x2ap_cp_pack_S16 
 * Inputs         : p_dest - destination
 *                  p_src - source 
 *                  varname - variable
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function copies and packs a signed short int from  
 *                  p_src  to p_dest
 ********************************************************************************/
x2ap_return_et
x2ap_cp_pack_S16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing S16 */

    ((U8*)p_dest)[0]    = (U8)((*(S16 *)p_src) >> 8);
    ((U8*)p_dest)[1]    = (U8)((*(S16 *)p_src));

    X2AP_TRACE(X2AP_DETAILED, "Value of S16 %s = %d", varname, *((S16 *)p_src));

    return(X2AP_SUCCESS);
}

/*****************************************************************************
 * Function Name  : x2ap_cp_pack_S32 
 * Inputs         : p_dest - destination 
 *                  p_src- source 
 *                  varname - variable name 
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function copies and packs a signed int from p_src  to 
 *                  p_dest
 ********************************************************************************/
x2ap_return_et
x2ap_cp_pack_S32
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

    X2AP_TRACE(X2AP_DETAILED, "Value of S32 %s = %d", varname, *((S32 *)p_src));

    return(X2AP_SUCCESS);
}

/*****************************************************************************
 * Function Name  : x2ap_cp_unpack_U8 
 * Inputs         : p_dest - destinaation
 *                  p_src - source 
 *                  varname - variable name 
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function copies and unpacks a unsigned char from
 *                  p_src  to p_dest 
 ********************************************************************************/
x2ap_return_et
x2ap_cp_unpack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    X2AP_MEMCPY(p_dest, p_src, sizeof(U8));

    X2AP_TRACE(X2AP_DETAILED, "Value of U8 %s = %d", varname, *((U8 *)p_dest));

    return(X2AP_SUCCESS);
}

/*****************************************************************************
 * Function Name  : x2ap_cp_unpack_U16 
 * Inputs         : p_dest - destination
 *                  p_src - source 
 *                  varname - variable name 
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function copies and unpacks a unsigned short int from
 *                  p_src  to p_dest
 ********************************************************************************/
x2ap_return_et
x2ap_cp_unpack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking U16 */

    *(U16 *)p_dest  = 0;

    *(U16 *)p_dest  |= (U16)((U8*)p_src)[0] << 8;
    *(U16 *)p_dest  |= (U16)((U8*)p_src)[1];

    X2AP_TRACE(X2AP_DETAILED, "Value of U16 %s = %d", varname, *((U16 *)p_dest));

    return(X2AP_SUCCESS);
}

/*****************************************************************************
 * Function Name  : x2ap_cp_unpack_U64 
 * Inputs         : p_dest - destination
 *                  p_src - source
 *                  varname - variable name
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function copies and unpacks a unsigned long long from  
 *                  p_src  to p_dest
 ********************************************************************************/
x2ap_return_et
x2ap_cp_unpack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
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

    X2AP_TRACE(X2AP_DETAILED, "Value of U64 %s = %llu", varname, *((U64 *)p_dest));

    return(X2AP_SUCCESS);
}

/*****************************************************************************
 * Function Name  : x2ap_cp_unpack_U32 
 * Inputs         : p_dest - destination 
 *                  p_src - source 
 *                  varname - variable name 
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function copies and unpacks a unsigned int from p_src
 *                   to p_dest
 ********************************************************************************/
x2ap_return_et
x2ap_cp_unpack_U32
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

    X2AP_TRACE(X2AP_DETAILED, "Value of U32 %s = %d", varname, *((U32 *)p_dest));

    return(X2AP_SUCCESS);
}

/*****************************************************************************
 * Function Name  : x2ap_cp_unpack_S8 
 * Inputs         : p_dest - destination
 *                  p_src - source 
 *                  varname - variable name 
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function copies and unpacks a signed char  from p_src
 *                  to p_dest
 ********************************************************************************/
x2ap_return_et
x2ap_cp_unpack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    X2AP_MEMCPY(p_dest, p_src, sizeof(S8));

    X2AP_TRACE(X2AP_DETAILED, "Value of S8 %s = %d", varname, *((S8 *)p_dest));

    return(X2AP_SUCCESS);
}

/*****************************************************************************
 * Function Name  : x2ap_cp_unpack_S16 
 * Inputs         : p_dest - destination 
 *                  p_src - source
 *                  varname - variable name
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function copies and unpacks a signed short int from p_src
 *                  to p_dest
 ********************************************************************************/
x2ap_return_et
x2ap_cp_unpack_S16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking S16 */

    *(S16 *)p_dest  = 0;

    *(S16 *)p_dest  |= (S16)((U8*)p_src)[0] << 8;
    *(S16 *)p_dest  |= (S16)((U8*)p_src)[1];

    X2AP_TRACE(X2AP_DETAILED, "Value of S16 %s = %d", varname, *((S16 *)p_dest));

    return(X2AP_SUCCESS);
}

/*****************************************************************************
 * Function Name  : x2ap_cp_unpack_S32 
 * Inputs         : p_dest - destination
 *                  p_src - source
 *                  varname - variable name
 * Outputs        : p_dest
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function copies and unpacks a signed int from p_src
 *                  to p_dest 
 ********************************************************************************/
x2ap_return_et
x2ap_cp_unpack_S32
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

    X2AP_TRACE(X2AP_DETAILED, "Value of S32 %s = %d", varname, *((S32 *)p_dest));

    return(X2AP_SUCCESS);
}

