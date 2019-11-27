/***************************************************************************
 **
 **  ARICENT -
 **
 **  Copyright (c) 2009 Aricent.
 **
 *****************************************************************************
 ** File Details
 ** ------------
 **  $Id: rrm_cp_common.c,v 1.1.2.1.8.1 2010/10/15 13:42:46 gur20491 Exp $
 *****************************************************************************
 **
 **  File Description :This file contains the common pack unpack functions
 **
 *****************************************************************************
 **
 ** Revision Details
 ** ----------------
 ** $Log: rrm_cp_common.c,v $
 ** Revision 1.1.2.1.8.1  2010/10/15 13:42:46  gur20491
 ** Changes for RRC 2.0.1 Compliance
 **
 ** Revision 1.1.2.1  2010/08/02 12:56:55  gur20491
 ** FrameworkSetupWith_RRC1.0_MAC1.2.4_NEW_OAM_RRM
 **
 ** 2012/08/09  15:38:00   gur27278   Removed cross compilation warnings
 **
 **
 *****************************************************************************/

 
/****************************************************************************
* Standard Library Includes
****************************************************************************/


/****************************************************************************
* Project Includes
****************************************************************************/

#include "rrm_utils.h"
#include "l3_api_compose_parse_utils.h"

S8 *p_rrc_intf_facility_name = (S8 *)"RRM_RRC";
/****************************************************************************
*  Private Definitions
****************************************************************************/
				  
/***************************************************************************
*	Private Types
****************************************************************************/

/****************************************************************************
* Private Function Prototypes
****************************************************************************/


 /****************************************************************************
  * * Function Name  : rrm_cp_pack_U8
  * * Inputs         : pointer to src, variable name
  * * Outputs        : pointer to dest
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : packs U8 value to dest.
  *****************************************************************************/

rrm_return_et
rrm_cp_pack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_COMP_PARSE, "Value of U8 %s = %d", varname, *((U8 *)p_src));

    RRM_MEMCPY(p_dest, p_src, sizeof(U8));

    return(RRM_SUCCESS);
}

 /****************************************************************************
  * * Function Name  : rrm_cp_pack_U16
  * * Inputs         : pointer to source, variable name
  * * Outputs        : pointer to destination
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : packs U16 value to dest.
  *****************************************************************************/
rrm_return_et
rrm_cp_pack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing U16 */

    ((U8*)p_dest)[0]    = (U8)((*(U16 *)p_src) >> 8);
    ((U8*)p_dest)[1]    = (U8)((*(U16 *)p_src));

    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_COMP_PARSE, "Value of U16 %s = %d", varname, *((U16 *)p_src));

    return(RRM_SUCCESS);
}

 /****************************************************************************
  * * Function Name  : rrm_cp_pack_U64
  * * Inputs         : pointer to source, variable name
  * * Outputs        : pointer to destination 
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : packs U64 value to dest.
  *****************************************************************************/
rrm_return_et
rrm_cp_pack_U64
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

    /* SPR_17377_Warning_fix + */
    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_DETAILED, "\nValue of U64 %s = %llu", varname, *((U64 *)p_src));
    /* SPR_17377_Warning_fix - */

    return(RRM_SUCCESS);
}

 /****************************************************************************
  * * Function Name  : rrm_cp_pack_U32
  * * Inputs         : pointer to source, variable name
  * * Outputs        : pointer to destination
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : packs U32 value to dest.
  *****************************************************************************/
rrm_return_et
rrm_cp_pack_U32
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

    /* SPR 18994 Fix Start */
    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_COMP_PARSE, "Value of U32 %s = %u", varname, *((U32 *)p_src));
    /* SPR 18994 Fix End */
    return(RRM_SUCCESS);
}

 /****************************************************************************
  * * Function Name  : rrm_cp_pack_S8
  * * Inputs         : pointer to source, variable name
  * * Outputs        : pointer to destination
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : packs S8 value to dest.
  *****************************************************************************/
