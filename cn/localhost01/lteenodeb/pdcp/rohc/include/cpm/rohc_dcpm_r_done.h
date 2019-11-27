/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dcpm_r_done.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function prototypes for R-mode
 *         decompressor CPM, when decompressor transition state is DONE.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dcpm_r_done.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/


#ifndef _ROHC_DCPM_R_DONE_
#define _ROHC_DCPM_R_DONE_


/* + SPR 17439 */
rohc_void_t rohc_d_r_ir_nc_dn ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                  *p_dsc));

rohc_void_t rohc_d_r_ir_sc_dn ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                  *p_dsc));

rohc_void_t rohc_d_r_ir_fc_dn ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                  *p_dsc));

rohc_void_t rohc_d_r_irdyn_nc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_irdyn_sc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_irdyn_fc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_0_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_0_crc_nc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_0_crc_fc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_0_crc_sc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_1_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_2_nc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_2_sc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_2_fc_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_ack_r_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_static_nack_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));


rohc_void_t rohc_d_r_nack_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_mt_r_to_u_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));


rohc_void_t rohc_d_r_mt_r_to_o_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));


/* - SPR 17439 */


#endif /* _ROHC_DCPM_R_DONE_ */

