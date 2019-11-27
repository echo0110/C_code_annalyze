/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_led_proto.h,v 1.2.36.4 2010/11/09 04:50:28 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_led_proto.h,v $
 * Revision 1.2.36.4  2010/11/09 04:50:28  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.3  2010/10/13 04:27:32  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.2  2010/10/04 03:22:37  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2.36.1  2010/09/23 07:01:12  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 05:20:12  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_LED_PROTO_H_
#define _ROHC_LED_PROTO_H_

#include "rohc_typ.h"
#include "rohc_g_typ.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"
#include "rohc_sl_windows.h"
rohc_void_t		c_func_hand_init_p1	ROHC_NO_PARAMS();
rohc_void_t		c_func_hand_init_p2	ROHC_NO_PARAMS();
/* + SPR 17439 */
rohc_return_t	rohc_cled_p1_uo_inneripext_list_comp ROHC_PROTO
	((
		rohc_entity_t	*p_entity	        ,
		rohc_csc_rtp_t	*p_csc		        ,
		rohc_U32bit_t	*p_length	        ,
		rohc_U8bit_t	*p_compressed_list	,
		rohc_error_code_t	*p_ecode
	));
rohc_return_t	rohc_cled_p2_uo_inneripext_list_comp ROHC_PROTO
	((
		rohc_entity_t	*p_entity	        ,
		rohc_csc_udp_t	*p_csc		        ,
		rohc_U32bit_t	*p_length	        ,
		rohc_U8bit_t	*p_compressed_list	,
		rohc_error_code_t	*p_ecode
	));

rohc_return_t	rohc_cled_p1_r_inneripext_list_comp ROHC_PROTO
	((
		rohc_entity_t	*p_entity	        ,
		rohc_csc_rtp_t  *p_csc		        ,
		rohc_U32bit_t	*p_length	        ,
		rohc_U8bit_t	*p_compressed_list	,
		rohc_error_code_t	*p_ecode
	));

rohc_return_t	rohc_cled_p2_r_inneripext_list_comp ROHC_PROTO
	((
		rohc_entity_t	*p_entity	        ,
		rohc_csc_udp_t	*p_csc		        ,
		rohc_U32bit_t	*p_length	        ,
		rohc_U8bit_t	*p_compressed_list	,
		rohc_error_code_t	*p_ecode
	));

rohc_return_t	rohc_cled_p1_uo_outeripext_list_comp ROHC_PROTO
	((
		rohc_entity_t	*p_entity	,
		rohc_csc_rtp_t	*p_csc		,
		rohc_U32bit_t	*p_length	,
		rohc_U8bit_t	*p_compressed_list	,
		rohc_error_code_t	*p_ecode
	));

rohc_return_t	rohc_cled_p2_uo_outeripext_list_comp ROHC_PROTO
	((
		rohc_entity_t	*p_entity	,
		rohc_csc_udp_t	*p_csc		,
		rohc_U32bit_t	*p_length	,
		rohc_U8bit_t	*p_compressed_list	,
		rohc_error_code_t	*p_ecode
	));

rohc_return_t	rohc_cled_p1_r_outeripext_list_comp ROHC_PROTO
	((
		rohc_entity_t	*p_entity	,
		rohc_csc_rtp_t	*p_csc		,
		rohc_U32bit_t	*p_length	,
		rohc_U8bit_t	*p_compressed_list	,
		rohc_error_code_t	*p_ecode
	));

rohc_return_t	rohc_cled_p2_r_outeripext_list_comp ROHC_PROTO
	((
		rohc_entity_t	*p_entity	,
		rohc_csc_udp_t	*p_csc		,
		rohc_U32bit_t	*p_length	,
		rohc_U8bit_t	*p_compressed_list	,
		rohc_error_code_t	*p_ecode
	));

rohc_return_t   rohc_cled_p1_uo_csrc_list_comp ROHC_PROTO
    ((
        rohc_entity_t       *p_entity   ,
        rohc_csc_rtp_t      *p_csc      ,
        rohc_U32bit_t       *p_length   ,
        rohc_U8bit_t        *p_compressed_list	,
	rohc_error_code_t	*p_ecode
    ));

rohc_return_t   rohc_cled_p1_r_csrc_list_comp ROHC_PROTO
    ((
        rohc_entity_t       *p_entity   ,
        rohc_csc_rtp_t      *p_csc      ,
        rohc_U32bit_t       *p_length   ,
        rohc_U8bit_t        *p_compressed_list	,
	rohc_error_code_t	*p_ecode
    ));

rohc_return_t	rohc_c_r_ext_hdr_list_comp_p1 ROHC_PROTO
	((
	rohc_entity_t       *p_entity 	,      
        rohc_csc_rtp_t      *p_csc 		,       
        YLIST               p_list 		,        
        rohc_U32bit_t        *p_length 	,     
        rohc_U8bit_t        *p_compressed_list	,
	rohc_error_code_t	*p_ecode
	));

rohc_return_t	rohc_c_r_ext_hdr_list_comp_p2 ROHC_PROTO
	((
	rohc_entity_t       *p_entity 	,      
        rohc_csc_udp_t      *p_csc 		,       
        YLIST               p_list 		,        
        rohc_U32bit_t       *p_length 	,     
        rohc_U8bit_t        *p_compressed_list	,
	rohc_error_code_t	*p_ecode
	));

