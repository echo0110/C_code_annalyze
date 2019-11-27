/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dled_slw_opr.c,v 1.3.36.2 2010/11/09 05:20:11 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains functions for updation of Sliding
 *                     Window at Deompressor side
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dled_slw_opr.c,v $
 * Revision 1.3.36.2  2010/11/09 05:20:11  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.36.1  2010/10/13 03:51:47  gur22059
 * Comment Incorporation after reduction in context block
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
#include "rohc_typ.h"
#include "rohc_def.h"
#include "rohc_d_typ.h"
#include "rohc_dcom_mcr.h"
#include "rohc_gcom_mcr.h"
#include "rohc_trace_mcr.h"
#include "rohc_sl_windows.h"
#include "rohc_led_mcr.h"
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




/*****************************************************************
**
**    	FUNCTION NAME
**        	rohc_d_update_p1_uo_csrc_sw
*******************************************************************
**    	DESCRIPTION
**			This function is used to update the decompressor 
**			side U/O Mode CSRC sliding Window
**			The list is inserted only if it had a gen_id and SW does not
**			contain any entry with same gen_id
**    	INPUTS
**         	p_dsc - The de-compressor stream context is taken as input.
**    	RETURNS
**        	void
**		NOTES
**			0xFF indicates list is not accompanied by gen_id
******************************************************************/

rohc_void_t	rohc_d_update_p1_uo_csrc_sw	ROHC_PARAMS
(	(p_dsc), rohc_dsc_rtp_t	*p_dsc)
{
    rohc_U8bit_t        count = 0;
	rohc_dpib_t		*p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
	rohc_U8bit_t	    ref_id = 0;

	ROHC_ENTER_FN("rohc_d_update_p1_uo_csrc_sw");

	if (p_dpib->csrc_uo_sw.gen_id != 0xFF)
	{
		/* Implies its a context updating packet */
    	for(count=ROHC_DSC_GET_UO_CSRC_SW_REF_IDX(p_dsc); 
            count<ROHC_DSC_GET_UO_CSRC_SW_NIDX(p_dsc); count++)
    	{
        	if(ROHC_DSC_GET_UO_CSRC_SW(p_dsc, count).gen_id == 
			p_dpib->csrc_uo_sw.gen_id)
        	{
           	 	break;
        	}
    	}
    	if(count == ROHC_DSC_GET_UO_CSRC_SW_NIDX(p_dsc))
    	{
        	/* Item with that gen_id not found, hence insert it */
			p_dsc->rohc_d_uo_csrc_sw.d_uo_csrc_sw
            [p_dsc->rohc_d_uo_csrc_sw.uo_csrc_sw_next_free_index] = 
                                                        p_dpib->csrc_uo_sw;
        	ROHC_DSC_SET_UO_CSRC_SW_NIDX(p_dsc, 
					ROHC_DSC_GET_UO_CSRC_SW_NIDX(p_dsc) + 1);
			if(ROHC_DSC_GET_UO_CSRC_SW_NIDX(p_dsc) >= ROHC_MAX_ENTRIES_IN_SW)
			{
				ROHC_DSC_SET_UO_CSRC_SW_NIDX(p_dsc, 0);
			}
    	}
		ROHC_DSC_SET_UO_CSRC_SW_REF_IDX(p_dsc, p_dpib->csrc_ref_index);
		ref_id = p_dpib->csrc_uo_sw.gen_id;
		p_dsc->map_uo_csrc_ref_id_n_idx[ref_id] = p_dpib->csrc_ref_index;
	}
	ROHC_EXIT_FN("rohc_d_update_p1_uo_csrc_sw");
}

/*****************************************************************
**
**    	FUNCTION NAME
**        	rohc_d_update_p1_uo_ext_sw
**
*******************************************************************
**
**    	DESCRIPTION
**			This function is used to update the decompressor 
**			side U/O Mode Extension header sliding Window
**			The list is inserted only if it had a gen_id and SW does not
**			contain any entry with same gen_id
**    	INPUTS
**         	p_dsc - The de-compressor stream context is taken as input.
**			p_sw - Pointer to Sliding Window
**    	RETURNS
**        	void
**		NOTES
******************************************************************/

