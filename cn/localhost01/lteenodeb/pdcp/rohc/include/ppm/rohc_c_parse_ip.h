/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_c_parse_ip.h,v 1.2.36.3 2010/11/09 04:52:13 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Function Prototypes for PPM
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_c_parse_ip.h,v $
 * Revision 1.2.36.3  2010/11/09 04:52:13  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.2  2010/10/13 04:31:42  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.1  2010/09/23 07:02:31  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 05:34:03  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_C_PARSE_IP_H_
#define _ROHC_C_PARSE_IP_H_

#include "rohc_c_typ.h"
#include "rohc_ccom_mcr.h"
#include "rohc_g_typ.h"
#include "rohc_protocol_header.h"
#include "rohc_ppm_ext_if.h"
#include "rohc_trace_mcr.h"
#include "rohc_def.h"
#include "rohc_typ.h"
#include "rohc_db_ext_if.h"

#define ROHC_GET_UPPER_NIBBLE(byte)	(((rohc_U8bit_t)(byte)&0xf0U)>>4U)
#define ROHC_GET_LOWER_NIBBLE(byte)	((rohc_U8bit_t)(byte)&0x0fU)
#define ROHC_GET_2MSB(byte)			(((rohc_U8bit_t)(byte)&0xc0U)>>6U)
#define ROHC_GET_7MSB(byte)			(((rohc_U8bit_t)(byte)&0xfeU)>>1U)

/* Structure used by C-PPM while parsing ,
only CPIB is visible to other modules */
typedef struct rohc_c_temp_context_t
{
        struct  rohc_cpib_t     * p_cpib;
        rohc_U32bit_t     	len_parsed; 
        rohc_U8bit_t      	profile_over;  
        rohc_U8bit_t      	cur_ip;  
} rohc_c_temp_context_t;

rohc_return_t rohc_parse_ip4_pkt
ROHC_PROTO((    struct rohc_ip4_t 		* p_pkt,
		struct rohc_entity_t		* p_entity,	
		struct rohc_c_temp_context_t   	* p_temp_fields,
		rohc_U32bit_t 			len,
		rohc_error_code_t		*p_ecode));

rohc_return_t rohc_parse_ip6_pkt
ROHC_PROTO((	struct rohc_ip6_t 		* p_pkt,
	        struct rohc_entity_t 		* p_entity,	
		struct rohc_c_temp_context_t   	* p_temp_fields,
		rohc_U32bit_t                   len,
		rohc_error_code_t               *p_ecode));

rohc_return_t rohc_parse_ah_pkt
ROHC_PROTO((    rohc_entity_t                *p_entity,
		struct rohc_ah_t             * p_pkt,
	        struct rohc_c_temp_context_t * p_temp_fields,
	        rohc_U32bit_t                *p_pkt_len,
	        rohc_error_code_t            *p_ecode));
							  
rohc_U32bit_t rohc_parse_esp_pkt
ROHC_PROTO((	struct rohc_esp_t            *p_pkt,
		struct rohc_c_temp_context_t * p_temp_fields));
							   
rohc_return_t rohc_parse_gre_pkt
ROHC_PROTO((    rohc_entity_t                *p_entity,
		struct rohc_gre_t            *p_pkt,
		struct rohc_c_temp_context_t * p_temp_fields,
		rohc_U32bit_t                *p_pkt_len,
		rohc_error_code_t            *p_ecode));
							   
rohc_U32bit_t rohc_parse_rtp_pkt
ROHC_PROTO((	struct rohc_rtp_t            *p_pkt,
		struct rohc_c_temp_context_t * p_temp_fields));
								
rohc_U32bit_t rohc_parse_udp_pkt
ROHC_PROTO((	struct rohc_udp_t            *p_pkt,
		struct rohc_c_temp_context_t * p_temp_fields));
							   
rohc_void_t rohc_parse_tcp_pkt
ROHC_PROTO((	struct rohc_tcp_t            *p_pkt,
		struct rohc_c_temp_context_t * p_temp_fields));

rohc_return_t rohc_parse_minenc_pkt
ROHC_PROTO((	rohc_entity_t                *p_entity,
		struct rohc_minenc_t 	     * p_pkt,
		struct rohc_c_temp_context_t * p_temp_fields,
		rohc_U32bit_t                *p_pkt_len,
		rohc_error_code_t            *p_ecode));

								
rohc_return_t rohc_push_protocol_node
ROHC_PROTO((	rohc_entity_t                *p_entity,
		rohc_U8bit_t                 proto,
		struct rohc_c_temp_context_t * p_temp_fields,
		rohc_U32bit_t                pkt_len,
		rohc_U8bit_t	             * p_pkt,
		rohc_error_code_t	     *p_ecode));

rohc_return_t rohc_parse_hopopts_pkt
ROHC_PROTO((	rohc_entity_t                *p_entity,
		struct rohc_hopopts_t 	     * p_pkt,
		struct rohc_c_temp_context_t * p_temp_fields,
		rohc_U32bit_t                *p_pkt_len,
		rohc_error_code_t            *p_ecode));


rohc_return_t rohc_parse_dstopts_pkt
ROHC_PROTO((	rohc_entity_t                *p_entity,
		struct rohc_dstoptions_t     * p_pkt,
		struct rohc_c_temp_context_t * p_temp_fields,
		rohc_U32bit_t                *p_pkt_len,
		rohc_error_code_t            *p_ecode));	

rohc_return_t rohc_parse_fragopts_pkt
ROHC_PROTO((	rohc_entity_t                *p_entity,
		struct rohc_fragopts_t 	     * p_pkt,
		struct rohc_c_temp_context_t * p_temp_fields,
		rohc_U32bit_t                *p_pkt_len,
		rohc_error_code_t            *p_ecode));

rohc_return_t rohc_parse_routopts_pkt
ROHC_PROTO((	rohc_entity_t                *p_entity,
		struct rohc_routopts_t 	     * p_pkt,
		struct rohc_c_temp_context_t * p_temp_fields,
		rohc_U32bit_t                *p_pkt_len,
		rohc_error_code_t            *p_ecode));

void rohc_c_check_ipid_beh
ROHC_PROTO((	struct rohc_csc_t * p_stream_context));

void rohc_c_check_ts_stride_beh
ROHC_PROTO((	struct rohc_csc_rtp_t * p_stream_context));

void rohc_c_check_ack_stride_beh
ROHC_PROTO((	struct rohc_csc_tcp_t * p_stream_context));

void rohc_esp_beh_det
	ROHC_PROTO((
	struct rohc_cpib_t 	  * p_cpib ,
	struct rohc_beh_det_blk_t * p_blk ,
	rohc_U8bit_t	          * p_pkt,
	struct rohc_beh_det_blk_t * * p_p_beh_det_blk));


#endif /* _ROHC_C_PARSE_IP_H_ */