rohc_return_t	rohc_c_uo_ext_hdr_list_comp_p1 ROHC_PROTO
	((
	rohc_entity_t       *p_entity	        ,       
        rohc_csc_rtp_t      *p_csc		        ,        
        YLIST               p_list		        ,         
        rohc_U32bit_t       *p_length	        ,      
        rohc_U8bit_t        *p_compressed_list	,
	rohc_error_code_t	*p_ecode
	));
rohc_return_t	rohc_c_uo_ext_hdr_list_comp_p2 ROHC_PROTO
	((
	rohc_entity_t       *p_entity	        ,       
        rohc_csc_udp_t      *p_csc		        ,        
        YLIST               p_list		        ,         
        rohc_U32bit_t       *p_length	        ,      
        rohc_U8bit_t        *p_compressed_list	,
	rohc_error_code_t	*p_ecode
	));
rohc_return_t   rohc_list_encoding_p6 ROHC_PROTO
    ((
        rohc_entity_t       *p_entity   ,
        rohc_csc_tcp_t      *p_csc      ,
        rohc_U32bit_t       *p_length   ,
        rohc_U8bit_t        *p_compressed_list	,
	rohc_error_code_t	*p_ecode
    ));
rohc_return_t	getIndexforOptionKindAndUpdateItemList ROHC_PROTO
	((
	    rohc_U8bit_t kind	                    ,       
        rohc_U8bit_t *p_item_list		        ,        
        rohc_c_tcp_opt_t *p_tcp_opt		        ,       
        rohc_csc_tcp_t  *p_csc                  ,
	    rohc_U8bit_t *p_len                     ,
        rohc_U8bit_t *gen_opt_idx
	));

rohc_return_t rohc_c_add_generic_option ROHC_PROTO
	((
        rohc_U8bit_t *p_item_list		        ,        
        rohc_c_generic_opt_t *p_generic_opt     ,
        rohc_error_code_t   *p_ecode
	));
rohc_U32bit_t  rohc_c_list_enc_tcp_opt_pkt ROHC_PROTO
((
 rohc_U8bit_t        PS_flag ,
 rohc_U8bit_t        count ,       
 rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_TCP_OPT_IDX] ,
 /* SPR 19690 fix start */
 rohc_U8bit_t       item_list[MAX_TCP_OPT_SIZE] ,
 /* SPR 19690 fix  end */
 rohc_U8bit_t        size ,
 rohc_U8bit_t        *p_compressed_list
 ));

rohc_void_t sack1_item_enc ROHC_PROTO
	((
	     rohc_U32bit_t *p_sack_field	    ,       
         rohc_U32bit_t  ack_no		        ,        
         rohc_U8bit_t *p_item_list 		    ,         
	     rohc_U8bit_t *p_len
	));

rohc_void_t sack2_item_enc ROHC_PROTO
	((
	     rohc_U32bit_t *p_sack_field	    ,       
         rohc_U32bit_t  ack_no		        ,        
         rohc_U8bit_t *p_item_list 		    ,         
	     rohc_U8bit_t *p_len
	));

rohc_void_t sack3_item_enc ROHC_PROTO
	((
	     rohc_U32bit_t *p_sack_field	    ,       
         rohc_U32bit_t  ack_no		        ,        
         rohc_U8bit_t *p_item_list 		    ,         
	     rohc_U8bit_t *p_len
	));

rohc_void_t sack4_item_enc ROHC_PROTO
	((
	     rohc_U32bit_t *p_sack_field	    ,       
         rohc_U32bit_t  ack_no		        ,        
         rohc_U8bit_t *p_item_list 		    ,         
	     rohc_U8bit_t *p_len
	));

rohc_void_t sack_block_enc ROHC_PROTO
	((
	     rohc_U32bit_t p_sack_field	    ,       
         rohc_U32bit_t ref_val		    ,        
         rohc_U8bit_t *p_item_list 		,         
	     rohc_U8bit_t *p_len
	));

rohc_return_t sack_pure_lsb ROHC_PROTO
	((
	     rohc_U32bit_t p_sack_field	    ,       
         rohc_U32bit_t ref_val		    
 ));

rohc_U32bit_t   rohc_c_enc_type_0_pkt ROHC_PROTO
    ((
                rohc_U8bit_t        gen_id ,
                rohc_U8bit_t        GP_flag ,
                rohc_U8bit_t        PS_flag ,
                rohc_U8bit_t        count ,
                rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] ,
                rohc_U8bit_t        *p_item_list ,
                rohc_U8bit_t        size ,
                rohc_U8bit_t        *p_compressed_list
    ));

rohc_U32bit_t   rohc_c_enc_type_0_csrc_pkt ROHC_PROTO
    ((
                rohc_U8bit_t        gen_id ,
                rohc_U8bit_t        GP_flag ,
                rohc_U8bit_t        PS_flag ,
                rohc_U8bit_t        count ,
                rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] ,
                rohc_U32bit_t       item_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] ,
                rohc_U8bit_t        size ,
                rohc_U8bit_t        *p_compressed_list
    ));

