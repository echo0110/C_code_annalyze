/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_unpk_func.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains unpacking funtions of ROHC.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_unpk_func.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 03:26:54  gur19836
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
#ifdef ROHC_FUNC_ENHANCEMENT


#include "rohc_gcom_mcr.h"
#include "rohc_unpk_proto.h"
#include "rohc_main_proto.h"

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




rohc_len_t rohc_unpk_rohc_static_ip_t_ip_union ROHC_PARAMS (
	(p_void_var, choice, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U32bit_t choice _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_static_ip_t *p_var;

	p_var = (rohc_static_ip_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	switch (choice)
	{
	case ROHC_VERSION_IP4:
		len += rohc_unpk_rohc_ip4_static_t (&p_var->ip_union.ip4, p_buf+len, max_buf_len-len, p_result);
		if (*p_result == ROHC_UNPACK_ERROR)
			return len;
		break;

	case ROHC_VERSION_IP6:
		len += rohc_unpk_rohc_ip6_static_t (&p_var->ip_union.ip6, p_buf+len, max_buf_len-len, p_result);
		if (*p_result == ROHC_UNPACK_ERROR)
			return len;
		break;

	}

	return len;
}


rohc_len_t rohc_unpk_stream_pkt_info_t_profile_union ROHC_PARAMS (
	(p_void_var, choice, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U32bit_t choice _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	stream_pkt_info_t *p_var;

	p_var = (stream_pkt_info_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	switch (choice)
	{
	case ROHC_UNCOMP:
		len += rohc_unpk_profile_0_t (&p_var->profile_union.profile_0, p_buf+len, max_buf_len-len, p_result);
		if (*p_result == ROHC_UNPACK_ERROR)
			return len;
		break;

	case ROHC_IP_UDP_RTP:
		len += rohc_unpk_profile_1_t (&p_var->profile_union.profile_1, p_buf+len, max_buf_len-len, p_result);
		if (*p_result == ROHC_UNPACK_ERROR)
			return len;
		break;

	case ROHC_IP_UDP:
		len += rohc_unpk_profile_2_t (&p_var->profile_union.profile_2, p_buf+len, max_buf_len-len, p_result);
		if (*p_result == ROHC_UNPACK_ERROR)
			return len;
		break;

	case ROHC_IP_ESP:
		len += rohc_unpk_profile_3_t (&p_var->profile_union.profile_3, p_buf+len, max_buf_len-len, p_result);
		if (*p_result == ROHC_UNPACK_ERROR)
			return len;
		break;

	}

	return len;
}


rohc_len_t rohc_unpk_S8bit ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_S8bit_t *p_var;

	p_var = (rohc_S8bit_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	*p_var = p_buf[len++];

	return len;
}


rohc_len_t rohc_unpk_S16bit ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_S16bit_t *p_var;

	p_var = (rohc_S16bit_t *) p_void_var;
	*p_result = ROHC_NO_ERR;


	if ((len+2)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	*p_var = p_buf[len++];
	*p_var |= p_buf[len++] << 8;

	return len;
}


rohc_len_t rohc_unpk_S32bit ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_S32bit_t *p_var;

	p_var = (rohc_S32bit_t *) p_void_var;
	*p_result = ROHC_NO_ERR;


	if ((len+4)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	*p_var = p_buf[len++];
	*p_var |= p_buf[len++] << 8;
	*p_var |= p_buf[len++] << 16;
	*p_var |= p_buf[len++] << 24;

	return len;
}


rohc_len_t rohc_unpk_U8bit ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_U8bit_t *p_var;

	p_var = (rohc_U8bit_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	*p_var = p_buf[len++];

	return len;
}


rohc_len_t rohc_unpk_U16bit ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_U16bit_t *p_var;

	p_var = (rohc_U16bit_t *) p_void_var;
	*p_result = ROHC_NO_ERR;


	if ((len+2)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	*p_var = p_buf[len++];
	*p_var |= p_buf[len++] << 8;

	return len;
}


rohc_len_t rohc_unpk_U32bit ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_U32bit_t *p_var;

	p_var = (rohc_U32bit_t *) p_void_var;
	*p_result = ROHC_NO_ERR;


	if ((len+4)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	*p_var = p_buf[len++];
	*p_var |= p_buf[len++] << 8;
	*p_var |= p_buf[len++] << 16;
	*p_var |= p_buf[len++] << 24;

	return len;
}


rohc_len_t rohc_unpk_rohc_ip4_static_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_stream_ip4_static_t *p_var;

	p_var = (rohc_stream_ip4_static_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->ver = p_buf[len++];

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->proto = p_buf[len++];

	if ((len+4)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->src = ROHC_GET_4BYTES(p_buf+len);
	len += 4;
	
	if ((len+4)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->dst = ROHC_GET_4BYTES(p_buf+len);
	len += 4;

	return len;
}


rohc_len_t rohc_unpk_rohc_ip6_static_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_stream_ip6_static_t *p_var;

	p_var = (rohc_stream_ip6_static_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+4)>max_buf_len)
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->v_flowlabel_nh = ROHC_GET_4BYTES(p_buf + len);
	len += 4;
	
	if ((len+16)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->src[0] = ROHC_GET_4BYTES(p_buf+len);
	len += 4;
	p_var->src[1] = ROHC_GET_4BYTES(p_buf+len);
	len += 4;
	p_var->src[2] = ROHC_GET_4BYTES(p_buf+len);
	len += 4;
	p_var->src[3] = ROHC_GET_4BYTES(p_buf+len);
	len += 4;

	if ((len+16)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->dst[0] = ROHC_GET_4BYTES(p_buf+len);
	len += 4;
	p_var->dst[1] = ROHC_GET_4BYTES(p_buf+len);
	len += 4;
	p_var->dst[2] = ROHC_GET_4BYTES(p_buf+len);
	len += 4;
	p_var->dst[3] = ROHC_GET_4BYTES(p_buf+len);
	len += 4;
	
	return len;
}


rohc_len_t rohc_unpk_rohc_udp_static_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_stream_udp_static_t *p_var;

	p_var = (rohc_stream_udp_static_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+2)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->src = ROHC_GET_2BYTES(p_buf+len);
	len += 2;
	
	if ((len+2)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->dst = ROHC_GET_2BYTES(p_buf+len);
	len += 2;
	
	return len;
}


rohc_len_t rohc_unpk_rohc_esp_static_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_stream_esp_static_t *p_var;

	p_var = (rohc_stream_esp_static_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+4)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->spi = ROHC_GET_4BYTES(p_buf+len);
	len += 4;
	
	return len;
}


rohc_len_t rohc_unpk_rohc_rtp_static_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_stream_rtp_static_t *p_var;

	p_var = (rohc_stream_rtp_static_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+4)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->ssrc = ROHC_GET_4BYTES(p_buf + len);
	len += 4;

	return len;
}


rohc_len_t rohc_unpk_rohc_static_ip_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_static_ip_t *p_var;

	p_var = (rohc_static_ip_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->ip_version = p_buf[len++];

	len += rohc_unpk_rohc_static_ip_t_ip_union (p_var, p_var->ip_version, p_buf+len, max_buf_len-len, p_result);
	if (*p_result == ROHC_UNPACK_ERROR)
		return len;

	return len;
}


rohc_len_t rohc_unpk_profile_0_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;

	*p_result = ROHC_NO_ERR;

	return len;
}


rohc_len_t rohc_unpk_profile_1_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	profile_1_t *p_var;

	p_var = (profile_1_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->is_outer_present = p_buf[len++];

	len += rohc_unpk_rohc_static_ip_t (&p_var->inner_ip, p_buf+len, max_buf_len-len, p_result);
	if (*p_result == ROHC_UNPACK_ERROR)
		return len;
	if(p_var->is_outer_present)
	{
		len += rohc_unpk_rohc_static_ip_t (&p_var->outer_ip, p_buf+len, max_buf_len-len, p_result);
		if (*p_result == ROHC_UNPACK_ERROR)
			return len;
	}
	len += rohc_unpk_rohc_udp_static_t (&p_var->udp, p_buf+len, max_buf_len-len, p_result);
	if (*p_result == ROHC_UNPACK_ERROR)
		return len;
	len += rohc_unpk_rohc_rtp_static_t (&p_var->rtp, p_buf+len, max_buf_len-len, p_result);
	if (*p_result == ROHC_UNPACK_ERROR)
		return len;

	return len;
}


rohc_len_t rohc_unpk_profile_2_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	profile_2_t *p_var;

	p_var = (profile_2_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->is_outer_present = p_buf[len++];

	len += rohc_unpk_rohc_static_ip_t (&p_var->inner_ip, p_buf+len, max_buf_len-len, p_result);
	if (*p_result == ROHC_UNPACK_ERROR)
		return len;
	if(p_var->is_outer_present)
	{
		len += rohc_unpk_rohc_static_ip_t (&p_var->outer_ip, p_buf+len, max_buf_len-len, p_result);
		if (*p_result == ROHC_UNPACK_ERROR)
			return len;
	}
	len += rohc_unpk_rohc_udp_static_t (&p_var->udp, p_buf+len, max_buf_len-len, p_result);
	if (*p_result == ROHC_UNPACK_ERROR)
		return len;

	return len;
}


rohc_len_t rohc_unpk_profile_3_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	profile_3_t *p_var;

	p_var = (profile_3_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->is_outer_present = p_buf[len++];

	len += rohc_unpk_rohc_static_ip_t (&p_var->inner_ip, p_buf+len, max_buf_len-len, p_result);
	if (*p_result == ROHC_UNPACK_ERROR)
		return len;
	if(p_var->is_outer_present)
	{
		len += rohc_unpk_rohc_static_ip_t (&p_var->outer_ip, p_buf+len, max_buf_len-len, p_result);
		if (*p_result == ROHC_UNPACK_ERROR)
			return len;
	}
	len += rohc_unpk_rohc_esp_static_t (&p_var->esp, p_buf+len, max_buf_len-len, p_result);
	if (*p_result == ROHC_UNPACK_ERROR)
		return len;

	return len;
}


rohc_len_t rohc_unpk_stream_pkt_info_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	stream_pkt_info_t *p_var;

	p_var = (stream_pkt_info_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->cid = p_buf[len++];

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->info_type = p_buf[len++];

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->profile = p_buf[len++];

	len += rohc_unpk_stream_pkt_info_t_profile_union (p_var, p_var->profile, p_buf+len, max_buf_len-len, p_result);
	if (*p_result == ROHC_UNPACK_ERROR)
		return len;

	return len;
}

rohc_len_t rohc_unpk_rohc_gsw_element_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_gsw_element_t *p_var;

	p_var = (rohc_gsw_element_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+2)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->inner_ip_id = ROHC_GET_2BYTES(p_buf+len);
	len += 2;

	if ((len+2)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->inner_offset_i = ROHC_GET_2BYTES(p_buf+len);
	len += 2;
	
	if ((len+2)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->outer_ip_id = ROHC_GET_2BYTES(p_buf+len);
	len += 2;

	if ((len+2)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->outer_offset_i = ROHC_GET_2BYTES(p_buf+len);
	len += 2;
	
	if ((len+4)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->sn = ROHC_GET_4BYTES(p_buf+len);
	len += 4;

	if ((len+4)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->ts_scaled = ROHC_GET_4BYTES(p_buf+len);
	len += 4;

	if ((len+4)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->pkt_arvl_time = ROHC_GET_4BYTES(p_buf+len);
	len += 4;


	return len;
}


rohc_len_t rohc_unpk_rohc_r_sw_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_U32bit_t i = 0;
	rohc_len_t len = 0;
	rohc_r_sw_t *p_var;

	p_var = (rohc_r_sw_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+4)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->rtp_sn_no = ROHC_GET_2BYTES(p_buf+len);
	len += 2;
	if ((len+2)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->icount = ROHC_GET_2BYTES(p_buf + len);
	len += 2;

	for (i=0; i<p_var->icount; ++i)
	{
		if ((len+1)>max_buf_len) 
		{ 
			*p_result = ROHC_UNPACK_ERROR; 
			return len; 
		} 
		p_var->index_list[i] = p_buf[len++];
	}

	return len;
}


rohc_len_t rohc_unpk_rohc_uo_sw_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_U32bit_t i = 0;
	rohc_len_t len = 0;
	rohc_uo_sw_t *p_var;

	p_var = (rohc_uo_sw_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->gen_id = p_buf[len++];

	if ((len+2)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->icount = ROHC_GET_2BYTES(p_buf+len);
	len +=2;

	for (i=0; i<p_var->icount; ++i)
	{
		if ((len+1)>max_buf_len) 
		{ 
			*p_result = ROHC_UNPACK_ERROR; 
			return len; 
		} 
		p_var->index_list[i] = p_buf[len++];
	}

	return len;
}


rohc_len_t rohc_unpk_r_csrc_tt_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	r_csrc_tt_t *p_var;

	p_var = (r_csrc_tt_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->known = p_buf[len++];

	if ((len+4)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->item = ROHC_GET_4BYTES(p_buf + len);
	len += 4;

	return len;
}

rohc_len_t rohc_unpk_uo_csrc_tt_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	uo_csrc_tt_t *p_var;

	p_var = (uo_csrc_tt_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->known = p_buf[len++];

	if ((len+4)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->item = ROHC_GET_4BYTES(p_buf+len);
	len += 4;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->counter = p_buf[len++];

	return len;
}

rohc_len_t rohc_unpk_rohc_d_uo_sw_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_U32bit_t i = 0;
	rohc_len_t len = 0;
	rohc_d_uo_sw_t *p_var;

	p_var = (rohc_d_uo_sw_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->gen_id = p_buf[len++];

	if ((len+2)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->icount = ROHC_GET_2BYTES(p_buf+len);
	len += 2;

	for (i=0; i<p_var->icount; ++i)
	{
		if ((len+1)>max_buf_len) 
		{ 
			*p_result = ROHC_UNPACK_ERROR; 
			return len; 
		} 
		p_var->index_list[i] = p_buf[len++];
	}

	return len;
}


rohc_len_t rohc_unpk_rohc_decomp_csrc_tt_t ROHC_PARAMS (
	(p_void_var, p_buf, max_buf_len, p_result),
	rohc_void_tp p_void_var _AND_ 
	rohc_U8bit_t *p_buf _AND_ 
	rohc_len_t max_buf_len _AND_ 
	rohc_error_code_t *p_result )
{
	rohc_len_t len = 0;
	rohc_decomp_csrc_tt_t *p_var;

	p_var = (rohc_decomp_csrc_tt_t *) p_void_var;
	*p_result = ROHC_NO_ERR;

	if ((len+1)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->known = p_buf[len++];

	if ((len+4)>max_buf_len) 
	{ 
		*p_result = ROHC_UNPACK_ERROR; 
		return len; 
	} 
	p_var->item = ROHC_GET_4BYTES(p_buf+len);
	len += 4;

	return len;
}
#endif
