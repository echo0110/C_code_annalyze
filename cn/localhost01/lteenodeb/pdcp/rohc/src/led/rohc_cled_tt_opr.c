/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cled_tt_opr.c,v 1.3.36.3 2010/11/09 05:18:14 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the funtions for various Translation
 *                     Table operations at the compressor side.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_cled_tt_opr.c,v $
 * Revision 1.3.36.3  2010/11/09 05:18:14  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.36.2  2010/10/13 03:50:16  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.36.1  2010/09/23 05:11:18  gur22059
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
#include "rohc_trace_mcr.h"
#include "rohc_led_def.h"
#include "rohc_led_mcr.h"
#include "rohc_led_proto.h"

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
**    	FUNCTION:
**        	rohc_c_add_n_chk_p1_r_ext_hdr_tt
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to add an entry in the R-Mode EXT HDR
**			Translation Table. This function also checks for duplicate
**			entries by comparing full items. 
**    	ARGUMENTS:
**			The R-Mode EXT HDR Translation Table and the 
**			p_item - item to be inserted is passed as input.
**			p_csc - Pointer to Compresor Context 
** 			p_item_present -
**			p_ecode - Pointer to Error Code
**    	RETURN VALUE:
**        	rohc_U8bit_t
**		NOTES:
**
*****************************************************************************/
rohc_U8bit_t	rohc_c_add_n_chk_p1_r_ext_hdr_tt ROHC_PARAMS 
	(
		(p_item, p_csc, p_item_present, p_ecode),
			rohc_list_node_t	*p_item         _AND_
		 	rohc_csc_rtp_t		*p_csc          _AND_
			rohc_U8bit_t		*p_item_present	_AND_
			rohc_error_code_t	*p_ecode
	)
{
	r_ext_hdr_tt_t		*p_tt_entry;
	rohc_U32bit_t		length = ((rohc_protocol_node_t *)p_item)->length;
	rohc_U32bit_t		l_index = ROHC_CSC_GET_R_EXT_TT_NIDX(p_csc), i;

	ROHC_ENTER_FN("rohc_c_add_n_chk_p1_r_ext_hdr_tt");

	/* Before adding item needs to be checked for duplicate entry */
	for(i = 0; i<l_index; i++)
	{
		p_tt_entry = &(ROHC_CSC_GET_R_EXT_TT(p_csc, i));
        /* +- SPR 17777 */
		if(ROHC_SUCCESS == 
			rohc_c_compare_p1_ext_hdr_item(p_tt_entry->p_item, p_item))
        /* +- SPR 17777 */
		{
			/* Item is present in TT */
			*p_item_present = ROHC_TRUE;
			ROHC_EXIT_FN("rohc_c_add_n_chk_p1_r_ext_hdr_tt");
			return i;
		}
	}

	p_tt_entry = &(ROHC_CSC_GET_R_EXT_TT_NEXT(p_csc));

	p_tt_entry->known = 0;

	/* Right now p_item points to rohc_protocol_node_t -- might change later */

	p_tt_entry->p_item = (rohc_U8bit_t *)ROHC_MALLOC(length);
	if(ROHC_NULL == p_tt_entry->p_item)
	{
		*p_ecode = ROHC_MEMORY_FAILURE;
		ROHC_EXIT_FN("rohc_c_add_n_chk_p1_r_ext_hdr_tt");
		return ROHC_FAILURE;
	}
	ROHC_MEMCPY(p_tt_entry->p_item, (rohc_U8bit_t *)p_item, length);

	ROHC_CSC_SET_R_EXT_TT_NIDX(p_csc, l_index + 1);

	ROHC_EXIT_FN("rohc_c_add_n_chk_p1_r_ext_hdr_tt");
	return  l_index;
}
/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_add_n_chk_p2_r_ext_hdr_tt
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to add an entry in the R-Mode EXT HDR
**			Translation Table. This function also checks for duplicate
**			entries by comparing full items. 
**    	ARGUMENTS:
**			The R-Mode EXT HDR Translation Table
**			p_item - item to be inserted is passed as input.
**			p_csc - Pointer to Compresor Context 
** 			p_item_present -
**			p_ecode - Pointer to Error Code
**    	RETURN VALUE:
**        	rohc_U8bit_t
**		NOTES:
**
*****************************************************************************/
rohc_U8bit_t	rohc_c_add_n_chk_p2_r_ext_hdr_tt ROHC_PARAMS 
	(
		(p_item, p_csc, p_item_present, p_ecode),
			rohc_list_node_t	*p_item _AND_
		 	rohc_csc_udp_t		*p_csc _AND_
			rohc_U8bit_t		*p_item_present	_AND_
			rohc_error_code_t	*p_ecode
	)
{
	r_ext_hdr_tt_t		*p_tt_entry;
	rohc_U32bit_t		length = ((rohc_protocol_node_t *)p_item)->length;
	rohc_U32bit_t		l_index = ROHC_CSC_GET_R_EXT_TT_NIDX(p_csc), i;

	ROHC_ENTER_FN("rohc_c_add_n_chk_p2_r_ext_hdr_tt");

	/* Before adding item needs to be checked for duplicate entry */
	for(i = 0; i<l_index; i++)
	{
		p_tt_entry = &(ROHC_CSC_GET_R_EXT_TT(p_csc, i));
        /* +- SPR 17777 */
		if(ROHC_SUCCESS == 
			rohc_c_compare_p2_ext_hdr_item(p_tt_entry->p_item, p_item))
        /* +- SPR 17777 */
		{
			/* Item is present in TT */
			*p_item_present = ROHC_TRUE;
			ROHC_EXIT_FN("rohc_c_add_n_chk_p2_r_ext_hdr_tt");
			return i;
		}
	}

	p_tt_entry = &(ROHC_CSC_GET_R_EXT_TT_NEXT(p_csc));

	p_tt_entry->known = 0;

	/* Right now p_item points to rohc_protocol_node_t -- might change later */

	p_tt_entry->p_item = (rohc_U8bit_t *)ROHC_MALLOC(length);
	if(ROHC_NULL == p_tt_entry->p_item)
	{
		*p_ecode = ROHC_MEMORY_FAILURE;
		ROHC_EXIT_FN("rohc_c_add_n_chk_p2_r_ext_hdr_tt");
		return ROHC_FAILURE;
	}
	ROHC_MEMCPY(p_tt_entry->p_item, (rohc_U8bit_t *)p_item, length);

	ROHC_CSC_SET_R_EXT_TT_NIDX(p_csc, l_index + 1);

	ROHC_EXIT_FN("rohc_c_add_n_chk_p2_r_ext_hdr_tt");
	return  l_index;
}