rohc_U32bit_t   rohc_c_enc_type_1_pkt ROHC_PROTO
    ((
                rohc_U8bit_t        gen_id ,
                rohc_U8bit_t        ref_id  ,
                rohc_U8bit_t        GP_flag  ,
                rohc_U8bit_t        PS_flag ,
                rohc_U8bit_t        count ,
                rohc_U16bit_t       insertion_bit_mask ,
                rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_TT] ,
                rohc_U8bit_t        *p_item_list 	,
                rohc_U32bit_t       size ,
                rohc_U8bit_t        *p_compressed_list
    ));

rohc_U32bit_t   rohc_c_enc_type_1_csrc_pkt ROHC_PROTO
    ((
                rohc_U8bit_t        gen_id ,
                rohc_U8bit_t        ref_id  ,
                rohc_U8bit_t        GP_flag  ,
                rohc_U8bit_t        PS_flag ,
                rohc_U8bit_t        count ,
                rohc_U16bit_t       insertion_bit_mask ,
                rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] ,
                rohc_U32bit_t       item_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] ,
                rohc_U32bit_t       size ,
                rohc_U8bit_t        *p_compressed_list
    ));

rohc_U32bit_t   rohc_c_enc_type_2_pkt  ROHC_PROTO
    ((
                rohc_U8bit_t        gen_id ,
                rohc_U8bit_t        ref_id ,
                rohc_U8bit_t        GP_flag ,
                rohc_U8bit_t        count ,
                rohc_U16bit_t       removal_bit_mask ,
                rohc_U8bit_t        *p_compressed_list
    ));

rohc_U32bit_t   rohc_c_enc_type_3_pkt ROHC_PROTO
    ((
                rohc_U8bit_t      gen_id ,
                rohc_U8bit_t      ref_id ,
                rohc_U8bit_t      GP_flag ,
                rohc_U8bit_t      PS_flag ,
                rohc_U8bit_t      count ,
                rohc_U16bit_t     removal_bit_mask ,
                rohc_U16bit_t     insertion_bit_mask ,
                rohc_U8bit_t      XI_list[ROHC_MAX_ENTRIES_IN_TT] ,
                rohc_U8bit_t      *p_item_list	,
                rohc_U32bit_t     size ,
                rohc_U8bit_t      *p_compressed_list
    ));

rohc_U32bit_t   rohc_c_enc_type_3_csrc_pkt ROHC_PROTO
    ((
                rohc_U8bit_t      gen_id ,
                rohc_U8bit_t      ref_id ,
                rohc_U8bit_t      GP_flag ,
                rohc_U8bit_t      PS_flag ,
                rohc_U8bit_t      count ,
                rohc_U16bit_t     removal_bit_mask ,
                rohc_U16bit_t     insertion_bit_mask ,
                rohc_U8bit_t      XI_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] ,
                rohc_U32bit_t     item_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] ,
                rohc_U32bit_t     size ,
                rohc_U8bit_t      *p_compressed_list
    ));

rohc_U32bit_t   rohc_c_cnst_pkt ROHC_PROTO
    ((
                rohc_U32bit_t       aseq_no ,
                rohc_U32bit_t       eseq_no ,
                rohc_U32bit_t       gseq_no ,
                rohc_U8bit_t        *p_compressed_list ,
                rohc_U32bit_t       tot_size ,
                rohc_U8bit_t        *p_final_list
    ));

rohc_void_t rohc_c_add_r_mode_ext_hdr_p1_sw ROHC_PROTO
    ((
            rohc_csc_rtp_t  *p_csc ,
            rohc_U8bit_t    count ,
            rohc_U8bit_t    *index_list
    ));

rohc_void_t rohc_c_add_r_mode_ext_hdr_p2_sw ROHC_PROTO
    ((
            rohc_csc_udp_t  *p_csc ,
            rohc_U8bit_t    count ,
            rohc_U8bit_t    *index_list
    ));

rohc_void_t rohc_c_add_p1_r_mode_csrc_sw ROHC_PROTO
    ((
            rohc_csc_rtp_t  *p_csc ,
            rohc_U8bit_t    count ,
            rohc_U8bit_t    *index_list
    ));

rohc_void_t   rohc_c_add_uo_mode_ext_hdr_p1_sw ROHC_PROTO
    ((
            rohc_csc_rtp_t  *p_csc ,
            rohc_U8bit_t    gen_id ,
            rohc_U8bit_t    count ,
            rohc_U8bit_t    *index_list
            ));

rohc_void_t   rohc_c_add_uo_mode_ext_hdr_p2_sw ROHC_PROTO
    ((
            rohc_csc_udp_t  *p_csc ,
            rohc_U8bit_t    gen_id ,
            rohc_U8bit_t    count ,
            rohc_U8bit_t    *index_list
    ));

rohc_void_t   rohc_c_add_p1_uo_mode_csrc_sw ROHC_PROTO
    ((
            rohc_csc_rtp_t      *p_csc ,
            rohc_U8bit_t        gen_id ,
            rohc_U8bit_t        count  ,
            rohc_U8bit_t        *index_list
    ));


