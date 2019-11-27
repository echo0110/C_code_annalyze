/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_c_crc_proto.h,v 1.1.1.1.16.1 2010/11/03 06:33:15 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the function prototypes for rohc
 *                     spcific CRC Computation.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_c_crc_proto.h,v $
 * Revision 1.1.1.1.16.1  2010/11/03 06:33:15  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.2.36.2  2010/10/13 04:22:09  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.1  2010/09/23 06:58:36  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:59:56  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_CRC_MAIN_H_
#define _ROHC_CRC_MAIN_H_

#include "rohc_crc_basic.h"

rohc_U8bit_t rohc_calculate_crc(void * data, rohc_U32bit_t start,rohc_U32bit_t end, rohc_U8bit_t previous_crc, rohc_U8bit_t crc_type);
rohc_U8bit_t rohc_c_compute_csrc_list_crc_p1(rohc_csc_rtp_t *p_csc,  rohc_U8bit_t crc_type, rohc_U8bit_t crc_val);
rohc_U8bit_t rohc_c_compute_ip_static_crc_p1 (rohc_csc_rtp_t *p_csc, rohc_U8bit_t crc_type, rohc_U8bit_t crc_val);
rohc_U8bit_t rohc_c_compute_ip_dynamic_crc_p1 (rohc_csc_rtp_t *p_csc, rohc_U8bit_t crc_type, rohc_U8bit_t crc_val);
rohc_U8bit_t rohc_c_compute_crc_static_p1 (rohc_csc_rtp_t *p_csc,  rohc_U8bit_t crc_type,rohc_U8bit_t crc_val);
rohc_U8bit_t rohc_c_compute_crc_dynamic_p1 (rohc_csc_rtp_t *p_csc,  rohc_U8bit_t crc_type, rohc_U8bit_t crc_val );
rohc_U8bit_t rohc_c_compute_ip_static_crc_p2 (rohc_csc_udp_t *p_csc, rohc_U8bit_t crc_type, rohc_U8bit_t crc_val);
rohc_U8bit_t rohc_c_compute_ip_dynamic_crc_p2 (rohc_csc_udp_t *p_csc, rohc_U8bit_t crc_type, rohc_U8bit_t crc_val);
rohc_U8bit_t rohc_c_compute_crc_static_p2 (rohc_csc_udp_t *p_csc,  rohc_U8bit_t crc_type,rohc_U8bit_t crc_val);
rohc_U8bit_t rohc_c_compute_crc_dynamic_p2 (rohc_csc_udp_t *p_csc,  rohc_U8bit_t crc_type, rohc_U8bit_t crc_val );

#endif /*End of _ROHC_CRC_MAIN_H_*/

