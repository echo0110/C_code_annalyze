/*****************************************************************************
*
* FUNCTION    : Source file for Generic library - Socket manager
*               implementation.
*
******************************************************************************
*
* FILENAME    : gl_skm_abstractd_wrppr_api.c
*
* DESCRIPTION : This file implements socket management functionality of
*               generic libraries.
*
*   DATE            NAME        REFERENCE           REASON
*-----------    ------------   -----------      ------------------------------
* April'01      Rajesh Jain     New file        Generic Libraries
*
* 18/08/2001    Prashant Jain   SPR 2276        Compiling with CC on Solaris.
*
* 04/09/2001    Prashant Jain   SPR-2560        Adding OSE Operating System
*                                               Support.
* 04/10/2001    Ravi Jindal     SPR-2751        Remove warnings with cc,
*                                               gcc,CC on Solaris.
* 18/10/2001    Ravi Jindal     SPR-2788        Packaging of Genlib-Code.
* 19/10/2001    Ravi Jindal     SPR-2802        Adding Pre-ANSI-C features.
* 17/11/2001    Ravi Jindal     SPR-3107        Incoprate the defects and change
*                                               cosmmetics of file.
*
* 04/04/2002    Ravi Jindal     SPR-3959        Enhancements for Rel-1.0.
*
*Copyright 2001, Hughes Software Systems Ltd.
*****************************************************************************/

#include "gl_skm_interfaces.h"


gl_sm_tcp_udp_lnk_id_info_t *g_a_p_tcp_udp_lnk_id_info = GL_NULL;

gl_sm_tcp_lnk_map_info_t    *g_a_p_tcp_lnk_map_info = GL_NULL;

gl_sm_udp_lnk_map_info_t    g_udp_lnk_map_info;

