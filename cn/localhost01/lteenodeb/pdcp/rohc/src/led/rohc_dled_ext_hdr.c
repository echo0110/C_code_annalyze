/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dled_ext_hdr.c,v 1.3.36.4 2010/11/24 03:40:07 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the functions for De-Compression of IP
 *                     Extension header list.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dled_ext_hdr.c,v $
 * Revision 1.3.36.4  2010/11/24 03:40:07  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.3.36.3  2010/11/09 05:18:57  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.36.2  2010/10/13 03:50:39  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.36.1  2010/10/04 03:41:50  gur22059
 * Rename function rohc_udp_led_init_inneripext to rohc_led_p2_init_inneripext
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
#include "rohc_com_def.h"
#include "rohc_g_typ.h"
#include "rohc_gcom_mcr.h"
#include "rohc_protocol_header.h"
#include "rohc_d_typ.h"
#include "rohc_dcom_mcr.h"
#include "rohc_trace_mcr.h"
#include "rohc_led_proto.h"
#include "rohc_led_def.h"
#include "rohc_led_mcr.h"

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
**          rohc_dled_p1_uo_inneripext_list_dec
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to decode INNER IP EXT HDR LIST in U/O Mode
**      ARGUMENTS:
**          p_dsc - Pointer to De-compressor context, 
**			p_entity - Pointer to Rohc entity context,
**			p_length - Pointer to length,
**			p_compressed_pkt - Pointer to compressed packet, 
**			p_ecode - Pointer to Error Code.
**      RETURN VALUE:
**          rohc_return_t
**      NOTES:
*****************************************************************************/


