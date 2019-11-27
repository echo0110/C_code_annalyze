/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dcpm_p2_u_initiated.h,v 1.1.2.1 2010/11/03 09:02:36 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function prototypes for U-mode
 *         decompressor CPM, when decompressor transition state is INITIATED.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dcpm_p2_u_initiated.h,v $
 * Revision 1.1.2.1  2010/11/03 09:02:36  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.1.2.1  2010/10/01 14:17:05  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/


#ifndef _ROHC_DCPM_P2_U_INITIATED_
#define _ROHC_DCPM_P2_U_INITIATED_


/* + SPR 17439 */
rohc_void_t rohc_d_p2_u_ir_nc_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_udp_t
                                                                   *p_dsc));

rohc_void_t rohc_d_p2_u_ir_irdyn_uor2_sc_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_udp_t
                                                                   *p_dsc));

rohc_void_t rohc_d_p2_u_ir_irdyn_uor2_fc_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_udp_t
                                                                   *p_dsc));


rohc_void_t rohc_d_p2_u_ack_u_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_udp_t *p_dsc));


rohc_void_t rohc_d_p2_u_mt_u_to_o_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_udp_t *p_dsc));


rohc_void_t rohc_d_p2_u_mt_u_to_r_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_udp_t *p_dsc));


/* - SPR 17439 */

#endif /* _ROHC_DCPM_U_INITIATED_ */

