/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_d_parse_rohc.h,v 1.2.36.3 2010/11/09 04:52:24 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : DeCompressor Packet Sentinel module Implementation
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_d_parse_rohc.h,v $
 * Revision 1.2.36.3  2010/11/09 04:52:24  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.2  2010/10/13 04:32:09  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.1  2010/10/04 03:26:26  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2  2009/05/28 05:34:03  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_D_PARSE_IP_H_
#define _ROHC_D_PARSE_IP_H_

#include "rohc_com_def.h"
#include "rohc_dcom_mcr.h"
#include "rohc_gcom_mcr.h"
#include "rohc_g_typ.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"
#include "rohc_protocol_header.h"
#include "rohc_ppm_ext_if.h"
#include "rohc_trace_mcr.h"
#include "rohc_def.h"
#include "rohc_typ.h"
#include "rohc_c_pbm.h"
#include "rohc_db_ext_if.h"


#define ROHC_GET_UPPER_NIBBLE(byte)	(((rohc_U8bit_t)(byte)&0xf0U)>>4U)
#define ROHC_GET_LOWER_NIBBLE(byte)	((rohc_U8bit_t)(byte)&0x0fU)
#define ROHC_GET_2MSB(byte)			(((rohc_U8bit_t)(byte)&0xc0U)>>6U)
#define ROHC_GET_3MSB(byte)			(((rohc_U8bit_t)(byte)&0xe0U)>>5U)
#define ROHC_GET_5MSB(byte)			(((rohc_U8bit_t)(byte)&0xf8U)>>3U)
#define ROHC_GET_7MSB(byte)			(((rohc_U8bit_t)(byte)&0xfeU)>>1U)
#define ROHC_GET_3LSB(byte)			((rohc_U8bit_t)(byte)&0x07U)

#define ROHC_IRDYN_RES_TYPE 0xf8U
#define ROHC_IR_RES_TYPE 0xfcU
#define ROHC_IR_CO_RES_TYPE 0xfaU
#define ROHC_IR_RES_OR_IR_CO_RES_TYPE 0xfeU

/* defines for internediate packets, using packet type determination function */
/*****************************************************************************/
#define ROHC_INTER_R0_PTYPE 		0U
#define ROHC_INTER_R0CRC_PTYPE 		1U
#define ROHC_INTER_UO0_PTYPE 		2U
#define ROHC_INTER_R1_PTYPE 		3U
#define ROHC_INTER_R1IDTS_PTYPE 	4U
#define ROHC_INTER_UO1_PTYPE 		5U
#define ROHC_INTER_UO1IDTS_PTYPE 	6U
#define ROHC_INTER_UOR2_PTYPE 		7U
#define ROHC_INTER_UOR2IDTS_PTYPE	8U

#define ROHC_INTER_MAX_PTYPES 	(ROHC_INTER_UOR2IDTS_PTYPE +1)

typedef rohc_U8bit_t (*rohc_funcptr_ptypes_t)
						ROHC_PROTO((
							rohc_U8bit_t	*p_pkt,
							rohc_U8bit_t	cid_len
						));
/*****************************************************************************/

rohc_U8bit_t rohc_sdvl_len
ROHC_PROTO((rohc_U8bit_t * val));

    /* + SPR 17439 */
rohc_return_t  rohc_d_decode_p1_value
	ROHC_PROTO((
	rohc_dsc_common_t	* p_dsc ,
	rohc_U8bit_t		    * p_rohc_pkt ,
	rohc_U8bit_t		    pkt_type ,
	rohc_U32bit_t		    *p_len ,
	struct rohc_entity_t    * p_entity ,
	rohc_error_code_t       *p_ecode));

rohc_return_t  rohc_d_decode_p2_value
	ROHC_PROTO((
	rohc_dsc_common_t	* p_dsc ,
	rohc_U8bit_t		    * p_rohc_pkt ,
	rohc_U8bit_t		    pkt_type ,
	rohc_U32bit_t		    *p_len ,
	struct rohc_entity_t    * p_entity ,
	rohc_error_code_t       *p_ecode));

rohc_return_t  rohc_d_DecodeP6Value
	ROHC_PROTO((
    rohc_dsc_common_t          	* p_dsc ,
	rohc_U8bit_t		    * p_rohc_pkt ,
	rohc_U8bit_t		    pkt_type ,
	rohc_U32bit_t		    *p_len ,
	struct rohc_entity_t    * p_entity ,
	rohc_error_code_t       *p_ecode));

    /* - SPR 17439 */
    /* +- SPR 17777 */
void
rohc_dec_ele_p1_ext3flagsp1
	ROHC_PROTO((
	rohc_U8bit_t    * * p_rohc_pkt ,
	rohc_U8bit_t	field_id ,
	rohc_dpib_t	* p_dpib));

void
rohc_dec_ele_p2_ext3flagsp1
	ROHC_PROTO((
	rohc_U8bit_t * * p_rohc_pkt ,
	rohc_U8bit_t field_id ,
	rohc_dpib_t  * p_dpib));

rohc_return_t
rohc_dec_ele_p1_inneripflagsp1
	ROHC_PROTO((
	rohc_U8bit_t        * * p_rohc_pkt ,
	rohc_U8bit_t	    field_id ,
	rohc_dsc_rtp_t	    * p_dsc ,
	rohc_entity_t       * p_entity,
	rohc_error_code_t   *p_ecode));

rohc_return_t
rohc_dec_ele_p2_inneripflagsp1
	ROHC_PROTO((
	rohc_U8bit_t        * * p_rohc_pkt ,
	rohc_U8bit_t	    field_id ,
	rohc_dsc_udp_t	    * p_dsc ,
	rohc_entity_t       * p_entity,
	rohc_error_code_t   *p_ecode));

rohc_return_t
rohc_dec_ele_p1_outeripflagsp1
	ROHC_PROTO((
	rohc_U8bit_t        * * p_rohc_pkt ,
	rohc_U8bit_t	    field_id ,
	rohc_dsc_rtp_t	    * p_dsc ,
	rohc_entity_t       * p_entity,
	rohc_error_code_t   *p_ecode)) ;

rohc_return_t
rohc_dec_ele_p2_outeripflagsp1
	ROHC_PROTO((
	rohc_U8bit_t * * p_rohc_pkt ,
	rohc_U8bit_t	field_id ,
	rohc_dsc_udp_t		* p_dsc ,
	rohc_entity_t   * p_entity,
	rohc_error_code_t	*p_ecode)) ;

rohc_return_t rohc_dec_ele_p1_flagsp1
	ROHC_PROTO((
	rohc_U8bit_t        * * p_rohc_pkt ,
	rohc_U8bit_t	    field_id ,
	rohc_dsc_rtp_t		* p_dsc ,
	rohc_entity_t       * p_entity,
	rohc_error_code_t	*p_ecode));

rohc_return_t rohcCalOptItemSize(
    rohc_U8bit_t    *p_sizeItems,
    rohc_dpib_t     *dpib,
    rohc_U8bit_t    xi_m ,
    rohc_U8bit_t    ps_flag,
    rohc_U8bit_t    padding_boundary);
/* +- SPR 17777 */

#endif /* _ROHC_D_PARSE_IP_H_ */
