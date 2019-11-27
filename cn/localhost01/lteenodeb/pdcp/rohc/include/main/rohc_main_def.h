/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_main_def.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains all the ROHC #defined constants
 *                     and macros for the main module i.e.. interface module
 *                     between PDCP and ROHC.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_main_def.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:27:38  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_MAIN_DEF_H_
#define _ROHC_MAIN_DEF_H_

#define ROHC_VERSION_IP4 			0x04
#define ROHC_VERSION_IP6 			0x06

#define ROHC_COMPR_INFO_PRESENT  	0x01
#define ROHC_DECOMPR_INFO_PRESENT	0x02

#define MAX_PACKET_SIZE 			1024
#define ROHC_MAX_PARAM_BYTES		100
 

#define ROHC_EXTRACT_ROHC_INFO_FIELDS(bit_mask_m, flag_m, result_m) 	\
		do 																\
		{ 																\
			if ((bit_mask_m & flag_m) == flag_m) 						\
				result_m = ROHC_TRUE ; 									\
			else														\
				result_m = ROHC_FALSE ; 								\
		} while(0)

#ifdef ROHC_FUNC_ENHANCEMENT
#define ROHC_LSHIFT(val, num_bits)   (((unsigned)(val)) << (num_bits))
#define ROHC_RSHIFT(val, num_bits)   (((unsigned)(val)) >> (num_bits))

static const rohc_U8bit_t rohc_mask[8] = { 0x01, 0x03, 0x07, 0x0f, \
                                      0x1f, 0x3f, 0x7f, 0xff };

#define ROHC_ENCODE_VAL(buf, byte_posn, bit_posn, bit_len, val)\
    (buf[byte_posn] |= ROHC_LSHIFT((val & rohc_mask[bit_len - 1]), bit_posn))

#define ROHC_DECODE_VAL(buf, byte_posn, bit_posn, bit_len) \
        (ROHC_RSHIFT( buf[byte_posn] , bit_posn ) & rohc_mask[bit_len-1] )
#endif


#endif /* _ROHC_MAIN_DEF_H_ */