rohc_void_t		rohc_d_update_p1_uo_ext_sw	ROHC_PARAMS
(	(p_dsc, p_sw), 
    rohc_dsc_rtp_t *p_dsc _AND_	
    rohc_d_uo_sw_t *p_sw)
{
    rohc_U8bit_t        count = 0;
	rohc_dpib_t		*p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
	rohc_U8bit_t	    ref_id = 0;
	ROHC_ENTER_FN("rohc_d_update_p1_uo_ext_sw");
	if (p_sw->gen_id != 0xFF)
	{
		/* Implies its a context updating packet */
    	for(count=ROHC_DSC_GET_UO_EXT_SW_REF_IDX(p_dsc); 
            count<ROHC_DSC_GET_UO_EXT_SW_NIDX(p_dsc); count++)
    	{
        	if(ROHC_DSC_GET_UO_EXT_SW(p_dsc, count).gen_id == 
					p_sw->gen_id)
        	{
            	break;
        	}
    	}
    	if(count == ROHC_DSC_GET_UO_EXT_SW_NIDX(p_dsc))
    	{
        	/* Item with that gen_id not found, hence insert it */
			ROHC_DSC_GET_UO_EXT_SW(p_dsc, count).gen_id = p_sw->gen_id;
			ROHC_DSC_GET_UO_EXT_SW(p_dsc, count).icount = p_sw->icount;
			ROHC_MEMCPY(ROHC_DSC_GET_UO_EXT_SW(p_dsc, count).index_list,
				p_sw->index_list, p_sw->icount);

        	ROHC_DSC_SET_UO_EXT_SW_NIDX(p_dsc, 
				ROHC_DSC_GET_UO_EXT_SW_NIDX(p_dsc) + 1);
			if(ROHC_DSC_GET_UO_EXT_SW_NIDX(p_dsc) >= ROHC_MAX_ENTRIES_IN_SW)
			{
				ROHC_DSC_SET_UO_EXT_SW_NIDX(p_dsc, 0);
			}
    	}
		ROHC_DSC_SET_UO_EXT_SW_REF_IDX(p_dsc, p_dpib->ext_hdr_ref_index);
		ref_id = p_sw->gen_id;
		p_dsc->map_uo_ext_ref_id_n_idx[ref_id] = p_dpib->ext_hdr_ref_index;
	}
	ROHC_EXIT_FN("rohc_d_update_p1_uo_ext_sw");
}

/*****************************************************************
**
**    	FUNCTION NAME
**        	rohc_d_update_p2_uo_ext_sw
**
*******************************************************************
**
**    	DESCRIPTION
**			This function is used to update the decompressor 
**			side U/O Mode Extension header sliding Window
**			The list is inserted only if it had a gen_id and SW does not
**			contain any entry with same gen_id
**    	INPUTS
**         	p_dsc - The de-compressor stream context is taken as input.
**			p_sw - Pointer to Sliding Window
**    	RETURNS
**        	void
**		NOTES
******************************************************************/

rohc_void_t		rohc_d_update_p2_uo_ext_sw	ROHC_PARAMS
(	(p_dsc, p_sw), rohc_dsc_udp_t *p_dsc _AND_	rohc_d_uo_sw_t *p_sw)
{
    rohc_U8bit_t    count = 0;
	rohc_dpib_t		*p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
	rohc_U8bit_t	ref_id = 0;
	ROHC_ENTER_FN("rohc_d_update_p2_uo_ext_sw");
	if (p_sw->gen_id != 0xFF)
	{
		/* Implies its a context updating packet */
    	for(count=ROHC_DSC_GET_UO_EXT_SW_REF_IDX(p_dsc); 
                count<ROHC_DSC_GET_UO_EXT_SW_NIDX(p_dsc); count++)
    	{
        	if(ROHC_DSC_GET_UO_EXT_SW(p_dsc, count).gen_id == 
					p_sw->gen_id)
        	{
            	break;
        	}
    	}
    	if(count == ROHC_DSC_GET_UO_EXT_SW_NIDX(p_dsc))
    	{
        	/* Item with that gen_id not found, hence insert it */
			ROHC_DSC_GET_UO_EXT_SW(p_dsc, count).gen_id = p_sw->gen_id;
			ROHC_DSC_GET_UO_EXT_SW(p_dsc, count).icount = p_sw->icount;
			ROHC_MEMCPY(ROHC_DSC_GET_UO_EXT_SW(p_dsc, count).index_list,
				p_sw->index_list, p_sw->icount);

        	ROHC_DSC_SET_UO_EXT_SW_NIDX(p_dsc, 
				ROHC_DSC_GET_UO_EXT_SW_NIDX(p_dsc) + 1);
			if(ROHC_DSC_GET_UO_EXT_SW_NIDX(p_dsc) >= ROHC_MAX_ENTRIES_IN_SW)
			{
				ROHC_DSC_SET_UO_EXT_SW_NIDX(p_dsc, 0);
			}
    	}
		ROHC_DSC_SET_UO_EXT_SW_REF_IDX(p_dsc, p_dpib->ext_hdr_ref_index);
		ref_id = p_sw->gen_id;
		p_dsc->map_uo_ext_ref_id_n_idx[ref_id] = p_dpib->ext_hdr_ref_index;
	}
	ROHC_EXIT_FN("rohc_d_update_p2_uo_ext_sw");
}

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_d_update_p1_r_ext_sw
**
*******************************************************************
**
**    DESCRIPTION
**			This function is used to update the decompressor 
**			side R Mode extension header sliding Window
**
**    INPUTS
**         	p_dsc - The de-compressor stream context is taken as input.
**			p_sw - Pointer to Sliding Window
**		 	ref_index - Reference index 
**
**    RETURNS
**        void
**
******************************************************************/
rohc_void_t	rohc_d_update_p1_r_ext_sw	ROHC_PARAMS
(	(p_dsc, p_sw, ref_index),
	rohc_dsc_rtp_t	*p_dsc	_AND_
	rohc_r_sw_t	    *p_sw	_AND_
	rohc_U8bit_t	ref_index
)
{
	rohc_U8bit_t	ref_id;
	ROHC_ENTER_FN("rohc_d_update_p1_r_ext_sw");
	ROHC_DSC_GET_R_EXT_SW_NEXT(p_dsc) = *p_sw;
	ROHC_DSC_SET_R_EXT_SW_NIDX(p_dsc, ROHC_DSC_GET_R_EXT_SW_NIDX(p_dsc) + 1);
	if(ROHC_DSC_GET_R_EXT_SW_NIDX(p_dsc) >= ROHC_MAX_ENTRIES_IN_SW)
	{
		ROHC_DSC_SET_R_EXT_SW_NIDX(p_dsc, 0);
	}

	ROHC_DSC_SET_R_EXT_SW_REF_IDX(p_dsc, ref_index);
	ref_id = ROHC_GET_LOWER_NIBBLE(p_sw->rtp_sn_no);
	/* spr 3532 klocwork fix */
	if(ref_id < ROHC_MAX_ENTRIES_IN_TT)
		p_dsc->map_r_ext_ref_id_n_idx[ref_id] = ref_index;
	ROHC_EXIT_FN("rohc_d_update_p1_r_ext_sw");
}

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_d_update_p2_r_ext_sw
**
*******************************************************************
**
**    DESCRIPTION
**			This function is used to update the decompressor 
**			side R Mode extension header sliding Window
**
**    INPUTS
**         	p_dsc - The de-compressor stream context is taken as input.
**			p_sw - Pointer to Sliding Window
**		 	ref_index - Reference index 
**
**    RETURNS
**        void
**
******************************************************************/
rohc_void_t	rohc_d_update_p2_r_ext_sw	ROHC_PARAMS
(	(p_dsc, p_sw, ref_index),
	rohc_dsc_udp_t	*p_dsc	_AND_
	rohc_r_sw_t	*p_sw	_AND_
	rohc_U8bit_t	ref_index
)
{
	rohc_U8bit_t	ref_id;
	ROHC_ENTER_FN("rohc_d_update_p2_r_ext_sw");
	ROHC_DSC_GET_R_EXT_SW_NEXT(p_dsc) = *p_sw;
	ROHC_DSC_SET_R_EXT_SW_NIDX(p_dsc, ROHC_DSC_GET_R_EXT_SW_NIDX(p_dsc) + 1);
	if(ROHC_DSC_GET_R_EXT_SW_NIDX(p_dsc) >= ROHC_MAX_ENTRIES_IN_SW)
	{
		ROHC_DSC_SET_R_EXT_SW_NIDX(p_dsc, 0);
	}

	ROHC_DSC_SET_R_EXT_SW_REF_IDX(p_dsc, ref_index);
	ref_id = ROHC_GET_LOWER_NIBBLE(p_sw->rtp_sn_no);
	/* spr 3532 klocwork fix */
	if(ref_id < ROHC_MAX_ENTRIES_IN_TT)
		p_dsc->map_r_ext_ref_id_n_idx[ref_id] = ref_index;
	ROHC_EXIT_FN("rohc_d_update_p2_r_ext_sw");
}

