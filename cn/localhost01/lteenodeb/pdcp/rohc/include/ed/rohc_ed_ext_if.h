/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_ed_ext_if.h,v 1.2.36.4 2010/11/09 04:49:19 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains encoding and decoding functions for
 *                     SN, IP-ID and TS.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_ed_ext_if.h,v $
 * Revision 1.2.36.4  2010/11/09 04:49:19  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.3  2010/10/13 04:25:13  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.2  2010/10/04 03:32:53  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2.36.1  2010/09/23 07:00:10  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 05:15:10  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_ED_EX_H_
#define _ROHC_ED_EX_H_

#include "rohc_typ.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"
#include "rohc_g_typ.h"
#include "rohc_com_def.h"

/* Funtion prototypes to be used by other layers */

/* Compressor Side */
/* + SPR 17439 */
rohc_return_t   rohc_encode_p1_sn      ROHC_PROTO
                ((
                    rohc_entity_t           *p_rohc_entity  ,
                    rohc_csc_rtp_t          *p_csc          ,
                    rohc_U32bit_t           sn              ,
                    rohc_encoded_value_t    *p_en_val       ,
                    rohc_error_code_t       *p_ecode
                ));

rohc_return_t   rohc_encode_p2_sn      ROHC_PROTO
                ((
                    rohc_entity_t           *p_rohc_entity  ,
                    rohc_csc_udp_t          *p_csc          ,
                    rohc_U32bit_t           sn              ,
                    rohc_encoded_value_t    *p_en_val       ,
                    rohc_error_code_t       *p_ecode
                ));
rohc_return_t   rohc_encode_inner_ip_id     ROHC_PROTO
                ((
                    rohc_entity_t           *p_rohc_entity  ,
                    rohc_csc_t              *p_csc          ,
                    rohc_U16bit_t           ip_id           ,
                    rohc_encoded_value_t    *p_en_val       ,
                    rohc_error_code_t       *p_ecode
                ));

rohc_return_t   rohc_encode_outer_ip_id     ROHC_PROTO
                ((
                    rohc_entity_t           *p_rohc_entity  ,
                    rohc_csc_t              *p_csc          ,
                    rohc_U16bit_t           ip_id           ,
                    rohc_encoded_value_t    *p_en_val       ,
                    rohc_error_code_t       *p_ecode
                ));

rohc_return_t   rohc_encode_p1_ts      ROHC_PROTO
                ((
                    rohc_entity_t           *p_rohc_entity  ,
                    rohc_csc_rtp_t          *p_csc          ,
                    rohc_U32bit_t           ts              ,
                    rohc_encoded_value_t    *p_en_val       ,
                    rohc_error_code_t       *p_ecode
                ));

/* Decompressor Side */
rohc_return_t   rohc_decode_p1_sn      ROHC_PROTO
                ((
                    rohc_entity_t           *p_rohc_entity  ,
                    rohc_dsc_rtp_t          *p_dsc          ,
                    rohc_encoded_value_t    *p_en_val       ,
                    rohc_U32bit_t           *p_dec_sn       ,
                    rohc_error_code_t       *p_ecode
                ));
                 
/* Decompressor Side */
rohc_return_t   rohc_decode_p2_sn      ROHC_PROTO
                ((
                    rohc_entity_t           *p_rohc_entity  ,
                    rohc_dsc_udp_t          *p_dsc          ,
                    rohc_encoded_value_t    *p_en_val       ,
                    rohc_U32bit_t           *p_dec_sn       ,
                    rohc_error_code_t       *p_ecode
                ));
/* - SPR 17439 */                 
rohc_return_t   rohc_decode_p6_src    
                (
                    rohc_entity_t           *p_rohc_entity,  
                    rohc_dsc_tcp_t          *p_dsc,          
                    rohc_encoded_value_t    *p_en_val,       
                    rohc_U16bit_t           *p_dec_src,       
                    rohc_error_code_t       *p_ecode
                );
                 
rohc_return_t   rohc_decode_p6_dst    
                (
                    rohc_entity_t           *p_rohc_entity,  
                    rohc_dsc_tcp_t          *p_dsc,          
                    rohc_encoded_value_t    *p_en_val,       
                    rohc_U16bit_t           *p_dec_dst,       
                    rohc_error_code_t       *p_ecode
                );
                 