rrm_return_et
rrm_cp_pack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_COMP_PARSE, "Value of S8 %s = %d", varname, *((S8 *)p_src));

    RRM_MEMCPY(p_dest, p_src, sizeof(S8));

    return(RRM_SUCCESS);
}

 /****************************************************************************
  * * Function Name  : rrm_cp_pack_S16
  * * Inputs         : pointer to source, variable name
  * * Outputs        : pointer to destination
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : packs S16 value to dest.
  *****************************************************************************/
rrm_return_et
rrm_cp_pack_S16
(
    void    *p_dest,
    void    *p_src,
    S8    *varname
)
{
    /* Packing S16 */

    ((S8*)p_dest)[0]    = (S8)((*(S16 *)p_src) >> 8);
    ((S8*)p_dest)[1]    = (S8)((*(S16 *)p_src));

    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_COMP_PARSE, "Value of S16 %s = %d", varname, *((S16 *)p_src));

    return(RRM_SUCCESS);
}

 /****************************************************************************
  * * Function Name  : rrm_cp_pack_S32
  * * Inputs         : pointer to source, variable name
  * * Outputs        : pointer to destination
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : packs S32 value to dest.
  *****************************************************************************/
rrm_return_et
rrm_cp_pack_S32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing S32 */

    ((S8*)p_dest)[0]    = (S8)((*(S32 *)p_src) >> 24);
    ((S8*)p_dest)[1]    = (S8)((*(S32 *)p_src) >> 16);
    ((S8*)p_dest)[2]    = (S8)((*(S32 *)p_src) >> 8);
    ((S8*)p_dest)[3]    = (S8)((*(S32 *)p_src));

    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_COMP_PARSE, "Value of S32 %s = %d", varname, *((S32 *)p_src));

    return(RRM_SUCCESS);
}

 /****************************************************************************
  * * Function Name  : rrm_cp_unpack_U8
  * * Inputs         : pointer to source, variable name
  * * Outputs        : pointer to destination
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : unpacks U8 value to dest.
  *****************************************************************************/
rrm_return_et
rrm_cp_unpack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    RRM_MEMCPY(p_dest, p_src, sizeof(U8));

    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_COMP_PARSE, "Value of U8 %s = %d", varname, *((U8 *)p_dest));

    return(RRM_SUCCESS);
}

 /****************************************************************************
  * * Function Name  : rrm_cp_unpack_U16
  * * Inputs         : pointer to source, variable name
  * * Outputs        : pointer to destination
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : unpacks U16 value to dest.
  *****************************************************************************/
rrm_return_et
rrm_cp_unpack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking U16 */

    *(U16 *)p_dest  = 0;

    *(U16 *)p_dest = (U16)(*(U16 *)p_dest  | (U16)(((U8*)p_src)[0] << 8));
    *(U16 *)p_dest = (U16)(*(U16 *)p_dest  | (U16)((U8*)p_src)[1]);

    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_COMP_PARSE, "Value of U16 %s = %d", varname, *((U16 *)p_dest));

    return(RRM_SUCCESS);
}

 /****************************************************************************
  * * Function Name  : rrm_cp_unpack_U64
  * * Inputs         : pointer to source, variable name
  * * Outputs        : pointer to destination
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : unpacks U64 value to dest.
  *****************************************************************************/
rrm_return_et
rrm_cp_unpack_U64
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

    /* SPR_17377_Warning_fix + */
    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_DETAILED, "Value of U64 %s = %llu", varname, *((U64 *)p_dest));
    /* SPR_17377_Warning_fix - */

    return(RRM_SUCCESS);
}

 /****************************************************************************
  * * Function Name  : rrm_cp_unpack_U32
  * * Inputs         : pointer to source, variable name
  * * Outputs        : pointer to destination
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : unpacks U32 value to dest.
  *****************************************************************************/
rrm_return_et
rrm_cp_unpack_U32
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
     /* SPR 18994 Fix Start */
    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_COMP_PARSE, "Value of U32 %s = %u", varname, *((U32 *)p_dest));
     /* SPR 18994 Fix End */
    return(RRM_SUCCESS);
}

 /****************************************************************************
  * * Function Name  : rrm_cp_unpack_S8
  * * Inputs         : pointer to source, variable name
  * * Outputs        : pointer to destination
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : unpacks S8 value to dest.
  *****************************************************************************/
