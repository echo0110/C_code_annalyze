/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dcpm_r_initiated.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function prototypes for R-mode
 *        decompressor CPM, when decompressor transition state is INITIATED.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dcpm_r_initiated.h,v $
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


#ifndef _ROHC_DCPM_R_INITIATED_
#define _ROHC_DCPM_R_INITIATED_


/* + SPR 17439 */
rohc_void_t rohc_d_r_ir_nc_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                   *p_dsc));

rohc_void_t rohc_d_r_ir_sc_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                   *p_dsc));

rohc_void_t rohc_d_r_ir_fc_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                   *p_dsc));

rohc_void_t rohc_d_r_irdyn_nc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_irdyn_sc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_irdyn_fc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_0_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                   *p_dsc));


rohc_void_t rohc_d_r_r_0_crc_nc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_0_crc_sc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_0_crc_fc_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_1_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                   *p_dsc));



rohc_void_t rohc_d_r_r_2_nc_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                   *p_dsc));

rohc_void_t rohc_d_r_r_2_sc_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                   *p_dsc));

rohc_void_t rohc_d_r_r_2_fc_in ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                   *p_dsc));

rohc_void_t rohc_d_r_ack_r_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_t *p_dsc));


rohc_void_t rohc_d_r_static_nack_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_t *p_dsc));


rohc_void_t rohc_d_r_nack_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_t *p_dsc));


rohc_void_t rohc_d_r_mt_r_to_u_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_t *p_dsc));


rohc_void_t rohc_d_r_mt_r_to_o_in ROHC_PROTO((rohc_entity_t *p_entity ,
                                                        rohc_dsc_t *p_dsc));


/* - SPR 17439 */
#endif /* _ROHC_DCPM_R_INITIATED_ */