/*****************************************************************
**
**    	FUNCTION NAME
**        	rohc_d_update_p1_r_csrc_sw
*******************************************************************
**    	DESCRIPTION
**			This function is used to update the decompressor 
**			side R Mode CSRC sliding Window
**    	INPUTS
**         	p_dsc - The de-compressor stream context is taken as input.
**			p_sw - Pointer to Sliding Window
**		 	ref_index - Reference index 
**    	RETURNS
**        	void
**		NOTES
******************************************************************/

rohc_void_t	rohc_d_update_p1_r_csrc_sw	ROHC_PARAMS
(	(p_dsc, p_sw, ref_index),
	rohc_dsc_rtp_t	*p_dsc	_AND_
	rohc_r_sw_t	    *p_sw	_AND_
	rohc_U8bit_t	ref_index
)
{
	rohc_U8bit_t	ref_id = 0;
	ROHC_ENTER_FN("rohc_d_update_p1_r_csrc_sw");
	ROHC_DSC_GET_R_CSRC_SW_NEXT(p_dsc) = *p_sw;
	ROHC_DSC_SET_R_CSRC_SW_NIDX(p_dsc, ROHC_DSC_GET_R_CSRC_SW_NIDX(p_dsc) + 1);
	if(ROHC_DSC_GET_R_CSRC_SW_NIDX(p_dsc) >= ROHC_MAX_ENTRIES_IN_SW)
	{
		ROHC_DSC_SET_R_CSRC_SW_NIDX(p_dsc, 0);
	}

	ROHC_DSC_SET_R_CSRC_SW_REF_IDX(p_dsc, ref_index);
	ref_id = ROHC_GET_LOWER_NIBBLE(p_sw->rtp_sn_no);
	/* spr 3532 Klocwork Fix */
	if(ref_id < ROHC_MAX_ENTRIES_IN_CSRC_TT)
	  p_dsc->map_r_csrc_ref_id_n_idx[ref_id] = ref_index;
	ROHC_EXIT_FN("rohc_d_update_p1_r_csrc_sw");
}
 
 
