/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cled_ext_hdr.c,v 1.4.34.3 2010/11/09 05:16:48 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the functions for Compression of IP
 *                     Extension header list.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_cled_ext_hdr.c,v $
 * Revision 1.4.34.3  2010/11/09 05:16:48  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.4.34.2  2010/10/13 03:49:37  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.4.34.1  2010/09/23 05:10:28  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.4  2009/05/27 16:14:06  gur19836
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
#include "rohc_stats_def.h"
#include "rohc_c_typ.h"
#include "rohc_com_def.h"
#include "rohc_g_typ.h"
#include "rohc_gcom_mcr.h"
#include "rohc_protocol_header.h"
#include "rohc_ccom_mcr.h"
#include "rohc_trace_mcr.h"
#include "rohc_db_ext_if.h"
#include "rohc_led_proto.h"
#include "rohc_led_mcr.h"
#include "rohc_led_def.h"
#include "rohc_opt_proto.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/
/* +- SPR 17777 */
extern tickType_t pdcpCurrentTime_g;
/* +- SPR 17777 */
/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
****************************************************************************/
/* + SPR 17439 */
static rohc_return_t         rohcUOExtAckNotRecvdP1  ROHC_PROTO((
                     rohc_entity_t      *p_entity           ,
                     rohc_list_node_t   *ext_hdr_list       ,
                     rohc_csc_rtp_t     *p_csc              ,
                     rohc_U32bit_t      *p_length           ,
                     rohc_U8bit_t       *p_final_comp_list  ,
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] ,
                     rohc_U8bit_t        *p_item_list       ,
                     rohc_U8bit_t        *p_compressed_list  ,
                     rohc_bool_t        item_present_in_tt  ,
                     rohc_U32bit_t      curr_index          ,
                     rohc_error_code_t  *p_ecode));

static rohc_return_t         rohcUOExtAckRecvdP1  ROHC_PROTO((
                     rohc_entity_t      *p_entity           ,
                     rohc_list_node_t   *ext_hdr_list       ,
                     rohc_csc_rtp_t     *p_csc              ,
                     rohc_U32bit_t      *p_length           ,
                     rohc_U8bit_t       *p_final_comp_list  ,
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] ,
                     rohc_U8bit_t        *p_item_list       ,
                     rohc_U8bit_t        *p_compressed_list  ,
			         rohc_list_t	    p_list			    ,
                     rohc_bool_t        item_present_in_tt  ,
                     rohc_U32bit_t      curr_index          ,
                     rohc_error_code_t  *p_ecode));

static rohc_return_t         rohcUOExtAckNotRecvdP2  ROHC_PROTO((
                     rohc_entity_t      *p_entity           ,
                     rohc_list_node_t   *ext_hdr_list       ,
                     rohc_csc_udp_t     *p_csc              ,
                     rohc_U32bit_t      *p_length           ,
                     rohc_U8bit_t       *p_final_comp_list  ,
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] ,
                     rohc_U8bit_t        *p_item_list       ,
                     rohc_U8bit_t        *p_compressed_list  ,
                     rohc_bool_t        item_present_in_tt  ,
                     rohc_U32bit_t      curr_index          ,
                     rohc_error_code_t  *p_ecode));

static rohc_return_t         rohcUOExtAckRecvdP2  ROHC_PROTO((
                     rohc_entity_t      *p_entity           ,
                     rohc_list_node_t   *ext_hdr_list       ,
                     rohc_csc_udp_t     *p_csc              ,
                     rohc_U32bit_t      *p_length           ,
                     rohc_U8bit_t       *p_final_comp_list  ,
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] ,
                     rohc_U8bit_t        *p_item_list       ,
                     rohc_U8bit_t        *p_compressed_list  ,
			         rohc_list_t	    p_list			    ,
                     rohc_bool_t        item_present_in_tt  ,
                     rohc_U32bit_t      curr_index          ,
                     rohc_error_code_t  *p_ecode));

static rohc_return_t         rohcRExtAckNotRecvdP1  ROHC_PROTO((
                     rohc_entity_t      *p_entity           ,
                     rohc_list_node_t   *ext_hdr_list       ,
                     rohc_csc_rtp_t     *p_csc              ,
                     rohc_U32bit_t      *p_length           ,
                     rohc_U8bit_t       *p_final_comp_list  ,
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] ,
                     rohc_U8bit_t        *p_item_list       ,
                     rohc_U8bit_t        *p_compressed_list  ,
                     rohc_bool_t        item_present_in_tt  ,
                     rohc_U32bit_t      curr_index          ,
                     rohc_error_code_t  *p_ecode));

static rohc_return_t         rohcRExtAckRecvdP1  ROHC_PROTO((
                     rohc_entity_t      *p_entity           ,
                     rohc_list_node_t   *ext_hdr_list       ,
                     rohc_csc_rtp_t     *p_csc              ,
                     rohc_U32bit_t      *p_length           ,
                     rohc_U8bit_t       *p_final_comp_list  ,
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] ,
                     rohc_U8bit_t        *p_item_list       ,
                     rohc_U8bit_t        *p_compressed_list  ,
			         rohc_list_t	    p_list			    ,
                     rohc_bool_t        item_present_in_tt  ,
                     rohc_U32bit_t      curr_index          ,
                     rohc_error_code_t  *p_ecode));

static rohc_return_t         rohcRExtAckNotRecvdP2  ROHC_PROTO((
                     rohc_entity_t      *p_entity           ,
                     rohc_list_node_t   *ext_hdr_list       ,
                     rohc_csc_udp_t     *p_csc              ,
                     rohc_U32bit_t      *p_length           ,
                     rohc_U8bit_t       *p_final_comp_list  ,
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] ,
                     rohc_U8bit_t        *p_item_list       ,
                     rohc_U8bit_t        *p_compressed_list  ,
                     rohc_bool_t        item_present_in_tt  ,
                     rohc_U32bit_t      curr_index          ,
                     rohc_error_code_t  *p_ecode));

static rohc_return_t         rohcRExtAckRecvdP2  ROHC_PROTO((
                     rohc_entity_t      *p_entity           ,
                     rohc_list_node_t   *ext_hdr_list       ,
                     rohc_csc_udp_t     *p_csc              ,
                     rohc_U32bit_t      *p_length           ,
                     rohc_U8bit_t       *p_final_comp_list  ,
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] ,
                     rohc_U8bit_t        *p_item_list       ,
                     rohc_U8bit_t        *p_compressed_list  ,
			         rohc_list_t	    p_list			    ,
                     rohc_bool_t        item_present_in_tt  ,
                     rohc_U32bit_t      curr_index          ,
                     rohc_error_code_t  *p_ecode));