rohc_void_t     rohc_c_add_sn_p1_r_mode_ext_hdr_tt  ROHC_PROTO
    ((
            rohc_csc_rtp_t      *p_csc ,
            rohc_U8bit_t        index ,
            r_ext_hdr_tt_t      *p_ext_hdr_tt
    ));

rohc_void_t     rohc_c_add_sn_p2_r_mode_ext_hdr_tt  ROHC_PROTO
    ((
            rohc_csc_udp_t      *p_csc ,
            rohc_U8bit_t        index ,
            r_ext_hdr_tt_t      *p_ext_hdr_tt
    ));

rohc_void_t     rohc_c_incr_counter_uo_ext_hdr_tt ROHC_PROTO
    ((
        rohc_entity_t       *p_entity ,
        uo_ext_hdr_tt_t     *p_ext_hdr_tt
    ));

rohc_void_t   rohc_c_add_p1_sn_r_mode_csrc_tt ROHC_PROTO
    ((
            rohc_csc_rtp_t  *p_csc   ,
            rohc_U8bit_t    index   ,
            r_csrc_tt_t     *p_csrc_tt
    ));

rohc_void_t   rohc_c_incr_counter_uo_csrc_tt ROHC_PROTO
    ((
        rohc_entity_t           *p_entity   ,
        uo_csrc_tt_t            *p_csrc_tt
    ));

rohc_return_t    rohc_c_convert_p1_in_buffer ROHC_PROTO
    ((
	rohc_entity_t	*p_entity	,
	rohc_csc_rtp_t	*p_csc	,
        rohc_U8bit_t    *p_node ,
	rohc_U8bit_t	*p_buf	,
        rohc_U32bit_t   *p_size	,
	rohc_error_code_t	*p_ecode
    ));

rohc_return_t    rohc_c_convert_p2_in_buffer ROHC_PROTO
    ((
	rohc_entity_t	*p_entity	,
	rohc_csc_udp_t  *p_csc	    ,
        rohc_U8bit_t    *p_node     ,
	rohc_U8bit_t	*p_buf	    ,
        rohc_U32bit_t   *p_size	    ,
	rohc_error_code_t	*p_ecode
    ));

    /* SPR +- 17777 */
rohc_return_t   rohc_c_compare_p1_ext_hdr_item    ROHC_PROTO
    ((
        rohc_U8bit_t        *p_item     ,
        rohc_list_node_t    *p_node 
    ));

rohc_return_t   rohc_c_compare_p2_ext_hdr_item    ROHC_PROTO
    ((
        rohc_U8bit_t        *p_item     ,
        rohc_list_node_t    *p_node 
    ));

rohc_return_t   rohc_c_compare_n_encode_p1_ext_hdr_item ROHC_PROTO
    ((
        rohc_U8bit_t    	*p_item     ,
        rohc_list_node_t 	*p_node     ,
        rohc_csc_rtp_t     	*p_csc      ,
        rohc_U32bit_t   	*p_aseq_no  ,
        rohc_U32bit_t   	*p_gseq_no
    ));

rohc_return_t   rohc_c_compare_n_encode_p2_ext_hdr_item ROHC_PROTO
    ((
        rohc_U8bit_t    	*p_item     ,
        rohc_list_node_t 	*p_node     ,
        rohc_csc_udp_t     	*p_csc      ,
        rohc_U32bit_t   	*p_aseq_no  ,
        rohc_U32bit_t   	*p_gseq_no
    ));

rohc_void_t rohc_update_XI_list ROHC_PROTO
    ((
        rohc_U8bit_t    *p_XI_byte  ,
        rohc_U8bit_t    I_value     ,
        rohc_U8bit_t    X_value
    ));

rohc_return_t   rohc_compare_prev_list  ROHC_PROTO
    ((
    	rohc_uo_sw_t    sw_entry    ,
    	rohc_U8bit_t    index_count ,
    	rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT]
    ));

rohc_U8bit_t   rohc_generate_gen_id    ROHC_PROTO
    ((
		rohc_uo_sw_t    *p_sw_prev_entry
    ));

rohc_void_t rohc_p6_update_XI_list ROHC_PROTO
    ((
        rohc_U8bit_t    *p_XI_byte  ,
        rohc_U8bit_t    I_value     ,
        rohc_U8bit_t    X_value     ,
        rohc_bool_t     PS_flag
    ));
/****** Decompressor Side Prototypes *********/

rohc_void_t		d_func_hand_init_p1	ROHC_NO_PARAMS();

rohc_void_t		d_func_hand_init_p2	ROHC_NO_PARAMS();

rohc_return_t	rohc_dled_p1_uo_inneripext_list_dec	ROHC_PROTO
	((
		rohc_entity_t	*p_entity	,
		rohc_dsc_rtp_t	*p_dsc		,
		rohc_U32bit_t	*p_length	,
		rohc_U8bit_t	*p_compressed_pkt	,
		rohc_error_code_t	*p_ecode
	));

rohc_return_t	rohc_dled_p2_uo_inneripext_list_dec	ROHC_PROTO
	((
		rohc_entity_t	    *p_entity	        ,
		rohc_dsc_udp_t    *p_dsc		        ,
		rohc_U32bit_t	    *p_length	        ,
		rohc_U8bit_t	    *p_compressed_pkt	,
		rohc_error_code_t	*p_ecode
	));