rrm_return_et
rrm_cp_unpack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    RRM_MEMCPY(p_dest, p_src, sizeof(S8));

    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_COMP_PARSE, "Value of S8 %s = %d", varname, *((S8 *)p_dest));

    return(RRM_SUCCESS);
}

 /****************************************************************************
  * * Function Name  : rrm_cp_unpack_S16
  * * Inputs         : pointer to source, variable name
  * * Outputs        : pointer to destination
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : unpacks S16 value to dest.
  *****************************************************************************/
rrm_return_et
rrm_cp_unpack_S16
(
    void    *p_dest,
    void    *p_src,
    S8    *varname
)
{
    /* Unpacking S16 */

    *(S16 *)p_dest  = 0;

    *(S16 *)p_dest = (S16)(*(S16 *)p_dest  | (S16)(((U8*)p_src)[0] << 8));
    *(S16 *)p_dest = (S16)(*(S16 *)p_dest  | (S16)((U8*)p_src)[1]);

    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_COMP_PARSE, "Value of S16 %s = %d", varname, *((S16 *)p_dest));

    return(RRM_SUCCESS);
}

 /****************************************************************************
  * * Function Name  : rrm_cp_unpack_S32
  * * Inputs         : pointer to source, variable name
  * * Outputs        : pointer to destination
  * * Returns        : RRM_SUCCESS or RRM_FAILURE
  * * Description    : unpacks S32 value to dest.
  *****************************************************************************/
rrm_return_et
rrm_cp_unpack_S32
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

    RRM_TRACE(RRM_TRUE, p_rrc_intf_facility_name, RRM_COMP_PARSE, "Value of S32 %s = %d", varname, *((S32 *)p_dest));

    return(RRM_SUCCESS);
}





 /****************************************************************************
  * * Function Name  : rrc_assert
  * * Inputs         : file pointer, line no, expression pointer
  * * Outputs        : None
  * * Returns        : void 
  * * Description    : Assertion function to check a pointer content
  *****************************************************************************/
#ifdef RRM_DEBUG
 void rrc_assert(
     void* file,
     U32 line,
     void* expression)
 {
     qvPanic("[%s:%u] Assertion Failed <%s>", (U8*)file, line, (U8*)expression);
 }
 #endif /* RRM_DEBUG */

//#ifndef LINUX_PC_TEST
#define RRC_TRACE
#define RRC_MEMCPY
#define RRC_COMP_PARSE  0
//#endif

#ifndef _MSC_VER
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
		 void* p_dest,
		 void* p_src,
		 char* varname
	 )
 {
	 RRC_TRACE(RRC_COMP_PARSE, "Value of U8 %s = %d", varname, *((U8*)p_src));

	 RRC_MEMCPY(p_dest, p_src, sizeof(U8));

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
		 void* p_dest,
		 void* p_src,
		 char* varname
	 )
 {
	 /* Packing U16 */

	 ((U8*)p_dest)[0] = (U8)((*(U16*)p_src) >> 8);
	 ((U8*)p_dest)[1] = (U8)((*(U16*)p_src));

	 RRC_TRACE(RRC_COMP_PARSE, "Value of U16 %s = %d", varname, *((U16*)p_src));

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
		 void* p_dest,
		 void* p_src,
		 char* varname
	 )
 {
	 /* Packing U32 */

	 ((U8*)p_dest)[0] = (U8)((*(U32*)p_src) >> 24);
	 ((U8*)p_dest)[1] = (U8)((*(U32*)p_src) >> 16);
	 ((U8*)p_dest)[2] = (U8)((*(U32*)p_src) >> 8);
	 ((U8*)p_dest)[3] = (U8)((*(U32*)p_src));

	 RRC_TRACE(RRC_COMP_PARSE, "Value of U32 %s = %u", varname, *((U32*)p_src));

	 return(RRC_SUCCESS);
 }
#endif //_MSC_VER