gl_sm_bit_val_t             g_a_bit_values[256] =
{
    {0,    {0, 0, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,0,0,0,0,0 */
    {1,    {1, 0, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,0,0,0,0,1 */
    {1,    {2, 0, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,0,0,0,1,0 */
    {2,    {1, 2, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,0,0,0,1,1 */
    {1,    {3, 0, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,0,0,1,0,0 */
    {2,    {1, 3, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,0,0,1,0,1 */
    {2,    {2, 3, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,0,0,1,1,0 */
    {3,    {1, 2, 3, 0, 0, 0, 0, 0}}, /* 0,0,0,0,0,1,1,1 */
    {1,    {4, 0, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,0,1,0,0,0 */
    {2,    {1, 4, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,0,1,0,0,1 */
    {2,    {2, 4, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,0,1,0,1,0 */
    {3,    {1, 2, 4, 0, 0, 0, 0, 0}}, /* 0,0,0,0,1,0,1,1 */
    {2,    {3, 4, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,0,1,1,0,0 */
    {3,    {1, 3, 4, 0, 0, 0, 0, 0}}, /* 0,0,0,0,1,1,0,1 */
    {3,    {2, 3, 4, 0, 0, 0, 0, 0}}, /* 0,0,0,0,1,1,1,0 */
    {4,    {1, 2, 3, 4, 0, 0, 0, 0}}, /* 0,0,0,0,1,1,1,1 */

    {1,    {5, 0, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,1,0,0,0,0 */
    {2,    {1, 5, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,1,0,0,0,1 */
    {2,    {2, 5, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,1,0,0,1,0 */
    {3,    {1, 2, 5, 0, 0, 0, 0, 0}}, /* 0,0,0,1,0,0,1,1 */
    {2,    {3, 5, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,1,0,1,0,0 */
    {3,    {1, 3, 5, 0, 0, 0, 0, 0}}, /* 0,0,0,1,0,1,0,1 */
    {3,    {2, 3, 5, 0, 0, 0, 0, 0}}, /* 0,0,0,1,0,1,1,0 */
    {4,    {1, 2, 3, 5, 0, 0, 0, 0}}, /* 0,0,0,1,0,1,1,1 */
    {2,    {4, 5, 0, 0, 0, 0, 0, 0}}, /* 0,0,0,1,1,0,0,0 */
    {3,    {1, 4, 5, 0, 0, 0, 0, 0}}, /* 0,0,0,1,1,0,0,1 */
    {3,    {2, 4, 5, 0, 0, 0, 0, 0}}, /* 0,0,0,1,1,0,1,0 */
    {4,    {1, 2, 4, 5, 0, 0, 0, 0}}, /* 0,0,0,1,1,0,1,1 */
    {3,    {3, 4, 5, 0, 0, 0, 0, 0}}, /* 0,0,0,1,1,1,0,0 */
    {4,    {1, 3, 4, 5, 0, 0, 0, 0}}, /* 0,0,0,1,1,1,0,1 */
    {4,    {2, 3, 4, 5, 0, 0, 0, 0}}, /* 0,0,0,1,1,1,1,0 */
    {5,    {1, 2, 3, 4, 5, 0, 0, 0}}, /* 0,0,0,1,1,1,1,1 */

    {1,    {6, 0, 0, 0, 0, 0, 0, 0}}, /* 0,0,1,0,0,0,0,0 */
    {2,    {1, 6, 0, 0, 0, 0, 0, 0}}, /* 0,0,1,0,0,0,0,1 */
    {2,    {2, 6, 0, 0, 0, 0, 0, 0}}, /* 0,0,1,0,0,0,1,0 */
    {3,    {1, 2, 6, 0, 0, 0, 0, 0}}, /* 0,0,1,0,0,0,1,1 */
    {2,    {3, 6, 0, 0, 0, 0, 0, 0}}, /* 0,0,1,0,0,1,0,0 */
    {3,    {1, 3, 6, 0, 0, 0, 0, 0}}, /* 0,0,1,0,0,1,0,1 */
    {3,    {2, 3, 6, 0, 0, 0, 0, 0}}, /* 0,0,1,0,0,1,1,0 */
    {4,    {1, 2, 3, 6, 0, 0, 0, 0}}, /* 0,0,1,0,0,1,1,1 */
    {2,    {4, 6, 0, 0, 0, 0, 0, 0}}, /* 0,0,1,0,1,0,0,0 */
    {3,    {1, 4, 6, 0, 0, 0, 0, 0}}, /* 0,0,1,0,1,0,0,1 */
    {3,    {2, 4, 6, 0, 0, 0, 0, 0}}, /* 0,0,1,0,1,0,1,0 */
    {4,    {1, 2, 4, 6, 0, 0, 0, 0}}, /* 0,0,1,0,1,0,1,1 */
    {3,    {3, 4, 6, 0, 0, 0, 0, 0}}, /* 0,0,1,0,1,1,0,0 */
    {4,    {1, 3, 4, 6, 0, 0, 0, 0}}, /* 0,0,1,0,1,1,0,1 */
    {4,    {2, 3, 4, 6, 0, 0, 0, 0}}, /* 0,0,1,0,1,1,1,0 */
    {5,    {1, 2, 3, 4, 6, 0, 0, 0}}, /* 0,0,1,0,1,1,1,1 */

    {2,    {5, 6, 0, 0, 0, 0, 0, 0}}, /* 0,0,1,1,0,0,0,0 */
    {3,    {1, 5, 6, 0, 0, 0, 0, 0}}, /* 0,0,1,1,0,0,0,1 */
    {3,    {2, 5, 6, 0, 0, 0, 0, 0}}, /* 0,0,1,1,0,0,1,0 */
    {4,    {1, 2, 5, 6, 0, 0, 0, 0}}, /* 0,0,1,1,0,0,1,1 */
    {3,    {3, 5, 6, 0, 0, 0, 0, 0}}, /* 0,0,1,1,0,1,0,0 */
    {4,    {1, 3, 5, 6, 0, 0, 0, 0}}, /* 0,0,1,1,0,1,0,1 */
    {4,    {2, 3, 5, 6, 0, 0, 0, 0}}, /* 0,0,1,1,0,1,1,0 */
    {5,    {1, 2, 3, 5, 6, 0, 0, 0}}, /* 0,0,1,1,0,1,1,1 */
    {3,    {4, 5, 6, 0, 0, 0, 0, 0}}, /* 0,0,1,1,1,0,0,0 */
    {4,    {1, 4, 5, 6, 0, 0, 0, 0}}, /* 0,0,1,1,1,0,0,1 */
    {4,    {2, 4, 5, 6, 0, 0, 0, 0}}, /* 0,0,1,1,1,0,1,0 */
    {5,    {1, 2, 4, 5, 6, 0, 0, 0}}, /* 0,0,1,1,1,0,1,1 */
    {4,    {3, 4, 5, 6, 0, 0, 0, 0}}, /* 0,0,1,1,1,1,0,0 */
    {5,    {1, 3, 4, 5, 6, 0, 0, 0}}, /* 0,0,1,1,1,1,0,1 */
    {5,    {2, 3, 4, 5, 6, 0, 0, 0}}, /* 0,0,1,1,1,1,1,0 */
    {6,    {1, 2, 3, 4, 5, 6, 0, 0}}, /* 0,0,1,1,1,1,1,1 */



    {1,    {7, 0, 0, 0, 0, 0, 0, 0}}, /* 0,1,0,0,0,0,0,0 */
    {2,    {1, 7, 0, 0, 0, 0, 0, 0}}, /* 0,1,0,0,0,0,0,1 */
    {2,    {2, 7, 0, 0, 0, 0, 0, 0}}, /* 0,1,0,0,0,0,1,0 */
    {3,    {1, 2, 7, 0, 0, 0, 0, 0}}, /* 0,1,0,0,0,0,1,1 */
    {2,    {3, 7, 0, 0, 0, 0, 0, 0}}, /* 0,1,0,0,0,1,0,0 */
    {3,    {1, 3, 7, 0, 0, 0, 0, 0}}, /* 0,1,0,0,0,1,0,1 */
    {3,    {2, 3, 7, 0, 0, 0, 0, 0}}, /* 0,1,0,0,0,1,1,0 */
    {4,    {1, 2, 3, 7, 0, 0, 0, 0}}, /* 0,1,0,0,0,1,1,1 */
    {2,    {4, 7, 0, 0, 0, 0, 0, 0}}, /* 0,1,0,0,1,0,0,0 */
    {3,    {1, 4, 7, 0, 0, 0, 0, 0}}, /* 0,1,0,0,1,0,0,1 */
    {3,    {2, 4, 7, 0, 0, 0, 0, 0}}, /* 0,1,0,0,1,0,1,0 */
    {4,    {1, 2, 4, 7, 0, 0, 0, 0}}, /* 0,1,0,0,1,0,1,1 */
    {3,    {3, 4, 7, 0, 0, 0, 0, 0}}, /* 0,1,0,0,1,1,0,0 */
    {4,    {1, 3, 4, 7, 0, 0, 0, 0}}, /* 0,1,0,0,1,1,0,1 */
    {4,    {2, 3, 4, 7, 0, 0, 0, 0}}, /* 0,1,0,0,1,1,1,0 */
    {5,    {1, 2, 3, 4, 7, 0, 0, 0}}, /* 0,1,0,0,1,1,1,1 */

    {2,    {5, 7, 0, 0, 0, 0, 0, 0}}, /* 0,1,0,1,0,0,0,0 */
    {3,    {1, 5, 7, 0, 0, 0, 0, 0}}, /* 0,1,0,1,0,0,0,1 */
    {3,    {2, 5, 7, 0, 0, 0, 0, 0}}, /* 0,1,0,1,0,0,1,0 */
    {4,    {1, 2, 5, 7, 0, 0, 0, 0}}, /* 0,1,0,1,0,0,1,1 */
    {3,    {3, 5, 7, 0, 0, 0, 0, 0}}, /* 0,1,0,1,0,1,0,0 */
    {4,    {1, 3, 5, 7, 0, 0, 0, 0}}, /* 0,1,0,1,0,1,0,1 */
    {4,    {2, 3, 5, 7, 0, 0, 0, 0}}, /* 0,1,0,1,0,1,1,0 */
    {5,    {1, 2, 3, 5, 7, 0, 0, 0}}, /* 0,1,0,1,0,1,1,1 */
    {3,    {4, 5, 7, 0, 0, 0, 0, 0}}, /* 0,1,0,1,1,0,0,0 */
    {4,    {1, 4, 5, 7, 0, 0, 0, 0}}, /* 0,1,0,1,1,0,0,1 */
    {4,    {2, 4, 5, 7, 0, 0, 0, 0}}, /* 0,1,0,1,1,0,1,0 */
    {5,    {1, 2, 4, 5, 7, 0, 0, 0}}, /* 0,1,0,1,1,0,1,1 */
    {4,    {3, 4, 5, 7, 0, 0, 0, 0}}, /* 0,1,0,1,1,1,0,0 */
    {5,    {1, 3, 4, 5, 7, 0, 0, 0}}, /* 0,1,0,1,1,1,0,1 */
    {5,    {2, 3, 4, 5, 7, 0, 0, 0}}, /* 0,1,0,1,1,1,1,0 */
    {6,    {1, 2, 3, 4, 5, 7, 0, 0}}, /* 0,1,0,1,1,1,1,1 */

    {2,    {6, 7, 0, 0, 0, 0, 0, 0}}, /* 0,1,1,0,0,0,0,0 */
    {3,    {1, 6, 7, 0, 0, 0, 0, 0}}, /* 0,1,1,0,0,0,0,1 */
    {3,    {2, 6, 7, 0, 0, 0, 0, 0}}, /* 0,1,1,0,0,0,1,0 */
    {4,    {1, 2, 6, 7, 0, 0, 0, 0}}, /* 0,1,1,0,0,0,1,1 */
    {3,    {3, 6, 7, 0, 0, 0, 0, 0}}, /* 0,1,1,0,0,1,0,0 */
    {4,    {1, 3, 6, 7, 0, 0, 0, 0}}, /* 0,1,1,0,0,1,0,1 */
    {4,    {2, 3, 6, 7, 0, 0, 0, 0}}, /* 0,1,1,0,0,1,1,0 */
    {5,    {1, 2, 3, 6, 7, 0, 0, 0}}, /* 0,1,1,0,0,1,1,1 */
    {3,    {4, 6, 7, 0, 0, 0, 0, 0}}, /* 0,1,1,0,1,0,0,0 */
    {4,    {1, 4, 6, 7, 0, 0, 0, 0}}, /* 0,1,1,0,1,0,0,1 */
    {4,    {2, 4, 6, 7, 0, 0, 0, 0}}, /* 0,1,1,0,1,0,1,0 */
    {5,    {1, 2, 4, 6, 7, 0, 0, 0}}, /* 0,1,1,0,1,0,1,1 */
    {4,    {3, 4, 6, 7, 0, 0, 0, 0}}, /* 0,1,1,0,1,1,0,0 */
    {5,    {1, 3, 4, 6, 7, 0, 0, 0}}, /* 0,1,1,0,1,1,0,1 */
    {5,    {2, 3, 4, 6, 7, 0, 0, 0}}, /* 0,1,1,0,1,1,1,0 */
    {6,    {1, 2, 3, 4, 6, 7, 0, 0}}, /* 0,1,1,0,1,1,1,1 */

    {3,    {5, 6, 7, 0, 0, 0, 0, 0}}, /* 0,1,1,1,0,0,0,0 */
    {4,    {1, 5, 6, 7, 0, 0, 0, 0}}, /* 0,1,1,1,0,0,0,1 */
    {4,    {2, 5, 6, 7, 0, 0, 0, 0}}, /* 0,1,1,1,0,0,1,0 */
    {5,    {1, 2, 5, 6, 7, 0, 0, 0}}, /* 0,1,1,1,0,0,1,1 */
    {4,    {3, 5, 6, 7, 0, 0, 0, 0}}, /* 0,1,1,1,0,1,0,0 */
    {5,    {1, 3, 5, 6, 7, 0, 0, 0}}, /* 0,1,1,1,0,1,0,1 */
    {5,    {2, 3, 5, 6, 7, 0, 0, 0}}, /* 0,1,1,1,0,1,1,0 */
    {6,    {1, 2, 3, 5, 6, 7, 0, 0}}, /* 0,1,1,1,0,1,1,1 */
    {4,    {4, 5, 6, 7, 0, 0, 0, 0}}, /* 0,1,1,1,1,0,0,0 */
    {5,    {1, 4, 5, 6, 7, 0, 0, 0}}, /* 0,1,1,1,1,0,0,1 */
    {5,    {2, 4, 5, 6, 7, 0, 0, 0}}, /* 0,1,1,1,1,0,1,0 */
    {6,    {1, 2, 4, 5, 6, 7, 0, 0}}, /* 0,1,1,1,1,0,1,1 */
    {5,    {3, 4, 5, 6, 7, 0, 0, 0}}, /* 0,1,1,1,1,1,0,0 */
    {6,    {1, 3, 4, 5, 6, 7, 0, 0}}, /* 0,1,1,1,1,1,0,1 */
    {6,    {2, 3, 4, 5, 6, 7, 0, 0}}, /* 0,1,1,1,1,1,1,0 */
    {7,    {1, 2, 3, 4, 5, 6, 7, 0}}, /* 0,1,1,1,1,1,1,1 */




    {1,    {8, 0, 0, 0, 0, 0, 0, 0}}, /* 1,0,0,0,0,0,0,0 */
    {2,    {1, 8, 0, 0, 0, 0, 0, 0}}, /* 1,0,0,0,0,0,0,1 */
    {2,    {2, 8, 0, 0, 0, 0, 0, 0}}, /* 1,0,0,0,0,0,1,0 */
    {3,    {1, 2, 8, 0, 0, 0, 0, 0}}, /* 1,0,0,0,0,0,1,1 */
    {2,    {3, 8, 0, 0, 0, 0, 0, 0}}, /* 1,0,0,0,0,1,0,0 */
    {3,    {1, 3, 8, 0, 0, 0, 0, 0}}, /* 1,0,0,0,0,1,0,1 */
    {3,    {2, 3, 8, 0, 0, 0, 0, 0}}, /* 1,0,0,0,0,1,1,0 */
    {4,    {1, 2, 3, 8, 0, 0, 0, 0}}, /* 1,0,0,0,0,1,1,1 */
    {2,    {4, 8, 0, 0, 0, 0, 0, 0}}, /* 1,0,0,0,1,0,0,0 */
    {3,    {1, 4, 8, 0, 0, 0, 0, 0}}, /* 1,0,0,0,1,0,0,1 */
    {3,    {2, 4, 8, 0, 0, 0, 0, 0}}, /* 1,0,0,0,1,0,1,0 */
    {4,    {1, 2, 4, 8, 0, 0, 0, 0}}, /* 1,0,0,0,1,0,1,1 */
    {3,    {3, 4, 8, 0, 0, 0, 0, 0}}, /* 1,0,0,0,1,1,0,0 */
    {4,    {1, 3, 4, 8, 0, 0, 0, 0}}, /* 1,0,0,0,1,1,0,1 */
    {4,    {2, 3, 4, 8, 0, 0, 0, 0}}, /* 1,0,0,0,1,1,1,0 */
    {5,    {1, 2, 3, 4, 8, 0, 0, 0}}, /* 1,0,0,0,1,1,1,1 */

    {2,    {5, 8, 0, 0, 0, 0, 0, 0}}, /* 1,0,0,1,0,0,0,0 */
    {3,    {1, 5, 8, 0, 0, 0, 0, 0}}, /* 1,0,0,1,0,0,0,1 */
    {3,    {2, 5, 8, 0, 0, 0, 0, 0}}, /* 1,0,0,1,0,0,1,0 */
    {4,    {1, 2, 5, 8, 0, 0, 0, 0}}, /* 1,0,0,1,0,0,1,1 */
    {3,    {3, 5, 8, 0, 0, 0, 0, 0}}, /* 1,0,0,1,0,1,0,0 */
    {4,    {1, 3, 5, 8, 0, 0, 0, 0}}, /* 1,0,0,1,0,1,0,1 */
    {4,    {2, 3, 5, 8, 0, 0, 0, 0}}, /* 1,0,0,1,0,1,1,0 */
    {5,    {1, 2, 3, 5, 8, 0, 0, 0}}, /* 1,0,0,1,0,1,1,1 */
    {3,    {4, 5, 8, 0, 0, 0, 0, 0}}, /* 1,0,0,1,1,0,0,0 */
    {4,    {1, 4, 5, 8, 0, 0, 0, 0}}, /* 1,0,0,1,1,0,0,1 */
    {4,    {2, 4, 5, 8, 0, 0, 0, 0}}, /* 1,0,0,1,1,0,1,0 */
    {5,    {1, 2, 4, 5, 8, 0, 0, 0}}, /* 1,0,0,1,1,0,1,1 */
    {4,    {3, 4, 5, 8, 0, 0, 0, 0}}, /* 1,0,0,1,1,1,0,0 */
    {5,    {1, 3, 4, 5, 8, 0, 0, 0}}, /* 1,0,0,1,1,1,0,1 */
    {5,    {2, 3, 4, 5, 8, 0, 0, 0}}, /* 1,0,0,1,1,1,1,0 */
    {6,    {1, 2, 3, 4, 5, 8, 0, 0}}, /* 1,0,0,1,1,1,1,1 */

    {2,    {6, 8, 0, 0, 0, 0, 0, 0}}, /* 1,0,1,0,0,0,0,0 */
    {3,    {1, 6, 8, 0, 0, 0, 0, 0}}, /* 1,0,1,0,0,0,0,1 */
    {3,    {2, 6, 8, 0, 0, 0, 0, 0}}, /* 1,0,1,0,0,0,1,0 */
    {4,    {1, 2, 6, 8, 0, 0, 0, 0}}, /* 1,0,1,0,0,0,1,1 */
    {3,    {3, 6, 8, 0, 0, 0, 0, 0}}, /* 1,0,1,0,0,1,0,0 */
    {4,    {1, 3, 6, 8, 0, 0, 0, 0}}, /* 1,0,1,0,0,1,0,1 */
    {4,    {2, 3, 6, 8, 0, 0, 0, 0}}, /* 1,0,1,0,0,1,1,0 */
    {5,    {1, 2, 3, 6, 8, 0, 0, 0}}, /* 1,0,1,0,0,1,1,1 */
    {3,    {4, 6, 8, 0, 0, 0, 0, 0}}, /* 1,0,1,0,1,0,0,0 */
    {4,    {1, 4, 6, 8, 0, 0, 0, 0}}, /* 1,0,1,0,1,0,0,1 */
    {4,    {2, 4, 6, 8, 0, 0, 0, 0}}, /* 1,0,1,0,1,0,1,0 */
    {5,    {1, 2, 4, 6, 8, 0, 0, 0}}, /* 1,0,1,0,1,0,1,1 */
    {4,    {3, 4, 6, 8, 0, 0, 0, 0}}, /* 1,0,1,0,1,1,0,0 */
    {5,    {1, 3, 4, 6, 8, 0, 0, 0}}, /* 1,0,1,0,1,1,0,1 */
    {5,    {2, 3, 4, 6, 8, 0, 0, 0}}, /* 1,0,1,0,1,1,1,0 */
    {6,    {1, 2, 3, 4, 6, 8, 0, 0}}, /* 1,0,1,0,1,1,1,1 */

    {3,    {5, 6, 8, 0, 0, 0, 0, 0}}, /* 1,0,1,1,0,0,0,0 */
    {4,    {1, 5, 6, 8, 0, 0, 0, 0}}, /* 1,0,1,1,0,0,0,1 */
    {4,    {2, 5, 6, 8, 0, 0, 0, 0}}, /* 1,0,1,1,0,0,1,0 */
    {5,    {1, 2, 5, 6, 8, 0, 0, 0}}, /* 1,0,1,1,0,0,1,1 */
    {4,    {3, 5, 6, 8, 0, 0, 0, 0}}, /* 1,0,1,1,0,1,0,0 */
    {5,    {1, 3, 5, 6, 8, 0, 0, 0}}, /* 1,0,1,1,0,1,0,1 */
    {5,    {2, 3, 5, 6, 8, 0, 0, 0}}, /* 1,0,1,1,0,1,1,0 */
    {6,    {1, 2, 3, 5, 6, 8, 0, 0}}, /* 1,0,1,1,0,1,1,1 */
    {4,    {4, 5, 6, 8, 0, 0, 0, 0}}, /* 1,0,1,1,1,0,0,0 */
    {5,    {1, 4, 5, 6, 8, 0, 0, 0}}, /* 1,0,1,1,1,0,0,1 */
    {5,    {2, 4, 5, 6, 8, 0, 0, 0}}, /* 1,0,1,1,1,0,1,0 */
    {6,    {1, 2, 4, 5, 6, 8, 0, 0}}, /* 1,0,1,1,1,0,1,1 */
    {5,    {3, 4, 5, 6, 8, 0, 0, 0}}, /* 1,0,1,1,1,1,0,0 */
    {6,    {1, 3, 4, 5, 6, 8, 0, 0}}, /* 1,0,1,1,1,1,0,1 */
    {6,    {2, 3, 4, 5, 6, 8, 0, 0}}, /* 1,0,1,1,1,1,1,0 */
    {7,    {1, 2, 3, 4, 5, 6, 8, 0}}, /* 1,0,1,1,1,1,1,1 */



    {2,    {7, 8, 0, 0, 0, 0, 0, 0}}, /* 1,1,0,0,0,0,0,0 */
    {3,    {1, 7, 8, 0, 0, 0, 0, 0}}, /* 1,1,0,0,0,0,0,1 */
    {3,    {2, 7, 8, 0, 0, 0, 0, 0}}, /* 1,1,0,0,0,0,1,0 */
    {4,    {1, 2, 7, 8, 0, 0, 0, 0}}, /* 1,1,0,0,0,0,1,1 */
    {3,    {3, 7, 8, 0, 0, 0, 0, 0}}, /* 1,1,0,0,0,1,0,0 */
    {4,    {1, 3, 7, 8, 0, 0, 0, 0}}, /* 1,1,0,0,0,1,0,1 */
    {4,    {2, 3, 7, 8, 0, 0, 0, 0}}, /* 1,1,0,0,0,1,1,0 */
    {5,    {1, 3, 7, 8, 0, 0, 0, 0}}, /* 1,1,0,0,0,1,1,1 */
    {3,    {4, 7, 8, 0, 0, 0, 0, 0}}, /* 1,1,0,0,1,0,0,0 */
    {4,    {1, 4, 7, 8, 0, 0, 0, 0}}, /* 1,1,0,0,1,0,0,1 */
    {4,    {2, 4, 7, 8, 0, 0, 0, 0}}, /* 1,1,0,0,1,0,1,0 */
    {5,    {1, 2, 4, 7, 8, 0, 0, 0}}, /* 1,1,0,0,1,0,1,1 */
    {4,    {3, 4, 7, 8, 0, 0, 0, 0}}, /* 1,1,0,0,1,1,0,0 */
    {5,    {1, 3, 4, 7, 8, 0, 0, 0}}, /* 1,1,0,0,1,1,0,1 */
    {5,    {2, 3, 4, 7, 8, 0, 0, 0}}, /* 1,1,0,0,1,1,1,0 */
    {6,    {1, 2, 3, 4, 7, 8, 0, 0}}, /* 1,1,0,0,1,1,1,1 */

    {3,    {5, 7, 8, 0, 0, 0, 0, 0}}, /* 1,1,0,1,0,0,0,0 */
    {4,    {1, 5, 7, 8, 0, 0, 0, 0}}, /* 1,1,0,1,0,0,0,1 */
    {4,    {2, 5, 7, 8, 0, 0, 0, 0}}, /* 1,1,0,1,0,0,1,0 */
    {5,    {1, 2, 5, 7, 8, 0, 0, 0}}, /* 1,1,0,1,0,0,1,1 */
    {4,    {3, 5, 7, 8, 0, 0, 0, 0}}, /* 1,1,0,1,0,1,0,0 */
    {5,    {1, 3, 5, 7, 8, 0, 0, 0}}, /* 1,1,0,1,0,1,0,1 */
    {5,    {2, 3, 5, 7, 8, 0, 0, 0}}, /* 1,1,0,1,0,1,1,0 */
    {6,    {1, 2, 3, 5, 7, 8, 0, 0}}, /* 1,1,0,1,0,1,1,1 */
    {4,    {4, 5, 7, 8, 0, 0, 0, 0}}, /* 1,1,0,1,1,0,0,0 */
    {5,    {1, 4, 5, 7, 8, 0, 0, 0}}, /* 1,1,0,1,1,0,0,1 */
    {5,    {2, 4, 5, 7, 8, 0, 0, 0}}, /* 1,1,0,1,1,0,1,0 */
    {6,    {1, 2, 4, 5, 7, 8, 0, 0}}, /* 1,1,0,1,1,0,1,1 */
    {5,    {3, 4, 5, 7, 8, 0, 0, 0}}, /* 1,1,0,1,1,1,0,0 */
    {6,    {1, 3, 4, 5, 7, 8, 0, 0}}, /* 1,1,0,1,1,1,0,1 */
    {6,    {2, 3, 4, 5, 7, 8, 0, 0}}, /* 1,1,0,1,1,1,1,0 */
    {7,    {1, 2, 3, 4, 5, 7, 8, 0}}, /* 1,1,0,1,1,1,1,1 */

    {3,    {6, 7, 8, 0, 0, 0, 0, 0}}, /* 1,1,1,0,0,0,0,0 */
    {4,    {1, 6, 7, 8, 0, 0, 0, 0}}, /* 1,1,1,0,0,0,0,1 */
    {4,    {2, 6, 7, 8, 0, 0, 0, 0}}, /* 1,1,1,0,0,0,1,0 */
    {5,    {1, 2, 6, 7, 8, 0, 0, 0}}, /* 1,1,1,0,0,0,1,1 */
    {4,    {3, 6, 7, 8, 0, 0, 0, 0}}, /* 1,1,1,0,0,1,0,0 */
    {5,    {1, 3, 6, 7, 8, 0, 0, 0}}, /* 1,1,1,0,0,1,0,1 */
    {5,    {2, 3, 6, 7, 8, 0, 0, 0}}, /* 1,1,1,0,0,1,1,0 */
    {6,    {1, 2, 3, 6, 7, 8, 0, 0}}, /* 1,1,1,0,0,1,1,1 */
    {4,    {4, 6, 7, 8, 0, 0, 0, 0}}, /* 1,1,1,0,1,0,0,0 */
    {5,    {1, 4, 6, 7, 8, 0, 0, 0}}, /* 1,1,1,0,1,0,0,1 */
    {5,    {2, 4, 6, 7, 8, 0, 0, 0}}, /* 1,1,1,0,1,0,1,0 */
    {6,    {1, 2, 4, 6, 7, 8, 0, 0}}, /* 1,1,1,0,1,0,1,1 */
    {5,    {3, 4, 6, 7, 8, 0, 0, 0}}, /* 1,1,1,0,1,1,0,0 */
    {6,    {1, 3, 4, 6, 7, 8, 0, 0}}, /* 1,1,1,0,1,1,0,1 */
    {6,    {2, 3, 4, 6, 7, 8, 0, 0}}, /* 1,1,1,0,1,1,1,0 */
    {7,    {1, 2, 3, 4, 6, 7, 8, 0}}, /* 1,1,1,0,1,1,1,1 */

    {4,    {5, 6, 7, 8, 0, 0, 0, 0}}, /* 1,1,1,1,0,0,0,0 */
    {5,    {1, 5, 6, 7, 8, 0, 0, 0}}, /* 1,1,1,1,0,0,0,1 */
    {5,    {2, 5, 6, 7, 8, 0, 0, 0}}, /* 1,1,1,1,0,0,1,0 */
    {6,    {1, 2, 5, 6, 7, 8, 0, 0}}, /* 1,1,1,1,0,0,1,1 */
    {5,    {3, 5, 6, 7, 8, 0, 0, 0}}, /* 1,1,1,1,0,1,0,0 */
    {6,    {1, 3, 5, 6, 7, 8, 0, 0}}, /* 1,1,1,1,0,1,0,1 */
    {6,    {2, 3, 5, 6, 7, 8, 0, 0}}, /* 1,1,1,1,0,1,1,0 */
    {7,    {1, 2, 3, 5, 6, 7, 8, 0}}, /* 1,1,1,1,0,1,1,1 */
    {5,    {4, 5, 6, 7, 8, 0, 0, 0}}, /* 1,1,1,1,1,0,0,0 */
    {6,    {1, 4, 5, 6, 7, 8, 0, 0}}, /* 1,1,1,1,1,0,0,1 */
    {6,    {2, 4, 5, 6, 7, 8, 0, 0}}, /* 1,1,1,1,1,0,1,0 */
    {7,    {1, 2, 4, 5, 6, 7, 8, 0}}, /* 1,1,1,1,1,0,1,1 */
    {6,    {3, 4, 5, 6, 7, 8, 0, 0}}, /* 1,1,1,1,1,1,0,0 */
    {7,    {1, 3, 4, 5, 6, 7, 8, 0}}, /* 1,1,1,1,1,1,0,1 */
    {7,    {2, 3, 4, 5, 6, 7, 8, 0}}, /* 1,1,1,1,1,1,1,0 */
    {8,    {1, 2, 3, 4, 5, 6, 7, 8}}  /* 1,1,1,1,1,1,1,1 */
};




/*****************************************************************************
* Function Name    : gl_sm_setup_tcp_server
* Arguments        : gw_S8bit           *p_server_ip_addr,
*                    gw_U16bit          server_portno
* Return value     : gw_S32bit
* Functionality    : Connects a specified socket to a specified remote address.
* Functions called :
*****************************************************************************/
gw_S32bit
gl_sm_setup_tcp_server

#ifdef _ANSI_C_
( gw_S8bit                  *p_server_ip_addr,
  gw_U16bit                 server_portno
)
#else
( p_server_ip_addr, server_portno )
gw_S8bit                    *p_server_ip_addr;
gw_U16bit                   server_portno;
#endif

/****************************************************************************/
{
    gw_S32bit                      sockid;
    gw_S32bit                   ioctl_arg = 1;
    gw_S8bit                    sock_optval = 1;

    if( GW_ERROR == ( sockid = gl_sm_create_socket( GL_SM_AF_INET,
                                                    GL_SM_SOCK_STREAM, GL_SM_IPPROTO_TCP ) ) )
    {
        return GW_ERROR;
    }

    else if( GL_FAILURE == gl_sm_ioctl_socket( sockid, GL_SM_FIONBIO,
                                               &ioctl_arg ) )
    {
        gl_sm_close_socket( sockid );
        return GW_ERROR;
    }

    else if( GL_FAILURE == gl_sm_setsockopt( sockid, GL_SM_LVL_SOL_SOCKET,
                                             GL_SM_SO_REUSEADDR, &sock_optval, ( gw_S32bit )sizeof( gw_S32bit ) ) )
    {
        gl_sm_close_socket( sockid );
        return GW_ERROR;
    }

    else if( GL_FAILURE == gl_sm_bind_socket( sockid, p_server_ip_addr,
                                              server_portno ) )
    {
        gl_sm_close_socket( sockid );
        return GW_ERROR;
    }

    else if( GL_FAILURE == gl_sm_listen_socket( sockid, 5 ) )
    {
        gl_sm_close_socket( sockid );
        return GW_ERROR;
    }

    else
    {
        return sockid;
    }
}





/*****************************************************************************
* Function Name    : gl_sm_setup_tcp_client
* Arguments        : gw_S8bit           *p_server_ip_addr,
*                    gw_U16bit          server_portno
* Return value     : gw_S32bit
* Functionality    : Connects a specified socket to a specified remote address.
* Functions called :
*****************************************************************************/
gw_S32bit
gl_sm_setup_tcp_client

#ifdef _ANSI_C_
( gw_S8bit                  *p_server_ip_addr,
  gw_U16bit                 server_portno
)
#else
( p_server_ip_addr, server_portno )
gw_S8bit                    *p_server_ip_addr;
gw_U16bit                   server_portno;
#endif

/****************************************************************************/
{
    gw_S32bit                      sockid;

    if( GW_ERROR == ( sockid = gl_sm_create_socket( GL_SM_AF_INET,
                                                    GL_SM_SOCK_STREAM, GL_SM_IPPROTO_TCP ) ) )
    {
        return GW_ERROR;
    }

    else if( GL_FAILURE == gl_sm_connect_socket( sockid, p_server_ip_addr,
                                                 server_portno ) )
    {
        gl_sm_close_socket( sockid );
        return GW_ERROR;
    }

    else
    {
        return sockid;
    }
}






/*****************************************************************************
* Function Name    : gl_sm_setup_udp_server_or_client
* Arguments        : gw_S8bit           *p_ip_addr,
*                    gw_U16bit          portno
* Return value     : gw_S32bit
* Functionality    : Connects a specified socket to a specified remote address.
* Functions called :
*****************************************************************************/
gw_S32bit
gl_sm_setup_udp_server_or_client

#ifdef _ANSI_C_
( gw_S8bit                  *p_ip_addr,
  gw_U16bit                 portno
)
#else
( p_ip_addr, portno )
gw_S8bit                  *p_ip_addr;
gw_U16bit                 portno;
#endif

/****************************************************************************/
{
    gw_S32bit                   sockid;

    if( GW_ERROR == ( sockid = gl_sm_create_socket( GL_SM_AF_INET,
                                                    GL_SM_SOCK_DGRAM, GL_SM_IPPROTO_UDP ) ) )
    {
        return GW_ERROR;
    }

    /*
     * To be confirmed.
     * else if(GL_FAILURE == gl_sm_setsocketopt(sockid, GL_SM_LVL_SOL_SOCKET,
     *           GL_SM_SO_REUSEADDR, &sock_optval, sizeof(gw_S8bit)))
     * {
     *    return GW_ERROR;
     * }
     */
    else if( GL_FAILURE == gl_sm_bind_socket( sockid, p_ip_addr, portno ) )
    {
        gl_sm_close_socket( sockid );
        return GW_ERROR;
    }

    else
    {
        return sockid;
    }
}





/*****************************************************************************
* Function Name    : gl_sm_tlm_init
* Arguments        : gw_U32bit                  num_of_tpt_link_ids,
*                    gw_U32bit                  num_of_tpt_link_keys,
*                    gl_sm_tpt_type_et          transport_type,
*                    gl_error_t                 *p_ecode
* Return value     : gl_return_t
* Functionality    : Intializes the transport link mapping databases for the
*                    given transport type.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_init

#ifdef _ANSI_C_
( gw_U32bit                 num_of_tpt_link_ids,
  gw_U32bit                 num_of_tpt_link_keys,
  gl_sm_tpt_type_et         transport_type,
  gl_error_t                *p_ecode
)
#else
( num_of_tpt_link_ids, num_of_tpt_link_keys, transport_type, p_ecode )
gw_U32bit                 num_of_tpt_link_ids;
gw_U32bit                 num_of_tpt_link_keys;
gl_sm_tpt_type_et         transport_type;
gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    if( ( GL_SM_TLM_TPT_TYPE_UDP == transport_type ) ||
            ( GL_SM_TLM_TPT_TYPE_TCP == transport_type ) )
    {
        if( FD_SETSIZE < num_of_tpt_link_ids )
        {
            *p_ecode = GL_SM_ERR_INV_NUM_OF_LKN_ID_VAL;
            return GL_FAILURE;
        }

        if( GL_NULL == g_a_p_tcp_udp_lnk_id_info )
        {
            g_a_p_tcp_udp_lnk_id_info = ( gl_sm_tcp_udp_lnk_id_info_t * )
                                        SYS_MALLOC( FD_SETSIZE );

            if( GL_NULL == g_a_p_tcp_udp_lnk_id_info )
            {
                *p_ecode = GL_SM_ERR_NO_RESOURCES;
                return GL_FAILURE;
            }
        }
    }

    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at
         * present.
         */
        return GL_FAILURE;
    }

    if( GL_SM_TLM_TPT_TYPE_UDP == transport_type )
    {
        if( GL_FAILURE == gl_htm_create_hash_table(
                    GL_NULL,
                    &( g_udp_lnk_map_info.hash_table_id ),
                    num_of_tpt_link_keys,
                    num_of_tpt_link_keys,
                    GL_HTM_USER_DEFINED_KEY_TYPE,
                    /* default type of key */
                    GL_HTM_HASH_FUNC_FOR_STRING_KEYS,
                    GL_NULL,
                    ( gl_htm_free_key_elem_fn_t )gl_sm_htm_free_func,
                    GL_NULL,
                    GL_NULL,
                    p_ecode ) )
        {
            *p_ecode = GL_SM_ERR_NO_RESOURCES;
            return GL_FAILURE;
        }
    }

    else if( GL_SM_TLM_TPT_TYPE_TCP == transport_type )
    {
        if( GL_NULL == g_a_p_tcp_lnk_map_info )
        {
            g_a_p_tcp_lnk_map_info = ( gl_sm_tcp_lnk_map_info_t * )
                                     SYS_MALLOC( sizeof( gl_sm_tcp_lnk_map_info_t ) * FD_SETSIZE );

            if( GL_NULL == g_a_p_tcp_lnk_map_info )
            {
                *p_ecode = GL_SM_ERR_NO_RESOURCES;
                return GL_FAILURE;
            }
        }

    }

    return GL_SUCCESS;

}





/*****************************************************************************
* Function Name    : gl_sm_tlm_set_tpt_link_id_data
* Arguments        : gl_sm_tpt_type_et          transport_type,
*                    gw_U32bit                  tpt_lnk_id,
*                    gl_sm_tpt_lnk_id_data_st   *p_tpt_lnk_id_data,
*                    gl_error_t                 *p_ecode
* Return value     : gl_return_t
* Functionality    : Set the transport link-id data corresponding to given
*                    transport link-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_set_tpt_lnk_id_data

#ifdef _ANSI_C_
( gl_sm_tpt_type_et         transport_type,
  gw_U32bit                 tpt_lnk_id,
  gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data,
  gl_error_t                *p_ecode
)
#else
( transport_type, tpt_lnk_id, p_tpt_lnk_id_data, p_ecode )
gl_sm_tpt_type_et         transport_type;
gw_U32bit                 tpt_lnk_id;
gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data;
gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    if( ( GL_SM_TLM_TPT_TYPE_UDP == transport_type ) ||
            ( GL_SM_TLM_TPT_TYPE_TCP == transport_type ) )
    {
        if( FD_SETSIZE < tpt_lnk_id )
        {
            *p_ecode = GL_SM_ERR_INV_NUM_OF_LKN_ID_VAL;
            return GL_FAILURE;
        }

        p_tpt_lnk_id_data->tpt_lnk_mgmt.tpt_type = transport_type;

        g_a_p_tcp_udp_lnk_id_info[tpt_lnk_id].p_tpt_lnk_id_data =
            p_tpt_lnk_id_data;

    }

    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at
         * present.
         */
        return GL_FAILURE;
    }

    return GL_SUCCESS;
}



/*****************************************************************************
* Function Name    : gl_sm_tlm_get_tpt_link_id_data
* Arguments        : gl_sm_tpt_type_et          transport_type,
*                    gw_U32bit                  tpt_lnk_id,
*                    gl_sm_tpt_lnk_id_data_st   **p_p_tpt_lnk_id_data,
*                    gl_error_t                 *p_ecode
* Return value     : gl_return_t
* Functionality    : Get the transport link-id data corresponding to given G
*                    transport link-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_get_tpt_lnk_id_data

#ifdef _ANSI_C_
( gl_sm_tpt_type_et         transport_type,
  gw_U32bit                 tpt_lnk_id,
  gl_sm_tpt_lnk_id_data_st  **p_p_tpt_lnk_id_data,
  gl_error_t                *p_ecode
)
#else
( transport_type, tpt_lnk_id, p_p_tpt_lnk_id_data, p_ecode )
gl_sm_tpt_type_et         transport_type;
gw_U32bit                 tpt_lnk_id;
gl_sm_tpt_lnk_id_data_st  **p_p_tpt_lnk_id_data;
gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    if( ( GL_SM_TLM_TPT_TYPE_UDP == transport_type ) ||
            ( GL_SM_TLM_TPT_TYPE_TCP == transport_type ) )
    {
        if( FD_SETSIZE < tpt_lnk_id )
        {
            *p_ecode = GL_SM_ERR_INV_NUM_OF_LKN_ID_VAL;
            return GL_FAILURE;
        }

        *p_p_tpt_lnk_id_data = g_a_p_tcp_udp_lnk_id_info[tpt_lnk_id].
                               p_tpt_lnk_id_data;
    }

    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at
         * present.
         */
        return GL_FAILURE;
    }

    return GL_SUCCESS;
}





/*****************************************************************************
* Function Name    : gl_sm_tlm_delete_tpt_link_id_data
* Arguments        : gl_sm_tpt_type_et          transport_type,
*                    gw_U32bit                  tpt_lnk_id,
*                    gl_sm_tpt_lnk_id_data_st   **p_p_tpt_lnk_id_data,
*                    gl_error_t                 *p_ecode
* Return value     : gl_return_t
* Functionality    : Delete the transport link-id data corresponding to given
*                    transport link-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_delete_tpt_lnk_id_data

#ifdef _ANSI_C_
( gl_sm_tpt_type_et         transport_type,
  gw_U32bit                 tpt_lnk_id,
  gl_sm_tpt_lnk_id_data_st  **p_p_tpt_lnk_id_data,
  gl_error_t                *p_ecode
)
#else
( transport_type, tpt_lnk_id, p_p_tpt_lnk_id_data, p_ecode )
gl_sm_tpt_type_et         transport_type;
gw_U32bit                 tpt_lnk_id;
gl_sm_tpt_lnk_id_data_st  **p_p_tpt_lnk_id_data;
gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    if( ( GL_SM_TLM_TPT_TYPE_UDP == transport_type ) ||
            ( GL_SM_TLM_TPT_TYPE_TCP == transport_type ) )
    {
        if( FD_SETSIZE < tpt_lnk_id )
        {
            *p_ecode = GL_SM_ERR_INV_NUM_OF_LKN_ID_VAL;
            return GL_FAILURE;
        }

        *p_p_tpt_lnk_id_data = g_a_p_tcp_udp_lnk_id_info[tpt_lnk_id].
                               p_tpt_lnk_id_data;
        g_a_p_tcp_udp_lnk_id_info[tpt_lnk_id].p_tpt_lnk_id_data = GL_NULL;
    }

    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at
         * present.
         */
        return GL_FAILURE;
    }

    return GL_SUCCESS;
}




/*****************************************************************************
* Function Name    : gl_sm_tlm_set_tpt_link_key_map
* Arguments        : gl_sm_tpt_type_et          transport_type,
*                    gl_sm_tpt_lnk_id_data_st   *p_tpt_lnk_id_data,
*                    gl_sm_tlm_tpt_lnk_key_ut   *p_tpt_lnk_key,
*                    gw_U32bit                  su_lnk_id,
*                    gl_pvoid_t                 p_su_lnk_id_data,
*                    gl_error_t                 *p_ecode
* Return value     : gl_return_t
* Functionality    : Set the mapping of transport link key (transport
*                    parameters) and system user provided logical link-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_set_tpt_link_key_map

#ifdef _ANSI_C_
( gl_sm_tpt_type_et         transport_type,
  gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data,
  gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key,
  gw_U32bit                 su_lnk_id,
  gl_pvoid_t                p_su_lnk_id_data,
  gl_error_t                *p_ecode
)
#else
( transport_type, p_tpt_lnk_id_data, p_tpt_lnk_key, su_lnk_id,
  p_su_lnk_id_data, p_ecode )
gl_sm_tpt_type_et         transport_type;
gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data;
gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key;
gw_U32bit                 su_lnk_id;
gl_pvoid_t                p_su_lnk_id_data;
gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    gw_U8bit                    *p_udp_str_key = GL_NULL;
    gl_sm_tcp_lnk_map_info_t    *p_map_info = GL_NULL;
    gl_pvoid_t                  p_key_node_handle;
    gl_hash_key_ut              hash_key;

    if( GL_SM_TLM_TPT_TYPE_UDP == transport_type )
    {
        p_udp_str_key = ( gw_U8bit * )gl_mm_get_buffer( GL_NULL, 30, p_ecode );

        if( GL_NULL == p_udp_str_key )
        {
            return GL_FAILURE;
        }

        gl_mm_memset( p_udp_str_key, '$', 30 );

        p_map_info = ( gl_sm_tcp_lnk_map_info_t * )gl_mm_get_buffer( GL_NULL,
                                                                     sizeof( gl_sm_tcp_lnk_map_info_t ), p_ecode );

        if( GL_NULL == p_map_info )
        {
            return GL_FAILURE;
        }

        p_map_info->su_lnk_id = su_lnk_id;
        p_map_info->p_su_lnk_id_data = p_su_lnk_id_data;

        if( GL_FAILURE == gl_sm_convert_udp_tpt_key_to_string_key( p_udp_str_key
                                                                   , p_tpt_lnk_key, p_ecode ) )
        {
            return GL_FAILURE;
        }

        hash_key.p_user_def_id = p_udp_str_key;

        if( GL_FAILURE == gl_htm_add_key_to_hash_table(
                    GL_NULL,
                    g_udp_lnk_map_info.hash_table_id,
                    &hash_key,
                    ( gl_pvoid_t )p_map_info,
                    &p_key_node_handle,
                    p_ecode ) )
        {
            return GL_FAILURE;
        }
    }

    else if( GL_SM_TLM_TPT_TYPE_TCP == transport_type )
    {
        if( FD_SETSIZE < p_tpt_lnk_key->tcp_tpt_info.socket_id )
        {
            return GL_FAILURE;
        }

        g_a_p_tcp_lnk_map_info[p_tpt_lnk_key->tcp_tpt_info.socket_id].su_lnk_id
            = su_lnk_id;
        g_a_p_tcp_lnk_map_info[p_tpt_lnk_key->tcp_tpt_info.socket_id].
        p_su_lnk_id_data = p_su_lnk_id_data;
    }

    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at
         * present.
         */
        /*
         * To remove warning.
         */
        p_tpt_lnk_id_data = p_tpt_lnk_id_data;
        return GL_FAILURE;
    }

    return GL_SUCCESS;
}



/*****************************************************************************
* Function Name    : gl_sm_tlm_get_tpt_link_key_map
* Arguments        : gl_sm_tpt_type_et          transport_type,
*                    gl_sm_tpt_lnk_id_data_st   *p_tpt_lnk_id_data,
*                    gl_sm_tlm_tpt_lnk_key_ut   *p_tpt_lnk_key,
*                    gw_U32bit                  *p_su_lnk_id,
*                    gl_pvoid_t                 *p_p_su_lnk_id_data,
*                    gl_error_t                 *p_ecode
* Return value     : gl_return_t
* Functionality    : Get the mapping of transport link key (transport
*                    parameters) and system user provided logical link-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_get_tpt_link_key_map

#ifdef _ANSI_C_
( gl_sm_tpt_type_et         transport_type,
  gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data,
  gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key,
  gw_U32bit                 *p_su_lnk_id,
  gl_pvoid_t                *p_p_su_lnk_id_data,
  gl_error_t                *p_ecode
)
#else
( transport_type, p_tpt_lnk_id_data, p_tpt_lnk_key, p_su_lnk_id,
  p_p_su_lnk_id_data, p_ecode )
gl_sm_tpt_type_et         transport_type;
gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data;
gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key;
gw_U32bit                 *p_su_lnk_id;
gl_pvoid_t                *p_p_su_lnk_id_data;
gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    gw_U8bit                    udp_str_key[30];
    gl_sm_tcp_lnk_map_info_t    *p_map_info = GL_NULL;
    gl_hash_key_ut              hash_key;

    if( GL_SM_TLM_TPT_TYPE_UDP == transport_type )
    {
        gl_mm_memset( udp_str_key, '$', 30 );

        if( GL_FAILURE == gl_sm_convert_udp_tpt_key_to_string_key( udp_str_key,
                                                                   p_tpt_lnk_key, p_ecode ) )
        {
            return GL_FAILURE;
        }

        hash_key.p_user_def_id = udp_str_key;

        if( GL_FAILURE == gl_htm_get_key_ref_from_hash_table(
                    GL_NULL,
                    g_udp_lnk_map_info.hash_table_id,
                    &hash_key,
                    ( gl_pvoid_t * )&p_map_info,
                    p_ecode ) )
        {
            return GL_FAILURE;
        }

        *p_su_lnk_id = p_map_info->su_lnk_id;
        *p_p_su_lnk_id_data = p_map_info->p_su_lnk_id_data;
    }

    else if( GL_SM_TLM_TPT_TYPE_TCP == transport_type )
    {
        if( FD_SETSIZE < p_tpt_lnk_key->tcp_tpt_info.socket_id )
        {
            return GL_FAILURE;
        }

        *p_su_lnk_id = g_a_p_tcp_lnk_map_info[p_tpt_lnk_key->tcp_tpt_info.
                                              socket_id].su_lnk_id;
        *p_p_su_lnk_id_data = g_a_p_tcp_lnk_map_info[p_tpt_lnk_key->
                                                     tcp_tpt_info.socket_id].p_su_lnk_id_data;
    }

    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at
         * present.
         */
        /*
         * To remove warning.
         */
        p_tpt_lnk_id_data = p_tpt_lnk_id_data;
        return GL_FAILURE;
    }

    return GL_SUCCESS;
}





/*****************************************************************************
* Function Name    : gl_sm_tlm_delete_tpt_link_key_map
* Arguments        : gl_sm_tpt_type_et          transport_type,
*                    gl_sm_tpt_lnk_id_data_st   *p_tpt_lnk_id_data,
*                    gl_sm_tlm_tpt_lnk_key_ut   *p_tpt_lnk_key,
*                    gw_U32bit                  *p_su_lnk_id,
*                    gl_pvoid_t                 *p_p_su_lnk_id_data,
*                    gl_error_t                 *p_ecode
* Return value     : gl_return_t
* Functionality    : Delete the mapping of transport link key (transport
*                    parameters) and system user provided logical link-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_delete_tpt_link_key_map

#ifdef _ANSI_C_
( gl_sm_tpt_type_et         transport_type,
  gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data,
  gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key,
  gw_U32bit                 *p_su_lnk_id,
  gl_pvoid_t                *p_p_su_lnk_id_data,
  gl_error_t                *p_ecode
)
#else
( transport_type, p_tpt_lnk_id_data, p_tpt_lnk_key, p_su_lnk_id,
  p_p_su_lnk_id_data, p_ecode )
gl_sm_tpt_type_et         transport_type;
gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data;
gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key;
gw_U32bit                 *p_su_lnk_id;
gl_pvoid_t                *p_p_su_lnk_id_data;
gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    gw_U8bit                    udp_str_key[30];
    gl_sm_tcp_lnk_map_info_t    *p_map_info = GL_NULL;
    gl_hash_key_ut              hash_key;
    gl_pvoid_t                  p_stored_key;

    if( GL_SM_TLM_TPT_TYPE_UDP == transport_type )
    {
        gl_mm_memset( udp_str_key, '$', 30 );

        if( GL_FAILURE == gl_sm_convert_udp_tpt_key_to_string_key( udp_str_key,
                                                                   p_tpt_lnk_key, p_ecode ) )
        {
            return GL_FAILURE;
        }

        hash_key.p_user_def_id = udp_str_key;

        if( GL_FAILURE == gl_htm_remove_key_from_hash_table(
                    GL_NULL,
                    g_udp_lnk_map_info.hash_table_id,
                    &hash_key,
                    &p_stored_key,
                    ( gl_pvoid_t * )&p_map_info,
                    p_ecode ) )
        {
            return GL_FAILURE;
        }

        else
        {
            /*
             * Free the memory allocated for the String key.
             */
            if( GL_FAILURE == gl_mm_release_buffer( GL_NULL, p_stored_key,
                                                    p_ecode ) )
            {
                return GL_FAILURE;
            }
        }

        *p_su_lnk_id = p_map_info->su_lnk_id;
        *p_p_su_lnk_id_data = p_map_info->p_su_lnk_id_data;
    }

    else if( GL_SM_TLM_TPT_TYPE_TCP == transport_type )
    {
        if( FD_SETSIZE < p_tpt_lnk_key->tcp_tpt_info.socket_id )
        {
            return GL_FAILURE;
        }

        *p_su_lnk_id = g_a_p_tcp_lnk_map_info[p_tpt_lnk_key->tcp_tpt_info.
                                              socket_id].su_lnk_id;
        *p_p_su_lnk_id_data = g_a_p_tcp_lnk_map_info[p_tpt_lnk_key->
                                                     tcp_tpt_info.socket_id].p_su_lnk_id_data;
    }

    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at
         * present.
         */
        /*
         * To remove warning.
         */
        p_tpt_lnk_id_data = p_tpt_lnk_id_data;
        return GL_FAILURE;
    }

    return GL_SUCCESS;
}


/*****************************************************************************
* Function Name    : gl_sm_convert_udp_tpt_key_to_string_key
* Arguments        : gw_U8bit                   *p_str_key,
*                    gl_sm_tlm_tpt_lnk_key_ut   *p_tpt_lnk_key,
*                    gl_error_t                 *p_ecope
* Return value     : gl_return_t
* Functionality    : Convert UDP transport parameters to string key.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_convert_udp_tpt_key_to_string_key

#ifdef _ANSI_C_
( gw_U8bit                  *p_str_key,
  gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key,
  gl_error_t                *p_ecode
)
#else
( p_str_key, p_tpt_lnk_key, p_ecode )
gw_U8bit                  *p_str_key;
gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key;
gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    gw_U8bit                    socket_id_str_array[10];
    gw_U8bit                    ip_addr_str_array[10];
    gw_U8bit                    port_num_str_array[5];
    gw_S32bit                   str_len;
    gw_S32bit                   i;

    if( GL_FAILURE == gl_stm_itoa( p_tpt_lnk_key->udp_tpt_info.socket_id,
                                   socket_id_str_array, p_ecode ) )
    {
        return GL_FAILURE;
    }

    if( GL_FAILURE == gl_stm_itoa( p_tpt_lnk_key->udp_tpt_info.remote_ip_addr,
                                   ip_addr_str_array, p_ecode ) )
    {
        return GL_FAILURE;
    }

    if( GL_FAILURE == gl_stm_itoa( p_tpt_lnk_key->udp_tpt_info.remote_port_number
                                   , port_num_str_array, p_ecode ) )
    {
        return GL_FAILURE;
    }

    if( GW_ERROR == ( str_len = gl_stm_strlen( socket_id_str_array, p_ecode ) ) )
    {
        return GL_FAILURE;
    }

    for( i = 9, --str_len; ( str_len >= 0 ) && ( i >= 0 ); i--, str_len-- )
    {
        p_str_key[i] = socket_id_str_array[str_len];
    }

    if( GW_ERROR == ( str_len = gl_stm_strlen( ip_addr_str_array, p_ecode ) ) )
    {
        return GL_FAILURE;
    }

    for( i = 19, --str_len; ( str_len >= 0 ) && ( i >= 10 ); i--, str_len-- )
    {
        p_str_key[i] = ip_addr_str_array[str_len];
    }

    if( GW_ERROR == ( str_len = gl_stm_strlen( port_num_str_array, p_ecode ) ) )
    {
        return GL_FAILURE;
    }

    for( i = 24, --str_len; ( str_len >= 0 ) && ( i >= 20 ); i--, str_len-- )
    {
        p_str_key[i] = port_num_str_array[str_len];
    }

    p_str_key[25] = '\0';


    return GL_SUCCESS;
}



/*****************************************************************************
* Function Name    : gl_sm_htm_free_func
* Arguments        : gw_U8bit                   *p_str_key,
*                    gl_pvoid_t                 p_elem_buf_ptr,
*                    gl_error_t                 *p_ecope
* Return value     : gl_return_t
* Functionality    : Convert UDP transport parameters to string key.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_htm_free_func

#ifdef _ANSI_C_
( gw_U32bit                 task_id,
  gl_pvoid_t                p_elem_buf_ptr,
  gl_error_t                *p_ecode
)
#else
( task_id, p_elem_buf_ptr, p_ecode )
gw_U32bit                 task_id;
gl_pvoid_t                p_elem_buf_ptr;
gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    /*
     * As this function is to be called only for
     * API "gl_htm_remove_all_keys_from_hash_table"
     */
    task_id = task_id;
    p_elem_buf_ptr = p_elem_buf_ptr;
    p_ecode = p_ecode;

    return GL_SUCCESS;
}




