/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_c_pbm.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the common data types which are to
 *                     be used in C-PPM.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_c_pbm.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:29:58  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_C_PBM_H_
#define _ROHC_C_PBM_H_

#include "rohc_pbm_ext_if.h"
#include "rohc_protocol_header.h"
#include "rohc_led_ext_if.h"
#include "rohc_led_proto.h"
#include "rohc_ccom_mcr.h"
#include "rohc_trace_mcr.h"
#include "rohc_ed_proto.h"

/* structure used to describe ROHC general packet*/
struct rohc_pbm_pkt_field_info_t
{
	rohc_U8bit_t	field_id;
	rohc_U8bit_t	no_of_bits;
/* used to interpret no_of_bits element  
whether fixed length or var len/list/presence absence dependent on
flags in extension headers*/
#define ROHC_ENCTYPE_VAR_NO_BITS			(255U)
#define ROHC_ENCTYPE_EXT3FLAGSP1			(254U)
#define	ROHC_ENCTYPE_INNERIPFLAGSP1			(253U)
#define ROHC_ENCTYPE_OUTERIPFLAGSP1			(252U)
#define ROHC_ENCTYPE_RTPFLAGSP1				(251U)
#define ROHC_ENCTYPE_SPLIT_FIELD			(250U)
#define ROHC_ENCTYPE_TCP_OPTIONS_LIST	    (249U)
#define ROHC_ENCTYPE_VAR_32_BITS			(248U)
#define ROHC_ENCTYPE_STATIC_OR_IRREG	    (247U)
#define ROHC_ENCTYPE_OPT_IP_ID_LSB	        (246U)

#define ROHC_ENC_MAX_FIXED_BITS_IN_FIELD	(32U)
/* following fields utilised only if 
no_of_bits = ROHC_ENCTYPE_SPLIT_FIELDS*/
/* actual field to be read determined from cur_occ */
		rohc_U8bit_t	no_of_bits_occ;
/* following field utilised only if 
no_of_bits == ROHC_ENCTYPE_SPLIT_FIELDS*/
/* First/second/third occurence of the field */
/*
	rohc_U8bit_t	cur_occ;	
#define ROHC_OCC1	13U
#define ROHC_OCC2	14U
#define ROHC_OCC3	15U
*/
};


/* SPR 3548 */
extern struct rohc_pbm_pkt_field_info_t	rohc_c_pkt_type[ROHC_MAX_PKT_TYPES]
									[ROHC_MAX_NO_OF_FIELDS_IN_A_PACKET];
extern struct rohc_pbm_pkt_field_info_t   rohc_c_irreg_chain_type[ROHC_MAX_IRREG_CHAINS]
                                            [ROHC_MAX_NO_OF_FIELDS_IN_A_IRREG_CHAIN];

rohc_void_t clear_options_from_context(rohc_csc_tcp_t * p_csc);
#endif /* _ROHC_C_PBM_H_ */
