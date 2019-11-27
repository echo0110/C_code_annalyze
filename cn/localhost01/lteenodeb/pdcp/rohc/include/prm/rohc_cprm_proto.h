/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_cprm_proto.h,v 1.2.28.6 2010/12/02 08:55:29 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains functions prototypes & #defines for
 *                     internal interface of moudle C-PRM.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_cprm_proto.h,v $
 * Revision 1.2.28.6  2010/12/02 08:55:29  gur22059
 * Conflicting profile 1 functions were renamed acc to new Naming Convention
 *
 * Revision 1.2.28.5  2010/11/09 04:53:37  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.28.4  2010/10/13 04:34:16  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.28.3  2010/09/23 07:03:46  gur22059
 * Reduction in the size of context block w.r.t profil
 *
 * Revision 1.2.28.2  2010/06/30 05:31:36  gur21145
 * profile2_enhancements
 *
 * Revision 1.2.28.1  2010/06/25 10:11:17  gur21145
 * PROFILE2_ENHANCEMENTS
 *
 * Revision 1.2  2009/05/28 05:46:18  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_CPRM_PROTO_H_
#define _ROHC_CPRM_PROTO_H_


#include "rohc_com_def.h"
#include "rohc_c_typ.h"
#include "rohc_cprm_def_p1.h"
#include "rohc_cprm_def_p23.h"
#include "rohc_cprm_def_p6.h"
#include "rohc_g_typ.h"
#include "rohc_gcom_mcr.h"
#include "rohc_typ.h"
#include "rohc_protocol_header.h"
#include "rohc_cprm_ext_if.h"
#include "rohc_trace_mcr.h"
#include "rohc_cprm_mcr.h"
#include "rohc_ccpm_event.h"
#include "rohc_ed_ext_if.h"
#include "rohc_crc_ext_if.h"


#define ROHC_FB_TYPE_MASK       0xf8        
#define ROHC_FB_PKT             0xf0          
#define ROHC_FB_SN_MASK         0x0f        
#define ROHC_FB_MODE_MASK       0x30      
#define ROHC_FB_ACKT_MASK       0xc0      
#define ROHC_FB_OPTYP_MASK      0xf0      
#define ROHC_FB_OPLEN_MASK      0x0f      
#define ROHC_ACK_ET             0xa1  
#define ROHC_NACK_ET            0xa2  
#define ROHC_STATIC_NACK_ET     0xa3  

/* prof 6 */
#define ROHC_FB_MSN_MASK        0x3f        


typedef struct rohc_p1_pkt_typ_info{   
    rohc_U8bit_t    sn_bits;
    rohc_U8bit_t    ts_bits;
    rohc_U8bit_t    ip_id_bits;
    rohc_U8bit_t    pbm_pkt_typ;
} rohc_p1_pkt_typ_info_t;


typedef struct rohc_p23_pkt_typ_info
{   
    rohc_U8bit_t    sn_bits;
    rohc_U8bit_t    in_ip_id_bits;
    rohc_U8bit_t    out_ip_id_bits;
    rohc_U8bit_t    pbm_pkt_typ;
}rohc_p23_pkt_typ_info_t;

/* + SPR 17439 */
rohc_U8bit_t rohc_cprm_resolve_pkt_typ_uncomp ROHC_PROTO((\
                rohc_entity_t   *p_entity ,\
                rohc_csc_uncomp_t      *p_csc));

/* - SPR 17439 */
rohc_void_t rohc_init_p1_uo_table ROHC_PROTO(ROHC_NO_PARAMS());

rohc_void_t rohc_init_p1_r_table ROHC_PROTO(ROHC_NO_PARAMS());

rohc_void_t rohc_init_p23_uo_table ROHC_PROTO(ROHC_NO_PARAMS());

rohc_void_t rohc_init_p23_r_table ROHC_PROTO(ROHC_NO_PARAMS());


