/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dcpm_p1_o_done.h,v 1.1.2.1 2010/11/09 04:37:17 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function prototypes for O-mode
 *         decompressor CPM, when decompressor transition state is DONE.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dcpm_p1_o_done.h,v $
 * Revision 1.1.2.1  2010/11/09 04:37:17  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.1.2.1  2010/10/04 03:36:01  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_DCPM_P1_O_DONE_
#define _ROHC_DCPM_P1_O_DONE_


/* + SPR 17439 */
rohc_void_t rohc_d_p1_o_ir_nc_dn ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_rtp_t
                                                                  *p_dsc));

rohc_void_t rohc_d_p1_o_ir_sc_dn ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_rtp_t
                                                                  *p_dsc));

rohc_void_t rohc_d_p1_o_ir_fc_dn ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_rtp_t
                                                                  *p_dsc));

rohc_void_t rohc_d_p1_o_irdyn_nc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_irdyn_sc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_irdyn_fc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_0_nc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_0_sc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_0_fc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_1_nc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_1_sc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_1_fc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_2_nc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_2_sc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_uo_2_fc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_ack_o_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));


rohc_void_t rohc_d_p1_o_static_nack_o_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

    

rohc_void_t rohc_d_p1_o_nack_o_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_mt_o_to_u_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));

rohc_void_t rohc_d_p1_o_mt_o_to_r_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_rtp_t *p_dsc));


/* - SPR 17439 */


#endif /* _ROHC_DCPM_O_DONE_ */