rohc_return_t   rohcDecodeP6SN   
                (
                    rohc_entity_t           *p_rohc_entity,  
                    rohc_dsc_tcp_t          *p_dsc,          
                    rohc_encoded_value_t    *p_en_val,       
                    rohc_U32bit_t           *p_dec_sn,       
                    rohc_error_code_t       *p_ecode,
                    rohc_U32bit_t           interpVal
                );
                 
rohc_return_t rohcDecodeP6MSN
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U32bit_t           *p_dec_msn,
        rohc_error_code_t       *p_ecode
    );

rohc_return_t rohcDecodeP6Win
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U16bit_t           *p_dec_win,
        rohc_error_code_t       *p_ecode
    );

rohc_return_t rohcDecodeP6TTL
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U8bit_t           *p_dec_ttl,
        rohc_error_code_t       *p_ecode
    );

rohc_return_t rohcDecodeP6AckNum
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U32bit_t           *p_dec_ack_num,
        rohc_error_code_t       *p_ecode,
        rohc_U32bit_t           interpVal
    );

rohc_return_t rohcDecodeP6TS
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U32bit_t           *p_dec_ts,
        rohc_error_code_t       *p_ecode
    );
/* + SPR 17439 */
rohc_return_t   rohc_decode_p1_inner_ip_id     ROHC_PROTO
                ((
                    rohc_entity_t           *p_rohc_entity  ,
                    rohc_dsc_rtp_t          *p_dsc          ,
                    rohc_encoded_value_t    *p_en_val       ,
                    rohc_U16bit_t           *p_dec_ip_id    ,
                    rohc_error_code_t       *p_ecode
                ));

rohc_return_t   rohc_decode_p2_inner_ip_id     ROHC_PROTO
                ((
                    rohc_entity_t           *p_rohc_entity  ,
                    rohc_dsc_udp_t          *p_dsc          ,
                    rohc_encoded_value_t    *p_en_val       ,
                    rohc_U16bit_t           *p_dec_ip_id    ,
                    rohc_error_code_t       *p_ecode
                ));
/* - SPR 17439 */
rohc_return_t   rohcDecodeP6InnerIpId
                (
                    rohc_entity_t           *p_rohc_entity,
                    rohc_dsc_tcp_t          *p_dsc,
                    rohc_encoded_value_t    *p_en_val,
                    rohc_U16bit_t           *p_dec_ip_id,
                    rohc_error_code_t       *p_ecode
                );
/* + SPR 17439 */
rohc_return_t   rohc_decode_p1_outer_ip_id     ROHC_PROTO
                ((
                    rohc_entity_t           *p_rohc_entity  ,
                    rohc_dsc_rtp_t          *p_dsc          ,
                    rohc_encoded_value_t    *p_en_val       ,
                    rohc_U16bit_t           *p_dec_ip_id    ,
                    rohc_error_code_t       *p_ecode
                ));

rohc_return_t   rohc_decode_p2_outer_ip_id     ROHC_PROTO
                ((
                    rohc_entity_t           *p_rohc_entity  ,
                    rohc_dsc_udp_t          *p_dsc          ,
                    rohc_encoded_value_t    *p_en_val       ,
                    rohc_U16bit_t           *p_dec_ip_id    ,
                    rohc_error_code_t       *p_ecode
                ));
/* - SPR 17439 */
rohc_return_t   rohcDecodeP6OuterIpId
                (
                    rohc_entity_t           *p_rohc_entity,
                    rohc_dsc_tcp_t          *p_dsc,
                    rohc_encoded_value_t    *p_en_val,
                    rohc_U16bit_t           *p_dec_ip_id,
                    rohc_error_code_t       *p_ecode
                );
/* + SPR 17439 */
rohc_return_t   rohc_p6_decode_msn_in_feedback      ROHC_PROTO
                ((
                    rohc_entity_t           *p_rohc_entity  ,
                    rohc_csc_tcp_t          *p_csc          ,
                    rohc_U32bit_t           temp_msn        ,
                    rohc_U32bit_t           *p_dec_msn      ,
                    rohc_error_code_t       *p_ecode
                ));