/* + SPR 17439 */
/* +- SPR 17777 */
rohc_void_t rohc_cprm_p1_update_csc_rtp ROHC_PROTO((
            rohc_csc_rtp_t          *p_csc      ,
            rohc_encoded_value_t    *en_sn      ,
            rohc_encoded_value_t    *en_ts 
            ));
            
rohc_void_t rohc_cprm_p1_update_csc_udp(
            rohc_csc_rtp_t  *p_csc);

rohc_void_t rohc_cprm_p1_update_csc_tcp ROHC_PROTO((
            rohc_entity_t           *p_entity   ,   
            rohc_csc_rtp_t          *p_csc      ,
            rohc_encoded_value_t    *en_sn      ,
            rohc_encoded_value_t    *en_msn 
            ));

rohc_void_t rohc_cprm_p1_update_csc_inner_ip4 ROHC_PROTO((
            
            rohc_csc_rtp_t          *p_csc      ,
            rohc_encoded_value_t    *en_ip_id 
            ));
            
rohc_void_t rohc_cprm_p1_update_csc_inner_ip6 ROHC_PROTO((
            
            rohc_entity_t   *p_entity ,   
            rohc_csc_udp_t  *p_csc 
             
            ));
            
rohc_void_t rohc_cprm_p1_update_csc_outer_ip4(
            rohc_csc_rtp_t  *p_csc );
             
rohc_void_t rohc_cprm_p1_update_csc_outer_ip6(
            rohc_csc_rtp_t  *p_csc); 

rohc_void_t rohc_cprm_update_csc_p2_udp ROHC_PROTO((
            
            rohc_entity_t           *p_entity   ,   
            rohc_csc_udp_t          *p_csc      ,
            rohc_encoded_value_t    *encoded_sn 
            ));
                    
rohc_void_t rohc_cprm_update_csc_p2_inner_ip4 ROHC_PROTO((
            rohc_entity_t           *p_entity       ,   
            rohc_csc_udp_t          *p_csc          ,
            rohc_encoded_value_t*   encoded_ip_id 
            ));         
            
rohc_void_t rohc_cprm_update_csc_p2_inner_ip6 ROHC_PROTO((
            rohc_entity_t   *p_entity ,   
            rohc_csc_udp_t  *p_csc 
            ));

rohc_void_t rohc_cprm_update_csc_p2_outer_ip4 ROHC_PROTO((
            rohc_entity_t           *p_entity       ,   
            rohc_csc_udp_t          *p_csc          ,
            rohc_encoded_value_t*   encoded_ip_id 
            ));

rohc_void_t rohc_cprm_update_csc_p2_outer_ip6 ROHC_PROTO((
            rohc_entity_t   *p_entity ,   
            rohc_csc_udp_t  *p_csc 
            ));

rohc_void_t rohc_cprm_p6_update_csc_inner_ip4 ROHC_PROTO((
            
            rohc_csc_tcp_t          *p_csc      ,
            rohc_encoded_value_t    *en_ip_id 
            ));
/* - SPR 17439 */
rohc_void_t rohc_parse_tcp_options ROHC_PROTO((
            rohc_csc_tcp_t          *p_csc,
            rohc_U8bit_t            *p_list_present));

/* + SPR 17439 */
rohc_void_t rohc_c_parse_generic_option ROHC_PROTO((
            
            rohc_U8bit_t    *tcp_opt_list   ,   
            rohc_c_generic_opt_t *p_generic_opt ));

rohc_U32bit_t rohc_cprm_p1_compute_delta ROHC_PROTO((
    rohc_csc_rtp_t          *p_csc      ,
    rohc_encoded_value_t    *en_sn      ,
    rohc_encoded_value_t    *en_ip_id));

rohc_U8bit_t rohc_cprm_p1_resolve_pkt_typ_r ROHC_PROTO((
                    rohc_csc_rtp_t          *p_csc          ,
                    rohc_U32bit_t           delta_bit_map   ,
                    rohc_encoded_value_t    *en_sn          ,
                    rohc_encoded_value_t    *en_ts          ,
                    rohc_encoded_value_t    *en_ip_id       ,
                    rohc_U8bit_t            no_crc_bit));

