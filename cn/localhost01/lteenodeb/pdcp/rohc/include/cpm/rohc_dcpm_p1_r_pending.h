/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dcpm_p1_r_pending.h,v 1.1.2.1 2010/11/09 04:43:12 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function prototypes for R-mode
 *         decompressor CPM, when decompressor transition state is PENDING.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dcpm_p1_r_pending.h,v $
 * Revision 1.1.2.1  2010/11/09 04:43:12  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.1.2.1  2010/10/04 03:38:39  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_DCPM_P1_R_PENDING_
#define _ROHC_DCPM_P1_R_PENDING_


/* + SPR 17439 */
rohc_void_t rohc_d_p1_r_ir_nc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_ir_sc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_ir_fc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_irdyn_nc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_irdyn_sc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_irdyn_fc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_r_0_nc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_r_0_sc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_r_0_fc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_r_0_crc_nc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_r_0_crc_sc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_r_0_crc_fc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_r_1_nc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_r_1_sc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_r_1_fc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_r_2_nc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_r_2_sc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_r_2_fc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_r_ack_r_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));


rohc_void_t rohc_d_p1_r_static_nack_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));



rohc_void_t rohc_d_p1_r_nack_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));


rohc_void_t rohc_d_p1_r_mt_r_to_u_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));


rohc_void_t rohc_d_p1_r_mt_r_to_o_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));


/* - SPR 17439 */

#endif /* _ROHC_DCPM_R_PENDING_ */

