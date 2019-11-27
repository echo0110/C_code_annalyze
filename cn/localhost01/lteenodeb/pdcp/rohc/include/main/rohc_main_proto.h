/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_main_proto.h,v 1.2.36.4 2010/11/09 04:51:01 gur22059 Exp $
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
 * $Log: rohc_main_proto.h,v $
 * Revision 1.2.36.4  2010/11/09 04:51:01  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.3  2010/10/13 04:28:58  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.2  2010/10/04 03:11:58  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2.36.1  2010/09/23 07:01:49  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 05:27:38  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_MAIN_PROTO_H_
#define _ROHC_MAIN_PROTO_H_

#include "rohc_typ.h"
#include "rohc_ex.h"
#include "rohc_g_typ.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"

#include "rohc_main_typ.h"

/* + SPR 17439 */
rohc_void_t rohc_update_incoming_api_stats ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity     ,
        rohc_U8bit_t    api_id
    ));

rohc_return_t   rohc_process_data_req   ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_U8bit_t        **p_p_out_buff      ,
        rohc_api_info_t     *p_rohc_api_info    ,
        rohc_error_code_t   *p_ecode
    ));
rohc_return_t   rohc_process_p0_compr_pkt   ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_csc_common_t   *p_csc_ctx              ,
        rohc_U8bit_t        *msg                ,
        rohc_U32bit_t       header_len          ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_error_code_t   *p_ecode
    ));
rohc_return_t   rohc_process_p1_compr_pkt   ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_csc_common_t      *p_csc_ctx              ,
        rohc_U8bit_t        *msg                ,
        rohc_U32bit_t       header_len          ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_error_code_t   *p_ecode
    ));
rohc_return_t   rohc_process_p2_compr_pkt   ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_csc_common_t      *p_csc_ctx              ,
        rohc_U8bit_t        *msg                ,
        rohc_U32bit_t       header_len          ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_error_code_t   *p_ecode
    ));

/* +- SPR 17777 */
rohc_return_t   rohc_process_p6_compr_pkt  ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_csc_common_t      *p_csc_ctx              ,
        rohc_U8bit_t        *msg                ,
        rohc_U32bit_t       header_len          ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_error_code_t   *p_ecode
     ));

rohc_return_t   rohc_process_config_req ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_U8bit_t        **p_p_out_buff      ,
        rohc_api_info_t     *p_rohc_api_info    ,
        rohc_error_code_t   *p_ecode
    ));

rohc_return_t   rohc_process_reconfig_req ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_U8bit_t        **p_p_out_buff      ,
        rohc_api_info_t     *p_rohc_api_info    ,
        rohc_error_code_t   *p_ecode
    ));
rohc_return_t   rohc_process_reset_req ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_U8bit_t        **p_p_out_buff      ,
        rohc_api_info_t     *p_rohc_api_info    ,
        rohc_error_code_t   *p_ecode
    ));
rohc_return_t   rohc_process_release_req    ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_U8bit_t        **p_p_out_buff      ,
        rohc_api_info_t     *p_rohc_api_info    ,
        rohc_error_code_t   *p_ecode
    ));

rohc_return_t   rohc_process_packet_size_used_req   ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_U8bit_t        **p_p_out_buff      ,
        rohc_api_info_t     *p_rohc_api_info    ,
        rohc_error_code_t   *p_ecode
    ));

/*#ifdef PDCP_REL_5*/
rohc_return_t   rohc_process_context_req        ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_U8bit_t        **p_p_out_buff      ,
        rohc_api_info_t     *p_rohc_api_info    ,
        rohc_error_code_t   *p_ecode
    ));
/*#endif*/

rohc_return_t   rohc_process_set_rohc_info_req  ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_U8bit_t        **p_p_out_buff      ,
        rohc_api_info_t     *p_rohc_api_info    ,
        rohc_error_code_t   *p_ecode
    ));

rohc_return_t   rohc_process_rlc_data_ind   ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_U8bit_t        **p_p_out_buff      ,
        rohc_api_info_t     *p_rohc_api_info    ,
        rohc_error_code_t   *p_ecode
    ));