rohc_return_t   rohc_dled_p1_r_inneripext_list_dec ROHC_PROTO
    ((
        rohc_entity_t       *p_entity   ,
        rohc_dsc_rtp_t      *p_dsc      ,
        rohc_U32bit_t       *p_length   ,
        rohc_U8bit_t        *p_compressed_pkt	,
	rohc_error_code_t   *p_ecode
    ));

rohc_return_t   rohc_dled_p2_r_inneripext_list_dec ROHC_PROTO
    ((
        rohc_entity_t   *p_entity   ,
        rohc_dsc_udp_t  *p_dsc      ,
        rohc_U32bit_t   *p_length   ,
        rohc_U8bit_t    *p_compressed_pkt	,
	rohc_error_code_t	*p_ecode
    ));

rohc_return_t   rohc_dled_p1_uo_outeripext_list_dec ROHC_PROTO
    ((
        rohc_entity_t       *p_entity           ,
        rohc_dsc_rtp_t      *p_dsc              ,
        rohc_U32bit_t       *p_length           ,
        rohc_U8bit_t        *p_compressed_pkt	,
	rohc_error_code_t	*p_ecode
    ));

rohc_return_t   rohc_dled_p1_r_outeripext_list_dec ROHC_PROTO
    ((
        rohc_entity_t       *p_entity   ,
        rohc_dsc_rtp_t      *p_dsc      ,
        rohc_U32bit_t       *p_length   ,
        rohc_U8bit_t        *p_compressed_pkt	,
	rohc_error_code_t	*p_ecode
    ));

rohc_return_t   rohc_dled_p2_uo_outeripext_list_dec ROHC_PROTO
    ((
        rohc_entity_t   *p_entity   ,
        rohc_dsc_udp_t  *p_dsc      ,
        rohc_U32bit_t   *p_length   ,
        rohc_U8bit_t    *p_compressed_pkt	,
	rohc_error_code_t	*p_ecode
    ));

rohc_return_t   rohc_dled_p2_r_outeripext_list_dec ROHC_PROTO
    ((
        rohc_entity_t   *p_entity   ,
        rohc_dsc_udp_t  *p_dsc      ,
        rohc_U32bit_t   *p_length   ,
        rohc_U8bit_t    *p_compressed_pkt	,
	rohc_error_code_t	*p_ecode
    ));

rohc_return_t   rohc_dled_p1_csrc_list_dec ROHC_PROTO
    ((
        rohc_entity_t       *p_entity           ,
        rohc_dsc_rtp_t      *p_dsc              ,
        rohc_U32bit_t       *p_length           ,
        rohc_U8bit_t        *p_compressed_pkt	,
	rohc_error_code_t   *p_ecode
    ));

rohc_return_t   rohc_d_uo_ext_hdr_list_dec ROHC_PROTO
    ((
        rohc_entity_t   *p_entity   		,
        rohc_dsc_t      *p_dsc      		,
        rohc_U32bit_t   *p_length   		,
        rohc_U8bit_t    *p_compressed_pkt	,
	rohc_ah_t	*p_ah			,
	rohc_gre_t	*p_gre			,
	rohc_error_code_t	*p_ecode
    ));

rohc_return_t   rohc_d_r_ext_hdr_list_dec ROHC_PROTO
    ((
        rohc_entity_t   *p_entity   		,
        rohc_dsc_t      *p_dsc      		,
        rohc_U32bit_t   *p_length   		,
        rohc_U8bit_t    *p_compressed_pkt	,
	rohc_ah_t	*p_ah			,
	rohc_gre_t	*p_gre			,
	rohc_error_code_t	*p_ecode
    ));

rohc_return_t parse_p1_csrc_enc_type_0_pkt   ROHC_PROTO
((
    rohc_entity_t   *p_entity	        ,
    rohc_dsc_rtp_t  *p_dsc              ,
    rohc_U8bit_t    *p_compressed_pkt   ,
    rohc_U32bit_t   *p_length	        
));

rohc_return_t parse_p1_csrc_enc_type_1_pkt   ROHC_PROTO
((
    rohc_entity_t       *p_entity	        ,
    rohc_dsc_rtp_t      *p_dsc              ,
    rohc_U8bit_t        *p_compressed_pkt   ,
    rohc_U32bit_t   	*p_length	        
));

rohc_return_t parse_p1_csrc_enc_type_2_pkt   ROHC_PROTO
((
    rohc_dsc_rtp_t      *p_dsc  ,
    rohc_U8bit_t    *p_compressed_pkt   ,
    rohc_U32bit_t    *p_length	,
    rohc_error_code_t	*p_ecode
));

rohc_return_t parse_p1_csrc_enc_type_3_pkt   ROHC_PROTO
((
    rohc_entity_t	*p_entity	,
    rohc_dsc_rtp_t      *p_dsc  ,
    rohc_U8bit_t    *p_compressed_pkt   ,
    rohc_U32bit_t    *p_length	,
    rohc_error_code_t	*p_ecode
));

