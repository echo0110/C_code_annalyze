/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cled_slw_opr.c,v 1.3.36.3 2010/11/09 05:17:59 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the functions for Sliding Window 
 *                     Operations
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_cled_slw_opr.c,v $
 * Revision 1.3.36.3  2010/11/09 05:17:59  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.36.2  2010/10/13 03:49:52  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.36.1  2010/09/23 05:10:53  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.3  2009/05/27 16:14:06  gur19836
 * File Header updated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_def.h"
#include "rohc_typ.h"
#include "rohc_c_typ.h"
#include "rohc_com_def.h"
#include "rohc_protocol_header.h"
#include "rohc_g_typ.h"
#include "rohc_led_def.h"
#include "rohc_led_mcr.h"
#include "rohc_trace_mcr.h"
/* + SPR 17439 */
#include "rohc_led_proto.h"
/* - SPR 17439 */
/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



/*****************************************************************************
**      FUNCTION:
**          rohc_c_add_r_mode_ext_hdr_p1_sw
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to add an entry in the R-Mode 
**          Extension header List Sliding Window
**      ARGUMENTS: 
**          The compressor context, number of items in the list and the
**          index list of the items in the translation table is 
**          taken as input. 
**      RETURN VALUE:
**          rohc_void_t
**      NOTES:
*****************************************************************************/
rohc_void_t rohc_c_add_r_mode_ext_hdr_p1_sw ROHC_PARAMS
    (
        (p_csc, count, index_list),
            rohc_csc_rtp_t  *p_csc  _AND_
            rohc_U8bit_t    count   _AND_
            rohc_U8bit_t    *index_list
    )
{
    rohc_r_sw_t     *p_sw_entry;
    rohc_U32bit_t       sn_no;
    ROHC_ENTER_FN("rohc_c_add_r_mode_ext_hdr_p1_sw");

    if(p_csc->dont_update_csc_flag == ROHC_FALSE)
    {
        sn_no = ROHC_CSC_GET_SN_NO(p_csc);

        p_sw_entry = &(ROHC_CSC_GET_R_EXT_SW_NEXT(p_csc));
        p_sw_entry->icount = count;
        p_sw_entry->rtp_sn_no = sn_no;
		/* Klocwork fix  - SPR 3532*/
        if((count > 0)&&(count < ROHC_MAX_ENTRIES_IN_TT))
        {
            ROHC_MEMCPY(p_sw_entry->index_list, index_list, count);
        }
        ROHC_CSC_SET_R_EXT_SW_NIDX(p_csc, 
            (ROHC_CSC_GET_R_EXT_SW_NIDX(p_csc) + 1));

        if(ROHC_CSC_GET_R_EXT_SW_NIDX(p_csc) >= ROHC_MAX_ENTRIES_IN_SW)
        {
            ROHC_CSC_SET_R_EXT_SW_NIDX(p_csc, 0);
        }
    }
    
    ROHC_EXIT_FN("rohc_c_add_r_mode_ext_hdr_p1_sw");
}
/*****************************************************************************
**      FUNCTION:
**          rohc_c_add_r_mode_ext_hdr_p2_sw
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to add an entry in the R-Mode 
**          Extension header List Sliding Window
**      ARGUMENTS: 
**          The compressor context, number of items in the list and the
**          index list of the items in the translation table is 
**          taken as input. 
**      RETURN VALUE:
**          rohc_void_t
**      NOTES:
*****************************************************************************/
rohc_void_t rohc_c_add_r_mode_ext_hdr_p2_sw ROHC_PARAMS
    (
        (p_csc, count, index_list),
            rohc_csc_udp_t      *p_csc _AND_
            rohc_U8bit_t    count _AND_
            rohc_U8bit_t    *index_list
    )
{
    rohc_r_sw_t     *p_sw_entry;
    rohc_U32bit_t       sn_no;
    ROHC_ENTER_FN("rohc_c_add_r_mode_ext_hdr_p2_sw");

    if(p_csc->dont_update_csc_flag == ROHC_FALSE)
    {
        sn_no = ROHC_CSC_GET_SN_NO(p_csc);

        p_sw_entry = &(ROHC_CSC_GET_R_EXT_SW_NEXT(p_csc));
        p_sw_entry->icount = count;
        p_sw_entry->rtp_sn_no = sn_no;
		/* Klocwork fix  - SPR 3532*/
        if((count > 0)&&(count < ROHC_MAX_ENTRIES_IN_TT))
        {
            ROHC_MEMCPY(p_sw_entry->index_list, index_list, count);
        }
        ROHC_CSC_SET_R_EXT_SW_NIDX(p_csc, 
            (ROHC_CSC_GET_R_EXT_SW_NIDX(p_csc) + 1));

        if(ROHC_CSC_GET_R_EXT_SW_NIDX(p_csc) >= ROHC_MAX_ENTRIES_IN_SW)
        {
            ROHC_CSC_SET_R_EXT_SW_NIDX(p_csc, 0);
        }
    }
    
    ROHC_EXIT_FN("rohc_c_add_r_mode_ext_hdr_p2_sw");
}

