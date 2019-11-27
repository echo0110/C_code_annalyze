/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_led_fn_tbl.h,v 1.2.36.4 2010/11/09 04:50:14 gur22059 Exp $
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
 * $Log: rohc_led_fn_tbl.h,v $
 * Revision 1.2.36.4  2010/11/09 04:50:14  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.3  2010/10/13 04:26:38  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.2  2010/10/04 03:24:20  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2.36.1  2010/09/23 07:00:50  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 05:20:12  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/


#ifndef _ROHC_LED_FN_TBL_H_
#define _ROHC_LED_FN_TBL_H_

#include "rohc_typ.h"
#include "rohc_com_def.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"
/* + SPR 17439 */
typedef	rohc_return_t	(*c_p1_func_table_t) ROHC_PROTO
	((
	rohc_entity_t   	*p_entity 	,
        rohc_csc_rtp_t    	*p_csc 		,
        rohc_U32bit_t    	*p_length 	,
        rohc_U8bit_t    	*p_compressed_list	,
	rohc_error_code_t	*p_ecode
	));

typedef	rohc_return_t	(*c_p2_func_table_t) ROHC_PROTO
	((
	rohc_entity_t   	*p_entity 	,
        rohc_csc_udp_t          *p_csc	        ,
        rohc_U32bit_t    	*p_length 	,
        rohc_U8bit_t    	*p_compressed_list	,
	rohc_error_code_t	*p_ecode
	));

typedef rohc_return_t   (*d_p1_func_table_t) ROHC_PROTO
    ((
        rohc_entity_t   	*p_entity 	,
        rohc_dsc_rtp_t      	*p_dsc 		,
        rohc_U32bit_t    	*p_length 	,
        rohc_U8bit_t    	*p_compressed_list	,
	rohc_error_code_t	*p_ecode
    ));

typedef rohc_return_t   (*d_p2_func_table_t) ROHC_PROTO
    ((
        rohc_entity_t   	*p_entity 	,
        rohc_dsc_udp_t   	*p_dsc 		,
        rohc_U32bit_t    	*p_length 	,
        rohc_U8bit_t    	*p_compressed_list	,
	rohc_error_code_t	*p_ecode
    ));
/* - SPR 17439 */
typedef rohc_return_t   (*d_p6_func_table_t)
(
    rohc_entity_t   	*p_entity   ,
    rohc_dsc_tcp_t   	*p_dsc 		,
    rohc_U32bit_t    	*p_length 	,
    rohc_U8bit_t    	*p_compressed_list	,
    rohc_error_code_t	*p_ecode
);
    
/* Start of change for VARIBLE Declaration */
extern c_p1_func_table_t	rohc_c_p1_func_table[CSRC_FID_R_MODE + 1];
extern c_p2_func_table_t	rohc_c_p2_func_table[OUTER_IP_EXT_LIST_FID_R_MODE + 1];
extern d_p1_func_table_t	rohc_d_p1_func_table[CSRC_FID_R_MODE + 1];
extern d_p2_func_table_t	rohc_d_p2_func_table[OUTER_IP_EXT_LIST_FID_R_MODE + 1];
/* End of change for VARIBLE Declaration */
/* SPR 6363 FIX start */
extern d_p6_func_table_t	rohc_d_p6_func_table[OUTER_IP_EXT_LIST_FID_R_MODE + 1];
/* SPR 6363 FIX end */

#endif /* _ROHC_LED_FN_TBL_H_ */
