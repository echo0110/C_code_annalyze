/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dcpm_r_pending.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
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
 * $Log: rohc_dcpm_r_pending.h,v $
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

#ifndef _ROHC_DCPM_R_PENDING_
#define _ROHC_DCPM_R_PENDING_


/* + SPR 17439 */
rohc_void_t rohc_d_r_ir_nc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_ir_sc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_ir_fc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_irdyn_nc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_irdyn_sc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_irdyn_fc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_0_nc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_0_sc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_0_fc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_0_crc_nc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_0_crc_sc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_0_crc_fc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_1_nc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_1_sc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_1_fc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_2_nc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_2_sc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_r_2_fc_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_r_ack_r_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));


rohc_void_t rohc_d_r_static_nack_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));



rohc_void_t rohc_d_r_nack_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));


rohc_void_t rohc_d_r_mt_r_to_u_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));


rohc_void_t rohc_d_r_mt_r_to_o_pnd ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));


/* - SPR 17439 */

#endif /* _ROHC_DCPM_R_PENDING_ */

