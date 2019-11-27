/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_ed_proto.h,v 1.2.36.3 2010/11/09 04:49:33 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains prototypes for encoding and
 *                     decoding module.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_ed_proto.h,v $
 * Revision 1.2.36.3  2010/11/09 04:49:33  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.2  2010/10/18 06:16:04  gur22059
 * Incorporated comments for new_val
 *
 * Revision 1.2.36.1  2010/09/10 09:51:44  gur22059
 * Register variables used to store context block
 * pointer and other frequently used variables.
 *
 * Revision 1.2  2009/05/28 05:15:10  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_ED_PROTO_H_
#define _ROHC_ED_PROTO_H_

#include "rohc_ed_ext_if.h"
/* + SPR 17439 */
rohc_U8bit_t rohc_compute_k 	ROHC_PROTO
	((
		rohc_U32bit_t 	v_ref		,
		rohc_U32bit_t 	new_v
	));

rohc_U8bit_t rohc_sn_g    ROHC_PROTO
  ((
    rohc_U32bit_t           v_ref ,
    rohc_S32bit_t           sn    ,
    register rohc_U8bit_t   profile
  ));
	
rohc_U8bit_t rohc_ip_id_g		ROHC_PROTO
	((
		rohc_U32bit_t 	v_ref	,
		rohc_S32bit_t 	offset_i
	));

rohc_U8bit_t rohc_ts_scaled_g		ROHC_PROTO
	((
		rohc_U32bit_t 	v_ref	,
		rohc_U32bit_t 	ts_scaled
	));

rohc_U8bit_t rohc_ts_p1_timer_based_g		ROHC_PROTO
	((
		rohc_U32bit_t 	v_ref	    ,
		rohc_S32bit_t 	ts_scaled   ,
		rohc_csc_rtp_t	*p_csc
	));

rohc_return_t rohc_is_in_intval   ROHC_PROTO
  ((
    rohc_U32bit_t           v_ref   ,
    rohc_S32bit_t           p       ,
    rohc_U8bit_t            k       ,
    register rohc_S32bit_t  new_val
  ));

rohc_void_t rohc_f		ROHC_PROTO
	((
		rohc_U32bit_t 	v_ref		,
		rohc_S32bit_t 	p		,
		rohc_U8bit_t 	k		,
		rohc_S32bit_t 	*p_min_v	,
		rohc_S32bit_t 	*p_max_v
	));

rohc_void_t	rohc_encode_p1_scaled_ts	ROHC_PROTO
	((
		rohc_entity_t 		*p_rohc_entity	,
		rohc_csc_rtp_t   	*p_csc		,
		rohc_U32bit_t 		ts		,
		rohc_encoded_value_t 	*p_en_val
	));

rohc_void_t	rohc_encode_p1_timer_based_ts 	ROHC_PROTO
	((
		rohc_entity_t   	*p_rohc_entity	,
		rohc_csc_rtp_t 		*p_csc		,
		rohc_U32bit_t 	    	ts		,
		rohc_encoded_value_t    *p_en_val
	));

rohc_void_t	rohc_decode_p1_scaled_ts	ROHC_PROTO
	((
		rohc_entity_t		*p_rohc_entity	,
		rohc_dsc_rtp_t		*p_dsc		,
		rohc_encoded_value_t 	*p_en_val	,
		rohc_U32bit_t 		*p_dec_ts       ,
                /*Start SPR 5818*/
                rohc_U32bit_t           sn_delta
                /*End SPR 5818*/
	));

rohc_void_t	rohc_decode_p1_timer_based_ts 	ROHC_PROTO
	((
		rohc_entity_t		*p_rohc_entity	,
		rohc_dsc_rtp_t		*p_dsc		,
		rohc_encoded_value_t 	*p_en_val	,
		rohc_U32bit_t 		*p_dec_ts
	));

/* Prof 6 */
rohc_return_t rohc_encode_p6_sn    ROHC_PROTO
  ((
    rohc_entity_t           *p_rohc_entity ,
    rohc_csc_tcp_t          *p_csc         ,
    rohc_U32bit_t           sn             ,
    rohc_encoded_value_t    *p_en_val      ,
    rohc_U8bit_t            lsb            ,
     rohc_U32bit_t           offset_param_
  ));
rohc_return_t rohc_encode_p6_scaled_sn    ROHC_PROTO
  ((
    
    rohc_U32bit_t           scaled_sn      ,
  rohc_encoded_value_t    *p_en_val      ,
  rohc_U8bit_t            lsb             ,
  rohc_U32bit_t           offset_param    ,
  rohc_U32bit_t           prev_sn         ,
  rohc_U16bit_t           stride
 ));
rohc_return_t rohc_encode_p6_ack    ROHC_PROTO
  ((
    rohc_entity_t           *p_rohc_entity ,
    rohc_csc_tcp_t          *p_csc         ,
    rohc_U32bit_t           ack            ,
    rohc_encoded_value_t    *p_en_val      ,
    rohc_U8bit_t            lsb            ,
    rohc_U32bit_t           offset_param_
  ));
rohc_return_t rohc_encode_p6_scaled_ack    ROHC_PROTO
  ((
    rohc_entity_t           *p_rohc_entity ,
    rohc_csc_tcp_t          *p_csc         ,
    rohc_U32bit_t           scaled_ack     ,
    rohc_encoded_value_t    *p_en_val      ,
    rohc_U8bit_t            lsb            ,
    rohc_U32bit_t           offset_param_
  ));
rohc_U8bit_t rohc_encode_p6_ts_opt    ROHC_PROTO
  ((
    rohc_entity_t           *p_rohc_entity ,
   
    rohc_U32bit_t           ts_val         ,
    rohc_encoded_value_t    *p_en_val      ,
    rohc_U32bit_t           v_ref_min      ,
    rohc_U32bit_t           v_ref_max      
  ));

rohc_return_t rohc_encode_p6_ttl  ROHC_PROTO
((
  rohc_U8bit_t            ttl             ,
  rohc_U8bit_t       ref_val         ,
  rohc_U8bit_t       lsb             ,
  rohc_U32bit_t      offset_param
 ));
/* - SPR 17439 */
rohc_void_t rohcDecodeP6SackBlock
(
   rohc_dsc_tcp_t          *p_dsc,
   rohc_encoded_value_t    *p_en_val,
   rohc_U32bit_t           *p_dec_ts,
   rohc_error_code_t       *p_ecode,
   rohc_U32bit_t           *pBaseVal
 );

#endif /* _ROHC_ED_PROTO_H_ */