rohc_return_t parse_p1_ext_hdr_enc_type_0_pkt    ROHC_PROTO
((
    rohc_entity_t		*p_entity	    ,
    rohc_dsc_rtp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t              	*p_length           ,
    rohc_U8bit_t                *p_new_item_count   ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_U8bit_t                *p_ext_hdr_sw       ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre	    ,
    rohc_error_code_t		*p_ecode
));

rohc_return_t parse_p2_ext_hdr_enc_type_0_pkt    ROHC_PROTO
((
    rohc_entity_t	        *p_entity	    ,
    rohc_dsc_udp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t              	*p_length           ,
    rohc_U8bit_t                *p_new_item_count   ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_U8bit_t                *p_ext_hdr_sw       ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre	    ,
    rohc_error_code_t	      	*p_ecode
));

rohc_return_t parse_p1_ext_hdr_enc_type_1_pkt   ROHC_PROTO
((
    rohc_entity_t			*p_entity	    ,
    rohc_dsc_rtp_t     			*p_dsc  	    ,
    rohc_U8bit_t    			*p_compressed_pkt   ,
    rohc_U32bit_t   			*p_length   	    ,
    rohc_U8bit_t    			*p_new_item_count   ,
    rohc_list_t     			*p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t            *p_ext_hdr_tt       ,
    rohc_U8bit_t    			*p_new_index_list   ,
    rohc_U32bit_t   			esp_seq     	    ,
    rohc_U8bit_t    			*p_ext_hdr_sw       ,
    rohc_ah_t       			**p_p_ah    	    ,
    rohc_gre_t      			**p_p_gre	    ,
    rohc_error_code_t			*p_ecode
));

rohc_return_t parse_p2_ext_hdr_enc_type_1_pkt   ROHC_PROTO
((
    rohc_entity_t			*p_entity	    ,
    rohc_dsc_udp_t    			*p_dsc  	    ,
    rohc_U8bit_t    			*p_compressed_pkt   ,
    rohc_U32bit_t   			*p_length   	    ,
    rohc_U8bit_t    			*p_new_item_count   ,
    rohc_list_t     			*p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t            *p_ext_hdr_tt       ,
    rohc_U8bit_t    			*p_new_index_list   ,
    rohc_U32bit_t   			esp_seq     	    ,
    rohc_U8bit_t    			*p_ext_hdr_sw       ,
    rohc_ah_t       			**p_p_ah    	    ,
    rohc_gre_t      			**p_p_gre	    ,
    rohc_error_code_t			*p_ecode
));

rohc_return_t parse_p1_ext_hdr_enc_type_2_pkt    ROHC_PROTO
((
    rohc_entity_t		*p_entity	    ,
    rohc_dsc_rtp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t              	*p_length           ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_U8bit_t                *p_ext_hdr_sw       ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre	    ,
    rohc_error_code_t		*p_ecode
));

rohc_return_t parse_p2_ext_hdr_enc_type_2_pkt    ROHC_PROTO
((
    rohc_entity_t		*p_entity	    ,
    rohc_dsc_udp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t              	*p_length           ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_U8bit_t                *p_ext_hdr_sw       ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre	    ,
    rohc_error_code_t		*p_ecode
));

rohc_return_t parse_p1_ext_hdr_enc_type_3_pkt   ROHC_PROTO
((
    rohc_entity_t		*p_entity	    ,
    rohc_dsc_rtp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t               *p_length           ,
    rohc_U8bit_t                *p_new_item_count   ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_U8bit_t                *p_ext_hdr_sw       ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre	    ,
    rohc_error_code_t		*p_ecode
));

rohc_return_t parse_p2_ext_hdr_enc_type_3_pkt   ROHC_PROTO
((
    rohc_entity_t		*p_entity	    ,
    rohc_dsc_udp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t               *p_length           ,
    rohc_U8bit_t                *p_new_item_count   ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_U8bit_t                *p_ext_hdr_sw       ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre	    ,
    rohc_error_code_t		*p_ecode
));

rohc_return_t   rohc_d_ext_hdr_list_dec_p1     ROHC_PROTO
((
    rohc_entity_t		*p_entity	    ,
    rohc_dsc_rtp_t     		*p_dsc      	    ,
    rohc_U8bit_t    		*p_compressed_pkt   ,
    rohc_U32bit_t  		*p_length   	    ,
    rohc_U8bit_t    		*p_new_item_count   ,
    rohc_list_t     		*p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t    		*p_new_index_list   ,
    rohc_U8bit_t    		*p_ext_hdr_sw       ,
    rohc_ah_t       		**p_p_ah            ,
    rohc_gre_t      		**p_p_gre	    ,
    rohc_error_code_t		*p_ecode
));

rohc_return_t   rohc_d_ext_hdr_list_dec_p2     ROHC_PROTO
((
    rohc_entity_t		*p_entity	    ,
    rohc_dsc_udp_t          	*p_dsc      	    ,
    rohc_U8bit_t    		*p_compressed_pkt   ,
    rohc_U32bit_t   		*p_length   	    ,
    rohc_U8bit_t    		*p_new_item_count   ,
    rohc_list_t     		*p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t    		*p_new_index_list   ,
    rohc_U8bit_t    		*p_ext_hdr_sw       ,
    rohc_ah_t       		**p_p_ah            ,
    rohc_gre_t      		**p_p_gre	    ,
    rohc_error_code_t		*p_ecode
));
/* - SPR 17439 */

