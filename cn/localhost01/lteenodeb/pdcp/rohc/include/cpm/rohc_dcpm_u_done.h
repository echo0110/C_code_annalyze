/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dcpm_u_done.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function prototypes for U-mode
 *         decompressor CPM, when decompressor transition state is DONE.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dcpm_u_done.h,v $
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


#ifndef _ROHC_DCPM_U_DONE_
#define _ROHC_DCPM_U_DONE_


/* + SPR 17439 */
rohc_void_t rohc_d_u_ir_nc_dn ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                  *p_dsc));

rohc_void_t rohc_d_u_ir_irdyn_uor2_sc_dn ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                  *p_dsc));

rohc_void_t rohc_d_u_all_fc_dn ROHC_PROTO((rohc_entity_t *p_entity , rohc_dsc_t
                                                                  *p_dsc));


rohc_void_t rohc_d_u_ack_u_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));


rohc_void_t rohc_d_u_mt_u_to_o_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

rohc_void_t rohc_d_u_mt_u_to_r_dn ROHC_PROTO((rohc_entity_t *p_entity ,
                                                       rohc_dsc_t *p_dsc));

/* - SPR 17439 */

#endif /* _ROHC_DCPM_U_DONE_ */