/* - SPR 17439 */
rohc_U8bit_t rohc_cprm_p1_resolve_pkt_bmp_uo (
                    rohc_encoded_value_t    *en_sn,
                    rohc_encoded_value_t    *en_ts,
                    rohc_encoded_value_t    *en_ip_id,
                    rohc_U8bit_t            pkt_bit_map[]);

rohc_U8bit_t rohc_cprm_p1_resolve_pkt_bmp_r (
                    rohc_encoded_value_t    *en_sn,
                    rohc_encoded_value_t    *en_ts,
                    rohc_encoded_value_t    *en_ip_id,
                    rohc_U8bit_t            pkt_bit_map[]);

/* + SPR 17439 */
rohc_U8bit_t rohc_cprm_p1_resolve_pkt_typ_uo ROHC_PROTO((
                    rohc_csc_rtp_t          *p_csc          ,
                    rohc_U32bit_t           delta_bit_map   ,
                    rohc_encoded_value_t    *en_sn          ,
                    rohc_encoded_value_t    *en_ts          ,
                    rohc_encoded_value_t    *en_ip_id       ,
                    rohc_U8bit_t            no_crc_bit)); 

rohc_U8bit_t rohc_cprm_resolve_pkt_typ_p2_uo ROHC_PROTO((
                    rohc_csc_udp_t          *p_csc          ,
                    rohc_U32bit_t           delta_bit_map   ,               
                    rohc_encoded_value_t    *en_sn          ,
                    rohc_encoded_value_t    *en_ip_id       ,
                    rohc_encoded_value_t    *en_ip_id2      ,
                    rohc_U8bit_t            no_crc_bit));
                    
rohc_U8bit_t rohc_cprm_resolve_pkt_typ_p2_r ROHC_PROTO((
                    rohc_csc_udp_t          *p_csc          ,
                    rohc_U32bit_t           delta_bit_map   ,
                    rohc_encoded_value_t    *en_sn          ,
                    rohc_encoded_value_t    *en_ip_id       ,
                    rohc_encoded_value_t    *en_ip_id2      ,
                    rohc_U8bit_t            no_crc_bit));

rohc_U32bit_t rohc_cprm_compute_p2_delta ROHC_PROTO(
                            (
                             rohc_csc_udp_t         *p_csc      ,
                             rohc_encoded_value_t   *en_sn      ,
                             rohc_encoded_value_t   *en_ip_id   ,
                             rohc_encoded_value_t   *en_ip_id2));                                     
                    
rohc_U32bit_t rohc_cprm_p6_compute_delta (rohc_csc_tcp_t  *p_csc);

rohc_U8bit_t rohc_cprm_p6_pkt_det ROHC_PROTO(
                            (rohc_entity_t          *p_entity         ,
                             rohc_csc_tcp_t         *p_csc            ,
                             rohc_U32bit_t          delta_bit_map     ,
                             rohc_encoded_value_t   *en_sn            ,
                             rohc_encoded_value_t   *p_en_scaled_sn   ,
                             rohc_encoded_value_t   *p_en_ack         ,
                             rohc_encoded_value_t   *p_en_scaled_ack  ,
                             rohc_encoded_value_t   *en_ip_id         ,
                             rohc_U8bit_t           *p_pkt_typ));       

rohc_void_t rohc_cprm_p6_type_updt_encd_val ROHC_PROTO(
                            (rohc_entity_t          *p_entity         ,
                             rohc_csc_tcp_t         *p_csc            ,
                             rohc_encoded_value_t   *en_sn            ,
                             rohc_encoded_value_t   *p_en_scaled_sn   ,
                             rohc_encoded_value_t   *p_en_ack         ,
                             rohc_encoded_value_t   *p_en_scaled_ack  ,
                             rohc_encoded_value_t   *en_ip_id         ,
                             rohc_U8bit_t           *p_sel_pkt));       