rohc_void_t     rohc_led_p1_init_inneripext 	ROHC_PROTO
(( rohc_dpib_t  *p_dpib ));

rohc_void_t     rohc_led_p2_init_inneripext 	ROHC_PROTO
(( rohc_dpib_t  *p_dpib ));

rohc_void_t     rohc_led_p1_init_outeripext 	ROHC_PROTO
(( rohc_dpib_t  *p_dpib ));

rohc_void_t     rohc_led_p2_init_outeripext 	ROHC_PROTO
(( rohc_dpib_t  *p_dpib ));

rohc_void_t     rohc_led_p1_init_csrc 			ROHC_PROTO
(( rohc_dpib_t  *p_dpib ));
/* + SPR 17439 */
rohc_return_t rohc_d_store_item_p1_in_list   ROHC_PROTO
((
    rohc_dsc_rtp_t		*p_dsc		,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt   ,
    rohc_U8bit_t                *p_buf          ,
    rohc_U32bit_t		*p_length	,
    rohc_ah_t                   **p_p_ah        ,
    rohc_gre_t                  **p_p_gre	,
    rohc_error_code_t		*p_ecode
));
rohc_return_t   rohc_d_store_item_p2_in_list   
    (rohc_dsc_udp_t              *p_dsc         ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt   ,
    rohc_U8bit_t                *p_buf          ,
    rohc_U32bit_t               *p_length       ,
    rohc_ah_t                   **p_p_ah        ,
    rohc_gre_t                  **p_p_gre       ,
    rohc_error_code_t           *p_ecode
    );
rohc_return_t     rohc_d_add_node_in_list     ROHC_PROTO
((
    rohc_list_t             *p_ext_hdr_list     ,
    rohc_protocol_node_t    *p_ext_hdr_node		,
    rohc_error_code_t	*p_ecode
));

rohc_return_t 	rohc_d_add_n_update_list    ROHC_PROTO
((
    rohc_list_t             *p_list     ,
    rohc_protocol_node_t    *p_node     ,
    rohc_U32bit_t           esp_seq     ,
    rohc_ah_t               **p_p_ah    ,
    rohc_gre_t              **p_p_gre	,
    rohc_error_code_t	*p_ecode
));

rohc_void_t 	rohc_led_encode_sn  ROHC_PROTO
((
    rohc_U32bit_t   *p_old_seq  ,
    rohc_U32bit_t   *p_seq
));

rohc_void_t     rohc_led_decode_sn  ROHC_PROTO
((
    rohc_U32bit_t   old_seq ,
    rohc_U32bit_t   *p_seq	,
    rohc_U8bit_t	len
));

rohc_U8bit_t    rohc_c_add_n_chk_p1_r_ext_hdr_tt ROHC_PROTO
((
    rohc_list_node_t    *p_item 	,
    rohc_csc_rtp_t      *p_csc 		,
    rohc_U8bit_t        *p_item_present	,
    rohc_error_code_t	*p_ecode
));

rohc_U8bit_t    rohc_c_add_n_chk_p2_r_ext_hdr_tt ROHC_PROTO
((
    rohc_list_node_t    *p_item 	,
    rohc_csc_udp_t      *p_csc 		,
    rohc_U8bit_t        *p_item_present	,
    rohc_error_code_t	*p_ecode
));

rohc_return_t    rohc_c_add_n_chk_p1_uo_ext_hdr_tt ROHC_PROTO
((
            rohc_list_node_t    *p_item ,
            rohc_csc_rtp_t      *p_csc  ,
            rohc_U8bit_t        *p_item_present	,
	    rohc_U32bit_t	*p_index ,
	    rohc_error_code_t	*p_ecode
));

rohc_return_t    rohc_c_add_n_chk_p2_uo_ext_hdr_tt ROHC_PROTO
((
            rohc_list_node_t    *p_item ,
            rohc_csc_udp_t      *p_csc  ,
            rohc_U8bit_t        *p_item_present	,
	    rohc_U32bit_t	*p_index ,
	    rohc_error_code_t	*p_ecode
));

rohc_U8bit_t    rohc_c_add_n_chk_p1_uo_csrc_tt ROHC_PROTO
((
            rohc_csc_rtp_t          *p_csc  ,
            rohc_U32bit_t           csrc_no ,
            rohc_U8bit_t            *p_item_present

));

rohc_U8bit_t    rohc_c_add_n_chk_p1_r_csrc_tt ROHC_PROTO
((
            rohc_csc_rtp_t          *p_csc  ,
            rohc_U32bit_t           csrc_no ,
            rohc_U8bit_t            *p_item_present
));
/* - SPR 17439 */
rohc_void_t rohc_d_update_p1_uo_csrc_sw    ROHC_PROTO
(( rohc_dsc_rtp_t *p_dsc ));
/* + SPR 17439 */
rohc_void_t     rohc_d_update_p1_uo_ext_sw ROHC_PROTO
(( rohc_dsc_rtp_t *p_dsc ,  rohc_d_uo_sw_t *p_sw ));

