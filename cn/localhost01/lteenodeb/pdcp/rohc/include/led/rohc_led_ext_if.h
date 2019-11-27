/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_led_ext_if.h,v 1.2.36.4 2010/11/09 04:50:00 gur22059 Exp $
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
 * $Log: rohc_led_ext_if.h,v $
 * Revision 1.2.36.4  2010/11/09 04:50:00  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.3  2010/10/13 04:26:10  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.2  2010/10/04 03:23:45  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2.36.1  2010/09/23 07:00:30  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 05:20:12  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/


#ifndef __ROHC_LED_EXT_IF_H__
#define __ROHC_LED_EXT_IF_H__

/* Function prototypes for funtions called by other modules */

/* Function to be called for IP Extension header list compression */
/* + SPR 17439 */
rohc_return_t 	rohc_list_encoding_p1 ROHC_PROTO
				((
					rohc_entity_t	    *p_entity	        ,
					rohc_U8bit_t	    field_id	        ,
					rohc_csc_rtp_t	    *p_csc	        ,
					rohc_U32bit_t	    *p_length	        ,
					rohc_U8bit_t	    *p_compressed_list	,
					rohc_error_code_t   *p_ecode
				));
rohc_return_t   rohc_list_encoding_p2 ROHC_PROTO
        ((
          rohc_entity_t     *p_entity           ,
          rohc_U8bit_t      field_id            ,
          rohc_csc_udp_t    *p_csc              ,
          rohc_U32bit_t     *p_length           ,
          rohc_U8bit_t      *p_compressed_list  ,
          rohc_error_code_t *p_ecode
        ));

/* Funtion to be called for list decoding */
rohc_return_t	rohc_list_decoding_p1 ROHC_PROTO
				((
					rohc_entity_t		*p_entity	        ,
					rohc_dsc_rtp_t		*p_dsc		        ,
					rohc_U8bit_t		field_id	        ,
					rohc_U32bit_t		*p_length	        ,
 					rohc_U8bit_t		*p_compressed_pkt	,
					rohc_error_code_t	*p_ecode
				));

/* Funtion to be called for list decoding */
rohc_return_t	rohc_list_decoding_p2 ROHC_PROTO
				((
					rohc_entity_t		*p_entity	        ,
					rohc_dsc_udp_t	        *p_dsc		        ,
					rohc_U8bit_t		field_id	        ,
					rohc_U32bit_t		*p_length	        ,
 					rohc_U8bit_t		*p_compressed_pkt	,
					rohc_error_code_t	*p_ecode
				));
/* - SPR 17439 */
/* Funtion to be called for list decoding */
rohc_return_t	rohc_list_decoding_p6
				(
					rohc_dsc_tcp_t	    *p_dsc		        ,
					rohc_U8bit_t		field_id	        ,
 					rohc_U8bit_t		*p_pkt	
				);
/* + SPR 17439 */
/* Function to be called for updating DSC */
                /* SPR +- 17777 */
rohc_return_t	rohc_dled_p1_update_dsc	ROHC_PROTO
				((
					rohc_dsc_rtp_t		*p_dsc		,
					rohc_dpib_t		*p_dpib
				));

/* Function to be called for updating DSC */
rohc_return_t	rohc_dled_p2_update_dsc	ROHC_PROTO
				((
					rohc_dsc_udp_t  	*p_dsc		,
					rohc_dpib_t		*p_dpib	
				));

/* Function to be called when ACK is received at compressor side */
rohc_return_t	rohc_cled_p1_ack_handling	ROHC_PROTO
				((
					rohc_csc_rtp_t  	*p_csc		,
					rohc_U16bit_t		rtp_sn	
				));
rohc_return_t rohc_cled_p2_ack_handling  ROHC_PROTO
        ((
          rohc_csc_udp_t    *p_csc    ,
          rohc_U16bit_t     rtp_sn   
        ));
        /* SPR +- 17777 */
/* - SPR 17439 */
/* Function to be called to initialize the LED module */
rohc_return_t   rohc_led_p1_init 	ROHC_NO_PARAMS ();

rohc_return_t   rohc_led_p2_init 	ROHC_NO_PARAMS ();
#endif /* __ROHC_LED_EXT_IF_H__ */
