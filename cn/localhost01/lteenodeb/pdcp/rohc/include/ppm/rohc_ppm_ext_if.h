/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_ppm_ext_if.h,v 1.3.36.4 2010/11/09 04:52:38 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : External functional interface for PPM
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_ppm_ext_if.h,v $
 * Revision 1.3.36.4  2010/11/09 04:52:38  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.36.3  2010/10/13 04:32:40  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.36.2  2010/10/04 03:27:10  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.3.36.1  2010/09/23 07:02:50  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.3  2009/05/28 05:34:03  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_PPM_EXT_IF_H_
#define _ROHC_PPM_EXT_IF_H_

#include "rohc_d_typ.h"

rohc_return_t
	rohc_c_parse_ip
	ROHC_PROTO((
	                rohc_U8bit_t                * p_pkt,
                        rohc_U32bit_t	            length,	
                        struct rohc_entity_t        * p_entity,
			struct rohc_csc_common_t    **p_p_csc,
                        rohc_U32bit_t               *p_header_length,
			rohc_error_code_t           *p_ecode));

rohc_return_t
	rohc_d_decode_pkt
	ROHC_PROTO((
		rohc_U8bit_t	     * p_pkt,
		rohc_U32bit_t	     length,
		struct rohc_entity_t * p_entity,
		rohc_dsc_common_t    **  p_p_dsc,
		rohc_error_code_t    *p_ecode));

rohc_return_t
	rohc_d_decode_p0_pkt
	ROHC_PROTO((
		rohc_U8bit_t	       * p_pkt,
		rohc_U32bit_t	       length,
		struct rohc_entity_t   * p_entity,
		rohc_dsc_uncomp_t      **  p_p_dsc,
		rohc_error_code_t      *p_ecode));

rohc_return_t
	rohc_d_decode_p1_pkt
	ROHC_PROTO((
		rohc_U8bit_t	     * p_pkt,
		rohc_U32bit_t	     length,
		struct rohc_entity_t * p_entity,
		rohc_dsc_rtp_t       **  p_p_dsc,
		rohc_error_code_t    *p_ecode));

rohc_return_t
	rohc_d_decode_p2_pkt
	ROHC_PROTO((
		rohc_U8bit_t	     * p_pkt,
		rohc_U32bit_t	     length,
		struct rohc_entity_t * p_entity,
		rohc_dsc_udp_t       **  p_p_dsc,
		rohc_error_code_t    *p_ecode));

void rohc_d_ppm_init
	ROHC_PROTO((void));
/* + SPR 17439 */
rohc_return_t rohc_dppm_decode_all_pkt_types ROHC_PROTO ((
                struct rohc_entity_t * p_entity     ,
                rohc_dsc_common_t    **  p_p_dsc    ,
                rohc_U8bit_t         * p_pkt        ,
                rohc_U8bit_t         * cid_len        ,
                rohc_U8bit_t         * add_cid_found  ,
                rohc_error_code_t    *p_ecode));
/* - SPR 17439 */
#endif /* _ROHC_PPM_EXT_IF_H_ */
