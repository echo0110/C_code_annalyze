/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dled_csrc.c,v 1.3.36.1 2010/11/09 05:18:42 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the functions for list compression
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dled_csrc.c,v $
 * Revision 1.3.36.1  2010/11/09 05:18:42  gur22059
 * Segregation of code w.r.t profile 1
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
#include "rohc_dcom_mcr.h"
#include "rohc_g_typ.h"
#include "rohc_gcom_mcr.h"
#include "rohc_led_proto.h"
#include "rohc_led_def.h"
#include "rohc_led_fn_tbl.h"
#include "rohc_trace_mcr.h"

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
**    FUNCTION NAME
**        rohc_dled_p1_csrc_list_dec
**
*******************************************************************
**
**    DESCRIPTION
**			This function is used to decode CSRC List in R-Mode
**
**    INPUTS
**         The de-compressor context and the field id which needs to be
**         compressed is taken as input.
**
**    RETURNS
**        Length of the de-compressed packet
**
******************************************************************/

rohc_return_t	rohc_dled_p1_csrc_list_dec	ROHC_PARAMS
(
	(p_entity, p_dsc, p_length, p_compressed_pkt, p_ecode),
	rohc_entity_t	*p_entity	_AND_
	rohc_dsc_rtp_t	*p_dsc		_AND_
	rohc_U32bit_t	*p_length	_AND_
	rohc_U8bit_t	*p_compressed_pkt	_AND_
	rohc_error_code_t	*p_ecode
)
{
	rohc_U8bit_t	enc_type;

	ROHC_ENTER_FN("rohc_dled_p1_csrc_list_dec");

	rohc_led_p1_init_csrc(ROHC_DSC_GET_DPIB(p_dsc));


	ROHC_GET_NBITS(*p_compressed_pkt, 7, 2, enc_type);

	if(enc_type == ENCODING_TYPE_0)
	{
    	ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    	"Decoding CSRC list received in Encoding Type 0 packet", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    	ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

        /* +- SPR 17777 */
		if(ROHC_FAILURE == parse_p1_csrc_enc_type_0_pkt(p_entity, p_dsc, 
                                p_compressed_pkt, p_length))
		{
			ROHC_EXIT_FN("rohc_dled_p1_csrc_list_dec");
			return ROHC_FAILURE;
		}
	}
	else if(enc_type == ENCODING_TYPE_1)
	{
    	ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    	"Decoding CSRC list received in Encoding Type 1 packet", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    	ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

        /* +- SPR 17777 */
		if(ROHC_FAILURE == parse_p1_csrc_enc_type_1_pkt(p_entity, p_dsc, 
                                p_compressed_pkt, p_length))
		{
			ROHC_EXIT_FN("rohc_dled_p1_csrc_list_dec");
			return ROHC_FAILURE;
		}
	}	
	else if(enc_type == ENCODING_TYPE_2)
	{
    	ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    	"Decoding CSRC list received in Encoding Type 2 packet", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    	ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

        /* +- SPR 17777 */
		if(ROHC_FAILURE == parse_p1_csrc_enc_type_2_pkt(p_dsc, 
                                p_compressed_pkt, p_length, p_ecode))
		{
			ROHC_EXIT_FN("rohc_dled_p1_csrc_list_dec");
			return ROHC_FAILURE;
		}
	}
	else if(enc_type == ENCODING_TYPE_3)
	{
    	ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    	"Decoding CSRC list received in Encoding Type 3 packet", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    	ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

		if(ROHC_FAILURE == parse_p1_csrc_enc_type_3_pkt(p_entity, p_dsc, 
                                p_compressed_pkt, p_length, p_ecode))
		{
			ROHC_EXIT_FN("rohc_dled_p1_csrc_list_dec");
			return ROHC_FAILURE;
		}
	}
	else
	{
    	ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
		" Not a valid encoding type scheme for CSRC list\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    	ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc));

		ROHC_EXIT_FN("rohc_dled_p1_csrc_list_dec");
		return ROHC_FAILURE;
	}

	ROHC_EXIT_FN("rohc_dled_p1_csrc_list_dec");
	return ROHC_SUCCESS;
}