rohc_return_t   rohc_decode_p1_ts      ROHC_PROTO
                ((
                    rohc_entity_t           *p_rohc_entity  ,
                    rohc_dsc_rtp_t          *p_dsc          ,
                    rohc_encoded_value_t    *p_en_val       ,
                    rohc_U32bit_t           *p_dec_ts       ,
                    rohc_error_code_t       *p_ecode        ,
                    /*Start SPR 5818*/
                    rohc_U32bit_t            sn_delta     
                    /*End SPR 5818*/
                ));

/* Other miscellaneous functions */
rohc_return_t   rohc_ed_p1_update_window       ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_rtp_t  *p_csc
    ));

rohc_return_t   rohc_ed_p2_update_window       ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_udp_t  *p_csc
    ));

rohc_return_t   rohc_ed_p6_update_window       ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_tcp_t  *p_csc
    ));
rohc_return_t   rohc_ed_p1_update_gsw_sn       ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_rtp_t  *p_csc          ,
        rohc_U32bit_t   sn              
    ));
rohc_return_t   rohc_ed_p2_update_gsw_sn       ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_udp_t  *p_csc          ,
        rohc_U32bit_t   sn
    ));

rohc_return_t   rohc_ed_p6_update_gsw_sn       ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_tcp_t  *p_csc          ,
        rohc_U32bit_t   sn
    ));


rohc_return_t   rohc_ed_p6_update_gsw_ack       ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_tcp_t  *p_csc          ,
        rohc_U32bit_t   ack
    ));

rohc_return_t   rohc_ed_p6_update_gsw_msn      ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_tcp_t  *p_csc          ,
        rohc_U32bit_t   msn
    ));

rohc_return_t   rohc_ed_update_gsw_inner_ip_id      ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_t      *p_csc          ,
        rohc_U16bit_t   inner_ip_id             
    ));

rohc_return_t   rohc_ed_update_gsw_outer_ip_id      ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_t      *p_csc          ,
        rohc_U16bit_t   outer_ip_id             
    ));

rohc_return_t   rohc_ed_update_gsw_inner_offset     ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_t      *p_csc          ,
        rohc_U16bit_t   inner_offset                
    ));

rohc_return_t   rohc_ed_update_gsw_outer_offset     ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_t      *p_csc          ,
        rohc_U16bit_t   outer_offset                
    ));

rohc_return_t   rohc_ed_p1_update_gsw_ts       ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_rtp_t  *p_csc          ,
        rohc_U32bit_t   ts
    ));

rohc_return_t   rohc_ed_update_gsw_pkt_arvl_time    ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity  ,
        rohc_csc_t      *p_csc          ,
        rohc_U32bit_t   pkt_arvl_time               
    ));

rohc_return_t   rohc_ack_handling_p1   ROHC_PROTO
    ((
        rohc_csc_rtp_t  *p_csc  ,
        rohc_U32bit_t   sn      ,
	rohc_U8bit_t 	ack_typ
    ));
rohc_return_t   rohc_ack_handling_p2   ROHC_PROTO
    ((
        rohc_csc_udp_t      *p_csc  ,
        rohc_U32bit_t       sn      ,
        rohc_U8bit_t        ack_typ
    ));

rohc_return_t   rohc_ack_handling_p6   ROHC_PROTO
    ((
        rohc_csc_tcp_t  *p_csc  ,
        rohc_U32bit_t   msn      ,
	rohc_U8bit_t 	ack_typ
    ));
rohc_return_t   rohc_ed_p1_update_gsw_pkt_typ ROHC_PROTO
        ((
            rohc_entity_t   *p_rohc_entity  ,
            rohc_csc_rtp_t  *p_csc          ,
            rohc_U32bit_t   pkt_typ
        ));

rohc_return_t   rohc_ed_p2_update_gsw_pkt_typ ROHC_PROTO
        ((
            rohc_entity_t   *p_rohc_entity  ,
            rohc_csc_udp_t  *p_csc          ,
            rohc_U32bit_t   pkt_typ
        ));

rohc_U32bit_t rohc_d_sdvalue_decode ROHC_PROTO
                ((const rohc_U8bit_t *data ,
                        rohc_U8bit_t *p_size));


rohc_U8bit_t rohc_c_encode_sdvl ROHC_PROTO
                ((rohc_U8bit_t  *dest , 
                  rohc_U32bit_t value));
rohc_U8bit_t rohc_c_calc_bytes_sdvl ROHC_PROTO
                ((rohc_U32bit_t value));
/* - SPR 17439 */
#endif /* _ROHC_ED_EX_H_ */