rohc_return_t	rohc_dled_p1_uo_inneripext_list_dec 	ROHC_PARAMS
(
	(p_entity, p_dsc, p_length, p_compressed_pkt, p_ecode),
	rohc_entity_t	*p_entity	_AND_
	rohc_dsc_rtp_t  *p_dsc		_AND_
	rohc_U32bit_t	*p_length	_AND_
	rohc_U8bit_t	*p_compressed_pkt	_AND_
	rohc_error_code_t	*p_ecode
)
{
	rohc_return_t	ret = ROHC_SUCCESS;
	rohc_ah_t	*p_ah = ROHC_NULL;
	rohc_gre_t	*p_gre = ROHC_NULL;
	rohc_dpib_t	*p_dpib = ROHC_DSC_GET_DPIB(p_dsc);

	ROHC_ENTER_FN("rohc_dled_p1_uo_inneripext_list_dec");

	rohc_led_p1_init_inneripext(p_dpib);
	ret = rohc_d_ext_hdr_list_dec_p1(p_entity, p_dsc, p_compressed_pkt, 
                p_length, &(p_dpib->inneripext_new_item_count), 
                &(p_dpib->inneripextlist), p_dpib->inneripext_tt, 
                p_dpib->inneripext_new_index_list, 
                (rohc_U8bit_t *)&(p_dpib->inneripext_uo_sw), 
                &p_ah,&p_gre, p_ecode);

	p_dpib->inner_ah_ptr = (rohc_U8bit_t *)p_ah;
	p_dpib->inner_gre = (rohc_U8bit_t *)p_gre;

	ROHC_EXIT_FN("rohc_dled_p1_uo_inneripext_list_dec");
	return ret;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_dled_p2_uo_inneripext_list_dec
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to decode INNER IP EXT HDR LIST in U/O Mode
**      ARGUMENTS:
**          p_dsc - Pointer to De-compressor context, 
**			p_entity - Pointer to Rohc entity context,
**			p_length - Pointer to length,
**			p_compressed_pkt - Pointer to compressed packet, 
**			p_ecode - Pointer to Error Code.
**      RETURN VALUE:
**          rohc_return_t
**      NOTES:
*****************************************************************************/


rohc_return_t   rohc_dled_p2_uo_inneripext_list_dec    ROHC_PARAMS
(
    (p_entity, p_dsc, p_length, p_compressed_pkt, p_ecode),
    rohc_entity_t       *p_entity           _AND_
    rohc_dsc_udp_t      *p_dsc              _AND_
    rohc_U32bit_t       *p_length           _AND_
    rohc_U8bit_t        *p_compressed_pkt   _AND_
    rohc_error_code_t   *p_ecode
)
{
    rohc_return_t   ret = ROHC_SUCCESS;
    rohc_ah_t   *p_ah = ROHC_NULL;
    rohc_gre_t  *p_gre = ROHC_NULL;
    rohc_dpib_t *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    ROHC_ENTER_FN("rohc_dled_p2_uo_inneripext_list_dec");

    rohc_led_p2_init_inneripext(p_dpib);
    ret = rohc_d_ext_hdr_list_dec_p2(p_entity, p_dsc, p_compressed_pkt, p_length,
            &(p_dpib->inneripext_new_item_count), &(p_dpib->inneripextlist),
            p_dpib->inneripext_tt, p_dpib->inneripext_new_index_list,
            (rohc_U8bit_t *)&(p_dpib->inneripext_uo_sw), &p_ah,&p_gre, p_ecode);

    p_dpib->inner_ah_ptr = (rohc_U8bit_t *)p_ah;
    p_dpib->inner_gre = (rohc_U8bit_t *)p_gre;

    ROHC_EXIT_FN("rohc_dled_p2_uo_inneripext_list_dec");
    return ret;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_dled_p1_r_inneripext_list_dec
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to de-compress INNER IP EXT HDR LIST in R Mode
**      ARGUMENTS:
**          p_dsc - Pointer to De-compressor context, 
**			p_entity - Pointer to Rohc entity context,
**			p_length - Pointer to length,
**			p_compressed_pkt - Pointer to compressed packet, 
**			p_ecode - Pointer to Error Code.
**      RETURN VALUE:
**          rohc_return_t
**      NOTES:
*****************************************************************************/
rohc_return_t	rohc_dled_p1_r_inneripext_list_dec 	ROHC_PARAMS
(
	(p_entity, p_dsc, p_length, p_compressed_pkt, p_ecode),
	rohc_entity_t	*p_entity	_AND_
	rohc_dsc_rtp_t	*p_dsc		_AND_
	rohc_U32bit_t	*p_length	_AND_
	rohc_U8bit_t	*p_compressed_pkt	_AND_
	rohc_error_code_t	*p_ecode
)
{
	rohc_return_t	ret = ROHC_SUCCESS;
	rohc_ah_t	    *p_ah = ROHC_NULL;
	rohc_gre_t	    *p_gre = ROHC_NULL;
	rohc_dpib_t	*p_dpib = ROHC_DSC_GET_DPIB(p_dsc);

	ROHC_ENTER_FN("rohc_dled_p1_r_inneripext_list_dec");

	ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                      " Decoding Inner IP Extension List \n", 
                      p_entity->rohc_entity_id.ueIndex,
                      p_entity->rohc_entity_id.lcId, \
                      ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

	ret = rohc_d_ext_hdr_list_dec_p1(p_entity, p_dsc, p_compressed_pkt, 
                p_length, &(p_dpib->inneripext_new_item_count), 
                &(p_dpib->inneripextlist), p_dpib->inneripext_tt, 
                p_dpib->inneripext_new_index_list, 
                (rohc_U8bit_t *)&(p_dpib->inneripext_r_sw), 
                &p_ah,&p_gre, p_ecode);

	if(p_ah != ROHC_NULL)
		p_dpib->inner_ah_ptr = (rohc_U8bit_t *)p_ah;
	if(p_gre != ROHC_NULL)
		p_dpib->inner_gre = (rohc_U8bit_t *)p_gre;

	ROHC_EXIT_FN("rohc_dled_p1_r_inneripext_list_dec");
	return ret;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_dled_p2_r_inneripext_list_dec
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to de-compress INNER IP EXT HDR LIST in R Mode
**      ARGUMENTS:
**          p_dsc - Pointer to De-compressor context, 
**			p_entity - Pointer to Rohc entity context,
**			p_length - Pointer to length,
**			p_compressed_pkt - Pointer to compressed packet, 
**			p_ecode - Pointer to Error Code.
**      RETURN VALUE:
**          rohc_return_t
**      NOTES:
*****************************************************************************/
rohc_return_t   rohc_dled_p2_r_inneripext_list_dec     ROHC_PARAMS
(
    (p_entity, p_dsc, p_length, p_compressed_pkt, p_ecode),
    rohc_entity_t   *p_entity   _AND_
    rohc_dsc_udp_t  *p_dsc      _AND_
    rohc_U32bit_t   *p_length   _AND_
    rohc_U8bit_t    *p_compressed_pkt   _AND_
    rohc_error_code_t   *p_ecode
)
{
    rohc_return_t   ret = ROHC_SUCCESS;
    rohc_ah_t   *p_ah = ROHC_NULL;
    rohc_gre_t  *p_gre = ROHC_NULL;
    rohc_dpib_t *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);

    ROHC_ENTER_FN("rohc_dled_p2_r_inneripext_list_dec");
    ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
        " Decoding Inner IP Extension List \n", 
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

    ret = rohc_d_ext_hdr_list_dec_p2(p_entity, p_dsc, p_compressed_pkt, p_length,
            &(p_dpib->inneripext_new_item_count), &(p_dpib->inneripextlist),
            p_dpib->inneripext_tt, p_dpib->inneripext_new_index_list,
            (rohc_U8bit_t *)&(p_dpib->inneripext_r_sw), &p_ah,&p_gre, p_ecode);

    if(p_ah != ROHC_NULL)
        p_dpib->inner_ah_ptr = (rohc_U8bit_t *)p_ah;
    if(p_gre != ROHC_NULL)
        p_dpib->inner_gre = (rohc_U8bit_t *)p_gre;

    ROHC_EXIT_FN("rohc_dled_p2_r_inneripext_list_dec");
    return ret;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_dled_p1_uo_outeripext_list_dec
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to decode OUTER IP EXT HDR LIST in U/O Mode
**      ARGUMENTS:
**          p_dsc - Pointer to De-compressor context, 
**			p_entity - Pointer to Rohc entity context,
**			p_length - Pointer to length,
**			p_compressed_pkt - Pointer to compressed packet, 
**			p_ecode - Pointer to Error Code.
**      RETURN VALUE:
**          rohc_return_t
**      NOTES:
*****************************************************************************/
rohc_return_t	rohc_dled_p1_uo_outeripext_list_dec 	ROHC_PARAMS
(
	(p_entity, p_dsc, p_length, p_compressed_pkt, p_ecode),
	rohc_entity_t	*p_entity	_AND_
	rohc_dsc_rtp_t  *p_dsc		_AND_
	rohc_U32bit_t	*p_length	_AND_
	rohc_U8bit_t	*p_compressed_pkt	_AND_
	rohc_error_code_t	*p_ecode
)
{
	rohc_return_t	ret = ROHC_SUCCESS;
	rohc_ah_t	    *p_ah = ROHC_NULL;
	rohc_gre_t	    *p_gre = ROHC_NULL;
	rohc_dpib_t	*p_dpib = ROHC_DSC_GET_DPIB(p_dsc);

	ROHC_ENTER_FN("rohc_dled_p1_uo_outeripext_list_dec");

	ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                      " Decoding Outer IP Extension List \n", 
                      p_entity->rohc_entity_id.ueIndex,
                      p_entity->rohc_entity_id.lcId, \
                      ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

	rohc_led_p1_init_outeripext(p_dpib);
	ret = rohc_d_ext_hdr_list_dec_p1(p_entity, p_dsc, p_compressed_pkt, 
                p_length, &(p_dpib->outeripext_new_item_count), 
                &(p_dpib->outeripextlist), p_dpib->outeripext_tt, 
                p_dpib->outeripext_new_index_list, 
			    (rohc_U8bit_t *)&(p_dpib->outeripext_uo_sw), 
                &p_ah,&p_gre, p_ecode);
	
	if(p_ah != ROHC_NULL)
		p_dpib->outer_ah_ptr = (rohc_U8bit_t *)p_ah;
	if(p_gre != ROHC_NULL)
		p_dpib->outer_gre = (rohc_U8bit_t *)p_gre;

	ROHC_EXIT_FN("rohc_dled_p1_uo_outeripext_list_dec");
	return ret;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_dled_p2_uo_outeripext_list_dec
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to decode OUTER IP EXT HDR LIST in U/O Mode
**      ARGUMENTS:
**          p_dsc - Pointer to De-compressor context, 
**			p_entity - Pointer to Rohc entity context,
**			p_length - Pointer to length,
**			p_compressed_pkt - Pointer to compressed packet, 
**			p_ecode - Pointer to Error Code.
**      RETURN VALUE:
**          rohc_return_t
**      NOTES:
*****************************************************************************/
rohc_return_t	rohc_dled_p2_uo_outeripext_list_dec 	ROHC_PARAMS
(
	(p_entity, p_dsc, p_length, p_compressed_pkt, p_ecode),
	rohc_entity_t	*p_entity	_AND_
	rohc_dsc_udp_t		*p_dsc		_AND_
	rohc_U32bit_t	*p_length	_AND_
	rohc_U8bit_t	*p_compressed_pkt	_AND_
	rohc_error_code_t	*p_ecode
)
{
	rohc_return_t	ret = ROHC_SUCCESS;
	rohc_ah_t	*p_ah = ROHC_NULL;
	rohc_gre_t	*p_gre = ROHC_NULL;
	rohc_dpib_t	*p_dpib = ROHC_DSC_GET_DPIB(p_dsc);

	ROHC_ENTER_FN("rohc_dled_p2_uo_outeripext_list_dec");

	ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
        " Decoding Outer IP Extension List \n", 
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

	rohc_led_p2_init_outeripext(p_dpib);
	ret = rohc_d_ext_hdr_list_dec_p2(p_entity, p_dsc, p_compressed_pkt,p_length, 
			&(p_dpib->outeripext_new_item_count), &(p_dpib->outeripextlist), 
			p_dpib->outeripext_tt, p_dpib->outeripext_new_index_list, 
			(rohc_U8bit_t *)&(p_dpib->outeripext_uo_sw), &p_ah,&p_gre, p_ecode);
	
	if(p_ah != ROHC_NULL)
		p_dpib->outer_ah_ptr = (rohc_U8bit_t *)p_ah;
	if(p_gre != ROHC_NULL)
		p_dpib->outer_gre = (rohc_U8bit_t *)p_gre;

	ROHC_EXIT_FN("rohc_dled_p2_uo_outeripext_list_dec");
	return ret;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_dled_p1_r_outeripext_list_dec
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to de-compress OUTER IP EXT HDR LIST in R Mode
**      ARGUMENTS:
**          p_dsc - Pointer to De-compressor context, 
**			p_entity - Pointer to Rohc entity context,
**			p_length - Pointer to length,
**			p_compressed_pkt - Pointer to compressed packet, 
**			p_ecode - Pointer to Error Code.
**      RETURN VALUE:
**          rohc_return_t
**      NOTES:
*****************************************************************************/
rohc_return_t	rohc_dled_p1_r_outeripext_list_dec 	ROHC_PARAMS
(
	(p_entity, p_dsc, p_length, p_compressed_pkt, p_ecode),
	rohc_entity_t	*p_entity	_AND_
	rohc_dsc_rtp_t	*p_dsc		_AND_
	rohc_U32bit_t	*p_length	_AND_
	rohc_U8bit_t	*p_compressed_pkt	_AND_
	rohc_error_code_t	*p_ecode
)
{
	rohc_return_t	ret = ROHC_SUCCESS;
	rohc_ah_t	    *p_ah = ROHC_NULL;
	rohc_gre_t	    *p_gre = ROHC_NULL;
	rohc_dpib_t	*p_dpib = ROHC_DSC_GET_DPIB(p_dsc);

	ROHC_ENTER_FN("rohc_dled_p1_r_outeripext_list_dec");

	ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
                      " Decoding Outer IP Extension List \n", 
                      p_entity->rohc_entity_id.ueIndex,
                      p_entity->rohc_entity_id.lcId, \
                      ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

	ret = rohc_d_ext_hdr_list_dec_p1(p_entity, p_dsc, p_compressed_pkt, 
                p_length, &(p_dpib->outeripext_new_item_count), 
                &(p_dpib->outeripextlist), p_dpib->outeripext_tt, 
                p_dpib->outeripext_new_index_list, 
			    (rohc_U8bit_t *)&(p_dpib->outeripext_r_sw), 
                &p_ah,&p_gre, p_ecode);

	if(p_ah != ROHC_NULL)
		p_dpib->outer_ah_ptr = (rohc_U8bit_t *)p_ah;
	if(p_gre != ROHC_NULL)
		p_dpib->outer_gre = (rohc_U8bit_t *)p_gre;

	ROHC_EXIT_FN("rohc_dled_p1_r_outeripext_list_dec");
	return ret;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_dled_p2_r_outeripext_list_dec
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to de-compress OUTER IP EXT HDR LIST in R Mode
**      ARGUMENTS:
**          p_dsc - Pointer to De-compressor context, 
**			p_entity - Pointer to Rohc entity context,
**			p_length - Pointer to length,
**			p_compressed_pkt - Pointer to compressed packet, 
**			p_ecode - Pointer to Error Code.
**      RETURN VALUE:
**          rohc_return_t
**      NOTES:
*****************************************************************************/
rohc_return_t	rohc_dled_p2_r_outeripext_list_dec 	ROHC_PARAMS
(
	(p_entity, p_dsc, p_length, p_compressed_pkt, p_ecode),
	rohc_entity_t	*p_entity	_AND_
	rohc_dsc_udp_t		*p_dsc		_AND_
	rohc_U32bit_t	*p_length	_AND_
	rohc_U8bit_t	*p_compressed_pkt	_AND_
	rohc_error_code_t	*p_ecode
)
{
	rohc_return_t	ret = ROHC_SUCCESS;
	rohc_ah_t	*p_ah = ROHC_NULL;
	rohc_gre_t	*p_gre = ROHC_NULL;
	rohc_dpib_t	*p_dpib = ROHC_DSC_GET_DPIB(p_dsc);

	ROHC_ENTER_FN("rohc_dled_p2_r_outeripext_list_dec");

	ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
        " Decoding Outer IP Extension List \n", 
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

	ret = rohc_d_ext_hdr_list_dec_p2(p_entity, p_dsc, p_compressed_pkt,p_length, 
			&(p_dpib->outeripext_new_item_count), &(p_dpib->outeripextlist), 
			p_dpib->outeripext_tt, p_dpib->outeripext_new_index_list, 
			(rohc_U8bit_t *)&(p_dpib->outeripext_r_sw), &p_ah,&p_gre, p_ecode);

	if(p_ah != ROHC_NULL)
		p_dpib->outer_ah_ptr = (rohc_U8bit_t *)p_ah;
	if(p_gre != ROHC_NULL)
		p_dpib->outer_gre = (rohc_U8bit_t *)p_gre;

	ROHC_EXIT_FN("rohc_dled_p2_r_outeripext_list_dec");
	return ret;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_d_ext_hdr_list_dec_p1
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to de-compress  EXT HDR LIST
**      ARGUMENTS:
**          p_dsc - The compressor context, 
**			p_entity - rohc entity context,
**			p_compressed_pkt - Pointer to compressed Packet, 
**			p_length - Pointer to Length,
**			p_new_item_count - pointer to new item count,
**			p_ext_hdr_list - Pointer to EXT Header List,
**			p_ext_hdr_tt - Pointer to EXT Header Transition Table,
**			p_new_index_list - Pointer to new index list,
**			p_ext_hdr_sw - Pointer to EXt Header slede window,
**			p_p_ah ,
**			p_p_gre,
**			p_ecode - Pointer to Error Code
**      RETURN VALUE:
**          rohc_return_t
**      NOTES:
*****************************************************************************/
rohc_return_t   rohc_d_ext_hdr_list_dec_p1     ROHC_PARAMS
(
    (p_entity, p_dsc, p_compressed_pkt, p_length, p_new_item_count, 
     p_ext_hdr_list, p_ext_hdr_tt, p_new_index_list, p_ext_hdr_sw, 
     p_p_ah, p_p_gre, p_ecode),

    rohc_entity_t	        *p_entity	    _AND_
    rohc_dsc_rtp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U8bit_t                *p_ext_hdr_sw       _AND_
    rohc_ah_t		        **p_p_ah	    _AND_
    rohc_gre_t		        **p_p_gre	    _AND_
    rohc_error_code_t	        *p_ecode
)
{
    rohc_return_t   ret = ROHC_SUCCESS;
	rohc_U8bit_t	enc_type = 0, *p_start = ROHC_NULL;
	rohc_gre_t		*p_gre;
	rohc_dpib_t	*p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
	
	ROHC_ENTER_FN("rohc_d_ext_hdr_list_dec_p1");
	p_start = p_compressed_pkt;
	p_compressed_pkt++;
	(*p_length)++;

	if(ROHC_GET_BIT(*p_start, ROHC_ASEQ_BIT))
	{	/* AH seq no exists */
		if(ROHC_GET_BIT(*p_compressed_pkt, 8))
		{	/* AH Seq no is 32 bit */
			p_dpib->decoded_ah_seq = ROHC_GET_4BYTES(p_compressed_pkt);
			p_compressed_pkt = p_compressed_pkt + 4;
			rohc_led_decode_sn(p_dsc->ah_seq, &(p_dpib->decoded_ah_seq), 4);
			*p_length += 4;
		}
		else
		{	/* AH seq no is 8 bit */
			p_dpib->decoded_ah_seq = *p_compressed_pkt;
			p_compressed_pkt++;
			rohc_led_decode_sn(p_dsc->ah_seq, &(p_dpib->decoded_ah_seq), 1);
			(*p_length)++;
		}
		ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                          " Decoded Sequence number of AH [%d] \n", 
                          p_entity->rohc_entity_id.ueIndex,
                          p_entity->rohc_entity_id.lcId, \
                          ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
		                  p_dpib->decoded_ah_seq);

	}
	if(ROHC_GET_BIT(*p_start, ROHC_GSEQ_BIT))
	{	/* GRE Seq no exists */
		if(ROHC_GET_BIT(*p_compressed_pkt, 8))
		{	/* GRE seq no is 32 bits */
			p_dpib->decoded_gre_seq = ROHC_GET_4BYTES(p_compressed_pkt);
			p_compressed_pkt = p_compressed_pkt + 4;
			rohc_led_decode_sn(p_dsc->gre_seq, &(p_dpib->decoded_gre_seq), 4);
			*p_length += 4;
		}
		else
		{	/* GRE seq no is 8 bits */
			p_dpib->decoded_gre_seq = *p_compressed_pkt;
			p_compressed_pkt++;
			rohc_led_decode_sn(p_dsc->gre_seq, &(p_dpib->decoded_gre_seq), 1);
			(*p_length)++;
		}
        ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                          " Decoded Sequence number of GRE [%d] \n", 
                          p_entity->rohc_entity_id.ueIndex,
                          p_entity->rohc_entity_id.lcId, \
                          ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                          p_dpib->decoded_gre_seq);

	}
	
	if(ROHC_GET_BIT(*p_start, ROHC_CL_BIT))
	{	/* Compressed list is present */
		ROHC_GET_NBITS(*p_compressed_pkt, 7, 2, enc_type);
		switch(enc_type)
		{
			case ENCODING_TYPE_0:
			{
        		ROHC_LOG(LOG_INFO,
				"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
				" Parsing Encoding Type 0 packet \n", \
				p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
        		ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

				ret = parse_p1_ext_hdr_enc_type_0_pkt(p_entity, p_dsc, 
					p_compressed_pkt, p_length,p_new_item_count,p_ext_hdr_list, 
					p_ext_hdr_tt, p_new_index_list, p_dpib->decoded_esp_seq, 
					p_ext_hdr_sw, p_p_ah, p_p_gre, p_ecode);

				ROHC_EXIT_FN("rohc_d_ext_hdr_list_dec_p1");
				return	ret;
			}
			break;
			case ENCODING_TYPE_1:
			{
        		ROHC_LOG(LOG_INFO,
				"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
				" Parsing Encoding Type 1 packet \n", \
				p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
        		ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

				ret = parse_p1_ext_hdr_enc_type_1_pkt(p_entity, p_dsc, 
					p_compressed_pkt, p_length, p_new_item_count,p_ext_hdr_list,
					p_ext_hdr_tt, p_new_index_list, p_dpib->decoded_esp_seq, 
					p_ext_hdr_sw, p_p_ah, p_p_gre, p_ecode);
			}
			break;
			case ENCODING_TYPE_2:
			{
        		ROHC_LOG(LOG_INFO,
				"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
				" Parsing Encoding Type 2 packet \n", \
				p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        		ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

				ret = parse_p1_ext_hdr_enc_type_2_pkt(p_entity, p_dsc, 
					p_compressed_pkt, p_length, p_ext_hdr_list, p_ext_hdr_tt, 
					p_new_index_list, p_dpib->decoded_esp_seq, p_ext_hdr_sw, 
					p_p_ah, p_p_gre, p_ecode);
				*p_new_item_count = 0;
			}
			break;
			case ENCODING_TYPE_3:
			{
        		ROHC_LOG(LOG_INFO,
				"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
				" Parsing Encoding Type 3 packet \n", \
				p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        		ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

				ret = parse_p1_ext_hdr_enc_type_3_pkt(p_entity, p_dsc, 
					p_compressed_pkt, p_length, p_new_item_count,p_ext_hdr_list,
					p_ext_hdr_tt, p_new_index_list, p_dpib->decoded_esp_seq, 
					p_ext_hdr_sw, p_p_ah, p_p_gre, p_ecode);
			}
			break;
		} /* end of switch */

		/* These steps needs to be done only when AH and GRE were compressed */
		if ((*p_p_ah != ROHC_NULL) && (p_dpib->decoded_ah_seq != 0))
		{
			((rohc_ah_t *)(*p_p_ah))->seq = p_dpib->decoded_ah_seq;
		}
	
		/* Fill the Seq number in GRE buffer */
		if ((*p_p_gre != ROHC_NULL) && (p_dpib->decoded_gre_seq != 0))
		{
			p_gre = (rohc_gre_t *)(*p_p_gre);
			p_gre->flags |= ROHC_GRE_SP;
                        /* SPR 4808 Fix Start */
			p_gre = p_gre + 1;
                        /* SPR 4808 Fix End */
			if(((rohc_gre_t *)(*p_p_gre))->flags & ROHC_GRE_CP)
			{
				/* Checksum present */
				p_gre =  p_gre + 4;
			}
			if(((rohc_gre_t *)(*p_p_gre))->flags & ROHC_GRE_KP)
			{
				/* Key Present */
				p_gre = p_gre + 4;
			}
			ROHC_SET_32BIT((rohc_U8bit_t *)p_gre, p_dpib->decoded_gre_seq);
		}
	}
	ROHC_EXIT_FN("rohc_d_ext_hdr_list_dec_p1");

	return ret;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_d_ext_hdr_list_dec_p2
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to de-compress  EXT HDR LIST
**      ARGUMENTS:
**          p_dsc - The compressor context, 
**			p_entity - rohc entity context,
**			p_compressed_pkt - Pointer to compressed Packet, 
**			p_length - Pointer to Length,
**			p_new_item_count - pointer to new item count,
**			p_ext_hdr_list - Pointer to EXT Header List,
**			p_ext_hdr_tt - Pointer to EXT Header Transition Table,
**			p_new_index_list - Pointer to new index list,
**			p_ext_hdr_sw - Pointer to EXt Header slede window,
**			p_p_ah ,
**			p_p_gre,
**			p_ecode - Pointer to Error Code
**      RETURN VALUE:
**          rohc_return_t
**      NOTES:
*****************************************************************************/
rohc_return_t   rohc_d_ext_hdr_list_dec_p2     ROHC_PARAMS
(
    (p_entity, p_dsc, p_compressed_pkt, p_length, p_new_item_count,
     p_ext_hdr_list, p_ext_hdr_tt, p_new_index_list, p_ext_hdr_sw,
     p_p_ah, p_p_gre, p_ecode),

    rohc_entity_t             *p_entity           _AND_
    rohc_dsc_udp_t            *p_dsc              _AND_
    rohc_U8bit_t              *p_compressed_pkt   _AND_
    rohc_U32bit_t             *p_length           _AND_
    rohc_U8bit_t              *p_new_item_count   _AND_
    rohc_list_t               *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t  *p_ext_hdr_tt       _AND_
    rohc_U8bit_t              *p_new_index_list   _AND_
    rohc_U8bit_t              *p_ext_hdr_sw       _AND_
    rohc_ah_t                 **p_p_ah            _AND_
    rohc_gre_t                **p_p_gre           _AND_
    rohc_error_code_t         *p_ecode
)
{
  rohc_return_t   ret = ROHC_SUCCESS;
    rohc_U8bit_t    enc_type = 0, *p_start = ROHC_NULL;
    rohc_gre_t      *p_gre;
    rohc_dpib_t     *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);

    ROHC_ENTER_FN("rohc_d_ext_hdr_list_dec_p2");
    p_start = p_compressed_pkt;
    p_compressed_pkt++;
    (*p_length)++;

    if(ROHC_GET_BIT(*p_start, ROHC_ASEQ_BIT))
    {   /* AH seq no exists */
        if(ROHC_GET_BIT(*p_compressed_pkt, 8))
        {   /* AH Seq no is 32 bit */
            p_dpib->decoded_ah_seq = ROHC_GET_4BYTES(p_compressed_pkt);
            p_compressed_pkt = p_compressed_pkt + 4;
            rohc_led_decode_sn(p_dsc->ah_seq, &(p_dpib->decoded_ah_seq), 4);
            *p_length += 4;
        }
        else
        {   /* AH seq no is 8 bit */
            p_dpib->decoded_ah_seq = *p_compressed_pkt;
            p_compressed_pkt++;
            rohc_led_decode_sn(p_dsc->ah_seq, &(p_dpib->decoded_ah_seq), 1);
            (*p_length)++;
        }
        ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        " Decoded Sequence number of AH [%d] \n", 
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
        p_dpib->decoded_ah_seq);

    }
    if(ROHC_GET_BIT(*p_start, ROHC_GSEQ_BIT))
    {   /* GRE Seq no exists */
        if(ROHC_GET_BIT(*p_compressed_pkt, 8))
        {   /* GRE seq no is 32 bits */
            p_dpib->decoded_gre_seq = ROHC_GET_4BYTES(p_compressed_pkt);
            p_compressed_pkt = p_compressed_pkt + 4;
            rohc_led_decode_sn(p_dsc->gre_seq, &(p_dpib->decoded_gre_seq), 4);
            *p_length += 4;
        }
        else
        {   /* GRE seq no is 8 bits */
            p_dpib->decoded_gre_seq = *p_compressed_pkt;
            p_compressed_pkt++;
            rohc_led_decode_sn(p_dsc->gre_seq, &(p_dpib->decoded_gre_seq), 1);
            (*p_length)++;
        }
        ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        " Decoded Sequence number of GRE [%d] \n", 
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
        p_dpib->decoded_gre_seq);

    }

    if(ROHC_GET_BIT(*p_start, ROHC_CL_BIT))
    {   /* Compressed list is present */
        ROHC_GET_NBITS(*p_compressed_pkt, 7, 2, enc_type);
        switch(enc_type)
        {
            case ENCODING_TYPE_0:
            {
                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                " Parsing Encoding Type 0 packet \n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,\
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

                ret = parse_p2_ext_hdr_enc_type_0_pkt(p_entity, p_dsc,
                    p_compressed_pkt, p_length,p_new_item_count,p_ext_hdr_list,
                    p_ext_hdr_tt, p_new_index_list, p_dpib->decoded_esp_seq,
                    p_ext_hdr_sw, p_p_ah, p_p_gre, p_ecode);

                ROHC_EXIT_FN("rohc_d_ext_hdr_list_dec_p2");
                return  ret;
            }
            break;
            case ENCODING_TYPE_1:
            {
                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                " Parsing Encoding Type 1 packet \n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,\
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

                ret = parse_p2_ext_hdr_enc_type_1_pkt(p_entity, p_dsc,
                    p_compressed_pkt, p_length, p_new_item_count,p_ext_hdr_list,
                    p_ext_hdr_tt, p_new_index_list, p_dpib->decoded_esp_seq,
                    p_ext_hdr_sw, p_p_ah, p_p_gre, p_ecode);
            }
            break;
            case ENCODING_TYPE_2:
            {
                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                " Parsing Encoding Type 2 packet \n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,\
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));
                ret = parse_p2_ext_hdr_enc_type_2_pkt(p_entity, p_dsc,
                    p_compressed_pkt, p_length, p_ext_hdr_list, p_ext_hdr_tt,
                    p_new_index_list, p_dpib->decoded_esp_seq, p_ext_hdr_sw,
                    p_p_ah, p_p_gre, p_ecode);
                *p_new_item_count = 0;
            }
            break;
            case ENCODING_TYPE_3:
            {
                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                " Parsing Encoding Type 3 packet \n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

                ret = parse_p2_ext_hdr_enc_type_3_pkt(p_entity, p_dsc,
                    p_compressed_pkt, p_length, p_new_item_count,p_ext_hdr_list,
                    p_ext_hdr_tt, p_new_index_list, p_dpib->decoded_esp_seq,
                    p_ext_hdr_sw, p_p_ah, p_p_gre, p_ecode);
            }
            break;
        } /* end of switch */

        /* These steps needs to be done only when AH and GRE were compressed */
        if ((*p_p_ah != ROHC_NULL) && (p_dpib->decoded_ah_seq != 0))
        {
            ((rohc_ah_t *)(*p_p_ah))->seq = p_dpib->decoded_ah_seq;
        }

        /* Fill the Seq number in GRE buffer */
        if ((*p_p_gre != ROHC_NULL) && (p_dpib->decoded_gre_seq != 0))
        {
            p_gre = (rohc_gre_t *)(*p_p_gre);
            p_gre->flags |= ROHC_GRE_SP;
            /* SPR 4808 Fix Start */
            p_gre = p_gre + 1;
            /* SPR 4808 Fix End */
            if(((rohc_gre_t *)(*p_p_gre))->flags & ROHC_GRE_CP)
            {
                /* Checksum present */
                p_gre =  p_gre + 4;
            }
            if(((rohc_gre_t *)(*p_p_gre))->flags & ROHC_GRE_KP)
            {
                /* Key Present */
                p_gre = p_gre + 4;
            }
            ROHC_SET_32BIT((rohc_U8bit_t *)p_gre, p_dpib->decoded_gre_seq);
        }
    }
    ROHC_EXIT_FN("rohc_d_ext_hdr_list_dec_p2");

    return ret;
}