rohc_return_t field_scaling_sn ROHC_PROTO(
                            (rohc_U32bit_t          unscaled_val      ,
                             rohc_U32bit_t          scaled_val        ,
                             rohc_U16bit_t          stride            ,
                             rohc_U32bit_t          residue           ,
                             rohc_U8bit_t           *sn_scaling_factor_estab_counter,
                             rohc_U8bit_t           sn_stride_change  ,
                             rohc_U8bit_t            sn_residue_change,
                             rohc_U32bit_t           old_tcp_sn       ,
                             rohc_encoded_value_t    *p_en_scaled_sn
                             ));

rohc_return_t field_scaling_ack ROHC_PROTO(
                            (rohc_U32bit_t          unscaled_val      ,
                             rohc_U32bit_t          scaled_val        ,
                             rohc_U16bit_t          stride            ,
                             rohc_U32bit_t          residue           ,
                             rohc_U8bit_t
                             *ack_scaling_factor_estab_counter,
                             rohc_U8bit_t            ack_stride_change,
                             rohc_U8bit_t            ack_residue_change));
/* - SPR 17439 */
rohc_void_t encode_sn_ack_co_common_pkt (
        rohc_entity_t           *p_entity,
        rohc_csc_tcp_t          *p_csc,
        rohc_U32bit_t           sn,
        rohc_U32bit_t           ack,
        rohc_encoded_value_t    *p_en_sn,
        rohc_encoded_value_t    *p_en_ack,
        rohc_U8bit_t            isSNChanged,
        rohc_U8bit_t            isACKChanged);

/* + SPR 17439 */
                    
rohc_void_t rohc_update_p1_ctx ROHC_PROTO((
                    rohc_entity_t           *p_entity   ,
                    rohc_csc_rtp_t          *p_csc      ,
                    rohc_encoded_value_t    *en_sn      ,
                    rohc_encoded_value_t    *en_ts      ,
                    rohc_encoded_value_t    *en_ip_id 
                     ));

rohc_return_t rohc_cprm_sel_pkt_p1_uo_ir ROHC_PROTO((
                    rohc_entity_t       * p_entity  ,
                    rohc_csc_rtp_t      *p_csc      ,
                    rohc_U8bit_t        *p_sel_pkt  ,
                    rohc_error_code_t   *p_ecode
                    ));

rohc_return_t rohc_cprm_sel_pkt_p1_uo_fo ROHC_PROTO((
                    rohc_entity_t       * p_entity  ,
                    rohc_csc_rtp_t      *p_csc      ,
                    rohc_U8bit_t        *p_sel_pkt  ,
                    rohc_error_code_t   *p_ecode
                    ));

rohc_return_t rohc_cprm_sel_pkt_p1_uo_so  ROHC_PROTO((
                    rohc_entity_t       * p_entity  ,
                    rohc_csc_rtp_t      *p_csc      ,
                    rohc_U8bit_t        *p_sel_pkt  ,
                    rohc_error_code_t   *p_ecode
                    ));

rohc_return_t rohc_cprm_sel_pkt_p1_r_ir  ROHC_PROTO((
                    rohc_entity_t       * p_entity  ,
                    rohc_csc_rtp_t      *p_csc      ,
                    rohc_U8bit_t        *p_sel_pkt  ,
                    rohc_error_code_t   *p_ecode
                    ));

rohc_return_t rohc_cprm_sel_pkt_p1_r_fo  ROHC_PROTO((
                    rohc_entity_t       * p_entity  ,
                    rohc_csc_rtp_t      *p_csc      ,
                    rohc_U8bit_t        *p_sel_pkt  ,
                    rohc_error_code_t   *p_ecode
                    ));

rohc_return_t rohc_cprm_sel_pkt_p1_r_so  ROHC_PROTO((
                    rohc_entity_t       * p_entity  ,
                    rohc_csc_rtp_t      *p_csc      ,
                    rohc_U8bit_t        *p_sel_pkt  ,
                    rohc_error_code_t   *p_ecode
                    ));