#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
**      FUNCTION:
**          rohc_c_add_r_mode_ext_hdr_tt
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to add an entry in the R-Mode EXT HDR
**          Translation Table.
**      ARGUMENTS:
**          The R-Mode EXT HDR Translation Table and 
**			p_item - the item to be inserted is passed as input. 
**			p_csc - Pointer to Compressor Context 
**			P_ecode - Pointer to Error Code
**      RETURN VALUE:
**          rohc_U8bit_t
**      NOTES:
**
*****************************************************************************/

rohc_U8bit_t    rohc_c_add_r_mode_ext_hdr_tt ROHC_PARAMS
    (
        (p_item, p_csc, p_ecode),
            rohc_list_node_t    *p_item _AND_
            rohc_csc_t          *p_csc	_AND_
			rohc_error_code_t	*p_ecode
    )
{
    r_ext_hdr_tt_t      *p_tt_entry;
    rohc_U32bit_t       length = ((rohc_protocol_node_t *)p_item)->length;
    rohc_U32bit_t       l_index = ROHC_CSC_GET_R_EXT_TT_NIDX(p_csc);

    ROHC_ENTER_FN("rohc_c_add_r_mode_ext_hdr_tt");

	p_tt_entry = &(ROHC_CSC_GET_R_EXT_TT_NEXT(p_csc));

	p_tt_entry->known = 0;

	/* Right now p_item points to rohc_protocol_node_t -- might change later */

	p_tt_entry->p_item = (rohc_U8bit_t *)ROHC_MALLOC(length);
	if(ROHC_NULL == p_tt_entry->p_item)
	{
		*p_ecode = ROHC_MEMORY_FAILURE;
		ROHC_EXIT_FN("rohc_c_add_r_mode_ext_hdr_tt");
		return ROHC_FAILURE;
	}
	ROHC_MEMCPY(p_tt_entry->p_item, (rohc_U8bit_t *)p_item, length);

	ROHC_CSC_SET_R_EXT_TT_NIDX(p_csc, l_index + 1);

    ROHC_EXIT_FN("rohc_c_add_r_mode_ext_hdr_tt");
	return l_index;
}
#endif

