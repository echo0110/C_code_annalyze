/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_d_crc_proto.h,v 1.2.36.2 2010/11/09 04:48:10 gur22059 Exp $
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
 * $Log: rohc_d_crc_proto.h,v $
 * Revision 1.2.36.2  2010/11/09 04:48:10  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.1  2010/10/04 03:34:00  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:59:56  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/


#ifndef _ROHC_D_CRC_PROO_H_
#define _ROHC_D_CRC_PROO_H_

#include "rohc_crc_basic.h"


rohc_U8bit_t rohc_d_compute_p1_crc_dynamic (rohc_dsc_rtp_t         *p_dsc,
			                    rohc_dprm_update_blk_t *p_plk,
                                            rohc_U8bit_t           crc_type, 
			                    rohc_U8bit_t           crc_val );

rohc_U8bit_t rohc_d_compute_p2_crc_dynamic (rohc_dsc_udp_t          *p_dsc,
			                    rohc_dprm_update_blk_t  *p_plk,
                                            rohc_U8bit_t            crc_type, 
                                            rohc_U8bit_t            crc_val );

rohc_U8bit_t rohc_d_compute_p6_crc_dynamic (rohc_dsc_tcp_t          *p_dsc,
			                    rohc_dprm_update_blk_t  *p_plk,
                                            rohc_U8bit_t            crc_type, 
                                            rohc_U8bit_t            crc_val );

rohc_U8bit_t rohc_d_calculate_crc(rohc_void_t * data,
                                  rohc_U32bit_t start,
                                  rohc_U32bit_t end, 
                                  rohc_U8bit_t  previous_crc, 
                                  rohc_U8bit_t  crc_type);

/* +- SPR 17777 */
rohc_U8bit_t rohc_d_compute_p1_csrc_list_crc(
        /* +- SPR 17777 */
					     rohc_dprm_update_blk_t    *p_plk,
                                             rohc_U8bit_t              crc_type, 
                                             rohc_U8bit_t              crc_val);

rohc_U8bit_t rohc_d_compute_p1_ip_static_crc (rohc_dsc_rtp_t           *p_dsc,
					      rohc_dprm_update_blk_t   *p_plk,
                                              rohc_U8bit_t             crc_type, 
                                              rohc_U8bit_t             crc_val);

rohc_U8bit_t rohc_d_compute_p1_ip_dynamic_crc (rohc_dsc_rtp_t          *p_dsc,
					       rohc_dprm_update_blk_t  *p_plk,
                                               rohc_U8bit_t            crc_type, 
                                               rohc_U8bit_t            crc_val);

rohc_U8bit_t rohc_d_compute_p1_crc_static (rohc_dsc_rtp_t           *p_dsc,
			                   rohc_dprm_update_blk_t   *p_plk,
                                           rohc_U8bit_t             crc_type,
                                           rohc_U8bit_t             crc_val);

rohc_U8bit_t rohc_d_compute_p2_crc_static (rohc_dsc_udp_t           *p_dsc,
			                   rohc_dprm_update_blk_t   *p_plk,
                                           rohc_U8bit_t             crc_type,
                                           rohc_U8bit_t             crc_val);

rohc_U8bit_t rohc_d_compute_p6_crc_static (rohc_dsc_tcp_t           *p_dsc,
			                   rohc_dprm_update_blk_t   *p_plk,
                                           rohc_U8bit_t             crc_type,
                                           rohc_U8bit_t             crc_val);

#endif /*End of _ROHC_CRC_MAIN_H_*/