/* - SPR 17439 */
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
**        	rohc_cled_p1_uo_inneripext_list_comp
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress INNER IP EXT HDR LIST in U/O Mode
**   	ARGUMENTS: 
**         	The compressor context, rohc entity context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
*****************************************************************************/
rohc_return_t	rohc_cled_p1_uo_inneripext_list_comp ROHC_PARAMS
	(
		(p_entity, p_csc, p_length, p_compressed_list, p_ecode),
			rohc_entity_t		*p_entity       	_AND_
			rohc_csc_rtp_t   	*p_csc 		        _AND_
			rohc_U32bit_t		*p_length 	        _AND_
			rohc_U8bit_t      	*p_compressed_list 	_AND_
			rohc_error_code_t	*p_ecode
	)
{
	rohc_cpib_t		*p_cpib = ROHC_CSC_GET_CPIB(p_csc);
	rohc_return_t	ret = ROHC_SUCCESS;
	ret = rohc_c_uo_ext_hdr_list_comp_p1(p_entity, p_csc, p_cpib->inneripextlist, 
                                      p_length, p_compressed_list, p_ecode);

	if(p_cpib->p_ah_data != ROHC_NULL)
	{
		p_csc->p_inner_ah_data = p_cpib->p_ah_data;
		p_csc->inner_ah_data_len = p_cpib->ah_data_len;
	}
	p_csc->inner_gre_chksum_present = p_cpib->gre_chksum_present;
	p_csc->inner_gre_chksum = p_cpib->gre_chksum;

	return ret;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_cled_p2_uo_inneripext_list_comp
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress INNER IP EXT HDR LIST in U/O Mode
**   	ARGUMENTS: 
**         	The compressor context, rohc entity context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
*****************************************************************************/
rohc_return_t	rohc_cled_p2_uo_inneripext_list_comp ROHC_PARAMS
	(
		(p_entity, p_csc, p_length, p_compressed_list, p_ecode),
			rohc_entity_t		  *p_entity 	        _AND_
			rohc_csc_udp_t	  *p_csc 		          _AND_
			rohc_U32bit_t		  *p_length 	        _AND_
			rohc_U8bit_t     	*p_compressed_list 	_AND_
			rohc_error_code_t	*p_ecode
	)
{
	rohc_cpib_t		*p_cpib = ROHC_CSC_GET_CPIB(p_csc);
	rohc_return_t	ret = ROHC_SUCCESS;
	ret = rohc_c_uo_ext_hdr_list_comp_p2(p_entity, p_csc, 
            p_cpib->inneripextlist, p_length, 
                    p_compressed_list, p_ecode);

	if(p_cpib->p_ah_data != ROHC_NULL)
	{
		p_csc->p_inner_ah_data = p_cpib->p_ah_data;
		p_csc->inner_ah_data_len = p_cpib->ah_data_len;
	}
	p_csc->inner_gre_chksum_present = p_cpib->gre_chksum_present;
	p_csc->inner_gre_chksum = p_cpib->gre_chksum;

	return ret;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_cled_p1_r_inneripext_list_comp
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to  compress INNER IP EXT HDR LIST in R Mode
**   	ARGUMENTS: 
**         	The compressor context, rohc entity context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
*****************************************************************************/
rohc_return_t   rohc_cled_p1_r_inneripext_list_comp ROHC_PARAMS
	(
		(p_entity, p_csc, p_length, p_compressed_list, p_ecode),
    		rohc_entity_t   	*p_entity 	_AND_
    		rohc_csc_rtp_t     	*p_csc 		_AND_
    		rohc_U32bit_t      	*p_length 	_AND_
    		rohc_U8bit_t      	*p_compressed_list 	_AND_
			rohc_error_code_t	*p_ecode
	)
{
	rohc_cpib_t		*p_cpib = ROHC_CSC_GET_CPIB(p_csc);
	rohc_return_t	ret = ROHC_SUCCESS;
	ret = rohc_c_r_ext_hdr_list_comp_p1(p_entity, p_csc, p_cpib->inneripextlist, 
                                     p_length, p_compressed_list, p_ecode);

	if(p_cpib->p_ah_data != ROHC_NULL)
	{
		p_csc->p_inner_ah_data = p_cpib->p_ah_data;
		p_csc->inner_ah_data_len = p_cpib->ah_data_len;
	}
	p_csc->inner_gre_chksum_present = p_cpib->gre_chksum_present;
	p_csc->inner_gre_chksum = p_cpib->gre_chksum;

	return ret;
}
/*****************************************************************************
**    	FUNCTION:
**        	rohc_cled_p2_r_inneripext_list_comp
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to  compress INNER IP EXT HDR LIST in R Mode
**   	ARGUMENTS: 
**         	The compressor context, rohc entity context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
*****************************************************************************/
rohc_return_t   rohc_cled_p2_r_inneripext_list_comp ROHC_PARAMS
	(
		(p_entity, p_csc, p_length, p_compressed_list, p_ecode),
    		rohc_entity_t   	*p_entity 	_AND_
    		rohc_csc_udp_t    	*p_csc 		_AND_
    		rohc_U32bit_t      	*p_length 	_AND_
    		rohc_U8bit_t      	*p_compressed_list 	_AND_
			rohc_error_code_t	*p_ecode
	)
{
	rohc_cpib_t		*p_cpib = ROHC_CSC_GET_CPIB(p_csc);
	rohc_return_t	ret = ROHC_SUCCESS;
	ret = rohc_c_r_ext_hdr_list_comp_p2(p_entity, p_csc, p_cpib->inneripextlist,
                                         p_length, p_compressed_list, p_ecode);

	if(p_cpib->p_ah_data != ROHC_NULL)
	{
		p_csc->p_inner_ah_data = p_cpib->p_ah_data;
		p_csc->inner_ah_data_len = p_cpib->ah_data_len;
	}
	p_csc->inner_gre_chksum_present = p_cpib->gre_chksum_present;
	p_csc->inner_gre_chksum = p_cpib->gre_chksum;

	return ret;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_cled_p1_uo_outeripext_list_comp
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress OUTER IP EXT HDR LIST in U/O Mode
**   	ARGUMENTS: 
**         	The compressor context, rohc entity context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
*****************************************************************************/

rohc_return_t   rohc_cled_p1_uo_outeripext_list_comp ROHC_PARAMS
	(
		(p_entity, p_csc, p_length, p_compressed_list, p_ecode),
    		rohc_entity_t   	*p_entity 	_AND_
    		rohc_csc_rtp_t     	*p_csc 		_AND_
    		rohc_U32bit_t      	*p_length 	_AND_
    		rohc_U8bit_t      	*p_compressed_list _AND_
			rohc_error_code_t	*p_ecode
	)
{
	rohc_cpib_t		*p_cpib = ROHC_CSC_GET_CPIB(p_csc);
	rohc_return_t	ret = ROHC_SUCCESS;
	ret = rohc_c_uo_ext_hdr_list_comp_p1(p_entity, p_csc, 
                p_cpib->outeripextlist, p_length, p_compressed_list, p_ecode);

	if(p_cpib->p_ah_data != ROHC_NULL)
	{
		p_csc->p_outer_ah_data = p_cpib->p_ah_data;
		p_csc->outer_ah_data_len = p_cpib->ah_data_len;
	}
	p_csc->outer_gre_chksum_present = p_cpib->gre_chksum_present;
	p_csc->outer_gre_chksum = p_cpib->gre_chksum;

	return ret;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_cled_p2_uo_outeripext_list_comp
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress OUTER IP EXT HDR LIST in U/O Mode
**   	ARGUMENTS: 
**         	The compressor context, rohc entity context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
*****************************************************************************/

rohc_return_t   rohc_cled_p2_uo_outeripext_list_comp ROHC_PARAMS
	(
		(p_entity, p_csc, p_length, p_compressed_list, p_ecode),
    		rohc_entity_t   	*p_entity 	_AND_
    		rohc_csc_udp_t     	*p_csc 		_AND_
    		rohc_U32bit_t      	*p_length 	_AND_
    		rohc_U8bit_t      	*p_compressed_list _AND_
			rohc_error_code_t	*p_ecode
	)
{
	rohc_cpib_t		*p_cpib = ROHC_CSC_GET_CPIB(p_csc);
	rohc_return_t	ret = ROHC_SUCCESS;
	ret = rohc_c_uo_ext_hdr_list_comp_p2(p_entity, p_csc, p_cpib->outeripextlist,
                                         p_length, p_compressed_list, p_ecode);

	if(p_cpib->p_ah_data != ROHC_NULL)
	{
		p_csc->p_outer_ah_data = p_cpib->p_ah_data;
		p_csc->outer_ah_data_len = p_cpib->ah_data_len;
	}
	p_csc->outer_gre_chksum_present = p_cpib->gre_chksum_present;
	p_csc->outer_gre_chksum = p_cpib->gre_chksum;

	return ret;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_cled_p1_r_outeripext_list_comp
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to  compress OUTER IP EXT HDR LIST in R Mode
**   	ARGUMENTS: 
**         	The compressor context, rohc entity context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
*****************************************************************************/

rohc_return_t   rohc_cled_p1_r_outeripext_list_comp ROHC_PARAMS
	(
		(p_entity, p_csc, p_length, p_compressed_list, p_ecode),
    		rohc_entity_t   	*p_entity 	_AND_
    		rohc_csc_rtp_t     	*p_csc 		_AND_
    		rohc_U32bit_t      	*p_length 	_AND_
    		rohc_U8bit_t      	*p_compressed_list 	_AND_
			rohc_error_code_t	*p_ecode
	)
{
	rohc_cpib_t		*p_cpib = ROHC_CSC_GET_CPIB(p_csc);
	rohc_return_t	ret = ROHC_SUCCESS;
	ret = rohc_c_r_ext_hdr_list_comp_p1(p_entity, p_csc, p_cpib->outeripextlist, 
                                        p_length, p_compressed_list, p_ecode);

	if(p_cpib->p_ah_data != ROHC_NULL)
	{
		p_csc->p_outer_ah_data = p_cpib->p_ah_data;
		p_csc->outer_ah_data_len = p_cpib->ah_data_len;
	}
	p_csc->outer_gre_chksum_present = p_cpib->gre_chksum_present;
	p_csc->outer_gre_chksum = p_cpib->gre_chksum;

	return ret;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_cled_p2_r_outeripext_list_comp
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to  compress OUTER IP EXT HDR LIST in R Mode
**   	ARGUMENTS: 
**         	The compressor context, rohc entity context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
*****************************************************************************/

rohc_return_t   rohc_cled_p2_r_outeripext_list_comp ROHC_PARAMS
	(
		(p_entity, p_csc, p_length, p_compressed_list, p_ecode),
    		rohc_entity_t   	*p_entity 	_AND_
    		rohc_csc_udp_t     	*p_csc 		_AND_
    		rohc_U32bit_t      	*p_length 	_AND_
    		rohc_U8bit_t      	*p_compressed_list 	_AND_
			rohc_error_code_t	*p_ecode
	)
{
	rohc_cpib_t		*p_cpib = ROHC_CSC_GET_CPIB(p_csc);
	rohc_return_t	ret = ROHC_SUCCESS;
	ret = rohc_c_r_ext_hdr_list_comp_p2(p_entity, p_csc, p_cpib->outeripextlist, 
                                        p_length, p_compressed_list, p_ecode);

	if(p_cpib->p_ah_data != ROHC_NULL)
	{
		p_csc->p_outer_ah_data = p_cpib->p_ah_data;
		p_csc->outer_ah_data_len = p_cpib->ah_data_len;
	}
	p_csc->outer_gre_chksum_present = p_cpib->gre_chksum_present;
	p_csc->outer_gre_chksum = p_cpib->gre_chksum;

	return ret;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_r_ext_hdr_list_comp_p1
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to  compress EXT HDR LIST in R Mode
**   	ARGUMENTS: 
**         	p_csc - The compressor context, 
**			p_entity - rohc entity context,
**			p_list - Pointer to List,
**			p_length - Length of list,
**			p_final_comp_list - Final Compressed List,
**			p_ecode - Pointer to Error Code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of RTP SN No is used as ref_id
*****************************************************************************/
rohc_return_t	rohc_c_r_ext_hdr_list_comp_p1 ROHC_PARAMS
	(
		(p_entity, p_csc, p_list, p_length, p_final_comp_list, p_ecode),
			rohc_entity_t		*p_entity 		_AND_
			rohc_csc_rtp_t		*p_csc 			_AND_
			rohc_list_t			p_list			_AND_
			rohc_U32bit_t		*p_length 		_AND_
			rohc_U8bit_t      	*p_final_comp_list 	_AND_
			rohc_error_code_t	*p_ecode
	)
{
	rohc_return_t		ret = ROHC_SUCCESS;
	rohc_U32bit_t		count = 0;
	rohc_U8bit_t		*p_item_list;
	rohc_U8bit_t		curr_index = 0;
    /* Coverity Fix 30667 Start */ 
	rohc_U8bit_t		XI_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
    /* Coverity Fix 30667 End */ 
	rohc_list_node_t	*ext_hdr_list = rohc_db_get_first_node(&p_list);
	rohc_bool_t			PS_flag = ROHC_FALSE;
	rohc_U8bit_t		*p_compressed_list = 
							p_final_comp_list + ROHC_MAX_EXT_HDR_SIZE;
	rohc_U32bit_t		size_item_list = 0;
	rohc_bool_t			item_present_in_tt = ROHC_FALSE; 
    rohc_return_t       returnVal = ROHC_FAILURE;

	ROHC_ENTER_FN("rohc_c_r_ext_hdr_list_comp_p1");

	if(ext_hdr_list == ROHC_NULL)
	{
		/* This implies that its an empty list */
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
		" No item in the list hence constructing an empty list\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));
        /*Start SPR 5869 Fix */
        *p_length = rohc_c_enc_type_0_pkt(0, ROHC_FALSE, PS_flag, 
                (rohc_U8bit_t)count, XI_list, ROHC_NULL, 
                (rohc_U8bit_t)size_item_list, p_final_comp_list);
        /*End SPR 5869 Fix */
		return ROHC_SUCCESS;
	}

	/* ROHC_MAX_EXT_PKT_HDR_SIZE = 25 is the packet header above item list */
	p_item_list = p_compressed_list + ROHC_MAX_EXT_PKT_HDR_SIZE; 

	if(ROHC_CSC_GET_R_EXT_TT_NIDX(p_csc) > 8)
	{
		/* This implies that the index list will be stored in 1 byte field */
		ROHC_LOG(LOG_INFO, 
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
		" Setting PS_flag as TRUE\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));
	}
	
	/* Checking the case when no ACK is received for the list till now */
	if (ROHC_FALSE == ROHC_CSC_GET_R_EXT_SW_ACKED(p_csc))
	{
		/* This implies that Generic Encoding scheme needs to be applied */
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
		" Handling the case when no ACK is recieved till now\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));


        returnVal = rohcRExtAckNotRecvdP1(p_entity, ext_hdr_list, p_csc, p_length,
                p_final_comp_list, XI_list, p_item_list, p_compressed_list, 
                item_present_in_tt, curr_index,p_ecode );
        
        if (ROHC_FAILURE == returnVal)
        {
            ret = ROHC_FAILURE;
        }
	}
	else
	{
        returnVal = rohcRExtAckRecvdP1(p_entity, ext_hdr_list, p_csc, p_length,
                                 p_final_comp_list, XI_list, p_item_list, p_compressed_list,
                                 p_list, item_present_in_tt, curr_index,p_ecode );

        if (ROHC_FAILURE == returnVal)
        {
            ret = ROHC_FAILURE;
        }
    }

	ROHC_EXIT_FN("rohc_c_r_ext_hdr_list_comp_p1");
	return ret;
}


/*****************************************************************************
**    	FUNCTION:
**        	rohcUOExtAckRecvdP1
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress EXT HDR LIST in U/O Mode when 
**          ACK is received
**   	ARGUMENTS: 
**         	p_csc - The compressor context, 
**			p_entity - rohc entity context,
**			ext_hdr_list - Pointer to EXT Header List,
**			p_length - Pointer to Length,
**			p_final_comp_list - pointer to Final Compressed List,
**			p_list - Pointer to List,
**			item_present_in_tt - Item Present in Transition Table,
**			curr_index - Current index,
**			p_ecode - Pointer to Error Code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of gen_id is used as ref_id in U/O Mode
*****************************************************************************/
static rohc_return_t         rohcRExtAckRecvdP1 ROHC_PARAMS(
     (p_entity, ext_hdr_list, p_csc, p_length, p_final_comp_list, XI_list,
      p_item_list, p_compressed_list, p_list, item_present_in_tt, curr_index,
      p_ecode),
                     rohc_entity_t      *p_entity           _AND_
                     rohc_list_node_t   *ext_hdr_list       _AND_
                     rohc_csc_rtp_t     *p_csc              _AND_
                     rohc_U32bit_t      *p_length           _AND_
                     rohc_U8bit_t       *p_final_comp_list  _AND_
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
                     rohc_U8bit_t        *p_item_list       _AND_
                     rohc_U8bit_t        *p_compressed_list  _AND_
			         rohc_list_t		p_list			    _AND_
                     rohc_bool_t        item_present_in_tt  _AND_
                     rohc_U32bit_t      curr_index          _AND_
                     rohc_error_code_t  *p_ecode)

{    
    rohc_U32bit_t       ref_id = 0,
                        aseq_no = 0, eseq_no = 0, gseq_no = 0;
	rohc_U32bit_t		size_item_list = 0, tot_size = 0,
						index_found = 0xFF; 
	rohc_bool_t			PS_flag = ROHC_FALSE;
    		    /* Coverity 11395 ends*/
    rohc_U8bit_t        index_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
    		    /* Coverity 11395 ends*/
	rohc_r_sw_t			sw_ref_entry;
	r_ext_hdr_tt_t		tt_entry;
    rohc_bool_t         item_remove_flag = ROHC_FALSE;
    rohc_U32bit_t       count = 0, i = 0,
						item_count = 0, 
						final_count = 0,
                        index_count = 0; 
    rohc_U16bit_t       removal_bit_mask = 0xFFFF, insertion_bit_mask = 0;
    rohc_U32bit_t       tot_count = rohc_db_count_of_list(&p_list);
    /* Coverity 11396 ends*/
    rohc_U8bit_t        final_idx_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
    /* Coverity 11396 ends*/

    /* 
     * Reference list exists . Hence we need to compare the current
     * Extension header list with the reference list and construct the
     * bit mask accordingly 
     */
    sw_ref_entry = ROHC_CSC_GET_R_EXT_SW_REF(p_csc);

    ref_id = ROHC_GET_LOWER_NIBBLE(sw_ref_entry.rtp_sn_no);

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),curr_index,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, 
            "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
            " RTP Sequence number of the ref list [%d]\n",  \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), 
            sw_ref_entry.rtp_sn_no);

    ROHC_LOG(LOG_INFO,
            "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
            " Number of items in ref list [%d]\n",  \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), sw_ref_entry.icount);

    /* First we will construct removal bit mask */

    ext_hdr_list = rohc_db_get_first_node(&p_list);
    for(count = 0; count < sw_ref_entry.icount; count++) /*ref_list*/
    {
        while(ext_hdr_list) /*curr_list*/
        {
            tt_entry = ROHC_CSC_GET_R_EXT_TT(p_csc,
                    sw_ref_entry.index_list[count]);
            /* +- SPR 17777 */
            if(ROHC_SUCCESS == 
                    rohc_c_compare_n_encode_p1_ext_hdr_item(
                        tt_entry.p_item, ext_hdr_list, 
                        p_csc, &aseq_no, &gseq_no))
                /* +- SPR 17777 */
            {
                /* Item found in ref_list */
                index_found = sw_ref_entry.index_list[count];
                ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);

                ROHC_LOG(LOG_INFO,
                        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                        " Item[%d] found in ref list at index [%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
                        ROHC_CSC_GET_MODE(p_csc), 
                        ((rohc_protocol_node_t *)(tt_entry.p_item))->protocol, 
                        index_found);

                break;
            }
            if(*p_ecode != ROHC_NO_ERR)
            {
                return ROHC_FAILURE;
            }
            ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);
        } /* end of while */
        if(index_found == 0xFF)
        {
            /* This implies that the reference item is not 
             * there in the current list. Set the remove bit as 1 */
            ext_hdr_list = rohc_db_get_first_node(&p_list);
            item_remove_flag = ROHC_TRUE;
        }
        else
        {
            /* Set the remove bit as 0 */
            if(count < 8)
                removal_bit_mask = ROHC_UNSET_BIT(removal_bit_mask, 
                        (7 -count));
            else
                removal_bit_mask = ROHC_UNSET_BIT(removal_bit_mask, 
                        (24 -count));
            /* SPR 3444 changes starts */
            if(item_count < ROHC_MAX_ENTRIES_IN_TT)
            {
                index_list[item_count++] = index_found;
            }
            /* SPR 3444 changes ends */
            index_found = 0xff;
        }

    } /* end of for */

    ROHC_LOG(LOG_INFO,
            "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
            "The constructed removal bit mask [%d]\n",\
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), removal_bit_mask);

    if(item_count < tot_count)
    {
        /* Construct insertion bit mask using the index list
         * constructed above as reference list */
        ROHC_LOG(LOG_INFO,
                "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: "\
                " Constructing Insertion bit mask\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc)); 

        ext_hdr_list = rohc_db_get_first_node(&p_list);
        count = 0;
        while(ext_hdr_list) /*curr_list*/
        {
            while(count < item_count)  /*ref_list*/
            {
                tt_entry = ROHC_CSC_GET_R_EXT_TT(p_csc,
                        index_list[count]);
                /* +- SPR 17777 */
                if(ROHC_SUCCESS == 
                        rohc_c_compare_n_encode_p1_ext_hdr_item(
                            tt_entry.p_item, ext_hdr_list, p_csc, 
                            &aseq_no, &gseq_no))
                /* +- SPR 17777 */
                {
                    /* Item found in ref_list */
                    index_found = index_list[count];

                    ROHC_LOG(LOG_INFO,
                            "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                            "Item[%d] found in ref list at index [%d]\n", \
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),\
                            ROHC_CSC_GET_MODE(p_csc), \
                            ((rohc_protocol_node_t *)(tt_entry.p_item))->protocol, \
                            index_found);

                    /* SPR 3444 changes starts */
                    if(final_count < ROHC_MAX_ENTRIES_IN_TT)
                    {
                        final_idx_list[final_count++] = index_found;
                    } 
                    /* SPR 3444 changes ends */

                    rohc_c_add_sn_p1_r_mode_ext_hdr_tt(p_csc, 
                            (rohc_U8bit_t)index_found,
                            &tt_entry); 
                    count++;
                    break;
                }
                else
                {
                    if(*p_ecode != ROHC_NO_ERR)
                    {
                        return ROHC_FAILURE;
                    }
                    index_found = 0xFF;
                }

                count++;
            } /* end of while */
            if(index_found == 0xFF)
            {
                /* This implies it is a new item */
                index_found = rohc_c_add_n_chk_p1_r_ext_hdr_tt(
                        ext_hdr_list, p_csc, &item_present_in_tt, p_ecode);
                if(*p_ecode != ROHC_NO_ERR)
                {
                    return ROHC_FAILURE;
                }
                ROHC_LOG(LOG_INFO,
                        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                        " Item[%d] not found in ref list, hence inserted at[%d]\n",\
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
                        ROHC_CSC_GET_MODE(p_csc), \
                        ((rohc_protocol_node_t *)(tt_entry.p_item))->protocol, \
                        index_found);
                /* SPR 3444 changes starts */
                if(final_count < ROHC_MAX_ENTRIES_IN_TT)
                {
                    final_idx_list[final_count++] = index_found;
                }

                if(item_present_in_tt && (index_found < ROHC_MAX_ENTRIES_IN_TT))
                    /* SPR 3444 changes ends */

                {
                    tt_entry = ROHC_CSC_GET_R_EXT_TT(p_csc, index_found);
                    if(tt_entry.known == ROHC_FALSE)
                    {
                        /* Item not yet acked hence needs to be resend */
                        rohc_update_XI_list(&XI_list[index_count], 
                                (rohc_U8bit_t)index_found, 1);
                        if(ROHC_FAILURE == rohc_c_convert_p1_in_buffer(
                                    p_entity, p_csc, 
                                    (rohc_U8bit_t *)ext_hdr_list, 
                                    p_item_list, &size_item_list,
                                    p_ecode))
                        {
                            ROHC_EXIT_FN("rohc_c_r_ext_hdr_list_comp_p1");
                            return ROHC_FAILURE;
                        }
                    }
                    else
                    {
                        /* Item Acked hence no need to send it again */
                        rohc_update_XI_list(&XI_list[index_count], 
                                (rohc_U8bit_t)index_found, 0); 
                    }
                }
                else
                {
                    rohc_update_XI_list(&XI_list[index_count], 
                            (rohc_U8bit_t)index_found, 1);
                    if(ROHC_FAILURE == rohc_c_convert_p1_in_buffer(p_entity, 
                                p_csc, (rohc_U8bit_t *)ext_hdr_list,
                                p_item_list,&size_item_list,p_ecode))
                    {
                        ROHC_EXIT_FN("rohc_c_r_ext_hdr_list_comp_p1");
                        return ROHC_FAILURE;
                    }
                }
                index_count++;
                if(i<=7)
                    insertion_bit_mask = ROHC_SET_BIT(insertion_bit_mask, 
                            (7 - i));
                else
                    insertion_bit_mask = ROHC_SET_BIT(insertion_bit_mask, 
                            (24 - i));
                ROHC_LOG(LOG_INFO,
                        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                        "Insertion bit mask constructed till now [%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
                        ROHC_CSC_GET_MODE(p_csc), insertion_bit_mask);
                index_found = 0xFF;
                count = 0;
            }
            else
            {
                index_found = 0xFF;
            }
            ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);
            i++;
        } /* end of while */


        if((insertion_bit_mask != 0) & (item_remove_flag == ROHC_TRUE))
        {
            ROHC_LOG(LOG_INFO,
                    "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                    " Constructing Encoding Type 3 packet\n", \
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                    ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

            /*
             ** Incrementing Stats - ROHC_FUTURE_STATS
             ** pdcpStats.statsRohc.numberOfListEncNoRemThenInsSchmUsedCreated++;
             */

            tot_size = rohc_c_enc_type_3_pkt(0, ref_id, 0, PS_flag, 
                    (rohc_U8bit_t)index_count, removal_bit_mask, 
                    insertion_bit_mask, XI_list, 
                    p_item_list, size_item_list, p_compressed_list);

            /* Add the entry in the sliding window */
            rohc_c_add_r_mode_ext_hdr_p1_sw (p_csc,
                    (rohc_U8bit_t)final_count,
                    final_idx_list);
        }
        else if(insertion_bit_mask != 0)
        {
            ROHC_LOG(LOG_INFO,
                    "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                    " Constructing Encoding Type 1 packet\n", \
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                    ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

            /*
             ** Incrementing Stats - ROHC_FUTURE_STATS
             ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedCreated++;
             */

            tot_size = rohc_c_enc_type_1_pkt(0, ref_id, 0, PS_flag, 
                    (rohc_U8bit_t)index_count, insertion_bit_mask, XI_list, 
                    p_item_list, size_item_list, p_compressed_list);

            /* Add the entry in the sliding window */
            rohc_c_add_r_mode_ext_hdr_p1_sw (p_csc,
                    (rohc_U8bit_t)final_count,final_idx_list);
        }

    }
    else if ((item_count != tot_count) || (item_remove_flag == ROHC_TRUE))
    {
        /* Construct the Encoding Type 2 packet */
        ROHC_LOG(LOG_INFO,
                "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                " Constructing Encoding Type 2 packet\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

        /*
         ** Incrementing Stats - ROHC_FUTURE_STATS
         ** pdcpStats.statsRohc.numberOfListEncNoRemOnlySchmUsedCreated++;
         */

        tot_size = rohc_c_enc_type_2_pkt (0, 0,  0, 
                (rohc_U8bit_t)sw_ref_entry.icount, 
                removal_bit_mask, p_compressed_list);

        /* Add the entry in the sliding window */
        /* Klocwork Fix */
        if((item_count > 0)&&(item_count < ROHC_MAX_ENTRIES_IN_TT))
        {
            rohc_c_add_r_mode_ext_hdr_p1_sw (p_csc, (rohc_U8bit_t)item_count, 
                    index_list);
        }

    }
    else
    {
        /* Implies List is unchanged */
        ROHC_LOG(LOG_INFO,
                "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                "Constructing Encoding Type 1 packet, as list is unchanged\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

        /*
         ** Incrementing Stats - ROHC_FUTURE_STATS
         ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedCreated++;
         */

        tot_size = rohc_c_enc_type_1_pkt (0, ref_id, ROHC_FALSE,PS_flag, 
                (rohc_U8bit_t)index_count, 0, XI_list, ROHC_NULL, 0, 
                p_compressed_list);
    } /* endif */

    /* Construct the final  packet */
    *p_length = rohc_c_cnst_pkt(aseq_no, eseq_no, gseq_no, 
            p_compressed_list, tot_size, p_final_comp_list);
    return ROHC_SUCCESS;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohcRExtAckNotRecvdP1
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress EXT HDR LIST in U/O Mode when 
**          ACK is not received
**   	ARGUMENTS: 
**			p_entity - rohc entity context,
**			ext_hdr_list - Pointer to EXT Header List,
**         	p_csc - The compressor context, 
**			p_length - Pointer to Length,
**			p_final_comp_list - pointer to Final Compressed List,
**			XI_list - 
**			p_item_list - Pointer to List of Items,
**			item_present_in_tt - Item Present in Transition Table,
**			curr_index - Current index,
**			p_ecode - Pointer to Error Code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of gen_id is used as ref_id in U/O Mode
*****************************************************************************/
static rohc_return_t         rohcRExtAckNotRecvdP1 ROHC_PARAMS(
     (p_entity, ext_hdr_list, p_csc, p_length, p_final_comp_list, XI_list,
      p_item_list, p_compressed_list, item_present_in_tt, curr_index, p_ecode),
                     rohc_entity_t      *p_entity           _AND_
                     rohc_list_node_t   *ext_hdr_list       _AND_
                     rohc_csc_rtp_t     *p_csc              _AND_
                     rohc_U32bit_t      *p_length           _AND_
                     rohc_U8bit_t       *p_final_comp_list  _AND_
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
                     rohc_U8bit_t        *p_item_list       _AND_
                     rohc_U8bit_t        *p_compressed_list  _AND_
                     rohc_bool_t        item_present_in_tt  _AND_
                     rohc_U32bit_t      curr_index          _AND_
                     rohc_error_code_t  *p_ecode)

{    
    rohc_U32bit_t       count = 0, aseq_no = 0,
    eseq_no = 0, gseq_no = 0;
    rohc_U32bit_t		size_item_list = 0, tot_size = 0;
    rohc_bool_t			PS_flag = ROHC_FALSE;
	rohc_U8bit_t		index_list[ROHC_MAX_ENTRIES_IN_TT]; 

    while(ext_hdr_list) /* curr_list */
    {
        curr_index = 
            rohc_c_add_n_chk_p1_r_ext_hdr_tt(ext_hdr_list, p_csc, 
                    &item_present_in_tt, p_ecode);
        if(*p_ecode != ROHC_NO_ERR)
        {
            return ROHC_FAILURE;
        }
        /* SPR 3444 changes starts */
        if(count < ROHC_MAX_ENTRIES_IN_TT)
        {
            index_list[count] = curr_index;
        }
        /* SPR 3444 changes ends */

        /* No need to check item_present_in_tt flag as no ACK is received
         * till now so item needs to be resend */

        rohc_update_XI_list(&XI_list[count], curr_index, 1);
        if(ROHC_FAILURE == rohc_c_convert_p1_in_buffer(p_entity, p_csc, 
                    (rohc_U8bit_t *)ext_hdr_list,p_item_list,&size_item_list,p_ecode))
        {
            ROHC_EXIT_FN("rohc_c_r_ext_hdr_list_comp_p1");
            return ROHC_FAILURE;
        }
        count++;
        /* SPR 3532 - Klocwork Fix*/
        if(count >= ROHC_MAX_ENTRIES_IN_TT)
        {
            ROHC_LOG(LOG_INFO,"Invalid value of count");
            ROHC_EXIT_FN("rohc_c_r_ext_hdr_list_comp_p1");
            return ROHC_FAILURE;
        }
        ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);
    } /* end of while */

    /* Construct an Encoding Type 0 packet*/
    ROHC_LOG(LOG_INFO, 
            "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
            "Constructing Encoding Type 0 packet\n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

    /*
     ** Incrementing Stats - ROHC_FUTURE_STATS
     ** pdcpStats.statsRohc.numberOfListEncNoGenSchmUsedCreated++;
     */

    tot_size = rohc_c_enc_type_0_pkt(0, ROHC_FALSE, PS_flag, 
            (rohc_U8bit_t)count, XI_list, p_item_list, 
            (rohc_U8bit_t)size_item_list, p_compressed_list);

    /* Add entry in Sliding Window */		
    rohc_c_add_r_mode_ext_hdr_p1_sw (p_csc, (rohc_U8bit_t)count, 
            index_list);

    /* Construct the final packet */
    /* Coverity 54675*/
    *p_length = rohc_c_cnst_pkt(aseq_no,eseq_no , gseq_no,
            p_compressed_list, tot_size, p_final_comp_list);
    /* Coverity 54675*/
    return ROHC_SUCCESS;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_r_ext_hdr_list_comp_p2
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to  compress EXT HDR LIST in R Mode
**   	ARGUMENTS: 
**         	p_csc - The compressor context, 
**			p_entity - rohc entity context,
**			p_list - Pointer to List,
**			p_length - Pointer to Length,
**			p_final_comp_list - Final compressed list,
**			p_ecode - Pointer to Erro Code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of RTP SN No is used as ref_id
*****************************************************************************/
rohc_return_t	rohc_c_r_ext_hdr_list_comp_p2 ROHC_PARAMS
	(
		(p_entity, p_csc, p_list, p_length, p_final_comp_list, p_ecode),
			rohc_entity_t		*p_entity 		_AND_
			rohc_csc_udp_t		*p_csc 			_AND_
			rohc_list_t			p_list			_AND_
			rohc_U32bit_t		*p_length 		_AND_
			rohc_U8bit_t      	*p_final_comp_list 	_AND_
			rohc_error_code_t	*p_ecode
	)
{
	rohc_return_t		ret = ROHC_SUCCESS;
	rohc_U32bit_t		count = 0;

	rohc_U8bit_t		*p_item_list;
	rohc_U8bit_t		curr_index = 0;
    /* Coverity Fix 30666 Start */ 
	rohc_U8bit_t		XI_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
    /* Coverity Fix 30666 End */ 
	rohc_list_node_t	*ext_hdr_list = rohc_db_get_first_node(&p_list);
	rohc_bool_t			PS_flag = ROHC_FALSE;
	rohc_U8bit_t		*p_compressed_list = 
							p_final_comp_list + ROHC_MAX_EXT_HDR_SIZE;
	rohc_U32bit_t		size_item_list = 0;
	rohc_bool_t			item_present_in_tt = ROHC_FALSE; 
    rohc_return_t       returnVal = ROHC_FAILURE;

	ROHC_ENTER_FN("rohc_c_r_ext_hdr_list_comp_p2");

	if(ext_hdr_list == ROHC_NULL)
	{
		/* This implies that its an empty list */
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
		" No item in the list hence constructing an empty list\n", \
		p_entity->rohc_entity_id.ueIndex,
        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
		ROHC_CSC_GET_MODE(p_csc));
        /*Start SPR 5869 Fix */
        *p_length = rohc_c_enc_type_0_pkt(0, ROHC_FALSE, PS_flag, 
                (rohc_U8bit_t)count, XI_list, ROHC_NULL, 
                (rohc_U8bit_t)size_item_list, p_final_comp_list);
        /*End SPR 5869 Fix */
		return ROHC_SUCCESS;
	}

	/* ROHC_MAX_EXT_PKT_HDR_SIZE = 25 is the packet header above item list */
	p_item_list = p_compressed_list + ROHC_MAX_EXT_PKT_HDR_SIZE; 

	if(ROHC_CSC_GET_R_EXT_TT_NIDX(p_csc) > 8)
	{
		/* This implies that the index list will be stored in 1 byte field */
		ROHC_LOG(LOG_INFO, 
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
		" Setting PS_flag as TRUE\n", \
		p_entity->rohc_entity_id.ueIndex,
        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
		ROHC_CSC_GET_MODE(p_csc));
	}
	
	/* Checking the case when no ACK is received for the list till now */
	if (ROHC_FALSE == ROHC_CSC_GET_R_EXT_SW_ACKED(p_csc))
	{
		/* This implies that Generic Encoding scheme needs to be applied */
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
		" Handling the case when no ACK is recieved till now\n", \
		p_entity->rohc_entity_id.ueIndex,
        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
		ROHC_CSC_GET_MODE(p_csc));


        returnVal = rohcRExtAckNotRecvdP2(p_entity, ext_hdr_list, p_csc, p_length,
                p_final_comp_list, XI_list, p_item_list, p_compressed_list, 
                item_present_in_tt, curr_index,p_ecode );
        
        if (ROHC_FAILURE == returnVal)
        {
            ret = ROHC_FAILURE;
        }
	}
	else
	{
        returnVal = rohcRExtAckRecvdP2(p_entity, ext_hdr_list, p_csc, p_length,
                                 p_final_comp_list, XI_list, p_item_list, p_compressed_list,
                                 p_list, item_present_in_tt, curr_index,p_ecode );

        if (ROHC_FAILURE == returnVal)
        {
            ret = ROHC_FAILURE;
        }
    }

	ROHC_EXIT_FN("rohc_c_r_ext_hdr_list_comp_p2");
	return ret;
}


/*****************************************************************************
**    	FUNCTION:
**        	rohcUOExtAckRecvdP2
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress EXT HDR LIST in U/O Mode when 
**          ACK is received
**   	ARGUMENTS: 
**			p_entity - rohc entity context,
**			ext_hdr_list - Pointer to EXT Header List,
**         	p_csc - The compressor context, 
**			p_length - Pointer to Length,
**			p_final_comp_list - pointer to Final Compressed List,
**			XI_list - 
**			p_item_list - Pointer to List of Items,
**			item_present_in_tt - Item Present in Transition Table,
**			curr_index - Current index,
**			p_ecode - Pointer to Error Code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of gen_id is used as ref_id in U/O Mode
*****************************************************************************/
static rohc_return_t         rohcRExtAckRecvdP2 ROHC_PARAMS(
     (p_entity, ext_hdr_list, p_csc, p_length, p_final_comp_list, XI_list,
      p_item_list, p_compressed_list, p_list, item_present_in_tt, curr_index,
      p_ecode),
                     rohc_entity_t      *p_entity           _AND_
                     rohc_list_node_t   *ext_hdr_list       _AND_
                     /* + SPR 17439 */
                     rohc_csc_udp_t     *p_csc              _AND_
                     /* - SPR 17439 */
                     rohc_U32bit_t      *p_length           _AND_
                     rohc_U8bit_t       *p_final_comp_list  _AND_
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
                     rohc_U8bit_t        *p_item_list       _AND_
                     rohc_U8bit_t        *p_compressed_list  _AND_
			         rohc_list_t		p_list			    _AND_
                     rohc_bool_t        item_present_in_tt  _AND_
                     rohc_U32bit_t      curr_index          _AND_
                     rohc_error_code_t  *p_ecode)

{    
    rohc_U32bit_t       ref_id = 0,
                        aseq_no = 0, eseq_no = 0, gseq_no = 0;
	rohc_U32bit_t		size_item_list = 0, tot_size = 0,
						index_found = 0xFF; 
	rohc_bool_t			PS_flag = ROHC_FALSE;
    		    /* Coverity 11395 ends*/
    rohc_U8bit_t        index_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
    		    /* Coverity 11395 ends*/
	rohc_r_sw_t			sw_ref_entry;
	r_ext_hdr_tt_t		tt_entry;
    rohc_bool_t         item_remove_flag = ROHC_FALSE;
    rohc_U32bit_t       count = 0, i = 0,
						item_count = 0, 
						final_count = 0,
                        index_count = 0; 
    rohc_U16bit_t       removal_bit_mask = 0xFFFF, insertion_bit_mask = 0;
    rohc_U32bit_t       tot_count = rohc_db_count_of_list(&p_list);
    /* Coverity 11396 ends*/
    rohc_U8bit_t        final_idx_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
    /* Coverity 11396 ends*/


    /* 
     * Reference list exists . Hence we need to compare the current
     * Extension header list with the reference list and construct the
     * bit mask accordingly 
     */
    sw_ref_entry = ROHC_CSC_GET_R_EXT_SW_REF(p_csc);

    ref_id = ROHC_GET_LOWER_NIBBLE(sw_ref_entry.rtp_sn_no);

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),curr_index,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, 
            "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
            " RTP Sequence number of the ref list [%d]\n",  \
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
            ROHC_CSC_GET_MODE(p_csc), sw_ref_entry.rtp_sn_no);

    ROHC_LOG(LOG_INFO,
            "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
            " Number of items in ref list [%d]\n",  \
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
            ROHC_CSC_GET_MODE(p_csc), sw_ref_entry.icount);

    /* First we will construct removal bit mask */

    ext_hdr_list = rohc_db_get_first_node(&p_list);
    for(count = 0; count < sw_ref_entry.icount; count++) /*ref_list*/
    {
        while(ext_hdr_list) /*curr_list*/
        {
            tt_entry = ROHC_CSC_GET_R_EXT_TT(p_csc,
                    sw_ref_entry.index_list[count]);
                /* +- SPR 17777 */
            if(ROHC_SUCCESS == 
                    rohc_c_compare_n_encode_p2_ext_hdr_item(
                        tt_entry.p_item, ext_hdr_list, 
                        p_csc, &aseq_no, &gseq_no))
                /* +- SPR 17777 */
            {
                /* Item found in ref_list */
                index_found = sw_ref_entry.index_list[count];
                ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);

                ROHC_LOG(LOG_INFO,
                        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                        " Item[%d] found in ref list at index [%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
                        ROHC_CSC_GET_MODE(p_csc), 
                        ((rohc_protocol_node_t *)(tt_entry.p_item))->protocol, 
                        index_found);

                break;
            }
            if(*p_ecode != ROHC_NO_ERR)
            {
                return ROHC_FAILURE;
            }
            ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);
        } /* end of while */
        if(index_found == 0xFF)
        {
            /* This implies that the reference item is not 
             * there in the current list. Set the remove bit as 1 */
            ext_hdr_list = rohc_db_get_first_node(&p_list);
            item_remove_flag = ROHC_TRUE;
        }
        else
        {
            /* Set the remove bit as 0 */
            if(count < 8)
                removal_bit_mask = ROHC_UNSET_BIT(removal_bit_mask, (7 -count));
            else
                removal_bit_mask = ROHC_UNSET_BIT(removal_bit_mask, (24 -count));
            /* SPR 3444 changes starts */
            if(item_count < ROHC_MAX_ENTRIES_IN_TT)
            {
                index_list[item_count++] = index_found;
            }
            /* SPR 3444 changes ends */
            index_found = 0xff;
        }

    } /* end of for */

    ROHC_LOG(LOG_INFO,
            "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
            "The constructed removal bit mask [%d]\n",\
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
            ROHC_CSC_GET_MODE(p_csc), removal_bit_mask);

    if(item_count < tot_count)
    {
        /* Construct insertion bit mask using the index list
         * constructed above as reference list */
        ROHC_LOG(LOG_INFO,
                "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: "\
                " Constructing Insertion bit mask\n", \
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
                ROHC_CSC_GET_MODE(p_csc)); 

        ext_hdr_list = rohc_db_get_first_node(&p_list);
        count = 0;
        while(ext_hdr_list) /*curr_list*/
        {
            while(count < item_count)  /*ref_list*/
            {
                tt_entry = ROHC_CSC_GET_R_EXT_TT(p_csc,
                        index_list[count]);
                /* +- SPR 17777 */
                if(ROHC_SUCCESS == 
                        rohc_c_compare_n_encode_p2_ext_hdr_item(
                            tt_entry.p_item, ext_hdr_list, p_csc, 
                            &aseq_no, &gseq_no))
                /* +- SPR 17777 */
                {
                    /* Item found in ref_list */
                    index_found = index_list[count];

                    ROHC_LOG(LOG_INFO,
                            "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                            "Item[%d] found in ref list at index [%d]\n", \
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
                            ROHC_CSC_GET_MODE(p_csc), \
                            ((rohc_protocol_node_t *)(tt_entry.p_item))->protocol, \
                            index_found);

                    final_idx_list[final_count++] = index_found;

                    rohc_c_add_sn_p2_r_mode_ext_hdr_tt(p_csc, 
                            (rohc_U8bit_t)index_found,&tt_entry); 
                    count++;
                    break;
                }
                else
                {
                    if(*p_ecode != ROHC_NO_ERR)
                    {
                        return ROHC_FAILURE;
                    }
                    index_found = 0xFF;
                }

                count++;
            } /* end of while */
            if(index_found == 0xFF)
            {
                /* This implies it is a new item */
                index_found = rohc_c_add_n_chk_p2_r_ext_hdr_tt(
                        ext_hdr_list, p_csc, &item_present_in_tt, p_ecode);
                if(*p_ecode != ROHC_NO_ERR)
                {
                    return ROHC_FAILURE;
                }
                ROHC_LOG(LOG_INFO,
                        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                        " Item[%d] not found in ref list, hence inserted at[%d]\n",\
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
                        ROHC_CSC_GET_MODE(p_csc), \
                        ((rohc_protocol_node_t *)(tt_entry.p_item))->protocol, \
                        index_found);

                final_idx_list[final_count++] = index_found;
                /* SPR 3444 changes starts */
                if(item_present_in_tt && (index_found < ROHC_MAX_ENTRIES_IN_TT))
                    /* SPR 3444 changes ends */
                {
                    tt_entry = ROHC_CSC_GET_R_EXT_TT(p_csc, index_found);
                    if(tt_entry.known == ROHC_FALSE)
                    {
                        /* Item not yet acked hence needs to be resend */
                        rohc_update_XI_list(&XI_list[index_count], 
                                (rohc_U8bit_t)index_found, 1);
                        if(ROHC_FAILURE == rohc_c_convert_p2_in_buffer(p_entity, p_csc, 
                                    (rohc_U8bit_t *)ext_hdr_list, p_item_list, &size_item_list,
                                    p_ecode))
                        {
                            ROHC_EXIT_FN("rohc_c_r_ext_hdr_list_comp_p2");
                            return ROHC_FAILURE;
                        }
                    }
                    else
                    {
                        /* Item Acked hence no need to send it again */
                        rohc_update_XI_list(&XI_list[index_count], 
                                (rohc_U8bit_t)index_found, 0); 
                    }
                }
                else
                {
                    rohc_update_XI_list(&XI_list[index_count],
                            (rohc_U8bit_t)index_found,1);

                    if(ROHC_FAILURE == rohc_c_convert_p2_in_buffer(p_entity,
                                p_csc,	(rohc_U8bit_t *)ext_hdr_list,p_item_list,
                                &size_item_list,p_ecode))
                    {
                        ROHC_EXIT_FN("rohc_c_r_ext_hdr_list_comp_p2");
                        return ROHC_FAILURE;
                    }
                }
                index_count++;
                if(i<=7)
                    insertion_bit_mask = ROHC_SET_BIT(insertion_bit_mask,
                            (7 - i));
                else
                    insertion_bit_mask = ROHC_SET_BIT(insertion_bit_mask,
                            (24 - i));
                ROHC_LOG(LOG_INFO,
                        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                        "Insertion bit mask constructed till now [%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
                        ROHC_CSC_GET_MODE(p_csc), insertion_bit_mask);
                index_found = 0xFF;
                count = 0;
            }
            else
            {
                index_found = 0xFF;
            }
            ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);
            i++;
        } /* end of while */


        if((insertion_bit_mask != 0) & (item_remove_flag == ROHC_TRUE))
        {
            ROHC_LOG(LOG_INFO,
                    "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                    " Constructing Encoding Type 3 packet\n", \
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
                    ROHC_CSC_GET_MODE(p_csc));

            /*
             ** Incrementing Stats - ROHC_FUTURE_STATS
             ** pdcpStats.statsRohc.numberOfListEncNoRemThenInsSchmUsedCreated++;
             */

            tot_size = rohc_c_enc_type_3_pkt(0, 
                    ref_id, 0, PS_flag, (rohc_U8bit_t)index_count, 
                    removal_bit_mask, insertion_bit_mask, XI_list, 
                    p_item_list, size_item_list, p_compressed_list);

            /* Add the entry in the sliding window */
            /* Klocwork Fix */
            if((final_count > 0)&&(final_count < ROHC_MAX_ENTRIES_IN_TT))
            {
                rohc_c_add_r_mode_ext_hdr_p2_sw (p_csc,
                        (rohc_U8bit_t)final_count,final_idx_list);
            }
        }
        else if(insertion_bit_mask != 0)
        {
            ROHC_LOG(LOG_INFO,
                    "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                    " Constructing Encoding Type 1 packet\n", \
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
                    ROHC_CSC_GET_MODE(p_csc));

            /*
             ** Incrementing Stats - ROHC_FUTURE_STATS
             ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedCreated++;
             */

            tot_size = rohc_c_enc_type_1_pkt(0, 
                    ref_id, 0, PS_flag, (rohc_U8bit_t)index_count, 
                    insertion_bit_mask, XI_list, p_item_list, 
                    size_item_list, p_compressed_list);

            /* Add the entry in the sliding window */
            rohc_c_add_r_mode_ext_hdr_p2_sw (p_csc,(
                        rohc_U8bit_t)final_count,final_idx_list);
        }

    }
    else if ((item_count != tot_count) || (item_remove_flag == ROHC_TRUE))
    {
        /* Construct the Encoding Type 2 packet */
        ROHC_LOG(LOG_INFO,
                "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                " Constructing Encoding Type 2 packet\n", \
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
                ROHC_CSC_GET_MODE(p_csc));

        /*
         ** Incrementing Stats - ROHC_FUTURE_STATS
         ** pdcpStats.statsRohc.numberOfListEncNoRemOnlySchmUsedCreated++;
         */

        tot_size = rohc_c_enc_type_2_pkt (0, 
                0,  0, (rohc_U8bit_t)sw_ref_entry.icount, 
                removal_bit_mask, p_compressed_list);

        /* Add the entry in the sliding window */
        rohc_c_add_r_mode_ext_hdr_p2_sw (p_csc, 
                (rohc_U8bit_t)item_count, index_list);

    }
    else
    {
        /* Implies List is unchanged */
        ROHC_LOG(LOG_INFO,
                "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                "Constructing Encoding Type 1 packet, as list is unchanged\n", \
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
                ROHC_CSC_GET_MODE(p_csc));

        /*
         ** Incrementing Stats - ROHC_FUTURE_STATS
         ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedCreated++;
         */

        tot_size = rohc_c_enc_type_1_pkt (0, ref_id, ROHC_FALSE,PS_flag, 
                (rohc_U8bit_t)index_count, 
                0, XI_list, ROHC_NULL, 0, p_compressed_list);
    } /* endif */

    /* Construct the final  packet */
    *p_length = rohc_c_cnst_pkt(aseq_no, eseq_no, gseq_no, 
            p_compressed_list, tot_size, p_final_comp_list);
    return ROHC_SUCCESS;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohcRExtAckNotRecvdP2
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress EXT HDR LIST in U/O Mode when 
**          ACK is not received
**   	ARGUMENTS: 
**			p_entity - rohc entity context,
**			ext_hdr_list - Pointer to EXT Header List,
**         	p_csc - The compressor context, 
**			p_length - Pointer to Length,
**			p_final_comp_list - pointer to Final Compressed List,
**			XI_list - 
**			p_item_list - Pointer to List of Items,
**			p_compressed_list - Pointer to Compressed List,
**			item_present_in_tt - Item Present in Transition Table,
**			curr_index - Current index,
**			p_ecode - Pointer to Error Code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of gen_id is used as ref_id in U/O Mode
*****************************************************************************/
static rohc_return_t         rohcRExtAckNotRecvdP2 ROHC_PARAMS(
     (p_entity, ext_hdr_list, p_csc, p_length, p_final_comp_list, XI_list,
      p_item_list, p_compressed_list, item_present_in_tt, curr_index, p_ecode),
                     rohc_entity_t      *p_entity           _AND_
                     rohc_list_node_t   *ext_hdr_list       _AND_
                     /* + SPR 17439 */
                     rohc_csc_udp_t     *p_csc              _AND_
                     /* - SPR 17439 */
                     rohc_U32bit_t      *p_length           _AND_
                     rohc_U8bit_t       *p_final_comp_list  _AND_
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
                     rohc_U8bit_t        *p_item_list       _AND_
                     rohc_U8bit_t        *p_compressed_list  _AND_
                     rohc_bool_t        item_present_in_tt  _AND_
                     rohc_U32bit_t      curr_index          _AND_
                     rohc_error_code_t  *p_ecode)

{    
    rohc_U32bit_t       count = 0, aseq_no = 0,
    eseq_no = 0, gseq_no = 0;
    rohc_U32bit_t		size_item_list = 0, tot_size = 0;
    rohc_bool_t			PS_flag = ROHC_FALSE;
    rohc_U8bit_t		index_list[ROHC_MAX_ENTRIES_IN_TT]; 

    while(ext_hdr_list) /* curr_list */
    {
        curr_index = 
            rohc_c_add_n_chk_p2_r_ext_hdr_tt(ext_hdr_list, p_csc, 
                    &item_present_in_tt, p_ecode);
        if(*p_ecode != ROHC_NO_ERR)
        {
            return ROHC_FAILURE;
        }
        index_list[count] = curr_index;

        /* No need to check item_present_in_tt flag as no ACK is received
         * till now so item needs to be resend */

        rohc_update_XI_list(&XI_list[count], curr_index, 1);
        if(ROHC_FAILURE == rohc_c_convert_p2_in_buffer(p_entity, p_csc, 
                    (rohc_U8bit_t *)ext_hdr_list,p_item_list,&size_item_list,p_ecode))
        {
            ROHC_EXIT_FN("rohc_c_r_ext_hdr_list_comp_p2");
            return ROHC_FAILURE;
        }
        count++;
        /* SPR 3532 - Klocwork Fix*/
        if(count >= ROHC_MAX_ENTRIES_IN_TT)
        {
            ROHC_LOG(LOG_INFO,"Invalid value of count");
            ROHC_EXIT_FN("rohc_c_r_ext_hdr_list_comp_p2");
            return ROHC_FAILURE;
        }
        ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);
    } /* end of while */

    /* Construct an Encoding Type 0 packet*/
    ROHC_LOG(LOG_INFO, 
            "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
            "Constructing Encoding Type 0 packet\n", \
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
            ROHC_CSC_GET_MODE(p_csc));

    /*
     ** Incrementing Stats - ROHC_FUTURE_STATS
     ** pdcpStats.statsRohc.numberOfListEncNoGenSchmUsedCreated++;
     */

    tot_size = rohc_c_enc_type_0_pkt(0, ROHC_FALSE, PS_flag, 
            (rohc_U8bit_t)count, XI_list,p_item_list, 
            (rohc_U8bit_t)size_item_list, p_compressed_list);

    /* Add entry in Sliding Window */		
    rohc_c_add_r_mode_ext_hdr_p2_sw (p_csc, (rohc_U8bit_t)count, index_list);

    /* Construct the final packet */
    /* Coverity 54676*/
    *p_length = rohc_c_cnst_pkt(aseq_no, eseq_no , gseq_no,  
            p_compressed_list, tot_size, p_final_comp_list);
    /* Coverity 54676*/
    return ROHC_SUCCESS;
}
/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_uo_ext_hdr_list_comp_p1
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress EXT HDR LIST in U/O Mode
**   	ARGUMENTS: 
**         	p_csc - The compressor context, 
**			p_entity -rohc entity context,
**			p_list - Pointer to List,
**			p_length - Pointer to List,
**			p_final_comp_list - Pointer to Final compressed list,
**			p_ecode - Pointer to Error Code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of gen_id is used as ref_id in U/O Mode
*****************************************************************************/
rohc_return_t	rohc_c_uo_ext_hdr_list_comp_p1 ROHC_PARAMS
	(
		(p_entity, p_csc, p_list, p_length, p_final_comp_list, p_ecode),
			rohc_entity_t	*p_entity 		    _AND_
			rohc_csc_rtp_t	*p_csc 			    _AND_
			rohc_list_t		p_list			    _AND_
			rohc_U32bit_t	*p_length 		    _AND_
			rohc_U8bit_t    *p_final_comp_list 	_AND_
			rohc_error_code_t	*p_ecode
	)
{
    rohc_return_t       ret = ROHC_SUCCESS;
    rohc_U8bit_t        *p_item_list;
   
    /* Coverity Fix 30669 Start */ 
    rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
    /* Coverity Fix 30669 End */ 

    rohc_list_node_t    *ext_hdr_list = rohc_db_get_first_node(&p_list);
    rohc_U32bit_t       count = 0;
    rohc_U32bit_t       curr_index =0;

	rohc_bool_t			PS_flag = ROHC_FALSE;
	rohc_U32bit_t		size_item_list = 0;
	rohc_U8bit_t		*p_compressed_list = 
							p_final_comp_list + ROHC_MAX_EXT_HDR_SIZE;
	rohc_bool_t			item_present_in_tt = ROHC_FALSE; 
    rohc_return_t       returnVal = ROHC_FAILURE;

	ROHC_ENTER_FN("rohc_c_uo_ext_hdr_list_comp_p1");
	if(ext_hdr_list == ROHC_NULL)
	{
        /*Start SPR 5869 Fix */
        /*Generic IP Extension list must be at least one byte with content being
         * 0x00*/
        *p_length = rohc_c_enc_type_0_pkt ((rohc_U8bit_t)0, 0, 
                PS_flag, (rohc_U8bit_t)count, XI_list, ROHC_NULL, 
                (rohc_U8bit_t)size_item_list, p_final_comp_list);
        /*End SPR 5869 Fix */
		ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p1");
		return ret;
	}

	/* Start copying the item list after leaving 25 bytes for the header
	 * the item list will be moved upwards if less bytes are consumed
	 * for the header */
	p_item_list = p_compressed_list + ROHC_MAX_EXT_PKT_HDR_SIZE;

	if(ROHC_CSC_GET_UO_EXT_TT_NIDX(p_csc) > 8)
	{
		/* This implies that the index list will be stored in 1 byte field */
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
		"Setting PS_flag as TRUE\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));
	}

	if (ROHC_FALSE == ROHC_CSC_GET_UO_EXT_SW_ACKED(p_csc))
    {
        /* This implies that Generic Encoding scheme needs to be applied 
		 * as REF ID is not present */
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
		"Handling the case when no ACK is recieved till now\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

        returnVal = rohcUOExtAckNotRecvdP1(p_entity, ext_hdr_list, p_csc, p_length,
                p_final_comp_list, XI_list, p_item_list, p_compressed_list, 
                item_present_in_tt, curr_index,p_ecode );
        
        if (ROHC_FAILURE == returnVal)
        {
            ret = ROHC_FAILURE;
        }
    }
    else
    {
        returnVal = rohcUOExtAckRecvdP1(p_entity, ext_hdr_list, p_csc, p_length,
                p_final_comp_list, XI_list, p_item_list, p_compressed_list,
                p_list, item_present_in_tt, curr_index,p_ecode );

        if (ROHC_FAILURE == returnVal)
        {
            ret = ROHC_FAILURE;
        }
    } /* endif */

	ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p1");
    return ret;
}


/*****************************************************************************
**    	FUNCTION:
**        	rohcUOExtAckRecvdP1
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress EXT HDR LIST in U/O Mode when 
**          ACK is received
**   	ARGUMENTS: 
**			p_entity - rohc entity context,
**			ext_hdr_list - Pointer to EXT Header List,
**         	p_csc - The compressor context, 
**			p_length - Pointer to Length,
**			p_final_comp_list - pointer to Final Compressed List,
**			XI_list - 
**			p_item_list - Pointer to List of Items,
**			p_compressed_list - Pointer to Compressed List,
**			p_list - Pointer to List,
**			item_present_in_tt - Item Present in Transition Table,
**			curr_index - Current index,
**			p_ecode - Pointer to Error Code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of gen_id is used as ref_id in U/O Mode
*****************************************************************************/
static rohc_return_t         rohcUOExtAckRecvdP1 ROHC_PARAMS(
     (p_entity, ext_hdr_list, p_csc, p_length, p_final_comp_list, XI_list,
      p_item_list, p_compressed_list, p_list, item_present_in_tt, curr_index,
      p_ecode),
                     rohc_entity_t      *p_entity           _AND_
                     rohc_list_node_t   *ext_hdr_list       _AND_
                     rohc_csc_rtp_t     *p_csc              _AND_
                     rohc_U32bit_t      *p_length           _AND_
                     rohc_U8bit_t       *p_final_comp_list  _AND_
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
                     rohc_U8bit_t        *p_item_list       _AND_
                     rohc_U8bit_t        *p_compressed_list  _AND_
			         rohc_list_t		p_list			    _AND_
                     rohc_bool_t        item_present_in_tt  _AND_
                     rohc_U32bit_t      curr_index          _AND_
                     rohc_error_code_t  *p_ecode)

{    
    rohc_U32bit_t       gen_id = 0, ref_id = 0,
                        aseq_no = 0, eseq_no = 0, gseq_no = 0;
	rohc_U32bit_t		size_item_list = 0, tot_size = 0,
						index_found = 0xFF; 
	rohc_bool_t			GP_flag = ROHC_TRUE, PS_flag = ROHC_FALSE;
    		    /* Coverity 11395 ends*/
    rohc_U8bit_t        index_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
    		    /* Coverity 11395 ends*/
    rohc_uo_sw_t        sw_ref_entry;
    uo_ext_hdr_tt_t     tt_entry;
    rohc_bool_t         item_remove_flag = ROHC_FALSE;
    rohc_U32bit_t       count = 0, i = 0,
						item_count = 0, 
						final_count = 0,
                        index_count = 0; 
    rohc_U16bit_t       removal_bit_mask = 0xFFFF, insertion_bit_mask = 0;
    rohc_U32bit_t       tot_count = rohc_db_count_of_list(&p_list);
    /* Coverity 11396 ends*/
    rohc_U8bit_t        final_idx_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
    /* Coverity 11396 ends*/

    /*
     * Entries are there in the TT. Hence we need to compare the current
     * Extension header list with the reference list and construct the
     * bit mask accordingly
     */
    sw_ref_entry = ROHC_CSC_GET_UO_EXT_SW_REF(p_csc);
    ref_id = sw_ref_entry.gen_id;
    /* First we will construct removal bit mask */

    ext_hdr_list = rohc_db_get_first_node(&p_list);
    for(count = 0; count < sw_ref_entry.icount; count++) /*ref_list*/
    {
        while(ext_hdr_list) /*curr_list*/
        {
            tt_entry = ROHC_CSC_GET_UO_EXT_TT(p_csc,
                    sw_ref_entry.index_list[count]);
                /* +- SPR 17777 */
            if(ROHC_SUCCESS ==
                    rohc_c_compare_n_encode_p1_ext_hdr_item(
                        tt_entry.p_item, ext_hdr_list, 
                        p_csc, &aseq_no, &gseq_no))
                /* +- SPR 17777 */
            {
                /* Item found in ref_list */
                index_found = sw_ref_entry.index_list[count];
                ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);
                ROHC_LOG(LOG_INFO,
                        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                        " Item found in ref_list at index [%d]\n",  \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), 
                        ROHC_CSC_GET_MODE(p_csc), index_found);

                break;
            }
            if(*p_ecode != ROHC_NO_ERR)
            {
                return ROHC_FAILURE;
            }
            ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);
        } /* end of while */
        if(index_found == 0xFF)
        {
            /* This implies that the reference item is not
             * there in the current list. Set remove bit as 1 */

            ext_hdr_list = rohc_db_get_first_node(&p_list);
            item_remove_flag = ROHC_TRUE;
        }
        else
        {
            /* Set removal bit as 0 */
            if(count < 8)
                removal_bit_mask = ROHC_UNSET_BIT(removal_bit_mask, 
                        (7 - count));
            else
                removal_bit_mask = ROHC_UNSET_BIT(removal_bit_mask, 
                        (24 - count));
            /* SPR 3444 changes starts */
            if(item_count < ROHC_MAX_ENTRIES_IN_TT)   
            {
                index_list[item_count++] = index_found;
            }
            /* SPR 3444 changes ends */

            index_found = 0xff;
        }

    } /* end of for */
    if(item_count < tot_count)
    {
        /* Construct insertion bit mask using the index list
         * constructed above as reference list */
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_CSC_CID(p_csc),curr_index,0,0, __func__,"");
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO,
                "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                " Now constructing Insertion Bit mask \n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

        ext_hdr_list = rohc_db_get_first_node(&p_list);
        count = 0;
        while(ext_hdr_list) /*curr_list*/
        {
            while(count < item_count)  /*ref_list*/
            {
                tt_entry = ROHC_CSC_GET_UO_EXT_TT(p_csc,
                        index_list[count]);
                /* +- SPR 17777 */
                if(ROHC_SUCCESS ==
                        rohc_c_compare_n_encode_p1_ext_hdr_item(
                            tt_entry.p_item, ext_hdr_list, 
                            p_csc, &aseq_no, &gseq_no))
                /* +- SPR 17777 */
                {
                    /* Item found in ref_list */
                    index_found = index_list[count];
                    final_idx_list[final_count++] = index_found;

                    /* Increment the counter */
                    rohc_c_incr_counter_uo_ext_hdr_tt(p_entity, &tt_entry);
                    count++;
                    break;
                }
                else
                {
                    if(*p_ecode != ROHC_NO_ERR)
                    {
                        return ROHC_FAILURE;
                    }
                    index_found = 0xFF;
                }
                count++;
            } /* end of while */
            if(index_found == 0xFF)
            {
                /* This implies it is a new item */
                if( ROHC_FAILURE == rohc_c_add_n_chk_p1_uo_ext_hdr_tt(
                            ext_hdr_list, p_csc, &item_present_in_tt, 
                            &index_found, p_ecode))
                {
                    ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p1");
                    return ROHC_FAILURE;
                }
                ROHC_LOG(LOG_INFO,"CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                        "Item not found in list hence inserted"
                        " in TT at index[%d]",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, 
                        ROHC_CSC_GET_CID(p_csc),
                        ROHC_CSC_GET_MODE(p_csc), index_found);

                final_idx_list[final_count++] = index_found;
                /* SPR 3444 changes starts */
                if(item_present_in_tt && (index_found < ROHC_MAX_ENTRIES_IN_CSRC_TT))
                    /* SPR 3444 changes ends */
                {
                    tt_entry = ROHC_CSC_GET_UO_EXT_TT(p_csc,
                            index_found);
                    if(tt_entry.known == ROHC_FALSE)
                    {
                        /* Item not yet acked hence needs to be resend */
                        rohc_update_XI_list(&XI_list[index_count], 
                                (rohc_U8bit_t)index_found, 1);
                        if(ROHC_FAILURE == rohc_c_convert_p1_in_buffer(
                                    p_entity, p_csc, 
                                    (rohc_U8bit_t *)ext_hdr_list, 
                                    p_item_list, &size_item_list,
                                    p_ecode))
                        {
                            ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p1");
                            return ROHC_FAILURE;
                        }
                    }
                    else
                    {
                        /* Item Acked hence should not be send again */
                        rohc_update_XI_list(&XI_list[index_count], 
                                (rohc_U8bit_t)index_found, 0);
                    }
                }
                else
                {
                    rohc_update_XI_list(&XI_list[index_count], 
                            (rohc_U8bit_t)index_found, 1);
                    if(ROHC_FAILURE == rohc_c_convert_p1_in_buffer(p_entity, 
                                p_csc,(rohc_U8bit_t *)ext_hdr_list, 
                                p_item_list, &size_item_list, 
                                p_ecode))
                    {
                        ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p1");
                        return ROHC_FAILURE;
                    }
                }
                index_count++;
                if(i<=7)
                    insertion_bit_mask = ROHC_SET_BIT(insertion_bit_mask, 
                            (7-i));
                else
                    insertion_bit_mask = ROHC_SET_BIT(insertion_bit_mask, 
                            (24-i));

                ROHC_LOG(LOG_INFO,
                        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                        "Insertion bit mask constructed till now [%d]\n",  \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
                        ROHC_CSC_GET_MODE(p_csc), insertion_bit_mask);

                index_found = 0xFF;
                count = 0;
            }
            else
            {
                index_found = 0xFF;
            }
            i++;
            ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);
        } /* end of while */

        /*Compare this compressed list with the previous list to see whether
         * they are identical or not */

        if(p_csc->dont_update_csc_flag)
        {
            /* Context need not be updated */
            gen_id = 0;
            GP_flag = ROHC_FALSE;
        }
        else
        {
            gen_id = rohc_led_p1_generate_ext_gen_id(p_entity, p_csc, 
                    (rohc_U8bit_t)final_count, final_idx_list, &GP_flag);
        }

        if((insertion_bit_mask != 0) & (item_remove_flag == ROHC_TRUE))
        {
            ROHC_LOG(LOG_INFO,
                    "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                    " Constructing Encoding Type 3 packet \n", \
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                    ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

            /*
             ** Incrementing Stats - ROHC_FUTURE_STATS
             ** pdcpStats.statsRohc.numberOfListEncNoRemThenInsSchmUsedCreated++;
             */

            tot_size = rohc_c_enc_type_3_pkt((rohc_U8bit_t)gen_id, 
                    (rohc_U8bit_t)ref_id, GP_flag, PS_flag, 
                    (rohc_U8bit_t)index_count, removal_bit_mask, 
                    insertion_bit_mask, XI_list, p_item_list, 
                    size_item_list,p_compressed_list);

            rohc_c_add_uo_mode_ext_hdr_p1_sw(p_csc, (rohc_U8bit_t)gen_id, 
                    (rohc_U8bit_t)index_count, 
                    final_idx_list);

        }
        else if(insertion_bit_mask != 0)
        {
            ROHC_LOG(LOG_INFO,
                    "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                    "Constructing Encoding Type 1 packet \n", \
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                    ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

            /*
             ** Incrementing Stats - ROHC_FUTURE_STATS
             ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedCreated++;
             */

            tot_size = rohc_c_enc_type_1_pkt((rohc_U8bit_t)gen_id,
                    (rohc_U8bit_t)ref_id, GP_flag, PS_flag, 
                    (rohc_U8bit_t)index_count, insertion_bit_mask, 
                    XI_list, p_item_list, size_item_list, 
                    p_compressed_list);

            rohc_c_add_uo_mode_ext_hdr_p1_sw(p_csc, (rohc_U8bit_t)gen_id, 
                    (rohc_U8bit_t)index_count, 
                    final_idx_list);

        }

    }
    else if ((item_count != tot_count) || (item_remove_flag == ROHC_TRUE))
    {
        /*Compare this compressed list with the previous list to see whether
         * they are identical or not */

        if(p_csc->dont_update_csc_flag)
        {
            /* Context need not be updated */
            gen_id = 0;
            GP_flag = ROHC_FALSE;
        }
        else
        {
            gen_id = rohc_led_p1_generate_ext_gen_id(p_entity, p_csc, 
                    (rohc_U8bit_t)item_count, index_list, &GP_flag);
        }


        /* Construct the Encoding Type 2 packet */
        ROHC_LOG(LOG_INFO, 
                "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                "Constructing Encoding Type 2 packet \n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

        /*
         ** Incrementing Stats - ROHC_FUTURE_STATS
         ** pdcpStats.statsRohc.numberOfListEncNoRemOnlySchmUsedCreated++;
         */

        tot_size = rohc_c_enc_type_2_pkt ((rohc_U8bit_t)gen_id,
                (rohc_U8bit_t)ref_id, GP_flag, 
                (rohc_U8bit_t)sw_ref_entry.icount,removal_bit_mask, 
                p_compressed_list);

        rohc_c_add_uo_mode_ext_hdr_p1_sw(p_csc, (rohc_U8bit_t)gen_id, 
                (rohc_U8bit_t)sw_ref_entry.icount, 
                index_list);
    }
    else
    {
        /* Implies List is unchanged w.r.t ref list */

        ROHC_LOG(LOG_INFO,
                "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                "Constructing Encoding Type 1 packet as list is unchanged\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

        /*
         ** Incrementing Stats - ROHC_FUTURE_STATS
         ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedCreated++;
         */


        if(p_csc->dont_update_csc_flag == ROHC_FALSE)
        {
            /* Increment the gen_id counter */
            sw_ref_entry.gen_count++;
        }
        tot_size = rohc_c_enc_type_1_pkt (0, (rohc_U8bit_t)ref_id, 
                ROHC_FALSE, PS_flag, (rohc_U8bit_t)item_count, 0, 0, 
                ROHC_NULL, 0, p_compressed_list);
    } /* endif */

    /* Construct the final  packet */
    *p_length =rohc_c_cnst_pkt(aseq_no,eseq_no, gseq_no, p_compressed_list,
            tot_size, p_final_comp_list);

    return ROHC_SUCCESS;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohcUOExtAckNotRecvdP1
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress EXT HDR LIST in U/O Mode when 
**          ACK is not received
**   	ARGUMENTS: 
**			p_entity - rohc entity context,
**			ext_hdr_list - Pointer to EXT Header List,
**         	p_csc - The compressor context, 
**			p_length - Pointer to Length,
**			p_final_comp_list - pointer to Final Compressed List,
**			XI_list - 
**			p_item_list - Pointer to List of Items,
**			p_compressed_list - Pointer to Compressed List,
**			item_present_in_tt - Item Present in Transition Table,
**			curr_index - Current index,
**			p_ecode - Pointer to Error Code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of gen_id is used as ref_id in U/O Mode
*****************************************************************************/
static rohc_return_t         rohcUOExtAckNotRecvdP1 ROHC_PARAMS(
     (p_entity, ext_hdr_list, p_csc, p_length, p_final_comp_list, XI_list,
      p_item_list, p_compressed_list, item_present_in_tt, curr_index, p_ecode),
                     rohc_entity_t      *p_entity           _AND_
                     rohc_list_node_t   *ext_hdr_list       _AND_
                     rohc_csc_rtp_t     *p_csc              _AND_
                     rohc_U32bit_t      *p_length           _AND_
                     rohc_U8bit_t       *p_final_comp_list  _AND_
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
                     rohc_U8bit_t        *p_item_list       _AND_
                     rohc_U8bit_t        *p_compressed_list  _AND_
                     rohc_bool_t        item_present_in_tt  _AND_
                     rohc_U32bit_t      curr_index          _AND_
                     rohc_error_code_t  *p_ecode)

{    
    rohc_U32bit_t       count = 0, gen_id = 0, aseq_no = 0,
                        eseq_no = 0, gseq_no = 0;
	rohc_U32bit_t		size_item_list = 0, tot_size = 0;
	rohc_bool_t			GP_flag = ROHC_TRUE, PS_flag = ROHC_FALSE;
    rohc_U8bit_t        index_list[ROHC_MAX_ENTRIES_IN_TT] = {0};

    while(ext_hdr_list)
    {
        if( ROHC_FAILURE == rohc_c_add_n_chk_p1_uo_ext_hdr_tt(ext_hdr_list,
                    p_csc, &item_present_in_tt, &curr_index,p_ecode))
        {
            ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p1");
            return ROHC_FAILURE;
        }
        index_list[count] = curr_index;

        /* No need to check item_present_in_tt flag */
        rohc_update_XI_list(&XI_list[count], (rohc_U8bit_t)curr_index, 1);
        if(ROHC_FAILURE == rohc_c_convert_p1_in_buffer(p_entity, p_csc, 
                    (rohc_U8bit_t *)ext_hdr_list,p_item_list,&size_item_list,p_ecode))
        {
            ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p1");
            return ROHC_FAILURE;
        }
        count++;
        /* SPR 3532 - Klocwork Fix */
        if(count >= ROHC_MAX_ENTRIES_IN_TT)
        {
            ROHC_LOG(LOG_INFO,"Invalid value of count");
            ROHC_EXIT_FN("rohc_c_r_ext_hdr_list_comp_p1");
            return ROHC_FAILURE;
        }
        ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);

    } /* end of while */
    if(p_csc->dont_update_csc_flag)
    {
        /* Context need not be updated */
        gen_id = 0;
        GP_flag = ROHC_FALSE;
    }
    else
    {
        if (ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc) != 0)
        {
            /* Items are present in SW
             * Compare this compressed list with the previous list 
             * to see whether they are identical or not */

            gen_id = rohc_led_p1_generate_ext_gen_id(p_entity, p_csc, 
                    (rohc_U8bit_t)count, index_list, &GP_flag);
        }
        else
        {
            /* No items are present in SW 
             * hence generate a new gen_id as context updating packet */
            gen_id = 1;
            ROHC_LOG(LOG_INFO,
                    "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                    "Generating new gen_id [%d] for the list\n", \
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                    ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), gen_id);

            /* Add the entry in the sliding window */
            GP_flag = ROHC_TRUE;
        }
        rohc_c_add_uo_mode_ext_hdr_p1_sw(p_csc, (rohc_U8bit_t)gen_id, 
                (rohc_U8bit_t)count,index_list);
    }

    /*
     ** Incrementing Stats - ROHC_FUTURE_STATS
     ** pdcpStats.statsRohc.numberOfListEncNoGenSchmUsedCreated++;
     */

    tot_size = rohc_c_enc_type_0_pkt ((rohc_U8bit_t)gen_id, GP_flag, 
            PS_flag, (rohc_U8bit_t)count, XI_list, p_item_list, 
            (rohc_U8bit_t)size_item_list, p_compressed_list);

    /* Construct the final packet */
    *p_length = rohc_c_cnst_pkt(aseq_no, eseq_no, gseq_no, 
            p_compressed_list, tot_size, p_final_comp_list);

    return ROHC_SUCCESS;
}
/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_uo_ext_hdr_list_comp_p2
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress EXT HDR LIST in U/O Mode
**   	ARGUMENTS: 
**			p_entity - rohc entity context,
**         	p_csc - The compressor context, 
**			p_length - Pointer to Length,
**			p_list - Pointer to List
**			p_final_comp_list - pointer to Final Compressed List,
**			p_ecode - Pointer to Error Code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of gen_id is used as ref_id in U/O Mode
*****************************************************************************/
rohc_return_t	rohc_c_uo_ext_hdr_list_comp_p2 ROHC_PARAMS
	(
		(p_entity, p_csc, p_list, p_length, p_final_comp_list, p_ecode),
			rohc_entity_t	    *p_entity 		      _AND_
			rohc_csc_udp_t		*p_csc 		          _AND_
			rohc_list_t		    p_list		          _AND_
			rohc_U32bit_t	    *p_length 		      _AND_
			rohc_U8bit_t      *p_final_comp_list   	  _AND_
			rohc_error_code_t	*p_ecode
	)
{
    rohc_return_t       ret = ROHC_SUCCESS;
    rohc_U32bit_t       count = 0;
    rohc_U8bit_t        *p_item_list;
    rohc_U32bit_t       curr_index = 0;
    /* Coverity Fix 30668 Start */ 
    rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
    /* Coverity Fix 30668 End */ 
    rohc_list_node_t    *ext_hdr_list = rohc_db_get_first_node(&p_list);
    rohc_return_t       returnVal = ROHC_FAILURE;

	rohc_bool_t			PS_flag = ROHC_FALSE;
	rohc_U32bit_t		size_item_list = 0;
	rohc_U8bit_t		*p_compressed_list = 
							p_final_comp_list + ROHC_MAX_EXT_HDR_SIZE;
	rohc_bool_t			item_present_in_tt = ROHC_FALSE;

	ROHC_ENTER_FN("rohc_c_uo_ext_hdr_list_comp_p2");
	if(ext_hdr_list == ROHC_NULL)
    {
        /*Start SPR 5869 Fix */
        /*Generic IP Extension list must be at least one byte with content being
         * 0x00*/
        *p_length = rohc_c_enc_type_0_pkt ((rohc_U8bit_t)0, 0, 
                PS_flag, (rohc_U8bit_t)count, XI_list, ROHC_NULL, 
                (rohc_U8bit_t)size_item_list, p_final_comp_list);
        /*End SPR 5869 Fix */
		ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p2");
		return ret;
	}

	/* Start copying the item list after leaving 25 bytes for the header
	 * the item list will be moved upwards if less bytes are consumed
	 * for the header */
	p_item_list = p_compressed_list + ROHC_MAX_EXT_PKT_HDR_SIZE;

	if(ROHC_CSC_GET_UO_EXT_TT_NIDX(p_csc) > 8)
	{
		/* This implies that the index list will be stored in 1 byte field */
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
		"Setting PS_flag as TRUE\n", \
		p_entity->rohc_entity_id.ueIndex,
        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),	\
		ROHC_CSC_GET_MODE(p_csc));
	}

	if (ROHC_FALSE == ROHC_CSC_GET_UO_EXT_SW_ACKED(p_csc))
    {
        /* This implies that Generic Encoding scheme needs to be applied 
		 * as REF ID is not present */
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
		"Handling the case when no ACK is recieved till now\n", \
		p_entity->rohc_entity_id.ueIndex,
        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
		ROHC_CSC_GET_MODE(p_csc));

        returnVal = rohcUOExtAckNotRecvdP2(p_entity, ext_hdr_list, p_csc, p_length,
                       p_final_comp_list, XI_list, p_item_list, p_compressed_list,
                       item_present_in_tt, curr_index,p_ecode );

        if (ROHC_FAILURE == returnVal)
        {
            ret = ROHC_FAILURE;
        }
    }
    else
    {
        returnVal = rohcUOExtAckRecvdP2(p_entity, ext_hdr_list, p_csc, p_length,
                p_final_comp_list, XI_list, p_item_list, p_compressed_list,
                p_list, item_present_in_tt, curr_index,p_ecode );

        if (ROHC_FAILURE == returnVal)
        {
            ret = ROHC_FAILURE;
        }
    } /* endif */

	ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p2");
    return ret;
}