/*****************************************************************************
**      FUNCTION:
**          rohc_c_add_p1_r_mode_csrc_sw
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to add an entry in the R-Mode 
**          CSRC List Sliding Window
**      ARGUMENTS:
**          The compressor context, number of items in the list and the
**          index list of the items in the translation table is 
**          taken as input. 
**      RETURN VALUE:
**          rohc_void_t
**      NOTES:
*****************************************************************************/

rohc_void_t rohc_c_add_p1_r_mode_csrc_sw ROHC_PARAMS
    (
        (p_csc, count, index_list),
            rohc_csc_rtp_t  *p_csc _AND_
            rohc_U8bit_t    count _AND_
            rohc_U8bit_t    *index_list
    )
{
    rohc_r_sw_t     *p_sw_entry;
    rohc_U32bit_t       sn_no;
    ROHC_ENTER_FN("rohc_c_add_p1_r_mode_csrc_sw");

    if(p_csc->dont_update_csc_flag == ROHC_FALSE)
    {
        sn_no = ROHC_CSC_GET_SN_NO(p_csc);
        p_sw_entry = &(ROHC_CSC_GET_R_CSRC_SW_NEXT(p_csc));
        p_sw_entry->icount = count;
        p_sw_entry->rtp_sn_no = sn_no;
		/* SPR 3532 - Klocwork Fix */
        if((count > 0)&&(count < ROHC_MAX_ENTRIES_IN_TT))
        {
            ROHC_MEMCPY(p_sw_entry->index_list, index_list, count);
        }
        ROHC_CSC_SET_R_CSRC_SW_NIDX(p_csc, 
            ((ROHC_CSC_GET_R_CSRC_SW_NIDX(p_csc)) + 1));
    
        if(ROHC_CSC_GET_R_CSRC_SW_NIDX(p_csc) >= ROHC_MAX_ENTRIES_IN_SW)
        {
            ROHC_CSC_SET_R_CSRC_SW_NIDX(p_csc, 0);
        }
    }

    ROHC_EXIT_FN("rohc_c_add_p1_r_mode_csrc_sw");
}

