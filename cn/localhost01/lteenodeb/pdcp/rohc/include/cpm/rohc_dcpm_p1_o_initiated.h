/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dcpm_p1_o_initiated.h,v 1.1.2.1 2010/11/09 04:37:56 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function prototypes for O-mode
 *        decompressor CPM, when decompressor transition state is INITIATED.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dcpm_p1_o_initiated.h,v $
 * Revision 1.1.2.1  2010/11/09 04:37:56  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.1.2.1  2010/10/04 03:36:29  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_DCPM_P1_O_INITIATED_
#define _ROHC_DCPM_P1_O_INITIATED_


/* + SPR 17439 */
rohc_void_t rohc_d_p1_o_ir_nc_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_rtp_t
                                                                   *p_dsc));

rohc_void_t rohc_d_p1_o_ir_sc_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_rtp_t
                                                                   *p_dsc));

rohc_void_t rohc_d_p1_o_ir_fc_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_rtp_t
                                                                   *p_dsc));

rohc_void_t rohc_d_p1_o_irdyn_nc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_irdyn_sc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_irdyn_fc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_0_nc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_0_sc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_0_fc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_1_nc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_1_sc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_1_fc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_2_nc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_2_sc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_2_fc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_ack_o_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));


rohc_void_t rohc_d_p1_o_static_nack_o_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_nack_o_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));


rohc_void_t rohc_d_p1_o_mt_o_to_u_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));


rohc_void_t rohc_d_p1_o_mt_o_to_r_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_rtp_t *p_dsc));


/* - SPR 17439 */



#endif /* _ROHC_DCPM_O_INITIATED_ */