/*****************************************************************************
**    	FUNCTION:
**        	rohcUOExtAckRecvdP2
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress EXT HDR LIST in U/O Mode when 
**          ACK is received
**   	ARGUMENTS: 
**			p_entity - rohc entity context,
**			ext_hdr_list - Pointer to EXT Header List,
**         	p_csc - The compressor context, 
**			p_length - Pointer to Length,
**			p_final_comp_list - pointer to Final Compressed List,
**			XI_list - 
**			p_item_list - Pointer to List of Items,
**			p_compressed_list - Pointer to Compressed List,
**			p_list - Pointer to List,
**			item_present_in_tt - Item Present in Transition Table,
**			curr_index - Current index,
**			p_ecode - Pointer to Error Code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of gen_id is used as ref_id in U/O Mode
*****************************************************************************/
static rohc_return_t         rohcUOExtAckRecvdP2 ROHC_PARAMS(
     (p_entity, ext_hdr_list, p_csc, p_length, p_final_comp_list, XI_list,
      p_item_list, p_compressed_list, p_list, item_present_in_tt, curr_index,
      p_ecode),
                     rohc_entity_t      *p_entity           _AND_
                     rohc_list_node_t   *ext_hdr_list       _AND_
                     /* + SPR 17439 */
                     rohc_csc_udp_t     *p_csc              _AND_
                     /* - SPR 17439 */
                     rohc_U32bit_t      *p_length           _AND_
                     rohc_U8bit_t       *p_final_comp_list  _AND_
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
                     rohc_U8bit_t        *p_item_list       _AND_
                     rohc_U8bit_t        *p_compressed_list  _AND_
			         rohc_list_t		p_list			    _AND_
                     rohc_bool_t        item_present_in_tt  _AND_
                     rohc_U32bit_t      curr_index          _AND_
                     rohc_error_code_t  *p_ecode)

