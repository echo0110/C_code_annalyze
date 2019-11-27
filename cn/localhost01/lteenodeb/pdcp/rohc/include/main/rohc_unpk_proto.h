/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_unpk_proto.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This is an automatically generated file.
 *                     Please do not make any modification to the file directly.
 *                     This file needs to be regenerated if the ROHC User i/f 
 *                     APIs are changed.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_unpk_proto.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:27:38  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/
#ifdef ROHC_FUNC_ENHANCEMENT

#ifndef _ROHC_UNPK_PROTO_H_
#define _ROHC_UNPK_PROTO_H_

#include "rohc_typ.h"
#include "rohc_com_def.h"

rohc_len_t rohc_unpk_rohc_static_ip_t_ip_union ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U32bit_t choice,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result));

rohc_len_t rohc_unpk_stream_pkt_info_t_profile_union ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U32bit_t choice,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result));

rohc_len_t rohc_unpk_S8bit ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_S16bit ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_S32bit ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_U8bit ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_U16bit ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_U32bit ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_rohc_ip4_static_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_rohc_ip6_static_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_rohc_static_ip_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_rohc_udp_static_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_rohc_rtp_static_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_profile_0_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_profile_1_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_profile_2_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_profile_3_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_stream_pkt_info_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_rohc_gsw_element_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_rohc_r_sw_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_rohc_uo_sw_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_r_csrc_tt_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_r_ext_hdr_tt_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_uo_csrc_tt_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_rohc_d_uo_sw_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

rohc_len_t rohc_unpk_rohc_decomp_csrc_tt_t ROHC_PROTO ((
	rohc_void_tp p_void_var,
	rohc_U8bit_t *p_buff,
	rohc_len_t max_buf_len,
	rohc_error_code_t *p_result ));

#endif /* _ROHC_UNPK_PROTO_H_ */
#endif