rohc_return_t rohc_cprm_sel_pkt_p2_uo_ir  ROHC_PROTO((
                    rohc_entity_t     * p_entity ,
                    rohc_csc_udp_t    *p_csc     ,
                    rohc_U8bit_t      *p_sel_pkt ,
                    rohc_error_code_t *p_ecode
                    ));
rohc_return_t rohc_cprm_sel_pkt_p2_uo_fo  ROHC_PROTO((
                    rohc_entity_t       * p_entity  ,
                    rohc_csc_udp_t      *p_csc      ,
                    rohc_U8bit_t        *p_sel_pkt  ,
                    rohc_error_code_t   *p_ecode
                    ));

rohc_return_t rohc_cprm_sel_pkt_p2_uo_so  ROHC_PROTO((
                    rohc_entity_t       * p_entity  ,
                    rohc_csc_udp_t      *p_csc      ,
                    rohc_U8bit_t        *p_sel_pkt  ,
                    rohc_error_code_t   *p_ecode
                    ));

rohc_return_t rohc_cprm_sel_pkt_p2_r_ir  ROHC_PROTO((
                    rohc_entity_t       * p_entity  ,
                    rohc_csc_udp_t      *p_csc      ,
                    rohc_U8bit_t        *p_sel_pkt  ,
                    rohc_error_code_t   *p_ecode
                    ));

rohc_return_t rohc_cprm_sel_pkt_p2_r_fo  ROHC_PROTO((
                    rohc_entity_t       * p_entity  ,
                    rohc_csc_udp_t      *p_csc      ,
                    rohc_U8bit_t        *p_sel_pkt  ,
                    rohc_error_code_t   *p_ecode
                    ));

rohc_return_t rohc_cprm_sel_pkt_p2_r_so  ROHC_PROTO((
                    rohc_entity_t       * p_entity  ,
                    rohc_csc_udp_t      *p_csc      ,
                    rohc_U8bit_t        *p_sel_pkt  ,
                    rohc_error_code_t   *p_ecode
                    ));

rohc_return_t rohc_cprm_sel_pkt_p6_ir  ROHC_PROTO((
                    rohc_entity_t     * p_entity ,
                    rohc_csc_tcp_t    *p_csc     ,
                    rohc_U8bit_t      *p_sel_pkt ,
                    rohc_error_code_t *p_ecode
                    ));
rohc_return_t rohc_cprm_sel_p6_co_pkt  ROHC_PROTO((
                    rohc_entity_t       * p_entity  ,
                    rohc_csc_tcp_t      *p_csc      ,
                    rohc_U8bit_t        *p_sel_pkt  ,
                    rohc_error_code_t   *p_ecode
                    ));

rohc_void_t rohc_cprm_p6_update_csc_tcp ROHC_PROTO((
            rohc_csc_tcp_t          *p_csc      ,
            rohc_encoded_value_t    *en_sn      ,
            rohc_encoded_value_t *en_ack        ,
            rohc_encoded_value_t    *en_msn 
            ));

rohc_return_t rohc_cprm_p1_call_encode ROHC_PROTO((
                    rohc_entity_t           *p_entity   ,
                    rohc_csc_rtp_t          *p_csc      ,
                    rohc_encoded_value_t    *en_sn      ,
                    rohc_encoded_value_t    *en_ts      ,
                    rohc_encoded_value_t    *en_ip_id   ,
                    rohc_error_code_t       *p_ecode
                    ));

rohc_U8bit_t rohc_cprm_p1_type_uo_fo ROHC_PROTO((
                    rohc_csc_rtp_t          *p_csc      ,
                    rohc_encoded_value_t    *en_sn      ,
                    rohc_encoded_value_t    *en_ts      ,
                    rohc_encoded_value_t    *en_ip_id 
                    ));
/* +- SPR 17777 */

rohc_U8bit_t rohc_cprm_p1_type_r_fo ROHC_PROTO((
                    rohc_entity_t           *p_entity   ,
                    rohc_csc_rtp_t          *p_csc      ,
                    rohc_encoded_value_t    *en_sn      ,
                    rohc_encoded_value_t    *en_ts      ,
                    rohc_encoded_value_t    *en_ip_id 
                    ));