{    
    rohc_U32bit_t       gen_id = 0, ref_id = 0,
    aseq_no = 0, eseq_no = 0, gseq_no = 0;
    rohc_U32bit_t		size_item_list = 0, tot_size = 0,
                        index_found = 0xFF; 
    rohc_bool_t			GP_flag = ROHC_TRUE, PS_flag = ROHC_FALSE;
    /* Coverity 11395 ends*/
    rohc_U8bit_t        index_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
    /* Coverity 11395 ends*/
    rohc_uo_sw_t        sw_ref_entry;
    uo_ext_hdr_tt_t     tt_entry;
    rohc_bool_t         item_remove_flag = ROHC_FALSE;
    rohc_U32bit_t       count = 0, i = 0,
                        item_count = 0, 
                        final_count = 0,
                        index_count = 0; 
    rohc_U16bit_t       removal_bit_mask = 0xFFFF, insertion_bit_mask = 0;
    rohc_U32bit_t       tot_count = rohc_db_count_of_list(&p_list);
    /* Coverity 11396 ends*/
    rohc_U8bit_t        final_idx_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
    /* Coverity 11396 ends*/

    /*
     * Entries are there in the TT. Hence we need to compare the current
     * Extension header list with the reference list and construct the
     * bit mask accordingly
     */
    sw_ref_entry = ROHC_CSC_GET_UO_EXT_SW_REF(p_csc);
    ref_id = sw_ref_entry.gen_id;
    /* First we will construct removal bit mask */

    ext_hdr_list = rohc_db_get_first_node(&p_list);
    for(count = 0; count < sw_ref_entry.icount; count++) /*ref_list*/
    {
        while(ext_hdr_list) /*curr_list*/
        {
            tt_entry = ROHC_CSC_GET_UO_EXT_TT(p_csc,
                    sw_ref_entry.index_list[count]);
                /* +- SPR 17777 */
            if(ROHC_SUCCESS ==
                    rohc_c_compare_n_encode_p2_ext_hdr_item(
                        tt_entry.p_item, ext_hdr_list, 
                        p_csc, &aseq_no, &gseq_no))
                /* +- SPR 17777 */
            {
                /* Item found in ref_list */
                index_found = sw_ref_entry.index_list[count];
                ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);
                ROHC_LOG(LOG_INFO,
                        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                        " Item found in ref_list at index [%d]\n",  \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
                        ROHC_CSC_GET_MODE(p_csc), index_found);

                break;
            }
            if(*p_ecode != ROHC_NO_ERR)
            {
                return ROHC_FAILURE;
            }
            ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);
        } /* end of while */
        if(index_found == 0xFF)
        {
            /* This implies that the reference item is not
             * there in the current list. Set remove bit as 1 */

            ext_hdr_list = rohc_db_get_first_node(&p_list);
            item_remove_flag = ROHC_TRUE;
        }
        else
        {
            /* Set removal bit as 0 */
            if(count < 8)
                removal_bit_mask = ROHC_UNSET_BIT(removal_bit_mask, 
                        (7 - count));
            else
                removal_bit_mask = ROHC_UNSET_BIT(removal_bit_mask,
                        (24 - count));
            /* Klocwork Fix */
            if(item_count < ROHC_MAX_ENTRIES_IN_TT)
            {
                index_list[item_count++] = index_found;
            }
            index_found = 0xff;
        }

    } /* end of for */
    if(item_count < tot_count)
    {
        /* Construct insertion bit mask using the index list
         * constructed above as reference list */
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_CSC_CID(p_csc),curr_index,0,0, __func__,"");
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO,
                "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                " Now constructing Insertion Bit mask \n", \
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
                ROHC_CSC_GET_MODE(p_csc));

        ext_hdr_list = rohc_db_get_first_node(&p_list);
        count = 0;
        while(ext_hdr_list) /*curr_list*/
        {
            while(count < item_count)  /*ref_list*/
            {
                tt_entry = ROHC_CSC_GET_UO_EXT_TT(p_csc,
                        index_list[count]);
                /* +- SPR 17777 */
                if(ROHC_SUCCESS ==
                        rohc_c_compare_n_encode_p2_ext_hdr_item(
                            tt_entry.p_item, ext_hdr_list, 
                            p_csc, &aseq_no, &gseq_no))
                /* +- SPR 17777 */
                {
                    /* Item found in ref_list */
                    index_found = index_list[count];
                    final_idx_list[final_count++] = index_found;

                    /* Increment the counter */
                    rohc_c_incr_counter_uo_ext_hdr_tt(p_entity, &tt_entry);
                    count++;
                    break;
                }
                else
                {
                    if(*p_ecode != ROHC_NO_ERR)
                    {
                        return ROHC_FAILURE;
                    }
                    index_found = 0xFF;
                }
                count++;
            } /* end of while */
            if(index_found == 0xFF)
            {
                /* This implies it is a new item */
                if( ROHC_FAILURE == rohc_c_add_n_chk_p2_uo_ext_hdr_tt(
                            ext_hdr_list, p_csc, &item_present_in_tt, 
                            &index_found, p_ecode))
                {
                    ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p2");
                    return ROHC_FAILURE;
                }
                ROHC_LOG(LOG_INFO,"CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                        "Item not found in list hence inserted in TT at index[%d]", \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
                        ROHC_CSC_GET_MODE(p_csc), index_found);

                final_idx_list[final_count++] = index_found;
                /* SPR 3444 changes starts */
                if(item_present_in_tt && (index_found < ROHC_MAX_ENTRIES_IN_CSRC_TT))
                    /* SPR 3444 changes ends */
                {
                    tt_entry = ROHC_CSC_GET_UO_EXT_TT(p_csc,
                            index_found);
                    if(tt_entry.known == ROHC_FALSE)
                    {
                        /* Item not yet acked hence needs to be resend */
                        rohc_update_XI_list(&XI_list[index_count], 
                                (rohc_U8bit_t)index_found, 1);
                        if(ROHC_FAILURE == rohc_c_convert_p2_in_buffer(p_entity, p_csc, 
                                    (rohc_U8bit_t *)ext_hdr_list, p_item_list, &size_item_list,
                                    p_ecode))
                        {
                            ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p2");
                            return ROHC_FAILURE;
                        }
                    }
                    else
                    {
                        /* Item Acked hence should not be send again */
                        rohc_update_XI_list(&XI_list[index_count], 
                                (rohc_U8bit_t)index_found, 0);
                    }
                }
                else
                {
                    rohc_update_XI_list(&XI_list[index_count], (rohc_U8bit_t)index_found,
                            1);
                    if(ROHC_FAILURE == rohc_c_convert_p2_in_buffer(p_entity, p_csc, 
                                (rohc_U8bit_t *)ext_hdr_list, p_item_list, 
                                &size_item_list, p_ecode))
                    {
                        ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p2");
                        return ROHC_FAILURE;
                    }
                }
                index_count++;
                if(i<=7)
                    insertion_bit_mask = ROHC_SET_BIT(insertion_bit_mask,
                            (7-i));
                else
                    insertion_bit_mask = ROHC_SET_BIT(insertion_bit_mask,
                            (24-i));

                ROHC_LOG(LOG_INFO,
                        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                        "Insertion bit mask constructed till now [%d]\n",  \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
                        ROHC_CSC_GET_MODE(p_csc), insertion_bit_mask);

                index_found = 0xFF;
                count = 0;
            }
            else
            {
                index_found = 0xFF;
            }
            i++;
            ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);
        } /* end of while */

        /*Compare this compressed list with the previous list to see whether
         * they are identical or not */

        if(p_csc->dont_update_csc_flag)
        {
            /* Context need not be updated */
            gen_id = 0;
            GP_flag = ROHC_FALSE;
        }
        else
        {
            gen_id = rohc_led_p2_generate_ext_gen_id(p_entity, p_csc, 
                    (rohc_U8bit_t)final_count, final_idx_list, &GP_flag);
        }

        if((insertion_bit_mask != 0) & (item_remove_flag == ROHC_TRUE))
        {
            ROHC_LOG(LOG_INFO,
                    "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                    " Constructing Encoding Type 3 packet \n", \
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
                    ROHC_CSC_GET_MODE(p_csc));

            /*
             ** Incrementing Stats - ROHC_FUTURE_STATS
             ** pdcpStats.statsRohc.numberOfListEncNoRemThenInsSchmUsedCreated++;
             */

            tot_size = rohc_c_enc_type_3_pkt((rohc_U8bit_t)gen_id, 
                    (rohc_U8bit_t)ref_id, GP_flag, PS_flag, 
                    (rohc_U8bit_t)index_count, removal_bit_mask, 
                    insertion_bit_mask, XI_list, p_item_list, 
                    size_item_list,p_compressed_list);

            rohc_c_add_uo_mode_ext_hdr_p2_sw(p_csc, (rohc_U8bit_t)gen_id,
                    (rohc_U8bit_t)index_count, final_idx_list);

        }
        else if(insertion_bit_mask != 0)
        {
            ROHC_LOG(LOG_INFO,
                    "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                    "Constructing Encoding Type 1 packet \n", \
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
                    ROHC_CSC_GET_MODE(p_csc));

            /*
             ** Incrementing Stats - ROHC_FUTURE_STATS
             ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedCreated++;
             */

            tot_size = rohc_c_enc_type_1_pkt((rohc_U8bit_t)gen_id,
                    (rohc_U8bit_t)ref_id, GP_flag, PS_flag, 
                    (rohc_U8bit_t)index_count, insertion_bit_mask, XI_list, 
                    p_item_list,size_item_list, p_compressed_list);

            rohc_c_add_uo_mode_ext_hdr_p2_sw(p_csc, (rohc_U8bit_t)gen_id,
                    (rohc_U8bit_t)index_count, final_idx_list);

        }

    }
    else if ((item_count != tot_count) || (item_remove_flag == ROHC_TRUE))
    {
        /*Compare this compressed list with the previous list to see whether
         * they are identical or not */

        if(p_csc->dont_update_csc_flag)
        {
            /* Context need not be updated */
            gen_id = 0;
            GP_flag = ROHC_FALSE;
        }
        else
        {
            gen_id = rohc_led_p2_generate_ext_gen_id(p_entity, p_csc, 
                    (rohc_U8bit_t)item_count, index_list, &GP_flag);
        }


        /* Construct the Encoding Type 2 packet */
        ROHC_LOG(LOG_INFO, 
                "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                "Constructing Encoding Type 2 packet \n", \
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
                ROHC_CSC_GET_MODE(p_csc));

        /*
         ** Incrementing Stats - ROHC_FUTURE_STATS
         ** pdcpStats.statsRohc.numberOfListEncNoRemOnlySchmUsedCreated++;
         */

        tot_size = rohc_c_enc_type_2_pkt ((rohc_U8bit_t)gen_id,
                (rohc_U8bit_t)ref_id, GP_flag, 
                (rohc_U8bit_t)sw_ref_entry.icount,removal_bit_mask,
                p_compressed_list);

        rohc_c_add_uo_mode_ext_hdr_p2_sw(p_csc, (rohc_U8bit_t)gen_id, 
                (rohc_U8bit_t)sw_ref_entry.icount, index_list);
    }
    else
    {
        /* Implies List is unchanged w.r.t ref list */

        ROHC_LOG(LOG_INFO,
                "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                "Constructing Encoding Type 1 packet as list is unchanged\n", \
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
                ROHC_CSC_GET_MODE(p_csc));

        /*
         ** Incrementing Stats - ROHC_FUTURE_STATS
         ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedCreated++;
         */

        if(p_csc->dont_update_csc_flag == ROHC_FALSE)
        {
            /* Increment the gen_id counter */
            sw_ref_entry.gen_count++;
        }
        tot_size = rohc_c_enc_type_1_pkt (0, (rohc_U8bit_t)ref_id, 
                ROHC_FALSE, PS_flag,(rohc_U8bit_t)item_count, 
                0, 0, ROHC_NULL, 0, p_compressed_list);
    } /* endif */

    /* Construct the final  packet */
    *p_length =rohc_c_cnst_pkt(aseq_no,eseq_no, gseq_no, p_compressed_list,
            tot_size, p_final_comp_list);
    return ROHC_SUCCESS;
}
/*****************************************************************************
**    	FUNCTION:
**        	rohcUOExtAckNotRecvdP2
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress EXT HDR LIST in U/O Mode when 
**          ACK is not received
**   	ARGUMENTS: 
**			p_entity - rohc entity context,
**			ext_hdr_list - Pointer to EXT Header List,
**         	p_csc - The compressor context, 
**			p_length - Pointer to Length,
**			p_final_comp_list - pointer to Final Compressed List,
**			XI_list - 
**			p_item_list - Pointer to List of Items,
**			p_compressed_list - Pointer to Compressed List,
**			item_present_in_tt - Item Present in Transition Table,
**			curr_index - Current index,
**			p_ecode - Pointer to Error Code
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of gen_id is used as ref_id in U/O Mode
*****************************************************************************/
static rohc_return_t         rohcUOExtAckNotRecvdP2 ROHC_PARAMS(
     (p_entity, ext_hdr_list, p_csc, p_length, p_final_comp_list, XI_list,
      p_item_list, p_compressed_list, item_present_in_tt, curr_index, p_ecode),
                     rohc_entity_t      *p_entity           _AND_
                     rohc_list_node_t   *ext_hdr_list       _AND_
                     /* + SPR 17439 */
                     rohc_csc_udp_t     *p_csc              _AND_
                     /* - SPR 17439 */
                     rohc_U32bit_t      *p_length           _AND_
                     rohc_U8bit_t       *p_final_comp_list  _AND_
                     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
                     rohc_U8bit_t        *p_item_list       _AND_
                     rohc_U8bit_t        *p_compressed_list  _AND_
                     rohc_bool_t        item_present_in_tt  _AND_
                     rohc_U32bit_t      curr_index          _AND_
                     rohc_error_code_t  *p_ecode)