rohc_return_t   rohc_process_p0_decompr_pkt   ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_dsc_common_t   *p_dsc_ctx              ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_api_info_t     *p_rohc_api_info    ,
        rohc_error_code_t   *p_ecode
    ));

rohc_return_t   rohc_process_p1_decompr_pkt   ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_dsc_common_t      *p_dsc_ctx              ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_api_info_t     *p_rohc_api_info    ,
        rohc_error_code_t   *p_ecode
    ));

rohc_return_t   rohc_process_p2_decompr_pkt   ROHC_PROTO
    ((
        rohc_entity_t       *p_rohc_entity      ,
        rohc_dsc_common_t      *p_dsc_ctx              ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_api_info_t     *p_rohc_api_info    ,
        rohc_error_code_t   *p_ecode
    ));

rohc_return_t   rohcProcessP6DecomprPkt   (
        rohc_entity_t       *p_rohc_entity      ,
        rohc_dsc_common_t         *p_dsc_ctx              ,
        rohc_U8bit_t        *p_in_buff          ,
        rohc_api_info_t     *p_rohc_api_info    ,
        rohc_error_code_t   *p_ecode
);
    
/* +- SPR 17777 */
rohc_void_t fill_csc_stream_packet_info     ROHC_PROTO
    ((
        stream_pkt_info_t       *p_stream_packet_info   ,
        rohc_csc_t              *p_csc
    ));

rohc_void_t fill_dsc_stream_packet_info     ROHC_PROTO
    ((
        stream_pkt_info_t   *p_stream_packet_info   ,
        rohc_dsc_t          *p_dsc
    ));


rohc_void_t fill_static_csc_info        ROHC_PROTO
    ((
        rohc_csc_t          *p_csc          ,
        stream_pkt_info_t   *p_stream_packet_info
    ));

rohc_void_t fill_static_dsc_info        ROHC_PROTO
    ((
        rohc_dsc_t          *p_dsc          ,
        stream_pkt_info_t   *p_stream_packet_info
    ));


rohc_len_t  rohc_pack_dyn_csc_info   ROHC_PROTO
    ((
        rohc_csc_t              *p_csc          ,
        rohc_U8bit_t            *p_buf          ,
        rohc_U32bit_t           max_buf_len     ,
        rohc_error_code_t       *p_ecode
    ));

rohc_len_t  rohc_pack_dyn_dsc_info   ROHC_PROTO
    ((
        rohc_dsc_t              *p_dsc          ,
        rohc_U8bit_t            *p_buf          ,
        rohc_U32bit_t           max_buf_len     ,
        rohc_error_code_t       *p_ecode
    ));

rohc_len_t  rohc_unpack_dyn_csc_info ROHC_PROTO
    ((
        rohc_csc_t              *p_csc          ,
        rohc_U8bit_t            *p_buf          ,
        rohc_U32bit_t           max_buf_len     ,
        rohc_error_code_t       *p_ecode
    ));

rohc_len_t  rohc_unpack_dyn_dsc_info ROHC_PROTO
    ((
        rohc_dsc_t              *p_dsc          ,
        rohc_U8bit_t            *p_buf          ,
        rohc_U32bit_t           max_buf_len     ,
        rohc_error_code_t       *p_ecode
    ));

rohc_len_t  rohc_pack_p_item     ROHC_PROTO
    ((
        rohc_U8bit_t            *p_item     ,
        rohc_U8bit_t            *p_buf  
    ));

rohc_len_t  rohc_unpack_p_item   ROHC_PROTO
    ((
        rohc_U8bit_t            **p_p_item  ,
        rohc_U8bit_t            *p_buf  
    ));

rohc_return_t 	rohc_validate_fb ROHC_PROTO
	((
		rohc_U8bit_t    *p_pkt  ,
		rohc_U16bit_t   len     ,
		rohc_U8bit_t    cid_len

	 )); 

rohc_return_t 	rohc_prof6_validate_fb ROHC_PROTO
	((
		rohc_U8bit_t    *p_pkt  ,
		rohc_U16bit_t   len     ,
		rohc_U8bit_t    cid_len
	 )); 
/* - SPR 17439 */
#endif /* _ROHC_MAIN_PROTO_H_ */