/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_add_n_chk_p1_uo_ext_hdr_tt
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to add an entry in the U/O-Mode EXT HDR
**			Translation Table. This function also checks for duplicate
**			entries in the table by comparing all items.
**    	ARGUMENTS:
**			p_item -the item to be inserted is passed as input.
**			p_csc - Pointer to Compressed Context 
**			p_item_present 
**			p_index - Pointer to index 
**			p_ecode - Pointer to Error code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**
*****************************************************************************/

rohc_return_t	rohc_c_add_n_chk_p1_uo_ext_hdr_tt ROHC_PARAMS
	(
		(p_item, p_csc, p_item_present, p_index, p_ecode),
			rohc_list_node_t	*p_item         _AND_
			rohc_csc_rtp_t		*p_csc          _AND_
			rohc_U8bit_t		*p_item_present	_AND_
			rohc_U32bit_t		*p_index        _AND_
			rohc_error_code_t	*p_ecode
	)
{
	uo_ext_hdr_tt_t		*p_tt_entry;
	rohc_U32bit_t		length = ((rohc_protocol_node_t *)p_item)->length;
	rohc_U32bit_t		i;

	ROHC_ENTER_FN("rohc_c_add_n_chk_p1_uo_ext_hdr_tt");
	*p_index = ROHC_CSC_GET_UO_EXT_TT_NIDX(p_csc);

	/* Before adding item needs to be checked for duplicate entry */
	for(i = 0; i< *p_index; i++)
	{
		p_tt_entry = &(ROHC_CSC_GET_UO_EXT_TT(p_csc, i));
        /* +- SPR 17777 */
		if(ROHC_SUCCESS == 
			rohc_c_compare_p1_ext_hdr_item(p_tt_entry->p_item, p_item))
        /* +- SPR 17777 */
		{
			/* Item is present in TT */
			*p_item_present = ROHC_TRUE;
			ROHC_EXIT_FN("rohc_c_add_n_chk_p1_uo_ext_hdr_tt");
			*p_index = i;
			return ROHC_SUCCESS;
		}
	}

	p_tt_entry = &(ROHC_CSC_GET_UO_EXT_TT_NEXT(p_csc));

	p_tt_entry->known = 0;
	p_tt_entry->counter = 1;

	p_tt_entry->p_item = (rohc_U8bit_t *)
                            ROHC_MALLOC(sizeof(rohc_protocol_node_t));
	if(ROHC_NULL == p_tt_entry->p_item)
	{
		*p_ecode = ROHC_MEMORY_FAILURE;
		ROHC_EXIT_FN("rohc_c_add_n_chk_p1_uo_ext_hdr_tt");
		return ROHC_FAILURE;
	}
    ((rohc_protocol_node_t *)(p_tt_entry->p_item))->length = length;
    ((rohc_protocol_node_t *)(p_tt_entry->p_item))->protocol =
        ((rohc_protocol_node_t *)p_item)->protocol;
    ((rohc_protocol_node_t *)(p_tt_entry->p_item))->ptr =
        (rohc_U8bit_t *)ROHC_MALLOC(length);
    ROHC_MEMCPY(((rohc_protocol_node_t *)(p_tt_entry->p_item))->ptr,
            ((rohc_protocol_node_t *)p_item)->ptr, length);

	ROHC_CSC_SET_UO_EXT_TT_NIDX(p_csc,  *p_index + 1);

	ROHC_EXIT_FN("rohc_c_add_n_chk_p1_uo_ext_hdr_tt");
	return ROHC_SUCCESS;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_add_n_chk_p2_uo_ext_hdr_tt
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to add an entry in the U/O-Mode EXT HDR
**			Translation Table. This function also checks for duplicate
**			entries in the table by comparing all items.
**    	ARGUMENTS:
**			The U/O-Mode EXT HDR Translation Table and
**			p_item -the item to be inserted is passed as input.
**			p_csc - Pointer to Compressed Context 
**			p_item_present 
**			p_index - Pointer to index 
**			p_ecode - Pointer to Error code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**
*****************************************************************************/

rohc_return_t	rohc_c_add_n_chk_p2_uo_ext_hdr_tt ROHC_PARAMS
	(
		(p_item, p_csc, p_item_present, p_index, p_ecode),
			rohc_list_node_t	*p_item _AND_
			rohc_csc_udp_t		*p_csc  _AND_
			rohc_U8bit_t		*p_item_present	_AND_
			rohc_U32bit_t		*p_index _AND_
			rohc_error_code_t	*p_ecode
	)
{
	uo_ext_hdr_tt_t		*p_tt_entry;
	rohc_U32bit_t		length = ((rohc_protocol_node_t *)p_item)->length;
	rohc_U32bit_t		i;

	ROHC_ENTER_FN("rohc_c_add_n_chk_p2_uo_ext_hdr_tt");
	*p_index = ROHC_CSC_GET_UO_EXT_TT_NIDX(p_csc);

	/* Before adding item needs to be checked for duplicate entry */
	for(i = 0; i< *p_index; i++)
	{
		p_tt_entry = &(ROHC_CSC_GET_UO_EXT_TT(p_csc, i));
        /* +- SPR 17777 */
		if(ROHC_SUCCESS == 
				rohc_c_compare_p2_ext_hdr_item(p_tt_entry->p_item, p_item))
        /* +- SPR 17777 */
		{
			/* Item is present in TT */
			*p_item_present = ROHC_TRUE;
			ROHC_EXIT_FN("rohc_c_add_n_chk_p2_uo_ext_hdr_tt");
			*p_index = i;
			return ROHC_SUCCESS;
		}
	}

	p_tt_entry = &(ROHC_CSC_GET_UO_EXT_TT_NEXT(p_csc));

	p_tt_entry->known = 0;
	p_tt_entry->counter = 1;

	p_tt_entry->p_item = (rohc_U8bit_t *)ROHC_MALLOC(sizeof(rohc_protocol_node_t));
	if(ROHC_NULL == p_tt_entry->p_item)
	{
		*p_ecode = ROHC_MEMORY_FAILURE;
		ROHC_EXIT_FN("rohc_c_add_n_chk_p2_uo_ext_hdr_tt");
		return ROHC_FAILURE;
	}
    ((rohc_protocol_node_t *)(p_tt_entry->p_item))->length = length;
    ((rohc_protocol_node_t *)(p_tt_entry->p_item))->protocol =
        ((rohc_protocol_node_t *)p_item)->protocol;
    ((rohc_protocol_node_t *)(p_tt_entry->p_item))->ptr =
        (rohc_U8bit_t *)ROHC_MALLOC(length);
    ROHC_MEMCPY(((rohc_protocol_node_t *)(p_tt_entry->p_item))->ptr,
            ((rohc_protocol_node_t *)p_item)->ptr, length);

	ROHC_CSC_SET_UO_EXT_TT_NIDX(p_csc,  *p_index + 1);

	ROHC_EXIT_FN("rohc_c_add_n_chk_p2_uo_ext_hdr_tt");
	return ROHC_SUCCESS;
}


/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_add_n_chk_p1_uo_csrc_tt
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to add an entry in the U/O-Mode CSRC
**			Translation Table. And also checks for duplicate entries.
**    	ARGUMENTS:
**			p_csc - pointer to Compressor Context 
**			csrc_no 
**			p_item_present
**    	RETURN VALUE:
**        	rohc_U8bit_t
**		NOTES:
**
*****************************************************************************/

rohc_U8bit_t	rohc_c_add_n_chk_p1_uo_csrc_tt ROHC_PARAMS
	(
		(p_csc, csrc_no, p_item_present),
			rohc_csc_rtp_t			*p_csc	_AND_
			rohc_U32bit_t			csrc_no _AND_
			rohc_U8bit_t			*p_item_present

	)
{
	uo_csrc_tt_t	*p_tt_entry;
	rohc_U32bit_t	l_index  = ROHC_CSC_GET_UO_CSRC_TT_NIDX(p_csc), i;

	ROHC_ENTER_FN("rohc_c_add_n_chk_p1_uo_csrc_tt");

	/* Before adding item needs to be checked for duplicate entry */
	for(i = 0; i<l_index; i++)
	{
		p_tt_entry = &(ROHC_CSC_GET_UO_CSRC_TT(p_csc, i));
		if(p_tt_entry->item == csrc_no)
		{
			/* Item is present in TT */
			*p_item_present = ROHC_TRUE;
			ROHC_EXIT_FN("rohc_c_add_n_chk_p1_uo_csrc_tt");
			return i;
		}
	}

	p_tt_entry = &(ROHC_CSC_GET_UO_CSRC_TT_NEXT(p_csc));

	p_tt_entry->known = 0;
	p_tt_entry->item = csrc_no;
	p_tt_entry->counter = 1;

	ROHC_CSC_SET_UO_CSRC_TT_NIDX(p_csc, l_index + 1);

	ROHC_EXIT_FN("rohc_c_add_n_chk_p1_uo_csrc_tt");

	return l_index;
}


/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_add_n_chk_p1_r_csrc_tt
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to add an entry in the R-Mode CSRC
**			Translation Table. It also checks for duplicate entries.
**    	ARGUMENTS:
**			p_csc - pointer to Compressor Context 
**			csrc_no 
**			p_item_present
**    	RETURN VALUE:
**        	rohc_U8bit_t
**		NOTES:
**
*****************************************************************************/

rohc_U8bit_t	rohc_c_add_n_chk_p1_r_csrc_tt ROHC_PARAMS
	(
		(p_csc, csrc_no, p_item_present),
			rohc_csc_rtp_t			*p_csc	_AND_
			rohc_U32bit_t			csrc_no _AND_
			rohc_U8bit_t			*p_item_present
	)
{
	r_csrc_tt_t		*p_tt_entry;
	rohc_U32bit_t	l_index = ROHC_CSC_GET_R_CSRC_TT_NIDX(p_csc), i;
	ROHC_ENTER_FN("rohc_c_add_n_chk_p1_r_csrc_tt");

	/* Before adding item needs to be checked for duplicate entry */
	for(i = 0; i<l_index; i++)
	{
		p_tt_entry = &(ROHC_CSC_GET_R_CSRC_TT(p_csc, i));
		if(p_tt_entry->item == csrc_no)
		{
			/* Item is present in TT */
			*p_item_present = ROHC_TRUE;
			ROHC_EXIT_FN("rohc_c_add_n_chk_p1_r_csrc_tt");
			return i;
		}
	}

	p_tt_entry = &(ROHC_CSC_GET_R_CSRC_TT_NEXT(p_csc));

	p_tt_entry->known = 0;
	p_tt_entry->item = csrc_no;

	ROHC_CSC_SET_R_CSRC_TT_NIDX(p_csc, l_index + 1);

	ROHC_EXIT_FN("rohc_c_add_n_chk_p1_r_csrc_tt");
	return l_index;
}


/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_add_sn_p1_r_mode_ext_hdr_tt
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to add a SN in the R-Mode EXT HDR
**			Translation Table.
**    	ARGUMENTS:
**			p_ext_hdr_tt - The pointer to R-Mode EXT HDR Translation Table, 
**			index ,
**			p_csc - Pointer to Compressor Context 
**    	RETURN VALUE:
**        	rohc_void_t
**		NOTES:
**
*****************************************************************************/
rohc_void_t 	rohc_c_add_sn_p1_r_mode_ext_hdr_tt  ROHC_PARAMS
	(
		(p_csc, index, p_ext_hdr_tt),
			rohc_csc_rtp_t		*p_csc _AND_
			rohc_U8bit_t		index _AND_
			r_ext_hdr_tt_t		*p_ext_hdr_tt
	)
{
	rohc_U32bit_t	sn_no, sn_index;
	ROHC_ENTER_FN("rohc_c_add_sn_p1_r_mode_ext_hdr_tt");

	sn_no = ROHC_CSC_GET_SN_NO(p_csc);
	sn_index = ROHC_CSC_GET_R_EXT_TT_SN_IDX(p_csc, index);
	p_ext_hdr_tt->sn_no_list[sn_index] = sn_no;

	ROHC_CSC_INCR_R_EXT_TT_SN_IDX(p_csc, index);

	ROHC_EXIT_FN("rohc_c_add_sn_p1_r_mode_ext_hdr_tt");
}
/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_add_sn_p2_r_mode_ext_hdr_tt
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to add a SN in the R-Mode EXT HDR
**			Translation Table.
**    	ARGUMENTS:
**			p_ext_hdr_tt - The pointer to R-Mode EXT HDR Translation Table, 
**			index ,
**			p_csc - Pointer to Compressor Context 
**    	RETURN VALUE:
**        	rohc_void_t
**		NOTES:
**
*****************************************************************************/
rohc_void_t 	rohc_c_add_sn_p2_r_mode_ext_hdr_tt  ROHC_PARAMS
	(
		(p_csc, index, p_ext_hdr_tt),
			rohc_csc_udp_t		*p_csc _AND_
			rohc_U8bit_t		index _AND_
			r_ext_hdr_tt_t		*p_ext_hdr_tt
	)
{
	rohc_U32bit_t	sn_no, sn_index;
	ROHC_ENTER_FN("rohc_c_add_sn_p2_r_mode_ext_hdr_tt");

	sn_no = ROHC_CSC_GET_SN_NO(p_csc);
	sn_index = ROHC_CSC_GET_R_EXT_TT_SN_IDX(p_csc, index);
	p_ext_hdr_tt->sn_no_list[sn_index] = sn_no;

	ROHC_CSC_INCR_R_EXT_TT_SN_IDX(p_csc, index);

	ROHC_EXIT_FN("rohc_c_add_sn_p2_r_mode_ext_hdr_tt");
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_incr_counter_u_o_ext_hdr_tt
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to increment the counter in the U/O Mode
**			EXT HDR Translation Table.
**    	ARGUMENTS:
**			p_entity - Pointer to Rohc Entity
**			p_ext_hdr_tt - Pointer to U/O-Mode EXT HDR Translation Table
**    	RETURN VALUE:
**        	rohc_void_t
**		NOTES:
**
*****************************************************************************/

rohc_void_t 	rohc_c_incr_counter_uo_ext_hdr_tt ROHC_PARAMS
	(
		(p_entity, p_ext_hdr_tt),
		rohc_entity_t		*p_entity _AND_
		uo_ext_hdr_tt_t 	*p_ext_hdr_tt
	)
{
	ROHC_ENTER_FN("rohc_c_incr_counter_uo_ext_hdr_tt");

	p_ext_hdr_tt->counter++;
	/* Check whether counter has reached MAX value */
	if(p_ext_hdr_tt->counter == p_entity->max_dynamic_info_update_counter)
	{
		/* This implies that now this item can be used as ref item 
		 * Hence the known field will be updated  */
		p_ext_hdr_tt->known = 1;
	}

	ROHC_EXIT_FN("rohc_c_incr_counter_uo_ext_hdr_tt");
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_add_p1_sn_r_mode_csrc_tt
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to add a SN in the R-Mode CSRC
**			Translation Table.
**    	ARGUMENTS:
**			p_csrc_tt - The pointer to R-Mode CSRC Translation Table, 
**			index and the
**			p_csc - Pointer to Compressor Context.
**    	RETURN VALUE:
**        	rohc_void_t
**		NOTES:
**
*****************************************************************************/

rohc_void_t   rohc_c_add_p1_sn_r_mode_csrc_tt ROHC_PARAMS
	(
		(p_csc, index, p_csrc_tt),
            rohc_csc_rtp_t  *p_csc 	_AND_
            rohc_U8bit_t   	index 	_AND_
            r_csrc_tt_t     *p_csrc_tt
	)
{
	rohc_U32bit_t	sn_no, sn_index;
	ROHC_ENTER_FN("rohc_c_add_p1_sn_r_mode_csrc_tt");

	sn_no = ROHC_CSC_GET_SN_NO(p_csc);
	sn_index = ROHC_CSC_GET_R_CSRC_TT_SN_IDX(p_csc, index);
	p_csrc_tt->sn_no_list[sn_index] = sn_no;

	ROHC_CSC_INCR_R_CSRC_TT_SN_IDX(p_csc, index);

	ROHC_EXIT_FN("rohc_c_add_p1_sn_r_mode_csrc_tt");
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_incr_counter_u_o_csrc_tt
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to increment the Counter in the U/O Mode
**			CSRC Translation Table.
**    	ARGUMENTS:
**			p_csrc_tt - The U/O-Mode CSRC Translation Table and 
**			p_entity - The Rohc entity whose counter needs to be incremented
**    	RETURN VALUE:
**        	rohc_void_t
**		NOTES:
**
*****************************************************************************/

rohc_void_t   rohc_c_incr_counter_uo_csrc_tt ROHC_PARAMS
	(
		(p_entity, p_csrc_tt),
		rohc_entity_t			*p_entity	_AND_
        uo_csrc_tt_t     		*p_csrc_tt
	)
{
	ROHC_ENTER_FN("rohc_c_incr_counter_uo_csrc_tt");
	p_csrc_tt->counter++;

	/* Check whether counter has reached MAX value or not */
	if(p_csrc_tt->counter == p_entity->max_dynamic_info_update_counter)
	{
		/* This implies that now this item can be used as ref item 
		 * Hence the known field will be updated  */
		p_csrc_tt->known = 1;
	}

	ROHC_EXIT_FN("rohc_c_incr_counter_uo_csrc_tt");
}