rohc_void_t     rohc_d_update_p2_uo_ext_sw ROHC_PROTO
(( rohc_dsc_udp_t *p_dsc ,  rohc_d_uo_sw_t *p_sw ));

rohc_void_t     rohc_d_update_p6_uo_ext_sw ROHC_PROTO
(( rohc_dsc_tcp_t *p_dsc ,  rohc_d_uo_sw_t *p_sw ));

rohc_void_t rohc_d_update_p1_r_ext_sw  ROHC_PROTO
((
    rohc_dsc_rtp_t  *p_dsc  ,
    rohc_r_sw_t     *p_sw   ,
    rohc_U8bit_t    ref_index
));

rohc_void_t rohc_d_update_p2_r_ext_sw  ROHC_PROTO
((
    rohc_dsc_udp_t  *p_dsc  ,
    rohc_r_sw_t *p_sw   ,
    rohc_U8bit_t    ref_index
));

rohc_void_t rohc_d_update_p6_r_ext_sw  ROHC_PROTO
((
    rohc_dsc_tcp_t  *p_dsc  ,
    rohc_r_sw_t *p_sw   ,
    rohc_U8bit_t    ref_index
));

rohc_void_t rohc_d_update_p1_r_csrc_sw ROHC_PROTO
((
    rohc_dsc_rtp_t  *p_dsc  ,
    rohc_r_sw_t     *p_sw   ,
    rohc_U8bit_t    ref_index
));

rohc_U8bit_t	rohc_led_p1_generate_ext_gen_id	ROHC_PROTO
((
	rohc_entity_t	*p_entity	,
	rohc_csc_rtp_t  *p_csc		,
	rohc_U8bit_t	count		,
	rohc_U8bit_t	index_list[ROHC_MAX_ENTRIES_IN_TT]	,
	rohc_bool_t	*p_gp_flag
));

rohc_U8bit_t  rohc_led_p2_generate_ext_gen_id  ROHC_PROTO
((
  rohc_entity_t     *p_entity ,
  rohc_csc_udp_t    *p_csc    ,
  rohc_U8bit_t      count     ,
  rohc_U8bit_t      index_list[ROHC_MAX_ENTRIES_IN_TT]  ,
  rohc_bool_t       *p_gp_flag
));

rohc_U8bit_t	rohc_led_p1_generate_csrc_gen_id	ROHC_PROTO
((
	rohc_entity_t	*p_entity	,
	rohc_csc_rtp_t	*p_csc		,
	rohc_U8bit_t	count		,
	rohc_U8bit_t	*index_list	,
	rohc_bool_t	*p_gp_flag
));

rohc_return_t   rohc_d_process_p1_new_item     ROHC_PROTO
((
    rohc_entity_t               *p_entity       ,
    rohc_dsc_rtp_t              *p_dsc          ,
    rohc_decomp_ext_hdr_tt_t    *p_tt           ,
    rohc_U32bit_t		*p_length	,
    rohc_list_t                 *p_ext_hdr_list ,
    rohc_ah_t                   **p_p_ah        ,
    rohc_gre_t                  **p_p_gre       ,
    rohc_U8bit_t                **p_p_item_start	,
    rohc_error_code_t		*p_ecode
));

rohc_return_t   rohc_d_process_p2_new_item     ROHC_PROTO
((
    rohc_entity_t               *p_entity         ,
    rohc_dsc_udp_t              *p_dsc            ,
    rohc_decomp_ext_hdr_tt_t    *p_tt             ,
    rohc_U32bit_t	        *p_length         ,
    rohc_list_t                 *p_ext_hdr_list   ,
    rohc_ah_t                   **p_p_ah          ,
    rohc_gre_t                  **p_p_gre         ,
    rohc_U8bit_t                **p_p_item_start  ,
    rohc_error_code_t	        *p_ecode
));

rohc_return_t   rohc_d_pick_item_from_tt  ROHC_PROTO
((
    rohc_decomp_ext_hdr_tt_t    *p_tt           ,
    rohc_list_t                 *p_ext_hdr_list ,
    rohc_U32bit_t               esp_seq         ,
    rohc_ah_t                   **p_p_ah        ,
    rohc_gre_t                  **p_p_gre	,
    rohc_error_code_t		*p_ecode
));

rohc_void_t     rohc_d_process_p1_csrc_new_item        ROHC_PROTO
((
    rohc_entity_t   *p_entity       ,
    rohc_dsc_rtp_t  *p_dsc          ,
    rohc_U8bit_t    index           ,
    rohc_U8bit_t    final_index     ,
    rohc_U8bit_t   *p_item_start
));

rohc_return_t     rohc_ed_p6_update_gsw_ts_val            ROHC_PROTO
(( 
    rohc_entity_t   *p_rohc_entity   ,
    rohc_csc_tcp_t  *p_csc           ,
    rohc_U32bit_t   ts_val
));

rohc_return_t     rohc_ed_p6_update_gsw_ts_echo            ROHC_PROTO
((
     rohc_entity_t  *p_rohc_entity    ,
     rohc_csc_tcp_t  *p_csc            ,
     rohc_U32bit_t  ts_val
));
/* - SPR 17439 */

#endif /* _ROHC_LED_PROTO_H_ */