{    
    rohc_U32bit_t       count = 0, gen_id = 0, aseq_no = 0,
    eseq_no = 0, gseq_no = 0;
    rohc_U32bit_t		size_item_list = 0, tot_size = 0;
    rohc_bool_t			GP_flag = ROHC_TRUE, PS_flag = ROHC_FALSE;
    rohc_U8bit_t        index_list[ROHC_MAX_ENTRIES_IN_TT] = {0};

    while(ext_hdr_list)
    {
        while(ext_hdr_list)
        {
            if( ROHC_FAILURE == rohc_c_add_n_chk_p2_uo_ext_hdr_tt(ext_hdr_list,
                        p_csc, &item_present_in_tt, &curr_index,p_ecode))
            {
                ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p2");
                return ROHC_FAILURE;
            }
            index_list[count] = curr_index;

            /* No need to check item_present_in_tt flag */
            rohc_update_XI_list(&XI_list[count], (rohc_U8bit_t)curr_index, 1);
            if(ROHC_FAILURE == rohc_c_convert_p2_in_buffer(p_entity, p_csc, 
                        (rohc_U8bit_t *)ext_hdr_list,p_item_list,&size_item_list,p_ecode))
            {
                ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p2");
                return ROHC_FAILURE;
            }
            count++;
            /* SPR 3532 - Klocwork Fix */
            if(count >= ROHC_MAX_ENTRIES_IN_TT)
            {
                ROHC_LOG(LOG_INFO,"Invalid value of count");
                ROHC_EXIT_FN("rohc_c_uo_ext_hdr_list_comp_p2");
                return ROHC_FAILURE;
            }
            ext_hdr_list = rohc_db_get_next_node(ext_hdr_list);
        } /* end of while */
        if(p_csc->dont_update_csc_flag)
        {
            /* Context need not be updated */
            gen_id = 0;
            GP_flag = ROHC_FALSE;
        }
        else
        {
            if (ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc) != 0)
            {
                /* Items are present in SW
                 * Compare this compressed list with the previous list 
                 * to see whether they are identical or not */
                gen_id = rohc_led_p2_generate_ext_gen_id(p_entity, 
                        p_csc, (rohc_U8bit_t)count, index_list, &GP_flag);
            }
            else
            {
                /* No items are present in SW 
                 * hence generate a new gen_id as context updating packet */
                gen_id = 1;
                ROHC_LOG(LOG_INFO,
                        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                        "Generating new gen_id [%d] for the list\n", \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
                        ROHC_CSC_GET_MODE(p_csc), gen_id);

                /* Add the entry in the sliding window */
                GP_flag = ROHC_TRUE;
            }
            rohc_c_add_uo_mode_ext_hdr_p2_sw(p_csc, (rohc_U8bit_t)gen_id, 
                    (rohc_U8bit_t)count,index_list);
        }

        /*
         ** Incrementing Stats - ROHC_FUTURE_STATS
         ** pdcpStats.statsRohc.numberOfListEncNoGenSchmUsedCreated++;
         */

        tot_size = rohc_c_enc_type_0_pkt ((rohc_U8bit_t)gen_id, GP_flag, 
                PS_flag, (rohc_U8bit_t)count, XI_list, p_item_list, 
                (rohc_U8bit_t)size_item_list, p_compressed_list);

        /* Construct the final packet */
        *p_length = rohc_c_cnst_pkt(aseq_no, eseq_no, gseq_no, 
                p_compressed_list, tot_size, p_final_comp_list);

    }

    return ROHC_SUCCESS;
}