rohc_void_t rohc_update_ctx_p2 ROHC_PROTO((
                    rohc_entity_t           *p_entity   ,
                    rohc_csc_udp_t          *p_csc      ,
                    rohc_encoded_value_t    *en_sn      ,
                    rohc_encoded_value_t    *en_ip_id   ,
                    rohc_encoded_value_t    *en_ip_id2  
                    ));

rohc_void_t rohc_update_ctx_p6 ROHC_PROTO((
                    rohc_entity_t           *p_entity   ,
                    rohc_csc_tcp_t          *p_csc      ,
                    rohc_encoded_value_t    *en_sn      ,
                    rohc_encoded_value_t    *en_ack   ,
                    rohc_encoded_value_t *en_msn      ,
                    rohc_encoded_value_t    *en_ip_id 
                    ));

rohc_U8bit_t rohc_cprm_p2_uo_fo ROHC_PROTO((
                    rohc_entity_t           *p_entity   ,
                    rohc_csc_udp_t          *p_csc      ,
                    rohc_encoded_value_t    *en_sn      ,
                    rohc_encoded_value_t    *en_ts      ,
                    rohc_encoded_value_t    *en_ip_id 
                    ));

rohc_U8bit_t rohc_cprm_p2_r_fo ROHC_PROTO((
                    rohc_entity_t           *p_entity   ,
                    rohc_csc_udp_t          *p_csc      ,
                    rohc_encoded_value_t    *en_sn      ,
                    rohc_encoded_value_t    *en_ts      ,
                    rohc_encoded_value_t    *en_ip_id 
                    ));                                                                                                                                                                                     
rohc_return_t rohc_cprm_call_encode_p2 ROHC_PROTO((
                    rohc_entity_t           *p_entity   ,
                    rohc_csc_udp_t          *p_csc      ,
                    rohc_encoded_value_t    *en_sn      ,
                    rohc_encoded_value_t    *en_ip_id   ,
                    rohc_encoded_value_t    *en_ip_id2  ,
                    rohc_error_code_t       *p_ecode
                    ));                                                                                                             
rohc_return_t rohc_cprm_p1_pr_fb  ROHC_PROTO((
            rohc_entity_t       *p_entity   ,   
            rohc_csc_rtp_t      *p_csc      ,
            rohc_U8bit_t        *p_buff     ,
            rohc_U8bit_t        len         ,
            rohc_error_code_t   *p_ecode 
            ));
rohc_return_t rohc_cprm_p2_pr_fb  ROHC_PROTO((
            rohc_entity_t     *p_entity ,
            rohc_csc_udp_t    *p_csc    ,
            rohc_U8bit_t      *p_buff   ,
            rohc_U8bit_t      len       ,
            rohc_error_code_t *p_ecode
            ));
rohc_return_t rohc_cprm_p6_pr_fb  ROHC_PROTO((
            rohc_entity_t       *p_entity   ,   
            rohc_csc_tcp_t      *p_csc      ,
            rohc_U8bit_t        *p_buff     ,
            rohc_U8bit_t        len         ,
            rohc_error_code_t   *p_ecode 
            ));
                 
rohc_void_t rohc_cprm_p1_type_updt_encd_val
	ROHC_PROTO((rohc_entity_t           *p_entity   ,
                    rohc_csc_rtp_t          *p_csc      ,
                    rohc_encoded_value_t    *en_sn      ,
                    rohc_encoded_value_t    *en_ts      ,
                    rohc_encoded_value_t    *en_ip_id 
                    ));

rohc_void_t rohc_cprm_p2_updt_encd_val
	ROHC_PROTO((rohc_entity_t            *p_entity   ,
                    rohc_csc_udp_t          *p_csc      ,
                    rohc_encoded_value_t    *en_sn      ,
                    rohc_encoded_value_t    *en_ip_id   ,
		    rohc_encoded_value_t    *en_ip_id2 
                    ));

/* + SPR 17439 */
#endif /* _ROHC_CPRM_INT_IF_H_ */