/*****************************************************************************
**      FUNCTION: 
**          rohc_c_add_uo_mode_ext_hdr_p1_sw
**
******************************************************************************
**      DESCRIPTION:
**          This function is used to add an entry in the U/O-Mode 
**          IP Extension header List Sliding Window
**      ARGUMENTS:
**          The compressor context, number of items in the list and the
**          index list of the items in the translation table is 
**          taken as input.
**      RETURN VALUE:
**          rohc_void_t
**      NOTES:
*****************************************************************************/
rohc_void_t   rohc_c_add_uo_mode_ext_hdr_p1_sw ROHC_PARAMS
    (
        (p_csc, gen_id, count, index_list),
            rohc_csc_rtp_t  *p_csc      _AND_
            rohc_U8bit_t    gen_id      _AND_
            rohc_U8bit_t    count       _AND_
            rohc_U8bit_t    *index_list
    )
{
    rohc_uo_sw_t    *p_uo_sw_entry;
    rohc_U16bit_t   sn_no = ROHC_CSC_GET_SN_NO(p_csc);

    ROHC_ENTER_FN("rohc_c_add_uo_mode_ext_hdr_p1_sw");
    
    if (p_csc->dont_update_csc_flag != ROHC_TRUE)
    {
        p_uo_sw_entry = &(ROHC_CSC_GET_UO_EXT_SW_NEXT(p_csc));
        p_uo_sw_entry->gen_id = gen_id;
        p_uo_sw_entry->icount = count;
        p_uo_sw_entry->gen_count = 1;
		/* SPR 3532 - Klocwork fix */
        if((count > 0)&&(count < ROHC_MAX_ENTRIES_IN_TT))
        {
            ROHC_MEMCPY(p_uo_sw_entry->index_list, index_list, count);
        }
        ROHC_CSC_SET_UO_EXT_SW_NIDX(p_csc, 
            (ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc) + 1));

        if(ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc) >= ROHC_MAX_ENTRIES_IN_SW)
        {
            ROHC_CSC_SET_UO_EXT_SW_NIDX(p_csc, 0);
        }

        p_uo_sw_entry->sn_no_list[p_uo_sw_entry->gen_count - 1] = sn_no;
    }

    ROHC_EXIT_FN("rohc_c_add_uo_mode_ext_hdr_p1_sw");
}
/*****************************************************************************
**      FUNCTION: 
**          rohc_c_add_uo_mode_ext_hdr_p2_sw
**
******************************************************************************
**      DESCRIPTION:
**          This function is used to add an entry in the U/O-Mode 
**          IP Extension header List Sliding Window
**      ARGUMENTS:
**          The compressor context, number of items in the list and the
**          index list of the items in the translation table is 
**          taken as input.
**      RETURN VALUE:
**          rohc_void_t
**      NOTES:
*****************************************************************************/
rohc_void_t   rohc_c_add_uo_mode_ext_hdr_p2_sw ROHC_PARAMS
    (
        (p_csc, gen_id, count, index_list),
            rohc_csc_udp_t      *p_csc _AND_
            rohc_U8bit_t    gen_id _AND_
            rohc_U8bit_t    count _AND_
            rohc_U8bit_t    *index_list
    )
{
    rohc_uo_sw_t    *p_uo_sw_entry;
    rohc_U16bit_t   sn_no = ROHC_CSC_GET_SN_NO(p_csc);

    ROHC_ENTER_FN("rohc_c_add_uo_mode_ext_hdr_p2_sw");
    
    if (p_csc->dont_update_csc_flag != ROHC_TRUE)
    {
        p_uo_sw_entry = &(ROHC_CSC_GET_UO_EXT_SW_NEXT(p_csc));
        p_uo_sw_entry->gen_id = gen_id;
        p_uo_sw_entry->icount = count;
        p_uo_sw_entry->gen_count = 1;
		/* SPR 3532 - Klocwork fix */
        if((count > 0)&&(count < ROHC_MAX_ENTRIES_IN_TT))
        {
            ROHC_MEMCPY(p_uo_sw_entry->index_list, index_list, count);
        }
        ROHC_CSC_SET_UO_EXT_SW_NIDX(p_csc, 
            (ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc) + 1));

        if(ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc) >= ROHC_MAX_ENTRIES_IN_SW)
        {
            ROHC_CSC_SET_UO_EXT_SW_NIDX(p_csc, 0);
        }

        p_uo_sw_entry->sn_no_list[p_uo_sw_entry->gen_count - 1] = sn_no;
    }

    ROHC_EXIT_FN("rohc_c_add_uo_mode_ext_hdr_p2_sw");
}

/*****************************************************************************
**      FUNCTION: 
**          rohc_c_add_p1_uo_mode_csrc_sw
**
******************************************************************************
**      DESCRIPTION:
**          This function is used to add an entry in the U/O-Mode 
**          CSRC List Sliding Window
**      ARGUMENTS:
**          The compressor context, number of items in the list and the
**          index list of the items in the translation table is 
**          taken as input.
**      RETURN VALUE:
**          rohc_void_t
**      NOTES:
*****************************************************************************/

rohc_void_t   rohc_c_add_p1_uo_mode_csrc_sw ROHC_PARAMS
    (
        (p_csc, gen_id, count, index_list),
            rohc_csc_rtp_t  *p_csc _AND_
            rohc_U8bit_t    gen_id _AND_
            rohc_U8bit_t    count _AND_
            rohc_U8bit_t    *index_list
    )
{
    rohc_uo_sw_t    *p_uo_sw_entry;
    rohc_U16bit_t   sn_no = ROHC_CSC_GET_SN_NO(p_csc);

    ROHC_ENTER_FN("rohc_c_add_p1_uo_mode_csrc_sw");

    if (p_csc->dont_update_csc_flag != ROHC_TRUE)
    {
        p_uo_sw_entry = &(ROHC_CSC_GET_UO_CSRC_SW_NEXT(p_csc));
        p_uo_sw_entry->gen_id = gen_id;
        p_uo_sw_entry->icount = count;
        p_uo_sw_entry->gen_count = 1;
		/* SPR 3532 - klocwork fix */
        if((count > 0)&&(count < ROHC_MAX_ENTRIES_IN_TT))
        {
            ROHC_MEMCPY(p_uo_sw_entry->index_list, index_list, count);
        }
        ROHC_CSC_SET_UO_CSRC_SW_NIDX(p_csc, 
            (ROHC_CSC_GET_UO_CSRC_SW_NIDX(p_csc) + 1));

        if(ROHC_CSC_GET_UO_CSRC_SW_NIDX(p_csc) >= ROHC_MAX_ENTRIES_IN_SW)
        {
            ROHC_CSC_SET_UO_CSRC_SW_NIDX(p_csc, 0);
        }

        p_uo_sw_entry->sn_no_list[p_uo_sw_entry->gen_count - 1] = sn_no;
    }

    ROHC_EXIT_FN("rohc_c_add_p1_uo_mode_csrc_sw");
}

